#include "pch.h"
#include "analysis.h"
#include "x264_structure.h"

uint32_t x264_analysis_t::Ue(uint8_t * pBuff, uint32_t nLen, uint32_t & nStartBit)
{
  uint32_t nZeroNum = 0;
  while (nStartBit < nLen * 8)
  {
    if(pBuff[nStartBit / 8] & (0x80 >> (nStartBit % 8)))
    {
       break;
    }

    nZeroNum ++;
    nStartBit ++;
  }

  nStartBit++;

  uint64_t dwRet = 0;
  for(uint32_t i=0; i<nZeroNum; ++i)
  {
    dwRet <<= 1;
    if(pBuff[nStartBit / 8] & (0x80 >> (nStartBit % 8)))
    {
      dwRet += 1;
    }
    nStartBit ++;
  }

  return (1 << nZeroNum) - 1 + dwRet;
}

int x264_analysis_t::Se(uint8_t * pBuff, uint32_t nLen, uint32_t & nStartBit)
{
  int UeVal = Ue(pBuff, nLen,nStartBit);
  double k  = UeVal;
  
  int nValue = ceil(k / 2);
  if(UeVal % 2 == 0)
    nValue = -nValue;
  
  return nValue;
}

uint64_t x264_analysis_t::U(uint8_t * pBuff, uint32_t BitCount, uint32_t & nStartBit)
{
  uint64_t dwRet = 0;
  for(uint32_t i=0; i<BitCount; ++i)
  {
    dwRet <<= 1;
    if(pBuff[nStartBit / 8] & (0x80 >> (nStartBit % 8)))
    {
      dwRet += 1;
    }
    nStartBit++;
  }
  
  return dwRet;
}

void x264_analysis_t::de_emulation_prevention(uint8_t * buf, uint32_t * buf_size)
{
  int i=0, j=0;
  uint8_t* tmp_ptr = NULL;
  unsigned int tmp_buf_size = 0;
  int val = 0;
  
  tmp_ptr = buf;
  tmp_buf_size = *buf_size;
  for(i=0; i<(tmp_buf_size-2); ++i)
  {
    val = (tmp_ptr[i] ^ 0x00) + (tmp_ptr[i+1] ^ 0x00) + (tmp_ptr[i+2] ^ 0x03);
    if(val == 0)
    {
      for(j=i+2; j<tmp_buf_size-1; ++j)
        tmp_ptr[j] = tmp_ptr[j+1];
      
      (*buf_size)--;
    }
  }
}

bool x264_analysis_t::h264_decode_sps(uint8_t * buf, uint32_t nLen, GY::x264_SPS_t * sps_data)
{
  uint32_t startBit = 0;
  de_emulation_prevention(buf, &nLen);
  
  sps_data->forbidden_zero_bit = U(buf, 1, startBit);
  sps_data->nal_ref_idc        = U(buf, 2, startBit);
  sps_data->nal_uint_type      = U(buf, 5, startBit);
  
  if(sps_data->nal_uint_type == 7)
  {
    sps_data->profile_idc          = U(buf, 8, startBit);
    sps_data->constraint_set0_flag = U(buf, 1, startBit);
    sps_data->constraint_set1_flag = U(buf, 1, startBit);
    sps_data->constraint_set2_flag = U(buf, 1, startBit);
    sps_data->reserved_zero_5bits  = U(buf, 5, startBit);
    sps_data->level_idx            = U(buf, 8, startBit);
    
    sps_data->seq_parameter_set_id      = Ue(buf, nLen, startBit);
    
    if(sps_data->profile_idc == 100 || sps_data->profile_idc == 110 ||
       sps_data->profile_idc == 122 || sps_data->profile_idc == 244 ||
       sps_data->profile_idc == 44  || sps_data->profile_idc == 83  ||
       sps_data->profile_idc == 86  || sps_data->profile_idc == 118 ||  
       sps_data->profile_idc ==128)
    {
      sps_data->chroma_format_idc=Ue(buf, nLen, startBit);
      if( sps_data->chroma_format_idc == 3 )
        sps_data->residual_colour_transform_flag=U(buf, 1, startBit);
      sps_data->bit_depth_luma_minus8                = Ue(buf,nLen,startBit);
      sps_data->bit_depth_chroma_minus8              = Ue(buf,nLen,startBit);
      sps_data->qpprime_y_zero_transform_bypass_flag = U(buf, 1, startBit);
      sps_data->seq_scaling_matrix_present_flag      = U(buf, 1,startBit);
      if(sps_data->seq_scaling_matrix_present_flag)
      {
        for(int i=0; i<8; i++ )
        {
          sps_data->seq_scaling_list_present_flag[i] = U(buf, 1, startBit);
        }
      }
    }
    
    sps_data->log2_max_frame_num_minus4 = Ue(buf, nLen, startBit);
    sps_data->pic_order_cnt_type        = Ue(buf, nLen, startBit);
    
    if(sps_data->pic_order_cnt_type == 0)
      sps_data->log2_max_pic_order_cnt_lsb_minus4 = Ue(buf, nLen, startBit);
    else if(sps_data->pic_order_cnt_type == 1)
    {
      sps_data->delta_pic_order_always_zero_flag = U(buf, 1, startBit);
      sps_data->offset_for_non_ref_pic           = Se(buf, nLen, startBit);
      sps_data->offset_for_top_to_bottom_field   = Se(buf, nLen, startBit);
      sps_data->num_ref_frames_int_pic_order_cnt_cycle = Ue(buf, nLen, startBit);
      for(int i=0; i<sps_data->num_ref_frames_int_pic_order_cnt_cycle; ++i)
        sps_data->offset_for_ref_frame[i] = Se(buf, nLen, startBit);
    }
    
    sps_data->num_ref_frames                       = Ue(buf, nLen, startBit);
    sps_data->gaps_in_frame_num_value_allowed_flag = U(buf, 1, startBit);
    sps_data->pic_width_in_mbs_minus1              = Ue(buf, nLen, startBit);
    sps_data->pic_height_in_mbs_minus1             = Ue(buf, nLen, startBit);
    sps_data->frame_mbs_only_flag                  = U(buf, 1, startBit);
    
    if(!sps_data->frame_mbs_only_flag)
      sps_data->mb_adaptive_frame_field_flag = U(buf, 1, startBit);
    sps_data->direct_8x8_inference_flag = U(buf, 1, startBit);
    sps_data->frame_cropping_flag       = U(buf, 1, startBit);
    
    if(sps_data->frame_cropping_flag)
    {
      sps_data->frame_crop_left_offset  = Ue(buf, nLen, startBit);
      sps_data->frame_crop_right_offset = Ue(buf, nLen, startBit);
      sps_data->frame_crop_top_offset   = Ue(buf, nLen, startBit);
      sps_data->frame_crop_bottom_offset = Ue(buf, nLen, startBit);
    }
    sps_data->vui_parameters_present_flag = U(buf, 1, startBit);
    
    if(sps_data->vui_parameters_present_flag)
    {
      sps_data->aspect_ratio_info_present_flag = U(buf, 1, startBit);
      if(sps_data->aspect_ratio_info_present_flag)
      {
        sps_data->aspect_ratio_idc = U(buf, 8, startBit);
        if(sps_data->aspect_ratio_idc == 255)
        {
          sps_data->sar_width  = U(buf, 16, startBit);
          sps_data->sar_height = U(buf, 16, startBit);
        }
      }
      
      sps_data->overscan_info_present_flag = U(buf, 1,startBit);
      if(sps_data->overscan_info_present_flag)
        sps_data->overscan_appropriate_flagu = U(buf, 1, startBit);
      
      sps_data->video_signal_type_present_flag = U(buf, 1, startBit);
      if (sps_data->video_signal_type_present_flag)
      {
        sps_data->video_format = U(buf, 3, startBit);
        sps_data->video_full_range_flag = U(buf, 1, startBit);
        sps_data->colour_description_present_flag = U(buf, 1, startBit);
        if (sps_data->colour_description_present_flag)
        {
          sps_data->colour_primaries=U(buf, 8, startBit);
          sps_data->transfer_characteristics=U(buf, 8, startBit);
          sps_data->matrix_coefficients=U(buf, 8, startBit);
        }
      }
      
      sps_data->chroma_loc_info_present_flag = U(buf, 1, startBit);
      if(sps_data->chroma_loc_info_present_flag)
      {
        sps_data->chroma_sample_loc_type_top_field    = Ue(buf, nLen, startBit);
        sps_data->chroma_sample_loc_type_bottom_field = Ue(buf, nLen, startBit);
      }
      
      sps_data->timing_info_present_flag = U(buf, 1, startBit);
      if(sps_data->timing_info_present_flag)
      {
        sps_data->num_units_in_tick = U(buf, 32, startBit);
        sps_data->time_scale = U(buf, 32, startBit);
        sps_data->fixed_frame_rate_flag = U(buf, 1, startBit);
      }
    }

    return true;
  }
  else
  {
    return false;
  }
}

void x264_analysis_t::displayUint(uint8_t _val)
{
  printf("%d ", (unsigned int)_val);
}

void x264_analysis_t::displayUint(uint32_t _val)
{
  printf("%d ", (unsigned int)_val);
}

void x264_analysis_t::displayUint(uint64_t _val)
{
  printf("%lld ", (unsigned long long)_val);
}
