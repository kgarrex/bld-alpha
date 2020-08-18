
#include <Windows.h>
#include <stdio.h>


#define MAX_FILENAME_LENGTH 256
#define MAX_DEFINE_NAME_LENGTH 32
#define MAX_DEFINE_VALUE_LENGTH 256


typedef struct {
	HANDLE hfile;
	char *filename;
} bld_config_t;


void bldconfig_new()
{}

HANDLE bldconfig_open_project_config(void *j)
{
	HANDLE hfile;

	/*
	hfile = CreateFile("config.ini", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(hfile == INVALID_HANDLE_VALUE) {
		printf("Error (%d): Could not open config\n", GetLastError());	
		return 0;
	}
	*/


	return hfile;
}

/*
void bldconfig_open_system_config()
{
}
*/


void bldconfig_read_srcfiles(void *obj, char *array[], int count)
{
	char *key = "source_files";

	struct bldconfig_object *cnf = obj;
	GetPrivateProfileStringA(key, 0, 0, 0, 0, cnf->filename);
}

void bldconfig_read_srcdirs(void *obj, char *array[], int count)
{
	char *key = "source_dirs";

	struct bldconfig_object *cnf = obj;
	GetPrivateProfileStringA(key, 0, 0, 0, 0, cnf->filename); 
}

void bldconfig_read_incdirs(void *obj, char *array[], int count)
{
	char *key = "include_dirs";

	struct bldconfig_object *cnf = obj;
	GetPrivateProfileStringA(key, 0, 0, 0, 0, cnf->filename);
}

void bldconfig_read_incfiles(void *obj, char *array[], int count)
{
	char *key = "include_files";

	struct bldconfig_object *cnf = obj;
	GetPrivateProfileStringA(key, 0, 0, 0, 0,cnf->filename);
}

void bldconfig_read_libdirs(void *obj, char *array[], int count)
{
	char *key = "lib_dirs";

	struct bldconfig_object *cnf = obj;
	GetPrivateProfileStringA(key, 0, 0, 0, 0, cnf->filename);
}

void bldconfig_read_libfiles(void *obj, char *array[], int count)
{
	char *key = "lib_files";

	struct bldconfig_object *cnf = obj;
	GetPrivateProfileStringA(key, 0, 0, 0, 0, cnf->filename);
}

void bldconfig_read_defstacksize(void *obj, int *size)
{
	char *key = "default_stack_size";

	struct bldconfig_object *cnf = obj;
	GetPrivateProfileStringA("linker", key, 0, 0, 0, cnf->filename);
}


void bldconfig_read_defines(void *obj, char *name[], int name_count, char *value[], int value_count)
{
	struct bldconfig_object *cnf = obj;
	GetPrivateProfileStringA(0, 0, 0, 0, 0, cnf->filename);
}


void bldconfig_close(HANDLE hfile)
{
	CloseHandle(hfile);
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	// Perform actions based on the reason for calling.
	switch( fdwReason )
	{
		case DLL_PROCESS_ATTACH:
		// Initialize once for each new process.
		// Return FALSE to fail DLL load.
			
			break;

		case DLL_THREAD_ATTACH:
		// Do thread-specific initialization.
			
			break;

		case DLL_THREAD_DETACH:
		// Do thread-specific cleanup.
			
			break;

		case DLL_PROCESS_DETACH:
		// Perform any necessary cleanup.
			
			break;
	}
	return TRUE;
}
