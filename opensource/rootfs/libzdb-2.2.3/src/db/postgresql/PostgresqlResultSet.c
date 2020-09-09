/*
 * Copyright (C) 2008 Tildeslash Ltd. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "Config.h"

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <libpq-fe.h>

#include "ResultSetStrategy.h"
#include "PostgresqlResultSet.h"


/**
 * Implementation of the ResultSet/Strategy interface for postgresql. 
 * Accessing columns with index outside range throws SQLException
 *
 * @version \$Id: PostgresqlResultSet.c,v 1.1 2008/11/12 13:25:56 wiley Exp $
 * @file
 */


/* ----------------------------------------------------------- Definitions */


const struct Rop_T postgresqlrops = {
      "postgresql",
      PostgresqlResultSet_free,
      PostgresqlResultSet_getColumnCount,
      PostgresqlResultSet_getColumnName,
      PostgresqlResultSet_next,
      PostgresqlResultSet_getColumnSize,
      PostgresqlResultSet_getString,
      PostgresqlResultSet_getStringByName,
      PostgresqlResultSet_getInt,
      PostgresqlResultSet_getIntByName,
      PostgresqlResultSet_getLLong,
      PostgresqlResultSet_getLLongByName,
      PostgresqlResultSet_getDouble,
      PostgresqlResultSet_getDoubleByName,
      PostgresqlResultSet_getBlob,
      PostgresqlResultSet_getBlobByName,
      PostgresqlResultSet_readData
};

#define T IResultSet_T
struct T {
      int keep;
      int maxRows;
      int currentRow;
      int columnCount;
      int rowCount;
      unsigned char **blob;
      PGresult *res;
};

#define TEST_INDEX(RETVAL) \
      int i; assert(R); i= columnIndex - 1; if (R->columnCount <= 0 || \
      i < 0 || i >= R->columnCount) { THROW(SQLException, "Column index out of range"); return(RETVAL); }
#define GET_INDEX(RETVAL) \
      int i; assert(R); if ((i= PQfnumber(R->res, columnName))<0) { \
      THROW(SQLException, "Invalid column name"); return (RETVAL);} i++;


/* ----------------------------------------------------- Protected methods */

#ifdef PACKAGE_PROTECTED
#pragma GCC visibility push(hidden)
#endif

T PostgresqlResultSet_new(void *res, int maxRows, int keep) {
      T R;
      assert(res);
      NEW(R);
      R->res = res;
      R->keep = keep;
      R->maxRows = maxRows;
      R->currentRow = -1;
      R->columnCount = PQnfields(R->res);
      R->rowCount = PQntuples(R->res);
      R->blob = CALLOC(R->columnCount, sizeof(unsigned char *));
      return R;
}


void PostgresqlResultSet_free(T *R) {
      int i;
      assert(R && *R);
      for (i = 0; i < (*R)->columnCount; i++) {
            PQfreemem((*R)->blob[i]);
      }
      FREE((*R)->blob);
      FREE(*R);
}


int PostgresqlResultSet_getColumnCount(T R) {
      assert(R);
      return R->columnCount;
}


const char *PostgresqlResultSet_getColumnName(T R, int column) {
      int i;
      assert(R);
      i = column - 1;
      if (R->columnCount <= 0 ||
         i < 0                ||
         i > R->columnCount)
            return NULL;
      return PQfname(R->res, i);
}


int PostgresqlResultSet_next(T R) {
      assert(R);
      if ((R->currentRow++ >= (R->rowCount - 1)) || (R->maxRows && (R->currentRow >= R->maxRows)))
            return false;
      return true;
}


long PostgresqlResultSet_getColumnSize(T R, int columnIndex) {
      TEST_INDEX(-1)
      return PQfsize(R->res, i);
}


const char *PostgresqlResultSet_getString(T R, int columnIndex) {
      TEST_INDEX(NULL)
      return PQgetvalue(R->res, R->currentRow, i);
}


const char *PostgresqlResultSet_getStringByName(T R, const char *columnName) {
      GET_INDEX(NULL)
      return PostgresqlResultSet_getString(R, i);
}


int PostgresqlResultSet_getInt(T R, int columnIndex) {
      TEST_INDEX(0)
      return Str_parseInt(PQgetvalue(R->res, R->currentRow, i));
}


int PostgresqlResultSet_getIntByName(T R, const char *columnName) {
      GET_INDEX(0)
      return PostgresqlResultSet_getInt(R, i);
}


long long int PostgresqlResultSet_getLLong(T R, int columnIndex) {
      TEST_INDEX(0)
      return Str_parseLLong(PQgetvalue(R->res, R->currentRow, i));
}


long long int PostgresqlResultSet_getLLongByName(T R, const char *columnName) {
      GET_INDEX(0)
      return PostgresqlResultSet_getLLong(R, i);
}


double PostgresqlResultSet_getDouble(T R, int columnIndex) {
      TEST_INDEX(0.0)
      return Str_parseDouble(PQgetvalue(R->res, R->currentRow, i));
}


double PostgresqlResultSet_getDoubleByName(T R, const char *columnName) {
      GET_INDEX(0.0)
      return PostgresqlResultSet_getDouble(R, i);
}


const void *PostgresqlResultSet_getBlob(T R, int columnIndex, int *size) {
      size_t s;
      TEST_INDEX(NULL)
      if (R->blob[i]) {
            PQfreemem(R->blob[i]);
      }
      R->blob[i] = PQunescapeBytea(PQgetvalue(R->res, R->currentRow, i), &s);
      *size = s;
      return R->blob[i];
}


const void *PostgresqlResultSet_getBlobByName(T R, const char *columnName, int *size) {
      GET_INDEX(NULL)
      return PostgresqlResultSet_getBlob(R, i, size);
}


int PostgresqlResultSet_readData(T R, int columnIndex, void *b, int l, long off) {
      long r;
      size_t size;
      unsigned char *blob;
      TEST_INDEX(0)
      blob = PQunescapeBytea(PQgetvalue(R->res, R->currentRow, i), &size);
      if (off > size)
            return 0;
      r = off + l>size?size-off:l;
      memcpy(b, blob + off, r);
      PQfreemem(blob);
      return r;
}

#ifdef PACKAGE_PROTECTED
#pragma GCC visibility pop
#endif

