// SDBan.h: interface for the CSDBan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SDBAN_H__AE9032BF_7367_4B7C_8021_9270243C93A0__INCLUDED_)
#define AFX_SDBAN_H__AE9032BF_7367_4B7C_8021_9270243C93A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "OperBan.h"
class CSDBan  
{
public:
	CSDBan();
	virtual ~CSDBan();
	int ClosePlayer(int GameID,char * SD_ServerIP,char * SD_ServerName,char *UserName);
	int OpenPlayer(int GameID,char * SD_ServerIP,char * SD_ServerName,char *UserName);
	int KickPlayer(int GameID,char * SD_ServerIP,char* SD_ServerName,char *UserName);
	//≤È—Ø–≈œ¢
	int QueryAccount(int GameID,char * SD_ServerIP,char * SD_ServerName,char *UserName,char * UserNick);
private:
	COperBan operBan;
	char operszSql[2048];
	char operRemoteSql[2048];
	COperVector DBVect;
};

#endif // !defined(AFX_SDBAN_H__AE9032BF_7367_4B7C_8021_9270243C93A0__INCLUDED_)
