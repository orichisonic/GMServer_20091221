// OperPal.h: interface for the COperPal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPERPAL_H__213E7A14_A1B5_4FF7_86B4_96FB8A877F11__INCLUDED_)
#define AFX_OPERPAL_H__213E7A14_A1B5_4FF7_86B4_96FB8A877F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "BaseStruct.h"
#import "msxml2.dll"
#include <atlbase.h>
#include <vector>
#include <iostream>
using namespace MSXML2;
#include "stdio.h"
using namespace std;
#include <windows.h>
#include <wininet.h>
//#include "PalSession.h"


#include "..\\GameIOCP\\OperatorGame.h"
class COperPal  : public COperatorGame
{
public:
	COperPal();
	virtual ~COperPal();
public:
	static void OnConnect		(int iSockID, long lParam);		// ���߳ɹ�
	static void OnDisconnect	(int iSockID, long lParam);		// ����
	static void OnConnectfailed(int iConnectID, long lParam);	// ����ʧ��
	void GetUserNamePwd(char * title,char * m_UserName,char * m_pwd,int * m_port);//����û���,����,�˿�
	void GetLogSendNum(int* m_log,int* m_send);//��ȡ�������,��ȡ���ݴ���
	int  FindWorldID(char * ServerName,char * ServerIP);
	
	//��ȡ��Ӧ��LogDBIP
	void FindLogdbIP(char * logdbip,char * servername);
	
	//��ȡ��Ӧ��GameDBIP
	void FindGameDBIP(char * gamedbip,char * servername);
	
	//��ȡ��Ӧ��AccountDBIP
	void FindAccountDBIP(char * accountdbip,char * servername);
	

	//��ȡ��Ӧ��RoleNameID
	void GetRoleNameID(char * lpGMServerName,char * rolename, int * rolenameid);
	
	
	//��ȡ��Ӧ��POINT
	void GetPoint(char * lpGMServerName,char * accountname, int *point);
	
	//����TFLV
	void InsertTFLV(int nPos,CGlobalStruct::TFLV m_tflv,COperVector* popervector);
	
	//����IDתΪ����
	void GiftIDToName(char * m_name,int m_id);
	//��¼����
	void RecordData(int UserByID,CEnumCore::Message_Tag_ID m_Message_Tag_ID,char * lpServerName,COperVector * pSocketData,char * lpReason);
	
	//�ʺ��ҽ�ɫ��
	int AccountToRoleName(S_ROLENAME* s_rolename,char* lpGMServerName,char* accoutnname);
	


	bool BuildTLV(COperVector operVect,int iIndex, int iPageSize);
	bool BuildTLV(char * message);
	bool BuildTLV(COperVector DBVect);

	//�������ļ�
	bool ReadValue(char * strKey,char * strValue,int strLength);

	bool SendEmail(char * lpRecv,char *lpContent,char *lptmpcontent);//maple update
	bool ReadXmlGet(char * strFlag,char *strUrl,char * strpostData,vector<CGlobalStruct::DataRecord> * pVect,int* iCow,int *iCol);

	int GetPuzzleMap(char * PuzzleMap);

	//maple add
	int FindAccountStatus(char *UserName);
private:
	//�������ļ�
	CLogFile logFile;

	//���ڲ�ѯ�������ݿ�
	CSqlHelper m_SqlHelper;
	
	//�����ѯ�õ��ĳ�������
	char petname[128];
	
	//���ݷ��������ƻ�ȡ��Ҫ��IP
	char connLocalDB[20];
	
	//��SqlExrepss�л�ȡ��sql���
	char m_sql[256];

	//�������ļ�����ֵ
	////char retMessage[15][256];////maple update

	bool SendMail(char *lpip,int Port,char *lpRecver,char *lpSender,char *lpHeader,char *lpBody1,char *lpBody2,char *lpBody3,char *lpBody4,char *lpBody5,char *lpBody6,char *lpBody7,char *lpAccount);
	

};
#endif // !defined(AFX_OPERPAL_H__213E7A14_A1B5_4FF7_86B4_96FB8A877F11__INCLUDED_)
static int g_state;
static vector <CGlobalStruct::TFLV> DBVect;
