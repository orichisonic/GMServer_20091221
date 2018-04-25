// PalLog.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PalLog.h"

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
// CPalLogApp

BEGIN_MESSAGE_MAP(CPalLogApp, CWinApp)
	//{{AFX_MSG_MAP(CPalLogApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPalLogApp construction

CPalLogApp::CPalLogApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPalLogApp object
extern "C" bool __declspec(dllexport) PalLog_Main(CSession *pSess,SOCKET m_socket,CEnumCore::Msg_Category m_category,CEnumCore::Message_Tag_ID m_message_tag,unsigned char * pbuf,int length)
{
	CLogFile logFile;
	char errMessage[256];
	try
	{
		CPalLogInfo PalLogInfo;
		COperVector * precvVect=PalLogInfo.initialize(pSess,m_message_tag,pbuf, length);//��ʼ�������
		if(!precvVect)
		{
			return FALSE;
		}
		switch((unsigned int)m_message_tag)
		{	
		case CEnumCore::Message_Tag_ID::PALLOG_ACCOUNT_LOGIN://��½��¼��־
			{
				PalLogInfo.LoginInfo(
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MALLTYPE,0)).lpdata,//���ǳƴ�������
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LOGINIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LOGINTIME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LOGOUTTIME,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
			}
			break;
		case CEnumCore::Message_Tag_ID::PALLOG_ITEMMAIL://�̳ǹ����¼
			{
				PalLogInfo.Shop_Buy_Query(
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MALLTYPE,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LOGINTIME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LOGOUTTIME,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
					);
			}
			break;
		case CEnumCore::Message_Tag_ID::PAL_CHECKMAILBOX_QUERY://������Ϣ��ѯ
			{
				PalLogInfo.MailBox_Info_Query(
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLEID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LOGINTIME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LOGOUTTIME,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
					);
			}
			break;
		case CEnumCore::Message_Tag_ID::PAL_CHECKMAILBOX_DETAIL://�ʼ�������ѯ
			{
				PalLogInfo.MailBox_Info_Detail(
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MAILID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LOGINTIME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LOGOUTTIME,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
					);
			}
			break;
		case CEnumCore::Message_Tag_ID::PAL_AUCTION_QUERY://������Ϣ��ѯ
			{
				PalLogInfo.Auction_Info_Query(
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLEID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LOGINTIME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LOGOUTTIME,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
					);
			}
			break;
		case CEnumCore::Message_Tag_ID::PAL_LOGINFO_QUERY://�����־��Ϣ
			{
				PalLogInfo.LogInfo_Query(
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LogBigTypeID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LogBigType,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LogSmallTypeID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LogSmallType,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LOGINTIME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LOGOUTTIME,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
					);
			}
			break;
		case CEnumCore::Message_Tag_ID::PAL_HISTORY_LOGINFO_QUERY://�����־��ʷ��Ϣ
			{
				PalLogInfo.HisLogInfo_Query(
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LogBigTypeID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LogBigType,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LogSmallTypeID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LogSmallType,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LOGINTIME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LOGOUTTIME,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
					);
			}
			break;
		case CEnumCore::Message_Tag_ID::PAL_CDKEY_QUERY://CDKEY������ȡ��ѯ
			{
				PalLogInfo.CDKey_Get_Query(
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LOGINTIME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LOGOUTTIME,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
					);
			}
			break;
		case CEnumCore::Message_Tag_ID::PAL_ITEMMAIL_WASTE://Ԫ�����ļ�¼
			{
				PalLogInfo.Money_Waste_Query(
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LOGINTIME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LOGOUTTIME,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
					);
			}
			break;
		case CEnumCore::Message_Tag_ID::PAL_LOG_BIGTYPE_QUERY://��־����
			{
				PalLogInfo.GetLogBigType();
			}
			break;
			
		case CEnumCore::Message_Tag_ID::PAL_LOG_SMALLTYPE_QUERY://��־С��
			{
				PalLogInfo.GetLogSmallType(StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LogBigTypeID,0)).lpdata));
			}
			break;
		case CEnumCore::Message_Tag_ID::PAL_ROLEDEL_QUERY://��ɫɾ����־
			{
				PalLogInfo.Pal_RoleDel_Query(
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIPNAME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_GMSERVERIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ACCOUNT,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_ROLENAME,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_MALLTYPE,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LOGINTIME,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_LOGOUTTIME,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
					);
			}
			break;
			//  add by ltx
		case CEnumCore::Message_Tag_ID::PAL_EQUIPMENT_QUERY://װ�����߲�ѯ
			{
				PalLogInfo.Pal_Equipment_Query(	
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PAL_CustomIter,0)).lpdata);
			}
			break;
		default:
			goto FALSE_FUNC;
			break;
		}
		if(!PalLogInfo.Destroy())
		{
			return FALSE;
		}
		return TRUE;
	FALSE_FUNC:
		PalLogInfo.Destroy();
		return FALSE;
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��Ҫ�����ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��Ҫ�����ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��Ҫ���������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		return FALSE;
	}
	
		

}
CPalLogApp theApp;
