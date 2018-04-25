/******************************************************************************
						GM Tools��GM Serverͨ��Э��

ȫ�ֳ��������ݽṹ
create by LiaoHuaifu 2008/9/2

���ڹ���: 2008/10/18
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
	//	���� oledb.h -> struct DBTIMESTAMP
	//	����˽ṹ��Ŀ���ǲ��ذ��� oledb.h ��ͷ�ļ���
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
	// ���ֵ����
	//
	enum
	{
		MAX_CHARACTER_NUM 	    = 3,			//  һ���ʺſɴ���������ɫ����	
		MAX_ACTIVE_CODE_LENGTH  = 6,			//  ��������󳤶�
		MAX_CHAR_NAME_LENGTH    = 20,			//	��ɫ����󳤶ȣ�DB�ֶγ��ȣ�
		MAX_ACCOUNT_LENGTH	    = 32,			//  �ʺ�����󳤶ȣ�DB�ֶγ��ȣ�
		MAX_PASSWORD_LENGTH     = 32,           //  ������󳤶ȣ�DB�ֶγ��ȣ�
		MAX_POP_MSG_LENGTH	    = 80,			//  ������Ϣ��󳤶�
		MAX_PUB_NAME_LENGTH		= 16,			//  ���ֲ�������󳤶�

	};

	//
	// GMTools ������������
	//
	typedef enum
	{
		OP_INVALID = 0,        // ��
		
		// һ�ڹ���
		OP_LOGIN		=   5,	// ��¼GMS 
		OP_PLAYER_INFO  =  10, 	// �鿴�û��Ļ�����Ϣ
		OP_CHAR_INFO    =  11, 	// �鿴��ҽ�ɫ�Ļ�����Ϣ
		OP_KICK  	    =  20,  // ����
		OP_BAN 	 	    =  30,  // ���
		OP_RELEASE      =  31,	// ���
		OP_POP   	    =  40,	// ����	
		OP_EXP_RATE		=  50,	// �޸ľ���ֵ	add 2008/9/26
		OP_SCORE_RATE	=  51, // �޸Ļ��ֱ���

		// ���ڹ���
		OP_ACCOUNT_ACTIVE_SEARCH	= 60,	// �ʺż����ѯ 
		OP_ITEM_SEARCH				= 70,	// ��Ʒ��ѯ
		OP_CONSUME_RECORD_SEARCH	= 80,	// ���Ѽ�¼��ѯ
		OP_ACCOUNT_LOGIN_LOGOUT_LOG	= 90,	// �ʺŵ�½��־
		OP_SKILL_SEARCH				= 100,	// ���ܲ�ѯ		
		OP_CHARACTER_INFO_OPERATION	= 110,	// ��ɫ�������޸ġ�ɾ�����ָ���

		// ���ڹ���
		OP_FRIEND_SEARCH = 112,				// ���Ѳ�ѯ
		OP_LOG_SEARCH = 114,				// �����־��Ϣ��ѯ
		OP_PRASENT_ITEM = 116,				// ���͵���

		OP_ACTIVE_CAMPAIGN = 150,			// �����
		
		OP_NUM,

	}CMD_TYPE;

	//
	// GM Server ������������
	//
	typedef enum
	{
		RE_INVALID = 0,        // ��
		
		// һ�ڹ���
		RE_LOGIN   = 5,			// ���ص�¼��Ϣ
		RE_PLAYER_INFO  =  10, 	// �鿴�û��Ļ�����Ϣ
		RE_CHAR_INFO    =  11, 	// �鿴��ҽ�ɫ�Ļ�����Ϣ
		RE_KICK  	    =  20,	// ����
		RE_BAN 	 	    =  30,	// ���
		RE_RELEASE      =  31,	// ���
		RE_POP   	    =  40,	// ����
		RE_EXP_RATE		=  50,	// �޸ľ���ֵ	add 2008/9/26
		RE_SCORE_RATE		=  51,	// �޸Ļ���ֵ

		// ���ڹ���
		RE_ACCOUNT_ACTIVE_SEARCH	= 60,	// �ʺż����ѯ 
		RE_ITEM_SEARCH				= 70,	// ��Ʒ��ѯ
		RE_CONSUME_RECORD_SEARCH	= 80,	// ���Ѽ�¼��ѯ
		RE_ACCOUNT_LOGIN_LOGOUT_LOG	= 90,	// �ʺŵ�½��־
		RE_SKILL_SEARCH				= 100,	// ���ܲ�ѯ
		RE_CHARACTER_INFO_OPERATION	= 110,	// ��ɫ�������޸ġ�ɾ�����ָ��� ��GB���޸ĵȼ���

		// ���ڹ���
		RE_FRIEND_SEARCH = 112,				// ���Ѳ�ѯ
		RE_LOG_SEARCH = 114,				// �����־��Ϣ��ѯ
		RE_PRASENT_ITEM = 116,				// ���͵���

		RE_ACTIVE_CAMPAIGN = 150,			// �����

		RE_NUM,

	}RES_TYPE;

	typedef enum
	{
		ERR_INVALID	= 0,
		
		ERR_LOGIN_1 = 5,				// �ʺŲ�����
		ERR_LOGIN_2 = 6,				// �������
		ERR_LOGIN_3 = 7,				// �Ѿ���¼
		ERR_LOGIN_4 = 8,				// GMTools������������  30��	
		ERR_LOGIN_5 = 9,				// ע���¼ʧ��	

		ERR_PLAYER_INFO  =  10,			// �鿴�û��Ļ�����Ϣʧ��(δ֪ԭ��)
		ERR_CHAR_INFO    =  11, 		// �鿴��ҽ�ɫ�Ļ�����Ϣʧ��(δ֪ԭ��)

		ERR_KICK_1		 =   20,		// ����ʧ��(δ֪ԭ��)
		ERR_KICK_2		 =   21,		// ����ʧ��(Ҫ�ߵ��˲�����)

		ERR_BAN_1       =   30,			// ���ʧ�� (δ֪ԭ��)
		ERR_BAN_2       =   31,  		// ���ʧ�� (Ҫ������˲�����),����ĳ� �������ڸ���ҡ�

		ERR_RELEASE_1	=   32,			//�����ڸ����
		ERR_RELEASE_2	=   33,			//�����δ�����
		ERR_RELEASE_3	=   34,			//���ʧ��(δ֪ԭ��)

		ERR_POP 		 =   40,	     // ����ʧ�� (δ֪ԭ��)

		ERR_ACCOUNT_ACTIVE_SEARCH	= 60,	// �������

		ERR_NUM,

		ERR_UNKNOW					=   ERR_NUM + 1, // ʧ��δ֪ԭ��(ͨ��)		
		ERR_ACCOUNT_NOT_EXIST		=   ERR_NUM + 2, // �ʺŲ�����(ͨ��)
		ERR_CHARACTER_NOT_EXIST		=   ERR_NUM + 3, // ��ɫ������(ͨ��)
		ERR_CHANNEL_NOT_EXIST		=   ERR_NUM + 3, // ������Ƶ�� 

		ERR_CHARNAME_CAN_NULL		=	ERR_NUM	+ 4,	// ��ɫ������Ϊ��
	}ERROR_CODE;

 	// �������
 	typedef enum 					
 	{
 		HSNFT_LAST,					//	��һ��������
 		HSNFT_CURRENT,				//	��ǰ��������
 		HSNFT_TOTAL,				//	������
 		HSNFT_COUNT,
 	}HSNFT_ENUM;

 	typedef union
 	{
 		struct  
 		{						
 			ui16	typeid_3;		//	������
 			ui8		typeid_2;		//	�η���
 			ui8		typeid_1;		//	������
 		}	sep_val;
 		ui32	com_val;
 
 	}ITEM_ID;

	//
	// ��ɫ�������ݽṹ
	//
	typedef struct _tagCHARACTER_BASIC_INFO
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];			// ��ɫ����
		SQLSVR_DATETIME tDeleteDate;						// ɾ������
		ui8		nFigure;									// ����
		ui8		nPosition;									// λ��
		ui16	nLevel;										// �ȼ�
		ui32	nExp;										// ��ǰ����
		char 	szClubName[MAX_CHAR_NAME_LENGTH];			// �������ֲ�
		ui8		nClubPosition;								// ���ֲ�ְλ
		ui32	nGB;										// ��ǰ��Ϸ�� 
		//ui32	nMB;										// ��ǰMB
		//ui8		nIsOnline;									// �Ƿ�����

		void CHARACTER_BASIC_INFO_GMS()
		{
			memset(this,0,sizeof(_tagCHARACTER_BASIC_INFO));
		}
	}CHARACTER_BASIC_INFO_GMS,*PCHARACTER_BASIC_INFO_GMS;

	//
	//	 ��ɫ��������
	//
	typedef struct _tagCHARACTER_ABILITY
	{
		i16		nRun;		//	�ܶ� 
		i16		nJump;		//	���� 
		i16		nStrength;	//	���� 
		i16		nPass;		//	���� 
		i16		nDribble;	//	���� 
		i16		nBlock;		//	��ñ 
		i16		nRebound;	//	���� 
		i16		nSteal;		//	���� 

		i16		nClose;		//	��Ͷ 
		i16		nShot;		//	��Ͷ 
		i16		nLongshot;	//	ԶͶ 
		i16		nDrive;		//	����

		void CHARACTER_ABILITY_GMS()
		{
			memset(this,0,sizeof(_tagCHARACTER_ABILITY));
		}
	}CHARACTER_ABILITY_GMS,*PCHARACTER_ABILITY_GMS;


	typedef struct _tagSEASON_STAT
    {
		ui32	nScore;			//	[�洢]�÷�
		float	fScore_pt;		//	[����]�����÷�
		ui32	nThree_point;	//	[����]������
		float   fTotal_ratio;   //  [����]��������
		float   fThree_ratio;   //  [����]3��������
		ui32	nAssist;		//	[�洢]����
		float   fAssist_pt;     //  [����]��������
       ui32		nRebound;		//	[�洢]����
		float   fRebound_pt;   //  [����]��������
       ui32		nBlock;			//	[�洢]��ñ
 		float   fBlock_pt;     //  [����]������ñ
       ui32		nSteal;			//	[�洢]����	
		float   fSteal_pt;     //  [����]��������
		ui32    nScore_pure;    //  [�洢]��ʤ��
		ui32    nScore_1v1;     //  [�洢]1v1�÷�
		ui32    nPlay_time;     //  [�洢]����ʱ��
		ui32    nMvp_times;     //  [�洢]POG����

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

		ui32	nTotal_shoot;	 //	[�洢]��Ͷ��
		ui32	nTotal_hit;		 //	[�洢]������
		ui32	nThree_shoot;	 //	[�洢]3��Ͷ��
		ui32	nThree_hit;	    //	[�洢]3������
		ui32    nPlay_times;    //  [�洢]��������

		void SEASON_STAT_GMS()
		{
			memset( this, 0, sizeof( _tagSEASON_STAT ) );
		}
   }SEASON_STAT_GMS;


	/*********************************** ���ڹ��� ***********************************/
	struct PLAYER_UNIQUE_INFO 
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];		 // �����߽�ɫ����
		char 	szAccountName[MAX_ACCOUNT_LENGTH];       // �������ʺ�����	

		PLAYER_UNIQUE_INFO()
		{
			memset(this,0,sizeof(PLAYER_UNIQUE_INFO));			
		}
	};

	//��Ҽ�¼���ݽṹ
	struct PLAYER_CHARACTER_BASE_INFO 
	{
		PLAYER_UNIQUE_INFO	playInfo;	// �ʺš���ɫ��
		ui8		nFigure;				// ����
		ui8		nPosition;				// λ��
		ui16	nLevel;					// �ȼ�
		char	szPubName[MAX_PUB_NAME_LENGTH];	//�������ֲ�
		ui32	nPubPosition;			// ���ֲ�ְλ
		ui32	nGB;					// G��
		//ui32	nMB;					// M��

		PLAYER_CHARACTER_BASE_INFO()
		{
			memset(this,0,sizeof(PLAYER_CHARACTER_BASE_INFO));				
		}
	};

	//���Ѽ�¼���ݽṹ
	struct CONSUME_RECORD_INFO 
	{
		SQLSVR_DATETIME	tGetDateTime;	// ���ʱ�� 
		ui16	nType;	 // ����
		ui32	nItemID; // ��ƷID			
		char	szItemName[MAX_CHAR_NAME_LENGTH];	// ��Ʒ����
		ui32	nGB;	 // G��
		ui32	nMB;	 // M��
		char	szPayer[MAX_CHAR_NAME_LENGTH];		// ������
		char	szReceiver[MAX_CHAR_NAME_LENGTH];	// ������

		CONSUME_RECORD_INFO()
		{
			memset(this,0,sizeof(CONSUME_RECORD_INFO));	
		}
	};

	//��½��־��¼���ݽṹ
	struct	LOGIN_OUT_INFO 
	{
		ui32	id;									// id
		char	szCharName[MAX_CHAR_NAME_LENGTH];	// ��ɫ����
		ui32	login_server_id;					// ����id
		ui32	game_server_id;						// Ƶ��id
		char	user_ip[15];						// ��¼ip
		ui8		login_out;							// ��½(0)/�ǳ�(1)
		SQLSVR_DATETIME	lgoin_out_time;				// ʱ��

		LOGIN_OUT_INFO()
		{
			memset(this,0,sizeof(LOGIN_OUT_INFO));	
		}
	};

	//���ܼ�¼���ݽṹ
	struct SKIN_RECORD_INFO 
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];	// ��ɫ����
		ui16	SkillID;				// ����ID
		char	szSkillName[MAX_CHAR_NAME_LENGTH];	// ��������
		ui16	nWorkType;				// ְҵ
		ui8		nSkillType;				// ����
		ui8		nSkillLevel;			// ���ܼ���
		SQLSVR_DATETIME	tBuyTime;	    // ����ʱ��
		SQLSVR_DATETIME	tFirstUseTime;  // ��һ��ʹ��ʱ�� 
		bool	bEquip;					// �Ƿ�װ��

		SKIN_RECORD_INFO()
		{
			memset(this,0,sizeof(SKIN_RECORD_INFO));	
		}
	};

	//��Ʒ��¼���ݽṹ
	struct GOODS_INFO 
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];	// ��ɫ����
		ITEM_ID	GoodsID;				// ��ƷID
		ui16	nType;					// ��Ʒ����
		char	szGoodsName[MAX_CHAR_NAME_LENGTH];	// ��Ʒ��
		ui8		nLevel;					// ��Ʒ����
		bool	bEquip;					// �Ƿ�װ��
		
		SQLSVR_DATETIME	tLastTime;	    // ����ʱ��

		struct
		{
			ui8		index;				//	���Ա����ID	
			i16		value;				//	���Ա����ֵ	

		}PA_Item[8];//��Ʒ�Ĺ̶�����

		struct 
		{
			ui8		index;		//	���Ա����ID	tinyint
			i16		value;		//	���Ա����ֵ	smallint
			i16		mb;			//	���Լ۸�_MB		smallint
			i16		gb;			//	���Լ۸�_GB		smallint

		}APA_Item[6];//�ɹ��������һ

		struct
		{
			ui8		index;		//	���Ա����ID	tinyint
			i16		value;		//	���Ա����ֵ	smallint
			i16		mb;			//	���Լ۸�_MB		smallint
			i16		gb;			//	���Լ۸�_GB		smallint

		}EPA_Item[6];//�ɹ�������Զ�

		GOODS_INFO()
		{
			memset(this,0,sizeof(GOODS_INFO));	
		}
	};

	// ������Ϣ���ݽṹ
	struct FRIEND_INFO
	{
		char szFriendName[MAX_CHAR_NAME_LENGTH];	// �����ǳ�
		ui8  nFriendStatus;							// ����״̬���Ƿ����ߣ�
	};
	
	// ������Ϣ
	struct UPLEVEL_LOG 
	{
		char	szCharName[MAX_CHAR_NAME_LENGTH];		 // ��ɫ����
		char 	szAccountName[MAX_ACCOUNT_LENGTH];       // �ʺ�����	
		ui32	nPlayerID;									// ��ɫID
		ui8		nLevel;									//�ȼ�
		SQLSVR_DATETIME upLevelTime;					//����ʱ��
	};

	//
	//	��Ʒ�������ݽṹ (GMtools ʹ��)
	//
	typedef struct _tagITEM_OBJ
	{
		ITEM_ID		type_id;
		i32			scheme_selected;	//	��Ʒɫ�ʷ���		int

		ui32		time_expired;		//	32bit time_t , ����ʱ��	datetime; 0 ��ʾΪ�������
		ui8			times_usable;		//	��ʹ�ô���	smallint; 0 ��ʾΪ��ʱ��Ʒѣ�����ֵ��ʾ��ʹ�ô���
		ui8			times_used;			//	��ʹ�ô���	smallint; ��ʼֵΪ 0

		struct
		{
			ui8		index;				//	�������Ա����ID_1	PAI	tinyint
			i16		value;				//	�������Ա����ֵ_1	PAV	smallint

		}APAI[4];

	}ITEM_OBJ,*PITEM_OBJ;

	// ����ʹ�ü�¼
	struct  ITEM_USE_LOG
	{
		ui32 nItemID;							// ����ID
		char szItemName[MAX_ACCOUNT_LENGTH];	// ��������	
		struct
		{
			ui8		index;		//	���Ա����ID	tinyint
			i16		value;		//	���Ա����ֵ	smallint

		}PA_Item[8];//��Ʒ�Ļ�������
		ITEM_OBJ	itemObj;					// ��������
		SQLSVR_DATETIME useTime;				// ʹ��ʱ��
		char szCharName[MAX_CHAR_NAME_LENGTH];	// ��Ӧ��ɫ�ǳ�
		
	};

	//����ɾ����¼
	struct  ITEM_DELETE_LOG
	{
		ui32 nItemID;							// ����ID
		char szItemName[MAX_ACCOUNT_LENGTH];	// ��������	
		struct
		{
			ui8		index;		//	���Ա����ID	tinyint
			i16		value;		//	���Ա����ֵ	smallint

		}PA_Item[8];//��Ʒ�Ļ�������
		ITEM_OBJ	itemObj;					// ��������
		SQLSVR_DATETIME availabilityTime;		// ʹ������
		SQLSVR_DATETIME getTime;				// ���ʱ��
		SQLSVR_DATETIME deleteTime;				// ɾ��ʱ��
		char szReason[256];						// ɾ��ԭ��
	};

	// ��ɫɾ����¼
	// �ʺš���ɫ�������͡�λ�á��ȼ�����ǰ���顢��ϷG�ҡ�M��������ɾ��ʱ��
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
