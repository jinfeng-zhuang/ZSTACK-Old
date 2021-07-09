#include "sps.h"
#include <stdio.h>
#include <zstack/log.h>

#define DATA_FORMAT1	"  %-80s | %d\n"
#define DATA_FORMAT2	"    %-78s | %d\n"
#define DATA_FORMAT3	"      %-76s | %d\n"
#define DATA_FORMAT4	"        %-74s | %d\n"

void sps_dump(struct seq_parameter_set_rbsp* sps)
{
	if (NULL == sps)
		return;

	DEBUG("seq_parameter_set_data( ) {\n");
	DEBUG(DATA_FORMAT1, "profile_idc", 1);
	DEBUG(DATA_FORMAT1, "constraint_set0_flag", 1);
	DEBUG(DATA_FORMAT1, "constraint_set1_flag", 1);
	DEBUG(DATA_FORMAT1, "constraint_set2_flag", 1);
	DEBUG(DATA_FORMAT1, "constraint_set3_flag", 1);
	DEBUG(DATA_FORMAT1, "constraint_set4_flag", 1);
	DEBUG(DATA_FORMAT1, "constraint_set5_flag", 1);
	DEBUG(DATA_FORMAT1, "reserved_zero_2bits", 1);
	DEBUG(DATA_FORMAT1, "level_idc", 1);
	DEBUG(DATA_FORMAT1, "seq_parameter_set_id", 1);

	DEBUG("  if( profile_idc = = 100 || profile_idc = = 110 ||\n");
	DEBUG("    profile_idc = = 122 || profile_idc = = 244 || profile_idc = = 44 ||\n");
	DEBUG("    profile_idc = = 83 || profile_idc = = 86 || profile_idc = = 118 ||\n");
	DEBUG("    profile_idc = = 128 || profile_idc = = 138 || profile_idc = = 139 ||\n");
	DEBUG("    profile_idc = = 134 || profile_idc = = 135 ) {\n");
	DEBUG(DATA_FORMAT2, "chroma_format_idc", 1);

	DEBUG("  if( chroma_format_idc = = 3 )\n");
	DEBUG(DATA_FORMAT3, "separate_colour_plane_flag", 1);

	DEBUG(DATA_FORMAT2, "bit_depth_luma_minus8", 1);
	DEBUG(DATA_FORMAT2, "bit_depth_chroma_minus8", 1);
	DEBUG(DATA_FORMAT2, "qpprime_y_zero_transform_bypass_flag", 1);
	DEBUG(DATA_FORMAT2, "seq_scaling_matrix_present_flag", 1);

	DEBUG("  if( seq_scaling_matrix_present_flag )\n");
	DEBUG("    for( i = 0; i < ( ( chroma_format_idc != 3 ) ? 8 : 12 ); i++ ) {\n");

	DEBUG(DATA_FORMAT4, "seq_scaling_list_present_flag[i]", 1);

	DEBUG(DATA_FORMAT1, "log2_max_frame_num_minus4", 1);
	DEBUG(DATA_FORMAT1, "pic_order_cnt_type", 1);

	DEBUG("  if( pic_order_cnt_type = = 0 )\n");
	DEBUG(DATA_FORMAT2, "log2_max_pic_order_cnt_lsb_minus4", 1);

	DEBUG("  else if( pic_order_cnt_type = = 1 ) {\n");
	DEBUG(DATA_FORMAT2, "delta_pic_order_always_zero_flag", 1);
	DEBUG(DATA_FORMAT2, "offset_for_non_ref_pic", 1);
	DEBUG(DATA_FORMAT2, "offset_for_top_to_bottom_field", 1);
	DEBUG(DATA_FORMAT2, "num_ref_frames_in_pic_order_cnt_cycle", 1);

	DEBUG(DATA_FORMAT1, "max_num_ref_frames", 1);
	DEBUG(DATA_FORMAT1, "gaps_in_frame_num_value_allowed_flag", 1);
	DEBUG(DATA_FORMAT1, "pic_width_in_mbs_minus1", 1);
	DEBUG(DATA_FORMAT1, "pic_height_in_map_units_minus1", 1);
	DEBUG(DATA_FORMAT1, "frame_mbs_only_flag", 1);

	DEBUG("  if( !frame_mbs_only_flag )\n");
	DEBUG(DATA_FORMAT2, "mb_adaptive_frame_field_flag", 1);

	DEBUG(DATA_FORMAT1, "direct_8x8_inference_flag", 1);
	DEBUG(DATA_FORMAT1, "frame_cropping_flag", 1);

	DEBUG("  if( frame_cropping_flag ) {\n");
	DEBUG(DATA_FORMAT2, "frame_crop_left_offset", 1);
	DEBUG(DATA_FORMAT2, "frame_crop_right_offset", 1);
	DEBUG(DATA_FORMAT2, "frame_crop_top_offset", 1);
	DEBUG(DATA_FORMAT2, "frame_crop_bottom_offset", 1);

	DEBUG(DATA_FORMAT1, "vui_parameters_present_flag", 1);

	DEBUG("  if( vui_parameters_present_flag )\n");
	DEBUG("    vui_parameters( )\n");
}
