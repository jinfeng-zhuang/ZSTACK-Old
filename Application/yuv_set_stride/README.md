# yuv_set_stride

FFMPEG 生成的 YUV 文件 stride 和 width 相同，但是 Windsor 测试的时候，测试码流的 stride 与之不同

不清楚 ffmpeg 如何设置 stride 就写了该工具。
