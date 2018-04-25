// ModifyAccountBankPw.h: interface for the CModifyAccountBankPw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODIFYACCOUNTBANKPW_H__02F257DD_9E65_471F_B95B_36C422DFD941__INCLUDED_)
#define AFX_MODIFYACCOUNTBANKPW_H__02F257DD_9E65_471F_B95B_36C422DFD941__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"
class CModifyAccountBankPw  
{
public:
	CModifyAccountBankPw();
	virtual ~CModifyAccountBankPw();
	vector <CGlobalStruct::TFLV> ModifyAccountBankPwMain(char *szGMServerName, char * szGMServerIP,char* Account,char * szBankPw);
	bool FindLocalAccountBankPw(char * szAccount);
	bool SaveAccountBankPw(char * szRoleAccount,char* szOldPasswd,char * szNewPasswd,char * lpGMServerName);//����ԭ���ʺ���������
	void GetBankPw(char * szAccount,char * szPasswd,char* lpGMServerName);
	bool RecallAccountBankPw(char* szRoleAccount,char * szBankPw);
	CLogFile logFile;
	COperPal operPal;
	CSqlHelper m_SqlHelper;
	//���汾�ز�ѯ�Ľ��
//	vector<CGlobalStruct::DataRecord> datarecord;
private:
	C_ObjNetClient g_ccNetObj;		// ��·����Ԫ��
	
public:
	static void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ������ 
	static void ModifyAccountBankPw(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// �޸Ĺ���ȼ����
	
	
};

#endif // !defined(AFX_MODIFYACCOUNTBANKPW_H__02F257DD_9E65_471F_B95B_36C422DFD941__INCLUDED_)
