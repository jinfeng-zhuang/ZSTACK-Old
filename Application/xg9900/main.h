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

	// �����ļ�
	struct {
		unsigned int ref_speed_rpm;
		unsigned char chan_enable[CHANNEL_MAX];
	} config;
	
	// ���������ݻ�����
	// 117 KB, but sram total 112KB + 16KB
	// �Ż��漰����β������ɷ��ã��м��Ƿ��������
	// sample_area
    //union {
	double sample[SAMPLE_SIZE];
	//struct {
	//	unsigned int D9557[9557]; // ��������֮��ֻ��Ҫǰ 1024 ����
	//	unsigned int fft_src[5*4096];
		// �����ζ��٣�
	//} sample;

    int lines;
    unsigned int low_cut;
	
	//�����ٲ��� + ����������
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

	//���۷��ֵ

    // ����λ��ͨƵֵ�����ֵ����Ƶ�׵�ƽ���ͣ���������
    double dsp_Overall;
    double dsp_Overall_um;

    // �����ٶ�ͨƵֵ����Ч������ֵ����Ƶ�׵�ƽ���ͣ���������
    double spectrum_vel;
    double vel_Overall;
    double vel_Overall_mmps;

    // ������ٶ�ͨƵֵ����Ч������ֵ����Ƶ�׵�ƽ���ͣ���������
    double spectrum_acc;
    double acc_Overall;
    double acc_Overall_gs;

    // ��ʵת��
    double spd_ref_hz;
    double spd_ref_line;
    double spd_true_line;
    double Amplitude_1xRPM_true;
    double spd_true_hz;
    double spd_true_rpm;

    // Ƶ�����
    double overall_pkpk;
    double para_nonsyn_value[10];
    double para_harmonic_value[10];

	// �������ֵ
    double true_pkpk_digit;
};

#endif