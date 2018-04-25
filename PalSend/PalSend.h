#include <windows.h>
#include <mmsystem.h>
#include <winsock.h>
#include <process.h>
#include <time.h>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <set>
#include <string>
#include <stdio.h>
#include <tchar.h>
#include <sys/stat.h>
#include <conio.h>
#include "LogFile.h"
using namespace std;
// NetSlot Client
#include "./NetSlot/NetSlot_Lib.h"
#pragma comment(lib, "./NetSlot/NetSlot_Release.lib")
#include "./PalDataReader/PalDataReader.h"
#pragma comment(lib, "./PalDataReader/PalDataReader.lib")

#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")

#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")


#include "shellapi.h" 
#pragma comment(lib,"shell32.lib")

#include "AU2_Dll.h"
#pragma comment(lib,"AU2_Dll.lib")

//add by ltx
#include "BUTT.h"
#pragma comment(lib,"XDJG_DLL.lib")

// add by ltx
#include "MySocket.h"

//====================================================================================================
// 列举定义
//====================================================================================================

// 封包编号-------------------------------------------------------------------------------------------
enum ENUM_GMPacket
{	
	ENUM_PGGMCConnection_CtoS_RequestLogin		= 0xe001,	// GMClient -> GMServer 要求登入
	ENUM_PGGMCConnection_StoC_LoginResult				,	// GMServer -> GMClient 回传登入结果

	ENUM_PGServerControl_CtoS_SendNonscheduleBulletin = 0xe300,	// GMC -> GMS 要求发送非固定公告
	ENUM_PGServerControl_StoC_SendNonscheduleBulletin = 0xe302,	// GMS -> GMC 回传发送非固定公告结果
};

// 登入结果-------------------------------------------------------------------------------------------
enum ENUM_GMCLoginResult
{
	ENUM_GMCLoginResult_Success = 0,	// 成功
	ENUM_GMCLoginResult_Failed,			// 失败
	ENUM_GMCLoginResult_AccountFailed,	// 帐号错误
	ENUM_GMCLoginResult_PasswordFailed, // 密码错误
	ENUM_GMCLoginResult_IPFailed,		// IP错误
	ENUM_GMCLoginResult_AccountUsing,	// 帐号已经被登入了			
};

// 发送非固定公告结果列举-----------------------------------------------------------------------------
enum ENUM_SendNonscheduleBulletinResult
{
	ENUM_SendNonscheduleBulletinResult_NULL = 0, 
	ENUM_SendNonscheduleBulletinResult_Success,				// 成功.
	ENUM_SendNonscheduleBulletinResult_WorldNotFound,		// 找不到世界
	ENUM_SendNonscheduleBulletinResult_LeaderNotFound,		// 找不到领头观测者
	ENUM_SendNonscheduleBulletinResult_LeaderDisconnect,	// 领头观测者未连线
};

//====================================================================================================
// 封包格式定义
//====================================================================================================

// GMC->GMS:帐号密码封包格式--------------------------------------------------------------------------
struct PGGMCConnection_CtoS_RequestLogin : public S_ObjNetPktBase
{
	char szAccount[32];		// 帐号
	char szPassword[32];	// 密码
	char szIP[16];			// IP位址
	
	PGGMCConnection_CtoS_RequestLogin()
	{
		uiCmdID = ENUM_PGGMCConnection_CtoS_RequestLogin;
	};
};

// GMS->GMC:登入结果封包格式--------------------------------------------------------------------------
struct PGGMCConnection_StoC_LoginResult : public S_ObjNetPktBase
{
	ENUM_GMCLoginResult emResult;	// 结果列举
	
	PGGMCConnection_StoC_LoginResult()
	{
		uiCmdID = ENUM_PGGMCConnection_StoC_LoginResult;
	};
};

// GMC->GMS:要求发送非固定公告------------------------------------------------------------------------
struct PGServerControl_CtoS_SendNonscheduleBulletin  : public S_ObjNetPktBase
{
	int  iWorldID;				// 世界编号,-1代表发送至所有世界
	int  iDelayTime;			// 延迟发送时间(秒),0代表立即发送
	char szContent[1024];		// 公告内容
	
	PGServerControl_CtoS_SendNonscheduleBulletin()
	{
		uiCmdID = ENUM_PGServerControl_CtoS_SendNonscheduleBulletin;
	};
};

// GMS->GMC:回传发送非固定公告结果--------------------------------------------------------------------
struct PGServerControl_StoC_SendNonscheduleBulletin : public S_ObjNetPktBase
{
	ENUM_SendNonscheduleBulletinResult emResult;		// 发送非固定公告结果
	
	PGServerControl_StoC_SendNonscheduleBulletin()
	{
		uiCmdID = ENUM_PGServerControl_StoC_SendNonscheduleBulletin;
	};
};

//Added by tengjie 2010-09-20
typedef struct SC_TASKINFO{
	string szIP;
	string szContent;
	int iTaskID;
	int iInterval;
}Sc_TaskInfo;

typedef struct SC_TOTALINFO{
	int iTotalNum;
	string strData;
	vector<Sc_TaskInfo> VecTaskInfo;
}Sc_TotalInfo;
//End

//====================================================================================================
// 介面宣告
//====================================================================================================
void OnConnect		(int iSockID, long lParam);		// 连线成功
void OnDisconnect	(int iSockID, long lParam);		// 断线
void OnConnectfailed(int iConnectID, long lParam);	// 连线失败
void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// 登入结果 
void SendNonscheduleBulletin(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// 发送非固定公告结果
void GetAccountPasswd(char * m_Account,char *m_passwd,int* iGMServerPort,int* iTime,int m_taskid);

void SendNonscheduleBulletinMain(int m_taskid,char *szGMServerIP,int WorldID,char *szContent,int interval);

void GetServerIPGateWay(char* lpServerIP, char* lpGateWay);
void AnalyseTaskId(int taskid,char * message);
bool WA_SendButtle(char * ServerIP, int Port,int Realmid,int ClusterID, char * BoardMessage);
bool WA_Connect_Remote_Login(CMySocket *bullSocket, char * ServerIP, int Port,int Realmid);
void initSocket(void);

//Added by tengjie 2010-08-12
bool SC_SendButtle(char * ServerIP, char * BoardMessage);
inline static unsigned char toHex(const unsigned char &x){ return x > 9 ? x + 55: x + 48; }
string Encode(const char* lpszData);
//====================================================================================================

C_ObjNetClient g_ccNetObj;		// 网路连线元件
int			   g_state;			// 状态(-1:离开 0:登入 1:等待登入 2:发送非固定公告 3:等待发送非固定公告)
char szContent[1024];   //公告内容
int taskid=0;
CLogFile logFile;
int login_num=0;
int send_num=0;
HANDLE m_Mutex;//读数据库
HANDLE m_SendMutex;
HANDLE m_SendJW2Mutex;
HANDLE m_SendWAMutex;
//add by ltx
HANDLE m_SendXDJGMutex;
//Added by tengjie 2010-08-12
HANDLE m_SendSCMutex;

int freTime;			//查询数据库时间频率
int  iDelayTime;		//延时时间
int SendFreTime;
char szData[256];
//modify by ltx
//SOCKET sendSocket;
//CMySocket bullSocket;
int err;
