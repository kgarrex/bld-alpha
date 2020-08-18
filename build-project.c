
int build_callback(void *NOT_USED)
{
    printf("Build\n");
    return 1;
}

int GetEachSourceDir(struct json_object *pobj, ICOMPILER *compiler)
{
    short count;
    int dirsize, rslt;
    char querybuf[64];
    char dirbuf[256];

    json_query_get_array_count(pobj, "$." SOURCE_DIRS,&count);
    for(int i = 0; i < count; i++)
    {
        sprintf(querybuf, "$." SOURCE_DIRS "[%d]", i); 
        rslt = json_query_get_string(pobj, querybuf, dirbuf, 256, &dirsize);
        Compiler_AddSourceDir(compiler, dirbuf, dirsize);
    }
    return 1;
}

int GetEachSourceFile(struct json_object *pobj, ICOMPILER *compiler)
{
    short count;
    int dirsize, rslt;
    char querybuf[64];
    char dirbuf[256];

    json_query_get_array_count(pobj, "$." SOURCE_FILES, &count);
    for(int i = 0; i < count; i++)
    {
        sprintf(querybuf, "$." SOURCE_FILES "[%d]", i);
        rslt = json_query_get_string(pobj, querybuf, dirbuf, 256, &dirsize);
        Compiler_AddSourceFile(compiler, dirbuf, dirsize);
    }
    return 1;
}

int GetEachIncludeDir(struct json_object *pobj, ICOMPILER *compiler)
{
    short count;
    int dirsize, rslt;
    char querybuf[64];
    char dirbuf[256];
 
    json_query_get_array_count(pobj, "$." INCLUDE_DIRS, &count);
    for(int i = 0; i < count; i++)
    {
        sprintf(querybuf, "$." INCLUDE_DIRS "[%d]", i);
	rslt = json_query_get_string(pobj, querybuf, dirbuf, 256, &dirsize); 
	if(rslt != 0){
            printf("Could not get string from json\n");	
            return 0;
        }
	Compiler_AddIncludeDir(compiler, dirbuf, dirsize);
    }
    return 1;
}

int GetEachIncludeFile(struct json_object *pobj, ICOMPILER *compiler)
{
    short count;
    int dirsize, rslt;
    char querybuf[64];
    char dirbuf[256];

    json_query_get_array_count(pobj, "$." INCLUDE_FILES, &count);
    for(int i = 0; i < count; i++)
    {
        sprintf(querybuf, "$." INCLUDE_FILES "[%d]", i);	    
        rslt = json_query_get_string(pobj, querybuf, dirbuf, 256, &dirsize);
	if(rslt != 0) {}
	Compiler_AddIncludeFile(compiler, dirbuf, dirsize);
    }
    return 1;
}

int GetEachMacro(struct json_object *pobj, ICOMPILER *compiler)
{
    short count;
    int dirsize, rslt;
    char querybuf[64];
    char name[64]; int namelen;
    char value[256]; int valuelen;

    json_query_get_array_count(pobj, "$." MACROS, &count);
    for(int i = 0; i < count; i++)
    {
        sprintf(querybuf, "$." MACROS "[%d].name", i);
        rslt = json_query_get_string(pobj, querybuf, name, 64, &namelen);
        if(rslt != 0) {}

        sprintf(querybuf, "$." MACROS "[%d].value", i);
	rslt = json_query_get_string(pobj, querybuf, value, 256, &valuelen);
	if(rslt != 0) {}

        Compiler_DefineMacro(compiler, name, namelen, value, valuelen);
    }
    return 1;
}

int GetStackSize(struct json_object *pobj, ILinker *linker)
{
    int rslt, sizelen;
    char size[8];

    rslt = json_query_get_string(pobj, "$." STACK_SIZE, size, 8, &sizelen);
    Linker_SetStackSize(linker, size, sizelen);
    return 1;
}

int GetEachLibDir(struct json_object *pobj, ILinker *linker)
{
    short count;
    int dirsize, rslt;
    char querybuf[64];
    char dirbuf[256];

    json_query_get_array_count(pobj, "$." LIB_DIRS, &count);
    for(int i = 0; i < count; i++)
    {
        sprintf(querybuf, "$." LIB_DIRS "[%d]", i); 
        rslt = json_query_get_string(pobj, querybuf, dirbuf, 256, &dirsize);
	if(rslt != 0) {}
        Linker_AddLibDir(linker, dirbuf, dirsize);
    }
    return 1;
}

int GetEachObjDir(struct json_object *pobj, ILinker *linker)
{
    short count;
    int dirsize, rslt;
    char querybuf[64];
    char dirbuf[256];

    json_query_get_array_count(pobj, "$." OBJECT_DIRS, &count);
    for(int i = 0; i < count; i++)
    {
        sprintf(querybuf, "$." OBJECT_DIRS "[%d]", i);
        rslt = json_query_get_string(pobj, querybuf, dirbuf, 256, &dirsize);
	if(rslt != 0){
            printf("Error: Could not get object directory\n");
            return 0;
        }
	Linker_AddObjectDir(linker, dirbuf, dirsize);
    }
    return 1;
}

void CompileProject(struct json_object *pobj, char *exepath, int pathsize )
{
    ICOMPILER *compiler;
    const char *curdir = "H:\\Dev\\code\\Environment\\projects\\Kendrick\\src\\obj";

    compiler = New(VisualCCompiler, exepath, pathsize);
    GetEachIncludeFile(pobj, compiler);
    GetEachIncludeDir(pobj, compiler);
    GetEachMacro(pobj, compiler);
    GetEachSourceDir(pobj, compiler);
    Compiler_SetWorkingDirectory(compiler, curdir, strlen(curdir)+1);
    Compiler_Exec(compiler);
    Delete(compiler);
}

void LinkProject(struct json_object *pobj, char *exepath, int pathsize)
{
    ILinker *linker;
    const char *curdir = "H:\\Dev\\code\\Environment\\projects\\Kendrick\\src\\bin";

    linker = New(VisualCLinker, exepath, pathsize);
    GetEachLibDir(pobj, linker);
    GetStackSize(pobj, linker);
    GetEachObjDir(pobj, linker);
    Linker_SetWorkingDirectory(linker, curdir, strlen(curdir)+1);
    Linker_Exec(linker);
    Delete(linker);
}
