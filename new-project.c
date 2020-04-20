
void print_new_usage()
{
    printf("usage bld new [<options>] <projname>\n");
}


int new_projtype_callback(void *NOT_USED, const char *projtype)
{
struct bld_struct *bld = NOT_USED;
int sqlError;
union sqlite3_dll db; 
sqlite3_stmt *pStmt;

ISQLDatabase Db;

const char *text;


struct ProjectTypeResultSet Rs[1];

    Db = New(SQLiteDb);
    MasterDb_Connect(Db);
    MasterDb_SelectProjectTypeByName(Db, Rs, 1, projtype);
    MasterDb_Disconnect(Db);
    memcpy(bld->jsonPath, Rs[0].templatePath, strlen(Rs[0].templatePath)+1);

    return 1;
}

int new_callback(void *NOT_USED, const char *projname)
{
struct bld_struct *bld = NOT_USED;
struct git_init gi = {0};
struct DirStackObject dirStack[8] = {0};
OperatingSystem os;


    os.CreateDirectory = Windows_CreateDirectory;

    //append projname to the PathBuffer and create the new directory
    //next run the template generator using the project type

    memcpy(bld->projname, projname, strlen(projname)+1); 

    sprintf(bld->PathEnd, "\\%s", projname);

    os.CreateDirectory(bld->PathBuffer);

    printf("Path %s\n", bld->jsonPath);
    createDirectoryTemplate
        (bld->jsonPath, bld->PathBuffer, dirStack, 8, &bld->JSONObject);

    

//Create git repository
/*
    *pdir = 0;
    SetCurrentDirectory(dirbuf);
    git_init(&gi);

    git_add_all();
*/

    //printf("New project: %s\n", bld->basepath);
    return 1;
}


