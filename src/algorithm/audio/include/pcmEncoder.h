#ifndef CJR_PCM_ENCODER_H
#define CJR_PCM_ENCODER_H

/**
 * @brief 通过传入的inFile和outFile文件地址，对inFile pcm文件进行解码。然后将结果保存到outFile文件中。
 * @param inFile : pcm文件的绝对或者相对地址。
 * @param outFile: 输出的aac文件的绝对地址或相对地址。
 * @return       : -1 error
                    0 sucess
 */
int pcmEncoder(const char* inFile, const char* outFile);

// 调用pcmEncoder的测试函数。
void pcmEncoderExample(const char* inFile, const char* outFile);

#endif