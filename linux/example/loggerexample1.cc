#include "../logger/logger.h"
#include "../common/global.h"
#include "../algorithm/transformCode.h"
#include "../algorithm/quantization.h"

#include <stdio.h>

#define DEBUG 1

int main()
{
  GY::Macroblock4x4 *m   = new GY::Macroblock4x4();
  GY::ImagePicture  *img = new GY::ImagePicture("213213", 1280, 720);
  img->setDataPath("D:\\Users\\Desktop\\out.yuv");
  img->getNextFrame();
  img->getMacroBlock_4x4(480, 480, *m);
  img->getUpMacroBlock(*m);
  img->getLeftMacroBlock(*m);
  img->getUpLeftMacroBlock(*m);
  m->runIntraPredData();
  DctTransform4x4(*m);
  Quantization_4x4(*m);
  deQuantization_4x4(*m);
  
  printf("\n");
  
  for(int i=0; i<4; ++i)
  {
    for(int j=0; j<4; ++j)
      printf("%4d ", m->getDctMatrixData(0, i, j));
    printf("\n");    
  }

  printf("\n");
  
  for(int i=0; i<4; ++i)
  {
    for(int j=0; j<4; ++j)
      printf("%4d ", (int)m->getQuantizationMatrixData(0, i, j));
    printf("\n");
  }

  printf("\n");
  
  for(int i=0; i<4; ++i)
  {
    for(int j=0; j<4; ++j)
      printf("%4d ", (int)m->getDeQuantizationMatrixData(0, i, j));
    printf("\n");
  }

  printf("\n");
  
  for(int i=0; i<4; ++i)
  {
    for(int j=0; j<4; ++j)
      printf("%9.3lf ", GY::QUAN.Q_4X4[0][i][j]);
    printf("\n");
  }
  
  printf("%lf %lf\n", GY::QPf, GY::QPF);
  
  delete m;
  delete img;
}