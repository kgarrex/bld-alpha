PUBLIC uint32
migrate_to_staging(char *cur_dir, char *proj_name)
/* copy .c and .h files to the staging area and compile them 
 * once the files have been compiled, delete source code */
{
HANDLE find_handle;
sbuf_t newfile_buf,
       file_buf; 
char   *newfile,
       *file;
uint32 len1,
       len2,
       len3;
WIN32_FIND_DATA find_data;

newfile_buf = new_sbuf(512, 0, cur_dir, "\\", proj_name,"\\.stage", END_ARGS); 
file_buf = new_sbuf(512, 0, cur_dir, "\\", proj_name, END_ARGS); 

newfile = stringof_sbuf(newfile_buf);
file = stringof_sbuf(file_buf);

len1 = append_sbuf(file_buf, "\\code", END_ARGS);
len2 = append_sbuf(file_buf, "\\*", END_ARGS);
find_handle = FindFirstFileA(file, &find_data);
assert(find_handle != INVALID_HANDLE_VALUE);
erase_sbuf(file_buf, len2);
do {
    if(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    continue;	
len2 = append_sbuf(file_buf, "\\", find_data.cFileName, END_ARGS);
len3 = append_sbuf(newfile_buf, "\\", find_data.cFileName, END_ARGS);
CopyFile(file, newfile, 0);
erase_sbuf(file_buf, len2);
erase_sbuf(newfile_buf, len3);
} while(FindNextFile(find_handle, &find_data));

erase_sbuf(file_buf, len1);
len1 = append_sbuf(file_buf, "\\inc", END_ARGS);
len2 = append_sbuf(file_buf, "\\*", END_ARGS);
find_handle = FindFirstFileA(file, &find_data);
assert(find_handle != INVALID_HANDLE_VALUE);
erase_sbuf(file_buf, len2);
do {
    if(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    continue;	
len2 = append_sbuf(file_buf, "\\", find_data.cFileName, END_ARGS);
len3 = append_sbuf(newfile_buf, "\\", find_data.cFileName, END_ARGS);
CopyFile(file, newfile, 0);
erase_sbuf(file_buf, len2);
erase_sbuf(newfile_buf, len3);
} while(FindNextFile(find_handle, &find_data));

/* resources */
create_resources(cur_dir, proj_name, 0);
compile_resources(cur_dir, proj_name);

return 1;
}
