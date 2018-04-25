#ifndef WAInfo_H_
#define WAInfo_H_

#include "OperatorWA.h"
#include "MySocket.h"

#include "..\\GameIOCP\\GameInfo.h"
#include "ASTA.h"
#include "ATAS.h"

using namespace MSG_ASTA;
using namespace MSG_ATAS;

class  CWAInfo:public CGameInfo
{
public:
	CWAInfo(void);
	~CWAInfo(void);
public:

	vector <CGlobalStruct::TFLV> initialize(CSession * pSess,unsigned char * pbuf, int length);

	//连接远程服务端
	bool Connect_Remote_Login(char * ServerIP, int gamedbid=1);

	//角色信息查询(帐号、角色)
	vector <CGlobalStruct::TFLV> WA_Net_UserInfo(char * ServerIP,char * UserName, char * Character,int iIndex, int iPageSize);

	//内部调用：通过角色名查询角色信息
	void WA_Net_UserInfoByChar(char * Character,vector <CGlobalStruct::TFLV>* pDBTFLV,int PageCount);

	//好友信息查询
	vector <CGlobalStruct::TFLV> WA_Net_FriendInfo(char *ServerIP, char *GameRegion, char *Account, char *Character, int iIndex, int iPageSize);

	//物品信息查询(身上、背包、银行)
	vector <CGlobalStruct::TFLV> WA_Net_ItemInfo(char *ServerIP, char *GameRegion, char *Account, char *Character, int itemPos, int iIndex, int iPageSize);
	
	//徽章信息查询
	vector <CGlobalStruct::TFLV> WA_Net_EmblemInfo(char *ServerIP, char *GameRegion, char *Account, char *Character, int iIndex, int iPageSize);

	//宠物信息查询
	vector <CGlobalStruct::TFLV> WA_Net_PetInfo(char *ServerIP, char *GameRegion, char *Account, char *Character, int petNo, int iIndex, int iPageSize);

	//邮箱信息查询
	vector <CGlobalStruct::TFLV> WA_Net_MailInfo(char *ServerIP, char *GameRegion, char *Account, char *Character, int iIndex, int iPageSize);
	
	//帐号登录信息查询
	vector <CGlobalStruct::TFLV> WA_Net_AccountLoginInfo(char *ServerIP, char *GameRegion, char *Account,char *charName,char *Begintime, char *Endtime, int iIndex, int iPageSize);

	//封停列表查询
	vector <CGlobalStruct::TFLV> WA_Net_CloseList(char *ServerIP, char *GameRegion, char *UserName, int iIndex, int iPageSize);

	//Added by tengjie 2010-05-14:单个帐号封停查询(帐号)
	vector <CGlobalStruct::TFLV> WA_Net_CloseSingle(char * ServerIP, char * UserName, int iIndex, int iPageSize);

	//内部调用查询单个帐号封停
	void WA_Net_SingleCloseInfo(char *UserName, vector <CGlobalStruct::TFLV>* pDBTFLV, int PageCount);
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
	//公告添加
	vector <CGlobalStruct::TFLV> WA_BoardTasker_Add(int userByID, char * ServerIP, char *BoardMessage, char *BeginTime, char *EndTime, int interval);

	//公告修改
	vector <CGlobalStruct::TFLV> WA_BoardTasker_Update(int userByID, char * ServerIP, int Taskid, char *BoardMessage, char *BeginTime, char *EndTime, int interval);

	//公告删除	
	vector <CGlobalStruct::TFLV> WA_BoardTasker_Delete(int userByID, int Taskid);

	//踢人(帐号、角色)
	vector <CGlobalStruct::TFLV> WA_Net_KickPlayer(int userByID,char * ServerIP, char * UserName, char * Character);
	
	//帐号封停(帐号、角色)
	vector <CGlobalStruct::TFLV> WA_Net_ClosePlayer(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character,int iTime, int Reason, char *description);

	//批量封停帐号
	vector <CGlobalStruct::TFLV> WA_Net_ClosePlayerGroup(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character,int iTime, int Reason, char *description);

	//帐号解封(帐号、角色)
	vector <CGlobalStruct::TFLV> WA_Net_OpenPlayer(int userByID, char * ServerIP, char * UserName, int CharacterID, char * Character);

	//道具删除
	vector <CGlobalStruct::TFLV> WA_Net_DeleteItem(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character, int itemIndex, int itemPos);
	vector <CGlobalStruct::TFLV> WA_Net_ModifyLevel(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character, int level);

	//修改金钱
	vector <CGlobalStruct::TFLV> WA_Net_ModifyMoney(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character, int CurMoney, int newMoney);
	vector<CGlobalStruct::TFLV>  WA_ITEMType_QUERY(char* gameName,char* strSql,int index,int Page);
	vector<CGlobalStruct::TFLV>  WA_ITEM_QUERY(char* gameName,char* strSql,char* Item_Type,char* Item_Name,int index,int Page);
	vector<CGlobalStruct::TFLV>  WA_SkillList_QUERY(char* gameName,char* strSql,char* Skill_Name,int index,int Page);
	vector <CGlobalStruct::TFLV> WA_RoleSkill_Query(char *ServerIP, char *GameRegion, char *Account,char *Character, int iIndex, int iPageSize);
	vector <CGlobalStruct::TFLV> WA_ModfiyPlayerSkill(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character, int skillID,char *skillname,int skilllvl);
	//寄送道具
//	vector <CGlobalStruct::TFLV> WA_SendMailItem(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character, int itemId,int itemCount,char *itemName);
	vector <CGlobalStruct::TFLV> WA_SendMailItem(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character, char *itemId,
		char *itemCount,char *itemName, int iMailTag, char *ItemGen, char *MailSub, char *MailContent, int iMoney, char *ItemExpire);
	
	//修改角色名
	vector <CGlobalStruct::TFLV> WA_RoleName_Modfiy(int userByID,char * ServerIP, char * UserName,char* Character,char * newRole);
	//恢复玩家角色
	vector <CGlobalStruct::TFLV> WA_RestoreRoleInfo(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character);
	//玩家密码查询
	vector<CGlobalStruct::TFLV> WA_UserPwd_QUERY(char* gameName,char* ServerIP,char* serverName,char* account,int index,int Page);
	//修改密码
	vector <CGlobalStruct::TFLV> WA_ChangeLoginPasswd(int userByID,char * ServerIP, char * UserName,int CharacterID,char * newPwd);
	//=====================================================================log========================================================================================
	
	//Added by tengjie 2010-06-30
	//恢复玩家密码
	vector <CGlobalStruct::TFLV> WA_ResumePassw(int userByID, char *ServerIP, char *Account);
	string WA_GetOldPassWord(char *ServerIP, char *Account);
	//End

	// 物品:怪物掉落
	vector <CGlobalStruct::TFLV> WA_Log_MonsterDropItem(char *ServerIP, char *GameRegion,char *Account, char *Character,char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize);

	// 物品:从NPC处购买
	vector <CGlobalStruct::TFLV> WA_Log_BuyItemNPC(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize);

	// 物品:卖给NPC
	vector <CGlobalStruct::TFLV> WA_Log_SellItemTONPC(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime,int nickID, int iIndex, int iPageSize);

	// 物品:玩家自行删除
	vector <CGlobalStruct::TFLV> WA_Log_DropItem(char *ServerIP, char *GameRegion, char *Account,char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize);
	
	// 物品:交易获得
	vector <CGlobalStruct::TFLV> WA_Log_GetItemTrade(char *ServerIP, char *GameRegion, char *Account,char *Character,  char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize);
	
	// 物品:交易失去
	vector <CGlobalStruct::TFLV> WA_Log_LoseItemTrade(char *ServerIP, char *GameRegion, char *Account,char *Character,  char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize);

	// 物品:装备绑定记录
	vector <CGlobalStruct::TFLV> WA_Log_BindItem(char *ServerIP, char *GameRegion, char *Account,char *Character,  char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize);

	// 物品:装备解绑定记录
	vector <CGlobalStruct::TFLV> WA_Log_UnBindItem(char *ServerIP, char *GameRegion, char *Account,char *Character,  char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize);

	// 物品:角色“摆摊”日志查询
	vector <CGlobalStruct::TFLV> WA_Log_BoothItem(char *ServerIP, char *GameRegion, char *Account,char *Character,  char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize);	

	// 物品:道具删除记录(指使用时间到了)
	vector <CGlobalStruct::TFLV> WA_Log_DeleteItem(char *ServerIP, char *GameRegion, char *Account, char *Character,  char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize);

	// 物品:道具使用记录
	vector <CGlobalStruct::TFLV> WA_Log_UseItem(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize);

	// 物品:装备制作记录
	vector <CGlobalStruct::TFLV> WA_Log_CraftItem(char *ServerIP, char *GameRegion,char *Account, char *Character,  char *Begintime,char *Endtime,int nickID, int iIndex, int iPageSize);

	// 物品:装备打孔
	vector <CGlobalStruct::TFLV> WA_Log_PunchItem(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize);

	// 物品:装备改孔
	vector <CGlobalStruct::TFLV> WA_Log_ChangeSlotItem(char *ServerIP, char *GameRegion, char *Account, char *Character,char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize);

	// 物品:装备升星
	vector <CGlobalStruct::TFLV> WA_Log_UpgradeStar(char *ServerIP, char *GameRegion,char *Account, char *Character,  char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize);

	// 物品:宝石
	vector <CGlobalStruct::TFLV> WA_Log_Gem(char *ServerIP, char *GameRegion,  char *Account, char *Character,char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize);

	// 物品:制药记录
	vector <CGlobalStruct::TFLV> WA_Log_Pharma(char *ServerIP, char *GameRegion, char *Account,char *Character, char *Begintime,char *Endtime,int nickID, int iIndex, int iPageSize);

	// 物品:宝石合成
	vector <CGlobalStruct::TFLV> WA_Log_CraftGem(char *ServerIP, char *GameRegion,char *Account,char *Character,char *Begintime,char *Endtime,int nickID, int iIndex, int iPageSize);

	// 物品:材料合成
	vector <CGlobalStruct::TFLV> WA_Log_CraftMatirail(char *ServerIP, char *GameRegion, char *Account,char *Character, char *Begintime,char *Endtime,int nickID, int iIndex, int iPageSize);

	// 物品:图纸合成
	vector <CGlobalStruct::TFLV> WA_Log_CraftRecipe(char *ServerIP, char *GameRegion,char *Account, char *Character,char *Begintime,char *Endtime,int nickID, int iIndex, int iPageSize);

	// 迷宫积分兑换奖励
	vector <CGlobalStruct::TFLV> WA_Log_MazeIntegral(char *ServerIP, char *GameRegion, char *Account,char *Character, char *Begintime,char *Endtime,int nickID, int iIndex, int iPageSize);

	// 游戏币:怪物掉落
	vector <CGlobalStruct::TFLV> WA_Log_MonsterDropMoney(char *ServerIP, char *GameRegion,char *Account,char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize);

	// 游戏币:任务获得
	vector <CGlobalStruct::TFLV> WA_Log_GetMoneyQuest(char *ServerIP, char *GameRegion, char *Account,char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize);

	// 游戏币:修装
	vector <CGlobalStruct::TFLV> WA_Log_LoseMoneyRepair(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize);

	// 游戏币:交易获得
	vector <CGlobalStruct::TFLV> WA_Log_GetMoneyTrade(char *ServerIP, char *GameRegion, char *Account, char *Character,char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize);

	// 游戏币:交易失去
	vector <CGlobalStruct::TFLV> WA_Log_LoseMoneyTrade(char *ServerIP, char *GameRegion, char *Account, char *Character,char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize);

	// 任务:完成获得记录
	vector <CGlobalStruct::TFLV> WA_Log_QuestReward(char *ServerIP, char *GameRegion, char *Account, char *Character,char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize);

	// 任务:任务物品删除记录
	vector <CGlobalStruct::TFLV> WA_Log_QuestGive(char *ServerIP, char *GameRegion, char *Account, char *Character,char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize);

	// 邮件:发送记录
	vector <CGlobalStruct::TFLV> WA_Log_MailSend(char *ServerIP, char *GameRegion, char *Account, char *Character,char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize);

	// 邮件:接收记录
	vector <CGlobalStruct::TFLV> WA_Log_MailRecv(char *ServerIP, char *GameRegion, char *Account, char *Character,char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize);

	// 邮件:删除记录
	vector <CGlobalStruct::TFLV> WA_Log_MailDelete(char *ServerIP, char *GameRegion, char *Account, char *Character,char *Begintime,char *Endtime,int nickID, int iIndex, int iPageSize);

	// 商城:购买记录
	vector <CGlobalStruct::TFLV> WA_Log_ItemShop(char *ServerIP, char *GameRegion, char *Account, char *Character,char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize);

	// 查询用户的副本日志
	vector <CGlobalStruct::TFLV> WA_Log_Instance(char *ServerIP, char *GameRegion, char *Account, char *Character,char *Begintime,char *Endtime,int nickID, int iIndex, int iPageSize);

	// 玩家登陆/登出日志
	vector <CGlobalStruct::TFLV> WA_Log_LoginLogout(char *ServerIP, char *GameRegion, char *Account, char *Character,char *Begintime,char *Endtime,int nickID, int iIndex, int iPageSize);
	
	
	//Added by tengjie 2010-05-13:本地记录封停原因,不再显示给用户
	void WA_SqlForUserInfo(char *UserName,char *ServerIP,int iTime,char *description,int iStatus = 0);
	CString WA_SearchDescription(char *UserName,int iStatus);
//	bool JudgeIsExist(vector <REP_LOG_MONSTER_DROP_ITEM_ELE> &vect_LogInfo, REP_LOG_MONSTER_DROP_ITEM_ELE elem);

	//Added by tengjie 2010-06-11
	//角色日志:角色升级记录
	vector <CGlobalStruct::TFLV> WA_Character_Upgrade_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize);

	//角色日志:角色创建记录查询
	vector <CGlobalStruct::TFLV> WA_Character_Create_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize);

	//角色日志:角色删除记录查询
	vector <CGlobalStruct::TFLV> WA_Character_Delete_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize);

	//角色日志:加入帮会
	vector <CGlobalStruct::TFLV> WA_Join_Guild_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize);

	//角色日志:离开帮会
	vector <CGlobalStruct::TFLV> WA_Quit_Guild_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize);

	//角色日志:创建帮会
	vector <CGlobalStruct::TFLV> WA_Create_Guild_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize);

	//角色日志:解散帮会
	vector <CGlobalStruct::TFLV> WA_Dissolve_Guild_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize);

	//角色日志:转职信息查询
	vector <CGlobalStruct::TFLV> WA_Transfer_Info_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize);

	//角色日志:技能日志记录
	vector <CGlobalStruct::TFLV> WA_Skills_Log_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize);

	//角色日志:生活技能日志记录
	vector <CGlobalStruct::TFLV> WA_LifeSkills_Log_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize);

	//佣兵团日志:创建记录
	vector <CGlobalStruct::TFLV> WA_CreateMercenary_Log_Query(char *ServerIP, char *GuildName, char *Begintime,char *Endtime, int iIndex, int iPageSize);
	
	//佣兵团日志:解散记录
	vector <CGlobalStruct::TFLV> WA_DissolveMercenary_Log_Query(char *ServerIP, char *GuildName, char *Begintime,char *Endtime, int iIndex, int iPageSize);

	//宠物徽章日志:宠物精魄捕捉记录
	vector <CGlobalStruct::TFLV> WA_CatchPetSpirit_Log_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize);
	
	//宠物徽章日志
	//精魄交易记录:交易获得
	vector <CGlobalStruct::TFLV> WA_GainSpirit_Log_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize);
	
	//宠物徽章日志
	//精魄交易记录:交易失去
	vector <CGlobalStruct::TFLV> WA_LoseSpirit_Log_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize);
	
	//宠物徽章日志:精魄出售记录
	vector <CGlobalStruct::TFLV> WA_SellSpirit_Log_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize);
	
	//用户信息查询:技能查询
	vector <CGlobalStruct::TFLV> WA_SkillsInfo_Log_Query(char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize);
	
	//用户信息查询:任务查询
	vector <CGlobalStruct::TFLV> WA_MissionsCompleted_Log_Query(char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize);

	//佣兵团信息查询
	vector <CGlobalStruct::TFLV> WA_MercenaryGroup_Query(char *ServerIP, char *MercenaryGroup_Name, int iIndex, int iPageSize);
		
	//佣兵团成员列表查询
	vector <CGlobalStruct::TFLV> WA_MemberList_Info_Query(char *ServerIP, char *MercenaryGroup_Name, int iIndex, int iPageSize);

	//操作类:
	//宠物管理:添加宠物(等级\属性)
	vector <CGlobalStruct::TFLV> WA_PetAdded_Operater(int userByID, char * ServerIP, char* Character, int PetUniqueID);
	
	//宠物管理:删除宠物(等级\属性)
	vector <CGlobalStruct::TFLV> WA_PetDeleted_Operater(int userByID, char * ServerIP, char* Character, int PetTypeID);

	//家族管理:转让会长权限
	vector <CGlobalStruct::TFLV> WA_AttornPopedom_Operater(int userByID, char * ServerIP, char * Character, char* GuildName);

	//公会管理:修改公会名称
	vector <CGlobalStruct::TFLV> WA_ModifyGuildName_Operater(int userByID, char * ServerIP, char * GuildName, char* NewGuildName);
	
	//公会管理:踢出公会成员
	vector <CGlobalStruct::TFLV> WA_KickGuildMember_Operater(int userByID, char * ServerIP, char * Character, char* GuildName);

	//公会管理:解散公会
	vector <CGlobalStruct::TFLV> WA_DissolveGuild_Operater(int userByID, char * ServerIP, char* GuildName);

	//宠物列表查询
	vector <CGlobalStruct::TFLV> WA_PET_QUERY(char* gameName,char* strSql,int index,int Page);

	// 玩家登陆/登出日志(按ip方式查询)
	vector <CGlobalStruct::TFLV> WA_Log_LoginLogoutByIP(char *ServerIP, char *Begintime,char *Endtime, char* LoginIP, int iIndex, int iPageSize);

	//玩家角色属性恢复		
	vector <CGlobalStruct::TFLV> WA_ResumeAttribute_Operate(int userByID, char * ServerIP, char *Account, char *Character);
	
	//批量发送邮件(无道具)
	vector <CGlobalStruct::TFLV> WA_SendMails_Operate(int userByID, char * ServerIP, char *Character, char *MailSub, char *MailContent);
	
//	string UserLogInterface(char *ServerIP,string NewUserName,string NewTime,string BanType = "0");
	//End
	
	//角色物品信息查询
	//	vector <CGlobalStruct::TFLV> WA_Net_BodyItemInfo(char *ServerIP, char *GameRegion, char *Account, char *Character, int iIndex, int iPageSize);
	
	//角色背包信息查询
	//	vector <CGlobalStruct::TFLV> WA_Net_BagItemInfo(char *ServerIP, char *GameRegion, char *Account, char *Character, int iIndex, int iPageSize);
	
	//角色银行信息查询
	//	vector <CGlobalStruct::TFLV> WA_Net_BankItemInfo(char *ServerIP, char *GameRegion, char *Account, char *Character, int iIndex, int iPageSize);

protected:
//	CMSocket msocket;
	CMySocket m_MySocket;
	COperatorWA m_OperatorWA;
	char operRemoteSql[2048];

	//本地数据库IP地址
	char LocalIP[30];
private:
	int srv_ClusterID;
	int srv_Realmid;
};
#endif




















