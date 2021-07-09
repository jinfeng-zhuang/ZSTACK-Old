#include <stdio.h>
#include <zstack/codec.h>
#include <zstack/log.h>
#include "codec.h"
#include "pps.h"
#include "sps.h"

int pps_parse(struct pic_parameter_set_rbsp* pps, unsigned char* buffer, unsigned int length)
{
	unsigned int i;
	struct bitstream* bs;
	struct seq_parameter_set_rbsp* sps = NULL;
	unsigned int iGroup;

	if ((NULL == pps) || (NULL == buffer) || (0 == length))
		return -1;

	bs = bitstream_malloc(buffer, length);
	if (NULL == bs)
		return -1;

	pps->pic_parameter_set_id = se_read_ue(bs);
	pps->seq_parameter_set_id = se_read_ue(bs);

	sps = sps_get(pps->seq_parameter_set_id);

	pps->entropy_coding_mode_flag = se_read_u(bs, 1);
	pps->bottom_field_pic_order_in_frame_present_flag = se_read_u(bs, 1);
	pps->num_slice_groups_minus1 = se_read_ue(bs);
	if (pps->num_slice_groups_minus1) {
		pps->slice_group_map_type = se_read_ue(bs);
		if (0 == pps->slice_group_map_type) {
			for (iGroup = 0; iGroup <= pps->num_slice_groups_minus1; iGroup++)
				pps->run_length_minus1[iGroup] = se_read_ue(bs);
		}
		else if (2 == pps->slice_group_map_type) {
			for (iGroup = 0; iGroup <= pps->num_slice_groups_minus1; iGroup++) {
				pps->top_left[iGroup] = se_read_ue(bs);
				pps->bottom_right[iGroup] = se_read_ue(bs);
			}
		}
		else if ((3 == pps->slice_group_map_type) ||
			(4 == pps->slice_group_map_type) ||
			(5 == pps->slice_group_map_type)) {
			pps->slice_group_change_direction_flag = se_read_u(bs, 1);
			pps->slice_group_change_rate_minus1 = se_read_ue(bs);
		}
		else if (6 == pps->slice_group_map_type) {
			pps->pic_size_in_map_units_minus1 = se_read_ue(bs);
			for (i = 0; i < pps->pic_size_in_map_units_minus1; i++)
				pps->size_group_id[i] = se_read_ue(bs);
		}
	}

	pps->num_ref_idx_10_default_active_minus1 = se_read_ue(bs);
	pps->num_ref_idx_11_default_active_minus1 = se_read_ue(bs);
	pps->weighted_pred_flag = se_read_u(bs, 1);
	pps->weighted_bipred_idc = se_read_u(bs, 2);
	pps->pic_init_qp_minus26 = se_read_se(bs);
	pps->pic_init_qs_minus26 = se_read_se(bs);
	pps->chroma_qp_index_offset = se_read_se(bs);
	pps->deblocking_filter_control_present_flag = se_read_u(bs, 1);
	pps->constrained_intra_pred_flag = se_read_u(bs, 1);
	pps->redundant_pic_cnt_present_flag = se_read_u(bs, 1);

	if (more_rbsp_data()) {
		pps->transform_8x8_mode_flag = se_read_u(bs, 1);
		pps->pic_scaling_matrix_present_flag = se_read_u(bs, 1);
		if (pps->pic_scaling_matrix_present_flag) {
			for (i = 0; i < 6 + (((sps->data.chroma_format_idc != 3) ? 2 : 6) & pps->transform_8x8_mode_flag); i++) {
				pps->pic_scaling_list_present_flag[i] = se_read_u(bs, 1);
				if (pps->pic_scaling_list_present_flag[i]) {
					if (i < 6)
						scaling_list(bs, pps->scale.ScalingList4x4[i], 16, &pps->scale.UseDefaultScalingMatrix4x4Flag[i]);
					else
						scaling_list(bs, pps->scale.ScalingList8x8[i - 6], 64, &pps->scale.UseDefaultScalingMatrix8x8Flag[i - 6]);
				}
			}
			pps->second_chroma_qp_index_offset = se_read_se(bs);
		}
	}
	rbsp_trailing_bits();

	bitstream_free(bs);

	return 0;
}