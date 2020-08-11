### fdk-aac部分解码结构体解析:

### fdk-aac解码函数解析:
```cpp
/**
 * @brief: 创建解码器的额外数组。
 * @param self   : 需要初始化的解码器标示符。
 * @param buffer : 需要传入的附加数据数组。
 * @param size   : 传入的数组的大小。
 * @return       : error code.
 */
AAC_DECODER_ERROR aacDecoder_AncDataInit ( HANDLE_AACDECODER self, UCHAR ∗ buffer, int size );

/**
 * @brief: 获取一个附加数据元素
 * @param self  : 解码器的标示符。
 * @param index : 需要获取的附加数据的index。
 * @param ptr   : 存放获取到的附加数据。
 * @param size  : 存放获取到的附加数据的长度数组。
 * @return      : error code.
 */
AAC_DECODER_ERROR aacDecoder_AncDataGet ( HANDLE_AACDECODER self, int index, UCHAR ∗∗ ptr, int ∗ size );

/**
 * @brief: 设置解码器一个参数的值。
 * @param self  : 解码器的标示符。
 * @param param : 需要设置的参数enum。
 * @param value : 需要设置的参数值。
 * @return      : error code.
 */
AAC_DECODER_ERROR aacDecoder_SetParam ( const HANDLE_AACDECODER self, const AACDEC_PARAM param, const INT value );

/**
 * @brief: 获取解码器内部缓冲区剩余的长度。
 * @param self       : 解码器的标示符。
 * @param pFreeBytes : 存放长度的指针。
 * @return           : error code.
 */
AAC_DECODER_ERROR aacDecoder_GetFreeBytes ( const HANDLE_AACDECODER self, UINT ∗ pFreeBytes );

/**
 * @brief: 创建一个解码器
 * @param transportFmt : 指定需要解码的音频格式。
 * @param nrOfLayers   : 默认为1
 * @return             : 返回一个音频解码器的标示符。
 */
HANDLE_AACDECODER aacDecoder_Open( TRANSPORT_TYPE transportFmt, UINT nrOfLayers );

/**
 * @brief: 关闭解码器
 * @param self: 需要关闭的解码器
 * @return no
 */
void aacDecoder_Close ( HANDLE_AACDECODER self );

/**
 * @brief: 填充解码器内部的数据通过使用外部的缓冲区。
 * @param self       : 解码器的标示符
 * @param pBuffer    : 传入的数据指针的数组。
 * @param bufferSize : 传入的数据指针的大小的数组。
 * @param bytesValid : 需要复制进去的所有数据的长度。
 * @return           : Error Code.
 */
AAC_DECODER_ERROR aacDecoder_Fill ( HANDLE_AACDECODER self, UCHAR ∗ pBuffer[], const UINT bufferSize[], UINT ∗ bytesValid );

/**
 * @brief: 解码一帧音频数据。
 * @param self         : 解码器标示符。
 * @param pTimeData    : 解码完的PCM数据所存放的数组指针。
 * @param timeDataSize : 存放PCM数据数组的长度。
 * @param flags        : (flags & AACDEC_CONCEAL) == 1: 不隐藏???
                         (flags & AACDEC_FLUSH)   == 2: 忽略输出数据。
                         (flags & AACDEC_INTR)    == 4: 输入数据是不连续的，必须同步内部的信息。
 * @return             : 0, sucess, else failure.
 */
AAC_DECODER_ERROR aacDecoder_DecodeFrame ( HANDLE_AACDECODER self, INT_PCM ∗ pTimeData, const INT timeDataSize, const UINT flags );

/**
 * @brief: 配置解码器的AudioSpeciﬁcConﬁg或者StreamMuxConfig(SMC)。当传进来的aac音频信息没有ADTS或者其它首部只有原音频数据时候，就需要配置该信息。
 * @param self   : 解码器标示符。
 * @param conf   : 配置信息的字符串指针。
 * @param length : 配置信息的长度。
 */
AAC_DECODER_ERROR aacDecoder_ConfigRaw ( HANDLE_AACDECODER self, UCHAR ∗ conf[], const UINT length[] )

/**
 * @brief: 获取解码器流的信息。
 * @param self : 解码器的标示符。
 * @return     : 获取到的流信息的指针。
 */
CStreamInfo∗ aacDecoder_GetStreamInfo ( HANDLE_AACDECODER self )

/**
 * @brief: 获取解码器lib信息。
 * @param info : 一个lib信息存放的指针。
 * @return     : 0 on success.
 */
INT aacDecoder_GetLibInfo ( LIB_INFO ∗ info )
```

### fdk-aac部分枚举类型意义:
```cpp
typedef enum
{
  TT_UNKNOWN       = -1, // 未知类型
  TT_MP4_RAW       = 0,  // 原生的音频格式，参数格式由解码器一开始就设置。
  TT_MP4_ADIF      = 1,  // 带有ADIF首部的音频数据。
  TT_MP4_ADTS      = 2,  // 带有ADTS首部的音频数据。

  TT_MP4_LATM_MCP1 = 6,
  TT_MP4_LATM_MCP0 = 7,

  TT_MP4_LOAS      = 10,
  TT_DRM           = 12,
}TRANSPORT_TYPE;

typedef enum
{
  // 指定解码进程要解码哪一个声道。
  // 0. 接完的码和原来一样。
  // 1. 使用第一通道解码。
  // 2. 使用第二通道解码。
  // 3. 第一二通道混音编码。
  AAC_PCM_DUAL_CHANNEL_OUTPUT_MODE,

  // 最小的pcm输出通道。
  // 参数值简述:
  // 1 : 
  // 2 : 如果原数据是单声道的，会将原来的数据复制一份到双声道上。
  // 6 : 3/0/2.1
  // 8 : 3/0/4.1
  // 注意事项:
  // 4. 如果这个通道数量设置超过8，则会直接被解码器忽略。
  AAC_PCM_MIN_OUTPUT_CHANNELS,

  // 最大的pcm输出通道
  // 参数值简述:
  // 1 : 当原文件的声道数超过1时，会将所有的声道信息混合到单声道上。
  // 2 : 当原文件的声道数超过2时，会将所有的声道信息混合到双声道上。
  // 6 : 3/0/2.1
  // 8 : 需要软件支持。
  // 注意事项:
  // 1. 
  // 2. 如果这个数值大于0小于AAC_PCM_MIN_OUTPUT_CHANNELS的值，则其值和AAC_PCM_MIN_OUTPUT_CHANNELS一样。
  // 3. MEPG环绕模式的工作程序也会额外的被设置。
  // 4. 某些值会使得立体音模式不能够被使用。
  // 5. 如果该参数超过8就会被忽略。
  AAC_PCM_MAX_OUTPUT_CHANNELS,
}AACDEC_PARAM;
```