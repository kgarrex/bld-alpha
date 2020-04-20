#include <stdio.h>
#include "ILLDLinker.h"

#define LLD_MAX_CREATE_PROCESS 32767
char cmdline[LLD_MAX_CREATE_PROCESS];

typedef struct {
  DEFINE_ILINKER_OBJECT

} LLDLinkerObject;

static void *ctor(DEBUG_DECL void *_obj, va_list args)
{

}

static void dtor(DEBUG_DECL void *obj)
{}

static void AddLibDir(LLDLinkerObject *lld, const char *str, unsigned len)
{
    
}

static void SetWorkingDirectory(
    LLDLinkerObject *lld, const char *dir, unsigned dirlen
){
    layerOS_processSetDir(lld->pproc, dir, dirlen);
}

static void Exec(LLDLinkerObject *lld)
{
    layerOS_processRun(lld->pproc);
}

static int SizeOf()
{
    return sizeof();
}

static ILLDLinker _ = {
  .Ctor                = ctor,
  .Dtor                = dtor,
  .SizeOf              = SizeOf,
  .AddLibDir           = AddLibDir, 
  .AddObjectFile       = AddObjectFile,
  .AddObjectDir        = AddObjectDir,
  .SetStackSize        = SetStackSize,
  .SetWorkingDirectory = SetWorkingDirectory,
  .Exec                = Exec 
};

void *LLDLinker = &_;

