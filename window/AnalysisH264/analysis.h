#pragma once

#ifndef GY_ANALYSIS_H
#define GY_ANALYSIS_H

#include "types.h"
#include "x264_structure.h"

typedef struct x264_analysis_t
{
  /**
   * 无符号整数的指数哥伦布解码Ue(v)
   * 详细的算法描述可以查阅《新一代高效视频编码 H.265/HEVC 原理、标准与实现》P235 ~ P236
   * @param pBuff    : 需要解析的h.264或h.265的码流
   * @param nLen     : pBuff的总长度
   * @param nStartBit: pBuff的开始长度
   * @return 经过指数哥伦布编码之前的原始数据(无符号)
   */
  static uint32_t Ue(uint8_t *pBuff, uint32_t nLen, uint32_t &nStartBit);
  
  /**
   * 有符号整数的指数哥伦布解码Se(v)
   * 详细的算法描述可以查阅《新一代高效视频编码 H.265/HEVC 原理、标准与实现》P235 ~ P236
   * @param pBuff    : 需要解析的h.264或h.265的码流
   * @param nLen     : pBuff的总长度
   * @param nStartBit: pBuff的开始长度
   * @return 经过指数哥伦布编码之前的原始数据(有符号)
   */
  static int Se(uint8_t *pBuff, uint32_t nLen, uint32_t &nStartBit);
  
  /**
   * 读进连续的bitCunt个比特，并将它们解释为无符号的。
   * @param pBuff    : 需要解析的h.264或h.265的码流
   * @param BitCount : 需要读取的比特数
   * @param nStartBit: pBuff的开始长度
   * @return 读取的bitCount个比特。
   */
  static uint64_t U(uint8_t *pBuff, uint32_t BitCount, uint32_t &nStartBit);

  /**
   * 为了防止RBSP的字节流与有序字节流传送方式下的SCP(start_code_prefix_one_3bytes, 0x000001)
   * 出现字节竞争的情况，循环检测RBSP前三个字节，在出现字节竞争时在第三字节前加入(0x03)，所以
   * 在解码的时候，我们需要对加入的(0x03)消除，这个函数就是实现这个功能的。
   * @param buf     : 需要解析的码流信息
   * @param buf_size: 码流的总长度 
   * return nothing
   */
  static void de_emulation_prevention(uint8_t *buf, uint32_t *buf_size);
  
  /**
   * 该函数用于解析h.264中的序列参数集的变量
   * @param buf : 需要解析的h.264码流数据
   * @param nLen: 需要解析的h.264码流数据的总长度
   * @param sps_data: 解析的结果放于该结构体中。
   */
  static bool h264_decode_sps(uint8_t *buf,uint32_t nLen, GY::x264_SPS_t *sps_data);

  static void displayUint(uint8_t _val);
  static void displayUint(uint32_t _val);
  static void displayUint(uint64_t _val);
}x264_analysis_t;

#endif // GY_ANALYSIS_H