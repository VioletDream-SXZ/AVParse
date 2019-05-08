#include "aacAdtsDecoder.h"

namespace GY
{

aacAdtsDecoder::aacAdtsDecoder()
{
}

bool aacAdtsDecoder::initDecoder()
{
  handler = aacDecoder_Open(TT_MP4_ADTS, 1);

  if( handler == NULL )
    return false;

  return true;
}

int aacAdtsDecoder::decoderFrameData(const unsigned char* pIn, int pInSize, std::string& pcmData)
{
  if( pInSize <= 0 || handler == NULL )
    return -1;

  CStreamInfo *pAacInfo;
  UCHAR *inBufChain[1] = { (UCHAR*)pIn };
  UINT  pbufSize[1] = { (UINT)pInSize };
  UINT  bytesValid = (UINT)pInSize;
  int ret;

  while( bytesValid > 0 )
  {
    ret = aacDecoder_Fill(handler, inBufChain, pbufSize, &bytesValid);

    if( ret != AAC_DEC_OK )
      return -1;

    ret = aacDecoder_DecodeFrame(handler, (INT_PCM*)_pcm_data, sizeof(_pcm_data) / sizeof(INT_PCM), 0);

    if( ret == AAC_DEC_NOT_ENOUGH_BITS )
      continue;

    if( ret != AAC_DEC_OK )
      return -1;

    pAacInfo = aacDecoder_GetStreamInfo(handler);
    if( pAacInfo != NULL )
    {
      if( pAacInfo->frameSize > 0 )
      {
        if( pAacInfo->numChannels == 2 )
          pcmData.append( (char*)_pcm_data, pAacInfo->frameSize * 2 * 2 );
        else
          pcmData.append( (char*)_pcm_data, pAacInfo->frameSize * 2 );
      }
    }
  }

  return 1;
}

int aacAdtsDecoder::closeDecoder()
{
  aacDecoder_Close(handler);
}

}