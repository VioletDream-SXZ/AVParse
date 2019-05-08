#include "pcmEncoder.h"
#include <fdk-aac/aacenc_lib.h>
#include <fdk-aac/FDK_audio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int pcmFileRead(FILE* fp, uint8_t* buff, int length)
{
  return fread(buff, 1, length, fp);
}

void convertBuffer(uint8_t* input, uint16_t* output, int length)
{
  for(int i=0; i<length / 2; ++i)
    output[i] = input[i * 2] + (input[i * 2 + 1] << 8);
}

int pcmEncoder(const char* inFile, const char* outFile)
{
  HANDLE_AACENCODER hAacEncoder = NULL;
  AACENC_ERROR ErrorStatus;
  AACENC_InfoStruct info = {0};
  
  // 创建编码器
  if( aacEncOpen(&hAacEncoder, 0, 2) != AACENC_OK )
  {
    printf("Unable to open fdkaac encoder.\n");
    return -1;
  }

  // 设置AOT参数
  if( aacEncoder_SetParam(hAacEncoder, AACENC_AOT, 2) != AACENC_OK )
  {
    printf("Unable to set the AOT.\n");
    return -1;
  }

  // 设置采样率
  if( aacEncoder_SetParam(hAacEncoder, AACENC_SAMPLERATE, 44100) != AACENC_OK )
  {
    printf("Unable to set the SAMPLERATE.\n");
    return -1;
  }

  // 设置通道模型
  if( aacEncoder_SetParam(hAacEncoder, AACENC_CHANNELMODE, MODE_2) != AACENC_OK )
  {
    printf("Unable to set the CHANNELMODE.\n");
    return -1;
  }

  // 设置比特率
  if( aacEncoder_SetParam(hAacEncoder, AACENC_BITRATE, 64000) != AACENC_OK )
  {
    printf("Unable to set the BITRATE.\n");
    return -1;
  }

  // 设置TRANSMUX参数
  if( aacEncoder_SetParam(hAacEncoder, AACENC_TRANSMUX, 2) != AACENC_OK )
  {
    printf("Unable to set the TRANSMUX.\n");
    return -1;
  }

  // 初始化编码器
  if( aacEncEncode(hAacEncoder, NULL, NULL, NULL, NULL) != AACENC_OK )
  {
    printf("Unable to set the encoder.\n");
    return -1;
  }

  // 获取解码器配置信息
  if( aacEncInfo(hAacEncoder, &info) != AACENC_OK )
  {
    printf("Unable to get info.\n");
    return -1;
  }

  int input_size  = 2 * 2 * info.frameLength;
  uint8_t* input_buf = (uint8_t*) malloc (sizeof(uint8_t) * input_size);
  uint16_t* convert_buf = (uint16_t*) malloc (sizeof(uint8_t) * input_size);

  FILE* inputFp = fopen(inFile, "rb");
  FILE* outputFp = fopen(outFile, "wb");

  if(inputFp <= 0)
    return -1;
  
  while(1)
  {
    AACENC_BufDesc in_buf = { 0 }, out_buf = { 0 };
    AACENC_InArgs  in_args = { 0 };
    AACENC_OutArgs out_args = { 0 };
    int in_identifier = IN_AUDIO_DATA;
    int in_size, in_elem_size;
    int out_identifier = OUT_BITSTREAM_DATA;
    int out_size, out_elem_size;
    int readNumber, i;
    void *in_ptr, *out_ptr;
    uint8_t outbuf[20480];
    AACENC_ERROR err;

    readNumber = pcmFileRead(inputFp, input_buf, input_size);

    // 由于PCM文件是以小端序存放的，需要转化成大端序才可以编码。
    convertBuffer(input_buf, convert_buf, readNumber);

    in_ptr = convert_buf;
    in_size = readNumber;
    in_elem_size = 2;

    in_args.numInSamples = readNumber <= 0 ? -1 : readNumber / 2;
    in_buf.numBufs = 1;
    in_buf.bufs = &in_ptr;
    in_buf.bufferIdentifiers = &in_identifier;
    in_buf.bufSizes = &in_size;
    in_buf.bufElSizes = &in_elem_size;

    out_ptr = outbuf;
    out_size = sizeof(outbuf);
    out_elem_size = 1;
    out_buf.numBufs = 1;
    out_buf.bufs = &out_ptr;
    out_buf.bufferIdentifiers = &out_identifier;
    out_buf.bufSizes = &out_size;
    out_buf.bufElSizes = &out_elem_size;

    // 开始编码
    if( (err = aacEncEncode(hAacEncoder, &in_buf, &out_buf, &in_args, &out_args)) != AACENC_OK )
    {
      if(err == AACENC_ENCODE_EOF)
        break;

      printf("Encoding failed.\n");
      return 1;
    }

    if(out_args.numOutBytes == 0)
      continue;

    if(outputFp > 0)
      fwrite(outbuf, 1, out_args.numOutBytes, outputFp);
  }

  free(input_buf);
  free(convert_buf);
  fclose(inputFp);
  fclose(outputFp);

  aacEncClose(&hAacEncoder);

  return 0;
}

void pcmEncoderExample(const char* inFile, const char* outFile)
{
  pcmEncoder(inFile, outFile);
}
