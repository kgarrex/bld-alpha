PRIVATE uint32
standup_build(char *cur_dir)
{
char   *path;
sbuf_t path_buf;
uint32 length;
HANDLE execs[5];

path_buf = new_sbuf(512, 0, cur_dir, "\\bin", 0);
path = stringof_sbuf(path_buf);

CreateDirectory(path, 0);

length = append_sbuf(path_buf, "\\cl.exe", 0);
execs[0] = extract_resource_to_file
(0, (uint16)RT_RCDATA, RESID_WINCL,
0, 0, path, 0);
erase_sbuf(path_buf, length);

length = append_sbuf(path_buf, "\\link.exe", 0);
execs[1] = extract_resource_to_file
(0, (uint16)RT_RCDATA, RESID_WINLINK,
0, 0, path, 0);
erase_sbuf(path_buf, length);

length = append_sbuf(path_buf, "\\rc.exe", 0);
execs[2] = extract_resource_to_file
(0, (uint16)RT_RCDATA, RESID_WINRC,
 0 , 0, path, 0);
erase_sbuf(path_buf, length);

length = append_sbuf(path_buf, "\\cvtres.exe", 0);
execs[3] = extract_resource_to_file
(0, (uint16)RT_RCDATA, RESID_WINCVTRES,
 0, 0, path, 0);
erase_sbuf(path_buf, length);

length = append_sbuf(path_buf, "\\mc.exe", 0);
execs[4] = extract_resource_to_file
(0, (uint16)RT_RCDATA, RESID_MC,
 0, 0, path, 0);
erase_sbuf(path_buf, length);

for(int i = 4; CloseHandle(execs[i--]); );

return 1;
}
