#pragma once

#include "OperatorXDJG.h"
#include "..\\GameIOCP\\GameInfo.h"

#include "..\\OperVector\\MyEvent.h"
#pragma comment(lib,"..\\OperVector\\debug\\OperVector.lib")

#include "XDJG_StructInfo.h"

#define HTTPPORT   8082

class CMyEvent;

//Added by tengjie 2010-07-08
//一级属性
#define NONE				0x0000
#define POWER				0x0001		//力量
#define AGILITY				0x0002		//敏捷
#define STAMINA				0x0003		//体力
#define SPIRIT				0x0004		//灵力
#define MAGIC				0x0005		//异能
//二级属性
#define AP					0x0006		//怒气
#define HP					0x0007		//生命值
#define MP					0x0008		//魔法值
#define APMAX				0x0009		//怒气
#define HPMAX				0x000a		//生命最大值
#define MPMAX				0x000b		//魔法最大值
#define ARMORING			0x000c		//护甲值
#define MOVINGSPEED			0x000d		//移动速度
#define HANDSPEED			0x000e		//手速
#define MAINHANDSPEED		0x000f		//主手武器攻击速度
#define SUBHANDSPEED		0x0010		//副手武器攻击速度
#define DOUBLEHANDSPEED		0x0011		//双手武器攻击速度
#define LONGDISHANDSPEED	0x0012		//远程武器攻击速度
#define NEARPA				0x0013		//近身物理攻击力
#define SLAVENEARPA			0x0014		//副手近身攻击力
#define FARPA				0x0015		//远程物理攻击力
#define SUPERATTACK			0x0016		//异能攻击力

//各种几率
#define PHYSICALHITRATE		0x0017		//物理命中率
#define MAGICALHITRATE		0x0018		//异能命中率
#define DODGERATE			0x0019		//躲闪率
#define BLOCKRATE			0x001a		//格挡率
#define PARRYINGRATE		0x001b		//招架率
#define PERFECTRATE			0x001c		//会心一击率
#define STRENGTHCRITRATE	0x001d		//物理致命一击率
#define SPIRITCRITRATE		0x001e		//异能致命一击率
#define CRITRESISTANCERATE	0x001f		//致命抵抗率
#define DAMAGESHORTDISTANCE	0x0020		//近身物理伤害值
#define DAMAGELONGDISTANCE	0x0021		//远程物理伤害值
#define DAMAGESPIRITE		0x0022		//异能伤害值
#define DOCURE				0x0023		//治疗效果

//各系抗性
#define FASTNESSFIRE		0x0024		//火焰抗性
#define FASTNESSFROST		0x0025		//冰霜抗性
#define FASTNESSTHUNDER		0x0026		//雷电抗性
#define FASTNESSERODE		0x0027		//腐蚀抗性
#define FASTNESSENERGY		0x0028		//能量抗性
#define FASTNESSSPIRIT		0x0029		//精神抗性

//各系伤害
#define DODAMAGEFIRE		0x002a		//产生火焰伤害(光能)
#define DODAMAGEFROST		0x002b		//产生冰霜伤害(暗能)
#define DODAMAGETHUNDER		0x002c		//产生雷电伤害(电能)
#define DODAMAGEERODE		0x002d		//产生腐蚀伤害(微生物能)
#define DODAMAGEENERGY		0x002e		//产生能量伤害(场能)
#define DODAMAGESPIRIT		0x002f		//产生精神伤害(电磁能)

//三级属性
#define ONNEARAP			0x0030		//受到近身物理伤害值
#define ONFARAP				0x0031		//受到远程物理伤害值
#define ONCURE				0x0032		//受到的治疗值

//受到的各系伤害
#define ONDAMAGEFIRE		0x0033		//受到火焰伤害
#define ONDAMAGEFROST		0x0034		//受到冰霜伤害
#define ONDAMAGETHUNDER		0x0035		//受到雷电伤害
#define ONDAMAGEERODE		0x0036		//受到腐蚀伤害
#define ONDAMAGEENERGY		0x0037		//受到能量伤害
#define ONDAMAGESPIRIT		0x0038		//受到精神伤害
#define PRIMARYPA			0x0039		//主手伤害
#define STEALTH				0x003a		//潜行能力
#define DETECT				0x003b		//反潜能力
#define DOCUREPERCENT		0x003c		//输出治疗百分比
#define DODAMAGEPERCENT		0x003d		//输出物理伤害百分比
#define DOMAGICPERCENT		0x003e		//输出异能伤害百分比
#define ONCUREPERCENT		0x003f		//被输出治疗百分比
#define ONDAMAGEPERCENT		0x0040		//被输出物理伤害百分比
#define ONMAGICPERCENT		0x0041		//被输出异能伤害百分比

#define SAFEFLAG	0x80
#define ITEMFLAG	0x40
#define STARLEVEL	0x7F

typedef struct _ST_ITEMINFO{
	int iItemId;
	string strItemName;
	string strItemType;
}st_ItemInfo,*pSt_ItemInfo;

typedef vector<st_ItemInfo> Vec_ItemInfo;

typedef struct _ST_SENDITEMINFO{
	int iItemID;
	int iItemNum;
// 	int iPropertyId;
// 	int iPropertyValue;
// 	int iIncreaseNum;
// 	int iIncreaseType;
// 	int iIncreaseValue;
// 	int iIncreaseEndValue;
// 	int iStarsLevel;
// 	int iAssemblyID;
// 	int iSlotNum;
// 	int iEquipSlotID1;
// 	int iEquipSlotID2;
// 	int iEquipSlotID3;
// 	int iQualityEquip;
// 	int iIsBind;
//	string strItemname;
	string strAttrValue;
}St_SendItemInfo, *pSt_SendItemInfo;

typedef vector<St_SendItemInfo> Vec_SendItemInfo;

typedef struct _ST_GUILDITEMINFO{
	int iItemId;
	int iItemNum;
}st_GuildItemInfo,*pSt_GuildItemInfo;

typedef vector<st_GuildItemInfo> Vec_GuildItemInfo;
//End

class  CXDJGInfo : public CGameInfo
{
public:
	CXDJGInfo(void);
	~CXDJGInfo(void);
 public:
	COperVector * initialize(CSession * pSess,CEnumCore::Message_Tag_ID MessageTagID, unsigned char * pbuf, int length);
	
	//
	vector <CGlobalStruct::TFLV> XDJG_Net_UserInfo(char *Account, char * Character, char *ServerIP, int type, int iIndex, int iPageSize);
	bool getUserInfoDataset(vector <CGlobalStruct::TFLV> &DataSetTFLV, vector<string> vectorCol, char *Character, char *ServerIP, int iIndex, int iPageSize, int iCol);
	vector <CGlobalStruct::TFLV> XDJG_Net_KickPlayer(int userByID, char *ServerIP, char * Character, char * Account);
	vector <CGlobalStruct::TFLV> XDJG_Net_ClosePlayer_List(char *ServerIP, int iIndex, int iPageSize);
	//Modified by tengjie 2010-05-17
//	vector <CGlobalStruct::TFLV> XDJG_Net_ClosePlayer(int userByID, char *ServerIP, char *Account, char *AccountID, int iTime,char *Reason);
	vector <CGlobalStruct::TFLV> XDJG_Net_ClosePlayer(int userByID, char *ServerIP, char *Account, char *AccountID, int iTime,char *Reason,int iReasonType);
	CString XDJG_CalEndFreezeTime(int iTime);	
//	vector <CGlobalStruct::TFLV> XDJG_Net_OpenPlayer(int userByID, char *ServerIP, char *Account, char *Reason);
	vector <CGlobalStruct::TFLV> XDJG_Net_OpenPlayer(int userByID, char *ServerIP, char *Account, char *Reason,int iReasonType);

	vector <CGlobalStruct::TFLV> XDJG_Net_BoardTasker_Query(char * GameName,char * m_sql,int iIndex,int iPageSize);
	vector <CGlobalStruct::TFLV> XDJG_Net_BoardTasker_Add(int userByID, char * ServerIP, char *BoardMessage, char *BeginTime, char *EndTime, int interval);
	vector <CGlobalStruct::TFLV> XDJG_Net_BoardTasker_Update(int userByID, char * ServerIP, int Taskid, char *BoardMessage, char *BeginTime, char *EndTime, int interval);
	vector <CGlobalStruct::TFLV> XDJG_Net_BoardTasker_Delete(int userByID, int Taskid);

	//Added by tengjie 2010-07-06
	//角色技能添加
	vector <CGlobalStruct::TFLV> XDJG_Net_RoleSkillAdd(int userByID, char *ServerIP, char *Character, int SkillID);
	 //角色技能删除
	vector <CGlobalStruct::TFLV> XDJG_Net_RoleSkillDel(int userByID, char *ServerIP, char *Character, int SkillID);
	//道具添加
//	vector <CGlobalStruct::TFLV> XDJG_Net_ItemAdd(int userByID, char *ServerIP, char *Character, char *ItemID, char *ItemNum);
	vector <CGlobalStruct::TFLV> XDJG_Net_ItemAdd(int userByID, char *ServerIP, char *Character, char *SendCharacter, char *mailTitle, char *mailContent, int Money, char *ItemID, char *ItemNum, char *PropertyId, char *PropertyValue, char *IncreaseNum, 
												  char *IncreaseType, char *IncreaseValue, char *IncreaseEndValue, char *StarsLevel, char *AssemblyID, char *SlotNum, char *EquipSlotID1, char *EquipSlotID2, char *EquipSlotID3, char *QualityEquip, char *IsBind/* ,char *ItemName*/);
	//道具删除
	vector <CGlobalStruct::TFLV> XDJG_Net_ItemDel(int userByID, char *ServerIP, char *Character, int ItemID, int ItemNum);
	//游戏币查询
	vector <CGlobalStruct::TFLV> XDJG_Net_MoneyQuery(char *ServerIP, char *Account);
	//添加游戏币
	vector <CGlobalStruct::TFLV> XDJG_Net_MoneyAdd(int userByID, char *ServerIP, char *Account, int iMoney);
	//日志类:玩家登录日志
	vector <CGlobalStruct::TFLV> XDJG_DB_Login_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, char *QueryIP, int iIndex, int iPageSize);
	//商城物品购买日志
	vector <CGlobalStruct::TFLV> XDJG_DB_Shoping_Log(int userByID, char *ServerIP, char *Account, char *Character, int QueryType, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//玩家物品日志:交易获得
	vector <CGlobalStruct::TFLV> XDJG_DB_TransGain_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//玩家物品日志:交易失去
	vector <CGlobalStruct::TFLV> XDJG_DB_TransLose_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//玩家物品日志:从NPC出购买
	vector <CGlobalStruct::TFLV> XDJG_DB_BuyFromNpc_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//玩家物品日志:卖给NPC
	vector <CGlobalStruct::TFLV> XDJG_DB_SaleToNpc_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//玩家物品日志:拾取物品
	vector <CGlobalStruct::TFLV> XDJG_DB_PickUpItem_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//玩家物品日志:丢弃物品
	vector <CGlobalStruct::TFLV> XDJG_DB_DiscardItem_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//玩家物品日志:物品消耗
	vector <CGlobalStruct::TFLV> XDJG_DB_ConsumeItem_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//玩家物品日志:拍卖行卖出
	vector <CGlobalStruct::TFLV> XDJG_DB_AuctioneerSaled_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//玩家物品日志:拍卖行购入
	vector <CGlobalStruct::TFLV> XDJG_DB_AuctioneerBought_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//玩家邮件日志:发送邮件
	vector <CGlobalStruct::TFLV> XDJG_DB_SendMail_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//玩家邮件日志:收取邮件
	vector <CGlobalStruct::TFLV> XDJG_DB_ReceiveMail_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//M币使用日志:背包扩充续费记录
	vector <CGlobalStruct::TFLV> XDJG_DB_ExpansionPack_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//M币使用日志:传送日志
	vector <CGlobalStruct::TFLV> XDJG_DB_Deliver_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//M币使用日志:角色仓库扩充续费记录
	vector <CGlobalStruct::TFLV> XDJG_DB_ExpansionWarehouse_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//玩家角色日志:角色升级
	vector <CGlobalStruct::TFLV> XDJG_DB_RoleUpgrade_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//玩家角色日志:角色创建
	vector <CGlobalStruct::TFLV> XDJG_DB_RoleCreate_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//玩家角色日志:角色技能学习
	vector <CGlobalStruct::TFLV> XDJG_DB_RoleSkill_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//玩家角色日志:领取任务
	vector <CGlobalStruct::TFLV> XDJG_DB_ReceiveTask_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//玩家角色日志:完成任务
	vector <CGlobalStruct::TFLV> XDJG_DB_CompleteTask_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	//玩家角色日志:副本查询
	vector <CGlobalStruct::TFLV> XDJG_DB_Copy_Log(int userByID, char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	
	void GetItemInfo(Vec_ItemInfo &VecItemInfo);

	//查询类
	//玩家物品信息:身上物品信息
	vector <CGlobalStruct::TFLV> XDJG_DB_ItemBody_Query(int userByID, char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize);
	//玩家物品信息:背包物品信息
	vector <CGlobalStruct::TFLV> XDJG_DB_Itembackpack_Query(int userByID, char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize);
	 
	//玩家物品信息:仓库物品信息
	vector <CGlobalStruct::TFLV> XDJG_DB_ItemStorage_Query(int userByID, char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize);
	 
	//玩家物品信息:公会仓库信息
	vector <CGlobalStruct::TFLV> XDJG_DB_GuildStorage_Query(int userByID, char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize);
	 
	//玩家物品信息:金刚仓库信息
	vector <CGlobalStruct::TFLV> XDJG_DB_KingkongStorage_Query(int userByID, char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize);
	
	//邮件信息
	vector <CGlobalStruct::TFLV> XDJG_DB_MailInfo_Query(int userByID, char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize);
	 
	//拍卖行信息
	vector <CGlobalStruct::TFLV> XDJG_DB_AuctioneerInfo_Query(int userByID, char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize);
	
	//用户角色信息的修改:修改等级
	vector <CGlobalStruct::TFLV> XDJG_Net_ModifyLevel(int userByID, char *ServerIP, char *Character, int UpdateType, int Level, int Exp);
	
	//用户密码管理:修改密码
	vector <CGlobalStruct::TFLV> XDJG_Net_ModifyPwd(int userByID, char *ServerIP, char *Account, char* NewPwd);
	
	//用户密码管理:临时密码恢复
	vector <CGlobalStruct::TFLV> XDJG_Net_ResumePwd(int userByID, char *ServerIP, char *Account);
	 
	//用户角色管理:角色恢复
	vector <CGlobalStruct::TFLV> XDJG_Net_ResumeRole(int userByID, char *ServerIP, char *Character);
	 
	//用户角色管理:角色删除
	vector <CGlobalStruct::TFLV> XDJG_Net_DeleteRole(int userByID, char *ServerIP, char *Character);
	 
	//修改角色身上金钱数量(绑定币)
	vector <CGlobalStruct::TFLV> XDJG_Net_UpRoleBind(int userByID, char *ServerIP, char *Character, int Money);
	
	//修改角色身上金钱数量(非绑定币)
	vector <CGlobalStruct::TFLV> XDJG_Net_UpRoleNonBind(int userByID, char *ServerIP, char *Character, int Money);

	//添加角色身上金钱数量(绑定币)
	vector <CGlobalStruct::TFLV> XDJG_Net_AddRoleBind(int userByID, char *ServerIP, char *Character, int Money);
	
	//添加角色身上金钱数量(非绑定币)
	vector <CGlobalStruct::TFLV> XDJG_Net_AddRoleNonBind(int userByID, char *ServerIP, char *Character, int Money);

	vector <CGlobalStruct::TFLV> CombinationTflv(St_ResCont &sResCont, vector <string> &VecItem);

	//获取sql
	string GetSqlCommend(char *sqlType);
	
	XDJG_StructInfo m_StructInfo;

	//角色技能信息查询
	vector <CGlobalStruct::TFLV> XDJG_RoleSkills_Query(char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize);

	//技能列表查询
	vector <CGlobalStruct::TFLV> XDJG_SkillList_Query(char *GameName, char *Sql, char *Profession);

	//道具类型查询
	vector <CGlobalStruct::TFLV> XDJG_ItemType_Query(char *GameName, char *Sql);
	
	//相应类型的道具列表查询
	vector <CGlobalStruct::TFLV> XDJG_ItemNameLst_Query(char *GameName, char *Sql, char *ItemType, char *ItemName, int index, int pagesize);

	//物品属性查询
	vector <CGlobalStruct::TFLV> XDJG_ItemAttribute_Query(char *ServerIP, char *Character, char *ItemId);
	
	//获取LogDB的名称
	vector <string> GetLogDbName(char *BeginTime, char *EndTime);

	vector <CGlobalStruct::TFLV> QueryPlayerInfo(char *ServerIP, string strCharacter, int type, int iIndex, int iPageSize);

	//获取技能名称
	string CXDJGInfo::GetSkillName(int SkillId, int iGetType);

	//获取生活技能名称
	string CXDJGInfo::GetLifeSkillName(char *Prototype);

	//获取角色称号列表
	vector <CGlobalStruct::TFLV> GetRoleTitles(char *ServerIP, char *Character);

	//取生活技能列表
	vector <CGlobalStruct::TFLV> GetLifeSkills(char *ServerIP, char *Character);

	//分析道具属性信息
	void AnalysisItemInfo(char *ItemID, char *ItemNum, char *PropertyId, char *PropertyValue, char *IncreaseNum, char *IncreaseType, char *IncreaseValue, char *IncreaseEndValue, char *StarsLevel, char *AssemblyID, char *SlotNum, char *EquipSlotID1, char *EquipSlotID2, char *EquipSlotID3, char *QualityEquip, char *IsBind/*, char *ItemName*/);
	
	//添加道具
//	BOOL SendSingleItem(char *ServerIP, char *Character, vector<St_SendItemInfo>::iterator pSendItemInfo);
	BOOL SendSingleItem(char *ServerIP, char *Character, char *SendCharacter, char *mailTitle, char *mailContent, int Money, vector<St_SendItemInfo>::iterator pSendItemInfo);

	//设置道具属性
	BOOL SetItemAttribute(char *ServerIP, char *Character, vector<St_SendItemInfo>::iterator pSendItemInfo);

	//公会信息查询
	//公会基本信息
	vector <CGlobalStruct::TFLV> XDJG_GuildBaseInfo_Query(char *ServerIP, char *GuildName);
		
	//公会成员列表
	vector <CGlobalStruct::TFLV> XDJG_GuildMemberList_Query(char *ServerIP, char *GuildName);
	
	//玩家任务查询
	vector <CGlobalStruct::TFLV> XDJG_PlayerTaskInfo_Query(char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize); 

	//金刚车辆信息
	//金刚车辆基本信息
	vector <CGlobalStruct::TFLV> XDJG_CarBaseInfo_Query(char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize); 
	
	//金刚车辆详细信息
	vector <CGlobalStruct::TFLV> XDJG_CarDetailInfo_Query(char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize); 
	
	//玩家公会信息
	//玩家公会基本信息
	vector <CGlobalStruct::TFLV> XDJG_PlayerGuildBaseInfo_Query(char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize); 

	//玩家公会物品信息
	vector <CGlobalStruct::TFLV> XDJG_PlayerGuildItemInfo_Query(char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize); 

	//玩家物品日志
	//装备增幅日志
	vector <CGlobalStruct::TFLV> XDJG_EquipIncrease_Log_Query(char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);

	//插片合成日志
	vector <CGlobalStruct::TFLV> XDJG_Synthesis_Log_Query(char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);

	//装备传导日志
	vector <CGlobalStruct::TFLV> XDJG_EquipTransfer_Log_Query(char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
				
	//物品鉴定
	vector <CGlobalStruct::TFLV> XDJG_ItemIdentify_Log_Query(char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	
	//个人仓库存放日志
	vector <CGlobalStruct::TFLV> XDJG_RoleStore_Log_Query(char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	
	//公会仓库存放日志
	vector <CGlobalStruct::TFLV> XDJG_GuildStore_Log_Query(char *ServerIP, char *Account, char *Character, char *BeginTime, char *EndTime, int iIndex, int iPageSize);

	//公会日志
	//公会成立
	vector <CGlobalStruct::TFLV> XDJG_GuildCreate_Log_Query(char *ServerIP, char *Guildname, char *BeginTime, char *EndTime, int iIndex, int iPageSize);

	//公会解散
	vector <CGlobalStruct::TFLV> XDJG_GuildDismiss_Log_Query(char *ServerIP, char *Guildname, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	
	//公会添加成员		
	vector <CGlobalStruct::TFLV> XDJG_GuildAddMember_Log_Query(char *ServerIP, char *Guildname, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	
	//公会成员离开
	vector <CGlobalStruct::TFLV> XDJG_GuildMemberLeaved_Log_Query(char *ServerIP, char *Guildname, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	
	//公会职务变更
	vector <CGlobalStruct::TFLV> XDJG_GuildPostChanged_Log_Query(char *ServerIP, char *Guildname, char *BeginTime, char *EndTime, int iIndex, int iPageSize);
	
	//根据角色取帐号
	CGlobalStruct::TFLV GetAccount(char *ServerIP, char *RoleName);

	//获取真正的时间
	string GetRealTime(int time);

	//帐号是否封停
	vector <CGlobalStruct::TFLV> XDJG_AccountIsClose_Query(char *ServerIP, char *Account);

	//取金刚名称
	string GetCarName(int carId);
	
	//取任务名称
	string GetTaskName(int TaskId);

	//取已删除的角色信息
	vector <CGlobalStruct::TFLV> GetDeletedRolesInfo(char *ServerIP, char *Account, int iIndex, int iPageSize);

	//NPC道具领取查询
	vector <CGlobalStruct::TFLV> XDJG_NPCGetItem_Query(char *ServerIP, char *Account, int AccountID, char *Character,int iIndex, int iPageSize);
	
	//获取领奖包信息
	vector <CGlobalStruct::TFLV> GetAwardPackageInfo(int iPackageId);

	//设置属性
	BOOL SetItemAttr(char *ServerIP,char *Character,int iType,int iParam1,int iParam2);

	//转换成16进制
	string ConvertTo16(int iValue);

	//添加称号
	vector <CGlobalStruct::TFLV> XDJG_AddTitle_Query(int userByID, char *ServerIP,char *Account,char *Character,int iTitleID, char *TitleName);
			
	//移除称号
	vector <CGlobalStruct::TFLV> XDJG_RemoveTitle_Query(int userByID, char *ServerIP,char *Account,char *Character,int iTitleID, char *TitleName);
			
	//添加BUFF
	vector <CGlobalStruct::TFLV> XDJG_AddBuff_Query(int userByID, char *ServerIP,char *Account,char *Character,int iBuffID, char *BuffName);
			
	//移除BUFF
	vector <CGlobalStruct::TFLV> XDJG_RemoveBuff_Query(int userByID, char *ServerIP,char *Account,char *Character,int iBuffID, char *BuffName);

	//移动到指定位置
	vector <CGlobalStruct::TFLV> XDJG_MoveToPosition_Query(int userByID, char *ServerIP,char *Account,char *Character, int iPX, int iPY, int iPZ);
			
	//移动到指定玩家身边
	vector <CGlobalStruct::TFLV> XDJG_MoveToRole_Query(int userByID, char *ServerIP,char *Account,char *Character,char *TargetCharacter);

	//移动到目标玩家身边
	vector <CGlobalStruct::TFLV> XDJG_MoveToTargetRole_Query(int userByID, char *ServerIP,char *Account,char *Character,char *TargetCharacter);
	
	//称号列表查询
	vector <CGlobalStruct::TFLV> XDJG_TitleList_Query(char *Gamename, char *Sql);
			
	//BUFF列表查询
	vector <CGlobalStruct::TFLV> XDJG_BuffList_Query(char *Gamename, char *Sql);
	//End
	
	//	vector <CGlobalStruct::TFLV> XDJG_Net_BoardTasker_Oper(int userByID, int op_type,char *content, char *begintime, int interval, int repeat, char * begindate, char *enddate, int notice_index/*=0*/); 
	//	vector <CGlobalStruct::TFLV> XDJG_Net_BoardTasker_Query(char * GameName,char * m_sql,int iIndex,int iPageSize);
	//	bool getBoardTaskerDetail(vector<string> &vectorDetail, int notice_index);
private:
	//记录日志信息
	char strLog[2048];

	//本地数据库IP地址
	char LocalIP[30];

	//帐号数据库
	char AccountDB[30];

	//日志数据库
	char LogDB[30];

	//游戏数据库
	char GameDB[30];

	//邮件数据库
	char MailDB[30];

	//Added by tengjie 2010-05-17
	struct  _ST_XDJG
	{
		enum __ENUM
		{
			NONE_BANACOOUNT = 0,
			MAX_LOGINTIMES	= 1,
			BANED_BYGM		= 2,
			BANED_BYPLUG	= 5,
			WEB_FREEZE		= 10,
			WEB_LOCK		= 11,
			NONE_REASON		= 15
		}_value;
	};

	Vec_SendItemInfo m_VecSendItemInfo;

	CSqlHelper m_SqlHelper;
	//服务器端口号
//	int port;
	
	COperatorXDJG m_operXDJG;

	Vec_GuildItemInfo m_VecGuildItemInfo;
};