#pragma once

#include "BackStageMgrDef.h"

//
// rs���͸���̨����ϵͳ����Ϣ�ṹ����
//

#pragma pack(push, 1)

namespace MSG_RSTBS
{
	const int CMD_REP_LOGIN					= BACKSTAGEMGR::MSG_BASE_RSTBS + 1;
	const int CMD_REP_CHARACTER_BASE_INFO	= BACKSTAGEMGR::MSG_BASE_RSTBS + 2;
	const int CMD_REP_CHARACTER_ITEMS_INFO	= BACKSTAGEMGR::MSG_BASE_RSTBS + 3;
	const int CMD_REP_CLOSURE_ACCOUNT_INFO	= BACKSTAGEMGR::MSG_BASE_RSTBS + 4;

	const int CMD_REP_KICK_USER				= BACKSTAGEMGR::MSG_BASE_RSTBS + 100;
	const int CMD_REP_CLOSE_ACCOUNT			= BACKSTAGEMGR::MSG_BASE_RSTBS + 101;
	const int CMD_REP_AVAILABLE_ACCOUNT		= BACKSTAGEMGR::MSG_BASE_RSTBS + 102;
	const int CMD_REP_SYSTEM_BROADCAST		= BACKSTAGEMGR::MSG_BASE_RSTBS + 103;

	// 
	// ���ص�½���
	// 
	struct REP_LOGIN : public BACKSTAGEMGR::TCP_MSG_BASE
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
	struct REP_CHARACTER_BASE_INFO : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		unsigned int	srv_realm_id;	// ����id
		unsigned int	srv_cluster_id;	// ��������id
		char			user_name[BACKSTAGEMGR::MAX_ID_LENGTH]; // �ʺ�
		unsigned int	char_id;		// ��ɫid
		char			char_name[BACKSTAGEMGR::MAX_CHARACTER_NAME_LENGTH];		// ��ɫ����
		char			char_title1[BACKSTAGEMGR::MAX_CHARACTER_NAME_LENGTH];	// ��ɫ��ν1
		char			char_title2[BACKSTAGEMGR::MAX_CHARACTER_NAME_LENGTH];	// ��ɫ��ν2
		unsigned char	char_type;			// ��ɫ����(ְҵ�Ա��)
		bool			is_online;			// �Ƿ�����
		bool			is_closed;			// �Ƿ��ͣ
		unsigned char	char_level;			// ��ɫ�ȼ�
		unsigned int	money;				// ��Ϸ������
		unsigned int	nineyou_money;		// 9�α�����
		unsigned int	reserve_money;		// ������
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
	// ���ؽ�ɫ��Ʒ��Ϣ
	// 
	struct REP_CHARACTER_ITEMS_INFO : public BACKSTAGEMGR::TCP_MSG_BASE
	{

		REP_CHARACTER_ITEMS_INFO()
			:TCP_MSG_BASE(CMD_REP_CHARACTER_ITEMS_INFO)
		{}
	};

	//
	// �����ʺŷ�ͣ��Ϣ
	//
	struct REP_CLOSURE_ACCOUNT_INFO : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		enum
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// ��ѯ��ͣ��Ϣ�ɹ�
			E_FAILED_ACCOUNT_NOTEXIST,				// �ʺŲ�����
			E_FAILED_SERVERINTERNALERROR,			// �������ڲ�����
		}eResult;

		// ������Ϣ����
		enum
		{
			R_UNKNOWNE = 0,
			R_USERNAME,			// �ʺ�����
			R_CHARACTER_NAME,	// ��ɫ����
		}rType;

		union
		{
			struct  
			{
				char userName[BACKSTAGEMGR::MAX_ID_LENGTH];
			}user_name; // �û��ʺ�

			struct  
			{
				char charName[BACKSTAGEMGR::MAX_CHARACTER_NAME_LENGTH];
			}char_name; // ��ɫ���� 
		};

		// �ɹ��Ļ�
		unsigned int	closure_reason;	// ��ͣԭ��
		unsigned int	duration_time;	// ��ͣʣ��ʱ��(����)

		REP_CLOSURE_ACCOUNT_INFO()
			:TCP_MSG_BASE(CMD_REP_CLOSURE_ACCOUNT_INFO),eResult(E_FAILED_UNKNOWNERROR)
		{}
	};

	//
	// �������˽��
	//
	struct REP_KICK_USER : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		enum
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// ���˳ɹ�
			E_FAILED_CHAR_NOTINGAME,				// �ʺ�û�н�ɫ����Ϸ��
			E_FAILED_SERVERINTERNALERROR,			// �������ڲ�����
		}eResult;
		// ������Ϣ����
		enum
		{
			R_UNKNOWNE = 0,
			R_USERNAME,			// �ʺ�����
			R_CHARACTER_NAME,	// ��ɫ����
		}rType;

		union
		{
			struct  
			{
				char userName[BACKSTAGEMGR::MAX_ID_LENGTH];
			}user_name; // �û��ʺ�

			struct  
			{
				char charName[BACKSTAGEMGR::MAX_CHARACTER_NAME_LENGTH];
			}char_name; // ��ɫ���� 
		};

		REP_KICK_USER()
			:TCP_MSG_BASE(CMD_REP_KICK_USER)
		{}
	};

	//
	// ��ͣ�ʺŽ��
	//
	struct REP_CLOSE_ACCOUNT : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		enum
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// ��ͣ�ɹ�
			E_FAILED_ACCOUNT_NOTEXIST,				// �ʺŲ�����
			E_FAILED_SERVERINTERNALERROR,			// �������ڲ�����
		}eResult;

		// ������Ϣ����
		enum
		{
			R_UNKNOWNE = 0,
			R_USERNAME,			// �ʺ�����
			R_CHARACTER_NAME,	// ��ɫ����
		}rType;

		union
		{
			struct  
			{
				char userName[BACKSTAGEMGR::MAX_ID_LENGTH];
			}user_name; // �û��ʺ�

			struct  
			{
				char charName[BACKSTAGEMGR::MAX_CHARACTER_NAME_LENGTH];
			}char_name; // ��ɫ���� 
		};

		// �ɹ��Ļ�
		unsigned int duration_time;	// ��ͣʣ��ʱ��(����)

		REP_CLOSE_ACCOUNT()
			:TCP_MSG_BASE(CMD_REP_CLOSE_ACCOUNT)
		{}
	};

	//
	// ����ʺŽ��
	//
	struct REP_AVAILABLE_ACCOUNT : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		enum
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// ���ɹ�
			E_FAILED_ACCOUNT_NOTEXIST,				// �ʺŲ�����
			E_FAILED_SERVERINTERNALERROR,			// �������ڲ�����
		}eResult;

		char userName[BACKSTAGEMGR::MAX_ID_LENGTH]; // �ʺ�
		
		REP_AVAILABLE_ACCOUNT()
			:TCP_MSG_BASE(CMD_REP_AVAILABLE_ACCOUNT)
		{
			userName[0] = '\0';
		}
	};

	//
	// ϵͳ�㲥��Ϣ���
	// 
	struct REP_SYSTEM_BROADCAST : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		enum
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// �㲥�ɹ�
			E_FAILED_SERVERINTERNALERROR,			// �������ڲ�����
		}eResult;

		REP_SYSTEM_BROADCAST()
			:TCP_MSG_BASE(CMD_REP_SYSTEM_BROADCAST)
		{}
	};

}; // namespace MSG_WSTBS

#pragma pack(pop)