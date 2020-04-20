
PUBLIC uint32
delete_all_files(sbuf_t dir)
/* recursively deletes all files and subdirectories in a directory */
{
register uint32 i,
         success,
         error;
char *dir_str;
HANDLE find_handle;
WIN32_FIND_DATA find_data = {0};

i = append_sbuf(dir, "\\*", END_ARGS);
find_handle = FindFirstFileA(stringof_sbuf(dir), &find_data);
assert(find_handle != INVALID_HANDLE_VALUE);
erase_sbuf(dir, i);
for(i = 2;i--; FindNextFileA(find_handle, &find_data));
do  {
i = append_sbuf(dir, "\\", find_data.cFileName, END_ARGS);
    if(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        if(!RemoveDirectoryA(stringof_sbuf(dir))) {
        error = GetLastError();
            if(error == ERROR_DIR_NOT_EMPTY) {
                if(!delete_all_files(dir)) {
                printf("Error %d: Could not delete %s\n",
                GetLastError(), find_data.cFileName);
                return 0;
                }
            }
        }
    RemoveDirectoryA(stringof_sbuf(dir));
    }
    else{
    success = DeleteFile(stringof_sbuf(dir));
    }
erase_sbuf(dir, i);
} while(FindNextFile(find_handle, &find_data));

FindClose(find_handle);
return 1;
}

