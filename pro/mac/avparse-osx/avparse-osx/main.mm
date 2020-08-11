//
//  main.m
//  avparse-osx
//
//  Created by 杨时权 on 2020/8/11.
//  Copyright © 2020 杨时权. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "hevc_ps.h"
#import "getbitcontext.h"
#import "logger.h"

int main(int argc, const char * argv[])
{
    @autoreleasepool
    {
        // insert code here...
        NSLog(@"Hello, World!");
    }

    HEVCSPS* hevcSps = (HEVCSPS*)malloc(sizeof(HEVCSPS));
    uint8_t sps[] = {
                    0x42, 0x01, 0x02, 0x01,
                    0x60, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x96, 0x00,
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
    unsigned int sps_id;
    
    ff_hevc_parse_sps(hevcSps, sps, sps_size, &sps_id, 1);
    GYLOGI("sps parse finish width:%d height:%d", hevcSps->width, hevcSps->height);
    
    return 0;
}
