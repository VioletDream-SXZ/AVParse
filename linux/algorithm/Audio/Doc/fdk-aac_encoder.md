### fdk-aac部分编码结构体解析:
```cpp
struct AACENC_BufDesc {
  int  numBufs;            // number of bufs.
  void ** bufs;            // pointer of data.
  int * bufferIdentifiers; // 每一个缓冲区元素的标识。
  int * bufSizes;          // 每一个bufs的长度。(8-bit bytes)
  int * bufElSizes;        // 每一个bufs的长度。(bytes)
};

struct AACENC_InArgs {
  int numlnSamples;   // 有效的音频采样数量。  
  int numAncBytes;    // 附属数据长度。
};

struct AACENC_OutArgs {
  int numOutBytes;    // 编码后的数据的长度，可能为0。
  int numInSamples;   // 由编码器计算的音频采样数量。
  int numAncBytes;    // 附属数据长度。
};

struct AACENC_InfoStruct {
  uint maxOutBufBytes;  // 最大的输出数据长度。
  uint maxAncBytes;     // 最大的附属信息长度。
  uint inBufFillLevel;
  uint inputChannels;   // 需要编码音频的通道。
  uint frameLength;     // frameLength * channels * bits = 需要编码的pcm数据长度。
  uint nDelay;
  uint nDelayCore;
  char confBuf[64];
  uint confSize;
};
```

### fdk-aac编码函数解析:
```cpp
#include <aacenc_lib.h>

/**
 * @brief: 打开编码器
 * @param phAacEncoder: 打开编码器的标示符
 * @param encModules  : 对于这个编码器实例，指定其特定的编码模式。
                        0x00: 申请内存，对于所有可用的编码器模式。
                        0x01: AAC
                        0x02: SBR
                        0x04: PS
                        0x08: MPS
                        0x10: Metadata
                        example: 0x00 = (0x01 | 0x02 | 0x04 | 0x08 | 0x10)
 * @param maxChannels : 指定音频的通道，比如双通道则为2，0表示申请最大的AAC或者SBR通道数。
 * @return            : AACENC_OK, on success, else failute.
 */
AACENC_ERROR aacEncOpen ( HANDLE_AACENCODER ∗ phAacEncoder, const UINT encModules, const UINT maxChannels );

/**
 * @brief: 关闭开启的编码器
 * @param phAacEncoder: 指定已经开启的解码器指针。
 * @return            : AACENC_OK, on success, else failure.
 */ 
AACENC_ERROR aacEncClose ( HANDLE_AACENCODER ∗ phAacEncoder );

/**
 * @brief: 用于编码音频数据。
 * @param hAacEncoder : 一个有效的AAC编码器标示符。
 * @param inBufDesc   : 输入的数据。可以为NULL。
 * @param outBufDesc  : 输出的数据。可以为NULL。
 * @param inargs      : 输入数据的格式。可以为NULL。
 * @param outargs     : 输出数据的格式。可以为NULL。
 * @return            : AACENC_OK             : 成功。
                        AACENC_INVALID_HANDLE : 
                        AACENC_ENCODE_ERROR   : 编码过程失败。
                        AACENC_INVALID_CONFIG :
                        AACENC_INIT_ERROR     :
                        AACENC_INIT_AAC_ERROR :
                        AACENC_INIT_SBR_ERROR :
                        AACENC_INIT_TP_ERROR  :
                        AACENC_INIT_META_ERROR:
                        AACENC_INIT_MPS_ERROR : 解码器初始化失败。
                        AACENC_UNSUPPORTED_PARAMETER : 初始化数据缓冲区失败。
                        AACENC_ENCODE_EOF     : 数据已到达结尾。
 */
AACENC_ERROR aacEncEncode ( const HANDLE_AACENCODER hAacEncoder, const AACENC_BufDesc ∗ inBufDesc, const AACENC_BufDesc ∗ outBufDesc, const AACENC_InArgs ∗ inargs, AACENC_OutArgs ∗ outargs );

/**
 * @brief: 获取指定解码器的配置信息。
 * @param hAacEncoder : 指定有效解码器的标示符。
 * @param pInfo       : 需要获取的信息指针。
 * @return            : AACENC_OK, on sucess, else failure.
 */
AACENC_ERROR aacEncInfo ( const HANDLE_AACENCODER hAacEncoder, AACENC_InfoStruct ∗ pInfo );

/**
 * @brief: 设置编码器一个参数。
 * @param hAacEncoder
 * @param param
 * @param value
 */
AACENC_ERROR aacEncoder_SetParam ( const HANDLE_AACENCODER hAacEncoder, const AACENC_PARAM param, const UINT value );

/**
 * @beirf: 查询编码器的一个配置参数。
 * @param hAacEncoder: 需要查询的编码器的标示符。
 * @param param      : 需要查询的参数标示符。
 * @return           : 返回需要查询的参数值。
 */
UINT_aacEncoder GetParam ( const HANDLE_AACENCODER hAacEncoder, const AACENC_PARAM param );
```

### fdk-aac部分枚举类型意义:
```cpp
enum AACENC_PARAM;

/**
 * 2: MPEG-4 AAC LC低复杂度规格————现在的手机比较常见的MP4文件中的音频部分就包括了该规格的音频文件。
 * 129: MPEG-2 AAC LC低复杂度规格————比较简单，没有增益控制，但提高了编码效率，在中等码率以及音质方面，都能找到平衡点。
 */
AACENC_AOT: 2  : MPEG-4 AAC Low Complexity
            5  : MPEG-4 AAC Low Complexity with Spectral Band Replication (HE-AAC)
            29 : MPEG-4 AAC Low Complexity with Spectral Band Replication and Parametric Stereo (HE-AAC v2)
            23 : MPEG-4 AAC Low-Delay
            39 : MPEG-4 AAC Enhanced Low-Delay.
            129: MPEG-2 AAC Low Complexity
            132: MPEG-2 AAC Low Complexity with Spectral Band Replication (HE-AAC)

AACENC_BITRATE: CBR: Bitrate in bits/second.
                VBR: Variable bitrate.

AACENC_BITRATEMODE: 0: 常量比特率，使用AACENC_BITRATE指定的比特率。
                    1: 非常低比特率。
                    2: 低比特率。
                    3: 中等比特率。
                    4: 高比特率。
                    5: 非常高比特率。

AACENC_SAMPLERATE: 音频的采样率。
                   8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000, 64000, 88200, 96000.

AACENC_CHANNELMODE: 指定输出通道模式。

AACENC_TRANSMUX: 使用的转换类型。
                 0: 原生音频数据。
                 1: 不包含首部的aac数据。
                 2: 包含adts首部的aac音频数据。
                 6: 
                 7:
                 10:音频同步流。

enum AACENC_BufferIdenrtifier;

IN_AUDIO_DATA     : Audio输入缓冲队列，交错的pcm数据。
IN_ANCILLRY_DATA  : 附属数据插入都二进制流中。
IN_METADATA_SETUP : 输入为AACENC_MetaData结构体。
OUT_BITSTREAM_DATA: 输出为二进制流。
OUT_AU_SIZES      : 对于大的数据流而言，这个参数是必须的。


```