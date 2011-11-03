#pragma once

#include <windows.h>

#include <queue>
#include <map>
#include <string>

#include "Lock.h"
#include "Service.h"
#include "task.h"

//////////////////////////////////////////////////////////////////////////
class WaitableEvent{
public:
	WaitableEvent(){
		event_ = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	~WaitableEvent(){
		CloseHandle(event_);
	}

	void Wait(){
		WaitForSingleObject(event_, INFINITE);
	}

	void Signal(){
		SetEvent(event_);
	}

private:
	HANDLE event_;
};

//////////////////////////////////////////////////////////////////////////
class Thread
{
public:
	// 必须保证 thread_name 是唯一的
	Thread(const std::string& thread_name);
	~Thread(void);

	void ThreadMain();

	void PostTask(Task* task);

	HANDLE Handle(){
		return thread_handle_;
	}

	DWORD ID(){
		return thread_id_;
	}

	std::string Name(){
		return thread_name_;
	}

private:
	void RunTask(Task* task);

private:
	HANDLE thread_handle_;
	DWORD thread_id_;
	std::string thread_name_;
	Lock lock_;
	std::queue<Task*> pending_tasks_;
	WaitableEvent wait_event_;
};

//////////////////////////////////////////////////////////////////////////
class ThreadObserver{
public:
	virtual void OnThreadCreated(Thread* thread) = 0;
	virtual void OnThreadDestoryed(Thread* thread) = 0;
};

//////////////////////////////////////////////////////////////////////////
class ThreadManager : public ThreadObserver,
					  public Service{
public:
	virtual void OnThreadCreated(Thread* thread){
		if(thread){
			AutoLock lock(lock_);
			managed_threads_[thread->Name()] = thread;
		}
	}

	virtual void OnThreadDestoryed(Thread* thread){
		ManagedThreads::iterator it = managed_threads_.find(thread->Name());
		if(it != managed_threads_.end()){
			AutoLock lock(lock_);
			managed_threads_.erase(it);
		}
	}

	Thread* GetThread(const char* thread_name){
		ManagedThreads::iterator it = managed_threads_.find(thread_name);
		return it == managed_threads_.end() ? NULL : managed_threads_[thread_name];
	}

private:
	Lock lock_;
	typedef std::map<std::string, Thread*> ManagedThreads;
	ManagedThreads managed_threads_;
};
