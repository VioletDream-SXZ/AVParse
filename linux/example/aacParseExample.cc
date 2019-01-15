#include "../algorithm/aacParse.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
  // init
  static const char* PATH = "D:\\Users\\Desktop\\test.aac";
  FILE* fp = fopen(PATH, "rb");
  uint8_t* buffer;
  size_t size;
  buffer = (uint8_t*)calloc(1024 * 1024 * 5, sizeof(uint8_t));
  
  // get file size
  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  
  // read file data
  int ret = fread(buffer, 1, size, fp);
  fclose(fp);
  
  // start
  parseAACData(buffer, (uint32_t)size);
}