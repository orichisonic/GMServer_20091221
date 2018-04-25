// GTOP.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "GTOP.h"
#include "GTOper.h"

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
// CGTOPApp

BEGIN_MESSAGE_MAP(CGTOPApp, CWinApp)
	//{{AFX_MSG_MAP(CGTOPApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGTOPApp construction

CGTOPApp::CGTOPApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGTOPApp object
extern "C"  bool __declspec(dllexport) GTOP_Main(CSession * pSess,SOCKET m_socket,CEnumCore::Msg_Category m_category,CEnumCore::Message_Tag_ID m_message_tag,unsigned char * pbuf,int length)
{
		HANDLE m_Mutex=NULL;
		//if(m_Mutex!=NULL)
		//	ReleaseMutex(m_Mutex);
		//m_Mutex=CreateMutex(NULL, FALSE, "GTOP_MAIN");//启用互斥量让GT_MAIN独用接口通道
        //WaitForSingleObject(m_Mutex,INFINITE);//锁线程
		CGTOper GTOper;

		COperVector * precvVect=GTOper.initialize(pSess,m_message_tag,pbuf, length);//初始化类对象
		if(!precvVect)
		{
			return FALSE;
		}
		
 		if(m_message_tag == CEnumCore::Message_Tag_ID::GT_BROADCAST_SELECT)//查询所有公告
		{
			try
			{
				GTOper.BroadTask_Query(
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
					);
			}
			catch(...)
			{
		//		ReleaseMutex(m_Mutex);//解锁
				return false;
			}
		}
		else if(m_message_tag == CEnumCore::Message_Tag_ID::GT_BROADCAST_ADD)//添加公告
		{
			try
			{
				GTOper.BroadTask_Insert(
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_IsImmediate,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_BoardMessage,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_StartTime,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_EndTime,0)).lpdata,
					StrToInt((LPCSTR)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Interval,0)).lpdata));
			}
			catch(...)
			{
			//	ReleaseMutex(m_Mutex);//解锁
				return false;
			}
		}
		else if(m_message_tag == CEnumCore::Message_Tag_ID::GT_BROADCAST_REMOVE)//删除公告
		{
			try
			{
				GTOper.BroadTask_Update(
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_NoticeID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_StartTime,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_EndTime,0)).lpdata,
					0,0
					);
			}
			catch(...)
			{
			//	ReleaseMutex(m_Mutex);//解锁
				return false;
			}
		}
		else if(m_message_tag == CEnumCore::Message_Tag_ID::GT_KICK_USER)//强制玩家下线
		{
			try
			{
				GTOper.KickPlayer(
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Account,0)).lpdata
					);
			}
			catch(...)
			{
			//	ReleaseMutex(m_Mutex);//解锁
				return false;
			}
		}
		else if(m_message_tag == CEnumCore::Message_Tag_ID::GT_MOVE_USER)//角色移至小黑屋
		{
			try
			{
				GTOper.GT_MOVE_USER(
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Account,0)).lpdata
					);
			}
			catch(...)
			{
			//	ReleaseMutex(m_Mutex);//解锁
				return false;
			}
		}
		else if(m_message_tag == CEnumCore::Message_Tag_ID::GT_USER_STOP)//账号封停、解封
		{
			try
			{
				GTOper.GT_BanUser(
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Account,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_UserID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_UserState,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_BanReason,0)).lpdata
					);
			}
			catch(...)
			{
		//		ReleaseMutex(m_Mutex);//解锁
				return false;
			}
		}
		else if(m_message_tag == CEnumCore::Message_Tag_ID::GT_ITEM_SELECT)//查询道具、宠物
		{
			try
			{
				GTOper.GM_ITEM_Check(
					);
			}
			catch(...)
			{
		//		ReleaseMutex(m_Mutex);//解锁
				return false;
			}
		}
		else if(m_message_tag == CEnumCore::Message_Tag_ID::GT_ADD_ITEM)//添加道具、宠物
		{
			try
			{
				GTOper.GT_ITEM_ADD(
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Account,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_MailTitle,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_MailContent,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageCount,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ItemName,0)).lpdata
					);
			}
			catch(...)
			{
		//		ReleaseMutex(m_Mutex);//解锁
				return false;
			}
		}
		else if(m_message_tag == CEnumCore::Message_Tag_ID::GT_USER_RESET)//重设玩家到出生状态
		{
			try
			{
				GTOper.GT_USER_RESET(
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Account,0)).lpdata
					);

			}
			catch(...)
			{
		//		ReleaseMutex(m_Mutex);//解锁
				return false;
			}
		}
		else if(m_message_tag == CEnumCore::Message_Tag_ID::GT_Transmit_Query)//瞬间移动
		{
			try
			{
				GTOper.GT_Transmit_Query(
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Account,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_X,0)).lpdata),
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Y,0)).lpdata),
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Z,0)).lpdata)
					);
			}
			catch(...)
			{
		//		ReleaseMutex(m_Mutex);//解锁
				return false;
			}
		}
		else if(m_message_tag == CEnumCore::Message_Tag_ID::GT_Picture_Stop)//禁止、解禁玩家上传图片
		{
			try
			{
				GTOper.GT_Picture_Stop(
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Account,0)).lpdata,
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_UserID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_UserState,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_BanReason,0)).lpdata
					);
			}
			catch(...)
			{
		//		ReleaseMutex(m_Mutex);//解锁
				return false;
			}
		}
		else if(m_message_tag == CEnumCore::Message_Tag_ID::GT_Picture_Clear)//清理违禁图片
		{
			try
			{
				GTOper.GT_Picture_Clear(
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Account,0)).lpdata
					);
			}
			catch(...)
			{
		//		ReleaseMutex(m_Mutex);//解锁
				return false;
			}
		}
		else if(m_message_tag == CEnumCore::Message_Tag_ID::GT_ClearName_Query)//清空个性签名
		{
			try
			{
				GTOper.GT_ClearName_Query(
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Account,0)).lpdata
					);
			}
			catch(...)
			{
		//		ReleaseMutex(m_Mutex);//解锁
				return false;
			}
		}
		else if(m_message_tag==CEnumCore::Message_Tag_ID::GT_Import_Item)//批量添加道具
		{
			try
			{
				GTOper.GT_Import_Item(
					StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_MailTitle,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_MailContent,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Account,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ItemID,0)).lpdata,
					(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ItemName,0)).lpdata//数量
					);
			}
			catch(...)
			{
				//		ReleaseMutex(m_Mutex);//解锁
				return false;
			}
		}
		//清空
		if(!GTOper.Destroy())
		{
			if(m_Mutex!=NULL)
	//			ReleaseMutex(m_Mutex);//解锁	
			return false;
		}

	//	ReleaseMutex(m_Mutex);//解锁

	return true;
}

bool GT_KICK_USER(char * ServerIP,char * Account)
{
	try
	{
		CGTOper GTOper;
		char retMessage[256];
		GTOper.GT_KICK_USER(ServerIP,Account,retMessage);
	}
	catch (...)
	{
		return true;
	}
	
	return true; 
}

bool GT_BAN_USER(char * ServerIP,char * Account,int status)
{
	try
	{
		CGTOper GTOper;
		GTOper.GT_BanUser(ServerIP,Account,status);
	}
	catch (...)
	{
		return true;
	}

	return true;
}
CGTOPApp theApp;
