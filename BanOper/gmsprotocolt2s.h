/******************************************************************************
						GM Tools��GM Serverͨ��Э��

GM Tools������(T2S)
create by LiaoHuaifu 2008/9/2

���ڹ���: 2008/10/18
*******************************************************************************/

#ifndef GMS_PROTOCOL_T2S_H
#define GMS_PROTOCOL_T2S_H
#pragma once
#pragma pack(push,1)

#include <string.h>

#include "gmsGlobalDefine.h"
//#include "..\SDBase\TypeDef.h"
//------------------------------------------------------------------------------
namespace GMSBase
{
	//
	// T2S �������ṹ
	//
	typedef struct _tagPacketBase_T2S
	{

		CMD_TYPE	wHeader;						 //	��Ϣͷ,

		_tagPacketBase_T2S()
		{
			wHeader   = OP_INVALID;
        }

	}PacketBase_T2S,*PPacketBase_T2S;

	/*********************************** һ�ڹ���***********************************/

	//
	//GMTools �����¼GMS,GMS����GmsID��szPassword��szAccountName����֤GMToolsʹ���ߵ����
	//
	typedef struct _tagREQ_LOGIN_GMS : PacketBase_T2S
	{
		char	szPassword[MAX_PASSWORD_LENGTH];
		char	szAccountName[MAX_ACCOUNT_LENGTH];
		//char	szIP[MAX_ACCOUNT_LENGTH];
		ui32	nDstGMSID;					     //	Ŀ��GM Server��ID

		_tagREQ_LOGIN_GMS()
		{
			memset(szPassword,0,sizeof(szPassword));
			memset(szAccountName,0,sizeof(szAccountName));
			//memset(szIP,0,sizeof(szIP));
			wHeader = OP_LOGIN;
			nDstGMSID = 0;
		}

	}REQ_LOGIN_GMS,*PREQ_LOGIN_GMS;

	//
	// �鿴��һ�����Ϣ
	//
	typedef struct _tagREQ_PLAYER_INFO : PacketBase_T2S
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];		 //	��ɫ����
		char 	szAccountName[MAX_ACCOUNT_LENGTH];       //  �ʺ�����

		_tagREQ_PLAYER_INFO()
		{
			memset(szCharName,0,sizeof(szCharName));
			memset(szAccountName,0,sizeof(szAccountName));
			wHeader = OP_PLAYER_INFO;
		}

	}REQ_PLAYER_INFO,*PREQ_PLAYER_INFO;

	//
	// �鿴��ҽ�ɫ�Ļ�����Ϣ
	//
	typedef struct _tagREQ_CHAR_INFO : PacketBase_T2S
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];		//	��ɫ����

		_tagREQ_CHAR_INFO()
		{
			memset(szCharName,0,sizeof(szCharName));
			wHeader = OP_CHAR_INFO;
        }
	}REQ_CHAR_INFO,*PREQ_CHAR_INFO;

	//
	// ǿ����Ҷ���
	//
	typedef struct _tagREQ_KICK_PLAYER : PacketBase_T2S
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];		//	��ɫ����

		_tagREQ_KICK_PLAYER()
		{
			memset(szCharName,0,sizeof(szCharName));
			wHeader = OP_KICK;
        }
	}REQ_KICK_PLAYER,*PREQ_KICK_PLAYER;

	//
	// ������
	//
	typedef struct _tagREQ_BAN_PLAYER : PacketBase_T2S
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];		 // ��ɫ����
		char 	szAccountName[MAX_ACCOUNT_LENGTH];      //  �ʺ�����
		i8		nDay;									 //  �������  ˵������������ķ�ΧΪ0��30��0��ʾ���÷��

		_tagREQ_BAN_PLAYER()
		{
			memset(szCharName,0,sizeof(szCharName));
			memset(szAccountName,0,sizeof(szAccountName));
			wHeader = OP_BAN;
			nDay = 31;
		}
	}REQ_BAN_PLAYER,*PREQ_BAN_PLAYER;

	//
	// ��������
	//
	typedef struct _tagREQ_POP_MSG : PacketBase_T2S
	{
		ui32 nIndex;                      // ��Ϣ����
		enum
		{
			NORMAL,							// ��ͨ����
			ROLL,         					// ��������
		}op_type;

		i8 nDelayTime;						// �ӳ�ʱ�䣨��λ������); 0 Ϊ��ʱ

		char popMsg[MAX_POP_MSG_LENGTH];   	// ��������
		
		_tagREQ_POP_MSG()
		{
			wHeader     = OP_POP; 
			nDelayTime  = 0;
			op_type     = NORMAL;
			nIndex      = 0;
			memset(popMsg,0,sizeof(popMsg));
		}
	}REQ_POP_MSG,*PREQ_POP_MSG;


	//
	// ������
	// 
	typedef struct _tagREQ_RELEASE_PLAYER : PacketBase_T2S
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];		 // ��ɫ����
		char 	szAccountName[MAX_ACCOUNT_LENGTH];      //  �ʺ�����

		_tagREQ_RELEASE_PLAYER()
		{
			memset(szCharName,0,sizeof(szCharName));
			memset(szAccountName,0,sizeof(szAccountName));
			wHeader = OP_RELEASE;
		}
	}REQ_RELEASE_PLAYER,*PREQ_RELEASEPLAYER;

	//
	// �޸ľ���ֵ����	
	//	Add 2008/9/26
	typedef struct _tagREQ_MODIFY_EXP_RATE :  PacketBase_T2S
	{
		real expRate;	// ����ֵ����
		int	 nDoubleExpBeginTime; // �趨���鿪ʼʱ��
		int	 nDoubleExpEndTime;	 //  �趨�������ʱ��
		_tagREQ_MODIFY_EXP_RATE()
		{
			wHeader = OP_EXP_RATE;
			expRate = 0;
		}
	}REQ_MODIFY_EXP_RATE,*PREQ_MODIFY_EXP_RATE;

	//
	// �޸Ļ���ֵ����	
	//	Add 2009/6/23
	typedef struct _tagREQ_MODIFY_SCORE_RATE :  PacketBase_T2S
	{
		real scoreRate;	// ���ֱ���
		int	 nDoubleScoreBeginTime; // �趨���ֿ�ʼʱ��
		int	 nDoubleScoreEndTime;	 //  �趨���ֽ���ʱ��
		_tagREQ_MODIFY_SCORE_RATE()
		{
			wHeader = OP_SCORE_RATE;
			scoreRate = 0;
		}
	}REQ_MODIFY_SCORE_RATE,*PREQ_MODIFY_SCORE_RATE;


	/*********************************** ���ڹ���***********************************/

	//
	//	�ʺż����ѯ 
	//
	typedef struct _tagREQ_ACCOUNT_ACTIVE_SEARCH:  PacketBase_T2S
	{
		PLAYER_UNIQUE_INFO	playerInfo;	

		_tagREQ_ACCOUNT_ACTIVE_SEARCH()
		{
			wHeader = OP_ACCOUNT_ACTIVE_SEARCH;			
		}
	}REQ_ACCOUNT_ACTIVE_SEARCH,*PREQ_ACCOUNT_ACTIVE_SEARCH;

	//
	//	��Ʒ��ѯ 
	//
	typedef struct _tagREQ_ITEM_SEARCH:  PacketBase_T2S
	{
		//�������ȼ�Ϊ����ɫ���ơ��˺����ƣ�������ɫ����Ϊ�գ����񰴽�ɫ����Ϊ��������
		PLAYER_UNIQUE_INFO	playerInfo;	

		_tagREQ_ITEM_SEARCH()
		{
			wHeader = OP_ITEM_SEARCH;
		}
	}REQ_ITEM_SEARCH,*PREQ_ITEM_SEARCH;

	//
	//	���Ѽ�¼��ѯ 
	//
	typedef struct _tagREQ_CONSUME_RECORD_SEARCH:  PacketBase_T2S
	{
		//�������ȼ�Ϊ����ɫ���ơ��˺����ƣ�������ɫ����Ϊ�գ����񰴽�ɫ����Ϊ��������
		PLAYER_UNIQUE_INFO	playerInfo;	

		char	szCharName_Receive[MAX_CHAR_NAME_LENGTH];		 // �����߽�ɫ����
		char 	szAccountName_Receive[MAX_ACCOUNT_LENGTH];       // �������ʺ�����
		//SQLSVR_DATETIME beginTime;								// ��ѯ��ʼʱ��
		//SQLSVR_DATETIME endTime;									// ��ѯ����ʱ��

		_tagREQ_CONSUME_RECORD_SEARCH()
		{
			wHeader = OP_CONSUME_RECORD_SEARCH;
		}
	}REQ_CONSUME_RECORD_SEARCH,*PREQ_CONSUME_RECORD_SEARCH;

	//
	//	�ʺŵ�½��־
	//
	typedef struct _tagREQ_ACCOUNT_LOGIN_LOGOUT_LOG:  PacketBase_T2S
	{
		//�������ȼ�Ϊ����ɫ���ơ��˺����ƣ�������ɫ����Ϊ�գ����񰴽�ɫ����Ϊ��������
		PLAYER_UNIQUE_INFO	playerInfo;	
		//SQLSVR_DATETIME beginTime;								// ��ѯ��ʼʱ��
		//SQLSVR_DATETIME endTime;									// ��ѯ����ʱ��

		_tagREQ_ACCOUNT_LOGIN_LOGOUT_LOG()
		{
			wHeader = OP_ACCOUNT_LOGIN_LOGOUT_LOG;
		}
	}REQ_ACCOUNT_LOGIN_LOGOUT_LOG,*PREQ_ACCOUNT_LOGIN_LOGOUT_LOG;

	//
	//	���ܲ�ѯ
	// 
	typedef struct _tagREQ_SKILL_SEARCH:  PacketBase_T2S
	{
		//�������ȼ�Ϊ����ɫ���ơ��˺����ƣ�������ɫ����Ϊ�գ����񰴽�ɫ����Ϊ��������
		PLAYER_UNIQUE_INFO	playerInfo;	

		_tagREQ_SKILL_SEARCH()
		{
			wHeader = OP_SKILL_SEARCH;
		}
	}REQ_SKILL_SEARCH,*PREQ_SKILL_SEARCH;	

	//
	//	��ɫ�������޸ġ�ɾ�����ָ���
	//  ע�⣺playerInfo�Ľ�ɫ����Ϊ�գ������������֪���޸�/ɾ��/�ָ������ǽ�ɫ����Ϣ
	typedef struct _tagREQ_CHARACTER_INFO_OPERATION:  PacketBase_T2S
	{
		// ��������
		enum
		{
			OP_INVALID = 0,
			OP_MODIFY_NAME,
			OP_DELETE,
			OP_RENEW,
			OP_MODIFIY_GB,
			OP_MODIFY_LEVEL,
			OP_MODIFY_PWS,
			OP_RENEW_PWS,
		}op_type;		

		//�������ȼ�Ϊ����ɫ���ơ��˺����ƣ�������ɫ����Ϊ�գ����񰴽�ɫ����Ϊ��������
		PLAYER_UNIQUE_INFO	playerInfo;		

		union
		{
			struct  
			{
				char szNewCharName[MAX_CHAR_NAME_LENGTH];		 // �½�ɫ����
			}data_modify;

			struct  
			{
				ui32 nReserve;									
			}data_delete;										// ɾ����ɫ

			struct  
			{
				ui32 nReserve;									
			}data_renew;										// �ָ���ɫ

			struct  
			{
				i32 newgb;
			}data_gb;											// �޸Ľ��

			struct  
			{
				ui8 nNewLevel;
			}data_modify_level;									// �޸ĵȼ�

			struct  
			{
				char newPws[32];								// �޸�����
			}date_modify_pws;

		};
		
		ui32	GetPakLength()
		{
			const int basic_len =sizeof(PacketBase_T2S) + sizeof(op_type) + sizeof(playerInfo);
			
			switch(op_type)
			{
			case OP_MODIFY_NAME: return basic_len + sizeof(data_modify);
			case OP_DELETE:		 return basic_len + sizeof(data_delete);
			case OP_RENEW:		 return basic_len + sizeof(data_renew);
			case OP_MODIFIY_GB:	 return basic_len + sizeof(data_gb);
			case OP_MODIFY_LEVEL:return basic_len + sizeof(data_modify_level);
			case OP_MODIFY_PWS:	 return basic_len + sizeof(date_modify_pws);
			case OP_RENEW_PWS:	 return basic_len;
			default: return basic_len;
			}
		}

		_tagREQ_CHARACTER_INFO_OPERATION()
		{
			op_type = OP_INVALID;
			wHeader = OP_CHARACTER_INFO_OPERATION;
		}
	}REQ_CHARACTER_INFO_OPERATION,*PREQ_CHARACTER_INFO_OPERATION;


	/*********************************** ���ڹ��� ***********************************/


	//
	//	������Ϣ��ѯ 
	//
	typedef struct _tagREQ_FRIEND_SEARCH:  PacketBase_T2S
	{
		//�������ȼ�Ϊ����ɫ���ơ��˺����ƣ�������ɫ����Ϊ�գ����񰴽�ɫ����Ϊ��������
		PLAYER_UNIQUE_INFO	playerInfo;	

		_tagREQ_FRIEND_SEARCH()
		{
			wHeader = OP_FRIEND_SEARCH;
		}
	}REQ_FRIEND_SEARCH,*PREQ_FRIEND_SEARCH;


	//
	//	�����־��Ϣ 
	//
	typedef struct _tagREQ_LOG_SEARCH:  PacketBase_T2S
	{
		// ��������
		enum
		{
			OP_INVALID = 0,
			OP_UPGRADE,    // ������¼
			OP_ITEM_USE,   // ����ʹ�ü�¼
			OP_ITEM_DELETE,// ����ɾ����¼
			OP_CHARECTORDELETE, // ��ɫɾ����¼
		}op_type;	

		//�������ȼ�Ϊ����ɫ���ơ��˺����ƣ�������ɫ����Ϊ�գ����񰴽�ɫ����Ϊ��������
		PLAYER_UNIQUE_INFO	playerInfo;	
		//SQLSVR_DATETIME beginTime;								// ��ѯ��ʼʱ��
		//SQLSVR_DATETIME endTime;									// ��ѯ����ʱ��
		_tagREQ_LOG_SEARCH()
		{
			op_type = OP_INVALID;
			wHeader = OP_LOG_SEARCH;
		}
	}REQ_LOG_SEARCH,*PREQ_LOG_SEARCH;


	//
	// ��������
	//
	typedef struct _tagREQ_ITEM_PRASENT:  PacketBase_T2S
	{
		// ��ɫ������Ϊ��
		PLAYER_UNIQUE_INFO playerInfo;
		ITEM_OBJ prasendtItem;
		char		mail_text[201];			//	�ʼ�����

		_tagREQ_ITEM_PRASENT()
		{
			memset(this, 0, sizeof(_tagREQ_ITEM_PRASENT));
			wHeader = OP_PRASENT_ITEM;
		}
	}REQ_PRASENT_ITEM,*PREQ_PRASENT_ITEM;

	// �
	typedef	struct _tagREQ_ACTIVE_CAMPAIGN :
	public PacketBase_T2S
	{	
		struct  
		{
			ui8 nDetailID;		// �η���
			ui8	nMainID;		// �����࣬ͬʱ����˳��
		}Data[8];

		_tagREQ_ACTIVE_CAMPAIGN()
		{
			memset(this,0,sizeof(_tagREQ_ACTIVE_CAMPAIGN));
			wHeader = OP_ACTIVE_CAMPAIGN; 
		}
	}REQ_ACTIVE_CAMPAIGN,*PREQ_ACTIVE_CAMPAIGN;

}
//------------------------------------------------------------------------------
#pragma pack(pop)
#endif