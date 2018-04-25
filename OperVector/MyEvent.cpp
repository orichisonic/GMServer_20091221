#include "stdafx.h"
#include "MyEvent.h"

CMyEvent::CMyEvent()
{
}

CMyEvent::~CMyEvent()
{
}

//获取TagFormat
CEnumCore::TagFormat CMyEvent::GetTagFormat(char* m_TagFormat)
{
	int iTagFormat = atoi(m_TagFormat);//字符串变成int
	switch(iTagFormat)
	{
	case 200:
		return CEnumCore::TagFormat::TLV_STRING;
	case 4:
		return CEnumCore::TagFormat::TLV_INTEGER;
	case 135:
		return CEnumCore::TagFormat::TLV_DATE;
	case 17:
		return CEnumCore::TagFormat::TLV_BOOLEAN;
	default:
		return CEnumCore::TagFormat::TLV_STRING;
	}
}

//获取TagName
CEnumCore::TagName CMyEvent::GetTagName(char* fieldname)
{
	if(!strcmp(fieldname,"UserByID"))
	{
		return CEnumCore::TagName::UserByID;
	}
	else if(!strcmp(fieldname,"serverIP"))
	{
		return CEnumCore::TagName::ServerInfo_IP;
	}
	else if(!strcmp(fieldname,"Game_Name"))
	{	
		return CEnumCore::TagName::GameName;
	}
	else if(!strcmp(fieldname,"game_Name"))
	{	
		return CEnumCore::TagName::GameName;
	}
	else if(!strcmp(fieldname,"gameName"))
	{
		return CEnumCore::TagName::GameName;
	}
	else if(!strcmp(fieldname,"ServerInfo_GameDBID"))
	{	
		return CEnumCore::TagName::ServerInfo_GameDBID;
	}
	
	else if(!strcmp(fieldname,"gameName"))
	{
		return CEnumCore::TagName::GameName;
	}
	else if(!strcmp(fieldname,"city"))
	{
		return CEnumCore::TagName::ServerInfo_City;
	}
	else if(!strcmp(fieldname,"serverinfo_gamename"))
	{
		return CEnumCore::TagName::ServerInfo_GameName;
	}
	else if(!strcmp(fieldname,"Real_Act"))
	{
		return CEnumCore::TagName::Real_ACT;
	}
	else if(!strcmp(fieldname,"Act_Time"))
	{
		return CEnumCore::TagName::ACT_Time;
	}
	else if(!strcmp(fieldname,"module_Name"))
	{
		return CEnumCore::TagName::ModuleName;
	}
	else if(!strcmp(fieldname,"module_Class"))
	{
		return CEnumCore::TagName::ModuleClass;
	}
	else if(!strcmp(fieldname,"userID"))
	{
		return CEnumCore::TagName::User_ID;
	}
	else if(!strcmp(fieldname,"userName"))
	{
		return CEnumCore::TagName::UserName;
	}
	else if(!strcmp(fieldname,"user_Pwd"))
	{
		return CEnumCore::TagName::PassWord;
	}
	else if(!strcmp(fieldname,"user_mac"))
	{
		return CEnumCore::TagName::MAC;
	}
	else if(!strcmp(fieldname,"realName"))
	{
		return CEnumCore::TagName::RealName;
	}
	else if(!strcmp(fieldname,"RealName"))
	{
		return CEnumCore::TagName::RealName;
	}
	else if(!strcmp(fieldname,"DepartID"))
	{
		return CEnumCore::TagName::DepartID;
	}
	else if(!strcmp(fieldname,"departName"))
	{
		return CEnumCore::TagName::DepartName;
	}
	else if(!strcmp(fieldname,"user_Status"))
	{
		return CEnumCore::TagName::User_Status;
	}
	else if(!strcmp(fieldname,"limit"))
	{
		return CEnumCore::TagName::Limit;
	}
	else if(!strcmp(fieldname,"Onlineactive"))
	{
		return CEnumCore::TagName::OnlineActive;
	}
	else if(!strcmp(fieldname,"SysAdmin"))
	{
		return CEnumCore::TagName::SysAdmin;
	}
	else if(!strcmp(fieldname,"userIP"))
	{
		return CEnumCore::TagName::userIP;
	}
	else if(!strcmp(fieldname,"Game_ID"))
	{
		return CEnumCore::TagName::GameID;
	}
	else if(!strcmp(fieldname,"gameID"))
	{
		return CEnumCore::TagName::GameID;
	}
	else if(!strcmp(fieldname,"Module_ID"))
	{
		return CEnumCore::TagName::Module_ID;
	}
	else if(!strcmp(fieldname,"Module_Name"))
	{
		return CEnumCore::TagName::ModuleName;
	}
	else if(!strcmp(fieldname,"PageCount"))
	{
		return CEnumCore::TagName::PageCount;
	}

	//Added by tengjie 2010-05-05:外挂进程
	if(!strcmp(fieldname,"Account")		|| 
	   !strcmp(fieldname,"account")	|| 
	   !strcmp(fieldname,"accountname")	|| 
	   !strcmp(fieldname,"account_name"))
	{
		return CEnumCore::TagName::PlayerAccount;
	}
	if(!strcmp(fieldname,"char_name") ||
	   !strcmp(fieldname,"character_name") ||
	   !strcmp(fieldname,"role_id"))
	{
		return CEnumCore::TagName::PlayerName;
	}
	if(!strcmp(fieldname,"harm_full_path")		|| 
	   !strcmp(fieldname,"virusfilefullpath")	|| 
	   !strcmp(fieldname,"szvirusfilefullpath") ||
	   !strcmp(fieldname,"virus_path") ||
	   !strcmp(fieldname,"viruspath"))
	{
		return CEnumCore::TagName::ToolProcPath;
	}
	if(!strcmp(fieldname,"kick_time") || 
	   !strcmp(fieldname,"kicktime") ||
	   !strcmp(fieldname,"create_time") ||
	   !strcmp(fieldname,"time"))
	{
		return CEnumCore::TagName::KickProcTime;
	}
	if(!strcmp(fieldname,"realm_Name"))
	{
		return CEnumCore::TagName::PlayerInServer;
	}
	//end

	if(!strcmp(fieldname,"Description"))
	{
		return CEnumCore::TagName::WA_Content;
	}

	//////////仙剑///////////////////////
	if(!strcmp(fieldname,"pal_worldid"))
	{
		return CEnumCore::TagName::PAL_WORLDID;
	}
	if(!strcmp(fieldname,"pal_playercount"))
	{
		return CEnumCore::TagName::PAL_PLAYERCOUNT;
	}
	if(!strcmp(fieldname,"pal_account"))
	{
		return CEnumCore::TagName::PAL_ACCOUNT;
	}
	if(!strcmp(fieldname,"pal_rolename"))
	{
		return CEnumCore::TagName::PAL_ROLENAME;
	}
	if(!strcmp(fieldname,"pal_bagtype"))
	{
		return CEnumCore::TagName::PAL_BAGTYPE;
	}
	if(!strcmp(fieldname,"pal_itemname"))
	{
		return CEnumCore::TagName::PAL_ITEMNAME;
	}
	if(!strcmp(fieldname,"pal_itempos"))
	{
		return CEnumCore::TagName::PAL_ITEMPOS;
	}
	if(!strcmp(fieldname,"pal_itemtype"))
	{
		return CEnumCore::TagName::PAL_ITEMTYPE;
	}
	if(!strcmp(fieldname,"pal_orgobjid"))
	{
		return CEnumCore::TagName::PAL_ORGOBJID;
	}
	if(!strcmp(fieldname,"pal_localtime"))
	{
		return CEnumCore::TagName::PAL_LOCALTIME;
	}
	if(!strcmp(fieldname,"pal_itemserial"))
	{
		return CEnumCore::TagName::PAL_ITEMSERIAL;
	}
	if(!strcmp(fieldname,"pal_livetime"))
	{
		return CEnumCore::TagName::PAL_LIVETIME;
	}
	if(!strcmp(fieldname,"pal_cooldown"))
	{
		return CEnumCore::TagName::PAL_COOLDOWN;
	}
	if(!strcmp(fieldname,"pal_npctrade"))
	{
		return CEnumCore::TagName::PAL_NPCTRADE;
	}
	if(!strcmp(fieldname,"pal_usertrade"))
	{
		return CEnumCore::TagName::PAL_USERTRADE;
	}
	if(!strcmp(fieldname,"pal_itemmallgoods"))
	{
		return CEnumCore::TagName::PAL_ITEMMALLGOODS;
	}
	if(!strcmp(fieldname,"pal_itemcount"))
	{
		return CEnumCore::TagName::PAL_ITEMCOUNT;
	}
	if(!strcmp(fieldname,"pal_itemsellcost"))
	{
		return CEnumCore::TagName::PAL_ITEMSELLCOST;
	}
	if(!strcmp(fieldname,"pal_itemdurable"))
	{
		return CEnumCore::TagName::PAL_ITEMDURABLE;
	}
	if(!strcmp(fieldname,"pal_inheritid0"))
	{
		return CEnumCore::TagName::PAL_INHERITID0;
	}
	if(!strcmp(fieldname,"pal_inheritid1"))
	{
		return CEnumCore::TagName::PAL_INHERITID1;
	}
	if(!strcmp(fieldname,"pal_inheritid2"))
	{
		return CEnumCore::TagName::PAL_INHERITID2;
	}
	if(!strcmp(fieldname,"pal_inheritid3"))
	{
		return CEnumCore::TagName::PAL_INHERITID3;
	}
	if(!strcmp(fieldname,"pal_inheritid4"))
	{
		return CEnumCore::TagName::PAL_INHERITID4;
	}
	if(!strcmp(fieldname,"pal_inheritid5"))
	{
		return CEnumCore::TagName::PAL_INHERITID5;
	}
	if(!strcmp(fieldname,"pal_inheritid6"))
	{
		return CEnumCore::TagName::PAL_INHERITID6;
	}
	if(!strcmp(fieldname,"pal_inheritid7"))
	{
		return CEnumCore::TagName::PAL_INHERITID7;
	}
	if(!strcmp(fieldname,"pal_inheritid8"))
	{
		return CEnumCore::TagName::PAL_INHERITID8;
	}
	if(!strcmp(fieldname,"pal_inheritid9"))
	{
		return CEnumCore::TagName::PAL_INHERITID9;
	}
	if(!strcmp(fieldname,"pal_petexp"))
	{
		return CEnumCore::TagName::PAL_PETEXP;
	}
	if(!strcmp(fieldname,"pal_petnowhp"))
	{
		return CEnumCore::TagName::PAL_PETNOWHP;
	}
	if(!strcmp(fieldname,"pal_petindex"))
	{
		return CEnumCore::TagName::PAL_PETINDEX;
	}
	if(!strcmp(fieldname,"pal_petrenewcount"))
	{
		return CEnumCore::TagName::PAL_PETRENEWCOUNT;
	}
	if(!strcmp(fieldname,"pal_petlv"))
	{
		return CEnumCore::TagName::PAL_PETLV;
	}
	if(!strcmp(fieldname,"pal_petgeneration"))
	{
		return CEnumCore::TagName::PAL_PETGENERATION;
	}
	if(!strcmp(fieldname,"pal_petmixcount"))
	{
		return CEnumCore::TagName::PAL_PETMIXCOUNT;
	}
	if(!strcmp(fieldname,"pal_petgrowthrate"))
	{
		return CEnumCore::TagName::PAL_PETGROWTHRATE;
	}
	if(!strcmp(fieldname,"pal_petattrquality"))
	{
		return CEnumCore::TagName::PAL_PETATTRQUALITY;
	}
	if(!strcmp(fieldname,"pal_pethp"))
	{
		return CEnumCore::TagName::PAL_PETHP;
	}
	if(!strcmp(fieldname,"pal_petwushu"))
	{
		return CEnumCore::TagName::PAL_PETWUSHU;
	}
	if(!strcmp(fieldname,"pal_petspirit"))
	{
		return CEnumCore::TagName::PAL_PETSPIRIT;
	}
	if(!strcmp(fieldname,"pal_petconstitution"))
	{
		return CEnumCore::TagName::PAL_PETCONSTITUTION;
	}
	if(!strcmp(fieldname,"pal_petquality"))
	{
		return CEnumCore::TagName::PAL_PETQUALITY;
	}
	if(!strcmp(fieldname,"pal_petdexterity"))
	{
		return CEnumCore::TagName::PAL_PETDEXTERITY;
	}
	if(!strcmp(fieldname,"pal_petfate"))
	{
		return CEnumCore::TagName::PAL_PETFATE;
	}
	if(!strcmp(fieldname,"pal_pethpplus"))
	{
		return CEnumCore::TagName::PAL_PETHPPLUS;
	}
	if(!strcmp(fieldname,"pal_petwushuplus"))
	{
		return CEnumCore::TagName::PAL_PETWUSHUPLUS;
	}
	if(!strcmp(fieldname,"pal_petspiritplus"))
	{
		return CEnumCore::TagName::PAL_PETSPIRITPLUS;
	}
	if(!strcmp(fieldname,"pal_petconstitutionplus"))
	{
		return CEnumCore::TagName::PAL_PETCONSTITUTIONPLUS;
	}
	if(!strcmp(fieldname,"pal_petqualityplus"))
	{
		return CEnumCore::TagName::PAL_PETQUALITYPLUS;
	}
	if(!strcmp(fieldname,"pal_petdexterityplus"))
	{
		return CEnumCore::TagName::PAL_PETDEXTERITYPLUS;
	}
	if(!strcmp(fieldname,"pal_petfateplus"))
	{
		return CEnumCore::TagName::PAL_PETFATEPLUS;
	}
	if(!strcmp(fieldname,"pal_petskill0"))
	{
		return CEnumCore::TagName::PAL_PETSKILL0;
	}
	if(!strcmp(fieldname,"pal_petskill1"))
	{
		return CEnumCore::TagName::PAL_PETSKILL1;
	}
	if(!strcmp(fieldname,"pal_petskill2"))
	{
		return CEnumCore::TagName::PAL_PETSKILL2;
	}
	if(!strcmp(fieldname,"pal_petskill3"))
	{
		return CEnumCore::TagName::PAL_PETSKILL3;
	}
	if(!strcmp(fieldname,"pal_petskill4"))
	{
		return CEnumCore::TagName::PAL_PETSKILL4;
	}
	if(!strcmp(fieldname,"pal_petskill5"))
	{
		return CEnumCore::TagName::PAL_PETSKILL5;
	}
	if(!strcmp(fieldname,"pal_friendname"))
	{
		return CEnumCore::TagName::PAL_FRIENDNAME;
	}
	if(!strcmp(fieldname,"pal_relation"))
	{
		return CEnumCore::TagName::PAL_RELATION;
	}
	if(!strcmp(fieldname,"pal_nickname"))
	{
		return CEnumCore::TagName::PAL_NICKNAME;
	}
	if(!strcmp(fieldname,"pal_guildname"))
	{
		return CEnumCore::TagName::PAL_GUILDNAME;
	}
	if(!strcmp(fieldname,"pal_guildpowerlevel"))
	{
		return CEnumCore::TagName::PAL_GUILDPOWERLEVEL;
	}
	if(!strcmp(fieldname,"pal_voc"))
	{
		return CEnumCore::TagName::PAL_VOC;
	}
	if(!strcmp(fieldname,"pal_level"))
	{
		return CEnumCore::TagName::PAL_LEVEL;
	}
	if(!strcmp(fieldname,"pal_exp"))
	{
		return CEnumCore::TagName::PAL_EXP;
	}
	if(!strcmp(fieldname,"pal_bodymoney"))
	{
		return CEnumCore::TagName::PAL_BODYMONEY;
	}
	if(!strcmp(fieldname,"pal_bankmoney"))
	{
		return CEnumCore::TagName::PAL_BANKMONEY;
	}
	if(!strcmp(fieldname,"pal_sex"))
	{
		return CEnumCore::TagName::PAL_SEX;
	}
	if(!strcmp(fieldname,"pal_fame"))
	{
		return CEnumCore::TagName::PAL_FAME;
	}
	if(!strcmp(fieldname,"pal_hp"))
	{
		return CEnumCore::TagName::PAL_HP;
	}
	if(!strcmp(fieldname,"pal_mp"))
	{
		return CEnumCore::TagName::PAL_MP;
	}
	if(!strcmp(fieldname,"pal_ap"))
	{
		return CEnumCore::TagName::PAL_AP;
	}
	if(!strcmp(fieldname,"pal_ep"))
	{
		return CEnumCore::TagName::PAL_EP;
	}
	if(!strcmp(fieldname,"pal_skillpoint"))
	{
		return CEnumCore::TagName::PAL_SKILLPOINT;
	}
	if(!strcmp(fieldname,"pal_skilltreenode"))
	{
		return CEnumCore::TagName::PAL_SKILLTREENODE;
	}
	if(!strcmp(fieldname,"pal_reincarnation"))
	{
		return CEnumCore::TagName::PAL_REINCARNATION;
	}
	if(!strcmp(fieldname,"pal_dbid"))
	{
		return CEnumCore::TagName::PAL_DBID;
	}
	if(!strcmp(fieldname,"pal_zoneid"))
	{
		return CEnumCore::TagName::PAL_ZONEID;
	}
	if(!strcmp(fieldname,"pal_zonename"))
	{
		return CEnumCore::TagName::PAL_ZONENAME;
	}
	if(!strcmp(fieldname,"pal_logintime"))
	{
		return CEnumCore::TagName::PAL_LOGINTIME;
	}
	if(!strcmp(fieldname,"pal_loginip"))
	{
		return CEnumCore::TagName::PAL_LOGINIP;
	}
	if(!strcmp(fieldname,"pal_questtype"))
	{
		return CEnumCore::TagName::PAL_QUESTTYPE;
	}
	if(!strcmp(fieldname,"pal_questname"))
	{
		return CEnumCore::TagName::PAL_QUESTNAME;
	}
	if(!strcmp(fieldname,"pal_questid"))
	{
		return CEnumCore::TagName::PAL_QUESTID;
	}
	if(!strcmp(fieldname,"pal_bulletinid"))
	{
		return CEnumCore::TagName::PAL_BULLETINID;
	}
	if(!strcmp(fieldname,"pal_guildlevel"))
	{
		return CEnumCore::TagName::PAL_GUILDLEVEL;
	}
	if(!strcmp(fieldname,"pal_guildscore"))
	{
		return CEnumCore::TagName::PAL_GUILDSCORE;
	}
	if(!strcmp(fieldname,"pal_guildmoney"))
	{
		return CEnumCore::TagName::PAL_GUILDMONEY;
	}
	if(!strcmp(fieldname,"pal_guildjointime"))
	{
		return CEnumCore::TagName::PAL_GUILDJOINTIME;
	}
	if(!strcmp(fieldname,"pal_newguildname"))
	{
		return CEnumCore::TagName::PAL_NEWGUILDNAME;
	}
	if(!strcmp(fieldname,"pal_bulletinmonth"))
	{
		return CEnumCore::TagName::PAL_BULLETINMONTH;
	}
	if(!strcmp(fieldname,"pal_bulletinday"))
	{
		return CEnumCore::TagName::PAL_BULLETINDAY;
	}
	if(!strcmp(fieldname,"pal_bulletindatofweek"))
	{
		return CEnumCore::TagName::PAL_BULLETINDAYOFWEEK;

	}
	if(!strcmp(fieldname,"pal_bulletinhour"))
	{
		return CEnumCore::TagName::PAL_BULLETINHOUR;

	}
	if(!strcmp(fieldname,"pal_bulletinminute"))
	{
		return CEnumCore::TagName::PAL_BULLETINMINUTE;

	}
	if(!strcmp(fieldname,"pal_bulletincontent"))
	{
		return CEnumCore::TagName::PAL_BULLETINCONTENT;

	}
	if(!strcmp(fieldname,"pal_mailsubject"))
	{
		return CEnumCore::TagName::PAL_MAILSUBJECT;
	}

	if(!strcmp(fieldname,"pal_mailcontent"))
	{
		return CEnumCore::TagName::PAL_MAILCONTENT;
	}

	if(!strcmp(fieldname,"pal_mailmoney"))
	{
		return CEnumCore::TagName::PAL_MAILMONEY;
	}

	if(!strcmp(fieldname,"pal_mailreason"))
	{
		return CEnumCore::TagName::PAL_MAILREASON;
	}
	if(!strcmp(fieldname,"pal_mailgiftdbid0"))
	{
		return CEnumCore::TagName::PAL_MAILGIFTDBID0;
	}
	if(!strcmp(fieldname,"pal_mailgiftdbid1"))
	{
		return CEnumCore::TagName::PAL_MAILGIFTDBID1;
	}
	if(!strcmp(fieldname,"pal_mailgiftdbid2"))
	{
		return CEnumCore::TagName::PAL_MAILGIFTDBID2;
	}
	if(!strcmp(fieldname,"pal_mailgiftdbid3"))
	{
		return CEnumCore::TagName::PAL_MAILGIFTDBID3;
	}
	if(!strcmp(fieldname,"pal_mailgiftdbid4"))
	{
		return CEnumCore::TagName::PAL_MAILGIFTDBID4;
	}
	if(!strcmp(fieldname,"pal_delaytime"))
	{
		return CEnumCore::TagName::PAL_DELAYTIME;
	}
	if(!strcmp(fieldname,"pal_reason"))
	{
		return CEnumCore::TagName::PAL_REASON;
	}
	if(!strcmp(fieldname,"pal_boardmessage"))
	{
		return CEnumCore::TagName::PAL_BOARDMESSAGE;
	}
	if(!strcmp(fieldname,"pal_endtime"))
	{
		return CEnumCore::TagName::PAL_ENDTIME;
	}
	if(!strcmp(fieldname,"pal_begintime"))
	{
		return CEnumCore::TagName::PAL_BEGINTIME;
	}
	if(!strcmp(fieldname,"pal_interval"))
	{
		return CEnumCore::TagName::PAL_INTERVAL;
	}
	if(!strcmp(fieldname,"pal_taskid"))
	{
		return CEnumCore::TagName::PAL_TASKID;
	}
	if(!strcmp(fieldname,"pal_status"))
	{
		return CEnumCore::TagName::PAL_STATUS;
	}
	if(!strcmp(fieldname,"pal_guildnotice"))
	{
		return CEnumCore::TagName::PAL_GUILDNOTICE;
	}
	if(!strcmp(fieldname,"pal_bankpw"))
	{
		return CEnumCore::TagName::PAL_BANKPW;
	}
	if(!strcmp(fieldname,"pal_droprolepw"))
	{
		return CEnumCore::TagName::PAL_DROPROLEPW;
	}
	if(!strcmp(fieldname,"pal_moneytype"))
	{
		return CEnumCore::TagName::PAL_MONEYTYPE;
	}
	if(!strcmp(fieldname,"pal_moneycount"))
	{
		return CEnumCore::TagName::PAL_MONEYCOUNT;
	}
	if(!strcmp(fieldname,"pal_accountid"))
	{
		return CEnumCore::TagName::PAL_ACCOUNTID;
	}
	if(!strcmp(fieldname,"pal_logouttime"))
	{
		return CEnumCore::TagName::PAL_LOGOUTTIME;
	}
	if(!strcmp(fieldname,"pal_roleid"))
	{
		return CEnumCore::TagName::PAL_ROLEID;
	}

	if(!strcmp(fieldname,"pal_eventtype"))
	{
		return CEnumCore::TagName::PAL_EVENTTYPE;
	}
	if(!strcmp(fieldname,"pal_eventname"))
	{
		return CEnumCore::TagName::PAL_EVENTNAME;
	}
	if(!strcmp(fieldname,"pal_fromid"))
	{
		return CEnumCore::TagName::PAL_FROMID;
	}
	if(!strcmp(fieldname,"pal_fromname"))
	{
		return CEnumCore::TagName::PAL_FROMNAME;
	}
	if(!strcmp(fieldname,"pal_officerdbid"))
	{
		return CEnumCore::TagName::PAL_OFFICERDBID;
	}
	if(!strcmp(fieldname,"pal_officername"))
	{
		return CEnumCore::TagName::PAL_OFFICERNAME;
	}
	if(!strcmp(fieldname,"pal_officerzoneid"))
	{
		return CEnumCore::TagName::PAL_OFFICERZONEID;
	}
	if(!strcmp(fieldname,"pal_officerzonename"))
	{
		return CEnumCore::TagName::PAL_OFFICERZONENAME;
	}
	if(!strcmp(fieldname,"pal_logbigtypeid"))
	{
		return CEnumCore::TagName::PAL_LogBigTypeID;
	}
	if(!strcmp(fieldname,"pal_logbigtype"))
	{
		return CEnumCore::TagName::PAL_LogBigType;
	}
	if(!strcmp(fieldname,"pal_logsmalltypeid"))
	{
		return CEnumCore::TagName::PAL_LogSmallTypeID;
	}
	if(!strcmp(fieldname,"pal_logsmalltype"))
	{
		return CEnumCore::TagName::PAL_LogSmallType;
	}
	if(!strcmp(fieldname,"pal_x"))
	{
		return CEnumCore::TagName::PAL_X;
	}
	if(!strcmp(fieldname,"pal_y"))
	{
		return CEnumCore::TagName::PAL_Y;
	}
	if(!strcmp(fieldname,"pal_z"))
	{
		return CEnumCore::TagName::PAL_Z;
	}
	if(!strcmp(fieldname,"pal_targetid"))
	{
		return CEnumCore::TagName::PAL_TARGETID;
	}
	if(!strcmp(fieldname,"pal_targetname"))
	{
		return CEnumCore::TagName::PAL_TARGETNAME;
	}
	if(!strcmp(fieldname,"pal_time"))
	{
		return CEnumCore::TagName::PAL_TIME;
	}
	if(!strcmp(fieldname,"pal_inheritid0_name"))
	{
		return CEnumCore::TagName::PAL_INHERITID0_NAME;
	}
	if(!strcmp(fieldname,"pal_inheritid1_name"))
	{
		return CEnumCore::TagName::PAL_INHERITID1_NAME;
	}
	if(!strcmp(fieldname,"pal_inheritid2_name"))
	{
		return CEnumCore::TagName::PAL_INHERITID2_NAME;
	}
	if(!strcmp(fieldname,"pal_inheritid3_name"))
	{
		return CEnumCore::TagName::PAL_INHERITID3_NAME;
	}
	if(!strcmp(fieldname,"pal_inheritid4_name"))
	{
		return CEnumCore::TagName::PAL_INHERITID4_NAME;
	}
	if(!strcmp(fieldname,"pal_inheritid5_name"))
	{
		return CEnumCore::TagName::PAL_INHERITID5_NAME;
	}
	if(!strcmp(fieldname,"pal_inheritid6_name"))
	{
		return CEnumCore::TagName::PAL_INHERITID6_NAME;
	}
	if(!strcmp(fieldname,"pal_inheritid7_name"))
	{
		return CEnumCore::TagName::PAL_INHERITID7_NAME;
	}
	if(!strcmp(fieldname,"pal_inheritid8_name"))
	{
		return CEnumCore::TagName::PAL_INHERITID8_NAME;
	}
	if(!strcmp(fieldname,"pal_inheritid9_name"))
	{
		return CEnumCore::TagName::PAL_INHERITID9_NAME;
	}
	if(!strcmp(fieldname,"pal_itemflag"))
	{
		return CEnumCore::TagName::PAL_ITEMFLAG;
	}
	if(!strcmp(fieldname,"pal_bossid"))
	{
		return CEnumCore::TagName::PAL_BOSSID;
	}
	if(!strcmp(fieldname,"pal_bossname"))
	{
		return CEnumCore::TagName::PAL_BOSSNAME;
	}

	if(!strcmp(fieldname,"pal_awardmoney"))
	{
		return CEnumCore::TagName::PAL_AWARDMONEY;
	}
	if(!strcmp(fieldname,"pal_awardexp"))
	{
		return CEnumCore::TagName::PAL_AWARDEXP;
	}
	if(!strcmp(fieldname,"pal_awardtitleid"))
	{
		return CEnumCore::TagName::PAL_AWARDTITLEID;
	}
	if(!strcmp(fieldname,"pal_awardtitlename"))
	{
		return CEnumCore::TagName::PAL_AWARDTITLENAME;
	}
	if(!strcmp(fieldname,"pal_fameid1"))
	{
		return CEnumCore::TagName::PAL_FAMEID1;
	}
	if(!strcmp(fieldname,"pal_famename1"))
	{
		return CEnumCore::TagName::PAL_FAMENAME1;
	}
	if(!strcmp(fieldname,"pal_famevalue1"))
	{
		return CEnumCore::TagName::PAL_FAMEVALUE1;
	}
	if(!strcmp(fieldname,"pal_fameid2"))
	{
		return CEnumCore::TagName::PAL_FAMEID2;
	}
	if(!strcmp(fieldname,"pal_famename2"))
	{
		return CEnumCore::TagName::PAL_FAMENAME2;
	}
	if(!strcmp(fieldname,"pal_famevalue2"))
	{
		return CEnumCore::TagName::PAL_FAMEVALUE2;
	}
	if(!strcmp(fieldname,"pal_itemid"))
	{
		return CEnumCore::TagName::PAL_ITEMID;
	}

	if(!strcmp(fieldname,"pal_skillid"))
	{
		return CEnumCore::TagName::PAL_SKILLID;
	}
	if(!strcmp(fieldname,"pal_magicid"))
	{
		return CEnumCore::TagName::PAL_MAGICID;
	}
	if(!strcmp(fieldname,"pal_magicname"))
	{
		return CEnumCore::TagName::PAL_MAGICNAME;
	}
	if(!strcmp(fieldname,"pal_petmixcountremain"))
	{
		return CEnumCore::TagName::PAL_PETMIXCOUNTREMAIN;
	}
	if(!strcmp(fieldname,"pal_petnote"))
	{
		return CEnumCore::TagName::PAL_PETNOTE;
	}
	if(!strcmp(fieldname,"pal_bantime"))
	{
		return CEnumCore::TagName::PAL_BANTIME;
	}
	if(!strcmp(fieldname,"pal_itembigtype"))
	{
		return CEnumCore::TagName::PAL_ITEMBIGTYPE;
	}
	if(!strcmp(fieldname,"pal_itembigtypeid"))
	{
		return CEnumCore::TagName::PAL_ITEMBIGTYPEID;
	}
	if(!strcmp(fieldname,"pal_point"))
	{
		return CEnumCore::TagName::PAL_POINT;
	}
	if(!strcmp(fieldname,"pal_itembuycost"))
	{
		return CEnumCore::TagName::PAL_ITEMBUYCOST;
	}
	if(!strcmp(fieldname,"pal_expiredflag"))
	{
		return CEnumCore::TagName::PAL_EXPIREDFLAG;
	}
	if(!strcmp(fieldname,"pal_expireddate"))
	{
		return CEnumCore::TagName::PAL_EXPIREDDATE;
	}
	if(!strcmp(fieldname,"pal_sellstatus"))
	{
		return CEnumCore::TagName::PAL_SELLSTATUS;
	}
	if(!strcmp(fieldname,"pal_timetype"))
	{
		return CEnumCore::TagName::PAL_TIMETYPE;
	}
	if(!strcmp(fieldname,"pal_itemsubtype"))
	{
		return CEnumCore::TagName::PAL_ITEMSUBTYPE;
	}
	if(!strcmp(fieldname,"pal_itemvoc"))
	{
		return CEnumCore::TagName::PAL_ITEMVOC;
	}
	if(!strcmp(fieldname,"pal_itemeqpos"))
	{
		return CEnumCore::TagName::PAL_ITEMEQPOS;
	}
	if(!strcmp(fieldname,"pal_itemlevel"))
	{
		return CEnumCore::TagName::PAL_ITEMLEVEL;
	}
	if(!strcmp(fieldname,"pal_guildid"))
	{
		return CEnumCore::TagName::PAL_GUILDID;
	}
	if(!strcmp(fieldname,"pal_money"))
	{
		return CEnumCore::TagName::PAL_MONEY;
	}



	if(!strcmp(fieldname,"pal_idex"))
	{
		return CEnumCore::TagName::PAL_IDEX;
	}
	if(!strcmp(fieldname,"pal_rewardid"))
	{
		return CEnumCore::TagName::PAL_REWARDID;
	}
	if(!strcmp(fieldname,"pal_gettime"))
	{
		return CEnumCore::TagName::PAL_GETTIME;
	}
	if(!strcmp(fieldname,"pal_describer"))
	{
		return CEnumCore::TagName::PAL_DESCRIBER;
	}
	if(!strcmp(fieldname,"pal_isgot"))
	{
		return CEnumCore::TagName::PAL_ISGOT;
	}
	if(!strcmp(fieldname,"pal_groupid"))
	{
		return CEnumCore::TagName::PAL_GROUPID;
	}
	if(!strcmp(fieldname,"pal_9youid"))
	{
		return CEnumCore::TagName::PAL_9YOUID;
	}
	if(!strcmp(fieldname,"pal_wastepoint"))
	{
		return CEnumCore::TagName::PAL_WASTEPOINT;
	}
	if(!strcmp(fieldname,"pal_logtype"))
	{
		return CEnumCore::TagName::PAL_LOGTYPE;
	}
	if(!strcmp(fieldname,"pal_createtime"))
	{
		return CEnumCore::TagName::PAL_CREATETIME;
	}
	if(!strcmp(fieldname,"pal_usermemo"))
	{
		return CEnumCore::TagName::PAL_USERMEMO;
	}
	if(!strcmp(fieldname,"pal_sysmemo"))
	{
		return CEnumCore::TagName::PAL_SYSMEMO;
	}
	if(!strcmp(fieldname,"pal_servername"))
	{
		return CEnumCore::TagName::PAL_SERVERNAME;
	}
	if(!strcmp(fieldname,"pal_actorname"))
	{
		return CEnumCore::TagName::PAL_ACTORNAME;
	}
	if(!strcmp(fieldname,"pal_actorlv"))
	{
		return CEnumCore::TagName::PAL_ACTORLV;
	}


	if(!strcmp(fieldname,"pal_centsell"))
	{
		return CEnumCore::TagName::PAL_CENDSELL;
	}
	if(!strcmp(fieldname,"pal_centbuy"))
	{
		return CEnumCore::TagName::PAL_CENTBUY;
	}
	if(!strcmp(fieldname,"pal_usermailbox"))
	{
		return CEnumCore::TagName::PAL_UserMailBox;
	}
	if(!strcmp(fieldname,"pal_petname"))
	{
		return CEnumCore::TagName::PAL_PETNAME;
	}

	if(!strcmp(fieldname,"pal_gmserverip"))
	{
		return CEnumCore::TagName::PAL_GMSERVERIP;
	}
	if(!strcmp(fieldname,"pal_gmserveripname"))
	{
		return CEnumCore::TagName::PAL_GMSERVERIPNAME;
	}
	if(!strcmp(fieldname,"pal_mailid"))
	{
		return CEnumCore::TagName::PAL_MAILID;
	}

	if(!strcmp(fieldname,"pal_mapid"))
	{
		return CEnumCore::TagName::PAL_MapID;
	}
	if(!strcmp(fieldname,"pal_map"))
	{
		return CEnumCore::TagName::PAL_Map;
	}
	// add by ltx
	if(!strcmp(fieldname,"pal_customitemID"))
	{
		return CEnumCore::TagName::PAL_CustomIterID;
	}
	if(!strcmp(fieldname,"pal_customitemName"))
	{
		return CEnumCore::TagName::PAL_CustomIter;
	}

	if(!strcmp(fieldname,"GT_ServerIP"))
	{
		return CEnumCore::TagName::GT_ServerIP;
	}
	if(!strcmp(fieldname,"GT_ServerName"))
	{
		return CEnumCore::TagName::GT_ServerName;
	}
	if(!strcmp(fieldname,"GT_UserID"))
	{
		return CEnumCore::TagName::GT_UserID;
	}
	if(!strcmp(fieldname,"GT_Account"))
	{
		return CEnumCore::TagName::GT_Account;
	}
	if(!strcmp(fieldname,"GT_UserNick"))
	{
		return CEnumCore::TagName::GT_UserNick;
	}

	if(!strcmp(fieldname,"GT_StartTime"))
	{
		return CEnumCore::TagName::GT_StartTime;
	}
	if(!strcmp(fieldname,"GT_EndTime"))
	{
		return CEnumCore::TagName::GT_EndTime;
	}

	if(!strcmp(fieldname,"GT_Sex"))
	{
		return CEnumCore::TagName::GT_Sex;
	}
	if(!strcmp(fieldname,"GT_CurPosition"))
	{
		return CEnumCore::TagName::GT_CurPosition;
	}
	if(!strcmp(fieldname,"GT_GCash"))
	{
		return CEnumCore::TagName::GT_GCash;
	}
	if(!strcmp(fieldname,"GT_MCash"))
	{
		return CEnumCore::TagName::GT_MCash;
	}
	if(!strcmp(fieldname,"GT_TotalPoint"))
	{
		return CEnumCore::TagName::GT_TotalPoint;
	}
	if(!strcmp(fieldname,"GT_IsOnline"))
	{
		return CEnumCore::TagName::GT_IsOnline;
	}

	if(!strcmp(fieldname,"GT_Physical"))
	{
		return CEnumCore::TagName::GT_Physical;
	}
	if(!strcmp(fieldname,"GT_Smart"))
	{
		return CEnumCore::TagName::GT_Smart;
	}
	if(!strcmp(fieldname,"GT_Financial"))
	{
		return CEnumCore::TagName::GT_Financial;
	}
	if(!strcmp(fieldname,"GT_Charm"))
	{
		return CEnumCore::TagName::GT_Charm;
	}
	if(!strcmp(fieldname,"GT_Title"))
	{
		return CEnumCore::TagName::GT_Title;
	}
	if(!strcmp(fieldname,"GT_HouseNum"))
	{
		return CEnumCore::TagName::GT_HouseNum;
	}
	if(!strcmp(fieldname,"GT_InFamily"))
	{
		return CEnumCore::TagName::GT_InFamily;
	}
	if(!strcmp(fieldname,"GT_FriendNum"))
	{
		return CEnumCore::TagName::GT_FriendNum;
	}
	if(!strcmp(fieldname,"GT_HaveInamorato"))
	{
		return CEnumCore::TagName::GT_HaveInamorato;
	}
	if(!strcmp(fieldname,"GT_IsMarried"))
	{
		return CEnumCore::TagName::GT_IsMarried;
	}

	if(!strcmp(fieldname,"GT_IsShaikh"))
	{
		return CEnumCore::TagName::GT_IsShaikh;
	}
	if(!strcmp(fieldname,"GT_InamoratoNick"))
	{
		return CEnumCore::TagName::GT_InamoratoNick;
	}
	if(!strcmp(fieldname,"GT_LoverNick"))
	{
		return CEnumCore::TagName::GT_LoverNick;
	}

	if(!strcmp(fieldname,"GT_Interval"))
	{
		return CEnumCore::TagName::GT_Interval;
	}
	if(!strcmp(fieldname,"GT_IsImmediate"))
	{
		return CEnumCore::TagName::GT_IsImmediate;
	}
	if(!strcmp(fieldname,"GT_BoardMessage"))
	{
		return CEnumCore::TagName::GT_BoardMessage;
	}

	if(!strcmp(fieldname,"GT_BanReason"))
	{
		return CEnumCore::TagName::GT_BanReason;
	}
	if(!strcmp(fieldname,"GT_UnbindReason"))
	{
		return CEnumCore::TagName::GT_UnbindReason;
	}

	if(!strcmp(fieldname,"GT_NoticeID"))
	{
		return CEnumCore::TagName::GT_NoticeID;
	}
	if(!strcmp(fieldname,"GT_UserState"))
	{
		return CEnumCore::TagName::GT_UserState;
	}
	if(!strcmp(fieldname,"GT_FamilyID"))
	{
		return CEnumCore::TagName::GT_FamilyID;
	}

	if(!strcmp(fieldname,"GT_Type"))
	{
		return CEnumCore::TagName::GT_Type;
	}
	if(!strcmp(fieldname,"GT_ZoneID"))
	{
		return CEnumCore::TagName::GT_ZoneID;
	}
	if(!strcmp(fieldname,"GT_MID"))
	{
		return CEnumCore::TagName::GT_MID;
	}
	if(!strcmp(fieldname,"GT_TimesLimit"))
	{
		return CEnumCore::TagName::GT_TimesLimit;
	}
	if(!strcmp(fieldname,"GT_DaysLimit"))
	{
		return CEnumCore::TagName::GT_DaysLimit;
	}

	if(!strcmp(fieldname,"GT_RoomName"))
	{
		return CEnumCore::TagName::GT_RoomName;
	}
	if(!strcmp(fieldname,"GT_RoomPos"))
	{
		return CEnumCore::TagName::GT_RoomPos;
	}
	if(!strcmp(fieldname,"GT_decorationName"))
	{
		return CEnumCore::TagName::GT_decorationName;
	}
	if(!strcmp(fieldname,"GT_RoomLimit"))
	{
		return CEnumCore::TagName::GT_RoomLimit;
	}

	if(!strcmp(fieldname,"GT_FamilyName"))
	{
		return CEnumCore::TagName::GT_FamilyName;
	}
	if(!strcmp(fieldname,"GT_FamilyCreate"))
	{
		return CEnumCore::TagName::GT_FamilyCreate;
	}
	if(!strcmp(fieldname,"GT_MemberNum"))
	{
		return CEnumCore::TagName::GT_MemberNum;
	}
	if(!strcmp(fieldname,"GT_FamilyLeader"))
	{
		return CEnumCore::TagName::GT_FamilyLeader;
	}
	if(!strcmp(fieldname,"GT_FamilyLeaderID"))
	{
		return CEnumCore::TagName::GT_FamilyLeaderID;
	}
	if(!strcmp(fieldname,"GT_MemberName"))
	{
		return CEnumCore::TagName::GT_MemberName;
	}
	if(!strcmp(fieldname,"GT_JoinTime"))
	{
		return CEnumCore::TagName::GT_JoinTime;
	}

	if(!strcmp(fieldname,"GT_PetName"))
	{
		return CEnumCore::TagName::GT_PetName;
	}
	if(!strcmp(fieldname,"GT_PetPos"))
	{
		return CEnumCore::TagName::GT_PetPos;
	}
	if(!strcmp(fieldname,"GT_PetLimit"))
	{
		return CEnumCore::TagName::GT_PetLimit;
	}

	if(!strcmp(fieldname,"GT_ItemID"))
	{
		return CEnumCore::TagName::GT_ItemID;
	}
	if(!strcmp(fieldname,"GT_ItemName"))
	{
		return CEnumCore::TagName::GT_ItemName;
	}
	if(!strcmp(fieldname,"GT_IsBlueIdent"))
	{
		return CEnumCore::TagName::GT_IsBlueIdent;
	}
	if(!strcmp(fieldname,"GT_Limit"))
	{
		return CEnumCore::TagName::GT_Limit;
	}

	if(!strcmp(fieldname,"GT_FriendName"))
	{
		return CEnumCore::TagName::GT_FriendName;
	}
	if(!strcmp(fieldname,"GT_BlackFName"))
	{
		return CEnumCore::TagName::GT_BlackFName;
	}
	if(!strcmp(fieldname,"GT_DarkFName"))
	{
		return CEnumCore::TagName::GT_DarkFName;
	}

	if(!strcmp(fieldname,"GT_OpenTime"))
	{
		return CEnumCore::TagName::GT_OpenTime;
	}
	if(!strcmp(fieldname,"GT_GetReward"))
	{
		return CEnumCore::TagName::GT_GetReward;
	}
	if(!strcmp(fieldname,"GT_FamilyDuty"))
	{
		return CEnumCore::TagName::GT_FamilyDuty;
	}
	if(!strcmp(fieldname,"GT_BanTime"))
	{
		return CEnumCore::TagName::GT_BanTime;
	}

	if(!strcmp(fieldname,"GT_SenderNick"))
	{
		return CEnumCore::TagName::GT_SenderNick;
	}
	if(!strcmp(fieldname,"GT_ReceiverNick"))
	{
		return CEnumCore::TagName::GT_ReceiverNick;
	}
	if(!strcmp(fieldname,"GT_TradeTime"))
	{
		return CEnumCore::TagName::GT_TradeTime;
	}
	if(!strcmp(fieldname,"GT_TradeItemID"))
	{
		return CEnumCore::TagName::GT_TradeItemID;
	}
	if(!strcmp(fieldname,"GT_TradeItemName"))
	{
		return CEnumCore::TagName::GT_TradeItemName;
	}
	if(!strcmp(fieldname,"GT_TradeItemNum"))
	{
		return CEnumCore::TagName::GT_TradeItemNum;
	}

	if(!strcmp(fieldname,"GT_MailTime"))
	{
		return CEnumCore::TagName::GT_MailTime;
	}
	if(!strcmp(fieldname,"GT_MailContent"))
	{
		return CEnumCore::TagName::GT_MailContent;
	}

	if(!strcmp(fieldname,"GT_TaskName"))
	{
		return CEnumCore::TagName::GT_TaskName;
	}
	if(!strcmp(fieldname,"GT_TaskTime"))
	{
		return CEnumCore::TagName::GT_TaskTime;
	}
	if(!strcmp(fieldname,"GT_TaskReward"))
	{
		return CEnumCore::TagName::GT_TaskReward;
	}

	if(!strcmp(fieldname,"GT_UseItemName"))
	{
		return CEnumCore::TagName::GT_UseItemName;
	}
	if(!strcmp(fieldname,"GT_UseItemNum"))
	{
		return CEnumCore::TagName::GT_UseItemNum;
	}
	if(!strcmp(fieldname,"GT_UseItemTime"))
	{
		return CEnumCore::TagName::GT_UseItemTime;
	}

	if(!strcmp(fieldname,"GT_MailTitle"))
	{
		return CEnumCore::TagName::GT_MailTitle;
	}

	if(!strcmp(fieldname,"GT_OnlineTime"))
	{
		return CEnumCore::TagName::GT_OnlineTime;
	}
	if(!strcmp(fieldname,"GT_RewardItemName"))
	{
		return CEnumCore::TagName::GT_RewardItemName;
	}
	if(!strcmp(fieldname,"GT_RewardGetTime"))
	{
		return CEnumCore::TagName::GT_RewardGetTime;
	}

	if(!strcmp(fieldname,"GT_SenderAccount"))
	{
		return CEnumCore::TagName::GT_SenderAccount;
	}
	if(!strcmp(fieldname,"GT_SendTime"))
	{
		return CEnumCore::TagName::GT_SendTime;
	}
	if(!strcmp(fieldname,"GT_SendContent"))
	{
		return CEnumCore::TagName::GT_SendContent;
	}

	if(!strcmp(fieldname,"GT_LoginTime"))
	{
		return CEnumCore::TagName::GT_LoginTime;
	}
	if(!strcmp(fieldname,"GT_LogoutTime"))
	{
		return CEnumCore::TagName::GT_LogoutTime;
	}
	if(!strcmp(fieldname,"GT_UserIP"))
	{
		return CEnumCore::TagName::GT_UserIP;
	}

	if(!strcmp(fieldname,"GT_SendNick"))
	{
		return CEnumCore::TagName::GT_SendNick;
	}
	if(!strcmp(fieldname,"GT_TreasureBox_Type"))
	{
		return CEnumCore::TagName::GT_TreasureBox_Type;
	}

	if(!strcmp(fieldname,"GT_TaskReward2"))
	{
		return CEnumCore::TagName::GT_TaskReward2;
	}
	if(!strcmp(fieldname,"GT_ChooseReward"))
	{
		return CEnumCore::TagName::GT_ChooseReward;
	}

	if(!strcmp(fieldname,"GT_BuyTime"))
	{
		return CEnumCore::TagName::GT_BuyTime;
	}
	if(!strcmp(fieldname,"GT_TimeLimit"))
	{
		return CEnumCore::TagName::GT_TimeLimit;
	}
	if(!strcmp(fieldname,"GT_ChargeTime"))
	{
		return CEnumCore::TagName::GT_ChargeTime;
	}
	if(!strcmp(fieldname,"GT_ChargeMoney"))
	{
		return CEnumCore::TagName::GT_ChargeMoney;
	}
	if(!strcmp(fieldname,"GT_Money"))
	{
		return CEnumCore::TagName::GT_Money;
	}
	if(!strcmp(fieldname,"GT_Resume"))
	{
		return CEnumCore::TagName::GT_Resume;
	}
	if(!strcmp(fieldname,"GT_Card"))
	{
		return CEnumCore::TagName::GT_Card;
	}
	if(!strcmp(fieldname,"GT_Male"))
	{
		return CEnumCore::TagName::GT_Male;
	}
	if(!strcmp(fieldname,"GT_Female"))
	{
		return CEnumCore::TagName::GT_Female;
	}
	if(!strcmp(fieldname,"GT_MarryPeople"))
	{
		return CEnumCore::TagName::GT_MarryPeople;
	}
	if(!strcmp(fieldname,"GT_Pktimes"))
	{
		return CEnumCore::TagName::GT_Pktimes;
	}
	if(!strcmp(fieldname,"GT_Win"))
	{
		return CEnumCore::TagName::GT_Win;
	}
	if(!strcmp(fieldname,"GT_Cowin"))
	{
		return CEnumCore::TagName::GT_Cowin;
	}
	if(!strcmp(fieldname,"GT_Times"))
	{
		return CEnumCore::TagName::GT_Times;
	}
	if(!strcmp(fieldname,"GT_Lustful"))
	{
		return CEnumCore::TagName::GT_Lustful;//花痴度
	}
	if(!strcmp(fieldname,"GT_Fanta"))
	{
		return CEnumCore::TagName::GT_Fanta;//迷人度
	}
	if(!strcmp(fieldname,"GT_Allexp"))
	{
		return CEnumCore::TagName::GT_Allexp;//综合指数
	}
	if(!strcmp(fieldname,"GT_SlaveCount"))
	{
		return CEnumCore::TagName::GT_SlaveCount;//奴隶数
	}
	if(!strcmp(fieldname,"GT_SlaveSum"))
	{
		return CEnumCore::TagName::GT_SlaveSum;//总资产
	}
	if(!strcmp(fieldname,"GT_Coin"))
	{
		return CEnumCore::TagName::GT_Coin;//现金
	}
	if(!strcmp(fieldname,"GT_SlaveID"))
	{
		return CEnumCore::TagName::GT_SlaveID;//奴隶ID
	}
	if(!strcmp(fieldname,"GT_SlaveAccount"))
	{
		return CEnumCore::TagName::GT_SlaveAccount;//奴隶名称
	}
	if(!strcmp(fieldname,"GT_SlaveNick"))
	{
		return CEnumCore::TagName::GT_SlaveNick;//奴隶昵称
	}
	if(!strcmp(fieldname,"GT_Price"))
	{
		return CEnumCore::TagName::GT_Price;//身价
	}
	if(!strcmp(fieldname,"GT_IsSell"))
	{
		return CEnumCore::TagName::GT_IsSell;//是否被买卖
	}
	if(!strcmp(fieldname,"GT_DateRecord"))
	{
		return CEnumCore::TagName::GT_DateRecord;//发生时间
	}
	if(!strcmp(fieldname,"GT_Discount"))
	{
		return CEnumCore::TagName::GT_Discount;//打折数
	}
	if(!strcmp(fieldname,"GT_JW2Name"))
	{
		return CEnumCore::TagName::GT_JW2Name;//JW2对应大区名
	}
	if(!strcmp(fieldname,"GT_ID"))
	{
		return CEnumCore::TagName::GT_ID;
	}
	if(!strcmp(fieldname,"jw2_account"))
	{
		return CEnumCore::TagName::JW2_ACCOUNT;
	}
	if(!strcmp(fieldname,"power"))
	{
		return CEnumCore::TagName::JW2_POWER;
	}
	if(!strcmp(fieldname,"jw2_exp"))
	{
		return CEnumCore::TagName::JW2_Exp;
	}
	if(!strcmp(fieldname,"jw2_money"))
	{
		return CEnumCore::TagName::JW2_Money;
	}
	if(!strcmp(fieldname,"jw2_cash"))
	{
		return CEnumCore::TagName::JW2_Cash;
	}
	if(!strcmp(fieldname,"goldmedal"))
	{
		return CEnumCore::TagName::JW2_GoldMedal;
	}
	if(!strcmp(fieldname,"silvermedal"))
	{
		return CEnumCore::TagName::JW2_SilverMedal;
	}
	if(!strcmp(fieldname,"coppermedal"))
	{
		return CEnumCore::TagName::JW2_CopperMedal;
	}
	if(!strcmp(fieldname,"jw2_level"))
	{
		return CEnumCore::TagName::JW2_Level;
	}
	if(!strcmp(fieldname,"region"))
	{
		return CEnumCore::TagName::JW2_Region;
	}
		if(!strcmp(fieldname,"qq"))
	{
		return CEnumCore::TagName::JW2_QQ;
	}
	if(!strcmp(fieldname,"para"))
	{
		return CEnumCore::TagName::JW2_PARA;
	}
			if(!strcmp(fieldname,"jw2_usernick"))
	{
		return CEnumCore::TagName::JW2_UserNick;
	}
	if(!strcmp(fieldname,"jw2_userid"))
	{
		return CEnumCore::TagName::JW2_UserID;
	}
	if(!strcmp(fieldname,"jw2_sex"))
	{
		return CEnumCore::TagName::JW2_Sex;
	}
	if(!strcmp(fieldname,"jw2_avataritem"))
	{
		return CEnumCore::TagName::JW2_AvatarItem;
	}
	if(!strcmp(fieldname,"jw2_avataritemname"))
	{
		return CEnumCore::TagName::JW2_AvatarItemName;
	}
	if(!strcmp(fieldname,"jw2_expiredate"))
	{
		return CEnumCore::TagName::JW2_ExpireDate;
	}
	if(!strcmp(fieldname,"jw2_useitem"))
	{
		return CEnumCore::TagName::JW2_UseItem;
	}
	if(!strcmp(fieldname,"jw2_remaincount"))
	{
		return CEnumCore::TagName::JW2_RemainCount;
	}
	if(!strcmp(fieldname,"jw2_usersn"))
	{
		return CEnumCore::TagName::JW2_UserSN;
	}
	if(!strcmp(fieldname,"jw2_boardsn"))
	{
		return CEnumCore::TagName::JW2_BOARDSN;
	}
	if(!strcmp(fieldname,"jw2_bugletype"))
	{
		return CEnumCore::TagName::JW2_BUGLETYPE;
	}
	if(!strcmp(fieldname,"jw2_boardmessage"))
	{
		return CEnumCore::TagName::JW2_BoardMessage;
	}
	if(!strcmp(fieldname,"jw2_usetime"))
	{
		return CEnumCore::TagName::JW2_UseTime;
	}
	if(!strcmp(fieldname,"jw2_taskid"))
	{
		return CEnumCore::TagName::JW2_TaskID;
	}
	if(!strcmp(fieldname,"jw2_begintime"))
	{
		return CEnumCore::TagName::JW2_BeginTime;
	}
	if(!strcmp(fieldname,"jw2_endtime"))
	{
		return CEnumCore::TagName::JW2_EndTime;
	}
	if(!strcmp(fieldname,"jw2_interval"))
	{
		return CEnumCore::TagName::JW2_Interval;
	}
	if(!strcmp(fieldname,"jw2_status"))
	{
		return CEnumCore::TagName::JW2_Status;
	}
	if(!strcmp(fieldname,"jw2_serverip"))
	{
		return CEnumCore::TagName::JW2_ServerIP;
	}
	if(!strcmp(fieldname,"jw2_stage0"))
	{
		return CEnumCore::TagName::JW2_Stage0;
	}
	if(!strcmp(fieldname,"jw2_stage1"))
	{
		return CEnumCore::TagName::JW2_Stage1;
	}
	if(!strcmp(fieldname,"jw2_stage2"))
	{
		return CEnumCore::TagName::JW2_Stage2;
	}
	if(!strcmp(fieldname,"jw2_stage3"))
	{
		return CEnumCore::TagName::JW2_Stage3;
	}
	if(!strcmp(fieldname,"jw2_stage4"))
	{
		return CEnumCore::TagName::JW2_Stage4;
	}
	if(!strcmp(fieldname,"jw2_stage5"))
	{
		return CEnumCore::TagName::JW2_Stage5;
	}
	if(!strcmp(fieldname,"jw2_stage6"))
	{
		return CEnumCore::TagName::JW2_Stage6;
	}
	if(!strcmp(fieldname,"jw2_stage7"))
	{
		return CEnumCore::TagName::JW2_Stage7;
	}
	if(!strcmp(fieldname,"jw2_stage8"))
	{
		return CEnumCore::TagName::JW2_Stage8;
	}
	if(!strcmp(fieldname,"jw2_stage9"))
	{
		return CEnumCore::TagName::JW2_Stage9;
	}
	if(!strcmp(fieldname,"jw2_stage10"))
	{
		return CEnumCore::TagName::JW2_Stage10;
	}
	if(!strcmp(fieldname,"jw2_stage11"))
	{
		return CEnumCore::TagName::JW2_Stage11;
	}
	if(!strcmp(fieldname,"jw2_stage12"))
	{
		return CEnumCore::TagName::JW2_Stage12;
	}
	if(!strcmp(fieldname,"jw2_stage13"))
	{
		return CEnumCore::TagName::JW2_Stage13;
	}
	if(!strcmp(fieldname,"jw2_stage14"))
	{
		return CEnumCore::TagName::JW2_Stage14;
	}
	if(!strcmp(fieldname,"jw2_stage15"))
	{
		return CEnumCore::TagName::JW2_Stage15;
	}
	if(!strcmp(fieldname,"jw2_stage16"))
	{
		return CEnumCore::TagName::JW2_Stage16;
	}
	if(!strcmp(fieldname,"jw2_stage17"))
	{
		return CEnumCore::TagName::JW2_Stage17;
	}
	if(!strcmp(fieldname,"jw2_stage18"))
	{
		return CEnumCore::TagName::JW2_Stage18;
	}
	if(!strcmp(fieldname,"jw2_stage19"))
	{
		return CEnumCore::TagName::JW2_Stage19;
	}
	if(!strcmp(fieldname,"jw2_chapter"))
	{
		return CEnumCore::TagName::JW2_Chapter;
	}
	if(!strcmp(fieldname,"jw2_curstage"))
	{
		return CEnumCore::TagName::JW2_CurStage;
	}
	if(!strcmp(fieldname,"jw2_maxstage"))
	{
		return CEnumCore::TagName::JW2_MaxStage;
	}
	if(!strcmp(fieldname,"jw2_buysn"))
	{
		return CEnumCore::TagName::JW2_BUYSN;
	}
	if(!strcmp(fieldname,"jw2_goodstype"))
	{
		return CEnumCore::TagName::JW2_GOODSTYPE;
	}
	if(!strcmp(fieldname,"jw2_goodsindex"))
	{
		return CEnumCore::TagName::JW2_GOODSINDEX;
	}
	if(!strcmp(fieldname,"jw2_recesn")||!strcmp(fieldname,"jw2_recvsn"))
	{
		return CEnumCore::TagName::JW2_RECESN;
	}
	if(!strcmp(fieldname,"jw2_buydate"))
	{
		return CEnumCore::TagName::JW2_BUYDATE;
	}
	if(!strcmp(fieldname,"jw2_reason"))
	{
		return CEnumCore::TagName::JW2_Reason;
	}
	if(!strcmp(fieldname,"jw2_receid"))
	{
		return CEnumCore::TagName::JW2_RECEID;
	}

	if(!strcmp(fieldname,"jw2_malesn"))//男性SN
	{
		return CEnumCore::TagName::JW2_MALESN;
	}
	if(!strcmp(fieldname,"jw2_maleusername"))//男性用户名
	{
		return CEnumCore::TagName::JW2_MALEUSERNAME;
	}
	if(!strcmp(fieldname,"jw2_maleusernick"))//男性昵称
	{
		return CEnumCore::TagName::JW2_MALEUSERNICK;
	}
	if(!strcmp(fieldname,"jw2_femalesn"))//女性SN
	{
		return CEnumCore::TagName::JW2_FEMAILESN;
	}
	if(!strcmp(fieldname,"jw2_femaleusername"))//女性用户名
	{
		return CEnumCore::TagName::JW2_FEMALEUSERNAME;
	}
	if(!strcmp(fieldname,"jw2_femaleusernick"))//女性昵称
	{
		return CEnumCore::TagName::JW2_FEMAILEUSERNICK;
	}
	if(!strcmp(fieldname,"jw2_weddingdate"))//结婚时间
	{
		return CEnumCore::TagName::JW2_WEDDINGDATE;
	}
	if(!strcmp(fieldname,"jw2_unweddingdate"))//离婚时间
	{
		return CEnumCore::TagName::JW2_UNWEDDINGDATE;
	}
	if(!strcmp(fieldname,"jw2_weddingname"))//婚礼名称
	{
		return CEnumCore::TagName::JW2_WEDDINGNAME;
	}
	if(!strcmp(fieldname,"jw2_weddingvow"))//婚礼誓言
	{
		return CEnumCore::TagName::JW2_WEDDINGVOW;
	}
	if(!strcmp(fieldname,"jw2_ringlevel"))//戒指等级
	{
		return CEnumCore::TagName::JW2_RINGLEVEL;
	}
	if(!strcmp(fieldname,"jw2_redheartnum"))//红心数量
	{
		return CEnumCore::TagName::JW2_REDHEARTNUM;
	}
	if(!strcmp(fieldname,"jw2_weddingsn")||!strcmp(fieldname,"jw2_weddingno"))//婚姻序号
	{
		return CEnumCore::TagName::JW2_WEDDINGNO;
	}
	if(!strcmp(fieldname,"jw2_confirmsn"))//见证者SN
	{
		return CEnumCore::TagName::JW2_CONFIRMSN;
	}
	if(!strcmp(fieldname,"jw2_confirmusername"))//见证者用户名
	{
		return CEnumCore::TagName::JW2_CONFIRMUSERNAME;
	}
	if(!strcmp(fieldname,"jw2_confirmusernick"))////见证者昵称
	{
		return CEnumCore::TagName::JW2_CONFIRMUSERNICK;
	}
	if(!strcmp(fieldname,"jw2_coupledate"))//情侣日期
	{
		return CEnumCore::TagName::JW2_COUPLEDATE;
	}
	if(!strcmp(fieldname,"jw2_kissnum"))//kiss次数
	{
		return CEnumCore::TagName::JW2_KISSNUM;
	}
	if(!strcmp(fieldname,"jw2_lastkissdate"))//最后一次Kiss时间
	{
		return CEnumCore::TagName::JW2_LASTKISSDATE;
	}
	if(!strcmp(fieldname,"jw2_breakdate"))//分手时间
	{
		return CEnumCore::TagName::JW2_BREAKDATE;
	}
	if(!strcmp(fieldname,"jw2_cmbreakdate"))//确认分手时间
	{
		return CEnumCore::TagName::JW2_CMBREAKDATE;
	}
	if(!strcmp(fieldname,"jw2_breaksn"))//提出SN
	{
		return CEnumCore::TagName::JW2_BREAKSN;
	}
	if(!strcmp(fieldname,"jw2_breakusername"))//提出用户名
	{
		return CEnumCore::TagName::JW2_BREAKUSERNAME;
	}
	if(!strcmp(fieldname,"jw2_breakusernick"))//提出昵称
	{
		return CEnumCore::TagName::JW2_BREAKUSERNICK;
	}
	if(!strcmp(fieldname,"jw2_lastlogindate"))//最后登陆时间
	{
		return CEnumCore::TagName::JW2_LASTLOGINDATE;
	}
	if(!strcmp(fieldname,"jw2_registdate"))//激活时间
	{
		return CEnumCore::TagName::JW2_REGISTDATE;
	}
	if(!strcmp(fieldname,"jw2_fcmstatus"))//防沉迷状态
	{
		return CEnumCore::TagName::JW2_FCMSTATUS;
	}
	if(!strcmp(fieldname,"jw2_familyid"))//家族ID
	{
		return CEnumCore::TagName::JW2_FAMILYID;
	}
	if(!strcmp(fieldname,"jw2_familyname"))//家族名称
	{
		return CEnumCore::TagName::JW2_FAMILYNAME;
	}
	if(!strcmp(fieldname,"jw2_dutyid"))//职业号
	{
		return CEnumCore::TagName::JW2_DUTYID;
	}
	if(!strcmp(fieldname,"jw2_dutyname"))//职业名
	{
		return CEnumCore::TagName::JW2_DUTYNAME;
	}
	if(!strcmp(fieldname,"jw2_attendtime"))//加入时间
	{
		return CEnumCore::TagName::JW2_ATTENDTIME;
	}
	if(!strcmp(fieldname,"jw2_couplesn"))//加入时间
	{
		return CEnumCore::TagName::JW2_COUPLESN;
	}
	if(!strcmp(fieldname,"jw2_createtime"))//创建时间
	{
		return CEnumCore::TagName::JW2_CREATETIME;
	}
	if(!strcmp(fieldname,"jw2_cnt"))//人数
	{
		return CEnumCore::TagName::JW2_CNT;
	}
	if(!strcmp(fieldname,"jw2_point"))//积分
	{
		return CEnumCore::TagName::JW2_POINT;
	}
	if(!strcmp(fieldname,"jw2_logintype"))//类型，0登入，1登出
	{
		return CEnumCore::TagName::JW2_LOGINTYPE;
	}
	if(!strcmp(fieldname,"jw2_time"))//时间
	{
		return CEnumCore::TagName::JW2_TIME;
	}
	if(!strcmp(fieldname,"jw2_ip"))//IP地址
	{
		return CEnumCore::TagName::JW2_IP;
	}
	
	if(!strcmp(fieldname,"jw2_itempos"))//物品位置
	{
		return CEnumCore::TagName::JW2_ItemPos;
	}
	if(!strcmp(fieldname,"jw2_itemno"))//物品序号
	{
		return CEnumCore::TagName::JW2_ItemNo;
	}
	if(!strcmp(fieldname,"jw2_mailtitle"))//邮件主题
	{
		return CEnumCore::TagName::JW2_MailTitle;
	}
	
	if(!strcmp(fieldname,"jw2_mailcontent"))//邮件内容
	{
		return CEnumCore::TagName::JW2_MailContent;
	}
	if(!strcmp(fieldname,"jw2_repute"))//声望
	{
		return CEnumCore::TagName::JW2_Repute;
	}
	if(!strcmp(fieldname,"jw2_nowtitle"))//称号
	{
		return CEnumCore::TagName::JW2_NowTitle;
	}

	if(!strcmp(fieldname,"jw2_familylevel"))//等级
	{
		return CEnumCore::TagName::JW2_FamilyLevel;
	}
	if(!strcmp(fieldname,"jw2_attendrank"))//人气排名
	{
		return CEnumCore::TagName::JW2_AttendRank;
	}
	if(!strcmp(fieldname,"jw2_moneyrank"))//财富排名
	{
		return CEnumCore::TagName::JW2_MoneyRank;
	}
	if(!strcmp(fieldname,"jw2_powerrank"))//实力排名
	{
		return CEnumCore::TagName::JW2_PowerRank;
	}
	
	if(!strcmp(fieldname,"jw2_itemcode"))//道具ID
	{
		return CEnumCore::TagName::JW2_ItemCode;
	}
	if(!strcmp(fieldname,"jw2_itemname"))//道具名称
	{
		return CEnumCore::TagName::JW2_ItemName;
	}
	if(!strcmp(fieldname,"jw2_productid"))//商品ID
	{
		return CEnumCore::TagName::JW2_Productid;
	}
	if(!strcmp(fieldname,"jw2_productname"))//商品名称
	{
		return CEnumCore::TagName::JW2_ProductName;
	}
	if(!strcmp(fieldname,"jw2_familypoint"))//家族点数
	{
		return CEnumCore::TagName::JW2_FamilyPoint;
	}
	if(!strcmp(fieldname,"jw2_petsn"))//宠物ID
	{
		return CEnumCore::TagName::JW2_PetSn;
	}
	if(!strcmp(fieldname,"jw2_petname"))//宠物名称
	{
		return CEnumCore::TagName::JW2_PetName;
	}
	if(!strcmp(fieldname,"jw2_petnick"))//宠物自定义名称
	{
		return CEnumCore::TagName::JW2_PetNick;
	}
	if(!strcmp(fieldname,"jw2_petlevel"))//宠物等级
	{
		return CEnumCore::TagName::JW2_PetLevel;
	}
	if(!strcmp(fieldname,"jw2_petexp"))//宠物经验
	{
		return CEnumCore::TagName::JW2_PetExp;
	}
	if(!strcmp(fieldname,"jw2_petevol"))//进阶阶段
	{
		return CEnumCore::TagName::JW2_PetEvol;
	}
	if(!strcmp(fieldname,"jw2_mailsn"))//序列号
	{
		return CEnumCore::TagName::JW2_MailSn;
	}
	if(!strcmp(fieldname,"jw2_sendnick"))//发件人昵称
	{
		return CEnumCore::TagName::JW2_SendNick;
	}
	if(!strcmp(fieldname,"jw2_senddate"))//发送日期
	{
		return CEnumCore::TagName::JW2_SendDate;
	}
	if(!strcmp(fieldname,"jw2_num"))//数量
	{
		return CEnumCore::TagName::JW2_Num;
	}
	if(!strcmp(fieldname,"jw2_rate"))//男女比例
	{
		return CEnumCore::TagName::JW2_Rate;
	}
	if(!strcmp(fieldname,"jw2_shaikhnick"))//族长名称
	{
		return CEnumCore::TagName::JW2_ShaikhNick;
	}
	if(!strcmp(fieldname,"jw2_subfamilyname"))//隶属家族名称
	{
		return CEnumCore::TagName::JW2_SubFamilyName;
	}
	if(!strcmp(fieldname,"jw2_moneytype"))//消费类型
	{
		return CEnumCore::TagName::JW2_MoneyType;
	}
	if(!strcmp(fieldname,"jw2_beforecash"))//消费前钱
	{
		return CEnumCore::TagName::JW2_BeforeCash;
	}
	if(!strcmp(fieldname,"jw2_aftercash"))//消费后钱
	{
		return CEnumCore::TagName::JW2_AfterCash;
	}
	if(!strcmp(fieldname,"Magic_GetTime")||!strcmp(fieldname,"BUYTIME"))
	{
		return CEnumCore::TagName::CG2_GetTime;
	}
	if(!strcmp(fieldname,"Magic_PetSrcName"))
	{
		return CEnumCore::TagName::CG2_PetSrcName;
	}
	if(!strcmp(fieldname,"Magic_Wind"))
	{
		return CEnumCore::TagName::CG2_Wind;
	}
	if(!strcmp(fieldname,"Magic_Soil"))
	{
		return CEnumCore::TagName::CG2_Soil;
	}
	if(!strcmp(fieldname,"Magic_Water"))
	{
		return CEnumCore::TagName::CG2_Water;
	}
	if(!strcmp(fieldname,"Magic_Frie"))
	{
		return CEnumCore::TagName::CG2_Frie;
	}
	if(!strcmp(fieldname,"Magic_petID"))
	{
		return CEnumCore::TagName::CG2_PetID;
	}
	if(!strcmp(fieldname,"Magic_userItemName"))
	{
		return CEnumCore::TagName::CG2_userItemName;
	}
	if(!strcmp(fieldname,"Mggic_SkillType")||!strcmp(fieldname,"skill_type"))
	{
		return CEnumCore::TagName::CG2_SkillType;
	}
	if(!strcmp(fieldname,"Magic_CreateTime"))
	{
		return CEnumCore::TagName::CG2_CreateTime;
	}
	if(!strcmp(fieldname,"Magic_IP"))
	{
		return CEnumCore::TagName::CG2_Ip;
	}
	if(!strcmp(fieldname,"Magic_Online"))
	{
		return CEnumCore::TagName::CG2_IsOnline;
	}
	if(!strcmp(fieldname,"content"))
	{
		return CEnumCore::TagName::CG2_Content;
	}
	if(!strcmp(fieldname,"guildId"))
	{
		return CEnumCore::TagName::CG2_GuildID;
	}
	if(!strcmp(fieldname,"guildName"))
	{
		return CEnumCore::TagName::CG2_GuildName;
	}
	if(!strcmp(fieldname,"honor"))
	{
		return CEnumCore::TagName::CG2_GuildHoro;
	}
	if(!strcmp(fieldname,"memberCnt"))
	{
		return CEnumCore::TagName::CG2_GuildNum;
	}
	if(!strcmp(fieldname,"masterName"))
	{
		return CEnumCore::TagName::CG2_Master;
	}
	if(!strcmp(fieldname,"masterIdkey"))
	{
		return CEnumCore::TagName::CG2_MasterIdkey;
	}
	if(!strcmp(fieldname,"rank"))
	{
		return CEnumCore::TagName::CG2_GuildRank;
	}
	if(!strcmp(fieldname,"iuReservet02"))
	{
		return CEnumCore::TagName::CG2_PetFullLvl;
	}
	if(!strcmp(fieldname,"max_lp") || !strcmp(fieldname,"max_pet_lp"))
	{
		return CEnumCore::TagName::CG2_PetMaxBlood;
	}
	if(!strcmp(fieldname,"max_fp") || !strcmp(fieldname,"max_pet_fp"))
	{
		return CEnumCore::TagName::CG2_PetMaxMag;
	}
	if(!strcmp(fieldname,"reserve01"))
	{
		return CEnumCore::TagName::CG2_ComposeTimes;
	}
	if(!strcmp(fieldname,"reserve02"))
	{
		return CEnumCore::TagName::CG2_ComposeItemID;
	}
	if(!strcmp(fieldname,"magic_Fnick"))
	{
		return CEnumCore::TagName::CG2_FriendCharNum;
	}
	if(!strcmp(fieldname,"friendIDkey"))
	{
		return CEnumCore::TagName::CG2_FriendIDKey;
	}
	if(!strcmp(fieldname,"note_id") || !strcmp(fieldname,"gm_id"))
	{
		return CEnumCore::TagName::CG2_NoticeID;
	}
	if(!strcmp(fieldname,"note_Content"))
	{
		return CEnumCore::TagName::CG2_Content;
	}
	if(!strcmp(fieldname,"datetime"))
	{
		return CEnumCore::TagName::BeginTime;
	}
	if(!strcmp(fieldname,"endtime"))
	{
		return CEnumCore::TagName::EndTime;
	}
	if(!strcmp(fieldname,"paramBase01") || !strcmp(fieldname,"Magic_Physical"))
	{
		return CEnumCore::TagName::CG2_Physical;
	}
	if(!strcmp(fieldname,"paramBase02"))
	{
		return CEnumCore::TagName::CG2_Power;
	}
	if(!strcmp(fieldname,"paramBase03"))
	{
		return CEnumCore::TagName::CG2_Smart;
	}
	if(!strcmp(fieldname,"paramBase04"))
	{
		return CEnumCore::TagName::CG2_Speed;
	}
	if(!strcmp(fieldname,"paramBase05"))
	{
		return CEnumCore::TagName::CG2_Spirit;
	}
	if(!strcmp(fieldname,"Magic_Times"))
	{
		return CEnumCore::TagName::CG2_Times;
	}
	if(!strcmp(fieldname,"map_name"))
	{
		return CEnumCore::TagName::CG2_MapName;
	}
	if(!strcmp(fieldname,"magic_mapid"))
	{
		return CEnumCore::TagName::CG2_MapID;
	}
	if(!strcmp(fieldname,"Magic_TypeId"))
	{
		return CEnumCore::TagName::CG2_TypeID;
	}
	if(!strcmp(fieldname,"Magic_TypeName")|| !strcmp(fieldname,"type"))
	{
		return CEnumCore::TagName::CG2_TypeName;
	}
	if(!strcmp(fieldname,"charisma"))
	{
		return CEnumCore::TagName::CG2_Charm;
	}
	if(!strcmp(fieldname,"prison"))
	{
		return CEnumCore::TagName::CG2_InPrison;
	}
	if(!strcmp(fieldname,"baseParam00"))
	{
		return CEnumCore::TagName::CG2_LeftPoint;
	}
	if(!strcmp(fieldname,"baseParam01"))
	{
		return CEnumCore::TagName::CG2_Physical;
	}
	if(!strcmp(fieldname,"baseParam02"))
	{
		return CEnumCore::TagName::CG2_Power;
	}
	if(!strcmp(fieldname,"baseParam03"))
	{
		return CEnumCore::TagName::CG2_Smart;
	}
	if(!strcmp(fieldname,"baseParam04"))
	{
		return CEnumCore::TagName::CG2_Speed;
	}
	if(!strcmp(fieldname,"baseParam05"))
	{
		return CEnumCore::TagName::CG2_Spirit;
	}
	if(!strcmp(fieldname,"paramParam01"))
	{
		return CEnumCore::TagName::CG2_MaxBlood;
	}
	if(!strcmp(fieldname,"paramParam02") || !strcmp(fieldname,"Magic_MaxMag"))
	{
		return CEnumCore::TagName::CG2_MaxMag;
	}
	if(!strcmp(fieldname,"paramParam03") || !strcmp(fieldname,"Magic_Attack"))
	{
		return CEnumCore::TagName::CG2_Attack;
	}
	if(!strcmp(fieldname,"paramParam04") || !strcmp(fieldname,"Magic_Defend"))
	{
		return CEnumCore::TagName::CG2_Defend;
	}
	if(!strcmp(fieldname,"paramParam05"))
	{
		return CEnumCore::TagName::CG2_Cheesy;
	}
	if(!strcmp(fieldname,"paramParam06"))
	{
		return CEnumCore::TagName::CG2_MagAttack;
	}
	if(!strcmp(fieldname,"paramParam07"))
	{
		return CEnumCore::TagName::CG2_MagDefend;
	}
	if(!strcmp(fieldname,"paramParam08"))
	{
		return CEnumCore::TagName::CG2_Recover;
	}
	if(!strcmp(fieldname,"paramParam09"))
	{
		return CEnumCore::TagName::CG2_Hit;
	}
	if(!strcmp(fieldname,"paramParam10"))
	{
		return CEnumCore::TagName::CG2_Avoid;
	}
	if(!strcmp(fieldname,"paramParam11"))
	{
		return CEnumCore::TagName::CG2_Critical;
	}
	if(!strcmp(fieldname,"paramParam12"))
	{
		return CEnumCore::TagName::CG2_Retort;
	}
	if(!strcmp(fieldname,"Magic_PetAttack"))
	{
		return CEnumCore::TagName::CG2_PetAttack;
	}
	if(!strcmp(fieldname,"Magic_PetDefend"))
	{
		return CEnumCore::TagName::CG2_PetDefend;
	}
	if(!strcmp(fieldname,"Magic_PetCheesy"))
	{
		return CEnumCore::TagName::CG2_PetCheesy;
	}
	if(!strcmp(fieldname,"Magic_PetMagAttack"))
	{
		return CEnumCore::TagName::CG2_PetMagAttack;
	}
	if(!strcmp(fieldname,"Magic_PetMagDefend"))
	{
		return CEnumCore::TagName::CG2_PetMagDefend;
	}
	if(!strcmp(fieldname,"Magic_PetRecover"))
	{
		return CEnumCore::TagName::CG2_PetRecover;
	}
	if(!strcmp(fieldname,"Magic_PetHit"))
	{
		return CEnumCore::TagName::CG2_PetHit;
	}
	if(!strcmp(fieldname,"Magic_PetAvoid"))
	{
		return CEnumCore::TagName::CG2_PetAvoid;
	}
	if(!strcmp(fieldname,"Magic_PetCritical"))
	{
		return CEnumCore::TagName::CG2_PetCritical;
	}
	if(!strcmp(fieldname,"Magic_PetRetort"))
	{
		return CEnumCore::TagName::CG2_PetRetort;
	}
	if(!strcmp(fieldname,"charNo"))
	{
		return CEnumCore::TagName::CG2_CharNo;
	}
	if(!strcmp(fieldname,"mag_itemID"))
	{
		return CEnumCore::TagName::CG2_PetItemID;
	}
	if(!strcmp(fieldname,"mag_itemName") || !strcmp(fieldname,"magic_itemName")|| !strcmp(fieldname,"item_name"))
	{
		return CEnumCore::TagName::CG2_ItemName;
	}
	if(!strcmp(fieldname,"loginTime"))
	{
		return CEnumCore::TagName::CG2_LatestLoginTime;
	}
	if(!strcmp(fieldname,"logoutTime"))
	{
		return CEnumCore::TagName::CG2_LatestLogoutTime;
	}
	if(!strcmp(fieldname,"charCount"))
	{
		return CEnumCore::TagName::CG2_UserNum;
	}
	if(!strcmp(fieldname,"friendVal"))
	{
		return CEnumCore::TagName::CG2_FriendShip;
	}
	if(!strcmp(fieldname,"qualityRank"))
	{
		return CEnumCore::TagName::CG2_QualityLevel;
	}
	if(!strcmp(fieldname,"magic_itemID") || !strcmp(fieldname,"Magic_LogitemID"))
	{
		return CEnumCore::TagName::CG2_PetItemID;
	}
	if(!strcmp(fieldname,"str"))
	{
		return CEnumCore::TagName::CG2_Power;
	}
	if(!strcmp(fieldname,"vit"))
	{
		return CEnumCore::TagName::CG2_PetPhysical;
	}
	if(!strcmp(fieldname,"active"))
	{
		return CEnumCore::TagName::CG2_Smart;
	}
	if(!strcmp(fieldname,"pet_speed"))
	{
		return CEnumCore::TagName::CG2_Speed;
	}
	if(!strcmp(fieldname,"spirit"))
	{
		return CEnumCore::TagName::CG2_Spirit;
	}
	if(!strcmp(fieldname,"loyalty"))
	{
		return CEnumCore::TagName::CG2_PetFaith;
	}
	if(!strcmp(fieldname,"petSlot"))
	{
		return CEnumCore::TagName::CG2_PetID;
	}
	if(!strcmp(fieldname,"getTime"))
	{
		return CEnumCore::TagName::CG2_PetItemGetTime;
	}
	if(!strcmp(fieldname,"chargeCombineItemID"))
	{
		return CEnumCore::TagName::CG2_ComposeItemID;
	}
	if(!strcmp(fieldname,"itemID"))
	{
		return CEnumCore::TagName::CG2_PetItemID;
	}
	if(!strcmp(fieldname,"repairNum"))
	{
		return CEnumCore::TagName::CG2_FixTimes;
	}
	if(!strcmp(fieldname,"dur"))
	{
		return CEnumCore::TagName::CG2_CurEndure;
	}
	if(!strcmp(fieldname,"palette"))
	{
		return CEnumCore::TagName::CG2_Palette;
	}
	if(!strcmp(fieldname,"maxDur"))
	{
		return CEnumCore::TagName::CG2_MaxEndure;
	}
	if(!strcmp(fieldname,"itemSlot"))
	{
		return CEnumCore::TagName::CG2_PetItemCol;
	}
	if(!strcmp(fieldname,"pet_name"))
	{
		return CEnumCore::TagName::CG2_PetSrcName;
	}
	if(!strcmp(fieldname,"youself_name"))
	{
		return CEnumCore::TagName::CG2_PetNewName;
	}
	if(!strcmp(fieldname,"lp"))
	{
		return CEnumCore::TagName::CG2_Blood;
	}
	if(!strcmp(fieldname,"fp") || !strcmp(fieldname,"Magic_Magic"))
	{
		return CEnumCore::TagName::CG2_Magic;
	}
	if(!strcmp(fieldname,"fame"))
	{
		return CEnumCore::TagName::CG2_Fame;
	}	
	if(!strcmp(fieldname,"magic_nick") || !strcmp(fieldname,"Magic_NickName"))
	{
		return CEnumCore::TagName::CG2_nickname;
	}
	if(!strcmp(fieldname,"IDKey") || !strcmp(fieldname,"IdKey"))
	{
		return CEnumCore::TagName::CG2_UserID;
	}
	if(!strcmp(fieldname,"cdkey")||!strcmp(fieldname,"username"))
	{
		return CEnumCore::TagName::CG2_UserName;
	}
	if(!strcmp(fieldname,"lv"))
	{
		return CEnumCore::TagName::CG2_Level;
	}
	if(!strcmp(fieldname,"exp"))
	{
		return CEnumCore::TagName::CG2_Exp;
	}
	if(!strcmp(fieldname,"magic_sex"))
	{
		return CEnumCore::TagName::CG2_Sex;
	}
	if(!strcmp(fieldname,"jobID"))
	{
		return CEnumCore::TagName::CG2_JobID;
	}
	if(!strcmp(fieldname,"gold")||!strcmp(fieldname,"PRICE"))
	{
		return CEnumCore::TagName::CG2_Money;
	}
	if(!strcmp(fieldname,"masterGuildID"))
	{
		return CEnumCore::TagName::CG2_isChairman;
	}
	if(!strcmp(fieldname,"choiceGuildID"))
	{
		return CEnumCore::TagName::CG2_GuildID;
	}
	if(!strcmp(fieldname,"injury"))
	{
		return CEnumCore::TagName::CG2_Injured;
	}
	if(!strcmp(fieldname,"soulInjury"))
	{
		return CEnumCore::TagName::CG2_Status;
	}
	if(!strcmp(fieldname,"strength"))
	{
		return CEnumCore::TagName::CG2_Physical;
	}
	if(!strcmp(fieldname,"Magic_attack"))
	{
		return CEnumCore::TagName::CG2_Attack;
	}	
	if(!strcmp(fieldname,"duelPoint"))
	{
		return CEnumCore::TagName::CG2_Fight;
	}
	if(!strcmp(fieldname,"dex"))
	{
		return CEnumCore::TagName::CG2_Smart;
	}
	if(!strcmp(fieldname,"speed"))
	{
		return CEnumCore::TagName::CG2_Speed;
	}
	if(!strcmp(fieldname,"vitality"))
	{
		return CEnumCore::TagName::CG2_Spirit;
	}
	if(!strcmp(fieldname,"job_name"))
	{
		return CEnumCore::TagName::CG2_Profession;
	}
	if(!strcmp(fieldname,"job_id"))
	{
		return CEnumCore::TagName::CG2_JobID;
	}	
	if(!strcmp(fieldname,"jobRank"))
	{
		return CEnumCore::TagName::CG2_ProfessionLevel;
	}
	if(!strcmp(fieldname,"userTitle"))
	{
		return CEnumCore::TagName::CG2_OwnName;
	}
	if(!strcmp(fieldname,"mapId"))
	{
		return CEnumCore::TagName::CG2_MapID;
	}
	if(!strcmp(fieldname,"map"))
	{
		return CEnumCore::TagName::CG2_Coordinate;
	}
	if(!strcmp(fieldname,"fameTitle"))
	{
		return CEnumCore::TagName::CG2_ProHonorRank;
	}
	if(!strcmp(fieldname,"tmpTitle"))
	{
		return CEnumCore::TagName::CG2_tempRank;
	}	
	if(!strcmp(fieldname,"slotNo"))
	{
		return CEnumCore::TagName::CG2_SlotNum;
	}
	if(!strcmp(fieldname,"friendName"))
	{
		return CEnumCore::TagName::CG2_FriendCharNum;
	}
	if(!strcmp(fieldname,"stackNum"))
	{
		return CEnumCore::TagName::CG2_CombineNum;
	}
	if(!strcmp(fieldname,"f_IdKey"))
	{
		return CEnumCore::TagName::CG2_FriendIDKey;
	}
	if(!strcmp(fieldname,"skillID")||!strcmp(fieldname,"skill_name"))
	{
		return CEnumCore::TagName::CG2_SkillName;
	}
	if(!strcmp(fieldname,"Magic_SkillID"))
	{
		return CEnumCore::TagName::CG2_SkillID;
	}	
	if(!strcmp(fieldname,"skillRank"))
	{
		return CEnumCore::TagName::CG2_SkillLevel;
	}
	if(!strcmp(fieldname,"skillExp"))
	{
		return CEnumCore::TagName::CG2_SkillExp;
	}
	if(!strcmp(fieldname,"targetIDKey"))
	{
		return CEnumCore::TagName::CG2_targetIDKey;
	}
	if(!strcmp(fieldname,"targetName"))
	{
		return CEnumCore::TagName::CG2_targetName;
	}
	if(!strcmp(fieldname,"category"))
	{
		return CEnumCore::TagName::CG2_category;
	}
	if(!strcmp(fieldname,"action"))
	{
		return CEnumCore::TagName::CG2_action;
	}	
	if(!strcmp(fieldname,"mgvalue"))
	{
		return CEnumCore::TagName::CG2_mgvalue;
	}
	if(!strcmp(fieldname,"mgstring"))
	{
		return CEnumCore::TagName::CG2_mgstring;
	}
	if(!strcmp(fieldname,"logtime"))
	{
		return CEnumCore::TagName::CG2_logtime;
	}	
	if(!strcmp(fieldname,"Magic_BanTime"))
	{
		return CEnumCore::TagName::CG2_BanTime;
	}	
	if(!strcmp(fieldname,"titleFlags001"))
	{
		return CEnumCore::TagName::CG2_titleFlags1;
	}	
	if(!strcmp(fieldname,"titleFlags002"))
	{
		return CEnumCore::TagName::CG2_titleFlags2;
	}	
	if(!strcmp(fieldname,"titleFlags003"))
	{
		return CEnumCore::TagName::CG2_titleFlags3;
	}	
	if(!strcmp(fieldname,"titleFlags004"))
	{
		return CEnumCore::TagName::CG2_titleFlags4;
	}	
	if(!strcmp(fieldname,"titleFlags005"))
	{
		return CEnumCore::TagName::CG2_titleFlags5;
	}	
	if(!strcmp(fieldname,"titleFlags006"))
	{
		return CEnumCore::TagName::CG2_titleFlags6;
	}	
	if(!strcmp(fieldname,"titleFlags007"))
	{
		return CEnumCore::TagName::CG2_titleFlags7;
	}	
	if(!strcmp(fieldname,"titleFlags008"))
	{
		return CEnumCore::TagName::CG2_titleFlags8;
	}	
	if(!strcmp(fieldname,"titleFlags009"))
	{
		return CEnumCore::TagName::CG2_titleFlags9;
	}	
	if(!strcmp(fieldname,"titleFlags010"))
	{
		return CEnumCore::TagName::CG2_titleFlags10;
	}	
	if(!strcmp(fieldname,"titleFlags011"))
	{
		return CEnumCore::TagName::CG2_titleFlags11;
	}	
	if(!strcmp(fieldname,"titleFlags012"))
	{
		return CEnumCore::TagName::CG2_titleFlags12;
	}	
	if(!strcmp(fieldname,"titleFlags013"))
	{
		return CEnumCore::TagName::CG2_titleFlags13;
	}	
	if(!strcmp(fieldname,"titleFlags014"))
	{
		return CEnumCore::TagName::CG2_titleFlags14;
	}	
	if(!strcmp(fieldname,"titleFlags015"))
	{
		return CEnumCore::TagName::CG2_titleFlags15;
	}	
	if(!strcmp(fieldname,"titleFlags016"))
	{
		return CEnumCore::TagName::CG2_titleFlags16;
	}	
	if(!strcmp(fieldname,"titleFlags017"))
	{
		return CEnumCore::TagName::CG2_titleFlags17;
	}	
	if(!strcmp(fieldname,"titleFlags018"))
	{
		return CEnumCore::TagName::CG2_titleFlags18;
	}	
	if(!strcmp(fieldname,"titleFlags019"))
	{
		return CEnumCore::TagName::CG2_titleFlags19;
	}	
	if(!strcmp(fieldname,"titleFlags020"))
	{
		return CEnumCore::TagName::CG2_titleFlags20;
	}	
	if(!strcmp(fieldname,"titleFlags021"))
	{
		return CEnumCore::TagName::CG2_titleFlags21;
	}	
	if(!strcmp(fieldname,"titleFlags022"))
	{
		return CEnumCore::TagName::CG2_titleFlags22;
	}	
	if(!strcmp(fieldname,"titleFlags023"))
	{
		return CEnumCore::TagName::CG2_titleFlags23;
	}	
	if(!strcmp(fieldname,"titleFlags024"))
	{
		return CEnumCore::TagName::CG2_titleFlags24;
	}	
	if(!strcmp(fieldname,"titleFlags025"))
	{
		return CEnumCore::TagName::CG2_titleFlags25;
	}	
	if(!strcmp(fieldname,"titleFlags026"))
	{
		return CEnumCore::TagName::CG2_titleFlags26;
	}	
	if(!strcmp(fieldname,"titleFlags027"))
	{
		return CEnumCore::TagName::CG2_titleFlags27;
	}	
	if(!strcmp(fieldname,"titleFlags028"))
	{
		return CEnumCore::TagName::CG2_titleFlags28;
	}	
	if(!strcmp(fieldname,"titleFlags029"))
	{
		return CEnumCore::TagName::CG2_titleFlags29;
	}	
	if(!strcmp(fieldname,"titleFlags030"))
	{
		return CEnumCore::TagName::CG2_titleFlags30;
	}
	//maple add
	if(!strcmp(fieldname,"charName"))
	{
		return CEnumCore::TagName::CG2_CharName;
	}
	if(!strcmp(fieldname,"mapID"))
	{
		return CEnumCore::TagName::CG2_MapID;
	}
	if(!strcmp(fieldname,"result"))
	{
		return CEnumCore::TagName::CG2_LevelResult;
	}
	if(!strcmp(fieldname,"value"))
	{
		return CEnumCore::TagName::CG2_LevelValue;
	}
	if(!strcmp(fieldname,"time"))
	{
		return CEnumCore::TagName::CG2_ActionTime;
	}
	if(!strcmp(fieldname,"string"))
	{
		return CEnumCore::TagName::CG2_String;
	}

	//new add
	if(!strcmp(fieldname,"iReserve00"))
	{
		return CEnumCore::TagName::CG2_iReserve00;//爱情树上次更新时间
	}
	if(!strcmp(fieldname,"Magic_BugIndex"))
	{
		return CEnumCore::TagName::CG2_BugIndex;//生虫指数
	}
	if(!strcmp(fieldname,"Magic_SoilIndex"))
	{
		return CEnumCore::TagName::CG2_SoilIndex;//土质指数
	}
	if(!strcmp(fieldname,"Magic_NutrientIndex"))
	{
		return CEnumCore::TagName::CG2_NutrientIndex;//养分指数
	}
	if(!strcmp(fieldname,"Magic_TreeState"))
	{
		return CEnumCore::TagName::CG2_TreeState;//树木状况
	}
	if(!strcmp(fieldname,"Magic_FruitNum"))
	{
		return CEnumCore::TagName::CG2_FruitNum;//果实数量
	}

	if(!strcmp(fieldname,"Magic_Dates"))
	{
		return CEnumCore::TagName::CG2_Dates;//时间
	}
	if(!strcmp(fieldname,"Magic_Marriage"))
	{
		return CEnumCore::TagName::CG2_Marriage;//预定人
	}
	if(!strcmp(fieldname,"Magic_Spouse"))
	{
		return CEnumCore::TagName::CG2_Spouse;//预定人配偶
	}
	if(!strcmp(fieldname,"Magic_MyIndex"))
	{
		return CEnumCore::TagName::CG2_MyIndex;//婚礼的场次号
	}
	if(!strcmp(fieldname,"Magic_isBooking"))
	{
		return CEnumCore::TagName::CG2_IsBooking;//是否被预定
	}
	if(!strcmp(fieldname,"Magic_MarriageTime"))
	{
		return CEnumCore::TagName::CG2_MarriageTime;//场地开始时间
	}
	if(!strcmp(fieldname,"Magic_Gmsv"))
	{
		return CEnumCore::TagName::CG2_Gmsv;//所在服务器
	}
	if(!strcmp(fieldname,"CG2_TempPwd"))
	{
		return CEnumCore::TagName::CG2_TempPwd;//所在服务器
	}
	if(!strcmp(fieldname,"itemNo"))
	{
		return CEnumCore::TagName::CG2_ItemColNum;//物品栏
	}
	if(!strcmp(fieldname,"SENDUSER"))
	{
		return CEnumCore::TagName::CG2_SendUser;//发送用户
	}
	if(!strcmp(fieldname,"RECVUSER"))
	{
		return CEnumCore::TagName::CG2_RecvUser;//接收用户 
	}
	if(!strcmp(fieldname,"ITEMTYPE"))
	{
		return CEnumCore::TagName::CG2_ItemType;//道具类型 
	}
	if(!strcmp(fieldname,"PERIOD"))
	{
		return CEnumCore::TagName::CG2_LimitTime;//道具时限 
	}
	if(!strcmp(fieldname,"BUYZONE")||!strcmp(fieldname,"areaid"))
	{
		return CEnumCore::TagName::CG2_ServerName;//服务器名称 
	}
	if(!strcmp(fieldname,"ORDERID"))
	{
		return CEnumCore::TagName::CG2_ID;//序列号
	}
	if(!strcmp(fieldname,"RECYCLE"))
	{
		return CEnumCore::TagName::CG2_Shop_RECYCLE;//是否兑换积分
	}
	if(!strcmp(fieldname,"regdate"))
	{
		return CEnumCore::TagName::CG2_Date;//激活时间
	}
	//超级舞者模块
	if(!strcmp(fieldname,"SDO_Times"))
	{
		return CEnumCore::TagName::SDO_Times;
	}
	if(!strcmp(fieldname,"SDO_OldIP"))
	{
		return CEnumCore::TagName::SDO_OldIP;
	}
	if(!strcmp(fieldname,"SDO_ServerIP"))
	{
		return CEnumCore::TagName::SDO_ServerIP;
	}
	if(!strcmp(fieldname,"SDO_UserIndexID"))
	{
		return CEnumCore::TagName::SDO_UserIndexID;
	}
	if(!strcmp(fieldname,"SDO_Account"))
	{
		return CEnumCore::TagName::SDO_Account;
	}
	if(!strcmp(fieldname,"SDO_CoupleUserName"))
	{
		return CEnumCore::TagName::SDO_CoupleUserName;
	}
	if(!strcmp(fieldname,"SDO_Level"))
	{
		return CEnumCore::TagName::SDO_Level;
	}
	if(!strcmp(fieldname,"SDO_Exp"))
	{
		return CEnumCore::TagName::SDO_Exp;
	}
	if(!strcmp(fieldname,"SDO_GameTotal"))
	{
		return CEnumCore::TagName::SDO_GameTotal;
	}
	if(!strcmp(fieldname,"SDO_GameWin"))
	{
		return CEnumCore::TagName::SDO_GameWin;
	}
	if(!strcmp(fieldname,"SDO_DogFall"))
	{
		return CEnumCore::TagName::SDO_DogFall;
	}
	if(!strcmp(fieldname,"SDO_GameFall"))
	{
		return CEnumCore::TagName::SDO_GameFall;
	}
	if(!strcmp(fieldname,"SDO_Reputation"))
	{
		return CEnumCore::TagName::SDO_Reputation;
	}
	if(!strcmp(fieldname,"SDO_GCash"))
	{
		return CEnumCore::TagName::SDO_GCash;
	}
	if(!strcmp(fieldname,"SDO_MCash"))
	{
		return CEnumCore::TagName::SDO_MCash;
	}
	if(!strcmp(fieldname,"SDO_Address"))
	{
		return CEnumCore::TagName::SDO_Address;
	}
	if(!strcmp(fieldname,"SDO_Age"))
	{
		return CEnumCore::TagName::SDO_Age;
	}
	if(!strcmp(fieldname,"SDO_ProductID"))
	{
		return CEnumCore::TagName::SDO_ProductID;
	}
	if(!strcmp(fieldname,"SDO_ProductName"))
	{
		return CEnumCore::TagName::SDO_ProductName;
	}
	if(!strcmp(fieldname,"SDO_ItemCode"))
	{
		return CEnumCore::TagName::SDO_ItemCode;
	}
	if(!strcmp(fieldname,"SDO_ItemName"))
	{
		return CEnumCore::TagName::SDO_ItemName;
	}
	if(!strcmp(fieldname,"SDO_TimesLimit"))
	{
		return CEnumCore::TagName::SDO_TimesLimit;
	}
	if(!strcmp(fieldname,"SDO_DateLimit"))
	{
		return CEnumCore::TagName::SDO_DateLimit;
	}
	if(!strcmp(fieldname,"SDO_MoneyType"))
	{
		return CEnumCore::TagName::SDO_MoneyType;
	}
	if(!strcmp(fieldname,"SDO_MoneyCost"))
	{
		return CEnumCore::TagName::SDO_MoneyCost;
	}
	if(!strcmp(fieldname,"SDO_ShopTime"))
	{
		return CEnumCore::TagName::SDO_ShopTime;
	}
	if(!strcmp(fieldname,"SDO_MAINCH"))
	{
		return CEnumCore::TagName::SDO_MAINCH;
	}
	if(!strcmp(fieldname,"SDO_SUBCH"))
	{
		return CEnumCore::TagName::SDO_SUBCH;
	}
	if(!strcmp(fieldname,"SDO_Online"))
	{
		return CEnumCore::TagName::SDO_Online;
	}
	if(!strcmp(fieldname,"SDO_LoginTime"))
	{
		return CEnumCore::TagName::SDO_LoginTime;
	}
	if(!strcmp(fieldname,"SDO_LogoutTime"))
	{
		return CEnumCore::TagName::SDO_LogoutTime;
	}
	if(!strcmp(fieldname,"SDO_AREANAME"))
	{
		return CEnumCore::TagName::SDO_AREANAME;
	}
	if(!strcmp(fieldname,"SDO_City"))
	{
		return CEnumCore::TagName::SDO_City;
	}
	if(!strcmp(fieldname,"SDO_Title"))
	{
		return CEnumCore::TagName::SDO_Title;
	}
	if(!strcmp(fieldname,"SDO_Context"))
	{
		return CEnumCore::TagName::SDO_Context;
	}
	if(!strcmp(fieldname,"SDO_MinLevel"))
	{
		return CEnumCore::TagName::SDO_MinLevel;
	}
	if(!strcmp(fieldname,"SDO_ActiveStatus"))
	{
		return CEnumCore::TagName::SDO_ActiveStatus;
	}
	if(!strcmp(fieldname,"SDO_StopStatus"))
	{
		return CEnumCore::TagName::SDO_StopStatus;
	}
	if(!strcmp(fieldname,"SDO_NickName"))
	{
		return CEnumCore::TagName::SDO_NickName;
	}
	if(!strcmp(fieldname,"SDO_9YouAccount"))
	{
		return CEnumCore::TagName::SDO_9YouAccount;
	}
	if(!strcmp(fieldname,"SDO_SEX"))
	{
		return CEnumCore::TagName::SDO_SEX;
	}
	if(!strcmp(fieldname,"SDO_RegistDate"))
	{
		return CEnumCore::TagName::SDO_RegistDate;
	}
	if(!strcmp(fieldname,"SDO_FirstLogintime"))
	{
		return CEnumCore::TagName::SDO_FirstLogintime;
	}
	if(!strcmp(fieldname,"SDO_LastLogintime"))
	{
		return CEnumCore::TagName::SDO_LastLogintime;
	}
	if(!strcmp(fieldname,"SDO_Ispad"))
	{
		return CEnumCore::TagName::SDO_Ispad;
	}
	if(!strcmp(fieldname,"SDO_Desc"))
	{
		return CEnumCore::TagName::SDO_Desc;
	}
	if(!strcmp(fieldname,"SDO_Postion"))
	{
		return CEnumCore::TagName::SDO_Postion;
	}
	if(!strcmp(fieldname,"SDO_BeginTime"))
	{
		return CEnumCore::TagName::SDO_BeginTime;
	}
	if(!strcmp(fieldname,"SDO_EndTime"))
	{
		return CEnumCore::TagName::SDO_EndTime;
	}
	if(!strcmp(fieldname,"SDO_SendTime"))
	{
		return CEnumCore::TagName::SDO_SendTime;
	}
	if(!strcmp(fieldname,"SDO_SendIndexID"))
	{
		return CEnumCore::TagName::SDO_SendIndexID;
	}
	if(!strcmp(fieldname,"SDO_SendUserID"))
	{
		return CEnumCore::TagName::SDO_SendUserID;
	}
	if(!strcmp(fieldname,"SDO_ReceiveNick"))
	{
		return CEnumCore::TagName::SDO_ReceiveNick;
	}
	if(!strcmp(fieldname,"SDO_BigType"))
	{
		return CEnumCore::TagName::SDO_BigType;
	}
	if(!strcmp(fieldname,"SDO_SmallType"))
	{
		return CEnumCore::TagName::SDO_SmallType;
	}
	if(!strcmp(fieldname,"SDO_REASON"))
	{
		return CEnumCore::TagName::SDO_REASON;
	}
	if(!strcmp(fieldname,"SDO_StopTime"))
	{
		return CEnumCore::TagName::SDO_StopTime;
	}
	if(!strcmp(fieldname,"SDO_DaysLimit"))
	{
		return CEnumCore::TagName::SDO_DaysLimit;
	}
	if(!strcmp(fieldname,"SDO_Email"))
	{
		return CEnumCore::TagName::SDO_Email;
	}
	if(!strcmp(fieldname,"SDO_ChargeSum"))
	{
		return CEnumCore::TagName::SDO_ChargeSum;
	}
	if(!strcmp(fieldname,"SDO_KeyID"))
	{
		return CEnumCore::TagName::SDO_KeyID;
	}
	if(!strcmp(fieldname,"SDO_KeyWord"))
	{
		return CEnumCore::TagName::SDO_KeyWord;
	}
	if(!strcmp(fieldname,"SDO_MasterID"))
	{
		return CEnumCore::TagName::SDO_MasterID;
	}
	if(!strcmp(fieldname,"SDO_Master"))
	{
		return CEnumCore::TagName::SDO_Master;
	}
	if(!strcmp(fieldname,"SDO_SlaverID"))
	{
		return CEnumCore::TagName::SDO_SlaverID;
	}
	if(!strcmp(fieldname,"SDO_Slaver"))
	{
		return CEnumCore::TagName::SDO_Slaver;
	}
	if(!strcmp(fieldname,"SDO_ChannelList"))
	{
		return CEnumCore::TagName::SDO_ChannelList;
	}
	if(!strcmp(fieldname,"SDO_BoardMessage"))
	{
		return CEnumCore::TagName::SDO_BoardMessage;
	}
	if(!strcmp(fieldname,"SDO_wPlanetID"))
	{
		return CEnumCore::TagName::SDO_wPlanetID;
	}
	if(!strcmp(fieldname,"SDO_wChannelID"))
	{
		return CEnumCore::TagName::SDO_wChannelID;
	}
	if(!strcmp(fieldname,"SDO_iLimitUser"))
	{
		return CEnumCore::TagName::SDO_iLimitUser;
	}
	if(!strcmp(fieldname,"SDO_iCurrentUser"))
	{
		return CEnumCore::TagName::SDO_iCurrentUser;
	}
	if(!strcmp(fieldname,"SDO_ipaddr"))
	{
		return CEnumCore::TagName::SDO_ipaddr;
	}
	if(!strcmp(fieldname,"SDO_Interval"))
	{
		return CEnumCore::TagName::SDO_Interval;
	}
	if(!strcmp(fieldname,"SDO_TaskID"))
	{
		return CEnumCore::TagName::SDO_TaskID;
	}
	if(!strcmp(fieldname,"SDO_Status"))
	{
		return CEnumCore::TagName::SDO_Status;
	}
	if(!strcmp(fieldname,"SDO_Score"))
	{
		return CEnumCore::TagName::SDO_Score;
	}
	if(!strcmp(fieldname,"SDO_FirstPadTime"))
	{
		return CEnumCore::TagName::SDO_FirstPadTime;
	}
	if(!strcmp(fieldname,"SDO_BanDate"))
	{
		return CEnumCore::TagName::SDO_BanDate;
	}
	if(!strcmp(fieldname,"SDO_Passwd"))
	{
		return CEnumCore::TagName::SDO_Passwd;
	}
	if(!strcmp(fieldname,"SDO_OnlineTime"))
	{
		return CEnumCore::TagName::SDO_OnlineTime;
	}
	if(!strcmp(fieldname,"SDO_LevPercent"))
	{
		return CEnumCore::TagName::SDO_LevPercent;
	}
	if(!strcmp(fieldname,"SDO_ItemCodeBy"))
	{
		return CEnumCore::TagName::SDO_ItemCodeBy;
	}
	if(!strcmp(fieldname,"SDO_Punish_Status"))
	{
		return CEnumCore::TagName::SDO_Punish_Status;
	}
	if(!strcmp(fieldname,"SDO_fragment_num"))
	{
		return CEnumCore::TagName::SDO_fragment_num;
	}
	if(!strcmp(fieldname,"SDO_fragment_id"))
	{
		return CEnumCore::TagName::SDO_fragment_id;
	}
	if(!strcmp(fieldname,"SDO_BuyTimes"))
	{
		return CEnumCore::TagName::SDO_BuyTimes;
	}
	if(!strcmp(fieldname,"SDO_CoupleIndexID"))
	{
		return CEnumCore::TagName::SDO_CoupleIndexID;
	}
	if(!strcmp(fieldname,"SDO_CoupleNickName"))
	{
		return CEnumCore::TagName::SDO_CoupleNickName;
	}
	if(!strcmp(fieldname,"SDO_RingLevel"))
	{
		return CEnumCore::TagName::SDO_RingLevel;
	}
	if(!strcmp(fieldname,"SDO_NewRingLevel"))
	{
		return CEnumCore::TagName::SDO_NewRingLevel;
	}
	if(!strcmp(fieldname,"SDO_MarriageApp_Date"))
	{
		return CEnumCore::TagName::SDO_MarriageApp_Date;
	}
	if(!strcmp(fieldname,"SDO_DivorceApp_Date"))
	{
		return CEnumCore::TagName::SDO_DivorceApp_Date;
	}
	if(!strcmp(fieldname,"SDO_Marriage_Date"))
	{
		return CEnumCore::TagName::SDO_Marriage_Date;
	}
	if(!strcmp(fieldname,"SDO_Divorce_Date"))
	{
		return CEnumCore::TagName::SDO_Divorce_Date;
	}
	if(!strcmp(fieldname,"SDO_RewardItemTime"))
	{
		return CEnumCore::TagName::SDO_RewardItemTime;
	}
	if(!strcmp(fieldname,"SDO_PCash"))
	{
		return CEnumCore::TagName::SDO_PCash;
	}
	if(!strcmp(fieldname,"SDO_PunishTimes"))
	{
		return CEnumCore::TagName::SDO_PunishTimes;
	}
	if(!strcmp(fieldname,"SDO_DelTimes"))
	{
		return CEnumCore::TagName::SDO_DelTimes;
	}
	if(!strcmp(fieldname,"SDO_DATE"))
	{
		return CEnumCore::TagName::SDO_DATE;
	}
	if(!strcmp(fieldname,"SDO_IsBattle"))
	{
		return CEnumCore::TagName::SDO_IsBattle;
	}
	if(!strcmp(fieldname,"SDO_SenceID"))
	{
		return CEnumCore::TagName::SDO_SenceID;
	}
	if(!strcmp(fieldname,"SDO_WeekDay"))
	{
		return CEnumCore::TagName::SDO_WeekDay;
	}
	if(!strcmp(fieldname,"SDO_MatPtHR"))
	{
		return CEnumCore::TagName::SDO_MatPtHR;
	}
	if(!strcmp(fieldname,"SDO_MatPtMin"))
	{
		return CEnumCore::TagName::SDO_MatPtMin;
	}
	if(!strcmp(fieldname,"SDO_StPtHR"))
	{
		return CEnumCore::TagName::SDO_StPtHR;
	}
	if(!strcmp(fieldname,"SDO_StPtMin"))
	{
		return CEnumCore::TagName::SDO_StPtMin;
	}
	if(!strcmp(fieldname,"SDO_EdPtHR"))
	{
		return CEnumCore::TagName::SDO_EdPtHR;
	}
	if(!strcmp(fieldname,"SDO_EdPtMin"))
	{
		return CEnumCore::TagName::SDO_EdPtMin;
	}
	if(!strcmp(fieldname,"SDO_Sence"))
	{
		return CEnumCore::TagName::SDO_Sence;
	}
	if(!strcmp(fieldname,"SDO_MusicID1"))
	{
		return CEnumCore::TagName::SDO_MusicID1;
	}
	if(!strcmp(fieldname,"SDO_MusicName1"))
	{
		return CEnumCore::TagName::SDO_MusicName1;
	}
	if(!strcmp(fieldname,"SDO_LV1"))
	{
		return CEnumCore::TagName::SDO_LV1;
	}
	if(!strcmp(fieldname,"SDO_MusicID2"))
	{
		return CEnumCore::TagName::SDO_MusicID2;
	}
	if(!strcmp(fieldname,"SDO_DATE"))
	{
		return CEnumCore::TagName::SDO_DATE;
	}
	if(!strcmp(fieldname,"SDO_IsBattle"))
	{
		return CEnumCore::TagName::SDO_IsBattle;
	}
	if(!strcmp(fieldname,"SDO_SenceID"))
	{
		return CEnumCore::TagName::SDO_SenceID;
	}
	if(!strcmp(fieldname,"SDO_WeekDay"))
	{
		return CEnumCore::TagName::SDO_WeekDay;
	}
	if(!strcmp(fieldname,"SDO_MatPtHR"))
	{
		return CEnumCore::TagName::SDO_MatPtHR;
	}
	if(!strcmp(fieldname,"SDO_MatPtMin"))
	{
		return CEnumCore::TagName::SDO_MatPtMin;
	}
	if(!strcmp(fieldname,"SDO_StPtHR"))
	{
		return CEnumCore::TagName::SDO_StPtHR;
	}
	if(!strcmp(fieldname,"SDO_StPtMin"))
	{
		return CEnumCore::TagName::SDO_StPtMin;
	}
	if(!strcmp(fieldname,"SDO_EdPtHR"))
	{
		return CEnumCore::TagName::SDO_EdPtHR;
	}
	if(!strcmp(fieldname,"SDO_EdPtMin"))
	{
		return CEnumCore::TagName::SDO_EdPtMin;
	}
	if(!strcmp(fieldname,"SDO_Sence"))
	{
		return CEnumCore::TagName::SDO_Sence;
	}
	if(!strcmp(fieldname,"SDO_MusicID1"))
	{
		return CEnumCore::TagName::SDO_MusicID1;
	}
	if(!strcmp(fieldname,"SDO_MusicName1"))
	{
		return CEnumCore::TagName::SDO_MusicName1;
	}
	if(!strcmp(fieldname,"SDO_LV1"))
	{
		return CEnumCore::TagName::SDO_LV1;
	}
	if(!strcmp(fieldname,"SDO_MusicID2"))
	{
		return CEnumCore::TagName::SDO_MusicID2;
	}
	if(!strcmp(fieldname,"SDO_MusicName2"))
	{
		return CEnumCore::TagName::SDO_MusicName2;
	}
	if(!strcmp(fieldname,"SDO_LV2"))
	{
		return CEnumCore::TagName::SDO_LV2;
	}
	if(!strcmp(fieldname,"SDO_MusicID3"))
	{
		return CEnumCore::TagName::SDO_MusicID3;
	}
	if(!strcmp(fieldname,"SDO_MusicName3"))
	{
		return CEnumCore::TagName::SDO_MusicName3;
	}
	if(!strcmp(fieldname,"SDO_LV3"))
	{
		return CEnumCore::TagName::SDO_LV3;
	}
	if(!strcmp(fieldname,"SDO_MusicID4"))
	{
		return CEnumCore::TagName::SDO_MusicID4;
	}
	if(!strcmp(fieldname,"SDO_MusicName4"))
	{
		return CEnumCore::TagName::SDO_MusicName4;
	}
	if(!strcmp(fieldname,"SDO_LV4"))
	{
		return CEnumCore::TagName::SDO_LV4;
	}
	if(!strcmp(fieldname,"SDO_MusicID5"))
	{
		return CEnumCore::TagName::SDO_MusicID5;
	}
	if(!strcmp(fieldname,"SDO_MusicName5"))
	{
		return CEnumCore::TagName::SDO_MusicName5;
	}
	if(!strcmp(fieldname,"SDO_LV5"))
	{
		return CEnumCore::TagName::SDO_LV5;
	}
	if(!strcmp(fieldname,"SDO_Precent"))
	{
		return CEnumCore::TagName::SDO_Precent;
	}
	if(!strcmp(fieldname,"SDO_Type"))
	{
		return CEnumCore::TagName::SDO_Type;
	}
	if(!strcmp(fieldname,"SDO_Usedate"))
	{
		return CEnumCore::TagName::SDO_Usedate;
	}
	if(!strcmp(fieldname,"SDO_Area"))
	{
		return CEnumCore::TagName::SDO_Area;
	}
	if(!strcmp(fieldname,"SDO_Padstatus"))
	{
		return CEnumCore::TagName::SDO_Padstatus;
	}
	if(!strcmp(fieldname,"SDO_expcash"))
	{
		return CEnumCore::TagName::SDO_expcash;
	}
	if(!strcmp(fieldname,"SDO_usecash"))
	{
		return CEnumCore::TagName::SDO_usecash;
	}
	if(!strcmp(fieldname,"SDO_State"))
	{
		return CEnumCore::TagName::SDO_State;
	}
	if(!strcmp(fieldname,"SDO_mood"))
	{
		return CEnumCore::TagName::SDO_mood;
	}
	if(!strcmp(fieldname,"SDO_Food"))
	{
		return CEnumCore::TagName::SDO_Food;
	}
	if(!strcmp(fieldname,"SDO_ItemCode1"))
	{
		return CEnumCore::TagName::SDO_ItemCode1;
	}
	if(!strcmp(fieldname,"SDO_DateLimit1"))
	{
		return CEnumCore::TagName::SDO_DateLimit1;
	}
	if(!strcmp(fieldname,"SDO_TimeLimit1"))
	{
		return CEnumCore::TagName::SDO_TimeLimit1;
	}

	if(!strcmp(fieldname,"SDO_ItemCode2"))
	{
		return CEnumCore::TagName::SDO_ItemCode2;
	}
	if(!strcmp(fieldname,"SDO_DateLimit2"))
	{
		return CEnumCore::TagName::SDO_DateLimit2;
	}
	if(!strcmp(fieldname,"SDO_TimeLimit2"))
	{
		return CEnumCore::TagName::SDO_TimeLimit2;
	}
	
	if(!strcmp(fieldname,"SDO_ItemCode3"))
	{
		return CEnumCore::TagName::SDO_ItemCode3;
	}
	if(!strcmp(fieldname,"SDO_DateLimit3"))
	{
		return CEnumCore::TagName::SDO_DateLimit3;
	}
	if(!strcmp(fieldname,"SDO_TimeLimit3"))
	{
		return CEnumCore::TagName::SDO_TimeLimit3;
	}

	if(!strcmp(fieldname,"SDO_ItemCode4"))
	{
		return CEnumCore::TagName::SDO_ItemCode4;
	}
	if(!strcmp(fieldname,"SDO_DateLimit4"))
	{
		return CEnumCore::TagName::SDO_DateLimit4;
	}
	if(!strcmp(fieldname,"SDO_TimeLimit4"))
	{
		return CEnumCore::TagName::SDO_TimeLimit4;
	}

	if(!strcmp(fieldname,"SDO_ItemCode5"))
	{
		return CEnumCore::TagName::SDO_ItemCode5;
	}
	if(!strcmp(fieldname,"SDO_DateLimit5"))
	{
		return CEnumCore::TagName::SDO_DateLimit5;
	}
	if(!strcmp(fieldname,"SDO_TimeLimit5"))
	{
		return CEnumCore::TagName::SDO_TimeLimit5;
	}

	if(!strcmp(fieldname,"SDO_ItemName1"))
	{
		return CEnumCore::TagName::SDO_ItemName1;
	}
	if(!strcmp(fieldname,"SDO_ItemName2"))
	{
		return CEnumCore::TagName::SDO_ItemName2;
	}
	if(!strcmp(fieldname,"SDO_ItemName3"))
	{
		return CEnumCore::TagName::SDO_ItemName3;
	}
	if(!strcmp(fieldname,"SDO_ItemName4"))
	{
		return CEnumCore::TagName::SDO_ItemName4;
	}
	if(!strcmp(fieldname,"SDO_ItemName5"))
	{
		return CEnumCore::TagName::SDO_ItemName5;
	}
	if(!strcmp(fieldname,"SDO_KeyPress"))
	{
		return CEnumCore::TagName::SDO_KeyPress;
	}
	if(!strcmp(fieldname,"SDO_LoginErr"))
	{
		return CEnumCore::TagName::SDO_LoginErr;
	}
	if(!strcmp(fieldname,"SDO_PreValue"))
	{
		return CEnumCore::TagName::SDO_PreValue;
	}
	if(!strcmp(fieldname,"SDO_EndValue"))
	{
		return CEnumCore::TagName::SDO_EndValue;
	}
	if(!strcmp(fieldname,"SDO_NorProFirst"))
	{
		return CEnumCore::TagName::SDO_NorProFirst;
	}
	if(!strcmp(fieldname,"SDO_NorPro"))
	{
		return CEnumCore::TagName::SDO_NorPro;
	}
	if(!strcmp(fieldname,"SDO_SpePro"))
	{
		return CEnumCore::TagName::SDO_SpePro;
	}
	if(!strcmp(fieldname,"SDO_baoxiangid"))
	{
		return CEnumCore::TagName::SDO_baoxiangid;
	}
	if(!strcmp(fieldname,"SDO_Mark"))
	{
		return CEnumCore::TagName::SDO_Mark;
	}

	if(!strcmp(fieldname,"SDO_RemoteIp"))
	{
		return CEnumCore::TagName::SDO_RemoteIp;
	}
	if(!strcmp(fieldname,"SDO_RemoteType"))
	{
		return CEnumCore::TagName::SDO_RemoteType;
	}
	if(!strcmp(fieldname,"SDO_MsgID"))
	{
		return CEnumCore::TagName::SDO_MsgID;
	}
	if(!strcmp(fieldname,"SDO_channelname"))
	{
		return CEnumCore::TagName::SDO_channelname;
	}
	if(!strcmp(fieldname,"SDO_type"))
	{
		return CEnumCore::TagName::SDO_type;
	}
	if(!strcmp(fieldname,"SDO_UserName"))
	{
		return CEnumCore::TagName::SDO_UserName;
	}
	if(!strcmp(fieldname,"SDO_Message"))
	{
		return CEnumCore::TagName::SDO_Message;
	}
	if(!strcmp(fieldname,"SDO_datetime"))
	{
		return CEnumCore::TagName::SDO_datetime;
	}
	if(!strcmp(fieldname,"SDO_ServerName"))
	{
		return CEnumCore::TagName::SDO_ServerName;
	}
	if(!strcmp(fieldname,"SDO_ChannelID"))
	{
		return CEnumCore::TagName::SDO_ChannelID;
	}
	if(!strcmp(fieldname,"SDO_PlanetID"))
	{
		return CEnumCore::TagName::SDO_PlanetID;
	}
	if(!strcmp(fieldname,"SDO_PlanetName"))
	{
		return CEnumCore::TagName::SDO_PlanetName;
	}
	if(!strcmp(fieldname,"SDO_ChannelName"))
	{
		return CEnumCore::TagName::SDO_ChannelName;
	}

	if(!strcmp(fieldname,"SDO_FamilyName"))
	{
		return CEnumCore::TagName::SDO_FamilyName;
	}
	if(!strcmp(fieldname,"SDO_FamilyID"))
	{
		return CEnumCore::TagName::SDO_FamilyID;
	}
	if(!strcmp(fieldname,"SDO_BuyUserNick"))
	{
		return CEnumCore::TagName::SDO_BuyUserNick;
	}
	if(!strcmp(fieldname,"SDO_BeforeMoney"))
	{
		return CEnumCore::TagName::SDO_BeforeMoney;
	}
	if(!strcmp(fieldname,"SDO_Shaikh"))
	{
		return CEnumCore::TagName::SDO_Shaikh;
	}
	if(!strcmp(fieldname,"SDO_Contribute"))
	{
		return CEnumCore::TagName::SDO_Contribute;
	}
	if(!strcmp(fieldname,"SDO_Popular"))
	{
		return CEnumCore::TagName::SDO_Popular;
	}
	if(!strcmp(fieldname,"SDO_Curdaypopular"))
	{
		return CEnumCore::TagName::SDO_Curdaypopular;
	}
	if(!strcmp(fieldname,"SDO_Opensign"))
	{
		return CEnumCore::TagName::SDO_Opensign;
	}
	if(!strcmp(fieldname,"SDO_Fmlevel"))
	{
		return CEnumCore::TagName::SDO_Fmlevel;
	}
	if(!strcmp(fieldname,"SDO_Maxmember"))
	{
		return CEnumCore::TagName::SDO_Maxmember;
	}
	if(!strcmp(fieldname,"SDO_Curmember"))
	{
		return CEnumCore::TagName::SDO_Curmember;
	}
	if(!strcmp(fieldname,"SDO_Fortune"))
	{
		return CEnumCore::TagName::SDO_Fortune;
	}
	if(!strcmp(fieldname,"SDO_Found_Date"))
	{
		return CEnumCore::TagName::SDO_Found_Date;
	}
	if(!strcmp(fieldname,"SDO_Rank"))
	{
		return CEnumCore::TagName::SDO_Rank;
	}
	if(!strcmp(fieldname,"SDO_Honor"))
	{
		return CEnumCore::TagName::SDO_Honor;
	}
	if(!strcmp(fieldname,"SDO_UseCost"))
	{
		return CEnumCore::TagName::SDO_UseCost;
	}
	if(!strcmp(fieldname,"SDO_Duty"))
	{
		return CEnumCore::TagName::SDO_Duty;
	}
	if(!strcmp(fieldname,"SDO_Formal"))
	{
		return CEnumCore::TagName::SDO_Formal;
	}
	if(!strcmp(fieldname,"SDO_Emblemtype"))
	{
		return CEnumCore::TagName::SDO_Emblemtype;
	}
	if(!strcmp(fieldname,"SDO_Emblemnum"))
	{
		return CEnumCore::TagName::SDO_Emblemnum;
	}
	if(!strcmp(fieldname,"SDO_MoneyTime"))
	{
		return CEnumCore::TagName::SDO_MoneyTime;
	}
	if(!strcmp(fieldname,"SDO_PetLevel"))
	{
		return CEnumCore::TagName::SDO_PetLevel;
	}
	if(!strcmp(fieldname,"SDO_PetExperience"))
	{
		return CEnumCore::TagName::SDO_PetExperience;
	}
	if(!strcmp(fieldname,"SDO_JoinDate"))
	{
		return CEnumCore::TagName::SDO_JoinDate;
	}
	if(!strcmp(fieldname,"SDO_Money"))
	{
		return CEnumCore::TagName::SDO_Money;
	}

	if(!strcmp(fieldname,"ban_userid"))
	{
		return CEnumCore::TagName::Ban_UserID;
	}
	if(!strcmp(fieldname,"ban_username"))
	{
		return CEnumCore::TagName::Ban_UserName;
	}
	if(!strcmp(fieldname,"ban_nickid"))
	{
		return CEnumCore::TagName::Ban_NickID;
	}
	if(!strcmp(fieldname,"ban_usernick"))
	{
		return CEnumCore::TagName::Ban_UserNick;
	}
	if(!strcmp(fieldname,"ban_starttime"))
	{
		return CEnumCore::TagName::Ban_StartTime;
	}
	if(!strcmp(fieldname,"ban_endtime"))
	{
		return CEnumCore::TagName::Ban_EndTime;
	}
	if(!strcmp(fieldname,"ban_reason"))
	{
		return CEnumCore::TagName::Ban_Reason;
	}
	if(!strcmp(fieldname,"ban_memo"))
	{
		return CEnumCore::TagName::Ban_Memo;
	}
	if(!strcmp(fieldname,"ban_type"))
	{
		return CEnumCore::TagName::Ban_Type;
	}
	//maple add
	if(!strcmp(fieldname,"JW2_ProductPrice"))
	{
		return CEnumCore::TagName::JW2_ProductPrice;
	}
	if(!strcmp(fieldname,"JW2_ServerName"))
	{
		return CEnumCore::TagName::JW2_ServerName;
	}
	if(!strcmp(fieldname,"UserName"))
	{
		return CEnumCore::TagName::UserName;
	}

	/////WA功能
	if(!strcmp(fieldname,"WA_Account"))
	{
		return CEnumCore::TagName::WA_Account;
	}
	if(!strcmp(fieldname,"WA_UserNick"))
	{
		return CEnumCore::TagName::WA_UserNick;
	}
	if(!strcmp(fieldname,"WA_AccountID"))
	{
		return CEnumCore::TagName::WA_AccountID;
	}
	if(!strcmp(fieldname,"WA_NickID"))
	{
		return CEnumCore::TagName::WA_NickID;
	}
	if(!strcmp(fieldname,"WA_TaskID"))
	{
		return CEnumCore::TagName::WA_TaskID;
	}
	if(!strcmp(fieldname,"WA_BeginTime"))
	{
		return CEnumCore::TagName::WA_BeginTime;
	}
	if(!strcmp(fieldname,"WA_EndTime"))
	{
		return CEnumCore::TagName::WA_EndTime;
	}
	if(!strcmp(fieldname,"WA_Interval"))
	{
		return CEnumCore::TagName::WA_Interval;
	}
	if(!strcmp(fieldname,"WA_BoardMessage"))
	{
		return CEnumCore::TagName::WA_BoardMessage;
	}
	if(!strcmp(fieldname,"WA_Status"))
	{
		return CEnumCore::TagName::WA_Status;
	}
	if(!strcmp(fieldname,"WA_ServerIP"))
	{
		return CEnumCore::TagName::WA_ServerIP;
	}
	if(!strcmp(fieldname,"WA_ServerName"))
	{
		return CEnumCore::TagName::WA_ServerName;
	}	
	if(!strcmp(fieldname,"WA_GSServerIP"))
	{
		return CEnumCore::TagName::WA_GSServerIP;
	}
	if(!strcmp(fieldname,"WA_RemainTime"))
	{
		return CEnumCore::TagName::WA_RemainTime;
	}
	if(!strcmp(fieldname,"WA_Reason"))
	{
		return CEnumCore::TagName::WA_Reason;
	}
	if(!strcmp(fieldname,"WA_skillID"))
	{
		return CEnumCore::TagName::WA_SkillID;
	}
	if(!strcmp(fieldname,"WA_SkillName"))
	{
		return CEnumCore::TagName::WA_SkillName;
	}
	if(!strcmp(fieldname,"WA_ItemID"))
	{
		return CEnumCore::TagName::WA_ItemID;
	}
	if(!strcmp(fieldname,"WA_ItemType"))
	{
		return CEnumCore::TagName::WA_TitleName1;
	}
	if(!strcmp(fieldname,"WA_ItemName"))
	{
		return CEnumCore::TagName::WA_itemName;
	}

	//add by ltx
	/////XDJG功能
	if(!strcmp(fieldname,"XDJG_TaskID"))
	{
		return CEnumCore::TagName::XDJG_TaskID;
	}
	if(!strcmp(fieldname,"XDJG_BeginTime"))
	{
		return CEnumCore::TagName::XDJG_BeginTime;
	}
	if(!strcmp(fieldname,"XDJG_EndTime"))
	{
		return CEnumCore::TagName::XDJG_EndTime;
	}
	if(!strcmp(fieldname,"XDJG_Interval"))
	{
		return CEnumCore::TagName::XDJG_Interval;
	}
	if(!strcmp(fieldname,"XDJG_Interval"))
	{
		return CEnumCore::TagName::XDJG_Interval;
	}
	if(!strcmp(fieldname,"XDJG_Status"))
	{
		return CEnumCore::TagName::XDJG_Status;
	}
	if(!strcmp(fieldname,"XDJG_Status"))
	{
		return CEnumCore::TagName::XDJG_Status;
	}
	if(!strcmp(fieldname,"XDJG_BoardMessage"))
	{
		return CEnumCore::TagName::XDJG_BoardMessage;
	}
	if(!strcmp(fieldname,"XDJG_ServerName"))
	{
		return CEnumCore::TagName::XDJG_ServerName;
	}

	//Added by tengjie 2010-07-09
	if(!strcmp(fieldname,"XDJG_AccountId"))
	{
		return CEnumCore::TagName::XDJG_AccountID;
	}
	if(!strcmp(fieldname,"XDJG_Account"))
	{
		return CEnumCore::TagName::XDJG_Account;
	}
	if(!strcmp(fieldname,"XDJG_Character"))
	{
		return CEnumCore::TagName::XDJG_Character;
	}
	if(!strcmp(fieldname,"XDJG_Opentime"))
	{
		return CEnumCore::TagName::XDJG_BANBEGINTIME;
	}
	if(!strcmp(fieldname,"XDJG_Closetime"))
	{
		return CEnumCore::TagName::XDJG_BANENDTIME;
	}
	if(!strcmp(fieldname,"XDJG_Reason"))
	{
		return CEnumCore::TagName::XDJG_BANREASON;
	}
	if(!strcmp(fieldname,"XDJG_LogType"))
	{
		return CEnumCore::TagName::XDJG_LoginType;
	}
	if(!strcmp(fieldname,"XDJG_LogTime"))
	{
		return CEnumCore::TagName::XDJG_LogTime;
	}
	if(!strcmp(fieldname,"XDJG_LogIP"))
	{
		return CEnumCore::TagName::XDJG_LogInOutIP;
	}
	//购买人帐号
	if(!strcmp(fieldname,"XDJG_AccountBuyer"))
	{
		return CEnumCore::TagName::XDJG_AccountBuyer;
	}
	//购买人昵称
	if(!strcmp(fieldname,"XDJG_RoleBuyer"))
	{
		return CEnumCore::TagName::XDJG_RoleBuyer;
	}
	//获赠人帐号
	if(!strcmp(fieldname,"XDJG_AccountReceiver"))
	{
		return CEnumCore::TagName::XDJG_AccountReceiver;
	}
	//获赠人昵称
	if(!strcmp(fieldname,"XDJG_RoleReceiver"))
	{
		return CEnumCore::TagName::XDJG_RoleReceiver;
	}
	//道具ID1
	if(!strcmp(fieldname,"XDJG_ItemID1"))
	{
		return CEnumCore::TagName::XDJG_ItemID1;
	}
	//道具ID2
	if(!strcmp(fieldname,"XDJG_ItemID2"))
	{
		return CEnumCore::TagName::XDJG_ItemID2;
	}
	//道具ID3
	if(!strcmp(fieldname,"XDJG_ItemID3"))
	{
		return CEnumCore::TagName::XDJG_ItemID3;
	}
	//道具ID4
	if(!strcmp(fieldname,"XDJG_ItemID4"))
	{
		return CEnumCore::TagName::XDJG_ItemID4;
	}
	//道具ID5
	if(!strcmp(fieldname,"XDJG_ItemID5"))
	{
		return CEnumCore::TagName::XDJG_ItemID5;
	}
	//道具ID6
	if(!strcmp(fieldname,"XDJG_ItemID6"))
	{
		return CEnumCore::TagName::XDJG_ItemID6;
	}
	//道具ID7
	if(!strcmp(fieldname,"XDJG_ItemID7"))
	{
		return CEnumCore::TagName::XDJG_ItemID7;
	}
	//道具ID8
	if(!strcmp(fieldname,"XDJG_ItemID8"))
	{
		return CEnumCore::TagName::XDJG_ItemID8;
	}
	//道具数量1
	if(!strcmp(fieldname,"XDJG_ItemNum1"))
	{
		return CEnumCore::TagName::XDJG_ItemNum1;
	}
	//道具数量2
	if(!strcmp(fieldname,"XDJG_ItemNum2"))
	{
		return CEnumCore::TagName::XDJG_ItemNum2;
	}
	//道具数量3
	if(!strcmp(fieldname,"XDJG_ItemNum3"))
	{
		return CEnumCore::TagName::XDJG_ItemNum3;
	}
	//道具数量4
	if(!strcmp(fieldname,"XDJG_ItemNum4"))
	{
		return CEnumCore::TagName::XDJG_ItemNum4;
	}
	//道具数量5
	if(!strcmp(fieldname,"XDJG_ItemNum5"))
	{
		return CEnumCore::TagName::XDJG_ItemNum5;
	}
	//道具数量6
	if(!strcmp(fieldname,"XDJG_ItemNum6"))
	{
		return CEnumCore::TagName::XDJG_ItemNum6;
	}
	//道具数量7
	if(!strcmp(fieldname,"XDJG_ItemNum7"))
	{
		return CEnumCore::TagName::XDJG_ItemNum7;
	}
	//道具数量8
	if(!strcmp(fieldname,"XDJG_ItemNum8"))
	{
		return CEnumCore::TagName::XDJG_ItemNum8;
	}
	//道具类型
	if(!strcmp(fieldname,"XDJG_ItemType"))
	{
		return CEnumCore::TagName::XDJG_ItemType;
	}
	//来源名字
	if(!strcmp(fieldname,"XDJG_CharacterFrom"))
	{
		return CEnumCore::TagName::XDJG_CharacterFrom;
	}
	//道具临时ID1
	if(!strcmp(fieldname,"XDJG_TempItemId1"))
	{
		return CEnumCore::TagName::XDJG_TempItemId1;
	}
	//道具临时ID2
	if(!strcmp(fieldname,"XDJG_TempItemId2"))
	{
		return CEnumCore::TagName::XDJG_TempItemId2;
	}
	//道具临时ID3
	if(!strcmp(fieldname,"XDJG_TempItemId3"))
	{
		return CEnumCore::TagName::XDJG_TempItemId3;
	}
	//道具临时ID4
	if(!strcmp(fieldname,"XDJG_TempItemId4"))
	{
		return CEnumCore::TagName::XDJG_TempItemId4;
	}
	//道具临时ID5
	if(!strcmp(fieldname,"XDJG_TempItemId5"))
	{
		return CEnumCore::TagName::XDJG_TempItemId5;
	}
	//道具临时ID6
	if(!strcmp(fieldname,"XDJG_TempItemId6"))
	{
		return CEnumCore::TagName::XDJG_TempItemId6;
	}
	//道具临时ID7
	if(!strcmp(fieldname,"XDJG_TempItemId7"))
	{
		return CEnumCore::TagName::XDJG_TempItemId7;
	}
	//道具临时ID8
	if(!strcmp(fieldname,"XDJG_TempItemId8"))
	{
		return CEnumCore::TagName::XDJG_TempItemId8;
	}
	//金钱数
	if(!strcmp(fieldname,"XDJG_Money"))
	{
		return CEnumCore::TagName::XDJG_Money;
	}
	//金钱类型
	if(!strcmp(fieldname,"XDJG_Moneytype"))
	{
		return CEnumCore::TagName::XDJG_MoneyType;
	}
	//道具名称
	if(!strcmp(fieldname,"XDJG_ItemName"))
	{
		return CEnumCore::TagName::XDJG_ItemName;
	}
	//玩家等级
	if(!strcmp(fieldname,"XDJG_Level"))
	{
		return CEnumCore::TagName::XDJG_Level;
	}
	//所有权
	if(!strcmp(fieldname,"XDJG_Property"))
	{
		return CEnumCore::TagName::XDJG_Property;
	}
	//善恶
	if(!strcmp(fieldname,"XDJG_Moral"))
	{
		return CEnumCore::TagName::XDJG_GoodEvil;
	}
	//红名
	if(!strcmp(fieldname,"XDJG_Cooldowns"))
	{
		return CEnumCore::TagName::XDJG_Cooldowns;
	}
	//角色称号
	if(!strcmp(fieldname,"XDJG_Titles"))
	{
		return CEnumCore::TagName::XDJG_Titles;
	}
	//发件人帐号
	if(!strcmp(fieldname,"XDJG_SendAccount"))
	{
		return CEnumCore::TagName::XDJG_SendAccount;
	}
	//发件人角色
	if(!strcmp(fieldname,"XDJG_SendCharacter"))
	{
		return CEnumCore::TagName::XDJG_SendCharacter;
	}
	//邮件标题
	if(!strcmp(fieldname,"XDJG_Title"))
	{
		return CEnumCore::TagName::XDJG_Title;
	}
	//接收时间
	if(!strcmp(fieldname,"XDJG_ReceiveTime"))
	{
		return CEnumCore::TagName::XDJG_ReceiveTime;
	}
	//技能ID
	if(!strcmp(fieldname,"XDJG_SkillId"))
	{
		return CEnumCore::TagName::XDJG_SkillID;
	}
	//技能名称
	if(!strcmp(fieldname,"XDJG_SkillName"))
	{
		return CEnumCore::TagName::XDJG_SkillName;
	}
	//任务ID
	if(!strcmp(fieldname,"XDJG_Task_ID"))
	{
		return CEnumCore::TagName::XDJG_Task_ID;
	}
	//任务临时ID
	if(!strcmp(fieldname,"XDJG_TempTaskId"))
	{
		return CEnumCore::TagName::XDJG_TempTaskId;
	}
	//任务类型
	if(!strcmp(fieldname,"XDJG_TaskType"))
	{
		return CEnumCore::TagName::XDJG_TaskType;
	}
	//失去道具ID1
	if(!strcmp(fieldname,"XDJG_ItemThrowID1"))
	{
		return CEnumCore::TagName::XDJG_ItemThrowID1;
	}
	//失去道具ID2
	if(!strcmp(fieldname,"XDJG_ItemThrowID2"))
	{
		return CEnumCore::TagName::XDJG_ItemThrowID2;
	}
	//失去道具ID3
	if(!strcmp(fieldname,"XDJG_ItemThrowID3"))
	{
		return CEnumCore::TagName::XDJG_ItemThrowID3;
	}
	//失去道具数量1
	if(!strcmp(fieldname,"XDJG_ItemThrowNum1"))
	{
		return CEnumCore::TagName::XDJG_ItemThrowNum1;
	}
	//失去道具数量2
	if(!strcmp(fieldname,"XDJG_ItemThrowNum2"))
	{
		return CEnumCore::TagName::XDJG_ItemThrowNum2;
	}
	//失去道具数量3
	if(!strcmp(fieldname,"XDJG_ItemThrowNum3"))
	{
		return CEnumCore::TagName::XDJG_ItemThrowNum3;
	}
	//失去道具临时ID1
	if(!strcmp(fieldname,"XDJG_TempItemThrowId1"))
	{
		return CEnumCore::TagName::XDJG_TempItemThrowId1;
	}
	//失去道具临时ID2
	if(!strcmp(fieldname,"XDJG_TempItemThrowId2"))
	{
		return CEnumCore::TagName::XDJG_TempItemThrowId2;
	}
	//失去道具临时ID3
	if(!strcmp(fieldname,"XDJG_TempItemThrowId3"))
	{
		return CEnumCore::TagName::XDJG_TempItemThrowId3;
	}

	//道具临时ID
	if(!strcmp(fieldname,"XDJG_TempItemID"))
	{
		return CEnumCore::TagName::XDJG_TempItemID;
	}
	//道具数量
	if(!strcmp(fieldname,"XDJG_ItemNum"))
	{
		return CEnumCore::TagName::XDJG_ItemNum;
	}
	//道具ID
	if(!strcmp(fieldname,"XDJG_ItemID"))
	{
		return CEnumCore::TagName::XDJG_ItemID;
	}
	//金钱类型
	if(!strcmp(fieldname,"XDJG_MoneyType"))
	{
		return CEnumCore::TagName::XDJG_MoneyType;
	}
	//物品属性
	if(!strcmp(fieldname,"XDJG_Items"))
	{
		return CEnumCore::TagName::XDJG_Items;
	}
	//剩余时间
	if(!strcmp(fieldname,"XDJG_SurplusTime"))
	{
		return CEnumCore::TagName::XDJG_SurplusTime;
	}
	//拍卖价格
	if(!strcmp(fieldname,"XDJG_AuctionPrice"))
	{
		return CEnumCore::TagName::XDJG_AuctionPrice;
	}
	//保证金
	if(!strcmp(fieldname,"XDJG_Deposit"))
	{
		return CEnumCore::TagName::XDJG_Deposit;
	}
	//生活技能类型
	if(!strcmp(fieldname,"XDJG_Prototype"))
	{
		return CEnumCore::TagName::XDJG_Prototype;
	}
	//公会ID
	if(!strcmp(fieldname,"XDJG_GuildID"))
	{
		return CEnumCore::TagName::XDJG_GuildID;
	}
	//公会名称
	if(!strcmp(fieldname,"XDJG_GuildName"))
	{
		return CEnumCore::TagName::XDJG_GuildName;
	}
	//公会等级
	if(!strcmp(fieldname,"XDJG_GuildLevel"))
	{
		return CEnumCore::TagName::XDJG_GuildLevel;
	}
	//会长名称
	if(!strcmp(fieldname,"XDJG_GuildLeader"))
	{
		return CEnumCore::TagName::XDJG_GuildLeader;
	}
	//操作时间
	if(!strcmp(fieldname,"XDJG_OperateTime"))
	{
		return CEnumCore::TagName::XDJG_OperateTime;
	}
	//操作类型
	if(!strcmp(fieldname,"XDJG_Operatetype"))
	{
		return CEnumCore::TagName::XDJG_Operatetype;
	}
	//消耗金钱
	if(!strcmp(fieldname,"XDJG_MoneyLose"))
	{
		return CEnumCore::TagName::XDJG_MoneyLose;
	}
	//临时角色名
	if(!strcmp(fieldname,"XDJG_TempCharacter"))
	{
		return CEnumCore::TagName::XDJG_TempCharacter;
	}
	//加入时间
	if(!strcmp(fieldname,"XDJG_JoinTime"))
	{
		return CEnumCore::TagName::XDJG_JoinTime;
	}
	//公会日志
	if(!strcmp(fieldname,"XDJG_GuildLog"))
	{
		return CEnumCore::TagName::XDJG_GuildLog;
	}
	//离开时间
	if(!strcmp(fieldname,"XDJG_LeaveTime"))
	{
		return CEnumCore::TagName::XDJG_LeaveTime;
	}
	//离开公会
	if(!strcmp(fieldname,"XDJG_ExitGuildType"))
	{
		return CEnumCore::TagName::XDJG_ExitGuildType;
	}
	//转职时间
	if(!strcmp(fieldname,"XDJG_LeaderChangedTime"))
	{
		return CEnumCore::TagName::XDJG_LeaderChangedTime;
	}
	//临时技能id
	if(!strcmp(fieldname,"XDJG_TempSkillId"))
	{
		return CEnumCore::TagName::XDJG_TempSkillId;
	}
	//创建时间
	if(!strcmp(fieldname,"XDJG_CreateTime"))
	{
		return CEnumCore::TagName::XDJG_CreateTime;
	}
	//会员名称
	if(!strcmp(fieldname,"XDJG_Guildmember"))
	{
		return CEnumCore::TagName::XDJG_Guildmember;
	}
	//会员职位
	if(!strcmp(fieldname,"XDJG_GuildPosition"))
	{
		return CEnumCore::TagName::XDJG_GuildPosition;
	}
	//会员人数
	if(!strcmp(fieldname,"XDJG_Guildmembers"))
	{
		return CEnumCore::TagName::XDJG_Guildmembers;
	}
	//变更后职位
	if(!strcmp(fieldname,"XDJG_GuildTransferedPosition"))
	{
		return CEnumCore::TagName::XDJG_GuildTransferedPosition;
	}
	//消耗道具
	if(!strcmp(fieldname,"XDJG_ConsumeItem"))
	{
		return CEnumCore::TagName::XDJG_ConsumeItem;
	}
	//基础属性
	if(!strcmp(fieldname,"XDJG_RoleProperty"))
	{
		return CEnumCore::TagName::XDJG_RoleProperty;
	}
	//金刚名称
	if(!strcmp(fieldname,"XDJG_CarName"))
	{
		return CEnumCore::TagName::XDJG_CarName;
	}
	//任务临时ID
	if(!strcmp(fieldname,"XDJG_TempTaskId"))
	{
		return CEnumCore::TagName::XDJG_TempTaskId;
	}
	//是否删除
	if(!strcmp(fieldname,"XDJG_IsDeleted"))
	{
		return CEnumCore::TagName::XDJG_IsDeleted;
	}
	//角色id
	if(!strcmp(fieldname,"XDJG_CharacterID"))
	{
		return CEnumCore::TagName::XDJG_CharacterID;
	}
	//性别
	if(!strcmp(fieldname,"XDJG_Sex"))
	{
		return CEnumCore::TagName::XDJG_Sex;
	}
	//职业
	if(!strcmp(fieldname,"XDJG_Profession"))
	{
		return CEnumCore::TagName::XDJG_Profession;
	}
	//场景id
	if(!strcmp(fieldname,"XDJG_SceneTypeID"))
	{
		return CEnumCore::TagName::XDJG_SceneTypeID;
	}
	//场景实例id
	if(!strcmp(fieldname,"XDJG_SceneInstanceID"))
	{
		return CEnumCore::TagName::XDJG_SceneInstanceID;
	}
	//BUFFid
	if(!strcmp(fieldname,"XDJG_BuffId"))
	{
		return CEnumCore::TagName::XDJG_BuffID;
	}
	//BUFF名称
	if(!strcmp(fieldname,"XDJG_BuffName"))
	{
		return CEnumCore::TagName::XDJG_BuffName;
	}
	//End
	
	return CEnumCore::TagName::MESSAGE;
}