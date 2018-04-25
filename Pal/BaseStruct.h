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

// ������-------------------------------------------------------------------------------------------
enum ENUM_GMPacket
{	
	ENUM_PGGMCConnection_CtoS_RequestLogin		= 0xe001,	// GMClient -> GMServer Ҫ�����
	ENUM_PGGMCConnection_StoC_LoginResult				,	// GMServer -> GMClient �ش�������

	ENUM_PGServerControl_CtoS_SendNonscheduleBulletin = 0xe300,	// GMC -> GMS Ҫ���ͷǹ̶�����
	ENUM_PGServerControl_StoC_SendNonscheduleBulletin = 0xe302,	// GMS -> GMC �ش����ͷǹ̶�������

	ENUM_PGServerControl_CtoS_AddScheduleBulletin	= 0xe310,	// GMC -> GMS Ҫ�������ų̹���
	ENUM_PGServerControl_StoC_AddScheduleBulletin	= 0xe313,	// GMS -> GMC �ش������ų̹�����

	ENUM_PGServerControl_CtoS_DelScheduleBulletin	= 0xe320,	// GMC -> GMS Ҫ��ɾ���ų̹���
	ENUM_PGServerControl_StoC_DelScheduleBulletin	= 0xe323,	// GMS -> GMC �ش�ɾ���ų̹�����

	ENUM_PGServerControl_CtoS_ReloadScheduleBulletin= 0xe330,	// GMC -> GMS Ҫ�����������ų̹���
	ENUM_PGServerControl_StoC_ReloadScheduleBulletin= 0xe333,	// GMS -> GMC �ش����������ų̹�����

	ENUM_PGServerControl_CtoS_SendGiftMail			= 0xe340,	// GMC -> GMS Ҫ����ͽ�Ʒ�ʼ�
	ENUM_PGServerControl_StoC_SendGiftMail			= 0xe343,	// GMS -> GMC �ش����ͽ�Ʒ�ʼ����

	ENUM_PGServerControl_CtoS_ReloadFunctionSwitch	= 0xe350,	// GMC -> GMS Ҫ���������빦�ܿ����б�
	ENUM_PGServerControl_StoC_ReloadFunctionSwitch	= 0xe353,	// GMS -> GMC �ش��������빦�ܿ����б���

	// add by ltx
	ENUM_PGServerControl_CtoS_SendCustomItemMail	= 0xe360,	// GMC -> GMS Ҫ����Ϳ�����Ʒ�ʼ�
	ENUM_PGServerControl_StoC_SendCustomItemMail	= 0xe363,	// GMS -> GMC �ش����Ϳ�����Ʒ�ʼ����

	ENUM_PGServerStatus_CtoS_PlayerOnlineCount	= 0xe500,	// GMC->GMS Ҫ����������
	ENUM_PGServerStatus_StoC_PlayerOnlineCount	= 0xe503,	// GMS->GMC �ش������������

	ENUM_PGServerStatus_CtoS_ScheduleBulletinList	= 0xe510,	// GMC -> GMS Ҫ���ų̹����б�
	ENUM_PGServerStatus_StoC_ScheduleBulletinList	= 0xe513,	// GMS -> GMC �ش��ų̹����б�

	ENUM_PGPlayerControl_CtoS_BanishPlayer		= 0xe600,	// GMC->GMS Ҫ����ĳ�������
	ENUM_PGPlayerControl_StoC_BanishPlayer		= 0xe603,	// GMS->GMC �ش���ĳ������߽��

	ENUM_PGPlayerControl_CtoS_ModifyGuildMemberPL	= 0xe610,	// GMC -> GMS Ҫ���޸Ĺ����Աְ��
	ENUM_PGPlayerControl_StoC_ModifyGuildMemberPL	= 0xe613,	// GMS -> GMC �ش��޸Ĺ����Աְ�׽��

	ENUM_PGPlayerControl_CtoS_DismissGuild			= 0xe620,	// GMC -> GMS Ҫ���ɢ����
	ENUM_PGPlayerControl_StoC_DismissGuild			= 0xe623,	// GMS -> GMC �ش���ɢ������
	
	ENUM_PGPlayerControl_CtoS_ModifyGuildName		= 0xe630,	// GMC -> GMS Ҫ���޸Ĺ�������
	ENUM_PGPlayerControl_StoC_ModifyGuildName		= 0xe633,	// GMS -> GMC �ش��޸Ĺ������ƽ��

	ENUM_PGPlayerControl_CtoS_ModifyCharacterQuest	= 0xe640,	// GMC -> GMS Ҫ���޸Ľ�ɫ����
	ENUM_PGPlayerControl_StoC_ModifyCharacterQuest	= 0xe643,	// GMS -> GMC �ش��޸Ľ�ɫ������

	ENUM_PGPlayerControl_CtoS_DelCharacterItem		= 0xe650,	// GMC -> GMS Ҫ��ɾ����ɫ��Ʒ
	ENUM_PGPlayerControl_StoC_DelCharacterItem		= 0xe653,	// GMS -> GMC �ش�ɾ����ɫ��Ʒ���

	ENUM_PGPlayerControl_CtoS_ModifyCharacterMoney	= 0xe660,	// GMC -> GMS Ҫ���޸Ľ�ɫ��Ǯ
	ENUM_PGPlayerControl_StoC_ModifyCharacterMoney	= 0xe663,	// GMS -> GMC �ش��޸Ľ�ɫ��Ǯ���

	ENUM_PGPlayerControl_CtoS_ModifyGuildLv			= 0xe670,	// GMC -> GMS Ҫ���޸Ĺ���ȼ�
	ENUM_PGPlayerControl_StoC_ModifyGuildLv			= 0xe673,	// GMS -> GMC �ش��޸Ĺ���ȼ����

	ENUM_PGPlayerControl_CtoS_ModifyGuildNotice		= 0xe680,	// GMC -> GMS Ҫ���޸Ĺ��ṫ��
	ENUM_PGPlayerControl_StoC_ModifyGuildNotice		= 0xe683,	// GMS -> GMC �ش��޸Ĺ��ṫ����

	ENUM_PGPlayerControl_CtoS_ModifyCharacterDropPw	= 0xe690,	// GMC -> GMS Ҫ���޸Ľ�ɫɾ������
	ENUM_PGPlayerControl_StoC_ModifyCharacterDropPw	= 0xe693,	// GMS -> GMC �ش��޸Ľ�ɫɾ��������

	ENUM_PGPlayerControl_CtoS_ModifyAccountBankPw	= 0xe700,	// GMC -> GMS Ҫ���޸Ľ�ɫ��������
	ENUM_PGPlayerControl_StoC_ModifyAccountBankPw	= 0xe703,	// GMS -> GMC �ش��޸Ľ�ɫ����������

	ENUM_PGPlayerControl_CtoS_DelCharacterMob		= 0xe710,	// GMC -> GMS Ҫ��ɾ����ɫ��ħ
	ENUM_PGPlayerControl_StoC_DelCharacterMob		= 0xe713,	// GMS -> GMC �ش�ɾ����ɫ��ħ���

	ENUM_PGPlayerControl_CtoS_ModifyPuzzleMap		= 0xe720,	// GMC -> GMS Ҫ���޸Ľ�ɫͼ��
	ENUM_PGPlayerControl_StoC_ModifyPuzzleMap		= 0xe723,	// GMS -> GMC �ش��޸Ľ�ɫͼ�����

	ENUM_PGPlayerControl_CtoS_MoveCharacterToSafe	= 0xe730,	// GMC->GMS Ҫ���ƶ���ɫ����ȫ��
	ENUM_PGPlayerControl_StoC_MoveCharacterToSafe	= 0xe733,	// GMS->GMC �ش��ƶ���ɫ����ȫ����

	ENUM_PGPlayerStatus_CtoS_CharacterList		= 0xe800,	// GMC->GMS Ҫ���ɫ�б�
	ENUM_PGPlayerStatus_StoC_CharacterList		= 0xe803,	// GMS->GMC �ش���ɫ�б���

	ENUM_PGPlayerStatus_CtoS_CharacterInfo		= 0xe810,	// GMC -> GMS Ҫ���ɫ������Ѷ
	ENUM_PGPlayerStatus_StoC_CharacterInfo		= 0xe813,	// GMS -> GMC �ش���ɫ������Ѷ���

	ENUM_PGPlayerStatus_CtoS_CharacterBag		= 0xe820,	// GMClient -> GMServer Ҫ���ɫ������Ѷ
	ENUM_PGPlayerStatus_StoC_CharacterBag		= 0xe823,	// GMServer -> GMClient �ش���ɫ������Ѷ���

	ENUM_PGPlayerStatus_CtoS_CharacterQuest		= 0xe830,	// GMC -> GMS Ҫ���ɫ������Ѷ
	ENUM_PGPlayerStatus_StoC_CharacterQuest		= 0xe833,	// GMS -> GMC �ش���ɫ������Ѷ���

	ENUM_PGPlayerStatus_CtoS_CharacterFriendList	= 0xe840,	// GMC -> GMS Ҫ���ɫ�����б�
	ENUM_PGPlayerStatus_StoC_CharacterFriendList	= 0xe843,	// GMS -> GMC �ش���ɫ�����б���

	ENUM_PGPlayerStatus_CtoS_GuildBaseInfo			= 0xe850,	// GMC -> GMS Ҫ�󹫻������Ѷ
	ENUM_PGPlayerStatus_StoC_GuildBaseInfo			= 0xe853,	// GMS -> GMC �ش����������Ѷ���

	ENUM_PGPlayerStatus_CtoS_GuildMemberList		= 0xe860,	// GMC -> GMS Ҫ�󹫻��Ա�б�
	ENUM_PGPlayerStatus_StoC_GuildMemberList		= 0xe863,	// GMS -> GMC �ش������Ա�б���

	ENUM_PGPlayerStatus_CtoS_CharacterSkill	= 0xe870,	// GMC -> GMS Ҫ���ɫ������Ѷ
	ENUM_PGPlayerStatus_StoC_CharacterSkill	= 0xe873,	// GMS -> GMC �ش���ɫ������Ѷ���

	ENUM_PGPlayerStatus_CtoS_CharacterMob	= 0xe880,	// GMC -> GMS Ҫ���ɫ��ħ��Ѷ
	ENUM_PGPlayerStatus_StoC_CharacterMob	= 0xe883,	// GMS -> GMC �ش���ɫ��ħ��Ѷ���

	ENUM_PGPlayerStatus_CtoS_CharacterPuzzleMap	= 0xe890,	// GMC -> GMS Ҫ���ɫͼ����Ѷ
	ENUM_PGPlayerStatus_StoC_CharacterPuzzleMap	= 0xe893,	// GMS -> GMC �ش���ɫͼ����Ѷ���

	ENUM_PGPlayerStatus_CtoS_CharacterAdvanceInfo	= 0xe8a0,	// GMC -> GMS Ҫ���ɫ������Ѷ
	ENUM_PGPlayerStatus_StoC_CharacterAdvanceInfo	= 0xe8a3,	// GMS -> GMC

	ENUM_PGPlayerStatus_CtoS_CharacterBlackList	= 0xe8b0,	// GMC -> GMS Ҫ���ɫ�������б�
	ENUM_PGPlayerStatus_StoC_CharacterBlackList	= 0xe8b3,	// GMS -> GMC �ش���ɫ�������б���

	ENUM_PGPlayerStatus_CtoS_AccountInfo		= 0xe8c0,	// GMC -> GMS Ҫ���ɫ������Ѷ
	ENUM_PGPlayerStatus_StoC_AccountInfo		= 0xe8c3,	// GMS -> GMC �ش���ɫ������Ѷ���
	
};

// ������-------------------------------------------------------------------------------------------
enum ENUM_GMCLoginResult
{
	ENUM_GMCLoginResult_Success = 0,	// �ɹ�
	ENUM_GMCLoginResult_Failed,			// ʧ��
	ENUM_GMCLoginResult_AccountFailed,	// �ʺŴ���
	ENUM_GMCLoginResult_PasswordFailed, // �������
	ENUM_GMCLoginResult_IPFailed,		// IP����		
};
// ���ͷǹ̶��������о�-----------------------------------------------------------------------------
enum ENUM_SendNonscheduleBulletinResult
{
	ENUM_SendNonscheduleBulletinResult_NULL = 0, 
	ENUM_SendNonscheduleBulletinResult_Success,				// �ɹ�.
	ENUM_SendNonscheduleBulletinResult_ArgumentError,		// ��������
	ENUM_SendNonscheduleBulletinResult_WorldNotFound,		// �Ҳ�������
	ENUM_SendNonscheduleBulletinResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
	ENUM_SendNonscheduleBulletinResult_LeaderDisconnect,	// ��ͷ�۲���δ����
};
// �����ų̹������о�-------------------------------------------------------------------------------
enum ENUM_AddScheduleBulletinResult
{
	ENUM_AddScheduleBulletinResult_NULL = 0, 
	ENUM_AddScheduleBulletinResult_Success,				// �ɹ�.
	ENUM_AddScheduleBulletinResult_ArgumentError,		// ��������
	ENUM_AddScheduleBulletinResult_WorldNotFound,		// �Ҳ�������
	ENUM_AddScheduleBulletinResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
	ENUM_AddScheduleBulletinResult_LeaderDisconnect,	// ��ͷ�۲���δ����
	ENUM_AddScheduleBulletinResult_AddFailed,			// ����ʧ��,����ID�Ѵ���
};
// ɾ���ų̹������о�-------------------------------------------------------------------------------
enum ENUM_DelScheduleBulletinResult
{
	ENUM_DelScheduleBulletinResult_NULL = 0, 
	ENUM_DelScheduleBulletinResult_Success,				// �ɹ�.
	ENUM_DelScheduleBulletinResult_WorldNotFound,		// �Ҳ�������
	ENUM_DelScheduleBulletinResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
	ENUM_DelScheduleBulletinResult_LeaderDisconnect,	// ��ͷ�۲���δ����
	ENUM_DelScheduleBulletinResult_DelFailed,			// ɾ��ʧ��
};



// ���������ų̹������о�---------------------------------------------------------------------------
enum ENUM_ReloadScheduleBulletinResult
{
	ENUM_ReloadScheduleBulletinResult_NULL = 0, 
	ENUM_ReloadScheduleBulletinResult_Success,				// �ɹ�.
	ENUM_ReloadScheduleBulletinResult_WorldNotFound,		// �Ҳ�������
	ENUM_ReloadScheduleBulletinResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
	ENUM_ReloadScheduleBulletinResult_LeaderDisconnect,		// ��ͷ�۲���δ����
	ENUM_ReloadScheduleBulletinResult_LocalDisconnect,		// �����ŷ���δ����
};
// ���ͽ�Ʒ�ʼ�����о�-------------------------------------------------------------------------------
enum ENUM_SendGiftMailResult
{
	ENUM_SendGiftMailResult_NULL = 0, 
	ENUM_SendGiftMailResult_Success,				// �ɹ�.
	ENUM_SendGiftMailResult_ArgumentError,			// ��������
	ENUM_SendGiftMailResult_WorldNotFound,			// �Ҳ�������
	ENUM_SendGiftMailResult_LeaderNotFound,			// �Ҳ�����ͷ�۲���
	ENUM_SendGiftMailResult_LeaderDisconnect,		// ��ͷ�۲���δ����
	ENUM_SendGiftMailResult_RoleNotFound,			// ��ɫ������
	ENUM_SendGiftMailResult_LocalDisconnect,		// �����ŷ���δ����
	ENUM_SendGiftMailResult_LocalOperationFailed,	// ����ʧ��(Local)
};
// ȡ��������������о�-------------------------------------------------------------------------------
enum ENUM_PlayerOnlineCountResult
{
	ENUM_PlayerOnlineCountResult_NULL = 0, 
	ENUM_PlayerOnlineCountResult_Success,			// �ɹ�.
	ENUM_PlayerOnlineCountResult_WorldNotFound,		// �Ҳ�������
	ENUM_PlayerOnlineCountResult_LeaderNotFound,	// �Ҳ�����ͷ�۲���
	ENUM_PlayerOnlineCountResult_LeaderDisconnect,	// ��ͷ�۲���δ����
};
// ȡ���ų̹����б����о�---------------------------------------------------------------------------
enum ENUM_ScheduleBulletinListResult
{
	ENUM_ScheduleBulletinListResult_NULL = 0, 
	ENUM_ScheduleBulletinListResult_Success,			// �ɹ�.
	ENUM_ScheduleBulletinListResult_WorldNotFound,		// �Ҳ�������
	ENUM_ScheduleBulletinListResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
	ENUM_ScheduleBulletinListResult_LeaderDisconnect,	// ��ͷ�۲���δ����
	ENUM_ScheduleBulletinListResult_BulletinNotFound,	// �б��ǿյ�
};
// ��ĳ������߽���о�-------------------------------------------------------------------------------
enum ENUM_BanishPlayerResult
{
	ENUM_BanishPlayerResult_NULL = 0, 
	ENUM_BanishPlayerResult_Success,			// �ɹ�.
	ENUM_BanishPlayerResult_RoleNotFound,		// ��ɫ������
	ENUM_BanishPlayerResult_RoleNotOnline,		// ��ɫ��������
	ENUM_BanishPlayerResult_WorldNotFound,		// �Ҳ�������
	ENUM_BanishPlayerResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
	ENUM_BanishPlayerResult_LeaderDisconnect,	// ��ͷ�۲���δ����
};
// �޸Ĺ����Աְ�׽���о�---------------------------------------------------------------------------
enum ENUM_ModifyGuildMemberPLResult
{
	ENUM_ModifyGuildMemberPLResult_NULL = 0, 
	ENUM_ModifyGuildMemberPLResult_Success,				// �ɹ�.
	ENUM_ModifyGuildMemberPLResult_WorldNotFound,		// �Ҳ�������
	ENUM_ModifyGuildMemberPLResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
	ENUM_ModifyGuildMemberPLResult_LeaderDisconnect,	// ��ͷ�۲���δ����
	ENUM_ModifyGuildMemberPLResult_LevelError,			// ְ�״���
	ENUM_ModifyGuildMemberPLResult_GuildNotFound,		// ���᲻����
	ENUM_ModifyGuildMemberPLResult_RoleNotFound,		// �����޴˽�ɫ
	ENUM_ModifyGuildMemberPLResult_LevelIsFull,			// ְ����������
	ENUM_ModifyGuildMemberPLResult_GMOOperationFailed,	// ����ʧ��(GMO)
	ENUM_ModifyGuildMemberPLResult_ChatOperationFailed,	// ����ʧ��(Chat)
};
// ��ɢ�������о�-----------------------------------------------------------------------------------
enum ENUM_DismissGuildResult
{
	ENUM_DismissGuildResult_NULL = 0, 
	ENUM_DismissGuildResult_Success,			// �ɹ�.
	ENUM_DismissGuildResult_WorldNotFound,		// �Ҳ�������
	ENUM_DismissGuildResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
	ENUM_DismissGuildResult_LeaderDisconnect,	// ��ͷ�۲���δ����
	ENUM_DismissGuildResult_GuildNotFound,		// ���᲻����
	ENUM_DismissGuildResult_GMOOperationFailed,	// ����ʧ��(GMO)
	ENUM_DismissGuildResult_SystemNotReady,		// ϵͳ׼����(Chat)
	ENUM_DismissGuildResult_ChatOperationFailed,// ����ʧ��(Chat)
};

// �޸Ĺ������ƽ���о�-------------------------------------------------------------------------------
enum ENUM_ModifyGuildNameResult
{
	ENUM_ModifyGuildNameResult_NULL = 0, 
	ENUM_ModifyGuildNameResult_Success,				// �ɹ�.
	ENUM_ModifyGuildNameResult_WorldNotFound,		// �Ҳ�������
	ENUM_ModifyGuildNameResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
	ENUM_ModifyGuildNameResult_LeaderDisconnect,	// ��ͷ�۲���δ����
	ENUM_ModifyGuildNameResult_GuildNotFound,		// ���᲻����
	ENUM_ModifyGuildNameResult_GuildNameRepeat,		// ���������ظ�
	ENUM_ModifyGuildNameResult_GMOOperationFailed,	// ����ʧ��(GMO)
	ENUM_ModifyGuildNameResult_ChatOperationFailed,	// ����ʧ��(Chat)
};
// �޸Ĺ���ȼ�����о�-------------------------------------------------------------------------------
enum ENUM_ModifyGuildLvResult
{
	ENUM_ModifyGuildLvResult_NULL = 0, 
		ENUM_ModifyGuildLvResult_Success,				// �ɹ�
		ENUM_ModifyGuildLvResult_WorldNotFound,			// �Ҳ�������
		ENUM_ModifyGuildLvResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
		ENUM_ModifyGuildLvResult_LeaderDisconnect,		// ��ͷ�۲���δ����
		ENUM_ModifyGuildLvResult_LevelError1,			// ����ȷ�ĵȼ�(����������Χ)
		ENUM_ModifyGuildLvResult_GuildNotFound,			// ���᲻����
		ENUM_ModifyGuildLvResult_SameLevel,				// ����Ŀǰ�Ѿ��Ǹõȼ�
		ENUM_ModifyGuildLvResult_LevelError2,			// ����ȷ�ĵȼ�(�ᵼ�¹��������������С�ڹ���Ŀǰ����)
		ENUM_ModifyGuildLvResult_GMOOperationFailed,	// ����ʧ��(GMO)
		ENUM_ModifyGuildLvResult_ChatOperationFailed,	// ����ʧ��(Chat)
};

// �޸Ľ�ɫ�������о�-------------------------------------------------------------------------------
enum ENUM_ModifyCharacterQuestResult
{
		ENUM_ModifyCharacterQuestResult_NULL = 0, 
		ENUM_ModifyCharacterQuestResult_Success,				// �ɹ�.
		ENUM_ModifyCharacterQuestResult_WorldNotFound,			// �Ҳ�������
		ENUM_ModifyCharacterQuestResult_LeaderNotFound,			// �Ҳ�����ͷ�۲���
		ENUM_ModifyCharacterQuestResult_LeaderDisconnect,		// ��ͷ�۲���δ����
		ENUM_ModifyCharacterQuestResult_ArgumentError,			// ��������
		ENUM_ModifyCharacterQuestResult_RoleNotFound,			// ��ɫ������
		ENUM_ModifyCharacterQuestResult_RoleIsOnline,			// ��ɫ��������
		ENUM_ModifyCharacterQuestResult_GMOOperationFailed,		// ����ʧ��(GMO)
};

// ְҵ-----------------------------------------------------------------------------------------------
enum ENUM_Vocation
{
	ENUM_Vocation_NULL = -1, 

	ENUM_Vocation_1	= 0,		// ����
	ENUM_Vocation_2,			// ���ߵ���
	ENUM_Vocation_3,			// ��������
	ENUM_Vocation_4,			// ����ʦ
	ENUM_Vocation_5,			// ���ҹ��
	ENUM_Vocation_6,			// ����ʿ
};

// ȡ�ý�ɫ�б���-----------------------------------------------------------------------------------
enum ENUM_CharacterListResult
{
	ENUM_CharacterListResult_NULL = 0, 
	ENUM_CharacterListResult_Success,			// �ɹ�
	ENUM_CharacterListResult_WorldNotFound,		// �Ҳ�������
	ENUM_CharacterListResult_LeaderNotFound,	// �Ҳ�����ͷ�۲���
	ENUM_CharacterListResult_LeaderDisconnect,	// ��ͷ�۲���δ���� 
};


// ȡ�ù����Ա�б���-------------------------------------------------------------------------------
enum ENUM_GuildMemberListResult
{
	ENUM_GuildMemberListResult_NULL = 0, 
	ENUM_GuildMemberListResult_Success,					// �ɹ�.
	ENUM_GuildMemberListResult_GuildNotFound,			// ���᲻����
	ENUM_GuildMemberListResult_WorldNotFound,			// �Ҳ�������
	ENUM_GuildMemberListResult_LeaderNotFound,			// �Ҳ�����ͷ�۲���
	ENUM_GuildMemberListResult_LeaderDisconnect,		// ��ͷ�۲���δ����
};

// ȡ�ý�ɫ������Ѷ���-------------------------------------------------------------------------------
enum ENUM_CharacterBagResult
{
	ENUM_CharacterBagResult_NULL = 0, 
	ENUM_CharacterBagResult_Success,			// �ɹ�
	ENUM_CharacterBagResult_TypeError,			// ����ȷ������
	ENUM_CharacterBagResult_RoleNotFound,		// ��ɫ������
	ENUM_CharacterBagResult_CharacterBagEmpty,	// ��ɫ�����ǿյ�
	ENUM_CharacterBagResult_WorldNotFound,		// �Ҳ�������
	ENUM_CharacterBagResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
	ENUM_CharacterBagResult_LeaderDisconnect,	// ��ͷ�۲���δ���� 
};


// ȡ�ý�ɫ�����б���-------------------------------------------------------------------------------
enum ENUM_CharacterFriendListResult
{
	ENUM_CharacterFriendListResult_NULL = 0, 
	ENUM_CharacterFriendListResult_Success,						// �ɹ�.
	ENUM_CharacterFriendListResult_RoleNotFound,				// ��ɫ������
	ENUM_CharacterFriendListResult_CharacterFriendListEmpty,	// ��ɫ�޺���
	ENUM_CharacterFriendListResult_WorldNotFound,				// �Ҳ�������
	ENUM_CharacterFriendListResult_LeaderNotFound,				// �Ҳ�����ͷ�۲���
	ENUM_CharacterFriendListResult_LeaderDisconnect,			// ��ͷ�۲���δ����
};

// �˼ʹ�ϵ-------------------------------------------------------------------------------------------
enum ENUM_RelationshipPairType
{
	ENUM_RelationshipPairType_None = 0, 	
	ENUM_RelationshipPairType_Recognition,			// ��ʶ
	ENUM_RelationshipPairType_FamilyMembers,		// ����
	ENUM_RelationshipPairType_Confidant, 			// ֪��
	ENUM_RelationshipPairType_SwornBrothers, 		// ����
	ENUM_RelationshipPairType_FightingBrother, 		// ս��
	ENUM_RelationshipPairType_MasterApprentice,		// ʦͽ
	ENUM_RelationshipPairType_ApprenticeMaster,		// ͽʦ
	ENUM_RelationshipPairType_Lover,				// ����
	ENUM_RelationshipPairType_Couple,				// ����
};


// ȡ�ý�ɫ������Ѷ���-------------------------------------------------------------------------------
enum ENUM_CharacterInfoResult
{
	ENUM_CharacterInfoResult_NULL = 0, 
	ENUM_CharacterInfoResult_Success,			// �ɹ�
	ENUM_CharacterInfoResult_RoleNotFound,		// ��ɫ������
	ENUM_CharacterInfoResult_WorldNotFound,		// �Ҳ�������
	ENUM_CharacterInfoResult_LeaderNotFound,	// �Ҳ�����ͷ�۲���
	ENUM_CharacterInfoResult_LeaderDisconnect,	// ��ͷ�۲���δ����
};

// ȡ�ý�ɫ������Ѷ���-------------------------------------------------------------------------------
enum ENUM_CharacterQuestResult
{
	ENUM_CharacterQuestResult_NULL = 0, 
	ENUM_CharacterQuestResult_Success,			// �ɹ�
	ENUM_CharacterQuestResult_TypeError,		// ����ȷ������
	ENUM_CharacterQuestResult_RoleNotFound,		// ��ɫ������
	ENUM_CharacterQuestResult_QuestNotFound,	// û�з��ϵ�����
	ENUM_CharacterQuestResult_WorldNotFound,	// �Ҳ�������
	ENUM_CharacterQuestResult_LeaderNotFound,	// �Ҳ�����ͷ�۲���
	ENUM_CharacterQuestResult_LeaderDisconnect,	// ��ͷ�۲���δ����
};


// ȡ�ù��������Ѷ���-------------------------------------------------------------------------------
enum ENUM_GuildBaseInfoResult
{
	ENUM_GuildBaseInfoResult_NULL = 0, 
	ENUM_GuildBaseInfoResult_Success,					// �ɹ�.
	ENUM_GuildBaseInfoResult_GuildNotFound,				// ���᲻����
	ENUM_GuildBaseInfoResult_WorldNotFound,				// �Ҳ�������
	ENUM_GuildBaseInfoResult_LeaderNotFound,			// �Ҳ�����ͷ�۲���
	ENUM_GuildBaseInfoResult_LeaderDisconnect,			// ��ͷ�۲���δ����
};

// �޸Ĺ��ṫ�����о�-------------------------------------------------------------------------------
enum ENUM_ModifyGuildNoticeResult
{
	ENUM_ModifyGuildNoticeResult_NULL = 0, 
		ENUM_ModifyGuildNoticeResult_Success,				// �ɹ�
		ENUM_ModifyGuildNoticeResult_WorldNotFound,			// �Ҳ�������
		ENUM_ModifyGuildNoticeResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
		ENUM_ModifyGuildNoticeResult_LeaderDisconnect,		// ��ͷ�۲���δ����
		ENUM_ModifyGuildNoticeResult_GuildNotFound,			// ���᲻����
		ENUM_ModifyGuildNoticeResult_GMOOperationFailed,	// ����ʧ��(GMO)
		ENUM_ModifyGuildNoticeResult_ChatOperationFailed,	// ����ʧ��(Chat)
};

// �޸Ľ�ɫ��Ǯ����о�-------------------------------------------------------------------------------
enum ENUM_ModifyCharacterMoneyResult
{
		ENUM_ModifyCharacterMoneyResult_NULL = 0, 
		ENUM_ModifyCharacterMoneyResult_Success,				// �ɹ�
		ENUM_ModifyCharacterMoneyResult_WorldNotFound,			// �Ҳ�������
		ENUM_ModifyCharacterMoneyResult_LeaderNotFound,			// �Ҳ�����ͷ�۲���
		ENUM_ModifyCharacterMoneyResult_LeaderDisconnect,		// ��ͷ�۲���δ����
		ENUM_ModifyCharacterMoneyResult_TypeError,				// ����ȷ������
		ENUM_ModifyCharacterMoneyResult_RoleNotFound,			// ��ɫ������
		ENUM_ModifyCharacterMoneyResult_RoleIsOnline,			// ��ɫ��������
		ENUM_ModifyCharacterMoneyResult_CountError,				// �޸ĺ�Ľ�����Χ
		ENUM_ModifyCharacterMoneyResult_GMOOperationFailed,		// ����ʧ��(GMO)
};

// �޸Ľ�ɫɾ���������о�---------------------------------------------------------------------------
enum ENUM_ModifyCharacterDropPwResult
{
	ENUM_ModifyCharacterDropPwResult_NULL = 0, 
		ENUM_ModifyCharacterDropPwResult_Success,				// �ɹ�
		ENUM_ModifyCharacterDropPwResult_WorldNotFound,			// �Ҳ�������
		ENUM_ModifyCharacterDropPwResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
		ENUM_ModifyCharacterDropPwResult_LeaderDisconnect,		// ��ͷ�۲���δ����
		ENUM_ModifyCharacterDropPwResult_PasswordError,			// �����ʽ����
		ENUM_ModifyCharacterDropPwResult_RoleNotFound,			// ��ɫ������
		ENUM_ModifyCharacterDropPwResult_RoleIsOnline,			// ��ɫ��������
		ENUM_ModifyCharacterDropPwResult_GMOOperationFailed,	// ����ʧ��(GMO)
};

// �޸��ʺ������������о�---------------------------------------------------------------------------
enum ENUM_ModifyAccountBankPwResult
{
		ENUM_ModifyAccountBankPwResult_NULL = 0, 
		ENUM_ModifyAccountBankPwResult_Success,				// �ɹ�
		ENUM_ModifyAccountBankPwResult_WorldNotFound,			// �Ҳ�������
		ENUM_ModifyAccountBankPwResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
		ENUM_ModifyAccountBankPwResult_LeaderDisconnect,		// ��ͷ�۲���δ����
		ENUM_ModifyAccountBankPwResult_PasswordError,			// �����ʽ����
		ENUM_ModifyAccountBankPwResult_RoleNotFound,			// ��ɫ������
		ENUM_ModifyAccountBankPwResult_RoleIsOnline,			// ��ɫ��������
		ENUM_ModifyAccountBankPwResult_GMOOperationFailed,	// ����ʧ��(GMO)
};

// ɾ����ɫ��Ʒ����о�-------------------------------------------------------------------------------
enum ENUM_DelCharacterItemResult
{
		ENUM_DelCharacterItemResult_NULL = 0, 
		ENUM_DelCharacterItemResult_Success,			// �ɹ�
		ENUM_DelCharacterItemResult_WorldNotFound,		// �Ҳ�������
		ENUM_DelCharacterItemResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
		ENUM_DelCharacterItemResult_LeaderDisconnect,	// ��ͷ�۲���δ����
		ENUM_DelCharacterItemResult_TypeError,			// ����ȷ������
		ENUM_DelCharacterItemResult_PositionError,		// ����ȷ��λ��
		ENUM_DelCharacterItemResult_RoleNotFound,		// ��ɫ������
		ENUM_DelCharacterItemResult_RoleIsOnline,		// ��ɫ��������
		ENUM_DelCharacterItemResult_ItemNotFound,		// ��λ����Ʒ������
		ENUM_DelCharacterItemResult_GMOOperationFailed,	// ����ʧ��(GMO)	
};

// ȡ���ʺ���Ѷ���-----------------------------------------------------------------------------------
enum ENUM_AccountInfoResult
{
		ENUM_AccountInfoResult_NULL = 0, 
		ENUM_AccountInfoResult_Success,				// �ɹ�
		ENUM_AccountInfoResult_AccountNotFound,		// �ʺŲ�����
		ENUM_AccountInfoResult_RoleNotFound,		// �ʺ���û�н�ɫ
		ENUM_AccountInfoResult_WorldNotFound,		// �Ҳ�������
		ENUM_AccountInfoResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
		ENUM_AccountInfoResult_LeaderDisconnect,	// ��ͷ�۲���δ����
};

// ȡ�ý�ɫ������Ѷ���-------------------------------------------------------------------------------
enum ENUM_CharacterAdvanceInfoResult
{
		ENUM_CharacterAdvanceInfoResult_NULL = 0, 
		ENUM_CharacterAdvanceInfoResult_Success,			// �ɹ�
		ENUM_CharacterAdvanceInfoResult_RoleNotFound,		// ��ɫ������
		ENUM_CharacterAdvanceInfoResult_WorldNotFound,		// �Ҳ�������
		ENUM_CharacterAdvanceInfoResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
		ENUM_CharacterAdvanceInfoResult_LeaderDisconnect,	// ��ͷ�۲���δ����
};
// ȡ�ý�ɫ�������б���-----------------------------------------------------------------------------
enum ENUM_CharacterBlackListResult
{
		ENUM_CharacterBlackListResult_NULL = 0, 
		ENUM_CharacterBlackListResult_Success,			// �ɹ�.
		ENUM_CharacterBlackListResult_RoleNotFound,		// ��ɫ������
		ENUM_CharacterBlackListResult_BlackListEmpty,	// ��ɫ�޺�����
		ENUM_CharacterBlackListResult_WorldNotFound,	// �Ҳ�������
		ENUM_CharacterBlackListResult_LeaderNotFound,	// �Ҳ�����ͷ�۲���
		ENUM_CharacterBlackListResult_LeaderDisconnect, // ��ͷ�۲���δ����
};

// ȡ�ý�ɫ��ħ��Ѷ���-------------------------------------------------------------------------------
enum ENUM_CharacterMobResult
{
	ENUM_CharacterMobResult_NULL = 0, 
		ENUM_CharacterMobResult_Success,			// �ɹ�
		ENUM_CharacterMobResult_RoleNotFound,		// ��ɫ������
		ENUM_CharacterMobResult_WorldNotFound,		// �Ҳ�������
		ENUM_CharacterMobResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
		ENUM_CharacterMobResult_LeaderDisconnect,	// ��ͷ�۲���δ����
};

// �ƶ���ɫ����ȫ�����о�---------------------------------------------------------------------------
enum ENUM_MoveCharacterToSafeResult
{
	ENUM_MoveCharacterToSafeResult_NULL = 0, 
		ENUM_MoveCharacterToSafeResult_Success,				// �ɹ�.
		ENUM_MoveCharacterToSafeResult_RoleNotFound,		// ��ɫ������
		ENUM_MoveCharacterToSafeResult_WorldNotFound,		// �Ҳ�������
		ENUM_MoveCharacterToSafeResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
		ENUM_MoveCharacterToSafeResult_LeaderDisconnect,	// ��ͷ�۲���δ����
		ENUM_MoveCharacterToSafeResult_GMOOperationFailed,	// ����ʧ��(GMO)
		ENUM_MoveCharacterToSafeResult_LocalOperationFailed,// ����ʧ��(Local)
};

// ȡ�ý�ɫͼ����Ѷ���-------------------------------------------------------------------------------
enum ENUM_CharacterPuzzleMapResult
{
		ENUM_CharacterPuzzleMapResult_NULL = 0, 
		ENUM_CharacterPuzzleMapResult_Success,				// �ɹ�
		ENUM_CharacterPuzzleMapResult_RoleNotFound,			// ��ɫ������
		ENUM_CharacterPuzzleMapResult_WorldNotFound,		// �Ҳ�������
		ENUM_CharacterPuzzleMapResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
		ENUM_CharacterPuzzleMapResult_LeaderDisconnect,		// ��ͷ�۲���δ����
};

// ȡ�ý�ɫ������Ѷ���-------------------------------------------------------------------------------
enum ENUM_CharacterSkillResult
{
	ENUM_CharacterSkillResult_NULL = 0, 
		ENUM_CharacterSkillResult_Success,				// �ɹ�
		ENUM_CharacterSkillResult_RoleNotFound,			// ��ɫ������
		ENUM_CharacterSkillResult_WorldNotFound,		// �Ҳ�������
		ENUM_CharacterSkillResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
		ENUM_CharacterSkillResult_LeaderDisconnect,		// ��ͷ�۲���δ����
};
// ɾ����ɫ��ħ����о�-------------------------------------------------------------------------------
enum ENUM_DelCharacterMobResult
{
	ENUM_DelCharacterMobResult_NULL = 0, 
		ENUM_DelCharacterMobResult_Success,				// �ɹ�
		ENUM_DelCharacterMobResult_WorldNotFound,		// �Ҳ�������
		ENUM_DelCharacterMobResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
		ENUM_DelCharacterMobResult_LeaderDisconnect,	// ��ͷ�۲���δ����
		ENUM_DelCharacterMobResult_RoleNotFound,		// ��ɫ������
		ENUM_DelCharacterMobResult_RoleIsOnline,		// ��ɫ��������
		ENUM_DelCharacterMobResult_MobNotFound,			// ��ɫ��δȡ�ø÷�ħ
		ENUM_DelCharacterMobResult_FlagError,			// ������
		ENUM_DelCharacterMobResult_GMOOperationFailed,	// ����ʧ��(GMO)	
};
// �޸Ľ�ɫͼ������о�-------------------------------------------------------------------------------
enum ENUM_ModifyPuzzleMapResult
{
	ENUM_ModifyPuzzleMapResult_NULL = 0, 
		ENUM_ModifyPuzzleMapResult_Success,				// �ɹ�
		ENUM_ModifyPuzzleMapResult_WorldNotFound,		// �Ҳ�������
		ENUM_ModifyPuzzleMapResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
		ENUM_ModifyPuzzleMapResult_LeaderDisconnect,	// ��ͷ�۲���δ����
		ENUM_ModifyPuzzleMapResult_RoleNotFound,		// ��ɫ������
		ENUM_ModifyPuzzleMapResult_RoleIsOnline,		// ��ɫ��������
		ENUM_ModifyPuzzleMapResult_FlagError,			// ������
		ENUM_ModifyPuzzleMapResult_GMOOperationFailed,	// ����ʧ��(GMO)	
};
// �������빦�ܿ����б����о�-----------------------------------------------------------------------
enum ENUM_ReloadFunctionSwitchResult
{
	ENUM_ReloadFunctionSwitchResult_NULL = 0, 
		ENUM_ReloadFunctionSwitchResult_Success,			// �ɹ�.
		ENUM_ReloadFunctionSwitchResult_WorldNotFound,		// �Ҳ�������
		ENUM_ReloadFunctionSwitchResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
		ENUM_ReloadFunctionSwitchResult_LeaderDisconnect,	// ��ͷ�۲���δ����
		ENUM_ReloadFunctionSwitchResult_LocalDisconnect,	// �����ŷ���δ����
};

// add by ltx
// ���Ϳ�����Ʒ�ʼ�����о�-------------------------------------------------------------------------------
enum ENUM_SendCustomItemMailResult
{
	ENUM_SendCustomItemMailResult_NULL = 0, 
		ENUM_SendCustomItemMailResult_Success,				// �ɹ�.
		ENUM_SendCustomItemMailResult_ArgumentError,		// ��������
		ENUM_SendCustomItemMailResult_ItemIDError,			// ����ȷ����Ʒ���
		ENUM_SendCustomItemMailResult_WorldNotFound,		// �Ҳ�������
		ENUM_SendCustomItemMailResult_LeaderNotFound,		// �Ҳ�����ͷ�۲���
		ENUM_SendCustomItemMailResult_LeaderDisconnect,		// ��ͷ�۲���δ����
		ENUM_SendCustomItemMailResult_RoleNotFound,			// ��ɫ������
		ENUM_SendCustomItemMailResult_LocalDisconnect,		// �����ŷ���δ����
		ENUM_SendCustomItemMailResult_LocalOperationFailed,	// ����ʧ��(Local)
};

// ��Ʒ����-------------------------------------------------------------------------------------------
struct S_GMItemData
{
	int    iPos;				// λ��(���ϱ���0~99��Ʒ��,100~115װ����,116~118������Ʒ��)
	int	   iItemType;			// �������(0:һ����Ʒ, 1:����)
	int    iOrgObjID;			// �������ID
	int    iCreateTime;			// ����ʱ��
	int    iSerial;				// ���(��ʶ��Ʒ�Ƿ��п���)
	char   szName[32];			// ����
	time_t iLiveTime;			// ���浽��ʱ��,(С��1����������)
	int    iCoolDown;			// �´ο���ʹ�õ�ʱ��(С��1������������)
	bool   bNPCTrade;			// �Ƿ���Ը�NPC����(1:��, 0:����)
	bool   bUserTrade;			// �Ƿ���Ը���ҽ���(1:��, 0:����)
	bool   bItemMallGoods;		// �Ƿ����̳���Ʒ(1:��, 0:����)
	int    iCount;				// ����
	int    iSellCost;			// ����(�̵� or ��̯)
	short  wDurable; 			// Ŀǰ�;ö�
	//-------------------------------------
	union
	{
		int	iInheritID[10];						// ��ʯID, -1:û�п��� 0:�ѿ�����û������ʯ	

		// ��������Ѷ
		struct 
		{
			unsigned int iExp;					// ����ֵ
			unsigned int iNowHP;				// ĿǰHPֵ
			unsigned int iPetIndex;				// �����������(0~256)						
			unsigned int iRenewCount;			// ��ͯ����(0-255)
			unsigned int iLV;					// �ȼ�(1-50��)
			unsigned int iGeneration;			// ����(1-31)
			unsigned int iMixCount;				// �ϳɴ���(0-31)		
			unsigned int iGrowthRate;			// �ɳ���(1-10) 
			unsigned int iAttrQuality;			// Ʒ��(0-10),ָ���Եĳɳ���(�����)
			// ����	
			unsigned int iHP;					// HP�ӳ�ֵ(0~32767)
			unsigned int iWushu;				// ����(0~2047)
			unsigned int iSpirit;				// ����(0~2047)
			unsigned int iConstitution;			// ����(0~2047)
			unsigned int iQuality;				// ����(0~2047)
			unsigned int iDexterity;			// ��(0~2047)
			unsigned int iFate;					// ����(0~2047)
			unsigned int iHP_Plus;				// HPֵ�۸�(0-127)
			unsigned int iWushu_Plus;			// �����۸�(0-127)
			unsigned int iSpirit_Plus;			// ���Ի۸�(0-127)
			unsigned int iConstitution_Plus;	// �����۸�(0-127)
			unsigned int iQuality_Plus;			// �����۸�(0-127)
			unsigned int iDexterity_Plus;		// ���۸�(0-127)
			unsigned int iFate_Plus;			// ���˻۸�(0-127)
			// ����		
			char sSkill[6];						// 6��������λ(-1:δ���� 0:δ���ü��� 1:���＼������)
		}sPet;// struct	
	};// union
};
// GMC->GMS:�ʺ���������ʽ--------------------------------------------------------------------------
struct PGGMCConnection_CtoS_RequestLogin : public S_ObjNetPktBase
{
	char szAccount[32];		// �ʺ�
	char szPassword[32];	// ����
	char szIP[16];			// IPλַ
		
	PGGMCConnection_CtoS_RequestLogin()
	{
		uiCmdID = ENUM_PGGMCConnection_CtoS_RequestLogin;
	};
};

// GMS->GMC:�����������ʽ--------------------------------------------------------------------------
struct PGGMCConnection_StoC_LoginResult : public S_ObjNetPktBase
{
	ENUM_GMCLoginResult emResult;	// ����о�
		
	PGGMCConnection_StoC_LoginResult()
	{
		uiCmdID = ENUM_PGGMCConnection_StoC_LoginResult;
	};
};
// GMC->GMS:Ҫ���ͷǹ̶�����------------------------------------------------------------------------
struct PGServerControl_CtoS_SendNonscheduleBulletin  : public S_ObjNetPktBase
{
	int  iWorldID;				// ������,-1����������������
	int  iDelayTime;			// �ӳٷ���ʱ��(��),0������������
	char szContent[512];		// ��������

	PGServerControl_CtoS_SendNonscheduleBulletin()
	{
		uiCmdID = ENUM_PGServerControl_CtoS_SendNonscheduleBulletin;
	};
};

// GMS->GMC:�ش����ͷǹ̶�������--------------------------------------------------------------------
struct PGServerControl_StoC_SendNonscheduleBulletin : public S_ObjNetPktBase
{
	ENUM_SendNonscheduleBulletinResult emResult;		// ���ͷǹ̶�������

	PGServerControl_StoC_SendNonscheduleBulletin()
	{
		uiCmdID = ENUM_PGServerControl_StoC_SendNonscheduleBulletin;
	};
};

// GMC->GMS:Ҫ�������ų̹���
struct PGServerControl_CtoS_AddScheduleBulletin  : public S_ObjNetPktBase
{
	int  iWorldID;				// ������
	int  iBulletinID;			// ����ID
	int	 iZoneID;				// ���AID,-1����ȫ��
	int  iMonth;				// �ų�ʱ��-��
	int  iDay;					// �ų�ʱ��-��
	int  iDayOfWeek;			// �ų�ʱ��-����
	int  iHour;					// �ų�ʱ��-ʱ
	int  iMinute;				// �ų�ʱ��-��
	char szContent[512];		// ��������

	PGServerControl_CtoS_AddScheduleBulletin()
	{
		uiCmdID = ENUM_PGServerControl_CtoS_AddScheduleBulletin;
	};
};

// GMS->GMC:�ش������ų̹�����----------------------------------------------------------------------
struct PGServerControl_StoC_AddScheduleBulletin : public S_ObjNetPktBase
{
	ENUM_AddScheduleBulletinResult emResult;	// ����о�
	int iWorldID;								// ������
	int iBulletinID;							// ����ID

	PGServerControl_StoC_AddScheduleBulletin()
	{
		uiCmdID = ENUM_PGServerControl_StoC_AddScheduleBulletin;
	};
};
// GMC->GMS:Ҫ��ɾ���ų̹���
struct PGServerControl_CtoS_DelScheduleBulletin  : public S_ObjNetPktBase
{
	int iWorldID;							// ������
	int iBulletinID;						// �ӳٷ���ʱ��(��),0������������

	PGServerControl_CtoS_DelScheduleBulletin()
	{
		uiCmdID = ENUM_PGServerControl_CtoS_DelScheduleBulletin;
	};
};

// GMS->GMC:�ش�ɾ���ų̹�����----------------------------------------------------------------------
struct PGServerControl_StoC_DelScheduleBulletin : public S_ObjNetPktBase
{
	ENUM_DelScheduleBulletinResult emResult;	// ����о�
	int iWorldID;								// ������
	int iBulletinID;							// ����ID

	PGServerControl_StoC_DelScheduleBulletin()
	{
		uiCmdID = ENUM_PGServerControl_StoC_DelScheduleBulletin;
	};
};
// GMC->GMS:Ҫ�����������ų̹���----------------------------------------------------------------------
struct PGServerControl_CtoS_ReloadScheduleBulletin  : public S_ObjNetPktBase
{
	int iWorldID;		// ������

	PGServerControl_CtoS_ReloadScheduleBulletin()
	{
		uiCmdID = ENUM_PGServerControl_CtoS_ReloadScheduleBulletin;
	};
};

// GMS->GMC:�ش����������ų̹�����------------------------------------------------------------------
struct PGServerControl_StoC_ReloadScheduleBulletin : public S_ObjNetPktBase
{
	ENUM_ReloadScheduleBulletinResult emResult;	// ����о�
	int iWorldID;								// ������

	PGServerControl_StoC_ReloadScheduleBulletin()
	{
		uiCmdID = ENUM_PGServerControl_StoC_ReloadScheduleBulletin;
	};
};
// GMC->GMS:Ҫ����ͽ�Ʒ�ʼ�--------------------------------------------------------------------------
struct PGServerControl_CtoS_SendGiftMail  : public S_ObjNetPktBase
{
	int  iWorldID;			// ������
	char szRoleName[32];	// ��ɫ����
	char szSubject[32];		// �ʼ���ּ
	char szContent[256];	// �ʼ�����
	int  iMoney;			// �ʼ���Ǯ
	int  aiGiftDBID[5];		// ��Ʒ���1~5(-1�����޸���)
	char szReason[1024];	// ���ͽ�Ʒԭ��
	
	PGServerControl_CtoS_SendGiftMail()
	{
		uiCmdID = ENUM_PGServerControl_CtoS_SendGiftMail;
	};
};

// GMS->GMC:�ش����ͽ�Ʒ�ʼ����----------------------------------------------------------------------
struct PGServerControl_StoC_SendGiftMail : public S_ObjNetPktBase
{
	ENUM_SendGiftMailResult emResult;	// ����о�
	int  iWorldID;						// ������
	char szRoleName[32];				// ��ɫ����
	char szSubject[32];					// �ʼ���ּ
	int  iMoney;						// �ʼ���Ǯ
	int  aiGiftDBID[5];					// ��Ʒ���1~5(-1�����޸���)
	int  aiItemStatus[5];				// ����1~5��Ʒ״̬,״̬��������:
	// 0:�����ɹ����޸���,			1:��Ʒ�������ϴ���(��Ʒ���)
	// 2:��Ʒ�������ϴ���(��Ʒ���),3:��Ʒ�������ϴ���(��ʯ���)
	// 4:��Ʒ�������ϴ���(��),	5:��Ʒ�������ϴ���(����)
	// 6:������Ʒʧ��
	PGServerControl_StoC_SendGiftMail()
	{
		uiCmdID = ENUM_PGServerControl_StoC_SendGiftMail;
	};
};

// GMC->GMS:Ҫ��ȡ����������
struct PGServerStatus_CtoS_PlayerOnlineCount : public S_ObjNetPktBase
{
	int iWorldID; // ������
		
	PGServerStatus_CtoS_PlayerOnlineCount()
	{
		uiCmdID = ENUM_PGServerStatus_CtoS_PlayerOnlineCount;
	};
};

// GMS -> GMC �ش�ȡ�������������
struct PGServerStatus_StoC_PlayerOnlineCount : public S_ObjNetPktBase
{
	ENUM_PlayerOnlineCountResult emResult;	// ����о�
	int	iWorldID;							// ������
	int	iCount;								// ��������
		
	PGServerStatus_StoC_PlayerOnlineCount()
	{
		uiCmdID = ENUM_PGServerStatus_StoC_PlayerOnlineCount;
	};
};





// GMC->GMS:Ҫ��ȡ���ų̹����б�
struct PGServerStatus_CtoS_ScheduleBulletinList  : public S_ObjNetPktBase
{
	int iWorldID; // ������
		
	PGServerStatus_CtoS_ScheduleBulletinList()
	{
		uiCmdID = ENUM_PGServerStatus_CtoS_ScheduleBulletinList;
	};
};

// GMS->GMC:�ش�ȡ���ų̹����б���------------------------------------------------------------------
struct PGServerStatus_StoC_ScheduleBulletinList : public S_ObjNetPktBase
{
	ENUM_ScheduleBulletinListResult emResult;	// ����о�
	int	 iWorldID;								// ������,-1����������
	int  iBulletinID;							// ����ID
	int	 iZoneID;								// ���AID,-1����ȫ��
	int  iMonth;								// �ų�ʱ��-��
	int  iDay;									// �ų�ʱ��-��
	int  iDayOfWeek;							// �ų�ʱ��-����
	int  iHour;									// �ų�ʱ��-ʱ
	int  iMinute;								// �ų�ʱ��-��
	char szContent[512];						// ��������
	
	PGServerStatus_StoC_ScheduleBulletinList()
	{
		uiCmdID = ENUM_PGServerStatus_StoC_ScheduleBulletinList;
	};
};

// GMC->GMS:Ҫ����ĳ�������--------------------------------------------------------------------------
struct PGPlayerControl_CtoS_BanishPlayer  : public S_ObjNetPktBase
{
	int  iWorldID;				// ������
	char szRoleName[32];		// ��ɫ����

	PGPlayerControl_CtoS_BanishPlayer()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_BanishPlayer;
	};
};

// GMS->GMC:�ش���ĳ������߽��----------------------------------------------------------------------
struct PGPlayerControl_StoC_BanishPlayer : public S_ObjNetPktBase
{
	ENUM_BanishPlayerResult emResult;	// ��ĳ������߽��
	int  iWorldID;						// ������
	char szRoleName[32];				// ��ɫ����

	PGPlayerControl_StoC_BanishPlayer()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_BanishPlayer;
	};
};
// GMC->GMS:Ҫ��ȡ�ý�ɫ������Ѷ----------------------------------------------------------------------
struct PGPlayerStatus_CtoS_CharacterBag : public S_ObjNetPktBase
{
	int		iWorldID;						// ������
	char	szRoleName[32];					// ��ɫ����
	int		iBagType;						// ��������(0:���� 1:����� 2:PK�������� 3:����)

	PGPlayerStatus_CtoS_CharacterBag()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_CharacterBag;
	};
};

// GMS->GMC:�ش�ȡ�ý�ɫ������Ѷ���------------------------------------------------------------------
struct PGPlayerStatus_StoC_CharacterBag : public S_ObjNetPktBase
{
	ENUM_CharacterBagResult emResult;		// ����о�
	int		iWorldID;						// ������,-1����������
	char	szAccount[32];					// �ʺ�
	char	szRoleName[32];					// ��ɫ����
	int		iBagType;						// ��������(0:���� 1:����� 2:PK�������� 3:����)
	S_GMItemData sGMItemData;				// ��Ʒ����

	PGPlayerStatus_StoC_CharacterBag()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_CharacterBag;
	};
};
// GMC->GMS:Ҫ��ȡ�ý�ɫ�����б�----------------------------------------------------------------------
struct PGPlayerStatus_CtoS_CharacterFriendList : public S_ObjNetPktBase
{
	int		iWorldID;							// ������
	char	szRoleName[32];		// ��ɫ����

	PGPlayerStatus_CtoS_CharacterFriendList()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_CharacterFriendList;
	};
};

// GMS->GMC:�ش�ȡ�ý�ɫ�����б���------------------------------------------------------------------
struct PGPlayerStatus_StoC_CharacterFriendList : public S_ObjNetPktBase
{
	ENUM_CharacterFriendListResult emResult;	// ����о�
	int		iWorldID;							// ������,-1����������
	char	szAccount[32];						// �ʺ�
	char	szRoleName[32];						// ��ɫ����
	char	szFriendName[32];					// ���ѽ�ɫ����
	ENUM_RelationshipPairType emRelation;		// ��ϵ

	PGPlayerStatus_StoC_CharacterFriendList()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_CharacterFriendList;
	};
};
// ��ɫ��������---------------------------------------------------------------------------------------
struct S_GMRoleInfo
{
	int	 X, Y, Z;				// ����λ��(x,y,z����)
	char szNickName[32];		// �ǳ�
	char szGuildName[32];		// ��������
	int	 iGuildPowerLevel;		// ����ְ��(-1�����޹���)
	ENUM_Vocation emVoc;		// ְҵ
	int	 iLevel;				// �ȼ�
	int  iExp;					// Ŀǰ����
	int	 iBodyMoney;			// ���Ͻ�Ǯ
    int	 iBankMoney;			// �ֿ��Ǯ
	
    int	 iSex;					// �Ա�(0:Ů 1:��)
	int  iFame;					// ����
    int	 iHP;					// ĿǰѪ��
	int	 iMP;					// Ŀǰ����
	int	 iAP;					// Ŀǰ����
	int	 iEP;					// Ŀǰ����
	int	 iSkillPoint;			// �����ü��ܵ���
	int	 iSkillTreeNode; 		// Ŀǰ�������ߵ���λ��
	int	 iReincarnation; 		// ת������
	
	int  iTitleID;				// ��ѡ�ƺ�ID(-1����ʹ��)
	int  dwTitleFlag[32];		// Ŀǰ��ȡ�õĳƺ����(ʹ�ô������PalDataReader.lib��ѯ����ƺ���Ѷ)
	int  iMurdererCount; 		// а��ֵ
	int	 iEvilCount;			// ��ҵֵ
	
	// ����������λ,����ɫ����ʱ����ȡ��,������ʱΪ0
	int  iWushu;				// ����
	int  iConstitution;			// ����
	int  iSpirit; 				// ����
	int  iQuality;				// ����
	int  iDexterity;			// ��
	int  iFate;					// ����
	
	S_GMRoleInfo()
	{
		szGuildName[0] = '\0';
		iGuildPowerLevel = -1;
	}
};
// �������ܽṹ---------------------------------------------------------------------------------------
struct S_GMLifeHoodSkill
{
	// ����6����������,���������������Ϊ: 0�ڿ�,1����,2���,3ұ��,4����,5����
	int iLevel[6];		// �ȼ�
	int iEXP[6];		// ����ֵ
	
	S_GMLifeHoodSkill()
	{
		for (int i=0; i<6; i++)
		{
			iLevel[i] = 0;
			iEXP[i]   = 0;
		}
	}
};
// �ʺŽ�ɫ��Ѷ---------------------------------------------------------------------------------------
struct S_GMAccountRoleInfo
{
	int	 iDBID;					// ��ɫDBID(-1����������)
	char szRoleName[32];		// ��ɫ����
	char szCreateTime[32];		// ����ʱ��
	
	S_GMAccountRoleInfo()
	{
		iDBID = -1;
	}
};
// GMC->GMS:Ҫ��ȡ�ý�ɫ������Ѷ----------------------------------------------------------------------
struct PGPlayerStatus_CtoS_CharacterInfo : public S_ObjNetPktBase
{
	int  iWorldID;					// ������
	char szRoleName[32];			// ��ɫ����

	PGPlayerStatus_CtoS_CharacterInfo()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_CharacterInfo;
	};
};

// GMS->GMC:�ش�ȡ�ý�ɫ������Ѷ���------------------------------------------------------------------
struct PGPlayerStatus_StoC_CharacterInfo : public S_ObjNetPktBase
{
	ENUM_CharacterInfoResult emResult;	// ����о�
	int	 iWorldID;						// ������
	char szAccount[32];					// �ʺ�
	char szRoleName[32];				// ��ɫ����
	int	 iDBID;							// ��ɫDBID
	int	 iZoneID;						// ������
	char szZoneName[32];				// ��������
	int	 iLoginTime;					// ������ʱ��
	char szIP[16];						// ���IP,��Ϊ���ִ���ʾ��ɫ��������
	S_GMRoleInfo sGMRoleInfo;			// ��ɫ��Ѷ
	
	PGPlayerStatus_StoC_CharacterInfo()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_CharacterInfo;
	};
};
// GMC->GMS:Ҫ��ȡ�ý�ɫ�б�--------------------------------------------------------------------------
struct PGPlayerStatus_CtoS_CharacterList : public S_ObjNetPktBase
{
	int iWorldID; // ������

	PGPlayerStatus_CtoS_CharacterList()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_CharacterList;
	};
};

// GMS->GMC:�ش�ȡ�ý�ɫ�б���----------------------------------------------------------------------
struct PGPlayerStatus_StoC_CharacterList : public S_ObjNetPktBase
{
	ENUM_CharacterListResult emResult;	// ����о�
	int		iWorldID;					// ������,-1����������
	int		iDBID;						// ��ɫDBID
	char	szAccount[32];				// �ʺ�
	char	szRoleName[32];				// ��ɫ����
	int		iZoneID;					// ������
	char	szZoneName[32];				// ��������
	int		iLoginTime;					// ����ʱ��
	char	szIP[16];					// IP,�������Ϊ���ִ���ʾ��������

	PGPlayerStatus_StoC_CharacterList()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_CharacterList;
	};
};
// GMC->GMS:Ҫ��ȡ�ý�ɫ������Ѷ----------------------------------------------------------------------
struct PGPlayerStatus_CtoS_CharacterQuest : public S_ObjNetPktBase
{
	int		iWorldID;							// ������
	char	szRoleName[32];						// ��ɫ����
	int		iQuestType;							// ��������(1:����� 2:�ѽ�)

	PGPlayerStatus_CtoS_CharacterQuest()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_CharacterQuest;
	};
};

// GMS->GMC:�ش�ȡ�ý�ɫ������Ѷ���------------------------------------------------------------------
struct PGPlayerStatus_StoC_CharacterQuest : public S_ObjNetPktBase
{
	ENUM_CharacterQuestResult emResult;			// ����о�
	int		iWorldID;							// ������,-1����������
	char	szAccount[32];						// �ʺ�
	char	szRoleName[32];						// ��ɫ����
	int		iQuestType;							// ��������(1:����� 2:�ѽ�)
	int		iQuestID;							// ������

	PGPlayerStatus_StoC_CharacterQuest()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_CharacterQuest;
	};
};

// GMC->GMS:Ҫ���ɢ����------------------------------------------------------------------------------
struct PGPlayerControl_CtoS_DismissGuild  : public S_ObjNetPktBase
{
	int  iWorldID;				// ������
	char szGuildName[32];		// ��������

	PGPlayerControl_CtoS_DismissGuild()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_DismissGuild;
	};
};

// GMS->GMC:�ش���ɢ������--------------------------------------------------------------------------
struct PGPlayerControl_StoC_DismissGuild : public S_ObjNetPktBase
{
	ENUM_DismissGuildResult emResult;	// ����о�
	int  iWorldID;								// ������
	char szGuildName[32];						// ��������

	PGPlayerControl_StoC_DismissGuild()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_DismissGuild;
	};
};
// GMC->GMS:Ҫ��ȡ�ù��������Ѷ----------------------------------------------------------------------
struct PGPlayerStatus_CtoS_GuildBaseInfo : public S_ObjNetPktBase
{
	int		iWorldID;				// ������
	char	szGuildName[32];		// ��������

	PGPlayerStatus_CtoS_GuildBaseInfo()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_GuildBaseInfo;
	};
};

// GMS->GMC:�ش�ȡ�ù��������Ѷ���------------------------------------------------------------------
struct PGPlayerStatus_StoC_GuildBaseInfo : public S_ObjNetPktBase
{
	ENUM_GuildBaseInfoResult emResult;		// ����о�
	int	 iWorldID;							// ������
	char szGuildName[32];					// ��������
	int  iLevel;							// ����ȼ�
	int  iScore;							// �������
	int  iMoney;							// �����Ǯ

	PGPlayerStatus_StoC_GuildBaseInfo()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_GuildBaseInfo;
	};
};
// GMC->GMS:Ҫ��ȡ�ù����Ա�б�----------------------------------------------------------------------
struct PGPlayerStatus_CtoS_GuildMemberList : public S_ObjNetPktBase
{
	int		iWorldID;				// ������
	char	szGuildName[32];		// ��������

	PGPlayerStatus_CtoS_GuildMemberList()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_GuildMemberList;
	};
};

// GMS->GMC:�ش�ȡ�ù����Ա�б���------------------------------------------------------------------
struct PGPlayerStatus_StoC_GuildMemberList : public S_ObjNetPktBase
{
	ENUM_GuildMemberListResult emResult;	// ����о�
	int	 iWorldID;							// ������,-1����������
	char szGuildName[32];					// ��������
	char szRoleName[32];					// ��Ա��ɫ����
	int  iLevel;							// ��Աְ��
	int  iJoinDate;							// ����ʱ��

	PGPlayerStatus_StoC_GuildMemberList()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_GuildMemberList;
	};
};
// GMC->GMS:Ҫ���޸Ľ�ɫ����--------------------------------------------------------------------------
struct PGPlayerControl_CtoS_ModifyCharacterQuest  : public S_ObjNetPktBase
{
	int	 iWorldID;			// ������
	char szRoleName[32];	// ��ɫ����
	int	 iQuestID;			// ������(0~6143)
	int	 iQuestStatus;		// ����״̬(0:δ�� 1:����� 2:�ѽ�)

	PGPlayerControl_CtoS_ModifyCharacterQuest()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_ModifyCharacterQuest;
	};
};

// GMS->GMC:�ش��޸Ľ�ɫ������----------------------------------------------------------------------
struct PGPlayerControl_StoC_ModifyCharacterQuest : public S_ObjNetPktBase
{
	ENUM_ModifyCharacterQuestResult emResult;	// ����о�
	int  iWorldID;								// ������
	char szRoleName[32];						// ��ɫ����
	int	 iQuestID;								// ������(0~6143)
	int	 iQuestStatus;							// ����״̬(0:δ�� 1:����� 2:�ѽ�)

	PGPlayerControl_StoC_ModifyCharacterQuest()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_ModifyCharacterQuest;
	};
};
// GMC->GMS:Ҫ���޸Ĺ����Աְ��----------------------------------------------------------------------
struct PGPlayerControl_CtoS_ModifyGuildMemberPL  : public S_ObjNetPktBase
{
	int  iWorldID;				// ������
	char szGuildName[32];		// ��������
	char szRoleName[32];		// ��Ա��ɫ����
	int  iLevel;				// ְ��(0:��Ա 1:�����ɲ� 2:һ���ɲ� 3:���᳤ 4:�᳤)

	PGPlayerControl_CtoS_ModifyGuildMemberPL()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_ModifyGuildMemberPL;
	};
};

// GMS->GMC:�ش��޸Ĺ����Աְ�׽��------------------------------------------------------------------
struct PGPlayerControl_StoC_ModifyGuildMemberPL : public S_ObjNetPktBase
{
	ENUM_ModifyGuildMemberPLResult emResult;	// ����о�
	int  iWorldID;								// ������
	char szGuildName[32];						// ��������
	char szRoleName[32];						// ��Ա��ɫ����
	int  iLevel;								// ְ��(0:��Ա 1:�����ɲ� 2:һ���ɲ� 3:���᳤ 4:�᳤)

	PGPlayerControl_StoC_ModifyGuildMemberPL()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_ModifyGuildMemberPL;
	};
};
// GMC->GMS:Ҫ���޸Ĺ���ȼ�--------------------------------------------------------------------------
struct PGPlayerControl_CtoS_ModifyGuildLv  : public S_ObjNetPktBase
{
	int  iWorldID;				// ������
	char szGuildName[32];		// ��������
	int	 iLevel;				// �ȼ�(1~25)
	
	PGPlayerControl_CtoS_ModifyGuildLv()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_ModifyGuildLv;
	};
};

// GMS->GMC:�ش��޸Ĺ���ȼ����----------------------------------------------------------------------
struct PGPlayerControl_StoC_ModifyGuildLv : public S_ObjNetPktBase
{
	ENUM_ModifyGuildLvResult emResult;	// ����о�
	int  iWorldID;						// ������
	char szGuildName[32];				// ��������
	int	 iLevel;						// �ȼ�(1~25)
	
	PGPlayerControl_StoC_ModifyGuildLv()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_ModifyGuildLv;
	};
};

// GMC->GMS:Ҫ���޸Ĺ�������--------------------------------------------------------------------------
struct PGPlayerControl_CtoS_ModifyGuildName  : public S_ObjNetPktBase
{
	int  iWorldID;				// ������
	char szGuildName[32];		// ԭ��������
	char szNewName[32];			// �¹�������

	PGPlayerControl_CtoS_ModifyGuildName()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_ModifyGuildName;
	};
};

// GMS->GMC:�ش��޸Ĺ������ƽ��----------------------------------------------------------------------
struct PGPlayerControl_StoC_ModifyGuildName : public S_ObjNetPktBase
{
	ENUM_ModifyGuildNameResult emResult;	// ����о�
	int  iWorldID;							// ������
	char szGuildName[32];					// ԭ��������
	char szNewName[32];						// �¹�������

	PGPlayerControl_StoC_ModifyGuildName()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_ModifyGuildName;
	};
};

// GMC->GMS:Ҫ���޸Ĺ��ṫ��--------------------------------------------------------------------------
struct PGPlayerControl_CtoS_ModifyGuildNotice  : public S_ObjNetPktBase
{
	int  iWorldID;				// ������
	char szGuildName[32];		// ��������
	char szNotice[512];			// ����
	
	PGPlayerControl_CtoS_ModifyGuildNotice()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_ModifyGuildNotice;
	};
};

// GMS->GMC:�ش��޸Ĺ��ṫ����----------------------------------------------------------------------
struct PGPlayerControl_StoC_ModifyGuildNotice : public S_ObjNetPktBase
{
	ENUM_ModifyGuildNoticeResult emResult;	// ����о�
	int  iWorldID;							// ������
	char szGuildName[32];					// ��������
	char szNotice[512];						// ����
	
	PGPlayerControl_StoC_ModifyGuildNotice()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_ModifyGuildNotice;
	};
};

// GMC->GMS:Ҫ���޸Ľ�ɫ��Ǯ--------------------------------------------------------------------------
struct PGPlayerControl_CtoS_ModifyCharacterMoney  : public S_ObjNetPktBase
{
	int	 iWorldID;			// ������
	char szRoleName[32];	// ��ɫ����
	int	 iType;				// ����(0:���� 1:����)
	int  iCount;			// ���ӽ��(��Ϊ������������)
	
	PGPlayerControl_CtoS_ModifyCharacterMoney()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_ModifyCharacterMoney;
	};
};

// GMS->GMC:�ش��޸Ľ�ɫ��Ǯ���----------------------------------------------------------------------
struct PGPlayerControl_StoC_ModifyCharacterMoney : public S_ObjNetPktBase
{
	ENUM_ModifyCharacterMoneyResult emResult;	// ����о�
	int	 iWorldID;								// ������
	char szRoleName[32];						// ��ɫ����
	int	 iType;									// ����(0:���� 1:����)
	int  iCount;								// ���ӽ��(��Ϊ������������)
	
	PGPlayerControl_StoC_ModifyCharacterMoney()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_ModifyCharacterMoney;
	};
};

// GMC->GMS:Ҫ���޸Ľ�ɫɾ������----------------------------------------------------------------------
struct PGPlayerControl_CtoS_ModifyCharacterDropPw  : public S_ObjNetPktBase
{
	int	 iWorldID;			// ������
	char szRoleName[32];	// ��ɫ����
	char szDropRolePw[13];	// ��ɾ������(6~12Ӣ����,���һ����Ԫ�����ǽ�����Ԫ)
	
	PGPlayerControl_CtoS_ModifyCharacterDropPw()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_ModifyCharacterDropPw;
	};
};

// GMS->GMC:�ش��޸Ľ�ɫɾ��������------------------------------------------------------------------
struct PGPlayerControl_StoC_ModifyCharacterDropPw : public S_ObjNetPktBase
{
	ENUM_ModifyCharacterDropPwResult emResult;	// ����о�
	int	 iWorldID;								// ������
	char szRoleName[32];						// ��ɫ����
	
	PGPlayerControl_StoC_ModifyCharacterDropPw()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_ModifyCharacterDropPw;
	};
};

// GMC->GMS:Ҫ���޸��ʺ���������----------------------------------------------------------------------
struct PGPlayerControl_CtoS_ModifyAccountBankPw  : public S_ObjNetPktBase
{
	int	 iWorldID;			// ������
	char szAccount[32];   // �ʺ�
	char szBankPw[7];		// ����������(6λӢ����)
	
	PGPlayerControl_CtoS_ModifyAccountBankPw()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_ModifyAccountBankPw;
	};
};

// GMS->GMC:�ش��޸��ʺ�����������------------------------------------------------------------------
struct PGPlayerControl_StoC_ModifyAccountBankPw : public S_ObjNetPktBase
{
	ENUM_ModifyAccountBankPwResult emResult;	// ����о�
	int	 iWorldID;								// ������
	char szAccount[32];						// ��ɫ����
	
	PGPlayerControl_StoC_ModifyAccountBankPw()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_ModifyAccountBankPw;
	};
};

// GMC->GMS:Ҫ��ɾ����ɫ��Ʒ--------------------------------------------------------------------------
struct PGPlayerControl_CtoS_DelCharacterItem  : public S_ObjNetPktBase
{
	int	 iWorldID;			// ������
	char szRoleName[32];	// ��ɫ����
	int	 iType;				// ����(0:���� 1:����� 2:PK�������� 3:����)
	int	 iPosition;			// λ��(��Χ:���ϱ���0~99��Ʒ��,100~115װ����,116~118������Ʒ��,
	//						 �����0~4,:PK��������0~4,����0~49)
	
	PGPlayerControl_CtoS_DelCharacterItem()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_DelCharacterItem;
	};
};

// GMS->GMC:�ش�ɾ����ɫ��Ʒ���----------------------------------------------------------------------
struct PGPlayerControl_StoC_DelCharacterItem : public S_ObjNetPktBase
{
	ENUM_DelCharacterItemResult emResult;	// ����о�
	int	 iWorldID;							// ������
	char szRoleName[32];					// ��ɫ����
	int	 iType;								// ����(0:���� 1:����� 2:PK�������� 3:����)
	int	 iPosition;							// λ��
	
	PGPlayerControl_StoC_DelCharacterItem()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_DelCharacterItem;
	};
};
// GMC->GMS:Ҫ��ȡ���ʺ���Ѷ--------------------------------------------------------------------------
struct PGPlayerStatus_CtoS_AccountInfo : public S_ObjNetPktBase
{
	int  iWorldID;					// ������
	char szAccountName[32];			// �ʺ�����
	
	PGPlayerStatus_CtoS_AccountInfo()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_AccountInfo;
	};
};

// GMS->GMC:�ش�ȡ���ʺ���Ѷ���----------------------------------------------------------------------
struct PGPlayerStatus_StoC_AccountInfo : public S_ObjNetPktBase
{
	ENUM_AccountInfoResult emResult;			// ����о�
	int	 iWorldID;								// ������
	int	 iAccountID;							// �ʺ�ID
	char szAccountName[32];						// �ʺ�����
	int  iMaxRoleCount;							// �ɽ�����ɫ��
	S_GMAccountRoleInfo sGMAccountRoleInfo[8];	// ��ɫ�б�
	
	PGPlayerStatus_StoC_AccountInfo()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_AccountInfo;
	};
};

// GMC->GMS:Ҫ��ȡ�ý�ɫ������Ѷ----------------------------------------------------------------------
struct PGPlayerStatus_CtoS_CharacterAdvanceInfo : public S_ObjNetPktBase
{
	int  iWorldID;					// ������
	char szRoleName[32];			// ��ɫ����
	
	PGPlayerStatus_CtoS_CharacterAdvanceInfo()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_CharacterAdvanceInfo;
	};
};

// GMS->GMC �ش�ȡ�ý�ɫ������Ѷ(����,֪ʶ,����)���--------------------------------------------------
struct PGPlayerStatus_StoC_CharacterAdvanceInfo : public S_ObjNetPktBase
{
	ENUM_CharacterAdvanceInfoResult emResult;	// ����о�
	int	 iWorldID;								// ������,-1����������
	char szAccount[32];							// �ʺ�
	char szRoleName[32];						// ��ɫ����
	int	 iPlotFame[20];							// ������Ѷ����
	
	PGPlayerStatus_StoC_CharacterAdvanceInfo()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_CharacterAdvanceInfo;
	};
};

// GMC->GMS:Ҫ��ȡ�ý�ɫ�������б�--------------------------------------------------------------------
struct PGPlayerStatus_CtoS_CharacterBlackList : public S_ObjNetPktBase
{
	int	 iWorldID;			// ������
	char szRoleName[32];	// ��ɫ����
	
	PGPlayerStatus_CtoS_CharacterBlackList()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_CharacterBlackList;
	};
};

// GMS->GMC:�ش�ȡ�ý�ɫ�������б���----------------------------------------------------------------
struct PGPlayerStatus_StoC_CharacterBlackList : public S_ObjNetPktBase
{
	ENUM_CharacterBlackListResult emResult;		// ����о�
	int	 iWorldID;								// ������,-1����������
	char szAccount[32];							// �ʺ�
	char szRoleName[32];						// ��ɫ����
	char szBlackList[15][32];					// �������б�
	
	PGPlayerStatus_StoC_CharacterBlackList()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_CharacterBlackList;
	};
};

// GMC->GMS:Ҫ��ȡ�ý�ɫ��ħ��Ѷ----------------------------------------------------------------------
struct PGPlayerStatus_CtoS_CharacterMob : public S_ObjNetPktBase
{
	int  iWorldID;					// ������
	char szRoleName[32];			// ��ɫ����
	
	PGPlayerStatus_CtoS_CharacterMob()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_CharacterMob;
	};
};

// GMS->GMC �ش�ȡ�ý�ɫ��ħ��Ѷ(����,֪ʶ,����)���--------------------------------------------------
struct PGPlayerStatus_StoC_CharacterMob : public S_ObjNetPktBase
{
	ENUM_CharacterMobResult emResult;	// ����о�
	int	 iWorldID;						// ������,-1����������
	char szAccount[32];					// �ʺ�
	char szRoleName[32];				// ��ɫ����
	int  dwMobFlag[32];					// ��ħ���
	
	PGPlayerStatus_StoC_CharacterMob()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_CharacterMob;
	};
};
// GMC->GMS:Ҫ��ȡ�ý�ɫͼ����Ѷ----------------------------------------------------------------------
struct PGPlayerStatus_CtoS_CharacterPuzzleMap : public S_ObjNetPktBase
{
	int  iWorldID;					// ������
	char szRoleName[32];			// ��ɫ����
	
	PGPlayerStatus_CtoS_CharacterPuzzleMap()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_CharacterPuzzleMap;
	};
};

// GMS->GMC �ش�ȡ�ý�ɫͼ����Ѷ(����,֪ʶ,����)���--------------------------------------------------
struct PGPlayerStatus_StoC_CharacterPuzzleMap : public S_ObjNetPktBase
{
	ENUM_CharacterPuzzleMapResult emResult;	// ����о�
	int	 iWorldID;							// ������,-1����������
	char szAccount[32];						// �ʺ�
	char szRoleName[32];					// ��ɫ����
	int  dwPuzzleMapFlag[64];				// ͼ�����
	
	PGPlayerStatus_StoC_CharacterPuzzleMap()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_CharacterPuzzleMap;
	};
};
// GMC->GMS:Ҫ��ȡ�ý�ɫ������Ѷ----------------------------------------------------------------------
struct PGPlayerStatus_CtoS_CharacterSkill : public S_ObjNetPktBase
{
	int  iWorldID;					// ������
	char szRoleName[32];			// ��ɫ����
	
	PGPlayerStatus_CtoS_CharacterSkill()
	{
		uiCmdID = ENUM_PGPlayerStatus_CtoS_CharacterSkill;
	};
};

// GMS->GMC �ش�ȡ�ý�ɫ������Ѷ(����,֪ʶ,����)���--------------------------------------------------
struct PGPlayerStatus_StoC_CharacterSkill : public S_ObjNetPktBase
{
	ENUM_CharacterSkillResult emResult;		// ����о�
	int	 iWorldID;							// ������,-1����������
	char szAccount[32];						// �ʺ�
	char szRoleName[32];					// ��ɫ����
	int  iVocType;							// ְҵ����(0��,1��,2��,3��,4��,5��)
	int	 iSkillPoint;						// �����ü��ܵ���
	int  iVocSkillID[100];					// �������
	S_GMLifeHoodSkill sLifeHoodSkill;		// ����������Ѷ
	
	PGPlayerStatus_StoC_CharacterSkill()
	{
		uiCmdID = ENUM_PGPlayerStatus_StoC_CharacterSkill;
	};
};
// GMC->GMS:Ҫ��ɾ����ɫ��ħ--------------------------------------------------------------------------
struct PGPlayerControl_CtoS_DelCharacterMob  : public S_ObjNetPktBase
{
	int	 iWorldID;			// ������
	char szRoleName[32];	// ��ɫ����
	int  iMobID;			// ������
	int  iFlagID;			// �����
	
	PGPlayerControl_CtoS_DelCharacterMob()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_DelCharacterMob;
	};
};

// GMS->GMC:�ش�ɾ����ɫ��ħ���----------------------------------------------------------------------
struct PGPlayerControl_StoC_DelCharacterMob : public S_ObjNetPktBase
{
	ENUM_DelCharacterMobResult emResult;	// ����о�
	int	 iWorldID;							// ������
	char szRoleName[32];					// ��ɫ����
	int  iMobID;							// ������
	
	PGPlayerControl_StoC_DelCharacterMob()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_DelCharacterMob;
	};
};
// GMC->GMS:Ҫ���޸Ľ�ɫͼ��--------------------------------------------------------------------------
struct PGPlayerControl_CtoS_ModifyPuzzleMap  : public S_ObjNetPktBase
{
	int	 iWorldID;							// ������
	char szRoleName[32];					// ��ɫ����
	S_GMPuzzleMapProc sGMPuzzleMapProc;		// ͼ��������Ѷ
	
	PGPlayerControl_CtoS_ModifyPuzzleMap()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_ModifyPuzzleMap;
	};
};

// GMS->GMC:�ش��޸Ľ�ɫͼ�����----------------------------------------------------------------------
struct PGPlayerControl_StoC_ModifyPuzzleMap : public S_ObjNetPktBase
{
	ENUM_ModifyPuzzleMapResult emResult;	// ����о�
	int	 iWorldID;							// ������
	char szRoleName[32];					// ��ɫ����
	S_GMPuzzleMapProc sGMPuzzleMapProc;		// ͼ��������Ѷ
	
	PGPlayerControl_StoC_ModifyPuzzleMap()
	{
		uiCmdID = ENUM_PGPlayerControl_StoC_ModifyPuzzleMap;
	};
};

// GMC->GMS:Ҫ���������빦�ܿ����б�------------------------------------------------------------------
struct PGServerControl_CtoS_ReloadFunctionSwitch  : public S_ObjNetPktBase
{
	int iWorldID;		// ������
	
	PGServerControl_CtoS_ReloadFunctionSwitch()
	{
		uiCmdID = ENUM_PGServerControl_CtoS_ReloadFunctionSwitch;
	};
};

// GMS->GMC:�ش��������빦�ܿ����б���--------------------------------------------------------------
struct PGServerControl_StoC_ReloadFunctionSwitch : public S_ObjNetPktBase
{
	ENUM_ReloadFunctionSwitchResult emResult;	// ����о�
	int iWorldID;								// ������
	
	PGServerControl_StoC_ReloadFunctionSwitch()
	{
		uiCmdID = ENUM_PGServerControl_StoC_ReloadFunctionSwitch;
	};
};


// GMC->GMS:Ҫ���ƶ���ɫ����ȫ��----------------------------------------------------------------------
struct PGPlayerControl_CtoS_MoveCharacterToSafe  : public S_ObjNetPktBase
{
	int  iWorldID;			// ������
	char szRoleName[32];	// ��ɫ����
	int	 iZoneID;			// ������
	int  X, Y, Z;			// ����λ��
	
	PGPlayerControl_CtoS_MoveCharacterToSafe()
	{
		uiCmdID = ENUM_PGPlayerControl_CtoS_MoveCharacterToSafe;
	};
};

// GMS->GMC:�ش��ƶ���ɫ����ȫ����------------------------------------------------------------------
struct PGPlayerControl_StoC_MoveCharacterToSafe : public S_ObjNetPktBase
{
	ENUM_MoveCharacterToSafeResult emResult;	// ����о�
	int  iWorldID;								// ������
	char szRoleName[32];						// ��ɫ����
	int	 iZoneID;								// ������
	
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
// GMC->GMS:Ҫ����Ϳ�����Ʒ�ʼ�--------------------------------------------------------------------------
struct PGServerControl_CtoS_SendCustomItemMail  : public S_ObjNetPktBase
{
	int  iWorldID;			// ������
	char szRoleName[32];	// ��ɫ����
	char szSubject[32];		// �ʼ���ּ
	char szContent[256];	// �ʼ�����
	int  iMoney;			// �ʼ���Ǯ
	int  iCustomItemID;		// ������Ʒ���
	bool bBindSetup;		// ��״̬(1:��Ϊ�� 0:ά��ԭ�����趨)
	int  iInherit[10];		// ��ʯ����,�������������������(��ʹ������-1):
	// 0:����ֵ(����) 1:��ħ, 2:������, 3:�巨��, 4~9:��ʯ1~6
	char szReason[1024];	// ���ͽ�Ʒԭ��
	
	PGServerControl_CtoS_SendCustomItemMail()
	{
		uiCmdID = ENUM_PGServerControl_CtoS_SendCustomItemMail;
	};
};

// GMS->GMC:�ش����Ϳ�����Ʒ�ʼ����----------------------------------------------------------------------
struct PGServerControl_StoC_SendCustomItemMail : public S_ObjNetPktBase
{
	ENUM_SendCustomItemMailResult emResult;	// ����о�
	int  iWorldID;							// ������
	char szRoleName[32];					// ��ɫ����
	char szSubject[32];						// �ʼ���ּ
	int  iMoney;							// �ʼ���Ǯ
	int  iCustomItemID;						// ������Ʒ���
	bool bBindSetup;						// ��״̬(1:��Ϊ�� 0:ά��ԭ�����趨)
	int  iInherit[10];						// ��ʯ����
	
	PGServerControl_StoC_SendCustomItemMail()
	{
		uiCmdID = ENUM_PGServerControl_StoC_SendCustomItemMail;
	};
};

