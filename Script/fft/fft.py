import numpy as np
from scipy.fftpack import fft,ifft
import matplotlib.pyplot as plt
import math

PI = 3.1415926
N=4096
Fs = 6630

x=np.linspace(0,Fs,N)
y=[]

for i in range(0, N):
    #value = 1000*math.sin(PI*i*50.0/Fs) + 2000*math.sin(PI*i*2500.0/Fs) + 3000*math.sin(PI*i*2550.0/Fs)
    value = math.sin(2.0*math.pi*1000*i/Fs) # full period
    y.append(value)

fft_y = fft(y)

abs_y=np.abs(fft_y)

#plt.plot(x, abs_y)
#plt.title('python.fft')

# Use ARM output result
abs_arm = np.loadtxt('fft_result.txt', delimiter='\r\n')

normalization_abs_arm=(abs_arm/N) * 2
normalization_abs_y=(abs_y/N) * 2

#normalization_half_y = normalization_y[range(int(N/2))]


'''
plt.subplot(231)
plt.plot(x[1:400], y[1:400])
plt.title('wave')

plt.subplot(232)
plt.plot(x, abs_y)
plt.title('python.fft')

plt.subplot(233)
#plt.plot(x[:1024], abs_arm)
plt.plot(x, abs_arm)
#plt.plot(x[:2048], abs_arm)
plt.title('arm.dsp.fft')

plt.subplot(234)
#plt.plot(x[:1024], abs_arm)
plt.plot(half_x, normalization_half_y)
#plt.plot(x[:2048], abs_arm)
plt.title('arm.dsp.fft.normal.half')
'''

plt.plot(x, abs_arm)
plt.title('arm.dsp.fft')

plt.show()