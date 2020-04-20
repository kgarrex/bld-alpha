
#include "ILinker.h"

void Linker_AddLibDir(void *obj, const char * str, unsigned len)
{
ILinker *lk;
    lk = *((ILinker**)obj);
    lk->AddLibDir(obj, str, len);
}

void Linker_AddObjectDir(void *obj, const char *str, unsigned len)
{
ILinker *lk;
    lk = *((ILinker**)obj);
    lk->AddObjectDir(obj, str, len);
}

void Linker_AddObjectFile(void *obj, const char *str, unsigned len)
{
ILinker *lk;
    lk = *((ILinker**)obj);
    lk->AddObjectFile(obj, str, len); 
}

void Linker_SetStackSize(void *obj, const char *str, unsigned len)
{
ILinker *lk;
    lk = *((ILinker**)obj);
    lk->SetStackSize(obj, str, len);
}

void Linker_SetWorkingDirectory(void *obj, const char *dir, unsigned len)
{
ILinker *lk;
    lk = *((ILinker**)obj);
    lk->SetWorkingDirectory(obj, dir, len);
}

void Linker_Exec(void *obj)
{
ILinker *lk;
    lk = *((ILinker**)obj);
    lk->Exec(obj);
}
