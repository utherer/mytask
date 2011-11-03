#pragma once

#include <windows.h>

class Lock
{
public:
	Lock(void){
		// �����ں�ǰ�����û�̬�����������������
		InitializeCriticalSectionAndSpinCount(&os_lock_, 2000);
	}

	~Lock(void){
		DeleteCriticalSection(&os_lock_);
	}

	void Acquire(){
		EnterCriticalSection(&os_lock_);
	}

	void Release(){
		LeaveCriticalSection(&os_lock_);
	}

private:
	typedef CRITICAL_SECTION OS_LOCK;
	OS_LOCK os_lock_;
};

class AutoLock{
public:
	AutoLock(Lock& lock):lock_(lock){
		lock_.Acquire();
	}

	~AutoLock(){
		lock_.Release();
	}

private:
	Lock& lock_;
};
