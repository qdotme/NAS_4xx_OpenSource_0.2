/*-------------------------------------------------------------------------
 *
 * functions.c
 *	  Execution of SQL-language functions
 *
 * Portions Copyright (c) 1996-2005, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $PostgreSQL: pgsql/src/backend/executor/functions.c,v 1.91.4.2 2006/10/12 17:02:34 tgl Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "access/heapam.h"
#include "catalog/pg_proc.h"
#include "catalog/pg_type.h"
#include "commands/trigger.h"
#include "executor/executor.h"
#include "executor/functions.h"
#include "parser/parse_coerce.h"
#include "parser/parse_expr.h"
#include "parser/parse_type.h"
#include "tcop/tcopprot.h"
#include "tcop/utility.h"
#include "utils/builtins.h"
#include "utils/datum.h"
#include "utils/lsyscache.h"
#include "utils/syscache.h"
#include "utils/typcache.h"


/*
 * We have an execution_state record for each query in a function.	Each
 * record contains a querytree and plantree for its query.	If the query
 * is currently in F_EXEC_RUN state then there's a QueryDesc too.
 */
typedef enum
{
	F_EXEC_START, F_EXEC_RUN, F_EXEC_DONE
} ExecStatus;

typedef struct local_es
{
	struct local_es *next;
	ExecStatus	status;
	Query	   *query;
	Plan	   *plan;
	QueryDesc  *qd;				/* null unless status == RUN */
} execution_state;

#define LAST_POSTQUEL_COMMAND(es) ((es)->next == NULL)


/*
 * An SQLFunctionCache record is built during the first call,
 * and linked to from the fn_extra field of the FmgrInfo struct.
 */
typedef struct
{
	Oid		   *argtypes;		/* resolved types of arguments */
	Oid			rettype;		/* actual return type */
	int			typlen;			/* length of the return type */
	bool		typbyval;		/* true if return type is pass by value */
	bool		returnsTuple;	/* true if returning whole tuple result */
	bool		shutdown_reg;	/* true if registered shutdown callback */
	bool		readonly_func;	/* true to run in "read only" mode */

	ParamListInfo paramLI;		/* Param list representing current args */

	JunkFilter *junkFilter;		/* used only if returnsTuple */

	/* head of linked list of execution_state records */
	execution_state *func_state;
} SQLFunctionCache;

typedef SQLFunctionCache *SQLFunctionCachePtr;


/* non-export function prototypes */
static execution_state *init_execution_state(List *queryTree_list,
											 bool readonly_func);
static void init_sql_fcache(FmgrInfo *finfo);
static void postquel_start(execution_state *es, SQLFunctionCachePtr fcache);
static TupleTableSlot *postquel_getnext(execution_state *es);
static void postquel_end(execution_state *es);
static void postquel_sub_params(SQLFunctionCachePtr fcache,
					FunctionCallInfo fcinfo);
static Datum postquel_execute(execution_state *es,
				 FunctionCallInfo fcinfo,
				 SQLFunctionCachePtr fcache,
				 MemoryContext resultcontext);
static void sql_exec_error_callback(void *arg);
static void ShutdownSQLFunction(Datum arg);


static execution_state *
init_execution_state(List *queryTree_list, bool readonly_func)
{
	execution_state *firstes = NULL;
	execution_state *preves = NULL;
	ListCell   *qtl_item;

	foreach(qtl_item, queryTree_list)
	{
		Query	   *queryTree = lfirst(qtl_item);
		Plan	   *planTree;
		execution_state *newes;

		/* Precheck all commands for validity in a function */
		if (queryTree->commandType == CMD_UTILITY &&
			IsA(queryTree->utilityStmt, TransactionStmt))
			ereport(ERROR,
					(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
					 /* translator: %s is a SQL statement name */
					 errmsg("%s is not allowed in a SQL function",
							CreateQueryTag(queryTree))));

		if (readonly_func && !QueryIsReadOnly(queryTree))
			ereport(ERROR,
					(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
					 /* translator: %s is a SQL statement name */
					 errmsg("%s is not allowed in a non-volatile function",
							CreateQueryTag(queryTree))));

		planTree = pg_plan_query(queryTree, NULL);

		newes = (execution_state *) palloc(sizeof(execution_state));
		if (preves)
			preves->next = newes;
		else
			firstes = newes;

		newes->next = NULL;
		newes->status = F_EXEC_START;
		newes->query = queryTree;
		newes->plan = planTree;
		newes->qd = NULL;

		preves = newes;
	}

	return firstes;
}


static void
init_sql_fcache(FmgrInfo *finfo)
{
	Oid			foid = finfo->fn_oid;
	Oid			rettype;
	HeapTuple	procedureTuple;
	HeapTuple	typeTuple;
	Form_pg_proc procedureStruct;
	Form_pg_type typeStruct;
	SQLFunctionCachePtr fcache;
	Oid		   *argOidVect;
	bool		haspolyarg;
	char	   *src;
	int			nargs;
	List	   *queryTree_list;
	Datum		tmp;
	bool		isNull;

	fcache = (SQLFunctionCachePtr) palloc0(sizeof(SQLFunctionCache));

	/*
	 * get the procedure tuple corresponding to the given function Oid
	 */
	procedureTuple = SearchSysCache(PROCOID,
									ObjectIdGetDatum(foid),
									0, 0, 0);
	if (!HeapTupleIsValid(procedureTuple))
		elog(ERROR, "cache lookup failed for function %u", foid);
	procedureStruct = (Form_pg_proc) GETSTRUCT(procedureTuple);

	/*
	 * get the result type from the procedure tuple, and check for
	 * polymorphic result type; if so, find out the actual result type.
	 */
	rettype = procedureStruct->prorettype;

	if (rettype == ANYARRAYOID || rettype == ANYELEMENTOID)
	{
		rettype = get_fn_expr_rettype(finfo);
		if (rettype == InvalidOid)		/* this probably should not happen */
			ereport(ERROR,
					(errcode(ERRCODE_DATATYPE_MISMATCH),
					 errmsg("could not determine actual result type for function declared to return type %s",
						  format_type_be(procedureStruct->prorettype))));
	}

	fcache->rettype = rettype;

	/* Remember if function is STABLE/IMMUTABLE */
	fcache->readonly_func =
		(procedureStruct->provolatile != PROVOLATILE_VOLATILE);

	/* Now look up the actual result type */
	typeTuple = SearchSysCache(TYPEOID,
							   ObjectIdGetDatum(rettype),
							   0, 0, 0);
	if (!HeapTupleIsValid(typeTuple))
		elog(ERROR, "cache lookup failed for type %u", rettype);
	typeStruct = (Form_pg_type) GETSTRUCT(typeTuple);

	/*
	 * get the type length and by-value flag from the type tuple; also do
	 * a preliminary check for returnsTuple (this may prove inaccurate,
	 * see below).
	 */
	fcache->typlen = typeStruct->typlen;
	fcache->typbyval = typeStruct->typbyval;
	fcache->returnsTuple = (typeStruct->typtype == 'c' ||
							rettype == RECORDOID);

	/*
	 * Parse and rewrite the queries.  We need the argument type info to
	 * pass to the parser.
	 */
	nargs = procedureStruct->pronargs;
	haspolyarg = false;

	if (nargs > 0)
	{
		int			argnum;

		argOidVect = (Oid *) palloc(nargs * sizeof(Oid));
		memcpy(argOidVect,
			   procedureStruct->proargtypes,
			   nargs * sizeof(Oid));
		/* Resolve any polymorphic argument types */
		for (argnum = 0; argnum < nargs; argnum++)
		{
			Oid			argtype = argOidVect[argnum];

			if (argtype == ANYARRAYOID || argtype == ANYELEMENTOID)
			{
				argtype = get_fn_expr_argtype(finfo, argnum);
				if (argtype == InvalidOid)
					ereport(ERROR,
							(errcode(ERRCODE_DATATYPE_MISMATCH),
							 errmsg("could not determine actual type of argument declared %s",
									format_type_be(argOidVect[argnum]))));
				argOidVect[argnum] = argtype;
				haspolyarg = true;
			}
		}
	}
	else
		argOidVect = NULL;
	fcache->argtypes = argOidVect;

	tmp = SysCacheGetAttr(PROCOID,
						  procedureTuple,
						  Anum_pg_proc_prosrc,
						  &isNull);
	if (isNull)
		elog(ERROR, "null prosrc for function %u", foid);
	src = DatumGetCString(DirectFunctionCall1(textout, tmp));

	queryTree_list = pg_parse_and_rewrite(src, argOidVect, nargs);

	/*
	 * If the function has any arguments declared as polymorphic types,
	 * then it wasn't type-checked at definition time; must do so now.
	 *
	 * Also, force a type-check if the declared return type is a rowtype; we
	 * need to find out whether we are actually returning the whole tuple
	 * result, or just regurgitating a rowtype expression result. In the
	 * latter case we clear returnsTuple because we need not act different
	 * from the scalar result case.
	 *
	 * In the returnsTuple case, check_sql_fn_retval will also construct
	 * a JunkFilter we can use to coerce the returned rowtype to the desired
	 * form.
	 */
	if (haspolyarg || fcache->returnsTuple)
		fcache->returnsTuple = check_sql_fn_retval(rettype,
												   get_typtype(rettype),
												   queryTree_list,
												   &fcache->junkFilter);

	/* Finally, plan the queries */
	fcache->func_state = init_execution_state(queryTree_list,
											  fcache->readonly_func);

	pfree(src);

	ReleaseSysCache(typeTuple);
	ReleaseSysCache(procedureTuple);

	finfo->fn_extra = (void *) fcache;
}


static void
postquel_start(execution_state *es, SQLFunctionCachePtr fcache)
{
	Snapshot	snapshot;

	Assert(es->qd == NULL);

	/*
	 * In a read-only function, use the surrounding query's snapshot;
	 * otherwise take a new snapshot for each query.  The snapshot should
	 * include a fresh command ID so that all work to date in this
	 * transaction is visible.  We copy in both cases so that postquel_end
	 * can unconditionally do FreeSnapshot.
	 */
	if (fcache->readonly_func)
		snapshot = CopySnapshot(ActiveSnapshot);
	else
	{
		CommandCounterIncrement();
		snapshot = CopySnapshot(GetTransactionSnapshot());
	}

	es->qd = CreateQueryDesc(es->query, es->plan,
							 snapshot, InvalidSnapshot,
							 None_Receiver,
							 fcache->paramLI, false);

	/* We assume we don't need to set up ActiveSnapshot for ExecutorStart */

	/* Utility commands don't need Executor. */
	if (es->qd->operation != CMD_UTILITY)
	{
		/*
		 * Only set up to collect queued triggers if it's not a SELECT.
		 * This isn't just an optimization, but is necessary in case a SELECT
		 * returns multiple rows to caller --- we mustn't exit from the
		 * function execution with a stacked AfterTrigger level still active.
		 */
		if (es->qd->operation != CMD_SELECT)
			AfterTriggerBeginQuery();
		ExecutorStart(es->qd, false);
	}

	es->status = F_EXEC_RUN;
}

static TupleTableSlot *
postquel_getnext(execution_state *es)
{
	TupleTableSlot *result;
	Snapshot	saveActiveSnapshot;
	long		count;

	/* Make our snapshot the active one for any called functions */
	saveActiveSnapshot = ActiveSnapshot;
	PG_TRY();
	{
		ActiveSnapshot = es->qd->snapshot;

		if (es->qd->operation == CMD_UTILITY)
		{
			ProcessUtility(es->qd->parsetree->utilityStmt, es->qd->params,
						   es->qd->dest, NULL);
			result = NULL;
		}
		else
		{
			/*
			 * If it's the function's last command, and it's a SELECT, fetch
			 * one row at a time so we can return the results. Otherwise just
			 * run it to completion.  (If we run to completion then
			 * ExecutorRun is guaranteed to return NULL.)
			 */
			if (LAST_POSTQUEL_COMMAND(es) && es->qd->operation == CMD_SELECT)
				count = 1L;
			else
				count = 0L;

			result = ExecutorRun(es->qd, ForwardScanDirection, count);
		}
	}
	PG_CATCH();
	{
		/* Restore global vars and propagate error */
		ActiveSnapshot = saveActiveSnapshot;
		PG_RE_THROW();
	}
	PG_END_TRY();

	ActiveSnapshot = saveActiveSnapshot;

	return result;
}

static void
postquel_end(execution_state *es)
{
	Snapshot	saveActiveSnapshot;

	/* mark status done to ensure we don't do ExecutorEnd twice */
	es->status = F_EXEC_DONE;

	/* Utility commands don't need Executor. */
	if (es->qd->operation != CMD_UTILITY)
	{
		/* Make our snapshot the active one for any called functions */
		saveActiveSnapshot = ActiveSnapshot;
		PG_TRY();
		{
			ActiveSnapshot = es->qd->snapshot;

			ExecutorEnd(es->qd);
			if (es->qd->operation != CMD_SELECT)
				AfterTriggerEndQuery();
		}
		PG_CATCH();
		{
			/* Restore global vars and propagate error */
			ActiveSnapshot = saveActiveSnapshot;
			PG_RE_THROW();
		}
		PG_END_TRY();
		ActiveSnapshot = saveActiveSnapshot;
	}

	FreeSnapshot(es->qd->snapshot);
	FreeQueryDesc(es->qd);
	es->qd = NULL;
}

/* Build ParamListInfo array representing current arguments */
static void
postquel_sub_params(SQLFunctionCachePtr fcache,
					FunctionCallInfo fcinfo)
{
	ParamListInfo paramLI;
	int			nargs = fcinfo->nargs;

	if (nargs > 0)
	{
		int			i;

		paramLI = (ParamListInfo) palloc0((nargs + 1) * sizeof(ParamListInfoData));

		for (i = 0; i < nargs; i++)
		{
			paramLI[i].kind = PARAM_NUM;
			paramLI[i].id = i + 1;
			paramLI[i].ptype = fcache->argtypes[i];
			paramLI[i].value = fcinfo->arg[i];
			paramLI[i].isnull = fcinfo->argnull[i];
		}
		paramLI[nargs].kind = PARAM_INVALID;
	}
	else
		paramLI = NULL;

	if (fcache->paramLI)
		pfree(fcache->paramLI);

	fcache->paramLI = paramLI;
}

static Datum
postquel_execute(execution_state *es,
				 FunctionCallInfo fcinfo,
				 SQLFunctionCachePtr fcache,
				 MemoryContext resultcontext)
{
	TupleTableSlot *slot;
	HeapTuple	tup;
	TupleDesc	tupDesc;
	Datum		value;
	MemoryContext oldcontext;

	if (es->status == F_EXEC_START)
		postquel_start(es, fcache);

	slot = postquel_getnext(es);

	if (TupIsNull(slot))
	{
		/*
		 * We fall out here for all cases except where we have obtained
		 * a row from a function's final SELECT.
		 */
		postquel_end(es);
		fcinfo->isnull = true;
		return (Datum) NULL;
	}

	/*
	 * If we got a row from a command within the function it has to be
	 * the final command.  All others shouldn't be returning anything.
	 */
	Assert(LAST_POSTQUEL_COMMAND(es));

	/*
	 * Set up to return the function value.  For pass-by-reference
	 * datatypes, be sure to allocate the result in resultcontext,
	 * not the current memory context (which has query lifespan).
	 */
	oldcontext = MemoryContextSwitchTo(resultcontext);

	if (fcache->returnsTuple)
	{
		/*
		 * We are returning the whole tuple, so filter it and apply the
		 * proper labeling to make it a valid Datum.  There are several
		 * reasons why we do this:
		 *
		 * 1. To copy the tuple out of the child execution context and
		 * into the desired result context.
		 *
		 * 2. To remove any junk attributes present in the raw subselect
		 * result.  (This is probably not absolutely necessary, but it
		 * seems like good policy.)
		 *
		 * 3. To insert dummy null columns if the declared result type
		 * has any attisdropped columns.
		 */
		HeapTuple	newtup;
		HeapTupleHeader dtup;
		uint32		t_len;
		Oid			dtuptype;
		int32		dtuptypmod;

		newtup = ExecRemoveJunk(fcache->junkFilter, slot);

		/*
		 * Compress out the HeapTuple header data.  We assume that
		 * heap_formtuple made the tuple with header and body in one
		 * palloc'd chunk.  We want to return a pointer to the chunk
		 * start so that it will work if someone tries to free it.
		 */
		t_len = newtup->t_len;
		dtup = (HeapTupleHeader) newtup;
		memmove((char *) dtup, (char *) newtup->t_data, t_len);

		/*
		 * Use the declared return type if it's not RECORD; else take
		 * the type from the computed result, making sure a typmod has
		 * been assigned.
		 */
		if (fcache->rettype != RECORDOID)
		{
			/* function has a named composite return type */
			dtuptype = fcache->rettype;
			dtuptypmod = -1;
		}
		else
		{
			/* function is declared to return RECORD */
			tupDesc = fcache->junkFilter->jf_cleanTupType;
			if (tupDesc->tdtypeid == RECORDOID &&
				tupDesc->tdtypmod < 0)
				assign_record_type_typmod(tupDesc);
			dtuptype = tupDesc->tdtypeid;
			dtuptypmod = tupDesc->tdtypmod;
		}

		HeapTupleHeaderSetDatumLength(dtup, t_len);
		HeapTupleHeaderSetTypeId(dtup, dtuptype);
		HeapTupleHeaderSetTypMod(dtup, dtuptypmod);

		value = PointerGetDatum(dtup);
		fcinfo->isnull = false;
	}
	else
	{
		/*
		 * Returning a scalar, which we have to extract from the first
		 * column of the SELECT result, and then copy into result
		 * context if needed.
		 */
		tup = slot->val;
		tupDesc = slot->ttc_tupleDescriptor;

		value = heap_getattr(tup, 1, tupDesc, &(fcinfo->isnull));

		if (!fcinfo->isnull)
			value = datumCopy(value, fcache->typbyval, fcache->typlen);
	}

	MemoryContextSwitchTo(oldcontext);

	/*
	 * If this is a single valued function we have to end the function
	 * execution now.
	 */
	if (!fcinfo->flinfo->fn_retset)
		postquel_end(es);

	return value;
}

Datum
fmgr_sql(PG_FUNCTION_ARGS)
{
	MemoryContext oldcontext;
	SQLFunctionCachePtr fcache;
	ErrorContextCallback sqlerrcontext;
	execution_state *es;
	Datum		result = 0;

	/*
	 * Switch to context in which the fcache lives.  This ensures that
	 * parsetrees, plans, etc, will have sufficient lifetime.  The
	 * sub-executor is responsible for deleting per-tuple information.
	 */
	oldcontext = MemoryContextSwitchTo(fcinfo->flinfo->fn_mcxt);

	/*
	 * Setup error traceback support for ereport()
	 */
	sqlerrcontext.callback = sql_exec_error_callback;
	sqlerrcontext.arg = fcinfo->flinfo;
	sqlerrcontext.previous = error_context_stack;
	error_context_stack = &sqlerrcontext;

	/*
	 * Initialize fcache (build plans) if first time through.
	 */
	fcache = (SQLFunctionCachePtr) fcinfo->flinfo->fn_extra;
	if (fcache == NULL)
	{
		init_sql_fcache(fcinfo->flinfo);
		fcache = (SQLFunctionCachePtr) fcinfo->flinfo->fn_extra;
	}
	es = fcache->func_state;

	/*
	 * Convert params to appropriate format if starting a fresh execution.
	 * (If continuing execution, we can re-use prior params.)
	 */
	if (es && es->status == F_EXEC_START)
		postquel_sub_params(fcache, fcinfo);

	/*
	 * Find first unfinished query in function.
	 */
	while (es && es->status == F_EXEC_DONE)
		es = es->next;

	/*
	 * Execute each command in the function one after another until we're
	 * executing the final command and get a result or we run out of
	 * commands.
	 */
	while (es)
	{
		result = postquel_execute(es, fcinfo, fcache, oldcontext);
		if (es->status != F_EXEC_DONE)
			break;
		es = es->next;
	}

	/*
	 * If we've gone through every command in this function, we are done.
	 */
	if (es == NULL)
	{
		/*
		 * Reset the execution states to start over again on next call.
		 */
		es = fcache->func_state;
		while (es)
		{
			es->status = F_EXEC_START;
			es = es->next;
		}

		/*
		 * Let caller know we're finished.
		 */
		if (fcinfo->flinfo->fn_retset)
		{
			ReturnSetInfo *rsi = (ReturnSetInfo *) fcinfo->resultinfo;

			if (rsi && IsA(rsi, ReturnSetInfo))
				rsi->isDone = ExprEndResult;
			else
				ereport(ERROR,
						(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
						 errmsg("set-valued function called in context that cannot accept a set")));
			fcinfo->isnull = true;
			result = (Datum) 0;

			/* Deregister shutdown callback, if we made one */
			if (fcache->shutdown_reg)
			{
				UnregisterExprContextCallback(rsi->econtext,
											  ShutdownSQLFunction,
											  PointerGetDatum(fcache));
				fcache->shutdown_reg = false;
			}
		}

		error_context_stack = sqlerrcontext.previous;

		MemoryContextSwitchTo(oldcontext);

		return result;
	}

	/*
	 * If we got a result from a command within the function it has to be
	 * the final command.  All others shouldn't be returning anything.
	 */
	Assert(LAST_POSTQUEL_COMMAND(es));

	/*
	 * Let caller know we're not finished.
	 */
	if (fcinfo->flinfo->fn_retset)
	{
		ReturnSetInfo *rsi = (ReturnSetInfo *) fcinfo->resultinfo;

		if (rsi && IsA(rsi, ReturnSetInfo))
			rsi->isDone = ExprMultipleResult;
		else
			ereport(ERROR,
					(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
					 errmsg("set-valued function called in context that cannot accept a set")));

		/*
		 * Ensure we will get shut down cleanly if the exprcontext is not
		 * run to completion.
		 */
		if (!fcache->shutdown_reg)
		{
			RegisterExprContextCallback(rsi->econtext,
										ShutdownSQLFunction,
										PointerGetDatum(fcache));
			fcache->shutdown_reg = true;
		}
	}

	error_context_stack = sqlerrcontext.previous;

	MemoryContextSwitchTo(oldcontext);

	return result;
}


/*
 * error context callback to let us supply a call-stack traceback
 */
static void
sql_exec_error_callback(void *arg)
{
	FmgrInfo   *flinfo = (FmgrInfo *) arg;
	SQLFunctionCachePtr fcache = (SQLFunctionCachePtr) flinfo->fn_extra;
	HeapTuple	func_tuple;
	Form_pg_proc functup;
	char	   *fn_name;
	int			syntaxerrposition;

	/* Need access to function's pg_proc tuple */
	func_tuple = SearchSysCache(PROCOID,
								ObjectIdGetDatum(flinfo->fn_oid),
								0, 0, 0);
	if (!HeapTupleIsValid(func_tuple))
		return;					/* shouldn't happen */
	functup = (Form_pg_proc) GETSTRUCT(func_tuple);
	fn_name = NameStr(functup->proname);

	/*
	 * If there is a syntax error position, convert to internal syntax
	 * error
	 */
	syntaxerrposition = geterrposition();
	if (syntaxerrposition > 0)
	{
		bool		isnull;
		Datum		tmp;
		char	   *prosrc;

		tmp = SysCacheGetAttr(PROCOID, func_tuple, Anum_pg_proc_prosrc,
							  &isnull);
		if (isnull)
			elog(ERROR, "null prosrc");
		prosrc = DatumGetCString(DirectFunctionCall1(textout, tmp));
		errposition(0);
		internalerrposition(syntaxerrposition);
		internalerrquery(prosrc);
		pfree(prosrc);
	}

	/*
	 * Try to determine where in the function we failed.  If there is a
	 * query with non-null QueryDesc, finger it.  (We check this rather
	 * than looking for F_EXEC_RUN state, so that errors during
	 * ExecutorStart or ExecutorEnd are blamed on the appropriate query;
	 * see postquel_start and postquel_end.)
	 */
	if (fcache)
	{
		execution_state *es;
		int			query_num;

		es = fcache->func_state;
		query_num = 1;
		while (es)
		{
			if (es->qd)
			{
				errcontext("SQL function \"%s\" statement %d",
						   fn_name, query_num);
				break;
			}
			es = es->next;
			query_num++;
		}
		if (es == NULL)
		{
			/*
			 * couldn't identify a running query; might be function entry,
			 * function exit, or between queries.
			 */
			errcontext("SQL function \"%s\"", fn_name);
		}
	}
	else
	{
		/* must have failed during init_sql_fcache() */
		errcontext("SQL function \"%s\" during startup", fn_name);
	}

	ReleaseSysCache(func_tuple);
}


/*
 * callback function in case a function-returning-set needs to be shut down
 * before it has been run to completion
 */
static void
ShutdownSQLFunction(Datum arg)
{
	SQLFunctionCachePtr fcache = (SQLFunctionCachePtr) DatumGetPointer(arg);
	execution_state *es = fcache->func_state;

	while (es != NULL)
	{
		/* Shut down anything still running */
		if (es->status == F_EXEC_RUN)
			postquel_end(es);
		/* Reset states to START in case we're called again */
		es->status = F_EXEC_START;
		es = es->next;
	}

	/* execUtils will deregister the callback... */
	fcache->shutdown_reg = false;
}


/*
 * check_sql_fn_retval() -- check return value of a list of sql parse trees.
 *
 * The return value of a sql function is the value returned by
 * the final query in the function.  We do some ad-hoc type checking here
 * to be sure that the user is returning the type he claims.
 *
 * This is normally applied during function definition, but in the case
 * of a function with polymorphic arguments, we instead apply it during
 * function execution startup.	The rettype is then the actual resolved
 * output type of the function, rather than the declared type.	(Therefore,
 * we should never see ANYARRAY or ANYELEMENT as rettype.)
 *
 * The return value is true if the function returns the entire tuple result
 * of its final SELECT, and false otherwise.  Note that because we allow
 * "SELECT rowtype_expression", this may be false even when the declared
 * function return type is a rowtype.
 *
 * If junkFilter isn't NULL, then *junkFilter is set to a JunkFilter defined
 * to convert the function's tuple result to the correct output tuple type.
 * Whenever the result value is false (ie, the function isn't returning a
 * tuple result), *junkFilter is set to NULL.
 */
bool
check_sql_fn_retval(Oid rettype, char fn_typtype, List *queryTreeList,
					JunkFilter **junkFilter)
{
	Query	   *parse;
	int			cmd;
	List	   *tlist;
	ListCell   *tlistitem;
	int			tlistlen;
	Oid			typerelid;
	Oid			restype;
	Relation	reln;
	int			relnatts;		/* physical number of columns in rel */
	int			rellogcols;		/* # of nondeleted columns in rel */
	int			colindex;		/* physical column index */

	if (junkFilter)
		*junkFilter = NULL;		/* default result */

	/* guard against empty function body; OK only if void return type */
	if (queryTreeList == NIL)
	{
		if (rettype != VOIDOID)
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_FUNCTION_DEFINITION),
					 errmsg("return type mismatch in function declared to return %s",
							format_type_be(rettype)),
			 errdetail("Function's final statement must be a SELECT.")));
		return false;
	}

	/* find the final query */
	parse = (Query *) lfirst(list_tail(queryTreeList));

	cmd = parse->commandType;
	tlist = parse->targetList;

	/*
	 * The last query must be a SELECT if and only if return type isn't
	 * VOID.
	 */
	if (rettype == VOIDOID)
	{
		if (cmd == CMD_SELECT)
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_FUNCTION_DEFINITION),
					 errmsg("return type mismatch in function declared to return %s",
							format_type_be(rettype)),
					 errdetail("Function's final statement must not be a SELECT.")));
		return false;
	}

	/* by here, the function is declared to return some type */
	if (cmd != CMD_SELECT)
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_FUNCTION_DEFINITION),
		 errmsg("return type mismatch in function declared to return %s",
				format_type_be(rettype)),
			 errdetail("Function's final statement must be a SELECT.")));

	/*
	 * Count the non-junk entries in the result targetlist.
	 */
	tlistlen = ExecCleanTargetListLength(tlist);

	typerelid = typeidTypeRelid(rettype);

	if (fn_typtype == 'b' || fn_typtype == 'd')
	{
		/* Shouldn't have a typerelid */
		Assert(typerelid == InvalidOid);

		/*
		 * For base-type returns, the target list should have exactly one
		 * entry, and its type should agree with what the user declared.
		 * (As of Postgres 7.2, we accept binary-compatible types too.)
		 */
		if (tlistlen != 1)
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_FUNCTION_DEFINITION),
					 errmsg("return type mismatch in function declared to return %s",
							format_type_be(rettype)),
			 errdetail("Final SELECT must return exactly one column.")));

		restype = ((TargetEntry *) linitial(tlist))->resdom->restype;
		if (!IsBinaryCoercible(restype, rettype))
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_FUNCTION_DEFINITION),
					 errmsg("return type mismatch in function declared to return %s",
							format_type_be(rettype)),
					 errdetail("Actual return type is %s.",
							   format_type_be(restype))));
	}
	else if (fn_typtype == 'c')
	{
		/* Must have a typerelid */
		Assert(typerelid != InvalidOid);

		/*
		 * If the target list is of length 1, and the type of the varnode
		 * in the target list matches the declared return type, this is
		 * okay. This can happen, for example, where the body of the
		 * function is 'SELECT func2()', where func2 has the same return
		 * type as the function that's calling it.
		 */
		if (tlistlen == 1)
		{
			restype = ((TargetEntry *) linitial(tlist))->resdom->restype;
			if (IsBinaryCoercible(restype, rettype))
				return false;	/* NOT returning whole tuple */
		}

		/*
		 * Otherwise verify that the targetlist matches the return tuple
		 * type. This part of the typechecking is a hack. We look up the
		 * relation that is the declared return type, and scan the
		 * non-deleted attributes to ensure that they match the datatypes
		 * of the non-resjunk columns.
		 */
		reln = relation_open(typerelid, AccessShareLock);
		relnatts = reln->rd_rel->relnatts;
		rellogcols = 0;			/* we'll count nondeleted cols as we go */
		colindex = 0;

		foreach(tlistitem, tlist)
		{
			TargetEntry *tle = (TargetEntry *) lfirst(tlistitem);
			Form_pg_attribute attr;
			Oid			tletype;
			Oid			atttype;

			if (tle->resdom->resjunk)
				continue;

			do
			{
				colindex++;
				if (colindex > relnatts)
					ereport(ERROR,
							(errcode(ERRCODE_INVALID_FUNCTION_DEFINITION),
							 errmsg("return type mismatch in function declared to return %s",
									format_type_be(rettype)),
					errdetail("Final SELECT returns too many columns.")));
				attr = reln->rd_att->attrs[colindex - 1];
			} while (attr->attisdropped);
			rellogcols++;

			tletype = exprType((Node *) tle->expr);
			atttype = attr->atttypid;
			if (!IsBinaryCoercible(tletype, atttype))
				ereport(ERROR,
						(errcode(ERRCODE_INVALID_FUNCTION_DEFINITION),
						 errmsg("return type mismatch in function declared to return %s",
								format_type_be(rettype)),
						 errdetail("Final SELECT returns %s instead of %s at column %d.",
								   format_type_be(tletype),
								   format_type_be(atttype),
								   rellogcols)));
		}

		for (;;)
		{
			colindex++;
			if (colindex > relnatts)
				break;
			if (!reln->rd_att->attrs[colindex - 1]->attisdropped)
				rellogcols++;
		}

		if (tlistlen != rellogcols)
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_FUNCTION_DEFINITION),
					 errmsg("return type mismatch in function declared to return %s",
							format_type_be(rettype)),
					 errdetail("Final SELECT returns too few columns.")));

		/* Set up junk filter if needed */
		if (junkFilter)
			*junkFilter = ExecInitJunkFilterConversion(tlist,
											CreateTupleDescCopy(reln->rd_att),
											NULL);

		relation_close(reln, AccessShareLock);

		/* Report that we are returning entire tuple result */
		return true;
	}
	else if (rettype == RECORDOID)
	{
		/*
		 * If the target list is of length 1, and the type of the varnode
		 * in the target list matches the declared return type, this is
		 * okay. This can happen, for example, where the body of the
		 * function is 'SELECT func2()', where func2 has the same return
		 * type as the function that's calling it.
		 */
		if (tlistlen == 1)
		{
			restype = ((TargetEntry *) linitial(tlist))->resdom->restype;
			if (IsBinaryCoercible(restype, rettype))
				return false;	/* NOT returning whole tuple */
		}

		/*
		 * Otherwise assume we are returning the whole tuple.
		 * Crosschecking against what the caller expects will happen at
		 * runtime.
		 */
		if (junkFilter)
			*junkFilter = ExecInitJunkFilter(tlist, false, NULL);

		return true;
	}
	else if (rettype == ANYARRAYOID || rettype == ANYELEMENTOID)
	{
		/* This should already have been caught ... */
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_FUNCTION_DEFINITION),
				 errmsg("cannot determine result data type"),
				 errdetail("A function returning \"anyarray\" or \"anyelement\" must have at least one argument of either type.")));
	}
	else
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_FUNCTION_DEFINITION),
			  errmsg("return type %s is not supported for SQL functions",
					 format_type_be(rettype))));

	return false;
}
