// WASend.h: interface for the CWASend class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WASEND_H__07283B69_B9FE_469E_BA15_5DB9CA913250__INCLUDED_)
#define AFX_WASEND_H__07283B69_B9FE_469E_BA15_5DB9CA913250__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BackStageMgrDef.h"
#include "BSTRS.h"
#include "RSTBS.h"
#include <vector>
using namespace std;

class CWASend  
{
public:
	static	bool SendButtle(char * ServerIP,char * BoardMessage);
	//连接远程服务端
	static	bool Connect_Remote_Login(char * ServerIP);
	void initSocket(void);
	void AddTaskid(int taskid);
	CWASend();
	virtual ~CWASend();
	static unsigned __stdcall SendThread( void *pParam );
public:
	vector <int> vector_taskid;
	int m_taskid;
	int err;
	HANDLE m_SendMutex;
};

#endif // !defined(AFX_WASEND_H__07283B69_B9FE_469E_BA15_5DB9CA913250__INCLUDED_)
