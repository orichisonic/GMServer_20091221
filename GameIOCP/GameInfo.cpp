// GameInfo.cpp: implementation of the CGameInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameIOCP.h"
#include "GameInfo.h"
#include <afxinet.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameInfo::CGameInfo()
{

}

CGameInfo::~CGameInfo()
{
	Destroy();
}

COperVector * CGameInfo::initialize(CSession * pSess,CEnumCore::Message_Tag_ID MessageTagID, unsigned char * pbuf, int length)
{
	if(length>0)
	{
		recvVect.Create(length/8);//分配空间
		recvVect.DeBody(pbuf,length);//解析包结构
	}
	pGameSess = pSess;	
	CLogic::SocketToUserName(pSess->GetSocket(), operName);//保存执行操作的用户名
	return &recvVect;
}

//////////////////////////////////////////
/////参数说明:////////////////////////////
/////GameName ServerIP 数据库连接说明/////
////int flag/////////////////////////////
/////szFormat+后面的参数构建///////////
//////////////////////////////////////
////////数据库查询//////////////////
////查询数据库返回格式为TFLV的记录集
vector<CGlobalStruct::TFLV> CGameInfo::GameDBQuery(char * GameName,char * ServerIP, int flag,int index,int iPageSize,const char * szFormat,...)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{
		va_list vaList;
		va_start(vaList, szFormat);
		vsprintf(m_operszSql, szFormat, vaList);//获取整个sql语句
		va_end( vaList );
		CLogFile logFile;
		logFile.WriteErrorLog(GameName,m_operszSql);
		printf("----------%s\n",m_operszSql);
		vector<CGlobalStruct::DataRecord> DBDateSet = sendopervector.GetDBSet(GameName, ServerIP, m_operszSql, flag, index, iPageSize);//获取数据集

		if(!DBDateSet.empty())
		{
			DBTFLV= sendopervector.GetTFLV(DBDateSet);//转化数据集的格式为TFLV
		}
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询数据库内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询数据库文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询数据库其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return DBTFLV;
	}
	return DBTFLV;
}

//操作数据库，返回操作结果
int CGameInfo::GameDBOper(char * GameName,char * ServerIP, int flag,const char * szFormat,...)
{
	try
	{
		va_list vaList;///编译系统编译时，会将函数的参数依次放到栈中，这样根据固定参数的地址以及固定参数给出的相关信息很容易得到可变参数的个数、类型、值
		va_start(vaList, szFormat);
		vsprintf(m_operszSql, szFormat, vaList);//获取整个sql语句
		va_end( vaList );
		CLogFile logFile;
		logFile.WriteErrorLog(GameName,m_operszSql);
		return sendopervector.OperVectorMain(GameName,ServerIP,m_operszSql,flag);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"执行数据库内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return 0;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"执行数据库文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return 0;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"执行数据库其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return 0;
	}
}
/////查询成员变量////////
COperVector * CGameInfo::GetDBVect()
{
	return &sendopervector;
}

int CGameInfo::GetDBLength()
{
	return sendDBLength;
}
//删除两个类的链表内容以及回收内存空间
bool CGameInfo::Destroy()
{
	recvVect.Destroy();
	return true;
}

//角色基本资料（查询类）
vector<CGlobalStruct::TFLV> CGameInfo::UserInfo(char * GameName,char * sql_type1, char * sql_type2,char * ServerName, char * ServerIP,char * UserName, char * UserNick,int flag,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorGame operGame;
	try
	{
		char operRemoteSql[2048];
		if(strcmp("",UserName))//帐号不为空
		{
			if(!operGame.getRemoteSql(GameName,sql_type1,operRemoteSql,0))
			{
				return DBTFLV;
			}
			DBTFLV=GameDBQuery(GameName,ServerIP,flag,iIndex,iPageSize,operRemoteSql,UserName);
		}
		else
		{
			if(!operGame.getRemoteSql(GameName,sql_type2,operRemoteSql,0))
			{
				return DBTFLV;
			}
			DBTFLV=GameDBQuery(GameName,ServerIP,flag,iIndex,iPageSize,operRemoteSql,"\%",UserNick,"\%");
		}
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询类内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询类文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询类其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return DBTFLV;
	}
	return DBTFLV;
}

//角色相关信息的查询(int 唯一标识符)
vector<CGlobalStruct::TFLV> CGameInfo::UserDefault(char * GameName,char * sql_type,char * ServerName, char * ServerIP,char * UserName, int UserID,int flag,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorGame operGame;
	try
	{
		char operRemoteSql[2048];
		if(!operGame.getRemoteSql(GameName,sql_type,operRemoteSql,0))
		{
			return DBTFLV;
		}
		DBTFLV=GameDBQuery(GameName,ServerIP,flag,iIndex,iPageSize,operRemoteSql,UserID);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询角色相关信息内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询角色相关信息文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询角色相关信息其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return DBTFLV;
	}
	return DBTFLV;
}
//根据帐号踢人
vector<CGlobalStruct::TFLV> CGameInfo::KickPlayer(int userByID,char * ServerName, char * ServerIP,char * UserName)
{
	return true;
}

//GameName游戏名,ServerName,服务器名,ServerIP,服务器IP,sql1本地查询语句,sql2远程语句sqll3本地查询
//////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////
vector<CGlobalStruct::TFLV> CGameInfo::ClosePlayer(char * GameName,char * sql1,char * ServerIP,int flag,char * sql2, char* sql3)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		if(GameDBQuery(GameName,"127.0.0.1",4,0,0,sql1).empty())
		{
			if(GameDBOper(GameName,ServerIP,flag,sql2)>0)
			{
				if(	GameDBOper(GameName,"127.0.0.1",4,sql3))
				{
					sprintf((char *)&m_tflv.lpdata,"%s","封停成功");
				}
				else
				{
					sprintf((char *)&m_tflv.lpdata,"%s","插入本地数据库错误");
				}
			}
			else
			{
				sprintf((char *)&m_tflv.lpdata,"%s","远程封停失败");
			}
		}
		else
		{
			sprintf((char *)&m_tflv.lpdata,"%s","玩家已封停");
		}
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"用户封停内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"用户封停文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"用户封停其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return DBTFLV;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;

}

//根据帐号解封
vector<CGlobalStruct::TFLV> CGameInfo::OpenPlayer(char * GameName,char * sql1,char * ServerIP,int flag,char * sql2, char* sql3)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		if(!GameDBQuery(GameName,"127.0.0.1",4,0,0,sql1).empty())
		{
			if(GameDBOper(GameName,ServerIP,flag,sql2)>0)
			{
				if(	GameDBOper(GameName,"127.0.0.1",4,sql3))
				{
					sprintf((char *)&m_tflv.lpdata,"%s","解封成功");
				}
				else
				{
					sprintf((char *)&m_tflv.lpdata,"%s","插入本地数据库错误");
				}
			}
			else
			{
				sprintf((char *)&m_tflv.lpdata,"%s","远程解封失败");
			}
		}
		else
		{
			sprintf((char *)&m_tflv.lpdata,"%s","玩家未封停");
		}
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"用户解封内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"用户解封文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"用户解封其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return DBTFLV;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;
}

//查询封停列表
vector<CGlobalStruct::TFLV> CGameInfo::CloseList(char * GameName,char * sql,char * ServerName,char * ServerIP,int iIndex, int iPageSize)
{
	return GameDBQuery(GameName,"127.0.0.1",4,iIndex,iPageSize,sql,ServerName);
}

//查询单个账号
vector<CGlobalStruct::TFLV> CGameInfo::CloseSingle(char * GameName,char * sql,char * ServerName, char * ServerIP,char * UserName,int iIndex, int iPageSize)
{
	return GameDBQuery(GameName, "127.0.0.1",4, iIndex, iPageSize,sql,ServerName,UserName);
}


//查询公告
vector<CGlobalStruct::TFLV> CGameInfo::BroadTask_Query(char * GameName,char * m_sql,int iIndex,int iPageSize)
{
	return GameDBQuery(GameName, "127.0.0.1",4, iIndex, iPageSize,m_sql);
}

//更新公告
vector<CGlobalStruct::TFLV> CGameInfo::BroadTask_Update(char * GameName,char * sql,int userByID,int taskid,char * boardMessage,char * begintime,char * endtime,int interval,int status)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{
		CGlobalStruct::TFLV m_tflv;
		m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
		m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
		int tmpLength=GameDBOper(GameName,"127.0.0.1",4,
			sql,userByID,taskid,boardMessage,begintime,
			endtime,interval,status);
		if(tmpLength>0)
		{
			sprintf((char *)&m_tflv.lpdata,"%s","更新公告成功");
		}
		else
		{
			sprintf((char *)&m_tflv.lpdata,"%s","更新公告失败");
		}
		m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
		DBTFLV.push_back(m_tflv);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"更新公告内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"更新公告文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"更新公告其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return DBTFLV;
	}
	return DBTFLV;
}

//插入公告
vector<CGlobalStruct::TFLV> CGameInfo::BroadTask_Insert(char * GameName,char * sql,int userByID,char * ServerIP,char * GSServerIP,char * boardMessage,char * begintime,char * endtime,int interval)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{
		CGlobalStruct::TFLV m_tflv;
		m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
		m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
		int tmpLength=GameDBOper(GameName,"127.0.0.1",4,
			sql,userByID,ServerIP,GSServerIP,
			boardMessage,begintime,endtime,interval);
		if(tmpLength>0)
		{
			sprintf((char *)&m_tflv.lpdata,"%s","添加公告成功");
		}
		else
		{
			sprintf((char *)&m_tflv.lpdata,"%s","添加公告失败");
		}
		m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
		DBTFLV.push_back(m_tflv);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"添加公告内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"添加公告文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"添加公告其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return DBTFLV;
	}
	return DBTFLV;
}
//登陆登出信息
vector<CGlobalStruct::TFLV> CGameInfo::LoginInfo(char * ServerName, char * ServerIP,char * UserName, char * UserNick,char * IP,char * begintime, char * endtime,int iIndex, int iPageSize)
{
	return true;
}


vector<CGlobalStruct::TFLV> CGameInfo::BaseLog(char * GameName,char * sql_type,
											   char * ServerName, char * ServerIP,char* UserNick,int UserID,
											   int type,char * szStartTime,char * szEndTime,int flag,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorGame operGame;
	try
	{
		char operRemoteSql[2048];
		if(!operGame.getRemoteSql(GameName,sql_type,operRemoteSql,0))
		{
			return DBTFLV;
		}
		switch(type)
		{
		case 0:
			DBTFLV=GameDBQuery(GameName,ServerIP,flag,iIndex,iPageSize,operRemoteSql,szStartTime,szEndTime);
			break;
		case 1:
			DBTFLV=GameDBQuery(GameName,ServerIP,flag,iIndex,iPageSize,operRemoteSql,UserNick,szStartTime,szEndTime);
			break;
		case 2:
			DBTFLV=GameDBQuery(GameName,ServerIP,flag,iIndex,iPageSize,operRemoteSql,UserID,szStartTime,szEndTime);
			break;
		case 3:
			DBTFLV=GameDBQuery(GameName,ServerIP,flag,iIndex,iPageSize,operRemoteSql,UserNick,UserID,szStartTime,szEndTime);
			break;
		default:
			break;
		}
		
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询相关日志信息内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询相关日志信息文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询相关日志信息其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog(GameName,errMessage);
		return DBTFLV;
	}
	return DBTFLV;
}

//string CGameInfo::UserLogInterface(char *ServerIP,string NewUserName,string NewTime,int gameID,string BanType, string QueryType)
string CGameInfo::UserLogInterface(char *ServerIP,string NewUserName,string NewTime,int gameID,string BanType)
{
	char *_totalpath = new char[100];
	string _temppath  = "";
	CString szTemUrl = "";

	CInternetSession MySession("Session");
	CHttpConnection* lpConnetction;
	CHttpFile*       lpHttpFile;
	INTERNET_PORT    HttpPort = 80;
//	CString szMd5 = "";
	CString strSentence, strGetSentence = "";
	CString szSql = "";
	CString szData = "";
	CSqlHelper m_SqlHelper;
	vector<CGlobalStruct::DataRecord> datarecord;
//	CMD5 md5_OOp;

	GetCurrentDirectory(100,_totalpath);
	strcat(_totalpath,"\\Config\\config.ini");
	_temppath = _totalpath;

	//获取游戏IP
	char WebServerIp[20]= "";
	char Url[128]="";
	GetPrivateProfileString("server","WebServer","61.152.150.158",WebServerIp,20,_temppath.c_str());//华东一区
	GetPrivateProfileString("url","Url","passport/games/gmtools_ban_log_w.php",Url,128,_temppath.c_str());//WA接收地址

	delete []_totalpath;
	
	szSql.Format("exec WA_GetIdx '%s', %d",ServerIP, gameID);
	
	m_SqlHelper.SqlHelperMain("WA", &datarecord,"127.0.0.1",szSql.GetBuffer(0),4,0,0);
	
	if(datarecord.empty())
		return "";
	
	vector<CGlobalStruct::DataRecord>::iterator iter;
	CGlobalStruct::DataRecord  m_DataRecord;
	for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
	{
		m_DataRecord = *iter;
		if(!strcmp("idx", m_DataRecord.recordfieldname))//获取相应的ServerIP
		{
			szData.Format("%s", m_DataRecord.recorddata);//保存查询得到的最后一条记录
			break;
		}
	}

	szTemUrl.Format("http://%s/%s",WebServerIp,Url);
	lpConnetction = MySession.GetHttpConnection(WebServerIp, HttpPort);
	
	lpHttpFile = lpConnetction->OpenRequest(CHttpConnection::HTTP_VERB_POST, 
		Url,
		szTemUrl.GetBuffer(0),1, NULL, NULL, 
		INTERNET_FLAG_NO_AUTO_REDIRECT);
	
	if(NULL != lpHttpFile)
	{
		string  strHeader = "Content-Type:application/x-www-form-urlencoded";
		
// 		//ascii转换成unicode
// 		int  wcsLen  =  ::MultiByteToWideChar(CP_ACP, NULL, you9Nickname.c_str(), you9Nickname.length(), NULL,  0 );
// 		wchar_t *  wszString  =   new  wchar_t[wcsLen  +   1 ];
// 		::MultiByteToWideChar(CP_ACP, NULL, you9Nickname.c_str(), you9Nickname.length(), wszString, wcsLen);
// 		wszString[wcsLen]  = '\0' ;
// 		
// 		//unicode转换成utf8
// 		int u8Len = ::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);
// 		char* szU8 = new char[u8Len + 1];
// 		::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), szU8, u8Len, NULL, NULL);
// 		szU8[u8Len] = '\0';
		
		int length=0;
		
		string strMd5 = NewUserName;
		strMd5 += "|T4pb5A.gmtoolbanlog";
		//md5加密
// 		char *szMd5 = new char[strlen(strMd5.c_str())+1];
// 		ZeroMemory(szMd5, strlen(strMd5.c_str())+1);
//		MD5_EnCode(szMd5,&length,(char*)strMd5.c_str(),strMd5.length());

		string requestString = "username=";
			   requestString += NewUserName;
			   requestString += "&gmareaid=";
			   requestString += szData.GetBuffer(0);
			   requestString += "&ban=";
			   requestString += BanType;
			   requestString += "&banhour=";
			   requestString += NewTime;
			   requestString += "&sign=";
			   requestString += md5_OOp.MD5Encode((char*)strMd5.c_str()).c_str();
		
		printf("the parameters of request: %s\n",requestString.c_str());

		if(lpHttpFile->SendRequest(strHeader.c_str(),
			(DWORD)strHeader.length(),
			(void*)requestString.c_str(),
			(DWORD)requestString.length()))
			printf("send request success!\n");
		
		while(lpHttpFile->ReadString(strSentence))    // 读取提交数据后的返回结果
		{
			strGetSentence = strGetSentence + strSentence;
		}
		
		printf("返回的数据:%s\n",strGetSentence);
		
	}

	return strGetSentence;
}

string CGameInfo::md5_PassWord(char *PassWord)
{
	if(PassWord == NULL)
		return "";

	return md5_OOp.MD5Encode(PassWord);
}