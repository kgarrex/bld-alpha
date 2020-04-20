#include <stdio.h>
#include "ClangCompiler.h"

#define CLANG_MAX_CREATE_PROCESS 32767
char cmdline[CLANG_MAX_CREATE_PROCESS+1];

typedef struct {
    IClangCompiler * class;

    struct LayerOSProcess process;
    struct LayerOSProcess *pproc;

} ClangCompilerObject;

static int AddIncludeDir(
  ClangCompilerObject *obj
 ,const char * string
 ,unsigned length
){
    char buf[256], *ptr = buf; 
    memcpy(ptr, "-I", 2);
    ptr += 2;
    memcpy(ptr, string, length);
    ptr += length;
    *ptr = '\0';
    layerOS_processAddArg(obj->pproc, buf, 0);
    return 1;
}

static int SetOutputName(
  ClangCompilerObject *obj
 ,const char *file
 ,unsigned length
){
    char buf[256], *ptr = buf;
    memcpy(ptr, "-o", 2);
    ptr += 2;
    memcpy(ptr, file, length);
    ptr += length;
    *ptr = '\0';
    layerOS_processAddArg(obj->pproc, buf, 0);
    return 1;
}

static int AddIncludeFile(
  ClangCompilerObject *obj
 ,const char *string
 ,unsigned length 
){
    char buf[256], *ptr = buf;
    memcpy(ptr, "-include", 8);
    ptr += 8;
    memcpy(ptr, string, length);
    ptr += length;
    *ptr = '\0';
    layerOS_processAddArg(obj->pproc, buf, 0);
    return 1;
}

static int DefineMacro(
  ClangCompilerObject *obj		
 ,const char * name
 ,unsigned name_len
 ,const char * value 
 ,unsigned value_len
){
    char buf[256], *ptr = buf;
    memcpy(ptr, "-D", 2);
    ptr += 2;
    memcpy(ptr, name, name_len);
    ptr += name_len;
    *ptr++ = '=';
    memcpy(ptr, value, value_len);
    ptr += value_len;
    *ptr = '\0';
    layerOS_processAddArg(obj->pproc, buf, 0);
    return 1;
}

static int AddSourceFile(
  ClangCompilerObject *obj
 ,const char *file,
 ,unsigned length
){
    char buf[256], *ptr = buf;
    memcpy(ptr, file, length);
    ptr += length;
    *ptr = '\0'; 
    layerOS_processAddArg(obj->pproc, buf, 0);
    return 1;
}

static int AddSourceDir(
  ClangCompilerObject *obj, const char *name, unsigned length		
){
    int n;
    char filebuf[256];
    HANDLE hfind;
    WIN32_FIND_DATA find_data;
    //go through the directory and add each source file using AddSourceFile
    sprintf(filebuf, "%s\\*.c", name);
    hfind = FindFirstFileA(filebuf, &find_data);
    if(INVALID_HANDLE_VALUE == hfind){
        printf("Error: could not find file (%d)\n", GetLastError()); 
        return 0;
    }
    do {
        n = sprintf(filebuf, "%s\\%s", name, find_data.cFileName);
        AddSourceFile(obj, filebuf, n);
    } while(FindNextFileA(hfind, &find_data));
    return 1;
}

static int SetWorkingDirectory(
  ClangCompilerObject *obj, const char *dir, unsigned dirlen
){
   layerOS_processSetDir(obj->pproc, dir, dirlen);
   return 1;
}


static int StructBoundary(
  ClangCompilerObject *obj
 ,unsigned n  
){
    //-fpack-struct | -fno-pack-struct
    //-fpack-struct=<arg>
}

static int Exec(ClangCompiler *obj)
{
    layerOS_processRun(obj->pproc);
    return 1;
}

static void * _ctor(DEBUG_DECL  void *_obj, va_list args)
{
ClangCompilerObject *obj;

char *buf = va_arg(args, char*);
unsigned bufsz = va_arg(args, unsigned);
char *path = va_arg(args, char*);
unsigned pathlen = va_arg(args, unsigned);

    obj = _obj ? _obj : malloc(sizeof(ClangCompilerObject));

    obj->class = ClangCompiler;

    obj->pproc = &obj->process;
    layerOS_processInit(obj->pproc, path, pathlen);
    layerOS_processAddArg(obj->pproc, "-c", 2);

    return obj;
}


static IClangCompiler _ = {
  .Ctor                = _ctor,
  .Dtor                = _dtor,
  .SizeOf              = SizeOf,
  .SetOutputName       = SetOutputName,
  .AddIncludeDir       = AddIncludeDir,
  .AddIncludeFile      = AddIncludeFile,
  .DefineMacro         = DefineMacro,
  .AddSourceFile       = AddSourceFile,
  .SetDefaultCall      = SetDefaultCall,
  .SetStructBoundary   = SetStructBoundary,
  .SetWorkingDirectory = SetWorkingDirectory,
  .Exec                = Exec 
};

void * ClangCompiler = &_;
