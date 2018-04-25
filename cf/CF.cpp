// CF.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CF.h"

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
// CCFApp

BEGIN_MESSAGE_MAP(CCFApp, CWinApp)
	//{{AFX_MSG_MAP(CCFApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCFApp construction

CCFApp::CCFApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCFApp object
extern "C"  bool __declspec(dllexport) CF_Main(CSession * pSess,SOCKET m_socket,CEnumCore::Msg_Category m_category,CEnumCore::Message_Tag_ID m_message_tag,unsigned char * pbuf,int length)
{
		CLogFile logFile;
	char errMessage[256];
	try
	{
		CCFApp theApp;
		CCFInfo CFInfo;
		COperatorCF operCF;
		vector <CGlobalStruct::TFLV> DBTFLV;
		CEnumCore::ServiceKey m_ServiceKey= (m_message_tag & 0x00FFFF) | 0x8000;//获取要回应的CEnumCore::ServiceKey
		vector <CGlobalStruct::TFLV> RECTFLV=CFInfo.initialize(pSess, pbuf, length);

		if(RECTFLV.empty())//初始化类对象
		{
			operCF.BuildTLV("远程命令未空");
			operCF.SendBuf(CFInfo.pGameSess,CEnumCore::Msg_Category::CF_ADMIN,m_ServiceKey,operCF.GetSendBuf(),operCF.GetSendLength());
			return TRUE;
		}
		switch((unsigned int)m_message_tag)
		{
			case CEnumCore::Message_Tag_ID::CF_Account_Query:
				{
				//	DBTFLV=theApp.UserInfo(RECTFLV);
				}
				break;
			case CEnumCore::Message_Tag_ID::CF_KickPlayer:
				{
				//	DBTFLV=theApp.KickPlayer(RECTFLV);
				}
				break;
			case CEnumCore::Message_Tag_ID::CF_ClosePlayer:
				{
				/*	DBTFLV=CFInfo.Net_ClosePlayer(
						StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_ServerIP,0)).lpdata,
						(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_Account,0)).lpdata,
						StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_NickID,0)).lpdata),
						(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_UserNick,0)).lpdata,
						StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_Time,0)).lpdata),
						(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_Reason,0)).lpdata);*/

				}
				break;
			case CEnumCore::Message_Tag_ID::CF_OpenPlayer:
				{
				/*	DBTFLV=CFInfo.Net_OpenPlayer(
						StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_ServerIP,0)).lpdata,
						(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_Account,0)).lpdata,
						StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_NickID,0)).lpdata),
						(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_UserNick,0)).lpdata,
						(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_Reason,0)).lpdata);*/
				}
				break;
			case CEnumCore::Message_Tag_ID::CF_CloseList:
				{
				/*	DBTFLV=CFInfo.CloseList("CF","exec CF_ACCOUNTREMOTE_QUERY '%s'",
						(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_ServerName,0)).lpdata,
						(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_ServerIP,0)).lpdata,
						StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));*/
				}
				break;
			case CEnumCore::Message_Tag_ID::CF_CloseSingle:
				{
				/*	DBTFLV=CFInfo.CloseSingle("CF",
						"select distinct BanPlayerAccount as CF_NickID,BanPlayerUserNick as CF_UserNick,BanPlayerUserId as CF_Account,city as CF_ServerName,BanReason as CF_Reason from CF_AutoUnBanUser a, gmtools_serverinfo b where a.serverip=b.serverip and (b.city='%s' and BanPlayerUserId='%s')",
						(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_ServerName,0)).lpdata,
						(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_ServerIP,0)).lpdata,
						(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_Account,0)).lpdata,
						StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));*/
				}
				break;
			case CEnumCore::Message_Tag_ID::CF_BoardTasker_Add:
				{
				/*	DBTFLV=CFInfo.BroadTask_Insert("CF","exec CF_BOARDTASK_INSERT %i,'%s','%s','%s','%s','%s',%i",
						StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_ServerIP,0)).lpdata,
						"255.255.255.255",
						(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_BoardMessage,0)).lpdata,
						(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_BeginTime,0)).lpdata,
						(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_EndTime,0)).lpdata,
						StrToInt((LPCSTR)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_Interval,0)).lpdata));*/
				}
				break;
			case CEnumCore::Message_Tag_ID::CF_BoardTasker_Del:
				{
				/*	DBTFLV=CFInfo.BroadTask_Update("CF","exec CF_BOARDTASK_UPDATE %i,%i,'%s','%s','%s',%i,%i",
						StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						StrToInt((LPCSTR)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_TaskID,0)).lpdata),
						(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_BoardMessage,0)).lpdata,
						(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_BeginTime,0)).lpdata,
						(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_EndTime,0)).lpdata,
						StrToInt((LPCSTR)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_Interval,0)).lpdata),
						1);*/
				}
				break;
			case CEnumCore::Message_Tag_ID::CF_BoardTasker_Query:
			/*	DBTFLV=CFInfo.BroadTask_Query("CF","exec CF_BOARDTASK_QUERY",
					StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
					StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));*/
				break;
			default:
				break;
			
		}

		if(!DBTFLV.empty())
		{
//			operCF.BuildTLV(DBTFLV,m_ServiceKey,(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::CF_ServerIP,0)).lpdata);
		}
		else
		{
	//		operCF.BuildTLV("数据库为空");
		}
		operCF.SendBuf(CFInfo.pGameSess,CEnumCore::Msg_Category::CF_ADMIN,m_ServiceKey,operCF.GetSendBuf(),operCF.GetSendLength());


		//清空
		if(!CFInfo.Destroy())
		{
			return false;
		}
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"主要程序内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("CF",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"主要程序文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("CF",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"主要程序其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("CF",errMessage);
		return false;
	}
	catch(...)
	{
		return false;
	}
	return true;
}
/*vector <CGlobalStruct::TFLV> CCFApp::UserInfo(vector <CGlobalStruct::TFLV> RECTFLV)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CWAInfo WAInfo;
	CLogFile logFile;
	char errMessage[256];
	try
	{
		printf("查询用户信息\n");
		DBTFLV=WAInfo.Net_UserInfo(
			(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
			(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
			(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
			StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
			StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata));
		
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询用户信息内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("WA",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询用户信息文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("WA",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询用户信息其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("WA",errMessage);
		return DBTFLV;
	}
	return DBTFLV;
	
}
vector <CGlobalStruct::TFLV> CWAApp::KickPlayer(vector <CGlobalStruct::TFLV> RECTFLV)
{
	CLogFile logFile;
	char errMessage[256];
	vector <CGlobalStruct::TFLV> DBTFLV;
	CWAInfo WAInfo;
	try
	{
		printf("强制玩家下线\n");
		DBTFLV=WAInfo.Net_KickPlayer(StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
			(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
			(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
			(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata);

	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"强制玩家下线内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("WA",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"强制玩家下线文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("WA",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"强制玩家下线其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("WA",errMessage);
		return DBTFLV;
	}
	return DBTFLV;
	
}*/

extern CCFApp theApp;
