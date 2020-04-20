PRIVATE uint32
teardown_project(char *cur_dir, char *proj_name)
{
sbuf_t stage_buf;
uint32 length,
	   success;
WIN32_FIND_DATA find_data = {0};
HANDLE find_handle;

if(!cur_dir || !proj_name) return 0;

stage_buf = new_sbuf(512, 0, 0);
length = append_sbuf(stage_buf, cur_dir, "\\", proj_name, "\\.stage", END_ARGS);

length = append_sbuf(stage_buf, "\\*", END_ARGS);

find_handle = FindFirstFile(stringof_sbuf(stage_buf), &find_data);
assert(find_handle != INVALID_HANDLE_VALUE);
erase_sbuf(stage_buf, length);

	do {
		if(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		continue;
	length = append_sbuf(stage_buf, "\\", find_data.cFileName, END_ARGS);
	DeleteFile(stringof_sbuf(stage_buf));
	erase_sbuf(stage_buf, length);
	} while(FindNextFile(find_handle, &find_data));
FindClose(find_handle);

success = RemoveDirectory(stringof_sbuf(stage_buf));
	if(!success) {
	printf("Error %d: Could not remove '.stage' dir\n", GetLastError());
	return 0;	
	}
return 1;
}
