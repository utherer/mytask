#include "Thread.h"
#include <windows.h>

#include "Profile.h"

namespace{

DWORD WINAPI ThreadFunc(void* param){
	((Thread*)param)->ThreadMain();
	return 0;
}

void SetThreadName(const char* name){
	typedef struct tagTHREADNAME_INFO {
		DWORD dwType;  // Must be 0x1000.
		LPCSTR szName;  // Pointer to name (in user addr space).
		DWORD dwThreadID;  // Thread ID (-1=caller thread).
		DWORD dwFlags;  // Reserved for future use, must be zero.
	} THREADNAME_INFO;

	if (!::IsDebuggerPresent())
		return;

	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = name;
	info.dwThreadID = GetCurrentThreadId();
	info.dwFlags = 0;

	__try {
		// The information on how to set the thread name comes from
		// a MSDN article: http://msdn2.microsoft.com/en-us/library/xcb2z8hs.aspx
		const DWORD kVCThreadNameException = 0x406D1388;
		RaiseException(kVCThreadNameException, 0, sizeof(info)/sizeof(DWORD),
			reinterpret_cast<DWORD_PTR*>(&info));
	} __except(EXCEPTION_CONTINUE_EXECUTION) {
	}
}

}

Thread::Thread( const std::string& thread_name )
	:thread_name_(thread_name)
{
	thread_handle_ = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc, this, 0, &thread_id_);
	Profile::Instance()->GetThreadManager()->OnThreadCreated(this);
}

Thread::~Thread(void)
{
	CloseHandle(thread_handle_);
	Profile::Instance()->GetThreadManager()->OnThreadDestoryed(this);
}

void Thread::ThreadMain()
{
	SetThreadName(thread_name_.c_str());

	while (true){
		if(!pending_tasks_.empty()){
			Task* task = pending_tasks_.front();
			pending_tasks_.pop();
			RunTask(task);
		}

		if(!pending_tasks_.empty())
			continue;

		wait_event_.Wait();
	}
}

void Thread::PostTask( Task* task )
{
	{
		AutoLock lock(lock_);
		pending_tasks_.push(task);
	}

	wait_event_.Signal();
}

void Thread::RunTask( Task* task )
{
	task->Run();
	delete task;
}