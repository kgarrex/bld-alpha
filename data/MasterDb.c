#ifndef MASTERDB_C 
#define MASTERDB_C 

#include ".\MasterDb.h"

#define MASTERDB_NAME "..\\Environment\\master.db"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

int MasterDb_Connect(ISQLDatabase Db){
    return layerDb_connect(Db, MASTERDB_NAME);
}

void MasterDb_Disconnect(ISQLDatabase Db){
    layerDb_disconnect(Db);
}


struct data_descriptor ProjectTypeResultSetDesc[] =
{
   {DDT_INT64, sizeof(long long)}
  ,{DDT_VARCHAR, PROJECT_TYPE_NAME_SIZE}
  ,{DDT_VARCHAR, PROJECT_TYPE_TEMPLATE_PATH_SIZE} 
};

// Get an entire row from the projectType table
// using the type name
void MasterDb_SelectProjectTypeByName(ISQLDatabase Db,
    struct ProjectTypeResultSet *result, int size, const char *TypeName)
{
    int count;

    struct data_descriptor params_desc[] = {
        {DDT_VARCHAR, -1}
    };

    struct {
        const char *typeName;
    } params[1] = {
        { .typeName = TypeName}
    };

    const char *sql =
    "SELECT "
      "rowid, "
      "name, "
      "templatePath "
    "FROM projectType "
    "WHERE name = ?";

   
    layerDb_setStatement(Db, ST_SQL_STRING, OP_SELECT, sql);

    layerDb_bindParams(Db, params_desc, ARRAY_SIZE(params_desc), &TypeName, 1);
    layerDb_bindResultSet(Db, ProjectTypeResultSetDesc, ARRAY_SIZE(ProjectTypeResultSetDesc), result, size);


    layerDb_execute(Db);
    layerDb_getRowCount(Db, &count);

}

void MasterDb_UpsertProjectByName(ISQLDatabase Db,
    const char *ProjectName,
    long long int ProjectTypeId,
    unsigned int ProjectCreateTime,
    unsigned int ProjectLastBuildTime
){
    const char *sql =
    "INSERT INTO projects() "
    "ON CONFLICT(rowid) DO UPDATE SET "
    "name=? "
    "WHERE rowid = ?";
}

void MasterDb_DeleteProjectByName(ISQLDatabase Db,
    const char *ProjectName
){
    const char *sql =
    "DELETE FROM project where name = ?";
}

#endif
