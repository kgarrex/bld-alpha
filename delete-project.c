
unsigned deleteAllFiles(char *path)
{
WIN32_FIND_DATAA findData = {0};
unsigned length;
char pathBuffer[260];

struct _dir{
HANDLE hFind;
char *endOfPath;
} dir[8];

unsigned idx = 0;
struct _dir *dirPath;

    DWORD attr = GetFileAttributes(path);
    if(attr == INVALID_FILE_ATTRIBUTES)
    {
        printf("Invalid path\n"); 
	return 0;
    }

    length = strlen(path);
    memcpy(pathBuffer, path, length);

ENTER_DIRECTORY:

    dirPath = &dir[idx];
    if(!idx)
        dirPath->endOfPath = pathBuffer + length;
    else
        dirPath->endOfPath = dir[idx-1].endOfPath + length;

    if(*(dirPath->endOfPath-1) != '\\')
    {
        *dirPath->endOfPath++ = '\\'; 
    }
    *dirPath->endOfPath = '*'; dirPath->endOfPath[1] = '\0';
    dirPath->hFind = FindFirstFileA(pathBuffer, &findData);
    if(dirPath->hFind == INVALID_HANDLE_VALUE){
        printf("Could not find file %s\n", pathBuffer); 
        return 0;
    }
    for(int i = 1; i--; FindNextFileA(dirPath->hFind, &findData) );

NEXT_FILE:
    if(!FindNextFile(dirPath->hFind, &findData)){
        goto LEAVE_DIRECTORY;
    }

    length = strlen(findData.cFileName);
    memcpy(dirPath->endOfPath, findData.cFileName, length+1);

    if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        if(!RemoveDirectoryA(pathBuffer))
	{
            if(GetLastError() != ERROR_DIR_NOT_EMPTY){
                printf("Error: Could not delete dir\n");
                return 0;	
	    }
            idx++;
            goto ENTER_DIRECTORY;
	}
    }
    else
    {
        DeleteFile(pathBuffer);
    }
    goto NEXT_FILE;

LEAVE_DIRECTORY:
    *(dirPath->endOfPath-1) = '\0';
    RemoveDirectory(pathBuffer);
    FindClose(dirPath->hFind);
    if(idx--){
        dirPath = &dir[idx]; //reset dirPath upon exiting directory
        goto NEXT_FILE;
    }
    return 1;
}



void print_delete_usage()
{
    printf("usage bld delete [<options>] <projname>\n");
}

int delete_callback(void *NOT_USED)
{
    printf("Delete\n");
    return 1;
}

void delete_project(const char *name)
{
int len;
char buf[256], *pdir = buf;

    printf("deleting a project\n");
    len = strlen(proj_dir);
    memcpy(pdir, proj_dir, len);
    pdir += len;

    len = strlen(name);
    memcpy(pdir, name, len);
    pdir += len;
    memcpy(pdir, "\\", 2);

    if(!RemoveDirectoryA(buf)) deleteAllFiles(buf);
    RemoveDirectoryA(buf);
}




