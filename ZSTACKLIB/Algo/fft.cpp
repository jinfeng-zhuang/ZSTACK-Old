// ��C����ϵ��֮FFT�㷨ʵ�֡� (https://zhuanlan.zhihu.com/p/135259438)
// 

#include <math.h>

#define DOUBLE_PI   6.283185307179586476925286766559
#define PI 3.141592653589793

// data �Ǹ�����ʵ����������ʵ��������
// data ���Ҳ���������
// N = ����������
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
    // ����Ϊ1�ĸ���Ҷ�任, λ�ý�������
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
        // �෴�Ķ����Ƽӷ�
        m = nn;
        while(m >= 2 && j > m)
        {
            j -= m;
            m >>= 1;
        }
        j += m;
    }
    // Danielson - Lanczos ����Ӧ��
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

	//�������е���
	for(I=0;I< N;I++)   //���ݹ����ģ���Ҫ������Ԫ��ִ����䵹��
	{  
		/*��ȡ�±�I�ķ���J����ֵ*/ 
		J=0;
		for(k=0;k<(M/2+0.5);k++)     //k��ʾ����
	     {
	         //*�������*/
	         m=1;//m�����λΪ1�Ķ�������
			 n=(unsigned int)pow((double)2,(double)(M-1));//n�ǵ�MλΪ1�Ķ�������
			 m <<= k; //��m����kλ
	         n >>= k; //��n����kλ
	         F0=I & n;//I��n��λ����ȡ��ǰ�벿�ֵ�kλ
			 F1=I & m;//I��m��λ����ȡ��F0��Ӧ�ĺ�벿�ֵĵ�λ
	         if(F0) J=J | m; //J��m��λ��ʹF0��Ӧ��λΪ1
	         if(F1) J=J | n; //J��n��λ��ʹF1��Ӧ��λΪ1 
	     }
	 		
	 	 if(I<J)
		{
			temp = dataR[I];
			dataR[I] = dataR[J];
			dataR[J] = temp;
			//�����������ֽ����� 
			temp = dataI[I];
			dataI[I] = dataI[J];
			dataI[J] = temp;
		}                                
	} 

	for(L=1; L<=M;L++)	//FFT���μ���L��1--M
	{
		//�ȼ���һ�¼�� B = 2^(L-1);
		B = 1;
		B = (int)pow((double)2,(double)(L-1));	
		for(j=0;j<=B-1;j++)		
		//j = 0,1,2,...,2^(L-1) - 1
		{	/*ͬ�ֵ�������*/	
			//�ȼ�������k=2^(M-L)
			k=1;
			k = (int)pow((double)2,(double)(M-L));
			//������תָ��p������Ϊkʱ����P=j*k
			p=1;
			p=j*k;
			for(i=0; i<=k-1;i++) 
				{
					//�����±궨Ϊr
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
