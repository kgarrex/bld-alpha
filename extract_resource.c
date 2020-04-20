typedef unsigned short uint16;

HANDLE extract_resource_to_file(
char* module_name,
uint16 resource_type,
uint16 resource_id,
uint16 lang_id,
uint16 is_temp_file,
char* file_name,
LPOVERLAPPED async_object)
/*
** Desc: Extracts a resource from a process and writes it to a file
** Return: Returns the handle to the file
** Params:
**    module_name    -
**    resource_type  -
**    resource_id    -
**    land_id        -
**    is_temp_file   -
**    file_name      -
**    async_object   -
** Notes:
** Revisions:
*/
{
    HANDLE file_handle;
    HRSRC resource_handle;
    HGLOBAL resource_data_handle;
    void* resource_data;
    uint32 resource_size,
           bytes_written,
           file_attributes = 0;
    SECURITY_ATTRIBUTES sec_attr = {0};

    sec_attr.nLength = sizeof(sec_attr);
    sec_attr.bInheritHandle = 1;

    resource_handle = FindResourceEx
    (
    GetModuleHandle(module_name),
    MAKEINTRESOURCE(resource_type),
    MAKEINTRESOURCE(resource_id),
    lang_id /* MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL) */
    );
    if(!resource_handle) {
        /* handle error */
        return 0;    
    }
    resource_data_handle = LoadResource
    (
    GetModuleHandle(module_name),
    resource_handle
    );
    if(!resource_data_handle) {
        /* handle error */
        return 0;    
    }
    resource_data = LockResource(resource_data_handle);
    if(!resource_data) {
        /* handle error */    
        return 0;
    }
    resource_size = SizeofResource
    (
    GetModuleHandle(module_name),
    resource_handle
    );
    if(!resource_size) {
        /* handle error */    
        return 0;
    }

/* set file attributes, create file and write data */
    if(is_temp_file)
    file_attributes |=
        FILE_ATTRIBUTE_TEMPORARY |
        FILE_FLAG_DELETE_ON_CLOSE;
    if(async_object)
        file_attributes |= FILE_FLAG_OVERLAPPED;
    if(!file_attributes) file_attributes = FILE_ATTRIBUTE_NORMAL;
    file_handle = CreateFile
    (
    file_name,
    GENERIC_READ | GENERIC_WRITE,
    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
    &sec_attr,
    CREATE_ALWAYS,
    file_attributes,
    0
    );
    if(file_handle == INVALID_HANDLE_VALUE) {
        /* handle error */
        return 0;    
    }
    if(!WriteFile
    (file_handle,
     resource_data,
     resource_size,
     &bytes_written,
     async_object))
    {
        /* handle error */
        return 0;        
    }
    return file_handle;
}
