#ifndef MAIN_H
#define MAIN_H

#define LOG_CONFIG_LENGTH   (256)
#define CHANNEL_MAX			(3)
#define SAMPLE_SIZE			(9557 + 5 * 4096)

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
		unsigned int ref_speed;
		unsigned char chan_enable[CHANNEL_MAX];
	} config;
	
	// ���������ݻ�����
	// 117 KB, but sram total 112KB + 16KB
	// �Ż��漰����β������ɷ��ã��м��Ƿ��������
	// sample_area
	double sample[SAMPLE_SIZE];
	//struct {
	//	unsigned int D9557[9557]; // ��������֮��ֻ��Ҫǰ 1024 ����
	//	unsigned int fft_src[5*4096];
		// �����ζ��٣�
	//} sample;
	
	//�����ٲ���
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
	//����������
	//���۷��ֵ
	//Ƶ��3����
	//�������ֵ
};

#endif