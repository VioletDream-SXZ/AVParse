#include "decoderExample.h"

#include "aacAdtsDecoder.h"
#include "audioTool.h"

#include "aacParse.h"

static void onAacAdtsDecoder(const unsigned char* data, int len, FILE* outFp);

void aacAdtsDecoderExample(const char* INPUT_FILE_NAME,
                           const char* OUTPUT_FILE_NAME)
{
  FILE* inFp = fopen( INPUT_FILE_NAME, "rb" );
  FILE* outFp = fopen( OUTPUT_FILE_NAME, "wb" );
  
  unsigned char* buffer;
  size_t size;
  buffer = (unsigned char*)calloc(1024 * 1024 * 5, sizeof(unsigned char));
  
  // get file size
  fseek(inFp, 0, SEEK_END);
  size = ftell(inFp);
  fseek(inFp, 0, SEEK_SET);
  
  // read file data
  int ret = fread(buffer, 1, size, inFp);
 
  // start
  onAacAdtsDecoder(buffer, size, outFp);

  fclose(inFp);
  fclose(outFp);
}

void onAacAdtsDecoder(const unsigned char* data, int len, FILE* outFp)
{
  int cur_index = 0;
  GY::aacAdtsDecoder decoder;
  if( !decoder.initDecoder() )
  {
    printf("open decoder error.\n");
    return;
  } 

  while( cur_index < len )
  {
    int aacLength = getAacAudioLength( (uint8_t * )data, (uint32_t)len );
    std::string pcmData = "";

    decoder.decoderFrameData(data, aacLength, pcmData);

    fwrite(pcmData.data(), 1, pcmData.size(), outFp);
    
    data += aacLength;
    cur_index += aacLength;
  }

  decoder.closeDecoder();
  return;
}