#ifndef _ZSTACK_CODEC_
#define _ZSTACK_CODEC_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pic_parameter_set_rbsp.h"
#include "sps.h"

enum nal_unit_type {
	NALU_UNDEF = 0,
	NALU_SLICE = 1,
	NALU_SLICE_A,
	NALU_SLICE_B,
	NALU_SLICE_C,
	NALU_SLICE_IDR,
	NALU_SEI = 6,
	NALU_SPS = 7,
	NALU_PPS = 8,
	NALU_SEQ_END = 9,
	NALU_SEQ_END2 = 10,
	NALU_UNKNOWN,
	NALU_RSVD12,
	NALU_RSVD13,
	NALU_RSVD14,
	NALU_RSVD15,
	NALU_RSVD16,
	NALU_RSVD17,
	NALU_RSVD18,
	NALU_RSVD19,
	NALU_RSVD20,
	NALU_RSVD21,
	NALU_RSVD22,
	NALU_RSVD23,
	NALU_RSVD24,
	NALU_RSVD25,
	NALU_RSVD26,
	NALU_RSVD27,
	NALU_RSVD28,
	NALU_RSVD29,
	NALU_RSVD30,
	NALU_RSVD31,
	NALU_TYPE_MAX = 64
};

struct nalu {
    unsigned int prefix;
    unsigned char *buffer;
    unsigned int length;
	enum nal_unit_type type;
};

struct bitstream_s {
	unsigned int bitpos;
	unsigned char *bits;
};


enum slice_type {
	SLICE_P,
	SLICE_B,
	SLICE_I,
	SLICE_SP,
	SLICE_SI
};

struct ref_pic_list_modification {
	unsigned int ref_pic_list_modification_flag_l0;
	unsigned int modification_of_pic_nums_idc;
	unsigned int abs_diff_pic_num_minus1;
	unsigned int long_term_pic_num;

	unsigned int ref_pic_list_modification_flag_l1;
};

struct pred_weight_table {
	unsigned int luma_log2_weight_denom;
	unsigned int chroma_log2_weight_denom;

	unsigned int luma_weight_l0_flag;
	unsigned int luma_weight_l0[1]; // ?
	unsigned int luma_offset_l0[1]; // ?
	unsigned int chroma_weight_l0_flag;
	unsigned int chroma_weight_l0[1][2];
	unsigned int chroma_offset_l0[1][2];

	unsigned int luma_weight_l1_flag;
	unsigned int luma_weight_l1[1]; // ?
	unsigned int luma_offset_l1[1]; // ?
	unsigned int chroma_weight_l1_flag;
	unsigned int chroma_weight_l1[1][2];
	unsigned int chroma_offset_l1[1][2];
};

struct dec_ref_pic_marking {
	unsigned int no_output_of_prior_pics_flag;
	unsigned int long_term_reference_flag;
	unsigned int adaptive_ref_pic_marking_mode_flag;
	unsigned int memory_management_control_operation;
	unsigned int difference_of_pic_nums_minus1;
	unsigned int long_term_pic_num;
	unsigned int long_term_frame_idx;
	unsigned int max_long_term_frame_idx_plus1;
};

struct slice_header {
	unsigned int first_mb_in_slice;
	unsigned int slice_type;
	unsigned int pic_parameter_set_id;
    unsigned int colour_plane_id;
	unsigned int frame_num;
	unsigned int field_pic_flag;
	unsigned int bottom_field_flag;
	unsigned int idr_pic_id;
	unsigned int pic_order_cnt_lsb;
	unsigned int delta_pic_order_cnt_bottom;
	unsigned int delta_pic_order_cnt[2];
	unsigned int redundant_pic_cnt;
	unsigned int direct_spatial_mv_pred_flag;
	unsigned int num_ref_idx_active_override_flag;
	unsigned int num_ref_idx_10_active_minus1;
	unsigned int num_ref_idx_11_active_minus1;
	// ref_pic_list_reordering
	// pred_weight_table
    struct dec_ref_pic_marking dec_ref_pic_marking;
	unsigned int cabac_init_idc;
	unsigned int slice_qp_delta;
	unsigned int sp_for_switch_flag;
	unsigned int slice_qs_delta;
	unsigned int disable_deblocking_filter_idc;
	unsigned int slice_alpha_c0_offset_div2;
	unsigned int slice_beta_offset_div2;
	unsigned int slice_group_change_cycle;
};

struct mb_pred {
	unsigned int prev_intra4x4_pred_mode_flag[16];
	unsigned int rem_intra4x4_pred_mode[16];
	unsigned int prev_intra8x8_pred_mode_flag[4];
	unsigned int rem_intra8x8_pred_mode[4];
	unsigned int intra_chroma_pred_mode;
	unsigned int ref_idx_l0[1]; // ?
	unsigned int ref_idx_l1[1]; // ?
	unsigned int mvd_l0[1][1][2]; // ?
	unsigned int mvd_l1[1][1][2]; // ?
};

struct sub_mb_pred {
	unsigned int sub_mb_type[4];
	unsigned int ref_idx_l0[1]; // ?
	unsigned int ref_idx_l1[1]; // ?
	unsigned int mvd_l0[1][1][2]; // ?
	unsigned int mvd_l1[1][1][2]; // ?
};

struct residual {
    int x;
};

struct residual_luma {
    int x;
};

struct residual_block_cavlc {
	unsigned int coeff_token;
	unsigned int trailing_ones_sign_flag;
	unsigned int level_prefix;
	unsigned int level_suffix;
	unsigned int total_zeros;
	unsigned int run_before;
};

struct residual_block_cabac {
	unsigned int coded_block_flag;
	unsigned int significant_coeff_flag;
	unsigned int last_significant_coeff_flag;
	unsigned int coeff_abs_level_minus1[1]; // ?
	unsigned int coeff_sign_flag[1]; // ?
};

struct macroblock_layer {
	unsigned int mb_type;
	unsigned int pcm_alignment_zero_bit;
	unsigned int pcm_sample_luma[256];
	unsigned int pcm_sample_chroma[256];
	unsigned int transform_size_8x8_flag;
	unsigned int coded_block_pattern;
	unsigned int mb_qp_delta;
};

struct slice_data {
	unsigned int cabac_alignment_one_bit;
	unsigned int mb_skip_run;
	unsigned int mb_skip_flag;
	unsigned int mb_field_decoding_flag;
	unsigned int end_of_slice_flag;

    struct macroblock_layer mb;
};

struct slice_layer_without_partitioning_rbsp {
	struct slice_header header;
	struct slice_data data;
};

enum {
    I_NxN = 0,
    I_PCM = 25,
};

typedef void (*nalu_callback)(unsigned char *buffer, unsigned int length);

extern nalu_callback g_nalu_callback[];

extern struct bitstream_s bitstream;

extern int IdrPicFlag;

// keep nalu in global array, for debug version, as I can dump it
// in release version, nalu_parse should return nalu structure
// return nalu count
extern int nalu_parse_all(unsigned char *content, unsigned int file_size);

extern int nalu_next(enum nal_unit_type *type, unsigned char **buffer, unsigned int *length);

extern int bitstream_init(unsigned char *buffer, unsigned int length);
extern unsigned int u(unsigned int num);
extern unsigned int ue(void);
extern int se(void);
extern int more_rbsp_data(void);
extern int rbsp_trailing_bits(void);
extern int bitstream_error(void);
extern int byte_aligned(void);
extern struct nalu * nalu_get(int *index);

extern struct sps *sps_proc(unsigned char *buffer, unsigned int length);
extern int pic_pararmeter_set_rbsp_parse(unsigned char* buffer, unsigned int length);
extern struct slice_layer_without_partitioning_rbsp *slice_layer_without_partitioning_rbsp_parse(unsigned char* buffer, unsigned int length);

#endif
