
#include <stdio.h>
#include "IVisualCLinker.h"
#include "process.h"

typedef struct {
  DEFINE_ILINKER_OBJECT

  struct LayerOSProcess process;
  struct LayerOSProcess *pproc;

} VisualCLinkerObject;


static void * Ctor(DEBUG_DECL void * _obj, va_list args)
{
VisualCLinkerObject *obj;
//char * buf = va_arg(args, char*);
//unsigned bufsz = va_arg(args, unsigned);
char * path = va_arg(args, char *); //path to linker executable 
unsigned pathlen = va_arg(args, unsigned); //path size

    obj = _obj ? _obj : malloc(sizeof(VisualCLinkerObject)) ;

    obj->class = VisualCLinker;

    obj->pproc = &obj->process;
    layerOS_processInit(obj->pproc, path, pathlen);
    layerOS_processAddArg(obj->pproc, "/NOLOGO", 7);

    return obj;
}

static void Dtor(DEBUG_DECL void *obj)
{}


static void AddLibDir(VisualCLinkerObject *wl, const char * str, unsigned len)
{
    char buf[256], *ptr = buf;
    memcpy(ptr, "/LIBPATH:\"", 10);  
    ptr += 10;
    memcpy(ptr, str, len);
    ptr += len;
    *ptr++ = '\"';
    *ptr = '\0';

    if(layerOS_fileExists(str))
        layerOS_processAddArg(wl->pproc, buf, 0);
    else
        printf("Error: dir '%s' does not exists\n", str);
 
}

static void AddObjectFile(VisualCLinkerObject *wl, const char * str, unsigned len)
{
    char buf[256], *ptr = buf;
    memcpy(ptr, str, len);
    ptr += len;
    *ptr = '\0';
    layerOS_processAddArg(wl->pproc, buf, 0);
}

static void SetStackSize(VisualCLinkerObject *wl, const char *str, unsigned len)
{
    char buf[256], *ptr = buf;
    memcpy(ptr, "/STACK:", 7);
    ptr += 7;
    memcpy(ptr, str, len);
    ptr += len;
    *ptr = '\0';
    layerOS_processAddArg(wl->pproc, buf, 0);
}

static void AddObjectDir(VisualCLinkerObject *wl, const char *name, unsigned len)
{
    int n;
    char filebuf[256];
    HANDLE hfind;
    WIN32_FIND_DATA find_data;
    sprintf(filebuf, "%s\\*.obj", name);
    hfind = FindFirstFileA(filebuf, &find_data);
    if(INVALID_HANDLE_VALUE == hfind){
        printf("Error: could not find file (%d)\n", GetLastError());
        return;
    }
    do {
        n = sprintf(filebuf, "%s\\%s", name, find_data.cFileName);
        AddObjectFile(wl, filebuf, n); 
    } while(FindNextFileA(hfind, &find_data));
}

static void SetWorkingDirectory(
  VisualCLinkerObject *wl, const char *dir, unsigned dirlen
){
   layerOS_processSetDir(wl->pproc, dir, dirlen);
}



static void Exec(VisualCLinkerObject *wl)
{
    layerOS_processRun(wl->pproc);
}

static int SizeOf()
{
    return sizeof(VisualCLinkerObject);
}

static IVisualCLinker _ = {
  .Ctor                = Ctor,
  .Dtor                = Dtor,
  .SizeOf              = SizeOf,
  .AddLibDir           = AddLibDir, 
  .AddObjectFile       = AddObjectFile,
  .AddObjectDir        = AddObjectDir,
  .SetStackSize        = SetStackSize,
  .SetWorkingDirectory = SetWorkingDirectory,
  .Exec                = Exec 
};

void * VisualCLinker = &_;
