#pragma once

#include "AdminDef.h"

#pragma pack(push, 1)

namespace MSG_ATAS
{
	// ��ѯ��
	const int CMD_REQ_LOGIN					= ADMIN::MSG_BASE_ATAS + 1;		// ��¼
	const int CMD_REQ_CHARACTER_INFO		= ADMIN::MSG_BASE_ATAS + 2;		// ��ѯ��ɫ��Ϣ
	const int CMD_REQ_CLOSURE_ACCOUNT_INFO	= ADMIN::MSG_BASE_ATAS + 3;		// ��ѯ�ʺ���Ϣ

	// ������
	const int CMD_REQ_KICK_USER				= ADMIN::MSG_BASE_ATAS + 100;	// ����
	const int CMD_REQ_CLOSE_ACCOUNT			= ADMIN::MSG_BASE_ATAS + 101;	// ��ͣ
	const int CMD_REQ_AVAILABLE_ACCOUNT		= ADMIN::MSG_BASE_ATAS + 102;	// ���
	const int CMD_REQ_SYSTEM_BROADCAST		= ADMIN::MSG_BASE_ATAS + 103;	// �㲥
	const int CMD_REQ_DELETE_ITEM			= ADMIN::MSG_BASE_ATAS + 104;	// ɾ����Ʒ
	const int CMD_REQ_MODIFY_CHARACTER		= ADMIN::MSG_BASE_ATAS + 105;	// ��Ǯ,����,�ĵȼ�
	const int CMD_REQ_MODIFY_CHARACTER_SKILL= ADMIN::MSG_BASE_ATAS + 106;	// �޸Ľ�ɫ����
	const int CMD_REQ_RECOVER_CHRACTER		= ADMIN::MSG_BASE_ATAS + 107;	// �ָ���ɫ
	const int CMD_REQ_MODIFY_ACCOUNT_PASSW	= ADMIN::MSG_BASE_ATAS + 108;	// �޸�����
	const int CMD_REQ_MAIL_ITEM				= ADMIN::MSG_BASE_ATAS + 109;	// ����ɫ�ʼ���Ʒ
	const int CMD_REQ_MODIFY_CHARACTER_NAME	= ADMIN::MSG_BASE_ATAS + 110;	// ����ɫ����

	// ��־��ѯ
	const int CMD_REQ_LOG_MONSTER_DROP_ITEM	= ADMIN::MSG_BASE_ATAS + 250;	// ��Ʒ:�������
	const int CMD_REQ_LOG_BUY_ITEM_FROM_NPC	= ADMIN::MSG_BASE_ATAS + 251;	// ��Ʒ:��NPC������
	const int CMD_REQ_LOG_SELL_ITEM_TO_NPC	= ADMIN::MSG_BASE_ATAS + 252;	// ��Ʒ:����NPC
	const int CMD_REQ_LOG_DROP_ITEM			= ADMIN::MSG_BASE_ATAS + 253;	// ��Ʒ:�������ɾ��
	const int CMD_REQ_LOG_GET_ITEM_TRADE	= ADMIN::MSG_BASE_ATAS + 254;	// ��Ʒ:���׻��
	const int CMD_REQ_LOG_LOSE_ITEM_TRADE	= ADMIN::MSG_BASE_ATAS + 255;	// ��Ʒ:����ʧȥ
	const int CMD_REQ_LOG_BIND_ITEM			= ADMIN::MSG_BASE_ATAS + 256;	// ��Ʒ:װ���󶨼�¼
	const int CMD_REQ_LOG_UNBIND_ITEM		= ADMIN::MSG_BASE_ATAS + 257;	// ��Ʒ:װ����󶨼�¼
	const int CMD_REQ_LOG_BOOTH_ITEM		= ADMIN::MSG_BASE_ATAS + 258;	// ��Ʒ:��ɫ����̯����־��ѯ
	const int CMD_REQ_LOG_DELETE_ITEM		= ADMIN::MSG_BASE_ATAS + 259;	// ��Ʒ:����ɾ����¼(ָʹ��ʱ�䵽��)
	const int CMD_REQ_LOG_USE_ITEM			= ADMIN::MSG_BASE_ATAS + 260;	// ��Ʒ:����ʹ�ü�¼
	const int CMD_REQ_LOG_CRAFT_ITEM		= ADMIN::MSG_BASE_ATAS + 261;	// ��Ʒ:װ��������¼
	const int CMD_REQ_LOG_PUNCH_ITEM		= ADMIN::MSG_BASE_ATAS + 262;	// ��Ʒ:װ�����
	const int CMD_REQ_LOG_CHANGE_SLOT_ITEM	= ADMIN::MSG_BASE_ATAS + 263;	// ��Ʒ:װ���Ŀ�
	const int CMD_REQ_LOG_UPGRADE_STAR		= ADMIN::MSG_BASE_ATAS + 264;	// ��Ʒ:װ������
	const int CMD_REQ_LOG_GEM				= ADMIN::MSG_BASE_ATAS + 265;	// ��Ʒ:��ʯ
	const int CMD_REQ_LOG_PHARMA			= ADMIN::MSG_BASE_ATAS + 266;	// ��Ʒ:��ҩ��¼
	const int CMD_REQ_LOG_CRAFT_GEM			= ADMIN::MSG_BASE_ATAS + 267;	// ��Ʒ:��ʯ�ϳ�
	const int CMD_REQ_LOG_CRAFT_MATIRAIL	= ADMIN::MSG_BASE_ATAS + 268;	// ��Ʒ:���Ϻϳ�
	const int CMD_REQ_LOG_CRAFT_RECIPE		= ADMIN::MSG_BASE_ATAS + 269;	// ��Ʒ:ͼֽ�ϳ�
	const int CMD_REQ_LOG_MAZE_INTEGRAL		= ADMIN::MSG_BASE_ATAS + 270;	// �Թ����ֶһ�����
	const int CMD_REQ_LOG_MONSTER_DROP_MONEY= ADMIN::MSG_BASE_ATAS + 271;	// ��Ϸ��:�������
	const int CMD_REQ_LOG_GET_MONEY_QUEST	= ADMIN::MSG_BASE_ATAS + 272;	// ��Ϸ��:������
	const int CMD_REQ_LOG_LOSE_MONEY_REPAIR	= ADMIN::MSG_BASE_ATAS + 273;	// ��Ϸ��:��װ
	const int CMD_REQ_LOG_GET_MONEY_TRADE	= ADMIN::MSG_BASE_ATAS + 274;	// ��Ϸ��:���׻��
	const int CMD_REQ_LOG_LOSE_MONEY_TRADE	= ADMIN::MSG_BASE_ATAS + 275;	// ��Ϸ��:����ʧȥ
	const int CMD_REQ_LOG_QUEST_REWARD		= ADMIN::MSG_BASE_ATAS + 276;	// ����:��ɻ�ü�¼
	const int CMD_REQ_LOG_QUEST_GIVE		= ADMIN::MSG_BASE_ATAS + 277;	// ����:������Ʒɾ����¼
	const int CMD_REQ_LOG_MAIL_SEND			= ADMIN::MSG_BASE_ATAS + 278;	// �ʼ�:���ͼ�¼
	const int CMD_REQ_LOG_MAIL_RECV			= ADMIN::MSG_BASE_ATAS + 279;	// �ʼ�:���ռ�¼
	const int CMD_REQ_LOG_MAIL_DELETE		= ADMIN::MSG_BASE_ATAS + 280;	// �ʼ�:ɾ����¼
	const int CMD_REQ_LOG_ITEM_SHOP			= ADMIN::MSG_BASE_ATAS + 281;	// �̳�:�����¼
	const int CMD_REQ_LOG_INSTANCE			= ADMIN::MSG_BASE_ATAS + 282;	// ��ѯ�û��ĸ�����־
	const int CMD_REQ_LOG_LOGIN_LOGOUT		= ADMIN::MSG_BASE_ATAS + 283;	// ��ҵ�½/�ǳ���־

	//
	// ��¼��as
	// 
	struct REQ_LOGIN : public ADMIN::TCP_MSG_BASE
	{
		unsigned int	srv_realm_id;								// ����ID
		char			loginUserName[ADMIN::MAX_ID_LENGTH];		// ��¼�û���
		char			loginPassWord[ADMIN::MAX_PASSWORD_LENGTH];	// ��¼����

		REQ_LOGIN()
			:TCP_MSG_BASE(CMD_REQ_LOGIN)
		{
			loginUserName[0] = '\0';
			loginPassWord[0] = '\0';
		}
	};

	//
	// ��ѯ��ɫ��Ϣ
	//
	struct REQ_CHARACTER_INFO : public ADMIN::TCP_MSG_BASE
	{
		// ��ѯ����
		enum REQ_TYPE
		{
			RT_INVALID				= 0,
			RT_CHAR_BASE_INFO		= 1,			// ��ɫ������Ϣ
			RT_CHAR_EQUIPMENT_INFO	= 2,			// ��ɫװ��
			RT_CHAR_ITEM_INFO		= 3,			// ��ɫ��Ʒ��Ϣ
			RT_CHAR_BANK_INFO		= 4,			// ��ɫ������Ϣ
			RT_CHAR_MEDAL_INFO		= 5,			// ��ɫ������Ϣ(��ʱ����)
			RT_CHAR_PET_INFO		= 6,			// ��ɫ������Ϣ
			RT_CHAR_FRIEND_INFO		= 7,			// ��ɫ������Ϣ
			RT_CHAR_SKILL_INFO		= 8,			// ��ɫ������Ϣ
		};

		unsigned int			srv_realm_id;		// ����id
		unsigned int			srv_cluster_id;		// ��������id
		unsigned int			param1;				// ����
		REQ_TYPE				req_type;			// ��ѯ����
		ADMIN::QUERY_CONDITION	query_condition;	// ��ѯ����

		union
		{
			ADMIN::QueryUserName		user_name;	// �û��ʺ�
			ADMIN::QueryCharacterName	char_name;	// ��ɫ����
		};

		REQ_CHARACTER_INFO()
			:TCP_MSG_BASE(CMD_REQ_CHARACTER_INFO),req_type(RT_INVALID),
			query_condition(ADMIN::QC_INVALID)
		{}
	};

	//
	// ��ѯ�ʺŷ�ͣ��Ϣ
	//
	struct REQ_CLOSURE_ACCOUNT_INFO : public ADMIN::TCP_MSG_BASE
	{
		// ��ѯ��Χ
		enum REQ_RANGE
		{
			RR_INVALID = 0,
			RR_PER_CLOSUREACCOUNT,				// ������ͣ�ʺ�	
			RR_ALL_CLOSUREACCOUNT,				// ���з�ͣ�ʺ�
		};

		unsigned int			srv_realm_id;	// ����id
		unsigned int			srv_cluster_id;	// ��������id
		REQ_RANGE				reqRange;		// ��ѯ��Χ
		ADMIN::QueryUserName	user_name;		// ��ѯ������ͣ�ʺ�ʱ�õ�

		REQ_CLOSURE_ACCOUNT_INFO()
			:TCP_MSG_BASE(CMD_REQ_CLOSURE_ACCOUNT_INFO),reqRange(RR_INVALID)
		{
			user_name.userName[0] = '\0';
		}
	};


	//
	// ��������
	// 
	struct REQ_KICK_USER : public ADMIN::TCP_MSG_BASE
	{
		unsigned int			srv_realm_id;		// ����id
		unsigned int			srv_cluster_id;		// ��������id
		ADMIN::QUERY_CONDITION	kickCondition;		// ��ѯ����
		union
		{
			ADMIN::QueryUserName		user_name;	// �û��ʺ�
			ADMIN::QueryCharacterName	char_name;	// ��ɫ����
		};

		REQ_KICK_USER()
			:TCP_MSG_BASE(CMD_REQ_KICK_USER),kickCondition(ADMIN::QC_INVALID)
		{}
	};

	//
	// ��ͣ�ʺ�
	//
	struct REQ_CLOSE_ACCOUNT : public ADMIN::TCP_MSG_BASE
	{
		// ��ͣԭ��
		enum CLOSE_ACCOUNT_REASON
		{
			R_UNKNOWN	= 0,
			R_SYSTEM,									// ϵͳ��ͣ
			R_SYSTEM_TEMP,								// ϵͳ��ʱ��ͣ
			R_USERCUSTOM,								// �û�����
			R_USERCUSTOM_TEMP,							// �û���ʱ����
		};

		unsigned int			srv_realm_id;			// ����id
		unsigned int			srv_cluster_id;			// ��������id
		unsigned int			duration_time;			// ��ͣʱ��(�����Ӽ���)
		CLOSE_ACCOUNT_REASON	eResult;				// ��ͣԭ��
		ADMIN::QUERY_CONDITION	closeCondition;			// ��ѯ����
		char					description[128 + 1];	// ����

		union
		{
			ADMIN::QueryUserName		user_name;		// �û��ʺ�
			ADMIN::QueryCharacterName	char_name;		// ��ɫ����
		};

		REQ_CLOSE_ACCOUNT()
			:TCP_MSG_BASE(CMD_REQ_CLOSE_ACCOUNT),closeCondition(ADMIN::QC_INVALID)
		{}
	};

	//
	// ����ʺ�
	//
	struct REQ_AVAILABLE_ACCOUNT : public ADMIN::TCP_MSG_BASE
	{
		unsigned int			srv_realm_id;			// ����id
		unsigned int			srv_cluster_id;			// ��������id
		ADMIN::QUERY_CONDITION	availableCondition;		// ��ѯ����
		union
		{
			ADMIN::QueryUserName		user_name;		// �û��ʺ�
			ADMIN::QueryCharacterName	char_name;		// ��ɫ����
		};

		REQ_AVAILABLE_ACCOUNT()
			:TCP_MSG_BASE(CMD_REQ_AVAILABLE_ACCOUNT),availableCondition(ADMIN::QC_INVALID)
		{}
	};

	//
	// ϵͳ�㲥(������ݰ�,�������ݰ�ʱ ���ݰ����ȵ���GetSize()���)
	//
	struct REQ_SYSTEM_BROADCAST : public ADMIN::TCP_MSG_BASE
	{
		// �㲥����
		enum SYSTEM_BROADCAST_TYPE
		{
			T_INVALID = 0,
			T_NOW,			// ���Ϲ㲥
			T_DELAY,		// �ӳٹ㲥
		};

		unsigned int			srv_realm_id;		// ����id
		unsigned int			srv_cluster_id;		// ��������id
		SYSTEM_BROADCAST_TYPE	bType;				// �㲥����
		unsigned short			broadcastSize;		// �㲥��Ϣ����
		unsigned short			start_month;		// ��ʼ�㲥ʱ��(�ӳٹ㲥�õ�)
		unsigned short			start_day;
		unsigned short			start_hour;
		unsigned short			start_minute;
		unsigned short			finish_month;		// �����㲥ʱ��(�ӳٹ㲥�õ�)
		unsigned short			finish_day;
		unsigned short			finish_hour;
		unsigned short			finish_minute;
		unsigned short			interval;			// �㲥���(����)(�ӳٹ㲥�õ�)
		char					broadcastTxt[512];	// �㲥����

		REQ_SYSTEM_BROADCAST()
			:TCP_MSG_BASE(CMD_REQ_SYSTEM_BROADCAST)
		{
			broadcastSize = (unsigned short)(sizeof(REQ_SYSTEM_BROADCAST) - 512);
		}

		// ��ӹ㲥��Ϣ����
		bool AddBroadcaseTxt(const char* pTxt)
		{
			unsigned short len = (unsigned short)strlen(pTxt);
			if(len >= 512 -1)
				return false;

			strcpy(broadcastTxt, pTxt);
			broadcastSize = (unsigned short)(sizeof(REQ_SYSTEM_BROADCAST) - 512 + len + 1);
			return true;
		}

		unsigned int GetSize()
		{
			return broadcastSize;
		}
	};

	struct REQ_DELETE_ITEM : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::QueryCharacterName	char_name;		// ��ɫ���� 
		ADMIN::ITEM_GROUP			itemGroup;		// ��Ʒ��
		unsigned short				itemIndex;		// ��Ʒ����Ʒ���е�λ��

		REQ_DELETE_ITEM() 
			: TCP_MSG_BASE(CMD_REQ_DELETE_ITEM)
		{}
	};

	struct REQ_MODIFY_CHARACTER : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::QueryCharacterName	char_name;		// ��ɫ���� 
		ADMIN::MOD_CHAR_ATTR		attr;			// Ҫ�޸ĵ�����
		unsigned int				newValue;		// �޸�Ϊ�µ�ֵ

		REQ_MODIFY_CHARACTER()
			: TCP_MSG_BASE(CMD_REQ_MODIFY_CHARACTER)
		{}
	};

	struct REQ_MODIFY_CHARACTER_SKILL : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::QueryCharacterName	char_name;		// ��ɫ���� 
		unsigned short				skill_id;		// ����ID
		unsigned short				skill_level;	// ���ܵȼ�

		REQ_MODIFY_CHARACTER_SKILL()
			: TCP_MSG_BASE(CMD_REQ_MODIFY_CHARACTER_SKILL)
		{}
	};

	struct REQ_RECOVER_CHARACTER : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::QueryCharacterName	char_name;		// ��ɫ���� 

		REQ_RECOVER_CHARACTER()
			: TCP_MSG_BASE(CMD_REQ_RECOVER_CHRACTER)
		{}
	};

	struct REQ_MODIFY_ACCOUNT_PASSW : public ADMIN::TCP_MSG_BASE
	{
		unsigned int			srv_realm_id;	// ����id
		ADMIN::QueryUserName	user_name;		// �û��ʺ�
												// ��¼����
		char					new_password[ADMIN::MAX_PASSWORD_LENGTH];

		REQ_MODIFY_ACCOUNT_PASSW()
			: TCP_MSG_BASE(CMD_REQ_MODIFY_ACCOUNT_PASSW)
		{}
	};

	struct REQ_MAIL_ITEM : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����
		unsigned int				item_type_id;	// ��Ʒ����id
		unsigned char				item_count;		// ��Ʒ����

		REQ_MAIL_ITEM()
			: TCP_MSG_BASE(CMD_REQ_MAIL_ITEM)
		{}
	};

	struct REQ_MODIFY_CHARACTER_NAME : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����
		ADMIN::QueryCharacterName	new_char_name;	// �½�ɫ����

		REQ_MODIFY_CHARACTER_NAME()
			: TCP_MSG_BASE(CMD_REQ_MODIFY_CHARACTER_NAME)
		{}
	};

	//////////////////////////////////////////////////////////////////////////
	// ��־��

	struct REQ_USER_LOG_BASE : public ADMIN::TCP_MSG_BASE
	{
		unsigned int			srv_realm_id;		// ����id
		unsigned int			srv_cluster_id;		// ��������id
		ADMIN::DATE_TIME		startTime;			// ��ʼʱ��
		ADMIN::DATE_TIME		endTime;			// ����ʱ��
		ADMIN::QUERY_CONDITION	availableCondition;	// ��ѯ����
		union
		{
			ADMIN::QueryUserName user_name;			// �û��ʺ�
			ADMIN::QueryCharacterName char_name;	// ��ɫ����
		};

		REQ_USER_LOG_BASE(unsigned short id)
			: TCP_MSG_BASE(id)
		{}
	};

	struct REQ_LOG_MONSTER_DROP_ITEM : public REQ_USER_LOG_BASE
	{
		REQ_LOG_MONSTER_DROP_ITEM()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_MONSTER_DROP_ITEM)
		{}
	};

	struct REQ_LOG_BUY_ITEM_FROM_NPC : public REQ_USER_LOG_BASE
	{
		REQ_LOG_BUY_ITEM_FROM_NPC()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_BUY_ITEM_FROM_NPC)
		{}
	};

	struct REQ_LOG_SELL_ITEM_TO_NPC : public REQ_USER_LOG_BASE
	{
		REQ_LOG_SELL_ITEM_TO_NPC()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_SELL_ITEM_TO_NPC)
		{}
	};

	struct REQ_LOG_DROP_ITEM : public REQ_USER_LOG_BASE
	{
		REQ_LOG_DROP_ITEM()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_DROP_ITEM)
		{}
	};

	struct REQ_LOG_GET_ITEM_TRADE : public REQ_USER_LOG_BASE
	{
		REQ_LOG_GET_ITEM_TRADE()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_GET_ITEM_TRADE)
		{}
	};

	struct REQ_LOG_LOSE_ITEM_TRADE : public REQ_USER_LOG_BASE
	{
		REQ_LOG_LOSE_ITEM_TRADE()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_LOSE_ITEM_TRADE)
		{}
	};

	struct REQ_LOG_BIND_ITEM : public REQ_USER_LOG_BASE
	{
		REQ_LOG_BIND_ITEM()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_BIND_ITEM)
		{}
	};

	struct REQ_LOG_UNBIND_ITEM : public REQ_USER_LOG_BASE
	{
		REQ_LOG_UNBIND_ITEM()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_UNBIND_ITEM)
		{}
	};

	struct REQ_LOG_BOOTH_ITEM : public REQ_USER_LOG_BASE
	{
		REQ_LOG_BOOTH_ITEM()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_BOOTH_ITEM)
		{}
	};

	struct REQ_LOG_DELETE_ITEM : public REQ_USER_LOG_BASE
	{
		REQ_LOG_DELETE_ITEM()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_DELETE_ITEM)
		{}
	};

	struct REQ_LOG_USE_ITEM : public REQ_USER_LOG_BASE
	{
		REQ_LOG_USE_ITEM()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_USE_ITEM)
		{}
	};

	struct REQ_LOG_CRAFT_ITEM : public REQ_USER_LOG_BASE
	{
		REQ_LOG_CRAFT_ITEM()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_CRAFT_ITEM)
		{}
	};

	struct REQ_LOG_PUNCH_ITEM : public REQ_USER_LOG_BASE
	{
		REQ_LOG_PUNCH_ITEM()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_PUNCH_ITEM)
		{}
	};

	struct REQ_LOG_CHANGE_SLOT_ITEM : public REQ_USER_LOG_BASE
	{
		REQ_LOG_CHANGE_SLOT_ITEM()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_CHANGE_SLOT_ITEM)
		{}
	};

	struct REQ_LOG_UPGRADE_STAR : public REQ_USER_LOG_BASE
	{
		REQ_LOG_UPGRADE_STAR()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_UPGRADE_STAR)
		{}
	};

	struct REQ_LOG_GEM : public REQ_USER_LOG_BASE
	{
		REQ_LOG_GEM()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_GEM)
		{}
	};

	struct REQ_LOG_PHARMA : public REQ_USER_LOG_BASE
	{
		REQ_LOG_PHARMA()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_PHARMA)
		{}
	};

	struct REQ_LOG_CRAFT_GEM : public REQ_USER_LOG_BASE
	{
		REQ_LOG_CRAFT_GEM()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_CRAFT_GEM)
		{}
	};

	struct REQ_LOG_CRAFT_MATIRAIL : public REQ_USER_LOG_BASE
	{
		REQ_LOG_CRAFT_MATIRAIL()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_CRAFT_MATIRAIL)
		{}
	};

	struct REQ_LOG_CRAFT_RECIPE : public REQ_USER_LOG_BASE
	{
		REQ_LOG_CRAFT_RECIPE()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_CRAFT_RECIPE)
		{}
	};

	struct REQ_LOG_MAZE_INTEGRAL : public REQ_USER_LOG_BASE
	{
		REQ_LOG_MAZE_INTEGRAL()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_MAZE_INTEGRAL)
		{}
	};

	struct REQ_LOG_MONSTER_DROP_MONEY : public REQ_USER_LOG_BASE
	{
		REQ_LOG_MONSTER_DROP_MONEY()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_MONSTER_DROP_MONEY)
		{}
	};

	struct REQ_LOG_GET_MONEY_QUEST : public REQ_USER_LOG_BASE
	{
		REQ_LOG_GET_MONEY_QUEST()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_GET_MONEY_QUEST)
		{}
	};

	struct REQ_LOG_LOSE_MONEY_REPAIR : public REQ_USER_LOG_BASE
	{
		REQ_LOG_LOSE_MONEY_REPAIR()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_LOSE_MONEY_REPAIR)
		{}
	};

	struct REQ_LOG_GET_MONEY_TRADE : public REQ_USER_LOG_BASE
	{
		REQ_LOG_GET_MONEY_TRADE()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_GET_MONEY_TRADE)
		{}
	};

	struct REQ_LOG_LOSE_MONEY_TRADE : public REQ_USER_LOG_BASE
	{
		REQ_LOG_LOSE_MONEY_TRADE()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_LOSE_MONEY_TRADE)
		{}
	};

	struct REQ_LOG_QUEST_REWARD : public REQ_USER_LOG_BASE
	{
		REQ_LOG_QUEST_REWARD()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_QUEST_REWARD)
		{}
	};

	struct REQ_LOG_QUEST_GIVE : public REQ_USER_LOG_BASE
	{
		REQ_LOG_QUEST_GIVE()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_QUEST_GIVE)
		{}
	};

	struct REQ_LOG_MAIL_SEND : public REQ_USER_LOG_BASE
	{
		REQ_LOG_MAIL_SEND()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_MAIL_SEND)
		{}
	};

	struct REQ_LOG_MAIL_RECV : public REQ_USER_LOG_BASE
	{
		REQ_LOG_MAIL_RECV()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_MAIL_RECV)
		{}
	};

	struct REQ_LOG_MAIL_DELETE : public REQ_USER_LOG_BASE
	{
		REQ_LOG_MAIL_DELETE()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_MAIL_DELETE)
		{}
	};

	struct REQ_LOG_ITEM_SHOP : public REQ_USER_LOG_BASE
	{
		REQ_LOG_ITEM_SHOP()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_ITEM_SHOP)
		{}
	};

	struct REQ_LOG_INSTANCE : public REQ_USER_LOG_BASE
	{
		REQ_LOG_INSTANCE()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_INSTANCE)
		{}
	};

	struct REQ_LOG_LOGIN_LOGOUT : public REQ_USER_LOG_BASE
	{
		REQ_LOG_LOGIN_LOGOUT()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_LOGIN_LOGOUT)
		{}
	};

	// ��־��
	//////////////////////////////////////////////////////////////////////////

}; // namespace MSG_ATAS


#pragma pack(pop)