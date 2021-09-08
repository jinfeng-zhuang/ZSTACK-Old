#include <stdio.h>
#include <zstack/codec.h>
#include <zstack/log.h>
#include "codec.h"
#include "sps.h"

static void hrd_parameters(struct bitstream* bs, struct hrd_parameters* hrd)
{
	u32 SchedSelIdx;

	hrd->cpb_cnt_minus1 = se_read_ue(bs);
	hrd->bit_rate_scale = se_read_u(bs, 4);
	hrd->cpb_size_scale = se_read_u(bs, 4);
	
	for (SchedSelIdx = 0; SchedSelIdx <= hrd->cpb_cnt_minus1; SchedSelIdx++) {
		hrd->bit_rate_value_minus1[SchedSelIdx] = se_read_ue(bs);
		hrd->cpb_size_value_minus1[SchedSelIdx] = se_read_ue(bs);
		hrd->cbr_flag[SchedSelIdx] = se_read_u(bs, 1);
	}

	hrd->initial_cpb_removal_delay_length_minus1 = se_read_u(bs, 5);
	hrd->cpb_removal_delay_length_minus1 = se_read_u(bs, 5);
	hrd->dpb_output_delay_length_minus1 = se_read_u(bs, 5);
	hrd->time_offset_length = se_read_u(bs, 5);
}

static void vui_parameters(struct bitstream* bs, struct vui_parameters* vui)
{
	vui->aspect_ratio_info_present_flag = se_read_u(bs, 1);
	if (vui->aspect_ratio_info_present_flag) {
		vui->aspect_ratio_idc = se_read_u(bs, 8);
		if (vui->aspect_ratio_idc == Extended_SAR) {
			vui->sar_width = se_read_u(bs, 16);
			vui->sar_height = se_read_u(bs, 16);
		}
	}

	vui->overscan_info_present_flag = se_read_u(bs, 1);
	if (vui->overscan_info_present_flag)
		vui->overscan_appropriate_flag = se_read_u(bs, 1);

	vui->video_signal_type_present_flag = se_read_u(bs, 1);
	if (vui->video_signal_type_present_flag) {
		vui->video_format = se_read_u(bs, 3);
		vui->video_full_range_flag = se_read_u(bs, 1);
		vui->colour_description_present_flag = se_read_u(bs, 1);
		if (vui->colour_description_present_flag) {
			vui->colour_primaries = se_read_u(bs, 8);
			vui->transfer_characteristics = se_read_u(bs, 8);
			vui->matrix_coefficients = se_read_u(bs, 8);

			DEBUG("VUI: CP = %d TC = %d MC = %d\n",
				vui->colour_primaries,
				vui->transfer_characteristics,
				vui->matrix_coefficients);
		}
	}

	vui->chroma_loc_info_present_flag = se_read_u(bs, 1);
	if (vui->chroma_loc_info_present_flag) {
		vui->chroma_sample_loc_type_top_field = se_read_ue(bs);
		vui->chroma_sample_loc_type_bottom_field = se_read_ue(bs);
	}

	vui->timing_info_present_flag = se_read_u(bs, 1);
	if (vui->timing_info_present_flag) {
		vui->num_units_in_tick = se_read_u(bs, 32);
		vui->time_scale = se_read_u(bs, 32);
		vui->fixed_frame_rate_flag = se_read_u(bs, 1);
	}

	vui->nal_hrd_parameters_present_flag = se_read_u(bs, 1);
	if (vui->nal_hrd_parameters_present_flag) {
		hrd_parameters(bs, &vui->nal_hrd_parameters);
	}

	vui->vcl_hrd_parameters_present_flag = se_read_u(bs, 1);
	if (vui->vcl_hrd_parameters_present_flag) {
		hrd_parameters(bs, &vui->vlc_hrd_parameters);
	}

	if (vui->nal_hrd_parameters_present_flag || vui->vcl_hrd_parameters_present_flag)
		vui->low_delay_hrd_flag = se_read_u(bs, 1);

	vui->pic_struct_present_flag = se_read_u(bs, 1);
	vui->bitstream_restriction_flag = se_read_u(bs, 1);
	if (vui->bitstream_restriction_flag) {
		vui->motion_vectors_over_pic_boundaries_flag = se_read_u(bs, 1);
		vui->max_bytes_per_pic_denom = se_read_ue(bs);
		vui->max_bits_per_mb_denom = se_read_ue(bs);
		vui->log2_max_mv_length_horizontal = se_read_ue(bs);
		vui->log2_max_mv_length_vertical = se_read_ue(bs);
		vui->max_num_reorder_frames = se_read_ue(bs);
		vui->max_dec_frame_buffering = se_read_ue(bs);
	}
}

// refer to JM: InterpretSPS()
int sps_parse(struct seq_parameter_set_rbsp *sps, unsigned char* buffer, unsigned int length)
{
	int i;
	struct bitstream* bs;

	if ((NULL == sps) || (NULL == buffer) || (0 == length))
		return -1;

	bs = bitstream_malloc(buffer, length);
	if (NULL == bs)
		return -1;

	sps->data.profile_idc = se_read_u(bs, 8);
	sps->data.constraint_set0_flag = se_read_u(bs, 1);
	sps->data.constraint_set1_flag = se_read_u(bs, 1);
	sps->data.constraint_set2_flag = se_read_u(bs, 1);
	sps->data.constraint_set3_flag = se_read_u(bs, 1);
	sps->data.constraint_set4_flag = se_read_u(bs, 1);
	sps->data.constraint_set5_flag = se_read_u(bs, 1);
	sps->data.reserved_zero_2bits = se_read_u(bs, 2);
	sps->data.level_idc = se_read_u(bs, 8);
	sps->data.seq_parameter_set_id = se_read_ue(bs);

	if (sps->data.profile_idc == 100 || sps->data.profile_idc == 110 ||
		sps->data.profile_idc == 122 || sps->data.profile_idc == 244 || sps->data.profile_idc == 44 ||
		sps->data.profile_idc == 83 || sps->data.profile_idc == 86 || sps->data.profile_idc == 118 ||
		sps->data.profile_idc == 128 || sps->data.profile_idc == 138 || sps->data.profile_idc == 139 ||
		sps->data.profile_idc == 134 || sps->data.profile_idc == 135) {
		sps->data.chroma_format_idc = se_read_ue(bs);
		if (3 == sps->data.chroma_format_idc)
			sps->data.separate_colour_plane_flag = se_read_u(bs, 1);

		if (0 == sps->data.separate_colour_plane_flag) {
			sps->ChromaArrayType = sps->data.chroma_format_idc;
		}
		else {
			sps->ChromaArrayType = 0;
		}

		DEBUG("chroma format: %d\n", sps->data.chroma_format_idc);

		sps->data.bit_depth_luma_minus8 = se_read_ue(bs);
		sps->data.bit_depth_chroma_minus8 = se_read_ue(bs);
		sps->data.qpprime_y_zero_transform_bypass_flag = se_read_u(bs, 1);
		sps->data.seq_scaling_matrix_present_flag = se_read_u(bs, 1);
		if (sps->data.seq_scaling_matrix_present_flag) {
			DEBUG("scale list\n");
			for (i = 0; i < ((sps->data.chroma_format_idc != 3) ? 8 : 12); i++) {
				sps->data.seq_scaling_list_present_flag[i] = se_read_u(bs, 1);
				if (sps->data.seq_scaling_list_present_flag[i]) {
					if (i < 6)
						scaling_list(bs, sps->tmp.ScalingList4x4[i], 16, &sps->tmp.UseDefaultScalingMatrix4x4Flag[i]);
					else
						scaling_list(bs, sps->tmp.ScalingList8x8[i-6], 64, &sps->tmp.UseDefaultScalingMatrix8x8Flag[i-6]);
				}
			}
		}
	}

	sps->data.log2_max_frame_num_minus4 = se_read_ue(bs);
	DEBUG("MaxFrameNum = %d\n", 1 << (sps->data.log2_max_frame_num_minus4 + 4));

	sps->data.pic_order_cnt_type = se_read_ue(bs);

	if (sps->data.pic_order_cnt_type == 0) {
		sps->data.log2_max_pic_order_cnt_lsb_minus4 = se_read_ue(bs);
		DEBUG("MaxPicOrderCntLsb = %d\n", 1 << (sps->data.log2_max_pic_order_cnt_lsb_minus4 + 4));
	}
	else if (sps->data.pic_order_cnt_type == 1) {
		sps->data.delta_pic_order_always_zero_flag = se_read_u(bs, 1);
		sps->data.offset_for_non_ref_pic = se_read_se(bs);
		sps->data.offset_for_top_to_bottom_field = se_read_se(bs);
		sps->data.num_ref_frames_in_pic_order_cnt_cycle = se_read_ue(bs);
		for (i = 0; i < (int)sps->data.num_ref_frames_in_pic_order_cnt_cycle; i++)
			sps->data.offset_for_ref_frame[i] = se_read_se(bs);
	}

	sps->data.max_num_ref_frames = se_read_ue(bs);
	sps->data.gaps_in_frame_num_value_allowed_flag = se_read_u(bs, 1);
	sps->data.pic_width_in_mbs_minus1 = se_read_ue(bs);
	sps->data.pic_height_in_map_units_minus1 = se_read_ue(bs);
	DEBUG("%dx%d\n", (sps->data.pic_width_in_mbs_minus1 + 1) * 16, (sps->data.pic_height_in_map_units_minus1 + 1) * 16);

	sps->PicWidthInMbs = sps->data.pic_width_in_mbs_minus1 + 1;
        sps->PicHeightInMapUnits = sps->data.pic_height_in_map_units_minus1 + 1;
        sps->PicSizeInMapUnits   = sps->PicWidthInMbs * sps->PicHeightInMapUnits;

	sps->data.frame_mbs_only_flag = se_read_u(bs, 1);
	if (!sps->data.frame_mbs_only_flag)
		sps->data.mb_adaptive_frame_field_flag = se_read_u(bs, 1);

	sps->FrameHeightInMbs = (2 - sps->data.frame_mbs_only_flag) * sps->PicHeightInMapUnits;

	sps->data.direct_8x8_inference_flag = se_read_u(bs, 1);
	sps->data.frame_cropping_flag = se_read_u(bs, 1);
	if (sps->data.frame_cropping_flag) {
		sps->data.frame_crop_left_offset = se_read_ue(bs);
		sps->data.frame_crop_right_offset = se_read_ue(bs);
		sps->data.frame_crop_top_offset = se_read_ue(bs);
		sps->data.frame_crop_bottom_offset = se_read_ue(bs);
		DEBUG("crop info: L %d R %d T %d B %d\n",
			sps->data.frame_crop_left_offset,
			sps->data.frame_crop_right_offset,
			sps->data.frame_crop_top_offset,
			sps->data.frame_crop_bottom_offset);
	}

	sps->data.vui_parameters_present_flag = se_read_u(bs, 1);

	if (sps->data.vui_parameters_present_flag) {
		DEBUG("VUI exist\n");
		vui_parameters(bs, &sps->data.vui);
	}

	bitstream_free(bs);

	return 0;
}
