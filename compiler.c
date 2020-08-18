
void *vccompiler_new()
{
	
}


int vccompiler_add_include_dir(void *vcc, const char *dir)
{
	char buf[256];

	if(!layerOS_fileExists(dir)) {
		printf("Error: dir '%s' does not exists\n");
		return 0;
	}
	snprintf(buf, 256, "/I\"%s\"", dir);
	layerOS_processAddArg(vc->pproc, buf, 0);

	return 1;
}

void vccompiler_define_macro(void *vcc, const char *name, const char *value)
{
	char buf[256];
	snprintf(buf, 256, "/D%s=%s", name, value);
	layerOS_processAddArg(vcc->pproc, buf, 0);
	return 1;
}

void vccompiler_set_obj_name(void *obj, const char *name)
{
	char buf[256];
	snprintf(buf, 256, "/Fo\"%s\"", name);
	layerOS_processAddArg(vcc->pproc, buf, 0);
	return 1;
}

void vccompiler_add_include_file(void *vcc, const char *file)
{
	char buf[256];
	if(!layerOS_fileExists(file)){
		printf("Error: file '%s' does not exists\n");	
		return 0;
	}
	snprintf(buf, 256, "/FI\"%s\"", file);
	layerOS_processAddArg(vcc->pproc, buf, 0);
	return 1;
}

void vccompiler_add_source_file(void *vcc, const char *file)
{
	//TODO should set a flag when a source file has been added
	if(!layerOS_fileExists(file)){
		printf("Error: file '%s' does not exists\n");	
		return 0;
	}
	layerOS_processAddArg(vcc->pproc, file, 0);
	return 1;
}


void vccompiler_add_source_dir(void *vcc, const char *dir)
{
	
}

void vccompiler_set_default_call_conv(void *obj, int type)
{}


void vccompiler_set_struct_bound(void *obj, int)
{}

void vccompiler_exec(void *obj)
{}
