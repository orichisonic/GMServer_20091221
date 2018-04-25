#pragma once

#include "AdminDef.h"

#pragma pack(push, 1)

namespace MSG_ASTA
{
	const int CMD_REP_LOGIN					= ADMIN::MSG_BASE_ASTA + 1;		// ��¼���
	const int CMD_REP_CHARACTER_BASE_INFO	= ADMIN::MSG_BASE_ASTA + 2;		// ��ѯ��ɫ������Ϣ���
	const int CMD_REP_CHARACTER_ITEMS_INFO	= ADMIN::MSG_BASE_ASTA + 3;		// ��ѯ��ɫ��Ʒ��Ϣ���
	const int CMD_REP_CLOSURE_ACCOUNT_INFO	= ADMIN::MSG_BASE_ASTA + 4;		// ��ѯ��ͣ�ʺ��ź���
	const int CMD_REP_CHARACTER_INFO_FAIL	= ADMIN::MSG_BASE_ASTA + 5;		// ��ѯ��ɫ��Ϣʧ��

	const int CMD_REP_KICK_USER				= ADMIN::MSG_BASE_ASTA + 100;	// ���˽��
	const int CMD_REP_CLOSE_ACCOUNT			= ADMIN::MSG_BASE_ASTA + 101;	// ��ͣ���
	const int CMD_REP_AVAILABLE_ACCOUNT		= ADMIN::MSG_BASE_ASTA + 102;	// �����
	const int CMD_REP_SYSTEM_BROADCAST		= ADMIN::MSG_BASE_ASTA + 103;	// ϵͳ�㲥���
	const int CMD_REP_NAME_ID_LIST			= ADMIN::MSG_BASE_ASTA + 104;	// ���ֲ�ѯ���ؽ�ɫ�����ֺ�ID
	const int CMD_REP_DELETE_ITEM			= ADMIN::MSG_BASE_ASTA + 105;	// ɾ����Ʒ���
	const int CMD_REP_CHARACTER_ITEM		= ADMIN::MSG_BASE_ASTA + 106;	// ��ѯ��ɫ��Ʒ���
	const int CMD_REP_CHARACTER_FRIEND		= ADMIN::MSG_BASE_ASTA + 107;	// ��ѯ��ɫ���ѽ��
	const int CMD_REP_CHARACTER_MEDAL		= ADMIN::MSG_BASE_ASTA + 108;	// ��ѯ��ɫ������Ϣ���(���Ƿ���)
	const int CMD_REP_CHARACTER_PET			= ADMIN::MSG_BASE_ASTA + 109;	// ��ѯ��ɫ������Ϣ���
	const int CMD_REP_MODIFY_CHARACTER		= ADMIN::MSG_BASE_ASTA + 110;	// �޸Ľ�ɫ���Խ��(��Ǯ,����,�ĵȼ�)
	const int CMD_REP_MODIFY_CHARACTER_SKILL= ADMIN::MSG_BASE_ASTA + 111;	// �޸Ľ�ɫ���ܽ��
	const int CMD_REP_RECOVER_CHARACTER		= ADMIN::MSG_BASE_ASTA + 112;	// �ָ���ɫ���
	const int CMD_REP_MODIFY_ACCOUNT_PASSW	= ADMIN::MSG_BASE_ASTA + 113;	// �޸ĵ�¼����
	const int CMD_REP_MAIL_ITEM				= ADMIN::MSG_BASE_ASTA + 114;	// ���ɫ�ʼ���Ʒ���
	const int CMD_REP_MODIFY_CHARACTER_NAME	= ADMIN::MSG_BASE_ASTA + 115;	// �޸Ľ�ɫ��
	const int CMD_REP_CHARACTER_SKILL		= ADMIN::MSG_BASE_ASTA + 116;	// ��ɫ������Ϣ
	const int CMD_REP_QUERY_QUEST_INFO		= ADMIN::MSG_BASE_ATAS + 111;	// ��ɫ������Ϣ
	const int CMD_REP_QUERY_GUILD_INFO		= ADMIN::MSG_BASE_ATAS + 112;	// Ӷ������Ϣ
	const int CMD_REP_QUERY_GUILD_MEMBER	= ADMIN::MSG_BASE_ATAS + 113;	// Ӷ���ų�Ա�б�
	const int CMD_REP_ADD_PET				= ADMIN::MSG_BASE_ATAS + 114;	// ��ӳ���
	const int CMD_REP_REMOVE_PET			= ADMIN::MSG_BASE_ATAS + 115;	// �Ƴ�����
	const int CMD_REP_MODIFY_PET			= ADMIN::MSG_BASE_ATAS + 116;	// �޸ĳ�������
	const int CMD_REP_GUILD_CHANGE_MASTER	= ADMIN::MSG_BASE_ATAS + 117;	// ת�ù��᳤��Ȩ��
	const int CMD_REP_GUILD_CHANGE_LEVEL	= ADMIN::MSG_BASE_ATAS + 118;	// �޸Ĺ���ȼ�(���޹���)
	const int CMD_REP_GUILD_CHANGE_NAME		= ADMIN::MSG_BASE_ATAS + 119;	// �޸Ĺ�������
	const int CMD_REP_GUILD_MEMBERSHIP_LEVEL= ADMIN::MSG_BASE_ATAS + 120;	// �޸Ĺ����Աְ��(���޹���)
	const int CMD_REP_GUILD_KICK_MEMBER		= ADMIN::MSG_BASE_ATAS + 121;	// �߳������Ա
	const int CMD_REP_GUILD_DISMISS			= ADMIN::MSG_BASE_ATAS + 122;	// ��ɢ����
	const int CMD_REP_RECOVER_PASSW			= ADMIN::MSG_BASE_ATAS + 123;	// �ظ�������

	// ��־��ѯ
	const int CMD_REP_LOG_MONSTER_DROP_ITEM	= ADMIN::MSG_BASE_ASTA + 250;	// ��Ʒ:�������
	const int CMD_REP_LOG_BUY_ITEM_FROM_NPC	= ADMIN::MSG_BASE_ASTA + 251;	// ��Ʒ:��NPC������
	const int CMD_REP_LOG_SELL_ITEM_TO_NPC	= ADMIN::MSG_BASE_ASTA + 252;	// ��Ʒ:����NPC
	const int CMD_REP_LOG_DROP_ITEM			= ADMIN::MSG_BASE_ASTA + 253;	// ��Ʒ:�������ɾ��
	const int CMD_REP_LOG_GET_ITEM_TRADE	= ADMIN::MSG_BASE_ASTA + 254;	// ��Ʒ:���׻��
	const int CMD_REP_LOG_LOSE_ITEM_TRADE	= ADMIN::MSG_BASE_ASTA + 255;	// ��Ʒ:����ʧȥ
	const int CMD_REP_LOG_BIND_ITEM			= ADMIN::MSG_BASE_ASTA + 256;	// ��Ʒ:װ���󶨼�¼
	const int CMD_REP_LOG_UNBIND_ITEM		= ADMIN::MSG_BASE_ASTA + 257;	// ��Ʒ:װ����󶨼�¼
	const int CMD_REP_LOG_BOOTH_ITEM		= ADMIN::MSG_BASE_ASTA + 258;	// ��Ʒ:��ɫ����̯����־��ѯ
	const int CMD_REP_LOG_DELETE_ITEM		= ADMIN::MSG_BASE_ASTA + 259;	// ��Ʒ:����ɾ����¼(ָʹ��ʱ�䵽��)
	const int CMD_REP_LOG_USE_ITEM			= ADMIN::MSG_BASE_ASTA + 260;	// ��Ʒ:����ʹ�ü�¼
	const int CMD_REP_LOG_CRAFT_ITEM		= ADMIN::MSG_BASE_ASTA + 261;	// ��Ʒ:װ��������¼
	const int CMD_REP_LOG_PUNCH_ITEM		= ADMIN::MSG_BASE_ASTA + 262;	// ��Ʒ:װ�����
	const int CMD_REP_LOG_CHANGE_SLOT_ITEM	= ADMIN::MSG_BASE_ASTA + 263;	// ��Ʒ:װ���Ŀ�
	const int CMD_REP_LOG_UPGRADE_STAR		= ADMIN::MSG_BASE_ASTA + 264;	// ��Ʒ:װ������
	const int CMD_REP_LOG_GEM				= ADMIN::MSG_BASE_ASTA + 265;	// ��Ʒ:��ʯ
	const int CMD_REP_LOG_PHARMA			= ADMIN::MSG_BASE_ASTA + 266;	// ��Ʒ:��ҩ��¼
	const int CMD_REP_LOG_CRAFT_GEM			= ADMIN::MSG_BASE_ASTA + 267;	// ��Ʒ:��ʯ�ϳ�
	const int CMD_REP_LOG_CRAFT_MATIRAIL	= ADMIN::MSG_BASE_ASTA + 268;	// ��Ʒ:���Ϻϳ�
	const int CMD_REP_LOG_CRAFT_RECIPE		= ADMIN::MSG_BASE_ASTA + 269;	// ��Ʒ:ͼֽ�ϳ�
	const int CMD_REP_LOG_MAZE_INTEGRAL		= ADMIN::MSG_BASE_ASTA + 270;	// �Թ����ֶһ�����
	const int CMD_REP_LOG_MONSTER_DROP_MONEY= ADMIN::MSG_BASE_ASTA + 271;	// ��Ϸ��:�������
	const int CMD_REP_LOG_GET_MONEY_QUEST	= ADMIN::MSG_BASE_ASTA + 272;	// ��Ϸ��:������
	const int CMD_REP_LOG_LOSE_MONEY_REPAIR	= ADMIN::MSG_BASE_ASTA + 273;	// ��Ϸ��:��װ
	const int CMD_REP_LOG_GET_MONEY_TRADE	= ADMIN::MSG_BASE_ASTA + 274;	// ��Ϸ��:���׻��
	const int CMD_REP_LOG_LOSE_MONEY_TRADE	= ADMIN::MSG_BASE_ASTA + 275;	// ��Ϸ��:����ʧȥ
	const int CMD_REP_LOG_QUEST_REWARD		= ADMIN::MSG_BASE_ASTA + 276;	// ����:��ɻ�ü�¼
	const int CMD_REP_LOG_QUEST_GIVE		= ADMIN::MSG_BASE_ASTA + 277;	// ����:������Ʒɾ����¼
	const int CMD_REP_LOG_MAIL_SEND			= ADMIN::MSG_BASE_ASTA + 278;	// �ʼ�:���ͼ�¼
	const int CMD_REP_LOG_MAIL_RECV			= ADMIN::MSG_BASE_ASTA + 279;	// �ʼ�:���ռ�¼
	const int CMD_REP_LOG_MAIL_DELETE		= ADMIN::MSG_BASE_ASTA + 280;	// �ʼ�:ɾ����¼
	const int CMD_REP_LOG_ITEM_SHOP			= ADMIN::MSG_BASE_ASTA + 281;	// �̳�:�����¼
	const int CMD_REP_LOG_INSTANCE			= ADMIN::MSG_BASE_ASTA + 282;	// ��ѯ�û��ĸ�����־
	const int CMD_REP_LOG_LOGIN_LOGOUT		= ADMIN::MSG_BASE_ASTA + 283;	// ��ҵ�½/�ǳ���־
	const int CMD_REP_LOG_CHAR_LEVELUP		= ADMIN::MSG_BASE_ATAS + 284;	// ��ɫ������¼
	const int CMD_REP_LOG_CHAR_CREATE		= ADMIN::MSG_BASE_ATAS + 285;	// ��ɫ������¼
	const int CMD_REP_LOG_CHAR_DELETE		= ADMIN::MSG_BASE_ATAS + 286;	// ��ɫɾ����¼
	const int CMD_REP_LOG_CHAR_JOIN_GUILD	= ADMIN::MSG_BASE_ATAS + 287;	// ��ɫ�������¼
	const int CMD_REP_LOG_CHAR_LEAVE_GUILD	= ADMIN::MSG_BASE_ATAS + 288;	// ��ɫ�뿪����¼
	const int CMD_REP_LOG_CHAR_CREATE_GUILD	= ADMIN::MSG_BASE_ATAS + 289;	// ��ɫ��������¼
	const int CMD_REP_LOG_CHAR_DISMISS_GUILD= ADMIN::MSG_BASE_ATAS + 290;	// ��ɫ��ɢ����¼
	const int CMD_REP_LOG_CHAR_CHANGE_OCC	= ADMIN::MSG_BASE_ATAS + 291;	// ��ɫתְ��¼
	const int CMD_REP_LOG_CHAR_SKILL		= ADMIN::MSG_BASE_ATAS + 292;	// ��ɫѧϰ���ܼ�¼
	const int CMD_REP_LOG_CHAR_LIVING_SKILL	= ADMIN::MSG_BASE_ATAS + 293;	// ��ɫѧϰ����ܼ�¼
	const int CMD_REP_LOG_GUILD_CREATE		= ADMIN::MSG_BASE_ATAS + 294;	// ���ᴴ����¼
	const int CMD_REP_LOG_GUILD_DISMISS		= ADMIN::MSG_BASE_ATAS + 295;	// �����ɢ��¼
	const int CMD_REP_LOG_INSTANCE_ASSEASS	= ADMIN::MSG_BASE_ATAS + 296;	// ��ɫ������־
	const int CMD_REP_LOG_PET_CATCH			= ADMIN::MSG_BASE_ATAS + 297;	// ����ץȡ��¼
	const int CMD_REP_LOG_PET_GET_TRADE		= ADMIN::MSG_BASE_ATAS + 298;	// ���ǽ���ȡ��
	const int CMD_REP_LOG_PET_LOSE_TRADE	= ADMIN::MSG_BASE_ATAS + 299;	// ���ǽ���ʧȥ
	const int CMD_REP_LOG_PET_BOOTH			= ADMIN::MSG_BASE_ATAS + 300;	// ���ǰ�̯
	const int CMD_REP_LOG_LOGIN_LOGOUT_BY_IP= ADMIN::MSG_BASE_ATAS + 301;	// ����IP��ѯ��¼

	// 
	// ���ص�½���
	// 
	struct REP_LOGIN : public ADMIN::TCP_MSG_BASE
	{
		enum
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// ��¼�ɹ�
			E_FAILED_REPEATEDREQUEST,				// �ظ����͵�¼����
			E_FAILED_SERVERINTERNALERROR,			// �������ڲ�����
			E_FAILED_INVALIDACCOUNTORPASSWORD,		// �ʺŻ����������
		}eResult;

		REP_LOGIN()
			:TCP_MSG_BASE(CMD_REP_LOGIN),eResult(E_FAILED_UNKNOWNERROR)
		{}
	};

	//
	// ���ؽ�ɫ������Ϣ
	//
	struct REP_CHARACTER_BASE_INFO : public ADMIN::TCP_MSG_BASE
	{
		unsigned int	srv_realm_id;	// ����id
		unsigned int	srv_cluster_id;	// ��������id
		char			user_name[ADMIN::MAX_ID_LENGTH]; // �ʺ�
		unsigned int	char_id;		// ��ɫid
		char			char_name[ADMIN::MAX_CHARACTER_NAME_LENGTH];		// ��ɫ����
		char			char_title1[ADMIN::MAX_CHARACTER_NAME_LENGTH];	// ��ɫ��ν1
		char			char_title2[ADMIN::MAX_CHARACTER_NAME_LENGTH];	// ��ɫ��ν2
		unsigned char	char_type;			// ��ɫ����(ְҵ�Ա��)
		bool			is_deleted;			// �Ƿ�ɾ��
		bool			is_online;			// �Ƿ�����
		bool			is_closed;			// �Ƿ��ͣ
		unsigned char	char_level;			// ��ɫ�ȼ�
		unsigned int	money;				// ��Ϸ������
		unsigned int	nineyou_money;		// 9�α�����
		unsigned int	reserve_money;		// ������
		char			cur_scene_name[ADMIN::MAX_SCENE_NAME_LENGTH];	// ��ǰ���ڳ�������
		unsigned int	cur_scene_id;		// ��ǰ���ڳ���id
		unsigned int	cur_scene_pos;		// ��ǰ���ڳ�������

		unsigned int	hp;					// ����ֵ
		unsigned short	mp;					// ħ��ֵ
		unsigned short	vitality;			// ����ֵ
		unsigned short	good_evil;			// �ƶ�ֵ
		unsigned short	physical_attack;	// ������
		unsigned short	physical_defense;	// �������
		unsigned short	magic_attack;		// ħ������
		unsigned short	magic_defense;		// ħ������
		float			critical;			// ������
		unsigned short	strength;			// ����
		unsigned short	dexterity;			// ����
		unsigned short	intellect;			// ����
		unsigned short	corporeity;			// ����
		unsigned short	potential_point;	// Ǳ����
		unsigned int	exp;				// ����
		unsigned int	exp_next_level;		// ��������

		float			physical_resist;	// ������
		float			magic_resist;		// ��������
		float			fire_resist;		// ���濹��
		float			cold_resist;		// ��������
		float			poison_resist;		// ���ؿ���

		REP_CHARACTER_BASE_INFO()
			:TCP_MSG_BASE(CMD_REP_CHARACTER_BASE_INFO)
		{}
	};

	//
	// ���ؽ�ɫ��Ϣ��ѯʧ��
	//
	struct REP_CHARACTER_INFO_FAIL : public ADMIN::TCP_MSG_BASE
	{
		enum QUERY_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,		// ��ѯ��ͣ��Ϣ�ɹ�
			E_FAILED_ACCOUNT_NOTEXIST,					// �ʺŲ�����
			E_FAILED_SERVERINTERNALERROR,				// �������ڲ�����
		};

		// ��ѯ����
		enum REQ_TYPE
		{
			RT_INVALID			= 0,
			RT_CHAR_BASE_INFO	= 1,					// ��ɫ������Ϣ
			RT_CHAR_ITEM_INFO	= 2,					// ��ɫ��Ʒ��Ϣ
			RT_CHAR_BANK_INFO	= 3,					// ��ɫ������Ϣ
			RT_CHAR_MEDAL_INFO	= 4,					// ��ɫ������Ϣ(��ʱ����)
			RT_CHAR_PET_INFO	= 5,					// ��ɫ������Ϣ
			RT_CHAR_FRIEND_INFO	= 6,					// ��ɫ������Ϣ
			RT_CHAR_SKILL_INFO	= 7,					// ��ɫ������Ϣ
		};

		QUERY_RESULT					eResult;		// ��ѯ���
		unsigned int					srv_realm_id;	// ����id
		unsigned int					srv_cluster_id;	// ��������id
		REQ_TYPE						req_type;		// ��ѯ����
		ADMIN::QUERY_CONDITION			query_condition;// ��ѯ����
		union
		{
			ADMIN::QueryUserName		user_name;		// �û��ʺ�
			ADMIN::QueryCharacterName	char_name;		// ��ɫ����
		};

		REP_CHARACTER_INFO_FAIL()
			: TCP_MSG_BASE(CMD_REP_CHARACTER_INFO_FAIL)
		{}
	};

	//
	// �����ʺ���Ϣ
	//
	struct REP_CLOSURE_ACCOUNT_INFO : public ADMIN::TCP_MSG_BASE
	{
		enum QUERY_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// ��ѯ��ͣ��Ϣ�ɹ�
			E_FAILED_ACCOUNT_NOTEXIST,				// �ʺŲ�����
			E_FAILED_SERVERINTERNALERROR,			// �������ڲ�����
		};

		QUERY_RESULT			eResult;			// ���
		unsigned int			srv_realm_id;		// ����id
		unsigned int			accountIndex;		// �ʺ�ID
		unsigned int			closure_reason;		// ��ͣԭ��
		bool					is_clousure;		// �Ƿ��ͣ
		ADMIN::QueryUserName	user_name;			// �ʺ�
		ADMIN::DATE_TIME		last_logout_time;	// �ϴε�¼ʱ��
		ADMIN::DATE_TIME		enable_time;		// ��ͣ����ʱ��

		REP_CLOSURE_ACCOUNT_INFO()
			:TCP_MSG_BASE(CMD_REP_CLOSURE_ACCOUNT_INFO),eResult(E_FAILED_UNKNOWNERROR)
		{}
	};

	//
	// �������˽��
	//
	struct REP_KICK_USER : public ADMIN::TCP_MSG_BASE
	{
		enum KICK_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// ���˳ɹ�
			E_FAILED_CHAR_NOTINGAME,				// �ʺ�û�н�ɫ����Ϸ��
			E_FAILED_SERVERINTERNALERROR,			// �������ڲ�����
		};

		KICK_RESULT				eResult;			// ���˽��
		unsigned int			srv_realm_id;		// ����id
		unsigned int			srv_cluster_id;		// ��������id
		ADMIN::QUERY_CONDITION	query_condition;	// ��ѯ����
		union
		{
			ADMIN::QueryUserName		user_name;	// �û��ʺ�
			ADMIN::QueryCharacterName	char_name;	// ��ɫ����
		};

		REP_KICK_USER()
			:TCP_MSG_BASE(CMD_REP_KICK_USER)
		{}
	};

	//
	// ��ͣ�ʺŽ��
	//
	struct REP_CLOSE_ACCOUNT : public ADMIN::TCP_MSG_BASE
	{
		enum CLOSE_ACCOUNT_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,		// ��ͣ�ɹ�
			E_FAILED_ACCOUNT_NOTEXIST,					// �ʺŲ�����
			E_FAILED_SERVERINTERNALERROR,				// �������ڲ�����
		};

		CLOSE_ACCOUNT_RESULT			eResult;		// ��ͣ���
		unsigned int					srv_realm_id;	// ����id
		unsigned int					srv_cluster_id;	// ��������id
		ADMIN::DATE_TIME				enable_time;	// ��ͣ����ʱ��
		ADMIN::QUERY_CONDITION			query_condition;// ��ѯ����
		union
		{
			ADMIN::QueryUserName		user_name;		// �û��ʺ�
			ADMIN::QueryCharacterName	char_name;		// ��ɫ����
		};

		REP_CLOSE_ACCOUNT()
			:TCP_MSG_BASE(CMD_REP_CLOSE_ACCOUNT)
		{}
	};

	//
	// ����ʺŽ��
	//
	struct REP_AVAILABLE_ACCOUNT : public ADMIN::TCP_MSG_BASE
	{
		enum OP_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,			// ���ɹ�
			E_FAILED_ACCOUNT_NOTEXIST,						// �ʺŲ�����
			E_FAILED_SERVERINTERNALERROR,					// �������ڲ�����
		};

		OP_RESULT						eResult;			// �����
		unsigned int					srv_realm_id;		// ����id
		unsigned int					srv_cluster_id;		// ��������id
		ADMIN::QUERY_CONDITION			availableCondition;	// ��ѯ����
		union
		{
			ADMIN::QueryUserName		user_name;			// �û��ʺ�
			ADMIN::QueryCharacterName	char_name;			// ��ɫ����
		};

		REP_AVAILABLE_ACCOUNT()
			:TCP_MSG_BASE(CMD_REP_AVAILABLE_ACCOUNT)
		{
			user_name.userName[0] = '\0';
		}
	};

	//
	// ϵͳ�㲥��Ϣ���
	// 
	struct REP_SYSTEM_BROADCAST : public ADMIN::TCP_MSG_BASE
	{
		enum SYSTEM_BROADCAST_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// �㲥�ɹ�
			E_FAILED_SERVERINTERNALERROR,			// �������ڲ�����
		};

		SYSTEM_BROADCAST_RESULT eResult;			// �㲥���

		REP_SYSTEM_BROADCAST()
			:TCP_MSG_BASE(CMD_REP_SYSTEM_BROADCAST)
		{}
	};

	//
	// ����ID�б�
	//
	struct REP_NAME_ID_LIST_HEADER : public ADMIN::TCP_MSG_BASE
	{
		// ���ؽ��
		enum NAME_ID_LIST_HEADER_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// ��ѯ�ɹ�
			E_FAILED_NOTEXIST,						// ��ɫ���ʺŲ�����
			E_FAILED_SERVERINTERNALERROR,			// �������ڲ�����
		};

		NAME_ID_LIST_HEADER_RESULT	eResult;		// ���
		ADMIN::QUERY_CONDITION		query_condition;// ��ѯ����
		bool						allSend;		// �Ƿ������
		unsigned int				param1;			// ��������(����)
		unsigned int				param2;			// ��������(����)

		REP_NAME_ID_LIST_HEADER()
			: TCP_MSG_BASE(CMD_REP_NAME_ID_LIST)
		{}
	};

	struct NAME_ID_ELEM
	{
		union 
		{
			struct  
			{
				unsigned int userIdx;
				char userName[ADMIN::MAX_ID_LENGTH];
			}user_name; // �û��ʺ�

			struct  
			{
				unsigned int charIdx;
				char charName[ADMIN::MAX_CHARACTER_NAME_LENGTH];
				bool isDeleted;
			}char_name; // ��ɫ���� 
		};
	};

	typedef ADMIN::VarLenStruct<REP_NAME_ID_LIST_HEADER, NAME_ID_ELEM, 20>
		REP_NAME_ID_LIST;

	struct REP_DELETE_ITEM : public ADMIN::TCP_MSG_BASE
	{
		enum DELETE_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// �ɹ�
			E_FAILED_SERVERINTERNALERROR,			// �������ڲ�����
			E_NOT_FOUND,
		};

		DELETE_RESULT				eResult;		// ɾ�����
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::QueryCharacterName	char_name;		// ��ɫ��
		ADMIN::ITEM_GROUP			itemGroup;		// ��Ʒ������
		unsigned short				itemIndex;		// ��Ʒ�����е�λ��

		REP_DELETE_ITEM()
			: TCP_MSG_BASE(CMD_REP_DELETE_ITEM)
		{}
	};

	// ��ɫ��Ʒ�б�
	struct CHARACTER_ITEM_INFO_HEADER : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::QueryCharacterName	char_name;		// ��ɫ��
		ADMIN::ITEM_GROUP			itemGroup;		// ����ѯ����Ʒ��
		bool						allSend;		// �Ƿ������

		CHARACTER_ITEM_INFO_HEADER()
			: TCP_MSG_BASE(CMD_REP_CHARACTER_ITEM)
		{}
	};

	struct CHARACTER_ITEM_INFO_ITEM
	{
		unsigned short		itemIndex;			// ���(�����е�λ��)
		unsigned int		itemTypeId;			// ����ID
												// ��Ʒ����
		char				itemName[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64	itemUniqueId;		// ��ƷΨһID
		unsigned char		itemCount;			// ����
		unsigned short		itemDur;			// �;�
	};

	typedef ADMIN::VarLenStruct<CHARACTER_ITEM_INFO_HEADER, CHARACTER_ITEM_INFO_ITEM, 20>
		CHARACTER_ITEM_INFO;

	// ��ɫ�����б�
	struct CHARACTER_FRIEND_INFO_HEADER : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::QueryCharacterName	char_name;		// ��ɫ��
		bool						allSend;		// �Ƿ������

		CHARACTER_FRIEND_INFO_HEADER()
			: TCP_MSG_BASE(CMD_REP_CHARACTER_FRIEND)
		{}
	};

	struct CHARACTER_FRIEND_INFO_ITEM
	{
		unsigned int				friendIndex;	// ���ѽ�ɫID
		ADMIN::QueryCharacterName	friend_name;	// ��������
		unsigned char				friendTypeId;	// ���ѵĽ�ɫ����
		unsigned char				friendLevel;	// ���ѵȼ�
		unsigned short				friendIni;		// ���ܶ�
	};

	typedef ADMIN::VarLenStruct<CHARACTER_FRIEND_INFO_HEADER, CHARACTER_FRIEND_INFO_ITEM, 20>
		CHARACTER_FRIEND_INFO;

	// ��ɫ�����б�(��ʱ����)
	struct CHARACTER_MEDAL_INFO_HEADER : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::QueryCharacterName	char_name;		// ��ɫ��
		bool						allSend;		// �Ƿ������

		CHARACTER_MEDAL_INFO_HEADER()
			: TCP_MSG_BASE(CMD_REP_CHARACTER_MEDAL)
		{}
	};

	struct CHARACTER_MEDAL_INFO_ITEM
	{
		unsigned int	medalTypeId;	// ��Ʒ����ID
										// ��Ʒ��
		char			medalName[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned short	medalDur;		// �;�
		float			medalBringUp;	// ����
		unsigned char	medalPetIndex;	// ������ų���ֵ255Ϊ�޳���
		unsigned char	medalPos;		// �����ڳ��ﱳ����λ�� 255Ϊװ����
	};

	typedef ADMIN::VarLenStruct<CHARACTER_MEDAL_INFO_HEADER, CHARACTER_MEDAL_INFO_ITEM, 9>
		CHARACTER_MEDAL_INFO;

	// ��ɫ�����б�
	struct CHARACTER_PET_INFO_HEADER : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::QueryCharacterName	char_name;		// ��ɫ��
		bool						allSend;		// �Ƿ������

		CHARACTER_PET_INFO_HEADER()
			: TCP_MSG_BASE(CMD_REP_CHARACTER_PET)
		{}
	};

	struct CHARACTER_PET_INFO_ITEM
	{
		unsigned __int64	pet_unique_id;		// ����ΨһID
		unsigned int		petTypeId;			// ����ID
												// ��������
		char				petName[ADMIN::MAX_CHARACTER_NAME_LENGTH];
		unsigned char		petLevel;			// �ȼ�
		unsigned char		petNature;			// �Ը�
		unsigned short		petGrowth;			// �ɳ���
		unsigned int		healthe;			// ������
		unsigned short		physical_attack;	// ������
		unsigned short		physical_defense;	// �������
		unsigned short		magic_attack;		// ħ������
		unsigned short		magic_defense;		// ħ������
		unsigned short		fire_attack;		// ���湥��
		unsigned short		cold_attack;		// ��������
		unsigned short		poison_attack;		// ���ع���
		float				fire_resist;		// ���濹��
		float				cold_resist;		// ��������
		float				poison_resist;		// ���ؿ���
	};

	typedef ADMIN::VarLenStruct<CHARACTER_PET_INFO_HEADER, CHARACTER_PET_INFO_ITEM, 8>
		CHARACTER_PET_INFO;

	struct REP_MODIFY_CHARACTER : public ADMIN::TCP_MSG_BASE
	{
		enum MODIFY_RESUL
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// �ɹ�
			E_FAILED_SERVERINTERNALERROR,			// �������ڲ�����
			E_NOT_FOUND,
		};

		MODIFY_RESUL				eResult;		// ���
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::QueryCharacterName	char_name;		// ��ɫ���� 
		ADMIN::MOD_CHAR_ATTR		attr;			// Ҫ�޸ĵ�����
		unsigned int				newValue;		// �µ�ֵ

		REP_MODIFY_CHARACTER()
			: TCP_MSG_BASE(CMD_REP_MODIFY_CHARACTER)
		{}
	};

	struct REP_MODIFY_CHARACTER_SKILL : public ADMIN::TCP_MSG_BASE
	{
		enum MODIFY_CHARACTER_SKILL_RESUL
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,		// �ɹ�
			E_FAILED_SERVERINTERNALERROR,				// �������ڲ�����
			E_NOT_FOUND,
		};

		MODIFY_CHARACTER_SKILL_RESUL	eResult;		// ���
		unsigned int					srv_realm_id;	// ����id
		unsigned int					srv_cluster_id;	// ��������id
		ADMIN::QueryCharacterName		char_name;		// ��ɫ���� 
		unsigned short					skill_id;		// ����ID
		unsigned short					skill_level;	// ���ܵȼ�

		REP_MODIFY_CHARACTER_SKILL()
			: TCP_MSG_BASE(CMD_REP_MODIFY_CHARACTER_SKILL)
		{}
	};

	struct REP_RECOVER_CHARACTER : public ADMIN::TCP_MSG_BASE
	{
		enum RECOVER_RESUL
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// �ɹ�
			E_FAILED_SERVERINTERNALERROR,			// �������ڲ�����
			E_NOT_FOUND,
		};

		RECOVER_RESUL				eResult;		// ���
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::QueryCharacterName	char_name;		// ��ɫ���� 

		REP_RECOVER_CHARACTER()
			: TCP_MSG_BASE(CMD_REP_RECOVER_CHARACTER)
		{}
	};

	struct REP_MODIFY_ACCOUNT_PASSW : public ADMIN::TCP_MSG_BASE
	{
		enum MODIFY_PASSW_RESUL
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,// �ɹ�
			E_FAILED_SERVERINTERNALERROR,		// �������ڲ�����
			E_NOT_FOUND,
		};

		MODIFY_PASSW_RESUL		eResult;			// ���
		unsigned int			srv_realm_id;		// ����id
		ADMIN::QueryUserName	user_name;			// �û��ʺ�
		char					old_password[33];	// �ɵ�����

		REP_MODIFY_ACCOUNT_PASSW()
			: TCP_MSG_BASE(CMD_REP_MODIFY_ACCOUNT_PASSW)
		{}
	};
		
	struct REP_MAIL_ITEM : public ADMIN::TCP_MSG_BASE
	{
		enum MAIL_ITEM_RESUL
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,// �ɹ�
			E_FAILED_SERVERINTERNALERROR,		// �������ڲ�����
			E_NOT_FOUND,
			E_TOO_MUCH,							// Ⱥ���������
		};

		MAIL_ITEM_RESUL				eResult;		// ���
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����
		unsigned int				item_type_id;	// ��Ʒ����id
		unsigned char				item_count;		// ��Ʒ����

		REP_MAIL_ITEM()
			: TCP_MSG_BASE(CMD_REP_MAIL_ITEM)
		{}
	};

	struct REP_MODIFY_CHARACTER_NAME : public ADMIN::TCP_MSG_BASE
	{
		enum MODIFY_CHARACTER_NAME_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,// �ɹ�
			E_FAILED_SERVERINTERNALERROR,		// �������ڲ�����
			E_NOT_FOUND,
			E_ERROR_NAME,						// ���Ϸ��Ľ�ɫ��
			E_REPEAT_NAME,						// �ؽ�ɫ��
		};

		MODIFY_CHARACTER_NAME_RESULT	eResult;		// ���
		unsigned int					srv_realm_id;	// ����id
		unsigned int					srv_cluster_id;	// ��������id

		REP_MODIFY_CHARACTER_NAME()
			: TCP_MSG_BASE(CMD_REP_MODIFY_CHARACTER_NAME)
		{}
	};

	struct REP_CHARACTER_SKILL_INFO_HEADER : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::QueryCharacterName	char_name;		// ��ɫ��
		bool						allSend;		// �Ƿ������

		REP_CHARACTER_SKILL_INFO_HEADER()
			: TCP_MSG_BASE(CMD_REP_CHARACTER_SKILL)
		{}
	};

	struct CHARACTER_SKILL_INFO_ITEM
	{
		unsigned int		skillId;							// ����ID
		unsigned char		skillLevel;							// ���ܵȼ� 0����ʾ��δѧ
		char				skillName[ADMIN::MAX_ID_LENGTH];	// ��������
	};

	typedef ADMIN::VarLenStruct<REP_CHARACTER_SKILL_INFO_HEADER, CHARACTER_SKILL_INFO_ITEM, 20>
		REP_CHARACTER_SKILL_INFO;

	// ��ɫ������Ϣ
	struct REP_QUERY_QUEST_INFO_HEADER : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;			// ����id
		unsigned int				srv_cluster_id;			// ��������id
		ADMIN::QueryCharacterName	char_name;				// ��ɫ��
		bool						allSend;				// �Ƿ������

		REP_QUERY_QUEST_INFO_HEADER()
			: TCP_MSG_BASE(CMD_REP_QUERY_QUEST_INFO)
		{}
	};

	struct CHARACTER_QUEST_INFO
	{
		enum CHARACTER_QUEST_TYPE
		{
			CQT_CURRENT,	// �ѽ���δ���������
			CQT_FINISHED,	// ����ɵ�����
		};

		CHARACTER_QUEST_TYPE	quest_type;									// ��������
		ADMIN::DATE_TIME		date_time;									// ʱ��
		unsigned short			quest_id;									// ����ID
		char					quest_name[ADMIN::MAX_QUEST_NAME_LENGTH];	// ��������
	};

	typedef ADMIN::VarLenStruct<REP_QUERY_QUEST_INFO_HEADER, CHARACTER_QUEST_INFO, 50>
		REP_QUERY_QUEST_INFO;

	// Ӷ������Ϣ
	struct REP_QUERY_GUILD_INFO_HEADER : public ADMIN::TCP_MSG_BASE
	{
		unsigned int		srv_realm_id;	// ����id
		unsigned int		srv_cluster_id;	// ��������id
		bool				allSend;		// �Ƿ������

		REP_QUERY_GUILD_INFO_HEADER()
			: TCP_MSG_BASE(CMD_REP_QUERY_GUILD_INFO)
		{}
	};

	struct GUILD_INFO_ELE
	{
		ADMIN::GuildId		guild_id;			// ����ID
		ADMIN::GuildName	guild_name;			// ������
		unsigned char		guild_level;		// ����ȼ�
		unsigned short		guild_member_count;	// ��������
		ADMIN::DATE_TIME	found_time;			// ����ʱ��
	};

	typedef ADMIN::VarLenStruct<REP_QUERY_GUILD_INFO_HEADER, GUILD_INFO_ELE, 50>
		REP_QUERY_GUILD_INFO;

	// Ӷ���ų�Ա�б�
	struct REP_QUERY_GUILD_MEMBER_HEADER : public ADMIN::TCP_MSG_BASE
	{
		unsigned int		srv_realm_id;		// ����id
		unsigned int		srv_cluster_id;		// ��������id
		ADMIN::GuildId		guild_id;			// ����ID
		ADMIN::GuildName	guild_name;			// ������
		bool				allSend;			// �Ƿ������

		REP_QUERY_GUILD_MEMBER_HEADER()
			: TCP_MSG_BASE(CMD_REP_QUERY_GUILD_MEMBER)
		{}
	};

	struct GUILD_MEMBER_INFO
	{
		ADMIN::QueryCharacterId			char_id;			// ��ɫID
		ADMIN::QueryCharacterName		char_name;			// ��ɫ����
		ADMIN::GUILD_MEMBERSHIP_LEVEL	membership_level;	// ְ��
		ADMIN::DATE_TIME				join_time;			// ����ʱ��
	};

	typedef ADMIN::VarLenStruct<REP_QUERY_GUILD_MEMBER_HEADER, GUILD_MEMBER_INFO, 50>
		REP_QUERY_GUILD_MEMBER;

	// ��ӳ���
	struct REP_ADD_PET : public ADMIN::TCP_MSG_BASE
	{
		enum ADD_PET_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,// �ɹ�
			E_FAILED_SERVERINTERNALERROR,		// �������ڲ�����
			E_NOT_FOUND,						// û�ҵ���ɫ
			E_ERROR_PET_TYPE,					// ���Ϸ��ĳ�������
			E_PET_FULL,							// ��������
		};

		ADD_PET_RESULT				eResult;		// ���
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����
		unsigned int				pet_type_id;	// ���������ID

		REP_ADD_PET()
			: TCP_MSG_BASE(CMD_REP_ADD_PET)
		{}
	};

	// �Ƴ�����
	struct REP_REMOVE_PET : public ADMIN::TCP_MSG_BASE
	{
		enum REMOVE_PET_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,// �ɹ�
			E_FAILED_SERVERINTERNALERROR,		// �������ڲ�����
			E_NOT_FOUND,						// û�ҵ�����
		};

		REMOVE_PET_RESULT			eResult;		// ���
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����
		unsigned __int64			pet_unique_id;	// ����ΨһID

		REP_REMOVE_PET()
			: TCP_MSG_BASE(CMD_REP_REMOVE_PET)
		{}
	};

	// �޸ĳ�������
	struct REP_MODIFY_PET : public ADMIN::TCP_MSG_BASE
	{
		enum MODIFY_PET_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,// �ɹ�
			E_FAILED_SERVERINTERNALERROR,		// �������ڲ�����
			E_NOT_FOUND,						// û�ҵ�����
		};

		MODIFY_PET_RESULT			eResult;		// ���
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����
		unsigned __int64			pet_unique_id;	// ����ΨһID
		ADMIN::MOD_CHAR_ATTR		attr;			// Ҫ�޸ĵ�����
		unsigned int				newValue;		// �޸�Ϊ�µ�ֵ

		REP_MODIFY_PET()
			: TCP_MSG_BASE(CMD_REP_MODIFY_PET)
		{}
	};

	struct REP_GUILD_CHANGE_MASTER : public ADMIN::TCP_MSG_BASE
	{
		enum CHANGE_MASTER_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,// �ɹ�
			E_FAILED_SERVERINTERNALERROR,		// �������ڲ�����
			E_NOT_FOUND,						// û�ҵ�����
			E_NOT_FOUND_CHARACTER,				// û�ҵ����
		};

		CHANGE_MASTER_RESULT		eResult;		// ���
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::GuildName			guld_name;		// ������
		ADMIN::QueryCharacterName	char_name;		// �»᳤�Ľ�ɫ����

		REP_GUILD_CHANGE_MASTER()
			: TCP_MSG_BASE(CMD_REP_GUILD_CHANGE_MASTER)
		{}
	};

	// �޸Ĺ�������
	struct REP_GUILD_CHANGE_NAME : public ADMIN::TCP_MSG_BASE
	{
		enum CHANGE_NAME_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,// �ɹ�
			E_FAILED_SERVERINTERNALERROR,		// �������ڲ�����
			E_NOT_FOUND,						// û�ҵ�����
			E_NOT_FOUND_CHARACTER,				// û�ҵ����
		};

		CHANGE_NAME_RESULT		eResult;		// ���
		unsigned int			srv_realm_id;	// ����id
		unsigned int			srv_cluster_id;	// ��������id
		ADMIN::GuildName		guld_name;		// ������
		ADMIN::GuildName		new_name;		// �µĹ�����

		REP_GUILD_CHANGE_NAME()
			: TCP_MSG_BASE(CMD_REP_GUILD_CHANGE_NAME)
		{}
	};

	// �߳������Ա
	struct REP_GUILD_KICK_MEMBER : public ADMIN::TCP_MSG_BASE
	{
		enum KICK_MEMBER_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,// �ɹ�
			E_FAILED_SERVERINTERNALERROR,		// �������ڲ�����
			E_NOT_FOUND,						// û�ҵ�����
			E_NOT_FOUND_CHARACTER,				// û�ҵ����
		};

		KICK_MEMBER_RESULT			eResult;		// ���
		unsigned int				srv_realm_id;	// ����id
		unsigned int				srv_cluster_id;	// ��������id
		ADMIN::GuildName			guld_name;		// ������
		ADMIN::QueryCharacterName	char_name;		// Ҫ�ߵĽ�ɫ����

		REP_GUILD_KICK_MEMBER()
			: TCP_MSG_BASE(CMD_REP_GUILD_KICK_MEMBER)
		{}
	};

	// ��ɢ����
	struct REP_GUILD_DISMISS : public ADMIN::TCP_MSG_BASE
	{
		enum DISMISS_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,// �ɹ�
			E_FAILED_SERVERINTERNALERROR,		// �������ڲ�����
			E_NOT_FOUND,						// û�ҵ�����
		};

		DISMISS_RESULT			eResult;		// ���
		unsigned int			srv_realm_id;	// ����id
		unsigned int			srv_cluster_id;	// ��������id
		ADMIN::GuildName		guld_name;		// ������

		REP_GUILD_DISMISS()
			: TCP_MSG_BASE(CMD_REP_GUILD_DISMISS)
		{}
	};

	// �ָ�����
	struct REP_RECOVER_PASSW : public ADMIN::TCP_MSG_BASE
	{
		enum RECOVER_PASSW_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,// �ɹ�
			E_FAILED_SERVERINTERNALERROR,		// �������ڲ�����
			E_NOT_FOUND,
		};

		RECOVER_PASSW_RESULT		eResult;
		unsigned int			srv_realm_id;		// ����id
		ADMIN::QueryUserName	user_name;			// �û��ʺ�
													// ��¼����
		char					old_password[33];	// ������

		REP_RECOVER_PASSW()
			: TCP_MSG_BASE(CMD_REP_RECOVER_PASSW)
		{}
	};
				
	//////////////////////////////////////////////////////////////////////////
	//

	struct REP_USER_LOG_BASE : public ADMIN::TCP_MSG_BASE
	{
		unsigned int					srv_realm_id;		// ����id
		unsigned int					srv_cluster_id;		// ��������id
		bool							all_send;			// �Ƿ������
		ADMIN::DATE_TIME				start_time;			// ��ʼʱ��
		ADMIN::DATE_TIME				end_time;			// ����ʱ��
		ADMIN::QUERY_CONDITION			query_condition;	// ��ѯ����
		union
		{
			ADMIN::QueryUserName		user_name;			// �û��ʺ�
			ADMIN::QueryCharacterName	char_name;			// ��ɫ����
			ADMIN::QueryCharacterId		char_id;			// ��ɫID
		};

		REP_USER_LOG_BASE(unsigned short id)
			: TCP_MSG_BASE(id)
		{}
	};

	struct REP_GUILD_LOG_BASE : public ADMIN::TCP_MSG_BASE
	{
		unsigned int					srv_realm_id;		// ����id
		unsigned int					srv_cluster_id;		// ��������id
		bool							all_send;			// �Ƿ������
		ADMIN::DATE_TIME				start_time;			// ��ʼʱ��
		ADMIN::DATE_TIME				end_time;			// ����ʱ��
		ADMIN::QUERY_CONDITION			query_condition;	// ��ѯ����
		union
		{
			ADMIN::GuildName	guild_name;		// ��������
			ADMIN::GuildId		guild_id;		// ����ID
		};

		REP_GUILD_LOG_BASE(unsigned short id)
			: TCP_MSG_BASE(id)
		{}
	};

	//////////////////////////////////////////////////////////////////////////
	// �������
	struct REP_LOG_MONSTER_DROP_ITEM_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;			// ��ɫID
		ADMIN::QueryCharacterName	char_name;			// ��ɫ����

		REP_LOG_MONSTER_DROP_ITEM_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_MONSTER_DROP_ITEM)
		{}
	};

	struct REP_LOG_MONSTER_DROP_ITEM_ELE
	{
		ADMIN::DATE_TIME			date_time;			// ���ʱ��
		unsigned int				item_type_id;		// ��Ʒ����ID
														// ��Ʒ��
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64			item_unique_id;		// ��ƷΨһID
		unsigned char				item_count;			// ����
	};

	typedef ADMIN::VarLenStruct<REP_LOG_MONSTER_DROP_ITEM_HEADER, REP_LOG_MONSTER_DROP_ITEM_ELE, 20>
		REP_LOG_MONSTER_DROP_ITEM;

	//////////////////////////////////////////////////////////////////////////
	// ��NPC������
	struct REP_LOG_BUY_ITEM_FROM_NPC_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;			// ��ɫID
		ADMIN::QueryCharacterName	char_name;			// ��ɫ����

		REP_LOG_BUY_ITEM_FROM_NPC_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_BUY_ITEM_FROM_NPC)
		{}
	};

	struct REP_LOG_BUY_ITEM_FROM_NPC_ELE
	{
		ADMIN::DATE_TIME			date_time;			// ���ʱ��
		unsigned int				item_type_id;		// ��Ʒ����ID
														// ��Ʒ����
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];		
		unsigned __int64			item_unique_id;		// ��ƷΨһID
		unsigned char				item_count;			// ����
		unsigned int				cost_money;			// ������Ϸ������
	};

	typedef ADMIN::VarLenStruct<REP_LOG_BUY_ITEM_FROM_NPC_HEADER, REP_LOG_BUY_ITEM_FROM_NPC_ELE, 20>
		REP_LOG_BUY_ITEM_FROM_NPC;

	//////////////////////////////////////////////////////////////////////////
	// ����NPC
	struct REP_LOG_SELL_ITEM_TO_NPC_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;			// ��ɫID
		ADMIN::QueryCharacterName	char_name;			// ��ɫ����

		REP_LOG_SELL_ITEM_TO_NPC_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_SELL_ITEM_TO_NPC)
		{}
	};

	struct REP_LOG_SELL_ITEM_TO_NPC_ELE
	{
		ADMIN::DATE_TIME			date_time;			// ����ʱ��
		unsigned int				item_type_id;		// ��Ʒ����ID
														// ��Ʒ����
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64			item_unique_id;		// ��ƷΨһID
		unsigned char				item_count;			// ����
		unsigned int				getMoney;			// �õ���Ϸ������
	};

	typedef ADMIN::VarLenStruct<REP_LOG_SELL_ITEM_TO_NPC_HEADER, REP_LOG_SELL_ITEM_TO_NPC_ELE, 20>
		REP_LOG_SELL_ITEM_TO_NPC;

	//////////////////////////////////////////////////////////////////////////
	// �������ɾ��
	struct REP_LOG_DROP_ITEM_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;			// ��ɫID
		ADMIN::QueryCharacterName	char_name;			// ��ɫ����

		REP_LOG_DROP_ITEM_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_DROP_ITEM)
		{}
	};

	struct REP_LOG_DROP_ITEM_ELE
	{
		ADMIN::DATE_TIME			date_time;			// ɾ��ʱ��
		unsigned int				item_type_id;		// ��Ʒ����ID
														// ��Ʒ����
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64			item_unique_id;		// ��ƷΨһID
		unsigned char				item_count;			// ����
	};

	typedef ADMIN::VarLenStruct<REP_LOG_DROP_ITEM_HEADER, REP_LOG_DROP_ITEM_ELE, 20>
		REP_LOG_DROP_ITEM;

	//////////////////////////////////////////////////////////////////////////
	// ���׻��
	struct REP_LOG_GET_ITEM_TRADE_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		dst_char_id;	// ��ƷĿ���ɫID
		ADMIN::QueryCharacterName	dst_char_name;	// ��ƷĿ���ɫ��

		REP_LOG_GET_ITEM_TRADE_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_GET_ITEM_TRADE)
		{}
	};

	struct REP_LOG_GET_ITEM_TRADE_ELE
	{
		ADMIN::QueryCharacterId		src_char_id;	// ��Դ��ɫID
		ADMIN::QueryCharacterName	src_char_name;	// ��Դ��ɫ����
		ADMIN::DATE_TIME			date_time;		// ����ʱ��
		unsigned int				item_type_id;	// ��Ʒ����ID
													// ��Ʒ����
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64			item_unique_id;	// ��ƷΨһID
		unsigned char				item_count;		// ����
	};

	typedef ADMIN::VarLenStruct<REP_LOG_GET_ITEM_TRADE_HEADER, REP_LOG_GET_ITEM_TRADE_ELE, 20>
		REP_LOG_GET_ITEM_TRADE;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_LOSE_ITEM_TRADE_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		src_char_id;	// ��ɫID
		ADMIN::QueryCharacterName	src_char_name;	// ��ɫ����

		REP_LOG_LOSE_ITEM_TRADE_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_LOSE_ITEM_TRADE)
		{}
	};

	struct REP_LOG_LOSE_ITEM_TRADE_ELE
	{
		ADMIN::QueryCharacterId		dst_char_id;	// ��ƷĿ���ɫID
		ADMIN::QueryCharacterName	dst_char_name;	// ��ƷĿ���ɫ��
		ADMIN::DATE_TIME			date_time;		// ����ʱ��
		unsigned int				item_type_id;	// ��Ʒ����ID
													// ��Ʒ����
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64			item_unique_id;	// ��ƷΨһID
		unsigned char				item_count;		// ����
	};

	typedef ADMIN::VarLenStruct<REP_LOG_LOSE_ITEM_TRADE_HEADER, REP_LOG_LOSE_ITEM_TRADE_ELE, 20>
		REP_LOG_LOSE_ITEM_TRADE;

	//////////////////////////////////////////////////////////////////////////
	// װ���󶨼�¼
	struct REP_LOG_BIND_ITEM_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;				// ��ɫID
		ADMIN::QueryCharacterName	char_name;				// ��ɫ����

		REP_LOG_BIND_ITEM_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_BIND_ITEM)
		{}
	};

	struct REP_LOG_BIND_ITEM_ELE
	{
		ADMIN::DATE_TIME			date_time;				// ��ʱ��
		unsigned int				item_type_id;			// ��Ʒ����ID
															// ��Ʒ����
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64			item_unique_id;			// ��ƷΨһID
		unsigned char				bind_reward[2];			// �󶨽���
		unsigned int				bind_reward_value[2];	// �󶨽�����ֵ
	};

	typedef ADMIN::VarLenStruct<REP_LOG_BIND_ITEM_HEADER, REP_LOG_BIND_ITEM_ELE, 20>
		REP_LOG_BIND_ITEM;

	//////////////////////////////////////////////////////////////////////////
	// װ����󶨼�¼
	struct REP_LOG_UNBIND_ITEM_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;				// ��ɫID
		ADMIN::QueryCharacterName	char_name;				// ��ɫ����

		REP_LOG_UNBIND_ITEM_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_UNBIND_ITEM)
		{}
	};

	struct REP_LOG_UNBIND_ITEM_ELE
	{
		ADMIN::DATE_TIME			date_time;				// ���ʱ��
		unsigned int				item_type_id;			// ��Ʒ����ID
															// ��Ʒ����
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64			item_unique_id;			// ��ƷΨһID
		unsigned char				bind_reward[2];			// �󶨽���
		unsigned int				bind_reward_value[2];	// �󶨽�����ֵ
		unsigned int				consume_item_type_id;	// ���������ƷID
	};

	typedef ADMIN::VarLenStruct<REP_LOG_UNBIND_ITEM_HEADER, REP_LOG_UNBIND_ITEM_ELE, 20>
		REP_LOG_UNBIND_ITEM;

	//////////////////////////////////////////////////////////////////////////
	// ��̯��־
	struct REP_LOG_BOOTH_ITEM_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;				// ��ɫID
		ADMIN::QueryCharacterName	char_name;				// ��ɫ����

		REP_LOG_BOOTH_ITEM_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_BOOTH_ITEM)
		{}
	};

	struct BOOTH_LOG_OPEN
	{

	};

	struct BOOTH_LOG_CLOSE
	{

	};

	struct BOOTH_LOG_SELL
	{
		ADMIN::QueryCharacterId		dst_char_id;	// Ŀ���ɫID
		ADMIN::QueryCharacterName	dst_char_name;	// Ŀ���ɫ��
		unsigned int				item_type_id;	// ��Ʒ����ID
													// ��Ʒ����
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned char				item_count;		// ����
		unsigned int				item_price;		// �۸�
	};

	struct BOOTH_LOG_BUY
	{
		ADMIN::QueryCharacterId		src_char_id;	// ��Դ��ɫID��ɫID
		ADMIN::QueryCharacterName	src_char_name;	// ��Դ��ɫID��ɫ��
		unsigned int				item_type_id;	// ��Ʒ����ID
													// ��Ʒ����
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned char				item_count;		// ����
		unsigned int				item_price;		// �۸�
	};

	struct REP_LOG_BOOTH_ITEM_ELE
	{
		enum BOOTH_LOG_TYPE
		{
			BOOTH_OPEN,						// ����
			BOOTH_SELL,						// ̯������
			BOOTH_BUY,						// ̯�����
			BOOTH_CLOSE,					// ��̯
		};

		BOOTH_LOG_TYPE		booth_type;		// ��������
		ADMIN::DATE_TIME	date_time;		// ����ʱ��
		union
		{
			BOOTH_LOG_OPEN	booth_open;		// ����
			BOOTH_LOG_CLOSE	booth_close;	// ̯������
			BOOTH_LOG_SELL	booth_sell;		// ̯�����
			BOOTH_LOG_BUY	booth_buy;		// ��̯
		};
	};

	typedef ADMIN::VarLenStruct<REP_LOG_BOOTH_ITEM_HEADER, REP_LOG_BOOTH_ITEM_ELE, 20>
		REP_LOG_BOOTH_ITEM;

	//////////////////////////////////////////////////////////////////////////
	// ɾ����Ʒ
	struct REP_LOG_DELETE_ITEM_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_DELETE_ITEM_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_DELETE_ITEM)
		{}
	};

	struct REP_LOG_DELETE_ITEM_ELE
	{
		ADMIN::DATE_TIME			date_time;		// ɾ��ʱ��
		unsigned int				item_type_id;	// ɾ����Ʒ����ID
													// ��Ʒ����
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64			item_unique_id;	// ��ƷΨһID
		unsigned int				item_time_limit;// ��Ʒ��ʹ������
	};

	typedef ADMIN::VarLenStruct<REP_LOG_DELETE_ITEM_HEADER, REP_LOG_DELETE_ITEM_ELE, 20>
		REP_LOG_DELETE_ITEM;

	//////////////////////////////////////////////////////////////////////////
	// ʹ����Ʒ
	struct REP_LOG_USE_ITEM_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_USE_ITEM_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_USE_ITEM)
		{}
	};

	struct REP_LOG_USE_ITEM_ELE
	{
		ADMIN::DATE_TIME			date_time;		// ʹ��ʱ��
		unsigned int				item_type_id;	// ʹ����Ʒ����ID
													// ʹ����Ʒ����
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64			item_unique_id;	// ʹ����ƷΨһID
		unsigned char				item_count;		// ��Ʒ��ʹ������
	};

	typedef ADMIN::VarLenStruct<REP_LOG_USE_ITEM_HEADER, REP_LOG_USE_ITEM_ELE, 20>
		REP_LOG_USE_ITEM;

	//////////////////////////////////////////////////////////////////////////
	// װ������
	struct REP_LOG_CRAFT_ITEM_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_CRAFT_ITEM_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_CRAFT_ITEM)
		{}
	};

	struct REP_LOG_CRAFT_ITEM_ELE
	{
		enum CRAFT_TYPE
		{
			CRAFT_BLACKSMITH,					// ����
			CRAFT_SEWING,						// �÷�
			CRAFT_HANDCRAFT,					// �ֹ�
		};

		ADMIN::DATE_TIME	date_time;			// ����ʱ��
		CRAFT_TYPE			craft_type;			// װ����������
		unsigned int		recipe_id;			// ͼֽID
		char				recipe_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned int		material_id[6];		// ����ID
		char				material_name[6][ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned int		product_id;			// ��ƷID
												// ��Ʒ����
		char				product_name[ADMIN::MAX_ITEM_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_CRAFT_ITEM_HEADER, REP_LOG_CRAFT_ITEM_ELE, 20>
		REP_LOG_CRAFT_ITEM;

	//////////////////////////////////////////////////////////////////////////
	// ���
	struct REP_LOG_PUNCH_ITEM_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_PUNCH_ITEM_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_PUNCH_ITEM)
		{}
	};

	struct REP_LOG_PUNCH_ITEM_ELE
	{
		enum CONSUME_HAMMER_LEVEL
		{
			LOW_LEVEL_HAMMER,						// ��������
			MIDDLE_LEVEL_HAMMER,					// �м�����
			HIGH_LEVEL_HAMMER,						// �߼�����
		};

		enum CONSUME_STITHY_LEVEL
		{
			LOW_LEVEL_STITHY,						// ��������
			MIDDLE_LEVEL_STITHY,					// �м�����
			HIGH_LEVEL_STITHY,						// �߼�����
		};

		ADMIN::DATE_TIME		date_time;			// ���ʱ��
		CONSUME_HAMMER_LEVEL	consume_hammer;		// ��������
		CONSUME_STITHY_LEVEL	consume_stithy;		// ��������
		unsigned int			consume_money;		// ���Ľ�Ǯ
		unsigned int			item_type_id;		// ��Ʒ����ID
													// ��Ʒ����
		char					item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64		item_unique_id;		// ��ƷΨһID
	};

	typedef ADMIN::VarLenStruct<REP_LOG_PUNCH_ITEM_HEADER, REP_LOG_PUNCH_ITEM_ELE, 20>
		REP_LOG_PUNCH_ITEM;

	//////////////////////////////////////////////////////////////////////////
	// �Ŀ�
	struct REP_LOG_CHANGE_SLOT_ITEM_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_CHANGE_SLOT_ITEM_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_CHANGE_SLOT_ITEM)
		{}
	};

	struct REP_LOG_CHANGE_SLOT_ITEM_ELE
	{
		ADMIN::DATE_TIME		date_time;			// �Ŀ�ʱ��
		ADMIN::GEM_COLOR		old_color;			// �Ŀ�ǰ��ɫ
		ADMIN::GEM_COLOR		new_color;			// �Ŀ׺���ɫ
		unsigned int			consume_money;		// ���Ľ�Ǯ
		unsigned int			item_type_id;		// ��Ʒ����ID
													// ��Ʒ����
		char					item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64		item_unique_id;		// ��ƷΨһID
	};

	typedef ADMIN::VarLenStruct<REP_LOG_CHANGE_SLOT_ITEM_HEADER, REP_LOG_CHANGE_SLOT_ITEM_ELE, 20>
		REP_LOG_CHANGE_SLOT_ITEM;

	//////////////////////////////////////////////////////////////////////////
	// ����
	struct REP_LOG_UPGRADE_STAR_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_UPGRADE_STAR_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_UPGRADE_STAR)
		{}
	};

	struct REP_LOG_UPGRADE_STAR_ELE
	{
		ADMIN::DATE_TIME		date_time;			// �Ŀ�ʱ��
		unsigned char			old_star_count;		// ����ǰ����
		unsigned char			new_star_count;		// ���Ǻ�����
		unsigned int			consume_money;		// ���Ľ�Ǯ
		unsigned int			item_type_id;		// ��Ʒ����ID
													// ��Ʒ����
		char					item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64		item_unique_id;		// ��ƷΨһID
	};

	typedef ADMIN::VarLenStruct<REP_LOG_UPGRADE_STAR_HEADER, REP_LOG_UPGRADE_STAR_ELE, 20>
		REP_LOG_UPGRADE_STAR;

	//////////////////////////////////////////////////////////////////////////
	// ��ʯ
	struct REP_LOG_GEM_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_GEM_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_GEM)
		{}
	};

	struct REP_LOG_GEM_ELE
	{
		enum GEM_OPERATION
		{
			GEM_ADD,								// ��Ƕ
			GEM_REMOVE,								// ȥ��
		};

		GEM_OPERATION			gem_operation;		// ����
		ADMIN::DATE_TIME		date_time;			// �Ŀ�ʱ��
		ADMIN::GEM_COLOR		gem_type;			// ��ʯ����
		unsigned char			gem_level;			// ��ʯ�ȼ�
		unsigned int			consume_money;		// ���Ľ�Ǯ
		unsigned int			item_type_id;		// ��Ʒ����ID
													// ��Ʒ����
		char					item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64		item_unique_id;		// ��ƷΨһID
	};

	typedef ADMIN::VarLenStruct<REP_LOG_GEM_HEADER, REP_LOG_GEM_ELE, 20>
		REP_LOG_GEM;

	//////////////////////////////////////////////////////////////////////////
	// ��ҩ
	struct REP_LOG_PHARMA_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_PHARMA_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_PHARMA)
		{}
	};

	struct REP_LOG_PHARMA_ELE
	{
		ADMIN::DATE_TIME	date_time;				// ����ʱ��
		unsigned int		recipe_id;				// ͼֽID
		char				recipe_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned int		material_id[6];			// ����ID
		char				material_name[6][ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned int		product_id;				// ��ƷID
													// ��Ʒ����
		char				product_name[ADMIN::MAX_ITEM_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_PHARMA_HEADER, REP_LOG_PHARMA_ELE, 20>
		REP_LOG_PHARMA;

	//////////////////////////////////////////////////////////////////////////
	// ��ʯ�ϳ�
	struct REP_LOG_CRAFT_GEM_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_CRAFT_GEM_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_CRAFT_GEM)
		{}
	};

	struct REP_LOG_CRAFT_GEM_ELE
	{
		ADMIN::DATE_TIME	date_time;				// �ϳ�ʱ��
		unsigned int		consume_money;			// ���Ľ�Ǯ
		unsigned int		prop_item_id;			// �ɹ��ʵ���
		unsigned int		material_id;			// ԭ����ID
													// ԭ��������
		char				material_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned int		product_id;				// ��ƷID
		char				product_name[ADMIN::MAX_ITEM_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_CRAFT_GEM_HEADER, REP_LOG_CRAFT_GEM_ELE, 20>
		REP_LOG_CRAFT_GEM;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_CRAFT_MATIRAIL_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_CRAFT_MATIRAIL_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_CRAFT_MATIRAIL)
		{}
	};

	struct REP_LOG_CRAFT_MATIRAIL_ELE
	{
		ADMIN::DATE_TIME	date_time;				// �ϳ�ʱ��
		unsigned int		consume_money;			// ���Ľ�Ǯ
		unsigned int		prop_item_id;			// �ɹ��ʵ���
		unsigned int		material_id;			// ԭ����ID
													// ԭ��������
		char				material_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned int		product_id;				// ��ƷID
		char				product_name[ADMIN::MAX_ITEM_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_CRAFT_MATIRAIL_HEADER, REP_LOG_CRAFT_MATIRAIL_ELE, 20>
		REP_LOG_CRAFT_MATIRAIL;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_CRAFT_RECIPE_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_CRAFT_RECIPE_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_CRAFT_RECIPE)
		{}
	};

	struct REP_LOG_CRAFT_RECIPE_ELE
	{
		ADMIN::DATE_TIME	date_time;				// �ϳ�ʱ��
		unsigned int		consume_money;			// ���Ľ�Ǯ
		unsigned int		prop_item_id;			// �ɹ��ʵ���
		unsigned int		material_id;			// ԭ����ID
													// ԭ��������
		char				material_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned int		product_id;				// ��ƷID
		char				product_name[ADMIN::MAX_ITEM_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_CRAFT_RECIPE_HEADER, REP_LOG_CRAFT_RECIPE_ELE, 20>
		REP_LOG_CRAFT_RECIPE;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_MAZE_INTEGRAL_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_MAZE_INTEGRAL_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_MAZE_INTEGRAL)
		{}
	};

	struct REP_LOG_MAZE_INTEGERAL_ELE
	{
		enum CHANGE_TYPE
		{
			CHANGE_EXP,							// �һ�����
			CHANGE_MONEY,						// �һ���Ǯ
		};

		ADMIN::DATE_TIME	date_time;			// �һ�ʱ��
		CHANGE_TYPE			change_type;		// �һ�����
		unsigned int		consume_integral;	// ���Ļ���
		unsigned int		change_point;		// �õ��ľ������Ǯ
	};

	typedef ADMIN::VarLenStruct<REP_LOG_MAZE_INTEGRAL_HEADER, REP_LOG_MAZE_INTEGERAL_ELE, 20>
		REP_LOG_MAZE_INTEGRAL;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_MONSTER_DROP_MONEY_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_MONSTER_DROP_MONEY_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_MONSTER_DROP_MONEY)
		{}
	};

	struct REP_LOG_MONSTER_DROP_MONEY_ELE
	{
		ADMIN::DATE_TIME	date_time;			// ���ʱ��
		unsigned int		get_money;			// ��ý�Ǯ����
		unsigned int		src_id;				// ��ԴID
												// ��Դ��
		char				src_name[ADMIN::MAX_CHARACTER_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_MONSTER_DROP_MONEY_HEADER, REP_LOG_MONSTER_DROP_MONEY_ELE, 20>
		REP_LOG_MONSTER_DROP_MONEY;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_GET_MONEY_QUEST_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_GET_MONEY_QUEST_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_GET_MONEY_QUEST)
		{}
	};

	struct REP_LOG_GET_MONEY_QUEST_ELE
	{
		ADMIN::DATE_TIME	date_time;			// ���ʱ��
		unsigned int		get_money;			// ��ý�Ǯ����
		unsigned short		src_id;				// ��ԴID
												// ��Դ��
		char				src_name[ADMIN::MAX_QUEST_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_GET_MONEY_QUEST_HEADER, REP_LOG_GET_MONEY_QUEST_ELE, 20>
		REP_LOG_GET_MONEY_QUEST;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_LOSE_MONEY_REPAIR_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_LOSE_MONEY_REPAIR_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_LOSE_MONEY_REPAIR)
		{}
	};

	struct REP_LOG_LOSE_MONEY_REPAIR_ELE
	{
		ADMIN::DATE_TIME	date_time;			// ʱ��
		unsigned int		consume_money;		// ���Ľ�Ǯ
		unsigned int		item_type_id;		// ��Ʒ����
												// ��Ʒ��
		char				item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_LOSE_MONEY_REPAIR_HEADER, REP_LOG_LOSE_MONEY_REPAIR_ELE, 20>
		REP_LOG_LOSE_MONEY_REPAIR;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_GET_MONEY_TRADE_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_GET_MONEY_TRADE_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_GET_MONEY_TRADE)
		{}
	};

	struct REP_LOG_GET_MONEY_TRADE_ELE
	{
		ADMIN::DATE_TIME	date_time;				// ʱ��
		unsigned int		get_money;				// ��Ǯ����
		unsigned int		src_id;					// ��ԴID
													// ��Դ����
		char				src_name[ADMIN::MAX_CHARACTER_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_GET_MONEY_TRADE_HEADER, REP_LOG_GET_MONEY_TRADE_ELE, 20>
		REP_LOG_GET_MONEY_TRADE;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_LOSE_MONEY_TRADE_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_LOSE_MONEY_TRADE_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_LOSE_MONEY_TRADE)
		{}
	};

	struct REP_LOG_LOSE_MONEY_TRADE_ELE
	{
		ADMIN::DATE_TIME	date_time;				// ʱ��
		unsigned int		lose_money;				// ��Ǯ����
		unsigned int		dst_id;					// ��ԴID
													// ��Դ����
		char				dst_name[ADMIN::MAX_CHARACTER_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_LOSE_MONEY_TRADE_HEADER, REP_LOG_LOSE_MONEY_TRADE_ELE, 20>
		REP_LOG_LOSE_MONEY_TRADE;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_QUEST_REWARD_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_QUEST_REWARD_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_QUEST_REWARD)
		{}
	};

	struct REP_LOG_QUEST_REWARD_ELE
	{
		ADMIN::DATE_TIME	date_time;				// ʱ��
		unsigned short		quest_id;				// ����ID
		unsigned int		reward_money;			// �����Ľ�Ǯ
													// ��������
		char				quest_name[ADMIN::MAX_QUEST_NAME_LENGTH];
		unsigned int		reward_item_id;			// ��������Ʒ
													// ��������Ʒ����
		char				reward_item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_QUEST_REWARD_HEADER, REP_LOG_QUEST_REWARD_ELE, 20>
		REP_LOG_QUEST_REWARD;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_QUEST_GIVE_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_QUEST_GIVE_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_QUEST_GIVE)
		{}
	};

	struct REP_LOG_QUEST_GIVE_ELE
	{
		ADMIN::DATE_TIME	date_time;				// ʱ��
		unsigned short		quest_id;				// ����ID
													// ��������
		char				quest_name[ADMIN::MAX_QUEST_NAME_LENGTH];
		unsigned int		give_item_id;			// ʧȥ����ƷID
													// ʧȥ����Ʒ����
		char				give_item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_QUEST_GIVE_HEADER, REP_LOG_QUEST_GIVE_ELE, 20>
		REP_LOG_QUEST_GIVE;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_MAIL_SEND_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_MAIL_SEND_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_MAIL_SEND)
		{}
	};

	struct REP_LOG_MAIL_SEND_ELE
	{
		ADMIN::DATE_TIME			date_time;			// ʱ��
		unsigned __int64			mail_id;			// �ʼ�ID
		unsigned int				mail_money;			// ��Ǯ����
		unsigned int				dst_id;				// �ռ���ID
														// �ռ�������
		char						dst_name[ADMIN::MAX_CHARACTER_NAME_LENGTH];
		unsigned char				mail_item_count[5];	// ��Ʒ����
														// ��Ʒ��
		char						mail_item[5][ADMIN::MAX_ITEM_NAME_LENGTH];
														// �ʼ�����
		char						mail_title[ADMIN::MAX_MAIL_TITLE];
		
	};

	typedef ADMIN::VarLenStruct<REP_LOG_MAIL_SEND_HEADER, REP_LOG_MAIL_SEND_ELE, 20>
		REP_LOG_MAIL_SEND;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_MAIL_RECV_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_MAIL_RECV_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_MAIL_RECV)
		{}
	};

	struct REP_LOG_MAIL_RECV_ELE
	{
		ADMIN::DATE_TIME			date_time;			// ʱ��
		unsigned __int64			mail_id;			// �ʼ�ID
		unsigned int				mail_money;			// ��Ǯ����
		unsigned int				src_id;				// ������ID
														// ����������
		char						src_name[ADMIN::MAX_CHARACTER_NAME_LENGTH];
		unsigned char				mail_item_count[5];	// ��Ʒ����
														// ��Ʒ��
		char						mail_item[5][ADMIN::MAX_ITEM_NAME_LENGTH];
														// �ʼ�����
		char						mail_title[ADMIN::MAX_MAIL_TITLE];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_MAIL_RECV_HEADER, REP_LOG_MAIL_RECV_ELE, 20>
		REP_LOG_MAIL_RECV;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_MAIL_DELETE_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_MAIL_DELETE_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_MAIL_DELETE)
		{}
	};

	struct REP_LOG_MAIL_DELETE_ELE
	{
		ADMIN::DATE_TIME			date_time;			// ʱ��
		unsigned __int64			mail_id;			// �ʼ�ID
		unsigned int				src_id;				// ������ID
														// ����������
		char						src_name[ADMIN::MAX_CHARACTER_NAME_LENGTH];
														// �ʼ�����
		char						mail_title[ADMIN::MAX_MAIL_TITLE];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_MAIL_DELETE_HEADER, REP_LOG_MAIL_DELETE_ELE, 20>
		REP_LOG_MAIL_DELETE;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_ITEM_SHOP_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_ITEM_SHOP_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_ITEM_SHOP)
		{}
	};

	struct REP_LOG_ITEM_SHOP_ELE
	{
		ADMIN::DATE_TIME		date_time;			// ʱ��ʱ��
		unsigned int			item_type_id;		// ��Ʒ����
		char					item_name[ADMIN::MAX_ITEM_NAME_LENGTH];	
		unsigned __int64		item_unique_id;		// ��ƷΨһID
		unsigned char			item_count;			// ��Ʒ����
		unsigned int			comsume_m_money;	// ����M��
		unsigned int			rest_m_money;		// ʣ��M��
	};

	typedef ADMIN::VarLenStruct<REP_LOG_ITEM_SHOP_HEADER, REP_LOG_ITEM_SHOP_ELE, 20>
		REP_LOG_ITEM_SHOP;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_INSTANCE_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_INSTANCE_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_INSTANCE)
		{}
	};

	struct REP_LOG_INSTANCE_ELE
	{
		enum INSTANCE_OPERATION
		{
			INSTANCE_ENTER,							// ���븱��
			INSTANCE_LEAVE,							// �뿪����
		};

		ADMIN::DATE_TIME		date_time;			// ʱ��ʱ��
		INSTANCE_OPERATION		op_type;			// ����
		unsigned short			instance_id;		// ����ID
													// ��������
		char					instance_name[ADMIN::MAX_SCENE_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_INSTANCE_HEADER, REP_LOG_INSTANCE_ELE, 20>
		REP_LOG_INSTANCE;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_LOGIN_LOGOUT_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_LOGIN_LOGOUT_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_LOGIN_LOGOUT)
		{}
	};

	struct REP_LOG_LOGIN_LOGOUT_ELE
	{
		enum LOGIN_LOGOUT_OPEARTION
		{
			LOGIN_CHAR,								// ��ɫ��¼
			LOGOUT_CHAR,							// ��ɫ�ǳ�
		};

		LOGIN_LOGOUT_OPEARTION		op_type;		// ����
		ADMIN::DATE_TIME			date_time;		// ʱ��ʱ��
		unsigned int				char_id;		// ��ɫID
													// ��ɫ����
		char						char_name[ADMIN::MAX_CHARACTER_NAME_LENGTH];
		unsigned int				clientIp;		// IP
	};

	typedef ADMIN::VarLenStruct<REP_LOG_LOGIN_LOGOUT_HEADER, REP_LOG_LOGIN_LOGOUT_ELE, 20>
		REP_LOG_LOGIN_LOGOUT;

	//////////////////////////////////////////////////////////////////////////
	// ��ɫ������¼
	struct REP_LOG_CHAR_LEVELUP_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_CHAR_LEVELUP_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_CHAR_LEVELUP)
		{}
	};

	struct REP_LOG_CHAR_LEVELUP_ELE
	{
		ADMIN::DATE_TIME	date_time;		// ʱ��
		unsigned char		char_level;		// �������ȼ�
	};

	typedef ADMIN::VarLenStruct<REP_LOG_CHAR_LEVELUP_HEADER, REP_LOG_CHAR_LEVELUP_ELE, 20>
		REP_LOG_CHAR_LEVELUP;

	//////////////////////////////////////////////////////////////////////////
	// ��ɫ������¼
	struct REP_LOG_CHAR_CREATE_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_CHAR_CREATE_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_CHAR_CREATE)
		{}
	};

	struct REP_LOG_CHAR_CREATE_ELE
	{
		ADMIN::DATE_TIME	date_time;		// ʱ��
	};

	typedef ADMIN::VarLenStruct<REP_LOG_CHAR_CREATE_HEADER, REP_LOG_CHAR_CREATE_ELE, 20>
		REP_LOG_CHAR_CREATE;

	//////////////////////////////////////////////////////////////////////////
	// ��ɫɾ����¼
	struct REP_LOG_CHAR_DELETE_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_CHAR_DELETE_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_CHAR_DELETE)
		{}
	};

	struct REP_LOG_CHAR_DELETE_ELE
	{
		ADMIN::DATE_TIME	date_time;		// ʱ��
		unsigned char		char_level;		// ɾ��ʱ�Ľ�ɫ�ȼ�
	};

	typedef ADMIN::VarLenStruct<REP_LOG_CHAR_DELETE_HEADER, REP_LOG_CHAR_DELETE_ELE, 20>
		REP_LOG_CHAR_DELETE;

	//////////////////////////////////////////////////////////////////////////
	// ��ɫ�������¼
	struct REP_LOG_CHAR_JOIN_GUILD_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_CHAR_JOIN_GUILD_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_CHAR_JOIN_GUILD)
		{}
	};

	struct REP_LOG_CHAR_JOIN_GUILD_ELE
	{
		ADMIN::DATE_TIME	date_time;	// ʱ��
		ADMIN::GuildId		guild_id;	// ����ID
		ADMIN::GuildName	guild_name;	// ��������
	};

	typedef ADMIN::VarLenStruct<REP_LOG_CHAR_JOIN_GUILD_HEADER, REP_LOG_CHAR_JOIN_GUILD_ELE, 20>
		REP_LOG_CHAR_JOIN_GUILD;

	//////////////////////////////////////////////////////////////////////////
	// ��ɫ�뿪����¼
	struct REP_LOG_CHAR_LEAVE_GUILD_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_CHAR_LEAVE_GUILD_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_CHAR_LEAVE_GUILD)
		{}
	};

	struct REP_LOG_CHAR_LEAVE_GUILD_ELE
	{
		ADMIN::DATE_TIME	date_time;	// ʱ��
		ADMIN::GuildId		guild_id;	// ����ID
		ADMIN::GuildName	guild_name;	// ��������
	};

	typedef ADMIN::VarLenStruct<REP_LOG_CHAR_LEAVE_GUILD_HEADER, REP_LOG_CHAR_LEAVE_GUILD_ELE, 20>
		REP_LOG_CHAR_LEAVE_GUILD;

	//////////////////////////////////////////////////////////////////////////
	// ��ɫ��������¼
	struct REP_LOG_CHAR_CREATE_GUILD_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_CHAR_CREATE_GUILD_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_CHAR_CREATE_GUILD)
		{}
	};

	struct REP_LOG_CHAR_CREATE_GUILD_ELE
	{
		enum CREATE_TYPE
		{
			CT_START_CREATE,			// ��������
			CT_SUCC_CREATE,				// �����ɹ�
			CT_FAIL_CREATE,				// ����ʧ��
		};

		CREATE_TYPE			create_type;	// ����
		ADMIN::DATE_TIME	date_time;		// ʱ��
		ADMIN::GuildId		guild_id;		// ����ID
		ADMIN::GuildName	guild_name;		// ��������
		unsigned int		item_type_id;	// ������ƷID
		char				item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned int		cosume_money;	// ���Ľ�Ǯ
	};

	typedef ADMIN::VarLenStruct<REP_LOG_CHAR_CREATE_GUILD_HEADER, REP_LOG_CHAR_CREATE_GUILD_ELE, 20>
		REP_LOG_CHAR_CREATE_GUILD;

	//////////////////////////////////////////////////////////////////////////
	// ��ɫ��ɢ����¼
	struct REP_LOG_CHAR_DISMISS_GUILD_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_CHAR_DISMISS_GUILD_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_CHAR_DISMISS_GUILD)
		{}
	};

	struct REP_LOG_CHAR_DISMISS_GUILD_ELE
	{
		ADMIN::DATE_TIME	date_time;				// ʱ��
		ADMIN::GuildId		guild_id;				// ����ID
		ADMIN::GuildName	guild_name;				// ��������
	};

	typedef ADMIN::VarLenStruct<REP_LOG_CHAR_DISMISS_GUILD_HEADER, REP_LOG_CHAR_DISMISS_GUILD_ELE, 20>
		REP_LOG_CHAR_DISMISS_GUILD;

	//////////////////////////////////////////////////////////////////////////
	// ��ɫתְ��¼
	struct REP_LOG_CHAR_CHANGE_OCC_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_CHAR_CHANGE_OCC_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_CHAR_CHANGE_OCC)
		{}
	};

	struct REP_LOG_CHAR_CHANGE_OCC_ELE
	{
		ADMIN::DATE_TIME		date_time;			// ʱ��
		ADMIN::OCCUPATION_TYPE	old_occupation;		// ��ְҵ
		ADMIN::OCCUPATION_TYPE	new_occupation;		// ��ְҵ
	};

	typedef ADMIN::VarLenStruct<REP_LOG_CHAR_CHANGE_OCC_HEADER, REP_LOG_CHAR_CHANGE_OCC_ELE, 20>
		REP_LOG_CHAR_CHANGE_OCC;

	//////////////////////////////////////////////////////////////////////////
	// ��ɫѧϰ���ܼ�¼
	struct REP_LOG_CHAR_SKILL_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_CHAR_SKILL_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_CHAR_SKILL)
		{}
	};

	struct REP_LOG_CHAR_SKILL_ELE
	{
		enum LEARN_TYPE
		{
			LT_LEARN_NEW,	// ѧϰ����
			LT_LEVELUP,		// ��������
		};

		ADMIN::DATE_TIME	date_time;							// ʱ��
		LEARN_TYPE			learn_type;							// ѧϰ����
		unsigned int		skill_id;							// ����ID
		char				skill_name[ADMIN::MAX_ID_LENGTH];	// ��������
		unsigned char		skill_level;						// �������ȼ�
		unsigned int		cosume_meony;						// ���Ľ�Ǯ
		unsigned int		cosume_exp;							// ���ľ���
	};

	typedef ADMIN::VarLenStruct<REP_LOG_CHAR_SKILL_HEADER, REP_LOG_CHAR_SKILL_ELE, 20>
		REP_LOG_CHAR_SKILL;

	//////////////////////////////////////////////////////////////////////////
	// ��ɫѧϰ����ܼ�¼
	struct REP_LOG_CHAR_LIVING_SKILL_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_CHAR_LIVING_SKILL_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_CHAR_LIVING_SKILL)
		{}
	};

	struct REP_LOG_CHAR_LIVING_SKILL_ELE
	{
		ADMIN::DATE_TIME	date_time;							// ʱ��
		ADMIN::LIVING_SKILL	learn_type;							// ��������
		unsigned char		skill_level;						// �������ȼ�
		unsigned int		cosume_meony;						// ���Ľ�Ǯ
		unsigned int		cosume_exp;							// ���ľ���
	};

	typedef ADMIN::VarLenStruct<REP_LOG_CHAR_LIVING_SKILL_HEADER, REP_LOG_CHAR_LIVING_SKILL_ELE, 20>
		REP_LOG_CHAR_LIVING_SKILL;

	//////////////////////////////////////////////////////////////////////////
	// ��ɫ������־
	struct REP_LOG_INSTANCE_ASSEASS_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_INSTANCE_ASSEASS_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_INSTANCE_ASSEASS)
		{}
	};

	struct REP_LOG_INSTANCE_ASSEASS_ELE
	{
		struct INSTANCE_MATE
		{
			ADMIN::QueryCharacterId		char_id;	// ��ɫID
			ADMIN::QueryCharacterName	char_name;	// ��ɫ����
		};

		ADMIN::DATE_TIME	enter_time;				// ����ʱ��
		ADMIN::DATE_TIME	leave_time;				// �뿪ʱ��
		unsigned int		cosume_time;			// ����ʱ��
		INSTANCE_MATE		instance_mate[3];		// ����������
	};

	typedef ADMIN::VarLenStruct<REP_LOG_INSTANCE_ASSEASS_HEADER, REP_LOG_INSTANCE_ASSEASS_ELE, 20>
		REP_LOG_INSTANCE_ASSEASS;

	//////////////////////////////////////////////////////////////////////////
	// ����ץȡ��¼
	struct REP_LOG_PET_CATCH_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_PET_CATCH_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_PET_CATCH)
		{}
	};

	struct REP_LOG_PET_CATCH_ELE
	{
		ADMIN::DATE_TIME	date_time;				// ʱ��
		unsigned int		pet_type_id;			// ��������ID
		char				pet_name[ADMIN::MAX_ITEM_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_PET_CATCH_HEADER, REP_LOG_PET_CATCH_ELE, 20>
		REP_LOG_PET_CATCH;

	//////////////////////////////////////////////////////////////////////////
	// ���ǽ���ȡ��
	struct REP_LOG_PET_GET_TRADE_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_PET_GET_TRADE_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_PET_GET_TRADE)
		{}
	};

	struct REP_LOG_PET_GET_TRADE_ELE
	{
		ADMIN::QueryCharacterId		src_char_id;	// ��Դ��ɫID
		ADMIN::QueryCharacterName	src_char_name;	// ��Դ��ɫ����
		ADMIN::DATE_TIME			date_time;		// ����ʱ��
		unsigned int				item_type_id;	// ��Ʒ����ID
													// ��Ʒ����
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64			item_unique_id;	// ��ƷΨһID
		unsigned char				item_count;		// ����
	};

	typedef ADMIN::VarLenStruct<REP_LOG_PET_GET_TRADE_HEADER, REP_LOG_PET_GET_TRADE_ELE, 20>
		REP_LOG_PET_GET_TRADE;

	//////////////////////////////////////////////////////////////////////////
	// ���ǽ���ʧȥ
	struct REP_LOG_PET_LOSE_TRADE_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����

		REP_LOG_PET_LOSE_TRADE_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_PET_LOSE_TRADE)
		{}
	};

	struct REP_LOG_PET_LOSE_TRADE_ELE
	{
		ADMIN::QueryCharacterId		dst_char_id;	// ��ƷĿ���ɫID
		ADMIN::QueryCharacterName	dst_char_name;	// ��ƷĿ���ɫ��
		ADMIN::DATE_TIME			date_time;		// ����ʱ��
		unsigned int				item_type_id;	// ��Ʒ����ID
													// ��Ʒ����
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64			item_unique_id;	// ��ƷΨһID
		unsigned char				item_count;		// ����
	};

	typedef ADMIN::VarLenStruct<REP_LOG_PET_LOSE_TRADE_HEADER, REP_LOG_PET_LOSE_TRADE_ELE, 20>
		REP_LOG_PET_LOSE_TRADE;

	//////////////////////////////////////////////////////////////////////////
	// ���ǰ�̯
	struct REP_LOG_PET_BOOTH_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;				// ��ɫID
		ADMIN::QueryCharacterName	char_name;				// ��ɫ����

		REP_LOG_PET_BOOTH_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_PET_BOOTH)
		{}
	};

	struct REP_LOG_PET_BOOTH_ELE
	{
		ADMIN::DATE_TIME			date_time;		// ����ʱ��
		ADMIN::QueryCharacterId		dst_char_id;	// Ŀ���ɫID
		ADMIN::QueryCharacterName	dst_char_name;	// Ŀ���ɫ��
		unsigned int				item_type_id;	// ��Ʒ����ID
													// ��Ʒ����
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned char				item_count;		// ����
		unsigned int				item_price;		// �۸�
	};

	typedef ADMIN::VarLenStruct<REP_LOG_PET_BOOTH_HEADER, REP_LOG_PET_BOOTH_ELE, 20>
		REP_LOG_PET_BOOTH;

	//////////////////////////////////////////////////////////////////////////
	// ���ᴴ����¼
	struct REP_LOG_GUILD_CREATE_HEADER : public REP_GUILD_LOG_BASE
	{
		REP_LOG_GUILD_CREATE_HEADER()
			: REP_GUILD_LOG_BASE(CMD_REP_LOG_GUILD_CREATE)
		{}
	};

	struct REP_LOG_GUILD_CREATE_ELE
	{
		ADMIN::GuildName			guild_name;		// ��������
		ADMIN::GuildId				guild_id;		// ����ID
		ADMIN::DATE_TIME			date_time;		// ʱ��
		ADMIN::QueryCharacterName	char_name;		// ��ɫ��
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
	};

	typedef ADMIN::VarLenStruct<REP_LOG_GUILD_CREATE_HEADER, REP_LOG_GUILD_CREATE_ELE, 20>
		REP_LOG_GUILD_CREATE;

	//////////////////////////////////////////////////////////////////////////
	// �����ɢ��¼
	struct REP_LOG_GUILD_DISMISS_HEADER : public REP_GUILD_LOG_BASE
	{
		REP_LOG_GUILD_DISMISS_HEADER()
			: REP_GUILD_LOG_BASE(CMD_REP_LOG_GUILD_DISMISS)
		{}
	};

	struct REP_LOG_GUILD_DISMISS_ELE
	{
		ADMIN::GuildName			guild_name;		// ��������
		ADMIN::GuildId				guild_id;		// ����ID
		ADMIN::DATE_TIME			date_time;		// ʱ��
		ADMIN::QueryCharacterName	char_name;		// ��ɫ��
		ADMIN::QueryCharacterId		char_id;		// ��ɫID
	};

	typedef ADMIN::VarLenStruct<REP_LOG_GUILD_DISMISS_HEADER, REP_LOG_GUILD_DISMISS_ELE, 20>
		REP_LOG_GUILD_DISMISS;

	struct REP_LOG_LOGIN_LOGOUT_BY_IP_HEADER : public ADMIN::TCP_MSG_BASE
	{
		unsigned int					srv_realm_id;		// ����id
		unsigned int					srv_cluster_id;		// ��������id
		bool							all_send;			// �Ƿ������
		ADMIN::DATE_TIME				start_time;			// ��ʼʱ��
		ADMIN::DATE_TIME				end_time;			// ����ʱ��
		unsigned int					clientIp;			// ��ѯʱ��IP

		REP_LOG_LOGIN_LOGOUT_BY_IP_HEADER()
			: ADMIN::TCP_MSG_BASE(CMD_REP_LOG_LOGIN_LOGOUT_BY_IP)
		{}
	};

	struct REP_LOG_LOGIN_LOGOUT_BY_IP_ELE
	{
		enum LOGIN_LOGOUT_OPEARTION
		{
			LOGIN_CHAR,								// ��ɫ��¼
			LOGOUT_CHAR,							// ��ɫ�ǳ�
		};

		ADMIN::QueryCharacterId		char_id;		// ��ɫID
		ADMIN::QueryCharacterName	char_name;		// ��ɫ����
		LOGIN_LOGOUT_OPEARTION		op_type;		// ����
		ADMIN::DATE_TIME			date_time;		// ʱ��ʱ��
	};

	typedef ADMIN::VarLenStruct<REP_LOG_LOGIN_LOGOUT_BY_IP_HEADER, REP_LOG_LOGIN_LOGOUT_BY_IP_ELE, 20>
		REP_LOG_LOGIN_LOGOUT_BY_IP;

	//
	//////////////////////////////////////////////////////////////////////////

}; // namespace MSG_ASTA


#pragma pack(pop)
