#include <stdio.h>
#include "IVisualCCompiler.h"
#include "process.h"

typedef struct {
  IVisualCCompiler *class; 

  struct LayerOSProcess process;
  struct LayerOSProcess *pproc;

} VisualCCompilerObject;


static int AddIncludeDir(
  VisualCCompilerObject * vc,const char * string ,unsigned length
){
    char buf[256], *ptr = buf;
    memcpy(ptr, "/I\"", 3);
    ptr += 3;
    memcpy(ptr, string, length);
    ptr += length;
    *ptr++ = '\"';
    *ptr = '\0';
    if(layerOS_fileExists(string))
        layerOS_processAddArg(vc->pproc, buf, 0);
    else
        printf("Error: dir '%s' does not exists\n", string);
    return 1;
}

static int SetOutputName(
  VisualCCompilerObject * vc ,const char * string ,unsigned length
){
    char buf[256], *ptr = buf;
    memcpy(ptr, "/Fo\"", 4);
    ptr += 4;
    memcpy(ptr, string, length);
    ptr += length;
    *ptr++ = '\"';
    *ptr = '\0';
    layerOS_processAddArg(vc->pproc, buf, 0);
    return 1;
}

static int AddIncludeFile(
  VisualCCompilerObject * vc ,const char * string ,unsigned length 
){
    char buf[256], *ptr = buf;
    memcpy(ptr, "/FI\"", 4);
    ptr += 4;
    memcpy(ptr, string, length);
    ptr += length;
    *ptr++ = '\"';
    *ptr = '\0';

    if(layerOS_fileExists(string))
        layerOS_processAddArg(vc->pproc, buf, 0);
    else
        printf("Error: file '%s' does not exists\n", string);
 
    return 1;
}

static int DefineMacro(
  VisualCCompilerObject * vc
 ,const char * name 
 ,unsigned name_len 
 ,const char * text
 ,unsigned text_len 
){
    char buf[256], *ptr = buf;
    memcpy(ptr, "/D", 2);
    ptr += 2;
    memcpy(ptr, name, name_len); 
    ptr += name_len;
    *ptr++ = '=';
    memcpy(ptr, text, text_len);
    ptr += text_len;
    *ptr = '\0';
    layerOS_processAddArg(vc->pproc, buf, 0);
    return 1;
}

static int AddSourceFile(
  VisualCCompilerObject * vc ,const char * name ,unsigned length
){
    char buf[256], *ptr = buf;
    memcpy(ptr, name, length); 
    ptr += length;
    *ptr = '\0';
    if(layerOS_fileExists(name))
        layerOS_processAddArg(vc->pproc, buf, 0);
    else
        printf("Error: file '%s' does not exists\n", name);
    return 1;
}

static int AddSourceDir(
  VisualCCompilerObject * vc, const char *name, unsigned length		
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
        AddSourceFile(vc, filebuf, n);
    } while(FindNextFileA(hfind, &find_data));
    return 1;
}

static int SetDefaultCall(
  VisualCCompilerObject * vc ,unsigned type
){
const char *str;
    char buf[256], *ptr = buf;
    switch(type)
    {
    case CALLCONV_STDCALL:    str = "/Gz"; break;
    case CALLCONV_CDECL:      str = "/Gd"; break;
    case CALLCONV_VECTORCALL: str = "/Gv"; break;
    case CALLCONV_FASTCALL:   str = "/Gr"; break;
    default:                  str = "/Gd"; break;
    }
    memcpy(ptr, str, 3);
    ptr += 3;
    *ptr = '\0';
    layerOS_processAddArg(vc->pproc, buf, 0);
    return 1;
}

static int SetStructBoundary(
  VisualCCompilerObject * vc ,unsigned n
){
    char buf[256], *ptr = buf;
    if(n != 0){
        if(n != 1 || n != 2 || n != 4
        || n != 8 || n != 16){
        //error: struct must be aligned	
        }
    }
    memcpy(ptr, "/Zp", 3);
    ptr += 3;
    //conert number to string
    *ptr++ = '4';
    *ptr = '\0';
    layerOS_processAddArg(vc->pproc, buf, 0);
    return 1;
}

static int SetWorkingDirectory(
  VisualCCompilerObject *vc, const char *dir, unsigned dirlen
){
   layerOS_processSetDir(vc->pproc, dir, dirlen);
   return 1;
}

static int Exec(VisualCCompilerObject * vc)
{
    layerOS_processRun(vc->pproc);
    return 1;
}

static void * _ctor(DEBUG_DECL void *_obj, va_list args)
{
VisualCCompilerObject *obj;
char *path = va_arg(args, char *);
unsigned pathlen = va_arg(args, unsigned);

    obj = _obj ? _obj : malloc(sizeof(VisualCCompilerObject)); 

    obj->class = VisualCCompiler;

    obj->pproc = &obj->process;
    layerOS_processInit(obj->pproc, path, pathlen);
    layerOS_processAddArg(obj->pproc, "/c", 2);
    layerOS_processAddArg(obj->pproc, "/nologo", 7);
    layerOS_processAddArg(obj->pproc, "/J", 2);

    return obj;
}

static void _dtor(DEBUG_DECL void * obj)
{}

static int SizeOf()
{
    return sizeof(VisualCCompilerObject);
}

static IVisualCCompiler _ = {
  .Ctor                = _ctor,
  .Dtor                = _dtor,
  .SizeOf              = SizeOf,
  .SetOutputName       = SetOutputName,
  .AddIncludeDir       = AddIncludeDir,
  .AddIncludeFile      = AddIncludeFile,
  .DefineMacro         = DefineMacro,
  .AddSourceDir        = AddSourceDir,
  .AddSourceFile       = AddSourceFile,
  .SetDefaultCall      = SetDefaultCall,
  .SetWorkingDirectory = SetWorkingDirectory,
  .SetStructBoundary   = SetStructBoundary,
  .Exec                = Exec 
};

void * VisualCCompiler = &_;
