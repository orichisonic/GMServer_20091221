#ifndef GTINFO_H_
#define GTINFO_H_

#include "OperatorGT.h"

#include "..\\GameIOCP\\GameInfo.h"
class  CGTInfo:public CGameInfo
{
private:
	//用于保存完整数据库执行操作的sql语句
	char operszSql[2048];

	//用于保存在远程数据库执行操作的sql语句
	char operRemoteSql[2048];

	//用于保存执行操作的用户名
	char m_UserName[128];

	//记录日志信息
	char strLog[2048];

public:


	//用于写日志信息


public:

	CGTInfo(void);
	~CGTInfo(void);

	
private:
	//接收的数据长度
	int recvLen;

public:

	//查询角色详细信息
	vector <CGlobalStruct::TFLV> GT_FriendTrade_Log(char * ServerName, char * ServerIP,char * UserName, int UserID,int iIndex,int iPageSize);

	//查询家族信息
	vector <CGlobalStruct::TFLV> GT_FamilyInfo(char * ServerName, char * ServerIP,char * FamilyName,int iIndex,int iPageSize);

	//查询家族成员信息
	vector <CGlobalStruct::TFLV> GT_MemberInfo(char * ServerName, char * ServerIP,char * FamilyName,int FamilyID,int iIndex,int iPageSize);

	//邮件信息查询
	vector <CGlobalStruct::TFLV> GT_MailInfo(char * ServerName, char * ServerIP,char * UserName, int UserID,char* szStartTime,char* szEndTime,int iIndex,int iPageSize);


	//查询玩家宝箱开启信息
	vector <CGlobalStruct::TFLV> GT_TreasureBox_Open(char * ServerName, char * ServerIP,char * UserName, char* szStartTime,char* szEndTime,int iIndex,int iPageSize);


	//查询道具购买消费记录
	vector <CGlobalStruct::TFLV> GT_ItemBuy(char * ServerName, char * ServerIP,char* UserName,
						 int UserID,int type,char * ItemName,char * szStartTime,char * szEndTime,int iIndex,int iPageSize);

	//血拼战绩
	vector <CGlobalStruct::TFLV> GT_Mcoin_Rank(char * ServerName,char * ServerIP,int iType,int iIndex,int iPageSize);
	
	//空间排行
	vector <CGlobalStruct::TFLV> GT_Sns_Query(char * ServerName,char * ServerIP,int iType,int iIndex,int iPageSize);
	
	//属性排行
	vector <CGlobalStruct::TFLV> GT_Attr_Queue(char * ServerName,char * ServerIP,int iType,int iIndex,int iPageSize);

	//名片排行
	vector <CGlobalStruct::TFLV> GT_Card_Record(char * ServerName,char * ServerIP,int iType,int iIndex,int iPageSize);

	//排行
	vector <CGlobalStruct::TFLV> GT_FriendTrade_Queue(char * ServerName,char * ServerIP,int iType,int iIndex,int iPageSize);
	
	//道具查询
	vector <CGlobalStruct::TFLV> GT_Select_Item(char * ServerName,char * ServerIP,char * ItemName);

	vector <CGlobalStruct::TFLV> GT_Back_Money(int userByID,char * ServerName, char * ServerIP, char * UserName, int iID,int iType,int iMoney,char * strItemName);
};
#endif




















