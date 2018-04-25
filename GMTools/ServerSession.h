// ServerSession.h: interface for the CServerSession class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERSESSION_H__2C4E0035_3841_43C4_837B_BB7785DA84B0__INCLUDED_)
#define AFX_SERVERSESSION_H__2C4E0035_3841_43C4_837B_BB7785DA84B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\\NetIOCP\\Session.h"
#include <queue>
using namespace std;


class CServiceManagerModule;
//NETWORK_BUF_SIZE
struct Mepacket 
{
	unsigned char * m_pPacketBuf;
	//unsigned char  m_pPacketBuf[2048];
	int m_iCount;
	Mepacket()
	{
		m_iCount=0;
		m_pPacketBuf=new unsigned char[NETWORK_BUF_SIZE];
	}
	~Mepacket()
	{
		delete[] m_pPacketBuf;
		m_pPacketBuf=NULL;
	}
};

class CServerSession  : public CSession
{
public:
	CServerSession(CServiceManagerModule* pModule);
	virtual ~CServerSession();
public:
	bool    SendActive();
	bool	Create(SOCKET hSocket);
	bool	Connect(LPCTSTR strAddr, int nPort);
	void	Close();
	void	CheckErrorReason(int nError);

	bool    InitWorkThread();
	
	
	int		Send(LPBYTE pBuf, int nBytes);
	int		SendMessage(CMsg& msg) {return Send(msg.GetBuf(), msg.GetSize());};
	
	int		Receive(DWORD dwFlag);

	void    InsertPacket();
	
//	Mepacket PupPacket();
//	vector<Mepacket> vePacket;
	Mepacket * PupPacket();
	vector <Mepacket *> vePacket;

//	int		Dispatch(int nBytes, LPOVERLAPPED2 lpov);
	HANDLE	m_Mute;
		
	int strNum;
	
protected:


	int			ParseNetworkMessage();
	//队列操作同步
	CCriticalSection  m_insertQue;

//	int			DispatchRecv(int nBytes, LPOVERLAPPED2 lpov);
//	int			DispatchSend(int nBytes, LPOVERLAPPED2 lpov);

};

#endif // !defined(AFX_SERVERSESSION_H__2C4E0035_3841_43C4_837B_BB7785DA84B0__INCLUDED_)
