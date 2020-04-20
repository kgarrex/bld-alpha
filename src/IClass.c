
#include <stdlib.h>
#include <assert.h>
#include "IClass.h"

void * _New (DEBUG_DECL void * _class, ...)
{
ICLASS * class;
va_list args;
void * obj = 0;
int size;
    class = _class;
    ASSERT(class->SizeOf);
    size = class->SizeOf();
    if(size) obj = malloc(size);
    va_start(args, _class);
    ASSERT(class->Ctor);
    obj = class->Ctor(DEBUG_INNER_PARAMS obj, args);
    ASSERT(obj);
    va_end(args);
    return obj;
}

void _Delete (DEBUG_DECL void *obj)
{
ICLASS * class;
    class = *((ICLASS**)obj);
    ASSERT(class->Dtor);
    class->Dtor(DEBUG_INNER_PARAMS obj);
    if(class->SizeOf()) free(obj);
}
