#include "example.h"

#include "aacParse.h"
#include "wavParse.h"
#include "oggParse.h"

#include "pcmEncoder.h"

#include "decoderExample.h"

#include <stdio.h>
#include <string.h>
#include <map>

void gyMain(GY::DataSource* source)
{
  const char* PATH_NAME = source->_argv[2];

  switch( getDataSourceOp(source) )
  {
    case Configure::AAC_PARSE_OP:
      aacParseExample(PATH_NAME);
      break;
    case Configure::WAC_PARSE_OP:
      wavParseExample(PATH_NAME);
      break;
    case Configure::OGG_PARSE_OP:
      oggParseExample(PATH_NAME);
      break;
    case Configure::PCM_ENCODER_OP:
      pcmEncoderExample(source->_argv[2], source->_argv[3]);
      break;
    case Configure::AAC_ADTS_DECODER_OP:
      aacAdtsDecoderExample(source->_argv[2], source->_argv[3]);
      break;
    default:
      break;
  }
}