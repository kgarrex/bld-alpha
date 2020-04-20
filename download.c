#include <windows.h>
#include <winhttp.h>

#include <stdio.h>

#pragma comment(lib, "winhttp.lib")


#define MEGABYTE 1048576

wchar_t *user_agent = L"MyDownloader";

wchar_t *host_name = L"dl.google.com";
wchar_t *android_sdk_path   = L"/android/repository/build-tools_r30-rc1-windows.zip";
wchar_t *android_ndk_path   = L"/android/repository/android-ndk-r21-windows-x86_64.zip";
wchar_t *android_tools_path = L"/android/repository/platform-tools-latest-windows.zip";

wchar_t *mime_types = L"application/zip";

//https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=BuildTools&rel=16
//vs_buildtools__66140540.1585965493.exe

#define BUFFER_SIZE 8192

int download_zip_file(HINTERNET hconnect, const wchar_t *filename)
{

  HINTERNET hrequest;
  HANDLE hfile;
  LARGE_INTEGER distance_to_move = {0};

  DWORD bytes_available;
  DWORD bytes_read;
  DWORD bytes_written;
  DWORD total_bytes_written = 0;
  char percentage = 0;
  char last_percentage = 0;
  char buf[BUFFER_SIZE];

  wchar_t network_path[256];
  wchar_t file_path[256];

  wchar_t content_length_buffer[11];
  DWORD bufsize = sizeof(content_length_buffer);
  int content_length;


  //wprintf(L"filename: %lls\n", filename);

  swprintf(network_path, 256, L"/android/repository/%lls", filename);
  //wprintf(L"network_path: %lls\n", network_path);

  hrequest = WinHttpOpenRequest(hconnect, L"GET", network_path, 0, WINHTTP_NO_REFERER, 0, WINHTTP_FLAG_SECURE);
  if(!hrequest){
    printf("Error: WinHttpOpenRequest (%d)\n", GetLastError());
    return 0;
  }

  if(!WinHttpSendRequest(hrequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0))
  {
    printf("Error: WinHttpSendRequest (%d)\n", GetLastError()); 
    return 0;
  }

  if(!WinHttpReceiveResponse(hrequest, 0))
  {
    printf("Response not received\n"); 
  }

  if(!WinHttpQueryHeaders(hrequest, WINHTTP_QUERY_CONTENT_LENGTH,
    WINHTTP_HEADER_NAME_BY_INDEX, content_length_buffer, &bufsize, WINHTTP_NO_HEADER_INDEX))
  {
    printf("Error: WinHttpQueryHeaders (%d)\n", GetLastError());
    return 0;
  }
  content_length = _wtoi(content_length_buffer);


  swprintf(file_path, 256, L".\\%lls", filename);
  //wprintf(L"file_path: %lls\n", file_path);

  hfile = CreateFileW(file_path, GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
  if(hfile == INVALID_HANDLE_VALUE){
    printf("Error: CreateFile (%d)\n", GetLastError()); 
    return 0;
  }


  do {

    if(!WinHttpQueryDataAvailable(hrequest, &bytes_available))
    {
      printf("Error: WinHttpQueryDataAvailable (%d)\n", GetLastError()); 
      return 0;
    }
    if(!bytes_available) break;

    //printf("Bytes Available: %u\n", bytes_available);

    if(!WinHttpReadData(hrequest, buf, bytes_available, &bytes_read))
    {
      printf("Error: WinHttpReadData (%d)\n", GetLastError());
      return 0;
    }

    //printf("bytes_available: %u | bytes_read: %u\n", bytes_available, bytes_read);
    //printf("Data: %.*s\n", bytes_available, buf);

    WriteFile(hfile, buf, bytes_read, &bytes_written, 0); 

    total_bytes_written += bytes_written;

    percentage = (total_bytes_written * 100) / content_length;
    if(last_percentage < percentage){
      last_percentage = percentage;
      if(percentage < 10)
        printf("%u%%\b\b", percentage);
      else if(percentage < 100)
        printf("%u%%\b\b\b", percentage);
      else
        printf("%u%%\b\b\b\b", percentage);
    }
    
    if(!SetFilePointerEx(hfile, distance_to_move, 0, FILE_END)){
      printf("Error: SetFilePointerEx (%d)\n", GetLastError());    
      return 0;
    }

  } while(1);


  WinHttpCloseHandle(hrequest);
  return 1;
}

int main(int argc, char *argv[])
{
  HINTERNET hsession;
  HINTERNET hconnect;


  hsession = WinHttpOpen(user_agent, WINHTTP_ACCESS_TYPE_NO_PROXY, 0, 0, 0);// WINHTTP_FLAG_ASYNC);
  if(!hsession){
    printf("Error: WinHttpOpen (%d)\n", GetLastError());
    return 0;
  }

  hconnect = WinHttpConnect(hsession, host_name, INTERNET_DEFAULT_HTTPS_PORT, 0);
  if(!hconnect){
    printf("Error: WinHttpConnect (%d)\n", GetLastError()); 
    return 0;
  }

  printf("Downloading android platform tools...\n");
  download_zip_file(hconnect, L"platform-tools-latest-windows.zip");
  printf("Downloading android build tools...\n");
  download_zip_file(hconnect, L"build-tools_r30-rc1-windows.zip");
  printf("Downloading android ndk...\n");
  download_zip_file(hconnect, L"android-ndk-r21-windows-x86_64.zip");

  
CLEANUP:
  WinHttpCloseHandle(hconnect);
  WinHttpCloseHandle(hsession); 

  return 1;
}
