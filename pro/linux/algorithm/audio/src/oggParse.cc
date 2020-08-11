#include "oggParse.h"
#include "audioTool.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static char CAPTUREPATTERN[] = "CapturePattern               ";
static char VERSION[]        = "StreamVersion                ";
static char TYPEFLAG[]       = "TypeFlag                     ";
static char SERIALNUMBER[]   = "BitstreamSerialNumber        ";
static char PAGESEQUENCE[]   = "PageSequenceNumber           ";
static char PAGESEGMENT[]    = "PageSegmentNumber            ";

static void displayOggHeader(OGG_HEADER * header)
{
  printf("Ogg Header:\n");
  printf("\t%s: %s.\n", CAPTUREPATTERN, header->_capture_pattern);
  printf("\t%s: %d.\n", VERSION,        header->_stream_structure_version);
  printf("\t%s: %d.\n", TYPEFLAG,       header->_header_type_flag);
  printf("\t%s: %d.\n", SERIALNUMBER,   header->_bitstream_serial_number);
  printf("\t%s: %d.\n", PAGESEQUENCE,   header->_page_sequence_number);
  printf("\t%s: %d.\n", PAGESEGMENT,    header->_number_page_segment);  
}

void parseOGGData(uint8_t* data, uint32_t length)
{
  OGG_LIST * list = getListByParseOGGData(data, length);

  do
  {
    displayOggHeader(list->_curr->_ogg_header);
  }
  while( OggListNext(list) );
}

OGG_LIST* getListByParseOGGData(uint8_t* data, uint32_t length)
{
  OGG_LIST * list = allocOggList();
  OGG_HEADER * header = NULL;

  int cur_index = 0;
  while( cur_index < length )
  {
    header = allocOggHeader();
    
    header->_capture_pattern[0] = data[cur_index ++];
    header->_capture_pattern[1] = data[cur_index ++];
    header->_capture_pattern[2] = data[cur_index ++];
    header->_capture_pattern[3] = data[cur_index ++];
    header->_capture_pattern[4] = 0;

    header->_stream_structure_version = data[cur_index ++];

    header->_header_type_flag = data[cur_index ++];

    for(int i=0; i<8; ++i)
      header->_granule_position[i] = data[cur_index ++];

    header->_bitstream_serial_number = GET_UINT32_FROM_ARRAY(data, cur_index);
    cur_index += 4;

    header->_page_sequence_number = GET_UINT32_FROM_ARRAY(data, cur_index);
    cur_index += 4;

    header->_CRC_checksum = GET_UINT32_FROM_ARRAY(data, cur_index);
    cur_index += 4;

    header->_number_page_segment = data[cur_index ++];

    for(int i=0; i<header->_number_page_segment; ++i)
      header->_segment_table[i] = data[cur_index ++];
  
    for(int i=0; i<header->_number_page_segment; ++i)
      cur_index += header->_segment_table[i];

    OggListAdd(&list, header);
  }

  return list;
}

OGG_LIST* allocOggList()
{
  OGG_LIST * res_list = static_cast<OGG_LIST*>( malloc( sizeof(OGG_LIST) ) );
  res_list->_head = res_list->_curr = res_list->_endr = NULL;

  return res_list;
}

void freeOggList(OGG_LIST* list)
{
  if( list == NULL )
    return;

  free( list );
  return;
}

bool OggListNext(OGG_LIST* list)
{
  if( list == NULL )
    return false;

  if( list->_curr->_nextr == NULL )
    return false;

  list->_curr = list->_curr->_nextr;
  return true;
}

void OggListAdd(OGG_LIST** list_point, OGG_HEADER* oggHeader)
{
  OGG_LIST* list = *list_point;

  if( list == NULL )
    return;

  OGG_LIST_NODE * node = allocOggListNode();
  node->_ogg_header = oggHeader;

  if( list->_head == NULL )
  {
    list->_head = list->_endr = list->_curr = node;
  }
  else
  {
    list->_endr->_nextr = node;
    node->_front = list->_endr;
    list->_endr = node;
  }
}

OGG_LIST_NODE* OggListGet(OGG_LIST* list)
{
  if( list == NULL || list->_curr == NULL )
    return NULL;

  return list->_curr;
}

OGG_LIST_NODE* allocOggListNode()
{
  OGG_LIST_NODE * node = (OGG_LIST_NODE*) malloc ( sizeof(OGG_LIST_NODE) );
  node->_nextr = node->_front = NULL;
  node->_ogg_header = NULL;

  return node;
}

void freeOggListNode(OGG_LIST_NODE* node)
{
  if( node )
    free(node);

  return;
}

OGG_HEADER* allocOggHeader()
{
  OGG_HEADER * header = (OGG_HEADER*) malloc ( sizeof(OGG_HEADER) );

  return header;
}

void freeOggHeader(OGG_HEADER* header)
{
  if( header )
    free( header );

  return;
}

void oggParseExample(const char* FILE_PATH_NAME)
{
  FILE* fp = fopen(FILE_PATH_NAME, "rb");
  
  uint8_t* buffer;
  size_t size;
  buffer = (uint8_t*)calloc(1024 * 1024 * 5, sizeof(uint8_t));
  
  // get file size
  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  
  if( size <= 0 )
    return;

  // read file data
  int ret = fread(buffer, 1, size, fp);
  fclose(fp);
 
  // start
  parseOGGData(buffer, (uint32_t)size);
}