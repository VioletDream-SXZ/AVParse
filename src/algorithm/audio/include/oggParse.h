#ifndef CJR_OGG_PARSE_H
#define CJR_OGG_PARSE_H

#include <stdint.h>

typedef struct Ogg_Header
{
  char    _capture_pattern[5];        // 为OGGS
  uint8_t _stream_structure_version;
  uint8_t _header_type_flag;
  uint8_t _granule_position[8];
  uint32_t _bitstream_serial_number;  // 逻辑页在全部页中的序号。
  uint32_t _page_sequence_number;     // 当前页在逻辑页里面的序号。
  uint32_t _CRC_checksum;
  uint8_t  _number_page_segment;      // 接下来页长度数组的大小。
  uint8_t  _segment_table[256];       // 每一个都包含一个长度，所有加起来就是接下来的数据的长度。
}OGG_HEADER;

typedef struct Ogg_List_Node
{
  OGG_HEADER* _ogg_header;

  struct Ogg_List_Node* _front;
  struct Ogg_List_Node* _nextr;
}OGG_LIST_NODE;

typedef struct Ogg_List
{
  OGG_LIST_NODE* _head;
  OGG_LIST_NODE* _curr;
  OGG_LIST_NODE* _endr;
}OGG_LIST;

/**
 * @brief: ogg格式音频解析例子。
 * @param FILE_PATH_NAME: 需要解析的文件地址。
 * @return no
 */
void oggParseExample(const char* FILE_PATH_NAME);

/**
 * @brief: 解析ogg格式音频的具体函数，内部包含输出格式函数。
 * @param data   : 需要解析的数据。
 * @param length : 数据的实际长度。
 * @return no
 */
void parseOGGData(uint8_t* data, uint32_t length);

/**
 * @brief: 解析ogg格式音频的具体函数，内部包含输出格式函数。
 * @param data   : 需要解析的数据。
 * @param length : 数据的实际长度。
 * @return       : 返回解析结束的OGG_LIST列表，内部包含解析的所有数据，以双向链表的形式返回。
 */
OGG_LIST* getListByParseOGGData(uint8_t* data, uint32_t length);

// 简单的申请内存
OGG_LIST* allocOggList();

// 简单的释放内存
void freeOggList(OGG_LIST* list);

/**
 * @brief: 将list的指向的当前位置指向下一个。
 * @param list: 需要操作的链表的指针。
 * @return    : true, 存在下一个元素，指向下一个元素。
                false, 元素已经到达尾部。
 */
bool OggListNext(OGG_LIST* list);

/**
 * @brief: 将一个ogg首部信息添加到一个OGG_LIST中去。
 * @param list      : 需要操作的链表的指针的指针，只是由于可能存在指针指向的head为NULL，需要重新指向。
 * @param oggHeader : 需要添加的head结构体。
 * @return no
 */
void OggListAdd(OGG_LIST** list, OGG_HEADER* oggHeader);

// 获取当前指向的元素，NULL表示当前没有元素或者已经指向尾部。
OGG_LIST_NODE* OggListGet(OGG_LIST* list);

// 简单的申请内存
OGG_LIST_NODE* allocOggListNode();

// 简单的释放内存
void freeOggListNode(OGG_LIST_NODE* node);

// 简单的申请内存
OGG_HEADER* allocOggHeader();

// 简单的释放内存
void freeOggHeader(OGG_HEADER* header);

#endif // CJR_OGG_PARSE_H