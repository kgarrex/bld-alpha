#define CREATE_FILE(rb, rel_path)\
rb = sbuf_append(dir_buf, rel_path, 0); \
CreateDirectory(dirpath, 0); \
sbuf_erase(dir_buf, (rb))

#define EXTRACT_FILE(rb, res_id)\
extract_resource_to_file(0,\
(uint16)RT_RCDATA, \
res_id, \
0, 0, sbuf_head(dir_buf),\
0); sbuf_erase(dir_buf, (rb))

//read the bld ini file and get the projects directory
//if its not set, use the current directory


PUBLIC uint32
create_project(char *cur_dir, char *proj_name)
{
sbuf_t dir_buf;
char   *dirpath;
uint32 rb1, rb2size;

char path[256], *endpath;

dir_buf = sbuf_new(512, 0, END_ARGS);
dirpath = sbuf_head(dir_buf);

/* create project folder */
if(!cur_dir || !proj_name) return 0;
sbuf_append(dir_buf, cur_dir, "\\", proj_name, END_ARGS);
CreateDirectory(dirpath, 0);

/*
lstrcatA(endpath, "\\obj");
CreateDirectoryExA(0, path, 0);
*/

/* create obj folder */
CREATE_FILE(rb1, "\\obj");

/* create resource folder */
rb1 = sbuf_append(dir_buf, "\\res", END_ARGS); 
printf("%s\n", dirpath);
CreateDirectory(dirpath, 0);

CREATE_FILE(rb2, "\\raw");
CREATE_FILE(rb2, "\\bmp");
CREATE_FILE(rb2, "\\cur");
CREATE_FILE(rb2, "\\dlg");
CREATE_FILE(rb2, "\\ico");
CREATE_FILE(rb2, "\\mc");

rb2 = sbuf_append(dir_buf, "\\res.ini", END_ARGS);
EXTRACT_FILE(rb2 + rb1, RESID_RESINI);

/* create inc folder */
CREATE_FILE(rb1, "\\inc");

/* create code folder */
CREATE_FILE(rb1, "\\code");

/* create ui folder */
CREATE_FILE(rb1, "\\ui");

/* create bin folder */
CREATE_FILE(rb1, "\\bin");

/* create references folder */
CREATE_FILE(rb1, "\\ref");

/* create project config file */
rb1 = sbuf_append(dir_buf, "\\proj.ini", END_ARGS);
EXTRACT_FILE(rb1, RESID_PROJINI);

/* create project main.c file */
rb1 = sbuf_append(dir_buf, "\\code\\main.c", END_ARGS);
EXTRACT_FILE(rb1, RESID_MAINC);

/* create project main.h file */
rb1 = sbuf_append(dir_buf, "\\inc\\main.h", END_ARGS);
EXTRACT_FILE(rb1, RESID_MAINH);

return 1;
}
