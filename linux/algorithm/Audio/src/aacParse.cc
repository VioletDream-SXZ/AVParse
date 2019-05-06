#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "aacParse.h"
#include "audioTool.h"

static const char ADTSHEADER[] = "ADTS_HEADER:";
static const char SYNCWORD[]   = "syncword                :";
static const char ID[]         = "ID                      :";
static const char LAYER[]      = "layer                   :";
static const char ABSENT[]     = "protection_absent       :";
static const char PROFILE[]    = "profile                 :";
static const char FREQUENCY[]  = "sampling_frequency      :";
static const char PRIVATE_BIT[]= "private_bit             :";
static const char CONFIGURE[]  = "channel_configuration   :";
static const char ORIGINAL[]   = "original_copy           :";
static const char HOME[]       = "home                    :";
static const char STREAM[]     = "copyrighted_stream      :";
static const char START[]      = "copyrighted_start       :";
static const char LENGTH[]     = "frame_length            :";
static const char FULLNESS[]   = "buffer_fullness         :";
static const char NUMBERS[]    = "numbers                 :";

void parseAACData(UINT8* RawData, UINT32 dataLen)
{
  ADTS_HEADER _header;
  AACData     _data;
  
  UINT32      cur_index = 0;
  while(cur_index < dataLen)
  {
    if(cur_index + 3 >= dataLen || (RawData[cur_index] != 0xFF && (RawData[cur_index + 1] & 0xF0) != 0xF0))
    {
      printf("The aac data start error.\n");
      return;
    }
    
    _header.syncword = 0x0FFF;
    cur_index += 1;
    _header.ID       = (UINT8)((RawData[cur_index] >> 3) & 0x01);
    _header.layer    = (UINT8)((RawData[cur_index] >> 1) & 0x03);
    _header.protection_absent = (UINT8)(RawData[cur_index] & 0x01);
    cur_index += 1;
    
    _header.profile  = (UINT8)((RawData[cur_index] & 0xc0) >> 6);
    _header.sampling_frequency_index = (UINT8)((RawData[cur_index] & 0x3c) >> 2);
    _header.private_bit = (UINT8)((RawData[cur_index] & 0x02) >> 1);
    
    _header.channel_configuration = (UINT8)((RawData[cur_index] & 0x01) << 2);
    _header.channel_configuration += (UINT8)((RawData[cur_index + 1] & 0xc0) >> 6);
    _header.original_copy = (UINT8)((RawData[cur_index + 1] & 0x20) >> 5);
    _header.home          = (UINT8)((RawData[cur_index + 1] & 0x10) >> 4);
    cur_index += 1;
    
    _header.copyrighted_stream = ((RawData[cur_index] & 0x08) >> 3);
    _header.copyrighted_start  = ((RawData[cur_index] & 0x04) >> 2);
    _header.frame_length       = ((RawData[cur_index] & 0x03) << 11);
    cur_index += 1;
    
    _header.frame_length       += ((RawData[cur_index]) << 3);
    cur_index += 1;
    
    _header.frame_length       += ((RawData[cur_index] & 0xe0) >> 5);
    _header.buffer_fullness    =  ((RawData[cur_index] & 0x1f) << 6);
    cur_index += 1;
    
    _header.buffer_fullness    += ((RawData[cur_index] & 0xfc) >> 2);
    _header.numbers            =  ((RawData[cur_index] & 0x03));
    cur_index += 1;
    
    printf("ADTS_HEADER:\n");
    printf("\tsyncword                : %u\n", _header.syncword);
    printf("\tID                      : %u\n", _header.ID);
    printf("\tlayer                   : %u\n", _header.layer);
    printf("\tprotection_absent       : %u\n", _header.protection_absent);
    printf("\tprofile                 : %s\n", getProfile(_header.profile));
    printf("\tsampling_frequency      : %s\n", getSamplingFrequency(_header.sampling_frequency_index));
    printf("\tprivate_bit             : %u\n", _header.private_bit);
    printf("\tchannel_configuration   : %u\n", _header.channel_configuration);
    printf("\toriginal_copy           : %u\n", _header.original_copy);
    printf("\thome                    : %u\n", _header.home);
    printf("\tcopyrighted_stream      : %u\n", _header.copyrighted_stream);
    printf("\tcopyrighted_start       : %u\n", _header.copyrighted_start);
    printf("\tframe_length            : %u\n", _header.frame_length);
    printf("\tbuffer_fullness         : %u\n", _header.buffer_fullness);
    printf("\tnumbers                 : %u\n", _header.numbers);
    
    cur_index += (_header.frame_length - 7);
    printf("\n");
  }
}

void aacParseExample(const char* FILE_PATH_NAME)
{
  FILE* fp = fopen(FILE_PATH_NAME, "rb");
  
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