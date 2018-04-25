/******************************************************************************
						GM Tools与GM Server通信协议

全局常量、数据结构
create by LiaoHuaifu 2008/9/2

二期功能: 2008/10/18
*******************************************************************************/

#ifndef GMS_GLOBAL_DEFINE_H
#define GMS_GLOBAL_DEFINE_H
#pragma once
#pragma pack(push,1)

//#include "..\SDBase\TypeDef.h"
//#include "..\SDBase\Item\ItemDef.h"

namespace GMSBase
{
	typedef unsigned char		ui8;
	typedef unsigned short	ui16;
	typedef unsigned int		ui32;
	typedef char				i8;
	typedef short				i16;
	typedef int				i32;
	typedef float				real;

	//
	//	来自 oledb.h -> struct DBTIMESTAMP
	//	定义此结构的目的是不必包含 oledb.h 在头文件内
	//
 	typedef struct _tagSQLSVR_DATETIME
 	{
 		i16		year;
 		ui16	month;
 		ui16	day;
 		ui16	hour;
 		ui16	minute;
 		ui16	second;
 
 		ui32	fraction;
 
 	} SQLSVR_DATETIME;

	//
	// 最大值定义
	//
	enum
	{
		MAX_CHARACTER_NUM 	    = 3,			//  一个帐号可创建的最多角色数量	
		MAX_ACTIVE_CODE_LENGTH  = 6,			//  激活码最大长度
		MAX_CHAR_NAME_LENGTH    = 20,			//	角色名最大长度（DB字段长度）
		MAX_ACCOUNT_LENGTH	    = 32,			//  帐号名最大长度（DB字段长度）
		MAX_PASSWORD_LENGTH     = 32,           //  密码最大长度（DB字段长度）
		MAX_POP_MSG_LENGTH	    = 80,			//  公告消息最大长度
		MAX_PUB_NAME_LENGTH		= 16,			//  俱乐部名字最大长度

	};

	//
	// GMTools 请求命令类型
	//
	typedef enum
	{
		OP_INVALID = 0,        // 空
		
		// 一期功能
		OP_LOGIN		=   5,	// 登录GMS 
		OP_PLAYER_INFO  =  10, 	// 查看用户的基本信息
		OP_CHAR_INFO    =  11, 	// 查看玩家角色的基本信息
		OP_KICK  	    =  20,  // 踢人
		OP_BAN 	 	    =  30,  // 封禁
		OP_RELEASE      =  31,	// 解封
		OP_POP   	    =  40,	// 公告	
		OP_EXP_RATE		=  50,	// 修改经验值	add 2008/9/26
		OP_SCORE_RATE	=  51, // 修改积分倍数

		// 二期功能
		OP_ACCOUNT_ACTIVE_SEARCH	= 60,	// 帐号激活查询 
		OP_ITEM_SEARCH				= 70,	// 物品查询
		OP_CONSUME_RECORD_SEARCH	= 80,	// 消费记录查询
		OP_ACCOUNT_LOGIN_LOGOUT_LOG	= 90,	// 帐号登陆日志
		OP_SKILL_SEARCH				= 100,	// 技能查询		
		OP_CHARACTER_INFO_OPERATION	= 110,	// 角色操作（修改、删除、恢复）

		// 三期功能
		OP_FRIEND_SEARCH = 112,				// 好友查询
		OP_LOG_SEARCH = 114,				// 玩家日志信息查询
		OP_PRASENT_ITEM = 116,				// 赠送道具

		OP_ACTIVE_CAMPAIGN = 150,			// 活动管理
		
		OP_NUM,

	}CMD_TYPE;

	//
	// GM Server 发送数据类型
	//
	typedef enum
	{
		RE_INVALID = 0,        // 空
		
		// 一期功能
		RE_LOGIN   = 5,			// 返回登录信息
		RE_PLAYER_INFO  =  10, 	// 查看用户的基本信息
		RE_CHAR_INFO    =  11, 	// 查看玩家角色的基本信息
		RE_KICK  	    =  20,	// 踢人
		RE_BAN 	 	    =  30,	// 封禁
		RE_RELEASE      =  31,	// 解封
		RE_POP   	    =  40,	// 公告
		RE_EXP_RATE		=  50,	// 修改经验值	add 2008/9/26
		RE_SCORE_RATE		=  51,	// 修改积分值

		// 二期功能
		RE_ACCOUNT_ACTIVE_SEARCH	= 60,	// 帐号激活查询 
		RE_ITEM_SEARCH				= 70,	// 物品查询
		RE_CONSUME_RECORD_SEARCH	= 80,	// 消费记录查询
		RE_ACCOUNT_LOGIN_LOGOUT_LOG	= 90,	// 帐号登陆日志
		RE_SKILL_SEARCH				= 100,	// 技能查询
		RE_CHARACTER_INFO_OPERATION	= 110,	// 角色操作（修改、删除、恢复、 加GB、修改等级）

		// 三期功能
		RE_FRIEND_SEARCH = 112,				// 好友查询
		RE_LOG_SEARCH = 114,				// 玩家日志信息查询
		RE_PRASENT_ITEM = 116,				// 赠送道具

		RE_ACTIVE_CAMPAIGN = 150,			// 活动管理

		RE_NUM,

	}RES_TYPE;

	typedef enum
	{
		ERR_INVALID	= 0,
		
		ERR_LOGIN_1 = 5,				// 帐号不存在
		ERR_LOGIN_2 = 6,				// 密码错误
		ERR_LOGIN_3 = 7,				// 已经登录
		ERR_LOGIN_4 = 8,				// GMTools数量超出上限  30人	
		ERR_LOGIN_5 = 9,				// 注册登录失败	

		ERR_PLAYER_INFO  =  10,			// 查看用户的基本信息失败(未知原因)
		ERR_CHAR_INFO    =  11, 		// 查看玩家角色的基本信息失败(未知原因)

		ERR_KICK_1		 =   20,		// 踢人失败(未知原因)
		ERR_KICK_2		 =   21,		// 踢人失败(要踢的人不在线)

		ERR_BAN_1       =   30,			// 封禁失败 (未知原因)
		ERR_BAN_2       =   31,  		// 封禁失败 (要封禁的人不在线),这个改成 “不存在该玩家”

		ERR_RELEASE_1	=   32,			//不存在该玩家
		ERR_RELEASE_2	=   33,			//该玩家未被封禁
		ERR_RELEASE_3	=   34,			//解封失败(未知原因)

		ERR_POP 		 =   40,	     // 公告失败 (未知原因)

		ERR_ACCOUNT_ACTIVE_SEARCH	= 60,	// 激活错误

		ERR_NUM,

		ERR_UNKNOW					=   ERR_NUM + 1, // 失败未知原因(通用)		
		ERR_ACCOUNT_NOT_EXIST		=   ERR_NUM + 2, // 帐号不存在(通用)
		ERR_CHARACTER_NOT_EXIST		=   ERR_NUM + 3, // 角色不存在(通用)
		ERR_CHANNEL_NOT_EXIST		=   ERR_NUM + 3, // 不存在频道 

		ERR_CHARNAME_CAN_NULL		=	ERR_NUM	+ 4,	// 角色名不能为空
	}ERROR_CODE;

 	// 赛季序号
 	typedef enum 					
 	{
 		HSNFT_LAST,					//	上一赛季排行
 		HSNFT_CURRENT,				//	当前赛季排行
 		HSNFT_TOTAL,				//	总排行
 		HSNFT_COUNT,
 	}HSNFT_ENUM;

 	typedef union
 	{
 		struct  
 		{						
 			ui16	typeid_3;		//	分类内
 			ui8		typeid_2;		//	次分类
 			ui8		typeid_1;		//	主分类
 		}	sep_val;
 		ui32	com_val;
 
 	}ITEM_ID;

	//
	// 角色基本数据结构
	//
	typedef struct _tagCHARACTER_BASIC_INFO
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];			// 角色名称
		SQLSVR_DATETIME tDeleteDate;						// 删除日期
		ui8		nFigure;									// 体型
		ui8		nPosition;									// 位置
		ui16	nLevel;										// 等级
		ui32	nExp;										// 当前经验
		char 	szClubName[MAX_CHAR_NAME_LENGTH];			// 所属俱乐部
		ui8		nClubPosition;								// 俱乐部职位
		ui32	nGB;										// 当前游戏币 
		//ui32	nMB;										// 当前MB
		//ui8		nIsOnline;									// 是否在线

		void CHARACTER_BASIC_INFO_GMS()
		{
			memset(this,0,sizeof(_tagCHARACTER_BASIC_INFO));
		}
	}CHARACTER_BASIC_INFO_GMS,*PCHARACTER_BASIC_INFO_GMS;

	//
	//	 角色能力数据
	//
	typedef struct _tagCHARACTER_ABILITY
	{
		i16		nRun;		//	跑动 
		i16		nJump;		//	弹跳 
		i16		nStrength;	//	力量 
		i16		nPass;		//	传球 
		i16		nDribble;	//	控球 
		i16		nBlock;		//	盖帽 
		i16		nRebound;	//	篮板 
		i16		nSteal;		//	断球 

		i16		nClose;		//	近投 
		i16		nShot;		//	近投 
		i16		nLongshot;	//	远投 
		i16		nDrive;		//	扣篮

		void CHARACTER_ABILITY_GMS()
		{
			memset(this,0,sizeof(_tagCHARACTER_ABILITY));
		}
	}CHARACTER_ABILITY_GMS,*PCHARACTER_ABILITY_GMS;


	typedef struct _tagSEASON_STAT
    {
		ui32	nScore;			//	[存储]得分
		float	fScore_pt;		//	[计算]场均得分
		ui32	nThree_point;	//	[计算]三分球
		float   fTotal_ratio;   //  [计算]总命中率
		float   fThree_ratio;   //  [计算]3分命中率
		ui32	nAssist;		//	[存储]助攻
		float   fAssist_pt;     //  [计算]场均助攻
       ui32		nRebound;		//	[存储]篮板
		float   fRebound_pt;   //  [计算]场均篮板
       ui32		nBlock;			//	[存储]盖帽
 		float   fBlock_pt;     //  [计算]场均盖帽
       ui32		nSteal;			//	[存储]断球	
		float   fSteal_pt;     //  [计算]场均抢断
		ui32    nScore_pure;    //  [存储]净胜分
		ui32    nScore_1v1;     //  [存储]1v1得分
		ui32    nPlay_time;     //  [存储]比赛时间
		ui32    nMvp_times;     //  [存储]POG次数

		ui32	nOrder_score;
		ui32	nOrder_score_pt;
		ui32	nOrder_three_point;
		ui32	nOrder_total_ratio;
		ui32	nOrder_three_ratio;
		ui32	nOrder_assist;
		ui32	nOrder_assist_pt;
		ui32	nOrder_rebound;
		ui32	nOrder_rebound_pt;
		ui32	nOrder_block;
		ui32	nOrder_block_pt;
		ui32	nOrder_steal;
		ui32	nOrder_steal_pt;
		ui32	nOrder_score_pure;
		ui32	nOrder_score_1v1;
		ui32	nOrder_play_time;
		ui32	nOrder_mvp_times;

		ui32	order_score_upordown;
		ui32	order_score_pt_upordown;
		ui32	order_three_point_upordown;
		ui32	order_total_ratio_upordown;
		ui32	order_three_ratio_upordown;
		ui32	order_assist_upordown;
		ui32	order_assist_pt_upordown;
		ui32	order_rebound_upordown;
		ui32	order_rebound_pt_upordown;
		ui32	order_block_upordown;
		ui32	order_block_pt_upordown;
		ui32	order_steal_upordown;
		ui32	order_steal_pt_upordown;
		ui32	order_score_pure_upordown;
		ui32	order_score_1v1_upordown;
		ui32	order_play_time_upordown;
		ui32	order_mvp_times_upordown;

		ui32	nTotal_shoot;	 //	[存储]总投球
		ui32	nTotal_hit;		 //	[存储]总命中
		ui32	nThree_shoot;	 //	[存储]3分投球
		ui32	nThree_hit;	    //	[存储]3分命中
		ui32    nPlay_times;    //  [存储]比赛次数

		void SEASON_STAT_GMS()
		{
			memset( this, 0, sizeof( _tagSEASON_STAT ) );
		}
   }SEASON_STAT_GMS;


	/*********************************** 二期功能 ***********************************/
	struct PLAYER_UNIQUE_INFO 
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];		 // 付费者角色名称
		char 	szAccountName[MAX_ACCOUNT_LENGTH];       // 付费者帐号名称	

		PLAYER_UNIQUE_INFO()
		{
			memset(this,0,sizeof(PLAYER_UNIQUE_INFO));			
		}
	};

	//玩家记录数据结构
	struct PLAYER_CHARACTER_BASE_INFO 
	{
		PLAYER_UNIQUE_INFO	playInfo;	// 帐号、角色名
		ui8		nFigure;				// 体型
		ui8		nPosition;				// 位置
		ui16	nLevel;					// 等级
		char	szPubName[MAX_PUB_NAME_LENGTH];	//所属俱乐部
		ui32	nPubPosition;			// 俱乐部职位
		ui32	nGB;					// G币
		//ui32	nMB;					// M币

		PLAYER_CHARACTER_BASE_INFO()
		{
			memset(this,0,sizeof(PLAYER_CHARACTER_BASE_INFO));				
		}
	};

	//消费记录数据结构
	struct CONSUME_RECORD_INFO 
	{
		SQLSVR_DATETIME	tGetDateTime;	// 获得时间 
		ui16	nType;	 // 类型
		ui32	nItemID; // 物品ID			
		char	szItemName[MAX_CHAR_NAME_LENGTH];	// 物品名称
		ui32	nGB;	 // G币
		ui32	nMB;	 // M币
		char	szPayer[MAX_CHAR_NAME_LENGTH];		// 付费者
		char	szReceiver[MAX_CHAR_NAME_LENGTH];	// 接收者

		CONSUME_RECORD_INFO()
		{
			memset(this,0,sizeof(CONSUME_RECORD_INFO));	
		}
	};

	//登陆日志记录数据结构
	struct	LOGIN_OUT_INFO 
	{
		ui32	id;									// id
		char	szCharName[MAX_CHAR_NAME_LENGTH];	// 角色名称
		ui32	login_server_id;					// 大区id
		ui32	game_server_id;						// 频道id
		char	user_ip[15];						// 登录ip
		ui8		login_out;							// 登陆(0)/登出(1)
		SQLSVR_DATETIME	lgoin_out_time;				// 时间

		LOGIN_OUT_INFO()
		{
			memset(this,0,sizeof(LOGIN_OUT_INFO));	
		}
	};

	//技能记录数据结构
	struct SKIN_RECORD_INFO 
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];	// 角色名称
		ui16	SkillID;				// 技能ID
		char	szSkillName[MAX_CHAR_NAME_LENGTH];	// 技能名称
		ui16	nWorkType;				// 职业
		ui8		nSkillType;				// 分类
		ui8		nSkillLevel;			// 技能级别
		SQLSVR_DATETIME	tBuyTime;	    // 购买时间
		SQLSVR_DATETIME	tFirstUseTime;  // 第一次使用时间 
		bool	bEquip;					// 是否装备

		SKIN_RECORD_INFO()
		{
			memset(this,0,sizeof(SKIN_RECORD_INFO));	
		}
	};

	//物品记录数据结构
	struct GOODS_INFO 
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];	// 角色名称
		ITEM_ID	GoodsID;				// 物品ID
		ui16	nType;					// 物品类型
		char	szGoodsName[MAX_CHAR_NAME_LENGTH];	// 物品名
		ui8		nLevel;					// 物品级别
		bool	bEquip;					// 是否装备
		
		SQLSVR_DATETIME	tLastTime;	    // 过期时间

		struct
		{
			ui8		index;				//	属性变更项ID	
			i16		value;				//	属性变更项值	

		}PA_Item[8];//物品的固定属性

		struct 
		{
			ui8		index;		//	属性变更项ID	tinyint
			i16		value;		//	属性变更项值	smallint
			i16		mb;			//	属性价格_MB		smallint
			i16		gb;			//	属性价格_GB		smallint

		}APA_Item[6];//可购买的属性一

		struct
		{
			ui8		index;		//	属性变更项ID	tinyint
			i16		value;		//	属性变更项值	smallint
			i16		mb;			//	属性价格_MB		smallint
			i16		gb;			//	属性价格_GB		smallint

		}EPA_Item[6];//可购买的属性二

		GOODS_INFO()
		{
			memset(this,0,sizeof(GOODS_INFO));	
		}
	};

	// 好友信息数据结构
	struct FRIEND_INFO
	{
		char szFriendName[MAX_CHAR_NAME_LENGTH];	// 好友昵称
		ui8  nFriendStatus;							// 好友状态（是否在线）
	};
	
	// 升级信息
	struct UPLEVEL_LOG 
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];		 // 角色名称
		char 	szAccountName[MAX_ACCOUNT_LENGTH];       // 帐号名称	
		ui32	nPlayerID;									// 角色ID
		ui8		nLevel;									//等级
		SQLSVR_DATETIME upLevelTime;					//升级时间
	};

	//
	//	物品对象数据结构 (GMtools 使用)
	//
	typedef struct _tagITEM_OBJ
	{
		ITEM_ID		type_id;
		i32			scheme_selected;	//	物品色彩方案		int

		ui32		time_expired;		//	32bit time_t , 过期时间	datetime; 0 表示为不会过期
		ui8			times_usable;		//	可使用次数	smallint; 0 表示为按时间计费，其他值表示可使用次数
		ui8			times_used;			//	已使用次数	smallint; 初始值为 0

		struct
		{
			ui8		index;				//	附加属性变更项ID_1	PAI	tinyint
			i16		value;				//	附加属性变更项值_1	PAV	smallint

		}APAI[4];

	}ITEM_OBJ,*PITEM_OBJ;

	// 道具使用记录
	struct  ITEM_USE_LOG
	{
		ui32 nItemID;							// 道具ID
		char szItemName[MAX_ACCOUNT_LENGTH];	// 道具名称	
		struct
		{
			ui8		index;		//	属性变更项ID	tinyint
			i16		value;		//	属性变更项值	smallint

		}PA_Item[8];//物品的基本属性
		ITEM_OBJ	itemObj;					// 道具属性
		SQLSVR_DATETIME useTime;				// 使用时间
		char szCharName[MAX_CHAR_NAME_LENGTH];	// 对应角色昵称
		
	};

	//道具删除记录
	struct  ITEM_DELETE_LOG
	{
		ui32 nItemID;							// 道具ID
		char szItemName[MAX_ACCOUNT_LENGTH];	// 道具名称	
		struct
		{
			ui8		index;		//	属性变更项ID	tinyint
			i16		value;		//	属性变更项值	smallint

		}PA_Item[8];//物品的基本属性
		ITEM_OBJ	itemObj;					// 道具属性
		SQLSVR_DATETIME availabilityTime;		// 使用期限
		SQLSVR_DATETIME getTime;				// 获得时间
		SQLSVR_DATETIME deleteTime;				// 删除时间
		char szReason[256];						// 删除原因
	};

	// 角色删除记录
	// 帐号、角色名、体型、位置、等级、当前经验、游戏G币、M币数量、删除时间
	struct	CHARECTOR_DELETE_LOG
	{
		char	account_name[32];
		char	char_name[20];	
		ui8		basic_figure;
		ui8		position;
		ui16	level;
		ui32	exp;
		ui32	gb;
		ui32	mb;
		SQLSVR_DATETIME	deleteTime;	
	};

}
#pragma pack(pop)

#endif
