#include "IFileSystem.h"

void * _Dir_GetFile(DEBUG_DECL void *obj, const char *name)
{
IDirectory * dir;
    dir = *((IDirectory**)obj);
    ASSERT(dir->GetFile);
    return dir->GetFile(DEBUG_INNER_PARAMS obj, name);
}

void _File_Open(DEBUG_DECL void *obj)
{
IFile * file;
    file = *((IFile**)obj);
    ASSERT(file->Open);
    file->Open(DEBUG_INNER_PARAMS obj);
}

void _File_Close(DEBUG_DECL void *obj)
{
IFile * file;
    file = *((IFile**)obj);
    ASSERT(file->Close);
    file->Close(DEBUG_INNER_PARAMS obj);
}

void *_File_Map(DEBUG_DECL void *obj)
{
IFile * file;
    file = *((IFile**)obj);
    ASSERT(file->Map);
    return file->Map(DEBUG_INNER_PARAMS obj);
}

void _File_UnMap(DEBUG_DECL void *obj)
{
IFile * file;
    file = *((IFile**)obj);
    ASSERT(file->UnMap);
    file->UnMap(DEBUG_INNER_PARAMS obj);
}
