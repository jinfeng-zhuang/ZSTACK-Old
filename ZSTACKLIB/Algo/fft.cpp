#include <math.h>

#define DOUBLE_PI   6.283185307179586476925286766559

// data 是复数：实数，虚数，实数，虚数
// data 最后也是输出部分
// N = 复数的数量
void fft(double * data, int n, int isInverse)
{
    int mmax, m, j, step, i;
    double temp;
    double theta, sin_htheta, sin_theta, pwr, wr, wi, tempr, tempi;
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
