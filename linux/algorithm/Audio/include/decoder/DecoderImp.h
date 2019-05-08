#ifndef CJR_AAC_DECODER_H
#define CJR_AAC_DECODER_H

#include <fdk-aac/aacenc_lib.h>

namespace GY
{

class FdkAacDecoder
{
 public:
   FdkAacDecoder();

   void InitRawDecoder();
   void InitAdtsDecoder();

 private:
   
}

}

#endif