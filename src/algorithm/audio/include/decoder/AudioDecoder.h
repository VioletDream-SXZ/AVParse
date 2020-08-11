#ifndef CJR_DECODER_ON_AUDIO_H
#define CJR_DECODER_ON_AUDIO_H

#include <string>
#include <fdk-aac/aacdecoder_lib.h>

namespace GY
{

class AudioDecoder
{
 public:
  virtual ~AudioDecoder() = default;

  virtual bool initDecoder() = 0;
  virtual int decoderFrameData(const unsigned char* pIn, int pInSize, std::string& pcmData) = 0;
  virtual int closeDecoder() = 0;

 protected:
  AudioDecoder() = default;
};

}

#endif // CJR_DECODER_ON_AUDIO_H