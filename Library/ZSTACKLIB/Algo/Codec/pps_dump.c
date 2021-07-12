#include "sps.h"
#include <stdio.h>
#include <zstack/log.h>

#define DATA_FORMAT1	"  %-90s | %d\n"
#define DATA_FORMAT2	"    %-88s | %d\n"
#define DATA_FORMAT3	"      %-86s | %d\n"
#define DATA_FORMAT4	"        %-84s | %d\n"

void pps_dump(struct pic_parameter_set_rbsp* pps)
{
	if (NULL == pps)
		return;

	DEBUG("pic_parameter_set_rbsp() {\n");
	DEBUG(DATA_FORMAT1, "pic_parameter_set_id", 1);
	DEBUG(DATA_FORMAT1, "seq_parameter_set_id", 1);
	DEBUG(DATA_FORMAT1, "entropy_coding_mode_flag", 1);
	DEBUG(DATA_FORMAT1, "bottom_field_pic_order_in_frame_present_flag", 1);
	DEBUG(DATA_FORMAT1, "num_slice_groups_minus1", 1);
	DEBUG("  if (num_slice_groups_minus1 > 0) {\n");
	DEBUG(DATA_FORMAT2, "slice_group_map_type", 1);
	DEBUG("    if( slice_group_map_type = = 0 )\n");
	DEBUG("      for( iGroup = 0; iGroup <= num_slice_groups_minus1; iGroup++ )\n");
	DEBUG(DATA_FORMAT4, "run_length_minus1[ iGroup ]", 1);
	DEBUG("    else if( slice_group_map_type = = 2 )\n");
	DEBUG("      for( iGroup = 0; iGroup < num_slice_groups_minus1; iGroup++ ) {\n");
	DEBUG(DATA_FORMAT4, "top_left[ iGroup ]", 1);
	DEBUG(DATA_FORMAT4, "bottom_right[ iGroup ]", 1);
	DEBUG("    else if( slice_group_map_type = = 3 ||\n"
		  "             slice_group_map_type = = 4 ||\n"
		  "             slice_group_map_type = = 5 ) {\n");
	DEBUG(DATA_FORMAT3, "slice_group_change_direction_flag", 1);
	DEBUG(DATA_FORMAT3, "slice_group_change_rate_minus1", 1);
	DEBUG("    else if( slice_group_map_type = = 6 ) {\n");
	DEBUG(DATA_FORMAT3, "pic_size_in_map_units_minus1", 1);
	DEBUG("      for( i = 0; i <= pic_size_in_map_units_minus1; i++ )\n");
	DEBUG(DATA_FORMAT4, "slice_group_id[ i ]", 1);

	DEBUG("  if( profile_idc = = 100 || profile_idc = = 110 ||\n");
	DEBUG("    profile_idc = = 122 || profile_idc = = 244 || profile_idc = = 44 ||\n");
	DEBUG("    profile_idc = = 83 || profile_idc = = 86 || profile_idc = = 118 ||\n");
	DEBUG("    profile_idc = = 128 || profile_idc = = 138 || profile_idc = = 139 ||\n");
	DEBUG("    profile_idc = = 134 || profile_idc = = 135 ) {\n");
	DEBUG(DATA_FORMAT2, "chroma_format_idc", 1);

	DEBUG(DATA_FORMAT1, "num_ref_idx_l0_default_active_minus1", 1);
	DEBUG(DATA_FORMAT1, "num_ref_idx_l1_default_active_minus1", 1);
	DEBUG(DATA_FORMAT1, "weighted_pred_flag", 1);
	DEBUG(DATA_FORMAT1, "weighted_bipred_idc", 1);
	DEBUG(DATA_FORMAT1, "pic_init_qp_minus26 /* relative to 26 */", 1);
	DEBUG(DATA_FORMAT1, "pic_init_qs_minus26 /* relative to 26 */", 1);
	DEBUG(DATA_FORMAT1, "chroma_qp_index_offset", 1);
	DEBUG(DATA_FORMAT1, "deblocking_filter_control_present_flag", 1);
	DEBUG(DATA_FORMAT1, "constrained_intra_pred_flag", 1);
	DEBUG(DATA_FORMAT1, "redundant_pic_cnt_present_flag", 1);
	DEBUG("  if( more_rbsp_data( ) ) {\n");
	DEBUG(DATA_FORMAT2, "transform_8x8_mode_flag", 1);
	DEBUG(DATA_FORMAT2, "pic_scaling_matrix_present_flag", 1);
	DEBUG("    if( pic_scaling_matrix_present_flag )\n");
	DEBUG("      for( i = 0;\n"
		  "           i < 6 +( ( chroma_format_idc != 3 ) ? 2 : 6 ) * transform_8x8_mode_flag;\n"
		  "           i++ ) {\n");
	DEBUG(DATA_FORMAT4, "pic_scaling_list_present_flag[ i ]", 1);
	DEBUG("        if( pic_scaling_list_present_flag[ i ] )\n");
	DEBUG("          if( i < 6 )\n");
	DEBUG("            scaling_list(ScalingList4x4[i], 16, UseDefaultScalingMatrix4x4Flag[i] )\n");
	DEBUG("          else\n");
	DEBUG("            scaling_list(ScalingList8x8[i-6], 64, UseDefaultScalingMatrix4x4Flag[i-6] )\n");
	DEBUG(DATA_FORMAT2, "second_chroma_qp_index_offset", 1);
	DEBUG("  rbsp_trailing_bits()\n");
}
