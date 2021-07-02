# ZSTACK

## 为什么创建该 Repo

为了复用。我需要开发很多工具。
## 原则

* Keep it Simple, Keep is Stupid!
* 为了方便开发，理所应当，包含了常用的第三方库
* ZSTACK 不是一个库，而是很多库的集合，放在一个地方
* Include 目录：根据 ZSTACKLIB 的功能划分为几个头文件，配合 ZSTACK.LIB 发布
* 调试问题的时候尽量用工具先诊断一下，如果工具不行，就给它加功能，让它达到诊断工具的水平

## Build

* VS 2008 和 Win7 发布时间接近，编译出来的 Release 版 exe 可以直接在 Win7 上运行
* GCC

### 注意

* 在需要的时候，在头文件中加入 extern "C" 之前没注意

## 框架

* UI 不允许使用同步 API
* 创建新工程的方法：进入 Application 目录，执行

  python newproj.py template <proj_name>

* LOG 分为 APP 模块和 ZSTACK 中目录如 misc, net, algo 等

## 第三方库

### ffmpeg

下载链接：ffmpeg-4.4-full_build-shared
https://github.com/FFmpeg/FFmpeg/commit/f68ab9de4e

## 版本管理

  <架构>.<功能>.<BUG修复>
