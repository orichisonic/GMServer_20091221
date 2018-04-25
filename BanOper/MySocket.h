#ifndef  __CMYSOCKET__
#define  __CMYSOCKET__
#include <WinSock.h>

class CMySocket  
{
public:
	CMySocket();
	virtual ~CMySocket();

public:
	static BOOL	Initialize();	// Initialize WS2_32.DLL.
	static BOOL	Uninitialize(); // UnInitialize WS2_32.DLL.
	
	BOOL MyConnect(LPCSTR lpIP, int dwPort, int timeout = 10);
	virtual BOOL MySend(LPCSTR lpData, const DWORD dwBytes);
	virtual int MyRecv(LPSTR lpcbBuff, DWORD dwBuffSize, long lTime = 10);
	const SOCKET GetSocketHandle() const;
	virtual BOOL Close();

private:
	SOCKET m_socket;
};

#endif
