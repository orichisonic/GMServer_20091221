// SendGiftMail.h: interface for the CSendGiftMail class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SENDGIFTMAIL_H__5DB6C301_F8B7_4799_8B67_A5D31EFBF67F__INCLUDED_)
#define AFX_SENDGIFTMAIL_H__5DB6C301_F8B7_4799_8B67_A5D31EFBF67F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"

class CSendGiftMail  
{
public:
	CSendGiftMail();
	virtual ~CSendGiftMail();
	vector <CGlobalStruct::TFLV> SendGiftMailMain(char * szGMServerName,char * szGMServerIP,char * szRoleName,char * szSubject,
						char * szContent,int iMoney,int Gift0,int Gift1,int Gift2,int Gift3,int Gift4,char * szReason);
public:
	COperPal operPal;
	CLogFile logFile;
private:
	C_ObjNetClient g_ccNetObj;		// ��·����Ԫ��
	
public:
	static 	void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ������
	
	static void SendGiftMail(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);		// ���ͽ�Ʒ�ʼ����
};

#endif // !defined(AFX_SENDGIFTMAIL_H__5DB6C301_F8B7_4799_8B67_A5D31EFBF67F__INCLUDED_)
