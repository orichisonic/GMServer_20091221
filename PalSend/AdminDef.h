#pragma once

#pragma pack(push, 1)

namespace ADMIN
{
	const int MAX_ID_LENGTH					= 20;
	const int MAX_PASSWORD_LENGTH			= 21;
	const int MAX_CHARACTER_NAME_LENGTH		= 13;
	const int MAX_ITEM_NAME_LENGTH			= 33;
	const int MAX_QUEST_NAME_LENGTH			= 65;
	const int MAX_MAIL_TITLE				= 21;
	const int MAX_SCENE_NAME_LENGTH			= 21;

	const int MSG_BASE_ASTA					= 25000; 
	const int MSG_BASE_ATAS					= 26000;

	//
	// 数据包头
	//
	struct TCP_MSG_BASE
	{
		unsigned short header;

		TCP_MSG_BASE(unsigned short id)
			:header(id)
		{
		}
	};

	// 时间
	struct DATE_TIME
	{
		unsigned int	year;
		unsigned int	month;
		unsigned int	day;
		unsigned int	hour;
		unsigned int	minute;
		unsigned int	second;
	};

	//
	// 查询条件
	//
	enum QUERY_CONDITION
	{
		QC_INVALID = 0,
		QC_USERNAME,		// 通过帐号名
		QC_CHARACTERNAME,	// 通过角色名
		QC_USERACCOUNTID,	// 通过帐号ID
		QC_CHARACTERID,		// 通过角色ID
	};

	struct QueryUserName
	{
		char userName[MAX_ID_LENGTH];
	};

	struct QueryCharacterName
	{
		char charName[MAX_CHARACTER_NAME_LENGTH];
	};

	struct QueryUserAccountId
	{
		unsigned int accountIndex;
	};

	struct QueryCharacterId
	{
		unsigned int charIndex;
	};

	//
	// 查询角色基本信息
	//
	struct REP_CHARACTER_BASE_INFO
	{
		unsigned int	srv_realm_id;	// 大区id
		unsigned int	srv_cluster_id;	// 服务器组id
		char			user_name[MAX_ID_LENGTH]; // 帐号
		unsigned int	char_id;		// 角色id
		char			char_name[MAX_CHARACTER_NAME_LENGTH];		// 角色名称
		char			char_title1[MAX_CHARACTER_NAME_LENGTH];	// 角色称谓1
		char			char_title2[MAX_CHARACTER_NAME_LENGTH];	// 角色称谓2
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
	};

	// 装备的组
	enum ITEM_GROUP
	{
		EG_EQUIPMENT_ITEM,
		EG_BAG_ITEM,
		EG_BANK_ITEM,
	};

	// 装备的位置
	enum EQUIPMENT_POSITION
	{
		EP_HEAD				= 0,	// 头
		EP_SHOULDERS		= 1,	// 肩
		EP_MAIN_HAND		= 2,	// 武器
		EP_UPPERBODY		= 3,	// 衣服
		EP_LOWERBODY		= 4,	// 裤子
		EP_FEET				= 5,	// 脚
		EP_HANDPOS			= 6,	// 手
		EP_NECK				= 7,	// 脖子
		EP_MEDALPOS			= 8,	// 徽章位置
		EP_FINGER			= 9,	// 手指
		EP_HEAD_F			= 10,	// 头(时装)
		EP_SHOULDERS_F		= 11,	// 肩膀(时装)
		EP_UPPERBODY_F		= 12,	// 衣服(时装)
		EP_LOWERBODY_F		= 13,	// 裤子(时装)
		EP_FEET_F			= 14,	// 脚时装)
		EP_HANDPOS_F		= 15,	// 手(时装)
	};

	// 宝石的颜色
	enum GEM_COLOR
	{
		GEM_COLOR_RED		= 1,	// 红
		GEM_COLOR_YELLOW	= 2,	// 黄
		GEM_COLOR_BLUE		= 3,	// 蓝
		GEM_COLOR_GREEN		= 4,	// 绿
		GEM_COLOR_PURPLE	= 5,	// 紫
		GEM_COLOR_WHITE		= 6,	// 白
	};

	// 修改角色属性
	enum MOD_CHAR_ATTR
	{

		MCA_LEVEL					= 1,	// 等级
		MCA_EXP						= 3,	// 经验值
		MCA_MONEY					= 13,	// 金钱
		MCA_VIRALITY_MAX			= 19,	// 活力最大值
		MCA_STRENGTH				= 24,	// 力量
		MCA_DEXTERITY				= 25,	// 敏捷
		MCA_INTELLECT				= 26,	// 智力
		MCA_CORPOREITY				= 27,	// 体质
		MCA_PHYSICAL_ATTACK			= 32,	// 物理攻击
		MCA_CRITICAL				= 35,	// 暴击率
		MCA_PHYSICAL_DEFENSE		= 36,	// 物理防御
		MCA_HP_MAX					= 40,	// 最大HP
		MCA_MP_MAX					= 44,	// 最大MP
		MCA_MAGIC_ATTACK			= 50,	// 魔法攻击
		MCA_MAGIC_DEFENSE			= 51,	// 魔法防御
		MCA_PHYSICAL_RESIST			= 80,	// 物理抗性
		MCA_PHYSICAL_DAMAGE_DERATE	= 81,	// 物理伤害减免
		MCA_MAGIC_RESIST			= 82,	// 魔法抗性
		MCA_MAGIC_DAMAGE_DERATE		= 83,	// 魔法伤害减免
		MCA_FIRE_RESIST				= 84,	// 火焰抗性
		MCA_COLD_RESIST				= 85,	// 冰冻抗性
		MCA_POISON_RESIST			= 86,	// 毒素抗性
		MCA_FIRE_ATTACK				= 87,	// 火焰攻击
		MCA_COLD_ATTACK				= 88,	// 冰冻攻击
		MCA_POISON_ATTACK			= 89,	// 毒素攻击

		MCA_MAX,
	};

	// 变长数据包
	template<class TBase, class TElemType, unsigned t_maxElemAmount>
	struct VarLenStruct : public TBase
	{
	private:
		typedef VarLenStruct<TBase, TElemType, t_maxElemAmount> Self;

	public:
		typedef TElemType		ElemType;
		enum{ MAX_ELEM_AMOUNT	= t_maxElemAmount,};

		unsigned short			amount;
		ElemType		elems[MAX_ELEM_AMOUNT];

		VarLenStruct()
			:amount(0)
		{
		}

		template<class TAddElemType>
		bool Add(const TAddElemType& elem)
		{
			if(amount >= MAX_ELEM_AMOUNT)
				return false;

			::memcpy(&elems[amount], &elem, sizeof(TAddElemType));
			++amount;
			return true;
		}

		ElemType* GetNextElemBuffer()
		{
			if(amount >= MAX_ELEM_AMOUNT)
				return NULL;

			return Get(amount++);
		}

		const ElemType* Get(unsigned index)const
		{
			if(index >= amount)
				return NULL;
			return &elems[index];
		}

		ElemType* Get(unsigned index)
		{
			if(index >= amount)
				return NULL;
			return &elems[index];
		}

		unsigned GetAmount()const
		{
			return amount;
		}

		static unsigned GetMaxAmount()
		{
			return MAX_ELEM_AMOUNT;
		}

		unsigned GetSize()const
		{
			return (unsigned int)(((Self*)0)->elems) + (amount * sizeof(ElemType));
		}

		void Clear()
		{
			memset(elems, 0, sizeof(elems));
			amount = 0;
		}
	};
}

#pragma pack(pop)
