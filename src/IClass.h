#ifndef ICLASS_H
#define ICLASS_H

#include <windows.h>
#include <stdarg.h>

#define DEBUG_DECL const char *__file__, unsigned long __line__,
#define DEBUG_PARAMS __FILE__, __LINE__,
#define DEBUG_INNER_PARAMS __file__, __line__,

typedef void * (*CLASS_CTOR)(DEBUG_DECL void *, va_list); 
typedef void (*CLASS_DTOR)(DEBUG_DECL void *);
typedef int (*CLASS_SIZEOF)();


#define DEFINE_ICLASS \
CLASS_CTOR Ctor;\
CLASS_DTOR Dtor;\
CLASS_SIZEOF SizeOf;

#define INHERIT_ICLASS DEFINE_ICLASS

#define DEFINE_ICLASS_OBJECT \
void * class;

typedef struct {
DEFINE_ICLASS
} ICLASS;

typedef struct {
DEFINE_ICLASS_OBJECT
} OBJECT;

#define New(CLASS, ...) _New(DEBUG_PARAMS CLASS, __VA_ARGS__) 
void *_New(DEBUG_DECL void *class, ...);

#define Delete(OBJECT) _Delete(DEBUG_PARAMS OBJECT)
void _Delete(DEBUG_DECL void *obj);

#define ASSERT(x) \
if(!(x)){\
printf("Error in file %s (%u)\n", __file__, __line__);\
ExitProcess(0);\
}

//int SizeOf(void *obj);

#endif
