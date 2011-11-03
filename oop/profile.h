#pragma once

#include "Lock.h"

class Service;
class ResService;
class XxxService;
class ThreadManager;
class Profile;

// 必须保证 Profile 的构造和析构的时候，进程里没有其他线程了。
// 最好是在main入口创建Profile，程序退出时销毁
class Profile{
public:
	enum ServiceType{
		RES_SERVICE,
		XXX_SERVICE,
		Thread,
		SERVICE_COUNT
	};

	static Profile* Instance();

	ResService* GetResService();
	XxxService* GetXxxService();
	ThreadManager* GetThreadManager();

private:
    friend class Application;
    Profile();
    ~Profile();
	void Init();

private:
	Lock lock_;
	Service* services_[SERVICE_COUNT];
    static Profile* self_;
};
