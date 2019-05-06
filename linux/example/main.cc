#include "example.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
  if(argc <= 2)
  {
    printf("Need Op And FilePath To Run.\n");
    printf("    aacParseExample:    aacParse.\n");
    printf("    wavParseExample:    wavParse.\n");
    return 0;
  }

  GY::DataSource* source = NULL;
  if( memcmp( argv[1], "aacParseExample", strlen("aacParseExample") ) == 0 )
  {
    source = GY::allocDataSource(Configure::AAC_PARSE_OP, strlen(argv[1]), argv[1], argc, argv);
  }
  else if( memcmp( argv[1], "wavParseExample", strlen("wavParseExample") ) == 0 )
  {
    source = GY::allocDataSource(Configure::WAC_PARSE_OP, strlen(argv[1]), argv[1], argc, argv);
  }

  gyMain(source);
}