#ifndef COMPILER_INTERFACE
#define COMPILER_INTERFACE

#include "IClass.h"

enum {
  CALLCONV_STDCALL
 ,CALLCONV_CDECL
 ,CALLCONV_VECTORCALL
 ,CALLCONV_FASTCALL
};

typedef struct _ICOMPILER ICOMPILER; 

#define DEFINE_ICOMPILER \
INHERIT_ICLASS \
int (__cdecl *SetOutputName)(void*, const char*, unsigned);\
int (__cdecl *AddIncludeDir)(void*, const char*, unsigned);\
int (__cdecl *AddIncludeFile)(void*, const char*, unsigned);\
int (__cdecl *AddSourceDir)(void*, const char*, unsigned);\
int (__cdecl *AddSourceFile)(void*, const char*, unsigned);\
int (__cdecl *DefineMacro)(void*, const char*, unsigned, const char*, unsigned);\
int (__cdecl *SetDefaultCall)(void*, unsigned);\
int (__cdecl *SetWorkingDirectory)(void *, const char *, unsigned);\
int (__cdecl *SetStructBoundary)(void*, unsigned);\
int (__cdecl *Exec)(void*);


#define DEFINE_ICOMPILER_OBJECT \
DEFINE_ICLASS_OBJECT

typedef struct _ICOMPILER {
  DEFINE_ICOMPILER 
} ICOMPILER;

int Compiler_SetOutpuName(void **obj, const char * str, unsigned len);
int Compiler_AddIncludeDir(void **obj, const char *str, unsigned len);
int Compiler_AddIncludeFile(void **obj, const char *str, unsigned len);
int Compiler_AddSourceDir(void **obj, const char *str, unsigned len);
int Compiler_AddSourceFile(void **obj, const char *str, unsigned len);
int Compiler_DefineMacro(void **obj, const char *name, unsigned name_len,
    const char *text, unsigned text_len);
int Compiler_SetDefaultCall(void **obj, unsigned type);
int Compiler_SetStructBoundary(void **obj, unsigned boundary);
int Compiler_SetWorkingDirectory(void **obj, const char *, unsigned );
int Compiler_Exec(void **obj);

#endif
