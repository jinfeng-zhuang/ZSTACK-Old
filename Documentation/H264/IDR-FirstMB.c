// 以第一个 I 帧第一个 MB 为例描述程序流程
// 2021-4-7, V2
// 2021-4-8, V3

decode_one_frame

    UseParameterSet // slice header
    init_picture
        setup_buffers
        if(p_Inp->conceal_mode !=0)
            conceal_lost_frames(p_Dpb, currSlice);
        decode_poc(p_Vid, currSlice);
        dec_picture->xxx = xxx;
        ...

    while(current_header != SOP && current_header !=EOS)
        ppSliceList[p_Vid->iSliceNumOfCurrPic] = malloc_slice(p_Inp, p_Vid);
        current_header = read_new_slice(currSlice);
            for (;;)
                read_next_nalu
                case NALU_TYPE_IDR:
                    setup_slice_methods(currSlice);
                    arideco_start_decoding
                case NALU_TYPE_PPS:
                    ProcessPPS
                case NALU_TYPE_SPS:
                    ProcessSPS
    init_picture_decoding
        UseParameterSet
        fmo_init
    
    for(iSliceNo=0; iSliceNo<p_Vid->iSliceNumOfCurrPic; iSliceNo++)
    
        currSlice = ppSliceList[iSliceNo];
        
        init_slice
            currSlice->init_lists (currSlice);
            reorder_lists
            
        decode_slice
            decode_one_slice
                while (end_of_slice == FALSE) {
                    start_macroblock
                    
                    currSlice->read_one_macroblock = read_one_macroblock_i_slice_cavlc {
                        currSlice->interpret_mb_mode(currMB);
                            currMB->mb_type = I4MB;
                        else if (currMB->mb_type == I4MB)
                            read_intra4x4_macroblock_cavlc(currMB, partMap)
                                currMB->luma_transform_size_8x8_flag = FALSE
                                init_macroblock(currMB)
                                    set_read_comp_coeff_cavlc
                                        currMB->read_comp_coeff_4x4_CAVLC = read_comp_coeff_4x4_CAVLC
                                read_ipred_modes(currMB)
                                    // luma pred mode
                                    read_ipred_4x4_modes
                                        for (0 ... 4)
                                            for (0 ... 2)
                                                for (0 .. 2)
                                                    readSyntaxElement_Intra4x4PredictionMode
                                                    get4x4Neighbour // up
                                                    get4x4Neighbour // left
                                                    mostProbableIntraPredMode = DC_PRED | upIntraPredMode | leftIntraPredMode
                                                    currSlice->ipredmode[bj][bi] = mostProbableIntraPredMode | ...
                                    
                                    // chroma pred mode
                                    dP->readSyntaxElement(currMB, &currSE, dP);
                                    currMB->c_ipred_mode = (char) currSE.value1;
                                    
                                currSlice->read_CBP_and_coeffs_from_NAL (currMB)
                                
                                    // CBP
                                    currMB->cbp = cbp = currSE.value1;
                                    
                                    // DQUANT
                                    if (cbp !=0)
                                        read_delta_quant
                                            currMB->delta_quant = (short) currSE->value1;
                                            currSlice->qp = ...
                                            update_qp(currMB, currSlice->qp);
                                    
                                    // Luma DC
                                    if (cbp)
                                        currMB->read_comp_coeff_4x4_CAVLC = read_comp_coeff_4x4_CAVLC
                                            currSlice->read_coeff_4x4_CAVLC = read_coeff_4x4_CAVLC
                                                case LUMA:
                                                    ...
                                                readSyntaxElement_NumCoeffTrailingOnes
                                                p_Vid->nz_coeff[mb_nr][0][j][i] = (byte) numcoeff;
                                                for (...)
                                                    levarr[k] = (code>>ntr)&1 ? -1 : 1;
                                                currSlice->cof[pl][j + j0][i + i0]= ...
                                                
                                    // CHROMA DC
                                    if (cbp > 15)
                                        currSlice->read_coeff_4x4_CAVLC
                                    
                                    // CHROMA AC
                                    if (cbp > 31)
                                        currSlice->read_coeff_4x4_CAVLC
                    }
                                        
                    decode_one_macroblock {
                        currSlice->decode_one_component = decode_one_component_i_slice
                            else if (currMB->mb_type == I4MB)
                                mb_pred_intra4x4(currMB, curr_plane, currImg, dec_picture);
                                
                                    // LUMA decoding
                                    for (0 ... 4)
                                        for (...)
                                            currSlice->intra_pred_4x4 = intra_pred_4x4_normal
                                                case DC_PRED:
                                                    intra4x4_dc_pred
                                                        getNonAffNeighbour // up
                                                        getNonAffNeighbour // left
                                                        for (...)
                                                            mb_pred[][] = ...
                                            currMB->itrans_4x4 = itrans4x4
                                                inverse4x4
                                                    ... // IDCT algo of 4x4 block
                                                sample_reconstruct(*output, *pred, *rres, dq_bits)
                                                    for (...)
                                                        *output++ = iClip1(rres++ << dq_bits, pred++)
                                            copy_image_data_4x4
                                                memcpy
                                    
                                    // CHROMA decoding
                                    intra_cr_decoding
                                        ...
                    }
                    
                    exit_macroblock
                        nal_startcode_follows
                }
            
    exit_picture
        DeblockPicture
        field_postprocessing
        store_picture_in_dpb
        