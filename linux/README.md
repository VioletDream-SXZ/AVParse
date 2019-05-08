### Linux视频编解码的实现
目前已有功能：    
　　1.  DCT变换，Hadamard变换。    
　　2.  4x4量化和反量化。    
　　3.  转换图像的分辨率。 

### Linux音频格式解析器和音频编解码器
1. aac文件解析，使用命令如下:（将解析结果保存到xxx.txt中）
./main.exe aacParseExample xxx.aac >> xxx.txt

2. wav文件解析，使用命令如下:
./main.exe wavParseExample xxx.wav

3. ogg文件解析，使用命令如下:
./main.exe oggParseExample xxx.ogg >> xxx.txt

4. aac(含有adts首部信息的音频数据)转化成pcm:
./main.exe aacAdtsDecoderExample xxx.aac xxx.pcm

5. pcm文件转化成aac(含有adts首部信息的音频文件):
./main.exe pcmEncoderExample xxx.pcm xxx.aac