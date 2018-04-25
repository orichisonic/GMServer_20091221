#pragma once

#include "AdminDef.h"

#pragma pack(push, 1)

namespace MSG_ASTA
{
	const int CMD_REP_LOGIN					= ADMIN::MSG_BASE_ASTA + 1;		// 登录结果
	const int CMD_REP_CHARACTER_BASE_INFO	= ADMIN::MSG_BASE_ASTA + 2;		// 查询角色基础信息结果
	const int CMD_REP_CHARACTER_ITEMS_INFO	= ADMIN::MSG_BASE_ASTA + 3;		// 查询角色物品信息结果
	const int CMD_REP_CLOSURE_ACCOUNT_INFO	= ADMIN::MSG_BASE_ASTA + 4;		// 查询封停帐号信候结果
	const int CMD_REP_CHARACTER_INFO_FAIL	= ADMIN::MSG_BASE_ASTA + 5;		// 查询角色信息失败

	const int CMD_REP_KICK_USER				= ADMIN::MSG_BASE_ASTA + 100;	// 踢人结果
	const int CMD_REP_CLOSE_ACCOUNT			= ADMIN::MSG_BASE_ASTA + 101;	// 封停结果
	const int CMD_REP_AVAILABLE_ACCOUNT		= ADMIN::MSG_BASE_ASTA + 102;	// 解封结果
	const int CMD_REP_SYSTEM_BROADCAST		= ADMIN::MSG_BASE_ASTA + 103;	// 系统广播结果
	const int CMD_REP_NAME_ID_LIST			= ADMIN::MSG_BASE_ASTA + 104;	// 各种查询返回角色的名字和ID
	const int CMD_REP_DELETE_ITEM			= ADMIN::MSG_BASE_ASTA + 105;	// 删除物品结果
	const int CMD_REP_CHARACTER_ITEM		= ADMIN::MSG_BASE_ASTA + 106;	// 查询角色物品结果
	const int CMD_REP_CHARACTER_FRIEND		= ADMIN::MSG_BASE_ASTA + 107;	// 查询角色好友结果
	const int CMD_REP_CHARACTER_MEDAL		= ADMIN::MSG_BASE_ASTA + 108;	// 查询角色徽章信息结果(暂是废弃)
	const int CMD_REP_CHARACTER_PET			= ADMIN::MSG_BASE_ASTA + 109;	// 查询角色宠物信息结果
	const int CMD_REP_MODIFY_CHARACTER		= ADMIN::MSG_BASE_ASTA + 110;	// 修改角色属性结果(改钱,改名,改等级)
	const int CMD_REP_MODIFY_CHARACTER_SKILL= ADMIN::MSG_BASE_ASTA + 111;	// 修改角色技能结果
	const int CMD_REP_RECOVER_CHARACTER		= ADMIN::MSG_BASE_ASTA + 112;	// 恢复角色结果
	const int CMD_REP_MODIFY_ACCOUNT_PASSW	= ADMIN::MSG_BASE_ASTA + 113;	// 修改登录密码
	const int CMD_REP_MAIL_ITEM				= ADMIN::MSG_BASE_ASTA + 114;	// 结角色邮寄物品结果
	const int CMD_REP_MODIFY_CHARACTER_NAME	= ADMIN::MSG_BASE_ASTA + 115;	// 修改角色名
	const int CMD_REP_CHARACTER_SKILL		= ADMIN::MSG_BASE_ASTA + 116;	// 角色技能信息

	// 日志查询
	const int CMD_REP_LOG_MONSTER_DROP_ITEM	= ADMIN::MSG_BASE_ASTA + 250;	// 物品:怪物掉落
	const int CMD_REP_LOG_BUY_ITEM_FROM_NPC	= ADMIN::MSG_BASE_ASTA + 251;	// 物品:从NPC处购买
	const int CMD_REP_LOG_SELL_ITEM_TO_NPC	= ADMIN::MSG_BASE_ASTA + 252;	// 物品:卖给NPC
	const int CMD_REP_LOG_DROP_ITEM			= ADMIN::MSG_BASE_ASTA + 253;	// 物品:玩家自行删除
	const int CMD_REP_LOG_GET_ITEM_TRADE	= ADMIN::MSG_BASE_ASTA + 254;	// 物品:交易获得
	const int CMD_REP_LOG_LOSE_ITEM_TRADE	= ADMIN::MSG_BASE_ASTA + 255;	// 物品:交易失去
	const int CMD_REP_LOG_BIND_ITEM			= ADMIN::MSG_BASE_ASTA + 256;	// 物品:装备绑定记录
	const int CMD_REP_LOG_UNBIND_ITEM		= ADMIN::MSG_BASE_ASTA + 257;	// 物品:装备解绑定记录
	const int CMD_REP_LOG_BOOTH_ITEM		= ADMIN::MSG_BASE_ASTA + 258;	// 物品:角色“摆摊”日志查询
	const int CMD_REP_LOG_DELETE_ITEM		= ADMIN::MSG_BASE_ASTA + 259;	// 物品:道具删除记录(指使用时间到了)
	const int CMD_REP_LOG_USE_ITEM			= ADMIN::MSG_BASE_ASTA + 260;	// 物品:道具使用记录
	const int CMD_REP_LOG_CRAFT_ITEM		= ADMIN::MSG_BASE_ASTA + 261;	// 物品:装备制作记录
	const int CMD_REP_LOG_PUNCH_ITEM		= ADMIN::MSG_BASE_ASTA + 262;	// 物品:装备打孔
	const int CMD_REP_LOG_CHANGE_SLOT_ITEM	= ADMIN::MSG_BASE_ASTA + 263;	// 物品:装备改孔
	const int CMD_REP_LOG_UPGRADE_STAR		= ADMIN::MSG_BASE_ASTA + 264;	// 物品:装备升星
	const int CMD_REP_LOG_GEM				= ADMIN::MSG_BASE_ASTA + 265;	// 物品:宝石
	const int CMD_REP_LOG_PHARMA			= ADMIN::MSG_BASE_ASTA + 266;	// 物品:制药记录
	const int CMD_REP_LOG_CRAFT_GEM			= ADMIN::MSG_BASE_ASTA + 267;	// 物品:宝石合成
	const int CMD_REP_LOG_CRAFT_MATIRAIL	= ADMIN::MSG_BASE_ASTA + 268;	// 物品:材料合成
	const int CMD_REP_LOG_CRAFT_RECIPE		= ADMIN::MSG_BASE_ASTA + 269;	// 物品:图纸合成
	const int CMD_REP_LOG_MAZE_INTEGRAL		= ADMIN::MSG_BASE_ASTA + 270;	// 迷宫积分兑换奖励
	const int CMD_REP_LOG_MONSTER_DROP_MONEY= ADMIN::MSG_BASE_ASTA + 271;	// 游戏币:怪物掉落
	const int CMD_REP_LOG_GET_MONEY_QUEST	= ADMIN::MSG_BASE_ASTA + 272;	// 游戏币:任务获得
	const int CMD_REP_LOG_LOSE_MONEY_REPAIR	= ADMIN::MSG_BASE_ASTA + 273;	// 游戏币:修装
	const int CMD_REP_LOG_GET_MONEY_TRADE	= ADMIN::MSG_BASE_ASTA + 274;	// 游戏币:交易获得
	const int CMD_REP_LOG_LOSE_MONEY_TRADE	= ADMIN::MSG_BASE_ASTA + 275;	// 游戏币:交易失去
	const int CMD_REP_LOG_QUEST_REWARD		= ADMIN::MSG_BASE_ASTA + 276;	// 任务:完成获得记录
	const int CMD_REP_LOG_QUEST_GIVE		= ADMIN::MSG_BASE_ASTA + 277;	// 任务:任务物品删除记录
	const int CMD_REP_LOG_MAIL_SEND			= ADMIN::MSG_BASE_ASTA + 278;	// 邮件:发送记录
	const int CMD_REP_LOG_MAIL_RECV			= ADMIN::MSG_BASE_ASTA + 279;	// 邮件:接收记录
	const int CMD_REP_LOG_MAIL_DELETE		= ADMIN::MSG_BASE_ASTA + 280;	// 邮件:删除记录
	const int CMD_REP_LOG_ITEM_SHOP			= ADMIN::MSG_BASE_ASTA + 281;	// 商城:购买记录
	const int CMD_REP_LOG_INSTANCE			= ADMIN::MSG_BASE_ASTA + 282;	// 查询用户的副本日志
	const int CMD_REP_LOG_LOGIN_LOGOUT		= ADMIN::MSG_BASE_ASTA + 283;	// 玩家登陆/登出日志

	// 
	// 返回登陆结果
	// 
	struct REP_LOGIN : public ADMIN::TCP_MSG_BASE
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
	struct REP_CHARACTER_BASE_INFO : public ADMIN::TCP_MSG_BASE
	{
		unsigned int	srv_realm_id;	// 大区id
		unsigned int	srv_cluster_id;	// 服务器组id
		char			user_name[ADMIN::MAX_ID_LENGTH]; // 帐号
		unsigned int	char_id;		// 角色id
		char			char_name[ADMIN::MAX_CHARACTER_NAME_LENGTH];		// 角色名称
		char			char_title1[ADMIN::MAX_CHARACTER_NAME_LENGTH];	// 角色称谓1
		char			char_title2[ADMIN::MAX_CHARACTER_NAME_LENGTH];	// 角色称谓2
		unsigned char	char_type;			// 角色类型(职业性别等)
		bool			is_deleted;			// 是否被删除
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
	// 返回角色信息查询失败
	//
	struct REP_CHARACTER_INFO_FAIL : public ADMIN::TCP_MSG_BASE
	{
		enum QUERY_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,		// 查询封停信息成功
			E_FAILED_ACCOUNT_NOTEXIST,					// 帐号不存在
			E_FAILED_SERVERINTERNALERROR,				// 服务器内部错误
		};

		// 查询类型
		enum REQ_TYPE
		{
			RT_INVALID			= 0,
			RT_CHAR_BASE_INFO	= 1,					// 角色基本信息
			RT_CHAR_ITEM_INFO	= 2,					// 角色物品信息
			RT_CHAR_BANK_INFO	= 3,					// 角色银行信息
			RT_CHAR_MEDAL_INFO	= 4,					// 角色徽章信息(暂时废弃)
			RT_CHAR_PET_INFO	= 5,					// 角色宠物信息
			RT_CHAR_FRIEND_INFO	= 6,					// 角色好友信息
			RT_CHAR_SKILL_INFO	= 7,					// 角色技能信息
		};

		QUERY_RESULT					eResult;		// 查询结果
		unsigned int					srv_realm_id;	// 大区id
		unsigned int					srv_cluster_id;	// 服务器组id
		REQ_TYPE						req_type;		// 查询类型
		ADMIN::QUERY_CONDITION			query_condition;// 查询条件
		union
		{
			ADMIN::QueryUserName		user_name;		// 用户帐号
			ADMIN::QueryCharacterName	char_name;		// 角色名称
		};

		REP_CHARACTER_INFO_FAIL()
			: TCP_MSG_BASE(CMD_REP_CHARACTER_INFO_FAIL)
		{}
	};

	//
	// 返回帐号信息
	//
	struct REP_CLOSURE_ACCOUNT_INFO : public ADMIN::TCP_MSG_BASE
	{
		enum QUERY_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// 查询封停信息成功
			E_FAILED_ACCOUNT_NOTEXIST,				// 帐号不存在
			E_FAILED_SERVERINTERNALERROR,			// 服务器内部错误
		};

		QUERY_RESULT			eResult;			// 结果
		unsigned int			srv_realm_id;		// 大区id
		unsigned int			accountIndex;		// 帐号ID
		unsigned int			closure_reason;		// 封停原因
		bool					is_clousure;		// 是否封停
		ADMIN::QueryUserName	user_name;			// 帐号
		ADMIN::DATE_TIME		last_logout_time;	// 上次登录时间
		ADMIN::DATE_TIME		enable_time;		// 封停结束时间

		REP_CLOSURE_ACCOUNT_INFO()
			:TCP_MSG_BASE(CMD_REP_CLOSURE_ACCOUNT_INFO),eResult(E_FAILED_UNKNOWNERROR)
		{}
	};

	//
	// 返回踢人结果
	//
	struct REP_KICK_USER : public ADMIN::TCP_MSG_BASE
	{
		enum KICK_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// 踢人成功
			E_FAILED_CHAR_NOTINGAME,				// 帐号没有角色在游戏中
			E_FAILED_SERVERINTERNALERROR,			// 服务器内部错误
		};

		KICK_RESULT				eResult;			// 踢人结果
		unsigned int			srv_realm_id;		// 大区id
		unsigned int			srv_cluster_id;		// 服务器组id
		ADMIN::QUERY_CONDITION	query_condition;	// 查询条件
		union
		{
			ADMIN::QueryUserName		user_name;	// 用户帐号
			ADMIN::QueryCharacterName	char_name;	// 角色名称
		};

		REP_KICK_USER()
			:TCP_MSG_BASE(CMD_REP_KICK_USER)
		{}
	};

	//
	// 封停帐号结果
	//
	struct REP_CLOSE_ACCOUNT : public ADMIN::TCP_MSG_BASE
	{
		enum CLOSE_ACCOUNT_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,		// 封停成功
			E_FAILED_ACCOUNT_NOTEXIST,					// 帐号不存在
			E_FAILED_SERVERINTERNALERROR,				// 服务器内部错误
		};

		CLOSE_ACCOUNT_RESULT			eResult;		// 封停结果
		unsigned int					srv_realm_id;	// 大区id
		unsigned int					srv_cluster_id;	// 服务器组id
		ADMIN::DATE_TIME				enable_time;	// 封停结束时间
		ADMIN::QUERY_CONDITION			query_condition;// 查询条件
		union
		{
			ADMIN::QueryUserName		user_name;		// 用户帐号
			ADMIN::QueryCharacterName	char_name;		// 角色名称
		};

		REP_CLOSE_ACCOUNT()
			:TCP_MSG_BASE(CMD_REP_CLOSE_ACCOUNT)
		{}
	};

	//
	// 解封帐号结果
	//
	struct REP_AVAILABLE_ACCOUNT : public ADMIN::TCP_MSG_BASE
	{
		enum OP_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,			// 解封成功
			E_FAILED_ACCOUNT_NOTEXIST,						// 帐号不存在
			E_FAILED_SERVERINTERNALERROR,					// 服务器内部错误
		};

		OP_RESULT						eResult;			// 解封结果
		unsigned int					srv_realm_id;		// 大区id
		unsigned int					srv_cluster_id;		// 服务器组id
		ADMIN::QUERY_CONDITION			availableCondition;	// 查询条件
		union
		{
			ADMIN::QueryUserName		user_name;			// 用户帐号
			ADMIN::QueryCharacterName	char_name;			// 角色名称
		};

		REP_AVAILABLE_ACCOUNT()
			:TCP_MSG_BASE(CMD_REP_AVAILABLE_ACCOUNT)
		{
			user_name.userName[0] = '\0';
		}
	};

	//
	// 系统广播消息结果
	// 
	struct REP_SYSTEM_BROADCAST : public ADMIN::TCP_MSG_BASE
	{
		enum SYSTEM_BROADCAST_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// 广播成功
			E_FAILED_SERVERINTERNALERROR,			// 服务器内部错误
		};

		SYSTEM_BROADCAST_RESULT eResult;			// 广播结果

		REP_SYSTEM_BROADCAST()
			:TCP_MSG_BASE(CMD_REP_SYSTEM_BROADCAST)
		{}
	};

	//
	// 名字ID列表
	//
	struct REP_NAME_ID_LIST_HEADER : public ADMIN::TCP_MSG_BASE
	{
		// 返回结果
		enum NAME_ID_LIST_HEADER_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// 查询成功
			E_FAILED_NOTEXIST,						// 角色或帐号不存在
			E_FAILED_SERVERINTERNALERROR,			// 服务器内部错误
		};

		NAME_ID_LIST_HEADER_RESULT	eResult;		// 结果
		ADMIN::QUERY_CONDITION		query_condition;// 查询条件
		bool						allSend;		// 是否发送完成
		unsigned int				param1;			// 两个参数(备用)
		unsigned int				param2;			// 两个参数(备用)

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
			}user_name; // 用户帐号

			struct  
			{
				unsigned int charIdx;
				char charName[ADMIN::MAX_CHARACTER_NAME_LENGTH];
				bool isDeleted;
			}char_name; // 角色名称 
		};
	};

	typedef ADMIN::VarLenStruct<REP_NAME_ID_LIST_HEADER, NAME_ID_ELEM, 20>
		REP_NAME_ID_LIST;

	struct REP_DELETE_ITEM : public ADMIN::TCP_MSG_BASE
	{
		enum DELETE_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// 成功
			E_FAILED_SERVERINTERNALERROR,			// 服务器内部错误
			E_NOT_FOUND,
		};

		DELETE_RESULT				eResult;		// 删除结果
		unsigned int				srv_realm_id;	// 大区id
		unsigned int				srv_cluster_id;	// 服务器组id
		ADMIN::QueryCharacterName	char_name;		// 角色名
		ADMIN::ITEM_GROUP			itemGroup;		// 物品所在组
		unsigned short				itemIndex;		// 物品在组中的位置

		REP_DELETE_ITEM()
			: TCP_MSG_BASE(CMD_REP_DELETE_ITEM)
		{}
	};

	// 角色物品列表
	struct CHARACTER_ITEM_INFO_HEADER : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// 大区id
		unsigned int				srv_cluster_id;	// 服务器组id
		ADMIN::QueryCharacterName	char_name;		// 角色名
		ADMIN::ITEM_GROUP			itemGroup;		// 所查询的物品组
		bool						allSend;		// 是否发送完成

		CHARACTER_ITEM_INFO_HEADER()
			: TCP_MSG_BASE(CMD_REP_CHARACTER_ITEM)
		{}
	};

	struct CHARACTER_ITEM_INFO_ITEM
	{
		unsigned short		itemIndex;			// 序号(在组中的位置)
		unsigned int		itemTypeId;			// 类型ID
												// 物品名字
		char				itemName[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64	itemUniqueId;		// 物品唯一ID
		unsigned char		itemCount;			// 数量
		unsigned short		itemDur;			// 耐久
	};

	typedef ADMIN::VarLenStruct<CHARACTER_ITEM_INFO_HEADER, CHARACTER_ITEM_INFO_ITEM, 20>
		CHARACTER_ITEM_INFO;

	// 角色好友列表
	struct CHARACTER_FRIEND_INFO_HEADER : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// 大区id
		unsigned int				srv_cluster_id;	// 服务器组id
		ADMIN::QueryCharacterName	char_name;		// 角色名
		bool						allSend;		// 是否发送完成

		CHARACTER_FRIEND_INFO_HEADER()
			: TCP_MSG_BASE(CMD_REP_CHARACTER_FRIEND)
		{}
	};

	struct CHARACTER_FRIEND_INFO_ITEM
	{
		unsigned int				friendIndex;	// 好友角色ID
		ADMIN::QueryCharacterName	friend_name;	// 好友名字
		unsigned char				friendTypeId;	// 好友的角色类型
		unsigned char				friendLevel;	// 好友等级
		unsigned short				friendIni;		// 新密度
	};

	typedef ADMIN::VarLenStruct<CHARACTER_FRIEND_INFO_HEADER, CHARACTER_FRIEND_INFO_ITEM, 20>
		CHARACTER_FRIEND_INFO;

	// 角色徽章列表(暂时废弃)
	struct CHARACTER_MEDAL_INFO_HEADER : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// 大区id
		unsigned int				srv_cluster_id;	// 服务器组id
		ADMIN::QueryCharacterName	char_name;		// 角色名
		bool						allSend;		// 是否发送完成

		CHARACTER_MEDAL_INFO_HEADER()
			: TCP_MSG_BASE(CMD_REP_CHARACTER_MEDAL)
		{}
	};

	struct CHARACTER_MEDAL_INFO_ITEM
	{
		unsigned int	medalTypeId;	// 物品类型ID
										// 物品名
		char			medalName[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned short	medalDur;		// 耐久
		float			medalBringUp;	// 培差度
		unsigned char	medalPetIndex;	// 宠物序号超所值255为无宠物
		unsigned char	medalPos;		// 徽章在宠物背包中位置 255为装备中
	};

	typedef ADMIN::VarLenStruct<CHARACTER_MEDAL_INFO_HEADER, CHARACTER_MEDAL_INFO_ITEM, 9>
		CHARACTER_MEDAL_INFO;

	// 角色宠物列表
	struct CHARACTER_PET_INFO_HEADER : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// 大区id
		unsigned int				srv_cluster_id;	// 服务器组id
		ADMIN::QueryCharacterName	char_name;		// 角色名
		bool						allSend;		// 是否发送完成

		CHARACTER_PET_INFO_HEADER()
			: TCP_MSG_BASE(CMD_REP_CHARACTER_PET)
		{}
	};

	struct CHARACTER_PET_INFO_ITEM
	{
		unsigned int	petTypeId;			// 类型ID
											// 宠物名字
		char			petName[ADMIN::MAX_CHARACTER_NAME_LENGTH];
		unsigned char	petLevel;			// 等级
		unsigned char	petNature;			// 性格
		unsigned short	petGrowth;			// 成长率
		unsigned int	healthe;			// 健康度
		unsigned short	physical_attack;	// 物理攻击
		unsigned short	physical_defense;	// 物理防御
		unsigned short	magic_attack;		// 魔法攻击
		unsigned short	magic_defense;		// 魔法防御
		unsigned short	fire_attack;		// 火焰攻击
		unsigned short	cold_attack;		// 冰冻攻击
		unsigned short	poison_attack;		// 毒素攻击
		float			fire_resist;		// 火焰抗性
		float			cold_resist;		// 冰冻抗性
		float			poison_resist;		// 毒素抗性
	};

	typedef ADMIN::VarLenStruct<CHARACTER_PET_INFO_HEADER, CHARACTER_PET_INFO_ITEM, 8>
		CHARACTER_PET_INFO;

	struct REP_MODIFY_CHARACTER : public ADMIN::TCP_MSG_BASE
	{
		enum MODIFY_RESUL
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// 成功
			E_FAILED_SERVERINTERNALERROR,			// 服务器内部错误
			E_NOT_FOUND,
		};

		MODIFY_RESUL				eResult;		// 结果
		unsigned int				srv_realm_id;	// 大区id
		unsigned int				srv_cluster_id;	// 服务器组id
		ADMIN::QueryCharacterName	char_name;		// 角色名称 
		ADMIN::MOD_CHAR_ATTR		attr;			// 要修改的属性
		unsigned int				newValue;		// 新的值

		REP_MODIFY_CHARACTER()
			: TCP_MSG_BASE(CMD_REP_MODIFY_CHARACTER)
		{}
	};

	struct REP_MODIFY_CHARACTER_SKILL : public ADMIN::TCP_MSG_BASE
	{
		enum MODIFY_CHARACTER_SKILL_RESUL
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,		// 成功
			E_FAILED_SERVERINTERNALERROR,				// 服务器内部错误
			E_NOT_FOUND,
		};

		MODIFY_CHARACTER_SKILL_RESUL	eResult;		// 结果
		unsigned int					srv_realm_id;	// 大区id
		unsigned int					srv_cluster_id;	// 服务器组id
		ADMIN::QueryCharacterName		char_name;		// 角色名称 
		unsigned short					skill_id;		// 技能ID
		unsigned short					skill_level;	// 技能等级

		REP_MODIFY_CHARACTER_SKILL()
			: TCP_MSG_BASE(CMD_REP_MODIFY_CHARACTER_SKILL)
		{}
	};

	struct REP_RECOVER_CHARACTER : public ADMIN::TCP_MSG_BASE
	{
		enum RECOVER_RESUL
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,	// 成功
			E_FAILED_SERVERINTERNALERROR,			// 服务器内部错误
			E_NOT_FOUND,
		};

		RECOVER_RESUL				eResult;		// 结果
		unsigned int				srv_realm_id;	// 大区id
		unsigned int				srv_cluster_id;	// 服务器组id
		ADMIN::QueryCharacterName	char_name;		// 角色名称 

		REP_RECOVER_CHARACTER()
			: TCP_MSG_BASE(CMD_REP_RECOVER_CHARACTER)
		{}
	};

	struct REP_MODIFY_ACCOUNT_PASSW : public ADMIN::TCP_MSG_BASE
	{
		enum MODIFY_PASSW_RESUL
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,// 成功
			E_FAILED_SERVERINTERNALERROR,		// 服务器内部错误
			E_NOT_FOUND,
		};

		MODIFY_PASSW_RESUL		eResult;		// 结果
		unsigned int			srv_realm_id;	// 大区id
		ADMIN::QueryUserName	user_name;		// 用户帐号

		REP_MODIFY_ACCOUNT_PASSW()
			: TCP_MSG_BASE(CMD_REP_MODIFY_ACCOUNT_PASSW)
		{}
	};
		
	struct REP_MAIL_ITEM : public ADMIN::TCP_MSG_BASE
	{
		enum MAIL_ITEM_RESUL
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,// 成功
			E_FAILED_SERVERINTERNALERROR,		// 服务器内部错误
			E_NOT_FOUND,
		};

		MAIL_ITEM_RESUL				eResult;		// 结果
		unsigned int				srv_realm_id;	// 大区id
		unsigned int				srv_cluster_id;	// 服务器组id
		ADMIN::QueryCharacterName	char_name;		// 角色名称
		unsigned int				item_type_id;	// 物品类型id
		unsigned char				item_count;		// 物品个数

		REP_MAIL_ITEM()
			: TCP_MSG_BASE(CMD_REP_MAIL_ITEM)
		{}
	};

	struct REP_MODIFY_CHARACTER_NAME : public ADMIN::TCP_MSG_BASE
	{
		enum MODIFY_CHARACTER_NAME_RESULT
		{
			E_FAILED_UNKNOWNERROR			= 0,
			E_SUCCESS						= 1,// 成功
			E_FAILED_SERVERINTERNALERROR,		// 服务器内部错误
			E_NOT_FOUND,
			E_ERROR_NAME,						// 不合法的角色名
			E_REPEAT_NAME,						// 重角色名
		};

		MODIFY_CHARACTER_NAME_RESULT	eResult;		// 结果
		unsigned int					srv_realm_id;	// 大区id
		unsigned int					srv_cluster_id;	// 服务器组id

		REP_MODIFY_CHARACTER_NAME()
			: TCP_MSG_BASE(CMD_REP_MODIFY_CHARACTER_NAME)
		{}
	};

	struct REP_CHARACTER_SKILL_INFO_HEADER : public ADMIN::TCP_MSG_BASE
	{
		unsigned int				srv_realm_id;	// 大区id
		unsigned int				srv_cluster_id;	// 服务器组id
		ADMIN::QueryCharacterName	char_name;		// 角色名
		bool						allSend;		// 是否发送完成

		REP_CHARACTER_SKILL_INFO_HEADER()
			: TCP_MSG_BASE(CMD_REP_CHARACTER_SKILL)
		{}
	};

	struct CHARACTER_SKILL_INFO_ITEM
	{
		unsigned int		skillId;							// 技能ID
		unsigned char		skillLevel;							// 技能等级 0级表示还未学
		char				skillName[ADMIN::MAX_ID_LENGTH];	// 技能名字
	};

	typedef ADMIN::VarLenStruct<REP_CHARACTER_SKILL_INFO_HEADER, CHARACTER_SKILL_INFO_ITEM, 20>
		REP_CHARACTER_SKILL_INFO;
				
	//////////////////////////////////////////////////////////////////////////
	//

	struct REP_USER_LOG_BASE : public ADMIN::TCP_MSG_BASE
	{
		unsigned int					srv_realm_id;		// 大区id
		unsigned int					srv_cluster_id;		// 服务器组id
		bool							all_send;			// 是否发送完成
		ADMIN::DATE_TIME				start_time;			// 开始时间
		ADMIN::DATE_TIME				end_time;			// 结束时间
		ADMIN::QUERY_CONDITION			query_condition;	// 查询条件
		union
		{
			ADMIN::QueryUserName		user_name;			// 用户帐号
			ADMIN::QueryCharacterName	char_name;			// 角色名称
		};

		REP_USER_LOG_BASE(unsigned short id)
			: TCP_MSG_BASE(id)
		{}
	};

	//////////////////////////////////////////////////////////////////////////
	// 怪物掉落
	struct REP_LOG_MONSTER_DROP_ITEM_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;			// 角色ID
		ADMIN::QueryCharacterName	char_name;			// 角色名字

		REP_LOG_MONSTER_DROP_ITEM_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_MONSTER_DROP_ITEM)
		{}
	};

	struct REP_LOG_MONSTER_DROP_ITEM_ELE
	{
		ADMIN::DATE_TIME			date_time;			// 获得时间
		unsigned int				item_type_id;		// 物品类型ID
														// 物品名
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64			item_unique_id;		// 物品唯一ID
		unsigned char				item_count;			// 数量
	};

	typedef ADMIN::VarLenStruct<REP_LOG_MONSTER_DROP_ITEM_HEADER, REP_LOG_MONSTER_DROP_ITEM_ELE, 20>
		REP_LOG_MONSTER_DROP_ITEM;

	//////////////////////////////////////////////////////////////////////////
	// 从NPC处购买
	struct REP_LOG_BUY_ITEM_FROM_NPC_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;			// 角色ID
		ADMIN::QueryCharacterName	char_name;			// 角色名字

		REP_LOG_BUY_ITEM_FROM_NPC_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_BUY_ITEM_FROM_NPC)
		{}
	};

	struct REP_LOG_BUY_ITEM_FROM_NPC_ELE
	{
		ADMIN::DATE_TIME			date_time;			// 买进时间
		unsigned int				item_type_id;		// 物品类型ID
														// 物品名字
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];		
		unsigned __int64			item_unique_id;		// 物品唯一ID
		unsigned char				item_count;			// 数量
		unsigned int				cost_money;			// 消耗游戏币数量
	};

	typedef ADMIN::VarLenStruct<REP_LOG_BUY_ITEM_FROM_NPC_HEADER, REP_LOG_BUY_ITEM_FROM_NPC_ELE, 20>
		REP_LOG_BUY_ITEM_FROM_NPC;

	//////////////////////////////////////////////////////////////////////////
	// 卖给NPC
	struct REP_LOG_SELL_ITEM_TO_NPC_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;			// 角色ID
		ADMIN::QueryCharacterName	char_name;			// 角色名字

		REP_LOG_SELL_ITEM_TO_NPC_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_SELL_ITEM_TO_NPC)
		{}
	};

	struct REP_LOG_SELL_ITEM_TO_NPC_ELE
	{
		ADMIN::DATE_TIME			date_time;			// 卖出时间
		unsigned int				item_type_id;		// 物品类型ID
														// 物品名字
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64			item_unique_id;		// 物品唯一ID
		unsigned char				item_count;			// 数量
		unsigned int				getMoney;			// 得到游戏币数量
	};

	typedef ADMIN::VarLenStruct<REP_LOG_SELL_ITEM_TO_NPC_HEADER, REP_LOG_SELL_ITEM_TO_NPC_ELE, 20>
		REP_LOG_SELL_ITEM_TO_NPC;

	//////////////////////////////////////////////////////////////////////////
	// 玩家自行删除
	struct REP_LOG_DROP_ITEM_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;			// 角色ID
		ADMIN::QueryCharacterName	char_name;			// 角色名字

		REP_LOG_DROP_ITEM_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_DROP_ITEM)
		{}
	};

	struct REP_LOG_DROP_ITEM_ELE
	{
		ADMIN::DATE_TIME			date_time;			// 删除时间
		unsigned int				item_type_id;		// 物品类型ID
														// 物品名字
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64			item_unique_id;		// 物品唯一ID
		unsigned char				item_count;			// 数量
	};

	typedef ADMIN::VarLenStruct<REP_LOG_DROP_ITEM_HEADER, REP_LOG_DROP_ITEM_ELE, 20>
		REP_LOG_DROP_ITEM;

	//////////////////////////////////////////////////////////////////////////
	// 交易获得
	struct REP_LOG_GET_ITEM_TRADE_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		dst_char_id;	// 物品目标角色ID
		ADMIN::QueryCharacterName	dst_char_name;	// 物品目标角色名

		REP_LOG_GET_ITEM_TRADE_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_GET_ITEM_TRADE)
		{}
	};

	struct REP_LOG_GET_ITEM_TRADE_ELE
	{
		ADMIN::QueryCharacterId		src_char_id;	// 来源角色ID
		ADMIN::QueryCharacterName	src_char_name;	// 来源角色名字
		ADMIN::DATE_TIME			date_time;		// 交易时间
		unsigned int				item_type_id;	// 物品类型ID
													// 物品名字
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64			item_unique_id;	// 物品唯一ID
		unsigned char				item_count;		// 数量
	};

	typedef ADMIN::VarLenStruct<REP_LOG_GET_ITEM_TRADE_HEADER, REP_LOG_GET_ITEM_TRADE_ELE, 20>
		REP_LOG_GET_ITEM_TRADE;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_LOSE_ITEM_TRADE_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		src_char_id;	// 角色ID
		ADMIN::QueryCharacterName	src_char_name;	// 角色名字

		REP_LOG_LOSE_ITEM_TRADE_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_LOSE_ITEM_TRADE)
		{}
	};

	struct REP_LOG_LOSE_ITEM_TRADE_ELE
	{
		ADMIN::QueryCharacterId		dst_char_id;	// 物品目标角色ID
		ADMIN::QueryCharacterName	dst_char_name;	// 物品目标角色名
		ADMIN::DATE_TIME			date_time;		// 交易时间
		unsigned int				item_type_id;	// 物品类型ID
													// 物品名字
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64			item_unique_id;	// 物品唯一ID
		unsigned char				item_count;		// 数量
	};

	typedef ADMIN::VarLenStruct<REP_LOG_LOSE_ITEM_TRADE_HEADER, REP_LOG_LOSE_ITEM_TRADE_ELE, 20>
		REP_LOG_LOSE_ITEM_TRADE;

	//////////////////////////////////////////////////////////////////////////
	// 装备绑定记录
	struct REP_LOG_BIND_ITEM_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;				// 角色ID
		ADMIN::QueryCharacterName	char_name;				// 角色名字

		REP_LOG_BIND_ITEM_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_BIND_ITEM)
		{}
	};

	struct REP_LOG_BIND_ITEM_ELE
	{
		ADMIN::DATE_TIME			date_time;				// 绑定时间
		unsigned int				item_type_id;			// 物品类型ID
															// 物品名字
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64			item_unique_id;			// 物品唯一ID
		unsigned char				bind_reward[2];			// 绑定奖励
		unsigned int				bind_reward_value[2];	// 绑定奖励数值
	};

	typedef ADMIN::VarLenStruct<REP_LOG_BIND_ITEM_HEADER, REP_LOG_BIND_ITEM_ELE, 20>
		REP_LOG_BIND_ITEM;

	//////////////////////////////////////////////////////////////////////////
	// 装备解绑定记录
	struct REP_LOG_UNBIND_ITEM_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;				// 角色ID
		ADMIN::QueryCharacterName	char_name;				// 角色名字

		REP_LOG_UNBIND_ITEM_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_UNBIND_ITEM)
		{}
	};

	struct REP_LOG_UNBIND_ITEM_ELE
	{
		ADMIN::DATE_TIME			date_time;				// 解绑定时间
		unsigned int				item_type_id;			// 物品类型ID
															// 物品名字
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64			item_unique_id;			// 物品唯一ID
		unsigned char				bind_reward[2];			// 绑定奖励
		unsigned int				bind_reward_value[2];	// 绑定奖励数值
		unsigned int				consume_item_type_id;	// 解绑消耗物品ID
	};

	typedef ADMIN::VarLenStruct<REP_LOG_UNBIND_ITEM_HEADER, REP_LOG_UNBIND_ITEM_ELE, 20>
		REP_LOG_UNBIND_ITEM;

	//////////////////////////////////////////////////////////////////////////
	// 摆摊日志
	struct REP_LOG_BOOTH_ITEM_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;				// 角色ID
		ADMIN::QueryCharacterName	char_name;				// 角色名字

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
		ADMIN::QueryCharacterId		dst_char_id;	// 目标角色ID
		ADMIN::QueryCharacterName	dst_char_name;	// 目标角色名
		unsigned int				item_type_id;	// 物品类型ID
													// 物品名字
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned char				item_count;		// 数量
		unsigned int				item_price;		// 价格
	};

	struct BOOTH_LOG_BUY
	{
		ADMIN::QueryCharacterId		src_char_id;	// 来源角色ID角色ID
		ADMIN::QueryCharacterName	src_char_name;	// 来源角色ID角色名
		unsigned int				item_type_id;	// 物品类型ID
													// 物品名字
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned char				item_count;		// 数量
		unsigned int				item_price;		// 价格
	};

	struct REP_LOG_BOOTH_ITEM_ELE
	{
		enum BOOTH_LOG_TYPE
		{
			BOOTH_OPEN,						// 开摆
			BOOTH_SELL,						// 摊主卖出
			BOOTH_BUY,						// 摊主买进
			BOOTH_CLOSE,					// 收摊
		};

		BOOTH_LOG_TYPE		booth_type;		// 操作类型
		ADMIN::DATE_TIME	date_time;		// 操作时间
		union
		{
			BOOTH_LOG_OPEN	booth_open;		// 开摆
			BOOTH_LOG_CLOSE	booth_close;	// 摊主卖出
			BOOTH_LOG_SELL	booth_sell;		// 摊主买进
			BOOTH_LOG_BUY	booth_buy;		// 收摊
		};
	};

	typedef ADMIN::VarLenStruct<REP_LOG_BOOTH_ITEM_HEADER, REP_LOG_BOOTH_ITEM_ELE, 20>
		REP_LOG_BOOTH_ITEM;

	//////////////////////////////////////////////////////////////////////////
	// 删除物品
	struct REP_LOG_DELETE_ITEM_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_DELETE_ITEM_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_DELETE_ITEM)
		{}
	};

	struct REP_LOG_DELETE_ITEM_ELE
	{
		ADMIN::DATE_TIME			date_time;		// 删除时间
		unsigned int				item_type_id;	// 删除物品类型ID
													// 物品名字
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64			item_unique_id;	// 物品唯一ID
		unsigned int				item_time_limit;// 物品的使用期限
	};

	typedef ADMIN::VarLenStruct<REP_LOG_DELETE_ITEM_HEADER, REP_LOG_DELETE_ITEM_ELE, 20>
		REP_LOG_DELETE_ITEM;

	//////////////////////////////////////////////////////////////////////////
	// 使用物品
	struct REP_LOG_USE_ITEM_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_USE_ITEM_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_USE_ITEM)
		{}
	};

	struct REP_LOG_USE_ITEM_ELE
	{
		ADMIN::DATE_TIME			date_time;		// 使用时间
		unsigned int				item_type_id;	// 使用物品类型ID
													// 使用物品名字
		char						item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64			item_unique_id;	// 使用物品唯一ID
		unsigned char				item_count;		// 物品的使用数量
	};

	typedef ADMIN::VarLenStruct<REP_LOG_USE_ITEM_HEADER, REP_LOG_USE_ITEM_ELE, 20>
		REP_LOG_USE_ITEM;

	//////////////////////////////////////////////////////////////////////////
	// 装备制作
	struct REP_LOG_CRAFT_ITEM_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_CRAFT_ITEM_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_CRAFT_ITEM)
		{}
	};

	struct REP_LOG_CRAFT_ITEM_ELE
	{
		enum CRAFT_TYPE
		{
			CRAFT_BLACKSMITH,					// 打造
			CRAFT_SEWING,						// 裁缝
			CRAFT_HANDCRAFT,					// 手工
		};

		ADMIN::DATE_TIME	date_time;			// 制作时间
		CRAFT_TYPE			craft_type;			// 装备制作类型
		unsigned int		recipe_id;			// 图纸ID
		unsigned int		material_id[6];		// 材料ID
		unsigned int		product_id;			// 产品ID
												// 产品名字
		char				product_name[ADMIN::MAX_ITEM_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_CRAFT_ITEM_HEADER, REP_LOG_CRAFT_ITEM_ELE, 20>
		REP_LOG_CRAFT_ITEM;

	//////////////////////////////////////////////////////////////////////////
	// 打孔
	struct REP_LOG_PUNCH_ITEM_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_PUNCH_ITEM_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_PUNCH_ITEM)
		{}
	};

	struct REP_LOG_PUNCH_ITEM_ELE
	{
		enum CONSUME_HAMMER_LEVEL
		{
			LOW_LEVEL_HAMMER,						// 初级铁锤
			MIDDLE_LEVEL_HAMMER,					// 中级铁锤
			HIGH_LEVEL_HAMMER,						// 高级铁锤
		};

		enum CONSUME_STITHY_LEVEL
		{
			LOW_LEVEL_STITHY,						// 初级铁砧
			MIDDLE_LEVEL_STITHY,					// 中级铁砧
			HIGH_LEVEL_STITHY,						// 高级铁砧
		};

		ADMIN::DATE_TIME		date_time;			// 打孔时间
		CONSUME_HAMMER_LEVEL	consume_hammer;		// 消耗铁锤
		CONSUME_STITHY_LEVEL	consume_stithy;		// 消耗铁砧
		unsigned int			consume_money;		// 消耗金钱
		unsigned int			item_type_id;		// 物品类型ID
													// 物品名字
		char					item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64		item_unique_id;		// 物品唯一ID
	};

	typedef ADMIN::VarLenStruct<REP_LOG_PUNCH_ITEM_HEADER, REP_LOG_PUNCH_ITEM_ELE, 20>
		REP_LOG_PUNCH_ITEM;

	//////////////////////////////////////////////////////////////////////////
	// 改孔
	struct REP_LOG_CHANGE_SLOT_ITEM_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_CHANGE_SLOT_ITEM_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_CHANGE_SLOT_ITEM)
		{}
	};

	struct REP_LOG_CHANGE_SLOT_ITEM_ELE
	{
		ADMIN::DATE_TIME		date_time;			// 改孔时间
		ADMIN::GEM_COLOR		old_color;			// 改孔前颜色
		ADMIN::GEM_COLOR		new_color;			// 改孔后颜色
		unsigned int			consume_money;		// 消耗金钱
		unsigned int			item_type_id;		// 物品类型ID
													// 物品名字
		char					item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64		item_unique_id;		// 物品唯一ID
	};

	typedef ADMIN::VarLenStruct<REP_LOG_CHANGE_SLOT_ITEM_HEADER, REP_LOG_CHANGE_SLOT_ITEM_ELE, 20>
		REP_LOG_CHANGE_SLOT_ITEM;

	//////////////////////////////////////////////////////////////////////////
	// 升星
	struct REP_LOG_UPGRADE_STAR_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_UPGRADE_STAR_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_UPGRADE_STAR)
		{}
	};

	struct REP_LOG_UPGRADE_STAR_ELE
	{
		ADMIN::DATE_TIME		date_time;			// 改孔时间
		unsigned char			old_star_count;		// 升星前星数
		unsigned char			new_star_count;		// 升星后星数
		unsigned int			consume_money;		// 消耗金钱
		unsigned int			item_type_id;		// 物品类型ID
													// 物品名字
		char					item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64		item_unique_id;		// 物品唯一ID
	};

	typedef ADMIN::VarLenStruct<REP_LOG_UPGRADE_STAR_HEADER, REP_LOG_UPGRADE_STAR_ELE, 20>
		REP_LOG_UPGRADE_STAR;

	//////////////////////////////////////////////////////////////////////////
	// 宝石
	struct REP_LOG_GEM_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_GEM_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_GEM)
		{}
	};

	struct REP_LOG_GEM_ELE
	{
		enum GEM_OPERATION
		{
			GEM_ADD,								// 镶嵌
			GEM_REMOVE,								// 去除
		};

		GEM_OPERATION			gem_operation;		// 操作
		ADMIN::DATE_TIME		date_time;			// 改孔时间
		ADMIN::GEM_COLOR		gem_type;			// 宝石类型
		unsigned char			gem_level;			// 宝石等级
		unsigned int			consume_money;		// 消耗金钱
		unsigned int			item_type_id;		// 物品类型ID
													// 物品名字
		char					item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned __int64		item_unique_id;		// 物品唯一ID
	};

	typedef ADMIN::VarLenStruct<REP_LOG_GEM_HEADER, REP_LOG_GEM_ELE, 20>
		REP_LOG_GEM;

	//////////////////////////////////////////////////////////////////////////
	// 制药
	struct REP_LOG_PHARMA_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_PHARMA_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_PHARMA)
		{}
	};

	struct REP_LOG_PHARMA_ELE
	{
		ADMIN::DATE_TIME	date_time;				// 制作时间
		unsigned int		recipe_id;				// 图纸ID
		unsigned int		material_id[6];			// 材料ID
		unsigned int		product_id;				// 产品ID
													// 产品名字
		char				product_name[ADMIN::MAX_ITEM_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_PHARMA_HEADER, REP_LOG_PHARMA_ELE, 20>
		REP_LOG_PHARMA;

	//////////////////////////////////////////////////////////////////////////
	// 宝石合成
	struct REP_LOG_CRAFT_GEM_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_CRAFT_GEM_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_CRAFT_GEM)
		{}
	};

	struct REP_LOG_CRAFT_GEM_ELE
	{
		ADMIN::DATE_TIME	date_time;				// 合成时间
		unsigned int		consume_money;			// 消耗金钱
		unsigned int		prop_item_id;			// 成功率道具
		unsigned int		material_id;			// 原材料ID
													// 原材料名字
		char				material_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned int		product_id;				// 成品ID
		char				product_name[ADMIN::MAX_ITEM_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_CRAFT_GEM_HEADER, REP_LOG_CRAFT_GEM_ELE, 20>
		REP_LOG_CRAFT_GEM;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_CRAFT_MATIRAIL_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_CRAFT_MATIRAIL_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_CRAFT_MATIRAIL)
		{}
	};

	struct REP_LOG_CRAFT_MATIRAIL_ELE
	{
		ADMIN::DATE_TIME	date_time;				// 合成时间
		unsigned int		consume_money;			// 消耗金钱
		unsigned int		prop_item_id;			// 成功率道具
		unsigned int		material_id;			// 原材料ID
													// 原材料名字
		char				material_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned int		product_id;				// 成品ID
		char				product_name[ADMIN::MAX_ITEM_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_CRAFT_MATIRAIL_HEADER, REP_LOG_CRAFT_MATIRAIL_ELE, 20>
		REP_LOG_CRAFT_MATIRAIL;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_CRAFT_RECIPE_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_CRAFT_RECIPE_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_CRAFT_RECIPE)
		{}
	};

	struct REP_LOG_CRAFT_RECIPE_ELE
	{
		ADMIN::DATE_TIME	date_time;				// 合成时间
		unsigned int		consume_money;			// 消耗金钱
		unsigned int		prop_item_id;			// 成功率道具
		unsigned int		material_id;			// 原材料ID
													// 原材料名字
		char				material_name[ADMIN::MAX_ITEM_NAME_LENGTH];
		unsigned int		product_id;				// 成品ID
		char				product_name[ADMIN::MAX_ITEM_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_CRAFT_RECIPE_HEADER, REP_LOG_CRAFT_RECIPE_ELE, 20>
		REP_LOG_CRAFT_RECIPE;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_MAZE_INTEGRAL_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_MAZE_INTEGRAL_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_MAZE_INTEGRAL)
		{}
	};

	struct REP_LOG_MAZE_INTEGERAL_ELE
	{
		enum CHANGE_TYPE
		{
			CHANGE_EXP,							// 兑换经验
			CHANGE_MONEY,						// 兑换金钱
		};

		ADMIN::DATE_TIME	date_time;			// 兑换时间
		CHANGE_TYPE			change_type;		// 兑换类型
		unsigned int		consume_integral;	// 消耗积分
		unsigned int		change_point;		// 得到的经验或是钱
	};

	typedef ADMIN::VarLenStruct<REP_LOG_MAZE_INTEGRAL_HEADER, REP_LOG_MAZE_INTEGERAL_ELE, 20>
		REP_LOG_MAZE_INTEGRAL;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_MONSTER_DROP_MONEY_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_MONSTER_DROP_MONEY_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_MONSTER_DROP_MONEY)
		{}
	};

	struct REP_LOG_MONSTER_DROP_MONEY_ELE
	{
		ADMIN::DATE_TIME	date_time;			// 获得时间
		unsigned int		get_money;			// 获得金钱数量
		unsigned int		src_id;				// 来源ID
												// 来源名
		char				src_name[ADMIN::MAX_CHARACTER_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_MONSTER_DROP_MONEY_HEADER, REP_LOG_MONSTER_DROP_MONEY_ELE, 20>
		REP_LOG_MONSTER_DROP_MONEY;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_GET_MONEY_QUEST_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_GET_MONEY_QUEST_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_GET_MONEY_QUEST)
		{}
	};

	struct REP_LOG_GET_MONEY_QUEST_ELE
	{
		ADMIN::DATE_TIME	date_time;			// 获得时间
		unsigned int		get_money;			// 获得金钱数量
		unsigned short		src_id;				// 来源ID
												// 来源名
		char				src_name[ADMIN::MAX_QUEST_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_GET_MONEY_QUEST_HEADER, REP_LOG_GET_MONEY_QUEST_ELE, 20>
		REP_LOG_GET_MONEY_QUEST;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_LOSE_MONEY_REPAIR_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_LOSE_MONEY_REPAIR_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_LOSE_MONEY_REPAIR)
		{}
	};

	struct REP_LOG_LOSE_MONEY_REPAIR_ELE
	{
		ADMIN::DATE_TIME	date_time;			// 时间
		unsigned int		consume_money;		// 消耗金钱
		unsigned int		item_type_id;		// 物品类型
												// 物品名
		char				item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_LOSE_MONEY_REPAIR_HEADER, REP_LOG_LOSE_MONEY_REPAIR_ELE, 20>
		REP_LOG_LOSE_MONEY_REPAIR;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_GET_MONEY_TRADE_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_GET_MONEY_TRADE_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_GET_MONEY_TRADE)
		{}
	};

	struct REP_LOG_GET_MONEY_TRADE_ELE
	{
		ADMIN::DATE_TIME	date_time;				// 时间
		unsigned int		get_money;				// 金钱数量
		unsigned int		src_id;					// 来源ID
													// 来源名字
		char				src_name[ADMIN::MAX_CHARACTER_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_GET_MONEY_TRADE_HEADER, REP_LOG_GET_MONEY_TRADE_ELE, 20>
		REP_LOG_GET_MONEY_TRADE;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_LOSE_MONEY_TRADE_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_LOSE_MONEY_TRADE_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_LOSE_MONEY_TRADE)
		{}
	};

	struct REP_LOG_LOSE_MONEY_TRADE_ELE
	{
		ADMIN::DATE_TIME	date_time;				// 时间
		unsigned int		lose_money;				// 金钱数量
		unsigned int		dst_id;					// 来源ID
													// 来源名字
		char				dst_name[ADMIN::MAX_CHARACTER_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_LOSE_MONEY_TRADE_HEADER, REP_LOG_LOSE_MONEY_TRADE_ELE, 20>
		REP_LOG_LOSE_MONEY_TRADE;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_QUEST_REWARD_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_QUEST_REWARD_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_QUEST_REWARD)
		{}
	};

	struct REP_LOG_QUEST_REWARD_ELE
	{
		ADMIN::DATE_TIME	date_time;				// 时间
		unsigned short		quest_id;				// 任务ID
		unsigned int		reward_money;			// 奖励的金钱
													// 任务名字
		char				quest_name[ADMIN::MAX_QUEST_NAME_LENGTH];
		unsigned int		reward_item_id;			// 奖励的物品
													// 奖励的物品名字
		char				reward_item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_QUEST_REWARD_HEADER, REP_LOG_QUEST_REWARD_ELE, 20>
		REP_LOG_QUEST_REWARD;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_QUEST_GIVE_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_QUEST_GIVE_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_QUEST_GIVE)
		{}
	};

	struct REP_LOG_QUEST_GIVE_ELE
	{
		ADMIN::DATE_TIME	date_time;				// 时间
		unsigned short		quest_id;				// 任务ID
													// 任务名字
		char				quest_name[ADMIN::MAX_QUEST_NAME_LENGTH];
		unsigned int		give_item_id;			// 失去的物品ID
													// 失去的物品名字
		char				give_item_name[ADMIN::MAX_ITEM_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_QUEST_GIVE_HEADER, REP_LOG_QUEST_GIVE_ELE, 20>
		REP_LOG_QUEST_GIVE;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_MAIL_SEND_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_MAIL_SEND_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_MAIL_SEND)
		{}
	};

	struct REP_LOG_MAIL_SEND_ELE
	{
		ADMIN::DATE_TIME			date_time;			// 时间
		unsigned __int64			mail_id;			// 邮件ID
		unsigned int				mail_money;			// 金钱数量
		unsigned int				dst_id;				// 收件人ID
														// 收件人名字
		char						dst_name[ADMIN::MAX_CHARACTER_NAME_LENGTH];
		unsigned char				mail_item_count[5];	// 物品个数
														// 物品名
		char						mail_item[5][ADMIN::MAX_ITEM_NAME_LENGTH];
														// 邮件标题
		char						mail_title[ADMIN::MAX_MAIL_TITLE];
		
	};

	typedef ADMIN::VarLenStruct<REP_LOG_MAIL_SEND_HEADER, REP_LOG_MAIL_SEND_ELE, 20>
		REP_LOG_MAIL_SEND;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_MAIL_RECV_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_MAIL_RECV_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_MAIL_RECV)
		{}
	};

	struct REP_LOG_MAIL_RECV_ELE
	{
		ADMIN::DATE_TIME			date_time;			// 时间
		unsigned __int64			mail_id;			// 邮件ID
		unsigned int				mail_money;			// 金钱数量
		unsigned int				src_id;				// 发件人ID
														// 发件人名字
		char						src_name[ADMIN::MAX_CHARACTER_NAME_LENGTH];
		unsigned char				mail_item_count[5];	// 物品个数
														// 物品名
		char						mail_item[5][ADMIN::MAX_ITEM_NAME_LENGTH];
														// 邮件标题
		char						mail_title[ADMIN::MAX_MAIL_TITLE];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_MAIL_RECV_HEADER, REP_LOG_MAIL_RECV_ELE, 20>
		REP_LOG_MAIL_RECV;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_MAIL_DELETE_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_MAIL_DELETE_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_MAIL_DELETE)
		{}
	};

	struct REP_LOG_MAIL_DELETE_ELE
	{
		ADMIN::DATE_TIME			date_time;			// 时间
		unsigned __int64			mail_id;			// 邮件ID
		unsigned int				src_id;				// 发件人ID
														// 发件人名字
		char						src_name[ADMIN::MAX_CHARACTER_NAME_LENGTH];
														// 邮件标题
		char						mail_title[ADMIN::MAX_MAIL_TITLE];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_MAIL_DELETE_HEADER, REP_LOG_MAIL_DELETE_ELE, 20>
		REP_LOG_MAIL_DELETE;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_ITEM_SHOP_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_ITEM_SHOP_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_ITEM_SHOP)
		{}
	};

	struct REP_LOG_ITEM_SHOP_ELE
	{
		ADMIN::DATE_TIME		date_time;			// 时入时间
		unsigned int			item_type_id;		// 物品类型
		char					item_name[ADMIN::MAX_ITEM_NAME_LENGTH];	
		unsigned __int64		item_unique_id;		// 物品唯一ID
		unsigned char			item_count;			// 物品个数
		unsigned int			comsume_m_money;	// 消耗M币
		unsigned int			rest_m_money;		// 剩余M币
	};

	typedef ADMIN::VarLenStruct<REP_LOG_ITEM_SHOP_HEADER, REP_LOG_ITEM_SHOP_ELE, 20>
		REP_LOG_ITEM_SHOP;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_INSTANCE_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_INSTANCE_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_INSTANCE)
		{}
	};

	struct REP_LOG_INSTANCE_ELE
	{
		enum INSTANCE_OPERATION
		{
			INSTANCE_ENTER,							// 进入副本
			INSTANCE_LEAVE,							// 离开副本
		};

		ADMIN::DATE_TIME		date_time;			// 时入时间
		INSTANCE_OPERATION		op_type;			// 类型
		unsigned short			instance_id;		// 副本ID
													// 副本名字
		char					instance_name[ADMIN::MAX_SCENE_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_INSTANCE_HEADER, REP_LOG_INSTANCE_ELE, 20>
		REP_LOG_INSTANCE;

	//////////////////////////////////////////////////////////////////////////
	struct REP_LOG_LOGIN_LOGOUT_HEADER : public REP_USER_LOG_BASE
	{
		ADMIN::QueryCharacterId		char_id;		// 角色ID
		ADMIN::QueryCharacterName	char_name;		// 角色名字

		REP_LOG_LOGIN_LOGOUT_HEADER()
			: REP_USER_LOG_BASE(CMD_REP_LOG_LOGIN_LOGOUT)
		{}
	};

	struct REP_LOG_LOGIN_LOGOUT_ELE
	{
		enum LOGIN_LOGOUT_OPEARTION
		{
			LOGIN_CHAR,								// 角色登录
			LOGOUT_CHAR,							// 角色登出
		};

		LOGIN_LOGOUT_OPEARTION		op_type;		// 类型
		ADMIN::DATE_TIME			date_time;		// 时入时间
		unsigned int				char_id;		// 角色ID
													// 角色名字
		char						char_name[ADMIN::MAX_CHARACTER_NAME_LENGTH];
	};

	typedef ADMIN::VarLenStruct<REP_LOG_LOGIN_LOGOUT_HEADER, REP_LOG_LOGIN_LOGOUT_ELE, 20>
		REP_LOG_LOGIN_LOGOUT;

	//
	//////////////////////////////////////////////////////////////////////////

}; // namespace MSG_ASTA


#pragma pack(pop)
