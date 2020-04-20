#ifndef SQLITE_DLL_H
#define SQLITE_DLL_H

#include "sqlite3.h"

typedef int (__stdcall *sqlite3_open_t)(const char*, sqlite3**);
typedef int (__stdcall *sqlite3_open16_t)(const void*, sqlite3 **);
typedef int (__stdcall *sqlite3_open_v2_t)(const char*, sqlite3**, int, const char*);
typedef int (__stdcall *sqlite3_exec_t)(sqlite3*, const char *sql, int (*callback)(void*, int, char**,char**), void*, char **errmsg);
typedef int (__stdcall *sqlite3_prepare_t)(sqlite3 *db, const char *zSql, int nByte,sqlite3_stmt **ppStmt, const char **pzTail);
typedef int (__stdcall *sqlite3_prepare_v2_t)(sqlite3 *db, const char *zSql, int nByte, sqlite3_stmt **ppStmt, const char **pzTail);
typedef int (__stdcall *sqlite3_prepare_v3_t)(sqlite3 *db, const char *zSql, int nByte, unsigned int prepFlags, sqlite3_stmt **ppStmt, const char **pzTail);
typedef int (__stdcall *sqlite3_prepare16_t)(sqlite3 *db, const void *, int, sqlite3_stmt **, const void **);

typedef int (__stdcall *sqlite3_bind_blob_t)(sqlite3_stmt*, int, const void*, int, void(*)(void*));
typedef int (__stdcall *sqlite3_bind_blob64_t)(sqlite3_stmt*, int, const void*, sqlite3_uint64, void (*)(void*));
typedef int (__stdcall *sqlite3_bind_double_t)(sqlite3_stmt*, int, double);
typedef int (__stdcall *sqlite3_bind_int_t)(sqlite3_stmt*, int, int);
typedef int (__stdcall *sqlite3_bind_int64_t)(sqlite3_stmt*, int, sqlite3_int64);
typedef int (__stdcall *sqlite3_bind_null_t)(sqlite3_stmt*, int);
typedef int (__stdcall *sqlite3_bind_text_t)(sqlite3_stmt*, int, const char *, int, void (*)(void*));
typedef int (__stdcall *sqlite3_bind_text16_t)(sqlite3_stmt*, int, const void*, int, void (*)(void*));
typedef int (__stdcall *sqlite3_bind_text64_t)(sqlite3_stmt*, int, const char *, sqlite3_uint64, void (*)(void*), unsigned char encoding);
typedef int (__stdcall *sqlite3_bind_value_t)(sqlite3_stmt*, int, const sqlite3_value*);
typedef int (__stdcall *sqlite3_bind_pointer_t)(sqlite3_stmt*, int, void*, const char*, void (*)(void*));
typedef int (__stdcall *sqlite3_bind_zeroblob_t)(sqlite3_stmt*, int, int);
typedef int (__stdcall *sqlite3_bind_zeroblob64_t)(sqlite3_stmt*, int, sqlite3_uint64);

typedef int (__stdcall *sqlite3_close_t)(sqlite3*);
typedef int (__stdcall *sqlite3_close_v2_t)(sqlite3*);

typedef const void * (__stdcall *sqlite3_column_blob_t)(sqlite3_stmt*, int iCol);
typedef double (__stdcall *sqlite3_column_double_t)(sqlite3_stmt*, int iCol);
typedef int (__stdcall *sqlite3_column_int_t)(sqlite3_stmt*, int iCol);
typedef sqlite3_int64 (__stdcall *sqlite3_column_int64_t)(sqlite3_stmt*, int iCol); 
typedef const unsigned char *(__stdcall *sqlite3_column_text_t)(sqlite3_stmt*, int);
typedef const void * (__stdcall *sqlite3_column_text16_t)(sqlite3_stmt*, int iCol);
typedef sqlite3_value * (__stdcall *sqlite3_column_value_t)(sqlite3_stmt*, int);
typedef int (__stdcall *sqlite3_column_bytes_t)(sqlite3_stmt*, int);
typedef int (__stdcall *sqlite3_column_bytes16_t)(sqlite3_stmt*, int);
typedef int (__stdcall *sqlite3_column_bytes32_t)(sqlite3_stmt*, int);

typedef int (__cdecl *sqlite3_db_config_t)(sqlite3*, int op, ...);
typedef int (__stdcall *sqlite3_step_t)(sqlite3_stmt*);
typedef int (__stdcall *sqlite3_finalize_t)(sqlite3_stmt *pStmt);

typedef int (__stdcall *sqlite3_data_count_t)(sqlite3_stmt*);
typedef int (__stdcall *sqlite3_column_count_t)(sqlite3_stmt*);
typedef int (__stdcall *sqlite3_reset_t)(sqlite3_stmt*);
typedef int (__stdcall *sqlite3_clear_bindings_t)(sqlite3_stmt*);


union sqlite3_dll {
void * fn;
sqlite3_open_t                    sqlite3_open;
sqlite3_open16_t                  sqlite3_open16;
sqlite3_open_v2_t                 sqlite3_open_v2;
sqlite3_exec_t                    sqlite3_exec;
sqlite3_prepare_t                 sqlite3_prepare;
sqlite3_prepare_v2_t              sqlite3_prepare_v2;
sqlite3_prepare_v3_t              sqlite3_prepare_v3;
sqlite3_prepare16_t               sqlite3_prepare16;

sqlite3_bind_blob_t               sqlite3_bind_blob;
sqlite3_bind_blob64_t             sqlite3_bind_blob64;
sqlite3_bind_double_t             sqlite3_bind_double;
sqlite3_bind_int_t                sqlite3_bind_int;
sqlite3_bind_int64_t              sqlite3_bind_int64;
sqlite3_bind_null_t               sqlite3_bind_null;
sqlite3_bind_text_t               sqlite3_bind_text;
sqlite3_bind_text16_t             sqlite3_bind_text16;
sqlite3_bind_text64_t             sqlite3_bind_text64;
sqlite3_bind_value_t              sqlite3_bind_value;
sqlite3_bind_pointer_t            sqlite3_bind_pointer;
sqlite3_bind_zeroblob_t           sqlite3_bind_zeroblob;
sqlite3_bind_zeroblob64_t         sqlite3_bind_zeroblob64;

sqlite3_close_t                   sqlite3_close;
sqlite3_close_v2_t                sqlite3_close_v2;

sqlite3_column_blob_t             sqlite3_column_blob;
sqlite3_column_double_t           sqlite3_column_double;
sqlite3_column_int_t              sqlite3_column_int;
sqlite3_column_int64_t            sqlite3_column_int64;
sqlite3_column_text_t             sqlite3_column_text;
sqlite3_column_text16_t           sqlite3_column_text16;
sqlite3_column_value_t            sqlite3_column_value;
sqlite3_column_bytes_t            sqlite3_column_bytes;
sqlite3_column_bytes16_t          sqlite3_column_bytes16;
sqlite3_column_bytes32_t          sqlite3_column_bytes32;

sqlite3_db_config_t               sqlite3_db_config;
sqlite3_data_count_t              sqlite3_data_count;
sqlite3_column_count_t            sqlite3_column_count;

sqlite3_clear_bindings_t          sqlite3_clear_bindings;
sqlite3_reset_t                   sqlite3_reset;
sqlite3_finalize_t                sqlite3_finalize;

sqlite3_step_t                    sqlite3_step;

};

#endif
