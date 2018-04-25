#pragma once

#ifdef DSNSHELL_EXPORTS
#define DSNSHELL_API __declspec(dllexport)
#else
#define DSNSHELL_API __declspec(dllimport)
#endif

class INormalTimer;
class INetHandler;
class CNetPacket;

enum DSNSHELL_STATECODE
{
	DSNSHELL_OK,

	DSNSHELL_NOTCREATE,

	DSNSHELL_NOTCONNECT,
	DSNSHELL_CONNECTINGNOW,
	DSNSHELL_CONNECTFAIL,

	DSNSHELL_UNKNOWN,

	DSNSHELL_BUOK = -1,
};

enum DSNSHELL_MSGIDX
{
	MSGIDX_LOGINGW = 1,
	MSGIDX_SERVERLIST,

	MSGIDX_LOGINGS,
	MSGIDX_CHANNELLIST,
	MSGIDX_ENTERCHANNEL,

	MSGIDX_USERLIST,
	MSGIDX_ROOMLIST,
	MSGIDX_ROOMDETAIL,
	MSGIDX_CHAT,
	MSGIDX_WHISPER,

	MSGIDX_ROOMCREATED,
	MSGIDX_ROOMDESTROYED,
	MSGIDX_USERENTERCHANNEL,
	MSGIDX_USERLEAVECHANNEL,
	MSGIDX_LABA,

	MSGIDX_ROOMSLOTLIST,

	MSGIDX_ENTERROOM,
};

#pragma pack(push)
#pragma pack(1)

//Update�����ķ���ֵ
struct UPDATE_RET
{
	unsigned char m_ucMsgCount;				//����Update�������ù��յ���������Ϣ
	unsigned char m_szMsgIdx[255];			//����Update���������յ�����ϢID����Ӧö������DSNSHELL_MSGIDX

#ifdef DSNSHELL_EXPORTS
	UPDATE_RET();
	void Clear(void);
	void AddMsg(unsigned char idx);
#endif
};

//��½GW�ķ�����Ϣ
struct LOGINGW_RET
{
	unsigned char m_ucRetval;				//����ֵ����������˵��������0Ϊ��ȷ������ֵΪ������ͬ
	TCHAR m_szNickName[255];				//�û��ǳ�
	unsigned long m_ulSerialNo;				//�û���ţ���½GSҪ�õģ�
	unsigned char m_ucGTownExist;			//GTown�Ƿ����
};

//һ��GS����Ϣ
struct SERVER_INFO
{
	unsigned short m_usServerNo;			//GS���
	TCHAR m_szServerName[255];				//GS����
	TCHAR m_szServerIp[255];				//IP
	unsigned short m_usServerPort;			//�˿�
	unsigned short m_usGrade;				//GS�ȼ�������û�ã�
	unsigned char m_ucRate;					//GS����ұ���
};

//����GS�б�ķ�����Ϣ
struct SERVERLIST_RET
{
	unsigned char m_ucRetval;
	unsigned short m_usServerCount;			//GS����
	SERVER_INFO m_szServerList[10];			//ÿ��GS����Ϣ
};

//һ��Ƶ������Ϣ
struct CHANNEL_INFO
{
	unsigned char m_ucChannelNo;			//Ƶ�����
	TCHAR m_szChannelName[255];				//Ƶ����
	unsigned short m_usCurrentPlayer;		//��ǰ�������
	unsigned short m_usMaxPlayer;			//������������������
	unsigned short m_usMinLevel;			//�������ĵȼ�����
	unsigned short m_usMaxLevel;			//�������ĵȼ�����
	unsigned char m_ucEventNum;				//��֪��
};

//����Ƶ���б�ķ�����Ϣ
struct CHANNELLIST_RET
{
	unsigned char m_ucRetval;				//2��ʾ�б�������
	unsigned short m_usChannelCount;			//Ƶ������
	CHANNEL_INFO m_szChannelInfo[25];		//Ƶ����Ϣ
};

//һ���û�����Ϣ
struct USER_INFO
{
	TCHAR m_szNickName[255];				//�ǳ�
	TCHAR m_szAccount[255];					//�ʺ�
	short m_sRoomNo;						//����ţ�-1Ϊû����
};

//�����û��б�ķ�����Ϣ
struct USERLIST_RET
{
	unsigned char m_ucRetval;				//2��ʾ�б�������
	unsigned short m_usUserCount;			//�û�����
	USER_INFO m_szUserInfo[512];			//�û���Ϣ
};

//һ���������Ϣ
struct ROOM_INFO
{
	unsigned short m_usRoomNo;				//�����
	TCHAR m_szRoomName[255];				//������
};

//���󷿼��б�ķ�����Ϣ
struct ROOMLIST_RET
{
	unsigned char m_ucRetval;				//2��ʾ�б�������
	unsigned short m_usRoomCount;			//��������
	ROOM_INFO m_szRoomInfo[512];			//������Ϣ
};

//һ���������ϸ��Ϣ
struct ROOMDETAIL_RET
{
	unsigned char m_ucRetval;
	unsigned short m_usRoomNo;				//�����
	TCHAR m_szRoomName[255];				//������
	TCHAR m_szNickName[18][255];			//�ǳ�
};

//������һ��slot����Ϣ
struct ROOMSLOT
{
	unsigned char m_ucStatus;				//״̬��0û�ˣ�1�رգ�2���ˣ�3NPC��
	TCHAR m_szNickName[255];				//�ǳ�
};

//����slot��Ϣ�б�
struct ROOMSLOTLIST_RET
{
	unsigned char m_ucRetval;				//2��ʾ�������
	ROOMSLOT m_SlotList[19];				//ÿ��slot����Ϣ��0~18��0~7��Ϸλ�ã�8������9~18�Թ�λ�ã�
};

#pragma pack(pop)

#ifdef DSNSHELL_EXPORTS

class CDSNShell
{
protected:

	INormalTimer* m_pNormalTimer;

	UPDATE_RET m_UpdateRetVal;

	INetHandler* m_pNetHandlerGW;
	int m_nNetStateGW;
	unsigned long m_ulConnectTimeGW;
	
	queue<LOGINGW_RET> m_qLoginGWRet;
	queue<SERVERLIST_RET> m_qServerListRet;

	INetHandler* m_pNetHandlerGS;
	int m_nNetStateGS;
	unsigned long m_ulConnectTimeGS;

	queue<unsigned char> m_qLoginGSRet;
	
	bool m_bReceivingChannelList;
	CHANNELLIST_RET m_ChannelListTmp;
	unsigned short m_usChannelListIdx;
	queue<CHANNELLIST_RET> m_qChannelListRet;

	queue<unsigned char> m_qEnterChannelRet;

	bool m_bReceivingUserList;
	USERLIST_RET m_UserListTmp;
	unsigned short m_usUserListIdx;
	queue<USERLIST_RET*> m_qUserListRet;

	bool m_bReceivingRoomList;
	ROOMLIST_RET m_RoomListTmp;
	unsigned short m_usRoomListIdx;
	queue<ROOMLIST_RET*> m_qRoomListRet;
	queue<ROOMDETAIL_RET> m_qRoomDetailRet;
	queue<tstring> m_qChatRet;
	queue<tstring> m_qWhisperRet;

	queue<ROOM_INFO> m_qRoomCreated;
	queue<unsigned short> m_qRoomDestroyed;
	queue<USER_INFO> m_qUserEnter;
	queue<tstring> m_qUserLeave;
	queue<tstring> m_qLaba;

	queue<unsigned char> m_qEnterRoomRet;

	bool m_bReceivingRoomSlotList;
	ROOMSLOTLIST_RET m_RoomSlotListTmp;
	queue<ROOMSLOTLIST_RET> m_qRoomSlotListRet;

public:

	CDSNShell();
	~CDSNShell();

	int Update(UPDATE_RET& ret);

	int ConnectGW(LPCTSTR szIPAddress, int nPort);
	int IsGWConnected(void);

	int LoginGW(LPCTSTR szAccount, LPCTSTR szPassword, LPCTSTR szVersion);
	int LoginGWRet(LOGINGW_RET& ret);
	int GetServerList(void);
	int ServerListRet(SERVERLIST_RET& ret);

	int SendBulletin(LPCTSTR szContent);
	int KickUser(LPCTSTR szAccount);

	int ConnectGS(LPCTSTR szIPAddress, int nPort);
	int IsGSConnected(void);

	int LoginGS(unsigned long ulSerialNo);
	int LoginGSRet(unsigned char& ret);
	int GetChannelList(void);
	int ChannelListRet(CHANNELLIST_RET& ret);
	int EnterChannel(unsigned char ucChannelNo);
	int EnterChannelRet(unsigned char& ret);

	int GetUserList(void);
	int UserListRet(USERLIST_RET& ret);
	int GetFriendList(void);
	int GetRoomList(void);
	int RoomListRet(ROOMLIST_RET& ret);
	int GetRoomDetail(unsigned short usRoomNo);
	int RoomDetailRet(ROOMDETAIL_RET& ret);
	int Chat(LPCTSTR szWord);
	int ChatRet(LPTSTR szWord);
	int Whisper(LPCTSTR szNick, LPCTSTR szWord);
	int WhisperRet(LPTSTR szWord);
	int SetGPointTimes(unsigned char times);
	int SetExpTimes(unsigned char times);
	int JinYan(LPCTSTR szNick, unsigned char type, unsigned char flag);

	int RoomCreated(ROOM_INFO& room);
	int RoomDestroyed(unsigned short& usRoomNo);
	int UserEnterChannel(USER_INFO& user);
	int UserLeaveChannel(LPTSTR szNick);
	int GetLaba(LPTSTR szWord);

	int EnterRoom(unsigned short usRoomNo);
	int EnterRoomRet(unsigned char& ret);
	int LeaveRoom(void);
	int GetRoomSlotList(void);
	int RoomSlotListRet(ROOMSLOTLIST_RET& ret);
	int ChangeSlot(unsigned char ucSlot);
	int ChangeRoomName(LPCTSTR szName);

protected:

	void ParseMsgGW(CNetPacket& msg);
	void ParseMsgGS(CNetPacket& msg);
};

#endif

//����DSNShell������ֵΪDSNShell�ľ����
extern "C" DSNSHELL_API int CreateDSNShell(void);
//����DSNShell�������
extern "C" DSNSHELL_API void DestroyDSNShell(int handle);
//����DSNShell��������յ���Ϣ�ı�־��
extern "C" DSNSHELL_API int DSNShellUpdate(int handle, UPDATE_RET& ret);

//����GW�������IP���˿ڣ�
extern "C" DSNSHELL_API int DSNShellConnectGW(int handle, LPCTSTR szIPAddress, int nPort);
extern "C" DSNSHELL_API int DSNShellIsGWConnected(int handle);

//��½GW��������ʺţ����룬�汾�ţ�
extern "C" DSNSHELL_API int DSNShellLoginGW(int handle, LPCTSTR szAccount, LPCTSTR szPassword, LPCTSTR szVersion);
extern "C" DSNSHELL_API int DSNShellLoginGWRet(int handle, LOGINGW_RET& ret);
//ȡ��GS�б������
extern "C" DSNSHELL_API int DSNShellServerList(int handle);
extern "C" DSNSHELL_API int DSNShellServerListRet(int handle, SERVERLIST_RET& ret);

//�����棨������������ݣ�
extern "C" DSNSHELL_API int DSNShellSendBulletin(int handle, LPCTSTR szContent);
//���ˣ�������ʺţ�
extern "C" DSNSHELL_API int DSNShellKickUser(int handle, LPCTSTR szAccount);

//����GS�������IP���˿ڣ�
extern "C" DSNSHELL_API int DSNShellConnectGS(int handle, LPCTSTR szIPAddress, int nPort);
extern "C" DSNSHELL_API int DSNShellIsGSConnected(int handle);

//��½GS��������û���ţ�
extern "C" DSNSHELL_API int DSNShellLoginGS(int handle, unsigned long ulSerialNo);
extern "C" DSNSHELL_API int DSNShellLoginGSRet(int handle, unsigned char& ret);
//ȡ��Ƶ���б������
extern "C" DSNSHELL_API int DSNShellChannelList(int handle);
extern "C" DSNSHELL_API int DSNShellChannelListRet(int handle, CHANNELLIST_RET& ret);
//����Ƶ���������Ƶ����ţ�
extern "C" DSNSHELL_API int DSNShellEnterChannel(int handle, unsigned char ucChannelNo);
extern "C" DSNSHELL_API int DSNShellEnterChannelRet(int handle, unsigned char& ret);

//ȡ��Ƶ�����û��б������
extern "C" DSNSHELL_API int DSNShellUserList(int handle);
extern "C" DSNSHELL_API int DSNShellUserListRet(int handle, USERLIST_RET& ret);
//ȡ�ú����б������
extern "C" DSNSHELL_API int DSNShellFriendList(int handle);
//ȡ��Ƶ���ڷ����б������
extern "C" DSNSHELL_API int DSNShellRoomList(int handle);
extern "C" DSNSHELL_API int DSNShellRoomListRet(int handle, ROOMLIST_RET& ret);
//ȡ�÷�����ϸ��Ϣ�������������ţ�
extern "C" DSNSHELL_API int DSNShellRoomDetail(int handle, unsigned short usRoomNo);
extern "C" DSNSHELL_API int DSNShellRoomDetailRet(int handle, ROOMDETAIL_RET& ret);
//���죨������������ݣ�
extern "C" DSNSHELL_API int DSNShellChat(int handle, LPCTSTR szWord);
extern "C" DSNSHELL_API int DSNShellChatRet(int handle, LPTSTR szWord);
//˽�ģ�������ǳƣ��������ݣ�
extern "C" DSNSHELL_API int DSNShellWhisper(int handle, LPCTSTR szNick, LPCTSTR szWord);
extern "C" DSNSHELL_API int DSNShellWhisperRet(int handle, LPTSTR szWord);
//���õ�ǰGS��G�ұ����������������������1��2��
extern "C" DSNSHELL_API int DSNShellSetGPointTimes(int handle, unsigned char times);
//���õ�ǰGS�ľ��鱶���������������������1��2��
extern "C" DSNSHELL_API int DSNShellSetExpTimes(int handle, unsigned char times);
//�������ã�������ǳƣ��������͡���0����1С����2������3�������־����0��ֹ1���ţ�
extern "C" DSNSHELL_API int DSNShellJinYan(int handle, LPCTSTR szNick, unsigned char type, unsigned char flag);

//�㲥���������䣨�����
extern "C" DSNSHELL_API int DSNShellRoomCreated(int handle, ROOM_INFO& room);
//�㲥������û�ˣ������
extern "C" DSNSHELL_API int DSNShellRoomDestroyed(int handle, unsigned short& usRoomNo);
//�㲥�����˽����������
extern "C" DSNSHELL_API int DSNShellUserEnterChannel(int handle, USER_INFO& user);
//�㲥���������ߣ������
extern "C" DSNSHELL_API int DSNShellUserLeaveChannel(int handle, LPTSTR szNick);
//�㲥���յ����ȣ������
extern "C" DSNSHELL_API int DSNShellGetLaba(int handle, LPTSTR szWord);

//���뷿�䣨�����������ţ�
extern "C" DSNSHELL_API int DSNShellEnterRoom(int handle, unsigned short usRoomNo);
extern "C" DSNSHELL_API int DSNShellEnterRoomRet(int handle, unsigned char& ret);
//�˳����䣨�����
extern "C" DSNSHELL_API int DSNShellLeaveRoom(int handle);
//ȡ������slot��Ϣ�������
extern "C" DSNSHELL_API int DSNShellRoomSlotList(int handle);
//����������Ϣ�ڸս������ʱ���ֱ���յ�
extern "C" DSNSHELL_API int DSNShellRoomSlotListRet(int handle, ROOMSLOTLIST_RET& ret);
//�ı�slot״̬�������slot�������˾����˱��û��/û�˱�ɹر�/�رձ��û�ˣ�
extern "C" DSNSHELL_API int DSNShellChangeSlot(int handle, unsigned char ucSlot);
//�ı䷿�����ƣ��������������
extern "C" DSNSHELL_API int DSNShellChangeRoomName(int handle, LPCTSTR szName);