#ifndef CJR_AAC_ADTS_DECODER_H
#define CJR_AAC_ADTS_DECODER_H

#include "AudioDecoder.h"

namespace GY
{

class aacAdtsDecoder : public AudioDecoder
{
 public:
  aacAdtsDecoder();
  virtual ~aacAdtsDecoder() = default;
  
  virtual bool initDecoder();
  virtual int decoderFrameData(const unsigned char* pIn, int pInSize, std::string& pcmData);
  virtual int closeDecoder();

 private:
  HANDLE_AACDECODER handler;
  char _pcm_data[20480];
};

}

#endif // CJR_AAC_ADTS_DECODER_H