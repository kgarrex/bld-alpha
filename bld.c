
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>

#include "src\IVisualCCompiler.h"
#include "src\IVisualCLinker.h"
#include "src\IFileSystem.h"
#include "..\lib-dilly\json.h"

#include "bldgit.c"
#include "common.c"

//#include "sqlite3.h"

#include ".\data\MasterDb.h"
#include ".\data\ProjectDb.h"

#include "bld_defines.h"

enum PROJECT_TYPES{
    WINDOWS_DRIVER,
    LINUX_DRIVER
};

enum {
  CONTROL_STOP
 ,CONTROL_CONTINUE
 ,CONTROL_RECALL_DIRECTORY
};

int callback(LPWIN32_FIND_DATAA findData, const char *fullPath)
{
    printf("%s\n", fullPath);

    if(findData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        if(!RemoveDirectoryA(fullPath))
        {
            if(GetLastError() != ERROR_DIR_NOT_EMPTY){
                return 0;
	    }
	    return CONTROL_RECALL_DIRECTORY;
        }
    }
    else {
        DeleteFile(fullPath); 
    }

    return CONTROL_CONTINUE;
}


const char *proj_dir = ".\\projects\\";

void print_usage()
{
    printf("usage bld [<options>] <command> [<args>]\n");
    printf("build\n");
    printf("new\n");
    printf("delete\n");
    printf("report\n");
}

/*
void iter_by_file_ext(const char *dir, const char *ext)
{
char *ptr; 
ptr = dir + strlen(dir);
memcpy(ptr, "\\*", 2);
memcpy(ptr, ext, strlen(ext)); 
WIN32_FIND_DATAA find_data;
HANDLE search_handle;
    search_handle = FindFirstFileA(ext, &find_data);
    if(search_handle == INVALID_HANDLE_VALUE){
        printf("Error %u: Could not find file\n", GetLastError());
	return;
    }

    do {
        printf("%s\n", find_data.cFileName);
    } while(FindNextFileA(search_handle, &find_data)); 
    FindClose(search_handle);
}

void foreach_header_file(const char *dir)
{
    iter_by_file_ext(dir, "*.h");
}

void foreach_source_file(FN, void *)
{
WIN32_FIND_DATAA find_data;
HANDLE search_handle;
    search_handle = FindFirst

}
*/


#define DEFINE_VERSION(b1, b2, b3, b4) (b1 << 24 | b2 << 16 | b3 << 8 | b1)
#define VERSION_1 0x1
#define VERSION_2 0x2

#define FLG1_BASE_PATH_SET_CMDLINE 0x00000001
#define FLG1_BASE_PATH_SET_CONFIG  0x00000002


#include "createDirectoryTemplate.c"
#include "oslayer.c"

struct bld_struct {
    char *PathBuffer;
    int PathBufferSize;
    int PathStringSize;
    char *PathEnd;

    char * basePtr;
    char * projDirPtr;

    int basepath_len;
    const char *cfgfile;
    int flags1;
    int version;

    char jsonPath[64];

    const char *baseConfig;

    struct json_object JSONObject;

    char projname[32];

    sqlite3 *pDb;
};


char ini_mem[8192];

char g_jsonPath;

void bonec_initEnv(struct bld_struct *bld)
{
struct json_parser_state parser_state = {0};
int result;
struct DirStackObject dirStack[4] = {0};
DWORD attr;
struct fileObject fo;
int size;
char projDirName[32];

    attr = GetFileAttributes(".\\bld_config.json");
    if(attr == INVALID_FILE_ATTRIBUTES || attr & FILE_ATTRIBUTE_DIRECTORY)
    {
        printf("Invalid file\n");
        return;
    }
 
 
    bld->baseConfig = loadFileInMemory(".\\bld_config.json", &fo);

    result = json_read(&parser_state, &bld->JSONObject, bld->baseConfig);
    if(result != JSON_SUCCESS){
        printf("Error: Could not read json file\n");
	return;
    }

    result = json_query_get_string(&bld->JSONObject, "$.BasePath",
             bld->PathBuffer, bld->PathBufferSize, &bld->PathStringSize);
    if(result != JSON_SUCCESS){
        printf("Error: Could not get basepath string\n"); 
	return;
    }

    createDirectoryTemplate("$.RootFolderSchema",
       bld->PathBuffer, dirStack, 4, &bld->JSONObject);

    json_query_get_string(&bld->JSONObject, "$.RootFolderSchema.Dirs[0].Name", projDirName, 32, &size); 

    bld->PathBuffer[bld->PathStringSize] = '\\';
    bld->PathStringSize++;

    memcpy(&bld->PathBuffer[bld->PathStringSize], projDirName, size+1);
    bld->PathStringSize += size;
    bld->PathEnd = &bld->PathBuffer[bld->PathStringSize];

}

/*
int config_callback(void * NOT_USED, const char * cfgfile)
{
struct bld_struct *bld = NOT_USED;
DWORD attr;
struct fileObject fo;
JSON_OBJECT obj = {0}, *pobj = &obj;
struct json_node *json_node;

char path[256];
int len;

    bld->cfgfile = cfgfile;

    json_init(pobj, json_alloc, json_errorlog);
    json_load(pobj, fileptr, &json_node);

    //should be checking if key exists, if so then set flag
    json_query(pobj, "$.basepath", path, 256, &len);
    path[len] = '\0';

    if(set_basepath(bld, path, len, FLG1_BASE_PATH_SET_CONFIG)){
        printf("Invalid path\n"); 
        return 1;
    }

    unloadFileFromMemory(&fo);
    return 0;
}
*/


int basepath_callback(void* NOT_USED, const char * path)
{
struct bld_struct *bld = NOT_USED;

    return 0;
}

int help_callback(void *NOT_USED)
{
const char * help =
    "Command line tool to build projects\n\n"
    "BLD [options] <commands> [options]\n"; 

    printf(help);
    return 0;
}


int version_callback(void *NOT_USED)
{
struct bld_struct *bld = NOT_USED;

    switch(bld->version){
    case VERSION_1:
        printf("Version: 1.0.0\n");
	break;
    case VERSION_2:
        printf("Version: 2.0.0\n");
	break;
    }
    return 0;
}

int bld_callback(void *NOT_USED)
{
    printf("bld callback\n");
    return 1;
}

void add_project_to_database()
{
ISQLDatabase Db;

    Db = New(SQLiteDb);
    MasterDb_Connect(Db);
    //MasterDb_InsertProject(Db, 
}

#include "report-project.c"
#include "build-project.c"
#include "delete-project.c"
#include "new-project.c"

int report_callback(void *NOT_USED, const char *projname)
{
    report_project(0);
    return 1;
}

void handle_args2(int argc, char * argv[])
{
DWORD attr;
struct fileObject fo;
struct json_parser_state parser_state = {0};

    char *sqlTail;
    int rslt;

    const char *sql = "";
    char *filePtr;

    char buf[256];
    short count;

    struct bld_struct blds = {0};
    struct json_node *json_node;
    int size;

    //all structs must be zero-ed out
    struct atom * atomTable[ATOM_TABLE_SIZE] = {0};
    struct json_value value_pool[512] = {0};
    struct atom atom_pool[512] = {0};

    struct json_object * pobj = &blds.JSONObject;
    pobj->value_pool = value_pool;
    pobj->value_pool_size = 512;
    pobj->atom_pool = atom_pool;
    pobj->atom_pool_size = 512;
    pobj->atom_table = atomTable;
    pobj->atom_table_size = ATOM_TABLE_SIZE;

    char * file = loadFileInMemory(".\\config.json", &fo);
    if(!file){
        printf("Error: Could not load file into memory\n"); 
        return ;
    }

    rslt = json_read(&parser_state, pobj, file); 
    if(rslt != JSON_SUCCESS){
        printf("Error: Could not read json\n"); 
        return ;
    }

    rslt = json_query_get_string(pobj, "$." MSVC_C_COMPILER, buf, 256, &count);
    if(rslt != JSON_SUCCESS) {
        printf("Error: Could not read compiler\n");
        return; 
    }
    CompileProject(pobj, buf, count);
    printf("\n");

    rslt = json_query_get_string(pobj, "$." MSVC_LINKER, buf, 256, &count);
    if(rslt != JSON_SUCCESS) {
        return; 
    }
    LinkProject(pobj, buf, count);

    unloadFileFromMemory(&fo);

    INIT_MODULES(mod, 5);

    char _path[512];
    blds.PathBuffer = _path;
    blds.PathBufferSize = 512;

    bonec_initEnv(&blds);

    SET_MODULE(mod, 0, "bld", bld_callback, 0);
    SET_MODULE(mod, 1, "new-proj", new_callback, 1);
    SET_MODULE(mod, 2, "build", build_callback, 0);
    SET_MODULE(mod, 3, "delete", delete_callback, 0);
    SET_MODULE(mod, 4, "report", report_callback, 1);

    CREATE_ARG(mod, 0, basepath, 'B', "basepath", basepath_callback, 1); 
    //CREATE_ARG(mod, 0, config, 'c', "config", config_callback, 1);
    CREATE_ARG(mod, 0, help, 'h', "help", help_callback, 0);
    CREATE_ARG(mod, 0, version, 'v', "version", version_callback, 0);
    
    CREATE_ARG(mod, 1, new_help, 'h', "help", help_callback, 0);
    CREATE_ARG(mod, 1, new_version, 'v', "version", version_callback, 0);
    //CREATE_ARG(mod, 1, new_vers_control, 'n', set_version_ctrl, 0);
    CREATE_ARG(mod, 1, new_proj_type, 'T', "type", new_projtype_callback, 1);


    blds.version = VERSION_2;
    handle_commandline(argc, argv, ARRAY_SIZE(mod), mod, &blds); 
}

int main(int argc, char **argv)
{


struct fileObject fo;

    //loadFileInMemory(BASE_CONFIG_FILE, &fo);
    //unloadFileFromMemory(&fo);

    handle_args2(argc-1, argv+1); 

    return 1;
}
