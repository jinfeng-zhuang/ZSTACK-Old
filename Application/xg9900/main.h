#ifndef MAIN_H
#define MAIN_H

#define LOG_CONFIG_LENGTH   (256)
#define CHANNEL_MAX			(3)
#define SAMPLE_SIZE         (9557)
#define FRAME_SIZE          (4096)
#define SAMPLE_FREQ         (15360)
#define FREQ_LIMIT_LOW      (5)     // 5Hz
#define FREQ_LIMIT_HIGH     (6000)  // 6KHz

#define BUFFER_SIZE			(SAMPLE_SIZE + FRAME_SIZE * 2 + FRAME_SIZE)

#define SAMPLE_CALC_ADDR    (SAMPLE_SIZE)
#define SAMPLE_AVER_ADDR    (SAMPLE_SIZE + FRAME_SIZE * 2)
#define SAMPLE_POWER_ADDR   (0)

#define ADDR_RAW            (0)
#define ADDR_CALC           (SAMPLE_SIZE)
#define ADDR_VEL            (ADDR_CALC)
#define ADDR_ACC            (ADDR_VEL + FRAME_SIZE)
#define ADDR_FFT            (SAMPLE_SIZE + FRAME_SIZE * 2)
#define ADDR_POWER          (SAMPLE_SIZE)

struct application {
    struct {
        char log_config[LOG_CONFIG_LENGTH];
        char filename[FILENAME_MAX];
        unsigned int reference_speed_rpm;
    } param;

	// 配置文件
	struct {
		unsigned int ref_speed_rpm;
		unsigned char chan_enable[CHANNEL_MAX];
	} config;
	
	// 采样和数据缓存区
	// 117 KB, but sram total 112KB + 16KB
	// 优化涉及到如何采样，可否复用，中间是否允许计算
	// sample_area
    //union {
	float sample[BUFFER_SIZE];
	//struct {
	//	unsigned int D9557[9557]; // 计算量纲之后只需要前 1024 个点
	//	unsigned int fft_src[5*4096];
		// 长波形多少？
	//} sample;

    int lines;
    unsigned int low_cut;
	
	//有量纲参数 + 无量纲因子
    struct {
        float max;
        float min;
        float mean;
        float aver;
        float rms;
        float smra;
        float formf;
        float crestfpos;
        float crestfneg;
        float impulsefpos;
        float impulsefneg;
        float marginfpos;
        float marginfneg;
        float kurtosis;
        float skewness;
    } waveform_parameter;

	//理论峰峰值

    // 计算位移通频值（峰峰值），频谱的平方和，经过修正
    float dsp_Overall;
    float dsp_Overall_um;

    // 计算速度通频值（有效均方根值），频谱的平方和，经过修正
    float spectrum_vel;
    float vel_Overall;
    float vel_Overall_mmps;

    // 计算加速度通频值（有效均方根值），频谱的平方和，经过修正
    float spectrum_acc;
    float acc_Overall;
    float acc_Overall_gs;

    // 真实转速
    float spd_ref_hz;
    float spd_ref_line;
    float spd_true_line;
    float Amplitude_1xRPM_true;
    float spd_true_hz;
    float spd_true_rpm;

    // 频域参数
    float overall_pkpk;
    float para_nonsyn_value[10];
    float para_harmonic_value[10];

	// 最大真峰峰值
    float true_pkpk_digit;
};

#endif
