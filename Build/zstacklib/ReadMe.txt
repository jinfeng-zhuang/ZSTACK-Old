
* �����̻߳�

	��һЩ console Ӧ�ó���ת��Ϊ��һ���̵߳Ĺ���
		����һ����Ϣ���У�ָ�� SIZE
		while ��ѭ����ȡ��Ϣ
		������Ϣ
	���������Ƕ�����ӵģ�����Ҫ�޸�֮ǰ�Ĵ���

* UI & Work Thread Эͬ

	�ȴ��� Work Thread �õ� Thread ID
	Ȼ�󴴽� UI Thread ���� Thread ID
	
	UI ִ�� Post ���Լ��� HWND ���ݸ� Thread �������
	
	Worker Thread �ṩ Hook ������ģ�飬����������ģ�鲻��Ҫ Windows ��صĴ��룬�����ڣ�
		
		MessageTransfer(Input param, Output param); // Output will be send to UI
	
	��Ϊ��ͬһ�����̿ռ䣬ȫ�ֱ����ǹ���ġ�Work Thread ��������֮��ȴ� UI ��Ӧ����������Ҳ�������������
	
	���Ǵ��������� Sample Thread����ͣ�Ĳ�������������̵߳õ���ʵ�֣�������ģ��ķ�ʽ��ɡ�
	
	Worker Thread ������ UI �ĺ�̨�̶߳ԽӴ��ڣ���ֻ����Ӧ�����������Ĵ���������һ���̡߳�����̻߳�ȡ���Ժ� UI Thread ����һ��
	
	ע�⣬Ҫ���ݸ������̵߳����ݲ�Ҫ����ջ�У������������߳�ʹ��ǰ��������ݿ��ܱ�ջ�޸ĵ���