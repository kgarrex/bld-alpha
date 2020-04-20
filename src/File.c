
#include <windows.h>
#include "IFileSystem.h"

typedef struct {
    void *class;

    char path[256];
    unsigned pathLen;
    char *endOfPath;
    HANDLE hdir;
} DirectoryObject;

typedef struct {
  void *class;

  DirectoryObject *parentDir;

  char name[32];
  HANDLE hfile;
  HANDLE hfmap;
  void *map;
  unsigned nameLen;

} FileObject;

static int _DirectoryExists(const char *szPath)
{
    unsigned dwAttrib = GetFileAttributes(szPath);
    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
            dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

static void * DirCtor(DEBUG_DECL void *_obj, va_list args)
{
DirectoryObject *obj = _obj;
char *szPath;

    obj->class = Directory;
    szPath = va_arg(args, char *);

    if(!_DirectoryExists(szPath)) return 0;
    obj->pathLen = strlen(szPath);
    memcpy(obj->path, szPath, obj->pathLen);
    obj->endOfPath = obj->path + obj->pathLen;
    *obj->endOfPath = '\0';
    //printf("%s\n", obj->path);
    return obj;
}

static void DirDtor(DEBUG_DECL void *_obj)
{}

static unsigned DirSizeOf()
{
    return sizeof(DirectoryObject);
}

static FileObject *DirGetFile(DEBUG_DECL
    DirectoryObject *obj, const char *name)
{
FileObject * fobj;
WIN32_FIND_DATAA find;
unsigned nameLen;
HANDLE hfind;
     
    nameLen = strlen(name);
    memcpy(obj->endOfPath, name, nameLen);
    (obj->endOfPath)[nameLen] = '\0';
    hfind = FindFirstFile(obj->path, &find);
    if(hfind == INVALID_HANDLE_VALUE){
        printf("Error: File not found\n");
        ASSERT(0);
    }
    fobj = New(File, obj, name);
    FindClose(hfind);
    return fobj;
}

static void *FileCtor(DEBUG_DECL void *_obj, va_list args)
{
FileObject *this = _obj;
char *szName;

    this->class = File;
    this->parentDir = va_arg(args, DirectoryObject *);
    szName = va_arg(args, char *);

    this->nameLen = strlen(szName);
    memcpy(this->name, szName, this->nameLen); 
    this->name[this->nameLen] = '\0';
    this->hfile = INVALID_HANDLE_VALUE;
    this->hfmap = 0;
    this->map = 0;

    return this;
}

static void FileDtor(void * _obj)
{}

static unsigned FileSizeOf()
{
    return sizeof(FileObject);
}

static void Open(DEBUG_DECL FileObject *this)
{
SECURITY_ATTRIBUTES sa = {0};
DirectoryObject * dir;
    dir = this->parentDir;
    memcpy(dir->endOfPath, this->name, this->nameLen);
    dir->endOfPath[this->nameLen] = '\0';

    sa.nLength = sizeof(sa);
    sa.bInheritHandle = 1;
    this->hfile = CreateFileA(dir->path,
        GENERIC_READ | GENERIC_WRITE, 0, &sa,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if(INVALID_HANDLE_VALUE == this->hfile){
        printf("Error: Could not open file\n"); 
    }
}

static void Close(DEBUG_DECL FileObject *obj)
{
    CloseHandle(obj->hfile);
}

static void *Map(DEBUG_DECL FileObject *this)
{
    if(INVALID_HANDLE_VALUE == this->hfile){
        printf("Must open the file first\n"); 
        ASSERT(0);
    }
    this->hfmap = CreateFileMappingA(
        this->hfile, 0, PAGE_READWRITE, 0, 0, 0
    );
    ASSERT(this->hfmap);
    this->map = MapViewOfFile(this->hfmap,
        FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
    ASSERT(this->map);
    return this->map;
}

static void UnMap(DEBUG_DECL FileObject *obj)
{
    UnmapViewOfFile(obj->map);
    CloseHandle(obj->hfmap);
}

static IDirectory _ = {
  .Ctor = DirCtor,
  .Dtor = DirDtor,
  .SizeOf = DirSizeOf,
  .GetFile = DirGetFile
};
void * Directory = &_;

static IFile __ = {
  .Ctor = FileCtor,
  .Dtor = FileDtor,
  .SizeOf = FileSizeOf,
  .Open = Open,
  .Close = Close,
  .Map  = Map,
  .UnMap = UnMap,
};

void * File = &__;
