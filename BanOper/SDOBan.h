// SDOBan.h: interface for the CSDOBan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SDOBAN_H__7F0E5EFB_02F9_4E40_B7C3_B5A017BEE722__INCLUDED_)
#define AFX_SDOBAN_H__7F0E5EFB_02F9_4E40_B7C3_B5A017BEE722__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "OperBan.h"
class CSDOBan  
{
public:
	CSDOBan();
	virtual ~CSDOBan();
	int ClosePlayer(int GameID,char * SDO_ServerIP,char * SDO_ServerName,char* UserName);
	int OpenPlayer(int GameID,char * SDO_ServerIP,char * SDO_ServerName,char* UserName);
	int KickPlayer(int GameID,char * SDO_ServerIP,char * SDO_ServerName,char *UserName);
	int RemoteTest(int GameID,char * SDO_ServerIP,char * SDO_ServerName,char *UserName);
	//≤È—Ø–≈œ¢
	int QueryAccount(int GameID,char * SDO_ServerIP,char * SDO_ServerName,char *UserName,char * UserNick);
	int QueryAccountID(int GameID,char* SDO_ServerIP,char* SDO_ServerName,char *UserName);
private:
	COperBan operBan;
	char operszSql[2048];
	char operRemoteSql[2048];
	COperVector DBVect;
};

#endif // !defined(AFX_SDOBAN_H__7F0E5EFB_02F9_4E40_B7C3_B5A017BEE722__INCLUDED_)
