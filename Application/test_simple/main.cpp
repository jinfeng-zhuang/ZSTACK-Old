#include <stdio.h>
#include <zstack.h>
#include <Windows.h>

typedef enum {
    MediaIPSW_DEC_ScaleFactor1_1, /**< 1:1 scaling */
    MediaIPSW_DEC_ScaleFactor1_2, /**< 1:2 scaling */
    MediaIPSW_DEC_ScaleFactor1_4, /**< 1:4 scaling */
    MediaIPSW_DEC_ScaleFactor1_8, /**< 1:8 scaling */
    MediaIPSW_DEC_ScaleFactorLast
} MediaIPSW_DEC_ScaleFactor_t;

typedef enum {
    MediaIPSW_DEC_RotationAngleNone = 0,      /**< 0 degree */
    MediaIPSW_DEC_RotationAngle90,            /**< + 90 degrees */
    MediaIPSW_DEC_RotationAngle180,           /**< 180 degrees */
    MediaIPSW_DEC_RotationAngleMinus90,       /**< -90 degrees */
    MediaIPSW_DEC_RotationAngleNotApplicable, /**< Not applicable */
    MediaIPSW_DEC_RotationAngleLast
} MediaIPSW_DEC_RotationAngle_t;

typedef unsigned char trid_uint8;
typedef unsigned int trid_uint32;

typedef struct {
    int usHorRes;
    int usVerRes;
    int usHorDecRes;
    int usVerDecRes;
} MediaIPSW_DEC_PictRes_t;

typedef struct {
    MediaIPSW_DEC_RotationAngle_t eRotationAngle;
    int eVScaleFactor;
    int eHScaleFactor;
} MediaIPSW_DEC_JpegParam_t;

static void sf_calc_jpegres(const MediaIPSW_DEC_JpegParam_t* const ptJpegParam, MediaIPSW_DEC_PictRes_t* ptPicRes) {
    // JPEG display size calculation:
    trid_uint8 rot_90, rot_270, rot_180, y_scale_ext;
    trid_uint32 x_size, y_size;
    MediaIPSW_DEC_RotationAngle_t angle = MediaIPSW_DEC_RotationAngleNone;
    trid_uint32 temp;
    trid_uint32 width_in_mb, height_in_mb;

    if (!ptJpegParam || !ptPicRes) {
        printf("NULL Pointer!!\n");
        return;
    }

    angle = ptJpegParam->eRotationAngle;
    y_scale_ext = (MediaIPSW_DEC_ScaleFactor1_4 == ptJpegParam->eVScaleFactor)
                      ? 3
                      : ((MediaIPSW_DEC_ScaleFactor1_2 == ptJpegParam->eVScaleFactor) ? 1 : 0);

    // --------- Need to correct for scaling & rotation ------------
    rot_90 = (MediaIPSW_DEC_RotationAngle90 == angle) || (MediaIPSW_DEC_RotationAngleMinus90 == angle);
    rot_270 = (MediaIPSW_DEC_RotationAngleMinus90 == angle);
    rot_180 = (MediaIPSW_DEC_RotationAngle180 == angle);

    // to make 420 format
    x_size = (ptPicRes->usHorRes) >> ptJpegParam->eHScaleFactor;
    x_size = (x_size + (15 >> rot_90)) >> (4 - rot_90);
    x_size = (x_size + (rot_270 & 0x1 & x_size)) << (4 - rot_90);

    // 90 => 16 align
    // 180 => 8 align, >>3, <<3+8 ? 16 align
    y_size = (ptPicRes->usVerRes + y_scale_ext) >> ptJpegParam->eVScaleFactor;
    y_size = (y_size + (rot_90 ? 15 : 7)) >> (3 + rot_90);
    y_size = (y_size + (y_size & 0x1 & rot_180)) << (3 + rot_90);

    // JPEG Decode size calculation:
    temp = (ptPicRes->usHorRes + 15) >> 4;
    switch (ptJpegParam->eHScaleFactor) {
        case 1:
            width_in_mb = (temp + 1) >> 1;
            break;
        case 2:
            width_in_mb = (temp + 3) >> 2;
            break;
        case 3:
            width_in_mb = (temp + 7) >> 3;
            break;
        default:
            width_in_mb = temp;
            break;
    }

    temp = (ptPicRes->usVerRes + 15) >> 4;
    switch (ptJpegParam->eVScaleFactor) {
        case 1:
            height_in_mb = (temp + 1) >> 1;
            break;
        case 2:
            height_in_mb = (temp + 3) >> 2;
            break;
        case 3:
            height_in_mb = (temp + 7) >> 3;
            break;
        default:
            height_in_mb = temp;
            break;
    }

    if (MediaIPSW_DEC_RotationAngle90 == angle || MediaIPSW_DEC_RotationAngleMinus90 == angle) {
        ptPicRes->usHorRes = y_size;
        ptPicRes->usVerRes = x_size;
        ptPicRes->usHorDecRes = height_in_mb << 4;
        ptPicRes->usVerDecRes = width_in_mb << 4;
    } else {
        ptPicRes->usHorRes = x_size;
        ptPicRes->usVerRes = y_size;
        ptPicRes->usHorDecRes = width_in_mb << 4;
        ptPicRes->usVerDecRes = height_in_mb << 4;
    }

    printf("JPEG resolution after scaling & rotation hor: %d, ver: %d\n", ptPicRes->usHorRes, ptPicRes->usVerRes);
}

extern void mem_format_parser_test(void);

int main(int argc, char *argv[])
{
    MediaIPSW_DEC_JpegParam_t jpeg_param = {MediaIPSW_DEC_RotationAngleMinus90,MediaIPSW_DEC_ScaleFactor1_2,MediaIPSW_DEC_ScaleFactor1_2};
    MediaIPSW_DEC_PictRes_t res = {4000,2000,4000,2000};
	

    //sf_calc_jpegres(&jpeg_param, &res);

	log_init(NULL);

	//log_info("%s\n", __FUNCTION__);

	//mem_format_parser_test();


    DWORD time_start, time_end;
    /* 获取开始时间 */
    time_start = GetTickCount(); //从操作系统启动经过的毫秒数
    Sleep(3000);
    time_end = GetTickCount();

    printf("time_end - time_start = %d\n", time_end - time_start);

    return 0;
}