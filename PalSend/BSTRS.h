#pragma once

#include "BackStageMgrDef.h"
//
// ��̨����ϵͳ���͸�rs����Ϣ�ṹ����
//
#pragma pack(push, 1)

namespace MSG_BSTRS
{
	// ��ѯ��
	const int CMD_REQ_LOGIN					= BACKSTAGEMGR::MSG_BASE_BSTRS + 1;
	const int CMD_REQ_CHARACTER_INFO		= BACKSTAGEMGR::MSG_BASE_BSTRS + 2;
	const int CMD_REQ_CLOSURE_ACCOUNT_INFO	= BACKSTAGEMGR::MSG_BASE_BSTRS + 3;

	// ������
	const int CMD_REQ_KICK_USER				= BACKSTAGEMGR::MSG_BASE_BSTRS + 100;
	const int CMD_REQ_CLOSE_ACCOUNT			= BACKSTAGEMGR::MSG_BASE_BSTRS + 101;
	const int CMD_REQ_AVAILABLE_ACCOUNT		= BACKSTAGEMGR::MSG_BASE_BSTRS + 102;
	const int CMD_REQ_SYSTEM_BROADCAST		= BACKSTAGEMGR::MSG_BASE_BSTRS + 103;

	//
	// ��¼��rs
	// 
	struct REQ_LOGIN : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		char	loginUserName[BACKSTAGEMGR::MAX_ID_LENGTH];		// ��¼�û���
		char	loginPassWord[BACKSTAGEMGR::MAX_PASSWORD_LENGTH]; // ��¼����

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
	struct REQ_CHARACTER_INFO : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		// ��ѯ����
		enum REQ_TYPE
		{
			RT_INVALID			= 0,
			RT_CHAR_BASE_INFO	= 1,	// ��ɫ������Ϣ
			RT_CHAR_ITEM_INFO	= 2,	// ��ɫ��Ʒ��Ϣ
		};
		
		// ��ѯ����
		enum QUERY_CONDITION
		{
			QC_INVALID = 0,
			QC_USERNAME,	// ͨ���ʺ�
			QC_CHARNAME,	// ��ɫ����
		};
	
		REQ_TYPE		req_type;		
		QUERY_CONDITION	query_condition;

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
		
		REQ_CHARACTER_INFO()
			:TCP_MSG_BASE(CMD_REQ_CHARACTER_INFO),req_type(RT_INVALID),
			query_condition(QC_INVALID)
		{}
	};

	//
	// ��ѯ�ʺŷ�ͣ��Ϣ
	//
	struct REQ_CLOSURE_ACCOUNT_INFO : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		// ��ѯ��Χ
		enum REQ_RANGE
		{
			RR_INVALID = 0,
			RR_PER_CLOSUREACCOUNT,	// ������ͣ�ʺ�	
			RR_ALL_CLOSUREACCOUNT,	// ���з�ͣ�ʺ�
		}reqRange;

		char		userName[BACKSTAGEMGR::MAX_ID_LENGTH]; // ��ѯ������ͣ�ʺ�ʱ�õ�

		REQ_CLOSURE_ACCOUNT_INFO()
			:TCP_MSG_BASE(CMD_REQ_CLOSURE_ACCOUNT_INFO),reqRange(RR_INVALID)
		{
			userName[0] = '\0';
		}
	};


	//
	// ��������
	// 
	struct REQ_KICK_USER : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		enum
		{
			C_INVALID = 0,
			C_USERNAME,			// ͨ���ʺ���
			C_CHARACTERNAME,	// ͨ����ɫ��
		}kickCondition;

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

		REQ_KICK_USER()
			:TCP_MSG_BASE(CMD_REQ_KICK_USER),kickCondition(C_INVALID)
		{}
	};

	//
	// ��ͣ�ʺ�
	//
	struct REQ_CLOSE_ACCOUNT : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		// ��ͣԭ��
		enum
		{
			R_UNKNOWN	= 0,
			R_SYSTEM,			// ϵͳ��ͣ
			R_SYSTEM_TEMP,		// ϵͳ��ʱ��ͣ
			R_USERCUSTOM,		// �û�����
			R_USERCUSTOM_TEMP,	// �û���ʱ����
		}eResult;

		enum
		{
			C_INVALID = 0,
			C_USERNAME,			// ͨ���ʺ���
			C_CHARACTERNAME,	// ͨ����ɫ��
		}closeCondition;

		unsigned int duration_time;	// ��ͣʱ��(�����Ӽ���)

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

		REQ_CLOSE_ACCOUNT()
			:TCP_MSG_BASE(CMD_REQ_CLOSE_ACCOUNT),closeCondition(C_INVALID)
		{}
	};

	//
	// ����ʺ�
	//
	struct REQ_AVAILABLE_ACCOUNT : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		enum
		{
			C_INVALID = 0,
			C_USERNAME,			// ͨ���ʺ���
			C_CHARACTERNAME,	// ͨ����ɫ��
		}availableCondition;

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

		REQ_AVAILABLE_ACCOUNT()
			:TCP_MSG_BASE(CMD_REQ_CLOSE_ACCOUNT),availableCondition(C_INVALID)
		{}
	};

	//
	// ϵͳ�㲥(������ݰ�,�������ݰ�ʱ ���ݰ����ȵ���GetSize()���)
	//
	struct REQ_SYSTEM_BROADCAST : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		// �㲥����
		enum
		{
			T_INVALID = 0,
			T_NOW,			// ���Ϲ㲥
			T_DELAY,		// �ӳٹ㲥
		}bType;
		
		unsigned short	broadcastSize;	// �㲥��Ϣ����
		// ��ʼ�㲥ʱ��(�ӳٹ㲥�õ�)
		unsigned short	start_month;	
		unsigned short	start_day;
		unsigned short	start_hour;
		unsigned short	start_minute;
		// �����㲥ʱ��(�ӳٹ㲥�õ�)
		unsigned short	finish_month;	
		unsigned short	finish_day;
		unsigned short	finish_hour;
		unsigned short	finish_minute;
		unsigned short	interval;	// �㲥���(����)(�ӳٹ㲥�õ�)

		char	broadcastTxt[0xff]; // �㲥����

		REQ_SYSTEM_BROADCAST()
			:TCP_MSG_BASE(CMD_REQ_SYSTEM_BROADCAST)
		{
			broadcastSize = (unsigned short)(sizeof(REQ_SYSTEM_BROADCAST) - 0xff);
		}

		// ��ӹ㲥��Ϣ����
		bool AddBroadcaseTxt(const char* pTxt)
		{
			unsigned short len = (unsigned short)strlen(pTxt);
			if(len >= 0xff -1)
				return false;

			strcpy(broadcastTxt, pTxt);
			broadcastSize = (unsigned short)(sizeof(REQ_SYSTEM_BROADCAST) - 0xff + len + 1);
			return true;
		}

		unsigned int GetSize()
		{
			return broadcastSize;
		}
	};

}; // namespace MSG_BSTWS

#pragma pack(pop)