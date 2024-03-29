#include "Profile.h"
#include "Service.h"
#include "Thread.h"
#include "types.h"

Profile* Profile::self_ = NULL;

Profile* Profile::Instance()
{
    return self_;
}

Profile::Profile()
{
    CHECK(!self_);
	Init();
    self_ = this;
}

Profile::~Profile()
{
	for (int i=0; i<SERVICE_COUNT; ++i){		
		if(services_[i]){
			delete services_[i];
		}
	}
}

ResService* Profile::GetResService()
{
	if( NULL == services_[RES_SERVICE] ){
		AutoLock lock(lock_);
		services_[RES_SERVICE] = new ResService;
	}
	return (ResService*)services_[RES_SERVICE];
}

XxxService* Profile::GetXxxService()
{
	if( NULL == services_[XXX_SERVICE] ){
		AutoLock lock(lock_);
		services_[XXX_SERVICE] = new XxxService;
	}
	return (XxxService*)services_[XXX_SERVICE];
}

void Profile::Init()
{
	memset(services_, 0, sizeof(services_));
}

ThreadManager* Profile::GetThreadManager()
{
	if( NULL == services_[Thread] ){
		AutoLock lock(lock_);
		services_[Thread] = new ThreadManager;
	}
	return (ThreadManager*)services_[Thread];
}
