#pragma once
#include "Thread.h"
#include "shlwapi.h"

//#include "ThreadManager.h"

#pragma comment(lib,"shlwapi.lib")


#include "..\\Logic\\Logic.h"
#pragma comment(lib,"..\\Logic\\debug\\Logic.lib")



class CTasker:public Thread
{
private:
	virtual DWORD run();
public:
	void Process();
	void Receive();
public:
	SOCKET m_socket;
	unsigned char recv[8192*100];


	
};

