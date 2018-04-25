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
	
	//���ڱ�����Զ�����ݿ�ִ�в�����sql���
	char operRemoteSql[2048];
	

	//��¼��־��Ϣ
	char strLog[2048];
	
	//���ݷ��������ƻ�ȡ��Ҫ��IP
	char logdbip[20];

	
private:
	//�������ݿ�
	COperPal operPal;

	
public:

	///////////���ܽӿ�////////////////
	//�û���Ϣ��ѯ
	bool UserInfo(char * ServerName, char * ServerIP,char * UserName, char * UserNick,int iIndex,int iPageSize);

	//�����ʺ�����
	bool Pal_KickPlayer(int userByID,char * ServerName, char * ServerIP,char * UserName);

	//�����ʺŷ�ͣ
	bool Pal_ClosePlayer(int userByID,char * ServerName, char * ServerIP ,char * UserName,char * UserNick, char* Reason);

	//�����ʺŽ��
	bool Pal_OpenPlayer(int userByID,char * ServerName, char * ServerIP ,char * UserName,char * UserNick, char* Reason);

	//��ѯ��ͣ�б�
	bool CloseList(char * ServerName, char * ServerIP,int iIndex, int iPageSize);

	//��ѯ�����˺�
	bool CloseSingle(char * ServerName, char * ServerIP,char * UserName,int iIndex, int iPageSize);
	
	//��ѯ����
	bool BroadTask_Query(int iIndex,int iPageSize);
	
	//���¹���
	bool BroadTask_Update(int userByID,int taskid,char * boardMessage,char * begintime,char * endtime,int interval,int status);

	//���빫��
	bool BroadTask_Insert(int userByID,char * ServerIP,char * GSServerIP,char * boardMessage,char * begintime,char * endtime,int interval);
	
	
	//��ѯ��ұ���
	void Pal_CharacterBag(char * ServerName, char * ServerIP, char * RoleName, int ibagtype,int iIndex,int iPageSize);

	//ȡ�ý�ɫ�����б�
	void Pal_CharacterFriendList(char * ServerName, char * ServerIP, char * RoleName,int iIndex,int iPageSize);
	
	//ȡ�ý�ɫ������Ѷ
	void Pal_CharacterQuest(char * ServerName, char * ServerIP,char * RoleName,int iType,int iIndex,int iPageSize);

	//ȡ�ù��������Ѷ
	void Pal_GuileBaseInfo(char * ServerName, char * ServerIP,char * GuildName,int iIndex, int iPageSize);
	
	//ȡ�ù����Ա�б�
	void Pal_GuildMemberList(char * ServerName, char * ServerIP,char * GuildName,int iIndex, int iPageSize);

	//���ͽ�Ʒ�ʼ�
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

	//��ɢ����
	void Pal_DismissGuild(int userByID,char * ServerName, char * ServerIP,char * GuildName,char * Reason);
	
	//�޸Ĺ�������
	void Pal_ModifyGuildName(int userByID,char * ServerName, char * ServerIP,char * GuildName,char * NewName,char * Reason);
	
	//�޸Ĺ����Աְ��
	void Pal_ModifyGuildMemberPL(int userByID,char * ServerName, char * ServerIP,char * GuildName,char * RoleName, int iLevel,char * Reason);
	
	//�޸Ľ�ɫ����	
	void Pal_ModifyCharacterQuest(int userByID,char * ServerName, char * ServerIP,char* szRoleName,int iQuestID,int iQuestStatus);
	
	//�޸Ľ�ɫ��Ǯ
	void Pal_ModifyCharacterMoney(int userByID,char * ServerName, char * ServerIP, char * szRoleName,int iType,int iCount);

	//�޸Ľ�ɫɾ������
	void Pal_ModifyCharacterDropPw(int userByID,char * ServerName, char * ServerIP, char * szRoleName,char * szDropRolePw);

	//��ԭ��ɫɾ������
	void Pal_RecallCharacterDropPw(int userByID,char * ServerName, char * ServerIP, char * szRoleName);

	//�޸��ʺ���������	
	void Pal_ModifyAccountBankPw(int userByID,char * ServerName, char * ServerIP, char * szAccount,char * szBankPw);
		
	//��ԭ�ʺ���������	
	void Pal_RecallAccountBankPw(int userByID,char * ServerName, char * ServerIP, char * szAccount);
	
	//�޸��ʺ�����
	void Pal_ModifyAccountPw(int userByID,char * ServerName, char * ServerIP, char * szAccount,char * szAccountPw);
	
	//��ԭ�ʺ�����	
	void Pal_RecallAccountPw(int userByID,char * ServerName, char * ServerIP, char * szAccount);

	//�޸Ķ�������
	void Pal_ModifySecondPw(int userByID,char * ServerName,char * ServerIP,char * szAccount,char * szSePw);
	
	//��ԭ��������
	void Pal_RecallSecondPw(int userByID,char * ServerName,char * ServerIP,char * szAccount);


	//������ȫ��
	void Pal_UseSafeLock(int userByID,char * ServerName,char * ServerIP,char * szAccount);
	
	//����������
	void Pal_FreeSafeLock(int userByID,char * ServerName,char * ServerIP,char * szAccount);


	// ɾ����ɫ��Ʒ��Ӧ
	void Pal_DelCharacterItem(int UserByID,char * ServerName,char * ServerIP,char * RoleName,int iType,int iPosition,char * ItemName);
	
	//�޸Ĺ���ȼ�
	void Pal_ModifyGuildLv(int userByID,char * ServerName,char * ServerIP, char * GuildName,int iLevel,char * Reason);
	
	//�޸Ĺ��ṫ��
	void Pal_ModifyGuildNotice(int userByID,char * ServerName, char * ServerIP, char * GuildName,char * szNotice,char * Reason);

		
	//��������ѯ
	void Pal_ItemType_Query();
	
	//�������Ʋ�ѯ
	void Pal_ItemName_Query(int iItemBigType);

	//���Ͳֿ�����
	void PAL_SendBankPwd_Query(int userByID,char * ServerName, char * ServerIP,char * szAccount,char * mailBox);
	
	//���ͽ�ɫɾ������
	void PAL_SendDeletePwd_Query(int userByID,char * ServerName, char * ServerIP, char * szRoleName,char * mailBox);

	//��ȡ�û���ַ
	bool Pal_Get_EmailAddress(char * szAccount);

	//ȡ�ý�ɫ������Ѷ
	bool Pal_CharacterAdvanceInfo(char * ServerName, char * ServerIP,char * RoleName,int iIndex,int iPageSize);
	
	//ȡ�ý�ɫ������Ѷ
	bool Pal_CharacterSkill(char * ServerName, char * ServerIP,char * RoleName,int iIndex,int iPageSize);

	//ȡ�ý�ɫ��ħ��Ѷ
	bool Pal_CharacterMob(char * ServerName, char * ServerIP,char * RoleName,int iIndex,int iPageSize);

	//ȡ�ý�ɫͼ����Ѷ
	bool Pal_CharacterPuzzleMap(char * ServerName, char * ServerIP,char * RoleName,int iIndex,int iPageSize);

	//ȡ�ý�ɫ�������б�
	bool Pal_CharacterBlackList(char * ServerName, char * ServerIP, char * RoleName,int iIndex,int iPageSize);

	//ɾ����ɫ��ħ
	bool Pal_DelCharacterMob(int userByID,char * ServerName, char * ServerIP,char * RoleName,int iMobID);
	
	//�޸Ľ�ɫͼ��
	bool Pal_ModifyPuzzleMap(int userByID,char * ServerName,char * ServerIP,char * RoleName,char *strPuzzleMap,
		int iMapID1,int iMapID2,int iMapID3,int iMapID4,int iMapID5,int iMapID6,int iMapID7,int iMapID8,int iMapID9);

	//ȡ���ʺ���Ѷ
	bool Pal_AccountInfo(char * ServerName, char * ServerIP, char * szAccountName, int iIndex, int iPageSize);

	//�ƶ���ɫ����ȫ��
	bool Pal_MoveCharacterToSafe(int userByID,char * ServerName, char * ServerIP, char * RoleName, int iZoneID);

	//��ѯ��ͼ
	bool PAL_SearchMap_Query();

	//�������빦�ܿ���
	bool Pal_ReloadFunctionSwitch();

	void Pal_BanishPlayer(char * pal_serverip,char * pal_gmserveripname,char * pal_userNick);

	//maple add
	//������ͣ����ʺ�
	bool Pal_BanMultiPlayer(int userByID,char * ServerName, char * ServerIP ,char * UserNameList,char * RoleNameList, char* Reason);
	
	//add by ltx

	//���Ϳ�����Ʒ�ʼ�
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
