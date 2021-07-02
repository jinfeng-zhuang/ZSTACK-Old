#include <zstack/codec.h>

static struct slice_layer_without_partitioning_rbsp slice;

// just process I frame, to make it clear, then add more
struct slice_layer_without_partitioning_rbsp *slice_layer_without_partitioning_rbsp_parse(unsigned char* buffer, unsigned int length)
{
	bitstream_init(buffer, length);

	memset(&slice, 0, sizeof(struct slice_layer_without_partitioning_rbsp));

	slice.header.first_mb_in_slice = ue();

	slice.header.slice_type = ue();
	slice.header.slice_type = slice.header.slice_type % 5;

	slice.header.pic_parameter_set_id = ue();

    if (sps.separate_colour_plane_flag) {
        slice.header.colour_plane_id = u(2);
    }

	slice.header.frame_num = u(sps.log2_max_frame_num_minus4 + 4);

    if (!sps.frame_mbs_only_flag) {
        slice.header.field_pic_flag = u(1);
        if (slice.header.field_pic_flag) {
            slice.header.bottom_field_flag = u(1);
        }
    }

    if (IdrPicFlag)
        slice.header.idr_pic_id = ue();

	if (0 == sps.pic_order_cnt_type) {
		slice.header.pic_order_cnt_lsb = u(sps.log2_max_pic_order_cnt_lsb_minus4 + 4);
		if (pps.bottom_field_pic_order_in_frame_present_flag && (!slice.header.field_pic_flag))
			slice.header.delta_pic_order_cnt_bottom = se();
	}

    //----------------------------------
    // BELOW TO BE DONE
    //----------------------------------

    slice.header.dec_ref_pic_marking.adaptive_ref_pic_marking_mode_flag = u(1);

    slice.header.slice_qp_delta = se();

    if (pps.deblocking_filter_control_present_flag) {
        slice.header.disable_deblocking_filter_idc = ue();

        if (1 != slice.header.disable_deblocking_filter_idc) {
            slice.header.slice_alpha_c0_offset_div2 = se();
            slice.header.slice_beta_offset_div2 = se();
        }
    }

    // Slice Data
    if (pps.entropy_coding_mode_flag) {
        while (!byte_aligned()) {
            slice.data.cabac_alignment_one_bit = u(1);
        }
    }

    int MbaffFrameFlag = 0; // TODO

    //unsigned int CurrMbAddr = slice.header.first_mb_in_slice * (1 + MbaffFrameFlag);
    int moreDataFlag = 1;
    //int prevMbSkipped = 0;

#if 0
    do {
        if (moreDataFlag) {
            // macroblock_layer()
            slice.data.mb.mb_type = ue();
            if (I_PCM == slice.data.mb.mb_type) {

            }
            else {
                if (slice.data.mb.mb_type != I_NxN) {

                }
                else {
                    if (pps.transform_8x8_mode_flag && slice.data.mb.mb_type == I_NxN) {
                        slice.data.mb.transform_size_8x8_flag = u(1);
                    }

                    // mb_pred()
                }

                //if (1) {
                    //slice.data.mb.coded_block_pattern = me(); // me ?
                //}
            }
        }

        if (!pps.entropy_coding_mode_flag) {
            //
        }
        else {
            // 
            //slice.data.end_of_slice_flag = ae();
        }

        //CurrMbAddr = NextMbAddress(CurrMbAddr);
    } while (moreDataFlag);
#endif

	return &slice;
}
