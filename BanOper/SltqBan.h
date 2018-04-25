// SltqBan.h: interface for the CSltqBan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLTQBAN_H__5B31853C_DB5A_48C3_9C04_1364BB831386__INCLUDED_)
#define AFX_SLTQBAN_H__5B31853C_DB5A_48C3_9C04_1364BB831386__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "OperBan.h"
class CSltqBan  
{
public:
	CSltqBan();
	virtual ~CSltqBan();
	int ClosePlayer(int GameID,char * STLQ_ServerIP,char * STLQ_ServerName,char * UserName,char * UserNick,char * szBeginTime,char * szBanTime);
	int OpenPlayer(int GameID,char * STLQ_ServerIP,char * STLQ_ServerName,char * UserName,char * UserNick);
	int KickPlayer(int GameID,char * STLQ_ServerIP,char * STLQ_ServerName,char * UserNick);
	//≤È—Ø–≈œ¢
	int QueryAccount(int GameID,char * STLQ_ServerIP,char * STLQ_ServerName,char *UserName,char * UserNick);

	bool LoginServer(char *STLQ_ServerIP);
	bool DisConnect();
	void GetUserNamePwd(char * title,char * m_UserName,char * m_pwd,int * m_port,unsigned int * m_DstGMSID);
private:
	COperBan operBan;
	char operszSql[2048];
	char operRemoteSql[2048];
	COperVector DBVect;

//	CMSocket m_socket;
};

#endif // !defined(AFX_SLTQBAN_H__5B31853C_DB5A_48C3_9C04_1364BB831386__INCLUDED_)
