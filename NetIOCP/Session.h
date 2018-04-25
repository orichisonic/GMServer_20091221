/*!----------------------------------------------------------------------------

# Ŭ������ : CSession

# ������ü : CMsg

# ��    �� : 
		1. Accept��  �ϳ��� ��ü�ν� �ٸ� ������ Connect �Ҽ� �ְ�, CMsg ���ۿ� �ִ� ����Ÿ�� ó�� ���ش�.
		2. ����ȭ�� ���� ����Ÿ�� Msg���۸� �ٸ� ���ۿ� �����ؼ� ���ϼ� �ִ�.

# ��    �� : 2002-09-03 (Kim Jong Nyool) : �ʱ� ��� ����

# �� �� �� : 

-----------------------------------------------------------------------------*/
// Session.h: interface for the CSession class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SESSION_H__07C34AD9_5910_4171_94E9_97A447F928CF__INCLUDED_)
#define AFX_SESSION_H__07C34AD9_5910_4171_94E9_97A447F928CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Msg.h"

#define ERROR_NONE			0

#define ERROR_INVALIDMSG	1

#define ERROR_SEND			2
#define ERROR_RECV			3

#define ERROR_LOGINFAILED	6
#define ERROR_INVALIDDESC	7
#define ERROR_LINKFAIL		8
#define ERROR_EXILE			9
#define ERROR_DUPUSER		10
#define ERROR_PORTALDOWN	11
#define ERROR_SEND2			20
#define ERROR_RECV2			21
#define ERROR_RECVBUFFULL	24
#define ERROR_SENDBUFUNFL	25

#define ERROR_MSGREADORWRITE 26

#define ERROR_UNKNOWN		100
#define ERROR_REDIRECT		101

#define LOGIN_SUCCESS		0
#define LOGIN_NOTREGISTERED	1
#define	LOGIN_INVALIDPASSWD	2
#define LOGIN_DUPUSER		3

#define PACKET_NORMAL		0x100
#define PACKET_PASSTHRU		0x200

#define PACKET_NONE			0x00
#define PACKET_RECV			0x01
#define PACKET_SEND			0x02




struct __declspec(dllexport) OVERLAPPED2 // IOCP��  ������  EVENT
{
	OVERLAPPED2(DWORD _dwFlag = 0)
	{
		memset(&ov, 0, sizeof(OVERLAPPED));
		dwFlag	=	_dwFlag;
	};
	
	OVERLAPPED	ov;
	DWORD		dwFlag;
};
typedef OVERLAPPED2 *LPOVERLAPPED2;

class CServiceModule;

class __declspec(dllexport) CSession  
{
public:

	CSession(CServiceModule* pModule);
	virtual			~CSession();

	virtual bool	Create(SOCKET hSocket);
	virtual	bool	Connect(LPCTSTR strAddr, int nPort);
	virtual void	Close();
	virtual void	CheckErrorReason(int nError);

	bool			IsActive() {return m_bIsActive;};
	int				GetStatus() {return i_status;};
	void			SetStatus(int status) {i_status = status;};

	virtual int		Send(LPBYTE pBuf, int nBytes);
	virtual int		SendMessage(CMsg& msg) {return Send(msg.GetBuf(), msg.GetSize());};
	
	virtual int		Receive(DWORD dwFlag);
	void			RemoveMsg();

	virtual	int		Dispatch(int nBytes, LPOVERLAPPED2 lpov);
	
	CString				m_strUserID ;

	SOCKET  GetSocket() {return m_hSocket;};

protected:
  
	// User Info
	bool				m_bIsActive ;

	int i_status;//0,����,1���� 2,������

	CString				m_strIPAddr ;

	// Net Info
	CMsg				m_msg;
	SOCKET				m_hSocket;
	CServiceModule*		m_pModule;

	//int					m_nSendCount;
	int					m_nReadCount;

	OVERLAPPED2			m_ovRecv;
	OVERLAPPED2			m_ovSend;

	bool				m_bSendInProgress;
	CCriticalSection	m_csSendBuf;

//	BYTE				m_pSendBuf[NETWORK_BUF_SIZE * 2];
	BYTE				*m_pSendBuf;

	int					m_nBytesInSendBuf;
	
	bool				AssociateIOCP();
	int					SendDataInBuf();

	virtual int			ParseNetworkMessage() = 0;

	virtual int			DispatchRecv(int nBytes, LPOVERLAPPED2 lpov);
	virtual int			DispatchSend(int nBytes, LPOVERLAPPED2 lpov);
};

#endif // !defined(AFX_SESSION_H__07C34AD9_5910_4171_94E9_97A447F928CF__INCLUDED_)

