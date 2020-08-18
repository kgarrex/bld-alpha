
#include <windows.h>
#include <stdio.h>

int create_dll_template(const char *filename)
{
	HANDLE hfile;
	DWORD numbytes;

	const char *string =
	"#include <windows.h>\r\n"
	"\r\n"
	"\r\n"
	"BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)\r\n"
	"{\r\n"
	"\t// Perform actions based on the reason for calling.\r\n"
	"\tswitch( fdwReason )\r\n"
	"\t{\r\n"
	"\t\tcase DLL_PROCESS_ATTACH:\r\n"
	"\t\t// Initialize once for each new process.\r\n"
	"\t\t// Return FALSE to fail DLL load.\r\n"
	"\t\t\t\r\n"
	"\t\t\tbreak;\r\n"
	"\r\n"
	"\t\tcase DLL_THREAD_ATTACH:\r\n"
	"\t\t// Do thread-specific initialization.\r\n"
	"\t\t\t\r\n"
	"\t\t\tbreak;\r\n"
	"\r\n"
	"\t\tcase DLL_THREAD_DETACH:\r\n"
	"\t\t// Do thread-specific cleanup.\r\n"
	"\t\t\t\r\n"
	"\t\t\tbreak;\r\n"
	"\r\n"
	"\t\tcase DLL_PROCESS_DETACH:\r\n"
	"\t\t// Perform any necessary cleanup.\r\n"
	"\t\t\t\r\n"
	"\t\t\tbreak;\r\n"
	"\t}\r\n"
	"\treturn TRUE;\r\n"
	"}\r\n";

	hfile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE,
			0, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
	if(hfile == INVALID_HANDLE_VALUE){
		printf("Error: Could not create dll file\n");	
		return 0;
	}

	WriteFile(hfile, string, strlen(string), &numbytes, 0);
	return 1;
}




int main(int argc, char *argv[])
{
	
	char *filename;

	if(argc > 1) filename = argv[1];
	else filename = "dll-template.c";

	create_dll_template(filename);
	return 1;
}
