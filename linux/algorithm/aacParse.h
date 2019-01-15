#ifndef GY_AAC_PARSE_H
#define GY_AAC_PARSE_H

#include <stdint.h>

#define UINT8  uint8_t
#define UINT16 uint16_t
#define UINT24 UINT8[3]
#define UINT32 uint32_t
#define UINT64 uint64_t

const char MEPG4[] = "MEPG-4";
const char MEPG2[] = "MEPG-2";

#define MAIN_PROFILE                   0
#define LOW_COMPLEXITY_PROFILE         1
#define SCALABLE_SAMPLING_RATE_PROFILE 2
#define RESERVED                       3

typedef struct adts_header
{
  UINT16 syncword;                  //12
  UINT8  ID;                        //1
  UINT8  layer;                     //2
  UINT8  protection_absent;         //1
  UINT8  profile;                   //2
  UINT8  sampling_frequency_index;  //4
  UINT8  private_bit;               //1
  UINT8  channel_configuration;     //3
  UINT8  original_copy;             //1
  UINT8  home;                      //1
  
  UINT8  copyrighted_stream;
  UINT8  copyrighted_start;
  UINT16 frame_length;
  UINT16 buffer_fullness;
  UINT8  numbers;
  UINT16 CRC;
}ADTS_HEADER;

typedef uint8_t* AACData;

void parseAACData(UINT8* RawData, UINT32 dataLen);

#endif // GY_AAC_PARSE_H