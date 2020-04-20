#include <windows.h>
#include <stdio.h>

/*
** GIT COMMANDS
** git init
** git add
** git commit
** git status
** git config
** git branch
** git checkout
** git merge
** git remote
** git clone
** git pull
** git push
** git stash
** git log
** git rm
*/

enum {
SHARED_FALSE = 0,
SHARED_TRUE,
SHARED_UMASK,
SHARED_GROUP,
SHARED_ALL,
SHARED_WORLD,
SHARED_EVERYBODY,
};

struct git_init
{
const char *binary;
int binsz;

// bit field to turn on the quiet option 
char quiet: 1;
// create a bare repository
char bare: 1;

const char *symlink;
int symlsz;

const char *template;
int templsz;

const char *directory;
int dirsz;
};

struct git_add
{
const char *binary;
int binsz;
char verbose: 1;
char dry_run: 1;
char force:   1;
char *files[8];
unsigned file_count;
};

//git --version git version string
//git -C<path>  change working directory
//git -c<name>=<value>  Pass a configuration command
//git --exec-path[=<path>] print or set the dir of core git programs
//git --html-path print the path of git html docs
//git --man-path  print the path for man pages
//git --info-path  print the path of info files
//git --git-dir=<path> set the repository path
//git --work-tree=<path> set the path to the work tree
//git --bare treat repository as bare

char *copy_git_binary(char buf[], const char *bin)
{
unsigned len;
char *ptr = buf;

    len = strlen(bin);
    memcpy(ptr, bin, len);
    ptr += len;
    return ptr;
}

unsigned run_process(char cmdline[])
{
DWORD success, error;
STARTUPINFO startup_info = {0};
PROCESS_INFORMATION process_info = {0};

    startup_info.cb = sizeof(startup_info);
    startup_info.dwFlags = STARTF_USESTDHANDLES;
    startup_info.hStdOutput = stdout;
    success = CreateProcessA
    (0,
     cmdline,
     0, /* process security */
     0, /* thread security */
     1, /* allowed to use inherited handles? */
     CREATE_NO_WINDOW, /* creation flags */
     0, /* environment */
     0, /* current directory */
     &startup_info,
     &process_info);

    if(!success){
        error = GetLastError();
        printf("Error %d: CreateProcessA failed\n", error);
        return 0;
    }
    WaitForSingleObject(process_info.hProcess, INFINITE);
    return 1; 
}

//GitInit();
//GitClone();
//GitAdd();
//GitMv();
//GitReset();
//GitRm();
//GitBisect();
//GitGrep()
//GitLog()
//GitShow()
//GitStatus()
//GitBranch()
//GitCheckout()
//GitCommit()
//GitDiff()
//GitMerge()
//GitRebase()
//GitTag()
//GitFetch()
//GitPull()
//GitPush()
//GitTag()
//GitRemote()
//GitStash()
unsigned git_init(struct git_init *gi)
{
DWORD success, error;
char buf[512], *ptr;
int len;

    ptr = copy_git_binary(buf, "git");

    memcpy(ptr, " init", 5);
    ptr += 5;

    if(gi->quiet){
        memcpy(ptr, " -q", 3); 
        ptr += 3;
    }

    if(gi->bare){
        memcpy(ptr, " --bare" , 7);
        ptr += 7;
    }

    if(gi->template){
        if(gi->templsz) len = gi->templsz;
        else len = strlen(gi->template);
        memcpy(ptr, " --template=", 13);
        ptr += 13;
        memcpy(ptr, gi->template, len);
        ptr += len;
    }

    if(gi->symlink){
        if(gi->symlsz) len = gi->symlsz;
        else len = strlen(gi->symlink);
        memcpy(ptr, " --separate-git-dir=", 20);
        ptr += 20;
        memcpy(ptr, gi->symlink, len);
        ptr += len;
    }

    if(gi->directory){
        *ptr++ = ' ';
        if(gi->dirsz) len = gi->dirsz;
        else len = strlen(gi->directory);
        memcpy(ptr, gi->directory, len);
        ptr += len;
    }
    ptr[0] = 0;
    run_process(buf);
    return 1;
}

unsigned git_add(struct git_add *ga)
{
DWORD success, error;
char buf[512], *ptr;
int len;


    ptr = copy_git_binary(buf, "git");

    memcpy(ptr, " add", 4);
    ptr += 4;

    if(ga->verbose){
        memcpy(ptr, " -v", 3);
        ptr += 3;
    }

    if(ga->dry_run){
        memcpy(ptr, " -n", 3);
        ptr += 3;
    }

    if(ga->force){
        memcpy(ptr, " -f", 3);
        ptr += 3;
    }
 
//Add files
    for(int i = 0; i < ga->file_count; i++)
    {
        *ptr++ = ' ';
        len = strlen(ga->files[i]);
        memcpy(ptr, ga->files[i], len); 
        ptr += len;
    }

    *ptr = 0;
    run_process(buf);
    return 1;
}

void git_add_all()
{
    run_process("git add --all");
}

void git_status()
{
    run_process("git status");
}

unsigned git_commit(struct git_commit *gc)
{
    return 1;
}

struct git_status{
char shortout: 1;
char verbose: 1;
};

/*
unsigned git_commit(struct git_commit *gc)
{
    return 1;
}

unsigned git_status(struct git_status *gs)
{
    return 1;
}
*/

