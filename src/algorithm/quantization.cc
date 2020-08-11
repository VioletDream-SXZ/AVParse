#include "quantization.h"

namespace GY
{

void Quantization_4x4(Macroblock4x4& m)
{
  double quan[10][4][4] = {0.0};
  
  for(int z=0; z<10; z++)
    for(int i=0; i<4; ++i)
      for(int j=0; j<4; ++j)
        quan[z][i][j] = ((int)m.getDctMatrixData(z, i, j) * QUAN.Q_4X4[0][i][j] + QPF)/(QPf);
      
  for(int z=0; z<10; z++)
    for(int i=0; i<4; ++i)
      for(int j=0; j<4; ++j)
        m.setQuantizationMatrixData(z, i, j, quan[z][i][j]);
}

void deQuantization_4x4(Macroblock4x4& m)
{
  double quan[10][4][4] = {0.0};
  
  for(int z=0; z<10; z++)
    for(int i=0; i<4; ++i)
      for(int j=0; j<4; ++j)
      {
        quan[z][i][j] = ((int)m.getQuantizationMatrixData(z, i, j) * QUAN.deQ_4X4[0][i][j]) / DEf;
        printf("%lf\n", quan[z][i][j]);
      }
      
  for(int z=0; z<10; z++)
    for(int i=0; i<4; ++i)
      for(int j=0; j<4; ++j)
        m.setDeQuantizationMatrixData(z, i, j, quan[z][i][j]);
}

}