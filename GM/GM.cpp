// GM.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "GM.h"
//#include "GMManagerModule.h"

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
// CGMApp

BEGIN_MESSAGE_MAP(CGMApp, CWinApp)
	//{{AFX_MSG_MAP(CGMApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGMApp construction

CGMApp::CGMApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGMApp object
void getloop(CGMInfo gmapi)
{
	
	if(checkfilelist::beforhavebegin)
	{
		checkfilelist::beforhavebegin=false;
		gmapi.GM_FileUpdate();
		
	}
	if(!checkfilelist::beforhavebegin)
	{
		//	printf("second\n");
		int waitecount=0;
		while(1)
		{
            waitecount++;
			Sleep(1000);
			if(waitecount>1*60*8)
			{
				checkfilelist::beforhavebegin=true;
				waitecount=0;
			}
			if(checkfilelist::beforhavebegin)
			{
				break;
			}
		}
		if(checkfilelist::beforhavebegin)
		{
			getloop(gmapi);
			
		}
		else
		{
			getloop(gmapi);
		}
	}
}

extern "C" bool __declspec(dllexport)GM_Main(CSession * pSess,SOCKET m_socket,CEnumCore::Msg_Category m_category,CEnumCore::Message_Tag_ID m_message_tag,unsigned char * pbuf,int length)
{

	CGMInfo gmapi;
	//COperatorGM operGM;
	//CEnumCore::ServiceKey m_ServiceKey= (m_message_tag & 0x00FFFF) | 0x8000;//��ȡҪ��Ӧ��CEnumCore::ServiceKey
	vector <CGlobalStruct::TFLV> RECTFLV = gmapi.initialize(pSess,m_message_tag,pbuf,length)->GetInnerList();

	switch((unsigned short int)m_category)
	{
	case CEnumCore::Msg_Category::COMMON://0x80
		{
			switch((unsigned int)m_message_tag)
			{
			case CEnumCore::Message_Tag_ID::CONNECT:///��½��֤
				{
					try
					{
						gmapi.IsLogin();
					}
					catch (...)
					{
					
					}
				
				}
				break;
			case CEnumCore::Message_Tag_ID::SERVERINFO_IP_QUERY://�鿴������Ϸ������
				{
					try
					{
						gmapi.Server_IP_Query();
					}
					catch (...)
					{
					
					}


				}

				break;
			case CEnumCore::Message_Tag_ID::SERVERINFO_IP_ALL_QUERY://�鿴������Ϸ������
				{
					try
					{
						gmapi.ServerInfo_IP_All_Query();
					}
					catch (...)
					{
						
					}

				}
				break;
			case CEnumCore::Message_Tag_ID::GMTOOLS_USERMAC_UPDATE://����GM�ʺŵĹ�������
				{
					try
					{
						gmapi.Gmtools_UserMac_Update();
					}
					catch(...)
					{

					}

				}
				break;
			case CEnumCore::Message_Tag_ID::GMTOOLS_OperateLog_Query://�鿴���߲�����¼
				{
					try
					{
						gmapi.Gmtools_OperateLog_Query();
					}
					catch(...)
					{

					}
					
				}
				break;
			case CEnumCore::Message_Tag_ID::CLIENT_PATCH_COMPARE://�Ƚ��ļ��汾
				{
					try
					{
						gmapi.GM_FileCompare();

					}
					catch(...)
					{
					
					}
				}
				break;
			case CEnumCore::Message_Tag_ID::CLIENT_PATCH_UPDATE://�ļ�����
				{
					try
					{
                    getloop(gmapi);

					}
					catch(...)
					{
					}
				}
				break;
			//Added by tengjie 2010-05-05
			case CEnumCore::Message_Tag_ID::HARMFUL_PROC_QUERY://��ҽ�����Ϣ��ѯ
				{
					try
					{
						gmapi.Gm_HarmFulProc_Query();
						
					}
					catch(...)
					{
					}
				}
				break;
			case CEnumCore::Message_Tag_ID::GM_AccountInfo_Vip://��֤�Ƿ�vip�ʺ�
				{
					try
					{
						cout<<"GM_AccountInfo_Vip"<<endl;
						gmapi.GM_Net_vipAccountInfo(
							(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::PlayerAccount,0)).lpdata);						
					}
					catch (...)
					{
					}
				}
				/*
			case CEnumCore::Message_Tag_ID::GM_ClosePlayerGroup://��ͣ�ʺ�
				{
					try
					{
						cout<<"WA_ClosePlayerGroup"<<endl;
						gmapi.GM_Net_ClosePlayerGroup(
							StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
							(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_ServerIP,0)).lpdata,
							(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Account,0)).lpdata,
							StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_NickID,0)).lpdata),
							(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_UserNick,0)).lpdata,
							StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Time,0)).lpdata),
							StrToInt((char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Reason,0)).lpdata),
							(char *)&(COperVector(RECTFLV).getTLVByTag(CEnumCore::TagName::WA_Description,0)).lpdata);
					}

					catch (...)
					{
					}
				}
				*/
			default:
				break;
			}
			
	

		}
		break;
	case CEnumCore::Msg_Category::USER_ADMIN://0x81
		{
			switch((unsigned int)m_message_tag)
			{
			case CEnumCore::Message_Tag_ID::USER_QUERY_ALL://�����б�
				{
					try
					{
						gmapi.User_Query_All();
					}
					catch (...)
					{
						
					}

				}
				break;
			case CEnumCore::Message_Tag_ID::USER_CREATE://�����û�
				{
					try
					{
						gmapi.User_Create();
					}
					catch (...)
					{
					}
					


				}
				break;
			case CEnumCore::Message_Tag_ID::USER_UPDATE://�����û�
				{
					try
					{
						gmapi.User_Update();					
					}
					catch (...)
					{
					}
					
				}
				break;
			case CEnumCore::Message_Tag_ID::USER_DELETE://ɾ���û�
				{
					try
					{
						gmapi.User_Delete();		
					}
					catch (...)
					{
					}
					
				}
				break;
			case CEnumCore::Message_Tag_ID::DEPART_QUERY://�����б��ѯ
				{
					try
					{
						gmapi.Depart_Query();
						
					}
					catch (...)
					{
					}
					
				
				}
				break;
			case CEnumCore::Message_Tag_ID::USER_PASSWD_MODIF://�û������޸�
				{
					try
					{
						gmapi.User_Passwd_Modif();
					}
					catch (...)
					{
					}		
				}
				break;
			
			default:
				{
				}
				break;
			}



		}
		break;
	case CEnumCore::Msg_Category::USER_MODULE_ADMIN://0x83
		{
			switch((unsigned int)m_message_tag)
			{
			case CEnumCore::Message_Tag_ID::USER_MODULE_QUERY://��ѯ�û���ģ��Ĺ�ϵ
				{
					try
					{
						gmapi.User_Module_Query();
					}
					catch(...)
					{
					}
				
				}
				break;
			case CEnumCore::Message_Tag_ID::USER_MODULE_UPDATE://����ģ��
				{
					try
					{
						gmapi.User_Module_Update();
					}
					catch(...)
					{
					}
					
				}
				break;
			default:
				break;
			}
		}
		break;
	 case CEnumCore::Msg_Category::GAME_ADMIN://0x84
		{
			switch((unsigned int)m_message_tag)
			{
			case CEnumCore::Message_Tag_ID::GAME_QUERY://��Ϸ��ѯ
				{
					try
					{
						gmapi.Game_Query();
					}
					catch(...)
					{
					}
				}
				break;
			case CEnumCore::Message_Tag_ID::GAME_MODULE_QUERY://��ѯ��Ϸ��ģ���б�
				{
					try
					{
						gmapi.Game_Module_Query();
					}
					catch(...)
					{
					}
					
					
				}
				break;
			default:
				break;
			}
		}
		break;
	default:
		{
			
			goto FALSE_FUNC;
		}
	
		break;

	}
	return TRUE;
FALSE_FUNC:
	return FALSE;
}
CGMApp theApp;