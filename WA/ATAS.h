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
	const int CMD_REQ_QUERY_QUEST_INFO		= ADMIN::MSG_BASE_ATAS + 111;	// ��ɫ������Ϣ
	const int CMD_REQ_QUERY_GUILD_INFO		= ADMIN::MSG_BASE_ATAS + 112;	// Ӷ������Ϣ
	const int CMD_REQ_QUERY_GUILD_MEMBER	= ADMIN::MSG_BASE_ATAS + 113;	// Ӷ���ų�Ա�б�
	const int CMD_REQ_ADD_PET				= ADMIN::MSG_BASE_ATAS + 114;	// ��ӳ���
	const int CMD_REQ_REMOVE_PET			= ADMIN::MSG_BASE_ATAS + 115;	// �Ƴ�����
	const int CMD_REQ_MODIFY_PET			= ADMIN::MSG_BASE_ATAS + 116;	// �޸ĳ�������
	const int CMD_REQ_GUILD_CHANGE_MASTER	= ADMIN::MSG_BASE_ATAS + 117;	// ת�ù��᳤��Ȩ��
	const int CMD_REQ_GUILD_CHANGE_LEVEL	= ADMIN::MSG_BASE_ATAS + 118;	// �޸Ĺ���ȼ�(���޹���)
	const int CMD_REQ_GUILD_CHANGE_NAME		= ADMIN::MSG_BASE_ATAS + 119;	// �޸Ĺ�������
	const int CMD_REQ_GUILD_MEMBERSHIP_LEVEL= ADMIN::MSG_BASE_ATAS + 120;	// �޸Ĺ����Աְ��(���޹���)
	const int CMD_REQ_GUILD_KICK_MEMBER		= ADMIN::MSG_BASE_ATAS + 121;	// �߳������Ա
	const int CMD_REQ_GUILD_DISMISS			= ADMIN::MSG_BASE_ATAS + 122;	// ��ɢ����
	const int CMD_REQ_RECOVER_PASSW			= ADMIN::MSG_BASE_ATAS + 123;	// �ָ�����

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
	const int CMD_REQ_LOG_CHAR_LEVELUP		= ADMIN::MSG_BASE_ATAS + 284;	// ��ɫ������¼
	const int CMD_REQ_LOG_CHAR_CREATE		= ADMIN::MSG_BASE_ATAS + 285;	// ��ɫ������¼
	const int CMD_REQ_LOG_CHAR_DELETE		= ADMIN::MSG_BASE_ATAS + 286;	// ��ɫɾ����¼
	const int CMD_REQ_LOG_CHAR_JOIN_GUILD	= ADMIN::MSG_BASE_ATAS + 287;	// ��ɫ�������¼
	const int CMD_REQ_LOG_CHAR_LEAVE_GUILD	= ADMIN::MSG_BASE_ATAS + 288;	// ��ɫ�뿪����¼
	const int CMD_REQ_LOG_CHAR_CREATE_GUILD	= ADMIN::MSG_BASE_ATAS + 289;	// ��ɫ��������¼
	const int CMD_REQ_LOG_CHAR_DISMISS_GUILD= ADMIN::MSG_BASE_ATAS + 290;	// ��ɫ��ɢ����¼
	const int CMD_REQ_LOG_CHAR_CHANGE_OCC	= ADMIN::MSG_BASE_ATAS + 291;	// ��ɫתְ��¼
	const int CMD_REQ_LOG_CHAR_SKILL		= ADMIN::MSG_BASE_ATAS + 292;	// ��ɫѧϰ���ܼ�¼
	const int CMD_REQ_LOG_CHAR_LIVING_SKILL	= ADMIN::MSG_BASE_ATAS + 293;	// ��ɫѧϰ����ܼ�¼
	const int CMD_REQ_LOG_GUILD_CREATE		= ADMIN::MSG_BASE_ATAS + 294;	// ���ᴴ����¼
	const int CMD_REQ_LOG_GUILD_DIMISS		= ADMIN::MSG_BASE_ATAS + 295;	// �����ɢ��¼
	const int CMD_REQ_LOG_INSTANCE_ASSESS	= ADMIN::MSG_BASE_ATAS + 296;	// ��ɫ������־
	const int CMD_REQ_LOG_PET_CATCH			= ADMIN::MSG_BASE_ATAS + 297;	// ����ץȡ��¼
	const int CMD_REQ_LOG_PET_GET_TRADE		= ADMIN::MSG_BASE_ATAS + 298;	// ���ǽ���ȡ��
	const int CMD_REQ_LOG_PET_LOSE_TRADE	= ADMIN::MSG_BASE_ATAS + 299;	// ���ǽ���ʧȥ
	const int CMD_REQ_LOG_PET_BOOTH			= ADMIN::MSG_BASE_ATAS + 300;	// ���ǰ�̯
	const int CMD_REQ_LOG_LOGIN_LOGOUT_BY_IP= ADMIN::MSG_BASE_ATAS + 301;	// ͨ��IP��ѯ��¼��־

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
			ADMIN::QueryCharacterId		char_id;	// ��ɫID
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
		enum
		{
			MAIL_ITEM_LIST_MAX	= 5,
			MAX_SUBJECT_LENGTH	= 21,
			MAX_BODY_LENGTH		= 201,
		};

		enum ITEM_GEN	// ��ƷƷ��
		{
			IG_WHITE,	// ��ɫƷ��
			IG_BLUE,	// ��ɫƷ��
			IG_PURPLE,	// ��ɫƷ��
			IG_ORANGE,	// ��ɫƷ��
		};

		enum RECEIVER_TYPE
		{
			RT_SINGLE_PLAYER,		// ����ָ�����
			RT_ALL_ONLINE_PLAYER,	// ���������������
			RT_ALL_PLAYER,			// �����������
		};

		unsigned int				srv_realm_id;							// ����id
		unsigned int				srv_cluster_id;							// ��������id
		ADMIN::QueryCharacterName	char_name;								// ��ɫ����
		unsigned int				item_type_id_list[MAIL_ITEM_LIST_MAX];	// ��Ʒ����id
		unsigned short				item_count_list[MAIL_ITEM_LIST_MAX];	// ��Ʒ����
		ITEM_GEN					item_gen_list[MAIL_ITEM_LIST_MAX];		// ��ƷƷ��
		unsigned int				item_expire[MAIL_ITEM_LIST_MAX];		// ��ƷʱЧ(ֻ��ʵװ����,��λ��,0��ʾ����)
		RECEIVER_TYPE				receiver_type;							// ���ն���

		char						subject[MAX_SUBJECT_LENGTH];			// ����
		char						body[MAX_BODY_LENGTH];					// ����
		unsigned int				money;									// ��Ǯ

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

	// ��ɫ������Ϣ
	struct REQ_QUERY_QUEST_INFO : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::QueryCharacterName	char_name;		// ��ɫ���� 

		REQ_QUERY_QUEST_INFO()
			: TCP_MSG_BASE(CMD_REQ_QUERY_QUEST_INFO)
		{}
	};

	// Ӷ������Ϣ
	struct REQ_QUERY_GUILD_INFO : public ADMIN::TCP_MSG_BASE
	{
		enum GUILD_QUERY_TYPE
		{
			ALL_GUILD,		// ��ѯȫ������
			ONE_GUILD,		// ��ѯָ������
		};

		GUILD_QUERY_TYPE	query_type;		// ��ѯ����
		unsigned int		srv_realm_id;	// ����id
		unsigned int		srv_cluster_id;	// ��������id
		ADMIN::GuildName	guld_name;		// ������

		REQ_QUERY_GUILD_INFO()
			: TCP_MSG_BASE(CMD_REQ_QUERY_GUILD_INFO)
		{}
	};

	// Ӷ���ų�Ա�б�
	struct REQ_QUERY_GUILD_MEMBER : public ADMIN::TCP_MSG_BASE
	{
		unsigned int		srv_realm_id;	// ����id
		unsigned int		srv_cluster_id;	// ��������id
		ADMIN::GuildName	guld_name;		// ������

		REQ_QUERY_GUILD_MEMBER()
			: TCP_MSG_BASE(CMD_REQ_QUERY_GUILD_MEMBER)
		{}
	};

	// ��ӳ���
	struct REQ_ADD_PET : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����
		unsigned int				pet_type_id;	// ���������ID

		REQ_ADD_PET()
			: TCP_MSG_BASE(CMD_REQ_ADD_PET)
		{}
	};

	// �Ƴ�����
	struct REQ_REMOVE_PET : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����
		unsigned __int64			pet_unique_id;	// ����ΨһID

		REQ_REMOVE_PET()
			: TCP_MSG_BASE(CMD_REQ_REMOVE_PET)
		{}
	};

	// �޸ĳ�������
	struct REQ_MODIFY_PET : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����
		unsigned __int64			pet_unique_id;	// ����ΨһID
		ADMIN::MOD_CHAR_ATTR		attr;			// Ҫ�޸ĵ�����
		unsigned int				newValue;		// �޸�Ϊ�µ�ֵ

		REQ_MODIFY_PET()
			: TCP_MSG_BASE(CMD_REQ_MODIFY_PET)
		{}
	};

	// ת�ù��᳤��Ȩ��
	struct REQ_GUILD_CHANGE_MASTER : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::GuildName			guld_name;		// ������
		ADMIN::QueryCharacterName	char_name;		// �»᳤�Ľ�ɫ����

		REQ_GUILD_CHANGE_MASTER()
			: TCP_MSG_BASE(CMD_REQ_GUILD_CHANGE_MASTER)
		{}
	};

	// �޸Ĺ�������
	struct REQ_GUILD_CHANGE_NAME : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::GuildName			guld_name;		// ������
		ADMIN::GuildName			new_name;		// �µĹ�����

		REQ_GUILD_CHANGE_NAME()
			: TCP_MSG_BASE(CMD_REQ_GUILD_CHANGE_NAME)
		{}
	};

	// �߳������Ա
	struct REQ_GUILD_KICK_MEMBER : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::GuildName			guld_name;		// ������
		ADMIN::QueryCharacterName	char_name;		// Ҫ�ߵĽ�ɫ����

		REQ_GUILD_KICK_MEMBER()
			: TCP_MSG_BASE(CMD_REQ_GUILD_KICK_MEMBER)
		{}
	};

	// ��ɢ����
	struct REQ_GUILD_DISMISS : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::GuildName			guld_name;		// ������

		REQ_GUILD_DISMISS()
			: TCP_MSG_BASE(CMD_REQ_GUILD_DISMISS)
		{}
	};

	struct REQ_RECOVER_PASSW : public ADMIN::TCP_MSG_BASE
	{
		unsigned int			srv_realm_id;		// ����id
		ADMIN::QueryUserName	user_name;			// �û��ʺ�
													// ��¼����
		char					old_password[33];	// ������

		REQ_RECOVER_PASSW()
			: TCP_MSG_BASE(CMD_REQ_RECOVER_PASSW)
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
			ADMIN::QueryCharacterId char_id;		// ��ɫID
		};

		REQ_USER_LOG_BASE(unsigned short id)
			: TCP_MSG_BASE(id)
		{}
	};

	struct REQ_GUILD_LOG_BASE : public ADMIN::TCP_MSG_BASE
	{
		unsigned int			srv_realm_id;		// ����id
		unsigned int			srv_cluster_id;		// ��������id
		ADMIN::DATE_TIME		startTime;			// ��ʼʱ��
		ADMIN::DATE_TIME		endTime;			// ����ʱ��
		ADMIN::QUERY_CONDITION	availableCondition;	// ��ѯ����
		union
		{
			ADMIN::GuildName	guild_name;		// ��������
			ADMIN::GuildId		guild_id;		// ����ID
		};

		REQ_GUILD_LOG_BASE(unsigned short id)
			: TCP_MSG_BASE(id)
		{}
	};

	//////////////////////////////////////////////////////////////////////////

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

	// ��ɫ������¼
	struct REQ_LOG_CHAR_LEVELUP : public REQ_USER_LOG_BASE
	{
		REQ_LOG_CHAR_LEVELUP()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_CHAR_LEVELUP)
		{}
	};

	// ��ɫ������¼
	struct REQ_LOG_CHAR_CREATE : public REQ_USER_LOG_BASE
	{
		REQ_LOG_CHAR_CREATE()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_CHAR_CREATE)
		{}
	};

	// ��ɫɾ����¼
	struct REQ_LOG_CHAR_DELETE : public REQ_USER_LOG_BASE
	{
		REQ_LOG_CHAR_DELETE()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_CHAR_DELETE)
		{}
	};

	// ��ɫ�������¼
	struct REQ_LOG_CHAR_JOIN_GUILD : public REQ_USER_LOG_BASE
	{
		REQ_LOG_CHAR_JOIN_GUILD()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_CHAR_JOIN_GUILD)
		{}
	};

	// ��ɫ�뿪����¼
	struct REQ_LOG_CHAR_LEAVE_GUILD : public REQ_USER_LOG_BASE
	{
		REQ_LOG_CHAR_LEAVE_GUILD()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_CHAR_LEAVE_GUILD)
		{}
	};

	// ��ɫ��������¼
	struct REQ_LOG_CHAR_CREATE_GUILD : public REQ_USER_LOG_BASE
	{
		REQ_LOG_CHAR_CREATE_GUILD()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_CHAR_CREATE_GUILD)
		{}
	};

	// ��ɫ��ɢ����¼
	struct REQ_LOG_CHAR_DISMISS_GUILD : public REQ_USER_LOG_BASE
	{
		REQ_LOG_CHAR_DISMISS_GUILD()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_CHAR_DISMISS_GUILD)
		{}
	};

	// ��ɫתְ��¼
	struct REQ_LOG_CHAR_CHANGE_OCC : public REQ_USER_LOG_BASE
	{
		REQ_LOG_CHAR_CHANGE_OCC()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_CHAR_CHANGE_OCC)
		{}
	};

	// ��ɫѧϰ���ܼ�¼
	struct REQ_LOG_CHAR_SKILL : public REQ_USER_LOG_BASE
	{
		REQ_LOG_CHAR_SKILL()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_CHAR_SKILL)
		{}
	};

	// ��ɫѧϰ����ܼ�¼
	struct REQ_LOG_CHAR_LIVING_SKILL : public REQ_USER_LOG_BASE
	{
		REQ_LOG_CHAR_LIVING_SKILL()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_CHAR_LIVING_SKILL)
		{}
	};

	// ��ɫ������־
	struct REQ_LOG_INSTANCE_ASSESS : public REQ_USER_LOG_BASE
	{
		REQ_LOG_INSTANCE_ASSESS()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_INSTANCE_ASSESS)
		{}
	};

	// ����ץȡ��¼
	struct REQ_LOG_PET_CATCH : public REQ_USER_LOG_BASE
	{
		REQ_LOG_PET_CATCH()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_PET_CATCH)
		{}
	};

	// ���ǽ���ȡ��
	struct REQ_LOG_PET_GET_TRADE : public REQ_USER_LOG_BASE
	{
		REQ_LOG_PET_GET_TRADE()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_PET_GET_TRADE)
		{}
	};

	// ���ǽ���ʧȥ
	struct REQ_LOG_PET_LOSE_TRADE : public REQ_USER_LOG_BASE
	{
		REQ_LOG_PET_LOSE_TRADE()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_PET_LOSE_TRADE)
		{}
	};

	// ���ǰ�̯
	struct REQ_LOG_PET_BOOTH : public REQ_USER_LOG_BASE
	{
		REQ_LOG_PET_BOOTH()
			: REQ_USER_LOG_BASE(CMD_REQ_LOG_PET_BOOTH)
		{}
	};

	// ���ᴴ����¼
	struct REQ_LOG_GUILD_CREATE : public REQ_GUILD_LOG_BASE
	{
		REQ_LOG_GUILD_CREATE()
			: REQ_GUILD_LOG_BASE(CMD_REQ_LOG_GUILD_CREATE)
		{}
	};

	// �����ɢ��¼
	struct REQ_LOG_GUILD_DIMISS : public REQ_GUILD_LOG_BASE
	{
		REQ_LOG_GUILD_DIMISS()
			: REQ_GUILD_LOG_BASE(CMD_REQ_LOG_GUILD_DIMISS)
		{}
	};

	struct REQ_LOG_LOGIN_LOGOUT_BY_IP : public ADMIN::TCP_MSG_BASE
	{
		unsigned int			srv_realm_id;		// ����id
		unsigned int			srv_cluster_id;		// ��������id
		ADMIN::DATE_TIME		startTime;			// ��ʼʱ��
		ADMIN::DATE_TIME		endTime;			// ����ʱ��
		unsigned int			clientIp;			// Ҫ��ѯ��IP

		REQ_LOG_LOGIN_LOGOUT_BY_IP()
			: TCP_MSG_BASE(CMD_REQ_LOG_LOGIN_LOGOUT_BY_IP)
		{}
	};

	// ��־��
	//////////////////////////////////////////////////////////////////////////

}; // namespace MSG_ATAS


#pragma pack(pop)