#ifndef CJR_AUDIO_TOOL_H
#define CJR_AUDIO_TOOL_H

#include <stdint.h>

uint32_t GET_UINT32_FROM_ARRAY(uint8_t*, int);
uint16_t GET_UINT16_FROM_ARRAY(uint8_t*, int);

const char* getProfile(const uint8_t& profile);
const char* getSamplingFrequency(const uint8_t& index);

#endif // CJR_AUDIO_TOOL_H