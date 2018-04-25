#pragma once

#include "BackStageMgrDef.h"
//
// 后台管理系统发送给rs的消息结构定义
//
#pragma pack(push, 1)

namespace MSG_BSTRS
{
	// 查询类
	const int CMD_REQ_LOGIN					= BACKSTAGEMGR::MSG_BASE_BSTRS + 1;
	const int CMD_REQ_CHARACTER_INFO		= BACKSTAGEMGR::MSG_BASE_BSTRS + 2;
	const int CMD_REQ_CLOSURE_ACCOUNT_INFO	= BACKSTAGEMGR::MSG_BASE_BSTRS + 3;

	// 操作类
	const int CMD_REQ_KICK_USER				= BACKSTAGEMGR::MSG_BASE_BSTRS + 100;
	const int CMD_REQ_CLOSE_ACCOUNT			= BACKSTAGEMGR::MSG_BASE_BSTRS + 101;
	const int CMD_REQ_AVAILABLE_ACCOUNT		= BACKSTAGEMGR::MSG_BASE_BSTRS + 102;
	const int CMD_REQ_SYSTEM_BROADCAST		= BACKSTAGEMGR::MSG_BASE_BSTRS + 103;

	//
	// 登录到rs
	// 
	struct REQ_LOGIN : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		char	loginUserName[BACKSTAGEMGR::MAX_ID_LENGTH];		// 登录用户名
		char	loginPassWord[BACKSTAGEMGR::MAX_PASSWORD_LENGTH]; // 登录密码

		REQ_LOGIN()
			:TCP_MSG_BASE(CMD_REQ_LOGIN)
		{
			loginUserName[0] = '\0';
			loginPassWord[0] = '\0';
		}
	};

	//
	// 查询角色信息
	//
	struct REQ_CHARACTER_INFO : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		// 查询类型
		enum REQ_TYPE
		{
			RT_INVALID			= 0,
			RT_CHAR_BASE_INFO	= 1,	// 角色基本信息
			RT_CHAR_ITEM_INFO	= 2,	// 角色物品信息
		};
		
		// 查询条件
		enum QUERY_CONDITION
		{
			QC_INVALID = 0,
			QC_USERNAME,	// 通过帐号
			QC_CHARNAME,	// 角色名称
		};
	
		REQ_TYPE		req_type;		
		QUERY_CONDITION	query_condition;

		union
		{
			struct  
			{
				char userName[BACKSTAGEMGR::MAX_ID_LENGTH];
			}user_name; // 用户帐号

			struct  
			{
				char charName[BACKSTAGEMGR::MAX_CHARACTER_NAME_LENGTH];
			}char_name; // 角色名称
		};
		
		REQ_CHARACTER_INFO()
			:TCP_MSG_BASE(CMD_REQ_CHARACTER_INFO),req_type(RT_INVALID),
			query_condition(QC_INVALID)
		{}
	};

	//
	// 查询帐号封停信息
	//
	struct REQ_CLOSURE_ACCOUNT_INFO : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		// 查询范围
		enum REQ_RANGE
		{
			RR_INVALID = 0,
			RR_PER_CLOSUREACCOUNT,	// 单个封停帐号	
			RR_ALL_CLOSUREACCOUNT,	// 所有封停帐号
		}reqRange;

		char		userName[BACKSTAGEMGR::MAX_ID_LENGTH]; // 查询当个封停帐号时用到

		REQ_CLOSURE_ACCOUNT_INFO()
			:TCP_MSG_BASE(CMD_REQ_CLOSURE_ACCOUNT_INFO),reqRange(RR_INVALID)
		{
			userName[0] = '\0';
		}
	};


	//
	// 踢人下线
	// 
	struct REQ_KICK_USER : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		enum
		{
			C_INVALID = 0,
			C_USERNAME,			// 通过帐号名
			C_CHARACTERNAME,	// 通过角色名
		}kickCondition;

		union
		{
			struct  
			{
				char userName[BACKSTAGEMGR::MAX_ID_LENGTH];
			}user_name; // 用户帐号

			struct  
			{
				char charName[BACKSTAGEMGR::MAX_CHARACTER_NAME_LENGTH];
			}char_name; // 角色名称 
		};

		REQ_KICK_USER()
			:TCP_MSG_BASE(CMD_REQ_KICK_USER),kickCondition(C_INVALID)
		{}
	};

	//
	// 封停帐号
	//
	struct REQ_CLOSE_ACCOUNT : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		// 封停原因
		enum
		{
			R_UNKNOWN	= 0,
			R_SYSTEM,			// 系统封停
			R_SYSTEM_TEMP,		// 系统临时封停
			R_USERCUSTOM,		// 用户自助
			R_USERCUSTOM_TEMP,	// 用户临时自助
		}eResult;

		enum
		{
			C_INVALID = 0,
			C_USERNAME,			// 通过帐号名
			C_CHARACTERNAME,	// 通过角色名
		}closeCondition;

		unsigned int duration_time;	// 封停时间(按分钟计算)

		union
		{
			struct  
			{
				char userName[BACKSTAGEMGR::MAX_ID_LENGTH];
			}user_name; // 用户帐号

			struct  
			{
				char charName[BACKSTAGEMGR::MAX_CHARACTER_NAME_LENGTH];
			}char_name; // 角色名称 
		};

		REQ_CLOSE_ACCOUNT()
			:TCP_MSG_BASE(CMD_REQ_CLOSE_ACCOUNT),closeCondition(C_INVALID)
		{}
	};

	//
	// 解封帐号
	//
	struct REQ_AVAILABLE_ACCOUNT : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		enum
		{
			C_INVALID = 0,
			C_USERNAME,			// 通过帐号名
			C_CHARACTERNAME,	// 通过角色名
		}availableCondition;

		union
		{
			struct  
			{
				char userName[BACKSTAGEMGR::MAX_ID_LENGTH];
			}user_name; // 用户帐号

			struct  
			{
				char charName[BACKSTAGEMGR::MAX_CHARACTER_NAME_LENGTH];
			}char_name; // 角色名称 
		};

		REQ_AVAILABLE_ACCOUNT()
			:TCP_MSG_BASE(CMD_REQ_CLOSE_ACCOUNT),availableCondition(C_INVALID)
		{}
	};

	//
	// 系统广播(变成数据包,发送数据包时 数据包长度调用GetSize()获得)
	//
	struct REQ_SYSTEM_BROADCAST : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		// 广播类型
		enum
		{
			T_INVALID = 0,
			T_NOW,			// 马上广播
			T_DELAY,		// 延迟广播
		}bType;
		
		unsigned short	broadcastSize;	// 广播消息长度
		// 开始广播时间(延迟广播用到)
		unsigned short	start_month;	
		unsigned short	start_day;
		unsigned short	start_hour;
		unsigned short	start_minute;
		// 结束广播时间(延迟广播用到)
		unsigned short	finish_month;	
		unsigned short	finish_day;
		unsigned short	finish_hour;
		unsigned short	finish_minute;
		unsigned short	interval;	// 广播间隔(分钟)(延迟广播用到)

		char	broadcastTxt[0xff]; // 广播内容

		REQ_SYSTEM_BROADCAST()
			:TCP_MSG_BASE(CMD_REQ_SYSTEM_BROADCAST)
		{
			broadcastSize = (unsigned short)(sizeof(REQ_SYSTEM_BROADCAST) - 0xff);
		}

		// 添加广播消息内容
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