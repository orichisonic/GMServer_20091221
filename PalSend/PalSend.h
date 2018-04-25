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
// �оٶ���
//====================================================================================================

// ������-------------------------------------------------------------------------------------------
enum ENUM_GMPacket
{	
	ENUM_PGGMCConnection_CtoS_RequestLogin		= 0xe001,	// GMClient -> GMServer Ҫ�����
	ENUM_PGGMCConnection_StoC_LoginResult				,	// GMServer -> GMClient �ش�������

	ENUM_PGServerControl_CtoS_SendNonscheduleBulletin = 0xe300,	// GMC -> GMS Ҫ���ͷǹ̶�����
	ENUM_PGServerControl_StoC_SendNonscheduleBulletin = 0xe302,	// GMS -> GMC �ش����ͷǹ̶�������
};

// ������-------------------------------------------------------------------------------------------
enum ENUM_GMCLoginResult
{
	ENUM_GMCLoginResult_Success = 0,	// �ɹ�
	ENUM_GMCLoginResult_Failed,			// ʧ��
	ENUM_GMCLoginResult_AccountFailed,	// �ʺŴ���
	ENUM_GMCLoginResult_PasswordFailed, // �������
	ENUM_GMCLoginResult_IPFailed,		// IP����
	ENUM_GMCLoginResult_AccountUsing,	// �ʺ��Ѿ���������			
};

// ���ͷǹ̶��������о�-----------------------------------------------------------------------------
enum ENUM_SendNonscheduleBulletinResult
{
	ENUM_SendNonscheduleBulletinResult_NULL = 0, 
	ENUM_SendNonscheduleBulletinResult_Success,				// �ɹ�.
	ENUM_SendNonscheduleBulletinResult_WorldNotFound,		// �Ҳ�������
	ENUM_SendNonscheduleBulletinResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
	ENUM_SendNonscheduleBulletinResult_LeaderDisconnect,	// ��ͷ�۲���δ����
};

//====================================================================================================
// �����ʽ����
//====================================================================================================

// GMC->GMS:�ʺ���������ʽ--------------------------------------------------------------------------
struct PGGMCConnection_CtoS_RequestLogin : public S_ObjNetPktBase
{
	char szAccount[32];		// �ʺ�
	char szPassword[32];	// ����
	char szIP[16];			// IPλַ
	
	PGGMCConnection_CtoS_RequestLogin()
	{
		uiCmdID = ENUM_PGGMCConnection_CtoS_RequestLogin;
	};
};

// GMS->GMC:�����������ʽ--------------------------------------------------------------------------
struct PGGMCConnection_StoC_LoginResult : public S_ObjNetPktBase
{
	ENUM_GMCLoginResult emResult;	// ����о�
	
	PGGMCConnection_StoC_LoginResult()
	{
		uiCmdID = ENUM_PGGMCConnection_StoC_LoginResult;
	};
};

// GMC->GMS:Ҫ���ͷǹ̶�����------------------------------------------------------------------------
struct PGServerControl_CtoS_SendNonscheduleBulletin  : public S_ObjNetPktBase
{
	int  iWorldID;				// ������,-1����������������
	int  iDelayTime;			// �ӳٷ���ʱ��(��),0������������
	char szContent[1024];		// ��������
	
	PGServerControl_CtoS_SendNonscheduleBulletin()
	{
		uiCmdID = ENUM_PGServerControl_CtoS_SendNonscheduleBulletin;
	};
};

// GMS->GMC:�ش����ͷǹ̶�������--------------------------------------------------------------------
struct PGServerControl_StoC_SendNonscheduleBulletin : public S_ObjNetPktBase
{
	ENUM_SendNonscheduleBulletinResult emResult;		// ���ͷǹ̶�������
	
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
// ��������
//====================================================================================================
void OnConnect		(int iSockID, long lParam);		// ���߳ɹ�
void OnDisconnect	(int iSockID, long lParam);		// ����
void OnConnectfailed(int iConnectID, long lParam);	// ����ʧ��
void LoginResult	(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ������ 
void SendNonscheduleBulletin(int iSockID, int iSize, S_ObjNetPktBase *pData, long lParam);	// ���ͷǹ̶�������
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

C_ObjNetClient g_ccNetObj;		// ��·����Ԫ��
int			   g_state;			// ״̬(-1:�뿪 0:���� 1:�ȴ����� 2:���ͷǹ̶����� 3:�ȴ����ͷǹ̶�����)
char szContent[1024];   //��������
int taskid=0;
CLogFile logFile;
int login_num=0;
int send_num=0;
HANDLE m_Mutex;//�����ݿ�
HANDLE m_SendMutex;
HANDLE m_SendJW2Mutex;
HANDLE m_SendWAMutex;
//add by ltx
HANDLE m_SendXDJGMutex;
//Added by tengjie 2010-08-12
HANDLE m_SendSCMutex;

int freTime;			//��ѯ���ݿ�ʱ��Ƶ��
int  iDelayTime;		//��ʱʱ��
int SendFreTime;
char szData[256];
//modify by ltx
//SOCKET sendSocket;
//CMySocket bullSocket;
int err;
