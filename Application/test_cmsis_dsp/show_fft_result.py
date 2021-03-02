import numpy as np
from scipy.fftpack import fft,ifft
import matplotlib.pyplot as plt
import math

N = 4096
Fs = 15360

x=np.linspace(0,Fs,N-1)

fft_y = np.loadtxt('fft_result_real.txt', delimiter='\r\n')
abs_y=np.abs(fft_y)

#normalization_abs_y=(abs_y/N) * 2

plt.plot(x, abs_y)
plt.show()
