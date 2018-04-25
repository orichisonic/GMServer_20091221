#include "StdAfx.h"
#include "SDOInfo.h"

#pragma warning(disable:4129) 

CSDOInfo::CSDOInfo(void)
{
}

CSDOInfo::~CSDOInfo(void)
{
}

//初始化类中的变量
COperVector * CSDOInfo::initialize(CSession * pSess,CEnumCore::Message_Tag_ID MessageTagID, unsigned char * pbuf, int length)
{
	COperVector * operVector = NULL;
	try
	{
		ZeroMemory(localIP, 20);
		ZeroMemory(SDO_DBName, 64);
		ZeroMemory(Log_DBName, 64);
		ZeroMemory(Item_DBName, 64);
		logFile.ReadValue("scheme", "DATABASE","Address",localIP, 20);//获取本地的数据库地址
		logFile.ReadValue("SDO", "DATABASE","DBName",SDO_DBName, 64);//获取游戏数据库名称
		logFile.ReadValue("SDO", "DATABASE","LogDBName",Log_DBName, 64);//获取日志数据库名称
		logFile.ReadValue("SDO", "DATABASE","ItemDBName",Item_DBName, 64);//获取道具数据库名称
		
		//调用基类初始化方法
		operVector = CGameInfo::initialize(pSess, MessageTagID, pbuf, length);

	}
	catch(...)
	{
	}
	return operVector;
}

//查询玩家基本资料信息
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_UserInfo(int userByID, char * ServerName, char * ServerIP,char * Account,char * NickName,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{		
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_UserInfo", "", operName, ServerName, Account);
		operSDO.WriteText("SDO",strLog);
		
		//获取远程操作的Sql语句
		if(strcmp("",Account))
		{
			if(!operSDO.getRemoteSql("SDO", "SDO_CharInfo_Account",operRemoteSql,1))
			{
				return DBTFLV;//无法得到查询玩家角色信息的sql语句
			}
			DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql,SDO_DBName,SDO_DBName,SDO_DBName,SDO_DBName,Account);

		}
		else
		{
			if(!operSDO.getRemoteSql("SDO", "SDO_CharInfo_Nick",operRemoteSql,1))
			{
				return DBTFLV;//无法得到查询玩家角色信息的sql语句
			}
//			printf(operRemoteSql,SDO_DBName,SDO_DBName,SDO_DBName,SDO_DBName,"\%",NickName,"\%");
			DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql,SDO_DBName,SDO_DBName,SDO_DBName,SDO_DBName,"\%",NickName,"\%");
		}
		
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//查询玩家帐号信息
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_AccountInfo(int userByID, char * ServerName, char * ServerIP,char * Account,char * NickName,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_AccountInfo", "", operName,ServerName,Account);
		operSDO.WriteText("SDO",strLog);
		
		//获取远程操作的Sql语句
		if(strcmp("",Account))
		{
			if(!operSDO.getRemoteSql("SDO","SDO_MemberInfo_Account",operRemoteSql, 1))
			{
				return DBTFLV;//无法得到查询玩家帐号信息的sql语句
			}
//			printf(operRemoteSql,SDO_DBName,SDO_DBName,Account);
			DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql,SDO_DBName,SDO_DBName,Account);

		}
		else
		{
			if(!operSDO.getRemoteSql("SDO","SDO_MemberInfo_Nick",operRemoteSql, 1))
			{
				return DBTFLV;//无法得到查询玩家帐号信息的sql语句
			}
//			sprintf(operszSql,operRemoteSql,SDO_DBName,SDO_DBName,"\%",NickName,"\%");
			DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql,SDO_DBName,SDO_DBName,"\%",NickName,"\%");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//查询玩家物品
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_ItemInfo(int userByID, char * ServerName, char * ServerIP,char * Account, int UserID,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_ItemInfo", "", operName,ServerName,Account);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))
		{
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		if(!operSDO.getRemoteSql("SDO","SDO_ItemInfo_Query",operRemoteSql,1))
		{
			return DBTFLV;//无法得到查询玩家物品信息的sql语句
		}
		DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql,Item_DBName,Item_DBName,UserID);
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//查询玩家宠物信息
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_PetInfo(int userByID,char * ServerName, char * ServerIP,char * Account, int UserID,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_PetInfo", "", operName,ServerName,Account);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))
		{
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_PetInfo_Query",operRemoteSql,1))
		{
			return DBTFLV;//无法得到查询玩家宠物信息的sql语句
		}
//		sprintf(operszSql,operRemoteSql,Item_DBName,Item_DBName,Item_DBName,UserID);//构造完整查询的sql语句
		DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql,Item_DBName,Item_DBName,Item_DBName,UserID);
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//查看玩家的宠物删除信息
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_PetDrop(int userByID, char * ServerName, char * ServerIP,char * Account, int UserID,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_PetDrop", "", operName,ServerName,Account);
		operSDO.WriteText("SDO",strLog);
		char tempServerIp[20];

		if(!operSDO.GetServerIP(tempServerIp,ServerIP,3))
		{
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_PetDrop_Query",operRemoteSql, 1))
		{
			return DBTFLV;//无法得到查询玩家宠物删除信息的sql语句
		}
//		sprintf(operszSql,operRemoteSql,Log_DBName,SDO_DBName,UserID);//构造完整查询的sql语句
		DBTFLV=GameDBQuery("SDO",tempServerIp,0,iIndex,iPageSize,operRemoteSql,Log_DBName,SDO_DBName,UserID);		
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//查询玩家账号登录记录
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_LoginLog(int userByID, char * ServerName, char * ServerIP,char * Account,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_LoginLog", "", operName,ServerName,Account);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_UserLogin_Query",operRemoteSql,1))
		{
			return DBTFLV;//无法得到查询玩家账号登录记录的sql语句
		}
//		sprintf(operszSql,operRemoteSql,Log_DBName,Account);//构造完整查询的sql语句
		DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql,Log_DBName,Account);

	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//查询玩家物品赠送记录
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_UserTrade_Log(int userByID, char * ServerName, char * ServerIP,char * Account,char * NickName,int itype, int iIndex,int iPageSize)
{	
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_UserTrade_Log", "", operName,ServerName,NickName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,3))
		{
			return DBTFLV;//无法获取真正的服务器IP
		}
		if(itype == 1)//玩家作为发送人
		{
			if(!operSDO.getRemoteSql("SDO", "SDO_TradeLog_Sender",operRemoteSql,1))
			{
				return DBTFLV;//无法得到查询玩家物品赠送记录的sql语句
			}
//			printf(operRemoteSql, Log_DBName, Account, Log_DBName, Account);
			DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql, Log_DBName, Account, Log_DBName, Account);
		}
		else if(itype == 2)//玩家作为接收人
		{
			if(!operSDO.getRemoteSql("SDO","SDO_TradeLog_Recver",operRemoteSql,1))
			{
				return DBTFLV;//无法得到查询玩家物品赠送记录的sql语句
			}
//			printf(operRemoteSql, Log_DBName, "\%", NickName, "\%", Log_DBName, "\%", NickName, "\%");
			DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql, Log_DBName, "\%", NickName, "\%", Log_DBName, "\%", NickName, "\%");
		}
		
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//查询玩家消费记录
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_UserConsume(int userByID, char * ServerName, char * ServerIP,char * Account,char * NickName,int iMoneytype, char *ItemName, char *BeginTime, char *EndTime, int iIndex,int iPageSize)
{	
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_UserConsume", "", operName,ServerName,NickName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_UserConsume",operRemoteSql,1))
		{
			return DBTFLV;//无法得到查询玩家消费记录的sql语句
		}
		DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql,Log_DBName,Account,iMoneytype,BeginTime,EndTime,"\%",ItemName,"\%",
							Log_DBName,Account,iMoneytype,BeginTime,EndTime,"\%",ItemName,"\%");
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//查看玩家当前状态(服务器/房间/在线状态)
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_UserStatus(int userByID, char * ServerName, char * ServerIP,char * Account,int UserID,int iIndex,int iPageSize)
{	
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_UserStatus", "", operName,ServerName,Account);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO","SDO_UserStatus",operRemoteSql, 1))
		{
			return DBTFLV;//无法得到查看玩家当前状态的sql语句
		}
//		printf(operRemoteSql,SDO_DBName,SDO_DBName,UserID);
		DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql,SDO_DBName,SDO_DBName,UserID);
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//查看玩家婚姻状态
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_UserMarriage(int userByID, char * ServerName, char * ServerIP,char * Account,char * NickName,int UserID,int iIndex,int iPageSize)
{	
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_UserMarriage", "", operName,ServerName,NickName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO","SDO_UserMarriage",operRemoteSql,1))
		{
			return DBTFLV;//无法得到查看玩家婚姻状态的sql语句
		}
		DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql,UserID,UserID,UserID,UserID,SDO_DBName,SDO_DBName,SDO_DBName,UserID,UserID);
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//查看玩家摇摇乐获得物品
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_YYItem_QUERY(int userByID, char * ServerName, char * ServerIP,char * Account,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;	
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_YYItem_QUERY", "", operName,ServerName,Account);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,3))
		{
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_RewardItem_Query",operRemoteSql,1))
		{
			return DBTFLV;//无法得到查看玩家摇摇乐获得物品的sql语句
		}
		DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql,Log_DBName,Account);		
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//强制玩家下线
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_KickPlayer_Off(int userByID, char * ServerName, char * ServerIP,char * Account, int UserID)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_KickPlayer", "", "", operName,ServerName,Account);
		
		int IsOnline = 0;
		if(!operSDO.getRemoteSql("SDO","SDO_UserOnline_Status",operRemoteSql,1))
		{
			return DBTFLV;//无法得到获取玩家在线状态的sql语句
		}
		operSDO.QuerySingleValue(&IsOnline, "SDO", ServerIP, 0, operRemoteSql, SDO_DBName, UserID);//获取玩家是否在线的状态
		
		if(IsOnline != 1)//玩家不在线
		{
			DBTFLV = operSDO.ReturnOpMsg(strLog,"SDO_UserNotOnline");//将错误信息写入日志中并返回错误信息
		}
		else
		{
			if(!operSDO.getRemoteSql("SDO", "SDO_KickPlayer_Off",operRemoteSql,1))
			{
				return DBTFLV;//无法得到强制玩家下线的sql语句
			}
			
			int tmpLength = 0;
			tmpLength = GameDBOper("SDO",ServerIP,0,operRemoteSql,SDO_DBName,UserID);
			if(tmpLength == 0)
			{
				DBTFLV = operSDO.ReturnOpMsg(strLog,"KickPlayer_Fail");//将错误信息写入日志中并返回错误信息

			}
			else
			{
				DBTFLV = operSDO.ReturnOpMsg(strLog,"KickPlayer_Success");//将错误信息写入日志中并返回错误信息				
			}
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//查询玩家封停状态
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_UserBanStatus(int userByID, char * ServerName, char * ServerIP,char * Account,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_UserBanStatus", "", operName,ServerName,Account);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO","SDO_Banishment_Query",operRemoteSql,1))
		{
			return DBTFLV;//无法得到查询玩家封停状态的sql语句
		}
//		sprintf(operszSql,operRemoteSql,SDO_DBName,Account);
		DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql,SDO_DBName,Account);
		if (DBTFLV.empty())
		{
			DBTFLV = operSDO.ReturnOpMsg(strLog,"ACOUNT_CLOSEED");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//封停玩家帐号
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_Account_Close(int userByID, char * ServerName, char * ServerIP,char * Account,char *BanReason)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
	
		operSDO.GetLogText(strLog, "SDO_Account_Close", "", "", operName,ServerName,Account,BanReason);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO","SDO_Account_Close",operRemoteSql,1))
		{
			return DBTFLV;//无法得到封停玩家帐号的sql语句
		}
		int tmpLength = 0;
		tmpLength = GameDBOper("SDO",ServerIP, 0,operRemoteSql,SDO_DBName,SDO_DBName,Account);
		if(tmpLength == 0)
		{
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Account_Close_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_Account_Close", "", "Success", operName,ServerName,Account,BanReason);
			operSDO.WriteDBLog(userByID,"SDO","SDO_Account_Close",ServerIP,strLog);
			
			if(!operSDO.getRemoteSql("SDO","SDO_BanAccount_Local",operRemoteSql,1))
			{
				return DBTFLV;//无法得到封停玩家帐号的sql语句
			}
			tmpLength = 0;
//			printf(operRemoteSql, userByID, Account, BanReason, ServerIP);
			tmpLength = GameDBOper("SDO",localIP, 4, operRemoteSql, userByID, Account, BanReason, ServerIP);
			
			if(tmpLength == 0)
			{
				DBTFLV = operSDO.ReturnOpMsg(strLog,"MiddleState");
			}
			else
			{
				DBTFLV = operSDO.ReturnOpMsg(strLog,"Account_Close_Success");
			}
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//查询所有被封停帐号列表
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_QueryBan_All(int userByID, char * ServerName, char * ServerIP,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage1", "SDO_QueryBanAll", "", operName,ServerName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO","SDO_QueryBan_All",operRemoteSql,1))
		{
			return false;//无法得到查询所有被封停帐号列表的sql语句
		}
		DBTFLV=GameDBQuery("SDO",localIP,4,iIndex,iPageSize,operRemoteSql,ServerIP);
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//解封玩家帐号
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_Account_Open(int userByID, char * ServerName, char * ServerIP,char * Account,char *UnbindReason)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_Account_Open", "", "", operName,ServerName,Account,UnbindReason);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO","SDO_Account_Open",operRemoteSql,1))
		{
			return DBTFLV;//无法得到解封玩家帐号的sql语句
		}
		int tmpLength = 0;
		tmpLength = GameDBOper("SDO",ServerIP, 0,operRemoteSql,SDO_DBName,Account);
		
		if(tmpLength == 0)
		{
			operSDO.WriteDBLog(userByID,"SDO","SDO_Account_Open",ServerIP,strLog);
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Account_Open_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_Account_Open", "", "Success", operName,ServerName,Account,UnbindReason);
			operSDO.WriteDBLog(userByID,"SDO","SDO_Account_Open",ServerIP,strLog);
			
			if(!operSDO.getRemoteSql("SDO","SDO_UnbindAccount_Local",operRemoteSql,1))
			{
				return DBTFLV;//无法得到本地解封玩家帐号的sql语句
			}

			tmpLength = 0;
			tmpLength = GameDBOper("SDO",localIP, 0,operRemoteSql, Account, ServerIP);
			
			if(tmpLength == 0)
			{
				DBTFLV = operSDO.ReturnOpMsg(strLog,"MiddleState");
			}
			else
			{
				DBTFLV = operSDO.ReturnOpMsg(strLog,"Account_Open_Success");
			}
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//查询公告信息列表
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_NoticeInfo(int userByID, char * ServerName, char * ServerIP,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage2", "SDO_NoticeInfo", "", operName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_NoticeInfo",operRemoteSql,1))
		{
			return DBTFLV;//无法得到查询所有被封停帐号列表的sql语句
		}
//		sprintf(operszSql,operRemoteSql);//构造完整查询的sql语句
		DBTFLV = BroadTask_Query("SDO",operRemoteSql,iIndex,iPageSize);	
//		DBTFLV=GameDBQuery("SDO",localIP,4,iIndex,iPageSize,operRemoteSql);		
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//添加公告信息
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_AddNotice(int userByID, char * ServerName, char * ServerIP, char *BoardMessage, char *BeginTime, char *EndTime, int interval, int iNoticeType)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{

		int iAtonce ;
		char strNotice[10];
		ZeroMemory(strNotice, 10);
/*
		if (iNoticeType == 1)	//及时公告
		{
			logFile.ReadValue("SDO", "NOTICETYPE", "NOtice_inTime", strNotice, 10);
			iAtonce = -1;
		}
		else					//定时公告
		{
			logFile.ReadValue("SDO", "NOTICETYPE", "NOtice_Time", strNotice, 10);
			iAtonce = 0;
		}
*/
		//获取公告类型名称<strNotice>以及类型标识<iAtonce>
		GetNoticeType(strNotice, iAtonce, iNoticeType);

		//写发送公告的日志
		operSDO.GetLogText(strLog, "SDO_AddNotice", "", "", operName, ServerName, BoardMessage,BeginTime, EndTime, interval, strNotice);
		operSDO.WriteText("SDO",strLog);
		
		sprintf(operszSql,"exec SDO_TaskList_Insert %i,'%s','%s','%s','%s',%i", 
			userByID,ServerIP,BoardMessage,BeginTime, EndTime, interval);

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO",localIP,4,"exec SDO_TaskList_Insert %i,'%s','%s','%s','%s',%i,%i", 
								userByID,ServerIP,BoardMessage,BeginTime, EndTime, interval, iAtonce);
		
		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_AddNotice", "", "Fail", operName, ServerName, BoardMessage,BeginTime, EndTime, interval, strNotice);
			operSDO.WriteDBLog(userByID,"SDO","SDO_AddNotice",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Add_Notice_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_AddNotice", "", "Success", operName, ServerName, BoardMessage,BeginTime, EndTime, interval, strNotice);
			operSDO.WriteDBLog(userByID,"SDO","SDO_AddNotice",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Add_Notice_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//查询发送公告的大区列表信息
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_BoardCity_Query(int userByID, int taskID,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{	
		operSDO.GetLogText(strLog, "SDO_BoardCity_Query", "", "", operName, taskID);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_BoardCity_Query",operRemoteSql,1))
		{
			return DBTFLV;//无法得到查询发送公告的大区列表信息的sql语句
		}
		DBTFLV = GameDBQuery("SDO",localIP,4, iIndex, iPageSize, operRemoteSql,taskID);
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//修改公告信息
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_EditNotice(int userByID, char * ServerName, char * ServerIP, int taskID, char *BoardMessage, char *BeginTime, char *EndTime, int interval,int status,int iNoticeType)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	
	try
	{
		int iAtonce ;
		char strNotice[10];
		ZeroMemory(strNotice, 10);
/*
		if (iNoticeType == 1)	//及时公告
		{
			logFile.ReadValue("SDO", "NOTICETYPE", "NOtice_inTime", strNotice, 10);
			iAtonce = -1;
		}
		else					//定时公告
		{
			logFile.ReadValue("SDO", "NOTICETYPE", "NOtice_Time", strNotice, 10);
			iAtonce = 0;
		}
*/		
		//获取公告类型名称<strNotice>以及类型标识<iAtonce>
		GetNoticeType(strNotice, iAtonce, iNoticeType);
		operSDO.GetLogText(strLog, "SDO_EditNotice", "", "", operName, 
			taskID,ServerName,BoardMessage,BeginTime,EndTime,interval,status, strNotice);
		operSDO.WriteText("SDO",strLog);

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", localIP, 4, "exec SDO_TaskList_Update %i,'%s',%i,'%s','%s','%s',%i,%i,%i", 
			userByID,ServerIP,taskID,BoardMessage,BeginTime, EndTime, interval, status,iAtonce);
		
		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_EditNotice", "", "Fail",  operName, taskID,ServerName,BoardMessage,BeginTime,EndTime,interval,status, strNotice);
			operSDO.WriteDBLog(userByID,"SDO","SDO_EditNotice",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Edit_Notice_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_EditNotice", "", "Success",  operName, taskID,ServerName,BoardMessage,BeginTime,EndTime,interval,status, strNotice);
			operSDO.WriteDBLog(userByID,"SDO","SDO_EditNotice",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Edit_Notice_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//查询家族基本信息
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_FamilyInfo(int userByID, char * ServerName, char * ServerIP, char *FamilyName, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "SDO_FamilyInfo", "SDO_UserInfo", "", operName, ServerName, FamilyName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO","SDO_FamilyInfo_Query",operRemoteSql,1))
		{
			return DBTFLV;//无法得到查询家族基本信息的sql语句
		}
//		printf(operRemoteSql,SDO_DBName,SDO_DBName,SDO_DBName, "\%", FamilyName, "\%");
		DBTFLV = GameDBQuery("SDO",ServerIP,0, iIndex, iPageSize, operRemoteSql,SDO_DBName,SDO_DBName,SDO_DBName, "\%", FamilyName, "\%");		
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//查询家族成员信息
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_FamilyMember(int userByID, char * ServerName, char * ServerIP, int FamilyID, char *FamilyName, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "SDO_FamilyInfo", "SDO_MemberInfo", "", operName, ServerName, FamilyName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_FamilyMember_Query",operRemoteSql,1))
		{
			return DBTFLV;//无法得到查询家族成员信息的sql语句
		}
//		printf(operRemoteSql,SDO_DBName,SDO_DBName,SDO_DBName,FamilyID);
		DBTFLV = GameDBQuery("SDO",ServerIP,0, iIndex, iPageSize, operRemoteSql,SDO_DBName,SDO_DBName,SDO_DBName,FamilyID);		

	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}

	return DBTFLV;
}

//查询申请中家族成员信息
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_FamilyFormal(int userByID, char * ServerName, char * ServerIP, int FamilyID, char *FamilyName, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "SDO_FamilyInfo", "SDO_ApplicationMember", "", operName, ServerName, FamilyName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_Family_ApplicMember",operRemoteSql,1))
		{
			return DBTFLV;//无法得到查询申请中家族成员信息的sql语句
		}
//		printf(operRemoteSql,SDO_DBName,SDO_DBName,FamilyID);
		DBTFLV = GameDBQuery("SDO",ServerIP,0, iIndex, iPageSize, operRemoteSql,SDO_DBName,SDO_DBName,FamilyID);
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//查询家族勋章信息
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_FamilyBadge(int userByID, char * ServerName, char * ServerIP, int FamilyID, char *FamilyName, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "SDO_FamilyInfo", "SDO_BadgeInfo", "", operName, ServerName, FamilyName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO","SDO_FamilyBadge_Query",operRemoteSql,1))
		{
			return DBTFLV;//无法得到查询家族勋章信息的sql语句
		}
//		printf(operRemoteSql,SDO_DBName,SDO_DBName,SDO_DBName,FamilyID);
		DBTFLV = GameDBQuery("SDO",ServerIP,0, iIndex, iPageSize, operRemoteSql,SDO_DBName,SDO_DBName,SDO_DBName,FamilyID);
	}
	catch(...)
	{
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//查询家族金库捐赠明细
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_FamilyDonate(int userByID, char * ServerName, char * ServerIP, int FamilyID, char *FamilyName, char *BeginTime, char *EndTime,int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "SDO_FamilyInfo", "SDO_DonateInfo", "", operName, ServerName, FamilyName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO","SDO_FamilyDonate_Query",operRemoteSql,1))
		{
			return DBTFLV;//无法得到查询家族金库捐赠明细的sql语句
		}
//		printf(operRemoteSql,SDO_DBName,SDO_DBName,FamilyID,BeginTime,EndTime);
		DBTFLV = GameDBQuery("SDO",ServerIP,0, iIndex, iPageSize, operRemoteSql,SDO_DBName,SDO_DBName,FamilyID,BeginTime,EndTime);
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//查询家族购买记录
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_FamilyConsume(int userByID, char * ServerName, char * ServerIP, int FamilyID, char *FamilyName, char *BeginTime, char *EndTime,int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "SDO_FamilyInfo", "SDO_FamilyConsume", "", operName, ServerName, FamilyName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO","SDO_FamilyConsume_Query",operRemoteSql,1))
		{
			return DBTFLV;//无法得到查询家族金库捐赠明细的sql语句
		}
		DBTFLV = GameDBQuery("SDO",ServerIP,0, iIndex, iPageSize, operRemoteSql,SDO_DBName,SDO_DBName,SDO_DBName,FamilyID,BeginTime,EndTime);		

	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//修改家族成员职务
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_ModiMemDuty(int userByID, char * ServerName, char * ServerIP, char *NickName, int UserID, int FamilyID, char *FamilyName, int type)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	
	try
	{
		int type = 0;
		type = StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::SDO_Type,0)).lpdata);//成员的职务类型
		char MemDuty[10];
		ZeroMemory(MemDuty, 10);
		if(type == 255)
		{
			logFile.ReadValue("SDO","Code","Shaikh",MemDuty,10);//族长
		}
		if(type == 2)
		{
			logFile.ReadValue("SDO","Code","AssistShaikh",MemDuty,10);//副族长
		}
		if(type == 0)
		{
			logFile.ReadValue("SDO","Code","CommonMem",MemDuty,10);//普通成员
		}
		if(type == 1)
		{
			logFile.ReadValue("SDO","Code","Admin",MemDuty,10);//管理员
		}
		
		operSDO.GetLogText(strLog, "SDO_ModiMemDuty", "", "", operName, ServerName, FamilyName, NickName, MemDuty);
		operSDO.WriteText("SDO",strLog);
		
		if(type == 255)//如果要修改玩家为族长
		{
			int ShaikhNum = 0;//初始化家族族长个数信息
			if(!operSDO.getRemoteSql("SDO","SDO_FamilyShaikh_Query",operRemoteSql,1))
			{
				return DBTFLV;//无法得到查询家族族长信息的sql语句
			}
			operSDO.QuerySingleValue(&ShaikhNum, "SDO", ServerIP, 0, operRemoteSql, SDO_DBName, FamilyID);//获取家族族长的个数
			if(ShaikhNum != 0)//如果已经存在家族族长
			{
				DBTFLV = operSDO.ReturnOpMsg(strLog, "SDO_ShaiExist");
				return DBTFLV;
			}
		}
		
		if(!operSDO.getRemoteSql("SDO","SDO_Update_MemDuty",operRemoteSql,1))
		{
			return DBTFLV;//无法得到修改玩家职务信息的sql语句
		}
		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,SDO_DBName,type,MemDuty,FamilyID,UserID);	
		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_ModiMemDuty", "", "Fail", operName, ServerName, FamilyName, NickName, MemDuty);
			operSDO.WriteDBLog(userByID,"SDO","SDO_ModiMemDuty",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog, "Update_Member_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_ModiMemDuty", "", "Success", operName, ServerName, FamilyName, NickName, MemDuty);
			operSDO.WriteDBLog(userByID,"SDO","SDO_ModiMemDuty",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog, "Update_Member_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog, "Error");
	}

	return DBTFLV;
}

//解散家族
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_DeleteFamily(int userByID, char * ServerName, char * ServerIP, int FamilyID, char *FamilyName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_FamilyDelete", "", "", operName, ServerName, FamilyName);
		operSDO.WriteText("SDO",strLog);
		
		char TableFlag[20];
		char TableName[40];
		ZeroMemory(TableFlag,20);//初始化表的标识符
		ZeroMemory(TableName,40);//初始化表名称
		logFile.ReadValue("SDO","TABLE","TabelName",TableName,40);//获取家族徽章的表名称
		
		if(!operSDO.getRemoteSql("SDO","SDO_FamSelDBInfo",operRemoteSql,1))
		{
			return DBTFLV;//无法得到查询家族的徽章相关信息的sql语句
		}
		sprintf(operszSql,operRemoteSql,SDO_DBName,SDO_DBName,FamilyID);//构造完整查询的sql语句
		
		if(!operSDO.BackupDataBase("",localIP,0,TableName,"SDO",ServerIP,0,operszSql))//将查询得到的数据插入到T_sdo_personal_emblem表里面
		{
			return DBTFLV;
		}
		
		if(!operSDO.getRemoteSql("SDO","SDO_FamilyDBInfo",operRemoteSql,1))
		{
			return DBTFLV;//无法得到查询家族的相关数据库信息的sql语句
		}
		char TableNum[4];
		ZeroMemory(TableNum,4);
		logFile.ReadValue("SDO","TABLE","TabelNum", TableNum, 4);//获取要备份的表的数量
		int intTableNum = StrToInt(TableNum);
		int i=1;
		for(i=1; i<=intTableNum; i++)
		{
			sprintf(TableFlag,"%s%i","TabelName",i);
			logFile.ReadValue("SDO","TABLE",TableFlag,TableName,40);//获取相应的家族表名称

			sprintf(operszSql,operRemoteSql,SDO_DBName,TableName,FamilyID);//构造完整查询的sql语句
			if(!operSDO.BackupDataBase("",localIP,0,TableName,"SDO",ServerIP,0,operszSql))//将查询得到的数据插入到备份家族表里面
			{
				return DBTFLV;
			}
		}
		
		if(!operSDO.getRemoteSql("SDO","SDO_DelFamEmblem",operRemoteSql,1))
		{
			return DBTFLV;//无法得到删除家族的徽章表信息的sql语句
		}

		int tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,SDO_DBName,SDO_DBName,FamilyID);

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_FamilyDelete", "", "Fail", operName, ServerName, FamilyName);
			operSDO.WriteDBLog(userByID,"SDO","SDO_FamilyDelete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog, "Delete_Family_Fail");
			return DBTFLV;
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_DelFamTable",operRemoteSql,1))
		{
			return DBTFLV;//无法得到删除家族的相应表信息的sql语句
		}
		for(i=1; i<=intTableNum; i++)
		{
			sprintf(TableFlag,"%s%i","TabelName",i);
			logFile.ReadValue("SDO","TABLE",TableFlag,TableName,40);//获取相应的家族表名称

			tmpLength = 0;
			tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,SDO_DBName,TableName,FamilyID);
			if(tmpLength == 0)
			{
				operSDO.GetLogText(strLog, "SDO_FamilyDelete", "", "Fail", operName, ServerName, FamilyName);
				operSDO.WriteDBLog(userByID,"SDO","SDO_FamilyDelete",ServerIP,strLog);
				
				DBTFLV = operSDO.ReturnOpMsg(strLog, "Delete_Family_Fail");
				return DBTFLV;
			}
		}
		operSDO.GetLogText(strLog, "SDO_FamilyDelete", "", "Success", operName, ServerName, FamilyName);
		operSDO.WriteDBLog(userByID,"SDO","SDO_FamilyDelete",ServerIP,strLog);
		
		DBTFLV = operSDO.ReturnOpMsg(strLog, "Delete_Family_Success");
		
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog, "Error");
	}

	return DBTFLV;
}

//添加家族徽章
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_Insert_FamilyBadge(int userByID, char * ServerName, char * ServerIP, char *NickName, int UserID, int FamilyID, char *FamilyName, int EmblemType, int EmblemNum)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_InsertEmblem", "", "", operName, ServerName, FamilyName, NickName, EmblemType, EmblemNum);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_Emblem_Insert",operRemoteSql,1))
		{
			return DBTFLV;//无法得到添加家族徽章的sql语句
		}

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,SDO_DBName,UserID,EmblemType,EmblemNum); 	

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_InsertEmblem", "", "Fail",operName, ServerName, FamilyName, NickName, EmblemType, EmblemNum);
			operSDO.WriteDBLog(userByID,"SDO","SDO_InsertEmblem",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog, "Insert_Emblem_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_InsertEmblem", "", "Success",operName, ServerName, FamilyName, NickName, EmblemType, EmblemNum);
			operSDO.WriteDBLog(userByID,"SDO","SDO_InsertEmblem",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog, "Insert_Emblem_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog, "Error");
	}

	return DBTFLV;
}

//删除家族徽章
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_Delete_FamilyBadge(int userByID, char * ServerName, char * ServerIP, char *NickName, int UserID, char *FamilyName, int EmblemType)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_DeleteEmblem", "", "", operName, ServerName, FamilyName, NickName, EmblemType);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO","SDO_Emblem_Delete",operRemoteSql,1))
		{
			return DBTFLV;//无法得到删除家族徽章的sql语句
		}
		sprintf(operszSql,operRemoteSql,SDO_DBName,UserID,EmblemType);//构造完整查询的sql语句

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,SDO_DBName,UserID,EmblemType);
				
		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_DeleteEmblem", "", "Fail",operName, ServerName, FamilyName, NickName, EmblemType);
			operSDO.WriteDBLog(userByID,"SDO","SDO_DeleteEmblem",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog, "Delete_Emblem_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_DeleteEmblem", "", "Success",operName, ServerName, FamilyName, NickName, EmblemType);
			operSDO.WriteDBLog(userByID,"SDO","SDO_DeleteEmblem",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog, "Delete_Emblem_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog, "Error");
	}

	return DBTFLV;
}

//查看玩家G币
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_UserGCash_Query(int userByID, char * ServerName, char * ServerIP, char *Account)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_UserGCash", "", operName,ServerName,Account);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO","SDO_UserGCash_Query",operRemoteSql,1))
		{
			return DBTFLV;//无法得到查看玩家G币信息的sql语句
		}
//		printf(operRemoteSql,SDO_DBName,Account);
		DBTFLV = GameDBQuery("SDO", ServerIP, 0, 0, 0, operRemoteSql,SDO_DBName,Account);
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//补发玩家G币
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_UserGCash_Update(int userByID, char * ServerName, char * ServerIP, char *Account, int GCash)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_UpdateGCash", "", "", operName, ServerName, Account, GCash);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_AddGCash_Query",operRemoteSql, 1))
		{
			return DBTFLV;//无法得到查询玩家有没有添加G币信息的sql语句
		}
//		printf(operRemoteSql,SDO_DBName,Account);
		DBTFLV = GameDBQuery("SDO", ServerIP, 0, 0, 0, operRemoteSql,SDO_DBName,Account);

		int tmpLength = 0;
		if(DBTFLV.empty())
		{
			if(!operSDO.getRemoteSql("SDO", "SDO_AddGCash_Insert",operRemoteSql,1))
			{
				return DBTFLV;//无法得到给玩家添加G币的sql语句
			}
//			printf(operRemoteSql,SDO_DBName,Account,GCash);
			tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,SDO_DBName,Account,GCash);
			}
		else
		{
			if(!operSDO.getRemoteSql("SDO", "SDO_AddGCash_Update",operRemoteSql, 1))
			{
				return DBTFLV;//无法得到给玩家更新G币的sql语句
			}
//			printf(operRemoteSql,SDO_DBName,GCash,Account);
			tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,SDO_DBName,GCash,Account);
		}

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_UpdateGCash", "", "Fail", operName, ServerName, Account, GCash);
			operSDO.WriteDBLog(userByID,"SDO","SDO_UpdateGCash",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Update_GCash_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_UpdateGCash", "", "Success", operName, ServerName, Account, GCash);
			operSDO.WriteDBLog(userByID,"SDO","SDO_UpdateGCash",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Update_GCash_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//大赛挑战信息查询
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_CHALLENGE_QUERY(int userByID, char * ServerName, char * ServerIP, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "BaseMessage1", "SDO_ChallengeInfo", "", operName,ServerName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))
		{
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_Challenge_Query",operRemoteSql, 1))
		{
			return DBTFLV;//无法得到查看大赛挑战信息的sql语句
		}
		DBTFLV = GameDBQuery("SDO", ServerIP, 0, iIndex, iPageSize, operRemoteSql,Item_DBName);
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//设置大赛挑战信息
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_CHALLENGE_INSERT(int userByID, char * ServerName, char * ServerIP, int weekDay, int matPtMin, int stPtMin, int GCash, int scence, int isBattle)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		char readTag[20];//获取配置文件的标签信息		
		char strWeekDay[10];//大赛设在周几的中文
		char SceneName[64];//场景的中文
		char BattleMode[20];//比赛模式的中文
		ZeroMemory(readTag,20);//初始化获取配置文件的标签信息
		ZeroMemory(strWeekDay,10);//初始化大赛设在周几的中文
		ZeroMemory(SceneName,64);//初始化场景的中文
		ZeroMemory(BattleMode,20);//初始化比赛模式的中文
		
		sprintf(readTag, "%s%i", "WeekDay", weekDay);
		logFile.ReadValue("SDO","Code",readTag,strWeekDay,10);//获取当前是星期几
		
		operSDO.getRemoteSql("SDO", "SDO_SceneName_Query",operRemoteSql,1);//获取查询场景名称的sql语句
		operSDO.QuerySingleValue(SceneName, "", localIP, 0, operRemoteSql, scence);//获取当前场景名称
		
		ZeroMemory(readTag,20);//初始化获取配置文件的标签信息
		if (isBattle == 10)
		{
			sprintf(readTag, "%s", "BattleType1");
			logFile.ReadValue("SDO","Code",readTag,BattleMode,20);//获取当前的比赛模式
		}
		else if (isBattle == 13)
		{
			sprintf(readTag, "%s", "BattleType2");
			logFile.ReadValue("SDO","Code",readTag,BattleMode,20);//获取当前的比赛模式
		}
		else if (isBattle == 12)
		{
			sprintf(readTag, "%s", "BattleType3");
			logFile.ReadValue("SDO","Code",readTag,BattleMode,20);//获取当前的比赛模式
		}
		else if (isBattle == 11)
		{
			sprintf(readTag, "%s", "BattleType4");
			logFile.ReadValue("SDO","Code",readTag,BattleMode,20);//获取当前的比赛模式
        }
		
		operSDO.GetLogText(strLog, "SDO_Challenge_Insert", "", "", operName, ServerName, strWeekDay,stPtMin,matPtMin,GCash,SceneName,BattleMode);//构造执行操作的日志
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))
		{
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_Challenge_Insert",operRemoteSql, 1))
		{
			return DBTFLV;//无法得到添加大赛挑战信息的sql语句
		}

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,Item_DBName,weekDay,matPtMin,stPtMin,GCash,scence,isBattle);
		
		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_Challenge_Insert", "", "Fail", operName, ServerName, strWeekDay,stPtMin,matPtMin,GCash,SceneName,BattleMode);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_Challenge_Insert",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Insert_Challenge_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_Challenge_Insert", "", "Success", operName, ServerName, strWeekDay,stPtMin,matPtMin,GCash,SceneName,BattleMode);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_Challenge_Insert",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Insert_Challenge_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//修改大赛挑战信息
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_CHALLENGE_UPDATE(int userByID, char * ServerName, char * ServerIP, int ChallengeID, int weekDay, int matPtMin, int stPtMin, int GCash, int scence, int isBattle)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		char readTag[20];//获取配置文件的标签信息		
		char strWeekDay[10];//大赛设在周几的中文
		char SceneName[64];//场景的中文
		char BattleMode[20];//比赛模式的中文
		ZeroMemory(readTag,20);//初始化获取配置文件的标签信息
		ZeroMemory(strWeekDay,10);//初始化大赛设在周几的中文
		ZeroMemory(SceneName,64);//初始化场景的中文
		ZeroMemory(BattleMode,20);//初始化比赛模式的中文
		
		sprintf(readTag, "%s%i", "WeekDay", weekDay);
		logFile.ReadValue("SDO","Code",readTag,strWeekDay,10);//获取当前是星期几
		
		operSDO.getRemoteSql("SDO", "SDO_SceneName_Query",operRemoteSql,1);//获取查询场景名称的sql语句
		operSDO.QuerySingleValue(SceneName,"",localIP,0,operRemoteSql,scence);//获取当前场景名称
		
		ZeroMemory(readTag,20);//初始化获取配置文件的标签信息
		if (isBattle == 10)
		{
			sprintf(readTag, "%s", "BattleType1");
			logFile.ReadValue("SDO","Code",readTag,BattleMode,20);//获取当前的比赛模式
		}
		else if (isBattle == 13)
		{
			sprintf(readTag, "%s", "BattleType2");
			logFile.ReadValue("SDO","Code",readTag,BattleMode,20);//获取当前的比赛模式
		}
		else if (isBattle == 12)
		{
			sprintf(readTag, "%s", "BattleType3");
			logFile.ReadValue("SDO","Code",readTag,BattleMode,20);//获取当前的比赛模式
		}
		else if (isBattle == 11)
		{
			sprintf(readTag, "%s", "BattleType4");
			logFile.ReadValue("SDO","Code",readTag,BattleMode,20);//获取当前的比赛模式
        }
		
		operSDO.GetLogText(strLog,"SDO_Challenge_Update","","",operName,ServerName,ChallengeID,strWeekDay,stPtMin,matPtMin,GCash,SceneName,BattleMode);//构造执行操作的日志
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))
		{
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		//保存备份信息
		operSDO.BakUpdateInfo(userByID,"SDO_Challenge_Update","SDO",ServerIP,0,Item_DBName, "SDO_ChallengeBak_Get", "SDO_CHALLENGE_UPDATE", ChallengeID, -1);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_Challenge_Update",operRemoteSql, 1))
		{
			return DBTFLV;//无法得到修改大赛挑战信息的sql语句
		}

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,Item_DBName,weekDay,matPtMin,stPtMin,GCash,scence,isBattle,ChallengeID);

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog,"SDO_Challenge_Update","","Fail",operName,ServerName,ChallengeID,strWeekDay,stPtMin,matPtMin,GCash,SceneName,BattleMode);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_Challenge_Update",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Update_Challenge_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog,"SDO_Challenge_Update","","Success",operName,ServerName,ChallengeID,strWeekDay,stPtMin,matPtMin,GCash,SceneName,BattleMode);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_Challenge_Update",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Update_Challenge_Success");
		}
	}
	catch(...)
	{ 
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//删除大赛挑战信息
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_CHALLENGE_DELETE(int userByID, char * ServerName, char * ServerIP, int ChallengeID)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog,"SDO_Challenge_Delete","","",operName,ServerName,ChallengeID);//构造执行操作的日志
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP, ServerIP,2))
		{
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_Challenge_Delete",operRemoteSql, 1))
		{
			return DBTFLV;//无法得到删除大赛挑战信息的sql语句
		}

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,Item_DBName,ChallengeID);
		
		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog,"SDO_Challenge_Delete","","Fail",operName,ServerName,ChallengeID);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_Challenge_Delete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Delete_Challenge_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog,"SDO_Challenge_Delete","","Success",operName,ServerName,ChallengeID);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_Challenge_Delete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Delete_Challenge_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//查看大赛场景列表
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_SCENE_QUERY(int userByID, char * ServerName, char * ServerIP)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage2", "SDO_Scene_Query", "", operName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_Scene_Query",operRemoteSql, 1))
		{
			return DBTFLV;//无法得到查看大赛场景列表的sql语句
		}

		DBTFLV = GameDBQuery("SDO", localIP, 4, 0, 0, operRemoteSql);		
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//添加大赛挑战的场景
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_SCENE_CREATE(int userByID, char * ServerName, char * ServerIP, char *SceneName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	
	try
	{
		operSDO.GetLogText(strLog, "SDO_Scene_Insert", "", "", operName,SceneName);//构造日志语句
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_Scene_Insert",operRemoteSql, 1))
		{
			return DBTFLV;//无法得到添加大赛挑战场景的sql语句
		}

		int tmpLength = 0;
//		printf(operRemoteSql,SceneName);
		tmpLength = GameDBOper("SDO", localIP, 4, operRemoteSql,SceneName);

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_Scene_Insert", "", "Fail", operName,SceneName);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_Scene_Insert",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Insert_Scene_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_Scene_Insert", "", "Success", operName,SceneName);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_Scene_Insert",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Insert_Scene_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//修改大赛挑战的场景
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_SCENE_UPDATE(int userByID, char * ServerName, char * ServerIP, int SceneID, char *SceneName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		int SceneID =StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::SDO_SenceID,0)).lpdata);//场景ID
		char SceneName[64];
		ZeroMemory(SceneName,64);//初始化场景名称
		sprintf(SceneName,"%s",(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::SDO_Sence,0)).lpdata);//获取场景名称
		
		operSDO.GetLogText(strLog, "SDO_Scene_Update", "", "", operName,SceneID,SceneName);//构造日志语句
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_Scene_Update",operRemoteSql, 1))
		{
			return DBTFLV;//无法得到修改大赛挑战场景的sql语句
		}

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", localIP, 4, operRemoteSql,SceneName,SceneID);

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_Scene_Update", "", "Fail", operName,SceneID,SceneName);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_Scene_Update",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Update_Scene_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_Scene_Update", "", "Success", operName,SceneID,SceneName);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_Scene_Update",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Update_Scene_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//删除大赛挑战的场景
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_SCENE_DELETE(int userByID, char * ServerName, char * ServerIP, int SceneID)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_Scene_Delete", "", "", operName,SceneID);//构造日志语句
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_Scene_Delete",operRemoteSql, 1))
		{
			return DBTFLV;//无法得到删除大赛挑战的场景的sql语句
		}

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", localIP, 4, operRemoteSql,SceneID);

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_Scene_Delete", "", "Fail", operName,SceneID);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_Scene_Delete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Delete_Scene_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_Scene_Delete", "", "Success", operName,SceneID);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_Scene_Delete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Delete_Scene_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//查看在游戏里摇摇乐获得概率
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_YYHappyItem_Query(int userByID, char * ServerName, char * ServerIP, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage1", "SDO_YYHappyItem_Query", "", operName,ServerName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))//获取真正的服务器IP
		{
			return DBTFLV;
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_YYHappyItem_Query",operRemoteSql, 1))
		{
			return DBTFLV;//无法得到查询游戏里摇摇乐获得概率的sql语句
		}

		DBTFLV = GameDBQuery("SDO", ServerIP, 0, iIndex, iPageSize, operRemoteSql,Item_DBName);

	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//添加摇摇乐获得概率
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_YYHappyItem_Insert(int userByID, char * ServerName, char * ServerIP, int itemCode1, int itemCode2, int level, int levPercent, int precent, int timeslimit, int dayslimit, char *itemName1, char *itemName2)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_YYHappyItem_Insert", "", "", operName, ServerName, 
			itemCode1,itemName1,itemCode2,itemName2,level,levPercent,precent,timeslimit,dayslimit);//构造执行操作的日志
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))
		{
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		int itemCount = 0;
		if(!operSDO.getRemoteSql("SDO", "SDO_YYHappyItem_Count",operRemoteSql,1))
		{
			return DBTFLV;//无法得到添加摇摇乐获得概率的sql语句
		}

		operSDO.QuerySingleValue(&itemCount,"SDO",ServerIP,0,operRemoteSql,Item_DBName,itemCode1);

		int tmpLength = 0;

		if(itemCount > 0)
		{
			if(!operSDO.getRemoteSql("SDO", "SDO_YYHappyItem_Create",operRemoteSql, 1))
			{
				return DBTFLV;//无法得到添加摇摇乐获得概率的sql语句
			}
			tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,Item_DBName,level,levPercent,precent,timeslimit,dayslimit,itemCode1);
		}
		else
		{
			if(!operSDO.getRemoteSql("SDO", "SDO_YYHappyItem_Insert",operRemoteSql,1))
			{
				return DBTFLV;//无法得到添加摇摇乐获得概率的sql语句
			}

			tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,Item_DBName,itemCode1,itemCode2,level,levPercent,precent,timeslimit,dayslimit);
		}		
		
		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_YYHappyItem_Insert", "", "Fail", operName, ServerName, 
				itemCode1,itemName1,itemCode2,itemName2,level,levPercent,precent,timeslimit,dayslimit);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_YYHappyItem_Insert",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Insert_YYHappyItem_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_YYHappyItem_Insert", "", "Success", operName, ServerName, 
				itemCode1,itemName1,itemCode2,itemName2,level,levPercent,precent,timeslimit,dayslimit);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_YYHappyItem_Insert",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Insert_YYHappyItem_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//更新摇摇乐获得概率
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_YYHappyItem_Update(int userByID, char * ServerName, char * ServerIP, int itemCode, int itemCode1, int itemCode2, int level, int levPercent, int precent, int timeslimit, int dayslimit, char *itemName1, char *itemName2)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_YYHappyItem_Update", "", "", operName, ServerName, itemCode,
			itemCode1,itemName1,itemCode2,itemName2,level,levPercent,precent,timeslimit,dayslimit);//构造执行操作的日志
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))
		{
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_YYHappyItem_Update",operRemoteSql, 1))
		{
			return DBTFLV;//无法得到更新摇摇乐获得概率的sql语句
		}

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,Item_DBName,itemCode1,itemCode2,level,levPercent,precent,timeslimit,dayslimit,itemCode);
		
		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_YYHappyItem_Update", "", "Fail", operName, ServerName, itemCode,
				itemCode1,itemName1,itemCode2,itemName2,level,levPercent,precent,timeslimit,dayslimit);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_YYHappyItem_Update",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Update_YYHappyItem_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_YYHappyItem_Update", "", "Success", operName, ServerName, itemCode,
				itemCode1,itemName1,itemCode2,itemName2,level,levPercent,precent,timeslimit,dayslimit);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_YYHappyItem_Update",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Update_YYHappyItem_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//删除摇摇乐道具获得概率
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_YYHappyItem_Delete(int userByID, char * ServerName, char * ServerIP, int itemCode)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_YYHappyItem_Delete", "", "", operName,ServerName, itemCode);//构造日志语句
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))
		{
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_YYHappyItem_Delete",operRemoteSql, 1))
		{
			return DBTFLV;//无法得到删除摇摇乐道具获得概率的sql语句
		}

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,Item_DBName,itemCode);

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_YYHappyItem_Delete", "", "Fail", operName,ServerName, itemCode);//构造日志语句
			operSDO.WriteDBLog(userByID,"SDO","SDO_Scene_Delete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Delete_YYHappyItem_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_YYHappyItem_Delete", "", "Success", operName,ServerName, itemCode);//构造日志语句
			operSDO.WriteDBLog(userByID,"SDO","SDO_Scene_Delete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Delete_YYHappyItem_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//查看游戏里面的道具信息
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_Item_QueryAll(int userByID, char * ServerName, char * ServerIP, int BigType, int SmallType, int Sex, char * itemName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_ItemInfo_All", "", "", operName,ServerName,BigType,SmallType,Sex,itemName);
		operSDO.WriteText("SDO",strLog);
		
//		printf("exec SDO_ItemShop_Query_ALL '%s','%i','%i',%i,'%s'",ServerIP, BigType, SmallType, Sex, itemName);
		DBTFLV = GameDBQuery("SDO", localIP, 4, 0, 0, "exec SDO_ItemShop_Query_ALL '%s','%i','%i',%i,'%s'", 
			ServerIP, BigType, SmallType, Sex, itemName);
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//道具获取比率查询
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_MedalItem_Query(int userByID, char * ServerName, char * ServerIP, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage1", "SDO_MedalItem_Query", "", operName,ServerName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))//获取真正的服务器IP
		{
			return DBTFLV;
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_MedalItem_Query",operRemoteSql, 1))
		{
			return DBTFLV;//无法得到查询道具获取比率的sql语句
		}

		DBTFLV = GameDBQuery("SDO", ServerIP, 0, iIndex, iPageSize, operRemoteSql,Item_DBName);

	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//单个道具获取比率查询
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_MedalItem_Owner_Query(int userByID, char * ServerName, char * ServerIP, char *itemName, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage1", "SDO_MedalItem_Owner_Query", "", operName,ServerName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))//获取真正的服务器IP
		{
			return DBTFLV;
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_MedalItem_Owner_Query",operRemoteSql, 1))
		{
			return DBTFLV;//无法得到查询单个道具获取比率的sql语句
		}
		printf(operRemoteSql, "\%", itemName, "\%");
		DBTFLV = GameDBQuery("SDO", localIP, 4, iIndex, iPageSize, operRemoteSql, "\%", itemName, "\%");
	
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//添加道具获取比率
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_MedalItem_Insert(int userByID, char * ServerName, char * ServerIP, int itemCode, int precent, char *itemName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog,"SDO_MedalItem_Insert","","",operName,ServerName,itemCode,itemName,precent);//构造执行操作的日志
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))
		{
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		int itemCount = 0;
		if(!operSDO.getRemoteSql("SDO", "SDO_MedalItem_Count",operRemoteSql, 1))
		{
			return DBTFLV;//无法得到查看道具相应比率存不存在的sql语句
		}
		operSDO.QuerySingleValue(&itemCount,"SDO",ServerIP,0,operRemoteSql,Item_DBName,itemCode);

		int tmpLength = 0;

		if(itemCount > 0)
		{
			if(!operSDO.getRemoteSql("SDO", "SDO_MedalItem_Update",operRemoteSql, 1))
			{
				return DBTFLV;//无法得到更新道具获取比率的sql语句
			}

			tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,Item_DBName,precent,itemCode);
		}
		else
		{
			if(!operSDO.getRemoteSql("SDO", "SDO_MedalItem_Insert",operRemoteSql, 1))
			{
				return DBTFLV;//无法得到添加道具获取比率的sql语句 
			}
			tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,Item_DBName,itemCode,precent);
		}		
		
		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog,"SDO_MedalItem_Insert","","Fail",operName,ServerName,itemCode,itemName,precent);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_MedalItem_Insert",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Insert_MedalItem_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog,"SDO_MedalItem_Insert","","Success",operName,ServerName,itemCode,itemName,precent);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_MedalItem_Insert",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Insert_MedalItem_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//更新道具获取比率
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_MedalItem_Update(int userByID, char * ServerName, char * ServerIP, int itemCode, int precent, char *itemName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog,"SDO_MedalItem_Update","","",operName,ServerName,itemCode,itemName,precent);//构造执行操作的日志
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))
		{
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_MedalItem_Update",operRemoteSql, 1))
		{
			return DBTFLV;//无法得到更新道具获取比率的sql语句
		}

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,Item_DBName,precent,itemCode);

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog,"SDO_MedalItem_Update","","Fail",operName,ServerName,itemCode,itemName,precent);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_MedalItem_Update",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Update_MedalItem_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog,"SDO_MedalItem_Update","","Success",operName,ServerName,itemCode,itemName,precent);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_MedalItem_Update",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Update_MedalItem_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//删除道具获取比率
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_MedalItem_Delete(int userByID, char * ServerName, char * ServerIP, int itemCode, char *itemName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		int itemCode = StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::SDO_ItemCode,0)).lpdata);//道具code
		char itemName[128];
		ZeroMemory(itemName,128);
		sprintf(itemName,"%s",(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::SDO_ItemName,0)).lpdata);//获取道具名称
		
		operSDO.GetLogText(strLog,"SDO_MedalItem_Delete","","",operName,ServerName,itemCode,itemName);//构造执行操作的日志
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))
		{
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_MedalItem_Delete",operRemoteSql, 1))
		{
			return DBTFLV;//无法得到删除道具获取比率的sql语句
		}

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,Item_DBName,itemCode);

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog,"SDO_MedalItem_Delete","","Fail",operName,ServerName,itemCode,itemName);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_MedalItem_Delete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Delete_MedalItem_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog,"SDO_MedalItem_Delete","","Success",operName,ServerName,itemCode,itemName);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_MedalItem_Delete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Delete_MedalItem_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//更新玩家角色信息
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_CharacterInfo_Update(int userByID, char * ServerName, char * ServerIP, int UserID, char *Account,int level, int experience, int battle, int win, int draw, int lose, int MCash, int GCash)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_CharacterInfo_Update", "", "", operName, ServerName,Account, 
			level,experience,battle,win,draw,lose,MCash,GCash);//构造执行操作的日志
		operSDO.WriteText("SDO",strLog);
		
		//备份更新信息
		operSDO.BakUpdateInfo(userByID,"SDO_CharacterInfo_Update","SDO",ServerIP,0,SDO_DBName,"SDO_CharInfo_Bak","SDO_CharInfo_Update",UserID,-1);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_CharInfo_Update",operRemoteSql, 1))
		{
			return DBTFLV;//无法得到更新玩家角色信息的sql语句
		}
	
		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,SDO_DBName,experience,win,lose,MCash,GCash,UserID);

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_CharacterInfo_Update", "", "Fail", operName, ServerName,Account, 
				level,experience,battle,win,draw,lose,MCash,GCash);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_CharacterInfo_Update",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Update_CharacterInfo_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_CharacterInfo_Update", "", "Success", operName, ServerName,Account, 
				level,experience,battle,win,draw,lose,MCash,GCash);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_CharacterInfo_Update",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Update_CharacterInfo_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//删除玩家身上道具
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_ItemShop_Delete(int userByID, char * ServerName, char * ServerIP, int UserID, char *Account, int itemCode, char *itemName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog,"SDO_UserEquip_Delete","","",operName,Account,itemCode,itemName);//构造执行操作的日志
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))
		{
			return DBTFLV;//无法获取真正的服务器IP
		}
		
		//备份删除信息
		operSDO.BakDeleteInfo(userByID,"SDO_UserEquip_Delete","SDO",ServerIP,0,Item_DBName,
			"T_sdo_item","SDO_Remote_GetEquipInfo","SDO_Remote_SaveInfo",itemCode, UserID);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_UserEquip_Delete",operRemoteSql,1))
		{
			return DBTFLV;//无法得到删除玩家身上道具的sql语句
		}

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,Item_DBName,itemCode,UserID);

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog,"SDO_UserEquip_Delete","","Fail",operName,ServerName,Account,itemCode,itemName);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_UserEquip_Delete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Delete_UserEquip_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog,"SDO_UserEquip_Delete","","Success",operName,ServerName,Account,itemCode,itemName);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_UserEquip_Delete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Delete_UserEquip_Success");
		}
	}
	catch (...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//查看玩家礼物盒道具
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_GiftBox_Query(int userByID, char * ServerName, char * ServerIP, int UserID, char *Account, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_GiftBox", "", operName,ServerName,Account);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_GiftBox_Query",operRemoteSql, 1))
		{
			return DBTFLV;//无法得到查看玩家礼物盒道具的sql语句
		}

		DBTFLV = GameDBQuery("SDO", ServerIP, 0, iIndex, iPageSize, operRemoteSql,SDO_DBName,UserID);		
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
	}
	return DBTFLV;
}

//删除玩家礼物盒道具
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_GiftBox_Delete(int userByID, char * ServerName, char * ServerIP, int UserID, char *Account, int itemCode, char *itemName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog,"SDO_GiftBox_Delete","","",operName,ServerName,Account,itemCode,itemName);//构造执行操作的日志
		operSDO.WriteText("SDO",strLog);
		
		//备份删除信息
		operSDO.BakDeleteInfo(userByID,"SDO_GiftBox_Delete","SDO",ServerIP,0,SDO_DBName,
			"T_sdo_Message","SDO_BackUp_GiftBox","SDO_Remote_SaveInfo",itemCode, UserID);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_GiftBox_Delete",operRemoteSql, 1))
		{
			return DBTFLV;//无法得到删除玩家礼物盒道具的sql语句
		}

		int tmpLength;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,SDO_DBName,itemCode,UserID);

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog,"SDO_GiftBox_Delete","","Fail",operName,ServerName,Account,itemCode,itemName);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_GiftBox_Delete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Delete_GiftBox_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog,"SDO_GiftBox_Delete","","Success",operName,ServerName,Account,itemCode,itemName);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_GiftBox_Delete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Delete_GiftBox_Success");
		}
	}
	catch (...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//给玩家发送道具
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_GiftBox_Insert(int userByID, char * ServerName, char * ServerIP, int UserID, char *Account, int itemCode, int timeslimit, int DateLimit, char *itemName, char *title, char *content)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_GiftBox_Insert", "", "", operName, ServerName, Account,
			itemCode,itemName,timeslimit,DateLimit,title,content);//构造执行操作的日志
		operSDO.WriteText("SDO",strLog);
		
		int bigtype = 0;
		if(!operSDO.getRemoteSql("SDO", "SDO_ItemType_Query",operRemoteSql, 1))
		{
			return DBTFLV;//无法得到查询道具所属大类的sql语句
		}
		operSDO.QuerySingleValue(&bigtype,"",localIP,0,operRemoteSql,itemCode);

		if(bigtype == 3)
		{
			if(!operSDO.getRemoteSql("SDO", "SDO_GiftBox_Insert",operRemoteSql, 1))
			{
				return DBTFLV;//无法得到给玩家发送道具的sql语句
			}
			sprintf(operszSql,operRemoteSql,SDO_DBName,UserID,title,content,itemCode,DateLimit,-1,0,3);//构造完整操作的sql语句		
		}
		else
		{
			char StrInt[10];
			char tmpStr[10];
			ZeroMemory(StrInt,10);//初始化
			ZeroMemory(tmpStr, 10);
			_itoa(itemCode, StrInt, 10);
			int i=0;//循环因子
			for(i=1; i<=12; i++)
			{
				sprintf(tmpStr, "0%i", i);
				if(!strcmp(tmpStr, StrInt))
				{
					if(!operSDO.getRemoteSql("SDO", "SDO_GiftMedal_Insert",operRemoteSql, 1))
					{
						return DBTFLV;//无法得到给玩家发送勋章道具的sql语句
					}
					sprintf(operszSql,operRemoteSql,SDO_DBName,2^(i-1),2^(i-1),UserID);//构造完整操作的sql语句
					break;
				}
			}
			if(i<=12)
			{
			}
			else
			{
				if(!operSDO.getRemoteSql("SDO", "SDO_GiftBox_Insert",operRemoteSql, 1))
				{
					return DBTFLV;//无法得到给玩家发送道具的sql语句
				}
			    if((itemCode==100003) || (itemCode==100004) || (itemCode==100005) || (itemCode==100006) || (itemCode==100007) ||
					(itemCode==100021) || (itemCode==100022) || (itemCode==100023) || (itemCode==100024) || (itemCode==100025) || 
					(itemCode==100061) || (itemCode==100063) || (itemCode==100611) || (itemCode==100612) || (itemCode==100868) || 
					(itemCode==100776) || (itemCode==100778) || (itemCode==100780) || (itemCode==100754) || (itemCode==100756) || 
					(itemCode==100758) || (itemCode==100069) || (itemCode==100065) || (itemCode==100067) || (itemCode==100420) || 
					(itemCode==100728) || (itemCode==100922) || (itemCode==100924) || (itemCode==100926) || (itemCode==100912) || 
					(itemCode==100913) || (itemCode==100914) || (itemCode==100915) || (itemCode==100898) || (itemCode==200000) || 
					(itemCode==202999))
				{					
					sprintf(operszSql,operRemoteSql,SDO_DBName,UserID,title,content,itemCode,DateLimit,-1,0,3);//构造完整操作的sql语句	
				}
				else if((itemCode==100000) || (itemCode==100002) || (itemCode==100020) || (itemCode==100030) || (itemCode==100031) || 
					(itemCode==100032) || (itemCode==100033) || (itemCode==100034) || (itemCode==100035) || (itemCode==100036) || 
					(itemCode==100037) || (itemCode==100038) || (itemCode==100039) || (itemCode==100040) || (itemCode==100041) || 
					(itemCode==100050) || (itemCode==100051) || (itemCode==100052) || (itemCode==100060) || (itemCode==100062) || 
					(itemCode==100064) || (itemCode==100066) || (itemCode==100068) || (itemCode==100070) || (itemCode==100071) || 
					(itemCode==100072) || (itemCode==100080) || (itemCode==100081) || (itemCode==100082) || (itemCode==100083) || 
					(itemCode==100084) || (itemCode==100085) || (itemCode==100086) || (itemCode==100087) || (itemCode==100088) || 
					(itemCode==100089) || (itemCode==100140) || (itemCode==100141) || (itemCode==100142) || (itemCode==100410) || 
					(itemCode==100500) || (itemCode==100501) || (itemCode==100502) || (itemCode==100601) || (itemCode==100602) || 
					(itemCode==100713) || (itemCode==100714) || (itemCode==100715) || (itemCode==100716) || (itemCode==100717) || 
					(itemCode==100718) || (itemCode==100719) || (itemCode==100720) || (itemCode==100721) || (itemCode==100722) || 
					(itemCode==100723) || (itemCode==100724) || (itemCode==100725) || (itemCode==100726) || (itemCode==100727) || 
					(itemCode==100729) || (itemCode==100730) || (itemCode==100731) || (itemCode==100732) || (itemCode==100733) || 
					(itemCode==100739) || (itemCode==100741) || (itemCode==100743) || (itemCode==100745) || (itemCode==100747) || 
					(itemCode==100749) || (itemCode==100751) || (itemCode==100753) || (itemCode==100755) || (itemCode==100757) || 
					(itemCode==100759) || (itemCode==100761) || (itemCode==100763) || (itemCode==100765) || (itemCode==100767) || 
					(itemCode==100769) || (itemCode==100771) || (itemCode==100773) || (itemCode==100775) || (itemCode==100777) || 
					(itemCode==100779) || (itemCode==100781) || (itemCode==100783) || (itemCode==100785) || (itemCode==100787) || 
					(itemCode==100789) || (itemCode==100790) || (itemCode==100792) || (itemCode==100794) || (itemCode==100796) || 
					(itemCode==100797) || (itemCode==100798) || (itemCode==100807) || (itemCode==100815) || (itemCode==100823) || 
					(itemCode==100831) || (itemCode==100839) || (itemCode==100847) || (itemCode==100855) || (itemCode==100866) || 
					(itemCode==100867) || (itemCode==100869) || (itemCode==100876) || (itemCode==100882) || (itemCode==100883) || 
					(itemCode==100884) || (itemCode==100886) || (itemCode==100887) || (itemCode==100888) || (itemCode==100889) || 
					(itemCode==100890) || (itemCode==100891) || (itemCode==100892) || (itemCode==100893) || (itemCode==100894) || 
					(itemCode==100895) || (itemCode==100896) || (itemCode==100897) || (itemCode==100903) || (itemCode==100904) || 
					(itemCode==100905) || (itemCode==100906) || (itemCode==100907) || (itemCode==100908) || (itemCode==100909) || 
					(itemCode==100910) || (itemCode==100911) || (itemCode==100916) || (itemCode==100917) || (itemCode==100918) || 
					(itemCode==100927))
				{
				    sprintf(operszSql,operRemoteSql,SDO_DBName,UserID,title,content,itemCode,-1,timeslimit,0,3);//构造完整操作的sql语句
				}
				else
				{
					sprintf(operszSql,operRemoteSql,SDO_DBName,UserID,title,content,itemCode,DateLimit,timeslimit,0,3);//构造完整操作的sql语句
				}
			}
		}
		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operszSql);

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_GiftBox_Insert", "", "Fail", operName, ServerName, Account,
			    itemCode,itemName,timeslimit,DateLimit,title,content);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_GiftBox_Insert",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Insert_GiftBox_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_GiftBox_Insert", "", "Success", operName, ServerName, Account,
			    itemCode,itemName,timeslimit,DateLimit,title,content);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_GiftBox_Insert",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Insert_GiftBox_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//批量添加比赛信息
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_Challenge_InsertAll(int userByID, char * ServerName, char * ServerIP, int weekDay, int matPtMin, int stPtMin, int GCash, int scence, int isBattle)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		char readTag[20];//获取配置文件的标签信息		
		char strWeekDay[10];//大赛设在周几的中文
		char SceneName[64];//场景的中文
		char BattleMode[20];//比赛模式的中文
		ZeroMemory(readTag,20);//初始化获取配置文件的标签信息
		ZeroMemory(strWeekDay,10);//初始化大赛设在周几的中文
		ZeroMemory(SceneName,64);//初始化场景的中文
		ZeroMemory(BattleMode,20);//初始化比赛模式的中文
		
		sprintf(readTag, "%s%i", "WeekDay", weekDay);
		logFile.ReadValue("SDO","Code",readTag,strWeekDay,10);//获取当前是星期几
		
		operSDO.getRemoteSql("SDO", "SDO_SceneName_Query",operRemoteSql, 1);//获取查询场景名称的sql语句
		operSDO.QuerySingleValue(SceneName, "", localIP, 0, operRemoteSql, scence);//获取当前场景名称
		
		ZeroMemory(readTag,20);//初始化获取配置文件的标签信息
		if (isBattle == 10)
		{
			sprintf(readTag, "%s", "BattleType1");
			logFile.ReadValue("SDO","Code",readTag,BattleMode,20);//获取当前的比赛模式
		}
		else if (isBattle == 13)
		{
			sprintf(readTag, "%s", "BattleType2");
			logFile.ReadValue("SDO","Code",readTag,BattleMode,20);//获取当前的比赛模式
		}
		else if (isBattle == 12)
		{
			sprintf(readTag, "%s", "BattleType3");
			logFile.ReadValue("SDO","Code",readTag,BattleMode,20);//获取当前的比赛模式
		}
		else if (isBattle == 11)
		{
			sprintf(readTag, "%s", "BattleType4");
			logFile.ReadValue("SDO","Code",readTag,BattleMode,20);//获取当前的比赛模式
        }
		
		operSDO.GetLogText(strLog, "SDO_Challenge_InsertAll", "", "", operName, ServerName, strWeekDay,stPtMin,matPtMin,GCash,SceneName,BattleMode);//构造执行操作的日志
		operSDO.WriteText("SDO",strLog);

		char SuccessSvr[500];
		char FailSvr[500];
		ZeroMemory(SuccessSvr, 500);//初始化添加成功的服务器名称
		ZeroMemory(FailSvr, 500);//初始化添加失败的服务器名称

		char tempServerIP[64];
		ZeroMemory(tempServerIP,64);

		const char* split = ",";//服务器地址用","隔开
		int splitNum = 1;//设置为获取第一个","号
		
		char tmpServerIP[64];
		char tmpServerName[64];
		ZeroMemory(tmpServerIP, 64);
		ZeroMemory(tmpServerName, 64);
		operSDO.strGetChar(ServerIP, split, tmpServerIP, 64, splitNum);//获取第一个服务器IP
		operSDO.strGetChar(ServerName, split, tmpServerName, 64, splitNum);//获取第一个服务器名称
		int tmpLength = 0;
		while(strcmp("",tmpServerIP))
		{
			if(!operSDO.GetServerIP(tempServerIP,tmpServerIP,2))
			{
				return DBTFLV;//无法获取真正的服务器IP
			}
			
			if(!operSDO.getRemoteSql("SDO", "SDO_Challenge_Insert",operRemoteSql, 1))
			{
				return DBTFLV;//无法得到添加大赛挑战信息的sql语句
			}

			tmpLength = GameDBOper("SDO", tempServerIP, 0, operRemoteSql,Item_DBName,weekDay,matPtMin,stPtMin,GCash,scence,isBattle);

			if(tmpLength == 0)
			{
				if(strcmp("",FailSvr))
				{
					sprintf(FailSvr,"%s,%s",FailSvr,tmpServerName);
				}
				else
				{
					sprintf(FailSvr,"%s",tmpServerName);
				}
			}
			else
			{
				if(strcmp("",SuccessSvr))
				{
					sprintf(SuccessSvr,"%s,%s",SuccessSvr,tmpServerName);
				}
				else
				{
					sprintf(SuccessSvr,"%s",tmpServerName);
				}
			}
			splitNum++;//循环增加","的个数
			operSDO.strGetChar(ServerIP, split, tmpServerIP, 64, splitNum);//获取第splitNum个服务器IP
	    	operSDO.strGetChar(ServerName, split, tmpServerName, 64, splitNum);//获取第splitNum个服务器名称
		}
		ZeroMemory(strLog, 2048);
		if(!strcmp("",FailSvr))
		{			
			operSDO.GetLogText(strLog, "SDO_Challenge_InsertAll", "", "Success", operName, ServerName, strWeekDay,stPtMin,matPtMin,GCash,SceneName,BattleMode);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_Challenge_InsertAll",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"InsertAll_Challenge_Success");
		}
		else if(!strcmp("",SuccessSvr))
		{
			operSDO.GetLogText(strLog, "SDO_Challenge_InsertAll", "", "Fail", operName, ServerName, strWeekDay,stPtMin,matPtMin,GCash,SceneName,BattleMode);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_Challenge_InsertAll",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"InsertAll_Challenge_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_Challenge_InsertAll_Result", "", "", operName, SuccessSvr, FailSvr, strWeekDay,stPtMin,matPtMin,GCash,SceneName,BattleMode);//构造执行操作的日志
			operSDO.WriteDBLog(userByID,"SDO","SDO_Challenge_InsertAll",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"MiddleState");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

/************************************************************************/
/* //查看玩家身上道具
bool CSDOInfo::SDO_UserEquip_Query()
{
try
{
operSDO.GetLogText(strLog, "BaseMessage", "SDO_UserEquip", "", m_UserName,SDO_ServerName,SDO_Account);
operSDO.WriteText("SDO",strLog);

		if(!operSDO.GetServerIP(SDO_ServerIP,SDO_ServerIP,2))
		{
		return false;//无法获取真正的服务器IP
		}
		
		  if(!operSDO.getRemoteSql("SDO_UserEquip_Query",operRemoteSql))
		  {
		  return false;//无法得到查看玩家身上道具的sql语句
		  }
		  sprintf(operszSql,operRemoteSql,Item_DBName,Item_DBName,UserID);//构造完整查询的sql语句
		  
			int tmpLength = DBVect.OperVectorMain("SDO", SDO_ServerIP, operszSql, 0, index, iPageSize);//构造从数据库查询得到的数据
			operSDO.ReturnData(&DBVect, tmpLength, operszSql);//发送数据
			}
			catch(...)
			{
			operSDO.ReturnOpMsg(strLog,"Error");//将错误信息写入日志中并返回错误信息
			}
			return false;
}                                                                     */
/************************************************************************/
//获取公告类型 <iSouNT: 1 及时公告，0 定时公告
void CSDOInfo::GetNoticeType(char *strDesNT,int &iDesNT, int iSouNT)
{
	if (iSouNT == 1)		//及时公告
	{
		logFile.ReadValue("SDO", "NOTICETYPE", "NOtice_inTime", strDesNT, sizeof(&strDesNT));
		iDesNT = -1;
	}
	else					//定时公告
	{
		logFile.ReadValue("SDO", "NOTICETYPE", "NOtice_Time", strDesNT, sizeof(&strDesNT));
		iDesNT = 0;
	}
}