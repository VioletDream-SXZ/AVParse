#ifndef CJR_AND_YSQ_EXAMPLE_H
#define CJR_AND_YSQ_EXAMPLE_H

#include "DataStructor.h"

enum Configure
{
  AAC_PARSE_OP = 1,
  WAC_PARSE_OP = 2,
  OGG_PARSE_OP = 3,
  
  PCM_ENCODER_OP = 100,

  AAC_ADTS_DECODER_OP = 1000,
};

void gyMain(GY::DataSource* source);

#endif // CJR_AND_YSQ_EXAMPLE_H