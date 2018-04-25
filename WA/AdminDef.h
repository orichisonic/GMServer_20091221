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
	const int MAX_GUILD_NAME_LENGTH			= 13;
	const int MAX_CURRENT_QUEST				= 15;

	const int MSG_BASE_ASTA					= 25000; 
	const int MSG_BASE_ATAS					= 26000;

	//
	// ���ݰ�ͷ
	//
	struct TCP_MSG_BASE
	{
		unsigned short header;

		TCP_MSG_BASE(unsigned short id)
			:header(id)
		{
		}
	};

	// ʱ��
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
	// ��ѯ����
	//
	enum QUERY_CONDITION
	{
		QC_INVALID = 0,
		QC_USERNAME,		// ͨ���ʺ���
		QC_CHARACTERNAME,	// ͨ����ɫ��
		QC_USERACCOUNTID,	// ͨ���ʺ�ID
		QC_CHARACTERID,		// ͨ����ɫID
		QC_GUILDID,			// ͨ������ID
		QC_GUILDNAME,		// ͨ����������
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

	struct GuildId
	{
		unsigned int guildIndex;
	};

	struct GuildName
	{
		char guildName[MAX_GUILD_NAME_LENGTH];
	};

	//
	// ��ѯ��ɫ������Ϣ
	//
	struct REP_CHARACTER_BASE_INFO
	{
		unsigned int	srv_realm_id;	// ����id
		unsigned int	srv_cluster_id;	// ��������id
		char			user_name[MAX_ID_LENGTH]; // �ʺ�
		unsigned int	char_id;		// ��ɫid
		char			char_name[MAX_CHARACTER_NAME_LENGTH];		// ��ɫ����
		char			char_title1[MAX_CHARACTER_NAME_LENGTH];	// ��ɫ��ν1
		char			char_title2[MAX_CHARACTER_NAME_LENGTH];	// ��ɫ��ν2
		unsigned char	char_type;			// ��ɫ����(ְҵ�Ա��)
		bool			is_online;			// �Ƿ�����
		bool			is_closed;			// �Ƿ��ͣ
		unsigned char	char_level;			// ��ɫ�ȼ�
		unsigned int	money;				// ��Ϸ������
		unsigned int	nineyou_money;		// 9�α�����
		unsigned int	reserve_money;		// ������
		unsigned int	cur_scene_id;		// ��ǰ���ڳ���id
		unsigned int	cur_scene_pos;		// ��ǰ���ڳ�������

		unsigned int	hp;					// ����ֵ
		unsigned short	mp;					// ħ��ֵ
		unsigned short	vitality;			// ����ֵ
		unsigned short	good_evil;			// �ƶ�ֵ
		unsigned short	physical_attack;	// ������
		unsigned short	physical_defense;	// �������
		unsigned short	magic_attack;		// ħ������
		unsigned short	magic_defense;		// ħ������
		float			critical;			// ������
		unsigned short	strength;			// ����
		unsigned short	dexterity;			// ����
		unsigned short	intellect;			// ����
		unsigned short	corporeity;			// ����
		unsigned short	potential_point;	// Ǳ����
		unsigned int	exp;				// ����
		unsigned int	exp_next_level;		// ��������

		float			physical_resist;	// ������
		float			magic_resist;		// ��������
		float			fire_resist;		// ���濹��
		float			cold_resist;		// ��������
		float			poison_resist;		// ���ؿ���
	};

	// װ������
	enum ITEM_GROUP
	{
		EG_EQUIPMENT_ITEM,
		EG_BAG_ITEM,
		EG_BANK_ITEM,
	};

	// װ����λ��
	enum EQUIPMENT_POSITION
	{
		EP_HEAD				= 0,	// ͷ
		EP_SHOULDERS		= 1,	// ��
		EP_MAIN_HAND		= 2,	// ����
		EP_UPPERBODY		= 3,	// �·�
		EP_LOWERBODY		= 4,	// ����
		EP_FEET				= 5,	// ��
		EP_HANDPOS			= 6,	// ��
		EP_NECK				= 7,	// ����
		EP_MEDALPOS			= 8,	// ����λ��
		EP_FINGER			= 9,	// ��ָ
		EP_HEAD_F			= 10,	// ͷ(ʱװ)
		EP_SHOULDERS_F		= 11,	// ���(ʱװ)
		EP_UPPERBODY_F		= 12,	// �·�(ʱװ)
		EP_LOWERBODY_F		= 13,	// ����(ʱװ)
		EP_FEET_F			= 14,	// ��ʱװ)
		EP_HANDPOS_F		= 15,	// ��(ʱװ)
	};

	// ��Ʒ����
	enum ITEM_STRUCT_TYPE
	{
		IST_WEAPON			= 1,	// ����
		IST_ARMOR			= 2,	// ����װ��
		IST_GOODS			= 3,	// ����Ʒ ����
		IST_EXPENDABLE		= 4,	// ����Ʒ
		IST_BAG				= 5,	// ����
		IST_RECIPE			= 6,	// �䷽
		IST_GEM				= 7,	// ��ʯ����Ʒ
	};

	inline unsigned char GetStructType(unsigned int typeId)
	{
		return ((typeId)>>(28))&((0x1<<(4))-1);
	}

	// ��ʯ����ɫ
	enum GEM_COLOR
	{
		GEM_COLOR_RED		= 1,	// ��
		GEM_COLOR_YELLOW	= 2,	// ��
		GEM_COLOR_BLUE		= 3,	// ��
		GEM_COLOR_GREEN		= 4,	// ��
		GEM_COLOR_PURPLE	= 5,	// ��
		GEM_COLOR_WHITE		= 6,	// ��
	};

	// �޸Ľ�ɫ����
	enum MOD_CHAR_ATTR
	{
		MCA_ONLINE_REPAIR			= 0,	// �������(�´ν�ɫ����ʱ�������������޸���ֵ�쳣)
		MCA_LEVEL					= 1,	// �ȼ�
		MCA_EXP						= 3,	// ����ֵ
		MCA_MONEY					= 13,	// ��Ǯ
		MCA_VIRALITY_MAX			= 19,	// �������ֵ
		MCA_STRENGTH				= 24,	// ����
		MCA_DEXTERITY				= 25,	// ����
		MCA_INTELLECT				= 26,	// ����
		MCA_CORPOREITY				= 27,	// ����
		MCA_PHYSICAL_ATTACK			= 32,	// ������
		MCA_CRITICAL				= 35,	// ������
		MCA_PHYSICAL_DEFENSE		= 36,	// �������
		MCA_HP_MAX					= 40,	// ���HP
		MCA_MP_MAX					= 44,	// ���MP
		MCA_MAGIC_ATTACK			= 50,	// ħ������
		MCA_MAGIC_DEFENSE			= 51,	// ħ������
		MCA_PHYSICAL_RESIST			= 80,	// ������
		MCA_PHYSICAL_DAMAGE_DERATE	= 81,	// �����˺�����
		MCA_MAGIC_RESIST			= 82,	// ħ������
		MCA_MAGIC_DAMAGE_DERATE		= 83,	// ħ���˺�����
		MCA_FIRE_RESIST				= 84,	// ���濹��
		MCA_COLD_RESIST				= 85,	// ��������
		MCA_POISON_RESIST			= 86,	// ���ؿ���
		MCA_FIRE_ATTACK				= 87,	// ���湥��
		MCA_COLD_ATTACK				= 88,	// ��������
		MCA_POISON_ATTACK			= 89,	// ���ع���

		MCA_MAX,
	};

	enum ADMIN_SERVER_RES_TYPE
	{
		RT_ITEM_NAME,	// ��ID��Ӧ����Ʒ����
		RT_SCENE_INFO,	// ��ID��Ӧ�ĳ�����Ϣ
		RT_QUEST_INFO,	// ������
		RT_SKILL_LEARN,	// ����ѧϰ
	};

	// ����ְ��
	enum GUILD_MEMBERSHIP_LEVEL
	{
		GJ_MASTER,				// �᳤
		GJ_NORMAL_MEMBER,		// һ���Ա
	};

	// ְҵ����
	enum OCCUPATION_TYPE
	{
		O_FENCER = 1,					// ��ʿ
		O_WUSHU = 2,					// ����
		O_MAGE = 3,						// ��ʦ
		O_BERSERKER = 4,				// ħ��ʿ
		O_WATCHER = 5,					// ����ʿ
		O_WISDOM = 6,					// ���ʦ
		O_HEAVYDUTY = 7,				// ��սʿ
		O_SHADOWDANCER = 8,				// ������
		O_ELEMENT = 9,					// Ԫ��ʹ
		O_EIHERJAR = 10,				// ʥսʿ
		O_WUXUEJIA = 11,				// �����
		O_SHENGJIAOSHI = 12,			// ���
		O_ALCHEMIST = 13,				// ������ʦ
		O_STELL_ALCHEMIST = 14,			// �ֱ���ʦ
		O_MAGIC_ALCHEMIST = 15,			// ħ����ʦ
		O_GUN_ALCHEMIST = 16,			// ������ʦ
	};

	// ���������
	enum LIVING_SKILL
	{
		LS_BLACKSMITH = 1,		// ����
		LS_SEWING = 2,			// �÷�
		LS_HANDCRAFT = 3,		// �ֹ�
		LS_PHARMACY = 4,		// ��ҩ
		LS_ENCHANTMENT = 5,		// ��ħ
	};

	// �䳤���ݰ�
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
