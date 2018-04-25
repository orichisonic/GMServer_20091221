// XDJG_DLL.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "XDJG_DLL.h"
#include "butt.h"
#include "afxinet.h"
#include <string>
#include <vector>
using namespace std;

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
// CXDJG_DLLApp

BEGIN_MESSAGE_MAP(CXDJG_DLLApp, CWinApp)
	//{{AFX_MSG_MAP(CXDJG_DLLApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXDJG_DLLApp construction

CXDJG_DLLApp::CXDJG_DLLApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CXDJG_DLLApp object
int XDJG_SendBulletin(char *ip,char *lpBoardMessaeg,int port)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	try
	{
		char message[255];
		vector <string> pVect;		
		CInternetSession mySession("XDJG_DLL", 0);
		CHttpFile* myHttpFile = NULL;
		CString myData;

		sprintf(message, "http://%s:%d/rpc_proxy?app_name=ws&fun=announce_notice(%s)", ip, port, lpBoardMessaeg);

		myHttpFile = (CHttpFile *)mySession.OpenURL(message, 1, INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_RELOAD, NULL, 0);
		if (myHttpFile==NULL)
		{
			printf("http连接失败\n");
			return 0;
		}
		
		if (myHttpFile->ReadString(myData))
		{
			if (myData[0] == '1')
			{
				printf("<公告:%s>发送成功\n", lpBoardMessaeg);
			}
			else
			{
				printf("发送失败\n");
				return 0;
			}

		}

		mySession.Close();
	}
	catch (...)
	{

		return 0;				
	}

	return 1;
}

CXDJG_DLLApp theApp;
