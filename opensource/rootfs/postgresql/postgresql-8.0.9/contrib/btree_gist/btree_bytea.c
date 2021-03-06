#include "btree_gist.h"
#include "btree_utils_var.h"
#include "utils/builtins.h"


/*
** Bytea ops
*/
PG_FUNCTION_INFO_V1(gbt_bytea_compress);
PG_FUNCTION_INFO_V1(gbt_bytea_union);
PG_FUNCTION_INFO_V1(gbt_bytea_picksplit);
PG_FUNCTION_INFO_V1(gbt_bytea_consistent);
PG_FUNCTION_INFO_V1(gbt_bytea_penalty);
PG_FUNCTION_INFO_V1(gbt_bytea_same);

Datum		gbt_bytea_compress(PG_FUNCTION_ARGS);
Datum		gbt_bytea_union(PG_FUNCTION_ARGS);
Datum		gbt_bytea_picksplit(PG_FUNCTION_ARGS);
Datum		gbt_bytea_consistent(PG_FUNCTION_ARGS);
Datum		gbt_bytea_penalty(PG_FUNCTION_ARGS);
Datum		gbt_bytea_same(PG_FUNCTION_ARGS);


/* define for comparison */

static bool
gbt_byteagt(const void *a, const void *b)
{
	return (DatumGetBool(DirectFunctionCall2(byteagt, PointerGetDatum(a), PointerGetDatum(b))));
}

static bool
gbt_byteage(const void *a, const void *b)
{
	return (DatumGetBool(DirectFunctionCall2(byteage, PointerGetDatum(a), PointerGetDatum(b))));
}

static bool
gbt_byteaeq(const void *a, const void *b)
{
	return (DatumGetBool(DirectFunctionCall2(byteaeq, PointerGetDatum(a), PointerGetDatum(b))));
}

static bool
gbt_byteale(const void *a, const void *b)
{
	return (DatumGetBool(DirectFunctionCall2(byteale, PointerGetDatum(a), PointerGetDatum(b))));
}

static bool
gbt_bytealt(const void *a, const void *b)
{
	return (DatumGetBool(DirectFunctionCall2(bytealt, PointerGetDatum(a), PointerGetDatum(b))));
}


static int32
gbt_byteacmp(const bytea *a, const bytea *b)
{
	return
		(DatumGetInt32(DirectFunctionCall2(byteacmp, PointerGetDatum(a), PointerGetDatum(b))));
}


static const gbtree_vinfo tinfo =
{
	gbt_t_bytea,
	FALSE,
	TRUE,
	gbt_byteagt,
	gbt_byteage,
	gbt_byteaeq,
	gbt_byteale,
	gbt_bytealt,
	gbt_byteacmp,
	NULL
};


/**************************************************
 * Text ops
 **************************************************/


Datum
gbt_bytea_compress(PG_FUNCTION_ARGS)
{
	GISTENTRY  *entry = (GISTENTRY *) PG_GETARG_POINTER(0);

	PG_RETURN_POINTER(gbt_var_compress(entry, &tinfo));
}



Datum
gbt_bytea_consistent(PG_FUNCTION_ARGS)
{
	GISTENTRY  *entry = (GISTENTRY *) PG_GETARG_POINTER(0);
	GBT_VARKEY *ktst = (GBT_VARKEY *) DatumGetPointer(entry->key);
	GBT_VARKEY *key = (GBT_VARKEY *) DatumGetPointer(PG_DETOAST_DATUM(entry->key));
	void	   *qtst = (void *) DatumGetPointer(PG_GETARG_DATUM(1));
	void	   *query = (void *) DatumGetByteaP(PG_GETARG_DATUM(1));
	StrategyNumber strategy = (StrategyNumber) PG_GETARG_UINT16(2);
	bool		retval = FALSE;
	GBT_VARKEY_R r = gbt_var_key_readable(key);

	retval = gbt_var_consistent(&r, query, &strategy, GIST_LEAF(entry), &tinfo);

	if (ktst != key)
		pfree(key);
	if (qtst != query)
		pfree(query);
	PG_RETURN_BOOL(retval);
}



Datum
gbt_bytea_union(PG_FUNCTION_ARGS)
{
	GistEntryVector *entryvec = (GistEntryVector *) PG_GETARG_POINTER(0);
	int32	   *size = (int *) PG_GETARG_POINTER(1);

	PG_RETURN_POINTER(gbt_var_union(entryvec, size, &tinfo));
}


Datum
gbt_bytea_picksplit(PG_FUNCTION_ARGS)
{
	GistEntryVector *entryvec = (GistEntryVector *) PG_GETARG_POINTER(0);
	GIST_SPLITVEC *v = (GIST_SPLITVEC *) PG_GETARG_POINTER(1);

	gbt_var_picksplit(entryvec, v, &tinfo);
	PG_RETURN_POINTER(v);
}

Datum
gbt_bytea_same(PG_FUNCTION_ARGS)
{
	Datum		d1 = PG_GETARG_DATUM(0);
	Datum		d2 = PG_GETARG_DATUM(1);
	bool	   *result = (bool *) PG_GETARG_POINTER(2);

	PG_RETURN_POINTER(gbt_var_same(result, d1, d2, &tinfo));
}


Datum
gbt_bytea_penalty(PG_FUNCTION_ARGS)
{
	float	   *result = (float *) PG_GETARG_POINTER(2);
	GISTENTRY  *o = (GISTENTRY *) PG_GETARG_POINTER(0);
	GISTENTRY  *n = (GISTENTRY *) PG_GETARG_POINTER(1);

	PG_RETURN_POINTER(gbt_var_penalty(result, o, n, &tinfo));
}
