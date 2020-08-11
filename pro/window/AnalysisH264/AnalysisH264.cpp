// AnalysisH264.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "analysis.h"
#include "x264_structure.h"

using namespace std;

uint8_t buff[] = {
  0x67, 0x64, 0x00, 0x1f, 0xac, 0xd9, 0x40, 0x50,
  0x05, 0xba, 0x10, 0x00, 0x00, 0x03, 0x00, 0x10,
  0x00, 0x00, 0x03, 0x03, 0x20, 0xf1, 0x83, 0x19,
  0x60
};

int main()
{
  uint32_t len = sizeof(buff);

  GY::x264_SPS_t sps_value;
  
  x264_analysis_t::h264_decode_sps(buff, len, &sps_value);
  
  sps_value.display();
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
