#include "example.h"

#include "aacParse.h"
#include "wavParse.h"

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
    default:
      break;
  }
}