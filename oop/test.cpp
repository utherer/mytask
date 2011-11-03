#include "Thread.h"

#include <stdio.h>
#include <map>
#include <string>

#include "Profile.h"

class Test_Task: public Task{
public:
	virtual ~Test_Task(){
	}

	virtual void Run(){
		printf("Run Test_Task in thread %d\n", GetCurrentThreadId());
	}
};

class MainCPP_Task: public Task{
public:
	virtual ~MainCPP_Task(){
	}

	virtual void Run(){
		printf("Run MainCPP_Task in thread %d\n", GetCurrentThreadId());

		Profile::Get()->GetThreadManager()->GetThread("xxx")->PostTask(new Test_Task);
		Profile::Get()->GetThreadManager()->GetThread("yyy")->PostTask(new Test_Task);
	}
};

void test(){
	ResService* res = Profile::Get()->GetResService();

	Thread t1("xxx");
	Thread t2("yyy");

	HANDLE handles[2]={ t1.Handle(), t2.Handle() };

	t1.PostTask(new MainCPP_Task);
	t2.PostTask(new MainCPP_Task);

	WaitForMultipleObjects(2, handles, TRUE, INFINITE);
}

int Run(int argc, TCHAR* argv[]){
	ResService* res = Profile::Get()->GetResService();
	XxxService* xxx = Profile::Get()->GetXxxService();

	res = Profile::Get()->GetResService();
	res = Profile::Get()->GetResService();

	test();

	return 0;
}

int main(int argc, TCHAR* argv[]){

	Profile profile;

	return Run(argc, argv);
}