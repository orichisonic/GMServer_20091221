#pragma once

class __declspec(dllexport) CEnumCore
{
public:
	CEnumCore(void);
	~CEnumCore(void);
	struct TagFormat
	{
		enum __Enum 
		{      
			TLV_STRING = 0,
			TLV_MONEY = 1,
			TLV_DATE = 2,
			TLV_INTEGER = 3,
			TLV_EXTEND = 4,
			TLV_NUMBER = 5,
			TLV_TIME = 6,
			TLV_TIMESTAMP = 7,
			TLV_BOOLEAN = 8,
			TLV_UNKNOW = 9,
			TLV_SINGLE = 10,
			TLV_BYTE = 11
		}; 
		__Enum _value;
		TagFormat(int value = 0) : _value((__Enum)value) {}
		TagFormat& operator=(int value) 
		{       
			this->_value = (__Enum)value;
			return *this;
		}  
		operator int() 
			const 
		{        return this->_value;    }
	};
	struct Body_Status
	{
		enum __Enum 
		{      
			MSG_STRUCT_OK = 0,
			MSG_STRUCT_ERROR = 1,
			ILLEGAL_SOURCE_ADDR = 2,
			AUTHEN_ERROR = 3,
			OTHER_ERROR = 4
		}; 
		__Enum _value;
		Body_Status(int value = 0) : _value((__Enum)value) {}
		Body_Status& operator=(int value) 
		{       
			this->_value = (__Enum)value;
			return *this;
		}  
		operator int() 
			const 
		{        return this->_value;    }
	};

	struct Msg_Category
	{    
		enum __Enum 
		{      
			COMMON = 0x80,
			USER_ADMIN = 0x81,
			MODULE_ADMIN = 0x82,
			USER_MODULE_ADMIN = 0x83,
			GAME_ADMIN = 0x84, 
			PAL_ADMIN = 0x62,//仙剑操作消息集
			PALLOG_ADMIN = 0x63,//仙剑日志操作消息集
			GT_ADMIN = 0x51,//GTOWN消息集
			GT_OP_ADMIN = 0x52,//GTOWN消息集
			WA_ADMIN=0x53,//勇士无双消息集
			CF_ADMIN=0x54,//侠道飞车消息集
			XDJG_ADMIN=0x55,//侠盗金刚消息集
			JW2_ADMIN=0x61,//劲舞团2
			CG2_ADMIN = 0x71,//魔力宝贝2操作消息集
			SDO_ADMIN = 0x87,//超级舞者操作消息集
			Ban_ADMIN=0x99,//封停操作消息集
			Message = 0xEE
		};  
		__Enum _value; // 枚举值  
		Msg_Category(int value = 0) : _value((__Enum)value) {}  
		Msg_Category& operator=(int value) 
		{       
			this->_value = (__Enum)value;
			return *this;
		}   
		operator int() 
			const 
		{        return this->_value;    }
	};
	struct TagName
	{    
		enum __Enum 
		{      
			UserName = 0x0101, //Format:STRING 用户名
			PassWord = 0x0102, //Format:STRING 密码
			MAC = 0x0103, //Format:STRING  MAC码
			Limit = 0x0104,//Format:DateTime GM帐号使用时效
			User_Status = 0x0105,//Format:INT 状态信息
			UserByID = 0x0106,//Format:INT 操作员ID
			RealName = 0x0107,//Format:STRING 中文名
			DepartID = 0x0108,//Format:INT 部门ID
			DepartName = 0x0109,//Format:STRING 部门名称
			DepartRemark = 0x0110,//Format:STRING 部门描述
			OnlineActive = 0x0111,//Format:Integer 在线状态
			UpdateFileName = 0x0112,//Format:String 文件名
			UpdateFileVersion = 0x0113,//Format:String 文件版本
			UpdateFilePath = 0x0114,//Format:String 文件路径
			UpdateFileSize = 0x0115,//Format:Integer 文件大小
			Process_Reason = 0x060B,//Format tring

			userIP=0x0119,//Format::STRING IP地址(新加)
			DepartStrGameID=0x0120,//Format:STRING部门游戏ID总和(新加)
			DugID=0x0121,////Format:STRINGBBugID(新加)
			DugSubject=0x0122,////Format:STRINGBBug主题(新加)
			DugType=0x0123,////Format:INTBug类型(新加)
			DugContent=0x0124,////Format:STRINGBug内容(新加)
			DugResult=0x0125,////Format:STRINGBug结果(新加)

			ServerInfo_IP = 0x0901,//Format:String 服务器IP
			ServerInfo_City = 0x0902,//Format:String 城市
			ServerInfo_GameID = 0x0903,//Format:Integer 游戏ID
			ServerInfo_GameName = 0x0904,//Format:String 游戏名
			ServerInfo_GameDBID = 0x0905,//Format:Integer 游戏数据库类型
			ServerInfo_GameFlag = 0x0906,//Format:Integer 游戏服务器状态
			ServerInfo_Idx = 0x0907,
			ServerInfo_DBName = 0x0908,

			ServerInfo_UserName=0x0909,////Format:STRING服务器用户名(新加)
			ServerInfo_PassWord=0x0910,////Format:STRINGB服务器密码(新加)

			GameID = 0x0200, //Format:INTEGER 消息ID
			ModuleName = 0x0201, //Format:STRING 模块名称
			ModuleClass = 0x0202, //Format:STRING 模块分类
			ModuleContent = 0x0203, //Format:STRING 模块描述

			//Added by tengjie 2010-05-05
			ToolProcTypeID	= 0x0204,		//进程工具类型ID
			PlayerAccount	= 0x0205,		//玩家帐号
			PlayerName		= 0x0206,		//玩家昵称
			ToolProcPath	= 0x0207,		//捕捉到的外挂进程全路径
			KickProcTime	= 0x0208,		//结束进程时间
			PlayerInServer	= 0x0209,		//玩家所在大区名称
//			ServerInfo_IP	= 0x0210,		//玩家所在大区ip
			GM_AccountInfo_Vip = 0x0211,		//是否vip用户
			WA_ClosePlayerGroup = 0x0212,    //封停帐号
			GM_ClosePlayerGroup_WA = 0x0213,    //封停帐号

			Module_ID = 0x0301, //Format:INTEGER 模块ID
			User_ID = 0x0302, //Format:INTEGER 用户ID
			ModuleList = 0x0303, //Format:String 模块列表
			SysAdmin = 0x0116,//Format:Integer 是否是系统管理员
			Host_Addr = 0x0401, //Format:STRING
			Host_Port = 0x0402, //Format:STRING
			Host_Pat = 0x0403,  //Format:STRING
			Conn_Time = 0x0404, //Format:DateTime 请求和响应时间
			Connect_Msg = 0x0405,//Format:STRING 请求连接信息
			DisConnect_Msg = 0x0406,//Format:STRING	 请求端开信息
			Author_Msg = 0x0407, //Format:STRING 验证用户的信息
			Status = 0x0408,//Format:STRING 操作结果
			Index = 0x0409, //Format:Integer 记录集序号
			PageSize = 0x0410,//Format:Integer 记录页显示长度
			PageCount = 0x0411,//Format:Integer 显示总页数
			SP_Name = 0x0412,//Format:Integer 存储过程名
			Real_ACT = 0x0413,//Format:String 操作的内容
			ACT_Time = 0x0414,//Format:TimeStamp 操作时间
			BeginTime = 0x0415,//Format:Date 开始日期
			EndTime = 0x0416,//Format:Date 结束日期
			GameName = 0x0501, //Format:STRING 游戏名称
			GameContent = 0x0502, //Format:STRING 消息描述
			Letter_ID = 0x0601, //Format:Integer 
			Letter_Sender = 0x0602, //Format:String
			Letter_Receiver = 0x0603, //Format:String
			Letter_Subject = 0x0604, //Format:String
			Letter_Text = 0x0605, //Format:String
			Send_Date = 0x0606, //Format:Date
			Process_Man = 0x0607, //Format:Integer
			Process_Date = 0x0608, //Format:Date
			Transmit_Man = 0x0609, //Format:Integer
			Is_Process = 0x060A, //Format:Integer
			LINKER_NAME = 0x0613,
			LINKER_CONTENT = 0x0614,

			/// <summary>
			/// 仙剑定义
			/// </summary>
			PAL_WORLDID=0x1001,//worldID Format:Integer
			PAL_PLAYERCOUNT=0x1002,//玩家数量 Format:Integer
			PAL_ACCOUNT=0x1003,//帐号Format:String
			PAL_ROLENAME=0x1004,//角色名Format:String
			PAL_BAGTYPE=0x1005,//背包类型Format:String
			PAL_ITEMNAME=0x1006,//物品名称Format:String
			PAL_ITEMPOS=0x1007,//物品位置Format:Integer
			PAL_ITEMTYPE=0x1008,//物品类型Format:Integer
			PAL_ORGOBJID=0x1009,//样版IDFormat:Integer
			PAL_LOCALTIME=0x1010,//建立时间Format:String 
			PAL_ITEMSERIAL=0x1011,//物品序号Format:Integer
			PAL_LIVETIME=0x1012,//到期时间Format:Integer
			PAL_COOLDOWN=0x1013,//下次可以使用的时间Format:Integer
			PAL_NPCTRADE=0x1014,// 是否可以跟NPC交易(1:可, 0:不可)Format:Integer
			PAL_USERTRADE=0x1015,// 是否可以跟玩家交易(1:可, 0:不可)Format:Integer
			PAL_ITEMMALLGOODS=0x1016,// 是否是商城物品(1:是, 0:不是)Format:Integer
			PAL_ITEMCOUNT=0x1017,//物品数量Format:Integer
			PAL_ITEMSELLCOST=0x1018,//物品卖价Format:Integer
			PAL_ITEMDURABLE=0x1019,//物品目前耐久度Format:Integer	
			PAL_INHERITID0=0x1020,//基本值(字首)%d %sFormat:String 
			PAL_INHERITID1=0x1021,//附魔%d %sFormat:String
			PAL_INHERITID2=0x1022,//冲物理%d %sFormat:String
			PAL_INHERITID3=0x1023,//冲法术%d %sFormat:String
			PAL_INHERITID4=0x1024,//玄石1%d %sFormat:String
			PAL_INHERITID5=0x1025,//玄石2%d %sFormat:String
			PAL_INHERITID6=0x1026,//玄石3%d %sFormat:String
			PAL_INHERITID7=0x1027,//玄石4%d %sFormat:String
			PAL_INHERITID8=0x1028,//玄石5%d %sFormat:String
			PAL_INHERITID9=0x1029,//玄石6%d %sFormat:String
			PAL_PETEXP=0x1030,//经验值Format:Integer
			PAL_PETNOWHP=0x1031,//目前HP值Format:Integer
			PAL_PETINDEX=0x1032,//宠物索引编号: %d (%s)Format:String
			PAL_PETRENEWCOUNT=0x1033,//还童次数Format:Integer
			PAL_PETLV=0x1034,//等级Format:Integer
			PAL_PETGENERATION=0x1035,//世代Format:Integer
			PAL_PETMIXCOUNT=0x1036,//合成次数Format:Integer
			PAL_PETGROWTHRATE=0x1037,//成长率Format:Integer
			PAL_PETATTRQUALITY=0x1038,//品质Format:Integer
			PAL_PETHP=0x1039,//HP加乘值Format:Integer
			PAL_PETWUSHU=0x1040,//武术Format:Integer
			PAL_PETSPIRIT=0x1041,//灵性Format:Integer
			PAL_PETCONSTITUTION=0x1042,//灵御Format:Integer
			PAL_PETQUALITY=0x1043,//灵御Format:Integer
			PAL_PETDEXTERITY=0x1044,//身法Format:Integer
			PAL_PETFATE=0x1045,//吉运Format:Integer
			PAL_PETHPPLUS=0x1046,//HP值慧根Format:Integer
			PAL_PETWUSHUPLUS=0x1047,//武术慧根Format:Integer
			PAL_PETSPIRITPLUS=0x1048,//灵性慧根Format:Integer
			PAL_PETCONSTITUTIONPLUS=0x1049,//防御慧根Format:Integer
			PAL_PETQUALITYPLUS=0x1050,//灵御慧根Format:Integer
			PAL_PETDEXTERITYPLUS=0x1051,//身法慧根Format:Integer
			PAL_PETFATEPLUS=0x1052,//吉运慧根Format:Integer
			PAL_PETSKILL0=0x1053,//技能1Format:Integer
			PAL_PETSKILL1=0x1054,//技能2Format:Integer
			PAL_PETSKILL2=0x1055,//技能3Format:Integer
			PAL_PETSKILL3=0x1056,//技能4Format:Integer
			PAL_PETSKILL4=0x1057,//技能5Format:Integer
			PAL_PETSKILL5=0x1058,//技能6Format:Integer
			PAL_FRIENDNAME=0x1059,//朋友角色名称Format:String
			PAL_RELATION=0x1060,//关系Format:String
			PAL_NICKNAME=0x1061,//昵称Format:String
			PAL_GUILDNAME=0x1062,//公会名称Format:String
			PAL_GUILDPOWERLEVEL=0x1063,//公会职级Format:Integer
			PAL_VOC=0x1064,//职业Format:String
			PAL_LEVEL=0x1065,//等级Format:Integer
			PAL_EXP=0x1066,//目前经验Format:Integer
			PAL_BODYMONEY=0x1067,//身上金钱Format:Integer
			PAL_BANKMONEY=0x1068,//仓库金钱Format:Integer
			PAL_SEX=0x1069,//性别Format:String
			PAL_FAME=0x1070,//名声Format:Integer
			PAL_HP=0x1071,//目前血量Format:Integer
			PAL_MP=0x1072,//目前法力Format:Integer
			PAL_AP=0x1073,//目前真气Format:Integer
			PAL_EP=0x1074,//目前精力Format:Integer
			PAL_SKILLPOINT=0x1075,//可配置技能点数Format:Integer
			PAL_SKILLTREENODE=0x1076,//目前技能树走到的位置Format:Integer
			PAL_REINCARNATION=0x1077,//转生次数Format:Integer
			PAL_DBID=0x1078,//数据库IDFormat:Integer
			PAL_ZONEID=0x1079,//区域IDFormat:Integer
			PAL_ZONENAME=0x1080,//区域名称Format:String
			PAL_LOGINTIME=0x1081,//登入时间Format:String
			PAL_LOGINIP=0x1082,//登入IPFormat:String
			PAL_QUESTTYPE=0x1083,//任务类型Format:Integer(1:已完成 2:已接)
			PAL_QUESTNAME=0x1084,//任务名称Format:String
			PAL_QUESTID=0x1085,//任务编号Format:Integer
			PAL_BULLETINID=0x1086,//公告IDFormat:Integer
			PAL_GUILDLEVEL=0x1087,//公会等级Format:Integer
			PAL_GUILDSCORE=0x1088,//公会积分Format:Integer
			PAL_GUILDMONEY=0x1089,//公会金钱Format:Integer
			PAL_GUILDJOINTIME=0x1090,//加入公会时间Format:String
			PAL_NEWGUILDNAME=0x1091,//新公会名称Format:String
			PAL_BULLETINMONTH=0x1092,//公告月份Format:Integer
			PAL_BULLETINDAY=0x1093,//公告天Format:Integer
			PAL_BULLETINDAYOFWEEK=0x1094,//公告星期数Format:Integer
			PAL_BULLETINHOUR=0x1095,//公告小时Format:Integer
			PAL_BULLETINMINUTE=0x1096,//公告分钟Format:Integer
			PAL_BULLETINCONTENT=0x1097,//公告内容Format:String
			PAL_MAILSUBJECT=0x1098,//邮件主旨Format:String
			PAL_MAILCONTENT=0x1099,//邮件内文Format:String
			PAL_MAILMONEY=0x1100,//邮件金钱Format:Integer
			PAL_MAILREASON=0x1101,// 发送奖品原因Format:String
			PAL_MAILGIFTDBID0=0x1102,// 奖品编号1Format:Integer
			PAL_MAILGIFTDBID1=0x1103,// 奖品编号2Format:Integer
			PAL_MAILGIFTDBID2=0x1104,// 奖品编号3Format:Integer
			PAL_MAILGIFTDBID3=0x1105,// 奖品编号4Format:Integer
			PAL_MAILGIFTDBID4=0x1106,// 奖品编号5Format:Integer
			PAL_DELAYTIME=0x1107,	// 延迟发送时间(秒),0代表立即发送Format:Integer
			PAL_REASON=0x1108,//解封停原因Format:String

			PAL_BOARDMESSAGE=0x1109,//公告内容Format:String
			PAL_ENDTIME=0x1110,//结束时间Format:String
			PAL_BEGINTIME=0x1111,//开始时间Format:String
			PAL_INTERVAL=0x1112,// 间隔时间Format:Integer
			PAL_TASKID=0x1113,//任务号Format:Integer
			PAL_STATUS=0x1114,//状态Format:Integer

			PAL_GUILDNOTICE=0x1115,//公会公告Format:String

			PAL_BANKPW=0x1116,//新银行密码(6位英数字)Format:String
			PAL_DROPROLEPW=0x1117,//新删角密码(6~12英数字)Format:String

			PAL_MONEYTYPE=0x1118,//类型(0:身上 1:银行)
			PAL_MONEYCOUNT=0x1119,// 增加金额(若为负数则代表减少)
			PAL_ACCOUNTID=0x1120,//帐号IDFormat:Integer
			PAL_LOGOUTTIME=0x1121,//登出时间Format:String
			PAL_ROLEID=0x1122,//角色IDFormat:Integer

			PAL_EVENTTYPE=0x1123,//事件类型Format:Integer
			PAL_EVENTNAME=0x1124,//事件名称Format:String
			PAL_FROMID=0x1125,//来源IDFormat:Integer
			PAL_FROMNAME=0x1126,//来源名字Format:String

			PAL_OFFICERDBID=0x1127,//邀请者角色编号Format:Integer
			PAL_OFFICERNAME=0x1128,//邀请者角色名称Format:String
			PAL_OFFICERZONEID=0x1129,//邀请者区域编号Format:Integer
			PAL_OFFICERZONENAME=0x1130,//邀请者区域名称Format:String


			PAL_LogBigTypeID=0x1131,//日志大类ID Format:Integer
			PAL_LogBigType=0x1132,//日志大类 Format:String
			PAL_LogSmallTypeID=0x1133,//日志小类ID Format:Integer
			PAL_LogSmallType=0x1134,//日志小类 Format:String

			PAL_X=0x1135,
			PAL_Y=0x1136,
			PAL_Z=0x1137,
			PAL_TARGETID=0x1138,
			PAL_TARGETNAME=0x1139,
			PAL_TIME=0x1140,//日志记录时间

			PAL_INHERITID0_NAME=0x1141,//基本值(字首)%d %sFormat:String 
			PAL_INHERITID1_NAME=0x1142,//附魔%d %sFormat:String
			PAL_INHERITID2_NAME=0x1143,//冲物理%d %sFormat:String
			PAL_INHERITID3_NAME=0x1144,//冲法术%d %sFormat:String
			PAL_INHERITID4_NAME=0x1145,//玄石1%d %sFormat:String
			PAL_INHERITID5_NAME=0x1146,//玄石2%d %sFormat:String
			PAL_INHERITID6_NAME=0x1147,//玄石3%d %sFormat:String
			PAL_INHERITID7_NAME=0x1148,//玄石4%d %sFormat:String
			PAL_INHERITID8_NAME=0x1149,//玄石5%d %sFormat:String
			PAL_INHERITID9_NAME=0x1150,//玄石6%d %sFormat:String

			PAL_ITEMFLAG=0x1151,//道具旗标
			PAL_BOSSID=0x1152,
			PAL_BOSSNAME=0x1153,



			PAL_AWARDMONEY=0x1154,//奖励金钱
			PAL_AWARDEXP=0x1155,//奖励经验值
			PAL_AWARDTITLEID=0x1156,//奖励称号编号
			PAL_AWARDTITLENAME=0x1157,//奖励称号名称
			PAL_FAMEID1=0x1158,//奖励声望1编号
			PAL_FAMENAME1=0x1159,//奖励声望1名称
			PAL_FAMEVALUE1=0x1160,//奖励声望1值
			PAL_FAMEID2=0x1161,//奖励声望2编号
			PAL_FAMENAME2=0x1162,//奖励声望2名称
			PAL_FAMEVALUE2=0x1163,//奖励声望2值

			PAL_ITEMID=0x1164,//物品ID

			PAL_SKILLID=0x1165,
			PAL_MAGICID=0x1166,
			PAL_MAGICNAME=0x1167,

			PAL_PETMIXCOUNTREMAIN=0x1168,
			PAL_PETNOTE=0x1169,

			PAL_ACCOUNTPW=0x1170,//帐号密码

			PAL_MALLTYPE=0x1171,//商城身份０，全部，１，买，２给，



			PAL_ITEMBIGTYPE=0x1172,//道具类别
			PAL_ITEMBIGTYPEID=0x1173,//道具类别ID 

			PAL_BANTIME=0x1174,//封停时间

			PAL_POINT=0x1175,//剩余元宝数量
		
			PAL_ITEMBUYCOST=0x1176,//物品买价Format:Integer

			PAL_EXPIREDFLAG=0x1177,//过期标志
			PAL_EXPIREDDATE=0x1178,//过期时间
			PAL_SELLSTATUS=0x1179,//拍卖状态
			PAL_ITEMLEVEL=0x1180,//物品等级
			PAL_TIMETYPE=0x1181,//时间类型
			//PAL_ITEMTYPE=0x1182,//物品大类
			PAL_ITEMSUBTYPE=0x1183,//物品小类
			PAL_ITEMVOC=0x1184,//道具可装备/使用职业
			PAL_ITEMEQPOS=0x1185,//道具装备位置

			PAL_GUILDID=0x1186,//帮会ID
			PAL_MONEY=0x1187,//金钱

			PAL_IDEX=0x1188,//序列号
			PAL_REWARDID=0x1189,//奖励包ID
			PAL_GETTIME=0x1190,//领取时间
			PAL_DESCRIBER=0x1191,//描述
			//PAL_BEGINTIME=0x1192,//开始时间
			//PAL_EXPIRETIME=0x1193,//到期时间
			PAL_ISGOT=0x1194,//是否领取
			PAL_GROUPID=0x1195,//组号
			PAL_9YOUID=0x1196,//9youid
			PAL_WASTEPOINT=0x1197,//消耗元宝
			PAL_LOGTYPE=0x1198,//日志类型
			PAL_CREATETIME=0x1199,//消耗时间
			PAL_USERMEMO=0x1200,//用户行为
			PAL_SYSMEMO=0x1201,//事件日志

			PAL_SERVERNAME=0x1202,//领取大区
			PAL_ACTORNAME=0x1203,//领取角色名
			PAL_ACTORLV=0x1204,//领取角色等级

			PAL_CENDSELL=0x1205,//文钱卖出价格
			PAL_CENTBUY=0x1206,//文钱购买价格

			PAL_UserMailBox = 0x1207,//玩家邮箱
			PAL_PETNAME=0x1208,//宠物名称
			PAL_SECONDPW=0x1209,//二级密码

			PAL_GMSERVERIP=0x1210,//GMServerIPFormat:String
			PAL_GMSERVERIPNAME=0x1211,//GMServerIPNameFormat:String(通过这两个确定worldid)

			PAL_MAILID=0x1212,//邮件ID
			PAL_Email=0x1213,//邮件地址
			
			PAL_MurdererCount=0x1214,//邪气值
			PAL_EvilCount=0x1215,//恶业值
			PAL_Wushu=0x1216,//武术
			PAL_Constitution=0x1217,//体质
			PAL_Spirit=0x1218,//灵性
			PAL_Quality=0x1219,//灵耐
			PAL_Dexterity=0x1220,//身法
			PAL_Fate=0x1221,//吉运

			PAL_TitleName0=0x1222,//称号0
			PAL_TitleName1=0x1223,//称号1
			PAL_TitleName2=0x1224,//称号2
			PAL_TitleName3=0x1225,//称号3
			PAL_TitleName4=0x1226,//称号4
			PAL_TitleName5=0x1227,//称号5
			PAL_TitleName6=0x1228,//称号6
			PAL_TitleName7=0x1229,//称号7
			PAL_TitleName8=0x1230,//称号8
			PAL_TitleName9=0x1231,//称号9
			PAL_TitleName10=0x1232,//称号10
			PAL_TitleName11=0x1233,//称号11
			PAL_TitleName12=0x1234,//称号12
			PAL_TitleName13=0x1235,//称号13
			PAL_TitleName14=0x1236,//称号14


			PAL_BlackList0=0x1237,//黑名单0
			PAL_BlackList1=0x1238,//黑名单1
			PAL_BlackList2=0x1239,//黑名单2
			PAL_BlackList3=0x1240,//黑名单3
			PAL_BlackList4=0x1241,//黑名单4
			PAL_BlackList5=0x1242,//黑名单5
			PAL_BlackList6=0x1243,//黑名单6
			PAL_BlackList7=0x1244,//黑名单7
			PAL_BlackList8=0x1245,//黑名单8
			PAL_BlackList9=0x1246,//黑名单9
			PAL_BlackList10=0x1247,//黑名单10
			PAL_BlackList11=0x1248,//黑名单11
			PAL_BlackList12=0x1249,//黑名单12
			PAL_BlackList13=0x1250,//黑名单13
			PAL_BlackList14=0x1251,//黑名单14

			PAL_PlotName0=0x1252,//猎户声望值(停用)
			PAL_PlotName1=0x1253,//信差声望值(停用)
			PAL_PlotName2=0x1254,//竞技声望
			PAL_PlotName3=0x1255,//商家声望值(停用)
			PAL_PlotName4=0x1256,//魔缘累计值
			PAL_PlotName5=0x1257,//魔缘现有值
			PAL_PlotName6=0x1258,//装备知识
			PAL_PlotName7=0x1259,//才能知识
			PAL_PlotName8=0x1260,//伏魔熟练度
			PAL_PlotName9=0x1261,//武器使用熟练度
			PAL_PlotName10=0x1262,//技能使用熟练度
			PAL_PlotName11=0x1263,//冲装熟练度
			PAL_PlotName12=0x1264,//仙缘累计值
			PAL_PlotName13=0x1265,//仙缘现有值
			PAL_PlotName14=0x1266,//仙缘今日获得
			PAL_PlotName15=0x1267,//仙缘今日增加上限
			PAL_PlotName16=0x1268,//尘缘累计值
			PAL_PlotName17=0x1269,//尘缘现有值
			PAL_PlotName18=0x1270,//魔缘今日获得
			PAL_PlotName19=0x1271,//魔缘今日增加上限
			PAL_SKILLNAME=0x1272,//技能
			PAL_MobName=0x1273,//伏魔名字
			PAL_MobID=0x1274,//伏魔ID
			
			PAL_PuzzleMap=0x1275,//图鉴名
			PAL_PuzzleMapID1=0x1276,//图鉴ID1
			PAL_PuzzleMapID2=0x1277,//图鉴ID2
			PAL_PuzzleMapID3=0x1278,//图鉴ID3
			PAL_PuzzleMapID4=0x1279,//图鉴ID4
			PAL_PuzzleMapID5=0x1280,//图鉴ID5
			PAL_PuzzleMapID6=0x1281,//图鉴ID6
			PAL_PuzzleMapID7=0x1282,//图鉴ID7
			PAL_PuzzleMapID8=0x1283,//图鉴ID8
			PAL_PuzzleMapID9=0x1284,//图鉴ID9
			PAL_MapID=0x1285,//地图ID
			PAL_Map=0x1286,//地图名	

			PAL_CustomIterID=0x1287,//客制物品ID
			PAL_CustomIter=0x1288,//客制物品名
			PAL_BindSetup=0x1289,//绑定状态(1:设为绑定 0:维持原样版设定)

			//GTown
			GT_ServerIP = 0x2001,   //服务器IP
			GT_ServerName = 0x2002, //服务器名称
			GT_UserID = 0x2003,     //玩家ID
			GT_Account = 0x2004,    //玩家帐号
			GT_UserNick = 0x2005,   //玩家昵称

			GT_StartTime = 0x2006,    //开始时间
			GT_EndTime = 0x2007,   //结束时间

			GT_Sex = 0x2008,//性别
			GT_CurPosition = 0x2009,//当前位置
			GT_GCash = 0x2010,//G币
			GT_MCash = 0x2011,//M币
			GT_TotalPoint = 0x2012,//积分数量
			GT_IsOnline = 0x2013,//是否在线

			GT_Physical = 0x2014,//体力值
			GT_Smart = 0x2015,//脑力值
			GT_Financial = 0x2016,//财力值
			GT_Charm = 0x2017,//魅力值
			GT_Title = 0x2018,//称号
			GT_HouseNum = 0x2019,//房屋数量
			GT_InFamily = 0x2020,//所在家族
			GT_FriendNum = 0x2021,//好友数量
			GT_HaveInamorato = 0x2022,//是否有情侣
			GT_IsMarried = 0x2023,//是否结婚

			GT_IsShaikh = 0x2024,//是否族长
			GT_InamoratoNick = 0x2025,//情侣昵称
			GT_LoverNick = 0x2026,//对方角色昵称

			GT_Interval = 0x2027,//时间间隔
			GT_IsImmediate = 0x2028,//是否立即发送
			GT_BoardMessage = 0x2029,//公告内容

			GT_BanReason = 0x2030,//封停原因
			GT_UnbindReason = 0x2031,//解封原因

			GT_NoticeID = 0x2032,//公告ID
			GT_UserState = 0x2033,//用户状态
			GT_FamilyID = 0x2034,//家族ID

			GT_Type = 0x2035,//类型
			GT_ZoneID = 0x2036,//大区ID			
			GT_MID = 0x2037,//商品ID
			GT_TimesLimit = 0x2038,//使用次数
			GT_DaysLimit = 0x2039,//使用天数

			GT_RoomName = 0x2040,//房屋名称
			GT_RoomPos = 0x2041,//所在坐标		
			GT_decorationName = 0x2042,//房屋内装饰品名称
			GT_RoomLimit = 0x2043,//到期时间

			GT_FamilyName = 0x2044,//家族名称
			GT_FamilyCreate = 0x2045,//创建时间
			GT_MemberNum = 0x2046,//成员人数	
			GT_FamilyLeader = 0x2047,//族长名称
			GT_FamilyLeaderID = 0x2048,//族长ID
			GT_MemberName = 0x2049,//成员名称
			GT_JoinTime = 0x2050,//加入时间

			GT_PetName = 0x2051,//宠物名称
			GT_PetPos = 0x2052,//当前位置
			GT_PetLimit = 0x2053,//到期时间

			GT_ItemID = 0x2054,//道具ID
			GT_ItemName = 0x2055,//道具名称
			GT_IsBlueIdent = 0x2056,//是否蓝标
			GT_Limit = 0x2057,//到期时间

			GT_FriendName = 0x2058,//好友名称
			GT_BlackFName = 0x2059,//黑友名称
			GT_DarkFName = 0x2060,//暗友名称

			GT_OpenTime = 0x2061,//开启时间
			GT_GetReward = 0x2062,//获得奖励

			GT_FamilyDuty= 0x2063,//家族职务
			GT_BanTime= 0x2064,//封停时间

			GT_SenderNick = 0x2065,//发送方昵称
			GT_ReceiverNick = 0x2066,//接收方昵称			
			GT_TradeTime= 0x2067,//交易时间
			GT_TradeItemID= 0x2068,//交易物品ID
			GT_TradeItemName= 0x2069,//物品名称
			GT_TradeItemNum= 0x2070,//物品数量

			GT_MailTime= 0x2071,//发送邮件时间
			GT_MailContent= 0x2072,//邮件内容

			GT_TaskName= 0x2073,//任务名称
			GT_TaskTime= 0x2074,//完成时间
			GT_TaskReward= 0x2075,//任务完成奖励1

			GT_UseItemName= 0x2076,//使用道具名称
			GT_UseItemNum= 0x2077,//使用道具数量
			GT_UseItemTime= 0x2078,//使用道具时间

			GT_MailTitle= 0x2079,//邮件标题

			GT_OnlineTime = 0x2080,//在线时长
			GT_RewardItemName = 0x2081,//获得奖励道具名称
			GT_RewardGetTime = 0x2082,//获得时间

			GT_SenderAccount = 0x2083,//发送帐号
			GT_SendTime = 0x2084,//发送时间
			GT_SendContent = 0x2085,//发送内容

			GT_LoginTime = 0x2086,//登录时间
			GT_LogoutTime = 0x2087,//登出时间
			GT_UserIP = 0x2088,//玩家IP

			GT_SendNick = 0x2089,//发送昵称

			GT_TreasureBox_Type = 0x2090,//宝箱类型

			GT_TaskReward2= 0x2091,//任务完成奖励2
			GT_ChooseReward= 0x2092,//任务可选奖励

			GT_BuyTime = 0x2093,//购买时间
			GT_TimeLimit = 0x2094,//使用期限

			GT_ChargeTime = 0x2095,//充值到账时间
			GT_ChargeMoney = 0x2096,//充值到账金额

			GT_Money=0x2097,//金钱
			GT_Resume=0x2098,//个性签名

			GT_Card=0x2099,//名片数

			GT_Male=0x2100,//男方名
			GT_Female=0x2101,//女方名
			GT_MarryPeople=0x2102,//婚礼人数
			GT_Pktimes=0x2103,//Pk数
			GT_Win=0x2104,//胜利数
			GT_Cowin=0x2105,//失败数
			GT_Times=0x2106,//次数

			GT_Lustful=0x2107,//花痴度
			GT_Fanta=0x2108,//迷人度
			GT_Allexp=0x2109,//综合指数


			GT_SlaveCount=0x2110,//奴隶数
			GT_SlaveSum=0x2111,//总资产
			GT_Coin=0x2112,//现金
			GT_SlaveID=0x2113,//奴隶ID
			GT_SlaveAccount=0x2114,//奴隶名称
			GT_SlaveNick=0x2115,//奴隶昵称
			GT_Price=0x2116,//身价
			GT_IsSell=0x2117,//是否被买卖
			GT_DateRecord=0x2118,//发生时间
			GT_Discount=0x2110,//打折数
			GT_X=0x2120,
			GT_Y=0x2121,
			GT_Z=0x2122,

			GT_JW2Name=0x2123,//对应JW2大区名
			GT_ID=0x2124,//序号




			/// <summary>
			/// 劲舞团2定义
			/// </summary>
			JW2_ACCOUNT = 0x3001,//玩家帐号 Format:String
			JW2_UserNick = 0x3002,//玩家呢称 Format:String
			JW2_ServerName=0x3003,//服务器名称
			JW2_ServerIP = 0x3004,//游戏服务器 Format:String
			JW2_State = 0x3005,//玩家状态 Format:Integer
			JW2_Reason = 0x3006,//封停理由 Format:String
			JW2_BanDate = 0x3007,//封停日期 Format:dataTime
			JW2_UserSN = 0x3008, //Format:Integer 用户序列号
			JW2_GSServerIP = 0x3009,//星球 Format:String
			JW2_UserID = 0x3010, //Format:String 玩家ID
			JW2_Sex = 0x3011,//玩家性别 Format:Integer
			JW2_AvatarItem = 0x3012, //Format:Integer
			JW2_ExpireDate = 0x3013,//Format:TimesStamp  过期日期
			JW2_Exp = 0x3014, //Format:Integer 玩家经验
			JW2_Money = 0x3015, //Format:Integer G币
			JW2_Cash = 0x3016, //Format:Integer 现金
			JW2_Level = 0x3017, //Format:Integer 等级
			JW2_UseItem = 0x3018,//Format:Integer是否在使用中，1用0不用
			JW2_RemainCount = 0x3019,//Format:Integer剩余次数，0为无限次
			JW2_BeginTime = 0x3020,//开始时间Format:DATE
			JW2_EndTime = 0x3021,//结束时间Format:DATE
			JW2_BoardMessage = 0x3022,//公告内容,喇叭发送内容Format:String
			JW2_TaskID = 0x3023,//任务号Format:Integer
			JW2_Status = 0x3024,//是否完成状态Format:Integer
			JW2_Interval = 0x3025,//间隔时间Format:Integer
			JW2_UseTime = 0x3026,//使用时间 TLV_DATE
			JW2_POWER=0x3027,//权限，普通用户是0，管理员为1 Format:Integer
			JW2_GoldMedal = 0x3028,//金牌 Format:Integer
			JW2_SilverMedal = 0x3029,//银牌 Format:Integer
			JW2_CopperMedal = 0x3030,//铜牌 Format:Integer
			JW2_Region=0x3031,//地区 Format:String
			JW2_QQ=0x3032,//QQ号 Format:String
			JW2_PARA=0x3033,//结婚任务用的一个参数 Format:Integer
			JW2_ATONCE=0x3034,//是否立即发送Format:Integer
			JW2_BOARDSN=0x3035,//大小喇叭，横幅记录唯一标示Format:Integer
			JW2_BUGLETYPE=0x3036,//类型0mb发小喇叭,1积分发小喇叭,1mb发大喇叭,11积分发大喇叭,20及20以上是横幅Format:Integer
			//情节///////////
			JW2_Chapter=0x3037,//章节
			JW2_CurStage=0x3038,//目前等级
			JW2_MaxStage=0x3039,//最大等级
			JW2_Stage0=0x3040,
			JW2_Stage1=0x3041,
			JW2_Stage2=0x3042,
			JW2_Stage3=0x3043,
			JW2_Stage4=0x3044,
			JW2_Stage5=0x3045,
			JW2_Stage6=0x3046,
			JW2_Stage7=0x3047,
			JW2_Stage8=0x3048,
			JW2_Stage9=0x3049,
			JW2_Stage10=0x3050,
			JW2_Stage11=0x3051,
			JW2_Stage12=0x3052,
			JW2_Stage13=0x3053,
			JW2_Stage14=0x3054,
			JW2_Stage15=0x3055,
			JW2_Stage16=0x3056,
			JW2_Stage17=0x3057,
			JW2_Stage18=0x3058,
			JW2_Stage19=0x3059,
			//情节end///////////
			JW2_BUYSN=0x3060,//购买SN
			JW2_GOODSTYPE=0x3061,//购买类型
			JW2_RECESN=0x3062,//接受人的SN（如果相同是本人）
			JW2_GOODSINDEX=0x3063,//物品编号
			JW2_BUYDATE=0x3064,//购买日期
			JW2_RECEID=0x3065,//接受人的ID（如果相同是本人）
			JW2_AvatarItemName = 0x3066, //道具名称 Format:String

			JW2_MALESN=0x3067,//男性SN
			JW2_MALEUSERNAME=0x3068,//男性用户名
			JW2_MALEUSERNICK=0x3069,//男性昵称
			JW2_FEMAILESN=0x3070,//女性SN
			JW2_FEMALEUSERNAME=0x3071,//女性用户名
			JW2_FEMAILEUSERNICK=0x3072,//女性昵称
			JW2_WEDDINGDATE=0x3073,//结婚时间
			JW2_UNWEDDINGDATE=0x3074,//离婚时间
			JW2_WEDDINGNAME=0x3075,//婚礼名称
			JW2_WEDDINGVOW=0x3076,//婚礼誓言
			JW2_RINGLEVEL=0x3077,//戒指等级
			JW2_REDHEARTNUM=0x3078,//红心数量
			JW2_WEDDINGNO=0x3079,//婚姻序号
			JW2_CONFIRMSN=0x3080,//见证者SN
			JW2_CONFIRMUSERNAME=0x3081,//见证者用户名
			JW2_CONFIRMUSERNICK=0x3082,//见证者昵称
			JW2_COUPLEDATE=0x3083,//情侣日期
			JW2_KISSNUM=0x3084,//kiss次数
			JW2_LASTKISSDATE=0x3085,//最后一次Kiss时间
			JW2_BREAKDATE=0x3088,//分手时间
			JW2_CMBREAKDATE=0x3089,//确认分手时间
			JW2_BREAKSN=0x3090,//提出SN
			JW2_BREAKUSERNAME=0x3091,//提出用户名
			JW2_BREAKUSERNICK=0x3092,//提出昵称

			JW2_LASTLOGINDATE=0x3093,//最后登陆时间
			JW2_REGISTDATE=0x3094,//激活时间
			JW2_FCMSTATUS=0x3095,//防沉迷状态

			JW2_FAMILYID=0x3096,//家族ID
			JW2_FAMILYNAME=0x3097,//家族名称
			JW2_DUTYID=0x3098,//职业号
			JW2_DUTYNAME=0x3099,//职业名
			JW2_ATTENDTIME=0x3100,//加入时间
			JW2_COUPLESN=0x3101,//情侣序号
			JW2_CREATETIME=0x3102,//创建时间
			JW2_CNT=0x3103,//人数
			JW2_POINT=0x3104,//积分
			JW2_LOGINTYPE=0x3105,//类型0登入，1登出
			JW2_TIME=0x3106,//时间
			JW2_IP=0x3107,//IP地址
			JW2_PWD=0x3108,//密码
			JW2_AvatarItemType=0x3109,//物品类型（头发等）
			JW2_ItemPos=0x3110,//物品位置(0,身上,1,物品栏,2,礼物栏)

			JW2_MailTitle=0x3111,//邮件主题
			JW2_MailContent=0x3112,//邮件内容
			JW2_ItemNo=0x3113,//物品序号

			JW2_Repute=0x3115,//声望
			JW2_NowTitle=0x3116,//称号

			JW2_FamilyLevel=0x3117,//等级
			JW2_AttendRank=0x3118,//人气排名
			JW2_MoneyRank=0x3119,//财富排名
			JW2_PowerRank=0x3120,//实力排名
			
			
			JW2_ItemCode=0x3121,//道具ID
			JW2_ItemName=0x3122,//道具名称
			JW2_Productid=0x3123,//商品ID
			JW2_ProductName=0x3124,//商品名称
			
			JW2_FamilyPoint=0x3125,//家族点数
			
			JW2_PetSn=0x3126,//宠物ID
			JW2_PetName=0x3127,//宠物名称
			JW2_PetNick=0x3128,//宠物自定义名称
			JW2_PetLevel=0x3129,//宠物等级
			JW2_PetExp=0x3130,//宠物经验
			JW2_PetEvol=0x3131,//进阶阶段
			
			
			JW2_MailSn=0x3132,//序列号
			JW2_SendNick=0x3133,//发件人昵称
			JW2_SendDate=0x3134,//发送日期
			
			JW2_Num=0x3135,//数量
			
			JW2_Rate=0x3136,//男女比例
			JW2_ShaikhNick=0x3137,//族长名称
			JW2_SubFamilyName=0x3138,//隶属家族名称
			
			JW2_MoneyType=0x3139,//消费类型
			JW2_BeforeCash=0x3140,//消费前钱
			JW2_AfterCash=0x3141,//消费后钱

			//maple add
			JW2_ProductPrice=0x3142,//道具单价
			JW2_TotalPrice=0x3143,//总额



			WA_ServerIP=0x4002,//服务器IP
			WA_Account=0x4003,//帐号
			WA_UserNick=0x4004,//昵称
			WA_BeginTime=0x4005,//开始时间
			WA_EndTime=0x4006,//结束时间
			WA_AccountID=0x4007,//帐号ID
			WA_NickID=0x4008,//角色ID
			WA_Time=0x4009,//时间
			WA_RealmId=0x4010,//大区ID
			WA_ClusterID=0x4011,//服务组ID
			WA_TitleName1=0x4012,//角色称谓1
			WA_TitleName2=0x4013,//角色称谓2
			WA_CharType=0x4014,//角色类型
			WA_Profession=0x4015,//角色职业
			WA_Sex=0x4016,//角色性别
			WA_IsOnline=0x4017,//是否在线
			WA_IsClosed=0x4018,//是否封停
			WA_Level=0x4019,//角色等级 
			WA_Money=0x4020,//游戏币数量
			WA_NineyouMoney=0x4021,//9游币数量
			WA_ReserveMoney=0x4022,//储备金
			WA_CurSceneID=0x4023,//当前所在场景ID
			WA_CurSceneName=0x4024,//当前所在场景名称
			WA_CurScenePos=0x4025,//当前所在场景坐标
			WA_Hp=0x4026,//生命值
			WA_Mp=0x4027,//魔法值
			WA_Vitality=0x4028,//活力值
			WA_GoodEvil=0x4029,//善恶值
			WA_PhysicalAttack=0x4030,//物理攻击
			WA_PhysicalDefense=0x4031,//物理防御
			WA_MagicAttack=0x4032,//魔法攻击
			WA_MagicDefense=0x4033,//魔法防御
			WA_Critical=0x4034,//暴击率
			WA_Strength=0x4035,//力量
			WA_Dexterity=0x4036,//敏捷
			WA_Intellect=0x4037,//智力
			WA_Corporeity=0x4038,//体质
			WA_PotentialPoint=0x4039,//潜力
			WA_Exp=0x4040,//经验
			WA_ExpNextLevel=0x4041,//升级经验,
			WA_PhysicalResist=0x4042,//物理抗性
			WA_MagicResist=0x4043,//法术抗性
			WA_FireResist=0x4044,//火焰抗性
			WA_ColdResist=0x4045,//冰冻抗性
			WA_PoisonResist=0x4046,//毒素抗性
			WA_Reason=0x4047,//原因
			WA_RemainTime=0x4048,//剩余时间
			WA_TaskID=0x4049,//公告ID
			WA_BoardMessage=0x4050,//公告内容
			WA_Interval=0x4051,//间隔时间
			WA_ServerName=0x4052,//服务器名称
			WA_Status=0x4053,//状态
			WA_GSServerIP=0x4054,//GS服务器

			WA_ItemLocation=0x4055,//道具存放位置
			WA_friendIndex=0x4056,// 好友角色ID
			WA_friendName=0x4057,// 好友名字
			WA_friendTypeId=0x4058,// 好友的角色类型
			WA_friendLevel=0x4059,// 好友等级
			WA_friendIni=0x4060,// 新密度
			WA_itemIndex=0x4061,// 序号
			WA_itemTypeId=0x4062,// 类型ID
			WA_itemName=0x4063,// 名字
			WA_itemUniqueId=0x4064,	// 唯一ID
			WA_itemCount=0x4065,// 数量
			WA_itemDur=0x4066,// 耐久
			
			WA_medalTypeId=0x4067,	// 类型ID
			WA_medalName=0x4068,	//徽章名字
			WA_medalDur=0x4069,		// 耐久
			WA_medalBringUp=0x4070,	// 培差度
			WA_medalPetIndex=0x4071,// 宠物序号超所值255为无宠物
			WA_medalPos=0x4072,		// 徽章在宠物背包中位置 255为装备中


			WA_petTypeId=0x4073,		// 类型ID
			WA_petName=0x4074,			// 宠物名字
			WA_petLevel=0x4075,			// 等级
			WA_petNature=0x4076,		// 性格
			WA_petGrowth=0x4077,		// 成长率
			WA_healthe=0x4078,			// 健康度

			WA_FireAttack=0x4079,		//火焰攻击
			WA_ColdAttack=0x4080,		//冰冻攻击
			WA_PoisonAttack=0x4081,		//毒素攻击

			WA_Description=0x4082,		//封停类型

			WA_LastLoginTime=0x4083,	//上次登录时间
			WA_EnableTime=0x4084,	//上次登录时间

			WA_ItemPos=0x4085,	//道具位置
			WA_IsDelete=0x4086,	// 是否被删除

			//add by ltx
			WA_SrcCharId=0x4087, //来源角色ID
			WA_SrcCharName=0x4088, //来源角色名字
			WA_DateTime=0x4089, //交易时间
			WA_DstCharId=0x4090, //目标角色ID
			WA_DstCharName=0x4091, //目标角色名字

			WA_BindReward1=0x4092, //绑定奖励1
			WA_BindRewardValue1=0x4093, //绑定奖励数值1
			WA_BindReward2=0x4094, //绑定奖励1
			WA_BindRewardValue2=0x4095, //绑定奖励数值1

			WA_ConsumeItemTypeId=0x4096, //解绑消耗物品ID
			WA_BoothType=0x4097, //操作类型
			WA_ItemTimeLimit=0x4098, //物品的使用期限

			WA_CraftType=0x4099, //装备制作类型
			WA_RecipeId=0x4100, //图纸ID
			WA_MaterialId1=0x4101, //材料ID
			WA_MaterialId2=0x4102, //材料ID
			WA_MaterialId3=0x4103, //材料ID
			WA_MaterialId4=0x4104, //材料ID
			WA_MaterialId5=0x4105, //材料ID
			WA_MaterialId6=0x4106, //材料ID
			WA_ProductId=0x4107, //产品ID
			WA_ProductName=0x4108, //产品名字

			WA_ConsumeHammer=0x4109, //消耗铁锤
			WA_ConsumeStithy=0x4110, //消耗铁砧
			WA_ConsumeMoney=0x4111, //消耗金钱

			WA_OldColor=0x4112, //改孔前颜色
			WA_NewColor=0x4113, //改孔后颜色

			WA_OldStarCount=0x4114, //升星前星数
			WA_NewStarCount=0x4115, //升星后星数

			WA_GemOperation=0x4116, //宝石操作
			WA_GemType=0x4117, //宝石类型
			WA_GemLevel=0x4118, //宝石类型

			WA_MaterialId=0x4119, //原材料ID
			WA_MaterialName=0x4120, //原材料名字
			WA_PropItemId=0x4121, //成功率道具

			WA_ChangeType=0x4122, //兑换类型
			WA_ConsumeIntegral=0x4123, //消耗积分
			WA_ChangePoint=0x4124, //得到的经验或是钱

			WA_GetMoney=0x4125, //获得金钱数量
			WA_SrcId=0x4126, //来源ID
			WA_SrcName=0x4127, //来源名字

			WA_LoseMoney=0x4128, //失去金钱数量
			WA_DstId=0x4129, //目标ID
			WA_DstName=0x4130, //目标名字

			WA_QuestId=0x4131, //任务ID
			WA_QuestName=0x4132, //任务名字
			WA_RewardMoney=0x4133, //奖励的金钱
			WA_RewardItemId=0x4134, //奖励的物品
			WA_RewardItemName=0x4135, //奖励的物品名字
			WA_GiveItemId=0x4136, //失去的物品ID
			WA_GiveItemName=0x4137, //失去的物品名字

			WA_MailId=0x4138, //邮件ID
			WA_MailMoney=0x4139, //金钱数量
			
			WA_Item1=0x4140, //物品1
			WA_Item2=0x4141, //物品2
			WA_Item3=0x4142, //物品3
			WA_Item4=0x4143, //物品4
			WA_Item5=0x4144, //物品5
			WA_Item1Count=0x4145, //物品1数量
			WA_Item2Count=0x4146, //物品2数量
			WA_Item3Count=0x4147, //物品3数量
			WA_Item4Count=0x4148, //物品4数量
			WA_Item5Count=0x4149, //物品5数量
			WA_MailTitle=0x4150, //邮件标题

			WA_OpType=0x4151, // 类型
			WA_InstanceId=0x4152, //副本ID
			WA_InstanceName=0x4153, //副本名字
			WA_USERTYPE = 0x4154,//用户类型1、赠送人 2受赠人
			WA_SkillName = 0x4155,//技能名称
			WA_NewPasswd = 0x4156,//新密码
			WA_ItemID = 0x4157,//道具ID
			WA_TYPE =  0x4158,//1、修改等级 2、修改金钱
			WA_NewRole = 0x4159,// 新的角色名
			WA_SkillID = 0x4160,//技能ID
			WA_SkillLvl = 0x4161,//技能等级
			WA_cluster_id = 0x4162,//组ID
			WA_realm_id = 0x4163,//领域ID
			WA_ItemType = 0x4164,//道具类型 中文
			WA_MailorName = 0x4165,//发件人名称
			WA_ItemPrice = 0x4166,//物品价格
			WA_UNBindRewardValue1  =0x4167,//解绑的获得奖励1
			WA_UNBindRewardValue2  =0x4168,//解绑的获得奖励2
			WA_GetTime  =0x4169,//获得时间//升级时间

			//Added by tengjie 2010-06-12
			WA_Content=0x4170,//封停描述
			WA_GuildName = 0x4171,//公会名称
			WA_NewProfession=0x4172,//转职后职业
			WA_ConsumeItem=0x4173,//消耗物品
//			WA_PetTypeID=0x4174,//宠物类型/唯一ID
			WA_NewGuildName = 0x4174,//新公会名称
			WA_GuildLevel = 0x4175,//公会等级
			WA_GuildMemCount = 0x4176,//公会人数
			WA_RecipeName=0x4177, //图纸名称
			WA_MaterialName1=0x4178, //材料名称1
			WA_MaterialName2=0x4179, //材料名称2
			WA_MaterialName3=0x4180, //材料名称3
			WA_MaterialName4=0x4181, //材料名称4
			WA_MaterialName5=0x4182, //材料名称5
			WA_MaterialName6=0x4183, //材料名称6
			WA_ItemNum=0x4184, //道具数量
			WA_LoginIP=0x4185, //登陆IP
			WA_MemLevel=0x4186, //职级
			WA_MailTag=0x4187, //是否群发邮件
			WA_ItemGen=0x4188, //道具品质
			WA_MailSubject=0x4189, //邮件标题
			WA_MailContent=0x4190, //邮件正文
			WA_SendMoney=0x4191, //发送金钱
			WA_ItemExpire=0x4192, //物品时效
			WA_GuildID=0x4193, //公会ID
			WA_PetUniqId=0x4194, //宠物唯一ID
			WA_CurMoney=0x4195,//当前游戏币数量

			/// <summary>
            /// 魔力宝贝2
            /// </summary>
            CG2_UserName = 0x5000,//用户名
            CG2_UserID = 0x5001,//角色IDKEY
            CG2_CharacterNum = 0x5002,//角色号
            CG2_Level = 0x5003,//等级
            CG2_Exp = 0x5004,// 经验值
            CG2_Sex = 0x5005,// 性别
            CG2_Money = 0x5006,// 金钱
            CG2_Profession = 0x5007,//职业
            CG2_Guild = 0x5008,//所属公会
            CG2_CreateTime = 0x5009,//角色创建时间
            CG2_IsOnline = 0x5010,//是否在线
            CG2_Blood = 0x5011,//血值
            CG2_Magic = 0x5012,//魔力值
            CG2_MapID = 0x5013,//地图ID
            CG2_Coordinate = 0x5014,//坐标
            CG2_Injured = 0x5015,//受伤值
            CG2_Status = 0x5016,//掉魂(状态)
            CG2_ProfessionLevel = 0x5017,//职业等级
            CG2_Honor = 0x5018,//荣誉值
            CG2_SpecialName = 0x5019,//特别称号
            CG2_Charm = 0x5020,//魅力值
            CG2_InPrison = 0x5021,//是否关监狱(状态)
            CG2_CharacterName = 0x5022,//角色名称
            CG2_OwnName = 0x5023,//玩家自设称号
            CG2_Fight = 0x5024,//战绩
            CG2_LeftPoint = 0x5025,//剩余点数
            CG2_Physical = 0x5026,//体力
            CG2_Power = 0x5027,//力量
            CG2_Smart = 0x5028,//灵巧
            CG2_Speed = 0x5029,//速度
            CG2_Spirit = 0x5030,//精神
            CG2_MaxBlood = 0x5031,//最大血值
            CG2_MaxMag = 0x5032,//最大魔值
            CG2_Attack = 0x5033,//攻击
            CG2_Defend = 0x5034,//防御
            CG2_MagAttack = 0x5035,//魔攻
            CG2_MagDefend = 0x5036,//魔防
            CG2_Recover = 0x5037,//回复
            CG2_Hit = 0x5038,//命中
            CG2_Avoid = 0x5039,//回避
            CG2_Critical = 0x5040,//暴击
            CG2_Retort = 0x5041,//反击
            CG2_isChairman = 0x5042,//是否会长
            CG2_ProHonorRank = 0x5043,//职业荣誉称号
            CG2_tempRank = 0x5044,//临时称号
            CG2_RankID = 0x5045,//称号ID
            CG2_FriendIDKey = 0x5046,//对方IDKey
            CG2_FriendCharNum = 0x5047,// 好友名称
            CG2_FriendShip = 0x5048,//友好度
            CG2_SlotNum = 0x5049,//卡槽号
            CG2_SkillName = 0x5050,//技能名称
            CG2_SkillLevel = 0x5051,//技能等级/////////maple
            CG2_SkillExp = 0x5052,//技能经验

            CG2_EquipNum = 0x5053,//装备号
            CG2_ItemColNum = 0x5054,//物品栏号
            CG2_CombineNum = 0x5055,//合并放置数
            CG2_ItemName = 0x5056,//物品名称
            CG2_QualityLevel = 0x5057,//质量等级
            CG2_CurEndure = 0x5058,//当前耐久
            CG2_MaxEndure = 0x5059,//最大耐久
            CG2_FixTimes = 0x5060,//修理次数
            CG2_ComposeTimes = 0x5061,//合成次数
            CG2_ComposeItemID = 0x5062,//合成时取得物品名的ID
            CG2_StorageMoney = 0x5063,//存储金钱数

            CG2_Serverip = 0x5064, //IP地址
            CG2_nickname = 0x5065, //昵称

            CG2_JobID = 0x5067, //职业ID
            CG2_GuildID = 0x5068, //公会ID
            CG2_ProHonorRankID = 0x5069,//职业荣誉称号ID

            CG2_logtime = 0x5070,//timestamp log时间
            CG2_category = 0x5071,//日志大类
            CG2_action = 0x5072,//日志小类
            CG2_targetName = 0x5073,//目标角色名
            CG2_targetIDKey = 0x5074,//目标ID
            CG2_mgvalue = 0x5075,//日志信息
            CG2_mgstring = 0x5076,//日志内容

            CG2_PetID = 0x5077,//宠物id
            CG2_PetStatus = 0x5078,//宠物状态
            CG2_PetExp = 0x5079,//经验
            CG2_PetBlood = 0x5080,//血值
            CG2_PetMag = 0x5081,//魔力值
            CG2_PetInjured = 0x5082,//受伤值
            CG2_PetLostSoul = 0x5083,//掉魂(状态)
            CG2_PetRace = 0x5084,//种族
            CG2_PetMaxSkillCol = 0x5085,//最大技能栏
            CG2_PetSrcName = 0x5086,//宠物原名
            CG2_PetNewName = 0x5087,//玩家自设宠物名称
            CG2_PetFaith = 0x5088,//忠诚度
            CG2_PetLeftPoint = 0x5089,//剩余点数
            CG2_PetPhysical = 0x5090,//体力
            CG2_PetPower = 0x5091,//力量
            CG2_PetSmart = 0x5092,//灵巧
            CG2_PetSpeed = 0x5093,//速度
            CG2_PetSpirit = 0x5094,//精神
            CG2_PetMaxBlood = 0x5095,//最大血值
            CG2_PetMaxMag = 0x5096,//最大魔值
            CG2_PetAttack = 0x5097,//攻击
            CG2_PetDefend = 0x5098,//防御
            CG2_PetCheesy = 0x5099,//俊敏
            CG2_PetMagAttack = 0x5100,//魔攻
            CG2_PetMagDefend = 0x5101,//魔防
            CG2_PetRecover = 0x5102,//回复
            CG2_PetHit = 0x5103,//命中
            CG2_PetAvoid = 0x5104,//回避
            CG2_PetCritical = 0x5105,//暴击
            CG2_PetRetort = 0x5106,//反击
            CG2_PetEvolveTimes = 0x5107,//进化次数
            CG2_PetSatisfLvl = 0x5108,//满意度
            CG2_PetFullLvl = 0x5109,//饱食度
            CG2_PetColNum = 0x5110,//PET栏号
            CG2_PetItemCol = 0x5111,//ITEM栏
            CG2_PetItemID = 0x5112,//ItemID
            CG2_PetItemGetTime = 0x5113,//获得时间
            CG2_PetCombineNum = 0x5114,//合并放置数
            CG2_PetQulityLvl = 0x5115,//质量等级
            CG2_PetCurEndure = 0x5116,//当前耐久
            CG2_PetMaxEndure = 0x5117,//最大耐久
            CG2_PetFixTimes = 0x5118,//修理次数
            CG2_PetComposeTimes = 0x5119,//合成次数
            CG2_PetComposeItemID = 0x5120,//合成时取得物品名的ID
            CG2_PetSkillID = 0x5121,//技能ID
            CG2_PetSkillLvl = 0x5122,//技能等级
            CG2_PetSkillExp = 0x5123,//技能经验值
            CG2_PetSkillColNum = 0x5124,//技能栏号

            CG2_UserNum = 0x5125,//角色数
            CG2_LatestLoginTime = 0x5126,//最近登录时间
            CG2_LatestLogoutTime = 0x5127,//最近登出时间
            CG2_LoginIP = 0x5128,//登录IP
            CG2_isAdult = 0x5129,//是否成年
            CG2_hasLockedItem = 0x5130,//是否锁定ITEM 
            CG2_Fame = 0x5131,//荣誉值

            CG2_titleFlags1 = 0x5132, //称号1
            CG2_titleFlags2 = 0x5133, //称号2
            CG2_titleFlags3 = 0x5134, //称号3
            CG2_titleFlags4 = 0x5135, //称号4
            CG2_titleFlags5 = 0x5136, //称号5
            CG2_titleFlags6 = 0x5137, //称号6
            CG2_titleFlags7 = 0x5138, //称号7
            CG2_titleFlags8 = 0x5139, //称号8
            CG2_titleFlags9 = 0x5140, //称号9
            CG2_titleFlags10 = 0x5141, //称号10
            CG2_titleFlags11 = 0x5142, //称号11
            CG2_titleFlags12 = 0x5143, //称号12
            CG2_titleFlags13 = 0x5144, //称号13
            CG2_titleFlags14 = 0x5145, //称号14
            CG2_titleFlags15 = 0x5146, //称号15
            CG2_titleFlags16 = 0x5147, //称号16
            CG2_titleFlags17 = 0x5148, //称号17
            CG2_titleFlags18 = 0x5149, //称号18
            CG2_titleFlags19 = 0x5150, //称号19
            CG2_titleFlags20 = 0x5151, //称号20
            CG2_titleFlags21 = 0x5172, //称号21
            CG2_titleFlags22 = 0x5173, //称号22
            CG2_titleFlags23 = 0x5174, //称号23
            CG2_titleFlags24 = 0x5175, //称号24
            CG2_titleFlags25 = 0x5176, //称号25
            CG2_titleFlags26 = 0x5177, //称号26
            CG2_titleFlags27 = 0x5178, //称号27
            CG2_titleFlags28 = 0x5179, //称号28
            CG2_titleFlags29 = 0x5180, //称号29
            CG2_titleFlags30 = 0x5181, //称号30

            CG2_MapName = 0x5152,//地图
            CG2_TypeName = 0x5153,//道具类别
            CG2_TypeID = 0x5154,//道具类别ID
            CG2_CharNo = 0x5155,//角色号

            CG2_Title = 0x5156,//标题
            CG2_Content = 0x5157,//内容
            CG2_BanTime = 0x5158,//封停时间
            CG2_NoteContent = 0x5159,//封停内容
            CG2_Interval = 0x5160,//时间间隔
            CG2_StartTime = 0x5161,//开始时间
            CG2_EndTime = 0x5162,//结束时间
            CG2_NoticeID = 0x5163,//公告ID
            CG2_Times = 0x5164,//封停间隔
            CG2_Cheesy = 0x5165,//俊敏

            CG2_GuildName = 0x5166,//公会名称
            CG2_GuildNum = 0x5167,//公会成员数量
            CG2_GuildHoro = 0x5168,//公会荣誉
            CG2_GuildRank = 0x5169,//公会等级
            CG2_Master = 0x5170,//公会会长
            CG2_MasterIdkey = 0x5171,//会长ID

            CG2_Ip = 0x5683,//登陆IP
            CG2_SkillID = 0x5684,//人物技能id
            CG2_SkillType = 0x5685,//技能类型
            CG2_PetSkillName = 0x5686,//宠物技能名
            CG2_ItemNum = 0x5687,//道具数量 int

            CG2_Palette = 0x5688,//颜色 int 
            CG2_Wind = 0x5689,//风属性 int 
            CG2_Frie = 0x5690,//火属性 int 
            CG2_Water = 0x5691,//水属性 int 
            CG2_Soil = 0x5692,//地属性 int 
            CG2_userItemName = 0x5693,//自定义的道具名字 string 
            CG2_Sn = 0x5694,//激活码 string
            CG2_ServerName = 0x5695,//服务器名 string
            CG2_Date = 0x5696,//激活时间 string
            CG2_isVIP = 0x5697,//是否高级帐号 string
            CG2_isUse = 0x5698,//是否使用 string

            CG2_Email = 0x5699,//Email地址 string

            CG2_ItemPass = 0x5700,//道具锁密码 int
            CG2_TempPwd = 0x5701,//临时密码 string

            CG2_SendUser = 0x5702,//发送用户 string
            CG2_RecvUser = 0x5703,//接受用户 string
            CG2_ItemType = 0x5704,//道具类型 string
            CG2_LimitTime = 0x5705,//道具时限 string
            CG2_GetTime = 0x5706,//获得时间 string
            CG2_Pwd = 0x5707,//密码 string

            SD_UserName_Old = 0x4522,//修改前用户名 string
            CG2_From_PetID = 0x5708,//交易(送出)宠物id	INT
            CG2_To_PetID = 0x5709,//交易(得到)宠物id	INT
            CG2_From_PetName = 0x5710,//交易(送出)宠物名	string
            CG2_To_PetName = 0x5711,//交易(得到)宠物名	string

            CG2_getMoney = 0x5712,//得到金钱 int

            CG2_From_UserName = 0x5713,//交易玩家	string
            CG2_To_UserName = 0x5714,//得到玩家	string
            CG2_From_UserID = 0x5715,//交易玩家ID	int
            CG2_To_UserID = 0x5716,//得到玩家ID	int
            CG2_LeftMoney = 0x5717,//剩余点数

			//maple add for log_query
			CG2_Log_StartTime = 0x5718,//log查询开始时间
			CG2_Log_EndTime =0x5719,//log查询结束时间

			CG2_CharName = 0x5720,//角色名称
			CG2_LevelValue =0x5721,//角色等级数
			CG2_LevelResult =0x5722,//结果
			CG2_ActionTime  =0x5723,//maple add
			CG2_String =0x5724,//
			CG2_JobName=0x5725,//职业名称
			CG2_Remark=0x5726,//修改职业备注

			//new add
			CG2_iReserve00=0x5727,//爱情树上次更新时间
			CG2_BugIndex=0x5728,//生虫指数
			CG2_SoilIndex=0x5729,//土质指数
			CG2_NutrientIndex=0x5730,//养分指数
			CG2_TreeState=0x5731,//树木状况
			CG2_FruitNum=0x5732,//果实数量

			CG2_Dates=0x5733,//时间
			CG2_Marriage=0x5734,//预定人
			CG2_Spouse=0x5735,//预定人配偶
			CG2_MyIndex=0x5736,//婚礼的场次号
			CG2_IsBooking=0x5737,//是否被预定
			CG2_MarriageTime=0x5738,//场地开始时间
			CG2_Gmsv=0x5739,//所在服务器
			
			CG2_ChangeCharNo = 0x5740,//新位置号
			CG2_Object = 0x5741,//日志信息

			CG2_ID=0x5742,//序列号
			CG2_Shop_RECYCLE=0x5743,//是否兑换积分

			/// <summary>
            /// 侠盗金刚
            /// </summary>
			
			XDJG_ServerIP=0x6002,//服务器IP
			XDJG_ServerName=0x6003,//服务器名称
			
			//查询
			XDJG_SEARCHTYPE=0x6010,//查询类型
			XDJG_Account=0x6011,//帐号
			XDJG_AccountID=0x6012,//帐号ID
			XDJG_Character=0x6013,//昵称
			XDJG_CharacterID=0x6014,//角色ID
			XDJG_Time=0x6015,//时间
			XDJG_Level=0x6016,//等级
			XDJG_Exp=0x6017,//经验
			XDJG_Sex=0x6018,//性别
			XDJG_Profession=0x6019,//职业
			XDJG_SceneTypeID=0x6020,//场景类型id
			XDJG_SceneInstanceID=0x6021,//场景实例id
			XDJG_Position=0x6022,//位置
			XDJG_Money=0x6023,//金钱数
			
			//封停
			XDJG_BANREASON=0x6030,//封停原因
			XDJG_BANTIME=0x6031,//封停时间
			XDJG_BANBEGINTIME=0x6032,//封停开始时间
			XDJG_BANENDTIME=0x6033,//解封时间
			XDJG_BANTYPE=0x6034,//封停类型
			
			//公告
			XDJG_TaskID=0x6040,//公告ID
			XDJG_NoticeType=0x6041,//公告类型
			XDJG_BeginTime=0x6042,//开始时间
			XDJG_EndTime=0x6043,//结束时间
			XDJG_BoardMessage=0x6044,//公告内容
			XDJG_Interval=0x6045,//间隔时间
			XDJG_Status=0x6046,//公告状态
			
			//公告接口参数
			XDJG_BeginDate=0x6047,//开始日期
			XDJG_EndDate=0x6048,//结束日期
			XDJG_StartTime=0x6049,//公告开始时间
			XDJG_Repeat=0x6050,//重复次数
			XDJG_NextTime=0x6051,//下次公告时间
			XDJG_ExecTimes=0x6052,//执行次数
			XDJG_Onoff=0x6053,//公告开关

			//Added by tengjie 2010-07-06
			XDJG_Strength=0x6054,//力量
			XDJG_Agility=0x6055,//敏捷
			XDJG_Stamina=0x6056,//体力
			XDJG_Spirit=0x6057,//灵力			
			XDJG_Magic=0x6058,//异能
			XDJG_NearPa=0x6059,//近身物理攻击
			XDJG_FarPa=0x6060,//远程物理攻击
			XDJG_SuperAttack=0x6061,//异能攻击力
			XDJG_PerfectRate=0x6062,//会心一击率
			XDJG_StrengthCritRate=0x6063,//物理致命一击率
			XDJG_SpiritCritRate=0x6064,//异能致命一击率
			XDJG_IsOnLine=0x6065,//是否在线
			
			//防御信息:
			XDJG_Armor=0x6066, //护甲值
			XDJG_DodgeRate=0x6067,//闪躲率
			XDJG_WardRate=0x6068,//格挡率
			XDJG_ParryRate=0x6069,//招架率
			XDJG_FatalResistanceRate=0x6070,//致命抵抗率
			XDJG_FlameResistance=0x6071,//抵抗火焰
			XDJG_ThunderResistance=0x6072,//抵抗雷电
			XDJG_FrostResistance=0x6073,//抵抗冰霜
			XDJG_CorrosionResistance=0x6074,//抵抗腐蚀
			XDJG_EnergyResistance=0x6075,//抵抗能量
			XDJG_SpiritResistance=0x6076,//抵抗精神
			
			//PK属性:
			XDJG_PK=0x6077,//PK
			XDJG_GoodEvil=0x6078,//善恶
			XDJG_RedNameTime=0x6079,//红名时间
			
			//角色称号:所有称号列表
			XDJG_RoleID1=0x6080,//角色称号1
			XDJG_RoleID2=0x6081,//角色称号2
			XDJG_RoleID3=0x6082,//角色称号3
			XDJG_RoleID4=0x6083,//角色称号4
			XDJG_RoleID5=0x6084,//角色称号5
			XDJG_RoleID6=0x6085,//角色称号6
			XDJG_RoleID7=0x6086,//角色称号7
			XDJG_RoleID8=0x6087,//角色称号8
			
			//生活技能
			XDJG_LifeSkill1=0x6088,//生活技能1
			XDJG_LifeSkill2=0x6089,//生活技能2
			XDJG_LifeSkill3=0x6090,//生活技能3
			XDJG_LifeSkill4=0x6091,//生活技能4
			XDJG_LifeSkill5=0x6092,//生活技能5
			XDJG_LifeSkill6=0x6093,//生活技能6
			XDJG_LifeSkill7=0x6094,//生活技能7
			XDJG_LifeSkill8=0x6095,//生活技能8

			XDJG_SkillID=0x6096,//技能ID
			XDJG_ItemID=0x6097,//道具ID
			XDJG_ItemNum=0x6098,//道具数量

			XDJG_QueryIP=0x6099,//按IP查询
			XDJG_QueryType=0x6100,//查询类型(获赠人、购买人)
			XDJG_LoginType=0x6101,//登录or登出
			XDJG_LogTime=0x6102,//日志时间
			XDJG_LogInOutIP=0x6103,//登录登出IP
			XDJG_ItemName=0x6104,//道具名称
			XDJG_TempItemID=0x6105,//临时道具ID
			XDJG_AccountBuyer=0x6106,//购买人帐号
			XDJG_RoleBuyer=0x6107,//购买人昵称
			XDJG_AccountReceiver=0x6108,//获赠人帐号
			XDJG_RoleReceiver=0x6109,//获赠人昵称
			XDJG_ItemType=0x6110,//物品类型
			XDJG_CharacterFrom=0x6111,//来源名字
			XDJG_TempItemId=0x6112,//道具临时ID
			XDJG_MoneyType=0x6113,//金钱类型
			XDJG_Property=0x6114,//所有权(二进制)
			XDJG_Cooldowns=0x6115,//红名(二进制)
			XDJG_Titles=0x6116,//角色称号(二进制)

			XDJG_SendAccount=0x6117,//发件人帐号
			XDJG_SendCharacter=0x6118,//发件人角色
			XDJG_Title=0x6119,//邮件标题
			XDJG_ReceiveTime=0x6120,//接收时间
			XDJG_sssTitles=0x6121,//角色称号(二进制)
			XDJG_IsBind=0x6122,//道具是否绑定
			XDJG_QualityEquip=0x6123,//是否为优质装备
			XDJG_SuperpositionNum=0x6124,//叠加数量
			XDJG_CreaterID=0x6125,//创建者ID
			XDJG_CreateTime=0x6126,//创建时间
			XDJG_DurableValue=0x6127,//耐久值
			XDJG_IncreaseType=0x6128,//增幅类型
			XDJG_IncreaseValue=0x6129,//增幅附加值
			XDJG_IncreaseEndValue=0x6130,//最后一次增幅附加值
			XDJG_IncreaseNum=0x6131,//增幅次数
			XDJG_AssemblyID=0x6132,//装备组装ID
			XDJG_PowerEffect=0x6133,//能力效果
			XDJG_PowerStartTime=0x6134,//能力开始时间
			XDJG_EquipSlotID1=0x6135,//装备镶嵌宝石的ID1
			XDJG_EquipSlotID2=0x6136,//装备镶嵌宝石的ID2
			XDJG_EquipSlotID3=0x6137,//装备镶嵌宝石的ID3
			XDJG_StarsLevel=0x6138,//装备星级
			XDJG_UnlockTime=0x6139,//解锁次数
			XDJG_RandomProperty=0x6140,//随机属性
			XDJG_Pwd=0x6141,//临时密码


			XDJG_ItemID1=0x6142,//道具ID1
			XDJG_ItemID2=0x6143,//道具ID2
			XDJG_ItemID3=0x6144,//道具ID3
			XDJG_ItemID4=0x6145,//道具ID4
			XDJG_ItemID5=0x6146,//道具ID5
			XDJG_ItemID6=0x6147,//道具ID6
			XDJG_ItemID7=0x6148,//道具ID7
			XDJG_ItemID8=0x6149,//道具ID8

			XDJG_ItemNum1=0x6150,//道具数量1
			XDJG_ItemNum2=0x6151,//道具数量2
			XDJG_ItemNum3=0x6152,//道具数量3
			XDJG_ItemNum4=0x6153,//道具数量4
			XDJG_ItemNum5=0x6154,//道具数量5
			XDJG_ItemNum6=0x6155,//道具数量6
			XDJG_ItemNum7=0x6156,//道具数量7
			XDJG_ItemNum8=0x6157,//道具数量8

			XDJG_TempItemId1=0x6158,//临时道具ID1
			XDJG_TempItemId2=0x6159,//临时道具ID2
			XDJG_TempItemId3=0x6160,//临时道具ID3
			XDJG_TempItemId4=0x6161,//临时道具ID4
			XDJG_TempItemId5=0x6162,//临时道具ID5
			XDJG_TempItemId6=0x6163,//临时道具ID6
			XDJG_TempItemId7=0x6164,//临时道具ID7
			XDJG_TempItemId8=0x6165,//临时道具ID8

			XDJG_Task_ID=0x6166,//任务ID
			XDJG_Task_Name=0x6167,//任务名称
			XDJG_TaskType=0x6168,//任务类型

			XDJG_ItemThrowID1=0x6169,//失去道具ID1
			XDJG_ItemThrowID2=0x6170,//失去道具ID2
			XDJG_ItemThrowID3=0x6171,//失去道具ID3
			XDJG_ItemThrowID4=0x6172,//失去道具ID4
			XDJG_ItemThrowID5=0x6173,//失去道具ID5
			XDJG_ItemThrowID6=0x6174,//失去道具ID6
			XDJG_ItemThrowID7=0x6175,//失去道具ID7
			XDJG_ItemThrowID8=0x6176,//失去道具ID8
 			
			XDJG_TempItemThrowId1=0x6177,//临时道具ID1
			XDJG_TempItemThrowId2=0x6178,//临时道具ID2
			XDJG_TempItemThrowId3=0x6179,//临时道具ID3
			XDJG_TempItemThrowId4=0x6180,//临时道具ID4
			XDJG_TempItemThrowId5=0x6181,//临时道具ID5
			XDJG_TempItemThrowId6=0x6182,//临时道具ID6
			XDJG_TempItemThrowId7=0x6183,//临时道具ID7
			XDJG_TempItemThrowId8=0x6184,//临时道具ID8

			XDJG_ItemThrowNum1=0x6185,//失去道具数量1
			XDJG_ItemThrowNum2=0x6186,//失去道具数量2
			XDJG_ItemThrowNum3=0x6187,//失去道具数量3
			XDJG_ItemThrowNum4=0x6188,//失去道具数量4
			XDJG_ItemThrowNum5=0x6189,//失去道具数量5
			XDJG_ItemThrowNum6=0x6190,//失去道具数量6
			XDJG_ItemThrowNum7=0x6191,//失去道具数量7
			XDJG_ItemThrowNum8=0x6192,//失去道具数量8

			XDJG_Items=0x6193,//物品属性
			XDJG_SurplusTime=0x6194,//剩余时间
			XDJG_AuctionPrice=0x6195,//拍卖价格
			XDJG_Deposit=0x6196,//保证金
			XDJG_SkillName=0x6197,//模糊技能名称
			XDJG_UpdateType=0x6198,//修改类型(等级,经验)
			XDJG_Prototype=0x6199,//生活技能类型
			XDJG_MailTitle=0x6200,//邮件标题
			XDJG_MailContent=0x6201,//邮件内容
			XDJG_PropertyId=0x6202,//附加属性ID
			XDJG_PropertyValue=0x6203,//附加属性值
			XDJG_SlotNum=0x6204,//插孔数量
			XDJG_CarID=0x6205,//金刚ID
			XDJG_cpValue=0x6206,//当前CP值
			XDJG_LookID=0x6207,//外观ID
			XDJG_IsLock=0x6208,//是否锁定
			XDJG_SkillStrValue=0x6209,//天赋附加值
			XDJG_TimePlugNum=0x6210,//时间芯片数量
			XDJG_TimePlugSlot1=0x6211,//时间芯片id1
			XDJG_TimePlugSlot2=0x6212,//时间芯片id2
			XDJG_BrandCreateTime=0x6213,//商标创建时间
			XDJG_BrandID=0x6214,//商标ID

			XDJG_GuildID=0x6215,//公会ID
			XDJG_GuildName=0x6216,//公会名称
			XDJG_GuildLevel=0x6217,//公会等级
			XDJG_GuildLeader=0x6218,//会长名称
			XDJG_Guildmember=0x6219,//会员名称
			XDJG_JoinGuildTime=0x6220,//入会时间
			XDJG_TaskStep=0x6221,//任务进度
			XDJG_Guildmembers=0x6222,//公会人数
			XDJG_OperateTime=0x6223,//操作时间
			XDJG_Operatetype=0x6224,//操作类型
			XDJG_MoneyLose=0x6225,//消耗金钱
			XDJG_TempCharacter=0x6226,//临时角色
			XDJG_JoinTime=0x6227,//加入时间
			XDJG_GuildLog=0x6228,//公会日志
			XDJG_LeaveTime=0x6229,//离开时间
			XDJG_ExitGuildType=0x6230,//离开公会
			XDJG_LeaderChangedTime=0x6231,//转职时间
			XDJG_TempSkillId=0x6232,//临时技能id
			XDJG_GuildPosition=0x6233,//会员职位
			XDJG_GuildTransferedPosition=0x6234,//变更后职位
			XDJG_ConsumeItem=0x6235,//消耗道具
			XDJG_CarName=0x6236,//金刚名称
			XDJG_TempTaskId=0x6237,//任务临时ID
			XDJG_IsDeleted=0x6238,//是否删除
			XDJG_RoleProperty=0x6239,//基础属性
			XDJG_ActivityName=0x6240,//礼包名称
			XDJG_GoodsName=0x6241,//物品名称
			XDJG_GoodsUsedOff=0x6242,//物品使用期限
			XDJG_GetTime=0x6243,//领取时间

			XDJG_ChoiceItemID1=0x6244,//可选道具id1
			XDJG_ChoiceItemID2=0x6245,//可选道具id2
			XDJG_ChoiceItemID3=0x6246,//可选道具id3
			XDJG_ChoiceItemID4=0x6247,//可选道具id4
			XDJG_ChoiceItemID5=0x6248,//可选道具id5
			XDJG_ChoiceItemID6=0x6249,//可选道具id6

			XDJG_ChoiceItemNum1=0x6250,//可选道具数量1
			XDJG_ChoiceItemNum2=0x6251,//可选道具数量2
			XDJG_ChoiceItemNum3=0x6252,//可选道具数量3
			XDJG_ChoiceItemNum4=0x6253,//可选道具数量4
			XDJG_ChoiceItemNum5=0x6254,//可选道具数量5
			XDJG_ChoiceItemNum6=0x6255,//可选道具数量6
			
			XDJG_TitleID=0x6256,//称号ID
			XDJG_BuffID=0x6257,//BUFFID
			XDJG_PositionX=0x6258,//X坐标
			XDJG_PositionY=0x6259,//Y坐标
			XDJG_PositionZ=0x6260,//Z坐标
			XDJG_TargetCharacter=0x6261,//目标角色
			XDJG_TitleName=0x6262,//称号
			XDJG_BuffName=0x6263,//BUFF名称
			//End

			/// <summary>
			/// 超级舞者定义
			/// </summary>
			SDO_ServerIP = 0x0801,//Format:String 大区IP
			SDO_UserIndexID = 0x0802,//Format:Integer 玩家用户ID
			SDO_Account = 0x0803,//Format:String 玩家的帐号
			SDO_Level = 0x0804,//Format:Integer 玩家的等级
			SDO_Exp = 0x0805,//Format:Integer 玩家的当前经验值
			SDO_GameTotal = 0x0806,//Format:Integer 总局数
			SDO_GameWin  = 0x0807,//Format:Integer 胜局数
			SDO_DogFall = 0x0808,//Format:Integer 平局数
			SDO_GameFall = 0x0809,//Format:Integer 负局数
			SDO_Reputation = 0x0810,//Format:Integer 声望值
			SDO_GCash = 0x0811,//Format:Integer G币
			SDO_MCash = 0x0812,//Format:Integer M币
			SDO_Address = 0x0813,//Format:Integer 地址
			SDO_Age = 0x0814,//Format:Integer 年龄
			SDO_ProductID = 0x0815,//Format:Integer 商品编号
			SDO_ProductName = 0x0816,//Format:String 商品名称
			SDO_ItemCode  = 0x0817,//Format:Integer 道具编号
			SDO_ItemName = 0x0818,//Format:String 道具名称
			SDO_TimesLimit = 0x0819,//Format:Integer 使用次数
			SDO_DateLimit = 0x0820,//Format:Integer 使用时效
			SDO_MoneyType = 0x0821,//Format:Integer 货币类型
			SDO_MoneyCost = 0x0822,//Format:Integer 道具的价格
			SDO_ShopTime = 0x0823,//Format:DateTime 消费时间
			SDO_MAINCH = 0x0824,//Format:Integer 服务器
			SDO_SUBCH = 0x0825,//Format:Integer 房间
			SDO_Online = 0x0826,//Format:Integer 是否在线
			SDO_LoginTime = 0x0827,//Format:DateTime 上线时间
			SDO_LogoutTime = 0x0828,//Format:DateTime 下线时间
			SDO_AREANAME = 0x0829,//Format:String 大区名字
			SDO_City = 0x0830,//Format:String 玩家所住城市
			SDO_Title = 0x0831,//Format:String 道具主题
			SDO_Context = 0x0832,//Format:String 道具描述
			SDO_MinLevel = 0x0833,//Format:Integer 所带道具的最小等级
			SDO_ActiveStatus = 0x0834,//Format:Integer 激活状态
			SDO_StopStatus = 0x0835,//Format:Integer 封停状态
			SDO_NickName = 0x0836,//Format:String 呢称
			SDO_9YouAccount = 0x0837,//Format:Integer 9you的帐号
			SDO_SEX = 0x0838,//Format:Integer 性别
			SDO_RegistDate =  0x0839,//Format:Date 注册日期
			SDO_FirstLogintime = 0x0840,//Format:Date 第一次登录时间
			SDO_LastLogintime  = 0x0841,//Format:Date 最后一次登录时间
			SDO_Ispad = 0x0842,//Format:Integer 是否已注册跳舞毯
			SDO_Desc = 0x0843,//Format:String 道具描述
			SDO_Postion = 0x0844,//Format:Integer 道具位置
			SDO_BeginTime = 0x0845,//Format:Date 消费记录开始时间
			SDO_EndTime = 0x0846,//Format:Date 消费记录结束时间
			SDO_SendTime = 0x0847,//Format:Date 道具送人日期
			SDO_SendIndexID = 0x0848,//Format:Integer 发送人的ID
			SDO_SendUserID = 0x0849,//Format:String 发送人帐号
			SDO_ReceiveNick = 0x0850,//Format:String 接受人呢称
			SDO_BigType = 0x0851,//Format:Integer 道具大类
			SDO_SmallType = 0x0852,//Format:Integer 道具小类
			SDO_REASON = 0x0853,//Format:String 停封理由
			SDO_StopTime = 0x0854,//Format:TimeStamp 停封时间
			SDO_DaysLimit  = 0x0855,//Format:Integer 使用天数
			SDO_Email = 0x0856,//Format:String 邮件
			SDO_ChargeSum = 0x0857,//Format:String 充值合计
			SDO_SenceID = 0x0858,
			SDO_WeekDay = 0x0859,
			SDO_MatPtHR = 0x0860,
			SDO_MatPtMin = 0x0861,
			SDO_StPtHR = 0x0862,
			SDO_StPtMin = 0x0863,
			SDO_EdPtHR = 0x0864,
			SDO_EdPtMin = 0x0865,
			SDO_Sence = 0x0868,
			SDO_MusicID1 = 0x0869,
			SDO_MusicName1 = 0x0870,
			SDO_LV1 = 0x0871,
			SDO_MusicID2 = 0x0872,
			SDO_MusicName2 = 0x0873,
			SDO_LV2 = 0x0874,
			SDO_MusicID3 = 0x0875,
			SDO_MusicName3 = 0x0876,
			SDO_LV3 = 0x0877,
			SDO_MusicID4 = 0x0878,
			SDO_MusicName4 = 0x0879,
			SDO_LV4 = 0x0880,
			SDO_MusicID5 = 0x0881,
			SDO_MusicName5 = 0x0882,
			SDO_LV5 = 0x0883,
			SDO_Precent = 0x0884,
			SDO_KeyID = 0x0885,
			SDO_KeyWord = 0x0886,
			SDO_MasterID = 0x0887,
			SDO_Master = 0x0888,
			SDO_SlaverID = 0x0889,
			SDO_Slaver = 0x0890,
			SDO_ChannelList = 0x0891,
			SDO_BoardMessage = 0x0892,
			SDO_wPlanetID = 0x0893, 
			SDO_wChannelID = 0x0894,
			SDO_iLimitUser = 0x0895, 
			SDO_iCurrentUser = 0x0896,
			SDO_ipaddr = 0x0897,
			SDO_Interval = 0x0898,
			SDO_TaskID = 0x0899,
			SDO_Status = 0x08100,
			SDO_Score = 0x08101,
			SDO_FirstPadTime = 0x08102,//跳舞毯第一次使用时间
			SDO_BanDate = 0x08103,//停封多少天
			SDO_Passwd = 0x08104,
			SDO_OnlineTime = 0x08105,//在线时间string
			SDO_LevPercent = 0x08106,
			SDO_ItemCodeBy = 0x08107,
			SDO_Punish_Status = 0x08108,
			SDO_fragment_id = 0x08109,//
			SDO_fragment_num = 0x08110,//
			SDO_BuyTimes = 0x08111,
			SDO_CoupleIndexID = 0x08112,
			SDO_CoupleNickName = 0x08113,
			SDO_RingLevel = 0x08114,
			SDO_NewRingLevel = 0x08115,
			SDO_MarriageApp_Date = 0x08116,
			SDO_DivorceApp_Date = 0x08117,
			SDO_Marriage_Date = 0x08118,
			SDO_Divorce_Date = 0x08119,
			SDO_Type = 0x08120,
			SDO_Usedate = 0x08121,
			SDO_Area = 0x08122,
			SDO_Padstatus = 0x08123,
			SDO_expcash = 0x08124,
			SDO_usecash = 0x08125,
			SDO_RewardItemTime = 0x08126,
			SDO_PunishTimes = 0x08127,
			SDO_DelTimes = 0x08128,
			SDO_DATE = 0x08129,
			SDO_PCash = 0x08130,
			SDO_IsBattle = 0x8131,
			SDO_OldIP = 0x8132,
			SDO_Times = 0x8133,
			SDO_State = 0x8134,//TLV_STRING  状态
			SDO_mood = 0x8135,//TLV_INTEGER 心情值
			SDO_Food = 0x8136,//TLV_INTEGER 饱食度
			SDO_ItemCode1 = 0x8137,//TLV_STRING  道具ID
			SDO_DateLimit1 = 0x8138,//TLV_INTEGER 日期限制
			SDO_TimeLimit1= 0x8139,//TLV_INTEGER 次数限制
			SDO_ItemCode2 = 0x8140,//TLV_STRING  道具ID
			SDO_DateLimit2 = 0x8141,//TLV_INTEGER 日期限制
			SDO_TimeLimit2= 0x8142,//TLV_INTEGER 次数限制
			SDO_ItemCode3 = 0x8143,//TLV_STRING  道具ID
			SDO_DateLimit3 = 0x8144,//TLV_INTEGER 日期限制
			SDO_TimeLimit3= 0x8145,//TLV_INTEGER 次数限制
			SDO_ItemCode4 = 0x8146,//TLV_STRING  道具ID
			SDO_DateLimit4 = 0x8147,//TLV_INTEGER 日期限制
			SDO_TimeLimit4= 0x8148,//TLV_INTEGER 次数限制
			SDO_ItemCode5 = 0x8149,//TLV_STRING  道具ID
			SDO_DateLimit5 = 0x8150,//TLV_INTEGER 日期限制
			SDO_TimeLimit5= 0x8151,//TLV_INTEGER 次数限制
			SDO_ItemName1 = 0x8152,//TLV_STRING  道具名1
			SDO_ItemName2 = 0x8153,//TLV_STRING  道具ID2
			SDO_ItemName3 = 0x8154,//TLV_STRING  道具ID3
			SDO_ItemName4 = 0x8155,//TLV_STRING  道具ID4
			SDO_ItemName5 = 0x8156,//TLV_STRING  道具ID5
			SDO_CoupleUserName = 0x8157,//TLV_STRING  女性帐号
			SDO_KeyPress = 0x8158,
			SDO_LoginErr = 0x8159,
			SDO_PreValue= 0x8160,//最小值
			SDO_EndValue= 0x8161,//最大值
			SDO_NorProFirst= 0x8162,//第一次打开的概率
			SDO_NorPro= 0x8163,//普通宝箱的概率
			SDO_SpePro= 0x8164,//特殊宝箱的概率
			SDO_baoxiangid= 0x8165,//id int
			SDO_Mark= 0x8166,//标识 int

			SDO_RemoteIp=0x8167,
			SDO_RemoteType=0x8168,
			SDO_MsgID=0x8169,//id
			SDO_channelname=0x8171,//string 
			SDO_type=0x8172,//string 
			SDO_UserName=0x8173,//string
			SDO_Message=0x8174,//string 
			SDO_datetime=0x8175,//string 
			SDO_ServerName=0x8176,//string
			SDO_ChannelID=0x8177,//string 频道id
			SDO_PlanetID=0x8178,//string 星球id
			SDO_PlanetName=0x8179,//string星球名
			SDO_ChannelName=0x8180,//string频道名
			SDO_FamilyName = 0x08181,//string 家族名称
			SDO_FamilyID = 0x08182,//int 家族ID
			SDO_BuyUserNick = 0x8183,//string 购买用户呢称
			SDO_BeforeMoney = 0x8184,//购买前家族M币数量
			SDO_Shaikh = 0x8185,//家族族长string
			SDO_Contribute = 0x8186,//家族贡献int
			SDO_Popular = 0x8187,//家族人气值int
			SDO_Curdaypopular = 0x8188,//当天累计家族人气值int
			SDO_Opensign = 0x8189,//开放标志 int
			SDO_Fmlevel = 0x8190,//家族等级 int
			SDO_Maxmember = 0x8191,//家族最大成员 int
			SDO_Curmember = 0x8192,//家族现有成员 int
			SDO_Fortune = 0x8193,//家族财富int
			SDO_Found_Date = 0x8194,//创建时间 datetime
			SDO_Rank = 0x8195,//家族荣誉 int
			SDO_Honor = 0x8196,//家族荣誉排名 string
			SDO_UseCost = 0x8197,//使用的G币 int 
			SDO_GetTime = 0x8198,//Format:Date 获得时间
			SDO_Duty = 0x8199,//家族权限(255表示族长0普通会员1管理员) string
			SDO_Formal = 0x8200,//加入情况(0表示申请加入，1表示现在加入) string
			SDO_Emblemtype = 0x8201,//勋章类型
			SDO_Emblemnum = 0x8202,//勋章数量
			SDO_MoneyTime = 0x8203,//捐献时间
			SDO_PetLevel = 0x8204,//宠物等级
			SDO_PetExperience = 0x8205,//宠物经验
			SDO_JoinDate = 0x8206,//加入时间
			SDO_Money = 0x8207,//玩家钱币

			SDO_SocialValue=0x8208,//社交值
			SDO_Lucky=0x8209,//幸运值



			Ban_UserID=0x9001,//用户名ID
			Ban_UserName=0x9002,//用户名
			Ban_NickID=0x9003,//昵称ID
			Ban_UserNick=0x9004,//昵称
			Ban_StartTime=0x9005,//开始时间
			Ban_EndTime=0x9006,//结束时间
			Ban_Reason=0x9007,//解/封停理由
			Ban_Memo=0x9008,//备注(账号1,昵称0)
			Ban_Type=0x9009,//立即解封0，定时解封1
			

			
			MESSAGE = 0x4001,
			ERROR_Msg = 0xFFFF
		};  
		__Enum _value; // 枚举值  
		TagName(int value = 0) : _value((__Enum)value) {}  
		TagName& operator=(int value) 
		{       
			this->_value = (__Enum)value;
			return *this;
		}   
		operator int() 
			const 
		{        return this->_value;    }
	};


	struct ServiceKey
	{    
		enum __Enum 
		{      
			CONNECT = 0x0001,
			CONNECT_RESP = 0x8001,
			DISCONNECT = 0x0002,
			DISCONNECT_RESP = 0x8002,
			ACCOUNT_AUTHOR = 0x0003,
			ACCOUNT_AUTHOR_RESP = 0x8003,
			SERVERINFO_IP_QUERY = 0x0004,
			SERVERINFO_IP_QUERY_RESP = 0x8004,
			GMTOOLS_OperateLog_Query = 0x0005,//查看工具操作记录
			GMTOOLS_OperateLog_Query_RESP = 0x8005,//查看工具操作记录响应
			SERVERINFO_IP_ALL_QUERY = 0x0006,//查看所有游戏服务器地址
			SERVERINFO_IP_ALL_QUERY_RESP = 0x8006,//查看所有游戏服务器地址响应
			LINK_SERVERIP_CREATE = 0x0007,//添加游戏链接数据库
			LINK_SERVERIP_CREATE_RESP = 0x8007,//添加游戏链接数据库响应
			CLIENT_PATCH_COMPARE = 0x0008,
			CLIENT_PATCH_COMPARE_RESP = 0x8008,
			CLIENT_PATCH_UPDATE = 0x0009,
			CLIENT_PATCH_UPDATE_RESP = 0x8009,
			LINK_SERVERIP_DELETE = 0x0010,
			LINK_SERVERIP_DELETE_RESP = 0x8010,
			GMTOOLS_BUGLIST_QUERY = 0x0011,
			GMTOOLS_BUGLIST_QUERY_RESP = 0x8011,
			GMTOOLS_BUGLIST_INSERT = 0x0012,
			GMTOOLS_BUGLIST_INSERT_RESP = 0x8012,
			GMTOOLS_BUGLIST_UPDATE = 0x0013,
			GMTOOLS_BUGLIST_UPDATE_RESP = 0x8013,
			GMTOOLS_UPDATELIST_QUERY = 0x0014,
			GMTOOLS_UPDATELIST_QUERY_RESP = 0x8014,
			GMTOOLS_RESETSTATICS_QUERY = 0x0015,
			GMTOOLS_RESETSTATICS_QUERY_RESP = 0x8015,
			GMTOOLS_USERMAC_UPDATE = 0x0016,
			GMTOOLS_USERMAC_UPDATE_RESP = 0x8016,
			DOMAIN_AUTHOR = 0x0017,
			DOMAIN_AUTHOR_RESP = 0x8017,
			GM_AccountInfo_Vip = 0x0020,			//查询vip帐号
			GM_AccountInfo_Vip_RESP = 0x8020,		//查询vip帐号响应
			GM_ClosePlayerGroup_WA = 0x0021,			//封停帐号
			GM_ClosePlayerGroup_RESP_WA = 0x8021,      //封停帐号响应
			///<summary>
			///用户管理模块(0x81) 
			/// </summary>
			USER_CREATE = 0x0001,
			USER_CREATE_RESP = 0x8001,
			USER_UPDATE = 0x0002,
			USER_UPDATE_RESP = 0x8002,
			USER_DELETE = 0x0003,
			USER_DELETE_RESP = 0x8003,
			USER_QUERY = 0x0004,
			USER_QUERY_RESP = 0x8004,
			USER_PASSWD_MODIF = 0x0005,
			USER_PASSWD_MODIF_RESP = 0x8005,
			USER_QUERY_ALL = 0x0006,
			USER_QUERY_ALL_RESP = 0x8006,
			DEPART_QUERY = 0x0007,
			DEPART_QUERY_RESP = 0x8007,
			UPDATE_ACTIVEUSER = 0x0008,//更新在线用户状态
			UPDATE_ACTIVEUSER_RESP = 0x8008,//更新在线用户状态响应
			DEPARTMENT_CREATE = 0x0009,//部门创建
			DEPARTMENT_CREATE_RESP = 0x8009,//部门创建响应
			DEPARTMENT_UPDATE= 0x0010,//部门修改
			DEPARTMENT_UPDATE_RESP = 0x8010,//部门修改响应
			DEPARTMENT_DELETE= 0x0011,//部门删除
			DEPARTMENT_DELETE_RESP = 0x8011,//部门删除响应
			DEPARTMENT_ADMIN = 0x0012,//部门管理
			DEPARTMENT_ADMIN_RESP = 0x8012,//部门管理响应
			DEPARTMENT_RELATE_QUERY = 0x0013,//部门关联查询
			DEPARTMENT_RELATE_QUERY_RESP = 0x8013,//部门关联查询
			DEPART_RELATE_GAME_QUERY = 0x0014,
			DEPART_RELATE_GAME_QUERY_RESP = 0x8014,
			USER_SYSADMIN_QUERY = 0x0015,
			USER_SYSADMIN_QUERY_RESP = 0x8015,
			USERBAN_QUERY = 0x0016,
			USERBAN_QUERY_RESP = 0x8016,


			USER_ACTIVE=0x0018,//心跳包
			USER_ACTIVE_RESP=0x8018,


			//Added by tengjie 2010-05-05
			HARMFUL_PROC_QUERY = 0x0019,//外挂进程工具包
			HARMFUL_PROC_QUERY_RESP=0x8019,
			/// <summary>
			/// 模块管理(0x82)
			/// </summary>
			MODULE_CREATE = 0x0001,
			MODULE_CREATE_RESP = 0x8001,
			MODULE_UPDATE = 0x0002,
			MODULE_UPDATE_RESP = 0x8002,
			MODULE_DELETE = 0x0003,
			MODULE_DELETE_RESP = 0x8003,
			MODULE_QUERY = 0x0004,
			MODULE_QUERY_RESP = 0x8004,

			/// <summary>
			/// 用户与模块(0x83) 
			/// </summary>
			USER_MODULE_CREATE = 0x0001,
			USER_MODULE_CREATE_RESP = 0x8001,
			USER_MODULE_UPDATE = 0x0002,
			USER_MODULE_UPDATE_RESP = 0x8002,
			USER_MODULE_DELETE = 0x0003,
			USER_MODULE_DELETE_RESP = 0x8003,
			USER_MODULE_QUERY= 0x0004,
			USER_MODULE_QUERY_RESP = 0x8004,

			/// <summary>
			/// 游戏管理(0x84) 
			/// </summary>
			GAME_CREATE = 0x0001,
			GAME_CREATE_RESP = 0x8001,
			GAME_UPDATE = 0x0002,
			GAME_UPDATE_RESP = 0x8002,
			GAME_DELETE = 0x0003,
			GAME_DELETE_RESP = 0x8003,
			GAME_QUERY = 0x0004,
			GAME_QUERY_RESP = 0x8004,
			GAME_MODULE_QUERY = 0x0005,
			GAME_MODULE_QUERY_RESP = 0x8005,

			/// <summary>
			/// 仙剑GM工具(0x62)
			/// </summary>
			PAL_PLAYERONLINECOUNT=0x0001,//玩家在线数量
			PAL_PLAYERONLINECOUNT_RESP=0x8001,//玩家在线数量响应

			PAL_CHARACTERBAG=0x0002,//玩家背包查询
			PAL_CHARACTERBAG_RESP=0x8002,//玩家背包查询响应

			PAL_CHARACTERFRIENDLIST=0x0003,//取得角色好友列表
			PAL_CHARACTERFRIENDLIST_RESP=0x8003,//取得角色好友列表响应

			PAL_CHARACTERINFO=0x0004,//取得角色基本资讯
			PAL_CHARACTERINFO_RESP=0x8004,//取得角色基本资讯响应

			PAL_CHARACTERLIST=0x0005,//取得角色列表
			PAL_CHARACTERLIST_RESP=0x8005,//取得角色列表响应

			PAL_CHARACTERQUEST=0x0006,//取得角色任务资讯
			PAL_CHARACTERQUEST_RESP=0x8006,//取得角色任务资讯响应

			PAL_GUILDBASEINFO=0x0007,//取得公会基本资讯
			PAL_GUILDBASEINFO_RESP=0x8007,//取得公会基本资讯响应

			PAL_GUILDMEMBERLIST=0x0008,//取得公会成员列表
			PAL_GUILDMEMBERLIST_RESP=0x8008,//取得公会成员列表响应

			PAL_SENDGIFTMAIL=0x0009,//寄送奖品邮件
			PAL_SENDGIFTMAIL_RESP=0x8009,//寄送奖品邮件响应

			PAL_ACCOUNTLOCAL_QUERY = 0x0010,//本地封停的玩家帐号查询
			PAL_ACCOUNTLOCAL_QUERY_RESP = 0x8010,//本地封停的玩家帐号查询响应
			PAL_ACCOUNT_CLOSE = 0x0011,//封停的玩家帐号
			PAL_ACCOUNT_CLOSE_RESP = 0x8011,//封停的玩家帐号响应
			PAL_ACCOUNT_OPEN = 0x0012,//解封的玩家帐号
			PAL_ACCOUNT_OPEN_RESP = 0x8012,//解封的玩家帐号响应
			PAL_ACCOUNT_BANISHMENT_QUERY = 0x0013,//玩家封停帐号查询
			PAL_ACCOUNT_BANISHMENT_QUERY_RESP = 0x8013,//玩家封停帐号查询响应

			
			PAL_MODIFYCHARACTERMONEY=0x0014,//修改角色金钱
			PAL_MODIFYCHARACTERMONEY_RESP=0x8014,//修改角色金钱响应

			PAL_MODIFYCHARACTERDROPPW=0x0015,//修改角色删除密码
			PAL_MODIFYCHARACTERDROPPW_RESP=0x8015,//修改角色删除密码响应

			PAL_MODIFYACCOUNTBANKPW=0x0016,//修改帐号银行密码
			PAL_MODIFYACCOUNTBANKPW_RESP=0x8016,//修改帐号银行密码响应

			PAL_DELCHARACTERITEM=0x0017,// 删除角色物品
			PAL_DELCHARACTERITEM_RESP=0x8017,// 删除角色物品响应

			PAL_RECALLCHARACTERDROPPW=0x0018,//还原角色删除密码
			PAL_RECALLCHARACTERDROPPW_RESP=0x8018,//还原角色删除密码响应

			PAL_RECALLACCOUNTBANKPW=0x0019,//还原帐号银行密码
			PAL_RECALLACCOUNTBANKPW_RESP=0x8019,//还原帐号银行密码响应

			PAL_MODIFYACCOUNTPW=0x0020,//修改帐号密码
			PAL_MODIFYACCOUNTPW_RESP=0x8020,//修改帐号密码响应

			PAL_RECALLACCOUNTPW=0x0021,//还原帐号密码
			PAL_RECALLACCOUNTPW_RESP=0x8021,//还原帐号密码响应

			PAL_BOARDTASK_INSERT = 0x0033,//公告添加
			PAL_BOARDTASK_INSERT_RESP = 0x8033,//公告添加响应
			PAL_BOARDTASK_QUERY = 0x0034,//公告查询
			PAL_BOARDTASK_QUERY_RESP = 0x8034,//公告查询响应
			PAL_BOARDTASK_UPDATE = 0x0035,//公告更新
			PAL_BOARDTASK_UPDATE_RESP = 0x8035,//公告更新响应


			PAL_CHECKMAILBOX_QUERY=0x0036,//邮箱信息查询
			PAL_CHECKMAILBOX_QUERY_RESP=0x8036,//邮箱信息查询响应
			
			PAL_CHECKMAILBOX_DETAIL=0x0043,//邮箱附件查询
			PAL_CHECKMAILBOX_DETAIL_RESP=0x8043,//邮箱附件查询响应
			
			PAL_AUCTION_QUERY=0x0037,//市场信息查询
			PAL_AUCTION_QUERY_RESP=0x8037,//市场信息查询响应 
			PAL_ITEMTYPE_QUERY=0x0037,//道具类别查询
			PAL_ITEMTYPE_QUERY_RESP=0x8037,//道具类别查询响应
			PAL_ITEMNAME_QUERY=0x0038,//道具名称查询
			PAL_ITEMNAME_QUERY_RESP=0x8038,//道具名称查询响应
			PAL_ITEMNAMEBLUR_QUERY=0x0039,//道具名称模糊查询
			PAL_ITEMNAMEBLUR_QUERY_RESP=0x8039,//道具名称模糊查询响应

			PAL_CDKEY_QUERY=0x0040,//CDKEY道具领取查询
			PAL_CDKEY_QUERY_RESP=0x8040,//CDKEY道具领取查询响应 

			PAL_ITEMMAIL_WASTE=0x0041,//消费记录
			PAL_ITEMMAIL_WASTE_RESP=0x8041,//消费记录响应

			PAL_HISTORY_LOGINFO_QUERY=0x0042,//日志历史
			PAL_HISTORY_LOGINFO_QUERY_RESP=0x8042,//日志历史响应

			PAL_ROLEDEL_QUERY=0x0050,//删除角色日志
			PAL_ROLEDEL_QUERY_RESP=0x8050,//删除角色日志
			
			PAL_MAILBOX_QUERY=0x0040,//查询玩家邮箱地址
			PAL_MAILBOX_QUERY_RESP=0x8040,//查询玩家邮箱地址响应 

			PAL_SendBankPwd_Query=0x0041,//发送仓库密码
			PAL_SendBankPwd_Query_RESP=0x8041,//发送仓库密码响应

			PAL_SendDeletePwd_Query=0x0042,//发送角色删除密码
			PAL_SendDeletePwd_Query_RESP=0x8042,//发送角色删除密码响应


			PAL_MODIFYSECONDPW=0x0043,//修改游戏二级密码
			PAL_MODIFYSECONDPW_RESP=0x8043,//修改游戏二级密码响应

			PAL_RECALLSECONDPW=0x0044,//还原游戏二级密码
			PAL_RECALLSECONDPW_RESP=0x8044,//还原游戏二级密码响应


			PAL_USESAFELOCK=0x0045,//使用安全锁
			PAL_USESAFELOCK_RESP=0x8045,//使用安全锁响应

			PAL_FREESAFELOCK=0x0046,//释放安全锁
			PAL_FREESAFELOCK_RESP=0x8046,//释放安全锁响应


			PAL_CHARACTERADVANCEINFO=0x0047,//取得角色进阶资讯
			PAL_CHARACTERADVANCEINFO_RESP=0x8047,//取得角色进阶资讯
			
			PAL_CHARACTERSKILL=0x0048,//取得角色技能资讯
			PAL_CHARACTERSKILL_RESP=0x8048,//取得角色技能资讯
			
			PAL_CHARACTERMOB=0x0049,//取得角色伏魔资讯
			PAL_CHARACTERMOB_RESP=0x8049,//取得角色伏魔资讯
			
			
			PAL_CHARACTERPUZZLEMAP=0x0050,//取得角色图鉴资讯
			PAL_CHARACTERPUZZLEMAP_RESP=0x8050,//取得角色图鉴资讯
			
			PAL_CHARACTERBLACKLIST=0x0051,//取得角色黑名单列表
			PAL_CHARACTERBLACKLIST_RESP=0x8051,//取得角色黑名单列表
			
			PAL_DELCHARACTERMOB=0x0052,//删除角色伏魔
			PAL_DELCHARACTERMOB_RESP=0x8052,//删除角色伏魔
			
			PAL_MODIFYPUZZLEMAP=0x0053,//修改角色图鉴
			PAL_MODIFYPUZZLEMAP_RESP=0x8053,//修改角色图鉴
			
			PAL_ACCOUNTINFO=0x0054,//取得帐号资讯
			PAL_ACCOUNTINFO_RESP=0x8054,//取得帐号资讯
			
			PAL_MOVECHARACTERTOSAFE=0x0055,//移动角色至安全点
			PAL_MOVECHARACTERTOSAFE_RESP=0x8055,//移动角色至安全点
			
			PAL_RELOADFUNCTIONSWITCH=0x0056,//重新载入功能开关列表
			PAL_RELOADFUNCTIONSWITCH_RESP=0x8056,//重新载入功能开关列表

			PAL_SearchMap_Query=0x0057,//查询地图
			PAL_SearchMap_Query_Resp=0x8057,//查询地图

			PAL_Get_EmailAddress=0x0066,
			PAL_Get_EmailAddress_Resp=0x8066,


			PAL_SCHEDULEBULLETINLIST=0x1001,//取得排程公告列表
			PAL_SCHEDULEBULLETINLIST_RESP=0x7001,//取得排程公告列表响应

			PAL_ADDSCHEDULEBULLETIN=0x1002,//新增排程公告
			PAL_ADDSCHEDULEBULLETIN_RESP=0x7002,//新增排程公告响应	

			PAL_DELSCHEDULEBULLETIN=0x1003,//删除排程公告
			PAL_DELSCHEDULEBULLETIN_RESP=0x7003,//删除排程公告响应	

			PAL_RELOADSCHEDULEBULLETIN=0x1004,//重新载入排程公告
			PAL_RELOADSCHEDULEBULLETIN_RESP=0x7004,//重新载入排程公告响应	

			PAL_SENDNONSCHEDULEBULLETIN=0x1005,//发送非固定公告
			PAL_SENDNONSCHEDULEBULLETIN_RESP=0x7005,//发送非固定公告响应	




			PAL_BANISHPLAYER =0x4001,//踢人
			PAL_BANISHPLAYER_RESP=0x6001,//踢人响应

			PAL_DISMISSGUILD=0x4002,//解散公会
			PAL_DISMISSGUILD_RESP=0x6002,//解散公会响应

			PAL_MODIFYGUILDNAME=0x4003,//修改公会名称
			PAL_MODIFYGUILDNAME_RESP=0x6003,//修改公会名称响应

			PAL_MODIFYGUILDMEMBERPL=0x4004,//修改公会成员职阶
			PAL_MODIFYGUILDMEMBERPL_RESP=0x6004,//修改公会成员职阶响应

			PAL_MODIFYCHARACTERQUEST=0x4005,//修改角色任务
			PAL_MODIFYCHARACTERQUEST_RESP=0x6005,//修改角色任务响应

			PAL_MODIFYGUILDLV=0x4006,//修改公会等级
			PAL_MODIFYGUILDLV_RESP=0x6006,//修改公会等级响应

			PAL_MODIFYGUILDNOTICE=0x4007,//修改公会公告
			PAL_MODIFYGUILDNOTICE_RESP=0x6007,//修改公会公告响应

			PALLOG_ACCOUNT_LOGIN=0x0001,//登陆记录日志
			PALLOG_ACCOUNT_LOGIN_RESP=0x8001,//登陆记录日志响应

			PAL_LOGINFO_QUERY=0x0002,//玩家日志信息
			PAL_LOGINFO_QUERY_RESP=0x8002,//玩家日志信息响应

			PAL_LOG_BIGTYPE_QUERY=0x0003,//日志大类
			PAL_LOG_BIGTYPE_QUERY_RESP=0x8003,//日志大类响应

			PAL_LOG_SMALLTYPE_QUERY=0x0004,//日志小类
			PAL_LOG_SMALLTYPE_QUERY_RESP=0x8004,//日志小类响应

			PALLOG_ITEMMAIL=0x0005,//商城购买日志
			PALLOG_ITEMMAIL_RESP=0x8005,//商城购买日志响应

			PAL_EQUIPMENT_QUERY=0x0061,//装备道具查询
			PAL_EQUIPMENT_QUERY_RESP=0x8061,//装备道具查询响应
			
			PAL_SENDCUSTOMITEMMAIL=0x0062,//寄送客制物品邮件
			PAL_SENDCUSTOMITEMMAIL_RESP=0x8062,//寄送客制物品邮件响应

			PAL_MODIFYACCOUNTPWALL=0x0063,//修改所有大区帐号密码
			PAL_MODIFYACCOUNTPWALL_RESP=0x8063,//修改所有大区帐号密码响应
			
			PAL_RECALLACCOUNTPWALL=0x0064,//恢复所以大区帐号密码
			PAL_RECALLACCOUNTPWALL_RESP=0x8064,//恢复所以大区帐号密码响应

			//////////////////
			//GTown(0x51)
			/////////////////
			//添加公告
			GT_BROADCAST_ADD = 0x0001,
			GT_BROADCAST_ADD_ACK = 0x8001,

			//查询所有公告
			GT_BROADCAST_SELECT = 0x0002,
			GT_BROADCAST_SELECT_ACK = 0x8002,

			//删除公告
			GT_BROADCAST_REMOVE = 0x0003,
			GT_BROADCAST_REMOVE_ACK = 0x8003,

			//强制玩家下线
			GT_KICK_USER = 0x0004,
			GT_KICK_USER_ACK = 0x8004,

			//角色移至小黑屋
			GT_MOVE_USER = 0x0005,
			GT_MOVE_USER_ACK = 0x8005,

			//家族管理
			GT_FAMILY_DISBAND = 0x0006,
			GT_FAMILY_DISBAND_ACK = 0x8006,

			//账号封停、解封
			GT_USER_STOP = 0x0007,
			GT_USER_STOP_ACK = 0x8007,

			//添加道具、宠物
			GT_ADD_ITEM = 0x0008,
			GT_ADD_ITEM_ACK = 0x8008,

			//查询道具、宠物
			GT_ITEM_SELECT = 0x0009,
			GT_ITEM_SELECT_ACK = 0x8009,

			//重设玩家到出生状态
			GT_USER_RESET = 0x0010,
			GT_USER_RESET_ACK = 0x8010,

			CONNECT_SERVERMANAGER_LOGIN = 0x0011,
			CONNECT_SERVERMANAGER_LOGIN_ACK = 0x8011,

			//查询用户资料信息
			GT_UserInfo_Query = 0x0012,
			GT_UserInfo_Query_Resp = 0x8012,
			
			//查询用户详细资料信息
			GT_User_DetailInfo_Query = 0x0013,
			GT_User_DetailInfo_Query_Resp = 0x8013,
			
			//查询角色家族信息
			GT_User_FamilyInfo_Query = 0x0014,
			GT_User_FamilyInfo_Query_Resp = 0x8014,
			
			//查询角色情侣信息
			GT_User_SentiInfo_Query = 0x0015,
			GT_User_SentiInfo_Query_Resp = 0x8015,
			
			//查询角色结婚信息
			GT_User_MarrigeInfo_Query = 0x0016,
			GT_User_MarrigeInfo_Query_Resp = 0x8016,

			//查询所有被封停账号列表
			GT_AllBanUser_Query = 0x0017,
			GT_AllBanUser_Query_Resp = 0x8017,

			//查询家族信息
			GT_FamilyInfo_Query = 0x0018,
			GT_FamilyInfo_Query_Resp = 0x8018,
			
			//查询家族成员信息
			GT_MemberInfo_Query = 0x0019,
			GT_MemberInfo_Query_Resp = 0x8019,
			
			//查询角色背包信息
			GT_UserPack_Query = 0x0020,
			GT_UserPack_Query_Resp = 0x8020,
			
			//查询角色装备信息
			GT_UserEquip_Query = 0x0021,
			GT_UserEquip_Query_Resp = 0x8021,
			
			//查询宠物背包信息
			GT_PetPack_Query = 0x0022,
			GT_PetPack_Query_Resp = 0x8022,


			//查询玩家交易日志
			GT_TradeLog_Query = 0x0023,
			GT_TradeLog_Query_Resp = 0x8023,
			
			//查询玩家邮件日志
			GT_MailLog_Query = 0x0024,
			GT_MailLog_Query_Resp = 0x8024,
			
			//查询玩家任务完成日志
			GT_TaskLog_Query = 0x0025,
			GT_TaskLog_Query_Resp = 0x8025,
			
			//查询玩家使用道具日志(收费道具)
			GT_CardLog_Query = 0x0026,
			GT_CardLog_Query_Resp = 0x8026,
			
			//查询玩家挂机奖励日志
			GT_GuajiLog_Query = 0x0027,
			GT_GuajiLog_Query_Resp = 0x8027,
			
			//查询玩家大小喇叭日志
			GT_BugleLog_Query = 0x0028,
			GT_BugleLog_Query_Resp = 0x8028,
			
			//查询账号登陆登出日志
			GT_LoginLog_Query = 0x0029,
			GT_LoginLog_Query_Resp = 0x8029,



			//查询人物房屋资料
			GT_RoomInfo_Query = 0x0030,
			GT_RoomInfo_Query_Resp = 0x8030,
			
			//查询人物宠物资料
			GT_PetInfo_Query = 0x0031,
			GT_PetInfo_Query_Resp = 0x8031,
			
			//查询GT-IM好友资料信息
			GT_IMFriend_Query = 0x0032,
			GT_IMFriend_Query_Resp = 0x8032,
			
			//查询GT-IM黑名单信息
			GT_IMBlackList_Query = 0x0033,
			GT_IMBlackList_Query_Resp = 0x8033,
			
			//查询GT-IM暗恋名单信息
			GT_IMSecretLove_Query = 0x0034,
			GT_IMSecretLove_Query_Resp = 0x8034,
			
			//查询宝箱开启信息
			GT_TreasureBox_Query = 0x0035,
			GT_TreasureBox_Query_Resp = 0x8035,
			
			//邮件信息查询
			GT_MailInfo_Query = 0x0036,
			GT_MailInfo_Query_Resp = 0x8036,
			
			//充值记录查询
			GT_ChargeInfo_Query = 0x0037,
			GT_ChargeInfo_Query_Resp = 0x8037,
			
			//道具购买消费记录
			GT_ItemConsume_Query = 0x0038,
			GT_ItemConsume_Query_Resp = 0x8038,


			//瞬间移动
			GT_Transmit_Query=0x0039,
			GT_Transmit_Query_Resp=0x8039,
			
			//禁止、解禁玩家上传图片
			GT_Picture_Stop=0x0040,
			GT_Picture_Stop_Resp=0x8040,

		
			//清理违禁图片
			GT_Picture_Clear=0x0043,
			GT_Picture_Clear_Resp=0x8043,


			//血拼战绩 T_gtown_Mcoin_Rank
			GT_Mcoin_Rank=0x0044,
			GT_Mcoin_Rank_Resp=0x8044,

			//空间排行 T_gtown_sns_db
			GT_Sns_Query=0x0045,
			GT_Sns_Query_Resp=0x8045,

			//属性排行
			GT_Attr_Queue=0x0046,
			GT_Attr_Queue_Resp=0x8046,

			//名片排行 T_gtown_card_record
			GT_Card_Record=0x0047,
			GT_Card_Record_Resp=0x8047,

			//朋友买卖中角色信息
			GT_FriendTrade_Info=0x0048,
			GT_FriendTrade_Info_Resp=0x8048,

			//奴隶信息
			GT_HelotInfo_Query=0x0049,
			GT_HelotInfo_Query_Resp=0x8049,

			//排行
			GT_FriendTrade_Queue=0x0050,
			GT_FriendTrade_Queue_Resp=0x8050,

			//买卖Log
			GT_FriendTrade_Log=0x0051,
			GT_FriendTrade_Log_Resp=0x8051,

			//清空个性签名
			GT_ClearName_Query=0x0052,
			GT_ClearName_Query_Resp=0x8052,

			//查询所有被禁止账号列表
			GT_AllStopUser_Query = 0x0053,
			GT_AllStopUser_Query_Resp = 0x8053,

			//查询道具(从原来接口变成数据库)
			GT_Select_Item=0x0054,
			GT_Select_Item_Resp=0x8054,


			//查询单个账号是否被封停
			GT_BanUser_Query=0x0055,
			GT_BanUser_Query_Resp=0x8055,

			//批量添加道具
			GT_Import_Item=0x0056,
			GT_Import_Item_Resp=0x8056,

			//退款
			GT_Back_Money=0x0057,
			GT_Back_Money_Resp=0x8057,

			/////////<summary>
			///勇士无双消息集(0x53)
			//////</summary>
			//查询用户信息
			WA_Account_Query=0x0001,
			WA_Account_Query_Resp=0x8001,
			
			//强制玩家下线
			WA_KickPlayer=0x0002,
			WA_KickPlayer_Resp=0x8002,
			
			//封停用户
			WA_ClosePlayer=0x0003,
			WA_ClosePlayer_Resp=0x8003,
			
			//解封用户
			WA_OpenPlayer=0x0004,
			WA_OpenPlayer_Resp=0x8004,
			
			//封停列表
			WA_CloseList=0x0005,
			WA_CloseList_Resp=0x8005,
			
			//单个封停
			WA_CloseSingle=0x0006,
			WA_CloseSingle_Resp=0x8006,
			
			//添加公告
			WA_BoardTasker_Add=0x0007,
			WA_BoardTasker_Add_Resp=0x8007,
			
			//删除公告
			WA_BoardTasker_Del=0x0008,
			WA_BoardTasker_Del_Resp=0x8008,
			
			//查询公告
			WA_BoardTasker_Query=0x0009,
			WA_BoardTasker_Query_Resp=0x8009,
			
			//好友信息查询
			WA_FriendInfo_Query=0x0010,
			WA_FriendInfo_Query_Resp=0x8010,

			//角色物品信息查询
			WA_BodyItemInfo_Query=0x0011,
			WA_BodyItemInfo_Query_Resp=0x8011,

			//角色背包信息查询
			WA_BagItemInfo_Query=0x0012,
			WA_BagItemInfo_Query_Resp=0x8012,

			//角色银行信息查询
			WA_BankItemInfo_Query=0x0013,
			WA_BankItemInfo_Query_Resp=0x8013,

			//邮箱信息查询
			WA_MailInfo_Query=0x0014,
			WA_MailInfo_Query_Resp=0x8014,
			
			//徽章信息查询
			WA_EmblemInfo_Query=0x0015,
			WA_EmblemInfo_Query_Resp=0x8015,

			//宠物信息查询
			WA_PetInfo_Query=0x0016,
			WA_PetInfo_Query_Resp=0x8016,

			//帐号登录信息查询
			WA_AccountLoginInfo_Query=0x0017,
			WA_AccountLoginInfo_Query_Resp=0x8017,

			//道具增加
			WA_Item_Add=0x0018,
			WA_Item_Add_Resp=0x8018,

			//道具删除
			WA_Item_Del=0x0019,
			WA_Item_Del_Resp=0x8019,

			//金钱修改
			WA_Money_Update=0x0020,
			WA_Money_Update_Resp=0x8020,

			//公告修改
			WA_BoardTasker_Update=0x0021,
			WA_BoardTasker_Update_Resp=0x8021,

			//add by ltx
			//修改角色技能
			WA_ModifyCharacterSkill=0x0022,
			WA_ModifyCharacterSkill_Resp=0x8022,

			//恢复角色
			WA_RecoverCharacter=0x0023,
			WA_RecoverCharacter_Resp=0x8023,

			//修改密码
			WA_ModifyAccountPassw=0x0024,
			WA_ModifyAccountPassw_Resp=0x8024,

			//给角色邮寄物品
			WA_MailItem=0x0025,
			WA_MailItem_Resp=0x8025,

			//物品:怪物掉落
			WA_Query_MonsterDropItem=0x0026,
			WA_Query_MonsterDropItem_Resp=0x8026,

			//物品:从NPC处购买
			WA_Query_BuyItemNPC=0x0027,
			WA_Query_BuyItemNPC_Resp=0x8027,
			
			//物品:卖给NPC
			WA_Query_SellItemTONPC=0x0028,
			WA_Query_SellItemTONPC_Resp=0x8028,

			//物品:玩家自行删除
			WA_Query_DropItem=0x0029,
			WA_Query_DropItem_Resp=0x8029,

			//物品:交易获得
			WA_Query_GetItemTrade=0x0030,
			WA_Query_GetItemTrade_Resp=0x8030,

			//物品:交易失去
			WA_Query_LoseItemTrade=0x0031,
			WA_Query_LoseItemTrade_Resp=0x8031,

			//物品:装备绑定记录
			WA_Query_BindItem=0x0032,
			WA_Query_BindItem_Resp=0x8032,

			//物品:装备解绑定记录
			WA_Query_UnBindItem=0x0033,
			WA_Query_UnBindItem_Resp=0x8033,

			//物品:角色“摆摊”日志查询
			WA_Query_BoothItem=0x0034,
			WA_Query_BoothItem_Resp=0x8034,

			//物品:道具删除记录(指使用时间到了)
			WA_Query_DeleteItem=0x0035,
			WA_Query_DeleteItem_Resp=0x8035,

			//物品:道具使用记录
			WA_Query_UseItem=0x0036,
			WA_Query_UseItem_Resp=0x8036,

			//物品:装备制作记录
			WA_Query_CraftItem=0x0037,
			WA_Query_CraftItem_Resp=0x8037,

			//物品:装备打孔
			WA_Query_PunchItem=0x0038,
			WA_Query_PunchItem_Resp=0x8038,

			//物品:装备改孔
			WA_Query_ChangeSlotItem=0x0039,
			WA_Query_ChangeSlotItem_Resp=0x8039,

			//物品:装备升星
			WA_Query_UpgradeStar=0x0040,
			WA_Query_UpgradeStar_Resp=0x8040,

			//物品:宝石
			WA_Query_Gem=0x0041,
			WA_Query_Gem_Resp=0x8041,

			//物品:制药记录
			WA_Query_Pharma=0x0042,
			WA_Query_Pharma_Resp=0x8042,

			//物品:宝石合成
			WA_Query_CraftGem=0x0043,
			WA_Query_CraftGem_Resp=0x8043,

			//物品:材料合成
			WA_Query_CraftMatirail=0x0044,
			WA_Query_CraftMatirail_Resp=0x8044,

			//物品:图纸合成
			WA_Query_CraftRecipe=0x0045,
			WA_Query_CraftRecipe_Resp=0x8045,

			//迷宫积分兑换奖励
			WA_Query_MazeIntegral=0x0046,
			WA_Query_MazeIntegral_Resp=0x8046,

			//游戏币:怪物掉落
			WA_Query_MonsterDropMoney=0x0047,
			WA_Query_MonsterDropMoney_Resp=0x8047,

			//游戏币:任务获得
			WA_Query_GetMoneyQuest=0x0048,
			WA_Query_GetMoneyQuest_Resp=0x8048,

			//游戏币:修装
			WA_Query_LoseMoneyRepair=0x0049,
			WA_Query_LoseMoneyRepair_Resp=0x8049,

			//游戏币:交易获得
			WA_Query_GetMoneyTrade=0x0050,
			WA_Query_GetMoneyTrade_Resp=0x8050,

			//游戏币:交易失去
			WA_Query_LoseMoneyTrade=0x0051,
			WA_Query_LoseMoneyTrade_Resp=0x8051,

			//任务:完成获得记录
			WA_Query_QuestReward=0x0052,
			WA_Query_QuestReward_Resp=0x8052,

			//任务:任务物品删除记录
			WA_Query_QuestGive=0x0053,
			WA_Query_QuestGive_Resp=0x8053,

			//邮件:发送记录
			WA_Query_MailSend=0x0054,
			WA_Query_MailSend_Resp=0x8054,

			//邮件:接收记录
			WA_Query_MailRecv=0x0055,
			WA_Query_MailRecv_Resp=0x8055,

			//邮件:删除记录
			WA_Query_MailDelete=0x0056,
			WA_Query_MailDelete_Resp=0x8056,

			//商城:购买记录
			WA_Query_ItemShop=0x0057,
			WA_Query_ItemShop_Resp=0x8057,

			//查询用户的副本日志
			WA_Query_Instance=0x0058,
			WA_Query_Instance_Resp=0x8058,

			//玩家登陆/登出日志
			WA_Query_LoginLogout=0x0059,
			WA_Query_LoginLogout_Resp=0x8059,
			
			//玩家登陆/登出日志(按ip方式查询)
			WA_Query_LoginLogoutByIP=0x0112,
			WA_Query_LoginLogoutByIP_Resp=0x8112,

			//恢复玩家密码
			WA_Query_ResumePassw=0x0113,
			WA_Query_ResumePassw_Resp=0x8113,

			//道具列表查询
			WA_ItemList_Query=0x0060,
			WA_ItemList_Query_Resp=0x8060,

			//技能列表查询
			WA_SkillList_Query=0x0061,
			WA_SkillList_Query_Resp=0x8061,

			//修改角色名
			WA_PlayerRole_Modfiy=0x0062,
			WA_PlayerRole_Modfiy_Resp=0x8062,
			
			//修改角色属性值
			WA_RoleInfo_Modfiy=0x0063,
			WA_RoleInfo_Modfiy_Resp=0x8063,

			//查询玩家身上技能信息
			WA_RoleSkill_Query=0x0064,
			WA_RoleSkill_Query_Resp=0x8064,
			
			//查询玩家道具类型查询
			WA_ItemType_Query=0x0065,
			WA_ItemType_Query_Resp=0x8065,

			WA_UserPwd_Recover=0x0066,
			WA_UserPwd_Recover_Resp=0x8066,

			//Added by tengjie 2010-06-04:For 角色日志
			//角色升级记录查询
			WA_Character_Upgrade=0x0067,
			WA_Character_Upgrade_Resp=0x8067,

			//角色创建记录查询
			WA_Character_Create=0x0068,
			WA_Character_Create_Resp=0x8068,

			//角色删除记录查询
			WA_Character_Delete=0x0069,
			WA_Character_Delete_Resp=0x8069,

			//加入帮会
			WA_Join_Guild=0x0070,
			WA_Join_Guild_Resp=0x8070,

			//离开帮会
			WA_Quit_Guild=0x0071,
			WA_Quit_Guild_Resp=0x8071,

			//创建帮会
			WA_Create_Guild=0x0072,
			WA_Create_Guild_Resp=0x8072,

			//解散帮会
			WA_Dissolve_Guild=0x0073,
			WA_Dissolve_Guild_Resp=0x8073,

			//转职信息查询
			WA_Transfer_Info=0x0074,
			WA_Transfer_Info_Resp=0x8074,
			
			//技能日志记录
			WA_Skills_Log=0x0075,
			WA_Skills_Log_Resp=0x8075,
			
			//生活技能日志记录
			WA_LifeSkills_Log=0x0076,
			WA_LifeSkills_Log_Resp=0x8076,

			//佣兵团日志
			//创建记录
			WA_Create_MercenaryGroup=0x0077,
			WA_Create_MercenaryGroup_Resp=0x8077,

			//解散记录
			WA_Dissolve_MercenaryGroup=0x0078,
			WA_Dissolve_MercenaryGroup_Resp=0x8078,

			//宠物徽章日志
			//徽章获得时间
			WA_GainBadge_Time=0x0079,
			WA_GainBadge_Time_Resp=0x8079,

			//徽章丢弃时间
			WA_DiscardBadge_Time=0x0080,
			WA_DiscardBadge_Time_Resp=0x8080,

			//徽章兑换记录
			WA_ExchangeBadge_Record=0x0081,
			WA_ExchangeBadge_Record_Resp=0x8081,

			//徽章鉴定
			WA_Identify_Badge=0x0082,
			WA_Identify_Badge_Resp=0x8082,
			
			//宠物精魂捕捉记录
			WA_CatchPetSpirit_Record=0x0083,
			WA_CatchPetSpirit_Record_Resp=0x8083,
			
			//封印精魂记录
			WA_SealSpirit_Record=0x0084,
			WA_SealSpirit_Record_Resp=0x8084,

			//徽章培养记录
			WA_CultureBadge_Record=0x0085,
			WA_CultureBadge_Record_Resp=0x8085,
			
			//精魂交易记录
			//精魂交易获得
			WA_GainSpirit_Record=0x0086,
			WA_GainSpirit_Record_Resp=0x8086,
			
			//精魂交易失去
			WA_LoseSpirit_Record=0x0087,
			WA_LoseSpirit_Record_Resp=0x8087,

			//精魂出售记录
			WA_SellSpirit_Record=0x0088,
			WA_SellSpirit_Record_Resp=0x8088,

			//用户信息查询
			//技能信息查询
			WA_SkillsInfo_Query=0x0089,
			WA_SkillsInfo_Query_Resp=0x8089,

			//任务信息查询:已完成任务
			WA_MissionsCompleted_Query=0x0090,
			WA_MissionsCompleted_Query_Resp=0x8090,

			//任务信息查询:已接任务
			WA_MissionsReceived_Query=0x0091,
			WA_MissionsReceived_Query_Resp=0x8091,

			//佣兵团信息查询
			//佣兵团信息查询
			WA_MercenaryGroup_Info_Query=0x0092,
			WA_MercenaryGroup_Info_Query_Resp=0x8092,

			//佣兵团成员列表查询
			WA_MemberList_Query=0x0093,
			WA_MemberList_Query_Resp=0x8093,

			//佣兵团仓库信息
			WA_WarehouseInfo_Query=0x0094,
			WA_WarehouseInfo_Query_Resp=0x8094,

			//领取佣兵团薪水信息
			WA_Receive_SalaryInfo_Query=0x0095,
			WA_Receive_SalaryInfo_Query_Resp=0x8095,

			//领取佣兵团经验信息
			WA_Receive_ExperienceInfo_Query=0x0096,
			WA_Receive_ExperienceInfo_Query_Resp=0x8096,

			//排行榜查询
			//等级排行
			WA_Rank_Query=0x0097,
			WA_Rank_Query_Resp=0x8097,

			//竞技排行
			WA_Sports_Query=0x0098,
			WA_Sports_Query_Resp=0x8098,

			//佣兵声望排行
			WA_Prestige_Query=0x0099,
			WA_Prestige_Query_Resp=0x8099,

			//副本积分排行
			WA_CopyIntegral_Query=0x0100,
			WA_CopyIntegral_Query_Resp=0x8100,

			//佣兵团排行
			WA_MercenaryGroup_Query=0x0101,
			WA_MercenaryGroup_Query_Resp=0x8101,

			//操作类:
			//宠物管理:添加宠物(等级\属性)
			WA_PetAdded_Operate=0x0102,
			WA_PetAdded_Operate_Resp=0x8102,
			
			//宠物管理:删除宠物(等级\属性)
			WA_PetDeleted_Operate=0x0103,
			WA_PetDeleted_Operate_Resp=0x8103,
			
			//家族管理
			WA_AttornPopedom_Operate=0x0104,
			WA_AttornPopedom_Operate_Resp=0x8104,
			
			//佣兵团管理:修改公会信息
			WA_ModifyGuildName_Operate=0x0105,
			WA_ModifyGuildName_Operate_Resp=0x8105,
			
			//佣兵团管理:踢出公会成员
			WA_KickGuildMember_Operate=0x0106,
			WA_KickGuildMember_Operate_Resp=0x8106,
			
			//佣兵团管理:解散公会
			WA_DissolveGuild_Operate=0x0107,
			WA_DissolveGuild_Operate_Resp=0x8107,
			
			//佣兵团仓库:删除物品
			WA_DeleteGoods_Operate=0x0108,
			WA_DeleteGoods_Operate_Resp=0x8108,
			
			//佣兵团仓库:添加物品
			WA_AddGoods_Operate=0x0109,
			WA_AddGoods_Operate_Resp=0x8109,
			
			//佣兵团仓库:修改金钱数量
			WA_ModifyMoney_Operate=0x0110,
			WA_ModifyMoney_Operate_Resp=0x8110,

			//宠物管理:宠物列表查询
			WA_PetList_Query=0x0111,
			WA_PetList_Query_Resp=0x8111,

			//恢复角色属性
			WA_ResumeAttribute_Op=0x0114,
			WA_ResumeAttribute_Op_Resp=0x8114,

			//批量发送邮件(无道具)
			WA_SendMails_Op=0x0115,
			WA_SendMails_Op_Resp=0x8115,


			//批量封停用户
			WA_ClosePlayerGroup=0x0116,
			WA_ClosePlayerGroup_Resp=0x8116,
			//End

			/////////<summary>
			///侠道飞车消息集(0x54)
			//////</summary>
			//查询用户信息
			CF_Account_Query=0x0001,
			CF_Account_Query_Resp=0x8001,
			
			//强制玩家下线
			CF_KickPlayer=0x0002,
			CF_KickPlayer_Resp=0x8002,
			
			//封停用户
			CF_ClosePlayer=0x0003,
			CF_ClosePlayer_Resp=0x8003,
			
			//解封用户
			CF_OpenPlayer=0x0004,
			CF_OpenPlayer_Resp=0x8004,
			
			//封停列表
			CF_CloseList=0x0005,
			CF_CloseList_Resp=0x8005,
			
			//单个封停
			CF_CloseSingle=0x0006,
			CF_CloseSingle_Resp=0x8006,
			
			//添加公告
			CF_BoardTasker_Add=0x0007,
			CF_BoardTasker_Add_Resp=0x8007,
			
			//删除公告
			CF_BoardTasker_Del=0x0008,
			CF_BoardTasker_Del_Resp=0x8008,
			
			//查询公告
			CF_BoardTasker_Query=0x0009,
			CF_BoardTasker_Query_Resp=0x8009,

			/////////<summary>
			///侠盗金刚消息集(0x55)
			//////</summary>
			//查询用户信息
			XDJG_Account_Query=0x0001,
			XDJG_Account_Query_Resp=0x8001,
			
			//强制玩家下线
			XDJG_KickPlayer=0x0002,
			XDJG_KickPlayer_Resp=0x8002,
			
			//封停用户
			XDJG_ClosePlayer=0x0003,
			XDJG_ClosePlayer_Resp=0x8003,
			
			//解封用户
			XDJG_OpenPlayer=0x0004,
			XDJG_OpenPlayer_Resp=0x8004,
			
			//封停列表
			XDJG_CloseList=0x0005,
			XDJG_CloseList_Resp=0x8005,
			
			//单个封停
			XDJG_CloseSingle=0x0006,
			XDJG_CloseSingle_Resp=0x8006,
			
			//添加公告
			XDJG_BoardTasker_Add=0x0007,
			XDJG_BoardTasker_Add_Resp=0x8007,
			
			//修改公告
			XDJG_BoardTasker_Update=0x0008,
			XDJG_BoardTasker_Update_Resp=0x8008,
			
			//删除公告
			XDJG_BoardTasker_Del=0x0009,
			XDJG_BoardTasker_Del_Resp=0x8009,
			
			//查询公告
			XDJG_BoardTasker_Query=0x0010,
			XDJG_BoardTasker_Query_Resp=0x8010,

			//Added by tengjie 2010-07-06
			//角色技能添加
			XDJG_RoleSkillAdd=0x0011,
			XDJG_RoleSkillAdd_Resp=0x8011,
			
			//角色技能删除
			XDJG_RoleSkillDel=0x0012,
			XDJG_RoleSkillDel_Resp=0x8012,
			
			//道具添加
			XDJG_ItemAdd=0x0013,
			XDJG_ItemAdd_Resp=0x8013,
			
			//道具删除
			XDJG_ItemDel=0x0014,
			XDJG_ItemDel_Resp=0x8014,
			
			//添加游戏币
			XDJG_MoneyAdd=0x0015,
			XDJG_MoneyAdd_Resp=0x8015,

			//日志类:玩家登录日志
			XDJG_Login_Log=0x0016,
			XDJG_Login_Log_Resp=0x8016,
			
			//商城物品购买日志
			XDJG_Shoping_Log=0x0017,
			XDJG_Shoping_Log_Resp=0x8017,
			
			//玩家物品日志:交易获得
			XDJG_TransGain_Log=0x0018,
			XDJG_TransGain_Log_Resp=0x8018,
			
			//玩家物品日志:交易失去
			XDJG_TransLose_Log=0x0019,
			XDJG_TransLose_Log_Resp=0x8019,
			
			//玩家物品日志:从NPC出购买
			XDJG_BuyFromNpc_Log=0x0020,
			XDJG_BuyFromNpc_Log_Resp=0x8020,
			
			//玩家物品日志:卖给NPC
			XDJG_SaleToNpc_Log=0x0021,
			XDJG_SaleToNpc_Log_Resp=0x8021,
			
			//玩家物品日志:拾取物品
			XDJG_PickUpItem_Log=0x0022,
			XDJG_PickUpItem_Log_Resp=0x8022,
			
			//玩家物品日志:丢弃物品
			XDJG_DiscardItem_Log=0x0023,
			XDJG_DiscardItem_Log_Resp=0x8023,
			
			//玩家物品日志:物品消耗
			XDJG_ConsumeItem_Log=0x0024,
			XDJG_ConsumeItem_Log_Resp=0x8024,
			
			//玩家物品日志:拍卖行卖出
			XDJG_AuctioneerSaled_Log=0x0025,
			XDJG_AuctioneerSaled_Log_Resp=0x8025,
			
			//玩家物品日志:拍卖行购入
			XDJG_AuctioneerBought_Log=0x0026,
			XDJG_AuctioneerBought_Log_Resp=0x8026,
			
			//玩家邮件日志:发送邮件
			XDJG_SendMail_Log=0x0027,
			XDJG_SendMail_Log_Resp=0x8027,
			
			//玩家邮件日志:收取邮件
			XDJG_ReceiveMail_Log=0x0028,
			XDJG_ReceiveMail_Log_Resp=0x8028,
			
			//M币使用日志:背包扩充续费记录
			XDJG_ExpansionPack_Log=0x0029,
			XDJG_ExpansionPack_Log_Resp=0x8029,
			
			//M币使用日志:传送日志
			XDJG_Deliver_Log=0x0030,
			XDJG_Deliver_Log_Resp=0x8030,
			
			//M币使用日志:角色仓库扩充续费记录
			XDJG_ExpansionWarehouse_Log=0x0031,
			XDJG_ExpansionWarehouse_Log_Resp=0x8031,
			
			//玩家角色日志:角色升级
			XDJG_RoleUpgrade_Log=0x0032,
			XDJG_RoleUpgrade_Log_Resp=0x8032,
			
			//玩家角色日志:角色创建
			XDJG_RoleCreate_Log=0x0033,
			XDJG_RoleCreate_Log_Resp=0x8033,
			
			//玩家角色日志:角色技能学习
			XDJG_RoleSkill_Log=0x0034,
			XDJG_RoleSkill_Log_Resp=0x8034,
			
			//玩家角色日志:领取任务
			XDJG_ReceiveTask_Log=0x0035,
			XDJG_ReceiveTask_Log_Resp=0x8035,
			
			//玩家角色日志:完成任务
			XDJG_CompleteTask_Log=0x0036,
			XDJG_CompleteTask_Log_Resp=0x8036,
			
			//玩家角色日志:副本查询
			XDJG_Copy_Log=0x0037,
			XDJG_Copy_Log_Resp=0x8037,

			//查询类
			//玩家物品信息:身上物品信息
			XDJG_ItemBody_Query=0x0038,
			XDJG_ItemBody_Query_Resp=0x8038,
			
			//玩家物品信息:背包物品信息
			XDJG_Itembackpack_Query=0x0039,
			XDJG_Itembackpack_Query_Resp=0x8039,
			
			//玩家物品信息:仓库物品信息
			XDJG_ItemStorage_Query=0x0040,
			XDJG_ItemStorage_Query_Resp=0x8040,
			
			//玩家物品信息:公会仓库信息
			XDJG_GuildStorage_Query=0x0041,
			XDJG_GuildStorage_Query_Resp=0x8041,
			
			//玩家物品信息:金刚仓库信息
			XDJG_KingkongStorage_Query=0x0042,
			XDJG_KingkongStorage_Query_Resp=0x8042,

			//邮件信息
			XDJG_MailInfo_Query=0x0043,
			XDJG_MailInfo_Query_Resp=0x8043,
			
			//拍卖行信息
			XDJG_AuctioneerInfo_Query=0x0044,
			XDJG_AuctioneerInfo_Query_Resp=0x8044,

			//操作类
			//用户角色信息的修改:修改等级
			XDJG_ModifyLevel_operate=0x0045,
			XDJG_ModifyLevel_operate_Resp=0x8045,
			
			//用户密码管理:修改密码
			XDJG_ModifyPwd_operate=0x0046,
			XDJG_ModifyPwd_operate_Resp=0x8046,
			
			//用户密码管理:临时密码恢复
			XDJG_ResumePwd_operate=0x0047,
			XDJG_ResumePwd_operate_Resp=0x8047,
			
			//用户角色管理:角色恢复
			XDJG_ResumeRole_operate=0x0048,
			XDJG_ResumeRole_operate_Resp=0x8048,
			
			//用户角色管理:角色删除
			XDJG_DeleteRole_operate=0x0049,
			XDJG_DeleteRole_operate_Resp=0x8049,

			//修改角色身上金钱数量(绑定币)
			XDJG_UpRoleBind_operate=0x0050,
			XDJG_UpRoleBind_operate_Resp=0x8050,
			
			//修改角色身上金钱数量(非绑定币)
			XDJG_UpRoleNonBind_operate=0x0051,
			XDJG_UpRoleNonBind_operate_Resp=0x8051,

			//角色技能信息查询
			XDJG_RoleSkills_Query=0x0052,
			XDJG_RoleSkills_Query_Resp=0x8052,
			
			//技能列表查询
			XDJG_SkillList_Query=0x0053,
			XDJG_SkillList_Query_Resp=0x8053,
			
			//道具类型查询
			XDJG_ItemType_Query=0x0054,
			XDJG_ItemType_Query_Resp=0x8054,
			
			//相应类型的道具列表查询
			XDJG_ItemNameLst_Query=0x0055,
			XDJG_ItemNameLst_Query_Resp=0x8055,

			//游戏币查询
			XDJG_MoneyQuery=0x0056,
			XDJG_MoneyQuery_Resp=0x8056,

			//添加角色身上金钱数量(绑定币)
			XDJG_AddRoleBind_operate=0x0057,
			XDJG_AddRoleBind_operate_Resp=0x8057,
			
			//添加角色身上金钱数量(非绑定币)
			XDJG_AddRoleNonBind_operate=0x0058,
			XDJG_AddRoleNonBind_operate_Resp=0x8058,

			//物品属性查询
			XDJG_ItemAttribute_Query=0x0059,
			XDJG_ItemAttribute_Query_Resp=0x8059,

			//公会信息查询
			//公会基本信息
			XDJG_GuildBaseInfo_Query=0x0060,
			XDJG_GuildBaseInfo_Query_Resp=0x8060,
			
			//公会成员列表
			XDJG_GuildMemberList_Query=0x0061,
			XDJG_GuildMemberList_Query_Resp=0x8061,
			
			//玩家任务查询
			XDJG_PlayerTaskInfo_Query=0x0062,
			XDJG_PlayerTaskInfo_Query_Resp=0x8062,
			
			//金刚车辆信息
			//金刚车辆基本信息
			XDJG_CarBaseInfo_Query=0x0063,
			XDJG_CarBaseInfo_Query_Resp=0x8063,
			
			//金刚车辆详细信息
			XDJG_CarDetailInfo_Query=0x0064,
			XDJG_CarDetailInfo_Query_Resp=0x8064,
			
			//玩家公会信息
			//玩家公会基本信息
			XDJG_PlayerGuildBaseInfo_Query=0x0065,
			XDJG_PlayerGuildBaseInfo_Query_Resp=0x8065,
			
			//玩家公会物品信息
			XDJG_PlayerGuildItemInfo_Query=0x0066,
			XDJG_PlayerGuildItemInfo_Query_Resp=0x8066,
			
			//玩家物品日志
			//装备增幅日志
			XDJG_EquipIncrease_Log=0x0067,
			XDJG_EquipIncrease_Log_Resp=0x8067,
			
			//插片合成日志
			XDJG_Synthesis_Log=0x0068,
			XDJG_Synthesis_Log_Resp=0x8068,
			
			//装备传导日志
			XDJG_EquipTransfer_Log=0x0069,
			XDJG_EquipTransfer_Log_Resp=0x8069,
			
			//物品鉴定
			XDJG_ItemIdentify_Log=0x0070,
			XDJG_ItemIdentify_Log_Resp=0x8070,
			
			//个人仓库存放日志
			XDJG_RoleStore_Log=0x0071,
			XDJG_RoleStore_Log_Resp=0x8071,
			
			//公会仓库存放日志
			XDJG_GuildStore_Log=0x0072,
			XDJG_GuildStore_Log_Resp=0x8072,
			
			//公会日志
			//公会成立
			XDJG_GuildCreate_Log=0x0073,
			XDJG_GuildCreate_Log_Resp=0x8073,
			
			//公会解散
			XDJG_GuildDismiss_Log=0x0074,
			XDJG_GuildDismiss_Log_Resp=0x8074,
			
			//公会添加成员
			XDJG_GuildAddMember_Log=0x0075,
			XDJG_GuildAddMember_Log_Resp=0x8075,
			
			//公会成员离开
			XDJG_GuildMemberLeaved_Log=0x0076,
			XDJG_GuildMemberLeaved_Log_Resp=0x8076,
			
			//公会职务变更
			XDJG_GuildPostChanged_Log=0x0077,
			XDJG_GuildPostChanged_Log_Resp=0x8077,

			//帐号是否封停
			XDJG_AccountIsClose_Query=0x0078,
			XDJG_AccountIsClose_Query_Log_Resp=0x8078,

			//NPC道具领取查询
			XDJG_NPCGetItem_Query=0x0079,
			XDJG_NPCGetItem_Query_Resp=0x8079,

			//添加称号
			XDJG_AddTitle_Query=0x0080,
			XDJG_AddTitle_Query_Resp=0x8080,
			
			//移除称号
			XDJG_RemoveTitle_Query=0x0081,
			XDJG_RemoveTitle_Query_Resp=0x8081,
			
			//添加BUFF
			XDJG_AddBuff_Query=0x0082,
			XDJG_AddBuff_Query_Resp=0x8082,
			
			//移除BUFF
			XDJG_RemoveBuff_Query=0x0083,
			XDJG_RemoveBuff_Query_Resp=0x8083,
			
			//移动到指定位置
			XDJG_MoveToPosition_Query=0x0084,
			XDJG_MoveToPosition_Query_Resp=0x8084,
			
			//移动到指定玩家身边
			XDJG_MoveToRole_Query=0x0085,
			XDJG_MoveToRole_Query_Resp=0x8085,
			
			//移动到目标玩家身边
			XDJG_MoveToTargetRole_Query=0x0086,
			XDJG_MoveToTargetRole_Query_Resp=0x8086,

			//称号列表查询
			XDJG_TitleList_Query=0x0087,
			XDJG_TitleList_Query_Resp=0x8087,
			
			//BUFF列表查询
			XDJG_BuffList_Query=0x0088,
			XDJG_BuffList_Query_Resp=0x8088,

			//End

			/// <summary>
			/// JW2_ADMIN 劲舞团工具操作消息集
			/// </summary>

			JW2_ACCOUNT_QUERY = 0x0001,//玩家帐号信息查询（1.2.3.4.5.8）
			JW2_ACCOUNT_QUERY_RESP = 0x8001,//玩家帐号信息查询响应（1.2.3.4.5.8）
			JW2_ACCOUNTREMOTE_QUERY = 0x0002,//游戏服务器封停的玩家帐号查询
			JW2_ACCOUNTREMOTE_QUERY_RESP = 0x8002,//游戏服务器封停的玩家帐号查询响应
			JW2_ACCOUNTLOCAL_QUERY = 0x0003,//本地封停的玩家帐号查询
			JW2_ACCOUNTLOCAL_QUERY_RESP = 0x8003,//本地封停的玩家帐号查询响应
			JW2_ACCOUNT_CLOSE = 0x0004,//封停的玩家帐号
			JW2_ACCOUNT_CLOSE_RESP = 0x8004,//封停的玩家帐号响应
			JW2_ACCOUNT_OPEN = 0x0005,//解封的玩家帐号
			JW2_ACCOUNT_OPEN_RESP = 0x8005,//解封的玩家帐号响应
			JW2_ACCOUNT_BANISHMENT_QUERY = 0x0006,//玩家封停帐号查询
			JW2_ACCOUNT_BANISHMENT_QUERY_RESP = 0x8006,//玩家封停帐号查询响应
			JW2_BANISHPLAYER=0x0007,//踢人
			JW2_BANISHPLAYER_RESP=0x8007,//踢人响应
			JW2_BOARDMESSAGE=0x0008,//公告
			JW2_BOARDMESSAGE_RESP=0x8008,//公告响应
			
			JW2_LOGINOUT_QUERY=0x0009,//玩家人物登入/登出信息
			JW2_LOGINOUT_QUERY_RESP=0x8009,//玩家人物登入/登出信息响应

			JW2_RPG_QUERY=0x0010,//查询故事情节（6）
			JW2_RPG_QUERY_RESP=0x8010,//查询故事情节响应（6）
					
			JW2_ITEMSHOP_BYOWNER_QUERY = 0x0011,////查看玩家身上道具信息（7－7）
			JW2_ITEMSHOP_BYOWNER_QUERY_RESP = 0x8011,////查看玩家身上道具信息响应（7－7）
			
			
			JW2_DUMMONEY_QUERY = 0x0012,////查询点数和虚拟币（7－8）
			JW2_DUMMONEY_QUERY_RESP = 0x8012,////查询点数和虚拟币响应（7－8）
			JW2_HOME_ITEM_QUERY = 0x0013,////查看房间物品清单与期限（7－9）
			JW2_HOME_ITEM_QUERY_RESP = 0x8013,////查看房间物品清单与期限响应（7－9）
			JW2_SMALL_PRESENT_QUERY = 0x0014,////查看购物，送礼（7－10）
			JW2_SMALL_PRESENT_QUERY_RESP = 0x8014,////查看购物，送礼响应（7－10）
			JW2_WASTE_ITEM_QUERY = 0x0015,////查看消耗性道具（7－11）
			JW2_WASTE_ITEM_QUERY_RESP = 0x8015,////查看消耗性道具响应（7－11）
			JW2_SMALL_BUGLE_QUERY = 0x0016,////查看小喇叭（7－12）
			JW2_SMALL_BUGLE_QUERY_RESP  = 0x8016,////查看小喇叭响应（7－12）
			
			
			
			JW2_WEDDINGINFO_QUERY=0x0017,//婚姻信息
			JW2_WEDDINGINFO_QUERY_RESP=0x8017,
			JW2_WEDDINGLOG_QUERY=0x0018,//婚姻历史
			JW2_WEDDINGLOG_QUERY_RESP=0x8018,
			JW2_WEDDINGGROUND_QUERY=0x0019,//结婚信息
			JW2_WEDDINGGROUND_QUERY_RESP=0x8019,
			JW2_COUPLEINFO_QUERY=0x0020,//情人信息
			JW2_COUPLEINFO_QUERY_RESP=0x8020,
			JW2_COUPLELOG_QUERY=0x0021,//情人历史
			JW2_COUPLELOG_QUERY_RESP=0x8021,
			
		
			JW2_FAMILYINFO_QUERY=0x0022,//家族信息
			JW2_FAMILYINFO_QUERY_RESP=0x8022,
			JW2_FAMILYMEMBER_QUERY=0x0023,//家族成员信息
			JW2_FAMILYMEMBER_QUERY_RESP=0x8023,
			JW2_SHOP_QUERY=0x0024,//商城信息
			JW2_SHOP_QUERY_RESP=0x8024,

			
			JW2_User_Family_Query=0x0025,//用户家族信息
			JW2_User_Family_Query_Resp=0x8025,

			JW2_FamilyItemInfo_Query=0x0026,//家族道具信息
			JW2_FamilyItemInfo_Query_Resp=0x8026,
			
			JW2_FamilyBuyLog_Query=0x0027,//家族购买日志
			JW2_FamilyBuyLog_Query_Resp=0x8027,
			
			JW2_FamilyTransfer_Query=0x0028,//家族转让信息
			JW2_FamilyTransfer_Query_Resp=0x8028,

			JW2_FriendList_Query=0x0029,//好友列表
			JW2_FriendList_Query_Resp=0x8029,


			JW2_BasicInfo_Query=0x0030,//基地信息查询
			JW2_BasicInfo_Query_Resp=0x8030,

			JW2_FamilyMember_Applying=0x0031,//申请中成员
			JW2_FamilyMember_Applying_Resp=0x8031,
			
			JW2_BasicRank_Query=0x0032,//基地等级
			JW2_BasicBank_Query_Resp=0x8032,

			///////////公告////////////////////////////
			JW2_BOARDTASK_INSERT = 0x0033,//公告添加
			JW2_BOARDTASK_INSERT_RESP = 0x8033,//公告添加响应
			JW2_BOARDTASK_QUERY = 0x0034,//公告查询
			JW2_BOARDTASK_QUERY_RESP = 0x8034,//公告查询响应
			JW2_BOARDTASK_UPDATE = 0x0035,//公告更新
			JW2_BOARDTASK_UPDATE_RESP = 0x8035,//公告更新响应

			JW2_ITEM_DEL=0x0036,//道具删除（身上0，物品栏1，礼盒2）
			JW2_ITEM_DEL_RESP=0x8036,//道具删除（身上0，物品栏1，礼盒2）

			JW2_MODIFYSEX_QUREY=0x0037,//修改角色性别
			JW2_MODIFYSEX_QUEYR_RESP=0x8037,

			JW2_MODIFYLEVEL_QUERY=0x0038,//修改等级
			JW2_MODIFYLEVEL_QUERY_RESP=0x8038,

			JW2_MODIFYEXP_QUERY=0x0039,//修改经验
			JW2_MODIFYEXP_QUERY_RESP=0x8039,
			
			JW2_BAN_BIGBUGLE=0x0040,//禁用大喇叭
			JW2_BAN_BIGBUGLE_RESP=0x8040,

			JW2_BAN_SMALLBUGLE=0x0041,//禁用小喇叭
			JW2_BAN_SMALLBUGLE_RESP=0x8041,

			JW2_DEL_CHARACTER=0x0042,//删除角色
			JW2_DEL_CHARACTER_RESP=0x8042,
			
			JW2_RECALL_CHARACTER=0x0043,//恢复角色
			JW2_RECALL_CHARACTER_RESP=0x8043,
			
			JW2_MODIFY_MONEY=0x0044,//修改金钱
			JW2_MODIFY_MONEY_RESP=0x8044,

			JW2_ADD_ITEM=0x0045,//增加道具
			JW2_ADD_ITEM_RESP=0x8045,

			JW2_CREATE_GM=0x0046,//每个大区创建GM
			JW2_CREATE_GM_RESP=0x8046,

			JW2_MODIFY_PWD=0x0047,//修改密码
			JW2_MODIFY_PWD_RESP=0x8047,

			JW2_RECALL_PWD=0x0048,//恢复密码
			JW2_RECALL_PWD_RESP=0x8048,

			JW2_ItemInfo_Query=0x0049,//道具查询
			JW2_ItemInfo_Query_Resp=0x8049,//

			
			JW2_ITEM_SELECT=0x0050,//道具模糊查询
			JW2_ITEM_SELECT_RESP=0x8050,//

			JW2_PetInfo_Query=0x0051,//宠物信息
			JW2_PetInfo_Query_Resp=0x8051,
		
			JW2_Messenger_Query=0x0052,//称号查询
			JW2_Messenger_Query_Resp=0x8052,

			JW2_Wedding_Paper=0x0053,//结婚证书
			JW2_Wedding_Paper_Resp=0x8053,

			JW2_CoupleParty_Card=0x0054,//情侣派对卡
			JW2_CoupleParty_Card_Resp=0x8054,


			JW2_MailInfo_Query=0x0055,//纸箱信息
			JW2_MailInfo_Query_Resp=0x8055,

			JW2_MoneyLog_Query=0x0056,//金钱日志查询
			JW2_MoneyLog_Query_Resp=0x8056,

			JW2_FamilyFund_Log=0x0057,//基金日志
			JW2_FamilyFund_Log_Resp=0x8057,

			JW2_FamilyItem_Log=0x0058,//家族道具日志
			JW2_FamilyItem_Log_Resp=0x8058,

			JW2_Item_Log=0x0059,//道具日志
			JW2_Item_Log_Resp=0x8059,

			JW2_Task_Log=0x0060,//任务日志
			JW2_Task_Log_Resp=0x8060,

			JW2_CashMoney_Log=0x0061,//消费日志
			JW2_CashMoney_Log_Resp=0x8061,


			JW2_Import_KickPlayer=0x0062,//批量踢人
			JW2_Import_KickPlayer_Resp=0x8062,
			
			JW2_Import_AccountClose=0x0063,//批量封停
			JW2_Import_AccountClose_Resp=0x8063,
			
			JW2_Import_AccountOpen=0x0064,//批量封停
			JW2_Import_AccountOpen_Resp=0x8064,

			JW2_GM_Update=0x0065,//更改GM状态
			JW2_GM_Update_Resp=0x8065,

			JW2_ProductManage_Insert=0x0066,//插入权限
			JW2_ProductManage_Insert_Resp=0x8066,

			JW2_ProductManage_View=0x0067,//查看权限
			JW2_ProductManage_View_Resp=0x8067,
			
			JW2_ProductManage_Del=0x0068,//删除权限
			JW2_ProductManage_Del_Resp=0x8068,

			//maple add
			JW2_ProductManage_Validate=0x0069,//验证权限
			JW2_ProductManage_Validate_Resp=0x8069,

			JW2_ProductManage_Deduct=0x0070,///验证通过减去权限
			JW2_ProductManage_Deduct_Resp=0x8070,
			
			
			//////////////////
			//CG2(0x71)
			/////////////////
			CG2_Account_Query = 0x0001,//角色基本资料
			CG2_Account_Query_Resp = 0x8001,//角色基本资料
			CG2_Account_Detail_Query = 0x0002, //角色详细资料
			CG2_Account_Detail_Query_Resp = 0x8002, //角色详细资料
			CG2_Account_Attribute_Query = 0x0003,//角色属性资料
			CG2_Account_Attribute_Query_Resp = 0x8003,//角色属性资料
			CG2_Account_Guild_Query = 0x0004,//角色工会资料
			CG2_Account_Guild_Query_Resp = 0x8004,//角色工会资料
			CG2_Account_Title_Query = 0x0005,//角色称号资料
			CG2_Account_Title_Query_Resp = 0x8005,//角色称号资料
			CG2_Account_Address_Query = 0x0006,//角色社会关系
			CG2_Account_Address_Query_Resp = 0x8006,//角色社会关系
			CG2_Account_Skill_Query = 0x0007,//角色技能信息
			CG2_Account_Skill_Query_Resp = 0x8007,//角色技能信息
			//背包物品资料
			CG2_CharItem_Query = 0x0008,//角色身上道具信息
			CG2_CharItem_Query_Resp = 0x8008,//角色身上道具信息
			CG2_TempItem_Query = 0x0009,//临时背包信息
			CG2_TempItem_Query_Resp = 0x8009,//临时背包信息
			CG2_ShopItem_Query = 0x0010,//商城物品领取
			CG2_ShopItem_Query_Resp = 0x8010,//商城物品领取
			//银行物品资料
			CG2_BankInfo_Query = 0x0011,//银行存款信息
			CG2_BankInfo_Query_Resp = 0x8011,//银行存款信息
			CG2_BankItem_Query = 0x0012,//银行物品信息
			CG2_BankItem_Query_Resp = 0x8012,//银行物品信息
			//宠物信息
			CG2_PetInfo_Query = 0x0013,//宠物基本资料
			CG2_PetInfo_Query_Resp = 0x8013,//宠物基本资料
			CG2_PetInfo_Detail_Query = 0x0014,//宠物详细资料
			CG2_PetInfo_Detail_Query_Resp = 0x8014,//宠物详细资料
			CG2_PetItem_Query = 0x0015,//宠物背包信息
			CG2_PetItem_Query_Resp = 0x8015,//	宠物背包信息
			CG2_PetSkill_Query = 0x0016,//宠物技能信息
			CG2_PetSkill_Query_Resp = 0x8016,//	宠物技能信息
			//任务
			CG2_Job_Query = 0x0017,	//任务 
			CG2_Job_Query_Resp = 0x8017,//任务 
			//账号激活查询
			CG2_Account_Active_Query = 0x0018,	//通过账号查询
			CG2_Account_Active_Query_Resp = 0x8018,//通过账号查询
			CG2_Sn_Active_Query  = 0x0019,		//通过激活码查询
			CG2_Sn_Active_Query_Resp  = 0x8019,//通过激活码查询
			//系统踢人
			CG2_Ban_Query = 0x0020,//根据选择时间范围、次数进行筛选，将指定服务器中符合条件的角色
			CG2_Ban_Query_Resp = 0x8020,//根据选择时间范围、次数进行筛选，将指定服务器中符合条件的角色
			CG2_Account_Ban_Query = 0x0021,//根据选择时间范围以账号、昵称进行查询，显示踢人次数
			CG2_Account_Ban_Query_Resp = 0x8021,//根据选择时间范围以账号、昵称进行查询，显示踢人次数
			
			//帐号登陆信息
			CG2_Use_Query = 0x0022,	//帐号登陆信息
			CG2_Use_Query_Resp = 0x8022,//帐号登陆信息
			CG2_Account_Ip_Query = 0x0023,	//通过IP地址查询所登陆的帐号
			CG2_Account_Ip_Query_Resp = 0x8023,//通过IP地址查询所登陆的帐号
			CG2_Account_User_Query = 0x0024,	//通过帐号查询历史登陆IP
			CG2_Account_User_Query_Resp = 0x8024,//通过帐号查询历史登陆IP
			CG2_UseLog_Query = 0x0025,	//邮件日志查询信息
			CG2_UseLog_Query_Resp = 0x8025,//邮件日志查询信息

			//操作类
			//道具
			CG2_Get_Item_Query = 0x0026,//获取游戏道具（大类）
			CG2_Get_Item_Query_Resp = 0x8026,//获取游戏道具（大类）
			CG2_Get_Item_Detail_Query = 0x0027,	//获取游戏道具（小类）
			CG2_Get_Item_Detail_Query_Resp = 0x8027,	//获取游戏道具（小类）

			
			//账号
			CG2_Account_Close = 0x0028,//封停玩家帐号
			CG2_Account_Close_RESP = 0x8028,//封停玩家帐号响应
			CG2_Account_Open = 0x0029,//解封玩家帐号
			CG2_Account_Open_RESP= 0x8030,//解封玩家帐号响应
			CG2_Account_BANISHMENT_QUERY = 0x0030,//玩家封停帐号查询状态
			CG2_Account_BANISHMENT_QUERY_RESP = 0x8030,//玩家封停帐号查询状态响应
			CG2_BanAccount_Query = 0x0031,//查询所有封停玩家帐号信息
			CG2_BanAccount_Query_Resp = 0x8031,//查询所有封停玩家帐号信息
			CG2_KickAccount_Query = 0x0032,//踢下线
			CG2_KickAccount_Query_Resp = 0x8032,//踢下线

			//地图
			CG2_Change_Map_Query = 0x0033,//地图传送
			CG2_Change_Map_Query_Resp = 0x8033,//地图传送

			//公告
			CG2_SendNote_Query = 0x0034,//发送公告
			CG2_SendNote_Query_Resp = 0x8034,//发送公告
			CG2_UpdateNote_Query = 0x0035,//修改公告
			CG2_UpdateNote_Query_Resp= 0x8035,//修改公告
			CG2_DeleteNote_Query = 0x0036,//删除公告
			CG2_DeleteNote_Query_Resp= 0x8036,//删除公告

			//角色
			CG2_Update_Account_Query = 0x0037,//修改角色信息
			CG2_Update_Account_Query_Resp = 0x8037,//修改角色信息
			CG2_Update_Pet_Query = 0x0038,//修改宠物信息
			CG2_Update_Pet_Query_Resp = 0x8038,//修改宠物信息

			//技能
			CG2_Insert_Account_Skill_Query = 0x0039,//添加角色技能信息
			CG2_Insert_Account_Skill_Query_Resp = 0x8039,//添加角色技能信息
			CG2_Insert_Pet_Skill_Query = 0x0040,//添加宠物技能信息
			CG2_Insert_Pet_Skill_Query_Resp = 0x8040,//添加宠物技能信息

			CG2_Update_Account_Skill_Query = 0x0041,//修改角色技能信息
			CG2_Update_Account_Skill_Query_Resp = 0x8041,//修改角色技能信息
			CG2_Update_Pet_Skill_Query = 0x0042,//修改宠物技能信息
			CG2_Update_Pet_Skill_Query_Resp = 0x8042,//修改宠物技能信息

			CG2_Delete_Account_Skill_Query = 0x0043,//删除角色技能信息
			CG2_Delete_Account_Skill_Query_Resp = 0x8043,//删除角色技能信息
			CG2_Delete_Pet_Skill_Query = 0x0044,//删除宠物技能信息
			CG2_Delete_Pet_Skill_Query_Resp = 0x8044,//删除宠物技能信息

			//临时账号
			CG2_Insert_TmpPwd_Query = 0x0045,//添加临时密码
			CG2_Insert_TmpPwd_Query_Resp = 0x8045,//添加临时密码
			CG2_Recover_TmpPwd_Query = 0x0046,//恢复用户密码
			CG2_Recover_TmpPwd_Query_Resp = 0x8046,//恢复用户密码

			CG2_Insert_Item_Query = 0x0047,//添加游戏道具
			CG2_Insert_Item_Query_Resp = 0x8047,//添加游戏道具
			CG2_RealAccount_Query = 0x0048,//查询服务器用户基本资料
			CG2_RealAccount_Query_Resp = 0x8048,//查询服务器用户基本资料
			
			CG2_SearchItem_Query = 0x0049,//模糊查询道具
			CG2_SearchItem_Query_Resp = 0x8049,//模糊查询道具

			CG2_MapList_Query = 0x0051,//查询地图列表
			CG2_MapList_Query_Resp = 0x8051,//查询地图列表
			CG2_SearchNote_Query = 0x0050,//查看公告
			CG2_SearchNote_Query_Resp= 0x8050,//查看公告

			CG2_SearchGuild_Query = 0x0052,//查询工会信息
			CG2_SearchGuild_Query_Resp = 0x8052,//查询工会信息

			CG2_SearchGuild_Detail_Query = 0x0053,//查询工会详细信息
			CG2_SearchGuild_Detail_Query_Resp = 0x8053,//查询工会详细信息

			CG2_CloseGuild_Query = 0x0054,//解散公会
			CG2_CloseGuild_Query_Resp = 0x8054,//解散公会

			CG2_RenameGuild_Query = 0x0055,//修改公会名字
			CG2_RenameGuild_Query_Resp = 0x8055,//修改公会名字

			CG2_Recovery_Account_Qurey = 0x0056,//恢复角色信息
			CG2_Recovery_Account_Qurey_Resp = 0x8056,//恢复角色信息

			CG2_Search_Account_Qurey = 0x0057,//查询角色存在
			CG2_Search_Account_Qurey_Resp = 0x8057,//查询角色存在

			CG2_BakAccount_Query = 0x0058,//查询备份用户
			CG2_BakAccount_Query_Resp = 0x8058,//查询备份用户
			
			//发送问答系统
			CG2_SendAsk_Query = 0x0059,//发送问答系统
			CG2_SendAsk_Query_Resp = 0x8059,//发送问答系统

			//修改角色属性
			CG2_UpdateAccount_Query = 0x0060,//修改角色属性
			CG2_UpdateAccount_Query_Resp = 0x8060,//修改角色属性

			//修改角色等级
			CG2_UpdateLevel_Query = 0x0061,//修改角色等级
			CG2_UpdateLevel_Query_Resp = 0x8061,//修改角色等级

			//修改角色职业
			CG2_UpdateJob_Query = 0x0062,//修改角色职业
			CG2_UpdateJob_Query_Resp = 0x8062,//修改角色职业

			//获取角色职业
			CG2_GetJob_Query = 0x0063,//获取角色职业
			CG2_GetJob_Query_Resp = 0x8063,//获取角色职业
			CG2_PackItem_Query = 0x0064,//背包道具信息
			CG2_PackItem_Query_Resp = 0x8064,//背包道具信息
			CG2_GetSkill_Query = 0x0065,//获得人物技能类型
			CG2_GetSkill_Query_Resp = 0x8065,//获得人物技能类型
			
			CG2_SkillList_Query = 0x0066,//获得人物技能列表
			CG2_SkillList_Query_Resp = 0x8066,//获得人物技能列表


			CG2_GetPetSkill_Query = 0x0067,//获得宠物技能类型
			CG2_GerPetSkill_Query_Resp = 0x8067,//获得宠物技能类型
			
			CG2_PetSkillList_Query = 0x0068,//获得宠物技能列表
			CG2_PetSkillList_Query_Resp = 0x8068,//获得宠物技能列表

			//修改银行资料信息
			CG2_InsertBankItem_Query = 0x0069,//银行道具添加
			CG2_InsertBankItem_Query_Resp = 0x8069,//银行道具添加

			CG2_SetBankMoney_Query = 0x0070,//银行金额设置
			CG2_SetBankMoney_Query_Resp = 0x8070,//银行金额设置

			//修改宠物资料信息
			CG2_ModiPetAttribute_Query = 0x0071,//修改宠物属性信息
			CG2_ModiPetAttribute_Query_Resp = 0x8071,//

			CG2_PetList_Query = 0x0072,//获取宠物列表
			CG2_PetList_Query_Resp = 0x8072,//获取宠物列表

			CG2_AddPet_Query = 0x0073,//添加宠物
			CG2_AddPet_Query_Resp = 0x8073,//添加宠物
			//恢复日志道具
			CG2_LogItem_Query = 0x0074,//查看日志道具信息
			CG2_LogItem_Query_Resp = 0x8074,//查看日志道具信息

			CG2_RecoverLogItem_Query = 0x0075,//恢复日志道具信息
			CG2_RecoverLogItem_Query_Resp = 0x8075,//恢复日志道具信息

			CG2_Account_Teach_Query = 0x0076,//查看玩家师生关系
			CG2_Account_Teach_Query_Resp = 0x8076,//查看玩家师生关系

			CG2_Send_ItemPassword = 0x0077,//获取道具锁
			CG2_Send_ItemPassword_Resp = 0x8077,//获取道具锁

			CG2_SearchPassWord_Query = 0x0078,//查询临时密码
			CG2_SearchPassWord_Query_Resp = 0x8078,//查询临时密码

			CG2_InsertItemALL_Query = 0x0079,//添加道具（导入）
			CG2_InsertItemALL_Query_Resp = 0x8079,//添加道具（导入）

			CG2_UserShoppingInfo_Query = 0x0080,//商城信息查询
			CG2_UserShoppingInfo_Query_Resp = 0x8080,//商城信息查询

			CG2_UserGoldCard_Query = 0x0081,//黄金激活卡
			CG2_UserGoldCard_Query_Resp = 0x8081,//黄金激活卡

			CG2_InsertBankItemALL_Query = 0x0082,//批量银行道具添加（导入）
			CG2_InsertBankItemALL_Query_Resp = 0x8082,//批量银行道具添加（导入）

			CG2_SearchMap_Query = 0x0083,//模糊查询地图
			CG2_SearchMap_Query_Resp = 0x8083,//模糊查询地图

			CG2_PetPic_Query = 0x0084,//宠物图鉴信息
			CG2_PetPic_Query_Resp = 0x8084,//宠物图鉴信息

			CG2_UpdatePetLevel_Query = 0x0085,//修改宠物等级
			CG2_UpdatePetLevel_Query_Resp = 0x8085,//修改宠物等级

			CG2_BankPet_Query = 0x0086,//银行宠物信息
			CG2_BankPet_Query_Resp = 0x8086,//银行宠物信息

			CG2_UpdateGuildLv_Query=0x0087,//工会等级修改
			CG2_UpdateGuildLv_Query_Resp=0x8087,//工会等级修改

			CG2_UpdateGuildHonor_Query=0x0088,//工会荣誉修改
			CG2_UpdateGuildHonor_Query_Resp=0x8088,//工会荣誉修改

			CG2_LogInfo_Pet_Query = 0x0089,//宠物详细属性
			CG2_LogInfo_Pet_Query_Resp = 0x8089,//宠物详细属性

			CG2_LogInfo_Item_Query = 0x0090,//道具详细属性
			CG2_LogInfo_Item_Query_Resp = 0x8090,//道具详细属性

			CG2_AccountErr_Query = 0x0091,//人物等级异常回档
			CG2_AccountErr_Query_Resp = 0x8091,//人物等级异常回档

			CG2_ChangeGold_Query = 0x0092,//修改随身金钱
			CG2_ChangeGold_Query_Resp = 0x8092,//修改随身金钱

			CG2_BagItem_Del_Query = 0x0093,//删除角色道具栏道具
			CG2_BagItem_Del_Query_Resp = 0x8093,//删除角色道具栏道具

			CG2_TmpItem_Del_Query = 0x0094,//删除临时道具栏道具
			CG2_TmpItem_Del_Query_Resp = 0x8094,//删除临时道具栏道具

			CG2_EquipItem_Del_Query = 0x0095,//删除随身装备道具
			CG2_EquipItem_Del_Query_Resp = 0x8095,//删除随身装备道具

			CG2_BankItem_Del_Query = 0x0096,//删除银行道具
			CG2_BankItem_Del_Query_Resp = 0x8096,//删除银行道具

			CG2_PetPic_Del_Query = 0x0097,//删除银行道具
			CG2_PetPic_Del_Query_Resp = 0x8097,//删除银行道具

			CG2_KickAccountCount_Query = 0x0098,//踢下线次数
			CG2_KickAccountCount_Query_Resp = 0x8098,//踢下线次数

			CG2_RecoverLogPet_Query = 0x0099,//恢复日志宠物
			CG2_RecoverLogPet_Query_Resp = 0x8099,//恢复日志宠物

			CG2_LoveTree_Query = 0x0100,//爱情树信息查询
			CG2_LoveTree_Query_Resp = 0x8100,//爱情树信息查询

			CG2_GuildMazeWar_Query = 0x0101,//公会迷宫占领
			CG2_GuildMazeWar_Query_Resp = 0x8101,//公会迷宫占领

			CG2_Marriage_Info_Query = 0x0102,//玩家婚姻信息
			CG2_Marriage_Info_Query_Resp = 0x8102,//玩家婚姻信息

			CG2_ChangeCharNo_Query = 0x0103,//角色换位
			CG2_ChangeCharNo_Query_Resp = 0x8103,//角色换位

			CG2_Log_Query=0x0104,
			CG2_Log_Query_Resp=0x8104,////查询玩家日志信息

			CG2_Get_EmailAddress=0x0105,//得到Email地址
			CG2_Get_EmailAddress_Resp=0x8105,//



			/// <summary>
			/// SDO_ADMIN 超级舞者工具操作消息集
			/// </summary>
			SDO_ACCOUNT_QUERY = 0x0026,//查看玩家的帐号信息
			SDO_ACCOUNT_QUERY_RESP = 0x8026,//查看玩家的帐号信息响应
			SDO_CHARACTERINFO_QUERY = 0x0027,//查看任务资料的信息
			SDO_CHARACTERINFO_QUERY_RESP = 0x8027,//查看人物资料的信息响应
			SDO_ACCOUNT_CLOSE = 0x0028,//封停帐户的权限信息
			SDO_ACCOUNT_CLOSE_RESP = 0x8028,//封停帐户的权限信息响应
			SDO_ACCOUNT_OPEN = 0x0029,//解封帐户的权限信息
			SDO_ACCOUNT_OPEN_RESP = 0x8029,//解封帐户的权限信息响应
			SDO_PASSWORD_RECOVERY = 0x0030,//玩家找回密码
			SDO_PASSWORD_RECOVERY_RESP = 0x8030,//玩家找回密码响应
			SDO_CONSUME_QUERY = 0x0031,//查看玩家的消费记录
			SDO_CONSUME_QUERY_RESP = 0x8031,//查看玩家的消费记录响应
			SDO_USERONLINE_QUERY = 0x0032,//查看玩家上下线状态
			SDO_USERONLINE_QUERY_RESP = 0x8032,//查看玩家上下线状态响应
			SDO_USERTRADE_QUERY = 0x0033,//查看玩家交易状态
			SDO_USERTRADE_QUERY_RESP = 0x8033,//查看玩家交易状态响应
			SDO_CHARACTERINFO_UPDATE = 0x0034,//修改玩家的账号信息
			SDO_CHARACTERINFO_UPDATE_RESP = 0x8034,//修改玩家的账号信息响应
			SDO_ITEMSHOP_QUERY = 0x0035,//查看游戏里面所有道具信息
			SDO_ITEMSHOP_QUERY_RESP = 0x8035,//查看游戏里面所有道具信息响应
			SDO_ITEMSHOP_DELETE = 0x0036,//删除玩家道具信息
			SDO_ITEMSHOP_DELETE_RESP  = 0x8036,//删除玩家道具信息响应
			SDO_GIFTBOX_CREATE  = 0x0037,//添加玩家礼物盒道具信息
			SDO_GIFTBOX_CREATE_RESP  = 0x8037,//添加玩家礼物盒道具信息响应
			SDO_GIFTBOX_QUERY = 0x0038,//查看玩家礼物盒的道具
			SDO_GIFTBOX_QUERY_RESP = 0x8038,//查看玩家礼物盒的道具响应
			SDO_GIFTBOX_DELETE = 0x0039,//删除玩家礼物盒的道具
			SDO_GIFTBOX_DELETE_RESP = 0x8039,//删除玩家礼物盒的道具响应
			SDO_USERLOGIN_STATUS_QUERY = 0x0040,//查看玩家登录状态
			SDO_USERLOGIN_STATUS_QUERY_RESP = 0x8040,//查看玩家登录状态响应
			SDO_ITEMSHOP_BYOWNER_QUERY = 0x0041,////查看玩家身上道具信息
			SDO_ITEMSHOP_BYOWNER_QUERY_RESP = 0x8041,////查看玩家身上道具信息
			SDO_ITEMSHOP_TRADE_QUERY = 0x0042,//查看玩家交易记录信息
			SDO_ITEMSHOP_TRADE_QUERY_RESP = 0x8042,//查看玩家交易记录信息的响应
			SDO_MEMBERSTOPSTATUS_QUERY = 0x0043,//查看该帐号状态
			SDO_MEMBERSTOPSTATUS_QUERY_RESP = 0x8043,///查看该帐号状态的响应
			SDO_MEMBERBANISHMENT_QUERY = 0x0044,//查看所有停封的帐号
			SDO_MEMBERBANISHMENT_QUERY_RESP = 0x8044,//查看所有停封的帐号响应
			SDO_USERMCASH_QUERY = 0x0045,//玩家充值记录查询
			SDO_USERMCASH_QUERY_RESP = 0x8045,//玩家充值记录查询响应
			SDO_USERGCASH_UPDATE = 0x0046,//补偿玩家G币
			SDO_USERGCASH_UPDATE_RESP = 0x8046,//补偿玩家G币的响应
			SDO_MEMBERLOCAL_BANISHMENT = 0x0047,//本地保存停封信息
			SDO_MEMBERLOCAL_BANISHMENT_RESP = 0x8047,//本地保存停封信息响应
			SDO_EMAIL_QUERY = 0x0048,//得到玩家的EMAIL
			SDO_EMAIL_QUERY_RESP = 0x8048,//得到玩家的EMAIL响应
			SDO_USERCHARAGESUM_QUERY = 0x0049,//得到充值记录总和
			SDO_USERCHARAGESUM_QUERY_RESP = 0x8049,//得到充值记录总和响应
			SDO_USERCONSUMESUM_QUERY = 0x0050,//得到消费记录总和
			SDO_USERCONSUMESUM_QUERY_RESP = 0x8050,//得到消费记录总和响应
			SDO_USERGCASH_QUERY = 0x0051,//玩家Ｇ币记录查询
			SDO_USERGCASH_QUERY_RESP = 0x8051,//玩家Ｇ币记录查询响应
			SDO_CHALLENGE_QUERY = 0x0052,
			SDO_CHALLENGE_QUERY_RESP = 0x8052,
			SDO_CHALLENGE_INSERT = 0x0053,
			SDO_CHALLENGE_INSERT_RESP = 0x8053,
			SDO_CHALLENGE_UPDATE = 0x0054,
			SDO_CHALLENGE_UPDATE_RESP = 0x8054,
			SDO_CHALLENGE_DELETE = 0x0055,
			SDO_CHALLENGE_DELETE_RESP = 0x8055,
			SDO_MUSICDATA_QUERY = 0x0056,
			SDO_MUSICDATA_QUERY_RESP = 0x8056,
			SDO_MUSICDATA_OWN_QUERY = 0x0057,
			SDO_MUSICDATA_OWN_QUERY_RESP = 0x8057,
			SDO_CHALLENGE_SCENE_QUERY = 0x0058,
			SDO_CHALLENGE_SCENE_QUERY_RESP = 0x8058,
			SDO_CHALLENGE_SCENE_CREATE = 0x0059,
			SDO_CHALLENGE_SCENE_CREATE_RESP = 0x8059,
			SDO_CHALLENGE_SCENE_UPDATE = 0x0060,
			SDO_CHALLENGE_SCENE_UPDATE_RESP = 0x8060,
			SDO_CHALLENGE_SCENE_DELETE = 0x0061,
			SDO_CHALLENGE_SCENE_DELETE_RESP = 0x8061,
			SDO_MEDALITEM_CREATE = 0x0062,
			SDO_MEDALITEM_CREATE_RESP = 0x8062,
			SDO_MEDALITEM_UPDATE = 0x0063,
			SDO_MEDALITEM_UPDATE_RESP = 0x8063,
			SDO_MEDALITEM_DELETE = 0x0064,
			SDO_MEDALITEM_DELETE_RESP = 0x8064,
			SDO_MEDALITEM_QUERY = 0x0065,
			SDO_MEDALITEM_QUERY_RESP = 0x8065,
			SDO_MEDALITEM_OWNER_QUERY = 0x0066,
			SDO_MEDALITEM_OWNER_QUERY_RESP = 0x8066,
			SDO_MEMBERDANCE_OPEN = 0x0067,
			SDO_MEMBERDANCE_OPEN_RESP = 0x8067,
			SDO_MEMBERDANCE_CLOSE = 0x0068,
			SDO_MEMBERDANCE_CLOSE_RESP = 0x8068,
			SDO_USERNICK_UPDATE =0x0069, 
			SDO_USERNICK_UPDATE_RESP =0x8069, 
			SDO_PADKEYWORD_QUERY = 0x0070,
			SDO_PADKEYWORD_QUERY_RESP = 0x8070,
			SDO_BOARDMESSAGE_REQ = 0x0071,
			SDO_BOARDMESSAGE_REQ_RESP = 0x8071,
			SDO_CHANNELLIST_QUERY =  0x0072,
			SDO_CHANNELLIST_QUERY_RESP = 0x8072,
			SDO_ALIVE_REQ = 0x0073,
			SDO_ALIVE_REQ_RESP = 0x8073,
			SDO_BOARDTASK_QUERY = 0x0074,
			SDO_BOARDTASK_QUERY_RESP = 0x8074,
			SDO_BOARDTASK_UPDATE = 0x0075,
			SDO_BOARDTASK_UPDATE_RESP = 0x8075,
			SDO_BOARDTASK_INSERT = 0x0076,
			SDO_BOARDTASK_INSERT_RESP = 0x8076,
			SDO_DAYSLIMIT_QUERY = 0x0077,
			SDO_DAYSLIMIT_QUERY_RESP = 0x8077,
			SDO_USERLOGIN_DEL = 0x0078,
			SDO_USERLOGIN_DEL_RESP = 0x8078,
			SDO_GATEWAY_QUERY = 0x0079,
			SDO_GATEWAY_QUERY_RESP = 0x8079,
			SDO_USERLOGIN_CLEAR = 0x0080,
			SDO_USERLOGIN_CLEAR_RESP = 0x8080,
			SDO_CHALLENGE_INSERTALL = 0x0081,
			SDO_CHALLENGE_INSERTALL_RESP = 0x8081,
			SDO_USERPASSWD_QUERY  = 0x0082,
			SDO_USERPASSWD_QUERY_RESP  = 0x8082,
			SDO_USERONLINETIME_QUERY  = 0x0083,
			SDO_USERONLINETIME_QUERY_RESP  = 0x8083,
			SDO_YYHAPPYITEM_QUERY = 0x0084,
			SDO_YYHAPPYITEM_QUERY_RESP = 0x8084,
			SDO_YYHAPPYITEM_CREATE = 0x0085,
			SDO_YYHAPPYITEM_CREATE_RESP = 0x8085,
			SDO_YYHAPPYITEM_UPDATE = 0x0086,
			SDO_YYHAPPYITEM_UPDATE_RESP = 0x8086,
			SDO_YYHAPPYITEM_DELETE = 0x0087,
			SDO_YYHAPPYITEM_DELETE_RESP = 0x8087,
			SDO_USER_PUNISH = 0x0088,
			SDO_USER_PUNISH_RESP = 0x8088,		
			SDO_PUNISHUSER_IMPORT_QUERY = 0x0089,
			SDO_PUNISHUSER_IMPORT_QUERY_RESP = 0x8089,
			SDO_PUNISHUSER_IMPORT = 0x0090,
			SDO_PUNISHUSER_IMPORT_RESP = 0x8090,
			SDO_USER_PUNISHALL = 0x0091,
			SDO_USER_PUNISHALL_RESP = 0x8091,
			SDO_FRAGMENT_QUERY = 0x0092,
			SDO_FRAGMENT_QUERY_RESP = 0x8092,
			SDO_FRAGMENT_UPDATE = 0x0093,
			SDO_FRAGMENT_UPDATE_RESP = 0x8093,
			SDO_PFUNCTIONITEM_QUERY = 0x0094,
			SDO_PFUNCTIONITEM_QUERY_RESP = 0x8094,
			SDO_USERMARRIAGE_QUERY = 0x0095,
			SDO_USERMARRIAGEQUERY_RESP = 0x8095,
			SDO_PUNISH_UPDATE_QUERY = 0x0096,
			SDO_PUNISH_UPDATE_QUERY_RESP = 0x8096,
			SDO_REMAINCASH_QUERY = 0x0097,
			SDO_REMAINCASH_QUERY_RESP = 0x8097,
			SDO_PADREGIST_QUERY = 0x0098,
			SDO_PADREGIST_QUERY_RESP = 0x8098,

			SDO_MiniDancerTime_QUERY = 0x0099,
			SDO_MiniDancerTime_QUERY_RESP = 0x8099,
			SDO_RewardItem_QUERY = 0x0100,
			SDO_RewardItem_QUERY_RESP = 0x8100,
			SDO_QueryPunishusertimes_QUERY = 0x0101,
			SDO_QueryPunishusertimes_QUERY_RESP = 0x8101,
			SDO_QueryDeleteItem_QUERY = 0x0102,
			SDO_QueryDeleteItem_QUERY_RESP = 0x8102,
			SDO_NotReachMoney_Query = 0x0103,
			SDO_NotReachMoney_Query_RESP = 0x8103,
			SDO_DoubleMoneyExp_Insert = 0x0104,
			SDO_DoubleMoneyExp_Insert_RESP = 0x8104,
			SDO_DoubleMoneyExp_Query = 0x0105,
			SDO_DoubleMoneyExp_Query_RESP = 0x8105,
			SDO_DoubleMoneyExp_Del = 0x0106,
			SDO_DoubleMoneyExp_Del_RESP = 0x8106,
			SDO_DoubleMoneyExp_Update = 0x0107,
			SDO_DoubleMoneyExp_Update_RESP = 0x8107,
			SDO_UserCheat_Query = 0x0108,
			SDO_UserCheat_Query_RESP = 0x8108,
			SDO_UserCheat_Del = 0x0109,
			SDO_UserCheat_Del_RESP = 0x8109,
			SDO_PetInfo_Query = 0x0110,
			SDO_PetInfo_Query_RESP = 0x8110,
			SDO_StageAward_Query = 0x0111,
			SDO_StageAward_Query_RESP = 0x8111,
			SDO_StageAward_Create = 0x0112,
			SDO_StageAward_Create_RESP = 0x8112,
			SDO_StageAward_Delete = 0x0113,
			SDO_StageAward_Delete_RESP = 0x8113,
			SDO_StageAward_Update = 0x0114,
			SDO_StageAward_Update_RESP = 0x8114,
			SDO_PetDrop_Query = 0x0115,
			SDO_PetDrop_Query_RESP = 0x8115,
			SDO_BatchItemShop_Insert = 0x0116,
			SDO_BatchItemShop_Insert_RESP = 0x8116,
			SDO_UserBugle_Query = 0x0117,
			SDO_UserBugle_Query_RESP = 0x8117,
			SDO_UserMsg_Query = 0x0118,
			SDO_UserMsg_Query_RESP = 0x8118,
			SDO_UserActive_Query = 0x0119,
			SDO_UserActive_Query_RESP = 0x8119,
			SDO_BAOXIANGRate_Query= 0x0120,
			SDO_BAOXIANGRate_Query_RESP= 0x8120,
			SDO_BAOXIANGRate_Update= 0x0121,
			SDO_BAOXIANGRate_Update_RESP= 0x8121,
			SDO_Planet_Query = 0x0122,
			SDO_Planet_Query_RESP = 0x8122,
			SDO_Channel_Query = 0x0123,
			SDO_Channel_Query_RESP = 0x8123,
			SDO_UserOnline_Query = 0x0124,
			SDO_UserOnline_Query_RESP = 0x8124,
			SDO_Family_CONSUME_QUERY = 0x0125,
			SDO_Family_CONSUME_QUERY_RESP = 0x8125,
			SDO_Family_Info_QUERY = 0x0126,//家族基本信息
			SDO_Family_Info_QUERY_RESP = 0x8126,//家族基本信息
			SDO_FamilyMember_Query = 0x0127,//家族成员信息
			SDO_FamilyMember_Query_RESP = 0x8127,//家族成员信息
			SDO_FamilyFormal_Query= 0x0128,//申请中成员信息
			SDO_FamilyFormal_Query_RESP = 0x8128,//申请中成员信息
			SDO_Personal_Emblem_Query = 0x0129,//家族勋章
			SDO_Personal_Emblem_Query_RESP = 0x8129,//家族勋章
			SDO_Family_Donate_Query = 0x0130,//家族金库捐赠明细
			SDO_Family_Donate_Query_RESP = 0x8130,//家族金库捐赠明细

			SDO_Family_Delete_Query = 0x0131,//解散家族
			SDO_Family_Delete_Query_RESP = 0x8131,//解散家族

			SDO_Family_Duty_Query = 0x0132,//家族成员职务修改
			SDO_Family_Duty_Query_RESP = 0x8132,//家族成员职务修改

			SDO_Family_Emblem_Insert_Query= 0x0133,//可添加家族徽章
			SDO_Family_Emblem_Insert_Query_RESP= 0x8133,//可添加家族徽章

			SDO_Family_Emblem_Delete_Query= 0x0134,//可删除家族徽章
			SDO_Family_Emblem_Delete_Query_RESP= 0x8134,//可删除家族徽章

			//maple add
			SDO_Pet_Update= 0x0135,//删除玩家宠物
			SDO_Pet_Update_RESP =0x8135,
			SDO_Social_Update=0x0136,//修改人物社交值
			SDO_Social_Update_Resp=0x8136,
			SDO_Lucky_Update=0x0137,//修改人物幸运值
			SDO_Lucky_Update_Resp=0x8137,


			Ban_Open_Query=0x0001,//解封账号
			Ban_Open_Query_Resp=0x8001,//解封账号回应
			
			Ban_Close_Query=0x0002,//封停账号
			Ban_Close_Query_Resp=0x8002,//封停账号回应
			
			Ban_AccountList_Query=0x0003,//账号列表查询
			Ban_AccountList_Query_Resp=0x8003,//账号列表查询回应
			
			Ban_Account_Query=0x0004,//账号查询
			Ban_Account_Query_Resp=0x8004,//账号查询响应
			
			Ban_UserInfo_Query=0x0005,//帐户信息查询
			Ban_UserInfo_Query_Resp=0x8005,//帐户信息查询响应

			MESSAGE = 0xEEEE
		};  
		__Enum _value; // 枚举值  
		ServiceKey(int value = 0) : _value((__Enum)value) {}  
		ServiceKey& operator=(int value) 
		{       
			this->_value = (__Enum)value;
			return *this;
		}   
		operator int() 
			const 
		{        return this->_value;    }
	};
	struct Message_Tag_ID
	{    
		enum __Enum 
		{      
			CONNECT = 0x800001,//连接请求
			CONNECT_RESP = 0x808001,//连接响应
			DISCONNECT = 0x800002,//断开请求
			DISCONNECT_RESP = 0x808002,//断开响应
			ACCOUNT_AUTHOR = 0x800003,//用户身份验证请求
			ACCOUNT_AUTHOR_RESP = 0x808003,//用户身份验证响应
			SERVERINFO_IP_QUERY = 0x800004,
			SERVERINFO_IP_QUERY_RESP = 0x808004,
			GMTOOLS_OperateLog_Query = 0x800005,//查看工具操作记录
			GMTOOLS_OperateLog_Query_RESP = 0x808005,//查看工具操作记录响应
			SERVERINFO_IP_ALL_QUERY = 0x800006,
			SERVERINFO_IP_ALL_QUERY_RESP = 0x808006,
			LINK_SERVERIP_CREATE = 0x800007,
			LINK_SERVERIP_CREATE_RESP = 0x808007,
			CLIENT_PATCH_COMPARE = 0x800008,
			CLIENT_PATCH_COMPARE_RESP = 0x808008,
			CLIENT_PATCH_UPDATE = 0x800009,
			CLIENT_PATCH_UPDATE_RESP = 0x808009,
			LINK_SERVERIP_DELETE = 0x800010,
			LINK_SERVERIP_DELETE_RESP = 0x808010,
			GMTOOLS_BUGLIST_QUERY = 0x800011,
			GMTOOLS_BUGLIST_QUERY_RESP = 0x808011,
			GMTOOLS_BUGLIST_INSERT = 0x800012,
			GMTOOLS_BUGLIST_INSERT_RESP = 0x808012,
			GMTOOLS_BUGLIST_UPDATE = 0x800013,
			GMTOOLS_BUGLIST_UPDATE_RESP = 0x808013,
			GMTOOLS_UPDATELIST_QUERY = 0x800014,
			GMTOOLS_UPDATELIST_QUERY_RESP = 0x808014,
			GMTOOLS_RESETSTATICS_QUERY = 0x800015,
			GMTOOLS_RESETSTATICS_QUERY_RESP = 0x808015,
			GMTOOLS_USERMAC_UPDATE = 0x800016,
			GMTOOLS_USERMAC_UPDATE_RESP = 0x808016,
			DOMAIN_AUTHOR = 0x800017,
			DOMAIN_AUTHOR_RESP = 0x808017,
			GM_AccountInfo_Vip = 0x800020,			//查询vip帐号
			GM_AccountInfo_Vip_RESP = 0x808020,		//查询vip帐号响应
			//GM_ClosePlayerGroup_WA = 0x800021,			//封停帐号
			GM_ClosePlayerGroup_RESP_WA = 0x808021,	//封停帐号响应


			USER_ACTIVE=0x800018,//心跳包
			USER_ACTIVE_RESP=0x808018,

			//Added by tengjie 2010-05-05
			HARMFUL_PROC_QUERY = 0x800019,//外挂进程工具包
			HARMFUL_PROC_QUERY_RESP=0x808019,

			/// <summary>
			/// 用户管理模块(0x81)
			/// </summary>
			USER_CREATE = 0x810001,//创建GM帐号请求
			USER_CREATE_RESP = 0x818001,//创建GM帐号响应
			USER_UPDATE = 0x810002,//更新GM帐号信息请求
			USER_UPDATE_RESP = 0x818002,//更新GM帐号信息响应
			USER_DELETE = 0x810003,//删除GM帐号信息请求
			USER_DELETE_RESP = 0x818003,//删除GM帐号信息响应
			USER_QUERY = 0x810004,//查询GM帐号信息请求
			USER_QUERY_RESP = 0x818004,//查询GM帐号信息响应
			USER_PASSWD_MODIF = 0x810005,//密码修改请求
			USER_PASSWD_MODIF_RESP = 0x818005, //密码修改信息响应
			USER_QUERY_ALL = 0x810006,//查询所有GM帐号信息
			USER_QUERY_ALL_RESP = 0x818006,//查询所有GM帐号信息响应
			DEPART_QUERY = 0x810007, //查询部门列表
			DEPART_QUERY_RESP = 0x818007,//查询部门列表响应
			UPDATE_ACTIVEUSER = 0x810008,//更新在线用户状态
			UPDATE_ACTIVEUSER_RESP = 0x818008,//更新在线用户状态响应
			DEPARTMENT_CREATE = 0x810009,//部门创建
			DEPARTMENT_CREATE_RESP = 0x818009,//部门创建响应
			DEPARTMENT_UPDATE= 0x810010,//部门修改
			DEPARTMENT_UPDATE_RESP = 0x818010,//部门修改响应
			DEPARTMENT_DELETE= 0x810011,//部门删除
			DEPARTMENT_DELETE_RESP = 0x818011,//部门删除响应
			DEPARTMENT_ADMIN = 0x810012,//部门管理
			DEPARTMENT_ADMIN_RESP = 0x818012,//部门管理响应
			DEPARTMENT_RELATE_QUERY = 0x810013,//部门关联查询
			DEPARTMENT_RELATE_QUERY_RESP = 0x818013,//部门关联查询
			DEPART_RELATE_GAME_QUERY = 0x810014,
			DEPART_RELATE_GAME_QUERY_RESP = 0x818014,
			USER_SYSADMIN_QUERY = 0x810015,
			USER_SYSADMIN_QUERY_RESP = 0x818015,
			USERBAN_QUERY = 0x810016,
			USERBAN_QUERY_RESP = 0x818016,


			/// <summary>
			/// 模块管理(0x82)
			/// </summary>
			MODULE_CREATE = 0x820001,//创建模块信息请求
			MDDULE_CREATE_RESP = 0x828001,//创建模块信息响应
			MODULE_UPDATE =0x820002,//更新模块信息请求
			MODULE_UPDATE_RESP = 0x828002,//更新模块信息响应
			MODULE_DELETE = 0x820003,//删除模块请求
			MODULE_DELETE_RESP = 0x828003,//删除模块响应
			MODULE_QUERY = 0x820004,//查询模块信息的请求
			MODULE_QUERY_RESP = 0x828004,//查询模块信息的响应

			/// <summary>
			/// 用户与模块管理(0x83)
			/// </summary>
			USER_MODULE_CREATE = 0x830001,//创建用户与模块请求
			USER_MODULE_CREATE_RESP = 0x838001,//创建用户与模块响应
			USER_MODULE_UPDATE = 0x830002,//更新用户与模块的请求
			USER_MODULE_UPDATE_RESP = 0x838002,//更新用户与模块的响应
			USER_MODULE_DELETE = 0x830003,//删除用户与模块请求
			USER_MODULE_DELETE_RESP = 0x838003,//删除用户与模块响应
			USER_MODULE_QUERY = 0x830004,//查询用户所对应模块请求
			USER_MODULE_QUERY_RESP = 0x838004,//查询用户所对应模块响应

			/// <summary>
			/// 游戏管理(0x84)
			/// </summary>
			GAME_CREATE = 0x840001,//创建GM帐号请求
			GAME_CREATE_RESP = 0x848001,//创建GM帐号响应
			GAME_UPDATE = 0x840002,//更新GM帐号信息请求
			GAME_UPDATE_RESP = 0x848002,//更新GM帐号信息响应
			GAME_DELETE = 0x840003,//删除GM帐号信息请求
			GAME_DELETE_RESP = 0x848003,//删除GM帐号信息响应
			GAME_QUERY = 0x840004,//查询GM帐号信息请求
			GAME_QUERY_RESP = 0x848004,//查询GM帐号信息响应
			GAME_MODULE_QUERY = 0x840005,//查询游戏的模块列表
			GAME_MODULE_QUERY_RESP = 0x848005,//查询游戏的模块列表响应
		
			/// <summary>
			/// 仙剑GM工具(0x62)
			/// </summary>
			PAL_PLAYERONLINECOUNT=0x620001,//玩家在线数量
			PAL_PLAYERONLINECOUNT_RESP=0x628001,//玩家在线数量响应

			PAL_CHARACTERBAG=0x620002,//玩家背包查询
			PAL_CHARACTERBAG_RESP=0x628002,//玩家背包查询响应

			PAL_CHARACTERFRIENDLIST=0x620003,//取得角色好友列表
			PAL_CHARACTERFRIENDLIST_RESP=0x628003,//取得角色好友列表响应

			PAL_CHARACTERINFO=0x620004,//取得角色基本资讯
			PAL_CHARACTERINFO_RESP=0x628004,//取得角色基本资讯响应
			
			PAL_CHARACTERLIST=0x620005,//取得角色列表
			PAL_CHARACTERLIST_RESP=0x628005,//取得角色列表响应

			PAL_CHARACTERQUEST=0x620006,//取得角色任务资讯
			PAL_CHARACTERQUEST_RESP=0x628006,//取得角色任务资讯响应

			PAL_GUILDBASEINFO=0x620007,//取得公会基本资讯
			PAL_GUILDBASEINFO_RESP=0x628007,//取得公会基本资讯响应
			
			PAL_GUILDMEMBERLIST=0x620008,//取得公会成员列表
			PAL_GUILDMEMBERLIST_RESP=0x628008,//取得公会成员列表响应

			PAL_SENDGIFTMAIL=0x620009,//寄送奖品邮件
			PAL_SENDGIFTMAIL_RESP=0x628009,//寄送奖品邮件响应

			PAL_ACCOUNTLOCAL_QUERY = 0x620010,//本地封停的玩家帐号查询
			PAL_ACCOUNTLOCAL_QUERY_RESP = 0x628010,//本地封停的玩家帐号查询响应
			PAL_ACCOUNT_CLOSE = 0x620011,//封停的玩家帐号
			PAL_ACCOUNT_CLOSE_RESP = 0x628011,//封停的玩家帐号响应
			PAL_ACCOUNT_OPEN = 0x620012,//解封的玩家帐号
			PAL_ACCOUNT_OPEN_RESP = 0x628012,//解封的玩家帐号响应
			PAL_ACCOUNT_BANISHMENT_QUERY = 0x620013,//玩家封停帐号查询
			PAL_ACCOUNT_BANISHMENT_QUERY_RESP = 0x628013,//玩家封停帐号查询响应


			PAL_MODIFYCHARACTERMONEY=0x620014,//修改角色金钱
			PAL_MODIFYCHARACTERMONEY_RESP=0x628014,//修改角色金钱响应

			PAL_MODIFYCHARACTERDROPPW=0x620015,//修改角色删除密码
			PAL_MODIFYCHARACTERDROPPW_RESP=0x628015,//修改角色删除密码响应

			PAL_MODIFYACCOUNTBANKPW=0x620016,//修改角色银行密码
			PAL_MODIFYACCOUNTBANKPW_RESP=0x628016,//修改角色银行密码响应

			PAL_DELCHARACTERITEM=0x620017,// 删除角色物品
			PAL_DELCHARACTERITEM_RESP=0x628017,// 删除角色物品响应

			PAL_RECALLCHARACTERDROPPW=0x620018,//还原角色删除密码
			PAL_RECALLCHARACTERDROPPW_RESP=0x628018,//还原角色删除密码响应

			PAL_RECALLACCOUNTBANKPW=0x620019,//还原帐号银行密码
			PAL_RECALLACCOUNTBANKPW_RESP=0x628019,//还原帐号银行密码响应

			PAL_MODIFYACCOUNTPW=0x620020,//修改帐号密码
			PAL_MODIFYACCOUNTPW_RESP=0x628020,//修改帐号密码响应

			PAL_RECALLACCOUNTPW=0x620021,//还原帐号密码
			PAL_RECALLACCOUNTPW_RESP=0x628021,//还原帐号密码响应

	
			PAL_BOARDTASK_INSERT = 0x620033,//公告添加
			PAL_BOARDTASK_INSERT_RESP = 0x628033,//公告添加响应
			PAL_BOARDTASK_QUERY = 0x620034,//公告查询
			PAL_BOARDTASK_QUERY_RESP = 0x628034,//公告查询响应
			PAL_BOARDTASK_UPDATE = 0x620035,//公告更新
			PAL_BOARDTASK_UPDATE_RESP = 0x628035,//公告更新响应

			
			PAL_ITEMTYPE_QUERY=0x620037,//道具类别查询
			PAL_ITEMTYPE_QUERY_RESP=0x628037,//道具类别查询响应
			PAL_ITEMNAME_QUERY=0x620038,//道具名称查询
			PAL_ITEMNAME_QUERY_RESP=0x628038,//道具名称查询响应
			PAL_ITEMNAMEBLUR_QUERY=0x620039,//道具名称模糊查询
			PAL_ITEMNAMEBLUR_QUERY_RESP=0x628039,//道具名称模糊查询响应

			PAL_MAILBOX_QUERY=0x620040,//查询玩家邮箱地址
			PAL_MAILBOX_QUERY_RESP=0x628040,//查询玩家邮箱地址响应 

			PAL_SendBankPwd_Query=0x620041,//发送仓库密码
			PAL_SendBankPwd_Query_RESP=0x628041,//发送仓库密码响应

			PAL_SendDeletePwd_Query=0x620042,//发送角色删除密码
			PAL_SendDeletePwd_Query_RESP=0x628042,//发送角色删除密码响应

			PAL_MODIFYSECONDPW=0x620043,//修改游戏二级密码
			PAL_MODIFYSECONDPW_RESP=0x628043,//修改游戏二级密码响应
			
			PAL_RECALLSECONDPW=0x620044,//还原游戏二级密码
			PAL_RECALLSECONDPW_RESP=0x628044,//还原游戏二级密码响应

			PAL_USESAFELOCK=0x620045,//使用安全锁
			PAL_USESAFELOCK_RESP=0x628045,//使用安全锁响应

			PAL_FREESAFELOCK=0x620046,//释放安全锁
			PAL_FREESAFELOCK_RESP=0x628046,//释放安全锁响应

			PAL_CHARACTERADVANCEINFO=0x620047,//取得角色进阶资讯
			PAL_CHARACTERADVANCEINFO_RESP=0x628047,//取得角色进阶资讯

			PAL_CHARACTERSKILL=0x620048,//取得角色技能资讯
			PAL_CHARACTERSKILL_RESP=0x628048,//取得角色技能资讯

			PAL_CHARACTERMOB=0x620049,//取得角色伏魔资讯
			PAL_CHARACTERMOB_RESP=0x628049,//取得角色伏魔资讯


			PAL_CHARACTERPUZZLEMAP=0x620050,//取得角色图鉴资讯
			PAL_CHARACTERPUZZLEMAP_RESP=0x628050,//取得角色图鉴资讯

			PAL_CHARACTERBLACKLIST=0x620051,//取得角色黑名单列表
			PAL_CHARACTERBLACKLIST_RESP=0x628051,//取得角色黑名单列表

			PAL_DELCHARACTERMOB=0x620052,//删除角色伏魔
			PAL_DELCHARACTERMOB_RESP=0x628052,//删除角色伏魔

			PAL_MODIFYPUZZLEMAP=0x620053,//修改角色图鉴
			PAL_MODIFYPUZZLEMAP_RESP=0x628053,//修改角色图鉴

			PAL_ACCOUNTINFO=0x620054,//取得帐号资讯
			PAL_ACCOUNTINFO_RESP=0x628054,//取得帐号资讯

			PAL_MOVECHARACTERTOSAFE=0x620055,//移动角色至安全点
			PAL_MOVECHARACTERTOSAFE_RESP=0x628055,//移动角色至安全点

			PAL_RELOADFUNCTIONSWITCH=0x620056,//重新载入功能开关列表
			PAL_RELOADFUNCTIONSWITCH_RESP=0x628056,//重新载入功能开关列表


			PAL_SearchMap_Query=0x620057,//查询地图
			PAL_SearchMap_Query_Resp=0x628057,//查询地图

			PAL_SCHEDULEBULLETINLIST=0x621001,//取得排程公告列表
			PAL_SCHEDULEBULLETINLIST_RESP=0x627001,//取得排程公告列表响应

			PAL_ADDSCHEDULEBULLETIN=0x621002,//新增排程公告
			PAL_ADDSCHEDULEBULLETIN_RESP=0x627002,//新增排程公告响应	

			PAL_DELSCHEDULEBULLETIN=0x621003,//删除排程公告
			PAL_DELSCHEDULEBULLETIN_RESP=0x627003,//删除排程公告响应	

			PAL_RELOADSCHEDULEBULLETIN=0x621004,//重新载入排程公告
			PAL_RELOADSCHEDULEBULLETIN_RESP=0x627004,//重新载入排程公告响应	

			PAL_SENDNONSCHEDULEBULLETIN=0x621005,//发送非固定公告
			PAL_SENDNONSCHEDULEBULLETIN_RESP=0x627005,//发送非固定公告响应	
	


			PAL_BANISHPLAYER =0x624001,//踢人
			PAL_BANISHPLAYER_RESP=0x626001,//踢人响应
			
			PAL_DISMISSGUILD=0x624002,//解散公会
			PAL_DISMISSGUILD_RESP=0x626002,//解散公会响应

			PAL_MODIFYGUILDNAME=0x624003,//修改公会名称
			PAL_MODIFYGUILDNAME_RESP=0x626003,//修改公会名称响应

			PAL_MODIFYGUILDMEMBERPL=0x624004,//修改公会成员职阶
			PAL_MODIFYGUILDMEMBERPL_RESP=0x626004,//修改公会成员职阶响应

			PAL_MODIFYCHARACTERQUEST=0x624005,//修改角色任务
			PAL_MODIFYCHARACTERQUEST_RESP=0x626005,//修改角色任务响应

			PAL_MODIFYGUILDLV=0x624006,//修改公会等级
			PAL_MODIFYGUILDLV_RESP=0x626006,//修改公会等级响应

			PAL_MODIFYGUILDNOTICE=0x624007,//修改公会公告
			PAL_MODIFYGUILDNOTICE_RESP=0x626007,//修改公会公告响应
			
			PAL_Get_EmailAddress=0x620066,
			PAL_Get_EmailAddress_Resp=0x628066,



			PALLOG_ACCOUNT_LOGIN=0x630001,//登陆记录日志
			PALLOG_ACCOUNT_LOGIN_RESP=0x638001,//登陆记录日志响应

			PAL_LOGINFO_QUERY=0x630002,//玩家日志信息
			PAL_LOGINFO_QUERY_RESP=0x638002,//玩家日志信息响应

			PAL_LOG_BIGTYPE_QUERY=0x630003,//日志大类
			PAL_LOG_BIGTYPE_QUERY_RESP=0x638003,//日志大类响应

			PAL_LOG_SMALLTYPE_QUERY=0x630004,//日志小类
			PAL_LOG_SMALLTYPE_QUERY_RESP=0x638004,//日志小类响应

			PALLOG_ITEMMAIL=0x630005,//商城购买日志
			PALLOG_ITEMMAIL_RESP=0x638005,//商城购买日志响应

			PAL_CHECKMAILBOX_QUERY=0x630036,//邮箱信息查询
			PAL_CHECKMAILBOX_QUERY_RESP=0x638036,//邮箱信息查询响应 

			PAL_CHECKMAILBOX_DETAIL=0x630043,//邮箱附件查询
			PAL_CHECKMAILBOX_DETAIL_RESP=0x6328043,//邮箱附件查询响应

			PAL_AUCTION_QUERY=0x630037,//市场信息查询
			PAL_AUCTION_QUERY_RESP=0x638037,//市场信息查询响应 

			PAL_CDKEY_QUERY=0x630040,//CDKEY道具领取查询
			PAL_CDKEY_QUERY_RESP=0x638040,//CDKEY道具领取查询响应 

			PAL_ITEMMAIL_WASTE=0x630041,//消费记录
			PAL_ITEMMAIL_WASTE_RESP=0x638041,//消费记录响应

			PAL_HISTORY_LOGINFO_QUERY=0x630042,//日志历史
			PAL_HISTORY_LOGINFO_QUERY_RESP=0x638042,//日志历史响应

			PAL_ROLEDEL_QUERY=0x630050,//删除角色日志
			PAL_ROLEDEL_QUERY_RESP=0x638050,//删除角色日志
			
			PAL_EQUIPMENT_QUERY=0x630061,//装备道具查询
			PAL_EQUIPMENT_QUERY_RESP=0x638061,//装备道具查询响应

			PAL_SENDCUSTOMITEMMAIL=0x620062,//寄送客制物品邮件
			PAL_SENDCUSTOMITEMMAIL_RESP=0x628062,//寄送客制物品邮件响应

			PAL_MODIFYACCOUNTPWALL=0x620063,//修改所有大区帐号密码
			PAL_MODIFYACCOUNTPWALL_RESP=0x628063,//修改所有大区帐号密码响应
			
			PAL_RECALLACCOUNTPWALL=0x620064,//恢复所以大区帐号密码
			PAL_RECALLACCOUNTPWALL_RESP=0x628064,//恢复所以大区帐号密码响应

			//////////////////
			//GTown(0x51)
			/////////////////
			//添加公告
			GT_BROADCAST_ADD = 0x520001,
			GT_BROADCAST_ADD_ACK = 0x528001,

			//查询所有公告
			GT_BROADCAST_SELECT = 0x520002,
			GT_BROADCAST_SELECT_ACK = 0x528002,

			//删除公告
			GT_BROADCAST_REMOVE = 0x520003,
			GT_BROADCAST_REMOVE_ACK = 0x528003,

			//强制玩家下线
			GT_KICK_USER = 0x520004,
			GT_KICK_USER_ACK = 0x528004,

			//角色移至小黑屋
			GT_MOVE_USER = 0x520005,
			GT_MOVE_USER_ACK = 0x528005,

			//家族管理
			GT_FAMILY_DISBAND = 0x520006,
			GT_FAMILY_DISBAND_ACK = 0x528006,

			//账号封停、解封
			GT_USER_STOP = 0x520007,
			GT_USER_STOP_ACK = 0x528007,

			//添加道具、宠物
			GT_ADD_ITEM = 0x520008,
			GT_ADD_ITEM_ACK = 0x528008,

			//查询道具、宠物
			GT_ITEM_SELECT = 0x520009,
			GT_ITEM_SELECT_ACK = 0x528009,

			//重设玩家到出生状态
			GT_USER_RESET = 0x520010,
			GT_USER_RESET_ACK = 0x528010,

			//连接远程服务器
			CONNECT_SERVERMANAGER_LOGIN = 0x520011,
			CONNECT_SERVERMANAGER_LOGIN_ACK = 0x528011,

			//查询用户资料信息
			GT_UserInfo_Query = 0x510012,
			GT_UserInfo_Query_Resp = 0x518012,

			//查询用户详细资料信息
			GT_User_DetailInfo_Query = 0x510013,
			GT_User_DetailInfo_Query_Resp = 0x518013,

			//查询角色家族信息
			GT_User_FamilyInfo_Query = 0x510014,
			GT_User_FamilyInfo_Query_Resp = 0x518014,

			//查询角色情侣信息
			GT_User_SentiInfo_Query = 0x510015,
			GT_User_SentiInfo_Query_Resp = 0x518015,

			//查询角色结婚信息
			GT_User_MarrigeInfo_Query = 0x510016,
			GT_User_MarrigeInfo_Query_Resp = 0x518016,

			//查询所有被封停账号列表
			GT_AllBanUser_Query = 0x510017,
			GT_AllBanUser_Query_Resp = 0x518017,

			//查询家族信息
			GT_FamilyInfo_Query = 0x510018,
			GT_FamilyInfo_Query_Resp = 0x518018,

			//查询家族成员信息
			GT_MemberInfo_Query = 0x510019,
			GT_MemberInfo_Query_Resp = 0x518019,

			//查询角色背包信息
			GT_UserPack_Query = 0x510020,
			GT_UserPack_Query_Resp = 0x518020,

			//查询角色装备信息
			GT_UserEquip_Query = 0x510021,
			GT_UserEquip_Query_Resp = 0x518021,

			//查询宠物背包信息
			GT_PetPack_Query = 0x510022,
			GT_PetPack_Query_Resp = 0x518022,



			//查询玩家交易日志
			GT_TradeLog_Query = 0x510023,
			GT_TradeLog_Query_Resp = 0x518023,
			
			//查询玩家邮件日志
			GT_MailLog_Query = 0x510024,
			GT_MailLog_Query_Resp = 0x518024,
			
			//查询玩家任务完成日志
			GT_TaskLog_Query = 0x510025,
			GT_TaskLog_Query_Resp = 0x518025,
			
			//查询玩家使用道具日志(收费道具)
			GT_UseItemLog_Query = 0x510026,
			GT_UseItemLog_Query_Resp = 0x518026,
			
			//查询玩家挂机奖励日志
			GT_GuajiLog_Query = 0x510027,
			GT_GuajiLog_Query_Resp = 0x518027,

			//查询玩家大小喇叭日志
			GT_BugleLog_Query = 0x510028,
			GT_BugleLog_Query_Resp = 0x518028,
			
			//查询账号登陆登出日志
			GT_LoginLog_Query = 0x510029,
			GT_LoginLog_Query_Resp = 0x518029,



			//查询人物房屋资料
			GT_RoomInfo_Query = 0x510030,
			GT_RoomInfo_Query_Resp = 0x518030,
			
			//查询人物宠物资料
			GT_PetInfo_Query = 0x510031,
			GT_PetInfo_Query_Resp = 0x518031,
			
			//查询GT-IM好友资料信息
			GT_IMFriend_Query = 0x510032,
			GT_IMFriend_Query_Resp = 0x518032,

			//查询GT-IM黑名单信息
			GT_IMBlackList_Query = 0x510033,
			GT_IMBlackList_Query_Resp = 0x518033,

			//查询GT-IM暗恋名单信息
			GT_IMSecretLove_Query = 0x510034,
			GT_IMSecretLove_Query_Resp = 0x518034,
			
			//查询宝箱开启信息
			GT_TreasureBox_Query = 0x510035,
			GT_TreasureBox_Query_Resp = 0x518035,

			//邮件信息查询
			GT_MailInfo_Query = 0x510036,
			GT_MailInfo_Query_Resp = 0x518036,

			//充值记录查询
			GT_ChargeInfo_Query = 0x510037,
			GT_ChargeInfo_Query_Resp = 0x518037,

			//道具购买消费记录
			GT_ItemConsume_Query = 0x510038,
			GT_ItemConsume_Query_Resp = 0x518038,


			//瞬间移动
			GT_Transmit_Query=0x520039,
			GT_Transmit_Query_Resp=0x528039,
			
			//禁止、解禁玩家上传图片
			GT_Picture_Stop=0x520040,
			GT_Picture_Stop_Resp=0x528040,
			
			
			//清理违禁图片
			GT_Picture_Clear=0x520043,
			GT_Picture_Clear_Resp=0x528043,


			//血拼战绩 T_gtown_Mcoin_Rank
			GT_Mcoin_Rank=0x510044,
			GT_Mcoin_Rank_Resp=0x518044,
			
			//空间排行 T_gtown_sns_db
			GT_Sns_Query=0x510045,
			GT_Sns_Query_Resp=0x518045,
			
			//属性排行
			GT_Attr_Queue=0x510046,
			GT_Attr_Queue_Resp=0x518046,
			
			//名片排行 T_gtown_card_record
			GT_Card_Record=0x510047,
			GT_Card_Record_Resp=0x518047,
			
			//朋友买卖中角色信息
			GT_FriendTrade_Info=0x510048,
			GT_FriendTrade_Info_Resp=0x518048,
			
			//奴隶信息
			GT_HelotInfo_Query=0x510049,
			GT_HelotInfo_Query_Resp=0x518049,
			
			//排行
			GT_FriendTrade_Queue=0x510050,
			GT_FriendTrade_Queue_Resp=0x518050,
			
			//买卖Log
			GT_FriendTrade_Log=0x510051,
			GT_FriendTrade_Log_Resp=0x518051,

			//清空个性签名
			GT_ClearName_Query=0x520052,
			GT_ClearName_Query_Resp=0x528052,

			//查询所有被禁止账号列表
			GT_AllStopUser_Query = 0x510053,
			GT_AllStopUser_Query_Resp = 0x518053,

			//查询道具(从原来接口变成数据库)
			GT_Select_Item=0x510054,
			GT_Select_Item_Resp=0x518054,
			
			//查询单个账号是否被封停
			GT_BanUser_Query=0x510055,
			GT_BanUser_Query_Resp=0x518055,


			//批量添加道具
			GT_Import_Item=0x520056,
			GT_Import_Item_Resp=0x528056,


			//退款
			GT_Back_Money=0x510057,
			GT_Back_Money_Resp=0x518057,


			/////////<summary>
			///勇士无双消息集(0x53)
			//////</summary>
			//查询用户信息
			WA_Account_Query=0x530001,
			WA_Account_Query_Resp=0x538001,

			//强制玩家下线
			WA_KickPlayer=0x530002,
			WA_KickPlayer_Resp=0x538002,

			//封停用户
			WA_ClosePlayer=0x530003,
			WA_ClosePlayer_Resp=0x538003,

			//解封用户
			WA_OpenPlayer=0x530004,
			WA_OpenPlayer_Resp=0x538004,

			//封停列表
			WA_CloseList=0x530005,
			WA_CloseList_Resp=0x538005,

			//单个封停
			WA_CloseSingle=0x530006,
			WA_CloseSingle_Resp=0x538006,

			//添加公告
			WA_BoardTasker_Add=0x530007,
			WA_BoardTasker_Add_Resp=0x538007,

			//删除公告
			WA_BoardTasker_Del=0x530008,
			WA_BoardTasker_Del_Resp=0x538008,

			//查询公告
			WA_BoardTasker_Query=0x530009,
			WA_BoardTasker_Query_Resp=0x538009,

			//好友信息查询
			WA_FriendInfo_Query=0x530010,
			WA_FriendInfo_Query_Resp=0x538010,
			
			//角色物品信息查询
			WA_BodyItemInfo_Query=0x530011,
			WA_BodyItemInfo_Query_Resp=0x538011,
			
			//角色背包信息查询
			WA_BagItemInfo_Query=0x530012,
			WA_BagItemInfo_Query_Resp=0x538012,
			
			//角色银行信息查询
			WA_BankItemInfo_Query=0x530013,
			WA_BankItemInfo_Query_Resp=0x538013,
			
			//邮箱信息查询
			WA_MailInfo_Query=0x530014,
			WA_MailInfo_Query_Resp=0x538014,
			
			//徽章信息查询
			WA_EmblemInfo_Query=0x530015,
			WA_EmblemInfo_Query_Resp=0x538015,
			
			//宠物信息查询
			WA_PetInfo_Query=0x530016,
			WA_PetInfo_Query_Resp=0x538016,
			
			//帐号登录信息查询
			WA_AccountLoginInfo_Query=0x530017,
			WA_AccountLoginInfo_Query_Resp=0x538017,

			//道具增加
			WA_Item_Add=0x530018,
			WA_Item_Add_Resp=0x538018,
			
			//道具删除
			WA_Item_Del=0x530019,
			WA_Item_Del_Resp=0x538019,
			
			//金钱修改
			WA_Money_Update=0x530020,
			WA_Money_Update_Resp=0x538020,

			//公告修改
			WA_BoardTasker_Update=0x530021,
			WA_BoardTasker_Update_Resp=0x538021,

			//add by ltx
			//修改角色技能
			WA_ModifyCharacterSkill=0x530022,
			WA_ModifyCharacterSkill_Resp=0x538022,

			//恢复角色
			WA_RecoverCharacter=0x530023,
			WA_RecoverCharacter_Resp=0x538023,

			//修改密码
			WA_ModifyAccountPassw=0x530024,
			WA_ModifyAccountPassw_Resp=0x538024,

			//给角色邮寄物品
			WA_MailItem=0x530025,
			WA_MailItem_Resp=0x538025,

			//物品:怪物掉落
			WA_Query_MonsterDropItem=0x530026,
			WA_Query_MonsterDropItem_Resp=0x538026,

			//物品:从NPC处购买
			WA_Query_BuyItemNPC=0x530027,
			WA_Query_BuyItemNPC_Resp=0x538027,
			
			//物品:卖给NPC
			WA_Query_SellItemTONPC=0x530028,
			WA_Query_SellItemTONPC_Resp=0x538028,

			//物品:玩家自行删除
			WA_Query_DropItem=0x530029,
			WA_Query_DropItem_Resp=0x538029,

			//物品:交易获得
			WA_Query_GetItemTrade=0x530030,
			WA_Query_GetItemTrade_Resp=0x538030,

			//物品:交易失去
			WA_Query_LoseItemTrade=0x530031,
			WA_Query_LoseItemTrade_Resp=0x538031,

			//物品:装备绑定记录
			WA_Query_BindItem=0x530032,
			WA_Query_BindItem_Resp=0x538032,

			//物品:装备解绑定记录
			WA_Query_UnBindItem=0x530033,
			WA_Query_UnBindItem_Resp=0x538033,

			//物品:角色“摆摊”日志查询
			WA_Query_BoothItem=0x530034,
			WA_Query_BoothItem_Resp=0x538034,

			//物品:道具删除记录(指使用时间到了)
			WA_Query_DeleteItem=0x530035,
			WA_Query_DeleteItem_Resp=0x538035,

			//物品:道具使用记录
			WA_Query_UseItem=0x530036,
			WA_Query_UseItem_Resp=0x538036,

			//物品:装备制作记录
			WA_Query_CraftItem=0x530037,
			WA_Query_CraftItem_Resp=0x538037,

			//物品:装备打孔
			WA_Query_PunchItem=0x530038,
			WA_Query_PunchItem_Resp=0x538038,

			//物品:装备改孔
			WA_Query_ChangeSlotItem=0x530039,
			WA_Query_ChangeSlotItem_Resp=0x538039,

			//物品:装备升星
			WA_Query_UpgradeStar=0x530040,
			WA_Query_UpgradeStar_Resp=0x538040,

			//物品:宝石
			WA_Query_Gem=0x530041,
			WA_Query_Gem_Resp=0x538041,

			//物品:制药记录
			WA_Query_Pharma=0x530042,
			WA_Query_Pharma_Resp=0x538042,

			//物品:宝石合成
			WA_Query_CraftGem=0x530043,
			WA_Query_CraftGem_Resp=0x538043,

			//物品:材料合成
			WA_Query_CraftMatirail=0x530044,
			WA_Query_CraftMatirail_Resp=0x538044,

			//物品:图纸合成
			WA_Query_CraftRecipe=0x530045,
			WA_Query_CraftRecipe_Resp=0x538045,

			//迷宫积分兑换奖励
			WA_Query_MazeIntegral=0x530046,
			WA_Query_MazeIntegral_Resp=0x538046,

			//游戏币:怪物掉落
			WA_Query_MonsterDropMoney=0x530047,
			WA_Query_MonsterDropMoney_Resp=0x538047,

			//游戏币:任务获得
			WA_Query_GetMoneyQuest=0x530048,
			WA_Query_GetMoneyQuest_Resp=0x538048,

			//游戏币:修装
			WA_Query_LoseMoneyRepair=0x530049,
			WA_Query_LoseMoneyRepair_Resp=0x538049,

			//游戏币:交易获得
			WA_Query_GetMoneyTrade=0x530050,
			WA_Query_GetMoneyTrade_Resp=0x538050,

			//游戏币:交易失去
			WA_Query_LoseMoneyTrade=0x530051,
			WA_Query_LoseMoneyTrade_Resp=0x538051,

			//任务:完成获得记录
			WA_Query_QuestReward=0x530052,
			WA_Query_QuestReward_Resp=0x538052,

			//任务:任务物品删除记录
			WA_Query_QuestGive=0x530053,
			WA_Query_QuestGive_Resp=0x538053,

			//邮件:发送记录
			WA_Query_MailSend=0x530054,
			WA_Query_MailSend_Resp=0x538054,

			//邮件:接收记录
			WA_Query_MailRecv=0x530055,
			WA_Query_MailRecv_Resp=0x538055,

			//邮件:删除记录
			WA_Query_MailDelete=0x530056,
			WA_Query_MailDelete_Resp=0x538056,

			//商城:购买记录
			WA_Query_ItemShop=0x530057,
			WA_Query_ItemShop_Resp=0x538057,

			//查询用户的副本日志
			WA_Query_Instance=0x530058,
			WA_Query_Instance_Resp=0x538058,

			//玩家登陆/登出日志
			WA_Query_LoginLogout=0x530059,
			WA_Query_LoginLogout_Resp=0x538059,
			
			//道具列表查询
			WA_ItemList_Query=0x530060,
			WA_ItemList_Query_Resp=0x538060,
			
			//技能列表查询
			WA_SkillList_Query=0x530061,
			WA_SkillList_Query_Resp=0x538061,

			//修改角色名
			WA_PlayerRole_Modfiy=0x530062,
			WA_PlayerRole_Modfiy_Resp=0x538062,

			//修改角色属性值
			WA_RoleInfo_Modfiy=0x530063,
			WA_RoleInfo_Modfiy_Resp=0x538063,

			//查询玩家身上技能信息
			WA_RoleSkill_Query=0x530064,
			WA_RoleSkill_Query_Resp=0x538064,
			
			//查询玩家道具类型查询
			WA_ItemType_Query=0x530065,
			WA_ItemType_Query_Resp=0x538065,

			//玩家密码恢复
			WA_UserPwd_Recover=0x530066,
			WA_UserPwd_Recover_Resp=0x538066,

			//Added by tengjie 2010-06-04:For 角色日志
			//角色升级记录查询
			WA_Character_Upgrade=0x530067,
			WA_Character_Upgrade_Resp=0x538067,

			//角色创建记录查询
			WA_Character_Create=0x530068,
			WA_Character_Create_Resp=0x538068,

			//角色删除记录查询
			WA_Character_Delete=0x530069,
			WA_Character_Delete_Resp=0x538069,

			//加入帮会
			WA_Join_Guild=0x530070,
			WA_Join_Guild_Resp=0x538070,

			//离开帮会
			WA_Quit_Guild=0x530071,
			WA_Quit_Guild_Resp=0x538071,

			//创建帮会
			WA_Create_Guild=0x530072,
			WA_Create_Guild_Resp=0x538072,

			//解散帮会
			WA_Dissolve_Guild=0x530073,
			WA_Dissolve_Guild_Resp=0x538073,

			//转职信息查询
			WA_Transfer_Info=0x530074,
			WA_Transfer_Info_Resp=0x538074,
			
			//技能日志记录
			WA_Skills_Log=0x530075,
			WA_Skills_Log_Resp=0x538075,
			
			//生活技能日志记录
			WA_LifeSkills_Log=0x530076,
			WA_LifeSkills_Log_Resp=0x538076,

			//佣兵团日志
			//创建记录
			WA_Create_MercenaryGroup=0x530077,
			WA_Create_MercenaryGroup_Resp=0x538077,

			//解散记录
			WA_Dissolve_MercenaryGroup=0x530078,
			WA_Dissolve_MercenaryGroup_Resp=0x538078,

			//宠物徽章日志
// 			//徽章获得时间
			WA_GainBadge_Time=0x530079,
			WA_GainBadge_Time_Resp=0x538079,

			//徽章丢弃时间
			WA_DiscardBadge_Time=0x530080,
			WA_DiscardBadge_Time_Resp=0x538080,

			//徽章兑换记录
			WA_ExchangeBadge_Record=0x530081,
			WA_ExchangeBadge_Record_Resp=0x538081,

			//徽章鉴定
			WA_Identify_Badge=0x530082,
			WA_Identify_Badge_Resp=0x538082,
			
			//宠物精魂捕捉记录
			WA_CatchPetSpirit_Record=0x530083,
			WA_CatchPetSpirit_Record_Resp=0x538083,
			
			//封印精魂记录
			WA_SealSpirit_Record=0x530084,
			WA_SealSpirit_Record_Resp=0x538084,

			//徽章培养记录
			WA_CultureBadge_Record=0x530085,
			WA_CultureBadge_Record_Resp=0x538085,
			
			//精魂交易记录
			//精魂交易获得
			WA_GainSpirit_Record=0x530086,
			WA_GainSpirit_Record_Resp=0x538086,
			
			//精魂交易失去
			WA_LoseSpirit_Record=0x530087,
			WA_LoseSpirit_Record_Resp=0x538087,

			//精魂出售记录
			WA_SellSpirit_Record=0x530088,
			WA_SellSpirit_Record_Resp=0x538088,

			//用户信息查询
			//技能信息查询
			WA_SkillsInfo_Query=0x530089,
			WA_SkillsInfo_Query_Resp=0x538089,

			//任务信息查询:已完成任务
			WA_MissionsCompleted_Query=0x530090,
			WA_MissionsCompleted_Query_Resp=0x538090,

			//任务信息查询:已接任务
			WA_MissionsReceived_Query=0x530091,
			WA_MissionsReceived_Query_Resp=0x538091,

			//佣兵团信息查询
			//佣兵团信息查询
			WA_MercenaryGroup_Info_Query=0x530092,
			WA_MercenaryGroup_Info_Query_Resp=0x538092,

			//佣兵团成员列表查询
			WA_MemberList_Query=0x530093,
			WA_MemberList_Query_Resp=0x538093,

			//佣兵团仓库信息
			WA_WarehouseInfo_Query=0x530094,
			WA_WarehouseInfo_Query_Resp=0x538094,

			//领取佣兵团薪水信息
			WA_Receive_SalaryInfo_Query=0x530095,
			WA_Receive_SalaryInfo_Query_Resp=0x538095,

			//领取佣兵团经验信息
			WA_Receive_ExperienceInfo_Query=0x530096,
			WA_Receive_ExperienceInfo_Query_Resp=0x538096,

			//排行榜查询
			//等级排行
			WA_Rank_Query=0x530097,
			WA_Rank_Query_Resp=0x538097,

			//竞技排行
			WA_Sports_Query=0x530098,
			WA_Sports_Query_Resp=0x538098,

			//佣兵声望排行
			WA_Prestige_Query=0x530099,
			WA_Prestige_Query_Resp=0x538099,

			//副本积分排行
			WA_CopyIntegral_Query=0x530100,
			WA_CopyIntegral_Query_Resp=0x538100,

			//佣兵团排行
			WA_MercenaryGroup_Query=0x530101,
			WA_MercenaryGroup_Query_Resp=0x538101,

			//操作类:
			//宠物管理:添加宠物(等级\属性)
			WA_PetAdded_Operate=0x530102,
			WA_PetAdded_Operate_Resp=0x538102,

			//宠物管理:删除宠物(等级\属性)
			WA_PetDeleted_Operate=0x530103,
			WA_PetDeleted_Operate_Resp=0x538103,
			
			//家族管理
			WA_AttornPopedom_Operate=0x530104,
			WA_AttornPopedom_Operate_Resp=0x538104,
			
			//佣兵团管理:修改公会名称
			WA_ModifyGuildName_Operate=0x530105,
			WA_ModifyGuildName_Operate_Resp=0x538105,

			//佣兵团管理:踢出公会成员
			WA_KickGuildMember_Operate=0x530106,
			WA_KickGuildMember_Operate_Resp=0x538106,

			//佣兵团管理:解散公会
			WA_DissolveGuild_Operate=0x530107,
			WA_DissolveGuild_Operate_Resp=0x538107,

			//佣兵团仓库:删除物品
			WA_DeleteGoods_Operate=0x530108,
			WA_DeleteGoods_Operate_Resp=0x538108,
			
			//佣兵团仓库:添加物品
			WA_AddGoods_Operate=0x530109,
			WA_AddGoods_Operate_Resp=0x538109,
			
			//佣兵团仓库:修改金钱数量
			WA_ModifyMoney_Operate=0x530110,
			WA_ModifyMoney_Operate_Resp=0x538110,

			//宠物管理:宠物列表查询
			WA_PetList_Query=0x530111,
			WA_PetList_Query_Resp=0x538111,

			//玩家登陆/登出日志(按ip方式查询)
			WA_Query_LoginLogoutByIP=0x530112,
			WA_Query_LoginLogoutByIP_Resp=0x538112,

			//恢复玩家密码
			WA_Query_ResumePassw=0x530113,
			WA_Query_ResumePassw_Resp=0x538113,

			//恢复角色属性
			WA_ResumeAttribute_Op=0x530114,
			WA_ResumeAttribute_Op_Resp=0x538114,

			//批量发送邮件(无道具)
			WA_SendMails_Op=0x530115,
			WA_SendMails_Op_Resp=0x538115,


			//批量封停用户
			WA_ClosePlayerGroup=0x530116,
			WA_ClosePlayerGroup_Resp=0x538116,
			//End

			/////////<summary>
			///侠道飞车消息集(0x54)
			//////</summary>
			//查询用户信息
			CF_Account_Query=0x540001,
			CF_Account_Query_Resp=0x548001,
			
			//强制玩家下线
			CF_KickPlayer=0x540002,
			CF_KickPlayer_Resp=0x548002,
			
			//封停用户
			CF_ClosePlayer=0x540003,
			CF_ClosePlayer_Resp=0x548003,
			
			//解封用户
			CF_OpenPlayer=0x540004,
			CF_OpenPlayer_Resp=0x548004,
			
			//封停列表
			CF_CloseList=0x540005,
			CF_CloseList_Resp=0x548005,
			
			//单个封停
			CF_CloseSingle=0x540006,
			CF_CloseSingle_Resp=0x548006,
			
			//添加公告
			CF_BoardTasker_Add=0x540007,
			CF_BoardTasker_Add_Resp=0x548007,
			
			//删除公告
			CF_BoardTasker_Del=0x540008,
			CF_BoardTasker_Del_Resp=0x548008,
			
			//查询公告
			CF_BoardTasker_Query=0x540009,
			CF_BoardTasker_Query_Resp=0x548009,

			/////////<summary>
			///侠盗金刚消息集(0x55)
			//////</summary>
			//查询用户信息
			XDJG_Account_Query=0x550001,
			XDJG_Account_Query_Resp=0x558001,
			
			//强制玩家下线
			XDJG_KickPlayer=0x550002,
			XDJG_KickPlayer_Resp=0x558002,
			
			//封停用户
			XDJG_ClosePlayer=0x550003,
			XDJG_ClosePlayer_Resp=0x558003,
			
			//解封用户
			XDJG_OpenPlayer=0x550004,
			XDJG_OpenPlayer_Resp=0x558004,
			
			//封停列表
			XDJG_CloseList=0x550005,
			XDJG_CloseList_Resp=0x558005,
			
			//单个封停
			XDJG_CloseSingle=0x550006,
			XDJG_CloseSingle_Resp=0x558006,
			
			//添加公告
			XDJG_BoardTasker_Add=0x550007,
			XDJG_BoardTasker_Add_Resp=0x558007,
			
			//修改公告
			XDJG_BoardTasker_Update=0x550008,
			XDJG_BoardTasker_Update_Resp=0x558008,
			
			//删除公告
			XDJG_BoardTasker_Del=0x550009,
			XDJG_BoardTasker_Del_Resp=0x558009,
			
			//查询公告
			XDJG_BoardTasker_Query=0x550010,
			XDJG_BoardTasker_Query_Resp=0x558010,

			//Added by tengjie 2010-07-06
			//角色技能添加
			XDJG_RoleSkillAdd=0x550011,
			XDJG_RoleSkillAdd_Resp=0x558011,
			
			//角色技能删除
			XDJG_RoleSkillDel=0x550012,
			XDJG_RoleSkillDel_Resp=0x558012,
			
			//道具添加
			XDJG_ItemAdd=0x550013,
			XDJG_ItemAdd_Resp=0x558013,
			
			//道具删除
			XDJG_ItemDel=0x550014,
			XDJG_ItemDel_Resp=0x558014,
			
			//添加游戏币
			XDJG_MoneyAdd=0x550015,
			XDJG_MoneyAdd_Resp=0x558015,

			//日志类:玩家登录日志
			XDJG_Login_Log=0x550016,
			XDJG_Login_Log_Resp=0x558016,

			//商城物品购买日志
			XDJG_Shoping_Log=0x550017,
			XDJG_Shoping_Log_Resp=0x558017,

			//玩家物品日志:交易获得
			XDJG_TransGain_Log=0x550018,
			XDJG_TransGain_Log_Resp=0x558018,

			//玩家物品日志:交易失去
			XDJG_TransLose_Log=0x550019,
			XDJG_TransLose_Log_Resp=0x558019,

			//玩家物品日志:从NPC出购买
			XDJG_BuyFromNpc_Log=0x550020,
			XDJG_BuyFromNpc_Log_Resp=0x558020,

			//玩家物品日志:卖给NPC
			XDJG_SaleToNpc_Log=0x550021,
			XDJG_SaleToNpc_Log_Resp=0x558021,

			//玩家物品日志:拾取物品
			XDJG_PickUpItem_Log=0x550022,
			XDJG_PickUpItem_Log_Resp=0x558022,

			//玩家物品日志:丢弃物品
			XDJG_DiscardItem_Log=0x550023,
			XDJG_DiscardItem_Log_Resp=0x558023,

			//玩家物品日志:物品消耗
			XDJG_ConsumeItem_Log=0x550024,
			XDJG_ConsumeItem_Log_Resp=0x558024,

			//玩家物品日志:拍卖行卖出
			XDJG_AuctioneerSaled_Log=0x550025,
			XDJG_AuctioneerSaled_Log_Resp=0x558025,

			//玩家物品日志:拍卖行购入
			XDJG_AuctioneerBought_Log=0x550026,
			XDJG_AuctioneerBought_Log_Resp=0x558026,

			//玩家邮件日志:发送邮件
			XDJG_SendMail_Log=0x550027,
			XDJG_SendMail_Log_Resp=0x558027,

			//玩家邮件日志:收取邮件
			XDJG_ReceiveMail_Log=0x550028,
			XDJG_ReceiveMail_Log_Resp=0x558028,

			//M币使用日志:背包扩充续费记录
			XDJG_ExpansionPack_Log=0x550029,
			XDJG_ExpansionPack_Log_Resp=0x558029,

			//M币使用日志:传送日志
			XDJG_Deliver_Log=0x550030,
			XDJG_Deliver_Log_Resp=0x558030,

			//M币使用日志:角色仓库扩充续费记录
			XDJG_ExpansionWarehouse_Log=0x550031,
			XDJG_ExpansionWarehouse_Log_Resp=0x558031,

			//玩家角色日志:角色升级
			XDJG_RoleUpgrade_Log=0x550032,
			XDJG_RoleUpgrade_Log_Resp=0x558032,

			//玩家角色日志:角色创建
			XDJG_RoleCreate_Log=0x550033,
			XDJG_RoleCreate_Log_Resp=0x558033,

			//玩家角色日志:角色技能学习
			XDJG_RoleSkill_Log=0x550034,
			XDJG_RoleSkill_Log_Resp=0x558034,

			//玩家角色日志:领取任务
			XDJG_ReceiveTask_Log=0x550035,
			XDJG_ReceiveTask_Log_Resp=0x558035,

			//玩家角色日志:完成任务
			XDJG_CompleteTask_Log=0x550036,
			XDJG_CompleteTask_Log_Resp=0x558036,

			//玩家角色日志:副本查询
			XDJG_Copy_Log=0x550037,
			XDJG_Copy_Log_Resp=0x558037,

			//查询类
			//玩家物品信息:身上物品信息
			XDJG_ItemBody_Query=0x550038,
			XDJG_ItemBody_Query_Resp=0x558038,

			//玩家物品信息:背包物品信息
			XDJG_Itembackpack_Query=0x550039,
			XDJG_Itembackpack_Query_Resp=0x558039,
			
			//玩家物品信息:仓库物品信息
			XDJG_ItemStorage_Query=0x550040,
			XDJG_ItemStorage_Query_Resp=0x558040,
			
			//玩家物品信息:公会仓库信息
			XDJG_GuildStorage_Query=0x550041,
			XDJG_GuildStorage_Query_Resp=0x558041,
			
			//玩家物品信息:金刚仓库信息
			XDJG_KingkongStorage_Query=0x550042,
			XDJG_KingkongStorage_Query_Resp=0x558042,

			//邮件信息
			XDJG_MailInfo_Query=0x550043,
			XDJG_MailInfo_Query_Resp=0x558043,

			//拍卖行信息
			XDJG_AuctioneerInfo_Query=0x550044,
			XDJG_AuctioneerInfo_Query_Resp=0x558044,

			//操作类
			//用户角色信息的修改:修改等级
			XDJG_ModifyLevel_operate=0x550045,
			XDJG_ModifyLevel_operate_Resp=0x558045,
			
			//用户密码管理:修改密码
			XDJG_ModifyPwd_operate=0x550046,
			XDJG_ModifyPwd_operate_Resp=0x558046,
			
			//用户密码管理:临时密码恢复
			XDJG_ResumePwd_operate=0x550047,
			XDJG_ResumePwd_operate_Resp=0x558047,

			//用户角色管理:角色恢复
			XDJG_ResumeRole_operate=0x550048,
			XDJG_ResumeRole_operate_Resp=0x558048,

			//用户角色管理:角色删除
			XDJG_DeleteRole_operate=0x550049,
			XDJG_DeleteRole_operate_Resp=0x558049,

			//修改角色身上金钱数量(绑定币)
			XDJG_UpRoleBind_operate=0x550050,
			XDJG_UpRoleBind_operate_Resp=0x558050,
			
			//修改角色身上金钱数量(非绑定币)
			XDJG_UpRoleNonBind_operate=0x550051,
			XDJG_UpRoleNonBind_operate_Resp=0x558051,

			//角色技能信息查询
			XDJG_RoleSkills_Query=0x550052,
			XDJG_RoleSkills_Query_Resp=0x558052,
			
			//技能列表查询
			XDJG_SkillList_Query=0x550053,
			XDJG_SkillList_Query_Resp=0x558053,
			
			//道具类型查询
			XDJG_ItemType_Query=0x550054,
			XDJG_ItemType_Query_Resp=0x558054,
			
			//相应类型的道具列表查询
			XDJG_ItemNameLst_Query=0x550055,
			XDJG_ItemNameLst_Query_Resp=0x558055,

			//游戏币查询
			XDJG_MoneyQuery=0x550056,
			XDJG_MoneyQuery_Resp=0x558056,

			//添加角色身上金钱数量(绑定币)
			XDJG_AddRoleBind_operate=0x550057,
			XDJG_AddRoleBind_operate_Resp=0x558057,
			
			//添加角色身上金钱数量(非绑定币)
			XDJG_AddRoleNonBind_operate=0x550058,
			XDJG_AddRoleNonBind_operate_Resp=0x558058,

			//物品属性查询
			XDJG_ItemAttribute_Query=0x550059,
			XDJG_ItemAttribute_Query_Resp=0x558059,

			//公会信息查询
			//公会基本信息
			XDJG_GuildBaseInfo_Query=0x550060,
			XDJG_GuildBaseInfo_Query_Resp=0x558060,

			//公会成员列表
			XDJG_GuildMemberList_Query=0x550061,
			XDJG_GuildMemberList_Query_Resp=0x558061,

			//玩家任务查询
			XDJG_PlayerTaskInfo_Query=0x550062,
			XDJG_PlayerTaskInfo_Query_Resp=0x558062,
			
			//金刚车辆信息
			//金刚车辆基本信息
			XDJG_CarBaseInfo_Query=0x550063,
			XDJG_CarBaseInfo_Query_Resp=0x558063,

			//金刚车辆详细信息
			XDJG_CarDetailInfo_Query=0x550064,
			XDJG_CarDetailInfo_Query_Resp=0x558064,

			//玩家公会信息
			//玩家公会基本信息
			XDJG_PlayerGuildBaseInfo_Query=0x550065,
			XDJG_PlayerGuildBaseInfo_Query_Resp=0x558065,

			//玩家公会物品信息
			XDJG_PlayerGuildItemInfo_Query=0x550066,
			XDJG_PlayerGuildItemInfo_Query_Resp=0x558066,

			//玩家物品日志
			//装备增幅日志
			XDJG_EquipIncrease_Log=0x550067,
			XDJG_EquipIncrease_Log_Resp=0x558067,

			//插片合成日志
			XDJG_Synthesis_Log=0x550068,
			XDJG_Synthesis_Log_Resp=0x558068,

			//装备传导日志
			XDJG_EquipTransfer_Log=0x550069,
			XDJG_EquipTransfer_Log_Resp=0x558069,

			//物品鉴定
			XDJG_ItemIdentify_Log=0x550070,
			XDJG_ItemIdentify_Log_Resp=0x558070,

			//个人仓库存放日志
			XDJG_RoleStore_Log=0x550071,
			XDJG_RoleStore_Log_Resp=0x558071,

			//公会仓库存放日志
			XDJG_GuildStore_Log=0x550072,
			XDJG_GuildStore_Log_Resp=0x558072,

			//公会日志
			//公会成立
			XDJG_GuildCreate_Log=0x550073,
			XDJG_GuildCreate_Log_Resp=0x558073,

			//公会解散
			XDJG_GuildDismiss_Log=0x550074,
			XDJG_GuildDismiss_Log_Resp=0x558074,

			//公会添加成员
			XDJG_GuildAddMember_Log=0x550075,
			XDJG_GuildAddMember_Log_Resp=0x558075,

			//公会成员离开
			XDJG_GuildMemberLeaved_Log=0x550076,
			XDJG_GuildMemberLeaved_Log_Resp=0x558076,

			//公会职务变更
			XDJG_GuildPostChanged_Log=0x550077,
			XDJG_GuildPostChanged_Log_Resp=0x558077,
			
			//帐号是否封停
			XDJG_AccountIsClose_Query=0x550078,
			XDJG_AccountIsClose_Query_Log_Resp=0x558078,

			//NPC道具领取查询
			XDJG_NPCGetItem_Query=0x550079,
			XDJG_NPCGetItem_Query_Resp=0x558079,

			//添加称号
			XDJG_AddTitle_Query=0x550080,
			XDJG_AddTitle_Query_Resp=0x558080,

			//移除称号
			XDJG_RemoveTitle_Query=0x550081,
			XDJG_RemoveTitle_Query_Resp=0x558081,

			//添加BUFF
			XDJG_AddBuff_Query=0x550082,
			XDJG_AddBuff_Query_Resp=0x558082,

			//移除BUFF
			XDJG_RemoveBuff_Query=0x550083,
			XDJG_RemoveBuff_Query_Resp=0x558083,

			//移动到指定位置
			XDJG_MoveToPosition_Query=0x550084,
			XDJG_MoveToPosition_Query_Resp=0x558084,

			//移动到指定玩家身边
			XDJG_MoveToRole_Query=0x550085,
			XDJG_MoveToRole_Query_Resp=0x558085,

			//移动到目标玩家身边
			XDJG_MoveToTargetRole_Query=0x550086,
			XDJG_MoveToTargetRole_Query_Resp=0x558086,

			//称号列表查询
			XDJG_TitleList_Query=0x550087,
			XDJG_TitleList_Query_Resp=0x558087,
			
			//BUFF列表查询
			XDJG_BuffList_Query=0x550088,
			XDJG_BuffList_Query_Resp=0x558088,
			//End

			/// <summary>
			/// 劲舞团2GM工具(0x61)
			/// </summary>
			////////////////////////查询类///////////////////////////
			JW2_ACCOUNT_QUERY = 0x610001,//玩家帐号信息查询（1.2.3.4.5.8）
			JW2_ACCOUNT_QUERY_RESP = 0x618001,//玩家帐号信息查询响应（1.2.3.4.5.8）
			/////////////封停类/////////////////////////////////////////
			JW2_ACCOUNTREMOTE_QUERY = 0x610002,//游戏服务器封停的玩家帐号查询
			JW2_ACCOUNTREMOTE_QUERY_RESP = 0x618002,//游戏服务器封停的玩家帐号查询响应
			
			JW2_ACCOUNTLOCAL_QUERY = 0x610003,//本地封停的玩家帐号查询
			JW2_ACCOUNTLOCAL_QUERY_RESP = 0x618003,//本地封停的玩家帐号查询响应
			
			JW2_ACCOUNT_CLOSE = 0x610004,//封停的玩家帐号
			JW2_ACCOUNT_CLOSE_RESP = 0x618004,//封停的玩家帐号响应
			JW2_ACCOUNT_OPEN = 0x610005,//解封的玩家帐号
			JW2_ACCOUNT_OPEN_RESP= 0x618005,//解封的玩家帐号响应
			JW2_ACCOUNT_BANISHMENT_QUERY = 0x610006,//玩家封停帐号查询
			JW2_ACCOUNT_BANISHMENT_QUERY_RESP = 0x618006,//玩家封停帐号查询响应
			////////////////////////////
			JW2_BANISHPLAYER=0x610007,//踢人
			JW2_BANISHPLAYER_RESP=0x618007,//踢人响应
			JW2_BOARDMESSAGE=0x610002,//公告
			JW2_BOARDMESSAGE_RESP=0x618002,//公告响应
			
			JW2_RPG_QUERY=0x610010,//查询故事情节（6）
			JW2_RPG_QUERY_RESP=0x618010,//查询故事情节响应（6）
			
			JW2_ITEMSHOP_BYOWNER_QUERY = 0x610011,////查看玩家身上道具信息（7－7）
			JW2_ITEMSHOP_BYOWNER_QUERY_RESP = 0x618011,////查看玩家身上道具信息响应（7－7）
			
			
			JW2_DUMMONEY_QUERY = 0x610012,////查询点数和虚拟币（7－8）
			JW2_DUMMONEY_QUERY_RESP = 0x618012,////查询点数和虚拟币响应（7－8）
			JW2_HOME_ITEM_QUERY = 0x610013,////查看房间物品清单与期限（7－9）
			JW2_HOME_ITEM_QUERY_RESP = 0x618013,////查看房间物品清单与期限响应（7－9）
			JW2_SMALL_PRESENT_QUERY = 0x610014,////查看购物，送礼（7－10）
			JW2_SMALL_PRESENT_QUERY_RESP = 0x618014,////查看购物，送礼响应（7－10）
			JW2_WASTE_ITEM_QUERY = 0x610015,////查看消耗性道具（7－11）
			JW2_WASTE_ITEM_QUERY_RESP = 0x618015,////查看消耗性道具响应（7－11）
			JW2_SMALL_BUGLE_QUERY = 0x610016,////查看小喇叭（7－12）
			JW2_SMALL_BUGLE_QUERY_RESP  = 0x618016,////查看小喇叭响应（7－12）
			
			JW2_LOGINOUT_QUERY=0x610009,//玩家人物登入/登出信息
			JW2_LOGINOUT_QUERY_RESP=0x618009,//玩家人物登入/登出信息响应
			JW2_WEDDINGINFO_QUERY=0x610017,//婚姻信息
			JW2_WEDDINGINFO_QUERY_RESP=0x618017,
			JW2_WEDDINGLOG_QUERY=0x610018,//婚姻历史
			JW2_WEDDINGLOG_QUERY_RESP=0x618018,
			JW2_WEDDINGGROUND_QUERY=0x610019,//结婚信息
			JW2_WEDDINGGROUND_QUERY_RESP=0x618019,
			JW2_COUPLEINFO_QUERY=0x610020,//情人信息
			JW2_COUPLEINFO_QUERY_RESP=0x618020,
			JW2_COUPLELOG_QUERY=0x610021,//情人历史
			JW2_COUPLELOG_QUERY_RESP=0x618021,
			
			
			JW2_FAMILYINFO_QUERY=0x610022,//家族信息
			JW2_FAMILYINFO_QUERY_RESP=0x618022,
			JW2_FAMILYMEMBER_QUERY=0x610023,//家族成员信息
			JW2_FAMILYMEMBER_QUERY_RESP=0x618023,
			JW2_SHOP_QUERY=0x610024,//商城信息
			JW2_SHOP_QUERY_RESP=0x618024,
			JW2_User_Family_Query=0x610025,//用户家族信息
			JW2_User_Family_Query_Resp=0x618025,
			
			JW2_FamilyItemInfo_Query=0x610026,//家族道具信息
			JW2_FamilyItemInfo_Query_Resp=0x618026,
			
			JW2_FamilyBuyLog_Query=0x610027,//家族购买日志
			JW2_FamilyBuyLog_Query_Resp=0x618027,
			
			JW2_FamilyTransfer_Query=0x610028,//家族转让信息
			JW2_FamilyTransfer_Query_Resp=0x618028,

			JW2_FriendList_Query=0x610029,//好友列表
			JW2_FriendList_Query_Resp=0x618029,

			JW2_BasicInfo_Query=0x610030,//基地信息查询
			JW2_BasicInfo_Query_Resp=0x618030,
			
			JW2_FamilyMember_Applying=0x610031,//申请中成员
			JW2_FamilyMember_Applying_Resp=0x618031,
			
			JW2_BasicRank_Query=0x610032,//基地等级
			JW2_BasicBank_Query_Resp=0x618032,
			
			
			///////////公告////////////////////////////
			JW2_BOARDTASK_INSERT = 0x610033,//公告添加
			JW2_BOARDTASK_INSERT_RESP = 0x618033,//公告添加响应
			JW2_BOARDTASK_QUERY = 0x610034,//公告查询
			JW2_BOARDTASK_QUERY_RESP = 0x618034,//公告查询响应
			JW2_BOARDTASK_UPDATE = 0x610035,//公告更新
			JW2_BOARDTASK_UPDATE_RESP = 0x618035,//公告更新响应
			
			JW2_ITEM_DEL=0x610036,//道具删除（身上0，物品栏1，礼盒2）
			JW2_ITEM_DEL_RESP=0x618036,//道具删除（身上0，物品栏1，礼盒2）
			
			JW2_MODIFYSEX_QUERY=0x610037,//修改角色性别
			JW2_MODIFYSEX_QUERY_RESP=0x618037,
			
			JW2_MODIFYLEVEL_QUERY=0x610038,//修改等级
			JW2_MODIFYLEVEL_QUERY_RESP=0x618038,
			
			JW2_MODIFYEXP_QUERY=0x610039,//修改经验
			JW2_MODIFYEXP_QUERY_RESP=0x618039,
			
			JW2_BAN_BIGBUGLE=0x610040,//禁用大喇叭
			JW2_BAN_BIGBUGLE_RESP=0x618040,
			
			JW2_BAN_SMALLBUGLE=0x610041,//禁用小喇叭
			JW2_BAN_SMALLBUGLE_RESP=0x618041,
			
			JW2_DEL_CHARACTER=0x610042,//删除角色
			JW2_DEL_CHARACTER_RESP=0x618042,
			
			JW2_RECALL_CHARACTER=0x610043,//恢复角色
			JW2_RECALL_CHARACTER_RESP=0x618043,
			
			JW2_MODIFY_MONEY=0x610044,//修改金钱
			JW2_MODIFY_MONEY_RESP=0x618044,
			
			JW2_ADD_ITEM=0x610045,//增加道具
			JW2_ADD_ITEM_RESP=0x618045,
			
			JW2_CREATE_GM=0x610046,//每个大区创建GM
			JW2_CREATE_GM_RESP=0x618046,
			
			JW2_MODIFY_PWD=0x610047,//修改密码
			JW2_MODIFY_PWD_RESP=0x618047,
			
			JW2_RECALL_PWD=0x610048,//恢复密码
			JW2_RECALL_PWD_RESP=0x618048,
			
			
			JW2_ItemInfo_Query=0x610049,//道具查询
			JW2_ItemInfo_Query_Resp=0x618049,//

			
			JW2_ITEM_SELECT=0x610050,//道具模糊查询
			JW2_ITEM_SELECT_RESP=0x618050,//
			
			JW2_PetInfo_Query=0x610051,//宠物信息
			JW2_PetInfo_Query_Resp=0x618051,
			
			JW2_Messenger_Query=0x610052,//称号查询
			JW2_Messenger_Query_Resp=0x618052,
			
			JW2_Wedding_Paper=0x610053,//结婚证书
			JW2_Wedding_Paper_Resp=0x618053,
			
			JW2_CoupleParty_Card=0x610054,//情侣派对卡
			JW2_CoupleParty_Card_Resp=0x618054,
			
			
			JW2_MailInfo_Query=0x610055,//纸箱信息
			JW2_MailInfo_Query_Resp=0x618055,
			
			JW2_MoneyLog_Query=0x610056,//金钱日志查询
			JW2_MoneyLog_Query_Resp=0x618056,
			
			JW2_FamilyFund_Log=0x610057,//基金日志
			JW2_FamilyFund_Log_Resp=0x618057,
			
			JW2_FamilyItem_Log=0x610058,//家族道具日志
			JW2_FamilyItem_Log_Resp=0x618058,
			
			JW2_Item_Log=0x610059,//道具日志
			JW2_Item_Log_Resp=0x618059,

			JW2_Task_Log=0x610060,//任务日志
			JW2_Task_Log_Resp=0x618060,

			JW2_CashMoney_Log=0x610061,//消费日志
			JW2_CashMoney_Log_Resp=0x618061,

			JW2_Import_KickPlayer=0x610062,//批量踢人
			JW2_Import_KickPlayer_Resp=0x618062,

			JW2_Import_AccountClose=0x610063,//批量封停
			JW2_Import_AccountClose_Resp=0x618063,

			JW2_Import_AccountOpen=0x610064,//批量封停
			JW2_Import_AccountOpen_Resp=0x618064,

			JW2_GM_Update=0x610065,//更改GM状态
			JW2_GM_Update_Resp=0x618065,
			
			JW2_ProductManage_Insert=0x610066,//插入权限
			JW2_ProductManage_Insert_Resp=0x618066,
			
			JW2_ProductManage_View=0x610067,//查看权限
			JW2_ProductManage_View_Resp=0x618067,
			
			JW2_ProductManage_Del=0x610068,//删除权限
			JW2_ProductManage_Del_Resp=0x618068,

			//maple add
			JW2_ProductManage_Validate=0x610069,//验证权限
			JW2_ProductManage_Validate_Resp=0x618069,

			JW2_ProductManage_Deduct=0x610070,///验证通过减去权限
			JW2_ProductManage_Deduct_Resp=0x618070,
			//////////////////
			//CG2(0x71)
			/////////////////	
			//角色资料
			CG2_Account_Query = 0x710001,//账号查询基本资料
			CG2_Account_Query_Resp = 0x718001,//账号查询基本资料
			CG2_Account_Detail_Query = 0x710002, //角色详细资料
			CG2_Account_Detail_Query_Resp = 0x718002, //角色详细资料
			CG2_Account_Attribute_Query = 0x710003,//角色属性资料
			CG2_Account_Attribute_Query_Resp = 0x718003,//角色属性资料
			CG2_Account_Guild_Query = 0x710004,//角色工会资料
			CG2_Account_Guild_Query_Resp = 0x718004,//角色工会资料
			CG2_Account_Title_Query = 0x710005,//角色称号资料
			CG2_Account_Title_Query_Resp = 0x718005,//角色称号资料
			CG2_Account_Address_Query = 0x710006,//角色社会关系
			CG2_Account_Address_Query_Resp = 0x718006,//角色社会关系
			CG2_Account_Skill_Query = 0x710007,//角色技能信息
			CG2_Account_Skill_Query_Resp = 0x718007,//角色技能信息
			//背包物品资料
			CG2_CharItem_Query = 0x710008,//角色身上道具信息
			CG2_CharItem_Query_Resp = 0x718008,//角色身上道具信息
			CG2_TempItem_Query = 0x710009,//临时背包信息
			CG2_TempItem_Query_Resp = 0x718009,//临时背包信息
			CG2_ShopItem_Query = 0x710010,//商城物品领取
			CG2_ShopItem_Query_Resp = 0x718010,//商城物品领取
			//银行物品资料
			CG2_BankInfo_Query = 0x710011,//银行存款信息
			CG2_BankInfo_Query_Resp = 0x718011,//银行存款信息
			CG2_BankItem_Query = 0x710012,//银行物品信息
			CG2_BankItem_Query_Resp = 0x718012,//银行物品信息
			//宠物信息
			CG2_PetInfo_Query = 0x710013,//宠物基本资料
			CG2_PetInfo_Query_Resp = 0x718013,//宠物基本资料
			CG2_PetInfo_Detail_Query = 0x710014,//宠物详细资料
			CG2_PetInfo_Detail_Query_Resp = 0x718014,//宠物详细资料
			CG2_PetItem_Query = 0x710015,//宠物背包信息
			CG2_PetItem_Query_Resp = 0x718015,//	宠物背包信息
			CG2_PetSkill_Query = 0x710016,//宠物技能信息
			CG2_PetSkill_Query_Resp = 0x718016,//	宠物技能信息
			//任务
			CG2_Job_Query = 0x710017,	//任务 
			CG2_Job_Query_Resp = 0x718017,//任务 
			//账号激活查询
			CG2_Account_Active_Query = 0x710018,	//通过账号查询
			CG2_Account_Active_Query_Resp = 0x718018,//通过账号查询
			CG2_Sn_Active_Query  = 0x710019,		//通过激活码查询
			CG2_Sn_Active_Query_Resp  = 0x718019,//通过激活码查询
			//系统踢人
			CG2_Ban_Query = 0x710020,//根据选择时间范围、次数进行筛选，将指定服务器中符合条件的角色
			CG2_Ban_Query_Resp = 0x718020,//根据选择时间范围、次数进行筛选，将指定服务器中符合条件的角色
			CG2_Account_Ban_Query = 0x710021,//根据选择时间范围以账号、昵称进行查询，显示踢人次数
			CG2_Account_Ban_Query_Resp = 0x718021,//根据选择时间范围以账号、昵称进行查询，显示踢人次数
			
			//帐号登陆信息
			CG2_Use_Query = 0x710022,	//帐号登陆信息
			CG2_Use_Query_Resp = 0x718022,//帐号登陆信息
			CG2_Account_Ip_Query = 0x710023,	//通过IP地址查询所登陆的帐号
			CG2_Account_Ip_Query_Resp = 0x718023,//通过IP地址查询所登陆的帐号
			CG2_Account_User_Query = 0x710024,	//通过帐号查询历史登陆IP
			CG2_Account_User_Query_Resp = 0x718024,//通过帐号查询历史登陆IP
			CG2_UseLog_Query = 0x710025,	//邮件日志查询信息
			CG2_UseLog_Query_Resp = 0x718025,//邮件日志查询信息
			
			//操作类
			//道具
			CG2_Get_Item_Query = 0x710026,//获取游戏道具（大类）
			CG2_Get_Item_Query_Resp = 0x718026,//获取游戏道具（大类）
			CG2_Get_Item_Detail_Query = 0x710027,	//获取游戏道具（小类）
			CG2_Get_Item_Detail_Query_Resp = 0x718027,	//获取游戏道具（小类）
			
			
			//账号
			CG2_Account_Close = 0x710028,//封停玩家帐号
			CG2_Account_Close_RESP = 0x718028,//封停玩家帐号响应
			CG2_Account_Open = 0x710029,//解封玩家帐号
			CG2_Account_Open_RESP= 0x718029,//解封玩家帐号响应
			CG2_Account_BANISHMENT_QUERY = 0x710030,//玩家封停帐号查询状态
			CG2_Account_BANISHMENT_QUERY_RESP = 0x718030,//玩家封停帐号查询状态响应
			CG2_BanAccount_Query = 0x710031,//查询所有封停玩家帐号信息
			CG2_BanAccount_Query_Resp = 0x718031,//查询所有封停玩家帐号信息
			CG2_KickAccount_Query = 0x710032,//踢下线
			CG2_KickAccount_Query_Resp = 0x718032,//踢下线
		
			//地图
			CG2_Change_Map_Query = 0x710033,//地图传送
			CG2_Change_Map_Query_Resp = 0x718033,//地图传送

			//公告
			CG2_SendNote_Query = 0x710034,//发送公告
			CG2_SendNote_Query_Resp = 0x718034,//发送公告
			CG2_UpdateNote_Query = 0x710035,//修改公告
			CG2_UpdateNote_Query_Resp= 0x718035,//修改公告
			CG2_DeleteNote_Query = 0x710036,//删除公告
			CG2_DeleteNote_Query_Resp= 0x718036,//删除公告

			

			//角色
			CG2_Update_Account_Query = 0x710037,//修改角色信息
			CG2_Update_Account_Query_Resp = 0x718037,//修改角色信息
			CG2_Update_Pet_Query = 0x710038,//修改宠物信息
			CG2_Update_Pet_Query_Resp = 0x718038,//修改宠物信息

			//技能
			CG2_Insert_Account_Skill_Query = 0x710039,//添加角色技能信息
			CG2_Insert_Account_Skill_Query_Resp = 0x718039,//添加角色技能信息
			CG2_Insert_Pet_Skill_Query = 0x710040,//添加宠物技能信息
			CG2_Insert_Pet_Skill_Query_Resp = 0x718040,//添加宠物技能信息

			CG2_Update_Account_Skill_Query = 0x710041,//修改角色技能信息
			CG2_Update_Account_Skill_Query_Resp = 0x718041,//修改角色技能信息
			CG2_Update_Pet_Skill_Query = 0x710042,//修改宠物技能信息
			CG2_Update_Pet_Skill_Query_Resp = 0x718042,//修改宠物技能信息

			CG2_Delete_Account_Skill_Query = 0x710043,//删除角色技能信息
			CG2_Delete_Account_Skill_Query_Resp = 0x718043,//删除角色技能信息
			CG2_Delete_Pet_Skill_Query = 0x710044,//删除宠物技能信息
			CG2_Delete_Pet_Skill_Query_Resp = 0x718044,//删除宠物技能信息

			//临时账号
			CG2_Insert_TmpPwd_Query = 0x710045,//添加临时密码
			CG2_Insert_TmpPwd_Query_Resp = 0x718045,//添加临时密码
			CG2_Recover_TmpPwd_Query = 0x710046,//恢复用户密码
			CG2_Recover_TmpPwd_Query_Resp = 0x718046,//恢复用户密码


			CG2_Insert_Item_Query = 0x710047,//添加游戏道具
			CG2_Insert_Item_Query_Resp = 0x718047,//添加游戏道具
			CG2_RealAccount_Query = 0x710048,//查询服务器用户基本资料
			CG2_RealAccount_Query_Resp = 0x718048,//查询服务器用户基本资料

			CG2_SearchItem_Query = 0x710049,//模糊查询道具
			CG2_SearchItem_Query_Resp = 0x718049,//模糊查询道具

			CG2_SearchNote_Query = 0x710050,//查看公告
			CG2_SearchNote_Query_Resp= 0x718050,//查看公告
			CG2_MapList_Query = 0x710051,//查询地图列表
			CG2_MapList_Query_Resp = 0x718051,//查询地图列表
			
			CG2_SearchGuild_Query = 0x710052,//查询工会信息
			CG2_SearchGuild_Query_Resp = 0x718052,//查询工会信息

			CG2_SearchGuild_Detail_Query = 0x710053,//查询工会信息
			CG2_SearchGuild_Detail_Query_Resp = 0x718053,//查询工会信息

			CG2_CloseGuild_Query = 0x710054,//解散公会
			CG2_CloseGuild_Query_Resp = 0x718054,//解散公会

			CG2_RenameGuild_Query = 0x710055,//修改公会名字
			CG2_RenameGuild_Query_Resp = 0x718055,//修改公会名字

			CG2_Recovery_Account_Qurey = 0x710056,//恢复角色信息
			CG2_Recovery_Account_Qurey_Resp = 0x718056,//恢复角色信息

			CG2_Search_Account_Qurey = 0x710057,//查询角色存在
			CG2_Search_Account_Qurey_Resp = 0x718057,//查询角色存在

			CG2_BakAccount_Query = 0x710058,//查询备份用户
			CG2_BakAccount_Query_Resp = 0x718058,//查询备份用户

			//发送问答系统
			CG2_SendAsk_Query = 0x710059,//发送问答系统
			CG2_SendAsk_Query_Resp = 0x718059,//发送问答系统
			
			//修改角色属性
			CG2_UpdateAccount_Query = 0x710060,//修改角色属性
			CG2_UpdateAccount_Query_Resp = 0x718060,//修改角色属性

			//修改角色等级
			CG2_UpdateLevel_Query = 0x710061,//修改角色等级
			CG2_UpdateLevel_Query_Resp = 0x718061,//修改角色等级

			//修改角色职业
			CG2_UpdateJob_Query = 0x710062,//修改角色职业
			CG2_UpdateJob_Query_Resp = 0x718062,//修改角色职业

			//获取角色职业
			CG2_GetJob_Query = 0x710063,//获取角色职业
			CG2_GetJob_Query_Resp = 0x718063,//获取角色职业

			CG2_PackItem_Query = 0x710064,//背包道具信息
			CG2_PackItem_Query_Resp = 0x718064,//背包道具信息

			CG2_GetSkill_Query = 0x710065,//获得人物技能类型
			CG2_GerSkill_Query_Resp = 0x718065,//获得人物技能类型
			
			CG2_SkillList_Query = 0x710066,//获得人物技能列表
			CG2_SkillList_Query_Resp = 0x718066,//获得人物技能列表


			CG2_GetPetSkill_Query = 0x710067,//获得宠物技能类型
			CG2_GerPetSkill_Query_Resp = 0x718067,//获得宠物技能类型
			
			CG2_PetSkillList_Query = 0x710068,//获得宠物技能列表
			CG2_PetSkillList_Query_Resp = 0x718068,//获得宠物技能列表

			//修改银行资料信息
			CG2_InsertBankItem_Query = 0x710069,//银行道具添加
			CG2_InsertBankItem_Query_Resp = 0x718069,//银行道具添加

			CG2_SetBankMoney_Query = 0x710070,//银行金额设置
			CG2_SetBankMoney_Query_Resp = 0x718070,//银行金额设置

			//修改宠物资料信息
			CG2_ModiPetAttribute_Query = 0x710071,//修改宠物属性信息
			CG2_ModiPetAttribute_Query_Resp = 0x718071,//

			CG2_PetList_Query = 0x710072,//获取宠物列表
			CG2_PetList_Query_Resp = 0x718072,//获取宠物列表

			CG2_AddPet_Query = 0x710073,//添加宠物
			CG2_AddPet_Query_Resp = 0x718073,//添加宠物
			//恢复日志道具
			CG2_LogItem_Query = 0x710074,//查看日志道具信息
			CG2_LogItem_Query_Resp = 0x718074,//查看日志道具信息

			CG2_RecoverLogItem_Query = 0x710075,//恢复日志道具信息
			CG2_RecoverLogItem_Query_Resp = 0x718075,//恢复日志道具信息

			CG2_Account_Teach_Query = 0x710076,//查看玩家师生关系
			CG2_Account_Teach_Query_Resp = 0x718076,//查看玩家师生关系
			CG2_Send_ItemPassword = 0x710077,//获取道具锁
			CG2_Send_ItemPassword_Resp = 0x718077,//获取道具锁

			CG2_SearchPassWord_Query = 0x710078,//查询临时密码
			CG2_SearchPassWord_Query_Resp = 0x718078,//查询临时密码

			CG2_InsertItemALL_Query = 0x710079,//添加道具（导入）
			CG2_InsertItemALL_Query_Resp = 0x718079,//添加道具（导入）

			CG2_UserShoppingInfo_Query = 0x710080,//商城信息查询
			CG2_UserShoppingInfo_Query_Resp = 0x718080,//商城信息查询

			CG2_UserGoldCard_Query = 0x710081,//黄金激活卡
			CG2_UserGoldCard_Query_Resp = 0x718081,//黄金激活卡

			CG2_InsertBankItemALL_Query = 0x710082,//批量银行道具添加（导入）
			CG2_InsertBankItemALL_Query_Resp = 0x718082,//批量银行道具添加（导入）

			CG2_SearchMap_Query = 0x710083,//模糊查询地图
			CG2_SearchMap_Query_Resp = 0x718083,//模糊查询地图

			CG2_PetPic_Query = 0x710084,//宠物图鉴信息
			CG2_PetPic_Query_Resp = 0x718084,//宠物图鉴信息

			CG2_UpdatePetLevel_Query = 0x710085,//修改宠物等级
			CG2_UpdatePetLevel_Query_Resp = 0x718085,//修改宠物等级

			CG2_BankPet_Query = 0x710086,//银行宠物信息
			CG2_BankPet_Query_Resp = 0x718086,//银行宠物信息

			CG2_UpdateGuildLv_Query=0x710087,//工会等级修改
			CG2_UpdateGuildLv_Query_Resp=0x718087,//工会等级修改

			CG2_UpdateGuildHonor_Query=0x710088,//工会荣誉修改
			CG2_UpdateGuildHonor_Query_Resp=0x718088,//工会荣誉修改

			CG2_LogInfo_Pet_Query = 0x710089,//宠物详细属性
			CG2_LogInfo_Pet_Query_Resp = 0x718089,//宠物详细属性

			CG2_LogInfo_Item_Query = 0x710090,//道具详细属性
			CG2_LogInfo_Item_Query_Resp = 0x718090,//道具详细属性

			CG2_AccountErr_Query = 0x710091,//人物等级异常回档
			CG2_AccountErr_Query_Resp = 0x718091,//人物等级异常回档

			CG2_ChangeGold_Query = 0x710092,//修改随身金钱
			CG2_ChangeGold_Query_Resp = 0x718092,//修改随身金钱

			CG2_BagItem_Del_Query = 0x710093,//删除角色道具栏道具
			CG2_BagItem_Del_Query_Resp = 0x718093,//删除角色道具栏道具

			CG2_TmpItem_Del_Query = 0x710094,//删除临时道具栏道具
			CG2_TmpItem_Del_Query_Resp = 0x718094,//删除临时道具栏道具

			CG2_EquipItem_Del_Query = 0x710095,//删除随身装备道具
			CG2_EquipItem_Del_Query_Resp = 0x718095,//删除随身装备道具

			CG2_BankItem_Del_Query = 0x710096,//删除银行道具
			CG2_BankItem_Del_Query_Resp = 0x718096,//删除银行道具

			CG2_PetPic_Del_Query = 0x710097,//设置/删除宠物图鉴
			CG2_PetPic_Del_Query_Resp = 0x718097,//设置/删除宠物图鉴

			CG2_KickAccountCount_Query = 0x710098,//踢下线次数
			CG2_KickAccountCount_Query_Resp = 0x718098,//踢下线次数

			CG2_RecoverLogPet_Query = 0x710099,//恢复日志宠物
			CG2_RecoverLogPet_Query_Resp = 0x718099,//恢复日志宠物

			CG2_LoveTree_Query = 0x710100,//爱情树信息查询
			CG2_LoveTree_Query_Resp = 0x718100,//爱情树信息查询

			CG2_GuildMazeWar_Query = 0x710101,//公会迷宫占领
			CG2_GuildMazeWar_Query_Resp = 0x718101,//公会迷宫占领

			CG2_Marriage_Info_Query = 0x710102,//玩家婚姻信息
			CG2_Marriage_Info_Query_Resp = 0x718102,//玩家婚姻信息

			CG2_ChangeCharNo_Query = 0x710103,//角色换位
			CG2_ChangeCharNo_Query_Resp = 0x718103,//角色换位
				
			CG2_Log_Query=0x710104,
			CG2_Log_Query_Resp=0x718104,////查询玩家日志信息

			CG2_Get_EmailAddress=0x710105,//得到Email地址
			CG2_Get_EmailAddress_Resp=0x718105,//


			/// <summary>
            /// 超级舞者GM工具(0x86)
            /// </summary>
            SDO_ACCOUNT_QUERY = 0x870026,//查看玩家的帐号信息
            SDO_ACCOUNT_QUERY_RESP = 0x878026,//查看玩家的帐号信息响应
            SDO_CHARACTERINFO_QUERY = 0x870027,//查看任务资料的信息
            SDO_CHARACTERINFO_QUERY_RESP = 0x878027,//查看人物资料的信息响应
            SDO_ACCOUNT_CLOSE = 0x870028,//封停帐户的权限信息
            SDO_ACCOUNT_CLOSE_RESP = 0x878028,//封停帐户的权限信息响应
            SDO_ACCOUNT_OPEN = 0x870029,//解封帐户的权限信息
            SDO_ACCOUNT_OPEN_RESP = 0x878029,//解封帐户的权限信息响应
            SDO_PASSWORD_RECOVERY = 0x870030,//玩家找回密码
            SDO_PASSWORD_RECOVERY_RESP = 0x878030,//玩家找回密码响应
            SDO_CONSUME_QUERY = 0x870031,//查看玩家的消费记录
            SDO_CONSUME_QUERY_RESP = 0x878031,//查看玩家的消费记录响应
            SDO_USERONLINE_QUERY = 0x870032,//查看玩家上下线状态
            SDO_USERONLINE_QUERY_RESP = 0x878032,//查看玩家上下线状态响应
            SDO_USERTRADE_QUERY = 0x870033,//查看玩家交易状态
            SDO_USERTRADE_QUERY_RESP = 0x878033,//查看玩家交易状态响应
            SDO_CHARACTERINFO_UPDATE = 0x870034,//修改玩家的账号信息
            SDO_CHARACTERINFO_UPDATE_RESP = 0x878034,//修改玩家的账号信息响应
            SDO_ITEMSHOP_QUERY = 0x870035,//查看游戏里面所有道具信息
            SDO_ITEMSHOP_QUERY_RESP = 0x878035,//查看游戏里面所有道具信息响应
            SDO_ITEMSHOP_DELETE = 0x870036,//删除玩家道具信息
            SDO_ITEMSHOP_DELETE_RESP = 0x878036,//删除玩家道具信息响应
            SDO_GIFTBOX_CREATE = 0x870037,//添加玩家礼物盒道具信息
            SDO_GIFTBOX_CREATE_RESP = 0x878037,//添加玩家礼物盒道具信息响应
            SDO_GIFTBOX_QUERY = 0x870038,//查看玩家礼物盒的道具
            SDO_GIFTBOX_QUERY_RESP = 0x878038,//查看玩家礼物盒的道具响应
            SDO_GIFTBOX_DELETE = 0x870039,//删除玩家礼物盒的道具
            SDO_GIFTBOX_DELETE_RESP = 0x878039,//删除玩家礼物盒的道具响应
            SDO_USERLOGIN_STATUS_QUERY = 0x870040,//查看玩家登录状态
            SDO_USERLOGIN_STATUS_QUERY_RESP = 0x878040,//查看玩家登录状态响应
            SDO_ITEMSHOP_BYOWNER_QUERY = 0x870041,////查看玩家身上道具信息
            SDO_ITEMSHOP_BYOWNER_QUERY_RESP = 0x878041,////查看玩家身上道具信息
            SDO_ITEMSHOP_TRADE_QUERY = 0x870042,//查看玩家交易记录信息
            SDO_ITEMSHOP_TRADE_QUERY_RESP = 0x878042,//查看玩家交易记录信息的响应
            SDO_MEMBERSTOPSTATUS_QUERY = 0x870043,//查看该帐号状态
            SDO_MEMBERSTOPSTATUS_QUERY_RESP = 0x878043,///查看该帐号状态的响应
            SDO_MEMBERBANISHMENT_QUERY = 0x870044,//查看所有停封的帐号
            SDO_MEMBERBANISHMENT_QUERY_RESP = 0x878044,//查看所有停封的帐号响应
            SDO_USERMCASH_QUERY = 0x870045,//玩家充值记录查询
            SDO_USERMCASH_QUERY_RESP = 0x878045,//玩家充值记录查询响应
            SDO_USERGCASH_UPDATE = 0x870046,//补偿玩家G币
            SDO_USERGCASH_UPDATE_RESP = 0x878046,//补偿玩家G币的响应
            SDO_MEMBERLOCAL_BANISHMENT = 0x870047,//本地保存停封信息
            SDO_MEMBERLOCAL_BANISHMENT_RESP = 0x878047,//本地保存停封信息响应
            SDO_EMAIL_QUERY = 0x870048,//得到玩家的EMAIL
            SDO_EMAIL_QUERY_RESP = 0x878048,//得到玩家的EMAIL响应
            SDO_USERCHARAGESUM_QUERY = 0x870049,//得到充值记录总和
            SDO_USERCHARAGESUM_QUERY_RESP = 0x878049,//得到充值记录总和响应
            SDO_USERCONSUMESUM_QUERY = 0x870050,//得到消费记录总和
            SDO_USERCONSUMESUM_QUERY_RESP = 0x878050,//得到消费记录总和响应
            SDO_USERGCASH_QUERY = 0x870051,//玩家Ｇ币记录查询
            SDO_USERGCASH_QUERY_RESP = 0x878051,//玩家Ｇ币记录查询响应

			SDO_CHALLENGE_QUERY = 0x870052,
            SDO_CHALLENGE_QUERY_RESP = 0x878052,
            SDO_CHALLENGE_INSERT = 0x870053,
            SDO_CHALLENGE_INSERT_RESP = 0x878053,
            SDO_CHALLENGE_UPDATE = 0x870054,
            SDO_CHALLENGE_UPDATE_RESP = 0x878054,
            SDO_CHALLENGE_DELETE = 0x870055,
            SDO_CHALLENGE_DELETE_RESP = 0x878055,
            SDO_MUSICDATA_QUERY = 0x870056,
            SDO_MUSICDATA_QUERY_RESP = 0x878056,
            SDO_MUSICDATA_OWN_QUERY = 0x870057,
            SDO_MUSICDATA_OWN_QUERY_RESP = 0x878057,
            SDO_CHALLENGE_SCENE_QUERY = 0x870058,
            SDO_CHALLENGE_SCENE_QUERY_RESP = 0x878058,
            SDO_CHALLENGE_SCENE_CREATE = 0x870059,
            SDO_CHALLENGE_SCENE_CREATE_RESP = 0x878059,
            SDO_CHALLENGE_SCENE_UPDATE = 0x870060,
            SDO_CHALLENGE_SCENE_UPDATE_RESP = 0x878060,
            SDO_CHALLENGE_SCENE_DELETE = 0x870061,
            SDO_CHALLENGE_SCENE_DELETE_RESP = 0x878061,
            SDO_MEDALITEM_CREATE = 0x870062,
            SDO_MEDALITEM_CREATE_RESP = 0x878062,
            SDO_MEDALITEM_UPDATE = 0x870063,
            SDO_MEDALITEM_UPDATE_RESP = 0x878063,
            SDO_MEDALITEM_DELETE = 0x870064,
            SDO_MEDALITEM_DELETE_RESP = 0x878064,
            SDO_MEDALITEM_QUERY = 0x870065,
            SDO_MEDALITEM_QUERY_RESP = 0x878065,
            SDO_MEDALITEM_OWNER_QUERY = 0x870066,
            SDO_MEDALITEM_OWNER_QUERY_RESP = 0x878066,
            SDO_MEMBERDANCE_OPEN = 0x870067,
            SDO_MEMBERDANCE_OPEN_RESP = 0x878067,
            SDO_MEMBERDANCE_CLOSE = 0x870068,
            SDO_MEMBERDANCE_CLOSE_RESP = 0x878068,

            SDO_USERNICK_UPDATE = 0x870069,
            SDO_USERNICK_UPDATE_RESP = 0x878069,
            SDO_PADKEYWORD_QUERY = 0x870070,
            SDO_PADKEYWORD_QUERY_RESP = 0x878070,
            SDO_BOARDMESSAGE_REQ = 0x870071,
            SDO_BOARDMESSAGE_REQ_RESP = 0x878071,
            SDO_CHANNELLIST_QUERY = 0x870072,
            SDO_CHANNELLIST_QUERY_RESP = 0x878072,
            SDO_ALIVE_REQ = 0x870073,
            SDO_ALIVE_REQ_RESP = 0x878073,
            SDO_BOARDTASK_QUERY = 0x870074,
            SDO_BOARDTASK_QUERY_RESP = 0x878074,
            SDO_BOARDTASK_UPDATE = 0x870075,
            SDO_BOARDTASK_UPDATE_RESP = 0x878075,
            SDO_BOARDTASK_INSERT = 0x870076,
            SDO_BOARDTASK_INSERT_RESP = 0x878076,
            SDO_DAYSLIMIT_QUERY = 0x870077,
            SDO_DAYSLIMIT_QUERY_RESP = 0x878077,
            SDO_USERLOGIN_DEL = 0x870078,
            SDO_USERLOGIN_DEL_RESP = 0x878078,
            SDO_GATEWAY_QUERY = 0x870079,
            SDO_GATEWAY_QUERY_RESP = 0x878079,
            SDO_USERLOGIN_CLEAR = 0x870080,
            SDO_USERLOGIN_CLEAR_RESP = 0x878080,
            SDO_CHALLENGE_INSERTALL = 0x870081,
            SDO_CHALLENGE_INSERTALL_RESP = 0x878081,
            SDO_USERPASSWD_QUERY = 0x870082,
            SDO_USERPASSWD_QUERY_RESP = 0x878082,
            SDO_USERONLINETIME_QUERY = 0x870083,
            SDO_USERONLINETIME_QUERY_RESP = 0x878083,

			SDO_YYHAPPYITEM_QUERY = 0x870084,
            SDO_YYHAPPYITEM_QUERY_RESP = 0x878084,
            SDO_YYHAPPYITEM_CREATE = 0x870085,
            SDO_YYHAPPYITEM_CREATE_RESP = 0x878085,
            SDO_YYHAPPYITEM_UPDATE = 0x870086,
            SDO_YYHAPPYITEM_UPDATE_RESP = 0x878086,
            SDO_YYHAPPYITEM_DELETE = 0x870087,
            SDO_YYHAPPYITEM_DELETE_RESP = 0x878087,

            SDO_USER_PUNISH = 0x870088,
            SDO_USER_PUNISH_RESP = 0x878088,
            SDO_PUNISHUSER_IMPORT_QUERY = 0x870089,
            SDO_PUNISHUSER_IMPORT_QUERY_RESP = 0x878089,
            SDO_PUNISHUSER_IMPORT = 0x870090,
            SDO_PUNISHUSER_IMPORT_RESP = 0x878090,
            SDO_USER_PUNISHALL = 0x870091,
            SDO_USER_PUNISHALL_RESP = 0x878091,
            SDO_FRAGMENT_QUERY = 0x870092,
            SDO_FRAGMENT_QUERY_RESP = 0x878092,
            SDO_FRAGMENT_UPDATE = 0x870093,
            SDO_FRAGMENT_UPDATE_RESP = 0x878093,
            SDO_PFUNCTIONITEM_QUERY = 0x870094,
            SDO_PFUNCTIONITEM_QUERY_RESP = 0x878094,
            SDO_USERMARRIAGE_QUERY = 0x870095,
            SDO_USERMARRIAGEQUERY_RESP = 0x878095,
            SDO_PUNISH_UPDATE_QUERY = 0x870096,
            SDO_PUNISH_UPDATE_QUERY_RESP = 0x878096,
            SDO_REMAINCASH_QUERY = 0x870097,
            SDO_REMAINCASH_QUERY_RESP = 0x878097,
            SDO_PADREGIST_QUERY = 0x870098,
            SDO_PADREGIST_QUERY_RESP = 0x878098,
            SDO_MiniDancerTime_QUERY = 0x870099,
            SDO_MiniDancerTime_QUERY_RESP = 0x878099,

            SDO_RewardItem_QUERY = 0x870100,
            SDO_RewardItem_QUERY_RESP = 0x878100,
            SDO_QueryPunishusertimes_QUERY = 0x870101,
            SDO_QueryPunishusertimes_QUERY_RESP = 0x878101,

			SDO_QueryDeleteItem_QUERY = 0x870102,
            SDO_QueryDeleteItem_QUERY_RESP = 0x878102,
            SDO_NotReachMoney_Query = 0x870103,
            SDO_NotReachMoney_Query_RESP = 0x878103,

            SDO_DoubleMoneyExp_Insert = 0x870104,
            SDO_DoubleMoneyExp_Insert_Resp = 0x878104,
            SDO_DoubleMoneyExp_Query = 0x870105,
            SDO_DoubleMoneyExp_Query_RESP = 0x878105,
            SDO_DoubleMoneyExp_Del = 0x870106,
            SDO_DoubleMoneyExp_Del_RESP = 0x878106,
            SDO_DoubleMoneyExp_Update = 0x870107,
            SDO_DoubleMoneyExp_Update_RESP = 0x878107,
            SDO_UserCheat_Query = 0x870108,
            SDO_UserCheat_Query_RESP = 0x878108,
            SDO_UserCheat_Del = 0x870109,
            SDO_UserCheat_Del_RESP = 0x878109,
            SDO_PetInfo_Query = 0x870110,
            SDO_PetInfo_Query_RESP = 0x878110,

			SDO_StageAward_Query = 0x870111,
            SDO_StageAward_Query_RESP = 0x878111,
            SDO_StageAward_Create = 0x870112,
            SDO_StageAward_Create_RESP = 0x878112,
            SDO_StageAward_Delete = 0x870113,
            SDO_StageAward_Delete_RESP = 0x878113,
            SDO_StageAward_Update = 0x870114,
            SDO_StageAward_Update_RESP = 0x878114,

            SDO_PetDrop_Query = 0x870115,
            SDO_PetDrop_Query_RESP = 0x878115,
            SDO_BatchItemShop_Insert = 0x870116,
            SDO_BatchItemShop_Insert_RESP = 0x878116,
            SDO_UserBugle_Query = 0x870117,
            SDO_UserBugle_Query_RESP = 0x878117,
            SDO_UserMsg_Query = 0x870118,
            SDO_UserMsg_Query_RESP = 0x878118,
            SDO_UserActive_Query = 0x870119,
            SDO_UserActive_Query_RESP = 0x878119,
            SDO_RemoteIp_Query = 0x800019,
            SDO_RemoteIp_Query_RESP = 0x808019,

			SDO_BAOXIANGRate_Query = 0x870120,
            SDO_BAOXIANGRate_Query_RESP = 0x878120,
            SDO_BAOXIANGRate_Update = 0x870121,
            SDO_BAOXIANGRate_Update_RESP = 0x878121,

            SDO_Planet_Query = 0x870122,
            SDO_Planet_Query_RESP = 0x878122,
            SDO_Channel_Query = 0x870123,
            SDO_Channel_Query_RESP = 0x878123,
            SDO_UserOnline_Query = 0x870124,
            SDO_UserOnline_Query_RESP = 0x878124,
            SDO_Family_CONSUME_QUERY = 0x870125,//家族消费查询
            SDO_Family_CONSUME_QUERY_RESP = 0x878125,//家族消费查询
            SDO_Family_Info_QUERY = 0x870126,//家族基本信息
            SDO_Family_Info_QUERY_RESP = 0x878126,//家族基本信息
            SDO_FamilyMember_Query = 0x870127,//家族成员信息
            SDO_FamilyMember_Query_RESP = 0x878127,//家族成员信息
            SDO_FamilyFormal_Query = 0x870128,//申请中成员信息
            SDO_FamilyFormal_Query_RESP = 0x878128,//申请中成员信息
            SDO_Personal_Emblem_Query = 0x870129,//家族勋章
            SDO_Personal_Emblem_Query_RESP = 0x878129,//家族勋章
            SDO_Family_Donate_Query = 0x870130,//家族金库捐赠明细
            SDO_Family_Donate_Query_RES = 0x878130,//家族金库捐赠明细
            SDO_Family_Delete_Query = 0x870131,//解散家族
            SDO_Family_Delete_Query_RESP = 0x878131,//解散家族

			SDO_Family_Duty_Query = 0x870132,//家族成员职务修改
            SDO_Family_Duty_Query_RESP = 0x878132,//家族成员职务修改
            SDO_Family_Emblem_Insert_Query = 0x870133,//可添加家族徽章
            SDO_Family_Emblem_Insert_Query_RESP = 0x878133,//可添加家族徽章
			
            SDO_Family_Emblem_Delete_Query = 0x870134,//可删除家族徽章
            SDO_Family_Emblem_Delete_Query_RESP = 0x878134,//可删除家族徽章

			//maple add
			SDO_Pet_Update= 0x870135,//删除玩家宠物
			SDO_Pet_Update_RESP =0x878135,
			SDO_Social_Update=0x870136,//修改人物社交值
			SDO_Social_Update_Resp=0x878136,
			SDO_Lucky_Update=0x870137,//修改人物幸运值
			SDO_Lucky_Update_Resp=0x878137,


			Ban_Open_Query=0x990001,//解封账号
			Ban_Open_Query_Resp=0x998001,//解封账号回应

			Ban_Close_Query=0x990002,//封停账号
			Ban_Close_Query_Resp=0x998002,//封停账号回应

			Ban_AccountList_Query=0x990003,//账号列表查询
			Ban_AccountList_Query_Resp=0x998003,//账号列表查询回应

			Ban_Account_Query=0x990004,//账号查询
			Ban_Account_Query_Resp=0x998004,//账号查询响应

			Ban_UserInfo_Query=0x990005,//帐户信息查询
			Ban_UserInfo_Query_Resp=0x998005,//帐户信息查询响应
			
			NOTDEFINED = 0x0,
			MESSAGE = 0xEEEEE
		};  
		__Enum _value; // 枚举值  
		Message_Tag_ID(int value = 0) : _value((__Enum)value) {}  
		Message_Tag_ID& operator=(int value) 
		{       
			this->_value = (__Enum)value;
			return *this;
		}   
		operator int() 
			const 
		{        return this->_value;    }
	};
};
