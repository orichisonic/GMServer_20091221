// BanInfo.h: interface for the CBanInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BANINFO_H__FE1CDBEC_B30D_4FBD_A430_CF13FB025694__INCLUDED_)
#define AFX_BANINFO_H__FE1CDBEC_B30D_4FBD_A430_CF13FB025694__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OperBan.h"

#include "..\\GameIOCP\\GameInfo.h"
class CBanInfo  : public CGameInfo
{
public:
	CBanInfo();
	virtual ~CBanInfo();
private:
	//���ڱ�����Զ�����ݿ�ִ�в�����sql���
	char operRemoteSql[2048];
	

private:
	//����д��־��Ϣ
	CLogFile logFile;

	//�����м�ĸ�������
	COperBan operBan;

public:
	bool Ban_Open_Query(int iType,int userByID,int GameID,char * ServerIP, char * ServerName, char*  UserID,char * UserName,char*  NickID,char * UserNick,char * StartTime,char * EndTime,char * Reason, char* memo);//����˺�
	bool Ban_Close_Query(int iType,int userByID,int GameID,char * ServerIP, char * ServerName, char*  UserID,char * UserName,char*  NickID,char * UserNick,char * StartTime,char * EndTime,char * Reason, char* memo);//��ͣ�˺�
	bool Ban_AccountList_Query(int GameID,char * ServerName,char * beginTime,char * endTime,int iIndex,int iPageSize);//�˺��б��ѯ
	bool Ban_Account_Query(int GameID,char * ServerName,char * UserName, char * UserNick,char* memo,char * beginTime,char * endTime);//�˺Ų�ѯ

	//Added by tengjie 2010-06-29
	string AnalysisEndTime(char *endTime);
};
#endif // !defined(AFX_BANINFO_H__FE1CDBEC_B30D_4FBD_A430_CF13FB025694__INCLUDED_)
