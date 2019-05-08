# H.264AnalysisProject

2018/12/17  20:24
  1. 添加SPS结构体函数，包含解析SPS所需要的所有参数
  2. 添加解析SPS的函数，包含于x264_analysis_t这个结构体中。
  3. 添加Ue,Se,U解码函数，包含于x264_analysis_t这个结构体中。
  
2018/12/21  14:37
  1. 添加YUV420转换分辨率函数，位于videoIO.h中。
  
2019/1/2    11:06  
　修改原本的结构布局，文件夹algorithm包含Hadamard编码和DCT编码，common包含基本的数据结构，example包含对以上三个模块的测试。
 
2019/5/6    xx:xx  
　添加wav音频文件解析和ogg音频格式解析。
  
2019/5/7    xx:xx  
　添加aacAdts解码器和pcm编码器。
