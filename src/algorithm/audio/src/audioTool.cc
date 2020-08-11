#include "audioTool.h"

uint32_t GET_UINT32_FROM_ARRAY(uint8_t* data, int index)
{
  uint32_t number = (data[index]          ) + (data[index + 1] << 8) +
                    (data[index + 2] << 16) + (data[index + 3] << 24);
  return number;
}

uint16_t GET_UINT16_FROM_ARRAY(uint8_t* data, int index)
{
  uint16_t number = data[index] + (data[index + 1] << 8);
  return number;
}

const char* getProfile(const uint8_t& profile)
{
  switch(profile)
  {
    case 0:
      return "Main profile";
    case 1:
      return "Low Complexity profile";
    case 2:
      return "Scalable Sampling Rate profile";
    case 3:
      return "reserved";
    default:
      return "unkonwn";
  }
}

const char* getSamplingFrequency(const uint8_t& index)
{
  switch(index)
  {
  case 0:
    return "96000Hz";
  case 1:
    return "88200Hz";
  case 2:
    return "64000Hz";
  case 3:
    return "48000Hz";
  case 4:
    return "44100Hz";
  case 5:
    return "32000Hz";
  case 6:
    return "24000Hz";
  case 7:
    return "22050Hz";
  case 8:
    return "16000Hz";
  case 9:
    return "12000Hz";
  case 10:
    return "11025Hz";
  case 11:
    return "8000Hz";
  case 12:
    return "7350Hz";
  case 13:
    return "Reserved";
  default:
    return "unkonwn";
  }
}