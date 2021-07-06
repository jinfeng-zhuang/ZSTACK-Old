#ifndef AVMIPS_H
#define AVMIPS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////

#define GET_VIDEO_DEBUG_INFO_BASE	 0xBADBAD00

#define GET_VIDEO_DEBUG_INFO_AVSYNC	(GET_VIDEO_DEBUG_INFO_BASE + 1)
#define GET_VIDEO_DEBUG_INFO_FORMAT	(GET_VIDEO_DEBUG_INFO_BASE + 2)
#define GET_VIDEO_DEBUG_INFO_FRAMEQ	(GET_VIDEO_DEBUG_INFO_BASE + 3)
#define GET_VIDEO_DEBUG_INFO_PRINTSET	(GET_VIDEO_DEBUG_INFO_BASE + 5)
#define GET_VIDEO_DEBUG_INFO_PRINTPARSE	(GET_VIDEO_DEBUG_INFO_BASE + 6)

#define GET_VIDEO_DEBUG_INFO_PRINTMSGADDR	(GET_VIDEO_DEBUG_INFO_BASE + 8)
#define GET_VIDEO_DEBUG_INFO_FWPRINTADDR	(GET_VIDEO_DEBUG_INFO_BASE + 9)
#define GET_VIDEO_DEBUG_INFO_HEVCDBGADDR	(GET_VIDEO_DEBUG_INFO_BASE + 10)

#define GET_VIDEO_DEBUG_INFO_VESDESCADDR	(GET_VIDEO_DEBUG_INFO_BASE + 11)

#define STEPFRM_MODE_START (GET_VIDEO_DEBUG_INFO_BASE + 12)
#define STEPFRM_MODE_STEP (GET_VIDEO_DEBUG_INFO_BASE + 13)
#define STEPFRM_MODE_STOP (GET_VIDEO_DEBUG_INFO_BASE + 14)

#define AVSYNC_MODE_RECOVER (GET_VIDEO_DEBUG_INFO_BASE + 15)
#define AVSYNC_MODE_DISABLE (GET_VIDEO_DEBUG_INFO_BASE + 16)

#define SET_STATUS_PAUSE (GET_VIDEO_DEBUG_INFO_BASE + 17)
#define SET_STATUS_RESUME (GET_VIDEO_DEBUG_INFO_BASE + 18)

#define GET_VIDEO_DEBUG_INFO_PTSDESCADDR (GET_VIDEO_DEBUG_INFO_BASE + 19)

#define SET_TIMER_SPEED (GET_VIDEO_DEBUG_INFO_BASE + 21)

#define GET_AVMIPS_VERSION (GET_VIDEO_DEBUG_INFO_BASE + 22)

#define DEBUG_RESET_DECODER (GET_VIDEO_DEBUG_INFO_BASE + 22)

///////////////////////////////////////////////////////////////////////////////

typedef struct {
    int cropOffsetTop;
    int cropOffsetBottom;
    int cropOffsetLeft;
    int cropOffsetRight;
} CROP_INFO_s;

typedef struct {
    int picdisplayOffsetTop;
    int picdisplayOffsetBottom;
    int picdisplayOffsetLeft;
    int picdisplayOffsetRight;
} PIC_DISPLAY_INFO_s;

struct MpegFormat {
    uint32_t wHsize;
    uint32_t wVsize;
    uint32_t wDispHsize;
    uint32_t wDispVsize;
    uint32_t AspectCode;
    uint32_t FrameRate;
    uint32_t ProgressiveSeq : 1;
    uint32_t FirstField : 1;
    uint32_t clockFreq : 4;
    uint32_t usStride : 16;
    uint32_t PixelDepth : 2;
    uint32_t FieldBufMode : 1;
    uint32_t FrmRateConv : 5;
    uint32_t FullRageMode : 1;
    uint32_t chromaFormat : 1;
    uint32_t AFD;
    uint32_t Par;

    uint32_t wHdeStart;
    uint32_t wVdeStart;
    uint32_t wHTotal;
    uint32_t wVTotal;
    uint8_t colorPrimary;
    uint8_t transferChar;
    uint8_t matrixCoef;
    uint8_t chromaLocBot;
    uint8_t chromaLocTop;
    CROP_INFO_s cropInfo;
    PIC_DISPLAY_INFO_s picDispInfo;
};

typedef struct _tagExtMpegFormat {
    // Deblocking info
    uint8_t bDeblkingEnabled;

    // QMeter info
    uint8_t bPictureQualityAvailable;
    uint8_t bGopBitRateAvailable;
    uint8_t bDummy; /**< dummy to ensure word alignment bit[0] 1 means fake interlace*/
    uint32_t ulPictureQuality;
    uint32_t ulGopBitRate;

    // Overscan info
    uint32_t ulOverscanInfo;
    // FieldsStoreMode_e FieldBufMode;
} ExtMpegFormat_t;

typedef struct _tagVideoBbdParam {
    uint32_t ulBbdHorActive;
    uint32_t ulBbdVerActive;
    uint32_t ulBbdLogoActive;
    uint32_t ulBbdBotPrev;
    uint32_t ulBbdMinColPrj;
    uint32_t ulBbdMinRowPrj;
} VideoBbdParam_t;

typedef struct _tagVideoProfileLvl {
    uint32_t VideoStandard;
    uint32_t VideoProfile;
    uint32_t VideoLevel;
} VideoProfileLvl_t;

typedef union _tagVideoPrivData {
    struct {
        uint8_t PicType;          /**< 0-null, 1-I, 2-P, 3-B */
        uint8_t RefCnt;           /**< reference count */
        uint8_t VerNum;           /**< version number */
        uint8_t FrameIdx;         /**< frame index */
        uint32_t PTS;             /**< video PTS */
        uint32_t CRC;             /**< mem CRC */
        uint32_t DecTime;         /**< decoding time (us) */
        uint8_t bTopFieldFirst;    /**< bTopFieldFirst flag */
        uint8_t bRepeatFirstField; /**< repeat first field flag */
        uint32_t ePictureStructure;
    };
    uint8_t dummy[64]; /**< total 64 bytes, dummy[60]: used for frame err percent from decoder */
} VideoPrivData_u;

typedef struct _tagEDRInfo {
    uint32_t EDRModeFlag;    // 0: EDR mode is disabled. 1: EDR mode is enabled.
    uint32_t MetaDataAvail;  // 0: no meta data for current BL frame. 1: meta data is available
    uint32_t MetaDataAddr;   // Meta data structure addr.
    uint32_t ELFrameAvail;   // 0: no EL frame for current BL frame. 1: EL frame is available in MPEG CH2
    uint32_t ELFrameInfoAddr;
    uint32_t HDRModeFlag;
    uint32_t DispColorSeiAddr;
    //    uint32_t Reserved[1];		// remove this, add one address ExtInfoAddr
} EDRInfo_t;

typedef struct _tagExtInfo {
    uint32_t uTotalSize;
    uint32_t eOutputFormat;
    uint32_t tLUTAddr;
} ExtInfo_t;

typedef struct _matering_display_color_tag {
    uint32_t display_primaries_xy0;
    uint32_t display_primaries_xy1;
    uint32_t display_primaries_xy2;
    uint32_t white_point_xy;
    uint32_t max_display_mastering_luma;
    uint32_t min_display_mastering_luma;

    // uint8_t matrixCoef;	same structure in MpegFormat_t
    uint8_t bitsPerChannel;
    uint8_t chromaSubsmplHorz;
    uint8_t chromaSubsmplVert;
    uint8_t cbSubsmplHorz;
    uint8_t cbSubsmplVert;
    uint8_t chromaSitHorz;
    uint8_t chromaSitVert;
    uint8_t range;
    // uint8_t transferChar;
    // uint8_t colorPrimary;
    uint16_t maxCLL;
    uint16_t maxFALL;
} matering_display_color_t;

typedef struct _tagHDR10PlusInfo {
    uint32_t targeted_system_display_maximum_luminance;
    uint32_t application_version_num_distributions;
    uint32_t maxscl_0;
    uint32_t maxscl_1;
    uint32_t maxscl_2;
    uint32_t average_maxrgb;
    uint32_t distribution_values_0_0;
    uint32_t distribution_values_0_1;
    uint32_t distribution_values_0_2;
    uint32_t distribution_values_0_3;
    uint32_t distribution_values_0_4;
    uint32_t distribution_values_0_5;
    uint32_t distribution_values_0_6;
    uint32_t distribution_values_0_7;
    uint32_t distribution_values_0_8;
    uint32_t distribution_values_0_9;
    uint32_t knee_point_x_y;
    uint32_t num_bezier_curve_anchors_0;
    uint32_t bezier_curve_anchors_1_2;
    uint32_t bezier_curve_anchors_3_4;
    uint32_t bezier_curve_anchors_5_6;
    uint32_t bezier_curve_anchors_7_8;
} HDR10PlusInfo;

struct SLHDR_PayloadMode0Metadata {
    uint32_t tmInputSignalLevelOffset;
    uint32_t shadowGain_highlightGain;
    uint32_t tmOutputFineTuningNumVal_saturationGainNumVal;
    uint32_t tmOutputFineTuningX_Y_0_1;  // Y = MSB 8 bits, X is stored in 8 LSB
    uint32_t tmOutputFineTuningX_Y_2_3;
    uint32_t tmOutputFineTuningX_Y_4_5;
    uint32_t tmOutputFineTuningX_Y_6_7;
    uint32_t tmOutputFineTuningX_Y_8_9;
    uint32_t saturationGainX_Y_0_1;  // Y = MSB 8 bits, X is stored in 8 LSB
    uint32_t saturationGainX_Y_2_3;
    uint32_t saturationGainX_Y_4_5;
    uint32_t midToneWidthAdjFactor;
};

typedef struct _tagSLHDRInfo {
    uint32_t hdr_display_min_max_luminance;  // raw, no calculation
    uint32_t sdr_display_min_max_luminance;
    uint32_t matrix_coeffs_0_1;
    uint32_t matrix_coeffs_2_3;
    uint32_t chroma_to_luma_injection;
    uint32_t kCoeffs_0_1;
    uint32_t kCoeffs_2_colour_space_payloadMode;  // bit 0 = hdr_pic; bit 1..2 = partID_minus1, bit 4..5 = hdr_display,bit 8 = sdr_pic;
                                                     // bit 12..13 = sdr_display,bit 15 = payload_mode
    // payloadMode
    // union SLHDR_PayloadModeMetadata payload_mode_metadata;
    struct SLHDR_PayloadMode0Metadata payload_mode_0_metadata;
} SLHDRInfo;

typedef union _tagHDRSEIMetadata {
    HDR10PlusInfo hdr10_plus;
    SLHDRInfo hdr_Technicolor;
} HDRSEIMetadata;

struct VideoFrameInfo {
    uint32_t start_code;
    uint32_t version;
    struct MpegFormat format;
    ExtMpegFormat_t tExtFormat;
    VideoBbdParam_t tBbdParam;
    VideoProfileLvl_t tProfileLvl;
    VideoPrivData_u tPrivData;
    EDRInfo_t tEDRInfo;
    uint32_t ExtInfoAddr; /**< the address to structure ExtInfo_t */
    uint32_t ulEndCode;
    matering_display_color_t tDispColor;

#if 1 // SX8
    uint32_t ulReserved;
    uint32_t ulHDRScheme;
    HDRSEIMetadata tHDRMetadata;
#else
    HDR10PlusInfo tHDR10PlusInfo;
#endif

    ExtInfo_t tExtInfo;
};

/* the interface of 'MPEG IP' & 'Display IP' */
struct mpegdisp_regs {
    unsigned int h_total;
    unsigned int v_total;
    unsigned int rsvd_04_18[4];
    unsigned int h_width;
    unsigned int h_height;
    unsigned int f_int_location;
    unsigned int crtc_interlace;
    unsigned int f_int_location_sel;
    unsigned int rsvd_2c_30[1];
    unsigned int addr_y[4];
    unsigned int addr_c[4];
    unsigned int blue_screen;
    unsigned int repeat_mode;
    unsigned int resolution_change;
    unsigned int rsvd_5c_80[9];
    unsigned int field_disable;
    unsigned int int_resolution_change;
    unsigned int vsync_latch_by_hsync;
    unsigned int rsvd_8c_90[1];
    unsigned int field_polarity;
    unsigned int vsync_source;
    unsigned int channel_1_enable;
    unsigned int extern_vsync_en_polarity;
    unsigned int clock_source;
    unsigned int clock_divider;
    unsigned int rsvd_a8_c0[6];
    unsigned int global_monitor_select;
    unsigned int global_mpeg_enable;
    unsigned int rsvd_c8_d0[2];
    unsigned int global_use_same_clk_from_channel_1;
    unsigned int rsvd_d4_e0[3];
    unsigned int channel_1_pixel_counter;
    unsigned int channel_1_line_counter;
    unsigned int channel_1_field_interrupt_ro;
    unsigned int channel_2_pixle_counter;
    unsigned int channel_2_line_counter;
    unsigned int channel_2_field_interrupt_ro;
    unsigned int channel_1_extern_vsync_counter;
    unsigned int channel_2_extern_vsync_counter;
};

struct HWDemuxVESDesc_t
{
	unsigned int wp;
	unsigned int rp;
	unsigned int start;
	unsigned int end;
};

struct AVStreamVESDesc_t
{
	unsigned int start;
	unsigned int end;
	unsigned int wp;
	unsigned int rp;
};

#define VPTS_QUEUE_SIZE 6300

struct VideoPTSQueue_t {
    unsigned long long input_pts_map[VPTS_QUEUE_SIZE];
    unsigned int start_wp[VPTS_QUEUE_SIZE][2];
    unsigned int sz;
    unsigned int wrIdx;
    unsigned int rdIdx;
};

struct VideoPTSQueue_t_Part {
    int size;
    int wr;
    int rd;
};

typedef struct _tagVideoFrame
{
	short Idx;
	unsigned char PicType;		/**< 0-null, 1-I, 2-P, 3-B */
	unsigned char bUsed;
	unsigned int PTS;
	unsigned int AddrLum;
	unsigned int AddrChrom;

	short Idx1;
	unsigned char RefCnt;
	unsigned char Reserved;
	unsigned int AddrLum1;		/*For 3D video: [0] top or left; [1] bottom or right*/
	unsigned int AddrChrom1;
	
	unsigned int AddrMetaDat;
	unsigned int AddrMetaDat1;
}Frame_t;

#define FRAMEQ_SIZE 		128				/* max frame buffer num. align with malone */
typedef struct _tagVideoFrameQ
{
	Frame_t q[FRAMEQ_SIZE];
	unsigned int sz;
	unsigned int wrIdx;
	unsigned int rdIdx;	
}FrameQ_t;

typedef struct _tagVideoFrameQArray
{
	FrameQ_t rdy2DispQ;
	FrameQ_t dispQ;
	FrameQ_t dispDoneQ;
	FrameQ_t discardQ;
	
	FrameQ_t s3dPictQueue;		/*record un-matched pictures;*/
	FrameQ_t rdy2EncQ;	/* ready 2 encoder queue */
}FrameQArray_t;

struct avmips_log_desc {
    int enable;
    int level;
    int total;
    int rd;
    int wr;
    char msg[256][256];
    int b3rdInit;
};

///////////////////////////////////////////////////////////////////////////////

extern const char *mpegformat_framerate_name(int rate);
extern void dump_mpegdisp_registers(struct mpegdisp_regs *regs);
extern int read_mpegdisp_regsiters(unsigned int base, struct mpegdisp_regs *regs);
extern int mpegdisp_is_resolution_change(struct mpegdisp_regs *regs);

extern int avmips_get_ves_desc(struct ringbuffer *r, unsigned int channel);
extern int avmips_get_pts_desc(struct ringbuffer *r, int channel);

extern int vs_disable_avsync(void);

extern int vs_get_Ready2DispQ_value(unsigned int *value, unsigned int channel);

extern int chip_id(unsigned int value);

extern unsigned char *avmips_dump_pts_queue(int channel);

extern const char *mpegformat_framerate_name(int rate);

///////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif
