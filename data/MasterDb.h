
#include "..\src\layerdb_sqlite.h"

#define PROJECT_NAME_SIZE 32

#define PROJECT_TYPE_NAME_SIZE 32
#define PROJECT_TYPE_TEMPLATE_PATH_SIZE 64 


struct ProjectTypeResultSet{
    long long int rowid;
    char name[PROJECT_TYPE_NAME_SIZE];
    char templatePath[PROJECT_TYPE_TEMPLATE_PATH_SIZE];
};

struct ProjectTable{
    const char project_name[PROJECT_NAME_SIZE];
    long long int project_type_id;
    unsigned int project_create_time;
    unsigned int project_last_build_timeUTC; 
    //char project_version_control_url[PROJECT_VERSION_CONTROL_URL_SIZE]; 
};

int MasterDb_Connect(ISQLDatabase Db);

void MasterDb_Disconnect(ISQLDatabase Db);

void MasterDb_SelectProjectTypeByName(ISQLDatabase Db
   ,struct ProjectTypeResultSet *Rs, int RsSize
   ,const char *TypeName
);

void MasterDb_InsertProject(ISQLDatabase Db
//    struct ProjectTable *table, int TblCount,
   ,const char *ProjectName
   ,long long int ProjectTypeId
   ,unsigned int ProjectCreateTime
   ,unsigned int ProjectLastBuildTime
);

void MasterDb_UpdateProjectByName(ISQLDatabase Db
   ,const char *ProjectName
   ,long long int ProjectTypeId
   ,unsigned int ProjectCreateTime
   ,unsigned int ProjectLastBuildTime
);

void MasterDb_DeleteProjectByName(ISQLDatabase Db,
    const char *ProjectName
);
