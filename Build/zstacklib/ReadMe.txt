
* 功能线程化

	将一些 console 应用程序转化为了一个线程的过程
		创建一个消息队列，指定 SIZE
		while 中循环获取消息
		处理消息
	以上三步是额外添加的，不需要修改之前的代码

* UI & Work Thread 协同

	先创建 Work Thread 得到 Thread ID
	然后创建 UI Thread 传入 Thread ID
	
	UI 执行 Post 将自己的 HWND 传递给 Thread 完成握手
	
	Worker Thread 提供 Hook 给其他模块，可以让其他模块不需要 Windows 相关的代码，类似于：
		
		MessageTransfer(Input param, Output param); // Output will be send to UI
	
	因为在同一个进程空间，全局变量是共享的。Work Thread 做完事情之后等待 UI 的应答，所以数据也不会产生竞争。
	
	但是存在这样的 Sample Thread，不停的采样。这个采样线程得单独实现，不能用模板的方式完成。
	
	Worker Thread 更像是 UI 的后台线程对接窗口，它只有响应能力，真正的处理交给另外一个线程。这个线程获取可以和 UI Thread 放在一起。
	
	注意，要传递给其他线程的数据不要放在栈中，否则在其他线程使用前，这个数据可能被栈修改掉。