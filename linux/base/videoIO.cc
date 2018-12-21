#include "videoIO.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Free(x) \
    if(x != NULL) \
      free(x);    \
    x = NULL      

int YUV420ToWidth_Height(const char* rawFile, const char* saveFile, int width, int height, int save_width, int save_height, int frameLen)
{
  int static_frame_num = 0;
  int file_size = 0;
  
  FILE* rawFp = fopen(rawFile, "rb");
  if(rawFp == NULL)
  {
    printf("[ERROR]open rawfile failed!\n");
    return -1;
  }
  
  FILE* saveFp = fopen(saveFile, "wb");
  if(saveFp == NULL)
  {
    printf("[ERROR]open saveFile failed.\n");
    fclose(rawFp);
    return -1;
  }
  
  const int SIZE_Y = width * height, SIZE_U = width * height / 4;
  char *buff_y = (char*)malloc((SIZE_Y + 1) * sizeof(char));
  char *buff_u = (char*)malloc((SIZE_U + 1) * sizeof(char));
  char *buff_v = (char*)malloc((SIZE_U + 1) * sizeof(char));
  char *saveBuff_y = (char*)malloc((save_width * save_height + 1) * sizeof(char));
  char *saveBuff_u = (char*)malloc((save_width * save_height / 4 + 1) * sizeof(char));
  char *saveBuff_v = (char*)malloc((save_width * save_height / 4 + 1) * sizeof(char));
  
  for(int i=0; i<frameLen; i++)
  {
    int readLen, writeLen;
    readLen =  fread(buff_y, 1, SIZE_Y, rawFp);
    readLen += fread(buff_u, 1, SIZE_U, rawFp);
    readLen += fread(buff_v, 1, SIZE_U, rawFp);
    
    if(readLen <= 0)
    {
      printf("[ERROR] no many frame!\n");
      goto FAILED;
    }
    
    for(int i=0; i<save_height; ++i)
    {
      for(int j=0; j<save_width; ++j)
        saveBuff_y[i*save_width+j] = buff_y[i*width+j];
      
      if((i&1) == 0)
      {
        for(int j=0; j<save_width/2; ++j)
        {
          saveBuff_u[i*save_width/4+j] = buff_u[i*width/4+j];
          saveBuff_v[i*save_width/4+j] = buff_v[i*width/4+j];
        }
      }
    }
    
    writeLen =  fwrite(saveBuff_y, 1, save_width*save_height, saveFp);
    writeLen += fwrite(saveBuff_u, 1, save_width*save_height/4, saveFp);
    writeLen +=fwrite(saveBuff_v, 1, save_width*save_height/4, saveFp);
    
    file_size += writeLen;
    static_frame_num ++;
  }
  
  printf("[INFO] The change is end.The new file size is %d!The number of frame is %d!\n", file_size, static_frame_num);
  
  Free(buff_y);
  Free(buff_u);
  Free(buff_v);
  Free(saveBuff_y);
  Free(saveBuff_u);
  Free(saveBuff_v);
  fclose(rawFp);
  fclose(saveFp);
  return 1;
  
FAILED:
  Free(buff_y);
  Free(buff_u);
  Free(buff_v);
  Free(saveBuff_y);
  Free(saveBuff_u);
  Free(saveBuff_v);
  fclose(rawFp);
  fclose(saveFp);
  return -1;
}