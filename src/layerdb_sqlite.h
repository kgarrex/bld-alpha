#ifndef LAYERDB_SQLITE_H
#define LAYERDB_SQLITE_H

#include <windows.h>
#include "layerdb.h"

#define DEFINE_ISQLITEDB \
INHERIT_ISQLDATABASE

typedef struct _ISQLiteDb {
  DEFINE_ISQLITEDB
} *ISQLiteDb;

extern void * SQLiteDb;

#endif
