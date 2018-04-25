// PalAPI.cpp: implementation of the CPalAPI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PalAPI.h"
#include "md5.h"

#pragma comment(lib,"md5.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPalAPI::CPalAPI()
{
}

CPalAPI::~CPalAPI()
{
	Destroy();
}

//查询玩家背包
void CPalAPI::Pal_CharacterBag(char * ServerName, char * ServerIP, char * RoleName, int ibagtype,int iIndex,int iPageSize)
{
	try
	{
		CCharacterBag CB;//实例化玩家背包对象
		vector <CGlobalStruct::TFLV> DBTFLV=CB.CharacterBagMain(ServerName,ServerIP,RoleName,ibagtype);//调用玩家背包查询接口
		operPal.BuildTLV(COperVector(DBTFLV),iIndex,iPageSize);////根据页数和记录数构建数据集数据包
		//给数据包添加包头
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERBAG_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	

	}
	
	catch (...)
	{
		operPal.BuildTLV("背包为空");///构建message数据包
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERBAG_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	
	}

}

//取得角色好友列表
void CPalAPI::Pal_CharacterFriendList(char * ServerName, char * ServerIP, char * RoleName,int iIndex,int iPageSize)
{
	try
	{
		CCharacterFriendList CFL;//实例化角色好友列表信息
		vector <CGlobalStruct::TFLV> DBTFLV=CFL.CharacterFriendListMain(ServerName,ServerIP,RoleName);//调用角色好友列表信息接口
		operPal.BuildTLV(COperVector(DBTFLV),iIndex,iPageSize);////根据页数和记录数构建数据集数据包
		//给数据包添加包头
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERFRIENDLIST_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	

	}
	catch(...)
	{
		operPal.BuildTLV("好友列表为空");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERFRIENDLIST_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	
	}
}

//取得角色任务资讯
void CPalAPI::Pal_CharacterQuest(char * ServerName, char * ServerIP,char * RoleName,int iType,int iIndex,int iPageSize)
{
	try
	{
		CCharacterQuest CQ;//实例化角色任务信息
		vector <CGlobalStruct::TFLV> DBTFLV=CQ.CharacterQuestMain(ServerName,ServerIP,RoleName,iType);//调用角色任务信息接口
		operPal.BuildTLV(COperVector(DBTFLV),iIndex,iPageSize);////根据页数和记录数构建数据集数据包
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERQUEST_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	

	}
	catch(...)
	{
		operPal.BuildTLV("任务为空");///构建message数据包
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERQUEST_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	
	}

}

//取得公会基本资讯
void CPalAPI::Pal_GuileBaseInfo(char * ServerName, char * ServerIP,char * GuildName,int iIndex, int iPageSize)
{
	try
	{
		CGuildBaseInfo GBI;	//实例化工会基本信息
		vector <CGlobalStruct::TFLV> DBTFLV=GBI.GuildBaseInfoMain(ServerName,ServerIP,GuildName);//调用工会基本信息接口
		operPal.BuildTLV(COperVector(DBTFLV),iIndex,iPageSize);////根据页数和记录数构建数据集数据包
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_GUILDBASEINFO_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	

	}
	catch(...)
	{
		operPal.BuildTLV("公会基本资讯无");///构建message数据包
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_GUILDBASEINFO_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	
	}
	
}

//取得公会成员列表
void CPalAPI::Pal_GuildMemberList(char * ServerName, char * ServerIP,char * GuildName,int iIndex, int iPageSize)
{
	try
	{
		CGuildMemberList GML;//实例化工会成员信息
		vector <CGlobalStruct::TFLV> DBTFLV=GML.GuildMemberListMain(ServerName,ServerIP,GuildName);//调用工会成员信息接口
		operPal.BuildTLV(COperVector(DBTFLV),iIndex,iPageSize);////根据页数和记录数构建数据集数据包
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_GUILDMEMBERLIST_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	

	}
	catch(...)
	{
		operPal.BuildTLV("公会成员信息无");///构建message数据包
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_GUILDMEMBERLIST_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	
	}
	
}

//寄送奖品邮件（最多赠送5个礼物）
void CPalAPI::Pal_SendGiftMail(int UserByID,
							   char * ServerName,
							   char * ServerIP,
							   char * RoleName,
							   char * Subject,
							   char * Content,
							   int iMoney,
							   int Gift0,
							   int Gift1,
							   int Gift2,
							   int Gift3,
							   int Gift4,
							   char * reason)
{
	try
	{
		CSendGiftMail SGM;//实例化寄送奖品邮件
		vector<CGlobalStruct::TFLV> DBTFLV=SGM.SendGiftMailMain(ServerName,ServerIP,RoleName,Subject,Content,iMoney,
			Gift0,Gift1,Gift2,Gift3,Gift4,reason);//调用寄送奖品邮件接口
		operPal.BuildTLV((COperVector)DBTFLV,0,0);//构建数据包
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_SENDGIFTMAIL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		char giftname[100];
		char text[512];
		ZeroMemory(giftname,100);
		ZeroMemory(text,512);
		sprintf(text,"角色名<%s>,",RoleName);
		operPal.GiftIDToName(giftname,Gift0);//根据道具ID获得道具名称
		sprintf(text,"%s道具名:%s",text,giftname);
		operPal.GiftIDToName(giftname,Gift1);
		sprintf(text,"%s道具名:%s",text,giftname);
		operPal.GiftIDToName(giftname,Gift2);
		sprintf(text,"%s道具名:%s",text,giftname);
		operPal.GiftIDToName(giftname,Gift3);
		sprintf(text,"%s道具名:%s",text,giftname);
		operPal.GiftIDToName(giftname,Gift4);
		sprintf(text,"%s道具名:%s",text,giftname);
		sprintf(text,"%s文钱数%i",text,iMoney);
		operPal.RecordData(UserByID,CEnumCore::Message_Tag_ID::PAL_SENDGIFTMAIL,ServerName,
			&COperVector(DBTFLV),text);//记录操作日志
	}
	catch(...)
	{
		operPal.BuildTLV("邮件发送失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_SENDGIFTMAIL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());

	}
	
}

//解散公会
void CPalAPI::Pal_DismissGuild(int userByID,char * ServerName, char * ServerIP,char * GuildName,char * Reason)
{
	try
	{
		CDismissGuild DG;//实例化解散工会对象	
		vector<CGlobalStruct::TFLV> DBTFLV=DG.DismissGuildMain(ServerName,ServerIP,GuildName);//调用解散工会接口
		operPal.BuildTLV((COperVector)DBTFLV,0,0);//构建数据包
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_DISMISSGUILD_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		char text[256];
		ZeroMemory(text,256);
		sprintf(text,"服务器名称:%s理由:%s",ServerName,Reason);
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_DISMISSGUILD,ServerName,&((COperVector)DBTFLV),text);//记录操作日志
		
	}
	catch(...)
	{
		operPal.BuildTLV("解散公会失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_DISMISSGUILD_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	
}

//修改公会名称
void CPalAPI::Pal_ModifyGuildName(int userByID,char * ServerName, char * ServerIP,char * GuildName,char * NewName,char * Reason)
{
	try
	{
		CModifyGuildName MGN;//实例化修改工会名称对象	
		vector<CGlobalStruct::TFLV> DBTFLV=MGN.ModifyGuildNameMain(ServerName,ServerIP,GuildName,NewName);//调用修改工会名称接口
		operPal.BuildTLV((COperVector)DBTFLV,0,0);//构建数据包
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYGUILDNAME_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYGUILDNAME,ServerName,&(COperVector(DBTFLV)),Reason);//记录操作日志

	}
	catch(...)
	{
		operPal.BuildTLV("更改公会名称失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYGUILDNAME_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	
}


//修改公会成员职阶
void CPalAPI::Pal_ModifyGuildMemberPL(int userByID,char * ServerName, char * ServerIP,char * GuildName,char * RoleName, int iLevel,char * Reason)
{
	try
	{
		CModifyGuildMemberPL MGMPL;//实例化修改工会成员职阶对象
		vector<CGlobalStruct::TFLV> DBTFLV=MGMPL.ModifyGuildMemberPLMain(ServerName,ServerIP,GuildName,RoleName,iLevel);//调用修改工会成员职阶接口
		operPal.BuildTLV((COperVector)DBTFLV,0,0);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYGUILDMEMBERPL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYGUILDMEMBERPL,ServerName,&(COperVector(DBTFLV)),Reason);//记录操作日志

	}
	catch(...)
	{
		operPal.BuildTLV("更改公会成员职级失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYGUILDMEMBERPL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	
}

//修改角色任务	
void CPalAPI::Pal_ModifyCharacterQuest(int userByID,char *ServerName, char * ServerIP,char* szRoleName,int iQuestID,int iQuestStatus)
{
	try
	{
		CModifyCharacterQuest MCQ;//实例化修改角色任务对象	
		vector <CGlobalStruct::TFLV> DBTFLV=MCQ.ModifyCharacterQuestMain(userByID,ServerName,ServerIP,szRoleName,iQuestID,iQuestStatus);//调用修改角色任务接口
		operPal.BuildTLV((COperVector)DBTFLV,0,0);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYCHARACTERMONEY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYCHARACTERMONEY,ServerName,&(COperVector(DBTFLV)),"");//记录操作日志
	}
	catch(...)
	{
		operPal.BuildTLV("修改角色任务失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYCHARACTERMONEY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	
}
	
//修改角色金钱
void CPalAPI::Pal_ModifyCharacterMoney(int userByID,char * ServerName, char * ServerIP, char * szRoleName,int iType,int iCount)
{
	try
	{
		CModifyCharacterMoney MCM;//实例化修改角色金钱对象	
		vector <CGlobalStruct::TFLV> DBTFLV=MCM.ModifyCharacterMoneyMain(ServerName,ServerIP,szRoleName,iType,iCount);//调用修改角色金钱接口
		operPal.BuildTLV((COperVector)DBTFLV,0,0);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYCHARACTERMONEY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYCHARACTERMONEY,ServerName,&(COperVector(DBTFLV)),"");//记录操作日志

	}
	catch(...)
	{
		operPal.BuildTLV("修改角色金钱失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYCHARACTERMONEY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	
}

//修改角色删除密码
void CPalAPI::Pal_ModifyCharacterDropPw(int userByID,char * ServerName, char * ServerIP, char * szRoleName,char * szDropRolePw)
{
	CModifyCharacterDropPw MCDP;//实例化修改角色删除密码对象

	char strlog[80];
	ZeroMemory(strlog,80);
	sprintf(strlog,"大区<%s>修改角色<%s>删除密码",ServerName,szRoleName);
	logFile.WriteText("仙剑OL",strlog);
	bool b_result=MCDP.FindLocalCharacterDropPw(szRoleName);//验证角色删除密码是否被修改过
	if(!b_result)
	{
		logFile.WriteText("仙剑OL","PAL_MODIFYCHARACTERDROPPW\n");
		char szOldPasswd[60];
		ZeroMemory(szOldPasswd,60);
		MCDP.GetDropPw(szRoleName,szOldPasswd,ServerName);//从本地数据库中获取角色删除密码
		
		CBanishPlayer BP;//实例化踢人对象
		BP.BanishPlayerMain(ServerName,ServerIP,szRoleName);//调用踢人接口

		
		vector <CGlobalStruct::TFLV> DBTFLV=MCDP.ModifyCharacterDropPwMain(ServerName,ServerIP,szRoleName,szDropRolePw);//调用修改角色删除密码接口
		operPal.BuildTLV((COperVector)DBTFLV,0,0);//构建数据包
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYCHARACTERDROPPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYCHARACTERDROPPW,ServerName,&(COperVector(DBTFLV)),"");//记录操作日志
		CGlobalStruct::TFLV m_tflv=DBTFLV.front();//指向vector的开始处
		char *pdest=strstr((char *)&m_tflv.lpdata,"成功");//在返回的vector中寻找是否存在'成功'
		if(pdest!=NULL)
		{
			b_result=MCDP.SaveCharacterDropPw(szRoleName,szOldPasswd,szDropRolePw,ServerName);//在本地数据库中保存修改之前的密码，同时做好标记
		}
	}
	else
	{
		operPal.BuildTLV("密码已修改,请恢复");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYCHARACTERDROPPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
}
//还原角色删除密码
void CPalAPI::Pal_RecallCharacterDropPw(int userByID,char * ServerName, char * ServerIP, char * szRoleName)
{
	try
	{
		char szDropRolePw[16];
		ZeroMemory(szDropRolePw,16);


		char strlog[80];
		ZeroMemory(strlog,80);
		sprintf(strlog,"大区<%s>还原角色<%s>删除密码",ServerName,szRoleName);
		logFile.WriteText("仙剑OL",strlog);

		CModifyCharacterDropPw MCDP;//
		bool b_result=false;
		b_result=MCDP.RecallCharacterDropPw(szRoleName,szDropRolePw);//判断角色删除密码是否被恢复过

		
		if(b_result)
		{
			CBanishPlayer BP;//实例化踢人对象
			BP.BanishPlayerMain(ServerName,ServerIP,szRoleName);//调用踢人接口

			vector <CGlobalStruct::TFLV> DBTFLV=MCDP.ModifyCharacterDropPwMain(ServerName,ServerIP,szRoleName,szDropRolePw);//调用修改角色删除密码接口
			operPal.BuildTLV((COperVector)DBTFLV,0,0);
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLCHARACTERDROPPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
			operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_RECALLCHARACTERDROPPW,ServerName,&(COperVector(DBTFLV)),"");//记录操作日志
		}
		else
		{
			operPal.BuildTLV("密码未重置");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLCHARACTERDROPPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}

	}
	catch(...)
	{
		operPal.BuildTLV("密码未重置");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLCHARACTERDROPPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
}

//修改帐号银行密码	
void CPalAPI::Pal_ModifyAccountBankPw(int userByID,char * ServerName, char * ServerIP, char * szAccount,char * szBankPw)
{
	char strlog[60];
	ZeroMemory(strlog,60);
	sprintf(strlog,"大区<%s>修改帐号<%s>银行密码",ServerName,szAccount);
	logFile.WriteText("仙剑OL",strlog);
	bool b_result=false;
	CModifyAccountBankPw MABP;//实例化修改帐号银行密码对象
	b_result=MABP.FindLocalAccountBankPw(szAccount);//在本地数据库中验证密码是否被修改过
	if(!b_result)
	{
		char szOldPasswd[60];
		ZeroMemory(szOldPasswd,60);
		MABP.GetBankPw(szAccount,szOldPasswd,ServerName);//从游戏数据库中获取帐号银行密码
		
		CBanishPlayer BP;//实例化踢人对象
		S_ROLENAME* s_rolename=new S_ROLENAME[10];//分配结构体指针
		int rolenum=0;
		/////int iIndex=0,iPageSize=0;
		int i=0;
		rolenum=operPal.AccountToRoleName(s_rolename,ServerName,szAccount);//根据帐号获得角色名
		for(i=0;i<rolenum;i++)
		{
			BP.BanishPlayerMain(ServerName,ServerIP,s_rolename[i].rolename);//调用踢人接口
		}
		
		delete s_rolename;
		s_rolename=NULL;


		vector <CGlobalStruct::TFLV> DBTFLV=MABP.ModifyAccountBankPwMain(ServerName,ServerIP,szAccount,szBankPw);//调用修改帐号银行密码接口
		operPal.BuildTLV((COperVector)DBTFLV,0,0);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYACCOUNTBANKPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTBANKPW,ServerName,&(COperVector(DBTFLV)),szBankPw);//记录操作日志
		CGlobalStruct::TFLV m_tflv=DBTFLV.front();//指向vector的开始处
		char *pdest=strstr((char *)&m_tflv.lpdata,"成功");
		if(pdest!=NULL)
		{
			MABP.SaveAccountBankPw(szAccount,szOldPasswd,szBankPw,ServerName);//如果本地数据库中没有相关记录，则在本地数据库中插入记录
		}
	}
	else
	{
		operPal.BuildTLV("密码已修改,请恢复");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYACCOUNTBANKPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
}

//还原帐号银行密码	
void CPalAPI::Pal_RecallAccountBankPw(int userByID,char * ServerName, char * ServerIP, char * szAccount)
{
	try
	{
		char szBankPw[7];
		ZeroMemory(szBankPw,7);
	

		char strlog[60];
		ZeroMemory(strlog,60);
		sprintf(strlog,"大区<%s>还原帐号<%s>银行密码",ServerName,szAccount);
		logFile.WriteText("仙剑OL",strlog);

		CModifyAccountBankPw MABP;//实例化修改帐号银行密码对象
		bool b_result=false;
		b_result=MABP.RecallAccountBankPw(szAccount,szBankPw);//在本地数据库中查找是否有修改过的密码,同时获得密码,修改记录的flag值为0（还原）
		
		
		if(b_result)
		{
			CBanishPlayer BP;//实例化踢人对象
			S_ROLENAME* s_rolename=new S_ROLENAME[10];//分配结构体指针
			int rolenum=0;
			////int iIndex=0,iPageSize=0;
			int i=0;
			rolenum=operPal.AccountToRoleName(s_rolename,ServerName,szAccount);//根据帐号获得角色名
			for(i=0;i<rolenum;i++)
			{
				BP.BanishPlayerMain(ServerName,ServerIP,s_rolename[i].rolename);//调用踢人接口
			}
			
			delete s_rolename;
			s_rolename=NULL;

			vector <CGlobalStruct::TFLV> DBTFLV=MABP.ModifyAccountBankPwMain(ServerName,ServerIP,szAccount,szBankPw);//调用修改帐号银行密码接口(还原)
			operPal.BuildTLV((COperVector)DBTFLV,0,0);
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLACCOUNTBANKPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
			operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTBANKPW,ServerName,&(COperVector(DBTFLV)),szBankPw);//记录操作日志
		}
		else
		{
			operPal.BuildTLV("密码未重置");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLACCOUNTBANKPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}
		
	}
	catch(...)
	{
		operPal.BuildTLV("密码未重置");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLACCOUNTBANKPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		
	}

}

//修改帐号密码
void CPalAPI::Pal_ModifyAccountPw(int userByID,char * ServerName, char * ServerIP, char * szAccount,char * szAccountPw)
{

	char text[512];
	char strlog[60];
	ZeroMemory(strlog,60);
	sprintf(strlog,"大区<%s>修改帐号<%s>密码",ServerName,szAccount);
	logFile.WriteText("仙剑OL",strlog);
	
	char accountdbip[30];
	ZeroMemory(strlog,30);
	operPal.FindAccountDBIP(accountdbip, ServerName);
	if (!strcmp(accountdbip, ""))
	{
		operPal.BuildTLV("查询数据库失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYACCOUNTPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		return;
	}
	
	CModifyAccountPw MAP;//实例化修改帐号密码对象
	int b_result = -1;
	char szMD5Password[50];
	ZeroMemory(szMD5Password,50);

	//判断本地数据库中是否有需要修改的帐号密码记录(flag=1),如果没有插入一条，同时把新密码进行md5加密
	b_result=MAP.SaveAccountPw(szAccount,szAccountPw,accountdbip,szMD5Password);
	if(b_result == 1)  //记录不存在
	{
					
		b_result=MAP.PutAccountPw(szAccount,szMD5Password,accountdbip);//调用修改帐号密码
		if(b_result)
		{
				sprintf(text,"帐号<%s>大区<%s>临时密码修改成功!新密码:%s",szAccount, accountdbip, szAccountPw);
				//Modified by tengjie 2010-07-08
//				operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPW,accountdbip,NULL,text);//记录操作日志
				operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPW,ServerName,NULL,text);//记录操作日志
				operPal.BuildTLV("临时密码修改成功");
				operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYACCOUNTPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}
		else
		{
				sprintf(text,"帐号<%s>大区<%s>临时密码修改失败!",szAccount, accountdbip);
				//Modified by tengjie 2010-07-08
//				operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPW,accountdbip,NULL,text);
				operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPW,ServerName,NULL,text);
				operPal.BuildTLV("临时密码修改失败");
				operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYACCOUNTPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}
	}
	else if(b_result == 0)  //记录存在
	{
		sprintf(text,"帐号<%s>大区<%s>临时密码已修改,请恢复!",szAccount, accountdbip);
		//Modified by tengjie 2010-07-08
//		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPW,accountdbip,NULL,text);
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPW,ServerName,NULL,text);
		operPal.BuildTLV("临时密码已修改,请恢复");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYACCOUNTPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	else if (b_result == -1)  //远程查询密码出错
	{
		sprintf(text,"帐号<%s>大区<%s>用户帐号不存在",szAccount, accountdbip);
		//Modified by tengjie 2010-07-08
//		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPW,accountdbip,NULL,text);
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPW,ServerName,NULL,text);
		operPal.BuildTLV("用户帐号不存在");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYACCOUNTPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
}

//还原帐号密码	
void CPalAPI::Pal_RecallAccountPw(int userByID,char * ServerName, char * ServerIP, char * szAccount)
{
	try
	{
		char szMD5Passwd[50];
		ZeroMemory(szMD5Passwd,50);
		char text[512];
		char strlog[60];
		ZeroMemory(strlog,60);
		sprintf(strlog,"大区<%s>还原帐号<%s>密码",ServerName,szAccount);
		logFile.WriteText("仙剑OL",strlog);

		char accountdbip[30];
		ZeroMemory(strlog,30);
		operPal.FindAccountDBIP(accountdbip, ServerName);
		if (!strcmp(accountdbip, ""))
		{
			operPal.BuildTLV("查询数据库失败");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYACCOUNTPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
			return;
		}
		
		CModifyAccountPw MAP;//实例化修改帐号密码对象
		bool b_result=false;
		b_result=MAP.RecallAccountPw(szAccount,szMD5Passwd, accountdbip);//在本地数据库中查找需要恢复的记录，获得原来密码，同时把flag置为0(还原)
	
		if(b_result)
		{
			b_result=MAP.PutAccountPw(szAccount,szMD5Passwd,accountdbip);//调用修改帐号密码
			if(b_result)
			{
				sprintf(text,"帐号<%s>大区<%s>还原密码成功",szAccount,accountdbip);
				//Modified by tengjie 2010-07-08
//				operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTPW,accountdbip,NULL,text);//记录操作日志
				operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTPW,ServerName,NULL,text);//记录操作日志
				operPal.BuildTLV("还原密码成功");
				operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLACCOUNTPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
			}
			else
			{
				sprintf(text,"帐号<%s>大区<%s>还原密码失败",szAccount,accountdbip);
				//Modified by tengjie 2010-07-08
//				operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTPW,accountdbip,NULL,text);
				operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTPW,ServerName,NULL,text);
				operPal.BuildTLV("还原密码失败");
				operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLACCOUNTPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
			}
		}
		else
		{
			operPal.BuildTLV("密码未修改");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLACCOUNTPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}
		
	}
	catch(...)
	{
		operPal.BuildTLV("还原密码失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLACCOUNTPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
}

//修改二级密码
void CPalAPI::Pal_ModifySecondPw(int userByID,char * ServerName,char * ServerIP,char * szAccount,char * szSePw)
{
	char text[256];
	char retMessage[256];
	char strlog[60];
	ZeroMemory(strlog,60);
	sprintf(strlog,"大区<%s>修改帐号<%s>的二级密码",ServerName,szAccount);
	logFile.WriteText("仙剑OL",strlog);
	bool b_result=false;
	char szMD5Password[50];
	ZeroMemory(szMD5Password,50);
	CModifySecLvlPwd mSLP;//实例化修改二级密码的类对象
	b_result=mSLP.SaveSecLvlPwd(szAccount,szSePw,ServerName,szMD5Password);//查询二级密码有没有被修改过
	if(b_result)
	{
		b_result=mSLP.PutSecLvlPwd(szAccount,szMD5Password,ServerName);//修改二级密码
		if(b_result)
		{
			sprintf(text,"修改帐号<%s>二级密码修改成功!新密码:%s",szAccount,szSePw);
		}
		else
		{
			sprintf(text,"修改帐号<%s>二级密码修改失败!新密码:%s",szAccount,szSePw);
			sprintf(retMessage,"%s","临时密码修改失败");
		}
	}
	else
	{
		sprintf(text,"帐号<%s>二级密码已修改,请恢复",szAccount);
		sprintf(retMessage,"%s","临时密码已修改,请恢复");
	}
	operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYSECONDPW,ServerName,NULL,text);///记录操作日志
	operPal.BuildTLV(retMessage);
	operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYSECONDPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());

}

//还原二级密码
void CPalAPI::Pal_RecallSecondPw(int userByID,char * ServerName,char * ServerIP,char * szAccount)
{
	try
	{
		char szMD5Passwd[50];
		ZeroMemory(szMD5Passwd,50);
		char text[256];
		char strlog[60];
		char retMessage[256];
		ZeroMemory(strlog,60);
		sprintf(strlog,"大区<%s>还原帐号<%s>的二级密码",ServerName,szAccount);
		logFile.WriteText("仙剑OL",strlog);

		CModifySecLvlPwd mSLP;//实例化修改二级密码的类对象
		bool b_result=false;
		b_result=mSLP.RecallSecLvlPwd(szAccount,szMD5Passwd);//在本地数据库中查找需要恢复的记录，获得原来密码，同时把flag置为0(还原)
		
		
		if(b_result)
		{
			b_result=mSLP.PutSecLvlPwd(szAccount,szMD5Passwd,ServerName);//修改二级密码
			if(b_result)
			{
				sprintf(text,"帐号<%s>还原密码成功",szAccount);
				sprintf(retMessage,"%s","还原密码成功");
			}
			else
			{
				sprintf(text,"帐号<%s>还原密码失败",szAccount);
				sprintf(retMessage,"%s","还原密码失败");
			}
		}
		else
		{
			sprintf(text,"%s","密码未重置");
			sprintf(retMessage,"%s","密码未重置");
		}
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_RECALLSECONDPW,ServerName,NULL,text);//记录操作日志
		operPal.BuildTLV(retMessage);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLSECONDPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());

		
	}
	catch(...)
	{
		operPal.BuildTLV("还原密码失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLSECONDPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
}


//锁定安全锁
void CPalAPI::Pal_UseSafeLock(int userByID,char * ServerName,char * ServerIP,char * szAccount)
{
	char text[256];
	char retMessage[256];
	char strlog[60];
	ZeroMemory(strlog,60);
	sprintf(strlog,"大区<%s>锁定帐号<%s>的安全锁",ServerName,szAccount);
	logFile.WriteText("仙剑OL",strlog);

	bool b_result=false;
	CModifySafeLock mSL;//实例化锁定安全锁对象
	b_result=mSL.GetSafeLock(szAccount,ServerName);//获得安全锁的状态,0解开，其他值锁定
	if(b_result)
	{
		b_result=mSL.LockSafeLock(szAccount,ServerName);//锁定安全锁(改变prg_lock = 100)
		if(b_result)
		{
			sprintf(text,"成功锁定帐号<%s>的安全锁",szAccount);
			sprintf(retMessage,"%s","锁定安全锁成功");
		}
		else
		{
			sprintf(text,"锁定帐号<%s>安全锁失败",szAccount);
			sprintf(retMessage,"%s","锁定安全锁失败");
		}
	}
	else
	{
		sprintf(text,"锁定帐号<%s>安全锁失败",szAccount);
		sprintf(retMessage,"%s","安全锁已被锁定");
	}
	operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_USESAFELOCK,ServerName,NULL,text);//记录操作日志
	operPal.BuildTLV(retMessage);
	operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_USESAFELOCK_RESP,operPal.GetSendBuf(),operPal.GetSendLength());

}

//解锁密码锁
void CPalAPI::Pal_FreeSafeLock(int userByID,char * ServerName,char * ServerIP,char * szAccount)
{
	try
	{
		char text[256];
		char retMessage[256];
		char strlog[60];
		ZeroMemory(strlog,60);
		sprintf(strlog,"大区<%s>解锁帐号<%s>安全锁",ServerName,szAccount);
		logFile.WriteText("仙剑OL",strlog);
		CModifySafeLock mSL;//实例化解密密码锁
		bool b_result=false;
		b_result=mSL.GetSafeLock(szAccount,ServerName);//获得安全锁的状态,0解开，其他值锁定
		
		if(!b_result)
		{
			b_result=mSL.UnLockSafeLock(szAccount,ServerName);//解密密码锁(改变prg_lock = 0)
			if(b_result)
			{
				sprintf(text,"帐号<%s>解锁密码锁成功",szAccount);
				sprintf(retMessage,"%s","解锁密码锁成功");
			}
			else
			{
				sprintf(text,"帐号<%s>解锁密码锁失败",szAccount);
				sprintf(retMessage,"%s","解锁密码锁失败");
			}
		}
		else
		{
			sprintf(text,"%s","密码锁未被锁定");
			sprintf(retMessage,"%s","密码锁未被锁定");
		}
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_FREESAFELOCK,ServerName,NULL,text);//记录操作日志
		operPal.BuildTLV(retMessage);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_FREESAFELOCK_RESP,operPal.GetSendBuf(),operPal.GetSendLength());

		
	}
	catch(...)
	{
		operPal.BuildTLV("解锁密码锁失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_FREESAFELOCK_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
}

// 删除角色物品响应
void CPalAPI::Pal_DelCharacterItem(int userByID,char * ServerName,char * ServerIP,char * RoleName,int iType,int iPosition,char * ItemName)
{
	try
	{
		CDelCharacterItem DCI;//实例化删除角色物品对象	
		vector<CGlobalStruct::TFLV> DBTFLV=DCI.DelCharacterItemMain(ServerName,ServerIP,RoleName,iType,iPosition);//调用删除角色物品接口
		operPal.BuildTLV((COperVector)DBTFLV,0,0);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_DELCHARACTERITEM_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_DELCHARACTERITEM,ServerName,&((COperVector)DBTFLV),ItemName);//记录操作日志
	}
	catch(...)
	{
		operPal.BuildTLV("删除道具失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_DELCHARACTERITEM_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	
}

//修改公会等级
void CPalAPI::Pal_ModifyGuildLv(int userByID,char * ServerName,char * ServerIP, char * GuildName,int iLevel,char * Reason)
{
	try
	{
		CModifyGuildLv MGL;	//实例化修改工会等级对象
		vector<CGlobalStruct::TFLV> DBTFLV=MGL.ModifyGuildLvMain(ServerName,ServerIP,GuildName,iLevel);//调用修改工会等级接口
		operPal.BuildTLV((COperVector)DBTFLV,0,0);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYGUILDLV_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYGUILDLV,ServerName,&((COperVector)DBTFLV),Reason);//记录操作日志

	}
	catch(...)
	{
		operPal.BuildTLV("修改公会等级失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYGUILDLV_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	

}

//修改公会公告
void CPalAPI::Pal_ModifyGuildNotice(int userByID,char * ServerName, char * ServerIP, char * GuildName,char * szNotice,char * Reason)
{
	try
	{
		CModifyGuildNotice MGN;//实例化修改工会公告对象
		vector<CGlobalStruct::TFLV> DBTFLV=MGN.ModifyGuildNoticeMain(ServerName,ServerIP,GuildName,szNotice);//调用修改工会公告接口
		operPal.BuildTLV((COperVector)DBTFLV,0,0);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYGUILDNOTICE_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYGUILDNOTICE,ServerName,&((COperVector)DBTFLV),Reason);//记录操作日志
	}
	catch(...)
	{
		operPal.BuildTLV("修改公会公告失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYGUILDNOTICE_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	

}




//道具类别查询
void CPalAPI::Pal_ItemType_Query()
{
	try
	{
		char strlog[60];
		ZeroMemory(strlog,60);
		sprintf(strlog,"道具类别查询");
		logFile.WriteText("仙剑OL",strlog);
		
		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("PAL","127.0.0.1",4,0,0,"exec Pal_ItemType_Query");
		if(!DBTFLV.empty())
		{
			operPal.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operPal.BuildTLV("道具类别查询失败");
		}
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ITEMTYPE_QUERY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	catch(...)
	{	
		operPal.BuildTLV("道具类别查询失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ITEMTYPE_QUERY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
}

//道具名称查询
void CPalAPI::Pal_ItemName_Query(int iItemBigType)
{
	try
	{
		char strlog[60];
		ZeroMemory(strlog,60);
		sprintf(strlog,"道具名称查询");
		logFile.WriteText("仙剑OL",strlog);

		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("PAL","127.0.0.1",4,0,0,"exec  pal_itemname_query %i",iItemBigType);
		if(!DBTFLV.empty())
		{
			operPal.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operPal.BuildTLV("道具名称查询失败");
		}
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ITEMNAME_QUERY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	
	}
	catch(...)
	{
		operPal.BuildTLV("道具名称查询失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ITEMNAME_QUERY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
}		

//发送仓库密码
void CPalAPI::PAL_SendBankPwd_Query(int userByID,char * ServerName, char * ServerIP,char * szAccount,char * mailBox)
{
	try
	{
		char strlog[60];
		
		sprintf(strlog,"发送仓库密码%s",szAccount);
		logFile.WriteText("仙剑OL",strlog);	

		char strPasswd[256];
		sprintf(strPasswd,"%06d",(int)rand()%100000);
		CModifyAccountBankPw MABP;//
		vector <CGlobalStruct::TFLV> DBTFLV=MABP.ModifyAccountBankPwMain(ServerName,ServerIP,szAccount,strPasswd);
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTBANKPW,ServerName,&(COperVector(DBTFLV)),strPasswd);
		
		//maple add
		char strMessage[128];
		ZeroMemory(strMessage,128);
		//operPal.ReadValue("PAL_Body",strMessage,1024);
		operPal.ReadValue("PAL_Body1",strMessage,128);
		char tmpcontext[512];
		ZeroMemory(tmpcontext,512);
		char context[512];
		ZeroMemory(context,512);

		sprintf(tmpcontext,"您的仙剑ONLINE仓库锁密码是：%s",strPasswd);

		//sprintf(context,strMessage,szAccount,tmpcontext);//maple update
		//if(operPal.SendEmail(mailBox,context))
		sprintf(context,strMessage,szAccount);
		if(operPal.SendEmail(mailBox,context,tmpcontext))
		{
			operPal.BuildTLV("发送密码成功");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_SendBankPwd_Query_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}
		else
		{
			operPal.BuildTLV("发送密码失败");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_SendBankPwd_Query_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}

	}
	catch(...)
	{
		operPal.BuildTLV("发送密码失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_SendBankPwd_Query_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
}

//发送角色删除密码
void CPalAPI::PAL_SendDeletePwd_Query(int userByID,char * ServerName, char * ServerIP, char * szRoleName,char * mailBox)
{
	try
	{
		char strlog[60];
		
		sprintf(strlog,"发送角色%s删除密码",szRoleName);
		logFile.WriteText("仙剑OL",strlog);	
		
		char strPasswd[256];
		sprintf(strPasswd,"%06d",(int)rand()%100000);
		
		CModifyCharacterDropPw MCDP;

		vector <CGlobalStruct::TFLV> DBTFLV=MCDP.ModifyCharacterDropPwMain(ServerName,ServerIP,szRoleName,strPasswd);
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYCHARACTERDROPPW,ServerName,&(COperVector(DBTFLV)),strPasswd);//记录操作日志
		
		//maple add
		char strMessage[128];
		ZeroMemory(strMessage,128);
		//operPal.ReadValue("PAL_Body",strMessage,1024);
		operPal.ReadValue("PAL_Body1",strMessage,128);
		char tmpcontext[512];
		ZeroMemory(tmpcontext,512);
		char context[512];
		ZeroMemory(context,512);
		
		sprintf(tmpcontext,"您的仙剑ONLINE角色删除密码是：%s",strPasswd);
		//sprintf(context,strMessage,szAccount,tmpcontext);//maple update
		//if(operPal.SendEmail(mailBox,context))
		sprintf(context,strMessage,szRoleName);
		if(operPal.SendEmail(mailBox,context,tmpcontext))
		{
			operPal.BuildTLV("发送密码成功");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_SendDeletePwd_Query_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}
		else
		{
			operPal.BuildTLV("发送密码失败");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_SendDeletePwd_Query_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}
	}
	catch(...)
	{
		operPal.BuildTLV("发送密码失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_SendDeletePwd_Query_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
}
//c#版本中，c++中没使用
//获取用户地址
bool CPalAPI::Pal_Get_EmailAddress(char * szAccount)
{
	try
	{
		
		char strsign[1024];
		char strMD5sign[1024];
		int iTypeID=1;
		
		
		sprintf(strsign,"%s%s",szAccount,"QUXUEXIBA");
		
		int newlength=0;
		ZeroMemory(strMD5sign,1024);
		MD5_EnCode(strMD5sign,&newlength,strsign,strlen(strsign));
		
		char url[256];
		
		
		sprintf(url,"http://222.73.121.86:8080/user?req=getUserInfo&id=GMTOOLS&userid=%s&usertype=0&type=userinfo&s=%s",szAccount,strMD5sign);//商城IP固定
		
		
		vector<CGlobalStruct::DataRecord> ret_vector;
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;
		int iCow=0,iCol=0;
		
		if(operPal.ReadXmlGet("Get",url,"",&ret_vector,&iCow,&iCol))
		{
			COperVector sendopervector;
			sendopervector.InitCreate(iCow+1,iCol+3);
			
			CEnumCore::TagName m_TagName;//保存获得的TagName
			CEnumCore::TagFormat m_TagFormat;//保存获得的TagFormat
			
			CMyEvent m_Event;//获取CEvent对象
			
			//获取查询的记录集的每一项
			
			for( iter = ret_vector.begin(); iter != ret_vector.end(); iter++ )
			{
					m_DataRecord = *iter;

					m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//获取记录集数据的TagName
					m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//获取记录集数据的TagFormat
					sendopervector.AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//构造TFLV,并添加到vector中

				
			}
			
			operPal.BuildTLV(sendopervector,0,0);
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MAILBOX_QUERY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
			sendopervector.Destroy();
			
		}
		else
		{
			iter = ret_vector.begin();
			m_DataRecord = *iter;
			operPal.BuildTLV(m_DataRecord.recorddata);
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MAILBOX_QUERY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}
		
		
		if(!ret_vector.empty())
		{
			ret_vector.clear();
		}
		
		
	}
	catch (...)
	{

		operPal.BuildTLV("获取E-Mail失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MAILBOX_QUERY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}


//取得角色进阶资讯
bool CPalAPI::Pal_CharacterAdvanceInfo(char * ServerName, char * ServerIP,char * RoleName,int iIndex,int iPageSize)
{
	try
	{
		CCharacterAdvanceInfo CAI;//实例化角色进阶信息对象
		vector <CGlobalStruct::TFLV> DBTFLV=CAI.CharacterAdvanceInfoMain(ServerName,ServerIP,RoleName);//调用角色进阶信息接口
		operPal.BuildTLV(COperVector(DBTFLV),iIndex,iPageSize);//构造数据包
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERADVANCEINFO_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	catch(...)
	{
		operPal.BuildTLV("进阶为空为空");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERADVANCEINFO_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}

//取得角色技能资讯
bool CPalAPI::Pal_CharacterSkill(char * ServerName, char * ServerIP,char * RoleName,int iIndex,int iPageSize)
{
	try
	{
		CCharacterSkill CS;//实例化角色技能信息对象
		vector <CGlobalStruct::TFLV> DBTFLV=CS.CharacterSkillMain(ServerName,ServerIP,RoleName);//调用角色技能信息接口
		operPal.BuildTLV(COperVector(DBTFLV),iIndex,iPageSize);//构造数据包
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERSKILL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());

		
	}
	catch(...)
	{
		operPal.BuildTLV("技能为空");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERSKILL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}
//取得角色伏魔资讯
bool CPalAPI::Pal_CharacterMob(char * ServerName, char * ServerIP,char * RoleName,int iIndex,int iPageSize)
{
	try
	{
		CCharacterMob CM;//实例化角色伏魔信息对象
		vector <CGlobalStruct::TFLV> DBTFLV=CM.CharacterMobMain(ServerName,ServerIP,RoleName);//调用角色伏魔信息接口
		operPal.BuildTLV(COperVector(DBTFLV),iIndex,iPageSize);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERMOB_RESP,operPal.GetSendBuf(),operPal.GetSendLength());

		
	}
	catch(...)
	{
		operPal.BuildTLV("伏魔为空");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERMOB_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}

//取得角色图鉴资讯
bool CPalAPI::Pal_CharacterPuzzleMap(char * ServerName, char * ServerIP,char * RoleName,int iIndex,int iPageSize)
{
	try
	{
		CCharacterPuzzleMap CPM;//实例化角色图鉴信息对象
		vector <CGlobalStruct::TFLV> DBTFLV=CPM.CharacterPuzzleMapMain(ServerName,ServerIP,RoleName);//调用角色图鉴信息接口
		operPal.BuildTLV(COperVector(DBTFLV),iIndex,iPageSize);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERPUZZLEMAP_RESP,operPal.GetSendBuf(),operPal.GetSendLength());

		
	}
	catch(...)
	{
		operPal.BuildTLV("图鉴为空");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERPUZZLEMAP_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}

//取得角色黑名单列表
bool CPalAPI::Pal_CharacterBlackList(char * ServerName, char * ServerIP, char * RoleName,int iIndex,int iPageSize)
{
	try
	{
		CCharacterBlackList CBL;//实例化角色黑名单对象
		vector <CGlobalStruct::TFLV> DBTFLV=CBL.CharacterBlackListMain(ServerName,ServerIP,RoleName);//调用角色黑名单信息接口
		operPal.BuildTLV(COperVector(DBTFLV),iIndex,iPageSize);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERBLACKLIST_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	
	}
	catch(...)
	{
		operPal.BuildTLV("黑名单为空");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERBLACKLIST_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	
	}
	return true;
}

//删除角色伏魔
bool CPalAPI::Pal_DelCharacterMob(int userByID,char * ServerName, char * ServerIP,char * RoleName,int iMobID)
{
	try
	{
		CDelCharacterMob DCM;//实例化删除角色伏魔对象
		vector<CGlobalStruct::TFLV> DBTFLV=DCM.DelCharacterMobMain(ServerName,ServerIP,RoleName,iMobID);//调用删除角色伏魔接口
		operPal.BuildTLV((COperVector)DBTFLV,0,0);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_DELCHARACTERMOB_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_DELCHARACTERMOB,ServerName,&(COperVector(DBTFLV)),"");//记录操作日志
		
	}
	catch(...)
	{
		operPal.BuildTLV("删除角色伏魔失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_DELCHARACTERMOB_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}

//修改角色图鉴
bool CPalAPI::Pal_ModifyPuzzleMap(int userByID,char * ServerName,char * ServerIP,char * RoleName,char *strPuzzleMap,
								  int iMapID1,int iMapID2,int iMapID3,int iMapID4,int iMapID5,int iMapID6,int iMapID7,int iMapID8,int iMapID9)
{
	try
	{
		CModifyPuzzleMap MPM;//实例化修改角色图鉴对象
		//调用修改角色图鉴接口
		vector<CGlobalStruct::TFLV> DBTFLV=MPM.ModifyPuzzleMapMain(ServerName,ServerIP,RoleName,strPuzzleMap,iMapID1,iMapID2,iMapID3,iMapID4,iMapID5,iMapID6,iMapID7,iMapID8,iMapID9);
		operPal.BuildTLV((COperVector)DBTFLV,0,0);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYPUZZLEMAP_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYPUZZLEMAP,ServerName,&(COperVector(DBTFLV)),"");//记录操作日志
	}
	catch(...)
	{
		operPal.BuildTLV("修改角色图鉴失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYPUZZLEMAP_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}

//取得帐号资讯
bool CPalAPI::Pal_AccountInfo(char * ServerName, char * ServerIP, char * szAccountName, int iIndex, int iPageSize)
{
	try
	{
		CAccountInfo AI;//实例化帐号信息对象
		vector <CGlobalStruct::TFLV> DBTFLV=AI.AccountInfoMain(ServerName,ServerIP,szAccountName);//调用帐号信息接口
		operPal.BuildTLV(COperVector(DBTFLV),iIndex,iPageSize);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ACCOUNTINFO_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	
	}
	catch(...)
	{
		operPal.BuildTLV("帐号资讯为空");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ACCOUNTINFO_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}

//移动角色至安全点
bool CPalAPI::Pal_MoveCharacterToSafe(int userByID,char * ServerName, char * ServerIP, char * RoleName, int iZoneID)
{
	try
	{
		CMoveCharacterToSafe MCTS;//实例化移动角色至安全点对象
		vector<CGlobalStruct::TFLV> DBTFLV=MCTS.MoveCharacterToSafeMain(ServerName,ServerIP,RoleName,iZoneID);//调用移动角色至安全点接口
		operPal.BuildTLV((COperVector)DBTFLV,0,0);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MOVECHARACTERTOSAFE_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MOVECHARACTERTOSAFE,ServerName,&(COperVector(DBTFLV)),"");//记录操作日志
	}
	catch(...)
	{
		operPal.BuildTLV("移动角色至安全点失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MOVECHARACTERTOSAFE_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}

//重新载入功能开关
bool CPalAPI::Pal_ReloadFunctionSwitch()
{
	try
	{
	//	CReloadFunctionSwitch RFS;
		//RFS.ReloadFunctionSwitchMain(m_socket,&recvVect,operPal.pPalSession);
		
	}
	catch(...)
	{
	}
	return true;
}
//查询地图
bool CPalAPI::PAL_SearchMap_Query()
{
	try
	{
		char strlog[60];
		ZeroMemory(strlog,60);
		sprintf(strlog,"查询地图");
		logFile.WriteText("仙剑OL",strlog);



		vector <CGlobalStruct::TFLV> DBTFLV=GameDBQuery("PAL","127.0.0.1",4,0,0,"exec Pal_ServerMap_Query");
		if(!DBTFLV.empty())
		{
			operPal.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operPal.BuildTLV("数据库为空");			
		}
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_SearchMap_Query_Resp,operPal.GetSendBuf(),operPal.GetSendLength());


	}
	catch (...)
	{
		
	}
	return true;
}

//仙剑踢人按用户角色踢, 操作时可以踢任一个角色都可以把帐号踢下线, 角色在线不在线都行
void CPalAPI::Pal_BanishPlayer(char * pal_serverip,char * pal_gmserveripname,char * pal_userNick)
{
	try
	{
		CBanishPlayer BP;
		/*
		S_ROLENAME* s_rolename=new S_ROLENAME[10];
		int rolenum=0;
		int i=0;
		rolenum=operPal.AccountToRoleName(s_rolename,pal_gmserveripname,pal_account);
		for(i=0;i<rolenum;i++)
		{
			BP.BanishPlayerMain(pal_gmserveripname,pal_serverip,s_rolename[i].rolename);
		}
		
		delete s_rolename;
		s_rolename=NULL;*/

		BP.BanishPlayerMain(pal_gmserveripname,pal_serverip,pal_userNick);
	}
	catch(...)
	{

	}

	return;
}


bool CPalAPI::UserInfo(char * ServerName, char * ServerIP,char * UserName, char * UserNick,int iIndex,int iPageSize)
{
	try
	{
		if(!strcmp(UserNick,""))//角色名为空
		{
			char strlog[128];
			ZeroMemory(strlog,128);
			sprintf(strlog,"大区<%s>账号<%s>查询角色基本信息",ServerName,UserName);
			logFile.WriteLog("仙剑",operName,strlog);//记LOG
			CCharacterInfo CI;
			int rolenum=0;
			int i=0,j=0;
			S_ROLENAME* s_rolename=new S_ROLENAME[10];
			rolenum=operPal.AccountToRoleName(s_rolename,ServerName,UserName);//该帐号下的角色
			if(rolenum==0)
			{
				if(s_rolename!=NULL)
				{
					delete[] s_rolename;
					s_rolename=NULL;
				}
				operPal.BuildTLV("没有该账号");
				operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERINFO_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
			}
			else
			{
				vector <CGlobalStruct::TFLV> * pOperVector=new vector <CGlobalStruct::TFLV>[rolenum];
				CGlobalStruct::TFLV m_tflv;
				for(i=0;i<rolenum;i++)
				{
					pOperVector[i]=CI.CharacterInfoMain(ServerName,ServerIP,s_rolename[i].rolename);
					char strInt[10];
					ZeroMemory(strInt,10);
					
					//////添加元宝数量
					int point=0;
					operPal.GetPoint(ServerName,UserName,&point);
					
					itoa(point,strInt,10);
					
					m_tflv.nIndex=pOperVector[i].size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PAL_POINT;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=4;
					sprintf((char*)&m_tflv.lpdata,"%s",(unsigned char *)&strInt);
					pOperVector[i].push_back(m_tflv);
				}
				vector <CGlobalStruct::TFLV> DBTFLV;
				for(i=0;i<rolenum;i++)
				{
					vector<CGlobalStruct::TFLV>::iterator iter;
					for( iter = pOperVector[i].begin(); iter != pOperVector[i].end(); iter++ )
					{
						m_tflv = *iter;
						m_tflv.nIndex=DBTFLV.size()+1;
						DBTFLV.push_back(m_tflv);
					}
				}
				operPal.BuildTLV(COperVector(DBTFLV),iIndex,iPageSize);
				operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,
					CEnumCore::ServiceKey::PAL_CHARACTERINFO_RESP,
					operPal.GetSendBuf(),operPal.GetSendLength());
				for(i=0;i<rolenum;i++)
				{
					if(!pOperVector[i].empty())
					{
						pOperVector[i].clear();
					}
				}
				delete[] pOperVector;
				pOperVector=NULL;
			}
			if(s_rolename!=NULL)
			{
				delete[] s_rolename;
				s_rolename=NULL;
			}
		}
		else
		{
			
			CCharacterInfo CI;
			vector <CGlobalStruct::TFLV> DBTFLV=CI.CharacterInfoMain(ServerName,ServerIP,UserNick);
			
			char strInt[10];
			ZeroMemory(strInt,10);
			CGlobalStruct::TFLV m_tflv;
			//////添加元宝数量
			int point=0;
			operPal.GetPoint(ServerName,
				(char *)&(COperVector(DBTFLV).getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0).lpdata),
				&point);
			
			itoa(point,strInt,10);
			m_tflv.nIndex=DBTFLV.size()+1;	
			m_tflv.m_tagName=CEnumCore::TagName::PAL_POINT;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
			m_tflv.m_tvlength=4;
			sprintf((char*)&m_tflv.lpdata,"%s",(unsigned char *)&strInt);
			
			DBTFLV.push_back(m_tflv);
			operPal.BuildTLV(COperVector(DBTFLV),iIndex,iPageSize);
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,
				CEnumCore::ServiceKey::PAL_CHARACTERINFO_RESP,
				operPal.GetSendBuf(),operPal.GetSendLength());
			
		}
	}
	catch(...)
	{
		operPal.BuildTLV("查询用户信息失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERINFO_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		
	}
	return true;
}
//根据帐号踢人
bool CPalAPI::Pal_KickPlayer(int userByID,char * ServerName, char * ServerIP,char * UserName)
{
	CBanishPlayer BP;//实例化踢人对象
	vector<CGlobalStruct::TFLV> DBTFLV=BP.BanishPlayerMain(ServerName,ServerIP,UserName);//调用踢人接口(角色名)
	operPal.BuildTLV((COperVector)DBTFLV,0,0);
	operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_BANISHPLAYER_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	char text[256];
	ZeroMemory(text,256);
	sprintf(text,"服务器名称:%s",ServerName);
	operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_BANISHPLAYER,ServerName,&(COperVector(DBTFLV)),text);//记录操作日志
	return TRUE;
}

//根据帐号封停
bool CPalAPI::Pal_ClosePlayer(int userByID,char * ServerName, char * ServerIP ,char * UserName,char * UserNick, char* Reason)
{
	try
	{

		char strlog[80];
		char accountdbip[60];
		char szRemoteSql[2048];
		char retMessage[256];

		ZeroMemory(strlog,80);
		sprintf(strlog,"大区<%s>封停的玩家帐号<%s>",ServerName,UserName);
		logFile.WriteText("仙剑OL",strlog);
		
		CBanishPlayer BP;//实例化踢人对象
		S_ROLENAME* s_rolename=new S_ROLENAME[10];//分配结构体指针
		int rolenum=0;
		int iIndex=0,iPageSize=0;
		int i=0;
		rolenum=operPal.AccountToRoleName(s_rolename,ServerName,UserName);//根据帐号获得角色名
// 		for(i=0;i<rolenum;i++)
// 		{
// 			BP.BanishPlayerMain(ServerName,ServerIP,s_rolename[i].rolename);//调用踢人接口
// 		}

		// 帐号下角色代替帐号踢人		
		BP.BanishPlayerMain(ServerName,ServerIP,s_rolename[0].rolename);//调用踢人接口		
		delete [] s_rolename;
		s_rolename=NULL;	

		
		operPal.FindAccountDBIP(accountdbip,ServerName);
		operPal.getRemoteSql("PAL","PAL_ACCOUNT_CLOSE",szRemoteSql,0);


		///////远程操作
		int length=0;
		length=GameDBOper("PAL",accountdbip,0,szRemoteSql,UserName);
		if(length>0)//远程数据库操作成功
		{
			length=GameDBOper("PAL","127.0.0.1",4,"exec Pal_Banishment_Close %i,'%s','%s','%s','%s'",
				userByID,ServerName,accountdbip,UserName,Reason);//本地数据库插入记录
			if(length>0)
			{
				//Added by tengjie 2010-08-02
				UserLogInterface(ServerName, UserName, "", 31, "1");

				sprintf(retMessage,"%s","封停成功");
			}
			else
			{
				sprintf(retMessage,"%s","封停失败,本地操作失败");
			}
		}
		else
		{
			sprintf(retMessage,"%s","封停失败");
		}
		operPal.BuildTLV(retMessage);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ACCOUNT_CLOSE_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		//logFile.WriteDBLog(userByID,"pal","PAL_ACCOUNT_CLOSE",ServerIP,retMessage);//maple add//Reason
	}
	catch (...)
	{
		operPal.BuildTLV("封停失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ACCOUNT_CLOSE_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}

//maple add
//批量封停玩家帐号
bool CPalAPI::Pal_BanMultiPlayer(int userByID,char * ServerName, char * ServerIP ,char * UserNameList,char * RoleNameList, char* Reason)
{
	try
	{
		char strlog[80];
		char accountdbip[60];
		char szRemoteSql[256];
		char retMessage[2048];
		char tmpMessage[2048];
		
		ZeroMemory(strlog,80);
		ZeroMemory(accountdbip,60);
		ZeroMemory(szRemoteSql,256);
		ZeroMemory(retMessage,2048);
		ZeroMemory(tmpMessage,2048);
		
		sprintf(strlog,"大区<%s>封停的玩家帐号",ServerName);
		logFile.WriteText("仙剑OL",strlog);
		
		vector <BanPlayerPACKET> v_banplayer;
		char * pUserName= strtok(UserNameList,"|");
		BanPlayerPACKET banplayerPacket;
		while(pUserName!=NULL)///账号
		{
			sprintf(banplayerPacket.szAccount,"%s",pUserName);
			v_banplayer.push_back(banplayerPacket);
			pUserName= strtok(NULL,"|");
		}
		
		vector<BanPlayerPACKET>::iterator iter;
		//char RoleName[128];
		//ZeroMemory(RoleName,128);
		char* pRole=strtok(RoleNameList,"|");
		for( iter = v_banplayer.begin(); iter != v_banplayer.end(); iter++)
		{
			//sprintf(RoleName,"%s",pRole);
			//(*iter).szRoleName=RoleName;//角色名
			sprintf((*iter).szRoleName,"%s",pRole);//RoleName
			pRole= strtok(NULL,"|");
			if(pRole==NULL)
				break;
		}
		
		CBanishPlayer BP;//实例化踢人对象
		int length=0;
		operPal.FindAccountDBIP(accountdbip,ServerName);
		operPal.getRemoteSql("PAL","PAL_ACCOUNT_CLOSE",szRemoteSql,0);
		
		for (iter = v_banplayer.begin(); iter != v_banplayer.end(); iter++)//根据迭代器循环添加
		{
			banplayerPacket = *iter;
			BP.BanishPlayerMain(ServerName,ServerIP,banplayerPacket.szRoleName);//banplayerPacket.szRoleName);//调用踢人接口
			Sleep(100);
			
			//operPal.FindAccountDBIP(accountdbip,ServerName);
			//operPal.getRemoteSql("PAL","PAL_ACCOUNT_CLOSE",szRemoteSql,0);
			
			///////远程操作
			length=GameDBOper("PAL",accountdbip,0,szRemoteSql,banplayerPacket.szAccount);
			if(length>0)
			{
				length=GameDBOper("PAL","127.0.0.1",4,"exec Pal_Banishment_Close %i,'%s','%s','%s','%s'",
				userByID,ServerName,accountdbip,banplayerPacket.szAccount,Reason);//本地数据库插入记录

				if (length>0)
				{
					sprintf(tmpMessage,"封停帐号:%s角色名:%s成功;\r\n",banplayerPacket.szAccount,banplayerPacket.szRoleName);
					sprintf(retMessage,"%s%s",retMessage,tmpMessage);
				}
				else
				{
					sprintf(tmpMessage,"封停帐号:%s角色名:%s本地操作失败;\r\n",banplayerPacket.szAccount,banplayerPacket.szRoleName);
					sprintf(retMessage,"%s%s",retMessage,tmpMessage);
				}
			}
			else
			{
				sprintf(tmpMessage,"封停帐号:%s角色名:%s失败;\r\n",banplayerPacket.szAccount,banplayerPacket.szRoleName);
				sprintf(retMessage,"%s%s",retMessage,tmpMessage);
			}
			
		}
		operPal.BuildTLV(retMessage);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RELOADFUNCTIONSWITCH_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		//logFile.WriteDBLog(userByID,"pal","PAL_ACCOUNT_CLOSE",ServerIP,"批量封停玩家帐号");//retMessage);//maple add
		if (!v_banplayer.empty())
		{
			v_banplayer.clear();
		}
	}
	catch(...)
	{
		operPal.BuildTLV("批量封停失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RELOADFUNCTIONSWITCH_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}

//根据帐号解封
bool CPalAPI::Pal_OpenPlayer(int userByID,char * ServerName, char * ServerIP ,char * UserName,char * UserNick, char* Reason)
{
	try
	{
		char strlog[80];
		char accountdbip[60];
		char szRemoteSql[2048];
		char retMessage[256];
		ZeroMemory(strlog,60);
		sprintf(strlog,"大区<%s>解封玩家帐号<%s>",ServerName,UserName);
		logFile.WriteText("仙剑OL",strlog);
		
		int iState=operPal.FindAccountStatus(UserName);
		if(iState==1)
		{
			
			
			operPal.FindAccountDBIP(accountdbip,ServerName);//根据大区名获得帐号所在ip
			operPal.getRemoteSql("PAL","PAL_ACCOUNT_OPEN",szRemoteSql,0);
			
			
			///////远程操作
			int length=0;
			length=GameDBOper("PAL",accountdbip,0,szRemoteSql,UserName);//在远程数据库中解封帐号
			printf("length:%d++++++++accountdbip:%s, UserName:%s\n", length,accountdbip,UserName);
			if(length>0)
			{
				length=GameDBOper("PAL","127.0.0.1",4,"exec Pal_Banishment_OPEN %i,'%s','%s','%s','%s'",
					userByID,ServerName,accountdbip,UserName,Reason);//在本地数据库中解封玩家帐号
				if(length>0)
				{
					//Added by tengjie 2010-08-02
					UserLogInterface(ServerName, UserName, "", 31, "0");

					sprintf(retMessage,"%s","解封成功");
				}
				else
				{
					sprintf(retMessage,"%s","解封失败,本地操作失败");
				}
			}
			else
			{
				sprintf(retMessage,"%s","解封失败");
			}
			operPal.BuildTLV(retMessage);
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ACCOUNT_OPEN_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
			logFile.WriteDBLog(userByID,"pal","PAL_ACCOUNT_OPEN",Reason,retMessage);//maple add
		}
		else
		{
			operPal.BuildTLV("无封停信息");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ACCOUNT_OPEN_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}

	}
	catch(...)
	{
		operPal.BuildTLV("解封失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ACCOUNT_OPEN_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}

//查询封停列表
bool CPalAPI::CloseList(char * ServerName,char * ServerIP,int iIndex, int iPageSize)
{
	try
	{
		char strlog[60];
		char accoutdbip[60];
		sprintf(strlog,"大区<%s>封停所有帐号",ServerName);
		logFile.WriteText("仙剑OL",strlog);
		operPal.FindGameDBIP(accoutdbip,ServerName);//根据大区名获得大区ip

		//查找ip(以上获得的大区ip)上封停的玩家信息
		vector <CGlobalStruct::TFLV> DBTFLV=GameDBQuery("PAL","127.0.0.1",4,iIndex,iPageSize,"exec Pal_Banishment_ALLQuery '%s'",accoutdbip);
		if(!DBTFLV.empty())
		{
			operPal.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operPal.BuildTLV("数据库为空");			
		}
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ACCOUNTLOCAL_QUERY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	catch(...)
	{
		operPal.BuildTLV("查询本地封停失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ACCOUNTLOCAL_QUERY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}

	return true;
}

//查询单个账号
bool CPalAPI::CloseSingle(char * ServerName, char * ServerIP,char * UserName,int iIndex, int iPageSize)
{
	try
	{
		char strlog[80];
		ZeroMemory(strlog,80);
		sprintf(strlog,"大区<%s>查询玩家账号<%s>是否封停",ServerName,UserName);
		logFile.WriteText("仙剑OL",strlog);
		char accountdbip[60];
		operPal.FindGameDBIP(accountdbip,ServerName);//根据大区名获得大区ip

		////查找ip(以上获得的大区ip)上玩家帐号是否被封停
		vector <CGlobalStruct::TFLV> DBTFLV=GameDBQuery("PAL","127.0.0.1",4,iIndex,iPageSize,"exec  Pal_Banishment_Query '%s','%s'",accountdbip,UserName);
		if(!DBTFLV.empty())
		{
			operPal.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operPal.BuildTLV("数据库为空");			
		}
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ACCOUNT_BANISHMENT_QUERY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	catch(...)
	{
		operPal.BuildTLV("查询账号封停失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ACCOUNT_BANISHMENT_QUERY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}


//查询公告
bool CPalAPI::BroadTask_Query(int iIndex,int iPageSize)
{
	try
	{
		char strlog[60];
		ZeroMemory(strlog,60);
		sprintf(strlog,"查询公告");
		logFile.WriteText("仙剑OL",strlog);
		
		
		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("PAL", "127.0.0.1",4, iIndex, iPageSize,"exec Pal_BoardTask_NewQuery");
		if(!DBTFLV.empty())
		{
			operPal.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operPal.BuildTLV("数据库为空");			
		}
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_BOARDTASK_QUERY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());

		
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("PAL",(char *)err.what());
		return false;
	}
	catch (...)
	{
		operPal.BuildTLV("数据库为空");			
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_BOARDTASK_QUERY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		return TRUE;
		
	}
	return TRUE;
}

//更新公告
bool CPalAPI::BroadTask_Update(int userByID,int taskid,char * boardMessage,char * begintime,char * endtime,int interval,int status)
{
	try
	{
		char strlog[60];
		char retMessage[256];
		sprintf(strlog,"更新公告号%i",taskid);
		logFile.WriteText("仙剑OL",strlog);

		int length=GameDBOper("PAL","127.0.0.1",4,"exec Pal_TaskList_Update %i,'%s',%i,'%s','%s','%s',%i,%i",
									userByID,
									"",
									taskid,
									boardMessage,
									begintime,
									endtime,
									interval,
									status);	
		if(length>0)
		{
			sprintf(retMessage,"%s","公告更新成功");
		}
		else
		{
			sprintf(retMessage,"%s","公告更新失败");
		}
		operPal.BuildTLV(retMessage);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_BOARDTASK_UPDATE_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	catch(...)
	{
		operPal.BuildTLV("公告更新失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_BOARDTASK_UPDATE_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}

//插入公告
bool CPalAPI::BroadTask_Insert(int userByID,char * ServerIP,char * GSServerIP,char * boardMessage,char * begintime,char * endtime,int interval)
{
	try
	{
		char strlog[2048];
		char retMessage[256];
		sprintf(strlog,"大区<%s>大区IP<%s>添加公告<%s>",ServerIP,GSServerIP,boardMessage);
		logFile.WriteText("仙剑OL",strlog);

		int length=GameDBOper("PAL","127.0.0.1",4,"exec Pal_TaskList_Insert %i,'%s','%s','%s','%s','%s',%i,'%s'",
				userByID,
				"",
				GSServerIP,
				boardMessage,
				begintime,
				endtime,
				interval,
				ServerIP);	
		
		if(length>0)
		{
			sprintf(retMessage,"%s","公告添加成功");
		}
		else
		{
			sprintf(retMessage,"%s","公告添加失败");
		}
		operPal.BuildTLV(retMessage);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_BOARDTASK_INSERT_RESP,operPal.GetSendBuf(),operPal.GetSendLength());

		
	}
	catch(...)
	{
		operPal.BuildTLV("公告添加失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_BOARDTASK_INSERT_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}

// add by ltx
//寄送客制物品邮件
void CPalAPI::Pal_SendCustomItemMail(int UserByID, char * szGMServerName,char * szGMServerIP,char * szRoleName,char * szSubject,
						char * szContent,int iMoney, int itemType, int iCustomItemID, char *szCustomItemIDName, bool bBindSetup, char *szInherit,char *szReason)
{
	try
	{
		CSendCustomItemMail SendCIM;//实例化寄送奖品邮件
		vector<CGlobalStruct::TFLV> DBTFLV=SendCIM.SendCustomItemMailMain(szGMServerName, szGMServerIP, szRoleName, szSubject,
						szContent, iMoney,  itemType, iCustomItemID, szCustomItemIDName, bBindSetup, szInherit,szReason);//调用寄送奖品邮件接口
		operPal.BuildTLV((COperVector)DBTFLV,0,0);//构建数据包
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_SENDCUSTOMITEMMAIL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		char strCItemMailLog[255];

		sprintf(strCItemMailLog, "角色名<%s>,寄送物品名称:%s,金钱数:%d", szRoleName, szCustomItemIDName, iMoney);

 		operPal.RecordData(UserByID,CEnumCore::Message_Tag_ID::PAL_SENDCUSTOMITEMMAIL,szGMServerName,
 			&COperVector(DBTFLV),strCItemMailLog);//记录操作日志
	}
	catch(...)
	{
		operPal.BuildTLV("邮件发送失败");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_SENDCUSTOMITEMMAIL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		
	}
}

// add by ltx
void CPalAPI::Pal_ModifyAccountPwAll(int userByID, char * szAccount, char * szAccountPw, char *ServerName)
{
	try
	{
		vector<CGlobalStruct::DataRecord> list_accountdbip;

		char text[512];
		char strlog[60];
		ZeroMemory(text, 512);
		ZeroMemory(strlog, 60);
		
		CModifyAccountPw MAP;	//实例化修改帐号密码对象
		char szMD5Password[50];
		ZeroMemory(szMD5Password,50);
		int errorno = 0;

		CSqlHelper m_SqlHelper;
		char szSql[256];
		sprintf(szSql,"exec Pal_FindAccountdbipAll");//在本地数据库中查找是否有需要恢复的记录
		
		m_SqlHelper.SqlHelperMain("PAL",&list_accountdbip, "127.0.0.1", szSql, 4, 0, 0);//获取SqlExpress中的相应sql语句
		
		if(list_accountdbip.empty())//如果记录为空
		{
			operPal.BuildTLV("大区信息获取失败");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLACCOUNTPWALL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
			return;
		}

		for(int i=0; i<list_accountdbip.size(); ++i)
		{
			if(i%2 == 1)
			{
				continue;
			}

			char *accountdbip = (char *)list_accountdbip[i].recorddata;
			
			sprintf(strlog, "大区<%s>修改帐号<%s>密码", accountdbip, szAccount);
			logFile.WriteText("仙剑OL",strlog);
			
			int b_result = -1;

 			//判断本地数据库中是否有需要修改的帐号密码记录(flag=1),如果没有插入一条，同时把新密码进行md5加密
  			b_result=MAP.SaveAccountPw(szAccount,szAccountPw, accountdbip, szMD5Password);
  			if(b_result == 1) //记录不存在
  			{
  				
  				b_result=MAP.PutAccountPw(szAccount, szMD5Password, accountdbip);//调用修改帐号密码
  				if(b_result)
  				{
  					sprintf(text,"帐号<%s>大区<%s>临时密码修改成功!新密码:%s", szAccount, accountdbip, szAccountPw);
					//Modified by tengjie 2010-07-08
//					operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPWALL_RESP, accountdbip,NULL,text);//记录操作日志
					operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPWALL_RESP, ServerName,NULL,text);//记录操作日志
  				}
  				else
  				{
  					sprintf(text,"帐号<%s>大区<%s>临时密码修改失败!",szAccount, accountdbip);
					//Modified by tengjie 2010-07-08
//					operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPWALL_RESP, accountdbip, NULL,text);
					operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPWALL_RESP, ServerName, NULL,text);
  					errorno++;
  				}
  			}
 			else if(b_result == 0)	//记录已存在
 			{
 				sprintf(text,"帐号<%s>大区<%s>临时密码已经修改!",szAccount, accountdbip);
				//Modified by tengjie 2010-07-08
//				operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPWALL_RESP, accountdbip, NULL,text);
				operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPWALL_RESP, ServerName, NULL,text);
 			}
 			else if (b_result == -1)  //远程查询密码出错
 			{
 				sprintf(text,"帐号<%s>大区<%s>帐号信息不存在!",szAccount, accountdbip);
				printf("%s\n", text);
				//Modified by tengjie 2010-07-08
//				operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPWALL_RESP, accountdbip, NULL,text);
				operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPWALL_RESP, ServerName, NULL,text);
 			}
		}

		if (errorno == 0)
		{
			operPal.BuildTLV("临时密码修改成功");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYACCOUNTPWALL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}
		else
		{
			operPal.BuildTLV("部分密码修改失败");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYACCOUNTPWALL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}

	}
	catch(...)
	{
		operPal.BuildTLV("临时密码修改错误");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYACCOUNTPWALL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());		
	}

	return;
}

void CPalAPI::Pal_RecallAccountPwAll(int userByID, char * szAccount, char *ServerName)
{
	try
	{
		char szMD5Passwd[50];
		ZeroMemory(szMD5Passwd,50);
		char accountdbip[50];
		ZeroMemory(accountdbip,50);

		char text[512];
		char strlog[60];
		ZeroMemory(strlog,60);

		vector<CGlobalStruct::DataRecord> datarecord;
		CSqlHelper m_SqlHelper;
		char szSql[256];
		sprintf(szSql,"exec Pal_FindOldAccountPwAll '%s'",szAccount);//在本地数据库中查找是否有需要恢复的记录

		m_SqlHelper.SqlHelperMain("PAL",&datarecord, "127.0.0.1", szSql, 4, 0, 0);//获取SqlExpress中的相应sql语句
		
		if(datarecord.empty())//如果记录为空
		{
			operPal.BuildTLV("没有密码需要恢复");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLACCOUNTPWALL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
			return;
		}

		CModifyAccountPw MAP;//实例化修改帐号密码对象
		bool b_result=false;
		int errorno = 0;

		//获取查询的记录集的每一项
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("oldpasswd", m_DataRecord.recordfieldname))//获取SqlExpress中对应sql_type的sql_statement
			{
				sprintf(szMD5Passwd,m_DataRecord.recorddata);//如果记录集中有记录，获取记录的原来密码
			}
			else if (!strcmp("serverip", m_DataRecord.recordfieldname))
			{
				sprintf(accountdbip, m_DataRecord.recorddata);//如果记录集中有记录，获取密码对应大区				

				sprintf(strlog,"大区<%s>还原帐号<%s>密码",accountdbip, szAccount);
				logFile.WriteText("仙剑OL",strlog);
				
				b_result=MAP.PutAccountPw(szAccount, szMD5Passwd, accountdbip);//调用修改帐号密码
				if(b_result)
				{
					sprintf(text,"帐号<%s>大区<%s>还原密码成功",szAccount, accountdbip);
					//Modified by tengjie 2010-07-08
//					operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTPWALL_RESP,accountdbip,NULL,text);//记录操作日志	
					operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTPWALL_RESP,ServerName,NULL,text);//记录操作日志	
					
					sprintf(szSql,"exec Pal_UpdateAccountPwFlag '%s', '%s'",szAccount, accountdbip);//修改记录的flag值为0（还原）
					m_SqlHelper.SqlHelperMain("127.0.0.1", szSql, 4);//获取SqlExpress中的相应sql语句
				}
				else
				{
					sprintf(text,"帐号<%s>大区<%s>还原密码失败",szAccount, accountdbip);
					//Modified by tengjie 2010-07-08
//					operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTPWALL_RESP,accountdbip,NULL,text);
					operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTPWALL_RESP,ServerName,NULL,text);
					errorno++;
				}
			}
		}
		
		//有修改失败的
		if (errorno == 0)
		{
			operPal.BuildTLV("所有密码恢复成功");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLACCOUNTPWALL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}
		else
		{
			operPal.BuildTLV("部分密码恢复失败");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLACCOUNTPWALL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}

	}
	catch(...)
	{
		operPal.BuildTLV("密码恢复错误");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLACCOUNTPWALL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}

	return;
}

//Added by tengjie 2010-08-02
// string CPalAPI::UserLogInterface(char *ServerIP,string NewUserName,string NewTime,string BanType)
// {
// 	char *_totalpath = new char[100];
// 	string _temppath  = "";
// 	CString szTemUrl = "";
// 	CInternetSession MySession;
// 	CHttpConnection* lpConnetction;
// 	CHttpFile*       lpHttpFile;
// 	INTERNET_PORT    HttpPort = 80;
// //	CString szMd5 = "";
// 	CString strSentence, strGetSentence = "";
// 	CString szSql = "";
// 	CString szData = "";
// 	CSqlHelper m_SqlHelper;
// 	vector<CGlobalStruct::DataRecord> datarecord;
// 
// 	GetCurrentDirectory(100,_totalpath);
// 	strcat(_totalpath,"\\Config\\config.ini");
// 	_temppath = _totalpath;
// 
// 	//获取游戏IP
// 	char PalServerIp[20]= "";
// 	char PalUrl[128]="";
// 	GetPrivateProfileString("server","PalServerIp","61.152.150.158",PalServerIp,20,_temppath.c_str());//华东一区
// 	GetPrivateProfileString("url","PalUrl","passport/games/gmtools_ban_log_w.php",PalUrl,128,_temppath.c_str());//WA接收地址
// 
// 	delete []_totalpath;
// 	
// 	szSql.Format("exec WA_GetIdx '%s'",ServerIP);
// 	
// 	m_SqlHelper.SqlHelperMain("PAL", &datarecord,"127.0.0.1",szSql.GetBuffer(0),4,0,0);
// 	
// 	if(datarecord.empty())
// 		return "";
// 	
// 	vector<CGlobalStruct::DataRecord>::iterator iter;
// 	CGlobalStruct::DataRecord  m_DataRecord;
// 	for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
// 	{
// 		m_DataRecord = *iter;
// 		if(!strcmp("idx", m_DataRecord.recordfieldname))//获取相应的ServerIP
// 		{
// 			szData.Format("%s", m_DataRecord.recorddata);//保存查询得到的最后一条记录
// 			break;
// 		}
// 	}
// 
// 	szTemUrl.Format("http://%s/%s",PalServerIp,PalUrl);
// 	lpConnetction = MySession.GetHttpConnection(PalServerIp, HttpPort);
// 	
// 	lpHttpFile = lpConnetction->OpenRequest(CHttpConnection::HTTP_VERB_POST, 
// 		PalUrl,
// 		szTemUrl.GetBuffer(0),1, NULL, NULL, 
// 		INTERNET_FLAG_NO_AUTO_REDIRECT);
// 	
// 	if(NULL != lpHttpFile)
// 	{
// 		string  strHeader = "Content-Type:application/x-www-form-urlencoded";
// 		
// // 		//ascii转换成unicode
// // 		int  wcsLen  =  ::MultiByteToWideChar(CP_ACP, NULL, you9Nickname.c_str(), you9Nickname.length(), NULL,  0 );
// // 		wchar_t *  wszString  =   new  wchar_t[wcsLen  +   1 ];
// // 		::MultiByteToWideChar(CP_ACP, NULL, you9Nickname.c_str(), you9Nickname.length(), wszString, wcsLen);
// // 		wszString[wcsLen]  = '\0' ;
// // 		
// // 		//unicode转换成utf8
// // 		int u8Len = ::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);
// // 		char* szU8 = new char[u8Len + 1];
// // 		::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), szU8, u8Len, NULL, NULL);
// // 		szU8[u8Len] = '\0';
// 		
// 		int length=0;
// 		
// 		string strMd5 = NewUserName;
// 		strMd5 += "|T4pb5A.gmtoolbanlog";
// 		//md5加密
// 		char *szMd5 = new char[strlen(strMd5.c_str())+1];
// 		ZeroMemory(szMd5, strlen(strMd5.c_str())+1);
// 		MD5_EnCode(szMd5,&length,(char*)strMd5.c_str(),strMd5.length());
// 
// // 		int newlength=0;
// // 		ZeroMemory(strMD5sign,1024);
// // 		MD5_EnCode(strMD5sign,&newlength,strsign,strlen(strsign));
// 
// 		string requestString = "username=";
// 			   requestString += NewUserName;
// 			   requestString += "&gmareaid=";
// 			   requestString += szData.GetBuffer(0);
// 			   requestString += "&ban=";
// 			   requestString += BanType;
// 			   requestString += "&banhour=";
// 			   requestString += NewTime;
// 			   requestString += "&sign=";
// 			   requestString += szMd5;
// 		
// 		printf("the parameters of request: %s\n",requestString.c_str());
// 
// 		if(lpHttpFile->SendRequest(strHeader.c_str(),
// 			(DWORD)strHeader.length(),
// 			(void*)requestString.c_str(),
// 			(DWORD)requestString.length()))
// 			printf("send request success!\n");
// 		
// 		while(lpHttpFile->ReadString(strSentence))    // 读取提交数据后的返回结果
// 		{
// 			strGetSentence = strGetSentence + strSentence + char(13) + char(10);
// 		}
// 		
// 		printf("返回的数据:%s\n",strGetSentence);
// 		
// 		delete []szMd5;
// 	}
// 
// 	return strGetSentence;
// }