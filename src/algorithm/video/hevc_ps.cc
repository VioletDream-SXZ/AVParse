#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hevc_ps.h"
#include "../../common/getbitcontext.h"

#define YSQ_JR_GIT 1
#if YSQ_JR_GIT
#include "../../logger/logger.h"
#endif

#define FF_ARRAY_ELEMS(a) (sizeof(a) / sizeof((a)[0]))

const uint8_t ff_hevc_diag_scan4x4_x[16] = {
    0, 0, 1, 0,
    1, 2, 0, 1,
    2, 3, 1, 2,
    3, 2, 3, 3,
};

const uint8_t ff_hevc_diag_scan4x4_y[16] = {
    0, 1, 0, 2,
    1, 0, 3, 2,
    1, 0, 3, 2,
    1, 3, 2, 3,
};

const uint8_t ff_hevc_diag_scan8x8_x[64] = {
    0, 0, 1, 0,
    1, 2, 0, 1,
    2, 3, 0, 1,
    2, 3, 4, 0,
    1, 2, 3, 4,
    5, 0, 1, 2,
    3, 4, 5, 6,
    0, 1, 2, 3,
    4, 5, 6, 7,
    1, 2, 3, 4,
    5, 6, 7, 2,
    3, 4, 5, 6,
    7, 3, 4, 5,
    6, 7, 4, 5,
    6, 7, 5, 6,
    7, 6, 7, 7,
};

const uint8_t ff_hevc_diag_scan8x8_y[64] = {
    0, 1, 0, 2,
    1, 0, 3, 2,
    1, 0, 4, 3,
    2, 1, 0, 5,
    4, 3, 2, 1,
    0, 6, 5, 4,
    3, 2, 1, 0,
    7, 6, 5, 4,
    3, 2, 1, 0,
    7, 6, 5, 4,
    3, 2, 1, 7,
    6, 5, 4, 3,
    2, 7, 6, 5,
    4, 3, 7, 6,
    5, 4, 7, 6,
    5, 7, 6, 7,
};

static const uint8_t default_scaling_list_intra[] = {
    16, 16, 16, 16, 17, 18, 21, 24,
    16, 16, 16, 16, 17, 19, 22, 25,
    16, 16, 17, 18, 20, 22, 25, 29,
    16, 16, 18, 21, 24, 27, 31, 36,
    17, 17, 20, 24, 30, 35, 41, 47,
    18, 19, 22, 27, 35, 44, 54, 65,
    21, 22, 25, 31, 41, 54, 70, 88,
    24, 25, 29, 36, 47, 65, 88, 115
};

static const uint8_t default_scaling_list_inter[] = {
    16, 16, 16, 16, 17, 18, 20, 24,
    16, 16, 16, 17, 18, 20, 24, 25,
    16, 16, 17, 18, 20, 24, 25, 28,
    16, 17, 18, 20, 24, 25, 28, 33,
    17, 18, 20, 24, 25, 28, 33, 41,
    18, 20, 24, 25, 28, 33, 41, 54,
    20, 24, 25, 28, 33, 41, 54, 71,
    24, 25, 28, 33, 41, 54, 71, 91
};

static const AVRational vui_sar[] = {
    {  0,   1 },
    {  1,   1 },
    { 12,  11 },
    { 10,  11 },
    { 16,  11 },
    { 40,  33 },
    { 24,  11 },
    { 20,  11 },
    { 32,  11 },
    { 80,  33 },
    { 18,  11 },
    { 15,  11 },
    { 64,  33 },
    { 160, 99 },
    {  4,   3 },
    {  3,   2 },
    {  2,   1 },
};

static const uint8_t hevc_sub_width_c[] = {
    1, 2, 2, 1
};

static const uint8_t hevc_sub_height_c[] = {
    1, 2, 1, 1
};

static inline unsigned gy_mod_uintp2_c(unsigned a, unsigned p)
{
    return a & ((1U << p) - 1);
}

static void set_default_scaling_list_data(ScalingList *sl)
{
    int matrixId;

    for (matrixId = 0; matrixId < 6; matrixId++) {
        // 4x4 default is 16
        memset(sl->sl[0][matrixId], 16, 16);
        sl->sl_dc[0][matrixId] = 16; // default for 16x16
        sl->sl_dc[1][matrixId] = 16; // default for 32x32
    }
    memcpy(sl->sl[1][0], default_scaling_list_intra, 64);
    memcpy(sl->sl[1][1], default_scaling_list_intra, 64);
    memcpy(sl->sl[1][2], default_scaling_list_intra, 64);
    memcpy(sl->sl[1][3], default_scaling_list_inter, 64);
    memcpy(sl->sl[1][4], default_scaling_list_inter, 64);
    memcpy(sl->sl[1][5], default_scaling_list_inter, 64);
    memcpy(sl->sl[2][0], default_scaling_list_intra, 64);
    memcpy(sl->sl[2][1], default_scaling_list_intra, 64);
    memcpy(sl->sl[2][2], default_scaling_list_intra, 64);
    memcpy(sl->sl[2][3], default_scaling_list_inter, 64);
    memcpy(sl->sl[2][4], default_scaling_list_inter, 64);
    memcpy(sl->sl[2][5], default_scaling_list_inter, 64);
    memcpy(sl->sl[3][0], default_scaling_list_intra, 64);
    memcpy(sl->sl[3][1], default_scaling_list_intra, 64);
    memcpy(sl->sl[3][2], default_scaling_list_intra, 64);
    memcpy(sl->sl[3][3], default_scaling_list_inter, 64);
    memcpy(sl->sl[3][4], default_scaling_list_inter, 64);
    memcpy(sl->sl[3][5], default_scaling_list_inter, 64);
}

static void decode_sublayer_hrd(GetBitContext *gb, unsigned int nb_cpb,
                                int subpic_params_present)
{
    int i;

    for (i = 0; i < nb_cpb; i++) {
        get_ue_golomb_long(gb); // bit_rate_value_minus1
        get_ue_golomb_long(gb); // cpb_size_value_minus1

        if (subpic_params_present) {
            get_ue_golomb_long(gb); // cpb_size_du_value_minus1
            get_ue_golomb_long(gb); // bit_rate_du_value_minus1
        }
        skip_bits(gb, 1); // cbr_flag
    }
}

int ff_hevc_decode_short_term_rps(GetBitContext *gb, ShortTermRPS *rps, const HEVCSPS *sps, int is_slice_header)
{
    uint8_t rps_predict = 0;
    int delta_poc;
    int k0 = 0;
    int k1 = 0;
    int k  = 0;
    int i;

    if (rps != sps->st_rps && sps->nb_st_rps)
        rps_predict = get_bit(gb);

    if (rps_predict) {
        const ShortTermRPS *rps_ridx;
        int delta_rps;
        unsigned abs_delta_rps;
        uint8_t use_delta_flag = 0;
        uint8_t delta_rps_sign;

        if (is_slice_header) {
            unsigned int delta_idx = get_ue_golomb_long(gb) + 1;
            if (delta_idx > sps->nb_st_rps) {
                // av_log(avctx, AV_LOG_ERROR,
                //        "Invalid value of delta_idx in slice header RPS: %d > %d.\n",
                //        delta_idx, sps->nb_st_rps);
                return -1;
            }
            rps_ridx = &sps->st_rps[sps->nb_st_rps - delta_idx];
            rps->rps_idx_num_delta_pocs = rps_ridx->num_delta_pocs;
        } else
            rps_ridx = &sps->st_rps[rps - sps->st_rps - 1];

        delta_rps_sign = get_bit(gb);
        abs_delta_rps  = get_ue_golomb_long(gb) + 1;
        if (abs_delta_rps < 1 || abs_delta_rps > 32768) {
            // av_log(avctx, AV_LOG_ERROR,
            //        "Invalid value of abs_delta_rps: %d\n",
            //        abs_delta_rps);
            return -1;
        }
        delta_rps      = (1 - (delta_rps_sign << 1)) * abs_delta_rps;
        for (i = 0; i <= rps_ridx->num_delta_pocs; i++) 
        {
            int used = rps->used[k] = get_bit(gb);

            if (!used)
                use_delta_flag = get_bit(gb);

            if (used || use_delta_flag) 
            {
                if (i < rps_ridx->num_delta_pocs)
                    delta_poc = delta_rps + rps_ridx->delta_poc[i];
                else
                    delta_poc = delta_rps;
                rps->delta_poc[k] = delta_poc;
                if (delta_poc < 0)
                    k0++;
                else
                    k1++;
                k++;
            }
        }

        if (k >= FF_ARRAY_ELEMS(rps->used)) {
            // av_log(avctx, AV_LOG_ERROR,
            //        "Invalid num_delta_pocs: %d\n", k);
            return -1;
        }

        rps->num_delta_pocs    = k;
        rps->num_negative_pics = k0;
        // sort in increasing order (smallest first)
        if (rps->num_delta_pocs != 0) {
            int used, tmp;
            for (i = 1; i < rps->num_delta_pocs; i++) {
                delta_poc = rps->delta_poc[i];
                used      = rps->used[i];
                for (k = i - 1; k >= 0; k--) {
                    tmp = rps->delta_poc[k];
                    if (delta_poc < tmp) {
                        rps->delta_poc[k + 1] = tmp;
                        rps->used[k + 1]      = rps->used[k];
                        rps->delta_poc[k]     = delta_poc;
                        rps->used[k]          = used;
                    }
                }
            }
        }
        if ((rps->num_negative_pics >> 1) != 0) {
            int used;
            k = rps->num_negative_pics - 1;
            // flip the negative values to largest first
            for (i = 0; i < rps->num_negative_pics >> 1; i++) {
                delta_poc         = rps->delta_poc[i];
                used              = rps->used[i];
                rps->delta_poc[i] = rps->delta_poc[k];
                rps->used[i]      = rps->used[k];
                rps->delta_poc[k] = delta_poc;
                rps->used[k]      = used;
                k--;
            }
        }
    } else {
        unsigned int prev, nb_positive_pics;
        rps->num_negative_pics = get_ue_golomb_long(gb);
        nb_positive_pics       = get_ue_golomb_long(gb);

        if (rps->num_negative_pics >= HEVC_MAX_REFS ||
            nb_positive_pics >= HEVC_MAX_REFS) {
            // av_log(avctx, AV_LOG_ERROR, "Too many refs in a short term RPS.\n");
            return -1;
        }

        rps->num_delta_pocs = rps->num_negative_pics + nb_positive_pics;
        if (rps->num_delta_pocs) 
        {
            prev = 0;
            for (i = 0; i < rps->num_negative_pics; i++) 
            {
                delta_poc = get_ue_golomb_long(gb) + 1;
                if (delta_poc < 1 || delta_poc > 32768) 
                {
                    // av_log(avctx, AV_LOG_ERROR,
                    //     "Invalid value of delta_poc: %d\n",
                    //     delta_poc);
                    return -1;
                }
                prev -= delta_poc;
                rps->delta_poc[i] = prev;
                rps->used[i]      = get_bit(gb);
            }
            prev = 0;
            for (i = 0; i < nb_positive_pics; i++) 
            {
                delta_poc = get_ue_golomb_long(gb) + 1;
                if (delta_poc < 1 || delta_poc > 32768) 
                {
                    // av_log(avctx, AV_LOG_ERROR,
                    //     "Invalid value of delta_poc: %d\n",
                    //     delta_poc);
                    return -1;
                }
                prev += delta_poc;
                rps->delta_poc[rps->num_negative_pics + i] = prev;
                rps->used[rps->num_negative_pics + i]      = get_bit(gb);
            }
        }
    }
    return 0;
}

static int decode_hrd(GetBitContext *gb, int common_inf_present, int max_sublayers)
{
    int nal_params_present = 0, vcl_params_present = 0;
    int subpic_params_present = 0;
    int i;

    if (common_inf_present) {
        nal_params_present = get_bit(gb);
        vcl_params_present = get_bit(gb);

        if (nal_params_present || vcl_params_present) {
            subpic_params_present = get_bit(gb);

            if (subpic_params_present) {
                skip_bits(gb, 8); // tick_divisor_minus2
                skip_bits(gb, 5); // du_cpb_removal_delay_increment_length_minus1
                skip_bits(gb, 1); // sub_pic_cpb_params_in_pic_timing_sei_flag
                skip_bits(gb, 5); // dpb_output_delay_du_length_minus1
            }

            skip_bits(gb, 4); // bit_rate_scale
            skip_bits(gb, 4); // cpb_size_scale

            if (subpic_params_present)
                skip_bits(gb, 4);  // cpb_size_du_scale

            skip_bits(gb, 5); // initial_cpb_removal_delay_length_minus1
            skip_bits(gb, 5); // au_cpb_removal_delay_length_minus1
            skip_bits(gb, 5); // dpb_output_delay_length_minus1
        }
    }

    for (i = 0; i < max_sublayers; i++) {
        int low_delay = 0;
        unsigned int nb_cpb = 1;
        int fixed_rate = get_bit(gb);

        if (!fixed_rate)
            fixed_rate = get_bit(gb);

        if (fixed_rate)
            get_ue_golomb_long(gb);  // elemental_duration_in_tc_minus1
        else
            low_delay = get_bit(gb);

        if (!low_delay) {
            nb_cpb = get_ue_golomb_long(gb) + 1;
            if (nb_cpb < 1 || nb_cpb > 32) {
                // av_log(NULL, AV_LOG_ERROR, "nb_cpb %d invalid\n", nb_cpb);
                return -1;
            }
        }

        if (nal_params_present)
            decode_sublayer_hrd(gb, nb_cpb, subpic_params_present);
        if (vcl_params_present)
            decode_sublayer_hrd(gb, nb_cpb, subpic_params_present);
    }
    return 0;
}

static void decode_vui(GetBitContext *gb, int apply_defdispwin, HEVCSPS *sps)
{
    VUI backup_vui, *vui = &sps->vui;
    GetBitContext backup;
    int sar_present, alt = 0;

    // av_log(avctx, AV_LOG_DEBUG, "Decoding VUI\n");

    sar_present = get_bit(gb);
    if (sar_present) {
        uint8_t sar_idx = get_bits(gb, 8);
        if (sar_idx < FF_ARRAY_ELEMS(vui_sar))
            vui->sar = vui_sar[sar_idx];
        else if (sar_idx == 255) {
            vui->sar.num = get_bits(gb, 16);
            vui->sar.den = get_bits(gb, 16);
        } 
        else
        {
            // av_log(avctx, AV_LOG_WARNING,
            //        "Unknown SAR index: %u.\n", sar_idx);
        }
    }

    vui->overscan_info_present_flag = get_bit(gb);
    if (vui->overscan_info_present_flag)
        vui->overscan_appropriate_flag = get_bit(gb);

    vui->video_signal_type_present_flag = get_bit(gb);
    if (vui->video_signal_type_present_flag) {
        vui->video_format                    = get_bits(gb, 3);
        vui->video_full_range_flag           = get_bit(gb);
        vui->colour_description_present_flag = get_bit(gb);
        if (vui->colour_description_present_flag) 
        {
            vui->colour_primaries        = get_bits(gb, 8);
            vui->transfer_characteristic = get_bits(gb, 8);
            vui->matrix_coeffs           = get_bits(gb, 8);
        }
    }

    vui->chroma_loc_info_present_flag = get_bit(gb);
    if (vui->chroma_loc_info_present_flag) {
        vui->chroma_sample_loc_type_top_field    = get_ue_golomb_long(gb);
        vui->chroma_sample_loc_type_bottom_field = get_ue_golomb_long(gb);
    }

    vui->neutra_chroma_indication_flag = get_bit(gb);
    vui->field_seq_flag                = get_bit(gb);
    vui->frame_field_info_present_flag = get_bit(gb);

    // Backup context in case an alternate header is detected
    memcpy(&backup, gb, sizeof(backup));
    memcpy(&backup_vui, vui, sizeof(backup_vui));
    if (get_bits_left(gb) >= 68 && show_bits(gb, 21) == 0x100000) 
    {
        vui->default_display_window_flag = 0;
        // av_log(avctx, AV_LOG_WARNING, "Invalid default display window\n");
    } 
    else
        vui->default_display_window_flag = get_bit(gb);

    if (vui->default_display_window_flag) {
        int vert_mult  = hevc_sub_height_c[sps->chroma_format_idc];
        int horiz_mult = hevc_sub_width_c[sps->chroma_format_idc];
        vui->def_disp_win.left_offset   = get_ue_golomb_long(gb) * horiz_mult;
        vui->def_disp_win.right_offset  = get_ue_golomb_long(gb) * horiz_mult;
        vui->def_disp_win.top_offset    = get_ue_golomb_long(gb) *  vert_mult;
        vui->def_disp_win.bottom_offset = get_ue_golomb_long(gb) *  vert_mult;
    }

timing_info:
    vui->vui_timing_info_present_flag = get_bit(gb);

    if (vui->vui_timing_info_present_flag) {
        if( get_bits_left(gb) < 66 && !alt) {
            // The alternate syntax seem to have timing info located
            // at where def_disp_win is normally located
            // av_log(avctx, AV_LOG_WARNING,
            //        "Strange VUI timing information, retrying...\n");
            memcpy(vui, &backup_vui, sizeof(backup_vui));
            memcpy(gb, &backup, sizeof(backup));
            alt = 1;
            goto timing_info;
        }
        vui->vui_num_units_in_tick               = get_bits(gb, 32);
        vui->vui_time_scale                      = get_bits(gb, 32);
        if (alt) {
            // av_log(avctx, AV_LOG_INFO, "Retry got %"PRIu32"/%"PRIu32" fps\n",
            //        vui->vui_time_scale, vui->vui_num_units_in_tick);
        }
        vui->vui_poc_proportional_to_timing_flag = get_bit(gb);
        if (vui->vui_poc_proportional_to_timing_flag)
            vui->vui_num_ticks_poc_diff_one_minus1 = get_ue_golomb_long(gb);
        vui->vui_hrd_parameters_present_flag = get_bit(gb);
        if (vui->vui_hrd_parameters_present_flag)
            decode_hrd(gb, 1, sps->max_sub_layers);
    }

    vui->bitstream_restriction_flag = get_bit(gb);
    if (vui->bitstream_restriction_flag) {
        if (get_bits_left(gb) < 8 && !alt) {
            // av_log(avctx, AV_LOG_WARNING,
            //        "Strange VUI bitstream restriction information, retrying"
            //        " from timing information...\n");
            memcpy(vui, &backup_vui, sizeof(backup_vui));
            memcpy(gb, &backup, sizeof(backup));
            alt = 1;
            goto timing_info;
        }
        vui->tiles_fixed_structure_flag              = get_bit(gb);
        vui->motion_vectors_over_pic_boundaries_flag = get_bit(gb);
        vui->restricted_ref_pic_lists_flag           = get_bit(gb);
        vui->min_spatial_segmentation_idc            = get_ue_golomb_long(gb);
        vui->max_bytes_per_pic_denom                 = get_ue_golomb_long(gb);
        vui->max_bits_per_min_cu_denom               = get_ue_golomb_long(gb);
        vui->log2_max_mv_length_horizontal           = get_ue_golomb_long(gb);
        vui->log2_max_mv_length_vertical             = get_ue_golomb_long(gb);
    }

    if (get_bits_left(gb) < 1 && !alt) {
        // XXX: Alternate syntax when sps_range_extension_flag != 0?
        // av_log(avctx, AV_LOG_WARNING,
        //        "Overread in VUI, retrying from timing information...\n");
        memcpy(vui, &backup_vui, sizeof(backup_vui));
        memcpy(gb, &backup, sizeof(backup));
        alt = 1;
        goto timing_info;
    }
}

static int scaling_list_data(GetBitContext *gb, ScalingList *sl, HEVCSPS *sps)
{
    uint8_t scaling_list_pred_mode_flag;
    int32_t scaling_list_dc_coef[2][6];
    int size_id, matrix_id, pos;
    int i;

    for (size_id = 0; size_id < 4; size_id++)
        for (matrix_id = 0; matrix_id < 6; matrix_id += ((size_id == 3) ? 3 : 1)) 
        {
            scaling_list_pred_mode_flag = get_bit(gb);
            if (!scaling_list_pred_mode_flag) 
            {
                unsigned int delta = get_ue_golomb_long(gb);
                /* Only need to handle non-zero delta. Zero means default,
                 * which should already be in the arrays. */
                if (delta) {
                    // Copy from previous array.
                    delta *= (size_id == 3) ? 3 : 1;
                    if (matrix_id < delta) 
                    {
                        return -1;
                    }

                    memcpy(sl->sl[size_id][matrix_id],
                           sl->sl[size_id][matrix_id - delta],
                           size_id > 0 ? 64 : 16);
                    if (size_id > 1)
                        sl->sl_dc[size_id - 2][matrix_id] = sl->sl_dc[size_id - 2][matrix_id - delta];
                }
            } 
            else 
            {
                int next_coef, coef_num;
                int32_t scaling_list_delta_coef;

                next_coef = 8;
                coef_num  = GYJRMIN(64, 1 << (4 + (size_id << 1)));
                if (size_id > 1) 
                {
                    scaling_list_dc_coef[size_id - 2][matrix_id] = get_se_golomb(gb) + 8;
                    next_coef = scaling_list_dc_coef[size_id - 2][matrix_id];
                    sl->sl_dc[size_id - 2][matrix_id] = next_coef;
                }
                for (i = 0; i < coef_num; i++) {
                    if (size_id == 0)
                        pos = 4 * ff_hevc_diag_scan4x4_y[i] +
                                  ff_hevc_diag_scan4x4_x[i];
                    else
                        pos = 8 * ff_hevc_diag_scan8x8_y[i] +
                                  ff_hevc_diag_scan8x8_x[i];

                    scaling_list_delta_coef = get_se_golomb(gb);
                    next_coef = (next_coef + 256U + scaling_list_delta_coef) % 256;
                    sl->sl[size_id][matrix_id][pos] = next_coef;
                }
            }
        }

    if (sps->chroma_format_idc == 3) 
    {
        for (i = 0; i < 64; i++) 
        {
            sl->sl[3][1][i] = sl->sl[2][1][i];
            sl->sl[3][2][i] = sl->sl[2][2][i];
            sl->sl[3][4][i] = sl->sl[2][4][i];
            sl->sl[3][5][i] = sl->sl[2][5][i];
        }
        sl->sl_dc[1][1] = sl->sl_dc[0][1];
        sl->sl_dc[1][2] = sl->sl_dc[0][2];
        sl->sl_dc[1][4] = sl->sl_dc[0][4];
        sl->sl_dc[1][5] = sl->sl_dc[0][5];
    }

    return 0;
}

static int decode_profile_tier_level(GetBitContext *gb, PTLCommon *ptl)
{
    int i;

    ptl->profile_space = get_bits(gb, 2);
    ptl->tier_flag     = get_bit(gb);
    ptl->profile_idc   = get_bits(gb, 5);

    for (i = 0; i < 32; i++) {
        ptl->profile_compatibility_flag[i] = get_bit(gb);

        if (ptl->profile_idc == 0 && i > 0 && ptl->profile_compatibility_flag[i])
            ptl->profile_idc = i;
    }
    ptl->progressive_source_flag    = get_bit(gb);
    ptl->interlaced_source_flag     = get_bit(gb);
    ptl->non_packed_constraint_flag = get_bit(gb);
    ptl->frame_only_constraint_flag = get_bit(gb);

    skip_bits(gb, 16); // XXX_reserved_zero_44bits[0..15]
    skip_bits(gb, 16); // XXX_reserved_zero_44bits[16..31]
    skip_bits(gb, 12); // XXX_reserved_zero_44bits[32..43/Users/yangshiquan/Downloads/work/opensorce/FFmpeg/libavcodec/mpeg4videodec.c]

    return 0;
}

static int parse_ptl(GetBitContext *gb, PTL *ptl, int max_num_sub_layers)
{
    int i;
    if (decode_profile_tier_level(gb, &ptl->general_ptl) < 0 ||
        get_bits_left(gb) < 8 + (8*2 * (max_num_sub_layers - 1 > 0))) {
        return -1;
    }

    ptl->general_ptl.level_idc = get_bits(gb, 8);

    for (i = 0; i < max_num_sub_layers - 1; i++) {
        ptl->sub_layer_profile_present_flag[i] = get_bit(gb);
        ptl->sub_layer_level_present_flag[i]   = get_bit(gb);
    }

    if (max_num_sub_layers - 1> 0)
        for (i = max_num_sub_layers - 1; i < 8; i++)
            skip_bits(gb, 2); // reserved_zero_2bits[i]
    for (i = 0; i < max_num_sub_layers - 1; i++) {
        if (ptl->sub_layer_profile_present_flag[i] &&
            decode_profile_tier_level(gb, &ptl->sub_layer_ptl[i]) < 0) {
            return -1;
        }
        if (ptl->sub_layer_level_present_flag[i]) {
            if (get_bits_left(gb) < 8) {
                return -1;
            } else
                ptl->sub_layer_ptl[i].level_idc = get_bits(gb, 8);
        }
    }

    return 0;
}

int ff_hevc_parse_sps(HEVCSPS *sps, uint8_t* data, size_t len, unsigned int* sps_id, int apply_defdispwin)
{
    GetBitContext* gb = (GetBitContext*)malloc(sizeof(GetBitContext));
    init_get_bits(gb, data, (unsigned int)len);
    gb->index = 16;

    HEVCWindow *ow;
    int ret = 0;
    int log2_diff_max_min_transform_block_size;
    int bit_depth_chroma, start, vui_present, sublayer_ordering_info;
    int i;

    sps->vps_id = get_bits(gb, 4);
    sps->max_sub_layers = get_bits(gb, 3) + 1;
    if (sps->max_sub_layers > HEVC_MAX_SUB_LAYERS)
    {
        GYLOGE("max_sub_layers:%d > HEVC_MAX_SUB_LAYERS:%d", sps->max_sub_layers, HEVC_MAX_SUB_LAYERS);
        goto failed;
    }

    sps->temporal_id_nesting_flag = get_bits(gb, 1);
    if ((ret = parse_ptl(gb, &sps->ptl, sps->max_sub_layers)) < 0)
    {
        GYLOGE("parse_ptl error:%d", ret);
        goto failed;
    }

    *sps_id = get_ue_golomb_long(gb);
    if (*sps_id >= HEVC_MAX_SPS_COUNT)
    {
        goto failed;
    }

    sps->chroma_format_idc = get_ue_golomb_long(gb);
    if (sps->chroma_format_idc > 3U)
    {
        goto failed;
    }

    if (sps->chroma_format_idc == 3)
    {
        sps->separate_colour_plane_flag = get_bit(gb);
    }

    if (sps->separate_colour_plane_flag)
    {
        sps->chroma_format_idc = 0;
    }
        
    sps->width  = get_ue_golomb_long(gb);
    sps->height = get_ue_golomb_long(gb);

    if (get_bit(gb)) // pic_conformance_flag
    {
        int vert_mult  = hevc_sub_height_c[sps->chroma_format_idc];
        int horiz_mult = hevc_sub_width_c[sps->chroma_format_idc];
        sps->pic_conf_win.left_offset   = get_ue_golomb_long(gb) * horiz_mult;
        sps->pic_conf_win.right_offset  = get_ue_golomb_long(gb) * horiz_mult;
        sps->pic_conf_win.top_offset    = get_ue_golomb_long(gb) *  vert_mult;
        sps->pic_conf_win.bottom_offset = get_ue_golomb_long(gb) *  vert_mult;

        sps->output_window = sps->pic_conf_win;
    }

    sps->bit_depth   = get_ue_golomb_long(gb) + 8;
    bit_depth_chroma = get_ue_golomb_long(gb) + 8;
    if (sps->chroma_format_idc && bit_depth_chroma != sps->bit_depth)
    {
        goto failed;
    }
    sps->bit_depth_chroma = bit_depth_chroma;

    sps->log2_max_poc_lsb = get_ue_golomb_long(gb) + 4;
    if (sps->log2_max_poc_lsb > 16)
    {
        goto failed;
    }

    sublayer_ordering_info = get_bit(gb);
    start = sublayer_ordering_info ? 0 : sps->max_sub_layers - 1;
    for (i = start; i < sps->max_sub_layers; i++)
    {
        sps->temporal_layer[i].max_dec_pic_buffering = get_ue_golomb_long(gb) + 1;
        sps->temporal_layer[i].num_reorder_pics      = get_ue_golomb_long(gb);
        sps->temporal_layer[i].max_latency_increase  = get_ue_golomb_long(gb) - 1;
        if (sps->temporal_layer[i].max_dec_pic_buffering > (unsigned)HEVC_MAX_DPB_SIZE)
        {
            goto failed;
        }
        
        if (sps->temporal_layer[i].num_reorder_pics > sps->temporal_layer[i].max_dec_pic_buffering - 1)
        {
            if (sps->temporal_layer[i].num_reorder_pics > HEVC_MAX_DPB_SIZE - 1)
            {
                goto failed;
            }
            
            sps->temporal_layer[i].max_dec_pic_buffering = sps->temporal_layer[i].num_reorder_pics + 1;
        }
    }

    if (!sublayer_ordering_info)
    {
        for (i = 0; i < start; i++)
        {
            sps->temporal_layer[i].max_dec_pic_buffering = sps->temporal_layer[start].max_dec_pic_buffering;
            sps->temporal_layer[i].num_reorder_pics      = sps->temporal_layer[start].num_reorder_pics;
            sps->temporal_layer[i].max_latency_increase  = sps->temporal_layer[start].max_latency_increase;
        }
    }

    sps->log2_min_cb_size                    = get_ue_golomb_long(gb) + 3;
    sps->log2_diff_max_min_coding_block_size = get_ue_golomb_long(gb);
    sps->log2_min_tb_size                    = get_ue_golomb_long(gb) + 2;
    log2_diff_max_min_transform_block_size   = get_ue_golomb_long(gb);
    sps->log2_max_trafo_size                 = log2_diff_max_min_transform_block_size +
                                               sps->log2_min_tb_size;
    
    if (sps->log2_min_cb_size < 3 || sps->log2_min_cb_size > 30)
    {
        goto failed;
    }

    if (sps->log2_diff_max_min_coding_block_size > 30)
    {
        goto failed;
    }

    if (sps->log2_min_tb_size >= sps->log2_min_cb_size || sps->log2_min_tb_size < 2)
    {
        goto failed;
    }

    if (log2_diff_max_min_transform_block_size < 0 || log2_diff_max_min_transform_block_size > 30)
    {
        goto failed;
    }

    sps->max_transform_hierarchy_depth_inter = get_ue_golomb_long(gb);
    sps->max_transform_hierarchy_depth_intra = get_ue_golomb_long(gb);

    sps->scaling_list_enable_flag = get_bit(gb);
    if (sps->scaling_list_enable_flag)
    {
        set_default_scaling_list_data(&sps->scaling_list);

        if (get_bit(gb))
        {
            ret = scaling_list_data(gb, &sps->scaling_list, sps);
            if (ret < 0)
                goto failed;
        }
    }

    sps->amp_enabled_flag = get_bit(gb);
    sps->sao_enabled      = get_bit(gb);

    sps->pcm_enabled_flag = get_bit(gb);
    if (sps->pcm_enabled_flag)
    {
        sps->pcm.bit_depth   = get_bits(gb, 4) + 1;
        sps->pcm.bit_depth_chroma = get_bits(gb, 4) + 1;
        sps->pcm.log2_min_pcm_cb_size = get_ue_golomb_long(gb) + 3;
        sps->pcm.log2_max_pcm_cb_size = sps->pcm.log2_min_pcm_cb_size +
                                        get_ue_golomb_long(gb);
        if (GYJRMAX(sps->pcm.bit_depth, sps->pcm.bit_depth_chroma) > sps->bit_depth)
        {
            goto failed;
        }

        sps->pcm.loop_filter_disable_flag = get_bit(gb);
    }

    sps->nb_st_rps = get_ue_golomb_long(gb);
    if (sps->nb_st_rps > HEVC_MAX_SHORT_TERM_REF_PIC_SETS)
    {
        goto failed;
    }
    
    for (i = 0; i < sps->nb_st_rps; i++)
    {
        if ((ret = ff_hevc_decode_short_term_rps(gb, &sps->st_rps[i], sps, 0)) < 0)
            goto failed;
    }

    sps->long_term_ref_pics_present_flag = get_bit(gb);
    if (sps->long_term_ref_pics_present_flag) 
    {
        sps->num_long_term_ref_pics_sps = get_ue_golomb_long(gb);
        if (sps->num_long_term_ref_pics_sps > HEVC_MAX_LONG_TERM_REF_PICS) 
        {
            goto failed;
        }
        
        for (i = 0; i < sps->num_long_term_ref_pics_sps; i++) 
        {
            sps->lt_ref_pic_poc_lsb_sps[i]       = get_bits(gb, sps->log2_max_poc_lsb);
            sps->used_by_curr_pic_lt_sps_flag[i] = get_bit(gb);
        }
    }

    sps->sps_temporal_mvp_enabled_flag          = get_bit(gb);
    sps->sps_strong_intra_smoothing_enable_flag = get_bit(gb);
    sps->vui.sar = (AVRational){0, 1};
    vui_present = get_bit(gb);
    if (vui_present)
    {
        decode_vui(gb, apply_defdispwin, sps);
    }

    if (get_bit(gb))
    {
        sps->sps_range_extension_flag = get_bit(gb);
        skip_bits(gb, 7); //sps_extension_7bits = get_bits(gb, 7);
        if (sps->sps_range_extension_flag)
        {
            sps->transform_skip_rotation_enabled_flag = get_bit(gb);
            sps->transform_skip_context_enabled_flag  = get_bit(gb);
            sps->implicit_rdpcm_enabled_flag = get_bit(gb);

            sps->explicit_rdpcm_enabled_flag = get_bit(gb);

            sps->extended_precision_processing_flag = get_bit(gb);
            if (sps->extended_precision_processing_flag)
            {
                // av_log(avctx, AV_LOG_WARNING,
                //    "extended_precision_processing_flag not yet implemented\n");
            }

            sps->intra_smoothing_disabled_flag       = get_bit(gb);
            sps->high_precision_offsets_enabled_flag = get_bit(gb);
            if (sps->high_precision_offsets_enabled_flag)
            {
                // av_log(avctx, AV_LOG_WARNING,
                //    "high_precision_offsets_enabled_flag not yet implemented\n");
            }

            sps->persistent_rice_adaptation_enabled_flag = get_bit(gb);

            sps->cabac_bypass_alignment_enabled_flag  = get_bit(gb);
            if (sps->cabac_bypass_alignment_enabled_flag)
            {
                // av_log(avctx, AV_LOG_WARNING,
                //    "cabac_bypass_alignment_enabled_flag not yet implemented\n");
            }
        }
    }
    
    if (apply_defdispwin) 
    {
        sps->output_window.left_offset   += sps->vui.def_disp_win.left_offset;
        sps->output_window.right_offset  += sps->vui.def_disp_win.right_offset;
        sps->output_window.top_offset    += sps->vui.def_disp_win.top_offset;
        sps->output_window.bottom_offset += sps->vui.def_disp_win.bottom_offset;
    }

    ow = &sps->output_window;
    if (ow->left_offset >= INT_MAX - ow->right_offset     ||
        ow->top_offset  >= INT_MAX - ow->bottom_offset    ||
        ow->left_offset + ow->right_offset  >= sps->width ||
        ow->top_offset  + ow->bottom_offset >= sps->height)
    {
        // av_log(avctx, AV_LOG_WARNING, "Invalid cropping offsets: %u/%u/%u/%u\n",
        //        ow->left_offset, ow->right_offset, ow->top_offset, ow->bottom_offset);
        // av_log(avctx, AV_LOG_WARNING,
        //        "Displaying the whole video surface.\n");
        memset(ow, 0, sizeof(*ow));
        memset(&sps->pic_conf_win, 0, sizeof(sps->pic_conf_win));
    }

    // Inferred parameters
    sps->log2_ctb_size = sps->log2_min_cb_size +
                         sps->log2_diff_max_min_coding_block_size;
    sps->log2_min_pu_size = sps->log2_min_cb_size - 1;

    if (sps->log2_ctb_size > HEVC_MAX_LOG2_CTB_SIZE)
    {
        // av_log(avctx, AV_LOG_ERROR, "CTB size out of range: 2^%d\n", sps->log2_ctb_size);
        return -1;
    }
    if (sps->log2_ctb_size < 4)
    {
        // av_log(avctx,
        //        AV_LOG_ERROR,
        //        "log2_ctb_size %d differs from the bounds of any known profile\n",
        //        sps->log2_ctb_size);
        // avpriv_request_sample(avctx, "log2_ctb_size %d", sps->log2_ctb_size);
#if YSQ_JR_GIT
        GYLOGE("log2_ctb_size %d differs from the bounds of any known profile", sps->log2_ctb_size);
#endif
        return -1;
    }

    sps->ctb_width  = (sps->width  + (1 << sps->log2_ctb_size) - 1) >> sps->log2_ctb_size;
    sps->ctb_height = (sps->height + (1 << sps->log2_ctb_size) - 1) >> sps->log2_ctb_size;
    sps->ctb_size   = sps->ctb_width * sps->ctb_height;

    sps->min_cb_width  = sps->width  >> sps->log2_min_cb_size;
    sps->min_cb_height = sps->height >> sps->log2_min_cb_size;
    sps->min_tb_width  = sps->width  >> sps->log2_min_tb_size;
    sps->min_tb_height = sps->height >> sps->log2_min_tb_size;
    sps->min_pu_width  = sps->width  >> sps->log2_min_pu_size;
    sps->min_pu_height = sps->height >> sps->log2_min_pu_size;
    sps->tb_mask       = (1 << (sps->log2_ctb_size - sps->log2_min_tb_size)) - 1;

    sps->qp_bd_offset = 6 * (sps->bit_depth - 8);

    if (gy_mod_uintp2_c(sps->width, sps->log2_min_cb_size) ||
        gy_mod_uintp2_c(sps->height, sps->log2_min_cb_size))
    {
#if YSQ_JR_GIT
        GYLOGE("Invalid coded frame dimensions w:%d h:%d mcs:%d.", sps->width, sps->height, sps->log2_min_cb_size);
#endif
        goto failed;
    }

    if (sps->max_transform_hierarchy_depth_inter > sps->log2_ctb_size - sps->log2_min_tb_size)
    {
#if YSQ_JR_GIT
        GYLOGE("error: %d > %d", sps->max_transform_hierarchy_depth_inter, sps->log2_ctb_size - sps->log2_min_tb_size);
#endif
        goto failed;
    }
    
    if (sps->max_transform_hierarchy_depth_intra > sps->log2_ctb_size - sps->log2_min_tb_size)
    {
#if YSQ_JR_GIT
        GYLOGE("error: %d > %d", sps->max_transform_hierarchy_depth_intra, sps->log2_ctb_size - sps->log2_min_tb_size);
#endif
        goto failed;
    }
    
    if (sps->log2_max_trafo_size > GYJRMIN(sps->log2_ctb_size, 5))
    {
#if YSQ_JR_GIT
        GYLOGE("error %d > %d", sps->log2_max_trafo_size, GYJRMIN(sps->log2_ctb_size, 5));
#else
#endif
        goto failed;
    }

    if (get_bits_left(gb) < 0) {
        // av_log(avctx, AV_LOG_ERROR,
        //        "Overread SPS by %d bits\n", -get_bits_left(gb));
#if YSQ_JR_GIT
        GYLOGI("Overread SPS by %d bits", -get_bits_left(gb));
#else
#endif
        goto failed;
    }

    return 0;

failed:
    if (gb)
    {
        free(gb);
        gb = NULL;
    }
    return -1;
}
