#pragma once

#include "BackStageMgrDef.h"

//
// rs发送给后台管理系统的消息结构定义
//

#pragma pack(push, 1)

namespace MSG_RSTBS
{
	const int CMD_REP_LOGIN					= BACKSTAGEMGR::MSG_BASE_RSTBS + 1;
	const int CMD_REP_CHARACTER_BASE_INFO	= BACKSTAGEMGR::MSG_BASE_RSTBS + 2;
	const int CMD_REP_CHARACTER_ITEMS_INFO	= BACKSTAGEMGR::MSG_BASE_RSTBS + 3;
	const int CMD_REP_CLOSURE_ACCOUNT_INFO	= BACKSTAGEMGR::MSG_BASE_RSTBS + 4;
	const int CMD_REP_CHARACTER_INFO_FAIL	= BACKSTAGEMGR::MSG_BASE_RSTBS + 5;

	const int CMD_REP_KICK_USER				= BACKSTAGEMGR::MSG_BASE_RSTBS + 100;
	const int CMD_REP_CLOSE_ACCOUNT			= BACKSTAGEMGR::MSG_BASE_RSTBS + 101;
	const int CMD_REP_AVAILABLE_ACCOUNT		= BACKSTAGEMGR::MSG_BASE_RSTBS + 102;
	const int CMD_REP_SYSTEM_BROADCAST		= BACKSTAGEMGR::MSG_BASE_RSTBS + 103;
	const int CMD_REP_NAME_ID_LIST			= BACKSTAGEMGR::MSG_BASE_RSTBS + 104;
	

	// 
	// 返回登陆结果
	// 
	struct REP_LOGIN : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		enum
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// 登录成功
			E_FAILED_REPEATEDREQUEST,				// 重复发送登录请求
			E_FAILED_SERVERINTERNALERROR,			// 服务器内部错误
			E_FAILED_INVALIDACCOUNTORPASSWORD,		// 帐号或者密码错误
		}eResult;

		REP_LOGIN()
			:TCP_MSG_BASE(CMD_REP_LOGIN),eResult(E_FAILED_UNKNOWNERROR)
		{}
	};

	//
	// 返回角色基本信息
	//
	struct REP_CHARACTER_BASE_INFO : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		unsigned int	srv_realm_id;	// 大区id
		unsigned int	srv_cluster_id;	// 服务器组id
		char			user_name[BACKSTAGEMGR::MAX_ID_LENGTH]; // 帐号
		unsigned int	char_id;		// 角色id
		char			char_name[BACKSTAGEMGR::MAX_CHARACTER_NAME_LENGTH];		// 角色名称
		char			char_title1[BACKSTAGEMGR::MAX_CHARACTER_NAME_LENGTH];	// 角色称谓1
		char			char_title2[BACKSTAGEMGR::MAX_CHARACTER_NAME_LENGTH];	// 角色称谓2
		unsigned char	char_type;			// 角色类型(职业性别等)
		bool			is_online;			// 是否在线
		bool			is_closed;			// 是否封停
		unsigned char	char_level;			// 角色等级
		unsigned int	money;				// 游戏币数量
		unsigned int	nineyou_money;		// 9游币数量
		unsigned int	reserve_money;		// 储备金
		unsigned int	cur_scene_id;		// 当前所在场景id
		unsigned int	cur_scene_pos;		// 当前躲在场景坐标

		unsigned int	hp;					// 生命值
		unsigned short	mp;					// 魔法值
		unsigned short	vitality;			// 活力值
		unsigned short	good_evil;			// 善恶值
		unsigned short	physical_attack;	// 物理攻击
		unsigned short	physical_defense;	// 物理防御
		unsigned short	magic_attack;		// 魔法攻击
		unsigned short	magic_defense;		// 魔法防御
		float			critical;			// 暴击率
		unsigned short	strength;			// 力量
		unsigned short	dexterity;			// 敏捷
		unsigned short	intellect;			// 智力
		unsigned short	corporeity;			// 体质
		unsigned short	potential_point;	// 潜力点
		unsigned int	exp;				// 经验
		unsigned int	exp_next_level;		// 升级经验

		float			physical_resist;	// 物理抗性
		float			magic_resist;		// 法术抗性
		float			fire_resist;		// 火焰抗性
		float			cold_resist;		// 冰冻抗性
		float			poison_resist;		// 毒素抗性

		REP_CHARACTER_BASE_INFO()
			:TCP_MSG_BASE(CMD_REP_CHARACTER_BASE_INFO)
		{}
	};

	//
	// 返回角色物品信息
	// 
	struct REP_CHARACTER_ITEMS_INFO : public BACKSTAGEMGR::TCP_MSG_BASE
	{

		REP_CHARACTER_ITEMS_INFO()
			:TCP_MSG_BASE(CMD_REP_CHARACTER_ITEMS_INFO)
		{}
	};

	//
	// 返回角色信息查询失败
	//
	struct REP_CHARACTER_INFO_FAIL : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		enum
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// 查询封停信息成功
			E_FAILED_ACCOUNT_NOTEXIST,				// 帐号不存在
			E_FAILED_SERVERINTERNALERROR,			// 服务器内部错误
		}eResult;

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

		REP_CHARACTER_INFO_FAIL()
			: TCP_MSG_BASE(CMD_REP_CHARACTER_INFO_FAIL)
		{}
	};

	//
	// 返回帐号封停信息
	//
	struct REP_CLOSURE_ACCOUNT_INFO : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		enum
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// 查询封停信息成功
			E_FAILED_ACCOUNT_NOTEXIST,				// 帐号不存在
			E_FAILED_SERVERINTERNALERROR,			// 服务器内部错误
		}eResult;

		// 返回信息类型
		enum
		{
			R_UNKNOWNE = 0,
			R_USERNAME,			// 帐号名称
			R_CHARACTER_NAME,	// 角色名称
		}rType;

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

		// 成功的话
		unsigned int	closure_reason;	// 封停原因
		unsigned int	duration_time;	// 封停剩余时间(分钟)

		REP_CLOSURE_ACCOUNT_INFO()
			:TCP_MSG_BASE(CMD_REP_CLOSURE_ACCOUNT_INFO),eResult(E_FAILED_UNKNOWNERROR)
		{}
	};

	//
	// 返回踢人结果
	//
	struct REP_KICK_USER : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		enum
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// 踢人成功
			E_FAILED_CHAR_NOTINGAME,				// 帐号没有角色在游戏中
			E_FAILED_SERVERINTERNALERROR,			// 服务器内部错误
		}eResult;
		// 返回信息类型
		enum
		{
			R_UNKNOWNE = 0,
			R_USERNAME,			// 帐号名称
			R_CHARACTER_NAME,	// 角色名称
		}rType;

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

		REP_KICK_USER()
			:TCP_MSG_BASE(CMD_REP_KICK_USER)
		{}
	};

	//
	// 封停帐号结果
	//
	struct REP_CLOSE_ACCOUNT : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		enum
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// 封停成功
			E_FAILED_ACCOUNT_NOTEXIST,				// 帐号不存在
			E_FAILED_SERVERINTERNALERROR,			// 服务器内部错误
		}eResult;

		// 返回信息类型
		enum
		{
			R_UNKNOWNE = 0,
			R_USERNAME,			// 帐号名称
			R_CHARACTER_NAME,	// 角色名称
		}rType;

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

		// 成功的话
		unsigned int duration_time;	// 封停剩余时间(分钟)

		REP_CLOSE_ACCOUNT()
			:TCP_MSG_BASE(CMD_REP_CLOSE_ACCOUNT)
		{}
	};

	//
	// 解封帐号结果
	//
	struct REP_AVAILABLE_ACCOUNT : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		enum
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// 解封成功
			E_FAILED_ACCOUNT_NOTEXIST,				// 帐号不存在
			E_FAILED_SERVERINTERNALERROR,			// 服务器内部错误
		}eResult;

		char userName[BACKSTAGEMGR::MAX_ID_LENGTH]; // 帐号
		
		REP_AVAILABLE_ACCOUNT()
			:TCP_MSG_BASE(CMD_REP_AVAILABLE_ACCOUNT)
		{
			userName[0] = '\0';
		}
	};

	//
	// 系统广播消息结果
	// 
	struct REP_SYSTEM_BROADCAST : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		enum
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// 广播成功
			E_FAILED_SERVERINTERNALERROR,			// 服务器内部错误
		}eResult;

		REP_SYSTEM_BROADCAST()
			:TCP_MSG_BASE(CMD_REP_SYSTEM_BROADCAST)
		{}
	};

	//
	// 名字ID列表
	//
	struct REP_NAME_ID_LIST : public BACKSTAGEMGR::TCP_MSG_BASE
	{
		struct NAME_ID_ELEM
		{
			union 
			{
				struct  
				{
					unsigned int userIdx;
					char userName[BACKSTAGEMGR::MAX_ID_LENGTH];
				}user_name; // 用户帐号

				struct  
				{
					unsigned int charIdx;
					char charName[BACKSTAGEMGR::MAX_CHARACTER_NAME_LENGTH];
				}char_name; // 角色名称 
			};
		};

		enum
		{
			MAX_NAME_ID_ELEM = 100,
		};

		// 返回信息类型
		enum
		{
			R_UNKNOWNE = 0,
			R_USERNAME,			// 帐号名称
			R_CHARACTER_NAME,	// 角色名称
		}rType;

		// 返回结果
		enum
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// 查询成功
			E_FAILED_NOTEXIST,				// 角色或帐号不存在
			E_FAILED_SERVERINTERNALERROR,			// 服务器内部错误
		}eResult;

		// 列表项的个数
		unsigned int elemCount;

		// 两个参数(备用)
		unsigned int param1;
		unsigned int param2;
		
		// 列表项
		NAME_ID_ELEM elem[MAX_NAME_ID_ELEM];

		bool AddElem(NAME_ID_ELEM &newElem)
		{
			if(elemCount == MAX_NAME_ID_ELEM)
			{
				return false;
			}
			else
			{
				memcpy(elem + elemCount, &newElem, sizeof(NAME_ID_ELEM));
				++elemCount;
				return true;
			}
		}

		unsigned int GetSize()
		{
			return sizeof(REP_NAME_ID_LIST) - sizeof(NAME_ID_ELEM) * (MAX_NAME_ID_ELEM - elemCount);
		}
		
		REP_NAME_ID_LIST()
			: TCP_MSG_BASE(CMD_REP_NAME_ID_LIST)
		{
			elemCount = 0;
		}
	};

}; // namespace MSG_WSTBS

#pragma pack(pop)