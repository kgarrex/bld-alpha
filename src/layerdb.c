#include "layerdb.h"

int layerDb_connect(ISQLDatabase *Db, const char *dbName)
{
    return (*Db)->connect(Db, dbName);
}

int layerDb_disconnect(ISQLDatabase *Db)
{
    return (*Db)->disconnect(Db);
}

int layerDb_bindParams(ISQLDatabase *Db,
    struct data_descriptor *desc_table, int desc_table_size,
    void *data_table, int data_table_size
)
{
    return (*Db)->bindParams(Db, desc_table,
           desc_table_size, data_table, data_table_size);
}

int layerDb_bindResultSet(ISQLDatabase *Db,
    struct data_descriptor *desc_table, int desc_table_size,
    void *data_table, int data_table_size
)
{
    return (*Db)->bindResultSet(Db, desc_table,
           desc_table_size, data_table, data_table_size);
}

int layerDb_setStatement(ISQLDatabase *Db, enum statement_type stmttype, enum operation_type optype, const char *stmt)
{
    return (*Db)->setStatement(Db, stmttype, optype, stmt);
}

int layerDb_execute(ISQLDatabase *Db)
{
    return (*Db)->execute(Db);
}

int layerDb_executeCb(ISQLDatabase *Db, void *cb, void *pb)
{
    return 0;
}

int layerDb_getRowCount(ISQLDatabase *Db, int *pcount)
{
    return (*Db)->getRowCount(Db, pcount);
}
