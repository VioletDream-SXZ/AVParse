#ifndef CJR_WAV_PARSE_H
#define CJR_WAV_PARSE_H

#include <stdint.h>

typedef struct wav_list_node_data
{
  char  _list_node_name[5];
  int   _list_node_length;
  char* _list_node_data;
}WAV_LIST_NODE_DATA;

typedef struct wav_list_node
{
  char  _list_node_name[5];
  int   _list_node_length;
  char* _list_node_data;

  struct wav_list_node* _front;
  struct wav_list_node* _nextr;
}WAV_NODE;

typedef struct wav_list
{
  uint32_t _list_length;
  char     _list_info[5];

  WAV_NODE* _list_head;
  WAV_NODE* _list_end;
  WAV_NODE* _list_cur;
}WAV_LIST;

typedef struct wav_header
{
  char     _chunk_id[5];
  uint32_t _chunk_size;
  char     _format[5];

  char     _sub_chunk_id[5];
  uint32_t _sub_chunk_size;
  uint16_t _audio_format;
  uint16_t _num_channels;
  uint32_t _sample_rate;
  uint32_t _byte_rate;
  uint16_t _block_align;
  uint16_t _bits_per_sample;

  char     _sub_chunk_2_id[5];
  uint32_t _sub_chunk_2_size;

  WAV_LIST* _list;
}WAV_HEADER;

WAV_NODE* allocWavListNode(const uint8_t* nodeName, int nodeLength, const uint8_t* nodeData);
void initWavListNode(WAV_NODE* node);
void freeWavListNode(WAV_NODE* node);

WAV_LIST* allocWavList(uint32_t listLength);
void initWavList(WAV_LIST* list);
void freeWavList(WAV_LIST* list);
void wavListAdd(WAV_LIST** list, const uint8_t* nodeName, int nodeLength, const uint8_t* nodeData);
WAV_LIST_NODE_DATA wavListCurData(WAV_LIST* list);
bool wavListNext(WAV_LIST* list);

void parseWAVData(uint8_t* wavData, uint32_t dataLen);
void wavParseExample(const char* PATH_NAME);

#endif // CJR_WAV_PARSE_H