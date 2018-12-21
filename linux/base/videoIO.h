#ifndef GY_VIDEO_IO_H
#define GY_VIDEO_IO_H

namespace GY
{

/**
 * 转换YUV420的分辨率。
 * @param rawFile    :源yuv视频地址。
 * @param saveFile   :目标文件地址。
 * @param width      :
 * @param height     :源yuv视频分辨率
 * @param save_width :
 * @param save_height:目标yuv视频分辨率
 * @param frameLen   :需要保存的帧数
 * @return =1        :SUCCESS
           =-1       :FAILED (文件不存在，新的分辨率比原来的大，帧数不足等问题) 
 */
int YUV420ToWidth_Height(const char* rawFile, const char* saveFile, int width, int height, int save_width, int save_height, int frameLen);

}
#endif // GY_VIDEO_IO_H