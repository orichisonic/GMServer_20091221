// Ban.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Ban.h"

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
// CBanApp

BEGIN_MESSAGE_MAP(CBanApp, CWinApp)
	//{{AFX_MSG_MAP(CBanApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBanApp construction

CBanApp::CBanApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBanApp object
extern "C"  bool __declspec(dllexport) Ban_Main(CSession * pSess,SOCKET m_socket,CEnumCore::Msg_Category m_category,CEnumCore::Message_Tag_ID m_message_tag,unsigned char * pbuf,int length)
{
	CLogFile logFile;
	try
	{
		CBanInfo banInfo;
		COperVector * precvVect=banInfo.initialize(pSess,m_message_tag, pbuf, length);
		
		if(!precvVect)//初始化类对象
		{
			return false;
		}
		if(m_message_tag == CEnumCore::Message_Tag_ID::Ban_Open_Query)//解封账号
		{
			banInfo.Ban_Open_Query(
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Ban_Type,0)).lpdata),
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::ServerInfo_GameID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::ServerInfo_IP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::ServerInfo_City,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Ban_UserID,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Ban_UserName,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Ban_NickID,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Ban_UserNick,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Ban_StartTime,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Ban_EndTime,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Ban_Reason,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Ban_Memo,0)).lpdata
					);//解封账号
		}
		else if(m_message_tag == CEnumCore::Message_Tag_ID::Ban_Close_Query)//封停账号
		{
			banInfo.Ban_Close_Query(
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Ban_Type,0)).lpdata),
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::ServerInfo_GameID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::ServerInfo_IP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::ServerInfo_City,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Ban_UserID,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Ban_UserName,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Ban_NickID,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Ban_UserNick,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Ban_StartTime,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Ban_EndTime,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Ban_Reason,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Ban_Memo,0)).lpdata
					);//封停账号
		
		}
		else if(m_message_tag == CEnumCore::Message_Tag_ID::Ban_AccountList_Query)//账号列表查询
		{
			banInfo.Ban_AccountList_Query(					
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::ServerInfo_GameID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::ServerInfo_City,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::BeginTime,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::EndTime,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
					);//账号列表查询
		
		}
		else if(m_message_tag == CEnumCore::Message_Tag_ID::Ban_Account_Query)//账号查询
		{
			banInfo.Ban_Account_Query(
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::ServerInfo_GameID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::ServerInfo_City,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Ban_UserName,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Ban_UserNick,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Ban_Memo,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::BeginTime,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::EndTime,0)).lpdata
					);
		}
		//清空
		if(!banInfo.Destroy())
		{
			return false;
		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("Ban","主函数内存不足");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("Ban","主函数文件异常");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("Ban","主函数其他异常");
		return false;
	}
	catch(...)
	{
		return false;
	}
	return true;
}

CBanApp theApp;
