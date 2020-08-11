#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../logger/logger.h"
#include "../algorithm/video/hevc_ps.h"
#include "../common/getbitcontext.h"

int main(int argc, char** argv)
{
    uint8_t sps[] = { 
                    0x00, 0x00, 0x00, 0x01, 
                    0x42, 0x01, 0x02, 0x01, 
                    0x60, 0x00, 0x00, 0x03, 
                    0x00, 0x00, 0x03, 0x00,
                    0x00, 0x03, 0x00, 0x00,
                    0x03, 0x00, 0x96, 0x00,
                    0x00, 0xa0, 0x01, 0x40,
                    0x20, 0x06, 0x41, 0xfe,
                    0x59, 0x5c, 0xa4, 0xa7,
                    0x23, 0x6C, 0x34, 0xB1,
                    0x15, 0x52, 0x62, 0x61,
                    0x97, 0xD9, 0xF5, 0xE6,
                    0xFC, 0xB3, 0xEB, 0xC2,
                    0x88, 0xC4, 0xE5, 0xDB,
                    0x20
    };
    const uint32_t sps_size = sizeof(sps) / sizeof(uint8_t);
    unsigned int sps_id = 0;
    GYLOGI("sps:%p, sps_size:%d", sps, sps_size);

    HEVCSPS* hevcSps = (HEVCSPS*)malloc(sizeof(HEVCSPS));
    ff_hevc_parse_sps(hevcSps, sps, sps_size, &sps_id, 1);

    GYLOGI("sps width:%d height:%d", hevcSps->width, hevcSps->height)

    return 0;
}
