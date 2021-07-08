# ZSTACK

## 为什么创建该 Repo

为了复用。我需要开发很多工具。

调试问题的时候尽量用工具先诊断一下，如果工具不行，就给它加功能，让它达到诊断工具的水平。

## 原则

* Keep it Simple, Keep is Stupid!
* 为了方便开发，理所应当，包含了常用的第三方库
** ZSTACK 不是一个库，而是很多库的集合，放在一个地方
* Include 目录：根据 ZSTACKLIB 的功能划分为几个头文件，配合 ZSTACK.LIB 发布
** 分为外部和内部头文件

## 构建

* VS 2008 和 Win7 发布时间接近，编译出来的 Release 版 exe 可以直接在 Win7 上运行
** 但是后来选择 VS 2019 新版本，因为更好用。
* GCC

### 注意

* 在需要的时候，在头文件中加入 extern "C" 之前没注意
* 创建新工程的方法：进入 Application 目录，执行

  python newproj.py template <proj_name>

*版本管理

  <架构>.<功能>.<BUG修复>
