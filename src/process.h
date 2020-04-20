#ifndef PROCESS_H
#define PROCESS_H

#define MAX_COMMAND_LINE 32767
#define MAX_ENVIRONMENT 32760

enum {
    PROCESS_SET_DIRECTORY
   ,PROCESS_ADD_ARG
};

struct LayerOSProcess{
  void *virtual_table;
  char CommandLine[MAX_COMMAND_LINE];
  //char Environment[MAX_ENVIRONMENT];
  char WorkingDirectory[256];
  char *cmdline_tail;
};

struct ProcessVirtualTable {
  int (*init)(struct LayerOSProcess *);
  int (*doCommand)(struct LayerOSProcess *, int command, int flags, ...);
  int (*runProcess)(struct LayerOSProcess*);
};

void layerOS_processInit(struct LayerOSProcess *, const char *, unsigned);
void layerOS_processAddArg(struct LayerOSProcess *, const char *, unsigned);
void layerOS_processSetDir(struct LayerOSProcess *, const char *, unsigned);
void layerOS_processRun(struct LayerOSProcess *);

int layerOS_fileExists(const char *file);

#endif
