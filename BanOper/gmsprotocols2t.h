/******************************************************************************
						GM Tools��GM Serverͨ��Э��

GM Server��Ӧ��(S2T)
create by LiaoHuaifu 2008/9/2 

���ڹ���: 2008/10/18
*******************************************************************************/

#ifndef GMS_PROTOCOL_S2T_H
#define GMS_PROTOCOL_S2T_H
#pragma once
#pragma pack(push,1)

#include <string.h>

#include "gmsGlobalDefine.h"
//#include "..\SDBase\Match\MatchDef.h"

namespace GMSBase
{
	//
	// S2T �������ṹ
	//
	typedef struct _tagPacketBase_S2T
	{

		RES_TYPE	wHeader;					//	��Ϣͷ,

		_tagPacketBase_S2T()
		{
			wHeader     =  RE_INVALID;
		}

	}PacketBase_S2T,*PPacketBase_S2T;

	//
	//���ص�¼��Ϣ
	//
	typedef struct _tagRES_LOGIN_GMS : PacketBase_S2T
	{
		bool      bOk;				// �Ƿ�ɹ�
		ERROR_CODE nError;			// ʧ�ܴ���
		
		_tagRES_LOGIN_GMS()
		{
			wHeader = RE_LOGIN;
			bOk = true;
			nError = ERR_INVALID;
		}

	}RES_LOGIN_GMS,*PRES_LOGIN_GMS;

	//
	// ������һ�����Ϣ
	//
	typedef struct _tagRES_PLAYER_INFO : PacketBase_S2T
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];			  //  ��ɫ����
		bool    bGet;										  //  �Ƿ�ɹ���ȡ����

		union
		{
			// �ɹ���ȡ��Ϣ
			struct
			{
				char 	szLoginAccount[MAX_ACCOUNT_LENGTH];       	  //  �ʺ�����
				char 	szLoginPassword[MAX_PASSWORD_LENGTH];	      //  ����
				SQLSVR_DATETIME  tLockDate;                           //  �������
				char    szActiveCode[MAX_ACTIVE_CODE_LENGTH];	 	  //  ������
				SQLSVR_DATETIME  tActiveDate;					      //  ��������

				ui8     nCharacterNum;						          //  ��ǰ��ɫ����
				CHARACTER_BASIC_INFO_GMS szCharacters[MAX_CHARACTER_NUM];  //   ��ɫ��Ϣ

			}play_info;

			// ��ȡ��Ϣʧ��
			struct
			{
				ERROR_CODE nError;	// �������
			}fail_info;

		};

		ui32	GetLength(void)
		{
			const int nBasic_len = sizeof(PacketBase_S2T) + sizeof(szCharName) + sizeof(bGet);  

			if (bGet)
			{
				return  nBasic_len + sizeof(play_info);
			}
			else
			{
				return  nBasic_len + sizeof(fail_info);
			}
		}

		_tagRES_PLAYER_INFO()
		{
			wHeader = RE_PLAYER_INFO;
			memset(szCharName,0,sizeof(szCharName));
			bGet = true;
		}

	}RES_PLAYER_INFO,*PRES_PLAYER_INFO;

	//
	// ������ҽ�ɫ�Ļ�����Ϣ
	//
	typedef struct _tagRES_CHAR_INFO :  PacketBase_S2T
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];				//	��ɫ����

		bool    bGet;										  	//  �Ƿ�ɹ���ȡ����

		union
		{
			struct
			{
				CHARACTER_ABILITY_GMS ability;						//  ��������
				SEASON_STAT_GMS szSeasonStatInfo[HSNFT_COUNT];      	//  ������Ϣ

			}character_info;

			struct
			{
				ERROR_CODE nError;	// �������
			}fail_info;

		};

		ui32	GetLength(void)
		{
			const int nBasic_len = sizeof(PacketBase_S2T) + sizeof(szCharName) + sizeof(bGet); 

			if (bGet)
			{
				return  nBasic_len + sizeof(character_info);
			}
			else
			{
				return  nBasic_len + sizeof(fail_info);
			}
		}

		_tagRES_CHAR_INFO()
		{
			memset(szCharName,0,sizeof(szCharName));
			wHeader = RE_CHAR_INFO;
		}
	}RES_CHAR_INFO,*PRES_CHAR_INFO;

	//
	// ����ǿ����Ҷ��߽��
	//
	typedef struct _tagRES_KICK_PLAYER : PacketBase_S2T
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];		// ��ɫ����

		bool 	bKick;                                  // �Ƿ�ɹ�ʹָ����Ҷ���
		ERROR_CODE nError;								// �������

		_tagRES_KICK_PLAYER()
		{
			wHeader = RE_KICK;
			memset(szCharName,0,sizeof(szCharName));
			bKick  = true;
			nError = ERR_INVALID;
        }
	}RES_KICK_PLAYER,*PRES_KICK_PLAYER;

	//
	// ���ط����ҽ��
	//
	typedef struct _tagRES_BAN_PLAYER : PacketBase_S2T
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];		// ��ɫ����
		char 	szAccountName[MAX_ACCOUNT_LENGTH];      // �ʺ�����

		bool 	bBan;									// �Ƿ�ɹ�ʹָ����ұ����
		ERROR_CODE nError;								// �������

		_tagRES_BAN_PLAYER()
		{
			wHeader = RE_BAN;
			memset(szCharName,0,sizeof(szCharName));
			memset(szAccountName,0,sizeof(szAccountName));
			bBan   = true;
			nError = ERR_INVALID;
		}
	}RES_BAN_PLAYER,*PRES_BAN_PLAYER;

	//
	// ���ط���������
	//
	typedef struct _tagRES_POP_MSG : PacketBase_S2T
	{
		ui32       nIndex;                // ������Ϣ����		
		bool   	    bSend;                // �Ƿ�ɹ����͹���
		ERROR_CODE  nError;               // �������

		_tagRES_POP_MSG()
		{
			wHeader = RE_POP;
			bSend   = true;
			nError  = ERR_INVALID;
		}
	}RES_POP_MSG,*PRES_POP_MSG;

	//
	// ���ؽ����
	//
	typedef struct _tagRES_RELEASE_PLAYER : PacketBase_S2T
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];		// ��ɫ����
		char 	szAccountName[MAX_ACCOUNT_LENGTH];      // �ʺ�����

		bool 	bRelease;							   // �Ƿ�ɹ�ʹָ����ҽ��
		ERROR_CODE nError;								// �������

		_tagRES_RELEASE_PLAYER()
		{
			wHeader = RE_RELEASE;
			memset(szCharName,0,sizeof(szCharName));
			memset(szAccountName,0,sizeof(szAccountName));
			bRelease   = true;
			nError = ERR_INVALID;
		}
	}RES_RELEASE_PLAYER,*PRES_RELEASE_PLAYER;

	//
	// �޸ľ���ֵ����	
	//	Add 2008/9/26
	typedef struct _tagRES_MODIFY_EXP_RATE :  PacketBase_S2T
	{
		bool bModify;					// �޸ĳɹ�
		ERROR_CODE nError;				// �������

		_tagRES_MODIFY_EXP_RATE()
		{
			wHeader = RE_EXP_RATE;
			bModify = true;
			nError = ERR_INVALID;
		}
	}RES_MODIFY_EXP_RATE,*PRES_MODIFY_EXP_RATE;

	//
	// �޸ľ���ֵ����	
	//	Add 2008/9/26
	typedef struct _tagRES_MODIFY_SCORE_RATE :  PacketBase_S2T
	{
		bool bModify;					// �޸ĳɹ�
		ERROR_CODE nError;				// �������

		_tagRES_MODIFY_SCORE_RATE()
		{
			wHeader = RE_SCORE_RATE;
			bModify = true;
			nError = ERR_INVALID;
		}
	}RES_MODIFY_SCORE_RATE,*PRES_MODIFY_SCORE_RATE;

	/*********************************** ���ڹ���***********************************/

	//
	//	�ʺż����ѯ 
	//
	typedef struct _tagRES_ACCOUNT_ACTIVE_SEARCH:  PacketBase_S2T
	{
		// ���ز�ѯ������ɹ���true ����ȡdata_success ��ʧ�ܣ�false����ȡ data_fail 
		bool bSearch;	

		union
		{
			struct  
			{
				char szAccountName[MAX_ACCOUNT_LENGTH];       // �ʺ�����
				ui32 nActiveRegionID;						  // ��������ID
				SQLSVR_DATETIME  tActiveDate;				  // ����ʱ��
			}data_success;

			struct
			{
				ERROR_CODE nError;							  // �������
			}data_fail;
		};
		ERROR_CODE nError;									  // �������
		ui32	GetPakLength(void)
		{
			const int nBasic_len = sizeof(PacketBase_S2T) + sizeof(bSearch); 

			if (bSearch)
			{
				return  nBasic_len + sizeof(data_success);
			}
			else
			{
				return  nBasic_len + sizeof(data_fail);
			}
		}

		_tagRES_ACCOUNT_ACTIVE_SEARCH()
		{
			bSearch = false;
			memset(&data_success,0,sizeof(data_success));
			memset(&data_fail,0,sizeof(data_fail));
			wHeader = RE_ACCOUNT_ACTIVE_SEARCH;	
			nError = ERR_INVALID;
		}
	}RES_ACCOUNT_ACTIVE_SEARCH,*PRES_ACCOUNT_ACTIVE_SEARCH;

	//
	//	������Ʒ��ѯ 
	//
	typedef struct _tagRES_ITEM_SEARCH:  PacketBase_S2T
	{
		ui32 nPlayerRecordNum;							// ���η�����һ�����Ϣ������
		PLAYER_CHARACTER_BASE_INFO *pPlayerCharInfos;	// ��һ�����Ϣ
		
		ui32 nDataRecordNum;							// ���η�����Ʒ��������
		GOODS_INFO	*pData;								// ��Ʒ����


		ui32 nPacketTotal;								// ���ж��ٸ����Ѽ�¼���ݰ�	  	
		ui32 nPacketIndex;								// ���η������Ѽ�¼���ݰ������ 

		ERROR_CODE nError;								// �������

		ui16	GetPakLength()
		{
			int basc_len = sizeof(PacketBase_S2T) + sizeof(ERROR_CODE) + sizeof(int)*4;
			return basc_len + sizeof(PLAYER_CHARACTER_BASE_INFO)*nPlayerRecordNum  + sizeof(GOODS_INFO) *nDataRecordNum;
		}	 

		_tagRES_ITEM_SEARCH()
		{
			
			wHeader = RE_ITEM_SEARCH;
			nError = ERR_INVALID;
		}
	}RES_ITEM_SEARCH,*PRES_ITEM_SEARCH;

	//
	//	�������Ѽ�¼��ѯ 
	//
	typedef struct _tagRES_CONSUME_RECORD_SEARCH:  PacketBase_S2T
	{
		ui32 nPlayerRecordNum;							// ���η�����һ�����Ϣ������
		PLAYER_CHARACTER_BASE_INFO *pPlayerCharInfos;	// ��һ�����Ϣ
								

		ui32 nDataRecordNum;							// ���η������Ѽ�¼��������
		CONSUME_RECORD_INFO	*pData;						// ���Ѽ�¼����
								
		
		ui32 nPacketTotal;								// ���ж��ٸ����Ѽ�¼���ݰ�	  	
		ui32 nPacketIndex;								// ���η������Ѽ�¼���ݰ������ 	

		ERROR_CODE nError;								// �������

		ui16	GetPakLength()
		{
			int basc_len = sizeof(PacketBase_S2T) + sizeof(ERROR_CODE) + sizeof(int)*4;
			return basc_len + sizeof(PLAYER_CHARACTER_BASE_INFO)*nPlayerRecordNum  + sizeof(CONSUME_RECORD_INFO) *nDataRecordNum;
		}	 

		_tagRES_CONSUME_RECORD_SEARCH()
		{
			memset(this,0,sizeof(_tagRES_CONSUME_RECORD_SEARCH));
			wHeader = RE_CONSUME_RECORD_SEARCH;
			nError = ERR_INVALID;
		}
	}RES_CONSUME_RECORD_SEARCH,*PRES_CONSUME_RECORD_SEARCH;

	//
	//	�����ʺŵ�½��־
	//
	typedef struct _tagRES_ACCOUNT_LOGIN_LOGOUT_LOG:  PacketBase_S2T
	{
		ui32 nPlayerRecordNum;					// ���η�����һ�����Ϣ������
		PLAYER_CHARACTER_BASE_INFO *pPlayerCharInfos;	// ��һ�����Ϣ
		
		ui32 nDataRecordNum;					// ���η��͵�½��־��Ϣ������
		LOGIN_OUT_INFO	*pData;					// ��½��־ 
		

		ui32 nPacketTotal;						// ���ж��ٸ���½��־���ݰ�	  	
		ui32 nPacketIndex;						// ���η��͵�½��־���ݰ������ 

		ERROR_CODE nError;						// �������

		ui16	GetPakLength()
		{
			int basc_len = sizeof(PacketBase_S2T) + sizeof(ERROR_CODE) + sizeof(int)*4;
			return basc_len + sizeof(PLAYER_CHARACTER_BASE_INFO)*nPlayerRecordNum  + sizeof(LOGIN_OUT_INFO) *nDataRecordNum;
		}	 
		
		_tagRES_ACCOUNT_LOGIN_LOGOUT_LOG()
		{
			memset(this,0,sizeof(_tagRES_ACCOUNT_LOGIN_LOGOUT_LOG));
			wHeader = RE_ACCOUNT_LOGIN_LOGOUT_LOG;
			nError = ERR_INVALID;
		}
	}RES_ACCOUNT_LOGIN_LOGOUT_LOG,*PRES_ACCOUNT_LOGIN_LOGOUT_LOG;

	//
	//	���ؼ��ܲ�ѯ
	//  
	typedef struct _tagRES_SKILL_SEARCH:  PacketBase_S2T
	{
		ui32 nPlayerRecordNum;					// ���η�����һ�����Ϣ������
		PLAYER_CHARACTER_BASE_INFO *pPlayerCharInfos;	// ��һ�����Ϣ
		
		ui32 nDataRecordNum;					// ���η��ͼ������ݵ�����
		SKIN_RECORD_INFO	*pData;				// ��������
		

		ui32 nPacketTotal;						// ���ж��ٸ��������ݼ�¼���ݰ�	  	
		ui32 nPacketIndex;						// ���η��ͼ������ݼ�¼���ݰ������ 

		ERROR_CODE nError;						// �������

		ui16	GetPakLength()
		{
			int basc_len = sizeof(PacketBase_S2T) + sizeof(ERROR_CODE) + sizeof(int)*4;
			return basc_len + sizeof(PLAYER_CHARACTER_BASE_INFO)*nPlayerRecordNum  + sizeof(SKIN_RECORD_INFO) *nDataRecordNum;
		}	 

		_tagRES_SKILL_SEARCH()
		{
			memset(this,0,sizeof(_tagRES_SKILL_SEARCH));
			wHeader = RE_SKILL_SEARCH;
			nError = ERR_INVALID;
		}
	}RES_SKILL_SEARCH,*PRES_SKILL_SEARCH;	

	//
	//	���ؽ�ɫ�������޸ġ�ɾ�����ָ���
	//  
	typedef struct _tagRES_CHARACTER_INFO_OPERATION:  PacketBase_S2T
	{
		// ��������
		enum
		{
			RE_INVALID = 0,
			RE_MODIFY_NAME,
			RE_DELETE,
			RE_RENEW,
			OP_MODIFIY_GB,
			OP_MODIFY_LEVEL,
			RE_MODIFY_PSW,
			RE_RENEW_PSW,
		}RE_type;	

		ui32 nPlayerRecordNum;					// ���η�����һ�����Ϣ������
		PLAYER_CHARACTER_BASE_INFO *pPlayerCharInfos;	// ��һ�����Ϣ
		bool bSuccess;									// �����Ƿ�ɹ�

		ERROR_CODE nError;								// �������

		ui16	GetPakLength()
		{
			int basc_len = (ui32)&nError - (ui32)&wHeader; /*sizeof(PacketBase_S2T) + sizeof(ERROR_CODE) + sizeof(RE_type) + sizeof(bool)*/;
			return basc_len + sizeof(PLAYER_CHARACTER_BASE_INFO)*nPlayerRecordNum;
		}

		_tagRES_CHARACTER_INFO_OPERATION()
		{
			memset(this,0,sizeof(_tagRES_CHARACTER_INFO_OPERATION));
			wHeader = RE_CHARACTER_INFO_OPERATION;
			RE_type = RE_INVALID;
			nError = ERR_INVALID;
		}
	}RES_CHARACTER_INFO_OPERATION,*PRES_CHARACTER_INFO_OPERATION;

	/*********************************** ���ڹ��� ***********************************/

	//
	//	���غ�����Ϣ��ѯ
	//
	typedef struct _tagRES_FRIEND_SEARCH:  PacketBase_S2T
	{
		ui32 nDataRecordNum;					// ���η�����Һ�����Ϣ������
		FRIEND_INFO *pData;				// ������Ϣ

		ui32 nPacketTotal;						// ���ж��ٸ��������ݼ�¼���ݰ�	  	
		ui32 nPacketIndex;						// ���η��ͺ������ݼ�¼���ݰ������ 

		ERROR_CODE nError;						// �������

		ui16	GetPakLength()
		{
			int basc_len = sizeof(PacketBase_S2T) + sizeof(ERROR_CODE) + sizeof(int)*3;
			return basc_len + sizeof(FRIEND_INFO) * nDataRecordNum;
		}

		_tagRES_FRIEND_SEARCH()
		{
			memset(this, 0, sizeof(_tagRES_FRIEND_SEARCH));
			wHeader = RE_FRIEND_SEARCH;
			nError = ERR_INVALID;
		}

	}RES_FRIEND_SEARCH, *PRES_FRIEND_SEARCH;

	//
	// ���������־��Ϣ
	//
	typedef struct _tagRES_UPLEVE_ITEM_LOG_SEARCH:  PacketBase_S2T
	{

		// ��������
		enum
		{
			OP_INVALID = 0,
			OP_UPGRADE,    // ������¼
			OP_ITEM_USE,   // ����ʹ�ü�¼
			OP_ITEM_DELETE,// ����ɾ����¼
			OP_CHARECTORDELETE, // ��ɫɾ����¼
		}RE_type;	

		union
		{
			struct  
			{
				ui32 nDataRecordNum;			// ���η������������Ϣ������	
				UPLEVEL_LOG *pData;		// ���������Ϣ
			}data_uplevel_log;

			struct  
			{
				ui32   nDataRecordNum;			 //����ʹ�ü�¼����
				ITEM_USE_LOG *pData;			
			}data_itemuser_log;

			struct  
			{
				ui32 nDataRecordNum;			// ���η������ɾ��������Ϣ������	
				ITEM_DELETE_LOG *pData;
			}data_item_delete_log;

			struct  
			{
				ui32 nDataRecordNum;			// ���η������ɾ����ɫ������	
				CHARECTOR_DELETE_LOG *pData;
			}data_chractor_delete_log;
		};

		ui32 nPacketTotal;					// ���ж��ٸ��������ݼ�¼���ݰ�	  	
		ui32 nPacketIndex;					// ���η��ͼ������ݼ�¼���ݰ������ 

		ERROR_CODE nError;					// �������

		ui16	GetPakLength()
		{
			int basc_len = sizeof(PacketBase_S2T) + sizeof(ERROR_CODE) + sizeof(int)*3 + sizeof(RE_type);
			switch(RE_type)
			{
			case OP_UPGRADE:		return basc_len + sizeof(UPLEVEL_LOG) * data_uplevel_log.nDataRecordNum;
			case OP_ITEM_USE:		return basc_len + sizeof(ITEM_USE_LOG) * data_itemuser_log.nDataRecordNum;
			case OP_ITEM_DELETE:	return basc_len + sizeof(ITEM_DELETE_LOG) * data_item_delete_log.nDataRecordNum;
			case OP_CHARECTORDELETE:	return basc_len + sizeof(CHARECTOR_DELETE_LOG) * data_chractor_delete_log.nDataRecordNum;
			default:				return basc_len;
			}
		}

		_tagRES_UPLEVE_ITEM_LOG_SEARCH()
		{
			memset(this, 0, sizeof(_tagRES_UPLEVE_ITEM_LOG_SEARCH));
			wHeader = RE_LOG_SEARCH;
			nError = ERR_INVALID;
			RE_type = OP_INVALID;
		}

	}RES_UPLEVE_ITEM_LOG_SEARCH, *PRES_UPLEVE_ITEM_LOG_SEARCH;

	//
	// �����б�
	// 

	//
	// �������͵�����Ϣ
	//
	typedef struct _tagRES_PRASENT_ITEM:  PacketBase_S2T
	{
		bool bSuccess;						// �Ƿ����ͳɹ�
		ERROR_CODE nError;					// �������
		_tagRES_PRASENT_ITEM()
		{
			memset(this, 0, sizeof(_tagRES_PRASENT_ITEM));
			wHeader = RE_PRASENT_ITEM;
			nError = ERR_INVALID;
		}

	}RES_PRASENT_ITEM,*PRES_PRASENT_ITEM;


	// �
	typedef	struct _tagRES_ACTIVE_CAMPAIGN :
		public PacketBase_S2T
	{	
		struct  
		{
			ui8 nDetailID;		// �η���
			ui8	nMainID;		// �����࣬ͬʱ����˳��
		}Data[8];

		_tagRES_ACTIVE_CAMPAIGN()
		{
			memset(this,0,sizeof(_tagRES_ACTIVE_CAMPAIGN));
			wHeader = RE_ACTIVE_CAMPAIGN; 
		}
	}RES_ACTIVE_CAMPAIGN,*PRES_ACTIVE_CAMPAIGN;

}
#pragma pack(pop)
#endif
