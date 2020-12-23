import numpy as np
import matplotlib.pyplot as plt

FS = 1024

fft_input = np.loadtxt('fft_input.txt', delimiter='\r\n')

fft_result = np.fft.fft(fft_input)
fft_result = abs(fft_result)
fft_result_size = fft_result.size

# 为什么频谱的 X 轴是这样的？
freq = np.arange(fft_result_size) * (FS / fft_result_size)

plt.plot(freq, fft_result)
plt.title("信号经过fft变换后的频谱")
plt.show()
