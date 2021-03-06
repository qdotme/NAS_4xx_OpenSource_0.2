/*-------------------------------------------------------------------------
 *
 * schemacmds.c
 *	  schema creation/manipulation commands
 *
 * Portions Copyright (c) 1996-2005, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $PostgreSQL: pgsql/src/backend/commands/schemacmds.c,v 1.27 2004/12/31 21:59:41 pgsql Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "access/heapam.h"
#include "catalog/catalog.h"
#include "catalog/catname.h"
#include "catalog/dependency.h"
#include "catalog/indexing.h"
#include "catalog/namespace.h"
#include "catalog/pg_namespace.h"
#include "commands/dbcommands.h"
#include "commands/schemacmds.h"
#include "miscadmin.h"
#include "parser/analyze.h"
#include "tcop/utility.h"
#include "utils/acl.h"
#include "utils/builtins.h"
#include "utils/lsyscache.h"
#include "utils/syscache.h"


/*
 * CREATE SCHEMA
 */
void
CreateSchemaCommand(CreateSchemaStmt *stmt)
{
	const char *schemaName = stmt->schemaname;
	const char *authId = stmt->authid;
	Oid			namespaceId;
	List	   *parsetree_list;
	ListCell   *parsetree_item;
	const char *owner_name;
	AclId		owner_userid;
	AclId		saved_userid;
	AclResult	aclresult;

	saved_userid = GetUserId();

	/*
	 * Figure out user identities.
	 */

	if (!authId)
	{
		owner_userid = saved_userid;
		owner_name = GetUserNameFromId(owner_userid);
	}
	else if (superuser())
	{
		owner_name = authId;
		/* The following will error out if user does not exist */
		owner_userid = get_usesysid(owner_name);

		/*
		 * Set the current user to the requested authorization so that
		 * objects created in the statement have the requested owner.
		 * (This will revert to session user on error or at the end of
		 * this routine.)
		 */
		SetUserId(owner_userid);
	}
	else
	{
		/* not superuser */
		owner_userid = saved_userid;
		owner_name = GetUserNameFromId(owner_userid);
		if (strcmp(authId, owner_name) != 0)
			ereport(ERROR,
					(errcode(ERRCODE_INSUFFICIENT_PRIVILEGE),
					 errmsg("permission denied"),
					 errdetail("\"%s\" is not a superuser, so cannot create a schema for \"%s\"",
							   owner_name, authId)));
	}

	/*
	 * Permissions checks.
	 */
	aclresult = pg_database_aclcheck(MyDatabaseId, saved_userid, ACL_CREATE);
	if (aclresult != ACLCHECK_OK)
		aclcheck_error(aclresult, ACL_KIND_DATABASE,
					   get_database_name(MyDatabaseId));

	if (!allowSystemTableMods && IsReservedName(schemaName))
		ereport(ERROR,
				(errcode(ERRCODE_RESERVED_NAME),
				 errmsg("unacceptable schema name \"%s\"", schemaName),
		errdetail("The prefix \"pg_\" is reserved for system schemas.")));

	/* Create the schema's namespace */
	namespaceId = NamespaceCreate(schemaName, owner_userid);

	/* Advance cmd counter to make the namespace visible */
	CommandCounterIncrement();

	/*
	 * Temporarily make the new namespace be the front of the search path,
	 * as well as the default creation target namespace.  This will be
	 * undone at the end of this routine, or upon error.
	 */
	PushSpecialNamespace(namespaceId);

	/*
	 * Examine the list of commands embedded in the CREATE SCHEMA command,
	 * and reorganize them into a sequentially executable order with no
	 * forward references.	Note that the result is still a list of raw
	 * parsetrees in need of parse analysis --- we cannot, in general, run
	 * analyze.c on one statement until we have actually executed the
	 * prior ones.
	 */
	parsetree_list = analyzeCreateSchemaStmt(stmt);

	/*
	 * Analyze and execute each command contained in the CREATE SCHEMA
	 */
	foreach(parsetree_item, parsetree_list)
	{
		Node	   *parsetree = (Node *) lfirst(parsetree_item);
		List	   *querytree_list;
		ListCell   *querytree_item;

		querytree_list = parse_analyze(parsetree, NULL, 0);

		foreach(querytree_item, querytree_list)
		{
			Query	   *querytree = (Query *) lfirst(querytree_item);

			/* schemas should contain only utility stmts */
			Assert(querytree->commandType == CMD_UTILITY);
			/* do this step */
			ProcessUtility(querytree->utilityStmt, NULL, None_Receiver, NULL);
			/* make sure later steps can see the object created here */
			CommandCounterIncrement();
		}
	}

	/* Reset search path to normal state */
	PopSpecialNamespace(namespaceId);

	/* Reset current user */
	SetUserId(saved_userid);
}


/*
 *	RemoveSchema
 *		Removes a schema.
 */
void
RemoveSchema(List *names, DropBehavior behavior)
{
	char	   *namespaceName;
	Oid			namespaceId;
	ObjectAddress object;

	if (list_length(names) != 1)
		ereport(ERROR,
				(errcode(ERRCODE_SYNTAX_ERROR),
				 errmsg("schema name may not be qualified")));
	namespaceName = strVal(linitial(names));

	namespaceId = GetSysCacheOid(NAMESPACENAME,
								 CStringGetDatum(namespaceName),
								 0, 0, 0);
	if (!OidIsValid(namespaceId))
		ereport(ERROR,
				(errcode(ERRCODE_UNDEFINED_SCHEMA),
				 errmsg("schema \"%s\" does not exist", namespaceName)));

	/* Permission check */
	if (!pg_namespace_ownercheck(namespaceId, GetUserId()))
		aclcheck_error(ACLCHECK_NOT_OWNER, ACL_KIND_NAMESPACE,
					   namespaceName);

	/*
	 * Do the deletion.  Objects contained in the schema are removed by
	 * means of their dependency links to the schema.
	 */
	object.classId = get_system_catalog_relid(NamespaceRelationName);
	object.objectId = namespaceId;
	object.objectSubId = 0;

	performDeletion(&object, behavior);
}


/*
 * Guts of schema deletion.
 */
void
RemoveSchemaById(Oid schemaOid)
{
	Relation	relation;
	HeapTuple	tup;

	relation = heap_openr(NamespaceRelationName, RowExclusiveLock);

	tup = SearchSysCache(NAMESPACEOID,
						 ObjectIdGetDatum(schemaOid),
						 0, 0, 0);
	if (!HeapTupleIsValid(tup)) /* should not happen */
		elog(ERROR, "cache lookup failed for namespace %u", schemaOid);

	simple_heap_delete(relation, &tup->t_self);

	ReleaseSysCache(tup);

	heap_close(relation, RowExclusiveLock);
}


/*
 * Rename schema
 */
void
RenameSchema(const char *oldname, const char *newname)
{
	HeapTuple	tup;
	Relation	rel;
	AclResult	aclresult;

	rel = heap_openr(NamespaceRelationName, RowExclusiveLock);

	tup = SearchSysCacheCopy(NAMESPACENAME,
							 CStringGetDatum(oldname),
							 0, 0, 0);
	if (!HeapTupleIsValid(tup))
		ereport(ERROR,
				(errcode(ERRCODE_UNDEFINED_SCHEMA),
				 errmsg("schema \"%s\" does not exist", oldname)));

	/* make sure the new name doesn't exist */
	if (HeapTupleIsValid(
						 SearchSysCache(NAMESPACENAME,
										CStringGetDatum(newname),
										0, 0, 0)))
		ereport(ERROR,
				(errcode(ERRCODE_DUPLICATE_SCHEMA),
				 errmsg("schema \"%s\" already exists", newname)));

	/* must be owner */
	if (!pg_namespace_ownercheck(HeapTupleGetOid(tup), GetUserId()))
		aclcheck_error(ACLCHECK_NOT_OWNER, ACL_KIND_NAMESPACE,
					   oldname);

	/* must have CREATE privilege on database */
	aclresult = pg_database_aclcheck(MyDatabaseId, GetUserId(), ACL_CREATE);
	if (aclresult != ACLCHECK_OK)
		aclcheck_error(aclresult, ACL_KIND_DATABASE,
					   get_database_name(MyDatabaseId));

	if (!allowSystemTableMods && IsReservedName(newname))
		ereport(ERROR,
				(errcode(ERRCODE_RESERVED_NAME),
				 errmsg("unacceptable schema name \"%s\"", newname),
		errdetail("The prefix \"pg_\" is reserved for system schemas.")));

	/* rename */
	namestrcpy(&(((Form_pg_namespace) GETSTRUCT(tup))->nspname), newname);
	simple_heap_update(rel, &tup->t_self, tup);
	CatalogUpdateIndexes(rel, tup);

	heap_close(rel, NoLock);
	heap_freetuple(tup);
}

/*
 * Change schema owner
 */
void
AlterSchemaOwner(const char *name, AclId newOwnerSysId)
{
	HeapTuple	tup;
	Relation	rel;
	Form_pg_namespace nspForm;

	rel = heap_openr(NamespaceRelationName, RowExclusiveLock);

	tup = SearchSysCache(NAMESPACENAME,
						 CStringGetDatum(name),
						 0, 0, 0);
	if (!HeapTupleIsValid(tup))
		ereport(ERROR,
				(errcode(ERRCODE_UNDEFINED_SCHEMA),
				 errmsg("schema \"%s\" does not exist", name)));
	nspForm = (Form_pg_namespace) GETSTRUCT(tup);

	/*
	 * If the new owner is the same as the existing owner, consider the
	 * command to have succeeded.  This is for dump restoration purposes.
	 */
	if (nspForm->nspowner != newOwnerSysId)
	{
		Datum		repl_val[Natts_pg_namespace];
		char		repl_null[Natts_pg_namespace];
		char		repl_repl[Natts_pg_namespace];
		Acl		   *newAcl;
		Datum		aclDatum;
		bool		isNull;
		HeapTuple	newtuple;

		/* Otherwise, must be superuser to change object ownership */
		if (!superuser())
			ereport(ERROR,
					(errcode(ERRCODE_INSUFFICIENT_PRIVILEGE),
					 errmsg("must be superuser to change owner")));

		memset(repl_null, ' ', sizeof(repl_null));
		memset(repl_repl, ' ', sizeof(repl_repl));

		repl_repl[Anum_pg_namespace_nspowner - 1] = 'r';
		repl_val[Anum_pg_namespace_nspowner - 1] = Int32GetDatum(newOwnerSysId);

		/*
		 * Determine the modified ACL for the new owner.  This is only
		 * necessary when the ACL is non-null.
		 */
		aclDatum = SysCacheGetAttr(NAMESPACENAME, tup,
								   Anum_pg_namespace_nspacl,
								   &isNull);
		if (!isNull)
		{
			newAcl = aclnewowner(DatumGetAclP(aclDatum),
								 nspForm->nspowner, newOwnerSysId);
			repl_repl[Anum_pg_namespace_nspacl - 1] = 'r';
			repl_val[Anum_pg_namespace_nspacl - 1] = PointerGetDatum(newAcl);
		}

		newtuple = heap_modifytuple(tup, rel, repl_val, repl_null, repl_repl);

		simple_heap_update(rel, &newtuple->t_self, newtuple);
		CatalogUpdateIndexes(rel, newtuple);

		heap_freetuple(newtuple);
	}

	ReleaseSysCache(tup);
	heap_close(rel, NoLock);
}
