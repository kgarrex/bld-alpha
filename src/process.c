
#include <windows.h>
#include "process.h"

/* NOTE: It is important that the program name contains not leading spaces */
void layerOS_processInit(struct LayerOSProcess *proc, const char *prog, unsigned len)
{
    proc->cmdline_tail = proc->CommandLine;
    if(layerOS_fileExists(prog)) {
        len = len ? len : strlen(prog);
        memcpy(proc->cmdline_tail, prog, len);
	proc->cmdline_tail += len;
	*proc->cmdline_tail = '\0';
    }
    else {
        printf("Error: program does not exists\n");
    }
    proc->WorkingDirectory[0] = '\0';
}

void layerOS_processAddArg(struct LayerOSProcess *proc,
  const char *arg, unsigned size)
{
    size = size ? size : strlen(arg);
    *proc->cmdline_tail++ = ' ';
    memcpy(proc->cmdline_tail, arg, size);
    proc->cmdline_tail+= size;
    *proc->cmdline_tail = '\0';
}

void layerOS_processSetDir(struct LayerOSProcess *proc,
	const char *dir, unsigned size)
{
    memcpy(proc->WorkingDirectory, dir, size);
}

void layerOS_processRun(struct LayerOSProcess *proc)
{
union { int success; int error; } r;

    HANDLE hFile;
    SECURITY_ATTRIBUTES sa;
    STARTUPINFO startupInfo = {0};
    PROCESS_INFORMATION processInfo = {0};

    startupInfo.cb = sizeof(startupInfo);
    startupInfo.dwFlags |= STARTF_USESTDHANDLES;
    startupInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    startupInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    startupInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);

   // startupInfo.hStdOutput = 0;
   // startupInfo.hStdInput = 0;
   // startupInfo.hStdError = 0;


    sa.nLength = sizeof(sa);
    sa.bInheritHandle = 1;
    sa.lpSecurityDescriptor = 0;

    //printf("Command Line: %s\n", proc->CommandLine);
    //printf("Directory: %s\n", proc->WorkingDirectory);
    r.success = CreateProcessA
    (0 //NOT USED
    ,proc->CommandLine
    ,&sa //Process Security
    ,&sa //Thread Security
    ,1
    ,0
    ,0
    ,(proc->WorkingDirectory[0] ? proc->WorkingDirectory : 0)
    ,&startupInfo
    ,&processInfo);

    if(!r.success){
        r.error = GetLastError();
        printf("Error %u: CreateProcessA failed\n", r.error);
        return;
    }
    WaitForSingleObject(processInfo.hProcess, INFINITE);
    //GetExitCodeProcess(process_info.hProcess, &exit_code);
}

int layerOS_fileExists(const char *file)
{
    DWORD attr;

    attr = GetFileAttributes(file);
    return attr == INVALID_FILE_ATTRIBUTES ? 0 : 1;
}

//layerOS_process_setopt(process, PROC_DIRECTORY, "C:\\windows");
//layeros_process_doCommand(, 
