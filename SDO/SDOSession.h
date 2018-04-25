// ServerSession.h: interface for the CServerSession class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERSESSION_H__2C4E0035_3841_43C4_837B_BB7785DA84B0__INCLUDED_)
#define AFX_SERVERSESSION_H__2C4E0035_3841_43C4_837B_BB7785DA84B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\\NetIOCP\\Session.h"

class CSDOManagerModule;



class CSDOSession  : public CSession
{
public:
	CSDOSession(CSDOManagerModule* pModule);
	virtual ~CSDOSession();
public:
	bool	Create(SOCKET hSocket);
	bool	Connect(LPCTSTR strAddr, int nPort);
	void	Close();
	void	CheckErrorReason(int nError);
	
	
	int		Send(LPBYTE pBuf, int nBytes);
	int		SendMessage(CMsg& msg) {return Send(msg.GetBuf(), msg.GetSize());};
	
	int		Receive(DWORD dwFlag);

	
//	int		Dispatch(int nBytes, LPOVERLAPPED2 lpov);
	HANDLE	m_Mute;


	int strNum;
	
protected:
	
	int			ParseNetworkMessage();
	unsigned char * sendbuf;
//	int			DispatchRecv(int nBytes, LPOVERLAPPED2 lpov);
//	int			DispatchSend(int nBytes, LPOVERLAPPED2 lpov);

};

#endif // !defined(AFX_SERVERSESSION_H__2C4E0035_3841_43C4_837B_BB7785DA84B0__INCLUDED_)
