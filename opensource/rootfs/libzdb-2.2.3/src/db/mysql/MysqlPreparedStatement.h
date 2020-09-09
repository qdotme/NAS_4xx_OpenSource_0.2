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
#ifndef MYSQLPREPAREDSTATEMENT_INCLUDED
#define MYSQLPREPAREDSTATEMENT_INCLUDED
#define T IPreparedStatement_T
T MysqlPreparedStatement_new(void *stmt, int maxRows);
void MysqlPreparedStatement_free(T *P);
int MysqlPreparedStatement_setString(T P, int parameterIndex, const char *x);
int MysqlPreparedStatement_setInt(T P, int parameterIndex, int x);
int MysqlPreparedStatement_setLLong(T P, int parameterIndex, long long int x);
int MysqlPreparedStatement_setDouble(T P, int parameterIndex, double x);
int MysqlPreparedStatement_setBlob(T P, int parameterIndex, const void *x, int size);
int MysqlPreparedStatement_execute(T P);
ResultSet_T MysqlPreparedStatement_executeQuery(T P);
#undef T
#endif
