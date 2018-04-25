#ifndef PALLOGINFO_H_
#define PALLOGINFO_H_

#include "OperatorPal.h"
#include "..\\GameIOCP\\GameInfo.h"

class CPalLogInfo:public CGameInfo
{
private:

	//用于保存完整数据库执行操作的sql语句
	char operszSql[2048];

	//用于保存在远程数据库执行操作的sql语句
	char operRemoteSql[2048];


	//记录日志信息
	char strLog[2048];

	//根据服务器名称获取需要的IP
	char logdbip[20];

private:
	//用于写日志信息
	CLogFile logFile;

	//用于中间的辅助操作
	COperatorPal operAssist;

public:

	//构造函数
	CPalLogInfo();

	//析构函数
	~CPalLogInfo();


	//查询商城购买记录
	bool Shop_Buy_Query(char * ServerName,char * ServerIP,char * RoleName,int iMallType,char * szBegintime, char * szEndtime,int iIndex, int iPageSize);

	//查询邮箱信息
	bool MailBox_Info_Query(char * ServerName,char * ServerIP,char * RoleName,int RoleID, char * szBegintime, char * szEndtime,int iIndex, int iPageSize);

	//查询具体邮件的附件
	bool MailBox_Info_Detail(char * ServerName,char * ServerIP,char * RoleName,int MailID, char * szBegintime, char * szEndtime,int iIndex, int iPageSize);

	//查询拍卖信息
	bool Auction_Info_Query(char * ServerName,char * ServerIP,char * RoleName,int RoleID, char * szBegintime, char * szEndtime,int iIndex, int iPageSize);

	//查询日志信息
	bool LogInfo_Query(char * ServerName,char * ServerIP,char * RoleName,int iBigType,char * szBigType,int iSmallType,char * szSmallType,char * szBegintime, char * szEndtime,int iIndex, int iPageSize);

	//查询日志信息
	bool HisLogInfo_Query(char * ServerName,char * ServerIP,char * RoleName,int iBigType,char * szBigType,int iSmallType,char * szSmallType,char * szBegintime, char * szEndtime,int iIndex, int iPageSize);

	//查询玩家CDKEY道具领取信息
	bool CDKey_Get_Query(char * ServerName,char * ServerIP,char * szAccount, char * szBegintime, char * szEndtime,int iIndex, int iPageSize);

	//查询玩家元宝消耗记录
	bool Money_Waste_Query(char * ServerName,char * ServerIP,char * szAccount, char * szBegintime, char * szEndtime,int iIndex, int iPageSize);

	//获取日志大类
	bool GetLogBigType();

	//获取日志小类
	bool GetLogSmallType(int iBigType);

	//查询角色删除日志
	bool Pal_RoleDel_Query(char * ServerName,char * ServerIP,char * szAccount,char * RoleName, int iType,char * szBegintime, char * szEndtime,int iIndex, int iPageSize);

	// add by ltx
	//装备道具查询
	void Pal_Equipment_Query(char * CustomItemName);



	//用户信息查询
	bool UserInfo(char * ServerName, char * ServerIP,char * UserName, char * UserNick,int iIndex,int iPageSize);
	//根据帐号封停
	bool ClosePlayer(int userByID,char * ServerName, char * ServerIP ,char * UserName,char * UserNick, char* Reason);
	//根据帐号解封
	bool OpenPlayer(int userByID,char * ServerName, char * ServerIP ,char * UserName,char * UserNick, char* Reason);
	//查询封停列表
	bool CloseList(char * ServerName,char * ServerIP,int iIndex, int iPageSize);
	//查询单个账号
	bool CloseSingle(char * ServerName, char * ServerIP,char * UserName,int iIndex, int iPageSize);
	//查询公告
	bool BroadTask_Query(int iIndex,int iPageSize);
	//更新公告
	bool BroadTask_Update(int userByID,int taskid,char * boardMessage,char * begintime,char * endtime,int interval,int status);
	//插入公告
	bool BroadTask_Insert(int userByID,char * ServerIP,char * GSServerIP,char * boardMessage,char * begintime,char * endtime,int interval);
	
	//登陆登出信息
	vector <CGlobalStruct::TFLV> LoginInfo(char * ServerName, char * ServerIP,char * UserName, char * UserNick,char * IP,char * begintime, char * endtime,int iIndex, int iPageSize);
	


};
#endif