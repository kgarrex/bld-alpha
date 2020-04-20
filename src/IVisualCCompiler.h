#ifndef WINCL_INTERFACE
#define WINCL_INTERFACE

#include <windows.h>
#include "ICompiler.h"

#define DEFINE_IVISUALCCOMPILER \
DEFINE_ICOMPILER

typedef struct {
  DEFINE_IVISUALCCOMPILER
} IVisualCCompiler;

extern void * VisualCCompiler;

#endif
