
int __cdecl itoa_zt (int n, char* buf,
unsigned char bufsize, unsigned char padding,
unsigned char base)
{
//check that the base is valid
  char *ptr = buf,
       *ptr1 = buf,
       tmp_char,
       _buf[16];
  int nn, i = 0;

  if(!buf) return 0;
  if(base < 2 || base > 36) return 0;

  do {
    nn = n;
    n /= base;

    *ptr++ = "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210" //71 chars
             "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    [35 + (nn-n*base)];
    i++;
  } while(n);

  while(i < padding) {
    *ptr++ = '0';
    i++;
  }

  //Apply negative sign
  if(nn < 0) {
    *ptr++ = '-'; /* todo: check size left */
    i++;
  }
  *ptr-- = '\0';

  while(ptr1 < ptr) {
    tmp_char = *ptr;
    *ptr-- = *ptr1;
    *ptr1++ = tmp_char;
  }
  return i;
}

struct DirTemplate {
  //char *dirPath[];
  unsigned dirPathSize;
  //char jsonPath[];
  unsigned jsonPathSize;
};

struct DirStackObject
{
  int dirObjectIdx;
  short dirObjectCount;
  char * jsonPathEnd;
  char * jsonLeftBracket;
  char * dirPathEnd;
};

char keys[] = {
  "Name"
 ,"Hidden"
 ,"CopyFrom"
 ,"Files"
 ,"Dirs"
};

void createFile
(struct json_object * pobj, char jsonPath[],
 char *jsonRightBracket, char dirPath[], char **dirPathEnd)
{
  int result;
  char strBuf[128];
  int strBufSize = 128;
  long hidden;
  int length;
  DWORD attr = 0;
 
  memcpy(jsonRightBracket, ".Name", 5+1);
  result = json_query_get_string(pobj, jsonPath, strBuf, strBufSize, &length);
  if(result != JSON_SUCCESS) return;

  if(**dirPathEnd != '\\') {
    memcpy(*dirPathEnd, "\\", 2); 
  }
  memcpy(*dirPathEnd+1, strBuf, length+1); 
    
  memcpy(jsonRightBracket, ".Hidden", 7+1);
  result = json_query_get_boolean(pobj, jsonPath, &hidden);
  if(result == JSON_SUCCESS){
    if(hidden) attr |= FILE_ATTRIBUTE_HIDDEN; 
  }

  memcpy(jsonRightBracket, ".CopyFrom", 9+1);
  result = json_query_get_string(pobj, jsonPath, strBuf, strBufSize, 0);
  if(result != JSON_SUCCESS){
    if(!attr) attr = FILE_ATTRIBUTE_NORMAL;
    CreateFile(dirPath, GENERIC_READ|GENERIC_WRITE, 0, 0, CREATE_NEW, attr, 0);
  }
  else {
    CopyFile(strBuf, dirPath, 1); 
    if(attr) SetFileAttributes(dirPath, attr);
  }
}

void createDirectory
(struct json_object * pobj, char jsonPath[],
 char *jsonRightBracket, char dirPath[], char **dirPathEnd)
{
  int result;
  char strBuf[32];
  int strBufSize = 32;
  int length;

  memcpy(jsonRightBracket, ".Name", 5+1);
  result = json_query_get_string(pobj, jsonPath, strBuf, strBufSize, &length);
  if(result != JSON_SUCCESS) return;

  if(**dirPathEnd != '\\') {
    memcpy(*dirPathEnd, "\\", 2);
  }
  memcpy(*dirPathEnd+1, strBuf, length+1);
  *dirPathEnd += length+1;

  CreateDirectory(dirPath, 0);
}



int createDirectoryTemplate
(char *jsonRoot, char *dirRoot, struct DirStackObject dirStack[], int dirStackMax, struct json_object * pobj)
{
  short fileCount;
  char *indexPtr;
  int result;
  int i;
  unsigned len;
  char *jsonLeftBracket;
  char *jsonRightBracket;
  char idxBuf[4];
  char *dirPathEnd;


  char dirPath[256];
  char jsonPath[256];

  //index into the directory stack that is incremented and decremented
  //as directories moved in and out of. Used as a tether to the root directory
  struct DirStack * dirStackPtr = 0;
  int dirStackIdx = 0; 

  result = json_query_path_exists(pobj, jsonRoot);
  if(result != JSON_SUCCESS) return 0;

  len = strlen(dirRoot);
  memcpy(dirPath, dirRoot, len);
  dirPathEnd = &dirPath[len];

  //copy json string into jsonPath and cache the pointer to end
  len = strlen(jsonRoot);
  memcpy(jsonPath, jsonRoot, len);
  dirStack[dirStackIdx].jsonPathEnd = &jsonPath[len];

CREATE_FILES:
  dirStack[dirStackIdx].dirPathEnd = dirPathEnd;

  memcpy(dirStack[dirStackIdx].jsonPathEnd, ".Files", 6+1); //append null char
  jsonLeftBracket = &((dirStack[dirStackIdx].jsonPathEnd)[6]);
  result = json_query_get_array_count(pobj, jsonPath, &fileCount);
  if(result != JSON_SUCCESS){
    if(result != JSON_ERROR_KEY_UNDEFINED){
      printf("Error: Key 'Files' undefined\n");
      return 0;	    
    }
    fileCount = 0;
  }

  for(i = 0; i < fileCount; i++)
  {
    len = itoa_zt(i, idxBuf, 4, 0, 10);
    len = sprintf(jsonLeftBracket, "[%s]", idxBuf);
    result = json_query_path_exists(pobj, jsonPath);
    if(result == JSON_SUCCESS){
      jsonRightBracket = &jsonLeftBracket[len];

      //create file on disk
      createFile(pobj, jsonPath, jsonRightBracket,
      dirPath, &dirPathEnd);
    }
  }

  memcpy(dirStack[dirStackIdx].jsonPathEnd, ".Dirs", 5+1); //append null char 
  dirStack[dirStackIdx].jsonLeftBracket = &dirStack[dirStackIdx].jsonPathEnd[5];
  result = json_query_get_array_count(pobj, jsonPath, &dirStack[dirStackIdx].dirObjectCount);
  if(result != JSON_SUCCESS){
    if(result != JSON_ERROR_KEY_UNDEFINED){
      printf("Error: Key 'Dirs' undefined\n");
      return 0;	    
    }
    dirStack[dirStackIdx].dirObjectIdx++;
  }

CREATE_DIRECTORY:

  while(dirStack[dirStackIdx].dirObjectIdx < dirStack[dirStackIdx].dirObjectCount)
  {
    len = itoa_zt(dirStack[dirStackIdx].dirObjectIdx, idxBuf, 4, 0, 10);
    len = sprintf(dirStack[dirStackIdx].jsonLeftBracket, "[%s]", idxBuf);

    jsonRightBracket = &dirStack[dirStackIdx].jsonLeftBracket[len];

    //create directory on disk
    createDirectory(pobj, jsonPath, jsonRightBracket, dirPath, &dirPathEnd);

    //error: exceeds dir stack upper bound
    if(++dirStackIdx == dirStackMax) return 0;

    //init the new directory on the stack
    dirStack[dirStackIdx].dirObjectIdx = 0;
    dirStack[dirStackIdx].jsonPathEnd = jsonRightBracket;

    //create files first since they don't require recursion
    goto CREATE_FILES;
  }

ROLLBACK_DIRSTACK:
  if(dirStackIdx){
    dirStackIdx--;
    dirPathEnd = dirStack[dirStackIdx].dirPathEnd;
    dirStack[dirStackIdx].dirObjectIdx++;
    goto CREATE_DIRECTORY;
  }
   
EXIT_LOOP:
  return 1;
}
