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
			PAL_ADMIN = 0x62,//�ɽ�������Ϣ��
			PALLOG_ADMIN = 0x63,//�ɽ���־������Ϣ��
			GT_ADMIN = 0x51,//GTOWN��Ϣ��
			GT_OP_ADMIN = 0x52,//GTOWN��Ϣ��
			WA_ADMIN=0x53,//��ʿ��˫��Ϣ��
			CF_ADMIN=0x54,//�����ɳ���Ϣ��
			XDJG_ADMIN=0x55,//���������Ϣ��
			JW2_ADMIN=0x61,//������2
			CG2_ADMIN = 0x71,//ħ������2������Ϣ��
			SDO_ADMIN = 0x87,//�������߲�����Ϣ��
			Ban_ADMIN=0x99,//��ͣ������Ϣ��
			Message = 0xEE
		};  
		__Enum _value; // ö��ֵ  
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
			UserName = 0x0101, //Format:STRING �û���
			PassWord = 0x0102, //Format:STRING ����
			MAC = 0x0103, //Format:STRING  MAC��
			Limit = 0x0104,//Format:DateTime GM�ʺ�ʹ��ʱЧ
			User_Status = 0x0105,//Format:INT ״̬��Ϣ
			UserByID = 0x0106,//Format:INT ����ԱID
			RealName = 0x0107,//Format:STRING ������
			DepartID = 0x0108,//Format:INT ����ID
			DepartName = 0x0109,//Format:STRING ��������
			DepartRemark = 0x0110,//Format:STRING ��������
			OnlineActive = 0x0111,//Format:Integer ����״̬
			UpdateFileName = 0x0112,//Format:String �ļ���
			UpdateFileVersion = 0x0113,//Format:String �ļ��汾
			UpdateFilePath = 0x0114,//Format:String �ļ�·��
			UpdateFileSize = 0x0115,//Format:Integer �ļ���С
			Process_Reason = 0x060B,//Format tring

			userIP=0x0119,//Format::STRING IP��ַ(�¼�)
			DepartStrGameID=0x0120,//Format:STRING������ϷID�ܺ�(�¼�)
			DugID=0x0121,////Format:STRINGBBugID(�¼�)
			DugSubject=0x0122,////Format:STRINGBBug����(�¼�)
			DugType=0x0123,////Format:INTBug����(�¼�)
			DugContent=0x0124,////Format:STRINGBug����(�¼�)
			DugResult=0x0125,////Format:STRINGBug���(�¼�)

			ServerInfo_IP = 0x0901,//Format:String ������IP
			ServerInfo_City = 0x0902,//Format:String ����
			ServerInfo_GameID = 0x0903,//Format:Integer ��ϷID
			ServerInfo_GameName = 0x0904,//Format:String ��Ϸ��
			ServerInfo_GameDBID = 0x0905,//Format:Integer ��Ϸ���ݿ�����
			ServerInfo_GameFlag = 0x0906,//Format:Integer ��Ϸ������״̬
			ServerInfo_Idx = 0x0907,
			ServerInfo_DBName = 0x0908,

			ServerInfo_UserName=0x0909,////Format:STRING�������û���(�¼�)
			ServerInfo_PassWord=0x0910,////Format:STRINGB����������(�¼�)

			GameID = 0x0200, //Format:INTEGER ��ϢID
			ModuleName = 0x0201, //Format:STRING ģ������
			ModuleClass = 0x0202, //Format:STRING ģ�����
			ModuleContent = 0x0203, //Format:STRING ģ������

			//Added by tengjie 2010-05-05
			ToolProcTypeID	= 0x0204,		//���̹�������ID
			PlayerAccount	= 0x0205,		//����ʺ�
			PlayerName		= 0x0206,		//����ǳ�
			ToolProcPath	= 0x0207,		//��׽������ҽ���ȫ·��
			KickProcTime	= 0x0208,		//��������ʱ��
			PlayerInServer	= 0x0209,		//������ڴ�������
//			ServerInfo_IP	= 0x0210,		//������ڴ���ip
			GM_AccountInfo_Vip = 0x0211,		//�Ƿ�vip�û�
			WA_ClosePlayerGroup = 0x0212,    //��ͣ�ʺ�
			GM_ClosePlayerGroup_WA = 0x0213,    //��ͣ�ʺ�

			Module_ID = 0x0301, //Format:INTEGER ģ��ID
			User_ID = 0x0302, //Format:INTEGER �û�ID
			ModuleList = 0x0303, //Format:String ģ���б�
			SysAdmin = 0x0116,//Format:Integer �Ƿ���ϵͳ����Ա
			Host_Addr = 0x0401, //Format:STRING
			Host_Port = 0x0402, //Format:STRING
			Host_Pat = 0x0403,  //Format:STRING
			Conn_Time = 0x0404, //Format:DateTime �������Ӧʱ��
			Connect_Msg = 0x0405,//Format:STRING ����������Ϣ
			DisConnect_Msg = 0x0406,//Format:STRING	 ����˿���Ϣ
			Author_Msg = 0x0407, //Format:STRING ��֤�û�����Ϣ
			Status = 0x0408,//Format:STRING �������
			Index = 0x0409, //Format:Integer ��¼�����
			PageSize = 0x0410,//Format:Integer ��¼ҳ��ʾ����
			PageCount = 0x0411,//Format:Integer ��ʾ��ҳ��
			SP_Name = 0x0412,//Format:Integer �洢������
			Real_ACT = 0x0413,//Format:String ����������
			ACT_Time = 0x0414,//Format:TimeStamp ����ʱ��
			BeginTime = 0x0415,//Format:Date ��ʼ����
			EndTime = 0x0416,//Format:Date ��������
			GameName = 0x0501, //Format:STRING ��Ϸ����
			GameContent = 0x0502, //Format:STRING ��Ϣ����
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
			/// �ɽ�����
			/// </summary>
			PAL_WORLDID=0x1001,//worldID Format:Integer
			PAL_PLAYERCOUNT=0x1002,//������� Format:Integer
			PAL_ACCOUNT=0x1003,//�ʺ�Format:String
			PAL_ROLENAME=0x1004,//��ɫ��Format:String
			PAL_BAGTYPE=0x1005,//��������Format:String
			PAL_ITEMNAME=0x1006,//��Ʒ����Format:String
			PAL_ITEMPOS=0x1007,//��Ʒλ��Format:Integer
			PAL_ITEMTYPE=0x1008,//��Ʒ����Format:Integer
			PAL_ORGOBJID=0x1009,//����IDFormat:Integer
			PAL_LOCALTIME=0x1010,//����ʱ��Format:String 
			PAL_ITEMSERIAL=0x1011,//��Ʒ���Format:Integer
			PAL_LIVETIME=0x1012,//����ʱ��Format:Integer
			PAL_COOLDOWN=0x1013,//�´ο���ʹ�õ�ʱ��Format:Integer
			PAL_NPCTRADE=0x1014,// �Ƿ���Ը�NPC����(1:��, 0:����)Format:Integer
			PAL_USERTRADE=0x1015,// �Ƿ���Ը���ҽ���(1:��, 0:����)Format:Integer
			PAL_ITEMMALLGOODS=0x1016,// �Ƿ����̳���Ʒ(1:��, 0:����)Format:Integer
			PAL_ITEMCOUNT=0x1017,//��Ʒ����Format:Integer
			PAL_ITEMSELLCOST=0x1018,//��Ʒ����Format:Integer
			PAL_ITEMDURABLE=0x1019,//��ƷĿǰ�;ö�Format:Integer	
			PAL_INHERITID0=0x1020,//����ֵ(����)%d %sFormat:String 
			PAL_INHERITID1=0x1021,//��ħ%d %sFormat:String
			PAL_INHERITID2=0x1022,//������%d %sFormat:String
			PAL_INHERITID3=0x1023,//�巨��%d %sFormat:String
			PAL_INHERITID4=0x1024,//��ʯ1%d %sFormat:String
			PAL_INHERITID5=0x1025,//��ʯ2%d %sFormat:String
			PAL_INHERITID6=0x1026,//��ʯ3%d %sFormat:String
			PAL_INHERITID7=0x1027,//��ʯ4%d %sFormat:String
			PAL_INHERITID8=0x1028,//��ʯ5%d %sFormat:String
			PAL_INHERITID9=0x1029,//��ʯ6%d %sFormat:String
			PAL_PETEXP=0x1030,//����ֵFormat:Integer
			PAL_PETNOWHP=0x1031,//ĿǰHPֵFormat:Integer
			PAL_PETINDEX=0x1032,//�����������: %d (%s)Format:String
			PAL_PETRENEWCOUNT=0x1033,//��ͯ����Format:Integer
			PAL_PETLV=0x1034,//�ȼ�Format:Integer
			PAL_PETGENERATION=0x1035,//����Format:Integer
			PAL_PETMIXCOUNT=0x1036,//�ϳɴ���Format:Integer
			PAL_PETGROWTHRATE=0x1037,//�ɳ���Format:Integer
			PAL_PETATTRQUALITY=0x1038,//Ʒ��Format:Integer
			PAL_PETHP=0x1039,//HP�ӳ�ֵFormat:Integer
			PAL_PETWUSHU=0x1040,//����Format:Integer
			PAL_PETSPIRIT=0x1041,//����Format:Integer
			PAL_PETCONSTITUTION=0x1042,//����Format:Integer
			PAL_PETQUALITY=0x1043,//����Format:Integer
			PAL_PETDEXTERITY=0x1044,//��Format:Integer
			PAL_PETFATE=0x1045,//����Format:Integer
			PAL_PETHPPLUS=0x1046,//HPֵ�۸�Format:Integer
			PAL_PETWUSHUPLUS=0x1047,//�����۸�Format:Integer
			PAL_PETSPIRITPLUS=0x1048,//���Ի۸�Format:Integer
			PAL_PETCONSTITUTIONPLUS=0x1049,//�����۸�Format:Integer
			PAL_PETQUALITYPLUS=0x1050,//�����۸�Format:Integer
			PAL_PETDEXTERITYPLUS=0x1051,//���۸�Format:Integer
			PAL_PETFATEPLUS=0x1052,//���˻۸�Format:Integer
			PAL_PETSKILL0=0x1053,//����1Format:Integer
			PAL_PETSKILL1=0x1054,//����2Format:Integer
			PAL_PETSKILL2=0x1055,//����3Format:Integer
			PAL_PETSKILL3=0x1056,//����4Format:Integer
			PAL_PETSKILL4=0x1057,//����5Format:Integer
			PAL_PETSKILL5=0x1058,//����6Format:Integer
			PAL_FRIENDNAME=0x1059,//���ѽ�ɫ����Format:String
			PAL_RELATION=0x1060,//��ϵFormat:String
			PAL_NICKNAME=0x1061,//�ǳ�Format:String
			PAL_GUILDNAME=0x1062,//��������Format:String
			PAL_GUILDPOWERLEVEL=0x1063,//����ְ��Format:Integer
			PAL_VOC=0x1064,//ְҵFormat:String
			PAL_LEVEL=0x1065,//�ȼ�Format:Integer
			PAL_EXP=0x1066,//Ŀǰ����Format:Integer
			PAL_BODYMONEY=0x1067,//���Ͻ�ǮFormat:Integer
			PAL_BANKMONEY=0x1068,//�ֿ��ǮFormat:Integer
			PAL_SEX=0x1069,//�Ա�Format:String
			PAL_FAME=0x1070,//����Format:Integer
			PAL_HP=0x1071,//ĿǰѪ��Format:Integer
			PAL_MP=0x1072,//Ŀǰ����Format:Integer
			PAL_AP=0x1073,//Ŀǰ����Format:Integer
			PAL_EP=0x1074,//Ŀǰ����Format:Integer
			PAL_SKILLPOINT=0x1075,//�����ü��ܵ���Format:Integer
			PAL_SKILLTREENODE=0x1076,//Ŀǰ�������ߵ���λ��Format:Integer
			PAL_REINCARNATION=0x1077,//ת������Format:Integer
			PAL_DBID=0x1078,//���ݿ�IDFormat:Integer
			PAL_ZONEID=0x1079,//����IDFormat:Integer
			PAL_ZONENAME=0x1080,//��������Format:String
			PAL_LOGINTIME=0x1081,//����ʱ��Format:String
			PAL_LOGINIP=0x1082,//����IPFormat:String
			PAL_QUESTTYPE=0x1083,//��������Format:Integer(1:����� 2:�ѽ�)
			PAL_QUESTNAME=0x1084,//��������Format:String
			PAL_QUESTID=0x1085,//������Format:Integer
			PAL_BULLETINID=0x1086,//����IDFormat:Integer
			PAL_GUILDLEVEL=0x1087,//����ȼ�Format:Integer
			PAL_GUILDSCORE=0x1088,//�������Format:Integer
			PAL_GUILDMONEY=0x1089,//�����ǮFormat:Integer
			PAL_GUILDJOINTIME=0x1090,//���빫��ʱ��Format:String
			PAL_NEWGUILDNAME=0x1091,//�¹�������Format:String
			PAL_BULLETINMONTH=0x1092,//�����·�Format:Integer
			PAL_BULLETINDAY=0x1093,//������Format:Integer
			PAL_BULLETINDAYOFWEEK=0x1094,//����������Format:Integer
			PAL_BULLETINHOUR=0x1095,//����СʱFormat:Integer
			PAL_BULLETINMINUTE=0x1096,//�������Format:Integer
			PAL_BULLETINCONTENT=0x1097,//��������Format:String
			PAL_MAILSUBJECT=0x1098,//�ʼ���ּFormat:String
			PAL_MAILCONTENT=0x1099,//�ʼ�����Format:String
			PAL_MAILMONEY=0x1100,//�ʼ���ǮFormat:Integer
			PAL_MAILREASON=0x1101,// ���ͽ�Ʒԭ��Format:String
			PAL_MAILGIFTDBID0=0x1102,// ��Ʒ���1Format:Integer
			PAL_MAILGIFTDBID1=0x1103,// ��Ʒ���2Format:Integer
			PAL_MAILGIFTDBID2=0x1104,// ��Ʒ���3Format:Integer
			PAL_MAILGIFTDBID3=0x1105,// ��Ʒ���4Format:Integer
			PAL_MAILGIFTDBID4=0x1106,// ��Ʒ���5Format:Integer
			PAL_DELAYTIME=0x1107,	// �ӳٷ���ʱ��(��),0������������Format:Integer
			PAL_REASON=0x1108,//���ͣԭ��Format:String

			PAL_BOARDMESSAGE=0x1109,//��������Format:String
			PAL_ENDTIME=0x1110,//����ʱ��Format:String
			PAL_BEGINTIME=0x1111,//��ʼʱ��Format:String
			PAL_INTERVAL=0x1112,// ���ʱ��Format:Integer
			PAL_TASKID=0x1113,//�����Format:Integer
			PAL_STATUS=0x1114,//״̬Format:Integer

			PAL_GUILDNOTICE=0x1115,//���ṫ��Format:String

			PAL_BANKPW=0x1116,//����������(6λӢ����)Format:String
			PAL_DROPROLEPW=0x1117,//��ɾ������(6~12Ӣ����)Format:String

			PAL_MONEYTYPE=0x1118,//����(0:���� 1:����)
			PAL_MONEYCOUNT=0x1119,// ���ӽ��(��Ϊ������������)
			PAL_ACCOUNTID=0x1120,//�ʺ�IDFormat:Integer
			PAL_LOGOUTTIME=0x1121,//�ǳ�ʱ��Format:String
			PAL_ROLEID=0x1122,//��ɫIDFormat:Integer

			PAL_EVENTTYPE=0x1123,//�¼�����Format:Integer
			PAL_EVENTNAME=0x1124,//�¼�����Format:String
			PAL_FROMID=0x1125,//��ԴIDFormat:Integer
			PAL_FROMNAME=0x1126,//��Դ����Format:String

			PAL_OFFICERDBID=0x1127,//�����߽�ɫ���Format:Integer
			PAL_OFFICERNAME=0x1128,//�����߽�ɫ����Format:String
			PAL_OFFICERZONEID=0x1129,//������������Format:Integer
			PAL_OFFICERZONENAME=0x1130,//��������������Format:String


			PAL_LogBigTypeID=0x1131,//��־����ID Format:Integer
			PAL_LogBigType=0x1132,//��־���� Format:String
			PAL_LogSmallTypeID=0x1133,//��־С��ID Format:Integer
			PAL_LogSmallType=0x1134,//��־С�� Format:String

			PAL_X=0x1135,
			PAL_Y=0x1136,
			PAL_Z=0x1137,
			PAL_TARGETID=0x1138,
			PAL_TARGETNAME=0x1139,
			PAL_TIME=0x1140,//��־��¼ʱ��

			PAL_INHERITID0_NAME=0x1141,//����ֵ(����)%d %sFormat:String 
			PAL_INHERITID1_NAME=0x1142,//��ħ%d %sFormat:String
			PAL_INHERITID2_NAME=0x1143,//������%d %sFormat:String
			PAL_INHERITID3_NAME=0x1144,//�巨��%d %sFormat:String
			PAL_INHERITID4_NAME=0x1145,//��ʯ1%d %sFormat:String
			PAL_INHERITID5_NAME=0x1146,//��ʯ2%d %sFormat:String
			PAL_INHERITID6_NAME=0x1147,//��ʯ3%d %sFormat:String
			PAL_INHERITID7_NAME=0x1148,//��ʯ4%d %sFormat:String
			PAL_INHERITID8_NAME=0x1149,//��ʯ5%d %sFormat:String
			PAL_INHERITID9_NAME=0x1150,//��ʯ6%d %sFormat:String

			PAL_ITEMFLAG=0x1151,//�������
			PAL_BOSSID=0x1152,
			PAL_BOSSNAME=0x1153,



			PAL_AWARDMONEY=0x1154,//������Ǯ
			PAL_AWARDEXP=0x1155,//��������ֵ
			PAL_AWARDTITLEID=0x1156,//�����ƺű��
			PAL_AWARDTITLENAME=0x1157,//�����ƺ�����
			PAL_FAMEID1=0x1158,//��������1���
			PAL_FAMENAME1=0x1159,//��������1����
			PAL_FAMEVALUE1=0x1160,//��������1ֵ
			PAL_FAMEID2=0x1161,//��������2���
			PAL_FAMENAME2=0x1162,//��������2����
			PAL_FAMEVALUE2=0x1163,//��������2ֵ

			PAL_ITEMID=0x1164,//��ƷID

			PAL_SKILLID=0x1165,
			PAL_MAGICID=0x1166,
			PAL_MAGICNAME=0x1167,

			PAL_PETMIXCOUNTREMAIN=0x1168,
			PAL_PETNOTE=0x1169,

			PAL_ACCOUNTPW=0x1170,//�ʺ�����

			PAL_MALLTYPE=0x1171,//�̳���ݣ���ȫ���������򣬣�����



			PAL_ITEMBIGTYPE=0x1172,//�������
			PAL_ITEMBIGTYPEID=0x1173,//�������ID 

			PAL_BANTIME=0x1174,//��ͣʱ��

			PAL_POINT=0x1175,//ʣ��Ԫ������
		
			PAL_ITEMBUYCOST=0x1176,//��Ʒ���Format:Integer

			PAL_EXPIREDFLAG=0x1177,//���ڱ�־
			PAL_EXPIREDDATE=0x1178,//����ʱ��
			PAL_SELLSTATUS=0x1179,//����״̬
			PAL_ITEMLEVEL=0x1180,//��Ʒ�ȼ�
			PAL_TIMETYPE=0x1181,//ʱ������
			//PAL_ITEMTYPE=0x1182,//��Ʒ����
			PAL_ITEMSUBTYPE=0x1183,//��ƷС��
			PAL_ITEMVOC=0x1184,//���߿�װ��/ʹ��ְҵ
			PAL_ITEMEQPOS=0x1185,//����װ��λ��

			PAL_GUILDID=0x1186,//���ID
			PAL_MONEY=0x1187,//��Ǯ

			PAL_IDEX=0x1188,//���к�
			PAL_REWARDID=0x1189,//������ID
			PAL_GETTIME=0x1190,//��ȡʱ��
			PAL_DESCRIBER=0x1191,//����
			//PAL_BEGINTIME=0x1192,//��ʼʱ��
			//PAL_EXPIRETIME=0x1193,//����ʱ��
			PAL_ISGOT=0x1194,//�Ƿ���ȡ
			PAL_GROUPID=0x1195,//���
			PAL_9YOUID=0x1196,//9youid
			PAL_WASTEPOINT=0x1197,//����Ԫ��
			PAL_LOGTYPE=0x1198,//��־����
			PAL_CREATETIME=0x1199,//����ʱ��
			PAL_USERMEMO=0x1200,//�û���Ϊ
			PAL_SYSMEMO=0x1201,//�¼���־

			PAL_SERVERNAME=0x1202,//��ȡ����
			PAL_ACTORNAME=0x1203,//��ȡ��ɫ��
			PAL_ACTORLV=0x1204,//��ȡ��ɫ�ȼ�

			PAL_CENDSELL=0x1205,//��Ǯ�����۸�
			PAL_CENTBUY=0x1206,//��Ǯ����۸�

			PAL_UserMailBox = 0x1207,//�������
			PAL_PETNAME=0x1208,//��������
			PAL_SECONDPW=0x1209,//��������

			PAL_GMSERVERIP=0x1210,//GMServerIPFormat:String
			PAL_GMSERVERIPNAME=0x1211,//GMServerIPNameFormat:String(ͨ��������ȷ��worldid)

			PAL_MAILID=0x1212,//�ʼ�ID
			PAL_Email=0x1213,//�ʼ���ַ
			
			PAL_MurdererCount=0x1214,//а��ֵ
			PAL_EvilCount=0x1215,//��ҵֵ
			PAL_Wushu=0x1216,//����
			PAL_Constitution=0x1217,//����
			PAL_Spirit=0x1218,//����
			PAL_Quality=0x1219,//����
			PAL_Dexterity=0x1220,//��
			PAL_Fate=0x1221,//����

			PAL_TitleName0=0x1222,//�ƺ�0
			PAL_TitleName1=0x1223,//�ƺ�1
			PAL_TitleName2=0x1224,//�ƺ�2
			PAL_TitleName3=0x1225,//�ƺ�3
			PAL_TitleName4=0x1226,//�ƺ�4
			PAL_TitleName5=0x1227,//�ƺ�5
			PAL_TitleName6=0x1228,//�ƺ�6
			PAL_TitleName7=0x1229,//�ƺ�7
			PAL_TitleName8=0x1230,//�ƺ�8
			PAL_TitleName9=0x1231,//�ƺ�9
			PAL_TitleName10=0x1232,//�ƺ�10
			PAL_TitleName11=0x1233,//�ƺ�11
			PAL_TitleName12=0x1234,//�ƺ�12
			PAL_TitleName13=0x1235,//�ƺ�13
			PAL_TitleName14=0x1236,//�ƺ�14


			PAL_BlackList0=0x1237,//������0
			PAL_BlackList1=0x1238,//������1
			PAL_BlackList2=0x1239,//������2
			PAL_BlackList3=0x1240,//������3
			PAL_BlackList4=0x1241,//������4
			PAL_BlackList5=0x1242,//������5
			PAL_BlackList6=0x1243,//������6
			PAL_BlackList7=0x1244,//������7
			PAL_BlackList8=0x1245,//������8
			PAL_BlackList9=0x1246,//������9
			PAL_BlackList10=0x1247,//������10
			PAL_BlackList11=0x1248,//������11
			PAL_BlackList12=0x1249,//������12
			PAL_BlackList13=0x1250,//������13
			PAL_BlackList14=0x1251,//������14

			PAL_PlotName0=0x1252,//�Ի�����ֵ(ͣ��)
			PAL_PlotName1=0x1253,//�Ų�����ֵ(ͣ��)
			PAL_PlotName2=0x1254,//��������
			PAL_PlotName3=0x1255,//�̼�����ֵ(ͣ��)
			PAL_PlotName4=0x1256,//ħԵ�ۼ�ֵ
			PAL_PlotName5=0x1257,//ħԵ����ֵ
			PAL_PlotName6=0x1258,//װ��֪ʶ
			PAL_PlotName7=0x1259,//����֪ʶ
			PAL_PlotName8=0x1260,//��ħ������
			PAL_PlotName9=0x1261,//����ʹ��������
			PAL_PlotName10=0x1262,//����ʹ��������
			PAL_PlotName11=0x1263,//��װ������
			PAL_PlotName12=0x1264,//��Ե�ۼ�ֵ
			PAL_PlotName13=0x1265,//��Ե����ֵ
			PAL_PlotName14=0x1266,//��Ե���ջ��
			PAL_PlotName15=0x1267,//��Ե������������
			PAL_PlotName16=0x1268,//��Ե�ۼ�ֵ
			PAL_PlotName17=0x1269,//��Ե����ֵ
			PAL_PlotName18=0x1270,//ħԵ���ջ��
			PAL_PlotName19=0x1271,//ħԵ������������
			PAL_SKILLNAME=0x1272,//����
			PAL_MobName=0x1273,//��ħ����
			PAL_MobID=0x1274,//��ħID
			
			PAL_PuzzleMap=0x1275,//ͼ����
			PAL_PuzzleMapID1=0x1276,//ͼ��ID1
			PAL_PuzzleMapID2=0x1277,//ͼ��ID2
			PAL_PuzzleMapID3=0x1278,//ͼ��ID3
			PAL_PuzzleMapID4=0x1279,//ͼ��ID4
			PAL_PuzzleMapID5=0x1280,//ͼ��ID5
			PAL_PuzzleMapID6=0x1281,//ͼ��ID6
			PAL_PuzzleMapID7=0x1282,//ͼ��ID7
			PAL_PuzzleMapID8=0x1283,//ͼ��ID8
			PAL_PuzzleMapID9=0x1284,//ͼ��ID9
			PAL_MapID=0x1285,//��ͼID
			PAL_Map=0x1286,//��ͼ��	

			PAL_CustomIterID=0x1287,//������ƷID
			PAL_CustomIter=0x1288,//������Ʒ��
			PAL_BindSetup=0x1289,//��״̬(1:��Ϊ�� 0:ά��ԭ�����趨)

			//GTown
			GT_ServerIP = 0x2001,   //������IP
			GT_ServerName = 0x2002, //����������
			GT_UserID = 0x2003,     //���ID
			GT_Account = 0x2004,    //����ʺ�
			GT_UserNick = 0x2005,   //����ǳ�

			GT_StartTime = 0x2006,    //��ʼʱ��
			GT_EndTime = 0x2007,   //����ʱ��

			GT_Sex = 0x2008,//�Ա�
			GT_CurPosition = 0x2009,//��ǰλ��
			GT_GCash = 0x2010,//G��
			GT_MCash = 0x2011,//M��
			GT_TotalPoint = 0x2012,//��������
			GT_IsOnline = 0x2013,//�Ƿ�����

			GT_Physical = 0x2014,//����ֵ
			GT_Smart = 0x2015,//����ֵ
			GT_Financial = 0x2016,//����ֵ
			GT_Charm = 0x2017,//����ֵ
			GT_Title = 0x2018,//�ƺ�
			GT_HouseNum = 0x2019,//��������
			GT_InFamily = 0x2020,//���ڼ���
			GT_FriendNum = 0x2021,//��������
			GT_HaveInamorato = 0x2022,//�Ƿ�������
			GT_IsMarried = 0x2023,//�Ƿ���

			GT_IsShaikh = 0x2024,//�Ƿ��峤
			GT_InamoratoNick = 0x2025,//�����ǳ�
			GT_LoverNick = 0x2026,//�Է���ɫ�ǳ�

			GT_Interval = 0x2027,//ʱ����
			GT_IsImmediate = 0x2028,//�Ƿ���������
			GT_BoardMessage = 0x2029,//��������

			GT_BanReason = 0x2030,//��ͣԭ��
			GT_UnbindReason = 0x2031,//���ԭ��

			GT_NoticeID = 0x2032,//����ID
			GT_UserState = 0x2033,//�û�״̬
			GT_FamilyID = 0x2034,//����ID

			GT_Type = 0x2035,//����
			GT_ZoneID = 0x2036,//����ID			
			GT_MID = 0x2037,//��ƷID
			GT_TimesLimit = 0x2038,//ʹ�ô���
			GT_DaysLimit = 0x2039,//ʹ������

			GT_RoomName = 0x2040,//��������
			GT_RoomPos = 0x2041,//��������		
			GT_decorationName = 0x2042,//������װ��Ʒ����
			GT_RoomLimit = 0x2043,//����ʱ��

			GT_FamilyName = 0x2044,//��������
			GT_FamilyCreate = 0x2045,//����ʱ��
			GT_MemberNum = 0x2046,//��Ա����	
			GT_FamilyLeader = 0x2047,//�峤����
			GT_FamilyLeaderID = 0x2048,//�峤ID
			GT_MemberName = 0x2049,//��Ա����
			GT_JoinTime = 0x2050,//����ʱ��

			GT_PetName = 0x2051,//��������
			GT_PetPos = 0x2052,//��ǰλ��
			GT_PetLimit = 0x2053,//����ʱ��

			GT_ItemID = 0x2054,//����ID
			GT_ItemName = 0x2055,//��������
			GT_IsBlueIdent = 0x2056,//�Ƿ�����
			GT_Limit = 0x2057,//����ʱ��

			GT_FriendName = 0x2058,//��������
			GT_BlackFName = 0x2059,//��������
			GT_DarkFName = 0x2060,//��������

			GT_OpenTime = 0x2061,//����ʱ��
			GT_GetReward = 0x2062,//��ý���

			GT_FamilyDuty= 0x2063,//����ְ��
			GT_BanTime= 0x2064,//��ͣʱ��

			GT_SenderNick = 0x2065,//���ͷ��ǳ�
			GT_ReceiverNick = 0x2066,//���շ��ǳ�			
			GT_TradeTime= 0x2067,//����ʱ��
			GT_TradeItemID= 0x2068,//������ƷID
			GT_TradeItemName= 0x2069,//��Ʒ����
			GT_TradeItemNum= 0x2070,//��Ʒ����

			GT_MailTime= 0x2071,//�����ʼ�ʱ��
			GT_MailContent= 0x2072,//�ʼ�����

			GT_TaskName= 0x2073,//��������
			GT_TaskTime= 0x2074,//���ʱ��
			GT_TaskReward= 0x2075,//������ɽ���1

			GT_UseItemName= 0x2076,//ʹ�õ�������
			GT_UseItemNum= 0x2077,//ʹ�õ�������
			GT_UseItemTime= 0x2078,//ʹ�õ���ʱ��

			GT_MailTitle= 0x2079,//�ʼ�����

			GT_OnlineTime = 0x2080,//����ʱ��
			GT_RewardItemName = 0x2081,//��ý�����������
			GT_RewardGetTime = 0x2082,//���ʱ��

			GT_SenderAccount = 0x2083,//�����ʺ�
			GT_SendTime = 0x2084,//����ʱ��
			GT_SendContent = 0x2085,//��������

			GT_LoginTime = 0x2086,//��¼ʱ��
			GT_LogoutTime = 0x2087,//�ǳ�ʱ��
			GT_UserIP = 0x2088,//���IP

			GT_SendNick = 0x2089,//�����ǳ�

			GT_TreasureBox_Type = 0x2090,//��������

			GT_TaskReward2= 0x2091,//������ɽ���2
			GT_ChooseReward= 0x2092,//�����ѡ����

			GT_BuyTime = 0x2093,//����ʱ��
			GT_TimeLimit = 0x2094,//ʹ������

			GT_ChargeTime = 0x2095,//��ֵ����ʱ��
			GT_ChargeMoney = 0x2096,//��ֵ���˽��

			GT_Money=0x2097,//��Ǯ
			GT_Resume=0x2098,//����ǩ��

			GT_Card=0x2099,//��Ƭ��

			GT_Male=0x2100,//�з���
			GT_Female=0x2101,//Ů����
			GT_MarryPeople=0x2102,//��������
			GT_Pktimes=0x2103,//Pk��
			GT_Win=0x2104,//ʤ����
			GT_Cowin=0x2105,//ʧ����
			GT_Times=0x2106,//����

			GT_Lustful=0x2107,//���ն�
			GT_Fanta=0x2108,//���˶�
			GT_Allexp=0x2109,//�ۺ�ָ��


			GT_SlaveCount=0x2110,//ū����
			GT_SlaveSum=0x2111,//���ʲ�
			GT_Coin=0x2112,//�ֽ�
			GT_SlaveID=0x2113,//ū��ID
			GT_SlaveAccount=0x2114,//ū������
			GT_SlaveNick=0x2115,//ū���ǳ�
			GT_Price=0x2116,//���
			GT_IsSell=0x2117,//�Ƿ�����
			GT_DateRecord=0x2118,//����ʱ��
			GT_Discount=0x2110,//������
			GT_X=0x2120,
			GT_Y=0x2121,
			GT_Z=0x2122,

			GT_JW2Name=0x2123,//��ӦJW2������
			GT_ID=0x2124,//���




			/// <summary>
			/// ������2����
			/// </summary>
			JW2_ACCOUNT = 0x3001,//����ʺ� Format:String
			JW2_UserNick = 0x3002,//����س� Format:String
			JW2_ServerName=0x3003,//����������
			JW2_ServerIP = 0x3004,//��Ϸ������ Format:String
			JW2_State = 0x3005,//���״̬ Format:Integer
			JW2_Reason = 0x3006,//��ͣ���� Format:String
			JW2_BanDate = 0x3007,//��ͣ���� Format:dataTime
			JW2_UserSN = 0x3008, //Format:Integer �û����к�
			JW2_GSServerIP = 0x3009,//���� Format:String
			JW2_UserID = 0x3010, //Format:String ���ID
			JW2_Sex = 0x3011,//����Ա� Format:Integer
			JW2_AvatarItem = 0x3012, //Format:Integer
			JW2_ExpireDate = 0x3013,//Format:TimesStamp  ��������
			JW2_Exp = 0x3014, //Format:Integer ��Ҿ���
			JW2_Money = 0x3015, //Format:Integer G��
			JW2_Cash = 0x3016, //Format:Integer �ֽ�
			JW2_Level = 0x3017, //Format:Integer �ȼ�
			JW2_UseItem = 0x3018,//Format:Integer�Ƿ���ʹ���У�1��0����
			JW2_RemainCount = 0x3019,//Format:Integerʣ�������0Ϊ���޴�
			JW2_BeginTime = 0x3020,//��ʼʱ��Format:DATE
			JW2_EndTime = 0x3021,//����ʱ��Format:DATE
			JW2_BoardMessage = 0x3022,//��������,���ȷ�������Format:String
			JW2_TaskID = 0x3023,//�����Format:Integer
			JW2_Status = 0x3024,//�Ƿ����״̬Format:Integer
			JW2_Interval = 0x3025,//���ʱ��Format:Integer
			JW2_UseTime = 0x3026,//ʹ��ʱ�� TLV_DATE
			JW2_POWER=0x3027,//Ȩ�ޣ���ͨ�û���0������ԱΪ1 Format:Integer
			JW2_GoldMedal = 0x3028,//���� Format:Integer
			JW2_SilverMedal = 0x3029,//���� Format:Integer
			JW2_CopperMedal = 0x3030,//ͭ�� Format:Integer
			JW2_Region=0x3031,//���� Format:String
			JW2_QQ=0x3032,//QQ�� Format:String
			JW2_PARA=0x3033,//��������õ�һ������ Format:Integer
			JW2_ATONCE=0x3034,//�Ƿ���������Format:Integer
			JW2_BOARDSN=0x3035,//��С���ȣ������¼Ψһ��ʾFormat:Integer
			JW2_BUGLETYPE=0x3036,//����0mb��С����,1���ַ�С����,1mb��������,11���ַ�������,20��20�����Ǻ��Format:Integer
			//���///////////
			JW2_Chapter=0x3037,//�½�
			JW2_CurStage=0x3038,//Ŀǰ�ȼ�
			JW2_MaxStage=0x3039,//���ȼ�
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
			//���end///////////
			JW2_BUYSN=0x3060,//����SN
			JW2_GOODSTYPE=0x3061,//��������
			JW2_RECESN=0x3062,//�����˵�SN�������ͬ�Ǳ��ˣ�
			JW2_GOODSINDEX=0x3063,//��Ʒ���
			JW2_BUYDATE=0x3064,//��������
			JW2_RECEID=0x3065,//�����˵�ID�������ͬ�Ǳ��ˣ�
			JW2_AvatarItemName = 0x3066, //�������� Format:String

			JW2_MALESN=0x3067,//����SN
			JW2_MALEUSERNAME=0x3068,//�����û���
			JW2_MALEUSERNICK=0x3069,//�����ǳ�
			JW2_FEMAILESN=0x3070,//Ů��SN
			JW2_FEMALEUSERNAME=0x3071,//Ů���û���
			JW2_FEMAILEUSERNICK=0x3072,//Ů���ǳ�
			JW2_WEDDINGDATE=0x3073,//���ʱ��
			JW2_UNWEDDINGDATE=0x3074,//���ʱ��
			JW2_WEDDINGNAME=0x3075,//��������
			JW2_WEDDINGVOW=0x3076,//��������
			JW2_RINGLEVEL=0x3077,//��ָ�ȼ�
			JW2_REDHEARTNUM=0x3078,//��������
			JW2_WEDDINGNO=0x3079,//�������
			JW2_CONFIRMSN=0x3080,//��֤��SN
			JW2_CONFIRMUSERNAME=0x3081,//��֤���û���
			JW2_CONFIRMUSERNICK=0x3082,//��֤���ǳ�
			JW2_COUPLEDATE=0x3083,//��������
			JW2_KISSNUM=0x3084,//kiss����
			JW2_LASTKISSDATE=0x3085,//���һ��Kissʱ��
			JW2_BREAKDATE=0x3088,//����ʱ��
			JW2_CMBREAKDATE=0x3089,//ȷ�Ϸ���ʱ��
			JW2_BREAKSN=0x3090,//���SN
			JW2_BREAKUSERNAME=0x3091,//����û���
			JW2_BREAKUSERNICK=0x3092,//����ǳ�

			JW2_LASTLOGINDATE=0x3093,//����½ʱ��
			JW2_REGISTDATE=0x3094,//����ʱ��
			JW2_FCMSTATUS=0x3095,//������״̬

			JW2_FAMILYID=0x3096,//����ID
			JW2_FAMILYNAME=0x3097,//��������
			JW2_DUTYID=0x3098,//ְҵ��
			JW2_DUTYNAME=0x3099,//ְҵ��
			JW2_ATTENDTIME=0x3100,//����ʱ��
			JW2_COUPLESN=0x3101,//�������
			JW2_CREATETIME=0x3102,//����ʱ��
			JW2_CNT=0x3103,//����
			JW2_POINT=0x3104,//����
			JW2_LOGINTYPE=0x3105,//����0���룬1�ǳ�
			JW2_TIME=0x3106,//ʱ��
			JW2_IP=0x3107,//IP��ַ
			JW2_PWD=0x3108,//����
			JW2_AvatarItemType=0x3109,//��Ʒ���ͣ�ͷ���ȣ�
			JW2_ItemPos=0x3110,//��Ʒλ��(0,����,1,��Ʒ��,2,������)

			JW2_MailTitle=0x3111,//�ʼ�����
			JW2_MailContent=0x3112,//�ʼ�����
			JW2_ItemNo=0x3113,//��Ʒ���

			JW2_Repute=0x3115,//����
			JW2_NowTitle=0x3116,//�ƺ�

			JW2_FamilyLevel=0x3117,//�ȼ�
			JW2_AttendRank=0x3118,//��������
			JW2_MoneyRank=0x3119,//�Ƹ�����
			JW2_PowerRank=0x3120,//ʵ������
			
			
			JW2_ItemCode=0x3121,//����ID
			JW2_ItemName=0x3122,//��������
			JW2_Productid=0x3123,//��ƷID
			JW2_ProductName=0x3124,//��Ʒ����
			
			JW2_FamilyPoint=0x3125,//�������
			
			JW2_PetSn=0x3126,//����ID
			JW2_PetName=0x3127,//��������
			JW2_PetNick=0x3128,//�����Զ�������
			JW2_PetLevel=0x3129,//����ȼ�
			JW2_PetExp=0x3130,//���ﾭ��
			JW2_PetEvol=0x3131,//���׽׶�
			
			
			JW2_MailSn=0x3132,//���к�
			JW2_SendNick=0x3133,//�������ǳ�
			JW2_SendDate=0x3134,//��������
			
			JW2_Num=0x3135,//����
			
			JW2_Rate=0x3136,//��Ů����
			JW2_ShaikhNick=0x3137,//�峤����
			JW2_SubFamilyName=0x3138,//������������
			
			JW2_MoneyType=0x3139,//��������
			JW2_BeforeCash=0x3140,//����ǰǮ
			JW2_AfterCash=0x3141,//���Ѻ�Ǯ

			//maple add
			JW2_ProductPrice=0x3142,//���ߵ���
			JW2_TotalPrice=0x3143,//�ܶ�



			WA_ServerIP=0x4002,//������IP
			WA_Account=0x4003,//�ʺ�
			WA_UserNick=0x4004,//�ǳ�
			WA_BeginTime=0x4005,//��ʼʱ��
			WA_EndTime=0x4006,//����ʱ��
			WA_AccountID=0x4007,//�ʺ�ID
			WA_NickID=0x4008,//��ɫID
			WA_Time=0x4009,//ʱ��
			WA_RealmId=0x4010,//����ID
			WA_ClusterID=0x4011,//������ID
			WA_TitleName1=0x4012,//��ɫ��ν1
			WA_TitleName2=0x4013,//��ɫ��ν2
			WA_CharType=0x4014,//��ɫ����
			WA_Profession=0x4015,//��ɫְҵ
			WA_Sex=0x4016,//��ɫ�Ա�
			WA_IsOnline=0x4017,//�Ƿ�����
			WA_IsClosed=0x4018,//�Ƿ��ͣ
			WA_Level=0x4019,//��ɫ�ȼ� 
			WA_Money=0x4020,//��Ϸ������
			WA_NineyouMoney=0x4021,//9�α�����
			WA_ReserveMoney=0x4022,//������
			WA_CurSceneID=0x4023,//��ǰ���ڳ���ID
			WA_CurSceneName=0x4024,//��ǰ���ڳ�������
			WA_CurScenePos=0x4025,//��ǰ���ڳ�������
			WA_Hp=0x4026,//����ֵ
			WA_Mp=0x4027,//ħ��ֵ
			WA_Vitality=0x4028,//����ֵ
			WA_GoodEvil=0x4029,//�ƶ�ֵ
			WA_PhysicalAttack=0x4030,//������
			WA_PhysicalDefense=0x4031,//�������
			WA_MagicAttack=0x4032,//ħ������
			WA_MagicDefense=0x4033,//ħ������
			WA_Critical=0x4034,//������
			WA_Strength=0x4035,//����
			WA_Dexterity=0x4036,//����
			WA_Intellect=0x4037,//����
			WA_Corporeity=0x4038,//����
			WA_PotentialPoint=0x4039,//Ǳ��
			WA_Exp=0x4040,//����
			WA_ExpNextLevel=0x4041,//��������,
			WA_PhysicalResist=0x4042,//������
			WA_MagicResist=0x4043,//��������
			WA_FireResist=0x4044,//���濹��
			WA_ColdResist=0x4045,//��������
			WA_PoisonResist=0x4046,//���ؿ���
			WA_Reason=0x4047,//ԭ��
			WA_RemainTime=0x4048,//ʣ��ʱ��
			WA_TaskID=0x4049,//����ID
			WA_BoardMessage=0x4050,//��������
			WA_Interval=0x4051,//���ʱ��
			WA_ServerName=0x4052,//����������
			WA_Status=0x4053,//״̬
			WA_GSServerIP=0x4054,//GS������

			WA_ItemLocation=0x4055,//���ߴ��λ��
			WA_friendIndex=0x4056,// ���ѽ�ɫID
			WA_friendName=0x4057,// ��������
			WA_friendTypeId=0x4058,// ���ѵĽ�ɫ����
			WA_friendLevel=0x4059,// ���ѵȼ�
			WA_friendIni=0x4060,// ���ܶ�
			WA_itemIndex=0x4061,// ���
			WA_itemTypeId=0x4062,// ����ID
			WA_itemName=0x4063,// ����
			WA_itemUniqueId=0x4064,	// ΨһID
			WA_itemCount=0x4065,// ����
			WA_itemDur=0x4066,// �;�
			
			WA_medalTypeId=0x4067,	// ����ID
			WA_medalName=0x4068,	//��������
			WA_medalDur=0x4069,		// �;�
			WA_medalBringUp=0x4070,	// ����
			WA_medalPetIndex=0x4071,// ������ų���ֵ255Ϊ�޳���
			WA_medalPos=0x4072,		// �����ڳ��ﱳ����λ�� 255Ϊװ����


			WA_petTypeId=0x4073,		// ����ID
			WA_petName=0x4074,			// ��������
			WA_petLevel=0x4075,			// �ȼ�
			WA_petNature=0x4076,		// �Ը�
			WA_petGrowth=0x4077,		// �ɳ���
			WA_healthe=0x4078,			// ������

			WA_FireAttack=0x4079,		//���湥��
			WA_ColdAttack=0x4080,		//��������
			WA_PoisonAttack=0x4081,		//���ع���

			WA_Description=0x4082,		//��ͣ����

			WA_LastLoginTime=0x4083,	//�ϴε�¼ʱ��
			WA_EnableTime=0x4084,	//�ϴε�¼ʱ��

			WA_ItemPos=0x4085,	//����λ��
			WA_IsDelete=0x4086,	// �Ƿ�ɾ��

			//add by ltx
			WA_SrcCharId=0x4087, //��Դ��ɫID
			WA_SrcCharName=0x4088, //��Դ��ɫ����
			WA_DateTime=0x4089, //����ʱ��
			WA_DstCharId=0x4090, //Ŀ���ɫID
			WA_DstCharName=0x4091, //Ŀ���ɫ����

			WA_BindReward1=0x4092, //�󶨽���1
			WA_BindRewardValue1=0x4093, //�󶨽�����ֵ1
			WA_BindReward2=0x4094, //�󶨽���1
			WA_BindRewardValue2=0x4095, //�󶨽�����ֵ1

			WA_ConsumeItemTypeId=0x4096, //���������ƷID
			WA_BoothType=0x4097, //��������
			WA_ItemTimeLimit=0x4098, //��Ʒ��ʹ������

			WA_CraftType=0x4099, //װ����������
			WA_RecipeId=0x4100, //ͼֽID
			WA_MaterialId1=0x4101, //����ID
			WA_MaterialId2=0x4102, //����ID
			WA_MaterialId3=0x4103, //����ID
			WA_MaterialId4=0x4104, //����ID
			WA_MaterialId5=0x4105, //����ID
			WA_MaterialId6=0x4106, //����ID
			WA_ProductId=0x4107, //��ƷID
			WA_ProductName=0x4108, //��Ʒ����

			WA_ConsumeHammer=0x4109, //��������
			WA_ConsumeStithy=0x4110, //��������
			WA_ConsumeMoney=0x4111, //���Ľ�Ǯ

			WA_OldColor=0x4112, //�Ŀ�ǰ��ɫ
			WA_NewColor=0x4113, //�Ŀ׺���ɫ

			WA_OldStarCount=0x4114, //����ǰ����
			WA_NewStarCount=0x4115, //���Ǻ�����

			WA_GemOperation=0x4116, //��ʯ����
			WA_GemType=0x4117, //��ʯ����
			WA_GemLevel=0x4118, //��ʯ����

			WA_MaterialId=0x4119, //ԭ����ID
			WA_MaterialName=0x4120, //ԭ��������
			WA_PropItemId=0x4121, //�ɹ��ʵ���

			WA_ChangeType=0x4122, //�һ�����
			WA_ConsumeIntegral=0x4123, //���Ļ���
			WA_ChangePoint=0x4124, //�õ��ľ������Ǯ

			WA_GetMoney=0x4125, //��ý�Ǯ����
			WA_SrcId=0x4126, //��ԴID
			WA_SrcName=0x4127, //��Դ����

			WA_LoseMoney=0x4128, //ʧȥ��Ǯ����
			WA_DstId=0x4129, //Ŀ��ID
			WA_DstName=0x4130, //Ŀ������

			WA_QuestId=0x4131, //����ID
			WA_QuestName=0x4132, //��������
			WA_RewardMoney=0x4133, //�����Ľ�Ǯ
			WA_RewardItemId=0x4134, //��������Ʒ
			WA_RewardItemName=0x4135, //��������Ʒ����
			WA_GiveItemId=0x4136, //ʧȥ����ƷID
			WA_GiveItemName=0x4137, //ʧȥ����Ʒ����

			WA_MailId=0x4138, //�ʼ�ID
			WA_MailMoney=0x4139, //��Ǯ����
			
			WA_Item1=0x4140, //��Ʒ1
			WA_Item2=0x4141, //��Ʒ2
			WA_Item3=0x4142, //��Ʒ3
			WA_Item4=0x4143, //��Ʒ4
			WA_Item5=0x4144, //��Ʒ5
			WA_Item1Count=0x4145, //��Ʒ1����
			WA_Item2Count=0x4146, //��Ʒ2����
			WA_Item3Count=0x4147, //��Ʒ3����
			WA_Item4Count=0x4148, //��Ʒ4����
			WA_Item5Count=0x4149, //��Ʒ5����
			WA_MailTitle=0x4150, //�ʼ�����

			WA_OpType=0x4151, // ����
			WA_InstanceId=0x4152, //����ID
			WA_InstanceName=0x4153, //��������
			WA_USERTYPE = 0x4154,//�û�����1�������� 2������
			WA_SkillName = 0x4155,//��������
			WA_NewPasswd = 0x4156,//������
			WA_ItemID = 0x4157,//����ID
			WA_TYPE =  0x4158,//1���޸ĵȼ� 2���޸Ľ�Ǯ
			WA_NewRole = 0x4159,// �µĽ�ɫ��
			WA_SkillID = 0x4160,//����ID
			WA_SkillLvl = 0x4161,//���ܵȼ�
			WA_cluster_id = 0x4162,//��ID
			WA_realm_id = 0x4163,//����ID
			WA_ItemType = 0x4164,//�������� ����
			WA_MailorName = 0x4165,//����������
			WA_ItemPrice = 0x4166,//��Ʒ�۸�
			WA_UNBindRewardValue1  =0x4167,//���Ļ�ý���1
			WA_UNBindRewardValue2  =0x4168,//���Ļ�ý���2
			WA_GetTime  =0x4169,//���ʱ��//����ʱ��

			//Added by tengjie 2010-06-12
			WA_Content=0x4170,//��ͣ����
			WA_GuildName = 0x4171,//��������
			WA_NewProfession=0x4172,//תְ��ְҵ
			WA_ConsumeItem=0x4173,//������Ʒ
//			WA_PetTypeID=0x4174,//��������/ΨһID
			WA_NewGuildName = 0x4174,//�¹�������
			WA_GuildLevel = 0x4175,//����ȼ�
			WA_GuildMemCount = 0x4176,//��������
			WA_RecipeName=0x4177, //ͼֽ����
			WA_MaterialName1=0x4178, //��������1
			WA_MaterialName2=0x4179, //��������2
			WA_MaterialName3=0x4180, //��������3
			WA_MaterialName4=0x4181, //��������4
			WA_MaterialName5=0x4182, //��������5
			WA_MaterialName6=0x4183, //��������6
			WA_ItemNum=0x4184, //��������
			WA_LoginIP=0x4185, //��½IP
			WA_MemLevel=0x4186, //ְ��
			WA_MailTag=0x4187, //�Ƿ�Ⱥ���ʼ�
			WA_ItemGen=0x4188, //����Ʒ��
			WA_MailSubject=0x4189, //�ʼ�����
			WA_MailContent=0x4190, //�ʼ�����
			WA_SendMoney=0x4191, //���ͽ�Ǯ
			WA_ItemExpire=0x4192, //��ƷʱЧ
			WA_GuildID=0x4193, //����ID
			WA_PetUniqId=0x4194, //����ΨһID
			WA_CurMoney=0x4195,//��ǰ��Ϸ������

			/// <summary>
            /// ħ������2
            /// </summary>
            CG2_UserName = 0x5000,//�û���
            CG2_UserID = 0x5001,//��ɫIDKEY
            CG2_CharacterNum = 0x5002,//��ɫ��
            CG2_Level = 0x5003,//�ȼ�
            CG2_Exp = 0x5004,// ����ֵ
            CG2_Sex = 0x5005,// �Ա�
            CG2_Money = 0x5006,// ��Ǯ
            CG2_Profession = 0x5007,//ְҵ
            CG2_Guild = 0x5008,//��������
            CG2_CreateTime = 0x5009,//��ɫ����ʱ��
            CG2_IsOnline = 0x5010,//�Ƿ�����
            CG2_Blood = 0x5011,//Ѫֵ
            CG2_Magic = 0x5012,//ħ��ֵ
            CG2_MapID = 0x5013,//��ͼID
            CG2_Coordinate = 0x5014,//����
            CG2_Injured = 0x5015,//����ֵ
            CG2_Status = 0x5016,//����(״̬)
            CG2_ProfessionLevel = 0x5017,//ְҵ�ȼ�
            CG2_Honor = 0x5018,//����ֵ
            CG2_SpecialName = 0x5019,//�ر�ƺ�
            CG2_Charm = 0x5020,//����ֵ
            CG2_InPrison = 0x5021,//�Ƿ�ؼ���(״̬)
            CG2_CharacterName = 0x5022,//��ɫ����
            CG2_OwnName = 0x5023,//�������ƺ�
            CG2_Fight = 0x5024,//ս��
            CG2_LeftPoint = 0x5025,//ʣ�����
            CG2_Physical = 0x5026,//����
            CG2_Power = 0x5027,//����
            CG2_Smart = 0x5028,//����
            CG2_Speed = 0x5029,//�ٶ�
            CG2_Spirit = 0x5030,//����
            CG2_MaxBlood = 0x5031,//���Ѫֵ
            CG2_MaxMag = 0x5032,//���ħֵ
            CG2_Attack = 0x5033,//����
            CG2_Defend = 0x5034,//����
            CG2_MagAttack = 0x5035,//ħ��
            CG2_MagDefend = 0x5036,//ħ��
            CG2_Recover = 0x5037,//�ظ�
            CG2_Hit = 0x5038,//����
            CG2_Avoid = 0x5039,//�ر�
            CG2_Critical = 0x5040,//����
            CG2_Retort = 0x5041,//����
            CG2_isChairman = 0x5042,//�Ƿ�᳤
            CG2_ProHonorRank = 0x5043,//ְҵ�����ƺ�
            CG2_tempRank = 0x5044,//��ʱ�ƺ�
            CG2_RankID = 0x5045,//�ƺ�ID
            CG2_FriendIDKey = 0x5046,//�Է�IDKey
            CG2_FriendCharNum = 0x5047,// ��������
            CG2_FriendShip = 0x5048,//�Ѻö�
            CG2_SlotNum = 0x5049,//���ۺ�
            CG2_SkillName = 0x5050,//��������
            CG2_SkillLevel = 0x5051,//���ܵȼ�/////////maple
            CG2_SkillExp = 0x5052,//���ܾ���

            CG2_EquipNum = 0x5053,//װ����
            CG2_ItemColNum = 0x5054,//��Ʒ����
            CG2_CombineNum = 0x5055,//�ϲ�������
            CG2_ItemName = 0x5056,//��Ʒ����
            CG2_QualityLevel = 0x5057,//�����ȼ�
            CG2_CurEndure = 0x5058,//��ǰ�;�
            CG2_MaxEndure = 0x5059,//����;�
            CG2_FixTimes = 0x5060,//�������
            CG2_ComposeTimes = 0x5061,//�ϳɴ���
            CG2_ComposeItemID = 0x5062,//�ϳ�ʱȡ����Ʒ����ID
            CG2_StorageMoney = 0x5063,//�洢��Ǯ��

            CG2_Serverip = 0x5064, //IP��ַ
            CG2_nickname = 0x5065, //�ǳ�

            CG2_JobID = 0x5067, //ְҵID
            CG2_GuildID = 0x5068, //����ID
            CG2_ProHonorRankID = 0x5069,//ְҵ�����ƺ�ID

            CG2_logtime = 0x5070,//timestamp logʱ��
            CG2_category = 0x5071,//��־����
            CG2_action = 0x5072,//��־С��
            CG2_targetName = 0x5073,//Ŀ���ɫ��
            CG2_targetIDKey = 0x5074,//Ŀ��ID
            CG2_mgvalue = 0x5075,//��־��Ϣ
            CG2_mgstring = 0x5076,//��־����

            CG2_PetID = 0x5077,//����id
            CG2_PetStatus = 0x5078,//����״̬
            CG2_PetExp = 0x5079,//����
            CG2_PetBlood = 0x5080,//Ѫֵ
            CG2_PetMag = 0x5081,//ħ��ֵ
            CG2_PetInjured = 0x5082,//����ֵ
            CG2_PetLostSoul = 0x5083,//����(״̬)
            CG2_PetRace = 0x5084,//����
            CG2_PetMaxSkillCol = 0x5085,//�������
            CG2_PetSrcName = 0x5086,//����ԭ��
            CG2_PetNewName = 0x5087,//��������������
            CG2_PetFaith = 0x5088,//�ҳ϶�
            CG2_PetLeftPoint = 0x5089,//ʣ�����
            CG2_PetPhysical = 0x5090,//����
            CG2_PetPower = 0x5091,//����
            CG2_PetSmart = 0x5092,//����
            CG2_PetSpeed = 0x5093,//�ٶ�
            CG2_PetSpirit = 0x5094,//����
            CG2_PetMaxBlood = 0x5095,//���Ѫֵ
            CG2_PetMaxMag = 0x5096,//���ħֵ
            CG2_PetAttack = 0x5097,//����
            CG2_PetDefend = 0x5098,//����
            CG2_PetCheesy = 0x5099,//����
            CG2_PetMagAttack = 0x5100,//ħ��
            CG2_PetMagDefend = 0x5101,//ħ��
            CG2_PetRecover = 0x5102,//�ظ�
            CG2_PetHit = 0x5103,//����
            CG2_PetAvoid = 0x5104,//�ر�
            CG2_PetCritical = 0x5105,//����
            CG2_PetRetort = 0x5106,//����
            CG2_PetEvolveTimes = 0x5107,//��������
            CG2_PetSatisfLvl = 0x5108,//�����
            CG2_PetFullLvl = 0x5109,//��ʳ��
            CG2_PetColNum = 0x5110,//PET����
            CG2_PetItemCol = 0x5111,//ITEM��
            CG2_PetItemID = 0x5112,//ItemID
            CG2_PetItemGetTime = 0x5113,//���ʱ��
            CG2_PetCombineNum = 0x5114,//�ϲ�������
            CG2_PetQulityLvl = 0x5115,//�����ȼ�
            CG2_PetCurEndure = 0x5116,//��ǰ�;�
            CG2_PetMaxEndure = 0x5117,//����;�
            CG2_PetFixTimes = 0x5118,//�������
            CG2_PetComposeTimes = 0x5119,//�ϳɴ���
            CG2_PetComposeItemID = 0x5120,//�ϳ�ʱȡ����Ʒ����ID
            CG2_PetSkillID = 0x5121,//����ID
            CG2_PetSkillLvl = 0x5122,//���ܵȼ�
            CG2_PetSkillExp = 0x5123,//���ܾ���ֵ
            CG2_PetSkillColNum = 0x5124,//��������

            CG2_UserNum = 0x5125,//��ɫ��
            CG2_LatestLoginTime = 0x5126,//�����¼ʱ��
            CG2_LatestLogoutTime = 0x5127,//����ǳ�ʱ��
            CG2_LoginIP = 0x5128,//��¼IP
            CG2_isAdult = 0x5129,//�Ƿ����
            CG2_hasLockedItem = 0x5130,//�Ƿ�����ITEM 
            CG2_Fame = 0x5131,//����ֵ

            CG2_titleFlags1 = 0x5132, //�ƺ�1
            CG2_titleFlags2 = 0x5133, //�ƺ�2
            CG2_titleFlags3 = 0x5134, //�ƺ�3
            CG2_titleFlags4 = 0x5135, //�ƺ�4
            CG2_titleFlags5 = 0x5136, //�ƺ�5
            CG2_titleFlags6 = 0x5137, //�ƺ�6
            CG2_titleFlags7 = 0x5138, //�ƺ�7
            CG2_titleFlags8 = 0x5139, //�ƺ�8
            CG2_titleFlags9 = 0x5140, //�ƺ�9
            CG2_titleFlags10 = 0x5141, //�ƺ�10
            CG2_titleFlags11 = 0x5142, //�ƺ�11
            CG2_titleFlags12 = 0x5143, //�ƺ�12
            CG2_titleFlags13 = 0x5144, //�ƺ�13
            CG2_titleFlags14 = 0x5145, //�ƺ�14
            CG2_titleFlags15 = 0x5146, //�ƺ�15
            CG2_titleFlags16 = 0x5147, //�ƺ�16
            CG2_titleFlags17 = 0x5148, //�ƺ�17
            CG2_titleFlags18 = 0x5149, //�ƺ�18
            CG2_titleFlags19 = 0x5150, //�ƺ�19
            CG2_titleFlags20 = 0x5151, //�ƺ�20
            CG2_titleFlags21 = 0x5172, //�ƺ�21
            CG2_titleFlags22 = 0x5173, //�ƺ�22
            CG2_titleFlags23 = 0x5174, //�ƺ�23
            CG2_titleFlags24 = 0x5175, //�ƺ�24
            CG2_titleFlags25 = 0x5176, //�ƺ�25
            CG2_titleFlags26 = 0x5177, //�ƺ�26
            CG2_titleFlags27 = 0x5178, //�ƺ�27
            CG2_titleFlags28 = 0x5179, //�ƺ�28
            CG2_titleFlags29 = 0x5180, //�ƺ�29
            CG2_titleFlags30 = 0x5181, //�ƺ�30

            CG2_MapName = 0x5152,//��ͼ
            CG2_TypeName = 0x5153,//�������
            CG2_TypeID = 0x5154,//�������ID
            CG2_CharNo = 0x5155,//��ɫ��

            CG2_Title = 0x5156,//����
            CG2_Content = 0x5157,//����
            CG2_BanTime = 0x5158,//��ͣʱ��
            CG2_NoteContent = 0x5159,//��ͣ����
            CG2_Interval = 0x5160,//ʱ����
            CG2_StartTime = 0x5161,//��ʼʱ��
            CG2_EndTime = 0x5162,//����ʱ��
            CG2_NoticeID = 0x5163,//����ID
            CG2_Times = 0x5164,//��ͣ���
            CG2_Cheesy = 0x5165,//����

            CG2_GuildName = 0x5166,//��������
            CG2_GuildNum = 0x5167,//�����Ա����
            CG2_GuildHoro = 0x5168,//��������
            CG2_GuildRank = 0x5169,//����ȼ�
            CG2_Master = 0x5170,//����᳤
            CG2_MasterIdkey = 0x5171,//�᳤ID

            CG2_Ip = 0x5683,//��½IP
            CG2_SkillID = 0x5684,//���＼��id
            CG2_SkillType = 0x5685,//��������
            CG2_PetSkillName = 0x5686,//���＼����
            CG2_ItemNum = 0x5687,//�������� int

            CG2_Palette = 0x5688,//��ɫ int 
            CG2_Wind = 0x5689,//������ int 
            CG2_Frie = 0x5690,//������ int 
            CG2_Water = 0x5691,//ˮ���� int 
            CG2_Soil = 0x5692,//������ int 
            CG2_userItemName = 0x5693,//�Զ���ĵ������� string 
            CG2_Sn = 0x5694,//������ string
            CG2_ServerName = 0x5695,//�������� string
            CG2_Date = 0x5696,//����ʱ�� string
            CG2_isVIP = 0x5697,//�Ƿ�߼��ʺ� string
            CG2_isUse = 0x5698,//�Ƿ�ʹ�� string

            CG2_Email = 0x5699,//Email��ַ string

            CG2_ItemPass = 0x5700,//���������� int
            CG2_TempPwd = 0x5701,//��ʱ���� string

            CG2_SendUser = 0x5702,//�����û� string
            CG2_RecvUser = 0x5703,//�����û� string
            CG2_ItemType = 0x5704,//�������� string
            CG2_LimitTime = 0x5705,//����ʱ�� string
            CG2_GetTime = 0x5706,//���ʱ�� string
            CG2_Pwd = 0x5707,//���� string

            SD_UserName_Old = 0x4522,//�޸�ǰ�û��� string
            CG2_From_PetID = 0x5708,//����(�ͳ�)����id	INT
            CG2_To_PetID = 0x5709,//����(�õ�)����id	INT
            CG2_From_PetName = 0x5710,//����(�ͳ�)������	string
            CG2_To_PetName = 0x5711,//����(�õ�)������	string

            CG2_getMoney = 0x5712,//�õ���Ǯ int

            CG2_From_UserName = 0x5713,//�������	string
            CG2_To_UserName = 0x5714,//�õ����	string
            CG2_From_UserID = 0x5715,//�������ID	int
            CG2_To_UserID = 0x5716,//�õ����ID	int
            CG2_LeftMoney = 0x5717,//ʣ�����

			//maple add for log_query
			CG2_Log_StartTime = 0x5718,//log��ѯ��ʼʱ��
			CG2_Log_EndTime =0x5719,//log��ѯ����ʱ��

			CG2_CharName = 0x5720,//��ɫ����
			CG2_LevelValue =0x5721,//��ɫ�ȼ���
			CG2_LevelResult =0x5722,//���
			CG2_ActionTime  =0x5723,//maple add
			CG2_String =0x5724,//
			CG2_JobName=0x5725,//ְҵ����
			CG2_Remark=0x5726,//�޸�ְҵ��ע

			//new add
			CG2_iReserve00=0x5727,//�������ϴθ���ʱ��
			CG2_BugIndex=0x5728,//����ָ��
			CG2_SoilIndex=0x5729,//����ָ��
			CG2_NutrientIndex=0x5730,//����ָ��
			CG2_TreeState=0x5731,//��ľ״��
			CG2_FruitNum=0x5732,//��ʵ����

			CG2_Dates=0x5733,//ʱ��
			CG2_Marriage=0x5734,//Ԥ����
			CG2_Spouse=0x5735,//Ԥ������ż
			CG2_MyIndex=0x5736,//����ĳ��κ�
			CG2_IsBooking=0x5737,//�Ƿ�Ԥ��
			CG2_MarriageTime=0x5738,//���ؿ�ʼʱ��
			CG2_Gmsv=0x5739,//���ڷ�����
			
			CG2_ChangeCharNo = 0x5740,//��λ�ú�
			CG2_Object = 0x5741,//��־��Ϣ

			CG2_ID=0x5742,//���к�
			CG2_Shop_RECYCLE=0x5743,//�Ƿ�һ�����

			/// <summary>
            /// �������
            /// </summary>
			
			XDJG_ServerIP=0x6002,//������IP
			XDJG_ServerName=0x6003,//����������
			
			//��ѯ
			XDJG_SEARCHTYPE=0x6010,//��ѯ����
			XDJG_Account=0x6011,//�ʺ�
			XDJG_AccountID=0x6012,//�ʺ�ID
			XDJG_Character=0x6013,//�ǳ�
			XDJG_CharacterID=0x6014,//��ɫID
			XDJG_Time=0x6015,//ʱ��
			XDJG_Level=0x6016,//�ȼ�
			XDJG_Exp=0x6017,//����
			XDJG_Sex=0x6018,//�Ա�
			XDJG_Profession=0x6019,//ְҵ
			XDJG_SceneTypeID=0x6020,//��������id
			XDJG_SceneInstanceID=0x6021,//����ʵ��id
			XDJG_Position=0x6022,//λ��
			XDJG_Money=0x6023,//��Ǯ��
			
			//��ͣ
			XDJG_BANREASON=0x6030,//��ͣԭ��
			XDJG_BANTIME=0x6031,//��ͣʱ��
			XDJG_BANBEGINTIME=0x6032,//��ͣ��ʼʱ��
			XDJG_BANENDTIME=0x6033,//���ʱ��
			XDJG_BANTYPE=0x6034,//��ͣ����
			
			//����
			XDJG_TaskID=0x6040,//����ID
			XDJG_NoticeType=0x6041,//��������
			XDJG_BeginTime=0x6042,//��ʼʱ��
			XDJG_EndTime=0x6043,//����ʱ��
			XDJG_BoardMessage=0x6044,//��������
			XDJG_Interval=0x6045,//���ʱ��
			XDJG_Status=0x6046,//����״̬
			
			//����ӿڲ���
			XDJG_BeginDate=0x6047,//��ʼ����
			XDJG_EndDate=0x6048,//��������
			XDJG_StartTime=0x6049,//���濪ʼʱ��
			XDJG_Repeat=0x6050,//�ظ�����
			XDJG_NextTime=0x6051,//�´ι���ʱ��
			XDJG_ExecTimes=0x6052,//ִ�д���
			XDJG_Onoff=0x6053,//���濪��

			//Added by tengjie 2010-07-06
			XDJG_Strength=0x6054,//����
			XDJG_Agility=0x6055,//����
			XDJG_Stamina=0x6056,//����
			XDJG_Spirit=0x6057,//����			
			XDJG_Magic=0x6058,//����
			XDJG_NearPa=0x6059,//����������
			XDJG_FarPa=0x6060,//Զ��������
			XDJG_SuperAttack=0x6061,//���ܹ�����
			XDJG_PerfectRate=0x6062,//����һ����
			XDJG_StrengthCritRate=0x6063,//��������һ����
			XDJG_SpiritCritRate=0x6064,//��������һ����
			XDJG_IsOnLine=0x6065,//�Ƿ�����
			
			//������Ϣ:
			XDJG_Armor=0x6066, //����ֵ
			XDJG_DodgeRate=0x6067,//������
			XDJG_WardRate=0x6068,//����
			XDJG_ParryRate=0x6069,//�м���
			XDJG_FatalResistanceRate=0x6070,//�����ֿ���
			XDJG_FlameResistance=0x6071,//�ֿ�����
			XDJG_ThunderResistance=0x6072,//�ֿ��׵�
			XDJG_FrostResistance=0x6073,//�ֿ���˪
			XDJG_CorrosionResistance=0x6074,//�ֿ���ʴ
			XDJG_EnergyResistance=0x6075,//�ֿ�����
			XDJG_SpiritResistance=0x6076,//�ֿ�����
			
			//PK����:
			XDJG_PK=0x6077,//PK
			XDJG_GoodEvil=0x6078,//�ƶ�
			XDJG_RedNameTime=0x6079,//����ʱ��
			
			//��ɫ�ƺ�:���гƺ��б�
			XDJG_RoleID1=0x6080,//��ɫ�ƺ�1
			XDJG_RoleID2=0x6081,//��ɫ�ƺ�2
			XDJG_RoleID3=0x6082,//��ɫ�ƺ�3
			XDJG_RoleID4=0x6083,//��ɫ�ƺ�4
			XDJG_RoleID5=0x6084,//��ɫ�ƺ�5
			XDJG_RoleID6=0x6085,//��ɫ�ƺ�6
			XDJG_RoleID7=0x6086,//��ɫ�ƺ�7
			XDJG_RoleID8=0x6087,//��ɫ�ƺ�8
			
			//�����
			XDJG_LifeSkill1=0x6088,//�����1
			XDJG_LifeSkill2=0x6089,//�����2
			XDJG_LifeSkill3=0x6090,//�����3
			XDJG_LifeSkill4=0x6091,//�����4
			XDJG_LifeSkill5=0x6092,//�����5
			XDJG_LifeSkill6=0x6093,//�����6
			XDJG_LifeSkill7=0x6094,//�����7
			XDJG_LifeSkill8=0x6095,//�����8

			XDJG_SkillID=0x6096,//����ID
			XDJG_ItemID=0x6097,//����ID
			XDJG_ItemNum=0x6098,//��������

			XDJG_QueryIP=0x6099,//��IP��ѯ
			XDJG_QueryType=0x6100,//��ѯ����(�����ˡ�������)
			XDJG_LoginType=0x6101,//��¼or�ǳ�
			XDJG_LogTime=0x6102,//��־ʱ��
			XDJG_LogInOutIP=0x6103,//��¼�ǳ�IP
			XDJG_ItemName=0x6104,//��������
			XDJG_TempItemID=0x6105,//��ʱ����ID
			XDJG_AccountBuyer=0x6106,//�������ʺ�
			XDJG_RoleBuyer=0x6107,//�������ǳ�
			XDJG_AccountReceiver=0x6108,//�������ʺ�
			XDJG_RoleReceiver=0x6109,//�������ǳ�
			XDJG_ItemType=0x6110,//��Ʒ����
			XDJG_CharacterFrom=0x6111,//��Դ����
			XDJG_TempItemId=0x6112,//������ʱID
			XDJG_MoneyType=0x6113,//��Ǯ����
			XDJG_Property=0x6114,//����Ȩ(������)
			XDJG_Cooldowns=0x6115,//����(������)
			XDJG_Titles=0x6116,//��ɫ�ƺ�(������)

			XDJG_SendAccount=0x6117,//�������ʺ�
			XDJG_SendCharacter=0x6118,//�����˽�ɫ
			XDJG_Title=0x6119,//�ʼ�����
			XDJG_ReceiveTime=0x6120,//����ʱ��
			XDJG_sssTitles=0x6121,//��ɫ�ƺ�(������)
			XDJG_IsBind=0x6122,//�����Ƿ��
			XDJG_QualityEquip=0x6123,//�Ƿ�Ϊ����װ��
			XDJG_SuperpositionNum=0x6124,//��������
			XDJG_CreaterID=0x6125,//������ID
			XDJG_CreateTime=0x6126,//����ʱ��
			XDJG_DurableValue=0x6127,//�;�ֵ
			XDJG_IncreaseType=0x6128,//��������
			XDJG_IncreaseValue=0x6129,//��������ֵ
			XDJG_IncreaseEndValue=0x6130,//���һ����������ֵ
			XDJG_IncreaseNum=0x6131,//��������
			XDJG_AssemblyID=0x6132,//װ����װID
			XDJG_PowerEffect=0x6133,//����Ч��
			XDJG_PowerStartTime=0x6134,//������ʼʱ��
			XDJG_EquipSlotID1=0x6135,//װ����Ƕ��ʯ��ID1
			XDJG_EquipSlotID2=0x6136,//װ����Ƕ��ʯ��ID2
			XDJG_EquipSlotID3=0x6137,//װ����Ƕ��ʯ��ID3
			XDJG_StarsLevel=0x6138,//װ���Ǽ�
			XDJG_UnlockTime=0x6139,//��������
			XDJG_RandomProperty=0x6140,//�������
			XDJG_Pwd=0x6141,//��ʱ����


			XDJG_ItemID1=0x6142,//����ID1
			XDJG_ItemID2=0x6143,//����ID2
			XDJG_ItemID3=0x6144,//����ID3
			XDJG_ItemID4=0x6145,//����ID4
			XDJG_ItemID5=0x6146,//����ID5
			XDJG_ItemID6=0x6147,//����ID6
			XDJG_ItemID7=0x6148,//����ID7
			XDJG_ItemID8=0x6149,//����ID8

			XDJG_ItemNum1=0x6150,//��������1
			XDJG_ItemNum2=0x6151,//��������2
			XDJG_ItemNum3=0x6152,//��������3
			XDJG_ItemNum4=0x6153,//��������4
			XDJG_ItemNum5=0x6154,//��������5
			XDJG_ItemNum6=0x6155,//��������6
			XDJG_ItemNum7=0x6156,//��������7
			XDJG_ItemNum8=0x6157,//��������8

			XDJG_TempItemId1=0x6158,//��ʱ����ID1
			XDJG_TempItemId2=0x6159,//��ʱ����ID2
			XDJG_TempItemId3=0x6160,//��ʱ����ID3
			XDJG_TempItemId4=0x6161,//��ʱ����ID4
			XDJG_TempItemId5=0x6162,//��ʱ����ID5
			XDJG_TempItemId6=0x6163,//��ʱ����ID6
			XDJG_TempItemId7=0x6164,//��ʱ����ID7
			XDJG_TempItemId8=0x6165,//��ʱ����ID8

			XDJG_Task_ID=0x6166,//����ID
			XDJG_Task_Name=0x6167,//��������
			XDJG_TaskType=0x6168,//��������

			XDJG_ItemThrowID1=0x6169,//ʧȥ����ID1
			XDJG_ItemThrowID2=0x6170,//ʧȥ����ID2
			XDJG_ItemThrowID3=0x6171,//ʧȥ����ID3
			XDJG_ItemThrowID4=0x6172,//ʧȥ����ID4
			XDJG_ItemThrowID5=0x6173,//ʧȥ����ID5
			XDJG_ItemThrowID6=0x6174,//ʧȥ����ID6
			XDJG_ItemThrowID7=0x6175,//ʧȥ����ID7
			XDJG_ItemThrowID8=0x6176,//ʧȥ����ID8
 			
			XDJG_TempItemThrowId1=0x6177,//��ʱ����ID1
			XDJG_TempItemThrowId2=0x6178,//��ʱ����ID2
			XDJG_TempItemThrowId3=0x6179,//��ʱ����ID3
			XDJG_TempItemThrowId4=0x6180,//��ʱ����ID4
			XDJG_TempItemThrowId5=0x6181,//��ʱ����ID5
			XDJG_TempItemThrowId6=0x6182,//��ʱ����ID6
			XDJG_TempItemThrowId7=0x6183,//��ʱ����ID7
			XDJG_TempItemThrowId8=0x6184,//��ʱ����ID8

			XDJG_ItemThrowNum1=0x6185,//ʧȥ��������1
			XDJG_ItemThrowNum2=0x6186,//ʧȥ��������2
			XDJG_ItemThrowNum3=0x6187,//ʧȥ��������3
			XDJG_ItemThrowNum4=0x6188,//ʧȥ��������4
			XDJG_ItemThrowNum5=0x6189,//ʧȥ��������5
			XDJG_ItemThrowNum6=0x6190,//ʧȥ��������6
			XDJG_ItemThrowNum7=0x6191,//ʧȥ��������7
			XDJG_ItemThrowNum8=0x6192,//ʧȥ��������8

			XDJG_Items=0x6193,//��Ʒ����
			XDJG_SurplusTime=0x6194,//ʣ��ʱ��
			XDJG_AuctionPrice=0x6195,//�����۸�
			XDJG_Deposit=0x6196,//��֤��
			XDJG_SkillName=0x6197,//ģ����������
			XDJG_UpdateType=0x6198,//�޸�����(�ȼ�,����)
			XDJG_Prototype=0x6199,//���������
			XDJG_MailTitle=0x6200,//�ʼ�����
			XDJG_MailContent=0x6201,//�ʼ�����
			XDJG_PropertyId=0x6202,//��������ID
			XDJG_PropertyValue=0x6203,//��������ֵ
			XDJG_SlotNum=0x6204,//�������
			XDJG_CarID=0x6205,//���ID
			XDJG_cpValue=0x6206,//��ǰCPֵ
			XDJG_LookID=0x6207,//���ID
			XDJG_IsLock=0x6208,//�Ƿ�����
			XDJG_SkillStrValue=0x6209,//�츳����ֵ
			XDJG_TimePlugNum=0x6210,//ʱ��оƬ����
			XDJG_TimePlugSlot1=0x6211,//ʱ��оƬid1
			XDJG_TimePlugSlot2=0x6212,//ʱ��оƬid2
			XDJG_BrandCreateTime=0x6213,//�̱괴��ʱ��
			XDJG_BrandID=0x6214,//�̱�ID

			XDJG_GuildID=0x6215,//����ID
			XDJG_GuildName=0x6216,//��������
			XDJG_GuildLevel=0x6217,//����ȼ�
			XDJG_GuildLeader=0x6218,//�᳤����
			XDJG_Guildmember=0x6219,//��Ա����
			XDJG_JoinGuildTime=0x6220,//���ʱ��
			XDJG_TaskStep=0x6221,//�������
			XDJG_Guildmembers=0x6222,//��������
			XDJG_OperateTime=0x6223,//����ʱ��
			XDJG_Operatetype=0x6224,//��������
			XDJG_MoneyLose=0x6225,//���Ľ�Ǯ
			XDJG_TempCharacter=0x6226,//��ʱ��ɫ
			XDJG_JoinTime=0x6227,//����ʱ��
			XDJG_GuildLog=0x6228,//������־
			XDJG_LeaveTime=0x6229,//�뿪ʱ��
			XDJG_ExitGuildType=0x6230,//�뿪����
			XDJG_LeaderChangedTime=0x6231,//תְʱ��
			XDJG_TempSkillId=0x6232,//��ʱ����id
			XDJG_GuildPosition=0x6233,//��Աְλ
			XDJG_GuildTransferedPosition=0x6234,//�����ְλ
			XDJG_ConsumeItem=0x6235,//���ĵ���
			XDJG_CarName=0x6236,//�������
			XDJG_TempTaskId=0x6237,//������ʱID
			XDJG_IsDeleted=0x6238,//�Ƿ�ɾ��
			XDJG_RoleProperty=0x6239,//��������
			XDJG_ActivityName=0x6240,//�������
			XDJG_GoodsName=0x6241,//��Ʒ����
			XDJG_GoodsUsedOff=0x6242,//��Ʒʹ������
			XDJG_GetTime=0x6243,//��ȡʱ��

			XDJG_ChoiceItemID1=0x6244,//��ѡ����id1
			XDJG_ChoiceItemID2=0x6245,//��ѡ����id2
			XDJG_ChoiceItemID3=0x6246,//��ѡ����id3
			XDJG_ChoiceItemID4=0x6247,//��ѡ����id4
			XDJG_ChoiceItemID5=0x6248,//��ѡ����id5
			XDJG_ChoiceItemID6=0x6249,//��ѡ����id6

			XDJG_ChoiceItemNum1=0x6250,//��ѡ��������1
			XDJG_ChoiceItemNum2=0x6251,//��ѡ��������2
			XDJG_ChoiceItemNum3=0x6252,//��ѡ��������3
			XDJG_ChoiceItemNum4=0x6253,//��ѡ��������4
			XDJG_ChoiceItemNum5=0x6254,//��ѡ��������5
			XDJG_ChoiceItemNum6=0x6255,//��ѡ��������6
			
			XDJG_TitleID=0x6256,//�ƺ�ID
			XDJG_BuffID=0x6257,//BUFFID
			XDJG_PositionX=0x6258,//X����
			XDJG_PositionY=0x6259,//Y����
			XDJG_PositionZ=0x6260,//Z����
			XDJG_TargetCharacter=0x6261,//Ŀ���ɫ
			XDJG_TitleName=0x6262,//�ƺ�
			XDJG_BuffName=0x6263,//BUFF����
			//End

			/// <summary>
			/// �������߶���
			/// </summary>
			SDO_ServerIP = 0x0801,//Format:String ����IP
			SDO_UserIndexID = 0x0802,//Format:Integer ����û�ID
			SDO_Account = 0x0803,//Format:String ��ҵ��ʺ�
			SDO_Level = 0x0804,//Format:Integer ��ҵĵȼ�
			SDO_Exp = 0x0805,//Format:Integer ��ҵĵ�ǰ����ֵ
			SDO_GameTotal = 0x0806,//Format:Integer �ܾ���
			SDO_GameWin  = 0x0807,//Format:Integer ʤ����
			SDO_DogFall = 0x0808,//Format:Integer ƽ����
			SDO_GameFall = 0x0809,//Format:Integer ������
			SDO_Reputation = 0x0810,//Format:Integer ����ֵ
			SDO_GCash = 0x0811,//Format:Integer G��
			SDO_MCash = 0x0812,//Format:Integer M��
			SDO_Address = 0x0813,//Format:Integer ��ַ
			SDO_Age = 0x0814,//Format:Integer ����
			SDO_ProductID = 0x0815,//Format:Integer ��Ʒ���
			SDO_ProductName = 0x0816,//Format:String ��Ʒ����
			SDO_ItemCode  = 0x0817,//Format:Integer ���߱��
			SDO_ItemName = 0x0818,//Format:String ��������
			SDO_TimesLimit = 0x0819,//Format:Integer ʹ�ô���
			SDO_DateLimit = 0x0820,//Format:Integer ʹ��ʱЧ
			SDO_MoneyType = 0x0821,//Format:Integer ��������
			SDO_MoneyCost = 0x0822,//Format:Integer ���ߵļ۸�
			SDO_ShopTime = 0x0823,//Format:DateTime ����ʱ��
			SDO_MAINCH = 0x0824,//Format:Integer ������
			SDO_SUBCH = 0x0825,//Format:Integer ����
			SDO_Online = 0x0826,//Format:Integer �Ƿ�����
			SDO_LoginTime = 0x0827,//Format:DateTime ����ʱ��
			SDO_LogoutTime = 0x0828,//Format:DateTime ����ʱ��
			SDO_AREANAME = 0x0829,//Format:String ��������
			SDO_City = 0x0830,//Format:String �����ס����
			SDO_Title = 0x0831,//Format:String ��������
			SDO_Context = 0x0832,//Format:String ��������
			SDO_MinLevel = 0x0833,//Format:Integer �������ߵ���С�ȼ�
			SDO_ActiveStatus = 0x0834,//Format:Integer ����״̬
			SDO_StopStatus = 0x0835,//Format:Integer ��ͣ״̬
			SDO_NickName = 0x0836,//Format:String �س�
			SDO_9YouAccount = 0x0837,//Format:Integer 9you���ʺ�
			SDO_SEX = 0x0838,//Format:Integer �Ա�
			SDO_RegistDate =  0x0839,//Format:Date ע������
			SDO_FirstLogintime = 0x0840,//Format:Date ��һ�ε�¼ʱ��
			SDO_LastLogintime  = 0x0841,//Format:Date ���һ�ε�¼ʱ��
			SDO_Ispad = 0x0842,//Format:Integer �Ƿ���ע������̺
			SDO_Desc = 0x0843,//Format:String ��������
			SDO_Postion = 0x0844,//Format:Integer ����λ��
			SDO_BeginTime = 0x0845,//Format:Date ���Ѽ�¼��ʼʱ��
			SDO_EndTime = 0x0846,//Format:Date ���Ѽ�¼����ʱ��
			SDO_SendTime = 0x0847,//Format:Date ������������
			SDO_SendIndexID = 0x0848,//Format:Integer �����˵�ID
			SDO_SendUserID = 0x0849,//Format:String �������ʺ�
			SDO_ReceiveNick = 0x0850,//Format:String �������س�
			SDO_BigType = 0x0851,//Format:Integer ���ߴ���
			SDO_SmallType = 0x0852,//Format:Integer ����С��
			SDO_REASON = 0x0853,//Format:String ͣ������
			SDO_StopTime = 0x0854,//Format:TimeStamp ͣ��ʱ��
			SDO_DaysLimit  = 0x0855,//Format:Integer ʹ������
			SDO_Email = 0x0856,//Format:String �ʼ�
			SDO_ChargeSum = 0x0857,//Format:String ��ֵ�ϼ�
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
			SDO_FirstPadTime = 0x08102,//����̺��һ��ʹ��ʱ��
			SDO_BanDate = 0x08103,//ͣ�������
			SDO_Passwd = 0x08104,
			SDO_OnlineTime = 0x08105,//����ʱ��string
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
			SDO_State = 0x8134,//TLV_STRING  ״̬
			SDO_mood = 0x8135,//TLV_INTEGER ����ֵ
			SDO_Food = 0x8136,//TLV_INTEGER ��ʳ��
			SDO_ItemCode1 = 0x8137,//TLV_STRING  ����ID
			SDO_DateLimit1 = 0x8138,//TLV_INTEGER ��������
			SDO_TimeLimit1= 0x8139,//TLV_INTEGER ��������
			SDO_ItemCode2 = 0x8140,//TLV_STRING  ����ID
			SDO_DateLimit2 = 0x8141,//TLV_INTEGER ��������
			SDO_TimeLimit2= 0x8142,//TLV_INTEGER ��������
			SDO_ItemCode3 = 0x8143,//TLV_STRING  ����ID
			SDO_DateLimit3 = 0x8144,//TLV_INTEGER ��������
			SDO_TimeLimit3= 0x8145,//TLV_INTEGER ��������
			SDO_ItemCode4 = 0x8146,//TLV_STRING  ����ID
			SDO_DateLimit4 = 0x8147,//TLV_INTEGER ��������
			SDO_TimeLimit4= 0x8148,//TLV_INTEGER ��������
			SDO_ItemCode5 = 0x8149,//TLV_STRING  ����ID
			SDO_DateLimit5 = 0x8150,//TLV_INTEGER ��������
			SDO_TimeLimit5= 0x8151,//TLV_INTEGER ��������
			SDO_ItemName1 = 0x8152,//TLV_STRING  ������1
			SDO_ItemName2 = 0x8153,//TLV_STRING  ����ID2
			SDO_ItemName3 = 0x8154,//TLV_STRING  ����ID3
			SDO_ItemName4 = 0x8155,//TLV_STRING  ����ID4
			SDO_ItemName5 = 0x8156,//TLV_STRING  ����ID5
			SDO_CoupleUserName = 0x8157,//TLV_STRING  Ů���ʺ�
			SDO_KeyPress = 0x8158,
			SDO_LoginErr = 0x8159,
			SDO_PreValue= 0x8160,//��Сֵ
			SDO_EndValue= 0x8161,//���ֵ
			SDO_NorProFirst= 0x8162,//��һ�δ򿪵ĸ���
			SDO_NorPro= 0x8163,//��ͨ����ĸ���
			SDO_SpePro= 0x8164,//���ⱦ��ĸ���
			SDO_baoxiangid= 0x8165,//id int
			SDO_Mark= 0x8166,//��ʶ int

			SDO_RemoteIp=0x8167,
			SDO_RemoteType=0x8168,
			SDO_MsgID=0x8169,//id
			SDO_channelname=0x8171,//string 
			SDO_type=0x8172,//string 
			SDO_UserName=0x8173,//string
			SDO_Message=0x8174,//string 
			SDO_datetime=0x8175,//string 
			SDO_ServerName=0x8176,//string
			SDO_ChannelID=0x8177,//string Ƶ��id
			SDO_PlanetID=0x8178,//string ����id
			SDO_PlanetName=0x8179,//string������
			SDO_ChannelName=0x8180,//stringƵ����
			SDO_FamilyName = 0x08181,//string ��������
			SDO_FamilyID = 0x08182,//int ����ID
			SDO_BuyUserNick = 0x8183,//string �����û��س�
			SDO_BeforeMoney = 0x8184,//����ǰ����M������
			SDO_Shaikh = 0x8185,//�����峤string
			SDO_Contribute = 0x8186,//���幱��int
			SDO_Popular = 0x8187,//��������ֵint
			SDO_Curdaypopular = 0x8188,//�����ۼƼ�������ֵint
			SDO_Opensign = 0x8189,//���ű�־ int
			SDO_Fmlevel = 0x8190,//����ȼ� int
			SDO_Maxmember = 0x8191,//��������Ա int
			SDO_Curmember = 0x8192,//�������г�Ա int
			SDO_Fortune = 0x8193,//����Ƹ�int
			SDO_Found_Date = 0x8194,//����ʱ�� datetime
			SDO_Rank = 0x8195,//�������� int
			SDO_Honor = 0x8196,//������������ string
			SDO_UseCost = 0x8197,//ʹ�õ�G�� int 
			SDO_GetTime = 0x8198,//Format:Date ���ʱ��
			SDO_Duty = 0x8199,//����Ȩ��(255��ʾ�峤0��ͨ��Ա1����Ա) string
			SDO_Formal = 0x8200,//�������(0��ʾ������룬1��ʾ���ڼ���) string
			SDO_Emblemtype = 0x8201,//ѫ������
			SDO_Emblemnum = 0x8202,//ѫ������
			SDO_MoneyTime = 0x8203,//����ʱ��
			SDO_PetLevel = 0x8204,//����ȼ�
			SDO_PetExperience = 0x8205,//���ﾭ��
			SDO_JoinDate = 0x8206,//����ʱ��
			SDO_Money = 0x8207,//���Ǯ��

			SDO_SocialValue=0x8208,//�罻ֵ
			SDO_Lucky=0x8209,//����ֵ



			Ban_UserID=0x9001,//�û���ID
			Ban_UserName=0x9002,//�û���
			Ban_NickID=0x9003,//�ǳ�ID
			Ban_UserNick=0x9004,//�ǳ�
			Ban_StartTime=0x9005,//��ʼʱ��
			Ban_EndTime=0x9006,//����ʱ��
			Ban_Reason=0x9007,//��/��ͣ����
			Ban_Memo=0x9008,//��ע(�˺�1,�ǳ�0)
			Ban_Type=0x9009,//�������0����ʱ���1
			

			
			MESSAGE = 0x4001,
			ERROR_Msg = 0xFFFF
		};  
		__Enum _value; // ö��ֵ  
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
			GMTOOLS_OperateLog_Query = 0x0005,//�鿴���߲�����¼
			GMTOOLS_OperateLog_Query_RESP = 0x8005,//�鿴���߲�����¼��Ӧ
			SERVERINFO_IP_ALL_QUERY = 0x0006,//�鿴������Ϸ��������ַ
			SERVERINFO_IP_ALL_QUERY_RESP = 0x8006,//�鿴������Ϸ��������ַ��Ӧ
			LINK_SERVERIP_CREATE = 0x0007,//�����Ϸ�������ݿ�
			LINK_SERVERIP_CREATE_RESP = 0x8007,//�����Ϸ�������ݿ���Ӧ
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
			GM_AccountInfo_Vip = 0x0020,			//��ѯvip�ʺ�
			GM_AccountInfo_Vip_RESP = 0x8020,		//��ѯvip�ʺ���Ӧ
			GM_ClosePlayerGroup_WA = 0x0021,			//��ͣ�ʺ�
			GM_ClosePlayerGroup_RESP_WA = 0x8021,      //��ͣ�ʺ���Ӧ
			///<summary>
			///�û�����ģ��(0x81) 
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
			UPDATE_ACTIVEUSER = 0x0008,//���������û�״̬
			UPDATE_ACTIVEUSER_RESP = 0x8008,//���������û�״̬��Ӧ
			DEPARTMENT_CREATE = 0x0009,//���Ŵ���
			DEPARTMENT_CREATE_RESP = 0x8009,//���Ŵ�����Ӧ
			DEPARTMENT_UPDATE= 0x0010,//�����޸�
			DEPARTMENT_UPDATE_RESP = 0x8010,//�����޸���Ӧ
			DEPARTMENT_DELETE= 0x0011,//����ɾ��
			DEPARTMENT_DELETE_RESP = 0x8011,//����ɾ����Ӧ
			DEPARTMENT_ADMIN = 0x0012,//���Ź���
			DEPARTMENT_ADMIN_RESP = 0x8012,//���Ź�����Ӧ
			DEPARTMENT_RELATE_QUERY = 0x0013,//���Ź�����ѯ
			DEPARTMENT_RELATE_QUERY_RESP = 0x8013,//���Ź�����ѯ
			DEPART_RELATE_GAME_QUERY = 0x0014,
			DEPART_RELATE_GAME_QUERY_RESP = 0x8014,
			USER_SYSADMIN_QUERY = 0x0015,
			USER_SYSADMIN_QUERY_RESP = 0x8015,
			USERBAN_QUERY = 0x0016,
			USERBAN_QUERY_RESP = 0x8016,


			USER_ACTIVE=0x0018,//������
			USER_ACTIVE_RESP=0x8018,


			//Added by tengjie 2010-05-05
			HARMFUL_PROC_QUERY = 0x0019,//��ҽ��̹��߰�
			HARMFUL_PROC_QUERY_RESP=0x8019,
			/// <summary>
			/// ģ�����(0x82)
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
			/// �û���ģ��(0x83) 
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
			/// ��Ϸ����(0x84) 
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
			/// �ɽ�GM����(0x62)
			/// </summary>
			PAL_PLAYERONLINECOUNT=0x0001,//�����������
			PAL_PLAYERONLINECOUNT_RESP=0x8001,//�������������Ӧ

			PAL_CHARACTERBAG=0x0002,//��ұ�����ѯ
			PAL_CHARACTERBAG_RESP=0x8002,//��ұ�����ѯ��Ӧ

			PAL_CHARACTERFRIENDLIST=0x0003,//ȡ�ý�ɫ�����б�
			PAL_CHARACTERFRIENDLIST_RESP=0x8003,//ȡ�ý�ɫ�����б���Ӧ

			PAL_CHARACTERINFO=0x0004,//ȡ�ý�ɫ������Ѷ
			PAL_CHARACTERINFO_RESP=0x8004,//ȡ�ý�ɫ������Ѷ��Ӧ

			PAL_CHARACTERLIST=0x0005,//ȡ�ý�ɫ�б�
			PAL_CHARACTERLIST_RESP=0x8005,//ȡ�ý�ɫ�б���Ӧ

			PAL_CHARACTERQUEST=0x0006,//ȡ�ý�ɫ������Ѷ
			PAL_CHARACTERQUEST_RESP=0x8006,//ȡ�ý�ɫ������Ѷ��Ӧ

			PAL_GUILDBASEINFO=0x0007,//ȡ�ù��������Ѷ
			PAL_GUILDBASEINFO_RESP=0x8007,//ȡ�ù��������Ѷ��Ӧ

			PAL_GUILDMEMBERLIST=0x0008,//ȡ�ù����Ա�б�
			PAL_GUILDMEMBERLIST_RESP=0x8008,//ȡ�ù����Ա�б���Ӧ

			PAL_SENDGIFTMAIL=0x0009,//���ͽ�Ʒ�ʼ�
			PAL_SENDGIFTMAIL_RESP=0x8009,//���ͽ�Ʒ�ʼ���Ӧ

			PAL_ACCOUNTLOCAL_QUERY = 0x0010,//���ط�ͣ������ʺŲ�ѯ
			PAL_ACCOUNTLOCAL_QUERY_RESP = 0x8010,//���ط�ͣ������ʺŲ�ѯ��Ӧ
			PAL_ACCOUNT_CLOSE = 0x0011,//��ͣ������ʺ�
			PAL_ACCOUNT_CLOSE_RESP = 0x8011,//��ͣ������ʺ���Ӧ
			PAL_ACCOUNT_OPEN = 0x0012,//��������ʺ�
			PAL_ACCOUNT_OPEN_RESP = 0x8012,//��������ʺ���Ӧ
			PAL_ACCOUNT_BANISHMENT_QUERY = 0x0013,//��ҷ�ͣ�ʺŲ�ѯ
			PAL_ACCOUNT_BANISHMENT_QUERY_RESP = 0x8013,//��ҷ�ͣ�ʺŲ�ѯ��Ӧ

			
			PAL_MODIFYCHARACTERMONEY=0x0014,//�޸Ľ�ɫ��Ǯ
			PAL_MODIFYCHARACTERMONEY_RESP=0x8014,//�޸Ľ�ɫ��Ǯ��Ӧ

			PAL_MODIFYCHARACTERDROPPW=0x0015,//�޸Ľ�ɫɾ������
			PAL_MODIFYCHARACTERDROPPW_RESP=0x8015,//�޸Ľ�ɫɾ��������Ӧ

			PAL_MODIFYACCOUNTBANKPW=0x0016,//�޸��ʺ���������
			PAL_MODIFYACCOUNTBANKPW_RESP=0x8016,//�޸��ʺ�����������Ӧ

			PAL_DELCHARACTERITEM=0x0017,// ɾ����ɫ��Ʒ
			PAL_DELCHARACTERITEM_RESP=0x8017,// ɾ����ɫ��Ʒ��Ӧ

			PAL_RECALLCHARACTERDROPPW=0x0018,//��ԭ��ɫɾ������
			PAL_RECALLCHARACTERDROPPW_RESP=0x8018,//��ԭ��ɫɾ��������Ӧ

			PAL_RECALLACCOUNTBANKPW=0x0019,//��ԭ�ʺ���������
			PAL_RECALLACCOUNTBANKPW_RESP=0x8019,//��ԭ�ʺ�����������Ӧ

			PAL_MODIFYACCOUNTPW=0x0020,//�޸��ʺ�����
			PAL_MODIFYACCOUNTPW_RESP=0x8020,//�޸��ʺ�������Ӧ

			PAL_RECALLACCOUNTPW=0x0021,//��ԭ�ʺ�����
			PAL_RECALLACCOUNTPW_RESP=0x8021,//��ԭ�ʺ�������Ӧ

			PAL_BOARDTASK_INSERT = 0x0033,//�������
			PAL_BOARDTASK_INSERT_RESP = 0x8033,//���������Ӧ
			PAL_BOARDTASK_QUERY = 0x0034,//�����ѯ
			PAL_BOARDTASK_QUERY_RESP = 0x8034,//�����ѯ��Ӧ
			PAL_BOARDTASK_UPDATE = 0x0035,//�������
			PAL_BOARDTASK_UPDATE_RESP = 0x8035,//���������Ӧ


			PAL_CHECKMAILBOX_QUERY=0x0036,//������Ϣ��ѯ
			PAL_CHECKMAILBOX_QUERY_RESP=0x8036,//������Ϣ��ѯ��Ӧ
			
			PAL_CHECKMAILBOX_DETAIL=0x0043,//���丽����ѯ
			PAL_CHECKMAILBOX_DETAIL_RESP=0x8043,//���丽����ѯ��Ӧ
			
			PAL_AUCTION_QUERY=0x0037,//�г���Ϣ��ѯ
			PAL_AUCTION_QUERY_RESP=0x8037,//�г���Ϣ��ѯ��Ӧ 
			PAL_ITEMTYPE_QUERY=0x0037,//��������ѯ
			PAL_ITEMTYPE_QUERY_RESP=0x8037,//��������ѯ��Ӧ
			PAL_ITEMNAME_QUERY=0x0038,//�������Ʋ�ѯ
			PAL_ITEMNAME_QUERY_RESP=0x8038,//�������Ʋ�ѯ��Ӧ
			PAL_ITEMNAMEBLUR_QUERY=0x0039,//��������ģ����ѯ
			PAL_ITEMNAMEBLUR_QUERY_RESP=0x8039,//��������ģ����ѯ��Ӧ

			PAL_CDKEY_QUERY=0x0040,//CDKEY������ȡ��ѯ
			PAL_CDKEY_QUERY_RESP=0x8040,//CDKEY������ȡ��ѯ��Ӧ 

			PAL_ITEMMAIL_WASTE=0x0041,//���Ѽ�¼
			PAL_ITEMMAIL_WASTE_RESP=0x8041,//���Ѽ�¼��Ӧ

			PAL_HISTORY_LOGINFO_QUERY=0x0042,//��־��ʷ
			PAL_HISTORY_LOGINFO_QUERY_RESP=0x8042,//��־��ʷ��Ӧ

			PAL_ROLEDEL_QUERY=0x0050,//ɾ����ɫ��־
			PAL_ROLEDEL_QUERY_RESP=0x8050,//ɾ����ɫ��־
			
			PAL_MAILBOX_QUERY=0x0040,//��ѯ��������ַ
			PAL_MAILBOX_QUERY_RESP=0x8040,//��ѯ��������ַ��Ӧ 

			PAL_SendBankPwd_Query=0x0041,//���Ͳֿ�����
			PAL_SendBankPwd_Query_RESP=0x8041,//���Ͳֿ�������Ӧ

			PAL_SendDeletePwd_Query=0x0042,//���ͽ�ɫɾ������
			PAL_SendDeletePwd_Query_RESP=0x8042,//���ͽ�ɫɾ��������Ӧ


			PAL_MODIFYSECONDPW=0x0043,//�޸���Ϸ��������
			PAL_MODIFYSECONDPW_RESP=0x8043,//�޸���Ϸ����������Ӧ

			PAL_RECALLSECONDPW=0x0044,//��ԭ��Ϸ��������
			PAL_RECALLSECONDPW_RESP=0x8044,//��ԭ��Ϸ����������Ӧ


			PAL_USESAFELOCK=0x0045,//ʹ�ð�ȫ��
			PAL_USESAFELOCK_RESP=0x8045,//ʹ�ð�ȫ����Ӧ

			PAL_FREESAFELOCK=0x0046,//�ͷŰ�ȫ��
			PAL_FREESAFELOCK_RESP=0x8046,//�ͷŰ�ȫ����Ӧ


			PAL_CHARACTERADVANCEINFO=0x0047,//ȡ�ý�ɫ������Ѷ
			PAL_CHARACTERADVANCEINFO_RESP=0x8047,//ȡ�ý�ɫ������Ѷ
			
			PAL_CHARACTERSKILL=0x0048,//ȡ�ý�ɫ������Ѷ
			PAL_CHARACTERSKILL_RESP=0x8048,//ȡ�ý�ɫ������Ѷ
			
			PAL_CHARACTERMOB=0x0049,//ȡ�ý�ɫ��ħ��Ѷ
			PAL_CHARACTERMOB_RESP=0x8049,//ȡ�ý�ɫ��ħ��Ѷ
			
			
			PAL_CHARACTERPUZZLEMAP=0x0050,//ȡ�ý�ɫͼ����Ѷ
			PAL_CHARACTERPUZZLEMAP_RESP=0x8050,//ȡ�ý�ɫͼ����Ѷ
			
			PAL_CHARACTERBLACKLIST=0x0051,//ȡ�ý�ɫ�������б�
			PAL_CHARACTERBLACKLIST_RESP=0x8051,//ȡ�ý�ɫ�������б�
			
			PAL_DELCHARACTERMOB=0x0052,//ɾ����ɫ��ħ
			PAL_DELCHARACTERMOB_RESP=0x8052,//ɾ����ɫ��ħ
			
			PAL_MODIFYPUZZLEMAP=0x0053,//�޸Ľ�ɫͼ��
			PAL_MODIFYPUZZLEMAP_RESP=0x8053,//�޸Ľ�ɫͼ��
			
			PAL_ACCOUNTINFO=0x0054,//ȡ���ʺ���Ѷ
			PAL_ACCOUNTINFO_RESP=0x8054,//ȡ���ʺ���Ѷ
			
			PAL_MOVECHARACTERTOSAFE=0x0055,//�ƶ���ɫ����ȫ��
			PAL_MOVECHARACTERTOSAFE_RESP=0x8055,//�ƶ���ɫ����ȫ��
			
			PAL_RELOADFUNCTIONSWITCH=0x0056,//�������빦�ܿ����б�
			PAL_RELOADFUNCTIONSWITCH_RESP=0x8056,//�������빦�ܿ����б�

			PAL_SearchMap_Query=0x0057,//��ѯ��ͼ
			PAL_SearchMap_Query_Resp=0x8057,//��ѯ��ͼ

			PAL_Get_EmailAddress=0x0066,
			PAL_Get_EmailAddress_Resp=0x8066,


			PAL_SCHEDULEBULLETINLIST=0x1001,//ȡ���ų̹����б�
			PAL_SCHEDULEBULLETINLIST_RESP=0x7001,//ȡ���ų̹����б���Ӧ

			PAL_ADDSCHEDULEBULLETIN=0x1002,//�����ų̹���
			PAL_ADDSCHEDULEBULLETIN_RESP=0x7002,//�����ų̹�����Ӧ	

			PAL_DELSCHEDULEBULLETIN=0x1003,//ɾ���ų̹���
			PAL_DELSCHEDULEBULLETIN_RESP=0x7003,//ɾ���ų̹�����Ӧ	

			PAL_RELOADSCHEDULEBULLETIN=0x1004,//���������ų̹���
			PAL_RELOADSCHEDULEBULLETIN_RESP=0x7004,//���������ų̹�����Ӧ	

			PAL_SENDNONSCHEDULEBULLETIN=0x1005,//���ͷǹ̶�����
			PAL_SENDNONSCHEDULEBULLETIN_RESP=0x7005,//���ͷǹ̶�������Ӧ	




			PAL_BANISHPLAYER =0x4001,//����
			PAL_BANISHPLAYER_RESP=0x6001,//������Ӧ

			PAL_DISMISSGUILD=0x4002,//��ɢ����
			PAL_DISMISSGUILD_RESP=0x6002,//��ɢ������Ӧ

			PAL_MODIFYGUILDNAME=0x4003,//�޸Ĺ�������
			PAL_MODIFYGUILDNAME_RESP=0x6003,//�޸Ĺ���������Ӧ

			PAL_MODIFYGUILDMEMBERPL=0x4004,//�޸Ĺ����Աְ��
			PAL_MODIFYGUILDMEMBERPL_RESP=0x6004,//�޸Ĺ����Աְ����Ӧ

			PAL_MODIFYCHARACTERQUEST=0x4005,//�޸Ľ�ɫ����
			PAL_MODIFYCHARACTERQUEST_RESP=0x6005,//�޸Ľ�ɫ������Ӧ

			PAL_MODIFYGUILDLV=0x4006,//�޸Ĺ���ȼ�
			PAL_MODIFYGUILDLV_RESP=0x6006,//�޸Ĺ���ȼ���Ӧ

			PAL_MODIFYGUILDNOTICE=0x4007,//�޸Ĺ��ṫ��
			PAL_MODIFYGUILDNOTICE_RESP=0x6007,//�޸Ĺ��ṫ����Ӧ

			PALLOG_ACCOUNT_LOGIN=0x0001,//��½��¼��־
			PALLOG_ACCOUNT_LOGIN_RESP=0x8001,//��½��¼��־��Ӧ

			PAL_LOGINFO_QUERY=0x0002,//�����־��Ϣ
			PAL_LOGINFO_QUERY_RESP=0x8002,//�����־��Ϣ��Ӧ

			PAL_LOG_BIGTYPE_QUERY=0x0003,//��־����
			PAL_LOG_BIGTYPE_QUERY_RESP=0x8003,//��־������Ӧ

			PAL_LOG_SMALLTYPE_QUERY=0x0004,//��־С��
			PAL_LOG_SMALLTYPE_QUERY_RESP=0x8004,//��־С����Ӧ

			PALLOG_ITEMMAIL=0x0005,//�̳ǹ�����־
			PALLOG_ITEMMAIL_RESP=0x8005,//�̳ǹ�����־��Ӧ

			PAL_EQUIPMENT_QUERY=0x0061,//װ�����߲�ѯ
			PAL_EQUIPMENT_QUERY_RESP=0x8061,//װ�����߲�ѯ��Ӧ
			
			PAL_SENDCUSTOMITEMMAIL=0x0062,//���Ϳ�����Ʒ�ʼ�
			PAL_SENDCUSTOMITEMMAIL_RESP=0x8062,//���Ϳ�����Ʒ�ʼ���Ӧ

			PAL_MODIFYACCOUNTPWALL=0x0063,//�޸����д����ʺ�����
			PAL_MODIFYACCOUNTPWALL_RESP=0x8063,//�޸����д����ʺ�������Ӧ
			
			PAL_RECALLACCOUNTPWALL=0x0064,//�ָ����Դ����ʺ�����
			PAL_RECALLACCOUNTPWALL_RESP=0x8064,//�ָ����Դ����ʺ�������Ӧ

			//////////////////
			//GTown(0x51)
			/////////////////
			//��ӹ���
			GT_BROADCAST_ADD = 0x0001,
			GT_BROADCAST_ADD_ACK = 0x8001,

			//��ѯ���й���
			GT_BROADCAST_SELECT = 0x0002,
			GT_BROADCAST_SELECT_ACK = 0x8002,

			//ɾ������
			GT_BROADCAST_REMOVE = 0x0003,
			GT_BROADCAST_REMOVE_ACK = 0x8003,

			//ǿ���������
			GT_KICK_USER = 0x0004,
			GT_KICK_USER_ACK = 0x8004,

			//��ɫ����С����
			GT_MOVE_USER = 0x0005,
			GT_MOVE_USER_ACK = 0x8005,

			//�������
			GT_FAMILY_DISBAND = 0x0006,
			GT_FAMILY_DISBAND_ACK = 0x8006,

			//�˺ŷ�ͣ�����
			GT_USER_STOP = 0x0007,
			GT_USER_STOP_ACK = 0x8007,

			//��ӵ��ߡ�����
			GT_ADD_ITEM = 0x0008,
			GT_ADD_ITEM_ACK = 0x8008,

			//��ѯ���ߡ�����
			GT_ITEM_SELECT = 0x0009,
			GT_ITEM_SELECT_ACK = 0x8009,

			//������ҵ�����״̬
			GT_USER_RESET = 0x0010,
			GT_USER_RESET_ACK = 0x8010,

			CONNECT_SERVERMANAGER_LOGIN = 0x0011,
			CONNECT_SERVERMANAGER_LOGIN_ACK = 0x8011,

			//��ѯ�û�������Ϣ
			GT_UserInfo_Query = 0x0012,
			GT_UserInfo_Query_Resp = 0x8012,
			
			//��ѯ�û���ϸ������Ϣ
			GT_User_DetailInfo_Query = 0x0013,
			GT_User_DetailInfo_Query_Resp = 0x8013,
			
			//��ѯ��ɫ������Ϣ
			GT_User_FamilyInfo_Query = 0x0014,
			GT_User_FamilyInfo_Query_Resp = 0x8014,
			
			//��ѯ��ɫ������Ϣ
			GT_User_SentiInfo_Query = 0x0015,
			GT_User_SentiInfo_Query_Resp = 0x8015,
			
			//��ѯ��ɫ�����Ϣ
			GT_User_MarrigeInfo_Query = 0x0016,
			GT_User_MarrigeInfo_Query_Resp = 0x8016,

			//��ѯ���б���ͣ�˺��б�
			GT_AllBanUser_Query = 0x0017,
			GT_AllBanUser_Query_Resp = 0x8017,

			//��ѯ������Ϣ
			GT_FamilyInfo_Query = 0x0018,
			GT_FamilyInfo_Query_Resp = 0x8018,
			
			//��ѯ�����Ա��Ϣ
			GT_MemberInfo_Query = 0x0019,
			GT_MemberInfo_Query_Resp = 0x8019,
			
			//��ѯ��ɫ������Ϣ
			GT_UserPack_Query = 0x0020,
			GT_UserPack_Query_Resp = 0x8020,
			
			//��ѯ��ɫװ����Ϣ
			GT_UserEquip_Query = 0x0021,
			GT_UserEquip_Query_Resp = 0x8021,
			
			//��ѯ���ﱳ����Ϣ
			GT_PetPack_Query = 0x0022,
			GT_PetPack_Query_Resp = 0x8022,


			//��ѯ��ҽ�����־
			GT_TradeLog_Query = 0x0023,
			GT_TradeLog_Query_Resp = 0x8023,
			
			//��ѯ����ʼ���־
			GT_MailLog_Query = 0x0024,
			GT_MailLog_Query_Resp = 0x8024,
			
			//��ѯ������������־
			GT_TaskLog_Query = 0x0025,
			GT_TaskLog_Query_Resp = 0x8025,
			
			//��ѯ���ʹ�õ�����־(�շѵ���)
			GT_CardLog_Query = 0x0026,
			GT_CardLog_Query_Resp = 0x8026,
			
			//��ѯ��ҹһ�������־
			GT_GuajiLog_Query = 0x0027,
			GT_GuajiLog_Query_Resp = 0x8027,
			
			//��ѯ��Ҵ�С������־
			GT_BugleLog_Query = 0x0028,
			GT_BugleLog_Query_Resp = 0x8028,
			
			//��ѯ�˺ŵ�½�ǳ���־
			GT_LoginLog_Query = 0x0029,
			GT_LoginLog_Query_Resp = 0x8029,



			//��ѯ���﷿������
			GT_RoomInfo_Query = 0x0030,
			GT_RoomInfo_Query_Resp = 0x8030,
			
			//��ѯ�����������
			GT_PetInfo_Query = 0x0031,
			GT_PetInfo_Query_Resp = 0x8031,
			
			//��ѯGT-IM����������Ϣ
			GT_IMFriend_Query = 0x0032,
			GT_IMFriend_Query_Resp = 0x8032,
			
			//��ѯGT-IM��������Ϣ
			GT_IMBlackList_Query = 0x0033,
			GT_IMBlackList_Query_Resp = 0x8033,
			
			//��ѯGT-IM����������Ϣ
			GT_IMSecretLove_Query = 0x0034,
			GT_IMSecretLove_Query_Resp = 0x8034,
			
			//��ѯ���俪����Ϣ
			GT_TreasureBox_Query = 0x0035,
			GT_TreasureBox_Query_Resp = 0x8035,
			
			//�ʼ���Ϣ��ѯ
			GT_MailInfo_Query = 0x0036,
			GT_MailInfo_Query_Resp = 0x8036,
			
			//��ֵ��¼��ѯ
			GT_ChargeInfo_Query = 0x0037,
			GT_ChargeInfo_Query_Resp = 0x8037,
			
			//���߹������Ѽ�¼
			GT_ItemConsume_Query = 0x0038,
			GT_ItemConsume_Query_Resp = 0x8038,


			//˲���ƶ�
			GT_Transmit_Query=0x0039,
			GT_Transmit_Query_Resp=0x8039,
			
			//��ֹ���������ϴ�ͼƬ
			GT_Picture_Stop=0x0040,
			GT_Picture_Stop_Resp=0x8040,

		
			//����Υ��ͼƬ
			GT_Picture_Clear=0x0043,
			GT_Picture_Clear_Resp=0x8043,


			//Ѫƴս�� T_gtown_Mcoin_Rank
			GT_Mcoin_Rank=0x0044,
			GT_Mcoin_Rank_Resp=0x8044,

			//�ռ����� T_gtown_sns_db
			GT_Sns_Query=0x0045,
			GT_Sns_Query_Resp=0x8045,

			//��������
			GT_Attr_Queue=0x0046,
			GT_Attr_Queue_Resp=0x8046,

			//��Ƭ���� T_gtown_card_record
			GT_Card_Record=0x0047,
			GT_Card_Record_Resp=0x8047,

			//���������н�ɫ��Ϣ
			GT_FriendTrade_Info=0x0048,
			GT_FriendTrade_Info_Resp=0x8048,

			//ū����Ϣ
			GT_HelotInfo_Query=0x0049,
			GT_HelotInfo_Query_Resp=0x8049,

			//����
			GT_FriendTrade_Queue=0x0050,
			GT_FriendTrade_Queue_Resp=0x8050,

			//����Log
			GT_FriendTrade_Log=0x0051,
			GT_FriendTrade_Log_Resp=0x8051,

			//��ո���ǩ��
			GT_ClearName_Query=0x0052,
			GT_ClearName_Query_Resp=0x8052,

			//��ѯ���б���ֹ�˺��б�
			GT_AllStopUser_Query = 0x0053,
			GT_AllStopUser_Query_Resp = 0x8053,

			//��ѯ����(��ԭ���ӿڱ�����ݿ�)
			GT_Select_Item=0x0054,
			GT_Select_Item_Resp=0x8054,


			//��ѯ�����˺��Ƿ񱻷�ͣ
			GT_BanUser_Query=0x0055,
			GT_BanUser_Query_Resp=0x8055,

			//������ӵ���
			GT_Import_Item=0x0056,
			GT_Import_Item_Resp=0x8056,

			//�˿�
			GT_Back_Money=0x0057,
			GT_Back_Money_Resp=0x8057,

			/////////<summary>
			///��ʿ��˫��Ϣ��(0x53)
			//////</summary>
			//��ѯ�û���Ϣ
			WA_Account_Query=0x0001,
			WA_Account_Query_Resp=0x8001,
			
			//ǿ���������
			WA_KickPlayer=0x0002,
			WA_KickPlayer_Resp=0x8002,
			
			//��ͣ�û�
			WA_ClosePlayer=0x0003,
			WA_ClosePlayer_Resp=0x8003,
			
			//����û�
			WA_OpenPlayer=0x0004,
			WA_OpenPlayer_Resp=0x8004,
			
			//��ͣ�б�
			WA_CloseList=0x0005,
			WA_CloseList_Resp=0x8005,
			
			//������ͣ
			WA_CloseSingle=0x0006,
			WA_CloseSingle_Resp=0x8006,
			
			//��ӹ���
			WA_BoardTasker_Add=0x0007,
			WA_BoardTasker_Add_Resp=0x8007,
			
			//ɾ������
			WA_BoardTasker_Del=0x0008,
			WA_BoardTasker_Del_Resp=0x8008,
			
			//��ѯ����
			WA_BoardTasker_Query=0x0009,
			WA_BoardTasker_Query_Resp=0x8009,
			
			//������Ϣ��ѯ
			WA_FriendInfo_Query=0x0010,
			WA_FriendInfo_Query_Resp=0x8010,

			//��ɫ��Ʒ��Ϣ��ѯ
			WA_BodyItemInfo_Query=0x0011,
			WA_BodyItemInfo_Query_Resp=0x8011,

			//��ɫ������Ϣ��ѯ
			WA_BagItemInfo_Query=0x0012,
			WA_BagItemInfo_Query_Resp=0x8012,

			//��ɫ������Ϣ��ѯ
			WA_BankItemInfo_Query=0x0013,
			WA_BankItemInfo_Query_Resp=0x8013,

			//������Ϣ��ѯ
			WA_MailInfo_Query=0x0014,
			WA_MailInfo_Query_Resp=0x8014,
			
			//������Ϣ��ѯ
			WA_EmblemInfo_Query=0x0015,
			WA_EmblemInfo_Query_Resp=0x8015,

			//������Ϣ��ѯ
			WA_PetInfo_Query=0x0016,
			WA_PetInfo_Query_Resp=0x8016,

			//�ʺŵ�¼��Ϣ��ѯ
			WA_AccountLoginInfo_Query=0x0017,
			WA_AccountLoginInfo_Query_Resp=0x8017,

			//��������
			WA_Item_Add=0x0018,
			WA_Item_Add_Resp=0x8018,

			//����ɾ��
			WA_Item_Del=0x0019,
			WA_Item_Del_Resp=0x8019,

			//��Ǯ�޸�
			WA_Money_Update=0x0020,
			WA_Money_Update_Resp=0x8020,

			//�����޸�
			WA_BoardTasker_Update=0x0021,
			WA_BoardTasker_Update_Resp=0x8021,

			//add by ltx
			//�޸Ľ�ɫ����
			WA_ModifyCharacterSkill=0x0022,
			WA_ModifyCharacterSkill_Resp=0x8022,

			//�ָ���ɫ
			WA_RecoverCharacter=0x0023,
			WA_RecoverCharacter_Resp=0x8023,

			//�޸�����
			WA_ModifyAccountPassw=0x0024,
			WA_ModifyAccountPassw_Resp=0x8024,

			//����ɫ�ʼ���Ʒ
			WA_MailItem=0x0025,
			WA_MailItem_Resp=0x8025,

			//��Ʒ:�������
			WA_Query_MonsterDropItem=0x0026,
			WA_Query_MonsterDropItem_Resp=0x8026,

			//��Ʒ:��NPC������
			WA_Query_BuyItemNPC=0x0027,
			WA_Query_BuyItemNPC_Resp=0x8027,
			
			//��Ʒ:����NPC
			WA_Query_SellItemTONPC=0x0028,
			WA_Query_SellItemTONPC_Resp=0x8028,

			//��Ʒ:�������ɾ��
			WA_Query_DropItem=0x0029,
			WA_Query_DropItem_Resp=0x8029,

			//��Ʒ:���׻��
			WA_Query_GetItemTrade=0x0030,
			WA_Query_GetItemTrade_Resp=0x8030,

			//��Ʒ:����ʧȥ
			WA_Query_LoseItemTrade=0x0031,
			WA_Query_LoseItemTrade_Resp=0x8031,

			//��Ʒ:װ���󶨼�¼
			WA_Query_BindItem=0x0032,
			WA_Query_BindItem_Resp=0x8032,

			//��Ʒ:װ����󶨼�¼
			WA_Query_UnBindItem=0x0033,
			WA_Query_UnBindItem_Resp=0x8033,

			//��Ʒ:��ɫ����̯����־��ѯ
			WA_Query_BoothItem=0x0034,
			WA_Query_BoothItem_Resp=0x8034,

			//��Ʒ:����ɾ����¼(ָʹ��ʱ�䵽��)
			WA_Query_DeleteItem=0x0035,
			WA_Query_DeleteItem_Resp=0x8035,

			//��Ʒ:����ʹ�ü�¼
			WA_Query_UseItem=0x0036,
			WA_Query_UseItem_Resp=0x8036,

			//��Ʒ:װ��������¼
			WA_Query_CraftItem=0x0037,
			WA_Query_CraftItem_Resp=0x8037,

			//��Ʒ:װ�����
			WA_Query_PunchItem=0x0038,
			WA_Query_PunchItem_Resp=0x8038,

			//��Ʒ:װ���Ŀ�
			WA_Query_ChangeSlotItem=0x0039,
			WA_Query_ChangeSlotItem_Resp=0x8039,

			//��Ʒ:װ������
			WA_Query_UpgradeStar=0x0040,
			WA_Query_UpgradeStar_Resp=0x8040,

			//��Ʒ:��ʯ
			WA_Query_Gem=0x0041,
			WA_Query_Gem_Resp=0x8041,

			//��Ʒ:��ҩ��¼
			WA_Query_Pharma=0x0042,
			WA_Query_Pharma_Resp=0x8042,

			//��Ʒ:��ʯ�ϳ�
			WA_Query_CraftGem=0x0043,
			WA_Query_CraftGem_Resp=0x8043,

			//��Ʒ:���Ϻϳ�
			WA_Query_CraftMatirail=0x0044,
			WA_Query_CraftMatirail_Resp=0x8044,

			//��Ʒ:ͼֽ�ϳ�
			WA_Query_CraftRecipe=0x0045,
			WA_Query_CraftRecipe_Resp=0x8045,

			//�Թ����ֶһ�����
			WA_Query_MazeIntegral=0x0046,
			WA_Query_MazeIntegral_Resp=0x8046,

			//��Ϸ��:�������
			WA_Query_MonsterDropMoney=0x0047,
			WA_Query_MonsterDropMoney_Resp=0x8047,

			//��Ϸ��:������
			WA_Query_GetMoneyQuest=0x0048,
			WA_Query_GetMoneyQuest_Resp=0x8048,

			//��Ϸ��:��װ
			WA_Query_LoseMoneyRepair=0x0049,
			WA_Query_LoseMoneyRepair_Resp=0x8049,

			//��Ϸ��:���׻��
			WA_Query_GetMoneyTrade=0x0050,
			WA_Query_GetMoneyTrade_Resp=0x8050,

			//��Ϸ��:����ʧȥ
			WA_Query_LoseMoneyTrade=0x0051,
			WA_Query_LoseMoneyTrade_Resp=0x8051,

			//����:��ɻ�ü�¼
			WA_Query_QuestReward=0x0052,
			WA_Query_QuestReward_Resp=0x8052,

			//����:������Ʒɾ����¼
			WA_Query_QuestGive=0x0053,
			WA_Query_QuestGive_Resp=0x8053,

			//�ʼ�:���ͼ�¼
			WA_Query_MailSend=0x0054,
			WA_Query_MailSend_Resp=0x8054,

			//�ʼ�:���ռ�¼
			WA_Query_MailRecv=0x0055,
			WA_Query_MailRecv_Resp=0x8055,

			//�ʼ�:ɾ����¼
			WA_Query_MailDelete=0x0056,
			WA_Query_MailDelete_Resp=0x8056,

			//�̳�:�����¼
			WA_Query_ItemShop=0x0057,
			WA_Query_ItemShop_Resp=0x8057,

			//��ѯ�û��ĸ�����־
			WA_Query_Instance=0x0058,
			WA_Query_Instance_Resp=0x8058,

			//��ҵ�½/�ǳ���־
			WA_Query_LoginLogout=0x0059,
			WA_Query_LoginLogout_Resp=0x8059,
			
			//��ҵ�½/�ǳ���־(��ip��ʽ��ѯ)
			WA_Query_LoginLogoutByIP=0x0112,
			WA_Query_LoginLogoutByIP_Resp=0x8112,

			//�ָ��������
			WA_Query_ResumePassw=0x0113,
			WA_Query_ResumePassw_Resp=0x8113,

			//�����б��ѯ
			WA_ItemList_Query=0x0060,
			WA_ItemList_Query_Resp=0x8060,

			//�����б��ѯ
			WA_SkillList_Query=0x0061,
			WA_SkillList_Query_Resp=0x8061,

			//�޸Ľ�ɫ��
			WA_PlayerRole_Modfiy=0x0062,
			WA_PlayerRole_Modfiy_Resp=0x8062,
			
			//�޸Ľ�ɫ����ֵ
			WA_RoleInfo_Modfiy=0x0063,
			WA_RoleInfo_Modfiy_Resp=0x8063,

			//��ѯ������ϼ�����Ϣ
			WA_RoleSkill_Query=0x0064,
			WA_RoleSkill_Query_Resp=0x8064,
			
			//��ѯ��ҵ������Ͳ�ѯ
			WA_ItemType_Query=0x0065,
			WA_ItemType_Query_Resp=0x8065,

			WA_UserPwd_Recover=0x0066,
			WA_UserPwd_Recover_Resp=0x8066,

			//Added by tengjie 2010-06-04:For ��ɫ��־
			//��ɫ������¼��ѯ
			WA_Character_Upgrade=0x0067,
			WA_Character_Upgrade_Resp=0x8067,

			//��ɫ������¼��ѯ
			WA_Character_Create=0x0068,
			WA_Character_Create_Resp=0x8068,

			//��ɫɾ����¼��ѯ
			WA_Character_Delete=0x0069,
			WA_Character_Delete_Resp=0x8069,

			//������
			WA_Join_Guild=0x0070,
			WA_Join_Guild_Resp=0x8070,

			//�뿪���
			WA_Quit_Guild=0x0071,
			WA_Quit_Guild_Resp=0x8071,

			//�������
			WA_Create_Guild=0x0072,
			WA_Create_Guild_Resp=0x8072,

			//��ɢ���
			WA_Dissolve_Guild=0x0073,
			WA_Dissolve_Guild_Resp=0x8073,

			//תְ��Ϣ��ѯ
			WA_Transfer_Info=0x0074,
			WA_Transfer_Info_Resp=0x8074,
			
			//������־��¼
			WA_Skills_Log=0x0075,
			WA_Skills_Log_Resp=0x8075,
			
			//�������־��¼
			WA_LifeSkills_Log=0x0076,
			WA_LifeSkills_Log_Resp=0x8076,

			//Ӷ������־
			//������¼
			WA_Create_MercenaryGroup=0x0077,
			WA_Create_MercenaryGroup_Resp=0x8077,

			//��ɢ��¼
			WA_Dissolve_MercenaryGroup=0x0078,
			WA_Dissolve_MercenaryGroup_Resp=0x8078,

			//���������־
			//���»��ʱ��
			WA_GainBadge_Time=0x0079,
			WA_GainBadge_Time_Resp=0x8079,

			//���¶���ʱ��
			WA_DiscardBadge_Time=0x0080,
			WA_DiscardBadge_Time_Resp=0x8080,

			//���¶һ���¼
			WA_ExchangeBadge_Record=0x0081,
			WA_ExchangeBadge_Record_Resp=0x8081,

			//���¼���
			WA_Identify_Badge=0x0082,
			WA_Identify_Badge_Resp=0x8082,
			
			//���ﾫ�겶׽��¼
			WA_CatchPetSpirit_Record=0x0083,
			WA_CatchPetSpirit_Record_Resp=0x8083,
			
			//��ӡ�����¼
			WA_SealSpirit_Record=0x0084,
			WA_SealSpirit_Record_Resp=0x8084,

			//����������¼
			WA_CultureBadge_Record=0x0085,
			WA_CultureBadge_Record_Resp=0x8085,
			
			//���꽻�׼�¼
			//���꽻�׻��
			WA_GainSpirit_Record=0x0086,
			WA_GainSpirit_Record_Resp=0x8086,
			
			//���꽻��ʧȥ
			WA_LoseSpirit_Record=0x0087,
			WA_LoseSpirit_Record_Resp=0x8087,

			//������ۼ�¼
			WA_SellSpirit_Record=0x0088,
			WA_SellSpirit_Record_Resp=0x8088,

			//�û���Ϣ��ѯ
			//������Ϣ��ѯ
			WA_SkillsInfo_Query=0x0089,
			WA_SkillsInfo_Query_Resp=0x8089,

			//������Ϣ��ѯ:���������
			WA_MissionsCompleted_Query=0x0090,
			WA_MissionsCompleted_Query_Resp=0x8090,

			//������Ϣ��ѯ:�ѽ�����
			WA_MissionsReceived_Query=0x0091,
			WA_MissionsReceived_Query_Resp=0x8091,

			//Ӷ������Ϣ��ѯ
			//Ӷ������Ϣ��ѯ
			WA_MercenaryGroup_Info_Query=0x0092,
			WA_MercenaryGroup_Info_Query_Resp=0x8092,

			//Ӷ���ų�Ա�б��ѯ
			WA_MemberList_Query=0x0093,
			WA_MemberList_Query_Resp=0x8093,

			//Ӷ���Ųֿ���Ϣ
			WA_WarehouseInfo_Query=0x0094,
			WA_WarehouseInfo_Query_Resp=0x8094,

			//��ȡӶ����нˮ��Ϣ
			WA_Receive_SalaryInfo_Query=0x0095,
			WA_Receive_SalaryInfo_Query_Resp=0x8095,

			//��ȡӶ���ž�����Ϣ
			WA_Receive_ExperienceInfo_Query=0x0096,
			WA_Receive_ExperienceInfo_Query_Resp=0x8096,

			//���а��ѯ
			//�ȼ�����
			WA_Rank_Query=0x0097,
			WA_Rank_Query_Resp=0x8097,

			//��������
			WA_Sports_Query=0x0098,
			WA_Sports_Query_Resp=0x8098,

			//Ӷ����������
			WA_Prestige_Query=0x0099,
			WA_Prestige_Query_Resp=0x8099,

			//������������
			WA_CopyIntegral_Query=0x0100,
			WA_CopyIntegral_Query_Resp=0x8100,

			//Ӷ��������
			WA_MercenaryGroup_Query=0x0101,
			WA_MercenaryGroup_Query_Resp=0x8101,

			//������:
			//�������:��ӳ���(�ȼ�\����)
			WA_PetAdded_Operate=0x0102,
			WA_PetAdded_Operate_Resp=0x8102,
			
			//�������:ɾ������(�ȼ�\����)
			WA_PetDeleted_Operate=0x0103,
			WA_PetDeleted_Operate_Resp=0x8103,
			
			//�������
			WA_AttornPopedom_Operate=0x0104,
			WA_AttornPopedom_Operate_Resp=0x8104,
			
			//Ӷ���Ź���:�޸Ĺ�����Ϣ
			WA_ModifyGuildName_Operate=0x0105,
			WA_ModifyGuildName_Operate_Resp=0x8105,
			
			//Ӷ���Ź���:�߳������Ա
			WA_KickGuildMember_Operate=0x0106,
			WA_KickGuildMember_Operate_Resp=0x8106,
			
			//Ӷ���Ź���:��ɢ����
			WA_DissolveGuild_Operate=0x0107,
			WA_DissolveGuild_Operate_Resp=0x8107,
			
			//Ӷ���Ųֿ�:ɾ����Ʒ
			WA_DeleteGoods_Operate=0x0108,
			WA_DeleteGoods_Operate_Resp=0x8108,
			
			//Ӷ���Ųֿ�:�����Ʒ
			WA_AddGoods_Operate=0x0109,
			WA_AddGoods_Operate_Resp=0x8109,
			
			//Ӷ���Ųֿ�:�޸Ľ�Ǯ����
			WA_ModifyMoney_Operate=0x0110,
			WA_ModifyMoney_Operate_Resp=0x8110,

			//�������:�����б��ѯ
			WA_PetList_Query=0x0111,
			WA_PetList_Query_Resp=0x8111,

			//�ָ���ɫ����
			WA_ResumeAttribute_Op=0x0114,
			WA_ResumeAttribute_Op_Resp=0x8114,

			//���������ʼ�(�޵���)
			WA_SendMails_Op=0x0115,
			WA_SendMails_Op_Resp=0x8115,


			//������ͣ�û�
			WA_ClosePlayerGroup=0x0116,
			WA_ClosePlayerGroup_Resp=0x8116,
			//End

			/////////<summary>
			///�����ɳ���Ϣ��(0x54)
			//////</summary>
			//��ѯ�û���Ϣ
			CF_Account_Query=0x0001,
			CF_Account_Query_Resp=0x8001,
			
			//ǿ���������
			CF_KickPlayer=0x0002,
			CF_KickPlayer_Resp=0x8002,
			
			//��ͣ�û�
			CF_ClosePlayer=0x0003,
			CF_ClosePlayer_Resp=0x8003,
			
			//����û�
			CF_OpenPlayer=0x0004,
			CF_OpenPlayer_Resp=0x8004,
			
			//��ͣ�б�
			CF_CloseList=0x0005,
			CF_CloseList_Resp=0x8005,
			
			//������ͣ
			CF_CloseSingle=0x0006,
			CF_CloseSingle_Resp=0x8006,
			
			//��ӹ���
			CF_BoardTasker_Add=0x0007,
			CF_BoardTasker_Add_Resp=0x8007,
			
			//ɾ������
			CF_BoardTasker_Del=0x0008,
			CF_BoardTasker_Del_Resp=0x8008,
			
			//��ѯ����
			CF_BoardTasker_Query=0x0009,
			CF_BoardTasker_Query_Resp=0x8009,

			/////////<summary>
			///���������Ϣ��(0x55)
			//////</summary>
			//��ѯ�û���Ϣ
			XDJG_Account_Query=0x0001,
			XDJG_Account_Query_Resp=0x8001,
			
			//ǿ���������
			XDJG_KickPlayer=0x0002,
			XDJG_KickPlayer_Resp=0x8002,
			
			//��ͣ�û�
			XDJG_ClosePlayer=0x0003,
			XDJG_ClosePlayer_Resp=0x8003,
			
			//����û�
			XDJG_OpenPlayer=0x0004,
			XDJG_OpenPlayer_Resp=0x8004,
			
			//��ͣ�б�
			XDJG_CloseList=0x0005,
			XDJG_CloseList_Resp=0x8005,
			
			//������ͣ
			XDJG_CloseSingle=0x0006,
			XDJG_CloseSingle_Resp=0x8006,
			
			//��ӹ���
			XDJG_BoardTasker_Add=0x0007,
			XDJG_BoardTasker_Add_Resp=0x8007,
			
			//�޸Ĺ���
			XDJG_BoardTasker_Update=0x0008,
			XDJG_BoardTasker_Update_Resp=0x8008,
			
			//ɾ������
			XDJG_BoardTasker_Del=0x0009,
			XDJG_BoardTasker_Del_Resp=0x8009,
			
			//��ѯ����
			XDJG_BoardTasker_Query=0x0010,
			XDJG_BoardTasker_Query_Resp=0x8010,

			//Added by tengjie 2010-07-06
			//��ɫ�������
			XDJG_RoleSkillAdd=0x0011,
			XDJG_RoleSkillAdd_Resp=0x8011,
			
			//��ɫ����ɾ��
			XDJG_RoleSkillDel=0x0012,
			XDJG_RoleSkillDel_Resp=0x8012,
			
			//�������
			XDJG_ItemAdd=0x0013,
			XDJG_ItemAdd_Resp=0x8013,
			
			//����ɾ��
			XDJG_ItemDel=0x0014,
			XDJG_ItemDel_Resp=0x8014,
			
			//�����Ϸ��
			XDJG_MoneyAdd=0x0015,
			XDJG_MoneyAdd_Resp=0x8015,

			//��־��:��ҵ�¼��־
			XDJG_Login_Log=0x0016,
			XDJG_Login_Log_Resp=0x8016,
			
			//�̳���Ʒ������־
			XDJG_Shoping_Log=0x0017,
			XDJG_Shoping_Log_Resp=0x8017,
			
			//�����Ʒ��־:���׻��
			XDJG_TransGain_Log=0x0018,
			XDJG_TransGain_Log_Resp=0x8018,
			
			//�����Ʒ��־:����ʧȥ
			XDJG_TransLose_Log=0x0019,
			XDJG_TransLose_Log_Resp=0x8019,
			
			//�����Ʒ��־:��NPC������
			XDJG_BuyFromNpc_Log=0x0020,
			XDJG_BuyFromNpc_Log_Resp=0x8020,
			
			//�����Ʒ��־:����NPC
			XDJG_SaleToNpc_Log=0x0021,
			XDJG_SaleToNpc_Log_Resp=0x8021,
			
			//�����Ʒ��־:ʰȡ��Ʒ
			XDJG_PickUpItem_Log=0x0022,
			XDJG_PickUpItem_Log_Resp=0x8022,
			
			//�����Ʒ��־:������Ʒ
			XDJG_DiscardItem_Log=0x0023,
			XDJG_DiscardItem_Log_Resp=0x8023,
			
			//�����Ʒ��־:��Ʒ����
			XDJG_ConsumeItem_Log=0x0024,
			XDJG_ConsumeItem_Log_Resp=0x8024,
			
			//�����Ʒ��־:����������
			XDJG_AuctioneerSaled_Log=0x0025,
			XDJG_AuctioneerSaled_Log_Resp=0x8025,
			
			//�����Ʒ��־:�����й���
			XDJG_AuctioneerBought_Log=0x0026,
			XDJG_AuctioneerBought_Log_Resp=0x8026,
			
			//����ʼ���־:�����ʼ�
			XDJG_SendMail_Log=0x0027,
			XDJG_SendMail_Log_Resp=0x8027,
			
			//����ʼ���־:��ȡ�ʼ�
			XDJG_ReceiveMail_Log=0x0028,
			XDJG_ReceiveMail_Log_Resp=0x8028,
			
			//M��ʹ����־:�����������Ѽ�¼
			XDJG_ExpansionPack_Log=0x0029,
			XDJG_ExpansionPack_Log_Resp=0x8029,
			
			//M��ʹ����־:������־
			XDJG_Deliver_Log=0x0030,
			XDJG_Deliver_Log_Resp=0x8030,
			
			//M��ʹ����־:��ɫ�ֿ��������Ѽ�¼
			XDJG_ExpansionWarehouse_Log=0x0031,
			XDJG_ExpansionWarehouse_Log_Resp=0x8031,
			
			//��ҽ�ɫ��־:��ɫ����
			XDJG_RoleUpgrade_Log=0x0032,
			XDJG_RoleUpgrade_Log_Resp=0x8032,
			
			//��ҽ�ɫ��־:��ɫ����
			XDJG_RoleCreate_Log=0x0033,
			XDJG_RoleCreate_Log_Resp=0x8033,
			
			//��ҽ�ɫ��־:��ɫ����ѧϰ
			XDJG_RoleSkill_Log=0x0034,
			XDJG_RoleSkill_Log_Resp=0x8034,
			
			//��ҽ�ɫ��־:��ȡ����
			XDJG_ReceiveTask_Log=0x0035,
			XDJG_ReceiveTask_Log_Resp=0x8035,
			
			//��ҽ�ɫ��־:�������
			XDJG_CompleteTask_Log=0x0036,
			XDJG_CompleteTask_Log_Resp=0x8036,
			
			//��ҽ�ɫ��־:������ѯ
			XDJG_Copy_Log=0x0037,
			XDJG_Copy_Log_Resp=0x8037,

			//��ѯ��
			//�����Ʒ��Ϣ:������Ʒ��Ϣ
			XDJG_ItemBody_Query=0x0038,
			XDJG_ItemBody_Query_Resp=0x8038,
			
			//�����Ʒ��Ϣ:������Ʒ��Ϣ
			XDJG_Itembackpack_Query=0x0039,
			XDJG_Itembackpack_Query_Resp=0x8039,
			
			//�����Ʒ��Ϣ:�ֿ���Ʒ��Ϣ
			XDJG_ItemStorage_Query=0x0040,
			XDJG_ItemStorage_Query_Resp=0x8040,
			
			//�����Ʒ��Ϣ:����ֿ���Ϣ
			XDJG_GuildStorage_Query=0x0041,
			XDJG_GuildStorage_Query_Resp=0x8041,
			
			//�����Ʒ��Ϣ:��ղֿ���Ϣ
			XDJG_KingkongStorage_Query=0x0042,
			XDJG_KingkongStorage_Query_Resp=0x8042,

			//�ʼ���Ϣ
			XDJG_MailInfo_Query=0x0043,
			XDJG_MailInfo_Query_Resp=0x8043,
			
			//��������Ϣ
			XDJG_AuctioneerInfo_Query=0x0044,
			XDJG_AuctioneerInfo_Query_Resp=0x8044,

			//������
			//�û���ɫ��Ϣ���޸�:�޸ĵȼ�
			XDJG_ModifyLevel_operate=0x0045,
			XDJG_ModifyLevel_operate_Resp=0x8045,
			
			//�û��������:�޸�����
			XDJG_ModifyPwd_operate=0x0046,
			XDJG_ModifyPwd_operate_Resp=0x8046,
			
			//�û��������:��ʱ����ָ�
			XDJG_ResumePwd_operate=0x0047,
			XDJG_ResumePwd_operate_Resp=0x8047,
			
			//�û���ɫ����:��ɫ�ָ�
			XDJG_ResumeRole_operate=0x0048,
			XDJG_ResumeRole_operate_Resp=0x8048,
			
			//�û���ɫ����:��ɫɾ��
			XDJG_DeleteRole_operate=0x0049,
			XDJG_DeleteRole_operate_Resp=0x8049,

			//�޸Ľ�ɫ���Ͻ�Ǯ����(�󶨱�)
			XDJG_UpRoleBind_operate=0x0050,
			XDJG_UpRoleBind_operate_Resp=0x8050,
			
			//�޸Ľ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)
			XDJG_UpRoleNonBind_operate=0x0051,
			XDJG_UpRoleNonBind_operate_Resp=0x8051,

			//��ɫ������Ϣ��ѯ
			XDJG_RoleSkills_Query=0x0052,
			XDJG_RoleSkills_Query_Resp=0x8052,
			
			//�����б��ѯ
			XDJG_SkillList_Query=0x0053,
			XDJG_SkillList_Query_Resp=0x8053,
			
			//�������Ͳ�ѯ
			XDJG_ItemType_Query=0x0054,
			XDJG_ItemType_Query_Resp=0x8054,
			
			//��Ӧ���͵ĵ����б��ѯ
			XDJG_ItemNameLst_Query=0x0055,
			XDJG_ItemNameLst_Query_Resp=0x8055,

			//��Ϸ�Ҳ�ѯ
			XDJG_MoneyQuery=0x0056,
			XDJG_MoneyQuery_Resp=0x8056,

			//��ӽ�ɫ���Ͻ�Ǯ����(�󶨱�)
			XDJG_AddRoleBind_operate=0x0057,
			XDJG_AddRoleBind_operate_Resp=0x8057,
			
			//��ӽ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)
			XDJG_AddRoleNonBind_operate=0x0058,
			XDJG_AddRoleNonBind_operate_Resp=0x8058,

			//��Ʒ���Բ�ѯ
			XDJG_ItemAttribute_Query=0x0059,
			XDJG_ItemAttribute_Query_Resp=0x8059,

			//������Ϣ��ѯ
			//���������Ϣ
			XDJG_GuildBaseInfo_Query=0x0060,
			XDJG_GuildBaseInfo_Query_Resp=0x8060,
			
			//�����Ա�б�
			XDJG_GuildMemberList_Query=0x0061,
			XDJG_GuildMemberList_Query_Resp=0x8061,
			
			//��������ѯ
			XDJG_PlayerTaskInfo_Query=0x0062,
			XDJG_PlayerTaskInfo_Query_Resp=0x8062,
			
			//��ճ�����Ϣ
			//��ճ���������Ϣ
			XDJG_CarBaseInfo_Query=0x0063,
			XDJG_CarBaseInfo_Query_Resp=0x8063,
			
			//��ճ�����ϸ��Ϣ
			XDJG_CarDetailInfo_Query=0x0064,
			XDJG_CarDetailInfo_Query_Resp=0x8064,
			
			//��ҹ�����Ϣ
			//��ҹ��������Ϣ
			XDJG_PlayerGuildBaseInfo_Query=0x0065,
			XDJG_PlayerGuildBaseInfo_Query_Resp=0x8065,
			
			//��ҹ�����Ʒ��Ϣ
			XDJG_PlayerGuildItemInfo_Query=0x0066,
			XDJG_PlayerGuildItemInfo_Query_Resp=0x8066,
			
			//�����Ʒ��־
			//װ��������־
			XDJG_EquipIncrease_Log=0x0067,
			XDJG_EquipIncrease_Log_Resp=0x8067,
			
			//��Ƭ�ϳ���־
			XDJG_Synthesis_Log=0x0068,
			XDJG_Synthesis_Log_Resp=0x8068,
			
			//װ��������־
			XDJG_EquipTransfer_Log=0x0069,
			XDJG_EquipTransfer_Log_Resp=0x8069,
			
			//��Ʒ����
			XDJG_ItemIdentify_Log=0x0070,
			XDJG_ItemIdentify_Log_Resp=0x8070,
			
			//���˲ֿ�����־
			XDJG_RoleStore_Log=0x0071,
			XDJG_RoleStore_Log_Resp=0x8071,
			
			//����ֿ�����־
			XDJG_GuildStore_Log=0x0072,
			XDJG_GuildStore_Log_Resp=0x8072,
			
			//������־
			//�������
			XDJG_GuildCreate_Log=0x0073,
			XDJG_GuildCreate_Log_Resp=0x8073,
			
			//�����ɢ
			XDJG_GuildDismiss_Log=0x0074,
			XDJG_GuildDismiss_Log_Resp=0x8074,
			
			//������ӳ�Ա
			XDJG_GuildAddMember_Log=0x0075,
			XDJG_GuildAddMember_Log_Resp=0x8075,
			
			//�����Ա�뿪
			XDJG_GuildMemberLeaved_Log=0x0076,
			XDJG_GuildMemberLeaved_Log_Resp=0x8076,
			
			//����ְ����
			XDJG_GuildPostChanged_Log=0x0077,
			XDJG_GuildPostChanged_Log_Resp=0x8077,

			//�ʺ��Ƿ��ͣ
			XDJG_AccountIsClose_Query=0x0078,
			XDJG_AccountIsClose_Query_Log_Resp=0x8078,

			//NPC������ȡ��ѯ
			XDJG_NPCGetItem_Query=0x0079,
			XDJG_NPCGetItem_Query_Resp=0x8079,

			//��ӳƺ�
			XDJG_AddTitle_Query=0x0080,
			XDJG_AddTitle_Query_Resp=0x8080,
			
			//�Ƴ��ƺ�
			XDJG_RemoveTitle_Query=0x0081,
			XDJG_RemoveTitle_Query_Resp=0x8081,
			
			//���BUFF
			XDJG_AddBuff_Query=0x0082,
			XDJG_AddBuff_Query_Resp=0x8082,
			
			//�Ƴ�BUFF
			XDJG_RemoveBuff_Query=0x0083,
			XDJG_RemoveBuff_Query_Resp=0x8083,
			
			//�ƶ���ָ��λ��
			XDJG_MoveToPosition_Query=0x0084,
			XDJG_MoveToPosition_Query_Resp=0x8084,
			
			//�ƶ���ָ��������
			XDJG_MoveToRole_Query=0x0085,
			XDJG_MoveToRole_Query_Resp=0x8085,
			
			//�ƶ���Ŀ��������
			XDJG_MoveToTargetRole_Query=0x0086,
			XDJG_MoveToTargetRole_Query_Resp=0x8086,

			//�ƺ��б��ѯ
			XDJG_TitleList_Query=0x0087,
			XDJG_TitleList_Query_Resp=0x8087,
			
			//BUFF�б��ѯ
			XDJG_BuffList_Query=0x0088,
			XDJG_BuffList_Query_Resp=0x8088,

			//End

			/// <summary>
			/// JW2_ADMIN �����Ź��߲�����Ϣ��
			/// </summary>

			JW2_ACCOUNT_QUERY = 0x0001,//����ʺ���Ϣ��ѯ��1.2.3.4.5.8��
			JW2_ACCOUNT_QUERY_RESP = 0x8001,//����ʺ���Ϣ��ѯ��Ӧ��1.2.3.4.5.8��
			JW2_ACCOUNTREMOTE_QUERY = 0x0002,//��Ϸ��������ͣ������ʺŲ�ѯ
			JW2_ACCOUNTREMOTE_QUERY_RESP = 0x8002,//��Ϸ��������ͣ������ʺŲ�ѯ��Ӧ
			JW2_ACCOUNTLOCAL_QUERY = 0x0003,//���ط�ͣ������ʺŲ�ѯ
			JW2_ACCOUNTLOCAL_QUERY_RESP = 0x8003,//���ط�ͣ������ʺŲ�ѯ��Ӧ
			JW2_ACCOUNT_CLOSE = 0x0004,//��ͣ������ʺ�
			JW2_ACCOUNT_CLOSE_RESP = 0x8004,//��ͣ������ʺ���Ӧ
			JW2_ACCOUNT_OPEN = 0x0005,//��������ʺ�
			JW2_ACCOUNT_OPEN_RESP = 0x8005,//��������ʺ���Ӧ
			JW2_ACCOUNT_BANISHMENT_QUERY = 0x0006,//��ҷ�ͣ�ʺŲ�ѯ
			JW2_ACCOUNT_BANISHMENT_QUERY_RESP = 0x8006,//��ҷ�ͣ�ʺŲ�ѯ��Ӧ
			JW2_BANISHPLAYER=0x0007,//����
			JW2_BANISHPLAYER_RESP=0x8007,//������Ӧ
			JW2_BOARDMESSAGE=0x0008,//����
			JW2_BOARDMESSAGE_RESP=0x8008,//������Ӧ
			
			JW2_LOGINOUT_QUERY=0x0009,//����������/�ǳ���Ϣ
			JW2_LOGINOUT_QUERY_RESP=0x8009,//����������/�ǳ���Ϣ��Ӧ

			JW2_RPG_QUERY=0x0010,//��ѯ������ڣ�6��
			JW2_RPG_QUERY_RESP=0x8010,//��ѯ���������Ӧ��6��
					
			JW2_ITEMSHOP_BYOWNER_QUERY = 0x0011,////�鿴������ϵ�����Ϣ��7��7��
			JW2_ITEMSHOP_BYOWNER_QUERY_RESP = 0x8011,////�鿴������ϵ�����Ϣ��Ӧ��7��7��
			
			
			JW2_DUMMONEY_QUERY = 0x0012,////��ѯ����������ң�7��8��
			JW2_DUMMONEY_QUERY_RESP = 0x8012,////��ѯ�������������Ӧ��7��8��
			JW2_HOME_ITEM_QUERY = 0x0013,////�鿴������Ʒ�嵥�����ޣ�7��9��
			JW2_HOME_ITEM_QUERY_RESP = 0x8013,////�鿴������Ʒ�嵥��������Ӧ��7��9��
			JW2_SMALL_PRESENT_QUERY = 0x0014,////�鿴�������7��10��
			JW2_SMALL_PRESENT_QUERY_RESP = 0x8014,////�鿴���������Ӧ��7��10��
			JW2_WASTE_ITEM_QUERY = 0x0015,////�鿴�����Ե��ߣ�7��11��
			JW2_WASTE_ITEM_QUERY_RESP = 0x8015,////�鿴�����Ե�����Ӧ��7��11��
			JW2_SMALL_BUGLE_QUERY = 0x0016,////�鿴С���ȣ�7��12��
			JW2_SMALL_BUGLE_QUERY_RESP  = 0x8016,////�鿴С������Ӧ��7��12��
			
			
			
			JW2_WEDDINGINFO_QUERY=0x0017,//������Ϣ
			JW2_WEDDINGINFO_QUERY_RESP=0x8017,
			JW2_WEDDINGLOG_QUERY=0x0018,//������ʷ
			JW2_WEDDINGLOG_QUERY_RESP=0x8018,
			JW2_WEDDINGGROUND_QUERY=0x0019,//�����Ϣ
			JW2_WEDDINGGROUND_QUERY_RESP=0x8019,
			JW2_COUPLEINFO_QUERY=0x0020,//������Ϣ
			JW2_COUPLEINFO_QUERY_RESP=0x8020,
			JW2_COUPLELOG_QUERY=0x0021,//������ʷ
			JW2_COUPLELOG_QUERY_RESP=0x8021,
			
		
			JW2_FAMILYINFO_QUERY=0x0022,//������Ϣ
			JW2_FAMILYINFO_QUERY_RESP=0x8022,
			JW2_FAMILYMEMBER_QUERY=0x0023,//�����Ա��Ϣ
			JW2_FAMILYMEMBER_QUERY_RESP=0x8023,
			JW2_SHOP_QUERY=0x0024,//�̳���Ϣ
			JW2_SHOP_QUERY_RESP=0x8024,

			
			JW2_User_Family_Query=0x0025,//�û�������Ϣ
			JW2_User_Family_Query_Resp=0x8025,

			JW2_FamilyItemInfo_Query=0x0026,//���������Ϣ
			JW2_FamilyItemInfo_Query_Resp=0x8026,
			
			JW2_FamilyBuyLog_Query=0x0027,//���幺����־
			JW2_FamilyBuyLog_Query_Resp=0x8027,
			
			JW2_FamilyTransfer_Query=0x0028,//����ת����Ϣ
			JW2_FamilyTransfer_Query_Resp=0x8028,

			JW2_FriendList_Query=0x0029,//�����б�
			JW2_FriendList_Query_Resp=0x8029,


			JW2_BasicInfo_Query=0x0030,//������Ϣ��ѯ
			JW2_BasicInfo_Query_Resp=0x8030,

			JW2_FamilyMember_Applying=0x0031,//�����г�Ա
			JW2_FamilyMember_Applying_Resp=0x8031,
			
			JW2_BasicRank_Query=0x0032,//���صȼ�
			JW2_BasicBank_Query_Resp=0x8032,

			///////////����////////////////////////////
			JW2_BOARDTASK_INSERT = 0x0033,//�������
			JW2_BOARDTASK_INSERT_RESP = 0x8033,//���������Ӧ
			JW2_BOARDTASK_QUERY = 0x0034,//�����ѯ
			JW2_BOARDTASK_QUERY_RESP = 0x8034,//�����ѯ��Ӧ
			JW2_BOARDTASK_UPDATE = 0x0035,//�������
			JW2_BOARDTASK_UPDATE_RESP = 0x8035,//���������Ӧ

			JW2_ITEM_DEL=0x0036,//����ɾ��������0����Ʒ��1�����2��
			JW2_ITEM_DEL_RESP=0x8036,//����ɾ��������0����Ʒ��1�����2��

			JW2_MODIFYSEX_QUREY=0x0037,//�޸Ľ�ɫ�Ա�
			JW2_MODIFYSEX_QUEYR_RESP=0x8037,

			JW2_MODIFYLEVEL_QUERY=0x0038,//�޸ĵȼ�
			JW2_MODIFYLEVEL_QUERY_RESP=0x8038,

			JW2_MODIFYEXP_QUERY=0x0039,//�޸ľ���
			JW2_MODIFYEXP_QUERY_RESP=0x8039,
			
			JW2_BAN_BIGBUGLE=0x0040,//���ô�����
			JW2_BAN_BIGBUGLE_RESP=0x8040,

			JW2_BAN_SMALLBUGLE=0x0041,//����С����
			JW2_BAN_SMALLBUGLE_RESP=0x8041,

			JW2_DEL_CHARACTER=0x0042,//ɾ����ɫ
			JW2_DEL_CHARACTER_RESP=0x8042,
			
			JW2_RECALL_CHARACTER=0x0043,//�ָ���ɫ
			JW2_RECALL_CHARACTER_RESP=0x8043,
			
			JW2_MODIFY_MONEY=0x0044,//�޸Ľ�Ǯ
			JW2_MODIFY_MONEY_RESP=0x8044,

			JW2_ADD_ITEM=0x0045,//���ӵ���
			JW2_ADD_ITEM_RESP=0x8045,

			JW2_CREATE_GM=0x0046,//ÿ����������GM
			JW2_CREATE_GM_RESP=0x8046,

			JW2_MODIFY_PWD=0x0047,//�޸�����
			JW2_MODIFY_PWD_RESP=0x8047,

			JW2_RECALL_PWD=0x0048,//�ָ�����
			JW2_RECALL_PWD_RESP=0x8048,

			JW2_ItemInfo_Query=0x0049,//���߲�ѯ
			JW2_ItemInfo_Query_Resp=0x8049,//

			
			JW2_ITEM_SELECT=0x0050,//����ģ����ѯ
			JW2_ITEM_SELECT_RESP=0x8050,//

			JW2_PetInfo_Query=0x0051,//������Ϣ
			JW2_PetInfo_Query_Resp=0x8051,
		
			JW2_Messenger_Query=0x0052,//�ƺŲ�ѯ
			JW2_Messenger_Query_Resp=0x8052,

			JW2_Wedding_Paper=0x0053,//���֤��
			JW2_Wedding_Paper_Resp=0x8053,

			JW2_CoupleParty_Card=0x0054,//�����ɶԿ�
			JW2_CoupleParty_Card_Resp=0x8054,


			JW2_MailInfo_Query=0x0055,//ֽ����Ϣ
			JW2_MailInfo_Query_Resp=0x8055,

			JW2_MoneyLog_Query=0x0056,//��Ǯ��־��ѯ
			JW2_MoneyLog_Query_Resp=0x8056,

			JW2_FamilyFund_Log=0x0057,//������־
			JW2_FamilyFund_Log_Resp=0x8057,

			JW2_FamilyItem_Log=0x0058,//���������־
			JW2_FamilyItem_Log_Resp=0x8058,

			JW2_Item_Log=0x0059,//������־
			JW2_Item_Log_Resp=0x8059,

			JW2_Task_Log=0x0060,//������־
			JW2_Task_Log_Resp=0x8060,

			JW2_CashMoney_Log=0x0061,//������־
			JW2_CashMoney_Log_Resp=0x8061,


			JW2_Import_KickPlayer=0x0062,//��������
			JW2_Import_KickPlayer_Resp=0x8062,
			
			JW2_Import_AccountClose=0x0063,//������ͣ
			JW2_Import_AccountClose_Resp=0x8063,
			
			JW2_Import_AccountOpen=0x0064,//������ͣ
			JW2_Import_AccountOpen_Resp=0x8064,

			JW2_GM_Update=0x0065,//����GM״̬
			JW2_GM_Update_Resp=0x8065,

			JW2_ProductManage_Insert=0x0066,//����Ȩ��
			JW2_ProductManage_Insert_Resp=0x8066,

			JW2_ProductManage_View=0x0067,//�鿴Ȩ��
			JW2_ProductManage_View_Resp=0x8067,
			
			JW2_ProductManage_Del=0x0068,//ɾ��Ȩ��
			JW2_ProductManage_Del_Resp=0x8068,

			//maple add
			JW2_ProductManage_Validate=0x0069,//��֤Ȩ��
			JW2_ProductManage_Validate_Resp=0x8069,

			JW2_ProductManage_Deduct=0x0070,///��֤ͨ����ȥȨ��
			JW2_ProductManage_Deduct_Resp=0x8070,
			
			
			//////////////////
			//CG2(0x71)
			/////////////////
			CG2_Account_Query = 0x0001,//��ɫ��������
			CG2_Account_Query_Resp = 0x8001,//��ɫ��������
			CG2_Account_Detail_Query = 0x0002, //��ɫ��ϸ����
			CG2_Account_Detail_Query_Resp = 0x8002, //��ɫ��ϸ����
			CG2_Account_Attribute_Query = 0x0003,//��ɫ��������
			CG2_Account_Attribute_Query_Resp = 0x8003,//��ɫ��������
			CG2_Account_Guild_Query = 0x0004,//��ɫ��������
			CG2_Account_Guild_Query_Resp = 0x8004,//��ɫ��������
			CG2_Account_Title_Query = 0x0005,//��ɫ�ƺ�����
			CG2_Account_Title_Query_Resp = 0x8005,//��ɫ�ƺ�����
			CG2_Account_Address_Query = 0x0006,//��ɫ����ϵ
			CG2_Account_Address_Query_Resp = 0x8006,//��ɫ����ϵ
			CG2_Account_Skill_Query = 0x0007,//��ɫ������Ϣ
			CG2_Account_Skill_Query_Resp = 0x8007,//��ɫ������Ϣ
			//������Ʒ����
			CG2_CharItem_Query = 0x0008,//��ɫ���ϵ�����Ϣ
			CG2_CharItem_Query_Resp = 0x8008,//��ɫ���ϵ�����Ϣ
			CG2_TempItem_Query = 0x0009,//��ʱ������Ϣ
			CG2_TempItem_Query_Resp = 0x8009,//��ʱ������Ϣ
			CG2_ShopItem_Query = 0x0010,//�̳���Ʒ��ȡ
			CG2_ShopItem_Query_Resp = 0x8010,//�̳���Ʒ��ȡ
			//������Ʒ����
			CG2_BankInfo_Query = 0x0011,//���д����Ϣ
			CG2_BankInfo_Query_Resp = 0x8011,//���д����Ϣ
			CG2_BankItem_Query = 0x0012,//������Ʒ��Ϣ
			CG2_BankItem_Query_Resp = 0x8012,//������Ʒ��Ϣ
			//������Ϣ
			CG2_PetInfo_Query = 0x0013,//�����������
			CG2_PetInfo_Query_Resp = 0x8013,//�����������
			CG2_PetInfo_Detail_Query = 0x0014,//������ϸ����
			CG2_PetInfo_Detail_Query_Resp = 0x8014,//������ϸ����
			CG2_PetItem_Query = 0x0015,//���ﱳ����Ϣ
			CG2_PetItem_Query_Resp = 0x8015,//	���ﱳ����Ϣ
			CG2_PetSkill_Query = 0x0016,//���＼����Ϣ
			CG2_PetSkill_Query_Resp = 0x8016,//	���＼����Ϣ
			//����
			CG2_Job_Query = 0x0017,	//���� 
			CG2_Job_Query_Resp = 0x8017,//���� 
			//�˺ż����ѯ
			CG2_Account_Active_Query = 0x0018,	//ͨ���˺Ų�ѯ
			CG2_Account_Active_Query_Resp = 0x8018,//ͨ���˺Ų�ѯ
			CG2_Sn_Active_Query  = 0x0019,		//ͨ���������ѯ
			CG2_Sn_Active_Query_Resp  = 0x8019,//ͨ���������ѯ
			//ϵͳ����
			CG2_Ban_Query = 0x0020,//����ѡ��ʱ�䷶Χ����������ɸѡ����ָ���������з��������Ľ�ɫ
			CG2_Ban_Query_Resp = 0x8020,//����ѡ��ʱ�䷶Χ����������ɸѡ����ָ���������з��������Ľ�ɫ
			CG2_Account_Ban_Query = 0x0021,//����ѡ��ʱ�䷶Χ���˺š��ǳƽ��в�ѯ����ʾ���˴���
			CG2_Account_Ban_Query_Resp = 0x8021,//����ѡ��ʱ�䷶Χ���˺š��ǳƽ��в�ѯ����ʾ���˴���
			
			//�ʺŵ�½��Ϣ
			CG2_Use_Query = 0x0022,	//�ʺŵ�½��Ϣ
			CG2_Use_Query_Resp = 0x8022,//�ʺŵ�½��Ϣ
			CG2_Account_Ip_Query = 0x0023,	//ͨ��IP��ַ��ѯ����½���ʺ�
			CG2_Account_Ip_Query_Resp = 0x8023,//ͨ��IP��ַ��ѯ����½���ʺ�
			CG2_Account_User_Query = 0x0024,	//ͨ���ʺŲ�ѯ��ʷ��½IP
			CG2_Account_User_Query_Resp = 0x8024,//ͨ���ʺŲ�ѯ��ʷ��½IP
			CG2_UseLog_Query = 0x0025,	//�ʼ���־��ѯ��Ϣ
			CG2_UseLog_Query_Resp = 0x8025,//�ʼ���־��ѯ��Ϣ

			//������
			//����
			CG2_Get_Item_Query = 0x0026,//��ȡ��Ϸ���ߣ����ࣩ
			CG2_Get_Item_Query_Resp = 0x8026,//��ȡ��Ϸ���ߣ����ࣩ
			CG2_Get_Item_Detail_Query = 0x0027,	//��ȡ��Ϸ���ߣ�С�ࣩ
			CG2_Get_Item_Detail_Query_Resp = 0x8027,	//��ȡ��Ϸ���ߣ�С�ࣩ

			
			//�˺�
			CG2_Account_Close = 0x0028,//��ͣ����ʺ�
			CG2_Account_Close_RESP = 0x8028,//��ͣ����ʺ���Ӧ
			CG2_Account_Open = 0x0029,//�������ʺ�
			CG2_Account_Open_RESP= 0x8030,//�������ʺ���Ӧ
			CG2_Account_BANISHMENT_QUERY = 0x0030,//��ҷ�ͣ�ʺŲ�ѯ״̬
			CG2_Account_BANISHMENT_QUERY_RESP = 0x8030,//��ҷ�ͣ�ʺŲ�ѯ״̬��Ӧ
			CG2_BanAccount_Query = 0x0031,//��ѯ���з�ͣ����ʺ���Ϣ
			CG2_BanAccount_Query_Resp = 0x8031,//��ѯ���з�ͣ����ʺ���Ϣ
			CG2_KickAccount_Query = 0x0032,//������
			CG2_KickAccount_Query_Resp = 0x8032,//������

			//��ͼ
			CG2_Change_Map_Query = 0x0033,//��ͼ����
			CG2_Change_Map_Query_Resp = 0x8033,//��ͼ����

			//����
			CG2_SendNote_Query = 0x0034,//���͹���
			CG2_SendNote_Query_Resp = 0x8034,//���͹���
			CG2_UpdateNote_Query = 0x0035,//�޸Ĺ���
			CG2_UpdateNote_Query_Resp= 0x8035,//�޸Ĺ���
			CG2_DeleteNote_Query = 0x0036,//ɾ������
			CG2_DeleteNote_Query_Resp= 0x8036,//ɾ������

			//��ɫ
			CG2_Update_Account_Query = 0x0037,//�޸Ľ�ɫ��Ϣ
			CG2_Update_Account_Query_Resp = 0x8037,//�޸Ľ�ɫ��Ϣ
			CG2_Update_Pet_Query = 0x0038,//�޸ĳ�����Ϣ
			CG2_Update_Pet_Query_Resp = 0x8038,//�޸ĳ�����Ϣ

			//����
			CG2_Insert_Account_Skill_Query = 0x0039,//��ӽ�ɫ������Ϣ
			CG2_Insert_Account_Skill_Query_Resp = 0x8039,//��ӽ�ɫ������Ϣ
			CG2_Insert_Pet_Skill_Query = 0x0040,//��ӳ��＼����Ϣ
			CG2_Insert_Pet_Skill_Query_Resp = 0x8040,//��ӳ��＼����Ϣ

			CG2_Update_Account_Skill_Query = 0x0041,//�޸Ľ�ɫ������Ϣ
			CG2_Update_Account_Skill_Query_Resp = 0x8041,//�޸Ľ�ɫ������Ϣ
			CG2_Update_Pet_Skill_Query = 0x0042,//�޸ĳ��＼����Ϣ
			CG2_Update_Pet_Skill_Query_Resp = 0x8042,//�޸ĳ��＼����Ϣ

			CG2_Delete_Account_Skill_Query = 0x0043,//ɾ����ɫ������Ϣ
			CG2_Delete_Account_Skill_Query_Resp = 0x8043,//ɾ����ɫ������Ϣ
			CG2_Delete_Pet_Skill_Query = 0x0044,//ɾ�����＼����Ϣ
			CG2_Delete_Pet_Skill_Query_Resp = 0x8044,//ɾ�����＼����Ϣ

			//��ʱ�˺�
			CG2_Insert_TmpPwd_Query = 0x0045,//�����ʱ����
			CG2_Insert_TmpPwd_Query_Resp = 0x8045,//�����ʱ����
			CG2_Recover_TmpPwd_Query = 0x0046,//�ָ��û�����
			CG2_Recover_TmpPwd_Query_Resp = 0x8046,//�ָ��û�����

			CG2_Insert_Item_Query = 0x0047,//�����Ϸ����
			CG2_Insert_Item_Query_Resp = 0x8047,//�����Ϸ����
			CG2_RealAccount_Query = 0x0048,//��ѯ�������û���������
			CG2_RealAccount_Query_Resp = 0x8048,//��ѯ�������û���������
			
			CG2_SearchItem_Query = 0x0049,//ģ����ѯ����
			CG2_SearchItem_Query_Resp = 0x8049,//ģ����ѯ����

			CG2_MapList_Query = 0x0051,//��ѯ��ͼ�б�
			CG2_MapList_Query_Resp = 0x8051,//��ѯ��ͼ�б�
			CG2_SearchNote_Query = 0x0050,//�鿴����
			CG2_SearchNote_Query_Resp= 0x8050,//�鿴����

			CG2_SearchGuild_Query = 0x0052,//��ѯ������Ϣ
			CG2_SearchGuild_Query_Resp = 0x8052,//��ѯ������Ϣ

			CG2_SearchGuild_Detail_Query = 0x0053,//��ѯ������ϸ��Ϣ
			CG2_SearchGuild_Detail_Query_Resp = 0x8053,//��ѯ������ϸ��Ϣ

			CG2_CloseGuild_Query = 0x0054,//��ɢ����
			CG2_CloseGuild_Query_Resp = 0x8054,//��ɢ����

			CG2_RenameGuild_Query = 0x0055,//�޸Ĺ�������
			CG2_RenameGuild_Query_Resp = 0x8055,//�޸Ĺ�������

			CG2_Recovery_Account_Qurey = 0x0056,//�ָ���ɫ��Ϣ
			CG2_Recovery_Account_Qurey_Resp = 0x8056,//�ָ���ɫ��Ϣ

			CG2_Search_Account_Qurey = 0x0057,//��ѯ��ɫ����
			CG2_Search_Account_Qurey_Resp = 0x8057,//��ѯ��ɫ����

			CG2_BakAccount_Query = 0x0058,//��ѯ�����û�
			CG2_BakAccount_Query_Resp = 0x8058,//��ѯ�����û�
			
			//�����ʴ�ϵͳ
			CG2_SendAsk_Query = 0x0059,//�����ʴ�ϵͳ
			CG2_SendAsk_Query_Resp = 0x8059,//�����ʴ�ϵͳ

			//�޸Ľ�ɫ����
			CG2_UpdateAccount_Query = 0x0060,//�޸Ľ�ɫ����
			CG2_UpdateAccount_Query_Resp = 0x8060,//�޸Ľ�ɫ����

			//�޸Ľ�ɫ�ȼ�
			CG2_UpdateLevel_Query = 0x0061,//�޸Ľ�ɫ�ȼ�
			CG2_UpdateLevel_Query_Resp = 0x8061,//�޸Ľ�ɫ�ȼ�

			//�޸Ľ�ɫְҵ
			CG2_UpdateJob_Query = 0x0062,//�޸Ľ�ɫְҵ
			CG2_UpdateJob_Query_Resp = 0x8062,//�޸Ľ�ɫְҵ

			//��ȡ��ɫְҵ
			CG2_GetJob_Query = 0x0063,//��ȡ��ɫְҵ
			CG2_GetJob_Query_Resp = 0x8063,//��ȡ��ɫְҵ
			CG2_PackItem_Query = 0x0064,//����������Ϣ
			CG2_PackItem_Query_Resp = 0x8064,//����������Ϣ
			CG2_GetSkill_Query = 0x0065,//������＼������
			CG2_GetSkill_Query_Resp = 0x8065,//������＼������
			
			CG2_SkillList_Query = 0x0066,//������＼���б�
			CG2_SkillList_Query_Resp = 0x8066,//������＼���б�


			CG2_GetPetSkill_Query = 0x0067,//��ó��＼������
			CG2_GerPetSkill_Query_Resp = 0x8067,//��ó��＼������
			
			CG2_PetSkillList_Query = 0x0068,//��ó��＼���б�
			CG2_PetSkillList_Query_Resp = 0x8068,//��ó��＼���б�

			//�޸�����������Ϣ
			CG2_InsertBankItem_Query = 0x0069,//���е������
			CG2_InsertBankItem_Query_Resp = 0x8069,//���е������

			CG2_SetBankMoney_Query = 0x0070,//���н������
			CG2_SetBankMoney_Query_Resp = 0x8070,//���н������

			//�޸ĳ���������Ϣ
			CG2_ModiPetAttribute_Query = 0x0071,//�޸ĳ���������Ϣ
			CG2_ModiPetAttribute_Query_Resp = 0x8071,//

			CG2_PetList_Query = 0x0072,//��ȡ�����б�
			CG2_PetList_Query_Resp = 0x8072,//��ȡ�����б�

			CG2_AddPet_Query = 0x0073,//��ӳ���
			CG2_AddPet_Query_Resp = 0x8073,//��ӳ���
			//�ָ���־����
			CG2_LogItem_Query = 0x0074,//�鿴��־������Ϣ
			CG2_LogItem_Query_Resp = 0x8074,//�鿴��־������Ϣ

			CG2_RecoverLogItem_Query = 0x0075,//�ָ���־������Ϣ
			CG2_RecoverLogItem_Query_Resp = 0x8075,//�ָ���־������Ϣ

			CG2_Account_Teach_Query = 0x0076,//�鿴���ʦ����ϵ
			CG2_Account_Teach_Query_Resp = 0x8076,//�鿴���ʦ����ϵ

			CG2_Send_ItemPassword = 0x0077,//��ȡ������
			CG2_Send_ItemPassword_Resp = 0x8077,//��ȡ������

			CG2_SearchPassWord_Query = 0x0078,//��ѯ��ʱ����
			CG2_SearchPassWord_Query_Resp = 0x8078,//��ѯ��ʱ����

			CG2_InsertItemALL_Query = 0x0079,//��ӵ��ߣ����룩
			CG2_InsertItemALL_Query_Resp = 0x8079,//��ӵ��ߣ����룩

			CG2_UserShoppingInfo_Query = 0x0080,//�̳���Ϣ��ѯ
			CG2_UserShoppingInfo_Query_Resp = 0x8080,//�̳���Ϣ��ѯ

			CG2_UserGoldCard_Query = 0x0081,//�ƽ𼤻
			CG2_UserGoldCard_Query_Resp = 0x8081,//�ƽ𼤻

			CG2_InsertBankItemALL_Query = 0x0082,//�������е�����ӣ����룩
			CG2_InsertBankItemALL_Query_Resp = 0x8082,//�������е�����ӣ����룩

			CG2_SearchMap_Query = 0x0083,//ģ����ѯ��ͼ
			CG2_SearchMap_Query_Resp = 0x8083,//ģ����ѯ��ͼ

			CG2_PetPic_Query = 0x0084,//����ͼ����Ϣ
			CG2_PetPic_Query_Resp = 0x8084,//����ͼ����Ϣ

			CG2_UpdatePetLevel_Query = 0x0085,//�޸ĳ���ȼ�
			CG2_UpdatePetLevel_Query_Resp = 0x8085,//�޸ĳ���ȼ�

			CG2_BankPet_Query = 0x0086,//���г�����Ϣ
			CG2_BankPet_Query_Resp = 0x8086,//���г�����Ϣ

			CG2_UpdateGuildLv_Query=0x0087,//����ȼ��޸�
			CG2_UpdateGuildLv_Query_Resp=0x8087,//����ȼ��޸�

			CG2_UpdateGuildHonor_Query=0x0088,//���������޸�
			CG2_UpdateGuildHonor_Query_Resp=0x8088,//���������޸�

			CG2_LogInfo_Pet_Query = 0x0089,//������ϸ����
			CG2_LogInfo_Pet_Query_Resp = 0x8089,//������ϸ����

			CG2_LogInfo_Item_Query = 0x0090,//������ϸ����
			CG2_LogInfo_Item_Query_Resp = 0x8090,//������ϸ����

			CG2_AccountErr_Query = 0x0091,//����ȼ��쳣�ص�
			CG2_AccountErr_Query_Resp = 0x8091,//����ȼ��쳣�ص�

			CG2_ChangeGold_Query = 0x0092,//�޸������Ǯ
			CG2_ChangeGold_Query_Resp = 0x8092,//�޸������Ǯ

			CG2_BagItem_Del_Query = 0x0093,//ɾ����ɫ����������
			CG2_BagItem_Del_Query_Resp = 0x8093,//ɾ����ɫ����������

			CG2_TmpItem_Del_Query = 0x0094,//ɾ����ʱ����������
			CG2_TmpItem_Del_Query_Resp = 0x8094,//ɾ����ʱ����������

			CG2_EquipItem_Del_Query = 0x0095,//ɾ������װ������
			CG2_EquipItem_Del_Query_Resp = 0x8095,//ɾ������װ������

			CG2_BankItem_Del_Query = 0x0096,//ɾ�����е���
			CG2_BankItem_Del_Query_Resp = 0x8096,//ɾ�����е���

			CG2_PetPic_Del_Query = 0x0097,//ɾ�����е���
			CG2_PetPic_Del_Query_Resp = 0x8097,//ɾ�����е���

			CG2_KickAccountCount_Query = 0x0098,//�����ߴ���
			CG2_KickAccountCount_Query_Resp = 0x8098,//�����ߴ���

			CG2_RecoverLogPet_Query = 0x0099,//�ָ���־����
			CG2_RecoverLogPet_Query_Resp = 0x8099,//�ָ���־����

			CG2_LoveTree_Query = 0x0100,//��������Ϣ��ѯ
			CG2_LoveTree_Query_Resp = 0x8100,//��������Ϣ��ѯ

			CG2_GuildMazeWar_Query = 0x0101,//�����Թ�ռ��
			CG2_GuildMazeWar_Query_Resp = 0x8101,//�����Թ�ռ��

			CG2_Marriage_Info_Query = 0x0102,//��һ�����Ϣ
			CG2_Marriage_Info_Query_Resp = 0x8102,//��һ�����Ϣ

			CG2_ChangeCharNo_Query = 0x0103,//��ɫ��λ
			CG2_ChangeCharNo_Query_Resp = 0x8103,//��ɫ��λ

			CG2_Log_Query=0x0104,
			CG2_Log_Query_Resp=0x8104,////��ѯ�����־��Ϣ

			CG2_Get_EmailAddress=0x0105,//�õ�Email��ַ
			CG2_Get_EmailAddress_Resp=0x8105,//



			/// <summary>
			/// SDO_ADMIN �������߹��߲�����Ϣ��
			/// </summary>
			SDO_ACCOUNT_QUERY = 0x0026,//�鿴��ҵ��ʺ���Ϣ
			SDO_ACCOUNT_QUERY_RESP = 0x8026,//�鿴��ҵ��ʺ���Ϣ��Ӧ
			SDO_CHARACTERINFO_QUERY = 0x0027,//�鿴�������ϵ���Ϣ
			SDO_CHARACTERINFO_QUERY_RESP = 0x8027,//�鿴�������ϵ���Ϣ��Ӧ
			SDO_ACCOUNT_CLOSE = 0x0028,//��ͣ�ʻ���Ȩ����Ϣ
			SDO_ACCOUNT_CLOSE_RESP = 0x8028,//��ͣ�ʻ���Ȩ����Ϣ��Ӧ
			SDO_ACCOUNT_OPEN = 0x0029,//����ʻ���Ȩ����Ϣ
			SDO_ACCOUNT_OPEN_RESP = 0x8029,//����ʻ���Ȩ����Ϣ��Ӧ
			SDO_PASSWORD_RECOVERY = 0x0030,//����һ�����
			SDO_PASSWORD_RECOVERY_RESP = 0x8030,//����һ�������Ӧ
			SDO_CONSUME_QUERY = 0x0031,//�鿴��ҵ����Ѽ�¼
			SDO_CONSUME_QUERY_RESP = 0x8031,//�鿴��ҵ����Ѽ�¼��Ӧ
			SDO_USERONLINE_QUERY = 0x0032,//�鿴���������״̬
			SDO_USERONLINE_QUERY_RESP = 0x8032,//�鿴���������״̬��Ӧ
			SDO_USERTRADE_QUERY = 0x0033,//�鿴��ҽ���״̬
			SDO_USERTRADE_QUERY_RESP = 0x8033,//�鿴��ҽ���״̬��Ӧ
			SDO_CHARACTERINFO_UPDATE = 0x0034,//�޸���ҵ��˺���Ϣ
			SDO_CHARACTERINFO_UPDATE_RESP = 0x8034,//�޸���ҵ��˺���Ϣ��Ӧ
			SDO_ITEMSHOP_QUERY = 0x0035,//�鿴��Ϸ�������е�����Ϣ
			SDO_ITEMSHOP_QUERY_RESP = 0x8035,//�鿴��Ϸ�������е�����Ϣ��Ӧ
			SDO_ITEMSHOP_DELETE = 0x0036,//ɾ����ҵ�����Ϣ
			SDO_ITEMSHOP_DELETE_RESP  = 0x8036,//ɾ����ҵ�����Ϣ��Ӧ
			SDO_GIFTBOX_CREATE  = 0x0037,//����������е�����Ϣ
			SDO_GIFTBOX_CREATE_RESP  = 0x8037,//����������е�����Ϣ��Ӧ
			SDO_GIFTBOX_QUERY = 0x0038,//�鿴�������еĵ���
			SDO_GIFTBOX_QUERY_RESP = 0x8038,//�鿴�������еĵ�����Ӧ
			SDO_GIFTBOX_DELETE = 0x0039,//ɾ���������еĵ���
			SDO_GIFTBOX_DELETE_RESP = 0x8039,//ɾ���������еĵ�����Ӧ
			SDO_USERLOGIN_STATUS_QUERY = 0x0040,//�鿴��ҵ�¼״̬
			SDO_USERLOGIN_STATUS_QUERY_RESP = 0x8040,//�鿴��ҵ�¼״̬��Ӧ
			SDO_ITEMSHOP_BYOWNER_QUERY = 0x0041,////�鿴������ϵ�����Ϣ
			SDO_ITEMSHOP_BYOWNER_QUERY_RESP = 0x8041,////�鿴������ϵ�����Ϣ
			SDO_ITEMSHOP_TRADE_QUERY = 0x0042,//�鿴��ҽ��׼�¼��Ϣ
			SDO_ITEMSHOP_TRADE_QUERY_RESP = 0x8042,//�鿴��ҽ��׼�¼��Ϣ����Ӧ
			SDO_MEMBERSTOPSTATUS_QUERY = 0x0043,//�鿴���ʺ�״̬
			SDO_MEMBERSTOPSTATUS_QUERY_RESP = 0x8043,///�鿴���ʺ�״̬����Ӧ
			SDO_MEMBERBANISHMENT_QUERY = 0x0044,//�鿴����ͣ����ʺ�
			SDO_MEMBERBANISHMENT_QUERY_RESP = 0x8044,//�鿴����ͣ����ʺ���Ӧ
			SDO_USERMCASH_QUERY = 0x0045,//��ҳ�ֵ��¼��ѯ
			SDO_USERMCASH_QUERY_RESP = 0x8045,//��ҳ�ֵ��¼��ѯ��Ӧ
			SDO_USERGCASH_UPDATE = 0x0046,//�������G��
			SDO_USERGCASH_UPDATE_RESP = 0x8046,//�������G�ҵ���Ӧ
			SDO_MEMBERLOCAL_BANISHMENT = 0x0047,//���ر���ͣ����Ϣ
			SDO_MEMBERLOCAL_BANISHMENT_RESP = 0x8047,//���ر���ͣ����Ϣ��Ӧ
			SDO_EMAIL_QUERY = 0x0048,//�õ���ҵ�EMAIL
			SDO_EMAIL_QUERY_RESP = 0x8048,//�õ���ҵ�EMAIL��Ӧ
			SDO_USERCHARAGESUM_QUERY = 0x0049,//�õ���ֵ��¼�ܺ�
			SDO_USERCHARAGESUM_QUERY_RESP = 0x8049,//�õ���ֵ��¼�ܺ���Ӧ
			SDO_USERCONSUMESUM_QUERY = 0x0050,//�õ����Ѽ�¼�ܺ�
			SDO_USERCONSUMESUM_QUERY_RESP = 0x8050,//�õ����Ѽ�¼�ܺ���Ӧ
			SDO_USERGCASH_QUERY = 0x0051,//��ңǱҼ�¼��ѯ
			SDO_USERGCASH_QUERY_RESP = 0x8051,//��ңǱҼ�¼��ѯ��Ӧ
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
			SDO_Family_Info_QUERY = 0x0126,//���������Ϣ
			SDO_Family_Info_QUERY_RESP = 0x8126,//���������Ϣ
			SDO_FamilyMember_Query = 0x0127,//�����Ա��Ϣ
			SDO_FamilyMember_Query_RESP = 0x8127,//�����Ա��Ϣ
			SDO_FamilyFormal_Query= 0x0128,//�����г�Ա��Ϣ
			SDO_FamilyFormal_Query_RESP = 0x8128,//�����г�Ա��Ϣ
			SDO_Personal_Emblem_Query = 0x0129,//����ѫ��
			SDO_Personal_Emblem_Query_RESP = 0x8129,//����ѫ��
			SDO_Family_Donate_Query = 0x0130,//�����������ϸ
			SDO_Family_Donate_Query_RESP = 0x8130,//�����������ϸ

			SDO_Family_Delete_Query = 0x0131,//��ɢ����
			SDO_Family_Delete_Query_RESP = 0x8131,//��ɢ����

			SDO_Family_Duty_Query = 0x0132,//�����Աְ���޸�
			SDO_Family_Duty_Query_RESP = 0x8132,//�����Աְ���޸�

			SDO_Family_Emblem_Insert_Query= 0x0133,//����Ӽ������
			SDO_Family_Emblem_Insert_Query_RESP= 0x8133,//����Ӽ������

			SDO_Family_Emblem_Delete_Query= 0x0134,//��ɾ���������
			SDO_Family_Emblem_Delete_Query_RESP= 0x8134,//��ɾ���������

			//maple add
			SDO_Pet_Update= 0x0135,//ɾ����ҳ���
			SDO_Pet_Update_RESP =0x8135,
			SDO_Social_Update=0x0136,//�޸������罻ֵ
			SDO_Social_Update_Resp=0x8136,
			SDO_Lucky_Update=0x0137,//�޸���������ֵ
			SDO_Lucky_Update_Resp=0x8137,


			Ban_Open_Query=0x0001,//����˺�
			Ban_Open_Query_Resp=0x8001,//����˺Ż�Ӧ
			
			Ban_Close_Query=0x0002,//��ͣ�˺�
			Ban_Close_Query_Resp=0x8002,//��ͣ�˺Ż�Ӧ
			
			Ban_AccountList_Query=0x0003,//�˺��б��ѯ
			Ban_AccountList_Query_Resp=0x8003,//�˺��б��ѯ��Ӧ
			
			Ban_Account_Query=0x0004,//�˺Ų�ѯ
			Ban_Account_Query_Resp=0x8004,//�˺Ų�ѯ��Ӧ
			
			Ban_UserInfo_Query=0x0005,//�ʻ���Ϣ��ѯ
			Ban_UserInfo_Query_Resp=0x8005,//�ʻ���Ϣ��ѯ��Ӧ

			MESSAGE = 0xEEEE
		};  
		__Enum _value; // ö��ֵ  
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
			CONNECT = 0x800001,//��������
			CONNECT_RESP = 0x808001,//������Ӧ
			DISCONNECT = 0x800002,//�Ͽ�����
			DISCONNECT_RESP = 0x808002,//�Ͽ���Ӧ
			ACCOUNT_AUTHOR = 0x800003,//�û������֤����
			ACCOUNT_AUTHOR_RESP = 0x808003,//�û������֤��Ӧ
			SERVERINFO_IP_QUERY = 0x800004,
			SERVERINFO_IP_QUERY_RESP = 0x808004,
			GMTOOLS_OperateLog_Query = 0x800005,//�鿴���߲�����¼
			GMTOOLS_OperateLog_Query_RESP = 0x808005,//�鿴���߲�����¼��Ӧ
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
			GM_AccountInfo_Vip = 0x800020,			//��ѯvip�ʺ�
			GM_AccountInfo_Vip_RESP = 0x808020,		//��ѯvip�ʺ���Ӧ
			//GM_ClosePlayerGroup_WA = 0x800021,			//��ͣ�ʺ�
			GM_ClosePlayerGroup_RESP_WA = 0x808021,	//��ͣ�ʺ���Ӧ


			USER_ACTIVE=0x800018,//������
			USER_ACTIVE_RESP=0x808018,

			//Added by tengjie 2010-05-05
			HARMFUL_PROC_QUERY = 0x800019,//��ҽ��̹��߰�
			HARMFUL_PROC_QUERY_RESP=0x808019,

			/// <summary>
			/// �û�����ģ��(0x81)
			/// </summary>
			USER_CREATE = 0x810001,//����GM�ʺ�����
			USER_CREATE_RESP = 0x818001,//����GM�ʺ���Ӧ
			USER_UPDATE = 0x810002,//����GM�ʺ���Ϣ����
			USER_UPDATE_RESP = 0x818002,//����GM�ʺ���Ϣ��Ӧ
			USER_DELETE = 0x810003,//ɾ��GM�ʺ���Ϣ����
			USER_DELETE_RESP = 0x818003,//ɾ��GM�ʺ���Ϣ��Ӧ
			USER_QUERY = 0x810004,//��ѯGM�ʺ���Ϣ����
			USER_QUERY_RESP = 0x818004,//��ѯGM�ʺ���Ϣ��Ӧ
			USER_PASSWD_MODIF = 0x810005,//�����޸�����
			USER_PASSWD_MODIF_RESP = 0x818005, //�����޸���Ϣ��Ӧ
			USER_QUERY_ALL = 0x810006,//��ѯ����GM�ʺ���Ϣ
			USER_QUERY_ALL_RESP = 0x818006,//��ѯ����GM�ʺ���Ϣ��Ӧ
			DEPART_QUERY = 0x810007, //��ѯ�����б�
			DEPART_QUERY_RESP = 0x818007,//��ѯ�����б���Ӧ
			UPDATE_ACTIVEUSER = 0x810008,//���������û�״̬
			UPDATE_ACTIVEUSER_RESP = 0x818008,//���������û�״̬��Ӧ
			DEPARTMENT_CREATE = 0x810009,//���Ŵ���
			DEPARTMENT_CREATE_RESP = 0x818009,//���Ŵ�����Ӧ
			DEPARTMENT_UPDATE= 0x810010,//�����޸�
			DEPARTMENT_UPDATE_RESP = 0x818010,//�����޸���Ӧ
			DEPARTMENT_DELETE= 0x810011,//����ɾ��
			DEPARTMENT_DELETE_RESP = 0x818011,//����ɾ����Ӧ
			DEPARTMENT_ADMIN = 0x810012,//���Ź���
			DEPARTMENT_ADMIN_RESP = 0x818012,//���Ź�����Ӧ
			DEPARTMENT_RELATE_QUERY = 0x810013,//���Ź�����ѯ
			DEPARTMENT_RELATE_QUERY_RESP = 0x818013,//���Ź�����ѯ
			DEPART_RELATE_GAME_QUERY = 0x810014,
			DEPART_RELATE_GAME_QUERY_RESP = 0x818014,
			USER_SYSADMIN_QUERY = 0x810015,
			USER_SYSADMIN_QUERY_RESP = 0x818015,
			USERBAN_QUERY = 0x810016,
			USERBAN_QUERY_RESP = 0x818016,


			/// <summary>
			/// ģ�����(0x82)
			/// </summary>
			MODULE_CREATE = 0x820001,//����ģ����Ϣ����
			MDDULE_CREATE_RESP = 0x828001,//����ģ����Ϣ��Ӧ
			MODULE_UPDATE =0x820002,//����ģ����Ϣ����
			MODULE_UPDATE_RESP = 0x828002,//����ģ����Ϣ��Ӧ
			MODULE_DELETE = 0x820003,//ɾ��ģ������
			MODULE_DELETE_RESP = 0x828003,//ɾ��ģ����Ӧ
			MODULE_QUERY = 0x820004,//��ѯģ����Ϣ������
			MODULE_QUERY_RESP = 0x828004,//��ѯģ����Ϣ����Ӧ

			/// <summary>
			/// �û���ģ�����(0x83)
			/// </summary>
			USER_MODULE_CREATE = 0x830001,//�����û���ģ������
			USER_MODULE_CREATE_RESP = 0x838001,//�����û���ģ����Ӧ
			USER_MODULE_UPDATE = 0x830002,//�����û���ģ�������
			USER_MODULE_UPDATE_RESP = 0x838002,//�����û���ģ�����Ӧ
			USER_MODULE_DELETE = 0x830003,//ɾ���û���ģ������
			USER_MODULE_DELETE_RESP = 0x838003,//ɾ���û���ģ����Ӧ
			USER_MODULE_QUERY = 0x830004,//��ѯ�û�����Ӧģ������
			USER_MODULE_QUERY_RESP = 0x838004,//��ѯ�û�����Ӧģ����Ӧ

			/// <summary>
			/// ��Ϸ����(0x84)
			/// </summary>
			GAME_CREATE = 0x840001,//����GM�ʺ�����
			GAME_CREATE_RESP = 0x848001,//����GM�ʺ���Ӧ
			GAME_UPDATE = 0x840002,//����GM�ʺ���Ϣ����
			GAME_UPDATE_RESP = 0x848002,//����GM�ʺ���Ϣ��Ӧ
			GAME_DELETE = 0x840003,//ɾ��GM�ʺ���Ϣ����
			GAME_DELETE_RESP = 0x848003,//ɾ��GM�ʺ���Ϣ��Ӧ
			GAME_QUERY = 0x840004,//��ѯGM�ʺ���Ϣ����
			GAME_QUERY_RESP = 0x848004,//��ѯGM�ʺ���Ϣ��Ӧ
			GAME_MODULE_QUERY = 0x840005,//��ѯ��Ϸ��ģ���б�
			GAME_MODULE_QUERY_RESP = 0x848005,//��ѯ��Ϸ��ģ���б���Ӧ
		
			/// <summary>
			/// �ɽ�GM����(0x62)
			/// </summary>
			PAL_PLAYERONLINECOUNT=0x620001,//�����������
			PAL_PLAYERONLINECOUNT_RESP=0x628001,//�������������Ӧ

			PAL_CHARACTERBAG=0x620002,//��ұ�����ѯ
			PAL_CHARACTERBAG_RESP=0x628002,//��ұ�����ѯ��Ӧ

			PAL_CHARACTERFRIENDLIST=0x620003,//ȡ�ý�ɫ�����б�
			PAL_CHARACTERFRIENDLIST_RESP=0x628003,//ȡ�ý�ɫ�����б���Ӧ

			PAL_CHARACTERINFO=0x620004,//ȡ�ý�ɫ������Ѷ
			PAL_CHARACTERINFO_RESP=0x628004,//ȡ�ý�ɫ������Ѷ��Ӧ
			
			PAL_CHARACTERLIST=0x620005,//ȡ�ý�ɫ�б�
			PAL_CHARACTERLIST_RESP=0x628005,//ȡ�ý�ɫ�б���Ӧ

			PAL_CHARACTERQUEST=0x620006,//ȡ�ý�ɫ������Ѷ
			PAL_CHARACTERQUEST_RESP=0x628006,//ȡ�ý�ɫ������Ѷ��Ӧ

			PAL_GUILDBASEINFO=0x620007,//ȡ�ù��������Ѷ
			PAL_GUILDBASEINFO_RESP=0x628007,//ȡ�ù��������Ѷ��Ӧ
			
			PAL_GUILDMEMBERLIST=0x620008,//ȡ�ù����Ա�б�
			PAL_GUILDMEMBERLIST_RESP=0x628008,//ȡ�ù����Ա�б���Ӧ

			PAL_SENDGIFTMAIL=0x620009,//���ͽ�Ʒ�ʼ�
			PAL_SENDGIFTMAIL_RESP=0x628009,//���ͽ�Ʒ�ʼ���Ӧ

			PAL_ACCOUNTLOCAL_QUERY = 0x620010,//���ط�ͣ������ʺŲ�ѯ
			PAL_ACCOUNTLOCAL_QUERY_RESP = 0x628010,//���ط�ͣ������ʺŲ�ѯ��Ӧ
			PAL_ACCOUNT_CLOSE = 0x620011,//��ͣ������ʺ�
			PAL_ACCOUNT_CLOSE_RESP = 0x628011,//��ͣ������ʺ���Ӧ
			PAL_ACCOUNT_OPEN = 0x620012,//��������ʺ�
			PAL_ACCOUNT_OPEN_RESP = 0x628012,//��������ʺ���Ӧ
			PAL_ACCOUNT_BANISHMENT_QUERY = 0x620013,//��ҷ�ͣ�ʺŲ�ѯ
			PAL_ACCOUNT_BANISHMENT_QUERY_RESP = 0x628013,//��ҷ�ͣ�ʺŲ�ѯ��Ӧ


			PAL_MODIFYCHARACTERMONEY=0x620014,//�޸Ľ�ɫ��Ǯ
			PAL_MODIFYCHARACTERMONEY_RESP=0x628014,//�޸Ľ�ɫ��Ǯ��Ӧ

			PAL_MODIFYCHARACTERDROPPW=0x620015,//�޸Ľ�ɫɾ������
			PAL_MODIFYCHARACTERDROPPW_RESP=0x628015,//�޸Ľ�ɫɾ��������Ӧ

			PAL_MODIFYACCOUNTBANKPW=0x620016,//�޸Ľ�ɫ��������
			PAL_MODIFYACCOUNTBANKPW_RESP=0x628016,//�޸Ľ�ɫ����������Ӧ

			PAL_DELCHARACTERITEM=0x620017,// ɾ����ɫ��Ʒ
			PAL_DELCHARACTERITEM_RESP=0x628017,// ɾ����ɫ��Ʒ��Ӧ

			PAL_RECALLCHARACTERDROPPW=0x620018,//��ԭ��ɫɾ������
			PAL_RECALLCHARACTERDROPPW_RESP=0x628018,//��ԭ��ɫɾ��������Ӧ

			PAL_RECALLACCOUNTBANKPW=0x620019,//��ԭ�ʺ���������
			PAL_RECALLACCOUNTBANKPW_RESP=0x628019,//��ԭ�ʺ�����������Ӧ

			PAL_MODIFYACCOUNTPW=0x620020,//�޸��ʺ�����
			PAL_MODIFYACCOUNTPW_RESP=0x628020,//�޸��ʺ�������Ӧ

			PAL_RECALLACCOUNTPW=0x620021,//��ԭ�ʺ�����
			PAL_RECALLACCOUNTPW_RESP=0x628021,//��ԭ�ʺ�������Ӧ

	
			PAL_BOARDTASK_INSERT = 0x620033,//�������
			PAL_BOARDTASK_INSERT_RESP = 0x628033,//���������Ӧ
			PAL_BOARDTASK_QUERY = 0x620034,//�����ѯ
			PAL_BOARDTASK_QUERY_RESP = 0x628034,//�����ѯ��Ӧ
			PAL_BOARDTASK_UPDATE = 0x620035,//�������
			PAL_BOARDTASK_UPDATE_RESP = 0x628035,//���������Ӧ

			
			PAL_ITEMTYPE_QUERY=0x620037,//��������ѯ
			PAL_ITEMTYPE_QUERY_RESP=0x628037,//��������ѯ��Ӧ
			PAL_ITEMNAME_QUERY=0x620038,//�������Ʋ�ѯ
			PAL_ITEMNAME_QUERY_RESP=0x628038,//�������Ʋ�ѯ��Ӧ
			PAL_ITEMNAMEBLUR_QUERY=0x620039,//��������ģ����ѯ
			PAL_ITEMNAMEBLUR_QUERY_RESP=0x628039,//��������ģ����ѯ��Ӧ

			PAL_MAILBOX_QUERY=0x620040,//��ѯ��������ַ
			PAL_MAILBOX_QUERY_RESP=0x628040,//��ѯ��������ַ��Ӧ 

			PAL_SendBankPwd_Query=0x620041,//���Ͳֿ�����
			PAL_SendBankPwd_Query_RESP=0x628041,//���Ͳֿ�������Ӧ

			PAL_SendDeletePwd_Query=0x620042,//���ͽ�ɫɾ������
			PAL_SendDeletePwd_Query_RESP=0x628042,//���ͽ�ɫɾ��������Ӧ

			PAL_MODIFYSECONDPW=0x620043,//�޸���Ϸ��������
			PAL_MODIFYSECONDPW_RESP=0x628043,//�޸���Ϸ����������Ӧ
			
			PAL_RECALLSECONDPW=0x620044,//��ԭ��Ϸ��������
			PAL_RECALLSECONDPW_RESP=0x628044,//��ԭ��Ϸ����������Ӧ

			PAL_USESAFELOCK=0x620045,//ʹ�ð�ȫ��
			PAL_USESAFELOCK_RESP=0x628045,//ʹ�ð�ȫ����Ӧ

			PAL_FREESAFELOCK=0x620046,//�ͷŰ�ȫ��
			PAL_FREESAFELOCK_RESP=0x628046,//�ͷŰ�ȫ����Ӧ

			PAL_CHARACTERADVANCEINFO=0x620047,//ȡ�ý�ɫ������Ѷ
			PAL_CHARACTERADVANCEINFO_RESP=0x628047,//ȡ�ý�ɫ������Ѷ

			PAL_CHARACTERSKILL=0x620048,//ȡ�ý�ɫ������Ѷ
			PAL_CHARACTERSKILL_RESP=0x628048,//ȡ�ý�ɫ������Ѷ

			PAL_CHARACTERMOB=0x620049,//ȡ�ý�ɫ��ħ��Ѷ
			PAL_CHARACTERMOB_RESP=0x628049,//ȡ�ý�ɫ��ħ��Ѷ


			PAL_CHARACTERPUZZLEMAP=0x620050,//ȡ�ý�ɫͼ����Ѷ
			PAL_CHARACTERPUZZLEMAP_RESP=0x628050,//ȡ�ý�ɫͼ����Ѷ

			PAL_CHARACTERBLACKLIST=0x620051,//ȡ�ý�ɫ�������б�
			PAL_CHARACTERBLACKLIST_RESP=0x628051,//ȡ�ý�ɫ�������б�

			PAL_DELCHARACTERMOB=0x620052,//ɾ����ɫ��ħ
			PAL_DELCHARACTERMOB_RESP=0x628052,//ɾ����ɫ��ħ

			PAL_MODIFYPUZZLEMAP=0x620053,//�޸Ľ�ɫͼ��
			PAL_MODIFYPUZZLEMAP_RESP=0x628053,//�޸Ľ�ɫͼ��

			PAL_ACCOUNTINFO=0x620054,//ȡ���ʺ���Ѷ
			PAL_ACCOUNTINFO_RESP=0x628054,//ȡ���ʺ���Ѷ

			PAL_MOVECHARACTERTOSAFE=0x620055,//�ƶ���ɫ����ȫ��
			PAL_MOVECHARACTERTOSAFE_RESP=0x628055,//�ƶ���ɫ����ȫ��

			PAL_RELOADFUNCTIONSWITCH=0x620056,//�������빦�ܿ����б�
			PAL_RELOADFUNCTIONSWITCH_RESP=0x628056,//�������빦�ܿ����б�


			PAL_SearchMap_Query=0x620057,//��ѯ��ͼ
			PAL_SearchMap_Query_Resp=0x628057,//��ѯ��ͼ

			PAL_SCHEDULEBULLETINLIST=0x621001,//ȡ���ų̹����б�
			PAL_SCHEDULEBULLETINLIST_RESP=0x627001,//ȡ���ų̹����б���Ӧ

			PAL_ADDSCHEDULEBULLETIN=0x621002,//�����ų̹���
			PAL_ADDSCHEDULEBULLETIN_RESP=0x627002,//�����ų̹�����Ӧ	

			PAL_DELSCHEDULEBULLETIN=0x621003,//ɾ���ų̹���
			PAL_DELSCHEDULEBULLETIN_RESP=0x627003,//ɾ���ų̹�����Ӧ	

			PAL_RELOADSCHEDULEBULLETIN=0x621004,//���������ų̹���
			PAL_RELOADSCHEDULEBULLETIN_RESP=0x627004,//���������ų̹�����Ӧ	

			PAL_SENDNONSCHEDULEBULLETIN=0x621005,//���ͷǹ̶�����
			PAL_SENDNONSCHEDULEBULLETIN_RESP=0x627005,//���ͷǹ̶�������Ӧ	
	


			PAL_BANISHPLAYER =0x624001,//����
			PAL_BANISHPLAYER_RESP=0x626001,//������Ӧ
			
			PAL_DISMISSGUILD=0x624002,//��ɢ����
			PAL_DISMISSGUILD_RESP=0x626002,//��ɢ������Ӧ

			PAL_MODIFYGUILDNAME=0x624003,//�޸Ĺ�������
			PAL_MODIFYGUILDNAME_RESP=0x626003,//�޸Ĺ���������Ӧ

			PAL_MODIFYGUILDMEMBERPL=0x624004,//�޸Ĺ����Աְ��
			PAL_MODIFYGUILDMEMBERPL_RESP=0x626004,//�޸Ĺ����Աְ����Ӧ

			PAL_MODIFYCHARACTERQUEST=0x624005,//�޸Ľ�ɫ����
			PAL_MODIFYCHARACTERQUEST_RESP=0x626005,//�޸Ľ�ɫ������Ӧ

			PAL_MODIFYGUILDLV=0x624006,//�޸Ĺ���ȼ�
			PAL_MODIFYGUILDLV_RESP=0x626006,//�޸Ĺ���ȼ���Ӧ

			PAL_MODIFYGUILDNOTICE=0x624007,//�޸Ĺ��ṫ��
			PAL_MODIFYGUILDNOTICE_RESP=0x626007,//�޸Ĺ��ṫ����Ӧ
			
			PAL_Get_EmailAddress=0x620066,
			PAL_Get_EmailAddress_Resp=0x628066,



			PALLOG_ACCOUNT_LOGIN=0x630001,//��½��¼��־
			PALLOG_ACCOUNT_LOGIN_RESP=0x638001,//��½��¼��־��Ӧ

			PAL_LOGINFO_QUERY=0x630002,//�����־��Ϣ
			PAL_LOGINFO_QUERY_RESP=0x638002,//�����־��Ϣ��Ӧ

			PAL_LOG_BIGTYPE_QUERY=0x630003,//��־����
			PAL_LOG_BIGTYPE_QUERY_RESP=0x638003,//��־������Ӧ

			PAL_LOG_SMALLTYPE_QUERY=0x630004,//��־С��
			PAL_LOG_SMALLTYPE_QUERY_RESP=0x638004,//��־С����Ӧ

			PALLOG_ITEMMAIL=0x630005,//�̳ǹ�����־
			PALLOG_ITEMMAIL_RESP=0x638005,//�̳ǹ�����־��Ӧ

			PAL_CHECKMAILBOX_QUERY=0x630036,//������Ϣ��ѯ
			PAL_CHECKMAILBOX_QUERY_RESP=0x638036,//������Ϣ��ѯ��Ӧ 

			PAL_CHECKMAILBOX_DETAIL=0x630043,//���丽����ѯ
			PAL_CHECKMAILBOX_DETAIL_RESP=0x6328043,//���丽����ѯ��Ӧ

			PAL_AUCTION_QUERY=0x630037,//�г���Ϣ��ѯ
			PAL_AUCTION_QUERY_RESP=0x638037,//�г���Ϣ��ѯ��Ӧ 

			PAL_CDKEY_QUERY=0x630040,//CDKEY������ȡ��ѯ
			PAL_CDKEY_QUERY_RESP=0x638040,//CDKEY������ȡ��ѯ��Ӧ 

			PAL_ITEMMAIL_WASTE=0x630041,//���Ѽ�¼
			PAL_ITEMMAIL_WASTE_RESP=0x638041,//���Ѽ�¼��Ӧ

			PAL_HISTORY_LOGINFO_QUERY=0x630042,//��־��ʷ
			PAL_HISTORY_LOGINFO_QUERY_RESP=0x638042,//��־��ʷ��Ӧ

			PAL_ROLEDEL_QUERY=0x630050,//ɾ����ɫ��־
			PAL_ROLEDEL_QUERY_RESP=0x638050,//ɾ����ɫ��־
			
			PAL_EQUIPMENT_QUERY=0x630061,//װ�����߲�ѯ
			PAL_EQUIPMENT_QUERY_RESP=0x638061,//װ�����߲�ѯ��Ӧ

			PAL_SENDCUSTOMITEMMAIL=0x620062,//���Ϳ�����Ʒ�ʼ�
			PAL_SENDCUSTOMITEMMAIL_RESP=0x628062,//���Ϳ�����Ʒ�ʼ���Ӧ

			PAL_MODIFYACCOUNTPWALL=0x620063,//�޸����д����ʺ�����
			PAL_MODIFYACCOUNTPWALL_RESP=0x628063,//�޸����д����ʺ�������Ӧ
			
			PAL_RECALLACCOUNTPWALL=0x620064,//�ָ����Դ����ʺ�����
			PAL_RECALLACCOUNTPWALL_RESP=0x628064,//�ָ����Դ����ʺ�������Ӧ

			//////////////////
			//GTown(0x51)
			/////////////////
			//��ӹ���
			GT_BROADCAST_ADD = 0x520001,
			GT_BROADCAST_ADD_ACK = 0x528001,

			//��ѯ���й���
			GT_BROADCAST_SELECT = 0x520002,
			GT_BROADCAST_SELECT_ACK = 0x528002,

			//ɾ������
			GT_BROADCAST_REMOVE = 0x520003,
			GT_BROADCAST_REMOVE_ACK = 0x528003,

			//ǿ���������
			GT_KICK_USER = 0x520004,
			GT_KICK_USER_ACK = 0x528004,

			//��ɫ����С����
			GT_MOVE_USER = 0x520005,
			GT_MOVE_USER_ACK = 0x528005,

			//�������
			GT_FAMILY_DISBAND = 0x520006,
			GT_FAMILY_DISBAND_ACK = 0x528006,

			//�˺ŷ�ͣ�����
			GT_USER_STOP = 0x520007,
			GT_USER_STOP_ACK = 0x528007,

			//��ӵ��ߡ�����
			GT_ADD_ITEM = 0x520008,
			GT_ADD_ITEM_ACK = 0x528008,

			//��ѯ���ߡ�����
			GT_ITEM_SELECT = 0x520009,
			GT_ITEM_SELECT_ACK = 0x528009,

			//������ҵ�����״̬
			GT_USER_RESET = 0x520010,
			GT_USER_RESET_ACK = 0x528010,

			//����Զ�̷�����
			CONNECT_SERVERMANAGER_LOGIN = 0x520011,
			CONNECT_SERVERMANAGER_LOGIN_ACK = 0x528011,

			//��ѯ�û�������Ϣ
			GT_UserInfo_Query = 0x510012,
			GT_UserInfo_Query_Resp = 0x518012,

			//��ѯ�û���ϸ������Ϣ
			GT_User_DetailInfo_Query = 0x510013,
			GT_User_DetailInfo_Query_Resp = 0x518013,

			//��ѯ��ɫ������Ϣ
			GT_User_FamilyInfo_Query = 0x510014,
			GT_User_FamilyInfo_Query_Resp = 0x518014,

			//��ѯ��ɫ������Ϣ
			GT_User_SentiInfo_Query = 0x510015,
			GT_User_SentiInfo_Query_Resp = 0x518015,

			//��ѯ��ɫ�����Ϣ
			GT_User_MarrigeInfo_Query = 0x510016,
			GT_User_MarrigeInfo_Query_Resp = 0x518016,

			//��ѯ���б���ͣ�˺��б�
			GT_AllBanUser_Query = 0x510017,
			GT_AllBanUser_Query_Resp = 0x518017,

			//��ѯ������Ϣ
			GT_FamilyInfo_Query = 0x510018,
			GT_FamilyInfo_Query_Resp = 0x518018,

			//��ѯ�����Ա��Ϣ
			GT_MemberInfo_Query = 0x510019,
			GT_MemberInfo_Query_Resp = 0x518019,

			//��ѯ��ɫ������Ϣ
			GT_UserPack_Query = 0x510020,
			GT_UserPack_Query_Resp = 0x518020,

			//��ѯ��ɫװ����Ϣ
			GT_UserEquip_Query = 0x510021,
			GT_UserEquip_Query_Resp = 0x518021,

			//��ѯ���ﱳ����Ϣ
			GT_PetPack_Query = 0x510022,
			GT_PetPack_Query_Resp = 0x518022,



			//��ѯ��ҽ�����־
			GT_TradeLog_Query = 0x510023,
			GT_TradeLog_Query_Resp = 0x518023,
			
			//��ѯ����ʼ���־
			GT_MailLog_Query = 0x510024,
			GT_MailLog_Query_Resp = 0x518024,
			
			//��ѯ������������־
			GT_TaskLog_Query = 0x510025,
			GT_TaskLog_Query_Resp = 0x518025,
			
			//��ѯ���ʹ�õ�����־(�շѵ���)
			GT_UseItemLog_Query = 0x510026,
			GT_UseItemLog_Query_Resp = 0x518026,
			
			//��ѯ��ҹһ�������־
			GT_GuajiLog_Query = 0x510027,
			GT_GuajiLog_Query_Resp = 0x518027,

			//��ѯ��Ҵ�С������־
			GT_BugleLog_Query = 0x510028,
			GT_BugleLog_Query_Resp = 0x518028,
			
			//��ѯ�˺ŵ�½�ǳ���־
			GT_LoginLog_Query = 0x510029,
			GT_LoginLog_Query_Resp = 0x518029,



			//��ѯ���﷿������
			GT_RoomInfo_Query = 0x510030,
			GT_RoomInfo_Query_Resp = 0x518030,
			
			//��ѯ�����������
			GT_PetInfo_Query = 0x510031,
			GT_PetInfo_Query_Resp = 0x518031,
			
			//��ѯGT-IM����������Ϣ
			GT_IMFriend_Query = 0x510032,
			GT_IMFriend_Query_Resp = 0x518032,

			//��ѯGT-IM��������Ϣ
			GT_IMBlackList_Query = 0x510033,
			GT_IMBlackList_Query_Resp = 0x518033,

			//��ѯGT-IM����������Ϣ
			GT_IMSecretLove_Query = 0x510034,
			GT_IMSecretLove_Query_Resp = 0x518034,
			
			//��ѯ���俪����Ϣ
			GT_TreasureBox_Query = 0x510035,
			GT_TreasureBox_Query_Resp = 0x518035,

			//�ʼ���Ϣ��ѯ
			GT_MailInfo_Query = 0x510036,
			GT_MailInfo_Query_Resp = 0x518036,

			//��ֵ��¼��ѯ
			GT_ChargeInfo_Query = 0x510037,
			GT_ChargeInfo_Query_Resp = 0x518037,

			//���߹������Ѽ�¼
			GT_ItemConsume_Query = 0x510038,
			GT_ItemConsume_Query_Resp = 0x518038,


			//˲���ƶ�
			GT_Transmit_Query=0x520039,
			GT_Transmit_Query_Resp=0x528039,
			
			//��ֹ���������ϴ�ͼƬ
			GT_Picture_Stop=0x520040,
			GT_Picture_Stop_Resp=0x528040,
			
			
			//����Υ��ͼƬ
			GT_Picture_Clear=0x520043,
			GT_Picture_Clear_Resp=0x528043,


			//Ѫƴս�� T_gtown_Mcoin_Rank
			GT_Mcoin_Rank=0x510044,
			GT_Mcoin_Rank_Resp=0x518044,
			
			//�ռ����� T_gtown_sns_db
			GT_Sns_Query=0x510045,
			GT_Sns_Query_Resp=0x518045,
			
			//��������
			GT_Attr_Queue=0x510046,
			GT_Attr_Queue_Resp=0x518046,
			
			//��Ƭ���� T_gtown_card_record
			GT_Card_Record=0x510047,
			GT_Card_Record_Resp=0x518047,
			
			//���������н�ɫ��Ϣ
			GT_FriendTrade_Info=0x510048,
			GT_FriendTrade_Info_Resp=0x518048,
			
			//ū����Ϣ
			GT_HelotInfo_Query=0x510049,
			GT_HelotInfo_Query_Resp=0x518049,
			
			//����
			GT_FriendTrade_Queue=0x510050,
			GT_FriendTrade_Queue_Resp=0x518050,
			
			//����Log
			GT_FriendTrade_Log=0x510051,
			GT_FriendTrade_Log_Resp=0x518051,

			//��ո���ǩ��
			GT_ClearName_Query=0x520052,
			GT_ClearName_Query_Resp=0x528052,

			//��ѯ���б���ֹ�˺��б�
			GT_AllStopUser_Query = 0x510053,
			GT_AllStopUser_Query_Resp = 0x518053,

			//��ѯ����(��ԭ���ӿڱ�����ݿ�)
			GT_Select_Item=0x510054,
			GT_Select_Item_Resp=0x518054,
			
			//��ѯ�����˺��Ƿ񱻷�ͣ
			GT_BanUser_Query=0x510055,
			GT_BanUser_Query_Resp=0x518055,


			//������ӵ���
			GT_Import_Item=0x520056,
			GT_Import_Item_Resp=0x528056,


			//�˿�
			GT_Back_Money=0x510057,
			GT_Back_Money_Resp=0x518057,


			/////////<summary>
			///��ʿ��˫��Ϣ��(0x53)
			//////</summary>
			//��ѯ�û���Ϣ
			WA_Account_Query=0x530001,
			WA_Account_Query_Resp=0x538001,

			//ǿ���������
			WA_KickPlayer=0x530002,
			WA_KickPlayer_Resp=0x538002,

			//��ͣ�û�
			WA_ClosePlayer=0x530003,
			WA_ClosePlayer_Resp=0x538003,

			//����û�
			WA_OpenPlayer=0x530004,
			WA_OpenPlayer_Resp=0x538004,

			//��ͣ�б�
			WA_CloseList=0x530005,
			WA_CloseList_Resp=0x538005,

			//������ͣ
			WA_CloseSingle=0x530006,
			WA_CloseSingle_Resp=0x538006,

			//��ӹ���
			WA_BoardTasker_Add=0x530007,
			WA_BoardTasker_Add_Resp=0x538007,

			//ɾ������
			WA_BoardTasker_Del=0x530008,
			WA_BoardTasker_Del_Resp=0x538008,

			//��ѯ����
			WA_BoardTasker_Query=0x530009,
			WA_BoardTasker_Query_Resp=0x538009,

			//������Ϣ��ѯ
			WA_FriendInfo_Query=0x530010,
			WA_FriendInfo_Query_Resp=0x538010,
			
			//��ɫ��Ʒ��Ϣ��ѯ
			WA_BodyItemInfo_Query=0x530011,
			WA_BodyItemInfo_Query_Resp=0x538011,
			
			//��ɫ������Ϣ��ѯ
			WA_BagItemInfo_Query=0x530012,
			WA_BagItemInfo_Query_Resp=0x538012,
			
			//��ɫ������Ϣ��ѯ
			WA_BankItemInfo_Query=0x530013,
			WA_BankItemInfo_Query_Resp=0x538013,
			
			//������Ϣ��ѯ
			WA_MailInfo_Query=0x530014,
			WA_MailInfo_Query_Resp=0x538014,
			
			//������Ϣ��ѯ
			WA_EmblemInfo_Query=0x530015,
			WA_EmblemInfo_Query_Resp=0x538015,
			
			//������Ϣ��ѯ
			WA_PetInfo_Query=0x530016,
			WA_PetInfo_Query_Resp=0x538016,
			
			//�ʺŵ�¼��Ϣ��ѯ
			WA_AccountLoginInfo_Query=0x530017,
			WA_AccountLoginInfo_Query_Resp=0x538017,

			//��������
			WA_Item_Add=0x530018,
			WA_Item_Add_Resp=0x538018,
			
			//����ɾ��
			WA_Item_Del=0x530019,
			WA_Item_Del_Resp=0x538019,
			
			//��Ǯ�޸�
			WA_Money_Update=0x530020,
			WA_Money_Update_Resp=0x538020,

			//�����޸�
			WA_BoardTasker_Update=0x530021,
			WA_BoardTasker_Update_Resp=0x538021,

			//add by ltx
			//�޸Ľ�ɫ����
			WA_ModifyCharacterSkill=0x530022,
			WA_ModifyCharacterSkill_Resp=0x538022,

			//�ָ���ɫ
			WA_RecoverCharacter=0x530023,
			WA_RecoverCharacter_Resp=0x538023,

			//�޸�����
			WA_ModifyAccountPassw=0x530024,
			WA_ModifyAccountPassw_Resp=0x538024,

			//����ɫ�ʼ���Ʒ
			WA_MailItem=0x530025,
			WA_MailItem_Resp=0x538025,

			//��Ʒ:�������
			WA_Query_MonsterDropItem=0x530026,
			WA_Query_MonsterDropItem_Resp=0x538026,

			//��Ʒ:��NPC������
			WA_Query_BuyItemNPC=0x530027,
			WA_Query_BuyItemNPC_Resp=0x538027,
			
			//��Ʒ:����NPC
			WA_Query_SellItemTONPC=0x530028,
			WA_Query_SellItemTONPC_Resp=0x538028,

			//��Ʒ:�������ɾ��
			WA_Query_DropItem=0x530029,
			WA_Query_DropItem_Resp=0x538029,

			//��Ʒ:���׻��
			WA_Query_GetItemTrade=0x530030,
			WA_Query_GetItemTrade_Resp=0x538030,

			//��Ʒ:����ʧȥ
			WA_Query_LoseItemTrade=0x530031,
			WA_Query_LoseItemTrade_Resp=0x538031,

			//��Ʒ:װ���󶨼�¼
			WA_Query_BindItem=0x530032,
			WA_Query_BindItem_Resp=0x538032,

			//��Ʒ:װ����󶨼�¼
			WA_Query_UnBindItem=0x530033,
			WA_Query_UnBindItem_Resp=0x538033,

			//��Ʒ:��ɫ����̯����־��ѯ
			WA_Query_BoothItem=0x530034,
			WA_Query_BoothItem_Resp=0x538034,

			//��Ʒ:����ɾ����¼(ָʹ��ʱ�䵽��)
			WA_Query_DeleteItem=0x530035,
			WA_Query_DeleteItem_Resp=0x538035,

			//��Ʒ:����ʹ�ü�¼
			WA_Query_UseItem=0x530036,
			WA_Query_UseItem_Resp=0x538036,

			//��Ʒ:װ��������¼
			WA_Query_CraftItem=0x530037,
			WA_Query_CraftItem_Resp=0x538037,

			//��Ʒ:װ�����
			WA_Query_PunchItem=0x530038,
			WA_Query_PunchItem_Resp=0x538038,

			//��Ʒ:װ���Ŀ�
			WA_Query_ChangeSlotItem=0x530039,
			WA_Query_ChangeSlotItem_Resp=0x538039,

			//��Ʒ:װ������
			WA_Query_UpgradeStar=0x530040,
			WA_Query_UpgradeStar_Resp=0x538040,

			//��Ʒ:��ʯ
			WA_Query_Gem=0x530041,
			WA_Query_Gem_Resp=0x538041,

			//��Ʒ:��ҩ��¼
			WA_Query_Pharma=0x530042,
			WA_Query_Pharma_Resp=0x538042,

			//��Ʒ:��ʯ�ϳ�
			WA_Query_CraftGem=0x530043,
			WA_Query_CraftGem_Resp=0x538043,

			//��Ʒ:���Ϻϳ�
			WA_Query_CraftMatirail=0x530044,
			WA_Query_CraftMatirail_Resp=0x538044,

			//��Ʒ:ͼֽ�ϳ�
			WA_Query_CraftRecipe=0x530045,
			WA_Query_CraftRecipe_Resp=0x538045,

			//�Թ����ֶһ�����
			WA_Query_MazeIntegral=0x530046,
			WA_Query_MazeIntegral_Resp=0x538046,

			//��Ϸ��:�������
			WA_Query_MonsterDropMoney=0x530047,
			WA_Query_MonsterDropMoney_Resp=0x538047,

			//��Ϸ��:������
			WA_Query_GetMoneyQuest=0x530048,
			WA_Query_GetMoneyQuest_Resp=0x538048,

			//��Ϸ��:��װ
			WA_Query_LoseMoneyRepair=0x530049,
			WA_Query_LoseMoneyRepair_Resp=0x538049,

			//��Ϸ��:���׻��
			WA_Query_GetMoneyTrade=0x530050,
			WA_Query_GetMoneyTrade_Resp=0x538050,

			//��Ϸ��:����ʧȥ
			WA_Query_LoseMoneyTrade=0x530051,
			WA_Query_LoseMoneyTrade_Resp=0x538051,

			//����:��ɻ�ü�¼
			WA_Query_QuestReward=0x530052,
			WA_Query_QuestReward_Resp=0x538052,

			//����:������Ʒɾ����¼
			WA_Query_QuestGive=0x530053,
			WA_Query_QuestGive_Resp=0x538053,

			//�ʼ�:���ͼ�¼
			WA_Query_MailSend=0x530054,
			WA_Query_MailSend_Resp=0x538054,

			//�ʼ�:���ռ�¼
			WA_Query_MailRecv=0x530055,
			WA_Query_MailRecv_Resp=0x538055,

			//�ʼ�:ɾ����¼
			WA_Query_MailDelete=0x530056,
			WA_Query_MailDelete_Resp=0x538056,

			//�̳�:�����¼
			WA_Query_ItemShop=0x530057,
			WA_Query_ItemShop_Resp=0x538057,

			//��ѯ�û��ĸ�����־
			WA_Query_Instance=0x530058,
			WA_Query_Instance_Resp=0x538058,

			//��ҵ�½/�ǳ���־
			WA_Query_LoginLogout=0x530059,
			WA_Query_LoginLogout_Resp=0x538059,
			
			//�����б��ѯ
			WA_ItemList_Query=0x530060,
			WA_ItemList_Query_Resp=0x538060,
			
			//�����б��ѯ
			WA_SkillList_Query=0x530061,
			WA_SkillList_Query_Resp=0x538061,

			//�޸Ľ�ɫ��
			WA_PlayerRole_Modfiy=0x530062,
			WA_PlayerRole_Modfiy_Resp=0x538062,

			//�޸Ľ�ɫ����ֵ
			WA_RoleInfo_Modfiy=0x530063,
			WA_RoleInfo_Modfiy_Resp=0x538063,

			//��ѯ������ϼ�����Ϣ
			WA_RoleSkill_Query=0x530064,
			WA_RoleSkill_Query_Resp=0x538064,
			
			//��ѯ��ҵ������Ͳ�ѯ
			WA_ItemType_Query=0x530065,
			WA_ItemType_Query_Resp=0x538065,

			//�������ָ�
			WA_UserPwd_Recover=0x530066,
			WA_UserPwd_Recover_Resp=0x538066,

			//Added by tengjie 2010-06-04:For ��ɫ��־
			//��ɫ������¼��ѯ
			WA_Character_Upgrade=0x530067,
			WA_Character_Upgrade_Resp=0x538067,

			//��ɫ������¼��ѯ
			WA_Character_Create=0x530068,
			WA_Character_Create_Resp=0x538068,

			//��ɫɾ����¼��ѯ
			WA_Character_Delete=0x530069,
			WA_Character_Delete_Resp=0x538069,

			//������
			WA_Join_Guild=0x530070,
			WA_Join_Guild_Resp=0x538070,

			//�뿪���
			WA_Quit_Guild=0x530071,
			WA_Quit_Guild_Resp=0x538071,

			//�������
			WA_Create_Guild=0x530072,
			WA_Create_Guild_Resp=0x538072,

			//��ɢ���
			WA_Dissolve_Guild=0x530073,
			WA_Dissolve_Guild_Resp=0x538073,

			//תְ��Ϣ��ѯ
			WA_Transfer_Info=0x530074,
			WA_Transfer_Info_Resp=0x538074,
			
			//������־��¼
			WA_Skills_Log=0x530075,
			WA_Skills_Log_Resp=0x538075,
			
			//�������־��¼
			WA_LifeSkills_Log=0x530076,
			WA_LifeSkills_Log_Resp=0x538076,

			//Ӷ������־
			//������¼
			WA_Create_MercenaryGroup=0x530077,
			WA_Create_MercenaryGroup_Resp=0x538077,

			//��ɢ��¼
			WA_Dissolve_MercenaryGroup=0x530078,
			WA_Dissolve_MercenaryGroup_Resp=0x538078,

			//���������־
// 			//���»��ʱ��
			WA_GainBadge_Time=0x530079,
			WA_GainBadge_Time_Resp=0x538079,

			//���¶���ʱ��
			WA_DiscardBadge_Time=0x530080,
			WA_DiscardBadge_Time_Resp=0x538080,

			//���¶һ���¼
			WA_ExchangeBadge_Record=0x530081,
			WA_ExchangeBadge_Record_Resp=0x538081,

			//���¼���
			WA_Identify_Badge=0x530082,
			WA_Identify_Badge_Resp=0x538082,
			
			//���ﾫ�겶׽��¼
			WA_CatchPetSpirit_Record=0x530083,
			WA_CatchPetSpirit_Record_Resp=0x538083,
			
			//��ӡ�����¼
			WA_SealSpirit_Record=0x530084,
			WA_SealSpirit_Record_Resp=0x538084,

			//����������¼
			WA_CultureBadge_Record=0x530085,
			WA_CultureBadge_Record_Resp=0x538085,
			
			//���꽻�׼�¼
			//���꽻�׻��
			WA_GainSpirit_Record=0x530086,
			WA_GainSpirit_Record_Resp=0x538086,
			
			//���꽻��ʧȥ
			WA_LoseSpirit_Record=0x530087,
			WA_LoseSpirit_Record_Resp=0x538087,

			//������ۼ�¼
			WA_SellSpirit_Record=0x530088,
			WA_SellSpirit_Record_Resp=0x538088,

			//�û���Ϣ��ѯ
			//������Ϣ��ѯ
			WA_SkillsInfo_Query=0x530089,
			WA_SkillsInfo_Query_Resp=0x538089,

			//������Ϣ��ѯ:���������
			WA_MissionsCompleted_Query=0x530090,
			WA_MissionsCompleted_Query_Resp=0x538090,

			//������Ϣ��ѯ:�ѽ�����
			WA_MissionsReceived_Query=0x530091,
			WA_MissionsReceived_Query_Resp=0x538091,

			//Ӷ������Ϣ��ѯ
			//Ӷ������Ϣ��ѯ
			WA_MercenaryGroup_Info_Query=0x530092,
			WA_MercenaryGroup_Info_Query_Resp=0x538092,

			//Ӷ���ų�Ա�б��ѯ
			WA_MemberList_Query=0x530093,
			WA_MemberList_Query_Resp=0x538093,

			//Ӷ���Ųֿ���Ϣ
			WA_WarehouseInfo_Query=0x530094,
			WA_WarehouseInfo_Query_Resp=0x538094,

			//��ȡӶ����нˮ��Ϣ
			WA_Receive_SalaryInfo_Query=0x530095,
			WA_Receive_SalaryInfo_Query_Resp=0x538095,

			//��ȡӶ���ž�����Ϣ
			WA_Receive_ExperienceInfo_Query=0x530096,
			WA_Receive_ExperienceInfo_Query_Resp=0x538096,

			//���а��ѯ
			//�ȼ�����
			WA_Rank_Query=0x530097,
			WA_Rank_Query_Resp=0x538097,

			//��������
			WA_Sports_Query=0x530098,
			WA_Sports_Query_Resp=0x538098,

			//Ӷ����������
			WA_Prestige_Query=0x530099,
			WA_Prestige_Query_Resp=0x538099,

			//������������
			WA_CopyIntegral_Query=0x530100,
			WA_CopyIntegral_Query_Resp=0x538100,

			//Ӷ��������
			WA_MercenaryGroup_Query=0x530101,
			WA_MercenaryGroup_Query_Resp=0x538101,

			//������:
			//�������:��ӳ���(�ȼ�\����)
			WA_PetAdded_Operate=0x530102,
			WA_PetAdded_Operate_Resp=0x538102,

			//�������:ɾ������(�ȼ�\����)
			WA_PetDeleted_Operate=0x530103,
			WA_PetDeleted_Operate_Resp=0x538103,
			
			//�������
			WA_AttornPopedom_Operate=0x530104,
			WA_AttornPopedom_Operate_Resp=0x538104,
			
			//Ӷ���Ź���:�޸Ĺ�������
			WA_ModifyGuildName_Operate=0x530105,
			WA_ModifyGuildName_Operate_Resp=0x538105,

			//Ӷ���Ź���:�߳������Ա
			WA_KickGuildMember_Operate=0x530106,
			WA_KickGuildMember_Operate_Resp=0x538106,

			//Ӷ���Ź���:��ɢ����
			WA_DissolveGuild_Operate=0x530107,
			WA_DissolveGuild_Operate_Resp=0x538107,

			//Ӷ���Ųֿ�:ɾ����Ʒ
			WA_DeleteGoods_Operate=0x530108,
			WA_DeleteGoods_Operate_Resp=0x538108,
			
			//Ӷ���Ųֿ�:�����Ʒ
			WA_AddGoods_Operate=0x530109,
			WA_AddGoods_Operate_Resp=0x538109,
			
			//Ӷ���Ųֿ�:�޸Ľ�Ǯ����
			WA_ModifyMoney_Operate=0x530110,
			WA_ModifyMoney_Operate_Resp=0x538110,

			//�������:�����б��ѯ
			WA_PetList_Query=0x530111,
			WA_PetList_Query_Resp=0x538111,

			//��ҵ�½/�ǳ���־(��ip��ʽ��ѯ)
			WA_Query_LoginLogoutByIP=0x530112,
			WA_Query_LoginLogoutByIP_Resp=0x538112,

			//�ָ��������
			WA_Query_ResumePassw=0x530113,
			WA_Query_ResumePassw_Resp=0x538113,

			//�ָ���ɫ����
			WA_ResumeAttribute_Op=0x530114,
			WA_ResumeAttribute_Op_Resp=0x538114,

			//���������ʼ�(�޵���)
			WA_SendMails_Op=0x530115,
			WA_SendMails_Op_Resp=0x538115,


			//������ͣ�û�
			WA_ClosePlayerGroup=0x530116,
			WA_ClosePlayerGroup_Resp=0x538116,
			//End

			/////////<summary>
			///�����ɳ���Ϣ��(0x54)
			//////</summary>
			//��ѯ�û���Ϣ
			CF_Account_Query=0x540001,
			CF_Account_Query_Resp=0x548001,
			
			//ǿ���������
			CF_KickPlayer=0x540002,
			CF_KickPlayer_Resp=0x548002,
			
			//��ͣ�û�
			CF_ClosePlayer=0x540003,
			CF_ClosePlayer_Resp=0x548003,
			
			//����û�
			CF_OpenPlayer=0x540004,
			CF_OpenPlayer_Resp=0x548004,
			
			//��ͣ�б�
			CF_CloseList=0x540005,
			CF_CloseList_Resp=0x548005,
			
			//������ͣ
			CF_CloseSingle=0x540006,
			CF_CloseSingle_Resp=0x548006,
			
			//��ӹ���
			CF_BoardTasker_Add=0x540007,
			CF_BoardTasker_Add_Resp=0x548007,
			
			//ɾ������
			CF_BoardTasker_Del=0x540008,
			CF_BoardTasker_Del_Resp=0x548008,
			
			//��ѯ����
			CF_BoardTasker_Query=0x540009,
			CF_BoardTasker_Query_Resp=0x548009,

			/////////<summary>
			///���������Ϣ��(0x55)
			//////</summary>
			//��ѯ�û���Ϣ
			XDJG_Account_Query=0x550001,
			XDJG_Account_Query_Resp=0x558001,
			
			//ǿ���������
			XDJG_KickPlayer=0x550002,
			XDJG_KickPlayer_Resp=0x558002,
			
			//��ͣ�û�
			XDJG_ClosePlayer=0x550003,
			XDJG_ClosePlayer_Resp=0x558003,
			
			//����û�
			XDJG_OpenPlayer=0x550004,
			XDJG_OpenPlayer_Resp=0x558004,
			
			//��ͣ�б�
			XDJG_CloseList=0x550005,
			XDJG_CloseList_Resp=0x558005,
			
			//������ͣ
			XDJG_CloseSingle=0x550006,
			XDJG_CloseSingle_Resp=0x558006,
			
			//��ӹ���
			XDJG_BoardTasker_Add=0x550007,
			XDJG_BoardTasker_Add_Resp=0x558007,
			
			//�޸Ĺ���
			XDJG_BoardTasker_Update=0x550008,
			XDJG_BoardTasker_Update_Resp=0x558008,
			
			//ɾ������
			XDJG_BoardTasker_Del=0x550009,
			XDJG_BoardTasker_Del_Resp=0x558009,
			
			//��ѯ����
			XDJG_BoardTasker_Query=0x550010,
			XDJG_BoardTasker_Query_Resp=0x558010,

			//Added by tengjie 2010-07-06
			//��ɫ�������
			XDJG_RoleSkillAdd=0x550011,
			XDJG_RoleSkillAdd_Resp=0x558011,
			
			//��ɫ����ɾ��
			XDJG_RoleSkillDel=0x550012,
			XDJG_RoleSkillDel_Resp=0x558012,
			
			//�������
			XDJG_ItemAdd=0x550013,
			XDJG_ItemAdd_Resp=0x558013,
			
			//����ɾ��
			XDJG_ItemDel=0x550014,
			XDJG_ItemDel_Resp=0x558014,
			
			//�����Ϸ��
			XDJG_MoneyAdd=0x550015,
			XDJG_MoneyAdd_Resp=0x558015,

			//��־��:��ҵ�¼��־
			XDJG_Login_Log=0x550016,
			XDJG_Login_Log_Resp=0x558016,

			//�̳���Ʒ������־
			XDJG_Shoping_Log=0x550017,
			XDJG_Shoping_Log_Resp=0x558017,

			//�����Ʒ��־:���׻��
			XDJG_TransGain_Log=0x550018,
			XDJG_TransGain_Log_Resp=0x558018,

			//�����Ʒ��־:����ʧȥ
			XDJG_TransLose_Log=0x550019,
			XDJG_TransLose_Log_Resp=0x558019,

			//�����Ʒ��־:��NPC������
			XDJG_BuyFromNpc_Log=0x550020,
			XDJG_BuyFromNpc_Log_Resp=0x558020,

			//�����Ʒ��־:����NPC
			XDJG_SaleToNpc_Log=0x550021,
			XDJG_SaleToNpc_Log_Resp=0x558021,

			//�����Ʒ��־:ʰȡ��Ʒ
			XDJG_PickUpItem_Log=0x550022,
			XDJG_PickUpItem_Log_Resp=0x558022,

			//�����Ʒ��־:������Ʒ
			XDJG_DiscardItem_Log=0x550023,
			XDJG_DiscardItem_Log_Resp=0x558023,

			//�����Ʒ��־:��Ʒ����
			XDJG_ConsumeItem_Log=0x550024,
			XDJG_ConsumeItem_Log_Resp=0x558024,

			//�����Ʒ��־:����������
			XDJG_AuctioneerSaled_Log=0x550025,
			XDJG_AuctioneerSaled_Log_Resp=0x558025,

			//�����Ʒ��־:�����й���
			XDJG_AuctioneerBought_Log=0x550026,
			XDJG_AuctioneerBought_Log_Resp=0x558026,

			//����ʼ���־:�����ʼ�
			XDJG_SendMail_Log=0x550027,
			XDJG_SendMail_Log_Resp=0x558027,

			//����ʼ���־:��ȡ�ʼ�
			XDJG_ReceiveMail_Log=0x550028,
			XDJG_ReceiveMail_Log_Resp=0x558028,

			//M��ʹ����־:�����������Ѽ�¼
			XDJG_ExpansionPack_Log=0x550029,
			XDJG_ExpansionPack_Log_Resp=0x558029,

			//M��ʹ����־:������־
			XDJG_Deliver_Log=0x550030,
			XDJG_Deliver_Log_Resp=0x558030,

			//M��ʹ����־:��ɫ�ֿ��������Ѽ�¼
			XDJG_ExpansionWarehouse_Log=0x550031,
			XDJG_ExpansionWarehouse_Log_Resp=0x558031,

			//��ҽ�ɫ��־:��ɫ����
			XDJG_RoleUpgrade_Log=0x550032,
			XDJG_RoleUpgrade_Log_Resp=0x558032,

			//��ҽ�ɫ��־:��ɫ����
			XDJG_RoleCreate_Log=0x550033,
			XDJG_RoleCreate_Log_Resp=0x558033,

			//��ҽ�ɫ��־:��ɫ����ѧϰ
			XDJG_RoleSkill_Log=0x550034,
			XDJG_RoleSkill_Log_Resp=0x558034,

			//��ҽ�ɫ��־:��ȡ����
			XDJG_ReceiveTask_Log=0x550035,
			XDJG_ReceiveTask_Log_Resp=0x558035,

			//��ҽ�ɫ��־:�������
			XDJG_CompleteTask_Log=0x550036,
			XDJG_CompleteTask_Log_Resp=0x558036,

			//��ҽ�ɫ��־:������ѯ
			XDJG_Copy_Log=0x550037,
			XDJG_Copy_Log_Resp=0x558037,

			//��ѯ��
			//�����Ʒ��Ϣ:������Ʒ��Ϣ
			XDJG_ItemBody_Query=0x550038,
			XDJG_ItemBody_Query_Resp=0x558038,

			//�����Ʒ��Ϣ:������Ʒ��Ϣ
			XDJG_Itembackpack_Query=0x550039,
			XDJG_Itembackpack_Query_Resp=0x558039,
			
			//�����Ʒ��Ϣ:�ֿ���Ʒ��Ϣ
			XDJG_ItemStorage_Query=0x550040,
			XDJG_ItemStorage_Query_Resp=0x558040,
			
			//�����Ʒ��Ϣ:����ֿ���Ϣ
			XDJG_GuildStorage_Query=0x550041,
			XDJG_GuildStorage_Query_Resp=0x558041,
			
			//�����Ʒ��Ϣ:��ղֿ���Ϣ
			XDJG_KingkongStorage_Query=0x550042,
			XDJG_KingkongStorage_Query_Resp=0x558042,

			//�ʼ���Ϣ
			XDJG_MailInfo_Query=0x550043,
			XDJG_MailInfo_Query_Resp=0x558043,

			//��������Ϣ
			XDJG_AuctioneerInfo_Query=0x550044,
			XDJG_AuctioneerInfo_Query_Resp=0x558044,

			//������
			//�û���ɫ��Ϣ���޸�:�޸ĵȼ�
			XDJG_ModifyLevel_operate=0x550045,
			XDJG_ModifyLevel_operate_Resp=0x558045,
			
			//�û��������:�޸�����
			XDJG_ModifyPwd_operate=0x550046,
			XDJG_ModifyPwd_operate_Resp=0x558046,
			
			//�û��������:��ʱ����ָ�
			XDJG_ResumePwd_operate=0x550047,
			XDJG_ResumePwd_operate_Resp=0x558047,

			//�û���ɫ����:��ɫ�ָ�
			XDJG_ResumeRole_operate=0x550048,
			XDJG_ResumeRole_operate_Resp=0x558048,

			//�û���ɫ����:��ɫɾ��
			XDJG_DeleteRole_operate=0x550049,
			XDJG_DeleteRole_operate_Resp=0x558049,

			//�޸Ľ�ɫ���Ͻ�Ǯ����(�󶨱�)
			XDJG_UpRoleBind_operate=0x550050,
			XDJG_UpRoleBind_operate_Resp=0x558050,
			
			//�޸Ľ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)
			XDJG_UpRoleNonBind_operate=0x550051,
			XDJG_UpRoleNonBind_operate_Resp=0x558051,

			//��ɫ������Ϣ��ѯ
			XDJG_RoleSkills_Query=0x550052,
			XDJG_RoleSkills_Query_Resp=0x558052,
			
			//�����б��ѯ
			XDJG_SkillList_Query=0x550053,
			XDJG_SkillList_Query_Resp=0x558053,
			
			//�������Ͳ�ѯ
			XDJG_ItemType_Query=0x550054,
			XDJG_ItemType_Query_Resp=0x558054,
			
			//��Ӧ���͵ĵ����б��ѯ
			XDJG_ItemNameLst_Query=0x550055,
			XDJG_ItemNameLst_Query_Resp=0x558055,

			//��Ϸ�Ҳ�ѯ
			XDJG_MoneyQuery=0x550056,
			XDJG_MoneyQuery_Resp=0x558056,

			//��ӽ�ɫ���Ͻ�Ǯ����(�󶨱�)
			XDJG_AddRoleBind_operate=0x550057,
			XDJG_AddRoleBind_operate_Resp=0x558057,
			
			//��ӽ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)
			XDJG_AddRoleNonBind_operate=0x550058,
			XDJG_AddRoleNonBind_operate_Resp=0x558058,

			//��Ʒ���Բ�ѯ
			XDJG_ItemAttribute_Query=0x550059,
			XDJG_ItemAttribute_Query_Resp=0x558059,

			//������Ϣ��ѯ
			//���������Ϣ
			XDJG_GuildBaseInfo_Query=0x550060,
			XDJG_GuildBaseInfo_Query_Resp=0x558060,

			//�����Ա�б�
			XDJG_GuildMemberList_Query=0x550061,
			XDJG_GuildMemberList_Query_Resp=0x558061,

			//��������ѯ
			XDJG_PlayerTaskInfo_Query=0x550062,
			XDJG_PlayerTaskInfo_Query_Resp=0x558062,
			
			//��ճ�����Ϣ
			//��ճ���������Ϣ
			XDJG_CarBaseInfo_Query=0x550063,
			XDJG_CarBaseInfo_Query_Resp=0x558063,

			//��ճ�����ϸ��Ϣ
			XDJG_CarDetailInfo_Query=0x550064,
			XDJG_CarDetailInfo_Query_Resp=0x558064,

			//��ҹ�����Ϣ
			//��ҹ��������Ϣ
			XDJG_PlayerGuildBaseInfo_Query=0x550065,
			XDJG_PlayerGuildBaseInfo_Query_Resp=0x558065,

			//��ҹ�����Ʒ��Ϣ
			XDJG_PlayerGuildItemInfo_Query=0x550066,
			XDJG_PlayerGuildItemInfo_Query_Resp=0x558066,

			//�����Ʒ��־
			//װ��������־
			XDJG_EquipIncrease_Log=0x550067,
			XDJG_EquipIncrease_Log_Resp=0x558067,

			//��Ƭ�ϳ���־
			XDJG_Synthesis_Log=0x550068,
			XDJG_Synthesis_Log_Resp=0x558068,

			//װ��������־
			XDJG_EquipTransfer_Log=0x550069,
			XDJG_EquipTransfer_Log_Resp=0x558069,

			//��Ʒ����
			XDJG_ItemIdentify_Log=0x550070,
			XDJG_ItemIdentify_Log_Resp=0x558070,

			//���˲ֿ�����־
			XDJG_RoleStore_Log=0x550071,
			XDJG_RoleStore_Log_Resp=0x558071,

			//����ֿ�����־
			XDJG_GuildStore_Log=0x550072,
			XDJG_GuildStore_Log_Resp=0x558072,

			//������־
			//�������
			XDJG_GuildCreate_Log=0x550073,
			XDJG_GuildCreate_Log_Resp=0x558073,

			//�����ɢ
			XDJG_GuildDismiss_Log=0x550074,
			XDJG_GuildDismiss_Log_Resp=0x558074,

			//������ӳ�Ա
			XDJG_GuildAddMember_Log=0x550075,
			XDJG_GuildAddMember_Log_Resp=0x558075,

			//�����Ա�뿪
			XDJG_GuildMemberLeaved_Log=0x550076,
			XDJG_GuildMemberLeaved_Log_Resp=0x558076,

			//����ְ����
			XDJG_GuildPostChanged_Log=0x550077,
			XDJG_GuildPostChanged_Log_Resp=0x558077,
			
			//�ʺ��Ƿ��ͣ
			XDJG_AccountIsClose_Query=0x550078,
			XDJG_AccountIsClose_Query_Log_Resp=0x558078,

			//NPC������ȡ��ѯ
			XDJG_NPCGetItem_Query=0x550079,
			XDJG_NPCGetItem_Query_Resp=0x558079,

			//��ӳƺ�
			XDJG_AddTitle_Query=0x550080,
			XDJG_AddTitle_Query_Resp=0x558080,

			//�Ƴ��ƺ�
			XDJG_RemoveTitle_Query=0x550081,
			XDJG_RemoveTitle_Query_Resp=0x558081,

			//���BUFF
			XDJG_AddBuff_Query=0x550082,
			XDJG_AddBuff_Query_Resp=0x558082,

			//�Ƴ�BUFF
			XDJG_RemoveBuff_Query=0x550083,
			XDJG_RemoveBuff_Query_Resp=0x558083,

			//�ƶ���ָ��λ��
			XDJG_MoveToPosition_Query=0x550084,
			XDJG_MoveToPosition_Query_Resp=0x558084,

			//�ƶ���ָ��������
			XDJG_MoveToRole_Query=0x550085,
			XDJG_MoveToRole_Query_Resp=0x558085,

			//�ƶ���Ŀ��������
			XDJG_MoveToTargetRole_Query=0x550086,
			XDJG_MoveToTargetRole_Query_Resp=0x558086,

			//�ƺ��б��ѯ
			XDJG_TitleList_Query=0x550087,
			XDJG_TitleList_Query_Resp=0x558087,
			
			//BUFF�б��ѯ
			XDJG_BuffList_Query=0x550088,
			XDJG_BuffList_Query_Resp=0x558088,
			//End

			/// <summary>
			/// ������2GM����(0x61)
			/// </summary>
			////////////////////////��ѯ��///////////////////////////
			JW2_ACCOUNT_QUERY = 0x610001,//����ʺ���Ϣ��ѯ��1.2.3.4.5.8��
			JW2_ACCOUNT_QUERY_RESP = 0x618001,//����ʺ���Ϣ��ѯ��Ӧ��1.2.3.4.5.8��
			/////////////��ͣ��/////////////////////////////////////////
			JW2_ACCOUNTREMOTE_QUERY = 0x610002,//��Ϸ��������ͣ������ʺŲ�ѯ
			JW2_ACCOUNTREMOTE_QUERY_RESP = 0x618002,//��Ϸ��������ͣ������ʺŲ�ѯ��Ӧ
			
			JW2_ACCOUNTLOCAL_QUERY = 0x610003,//���ط�ͣ������ʺŲ�ѯ
			JW2_ACCOUNTLOCAL_QUERY_RESP = 0x618003,//���ط�ͣ������ʺŲ�ѯ��Ӧ
			
			JW2_ACCOUNT_CLOSE = 0x610004,//��ͣ������ʺ�
			JW2_ACCOUNT_CLOSE_RESP = 0x618004,//��ͣ������ʺ���Ӧ
			JW2_ACCOUNT_OPEN = 0x610005,//��������ʺ�
			JW2_ACCOUNT_OPEN_RESP= 0x618005,//��������ʺ���Ӧ
			JW2_ACCOUNT_BANISHMENT_QUERY = 0x610006,//��ҷ�ͣ�ʺŲ�ѯ
			JW2_ACCOUNT_BANISHMENT_QUERY_RESP = 0x618006,//��ҷ�ͣ�ʺŲ�ѯ��Ӧ
			////////////////////////////
			JW2_BANISHPLAYER=0x610007,//����
			JW2_BANISHPLAYER_RESP=0x618007,//������Ӧ
			JW2_BOARDMESSAGE=0x610002,//����
			JW2_BOARDMESSAGE_RESP=0x618002,//������Ӧ
			
			JW2_RPG_QUERY=0x610010,//��ѯ������ڣ�6��
			JW2_RPG_QUERY_RESP=0x618010,//��ѯ���������Ӧ��6��
			
			JW2_ITEMSHOP_BYOWNER_QUERY = 0x610011,////�鿴������ϵ�����Ϣ��7��7��
			JW2_ITEMSHOP_BYOWNER_QUERY_RESP = 0x618011,////�鿴������ϵ�����Ϣ��Ӧ��7��7��
			
			
			JW2_DUMMONEY_QUERY = 0x610012,////��ѯ����������ң�7��8��
			JW2_DUMMONEY_QUERY_RESP = 0x618012,////��ѯ�������������Ӧ��7��8��
			JW2_HOME_ITEM_QUERY = 0x610013,////�鿴������Ʒ�嵥�����ޣ�7��9��
			JW2_HOME_ITEM_QUERY_RESP = 0x618013,////�鿴������Ʒ�嵥��������Ӧ��7��9��
			JW2_SMALL_PRESENT_QUERY = 0x610014,////�鿴�������7��10��
			JW2_SMALL_PRESENT_QUERY_RESP = 0x618014,////�鿴���������Ӧ��7��10��
			JW2_WASTE_ITEM_QUERY = 0x610015,////�鿴�����Ե��ߣ�7��11��
			JW2_WASTE_ITEM_QUERY_RESP = 0x618015,////�鿴�����Ե�����Ӧ��7��11��
			JW2_SMALL_BUGLE_QUERY = 0x610016,////�鿴С���ȣ�7��12��
			JW2_SMALL_BUGLE_QUERY_RESP  = 0x618016,////�鿴С������Ӧ��7��12��
			
			JW2_LOGINOUT_QUERY=0x610009,//����������/�ǳ���Ϣ
			JW2_LOGINOUT_QUERY_RESP=0x618009,//����������/�ǳ���Ϣ��Ӧ
			JW2_WEDDINGINFO_QUERY=0x610017,//������Ϣ
			JW2_WEDDINGINFO_QUERY_RESP=0x618017,
			JW2_WEDDINGLOG_QUERY=0x610018,//������ʷ
			JW2_WEDDINGLOG_QUERY_RESP=0x618018,
			JW2_WEDDINGGROUND_QUERY=0x610019,//�����Ϣ
			JW2_WEDDINGGROUND_QUERY_RESP=0x618019,
			JW2_COUPLEINFO_QUERY=0x610020,//������Ϣ
			JW2_COUPLEINFO_QUERY_RESP=0x618020,
			JW2_COUPLELOG_QUERY=0x610021,//������ʷ
			JW2_COUPLELOG_QUERY_RESP=0x618021,
			
			
			JW2_FAMILYINFO_QUERY=0x610022,//������Ϣ
			JW2_FAMILYINFO_QUERY_RESP=0x618022,
			JW2_FAMILYMEMBER_QUERY=0x610023,//�����Ա��Ϣ
			JW2_FAMILYMEMBER_QUERY_RESP=0x618023,
			JW2_SHOP_QUERY=0x610024,//�̳���Ϣ
			JW2_SHOP_QUERY_RESP=0x618024,
			JW2_User_Family_Query=0x610025,//�û�������Ϣ
			JW2_User_Family_Query_Resp=0x618025,
			
			JW2_FamilyItemInfo_Query=0x610026,//���������Ϣ
			JW2_FamilyItemInfo_Query_Resp=0x618026,
			
			JW2_FamilyBuyLog_Query=0x610027,//���幺����־
			JW2_FamilyBuyLog_Query_Resp=0x618027,
			
			JW2_FamilyTransfer_Query=0x610028,//����ת����Ϣ
			JW2_FamilyTransfer_Query_Resp=0x618028,

			JW2_FriendList_Query=0x610029,//�����б�
			JW2_FriendList_Query_Resp=0x618029,

			JW2_BasicInfo_Query=0x610030,//������Ϣ��ѯ
			JW2_BasicInfo_Query_Resp=0x618030,
			
			JW2_FamilyMember_Applying=0x610031,//�����г�Ա
			JW2_FamilyMember_Applying_Resp=0x618031,
			
			JW2_BasicRank_Query=0x610032,//���صȼ�
			JW2_BasicBank_Query_Resp=0x618032,
			
			
			///////////����////////////////////////////
			JW2_BOARDTASK_INSERT = 0x610033,//�������
			JW2_BOARDTASK_INSERT_RESP = 0x618033,//���������Ӧ
			JW2_BOARDTASK_QUERY = 0x610034,//�����ѯ
			JW2_BOARDTASK_QUERY_RESP = 0x618034,//�����ѯ��Ӧ
			JW2_BOARDTASK_UPDATE = 0x610035,//�������
			JW2_BOARDTASK_UPDATE_RESP = 0x618035,//���������Ӧ
			
			JW2_ITEM_DEL=0x610036,//����ɾ��������0����Ʒ��1�����2��
			JW2_ITEM_DEL_RESP=0x618036,//����ɾ��������0����Ʒ��1�����2��
			
			JW2_MODIFYSEX_QUERY=0x610037,//�޸Ľ�ɫ�Ա�
			JW2_MODIFYSEX_QUERY_RESP=0x618037,
			
			JW2_MODIFYLEVEL_QUERY=0x610038,//�޸ĵȼ�
			JW2_MODIFYLEVEL_QUERY_RESP=0x618038,
			
			JW2_MODIFYEXP_QUERY=0x610039,//�޸ľ���
			JW2_MODIFYEXP_QUERY_RESP=0x618039,
			
			JW2_BAN_BIGBUGLE=0x610040,//���ô�����
			JW2_BAN_BIGBUGLE_RESP=0x618040,
			
			JW2_BAN_SMALLBUGLE=0x610041,//����С����
			JW2_BAN_SMALLBUGLE_RESP=0x618041,
			
			JW2_DEL_CHARACTER=0x610042,//ɾ����ɫ
			JW2_DEL_CHARACTER_RESP=0x618042,
			
			JW2_RECALL_CHARACTER=0x610043,//�ָ���ɫ
			JW2_RECALL_CHARACTER_RESP=0x618043,
			
			JW2_MODIFY_MONEY=0x610044,//�޸Ľ�Ǯ
			JW2_MODIFY_MONEY_RESP=0x618044,
			
			JW2_ADD_ITEM=0x610045,//���ӵ���
			JW2_ADD_ITEM_RESP=0x618045,
			
			JW2_CREATE_GM=0x610046,//ÿ����������GM
			JW2_CREATE_GM_RESP=0x618046,
			
			JW2_MODIFY_PWD=0x610047,//�޸�����
			JW2_MODIFY_PWD_RESP=0x618047,
			
			JW2_RECALL_PWD=0x610048,//�ָ�����
			JW2_RECALL_PWD_RESP=0x618048,
			
			
			JW2_ItemInfo_Query=0x610049,//���߲�ѯ
			JW2_ItemInfo_Query_Resp=0x618049,//

			
			JW2_ITEM_SELECT=0x610050,//����ģ����ѯ
			JW2_ITEM_SELECT_RESP=0x618050,//
			
			JW2_PetInfo_Query=0x610051,//������Ϣ
			JW2_PetInfo_Query_Resp=0x618051,
			
			JW2_Messenger_Query=0x610052,//�ƺŲ�ѯ
			JW2_Messenger_Query_Resp=0x618052,
			
			JW2_Wedding_Paper=0x610053,//���֤��
			JW2_Wedding_Paper_Resp=0x618053,
			
			JW2_CoupleParty_Card=0x610054,//�����ɶԿ�
			JW2_CoupleParty_Card_Resp=0x618054,
			
			
			JW2_MailInfo_Query=0x610055,//ֽ����Ϣ
			JW2_MailInfo_Query_Resp=0x618055,
			
			JW2_MoneyLog_Query=0x610056,//��Ǯ��־��ѯ
			JW2_MoneyLog_Query_Resp=0x618056,
			
			JW2_FamilyFund_Log=0x610057,//������־
			JW2_FamilyFund_Log_Resp=0x618057,
			
			JW2_FamilyItem_Log=0x610058,//���������־
			JW2_FamilyItem_Log_Resp=0x618058,
			
			JW2_Item_Log=0x610059,//������־
			JW2_Item_Log_Resp=0x618059,

			JW2_Task_Log=0x610060,//������־
			JW2_Task_Log_Resp=0x618060,

			JW2_CashMoney_Log=0x610061,//������־
			JW2_CashMoney_Log_Resp=0x618061,

			JW2_Import_KickPlayer=0x610062,//��������
			JW2_Import_KickPlayer_Resp=0x618062,

			JW2_Import_AccountClose=0x610063,//������ͣ
			JW2_Import_AccountClose_Resp=0x618063,

			JW2_Import_AccountOpen=0x610064,//������ͣ
			JW2_Import_AccountOpen_Resp=0x618064,

			JW2_GM_Update=0x610065,//����GM״̬
			JW2_GM_Update_Resp=0x618065,
			
			JW2_ProductManage_Insert=0x610066,//����Ȩ��
			JW2_ProductManage_Insert_Resp=0x618066,
			
			JW2_ProductManage_View=0x610067,//�鿴Ȩ��
			JW2_ProductManage_View_Resp=0x618067,
			
			JW2_ProductManage_Del=0x610068,//ɾ��Ȩ��
			JW2_ProductManage_Del_Resp=0x618068,

			//maple add
			JW2_ProductManage_Validate=0x610069,//��֤Ȩ��
			JW2_ProductManage_Validate_Resp=0x618069,

			JW2_ProductManage_Deduct=0x610070,///��֤ͨ����ȥȨ��
			JW2_ProductManage_Deduct_Resp=0x618070,
			//////////////////
			//CG2(0x71)
			/////////////////	
			//��ɫ����
			CG2_Account_Query = 0x710001,//�˺Ų�ѯ��������
			CG2_Account_Query_Resp = 0x718001,//�˺Ų�ѯ��������
			CG2_Account_Detail_Query = 0x710002, //��ɫ��ϸ����
			CG2_Account_Detail_Query_Resp = 0x718002, //��ɫ��ϸ����
			CG2_Account_Attribute_Query = 0x710003,//��ɫ��������
			CG2_Account_Attribute_Query_Resp = 0x718003,//��ɫ��������
			CG2_Account_Guild_Query = 0x710004,//��ɫ��������
			CG2_Account_Guild_Query_Resp = 0x718004,//��ɫ��������
			CG2_Account_Title_Query = 0x710005,//��ɫ�ƺ�����
			CG2_Account_Title_Query_Resp = 0x718005,//��ɫ�ƺ�����
			CG2_Account_Address_Query = 0x710006,//��ɫ����ϵ
			CG2_Account_Address_Query_Resp = 0x718006,//��ɫ����ϵ
			CG2_Account_Skill_Query = 0x710007,//��ɫ������Ϣ
			CG2_Account_Skill_Query_Resp = 0x718007,//��ɫ������Ϣ
			//������Ʒ����
			CG2_CharItem_Query = 0x710008,//��ɫ���ϵ�����Ϣ
			CG2_CharItem_Query_Resp = 0x718008,//��ɫ���ϵ�����Ϣ
			CG2_TempItem_Query = 0x710009,//��ʱ������Ϣ
			CG2_TempItem_Query_Resp = 0x718009,//��ʱ������Ϣ
			CG2_ShopItem_Query = 0x710010,//�̳���Ʒ��ȡ
			CG2_ShopItem_Query_Resp = 0x718010,//�̳���Ʒ��ȡ
			//������Ʒ����
			CG2_BankInfo_Query = 0x710011,//���д����Ϣ
			CG2_BankInfo_Query_Resp = 0x718011,//���д����Ϣ
			CG2_BankItem_Query = 0x710012,//������Ʒ��Ϣ
			CG2_BankItem_Query_Resp = 0x718012,//������Ʒ��Ϣ
			//������Ϣ
			CG2_PetInfo_Query = 0x710013,//�����������
			CG2_PetInfo_Query_Resp = 0x718013,//�����������
			CG2_PetInfo_Detail_Query = 0x710014,//������ϸ����
			CG2_PetInfo_Detail_Query_Resp = 0x718014,//������ϸ����
			CG2_PetItem_Query = 0x710015,//���ﱳ����Ϣ
			CG2_PetItem_Query_Resp = 0x718015,//	���ﱳ����Ϣ
			CG2_PetSkill_Query = 0x710016,//���＼����Ϣ
			CG2_PetSkill_Query_Resp = 0x718016,//	���＼����Ϣ
			//����
			CG2_Job_Query = 0x710017,	//���� 
			CG2_Job_Query_Resp = 0x718017,//���� 
			//�˺ż����ѯ
			CG2_Account_Active_Query = 0x710018,	//ͨ���˺Ų�ѯ
			CG2_Account_Active_Query_Resp = 0x718018,//ͨ���˺Ų�ѯ
			CG2_Sn_Active_Query  = 0x710019,		//ͨ���������ѯ
			CG2_Sn_Active_Query_Resp  = 0x718019,//ͨ���������ѯ
			//ϵͳ����
			CG2_Ban_Query = 0x710020,//����ѡ��ʱ�䷶Χ����������ɸѡ����ָ���������з��������Ľ�ɫ
			CG2_Ban_Query_Resp = 0x718020,//����ѡ��ʱ�䷶Χ����������ɸѡ����ָ���������з��������Ľ�ɫ
			CG2_Account_Ban_Query = 0x710021,//����ѡ��ʱ�䷶Χ���˺š��ǳƽ��в�ѯ����ʾ���˴���
			CG2_Account_Ban_Query_Resp = 0x718021,//����ѡ��ʱ�䷶Χ���˺š��ǳƽ��в�ѯ����ʾ���˴���
			
			//�ʺŵ�½��Ϣ
			CG2_Use_Query = 0x710022,	//�ʺŵ�½��Ϣ
			CG2_Use_Query_Resp = 0x718022,//�ʺŵ�½��Ϣ
			CG2_Account_Ip_Query = 0x710023,	//ͨ��IP��ַ��ѯ����½���ʺ�
			CG2_Account_Ip_Query_Resp = 0x718023,//ͨ��IP��ַ��ѯ����½���ʺ�
			CG2_Account_User_Query = 0x710024,	//ͨ���ʺŲ�ѯ��ʷ��½IP
			CG2_Account_User_Query_Resp = 0x718024,//ͨ���ʺŲ�ѯ��ʷ��½IP
			CG2_UseLog_Query = 0x710025,	//�ʼ���־��ѯ��Ϣ
			CG2_UseLog_Query_Resp = 0x718025,//�ʼ���־��ѯ��Ϣ
			
			//������
			//����
			CG2_Get_Item_Query = 0x710026,//��ȡ��Ϸ���ߣ����ࣩ
			CG2_Get_Item_Query_Resp = 0x718026,//��ȡ��Ϸ���ߣ����ࣩ
			CG2_Get_Item_Detail_Query = 0x710027,	//��ȡ��Ϸ���ߣ�С�ࣩ
			CG2_Get_Item_Detail_Query_Resp = 0x718027,	//��ȡ��Ϸ���ߣ�С�ࣩ
			
			
			//�˺�
			CG2_Account_Close = 0x710028,//��ͣ����ʺ�
			CG2_Account_Close_RESP = 0x718028,//��ͣ����ʺ���Ӧ
			CG2_Account_Open = 0x710029,//�������ʺ�
			CG2_Account_Open_RESP= 0x718029,//�������ʺ���Ӧ
			CG2_Account_BANISHMENT_QUERY = 0x710030,//��ҷ�ͣ�ʺŲ�ѯ״̬
			CG2_Account_BANISHMENT_QUERY_RESP = 0x718030,//��ҷ�ͣ�ʺŲ�ѯ״̬��Ӧ
			CG2_BanAccount_Query = 0x710031,//��ѯ���з�ͣ����ʺ���Ϣ
			CG2_BanAccount_Query_Resp = 0x718031,//��ѯ���з�ͣ����ʺ���Ϣ
			CG2_KickAccount_Query = 0x710032,//������
			CG2_KickAccount_Query_Resp = 0x718032,//������
		
			//��ͼ
			CG2_Change_Map_Query = 0x710033,//��ͼ����
			CG2_Change_Map_Query_Resp = 0x718033,//��ͼ����

			//����
			CG2_SendNote_Query = 0x710034,//���͹���
			CG2_SendNote_Query_Resp = 0x718034,//���͹���
			CG2_UpdateNote_Query = 0x710035,//�޸Ĺ���
			CG2_UpdateNote_Query_Resp= 0x718035,//�޸Ĺ���
			CG2_DeleteNote_Query = 0x710036,//ɾ������
			CG2_DeleteNote_Query_Resp= 0x718036,//ɾ������

			

			//��ɫ
			CG2_Update_Account_Query = 0x710037,//�޸Ľ�ɫ��Ϣ
			CG2_Update_Account_Query_Resp = 0x718037,//�޸Ľ�ɫ��Ϣ
			CG2_Update_Pet_Query = 0x710038,//�޸ĳ�����Ϣ
			CG2_Update_Pet_Query_Resp = 0x718038,//�޸ĳ�����Ϣ

			//����
			CG2_Insert_Account_Skill_Query = 0x710039,//��ӽ�ɫ������Ϣ
			CG2_Insert_Account_Skill_Query_Resp = 0x718039,//��ӽ�ɫ������Ϣ
			CG2_Insert_Pet_Skill_Query = 0x710040,//��ӳ��＼����Ϣ
			CG2_Insert_Pet_Skill_Query_Resp = 0x718040,//��ӳ��＼����Ϣ

			CG2_Update_Account_Skill_Query = 0x710041,//�޸Ľ�ɫ������Ϣ
			CG2_Update_Account_Skill_Query_Resp = 0x718041,//�޸Ľ�ɫ������Ϣ
			CG2_Update_Pet_Skill_Query = 0x710042,//�޸ĳ��＼����Ϣ
			CG2_Update_Pet_Skill_Query_Resp = 0x718042,//�޸ĳ��＼����Ϣ

			CG2_Delete_Account_Skill_Query = 0x710043,//ɾ����ɫ������Ϣ
			CG2_Delete_Account_Skill_Query_Resp = 0x718043,//ɾ����ɫ������Ϣ
			CG2_Delete_Pet_Skill_Query = 0x710044,//ɾ�����＼����Ϣ
			CG2_Delete_Pet_Skill_Query_Resp = 0x718044,//ɾ�����＼����Ϣ

			//��ʱ�˺�
			CG2_Insert_TmpPwd_Query = 0x710045,//�����ʱ����
			CG2_Insert_TmpPwd_Query_Resp = 0x718045,//�����ʱ����
			CG2_Recover_TmpPwd_Query = 0x710046,//�ָ��û�����
			CG2_Recover_TmpPwd_Query_Resp = 0x718046,//�ָ��û�����


			CG2_Insert_Item_Query = 0x710047,//�����Ϸ����
			CG2_Insert_Item_Query_Resp = 0x718047,//�����Ϸ����
			CG2_RealAccount_Query = 0x710048,//��ѯ�������û���������
			CG2_RealAccount_Query_Resp = 0x718048,//��ѯ�������û���������

			CG2_SearchItem_Query = 0x710049,//ģ����ѯ����
			CG2_SearchItem_Query_Resp = 0x718049,//ģ����ѯ����

			CG2_SearchNote_Query = 0x710050,//�鿴����
			CG2_SearchNote_Query_Resp= 0x718050,//�鿴����
			CG2_MapList_Query = 0x710051,//��ѯ��ͼ�б�
			CG2_MapList_Query_Resp = 0x718051,//��ѯ��ͼ�б�
			
			CG2_SearchGuild_Query = 0x710052,//��ѯ������Ϣ
			CG2_SearchGuild_Query_Resp = 0x718052,//��ѯ������Ϣ

			CG2_SearchGuild_Detail_Query = 0x710053,//��ѯ������Ϣ
			CG2_SearchGuild_Detail_Query_Resp = 0x718053,//��ѯ������Ϣ

			CG2_CloseGuild_Query = 0x710054,//��ɢ����
			CG2_CloseGuild_Query_Resp = 0x718054,//��ɢ����

			CG2_RenameGuild_Query = 0x710055,//�޸Ĺ�������
			CG2_RenameGuild_Query_Resp = 0x718055,//�޸Ĺ�������

			CG2_Recovery_Account_Qurey = 0x710056,//�ָ���ɫ��Ϣ
			CG2_Recovery_Account_Qurey_Resp = 0x718056,//�ָ���ɫ��Ϣ

			CG2_Search_Account_Qurey = 0x710057,//��ѯ��ɫ����
			CG2_Search_Account_Qurey_Resp = 0x718057,//��ѯ��ɫ����

			CG2_BakAccount_Query = 0x710058,//��ѯ�����û�
			CG2_BakAccount_Query_Resp = 0x718058,//��ѯ�����û�

			//�����ʴ�ϵͳ
			CG2_SendAsk_Query = 0x710059,//�����ʴ�ϵͳ
			CG2_SendAsk_Query_Resp = 0x718059,//�����ʴ�ϵͳ
			
			//�޸Ľ�ɫ����
			CG2_UpdateAccount_Query = 0x710060,//�޸Ľ�ɫ����
			CG2_UpdateAccount_Query_Resp = 0x718060,//�޸Ľ�ɫ����

			//�޸Ľ�ɫ�ȼ�
			CG2_UpdateLevel_Query = 0x710061,//�޸Ľ�ɫ�ȼ�
			CG2_UpdateLevel_Query_Resp = 0x718061,//�޸Ľ�ɫ�ȼ�

			//�޸Ľ�ɫְҵ
			CG2_UpdateJob_Query = 0x710062,//�޸Ľ�ɫְҵ
			CG2_UpdateJob_Query_Resp = 0x718062,//�޸Ľ�ɫְҵ

			//��ȡ��ɫְҵ
			CG2_GetJob_Query = 0x710063,//��ȡ��ɫְҵ
			CG2_GetJob_Query_Resp = 0x718063,//��ȡ��ɫְҵ

			CG2_PackItem_Query = 0x710064,//����������Ϣ
			CG2_PackItem_Query_Resp = 0x718064,//����������Ϣ

			CG2_GetSkill_Query = 0x710065,//������＼������
			CG2_GerSkill_Query_Resp = 0x718065,//������＼������
			
			CG2_SkillList_Query = 0x710066,//������＼���б�
			CG2_SkillList_Query_Resp = 0x718066,//������＼���б�


			CG2_GetPetSkill_Query = 0x710067,//��ó��＼������
			CG2_GerPetSkill_Query_Resp = 0x718067,//��ó��＼������
			
			CG2_PetSkillList_Query = 0x710068,//��ó��＼���б�
			CG2_PetSkillList_Query_Resp = 0x718068,//��ó��＼���б�

			//�޸�����������Ϣ
			CG2_InsertBankItem_Query = 0x710069,//���е������
			CG2_InsertBankItem_Query_Resp = 0x718069,//���е������

			CG2_SetBankMoney_Query = 0x710070,//���н������
			CG2_SetBankMoney_Query_Resp = 0x718070,//���н������

			//�޸ĳ���������Ϣ
			CG2_ModiPetAttribute_Query = 0x710071,//�޸ĳ���������Ϣ
			CG2_ModiPetAttribute_Query_Resp = 0x718071,//

			CG2_PetList_Query = 0x710072,//��ȡ�����б�
			CG2_PetList_Query_Resp = 0x718072,//��ȡ�����б�

			CG2_AddPet_Query = 0x710073,//��ӳ���
			CG2_AddPet_Query_Resp = 0x718073,//��ӳ���
			//�ָ���־����
			CG2_LogItem_Query = 0x710074,//�鿴��־������Ϣ
			CG2_LogItem_Query_Resp = 0x718074,//�鿴��־������Ϣ

			CG2_RecoverLogItem_Query = 0x710075,//�ָ���־������Ϣ
			CG2_RecoverLogItem_Query_Resp = 0x718075,//�ָ���־������Ϣ

			CG2_Account_Teach_Query = 0x710076,//�鿴���ʦ����ϵ
			CG2_Account_Teach_Query_Resp = 0x718076,//�鿴���ʦ����ϵ
			CG2_Send_ItemPassword = 0x710077,//��ȡ������
			CG2_Send_ItemPassword_Resp = 0x718077,//��ȡ������

			CG2_SearchPassWord_Query = 0x710078,//��ѯ��ʱ����
			CG2_SearchPassWord_Query_Resp = 0x718078,//��ѯ��ʱ����

			CG2_InsertItemALL_Query = 0x710079,//��ӵ��ߣ����룩
			CG2_InsertItemALL_Query_Resp = 0x718079,//��ӵ��ߣ����룩

			CG2_UserShoppingInfo_Query = 0x710080,//�̳���Ϣ��ѯ
			CG2_UserShoppingInfo_Query_Resp = 0x718080,//�̳���Ϣ��ѯ

			CG2_UserGoldCard_Query = 0x710081,//�ƽ𼤻
			CG2_UserGoldCard_Query_Resp = 0x718081,//�ƽ𼤻

			CG2_InsertBankItemALL_Query = 0x710082,//�������е�����ӣ����룩
			CG2_InsertBankItemALL_Query_Resp = 0x718082,//�������е�����ӣ����룩

			CG2_SearchMap_Query = 0x710083,//ģ����ѯ��ͼ
			CG2_SearchMap_Query_Resp = 0x718083,//ģ����ѯ��ͼ

			CG2_PetPic_Query = 0x710084,//����ͼ����Ϣ
			CG2_PetPic_Query_Resp = 0x718084,//����ͼ����Ϣ

			CG2_UpdatePetLevel_Query = 0x710085,//�޸ĳ���ȼ�
			CG2_UpdatePetLevel_Query_Resp = 0x718085,//�޸ĳ���ȼ�

			CG2_BankPet_Query = 0x710086,//���г�����Ϣ
			CG2_BankPet_Query_Resp = 0x718086,//���г�����Ϣ

			CG2_UpdateGuildLv_Query=0x710087,//����ȼ��޸�
			CG2_UpdateGuildLv_Query_Resp=0x718087,//����ȼ��޸�

			CG2_UpdateGuildHonor_Query=0x710088,//���������޸�
			CG2_UpdateGuildHonor_Query_Resp=0x718088,//���������޸�

			CG2_LogInfo_Pet_Query = 0x710089,//������ϸ����
			CG2_LogInfo_Pet_Query_Resp = 0x718089,//������ϸ����

			CG2_LogInfo_Item_Query = 0x710090,//������ϸ����
			CG2_LogInfo_Item_Query_Resp = 0x718090,//������ϸ����

			CG2_AccountErr_Query = 0x710091,//����ȼ��쳣�ص�
			CG2_AccountErr_Query_Resp = 0x718091,//����ȼ��쳣�ص�

			CG2_ChangeGold_Query = 0x710092,//�޸������Ǯ
			CG2_ChangeGold_Query_Resp = 0x718092,//�޸������Ǯ

			CG2_BagItem_Del_Query = 0x710093,//ɾ����ɫ����������
			CG2_BagItem_Del_Query_Resp = 0x718093,//ɾ����ɫ����������

			CG2_TmpItem_Del_Query = 0x710094,//ɾ����ʱ����������
			CG2_TmpItem_Del_Query_Resp = 0x718094,//ɾ����ʱ����������

			CG2_EquipItem_Del_Query = 0x710095,//ɾ������װ������
			CG2_EquipItem_Del_Query_Resp = 0x718095,//ɾ������װ������

			CG2_BankItem_Del_Query = 0x710096,//ɾ�����е���
			CG2_BankItem_Del_Query_Resp = 0x718096,//ɾ�����е���

			CG2_PetPic_Del_Query = 0x710097,//����/ɾ������ͼ��
			CG2_PetPic_Del_Query_Resp = 0x718097,//����/ɾ������ͼ��

			CG2_KickAccountCount_Query = 0x710098,//�����ߴ���
			CG2_KickAccountCount_Query_Resp = 0x718098,//�����ߴ���

			CG2_RecoverLogPet_Query = 0x710099,//�ָ���־����
			CG2_RecoverLogPet_Query_Resp = 0x718099,//�ָ���־����

			CG2_LoveTree_Query = 0x710100,//��������Ϣ��ѯ
			CG2_LoveTree_Query_Resp = 0x718100,//��������Ϣ��ѯ

			CG2_GuildMazeWar_Query = 0x710101,//�����Թ�ռ��
			CG2_GuildMazeWar_Query_Resp = 0x718101,//�����Թ�ռ��

			CG2_Marriage_Info_Query = 0x710102,//��һ�����Ϣ
			CG2_Marriage_Info_Query_Resp = 0x718102,//��һ�����Ϣ

			CG2_ChangeCharNo_Query = 0x710103,//��ɫ��λ
			CG2_ChangeCharNo_Query_Resp = 0x718103,//��ɫ��λ
				
			CG2_Log_Query=0x710104,
			CG2_Log_Query_Resp=0x718104,////��ѯ�����־��Ϣ

			CG2_Get_EmailAddress=0x710105,//�õ�Email��ַ
			CG2_Get_EmailAddress_Resp=0x718105,//


			/// <summary>
            /// ��������GM����(0x86)
            /// </summary>
            SDO_ACCOUNT_QUERY = 0x870026,//�鿴��ҵ��ʺ���Ϣ
            SDO_ACCOUNT_QUERY_RESP = 0x878026,//�鿴��ҵ��ʺ���Ϣ��Ӧ
            SDO_CHARACTERINFO_QUERY = 0x870027,//�鿴�������ϵ���Ϣ
            SDO_CHARACTERINFO_QUERY_RESP = 0x878027,//�鿴�������ϵ���Ϣ��Ӧ
            SDO_ACCOUNT_CLOSE = 0x870028,//��ͣ�ʻ���Ȩ����Ϣ
            SDO_ACCOUNT_CLOSE_RESP = 0x878028,//��ͣ�ʻ���Ȩ����Ϣ��Ӧ
            SDO_ACCOUNT_OPEN = 0x870029,//����ʻ���Ȩ����Ϣ
            SDO_ACCOUNT_OPEN_RESP = 0x878029,//����ʻ���Ȩ����Ϣ��Ӧ
            SDO_PASSWORD_RECOVERY = 0x870030,//����һ�����
            SDO_PASSWORD_RECOVERY_RESP = 0x878030,//����һ�������Ӧ
            SDO_CONSUME_QUERY = 0x870031,//�鿴��ҵ����Ѽ�¼
            SDO_CONSUME_QUERY_RESP = 0x878031,//�鿴��ҵ����Ѽ�¼��Ӧ
            SDO_USERONLINE_QUERY = 0x870032,//�鿴���������״̬
            SDO_USERONLINE_QUERY_RESP = 0x878032,//�鿴���������״̬��Ӧ
            SDO_USERTRADE_QUERY = 0x870033,//�鿴��ҽ���״̬
            SDO_USERTRADE_QUERY_RESP = 0x878033,//�鿴��ҽ���״̬��Ӧ
            SDO_CHARACTERINFO_UPDATE = 0x870034,//�޸���ҵ��˺���Ϣ
            SDO_CHARACTERINFO_UPDATE_RESP = 0x878034,//�޸���ҵ��˺���Ϣ��Ӧ
            SDO_ITEMSHOP_QUERY = 0x870035,//�鿴��Ϸ�������е�����Ϣ
            SDO_ITEMSHOP_QUERY_RESP = 0x878035,//�鿴��Ϸ�������е�����Ϣ��Ӧ
            SDO_ITEMSHOP_DELETE = 0x870036,//ɾ����ҵ�����Ϣ
            SDO_ITEMSHOP_DELETE_RESP = 0x878036,//ɾ����ҵ�����Ϣ��Ӧ
            SDO_GIFTBOX_CREATE = 0x870037,//����������е�����Ϣ
            SDO_GIFTBOX_CREATE_RESP = 0x878037,//����������е�����Ϣ��Ӧ
            SDO_GIFTBOX_QUERY = 0x870038,//�鿴�������еĵ���
            SDO_GIFTBOX_QUERY_RESP = 0x878038,//�鿴�������еĵ�����Ӧ
            SDO_GIFTBOX_DELETE = 0x870039,//ɾ���������еĵ���
            SDO_GIFTBOX_DELETE_RESP = 0x878039,//ɾ���������еĵ�����Ӧ
            SDO_USERLOGIN_STATUS_QUERY = 0x870040,//�鿴��ҵ�¼״̬
            SDO_USERLOGIN_STATUS_QUERY_RESP = 0x878040,//�鿴��ҵ�¼״̬��Ӧ
            SDO_ITEMSHOP_BYOWNER_QUERY = 0x870041,////�鿴������ϵ�����Ϣ
            SDO_ITEMSHOP_BYOWNER_QUERY_RESP = 0x878041,////�鿴������ϵ�����Ϣ
            SDO_ITEMSHOP_TRADE_QUERY = 0x870042,//�鿴��ҽ��׼�¼��Ϣ
            SDO_ITEMSHOP_TRADE_QUERY_RESP = 0x878042,//�鿴��ҽ��׼�¼��Ϣ����Ӧ
            SDO_MEMBERSTOPSTATUS_QUERY = 0x870043,//�鿴���ʺ�״̬
            SDO_MEMBERSTOPSTATUS_QUERY_RESP = 0x878043,///�鿴���ʺ�״̬����Ӧ
            SDO_MEMBERBANISHMENT_QUERY = 0x870044,//�鿴����ͣ����ʺ�
            SDO_MEMBERBANISHMENT_QUERY_RESP = 0x878044,//�鿴����ͣ����ʺ���Ӧ
            SDO_USERMCASH_QUERY = 0x870045,//��ҳ�ֵ��¼��ѯ
            SDO_USERMCASH_QUERY_RESP = 0x878045,//��ҳ�ֵ��¼��ѯ��Ӧ
            SDO_USERGCASH_UPDATE = 0x870046,//�������G��
            SDO_USERGCASH_UPDATE_RESP = 0x878046,//�������G�ҵ���Ӧ
            SDO_MEMBERLOCAL_BANISHMENT = 0x870047,//���ر���ͣ����Ϣ
            SDO_MEMBERLOCAL_BANISHMENT_RESP = 0x878047,//���ر���ͣ����Ϣ��Ӧ
            SDO_EMAIL_QUERY = 0x870048,//�õ���ҵ�EMAIL
            SDO_EMAIL_QUERY_RESP = 0x878048,//�õ���ҵ�EMAIL��Ӧ
            SDO_USERCHARAGESUM_QUERY = 0x870049,//�õ���ֵ��¼�ܺ�
            SDO_USERCHARAGESUM_QUERY_RESP = 0x878049,//�õ���ֵ��¼�ܺ���Ӧ
            SDO_USERCONSUMESUM_QUERY = 0x870050,//�õ����Ѽ�¼�ܺ�
            SDO_USERCONSUMESUM_QUERY_RESP = 0x878050,//�õ����Ѽ�¼�ܺ���Ӧ
            SDO_USERGCASH_QUERY = 0x870051,//��ңǱҼ�¼��ѯ
            SDO_USERGCASH_QUERY_RESP = 0x878051,//��ңǱҼ�¼��ѯ��Ӧ

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
            SDO_Family_CONSUME_QUERY = 0x870125,//�������Ѳ�ѯ
            SDO_Family_CONSUME_QUERY_RESP = 0x878125,//�������Ѳ�ѯ
            SDO_Family_Info_QUERY = 0x870126,//���������Ϣ
            SDO_Family_Info_QUERY_RESP = 0x878126,//���������Ϣ
            SDO_FamilyMember_Query = 0x870127,//�����Ա��Ϣ
            SDO_FamilyMember_Query_RESP = 0x878127,//�����Ա��Ϣ
            SDO_FamilyFormal_Query = 0x870128,//�����г�Ա��Ϣ
            SDO_FamilyFormal_Query_RESP = 0x878128,//�����г�Ա��Ϣ
            SDO_Personal_Emblem_Query = 0x870129,//����ѫ��
            SDO_Personal_Emblem_Query_RESP = 0x878129,//����ѫ��
            SDO_Family_Donate_Query = 0x870130,//�����������ϸ
            SDO_Family_Donate_Query_RES = 0x878130,//�����������ϸ
            SDO_Family_Delete_Query = 0x870131,//��ɢ����
            SDO_Family_Delete_Query_RESP = 0x878131,//��ɢ����

			SDO_Family_Duty_Query = 0x870132,//�����Աְ���޸�
            SDO_Family_Duty_Query_RESP = 0x878132,//�����Աְ���޸�
            SDO_Family_Emblem_Insert_Query = 0x870133,//����Ӽ������
            SDO_Family_Emblem_Insert_Query_RESP = 0x878133,//����Ӽ������
			
            SDO_Family_Emblem_Delete_Query = 0x870134,//��ɾ���������
            SDO_Family_Emblem_Delete_Query_RESP = 0x878134,//��ɾ���������

			//maple add
			SDO_Pet_Update= 0x870135,//ɾ����ҳ���
			SDO_Pet_Update_RESP =0x878135,
			SDO_Social_Update=0x870136,//�޸������罻ֵ
			SDO_Social_Update_Resp=0x878136,
			SDO_Lucky_Update=0x870137,//�޸���������ֵ
			SDO_Lucky_Update_Resp=0x878137,


			Ban_Open_Query=0x990001,//����˺�
			Ban_Open_Query_Resp=0x998001,//����˺Ż�Ӧ

			Ban_Close_Query=0x990002,//��ͣ�˺�
			Ban_Close_Query_Resp=0x998002,//��ͣ�˺Ż�Ӧ

			Ban_AccountList_Query=0x990003,//�˺��б��ѯ
			Ban_AccountList_Query_Resp=0x998003,//�˺��б��ѯ��Ӧ

			Ban_Account_Query=0x990004,//�˺Ų�ѯ
			Ban_Account_Query_Resp=0x998004,//�˺Ų�ѯ��Ӧ

			Ban_UserInfo_Query=0x990005,//�ʻ���Ϣ��ѯ
			Ban_UserInfo_Query_Resp=0x998005,//�ʻ���Ϣ��ѯ��Ӧ
			
			NOTDEFINED = 0x0,
			MESSAGE = 0xEEEEE
		};  
		__Enum _value; // ö��ֵ  
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
