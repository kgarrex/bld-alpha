PRIVATE uint32
teardown_build(char* cur_dir)
{
uint32 success,
       length;
sbuf_t path;
WIN32_FIND_DATA find_data = {0};
HANDLE find_handle;

path = new_sbuf(256, 0, cur_dir, "\\bin", 0);
length = append_sbuf(path, "\\*", 0);

find_handle = FindFirstFile(stringof_sbuf(path), &find_data);
if(find_handle == INVALID_HANDLE_VALUE)
    {
    printf("Error %d: Could not find first file\n", GetLastError());
    assert(0);
    }
erase_sbuf(path, length);
do
    {
    if(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        continue;
    length = append_sbuf(path, "\\", find_data.cFileName, END_ARGS);
    DeleteFile(stringof_sbuf(path));
    erase_sbuf(path, length);
    } while(FindNextFile(find_handle, &find_data));
FindClose(find_handle);

success = RemoveDirectory(stringof_sbuf(path));
if(!success) {
    printf("Error %d: Could not remove directory\n", GetLastError());
    assert(0);
    }
return 1;
}
