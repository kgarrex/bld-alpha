
#include "ICompiler.h"

int Compiler_SetOutputName(void **obj, const char * str, unsigned len)
{
ICOMPILER * cl;
    cl = *obj;
    return cl->SetOutputName(obj, str, len);
}

int Compiler_AddIncludeDir(void **obj, const char * str, unsigned len)
{
ICOMPILER * cl;
    cl = *obj;
    return cl->AddIncludeDir(obj, str, len);
}

int Compiler_AddIncludeFile(void **obj, const char * str, unsigned len)
{
ICOMPILER * cl;
    cl = *obj;
    return cl->AddIncludeFile(obj, str, len);
}

int Compiler_AddSourceDir(void **obj, const char * str, unsigned len)
{
ICOMPILER * cl;
    cl = *obj;
    return cl->AddSourceDir(obj, str, len);
}

int Compiler_AddSourceFile(void **obj, const char * str, unsigned len)
{
ICOMPILER * cl;
    cl = *obj;
    return cl->AddSourceFile(obj, str, len);
}

int Compiler_DefineMacro(void **obj, const char * name, unsigned name_len,
	const char * text, unsigned text_len)
{
ICOMPILER * cl;
    cl = *obj;
    return cl->DefineMacro(obj, name, name_len, text, text_len);
}

int Compiler_SetDefaultCall(void **obj, unsigned call_type)
{
ICOMPILER * cl;
    cl = *obj;
    return cl->SetDefaultCall(obj, call_type);
}

int Compiler_SetStructBoundary(void **obj, unsigned boundary)
{
ICOMPILER * cl;
    cl = *obj;
    return cl->SetStructBoundary(obj, boundary);
}

int Compiler_SetWorkingDirectory(void **obj, const char *dir, unsigned len)
{
ICOMPILER * cl;
    cl = *obj;
    return cl->SetWorkingDirectory(obj, dir, len);
}

int Compiler_Exec(void **obj)
{
ICOMPILER * cl;
    cl = *obj;
    return cl->Exec(obj);
}
