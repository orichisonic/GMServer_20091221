// GMAPI.cpp: implementation of the CGMInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GMInfo.h"

//#include "ASTA.h"
//#include "ATAS.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGMInfo::CGMInfo()
{

}

CGMInfo::~CGMInfo()
{
}
//判断是否登陆成功
void CGMInfo::IsLogin()
{
	try
	{
		SOCKADDR_IN remoteaddr;//IP地址
		int iAddrSize=0;//IP地址的长度
		int length=0;
		//清空szMD5password
		ZeroMemory(szMD5Password,256);
		//md5加密
		MD5_Password(szMD5Password,&length,(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::PassWord,0)).lpdata,
			strlen((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::PassWord,0)).lpdata));

		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GM","127.0.0.1",4,0,0,"exec IsLogin '%s','%s','%s'",
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::UserName,0)).lpdata,
			szMD5Password,
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::MAC,0)).lpdata);
		if(!DBTFLV.empty())
		{
			ZeroMemory(&remoteaddr,sizeof(SOCKADDR_IN));
			getpeername(pGameSess->GetSocket(),(SOCKADDR   *)&remoteaddr,&iAddrSize); 

			/*CLogic::AddLog(pGameSess->GetSocket(),
			StrToInt((char *)&(GetDBVect()->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
			(char *)&(GetDBVect()->getTLVByTag(CEnumCore::TagName::UserName,0)).lpdata,
			(char *)&(GetDBVect()->getTLVByTag(CEnumCore::TagName::RealName,0)).lpdata,
			(char *)inet_ntoa(remoteaddr.sin_addr)); */
			CLogic::AddLog(pGameSess->GetSocket(),
				StrToInt((char *)&(COperVector(DBTFLV)).getTLVByTag(CEnumCore::TagName::UserByID,0).lpdata),
				(char *)&(COperVector(DBTFLV)).getTLVByTag(CEnumCore::TagName::UserName,0).lpdata,
				(char *)&(COperVector(DBTFLV)).getTLVByTag(CEnumCore::TagName::RealName,0).lpdata,
				(char *)inet_ntoa(remoteaddr.sin_addr));
			
			GameDBOper("GM","127.0.0.1",4,"exec UpdateMac %i,'%s'",
				StrToInt((char *)&((COperVector(DBTFLV)).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::MAC,0)).lpdata);
			logFile.WriteLog("GM",(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::UserName,0)).lpdata,"登陆成功");
			operGM.BuildTLV(COperVector(DBTFLV));
			
		}
		else
		{
			char passwd[40];
			char mac[40];
			int status=0;
			logFile.WriteLog("GM",(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::UserName,0)).lpdata,"登陆失败");//记LOG
			if(GetUserInfo((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::UserName,0)).lpdata,passwd,mac,&status))//得到用户信息
			{
				if(strcmp(passwd,szMD5Password))//比较密码
				{
					sprintf(retMessage,"%s","密码错误\n");
				}
				else if(strcmp(mac,(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::MAC,0)).lpdata))//比较MAC码
				{
					sprintf(retMessage,"%s","MAC码错误\n");
				}
				else if(status==0)//比较状态
				{
					sprintf(retMessage,"%s","状态错误\n");
				}
				else//其他情况
				{
					sprintf(retMessage,"%s","有效期错误\n");
				}

			}	
			else
			{
				sprintf(retMessage,"%s","用户名错误\n");
			}
			operGM.BuildTLV(retMessage);
		}
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::CONNECT_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"用户登录内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"用户登录文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"用户登录其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch(...)
	{
		//打印出SQL语句	
		logFile.WriteErrorLog("GM","catch error");
		//返回消息
		operGM.BuildTLV("连接错误");
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::CONNECT_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
	}

}
//查询模块和用户关系
void CGMInfo::User_Module_Query()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);
		logFile.WriteLog("GM",m_username,"查询模块和用户关系");


		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GM","127.0.0.1",4,0,0,"exec GMTOOLS_MODULE_QUERY %i",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::User_ID,0)).lpdata));
		if(!DBTFLV.empty())
		{
			operGM.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operGM.BuildTLV("没有模块");
		}
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::USER_MODULE_ADMIN,CEnumCore::ServiceKey::USER_MODULE_QUERY_RESP,operGM.GetSendBuf(),operGM.GetSendLength());

	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询模块和用户关系内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询模块和用户关系文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询模块和用户关系其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch(...)
	{
		operGM.BuildTLV("没有模块");
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::USER_MODULE_ADMIN,CEnumCore::ServiceKey::USER_MODULE_QUERY_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
	}

}
//分配模块
void CGMInfo::User_Module_Update()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//通过socket找username
		logFile.WriteLog("GM工具",m_username,"分配模块");//记LOG

		int	length=GameDBOper("GM","127.0.0.1",4,"exec  Gmtool_GmUserModule_Admin %i,'%s'",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::User_ID,0)).lpdata),
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::ModuleList,0)).lpdata);
		
		if(length==0)
		{
			//返回消息
			sprintf(retMessage,"%s","分配模块失败\n");
		}
		else
		{
			sprintf(retMessage,"%s","分配模块成功\n");
		}
		operGM.BuildTLV(retMessage);
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::USER_MODULE_ADMIN,CEnumCore::ServiceKey::USER_MODULE_UPDATE_RESP,operGM.GetSendBuf(),operGM.GetSendLength());

	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"分配模块内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"分配模块文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"分配模块其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch(...)
	{
		//返回消息
		operGM.BuildTLV("分配模块失败");
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::USER_MODULE_ADMIN,CEnumCore::ServiceKey::USER_MODULE_UPDATE_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
	}

}
//查询用户列表
void CGMInfo::User_Query_All()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//通过socket找username
		logFile.WriteLog("GM工具",m_username,"查询用户列表");//记LOG


		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GM","127.0.0.1",4,
			StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
			StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata),
			"exec Gmtool_UserInfo_Query %i",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata));
		if(!DBTFLV.empty())
		{
			operGM.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operGM.BuildTLV("没有用户");
		}
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::USER_ADMIN,CEnumCore::ServiceKey::USER_QUERY_ALL_RESP,operGM.GetSendBuf(),operGM.GetSendLength());

		
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询用户列表内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询用户列表文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询用户列表其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch(...)
	{
		operGM.BuildTLV("没有用户");
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::USER_ADMIN,CEnumCore::ServiceKey::USER_QUERY_ALL_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
	}
}

//查看特定游戏服务器
void  CGMInfo::Server_IP_Query()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//通过socket找username
		logFile.WriteLog("GM工具",m_username,"查看所有游戏服务器");//记LOG
		//构造SQL语句

		//////IDC///////////////////////////////
		/*	sprintf(sql,"exec ServerInfo_Query %i,%i,%i",
						StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::ServerInfo_GameID,0)).lpdata),
						StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::ServerInfo_GameDBID,0)).lpdata));
		
		sprintf(sql,"exec ServerInfo_Query %i,%i",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::ServerInfo_GameID,0)).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::ServerInfo_GameDBID,0)).lpdata));*/
	
		
		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GM","127.0.0.1",4,
			0,0,
			"exec ServerInfo_Query_New %i,%i",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::ServerInfo_GameID,0)).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::ServerInfo_GameDBID,0)).lpdata));
		if(!DBTFLV.empty())
		{
			operGM.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operGM.BuildTLV("没有游戏服务器");
		}
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::SERVERINFO_IP_QUERY_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查看特定游戏服务器内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查看特定游戏服务器文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查看特定游戏服务器其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
}

//查看所有游戏服务器
void CGMInfo::ServerInfo_IP_All_Query()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//通过socket找username
		logFile.WriteLog("GM工具",m_username,"查看所有游戏服务器");//记LOG
		vector<CGlobalStruct::TFLV> DBTFLV;
		//构造SQL语句
		if(StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::GameID,0)).lpdata)==0)
		{
			DBTFLV=GameDBQuery("GM","127.0.0.1",4,0,0,"exec ServerInfo_Query_NewALL '1'");
		}
		else
		{
			DBTFLV=GameDBQuery("GM","127.0.0.1",4,0,0,"exec ServerInfo_Query_NewALL '%s'",
				(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::GameID,0)).lpdata);
		}

		if(!DBTFLV.empty())
		{
			operGM.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operGM.BuildTLV("没有游戏服务器");
		}
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::SERVERINFO_IP_ALL_QUERY_RESP,operGM.GetSendBuf(),operGM.GetSendLength());

	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询游戏服务器内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询游戏服务器文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询游戏服务器其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
}
//更新GM帐号的过期日期
void CGMInfo::Gmtools_UserMac_Update()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//通过socket找username
		logFile.WriteLog("GM工具",m_username,"更新GM帐号的过期日期");//记LOG

		int	length=GameDBOper("GM","127.0.0.1",4,"exec GMTOOLS_UpdateMAC %i,%i,'%s'",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::User_ID,0)).lpdata),
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::Limit,0)).lpdata);
		
		if(length==0)
		{
			//返回消息
			sprintf(retMessage,"%s","更新GM帐号的过期日期失败");
		}
		else
		{
			sprintf(retMessage,"%s","更新GM帐号的过期日期成功");
		}
		operGM.BuildTLV(retMessage);
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::GMTOOLS_USERMAC_UPDATE_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
		
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"更新GM帐号的过期日期内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"更新GM帐号的过期日期文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"更新GM帐号的过期日期其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
}

//查看工具操作记录
void CGMInfo::Gmtools_OperateLog_Query()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//通过socket找username
		logFile.WriteLog("GM工具",m_username,"查看工具操作记录");//记LOG

		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GM","127.0.0.1",4,
			StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
			StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata),
			"exec GmTools_OperatorLog_NewQuery %i,%i,'%s','%s','%s'",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::User_ID,0)).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::GameID,0)).lpdata),
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::ServerInfo_IP,0)).lpdata,
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::BeginTime,0)).lpdata,
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::EndTime,0)).lpdata);
		if(!DBTFLV.empty())
		{
			operGM.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operGM.BuildTLV("没有记录");
		}
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::GMTOOLS_OperateLog_Query_RESP,operGM.GetSendBuf(),operGM.GetSendLength());

		
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查看工具操作记录内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查看工具操作记录文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查看工具操作记录其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
}

//查询部门列表
void CGMInfo::Depart_Query()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//通过socket找username
		logFile.WriteLog("GM工具",m_username,"查询部门列表");//记LOG

		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GM","127.0.0.1",4,
			0,0,
			"exec Depart_Query");
		if(!DBTFLV.empty())
		{
			operGM.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operGM.BuildTLV("没有部门");
		}
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::USER_ADMIN,CEnumCore::ServiceKey::DEPART_QUERY_RESP,operGM.GetSendBuf(),operGM.GetSendLength());

	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询部门列表内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询部门列表文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询部门列表其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
}

//创建用户
void CGMInfo::User_Create()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//通过socket找username
		logFile.WriteLog("GM工具",m_username,"创建用户");//记LOG
		ZeroMemory(szMD5Password,256);
		int length=0;
		MD5_Password(szMD5Password,&length,(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::PassWord,0)).lpdata,
								strlen((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::PassWord,0)).lpdata));//获取MD5加密后密码


		length=GameDBOper("GM","127.0.0.1",4,
			"exec Gmtool_Gminfo_Add_New %i,'%s','%s',%i,'%s','%s',%i,%i",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::UserName,0)).lpdata,
			szMD5Password,
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::DepartID,0)).lpdata),
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::RealName,0)).lpdata,
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::Limit,0)).lpdata,
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::User_Status,0)).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::SysAdmin,0)).lpdata));
		
		if(length==0)
		{
			//返回消息
			sprintf(retMessage,"%s","创建用户失败");
		}
		else
		{
			sprintf(retMessage,"%s","创建用户成功");
		}
		operGM.BuildTLV(retMessage);
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::USER_ADMIN,CEnumCore::ServiceKey::USER_CREATE_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
		
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"创建用户内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"创建用户文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"创建用户其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	////////IDC/////////////////////////
	/*	sprintf(sql,"exec Gmtool_Gminfo_Add %i,'%s','%s',%i,'%s','%s',%i,%i,'%s'",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::UserName,0)).lpdata,
			szMD5Password,
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::DepartID,0)).lpdata),
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::RealName,0)).lpdata,
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::Limit,0)).lpdata,
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::User_Status,0)).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::SysAdmin,0)).lpdata),
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::ServerInfo_IP,0)).lpdata);//构建SQL语句*/

}


//更新用户
void CGMInfo::User_Update()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//通过socket找username
		logFile.WriteLog("GM工具",m_username,"更新用户");//记LOG

		int length=GameDBOper("GM","127.0.0.1",4,
			"exec Gmtool_Gminfo_Edit %i,%i,'%s',%i,'%s',%i,%i,%i",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::User_ID,0)).lpdata),
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::RealName,0)).lpdata,
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::DepartID,0)).lpdata),
			(char*)&(recvVect.getTLVByTag(CEnumCore::TagName::Limit,0).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::User_Status,0)).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::OnlineActive,0)).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::SysAdmin,0)).lpdata));
		
		if(length==0)
		{
			//返回消息
			sprintf(retMessage,"%s","更新用户失败");
		}
		else
		{
			sprintf(retMessage,"%s","更新用户成功");
		}
		operGM.BuildTLV(retMessage);
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::USER_ADMIN,CEnumCore::ServiceKey::USER_UPDATE_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"更新用户内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"更新用户文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"更新用户其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
}

//删除用户
void CGMInfo::User_Delete()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//通过socket找username
		logFile.WriteLog("GM工具",m_username,"删除用户");//记LOG

		int	length=GameDBOper("GM","127.0.0.1",4,
			"exec Gmtool_Gminfo_Del %i,%i",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::User_ID,0)).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata));
		
		if(length==0)
		{
			//返回消息
			sprintf(retMessage,"%s","删除用户失败");
		}
		else
		{
			sprintf(retMessage,"%s","删除用户成功");
		}
		operGM.BuildTLV(retMessage);
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::USER_ADMIN,CEnumCore::ServiceKey::USER_DELETE_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"删除用户内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"删除用户文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"删除用户其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
}

//修改密码
void CGMInfo::User_Passwd_Modif()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//通过socket找username
		logFile.WriteLog("GM工具",m_username,"修改密码");//记LOG
		ZeroMemory(szMD5Password,256);
		int length=0;
		MD5_Password(szMD5Password,&length,(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::PassWord,0)).lpdata,
									strlen((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::PassWord,0)).lpdata));
	
		
		
		
		length=GameDBOper("GM","127.0.0.1",4,
			"exec Gmtool_PASSWD_Edit %i,%i,'%s'",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::User_ID,0)).lpdata),
			szMD5Password);
		
		if(length==0)
		{
			//返回消息
			sprintf(retMessage,"%s","修改密码失败");
		}
		else
		{
			sprintf(retMessage,"%s","修改密码成功");
		}
		operGM.BuildTLV(retMessage);
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::USER_ADMIN,CEnumCore::ServiceKey::USER_PASSWD_MODIF_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
		
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"修改密码内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"修改密码文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"修改密码其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
}

//游戏查询
void CGMInfo::Game_Query()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//通过socket找username
		logFile.WriteLog("GM工具",m_username,"游戏查询");//记LOG



		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GM","127.0.0.1",4,
			0,0,
			"exec  PAL_GAME_QUERY");
		if(!DBTFLV.empty())
		{
			operGM.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operGM.BuildTLV("游戏查询失败");
		}
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::GAME_ADMIN,CEnumCore::ServiceKey::GAME_QUERY_RESP,operGM.GetSendBuf(),operGM.GetSendLength());

	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"游戏查询内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"游戏查询文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"游戏查询其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
}


//查询游戏的模块列表
void CGMInfo::Game_Module_Query()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//通过socket找username
		logFile.WriteLog("GM工具",m_username,"查询游戏的模块列表");//记LOG
		
		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GM","127.0.0.1",4,
			0,0,
			"exec PAL_GAME_MODULE_QUERY");
		if(!DBTFLV.empty())
		{
			operGM.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operGM.BuildTLV("没有游戏模块");
		}
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::GAME_ADMIN,CEnumCore::ServiceKey::GAME_MODULE_QUERY_RESP,operGM.GetSendBuf(),operGM.GetSendLength());

	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询游戏的模块列表内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询游戏的模块列表文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询游戏的模块列表其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}

}
//文件比较
void CGMInfo::GM_FileCompare()
{
	try
	{
		//比较文件
		FUpdate.CompareFile((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::UpdateFileName,0)).lpdata,
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::UpdateFilePath,0)).lpdata,pGameSess);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"文件比较内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);		
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"文件比较文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"文件比较其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
}
//文件更新
void CGMInfo::GM_FileUpdate()
{
	try
	{
		//更新文件
		FUpdate.UpdateFile((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::UpdateFileName,0)).lpdata,
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::UpdateFilePath,0)).lpdata,
	    StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::UpdateFileSize,0)).lpdata),pGameSess);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"文件更新内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"文件更新文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"文件更新其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	

}

//获取UserName,PASSWORD,User_State,MAC,
bool CGMInfo::GetUserInfo(char * username,char * passwd,char * mac,int *user_state)
{
	try
	{

		
		char sql[256];
		ZeroMemory(sql,256);
		sprintf(sql, "exec GetUser '%s'",username);//构成SQL
		
		CSqlHelper m_SqlHelper;
		
		vector<CGlobalStruct::DataRecord> datarecord;//构建数据队列
		
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		m_SqlHelper.SqlHelperMain("GM",&datarecord,"127.0.0.1",sql,4,0,0);
		
		if(datarecord.empty())//如果记录为空
		{
			return false;
		}
		
		//获取查询的记录集的每一项
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("user_pwd", m_DataRecord.recordfieldname))//获取相应的LogDBIP
			{
				sprintf(passwd,"%s",m_DataRecord.recorddata);
			}
			else if(!strcmp("user_mac", m_DataRecord.recordfieldname))
			{
				sprintf(mac,"%s",m_DataRecord.recorddata);
			}
			else if(!strcmp("user_status", m_DataRecord.recordfieldname))
			{
				*user_state=StrToInt(m_DataRecord.recorddata);
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取登录用户信息内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取登录用户信息文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取登录用户信息其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
		return FALSE;
	}
	return true;
}

//Added by tengjie 2010-05-05:查询外挂进程信息
void CGMInfo::Gm_HarmFulProc_Query()
{
	try
	{
		CString path = "";
		CString filepath = "";
		CString lpIP = "";
		CSqlHelper m_SqlHelper;
		char sql[256];
		ZeroMemory(sql,256);
		GetCurrentDirectory(256,path.GetBuffer(0));//获取当前目录
		sprintf(filepath.GetBuffer(0),"%s%s",path.GetBuffer(0),"\\config\\scheme.ini");//获取scheme.ini文件路径
		GetPrivateProfileString("SERVER","OutAddress","127.0.0.1",lpIP.GetBuffer(0),40,filepath.GetBuffer(0));

		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GM","127.0.0.1",4,
			StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
			StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata),
			"exec Game_ToolsAnalysis %i,%i,'%s','%s','%s'",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::GameID,0)).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::ToolProcTypeID,0)).lpdata),
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::ServerInfo_IP,0)).lpdata,
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::ServerInfo_City,0)).lpdata,
			lpIP.GetBuffer(0));

		if(!DBTFLV.empty())
		{
			operGM.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operGM.BuildTLV("无外挂进程");
		}

		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::HARMFUL_PROC_QUERY_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询外挂进程信息内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询外挂进程信息文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询外挂进程信息其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch(...)
	{
		operGM.BuildTLV("无外挂进程");
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::HARMFUL_PROC_QUERY_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
	}
}


// void CGMInfo::Gm_HarmFulProc_Query()
// {
// 	try
// 	{
// 		CString path = "";
// 		CString filepath = "";
// 		CString lpIP = "";
// 		CString strVarValue ="";
// 		CString strTempValue ="";
// 		CSqlHelper m_SqlHelper;
// 		vector<string> strVect;
// 		char sql[256];
// 		string strtext = "";
// 		ZeroMemory(sql,256);
// 		GetCurrentDirectory(256,path.GetBuffer(0));//获取当前目录
// 		sprintf(filepath.GetBuffer(0),"%s%s",path.GetBuffer(0),"\\config\\scheme.ini");//获取scheme.ini文件路径
// 		GetPrivateProfileString("SERVER","OutAddress","127.0.0.1",lpIP.GetBuffer(0),40,filepath.GetBuffer(0));
// 
// 		CLogic::SocketToClientIP(pGameSess->GetSocket(),m_ClientIP);//通过socket找clientIP
// 		logFile.WriteLog("GM工具",m_ClientIP,"查询外挂进程信息");//记LOG
// 
// 		//sql.Format("%s","exec Game_GetVarValue '%s'",lpIP.GetBuffer(0));//构成SQL	
// 		sprintf(sql,"exec Game_GetVarValue '%s'",lpIP.GetBuffer(0));//构成SQL	
// 		vector<CGlobalStruct::DataRecord> datarecord;//构建数据队列
// 		
// 		if(!datarecord.empty())
// 		{
// 			datarecord.clear();
// 		}
// 		m_SqlHelper.SqlHelperMain("GM",&datarecord,"127.0.0.1",sql,4,0,0);
// 		
// 		if(datarecord.empty())//如果记录为空
// 		{
// 			logFile.WriteErrorLog("GM工具","外挂信息记录为空");//记LOG
// 			return;
// 		}
// 
// 		if(!strVect.empty())
// 			strVect.clear();
// 		
// 		//获取关键字记录集
// 		vector<CGlobalStruct::DataRecord>::iterator iter;
// 		vector<CGlobalStruct::TFLV> TempDBTFLV;
// 		CGlobalStruct::DataRecord  m_DataRecord;
// 		vector<string>::iterator pStrData = NULL;
// 		int iNum = 0,iDataSize,iMod,iLeave = 0;
// 
// 		if(!TempDBTFLV.empty())
// 			TempDBTFLV.clear();
// 
// 		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
// 		{
// 			m_DataRecord = *iter;
// 
// 			if(!strcmp("VAR", m_DataRecord.recordfieldname))
// 			{
// 				strVect.push_back(m_DataRecord.recorddata);
// 			}
// 		}
// 
// 		iDataSize = strVect.size()/10;
// 		iMod	  = strVect.size()%10;
// 
// 
// 		for( pStrData = strVect.begin(); pStrData != strVect.end(); pStrData++ )
// 		{
// 			strtext = *pStrData;
// 
// 			iNum++;
// 
// 			if(iNum == 1)
// 				strVarValue.Format("%s","[");
// 
// 			if(iNum == iDataSize && iMod != strVect.size() - iLeave*iDataSize)
// 			{
// 				iLeave++;
// 				strTempValue.Format("%s]",strtext.c_str());
// 				strcat(strVarValue.GetBuffer(0),strTempValue.GetBuffer(0));
// 				iNum = 0;
// 				vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GM","127.0.0.1",4,
// 					StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::Index + iLeave - 1,0)).lpdata),
// 					StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata),
// 					"exec Game_ToolsAnalysis %i,%i,'%s','%s','%s'",
// 					StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::GameID,0)).lpdata),
// 					StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::ToolProcTypeID,0)).lpdata),
// 					(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::ServerInfo_IP,0)).lpdata,
// 					lpIP.GetBuffer(0),strVarValue.GetBuffer(0));
// 
// 				strVarValue = "";
// 
// 				if(!DBTFLV.empty())
// 					CreateNewDBTFLV(TempDBTFLV,DBTFLV);
// 			}
// 			else if(iNum == strVect.size() - iLeave*iDataSize)
// 			{
// 				strTempValue.Format("%s]",strtext.c_str());
// 				strcat(strVarValue.GetBuffer(0),strTempValue.GetBuffer(0));
// 				vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GM","127.0.0.1",4,
// 					StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::Index + iLeave - 1,0)).lpdata),
// 					StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata),
// 					"exec Game_ToolsAnalysis %i,%i,'%s','%s','%s'",
// 					StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::GameID,0)).lpdata),
// 					StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::ToolProcTypeID,0)).lpdata),
// 					(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::ServerInfo_IP,0)).lpdata,
// 					lpIP.GetBuffer(0), strVarValue.GetBuffer(0));
// 				
// 				strVarValue = "";
// 
// 				if(!DBTFLV.empty())
// 					CreateNewDBTFLV(TempDBTFLV,DBTFLV);
// 			}
// 			else
// 			{
// 				strTempValue.Format("%s,",strtext.c_str());
// 				strcat(strVarValue.GetBuffer(0),strTempValue.GetBuffer(0));
// 			}
// 		}
// 		
// 		if(!TempDBTFLV.empty())
// 		{
// 			operGM.BuildTLV(COperVector(TempDBTFLV));
// 		}
// 		else
// 		{
// 			operGM.BuildTLV("无外挂进程");
// 		}
// 
// 		if(!datarecord.empty())
// 		{
// 			datarecord.clear();
// 		}
// 
// 		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::HARMFUL_PROC_QUERY_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
// 	}
// 	catch (CMemoryException* e)
// 	{
// 		e->GetErrorMessage(errMessage,256);
// 		sprintf(errMessage,"查询外挂进程信息内存不足,错误信息:%s",errMessage);
// 		logFile.WriteErrorLog("GM",errMessage);
// 	}
// 	catch (CFileException* e)
// 	{
// 		e->GetErrorMessage(errMessage,256);
// 		sprintf(errMessage,"查询外挂进程信息文件异常,错误信息:%s",errMessage);
// 		logFile.WriteErrorLog("GM",errMessage);
// 	}
// 	catch (CException* e)
// 	{
// 		e->GetErrorMessage(errMessage,256);
// 		sprintf(errMessage,"查询外挂进程信息其他异常,错误信息:%s",errMessage);
// 		logFile.WriteErrorLog("GM",errMessage);
// 	}
// 	catch(...)
// 	{
// 		operGM.BuildTLV("无外挂进程");
// 		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::USER_ADMIN,CEnumCore::ServiceKey::USER_QUERY_ALL_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
// 	}
// }
// 
// void CGMInfo::CreateNewDBTFLV(vector<CGlobalStruct::TFLV> &NewDBTFLV, vector<CGlobalStruct::TFLV> DBTFLV)
// {
// 	if(DBTFLV.empty())
// 		return;
// 
// 	vector<CGlobalStruct::TFLV>::iterator pDbTflv = NULL;
// 	CGlobalStruct::TFLV szTflv;
// 
// 	for(pDbTflv = DBTFLV.begin(); pDbTflv != DBTFLV.end(); pDbTflv++)
// 	{
// 		szTflv = *pDbTflv;
// 
// 		NewDBTFLV.push_back(szTflv);
// 	}
// 
// }

//查询帐号是否为vip用户
void CGMInfo::GM_Net_vipAccountInfo(char * account)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV tflv;
	vector<string> strResult;
	char strConnect[512];
	ZeroMemory(strConnect, 512);
	
	try
	{
		printf("account:%s\n",account);
		sprintf(strConnect, "http://61.152.150.158/PayCenter/check_vip_rule.php");
		if(!operGM.getWebInfo(strConnect, account, strResult))
		{
			operGM.BuildTLV("CGMInfo::GM_Net_vipAccountInfo----连接错误");
			return;
		}
		else
		{
			int i;
			int isVip = FALSE;
			//char patOK[20] = "RESULT_00";
			string patOK = "RESULT_00";

			string tmp = operGM.getXMLNodeAttribute(strResult,"status");
			if(patOK == tmp)
				isVip = TRUE;
			printf("%s is vip account? %d\n", account, isVip);
			CGlobalStruct::TFLV tflv;
			
			tflv.nIndex = DBTFLV.size() + 1;
			tflv.m_tagName = CEnumCore::TagName::PlayerAccount;
			tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			tflv.m_tvlength = strlen(account);
			sprintf((char *)&tflv.lpdata,"%s", account);
			DBTFLV.push_back(tflv);
			
			tflv.nIndex = DBTFLV.size() + 1;
			tflv.m_tagName = CEnumCore::TagName::GM_AccountInfo_Vip;
			tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
			tflv.m_tvlength = sizeof(int);
			sprintf((char *)&tflv.lpdata, "%d", isVip);
			DBTFLV.push_back(tflv);
			
			if(!DBTFLV.empty())
				operGM.BuildTLV(DBTFLV);

			operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::GM_AccountInfo_Vip_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
	
			//DBTFLV = operGM.ReturnOpMsg(operName, "查询成功", "vip用户查询");
			return;
		}
		
	}
	catch (...)
	{
		//return operGM.ReturnOpMsg(operName, "异常", "vip用户查询");
		return;
	}
	
	//return DBTFLV = operGM.ReturnOpMsg(operName, "查询失败", "vip用户查询");
	return;
}

//踢人
/*
BOOL CGMInfo::GM_Net_KickPlayer(int userByID,char * ServerIP, char * UserName, char * Character)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	char retMessage[255];
	memset(retMessage, 0, 255);
	BOOL ret;

	////////--踢人--///////////////////////////////////////////////////////////
	try
	{
		REQ_KICK_USER req_kick_user;		//角色查询请求
		REP_KICK_USER rep_kick_user;		//角色查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];		
		
		req_packetLength = sizeof(REQ_KICK_USER);
		
		memset(sendbuf,0, 1024);
		memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
		memcpy(sendbuf+2, &flag, 2);			//固定字段
		
		
		if (!Connect_Remote_Login(ServerIP))
		{
			//DBTFLV = operGM.BuildTLV("无外挂进程");;
			logFile.WriteLog("GM",userByID, "[GM踢人][登录服务器失败]");
			ret = FALSE;
			//return;
		}
		else
		{
			sprintf(retMessage, "GM踢人：<大区>%s, <帐号>%s, <角色名>%s", ServerIP, UserName, Character);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG
			
			if(!strcmp(UserName,""))		//角色踢人
			{
				req_kick_user.kickCondition = ADMIN::QC_CHARACTERNAME;
				memcpy(&req_kick_user.char_name, Character, strlen(Character)+1);
				
			}
			else							//帐号踢人
			{
				req_kick_user.kickCondition = ADMIN::QC_USERNAME;
				memcpy(&req_kick_user.user_name, UserName, strlen(UserName)+1);
			}
			//大区
			req_kick_user.srv_realm_id=srv_Realmid;
			//服务器组ID
			req_kick_user.srv_cluster_id=srv_ClusterID;
			memcpy(sendbuf+4, &req_kick_user, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				//DBTFLV = operGM.BuildTLV("[GM踢人]--[发送请求失败]");
				//return DBTFLV;
				logFile.WriteLog("GM",userByID, "[GM踢人][发送请求失败]");
				ret = FALSE;
			}
			else
			{
				memset(recvbuf, 0, 10240);
				int recvbyte = m_MySocket.MyRecv(recvbuf, 10240);	//默认超时时间20秒
				if (recvbyte>0 && strcmp(recvbuf, ""))
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_kick_user, recvbuf+4, packetSize);
					
					switch (rep_kick_user.eResult)
					{
					case REP_KICK_USER::E_SUCCESS:
						//DBTFLV = operGM.BuildTLV("[GM踢人][踢人成功]");
						logFile.WriteLog("GM",userByID, "[GM踢人][踢人成功]");
						ret = TRUE;
						break;
					case REP_KICK_USER::E_FAILED_CHAR_NOTINGAME:
						//DBTFLV = operGM.BuildTLV("[GM踢人][角色不在线]");
						logFile.WriteLog("GM",userByID, "[GM踢人][角色不在线]");
						ret = TRUE;
						break;
					default:
						//DBTFLV = operGM.BuildTLV("[GM踢人]--[踢人失败]");
						logFile.WriteLog("GM", userByID, "[GM踢人][踢人失败]");
						ret = FALSE;
						break;
					}
				}
				else
				{
					//DBTFLV = operGM.BuildTLV("[GM踢人]--[踢人超时]");
					logFile.WriteLog("GM", userByID, "[GM踢人][踢人超时]");
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		sendbuf = NULL;
		delete []recvbuf;
		recvbuf = NULL;
	}
	catch(...)
	{
		//DBTFLV = operGM.BuildTLV("[GM踢人]--[未知错误]");
		logFile.WriteLog("GM", userByID, "[GM踢人][未知异常]");
		ret = FALSE;
		return ret;
	}	
	//m_OperatorWA.GmOperLog(userByID, ServerIP, "GM踢人", retMessage, DBTFLV[0]);
	return ret;
}


//批量封停(暂时只支持勇士无双)
void CGMInfo::GM_Net_ClosePlayerGroup(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character,int iTime, int Reason, char *description)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{
		//Added by tengjie 2010-08-05
// 		New_Time.Format("%d", iTime/60);
// 		if(!UserLogInterface(ServerIP,UserName,New_Time.GetBuffer(0),38,"1",1).compare("RESULT_6"))
// 		{
// 			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "已自助封停", "帐号封停");
// 			WA_SqlForUserInfo(UserName,ServerIP,iTime,description);
// 			return DBTFLV;
// 		}

		
		if (!GM_Net_KickPlayer(userByID, ServerIP, UserName, Character))
		{
			DBTFLV = operGM.ReturnOpMsg(operName, "该角色仍在线或发生异常", "帐号封停");
		}


		REQ_CLOSE_ACCOUNT req_close_account;		//封停帐号请求
		REP_CLOSE_ACCOUNT rep_close_account;		//封停帐号回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];

		//Added by tengjie 2010-07-30
		CString New_Time = "";

		req_packetLength = sizeof(REQ_CLOSE_ACCOUNT);
		
		memset(sendbuf,0, 1024);
		memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
		memcpy(sendbuf+2, &flag, 2);			//固定字段

		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = operGM.ReturnOpMsg(operName, "登录游戏服务器失败", "帐号封停");
		}
		else
		{
			sprintf(retMessage, "帐号封停：<大区>%s, <帐号>%s, <时间>%d分, <原因>%d, <描述>%s", ServerIP, UserName, iTime, Reason, description);			
			logFile.WriteLog("GM",operName, retMessage);//记LOG

			if(!strcmp(UserName,""))	//角色封停
			{
				req_close_account.closeCondition = ADMIN::QC_CHARACTERNAME;
				memcpy(&req_close_account.char_name, Character, strlen(Character)+1);
				
			}
			else						//帐号封停
			{
				req_close_account.closeCondition = ADMIN::QC_USERNAME;
				memcpy(&req_close_account.user_name, UserName, strlen(UserName)+1);
			}

			switch (Reason)
			{
// 			case 0:	// 未知
// 				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_UNKNOWN;
// 				break;
			case 1:	// 系统封停
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_SYSTEM;
				break;
			case 2: // 用户自助
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_SYSTEM_TEMP;
				break;
			case 3: // 用户自助
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_USERCUSTOM;
				break;
			case 4: // 用户临时自助
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_USERCUSTOM_TEMP;
				break;
			default:// 未知
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_SYSTEM;
				break;
			}
			//大区
			req_close_account.srv_realm_id=srv_Realmid;
			//服务器组ID
			req_close_account.srv_cluster_id=srv_ClusterID;

			req_close_account.duration_time = iTime;

			//Modified by tengjie 2010-05-13
//			strcpy(req_close_account.description, description);		
			strcpy(req_close_account.description, "");
			memcpy(sendbuf+4, &req_close_account, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "帐号封停");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				int recvbyte = m_MySocket.MyRecv(recvbuf, 10240);	//默认超时时间20秒
				if (recvbyte>0 && strcmp(recvbuf, ""))
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_close_account, recvbuf+4, packetSize);
					
					switch (rep_close_account.eResult)
					{
					case REP_CLOSE_ACCOUNT::E_SUCCESS:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "封停成功", "帐号封停");				
						//Added by tengjie 2010-05-13
						GM_SqlForCloseUserInfo(UserName,ServerIP,iTime,description);
						//Added by tengjie 2010-07-29:帐户中心记录日志接口
						New_Time.Format("%d", iTime/60);
						UserLogInterface(ServerIP,UserName,New_Time.GetBuffer(0),38,"1");
						break;
					case REP_CLOSE_ACCOUNT::E_FAILED_ACCOUNT_NOTEXIST:	
						DBTFLV = operGM.ReturnOpMsg(operName, "帐号不存在", "帐号封停");
						break;
					default:
						DBTFLV = operGM.ReturnOpMsg(operName, "封停失败", "帐号封停");
						break;
					}
				}
				else
				{
					DBTFLV = operGM.ReturnOpMsg(operName, "帐号封停超时", "帐号封停");
				}
			}

			m_MySocket.Close();
		}
	
		delete []sendbuf;
		sendbuf = NULL;
		delete []recvbuf;
		recvbuf = NULL;
	}
	catch (...)
	{
		DBTFLV = operGM.ReturnOpMsg(operName, "未知错误", "帐号封停");
	}

	operGM.GmOperLog(userByID, ServerIP, "帐号封停", retMessage, DBTFLV[0]);

	if(!DBTFLV.empty())
		operGM.BuildTLV(DBTFLV);
	
	operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::GM_ClosePlayerGroup_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
	
}
*/

//本地记录封停原因,不再显示给用户(暂时只支持勇士无双)
void CGMInfo::GM_SqlForCloseUserInfo(char *UserName,char *ServerIP,int iTime,char *description,int iStatus)
{
	try{
		
		CString szTime = "";
		CString szSql = "";
		CSqlHelper m_SqlHelper;
		
		SYSTEMTIME timeDest;
		GetLocalTime(&timeDest);
		
		szTime.Format("%4d-%2d-%2d %2d:%2d:%2d",timeDest.wYear,timeDest.wMonth,timeDest.wDay,timeDest.wHour,timeDest.wMinute,timeDest.wSecond);
		
		szSql.Format("exec WA_CloseDescription '%s','%s','%s',%i,'%s',%i",UserName,ServerIP,szTime.GetBuffer(0),iTime,description,iStatus);
		
		m_SqlHelper.SqlHelperMain("127.0.0.1", szSql.GetBuffer(0), 4);
	}
	catch(...){
	}
}