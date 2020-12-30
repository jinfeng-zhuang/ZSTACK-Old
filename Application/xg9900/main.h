#ifndef MAIN_H
#define MAIN_H

#define LOG_CONFIG_LENGTH   (256)
#define CHANNEL_MAX			(3)
#define SAMPLE_SIZE			(9557 + 4096 * 2 + 4096)

#define SAMPLE_CALC_ADDR    (9557)
#define SAMPLE_AVER_ADDR    (9557 + 4096 * 2)
#define SAMPLE_POWER_ADDR   (0)

#define ADDR_RAW            (0)
#define ADDR_CALC           (9557)
#define ADDR_VEL            (ADDR_CALC)
#define ADDR_ACC            (ADDR_VEL + 4096)
#define ADDR_FFT            (9557 + 4096 * 2)
#define ADDR_POWER          (9557)

#define FREQ_LIMIT_LOW      (5)     // 5Hz
#define FREQ_LIMIT_HIGH     (6000)  // 6KHz

struct application {
    struct {
        char log_config[LOG_CONFIG_LENGTH];
        char filename[FILENAME_MAX];
		// FOR TEST
		int freq;
		int sample_freq;
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
	double sample[SAMPLE_SIZE];
	//struct {
	//	unsigned int D9557[9557]; // 计算量纲之后只需要前 1024 个点
	//	unsigned int fft_src[5*4096];
		// 长波形多少？
	//} sample;

    int lines;
    unsigned int low_cut;
	
	//有量纲参数 + 无量纲因子
    struct {
        double max;
        double min;
        double mean;
        double aver;
        double rms;
        double smra;
        double formf;
        double crestfpos;
        double crestfneg;
        double impulsefpos;
        double impulsefneg;
        double marginfpos;
        double marginfneg;
        double kurtosis;
        double skewness;
    } waveform_parameter;

	//理论峰峰值

    // 计算位移通频值（峰峰值），频谱的平方和，经过修正
    double dsp_Overall;
    double dsp_Overall_um;

    // 计算速度通频值（有效均方根值），频谱的平方和，经过修正
    double spectrum_vel;
    double vel_Overall;
    double vel_Overall_mmps;

    // 计算加速度通频值（有效均方根值），频谱的平方和，经过修正
    double spectrum_acc;
    double acc_Overall;
    double acc_Overall_gs;

    // 真实转速
    double spd_ref_hz;
    double spd_ref_line;
    double spd_true_line;
    double Amplitude_1xRPM_true;
    double spd_true_hz;
    double spd_true_rpm;

    // 频域参数
    double overall_pkpk;
    double para_nonsyn_value[10];
    double para_harmonic_value[10];

	// 最大真峰峰值
    double true_pkpk_digit;
};

#endif