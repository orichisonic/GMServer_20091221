#include "stdafx.h"
#include "time.h"
#include "JW2Info.h"
#include "AU2_Dll.h"
#include "md5.h"
#pragma comment(lib,"AU2_Dll.lib")
#pragma comment(lib,"md5.lib")

#include "OperTool.h"
#pragma comment(lib,"OperTool.lib")


//构造函数
CJW2Info::CJW2Info()
{
}

//析构函数
CJW2Info::~CJW2Info()
{
}

//查看购物，送礼（7－10）
vector<CGlobalStruct::TFLV> CJW2Info::JW2_SMALL_PRESENT_QUERY(char* ServerName,char * ServerIP,char * UserName,int JW2_UserSN,int iType,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_SMALL_PRESENT_QUERY","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
		
		operJW2.FindDBIP(ServerIP,ServerIP,4);
		//获取远程操作的Sql语句
		if(!operJW2.getRemoteSql("JW2","JW2_SMALL_PRESENT_QUERY",operRemoteSql,4))
		{
			return DBTFLV;
		}
		switch(iType)
		{
		case 1:
			DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,JW2_UserSN,"M","G");
			break;
		case 2:
			DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,JW2_UserSN,"M","M");
			break;
		case 3:
			DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,JW2_UserSN,"G","G");
			break;
		default:
			break;
		}

	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","查看购物/送礼内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","查看购物/送礼文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","查看购物/送礼其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_SMALL_PRESENT_QUERY Error");
		return DBTFLV;
	}
	return DBTFLV;
}
//道具查询
vector<CGlobalStruct::TFLV>  CJW2Info::JW2_ItemInfo_Query(char* ServerName,char * ServerIP,char * UserName,int JW2_UserSN,int iItemPos,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_ItemInfo_Query","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
		
		switch(iItemPos)
		{
		case 0://身上
			{
				if(!operJW2.getRemoteSql("JW2","JW2_ItemInfo_Query0",operRemoteSql,4))
				{
					return false;
				}
				operJW2.FindDBIP(ServerIP,ServerIP,ItemDB);
			}
			break;
		case 1://物品栏
			{
				if(!operJW2.getRemoteSql("JW2","JW2_ItemInfo_Query1",operRemoteSql,4))
				{
					return false;
				}
				operJW2.FindDBIP(ServerIP,ServerIP,ItemDB);
			}
			break;
		case 2://礼物栏
			{
				if(!operJW2.getRemoteSql("JW2","JW2_ItemInfo_Query2",operRemoteSql,4))
				{
					return false;
				}
				operJW2.FindDBIP(ServerIP,ServerIP,Messenger);
			}
			break;
		default:
			break;
		}
		

		DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,JW2_UserSN);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","道具查询内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","道具查询文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","道具查询其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		return DBTFLV;
	}
	return DBTFLV;
}
//T人功能GateWay
vector<CGlobalStruct::TFLV>  CJW2Info::KickPlayer(int userByID,char * ServerName,char * ServerIP,char * UserName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	COperatorJW2 operJW2;
	try
	{
		char logServerIP[40];
		sprintf(logServerIP,"%s",ServerIP);
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_BANISHPLAYER","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
		operJW2.FindDBIP(ServerIP,ServerIP,5);

		int res=0;
		res=BanishPlayer(ServerIP,UserName,operName);
		
		m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
		m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
		if(res==0)
		{
			
			sprintf((char *)&m_tflv.lpdata,"踢%s成功",UserName);
			
		}
		else
		{
			sprintf((char *)&m_tflv.lpdata,"踢%s失败",UserName);
		
		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","踢人操作内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","踢人操作文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","踢人操作其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		sprintf((char *)&m_tflv.lpdata,"踢%s失败",UserName);
		return DBTFLV;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;
}

//解封帐号logindb
vector<CGlobalStruct::TFLV>  CJW2Info::MakeOpen(int userByID,char * ServerName, char * ServerIP ,char * UserName,char * UserNick,
												int JW2_UserSN,char* Reason)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{
		COperatorJW2 operJW2;
		char sql1[256];
		char sql2[256];
		char sql3[256];
		char logServerIP[40];
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_ACCOUNT_OPEN","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
		sprintf(logServerIP,"%s",ServerIP);
		sprintf(sql1,"exec JW2_BanUserID '%s'",UserName);
		if(!operJW2.getRemoteSql("JW2","JW2_ACCOUNT_OPEN",operRemoteSql,4))
		{
			return DBTFLV;
		}
		sprintf(sql2,operRemoteSql,JW2_UserSN);
		sprintf(sql3,"exec JW2_UnBanUser %i, %i,'%s','%s','%s'",
			userByID,
			JW2_UserSN,
			UserName,
			logServerIP,
			Reason);
		operJW2.FindDBIP(ServerIP,ServerIP,8);
		DBTFLV=OpenPlayer("JW2",sql1,ServerIP,1,sql2,sql3);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","解封帐号内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","解封帐号文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","解封帐号其他异常");
		return DBTFLV;
	}
	return DBTFLV;
}


//根据帐号封停logindb
vector<CGlobalStruct::TFLV>  CJW2Info::MakeClose(int userByID,char * ServerName, char * ServerIP ,char * UserName,char * UserNick,
												 int JW2_UserSN, char* Reason)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{
		COperatorJW2 operJW2;
		char sql1[256];
		char sql2[256];
		char sql3[256];
		char logServerIP[40];
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_ACCOUNT_CLOSE","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
		sprintf(logServerIP,"%s",ServerIP);
		operJW2.FindDBIP(ServerIP,ServerIP,5);
		BanishPlayer(ServerIP,UserName,operName);
		sprintf(sql1,"exec JW2_BanUserID %s",UserName);///查询本地数据库此账号是否被封停
		if(!operJW2.getRemoteSql("JW2","JW2_ACCOUNT_CLOSE",operRemoteSql,4))
		{
			return DBTFLV;
		}
		sprintf(sql2,operRemoteSql,JW2_UserSN);///操作远程数据库
		sprintf(sql3,"exec JW2_BanUser %i, %i,'%s','%s','%s','%s'",
			userByID,
			JW2_UserSN,
			UserName,
			UserNick,
			logServerIP,
			Reason);///本地数据库记录log日志
		operJW2.FindDBIP(ServerIP,ServerIP,8);
		DBTFLV=ClosePlayer("JW2",sql1,ServerIP,1,sql2,sql3);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","封停帐号内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","封停帐号文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","封停帐号其他异常");
		return DBTFLV;
	}
	return DBTFLV;
}
//玩家人物登入/登出信息
vector<CGlobalStruct::TFLV>  CJW2Info::LoginInfo(char * ServerName, char * ServerIP,char * UserName, char * UserNick,char * IP,char * begintime, char * endtime,int iIndex, int iPageSize)
{

	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_LOGINOUT_QUERY","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);

		operJW2.FindDBIP(ServerIP,ServerIP,LoginDB);
		
		if(strcmp("",UserName))//帐号不为空
		{
			if(!operJW2.getRemoteSql("JW2","JW2_LOGINOUT_QUERYBYACCOUNT",operRemoteSql,4))
			{
				return DBTFLV;
			}
			DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,UserName,begintime,endtime);
		}
		else if(strcmp("",UserNick))
		{
			if(!operJW2.getRemoteSql("JW2","JW2_LOGINOUT_QUERYBYNICKNAME",operRemoteSql,4))
			{
				return false;
			}
			DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,UserNick,begintime,endtime);
		}
		else
		{
			if(!operJW2.getRemoteSql("JW2","JW2_LOGINOUT_QUERYBYIP",operRemoteSql,4))
			{
				return false;
			}
			DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,IP,begintime,endtime);

		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","玩家人物登入/登出信息内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","玩家人物登入/登出信息文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","玩家人物登入/登出信息其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_LOGINOUT_QUERY Error");
		return DBTFLV;
	}
	return DBTFLV;
}



//查询家族信息
vector<CGlobalStruct::TFLV>  CJW2Info::JW2_FAMILYINFO_QUERY(char * ServerName, char * ServerIP,char * FamilyName, int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_FAMILYINFO_QUERY","", operName,ServerName,FamilyName);
		operJW2.WriteText(strLog);
		
		operJW2.FindDBIP(ServerIP,ServerIP,Messenger);
		//获取远程操作的Sql语句
		if(!operJW2.getRemoteSql("JW2","JW2_FAMILYINFO_QUERY",operRemoteSql,4))
		{
			return false;
		}

		DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,"%",FamilyName,"%");
		
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","查询家族信息内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","查询家族信息文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","查询家族信息其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_COUPLELOG_QUERY Error");
		return DBTFLV;
	}
	return DBTFLV;
}
//查询家族相关;
vector<CGlobalStruct::TFLV>  CJW2Info::JW2_Family_Detail(CEnumCore::Message_Tag_ID m_tagID,char * ServerName, char * ServerIP,char * FamilyName, int FamilyID,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		char sql_type[256];
		operJW2.TagIDToSqlType(m_tagID,sql_type);
		operJW2.GetLogText(strLog, "BaseMessage", sql_type,"", operName,ServerName,FamilyName);
		operJW2.WriteText(strLog);
		
		operJW2.FindDBIP(ServerIP,ServerIP,operJW2.TagIDToDBID(m_tagID));
		//获取远程操作的Sql语句
		if(!operJW2.getRemoteSql("JW2",sql_type,operRemoteSql,4))
		{
			return false;
		}

		DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,FamilyID);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","查询家族相关信息内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","查询家族相关信息文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","查询家族相关信息其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_COUPLELOG_QUERY Error");
		return DBTFLV;
	}
	return DBTFLV;	
}

//修改密码
vector<CGlobalStruct::TFLV>  CJW2Info::JW2_MODIFY_PWD(int userByID, char * ServerName,char * ServerIP,char * UserName,int JW2_UserSN,char * szNewPw)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	COperatorJW2 operJW2;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		char logServerIP[40];
		sprintf(logServerIP,"%s",ServerIP);
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_MODIFY_PWD","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
	
		char m_Result[256];
		ZeroMemory(m_Result,256);
		///查询密码////////////
		sprintf(operszSql,"exec JW2_FindAccountPw '%s'",UserName);
		operJW2.QuerySingleValue("JW2",logdbip,0,operszSql,m_Result);

		if(strcmp(m_Result,""))//没有PASSWPRD
		{
			sprintf((char *)&m_tflv.lpdata,"%s","修改密码成功");
			m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
			DBTFLV.push_back(m_tflv);
			return DBTFLV;
		}
		else
		{
		
			if(!operJW2.getRemoteSql("JW2","JW2_Get_AccountPw",operRemoteSql,4))
			{
				return DBTFLV;
			}
			sprintf(operszSql,operRemoteSql,ServerIP);//构造完整查询的sql语句
			operJW2.FindDBIP(ServerIP,ServerIP,3);
			operJW2.QuerySingleValue("JW2",ServerIP,1,operszSql,m_Result);

			//获取远程操作的Sql语句
			
			if(!operJW2.getRemoteSql("JW2","JW2_MODIFY_PWD",operRemoteSql,4))
			{
				return DBTFLV;
			}
			char JW2_NewMd5[256];
			int	 iJW2_NewMd5=0;
			char JW2_NewPasswd[256];
			ZeroMemory(JW2_NewMd5,256);
			ZeroMemory(JW2_NewPasswd,256);
			sprintf(JW2_NewPasswd,"%s",szNewPw);
			MD5_EnCode(JW2_NewMd5,&iJW2_NewMd5,JW2_NewPasswd,strlen(JW2_NewPasswd));
			operJW2.FindDBIP(ServerIP,ServerIP,8);
			sprintf(operszSql,operRemoteSql,JW2_NewMd5,JW2_UserSN);//构造完整查询的sql语句

			int tmpLength =GameDBOper("JW2",ServerIP,1,operRemoteSql,JW2_NewMd5,JW2_UserSN);
			if(tmpLength>0)
			{
				sprintf((char *)&m_tflv.lpdata,"%s","修改密码成功");
				GameDBOper("JW2","127.0.0.1",4,"exec JW2_InsertAccountPw %i,'%s','%s','%s','%s'",
						userByID,logServerIP,UserName,m_Result,JW2_NewPasswd);
			}
			else
			{
				sprintf((char *)&m_tflv.lpdata,"%s","修改密码失败");
			}
		}

	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","修改密码内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","修改密码文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","修改密码其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		sprintf((char *)&m_tflv.lpdata,"%s","连接游戏服务器失败");	
		return DBTFLV;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;
}
//恢复密码
vector<CGlobalStruct::TFLV>  CJW2Info::JW2_RECALL_PWD(int userByID, char * ServerName,char * ServerIP,char * UserName,int JW2_UserSN)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	COperatorJW2 operJW2;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		char logServerIP[40];
		sprintf(logServerIP,"%s",ServerIP);
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_RECALL_PWD","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
		
		char m_Result[256];
		ZeroMemory(m_Result,256);
		///查询密码////////////
		sprintf(operszSql,"exec JW2_FindAccountPw '%s'",UserName);
		operJW2.QuerySingleValue("JW2",logdbip,0,operszSql,m_Result);
		if(strcmp(m_Result,""))//没有PASSWPRD
		{
				
			//获取远程操作的Sql语句!
			
			if(!operJW2.getRemoteSql("JW2","JW2_MODIFY_PWD",operRemoteSql,4))
			{
				return DBTFLV;
			}
			operJW2.FindDBIP(ServerIP,ServerIP,8);
			int tmpLength=GameDBOper("JW2",ServerIP,1,operRemoteSql,m_Result,JW2_UserSN);
			if(tmpLength>0)
			{
				sprintf((char *)&m_tflv.lpdata,"%s","还原密码成功");

				GameDBOper("JW2","127.0.0.1",4,"exec JW2_RecallAccountPw %i,'%s','%s'",
						userByID,logServerIP,UserName);
				
			}
			else
			{
				sprintf((char *)&m_tflv.lpdata,"%s","还原密码失败");
			}


		}
		else
		{
			sprintf((char *)&m_tflv.lpdata,"%s","密码没有修改");
		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","还原密码内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","还原密码文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","还原密码其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		sprintf((char *)&m_tflv.lpdata,"%s","连接游戏服务器失败");
		return DBTFLV;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;
}



//修改等级
vector<CGlobalStruct::TFLV> CJW2Info::JW2_MODIFYLEVEL_QUERY(int userByID, char * ServerName,char * ServerIP,char * UserName,int JW2_UserSN,int iLevel)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	COperatorJW2 operJW2;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		char logServerIP[40];
		sprintf(logServerIP,"%s",ServerIP);
		int tmpLength=0;
		char m_Result[256];
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_MODIFYLEVEL_QUERY","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
		
		//获取远程操作的Sql语句
		if(!operJW2.getRemoteSql("JW2","JW2_MODIFYLEVEL_QUERY1",operRemoteSql,4))
		{
			return DBTFLV;
		}
		operJW2.FindDBIP(ServerIP,ServerIP,7);

		tmpLength=GameDBOper("JW2",ServerIP,1,operRemoteSql,iLevel,JW2_UserSN);

		if(!operJW2.getRemoteSql("JW2","JW2_MODIFYLEVEL_QUERY2",operRemoteSql,4))
		{
			return DBTFLV;
		}
		operJW2.FindDBIP(ServerIP,ServerIP,9);
		tmpLength=GameDBOper("JW2",ServerIP,1,operRemoteSql,iLevel,JW2_UserSN);

		sprintf(operszSql,"exec JW2_LevelToExp %i",iLevel);
		operJW2.QuerySingleValue("JW2",logdbip,0,operszSql,m_Result);
		long iExp=StrToLong(m_Result);

		if(!operJW2.getRemoteSql("JW2","JW2_MODIFYEXP_QUERY",operRemoteSql,4))
		{
			return DBTFLV;
		}
		operJW2.FindDBIP(ServerIP,ServerIP,7);
		tmpLength=GameDBOper("JW2",ServerIP,1,operRemoteSql,iExp,JW2_UserSN);

		if(tmpLength>0)
		{
			sprintf((char *)&m_tflv.lpdata,"修改等级,新等级%i成功",iLevel);
			logFile.WriteDBLog(userByID,"劲舞团2","JW2_MODIFYLEVEL_QUERY",logServerIP,(char *)&m_tflv.lpdata);
		}
		else
		{
			sprintf((char *)&m_tflv.lpdata,"修改等级,新等级%i失败",iLevel);
		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","修改等级内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","修改等级文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","修改等级其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		sprintf((char *)&m_tflv.lpdata,"%s","连接游戏服务器失败");
		return DBTFLV;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;
}


//修改经验
vector<CGlobalStruct::TFLV> CJW2Info::JW2_MODIFYEXP_QUERY(int userByID, char * ServerName,char * ServerIP,char * UserName,int JW2_UserSN,int iExp)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	COperatorJW2 operJW2;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		char logServerIP[40];
		sprintf(logServerIP,"%s",ServerIP);
		int tmpLength=0;
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_MODIFYEXP_QUERY","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
		
		char m_Result[256];
		//获取远程操作的Sql语句
		if(!operJW2.getRemoteSql("JW2","JW2_MODIFYEXP_QUERY",operRemoteSql,4))
		{
			return DBTFLV;
		}
		operJW2.FindDBIP(ServerIP,ServerIP,7);
		tmpLength=GameDBOper("JW2",ServerIP,1,operRemoteSql,iExp,JW2_UserSN);

		sprintf(operszSql,"exec JW2_ExpToLevel %d",iExp);
		operJW2.QuerySingleValue("JW2",logdbip,0,operszSql,m_Result);
		int iLevel=StrToInt(m_Result);
		
		if(!operJW2.getRemoteSql("JW2","JW2_MODIFYLEVEL_QUERY1",operRemoteSql,4))
		{
			return DBTFLV;
		}
		operJW2.FindDBIP(ServerIP,ServerIP,7);
		tmpLength=GameDBOper("JW2",ServerIP,1,operRemoteSql,iLevel,JW2_UserSN);

		
		if(!operJW2.getRemoteSql("JW2","JW2_MODIFYLEVEL_QUERY2",operRemoteSql,4))
		{
			return DBTFLV;
		}
		operJW2.FindDBIP(ServerIP,ServerIP,9);
		tmpLength=GameDBOper("JW2",ServerIP,1,operRemoteSql,iLevel,JW2_UserSN);

		if(tmpLength>0)
		{
			sprintf((char *)&m_tflv.lpdata,"修改经验,新经验%i成功",iExp);
			logFile.WriteDBLog(userByID,"劲舞团2","JW2_MODIFYLEVEL_QUERY",logServerIP,(char *)&m_tflv.lpdata);
			
		}
		else
		{
			sprintf((char *)&m_tflv.lpdata,"修改经验,新经验%i失败",iExp);
		}
			
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","修改经验内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","修改经验文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","修改经验其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
	}
	catch (...)
	{
		sprintf((char *)&m_tflv.lpdata,"%s","连接游戏服务器失败");
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;
}

//删除道具
vector<CGlobalStruct::TFLV> CJW2Info::JW2_ITEM_DEL(int userByID,char * ServerName,char * ServerIP,
							char * UserName, int JW2_UserSN,char * avatarItem,char * avatarItemName,
							int iItemPos, int iItemNo)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	COperatorJW2 operJW2;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		char logServerIP[40];
		sprintf(logServerIP,"%s",ServerIP);
		int tmpLength=0;
		char retMessage[256];
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_ITEM_DEL","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);

		switch(iItemPos)
		{
		case 0://身上
			{
				//获取远程操作的Sql语句
				if(!operJW2.getRemoteSql("JW2","JW2_ITEM_DEL",operRemoteSql,4))
				{
					return false;
				}
				operJW2.FindDBIP(ServerIP,ServerIP,10);
				tmpLength=GameDBOper("JW2",ServerIP,1,operRemoteSql,avatarItem,JW2_UserSN);
			}
			break;
		case 1://物品栏
			{
				//获取远程操作的Sql语句
				if(!operJW2.getRemoteSql("JW2","JW2_ITEM_DEL",operRemoteSql,4))
				{
					return false;
				}
				operJW2.FindDBIP(ServerIP,ServerIP,10);
				tmpLength=GameDBOper("JW2",ServerIP,1,operRemoteSql,avatarItem,JW2_UserSN);
			}
			break;
		case 2://礼物栏
			{
				if(!operJW2.getRemoteSql("JW2","JW2_ITEM_DEL1",operRemoteSql,4))
				{
					return false;
				}
				operJW2.FindDBIP(ServerIP,ServerIP,9);
				tmpLength=GameDBOper("JW2",ServerIP,1,operRemoteSql,avatarItem,JW2_UserSN,iItemNo);
			}
			break;
		default:
			break;
		}
	
		if(tmpLength>0)
		{
			sprintf(retMessage,"删除道具成功,道具名：%s",avatarItemName);
			logFile.WriteDBLog(userByID,"劲舞团2","JW2_ITEM_DEL",logServerIP,retMessage);
			
		}
		else
		{
			sprintf(retMessage,"删除道具失败,道具名：%s",avatarItemName);
		}
		
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","删除道具内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","删除道具文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","删除道具其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		return DBTFLV;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;
}

//添加道具
vector<CGlobalStruct::TFLV> CJW2Info::JW2_ADD_ITEM(int userByID,char * ServerName, char * ServerIP ,char* UserName,int JW2_UserSN,
							char * strMailTitle,char * strMailContent,char* JW2_AvatarItem,int count)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	COperatorJW2 operJW2;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		char logServerIP[40];
		sprintf(logServerIP,"%s",ServerIP);
		char retMessage[256];
		int tmpLength=0;
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_ADD_ITEM","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);

		//获取远程操作的Sql语句
		if(!operJW2.getRemoteSql("JW2","JW2_ADD_ITEM",operRemoteSql,4))
		{
			return false;
		}
		operJW2.FindDBIP(ServerIP,ServerIP,9);//Item_db数据库

		int mID = 0;//商品ID
		int itemNum = 0;//道具数量
		char strMID[20];
		char strItemNum[20];
		ZeroMemory(strMID, 20);//初始化临时保存商品ID的变量		
		ZeroMemory(strItemNum, 20);//初始化临时保存道具数量的变量

		char strAddItemName[2048];//总的中文字
		char strtmpItemName[256];
		char strMIDName[256];//道具名
		ZeroMemory(strAddItemName,2048);
		ZeroMemory(strtmpItemName,256);
		ZeroMemory(strMIDName,256);
			
		int nNum = 1;//第一个字符的次数
		int mNum = 1;//第二个字符的次数
		
		for(int i=0;i<count;i++)
		{
			ZeroMemory(strMID, 20);//初始化临时保存商品ID的变量		
			ZeroMemory(strItemNum, 20);//初始化临时保存道具数量的变量
			if(i == 0)
			{
				operJW2.getSpecChar(JW2_AvatarItem, "", mNum, "*", nNum, strMID);
				operJW2.getSpecChar(JW2_AvatarItem, "*", nNum, "|", mNum, strItemNum);
				nNum++;
			}
			else if(i == count-1)
			{
				operJW2.getSpecChar(JW2_AvatarItem, "|", mNum, "*", nNum, strMID);
				operJW2.getSpecChar(JW2_AvatarItem, "*", nNum, "", mNum, strItemNum);
			}
			else
			{
				operJW2.getSpecChar(JW2_AvatarItem, "|", mNum, "*", nNum, strMID);
				operJW2.getSpecChar(JW2_AvatarItem, "*", nNum, "|", mNum+1, strItemNum);
				nNum++;
				mNum++;
			}
			mID=atoi(strMID);
			///通过ID找中文字
			sprintf(operszSql,"select b.itemname from jw2_item a,jw2_product b where a.itemcode=b.itemcode and a.productid=%i",mID);
			operJW2.QuerySingleValue("JW2","127.0.0.1",4,operszSql,strMIDName);
			itemNum = atoi(strItemNum);
			sprintf(strtmpItemName,"道具名%s(ID号%i)*数量:%i|",strMIDName,mID,itemNum);
			sprintf(strAddItemName,"%s%s",strAddItemName,strtmpItemName);

			for(int j=0;j<itemNum;j++)
			{
				tmpLength= GameDBOper("JW2",ServerIP,1,operRemoteSql,JW2_UserSN,strMailTitle,strMailContent,strMID);
			}




		}
	
		if(tmpLength>0)
		{
			sprintf((char *)&m_tflv.lpdata,"%s","Add_Item_Success");
			sprintf(retMessage,"添加账号%s道具成功,%s",UserName,JW2_AvatarItem);
			logFile.WriteDBLog(userByID,"劲舞团2","JW2_ADD_ITEM",logServerIP,retMessage);
			
		}
		else
		{
			sprintf((char *)&m_tflv.lpdata,"%s","Add_Item_Failed");
		}

		
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","添加道具内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","添加道具文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","添加道具其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		return DBTFLV;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;
}


//修改金钱
vector<CGlobalStruct::TFLV> CJW2Info::JW2_MODIFY_MONEY(int userByID, char * ServerName,char * ServerIP,char * UserName,int JW2_UserSN,int iMoney)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	COperatorJW2 operJW2;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		char logServerIP[40];
		sprintf(logServerIP,"%s",ServerIP);
		int tmpLength=0;
		char retMessage[256];
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_MODIFY_MONEY","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
		

		//获取远程操作的Sql语句
		if(!operJW2.getRemoteSql("JW2","JW2_MODIFY_MONEY",operRemoteSql,4))
		{
			return false;
		}
		operJW2.FindDBIP(ServerIP,ServerIP,7);
		tmpLength=GameDBOper("JW2",ServerIP,1,operRemoteSql,iMoney,JW2_UserSN);
		
		if(tmpLength>0)
		{
			sprintf(retMessage,"修改G币成功,新G币%i",iMoney);
		}
		else
		{
			sprintf(retMessage,"修改G币失败,新G币%i",iMoney);
		}
		logFile.WriteDBLog(userByID,"劲舞团2","JW2_MODIFY_MONEY",logServerIP,retMessage);
		sprintf((char *)&m_tflv.lpdata,"%s",retMessage);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","修改G币内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","修改G币文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","修改G币其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{

		return TRUE;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;
}


//模糊道具查询
vector<CGlobalStruct::TFLV> CJW2Info::JW2_ITEM_SELECT(int userByID,char * JW2_AvatarItemName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		int tmpLength=0;
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_ITEM_SELECT","", operName);
		operJW2.WriteText(strLog);
		
		DBTFLV=GameDBQuery("JW2","127.0.0.1",4,0,0,"exec JW2_ITEM_SELECT %i,'%s'",userByID,JW2_AvatarItemName);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","模糊道具查询内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","模糊道具查询文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","模糊道具查询其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	return DBTFLV;
}



//基地信息查询
vector<CGlobalStruct::TFLV> CJW2Info::JW2_BasicInfo_Query(char * ServerName, char * ServerIP,char * FamilyName, int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_BasicInfo_Query","", operName,ServerName,FamilyName);
		operJW2.WriteText(strLog);
		
		operJW2.FindDBIP(ServerIP,ServerIP,Messenger);
		//获取远程操作的Sql语句
		if(!operJW2.getRemoteSql("JW2","JW2_BasicInfo_Query",operRemoteSql,4))
		{
			return false;
		}
		
		DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,"%",FamilyName,"%");
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","基地信息查询内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","基地信息查询文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","基地信息查询其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_COUPLELOG_QUERY Error");
	}
	return DBTFLV;
}


//金钱日志查询
vector<CGlobalStruct::TFLV> CJW2Info::JW2_MoneyLog_Query(char * ServerName, char * ServerIP,char* UserName,
						 int JW2_UserSN,int iType,char * szStartTime,char * szEndTime,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_MoneyLog_Query","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
		
		operJW2.FindDBIP(ServerIP,ServerIP,LogDB);

		switch (iType)
		{
		case 2:
			if(!operJW2.getRemoteSql("JW2","JW2_MMoney_Log",operRemoteSql,4))
			{
				return DBTFLV;
			}
			break;
		case 3:
			if(!operJW2.getRemoteSql("JW2","JW2_GMoney_Log",operRemoteSql,4))
			{
				return DBTFLV;
			}
			break;
		default:
			break;
		}
		//获取远程操作的Sql语句
		DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,JW2_UserSN,szStartTime,szEndTime);

	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_MoneyLog_Query Error");
	}
	return DBTFLV;
}

//基金日志(0,捐赠,1,消费)
vector<CGlobalStruct::TFLV> CJW2Info::JW2_FamilyFund_Log(char * ServerName, char * ServerIP,char* FamilyName,
						 int FamilyID,int iStatus,char * szStartTime,char * szEndTime,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_FamilyFund_Log","", operName,ServerName,FamilyName);
		operJW2.WriteText(strLog);
		
		operJW2.FindDBIP(ServerIP,ServerIP,Messenger);

		if(iStatus==0)
		{
			if(!operJW2.getRemoteSql("JW2","JW2_FamilyFund_Log0",operRemoteSql,4))
			{
				return DBTFLV;
			}
	
		}
		else
		{
			if(!operJW2.getRemoteSql("JW2","JW2_FamilyFund_Log1",operRemoteSql,4))
			{
				return DBTFLV;
			}
		}
		//获取远程操作的Sql语句
		DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,FamilyID,szStartTime,szEndTime);
		
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","查询基金日志内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","查询基金日志文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","查询基金日志其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_FamilyFund_Log Error");
		return DBTFLV;
	}
	return DBTFLV;
}

//家族道具购买日志
vector<CGlobalStruct::TFLV> CJW2Info::JW2_FamilyBuyLog_Query(char * ServerName, char * ServerIP,char* FamilyName,
						 int FamilyID,char * szStartTime,char * szEndTime,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_FamilyBuyLog_Query","", operName,ServerName,FamilyName);
		operJW2.WriteText(strLog);
		
		operJW2.FindDBIP(ServerIP,ServerIP,Messenger);
		//获取远程操作的Sql语句
		if(!operJW2.getRemoteSql("JW2","JW2_FamilyBuyLog_Query",operRemoteSql,4))
		{
			return false;
		}
		DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,FamilyID,szStartTime,szEndTime);
		
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","家族道具购买日志内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","家族道具购买日志文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","家族道具购买日志其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_FamilyBuyLog_Query Error");
		return DBTFLV;
	}
	return DBTFLV;
}

//消费日志
vector<CGlobalStruct::TFLV> CJW2Info::JW2_CashMoney_Log(char * ServerName, char * ServerIP,char* UserName,
						 int JW2_UserSN,int iType,char * szStartTime,char * szEndTime,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_CashMoney_Log","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
		
		operJW2.FindDBIP(ServerIP,ServerIP,LogDB);
		//获取远程操作的Sql语句
		if(!operJW2.getRemoteSql("JW2","JW2_CashMoney_Log",operRemoteSql,4))
		{
			return DBTFLV;
		}
		DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,JW2_UserSN,iType,szStartTime,szEndTime);
	
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","消费日志内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","消费日志文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","消费日志其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_Task_Log Error");
		return DBTFLV;
	}
	return DBTFLV;
}

//批量踢人
vector<CGlobalStruct::TFLV> CJW2Info::JW2_Import_KickPlayer()
{
/*	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_Import_KickPlayer","", m_UserName,JW2_ServerName,JW2_UserName);
		operJW2.WriteText(strLog);
		
		operJW2.FindDBIP(JW2_ServerIP,JW2_ServerIP,5);
		char m_result[256];
		CLogic::SocketToRealName(m_hServerSocket,m_result);

		int res=0;
		char allUserName[2048];
		char retMessage[2048];
		char tmpMessage[256];
		ZeroMemory(retMessage,2048);
		sprintf(allUserName,"%s",(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::JW2_BoardMessage,0)).lpdata);

		
		char *username=strtok(allUserName,"|");//查询标志"|"
		while(username!=NULL)
		{
			res=BanishPlayer(JW2_ServerIP,username,m_result);
			
			if(res==0)
			{
				logFile.WriteDBLog(userByID,"劲舞团2","JW2_BANISHPLAYER",JW2_ServerIP,"踢人成功");
			}
			else
			{
				sprintf(tmpMessage,"踢%s失败\n",username);
				sprintf(retMessage,"%s%s",retMessage,tmpMessage);
				logFile.WriteDBLog(userByID,"劲舞团2","JW2_BANISHPLAYER",JW2_ServerIP,"踢人失败");
			}		
			username=strtok(NULL,"|");//查询标志";"
		}
		if(strlen(retMessage)>0)
		{
			operJW2.ReturnMessage(retMessage);
		}
		else
		{
			operJW2.ReturnMessage("批量踢人成功");
		}
		
		
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return false;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_Task_Log Error");
		operJW2.ReturnMessage("连接游戏服务器失败");
		return TRUE;
	}*/
	return true;
}

//封停帐号
vector<CGlobalStruct::TFLV> CJW2Info::JW2_Import_AccountClose()
{
	/*try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_Task_Log","", m_UserName,JW2_ServerName,JW2_UserName);
		operJW2.WriteText(strLog);
		
		char m_result[256];

		char allUserName[2048];
		char retMessage[2048];
		char tmpMessage[256];
		ZeroMemory(retMessage,2048);
		sprintf(allUserName,"%s",(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::JW2_BoardMessage,0)).lpdata);


		char *username=strtok(allUserName,"|");//查询标志"|"
		while(username!=NULL)
		{
			operJW2.FindDBIP(JW2_ServerIP,JW2_ServerIP,GameDB);
			if(!operJW2.getRemoteSql("JW2_ACCOUNTTOSN",operRemoteSql,1))
			{
				return false;
			}
			sprintf(operszSql,operRemoteSql,JW2_UserName);
			operJW2.QuerySingleValue(JW2_ServerIP,1,operszSql,m_result);
			JW2_UserSN=StrToInt(m_result);
			if(JW2_UserSN==0)
			{
				sprintf(tmpMessage,"封停%s失败\n",username);
				sprintf(retMessage,"%s%s",retMessage,tmpMessage);
				
			}
			else
			{
				CLogic::SocketToRealName(m_hServerSocket,m_result);
				operJW2.FindDBIP(JW2_ServerIP,JW2_ServerIP,5);
				int res=BanishPlayer(JW2_ServerIP,JW2_UserName,m_result);
				
				operJW2.FindDBIP(JW2_ServerIP,JW2_ServerIP,8);
				if(!operJW2.getRemoteSql("JW2_ACCOUNT_CLOSE",operRemoteSql,1))
				{
					return false;
				}
				sprintf(operszSql,operRemoteSql,JW2_UserSN);//构造完整查询的sql语句
				int tmpLength = DBVect.OperVectorMain("JW2", JW2_ServerIP, operszSql, 1);//构造从数据库查询得到的数据
				if(tmpLength>0)
				{
					sprintf(operszSql,"exec JW2_UnBanUser %i,%i,'%s','%s','%s'",
						userByID,JW2_UserSN,JW2_UserName,JW2_ServerIP,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::JW2_Reason,0)).lpdata);
					DBVect.OperVectorMain("JW2",logdbip,operszSql,4);
				}
				else
				{
					sprintf(tmpMessage,"封停%s失败\n",username);
					sprintf(retMessage,"%s%s",retMessage,tmpMessage);
				}
			}
			username=strtok(NULL,"|");//查询标志";"
		}
		if(strlen(retMessage)>0)
		{
			operJW2.ReturnMessage(retMessage);
		}
		else
		{
			operJW2.ReturnMessage("批量封停成功");
		}

		
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return false;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_Task_Log Error");
		operJW2.ReturnMessage("连接游戏服务器失败");
		return TRUE;
	}*/
	return true;
}

//批量解封
vector<CGlobalStruct::TFLV> CJW2Info::JW2_Import_AccountOpen()
{
/*	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_Task_Log","", m_UserName,JW2_ServerName,JW2_UserName);
		operJW2.WriteText(strLog);
		
		char m_result[256];
		
		char allUserName[2048];
		char retMessage[2048];
		char tmpMessage[256];
		ZeroMemory(retMessage,2048);
		sprintf(allUserName,"%s",(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::JW2_BoardMessage,0)).lpdata);
		
		
		char *username=strtok(allUserName,"|");//查询标志"|"
		while(username!=NULL)
		{
			operJW2.FindDBIP(JW2_ServerIP,JW2_ServerIP,GameDB);
			if(!operJW2.getRemoteSql("JW2_ACCOUNTTOSN",operRemoteSql,1))
			{
				return false;
			}
			sprintf(operszSql,operRemoteSql,JW2_UserName);
			operJW2.QuerySingleValue(JW2_ServerIP,1,operszSql,m_result);
			JW2_UserSN=StrToInt(m_result);
			if(JW2_UserSN==0)
			{
				sprintf(tmpMessage,"解封%s失败\n",username);
				sprintf(retMessage,"%s%s",retMessage,tmpMessage);
				
			}
			else
			{
			
				operJW2.FindDBIP(JW2_ServerIP,JW2_ServerIP,8);
				if(!operJW2.getRemoteSql("JW2_ACCOUNT_OPEN",operRemoteSql,1))
				{
					return false;
				}
				sprintf(operszSql,operRemoteSql,JW2_UserSN);//构造完整查询的sql语句
				int tmpLength = DBVect.OperVectorMain("JW2", JW2_ServerIP, operszSql, 1);//构造从数据库查询得到的数据
				if(tmpLength>0)
				{
					sprintf(operszSql,"exec JW2_BanUser %i, %i,'%s','%s','%s','%s'",
						userByID,JW2_UserSN,JW2_UserName,JW2_NickName,JW2_ServerIP,	(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::JW2_Reason,0)).lpdata);
					DBVect.OperVectorMain("JW2",logdbip,operszSql,4);
				}
				else
				{
					sprintf(tmpMessage,"解封%s失败\n",username);
					sprintf(retMessage,"%s%s",retMessage,tmpMessage);
				}
			}
			username=strtok(NULL,"|");//查询标志";"
		}
		if(strlen(retMessage)>0)
		{
			operJW2.ReturnMessage(retMessage);
		}
		else
		{
			operJW2.ReturnMessage("批量解封成功");
		}
		
		
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return false;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_Task_Log Error");
		operJW2.ReturnMessage("连接游戏服务器失败");
		return TRUE;
	}*/
	return true;
}



//更改GM状态
vector<CGlobalStruct::TFLV> CJW2Info::JW2_GM_Update(int userByID, char * ServerName,char * ServerIP,char * UserName,int JW2_UserSN,int iStatus)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	CGlobalStruct::TFLV m_tflv;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		char logServerIP[40];
		sprintf(logServerIP,"%s",ServerIP);
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_GM_Update","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
		///通过帐号查询UserSN
		
		operJW2.FindDBIP(ServerIP,ServerIP,7);
		
		//获取远程操作的Sql语句
		if(!operJW2.getRemoteSql("JW2","JW2_GM_Update",operRemoteSql,4))
		{

			return false;
		}
		
		int tmpLength = GameDBOper("JW2", ServerIP, 1,operRemoteSql,iStatus,JW2_UserSN);//构造从数据库查询得到的数据
		if(tmpLength>0)
		{
			sprintf((char *)&m_tflv.lpdata,"%s","更改GM状态成功");
			logFile.WriteDBLog(userByID,"劲舞团2","JW2_GM_Update",logServerIP,"更改GM状态成功");
		}
		else
		{
			sprintf((char *)&m_tflv.lpdata,"%s","更改GM状态失败");
			logFile.WriteDBLog(userByID,"劲舞团2","JW2_GM_Update",logServerIP,"更改GM状态失败");
		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","更改GM状态内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","更改GM状态文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","更改GM状态其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_Task_Log Error");
		return DBTFLV;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;
}

//删除权限
vector<CGlobalStruct::TFLV> CJW2Info::JW2_ProductManage_Del(char * ServerName, char * ServerIP,int userByID,int iTaskid)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	CGlobalStruct::TFLV m_tflv;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_PRODUCTMANAGE_DEL","", operName);
		operJW2.WriteText(strLog);

		int tmpLength=GameDBOper("JW2","127.0.0.1",4,"exec JW2_PRODUCTMANAGE_DEL %i",iTaskid);

		if(tmpLength>0)
		{
			sprintf((char *)&m_tflv.lpdata,"%s","删除权限成功！");
			logFile.WriteDBLog(userByID,"劲舞团2","JW2_PRODUCTMANAGE_DEL",ServerIP,"删除权限成功！");
		}
		else
		{
			logFile.WriteDBLog(userByID,"劲舞团2","JW2_PRODUCTMANAGE_DEL",ServerIP,"删除权限失败！");
			sprintf((char *)&m_tflv.lpdata,"%s","删除权限失败！");
		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","删除权限内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","删除权限文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","删除权限其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_Task_Log Error");
		return DBTFLV;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;
}

//查看权限
vector<CGlobalStruct::TFLV> CJW2Info::JW2_ProductManage_View(int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_ProductManage_View","", operName);
		operJW2.WriteText(strLog);
		
		
		DBTFLV=GameDBQuery("JW2","127.0.0.1",4,iIndex,iPageSize,"exec JW2_ProductManage_View");
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","查看权限内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","查看权限文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","查看权限其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_Task_Log Error");
		return DBTFLV;
	}
	return DBTFLV;
}
////maple add//////////验证权限
vector<CGlobalStruct::TFLV> CJW2Info::JW2_ProductManage_Validate(char * ServerName,char * ServerIP, int GMID,char * JW2_AvatarItem)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_ProductManage_Validate","", operName,ServerName);
		operJW2.WriteText(strLog);

		char result[128]=_T("F");
		
		int tmpLength=GameDBOper("JW2",ServerIP,4,"exec JW2_PRODUCTPOWER_Valid '%s',%i,'%s','%s'",
							ServerIP,GMID,JW2_AvatarItem,result);
		if(tmpLength>0)
		{
	
			int tmp=GameDBOper("JW2",ServerIP,4,"exec JW2_VaildResult '%s'",JW2_AvatarItem);

			if(tmp>0)
			{
				//获取远程操作的Sql语句
				if(!operJW2.getRemoteSql("JW2","JW2_GetValidateResult",operRemoteSql,4))
				{
					return false;
				}
				DBTFLV=GameDBQuery("JW2",logdbip,4,1,1,operRemoteSql);
			}
		}
		
		
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","验证权限内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","验证权限文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","验证权限其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_Task_Log Error");
		return DBTFLV;
	}
	return DBTFLV;
}
//验证通过在权限表中减去
vector<CGlobalStruct::TFLV> CJW2Info::JW2_ProductManage_Deduct(int userByID,int GMID,char * ServerIP, char * JW2_AvatarItem)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	CGlobalStruct::TFLV m_tflv;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_ProductManage_Deduct","", operName);
		operJW2.WriteText(strLog);
		
		int tmpLength =GameDBOper("JW2","127.0.0.1",4,"exec JW2_PRODUCT_Deduct %i,'%s','%s'",
			GMID,ServerIP,JW2_AvatarItem);

		if(tmpLength>0)
		{
			
			logFile.WriteDBLog(userByID,"劲舞团2","JW2_ProductManage_Deduct",ServerIP,"成功在权限表中减去并且添加道具成功!！");
			sprintf((char *)&m_tflv.lpdata,"%s","添加道具成功!");
		}
		else
		{
			logFile.WriteDBLog(userByID,"劲舞团2","JW2_ProductManage_Deduct",ServerIP,"在权限表中减去失败导致添加道具失败!！");
			sprintf((char *)&m_tflv.lpdata,"%s","添加道具失败!");
		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","在权限表中减去过程中内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","在权限表中减去过程中文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","在权限表中减去过程中其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_Task_Log Error");
		return DBTFLV;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;


}

//插入权限
vector<CGlobalStruct::TFLV> CJW2Info::JW2_ProductManage_Insert(int userByID,char * ServerName,char * ServerIP,char * JW2_AvatarItem,
										char * GMAccount,char * begintime,char * endtime,int totalprice)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	CGlobalStruct::TFLV m_tflv;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_ProductManage_Insert","", operName,ServerName);
		operJW2.WriteText(strLog);

		int tmpLength=GameDBOper("JW2",logdbip,4,"exec JW2_ITEMPOWNER_ADMIN '%s','%s','%s','%s','%s',%i",
								ServerIP,
								JW2_AvatarItem,
								GMAccount,
								begintime,
								endtime,
								totalprice);
		if(tmpLength>0)
		{
			sprintf((char *)&m_tflv.lpdata,"%s","增加权限成功!");
			GameDBOper("JW2","127.0.0.1",4,"exec JW2_ITEMPOWNER_LOG %i,'劲舞团2','JW2_ProductManage_Insert','%s','增加权限成功!'",
				userByID,ServerIP);
		}
		else
		{
			sprintf((char *)&m_tflv.lpdata,"%s","增加权限失败!");
			GameDBOper("JW2","127.0.0.1",4,"exec JW2_ITEMPOWNER_LOG %i,'劲舞团2','JW2_ProductManage_Insert','%s','增加权限失败!'",
				userByID,ServerIP);
		}

	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","插入权限内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","插入权限文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","插入权限其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_Task_Log Error");
		return DBTFLV;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;
}

