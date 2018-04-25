// MFBan.h: interface for the CMFBan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFBAN_H__FC3A096E_B6FA_4BB1_B8A4_22DC8ADD975D__INCLUDED_)
#define AFX_MFBAN_H__FC3A096E_B6FA_4BB1_B8A4_22DC8ADD975D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "OperBan.h"
class CMFBan  
{
public:
	CMFBan();
	virtual ~CMFBan();
	int ClosePlayer(int GameID,char * MF_ServerIP,char * MF_ServerName,char *UserName,char * reason);
	int OpenPlayer(int GameID,char * MF_ServerIP,char * MF_ServerName,char *UserName);
	int KickPlayer(int GameID,char * MF_ServerIP,char * MF_ServerName,char *UserName);
	//≤È—Ø–≈œ¢
	int QueryAccount(int GameID,char * MF_ServerIP,char * MF_ServerName,char *UserName,char * UserNick);

	int QueryUserID(int GameID,char * MF_ServerIP,char * MF_ServerName,char *UserName,char * UserID);


	int RemoteTest(int GameID,char * MF_ServerIP,char* MF_ServerName,char *UserName);

private:
	COperBan operBan;
	char operszSql[2048];
	char operRemoteSql[2048];
	COperVector DBVect;
};

#endif // !defined(AFX_MFBAN_H__FC3A096E_B6FA_4BB1_B8A4_22DC8ADD975D__INCLUDED_)
