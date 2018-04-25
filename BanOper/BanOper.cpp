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
////输入：type=0立即封1,定时封
///    
////userByID
////	  
////输出是否成功1.成功
////
/////////////////////////////////////
int CloseOper(int type,int userByID,int GameID,char* ServerIP,char * ServerName,char * UserID, char * UserName, char * NickID, char * UserNick, char * beginTime, char * endTime, char * reason,char *memo)
{
	COperBan operBan;
	char m_result[256];
	if(type==0)//立即封停
	{
		switch(GameID)
		{
		case 35://劲舞团II
			{
				CJW2Ban jw2Ban;
				///踢人
				if(jw2Ban.KickPlayer(GameID,ServerIP,ServerName,UserName,userByID)==0)
				{
					//封停账号
					if(!strcmp(UserID,"")||!strcmp(UserID,"NULL"))
					{
						jw2Ban.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
					}
					if(jw2Ban.ClosePlayer(GameID,ServerIP,ServerName,UserName)>0)
					{
						operBan.RecordData(1,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
						sprintf(m_result,"封停帐号%s成功",UserName);
						WriteLog("劲舞团II",m_result);
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
		case 34://吉堂
			{
				CGTBan gtBan;
				///踢人
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					gtBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if(gtBan.KickPlayer(GameID,ServerIP,ServerName,UserName))
				{
					//封停账号
					if(gtBan.ClosePlayer(GameID,ServerIP,ServerName,UserName)>0)
					{
						operBan.RecordData(1,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
						sprintf(m_result,"封停帐号%s成功",UserName);
						WriteLog("吉堂Online",m_result);
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
		case 31://仙剑
			{
				CPalBan palBan;
				///踢人
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
					//封停账号
					if(palBan.ClosePlayer(GameID,ServerIP,ServerName,UserName)>0)
					{
						operBan.RecordData(1,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
						sprintf(m_result,"封停帐号%s成功",UserName);
						WriteLog("仙剑Online",m_result);

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
		case 25://魔力宝贝II(比较特殊 按角色封停,按账号封停)
			{
				
				CCG2Ban cg2Ban;
				if(!strcmp(memo,"1") )//如果封停账号
				{
					if(!strcmp(UserName,""))
					{
						cg2Ban.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);//通过昵称查账号
					}
					strcpy(UserNick,"");	
				}
				
				///踢人
				if(cg2Ban.KickPlayer(GameID,ServerIP,ServerName,UserName,UserNick,memo))
				{
					//封停账号
					if(cg2Ban.ClosePlayer(GameID,ServerIP,ServerName,UserName,UserNick,endTime,reason,memo))
					{
						//写数据库
						operBan.RecordData(1,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
						sprintf(m_result,"封停帐号%s成功",UserName);
						WriteLog("魔力宝贝II",m_result);
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
		case 3://超级舞者
			{
				CSDOBan sdoBan;
				///踢人
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					sdoBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if(sdoBan.KickPlayer(GameID,ServerIP,ServerName,UserName))
				{
					//封停账号
					if(sdoBan.ClosePlayer(GameID,ServerIP,ServerName,UserName)>0)
					{
						operBan.RecordData(1,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
						sprintf(m_result,"封停帐号%s成功",UserName);
						WriteLog("超级舞者",m_result);
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
		case 6://劲舞团
			{
				CAUBan auBan;
				///踢人
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					auBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				auBan.KickPlayer(GameID,ServerIP,ServerName,UserName);
				//封停账号
				if(auBan.ClosePlayer(GameID,ServerIP,ServerName,UserName))
				{
					operBan.RecordData(1,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
					sprintf(m_result,"封停帐号%s成功",UserName);
					WriteLog("劲舞团",m_result);
				}
				else
				{
						return 0;
				}
			}
			break;
	/*	case 24://SD高达
			{
				CSDBan sdBan;
				///踢人
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					sdBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if(sdBan.KickPlayer(GameID,ServerIP,ServerName,UserName))
				{
					//封停账号
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
		case 27://篮球
			{
				CSltqBan sltqBan;

				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL")||!strcmp(UserNick,"")||!strcmp(UserNick,"NULL"))
				{
					sltqBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if (!strcmp(UserNick,"")||!strcmp(UserNick,"NULL"))
				{
					WriteLog("篮球","角色不存在");
					return 0;
				}

				//踢人
				if(sltqBan.KickPlayer(GameID,ServerIP,ServerName,UserNick))
				{
					//封停账号
					if(sltqBan.ClosePlayer(GameID,ServerIP,ServerName,UserName,UserNick,beginTime,endTime))//ServerIP
					{
						operBan.RecordData(1,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
						sprintf(m_result,"封停帐号%s成功",UserName);
						WriteLog("篮球",m_result);
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
		case 18://风火
			{
				CFJBan fjBan;
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					fjBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				//封停账号
				if(fjBan.ClosePlayer(GameID,ServerIP,ServerName,UserName))
				{
					operBan.RecordData(1,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
					sprintf(m_result,"封停帐号%s成功",UserName);
					WriteLog("风火",m_result);
				}
				else
				{
					return 0;
				}
			}
			break;
		case 19://光线飞车(没有踢人)
			{
				CRayBan rayBan;
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					rayBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
			//	if(rayBan.KickPlayer(GameID,ServerIP,ServerName,UserName))
			//	{
					//封停账号
					if(rayBan.ClosePlayer(GameID,ServerIP,ServerName,UserName))
					{
						operBan.RecordData(1,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
						sprintf(m_result,"封停帐号%s成功",UserName);
						WriteLog("光线飞车",m_result);
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
		case 22://宠物森林
			{
				CMFBan mfBan;
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					mfBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if(mfBan.KickPlayer(GameID,ServerIP,ServerName,UserName))
				{
					//封停账号
					if(mfBan.ClosePlayer(GameID,ServerIP,ServerName,UserName,reason))
					{
						operBan.RecordData(1,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
						sprintf(m_result,"封停帐号%s成功",UserName);
						WriteLog("宠物森林",m_result);
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
	else//定时封停操作数据库
	{
		switch(GameID)
		{
		case 35://劲舞团II
			{
				CJW2Ban jw2Ban;
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					jw2Ban.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				
			}
			break;
		case 34://吉堂
			{
				CGTBan gtBan;

				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					gtBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				
			}
			break;
		case 31://仙剑
			{
				CPalBan palBan;
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					palBan.FindAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				
			}
			break;
		case 25://魔力宝贝II(比较特殊 按角色封停,按账号封停)
			{
				
				CCG2Ban cg2Ban;
				///踢人
				if(!strcmp(memo,"1") )//如果封停账号
				{
					if(!strcmp(UserName,""))
					{
						cg2Ban.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);//通过昵称查账号
					}
					strcpy(UserNick,"");	
				}
				
				
			}	
			break;
		case 3://超级舞者
			{
				CSDOBan sdoBan;
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					sdoBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				
			}
			break;
		case 6://劲舞团
			{
				CAUBan auBan;
				///踢人
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					auBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				
			}
			break;
	
		case 27://篮球
			{
				CSltqBan sltqBan;
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					sltqBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
			}
			break;
		case 18://风火
			{
				CFJBan fjBan;
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					fjBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
			}
			break;
		case 19://光线飞车(没有踢人)
			{
				CRayBan rayBan;
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					rayBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
			}
			break;
		case 22://宠物森林
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
		sprintf(m_result,"定时封停帐号%s成功",UserName);
		switch(GameID)
		{
		case 35://劲舞团II
			{
				WriteLog("劲舞团II",m_result);			
			}
			break;
		case 34://吉堂
			{
				WriteLog("吉堂Online",m_result);	
			}
			break;
		case 31://仙剑
			{
				WriteLog("仙剑Online",m_result);	
			}
			break;
		case 25://魔力宝贝II(比较特殊 按角色封停,按账号封停)
			{
				WriteLog("魔力宝贝II",m_result);	
			}	
			break;
		case 3://超级舞者
			{

				WriteLog("超级舞者",m_result);	
			}
			break;
		case 6://劲舞团
			{
				WriteLog("劲舞团",m_result);	
			}
			break;
	
		case 27://篮球
			{
				WriteLog("篮球",m_result);	
			}
			break;
		case 18://风火
			{
				WriteLog("风火",m_result);	
			}
			break;
		case 19://光线飞车(没有踢人)
			{
				WriteLog("光线飞车",m_result);	
			}
			break;
		case 22://宠物森林
			{
				WriteLog("宠物森林",m_result);					
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
	if(type==0)//立即解封
	{
		switch(GameID)
		{
		case 35://劲舞团II
			{
				CJW2Ban jw2Ban;
				//封停账号
				if(!strcmp(UserID,"")||!strcmp(UserID,"NULL"))
				{
					jw2Ban.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if(jw2Ban.OpenPlayer(GameID,ServerIP,ServerName,UserName)>0)
				{
					operBan.RecordData(2,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
					sprintf(m_result,"解封帐号%s成功",UserName);
					WriteLog("劲舞团II",m_result);
				}
				else
				{
					return 0;
				}
			
			}
			break;
		case 34://吉堂
			{
				CGTBan gtBan;
				//封停账号
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					gtBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if(gtBan.OpenPlayer(GameID,ServerIP,ServerName,UserName)>0)
				{
					operBan.RecordData(2,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
					sprintf(m_result,"解封帐号%s成功",UserName);
					WriteLog("吉堂Online",m_result);
				}
				else
				{
					return 0;
				}
			}
			break;
		case 31://仙剑
			{
				CPalBan palBan;
				//封停账号
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					palBan.FindAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if(palBan.OpenPlayer(GameID,ServerIP,ServerName,UserName)>0)
				{
					operBan.RecordData(2,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
					sprintf(m_result,"解封帐号%s成功",UserName);
					WriteLog("仙剑Online",m_result);
				}
				else
				{
					return 0;
				}

			}
			break;
		case 25://魔力宝贝II
			{
				CCG2Ban cg2Ban;
				//解封账号
				if(!strcmp(memo,"1") )//如果封停账号
				{
					if(!strcmp(UserName,""))
					{
						cg2Ban.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);//通过昵称查账号
					}
					strcpy(UserNick,"NULL");	
				}
				if(cg2Ban.OpenPlayer(GameID,ServerIP,ServerName,UserName,UserNick,reason,memo)>0)
				{
					operBan.RecordData(2,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
					sprintf(m_result,"解封帐号%s成功",UserName);
					WriteLog("魔力宝贝II",m_result);
				}
				else
				{
					return 0;
				}

			}
			break;
		case 3://超级舞者
			{
				CSDOBan sdoBan;
				//封停账号
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					sdoBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if(sdoBan.OpenPlayer(GameID,ServerIP,ServerName,UserName)>0)
				{
					operBan.RecordData(2,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
					sprintf(m_result,"解封帐号%s成功",UserName);
					WriteLog("超级舞者",m_result);
				}
				else
				{
					return 0;
				}	
			}
			break;
		case 6://劲舞团
			{
				CAUBan auBan;
				//封停账号
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					auBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if(auBan.OpenPlayer(GameID,ServerIP,ServerName,UserName)>0)
				{
					operBan.RecordData(2,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
					sprintf(m_result,"解封帐号%s成功",UserName);
					WriteLog("劲舞团",m_result);
				}
				else
				{
					return 0;
				}

			}
			break;
	/*	case 24://SD高达
			{
				CSDBan sdBan;
				//封停账号
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
		case 27://篮球
			{
				CSltqBan sltqBan;

				//解封账号(判断下应该不会执行)
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					sltqBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}

				if(sltqBan.OpenPlayer(GameID,ServerIP,ServerName,UserName,UserNick))
				{
					operBan.RecordData(2,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
					sprintf(m_result,"解封帐号%s成功",UserName);
					WriteLog("篮球",m_result);
				}
				else
				{
					return 0;
				}

			}
			break;
		case 18://风火
			{
				CFJBan fjBan;
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					fjBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				//封停账号
				if(fjBan.OpenPlayer(GameID,ServerIP,ServerName,UserName)>0)
				{
					operBan.RecordData(2,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
					sprintf(m_result,"解封帐号%s成功",UserName);
					WriteLog("风火",m_result);
				}
				else
				{
					return 0;
				}
			}
			break;
		case 19://光线飞车
			{
				CRayBan rayBan;
				//封停账号
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					rayBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if(rayBan.OpenPlayer(GameID,ServerIP,ServerName,UserName)>0)
				{
					operBan.RecordData(2,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
					sprintf(m_result,"解封帐号%s成功",UserName);
					WriteLog("光线飞车",m_result);
				}
				else
				{
					return 0;
				}

			}
			break;
		case 22://宠物森林
			{
				CMFBan mfBan;
				//封停账号
				if(!strcmp(UserName,"")||!strcmp(UserName,"NULL"))
				{
					mfBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				}
				if(mfBan.OpenPlayer(GameID,ServerIP,ServerName,UserName)>0)
				{
					operBan.RecordData(2,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);	
					sprintf(m_result,"解封帐号%s成功",UserName);
					WriteLog("宠物森林",m_result);
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
	else//定时解封操作数据库
	{
		operBan.RecordData(2,type,userByID,GameID,ServerIP,ServerName,UserID,UserName,NickID,UserNick,beginTime,endTime,reason,memo);
		sprintf(m_result,"定时解封帐号%s成功",UserName);
		switch(GameID)
		{
		case 35://劲舞团II
			{
				WriteLog("劲舞团II",m_result);			
			}
			break;
		case 34://吉堂
			{
				WriteLog("吉堂Online",m_result);	
			}
			break;
		case 31://仙剑
			{
				WriteLog("仙剑Online",m_result);	
			}
			break;
		case 25://魔力宝贝II(比较特殊 按角色封停,按账号封停)
			{
				WriteLog("魔力宝贝II",m_result);	
			}	
			break;
		case 3://超级舞者
			{
				
				WriteLog("超级舞者",m_result);	
			}
			break;
		case 6://劲舞团
			{
				WriteLog("劲舞团",m_result);	
			}
			break;
			
		case 27://篮球
			{
				WriteLog("篮球",m_result);	
			}
			break;
		case 18://风火
			{
				WriteLog("风火",m_result);	
			}
			break;
		case 19://光线飞车(没有踢人)
			{
				WriteLog("光线飞车",m_result);	
			}
			break;
		case 22://宠物森林
			{
				WriteLog("宠物森林",m_result);					
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
	case 22://宠物森林
		{
			CMFBan mfBan;
			
		}
		break;
	case 3://超级舞者
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
		case 35://劲舞团II
			{
				CJW2Ban jw2Ban;
				jw2Ban.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				
			}
			break;
		case 34://吉堂
			{
				CGTBan gtBan;
				gtBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);

				
			}
			break;
		case 31://仙剑
			{
				CPalBan palBan;
				palBan.FindAccount(GameID,ServerIP,ServerName,UserName,UserNick);
		
			}
			break;
		case 25://魔力宝贝II(比较特殊 按角色封停,按账号封停)
			{
				
				CCG2Ban cg2Ban;
				cg2Ban.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
				
			}	
			break;
		case 3://超级舞者
			{
				CSDOBan sdoBan;
				sdoBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
			}
			break;
		case 6://劲舞团
			{
				CAUBan auBan;
				auBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
			}
			break;
	
		case 27://篮球
			{
				CSltqBan sltqBan;
				sltqBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
			}
			break;
		case 18://风火
			{
				CFJBan fjBan;
				fjBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
			}
			break;
		case 19://光线飞车(没有踢人)
			{
				CRayBan rayBan;
				rayBan.QueryAccount(GameID,ServerIP,ServerName,UserName,UserNick);
			}
			break;
		case 22://宠物森林
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
//写日志信息
void WriteLog(char* gameName,char * lpText)
{
	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);//获取系统当前时间
	
	char path[256];
	GetCurrentDirectory(256,path);//获取当前运行目录
	char filepath[256];
	sprintf(filepath,"%s%s",path,"\\log\\GMServer%04i%02i%02i.log");
	char filename[256];
	sprintf(filename,filepath,nowtime.wYear,nowtime.wMonth,nowtime.wDay);//获取文件路径
	
	ofstream out;
	out.open(filename,ios::app);//打开日志文件
	if (!out) 
	{ 
		cout<<"Open File Error"<<endl; 
	} 
	
	//在日志文件中写入用户名和具体内容
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
