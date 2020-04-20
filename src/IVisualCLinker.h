#ifndef WINLINK_H
#define WINLINK_H

#include <windows.h>
#include "ILinker.h"

#define DEFINE_IWINLINK \
DEFINE_ILINKER

typedef struct {
  DEFINE_IWINLINK
} IVisualCLinker;

extern void * VisualCLinker;

#endif
