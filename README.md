# ZSTACK

## 为什么创建该 Repo

为了复用。

以后我开发的所有应用都放在这里，无关平台语言。

## 原则

* Keep it Simple, Keep is Stupid!

## Build

* VS 2008 和 Win7 发布时间接近，编译出来的 Release 版 exe 可以直接在 Win7 上运行
* GCC
* ARM-LINUX-GCC
* ARM-NONE-EABI-GCC
* MIPS
* RISVC
* DSP

## 框架

* UI 都用 MESSAGE 来传递数据

Application
	main.cpp
	application.h: 包含该应用所有需要的结构体信息，包括参数，UI 和 工作线程共享该 DB
	param_parser.cpp:
	usage.cpp: