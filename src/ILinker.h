#ifndef ILINKER_H
#define ILINKER_H

#include "IClass.h"

typedef void (*LINKER_STRING_FN)(void*, const char *, unsigned);
typedef void (*LINKER_STRING_FN)(void*, const char *, unsigned);
typedef void (*LINKER_FUNCTION)(void*);

#define DEFINE_ILINKER \
INHERIT_ICLASS \
LINKER_STRING_FN AddLibDir;\
LINKER_STRING_FN AddObjectFile;\
LINKER_STRING_FN AddObjectDir;\
LINKER_STRING_FN SetWorkingDirectory;\
LINKER_STRING_FN SetStackSize;\
LINKER_FUNCTION  Exec;\

#define DEFINE_ILINKER_OBJECT \
DEFINE_ICLASS_OBJECT

typedef struct {
  DEFINE_ILINKER
} ILinker;

void Linker_AddLibDir(void *obj, const char * str, unsigned len);
void Linker_AddObjectFile(void *obj, const char *str, unsigned len);
void Linker_AddObjectDir(void *obj, const char *str, unsigned len);
void Linker_SetWorkingDirectory(void *obj, const char *str, unsigned len);
void Linker_SetStackSize(void *obj, const char *str, unsigned len);
void Linker_Exec(void *obj);

#endif
