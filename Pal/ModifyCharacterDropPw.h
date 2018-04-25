// ModifyCharacterDropPw.h: interface for the CModifyCharacterDropPw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODIFYCHARACTERDROPPW_H__B2A66885_4AB6_4066_8AA1_0A27DF2905D3__INCLUDED_)
#define AFX_MODIFYCHARACTERDROPPW_H__B2A66885_4AB6_4066_8AA1_0A27DF2905D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseStruct.h"
#include "OperPal.h"
class CModifyCharacterDropPw  
{
public:
	CModifyCharacterDropPw();
	virtual ~CModifyCharacterDropPw();
	vector <CGlobalStruct::TFLV> ModifyCharacterDropPwMain(char *szGMServerName, char * szGMServerIP,char* szRoleName,char * szDropRolePw);
	bool SaveCharacterDropPw(char * szRoleName,char * szOldPasswd,char * szNewPasswd,char * lpGMServerName);//����ԭ����ɫɾ������
	void GetDropPw(char * szRoleName,char * szPasswd,char* lpGMServerName);
	bool RecallCharacterDropPw(char* szRoleName,char * szDropRolePw);
	bool FindLocalCharacterDropPw(char * szRoleName);
	CLogFile logFile;
	COperPal operPal;
	CSqlHelper m_SqlHelper;
	//���汾�ز�ѯ�Ľ��
//	vector<CGlobalStruct::DataRecord> datarecord;
private:
	C_ObjNetClient g_ccNetObj;		// ��·����Ԫ��
	
public:
	static void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ������ 
	static void ModifyCharacterDropPw(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// �޸Ĺ���ȼ����
	
	
};

#endif // !defined(AFX_MODIFYCHARACTERDROPPW_H__B2A66885_4AB6_4066_8AA1_0A27DF2905D3__INCLUDED_)
