#ifndef IFILE_H
#define IFILE_H

#include "IClass.h"

typedef void (*DIRECTORY_FN)(void *obj, const char *name);
typedef void (*PARAM_ONE)(void *obj);

#define DEFINE_IDIRECTORY \
INHERIT_ICLASS \
void * (*GetFile)(DEBUG_DECL void *obj, const char *name);\
DIRECTORY_FN AddFile;\
DIRECTORY_FN RenameFile;\
DIRECTORY_FN DeleteFile;\
DIRECTORY_FN GetDir;\
DIRECTORY_FN AddDir;\
DIRECTORY_FN DeleteDir;\
DIRECTORY_FN RenameDir;


#define DEFINE_IFILE \
INHERIT_ICLASS \
void (*Open)(DEBUG_DECL void *obj);\
void (*Close)(DEBUG_DECL void *obj);\
void *(*Map)(DEBUG_DECL void *obj);\
void (*UnMap)(DEBUG_DECL void *obj);

#define INHERIT_IFILE DEFINE_IFILE

typedef struct {
DEFINE_IDIRECTORY
} IDirectory;

typedef struct {
DEFINE_IFILE
} IFile;


#define Dir_GetFile(OBJ, NAME) \
    _Dir_GetFile(DEBUG_PARAMS OBJ, NAME)
void * _Dir_GetFile(DEBUG_DECL void *obj, const char *name);

void   Dir_AddFile(void *obj, const char *name);
void   Dir_RenameFile(void *obj, const char *name, const char *newName);
void   Dir_DeleteFile(void *obj, const char *name);
void * Dir_GetDir(void *obj, const char *name);
void   Dir_AddDir(void *obj, const char *name);
void   Dir_DeleteDir(void *obj, const char *name);
void * Dir_GetParentDir(void *obj);
void   Dir_RenameDir(void * obj, const char *name, const char *newName);

//void Dir_Iterate

#define File_Open(OBJ) \
    _File_Open(DEBUG_PARAMS OBJ)
void _File_Open(DEBUG_DECL void *obj);

#define File_Close(OBJ) \
    _File_Close(DEBUG_PARAMS OBJ)
void _File_Close(DEBUG_DECL void *obj);

#define File_Map(OBJ) \
    _File_Map(DEBUG_PARAMS OBJ)
void *_File_Map(DEBUG_DECL void *obj);

#define File_UnMap(OBJ) \
    _File_UnMap(DEBUG_PARAMS OBJ)
void _File_UnMap(DEBUG_DECL void *obj);

extern void * Directory;
extern void * File;

#endif

