/******************************************************************************
						GM Tools与GM Server通信协议

GM Tools请求类(T2S)
create by LiaoHuaifu 2008/9/2

二期功能: 2008/10/18
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
	// T2S 基本包结构
	//
	typedef struct _tagPacketBase_T2S
	{

		CMD_TYPE	wHeader;						 //	消息头,

		_tagPacketBase_T2S()
		{
			wHeader   = OP_INVALID;
        }

	}PacketBase_T2S,*PPacketBase_T2S;

	/*********************************** 一期功能***********************************/

	//
	//GMTools 请求登录GMS,GMS根据GmsID、szPassword、szAccountName来验证GMTools使用者的身份
	//
	typedef struct _tagREQ_LOGIN_GMS : PacketBase_T2S
	{
		char	szPassword[MAX_PASSWORD_LENGTH];
		char	szAccountName[MAX_ACCOUNT_LENGTH];
		//char	szIP[MAX_ACCOUNT_LENGTH];
		ui32	nDstGMSID;					     //	目的GM Server的ID

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
	// 查看玩家基本信息
	//
	typedef struct _tagREQ_PLAYER_INFO : PacketBase_T2S
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];		 //	角色名称
		char 	szAccountName[MAX_ACCOUNT_LENGTH];       //  帐号名称

		_tagREQ_PLAYER_INFO()
		{
			memset(szCharName,0,sizeof(szCharName));
			memset(szAccountName,0,sizeof(szAccountName));
			wHeader = OP_PLAYER_INFO;
		}

	}REQ_PLAYER_INFO,*PREQ_PLAYER_INFO;

	//
	// 查看玩家角色的基本信息
	//
	typedef struct _tagREQ_CHAR_INFO : PacketBase_T2S
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];		//	角色名称

		_tagREQ_CHAR_INFO()
		{
			memset(szCharName,0,sizeof(szCharName));
			wHeader = OP_CHAR_INFO;
        }
	}REQ_CHAR_INFO,*PREQ_CHAR_INFO;

	//
	// 强制玩家断线
	//
	typedef struct _tagREQ_KICK_PLAYER : PacketBase_T2S
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];		//	角色名称

		_tagREQ_KICK_PLAYER()
		{
			memset(szCharName,0,sizeof(szCharName));
			wHeader = OP_KICK;
        }
	}REQ_KICK_PLAYER,*PREQ_KICK_PLAYER;

	//
	// 封禁玩家
	//
	typedef struct _tagREQ_BAN_PLAYER : PacketBase_T2S
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];		 // 角色名称
		char 	szAccountName[MAX_ACCOUNT_LENGTH];      //  帐号名称
		i8		nDay;									 //  封禁天数  说明：封禁天数的范围为0—30，0表示永久封禁

		_tagREQ_BAN_PLAYER()
		{
			memset(szCharName,0,sizeof(szCharName));
			memset(szAccountName,0,sizeof(szAccountName));
			wHeader = OP_BAN;
			nDay = 31;
		}
	}REQ_BAN_PLAYER,*PREQ_BAN_PLAYER;

	//
	// 发布公告
	//
	typedef struct _tagREQ_POP_MSG : PacketBase_T2S
	{
		ui32 nIndex;                      // 消息索引
		enum
		{
			NORMAL,							// 普通公告
			ROLL,         					// 滚动公告
		}op_type;

		i8 nDelayTime;						// 延迟时间（单位：分钟); 0 为即时

		char popMsg[MAX_POP_MSG_LENGTH];   	// 公告内容
		
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
	// 解封玩家
	// 
	typedef struct _tagREQ_RELEASE_PLAYER : PacketBase_T2S
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];		 // 角色名称
		char 	szAccountName[MAX_ACCOUNT_LENGTH];      //  帐号名称

		_tagREQ_RELEASE_PLAYER()
		{
			memset(szCharName,0,sizeof(szCharName));
			memset(szAccountName,0,sizeof(szAccountName));
			wHeader = OP_RELEASE;
		}
	}REQ_RELEASE_PLAYER,*PREQ_RELEASEPLAYER;

	//
	// 修改经验值倍数	
	//	Add 2008/9/26
	typedef struct _tagREQ_MODIFY_EXP_RATE :  PacketBase_T2S
	{
		real expRate;	// 经验值倍数
		int	 nDoubleExpBeginTime; // 设定经验开始时间
		int	 nDoubleExpEndTime;	 //  设定经验结束时间
		_tagREQ_MODIFY_EXP_RATE()
		{
			wHeader = OP_EXP_RATE;
			expRate = 0;
		}
	}REQ_MODIFY_EXP_RATE,*PREQ_MODIFY_EXP_RATE;

	//
	// 修改积分值倍数	
	//	Add 2009/6/23
	typedef struct _tagREQ_MODIFY_SCORE_RATE :  PacketBase_T2S
	{
		real scoreRate;	// 积分倍数
		int	 nDoubleScoreBeginTime; // 设定积分开始时间
		int	 nDoubleScoreEndTime;	 //  设定积分结束时间
		_tagREQ_MODIFY_SCORE_RATE()
		{
			wHeader = OP_SCORE_RATE;
			scoreRate = 0;
		}
	}REQ_MODIFY_SCORE_RATE,*PREQ_MODIFY_SCORE_RATE;


	/*********************************** 二期功能***********************************/

	//
	//	帐号激活查询 
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
	//	物品查询 
	//
	typedef struct _tagREQ_ITEM_SEARCH:  PacketBase_T2S
	{
		//操作优先级为：角色名称、账号名称；即当角色名不为空，服务按角色名称为操作依据
		PLAYER_UNIQUE_INFO	playerInfo;	

		_tagREQ_ITEM_SEARCH()
		{
			wHeader = OP_ITEM_SEARCH;
		}
	}REQ_ITEM_SEARCH,*PREQ_ITEM_SEARCH;

	//
	//	消费记录查询 
	//
	typedef struct _tagREQ_CONSUME_RECORD_SEARCH:  PacketBase_T2S
	{
		//操作优先级为：角色名称、账号名称；即当角色名不为空，服务按角色名称为操作依据
		PLAYER_UNIQUE_INFO	playerInfo;	

		char	szCharName_Receive[MAX_CHAR_NAME_LENGTH];		 // 接收者角色名称
		char 	szAccountName_Receive[MAX_ACCOUNT_LENGTH];       // 接收者帐号名称
		//SQLSVR_DATETIME beginTime;								// 查询开始时间
		//SQLSVR_DATETIME endTime;									// 查询结束时间

		_tagREQ_CONSUME_RECORD_SEARCH()
		{
			wHeader = OP_CONSUME_RECORD_SEARCH;
		}
	}REQ_CONSUME_RECORD_SEARCH,*PREQ_CONSUME_RECORD_SEARCH;

	//
	//	帐号登陆日志
	//
	typedef struct _tagREQ_ACCOUNT_LOGIN_LOGOUT_LOG:  PacketBase_T2S
	{
		//操作优先级为：角色名称、账号名称；即当角色名不为空，服务按角色名称为操作依据
		PLAYER_UNIQUE_INFO	playerInfo;	
		//SQLSVR_DATETIME beginTime;								// 查询开始时间
		//SQLSVR_DATETIME endTime;									// 查询结束时间

		_tagREQ_ACCOUNT_LOGIN_LOGOUT_LOG()
		{
			wHeader = OP_ACCOUNT_LOGIN_LOGOUT_LOG;
		}
	}REQ_ACCOUNT_LOGIN_LOGOUT_LOG,*PREQ_ACCOUNT_LOGIN_LOGOUT_LOG;

	//
	//	技能查询
	// 
	typedef struct _tagREQ_SKILL_SEARCH:  PacketBase_T2S
	{
		//操作优先级为：角色名称、账号名称；即当角色名不为空，服务按角色名称为操作依据
		PLAYER_UNIQUE_INFO	playerInfo;	

		_tagREQ_SKILL_SEARCH()
		{
			wHeader = OP_SKILL_SEARCH;
		}
	}REQ_SKILL_SEARCH,*PREQ_SKILL_SEARCH;	

	//
	//	角色操作（修改、删除、恢复）
	//  注意：playerInfo的角色不能为空，否则服务器不知道修改/删除/恢复具体那角色的信息
	typedef struct _tagREQ_CHARACTER_INFO_OPERATION:  PacketBase_T2S
	{
		// 操作类型
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

		//操作优先级为：角色名称、账号名称；即当角色名不为空，服务按角色名称为操作依据
		PLAYER_UNIQUE_INFO	playerInfo;		

		union
		{
			struct  
			{
				char szNewCharName[MAX_CHAR_NAME_LENGTH];		 // 新角色名称
			}data_modify;

			struct  
			{
				ui32 nReserve;									
			}data_delete;										// 删除角色

			struct  
			{
				ui32 nReserve;									
			}data_renew;										// 恢复角色

			struct  
			{
				i32 newgb;
			}data_gb;											// 修改金币

			struct  
			{
				ui8 nNewLevel;
			}data_modify_level;									// 修改等级

			struct  
			{
				char newPws[32];								// 修改密码
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


	/*********************************** 三期功能 ***********************************/


	//
	//	好友信息查询 
	//
	typedef struct _tagREQ_FRIEND_SEARCH:  PacketBase_T2S
	{
		//操作优先级为：角色名称、账号名称；即当角色名不为空，服务按角色名称为操作依据
		PLAYER_UNIQUE_INFO	playerInfo;	

		_tagREQ_FRIEND_SEARCH()
		{
			wHeader = OP_FRIEND_SEARCH;
		}
	}REQ_FRIEND_SEARCH,*PREQ_FRIEND_SEARCH;


	//
	//	玩家日志信息 
	//
	typedef struct _tagREQ_LOG_SEARCH:  PacketBase_T2S
	{
		// 操作类型
		enum
		{
			OP_INVALID = 0,
			OP_UPGRADE,    // 升级记录
			OP_ITEM_USE,   // 道具使用记录
			OP_ITEM_DELETE,// 道具删除记录
			OP_CHARECTORDELETE, // 角色删除记录
		}op_type;	

		//操作优先级为：角色名称、账号名称；即当角色名不为空，服务按角色名称为操作依据
		PLAYER_UNIQUE_INFO	playerInfo;	
		//SQLSVR_DATETIME beginTime;								// 查询开始时间
		//SQLSVR_DATETIME endTime;									// 查询结束时间
		_tagREQ_LOG_SEARCH()
		{
			op_type = OP_INVALID;
			wHeader = OP_LOG_SEARCH;
		}
	}REQ_LOG_SEARCH,*PREQ_LOG_SEARCH;


	//
	// 道具赠送
	//
	typedef struct _tagREQ_ITEM_PRASENT:  PacketBase_T2S
	{
		// 角色名不能为空
		PLAYER_UNIQUE_INFO playerInfo;
		ITEM_OBJ prasendtItem;
		char		mail_text[201];			//	邮件正文

		_tagREQ_ITEM_PRASENT()
		{
			memset(this, 0, sizeof(_tagREQ_ITEM_PRASENT));
			wHeader = OP_PRASENT_ITEM;
		}
	}REQ_PRASENT_ITEM,*PREQ_PRASENT_ITEM;

	// 活动
	typedef	struct _tagREQ_ACTIVE_CAMPAIGN :
	public PacketBase_T2S
	{	
		struct  
		{
			ui8 nDetailID;		// 次分类
			ui8	nMainID;		// 主分类，同时代表顺序
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