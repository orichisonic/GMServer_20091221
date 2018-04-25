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
	static void OnConnect		(int iSockID, long lParam);		// 连线成功
	static void OnDisconnect	(int iSockID, long lParam);		// 断线
	static void OnConnectfailed(int iConnectID, long lParam);	// 连线失败
	void GetUserNamePwd(char * title,char * m_UserName,char * m_pwd,int * m_port);//获得用户名,密码,端口
	void GetLogSendNum(int* m_log,int* m_send);//获取登入次数,获取内容次数
	int  FindWorldID(char * ServerName,char * ServerIP);
	
	//获取对应的LogDBIP
	void FindLogdbIP(char * logdbip,char * servername);
	
	//获取对应的GameDBIP
	void FindGameDBIP(char * gamedbip,char * servername);
	
	//获取对应的AccountDBIP
	void FindAccountDBIP(char * accountdbip,char * servername);
	

	//获取对应的RoleNameID
	void GetRoleNameID(char * lpGMServerName,char * rolename, int * rolenameid);
	
	
	//获取对应的POINT
	void GetPoint(char * lpGMServerName,char * accountname, int *point);
	
	//插入TFLV
	void InsertTFLV(int nPos,CGlobalStruct::TFLV m_tflv,COperVector* popervector);
	
	//礼物ID转为名称
	void GiftIDToName(char * m_name,int m_id);
	//记录数据
	void RecordData(int UserByID,CEnumCore::Message_Tag_ID m_Message_Tag_ID,char * lpServerName,COperVector * pSocketData,char * lpReason);
	
	//帐号找角色名
	int AccountToRoleName(S_ROLENAME* s_rolename,char* lpGMServerName,char* accoutnname);
	


	bool BuildTLV(COperVector operVect,int iIndex, int iPageSize);
	bool BuildTLV(char * message);
	bool BuildTLV(COperVector DBVect);

	//读配置文件
	bool ReadValue(char * strKey,char * strValue,int strLength);

	bool SendEmail(char * lpRecv,char *lpContent,char *lptmpcontent);//maple update
	bool ReadXmlGet(char * strFlag,char *strUrl,char * strpostData,vector<CGlobalStruct::DataRecord> * pVect,int* iCow,int *iCol);

	int GetPuzzleMap(char * PuzzleMap);

	//maple add
	int FindAccountStatus(char *UserName);
private:
	//读配置文件
	CLogFile logFile;

	//用于查询本地数据库
	CSqlHelper m_SqlHelper;
	
	//保存查询得到的宠物名称
	char petname[128];
	
	//根据服务器名称获取需要的IP
	char connLocalDB[20];
	
	//从SqlExrepss中获取的sql语句
	char m_sql[256];

	//读配置文件返回值
	////char retMessage[15][256];////maple update

	bool SendMail(char *lpip,int Port,char *lpRecver,char *lpSender,char *lpHeader,char *lpBody1,char *lpBody2,char *lpBody3,char *lpBody4,char *lpBody5,char *lpBody6,char *lpBody7,char *lpAccount);
	

};
#endif // !defined(AFX_OPERPAL_H__213E7A14_A1B5_4FF7_86B4_96FB8A877F11__INCLUDED_)
static int g_state;
static vector <CGlobalStruct::TFLV> DBVect;
