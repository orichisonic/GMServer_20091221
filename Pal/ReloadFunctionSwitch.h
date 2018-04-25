// ReloadFunctionSwitch.h: interface for the CReloadFunctionSwitch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ReloadFunctionSwitch_H__24A2EC29_AAAB_4FF2_AB9C_15ABFD68C297__INCLUDED_)
#define AFX_ReloadFunctionSwitch_H__24A2EC29_AAAB_4FF2_AB9C_15ABFD68C297__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"

class CReloadFunctionSwitch  
{
public:
	CReloadFunctionSwitch();
	virtual ~CReloadFunctionSwitch();
	void ReloadFunctionSwitchMain(SOCKET m_socket,COperVector * recvopervector,CSession * pPalSession);
private:
	C_ObjNetClient g_ccNetObj;		// ��·����Ԫ��
	
public:
	static 	void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ������
	static void ReloadFunctionSwitch(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ȡ�ý�ɫ�����б���
public:
	COperPal operPal;
	CLogFile logFile;
};

#endif // !defined(AFX_ReloadFunctionSwitch_H__24A2EC29_AAAB_4FF2_AB9C_15ABFD68C297__INCLUDED_)
