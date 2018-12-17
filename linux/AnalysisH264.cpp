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
  
  cout << sps_value.getWidth() << " " << sps_value.getHeight() << endl;
  cout << sps_value.getFps() << endl;
  cout << sps_value.getMaxFrameNum() << endl;
  cout << sps_value.getNumberRefFrames() << endl;
}