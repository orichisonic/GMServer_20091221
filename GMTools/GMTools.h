#pragma  once
#include "..\\Global\\Global.h"
#include <Winsock2.h>
#include <vector>
#include "ServiceManagerModule.h"

using namespace std;

#define PROCESS_NAME "SendSCBroadCast.exe"

struct FileInfo
{
	char FilePath[256];
	char FileName[256];
	char FileVersion[40];
	int FileSize;
	bool flag;
	FileInfo()
	{
		FileSize=0;
		flag=1;
	//	ZeroMemory(&FileSize,256);
	//	ZeroMemory(&FileName,256);
	//	ZeroMemory(&FileVersion,40);
	}
	
};
typedef bool (*Dll_Main)(CSession *,SOCKET,CEnumCore::Msg_Category,CEnumCore::Message_Tag_ID,unsigned char * ,int);
typedef int(_cdecl *PalSend)();
typedef int(_cdecl *MYSendServer)();
typedef void(_cdecl *BanServer)();
typedef int(_cdecl *SendSCBroad)();
///得到当前目录下all dll
void GetLocalDll(vector <FileInfo> * m_file_vector);
//////////////////////////////////////////////////////////////////////////
extern vector <Dll_Main> dll_vector;
extern Dll_Main GM_Main;
extern Dll_Main JW2_Main;
extern Dll_Main GT_Main;
extern Dll_Main GTOP_Main;
extern Dll_Main Pal_Main;
extern Dll_Main PalLog_Main;
extern Dll_Main CG2_Main;
extern Dll_Main SDO_Main;
extern Dll_Main WA_Main;
extern Dll_Main Ban_Main;
extern Dll_Main CF_Main;
//add by ltx
extern Dll_Main XDJG_Main;

