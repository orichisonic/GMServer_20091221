// Pal.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Pal.h"
//#include "PalManagerModule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CPalApp

BEGIN_MESSAGE_MAP(CPalApp, CWinApp)
	//{{AFX_MSG_MAP(CPalApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPalApp construction

CPalApp::CPalApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPalApp object
extern "C"  bool __declspec(dllexport) Pal_Main(CSession * pSess,SOCKET m_socket,CEnumCore::Msg_Category m_category,CEnumCore::Message_Tag_ID m_message_tag,unsigned char * pbuf,int length)
{
		HANDLE m_Mutex;
		m_Mutex=CreateMutex(NULL, FALSE, "PAL_MAIN");//���û�������PAL_MAIN���ýӿ�ͨ��
		WaitForSingleObject(m_Mutex,INFINITE);//���߳�

		CPalAPI PalAPI;
		COperVector * precvVect=PalAPI.initialize(pSess,m_message_tag,pbuf, length);//��ʼ�������
		if(!precvVect)
		{
			return FALSE;
		}

		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_CHARACTERINFO)//��ѯ��ɫ������Ϣ
			{
				try
				{
					PalAPI.UserInfo(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_BANISHPLAYER)//����
			{
				try
				{
					PalAPI.Pal_KickPlayer(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata//��rolename�����˺�
						);

				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_CHARACTERBAG)//��ѯ��ұ���
			{
				try
				{
					PalAPI.Pal_CharacterBag(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_BAGTYPE,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_CHARACTERFRIENDLIST)//ȡ�ý�ɫ�����б�
			{
				try
				{
					PalAPI.Pal_CharacterFriendList(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_CHARACTERQUEST)//ȡ�ý�ɫ������Ѷ
			{
				try
				{
					PalAPI.Pal_CharacterQuest(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_QUESTTYPE,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_GUILDBASEINFO)//ȡ�ù��������Ѷ
			{
				try
				{
					PalAPI.Pal_GuileBaseInfo(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GUILDNAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_GUILDMEMBERLIST)//ȡ�ù����Ա�б�
			{
				try
				{
					PalAPI.Pal_GuildMemberList(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GUILDNAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_SENDGIFTMAIL)//���ͽ�Ʒ�ʼ�
			{
				try
				{
					PalAPI.Pal_SendGiftMail(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILSUBJECT,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILCONTENT,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILMONEY,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILGIFTDBID0,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILGIFTDBID1,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILGIFTDBID2,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILGIFTDBID3,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILGIFTDBID4,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILCONTENT,0)).lpdata);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_DISMISSGUILD)//��ɢ����
			{
				try
				{
					PalAPI.Pal_DismissGuild(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GUILDNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_REASON,0)).lpdata);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MODIFYGUILDNAME)//�޸Ĺ�������
			{
				try
				{
					PalAPI.Pal_ModifyGuildName(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GUILDNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_NEWGUILDNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_REASON,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MODIFYGUILDMEMBERPL)//�޸Ĺ����Աְ��
			{
				try
				{
					PalAPI.Pal_ModifyGuildMemberPL(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GUILDNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GUILDPOWERLEVEL,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_REASON,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MODIFYCHARACTERQUEST)//�޸Ľ�ɫ����
			{
				try
				{
					PalAPI.Pal_ModifyCharacterQuest(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_QUESTID,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_QUESTTYPE,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MODIFYCHARACTERMONEY)//�޸Ľ�ɫ��Ǯ
			{
				try
				{
					PalAPI.Pal_ModifyCharacterMoney(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MONEYTYPE,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MONEYCOUNT,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MODIFYCHARACTERDROPPW)//�޸Ľ�ɫɾ������
			{
				try
				{
					PalAPI.Pal_ModifyCharacterDropPw(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_DROPROLEPW,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_RECALLCHARACTERDROPPW)//��ԭ��ɫɾ������
			{
				try
				{
					PalAPI.Pal_RecallCharacterDropPw(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTBANKPW)//�޸��ʺ���������
			{
				try
				{
					PalAPI.Pal_ModifyAccountBankPw(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_BANKPW,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTBANKPW)//��ԭ�ʺ���������
			{
				try
				{
					PalAPI.Pal_RecallAccountBankPw(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPW)//�޸��ʺ�����
			{
				try
				{
					PalAPI.Pal_ModifyAccountPw(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNTPW,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTPW)//��ԭ�ʺ�����
			{
				try
				{
					PalAPI.Pal_RecallAccountPw(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MODIFYSECONDPW)//�޸Ķ�������
			{
				try
				{
					PalAPI.Pal_ModifySecondPw(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_SECONDPW,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_RECALLSECONDPW)//��ԭ��������
			{
				try
				{
					PalAPI.Pal_RecallSecondPw(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_USESAFELOCK)//������ȫ��
			{
				try
				{
					PalAPI.Pal_UseSafeLock(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_FREESAFELOCK)//����������
			{
				try
				{
					PalAPI.Pal_FreeSafeLock(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_DELCHARACTERITEM)//ɾ����ɫ��Ʒ��Ӧ
			{
				try
				{
					PalAPI.Pal_DelCharacterItem(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_BAGTYPE,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ITEMPOS,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ITEMNAME,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MODIFYGUILDLV)//�޸Ĺ���ȼ�
			{
				try
				{
					PalAPI.Pal_ModifyGuildLv(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GUILDNAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GUILDLEVEL,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_REASON,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MODIFYGUILDNOTICE)//�޸Ĺ��ṫ��
			{
				try
				{
					PalAPI.Pal_ModifyGuildNotice(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GUILDNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GUILDLEVEL,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_REASON,0)).lpdata);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_ACCOUNTLOCAL_QUERY)//���ط�ͣ������ʺŲ�ѯ
			{
				try
				{
					PalAPI.CloseList(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_ACCOUNT_CLOSE)//��ͣ������ʺ�(�ʺ��ҽ�ɫ)
			{
				try
				{
					PalAPI.Pal_ClosePlayer(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_REASON,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_ACCOUNT_OPEN)//��������ʺ�
			{
				try
				{
					PalAPI.Pal_OpenPlayer(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_REASON,0)).lpdata
						);
				}
				catch (...)
				{
				return false;
				}
			}
		//maple update
		//������ͣ����ʺ�
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_ACCOUNT_BANISHMENT_QUERY)//��ҷ�ͣ�ʺŲ�ѯ
			{
				try
				{
					PalAPI.CloseSingle(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
					
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_BOARDTASK_INSERT)//�������
			{
				try
				{
					PalAPI.BroadTask_Insert(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,//����������
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_WORLDID,0)).lpdata,//������worldID
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_BOARDMESSAGE,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_BEGINTIME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ENDTIME,0)).lpdata,
						StrToInt((LPCSTR)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_INTERVAL,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_BOARDTASK_QUERY)//�����ѯ
			{
				try
				{
					//�����ѯ
					PalAPI.BroadTask_Query(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_BOARDTASK_UPDATE)//�������
			{
				try
				{
					PalAPI.BroadTask_Update(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						StrToInt((LPCSTR)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_TASKID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_BOARDMESSAGE,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_BEGINTIME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ENDTIME,0)).lpdata,
						StrToInt((LPCSTR)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_INTERVAL,0)).lpdata),
						StrToInt((LPCSTR)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_STATUS,0)).lpdata));
				}
				catch (...)
				{
					return false;
				}
			}
			if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_ITEMTYPE_QUERY)//��������ѯ
			{
				try
				{
					PalAPI.Pal_ItemType_Query();
				}
				catch (...)
				{
					return false;
				}
			}
			if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_ITEMNAME_QUERY)//�������Ʋ�ѯ
			{
				try
				{
					PalAPI.Pal_ItemName_Query(
						StrToInt((LPCSTR)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ITEMBIGTYPEID,0)).lpdata));
				}
				catch (...)
				{
					return false;
				}
			}
			if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_SendBankPwd_Query)//���Ͳֿ�����
			{
				try
				{
					PalAPI.PAL_SendBankPwd_Query(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_UserMailBox,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
			if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_SendDeletePwd_Query)//���ͽ�ɫɾ������
			{
				try
				{
					PalAPI.PAL_SendDeletePwd_Query(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_UserMailBox,0)).lpdata
						);
				}
				catch (...)
				{
					return false;
				}
			}
			if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MAILBOX_QUERY)//�����ѯ
			{
				try
				{
					PalAPI.Pal_Get_EmailAddress((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata);
				}
				catch (...)
				{
					return false;
				}
			}
			if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_CHARACTERADVANCEINFO)//ȡ�ý�ɫ������Ѷ
			{
				try
				{
					PalAPI.Pal_CharacterAdvanceInfo(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
			if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_CHARACTERSKILL)//ȡ�ý�ɫ������Ѷ
			{
				try
				{
					PalAPI.Pal_CharacterSkill(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_CHARACTERMOB)//ȡ�ý�ɫ��ħ��Ѷ
			{
				try
				{
					PalAPI.Pal_CharacterMob(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_CHARACTERPUZZLEMAP)//ȡ�ý�ɫͼ����Ѷ
			{
				try
				{
					PalAPI.Pal_CharacterPuzzleMap(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_CHARACTERBLACKLIST)//ȡ�ý�ɫ�������б�
			{
				try
				{
					PalAPI.Pal_CharacterBlackList(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_DELCHARACTERMOB)//ɾ����ɫ��ħ
			{
				try
				{
					PalAPI.Pal_DelCharacterMob(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MobID,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MODIFYPUZZLEMAP)//�޸Ľ�ɫͼ��
			{
				try
				{
					PalAPI.Pal_ModifyPuzzleMap(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_PuzzleMap,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_PuzzleMapID1,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_PuzzleMapID2,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_PuzzleMapID3,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_PuzzleMapID4,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_PuzzleMapID5,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_PuzzleMapID6,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_PuzzleMapID7,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_PuzzleMapID8,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_PuzzleMapID9,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
	/*	if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_ACCOUNTINFO)//ȡ���ʺ���Ѷ
			{
				try
				{
					PalAPI.Pal_AccountInfo(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}*/
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MOVECHARACTERTOSAFE)//�ƶ���ɫ����ȫ��
			{
				try
				{
					PalAPI.Pal_MoveCharacterToSafe(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MapID,0)).lpdata)
						);
				}
				catch (...)
				{
					return false;
				}
			}
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_RELOADFUNCTIONSWITCH)//������ͣ����ʺ� maple update//�������빦�ܿ����б�
			{
				try
				{
					//PalAPI.Pal_ReloadFunctionSwitch();//maple update
					PalAPI.Pal_BanMultiPlayer(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_REASON,0)).lpdata);
				}
				catch (...)
				{
					return false;
				}
			}
		// add by ltx
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_SENDCUSTOMITEMMAIL)//���Ϳ�����Ʒ�ʼ�
		{
			try
			{
				PalAPI.Pal_SendCustomItemMail(
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILSUBJECT,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILCONTENT,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILMONEY,0)).lpdata),
					0,//ItemType
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_CustomIterID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_CustomIter,0)).lpdata,
					(bool)StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_BindSetup,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_INHERITID0_NAME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILREASON,0)).lpdata);
			}
			catch (...)
			{
				return false;
			}
		}

		// add by ltx
		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPWALL)
		{
			//Modified by tengjie 2010-07-08
// 			PalAPI.Pal_ModifyAccountPwAll(
// 				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
// 				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
// 				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNTPW,0)).lpdata);
			PalAPI.Pal_ModifyAccountPwAll(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNTPW,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata);
		}

		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTPWALL)
		{
			//Modified by tengjie 2010-07-08
// 			PalAPI.Pal_RecallAccountPwAll(
// 				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
// 				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata);
			PalAPI.Pal_RecallAccountPwAll(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata);
		}

		if(m_message_tag==CEnumCore::Message_Tag_ID::PAL_SearchMap_Query)//��ѯ��ͼ
			{
				try
				{
					PalAPI.PAL_SearchMap_Query();
				}
				catch (...)
				{
					return false;
				}
			}
		else
		{
			goto FALSE_FUNC;
		}
		if(!PalAPI.Destroy())
		{
			return FALSE;
		}
		ReleaseMutex(m_Mutex);//����
		CloseHandle(m_Mutex);
		return TRUE;
FALSE_FUNC:
		PalAPI.Destroy();
		ReleaseMutex(m_Mutex);//����
		CloseHandle(m_Mutex);
		return FALSE;
}

bool Pal_BanishPlayer(char * ServerIP,char * ServerName,char * pal_userNick)
{
	CPalAPI PalAPI;
	PalAPI.Pal_BanishPlayer(ServerIP,ServerName,pal_userNick);
	return TRUE;
}

CPalApp theApp;
