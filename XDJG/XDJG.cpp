// XDJG.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "XDJG.h"

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
// CXDJGApp

BEGIN_MESSAGE_MAP(CXDJGApp, CWinApp)
	//{{AFX_MSG_MAP(CXDJGApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXDJGApp construction

CXDJGApp::CXDJGApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CXDJGApp object
extern "C"  bool __declspec(dllexport) XDJG_Main(CSession * pSess,SOCKET m_socket,CEnumCore::Msg_Category m_category,CEnumCore::Message_Tag_ID m_message_tag,unsigned char * pbuf,int length)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	try
	{
//		CXDJGApp theApp;
		CXDJGInfo XDJGInfo;
		COperatorXDJG operXDJG;
		vector <CGlobalStruct::TFLV> DBTFLV;

		//��ȡҪ��Ӧ��CEnumCore::ServiceKey
		CEnumCore::ServiceKey m_ServiceKey= (m_message_tag & 0x00FFFF) | 0x8000;

		//��ʼ�������
		COperVector * precvVect = XDJGInfo.initialize(pSess,m_message_tag, pbuf,length);

		if(!precvVect)
		{
			return false;
		}

		switch((unsigned int)m_message_tag)
		{
		//��ѯ���������Ϣ
		case CEnumCore::Message_Tag_ID::XDJG_Account_Query:
			DBTFLV = XDJGInfo.XDJG_Net_UserInfo((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_SEARCHTYPE,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//���������
		case CEnumCore::Message_Tag_ID::XDJG_KickPlayer:
			DBTFLV = XDJGInfo.XDJG_Net_KickPlayer(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata);
			break;

		//����ʺŷ�ͣ
		case CEnumCore::Message_Tag_ID::XDJG_ClosePlayer:
			DBTFLV = XDJGInfo.XDJG_Net_ClosePlayer(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_AccountID,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BANTIME,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BANREASON,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BANTYPE,0)).lpdata));
			break;

		//����ʺŽ��
		case CEnumCore::Message_Tag_ID::XDJG_OpenPlayer:
			DBTFLV = XDJGInfo.XDJG_Net_OpenPlayer(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BANREASON,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BANTYPE,0)).lpdata));
			break;

		//��ѯ��ͣ�б�
		case CEnumCore::Message_Tag_ID::XDJG_CloseList:
			DBTFLV = XDJGInfo.XDJG_Net_ClosePlayer_List((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//��ӹ���
		case CEnumCore::Message_Tag_ID::XDJG_BoardTasker_Add:
			DBTFLV = XDJGInfo.XDJG_Net_BoardTasker_Add(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BoardMessage,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BeginTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Interval,0)).lpdata));
			break;

		//�޸Ĺ���
		case CEnumCore::Message_Tag_ID::XDJG_BoardTasker_Update:
			DBTFLV = XDJGInfo.XDJG_Net_BoardTasker_Update(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_TaskID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BoardMessage,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BeginTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Interval,0)).lpdata));
			break;

		//ɾ������
		case CEnumCore::Message_Tag_ID::XDJG_BoardTasker_Del:
			DBTFLV = XDJGInfo.XDJG_Net_BoardTasker_Delete(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_TaskID,0)).lpdata));
			break;

		//��ѯ����
		case CEnumCore::Message_Tag_ID::XDJG_BoardTasker_Query:
			DBTFLV=XDJGInfo.XDJG_Net_BoardTasker_Query("XDJG","exec XDJG_BOARDTASK_QUERY",
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//Added by tengjie 2010-07-06
		//��־��:��ҵ�¼��־
		case CEnumCore::Message_Tag_ID::XDJG_Login_Log:
			DBTFLV = XDJGInfo.XDJG_DB_Login_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_LogInOutIP,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//�̳���Ʒ������־
		case CEnumCore::Message_Tag_ID::XDJG_Shoping_Log:
			DBTFLV = XDJGInfo.XDJG_DB_Shoping_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_QueryType,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//�����Ʒ��־:���׻��
		case CEnumCore::Message_Tag_ID::XDJG_TransGain_Log:
			DBTFLV = XDJGInfo.XDJG_DB_TransGain_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//�����Ʒ��־:����ʧȥ
		case CEnumCore::Message_Tag_ID::XDJG_TransLose_Log:
			DBTFLV = XDJGInfo.XDJG_DB_TransLose_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//�����Ʒ��־:��NPC������
		case CEnumCore::Message_Tag_ID::XDJG_BuyFromNpc_Log:
			DBTFLV = XDJGInfo.XDJG_DB_BuyFromNpc_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//�����Ʒ��־:����NPC
		case CEnumCore::Message_Tag_ID::XDJG_SaleToNpc_Log:
			DBTFLV = XDJGInfo.XDJG_DB_SaleToNpc_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//�����Ʒ��־:ʰȡ��Ʒ
		case CEnumCore::Message_Tag_ID::XDJG_PickUpItem_Log:
			DBTFLV = XDJGInfo.XDJG_DB_PickUpItem_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//�����Ʒ��־:������Ʒ
		case CEnumCore::Message_Tag_ID::XDJG_DiscardItem_Log:
			DBTFLV = XDJGInfo.XDJG_DB_DiscardItem_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//�����Ʒ��־:��Ʒ����
		case CEnumCore::Message_Tag_ID::XDJG_ConsumeItem_Log:
			DBTFLV = XDJGInfo.XDJG_DB_ConsumeItem_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//�����Ʒ��־:����������
		case CEnumCore::Message_Tag_ID::XDJG_AuctioneerSaled_Log:
			DBTFLV = XDJGInfo.XDJG_DB_AuctioneerSaled_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//�����Ʒ��־:�����й���
		case CEnumCore::Message_Tag_ID::XDJG_AuctioneerBought_Log:
			DBTFLV = XDJGInfo.XDJG_DB_AuctioneerBought_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//����ʼ���־:�����ʼ�
		case CEnumCore::Message_Tag_ID::XDJG_SendMail_Log:
			DBTFLV = XDJGInfo.XDJG_DB_SendMail_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//����ʼ���־:��ȡ�ʼ�
		case CEnumCore::Message_Tag_ID::XDJG_ReceiveMail_Log:
			DBTFLV = XDJGInfo.XDJG_DB_ReceiveMail_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//M��ʹ����־:�����������Ѽ�¼
		case CEnumCore::Message_Tag_ID::XDJG_ExpansionPack_Log:
			DBTFLV = XDJGInfo.XDJG_DB_ExpansionPack_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//M��ʹ����־:������־
		case CEnumCore::Message_Tag_ID::XDJG_Deliver_Log:
			DBTFLV = XDJGInfo.XDJG_DB_Deliver_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//M��ʹ����־:��ɫ�ֿ��������Ѽ�¼
		case CEnumCore::Message_Tag_ID::XDJG_ExpansionWarehouse_Log:
			DBTFLV = XDJGInfo.XDJG_DB_ExpansionWarehouse_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//��ҽ�ɫ��־:��ɫ����
		case CEnumCore::Message_Tag_ID::XDJG_RoleUpgrade_Log:
			DBTFLV = XDJGInfo.XDJG_DB_RoleUpgrade_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//��ҽ�ɫ��־:��ɫ����
		case CEnumCore::Message_Tag_ID::XDJG_RoleCreate_Log:
			DBTFLV = XDJGInfo.XDJG_DB_RoleCreate_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//��ҽ�ɫ��־:��ɫ����ѧϰ
		case CEnumCore::Message_Tag_ID::XDJG_RoleSkill_Log:
			DBTFLV = XDJGInfo.XDJG_DB_RoleSkill_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//��ҽ�ɫ��־:��ȡ����
		case CEnumCore::Message_Tag_ID::XDJG_ReceiveTask_Log:
			DBTFLV = XDJGInfo.XDJG_DB_ReceiveTask_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//��ҽ�ɫ��־:�������
		case CEnumCore::Message_Tag_ID::XDJG_CompleteTask_Log:
			DBTFLV = XDJGInfo.XDJG_DB_CompleteTask_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//��ҽ�ɫ��־:������ѯ
		case CEnumCore::Message_Tag_ID::XDJG_Copy_Log:
			DBTFLV = XDJGInfo.XDJG_DB_Copy_Log(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//��ѯ��
		//�����Ʒ��Ϣ:������Ʒ��Ϣ
		case CEnumCore::Message_Tag_ID::XDJG_ItemBody_Query:
			DBTFLV = XDJGInfo.XDJG_DB_ItemBody_Query(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
			
		//�����Ʒ��Ϣ:������Ʒ��Ϣ
		case CEnumCore::Message_Tag_ID::XDJG_Itembackpack_Query:
			DBTFLV = XDJGInfo.XDJG_DB_Itembackpack_Query(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
			
		//�����Ʒ��Ϣ:�ֿ���Ʒ��Ϣ
		case CEnumCore::Message_Tag_ID::XDJG_ItemStorage_Query:
			DBTFLV = XDJGInfo.XDJG_DB_ItemStorage_Query(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
			
		//�����Ʒ��Ϣ:����ֿ���Ϣ
		case CEnumCore::Message_Tag_ID::XDJG_GuildStorage_Query:
			DBTFLV = XDJGInfo.XDJG_DB_GuildStorage_Query(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
			
		//�����Ʒ��Ϣ:��ղֿ���Ϣ
		case CEnumCore::Message_Tag_ID::XDJG_KingkongStorage_Query:
			DBTFLV = XDJGInfo.XDJG_DB_KingkongStorage_Query(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
			
		//�ʼ���Ϣ
		case CEnumCore::Message_Tag_ID::XDJG_MailInfo_Query:
			DBTFLV = XDJGInfo.XDJG_DB_MailInfo_Query(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
			
		//��������Ϣ
		case CEnumCore::Message_Tag_ID::XDJG_AuctioneerInfo_Query:
			DBTFLV = XDJGInfo.XDJG_DB_AuctioneerInfo_Query(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//������
		//��ɫ�������
		case CEnumCore::Message_Tag_ID::XDJG_RoleSkillAdd:
			DBTFLV = XDJGInfo.XDJG_Net_RoleSkillAdd(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_SkillID,0)).lpdata));
			break;
		//��ɫ����ɾ��
		case CEnumCore::Message_Tag_ID::XDJG_RoleSkillDel:
			DBTFLV = XDJGInfo.XDJG_Net_RoleSkillDel(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_SkillID,0)).lpdata));
			break;
		//�������
		case CEnumCore::Message_Tag_ID::XDJG_ItemAdd:
			DBTFLV = XDJGInfo.XDJG_Net_ItemAdd(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_SendCharacter,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_MailTitle,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_MailContent,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Money,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ItemID,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ItemNum,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_PropertyId,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_PropertyValue,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_IncreaseNum,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_IncreaseType,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_IncreaseValue,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_IncreaseEndValue,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StarsLevel,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_AssemblyID,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_SlotNum,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EquipSlotID1,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EquipSlotID2,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EquipSlotID3,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_QualityEquip,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_IsBind,0)).lpdata/*,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ItemName,0)).lpdata*/);
			break;
// 		case CEnumCore::Message_Tag_ID::XDJG_ItemAdd:
// 			DBTFLV = XDJGInfo.XDJG_Net_ItemAdd(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
// 				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
// 				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
// 				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ItemID,0)).lpdata,
// 				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ItemNum,0)).lpdata);
// 			break;
		//����ɾ��
		case CEnumCore::Message_Tag_ID::XDJG_ItemDel:
			DBTFLV = XDJGInfo.XDJG_Net_ItemDel(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ItemID,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ItemNum,0)).lpdata));
			break;

		//��Ϸ�Ҳ�ѯ
		case CEnumCore::Message_Tag_ID::XDJG_MoneyQuery:
			DBTFLV = XDJGInfo.XDJG_Net_MoneyQuery((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata);
			break;

		//�����Ϸ��
		case CEnumCore::Message_Tag_ID::XDJG_MoneyAdd:
			DBTFLV = XDJGInfo.XDJG_Net_MoneyAdd(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Money,0)).lpdata));
			break;

		//�û���ɫ��Ϣ���޸�:�޸ĵȼ�/����
		case CEnumCore::Message_Tag_ID::XDJG_ModifyLevel_operate:
			DBTFLV = XDJGInfo.XDJG_Net_ModifyLevel(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_UpdateType,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Level,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Exp,0)).lpdata));
			break;
				
		//�û��������:�޸�����
		case CEnumCore::Message_Tag_ID::XDJG_ModifyPwd_operate:
			DBTFLV = XDJGInfo.XDJG_Net_ModifyPwd(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Pwd,0)).lpdata);
			break;
				
		//�û��������:��ʱ����ָ�
		case CEnumCore::Message_Tag_ID::XDJG_ResumePwd_operate:
			DBTFLV = XDJGInfo.XDJG_Net_ResumePwd(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata);
			break;
			
		//�û���ɫ����:��ɫ�ָ�
		case CEnumCore::Message_Tag_ID::XDJG_ResumeRole_operate:
			DBTFLV = XDJGInfo.XDJG_Net_ResumeRole(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata);
			break;

		//�û���ɫ����:��ɫɾ��
		case CEnumCore::Message_Tag_ID::XDJG_DeleteRole_operate:
			DBTFLV = XDJGInfo.XDJG_Net_DeleteRole(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata);
			break;
				
		//�޸Ľ�ɫ���Ͻ�Ǯ����(�󶨱�)
		case CEnumCore::Message_Tag_ID::XDJG_UpRoleBind_operate:
			DBTFLV = XDJGInfo.XDJG_Net_UpRoleBind(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Money,0)).lpdata));
			break;
				
		//�޸Ľ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)
		case CEnumCore::Message_Tag_ID::XDJG_UpRoleNonBind_operate:
			DBTFLV = XDJGInfo.XDJG_Net_UpRoleNonBind(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Money,0)).lpdata));
			break;

		//��ӽ�ɫ���Ͻ�Ǯ����(�󶨱�)
		case CEnumCore::Message_Tag_ID::XDJG_AddRoleBind_operate:
			DBTFLV = XDJGInfo.XDJG_Net_AddRoleBind(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Money,0)).lpdata));
			break;

		//��ӽ�ɫ���Ͻ�Ǯ����(�ǰ󶨱�)
		case CEnumCore::Message_Tag_ID::XDJG_AddRoleNonBind_operate:
			DBTFLV = XDJGInfo.XDJG_Net_AddRoleNonBind(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Money,0)).lpdata));
			break;

		//��ɫ������Ϣ��ѯ
		case CEnumCore::Message_Tag_ID::XDJG_RoleSkills_Query:
			DBTFLV = XDJGInfo.XDJG_RoleSkills_Query((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//�����б��ѯ
		case CEnumCore::Message_Tag_ID::XDJG_SkillList_Query:
			DBTFLV = XDJGInfo.XDJG_SkillList_Query("XDJG", "exec XDJG_Skilllist_Query %s",
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Profession,0)).lpdata);
			break;

		//�������Ͳ�ѯ
		case CEnumCore::Message_Tag_ID::XDJG_ItemType_Query:
			DBTFLV = XDJGInfo.XDJG_ItemType_Query("XDJG", "exec XDJG_Itemtype_Query");
			break;

		//��Ӧ���͵ĵ����б��ѯ
		case CEnumCore::Message_Tag_ID::XDJG_ItemNameLst_Query:
			DBTFLV = XDJGInfo.XDJG_ItemNameLst_Query("XDJG","exec XDJG_Item_Query '%s','%s'",
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ItemType,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ItemName,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//��Ʒ���Բ�ѯ
		case CEnumCore::Message_Tag_ID::XDJG_ItemAttribute_Query:
			DBTFLV = XDJGInfo.XDJG_ItemAttribute_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ItemID,0)).lpdata);
			break;

		//������Ϣ��ѯ
		//���������Ϣ
		case CEnumCore::Message_Tag_ID::XDJG_GuildBaseInfo_Query:
			DBTFLV = XDJGInfo.XDJG_GuildBaseInfo_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_GuildName,0)).lpdata);
			break;
			
		//�����Ա�б�
		case CEnumCore::Message_Tag_ID::XDJG_GuildMemberList_Query:
			DBTFLV = XDJGInfo.XDJG_GuildMemberList_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_GuildName,0)).lpdata);
			break;

		//��������ѯ
		case CEnumCore::Message_Tag_ID::XDJG_PlayerTaskInfo_Query:
			DBTFLV = XDJGInfo.XDJG_PlayerTaskInfo_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//��ճ�����Ϣ
		//��ճ���������Ϣ
		case CEnumCore::Message_Tag_ID::XDJG_CarBaseInfo_Query:
			DBTFLV = XDJGInfo.XDJG_CarBaseInfo_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//��ճ�����ϸ��Ϣ
		case CEnumCore::Message_Tag_ID::XDJG_CarDetailInfo_Query:
			DBTFLV = XDJGInfo.XDJG_CarDetailInfo_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//��ҹ�����Ϣ
		//��ҹ��������Ϣ
		case CEnumCore::Message_Tag_ID::XDJG_PlayerGuildBaseInfo_Query:
			DBTFLV = XDJGInfo.XDJG_PlayerGuildBaseInfo_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//��ҹ�����Ʒ��Ϣ
		case CEnumCore::Message_Tag_ID::XDJG_PlayerGuildItemInfo_Query:
			DBTFLV = XDJGInfo.XDJG_PlayerGuildItemInfo_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//�����Ʒ��־
		//װ��������־
		case CEnumCore::Message_Tag_ID::XDJG_EquipIncrease_Log:
			DBTFLV = XDJGInfo.XDJG_EquipIncrease_Log_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//��Ƭ�ϳ���־
		case CEnumCore::Message_Tag_ID::XDJG_Synthesis_Log:
			DBTFLV = XDJGInfo.XDJG_Synthesis_Log_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//װ��������־
		case CEnumCore::Message_Tag_ID::XDJG_EquipTransfer_Log:
			DBTFLV = XDJGInfo.XDJG_EquipTransfer_Log_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;
				
		//��Ʒ����
		case CEnumCore::Message_Tag_ID::XDJG_ItemIdentify_Log:
			DBTFLV = XDJGInfo.XDJG_ItemIdentify_Log_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//���˲ֿ�����־
		case CEnumCore::Message_Tag_ID::XDJG_RoleStore_Log:
			DBTFLV = XDJGInfo.XDJG_RoleStore_Log_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//����ֿ�����־
		case CEnumCore::Message_Tag_ID::XDJG_GuildStore_Log:
			DBTFLV = XDJGInfo.XDJG_GuildStore_Log_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//������־
		//�������
		case CEnumCore::Message_Tag_ID::XDJG_GuildCreate_Log:
			DBTFLV = XDJGInfo.XDJG_GuildCreate_Log_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_GuildName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//�����ɢ
		case CEnumCore::Message_Tag_ID::XDJG_GuildDismiss_Log:
			DBTFLV = XDJGInfo.XDJG_GuildDismiss_Log_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_GuildName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//������ӳ�Ա		
		case CEnumCore::Message_Tag_ID::XDJG_GuildAddMember_Log:
			DBTFLV = XDJGInfo.XDJG_GuildAddMember_Log_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_GuildName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//�����Ա�뿪
		case CEnumCore::Message_Tag_ID::XDJG_GuildMemberLeaved_Log:
			DBTFLV = XDJGInfo.XDJG_GuildMemberLeaved_Log_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_GuildName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//����ְ����
		case CEnumCore::Message_Tag_ID::XDJG_GuildPostChanged_Log:
			DBTFLV = XDJGInfo.XDJG_GuildPostChanged_Log_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_GuildName,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_StartTime,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_EndTime,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//�ʺ��Ƿ��ͣ
		case CEnumCore::Message_Tag_ID::XDJG_AccountIsClose_Query:
			DBTFLV = XDJGInfo.XDJG_AccountIsClose_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata);
			break;

		//NPC������ȡ��ѯ
		case CEnumCore::Message_Tag_ID::XDJG_NPCGetItem_Query:
			DBTFLV = XDJGInfo.XDJG_NPCGetItem_Query(
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_AccountID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			break;

		//��ӳƺ�
		case CEnumCore::Message_Tag_ID::XDJG_AddTitle_Query:
			DBTFLV = XDJGInfo.XDJG_AddTitle_Query(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_TitleID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_TitleName,0)).lpdata);
			break;
				
		//�Ƴ��ƺ�
		case CEnumCore::Message_Tag_ID::XDJG_RemoveTitle_Query:
			DBTFLV = XDJGInfo.XDJG_RemoveTitle_Query(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_TitleID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_TitleName,0)).lpdata);
			break;
			
		//���BUFF
		case CEnumCore::Message_Tag_ID::XDJG_AddBuff_Query:
			DBTFLV = XDJGInfo.XDJG_AddBuff_Query(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BuffID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BuffName,0)).lpdata);
			break;

		//�Ƴ�BUFF
		case CEnumCore::Message_Tag_ID::XDJG_RemoveBuff_Query:
			DBTFLV = XDJGInfo.XDJG_RemoveBuff_Query(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BuffID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_BuffName,0)).lpdata);
			break;
				
		//�ƶ���ָ��λ��
		case CEnumCore::Message_Tag_ID::XDJG_MoveToPosition_Query:
			DBTFLV = XDJGInfo.XDJG_MoveToPosition_Query(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_PositionX,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_PositionY,0)).lpdata),
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_PositionZ,0)).lpdata));
			break;
				
		//�ƶ���ָ��������
		case CEnumCore::Message_Tag_ID::XDJG_MoveToRole_Query:
			DBTFLV = XDJGInfo.XDJG_MoveToRole_Query(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_TargetCharacter,0)).lpdata);
			break;
				
		//�ƶ���Ŀ��������
		case CEnumCore::Message_Tag_ID::XDJG_MoveToTargetRole_Query:
			DBTFLV = XDJGInfo.XDJG_MoveToTargetRole_Query(
				StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Account,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_Character,0)).lpdata,
				(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_TargetCharacter,0)).lpdata);
			break;

		//�ƺ��б��ѯ
		case CEnumCore::Message_Tag_ID::XDJG_TitleList_Query:
			DBTFLV = XDJGInfo.XDJG_TitleList_Query("XDJG","exec XDJG_TitleInfo_Query");
			break;
				
		//BUFF�б��ѯ
		case CEnumCore::Message_Tag_ID::XDJG_BuffList_Query:
			DBTFLV = XDJGInfo.XDJG_BuffList_Query("XDJG","exec XDJG_BuffInfo_Query");
			break;
		//End

		default:
//			XDJGInfo.Destroy();
//			return FALSE;
			break;
		}

 		
		if(!DBTFLV.empty())
		{

#ifdef _DEBUG
			vector <CGlobalStruct::TFLV>::iterator iter;
			int tflvSize = DBTFLV.size();
			for(iter=DBTFLV.begin(); iter!=DBTFLV.end(); iter++)
			{
				
				printf("%x %x %s\n", (*iter).m_tagName, (*iter).m_tagFormat, (*iter).lpdata);
			}
#endif
			operXDJG.BuildTLV(DBTFLV,m_ServiceKey,(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::XDJG_ServerIP,0)).lpdata);
		}
		else
		{
			operXDJG.BuildTLV("���ݿ�Ϊ��");
		}

		//���ݷ���
		operXDJG.SendBuf(XDJGInfo.pGameSess,CEnumCore::Msg_Category::XDJG_ADMIN,m_ServiceKey,operXDJG.GetSendBuf(),operXDJG.GetSendLength());
		
		if(!XDJGInfo.Destroy())
		{
			return FALSE;
		}

	}
	catch (...)
	{
		return FALSE;
	}
	
	return TRUE;
}
