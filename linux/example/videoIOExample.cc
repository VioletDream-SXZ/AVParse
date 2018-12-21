#include "../base/videoIO.h"

int main()
{
  YUV420ToWidth_Height("D:\\Users\\Desktop\\out1.yuv", "D:\\Users\\Desktop\\out3.yuv", 1280, 720, 640, 720, 1000);
  return 0;
}