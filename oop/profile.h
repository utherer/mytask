#pragma once

#include "Lock.h"

class Service;
class ResService;
class XxxService;
class ThreadManager;
class Profile;

// ���뱣֤ Profile �Ĺ����������ʱ�򣬽�����û�������߳��ˡ�
// �������main��ڴ���Profile�������˳�ʱ����
class Profile{
public:
	enum ServiceType{
		RES_SERVICE,
		XXX_SERVICE,
		Thread,
		SERVICE_COUNT
	};

	Profile();
	~Profile();

	static Profile* Get();

	ResService* GetResService();
	XxxService* GetXxxService();
	ThreadManager* GetThreadManager();

private:
	void Init();

private:
	Lock lock_;
	Service* services_[SERVICE_COUNT];
};
