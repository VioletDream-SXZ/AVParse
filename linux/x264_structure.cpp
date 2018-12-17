#include "x264_structure.h"

int GY::x264_SPS_t::getWidth()
{
  int mWidth = pic_width_in_mbs_minus1 + 1;
  mWidth = mWidth << 4;
  
  return mWidth;
}

int GY::x264_SPS_t::getHeight()
{
  int mHeight = pic_height_in_mbs_minus1 + 1;
  mHeight = mHeight << 4;
  
  return mHeight;
}

int GY::x264_SPS_t::getFps()
{
  int mFps = time_scale / num_units_in_tick;
  if(fixed_frame_rate_flag)
    mFps = mFps / 2;
  return mFps;
}

int GY::x264_SPS_t::getMaxFrameNum()
{
  int MaxFrameNum = (log2_max_frame_num_minus4 + 4);
  MaxFrameNum = std::pow(2, MaxFrameNum);

  return MaxFrameNum;
}

int GY::x264_SPS_t::getNumberRefFrames()
{
	return num_ref_frames;
}
