#ifndef GTOPER_H_
#define GTOPER_H_

#include "packet.h"
#include "OperatorGT.h"
#include "..\\GameIOCP\\GameInfo.h"
struct TIMEEVENT
{
	//开始时间
	int starthour;		//[0,23]
	int startminute;	//[0,59]
};
struct ITEMPACKET
{
	char szAccount[256];
	int	 mID;
	int  iNum;
};
class  CGTOper:public CGameInfo
{

private:

	//用于中间的辅助操作
	COperatorGT operGT;

public:

	CGTOper(void);
	~CGTOper(void);
private:
	char strLog[2048];

	//接收的数据长度
	int recvLen;

	//连接远程接口
	CMSocket msocket;
public:
	
	//连接远程接口
	bool CONNECT_SERVERMANAGER_LOGIN(char * ServerIP,char * retMessage);

	//查询公告
	vector<CGlobalStruct::TFLV> GT_BROADCAST_SELECT(char * ServerIP,int iIndex,int iPageSize);

	//添加公告
	BOOL GT_BROADCAST_ADD(int IsImmediate,wstring context,DWORD interval,size_t StartTime,size_t EndTime);

	//删除公告
	BOOL GT_BROADCAST_REMOVE(int NoticeID);

	//强制玩家下线（查询通过数据表，这里只用账号踢人）
	BOOL GT_KICK_USER(char * ServerIP,char * szAccount,char * retMessage);
	
	//
	static int GT_KICK_USER(char * ServerIP,char * strAccount);

	bool GT_BanUser(char * ServerIP,char * strAccount,int status);//1.封停,0.解封


	//角色移至黑屋
	void GT_MOVE_USER(int userByID, char * ServerName, char * ServerIP,char * UserName);

	//重设玩家到出生状态
	void GT_USER_RESET(int userByID, char * ServerName, char * ServerIP,char * UserName);

	//查询道具、宠物
	void GM_ITEM_Check();

	//添加道具
	void GT_ITEM_ADD(int userByID,char * ServerName, char * ServerIP,char * UserName,char * szMailTitle, char * szMailContent,int count,char * itemName);

	
	//封停和解封
	BOOL GT_BanUser(int userByID, char * ServerName, char * ServerIP,char * UserName,int UserID,char* userState,char * Reason);

	//瞬间移动
	void GT_Transmit_Query(int userByID, char * ServerName, char * ServerIP,char * UserName,int x,int y,int z);
	
	//禁止、解禁玩家上传图片
	bool GT_Picture_Stop(int userByID, char * ServerName, char * ServerIP,char * UserName,int UserID,char* userState,char * Reason);
	
	//清理违禁图片
	void GT_Picture_Clear(int userByID, char * ServerName, char * ServerIP,char * UserName);
	
	//清空个性签名
	void GT_ClearName_Query(int userByID, char * ServerName, char * ServerIP,char * UserName);

	//数据导入
	bool GT_Import_Item(int userByID,char * ServerName, char * ServerIP, char * MailTitle,char * MailContent, char * ListUserName, char * ListItemID, char * ListItemNum);


	//用户信息查询
	bool UserInfo(char * ServerName, char * ServerIP,char * UserName, char * UserNick,int iIndex,int iPageSize);
	//根据帐号踢人
	vector <CGlobalStruct::TFLV> KickPlayer(int userByID,char * ServerName, char * ServerIP,char * UserName);

	//查询公告
	bool BroadTask_Query(char * ServerIP,int iIndex,int iPageSize);

	//更新公告
	bool BroadTask_Update(int userByID,int taskid,char * boardMessage,char * begintime,char * endtime,int interval,int status);

	//插入公告
	bool BroadTask_Insert(int userByID,char * ServerIP,char * GSServerIP,char * boardMessage,char * begintime,char * endtime,int interval);
	
	
	


};
#endif




















