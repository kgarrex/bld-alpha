#include <stdio.h>
#include <assert.h>
#include "layerdb_sqlite.h"

const char *libPath = ".\\sqlite3.dll";

typedef struct {
    ISQLiteDb class;

    union sqlite3_dll lib;
    //HANDLE sqlite3lib;
    sqlite3 * pDatabase;
    sqlite3_stmt * pStatement;
    int stmttype; //statement type
    int optype; //operation type
    struct data_descriptor *desc_table;
    int desc_table_size;
    void *data_table;
    int data_table_size;
    int result_idx;
    int row_count;
    
} SQLiteDbObject;


void * GetFnAddress(const char *fnName)
{
static HANDLE sqlite3lib = 0;
void * fn;

    do{
        if(sqlite3lib) break;
        sqlite3lib = GetModuleHandleA(libPath); 
        if(sqlite3lib) break;
        sqlite3lib = LoadLibrary(libPath);	
        if(!sqlite3lib){
            printf("Error: Could not load library %s\n", libPath);
            return 0;
	}
    } while(0);

    fn = GetProcAddress(sqlite3lib, fnName);
    if(!fn) {
        printf("Error: Could not get proc address\n"); 
	return 0;
    }
    return fn;
}

static int Connect(SQLiteDbObject *pDb, const char *dbName)
{
int sqlError;

assert(pDb);
    pDb->lib.sqlite3_open = GetFnAddress("sqlite3_open");
    sqlError = pDb->lib.sqlite3_open(dbName, &pDb->pDatabase);
    if(sqlError != SQLITE_OK) {
        printf("Error: Could not open database %s\n", dbName); 
	return 0;
    }
    return 1;
}

static int SetStatement(void *_pDb,
    enum statement_type stmttype,
    enum operation_type optype, const char *sql)
{
SQLiteDbObject *pDb = _pDb;
int sqlError;
    pDb->lib.sqlite3_prepare_v2 = GetFnAddress("sqlite3_prepare_v2");

    switch(stmttype)
    {
    case ST_STORED_PROCEDURE:
        return 0; //statement type not supported
    case ST_SQL_STRING:
        pDb->stmttype = stmttype;
        sqlError = pDb->lib.sqlite3_prepare_v2(pDb->pDatabase, sql, -1, &pDb->pStatement, 0);
	if(sqlError != SQLITE_OK) {
            printf("Failed to prepare query\n");	
            return 0;
	}
        break;
    default: return 0; //statement type not supported
    }
    //should do bounds checking on optype
    pDb->optype = optype;
    return 1;
}

static int _BindColumn(SQLiteDbObject *pDb, void *col, int type, int size, int i)
{
const void * tmp;
int numBytes;
int n;
    switch(type)
    {
        case DDT_VARCHAR_ZT:
        case DDT_GUID:
        case DDT_VARCHAR:
            goto CALL_SQLITE3_COLUMN_TEXT;

        case DDT_INT8:
	    goto CALL_SQLITE3_COLUMN_INT;
        case DDT_INT16:
            goto CALL_SQLITE3_COLUMN_INT;
        case DDT_INT32:
            goto CALL_SQLITE3_COLUMN_INT;
        case DDT_DATETIME:
            goto CALL_SQLITE3_COLUMN_INT;

        case DDT_VARCHAR16_ZT:
        case DDT_VARCHAR16:
            goto CALL_SQLITE3_COLUMN_TEXT16;

        case DDT_INT64:
            goto CALL_SQLITE3_COLUMN_INT64;

        case DDT_REAL:
	case DDT_DOUBLE:
            goto CALL_SQLITE3_COLUMN_DOUBLE;

	case DDT_BLOB:
            goto CALL_SQLITE3_COLUMN_BLOB;

        default:
	    return 0;
    }

CALL_SQLITE3_COLUMN_BLOB:
    pDb->lib.sqlite3_column_blob = GetFnAddress("sqlite3_column_blob");
    tmp = pDb->lib.sqlite3_column_blob(pDb->pStatement, i); 
    goto COPY_BYTES;

CALL_SQLITE3_COLUMN_DOUBLE:
    pDb->lib.sqlite3_column_double = GetFnAddress("sqlite3_column_double");
    *((double*)col) = pDb->lib.sqlite3_column_double(pDb->pStatement, i);
    goto EXIT;

CALL_SQLITE3_COLUMN_INT:
    pDb->lib.sqlite3_column_int = GetFnAddress("sqlite3_column_int");
    *((int*)col) = pDb->lib.sqlite3_column_int(pDb->pStatement, i);
    goto EXIT;

CALL_SQLITE3_COLUMN_INT64:
    pDb->lib.sqlite3_column_int64 = GetFnAddress("sqlite3_column_int64");
    tmp = pDb->lib.sqlite3_column_int64(pDb->pStatement, i);

CALL_SQLITE3_COLUMN_TEXT:
    pDb->lib.sqlite3_column_text = GetFnAddress("sqlite3_column_text");
    tmp = pDb->lib.sqlite3_column_text(pDb->pStatement, i);
    goto COPY_BYTES;

CALL_SQLITE3_COLUMN_TEXT16:
    pDb->lib.sqlite3_column_text16 = GetFnAddress("sqlite3_column_text16");
    tmp = pDb->lib.sqlite3_column_text16(pDb->pStatement, i);
    goto COPY_BYTES16;

CALL_SQLITE3_COLUMN_VALUE: //NOT HANDLED PROPERLY, DONT USE YET
    pDb->lib.sqlite3_column_value = GetFnAddress("sqlite3_column_value");
    tmp = pDb->lib.sqlite3_column_value(pDb->pStatement, i);
    goto COPY_BYTES;

COPY_BYTES:
    if(size == -1); //col = tmp;
    else {
        pDb->lib.sqlite3_column_bytes = GetFnAddress("sqlite3_column_bytes");
        numBytes = pDb->lib.sqlite3_column_bytes(pDb->pStatement, i);
        if(numBytes > size) {} //error data too large, truncate or exit
        memcpy((char*)col, tmp, numBytes+1); 
    }
    goto EXIT;
COPY_BYTES16:
    if(size == -1); //col = tmp;
    else {
        pDb->lib.sqlite3_column_bytes16 = GetFnAddress("sqlite3_column_bytes16");
        numBytes = pDb->lib.sqlite3_column_bytes16(pDb->pStatement, i);
	if(numBytes > size) {} //error data too large, truncate or exit
	memcpy((wchar_t*)col, tmp, numBytes+1);
    }

EXIT:
    return 1;
}

static int _BindParam(SQLiteDbObject *pDb, int type, const void  *value, int len, int i)
{
int r;

    if(!value) goto CALL_SQLITE3_BIND_NULL;
    switch(type)
    {
        case DDT_VARCHAR_ZT:
            len = -1;
        case DDT_GUID:
        case DDT_VARCHAR:
            goto CALL_SQLITE3_BIND_TEXT;

        case DDT_INT8:
            len = sizeof(char);
	    goto CALL_SQLITE3_BIND_INT;
        case DDT_INT16:
            len = sizeof(short);
            goto CALL_SQLITE3_BIND_INT;
        case DDT_INT32:
            len = sizeof(int);
            goto CALL_SQLITE3_BIND_INT;
        case DDT_DATETIME:
            goto CALL_SQLITE3_BIND_INT;

        case DDT_VARCHAR16_ZT:
            len = -1;
        case DDT_VARCHAR16:
            goto CALL_SQLITE3_BIND_TEXT16;

        case DDT_INT64:
            goto CALL_SQLITE3_BIND_INT64;

        case DDT_REAL:
	case DDT_DOUBLE:
            goto CALL_SQLITE3_BIND_DOUBLE;

	case DDT_BLOB:
            goto CALL_SQLITE3_BIND_BLOB;

        case DDT_BLOB64:
            goto CALL_SQLITE3_BIND_BLOB64;

        default:
	    return 0;
    }

CALL_SQLITE3_BIND_TEXT:
    pDb->lib.sqlite3_bind_text = GetFnAddress("sqlite3_bind_text");
    r = pDb->lib.sqlite3_bind_text(pDb->pStatement, i, (const char*)value, len, SQLITE_STATIC); 
    goto EXIT;

CALL_SQLITE3_BIND_INT:
    pDb->lib.sqlite3_bind_int = GetFnAddress("sqlite3_bind_int");
    r = pDb->lib.sqlite3_bind_int(pDb->pStatement, i, *((int*)value));
    goto EXIT;

CALL_SQLITE3_BIND_NULL:
    pDb->lib.sqlite3_bind_null = GetFnAddress("sqlite3_bind_null");
    r = pDb->lib.sqlite3_bind_null(pDb->pStatement, i);
    goto EXIT;

CALL_SQLITE3_BIND_TEXT16:
    pDb->lib.sqlite3_bind_text16 = GetFnAddress("sqlite3_bind_text16");
    r = pDb->lib.sqlite3_bind_text16(pDb->pStatement, i, (const wchar_t*)value, len, SQLITE_STATIC);
    goto EXIT;

CALL_SQLITE3_BIND_INT64:
    pDb->lib.sqlite3_bind_int64 = GetFnAddress("sqlite3_bind_int64");
    r = pDb->lib.sqlite3_bind_int64(pDb->pStatement, i, (sqlite3_uint64)value);
    goto EXIT;

CALL_SQLITE3_BIND_DOUBLE:
    pDb->lib.sqlite3_bind_double = GetFnAddress("sqlite3_bind_double");
    r = pDb->lib.sqlite3_bind_double(pDb->pStatement, i, *((double*)value));
    goto EXIT;

CALL_SQLITE3_BIND_BLOB:
    pDb->lib.sqlite3_bind_blob = GetFnAddress("sqlite3_bind_blob");
    r = pDb->lib.sqlite3_bind_blob(pDb->pStatement, i, value, len ,SQLITE_STATIC);
    goto EXIT;

CALL_SQLITE3_BIND_BLOB64:
    pDb->lib.sqlite3_bind_blob64 = GetFnAddress("sqlite3_bind_blob64");
    r = pDb->lib.sqlite3_bind_blob64(pDb->pStatement, i, value, (sqlite3_uint64) len, SQLITE_STATIC);
    goto EXIT;

CALL_SQLITE3_BIND_POINTER:
CALL_SQLITE3_ZERO_BLOB:
CALL_SQLITE3_ZERO_BLOB64:
CALL_SQLITE3_ZERO_VALUE:
 
EXIT:
    if(r != SQLITE_OK) {
        printf("Error: Could not bind params\n");
        return 0;
    }
    return 1;
}

static int BindResultSet(SQLiteDbObject *pDb,
    struct data_descriptor *desc_table, int desc_table_size,
    void *data_table, int data_table_size)
{
    pDb->desc_table = desc_table;
    pDb->desc_table_size = desc_table_size;
    pDb->data_table = data_table;
    pDb->data_table_size = data_table_size;
    return 1;
}

static int BindParams(SQLiteDbObject *pDb,
    struct data_descriptor *desc_table, int desc_table_size,
    void *data_table, int data_table_size)
{
char *ptr;
struct data_descriptor *pdesc;
    ptr = *((char**)data_table);
    printf("String: %s\n", (char*)ptr);
    for(int i = 0; i < data_table_size; i++)
    {
        for(int ii = 0; ii < desc_table_size; ii++)
        {
            pdesc = &desc_table[ii];
            _BindParam(pDb, pdesc->data_type, ptr, pdesc->data_size, ii+1);
	    if(pdesc->data_size == -1)
                ptr += sizeof(void*);
            else
                ptr += pdesc->data_size;
        }
    }
    return 1;
}

    //pDb->lib.sqlite3_reset = GetFnAddress("sqlite3_reset");
    //pDb->lib.sqlite3_clear_bindings = GetFnAddress("sqlite3_clear_bindings");
    //pDb->lib.sqlite3_finalize = GetFnAddress("sqlite3_finalize");
 
static int Execute(SQLiteDbObject *pDb)
{
int result;
sqlite3_step_t step;
int col_count;
char *ptr;
struct data_descriptor *pdesc;
int colCount;

    pDb->lib.sqlite3_step = GetFnAddress("sqlite3_step");
    step = pDb->lib.sqlite3_step;
   
    pDb->lib.sqlite3_column_count = GetFnAddress("sqlite3_column_count");
    colCount = pDb->lib.sqlite3_column_count(pDb->pStatement);
    if(colCount != pDb->desc_table_size){
        printf("Error: Column count (%d) does not match (%d)\n", colCount, pDb->desc_table_size);
        return 0;
    }

    ptr = pDb->data_table;
    while(SQLITE_DONE != (result = step(pDb->pStatement)))
    {
        if(result != SQLITE_ROW){} // error
	//go through each column and bind column
        for(int i = 0; i < pDb->desc_table_size; i++) 
        {
             pdesc = &pDb->desc_table[i];
             _BindColumn(pDb, ptr, pdesc->data_type, pdesc->data_size, i);
	     if(pdesc->data_size == -1)
                 ptr += sizeof(void*);
             else
	         ptr += pdesc->data_size;
	}

	pDb->row_count++;

	if(++pDb->result_idx == pDb->data_table_size){
            pDb->result_idx = 0; 
            return 2; //results pending
        }
    }

    return 1;
}

static int GetRowCount(SQLiteDbObject *pDb, int *pcount)
{
    *pcount = pDb->row_count;
    return 1;
}


static int Disconnect(SQLiteDbObject *pDb)
{
    pDb->lib.sqlite3_finalize = GetFnAddress("sqlite3_finalize");
    pDb->lib.sqlite3_finalize(pDb->pStatement);
    pDb->lib.sqlite3_close = GetFnAddress("sqlite3_close");
    pDb->lib.sqlite3_close(pDb->pDatabase);
    return 1;
}

static void * _ctor(DEBUG_DECL void *_obj, va_list args)
{
SQLiteDbObject *obj;
    obj = _obj ? _obj : malloc(sizeof(SQLiteDbObject));

    obj->class = SQLiteDb;
    obj->stmttype = 0;
    obj->optype = 0;
    obj->desc_table = 0;
    obj->desc_table_size = 0;
    obj->data_table = 0;
    obj->data_table_size = 0;
    obj->result_idx = 0;
    obj->row_count = 0;

    return obj;
}

static int SizeOf()
{
    return sizeof(SQLiteDbObject);
}

static void _dtor(DEBUG_DECL void *obj)
{}

static struct _ISQLiteDb  _ = {
  .Ctor            = _ctor,
  .Dtor            = _dtor,
  .SizeOf          = SizeOf,
  .connect         = Connect,
  .disconnect      = Disconnect,
  .bindParams      = BindParams,
  .bindResultSet   = BindResultSet,
  .setStatement    = SetStatement,
  .execute         = Execute,
  .getRowCount     = GetRowCount
};

void * SQLiteDb = &_;
