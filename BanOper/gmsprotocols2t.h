/******************************************************************************
						GM Tools与GM Server通信协议

GM Server响应类(S2T)
create by LiaoHuaifu 2008/9/2 

二期功能: 2008/10/18
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
	// S2T 基本包结构
	//
	typedef struct _tagPacketBase_S2T
	{

		RES_TYPE	wHeader;					//	消息头,

		_tagPacketBase_S2T()
		{
			wHeader     =  RE_INVALID;
		}

	}PacketBase_S2T,*PPacketBase_S2T;

	//
	//返回登录信息
	//
	typedef struct _tagRES_LOGIN_GMS : PacketBase_S2T
	{
		bool      bOk;				// 是否成功
		ERROR_CODE nError;			// 失败代码
		
		_tagRES_LOGIN_GMS()
		{
			wHeader = RE_LOGIN;
			bOk = true;
			nError = ERR_INVALID;
		}

	}RES_LOGIN_GMS,*PRES_LOGIN_GMS;

	//
	// 返回玩家基本信息
	//
	typedef struct _tagRES_PLAYER_INFO : PacketBase_S2T
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];			  //  角色名称
		bool    bGet;										  //  是否成功获取数据

		union
		{
			// 成功获取信息
			struct
			{
				char 	szLoginAccount[MAX_ACCOUNT_LENGTH];       	  //  帐号名称
				char 	szLoginPassword[MAX_PASSWORD_LENGTH];	      //  密码
				SQLSVR_DATETIME  tLockDate;                           //  解封日期
				char    szActiveCode[MAX_ACTIVE_CODE_LENGTH];	 	  //  激活码
				SQLSVR_DATETIME  tActiveDate;					      //  激活日期

				ui8     nCharacterNum;						          //  当前角色数量
				CHARACTER_BASIC_INFO_GMS szCharacters[MAX_CHARACTER_NUM];  //   角色信息

			}play_info;

			// 获取信息失败
			struct
			{
				ERROR_CODE nError;	// 错误代号
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
	// 返回玩家角色的基本信息
	//
	typedef struct _tagRES_CHAR_INFO :  PacketBase_S2T
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];				//	角色名称

		bool    bGet;										  	//  是否成功获取数据

		union
		{
			struct
			{
				CHARACTER_ABILITY_GMS ability;						//  能力属性
				SEASON_STAT_GMS szSeasonStatInfo[HSNFT_COUNT];      	//  赛季信息

			}character_info;

			struct
			{
				ERROR_CODE nError;	// 错误代号
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
	// 返回强制玩家断线结果
	//
	typedef struct _tagRES_KICK_PLAYER : PacketBase_S2T
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];		// 角色名称

		bool 	bKick;                                  // 是否成功使指定玩家断线
		ERROR_CODE nError;								// 错误代号

		_tagRES_KICK_PLAYER()
		{
			wHeader = RE_KICK;
			memset(szCharName,0,sizeof(szCharName));
			bKick  = true;
			nError = ERR_INVALID;
        }
	}RES_KICK_PLAYER,*PRES_KICK_PLAYER;

	//
	// 返回封禁玩家结果
	//
	typedef struct _tagRES_BAN_PLAYER : PacketBase_S2T
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];		// 角色名称
		char 	szAccountName[MAX_ACCOUNT_LENGTH];      // 帐号名称

		bool 	bBan;									// 是否成功使指定玩家被封禁
		ERROR_CODE nError;								// 错误代号

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
	// 返回发布公告结果
	//
	typedef struct _tagRES_POP_MSG : PacketBase_S2T
	{
		ui32       nIndex;                // 公告消息索引		
		bool   	    bSend;                // 是否成功发送公告
		ERROR_CODE  nError;               // 错误代号

		_tagRES_POP_MSG()
		{
			wHeader = RE_POP;
			bSend   = true;
			nError  = ERR_INVALID;
		}
	}RES_POP_MSG,*PRES_POP_MSG;

	//
	// 返回解封结果
	//
	typedef struct _tagRES_RELEASE_PLAYER : PacketBase_S2T
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];		// 角色名称
		char 	szAccountName[MAX_ACCOUNT_LENGTH];      // 帐号名称

		bool 	bRelease;							   // 是否成功使指定玩家解封
		ERROR_CODE nError;								// 错误代号

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
	// 修改经验值倍数	
	//	Add 2008/9/26
	typedef struct _tagRES_MODIFY_EXP_RATE :  PacketBase_S2T
	{
		bool bModify;					// 修改成功
		ERROR_CODE nError;				// 错误代号

		_tagRES_MODIFY_EXP_RATE()
		{
			wHeader = RE_EXP_RATE;
			bModify = true;
			nError = ERR_INVALID;
		}
	}RES_MODIFY_EXP_RATE,*PRES_MODIFY_EXP_RATE;

	//
	// 修改经验值倍数	
	//	Add 2008/9/26
	typedef struct _tagRES_MODIFY_SCORE_RATE :  PacketBase_S2T
	{
		bool bModify;					// 修改成功
		ERROR_CODE nError;				// 错误代号

		_tagRES_MODIFY_SCORE_RATE()
		{
			wHeader = RE_SCORE_RATE;
			bModify = true;
			nError = ERR_INVALID;
		}
	}RES_MODIFY_SCORE_RATE,*PRES_MODIFY_SCORE_RATE;

	/*********************************** 二期功能***********************************/

	//
	//	帐号激活查询 
	//
	typedef struct _tagRES_ACCOUNT_ACTIVE_SEARCH:  PacketBase_S2T
	{
		// 返回查询结果；成功：true ，读取data_success ，失败：false，读取 data_fail 
		bool bSearch;	

		union
		{
			struct  
			{
				char szAccountName[MAX_ACCOUNT_LENGTH];       // 帐号名称
				ui32 nActiveRegionID;						  // 激活区域ID
				SQLSVR_DATETIME  tActiveDate;				  // 激活时间
			}data_success;

			struct
			{
				ERROR_CODE nError;							  // 错误代号
			}data_fail;
		};
		ERROR_CODE nError;									  // 错误代号
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
	//	返回物品查询 
	//
	typedef struct _tagRES_ITEM_SEARCH:  PacketBase_S2T
	{
		ui32 nPlayerRecordNum;							// 本次发送玩家基本信息的条数
		PLAYER_CHARACTER_BASE_INFO *pPlayerCharInfos;	// 玩家基本信息
		
		ui32 nDataRecordNum;							// 本次发送物品数据条数
		GOODS_INFO	*pData;								// 物品数据


		ui32 nPacketTotal;								// 共有多少个消费记录数据包	  	
		ui32 nPacketIndex;								// 本次发送消费记录数据包的序号 

		ERROR_CODE nError;								// 错误代号

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
	//	返回消费记录查询 
	//
	typedef struct _tagRES_CONSUME_RECORD_SEARCH:  PacketBase_S2T
	{
		ui32 nPlayerRecordNum;							// 本次发送玩家基本信息的条数
		PLAYER_CHARACTER_BASE_INFO *pPlayerCharInfos;	// 玩家基本信息
								

		ui32 nDataRecordNum;							// 本次发送消费记录数据条数
		CONSUME_RECORD_INFO	*pData;						// 消费记录数据
								
		
		ui32 nPacketTotal;								// 共有多少个消费记录数据包	  	
		ui32 nPacketIndex;								// 本次发送消费记录数据包的序号 	

		ERROR_CODE nError;								// 错误代号

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
	//	返回帐号登陆日志
	//
	typedef struct _tagRES_ACCOUNT_LOGIN_LOGOUT_LOG:  PacketBase_S2T
	{
		ui32 nPlayerRecordNum;					// 本次发送玩家基本信息的条数
		PLAYER_CHARACTER_BASE_INFO *pPlayerCharInfos;	// 玩家基本信息
		
		ui32 nDataRecordNum;					// 本次发送登陆日志信息的条数
		LOGIN_OUT_INFO	*pData;					// 登陆日志 
		

		ui32 nPacketTotal;						// 共有多少个登陆日志数据包	  	
		ui32 nPacketIndex;						// 本次发送登陆日志数据包的序号 

		ERROR_CODE nError;						// 错误代号

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
	//	返回技能查询
	//  
	typedef struct _tagRES_SKILL_SEARCH:  PacketBase_S2T
	{
		ui32 nPlayerRecordNum;					// 本次发送玩家基本信息的条数
		PLAYER_CHARACTER_BASE_INFO *pPlayerCharInfos;	// 玩家基本信息
		
		ui32 nDataRecordNum;					// 本次发送技能数据的条数
		SKIN_RECORD_INFO	*pData;				// 技能数据
		

		ui32 nPacketTotal;						// 共有多少个技能数据记录数据包	  	
		ui32 nPacketIndex;						// 本次发送技能数据记录数据包的序号 

		ERROR_CODE nError;						// 错误代号

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
	//	返回角色操作（修改、删除、恢复）
	//  
	typedef struct _tagRES_CHARACTER_INFO_OPERATION:  PacketBase_S2T
	{
		// 操作类型
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

		ui32 nPlayerRecordNum;					// 本次发送玩家基本信息的条数
		PLAYER_CHARACTER_BASE_INFO *pPlayerCharInfos;	// 玩家基本信息
		bool bSuccess;									// 操作是否成功

		ERROR_CODE nError;								// 错误代号

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

	/*********************************** 三期功能 ***********************************/

	//
	//	返回好友信息查询
	//
	typedef struct _tagRES_FRIEND_SEARCH:  PacketBase_S2T
	{
		ui32 nDataRecordNum;					// 本次发送玩家好友信息的条数
		FRIEND_INFO *pData;				// 好友信息

		ui32 nPacketTotal;						// 共有多少个好友数据记录数据包	  	
		ui32 nPacketIndex;						// 本次发送好友数据记录数据包的序号 

		ERROR_CODE nError;						// 错误代号

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
	// 返回玩家日志信息
	//
	typedef struct _tagRES_UPLEVE_ITEM_LOG_SEARCH:  PacketBase_S2T
	{

		// 操作类型
		enum
		{
			OP_INVALID = 0,
			OP_UPGRADE,    // 升级记录
			OP_ITEM_USE,   // 道具使用记录
			OP_ITEM_DELETE,// 道具删除记录
			OP_CHARECTORDELETE, // 角色删除记录
		}RE_type;	

		union
		{
			struct  
			{
				ui32 nDataRecordNum;			// 本次发送玩家升级信息的条数	
				UPLEVEL_LOG *pData;		// 玩家升级信息
			}data_uplevel_log;

			struct  
			{
				ui32   nDataRecordNum;			 //道具使用记录条数
				ITEM_USE_LOG *pData;			
			}data_itemuser_log;

			struct  
			{
				ui32 nDataRecordNum;			// 本次发送玩家删除道具信息的条数	
				ITEM_DELETE_LOG *pData;
			}data_item_delete_log;

			struct  
			{
				ui32 nDataRecordNum;			// 本次发送玩家删除角色的条数	
				CHARECTOR_DELETE_LOG *pData;
			}data_chractor_delete_log;
		};

		ui32 nPacketTotal;					// 共有多少个技能数据记录数据包	  	
		ui32 nPacketIndex;					// 本次发送技能数据记录数据包的序号 

		ERROR_CODE nError;					// 错误代号

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
	// 道具列表
	// 

	//
	// 返回赠送道具信息
	//
	typedef struct _tagRES_PRASENT_ITEM:  PacketBase_S2T
	{
		bool bSuccess;						// 是否赠送成功
		ERROR_CODE nError;					// 错误代号
		_tagRES_PRASENT_ITEM()
		{
			memset(this, 0, sizeof(_tagRES_PRASENT_ITEM));
			wHeader = RE_PRASENT_ITEM;
			nError = ERR_INVALID;
		}

	}RES_PRASENT_ITEM,*PRES_PRASENT_ITEM;


	// 活动
	typedef	struct _tagRES_ACTIVE_CAMPAIGN :
		public PacketBase_S2T
	{	
		struct  
		{
			ui8 nDetailID;		// 次分类
			ui8	nMainID;		// 主分类，同时代表顺序
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
