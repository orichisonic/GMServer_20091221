#pragma once

#include "AdminDef.h"

#pragma pack(push, 1)

namespace MSG_ATAS
{
	// 查询类
	const int CMD_REQ_LOGIN					= ADMIN::MSG_BASE_ATAS + 1;		// 登录
	const int CMD_REQ_CHARACTER_INFO		= ADMIN::MSG_BASE_ATAS + 2;		// 查询角色信息
	const int CMD_REQ_CLOSURE_ACCOUNT_INFO	= ADMIN::MSG_BASE_ATAS + 3;		// 查询帐号信息

	// 操作类
	const int CMD_REQ_KICK_USER				= ADMIN::MSG_BASE_ATAS + 100;	// 踢人
	const int CMD_REQ_CLOSE_ACCOUNT			= ADMIN::MSG_BASE_ATAS + 101;	// 封停
	const int CMD_REQ_AVAILABLE_ACCOUNT		= ADMIN::MSG_BASE_ATAS + 102;	// 解封
	const int CMD_REQ_SYSTEM_BROADCAST		= ADMIN::MSG_BASE_ATAS + 103;	// 广播
	const int CMD_REQ_DELETE_ITEM			= ADMIN::MSG_BASE_ATAS + 104;	// 删除物品
	const int CMD_REQ_MODIFY_CHARACTER		= ADMIN::MSG_BASE_ATAS + 105;	// 改钱,改名,改等级
	const int CMD_REQ_MODIFY_CHARACTER_SKILL= ADMIN::MSG_BASE_ATAS + 106;	// 修改角色技能
	const int CMD_REQ_RECOVER_CHRACTER		= ADMIN::MSG_BASE_ATAS + 107;	// 恢复角色
	const int CMD_REQ_MODIFY_ACCOUNT_PASSW	= ADMIN::MSG_BASE_ATAS + 108;	// 修改密码
	const int CMD_REQ_MAIL_ITEM				= ADMIN::MSG_BASE_ATAS + 109;	// 给角色邮寄物品
	const int CMD_REQ_MODIFY_CHARACTER_NAME	= ADMIN::MSG_BASE_ATAS + 110;	// 给角色改名

	// 日志查询
	const int CMD_REQ_LOG_MONSTER_DROP_ITEM	= ADMIN::MSG_BASE_ATAS + 250;	// 物品:怪物掉落
	const int CMD_REQ_LOG_BUY_ITEM_FROM_NPC	= ADMIN::MSG_BASE_ATAS + 251;	// 物品:从NPC处购买
	const int CMD_REQ_LOG_SELL_ITEM_TO_NPC	= ADMIN::MSG_BASE_ATAS + 252;	// 物品:卖给NPC
	const int CMD_REQ_LOG_DROP_ITEM			= ADMIN::MSG_BASE_ATAS + 253;	// 物品:玩家自行删除
	const int CMD_REQ_LOG_GET_ITEM_TRADE	= ADMIN::MSG_BASE_ATAS + 254;	// 物品:交易获得
	const int CMD_REQ_LOG_LOSE_ITEM_TRADE	= ADMIN::MSG_BASE_ATAS + 255;	// 物品:交易失去
	const int CMD_REQ_LOG_BIND_ITEM			= ADMIN::MSG_BASE_ATAS + 256;	// 物品:装备绑定记录
	const int CMD_REQ_LOG_UNBIND_ITEM		= ADMIN::MSG_BASE_ATAS + 257;	// 物品:装备解绑定记录
	const int CMD_REQ_LOG_BOOTH_ITEM		= ADMIN::MSG_BASE_ATAS + 258;	// 物品:角色“摆摊”日志查询
	const int CMD_REQ_LOG_DELETE_ITEM		= ADMIN::MSG_BASE_ATAS + 259;	// 物品:道具删除记录(指使用时间到了)
	const int CMD_REQ_LOG_USE_ITEM			= ADMIN::MSG_BASE_ATAS + 260;	// 物品:道具使用记录
	const int CMD_REQ_LOG_CRAFT_ITEM		= ADMIN::MSG_BASE_ATAS + 261;	// 物品:装备制作记录
	const int CMD_REQ_LOG_PUNCH_ITEM		= ADMIN::MSG_BASE_ATAS + 262;	// 物品:装备打孔
	const int CMD_REQ_LOG_CHANGE_SLOT_ITEM	= ADMIN::MSG_BASE_ATAS + 263;	// 物品:装备改孔
	const int CMD_REQ_LOG_UPGRADE_STAR		= ADMIN::MSG_BASE_ATAS + 264;	// 物品:装备升星
	const int CMD_REQ_LOG_GEM				= ADMIN::MSG_BASE_ATAS + 265;	// 物品:宝石
	const int CMD_REQ_LOG_PHARMA			= ADMIN::MSG_BASE_ATAS + 266;	// 物品:制药记录
	const int CMD_REQ_LOG_CRAFT_GEM			= ADMIN::MSG_BASE_ATAS + 267;	// 物品:宝石合成
	const int CMD_REQ_LOG_CRAFT_MATIRAIL	= ADMIN::MSG_BASE_ATAS + 268;	// 物品:材料合成
	const int CMD_REQ_LOG_CRAFT_RECIPE		= ADMIN::MSG_BASE_ATAS + 269;	// 物品:图纸合成
	const int CMD_REQ_LOG_MAZE_INTEGRAL		= ADMIN::MSG_BASE_ATAS + 270;	// 迷宫积分兑换奖励
	const int CMD_REQ_LOG_MONSTER_DROP_MONEY= ADMIN::MSG_BASE_ATAS + 271;	// 游戏币:怪物掉落
	const int CMD_REQ_LOG_GET_MONEY_QUEST	= ADMIN::MSG_BASE_ATAS + 272;	// 游戏币:任务获得
	const int CMD_REQ_LOG_LOSE_MONEY_REPAIR	= ADMIN::MSG_BASE_ATAS + 273;	// 游戏币:修装
	const int CMD_REQ_LOG_GET_MONEY_TRADE	= ADMIN::MSG_BASE_ATAS + 274;	// 游戏币:交易获得
	const int CMD_REQ_LOG_LOSE_MONEY_TRADE	= ADMIN::MSG_BASE_ATAS + 275;	// 游戏币:交易失去
	const int CMD_REQ_LOG_QUEST_REWARD		= ADMIN::MSG_BASE_ATAS + 276;	// 任务:完成获得记录
	const int CMD_REQ_LOG_QUEST_GIVE		= ADMIN::MSG_BASE_ATAS + 277;	// 任务:任务物品删除记录
	const int CMD_REQ_LOG_MAIL_SEND			= ADMIN::MSG_BASE_ATAS + 278;	// 邮件:发送记录
	const int CMD_REQ_LOG_MAIL_RECV			= ADMIN::MSG_BASE_ATAS + 279;	// 邮件:接收记录
	const int CMD_REQ_LOG_MAIL_DELETE		= ADMIN::MSG_BASE_ATAS + 280;	// 邮件:删除记录
	const int CMD_REQ_LOG_ITEM_SHOP			= ADMIN::MSG_BASE_ATAS + 281;	// 商城:购买记录
	const int CMD_REQ_LOG_INSTANCE			= ADMIN::MSG_BASE_ATAS + 282;	// 查询用户的副本日志
	const int CMD_REQ_LOG_LOGIN_LOGOUT		= ADMIN::MSG_BASE_ATAS + 283;	// 玩家登陆/登出日志

	//
	// 登录到as
	// 
	struct REQ_LOGIN : public ADMIN::TCP_MSG_BASE
	{
		unsigned int	srv_realm_id;								// 大区ID
		char			loginUserName[ADMIN::MAX_ID_LENGTH];		// 登录用户名
		char			loginPassWord[ADMIN::MAX_PASSWORD_LENGTH];	// 登录密码

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
	struct REQ_CHARACTER_INFO : public ADMIN::TCP_MSG_BASE
	{
		// 查询类型
		enum REQ_TYPE
		{
			RT_INVALID				= 0,
			RT_CHAR_BASE_INFO		= 1,			// 角色基本信息
			RT_CHAR_EQUIPMENT_INFO	= 2,			// 角色装备
			RT_CHAR_ITEM_INFO		= 3,			// 角色物品信息
			RT_CHAR_BANK_INFO		= 4,			// 角色银行信息
			RT_CHAR_MEDAL_INFO		= 5,			// 角色徽章信息(暂时废弃)
			RT_CHAR_PET_INFO		= 6,			// 角色宠物信息
			RT_CHAR_FRIEND_INFO		= 7,			// 角色好友信息
			RT_CHAR_SKILL_INFO		= 8,			// 角色技能信息
		};

		unsigned int			srv_realm_id;		// 大区id
		unsigned int			srv_cluster_id;		// 服务器组id
		unsigned int			param1;				// 参数
		REQ_TYPE				req_type;			// 查询类型
		ADMIN::QUERY_CONDITION	query_condition;	// 查询条件

		union
		{
			ADMIN::QueryUserName		user_name;	// 用户帐号
			ADMIN::QueryCharacterName	char_name;	// 角色名称
		};

		REQ_CHARACTER_INFO()
			:TCP_MSG_BASE(CMD_REQ_CHARACTER_INFO),req_type(RT_INVALID),
			query_condition(ADMIN::QC_INVALID)
		{}
	};

	//
	// 查询帐号封停信息
	//
	struct REQ_CLOSURE_ACCOUNT_INFO : public ADMIN::TCP_MSG_BASE
	{
		// 查询范围
		enum REQ_RANGE
		{
			RR_INVALID = 0,
			RR_PER_CLOSUREACCOUNT,				// 单个封停帐号	
			RR_ALL_CLOSUREACCOUNT,				// 所有封停帐号
		};

		unsigned int			srv_realm_id;	// 大区id
		unsigned int			srv_cluster_id;	// 服务器组id
		REQ_RANGE				reqRange;		// 查询范围
		ADMIN::QueryUserName	user_name;		// 查询当个封停帐号时用到

		REQ_CLOSURE_ACCOUNT_INFO()
			:TCP_MSG_BASE(CMD_REQ_CLOSURE_ACCOUNT_INFO),reqRange(RR_INVALID)
		{
			user_name.userName[0] = '\0';
		}
	};


	//
	// 踢人下线
	// 
	struct REQ_KICK_USER : public ADMIN::TCP_MSG_BASE
	{
		unsigned int			srv_realm_id;		// 大区id
		unsigned int			srv_cluster_id;		// 服务器组id
		ADMIN::QUERY_CONDITION	kickCondition;		// 查询条件
		union
		{
			ADMIN::QueryUserName		user_name;	// 用户帐号
			ADMIN::QueryCharacterName	char_name;	// 角色名称
		};

		REQ_KICK_USER()
			:TCP_MSG_BASE(CMD_REQ_KICK_USER),kickCondition(ADMIN::QC_INVALID)
		{}
	};

	//
	// 封停帐号
	//
	struct REQ_CLOSE_ACCOUNT : public ADMIN::TCP_MSG_BASE
	{
		// 封停原因
		enum CLOSE_ACCOUNT_REASON
		{
			R_UNKNOWN	= 0,
			R_SYSTEM,									// 系统封停
			R_SYSTEM_TEMP,								// 系统临时封停
			R_USERCUSTOM,								// 用户自助
			R_USERCUSTOM_TEMP,							// 用户临时自助
		};

		unsigned int			srv_realm_id;			// 大区id
		unsigned int			srv_cluster_id;			// 服务器组id
		unsigned int			duration_time;			// 封停时间(按分钟计算)
		CLOSE_ACCOUNT_REASON	eResult;				// 封停原因
		ADMIN::QUERY_CONDITION	closeCondition;			// 查询条件
		char					description[128 + 1];	// 描述

		union
		{
			ADMIN::QueryUserName		user_name;		// 用户帐号
			ADMIN::QueryCharacterName	char_name;		// 角色名称
		};

		REQ_CLOSE_ACCOUNT()
			:TCP_MSG_BASE(CMD_REQ_CLOSE_ACCOUNT),closeCondition(ADMIN::QC_INVALID)
		{}
	};

	//
	// 解封帐号
	//
	struct REQ_AVAILABLE_ACCOUNT : public ADMIN::TCP_MSG_BASE
	{
		unsigned int			srv_realm_id;			// 大区id
		unsigned int			srv_cluster_id;			// 服务器组id
		ADMIN::QUERY_CONDITION	availableCondition;		// 查询条件
		union
		{
			ADMIN::QueryUserName		user_name;		// 用户帐号
			ADMIN::QueryCharacterName	char_name;		// 角色名称
		};

		REQ_AVAILABLE_ACCOUNT()
			:TCP_MSG_BASE(CMD_REQ_AVAILABLE_ACCOUNT),availableCondition(ADMIN::QC_INVALID)
		{}
	};

	//
	// 系统广播(变成数据包,发送数据包时 数据包长度调用GetSize()获得)
	//
	struct REQ_SYSTEM_BROADCAST : public ADMIN::TCP_MSG_BASE
	{
		// 广播类型
		enum SYSTEM_BROADCAST_TYPE
		{
			T_INVALID = 0,
			T_NOW,			// 马上广播
			T_DELAY,		// 延迟广播
		};

		unsigned int			srv_realm_id;		// 大区id
		unsigned int			srv_cluster_id;		// 服务器组id
		SYSTEM_BROADCAST_TYPE	bType;				// 广播类型
		unsigned short			broadcastSize;		// 广播消息长度
		unsigned short			start_month;		// 开始广播时间(延迟广播用到)
		unsigned short			start_day;
		unsigned short			start_hour;
		unsigned short			start_minute;
		unsigned short			finish_month;		// 结束广播时间(延迟广播用到)
		unsigned short			finish_day;
		unsigned short			finish_hour;
		unsigned short			finish_minute;
		unsigned short			interval;			// 广播间隔(分钟)(延迟广播用到)
		char					broadcastTxt[512];	// 广播内容

		REQ_SYSTEM_BROADCAST()
			:TCP_MSG_BASE(CMD_REQ_SYSTEM_BROADCAST)
		{
			broadcastSize = (unsigned short)(sizeof(REQ_SYSTEM_BROADCAST) - 512);
		}

		// 添加广播消息内容
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
		unsigned int				srv_realm_id;	// 大区id
		unsigned int				srv_cluster_id;	// 服务器组id
		ADMIN::QueryCharacterName	char_name;		// 角色名称 
		ADMIN::ITEM_GROUP			itemGroup;		// 物品组
		unsigned short				itemIndex;		// 物品在物品组中的位置

		REQ_DELETE_ITEM() 
			: TCP_MSG_BASE(CMD_REQ_DELETE_ITEM)
		{}
	};

	struct REQ_MODIFY_CHARACTER : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// 大区id
		unsigned int				srv_cluster_id;	// 服务器组id
		ADMIN::QueryCharacterName	char_name;		// 角色名称 
		ADMIN::MOD_CHAR_ATTR		attr;			// 要修改的属性
		unsigned int				newValue;		// 修改为新的值

		REQ_MODIFY_CHARACTER()
			: TCP_MSG_BASE(CMD_REQ_MODIFY_CHARACTER)
		{}
	};

	struct REQ_MODIFY_CHARACTER_SKILL : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// 大区id
		unsigned int				srv_cluster_id;	// 服务器组id
		ADMIN::QueryCharacterName	char_name;		// 角色名称 
		unsigned short				skill_id;		// 技能ID
		unsigned short				skill_level;	// 技能等级

		REQ_MODIFY_CHARACTER_SKILL()
			: TCP_MSG_BASE(CMD_REQ_MODIFY_CHARACTER_SKILL)
		{}
	};

	struct REQ_RECOVER_CHARACTER : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// 大区id
		unsigned int				srv_cluster_id;	// 服务器组id
		ADMIN::QueryCharacterName	char_name;		// 角色名称 

		REQ_RECOVER_CHARACTER()
			: TCP_MSG_BASE(CMD_REQ_RECOVER_CHRACTER)
		{}
	};

	struct REQ_MODIFY_ACCOUNT_PASSW : public ADMIN::TCP_MSG_BASE
	{
		unsigned int			srv_realm_id;	// 大区id
		ADMIN::QueryUserName	user_name;		// 用户帐号
												// 登录密码
		char					new_password[ADMIN::MAX_PASSWORD_LENGTH];

		REQ_MODIFY_ACCOUNT_PASSW()
			: TCP_MSG_BASE(CMD_REQ_MODIFY_ACCOUNT_PASSW)
		{}
	};

	struct REQ_MAIL_ITEM : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// 大区id
		unsigned int				srv_cluster_id;	// 服务器组id
		ADMIN::QueryCharacterName	char_name;		// 角色名称
		unsigned int				item_type_id;	// 物品类型id
		unsigned char				item_count;		// 物品个数

		REQ_MAIL_ITEM()
			: TCP_MSG_BASE(CMD_REQ_MAIL_ITEM)
		{}
	};

	struct REQ_MODIFY_CHARACTER_NAME : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// 大区id
		unsigned int				srv_cluster_id;	// 服务器组id
		ADMIN::QueryCharacterName	char_name;		// 角色名称
		ADMIN::QueryCharacterName	new_char_name;	// 新角色名称

		REQ_MODIFY_CHARACTER_NAME()
			: TCP_MSG_BASE(CMD_REQ_MODIFY_CHARACTER_NAME)
		{}
	};

	//////////////////////////////////////////////////////////////////////////
	// 日志类

	struct REQ_USER_LOG_BASE : public ADMIN::TCP_MSG_BASE
	{
		unsigned int			srv_realm_id;		// 大区id
		unsigned int			srv_cluster_id;		// 服务器组id
		ADMIN::DATE_TIME		startTime;			// 开始时间
		ADMIN::DATE_TIME		endTime;			// 结束时间
		ADMIN::QUERY_CONDITION	availableCondition;	// 查询条件
		union
		{
			ADMIN::QueryUserName user_name;			// 用户帐号
			ADMIN::QueryCharacterName char_name;	// 角色名称
		};

		REQ_USER_LOG_BASE(unsigned short id)
			: TCP_MSG_BASE(id)
		{}
	};

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

	// 日志类
	//////////////////////////////////////////////////////////////////////////

}; // namespace MSG_ATAS


#pragma pack(pop)