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

	// �����ļ�
	struct {
		unsigned int ref_speed;
		unsigned char chan_enable[CHANNEL_MAX];
	} config;
	
	// ���������ݻ�����
	// 117 KB, but sram total 112KB + 16KB
	// �Ż��漰����β������ɷ��ã��м��Ƿ��������
	// sample_area
	unsigned int sample[SAMPLE_SIZE];
	//struct {
	//	unsigned int D9557[9557]; // ��������֮��ֻ��Ҫǰ 1024 ����
	//	unsigned int fft_src[5*4096];
		// �����ζ��٣�
	//} sample;
	
	//�����ٲ���
	//����������
	//���۷��ֵ
	//Ƶ��3����
	//�������ֵ
};

#endif