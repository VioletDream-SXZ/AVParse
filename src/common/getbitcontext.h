/*
 * Copyright (c) 2004 Michael Niedermayer <michaelni@gmx.at>
 * Copyright (c) 2016 Alexandra Hájková
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file
 * bitstream reader API header.
 */

#ifndef GYJR_MKF_GET_BITS_H
#define GYJR_MKF_GET_BITS_H

#include <stdint.h>
#include <limits.h>

#define GYJRMAX(a, b) ((a) > (b) ? (a) : (b))

#define GYJRMIN(a, b) ((a) > (b) ? (b) : (a))

typedef struct GetBitContext 
{
    const uint8_t *buffer, *buffer_end;
    int index;
    int size_in_bits;
    int size_in_bits_plus8;
} GetBitContext;

int init_get_bits(GetBitContext *s, const uint8_t *buffer, int bit_size);

int init_get_bits_xe(GetBitContext *s, const uint8_t *buffer, int bit_size, int is_le);

uint8_t get_bit(GetBitContext* s);

unsigned int show_bits(GetBitContext *s, int n);

uint32_t get_bits(GetBitContext *s, int n);

int get_bits_left(GetBitContext *gb);

int get_bits_count(const GetBitContext *s);

unsigned get_ue_golomb_long(GetBitContext *gb);

int get_se_golomb(GetBitContext *gb);

void skip_bits(GetBitContext *s, int n);

#endif // GYJR_MKF_GET_BITS_H
