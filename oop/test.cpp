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

		Profile::Instance()->GetThreadManager()->GetThread("xxx")->PostTask(new Test_Task);
		Profile::Instance()->GetThreadManager()->GetThread("yyy")->PostTask(new Test_Task);
	}
};

void test(){
	ResService* res = Profile::Instance()->GetResService();

	Thread t1("xxx");
	Thread t2("yyy");

	HANDLE handles[2]={ t1.Handle(), t2.Handle() };

	t1.PostTask(new MainCPP_Task);
	t2.PostTask(new MainCPP_Task);

	WaitForMultipleObjects(2, handles, TRUE, INFINITE);
}

int Run1(int argc, TCHAR* argv[]){
	ResService* res = Profile::Instance()->GetResService();
	XxxService* xxx = Profile::Instance()->GetXxxService();

	res = Profile::Instance()->GetResService();
	res = Profile::Instance()->GetResService();

	test();

	return 0;
}

class Application{
public:
    int Exec(){
        LPWSTR* argv;
        int argc;
        int ret;

        argv = CommandLineToArgvW(GetCommandLineW(), &argc);
        ret = Run(argc, argv);
        LocalFree(argv);

        return ret;
    }

    virtual int Run(int argc, TCHAR* argv[]) = 0;

private:
    Profile profile_;
};

class MyApp: public Application{
private:
    virtual int Run(int argc, TCHAR* argv[]){
        return Run1(argc, argv);
    }
};

int main(int argc, TCHAR* argv[]){
	MyApp app;
    return app.Exec();
}