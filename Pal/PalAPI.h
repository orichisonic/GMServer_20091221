// PalAPI.h: interface for the CPalAPI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PALAPI_H__3B270351_F11B_4F40_8BC3_B464590B786B__INCLUDED_)
#define AFX_PALAPI_H__3B270351_F11B_4F40_8BC3_B464590B786B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "CharacterInfo.h"
#include "BanishPlayer.h"
#include "CharacterBag.h"
#include "CharacterFriendList.h"
#include "CharacterQuest.h"
#include "DelCharacterItem.h"
#include "DismissGuild.h"
#include "GuildBaseInfo.h"
#include "GuildMemberList.h"
#include "ModifyAccountBankPw.h"
#include "ModifyAccountPw.h"
#include "ModifyCharacterDropPw.h"
#include "ModifyCharacterMoney.h"
#include "ModifyCharacterQuest.h"
#include "ModifyGuildLv.h"
#include "ModifyGuildMemberPL.h"
#include "ModifyGuildName.h"
#include "ModifyGuildNotice.h"
#include "ModifySafeLock.h"
#include "ModifySecLvlPwd.h"
#include "SendGiftMail.h"
#include "CharacterAdvanceInfo.h"
#include "CharacterSkill.h"
#include "CharacterMob.h"
#include "CharacterPuzzleMap.h"
#include "CharacterBlackList.h"
#include "DelCharacterMob.h"
#include "ModifyPuzzleMap.h"
#include "AccountInfo.h"
#include "MoveCharacterToSafe.h"
#include "ReloadFunctionSwitch.h"
#include "OperPal.h"
// add by ltx
#include "SendCustomItemMail.h"

#include "..\\GameIOCP\\GameInfo.h"

//#include "PalManagerModule.h"
//maple add
struct BanPlayerPACKET
{
	char szAccount[128];
	char szRoleName[128];

	BanPlayerPACKET()
	{
		ZeroMemory(szAccount,128);
		ZeroMemory(szRoleName,128);
	}
};

class CPalAPI  : public CGameInfo
{
public:
	CPalAPI();
	virtual ~CPalAPI();
private:
	
	//用于保存在远程数据库执行操作的sql语句
	char operRemoteSql[2048];
	

	//记录日志信息
	char strLog[2048];
	
	//根据服务器名称获取需要的IP
	char logdbip[20];

	
private:
	//操作数据库
	COperPal operPal;

	
public:

	///////////功能接口////////////////
	//用户信息查询
	bool UserInfo(char * ServerName, char * ServerIP,char * UserName, char * UserNick,int iIndex,int iPageSize);

	//根据帐号踢人
	bool Pal_KickPlayer(int userByID,char * ServerName, char * ServerIP,char * UserName);

	//根据帐号封停
	bool Pal_ClosePlayer(int userByID,char * ServerName, char * ServerIP ,char * UserName,char * UserNick, char* Reason);

	//根据帐号解封
	bool Pal_OpenPlayer(int userByID,char * ServerName, char * ServerIP ,char * UserName,char * UserNick, char* Reason);

	//查询封停列表
	bool CloseList(char * ServerName, char * ServerIP,int iIndex, int iPageSize);

	//查询单个账号
	bool CloseSingle(char * ServerName, char * ServerIP,char * UserName,int iIndex, int iPageSize);
	
	//查询公告
	bool BroadTask_Query(int iIndex,int iPageSize);
	
	//更新公告
	bool BroadTask_Update(int userByID,int taskid,char * boardMessage,char * begintime,char * endtime,int interval,int status);

	//插入公告
	bool BroadTask_Insert(int userByID,char * ServerIP,char * GSServerIP,char * boardMessage,char * begintime,char * endtime,int interval);
	
	
	//查询玩家背包
	void Pal_CharacterBag(char * ServerName, char * ServerIP, char * RoleName, int ibagtype,int iIndex,int iPageSize);

	//取得角色好友列表
	void Pal_CharacterFriendList(char * ServerName, char * ServerIP, char * RoleName,int iIndex,int iPageSize);
	
	//取得角色任务资讯
	void Pal_CharacterQuest(char * ServerName, char * ServerIP,char * RoleName,int iType,int iIndex,int iPageSize);

	//取得公会基本资讯
	void Pal_GuileBaseInfo(char * ServerName, char * ServerIP,char * GuildName,int iIndex, int iPageSize);
	
	//取得公会成员列表
	void Pal_GuildMemberList(char * ServerName, char * ServerIP,char * GuildName,int iIndex, int iPageSize);

	//寄送奖品邮件
	void Pal_SendGiftMail(int UserByID,
		char * ServerName,
		char * ServerIP,
		char * RoleName,
		char * Subject,
		char * Content,
		int iMoney,
		int Gift0,
		int Gift1,
		int Gift2,
		int Gift3,
		int Gift4,
		char * reason);

	//解散公会
	void Pal_DismissGuild(int userByID,char * ServerName, char * ServerIP,char * GuildName,char * Reason);
	
	//修改公会名称
	void Pal_ModifyGuildName(int userByID,char * ServerName, char * ServerIP,char * GuildName,char * NewName,char * Reason);
	
	//修改公会成员职阶
	void Pal_ModifyGuildMemberPL(int userByID,char * ServerName, char * ServerIP,char * GuildName,char * RoleName, int iLevel,char * Reason);
	
	//修改角色任务	
	void Pal_ModifyCharacterQuest(int userByID,char * ServerName, char * ServerIP,char* szRoleName,int iQuestID,int iQuestStatus);
	
	//修改角色金钱
	void Pal_ModifyCharacterMoney(int userByID,char * ServerName, char * ServerIP, char * szRoleName,int iType,int iCount);

	//修改角色删除密码
	void Pal_ModifyCharacterDropPw(int userByID,char * ServerName, char * ServerIP, char * szRoleName,char * szDropRolePw);

	//还原角色删除密码
	void Pal_RecallCharacterDropPw(int userByID,char * ServerName, char * ServerIP, char * szRoleName);

	//修改帐号银行密码	
	void Pal_ModifyAccountBankPw(int userByID,char * ServerName, char * ServerIP, char * szAccount,char * szBankPw);
		
	//还原帐号银行密码	
	void Pal_RecallAccountBankPw(int userByID,char * ServerName, char * ServerIP, char * szAccount);
	
	//修改帐号密码
	void Pal_ModifyAccountPw(int userByID,char * ServerName, char * ServerIP, char * szAccount,char * szAccountPw);
	
	//还原帐号密码	
	void Pal_RecallAccountPw(int userByID,char * ServerName, char * ServerIP, char * szAccount);

	//修改二级密码
	void Pal_ModifySecondPw(int userByID,char * ServerName,char * ServerIP,char * szAccount,char * szSePw);
	
	//还原二级密码
	void Pal_RecallSecondPw(int userByID,char * ServerName,char * ServerIP,char * szAccount);


	//锁定安全锁
	void Pal_UseSafeLock(int userByID,char * ServerName,char * ServerIP,char * szAccount);
	
	//解锁密码锁
	void Pal_FreeSafeLock(int userByID,char * ServerName,char * ServerIP,char * szAccount);


	// 删除角色物品响应
	void Pal_DelCharacterItem(int UserByID,char * ServerName,char * ServerIP,char * RoleName,int iType,int iPosition,char * ItemName);
	
	//修改公会等级
	void Pal_ModifyGuildLv(int userByID,char * ServerName,char * ServerIP, char * GuildName,int iLevel,char * Reason);
	
	//修改公会公告
	void Pal_ModifyGuildNotice(int userByID,char * ServerName, char * ServerIP, char * GuildName,char * szNotice,char * Reason);

		
	//道具类别查询
	void Pal_ItemType_Query();
	
	//道具名称查询
	void Pal_ItemName_Query(int iItemBigType);

	//发送仓库密码
	void PAL_SendBankPwd_Query(int userByID,char * ServerName, char * ServerIP,char * szAccount,char * mailBox);
	
	//发送角色删除密码
	void PAL_SendDeletePwd_Query(int userByID,char * ServerName, char * ServerIP, char * szRoleName,char * mailBox);

	//获取用户地址
	bool Pal_Get_EmailAddress(char * szAccount);

	//取得角色进阶资讯
	bool Pal_CharacterAdvanceInfo(char * ServerName, char * ServerIP,char * RoleName,int iIndex,int iPageSize);
	
	//取得角色技能资讯
	bool Pal_CharacterSkill(char * ServerName, char * ServerIP,char * RoleName,int iIndex,int iPageSize);

	//取得角色伏魔资讯
	bool Pal_CharacterMob(char * ServerName, char * ServerIP,char * RoleName,int iIndex,int iPageSize);

	//取得角色图鉴资讯
	bool Pal_CharacterPuzzleMap(char * ServerName, char * ServerIP,char * RoleName,int iIndex,int iPageSize);

	//取得角色黑名单列表
	bool Pal_CharacterBlackList(char * ServerName, char * ServerIP, char * RoleName,int iIndex,int iPageSize);

	//删除角色伏魔
	bool Pal_DelCharacterMob(int userByID,char * ServerName, char * ServerIP,char * RoleName,int iMobID);
	
	//修改角色图鉴
	bool Pal_ModifyPuzzleMap(int userByID,char * ServerName,char * ServerIP,char * RoleName,char *strPuzzleMap,
		int iMapID1,int iMapID2,int iMapID3,int iMapID4,int iMapID5,int iMapID6,int iMapID7,int iMapID8,int iMapID9);

	//取得帐号资讯
	bool Pal_AccountInfo(char * ServerName, char * ServerIP, char * szAccountName, int iIndex, int iPageSize);

	//移动角色至安全点
	bool Pal_MoveCharacterToSafe(int userByID,char * ServerName, char * ServerIP, char * RoleName, int iZoneID);

	//查询地图
	bool PAL_SearchMap_Query();

	//重新载入功能开关
	bool Pal_ReloadFunctionSwitch();

	void Pal_BanishPlayer(char * pal_serverip,char * pal_gmserveripname,char * pal_userNick);

	//maple add
	//批量封停玩家帐号
	bool Pal_BanMultiPlayer(int userByID,char * ServerName, char * ServerIP ,char * UserNameList,char * RoleNameList, char* Reason);
	
	//add by ltx

	//寄送客制物品邮件
	void Pal_SendCustomItemMail(int UserByID, char * szGMServerName,char * szGMServerIP,char * szRoleName,char * szSubject,
						char * szContent,int iMoney, int itemType, int iCustomItemID, char *szCustomItemIDName,bool bBindSetup, char *szInherit, char *szReason);

	//Modified by tengjie 2010-07-08
// 	void Pal_ModifyAccountPwAll(int userByID, char * szAccount, char * szAccountPw);
// 	
// 	void Pal_RecallAccountPwAll(int userByID, char * szAccount);
	void Pal_ModifyAccountPwAll(int userByID, char * szAccount, char * szAccountPw, char *ServerName);
	
	void Pal_RecallAccountPwAll(int userByID, char * szAccount, char *ServerName);
};

#endif // !defined(AFX_PALAPI_H__3B270351_F11B_4F40_8BC3_B464590B786B__INCLUDED_)
