```
ffplay相关操作:
1. 播放原始视频yuv数据，以1280 * 720的xxx.yuv为例子:
ffplay -f rawvideo -video_size 1280x720 xxx.yuv

2. 播放16k Hz单声道16bit的xxx.pcm的pcm文件为例子:
ffplay -ar 16000 -channels 1 -f s16le -i xxx.pcm

```

```
ffmpeg转码的相关操作:
参数介绍:
    1. -i  表示输入的音频或者视频地址。
    2. -ac 表示音频的通道数，双通道和单通道。
    3. -ar 音频采样率。
    4. -acodec 使用的音频编解码器。
    5. -ab 音频的码率。
    6. -y  覆盖输出文件。

1. mp4 to mp3
ffmpeg -i xxx.mp4 -acodec libmp3lame -ac 1 -ar 16000 xxx.mp3

2. mp4 to wav
ffmpeg -i xxx.mp4 -ac 1 -ar 16000 xxx.wav

3. mp3 to wav
ffmpeg -i xxx.mp3 xxx.wav

4. flac to wav
ffmpeg -i xxx.flac xxx.wav

5. aac to mp3
ffmpeg -i xxx.aac -acodec libmp3lame -ac 2 -ab 160 xxx.mp3

6. aac to wav
ffmpeg -i xxx.aac xxx.wav

7. wav to mp3
ffmpeg -i xxx.wav -vn -ar 44100 -ac 2 -ab 192k -f mp3 xxx.mp3

8. wav to flac
ffmpeg -i xxx.wav xxx.flac

9. wav to aac
ffmpeg -i xxx.wav -acodec libfaac xxx.aac

10. wav to ogg
ffmpeg -i xxx.wav -acodec libvorbis xxx.ogg

11. wav to pcm
ffmpeg -i xxx.wav -f s16le -ar 44100 -acodec pcm_s16le xxx.pcm

12. pcm to wav
ffmpeg -i xxx.pcm -f s16le -ar 44100 -ac 2 -acodec pcm_s16le xxx.wav
```