// GT.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "GT.h"

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
// CGTApp

BEGIN_MESSAGE_MAP(CGTApp, CWinApp)
	//{{AFX_MSG_MAP(CGTApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGTApp construction

CGTApp::CGTApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGTApp object
extern "C"  bool __declspec(dllexport) GT_Main(CSession * pSess,SOCKET m_socket,CEnumCore::Msg_Category m_category,CEnumCore::Message_Tag_ID m_message_tag,unsigned char * pbuf,int length)
{
	CLogFile logFile;
	char errMessage[256];
	try
	{
		CGTApp theApp;
		CGTInfo GTInfo;
		COperatorGT operGT;
		CLogFile logFile;
		vector <CGlobalStruct::TFLV> DBTFLV;
		CEnumCore::ServiceKey m_ServiceKey= (m_message_tag & 0x00FFFF) | 0x8000;//��ȡҪ��Ӧ��CEnumCore::ServiceKey
		COperVector * precvVect=GTInfo.initialize(pSess,m_message_tag, pbuf, length);

		if(!precvVect)//��ʼ�������
		{
			return false;
		}
		switch((unsigned int)m_message_tag)
		{
			case CEnumCore::Message_Tag_ID::GT_UserInfo_Query://��ҽ�ɫ��Ϣ
				{
					DBTFLV=theApp.UserInfo(precvVect);
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_User_DetailInfo_Query://��ϸ��Ϣ
			case CEnumCore::Message_Tag_ID::GT_User_FamilyInfo_Query://������Ϣ
			case CEnumCore::Message_Tag_ID::GT_User_SentiInfo_Query://������Ϣ
			case CEnumCore::Message_Tag_ID::GT_User_MarrigeInfo_Query://������Ϣ
			case CEnumCore::Message_Tag_ID::GT_UserPack_Query://������Ʒ������Ϣ
			case CEnumCore::Message_Tag_ID::GT_UserEquip_Query://��ɫװ����Ϣ
			case CEnumCore::Message_Tag_ID::GT_IMFriend_Query://������Ϣ
			case CEnumCore::Message_Tag_ID::GT_IMBlackList_Query://��������Ϣ
			case CEnumCore::Message_Tag_ID::GT_IMSecretLove_Query://����������Ϣ
			case CEnumCore::Message_Tag_ID::GT_PetInfo_Query://������Ϣ��û�У�
			case CEnumCore::Message_Tag_ID::GT_RoomInfo_Query://������Ϣ
			case CEnumCore::Message_Tag_ID::GT_FriendTrade_Info://���������н�ɫ��Ϣ
			case CEnumCore::Message_Tag_ID::GT_HelotInfo_Query://ū����Ϣ
				{
					DBTFLV=theApp.UserProperty(precvVect,m_message_tag);	
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_FriendTrade_Log://������־
				{
					DBTFLV=GTInfo.GT_FriendTrade_Log(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Account,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_UserID,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_AllStopUser_Query://��ֹͼƬ�б�
				{
					DBTFLV=GTInfo.CloseList
						("GT","exec GT_AllStopUser '%s'",
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata))
						;
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_AllBanUser_Query://��ͣ����б�
				{
					DBTFLV=GTInfo.CloseList
						("GT","exec GT_AllBanUser '%s'",
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata))
						;
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_BanUser_Query:////��ѯ��������Ƿ��ͣ
				{
					DBTFLV=GTInfo.CloseSingle("GT",
					"exec GT_UserState_Query '%s','%s'",
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Account,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_FamilyInfo_Query://������Ϣ
				{
					DBTFLV=GTInfo.GT_FamilyInfo(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_FamilyName,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_MemberInfo_Query://�����Ա��Ϣ
				{
					DBTFLV=GTInfo.GT_MemberInfo(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_FamilyName,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_FamilyID,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_MailLog_Query://������־
			case CEnumCore::Message_Tag_ID::GT_TradeLog_Query://������־
			case CEnumCore::Message_Tag_ID::GT_TaskLog_Query://������־
			case CEnumCore::Message_Tag_ID::GT_UseItemLog_Query://ʹ�õ�����־
			case CEnumCore::Message_Tag_ID::GT_GuajiLog_Query://�һ���־
			case CEnumCore::Message_Tag_ID::GT_BugleLog_Query://��С������־
			case CEnumCore::Message_Tag_ID::GT_LoginLog_Query://��½��־
				{
					DBTFLV=theApp.LogInfo(precvVect,m_message_tag);
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_MailInfo_Query://�ʼ���Ϣ
				{
					DBTFLV=GTInfo.GT_MailInfo(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Account,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_UserID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_StartTime,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_EndTime,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_TreasureBox_Query://������Ϣ
				{
					DBTFLV=GTInfo.GT_TreasureBox_Open(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Account,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_StartTime,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_EndTime,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_ItemConsume_Query://�������������Ϣ
				{
					DBTFLV=GTInfo.GT_ItemBuy(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Account,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_UserID,0)).lpdata),
						///0û�е������м�����ͷ�,1û�е��������������ͷ�,2û�е������м�����ܷ�,3û�е��������������ܷ�
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Type,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ItemName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_StartTime,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_EndTime,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_Select_Item://����ģ����ѯ
				{
					DBTFLV=GTInfo.GT_Select_Item(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ItemName,0)).lpdata
						);
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_Mcoin_Rank://Ѫƴս��
				{
					DBTFLV=GTInfo.GT_Mcoin_Rank(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
						//////0�������ѣ�1�������ѣ�2��ʷ����
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Type,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_Sns_Query://�ռ�����
				{
					DBTFLV=GTInfo.GT_Sns_Query(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
						/////0�ռ���£�1�ռ�����2�ռ�ظ�
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Type,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_Attr_Queue://��������
				{
					DBTFLV=GTInfo.GT_Attr_Queue(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
						//0������1������2������3���նȣ�4���˶ȣ�5�ۺ�ָ��
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Type,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_Card_Record://��Ƭ����
				{
					DBTFLV=GTInfo.GT_Card_Record(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
						//0���ͣ�1��ȡ��2�����3PK��4��������
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Type,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_FriendTrade_Queue://������Ϣ
				{
					DBTFLV=GTInfo.GT_FriendTrade_Queue(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
						//0������1���±���2�ʲ���࣬3�ֽ���࣬4�ۿ����
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Type,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_Back_Money://�˿����Ϣ
				{
					//�û�ID,���������֣�������IP���û�ID���û�����Money ����
					DBTFLV=GTInfo.GT_Back_Money(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Account,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ID,0)).lpdata),
						//0,2,4,6�м���Ƿ��˿�,�����������Ƿ��˿�
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Type,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_MCash,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ItemName,0)).lpdata
						);
				}
				break;
			default:
				break;
			
		}

		if(!DBTFLV.empty())
		{
			operGT.BuildTLV(COperVector(DBTFLV),m_ServiceKey,(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata);
		}
		else
		{
			operGT.BuildTLV("���ݿ�Ϊ��");
		}
		operGT.SendBuf(GTInfo.pGameSess,CEnumCore::Msg_Category::GT_ADMIN,m_ServiceKey,operGT.GetSendBuf(),operGT.GetSendLength());

		//���
		if(!GTInfo.Destroy())
		{
			return false;
		}
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��Ҫ�����ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��Ҫ�����ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��Ҫ���������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return false;
	}
	return true;
}

//��ҽ�ɫ��Ϣ
vector <CGlobalStruct::TFLV> CGTApp::UserInfo(COperVector * precvVect)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGTInfo GTInfo;
	CLogFile logFile;
	char errMessage[256];
	try
	{
		DBTFLV=GTInfo.UserInfo(
			"GT","GT_UserInfo_Account_New","GT_UserInfo_Nick_New",
			(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
			(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
			(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Account,0)).lpdata,
			(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_UserNick,0)).lpdata,
			0,
			StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
			StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
			);//��ѯ���������Ϣ
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ�û������ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ�û������ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ�û����������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	return DBTFLV;
	
}

//���������Ϣ��ͳһ��ѯ��userIDΪΨһ��ʾ����
vector <CGlobalStruct::TFLV> CGTApp::UserProperty(COperVector * precvVect,CEnumCore::Message_Tag_ID m_message_tag)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGTInfo GTInfo;
	COperatorGT operGT;
	CLogFile logFile;
	char errMessage[256];
	try
	{
		char sql_type[256];
		operGT.TagIDToSqlType(m_message_tag,sql_type);//����msg_tag_ID�����Ӧ��sql���
		DBTFLV=GTInfo.UserDefault("GT",sql_type,///��ѯ���������Ϣ��userIDΪΨһ��ʾ����
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Account,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_UserID,0)).lpdata),
				0,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ�û����������ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ�û����������ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ�û��������������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	return DBTFLV;
	
}
//��ѯ��ҵ���־��Ϣ
vector <CGlobalStruct::TFLV> CGTApp::LogInfo(COperVector * precvVect,CEnumCore::Message_Tag_ID m_message_tag)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGTInfo GTInfo;
	COperatorGT operGT;
	CLogFile logFile;
	char errMessage[256];
	try
	{
		char sql_type[256];
		operGT.TagIDToSqlType(m_message_tag,StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Type,0)).lpdata),sql_type);//����Msg_Tag_ID���sql���
		DBTFLV=GTInfo.BaseLog("GT",sql_type,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_UserNick,0)).lpdata,
				0,
				1,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_EndTime,0)).lpdata,
				0,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
				);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ��־�����ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ��־�����ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ��־���������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	return DBTFLV;
	
}
CGTApp theApp;


