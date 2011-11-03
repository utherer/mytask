#pragma once

class Service{
public:
	virtual ~Service(){}
};

class ResService : public Service{
public:
	virtual ~ResService(){}
};

class XxxService: public Service{
public:
	virtual ~XxxService(){}
};