#include "wavParse.h"
#include "audioTool.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char CHUNKID[]       = "ChunkID        ";
static const char CHUNKSIZE[]     = "ChunkSize      ";
static const char FORMAT[]        = "Format         ";
static const char SUBCHUNKID1[]   = "FmtChunkID     ";
static const char SUBCHUNKSIZE1[] = "FmtChunkSize   ";
static const char AUDIOFORMAT[]   = "AudioFormat    ";
static const char NUMCHANNELS[]   = "NumChannels    ";
static const char SAMPLERATE[]    = "SampleRate     ";
static const char BYTERATE[]      = "ByteRate       ";
static const char BLOCKALIGN[]    = "BlockAlign     ";
static const char BITSPERSAMPLE[] = "BitsPerSample  ";
static const char SUBCHUNKID2[]   = "DataChunkID    ";
static const char SUBCHUNKSIZE2[] = "DataChunkSize  ";
static const char INFO[]          = "Info           ";  

static void DisplayWavHeader(WAV_HEADER* header);

WAV_HEADER* parseWAVData(uint8_t* wavData, uint32_t dataLen)
{
  WAV_HEADER* _res_header = static_cast<WAV_HEADER*>(malloc(sizeof(WAV_HEADER)));
  _res_header->_data = NULL;
  _res_header->_list = NULL;

  WAV_HEADER  _header = *_res_header;

  uint32_t cur_index = 0;

  while(cur_index < dataLen)
  {
    // RIFF
    _header._chunk_id[0] = wavData[cur_index++];
    _header._chunk_id[1] = wavData[cur_index++];
    _header._chunk_id[2] = wavData[cur_index++];
    _header._chunk_id[3] = wavData[cur_index++];
    _header._chunk_id[4] = 0; 

    _header._chunk_size = GET_UINT32_FROM_ARRAY(wavData, cur_index);
    cur_index += 4;

    // WAVE
    _header._format[0] = wavData[cur_index++];
    _header._format[1] = wavData[cur_index++];
    _header._format[2] = wavData[cur_index++];
    _header._format[3] = wavData[cur_index++];
    _header._format[4] = 0;
    
    // fmt 
    _header._sub_chunk_id[0] = wavData[cur_index++];
    _header._sub_chunk_id[1] = wavData[cur_index++];
    _header._sub_chunk_id[2] = wavData[cur_index++];
    _header._sub_chunk_id[3] = wavData[cur_index++];
    _header._sub_chunk_id[4] = 0;

    _header._sub_chunk_size = GET_UINT32_FROM_ARRAY(wavData, cur_index);
    cur_index += 4;

    _header._audio_format = GET_UINT16_FROM_ARRAY(wavData, cur_index);
    cur_index += 2;

    _header._num_channels = GET_UINT16_FROM_ARRAY(wavData, cur_index);
    cur_index += 2;

    _header._sample_rate = GET_UINT32_FROM_ARRAY(wavData, cur_index);
    cur_index += 4;

    _header._byte_rate = GET_UINT32_FROM_ARRAY(wavData, cur_index);
    cur_index += 4;

    _header._block_align = GET_UINT16_FROM_ARRAY(wavData, cur_index);
    cur_index += 2;

    _header._bits_per_sample = GET_UINT16_FROM_ARRAY(wavData, cur_index);
    cur_index += 2;

    _header._sub_chunk_2_id[0] = wavData[cur_index++];
    _header._sub_chunk_2_id[1] = wavData[cur_index++];
    _header._sub_chunk_2_id[2] = wavData[cur_index++];
    _header._sub_chunk_2_id[3] = wavData[cur_index++];
    _header._sub_chunk_2_id[4] = 0;

    _header._sub_chunk_2_size = GET_UINT32_FROM_ARRAY(wavData, cur_index);
    cur_index += 4;
    
    if( memcmp(_header._sub_chunk_2_id, "LIST", 4) == 0 )
    {
      _header._list = allocWavList(_header._sub_chunk_2_size);
      _header._list->_list_info[0] = wavData[cur_index++];
      _header._list->_list_info[1] = wavData[cur_index++];
      _header._list->_list_info[2] = wavData[cur_index++];
      _header._list->_list_info[3] = wavData[cur_index++];
      _header._list->_list_info[4] = 0;

      int number = _header._list->_list_length;
      number -= 4;
      while(number > 0)
      {
        char nodeName[5];
        int  nodeLength;
         
        nodeName[0] = wavData[cur_index++];
        nodeName[1] = wavData[cur_index++];
        nodeName[2] = wavData[cur_index++];
        nodeName[3] = wavData[cur_index++];
        nodeName[4] = 0;

        nodeLength = GET_UINT32_FROM_ARRAY(wavData, cur_index);
        cur_index += 4;

        wavListAdd(&(_header._list), wavData + cur_index - 8, nodeLength, wavData + cur_index);;

        cur_index += nodeLength;
        number = number - nodeLength - 8;

        while( wavData[cur_index] == 0 ) cur_index++, number--;
      }
    }

    _header._sub_chunk_2_id[0] = wavData[cur_index++];
    _header._sub_chunk_2_id[1] = wavData[cur_index++];
    _header._sub_chunk_2_id[2] = wavData[cur_index++];
    _header._sub_chunk_2_id[3] = wavData[cur_index++];
    _header._sub_chunk_2_id[4] = 0;

    _header._sub_chunk_2_size = GET_UINT32_FROM_ARRAY(wavData, cur_index);
    cur_index += 4;

    _header._data = (WAV_CHAR*) malloc ( sizeof(char) * _header._sub_chunk_2_size );
    memcpy(_header._data, wavData + cur_index, _header._sub_chunk_2_size);

    DisplayWavHeader(&_header);
    break;
  }

  return _res_header;
}

void DisplayWavHeader(WAV_HEADER* header)
{
  printf("WAV_HEADER:\n");
  printf("\t%s: %s.\n", CHUNKID,       header->_chunk_id);
  printf("\t%s: %d.\n", CHUNKSIZE,     header->_chunk_size);
  printf("\t%s: %s.\n", FORMAT,        header->_format);
  printf("\t%s: %s.\n", SUBCHUNKID1,   header->_sub_chunk_id);
  printf("\t%s: %d.\n", SUBCHUNKSIZE1, header->_sub_chunk_size);
  printf("\t%s: %d.\n", AUDIOFORMAT,   header->_audio_format);
  printf("\t%s: %d.\n", NUMCHANNELS,   header->_num_channels);
  printf("\t%s: %d.\n", SAMPLERATE,    header->_sample_rate);
  printf("\t%s: %d.\n", BYTERATE,      header->_byte_rate);
  printf("\t%s: %d.\n", BLOCKALIGN,    header->_block_align);
  printf("\t%s: %d.\n", BITSPERSAMPLE, header->_bits_per_sample);

  if( header->_list != NULL )
  {
    printf("\t%s: ", INFO);

    WAV_LIST* list = header->_list;
    printf("%s: %s\n", list->_list_cur->_list_node_name, 
                       list->_list_cur->_list_node_data);
    while( wavListNext(list) )
    {
      printf("\t                 %s: %s\n", 
        list->_list_cur->_list_node_name, list->_list_cur->_list_node_data);
    }
  }

  printf("\t%s: %s.\n", SUBCHUNKID2,   header->_sub_chunk_2_id);
  printf("\t%s: %d.\n", SUBCHUNKSIZE2, header->_sub_chunk_2_size);

  return;
}

void freeWavHeader(WAV_HEADER* header)
{
  if( header->_list != NULL)
    freeWavList( header->_list );

  if( header->_data != NULL )
    free( header->_data = NULL );

  header->_list = NULL;
  header->_data = NULL;
}

WAV_NODE* allocWavListNode(const uint8_t* nodeName, int nodeLength, const uint8_t* nodeData)
{
  WAV_NODE* node = static_cast<WAV_NODE*>(malloc(sizeof(WAV_NODE)));
  initWavListNode(node);

  memcpy(node->_list_node_name, nodeName, 4);
  node->_list_node_name[4] = 0;

  node->_list_node_length = nodeLength + 1;
  node->_list_node_data   = (char*) malloc (sizeof(char) * (nodeLength + 1));
  memcpy(node->_list_node_data, nodeData, nodeLength);
  node->_list_node_data[nodeLength] = 0;

  return node;
}

void initWavListNode(WAV_NODE* node)
{
  memcpy(node->_list_node_name, "none", 4);
  node->_list_node_length = 0;
  node->_list_node_data   = NULL;

  node->_front = NULL;
  node->_nextr = NULL;
}

void freeWavListNode(WAV_NODE* node)
{
  if(node == NULL)
    return;

  if( node->_list_node_data != NULL )
    free( node->_list_node_data );

  node->_list_node_data = NULL;
  free( node );
}

WAV_LIST* allocWavList(uint32_t listLength)
{
  WAV_LIST* list = static_cast<WAV_LIST*>( malloc(sizeof(WAV_LIST)) );
  initWavList(list);

  list->_list_length = listLength;

  return list;
}

void initWavList(WAV_LIST* list)
{
  if( list == NULL )
    return;

  list->_list_length = 0;
  list->_list_head   = NULL;
  list->_list_end    = NULL;
  list->_list_cur    = NULL;
}

void freeWavList(WAV_LIST* list)
{
  if( list != NULL )
    free(list);
}

void wavListAdd(WAV_LIST** list, const uint8_t* nodeName, int nodeLength, const uint8_t* nodeData)
{
  if( *list == NULL )
    return;

  WAV_LIST* _list = *list;
  WAV_NODE* node = allocWavListNode(nodeName, nodeLength, nodeData);

  if( _list->_list_end == NULL )
  {
    _list->_list_head = _list->_list_end = _list->_list_cur = node;
  }
  else
  {
    _list->_list_end->_nextr = node;
    node->_front = _list->_list_end;
    _list->_list_end = node;
  }
}

WAV_LIST_NODE_DATA wavListCurData(WAV_LIST* list)
{
  WAV_LIST_NODE_DATA data;
  memcpy(data._list_node_name, list->_list_cur->_list_node_name, 4);
  data._list_node_length = list->_list_cur->_list_node_length;
  data._list_node_data   = list->_list_cur->_list_node_data;

  return data;
}

bool wavListNext(WAV_LIST* list)
{
  if( list->_list_cur->_nextr == NULL )
    return false;

  list->_list_cur = list->_list_cur->_nextr;
  return true;
}

void wavParseExample(const char* FILE_PATH_NAME)
{
  FILE* fp = fopen(FILE_PATH_NAME, "rb");
  
  uint8_t* buffer;
  size_t size;
  buffer = (uint8_t*)calloc(1024 * 1024 * 50, sizeof(uint8_t));
  
  // get file size
  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  if(size <= 0)
  {
    printf("File Path Error.\n");
    return;
  }
  
  // read file data
  int ret = fread(buffer, 1, size, fp);
  fclose(fp);
 
  // start
  WAV_HEADER* header = parseWAVData(buffer, (uint32_t)size);
  freeWavHeader( header );
}