#ifndef GY_X264_STRUCTURE_H
#define GY_X264_STRUCTURE_H

#include "types.h"

namespace GY
{
  
typedef struct x264_SPS_t
{
  /**
   * It is equals zero.
   */
  uint8_t forbidden_zero_bit;
  
  // 指示当前NAL的优先级
  /**
   * 00,01,10,11
   */
  uint8_t nal_ref_idc;
  
  // 指示当前NAL的类型
  /**
   * =0: 未使用
   * =1: 不分区,非IDR图像的片
   * =2: 片分区A
   * =3: 片分区B
   * =4: 片分区C
   * =5: IDR图像的片
   * =6: 补充增强信息单元(SEI)
   * =7: 序列参数集(SPS)
   * =8: 图像参数集(PPS)
   * =9: 分界符
   * =10:序列结束
   * =11:码流结束
   * =12:填充
   */
  uint8_t nal_uint_type;
  
  // 指示当前H.264码流的profile。
  /**
   * =0x66(十进制的100): 基准档次
   * =0x77(十进制的119): 主要档次
   * =0x88(十进制的136): 扩展档次
   * =else             : 有待更新
   */
  uint8_t profile_idc;
  
  // 以下三个参数有两个以上等于1，都必须遵循A.2里面所有的制约。
  /**
   * =1: 必须遵从附录A.2.1所指明的所有制约条件。
   * =0: 不必遵循
   */
  uint8_t constraint_set0_flag;
  
  /**
   * =1: 必须遵从附录A.2.2所指明的所有制约条件。
   * =0: 不必遵循
   */
  uint8_t constraint_set1_flag;
  
  /**
   * =1: 必须遵从附录A.2.3所指明的所有制约条件。
   * =0: 不必遵循
   */
  uint8_t constraint_set2_flag;
  
  // 在目前的标准中本句法元素必须等于0，其他的值保留做将来用，解码器应该忽略本句法元素的值
  uint8_t reserved_zero_5bits;
  
  // 表示当前的码流Level。编码的Level精益了某种条件下的最大视频分辨率，最大视频帧率等参数。
  uint8_t level_idx;
  
  // 所引用的序列参数集的id号
  uint32_t seq_parameter_set_id;
  
  uint32_t chroma_format_idc;
  uint32_t residual_colour_transform_flag;
  uint32_t bit_depth_luma_minus8;
  uint32_t bit_depth_chroma_minus8;
  uint8_t  qpprime_y_zero_transform_bypass_flag;
  uint8_t  seq_scaling_matrix_present_flag;
  uint8_t  seq_scaling_list_present_flag[8];
  
  // 
  uint32_t log2_max_frame_num_minus4;
  uint32_t pic_order_cnt_type;
  uint32_t log2_max_pic_order_cnt_lsb_minus4;
  uint8_t delta_pic_order_always_zero_flag;
  int32_t offset_for_non_ref_pic;
  int32_t offset_for_top_to_bottom_field;
  uint32_t num_ref_frames_int_pic_order_cnt_cycle;
  int32_t offset_for_ref_frame[30];
  
  uint32_t num_ref_frames;
  uint8_t gaps_in_frame_num_value_allowed_flag;
  
  // 本句法元素+1后表明图像宽度，以宏块为单位：
  //     PicWidthInMbs = pic_width_in_mbs_minus1 + 1
  // 通过这个元素解码器可以计算得到亮度分量以像素为单位的图像宽度：
  //     PicWidthInSamplesL = PicWidthInMbs * 16
  // 从而也得到色度分量以像素为单位的图像宽度：
  //     PicWidthInSamplesC = PicWidthInMbs * 8
  uint32_t pic_width_in_mbs_minus1;
  
  // 本句法元素+1后表明图像高度，以宏块为单位：
  //     PicHeightInMbs = pic_height_in_mbs_minus1 + 1
  //     PiCSizeInMapUnits = PicHeightInMbs * PicWidthInMbs
  uint32_t pic_height_in_mbs_minus1;
  uint8_t frame_mbs_only_flag;
  uint8_t mb_adaptive_frame_field_flag;
  uint8_t direct_8x8_inference_flag;
  uint8_t frame_cropping_flag;
  
  uint32_t frame_crop_left_offset;
  uint32_t frame_crop_right_offset;
  uint32_t frame_crop_top_offset;
  uint32_t frame_crop_bottom_offset;
  
  uint8_t vui_parameters_present_flag;

  uint8_t aspect_ratio_info_present_flag;
  uint8_t aspect_ratio_idc;
  uint16_t sar_width;
  uint16_t sar_height;
  uint8_t overscan_info_present_flag;
  uint8_t overscan_appropriate_flagu;
  uint8_t video_signal_type_present_flag;
  uint8_t video_format;
  uint8_t video_full_range_flag;
  uint8_t colour_description_present_flag;
  uint8_t colour_primaries;
  uint8_t transfer_characteristics;
  uint8_t matrix_coefficients;
  uint8_t chroma_loc_info_present_flag;
  uint8_t chroma_sample_loc_type_top_field;
  uint8_t chroma_sample_loc_type_bottom_field;
  uint8_t timing_info_present_flag;
  uint32_t num_units_in_tick;
  uint32_t time_scale;
  uint8_t fixed_frame_rate_flag;

  /**
   * 获取图像的Width，比如图像为1280*720，则会输出1280.
   * @return the width of picture.
   */
  int getWidth();
  
  /**
   * 获取图像的Height，比如图像为1280*720，则会输出720.
   * @return the height of picture.
   */
  int getHeight();
  
  /**
   * 获取该H.264的帧率。
   * @return the fps of video 
   */
  int getFps();
  
  /**
   * 获取frame_num所能达到的最大值。
   * @return the MaxFrameNumber of H.264 video
   */
  int getMaxFrameNum();
  
  int getNumberRefFrames();

  void display();
}x264_SPS_t;

}

#endif // GY_X264_STRUCTURE_H