import numpy as np
from scipy.fftpack import fft,ifft
import matplotlib.pyplot as plt
import math

PI = 3.1415926
N=1024
freq=1000
Fs = freq * 2
x=np.linspace(0,Fs,N) # Between 0-N, there are Fs's numbers.

y=[]

for i in range(0, N):
    #value = 1000*math.sin(PI*i*50.0/Fs) + 2000*math.sin(PI*i*2500.0/Fs) + 3000*math.sin(PI*i*2550.0/Fs)
    value = math.sin(2.0*math.pi*freq*i/Fs) # full period
    y.append(value)

fft_y = fft(y)

abs_y=np.abs(fft_y)

normalization_abs_y=(abs_y/N) * 2

plt.plot(x, normalization_abs_y)
plt.title('python.fft')

plt.show()
