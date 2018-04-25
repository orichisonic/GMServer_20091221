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
		CEnumCore::ServiceKey m_ServiceKey= (m_message_tag & 0x00FFFF) | 0x8000;//获取要回应的CEnumCore::ServiceKey
		COperVector * precvVect=GTInfo.initialize(pSess,m_message_tag, pbuf, length);

		if(!precvVect)//初始化类对象
		{
			return false;
		}
		switch((unsigned int)m_message_tag)
		{
			case CEnumCore::Message_Tag_ID::GT_UserInfo_Query://玩家角色信息
				{
					DBTFLV=theApp.UserInfo(precvVect);
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_User_DetailInfo_Query://详细信息
			case CEnumCore::Message_Tag_ID::GT_User_FamilyInfo_Query://家族信息
			case CEnumCore::Message_Tag_ID::GT_User_SentiInfo_Query://情侣信息
			case CEnumCore::Message_Tag_ID::GT_User_MarrigeInfo_Query://婚姻信息
			case CEnumCore::Message_Tag_ID::GT_UserPack_Query://背包物品资料信息
			case CEnumCore::Message_Tag_ID::GT_UserEquip_Query://角色装备信息
			case CEnumCore::Message_Tag_ID::GT_IMFriend_Query://好友信息
			case CEnumCore::Message_Tag_ID::GT_IMBlackList_Query://黑名单信息
			case CEnumCore::Message_Tag_ID::GT_IMSecretLove_Query://暗恋名单信息
			case CEnumCore::Message_Tag_ID::GT_PetInfo_Query://宠物信息（没有）
			case CEnumCore::Message_Tag_ID::GT_RoomInfo_Query://房屋信息
			case CEnumCore::Message_Tag_ID::GT_FriendTrade_Info://朋友买卖中角色信息
			case CEnumCore::Message_Tag_ID::GT_HelotInfo_Query://奴隶信息
				{
					DBTFLV=theApp.UserProperty(precvVect,m_message_tag);	
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_FriendTrade_Log://买卖日志
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
			case CEnumCore::Message_Tag_ID::GT_AllStopUser_Query://禁止图片列表
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
			case CEnumCore::Message_Tag_ID::GT_AllBanUser_Query://封停玩家列表
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
			case CEnumCore::Message_Tag_ID::GT_BanUser_Query:////查询单个玩家是否封停
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
			case CEnumCore::Message_Tag_ID::GT_FamilyInfo_Query://家族信息
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
			case CEnumCore::Message_Tag_ID::GT_MemberInfo_Query://家族成员信息
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
			case CEnumCore::Message_Tag_ID::GT_MailLog_Query://邮箱日志
			case CEnumCore::Message_Tag_ID::GT_TradeLog_Query://交易日志
			case CEnumCore::Message_Tag_ID::GT_TaskLog_Query://任务日志
			case CEnumCore::Message_Tag_ID::GT_UseItemLog_Query://使用道具日志
			case CEnumCore::Message_Tag_ID::GT_GuajiLog_Query://挂机日志
			case CEnumCore::Message_Tag_ID::GT_BugleLog_Query://大小喇叭日志
			case CEnumCore::Message_Tag_ID::GT_LoginLog_Query://登陆日志
				{
					DBTFLV=theApp.LogInfo(precvVect,m_message_tag);
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_MailInfo_Query://邮件信息
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
			case CEnumCore::Message_Tag_ID::GT_TreasureBox_Query://宝箱信息
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
			case CEnumCore::Message_Tag_ID::GT_ItemConsume_Query://购买道具消费信息
				{
					DBTFLV=GTInfo.GT_ItemBuy(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Account,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_UserID,0)).lpdata),
						///0没有道具名中间件发送方,1没有道具名其他件发送方,2没有道具名中间件接受方,3没有道具名其他件接受方
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Type,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ItemName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_StartTime,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_EndTime,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_Select_Item://道具模糊查询
				{
					DBTFLV=GTInfo.GT_Select_Item(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ItemName,0)).lpdata
						);
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_Mcoin_Rank://血拼战绩
				{
					DBTFLV=GTInfo.GT_Mcoin_Rank(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
						//////0今日消费，1昨日消费，2历史消费
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Type,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_Sns_Query://空间排行
				{
					DBTFLV=GTInfo.GT_Sns_Query(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
						/////0空间更新，1空间点击，2空间回复
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Type,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_Attr_Queue://属性排行
				{
					DBTFLV=GTInfo.GT_Attr_Queue(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
						//0体力，1脑力，2魅力，3花痴度，4迷人度，5综合指数
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Type,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_Card_Record://名片排行
				{
					DBTFLV=GTInfo.GT_Card_Record(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
						//0递送，1索取，2婚礼榜，3PK榜，4开启宝箱
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Type,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_FriendTrade_Queue://排行信息
				{
					DBTFLV=GTInfo.GT_FriendTrade_Queue(
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
						//0身价最贵，1最新被买，2资产最多，3现金最多，4折扣最多
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Type,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata)
						);
				}
				break;
			case CEnumCore::Message_Tag_ID::GT_Back_Money://退款反馈信息
				{
					//用户ID,服务器名字，服务器IP，用户ID，用户名，Money 数量
					DBTFLV=GTInfo.GT_Back_Money(
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerName,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ServerIP,0)).lpdata,
						(char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Account,0)).lpdata,
						StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_ID,0)).lpdata),
						//0,2,4,6中间件是否退款,其它其他件是否退款
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
			operGT.BuildTLV("数据库为空");
		}
		operGT.SendBuf(GTInfo.pGameSess,CEnumCore::Msg_Category::GT_ADMIN,m_ServiceKey,operGT.GetSendBuf(),operGT.GetSendLength());

		//清空
		if(!GTInfo.Destroy())
		{
			return false;
		}
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"主要程序内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"主要程序文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"主要程序其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return false;
	}
	return true;
}

//玩家角色信息
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
			);//查询玩家资料信息
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询用户资料内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询用户资料文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询用户资料其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	return DBTFLV;
	
}

//玩家其它信息的统一查询（userID为唯一标示符）
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
		operGT.TagIDToSqlType(m_message_tag,sql_type);//根据msg_tag_ID获得相应的sql语句
		DBTFLV=GTInfo.UserDefault("GT",sql_type,///查询玩家其它信息（userID为唯一标示符）
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
		sprintf(errMessage,"查询用户属性资料内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询用户属性资料文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询用户属性资料其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	return DBTFLV;
	
}
//查询玩家的日志信息
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
		operGT.TagIDToSqlType(m_message_tag,StrToInt((char *)&(precvVect->getTLVByTag(CEnumCore::TagName::GT_Type,0)).lpdata),sql_type);//根据Msg_Tag_ID获得sql语句
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
		sprintf(errMessage,"查询日志资料内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询日志资料文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询日志资料其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	return DBTFLV;
	
}
CGTApp theApp;


