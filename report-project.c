
void report_project(const char *name)
{
char buf[128], *tail;
int len;
HANDLE find_handle;
WIN32_FIND_DATA find_data = {0};

    len = strlen(proj_dir);
    memcpy(buf, proj_dir, len);
    tail = buf+len;
    memcpy(tail, "*", 2);

    find_handle = FindFirstFileA(buf, &find_data);
    for(int i = 2; i--; FindNextFileA(find_handle, &find_data));
    do{
        memcpy(tail, find_data.cFileName, strlen(find_data.cFileName)+1);
	SetCurrentDirectoryA(buf);
	git_status();
        printf("%s\n", buf);
    } while(FindNextFileA(find_handle, &find_data));
}
