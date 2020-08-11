#include "global.h"
#include "../logger/logger.h"

namespace GY
{

/********************************************
          start Macroblock16x16
 *******************************************/
void Macroblock16x16::setMatrixData(int x, int y, int data)
{
  if(x < 0 || y < 0 || x >= 16 || y >= 16)
  {
    gy_log_error(__FILE__, __LINE__, "The (%d, %d) is error.\n", x, y);
    return;
  }
  
  this->Matrix[x][y] = data;
}

int Macroblock16x16::getMatrixData(int x, int y)
{
  if(x < 0 || y < 0 || x >= 16 || y >= 16)
  {
    gy_log_error(__FILE__, __LINE__, "The (%d, %d) is error.\n", x, y);
    return 0;
  }
  
  return this->Matrix[x][y];
}

void Macroblock16x16::setPreMatrixData(int IMODE, int x, int y, int data)
{
  this->preMatrix[IMODE][x][y] = data;
}

int  Macroblock16x16::getPreMatrixData(int IMODE, int x, int y)
{
  return this->preMatrix[IMODE][x][y];
}

void Macroblock16x16::setDctMatrixData(int IMODE, int x, int y, int data)
{
  this->dctMatrix[IMODE][x][y] = data;
}

int Macroblock16x16::getDctMatrixData(int IMODE, int x, int y)
{
  return this->dctMatrix[IMODE][x][y];
}

void Macroblock16x16::setHadamardMatrixData(int IMODE, int x, int y, double lf)
{
  this->hadamardMatrix[IMODE][x][y] = lf;
}

double Macroblock16x16::getHadamardMatrixData(int IMODE, int x, int y)
{
  return this->hadamardMatrix[IMODE][x][y];
}

void Macroblock16x16::setCostData(int IMODE, double cost)
{
  this->cost[IMODE] = cost;
}

double Macroblock16x16::getCostData(int IMODE)
{
  return this->cost[IMODE];
}

void Macroblock16x16::setX(int x)
{
  this->pos_x = x;
}

int Macroblock16x16::getX()
{
  return this->pos_x;
}

void Macroblock16x16::setY(int y)
{
  this->pos_y = y;
}

int Macroblock16x16::getY()
{
  return this->pos_y;
}

void Macroblock16x16::runIntraPredData()
{
  gy_log_debug(__FILE__, __LINE__, "runIntraPredData start.\n");

  // VERT_PRED
  if(up_avail)
  {
    for(int i=0; i<16; ++i)
      for(int j=0; j<16; ++j)
        preMatrix[IMODE::VERT_PRED][i][j] = up[j];
  }
  
  // HOR_PRED
  if(left_avail)
  {
    for(int i=0; i<16; ++i)
      for(int j=0; j<16; ++j)
        preMatrix[IMODE::HOR_PRED][i][j]  = left[i];
  }
  
  // DC_PRED
  int sum = 0;
  if(left_avail)
    for(int i=0; i<16; ++i)
      sum += left[i];
  if(up_avail)
    for(int i=0; i<16; ++i)
      sum += up[i];
  if(left_avail && up_avail)
    sum = (sum + 16) >> 5;
  else if(left_avail || up_avail)
    sum = (sum + 8) >> 4;
  else 
    sum = 128;
  
  for(int i=0; i<16; ++i)
      for(int j=0; j<16; ++j)
        preMatrix[IMODE::DC_PRED][i][j] = sum;
      
  // PLANE_PRED
  if(up_avail && left_avail)
  {
    int H = 0, V = 0;
    int a = 0, b = 0, c = 0;
    
    for(int i=0; i<8; ++i)
      H += i*(up[7+i] - up[7-i]);
    H += 8*(up[15] - P_Q);
    for(int i=0; i<8; ++i)
      V += i*(left[7+i] - left[7-i]);
    V += 8*(left[15] - P_Q);
    
    a = 16 * (up[15] + left[15]);
    b = (5 * H + 32) >> 6;
    c = (5 * H + 32) >> 6;
    
    for(int i=0; i<16; ++i)
      for(int j=0; j<16; ++j)
      {
        int _val = (a + b * (i - 7) + c * (j - 7) + 16) >> 5;
        preMatrix[IMODE::PLANE_PRED][i][j] = _val > 255 ? 255 : _val < 0 ? 0 : _val;
      }
  }
}

/********************************************
          Macroblock16x16 end
 *******************************************/

/********************************************
          start Macroblock4x4
 *******************************************/

void Macroblock4x4::setMatrixData(int x, int y, int data)
{
  if(x >= 4 || x < 0 || y >= 4 || y < 0)
  {
    gy_log_warning(__FILE__, __LINE__, "The (%d, %d) is error.\n", x, y);
    return;
  }
  
  this->Matrix[x][y] = data;
}

int Macroblock4x4::getMatrixData(int x, int y)
{
  if(x >= 4 || x < 0 || y >= 4 || y < 0)
  {
    gy_log_warning(__FILE__, __LINE__, "The (%d, %d) is error.\n", x, y);
    return -1;
  }
  
  return this->Matrix[x][y];
}

void Macroblock4x4::setPreMatrixData(int IMODE, int x, int y, int data)
{
  if(x >= 4 || x < 0 || y >= 4 || y < 0)
  {
    gy_log_warning(__FILE__, __LINE__, "The (%d, %d) is error.\n", x, y);
    return;
  }
  
  this->preMatrix[IMODE][x][y] = data;
}

int Macroblock4x4::getPreMatrixData(int IMODE, int x, int y)
{
  if(x >= 4 || x < 0 || y >= 4 || y < 0)
  {
    gy_log_warning(__FILE__, __LINE__, "The (%d, %d) is error.\n", x, y);
    return -1;
  }
  
  return this->preMatrix[IMODE][x][y];
}

void Macroblock4x4::setDctMatrixData(int IMODE, int x, int y, int data)
{
  this->dctMatrix[IMODE][x][y] = data;
}

int Macroblock4x4::getDctMatrixData(int IMODE, int x, int y)
{
  return this->dctMatrix[IMODE][x][y];
}

void Macroblock4x4::setHadamardMatrixData(int IMODE, int x, int y, double lf)
{
  this->hadamardMatrix[IMODE][x][y] = lf;
}

double Macroblock4x4::getHadamardMatrixData(int IMODE, int x, int y)
{
  return this->hadamardMatrix[IMODE][x][y];
}

void Macroblock4x4::setQuantizationMatrixData(int IMODE, int x, int y, double lf)
{
  this->quantizationMatrix[IMODE][x][y] = lf;
}

double Macroblock4x4::getQuantizationMatrixData(int IMODE, int x, int y)
{
  return this->quantizationMatrix[IMODE][x][y];
}

void Macroblock4x4::setDeQuantizationMatrixData(int IMODE, int x, int y, double lf)
{
  this->deQuantizationMatrix[IMODE][x][y] = lf;
}

double Macroblock4x4::getDeQuantizationMatrixData(int IMODE, int x, int y)
{
  return deQuantizationMatrix[IMODE][x][y];
}

void Macroblock4x4::setCostData(int IMODE, double cost)
{
  this->cost[IMODE] = cost;
}

double Macroblock4x4::getCostData(int IMODE)
{
  return this->cost[IMODE];
}

void Macroblock4x4::setX(int x)
{
  this->pos_x = x;
}

int Macroblock4x4::getX()
{
  return this->pos_x;
}

void Macroblock4x4::setY(int y)
{
  this->pos_y = y;
}

int Macroblock4x4::getY()
{
  return this->pos_y;
}

void Macroblock4x4::runIntraPredData()
{
  int sum = 0;
  int P_A = up[0], P_B = up[1], P_C = up[2], P_D = up[3];
  int P_E = up_right[0], P_F = up_right[1], P_G = up_right[2], P_H = up_right[3];
  int P_I = left[0], P_J = left[1], P_K = left[2], P_L = left[3];
  int P_M = down_left[0], P_N = down_left[1], P_O = down_left[2], P_P = down_left[3]; 
  
  // VERT_PRED
  if(up_avail)
  {
    for(int i=0; i<4; ++i)
      sum += up[i];
    
    for(int i=0; i<4; ++i)
      for(int j=0; j<4; ++j)
        preMatrix[IMODE::VERT_PRED][i][j] = up[j];
  }
  
  // HOR_PRED
  if(left_avail)
  {
    for(int i=0; i<4; ++i)
      sum += left[i];
    
    for(int i=0; i<4; ++i)
      for(int j=0; j<4; ++j)
        preMatrix[IMODE::HOR_PRED][i][j] = left[i];
  }
  
  // DC_PRED
  if(left_avail && up_avail)
  {
    for(int i=0; i<4; ++i)
      for(int j=0; j<4; ++j)
        preMatrix[IMODE::DC_PRED][i][j] = (sum + 4) >> 3;
  }
  else if(up_avail || left_avail)
  {
    for(int i=0; i<4; ++i)
      for(int j=0; j<4; ++j)
        preMatrix[IMODE::DC_PRED][i][j] = (sum + 2) >> 2;
  }
  else
  {
    for(int i=0; i<4; ++i)
      for(int j=0; j<4; ++j)
        preMatrix[IMODE::DC_PRED][i][j] = 128;
  }
  
  // DIAG_DOWN_RIGHT_PRED
  if(left_avail && up_avail)
  {
    preMatrix[IMODE::DIAG_DOWN_RIGHT_PRED][3][0] = (P_J + P_K * 2 + P_L + 2) >> 2;
    preMatrix[IMODE::DIAG_DOWN_RIGHT_PRED][2][0] = 
    preMatrix[IMODE::DIAG_DOWN_RIGHT_PRED][3][1] = (P_I + P_J * 2 + P_K + 2) >> 2;
    preMatrix[IMODE::DIAG_DOWN_RIGHT_PRED][1][0] = 
    preMatrix[IMODE::DIAG_DOWN_RIGHT_PRED][2][1] =
    preMatrix[IMODE::DIAG_DOWN_RIGHT_PRED][3][2] = (P_Q + P_I * 2 + P_J + 2) >> 2;
    preMatrix[IMODE::DIAG_DOWN_RIGHT_PRED][0][0] =
    preMatrix[IMODE::DIAG_DOWN_RIGHT_PRED][1][1] =
    preMatrix[IMODE::DIAG_DOWN_RIGHT_PRED][2][2] =
    preMatrix[IMODE::DIAG_DOWN_RIGHT_PRED][3][3] = (P_A + P_Q * 2 + P_I + 2) >> 2;
    preMatrix[IMODE::DIAG_DOWN_RIGHT_PRED][0][1] = 
    preMatrix[IMODE::DIAG_DOWN_RIGHT_PRED][1][2] = 
    preMatrix[IMODE::DIAG_DOWN_RIGHT_PRED][2][3] = (P_B + P_A * 2 + P_Q + 2) >> 2;
    preMatrix[IMODE::DIAG_DOWN_RIGHT_PRED][0][2] = 
    preMatrix[IMODE::DIAG_DOWN_RIGHT_PRED][1][3] = (P_C + P_B * 2 + P_A + 2) >> 2;
    preMatrix[IMODE::DIAG_DOWN_RIGHT_PRED][0][3] = (P_D + P_C * 2 + P_B + 2) >> 2;
  }
  
  // DIAG_DOWN_LEFT_PRED
  if(up_avail && up_right_avail)
  {
    preMatrix[IMODE::DIAG_DOWN_LEFT_PRED][0][0] = (P_A + P_B * 2 + P_C + 2) >> 2;
    preMatrix[IMODE::DIAG_DOWN_LEFT_PRED][0][1] =
    preMatrix[IMODE::DIAG_DOWN_LEFT_PRED][1][0] = (P_B + P_C * 2 + P_D + 2) >> 2;
    preMatrix[IMODE::DIAG_DOWN_LEFT_PRED][0][2] =
    preMatrix[IMODE::DIAG_DOWN_LEFT_PRED][1][1] =
    preMatrix[IMODE::DIAG_DOWN_LEFT_PRED][2][0] = (P_C + P_D * 2 + P_E + 2) >> 2;
    preMatrix[IMODE::DIAG_DOWN_LEFT_PRED][0][3] = 
    preMatrix[IMODE::DIAG_DOWN_LEFT_PRED][1][2] =
    preMatrix[IMODE::DIAG_DOWN_LEFT_PRED][2][1] = 
    preMatrix[IMODE::DIAG_DOWN_LEFT_PRED][3][0] = (P_D + P_E * 2 + P_F + 2) >> 2;
    preMatrix[IMODE::DIAG_DOWN_LEFT_PRED][1][3] =
    preMatrix[IMODE::DIAG_DOWN_LEFT_PRED][2][2] = 
    preMatrix[IMODE::DIAG_DOWN_LEFT_PRED][3][1] = (P_E + P_F * 2 + P_G + 2) >> 2;
    preMatrix[IMODE::DIAG_DOWN_LEFT_PRED][2][3] = 
    preMatrix[IMODE::DIAG_DOWN_LEFT_PRED][3][2] = (P_F + P_G * 2 + P_H + 2) >> 2;
    preMatrix[IMODE::DIAG_DOWN_LEFT_PRED][3][3] = (P_G + P_H * 3 + 2) >> 2;
  }
  
  // VERT_RIGHT_PRED
  if(up_avail && left_avail)
  {
    preMatrix[IMODE::VERT_RIGHT_PRED][0][0] = (P_A * 2 + P_B * 2 + P_J + P_K * 2 + P_L + 4) >> 3;
    preMatrix[IMODE::VERT_RIGHT_PRED][0][1] =                                             // B
    preMatrix[IMODE::VERT_RIGHT_PRED][2][0] = (P_B + P_C + 1) >> 1;
    preMatrix[IMODE::VERT_RIGHT_PRED][0][2] =                                             // C
    preMatrix[IMODE::VERT_RIGHT_PRED][2][1] = (P_C + P_D + 1) >> 1;
    preMatrix[IMODE::VERT_RIGHT_PRED][0][3] =
    preMatrix[IMODE::VERT_RIGHT_PRED][2][2] = (P_D + P_E + 1) >> 1;
    preMatrix[IMODE::VERT_RIGHT_PRED][2][3] = (P_E + P_F + 1) >> 1;
    preMatrix[IMODE::VERT_RIGHT_PRED][1][0] = (P_A + P_B * 2 + P_C + P_K + 2 * P_L + P_M + 4) >> 3;
    preMatrix[IMODE::VERT_RIGHT_PRED][1][1] = 
    preMatrix[IMODE::VERT_RIGHT_PRED][3][0] = (P_B + 2 * P_C + P_D + 2) >> 2;
    preMatrix[IMODE::VERT_RIGHT_PRED][1][2] = 
    preMatrix[IMODE::VERT_RIGHT_PRED][3][1] = (P_C + 2 * P_D + P_E + 2) >> 2;
    preMatrix[IMODE::VERT_RIGHT_PRED][1][3] =     
    preMatrix[IMODE::VERT_RIGHT_PRED][3][2] = (P_D + 2 * P_E + P_F + 2) >> 2;
    preMatrix[IMODE::VERT_RIGHT_PRED][3][3] = (P_E + 2 * P_F + P_G + 2) >> 2;
  }
  
  // VERT_LEFT_PRED
  if(up_avail && left_avail)
  {
    preMatrix[IMODE::VERT_LEFT_PRED][0][0] = 
    preMatrix[IMODE::VERT_LEFT_PRED][2][1] = (P_Q + P_A + 1) >> 1;
    preMatrix[IMODE::VERT_LEFT_PRED][0][1] = 
    preMatrix[IMODE::VERT_LEFT_PRED][2][2] = (P_A + P_B + 1) >> 1;
    preMatrix[IMODE::VERT_LEFT_PRED][0][2] =
    preMatrix[IMODE::VERT_LEFT_PRED][2][3] = (P_B + P_C + 1) >> 1;
    preMatrix[IMODE::VERT_LEFT_PRED][0][3] = (P_C + P_D + 1) >> 1;
    preMatrix[IMODE::VERT_LEFT_PRED][1][0] =
    preMatrix[IMODE::VERT_LEFT_PRED][3][1] = (P_I + P_Q * 2 + P_A + 2) >> 2;
    preMatrix[IMODE::VERT_LEFT_PRED][1][1] = 
    preMatrix[IMODE::VERT_LEFT_PRED][3][2] = (P_Q + P_A * 2 + P_B + 2) >> 2;
    preMatrix[IMODE::VERT_LEFT_PRED][1][2] = 
    preMatrix[IMODE::VERT_LEFT_PRED][3][3] = (P_A + P_B * 2 + P_C + 2) >> 2;
    preMatrix[IMODE::VERT_LEFT_PRED][1][3] = (P_B + P_C * 2 + P_D + 2) >> 2;
    preMatrix[IMODE::VERT_LEFT_PRED][2][0] = (P_Q + P_I * 2 + P_J + 2) >> 2;
    preMatrix[IMODE::VERT_LEFT_PRED][3][0] = (P_I + P_J * 2 + P_K + 2) >> 2;
  }
  
  // HOR_UP_PRED
  if(up_avail && left_avail && up_right_avail && down_left_avail)
  {
    preMatrix[IMODE::HOR_UP_PRED][0][0] = (P_B + 2 * P_C + P_D + 2 * P_I + 2 * P_J + 4) >> 3;
    preMatrix[IMODE::HOR_UP_PRED][0][1] = (P_C + 2 * P_D + P_E + P_I + 2 * P_J + P_K + 4) >> 3;
    preMatrix[IMODE::HOR_UP_PRED][0][2] =
    preMatrix[IMODE::HOR_UP_PRED][1][0] = (P_J + P_K + 1) >> 1;
    preMatrix[IMODE::HOR_UP_PRED][0][3] =
    preMatrix[IMODE::HOR_UP_PRED][1][1] = (P_J + P_K * 2 + P_L + 2) >> 2;
    preMatrix[IMODE::HOR_UP_PRED][1][2] =
    preMatrix[IMODE::HOR_UP_PRED][2][0] = (P_K + P_L + 1) >> 1;
    preMatrix[IMODE::HOR_UP_PRED][1][3] = 
    preMatrix[IMODE::HOR_UP_PRED][2][1] = (P_K + P_L * 2 + P_M + 2) >> 2;
    preMatrix[IMODE::HOR_UP_PRED][2][2] = 
    preMatrix[IMODE::HOR_UP_PRED][3][0] = (P_L + P_M * 2 + P_N + 2) >> 2;
    preMatrix[IMODE::HOR_UP_PRED][2][3] = 
    preMatrix[IMODE::HOR_UP_PRED][3][1] = (P_L + P_M + 1) >> 1;
    preMatrix[IMODE::HOR_UP_PRED][3][2] = (P_M + P_N + 1) >> 1;
    preMatrix[IMODE::HOR_UP_PRED][3][3] = (P_M + P_N * 2 + P_O + 2) >> 2;
  }
  
  // HOR_DOWN_PRED
  if(up_avail && left_avail && up_right_avail && down_left_avail)
  {
    preMatrix[IMODE::HOR_UP_PRED][0][0] =
    preMatrix[IMODE::HOR_UP_PRED][1][2] = (P_Q + P_I + 1) >> 1;
    preMatrix[IMODE::HOR_UP_PRED][0][1] =
    preMatrix[IMODE::HOR_UP_PRED][1][3] = (P_I + P_Q * 2 + P_A + 2) >> 2; 
    preMatrix[IMODE::HOR_UP_PRED][0][2] = (P_Q + P_A * 2 + P_B + 2) >> 2; 
    preMatrix[IMODE::HOR_UP_PRED][0][3] = (P_A + P_B * 2 + P_C + 2) >> 2;
    preMatrix[IMODE::HOR_UP_PRED][1][0] =
    preMatrix[IMODE::HOR_UP_PRED][2][2] = (P_I + P_J + 1) >> 1; 
    preMatrix[IMODE::HOR_UP_PRED][1][1] =
    preMatrix[IMODE::HOR_UP_PRED][2][3] = (P_Q + P_I * 2 + P_J + 2) >> 2; 
    preMatrix[IMODE::HOR_UP_PRED][2][0] =
    preMatrix[IMODE::HOR_UP_PRED][3][2] = (P_J + P_K + 1) >> 1;
    preMatrix[IMODE::HOR_UP_PRED][2][1] =
    preMatrix[IMODE::HOR_UP_PRED][3][3] = (P_I + P_J * 2 + P_K + 2) >> 2;
    preMatrix[IMODE::HOR_UP_PRED][3][0] = (P_K + P_L + 1) >> 1;
    preMatrix[IMODE::HOR_UP_PRED][3][1] = (P_J + P_K * 2 + P_L + 2) >> 2;
  }
}

/********************************************
          Macroblock4x4 end
 *******************************************/

/********************************************
          start ImagePicture
 *******************************************/

ImagePicture::ImagePicture(std::string Path, uint16_t width, uint16_t height)
{
  this->Path = Path;
  this->fp   = fopen(Path.c_str(), "rb");
  
  if(this->fp == NULL)
  {
    gy_log_warning(__FILE__, __LINE__, "Open file error.Please setFileData again.\n");
  }
  
  this->width  = width;
  this->height = height;
  
  this->yFrame = (byte*)calloc(width * height, sizeof(byte));
  this->uFrame = (byte*)calloc(width * height / 4, sizeof(byte));
  this->vFrame = (byte*)calloc(width * height / 4, sizeof(byte));
}

ImagePicture::~ImagePicture()
{
  if(yFrame)
    free(yFrame);
  
  if(uFrame)
    free(uFrame);
  
  if(vFrame)
    free(vFrame);
  
  if(fp)
    fclose(fp);
}

void ImagePicture::setYFrameData(int x, int y, byte b)
{
  if(x < 0 || x >= height || y < 0 || y >= width)
  {
    gy_log_error(__FILE__, __LINE__, "The (%d, %d) is error.\n", x, y);
    return;
  }
  
  yFrame[x * width + y] = b;
}

byte ImagePicture::getYFrameData(int x, int y)
{
  if(x < 0 || x >= height || y < 0 || y >= width)
  {
    gy_log_error(__FILE__, __LINE__, "The (%d, %d) is error.\n", x, y);
    return 0;
  }
  
  return yFrame[x * width + y];
}

bool ImagePicture::setDataPath(std::string path)
{
  this->Path = path;
  this->fp   = fopen(path.c_str(), "rb");
  
  if(this->fp)
    return true;
  else
  {
    gy_log_warning(__FILE__, __LINE__, "SetDataPath open file error.\n");
    return false;
  }
}

bool ImagePicture::getNextFrame()
{
  if(fp == NULL)
  {
    gy_log_error(__FILE__, __LINE__, "The fileFp is null.Don't getFrame from it.\n");
    return false;
  }
  
  uint32_t ret = 0;
  ret += fread(yFrame, 1, width * height, fp);
  ret += fread(uFrame, 1, width * height / 4, fp);
  ret += fread(vFrame, 1, width * height / 4, fp);
  
  if(ret == 0)
  {
    gy_log_warning(__FILE__, __LINE__, "The fp is end.\n");
    return false;
  }
  else
  {
    gy_log_info(__FILE__, __LINE__, "Read data with %d byte.\n", ret);
    return true;
  }
}

bool ImagePicture::getMacroBlock_4x4(int x, int y, Macroblock4x4& m)
{
  if(x % 4 != 0 || y % 4 != 0 ||
     x < 0 || x >= height || y < 0 || y >= width)
  {
    gy_log_warning(__FILE__, __LINE__, "The (%d, %d) is error.\n", x, y);
    return false;
  }
  
  for(int i=0; i<4; ++i)
    for(int j=0; j<4; ++j)
      m.setMatrixData(i, j, yFrame[(x+i)*width + (y+j)]);
  
  m.setX(x);
  m.setY(y);
  
  return true;
}

bool ImagePicture::getMacroBlock_16x16(int x, int y, Macroblock16x16& m)
{
  if(x % 16 != 0 || y % 16 != 0 ||
     x < 0 || x >= height || y < 0 || y >= width)
  {
    gy_log_warning(__FILE__, __LINE__, "The (%d, %d) is error.\n", x, y);
    return false;
  }

  for(int i=0; i<16; ++i)
    for(int j=0; j<16; ++j)
      m.setMatrixData(i, j, yFrame[(x+i)*width + (y+j)]);
  
  m.setX(x);
  m.setY(y);
  
  return true;
}

bool ImagePicture::setResolutionRate(uint16_t _width, uint16_t _height, std::string newFilePath, int kFrame)
{
  if(fp == NULL)
  {
    gy_log_error(__FILE__, __LINE__, "Not frame data.\n");
    return false;
  }
  
  if(_width < 0 || _height < 0 || _width >= width || _height >= height)
  {
    gy_log_error(__FILE__, __LINE__, "The resolution rate (%dx%d) is error.\n", _width, _height);
    return false;
  }
  
  FILE*    new_fp = fopen(newFilePath.c_str(), "wb");
  uint64_t offset = ftell(fp);
  
  byte*    new_yframe = (byte*)calloc(_width * _height, sizeof(byte));
  byte*    new_uframe = (byte*)calloc(_width * _height / 4, sizeof(byte));
  byte*    new_vframe = (byte*)calloc(_width * _height / 4, sizeof(byte));
  
  for(int k=0; k<kFrame; ++k)
  {
    fread(yFrame, 1, width * height, fp);
    fread(uFrame, 1, width * height / 4, fp);
    fread(vFrame, 1, width * height / 4, fp);
    
    
    for(int i=0; i<_height; ++i)
      for(int j=0; j<_width; ++j)
      {
        new_yframe[i*_width+j] = yFrame[i*width+j];

        if(!(i&1) && !(j&1))
        {
          new_uframe[i*_width/4+j/2] = uFrame[i*width/4+j/2];
          new_vframe[i*_width/4+j/2] = vFrame[i*width/4+j/2];
        }

      }
      
    fwrite(new_yframe, 1, _width * _height, new_fp);
    fwrite(new_uframe, 1, _width * _height / 4, new_fp);
    fwrite(new_vframe, 1, _width * _height / 4, new_fp);
  }
      
  free(new_yframe);
  free(new_uframe);
  free(new_vframe);
  fseek(fp, offset, SEEK_SET);
  fclose(new_fp);
  
  return true;
}

void ImagePicture::getUpMacroBlock(Macroblock4x4& m)
{
  int x = m.getX(), y = m.getY();
  if(x-1 < 0)
  {
    m.up_avail = false;
    for(int j=0; j<16; ++j)
      m.up[j] = 128;
    
    return;
  }
  
  m.up_avail = true;
  for(int j=0; j<4; ++j)
    m.up[j] = getYFrameData(x-1, y+j);
}

void ImagePicture::getUpMacroBlock(Macroblock16x16& m)
{
  int x = m.getX(), y = m.getY();
  if(x-1 < 0)
  {
    m.up_avail = false;
    for(int j=0; j<16; ++j)
      m.up[j] = 128;
    
    return;
  }
  
  m.up_avail = true;
  for(int j=0; j<16; ++j)
    m.up[j] = getYFrameData(x-1, y+j);
}

void ImagePicture::getLeftMacroBlock(Macroblock4x4& m)
{
  int x = m.getX(), y = m.getY();
  if(y-1 < 0)
  {
    m.left_avail = false;
    for(int j=0; j<4; ++j)
      m.left[j] = 128;
    
    return;
  }
  
  m.left_avail = true;
  for(int j=0; j<4; ++j)
    m.left[j] = getYFrameData(x+j, y-1);
}

void ImagePicture::getLeftMacroBlock(Macroblock16x16& m)
{
  int x = m.getX(), y = m.getY();
  if(y-1 < 0)
  {
    m.left_avail = false;
    for(int j=0; j<16; ++j)
      m.left[j] = 128;
    
    return;
  }
  
  m.left_avail = true;
  for(int j=0; j<16; ++j)
    m.left[j] = getYFrameData(x+j, y-1);
}

void ImagePicture::getUpLeftMacroBlock(Macroblock4x4& m)
{
  int x = m.getX(), y = m.getY();
  if((x-1) < 0 || (y-1) < 0)
  {
    m.P_Q = 128;
    
    return;
  }
  
  m.P_Q = getYFrameData(x-1, y-1);
}

void ImagePicture::getUpLeftMacroBlock(Macroblock16x16& m)
{
  int x = m.getX(), y = m.getY();
  if((x-1) < 0 || (y-1) < 0)
  {
    m.P_Q = 128;
    
    return;
  }
  
  m.P_Q = getYFrameData(x-1, y-1);
}

void ImagePicture::getUpRighttMacroBlock(Macroblock4x4& m)
{
  int x = m.getX(), y = m.getY();
  if((x-1) < 0 || (y + 4) >= width)
  {
    m.up_right_avail = false;
    for(int i=0; i<4; ++i)
      m.up_right[i] = 128;
    
    return;
  }
  
  m.up_right_avail = true;
  for(int i=0; i<4; ++i)
    m.up_right[i] = getYFrameData(x-1, (y+4+i));
  return;
}

void ImagePicture::getUpRightMacroBlock(Macroblock16x16& m)
{
  int x = m.getX(), y = m.getY();
  if((x-1) < 0 || (y + 16) >= width)
  {
    m.up_right_avail = false;
    for(int i=0; i<16; ++i)
      m.up_right[i] = 128;
    
    return;
  }
  
  m.up_right_avail = true;
  for(int i=0; i<16; ++i)
    m.up_right[i] = getYFrameData(x-1, (y+16+i));
  return;
}

void ImagePicture::getDownLeftMacroBlock(Macroblock4x4& m)
{
  int x = m.getX(), y = m.getY();
  if((x+4) > height || (y-1) < 0)
  {
    m.down_left_avail = false;
    for(int i=0; i<4; ++i)
      m.down_left[i] = 128;
    
    return;
  }
  
  m.down_left_avail = true;
  for(int i=0; i<4; ++i)
    m.down_left[i] = getYFrameData(x+4+i, y-1);
  return;
}

void ImagePicture::getDownLeftMacroBlock(Macroblock16x16& m)
{
  int x = m.getX(), y = m.getY();
  if((x+16) > height || (y-1) < 0)
  {
    m.down_left_avail = false;
    for(int i=0; i<16; ++i)
      m.down_left[i] = 128;
    
    return;
  }
  
  m.down_left_avail = true;
  for(int i=0; i<16; ++i)
    m.down_left[i] = getYFrameData(x+16+i, y-1);
  return;
}

void ImagePicture::saveFile(std::string Path)
{
  FILE* fp = fopen(Path.c_str(), "wb");
  fwrite(yFrame, 1, width * height, fp);
  fwrite(uFrame, 1, width * height / 4, fp);
  fwrite(vFrame, 1, width * height / 4, fp);
  fclose(fp);
}

const byte* ImagePicture::getYFrame() const
{
  return yFrame;
}

const byte* ImagePicture::getUFrame() const
{
  return uFrame;
}

const byte* ImagePicture::getVFrame() const
{
  return vFrame;
}

byte* ImagePicture::getYFrame()
{
  return yFrame;
}

byte* ImagePicture::getUFrame()
{
  return uFrame;
}

byte* ImagePicture::getVFrame()
{
  return vFrame;
}

/********************************************
          ImagePicture end
 *******************************************/

}