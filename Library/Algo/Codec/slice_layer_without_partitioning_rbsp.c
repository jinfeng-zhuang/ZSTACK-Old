#include "codec.h"
#include "slice.h"

static struct slice_layer_without_partitioning_rbsp slice;

int ref_pic_list_mvc_modification(struct bitstream* bs, struct ref_pic_list_mvc_modification* header)
{
    return 0;
}

int ref_pic_list_modification(struct bitstream* bs, unsigned char type, struct ref_pic_list_modification* modify)
{
    unsigned int i;

    if ((type % 5 != 2) && (type % 5 != 4)) {
        modify->ref_pic_list_modification_flag_l0 = se_read_u(bs, 1);
        if (modify->ref_pic_list_modification_flag_l0) {
            i = 0;
            do {
                modify->modification_of_pic_nums_idc[0][i] = se_read_ue(bs);
                if ((0 == modify->modification_of_pic_nums_idc[0][i]) ||
                    (1 == modify->modification_of_pic_nums_idc[0][i])) {
                    modify->abs_diff_pic_num_minus1[0][i] = se_read_ue(bs);
                }
                else if (2 == modify->modification_of_pic_nums_idc[0][i]) {
                    modify->long_term_pic_num[0][i] = se_read_ue(bs);
                }
            } while (3 != modify->modification_of_pic_nums_idc[0][i++]);
        }
    }

    if (1 == type % 5) {
        modify->ref_pic_list_modification_flag_l1 = se_read_u(bs, 1);
        if (modify->ref_pic_list_modification_flag_l1) {
            i = 0;
            do {
                modify->modification_of_pic_nums_idc[1][i] = se_read_ue(bs);
                if ((0 == modify->modification_of_pic_nums_idc[1][i]) ||
                    (1 == modify->modification_of_pic_nums_idc[1][i])) {
                    modify->abs_diff_pic_num_minus1[1][i] = se_read_ue(bs);
                }
                else if (2 == modify->modification_of_pic_nums_idc[1][i]) {
                    modify->long_term_pic_num[1][i] = se_read_ue(bs);
                }
            } while (3 != modify->modification_of_pic_nums_idc[1][i++]);
        }
    }

    return 0;
}

int pred_weight_table(struct bitstream* bs, struct slice_header* header, struct seq_parameter_set_rbsp* sps)
{
    unsigned int i;
    unsigned int j;

    header->pred_weight_table.luma_log2_weight_denom = se_read_ue(bs);

    if (0 != sps->ChromaArrayType)
        header->pred_weight_table.chroma_log2_weight_denom = se_read_ue(bs);

    for (i = 0; i < header->num_ref_idx_l0_active_minus1; i++) {
        header->pred_weight_table.luma_weight_l0_flag = se_read_u(bs, 1);
        if (header->pred_weight_table.luma_weight_l0_flag) {
            header->pred_weight_table.luma_weight_l0[i] = se_read_se(bs);
            header->pred_weight_table.luma_offset_l0[i] = se_read_se(bs);
        }

        if (0 != sps->ChromaArrayType) {
            header->pred_weight_table.chroma_weight_l0_flag = se_read_u(bs, 1);
            if (header->pred_weight_table.chroma_weight_l0_flag) {
                for (j = 0; j < 2; j++) {
                    header->pred_weight_table.chroma_weight_l0[i][j] = se_read_se(bs);
                    header->pred_weight_table.chroma_offset_l0[i][j] = se_read_se(bs);
                }
            }
        }
    }

    if (1 == header->slice_type % 5) {
        for (i = 0; i < header->num_ref_idx_l1_active_minus1; i++) {
            header->pred_weight_table.luma_weight_l1_flag = se_read_u(bs, 1);
            if (header->pred_weight_table.luma_weight_l1_flag) {
                header->pred_weight_table.luma_weight_l1[i] = se_read_se(bs);
                header->pred_weight_table.luma_offset_l1[i] = se_read_se(bs);
            }

            if (0 != sps->ChromaArrayType) {
                header->pred_weight_table.chroma_weight_l1_flag = se_read_u(bs, 1);
                if (header->pred_weight_table.chroma_weight_l1_flag) {
                    for (j = 0; j < 2; j++) {
                        header->pred_weight_table.chroma_weight_l1[i][j] = se_read_se(bs);
                        header->pred_weight_table.chroma_offset_l1[i][j] = se_read_se(bs);
                    }
                }
            }
        }
    }

    return 0;
}

int dec_ref_pic_marking(struct bitstream* bs, struct dec_ref_pic_marking* mark)
{
    unsigned int i;
    struct nalu* nalu;

    nalu = nalu_get_info();

    if (nalu->IdrPicFlag) {
        mark->no_output_of_prior_pics_flag = se_read_u(bs, 1);
        mark->long_term_reference_flag = se_read_u(bs, 1);
    }
    else {
        mark->adaptive_ref_pic_marking_mode_flag = se_read_u(bs, 1);
        if (mark->adaptive_ref_pic_marking_mode_flag) {
            i = 0;
            do {
                mark->memory_management_control_operation[i] = se_read_ue(bs);
                if ((1 == mark->memory_management_control_operation[i]) ||
                    (3 == mark->memory_management_control_operation[i])) {
                    mark->difference_of_pic_nums_minus1[i] = se_read_ue(bs);
                }
                if (2 == mark->memory_management_control_operation[i]) {
                    mark->long_term_pic_num[i] = se_read_ue(bs);
                }
                if ((3 == mark->memory_management_control_operation[i]) ||
                    (6 == mark->memory_management_control_operation[i])) {
                    mark->long_term_frame_idx[i] = se_read_ue(bs);
                }
                if (4 == mark->memory_management_control_operation[i]) {
                    mark->max_long_term_frame_idx_plus1[i] = se_read_ue(bs);
                }
            } while (mark->memory_management_control_operation[i++] != 0);
        }
    }

    return 0;
}

static int slice_header(struct bitstream* bs, struct slice_header* header)
{
    struct seq_parameter_set_rbsp* sps;
    struct pic_parameter_set_rbsp* pps;
    struct nalu* nalu;
    struct h264_context* ctx;

    ctx = h264_context_get();

    header->first_mb_in_slice = se_read_ue(bs);
    header->slice_type = se_read_ue(bs);

    if (header->slice_type >= 5) while (1);
    //header->slice_type = header->slice_type % 5;

    header->pic_parameter_set_id = se_read_ue(bs);

    nalu = nalu_get_info();
    if (NULL == nalu)
        return -1;

    pps = pps_get(header->pic_parameter_set_id);
    if (NULL == pps)
        return -1;

    ctx->pps = pps;

    sps = sps_get(pps->seq_parameter_set_id);
    if (NULL == sps)
        return -1;

    ctx->sps = sps;

    if (1 == sps->data.separate_colour_plane_flag) {
        header->colour_plane_id = se_read_u(bs, 2);
    }

    header->frame_num = se_read_u(bs, sps->data.log2_max_frame_num_minus4 + 4);

    if (!sps->data.frame_mbs_only_flag) {
        header->field_pic_flag = se_read_u(bs, 1);

        ctx->slice->MbaffFrameFlag = sps->data.mb_adaptive_frame_field_flag && (!header->field_pic_flag);

        if (header->field_pic_flag) {
            header->bottom_field_flag = se_read_u(bs, 1);
        }
    }

    ctx->slice->PicHeightInMbs = sps->FrameHeightInMbs / (1 + header->field_pic_flag);
    ctx->slice->PicSizeInMbs   = sps->PicWidthInMbs * ctx->slice->PicHeightInMbs;

    if (nalu->IdrPicFlag)
        header->idr_pic_id = se_read_ue(bs);

    if (0 == sps->data.pic_order_cnt_type) {
        header->pic_order_cnt_lsb = se_read_u(bs, sps->data.log2_max_pic_order_cnt_lsb_minus4 + 4);
        if (pps->bottom_field_pic_order_in_frame_present_flag && (!header->field_pic_flag))
            header->delta_pic_order_cnt_bottom = se_read_se(bs);
    }

    if ((1 == sps->data.pic_order_cnt_type) && (!sps->data.delta_pic_order_always_zero_flag)) {
        header->delta_pic_order_cnt[0] = se_read_se(bs);
        if (pps->bottom_field_pic_order_in_frame_present_flag && (!header->field_pic_flag)) {
            header->delta_pic_order_cnt[1] = se_read_se(bs);
        }
    }

    if (pps->redundant_pic_cnt_present_flag) {
        header->redundant_pic_cnt = se_read_ue(bs);
    }

    if (SLICE_TYPE_B == header->slice_type) {
        header->direct_spatial_mv_pred_flag = se_read_u(bs, 1);
    }

    if ((SLICE_TYPE_P == header->slice_type) || (SLICE_TYPE_SP == header->slice_type) || (SLICE_TYPE_B == header->slice_type)) {
        header->num_ref_idx_active_override_flag = se_read_u(bs, 1);
        if (header->num_ref_idx_active_override_flag) {
            header->num_ref_idx_l0_active_minus1 = se_read_ue(bs);
            if (SLICE_TYPE_B == header->slice_type) {
                header->num_ref_idx_l1_active_minus1 = se_read_ue(bs);
            }
        }
    }

    if ((20 == nalu->type) || (21 == nalu->type)) {
        ref_pic_list_mvc_modification(bs, &header->ref_pic_list_mvc_modification);
    }
    else {
        ref_pic_list_modification(bs, header->slice_type, &header->ref_pic_list_modification);
    }
    
    if (((pps->weighted_pred_flag) && ((SLICE_TYPE_P == header->slice_type) || (SLICE_TYPE_SP == header->slice_type))) ||
        ((1 == pps->weighted_bipred_idc) && (SLICE_TYPE_B == header->slice_type))) {
        pred_weight_table(bs, header, sps);
    }

    if (0 != nalu->nal_ref_idc) {
        dec_ref_pic_marking(bs, &header->dec_ref_pic_marking);
    }

    if (pps->entropy_coding_mode_flag && (SLICE_TYPE_I != header->slice_type) && (SLICE_TYPE_SI != header->slice_type)) {
        header->cabac_init_idc = se_read_ue(bs);
    }

    header->slice_qp_delta = se_read_se(bs);

    if ((SLICE_TYPE_SP == header->slice_type) || (SLICE_TYPE_SI == header->slice_type)) {
        if (SLICE_TYPE_SP == header->slice_type) {
            header->sp_for_switch_flag = se_read_u(bs, 1);
        }
        header->slice_qs_delta = se_read_se(bs);
    }

    if (pps->deblocking_filter_control_present_flag) {
        header->disable_deblocking_filter_idc = se_read_ue(bs);
        if (1 != header->disable_deblocking_filter_idc) {
            header->slice_alpha_c0_offset_div2 = se_read_se(bs);
            header->slice_beta_offset_div2 = se_read_se(bs);
        }
    }

    if ((pps->num_slice_groups_minus1 > 0) && (pps->slice_group_map_type >= 3) && (pps->slice_group_map_type <= 5)) {
        header->slice_group_change_cycle = se_read_uv(bs);
    }
}

//============================================================================
// Slice Data
//============================================================================

static unsigned int NextMbAddress(unsigned int n)
{
    unsigned int i;
    unsigned int j;
    unsigned int x;
    unsigned int y;
    unsigned int iGroup;
    struct h264_context* ctx;
    struct slice_layer_without_partitioning_rbsp* slice;
    struct pic_parameter_set_rbsp* pps;
    struct seq_parameter_set_rbsp* sps;

    ctx = h264_context_get();
    slice = ctx->slice;
    pps   = ctx->pps;
    sps   = ctx->sps;

    // TODO: requre free after slice decode complete
    slice->mapUnitToSliceGroupMap = (unsigned int*)malloc(4 * pps->pic_size_in_map_units_minus1);
    if (pps->num_slice_groups_minus1 == 0) {
        memset(slice->mapUnitToSliceGroupMap, 0, 4 * pps->pic_size_in_map_units_minus1);
    } else {
        switch (pps->slice_group_map_type) {
            case 0:
                i = 0;
                do {
                    for (iGroup = 0; iGroup <= pps->num_slice_groups_minus1 && i < sps->PicSizeInMapUnits; i += pps->run_length_minus1[iGroup++] + 1) {
                        for (j = 0; j < pps->run_length_minus1[iGroup] && (i + j) < slice->PicSizeInMbs; j++) {
                            slice->mapUnitToSliceGroupMap[i + j] = iGroup;
                        }
                    }
                } while (i < slice->PicSizeInMbs);
                break;
            case 1:
                for (i = 0; i < sps->PicSizeInMapUnits; i++) {
                    slice->mapUnitToSliceGroupMap[i] = ((i % sps->PicWidthInMbs) + (((i / sps->PicWidthInMbs) * (pps->num_slice_groups_minus1 + 1)) / 2)) % (pps->num_slice_groups_minus1 + 1);
                }
                break;
            case 2:
                for (i = 0; i < sps->PicSizeInMapUnits; i++) {
                    slice->mapUnitToSliceGroupMap[i] = pps->num_slice_groups_minus1;
                }

                for (iGroup = pps->num_slice_groups_minus1 - 1; iGroup >= 0; iGroup--) {
                    unsigned int xTopLeft, yTopLeft, xBottomRight, yBottomRight;
                    yTopLeft = pps->top_left[iGroup] / sps->PicWidthInMbs;
                    xTopLeft = pps->top_left[iGroup] % sps->PicWidthInMbs;
                    yBottomRight = pps->bottom_right[iGroup] / sps->PicWidthInMbs;
                    xBottomRight = pps->bottom_right[iGroup] % sps->PicWidthInMbs;
                    for (y = yTopLeft; y < yBottomRight; y++) {
                        for (x = xTopLeft; x < xBottomRight; x++) {
                            slice->mapUnitToSliceGroupMap[y * sps->PicWidthInMbs + x] = iGroup;
                        }
                    }
                }
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            default:
                break;
        }
    }

    i = n + 1;
    while ((i < ctx->slice->PicSizeInMbs) && (slice->MbToSliceGroupMap[i] != slice->MbToSliceGroupMap[n]))
        i++;

    return i;
}

static int slice_data(void)
{
    struct h264_context *ctx;
    struct seq_parameter_set_rbsp* sps;
    struct pic_parameter_set_rbsp* pps;
    struct slice_layer_without_partitioning_rbsp* slice;
    struct slice_data* data;
    struct slice_header* header;
    struct bitstream* bs;

    unsigned int CurrMbAddr;
    unsigned int moreDataFlag = 1;
    unsigned int prevMbSkipped = 0;
    unsigned int i;

    ctx = h264_context_get();
    sps = ctx->sps;
    pps = ctx->pps;
    slice = ctx->slice;
    data = &slice->data;
    header = &slice->header;
    bs = ctx->bs;

    if (pps->entropy_coding_mode_flag) {
        while (!byte_aligned(bs))
            data->cabac_alignment_one_bit = se_read_f(bs, 1);
    }

    CurrMbAddr = slice->header.first_mb_in_slice * (1 + slice->MbaffFrameFlag);

    do {
        if ((SLICE_TYPE_I != header->slice_type) && (SLICE_TYPE_SI != header->slice_type)) {
            if (!pps->entropy_coding_mode_flag) {
                data->mb_skip_run = se_read_ue(bs);
                prevMbSkipped = data->mb_skip_run > 0 ? 1 : 0;

                for (i = 0; i < data->mb_skip_run; i++) {
                    CurrMbAddr = NextMbAddress(CurrMbAddr);
                }

                if (data->mb_skip_run > 0) {
                    moreDataFlag = more_rbsp_data(bs);
                }
            }
            else {
                data->mb_skip_flag = se_read_ae(bs);
                moreDataFlag = !data->mb_skip_flag;
            }
        }

        if (moreDataFlag) {
            if (slice->MbaffFrameFlag && ((CurrMbAddr % 2 == 0) || ((CurrMbAddr % 2 == 1) && (prevMbSkipped)))) {
                data->mb_field_decoding_flag = se_read_ae(bs); // TBD
            }

            macroblock_layer();
        }

        if (!pps->entropy_coding_mode_flag) {
            moreDataFlag = more_rbsp_data(bs);
        }
        else {
            if ((SLICE_TYPE_I != header->slice_type) && (SLICE_TYPE_SI != header->slice_type)) {
                prevMbSkipped = data->mb_skip_flag;
            }

            if (slice->MbaffFrameFlag && (CurrMbAddr % 2 == 0)) {
                moreDataFlag = 1;
            }
            else {
                data->end_of_slice_flag = se_read_ae(bs);
                moreDataFlag = !data->end_of_slice_flag;
            }
        }

        CurrMbAddr = NextMbAddress(CurrMbAddr);

    } while (moreDataFlag);
}

// just process I frame, to make it clear, then add more
int slice_layer_without_partitioning_rbsp_parse(struct slice_layer_without_partitioning_rbsp *slice, unsigned char* buffer, unsigned int length)
{
    struct bitstream* bs;
    struct h264_context* ctx;

    ctx = h264_context_get();

	bs = bitstream_malloc(buffer, length);
    if (NULL == bs)
        return -1;

    ctx->bs = bs;
    ctx->slice = slice;

    slice_header(bs, &slice->header);
    slice_data();

    bitstream_free(bs);

	return 0;
}
