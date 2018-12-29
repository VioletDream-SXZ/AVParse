#ifndef GY_GLOBAL_H
#define GY_GLOBAL_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <string>

typedef unsigned char byte;

namespace GY
{
  
enum IMODE
{
  VERT_PRED   = 0,
  HOR_PRED    = 1,
  DC_PRED     = 2,
  PLANE_PRED  = 3,
  DIAG_DOWN_RIGHT_PRED = 4,
  DIAG_DOWN_LEFT_PRED  = 5,
  VERT_RIGHT_PRED      = 6,
  VERT_LEFT_PRED       = 7,
  HOR_UP_PRED          = 8,
  HOR_DOWN_PRED        = 9,
};

class Macroblock16x16
{
public:
  void setMatrixData(int x, int y, int data);
  int  getMatrixData(int x, int y);
  
  void setPreMatrixData(int IMODE, int x, int y, int data);
  int  getPreMatrixData(int IMODE, int x, int y);
  
  void setDctMatrixData(int IMODE, int x, int y, int data);
  int  getDctMatrixData(int IMODE, int x, int y);
  
  void   setHadamardMatrixData(int IMODE, int x, int y, double lf);
  double getHadamardMatrixData(int IMODE, int x, int y);
  
  void   setCostData(int IMODE, double cost);
  double getCostData(int IMODE);
  
  void setX(int x);
  int  getX();
  void setY(int y);
  int  getY();
  
  void runIntraPredData();
  
public:
  int up[16];               // up
  int up_right[16];          // up_right
  int left[16];             // left
  int down_left[16];        // down_left
  int P_Q;                  // up_right
  bool up_avail, left_avail;
  bool up_right_avail, down_left_avail;

private:
  int pos_x, pos_y;
  int Matrix[16][16];
  int preMatrix[5][16][16];
  int dctMatrix[5][16][16];
  double hadamardMatrix[5][16][16];
  double cost[5];
};

class Macroblock4x4
{
public:
  void setMatrixData(int x, int y, int data);
  int  getMatrixData(int x, int y);
  
  void setPreMatrixData(int IMODE, int x, int y, int data);
  int  getPreMatrixData(int IMODE, int x, int y);
  
  void setDctMatrixData(int IMODE, int x, int y, int data);
  int  getDctMatrixData(int IMODE, int x, int y);
  
  void   setHadamardMatrixData(int IMODE, int x, int y, double lf);
  double getHadamardMatrixData(int IMODE, int x, int y); 
  
  void   setCostData(int IMODE, double cost);
  double getCostData(int IMODE);
  
  void setX(int x);
  int  getX();
  void setY(int y);
  int  getY();
  
  void runIntraPredData();

public:
  int up[4];                // up
  int up_right[4];          // up_right
  int left[4];              // left
  int down_left[4];         // down_left
  int P_Q;                  // up_right
  bool up_avail, left_avail;
  bool up_right_avail, down_left_avail;
  
private:
  int pos_x, pos_y;
  int Matrix[4][4];
  int preMatrix[10][4][4];
  int dctMatrix[10][4][4];
  int cost[15];
  double hadamardMatrix[10][4][4];
};

class ImagePicture
{
public:
  ImagePicture(std::string Path, uint16_t width, uint16_t height);
  ~ImagePicture();

  void setYFrameData(int x, int y, byte b);
  byte getYFrameData(int x, int y);

  bool setDataPath(std::string path);
  bool getNextFrame();

  bool getMacroBlock_4x4(int x, int y, Macroblock4x4& m);
  bool getMacroBlock_16x16(int x, int y, Macroblock16x16& m);

  void getUpMacroBlock(Macroblock4x4& m);
  void getUpMacroBlock(Macroblock16x16& m);
  void getLeftMacroBlock(Macroblock4x4& m);
  void getLeftMacroBlock(Macroblock16x16& m);
  void getUpLeftMacroBlock(Macroblock4x4& m);
  void getUpLeftMacroBlock(Macroblock16x16& m);
  void getUpRighttMacroBlock(Macroblock4x4& m);
  void getUpRightMacroBlock(Macroblock16x16& m);
  void getDownLeftMacroBlock(Macroblock4x4& m);
  void getDownLeftMacroBlock(Macroblock16x16& m);

private:
  std::string Path;
  FILE* fp;

  uint16_t width;
  uint16_t height;

  byte* yFrame;
  byte* uFrame;
  byte* vFrame;
};

}

#endif // GY_GLOBAL_H