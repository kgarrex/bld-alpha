#define MAX_NAME_LENGTH 16
#define MAX_ALIAS_LENGTH 3

#define ARRAY_SIZE(var) (sizeof(var)/sizeof(var[0]))

#define MAP_ARG(c)\
(c >= 'A' && c <= 'Z' ?\
c - 65 : (c >= 'a' && c <= 'z' ? c - 71 : 52));  

#define INIT_MODULES(name, cnt)\
struct module name[cnt] = {0}

#define SET_MODULE(mod, idx, str, fn, cnt)\
mod[idx].name = str;\
mod[idx].callback = fn;\
mod[idx].paramCount = cnt

#define CREATE_ARG(mod, idx, name, c, str, fn, pc)\
struct arg name = {0};\
name.callback = fn;\
name.long_name = str;\
name.paramCount = pc;\
name.hash = MAP_ARG(c);\
((mod[idx]).args)[name.hash] = &name

//printf("%c %d\n", c, name.hash);\

struct fileObject {
  HANDLE hFile;
  HANDLE hFmap;
  void *map;
};


void *loadFileInMemory(const char *file, struct fileObject *fo)
{
SECURITY_ATTRIBUTES sa = {0};

    fo->hFile = CreateFileA(file,
        GENERIC_READ | GENERIC_WRITE, 0, &sa,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if(INVALID_HANDLE_VALUE == fo->hFile){
        printf("Error: Could not open file\n"); 
	return 0;
    }

    fo->hFmap = CreateFileMappingA(
        fo->hFile, 0, PAGE_READWRITE, 0, 0, 0
    );
    if(!fo->hFmap){
        printf("Error: Could not create file mapping\n"); 
	return 0;
    }

    fo->map = MapViewOfFile(fo->hFmap,
        FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
    if(!fo->map){
        printf("Error: Could not map file\n");
	return 0;
    }
    return fo->map;
}

void unloadFileFromMemory(struct fileObject *fo)
{
    UnmapViewOfFile(fo->map);
    CloseHandle(fo->hFmap);
    CloseHandle(fo->hFile);
}




typedef int (__cdecl *PROC_CALLBACK)(void *, ...);

struct cmdline_proc{
    char name[MAX_NAME_LENGTH];
    char alias[MAX_ALIAS_LENGTH];
    PROC_CALLBACK callback;
    char numParams;
};

struct function {
    PROC_CALLBACK callback;
    char paramCount;
};

struct arg{
    PROC_CALLBACK callback;
    char paramCount;

    const char *long_name;
    unsigned int hash;
};

struct module{
    PROC_CALLBACK callback;
    char paramCount;

    struct arg *args[52];
    const char *name; 
    unsigned int hash;
};

#define NOT_FOUND -1

#define BASE_CONFIG_FILE ".\\config.json"

int call_callback(char * argv[], int argc, PROC_CALLBACK cb, void *pb)
{
int n = argc, rslt;
    while(n--){
        __asm mov eax, n  
        __asm mov ebx, 4
        __asm mul ebx
	__asm mov edx, argv
        __asm push [edx + eax]
    }
    rslt = cb(pb);
    n = argc;
    while(n--) __asm pop eax
    return rslt;
}

int find_longname(struct module *mod, const char *str)
{
int i;
struct arg *arg;
    for(i = 0, arg = mod->args[0]; i < 52; i++, arg = mod->args[i])
    {
        if(!arg) continue;
	if(!strncmp(str, arg->long_name, MAX_NAME_LENGTH))
            return i;
    }
    return NOT_FOUND;
}

int find_switch(struct module *mod, const char *str)
{
int i;
    i = MAP_ARG(*str);
    if(i < 52 && mod->args[i]) return i;
    return NOT_FOUND;
}

int find_submodule(const char *str, int modc, struct module mod[]) 
{
    for(int i = 1; i < modc; i++){
        if(!strncmp(str, mod[i].name, MAX_NAME_LENGTH)){
            return i;
        }
    }
    return 0;
}


int handle_commandline(int argc, char * argv[],
		int modc, struct module module[], void * pb)
{
int i, ii; 
unsigned int len1, len2;
int idx = 0;
struct arg * ptr;
struct function *func;
struct module * mod;
struct module * tmp;
char in_submodule = 0;
char *str;

    mod = module;
    i = 0;
    
START_LOOP:
    if(i >= argc) goto END_LOOP;

    str = argv[i];
    if(*str == '-'){
        if(str[1] == '-'){
            ii = find_longname(mod, str+2);	
	    if(ii == NOT_FOUND) {
                printf("Error: Invalid option\n");
                return 0;
            }
	    goto HANDLE_SWITCH;
        }
        else {
            ii = find_switch(mod, str+1);
            if(ii == NOT_FOUND) {
                printf("Error: Invalid option\n");
                return 0;	
            }
            func = &mod->args[ii];
            if(func->paramCount && str[2]){
                printf("Error: Invalid parameter to option\n"); 
                return 0;
	    }
	    goto HANDLE_SWITCH;
        }
    }

HANDLE_SUBMODULE:
    if(!in_submodule && 
    (ii = find_submodule(argv[i], modc, module)))
    {
        in_submodule = 1;
        mod = &module[ii];
	func = (struct function*)mod;
	if(func->paramCount > (argc - i)){
            printf("Error: not enough params\n"); 
            return 0;
	}
        goto RESTART_LOOP;
    }

HANDLE_MAINMODULE:
    func = (struct function*)mod;
    if(func->paramCount > (argc - i)){
        printf("Error: not enough params\n");
        return 0;
    }
    if(!call_callback(argv + i, func->paramCount, func->callback, pb)){
        printf("Error: incorrect positional parameter\n");
        return 0;
    }
    return 1;

HANDLE_SWITCH:
    ptr = mod->args[ii];
    func = (struct function*)ptr;
    if( func->paramCount > (argc - i - 1) ){
        printf("Error: not enough params\n");
        return 0;
    }
    call_callback(argv + i + 1, func->paramCount, func->callback, pb); 
    i += func->paramCount;

/*
HANDLE_CALLBACK:
    if(!call_callback(argv + i, func->paramCount, func->callback, pb)){
        printf("Error: callback failed\n");
       return 0;	
    }
    i += func->paramCount;
*/

RESTART_LOOP:
    i++;
    goto START_LOOP;
    
END_LOOP:
    return 0;
}
