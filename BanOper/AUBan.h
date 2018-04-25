// AUBan.h: interface for the CAUBan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUBAN_H__E0B02081_A86F_48A5_A90F_B855EC7D5EC4__INCLUDED_)
#define AFX_AUBAN_H__E0B02081_A86F_48A5_A90F_B855EC7D5EC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OperBan.h"
struct AU_BanPacket
{
	unsigned short int length;
	unsigned int count;
	unsigned char major;
	unsigned char minor;
	unsigned char nicklength;
	char nick[256];

};

typedef void* (_cdecl *Create)();
typedef int (_cdecl *Encrypt)(void *,char *,int,char *,int);

class CAUBan  
{
public:
	CAUBan();
	virtual ~CAUBan();
	int ClosePlayer(int GameID,char * AU_ServerIP,char* AU_ServerName,char* UserName);
	int OpenPlayer(int GameID,char * AU_ServerIP,char* AU_ServerName,char* UserName);
	int KickPlayer(int GameID,char * AU_ServerIP,char* AU_ServerName,char *UserName);
	//≤È—Ø–≈œ¢
	int QueryAccount(int GameID,char * AU_ServerIP,char * AU_ServerName,char *UserName,char * UserNick);
	int AccountToNick(int GameID,char * AU_ServerIP,char * AU_ServerName,char *UserName,char * UserNick);
	void LoadDll();
	void FreeDll();

private:
	HINSTANCE hInst;
	Create au_create;
	Encrypt au_encrypt;
	COperBan operBan;
	char operszSql[2048];
	char operRemoteSql[2048];
	COperVector DBVect;
};

#endif // !defined(AFX_AUBAN_H__E0B02081_A86F_48A5_A90F_B855EC7D5EC4__INCLUDED_)
