#include "transformCode.h"

namespace GY
{

typedef struct TRANSFORM_MATRIX
{
  int hadamard = 1;
  int hadamard2x2[2][2];
  int hadamard4x4[4][4];
  int hadamard8x8[8][8];
  int hadamard16x16[16][16];
  
  const double COEFF2X2   = 1.0 / sqrt(2);
  const double COEFF4X4   = 1.0 / 2.0;
  const double COEFF8X8   = COEFF2X2 * COEFF4X4;
  const double COEFF16X16 = COEFF2X2 * COEFF8X8;
  
  const int dctMatric4X4[4][4]  =
  {
    {1,  1,  1,  1},
    {2,  1, -1, -2},
    {1, -1, -1,  1},
    {1, -2,  2, -1}
  };
  
  const int dctMatric4X4T[4][4] = 
  {
    {1,  2,  1,  1},
    {1,  1, -1, -2},
    {1, -1, -1,  2},
    {1, -2,  1, -1}
  };
  
  TRANSFORM_MATRIX()
  {
    // 2*2 hadamard matrix
    hadamard2x2[0][0] = 
    hadamard2x2[0][1] = 
    hadamard2x2[1][0] = hadamard;
    hadamard2x2[1][1] = -hadamard;
    
    // 4*4 hadamard matrix
    for(int i=0; i<2; ++i)
      for(int j=0; j<2; ++j)
        hadamard4x4[i][j] = hadamard2x2[i][j];
    for(int i=0; i<2; ++i)
      for(int j=2; j<4; ++j)
        hadamard4x4[i][j] = hadamard2x2[i][j-2];
    for(int i=2; i<4; ++i)
      for(int j=0; j<2; ++j)
        hadamard4x4[i][j] = hadamard2x2[i-2][j];
    for(int i=2; i<4; ++i)
      for(int j=2; j<4; ++j)
        hadamard4x4[i][j] = -hadamard2x2[i-2][j-2];
    
    // 8*8 hadamard matrix
    for(int i=0; i<4; ++i)
      for(int j=0; j<4; ++j)
        hadamard8x8[i][j] = hadamard4x4[i][j];
    for(int i=0; i<4; ++i)
      for(int j=4; j<8; ++j)
        hadamard8x8[i][j] = hadamard4x4[i][j-4];
    for(int i=4; i<8; ++i)
      for(int j=0; j<4; ++j)
        hadamard8x8[i][j] = hadamard4x4[i-4][j];
    for(int i=4; i<8; ++i)
      for(int j=4; j<8; ++j)
        hadamard8x8[i][j] = -hadamard4x4[i-4][j-4];
      
    // 16*16 hadamard matrix
    for(int i=0; i<8; ++i)
      for(int j=0; j<8; ++j)
        hadamard16x16[i][j] = hadamard8x8[i][j];
    for(int i=0; i<8; ++i)
      for(int j=8; j<16; ++j)
        hadamard16x16[i][j] = hadamard8x8[i][j-4];
    for(int i=8; i<16; ++i)
      for(int j=0; j<8; ++j)
        hadamard16x16[i][j] = hadamard8x8[i-4][j];
    for(int i=8; i<16; ++i)
      for(int j=8; j<16; ++j)
        hadamard16x16[i][j] = -hadamard8x8[i-4][j-4];
  }
}TRANSFORM_MATRIX;

static TRANSFORM_MATRIX HADAMARD_MATRIX;

void HadamardTransform4x4(Macroblock4x4& m)
{
  double kmm[10][4][4]  = {0.0};
  double kmn[10][4][4]  = {0.0};
  int    diff[10][4][4] = {0};
  
  for(int z=0; z<10; z++)
    for(int i=0; i<4; ++i)
      for(int j=0; j<4; ++j)
        diff[z][i][j] = m.getMatrixData(i, j) - m.getPreMatrixData(z, i, j);
  
  for(int z=0; z<10; ++z)
    for(int i=0; i<4; i++)
      for(int j=0; j<4; ++j)
        for(int k=0; k<4; ++k)
          kmm[z][i][j] += HADAMARD_MATRIX.hadamard4x4[i][k] * diff[z][k][j];
        
  for(int z=0; z<10; ++z)
    for(int i=0; i<4; i++)
      for(int j=0; j<4; ++j)
        for(int k=0; k<4; ++k)
          kmn[z][i][j] += kmm[z][i][k] * HADAMARD_MATRIX.hadamard4x4[k][j];
  
  for(int z=0; z<10; z++)
    for(int i=0; i<4; ++i)
      for(int j=0; j<4 ;++j)
        m.setHadamardMatrixData(z, i, j, kmn[z][i][j] * HADAMARD_MATRIX.COEFF4X4);
}

void HadamardTransform16x16(Macroblock16x16& m)
{
  double kmm[5][16][16]  = {0.0};
  double kmn[5][16][16]  = {0.0};
  int    diff[5][16][16] = {0};
  
  for(int z=0; z<5; z++)
    for(int i=0; i<16; ++i)
      for(int j=0; j<16; ++j)
        diff[z][i][j] = m.getMatrixData(i, j) - m.getPreMatrixData(z, i, j);
  
  for(int z=0; z<5; ++z)
    for(int i=0; i<16; i++)
      for(int j=0; j<16; ++j)
        for(int k=0; k<16; ++k)
          kmm[z][i][j] += HADAMARD_MATRIX.hadamard16x16[i][k] * diff[z][k][j];
        
  for(int z=0; z<5; ++z)
    for(int i=0; i<16; i++)
      for(int j=0; j<16; ++j)
        for(int k=0; k<16; ++k)
          kmn[z][i][j] += kmm[z][i][k] * HADAMARD_MATRIX.hadamard16x16[k][j];
  
  for(int z=0; z<5; z++)
    for(int i=0; i<16; ++i)
      for(int j=0; j<16 ;++j)
        m.setHadamardMatrixData(z, i, j, kmn[z][i][j] * HADAMARD_MATRIX.COEFF16X16);
}

void DctTransform4x4(Macroblock4x4& m)
{
  int kmm[10][4][4]  = {0};
  int kmn[10][4][4]  = {0};
  int diff[10][4][4] = {0};
  
  for(int z=0; z<10; z++)
    for(int i=0; i<4; ++i)
      for(int j=0; j<4; ++j)
        diff[z][i][j] = m.getMatrixData(i, j) - m.getPreMatrixData(z, i, j);
      
  for(int z=0; z<10; ++z)
    for(int i=0; i<4; i++)
      for(int j=0; j<4; ++j)
        for(int k=0; k<4; ++k)
          kmm[z][i][j] += HADAMARD_MATRIX.dctMatric4X4[i][k] * diff[z][k][j];
        
  for(int z=0; z<10; ++z)
    for(int i=0; i<4; i++)
      for(int j=0; j<4; ++j)
        for(int k=0; k<4; ++k)
          kmn[z][i][j] += kmm[z][i][k] * HADAMARD_MATRIX.dctMatric4X4[k][j];
        
  for(int z=0; z<10; z++)
    for(int i=0; i<4; ++i)
      for(int j=0; j<4; ++j)
        m.setDctMatrixData(z, i, j, kmn[z][i][j]);
}

}