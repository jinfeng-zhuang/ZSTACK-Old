# ZSTACK

## 为什么创建该 Repo

为了复用。

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

* UI 不允许使用同步 API
* 创建新工程的方法：进入 Application 目录，执行

  python newproj.py template <proj_name>

* LOG 分为 APP 模块和 ZSTACK 中目录如 misc, net, algo 等

## 版本管理

  <架构>.<功能>.<BUG修复>
