#ifndef PALLOGINFO_H_
#define PALLOGINFO_H_
#define GameDB 1
#define ItemDB 2
#define LoginDB 3
#define LogDB 4
#define GateWay 5
#define Messenger 6

#include "OperatorJW2.h"

#include "..\\GameIOCP\\GameInfo.h"

class CJW2Info:public CGameInfo
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
		

public:

	//构造函数
	CJW2Info();

	//析构函数
	~CJW2Info();


	//根据帐号踢人
	vector<CGlobalStruct::TFLV> KickPlayer(int userByID,char * ServerName, char * ServerIP,char * UserName);
	//根据帐号封停
	vector<CGlobalStruct::TFLV> MakeClose(int userByID,char * ServerName, char * ServerIP ,char * UserName,char * UserNick,int JW2_UserSN, char* Reason);
	//根据帐号解封
	vector<CGlobalStruct::TFLV> MakeOpen(int userByID,char * ServerName, char * ServerIP ,char * UserName,char * UserNick,int JW2_UserSN,char* Reason);

	//查看购物，送礼（7－10）
	vector<CGlobalStruct::TFLV> JW2_SMALL_PRESENT_QUERY(char* ServerName,char * ServerIP,char * UserName,int JW2_UserSN,int iType,int iIndex,int iPageSize);

	//玩家人物登入/登出信息
	vector<CGlobalStruct::TFLV> LoginInfo(char * ServerName, char * ServerIP,char * UserName, char * UserNick,char * IP,char * begintime, char * endtime,int iIndex, int iPageSize);
	
	//查询家族信息
	vector<CGlobalStruct::TFLV> JW2_FAMILYINFO_QUERY(char * ServerName, char * ServerIP,char * FamilyName, int iIndex,int iPageSize);

	vector<CGlobalStruct::TFLV> JW2_Family_Detail(CEnumCore::Message_Tag_ID m_tagID,char * ServerName, char * ServerIP,char * FamilyName, int FamilyID,int iIndex,int iPageSize);

	//修改密码
	vector<CGlobalStruct::TFLV>  JW2_MODIFY_PWD(int userByID, char * ServerName,char * ServerIP,char * UserName,int JW2_UserSN,char * szNewPw);

	//恢复密码
	vector<CGlobalStruct::TFLV> JW2_RECALL_PWD(int userByID, char * ServerName,char * ServerIP,char * UserName,int JW2_UserSN);

	//修改等级
	vector<CGlobalStruct::TFLV> JW2_MODIFYLEVEL_QUERY(int userByID, char * ServerName,char * ServerIP,char * UserName,int JW2_UserSN,int iLevel);
	
	//修改经验
	vector<CGlobalStruct::TFLV> JW2_MODIFYEXP_QUERY(int userByID, char * ServerName,char * ServerIP,char * UserName,int JW2_UserSN,int iExp);

	//查询道具
	vector<CGlobalStruct::TFLV> JW2_ItemInfo_Query(char* ServerName,char * ServerIP,char * UserName,int JW2_UserSN,int iItemPos,int iIndex,int iPageSize);

	//删除道具
	vector<CGlobalStruct::TFLV> JW2_ITEM_DEL(int userByID,char * ServerName,char * ServerIP,char * UserName, int JW2_UserID,char * avatarItem,char * avatarItemName,int iItemPos, int iItemNo);

	//添加道具
	vector<CGlobalStruct::TFLV> JW2_ADD_ITEM(int userByID,char * ServerName, char * ServerIP ,char* UserName,int JW2_UserSN,
							char * strMailTitle,char * strMailContent,char* JW2_AvatarItem,int count);

	//修改金钱
	vector<CGlobalStruct::TFLV> JW2_MODIFY_MONEY(int userByID, char * ServerName,char * ServerIP,char * UserName,int JW2_UserSN,int iMoney);

	//模糊道具查询
	vector<CGlobalStruct::TFLV> JW2_ITEM_SELECT(int userByID,char * JW2_AvatarItemName);
	
	//基地信息查询
	vector<CGlobalStruct::TFLV> JW2_BasicInfo_Query(char * ServerName, char * ServerIP,char * FamilyName, int iIndex,int iPageSize);


	
	//金钱日志查询
	vector<CGlobalStruct::TFLV> JW2_MoneyLog_Query(char * ServerName, char * ServerIP,char* UserName,
						 int JW2_UserSN,int iType,char * szStartTime,char * szEndTime,int iIndex,int iPageSize);
	
	//基金日志
	vector<CGlobalStruct::TFLV> JW2_FamilyFund_Log(char * ServerName, char * ServerIP,char* FamilyName,
						 int FamilyID,int iStatus,char * szStartTime,char * szEndTime,int iIndex,int iPageSize);
	
	//家族道具购买日志
	vector<CGlobalStruct::TFLV> JW2_FamilyBuyLog_Query(char * ServerName, char * ServerIP,char* FamilyName,
						 int FamilyID,char * szStartTime,char * szEndTime,int iIndex,int iPageSize);
	
	//消费日志
	vector<CGlobalStruct::TFLV> JW2_CashMoney_Log(char * ServerName, char * ServerIP,char* UserName,int JW2_UserSN,int iType,char * szStartTime,char * szEndTime,int iIndex,int iPageSize);
	
	//批量踢人
	vector<CGlobalStruct::TFLV> JW2_Import_KickPlayer();
	
	//批量封停
	vector<CGlobalStruct::TFLV> JW2_Import_AccountClose();
	
	//批量解封
	vector<CGlobalStruct::TFLV> JW2_Import_AccountOpen();
	
	//更改GM状态
	vector<CGlobalStruct::TFLV> JW2_GM_Update(int userByID, char * ServerName,char * ServerIP,char * UserName,int JW2_UserSN,int iStatus);

	//插入权限
	vector<CGlobalStruct::TFLV> JW2_ProductManage_Insert(int userByID,char * ServerName,char * ServerIP,char * JW2_AvatarItem,	char * GMAccount,char * begintime,char * endtime,int totalprice);
		
	//查看权限
	vector<CGlobalStruct::TFLV> JW2_ProductManage_View(int iIndex, int iPageSize);
	
	//删除权限
	vector<CGlobalStruct::TFLV> JW2_ProductManage_Del(char * ServerName, char * ServerIP,int userByID,int iTaskid);

	//maple add
	//验证权限
	vector<CGlobalStruct::TFLV> JW2_ProductManage_Validate(char * ServerName,char * ServerIP, int GMID,char * JW2_AvatarItem);

	//验证通过在权限表中减去
	vector<CGlobalStruct::TFLV> JW2_ProductManage_Deduct(int userByID,int GMID,char * ServerIP, char * JW2_AvatarItem);
};
#endif