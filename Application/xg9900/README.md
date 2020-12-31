# README

## Usage

xg9900 input_reference.txt

## Files

* source.txt - 和 Python 生成的模拟数据一样，为了确保我的计算结果对的上

## Workflow

获取配置文件（参考转速，channel_enable）
上传本机信息，避免封装

for channel=0,1,2 & channel_enable
	sample 3072
	delta = max - min
	if delta > threshold:
		channel_workable = 1

for channel=0,1,2 & channel_enable & channel_workable
	machine_running = 1

// don't implement it now
if 0 == machine_running
	// max - min => server
	// trigger 10 seconds wait

// 第一版本：默认只有一个通道
for channel=0,1,2 & channel_enable & channel_workable & machine_running
	sample 9557 (12bit)
	convert to 32 bit float for future calc
	去除趋势项
	计算时域波形有量纲参数
	计算时域波形的无量纲因子
	计算理论峰峰值
	
	if (exist reference speed)
		for i = 0 ~ 5
			sample 4096
			hanning window
			FFT
			归一化
		average => 1601 data
		
		获取真实转速（能量重力法）
		
		如果获取真实转速
			计算频域3个参数：同步参数、次同步参数、非同步参数
			
		获取最大真峰峰值 （去趋势后的9557 个点来计算）
		
		1601频谱 float 转成 16 bit 的 + 9557 的前 1024 个点 发送到服务器
		
		如果要上传波形（优先级最低）：
			采集长波形数据并上传

## Data Base

#define CHANNEL_MAX		(3)

struct xg9900 {
	struct {
		unsigned int ref_speed;
		unsigned char chan_enable[CHANNEL_MAX];
	} config;
	
	// 117 KB, but sram total 112KB + 16KB
	// 优化涉及到如何采样，可否复用，中间是否允许计算
	// sample_area
	struct {
		unsigned int D9557[9557]; // 计算量纲之后只需要前 1024 个点
		unsigned int fft_src[5*4096];
		// 长波形多少？
	} sample;
	
	有量纲参数
	无量纲因子
	理论峰峰值
	频域3参数
	最大真峰峰值
};
