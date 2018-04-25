#ifndef SDOINFO_H_
#define SDOINFO_H_

#include "OperatorSDO.h"
#include "..\\GameIOCP\\GameInfo.h"

class  CSDOInfo:public CGameInfo 
{
private:

	//用于保存完整数据库执行操作的sql语句
	char operszSql[2048];

	//用于保存在远程数据库执行操作的sql语句
	char operRemoteSql[2048];

	//根据服务器名称获取需要的IP
	char localIP[20];

	//数据库名称
	char SDO_DBName[64];

	//日志数据库名称
	char Log_DBName[64];

	//装备数据库名称
	char Item_DBName[64];

	//记录日志信息
	char strLog[2048];
/*
	//角色ID
	int UserID;

	//操作员ID
	int UserByID;
	
	//帐号
	char SDO_Account[40];

	//角色昵称
	char SDO_NickName[40];

	//开始时间
	char SDO_BeginTime[40];

	//结束时间
	char SDO_EndTime[40];

	//家族ID
	int FamilyID;

	//家族名称
	char FamilyName[20];

	//保存页索引
	int index;

	//保存页面大小
	int iPageSize;

private:

	//用于保存查询接口返回的结果
	COperVector DBVect;

	//用于保存解析客户端数据得到的结果
	COperVector recvVect;

	//用于写日志信息
	CLogFile logFile;

	//用于中间的辅助操作
	COperatorSDO operSDO;
*/
public:

	CSDOInfo(void);
	~CSDOInfo(void);

	//初始化类中的变量
	virtual COperVector * initialize(CSession * pSess,CEnumCore::Message_Tag_ID MessageTagID, unsigned char * pbuf, int length);
	
public:

	//查询玩家基本资料信息
	vector<CGlobalStruct::TFLV> SDO_UserInfo(int userByID,char * ServerName, char * ServerIP,char * Account,char * NickName,int iIndex,int iPageSize);

	//查询玩家帐号信息
	vector<CGlobalStruct::TFLV> SDO_AccountInfo(int userByID,char * ServerName, char * ServerIP,char * Account,char * NickName,int iIndex,int iPageSize);

	//查询玩家物品
	vector<CGlobalStruct::TFLV> SDO_ItemInfo(int userByID,char * ServerName, char * ServerIP,char * Account, int UserID,int iIndex,int iPageSize);

	//查询玩家宠物信息
	vector<CGlobalStruct::TFLV> SDO_PetInfo(int userByID,char * ServerName, char * ServerIP,char * Account, int UserID,int iIndex,int iPageSize);

	//查看玩家的宠物删除信息
	vector<CGlobalStruct::TFLV> SDO_PetDrop(int userByID, char * ServerName, char * ServerIP,char * Account, int UserID,int iIndex,int iPageSize);

	//查询玩家账号登录记录
	vector<CGlobalStruct::TFLV> SDO_LoginLog(int userByID, char * ServerName, char * ServerIP,char * Account,int iIndex,int iPageSize);

	//查询玩家物品赠送记录
	vector<CGlobalStruct::TFLV> SDO_UserTrade_Log(int userByID, char * ServerName, char * ServerIP,char * Account,char * NickName,int itype, int iIndex,int iPageSize);

	//查询玩家消费记录
	vector<CGlobalStruct::TFLV> SDO_UserConsume(int userByID, char * ServerName, char * ServerIP,char * Account,char * NickName,int iMoneytype, char *ItemName, char *BeginTime, char *EndTime, int iIndex,int iPageSize);

	//查看玩家当前状态(服务器/房间/在线状态)
	vector<CGlobalStruct::TFLV> SDO_UserStatus(int userByID, char * ServerName, char * ServerIP,char * Account,int UserID,int iIndex,int iPageSize);

	//查看玩家婚姻状态
	vector<CGlobalStruct::TFLV> SDO_UserMarriage(int userByID, char * ServerName, char * ServerIP,char * Account,char * NickName,int UserID,int iIndex,int iPageSize);

	//查看玩家摇摇乐获得物品
	vector<CGlobalStruct::TFLV> SDO_YYItem_QUERY(int userByID, char * ServerName, char * ServerIP,char * Account,int iIndex,int iPageSize);

	//强制玩家下线
	vector<CGlobalStruct::TFLV> SDO_KickPlayer_Off(int userByID, char * ServerName, char * ServerIP,char * Account, int UserID);

	//查询玩家封停状态
	vector<CGlobalStruct::TFLV> SDO_UserBanStatus(int userByID, char * ServerName, char * ServerIP,char * Account,int iIndex,int iPageSize);

	//封停玩家帐号
	vector<CGlobalStruct::TFLV> SDO_Account_Close(int userByID, char * ServerName, char * ServerIP,char * Account,char *BanReason);

	//查询所有被封停帐号列表
	vector<CGlobalStruct::TFLV> SDO_QueryBan_All(int userByID, char * ServerName, char * ServerIP,int iIndex,int iPageSize);

	//解封玩家帐号
	vector<CGlobalStruct::TFLV> SDO_Account_Open(int userByID, char * ServerName, char * ServerIP,char * Account,char *UnbindReason);

	//查询公告信息列表
	vector<CGlobalStruct::TFLV> SDO_NoticeInfo(int userByID, char * ServerName, char * ServerIP,int iIndex,int iPageSize);

	//添加公告信息
	vector<CGlobalStruct::TFLV> SDO_AddNotice(int userByID, char * ServerName, char * ServerIP, char *BoardMessage, char *BeginTime, char *EndTime, int interval, int iNoticeType);

	//查询发送公告的大区列表信息
	vector<CGlobalStruct::TFLV> SDO_BoardCity_Query(int userByID, int taskID,int iIndex,int iPageSize);

	//修改公告信息
	vector<CGlobalStruct::TFLV> SDO_EditNotice(int userByID, char * ServerName, char * ServerIP, int taskID, char *BoardMessage, char *BeginTime, char *EndTime, int interval,int status,int iNoticeType);

	//查询家族基本信息
	vector<CGlobalStruct::TFLV> SDO_FamilyInfo(int userByID, char * ServerName, char * ServerIP, char *FamilyName, int index, int iPageSize);

	//查询家族成员信息
	vector<CGlobalStruct::TFLV> SDO_FamilyMember(int userByID, char * ServerName, char * ServerIP, int FamilyID, char *FamilyName, int iIndex, int iPageSize);

	//查询申请中家族成员信息
	vector<CGlobalStruct::TFLV> SDO_FamilyFormal(int userByID, char * ServerName, char * ServerIP, int FamilyID, char *FamilyName, int iIndex, int iPageSize);

	//查询家族勋章信息
	vector<CGlobalStruct::TFLV> SDO_FamilyBadge(int userByID, char * ServerName, char * ServerIP, int FamilyID, char *FamilyName, int iIndex, int iPageSize);

	//查询家族金库捐赠明细
	vector<CGlobalStruct::TFLV> SDO_FamilyDonate(int userByID, char * ServerName, char * ServerIP, int FamilyID, char *FamilyName, char *BeginTime, char *EndTime,int iIndex, int iPageSize);

	//查询家族购买记录
	vector<CGlobalStruct::TFLV> SDO_FamilyConsume(int userByID, char * ServerName, char * ServerIP, int FamilyID, char *FamilyName, char *BeginTime, char *EndTime,int iIndex, int iPageSize);

	//修改家族成员职务
	vector<CGlobalStruct::TFLV> SDO_ModiMemDuty(int userByID, char * ServerName, char * ServerIP, char *NickName, int UserID, int FamilyID, char *FamilyName, int type);

	//解散家族
	vector<CGlobalStruct::TFLV> SDO_DeleteFamily(int userByID, char * ServerName, char * ServerIP, int FamilyID, char *FamilyName);

	//添加家族徽章
	vector<CGlobalStruct::TFLV> SDO_Insert_FamilyBadge(int userByID, char * ServerName, char * ServerIP, char *NickName, int UserID, int FamilyID, char *FamilyName, int EnblemType, int EnblemNum);

	//删除家族徽章
	vector<CGlobalStruct::TFLV> SDO_Delete_FamilyBadge(int userByID, char * ServerName, char * ServerIP, char *NickName, int UserID, char *FamilyName, int EmblemType);

	//查看玩家G币
	vector<CGlobalStruct::TFLV> SDO_UserGCash_Query(int userByID, char * ServerName, char * ServerIP, char *Account);

	//补发玩家G币
	vector<CGlobalStruct::TFLV> SDO_UserGCash_Update(int userByID, char * ServerName, char * ServerIP, char *Account, int GCash);

	//大赛挑战信息查询
	vector<CGlobalStruct::TFLV> SDO_CHALLENGE_QUERY(int userByID, char * ServerName, char * ServerIP, int iIndex, int iPageSize);

	//设置大赛挑战信息
	vector<CGlobalStruct::TFLV> SDO_CHALLENGE_INSERT(int userByID, char * ServerName, char * ServerIP, int weekDay, int matPtMin, int stPtMin, int GCash, int scence, int isBattle);
			
	//修改大赛挑战信息
	vector<CGlobalStruct::TFLV> SDO_CHALLENGE_UPDATE(int userByID, char * ServerName, char * ServerIP, int ChallengeID, int weekDay, int matPtMin, int stPtMin, int GCash, int scence, int isBattle);
			
	//删除大赛挑战信息
	vector<CGlobalStruct::TFLV> SDO_CHALLENGE_DELETE(int userByID, char * ServerName, char * ServerIP, int ChallengeID);

	//查看游戏场景列表
	vector<CGlobalStruct::TFLV> SDO_SCENE_QUERY(int userByID, char * ServerName, char * ServerIP);

	//添加大赛挑战的场景
	vector<CGlobalStruct::TFLV> SDO_SCENE_CREATE(int userByID, char * ServerName, char * ServerIP, char *SceneName);
	
	//修改大赛挑战的场景
	vector<CGlobalStruct::TFLV> SDO_SCENE_UPDATE(int userByID, char * ServerName, char * ServerIP, int SceneID, char *SceneName);
			
	//删除大赛挑战的场景
	vector<CGlobalStruct::TFLV> SDO_SCENE_DELETE(int userByID, char * ServerName, char * ServerIP, int SceneID);

	//查看在游戏里摇摇乐获得概率
	vector<CGlobalStruct::TFLV> SDO_YYHappyItem_Query(int userByID, char * ServerName, char * ServerIP, int iIndex, int iPageSize);
	
	//添加摇摇乐获得概率
	vector<CGlobalStruct::TFLV> SDO_YYHappyItem_Insert(int userByID, char * ServerName, char * ServerIP, int itemCode1, int itemCode2, int level, int levPercent, int precent, int timeslimit, int dayslimit, char *itemName1, char *itemName2);
		
	//更新摇摇乐获得概率
	vector<CGlobalStruct::TFLV> SDO_YYHappyItem_Update(int userByID, char * ServerName, char * ServerIP, int itemCode, int itemCode1, int itemCode2, int level, int levPercent, int precent, int timeslimit, int dayslimit, char *itemName1, char *itemName2);
	
	//删除摇摇乐获得概率
	vector<CGlobalStruct::TFLV> SDO_YYHappyItem_Delete(int userByID, char * ServerName, char * ServerIP, int itemCode);
	
	//查看游戏里面的道具信息
	vector<CGlobalStruct::TFLV> SDO_Item_QueryAll(int userByID, char * ServerName, char * ServerIP, int BigType, int SmallType, int sex, char * itemName);

	//道具获取比率查询
	vector<CGlobalStruct::TFLV> SDO_MedalItem_Query(int userByID, char * ServerName, char * ServerIP, int iIndex, int iPageSize);
			
	//单个道具获取比率查询
	vector<CGlobalStruct::TFLV> SDO_MedalItem_Owner_Query(int userByID, char * ServerName, char * ServerIP, char *itemName, int iIndex, int iPageSize);
			
	//添加道具获取比率
	vector<CGlobalStruct::TFLV> SDO_MedalItem_Insert(int userByID, char * ServerName, char * ServerIP, int itemCode, int precent, char *itemName);
			
	//更新道具获取比率
	vector<CGlobalStruct::TFLV> SDO_MedalItem_Update(int userByID, char * ServerName, char * ServerIP, int itemCode, int precent, char *itemName);
			
	//删除道具获取比率
	vector<CGlobalStruct::TFLV> SDO_MedalItem_Delete(int userByID, char * ServerName, char * ServerIP, int itemCode, char *itemName);

	//更新玩家角色信息
	vector<CGlobalStruct::TFLV> SDO_CharacterInfo_Update(int userByID, char * ServerName, char * ServerIP, int UserID, char *Account,int level, int experience, int battle, int win, int draw, int lose, int MCash, int GCash);
/*			
	//查看玩家身上道具
	bool SDO_UserEquip_Query();
*/			
	//删除玩家身上道具
	vector<CGlobalStruct::TFLV> SDO_ItemShop_Delete(int userByID, char * ServerName, char * ServerIP, int UserID, char *Account, int itemCode, char *itemName);
			
	//查看玩家礼物盒道具
	vector<CGlobalStruct::TFLV> SDO_GiftBox_Query(int userByID, char * ServerName, char * ServerIP, int UserID, char *Account, int iIndex, int iPageSize);
			
	//删除玩家礼物盒道具
	vector<CGlobalStruct::TFLV> SDO_GiftBox_Delete(int userByID, char * ServerName, char * ServerIP, int UserID, char *Account, int itemCode, char *itemName);
			
	//给玩家发送道具
	vector<CGlobalStruct::TFLV> SDO_GiftBox_Insert(int userByID, char * ServerName, char * ServerIP, int UserID, char *Account, int itemCode, int timeslimit, int DateLimit, char *itemName, char *title, char *content);

	//批量添加比赛信息
	vector<CGlobalStruct::TFLV> SDO_Challenge_InsertAll(int userByID, char * ServerName, char * ServerIP, int weekDay, int matPtMin, int stPtMin, int GCash, int scence, int isBattle);
	
	//获取公告类型 <iSouNT: 1 及时公告，0 定时公告
	void GetNoticeType(char *strDesNT,int &iDesNT, int iSouNT);

};
#endif




















