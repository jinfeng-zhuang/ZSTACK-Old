import numpy as np
import matplotlib.pyplot as plt
import math

fft_points = 4096
k = np.arange(fft_points)
hanning_window = 0.5 * ( 1.0 + np.cos( 2.0 * math.pi * k / float(fft_points-1) - math.pi) )
plt.plot(k,hanning_window)
plt.show()
