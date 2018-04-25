#include "StdAfx.h"
#include "GTInfo.h"

CGTInfo::CGTInfo(void)
{
}

CGTInfo::~CGTInfo(void)
{
}

//朋友交易日志
vector <CGlobalStruct::TFLV> CGTInfo::GT_FriendTrade_Log(char * ServerName, char * ServerIP,char * UserName, int UserID,int iIndex,int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	try
	{
		sprintf(strLog,"<%s>,查询大区<%s>玩家<%s>朋友交易日志",ServerName,UserName);
		logFile.WriteText("GTown",strLog);
		
		//获取远程操作的Sql语句
		operGT.getRemoteSql("GT","GT_FriendTrade_Log",operRemoteSql,4);
		if(!strcmp("",operRemoteSql))
		{
			return DBTFLV;
		}
		DBTFLV=GameDBQuery("GT",ServerIP,0,iIndex,iPageSize,operRemoteSql,UserID,UserID);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"朋友交易日志内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"朋友交易日志文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"朋友交易日志其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,查询大区<%s>玩家<%s>详细资料信息发生系统错误",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);//将错误信息写入日志中
		return DBTFLV;
	}
	return DBTFLV;
}

//查询家族信息
vector <CGlobalStruct::TFLV> CGTInfo::GT_FamilyInfo(char * ServerName, char * ServerIP,char * FamilyName,int iIndex,int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	try
	{
		sprintf(strLog,"<%s>,查询大区<%s>家族<%s>基本信息",operName,ServerName,FamilyName);
		logFile.WriteText("GTown",strLog);
		
		//获取远程操作的Sql语句
		operGT.getRemoteSql("GT","GT_FamilyInfo",operRemoteSql,4);
		if(!strcmp("",operRemoteSql))
		{
			return DBTFLV;
		}
		
		DBTFLV=GameDBQuery("GT", ServerIP,0, iIndex, iPageSize,
			operRemoteSql, "gtown_db_gamedb","gtown_db_gamedb", "\%", FamilyName, "\%");
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"家族基本信息内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"家族基本信息文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"家族基本信息其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,查询大区<%s>家族<%s>基本信息发生系统错误",m_UserName,ServerName,FamilyName);
		logFile.WriteText("GTown",strLog);//将错误信息写入日志中
		return DBTFLV;
	}
	return DBTFLV;
}

//查询家族成员信息
vector <CGlobalStruct::TFLV> CGTInfo::GT_MemberInfo(char * ServerName, char * ServerIP,char * FamilyName,int FamilyID,int iIndex,int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	try
	{
		sprintf(strLog,"<%s>,查询大区<%s>家族<%s>详细信息",m_UserName,ServerName,FamilyName);
		logFile.WriteText("GTown",strLog);
		
		//获取远程操作的Sql语句
		operGT.getRemoteSql("GT","GT_Family_MemberInfo",operRemoteSql,4);
		if(!strcmp("",operRemoteSql))
		{
			return DBTFLV;
		}
		DBTFLV=GameDBQuery("GT", ServerIP,0, iIndex, iPageSize,
			operRemoteSql, "gtown_db_gamedb",FamilyID);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"家族详细信息内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"家族详细信息文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"家族详细信息其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,查询大区<%s>家族<%s>详细信息发生系统错误",operName,ServerName,FamilyName);
		logFile.WriteText("GTown",strLog);//将错误信息写入日志中
		return DBTFLV;
	}
	return DBTFLV;
}


//邮件信息查询
vector <CGlobalStruct::TFLV> CGTInfo::GT_MailInfo(char * ServerName, char * ServerIP,char * UserName, int UserID,char* szStartTime,char* szEndTime,int iIndex,int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	try
	{
		sprintf(strLog,"<%s>,查询大区<%s>玩家<%s>邮件信息",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);
		
		operGT.getRemoteSql("GT","GT_MailInfo",operRemoteSql,4);//获取远程操作的Sql语句
		if(!strcmp("",operRemoteSql))
		{
			return DBTFLV;
		}
		DBTFLV=GameDBQuery("GT", ServerIP, 0, iIndex, iPageSize,operRemoteSql,
				"gtown_db_gamedb","gtown_db_gamedb",UserID,szStartTime,szEndTime);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"玩家邮件信息内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"玩家邮件信息文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"玩家邮件信息其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,查询大区<%s>玩家<%s>邮件信息发生系统错误",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);//将错误信息写入日志中
		return DBTFLV;
	}
	return DBTFLV;
}

//查询玩家宝箱开启信息
vector <CGlobalStruct::TFLV> CGTInfo::GT_TreasureBox_Open(char * ServerName, char * ServerIP,char * UserName, char* szStartTime,char* szEndTime,int iIndex,int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	try
	{
		sprintf(strLog,"<%s>,查询大区<%s>玩家<%s>宝箱开启信息",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);
		
		operGT.getRemoteSql("GT","GT_TreasureBox_Open",operRemoteSql,4);//获取远程操作的Sql语句
		if(!strcmp("",operRemoteSql))
		{
			return DBTFLV;
		}
		DBTFLV=GameDBQuery("GT", ServerIP, 0, iIndex, iPageSize,operRemoteSql,
			"gtown_db_gamedb","gtown_db_logdb","gtown_db_gamedb",UserName,szStartTime,szEndTime);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"玩家宝箱开启信息内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"玩家宝箱开启信息文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"玩家宝箱开启信息其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,查询大区<%s>玩家<%s>宝箱开启信息发生系统错误",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);//将错误信息写入日志中
		return DBTFLV;
	}
	return DBTFLV;
}

//查询道具购买消费记录
vector <CGlobalStruct::TFLV> CGTInfo::GT_ItemBuy(char * ServerName, char * ServerIP,char* UserName,
						 int UserID,int type,char * ItemName,char * szStartTime,char * szEndTime,int iIndex,int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	try
	{
	
		sprintf(strLog,"<%s>,查询大区<%s>玩家<%s>道具购买消费记录",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);
		if(!strcmp(ItemName,""))//没有名字
		{
			switch(type)
			{
			case 0:
				operGT.getRemoteSql("GT","GT_ItemBuy_0",operRemoteSql,4);
				break;
			case 1:
				operGT.getRemoteSql("GT","GT_ItemBuy_1",operRemoteSql,4);
				break;
			case 2:
				operGT.getRemoteSql("GT","GT_ItemBuy_2",operRemoteSql,4);
				break;
			case 3:
				operGT.getRemoteSql("GT","GT_ItemBuy_3",operRemoteSql,4);
				break;
			default:
				break;
			}
			if(!strcmp("",operRemoteSql))
			{
				operGT.BuildTLV("数据库中找不到对应的sql语句");
				operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_ADMIN,CEnumCore::ServiceKey::GT_ItemConsume_Query_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
				return false;
			}
			DBTFLV=GameDBQuery("GT",ServerIP,0,iIndex,iPageSize,operRemoteSql, UserID, szStartTime, szEndTime);//构造远程操作的sql语句
		}
		else
		{
			switch(type)
			{
			case 0:
				operGT.getRemoteSql("GT","GT_ItemBuy_4",operRemoteSql,4);
				break;
			case 1:
				operGT.getRemoteSql("GT","GT_ItemBuy_5",operRemoteSql,4);
				break;
			case 2:
				operGT.getRemoteSql("GT","GT_ItemBuy_6",operRemoteSql,4);
				break;
			case 3:
				operGT.getRemoteSql("GT","GT_ItemBuy_7",operRemoteSql,4);
				break;
			default:
				break;
			}
			if(!strcmp("",operRemoteSql))
			{
				return DBTFLV;
			}
			DBTFLV=GameDBQuery("GT",ServerIP,0,iIndex,iPageSize,operRemoteSql,ItemName, UserID, szStartTime, szEndTime);//构造远程操作的sql语句
		}
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"玩家道具购买消费记录内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"玩家道具购买消费记录文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"玩家道具购买消费记录其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,查询大区<%s>玩家<%s>道具购买消费记录发生系统错误",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);//将错误信息写入日志中
		return DBTFLV;
	}
	return DBTFLV;
}



//血拼战绩
vector <CGlobalStruct::TFLV> CGTInfo::GT_Mcoin_Rank(char * ServerName,char * ServerIP,int iType,int iIndex,int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	try
	{
		sprintf(strLog,"<%s>,查询大区<%s>血拼战绩",operName,ServerName);
		switch(iType)
		{
		case 0:
			operGT.getRemoteSql("GT","GT_Mcoin_Rank0",operRemoteSql,4);//获取远程操作的Sql语句
			break;
		case 1:
			operGT.getRemoteSql("GT","GT_Mcoin_Rank1",operRemoteSql,4);//获取远程操作的Sql语句
			break;
		case 2:
			operGT.getRemoteSql("GT","GT_Mcoin_Rank2",operRemoteSql,4);//获取远程操作的Sql语句
			break;
		default:
			break;
		}
		if(!strcmp("",operRemoteSql))
		{
			return DBTFLV;
		}
		
		DBTFLV=GameDBQuery("GT",ServerIP,0,iIndex,iPageSize,operRemoteSql,"gtown_db_gamedb","gtown_db_gamedb");
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查看大区血拼战绩内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查看大区血拼战绩文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查看大区血拼战绩其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,查询大区<%s>血拼战绩发生系统错误",operName,ServerName);
		logFile.WriteText("GTown",strLog);//将错误信息写入日志中
		return DBTFLV;
	}
	return DBTFLV;
}

//空间排行
vector <CGlobalStruct::TFLV> CGTInfo::GT_Sns_Query(char * ServerName,char * ServerIP,int iType,int iIndex,int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	try
	{
		sprintf(strLog,"<%s>,查询大区<%s>空间排行",operName,ServerName);
		logFile.WriteText("GTown",strLog);
		switch(iType)
		{
		case 0:
			operGT.getRemoteSql("GT","GT_Sns_Query0",operRemoteSql,4);//获取远程操作的Sql语句
			break;
		case 1:
			operGT.getRemoteSql("GT","GT_Sns_Query1",operRemoteSql,4);//获取远程操作的Sql语句
			break;
		case 2:
			operGT.getRemoteSql("GT","GT_Sns_Query2",operRemoteSql,4);//获取远程操作的Sql语句
			break;
		default:
			break;
		}
		if(!strcmp("",operRemoteSql))
		{
			return DBTFLV;
		}
		DBTFLV=GameDBQuery("GT",ServerIP,0,iIndex,iPageSize,operRemoteSql);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查看大区空间排行内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查看大区空间排行文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查看大区空间排行其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,查询大区<%s>空间排行发生系统错误",operName,ServerName);
		logFile.WriteText("GTown",strLog);//将错误信息写入日志中
		return DBTFLV;
	}
	return DBTFLV;
}

//属性排行
vector <CGlobalStruct::TFLV> CGTInfo::GT_Attr_Queue(char * ServerName,char * ServerIP,int iType,int iIndex,int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	try
	{
		sprintf(strLog,"<%s>,查询大区<%s>属性排行",operName,ServerName);
		logFile.WriteText("GTown",strLog);
		
		switch(iType)
		{
		case 0:
			operGT.getRemoteSql("GT","GT_Attr_Queue0",operRemoteSql,4);//获取远程操作的Sql语句
			break;
		case 1:
			operGT.getRemoteSql("GT","GT_Attr_Queue1",operRemoteSql,4);//获取远程操作的Sql语句
			break;
		case 2:
			operGT.getRemoteSql("GT","GT_Attr_Queue2",operRemoteSql,4);//获取远程操作的Sql语句
			break;
		case 3:
			operGT.getRemoteSql("GT","GT_Attr_Queue3",operRemoteSql,4);//获取远程操作的Sql语句
			break;
		case 4:
			operGT.getRemoteSql("GT","GT_Attr_Queue4",operRemoteSql,4);//获取远程操作的Sql语句
			break;
		case 5:
			operGT.getRemoteSql("GT","GT_Attr_Queue5",operRemoteSql,4);//获取远程操作的Sql语句
			break;
		default:
			break;
		}
		if(!strcmp("",operRemoteSql))
		{
			return DBTFLV;
		}
		DBTFLV=GameDBQuery("GT",ServerIP,0,iIndex,iPageSize,operRemoteSql,"gtown_db_gamedb","gtown_db_gamedb");
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查看大区属性排行内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查看大区属性排行文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查看大区属性排行其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,查询大区<%s>属性排行发生系统错误",operName,ServerName);
		logFile.WriteText("GTown",strLog);//将错误信息写入日志中
		return DBTFLV;
	}
	return DBTFLV;
}

//名片排行
vector <CGlobalStruct::TFLV> CGTInfo::GT_Card_Record(char * ServerName,char * ServerIP,int iType,int iIndex,int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	try
	{
		sprintf(strLog,"<%s>,查询大区<%s>名片排行",operName,ServerName);
		logFile.WriteText("GTown",strLog);
		switch(iType)
		{
		case 0:
			operGT.getRemoteSql("GT","GT_Card_Record0",operRemoteSql,4);//获取远程操作的Sql语句
			if(!strcmp("",operRemoteSql))
			{
				return DBTFLV;
			}
			DBTFLV=GameDBQuery("GT",ServerIP,0,iIndex,iPageSize,operRemoteSql,"gtown_db_carddb","gtown_db_gamedb");
			break;
		case 1:
			operGT.getRemoteSql("GT","GT_Card_Record1",operRemoteSql,4);//获取远程操作的Sql语句
			if(!strcmp("",operRemoteSql))
			{
				return DBTFLV;
			}
			DBTFLV=GameDBQuery("GT",ServerIP,0,iIndex,iPageSize,operRemoteSql,"gtown_db_carddb","gtown_db_gamedb");
			break;
		case 2:
			operGT.getRemoteSql("GT","GT_Card_Record2",operRemoteSql,4);//获取远程操作的Sql语句
			if(!strcmp("",operRemoteSql))
			{
				return DBTFLV;
			}
			DBTFLV=GameDBQuery("GT",ServerIP,0,iIndex,iPageSize,operRemoteSql,"gtown_db_gamedb","gtown_db_gamedb","gtown_db_gamedb");
			break;
		case 3:
			operGT.getRemoteSql("GT","GT_Card_Record3",operRemoteSql,4);//获取远程操作的Sql语句
			if(!strcmp("",operRemoteSql))
			{
				return DBTFLV;
			}
			DBTFLV=GameDBQuery("GT",ServerIP,0,iIndex,iPageSize,operRemoteSql,"gtown_db_gamedb","gtown_db_gamedb");
			break;
		case 4:
			operGT.getRemoteSql("GT","GT_Card_Record4",operRemoteSql,4);//获取远程操作的Sql语句
			if(!strcmp("",operRemoteSql))
			{
				return DBTFLV;
			}
			DBTFLV=GameDBQuery("GT",ServerIP,0,iIndex,iPageSize,operRemoteSql,"gtown_db_logdb","gtown_db_gamedb");
			break;
		default:
			break;
		}
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查看大区名片排行内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查看大区名片排行文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查看大区名片排行其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,查询大区<%s>名片排行发生系统错误",operName,ServerName);
		logFile.WriteText("GTown",strLog);//将错误信息写入日志中
		return DBTFLV;
	}
	return DBTFLV;
}






//排行
vector <CGlobalStruct::TFLV> CGTInfo::GT_FriendTrade_Queue(char * ServerName,char * ServerIP,int iType,int iIndex,int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	try
	{
		sprintf(strLog,"<%s>,查询大区<%s>交易排行",operName,ServerName);
		logFile.WriteText("GTown",strLog);
	
		switch(iType)
		{
		case 0://身价最贵的
			operGT.getRemoteSql("GT","GT_FriendTrade_Queue0",operRemoteSql,4);//获取远程操作的Sql语句
			break;
		case 1://最新被买
			operGT.getRemoteSql("GT","GT_FriendTrade_Queue1",operRemoteSql,4);//获取远程操作的Sql语句
			break;
		case 2://资产最多
			operGT.getRemoteSql("GT","GT_FriendTrade_Queue2",operRemoteSql,4);//获取远程操作的Sql语句
			break;
		case 3://现金最多
			operGT.getRemoteSql("GT","GT_FriendTrade_Queue3",operRemoteSql,4);//获取远程操作的Sql语句
			break;
		case 4://折扣最多
			operGT.getRemoteSql("GT","GT_FriendTrade_Queue4",operRemoteSql,4);//获取远程操作的Sql语句
			break;
		default:
			break;
		}
		if(!strcmp("",operRemoteSql))
		{
			return DBTFLV;
		}

		DBTFLV=GameDBQuery("GT",ServerIP,0,iIndex,iPageSize,operRemoteSql);
		
		
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查看大区交易排行内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查看大区交易排行文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查看大区交易排行其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,查询大区<%s>交易排行",operName,ServerName);
		logFile.WriteText("GTown",strLog);//将错误信息写入日志中
		return DBTFLV;		
	}
	return DBTFLV;
}


//道具查询
vector <CGlobalStruct::TFLV> CGTInfo::GT_Select_Item(char * ServerName,char * ServerIP,char * ItemName)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	try
	{
		sprintf(strLog,"<%s>,查询大区<%s>道具查询",operName,ServerName);
		logFile.WriteText("GTown",strLog);
		
		operGT.getRemoteSql("GT","GT_Select_Item",operRemoteSql,4);//获取远程操作的Sql语句


		if(!strcmp("",operRemoteSql))
		{
			return DBTFLV;
		}
		DBTFLV=GameDBQuery(
			"GT",ServerIP,0,0,0,operRemoteSql,"gtown_db_gamedb","gtown_db_gamedb", "\%",ItemName,"\%");//构造远程操作的sql语句

	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查看道具查询内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查看道具查询文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查看道具查询其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,查询大区<%s>道具查询",operName,ServerName);
		logFile.WriteText("GTown",strLog);//将错误信息写入日志中
		return DBTFLV;
	}
	return DBTFLV;
}
//重复购买道具退款
vector <CGlobalStruct::TFLV> CGTInfo::GT_Back_Money(int userByID,char * ServerName, char * ServerIP, char * UserName, int iID,int iType,int iMoney,char * strItemName)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	CGlobalStruct::TFLV m_tflv;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		sprintf(strLog,"<%s>,大区<%s>玩家重复购买道具退款",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);
		

		switch(iType)
		{
		case 0:
		case 2:
		case 4:
		case 6:
			operGT.getRemoteSql("GT","GT_IS_BackMoney0",operRemoteSql,4);//获取远程操作的Sql语句
			break;
		default:
			operGT.getRemoteSql("GT","GT_IS_BackMoney1",operRemoteSql,4);//获取远程操作的Sql语句	
			break;
		}
	
		if(!strcmp("",operRemoteSql))
		{
			return DBTFLV;
		}
		char strResult[256];
		sprintf(operszSql,operRemoteSql,UserName,strItemName);
		operGT.QuerySingleValue("GT",ServerIP,0,operszSql,strResult);
		if(StrToInt(strResult)<=1)
		{
			sprintf((char *)&m_tflv.lpdata,"玩家%s,未重复购买道具%s",UserName,strItemName);
			m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
			DBTFLV.push_back(m_tflv);
			return DBTFLV;
		}

		switch(iType)
		{
		case 0:
		case 2:
		case 4:
		case 6:
			operGT.getRemoteSql("GT","GT_Back_Money0",operRemoteSql,4);//获取远程操作的Sql语句
			break;
		default:
			operGT.getRemoteSql("GT","GT_Back_Money1",operRemoteSql,4);//获取远程操作的Sql语句	
			break;
		}
		
		if(!strcmp("",operRemoteSql))
		{
			return DBTFLV;
		}

		int tmpLength=GameDBOper("GT",ServerIP,0,operRemoteSql,iID,UserName,UserName,iMoney);
		
		if(tmpLength>0)
		{
			sprintf((char *)&m_tflv.lpdata,"玩家%s,退款数%i成功",UserName,iMoney);
		}
		else
		{
			sprintf((char *)&m_tflv.lpdata,"玩家%s,退款数%i成功",UserName,iMoney);
		}
		logFile.WriteDBLog(userByID,"吉堂Online","GT_Back_Money",ServerIP,(char *)&m_tflv.lpdata);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"玩家退款内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"玩家退款文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"玩家退款其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;

}



