void __cdecl Windows_CreateDirectory(const char * path)
{
    CreateDirectory(path, 0);
}

void __cdecl Windows_CreateFile(const char *path, int flags)
{
    
}

enum FileFlags {
  FILE_FLAG_HIDDEN
 ,FILE_FLAG_READONLY
};

typedef struct _OperatingSystem {
    void (__cdecl *CreateDirectory)(const char *path);
    void (__cdecl *CreateFile)(const char *path, int flags);
} OperatingSystem;
