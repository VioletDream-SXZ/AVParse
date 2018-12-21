#include "../base/videoIO.h"

using namespace GY;

int main()
{
  YUV420ToWidth_Height("D:\\Users\\Desktop\\out1.yuv", "D:\\Users\\Desktop\\out3.yuv", 1280, 720, 64, 64, 1000);
  return 0;
}