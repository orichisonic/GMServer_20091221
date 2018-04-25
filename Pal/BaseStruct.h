#pragma once
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


// NetSlot Client
#include "./NetSlot/NetSlot_Lib.h"
#pragma comment(lib, "./NetSlot/NetSlot_Release.lib")

#include "./PalDataReader/PalDataReader.h"
#pragma comment(lib, "./PalDataReader/PalDataReader.lib")

// 封包编号-------------------------------------------------------------------------------------------
enum ENUM_GMPacket
{	
	ENUM_PGGMCConnection_CtoS_RequestLogin		= 0xe001,	// GMClient -> GMServer 要求登入
	ENUM_PGGMCConnection_StoC_LoginResult				,	// GMServer -> GMClient 回传登入结果

	ENUM_PGServerControl_CtoS_SendNonscheduleBulletin = 0xe300,	// GMC -> GMS 要求发送非固定公告
	ENUM_PGServerControl_StoC_SendNonscheduleBulletin = 0xe302,	// GMS -> GMC 回传发送非固定公告结果

	ENUM_PGServerControl_CtoS_AddScheduleBulletin	= 0xe310,	// GMC -> GMS 要求新增排程公告
	ENUM_PGServerControl_StoC_AddScheduleBulletin	= 0xe313,	// GMS -> GMC 回传新增排程公告结果

	ENUM_PGServerControl_CtoS_DelScheduleBulletin	= 0xe320,	// GMC -> GMS 要求删除排程公告
	ENUM_PGServerControl_StoC_DelScheduleBulletin	= 0xe323,	// GMS -> GMC 回传删除排程公告结果

	ENUM_PGServerControl_CtoS_ReloadScheduleBulletin= 0xe330,	// GMC -> GMS 要求重新载入排程公告
	ENUM_PGServerControl_StoC_ReloadScheduleBulletin= 0xe333,	// GMS -> GMC 回传重新载入排程公告结果

	ENUM_PGServerControl_CtoS_SendGiftMail			= 0xe340,	// GMC -> GMS 要求寄送奖品邮件
	ENUM_PGServerControl_StoC_SendGiftMail			= 0xe343,	// GMS -> GMC 回传寄送奖品邮件结果

	ENUM_PGServerControl_CtoS_ReloadFunctionSwitch	= 0xe350,	// GMC -> GMS 要求重新载入功能开关列表
	ENUM_PGServerControl_StoC_ReloadFunctionSwitch	= 0xe353,	// GMS -> GMC 回传重新载入功能开关列表结果

	// add by ltx
	ENUM_PGServerControl_CtoS_SendCustomItemMail	= 0xe360,	// GMC -> GMS 要求寄送客制物品邮件
	ENUM_PGServerControl_StoC_SendCustomItemMail	= 0xe363,	// GMS -> GMC 回传寄送客制物品邮件结果

	ENUM_PGServerStatus_CtoS_PlayerOnlineCount	= 0xe500,	// GMC->GMS 要求上线人数
	ENUM_PGServerStatus_StoC_PlayerOnlineCount	= 0xe503,	// GMS->GMC 回传上线人数结果

	ENUM_PGServerStatus_CtoS_ScheduleBulletinList	= 0xe510,	// GMC -> GMS 要求排程公告列表
	ENUM_PGServerStatus_StoC_ScheduleBulletinList	= 0xe513,	// GMS -> GMC 回传排程公告列表

	ENUM_PGPlayerControl_CtoS_BanishPlayer		= 0xe600,	// GMC->GMS 要求踢某玩家下线
	ENUM_PGPlayerControl_StoC_BanishPlayer		= 0xe603,	// GMS->GMC 回传踢某玩家下线结果

	ENUM_PGPlayerControl_CtoS_ModifyGuildMemberPL	= 0xe610,	// GMC -> GMS 要求修改公会成员职阶
	ENUM_PGPlayerControl_StoC_ModifyGuildMemberPL	= 0xe613,	// GMS -> GMC 回传修改公会成员职阶结果

	ENUM_PGPlayerControl_CtoS_DismissGuild			= 0xe620,	// GMC -> GMS 要求解散公会
	ENUM_PGPlayerControl_StoC_DismissGuild			= 0xe623,	// GMS -> GMC 回传解散公会结果
	
	ENUM_PGPlayerControl_CtoS_ModifyGuildName		= 0xe630,	// GMC -> GMS 要求修改公会名称
	ENUM_PGPlayerControl_StoC_ModifyGuildName		= 0xe633,	// GMS -> GMC 回传修改公会名称结果

	ENUM_PGPlayerControl_CtoS_ModifyCharacterQuest	= 0xe640,	// GMC -> GMS 要求修改角色任务
	ENUM_PGPlayerControl_StoC_ModifyCharacterQuest	= 0xe643,	// GMS -> GMC 回传修改角色任务结果

	ENUM_PGPlayerControl_CtoS_DelCharacterItem		= 0xe650,	// GMC -> GMS 要求删除角色物品
	ENUM_PGPlayerControl_StoC_DelCharacterItem		= 0xe653,	// GMS -> GMC 回传删除角色物品结果

	ENUM_PGPlayerControl_CtoS_ModifyCharacterMoney	= 0xe660,	// GMC -> GMS 要求修改角色金钱
	ENUM_PGPlayerControl_StoC_ModifyCharacterMoney	= 0xe663,	// GMS -> GMC 回传修改角色金钱结果

	ENUM_PGPlayerControl_CtoS_ModifyGuildLv			= 0xe670,	// GMC -> GMS 要求修改公会等级
	ENUM_PGPlayerControl_StoC_ModifyGuildLv			= 0xe673,	// GMS -> GMC 回传修改公会等级结果

	ENUM_PGPlayerControl_CtoS_ModifyGuildNotice		= 0xe680,	// GMC -> GMS 要求修改公会公告
	ENUM_PGPlayerControl_StoC_ModifyGuildNotice		= 0xe683,	// GMS -> GMC 回传修改公会公告结果

	ENUM_PGPlayerControl_CtoS_ModifyCharacterDropPw	= 0xe690,	// GMC -> GMS 要求修改角色删除密码
	ENUM_PGPlayerControl_StoC_ModifyCharacterDropPw	= 0xe693,	// GMS -> GMC 回传修改角色删除密码结果

	ENUM_PGPlayerControl_CtoS_ModifyAccountBankPw	= 0xe700,	// GMC -> GMS 要求修改角色银行密码
	ENUM_PGPlayerControl_StoC_ModifyAccountBankPw	= 0xe703,	// GMS -> GMC 回传修改角色银行密码结果

	ENUM_PGPlayerControl_CtoS_DelCharacterMob		= 0xe710,	// GMC -> GMS 要求删除角色伏魔
	ENUM_PGPlayerControl_StoC_DelCharacterMob		= 0xe713,	// GMS -> GMC 回传删除角色伏魔结果

	ENUM_PGPlayerControl_CtoS_ModifyPuzzleMap		= 0xe720,	// GMC -> GMS 要求修改角色图鉴
	ENUM_PGPlayerControl_StoC_ModifyPuzzleMap		= 0xe723,	// GMS -> GMC 回传修改角色图鉴结果

	ENUM_PGPlayerControl_CtoS_MoveCharacterToSafe	= 0xe730,	// GMC->GMS 要求移动角色至安全点
	ENUM_PGPlayerControl_StoC_MoveCharacterToSafe	= 0xe733,	// GMS->GMC 回传移动角色至安全点结果

	ENUM_PGPlayerStatus_CtoS_CharacterList		= 0xe800,	// GMC->GMS 要求角色列表
	ENUM_PGPlayerStatus_StoC_CharacterList		= 0xe803,	// GMS->GMC 回传角色列表结果

	ENUM_PGPlayerStatus_CtoS_CharacterInfo		= 0xe810,	// GMC -> GMS 要求角色基本资讯
	ENUM_PGPlayerStatus_StoC_CharacterInfo		= 0xe813,	// GMS -> GMC 回传角色基本资讯结果

	ENUM_PGPlayerStatus_CtoS_CharacterBag		= 0xe820,	// GMClient -> GMServer 要求角色背包资讯
	ENUM_PGPlayerStatus_StoC_CharacterBag		= 0xe823,	// GMServer -> GMClient 回传角色背包资讯结果

	ENUM_PGPlayerStatus_CtoS_CharacterQuest		= 0xe830,	// GMC -> GMS 要求角色任务资讯
	ENUM_PGPlayerStatus_StoC_CharacterQuest		= 0xe833,	// GMS -> GMC 回传角色任务资讯结果

	ENUM_PGPlayerStatus_CtoS_CharacterFriendList	= 0xe840,	// GMC -> GMS 要求角色好友列表
	ENUM_PGPlayerStatus_StoC_CharacterFriendList	= 0xe843,	// GMS -> GMC 回传角色好友列表结果

	ENUM_PGPlayerStatus_CtoS_GuildBaseInfo			= 0xe850,	// GMC -> GMS 要求公会基本资讯
	ENUM_PGPlayerStatus_StoC_GuildBaseInfo			= 0xe853,	// GMS -> GMC 回传公会基本资讯结果

	ENUM_PGPlayerStatus_CtoS_GuildMemberList		= 0xe860,	// GMC -> GMS 要求公会成员列表
	ENUM_PGPlayerStatus_StoC_GuildMemberList		= 0xe863,	// GMS -> GMC 回传公会成员列表结果

	ENUM_PGPlayerStatus_CtoS_CharacterSkill	= 0xe870,	// GMC -> GMS 要求角色技能资讯
	ENUM_PGPlayerStatus_StoC_CharacterSkill	= 0xe873,	// GMS -> GMC 回传角色技能资讯结果

	ENUM_PGPlayerStatus_CtoS_CharacterMob	= 0xe880,	// GMC -> GMS 要求角色伏魔资讯
	ENUM_PGPlayerStatus_StoC_CharacterMob	= 0xe883,	// GMS -> GMC 回传角色伏魔资讯结果

	ENUM_PGPlayerStatus_CtoS_CharacterPuzzleMap	= 0xe890,	// GMC -> GMS 要求角色图鉴资讯
	ENUM_PGPlayerStatus_StoC_CharacterPuzzleMap	= 0xe893,	// GMS -> GMC 回传角色图鉴资讯结果

	ENUM_PGPlayerStatus_CtoS_CharacterAdvanceInfo	= 0xe8a0,	// GMC -> GMS 要求角色进阶资讯
	ENUM_PGPlayerStatus_StoC_CharacterAdvanceInfo	= 0xe8a3,	// GMS -> GMC

	ENUM_PGPlayerStatus_CtoS_CharacterBlackList	= 0xe8b0,	// GMC -> GMS 要求角色黑名单列表
	ENUM_PGPlayerStatus_StoC_CharacterBlackList	= 0xe8b3,	// GMS -> GMC 回传角色黑名单列表结果

	ENUM_PGPlayerStatus_CtoS_AccountInfo		= 0xe8c0,	// GMC -> GMS 要求角色基本资讯
	ENUM_PGPlayerStatus_StoC_AccountInfo		= 0xe8c3,	// GMS -> GMC 回传角色基本资讯结果
	
};

// 登入结果-------------------------------------------------------------------------------------------
enum ENUM_GMCLoginResult
{
	ENUM_GMCLoginResult_Success = 0,	// 成功
	ENUM_GMCLoginResult_Failed,			// 失败
	ENUM_GMCLoginResult_AccountFailed,	// 帐号错误
	ENUM_GMCLoginResult_PasswordFailed, // 密码错误
	ENUM_GMCLoginResult_IPFailed,		// IP错误		
};
// 发送非固定公告结果列举-----------------------------------------------------------------------------
enum ENUM_SendNonscheduleBulletinResult
{
	ENUM_SendNonscheduleBulletinResult_NULL = 0, 
	ENUM_SendNonscheduleBulletinResult_Success,				// 成功.
	ENUM_SendNonscheduleBulletinResult_ArgumentError,		// 参数错误
	ENUM_SendNonscheduleBulletinResult_WorldNotFound,		// 找不到世界
	ENUM_SendNonscheduleBulletinResult_LeaderNotFound,		// 找不到领头观测者
	ENUM_SendNonscheduleBulletinResult_LeaderDisconnect,	// 领头观测者未连线
};
// 新增排程公告结果列举-------------------------------------------------------------------------------
enum ENUM_AddScheduleBulletinResult
{
	ENUM_AddScheduleBulletinResult_NULL = 0, 
	ENUM_AddScheduleBulletinResult_Success,				// 成功.
	ENUM_AddScheduleBulletinResult_ArgumentError,		// 参数错误
	ENUM_AddScheduleBulletinResult_WorldNotFound,		// 找不到世界
	ENUM_AddScheduleBulletinResult_LeaderNotFound,		// 找不到领头观测者
	ENUM_AddScheduleBulletinResult_LeaderDisconnect,	// 领头观测者未连线
	ENUM_AddScheduleBulletinResult_AddFailed,			// 新增失败,公告ID已存在
};
// 删除排程公告结果列举-------------------------------------------------------------------------------
enum ENUM_DelScheduleBulletinResult
{
	ENUM_DelScheduleBulletinResult_NULL = 0, 
	ENUM_DelScheduleBulletinResult_Success,				// 成功.
	ENUM_DelScheduleBulletinResult_WorldNotFound,		// 找不到世界
	ENUM_DelScheduleBulletinResult_LeaderNotFound,		// 找不到领头观测者
	ENUM_DelScheduleBulletinResult_LeaderDisconnect,	// 领头观测者未连线
	ENUM_DelScheduleBulletinResult_DelFailed,			// 删除失败
};



// 重新载入排程公告结果列举---------------------------------------------------------------------------
enum ENUM_ReloadScheduleBulletinResult
{
	ENUM_ReloadScheduleBulletinResult_NULL = 0, 
	ENUM_ReloadScheduleBulletinResult_Success,				// 成功.
	ENUM_ReloadScheduleBulletinResult_WorldNotFound,		// 找不到世界
	ENUM_ReloadScheduleBulletinResult_LeaderNotFound,		// 找不到领头观测者
	ENUM_ReloadScheduleBulletinResult_LeaderDisconnect,		// 领头观测者未连线
	ENUM_ReloadScheduleBulletinResult_LocalDisconnect,		// 区域伺服器未连线
};
// 寄送奖品邮件结果列举-------------------------------------------------------------------------------
enum ENUM_SendGiftMailResult
{
	ENUM_SendGiftMailResult_NULL = 0, 
	ENUM_SendGiftMailResult_Success,				// 成功.
	ENUM_SendGiftMailResult_ArgumentError,			// 参数错误
	ENUM_SendGiftMailResult_WorldNotFound,			// 找不到世界
	ENUM_SendGiftMailResult_LeaderNotFound,			// 找不到领头观测者
	ENUM_SendGiftMailResult_LeaderDisconnect,		// 领头观测者未连线
	ENUM_SendGiftMailResult_RoleNotFound,			// 角色不存在
	ENUM_SendGiftMailResult_LocalDisconnect,		// 区域伺服器未连线
	ENUM_SendGiftMailResult_LocalOperationFailed,	// 操作失败(Local)
};
// 取得上线人数结果列举-------------------------------------------------------------------------------
enum ENUM_PlayerOnlineCountResult
{
	ENUM_PlayerOnlineCountResult_NULL = 0, 
	ENUM_PlayerOnlineCountResult_Success,			// 成功.
	ENUM_PlayerOnlineCountResult_WorldNotFound,		// 找不到世界
	ENUM_PlayerOnlineCountResult_LeaderNotFound,	// 找不到领头观测者
	ENUM_PlayerOnlineCountResult_LeaderDisconnect,	// 领头观测者未连线
};
// 取得排程公告列表结果列举---------------------------------------------------------------------------
enum ENUM_ScheduleBulletinListResult
{
	ENUM_ScheduleBulletinListResult_NULL = 0, 
	ENUM_ScheduleBulletinListResult_Success,			// 成功.
	ENUM_ScheduleBulletinListResult_WorldNotFound,		// 找不到世界
	ENUM_ScheduleBulletinListResult_LeaderNotFound,		// 找不到领头观测者
	ENUM_ScheduleBulletinListResult_LeaderDisconnect,	// 领头观测者未连线
	ENUM_ScheduleBulletinListResult_BulletinNotFound,	// 列表是空的
};
// 踢某玩家下线结果列举-------------------------------------------------------------------------------
enum ENUM_BanishPlayerResult
{
	ENUM_BanishPlayerResult_NULL = 0, 
	ENUM_BanishPlayerResult_Success,			// 成功.
	ENUM_BanishPlayerResult_RoleNotFound,		// 角色不存在
	ENUM_BanishPlayerResult_RoleNotOnline,		// 角色不在线上
	ENUM_BanishPlayerResult_WorldNotFound,		// 找不到世界
	ENUM_BanishPlayerResult_LeaderNotFound,		// 找不到领头观测者
	ENUM_BanishPlayerResult_LeaderDisconnect,	// 领头观测者未连线
};
// 修改公会成员职阶结果列举---------------------------------------------------------------------------
enum ENUM_ModifyGuildMemberPLResult
{
	ENUM_ModifyGuildMemberPLResult_NULL = 0, 
	ENUM_ModifyGuildMemberPLResult_Success,				// 成功.
	ENUM_ModifyGuildMemberPLResult_WorldNotFound,		// 找不到世界
	ENUM_ModifyGuildMemberPLResult_LeaderNotFound,		// 找不到领头观测者
	ENUM_ModifyGuildMemberPLResult_LeaderDisconnect,	// 领头观测者未连线
	ENUM_ModifyGuildMemberPLResult_LevelError,			// 职阶错误
	ENUM_ModifyGuildMemberPLResult_GuildNotFound,		// 公会不存在
	ENUM_ModifyGuildMemberPLResult_RoleNotFound,		// 公会无此角色
	ENUM_ModifyGuildMemberPLResult_LevelIsFull,			// 职阶人数已满
	ENUM_ModifyGuildMemberPLResult_GMOOperationFailed,	// 操作失败(GMO)
	ENUM_ModifyGuildMemberPLResult_ChatOperationFailed,	// 操作失败(Chat)
};
// 解散公会结果列举-----------------------------------------------------------------------------------
enum ENUM_DismissGuildResult
{
	ENUM_DismissGuildResult_NULL = 0, 
	ENUM_DismissGuildResult_Success,			// 成功.
	ENUM_DismissGuildResult_WorldNotFound,		// 找不到世界
	ENUM_DismissGuildResult_LeaderNotFound,		// 找不到领头观测者
	ENUM_DismissGuildResult_LeaderDisconnect,	// 领头观测者未连线
	ENUM_DismissGuildResult_GuildNotFound,		// 公会不存在
	ENUM_DismissGuildResult_GMOOperationFailed,	// 操作失败(GMO)
	ENUM_DismissGuildResult_SystemNotReady,		// 系统准备中(Chat)
	ENUM_DismissGuildResult_ChatOperationFailed,// 操作失败(Chat)
};

// 修改公会名称结果列举-------------------------------------------------------------------------------
enum ENUM_ModifyGuildNameResult
{
	ENUM_ModifyGuildNameResult_NULL = 0, 
	ENUM_ModifyGuildNameResult_Success,				// 成功.
	ENUM_ModifyGuildNameResult_WorldNotFound,		// 找不到世界
	ENUM_ModifyGuildNameResult_LeaderNotFound,		// 找不到领头观测者
	ENUM_ModifyGuildNameResult_LeaderDisconnect,	// 领头观测者未连线
	ENUM_ModifyGuildNameResult_GuildNotFound,		// 公会不存在
	ENUM_ModifyGuildNameResult_GuildNameRepeat,		// 公会名称重覆
	ENUM_ModifyGuildNameResult_GMOOperationFailed,	// 操作失败(GMO)
	ENUM_ModifyGuildNameResult_ChatOperationFailed,	// 操作失败(Chat)
};
// 修改公会等级结果列举-------------------------------------------------------------------------------
enum ENUM_ModifyGuildLvResult
{
	ENUM_ModifyGuildLvResult_NULL = 0, 
		ENUM_ModifyGuildLvResult_Success,				// 成功
		ENUM_ModifyGuildLvResult_WorldNotFound,			// 找不到世界
		ENUM_ModifyGuildLvResult_LeaderNotFound,		// 找不到领头观测者
		ENUM_ModifyGuildLvResult_LeaderDisconnect,		// 领头观测者未连线
		ENUM_ModifyGuildLvResult_LevelError1,			// 不正确的等级(超出正常范围)
		ENUM_ModifyGuildLvResult_GuildNotFound,			// 公会不存在
		ENUM_ModifyGuildLvResult_SameLevel,				// 公会目前已经是该等级
		ENUM_ModifyGuildLvResult_LevelError2,			// 不正确的等级(会导致公会最大人数上限小于公会目前人数)
		ENUM_ModifyGuildLvResult_GMOOperationFailed,	// 操作失败(GMO)
		ENUM_ModifyGuildLvResult_ChatOperationFailed,	// 操作失败(Chat)
};

// 修改角色任务结果列举-------------------------------------------------------------------------------
enum ENUM_ModifyCharacterQuestResult
{
		ENUM_ModifyCharacterQuestResult_NULL = 0, 
		ENUM_ModifyCharacterQuestResult_Success,				// 成功.
		ENUM_ModifyCharacterQuestResult_WorldNotFound,			// 找不到世界
		ENUM_ModifyCharacterQuestResult_LeaderNotFound,			// 找不到领头观测者
		ENUM_ModifyCharacterQuestResult_LeaderDisconnect,		// 领头观测者未连线
		ENUM_ModifyCharacterQuestResult_ArgumentError,			// 参数错误
		ENUM_ModifyCharacterQuestResult_RoleNotFound,			// 角色不存在
		ENUM_ModifyCharacterQuestResult_RoleIsOnline,			// 角色正在线上
		ENUM_ModifyCharacterQuestResult_GMOOperationFailed,		// 操作失败(GMO)
};

// 职业-----------------------------------------------------------------------------------------------
enum ENUM_Vocation
{
	ENUM_Vocation_NULL = -1, 

	ENUM_Vocation_1	= 0,		// 剑侠
	ENUM_Vocation_2,			// 灵蛇刀客
	ENUM_Vocation_3,			// 蝶衣舞者
	ENUM_Vocation_4,			// 五灵师
	ENUM_Vocation_5,			// 灵狐夜盗
	ENUM_Vocation_6,			// 练气士
};

// 取得角色列表结果-----------------------------------------------------------------------------------
enum ENUM_CharacterListResult
{
	ENUM_CharacterListResult_NULL = 0, 
	ENUM_CharacterListResult_Success,			// 成功
	ENUM_CharacterListResult_WorldNotFound,		// 找不到世界
	ENUM_CharacterListResult_LeaderNotFound,	// 找不到领头观测者
	ENUM_CharacterListResult_LeaderDisconnect,	// 领头观测者未连线 
};


// 取得公会成员列表结果-------------------------------------------------------------------------------
enum ENUM_GuildMemberListResult
{
	ENUM_GuildMemberListResult_NULL = 0, 
	ENUM_GuildMemberListResult_Success,					// 成功.
	ENUM_GuildMemberListResult_GuildNotFound,			// 公会不存在
	ENUM_GuildMemberListResult_WorldNotFound,			// 找不到世界
	ENUM_GuildMemberListResult_LeaderNotFound,			// 找不到领头观测者
	ENUM_GuildMemberListResult_LeaderDisconnect,		// 领头观测者未连线
};

// 取得角色背包资讯结果-------------------------------------------------------------------------------
enum ENUM_CharacterBagResult
{
	ENUM_CharacterBagResult_NULL = 0, 
	ENUM_CharacterBagResult_Success,			// 成功
	ENUM_CharacterBagResult_TypeError,			// 不正确的类型
	ENUM_CharacterBagResult_RoleNotFound,		// 角色不存在
	ENUM_CharacterBagResult_CharacterBagEmpty,	// 角色背包是空的
	ENUM_CharacterBagResult_WorldNotFound,		// 找不到世界
	ENUM_CharacterBagResult_LeaderNotFound,		// 找不到领头观测者
	ENUM_CharacterBagResult_LeaderDisconnect,	// 领头观测者未连线 
};


// 取得角色好友列表结果-------------------------------------------------------------------------------
enum ENUM_CharacterFriendListResult
{
	ENUM_CharacterFriendListResult_NULL = 0, 
	ENUM_CharacterFriendListResult_Success,						// 成功.
	ENUM_CharacterFriendListResult_RoleNotFound,				// 角色不存在
	ENUM_CharacterFriendListResult_CharacterFriendListEmpty,	// 角色无好友
	ENUM_CharacterFriendListResult_WorldNotFound,				// 找不到世界
	ENUM_CharacterFriendListResult_LeaderNotFound,				// 找不到领头观测者
	ENUM_CharacterFriendListResult_LeaderDisconnect,			// 领头观测者未连线
};

// 人际关系-------------------------------------------------------------------------------------------
enum ENUM_RelationshipPairType
{
	ENUM_RelationshipPairType_None = 0, 	
	ENUM_RelationshipPairType_Recognition,			// 认识
	ENUM_RelationshipPairType_FamilyMembers,		// 亲人
	ENUM_RelationshipPairType_Confidant, 			// 知音
	ENUM_RelationshipPairType_SwornBrothers, 		// 结义
	ENUM_RelationshipPairType_FightingBrother, 		// 战友
	ENUM_RelationshipPairType_MasterApprentice,		// 师徒
	ENUM_RelationshipPairType_ApprenticeMaster,		// 徒师
	ENUM_RelationshipPairType_Lover,				// 情侣
	ENUM_RelationshipPairType_Couple,				// 夫妻
};


// 取得角色基本资讯结果-------------------------------------------------------------------------------
enum ENUM_CharacterInfoResult
{
	ENUM_CharacterInfoResult_NULL = 0, 
	ENUM_CharacterInfoResult_Success,			// 成功
	ENUM_CharacterInfoResult_RoleNotFound,		// 角色不存在
	ENUM_CharacterInfoResult_WorldNotFound,		// 找不到世界
	ENUM_CharacterInfoResult_LeaderNotFound,	// 找不到领头观测者
	ENUM_CharacterInfoResult_LeaderDisconnect,	// 领头观测者未连线
};

// 取得角色任务资讯结果-------------------------------------------------------------------------------
enum ENUM_CharacterQuestResult
{
	ENUM_CharacterQuestResult_NULL = 0, 
	ENUM_CharacterQuestResult_Success,			// 成功
	ENUM_CharacterQuestResult_TypeError,		// 不正确的类型
	ENUM_CharacterQuestResult_RoleNotFound,		// 角色不存在
	ENUM_CharacterQuestResult_QuestNotFound,	// 没有符合的任务
	ENUM_CharacterQuestResult_WorldNotFound,	// 找不到世界
	ENUM_CharacterQuestResult_LeaderNotFound,	// 找不到领头观测者
	ENUM_CharacterQuestResult_LeaderDisconnect,	// 领头观测者未连线
};


// 取得公会基本资讯结果-------------------------------------------------------------------------------
enum ENUM_GuildBaseInfoResult
{
	ENUM_GuildBaseInfoResult_NULL = 0, 
	ENUM_GuildBaseInfoResult_Success,					// 成功.
	ENUM_GuildBaseInfoResult_GuildNotFound,				// 公会不存在
	ENUM_GuildBaseInfoResult_WorldNotFound,				// 找不到世界
	ENUM_GuildBaseInfoResult_LeaderNotFound,			// 找不到领头观测者
	ENUM_GuildBaseInfoResult_LeaderDisconnect,			// 领头观测者未连线
};

// 修改公会公告结果列举-------------------------------------------------------------------------------
enum ENUM_ModifyGuildNoticeResult
{
	ENUM_ModifyGuildNoticeResult_NULL = 0, 
		ENUM_ModifyGuildNoticeResult_Success,				// 成功
		ENUM_ModifyGuildNoticeResult_WorldNotFound,			// 找不到世界
		ENUM_ModifyGuildNoticeResult_LeaderNotFound,		// 找不到领头观测者
		ENUM_ModifyGuildNoticeResult_LeaderDisconnect,		// 领头观测者未连线
		ENUM_ModifyGuildNoticeResult_GuildNotFound,			// 公会不存在
		ENUM_ModifyGuildNoticeResult_GMOOperationFailed,	// 操作失败(GMO)
		ENUM_ModifyGuildNoticeResult_ChatOperationFailed,	// 操作失败(Chat)
};

// 修改角色金钱结果列举-------------------------------------------------------------------------------
enum ENUM_ModifyCharacterMoneyResult
{
		ENUM_ModifyCharacterMoneyResult_NULL = 0, 
		ENUM_ModifyCharacterMoneyResult_Success,				// 成功
		ENUM_ModifyCharacterMoneyResult_WorldNotFound,			// 找不到世界
		ENUM_ModifyCharacterMoneyResult_LeaderNotFound,			// 找不到领头观测者
		ENUM_ModifyCharacterMoneyResult_LeaderDisconnect,		// 领头观测者未连线
		ENUM_ModifyCharacterMoneyResult_TypeError,				// 不正确的类型
		ENUM_ModifyCharacterMoneyResult_RoleNotFound,			// 角色不存在
		ENUM_ModifyCharacterMoneyResult_RoleIsOnline,			// 角色正在线上
		ENUM_ModifyCharacterMoneyResult_CountError,				// 修改后的金额超出范围
		ENUM_ModifyCharacterMoneyResult_GMOOperationFailed,		// 操作失败(GMO)
};

// 修改角色删除密码结果列举---------------------------------------------------------------------------
enum ENUM_ModifyCharacterDropPwResult
{
	ENUM_ModifyCharacterDropPwResult_NULL = 0, 
		ENUM_ModifyCharacterDropPwResult_Success,				// 成功
		ENUM_ModifyCharacterDropPwResult_WorldNotFound,			// 找不到世界
		ENUM_ModifyCharacterDropPwResult_LeaderNotFound,		// 找不到领头观测者
		ENUM_ModifyCharacterDropPwResult_LeaderDisconnect,		// 领头观测者未连线
		ENUM_ModifyCharacterDropPwResult_PasswordError,			// 密码格式错误
		ENUM_ModifyCharacterDropPwResult_RoleNotFound,			// 角色不存在
		ENUM_ModifyCharacterDropPwResult_RoleIsOnline,			// 角色正在线上
		ENUM_ModifyCharacterDropPwResult_GMOOperationFailed,	// 操作失败(GMO)
};

// 修改帐号银行密码结果列举---------------------------------------------------------------------------
enum ENUM_ModifyAccountBankPwResult
{
		ENUM_ModifyAccountBankPwResult_NULL = 0, 
		ENUM_ModifyAccountBankPwResult_Success,				// 成功
		ENUM_ModifyAccountBankPwResult_WorldNotFound,			// 找不到世界
		ENUM_ModifyAccountBankPwResult_LeaderNotFound,		// 找不到领头观测者
		ENUM_ModifyAccountBankPwResult_LeaderDisconnect,		// 领头观测者未连线
		ENUM_ModifyAccountBankPwResult_PasswordError,			// 密码格式错误
		ENUM_ModifyAccountBankPwResult_RoleNotFound,			// 角色不存在
		ENUM_ModifyAccountBankPwResult_RoleIsOnline,			// 角色正在线上
		ENUM_ModifyAccountBankPwResult_GMOOperationFailed,	// 操作失败(GMO)
};

// 删除角色物品结果列举-------------------------------------------------------------------------------
enum ENUM_DelCharacterItemResult
{
		ENUM_DelCharacterItemResult_NULL = 0, 
		ENUM_DelCharacterItemResult_Success,			// 成功
		ENUM_DelCharacterItemResult_WorldNotFound,		// 找不到世界
		ENUM_DelCharacterItemResult_LeaderNotFound,		// 找不到领头观测者
		ENUM_DelCharacterItemResult_LeaderDisconnect,	// 领头观测者未连线
		ENUM_DelCharacterItemResult_TypeError,			// 不正确的类型
		ENUM_DelCharacterItemResult_PositionError,		// 不正确的位置
		ENUM_DelCharacterItemResult_RoleNotFound,		// 角色不存在
		ENUM_DelCharacterItemResult_RoleIsOnline,		// 角色正在线上
		ENUM_DelCharacterItemResult_ItemNotFound,		// 该位置物品不存在
		ENUM_DelCharacterItemResult_GMOOperationFailed,	// 操作失败(GMO)	
};

// 取得帐号资讯结果-----------------------------------------------------------------------------------
enum ENUM_AccountInfoResult
{
		ENUM_AccountInfoResult_NULL = 0, 
		ENUM_AccountInfoResult_Success,				// 成功
		ENUM_AccountInfoResult_AccountNotFound,		// 帐号不存在
		ENUM_AccountInfoResult_RoleNotFound,		// 帐号内没有角色
		ENUM_AccountInfoResult_WorldNotFound,		// 找不到世界
		ENUM_AccountInfoResult_LeaderNotFound,		// 找不到领头观测者
		ENUM_AccountInfoResult_LeaderDisconnect,	// 领头观测者未连线
};

// 取得角色进阶资讯结果-------------------------------------------------------------------------------
enum ENUM_CharacterAdvanceInfoResult
{
		ENUM_CharacterAdvanceInfoResult_NULL = 0, 
		ENUM_CharacterAdvanceInfoResult_Success,			// 成功
		ENUM_CharacterAdvanceInfoResult_RoleNotFound,		// 角色不存在
		ENUM_CharacterAdvanceInfoResult_WorldNotFound,		// 找不到世界
		ENUM_CharacterAdvanceInfoResult_LeaderNotFound,		// 找不到领头观测者
		ENUM_CharacterAdvanceInfoResult_LeaderDisconnect,	// 领头观测者未连线
};
// 取得角色黑名单列表结果-----------------------------------------------------------------------------
enum ENUM_CharacterBlackListResult
{
		ENUM_CharacterBlackListResult_NULL = 0, 
		ENUM_CharacterBlackListResult_Success,			// 成功.
		ENUM_CharacterBlackListResult_RoleNotFound,		// 角色不存在
		ENUM_CharacterBlackListResult_BlackListEmpty,	// 角色无黑名单
		ENUM_CharacterBlackListResult_WorldNotFound,	// 找不到世界
		ENUM_CharacterBlackListResult_LeaderNotFound,	// 找不到领头观测者
		ENUM_CharacterBlackListResult_LeaderDisconnect, // 领头观测者未连线
};

// 取得角色伏魔资讯结果-------------------------------------------------------------------------------
enum ENUM_CharacterMobResult
{
	ENUM_CharacterMobResult_NULL = 0, 
		ENUM_CharacterMobResult_Success,			// 成功
		ENUM_CharacterMobResult_RoleNotFound,		// 角色不存在
		ENUM_CharacterMobResult_WorldNotFound,		// 找不到世界
		ENUM_CharacterMobResult_LeaderNotFound,		// 找不到领头观测者
		ENUM_CharacterMobResult_LeaderDisconnect,	// 领头观测者未连线
};

// 移动角色至安全点结果列举---------------------------------------------------------------------------
enum ENUM_MoveCharacterToSafeResult
{
	ENUM_MoveCharacterToSafeResult_NULL = 0, 
		ENUM_MoveCharacterToSafeResult_Success,				// 成功.
		ENUM_MoveCharacterToSafeResult_RoleNotFound,		// 角色不存在
		ENUM_MoveCharacterToSafeResult_WorldNotFound,		// 找不到世界
		ENUM_MoveCharacterToSafeResult_LeaderNotFound,		// 找不到领头观测者
		ENUM_MoveCharacterToSafeResult_LeaderDisconnect,	// 领头观测者未连线
		ENUM_MoveCharacterToSafeResult_GMOOperationFailed,	// 操作失败(GMO)
		ENUM_MoveCharacterToSafeResult_LocalOperationFailed,// 操作失败(Local)
};

// 取得角色图鉴资讯结果-------------------------------------------------------------------------------
enum ENUM_CharacterPuzzleMapResult
{
		ENUM_CharacterPuzzleMapResult_NULL = 0, 
		ENUM_CharacterPuzzleMapResult_Success,				// 成功
		ENUM_CharacterPuzzleMapResult_RoleNotFound,			// 角色不存在
		ENUM_CharacterPuzzleMapResult_WorldNotFound,		// 找不到世界
		ENUM_CharacterPuzzleMapResult_LeaderNotFound,		// 找不到领头观测者
		ENUM_CharacterPuzzleMapResult_LeaderDisconnect,		// 领头观测者未连线
};

// 取得角色技能资讯结果-------------------------------------------------------------------------------
enum ENUM_CharacterSkillResult
{
	ENUM_CharacterSkillResult_NULL = 0, 
		ENUM_CharacterSkillResult_Success,				// 成功
		ENUM_CharacterSkillResult_RoleNotFound,			// 角色不存在
		ENUM_CharacterSkillResult_WorldNotFound,		// 找不到世界
		ENUM_CharacterSkillResult_LeaderNotFound,		// 找不到领头观测者
		ENUM_CharacterSkillResult_LeaderDisconnect,		// 领头观测者未连线
};
// 删除角色伏魔结果列举-------------------------------------------------------------------------------
enum ENUM_DelCharacterMobResult
{
	ENUM_DelCharacterMobResult_NULL = 0, 
		ENUM_DelCharacterMobResult_Success,				// 成功
		ENUM_DelCharacterMobResult_WorldNotFound,		// 找不到世界
		ENUM_DelCharacterMobResult_LeaderNotFound,		// 找不到领头观测者
		ENUM_DelCharacterMobResult_LeaderDisconnect,	// 领头观测者未连线
		ENUM_DelCharacterMobResult_RoleNotFound,		// 角色不存在
		ENUM_DelCharacterMobResult_RoleIsOnline,		// 角色正在线上
		ENUM_DelCharacterMobResult_MobNotFound,			// 角色尚未取得该伏魔
		ENUM_DelCharacterMobResult_FlagError,			// 旗标错误
		ENUM_DelCharacterMobResult_GMOOperationFailed,	// 操作失败(GMO)	
};
// 修改角色图鉴结果列举-------------------------------------------------------------------------------
enum ENUM_ModifyPuzzleMapResult
{
	ENUM_ModifyPuzzleMapResult_NULL = 0, 
		ENUM_ModifyPuzzleMapResult_Success,				// 成功
		ENUM_ModifyPuzzleMapResult_WorldNotFound,		// 找不到世界
		ENUM_ModifyPuzzleMapResult_LeaderNotFound,		// 找不到领头观测者
		ENUM_ModifyPuzzleMapResult_LeaderDisconnect,	// 领头观测者未连线
		ENUM_ModifyPuzzleMapResult_RoleNotFound,		// 角色不存在
		ENUM_ModifyPuzzleMapResult_RoleIsOnline,		// 角色正在线上
		ENUM_ModifyPuzzleMapResult_FlagError,			// 旗标错误
		ENUM_ModifyPuzzleMapResult_GMOOperationFailed,	// 操作失败(GMO)	
};
// 重新载入功能开关列表结果列举-----------------------------------------------------------------------
enum ENUM_ReloadFunctionSwitchResult
{
	ENUM_ReloadFunctionSwitchResult_NULL = 0, 
		ENUM_ReloadFunctionSwitchResult_Success,			// 成功.
		ENUM_ReloadFunctionSwitchResult_WorldNotFound,		// 找不到世界
		ENUM_ReloadFunctionSwitchResult_LeaderNotFound,		// 找不到领头观测者
		ENUM_ReloadFunctionSwitchResult_LeaderDisconnect,	// 领头观测者未连线
		ENUM_ReloadFunctionSwitchResult_LocalDisconnect,	// 区域伺服器未连线
};

// add by ltx
// 寄送客制物品邮件结果列举-------------------------------------------------------------------------------
enum ENUM_SendCustomItemMailResult
{
	ENUM_SendCustomItemMailResult_NULL = 0, 
		ENUM_SendCustomItemMailResult_Success,				// 成功.
		ENUM_SendCustomItemMailResult_ArgumentError,		// 参数错误
		ENUM_SendCustomItemMailResult_ItemIDError,			// 不正确的物品编号
		ENUM_SendCustomItemMailResult_WorldNotFound,		// 找不到世界
		ENUM_SendCustomItemMailResult_LeaderNotFound,		// 找不到领头观测者
		ENUM_SendCustomItemMailResult_LeaderDisconnect,		// 领头观测者未连线
		ENUM_SendCustomItemMailResult_RoleNotFound,			// 角色不存在
		ENUM_SendCustomItemMailResult_LocalDisconnect,		// 区域伺服器未连线
		ENUM_SendCustomItemMailResult_LocalOperationFailed,	// 操作失败(Local)
};

// 物品资料-------------------------------------------------------------------------------------------
struct S_GMItemData
{
	int    iPos;				// 位置(身上背包0~99物品栏,100~115装备栏,116~118生产用品栏)
	int	   iItemType;			// 物件类型(0:一般物品, 1:宠物)
	int    iOrgObjID;			// 物件样版ID
	int    iCreateTime;			// 建立时间
	int    iSerial;				// 序号(辨识物品是否有拷贝)
	char   szName[32];			// 名称
	time_t iLiveTime;			// 生存到期时间,(小于1代表无限期)
	int    iCoolDown;			// 下次可以使用的时间(小于1代表立即可用)
	bool   bNPCTrade;			// 是否可以跟NPC交易(1:可, 0:不可)
	bool   bUserTrade;			// 是否可以跟玩家交易(1:可, 0:不可)
	bool   bItemMallGoods;		// 是否是商城物品(1:是, 0:不是)
	int    iCount;				// 数量
	int    iSellCost;			// 卖价(商店 or 摆摊)
	short  wDurable; 			// 目前耐久度
	//-------------------------------------
	union
	{
		int	iInheritID[10];						// 玄石ID, -1:没有开洞 0:已开洞但没有设玄石	

		// 宠物用资讯
		struct 
		{
			unsigned int iExp;					// 经验值
			unsigned int iNowHP;				// 目前HP值
			unsigned int iPetIndex;				// 宠物索引编号(0~256)						
			unsigned int iRenewCount;			// 还童次数(0-255)
			unsigned int iLV;					// 等级(1-50级)
			unsigned int iGeneration;			// 世代(1-31)
			unsigned int iMixCount;				// 合成次数(0-31)		
			unsigned int iGrowthRate;			// 成长率(1-10) 
			unsigned int iAttrQuality;			// 品质(0-10),指属性的成长度(冲宠物)
			// 属性	
			unsigned int iHP;					// HP加乘值(0~32767)
			unsigned int iWushu;				// 武术(0~2047)
			unsigned int iSpirit;				// 灵性(0~2047)
			unsigned int iConstitution;			// 防御(0~2047)
			unsigned int iQuality;				// 灵御(0~2047)
			unsigned int iDexterity;			// 身法(0~2047)
			unsigned int iFate;					// 吉运(0~2047)
			unsigned int iHP_Plus;				// HP值慧根(0-127)
			unsigned int iWushu_Plus;			// 武术慧根(0-127)
			unsigned int iSpirit_Plus;			// 灵性慧根(0-127)
			unsigned int iConstitution_Plus;	// 防御慧根(0-127)
			unsigned int iQuality_Plus;			// 灵御慧根(0-127)
			unsigned int iDexterity_Plus;		// 身法慧根(0-127)
			unsigned int iFate_Plus;			// 吉运慧根(0-127)
			// 技能		
			char sSkill[6];						// 6个技能栏位(-1:未开启 0:未放置技能 1:宠物技能索引)
		}sPet;// struct	
	};// union
};
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
	char szContent[512];		// 公告内容

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

// GMC->GMS:要求新增排程公告
struct PGServerControl_CtoS_AddScheduleBulletin  : public S_ObjNetPktBase
{
	int  iWorldID;				// 世界编号
	int  iBulletinID;			// 公告ID
	int	 iZoneID;				// 区堿ID,-1代表全区
	int  iMonth;				// 排程时间-月
	int  iDay;					// 排程时间-日
	int  iDayOfWeek;			// 排程时间-星期
	int  iHour;					// 排程时间-时
	int  iMinute;				// 排程时间-分
	char szContent[512];		// 公告内容

	PGServerControl_CtoS_AddScheduleBulletin()
	{
		uiCmdID = ENUM_PGServerControl_CtoS_AddScheduleBulletin;
	};
};

// GMS->GMC:回传新增排程公告结果----------------------------------------------------------------------
struct PGServerControl_StoC_AddScheduleBulletin : public S_ObjNetPktBase
{
	ENUM_AddScheduleBulletinResult emResult;	// 结果列举
	int iWorldID;								// 世界编号
	int iBulletinID;							// 公告ID

	PGServerControl_StoC_AddScheduleBulletin()
	{
		uiCmdID = ENUM_PGServerControl_StoC_AddScheduleBulletin;
	};
};
// GMC->GMS:要求删除排程公告
struct PGServerControl_CtoS_DelScheduleBulletin  : public S_ObjNetPktBase
{
	int iWorldID;							// 世界编号
	int iBulletinID;						// 延迟发送时间(秒),0代表立即发送

	PGServerControl_CtoS_DelScheduleBulletin()
	{
		uiCmdID = ENUM_PGServerControl_CtoS_DelScheduleBulletin;
	};
};

// GMS->GMC:回传删除排程公告结果----------------------------------------------------------------------
struct PGServerControl_StoC_DelScheduleBulletin : public S_ObjNetPktBase
{
	ENUM_DelScheduleBulletinResult emResult;	// 结果列举
	int iWorldID;								// 世界编号
	int iBulletinID;							// 公告ID

	PGServerControl_StoC_DelScheduleBulletin()
	{
		uiCmdID = ENUM_PGServerControl_StoC_DelScheduleBulletin;
	};
};
// GMC->GMS:要求重新载入排程公告----------------------------------------------------------------------
struct PGServerControl_CtoS_ReloadScheduleBulletin  : public S_ObjNetPktBase
{
	int iWorldID;		// 世界编号

	PGServerControl_CtoS_ReloadScheduleBulletin()
	{
		uiCmdID = ENUM_PGServerControl_CtoS_ReloadScheduleBulletin;
	};
};

// GMS->GMC:回传重新载入排程公告结果------------------------------------------------------------------
struct PGServerControl_StoC_ReloadScheduleBulletin : public S_ObjNetPktBase
{
	ENUM_ReloadScheduleBulletinResult emResult;	// 结果列举
	int iWorldID;								// 世界编号

	PGServerControl_StoC_ReloadScheduleBulletin()
	{
		uiCmdID = ENUM_PGServerControl_StoC_ReloadScheduleBulletin;
	};
};
// GMC->GMS:要求寄送奖品邮件--------------------------------------------------------------------------
struct PGServerControl_CtoS_SendGiftMail  : public S_ObjNetPktBase
{
	int  iWorldID;			// 世界编号
	char szRoleName[32];	// 角色名称
	char szSubject[32];		// 邮件主旨
	char szContent[256];	// 邮件内文
	int  iMoney;			// 邮件金钱
	int  aiGiftDBID[5];		// 奖品编号1~5(-1代表无附件)
	char szReason[1024];	// 发送奖品原因
	
	PGServerControl_CtoS_SendGiftMail()
	{
		uiCmdID = ENUM_PGServerControl_CtoS_SendGiftMail;
	};
};

// GMS->GMC:回传寄送奖品邮件结果----------------------------------------------------------------------
struct PGServerControl_StoC_SendGiftMail : public S_ObjNetPktBase
{
	ENUM_SendGiftMailResult emResult;	// 结果列举
	int  iWorldID;						// 世界编号
	char szRoleName[32];				// 角色名称
	char szSubject[32];					// 邮件主旨
	int  iMoney;						// 邮件金钱
	int  aiGiftDBID[5];					// 奖品编号1~5(-1代表无附件)
	int  aiItemStatus[5];				// 附件1~5物品状态,状态意义如下:
	// 0:建立成功或无附件,			1:奖品样版资料错误(奖品编号)
	// 2:奖品样版资料错误(物品编号),3:奖品样版资料错误(玄石编号)
	// 4:奖品样版资料错误(绑定),	5:奖品样版资料错误(数量)
	// 6:建立物品失败
	PGServerControl_StoC_SendGiftMail()
	{
		uiCmdID = ENUM_PGServerControl_StoC_SendGiftMail;
	};
};

// GMC->GMS:要求取得上线人数
struct PGServerStatus_CtoS_PlayerOnlineCount : public S_ObjNetPktBase
{
	int iWorldID; // 世界编号
		
	PGServerStatus_CtoS_PlayerOnlineCount()
	{
		uiCmdID = ENUM_PGServerStatus_CtoS_PlayerOnlineCount;
	};
};

// GMS -> GMC 回传取得上线人数结果
struct PGServerStatus_StoC_PlayerOnlineCount : public S_ObjNetPktBase
{
	ENUM_PlayerOnlineCountResult emResult;	// 结果列举
	int	iWorldID;							// 世界编号
	int	iCount;								// 上线人数
		
	PGServerStatus_StoC_PlayerOnlineCount()
	{
		uiCmdID = ENUM_PGServerStatus_StoC_PlayerOnlineCount;
	};
};





// GMC->GMS:要求取得排程公告列表
struct PGServerStatus_CtoS_ScheduleBulletinList  : public S_ObjNetPktBase
{
	int iWorldID; // 世界编号
		
	PGServerStatus_CtoS_ScheduleBulletinList()
	{
		uiCmdID = ENUM_PGServerStatus_CtoS_ScheduleBulletinList;
	};
};

// GMS->GMC:回传取得排程公告列表结果------------------------------------------------------------------
struct PGServerStatus_StoC_ScheduleBulletinList : public S_ObjNetPktBase
{
	ENUM_ScheduleBulletinListResult emResult;	// 结果列举
	int	 iWorldID;								// 世界编号,-1代表结束封包
	int  iBulletinID;							// 公告ID
	int	 iZoneID;								// 区堿ID,-1代表全区
	int  iMonth;								// 排程时间-月
	int  iDay;									// 排程时间-日
	int  iDayOfWeek;							// 排程时间-星期
	int  iHour;									// 排程时间-时
	int  iMinute;								// 排程时间-分
	char szContent[512];						// 公告内容
	
	PGServerStatus_StoC_ScheduleBulletinList()
	{
		uiCmdID = ENUM_PGServerStatus_StoC_ScheduleBulletinList;
	};
};

// GMC->GMS:要求踢某玩家下线--------------------------------------------------------------------------
struct PGPlayerControl_CtoS_BanishPlayer  : public S_ObjNetPktBase
{
	int  iWorldID;				// 世界编号
	char szRoleName[32];		// 角色名称

	PGPlayerControl_CtoS_BanishPlayer()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_BanishPlayer;
	};
};

// GMS->GMC:回传踢某玩家下线结果----------------------------------------------------------------------
struct PGPlayerControl_StoC_BanishPlayer : public S_ObjNetPktBase
{
	ENUM_BanishPlayerResult emResult;	// 踢某玩家下线结果
	int  iWorldID;						// 世界编号
	char szRoleName[32];				// 角色名称

	PGPlayerControl_StoC_BanishPlayer()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_BanishPlayer;
	};
};
// GMC->GMS:要求取得角色背包资讯----------------------------------------------------------------------
struct PGPlayerStatus_CtoS_CharacterBag : public S_ObjNetPktBase
{
	int		iWorldID;						// 世界编号
	char	szRoleName[32];					// 角色名称
	int		iBagType;						// 背包类型(0:身上 1:宠物包 2:PK不掉宝包 3:银行)

	PGPlayerStatus_CtoS_CharacterBag()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_CharacterBag;
	};
};

// GMS->GMC:回传取得角色背包资讯结果------------------------------------------------------------------
struct PGPlayerStatus_StoC_CharacterBag : public S_ObjNetPktBase
{
	ENUM_CharacterBagResult emResult;		// 结果列举
	int		iWorldID;						// 世界编号,-1代表结束封包
	char	szAccount[32];					// 帐号
	char	szRoleName[32];					// 角色名称
	int		iBagType;						// 背包类型(0:身上 1:宠物包 2:PK不掉宝包 3:银行)
	S_GMItemData sGMItemData;				// 物品资料

	PGPlayerStatus_StoC_CharacterBag()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_CharacterBag;
	};
};
// GMC->GMS:要求取得角色好友列表----------------------------------------------------------------------
struct PGPlayerStatus_CtoS_CharacterFriendList : public S_ObjNetPktBase
{
	int		iWorldID;							// 世界编号
	char	szRoleName[32];		// 角色名称

	PGPlayerStatus_CtoS_CharacterFriendList()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_CharacterFriendList;
	};
};

// GMS->GMC:回传取得角色好友列表结果------------------------------------------------------------------
struct PGPlayerStatus_StoC_CharacterFriendList : public S_ObjNetPktBase
{
	ENUM_CharacterFriendListResult emResult;	// 结果列举
	int		iWorldID;							// 世界编号,-1代表结束封包
	char	szAccount[32];						// 帐号
	char	szRoleName[32];						// 角色名称
	char	szFriendName[32];					// 好友角色名称
	ENUM_RelationshipPairType emRelation;		// 关系

	PGPlayerStatus_StoC_CharacterFriendList()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_CharacterFriendList;
	};
};
// 角色基本资料---------------------------------------------------------------------------------------
struct S_GMRoleInfo
{
	int	 X, Y, Z;				// 所在位置(x,y,z座标)
	char szNickName[32];		// 昵称
	char szGuildName[32];		// 公会名称
	int	 iGuildPowerLevel;		// 公会职级(-1代表无公会)
	ENUM_Vocation emVoc;		// 职业
	int	 iLevel;				// 等级
	int  iExp;					// 目前经验
	int	 iBodyMoney;			// 身上金钱
    int	 iBankMoney;			// 仓库金钱
	
    int	 iSex;					// 性别(0:女 1:男)
	int  iFame;					// 名声
    int	 iHP;					// 目前血量
	int	 iMP;					// 目前法力
	int	 iAP;					// 目前真气
	int	 iEP;					// 目前精力
	int	 iSkillPoint;			// 可配置技能点数
	int	 iSkillTreeNode; 		// 目前技能树走到的位置
	int	 iReincarnation; 		// 转生次数
	
	int  iTitleID;				// 自选称号ID(-1代表不使用)
	int  dwTitleFlag[32];		// 目前已取得的称号旗标(使用此旗标向PalDataReader.lib查询具体称号资讯)
	int  iMurdererCount; 		// 邪气值
	int	 iEvilCount;			// 恶业值
	
	// 以下六个栏位,当角色在线时方可取得,不在线时为0
	int  iWushu;				// 武术
	int  iConstitution;			// 体质
	int  iSpirit; 				// 灵性
	int  iQuality;				// 灵耐
	int  iDexterity;			// 身法
	int  iFate;					// 吉运
	
	S_GMRoleInfo()
	{
		szGuildName[0] = '\0';
		iGuildPowerLevel = -1;
	}
};
// 生产技能结构---------------------------------------------------------------------------------------
struct S_GMLifeHoodSkill
{
	// 共有6种生产技能,各索引代表的意义为: 0挖矿,1采收,2摄魂,3冶铁,4缝纫,5炼器
	int iLevel[6];		// 等级
	int iEXP[6];		// 经验值
	
	S_GMLifeHoodSkill()
	{
		for (int i=0; i<6; i++)
		{
			iLevel[i] = 0;
			iEXP[i]   = 0;
		}
	}
};
// 帐号角色资讯---------------------------------------------------------------------------------------
struct S_GMAccountRoleInfo
{
	int	 iDBID;					// 角色DBID(-1代表无资料)
	char szRoleName[32];		// 角色名称
	char szCreateTime[32];		// 建立时间
	
	S_GMAccountRoleInfo()
	{
		iDBID = -1;
	}
};
// GMC->GMS:要求取得角色基本资讯----------------------------------------------------------------------
struct PGPlayerStatus_CtoS_CharacterInfo : public S_ObjNetPktBase
{
	int  iWorldID;					// 世界编号
	char szRoleName[32];			// 角色名称

	PGPlayerStatus_CtoS_CharacterInfo()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_CharacterInfo;
	};
};

// GMS->GMC:回传取得角色基本资讯结果------------------------------------------------------------------
struct PGPlayerStatus_StoC_CharacterInfo : public S_ObjNetPktBase
{
	ENUM_CharacterInfoResult emResult;	// 结果列举
	int	 iWorldID;						// 世界编号
	char szAccount[32];					// 帐号
	char szRoleName[32];				// 角色名称
	int	 iDBID;							// 角色DBID
	int	 iZoneID;						// 区域编号
	char szZoneName[32];				// 区域名称
	int	 iLoginTime;					// 最后登入时间
	char szIP[16];						// 玩家IP,若为空字串表示角色不在线上
	S_GMRoleInfo sGMRoleInfo;			// 角色资讯
	
	PGPlayerStatus_StoC_CharacterInfo()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_CharacterInfo;
	};
};
// GMC->GMS:要求取得角色列表--------------------------------------------------------------------------
struct PGPlayerStatus_CtoS_CharacterList : public S_ObjNetPktBase
{
	int iWorldID; // 世界编号

	PGPlayerStatus_CtoS_CharacterList()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_CharacterList;
	};
};

// GMS->GMC:回传取得角色列表结果----------------------------------------------------------------------
struct PGPlayerStatus_StoC_CharacterList : public S_ObjNetPktBase
{
	ENUM_CharacterListResult emResult;	// 结果列举
	int		iWorldID;					// 世界编号,-1代表结束封包
	int		iDBID;						// 角色DBID
	char	szAccount[32];				// 帐号
	char	szRoleName[32];				// 角色名称
	int		iZoneID;					// 区域编号
	char	szZoneName[32];				// 区域名称
	int		iLoginTime;					// 登入时间
	char	szIP[16];					// IP,如果此栏为空字串表示不在线上

	PGPlayerStatus_StoC_CharacterList()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_CharacterList;
	};
};
// GMC->GMS:要求取得角色任务资讯----------------------------------------------------------------------
struct PGPlayerStatus_CtoS_CharacterQuest : public S_ObjNetPktBase
{
	int		iWorldID;							// 世界编号
	char	szRoleName[32];						// 角色名称
	int		iQuestType;							// 任务类型(1:已完成 2:已接)

	PGPlayerStatus_CtoS_CharacterQuest()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_CharacterQuest;
	};
};

// GMS->GMC:回传取得角色任务资讯结果------------------------------------------------------------------
struct PGPlayerStatus_StoC_CharacterQuest : public S_ObjNetPktBase
{
	ENUM_CharacterQuestResult emResult;			// 结果列举
	int		iWorldID;							// 世界编号,-1代表结束封包
	char	szAccount[32];						// 帐号
	char	szRoleName[32];						// 角色名称
	int		iQuestType;							// 任务类型(1:已完成 2:已接)
	int		iQuestID;							// 任务编号

	PGPlayerStatus_StoC_CharacterQuest()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_CharacterQuest;
	};
};

// GMC->GMS:要求解散公会------------------------------------------------------------------------------
struct PGPlayerControl_CtoS_DismissGuild  : public S_ObjNetPktBase
{
	int  iWorldID;				// 世界编号
	char szGuildName[32];		// 公会名称

	PGPlayerControl_CtoS_DismissGuild()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_DismissGuild;
	};
};

// GMS->GMC:回传解散公会结果--------------------------------------------------------------------------
struct PGPlayerControl_StoC_DismissGuild : public S_ObjNetPktBase
{
	ENUM_DismissGuildResult emResult;	// 结果列举
	int  iWorldID;								// 世界编号
	char szGuildName[32];						// 公会名称

	PGPlayerControl_StoC_DismissGuild()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_DismissGuild;
	};
};
// GMC->GMS:要求取得公会基本资讯----------------------------------------------------------------------
struct PGPlayerStatus_CtoS_GuildBaseInfo : public S_ObjNetPktBase
{
	int		iWorldID;				// 世界编号
	char	szGuildName[32];		// 公会名称

	PGPlayerStatus_CtoS_GuildBaseInfo()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_GuildBaseInfo;
	};
};

// GMS->GMC:回传取得公会基本资讯结果------------------------------------------------------------------
struct PGPlayerStatus_StoC_GuildBaseInfo : public S_ObjNetPktBase
{
	ENUM_GuildBaseInfoResult emResult;		// 结果列举
	int	 iWorldID;							// 世界编号
	char szGuildName[32];					// 公会名称
	int  iLevel;							// 公会等级
	int  iScore;							// 公会积分
	int  iMoney;							// 公会金钱

	PGPlayerStatus_StoC_GuildBaseInfo()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_GuildBaseInfo;
	};
};
// GMC->GMS:要求取得公会成员列表----------------------------------------------------------------------
struct PGPlayerStatus_CtoS_GuildMemberList : public S_ObjNetPktBase
{
	int		iWorldID;				// 世界编号
	char	szGuildName[32];		// 公会名称

	PGPlayerStatus_CtoS_GuildMemberList()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_GuildMemberList;
	};
};

// GMS->GMC:回传取得公会成员列表结果------------------------------------------------------------------
struct PGPlayerStatus_StoC_GuildMemberList : public S_ObjNetPktBase
{
	ENUM_GuildMemberListResult emResult;	// 结果列举
	int	 iWorldID;							// 世界编号,-1代表结束封包
	char szGuildName[32];					// 公会名称
	char szRoleName[32];					// 成员角色名称
	int  iLevel;							// 成员职等
	int  iJoinDate;							// 加入时间

	PGPlayerStatus_StoC_GuildMemberList()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_GuildMemberList;
	};
};
// GMC->GMS:要求修改角色任务--------------------------------------------------------------------------
struct PGPlayerControl_CtoS_ModifyCharacterQuest  : public S_ObjNetPktBase
{
	int	 iWorldID;			// 世界编号
	char szRoleName[32];	// 角色名称
	int	 iQuestID;			// 任务编号(0~6143)
	int	 iQuestStatus;		// 任务状态(0:未解 1:已完成 2:已接)

	PGPlayerControl_CtoS_ModifyCharacterQuest()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_ModifyCharacterQuest;
	};
};

// GMS->GMC:回传修改角色任务结果----------------------------------------------------------------------
struct PGPlayerControl_StoC_ModifyCharacterQuest : public S_ObjNetPktBase
{
	ENUM_ModifyCharacterQuestResult emResult;	// 结果列举
	int  iWorldID;								// 世界编号
	char szRoleName[32];						// 角色名称
	int	 iQuestID;								// 任务编号(0~6143)
	int	 iQuestStatus;							// 任务状态(0:未解 1:已完成 2:已接)

	PGPlayerControl_StoC_ModifyCharacterQuest()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_ModifyCharacterQuest;
	};
};
// GMC->GMS:要求修改公会成员职阶----------------------------------------------------------------------
struct PGPlayerControl_CtoS_ModifyGuildMemberPL  : public S_ObjNetPktBase
{
	int  iWorldID;				// 世界编号
	char szGuildName[32];		// 公会名称
	char szRoleName[32];		// 成员角色名称
	int  iLevel;				// 职阶(0:会员 1:二级干部 2:一级干部 3:副会长 4:会长)

	PGPlayerControl_CtoS_ModifyGuildMemberPL()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_ModifyGuildMemberPL;
	};
};

// GMS->GMC:回传修改公会成员职阶结果------------------------------------------------------------------
struct PGPlayerControl_StoC_ModifyGuildMemberPL : public S_ObjNetPktBase
{
	ENUM_ModifyGuildMemberPLResult emResult;	// 结果列举
	int  iWorldID;								// 世界编号
	char szGuildName[32];						// 公会名称
	char szRoleName[32];						// 成员角色名称
	int  iLevel;								// 职阶(0:会员 1:二级干部 2:一级干部 3:副会长 4:会长)

	PGPlayerControl_StoC_ModifyGuildMemberPL()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_ModifyGuildMemberPL;
	};
};
// GMC->GMS:要求修改公会等级--------------------------------------------------------------------------
struct PGPlayerControl_CtoS_ModifyGuildLv  : public S_ObjNetPktBase
{
	int  iWorldID;				// 世界编号
	char szGuildName[32];		// 公会名称
	int	 iLevel;				// 等级(1~25)
	
	PGPlayerControl_CtoS_ModifyGuildLv()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_ModifyGuildLv;
	};
};

// GMS->GMC:回传修改公会等级结果----------------------------------------------------------------------
struct PGPlayerControl_StoC_ModifyGuildLv : public S_ObjNetPktBase
{
	ENUM_ModifyGuildLvResult emResult;	// 结果列举
	int  iWorldID;						// 世界编号
	char szGuildName[32];				// 公会名称
	int	 iLevel;						// 等级(1~25)
	
	PGPlayerControl_StoC_ModifyGuildLv()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_ModifyGuildLv;
	};
};

// GMC->GMS:要求修改公会名称--------------------------------------------------------------------------
struct PGPlayerControl_CtoS_ModifyGuildName  : public S_ObjNetPktBase
{
	int  iWorldID;				// 世界编号
	char szGuildName[32];		// 原公会名称
	char szNewName[32];			// 新公会名称

	PGPlayerControl_CtoS_ModifyGuildName()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_ModifyGuildName;
	};
};

// GMS->GMC:回传修改公会名称结果----------------------------------------------------------------------
struct PGPlayerControl_StoC_ModifyGuildName : public S_ObjNetPktBase
{
	ENUM_ModifyGuildNameResult emResult;	// 结果列举
	int  iWorldID;							// 世界编号
	char szGuildName[32];					// 原公会名称
	char szNewName[32];						// 新公会名称

	PGPlayerControl_StoC_ModifyGuildName()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_ModifyGuildName;
	};
};

// GMC->GMS:要求修改公会公告--------------------------------------------------------------------------
struct PGPlayerControl_CtoS_ModifyGuildNotice  : public S_ObjNetPktBase
{
	int  iWorldID;				// 世界编号
	char szGuildName[32];		// 公会名称
	char szNotice[512];			// 公告
	
	PGPlayerControl_CtoS_ModifyGuildNotice()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_ModifyGuildNotice;
	};
};

// GMS->GMC:回传修改公会公告结果----------------------------------------------------------------------
struct PGPlayerControl_StoC_ModifyGuildNotice : public S_ObjNetPktBase
{
	ENUM_ModifyGuildNoticeResult emResult;	// 结果列举
	int  iWorldID;							// 世界编号
	char szGuildName[32];					// 公会名称
	char szNotice[512];						// 公告
	
	PGPlayerControl_StoC_ModifyGuildNotice()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_ModifyGuildNotice;
	};
};

// GMC->GMS:要求修改角色金钱--------------------------------------------------------------------------
struct PGPlayerControl_CtoS_ModifyCharacterMoney  : public S_ObjNetPktBase
{
	int	 iWorldID;			// 世界编号
	char szRoleName[32];	// 角色名称
	int	 iType;				// 类型(0:身上 1:银行)
	int  iCount;			// 增加金额(若为负数则代表减少)
	
	PGPlayerControl_CtoS_ModifyCharacterMoney()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_ModifyCharacterMoney;
	};
};

// GMS->GMC:回传修改角色金钱结果----------------------------------------------------------------------
struct PGPlayerControl_StoC_ModifyCharacterMoney : public S_ObjNetPktBase
{
	ENUM_ModifyCharacterMoneyResult emResult;	// 结果列举
	int	 iWorldID;								// 世界编号
	char szRoleName[32];						// 角色名称
	int	 iType;									// 类型(0:身上 1:银行)
	int  iCount;								// 增加金额(若为负数则代表减少)
	
	PGPlayerControl_StoC_ModifyCharacterMoney()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_ModifyCharacterMoney;
	};
};

// GMC->GMS:要求修改角色删除密码----------------------------------------------------------------------
struct PGPlayerControl_CtoS_ModifyCharacterDropPw  : public S_ObjNetPktBase
{
	int	 iWorldID;			// 世界编号
	char szRoleName[32];	// 角色名称
	char szDropRolePw[13];	// 新删角密码(6~12英数字,最后一个字元必须是结束字元)
	
	PGPlayerControl_CtoS_ModifyCharacterDropPw()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_ModifyCharacterDropPw;
	};
};

// GMS->GMC:回传修改角色删除密码结果------------------------------------------------------------------
struct PGPlayerControl_StoC_ModifyCharacterDropPw : public S_ObjNetPktBase
{
	ENUM_ModifyCharacterDropPwResult emResult;	// 结果列举
	int	 iWorldID;								// 世界编号
	char szRoleName[32];						// 角色名称
	
	PGPlayerControl_StoC_ModifyCharacterDropPw()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_ModifyCharacterDropPw;
	};
};

// GMC->GMS:要求修改帐号银行密码----------------------------------------------------------------------
struct PGPlayerControl_CtoS_ModifyAccountBankPw  : public S_ObjNetPktBase
{
	int	 iWorldID;			// 世界编号
	char szAccount[32];   // 帐号
	char szBankPw[7];		// 新银行密码(6位英数字)
	
	PGPlayerControl_CtoS_ModifyAccountBankPw()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_ModifyAccountBankPw;
	};
};

// GMS->GMC:回传修改帐号银行密码结果------------------------------------------------------------------
struct PGPlayerControl_StoC_ModifyAccountBankPw : public S_ObjNetPktBase
{
	ENUM_ModifyAccountBankPwResult emResult;	// 结果列举
	int	 iWorldID;								// 世界编号
	char szAccount[32];						// 角色名称
	
	PGPlayerControl_StoC_ModifyAccountBankPw()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_ModifyAccountBankPw;
	};
};

// GMC->GMS:要求删除角色物品--------------------------------------------------------------------------
struct PGPlayerControl_CtoS_DelCharacterItem  : public S_ObjNetPktBase
{
	int	 iWorldID;			// 世界编号
	char szRoleName[32];	// 角色名称
	int	 iType;				// 类型(0:身上 1:宠物包 2:PK不掉宝包 3:银行)
	int	 iPosition;			// 位置(范围:身上背包0~99物品栏,100~115装备栏,116~118生产用品栏,
	//						 宠物包0~4,:PK不掉宝包0~4,银行0~49)
	
	PGPlayerControl_CtoS_DelCharacterItem()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_DelCharacterItem;
	};
};

// GMS->GMC:回传删除角色物品结果----------------------------------------------------------------------
struct PGPlayerControl_StoC_DelCharacterItem : public S_ObjNetPktBase
{
	ENUM_DelCharacterItemResult emResult;	// 结果列举
	int	 iWorldID;							// 世界编号
	char szRoleName[32];					// 角色名称
	int	 iType;								// 类型(0:身上 1:宠物包 2:PK不掉宝包 3:银行)
	int	 iPosition;							// 位置
	
	PGPlayerControl_StoC_DelCharacterItem()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_DelCharacterItem;
	};
};
// GMC->GMS:要求取得帐号资讯--------------------------------------------------------------------------
struct PGPlayerStatus_CtoS_AccountInfo : public S_ObjNetPktBase
{
	int  iWorldID;					// 世界编号
	char szAccountName[32];			// 帐号名称
	
	PGPlayerStatus_CtoS_AccountInfo()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_AccountInfo;
	};
};

// GMS->GMC:回传取得帐号资讯结果----------------------------------------------------------------------
struct PGPlayerStatus_StoC_AccountInfo : public S_ObjNetPktBase
{
	ENUM_AccountInfoResult emResult;			// 结果列举
	int	 iWorldID;								// 世界编号
	int	 iAccountID;							// 帐号ID
	char szAccountName[32];						// 帐号名称
	int  iMaxRoleCount;							// 可建立角色数
	S_GMAccountRoleInfo sGMAccountRoleInfo[8];	// 角色列表
	
	PGPlayerStatus_StoC_AccountInfo()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_AccountInfo;
	};
};

// GMC->GMS:要求取得角色进阶资讯----------------------------------------------------------------------
struct PGPlayerStatus_CtoS_CharacterAdvanceInfo : public S_ObjNetPktBase
{
	int  iWorldID;					// 世界编号
	char szRoleName[32];			// 角色名称
	
	PGPlayerStatus_CtoS_CharacterAdvanceInfo()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_CharacterAdvanceInfo;
	};
};

// GMS->GMC 回传取得角色进阶资讯(熟练,知识,声望)结果--------------------------------------------------
struct PGPlayerStatus_StoC_CharacterAdvanceInfo : public S_ObjNetPktBase
{
	ENUM_CharacterAdvanceInfoResult emResult;	// 结果列举
	int	 iWorldID;								// 世界编号,-1代表结束封包
	char szAccount[32];							// 帐号
	char szRoleName[32];						// 角色名称
	int	 iPlotFame[20];							// 进阶资讯阵列
	
	PGPlayerStatus_StoC_CharacterAdvanceInfo()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_CharacterAdvanceInfo;
	};
};

// GMC->GMS:要求取得角色黑名单列表--------------------------------------------------------------------
struct PGPlayerStatus_CtoS_CharacterBlackList : public S_ObjNetPktBase
{
	int	 iWorldID;			// 世界编号
	char szRoleName[32];	// 角色名称
	
	PGPlayerStatus_CtoS_CharacterBlackList()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_CharacterBlackList;
	};
};

// GMS->GMC:回传取得角色黑名单列表结果----------------------------------------------------------------
struct PGPlayerStatus_StoC_CharacterBlackList : public S_ObjNetPktBase
{
	ENUM_CharacterBlackListResult emResult;		// 结果列举
	int	 iWorldID;								// 世界编号,-1代表结束封包
	char szAccount[32];							// 帐号
	char szRoleName[32];						// 角色名称
	char szBlackList[15][32];					// 黑名单列表
	
	PGPlayerStatus_StoC_CharacterBlackList()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_CharacterBlackList;
	};
};

// GMC->GMS:要求取得角色伏魔资讯----------------------------------------------------------------------
struct PGPlayerStatus_CtoS_CharacterMob : public S_ObjNetPktBase
{
	int  iWorldID;					// 世界编号
	char szRoleName[32];			// 角色名称
	
	PGPlayerStatus_CtoS_CharacterMob()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_CharacterMob;
	};
};

// GMS->GMC 回传取得角色伏魔资讯(熟练,知识,声望)结果--------------------------------------------------
struct PGPlayerStatus_StoC_CharacterMob : public S_ObjNetPktBase
{
	ENUM_CharacterMobResult emResult;	// 结果列举
	int	 iWorldID;						// 世界编号,-1代表结束封包
	char szAccount[32];					// 帐号
	char szRoleName[32];				// 角色名称
	int  dwMobFlag[32];					// 伏魔旗标
	
	PGPlayerStatus_StoC_CharacterMob()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_CharacterMob;
	};
};
// GMC->GMS:要求取得角色图鉴资讯----------------------------------------------------------------------
struct PGPlayerStatus_CtoS_CharacterPuzzleMap : public S_ObjNetPktBase
{
	int  iWorldID;					// 世界编号
	char szRoleName[32];			// 角色名称
	
	PGPlayerStatus_CtoS_CharacterPuzzleMap()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_CharacterPuzzleMap;
	};
};

// GMS->GMC 回传取得角色图鉴资讯(熟练,知识,声望)结果--------------------------------------------------
struct PGPlayerStatus_StoC_CharacterPuzzleMap : public S_ObjNetPktBase
{
	ENUM_CharacterPuzzleMapResult emResult;	// 结果列举
	int	 iWorldID;							// 世界编号,-1代表结束封包
	char szAccount[32];						// 帐号
	char szRoleName[32];					// 角色名称
	int  dwPuzzleMapFlag[64];				// 图鉴旗标
	
	PGPlayerStatus_StoC_CharacterPuzzleMap()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_CharacterPuzzleMap;
	};
};
// GMC->GMS:要求取得角色技能资讯----------------------------------------------------------------------
struct PGPlayerStatus_CtoS_CharacterSkill : public S_ObjNetPktBase
{
	int  iWorldID;					// 世界编号
	char szRoleName[32];			// 角色名称
	
	PGPlayerStatus_CtoS_CharacterSkill()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_CharacterSkill;
	};
};

// GMS->GMC 回传取得角色技能资讯(熟练,知识,声望)结果--------------------------------------------------
struct PGPlayerStatus_StoC_CharacterSkill : public S_ObjNetPktBase
{
	ENUM_CharacterSkillResult emResult;		// 结果列举
	int	 iWorldID;							// 世界编号,-1代表结束封包
	char szAccount[32];						// 帐号
	char szRoleName[32];					// 角色名称
	int  iVocType;							// 职业代号(0剑,1刀,2舞,3灵,4盗,5气)
	int	 iSkillPoint;						// 可配置技能点数
	int  iVocSkillID[100];					// 法术编号
	S_GMLifeHoodSkill sLifeHoodSkill;		// 生产技能资讯
	
	PGPlayerStatus_StoC_CharacterSkill()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_CharacterSkill;
	};
};
// GMC->GMS:要求删除角色伏魔--------------------------------------------------------------------------
struct PGPlayerControl_CtoS_DelCharacterMob  : public S_ObjNetPktBase
{
	int	 iWorldID;			// 世界编号
	char szRoleName[32];	// 角色名称
	int  iMobID;			// 怪物编号
	int  iFlagID;			// 旗标编号
	
	PGPlayerControl_CtoS_DelCharacterMob()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_DelCharacterMob;
	};
};

// GMS->GMC:回传删除角色伏魔结果----------------------------------------------------------------------
struct PGPlayerControl_StoC_DelCharacterMob : public S_ObjNetPktBase
{
	ENUM_DelCharacterMobResult emResult;	// 结果列举
	int	 iWorldID;							// 世界编号
	char szRoleName[32];					// 角色名称
	int  iMobID;							// 怪物编号
	
	PGPlayerControl_StoC_DelCharacterMob()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_DelCharacterMob;
	};
};
// GMC->GMS:要求修改角色图鉴--------------------------------------------------------------------------
struct PGPlayerControl_CtoS_ModifyPuzzleMap  : public S_ObjNetPktBase
{
	int	 iWorldID;							// 世界编号
	char szRoleName[32];					// 角色名称
	S_GMPuzzleMapProc sGMPuzzleMapProc;		// 图鉴操作资讯
	
	PGPlayerControl_CtoS_ModifyPuzzleMap()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_ModifyPuzzleMap;
	};
};

// GMS->GMC:回传修改角色图鉴结果----------------------------------------------------------------------
struct PGPlayerControl_StoC_ModifyPuzzleMap : public S_ObjNetPktBase
{
	ENUM_ModifyPuzzleMapResult emResult;	// 结果列举
	int	 iWorldID;							// 世界编号
	char szRoleName[32];					// 角色名称
	S_GMPuzzleMapProc sGMPuzzleMapProc;		// 图鉴操作资讯
	
	PGPlayerControl_StoC_ModifyPuzzleMap()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_ModifyPuzzleMap;
	};
};

// GMC->GMS:要求重新载入功能开关列表------------------------------------------------------------------
struct PGServerControl_CtoS_ReloadFunctionSwitch  : public S_ObjNetPktBase
{
	int iWorldID;		// 世界编号
	
	PGServerControl_CtoS_ReloadFunctionSwitch()
	{
		uiCmdID = ENUM_PGServerControl_CtoS_ReloadFunctionSwitch;
	};
};

// GMS->GMC:回传重新载入功能开关列表结果--------------------------------------------------------------
struct PGServerControl_StoC_ReloadFunctionSwitch : public S_ObjNetPktBase
{
	ENUM_ReloadFunctionSwitchResult emResult;	// 结果列举
	int iWorldID;								// 世界编号
	
	PGServerControl_StoC_ReloadFunctionSwitch()
	{
		uiCmdID = ENUM_PGServerControl_StoC_ReloadFunctionSwitch;
	};
};


// GMC->GMS:要求移动角色至安全点----------------------------------------------------------------------
struct PGPlayerControl_CtoS_MoveCharacterToSafe  : public S_ObjNetPktBase
{
	int  iWorldID;			// 世界编号
	char szRoleName[32];	// 角色名称
	int	 iZoneID;			// 区域编号
	int  X, Y, Z;			// 所在位置
	
	PGPlayerControl_CtoS_MoveCharacterToSafe()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_MoveCharacterToSafe;
	};
};

// GMS->GMC:回传移动角色至安全点结果------------------------------------------------------------------
struct PGPlayerControl_StoC_MoveCharacterToSafe : public S_ObjNetPktBase
{
	ENUM_MoveCharacterToSafeResult emResult;	// 结果列举
	int  iWorldID;								// 世界编号
	char szRoleName[32];						// 角色名称
	int	 iZoneID;								// 区域编号
	
	PGPlayerControl_StoC_MoveCharacterToSafe()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_MoveCharacterToSafe;
	};
};

struct S_ROLENAME 
{
	int rolenameid;
	char rolename[256];
	S_ROLENAME()
	{
		rolenameid=0;
		ZeroMemory(rolename,256);
	}
};

// add by ltx
// GMC->GMS:要求寄送客制物品邮件--------------------------------------------------------------------------
struct PGServerControl_CtoS_SendCustomItemMail  : public S_ObjNetPktBase
{
	int  iWorldID;			// 世界编号
	char szRoleName[32];	// 角色名称
	char szSubject[32];		// 邮件主旨
	char szContent[256];	// 邮件内文
	int  iMoney;			// 邮件金钱
	int  iCustomItemID;		// 客制物品编号
	bool bBindSetup;		// 绑定状态(1:设为绑定 0:维持原样版设定)
	int  iInherit[10];		// 玄石参数,各索引代表的意义如下(无使用请填-1):
	// 0:基本值(字首) 1:附魔, 2:冲物理, 3:冲法术, 4~9:玄石1~6
	char szReason[1024];	// 发送奖品原因
	
	PGServerControl_CtoS_SendCustomItemMail()
	{
		uiCmdID = ENUM_PGServerControl_CtoS_SendCustomItemMail;
	};
};

// GMS->GMC:回传寄送客制物品邮件结果----------------------------------------------------------------------
struct PGServerControl_StoC_SendCustomItemMail : public S_ObjNetPktBase
{
	ENUM_SendCustomItemMailResult emResult;	// 结果列举
	int  iWorldID;							// 世界编号
	char szRoleName[32];					// 角色名称
	char szSubject[32];						// 邮件主旨
	int  iMoney;							// 邮件金钱
	int  iCustomItemID;						// 客制物品编号
	bool bBindSetup;						// 绑定状态(1:设为绑定 0:维持原样版设定)
	int  iInherit[10];						// 玄石参数
	
	PGServerControl_StoC_SendCustomItemMail()
	{
		uiCmdID = ENUM_PGServerControl_StoC_SendCustomItemMail;
	};
};

