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

//��ѯ��ұ���
void CPalAPI::Pal_CharacterBag(char * ServerName, char * ServerIP, char * RoleName, int ibagtype,int iIndex,int iPageSize)
{
	try
	{
		CCharacterBag CB;//ʵ������ұ�������
		vector <CGlobalStruct::TFLV> DBTFLV=CB.CharacterBagMain(ServerName,ServerIP,RoleName,ibagtype);//������ұ�����ѯ�ӿ�
		operPal.BuildTLV(COperVector(DBTFLV),iIndex,iPageSize);////����ҳ���ͼ�¼���������ݼ����ݰ�
		//�����ݰ���Ӱ�ͷ
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERBAG_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	

	}
	
	catch (...)
	{
		operPal.BuildTLV("����Ϊ��");///����message���ݰ�
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERBAG_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	
	}

}

//ȡ�ý�ɫ�����б�
void CPalAPI::Pal_CharacterFriendList(char * ServerName, char * ServerIP, char * RoleName,int iIndex,int iPageSize)
{
	try
	{
		CCharacterFriendList CFL;//ʵ������ɫ�����б���Ϣ
		vector <CGlobalStruct::TFLV> DBTFLV=CFL.CharacterFriendListMain(ServerName,ServerIP,RoleName);//���ý�ɫ�����б���Ϣ�ӿ�
		operPal.BuildTLV(COperVector(DBTFLV),iIndex,iPageSize);////����ҳ���ͼ�¼���������ݼ����ݰ�
		//�����ݰ���Ӱ�ͷ
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERFRIENDLIST_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	

	}
	catch(...)
	{
		operPal.BuildTLV("�����б�Ϊ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERFRIENDLIST_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	
	}
}

//ȡ�ý�ɫ������Ѷ
void CPalAPI::Pal_CharacterQuest(char * ServerName, char * ServerIP,char * RoleName,int iType,int iIndex,int iPageSize)
{
	try
	{
		CCharacterQuest CQ;//ʵ������ɫ������Ϣ
		vector <CGlobalStruct::TFLV> DBTFLV=CQ.CharacterQuestMain(ServerName,ServerIP,RoleName,iType);//���ý�ɫ������Ϣ�ӿ�
		operPal.BuildTLV(COperVector(DBTFLV),iIndex,iPageSize);////����ҳ���ͼ�¼���������ݼ����ݰ�
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERQUEST_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	

	}
	catch(...)
	{
		operPal.BuildTLV("����Ϊ��");///����message���ݰ�
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERQUEST_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	
	}

}

//ȡ�ù��������Ѷ
void CPalAPI::Pal_GuileBaseInfo(char * ServerName, char * ServerIP,char * GuildName,int iIndex, int iPageSize)
{
	try
	{
		CGuildBaseInfo GBI;	//ʵ�������������Ϣ
		vector <CGlobalStruct::TFLV> DBTFLV=GBI.GuildBaseInfoMain(ServerName,ServerIP,GuildName);//���ù��������Ϣ�ӿ�
		operPal.BuildTLV(COperVector(DBTFLV),iIndex,iPageSize);////����ҳ���ͼ�¼���������ݼ����ݰ�
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_GUILDBASEINFO_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	

	}
	catch(...)
	{
		operPal.BuildTLV("���������Ѷ��");///����message���ݰ�
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_GUILDBASEINFO_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	
	}
	
}

//ȡ�ù����Ա�б�
void CPalAPI::Pal_GuildMemberList(char * ServerName, char * ServerIP,char * GuildName,int iIndex, int iPageSize)
{
	try
	{
		CGuildMemberList GML;//ʵ���������Ա��Ϣ
		vector <CGlobalStruct::TFLV> DBTFLV=GML.GuildMemberListMain(ServerName,ServerIP,GuildName);//���ù����Ա��Ϣ�ӿ�
		operPal.BuildTLV(COperVector(DBTFLV),iIndex,iPageSize);////����ҳ���ͼ�¼���������ݼ����ݰ�
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_GUILDMEMBERLIST_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	

	}
	catch(...)
	{
		operPal.BuildTLV("�����Ա��Ϣ��");///����message���ݰ�
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_GUILDMEMBERLIST_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	
	}
	
}

//���ͽ�Ʒ�ʼ����������5�����
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
		CSendGiftMail SGM;//ʵ�������ͽ�Ʒ�ʼ�
		vector<CGlobalStruct::TFLV> DBTFLV=SGM.SendGiftMailMain(ServerName,ServerIP,RoleName,Subject,Content,iMoney,
			Gift0,Gift1,Gift2,Gift3,Gift4,reason);//���ü��ͽ�Ʒ�ʼ��ӿ�
		operPal.BuildTLV((COperVector)DBTFLV,0,0);//�������ݰ�
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_SENDGIFTMAIL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		char giftname[100];
		char text[512];
		ZeroMemory(giftname,100);
		ZeroMemory(text,512);
		sprintf(text,"��ɫ��<%s>,",RoleName);
		operPal.GiftIDToName(giftname,Gift0);//���ݵ���ID��õ�������
		sprintf(text,"%s������:%s",text,giftname);
		operPal.GiftIDToName(giftname,Gift1);
		sprintf(text,"%s������:%s",text,giftname);
		operPal.GiftIDToName(giftname,Gift2);
		sprintf(text,"%s������:%s",text,giftname);
		operPal.GiftIDToName(giftname,Gift3);
		sprintf(text,"%s������:%s",text,giftname);
		operPal.GiftIDToName(giftname,Gift4);
		sprintf(text,"%s������:%s",text,giftname);
		sprintf(text,"%s��Ǯ��%i",text,iMoney);
		operPal.RecordData(UserByID,CEnumCore::Message_Tag_ID::PAL_SENDGIFTMAIL,ServerName,
			&COperVector(DBTFLV),text);//��¼������־
	}
	catch(...)
	{
		operPal.BuildTLV("�ʼ�����ʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_SENDGIFTMAIL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());

	}
	
}

//��ɢ����
void CPalAPI::Pal_DismissGuild(int userByID,char * ServerName, char * ServerIP,char * GuildName,char * Reason)
{
	try
	{
		CDismissGuild DG;//ʵ������ɢ�������	
		vector<CGlobalStruct::TFLV> DBTFLV=DG.DismissGuildMain(ServerName,ServerIP,GuildName);//���ý�ɢ����ӿ�
		operPal.BuildTLV((COperVector)DBTFLV,0,0);//�������ݰ�
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_DISMISSGUILD_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		char text[256];
		ZeroMemory(text,256);
		sprintf(text,"����������:%s����:%s",ServerName,Reason);
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_DISMISSGUILD,ServerName,&((COperVector)DBTFLV),text);//��¼������־
		
	}
	catch(...)
	{
		operPal.BuildTLV("��ɢ����ʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_DISMISSGUILD_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	
}

//�޸Ĺ�������
void CPalAPI::Pal_ModifyGuildName(int userByID,char * ServerName, char * ServerIP,char * GuildName,char * NewName,char * Reason)
{
	try
	{
		CModifyGuildName MGN;//ʵ�����޸Ĺ������ƶ���	
		vector<CGlobalStruct::TFLV> DBTFLV=MGN.ModifyGuildNameMain(ServerName,ServerIP,GuildName,NewName);//�����޸Ĺ������ƽӿ�
		operPal.BuildTLV((COperVector)DBTFLV,0,0);//�������ݰ�
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYGUILDNAME_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYGUILDNAME,ServerName,&(COperVector(DBTFLV)),Reason);//��¼������־

	}
	catch(...)
	{
		operPal.BuildTLV("���Ĺ�������ʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYGUILDNAME_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	
}


//�޸Ĺ����Աְ��
void CPalAPI::Pal_ModifyGuildMemberPL(int userByID,char * ServerName, char * ServerIP,char * GuildName,char * RoleName, int iLevel,char * Reason)
{
	try
	{
		CModifyGuildMemberPL MGMPL;//ʵ�����޸Ĺ����Աְ�׶���
		vector<CGlobalStruct::TFLV> DBTFLV=MGMPL.ModifyGuildMemberPLMain(ServerName,ServerIP,GuildName,RoleName,iLevel);//�����޸Ĺ����Աְ�׽ӿ�
		operPal.BuildTLV((COperVector)DBTFLV,0,0);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYGUILDMEMBERPL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYGUILDMEMBERPL,ServerName,&(COperVector(DBTFLV)),Reason);//��¼������־

	}
	catch(...)
	{
		operPal.BuildTLV("���Ĺ����Աְ��ʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYGUILDMEMBERPL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	
}

//�޸Ľ�ɫ����	
void CPalAPI::Pal_ModifyCharacterQuest(int userByID,char *ServerName, char * ServerIP,char* szRoleName,int iQuestID,int iQuestStatus)
{
	try
	{
		CModifyCharacterQuest MCQ;//ʵ�����޸Ľ�ɫ�������	
		vector <CGlobalStruct::TFLV> DBTFLV=MCQ.ModifyCharacterQuestMain(userByID,ServerName,ServerIP,szRoleName,iQuestID,iQuestStatus);//�����޸Ľ�ɫ����ӿ�
		operPal.BuildTLV((COperVector)DBTFLV,0,0);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYCHARACTERMONEY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYCHARACTERMONEY,ServerName,&(COperVector(DBTFLV)),"");//��¼������־
	}
	catch(...)
	{
		operPal.BuildTLV("�޸Ľ�ɫ����ʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYCHARACTERMONEY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	
}
	
//�޸Ľ�ɫ��Ǯ
void CPalAPI::Pal_ModifyCharacterMoney(int userByID,char * ServerName, char * ServerIP, char * szRoleName,int iType,int iCount)
{
	try
	{
		CModifyCharacterMoney MCM;//ʵ�����޸Ľ�ɫ��Ǯ����	
		vector <CGlobalStruct::TFLV> DBTFLV=MCM.ModifyCharacterMoneyMain(ServerName,ServerIP,szRoleName,iType,iCount);//�����޸Ľ�ɫ��Ǯ�ӿ�
		operPal.BuildTLV((COperVector)DBTFLV,0,0);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYCHARACTERMONEY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYCHARACTERMONEY,ServerName,&(COperVector(DBTFLV)),"");//��¼������־

	}
	catch(...)
	{
		operPal.BuildTLV("�޸Ľ�ɫ��Ǯʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYCHARACTERMONEY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	
}

//�޸Ľ�ɫɾ������
void CPalAPI::Pal_ModifyCharacterDropPw(int userByID,char * ServerName, char * ServerIP, char * szRoleName,char * szDropRolePw)
{
	CModifyCharacterDropPw MCDP;//ʵ�����޸Ľ�ɫɾ���������

	char strlog[80];
	ZeroMemory(strlog,80);
	sprintf(strlog,"����<%s>�޸Ľ�ɫ<%s>ɾ������",ServerName,szRoleName);
	logFile.WriteText("�ɽ�OL",strlog);
	bool b_result=MCDP.FindLocalCharacterDropPw(szRoleName);//��֤��ɫɾ�������Ƿ��޸Ĺ�
	if(!b_result)
	{
		logFile.WriteText("�ɽ�OL","PAL_MODIFYCHARACTERDROPPW\n");
		char szOldPasswd[60];
		ZeroMemory(szOldPasswd,60);
		MCDP.GetDropPw(szRoleName,szOldPasswd,ServerName);//�ӱ������ݿ��л�ȡ��ɫɾ������
		
		CBanishPlayer BP;//ʵ�������˶���
		BP.BanishPlayerMain(ServerName,ServerIP,szRoleName);//�������˽ӿ�

		
		vector <CGlobalStruct::TFLV> DBTFLV=MCDP.ModifyCharacterDropPwMain(ServerName,ServerIP,szRoleName,szDropRolePw);//�����޸Ľ�ɫɾ������ӿ�
		operPal.BuildTLV((COperVector)DBTFLV,0,0);//�������ݰ�
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYCHARACTERDROPPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYCHARACTERDROPPW,ServerName,&(COperVector(DBTFLV)),"");//��¼������־
		CGlobalStruct::TFLV m_tflv=DBTFLV.front();//ָ��vector�Ŀ�ʼ��
		char *pdest=strstr((char *)&m_tflv.lpdata,"�ɹ�");//�ڷ��ص�vector��Ѱ���Ƿ����'�ɹ�'
		if(pdest!=NULL)
		{
			b_result=MCDP.SaveCharacterDropPw(szRoleName,szOldPasswd,szDropRolePw,ServerName);//�ڱ������ݿ��б����޸�֮ǰ�����룬ͬʱ���ñ��
		}
	}
	else
	{
		operPal.BuildTLV("�������޸�,��ָ�");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYCHARACTERDROPPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
}
//��ԭ��ɫɾ������
void CPalAPI::Pal_RecallCharacterDropPw(int userByID,char * ServerName, char * ServerIP, char * szRoleName)
{
	try
	{
		char szDropRolePw[16];
		ZeroMemory(szDropRolePw,16);


		char strlog[80];
		ZeroMemory(strlog,80);
		sprintf(strlog,"����<%s>��ԭ��ɫ<%s>ɾ������",ServerName,szRoleName);
		logFile.WriteText("�ɽ�OL",strlog);

		CModifyCharacterDropPw MCDP;//
		bool b_result=false;
		b_result=MCDP.RecallCharacterDropPw(szRoleName,szDropRolePw);//�жϽ�ɫɾ�������Ƿ񱻻ָ���

		
		if(b_result)
		{
			CBanishPlayer BP;//ʵ�������˶���
			BP.BanishPlayerMain(ServerName,ServerIP,szRoleName);//�������˽ӿ�

			vector <CGlobalStruct::TFLV> DBTFLV=MCDP.ModifyCharacterDropPwMain(ServerName,ServerIP,szRoleName,szDropRolePw);//�����޸Ľ�ɫɾ������ӿ�
			operPal.BuildTLV((COperVector)DBTFLV,0,0);
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLCHARACTERDROPPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
			operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_RECALLCHARACTERDROPPW,ServerName,&(COperVector(DBTFLV)),"");//��¼������־
		}
		else
		{
			operPal.BuildTLV("����δ����");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLCHARACTERDROPPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}

	}
	catch(...)
	{
		operPal.BuildTLV("����δ����");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLCHARACTERDROPPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
}

//�޸��ʺ���������	
void CPalAPI::Pal_ModifyAccountBankPw(int userByID,char * ServerName, char * ServerIP, char * szAccount,char * szBankPw)
{
	char strlog[60];
	ZeroMemory(strlog,60);
	sprintf(strlog,"����<%s>�޸��ʺ�<%s>��������",ServerName,szAccount);
	logFile.WriteText("�ɽ�OL",strlog);
	bool b_result=false;
	CModifyAccountBankPw MABP;//ʵ�����޸��ʺ������������
	b_result=MABP.FindLocalAccountBankPw(szAccount);//�ڱ������ݿ�����֤�����Ƿ��޸Ĺ�
	if(!b_result)
	{
		char szOldPasswd[60];
		ZeroMemory(szOldPasswd,60);
		MABP.GetBankPw(szAccount,szOldPasswd,ServerName);//����Ϸ���ݿ��л�ȡ�ʺ���������
		
		CBanishPlayer BP;//ʵ�������˶���
		S_ROLENAME* s_rolename=new S_ROLENAME[10];//����ṹ��ָ��
		int rolenum=0;
		/////int iIndex=0,iPageSize=0;
		int i=0;
		rolenum=operPal.AccountToRoleName(s_rolename,ServerName,szAccount);//�����ʺŻ�ý�ɫ��
		for(i=0;i<rolenum;i++)
		{
			BP.BanishPlayerMain(ServerName,ServerIP,s_rolename[i].rolename);//�������˽ӿ�
		}
		
		delete s_rolename;
		s_rolename=NULL;


		vector <CGlobalStruct::TFLV> DBTFLV=MABP.ModifyAccountBankPwMain(ServerName,ServerIP,szAccount,szBankPw);//�����޸��ʺ���������ӿ�
		operPal.BuildTLV((COperVector)DBTFLV,0,0);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYACCOUNTBANKPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTBANKPW,ServerName,&(COperVector(DBTFLV)),szBankPw);//��¼������־
		CGlobalStruct::TFLV m_tflv=DBTFLV.front();//ָ��vector�Ŀ�ʼ��
		char *pdest=strstr((char *)&m_tflv.lpdata,"�ɹ�");
		if(pdest!=NULL)
		{
			MABP.SaveAccountBankPw(szAccount,szOldPasswd,szBankPw,ServerName);//����������ݿ���û����ؼ�¼�����ڱ������ݿ��в����¼
		}
	}
	else
	{
		operPal.BuildTLV("�������޸�,��ָ�");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYACCOUNTBANKPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
}

//��ԭ�ʺ���������	
void CPalAPI::Pal_RecallAccountBankPw(int userByID,char * ServerName, char * ServerIP, char * szAccount)
{
	try
	{
		char szBankPw[7];
		ZeroMemory(szBankPw,7);
	

		char strlog[60];
		ZeroMemory(strlog,60);
		sprintf(strlog,"����<%s>��ԭ�ʺ�<%s>��������",ServerName,szAccount);
		logFile.WriteText("�ɽ�OL",strlog);

		CModifyAccountBankPw MABP;//ʵ�����޸��ʺ������������
		bool b_result=false;
		b_result=MABP.RecallAccountBankPw(szAccount,szBankPw);//�ڱ������ݿ��в����Ƿ����޸Ĺ�������,ͬʱ�������,�޸ļ�¼��flagֵΪ0����ԭ��
		
		
		if(b_result)
		{
			CBanishPlayer BP;//ʵ�������˶���
			S_ROLENAME* s_rolename=new S_ROLENAME[10];//����ṹ��ָ��
			int rolenum=0;
			////int iIndex=0,iPageSize=0;
			int i=0;
			rolenum=operPal.AccountToRoleName(s_rolename,ServerName,szAccount);//�����ʺŻ�ý�ɫ��
			for(i=0;i<rolenum;i++)
			{
				BP.BanishPlayerMain(ServerName,ServerIP,s_rolename[i].rolename);//�������˽ӿ�
			}
			
			delete s_rolename;
			s_rolename=NULL;

			vector <CGlobalStruct::TFLV> DBTFLV=MABP.ModifyAccountBankPwMain(ServerName,ServerIP,szAccount,szBankPw);//�����޸��ʺ���������ӿ�(��ԭ)
			operPal.BuildTLV((COperVector)DBTFLV,0,0);
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLACCOUNTBANKPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
			operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTBANKPW,ServerName,&(COperVector(DBTFLV)),szBankPw);//��¼������־
		}
		else
		{
			operPal.BuildTLV("����δ����");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLACCOUNTBANKPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}
		
	}
	catch(...)
	{
		operPal.BuildTLV("����δ����");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLACCOUNTBANKPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		
	}

}

//�޸��ʺ�����
void CPalAPI::Pal_ModifyAccountPw(int userByID,char * ServerName, char * ServerIP, char * szAccount,char * szAccountPw)
{

	char text[512];
	char strlog[60];
	ZeroMemory(strlog,60);
	sprintf(strlog,"����<%s>�޸��ʺ�<%s>����",ServerName,szAccount);
	logFile.WriteText("�ɽ�OL",strlog);
	
	char accountdbip[30];
	ZeroMemory(strlog,30);
	operPal.FindAccountDBIP(accountdbip, ServerName);
	if (!strcmp(accountdbip, ""))
	{
		operPal.BuildTLV("��ѯ���ݿ�ʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYACCOUNTPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		return;
	}
	
	CModifyAccountPw MAP;//ʵ�����޸��ʺ��������
	int b_result = -1;
	char szMD5Password[50];
	ZeroMemory(szMD5Password,50);

	//�жϱ������ݿ����Ƿ�����Ҫ�޸ĵ��ʺ������¼(flag=1),���û�в���һ����ͬʱ�����������md5����
	b_result=MAP.SaveAccountPw(szAccount,szAccountPw,accountdbip,szMD5Password);
	if(b_result == 1)  //��¼������
	{
					
		b_result=MAP.PutAccountPw(szAccount,szMD5Password,accountdbip);//�����޸��ʺ�����
		if(b_result)
		{
				sprintf(text,"�ʺ�<%s>����<%s>��ʱ�����޸ĳɹ�!������:%s",szAccount, accountdbip, szAccountPw);
				//Modified by tengjie 2010-07-08
//				operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPW,accountdbip,NULL,text);//��¼������־
				operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPW,ServerName,NULL,text);//��¼������־
				operPal.BuildTLV("��ʱ�����޸ĳɹ�");
				operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYACCOUNTPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}
		else
		{
				sprintf(text,"�ʺ�<%s>����<%s>��ʱ�����޸�ʧ��!",szAccount, accountdbip);
				//Modified by tengjie 2010-07-08
//				operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPW,accountdbip,NULL,text);
				operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPW,ServerName,NULL,text);
				operPal.BuildTLV("��ʱ�����޸�ʧ��");
				operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYACCOUNTPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}
	}
	else if(b_result == 0)  //��¼����
	{
		sprintf(text,"�ʺ�<%s>����<%s>��ʱ�������޸�,��ָ�!",szAccount, accountdbip);
		//Modified by tengjie 2010-07-08
//		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPW,accountdbip,NULL,text);
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPW,ServerName,NULL,text);
		operPal.BuildTLV("��ʱ�������޸�,��ָ�");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYACCOUNTPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	else if (b_result == -1)  //Զ�̲�ѯ�������
	{
		sprintf(text,"�ʺ�<%s>����<%s>�û��ʺŲ�����",szAccount, accountdbip);
		//Modified by tengjie 2010-07-08
//		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPW,accountdbip,NULL,text);
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPW,ServerName,NULL,text);
		operPal.BuildTLV("�û��ʺŲ�����");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYACCOUNTPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
}

//��ԭ�ʺ�����	
void CPalAPI::Pal_RecallAccountPw(int userByID,char * ServerName, char * ServerIP, char * szAccount)
{
	try
	{
		char szMD5Passwd[50];
		ZeroMemory(szMD5Passwd,50);
		char text[512];
		char strlog[60];
		ZeroMemory(strlog,60);
		sprintf(strlog,"����<%s>��ԭ�ʺ�<%s>����",ServerName,szAccount);
		logFile.WriteText("�ɽ�OL",strlog);

		char accountdbip[30];
		ZeroMemory(strlog,30);
		operPal.FindAccountDBIP(accountdbip, ServerName);
		if (!strcmp(accountdbip, ""))
		{
			operPal.BuildTLV("��ѯ���ݿ�ʧ��");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYACCOUNTPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
			return;
		}
		
		CModifyAccountPw MAP;//ʵ�����޸��ʺ��������
		bool b_result=false;
		b_result=MAP.RecallAccountPw(szAccount,szMD5Passwd, accountdbip);//�ڱ������ݿ��в�����Ҫ�ָ��ļ�¼�����ԭ�����룬ͬʱ��flag��Ϊ0(��ԭ)
	
		if(b_result)
		{
			b_result=MAP.PutAccountPw(szAccount,szMD5Passwd,accountdbip);//�����޸��ʺ�����
			if(b_result)
			{
				sprintf(text,"�ʺ�<%s>����<%s>��ԭ����ɹ�",szAccount,accountdbip);
				//Modified by tengjie 2010-07-08
//				operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTPW,accountdbip,NULL,text);//��¼������־
				operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTPW,ServerName,NULL,text);//��¼������־
				operPal.BuildTLV("��ԭ����ɹ�");
				operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLACCOUNTPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
			}
			else
			{
				sprintf(text,"�ʺ�<%s>����<%s>��ԭ����ʧ��",szAccount,accountdbip);
				//Modified by tengjie 2010-07-08
//				operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTPW,accountdbip,NULL,text);
				operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTPW,ServerName,NULL,text);
				operPal.BuildTLV("��ԭ����ʧ��");
				operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLACCOUNTPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
			}
		}
		else
		{
			operPal.BuildTLV("����δ�޸�");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLACCOUNTPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}
		
	}
	catch(...)
	{
		operPal.BuildTLV("��ԭ����ʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLACCOUNTPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
}

//�޸Ķ�������
void CPalAPI::Pal_ModifySecondPw(int userByID,char * ServerName,char * ServerIP,char * szAccount,char * szSePw)
{
	char text[256];
	char retMessage[256];
	char strlog[60];
	ZeroMemory(strlog,60);
	sprintf(strlog,"����<%s>�޸��ʺ�<%s>�Ķ�������",ServerName,szAccount);
	logFile.WriteText("�ɽ�OL",strlog);
	bool b_result=false;
	char szMD5Password[50];
	ZeroMemory(szMD5Password,50);
	CModifySecLvlPwd mSLP;//ʵ�����޸Ķ�������������
	b_result=mSLP.SaveSecLvlPwd(szAccount,szSePw,ServerName,szMD5Password);//��ѯ����������û�б��޸Ĺ�
	if(b_result)
	{
		b_result=mSLP.PutSecLvlPwd(szAccount,szMD5Password,ServerName);//�޸Ķ�������
		if(b_result)
		{
			sprintf(text,"�޸��ʺ�<%s>���������޸ĳɹ�!������:%s",szAccount,szSePw);
		}
		else
		{
			sprintf(text,"�޸��ʺ�<%s>���������޸�ʧ��!������:%s",szAccount,szSePw);
			sprintf(retMessage,"%s","��ʱ�����޸�ʧ��");
		}
	}
	else
	{
		sprintf(text,"�ʺ�<%s>�����������޸�,��ָ�",szAccount);
		sprintf(retMessage,"%s","��ʱ�������޸�,��ָ�");
	}
	operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYSECONDPW,ServerName,NULL,text);///��¼������־
	operPal.BuildTLV(retMessage);
	operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYSECONDPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());

}

//��ԭ��������
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
		sprintf(strlog,"����<%s>��ԭ�ʺ�<%s>�Ķ�������",ServerName,szAccount);
		logFile.WriteText("�ɽ�OL",strlog);

		CModifySecLvlPwd mSLP;//ʵ�����޸Ķ�������������
		bool b_result=false;
		b_result=mSLP.RecallSecLvlPwd(szAccount,szMD5Passwd);//�ڱ������ݿ��в�����Ҫ�ָ��ļ�¼�����ԭ�����룬ͬʱ��flag��Ϊ0(��ԭ)
		
		
		if(b_result)
		{
			b_result=mSLP.PutSecLvlPwd(szAccount,szMD5Passwd,ServerName);//�޸Ķ�������
			if(b_result)
			{
				sprintf(text,"�ʺ�<%s>��ԭ����ɹ�",szAccount);
				sprintf(retMessage,"%s","��ԭ����ɹ�");
			}
			else
			{
				sprintf(text,"�ʺ�<%s>��ԭ����ʧ��",szAccount);
				sprintf(retMessage,"%s","��ԭ����ʧ��");
			}
		}
		else
		{
			sprintf(text,"%s","����δ����");
			sprintf(retMessage,"%s","����δ����");
		}
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_RECALLSECONDPW,ServerName,NULL,text);//��¼������־
		operPal.BuildTLV(retMessage);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLSECONDPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());

		
	}
	catch(...)
	{
		operPal.BuildTLV("��ԭ����ʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLSECONDPW_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
}


//������ȫ��
void CPalAPI::Pal_UseSafeLock(int userByID,char * ServerName,char * ServerIP,char * szAccount)
{
	char text[256];
	char retMessage[256];
	char strlog[60];
	ZeroMemory(strlog,60);
	sprintf(strlog,"����<%s>�����ʺ�<%s>�İ�ȫ��",ServerName,szAccount);
	logFile.WriteText("�ɽ�OL",strlog);

	bool b_result=false;
	CModifySafeLock mSL;//ʵ����������ȫ������
	b_result=mSL.GetSafeLock(szAccount,ServerName);//��ð�ȫ����״̬,0�⿪������ֵ����
	if(b_result)
	{
		b_result=mSL.LockSafeLock(szAccount,ServerName);//������ȫ��(�ı�prg_lock = 100)
		if(b_result)
		{
			sprintf(text,"�ɹ������ʺ�<%s>�İ�ȫ��",szAccount);
			sprintf(retMessage,"%s","������ȫ���ɹ�");
		}
		else
		{
			sprintf(text,"�����ʺ�<%s>��ȫ��ʧ��",szAccount);
			sprintf(retMessage,"%s","������ȫ��ʧ��");
		}
	}
	else
	{
		sprintf(text,"�����ʺ�<%s>��ȫ��ʧ��",szAccount);
		sprintf(retMessage,"%s","��ȫ���ѱ�����");
	}
	operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_USESAFELOCK,ServerName,NULL,text);//��¼������־
	operPal.BuildTLV(retMessage);
	operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_USESAFELOCK_RESP,operPal.GetSendBuf(),operPal.GetSendLength());

}

//����������
void CPalAPI::Pal_FreeSafeLock(int userByID,char * ServerName,char * ServerIP,char * szAccount)
{
	try
	{
		char text[256];
		char retMessage[256];
		char strlog[60];
		ZeroMemory(strlog,60);
		sprintf(strlog,"����<%s>�����ʺ�<%s>��ȫ��",ServerName,szAccount);
		logFile.WriteText("�ɽ�OL",strlog);
		CModifySafeLock mSL;//ʵ��������������
		bool b_result=false;
		b_result=mSL.GetSafeLock(szAccount,ServerName);//��ð�ȫ����״̬,0�⿪������ֵ����
		
		if(!b_result)
		{
			b_result=mSL.UnLockSafeLock(szAccount,ServerName);//����������(�ı�prg_lock = 0)
			if(b_result)
			{
				sprintf(text,"�ʺ�<%s>�����������ɹ�",szAccount);
				sprintf(retMessage,"%s","�����������ɹ�");
			}
			else
			{
				sprintf(text,"�ʺ�<%s>����������ʧ��",szAccount);
				sprintf(retMessage,"%s","����������ʧ��");
			}
		}
		else
		{
			sprintf(text,"%s","������δ������");
			sprintf(retMessage,"%s","������δ������");
		}
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_FREESAFELOCK,ServerName,NULL,text);//��¼������־
		operPal.BuildTLV(retMessage);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_FREESAFELOCK_RESP,operPal.GetSendBuf(),operPal.GetSendLength());

		
	}
	catch(...)
	{
		operPal.BuildTLV("����������ʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_FREESAFELOCK_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
}

// ɾ����ɫ��Ʒ��Ӧ
void CPalAPI::Pal_DelCharacterItem(int userByID,char * ServerName,char * ServerIP,char * RoleName,int iType,int iPosition,char * ItemName)
{
	try
	{
		CDelCharacterItem DCI;//ʵ����ɾ����ɫ��Ʒ����	
		vector<CGlobalStruct::TFLV> DBTFLV=DCI.DelCharacterItemMain(ServerName,ServerIP,RoleName,iType,iPosition);//����ɾ����ɫ��Ʒ�ӿ�
		operPal.BuildTLV((COperVector)DBTFLV,0,0);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_DELCHARACTERITEM_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_DELCHARACTERITEM,ServerName,&((COperVector)DBTFLV),ItemName);//��¼������־
	}
	catch(...)
	{
		operPal.BuildTLV("ɾ������ʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_DELCHARACTERITEM_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	
}

//�޸Ĺ���ȼ�
void CPalAPI::Pal_ModifyGuildLv(int userByID,char * ServerName,char * ServerIP, char * GuildName,int iLevel,char * Reason)
{
	try
	{
		CModifyGuildLv MGL;	//ʵ�����޸Ĺ���ȼ�����
		vector<CGlobalStruct::TFLV> DBTFLV=MGL.ModifyGuildLvMain(ServerName,ServerIP,GuildName,iLevel);//�����޸Ĺ���ȼ��ӿ�
		operPal.BuildTLV((COperVector)DBTFLV,0,0);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYGUILDLV_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYGUILDLV,ServerName,&((COperVector)DBTFLV),Reason);//��¼������־

	}
	catch(...)
	{
		operPal.BuildTLV("�޸Ĺ���ȼ�ʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYGUILDLV_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	

}

//�޸Ĺ��ṫ��
void CPalAPI::Pal_ModifyGuildNotice(int userByID,char * ServerName, char * ServerIP, char * GuildName,char * szNotice,char * Reason)
{
	try
	{
		CModifyGuildNotice MGN;//ʵ�����޸Ĺ��ṫ�����
		vector<CGlobalStruct::TFLV> DBTFLV=MGN.ModifyGuildNoticeMain(ServerName,ServerIP,GuildName,szNotice);//�����޸Ĺ��ṫ��ӿ�
		operPal.BuildTLV((COperVector)DBTFLV,0,0);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYGUILDNOTICE_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYGUILDNOTICE,ServerName,&((COperVector)DBTFLV),Reason);//��¼������־
	}
	catch(...)
	{
		operPal.BuildTLV("�޸Ĺ��ṫ��ʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYGUILDNOTICE_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	

}




//��������ѯ
void CPalAPI::Pal_ItemType_Query()
{
	try
	{
		char strlog[60];
		ZeroMemory(strlog,60);
		sprintf(strlog,"��������ѯ");
		logFile.WriteText("�ɽ�OL",strlog);
		
		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("PAL","127.0.0.1",4,0,0,"exec Pal_ItemType_Query");
		if(!DBTFLV.empty())
		{
			operPal.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operPal.BuildTLV("��������ѯʧ��");
		}
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ITEMTYPE_QUERY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	catch(...)
	{	
		operPal.BuildTLV("��������ѯʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ITEMTYPE_QUERY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
}

//�������Ʋ�ѯ
void CPalAPI::Pal_ItemName_Query(int iItemBigType)
{
	try
	{
		char strlog[60];
		ZeroMemory(strlog,60);
		sprintf(strlog,"�������Ʋ�ѯ");
		logFile.WriteText("�ɽ�OL",strlog);

		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("PAL","127.0.0.1",4,0,0,"exec  pal_itemname_query %i",iItemBigType);
		if(!DBTFLV.empty())
		{
			operPal.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operPal.BuildTLV("�������Ʋ�ѯʧ��");
		}
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ITEMNAME_QUERY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	
	}
	catch(...)
	{
		operPal.BuildTLV("�������Ʋ�ѯʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ITEMNAME_QUERY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
}		

//���Ͳֿ�����
void CPalAPI::PAL_SendBankPwd_Query(int userByID,char * ServerName, char * ServerIP,char * szAccount,char * mailBox)
{
	try
	{
		char strlog[60];
		
		sprintf(strlog,"���Ͳֿ�����%s",szAccount);
		logFile.WriteText("�ɽ�OL",strlog);	

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

		sprintf(tmpcontext,"�����ɽ�ONLINE�ֿ��������ǣ�%s",strPasswd);

		//sprintf(context,strMessage,szAccount,tmpcontext);//maple update
		//if(operPal.SendEmail(mailBox,context))
		sprintf(context,strMessage,szAccount);
		if(operPal.SendEmail(mailBox,context,tmpcontext))
		{
			operPal.BuildTLV("��������ɹ�");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_SendBankPwd_Query_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}
		else
		{
			operPal.BuildTLV("��������ʧ��");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_SendBankPwd_Query_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}

	}
	catch(...)
	{
		operPal.BuildTLV("��������ʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_SendBankPwd_Query_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
}

//���ͽ�ɫɾ������
void CPalAPI::PAL_SendDeletePwd_Query(int userByID,char * ServerName, char * ServerIP, char * szRoleName,char * mailBox)
{
	try
	{
		char strlog[60];
		
		sprintf(strlog,"���ͽ�ɫ%sɾ������",szRoleName);
		logFile.WriteText("�ɽ�OL",strlog);	
		
		char strPasswd[256];
		sprintf(strPasswd,"%06d",(int)rand()%100000);
		
		CModifyCharacterDropPw MCDP;

		vector <CGlobalStruct::TFLV> DBTFLV=MCDP.ModifyCharacterDropPwMain(ServerName,ServerIP,szRoleName,strPasswd);
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYCHARACTERDROPPW,ServerName,&(COperVector(DBTFLV)),strPasswd);//��¼������־
		
		//maple add
		char strMessage[128];
		ZeroMemory(strMessage,128);
		//operPal.ReadValue("PAL_Body",strMessage,1024);
		operPal.ReadValue("PAL_Body1",strMessage,128);
		char tmpcontext[512];
		ZeroMemory(tmpcontext,512);
		char context[512];
		ZeroMemory(context,512);
		
		sprintf(tmpcontext,"�����ɽ�ONLINE��ɫɾ�������ǣ�%s",strPasswd);
		//sprintf(context,strMessage,szAccount,tmpcontext);//maple update
		//if(operPal.SendEmail(mailBox,context))
		sprintf(context,strMessage,szRoleName);
		if(operPal.SendEmail(mailBox,context,tmpcontext))
		{
			operPal.BuildTLV("��������ɹ�");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_SendDeletePwd_Query_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}
		else
		{
			operPal.BuildTLV("��������ʧ��");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_SendDeletePwd_Query_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}
	}
	catch(...)
	{
		operPal.BuildTLV("��������ʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_SendDeletePwd_Query_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
}
//c#�汾�У�c++��ûʹ��
//��ȡ�û���ַ
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
		
		
		sprintf(url,"http://222.73.121.86:8080/user?req=getUserInfo&id=GMTOOLS&userid=%s&usertype=0&type=userinfo&s=%s",szAccount,strMD5sign);//�̳�IP�̶�
		
		
		vector<CGlobalStruct::DataRecord> ret_vector;
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;
		int iCow=0,iCol=0;
		
		if(operPal.ReadXmlGet("Get",url,"",&ret_vector,&iCow,&iCol))
		{
			COperVector sendopervector;
			sendopervector.InitCreate(iCow+1,iCol+3);
			
			CEnumCore::TagName m_TagName;//�����õ�TagName
			CEnumCore::TagFormat m_TagFormat;//�����õ�TagFormat
			
			CMyEvent m_Event;//��ȡCEvent����
			
			//��ȡ��ѯ�ļ�¼����ÿһ��
			
			for( iter = ret_vector.begin(); iter != ret_vector.end(); iter++ )
			{
					m_DataRecord = *iter;

					m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//��ȡ��¼�����ݵ�TagName
					m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//��ȡ��¼�����ݵ�TagFormat
					sendopervector.AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//����TFLV,����ӵ�vector��

				
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

		operPal.BuildTLV("��ȡE-Mailʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MAILBOX_QUERY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}


//ȡ�ý�ɫ������Ѷ
bool CPalAPI::Pal_CharacterAdvanceInfo(char * ServerName, char * ServerIP,char * RoleName,int iIndex,int iPageSize)
{
	try
	{
		CCharacterAdvanceInfo CAI;//ʵ������ɫ������Ϣ����
		vector <CGlobalStruct::TFLV> DBTFLV=CAI.CharacterAdvanceInfoMain(ServerName,ServerIP,RoleName);//���ý�ɫ������Ϣ�ӿ�
		operPal.BuildTLV(COperVector(DBTFLV),iIndex,iPageSize);//�������ݰ�
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERADVANCEINFO_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	catch(...)
	{
		operPal.BuildTLV("����Ϊ��Ϊ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERADVANCEINFO_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}

//ȡ�ý�ɫ������Ѷ
bool CPalAPI::Pal_CharacterSkill(char * ServerName, char * ServerIP,char * RoleName,int iIndex,int iPageSize)
{
	try
	{
		CCharacterSkill CS;//ʵ������ɫ������Ϣ����
		vector <CGlobalStruct::TFLV> DBTFLV=CS.CharacterSkillMain(ServerName,ServerIP,RoleName);//���ý�ɫ������Ϣ�ӿ�
		operPal.BuildTLV(COperVector(DBTFLV),iIndex,iPageSize);//�������ݰ�
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERSKILL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());

		
	}
	catch(...)
	{
		operPal.BuildTLV("����Ϊ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERSKILL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}
//ȡ�ý�ɫ��ħ��Ѷ
bool CPalAPI::Pal_CharacterMob(char * ServerName, char * ServerIP,char * RoleName,int iIndex,int iPageSize)
{
	try
	{
		CCharacterMob CM;//ʵ������ɫ��ħ��Ϣ����
		vector <CGlobalStruct::TFLV> DBTFLV=CM.CharacterMobMain(ServerName,ServerIP,RoleName);//���ý�ɫ��ħ��Ϣ�ӿ�
		operPal.BuildTLV(COperVector(DBTFLV),iIndex,iPageSize);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERMOB_RESP,operPal.GetSendBuf(),operPal.GetSendLength());

		
	}
	catch(...)
	{
		operPal.BuildTLV("��ħΪ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERMOB_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}

//ȡ�ý�ɫͼ����Ѷ
bool CPalAPI::Pal_CharacterPuzzleMap(char * ServerName, char * ServerIP,char * RoleName,int iIndex,int iPageSize)
{
	try
	{
		CCharacterPuzzleMap CPM;//ʵ������ɫͼ����Ϣ����
		vector <CGlobalStruct::TFLV> DBTFLV=CPM.CharacterPuzzleMapMain(ServerName,ServerIP,RoleName);//���ý�ɫͼ����Ϣ�ӿ�
		operPal.BuildTLV(COperVector(DBTFLV),iIndex,iPageSize);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERPUZZLEMAP_RESP,operPal.GetSendBuf(),operPal.GetSendLength());

		
	}
	catch(...)
	{
		operPal.BuildTLV("ͼ��Ϊ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERPUZZLEMAP_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}

//ȡ�ý�ɫ�������б�
bool CPalAPI::Pal_CharacterBlackList(char * ServerName, char * ServerIP, char * RoleName,int iIndex,int iPageSize)
{
	try
	{
		CCharacterBlackList CBL;//ʵ������ɫ����������
		vector <CGlobalStruct::TFLV> DBTFLV=CBL.CharacterBlackListMain(ServerName,ServerIP,RoleName);//���ý�ɫ��������Ϣ�ӿ�
		operPal.BuildTLV(COperVector(DBTFLV),iIndex,iPageSize);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERBLACKLIST_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	
	}
	catch(...)
	{
		operPal.BuildTLV("������Ϊ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERBLACKLIST_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	
	}
	return true;
}

//ɾ����ɫ��ħ
bool CPalAPI::Pal_DelCharacterMob(int userByID,char * ServerName, char * ServerIP,char * RoleName,int iMobID)
{
	try
	{
		CDelCharacterMob DCM;//ʵ����ɾ����ɫ��ħ����
		vector<CGlobalStruct::TFLV> DBTFLV=DCM.DelCharacterMobMain(ServerName,ServerIP,RoleName,iMobID);//����ɾ����ɫ��ħ�ӿ�
		operPal.BuildTLV((COperVector)DBTFLV,0,0);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_DELCHARACTERMOB_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_DELCHARACTERMOB,ServerName,&(COperVector(DBTFLV)),"");//��¼������־
		
	}
	catch(...)
	{
		operPal.BuildTLV("ɾ����ɫ��ħʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_DELCHARACTERMOB_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}

//�޸Ľ�ɫͼ��
bool CPalAPI::Pal_ModifyPuzzleMap(int userByID,char * ServerName,char * ServerIP,char * RoleName,char *strPuzzleMap,
								  int iMapID1,int iMapID2,int iMapID3,int iMapID4,int iMapID5,int iMapID6,int iMapID7,int iMapID8,int iMapID9)
{
	try
	{
		CModifyPuzzleMap MPM;//ʵ�����޸Ľ�ɫͼ������
		//�����޸Ľ�ɫͼ���ӿ�
		vector<CGlobalStruct::TFLV> DBTFLV=MPM.ModifyPuzzleMapMain(ServerName,ServerIP,RoleName,strPuzzleMap,iMapID1,iMapID2,iMapID3,iMapID4,iMapID5,iMapID6,iMapID7,iMapID8,iMapID9);
		operPal.BuildTLV((COperVector)DBTFLV,0,0);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYPUZZLEMAP_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYPUZZLEMAP,ServerName,&(COperVector(DBTFLV)),"");//��¼������־
	}
	catch(...)
	{
		operPal.BuildTLV("�޸Ľ�ɫͼ��ʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYPUZZLEMAP_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}

//ȡ���ʺ���Ѷ
bool CPalAPI::Pal_AccountInfo(char * ServerName, char * ServerIP, char * szAccountName, int iIndex, int iPageSize)
{
	try
	{
		CAccountInfo AI;//ʵ�����ʺ���Ϣ����
		vector <CGlobalStruct::TFLV> DBTFLV=AI.AccountInfoMain(ServerName,ServerIP,szAccountName);//�����ʺ���Ϣ�ӿ�
		operPal.BuildTLV(COperVector(DBTFLV),iIndex,iPageSize);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ACCOUNTINFO_RESP,operPal.GetSendBuf(),operPal.GetSendLength());	
	}
	catch(...)
	{
		operPal.BuildTLV("�ʺ���ѶΪ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ACCOUNTINFO_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}

//�ƶ���ɫ����ȫ��
bool CPalAPI::Pal_MoveCharacterToSafe(int userByID,char * ServerName, char * ServerIP, char * RoleName, int iZoneID)
{
	try
	{
		CMoveCharacterToSafe MCTS;//ʵ�����ƶ���ɫ����ȫ�����
		vector<CGlobalStruct::TFLV> DBTFLV=MCTS.MoveCharacterToSafeMain(ServerName,ServerIP,RoleName,iZoneID);//�����ƶ���ɫ����ȫ��ӿ�
		operPal.BuildTLV((COperVector)DBTFLV,0,0);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MOVECHARACTERTOSAFE_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MOVECHARACTERTOSAFE,ServerName,&(COperVector(DBTFLV)),"");//��¼������־
	}
	catch(...)
	{
		operPal.BuildTLV("�ƶ���ɫ����ȫ��ʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MOVECHARACTERTOSAFE_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}

//�������빦�ܿ���
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
//��ѯ��ͼ
bool CPalAPI::PAL_SearchMap_Query()
{
	try
	{
		char strlog[60];
		ZeroMemory(strlog,60);
		sprintf(strlog,"��ѯ��ͼ");
		logFile.WriteText("�ɽ�OL",strlog);



		vector <CGlobalStruct::TFLV> DBTFLV=GameDBQuery("PAL","127.0.0.1",4,0,0,"exec Pal_ServerMap_Query");
		if(!DBTFLV.empty())
		{
			operPal.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operPal.BuildTLV("���ݿ�Ϊ��");			
		}
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_SearchMap_Query_Resp,operPal.GetSendBuf(),operPal.GetSendLength());


	}
	catch (...)
	{
		
	}
	return true;
}

//�ɽ����˰��û���ɫ��, ����ʱ��������һ����ɫ�����԰��ʺ�������, ��ɫ���߲����߶���
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
		if(!strcmp(UserNick,""))//��ɫ��Ϊ��
		{
			char strlog[128];
			ZeroMemory(strlog,128);
			sprintf(strlog,"����<%s>�˺�<%s>��ѯ��ɫ������Ϣ",ServerName,UserName);
			logFile.WriteLog("�ɽ�",operName,strlog);//��LOG
			CCharacterInfo CI;
			int rolenum=0;
			int i=0,j=0;
			S_ROLENAME* s_rolename=new S_ROLENAME[10];
			rolenum=operPal.AccountToRoleName(s_rolename,ServerName,UserName);//���ʺ��µĽ�ɫ
			if(rolenum==0)
			{
				if(s_rolename!=NULL)
				{
					delete[] s_rolename;
					s_rolename=NULL;
				}
				operPal.BuildTLV("û�и��˺�");
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
					
					//////���Ԫ������
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
			//////���Ԫ������
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
		operPal.BuildTLV("��ѯ�û���Ϣʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_CHARACTERINFO_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		
	}
	return true;
}
//�����ʺ�����
bool CPalAPI::Pal_KickPlayer(int userByID,char * ServerName, char * ServerIP,char * UserName)
{
	CBanishPlayer BP;//ʵ�������˶���
	vector<CGlobalStruct::TFLV> DBTFLV=BP.BanishPlayerMain(ServerName,ServerIP,UserName);//�������˽ӿ�(��ɫ��)
	operPal.BuildTLV((COperVector)DBTFLV,0,0);
	operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_BANISHPLAYER_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	char text[256];
	ZeroMemory(text,256);
	sprintf(text,"����������:%s",ServerName);
	operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_BANISHPLAYER,ServerName,&(COperVector(DBTFLV)),text);//��¼������־
	return TRUE;
}

//�����ʺŷ�ͣ
bool CPalAPI::Pal_ClosePlayer(int userByID,char * ServerName, char * ServerIP ,char * UserName,char * UserNick, char* Reason)
{
	try
	{

		char strlog[80];
		char accountdbip[60];
		char szRemoteSql[2048];
		char retMessage[256];

		ZeroMemory(strlog,80);
		sprintf(strlog,"����<%s>��ͣ������ʺ�<%s>",ServerName,UserName);
		logFile.WriteText("�ɽ�OL",strlog);
		
		CBanishPlayer BP;//ʵ�������˶���
		S_ROLENAME* s_rolename=new S_ROLENAME[10];//����ṹ��ָ��
		int rolenum=0;
		int iIndex=0,iPageSize=0;
		int i=0;
		rolenum=operPal.AccountToRoleName(s_rolename,ServerName,UserName);//�����ʺŻ�ý�ɫ��
// 		for(i=0;i<rolenum;i++)
// 		{
// 			BP.BanishPlayerMain(ServerName,ServerIP,s_rolename[i].rolename);//�������˽ӿ�
// 		}

		// �ʺ��½�ɫ�����ʺ�����		
		BP.BanishPlayerMain(ServerName,ServerIP,s_rolename[0].rolename);//�������˽ӿ�		
		delete [] s_rolename;
		s_rolename=NULL;	

		
		operPal.FindAccountDBIP(accountdbip,ServerName);
		operPal.getRemoteSql("PAL","PAL_ACCOUNT_CLOSE",szRemoteSql,0);


		///////Զ�̲���
		int length=0;
		length=GameDBOper("PAL",accountdbip,0,szRemoteSql,UserName);
		if(length>0)//Զ�����ݿ�����ɹ�
		{
			length=GameDBOper("PAL","127.0.0.1",4,"exec Pal_Banishment_Close %i,'%s','%s','%s','%s'",
				userByID,ServerName,accountdbip,UserName,Reason);//�������ݿ�����¼
			if(length>0)
			{
				//Added by tengjie 2010-08-02
				UserLogInterface(ServerName, UserName, "", 31, "1");

				sprintf(retMessage,"%s","��ͣ�ɹ�");
			}
			else
			{
				sprintf(retMessage,"%s","��ͣʧ��,���ز���ʧ��");
			}
		}
		else
		{
			sprintf(retMessage,"%s","��ͣʧ��");
		}
		operPal.BuildTLV(retMessage);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ACCOUNT_CLOSE_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		//logFile.WriteDBLog(userByID,"pal","PAL_ACCOUNT_CLOSE",ServerIP,retMessage);//maple add//Reason
	}
	catch (...)
	{
		operPal.BuildTLV("��ͣʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ACCOUNT_CLOSE_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}

//maple add
//������ͣ����ʺ�
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
		
		sprintf(strlog,"����<%s>��ͣ������ʺ�",ServerName);
		logFile.WriteText("�ɽ�OL",strlog);
		
		vector <BanPlayerPACKET> v_banplayer;
		char * pUserName= strtok(UserNameList,"|");
		BanPlayerPACKET banplayerPacket;
		while(pUserName!=NULL)///�˺�
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
			//(*iter).szRoleName=RoleName;//��ɫ��
			sprintf((*iter).szRoleName,"%s",pRole);//RoleName
			pRole= strtok(NULL,"|");
			if(pRole==NULL)
				break;
		}
		
		CBanishPlayer BP;//ʵ�������˶���
		int length=0;
		operPal.FindAccountDBIP(accountdbip,ServerName);
		operPal.getRemoteSql("PAL","PAL_ACCOUNT_CLOSE",szRemoteSql,0);
		
		for (iter = v_banplayer.begin(); iter != v_banplayer.end(); iter++)//���ݵ�����ѭ�����
		{
			banplayerPacket = *iter;
			BP.BanishPlayerMain(ServerName,ServerIP,banplayerPacket.szRoleName);//banplayerPacket.szRoleName);//�������˽ӿ�
			Sleep(100);
			
			//operPal.FindAccountDBIP(accountdbip,ServerName);
			//operPal.getRemoteSql("PAL","PAL_ACCOUNT_CLOSE",szRemoteSql,0);
			
			///////Զ�̲���
			length=GameDBOper("PAL",accountdbip,0,szRemoteSql,banplayerPacket.szAccount);
			if(length>0)
			{
				length=GameDBOper("PAL","127.0.0.1",4,"exec Pal_Banishment_Close %i,'%s','%s','%s','%s'",
				userByID,ServerName,accountdbip,banplayerPacket.szAccount,Reason);//�������ݿ�����¼

				if (length>0)
				{
					sprintf(tmpMessage,"��ͣ�ʺ�:%s��ɫ��:%s�ɹ�;\r\n",banplayerPacket.szAccount,banplayerPacket.szRoleName);
					sprintf(retMessage,"%s%s",retMessage,tmpMessage);
				}
				else
				{
					sprintf(tmpMessage,"��ͣ�ʺ�:%s��ɫ��:%s���ز���ʧ��;\r\n",banplayerPacket.szAccount,banplayerPacket.szRoleName);
					sprintf(retMessage,"%s%s",retMessage,tmpMessage);
				}
			}
			else
			{
				sprintf(tmpMessage,"��ͣ�ʺ�:%s��ɫ��:%sʧ��;\r\n",banplayerPacket.szAccount,banplayerPacket.szRoleName);
				sprintf(retMessage,"%s%s",retMessage,tmpMessage);
			}
			
		}
		operPal.BuildTLV(retMessage);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RELOADFUNCTIONSWITCH_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		//logFile.WriteDBLog(userByID,"pal","PAL_ACCOUNT_CLOSE",ServerIP,"������ͣ����ʺ�");//retMessage);//maple add
		if (!v_banplayer.empty())
		{
			v_banplayer.clear();
		}
	}
	catch(...)
	{
		operPal.BuildTLV("������ͣʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RELOADFUNCTIONSWITCH_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}

//�����ʺŽ��
bool CPalAPI::Pal_OpenPlayer(int userByID,char * ServerName, char * ServerIP ,char * UserName,char * UserNick, char* Reason)
{
	try
	{
		char strlog[80];
		char accountdbip[60];
		char szRemoteSql[2048];
		char retMessage[256];
		ZeroMemory(strlog,60);
		sprintf(strlog,"����<%s>�������ʺ�<%s>",ServerName,UserName);
		logFile.WriteText("�ɽ�OL",strlog);
		
		int iState=operPal.FindAccountStatus(UserName);
		if(iState==1)
		{
			
			
			operPal.FindAccountDBIP(accountdbip,ServerName);//���ݴ���������ʺ�����ip
			operPal.getRemoteSql("PAL","PAL_ACCOUNT_OPEN",szRemoteSql,0);
			
			
			///////Զ�̲���
			int length=0;
			length=GameDBOper("PAL",accountdbip,0,szRemoteSql,UserName);//��Զ�����ݿ��н���ʺ�
			printf("length:%d++++++++accountdbip:%s, UserName:%s\n", length,accountdbip,UserName);
			if(length>0)
			{
				length=GameDBOper("PAL","127.0.0.1",4,"exec Pal_Banishment_OPEN %i,'%s','%s','%s','%s'",
					userByID,ServerName,accountdbip,UserName,Reason);//�ڱ������ݿ��н������ʺ�
				if(length>0)
				{
					//Added by tengjie 2010-08-02
					UserLogInterface(ServerName, UserName, "", 31, "0");

					sprintf(retMessage,"%s","���ɹ�");
				}
				else
				{
					sprintf(retMessage,"%s","���ʧ��,���ز���ʧ��");
				}
			}
			else
			{
				sprintf(retMessage,"%s","���ʧ��");
			}
			operPal.BuildTLV(retMessage);
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ACCOUNT_OPEN_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
			logFile.WriteDBLog(userByID,"pal","PAL_ACCOUNT_OPEN",Reason,retMessage);//maple add
		}
		else
		{
			operPal.BuildTLV("�޷�ͣ��Ϣ");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ACCOUNT_OPEN_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}

	}
	catch(...)
	{
		operPal.BuildTLV("���ʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ACCOUNT_OPEN_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}

//��ѯ��ͣ�б�
bool CPalAPI::CloseList(char * ServerName,char * ServerIP,int iIndex, int iPageSize)
{
	try
	{
		char strlog[60];
		char accoutdbip[60];
		sprintf(strlog,"����<%s>��ͣ�����ʺ�",ServerName);
		logFile.WriteText("�ɽ�OL",strlog);
		operPal.FindGameDBIP(accoutdbip,ServerName);//���ݴ�������ô���ip

		//����ip(���ϻ�õĴ���ip)�Ϸ�ͣ�������Ϣ
		vector <CGlobalStruct::TFLV> DBTFLV=GameDBQuery("PAL","127.0.0.1",4,iIndex,iPageSize,"exec Pal_Banishment_ALLQuery '%s'",accoutdbip);
		if(!DBTFLV.empty())
		{
			operPal.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operPal.BuildTLV("���ݿ�Ϊ��");			
		}
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ACCOUNTLOCAL_QUERY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	catch(...)
	{
		operPal.BuildTLV("��ѯ���ط�ͣʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ACCOUNTLOCAL_QUERY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}

	return true;
}

//��ѯ�����˺�
bool CPalAPI::CloseSingle(char * ServerName, char * ServerIP,char * UserName,int iIndex, int iPageSize)
{
	try
	{
		char strlog[80];
		ZeroMemory(strlog,80);
		sprintf(strlog,"����<%s>��ѯ����˺�<%s>�Ƿ��ͣ",ServerName,UserName);
		logFile.WriteText("�ɽ�OL",strlog);
		char accountdbip[60];
		operPal.FindGameDBIP(accountdbip,ServerName);//���ݴ�������ô���ip

		////����ip(���ϻ�õĴ���ip)������ʺ��Ƿ񱻷�ͣ
		vector <CGlobalStruct::TFLV> DBTFLV=GameDBQuery("PAL","127.0.0.1",4,iIndex,iPageSize,"exec  Pal_Banishment_Query '%s','%s'",accountdbip,UserName);
		if(!DBTFLV.empty())
		{
			operPal.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operPal.BuildTLV("���ݿ�Ϊ��");			
		}
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ACCOUNT_BANISHMENT_QUERY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	catch(...)
	{
		operPal.BuildTLV("��ѯ�˺ŷ�ͣʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_ACCOUNT_BANISHMENT_QUERY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}


//��ѯ����
bool CPalAPI::BroadTask_Query(int iIndex,int iPageSize)
{
	try
	{
		char strlog[60];
		ZeroMemory(strlog,60);
		sprintf(strlog,"��ѯ����");
		logFile.WriteText("�ɽ�OL",strlog);
		
		
		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("PAL", "127.0.0.1",4, iIndex, iPageSize,"exec Pal_BoardTask_NewQuery");
		if(!DBTFLV.empty())
		{
			operPal.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operPal.BuildTLV("���ݿ�Ϊ��");			
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
		operPal.BuildTLV("���ݿ�Ϊ��");			
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_BOARDTASK_QUERY_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		return TRUE;
		
	}
	return TRUE;
}

//���¹���
bool CPalAPI::BroadTask_Update(int userByID,int taskid,char * boardMessage,char * begintime,char * endtime,int interval,int status)
{
	try
	{
		char strlog[60];
		char retMessage[256];
		sprintf(strlog,"���¹����%i",taskid);
		logFile.WriteText("�ɽ�OL",strlog);

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
			sprintf(retMessage,"%s","������³ɹ�");
		}
		else
		{
			sprintf(retMessage,"%s","�������ʧ��");
		}
		operPal.BuildTLV(retMessage);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_BOARDTASK_UPDATE_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	catch(...)
	{
		operPal.BuildTLV("�������ʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_BOARDTASK_UPDATE_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}

//���빫��
bool CPalAPI::BroadTask_Insert(int userByID,char * ServerIP,char * GSServerIP,char * boardMessage,char * begintime,char * endtime,int interval)
{
	try
	{
		char strlog[2048];
		char retMessage[256];
		sprintf(strlog,"����<%s>����IP<%s>��ӹ���<%s>",ServerIP,GSServerIP,boardMessage);
		logFile.WriteText("�ɽ�OL",strlog);

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
			sprintf(retMessage,"%s","������ӳɹ�");
		}
		else
		{
			sprintf(retMessage,"%s","�������ʧ��");
		}
		operPal.BuildTLV(retMessage);
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_BOARDTASK_INSERT_RESP,operPal.GetSendBuf(),operPal.GetSendLength());

		
	}
	catch(...)
	{
		operPal.BuildTLV("�������ʧ��");
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_BOARDTASK_INSERT_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
	}
	return true;
}

// add by ltx
//���Ϳ�����Ʒ�ʼ�
void CPalAPI::Pal_SendCustomItemMail(int UserByID, char * szGMServerName,char * szGMServerIP,char * szRoleName,char * szSubject,
						char * szContent,int iMoney, int itemType, int iCustomItemID, char *szCustomItemIDName, bool bBindSetup, char *szInherit,char *szReason)
{
	try
	{
		CSendCustomItemMail SendCIM;//ʵ�������ͽ�Ʒ�ʼ�
		vector<CGlobalStruct::TFLV> DBTFLV=SendCIM.SendCustomItemMailMain(szGMServerName, szGMServerIP, szRoleName, szSubject,
						szContent, iMoney,  itemType, iCustomItemID, szCustomItemIDName, bBindSetup, szInherit,szReason);//���ü��ͽ�Ʒ�ʼ��ӿ�
		operPal.BuildTLV((COperVector)DBTFLV,0,0);//�������ݰ�
		operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_SENDCUSTOMITEMMAIL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		char strCItemMailLog[255];

		sprintf(strCItemMailLog, "��ɫ��<%s>,������Ʒ����:%s,��Ǯ��:%d", szRoleName, szCustomItemIDName, iMoney);

 		operPal.RecordData(UserByID,CEnumCore::Message_Tag_ID::PAL_SENDCUSTOMITEMMAIL,szGMServerName,
 			&COperVector(DBTFLV),strCItemMailLog);//��¼������־
	}
	catch(...)
	{
		operPal.BuildTLV("�ʼ�����ʧ��");
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
		
		CModifyAccountPw MAP;	//ʵ�����޸��ʺ��������
		char szMD5Password[50];
		ZeroMemory(szMD5Password,50);
		int errorno = 0;

		CSqlHelper m_SqlHelper;
		char szSql[256];
		sprintf(szSql,"exec Pal_FindAccountdbipAll");//�ڱ������ݿ��в����Ƿ�����Ҫ�ָ��ļ�¼
		
		m_SqlHelper.SqlHelperMain("PAL",&list_accountdbip, "127.0.0.1", szSql, 4, 0, 0);//��ȡSqlExpress�е���Ӧsql���
		
		if(list_accountdbip.empty())//�����¼Ϊ��
		{
			operPal.BuildTLV("������Ϣ��ȡʧ��");
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
			
			sprintf(strlog, "����<%s>�޸��ʺ�<%s>����", accountdbip, szAccount);
			logFile.WriteText("�ɽ�OL",strlog);
			
			int b_result = -1;

 			//�жϱ������ݿ����Ƿ�����Ҫ�޸ĵ��ʺ������¼(flag=1),���û�в���һ����ͬʱ�����������md5����
  			b_result=MAP.SaveAccountPw(szAccount,szAccountPw, accountdbip, szMD5Password);
  			if(b_result == 1) //��¼������
  			{
  				
  				b_result=MAP.PutAccountPw(szAccount, szMD5Password, accountdbip);//�����޸��ʺ�����
  				if(b_result)
  				{
  					sprintf(text,"�ʺ�<%s>����<%s>��ʱ�����޸ĳɹ�!������:%s", szAccount, accountdbip, szAccountPw);
					//Modified by tengjie 2010-07-08
//					operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPWALL_RESP, accountdbip,NULL,text);//��¼������־
					operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPWALL_RESP, ServerName,NULL,text);//��¼������־
  				}
  				else
  				{
  					sprintf(text,"�ʺ�<%s>����<%s>��ʱ�����޸�ʧ��!",szAccount, accountdbip);
					//Modified by tengjie 2010-07-08
//					operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPWALL_RESP, accountdbip, NULL,text);
					operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPWALL_RESP, ServerName, NULL,text);
  					errorno++;
  				}
  			}
 			else if(b_result == 0)	//��¼�Ѵ���
 			{
 				sprintf(text,"�ʺ�<%s>����<%s>��ʱ�����Ѿ��޸�!",szAccount, accountdbip);
				//Modified by tengjie 2010-07-08
//				operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPWALL_RESP, accountdbip, NULL,text);
				operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPWALL_RESP, ServerName, NULL,text);
 			}
 			else if (b_result == -1)  //Զ�̲�ѯ�������
 			{
 				sprintf(text,"�ʺ�<%s>����<%s>�ʺ���Ϣ������!",szAccount, accountdbip);
				printf("%s\n", text);
				//Modified by tengjie 2010-07-08
//				operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPWALL_RESP, accountdbip, NULL,text);
				operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPWALL_RESP, ServerName, NULL,text);
 			}
		}

		if (errorno == 0)
		{
			operPal.BuildTLV("��ʱ�����޸ĳɹ�");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYACCOUNTPWALL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}
		else
		{
			operPal.BuildTLV("���������޸�ʧ��");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_MODIFYACCOUNTPWALL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}

	}
	catch(...)
	{
		operPal.BuildTLV("��ʱ�����޸Ĵ���");
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
		sprintf(szSql,"exec Pal_FindOldAccountPwAll '%s'",szAccount);//�ڱ������ݿ��в����Ƿ�����Ҫ�ָ��ļ�¼

		m_SqlHelper.SqlHelperMain("PAL",&datarecord, "127.0.0.1", szSql, 4, 0, 0);//��ȡSqlExpress�е���Ӧsql���
		
		if(datarecord.empty())//�����¼Ϊ��
		{
			operPal.BuildTLV("û��������Ҫ�ָ�");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLACCOUNTPWALL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
			return;
		}

		CModifyAccountPw MAP;//ʵ�����޸��ʺ��������
		bool b_result=false;
		int errorno = 0;

		//��ȡ��ѯ�ļ�¼����ÿһ��
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("oldpasswd", m_DataRecord.recordfieldname))//��ȡSqlExpress�ж�Ӧsql_type��sql_statement
			{
				sprintf(szMD5Passwd,m_DataRecord.recorddata);//�����¼�����м�¼����ȡ��¼��ԭ������
			}
			else if (!strcmp("serverip", m_DataRecord.recordfieldname))
			{
				sprintf(accountdbip, m_DataRecord.recorddata);//�����¼�����м�¼����ȡ�����Ӧ����				

				sprintf(strlog,"����<%s>��ԭ�ʺ�<%s>����",accountdbip, szAccount);
				logFile.WriteText("�ɽ�OL",strlog);
				
				b_result=MAP.PutAccountPw(szAccount, szMD5Passwd, accountdbip);//�����޸��ʺ�����
				if(b_result)
				{
					sprintf(text,"�ʺ�<%s>����<%s>��ԭ����ɹ�",szAccount, accountdbip);
					//Modified by tengjie 2010-07-08
//					operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTPWALL_RESP,accountdbip,NULL,text);//��¼������־	
					operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTPWALL_RESP,ServerName,NULL,text);//��¼������־	
					
					sprintf(szSql,"exec Pal_UpdateAccountPwFlag '%s', '%s'",szAccount, accountdbip);//�޸ļ�¼��flagֵΪ0����ԭ��
					m_SqlHelper.SqlHelperMain("127.0.0.1", szSql, 4);//��ȡSqlExpress�е���Ӧsql���
				}
				else
				{
					sprintf(text,"�ʺ�<%s>����<%s>��ԭ����ʧ��",szAccount, accountdbip);
					//Modified by tengjie 2010-07-08
//					operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTPWALL_RESP,accountdbip,NULL,text);
					operPal.RecordData(userByID,CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTPWALL_RESP,ServerName,NULL,text);
					errorno++;
				}
			}
		}
		
		//���޸�ʧ�ܵ�
		if (errorno == 0)
		{
			operPal.BuildTLV("��������ָ��ɹ�");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLACCOUNTPWALL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}
		else
		{
			operPal.BuildTLV("��������ָ�ʧ��");
			operPal.SendBuf(pGameSess,CEnumCore::Msg_Category::PAL_ADMIN,CEnumCore::ServiceKey::PAL_RECALLACCOUNTPWALL_RESP,operPal.GetSendBuf(),operPal.GetSendLength());
		}

	}
	catch(...)
	{
		operPal.BuildTLV("����ָ�����");
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
// 	//��ȡ��ϷIP
// 	char PalServerIp[20]= "";
// 	char PalUrl[128]="";
// 	GetPrivateProfileString("server","PalServerIp","61.152.150.158",PalServerIp,20,_temppath.c_str());//����һ��
// 	GetPrivateProfileString("url","PalUrl","passport/games/gmtools_ban_log_w.php",PalUrl,128,_temppath.c_str());//WA���յ�ַ
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
// 		if(!strcmp("idx", m_DataRecord.recordfieldname))//��ȡ��Ӧ��ServerIP
// 		{
// 			szData.Format("%s", m_DataRecord.recorddata);//�����ѯ�õ������һ����¼
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
// // 		//asciiת����unicode
// // 		int  wcsLen  =  ::MultiByteToWideChar(CP_ACP, NULL, you9Nickname.c_str(), you9Nickname.length(), NULL,  0 );
// // 		wchar_t *  wszString  =   new  wchar_t[wcsLen  +   1 ];
// // 		::MultiByteToWideChar(CP_ACP, NULL, you9Nickname.c_str(), you9Nickname.length(), wszString, wcsLen);
// // 		wszString[wcsLen]  = '\0' ;
// // 		
// // 		//unicodeת����utf8
// // 		int u8Len = ::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);
// // 		char* szU8 = new char[u8Len + 1];
// // 		::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), szU8, u8Len, NULL, NULL);
// // 		szU8[u8Len] = '\0';
// 		
// 		int length=0;
// 		
// 		string strMd5 = NewUserName;
// 		strMd5 += "|T4pb5A.gmtoolbanlog";
// 		//md5����
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
// 		while(lpHttpFile->ReadString(strSentence))    // ��ȡ�ύ���ݺ�ķ��ؽ��
// 		{
// 			strGetSentence = strGetSentence + strSentence + char(13) + char(10);
// 		}
// 		
// 		printf("���ص�����:%s\n",strGetSentence);
// 		
// 		delete []szMd5;
// 	}
// 
// 	return strGetSentence;
// }