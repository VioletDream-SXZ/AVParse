#ifndef CJR_AUDIO_TOOL_H
#define CJR_AUDIO_TOOL_H

#include <stdint.h>

uint32_t GET_UINT32_FROM_ARRAY(uint8_t*, int);
uint16_t GET_UINT16_FROM_ARRAY(uint8_t*, int);

const char* getProfile(const uint8_t& profile);
const char* getSamplingFrequency(const uint8_t& index);

void getAdtsHdr(char * data, int sampleRate, int nChannelsOut);
void getAdtsHdrUp(char * data, int size);

int getFrameLength(const unsigned char * audioConfigure);

void getAudioSpecificConfig(char * asc, int sampleRate, int chOut, int aacProfile);

#endif // CJR_AUDIO_TOOL_H