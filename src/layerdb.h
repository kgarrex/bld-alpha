#ifndef layerDb_h 
#define layerDb_h 

#include "IClass.h"
#include "sqlite3dll.h"


enum data_descriptor_type {
  DDT_VARCHAR
 ,DDT_VARCHAR_ZT
 ,DDT_VARCHAR16
 ,DDT_VARCHAR16_ZT
 ,DDT_CHAR
 ,DDT_INT8
 ,DDT_INT16
 ,DDT_INT32
 ,DDT_INT64
 ,DDT_DOUBLE
 ,DDT_REAL
 ,DDT_GUID
 ,DDT_DATETIME
 ,DDT_BLOB
 ,DDT_BLOB64
};

enum statement_type {
  ST_STORED_PROCEDURE
 ,ST_SQL_STRING
};

enum operation_type {
  OP_SELECT
 ,OP_UPDATE
 ,OP_DELETE
 ,OP_INSERT
};

struct data_descriptor{
    enum data_descriptor_type data_type;
    int data_size;
};


#define DEFINE_ISQLDATABASE \
INHERIT_ICLASS \
int (__cdecl *connect)(void *pDb, const char *dbName);\
int (__cdecl *disconnect)(void *pDb);\
int (__cdecl *bindParams)(void *pDb, struct data_descriptor *, int, void **, int);\
int (__cdecl *bindResultSet)(void *pDb, struct data_descriptor *, int, void **, int);\
int (__cdecl *setStatement)(void *pDb, enum statement_type stmttype, \
enum operation_type optype, const char *stmt);\
int (__cdecl *execute)(void *pDb);\
int (__cdecl *executeCb)(void *pDb, void * cb, void * pb);\
int (__cdecl *getRowCount)(void *pDb, int*);

#define INHERIT_ISQLDATABASE DEFINE_ISQLDATABASE


typedef struct _ISQLDatabase *ISQLDatabase;

typedef struct _ISQLDatabase {
  DEFINE_ISQLDATABASE
} *ISQLDatabase;

int layerDb_connect(void *Db, const char *dbName);
int layerDb_disconnect(void *Db);
int layerDb_bindParams(void *Db, struct data_descriptor *, int, void *, int);
int layerDb_bindResultSet(void *Db, struct data_descriptor *, int, void *, int);
int layerDb_setStatement(void *Db, enum statement_type, enum operation_type, const char *stmt);
int layerDb_execute(void *Db);
int layerDb_executeCb(void *Db, void *cb, void *pb);
int layerDb_getRowCount(void *Db, int*);

#endif
