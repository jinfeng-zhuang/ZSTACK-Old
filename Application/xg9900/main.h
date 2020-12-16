#ifndef MAIN_H
#define MAIN_H

#define LOG_CONFIG_LENGTH   (256)
#define CHANNEL_MAX			(3)
#define SAMPLE_SIZE			(9557 + 5 * 4096)

struct application {
    struct {
        char log_config[LOG_CONFIG_LENGTH];
		// FOR TEST
		int freq;
		int sample_freq;
    } param;

	// 配置文件
	struct {
		unsigned int ref_speed;
		unsigned char chan_enable[CHANNEL_MAX];
	} config;
	
	// 采样和数据缓存区
	// 117 KB, but sram total 112KB + 16KB
	// 优化涉及到如何采样，可否复用，中间是否允许计算
	// sample_area
	unsigned int sample[SAMPLE_SIZE];
	//struct {
	//	unsigned int D9557[9557]; // 计算量纲之后只需要前 1024 个点
	//	unsigned int fft_src[5*4096];
		// 长波形多少？
	//} sample;
	
	//有量纲参数
	//无量纲因子
	//理论峰峰值
	//频域3参数
	//最大真峰峰值
};

#endif