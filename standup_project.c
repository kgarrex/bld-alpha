
PRIVATE uint32
standup_project(char *cur_dir, char *proj_name)
{
sbuf_t stage_buf;
if(!cur_dir || !proj_name) return 0;
stage_buf = new_sbuf(512, 0, cur_dir, "\\", proj_name, "\\.stage", 0);
CreateDirectoryA(stringof_sbuf(stage_buf), 0);
SetFileAttributes(stringof_sbuf(stage_buf), FILE_ATTRIBUTE_HIDDEN);
return 1;
}
