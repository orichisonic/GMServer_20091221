#include "stdafx.h"
#include "time.h"
#include "PalLogInfo.h"
//构造函数
CPalLogInfo::CPalLogInfo()
{
	ZeroMemory(strLog,2048);//初始化日志信息
	ZeroMemory(operRemoteSql, 2048);//初始化在远程数据库执行操作的sql语句
	ZeroMemory(logdbip, 20);//初始化服务器IP
}

//析构函数
CPalLogInfo::~CPalLogInfo()
{
}

//查询商城购买记录
bool CPalLogInfo::Shop_Buy_Query(char * ServerName,char * ServerIP,char * RoleName,int iMallType,
								 char * szBegintime, char * szEndtime,int iIndex, int iPageSize)
{
	try
	{
		int recordnum=0;
		int PageNum=0;
		sprintf(strLog,"<%s>,查询大区<%s>玩家<%s>商城购买记录",operName,ServerName,RoleName);
		logFile.WriteText("仙剑OL",strLog);
		vector <CGlobalStruct::TFLV> DBTFLV;
	    //根据servername查询logdbid
		operAssist.FindLogdbIP(logdbip,ServerName);

		//获取远程操作的Sql语句
		switch(iMallType)
		{
			case 0:
				{
					///查到记录数
					sprintf(operszSql,"select count(*) as countnum from logdb.item_itemmall_buy where (rolename='%s' or fromname='%s') and time>='%s'  and time<='%s'",
									RoleName,RoleName,szBegintime,szEndtime);
					recordnum=operAssist.FindRecordNum(logdbip,operszSql,1);
					if(recordnum==0)
					{
						operAssist.BuildTLV("查询数据库结果为空");
						operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ITEMMAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
						return false;

					}
					if(recordnum%iPageSize==0)
					{
						PageNum=recordnum/iPageSize;
					}
					else
					{
						PageNum=recordnum/iPageSize+1;
					}
					
					operAssist.getRemoteSql("PAL","PALLOG_ITEMMALL_ALL",operRemoteSql,4);

					if(!strcmp("",operRemoteSql))
					{
						operAssist.BuildTLV("数据库中找不到对应的sql语句");
						operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ITEMMAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
						return false;
					}
					DBTFLV=GameDBQuery("PAL",logdbip,1, 0, 0,operRemoteSql,RoleName,RoleName,szBegintime,szEndtime,(iIndex-1)*iPageSize,iPageSize);
					//DBTFLV=GameDBQuery("PAL",logdbip,1, iIndex, iPageSize,operRemoteSql,RoleName,RoleName,szBegintime,szEndtime,(iIndex-1)*iPageSize,iPageSize);
				}
				break;
			case 1:
				{
				}
				break;
			case 2:
				{
				}
				break;
		}
		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV),CEnumCore::ServiceKey::PALLOG_ITEMMAIL_RESP,PageNum);
		}
		else
		{
			operAssist.BuildTLV("数据库为空");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ITEMMAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"商城购买记录内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ITEMMAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"商城购买记录文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ITEMMAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"商城购买记录其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ITEMMAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,查询大区<%s>玩家<%s>商城购买记录发生系统错误",operName,ServerName,RoleName);
		logFile.WriteText("仙剑OL",strLog);//将错误信息写入日志中
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ITEMMAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	return true;
}

//查询邮箱信息
bool CPalLogInfo::MailBox_Info_Query(char * ServerName,char * ServerIP,char * RoleName,int RoleID, char * szBegintime, char * szEndtime,int iIndex, int iPageSize)
{
	try
	{
		int tmpLength=0;
		sprintf(strLog,"<%s>,查询大区<%s>玩家<%s>邮箱信息",operName,ServerName,RoleName);
		logFile.WriteText("仙剑OL",strLog);

		//根据servername查询logdbid
		operAssist.FindGameDBIP(logdbip,ServerName);

		char begintime[20];
		char endtime[20];
		ZeroMemory(begintime,20);//初始化开始时间
		ZeroMemory(endtime,20);//初始化结束时间

		struct tm t_BeginTime; //	tm结构体类型的变量，保存时间					
		sscanf(szBegintime,"%d-%d-%d %d:%d:%d",&t_BeginTime.tm_year,&t_BeginTime.tm_mon,&t_BeginTime.tm_mday,&t_BeginTime.tm_hour,&t_BeginTime.tm_min,&t_BeginTime.tm_sec);//将开始时间赋值给tm结构体
		t_BeginTime.tm_year = t_BeginTime.tm_year - 1900;
		t_BeginTime.tm_mon = t_BeginTime.tm_mon - 1;
		time_t BeginTime;//time_t类型的变量，保存时间的秒数表示
		BeginTime =mktime(&t_BeginTime); //获取封停时间到1970-1-1之间的时间的秒数表示
		
		struct tm t_EndTime; //	tm结构体类型的变量，保存结束时间
		sscanf(szEndtime,"%d-%d-%d %d:%d:%d",&t_EndTime.tm_year,&t_EndTime.tm_mon,&t_EndTime.tm_mday,&t_EndTime.tm_hour,&t_EndTime.tm_min,&t_EndTime.tm_sec);//将结束时间赋值给tm结构体
		t_EndTime.tm_year = t_EndTime.tm_year - 1900;
		t_EndTime.tm_mon = t_EndTime.tm_mon - 1;
		time_t EndTime;//time_t类型的变量，保存时间的秒数表示
		EndTime=mktime(&t_EndTime); //获取封停时间到1970-1-1之间的时间的秒数表示
		
		sprintf(begintime,"%ld",BeginTime);//获取开始时间
		sprintf(endtime,"%ld",EndTime);//获取结束时间
	
		operAssist.getRemoteSql("PAL","PALLOG_MAILBOX_INFO",operRemoteSql,4);
		if(!strcmp("",operRemoteSql))
		{
			operAssist.BuildTLV("数据库中找不到对应的sql语句");
			operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CHECKMAILBOX_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
			return false;
		}
		vector <CGlobalStruct::TFLV> DBTFLV=GameDBQuery("PAL",logdbip,1,0,0,operRemoteSql,RoleID%20,RoleID,begintime,endtime);
		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV),CEnumCore::ServiceKey::PAL_CHECKMAILBOX_QUERY_RESP);
		}
		else
		{
			operAssist.BuildTLV("数据库为空");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CHECKMAILBOX_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询邮箱信息内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CHECKMAILBOX_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询邮箱信息文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CHECKMAILBOX_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询邮箱信息其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CHECKMAILBOX_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,查询大区<%s>玩家<%s>邮箱信息发生系统错误",operName,ServerName,RoleName);
		logFile.WriteText("仙剑OL",strLog);//将错误信息写入日志中
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CHECKMAILBOX_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	return true;
}

//查询具体邮件的附件
bool CPalLogInfo::MailBox_Info_Detail(char * ServerName,char * ServerIP,char * RoleName,int MailID, char * szBegintime, char * szEndtime,int iIndex, int iPageSize)
{
	try
	{
		int tmpLength=0;
		sprintf(strLog,"<%s>,查询大区<%s>玩家<%s>邮箱信息",operName,ServerName,RoleName);
		logFile.WriteText("仙剑OL",strLog);
		
		//根据servername查询logdbid
		operAssist.FindGameDBIP(logdbip,ServerName);
		
		char begintime[20];
		char endtime[20];
		ZeroMemory(begintime,20);//初始化开始时间
		ZeroMemory(endtime,20);//初始化结束时间
		
		struct tm t_BeginTime; //	tm结构体类型的变量，保存时间					
		sscanf(szBegintime,"%d-%d-%d %d:%d:%d",&t_BeginTime.tm_year,&t_BeginTime.tm_mon,&t_BeginTime.tm_mday,&t_BeginTime.tm_hour,&t_BeginTime.tm_min,&t_BeginTime.tm_sec);//将开始时间赋值给tm结构体
		t_BeginTime.tm_year = t_BeginTime.tm_year - 1900;
		t_BeginTime.tm_mon = t_BeginTime.tm_mon - 1;
		time_t BeginTime;//time_t类型的变量，保存时间的秒数表示
		BeginTime =mktime(&t_BeginTime); //获取封停时间到1970-1-1之间的时间的秒数表示
		
		struct tm t_EndTime; //	tm结构体类型的变量，保存结束时间
		sscanf(szEndtime,"%d-%d-%d %d:%d:%d",&t_EndTime.tm_year,&t_EndTime.tm_mon,&t_EndTime.tm_mday,&t_EndTime.tm_hour,&t_EndTime.tm_min,&t_EndTime.tm_sec);//将结束时间赋值给tm结构体
		t_EndTime.tm_year = t_EndTime.tm_year - 1900;
		t_EndTime.tm_mon = t_EndTime.tm_mon - 1;
		time_t EndTime;//time_t类型的变量，保存时间的秒数表示
		EndTime=mktime(&t_EndTime); //获取封停时间到1970-1-1之间的时间的秒数表示
		
		sprintf(begintime,"%ld",BeginTime);//获取开始时间
		sprintf(endtime,"%ld",EndTime);//获取结束时间

		
		operAssist.getRemoteSql("PAL","PALLOG_MAILBOX_DETAIL",operRemoteSql,4);
		if(!strcmp("",operRemoteSql))
		{
			operAssist.BuildTLV("数据库中找不到对应的sql语句");
			operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CHECKMAILBOX_DETAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
			return false;
		}
		vector <CGlobalStruct::TFLV> DBTFLV=GameDBQuery("PAL",logdbip,1,0,0,operRemoteSql,MailID%20,MailID,begintime,endtime);
		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV),CEnumCore::ServiceKey::PAL_CHECKMAILBOX_DETAIL_RESP);
		}
		else
		{
			operAssist.BuildTLV("数据库为空");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CHECKMAILBOX_DETAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询具体邮件的附件内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CHECKMAILBOX_DETAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询具体邮件的附件文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CHECKMAILBOX_DETAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询具体邮件的附件其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CHECKMAILBOX_DETAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,查询大区<%s>玩家<%s>邮箱信息发生系统错误",operName,ServerName,RoleName);
		logFile.WriteText("仙剑OL",strLog);//将错误信息写入日志中
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CHECKMAILBOX_DETAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	return true;
}
//查询拍卖信息
bool CPalLogInfo::Auction_Info_Query(char * ServerName,char * ServerIP,char * RoleName,int RoleID, char * szBegintime, char * szEndtime,int iIndex, int iPageSize)
{
	try
	{
		COperVector tmpOperVector;
		int tmpOperVectorlength=0;
		int tmpLength=0;
		sprintf(strLog,"<%s>,查询大区<%s>玩家<%s>拍卖信息",operName,ServerName,RoleName);
		logFile.WriteText("仙剑OL",strLog);
		
		//根据servername查询logdbid
		operAssist.FindGameDBIP(logdbip,ServerName);

		operAssist.getRemoteSql("PAL","PALLOG_AUCTION_INFO",operRemoteSql,4);//构造根据IP查询拍卖信息的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.BuildTLV("数据库中找不到对应的sql语句");
			operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_AUCTION_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
			return false;
		}


		vector <CGlobalStruct::TFLV> DBTFLV=GameDBQuery("PAL",logdbip,1,iIndex,iPageSize,operRemoteSql,
			RoleID,RoleID,RoleID,RoleID,RoleID
			,RoleID,RoleID,RoleID,RoleID,RoleID
			,RoleID,RoleID,RoleID,RoleID,RoleID
			,RoleID,RoleID,RoleID,RoleID,RoleID
			,RoleID,RoleID,RoleID,RoleID,RoleID);
		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV),CEnumCore::ServiceKey::PAL_AUCTION_QUERY_RESP);
		}
		else
		{
			operAssist.BuildTLV("数据库为空");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_AUCTION_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询拍卖信息内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_AUCTION_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询拍卖信息文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_AUCTION_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询拍卖信息其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_AUCTION_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,查询大区<%s>玩家<%s>拍卖信息发生系统错误",operName,ServerName,RoleName);
		logFile.WriteText("仙剑OL",strLog);//将错误信息写入日志中
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_AUCTION_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	return true;
}

//查询日志信息
bool CPalLogInfo::LogInfo_Query(char * ServerName,char * ServerIP,char * RoleName,int iBigType,char * szBigType,
								int iSmallType,char * szSmallType,char * szBegintime, char * szEndtime,int iIndex, int iPageSize)
{
	try
	{
		char szTableName[100];
		char szSqlFlag[40];
		ZeroMemory(szTableName,100);
		ZeroMemory(szSqlFlag,40);
		int recordnum=0;
		int PageNum=0;
		vector <CGlobalStruct::TFLV> DBTFLV;

		sprintf(strLog,"%s, 查询大区<%s>玩家<%s>的日志信息:大类%s,小类%s", operName,ServerName,RoleName,szBigType,szSmallType);
		logFile.WriteText("仙剑OL",strLog);
		
		
		//根据servername查询logdbid
		operAssist.FindLogdbIP(logdbip,ServerName);
		
		operAssist.FindTableAndSql(iBigType,iSmallType,szTableName,szSqlFlag);
		//////////根据大小类，找到table name ,sql_flag/////////////
		if(iBigType == 5)//宠物大类比较特殊
		{
			sprintf(operszSql,"select count(*) as countnum from %s where eventtype=%i and rolename='%s' and time>='%s'and time<'%s' ",
				szTableName,iSmallType,RoleName,szBegintime,szEndtime);	
			recordnum=operAssist.FindRecordNum(logdbip,operszSql,1);
			if(recordnum==0)
			{
				operAssist.BuildTLV("查询数据库结果为空");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
			}
			if(recordnum%iPageSize==0)
			{
				PageNum=recordnum/iPageSize;
			}
			else
			{
				PageNum=recordnum/iPageSize+1;
			}
			operAssist.getRemoteSql("PAL",szSqlFlag,operRemoteSql,4);//获取远程操作的Sql语句
			if(!strcmp("",operRemoteSql))
			{
				operAssist.BuildTLV("数据库中找不到对应的sql语句");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
			}
			DBTFLV=GameDBQuery("PAL",logdbip,1,0,0,operRemoteSql,szTableName,
				iSmallType,RoleName,szBegintime,szEndtime,(iIndex-1)*iPageSize,iPageSize);
		}
		else if(iBigType == 4&& iSmallType!=28)//文钱测试
		{
			
			
			sprintf(operszSql,"select count(*) as countnum from %s where eventtype=%i and rolename='%s' and time>='%s'and time<'%s' ",
				szTableName,iSmallType,RoleName,szBegintime,szEndtime);	
			recordnum=operAssist.FindRecordNum(logdbip,operszSql,1);
			if(recordnum==0)
			{
				operAssist.BuildTLV("查询数据库结果为空");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;				
			}
			if(recordnum%iPageSize==0)
			{
				PageNum=recordnum/iPageSize;
			}
			else
			{
				PageNum=recordnum/iPageSize+1;
			}
			
			operAssist.getRemoteSql("PAL",szSqlFlag,operRemoteSql,4);//获取远程操作的Sql语句
			if(!strcmp("",operRemoteSql))
			{
				operAssist.BuildTLV("数据库中找不到对应的sql语句");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
			}
			DBTFLV=GameDBQuery("PAL",logdbip,1,0,0,operRemoteSql,szTableName,
				iSmallType,RoleName,szBegintime,szEndtime,(iIndex-1)*iPageSize,iPageSize);
		}
		else
		{
			if(iBigType==1&&iSmallType==8)
			{
				sprintf(operszSql,"select count(*) as countnum from %s where rolename='%s' and logintime>='%s' and logintime<'%s' ",
						szTableName,
						RoleName,
						szBegintime,
						szEndtime);//构造远程操作的sql语句
			}
			else if(iBigType==1&&iSmallType==2)
			{
				
				sprintf(operszSql,"select count(*) as countnum from %s where rolename='%s' and createtime>='%s' and createtime<'%s' ",
					szTableName,
					RoleName,
					szBegintime,
					szEndtime);//构造远程操作的sql语句

			}
			else if(iBigType==1&&iSmallType==9)
			{
				sprintf(operszSql,"select count(*) as countnum from %s where rolename='%s' and fromname='DelByTimeOut' and time>='%s' and time<'%s' ",
					szTableName,
					RoleName,
					szBegintime,
					szEndtime);//构造远程操作的sql语句
			}
			else
			{

				sprintf(operszSql,"select count(*) as countnum from %s where rolename='%s' and time>='%s' and time<'%s' ",
							szTableName,
							RoleName,
							szBegintime,
							szEndtime);//构造远程操作的sql语句
			}
			
			
			recordnum=operAssist.FindRecordNum(logdbip,operszSql,1);
			if(recordnum==0)
			{
				operAssist.BuildTLV("查询数据库结果为空");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
				
			}
			if(recordnum%iPageSize==0)
			{
				PageNum=recordnum/iPageSize;
			}
			else
			{
				PageNum=recordnum/iPageSize+1;
			}
			operAssist.getRemoteSql("PAL",szSqlFlag,operRemoteSql,4);//获取远程操作的Sql语句
			if(!strcmp("",operRemoteSql))
			{
				operAssist.BuildTLV("数据库中找不到对应的sql语句");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
			}
			DBTFLV=GameDBQuery("PAL",logdbip,1,0,0,operRemoteSql,szTableName,
				RoleName,szBegintime,szEndtime,(iIndex-1)*iPageSize,iPageSize);
		}
		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV),CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP,PageNum);
		}
		else
		{
			operAssist.BuildTLV("数据库为空");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());

	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询日志信息内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询日志信息文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询日志信息其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,查询大区<%s>玩家<%s>日志信息发生系统错误",operName,ServerName,RoleName);
		logFile.WriteText("仙剑OL",strLog);//将错误信息写入日志中
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	return TRUE;
}

//查询日志历史信息
bool CPalLogInfo::HisLogInfo_Query(char * ServerName,char * ServerIP,char * RoleName,int iBigType,char * szBigType,
								   int iSmallType,char * szSmallType,char * szBegintime, char * szEndtime,int iIndex, int iPageSize)
{
	try
	{
		char szTableName[100];
		char szSqlFlag[40];
		ZeroMemory(szTableName,100);
		ZeroMemory(szSqlFlag,40);
		int recordnum=0;
		int PageNum=0;
		vector <CGlobalStruct::TFLV> DBTFLV;

		sprintf(strLog,"%s, 查询大区<%s>玩家<%s>的日志信息:大类%s,小类%s", operName,ServerName,RoleName,szBigType,szSmallType);
		logFile.WriteText("仙剑OL",strLog);
		
		
		//根据servername查询logdbid
		operAssist.FindLogdbIP(logdbip,ServerName);
		
		operAssist.FindTableAndSql1(iBigType,iSmallType,szTableName,szSqlFlag);
		//////////根据大小类，找到table name ,sql_flag/////////////
		if(iBigType == 5)//宠物大类比较特殊
		{
			sprintf(operszSql,"select count(*) as countnum from %s where eventtype=%i and rolename='%s' and time>='%s'and time<'%s' ",
				szTableName,iSmallType,RoleName,szBegintime,szEndtime);	
			recordnum=operAssist.FindRecordNum(logdbip,operszSql,1);
			if(recordnum==0)
			{
				operAssist.BuildTLV("查询数据库结果为空");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_HISTORY_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
			}
			if(recordnum%iPageSize==0)
			{
				PageNum=recordnum/iPageSize;
			}
			else
			{
				PageNum=recordnum/iPageSize+1;
			}
			operAssist.getRemoteSql("PAL",szSqlFlag,operRemoteSql,4);//获取远程操作的Sql语句
			if(!strcmp("",operRemoteSql))
			{
				operAssist.BuildTLV("数据库中找不到对应的sql语句");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_HISTORY_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
			}
			DBTFLV=GameDBQuery("PAL",logdbip,1,0,0,operRemoteSql,szTableName,
				iSmallType,RoleName,szBegintime,szEndtime,(iIndex-1)*iPageSize,iPageSize);
		}
		else if(iBigType == 4&& iSmallType!=28)//文钱测试
		{
			
			
			sprintf(operszSql,"select count(*) as countnum from %s where eventtype=%i and rolename='%s' and time>='%s'and time<'%s' ",
				szTableName,iSmallType,RoleName,szBegintime,szEndtime);	
			recordnum=operAssist.FindRecordNum(logdbip,operszSql,1);
			if(recordnum==0)
			{
				operAssist.BuildTLV("查询数据库结果为空");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_HISTORY_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;				
			}
			if(recordnum%iPageSize==0)
			{
				PageNum=recordnum/iPageSize;
			}
			else
			{
				PageNum=recordnum/iPageSize+1;
			}
			
			operAssist.getRemoteSql("PAL",szSqlFlag,operRemoteSql,4);//获取远程操作的Sql语句
			if(!strcmp("",operRemoteSql))
			{
				operAssist.BuildTLV("数据库中找不到对应的sql语句");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_HISTORY_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
			}
			DBTFLV=GameDBQuery("PAL",logdbip,1,0,0,operRemoteSql,szTableName,
				iSmallType,RoleName,szBegintime,szEndtime,(iIndex-1)*iPageSize,iPageSize);
		}
		else
		{
			if(iBigType==1&&iSmallType==8)
			{
				sprintf(operszSql,"select count(*) as countnum from %s where rolename='%s' and logintime>='%s' and logintime<'%s' ",
						szTableName,
						RoleName,
						szBegintime,
						szEndtime);//构造远程操作的sql语句
			}
			else if(iBigType==1&&iSmallType==2)
			{
				
				sprintf(operszSql,"select count(*) as countnum from %s where rolename='%s' and createtime>='%s' and createtime<'%s' ",
					szTableName,
					RoleName,
					szBegintime,
					szEndtime);//构造远程操作的sql语句

			}
			else if(iBigType==1&&iSmallType==9)
			{
				sprintf(operszSql,"select count(*) as countnum from %s where rolename='%s' and fromname='DelByTimeOut' and time>='%s' and time<'%s' ",
					szTableName,
					RoleName,
					szBegintime,
					szEndtime);//构造远程操作的sql语句
			}
			else
			{

				sprintf(operszSql,"select count(*) as countnum from %s where rolename='%s' and time>='%s' and time<'%s' ",
							szTableName,
							RoleName,
							szBegintime,
							szEndtime);//构造远程操作的sql语句
			}
			
			
			recordnum=operAssist.FindRecordNum(logdbip,operszSql,1);
			if(recordnum==0)
			{
				operAssist.BuildTLV("查询数据库结果为空");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_HISTORY_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
				
			}
			if(recordnum%iPageSize==0)
			{
				PageNum=recordnum/iPageSize;
			}
			else
			{
				PageNum=recordnum/iPageSize+1;
			}
			operAssist.getRemoteSql("PAL",szSqlFlag,operRemoteSql,4);//获取远程操作的Sql语句
			if(!strcmp("",operRemoteSql))
			{
				operAssist.BuildTLV("数据库中找不到对应的sql语句");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_HISTORY_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
			}
			DBTFLV=GameDBQuery("PAL",logdbip,1,0,0,operRemoteSql,szTableName,
				RoleName,szBegintime,szEndtime,(iIndex-1)*iPageSize,iPageSize);
		}
		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV),CEnumCore::ServiceKey::PAL_HISTORY_LOGINFO_QUERY_RESP,PageNum);
		}
		else
		{
			operAssist.BuildTLV("数据库为空");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_HISTORY_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询日志历史信息内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_HISTORY_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询日志历史信息文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_HISTORY_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询日志历史信息其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_HISTORY_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,查询大区<%s>玩家<%s>日志信息发生系统错误",operName,ServerName,RoleName);
		logFile.WriteText("仙剑OL",strLog);//将错误信息写入日志中
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_HISTORY_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	return TRUE;
}

//查询玩家CDKEY道具领取信息
bool CPalLogInfo::CDKey_Get_Query(char * ServerName,char * ServerIP,char * szAccount,char * szBegintime, char * szEndtime,int iIndex, int iPageSize)
{
	try
	{
		sprintf(strLog,"%s, 查询大区<%s>玩家<%s>的CDKey道具领取记录",operName,ServerName,szAccount);
		logFile.WriteText("仙剑OL",strLog);
		
		
		//根据servername查询logdbid
		operAssist.FindAccountDBIP(logdbip,ServerName);
		
		operAssist.getRemoteSql("PAL","PAL_CDKEY_QUERY",operRemoteSql,4);//获取查询CDKEY道具领取记录的Sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.BuildTLV("数据库中找不到对应的sql语句");
			operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CDKEY_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
			return false;
		}

		vector<CGlobalStruct::TFLV>	DBTFLV=GameDBQuery("PAL",logdbip,0,iIndex,iPageSize,operRemoteSql,szAccount,szBegintime,szEndtime);

		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV),CEnumCore::ServiceKey::PAL_CDKEY_QUERY_RESP);
		}
		else
		{
			operAssist.BuildTLV("数据库为空");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CDKEY_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询玩家CDKEY道具领取信息内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CDKEY_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询玩家CDKEY道具领取信息文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CDKEY_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询玩家CDKEY道具领取信息其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CDKEY_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,查询大区<%s>玩家<%s>的CDKey道具领取记录发生系统错误",operName,ServerName,szAccount);
		logFile.WriteText("仙剑OL",strLog);//将错误信息写入日志中
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CDKEY_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	return true;
}

//查询玩家元宝消耗记录
bool CPalLogInfo::Money_Waste_Query(char * ServerName,char * ServerIP,char * szAccount,char * szBegintime, char * szEndtime,int iIndex, int iPageSize)
{
	try
	{
		sprintf(strLog,"%s, 查询大区<%s>玩家<%s>的元宝消耗记录",operName,ServerName,szAccount);
		logFile.WriteText("仙剑OL",strLog);
		
		
		//根据servername查询logdbid
		operAssist.FindAccountDBIP(logdbip,ServerName);
		
		operAssist.getRemoteSql("PAL","PAL_ITEMMAIL_WASTE",operRemoteSql,4);//获取查询玩家元宝消耗记录的Sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.BuildTLV("数据库中找不到对应的sql语句");
			operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ITEMMAIL_WASTE_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
			return false;
		}
		vector<CGlobalStruct::TFLV>	DBTFLV=GameDBQuery("PAL",logdbip,0,iIndex,iPageSize,operRemoteSql,szAccount,szBegintime,szEndtime);

		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV),CEnumCore::ServiceKey::PAL_ITEMMAIL_WASTE_RESP);
		}
		else
		{
			operAssist.BuildTLV("数据库为空");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ITEMMAIL_WASTE_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询玩家元宝消耗记录内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ITEMMAIL_WASTE_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询玩家元宝消耗记录文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ITEMMAIL_WASTE_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询玩家元宝消耗记录其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ITEMMAIL_WASTE_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,查询大区<%s>玩家<%s>的元宝消耗记录发生系统错误",operName,ServerName,szAccount);
		logFile.WriteText("仙剑OL",strLog);//将错误信息写入日志中
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ITEMMAIL_WASTE_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;	
	}
	return true;
}

//获取日志大类
bool CPalLogInfo::GetLogBigType()
{
	try
	{
		sprintf(strLog,"%s, 查询日志大类信息",operName);
		logFile.WriteText("仙剑OL",strLog);
		
		operAssist.getRemoteSql("PAL","PAL_GET_LOGBIGTYPE",operRemoteSql,4);//获取日志大类信息的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.BuildTLV("数据库中找不到对应的sql语句");
			operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOG_BIGTYPE_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
			return false;
		}
		vector<CGlobalStruct::TFLV>	DBTFLV=GameDBQuery("PAL","127.0.0.1",4,0,0,operRemoteSql);
		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV),CEnumCore::ServiceKey::PAL_LOG_BIGTYPE_QUERY_RESP);
		}
		else
		{
			operAssist.BuildTLV("数据库为空");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOG_BIGTYPE_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取日志大类内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOG_BIGTYPE_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取日志大类文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOG_BIGTYPE_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取日志大类其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOG_BIGTYPE_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,查询日志大类信息发生系统错误",operName);
		logFile.WriteText("仙剑OL",strLog);//将错误信息写入日志中
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOG_BIGTYPE_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	return true;
}

//获取日志小类
bool CPalLogInfo::GetLogSmallType(int iBigType)
{
	try
	{
		sprintf(strLog,"%s, 查询日志小类信息",operName);
		logFile.WriteText("仙剑OL",strLog);
		
		operAssist.getRemoteSql("PAL","PAL_GET_LOGSMALLTYPE",operRemoteSql,4);//获取日志大类信息的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.BuildTLV("数据库中找不到对应的sql语句");
			operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOG_SMALLTYPE_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
			return false;
		}
		vector<CGlobalStruct::TFLV>	DBTFLV=GameDBQuery("PAL","127.0.0.1",4,0,0,operRemoteSql,iBigType);

		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV),CEnumCore::ServiceKey::PAL_LOG_SMALLTYPE_QUERY_RESP);
		}
		else
		{
			operAssist.BuildTLV("数据库为空");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOG_SMALLTYPE_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取日志小类内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOG_SMALLTYPE_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取日志小类文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOG_SMALLTYPE_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取日志小类其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOG_SMALLTYPE_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,查询日志大类<%s>的日志小类信息发生系统错误", operName,iBigType);
		logFile.WriteText("PAL",strLog);//将错误信息写入日志中
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOG_SMALLTYPE_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	return TRUE;
}


//查询角色删除日志
bool CPalLogInfo::Pal_RoleDel_Query(char * ServerName,char * ServerIP,char * szAccount,char * RoleName,int iType,char * szBegintime, char * szEndtime,int iIndex, int iPageSize)
{
	try
	{
		sprintf(strLog,"<%s>,查询大区<%s>玩家<%s>角色删除日志",operName,ServerName,szAccount);
		logFile.WriteText("仙剑OL",strLog);
		
		//根据servername查询logdbid
		
		operAssist.FindLogdbIP(logdbip,ServerName);

		vector<CGlobalStruct::TFLV>	DBTFLV;
		
		//判断帐号是否为空
		if(!strcmp("", szAccount))
		{
			operAssist.getRemoteSql("PAL","Pal_RoleDel_byRoleName",operRemoteSql,4);//构造根据IP查询登录记录的sql语句
			if(!strcmp("",operRemoteSql))
			{
				operAssist.BuildTLV("数据库中找不到对应的sql语句");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ROLEDEL_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
			}
			if(iType==0)
			{
				DBTFLV=GameDBQuery("PAL",logdbip,1,iIndex,iPageSize,operRemoteSql,"logdb",RoleName,szBegintime,szEndtime);
			}
			else
			{
				DBTFLV=GameDBQuery("PAL",logdbip,1,iIndex,iPageSize,operRemoteSql,"logdb1",RoleName,szBegintime,szEndtime);
			}
			
		}
		else
		{
			operAssist.getRemoteSql("PAL","Pal_RoleDel_byAccount",operRemoteSql,4);//构造根据帐号查询登录记录的sql语句
			if(!strcmp("",operRemoteSql))
			{
				operAssist.BuildTLV("数据库中找不到对应的sql语句");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ROLEDEL_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
			}
			if(iType==0)
			{
				DBTFLV=GameDBQuery("PAL",logdbip,1,iIndex,iPageSize,operRemoteSql,"logdb",szAccount,szBegintime,szEndtime);
			}
			else
			{
				DBTFLV=GameDBQuery("PAL",logdbip,1,iIndex,iPageSize,operRemoteSql,"logdb1",szAccount,szBegintime,szEndtime);
			}
			
			
		}
		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV),CEnumCore::ServiceKey::PAL_ROLEDEL_QUERY_RESP);
		}
		else
		{
			operAssist.BuildTLV("数据库为空");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ROLEDEL_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询角色删除日志内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ROLEDEL_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询角色删除日志文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ROLEDEL_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询角色删除日志其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ROLEDEL_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,查询大区<%s>玩家<%s>登陆记录日志发生系统错误",operName,ServerName,szAccount);
		logFile.WriteText("仙剑OL",strLog);//将错误信息写入日志中
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ROLEDEL_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	return true;
}


//登陆登出信息
vector <CGlobalStruct::TFLV>  CPalLogInfo::LoginInfo(char * ServerName, char * ServerIP,char * UserName, char * UserNick,char * IP,char * begintime, char * endtime,int iIndex, int iPageSize)
{
	try
	{
		sprintf(strLog,"<%s>,查询大区<%s>玩家<%s>登陆记录日志",operName,ServerName,UserName);
		logFile.WriteText("仙剑OL",strLog);
		
		//根据servername查询logdbid
		int iType=StrToInt(UserNick);//用UserNick代替itype历史还是当前
		operAssist.FindLogdbIP(logdbip,ServerName);

		vector <CGlobalStruct::TFLV> DBTFLV;
		
		
		//判断帐号是否为空
		if(!strcmp("", UserName))
		{
			/*operAssist.getRemoteSql("PAL","PALLOG_ACCOUNT_LOGINBYIP",operRemoteSql,4);//构造根据IP查询登录记录的sql语句
			if(!strcmp("",operRemoteSql))
			{
				operAssist.BuildTLV("SQL为空");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ACCOUNT_LOGIN_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
			}*/
			if(iType==0)
			{
				operAssist.getRemoteSql("PAL","PALLOG_ACCOUNT_LOGINBYIP",operRemoteSql,4);//构造根据IP查询登录记录的sql语句
				if(!strcmp("",operRemoteSql))
				{
					operAssist.BuildTLV("SQL为空");
					operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ACCOUNT_LOGIN_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
					return false;
				}
				DBTFLV=GameDBQuery("PAL",logdbip,1,iIndex,iPageSize,operRemoteSql,"logdb",IP,begintime,endtime);
			}
			else
			{
				//maple update
				operAssist.getRemoteSql("PAL","PALLOG_IP_LOGINHISTORY",operRemoteSql,4);//构造根据IP查询登录记录的sql语句
				if(!strcmp("",operRemoteSql))
				{
					operAssist.BuildTLV("SQL为空");
					operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ACCOUNT_LOGIN_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
					return false;
				}
				DBTFLV=GameDBQuery("PAL",logdbip,1,iIndex,iPageSize,operRemoteSql,IP,begintime,endtime,IP,begintime,endtime,IP,begintime,endtime);

			}
			
		}
		else
		{
			/*operAssist.getRemoteSql("PAL","PALLOG_ACCOUNT_LOGINBYACCOUNT",operRemoteSql,4);//构造根据帐号查询登录记录的sql语句
			if(!strcmp("",operRemoteSql))
			{
				operAssist.BuildTLV("SQL为空");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ACCOUNT_LOGIN_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
			}*/
			
			if(iType==0)
			{
				operAssist.getRemoteSql("PAL","PALLOG_ACCOUNT_LOGINBYACCOUNT",operRemoteSql,4);//构造根据帐号查询登录记录的sql语句
				if(!strcmp("",operRemoteSql))
				{
					operAssist.BuildTLV("SQL为空");
					operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ACCOUNT_LOGIN_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
					return false;
				}
				DBTFLV=GameDBQuery("PAL",logdbip,1,iIndex,iPageSize,operRemoteSql,"logdb",UserName,begintime,endtime);
			}
			else
			{
				//maple update
				operAssist.getRemoteSql("PAL","PALLOG_ACCOUNT_LOGINHISTORY",operRemoteSql,4);//构造根据帐号查询登录记录的sql语句
				if(!strcmp("",operRemoteSql))
				{
					operAssist.BuildTLV("SQL为空");
					operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ACCOUNT_LOGIN_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
					return false;
				}
				//DBTFLV=GameDBQuery("PAL",logdbip,1,iIndex,iPageSize,operRemoteSql,"logdb1",UserName,begintime,endtime);
				DBTFLV=GameDBQuery("PAL",logdbip,1,iIndex,iPageSize,operRemoteSql,UserName,begintime,endtime,UserName,begintime,endtime,UserName,begintime,endtime);
			}
		}


		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV),CEnumCore::ServiceKey::PALLOG_ACCOUNT_LOGIN_RESP);
		}
		else
		{
			operAssist.BuildTLV("数据库为空");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ACCOUNT_LOGIN_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"登陆登出信息内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ACCOUNT_LOGIN_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"登陆登出信息文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ACCOUNT_LOGIN_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"登陆登出信息其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ACCOUNT_LOGIN_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,查询大区<%s>玩家<%s>登陆记录日志发生系统错误",operName,ServerName,UserName);
		logFile.WriteText("仙剑OL",strLog);//将错误信息写入日志中
		operAssist.BuildTLV("数据库为空");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ACCOUNT_LOGIN_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	return true;
}

//add by ltx
//装备道具查询
void CPalLogInfo::Pal_Equipment_Query(char * CustomItemName)
{
	try
	{
		char strlog[60];
		ZeroMemory(strlog,60);
		sprintf(strlog,"装备道具查询");
		logFile.WriteText("仙剑OL",strlog);
		
		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("PAL","127.0.0.1",4,0,0,"exec Pal_Customitem_Query '%s'", CustomItemName);
		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV));

#ifdef _DEBUG
			vector <CGlobalStruct::TFLV>::iterator iter;
			for(iter=DBTFLV.begin(); iter!=DBTFLV.end(); iter++)
			{
				printf("%x %x %s\n", (*iter).m_tagName, (*iter).m_tagFormat, (*iter).lpdata);
			}
#endif
		}
		else
		{
			operAssist.BuildTLV("装备道具不存在");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_EQUIPMENT_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	catch(...)
	{	
		operAssist.BuildTLV("装备道具查询失败");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_EQUIPMENT_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
}
