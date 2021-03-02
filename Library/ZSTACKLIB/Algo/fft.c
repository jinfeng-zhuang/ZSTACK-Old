// 《C语言系列之FFT算法实现》 (https://zhuanlan.zhihu.com/p/135259438)
// 

#include <math.h>

#define DOUBLE_PI   6.283185307179586476925286766559
#define PI 3.141592653589793

// data 是复数：实数，虚数，实数，虚数
// data 最后也是输出部分
// N = 复数的数量
void fft_complex(float * data, int n, int isInverse)
{
    // not correct !!!
    return;

    int mmax, m, j, step, i;
    float temp;
    float theta, sin_htheta, sin_theta, pwr, wr, wi, tempr, tempi;
    //n = 2 * (1 << n);
    n = 2 * n;
    int nn = n >> 1;
    // 长度为1的傅里叶变换, 位置交换过程
    j = 1;
    for(i = 1; i < n; i += 2)
    {
        if(j > i)
        {
            temp = data[j - 1];
            data[j - 1] = data[i - 1];
            data[i - 1] = temp;
            data[j] = temp;
            data[j] = data[i];
            data[i] = temp;
        }
        // 相反的二进制加法
        m = nn;
        while(m >= 2 && j > m)
        {
            j -= m;
            m >>= 1;
        }
        j += m;
    }
    // Danielson - Lanczos 引理应用
    mmax = 2;
    while(n > mmax)
    {
        step = mmax << 1;
        theta = DOUBLE_PI / mmax;
        if(isInverse)
        {
            theta = -theta;
        }
        sin_htheta = sin(0.5 * theta);
        sin_theta = sin(theta);
        pwr = -2.0 * sin_htheta * sin_htheta;
        wr = 1.0;
        wi = 0.0;
        for(m = 1; m < mmax; m += 2)
        {
            for(i = m; i <= n; i += step)
            {
                j = i + mmax;
                tempr = wr * data[j - 1] - wi * data[j];
                tempi = wr * data[j] + wi * data[j - 1];
                data[j - 1] = data[i - 1] - tempr;
                data[j] = data[i] - tempi;
                data[i - 1] += tempr;
                data[i] += tempi;
            }
            sin_htheta = wr;
            wr = sin_htheta * pwr - wi * sin_theta + wr;
            wi = wi * pwr + sin_htheta * sin_theta + wi;
        }
        mmax = step;
    }
}

void fft(float *dataR, float *dataI, int N)
{
	int i,j,k,r;
	int p,L,B;
	unsigned int I,J,K,F0,F1,m,n;
	float Tr,Ti,temp;
    int M;

    M = log((double)N) / log(2.0);

	//输入序列倒序
	for(I=0;I< N;I++)   //根据规律四，需要对数组元素执行码间倒序
	{  
		/*获取下标I的反序J的数值*/ 
		J=0;
		for(k=0;k<(M/2+0.5);k++)     //k表示操作
	     {
	         //*反序操作*/
	         m=1;//m是最低位为1的二进制数
			 n=(unsigned int)pow((double)2,(double)(M-1));//n是第M位为1的二进制数
			 m <<= k; //对m左移k位
	         n >>= k; //对n右移k位
	         F0=I & n;//I与n按位与提取出前半部分第k位
			 F1=I & m;//I与m按位与提取出F0对应的后半部分的低位
	         if(F0) J=J | m; //J与m按位或使F0对应低位为1
	         if(F1) J=J | n; //J与n按位或使F1对应高位为1 
	     }
	 		
	 	 if(I<J)
		{
			temp = dataR[I];
			dataR[I] = dataR[J];
			dataR[J] = temp;
			//补充虚数部分交换： 
			temp = dataI[I];
			dataI[I] = dataI[J];
			dataI[J] = temp;
		}                                
	} 

	for(L=1; L<=M;L++)	//FFT蝶形级数L从1--M
	{
		//先计算一下间隔 B = 2^(L-1);
		B = 1;
		B = (int)pow((double)2,(double)(L-1));	
		for(j=0;j<=B-1;j++)		
		//j = 0,1,2,...,2^(L-1) - 1
		{	/*同种蝶形运算*/	
			//先计算增量k=2^(M-L)
			k=1;
			k = (int)pow((double)2,(double)(M-L));
			//计算旋转指数p，增量为k时，则P=j*k
			p=1;
			p=j*k;
			for(i=0; i<=k-1;i++) 
				{
					//数组下标定为r
					r=1;
					r=j+2*B*i;
					Tr=dataR[r+B]*cos(2.0*PI*p/N) + dataI[r+B]*sin(2.0*PI*p/N);
					Ti=dataI[r+B]*cos(2.0*PI*p/N) - dataR[r+B]*sin(2.0*PI*p/N);
					dataR[r+B]=dataR[r]-Tr;
					dataI[r+B]=dataI[r]-Ti;
					dataR[r]=dataR[r]+Tr;
					dataI[r]=dataI[r]+Ti;
				}	
			}
	}
}

int power_spectrum(float *real, float *image, float *power, unsigned int size)
{
    unsigned int i;

    for (i = 0; i < size; i++) {
        power[i] = sqrt(real[i]*real[i] + image[i]*image[i]);
    }

    return 0;
}

int power_spectrum_complex(float *complex, float *power, unsigned int size)
{
    unsigned int i;

    for (i = 0; i < size; i++) {
        power[i] = sqrt(complex[i * 2 + 0] * complex[i * 2 + 0] + complex[i * 2 + 1] * complex[i * 2 + 1]);
    }

    return 0;
}
