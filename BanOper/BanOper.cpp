// BanOper.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "BanOper.h"
#include "JW2Ban.h"
#include "PalBan.h"
#include "GTBan.h"
#include "AUBan.h"
#include "MFBan.h"
#include "RayBan.h"
#include "SDBan.h"
#include "SDOBan.h"
#include "SltqBan.h"
#include "CG2Ban.h"
#include "FJBan.h"

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
// CBanOperApp

BEGIN_MESSAGE_MAP(CBanOperApp, CWinApp)
	//{{AFX_MSG_MAP(CBanOperApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBanOperApp construction

CBanOperApp::CBanOperApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBanOperApp object

/////////////////////////////////////
////���룺type=0������1,��ʱ��
///    
////userByID
////	  
////����Ƿ�ɹ�1.�ɹ�
////
/////////////////////////////////////
int CloseOper(int type,int userByID,int GameID,char* ServerIP,char * ServerName,char * UserID, char * UserName, char * NickID, char * UserNick, char * beginTime, char * endTime, char * reason,char *memo)
{
	COperBan operBan;
	char m_result[256];
	if(type==0)//������ͣ
	{
		switch(GameID)
		{
		case 35://������II
			{
				CJW2Ban jw2Ban;
				///����
				if(jw2Ban.KickPlayer(GameID,ServerIP,ServerName,UserName,userByID)==0)
				{
					//��ͣ�˺�
					if(!strcmp(UserID,"")||!strcmp(UserID,"NULL"))
					{
						jw2Ban.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
					}
					if(jw2Ban.ClosePlayer(GameID,ServerIP,ServerName,UserName)>0)
					{
						operBan.RecordData(1,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
						sprintf(m_result,"��ͣ�ʺ�%s�ɹ�",UserName);
						WriteLog("������II",m_result);
					}
					else
					{
						return 0;
					}
				}
				else
				{
					return 0;
				}
				
				
			}
			break;
		case 34://����
			{
				CGTBan gtBan;
				///����
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					gtBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if(gtBan.KickPlayer(GameID,ServerIP,ServerName,UserName))
				{
					//��ͣ�˺�
					if(gtBan.ClosePlayer(GameID,ServerIP,ServerName,UserName)>0)
					{
						operBan.RecordData(1,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
						sprintf(m_result,"��ͣ�ʺ�%s�ɹ�",UserName);
						WriteLog("����Online",m_result);
					}
					else
					{
						return 0;
					}
				}
				else
				{
					return 0;
				}
			}
			break;
		case 31://�ɽ�
			{
				CPalBan palBan;
				///����
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL")||!strcmp(UserNick,"")||!strcmp(UserNick,"NULL"))
				{
					palBan.FindAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}

				if(!strcmp(UserNick,"")||!strcmp(UserNick,"NULL"))
				{
					return 0;
				}

				if(palBan.KickPlayer(GameID,ServerIP,ServerName,UserNick))
				{
					//��ͣ�˺�
					if(palBan.ClosePlayer(GameID,ServerIP,ServerName,UserName)>0)
					{
						operBan.RecordData(1,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
						sprintf(m_result,"��ͣ�ʺ�%s�ɹ�",UserName);
						WriteLog("�ɽ�Online",m_result);

					}
					else
					{
						return 0;
					}
				}
				else
				{
					return 0;
				}
			}
			break;
		case 25://ħ������II(�Ƚ����� ����ɫ��ͣ,���˺ŷ�ͣ)
			{
				
				CCG2Ban cg2Ban;
				if(!strcmp(memo,"1") )//�����ͣ�˺�
				{
					if(!strcmp(UserName,""))
					{
						cg2Ban.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);//ͨ���ǳƲ��˺�
					}
					strcpy(UserNick,"");	
				}
				
				///����
				if(cg2Ban.KickPlayer(GameID,ServerIP,ServerName,UserName,UserNick,memo))
				{
					//��ͣ�˺�
					if(cg2Ban.ClosePlayer(GameID,ServerIP,ServerName,UserName,UserNick,endTime,reason,memo))
					{
						//д���ݿ�
						operBan.RecordData(1,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
						sprintf(m_result,"��ͣ�ʺ�%s�ɹ�",UserName);
						WriteLog("ħ������II",m_result);
					}
					else
					{
						return 0;
					}
				}
				else
				{
					return 0;
				}
			}	
			break;
		case 3://��������
			{
				CSDOBan sdoBan;
				///����
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					sdoBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if(sdoBan.KickPlayer(GameID,ServerIP,ServerName,UserName))
				{
					//��ͣ�˺�
					if(sdoBan.ClosePlayer(GameID,ServerIP,ServerName,UserName)>0)
					{
						operBan.RecordData(1,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
						sprintf(m_result,"��ͣ�ʺ�%s�ɹ�",UserName);
						WriteLog("��������",m_result);
					}
					else
					{
						return 0;
					}
				}
				else
				{
					return 0;
				}
			}
			break;
		case 6://������
			{
				CAUBan auBan;
				///����
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					auBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				auBan.KickPlayer(GameID,ServerIP,ServerName,UserName);
				//��ͣ�˺�
				if(auBan.ClosePlayer(GameID,ServerIP,ServerName,UserName))
				{
					operBan.RecordData(1,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
					sprintf(m_result,"��ͣ�ʺ�%s�ɹ�",UserName);
					WriteLog("������",m_result);
				}
				else
				{
						return 0;
				}
			}
			break;
	/*	case 24://SD�ߴ�
			{
				CSDBan sdBan;
				///����
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					sdBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if(sdBan.KickPlayer(GameID,ServerIP,ServerName,UserName))
				{
					//��ͣ�˺�
					if(sdBan.ClosePlayer(GameID,ServerIP,ServerName,UserName))
					{
						operBan.RecordData(1,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,reason,beginTime,endTime,memo);	
					}
					else
					{
						return 0;
					}
				}
				else
				{
					return 0;
				}

			}
			break;*/
		case 27://����
			{
				CSltqBan sltqBan;

				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL")||!strcmp(UserNick,"")||!strcmp(UserNick,"NULL"))
				{
					sltqBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if (!strcmp(UserNick,"")||!strcmp(UserNick,"NULL"))
				{
					WriteLog("����","��ɫ������");
					return 0;
				}

				//����
				if(sltqBan.KickPlayer(GameID,ServerIP,ServerName,UserNick))
				{
					//��ͣ�˺�
					if(sltqBan.ClosePlayer(GameID,ServerIP,ServerName,UserName,UserNick,beginTime,endTime))//ServerIP
					{
						operBan.RecordData(1,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
						sprintf(m_result,"��ͣ�ʺ�%s�ɹ�",UserName);
						WriteLog("����",m_result);
					}
					else
					{
						return 0;
					}
				}
				else
				{
					return 0;
				}
			}
			break;
		case 18://���
			{
				CFJBan fjBan;
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					fjBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				//��ͣ�˺�
				if(fjBan.ClosePlayer(GameID,ServerIP,ServerName,UserName))
				{
					operBan.RecordData(1,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
					sprintf(m_result,"��ͣ�ʺ�%s�ɹ�",UserName);
					WriteLog("���",m_result);
				}
				else
				{
					return 0;
				}
			}
			break;
		case 19://���߷ɳ�(û������)
			{
				CRayBan rayBan;
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					rayBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
			//	if(rayBan.KickPlayer(GameID,ServerIP,ServerName,UserName))
			//	{
					//��ͣ�˺�
					if(rayBan.ClosePlayer(GameID,ServerIP,ServerName,UserName))
					{
						operBan.RecordData(1,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
						sprintf(m_result,"��ͣ�ʺ�%s�ɹ�",UserName);
						WriteLog("���߷ɳ�",m_result);
					}
					else
					{
						return 0;
					}
			//	}
			//	else
			//	{
			//		return 0;
			//	}
			}
			break;
		case 22://����ɭ��
			{
				CMFBan mfBan;
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					mfBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if(mfBan.KickPlayer(GameID,ServerIP,ServerName,UserName))
				{
					//��ͣ�˺�
					if(mfBan.ClosePlayer(GameID,ServerIP,ServerName,UserName,reason))
					{
						operBan.RecordData(1,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
						sprintf(m_result,"��ͣ�ʺ�%s�ɹ�",UserName);
						WriteLog("����ɭ��",m_result);
					}
					else
					{
						return 0;
					}
				}
				else
				{
					return 0;
				}

			}
			break;
		default:
			return 0;
			break;
		}
	}
	else//��ʱ��ͣ�������ݿ�
	{
		switch(GameID)
		{
		case 35://������II
			{
				CJW2Ban jw2Ban;
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					jw2Ban.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				
			}
			break;
		case 34://����
			{
				CGTBan gtBan;

				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					gtBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				
			}
			break;
		case 31://�ɽ�
			{
				CPalBan palBan;
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					palBan.FindAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				
			}
			break;
		case 25://ħ������II(�Ƚ����� ����ɫ��ͣ,���˺ŷ�ͣ)
			{
				
				CCG2Ban cg2Ban;
				///����
				if(!strcmp(memo,"1") )//�����ͣ�˺�
				{
					if(!strcmp(UserName,""))
					{
						cg2Ban.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);//ͨ���ǳƲ��˺�
					}
					strcpy(UserNick,"");	
				}
				
				
			}	
			break;
		case 3://��������
			{
				CSDOBan sdoBan;
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					sdoBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				
			}
			break;
		case 6://������
			{
				CAUBan auBan;
				///����
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					auBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				
			}
			break;
	
		case 27://����
			{
				CSltqBan sltqBan;
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					sltqBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
			}
			break;
		case 18://���
			{
				CFJBan fjBan;
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					fjBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
			}
			break;
		case 19://���߷ɳ�(û������)
			{
				CRayBan rayBan;
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					rayBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
			}
			break;
		case 22://����ɭ��
			{
				CMFBan mfBan;
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					mfBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				
			}
			break;
		default:
			return 0;
			break;
		}
		operBan.RecordData(1,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);
		sprintf(m_result,"��ʱ��ͣ�ʺ�%s�ɹ�",UserName);
		switch(GameID)
		{
		case 35://������II
			{
				WriteLog("������II",m_result);			
			}
			break;
		case 34://����
			{
				WriteLog("����Online",m_result);	
			}
			break;
		case 31://�ɽ�
			{
				WriteLog("�ɽ�Online",m_result);	
			}
			break;
		case 25://ħ������II(�Ƚ����� ����ɫ��ͣ,���˺ŷ�ͣ)
			{
				WriteLog("ħ������II",m_result);	
			}	
			break;
		case 3://��������
			{

				WriteLog("��������",m_result);	
			}
			break;
		case 6://������
			{
				WriteLog("������",m_result);	
			}
			break;
	
		case 27://����
			{
				WriteLog("����",m_result);	
			}
			break;
		case 18://���
			{
				WriteLog("���",m_result);	
			}
			break;
		case 19://���߷ɳ�(û������)
			{
				WriteLog("���߷ɳ�",m_result);	
			}
			break;
		case 22://����ɭ��
			{
				WriteLog("����ɭ��",m_result);					
			}
			break;
		default:
			return 0;
			break;
		}
		
	}

	return 1;
}
int OpenOper(int type,int userByID,int GameID,char* ServerIP,char * ServerName,char * UserID, char * UserName, char* NickID, char * UserNick, char * beginTime, char * endTime,char * reason, char *memo)
{
	COperBan operBan;
	char m_result[256];
	if(type==0)//�������
	{
		switch(GameID)
		{
		case 35://������II
			{
				CJW2Ban jw2Ban;
				//��ͣ�˺�
				if(!strcmp(UserID,"")||!strcmp(UserID,"NULL"))
				{
					jw2Ban.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if(jw2Ban.OpenPlayer(GameID,ServerIP,ServerName,UserName)>0)
				{
					operBan.RecordData(2,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
					sprintf(m_result,"����ʺ�%s�ɹ�",UserName);
					WriteLog("������II",m_result);
				}
				else
				{
					return 0;
				}
			
			}
			break;
		case 34://����
			{
				CGTBan gtBan;
				//��ͣ�˺�
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					gtBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if(gtBan.OpenPlayer(GameID,ServerIP,ServerName,UserName)>0)
				{
					operBan.RecordData(2,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
					sprintf(m_result,"����ʺ�%s�ɹ�",UserName);
					WriteLog("����Online",m_result);
				}
				else
				{
					return 0;
				}
			}
			break;
		case 31://�ɽ�
			{
				CPalBan palBan;
				//��ͣ�˺�
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					palBan.FindAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if(palBan.OpenPlayer(GameID,ServerIP,ServerName,UserName)>0)
				{
					operBan.RecordData(2,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
					sprintf(m_result,"����ʺ�%s�ɹ�",UserName);
					WriteLog("�ɽ�Online",m_result);
				}
				else
				{
					return 0;
				}

			}
			break;
		case 25://ħ������II
			{
				CCG2Ban cg2Ban;
				//����˺�
				if(!strcmp(memo,"1") )//�����ͣ�˺�
				{
					if(!strcmp(UserName,""))
					{
						cg2Ban.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);//ͨ���ǳƲ��˺�
					}
					strcpy(UserNick,"NULL");	
				}
				if(cg2Ban.OpenPlayer(GameID,ServerIP,ServerName,UserName,UserNick,reason,memo)>0)
				{
					operBan.RecordData(2,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
					sprintf(m_result,"����ʺ�%s�ɹ�",UserName);
					WriteLog("ħ������II",m_result);
				}
				else
				{
					return 0;
				}

			}
			break;
		case 3://��������
			{
				CSDOBan sdoBan;
				//��ͣ�˺�
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					sdoBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if(sdoBan.OpenPlayer(GameID,ServerIP,ServerName,UserName)>0)
				{
					operBan.RecordData(2,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
					sprintf(m_result,"����ʺ�%s�ɹ�",UserName);
					WriteLog("��������",m_result);
				}
				else
				{
					return 0;
				}	
			}
			break;
		case 6://������
			{
				CAUBan auBan;
				//��ͣ�˺�
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					auBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if(auBan.OpenPlayer(GameID,ServerIP,ServerName,UserName)>0)
				{
					operBan.RecordData(2,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
					sprintf(m_result,"����ʺ�%s�ɹ�",UserName);
					WriteLog("������",m_result);
				}
				else
				{
					return 0;
				}

			}
			break;
	/*	case 24://SD�ߴ�
			{
				CSDBan sdBan;
				//��ͣ�˺�
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					sdBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if(sdBan.OpenPlayer(GameID,ServerIP,ServerName,UserName)>0)
				{
					operBan.RecordData(2,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,reason,beginTime,endTime,memo);	
				}
				else
				{
					return 0;
				}

			}
			break;*/
		case 27://����
			{
				CSltqBan sltqBan;

				//����˺�(�ж���Ӧ�ò���ִ��)
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					sltqBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}

				if(sltqBan.OpenPlayer(GameID,ServerIP,ServerName,UserName,UserNick))
				{
					operBan.RecordData(2,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
					sprintf(m_result,"����ʺ�%s�ɹ�",UserName);
					WriteLog("����",m_result);
				}
				else
				{
					return 0;
				}

			}
			break;
		case 18://���
			{
				CFJBan fjBan;
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					fjBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				//��ͣ�˺�
				if(fjBan.OpenPlayer(GameID,ServerIP,ServerName,UserName)>0)
				{
					operBan.RecordData(2,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
					sprintf(m_result,"����ʺ�%s�ɹ�",UserName);
					WriteLog("���",m_result);
				}
				else
				{
					return 0;
				}
			}
			break;
		case 19://���߷ɳ�
			{
				CRayBan rayBan;
				//��ͣ�˺�
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					rayBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if(rayBan.OpenPlayer(GameID,ServerIP,ServerName,UserName)>0)
				{
					operBan.RecordData(2,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
					sprintf(m_result,"����ʺ�%s�ɹ�",UserName);
					WriteLog("���߷ɳ�",m_result);
				}
				else
				{
					return 0;
				}

			}
			break;
		case 22://����ɭ��
			{
				CMFBan mfBan;
				//��ͣ�˺�
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					mfBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if(mfBan.OpenPlayer(GameID,ServerIP,ServerName,UserName)>0)
				{
					operBan.RecordData(2,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
					sprintf(m_result,"����ʺ�%s�ɹ�",UserName);
					WriteLog("����ɭ��",m_result);
				}
				else
				{
					return 0;
				}

			}
			break;
		default:
			return 0;
			break;
		}
	}
	else//��ʱ���������ݿ�
	{
		operBan.RecordData(2,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);
		sprintf(m_result,"��ʱ����ʺ�%s�ɹ�",UserName);
		switch(GameID)
		{
		case 35://������II
			{
				WriteLog("������II",m_result);			
			}
			break;
		case 34://����
			{
				WriteLog("����Online",m_result);	
			}
			break;
		case 31://�ɽ�
			{
				WriteLog("�ɽ�Online",m_result);	
			}
			break;
		case 25://ħ������II(�Ƚ����� ����ɫ��ͣ,���˺ŷ�ͣ)
			{
				WriteLog("ħ������II",m_result);	
			}	
			break;
		case 3://��������
			{
				
				WriteLog("��������",m_result);	
			}
			break;
		case 6://������
			{
				WriteLog("������",m_result);	
			}
			break;
			
		case 27://����
			{
				WriteLog("����",m_result);	
			}
			break;
		case 18://���
			{
				WriteLog("���",m_result);	
			}
			break;
		case 19://���߷ɳ�(û������)
			{
				WriteLog("���߷ɳ�",m_result);	
			}
			break;
		case 22://����ɭ��
			{
				WriteLog("����ɭ��",m_result);					
			}
			break;
		default:
			return 0;
			break;
		}
	}
	return 1;
}
int RemoteTest(int GameID,char* ServerIP,char * ServerName,char * UserID, char * UserName, char* NickID, char * UserNick)
{
	switch(GameID)
	{
	case 22://����ɭ��
		{
			CMFBan mfBan;
			
		}
		break;
	case 3://��������
		break;
	default:
		break;
	}
	return 1;
}

int QueryAccount(int GameID,char *ServerIP,char *ServerName,char *UserName,char *UserNick)
{
	try
	{
		switch(GameID)
		{
		case 35://������II
			{
				CJW2Ban jw2Ban;
				jw2Ban.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				
			}
			break;
		case 34://����
			{
				CGTBan gtBan;
				gtBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);

				
			}
			break;
		case 31://�ɽ�
			{
				CPalBan palBan;
				palBan.FindAccount(GameID,ServerIP,ServerName,UserName,UserNick);
		
			}
			break;
		case 25://ħ������II(�Ƚ����� ����ɫ��ͣ,���˺ŷ�ͣ)
			{
				
				CCG2Ban cg2Ban;
				cg2Ban.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				
			}	
			break;
		case 3://��������
			{
				CSDOBan sdoBan;
				sdoBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
			}
			break;
		case 6://������
			{
				CAUBan auBan;
				auBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
			}
			break;
	
		case 27://����
			{
				CSltqBan sltqBan;
				sltqBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
			}
			break;
		case 18://���
			{
				CFJBan fjBan;
				fjBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
			}
			break;
		case 19://���߷ɳ�(û������)
			{
				CRayBan rayBan;
				rayBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
			}
			break;
		case 22://����ɭ��
			{
				CMFBan mfBan;
				mfBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
			
			}
			break;
		default:
			return 0;
			break;
		}
	}
	catch(...)
	{

	}
	return 1;
}
//д��־��Ϣ
void WriteLog(char* gameName,char * lpText)
{
	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);//��ȡϵͳ��ǰʱ��
	
	char path[256];
	GetCurrentDirectory(256,path);//��ȡ��ǰ����Ŀ¼
	char filepath[256];
	sprintf(filepath,"%s%s",path,"\\log\\GMServer%04i%02i%02i.log");
	char filename[256];
	sprintf(filename,filepath,nowtime.wYear,nowtime.wMonth,nowtime.wDay);//��ȡ�ļ�·��
	
	ofstream out;
	out.open(filename,ios::app);//����־�ļ�
	if (!out) 
	{ 
		cout<<"Open File Error"<<endl; 
	} 
	
	//����־�ļ���д���û����;�������
	cout<<nowtime.wYear<<"-"<<setfill('0')<<setw(2)<<nowtime.wMonth<<"-"
		<<setw(2)<<nowtime.wDay<<" "
		<<setw(2)<<nowtime.wHour<<":"
		<<setw(2)<<nowtime.wMinute<<":"
		<<setw(2)<<nowtime.wMinute;
	cout<<"---"<<gameName<<"---";
	cout<<lpText<<endl;
	
	out<<nowtime.wYear<<"-"<<setfill('0')<<setw(2)<<nowtime.wMonth<<"-"
		<<setw(2)<<nowtime.wDay<<" "
		<<setw(2)<<nowtime.wHour<<":"
		<<setw(2)<<nowtime.wMinute<<":"
		<<setw(2)<<nowtime.wMinute;
	out<<"---"<<gameName<<"---";
	out<<lpText<<"\n";
	out.close();
}
CBanOperApp theApp;
