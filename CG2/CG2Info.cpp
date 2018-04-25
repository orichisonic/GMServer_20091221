#include "stdafx.h"
#include "time.h"
#include "CG2Info.h"
#include "md5.h"
#pragma comment(lib,"md5.lib")


//构造函数
CCG2Info::CCG2Info()
{
	//ZeroMemory(m_UserName,128);//初始化执行操作的用户名
	//ZeroMemory(strLog,2048);//初始化日志信息
	//ZeroMemory(operRemoteSql, 2048);//初始化在远程数据库执行操作的sql语句
	//index = 0;//初始化页索引为0
	//iPageSize = 0;//初始化页面大小为0
	//ZeroMemory(logdbip, 20);//初始化服务器IP
}

//析构函数
CCG2Info::~CCG2Info()
{
	Destroy();
}

//初始化类中的变量
bool CCG2Info::initialize(CSession * pSess,SOCKET m_Socket, unsigned char * pbuf, int length,CEnumCore::Message_Tag_ID m_message_tag_id)
{
	try
	{
		if(length>0)
		{
			recvVect.Create(length/8);
			if(recvVect.DeBody((unsigned char *)pbuf,length) == 0)//解析客户端发送过来的数据
			{
				return false;
			}
		}
		index = 0;//初始化页索引为0
		iPageSize = 0;//初始化页面大小为0
		if(recvVect.getTLVByTag(CEnumCore::TagName::Index,0).nIndex != 0)
		{
			index=StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::Index,0)).lpdata);//获取页索引
		}
		if(recvVect.getTLVByTag(CEnumCore::TagName::PageSize,0).nIndex != 0)
		{
			iPageSize=StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata);//获取页面大小
		}
	
		ZeroMemory(cg2_serverip,40);
		ZeroMemory(cg2_username,40);
		ZeroMemory(cg2_nickname,40);

		if(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Serverip,0).nIndex != 0)
		{
			sprintf(cg2_serverip,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Serverip,0)).lpdata);//获取服务器IP
		}
		if(m_message_tag_id!=CEnumCore::Message_Tag_ID::CG2_InsertBankItemALL_Query)
		{
			if(recvVect.getTLVByTag(CEnumCore::TagName::CG2_UserName,0).nIndex != 0)
			{
				sprintf(cg2_username,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_UserName,0)).lpdata);//获取用户名
			}
			if(recvVect.getTLVByTag(CEnumCore::TagName::CG2_nickname,0).nIndex != 0)
			{
				sprintf(cg2_nickname,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_nickname,0)).lpdata);//获取昵称
			}
		}

		
		if(recvVect.getTLVByTag(CEnumCore::TagName::CG2_UserID,0).nIndex != 0)
		{
			sprintf(cg2_usersn,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_UserID,0)).lpdata);//获取玩家ID
		}
		if(recvVect.getTLVByTag(CEnumCore::TagName::CG2_CharNo,0).nIndex != 0)
		{
			cg2_charno=StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_CharNo,0)).lpdata);//获取用户角色号
		}
		if(recvVect.getTLVByTag(CEnumCore::TagName::CG2_GuildName,0).nIndex != 0)
		{
			sprintf(cg2_guildname,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_GuildName,0)).lpdata);//公会名称
		}
		if(recvVect.getTLVByTag(CEnumCore::TagName::CG2_GuildID,0).nIndex != 0)
		{
			cg2_guildid=StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_GuildID,0)).lpdata);//公会ID
		}
		ZeroMemory(strLog,2048);//初始化日志信息		
		ZeroMemory(operRemoteSql, 2048);//初始化在远程数据库执行操作的sql语句
		
		ZeroMemory(m_UserName,128);//初始化执行操作的用户名
		CLogic::SocketToUserName(m_Socket, m_UserName);//保存执行操作的用户名
		
		ZeroMemory(logdbip, 20);//初始化服务器IP
		char path[256];
		GetCurrentDirectory(256,path);
		char schemefilepath[256];
		sprintf(schemefilepath,"%s%s",path,"\\config\\scheme.ini");//构造配置文件的路径
		GetPrivateProfileString("DATABASE","Address","127.0.0.1",logdbip,20,schemefilepath);//获取本地的数据库地址
		sprintf(filepath,"%s%s",path,"\\config\\M_CG2.INI");//配置文件的路径
		
		operAssist.initialize(pSess,m_Socket);
		m_hServerSocket=m_Socket;
		userByID = CLogic::SocketToUserID(m_Socket);//获取操作员ID

		SYSTEMTIME nowtime;
		GetLocalTime(&nowtime);//获取系统当前时间
		sprintf(curTime,"%i-%i-%i %i:%i:%i",nowtime.wYear,nowtime.wMonth,nowtime.wDay,nowtime.wHour,nowtime.wMinute,nowtime.wSecond);//给当前时间赋值

	}
	catch(...)
	{
	}
	return true;
}
//删除类中的变量
bool CCG2Info::Destroy()
{
	try
	{
		recvVect.Destroy();
		DBVect.Destroy();
		DBVect1.Destroy();
		
	}
	catch (...)
	{
		
	}
	return true;
}

//角色基本资料（查询类）
bool CCG2Info::CG2_Account_Query()
{
	try
	{
	
		operAssist.ReadValue("GameName",retMessage[0],256);
		operAssist.ReadValue("CG2_Account_Query",retMessage[1],256);
		logFile.WriteLog(retMessage[0],m_UserName,retMessage[1]);//将操作类型写入日志文件


		int dbid=0;
		if(strcmp("", cg2_username))//如果玩家帐号不为空
		{

			operAssist.getRemoteSql("CG2_ACCOUNT_QUERYBYACCOUNT",operRemoteSql);//构造根据IP查询登录记录的sql语句
			if(!strcmp("",operRemoteSql))
			{
				logFile.ReadValue("M_Global","Global","DatabaseWithoutSql",retMessage[0],256);
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Query_Resp, retMessage[0]);
				return false;
			}
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteLogFile(cg2_serverip);
			dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
			sprintf(operszSql,operRemoteSql,cg2_username);
			logFile.WriteLogFile(operszSql);
			int tmpLength = 0;
			if(dbid==1)//玩家帐号在第一个数据库中
			{
			
				tmpLength =DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//构造从数据库查询得到的数据
				sprintf(operszSql,"长度%i",tmpLength);
				logFile.WriteLogFile(operszSql);
				operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Query_Resp, &DBVect, tmpLength);

			}
			else if(dbid==2)
			{
				tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//构造从数据库查询得到的数据
				sprintf(operszSql,"长度%i",tmpLength);
				logFile.WriteLogFile(operszSql);
				operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Query_Resp, &DBVect, tmpLength);
			}
			else
			{
				operAssist.ReadValue("No_Account_Exist",retMessage[0],256);
				operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Query_Resp, retMessage[0]);
			}

		}
		else
		{
			operAssist.getRemoteSql("CG2_ACCOUNT_QUERYBYUSERNICK",operRemoteSql);//获取利用昵称查询的sql语句
			if(!strcmp("",operRemoteSql))
			{
				logFile.ReadValue("M_Global","Global","DatabaseWithoutSql",retMessage[0],256);
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Query_Resp, retMessage[0]);
				return false;
			}
			sprintf(operszSql,operRemoteSql,"\%",cg2_nickname,"\%");//格式化sql语句，注意转义符
			logFile.WriteLogFile(operszSql);
			DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, 0, 0);//构造从数据库查询得到的数据
			DBVect1.OperVectorMain("CG2",cg2_serverip, operszSql, 5, 0, 0);//构造从数据库查询得到的数据
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Query_Resp, &DBVect, &DBVect1,index,iPageSize);


		}

	}
	catch (...)
	{
		logFile.WriteLogFile("CG2_Account_Query Error");
	}
	return true;
}
//角色详细资料
bool CCG2Info::CG2_Account_Detail_Query()
{
	try
	{
		
		operAssist.ReadValue("GameName",retMessage[0],256);
		operAssist.ReadValue("CG2_Account_Detail_Query",retMessage[1],256);
		logFile.WriteLog(retMessage[0],m_UserName,retMessage[1]);//将操作类型写入日志文件
		
		int dbid=0;
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		operAssist.getRemoteSql("CG2_Account_Detail_Query",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			logFile.ReadValue("M_Global","Global","DatabaseWithoutSql",retMessage[0],256);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Detail_Query_Resp,retMessage[0]);
			return false;
		}
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		sprintf(operszSql,operRemoteSql,cg2_username,cg2_usersn);
		if(dbid==1)//玩家帐号在第一个数据库中
		{
		
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//构造从数据库查询得到的数据
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Detail_Query_Resp, &DBVect, tmpLength);
			
		}
		else if(dbid==2)
		{
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//构造从数据库查询得到的数据
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Detail_Query_Resp, &DBVect, tmpLength);
		}


		
	}
	catch (...)
	{
		logFile.WriteLogFile("CG2_Account_Detail_Query Error");
	}
	return true;
}

//角色属性资料
bool CCG2Info::CG2_Account_Attribute_Query()
{
	try
	{
		operAssist.ReadValue("GameName",retMessage[0],256);
		operAssist.ReadValue("CG2_Account_Attribute_Query",retMessage[1],256);
		logFile.WriteLog(retMessage[0],m_UserName,retMessage[1]);//将操作类型写入日志文件

		int dbid=0;
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		operAssist.getRemoteSql("CG2_Account_Attribute_Query",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{

			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Attribute_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		sprintf(operszSql,operRemoteSql,cg2_username,cg2_usersn);
		if(dbid==1)//玩家帐号在第一个数据库中
		{
		
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//构造从数据库查询得到的数据
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Attribute_Query_Resp, &DBVect, tmpLength);
			
		}
		else if(dbid==2)
		{
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//构造从数据库查询得到的数据
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Attribute_Query_Resp, &DBVect, tmpLength);
		}
	}
	catch (...)
	{
		logFile.WriteLogFile("CG2_Account_Attribute_Query Error");
	}
	return true;
}
//角色公会资料
bool CCG2Info::CG2_Account_Guild_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"帐号<%s>查询大区<%s>玩家<%s>角色公会资料",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("魔力宝贝2",strLog);
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		operAssist.getRemoteSql("CG2_Account_Guild_Query",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Guild_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		sprintf(operszSql,operRemoteSql,cg2_username,cg2_usersn);
		if(dbid==1)//玩家帐号在第一个数据库中
		{

			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//构造从数据库查询得到的数据
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Guild_Query_Resp, &DBVect, tmpLength);
			
		}
		else if(dbid==2)
		{
			
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//构造从数据库查询得到的数据
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Guild_Query_Resp, &DBVect, tmpLength);
		}
	}
	catch (...)
	{
		
	}
	return true;
}
//角色称号资料(比较特殊直接查数据库)
bool CCG2Info::CG2_Account_Title_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"帐号<%s>查询大区<%s>玩家<%s>角色称号资料",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("魔力宝贝2",strLog);
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		operAssist.getRemoteSql("CG2_Account_Title_Query",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Title_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		sprintf(operszSql,operRemoteSql,cg2_username,cg2_usersn);
		
			
			CSqlHelper m_SqlHelper;
			vector<CGlobalStruct::DataRecord> datarecord;
			
			if(!datarecord.empty())//清空队列
			{
				datarecord.clear();
			}
			if(dbid==1)//玩家帐号在第一个数据库中
			{
				m_SqlHelper.SqlHelperMain("CG2",&datarecord, cg2_serverip, operszSql, 3,index,iPageSize);//执行查询操作并得到结果
			}
			else if(dbid==2)
			{
				m_SqlHelper.SqlHelperMain("CG2",&datarecord, cg2_serverip, operszSql, 5,index,iPageSize);//执行查询操作并得到结果
			}
			DBVect.ClearInnerList();
					
			DBVect.InitCreate(datarecord.size()/m_SqlHelper.GetFieldNum()+1,m_SqlHelper.GetFieldNum()+5);//初始化变量，并分配空间
			
			CEnumCore::TagName m_TagName;//保存获得的TagName
			CEnumCore::TagFormat m_TagFormat;//保存获得的TagFormat
			
			CMyEvent m_Event;//获取CEvent对象
			
			//获取查询的记录集的每一项
			vector<CGlobalStruct::DataRecord>::iterator iter;
			CGlobalStruct::DataRecord  m_DataRecord;

			int i=0;
			char strTitle[15];//保存称号的列名
			int titleIndex = 0;//保存不为空的称号序号

			for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
			{
				m_DataRecord = *iter;
				sprintf(strTitle,"titleFlags%03d",i);
				if(!strcmp(m_DataRecord.recordfieldname,strTitle))
				{
					i++;
					unsigned int title=StrToInt((char *)&m_DataRecord.recorddata);//获取数据库记录中的称号ID

					if(title!=0)
					{
						titleIndex++;
						sprintf(m_DataRecord.recordfieldname,"titleFlags%03d",titleIndex);
						int cnt = 0;
						while(true)
						{
							if(title ==1)
							{
								break;
							}
							title = title >> 1;
							cnt++;
						}
						int titleID = i*32+cnt;//获取真正的称号ID
						char procName[256];
						sprintf(procName,"exec CG2_titleName_query %i",titleID);//初始化存储过程的语句
						
						char title_Name[256];
						ZeroMemory(title_Name,256);
						operAssist.CG2_Proc_Query(logdbip,4,procName,title_Name);//获取玩家的称号信息
						
						if(!strcmp(title_Name,""))
						{
							//将从数据库中得到的值保存到CSocketData对象中
							sprintf(m_DataRecord.recorddata,"NULL");
						}
						else
						{
							//将从数据库中得到的值保存到CSocketData对象中
							sprintf(m_DataRecord.recorddata,title_Name);
						}
						m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//获取记录集数据的TagName
						m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//获取记录集数据的TagFormat
						DBVect.AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//构造TFLV,并添加到vector中

					}
					
						
				
					
				}
				else if(!strcmp(m_DataRecord.recordfieldname,"fameTitle"))
				{
					int title=StrToInt((char *)&m_DataRecord.recorddata);
					
					char procName[256];
					sprintf(procName,"exec CG2_FameName_query %i,%i",-1,title);//初始化存储过程的语句
					
					char title_Name[256];
					ZeroMemory(title_Name,256);
					operAssist.CG2_Proc_Query(logdbip,4,procName,title_Name);//获取玩家的临时称号
					
					if(!strcmp(title_Name,""))
					{
						//将从数据库中得到的值保存到CSocketData对象中
						sprintf(m_DataRecord.recorddata,"NULL");
					}
					else
					{
						//将从数据库中得到的值保存到CSocketData对象中
						sprintf(m_DataRecord.recorddata,title_Name);
					}
					m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//获取记录集数据的TagName
					m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//获取记录集数据的TagFormat
					DBVect.AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//构造TFLV,并添加到vector中
					
			
				}
				else if(!strcmp(m_DataRecord.recordfieldname,"tmpTitle"))
				{
					int title=StrToInt((char *)&m_DataRecord.recorddata);
					
					char procName[256];
					sprintf(procName,"exec CG2_TmptitleName_query %i",title);//初始化存储过程的语句
					
					char title_Name[256];
					ZeroMemory(title_Name,256);
					operAssist.CG2_Proc_Query(logdbip,4,procName,title_Name);//获取玩家的临时称号
					
					if(!strcmp(title_Name,""))
					{
						//将从数据库中得到的值保存到CSocketData对象中
						sprintf(m_DataRecord.recorddata,"NULL");
					}
					else
					{
						//将从数据库中得到的值保存到CSocketData对象中
						sprintf(m_DataRecord.recorddata,title_Name);
					}
					m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//获取记录集数据的TagName
					m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//获取记录集数据的TagFormat
					DBVect.AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//构造TFLV,并添加到vector中

				}
				else if(!strcmp(m_DataRecord.recordfieldname,"counter00")||
						!strcmp(m_DataRecord.recordfieldname,"reserve00")||
						!strcmp(m_DataRecord.recordfieldname,"reserve01")||
						!strcmp(m_DataRecord.recordfieldname,"reserve02")||
						!strcmp(m_DataRecord.recordfieldname,"reserve03")||
						!strcmp(m_DataRecord.recordfieldname,"reserve04")||
						!strcmp(m_DataRecord.recordfieldname,"reserve05")||
						!strcmp(m_DataRecord.recordfieldname,"reserve06")||
						!strcmp(m_DataRecord.recordfieldname,"reserve07")||
						!strcmp(m_DataRecord.recordfieldname,"charNo")||
						!strcmp(m_DataRecord.recordfieldname,"signalMessage"))
				{

				}
				else
				{
					m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//获取记录集数据的TagName
					m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//获取记录集数据的TagFormat

					DBVect.AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//构造TFLV,并添加到vector中

				}


			}
			
			if(!datarecord.empty())//清空队列
			{
				datarecord.clear();
			}
			int tmpLength = DBVect.EnBody();//解析数据包，得到要发送的数组
			operAssist.ReturnSpecData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Title_Query_Resp, &DBVect, tmpLength);

	}
	catch (...)
	{
		
	}
	return true;
}
//角色社会关系
bool CCG2Info::CG2_Account_Address_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"帐号<%s>查询大区<%s>玩家<%s>角色社会关系",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("魔力宝贝2",strLog);
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		operAssist.getRemoteSql("CG2_Account_Address_Query",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Address_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		sprintf(operszSql,operRemoteSql,cg2_username,cg2_usersn);
		if(dbid==1)//玩家帐号在第一个数据库中
		{
			
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//构造从数据库查询得到的数据
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Address_Query_Resp, &DBVect, tmpLength);
			
		}
		else if(dbid==2)
		{
			
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//构造从数据库查询得到的数据
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Address_Query_Resp, &DBVect, tmpLength);
		}
		
	}
	catch (...)
	{
		
	}
	return true;
}
//角色技能資訊
bool CCG2Info::CG2_Account_Skill_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"帐号<%s>查询大区<%s>玩家<%s>角色技能資訊",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("魔力宝贝2",strLog);
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		operAssist.getRemoteSql("CG2_Account_Skill_Query",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Skill_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		sprintf(operszSql,operRemoteSql,cg2_username,cg2_usersn);
		if(dbid==1)//玩家帐号在第一个数据库中
		{
			
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//构造从数据库查询得到的数据
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Skill_Query_Resp, &DBVect, tmpLength);
			
		}
		else if(dbid==2)
		{
			
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//构造从数据库查询得到的数据
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Skill_Query_Resp, &DBVect, tmpLength);
		}
	}
	catch (...)
	{
		
	}
	return true;
}
//师生关系查询
bool CCG2Info::CG2_Account_Teach_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"帐号<%s>查询大区<%s>玩家<%s>师生关系查询",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("魔力宝贝2",strLog);
		
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);


		int type = StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_TypeID,0)).lpdata);//获取要查询的类型
		if(type == 0)//类型为0，按照学生来查询
		{
			operAssist.getRemoteSql("CG2_Account_Teach_Query",operRemoteSql);//获取查询角色师生关系的sql语句
		}
		else//类型为1，按照老师来查询
		{
			operAssist.getRemoteSql("CG2_Account_Student_Query",operRemoteSql);//获取查询角色师生关系的sql语句
		}
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Teach_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		sprintf(operszSql,operRemoteSql,cg2_username,cg2_usersn);
		if(dbid==1)//玩家帐号在第一个数据库中
		{
			
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//构造从数据库查询得到的数据
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Teach_Query_Resp, &DBVect, tmpLength);
			
		}
		else if(dbid==2)
		{
			
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//构造从数据库查询得到的数据
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Teach_Query_Resp, &DBVect, tmpLength);
		}
	}
	catch (...)
	{
		
	}
	return true;
}

//日志查询
bool CCG2Info::CG2_Log_Query()
{
		
	try
	{
		logFile.ReadValue("M_CG2","CG2","GameName",retMessage[0],256);
		logFile.ReadValue("M_CG2","CG2","CG2_Log_Query",retMessage[1],256);
		logFile.WriteLog(retMessage[0],m_UserName,retMessage[1]);//将操作类型写入日志文件

		char Category[20];//日志大类
		char Action[20];//日志小类
		char StartTime[25],EndTime[25];//日志查询的开始时间和结束时间
		ZeroMemory(Category,20);
		ZeroMemory(Action,20);
		ZeroMemory(StartTime,25);
		ZeroMemory(EndTime,25);
		sprintf(Category,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_category,0)).lpdata);//日志大类
		sprintf(Action,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_action,0)).lpdata);//日志小类
		sprintf(StartTime,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Log_StartTime,0)).lpdata);//获取日志查询的开始时间
		sprintf(EndTime,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Log_EndTime,0)).lpdata);//获取日志查询的结束时间

		operAssist.getRemoteSql("CG2_LOG_QUERY",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Log_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,3);
		sprintf(operszSql,operRemoteSql,Category,Category,Action,cg2_username,cg2_usersn,StartTime,EndTime);//格式化sql语句
		int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 7, index, iPageSize);//构造从数据库查询得到的数据
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Log_Query_Resp, &DBVect, tmpLength);
	}
	catch (...)
	{
		
	}
	return true;
}
//查询职业种类
bool CCG2Info::CG2_Job_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_Job_Query");//将操作类型写入日志文件
	
		operAssist.getRemoteSql("CG2_Job_Query",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Job_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		sprintf(operszSql,operRemoteSql);
		int tmpLength = DBVect.OperVectorMain(logdbip, operszSql, 2, 0, 0);//构造从数据库查询得到的数据
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Job_Query_Resp, &DBVect, tmpLength);
	}
	catch (...)
	{
		
	}
	return true;
}
//查询技能类别
bool CCG2Info::CG2_GetSkill_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_GetSkill_Query");//将操作类型写入日志文件
		
		sprintf(operszSql,"%s","exec CG2_GetSkill_Query");
		int tmpLength = DBVect.OperVectorMain(logdbip, operszSql, 4, 0, 0);//构造从数据库查询得到的数据
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_GetSkill_Query_Resp, &DBVect, tmpLength);
	}
	catch (...)
	{
		
	}
	return true;
}
//查询技能名称
bool CCG2Info::CG2_SkillList_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_SkillList_Query");//将操作类型写入日志文件
		
		char SkillType[256];
		ZeroMemory(SkillType,256);
		sprintf(SkillType,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SkillType,0)).lpdata);//技能类别

		sprintf(operszSql,"exec CG2_SkillList_Query '%s'",SkillType);
		int tmpLength = DBVect.OperVectorMain(logdbip, operszSql, 4, 0, 0);//构造从数据库查询得到的数据
		operAssist.ReturnData(CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_SkillList_Query_Resp, DBVect.getDes(),tmpLength);
	}
	catch (...)
	{
		
	}
	return true;

}
//模糊查询道具
bool CCG2Info::CG2_SearchItem_Query()
{
	try
	{
		operAssist.ReadValue("GameName",retMessage[0],256);
		operAssist.ReadValue("CG2_SearchItem_Query",retMessage[1],256);
		logFile.WriteLog(retMessage[0],m_UserName,retMessage[1]);//将操作类型写入日志文件
		int iType=0;
		int tmpLength=0;
		char itemname[40];
		iType=StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_TypeID,0).lpdata));
		sprintf(itemname,"%s",(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_TypeName,0).lpdata));
		tmpLength=sprintf(operszSql,"exec CG2_SearchItem_Query %i,%s",iType,itemname);//0:非银行1://银行
		DBVect.OperVectorMain(logdbip,operszSql,4,0,0);
		operAssist.ReturnData(CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_SearchItem_Query_Resp,DBVect.getDes(),tmpLength);
	}
	catch (...)
	{
		
	}
	return true;

}
//查询道具类别
bool CCG2Info::CG2_Get_Item_Query()
{
	try
	{
		operAssist.ReadValue("GameName",retMessage[0],256);
		operAssist.ReadValue("CG2_Get_Item_Query",retMessage[1],256);
		logFile.WriteLog(retMessage[0],m_UserName,retMessage[1]);//将操作类型写入日志文件
		int iType=0;
		int tmpLength=0;
		iType=StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_TypeID,0).lpdata));
		tmpLength=sprintf(operszSql,"exec CG2_Get_Item_Query %i",iType);//0:非银行1://银行
		tmpLength = DBVect.OperVectorMain(logdbip, operszSql, 4, 0, 0);//构造从数据库查询得到的数据
		operAssist.ReturnData(CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Get_Item_Query_Resp,DBVect.getDes(),tmpLength);
	}
	catch (...)
	{
		
	}
	return true;
}
//道具名称查询
bool CCG2Info::CG2_Get_Item_Detail_Query()
{
	try
	{
		operAssist.ReadValue("GameName",retMessage[0],256);
		operAssist.ReadValue("CG2_Get_Item_Detail_Query",retMessage[1],256);
		logFile.WriteLog(retMessage[0],m_UserName,retMessage[1]);//将操作类型写入日志文件
		
		char ItemType[40];
		ZeroMemory(ItemType,40);
		sprintf(ItemType,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemType,0)).lpdata);//技能类别

		////运行存储过程获得item ID
		char procName[256];
		ZeroMemory(procName,256);
		sprintf(procName,"exec CG2_ItemID_Query '%s'",ItemType);
		
		
		char strID[10];
		ZeroMemory(strID,10);
		operAssist.CG2_Proc_Query(logdbip,4,procName,strID);////根据道具类别获得道具ID

		operAssist.getRemoteSql("CG2_ItemName_Query",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Get_Item_Detail_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		sprintf(operszSql,operRemoteSql,strID);
		int tmpLength = DBVect.OperVectorMain(logdbip, operszSql, 4, 0, 0);//构造从数据库查询得到的数据
		operAssist.ReturnData(CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Get_Item_Detail_Query_Resp,DBVect.getDes(),tmpLength);
	}
	catch (...)
	{
		
	}
	return true;
}
//添加道具
bool CCG2Info::CG2_Insert_Item_Query()
{
	try
	{
		operAssist.ReadValue("GameName",retMessage[0],256);
		operAssist.ReadValue("CG2_Insert_Item_Query",retMessage[1],256);
		logFile.WriteLog(retMessage[0],m_UserName,retMessage[1]);//将操作类型写入日志文件
		
		int result=0;

		char ItemName[256];
		ZeroMemory(ItemName,256);
		char ItemNumber[256];
		ZeroMemory(ItemNumber,256);
		sprintf(ItemName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemName,0)).lpdata);//获得道具名称
		sprintf(ItemNumber,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemNum,0)).lpdata);//获得道具数量
		////运行存储过程获得item ID
		char procName[256];
		ZeroMemory(procName,256);
		sprintf(procName,"exec CG2_ItemID_Query_itemname '%s'",ItemName);

		char strID[10];
		ZeroMemory(strID,10);
		operAssist.CG2_Proc_Query(logdbip,4,procName,strID);////根据道具名称获得道具ID

		char remark[256];
		ZeroMemory(remark,256);
		sprintf(remark,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Remark,0)).lpdata);//备注///present item

		operAssist.getRemoteSql("CG2_AddItem_Query",operRemoteSql);//获取添加道具sql语句
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,strID,ItemNumber,remark,cg2_usersn);//格式化sql语句

		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		int dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		int tmpLength=0;
		if(dbid==1)//如果玩家在第一个数据库中
		{
			//构造对第一个数据库进行操作的sql语句
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		}
		else if(dbid==2)//如果玩家在第二个数据库中
		{
			//构造对第二个数据库进行操作的sql语句
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,11);
		}

		if(tmpLength>0)//操作成功
		{
			operAssist.getRemoteSql("CG2_UpdateChar_Query",operRemoteSql);//获取插入到数据库中的sql语句
			sprintf(operszSql,operRemoteSql,cg2_username,cg2_charno,cg2_usersn,remark,0,cg2_serverip,result);//格式化sql语句
			tmpLength=DBVect.OperVectorMain(logdbip,operszSql,0);
							
			if(tmpLength>0)
			{
					//maple add///重新再连接一次数据库
					Sleep(30000);
					
					operAssist.getRemoteSql("CG2_Return_Result",operRemoteSql);//获取返回结果sql语句
					sprintf(operszSql,operRemoteSql,curTime,cg2_username,strID,ItemNumber);//格式化sql语句，当前时间，玩家帐号，参数
					char gm_result[2];
					ZeroMemory(gm_result,2);
					operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
					operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,gm_result);
					if(!strcmp(gm_result,"2"))
					{
						GetPrivateProfileString("CG2","Add_PresentItem_Fail",NULL,RetMessage,30,filepath);//保存单个返回信息
						operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Item_Query_Resp,RetMessage);//返回信息
						char LogContent[128];
						ZeroMemory(LogContent,128);
						//写入日志文件
						sprintf(LogContent,"%s,In Game Concerto Gate,%s Add PresentItem %s Fail",curTime,m_UserName,cg2_username);
						logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query_Query",cg2_serverip,LogContent);
					}
					else
					{
						GetPrivateProfileString("CG2","Add_PresentItem_Success",NULL,RetMessage,30,filepath);//保存单个返回信息
						operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Item_Query_Resp,RetMessage);//返回信息
						char LogContent[128];
						ZeroMemory(LogContent,128);
						//写入日志文件
						sprintf(LogContent,"%s,In Game Concerto Gate,%s Add PresentItem %s Success",curTime,m_UserName,cg2_username);
						logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query",cg2_serverip,LogContent);
					}
				}

			}
			else
			{
				GetPrivateProfileString("CG2","Add_PresentItem_Fail",NULL,RetMessage,30,filepath);//保存单个返回信息
				operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Item_Query_Resp,RetMessage);//返回信息
				char LogContent[128];
				ZeroMemory(LogContent,128);
				//写入日志文件
				sprintf(LogContent,"%s,In Game Concerto Gate,%s Add Present Item %s Fail",curTime,m_UserName,cg2_username);
				logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query",cg2_serverip,LogContent);
			}
		}
		catch(...)
		{
				operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Item_Query_Resp,"ERROR");//返回修改职业错误信息
				char LogContent[128];
				ZeroMemory(LogContent,128);
				//写入日志文件
				sprintf(LogContent,"%s,In Game Concerto Gate,%s Add PresentItem %s Fail",curTime,m_UserName,cg2_serverip);
				logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query",cg2_serverip,LogContent);
		}
		return true;
}
//添加银行道具
bool CCG2Info::CG2_InsertBankItem_Query()
{

	try
	{
		operAssist.ReadValue("GameName",retMessage[0],256);
		operAssist.ReadValue("CG2_InsertBankItem_Query",retMessage[1],256);
		logFile.WriteLog(retMessage[0],m_UserName,retMessage[1]);//将操作类型写入日志文件
		
		int result=0;

		char ItemName[256];
		char ItemNumber[256];
		char pame[256];
		ZeroMemory(pame,256);
		ZeroMemory(ItemName,256);
		ZeroMemory(ItemNumber,256);
		sprintf(ItemName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemName,0)).lpdata);//获得道具名称
		sprintf(ItemNumber,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemNum,0)).lpdata);//获得道具数量
		char procName[256];
		ZeroMemory(procName,256);
		sprintf(procName,"exec CG2_ItemID_Query_itemname '%s'",ItemName);
		char strID[10];
		ZeroMemory(strID,10);
		operAssist.CG2_Proc_Query(logdbip,4,procName,strID);////根据道具名称获得道具ID
	
		sprintf(pame,"%s %s",strID,ItemNumber);


	/*	char *lpItemName=strtok((char*)&ItemName,"|");
		char lpItemNum[10];
		while(lpItemName!=NULL)
		{
			//////////////////////////////////////////////////////////////////////////
			char procName[256];
			ZeroMemory(procName,256);
			sprintf(procName,"exec CG2_ItemID_Query_itemname '%s'",lpItemName);
			char strID[10];
			ZeroMemory(strID,10);
			operAssist.CG2_Proc_Query(logdbip,0,procName,strID);////根据道具名称获得道具ID
			for(int i=0;i<strlen(ItemNumber);i++)
			{
				if(ItemNumber[i]=='|')//查询标志","
				{
					memcpy(lpItemNum,ItemNumber,i);//得到文件名
					lpItemNum[i]='\0';
					memcpy(ItemNumber,ItemNumber+i+1,strlen(ItemNumber)-i-1);
					break;
				}
			}
			sprintf(pame,"%s%s %s ",pame,strID,lpItemNum);
			lpItemName=strtok(NULL,"|");//查询标志"|"
		}*/

		
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);//获取添加道具sql语句
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,19,pame,0,"",cg2_usersn);//格式化sql语句
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		int dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		int tmpLength=0;
		if(dbid==1)//如果玩家在第一个数据库中
		{
			//构造对第一个数据库进行操作的sql语句
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		}
		else if(dbid==2)//如果玩家在第二个数据库中
		{
			//构造对第二个数据库进行操作的sql语句
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,11);
		}

		if(tmpLength>0)//操作成功
		{
				//maple add///重新再连接一次数据库
 				Sleep(30000);
					
				operAssist.getRemoteSql("CG2_getResult",operRemoteSql);//获取返回结果sql语句
				sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,19,pame,cg2_usersn);//格式化sql语句
				char gm_result[2];
				ZeroMemory(gm_result,2);
				operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
				operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,gm_result);
				if(!strcmp(gm_result,"2"))
				{
					GetPrivateProfileString("CG2","Add_PresentItem_Fail",NULL,RetMessage,30,filepath);//保存单个返回信息
					operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_InsertBankItem_Query_Resp,RetMessage);//返回信息
					char LogContent[128];
					ZeroMemory(LogContent,128);
					//写入日志文件
					sprintf(LogContent,"%s,In Game Concerto Gate,%s Add PresentItem %s Fail",curTime,m_UserName,cg2_username);
					logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query_Query",cg2_serverip,LogContent);
				}
				else
				{
					GetPrivateProfileString("CG2","Add_PresentItem_Success",NULL,RetMessage,30,filepath);//保存单个返回信息
					operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_InsertBankItem_Query_Resp,RetMessage);//返回信息
					char LogContent[128];
					ZeroMemory(LogContent,128);
					//写入日志文件
					sprintf(LogContent,"%s,In Game Concerto Gate,%s Add PresentItem %s Success",curTime,m_UserName,cg2_username);
					logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query",cg2_serverip,LogContent);
				}
		}
		else
		{
			GetPrivateProfileString("CG2","Add_PresentItem_Fail",NULL,RetMessage,30,filepath);//保存单个返回信息
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Item_Query_Resp,RetMessage);//返回信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//写入日志文件
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Add Present Item %s Fail",curTime,m_UserName,cg2_username);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query",cg2_serverip,LogContent);
		}
	}
	catch(...)
	{
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Item_Query_Resp,"ERROR");//返回修改职业错误信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//写入日志文件
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Add PresentItem %s Fail",curTime,m_UserName,cg2_serverip);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query",cg2_serverip,LogContent);
	}
	return true;
}
//批量银行道具添加（导入）
bool CCG2Info::CG2_InsertBankItemALL_Query()
{

	try
	{
		operAssist.ReadValue("GameName",retMessage[0],256);
		operAssist.ReadValue("CG2_InsertBankItem_Query",retMessage[1],256);
		logFile.WriteLog(retMessage[0],m_UserName,retMessage[1]);//将操作类型写入日志文件
		
		int result=0;

		char ItemName[256];
		char ItemNumber[256];
		char pame[256];
		ZeroMemory(pame,256);
		ZeroMemory(ItemName,256);
		ZeroMemory(ItemNumber,256);
		sprintf(ItemName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemName,0)).lpdata);//获得道具名称
		sprintf(ItemNumber,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemNum,0)).lpdata);//获得道具数量
		char procName[256];
		ZeroMemory(procName,256);
		sprintf(procName,"exec CG2_ItemID_Query_itemname '%s'",ItemName);
		char strID[10];
		ZeroMemory(strID,10);
		operAssist.CG2_Proc_Query(logdbip,4,procName,strID);////根据道具名称获得道具ID
	
		sprintf(pame,"%s %s",strID,ItemNumber);


	/*	char *lpItemName=strtok((char*)&ItemName,"|");
		char lpItemNum[10];
		while(lpItemName!=NULL)
		{
			//////////////////////////////////////////////////////////////////////////
			char procName[256];
			ZeroMemory(procName,256);
			sprintf(procName,"exec CG2_ItemID_Query_itemname '%s'",lpItemName);
			char strID[10];
			ZeroMemory(strID,10);
			operAssist.CG2_Proc_Query(logdbip,0,procName,strID);////根据道具名称获得道具ID
			for(int i=0;i<strlen(ItemNumber);i++)
			{
				if(ItemNumber[i]=='|')//查询标志","
				{
					memcpy(lpItemNum,ItemNumber,i);//得到文件名
					lpItemNum[i]='\0';
					memcpy(ItemNumber,ItemNumber+i+1,strlen(ItemNumber)-i-1);
					break;
				}
			}
			sprintf(pame,"%s%s %s ",pame,strID,lpItemNum);
			lpItemName=strtok(NULL,"|");//查询标志"|"
		}*/

		
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);//获取添加道具sql语句
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,19,pame,0,"",cg2_usersn);//格式化sql语句
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		int dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		int tmpLength=0;
		if(dbid==1)//如果玩家在第一个数据库中
		{
			//构造对第一个数据库进行操作的sql语句
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		}
		else if(dbid==2)//如果玩家在第二个数据库中
		{
			//构造对第二个数据库进行操作的sql语句
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,11);
		}

		if(tmpLength>0)//操作成功
		{
				//maple add///重新再连接一次数据库
 				Sleep(30000);
					
				operAssist.getRemoteSql("CG2_getResult",operRemoteSql);//获取返回结果sql语句
				sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,19,pame,cg2_usersn);//格式化sql语句
				char gm_result[2];
				ZeroMemory(gm_result,2);
				operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
				operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,gm_result);
				if(!strcmp(gm_result,"2"))
				{
					GetPrivateProfileString("CG2","Add_PresentItem_Fail",NULL,RetMessage,30,filepath);//保存单个返回信息
					operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_InsertBankItemALL_Query_Resp,RetMessage);//返回信息
					char LogContent[128];
					ZeroMemory(LogContent,128);
					//写入日志文件
					sprintf(LogContent,"%s,In Game Concerto Gate,%s Add PresentItem %s Fail",curTime,m_UserName,cg2_username);
					logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query_Query",cg2_serverip,LogContent);
				}
				else
				{
					GetPrivateProfileString("CG2","Add_PresentItem_Success",NULL,RetMessage,30,filepath);//保存单个返回信息
					operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_InsertBankItemALL_Query_Resp,RetMessage);//返回信息
					char LogContent[128];
					ZeroMemory(LogContent,128);
					//写入日志文件
					sprintf(LogContent,"%s,In Game Concerto Gate,%s Add PresentItem %s Success",curTime,m_UserName,cg2_username);
					logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query",cg2_serverip,LogContent);
				}
		}
		else
		{
			GetPrivateProfileString("CG2","Add_PresentItem_Fail",NULL,RetMessage,30,filepath);//保存单个返回信息
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_InsertBankItemALL_Query_Resp,RetMessage);//返回信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//写入日志文件
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Add Present Item %s Fail",curTime,m_UserName,cg2_username);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query",cg2_serverip,LogContent);
		}
	}
	catch(...)
	{
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_InsertBankItemALL_Query_Resp,"ERROR");//返回修改职业错误信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//写入日志文件
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Add PresentItem %s Fail",curTime,m_UserName,cg2_serverip);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query",cg2_serverip,LogContent);
	}
	return true;
}

//修改玩家角色职业
bool CCG2Info::CG2_UpdateJob_Query()
{
	try
	{
			logFile.WriteLog("Concerto Gate",m_UserName,"CG2_UpdateJob_Query");//将操作类型写入日志文件
			int result=0;

			char JobName[50];
			ZeroMemory(JobName,50);
			sprintf(JobName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_JobID,0)).lpdata);//获取用户job ID

			////运行存储过程获得JobID
			char procName[256];
			ZeroMemory(procName,256);
			sprintf(procName,"exec CG2_JobID_Query '%s'",JobName);

			char JobID[10];
			ZeroMemory(JobID,10);
			operAssist.CG2_Proc_Query(logdbip,4,procName,JobID);////根据职业名称获得职业ID


			char remark[256];
			ZeroMemory(remark,256);
			sprintf(remark,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Remark,0)).lpdata);//备注///update job


			operAssist.getRemoteSql("CG2_UpdateJob_Query",operRemoteSql);//获取修改玩家角色职业sql语句
			sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,JobID,remark,cg2_usersn);//格式化sql语句
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			int dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
			int tmpLength=0;
			if(dbid==1)//如果玩家在第一个数据库中
			{
				//构造对第一个数据库进行操作的sql语句
				tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
			}
			else if(dbid==2)//如果玩家在第二个数据库中
			{
				//构造对第二个数据库进行操作的sql语句
				tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,11);
			}

			if(tmpLength>0)//操作成功
			{							
				operAssist.getRemoteSql("CG2_UpdateChar_Query",operRemoteSql);//获取插入到数据库中的sql语句
				sprintf(operszSql,operRemoteSql,cg2_username,cg2_charno,cg2_usersn,remark,0,cg2_serverip,result);//格式化sql语句

				tmpLength=DBVect.OperVectorMain(logdbip,operszSql,0);
				if(tmpLength>0)//操作成功
				{
					//maple add
					Sleep(30000);
					operAssist.getRemoteSql("CG2_Return_Result_1",operRemoteSql);//获取插入到数据库中的sql语句
					sprintf(operszSql,operRemoteSql,curTime,cg2_username,JobID);//格式化sql语句，当前时间，玩家帐号，参数
					char gm_result[2];
					ZeroMemory(gm_result,2);
					operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
					if(dbid==1)//如果玩家在第一个数据库中
					{
						//构造对第一个数据库进行操作的sql语句
						operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,gm_result);
					}
					else if(dbid==2)//如果玩家在第二个数据库中
					{
						//构造对第二个数据库进行操作的sql语句
						operAssist.CG2_Proc_Query(cg2_serverip,5,operszSql,gm_result);
					}

				

					if(!strcmp(gm_result,"2"))
					{
						GetPrivateProfileString("CG2","Update_CharJob_Fail",NULL,RetMessage,30,filepath);//保存单个返回信息
						operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UpdateJob_Query_Resp,RetMessage);//返回信息
						char LogContent[128];
						ZeroMemory(LogContent,128);
						//写入日志文件
						sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Job %s Fail",curTime,m_UserName,cg2_username);
						logFile.WriteDBLog(userByID,"Concerto Gate","CG2_UpdateJob_Query",cg2_serverip,LogContent);
					}
					else
					{
						GetPrivateProfileString("CG2","Update_CharJob_Success",NULL,RetMessage,30,filepath);//保存单个返回信息
						operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UpdateJob_Query_Resp,RetMessage);//返回信息
						char LogContent[128];
						ZeroMemory(LogContent,128);
						//写入日志文件
						sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Job %s Success",curTime,m_UserName,cg2_username);
						logFile.WriteDBLog(userByID,"Concerto Gate","CG2_UpdateJob_Query",cg2_serverip,LogContent);
					}
				}
			}
			else
			{
				GetPrivateProfileString("CG2","Update_CharJob_Fail",NULL,RetMessage,30,filepath);//保存单个返回信息
				operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UpdateJob_Query_Resp,RetMessage);//返回封停玩家失败的信息
				char LogContent[128];
				ZeroMemory(LogContent,128);
				//写入日志文件
				sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Job %s Fail",curTime,m_UserName,cg2_username);
				logFile.WriteDBLog(userByID,"Concerto Gate","CG2_UpdateJob_Query",cg2_serverip,LogContent);
			}

		}
		catch(...)
		{
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UpdateJob_Query_Resp,"ERROR");//返回修改职业错误信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//写入日志文件
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Job %s Fail",curTime,m_UserName,cg2_username);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_UpdateJob_Query",cg2_serverip,LogContent);
		}
		return true;
}
//修改技能等级
bool CCG2Info::CG2_Update_Account_Skill_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_Update_Account_Skill_Query");//将操作类型写入日志文件
		
		int result=0;

		char UserSkillLevel[50];
		ZeroMemory(UserSkillLevel,50);
		sprintf(UserSkillLevel,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SkillLevel,0)).lpdata);//获取用户技能等级

		char SkillID[20];
		ZeroMemory(SkillID,20);
		sprintf(SkillID,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SkillID,0)).lpdata);//获取用户技能id

		char remark[256];
		ZeroMemory(remark,256);
		sprintf(remark,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Remark,0)).lpdata);///备注////update skill level

		//GetConfigInfo(ServerIP,DBUser,DBUserOper,DBPwd,DBName,DBNameSec,CharSet);//获得配置文件信息

		operAssist.getRemoteSql("CG2_SkillLevel_Update",operRemoteSql);//获取修改玩家角色职业sql语句


		//sprintf(szSql,remotesql,curTime,UserName,charNo,UserSkillLevel,remark,UserSN);//格式化sql语句
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,SkillID,UserSkillLevel,remark,cg2_usersn);//格式化sql语句

		//MakeRemoteSql((char *)&ServerIP,(char*)&DBUser,(char*)&DBPwd,(char*)&DBName, (char*)&CharSet, remoteConnectSql);
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		int dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		int tmpLength=0;
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)//如果玩家在第一个数据库中
		{
			//构造对第一个数据库进行操作的sql语句
			//	MakeRemoteSql((char *)&ServerIP,(char*)&DBUserOper,(char*)&DBPwd,(char*)&DBName, (char*)&CharSet, remoteConnectSql);
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		}
		else if(dbid==2)//如果玩家在第二个数据库中
		{
			//构造对第二个数据库进行操作的sql语句
			//MakeRemoteSql((char *)&ServerIP,(char*)&DBUserOper,(char*)&DBPwd,(char*)&DBNameSec, (char*)&CharSet, remoteConnectSql);
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,11);
		}

		if(tmpLength>0)//操作成功
		{							
			operAssist.getRemoteSql("CG2_UpdateChar_Query",operRemoteSql);//获取插入到数据库中的sql语句
			sprintf(operszSql,operRemoteSql,cg2_username,cg2_charno,cg2_usersn,remark,0,cg2_serverip,result);//格式化sql语句
			tmpLength=DBVect.OperVectorMain(logdbip,operszSql,0);
			if(tmpLength>0)//操作成功
			{
				//maple add
					Sleep(30000);

					operAssist.getRemoteSql("CG2_Return_Result",operRemoteSql);//获取返回结果sql语句
					sprintf(operszSql,operRemoteSql,curTime,cg2_username,SkillID,UserSkillLevel);//格式化sql语句，当前时间，玩家帐号，参数

					char gm_result[2];
					ZeroMemory(gm_result,2);
					operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
					if(dbid==1)//如果玩家在第一个数据库中
					{
						//构造对第一个数据库进行操作的sql语句
						operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,gm_result);
					}
					else if(dbid==2)//如果玩家在第二个数据库中
					{
						//构造对第二个数据库进行操作的sql语句
						operAssist.CG2_Proc_Query(cg2_serverip,5,operszSql,gm_result);
					}
					
							

					if(!strcmp(gm_result,"2"))
					{
						GetPrivateProfileString("CG2","Update_SkillLevel_Fail",NULL,RetMessage,30,filepath);//保存单个返回信息
						operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Update_Account_Skill_Query_Resp,RetMessage);//返回信息
						char LogContent[128];
						ZeroMemory(LogContent,128);
						//写入日志文件
						sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill Level %s Fail",curTime,m_UserName,cg2_username);
						logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
					}
					else
					{
						GetPrivateProfileString("CG2","Update_SkillLevel_Success",NULL,RetMessage,30,filepath);//保存单个返回信息
						operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Update_Account_Skill_Query_Resp,RetMessage);//返回信息
						char LogContent[128];
						ZeroMemory(LogContent,128);
						//写入日志文件
						sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill Level %s Success",curTime,m_UserName,cg2_username);
						logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
					}
				}
			}
			else
			{
				GetPrivateProfileString("CG2","Update_CharJob_Fail",NULL,RetMessage,30,filepath);//保存单个返回信息
				operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Update_Account_Skill_Query_Resp,RetMessage);//返回封停玩家失败的信息
				char LogContent[128];
				ZeroMemory(LogContent,128);
				//写入日志文件
				sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Job %s Fail",curTime,m_UserName,cg2_username);
				logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
			}
		}
		catch(...)
		{
				operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Update_Account_Skill_Query_Resp,"ERROR");//返回修改职业错误信息
				char LogContent[128];
				ZeroMemory(LogContent,128);
				//写入日志文件
				sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill %s Fail",curTime,m_UserName,cg2_username);
				logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
		}
		return true;
}
//添加角色技能
bool CCG2Info::CG2_Insert_Account_Skill_Query()
{
	try
	{
		int tmpLength=0;
		int dbid=0;
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_Insert_Account_Skill_Query");//将操作类型写入日志文件

		int result=0;
		
		char SkillName[50];
		ZeroMemory(SkillName,50);
		sprintf(SkillName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SkillName,0)).lpdata);//获取用户Skill ID
		
		
		////运行存储过程获得SkillID
		char procName[256];
		ZeroMemory(procName,256);
		sprintf(procName,"exec CG2_SkillID_Query '%s'",SkillName);
		
		char SkillID[10];
		ZeroMemory(SkillID,10);
		operAssist.CG2_Proc_Query(logdbip,4,procName,SkillID);////根据技能名称获得技能ID
		
		char remark[256];
		ZeroMemory(remark,256);
		sprintf(remark,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Remark,0)).lpdata);///备注////update skill level

		
		operAssist.getRemoteSql("CG2_AddSkill_Query",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Insert_Account_Skill_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,SkillID,remark,cg2_usersn);//格式化sql语句
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)//玩家帐号在第一个数据库中
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//构造从数据库查询得到的数据
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//构造从数据库查询得到的数据
		}
		if(tmpLength>0)//远程操作成功
		{
			operAssist.getRemoteSql("CG2_UpdateChar_Query",operRemoteSql);//获取插入到数据库中的sql语句
			sprintf(operszSql,operRemoteSql,cg2_username,cg2_charno,cg2_usersn,remark,0,cg2_serverip,result);//格式化sql语句
			tmpLength=DBVect.OperVectorMain(logdbip, operszSql, 0);//本地数据库操作
			if(tmpLength>0)
			{
				Sleep(30000);
				operAssist.getRemoteSql("CG2_Return_Result_1",operRemoteSql);//获取返回结果sql语句
				sprintf(operszSql,operRemoteSql,curTime,cg2_username,SkillID);//格式化sql语句，当前时间，玩家帐号，参数
				char gm_result[2];
				ZeroMemory(gm_result,2);
				operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
				if(dbid==1)//玩家帐号在第一个数据库中
				{
					operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,gm_result);
				}
				else if(dbid==2)
				{
					operAssist.CG2_Proc_Query(cg2_serverip,5,operszSql,gm_result);
				}
			
				if(!strcmp(gm_result,"2"))
				{
					GetPrivateProfileString("CG2","AddSkill_Fail",NULL,RetMessage,30,filepath);//保存单个返回信息
					operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Account_Skill_Query_Resp,RetMessage);//返回信息
					char LogContent[128];
					ZeroMemory(LogContent,128);
					//写入日志文件
					sprintf(LogContent,"%s,In Game Concerto Gate,%s Add Skill %s Fail",curTime,m_UserName,cg2_username);
					logFile.WriteDBLog(userByID,"Concerto Gate","CG2_AddSkill_Query",cg2_serverip,LogContent);
				}
				else
				{
					GetPrivateProfileString("CG2","AddSkill_Success",NULL,RetMessage,30,filepath);//保存单个返回信息
					operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Account_Skill_Query_Resp,RetMessage);//返回信息
					char LogContent[128];
					ZeroMemory(LogContent,128);
					//写入日志文件
					sprintf(LogContent,"%s,In Game Concerto Gate,%s Add Skill %s Success",curTime,m_UserName,cg2_username);
					logFile.WriteDBLog(userByID,"Concerto Gate","CG2_AddSkill_Query",cg2_serverip,LogContent);
				}


			}
		}
		else
		{

		}
	}
	catch (...)
	{
		
	}
	return true;
}
//帐号登陆登出记录
bool CCG2Info::CG2_Use_Query()
{
	try
	{
		int dbid=0;
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_Use_Query");//将操作类型写入日志文件
		char LoginTime[25],LogoutTime[25];
		ZeroMemory(LoginTime,25);
		ZeroMemory(LogoutTime,25);
		sprintf(LoginTime,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_LatestLoginTime,0)).lpdata);//获取登录时间
		sprintf(LogoutTime,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_LatestLogoutTime,0)).lpdata);//获取登出时间
	
		operAssist.getRemoteSql("CG2_UserLogin_Query",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Use_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		sprintf(operszSql,operRemoteSql,cg2_username,LoginTime,LogoutTime);//格式化sql语句

		if(dbid==1)//玩家帐号在第一个数据库中
		{
			
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//构造从数据库查询得到的数据
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Use_Query_Resp, &DBVect, tmpLength);
			
		}
		else if(dbid==2)
		{
			
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//构造从数据库查询得到的数据
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Use_Query_Resp, &DBVect, tmpLength);
		}
	}
	catch (...)
	{
		
	}
	return true;
}

//强制玩家下线
bool CCG2Info::CG2_KickAccount_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"帐号<%s>大区<%s>强制玩家<%s>下线",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("魔力宝贝2",strLog);
		
		operAssist.getRemoteSql("CG2_KickPlayer_Query",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_KickAccount_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
	
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,24,"",0,"",cg2_usersn);//格式化sql语句
		int tmpLength =0;
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)//玩家帐号在第一个数据库中
		{
			
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//构造从数据库查询得到的数据
			
		}
		else if(dbid==2)
		{
			
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//构造从数据库查询得到的数据
		}
		if(tmpLength>0)
		{
			GetPrivateProfileString("CG2","Kick_Player_Success",NULL,RetMessage,30,filepath);//保存单个返回信息
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_KickAccount_Query_Resp, RetMessage);//返回强制玩家下线成功的信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Kick Player %s Success",curTime,m_UserName,cg2_username);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_KickPlayer_Query",cg2_serverip,LogContent);
		}
		else
		{
			GetPrivateProfileString("CG2","Kick_Player_Fail",NULL,RetMessage,30,filepath);//保存单个返回信息
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_KickAccount_Query_Resp, RetMessage);//返回强制玩家下线失败的信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Kick Player %s Fail",curTime,m_UserName,cg2_username);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_KickPlayer_Query",cg2_serverip,LogContent);
		}
	}	
	catch (...)
	{
		
	}
	return true;
}

//强制玩家下线(没有回答)
bool CCG2Info::CG2_KickAccount_Query_NoAnswer()
{
	try
	{
		int dbid=0;
		
		operAssist.getRemoteSql("CG2_KickPlayer_Query",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,24,"",0,"",cg2_usersn);//格式化sql语句
		int tmpLength =0;
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)//玩家帐号在第一个数据库中
		{
			
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//构造从数据库查询得到的数据
			
		}
		else if(dbid==2)
		{
			
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//构造从数据库查询得到的数据
		}
		if(tmpLength>0)
		{
			char LogContent[128];
			ZeroMemory(LogContent,128);
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Kick Player %s Success",curTime,m_UserName,cg2_username);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_KickPlayer_Query",cg2_serverip,LogContent);
		}
		else
		{
			char LogContent[128];
			ZeroMemory(LogContent,128);
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Kick Player %s Fail",curTime,m_UserName,cg2_username);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_KickPlayer_Query",cg2_serverip,LogContent);
		}
	}	
	catch (...)
	{
		
	}
	return true;
}
//bool CCG2Info::CG2_Insert_Gmtools
//查询所有封停帐号信息
bool CCG2Info::CG2_BanAccount_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_BanAccount_Query");//将操作类型写入日志文件
		operAssist.getRemoteSql("CG2_SQLGetAllBanAccount_Qurey",operRemoteSql);//获取在远程数据库中查询所有帐号封停信息的sql语句
		sprintf(operszSql,operRemoteSql,cg2_serverip);//格式化sql语句
		int tmpLength = DBVect.OperVectorMain(logdbip, operszSql, 2, index, iPageSize);//构造从数据库查询得到的数据
		if(tmpLength>0)
		{
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BanAccount_Query_Resp, &DBVect, tmpLength);
		}
		else
		{
			GetPrivateProfileString("CG2","No_BanAccount_Exist",NULL,RetMessage,30,filepath);//保存单个返回信息
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BanAccount_Query_Resp,RetMessage);
		}
		
		
		
	}
	catch (...)
	{
		operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BanAccount_Query_Resp,"ERROR");
	}
	return true;
}
//查询特定帐号有没有被封停
bool CCG2Info::CG2_Account_BANISHMENT_QUERY()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_Account_BANISHMENT_QUERY");//将操作类型写入日志文件
		operAssist.getRemoteSql("CG2_SQLGetBanAccount_Qurey",operRemoteSql);//获取在远程数据库中查询所有帐号封停信息的sql语句
		sprintf(operszSql,operRemoteSql,cg2_username,cg2_serverip);//格式化sql语句
		int tmpLength = DBVect.OperVectorMain(logdbip, operszSql, 2, index, iPageSize);//构造从数据库查询得到的数据
		if(tmpLength>0)
		{
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_BANISHMENT_QUERY_RESP, &DBVect, tmpLength);
		}
		else
		{
			GetPrivateProfileString("CG2","No_Select_BanAccount_Exist",NULL,RetMessage,30,filepath);//保存单个返回信息
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_BANISHMENT_QUERY_RESP,RetMessage);
		}
		
		
		
	}
	catch (...)
	{
		operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_BANISHMENT_QUERY_RESP,"ERROR");
	}
	return true;
}


//封停玩家帐号
bool CCG2Info::CG2_Account_Close()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_Account_Close");//将操作类型写入日志文件						
		
		int type = 0;
		type = StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_TypeID,0)).lpdata);//获取按帐号还是角色封停

		char BanContent[512];
		ZeroMemory(BanContent,512);
		sprintf(BanContent,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Content,0)).lpdata);//获取封停原因

		time_t curTimeSec;
		time( &curTimeSec );//获取当前时间到1970-1-1之间的时间的秒数表示

		char EndTime[25];
		ZeroMemory(EndTime,25);
		sprintf(EndTime,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_BanTime,0)).lpdata);//获取封停时间	
		
		struct tm t_BanTime; //	tm结构体类型的变量，保存时间					
		sscanf(EndTime,"%d-%d-%d %d:%d:%d",&t_BanTime.tm_year,&t_BanTime.tm_mon,&t_BanTime.tm_mday,&t_BanTime.tm_hour,&t_BanTime.tm_min,&t_BanTime.tm_sec);//将封停时间赋值给tm结构体
		t_BanTime.tm_year = t_BanTime.tm_year - 1900;
		t_BanTime.tm_mon = t_BanTime.tm_mon - 1;
		time_t BanTime;//time_t类型的变量，保存时间的秒数表示
		BanTime=mktime(&t_BanTime); //获取封停时间到1970-1-1之间的时间的秒数表示
		char strBanTime[20];
		ZeroMemory(strBanTime,20);
		sprintf(strBanTime,"%i",BanTime-curTimeSec);//获取封停时间			

		operAssist.getRemoteSql("CG2_GMInsert_Query",operRemoteSql);//构造根据IP查询登录记录的sql语句
		int tmpLength=0;
		if(type == 0)
		{
			sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,1,strBanTime,0,BanContent,cg2_usersn);//格式化sql语句
		}
		else
		{
			sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,24,strBanTime,0,BanContent,cg2_usersn);//格式化sql语句
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		int  dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)//如果玩家在第一个数据库中
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//执行远程数据操作
		
		}
		else if(dbid==2)//如果玩家在第二个数据库中
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//执行远程数据操作
		}
		if(tmpLength>0)//远程操作成功
		{
			operAssist.getRemoteSql("CG2_Account_Close",operRemoteSql);//获取插入到本地数据库中的sql语句
			sprintf(operszSql,operRemoteSql,cg2_username,cg2_charno,cg2_usersn,EndTime,BanContent,cg2_serverip,type);//格式化sql语句
			tmpLength = DBVect.OperVectorMain(logdbip, operszSql, 0);//执行本地数据操作
			if(tmpLength>0)
			{
				GetPrivateProfileString("CG2","Ban_Player_Success",NULL,RetMessage,30,filepath);//保存单个返回信息
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Close_RESP, RetMessage);//返回封停玩家失败的信息
				char LogContent[128];
				ZeroMemory(LogContent,128);
				sprintf(LogContent,"%s,In Game Concerto Gate,%s Ban Player %s Success",curTime,m_UserName,cg2_username);
				logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Account_Close",cg2_serverip,LogContent);		
			}
			else
			{
				GetPrivateProfileString("CG2","Ban_Player_Fail",NULL,RetMessage,30,filepath);//保存单个返回信息
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Close_RESP, RetMessage);//返回封停玩家失败的信息
				char LogContent[128];
				ZeroMemory(LogContent,128);
				sprintf(LogContent,"%s,In Game Concerto Gate,%s Ban Player %s Fail",curTime,m_UserName,cg2_username);
				logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Account_Close",cg2_serverip,LogContent);		
			}

		}
		else
		{
			GetPrivateProfileString("CG2","Ban_Player_Fail",NULL,RetMessage,30,filepath);//保存单个返回信息
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Close_RESP, RetMessage);//返回封停玩家失败的信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Ban Player %s Fail",curTime,m_UserName,cg2_username);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Account_Close",cg2_serverip,LogContent);			
		}
	}
	catch (...)
	{
		
	}
	return true;
}

//解封玩家帐号
bool CCG2Info::CG2_Account_Open()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_Account_Open");//将操作类型写入日志文件				
		
		int type = 0;
		type = StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_TypeID,0)).lpdata);//获取按帐号还是角色封停
		
		char UnbindContent[512];
		sprintf(UnbindContent,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Content,0)).lpdata);//获取封停原因
	
		
		operAssist.getRemoteSql("CG2_GMInsert_Query",operRemoteSql);//构造根据IP查询登录记录的sql语句
		int tmpLength=0;
		if(type == 0)
		{
			sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,2,"",0,UnbindContent,cg2_usersn);//格式化sql语句
		}
		else
		{
			sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,24,"0",0,UnbindContent,"");//格式化sql语句
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		int  dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)//如果玩家在第一个数据库中
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//执行远程数据操作
			
		}
		else if(dbid==2)//如果玩家在第二个数据库中
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//执行远程数据操作
		}
		if(tmpLength>0)//远程操作成功
		{
			if(type == 1)
			{
				operAssist.getRemoteSql("CG2_Account_Open_ByAccount",operRemoteSql);//获取通过帐号进行解封插入到本地数据库中的sql语句
				sprintf(operszSql,operRemoteSql,cg2_username,type);//格式化sql语句
			}
			else
			{
				operAssist.getRemoteSql("CG2_Account_Open_ByChar",operRemoteSql);//获取通过角色名称进行解封插入到本地数据库中的sql语句
				sprintf(operszSql,operRemoteSql,cg2_username,cg2_usersn,type);//格式化sql语句
			}	
			tmpLength = DBVect.OperVectorMain(logdbip, operszSql, 0);//执行本地数据操作
			if(tmpLength>0)
			{
				GetPrivateProfileString("CG2","Unbind_Player_Sucess",NULL,RetMessage,30,filepath);//保存单个返回信息
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Open_RESP, RetMessage);//返回封停玩家失败的信息
				char LogContent[128];
				ZeroMemory(LogContent,128);
				sprintf(LogContent,"%s,In Game Concerto Gate,%s Unbind Player %s Success",curTime,m_UserName,cg2_username);
				logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Account_Open",cg2_serverip,LogContent);		
			}
			else
			{
				GetPrivateProfileString("CG2","Unbind_Player_Fail",NULL,RetMessage,30,filepath);//保存单个返回信息
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Open_RESP, RetMessage);//返回封停玩家失败的信息
				char LogContent[128];
				ZeroMemory(LogContent,128);
				sprintf(LogContent,"%s,In Game Concerto Gate,%s Unbind Player %s Fail",curTime,m_UserName,cg2_username);
				logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Account_Open",cg2_serverip,LogContent);		
			}
			
		}
		else
		{
			GetPrivateProfileString("CG2","Unbind_Player_Fail",NULL,RetMessage,30,filepath);//保存单个返回信息
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Open_RESP, RetMessage);//返回封停玩家失败的信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Unbind Player %s Fail",curTime,m_UserName,cg2_username);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Account_Open",cg2_serverip,LogContent);		
		}	
	}
	catch (...)
	{
		operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Open_RESP, "ERROR");//返回解封玩家帐号错误信息
		char LogContent[128];
		ZeroMemory(LogContent,128);
		sprintf(LogContent,"%s,In Game Concerto Gate,%s Unbind Player %s Error",curTime,m_UserName,cg2_username);
		logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Account_Open",cg2_serverip,LogContent);	
	}
	return true;
}

//查看公告
bool CCG2Info::CG2_SearchNote_Query()
{
	
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_SearchNote_Query");//将操作类型写入日志文件
		
		operAssist.getRemoteSql("CG2_SearchNote_Query",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_SearchNote_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		sprintf(operszSql,operRemoteSql);
		int tmpLength = DBVect.OperVectorMain(logdbip, operszSql, 2, index, iPageSize);//构造从数据库查询得到的数据
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_SearchNote_Query_Resp, &DBVect, tmpLength);
		
	}
	catch (...)
	{
		
	}
	return true;
}

//发送公告
bool CCG2Info::CG2_SendNote_Query()
{		
	try
	{
		char CurServerIP[256];//服务器IP
		ZeroMemory(CurServerIP,256);
		char AllServerIP[256];//服务器IP
		ZeroMemory(AllServerIP,256);
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_SendNote_Query");//将操作类型写入日志文件	
		
		sprintf(CurServerIP,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Serverip,0)).lpdata);//获取所有的服务器IP
		sprintf(AllServerIP,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Serverip,0)).lpdata);//获取所有的服务器IP
		
		char CurServerName[1024];//服务器名称
		sprintf(CurServerName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ServerName,0)).lpdata);//获取所有的服务器名称
		
		char noteContent[512];//公告内容
		sprintf(noteContent,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_NoteContent,0)).lpdata);//获取公告内容
		
		int interval = StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Interval,0)).lpdata);//获取公告时间间隔
		int type = StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_TypeID,0)).lpdata);//获取类型，是否即时发送公告
		
		char EndTime[20];//保存发送公告截止时间	
		ZeroMemory(EndTime,20);
		time_t LimitTime;//time_t类型的变量，保存时间的秒数表示
			
		
		
		if(type != 0)//如果不是即时发送公告
		{
			sprintf(EndTime,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_EndTime,0)).lpdata);//获取发送公告截止时间					
			struct tm t_EndTime; //	tm结构体类型的变量，保存时间					
			sscanf(EndTime,"%d-%d-%d %d:%d:%d",&t_EndTime.tm_year,&t_EndTime.tm_mon,&t_EndTime.tm_mday,&t_EndTime.tm_hour,&t_EndTime.tm_min,&t_EndTime.tm_sec);//将发送公告截止时间赋值给tm结构体							
			t_EndTime.tm_year = t_EndTime.tm_year - 1900;
			t_EndTime.tm_mon = t_EndTime.tm_mon - 1;
			LimitTime=mktime(&t_EndTime); 
		}
		else
		{
			sprintf(EndTime,curTime);//即时发送公告	
			LimitTime = 0; 
		}
		
		char result[1024];//保存操作结果
		ZeroMemory(result,1024);
		char tmpResult[256];//保存临时操作结果
		ZeroMemory(tmpResult,256);//初始化临时操作结果
		
		int noticeID = 0;//要获取的公告ID
		char sNoticeID[128];
		ZeroMemory(sNoticeID,128);//初始化所有的公告ID
		char tmpNoticeID[8];
		ZeroMemory(tmpNoticeID,8);//初始化临时公告ID
		
		char tmpInfo[1024];//保存临时结果
		ZeroMemory(tmpInfo,1024);//初始化临时结果
		
		const char* split = "|";//服务器地址用"|"隔开
		char* tmpServerIP = strtok(CurServerIP,split);//获取第一个服务器IP
		int tmpLength=0;
		while(tmpServerIP != NULL)
		{
			operAssist.FindDBIP(tmpServerIP,tmpServerIP,2);
			operAssist.getRemoteSql("CG2_GetNoticeCount_Query",operRemoteSql);//获取要查询数据库中当前正在发送公告数量的sql语句
			int iCount=0;
			char strCount[10];
			operAssist.CG2_Proc_Query(tmpServerIP,3,operRemoteSql,strCount);
			iCount=StrToInt(strCount);
			if(iCount<10)
			{
				char pame[520];
				ZeroMemory(pame,520);
				sprintf(pame,"%i %s",interval,noteContent);
				operAssist.getRemoteSql("CG2_GMInsert_Query",operRemoteSql);//获取插入到数据库中的sql语句
				sprintf(operszSql,operRemoteSql,curTime,"",0,4,pame,LimitTime,"","");//格式化sql语句
				operAssist.FindDBIP(tmpServerIP,tmpServerIP,1);
				tmpLength = DBVect.OperVectorMain(tmpServerIP, operszSql, 9);
				if(tmpLength>0)
				{
					operAssist.getRemoteSql("CG2_NoticeResult_Query",operRemoteSql);//获取查询公告ID的sql语句
					sprintf(operszSql,operRemoteSql,curTime,4,LimitTime);//格式化sql语句
					int inoticeID=0;
					char strnoticeID[10];
					operAssist.FindDBIP(tmpServerIP,tmpServerIP,2);
					operAssist.CG2_Proc_Query(tmpServerIP,3,operRemoteSql,strnoticeID);
					inoticeID=StrToInt(strnoticeID);
					ZeroMemory(tmpNoticeID,8);//初始化临时公告ID									
					sprintf(tmpNoticeID,"%i|",noticeID);//获取临时公告ID
					
					ZeroMemory(tmpInfo,1024);//初始化临时结果
					sprintf(tmpInfo,"%s",sNoticeID);
					sprintf(sNoticeID,"%s%s",tmpInfo,tmpNoticeID);//获取公告ID
					
					ZeroMemory(tmpResult,256);//初始化临时操作结果
					GetPrivateProfileString("CG2","Server",NULL,RetMessage,30,filepath);//服务器
					sprintf(tmpResult,"%s%s,",RetMessage,tmpServerIP);
					GetPrivateProfileString("CG2","Send_BoardTask_Success",NULL,RetMessage,30,filepath);//公告发送成功!
					sprintf(tmpResult,"%s%s,",tmpResult,RetMessage);//构造每一次操作的操作结果
					
					ZeroMemory(tmpInfo,1024);//初始化临时结果
					sprintf(tmpInfo,"%s",result);
					sprintf(result,"%s\n%s",tmpInfo,tmpResult);//构造最终操作的操作结果
				}
				else
				{
					ZeroMemory(tmpResult,256);//初始化临时操作结果
					GetPrivateProfileString("CG2","Server",NULL,RetMessage,30,filepath);//服务器
					sprintf(tmpResult,"%s%s,",RetMessage,tmpServerIP);
					GetPrivateProfileString("CG2","TooMany_BoardTask",NULL,RetMessage,30,filepath);//待发公告超过10条，请稍候再发!
					sprintf(tmpResult,"%s%s,",tmpResult,RetMessage);//构造每一次操作的操作结果
					
					ZeroMemory(tmpInfo,1024);//初始化临时结果
					sprintf(tmpInfo,"%s",result);
					sprintf(result,"%s\n%s",tmpInfo,tmpResult);//构造最终操作的操作结果
				}
				


			}
			else
			{
				ZeroMemory(tmpResult,256);//初始化临时操作结果
				GetPrivateProfileString("CG2","Server",NULL,RetMessage,30,filepath);//服务器
				sprintf(tmpResult,"%s%s,",RetMessage,tmpServerIP);
				GetPrivateProfileString("CG2","TooMany_BoardTask",NULL,RetMessage,30,filepath);//待发公告超过10条，请稍候再发!
				sprintf(tmpResult,"%s%s,",tmpResult,RetMessage);//构造每一次操作的操作结果
				
				ZeroMemory(tmpInfo,1024);//初始化临时结果
				sprintf(tmpInfo,"%s",result);
				sprintf(result,"%s\n%s",tmpInfo,tmpResult);//构造最终操作的操作结果
			}

			tmpServerIP = strtok(NULL,split);//顺序得到服务器IP
		}
		if(tmpServerIP != NULL)
		{
			delete []tmpServerIP;
			tmpServerIP=NULL;
		}


		operAssist.getRemoteSql("CG2_SendNote_Query",operRemoteSql);//获取最后一条sql语句
		sprintf(operszSql,operRemoteSql,AllServerIP,CurServerName,noteContent,"0",curTime,sNoticeID,EndTime);//格式化sql语句
		
		tmpLength=DBVect.OperVectorMain(logdbip, operszSql, 0);
		if(tmpLength>0)
		{
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_SendNote_Query_Resp,result);//返回最后操作得到的结果
			char LogContent[256];
			ZeroMemory(LogContent,256);
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Send Notice %s %s",curTime,m_UserName,sNoticeID,result);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SendNote_Query",AllServerIP,LogContent);
		}
		else
		{
			GetPrivateProfileString("CG2","Send_BoardTask_Fail",NULL,RetMessage,30,filepath);//保存单个返回信息
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_SendNote_Query_Resp,RetMessage);//返回最后操作得到的结果
			
			char LogContent[256];
			ZeroMemory(LogContent,256);
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Send Notice sNoticeID Fail",curTime,m_UserName,sNoticeID);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SendNote_Query",AllServerIP,LogContent);
		}		
		
		
		
	}
	catch (...)
	{
		
	}
	return true;
}

//停止发送公告
bool CCG2Info::CG2_DeleteNote_Query()
{
	char CurServerIP[256];//服务器IP
	ZeroMemory(CurServerIP,256);
	char AllServerIP[256];//服务器IP
	ZeroMemory(AllServerIP,256);
	try
	{
		
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_DeleteNote_Query");//将操作类型写入日志文件						
		
		sprintf(CurServerIP,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Serverip,0)).lpdata);//获取所有的服务器IP
		sprintf(AllServerIP,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Serverip,0)).lpdata);//获取所有的服务器IP
		
		char noticeID[256];//公告ID
		char AllNoticeID[256];//公告ID^
		sprintf(noticeID,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_NoticeID,0)).lpdata);//获取公告ID
		sprintf(AllNoticeID,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_NoticeID,0)).lpdata);//获取所有的服务器IP
		
		const char* split = "|";//服务器地址用"|"隔开
		char* tmpServerIP = strtok(CurServerIP,split);//获取第一个服务器IP
		char* tmpNoticeID = strtok(noticeID,split);//获取第一个公告ID
		int tmpLength=0;
		while(tmpServerIP != NULL)
		{
		//	GetConfigInfo(tmpServerIP,DBUser,DBUserOper,DBPwd,DBName,DBNameSec,CharSet);
			
			operAssist.getRemoteSql("CG2_GMInsert_Query",operRemoteSql);//获取插入到数据库中的sql语句
			sprintf(operszSql,operRemoteSql,curTime,"",0,5,tmpNoticeID,0,"","");//格式化sql语句
			operAssist.FindDBIP(tmpServerIP,tmpServerIP,1);
			tmpLength=DBVect.OperVectorMain(tmpServerIP, operszSql, 9);

			
			tmpServerIP = strtok(NULL,split);//顺序得到服务器IP
			tmpNoticeID = strtok(NULL,split);//顺序得到公告ID
		}						
		
		if(tmpServerIP != NULL)
		{
			delete []tmpServerIP;
			tmpServerIP = NULL;
		}
		if(tmpNoticeID != NULL)
		{
			delete []tmpNoticeID;
			tmpNoticeID = NULL;
		}
		
		if(tmpLength>0)//操作成功
		{
			sprintf(operszSql,"update CG2_SendNote set result=1 where gm_id='%s'",AllNoticeID);//格式化sql语句
			DBVect.OperVectorMain(logdbip, operszSql, 0);
			
			GetPrivateProfileString("CG2","Stop_BoardTask_Success",NULL,RetMessage,30,filepath);//保存单个返回信息
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_DeleteNote_Query_Resp,RetMessage);//返回停止发送公告成功的信息
			
			char LogContent[256];
			ZeroMemory(LogContent,256);
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Stop Notice %s Success",curTime,m_UserName,AllNoticeID);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_DeleteNote_Query",AllServerIP,LogContent);
		}
		else
		{
			GetPrivateProfileString("CG2","Stop_BoardTask_Fail",NULL,RetMessage,30,filepath);//保存单个返回信息
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_DeleteNote_Query_Resp,RetMessage);//返回停止发送公告成功的信息
			
			char LogContent[256];
			ZeroMemory(LogContent,256);
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Stop Notice %s Fail",curTime,m_UserName,AllNoticeID);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_DeleteNote_Query",AllServerIP,LogContent);
		}
	}
	catch(...)
	{
		char ErrorMsg[400];
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER| //自动分配消息缓冲区
			FORMAT_MESSAGE_FROM_SYSTEM, //从系统获取信息
			NULL,GetLastError(), //获取错误信息标识
			MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),//使用系统缺省语言
			(LPTSTR)&lpMsgBuf, //消息缓冲区
			0,
			NULL);
		sprintf(ErrorMsg, "%s",lpMsgBuf);
		LocalFree(lpMsgBuf);
		
		operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_DeleteNote_Query_Resp,"Error");//返回错误信息

		
		char LogContent[256];
		ZeroMemory(LogContent,256);
		sprintf(LogContent,"%s,In Game Concerto Gate,%s Stop Notice Error",curTime,m_UserName);
		logFile.WriteDBLog(userByID,"Concerto Gate","CG2_DeleteNote_Query",AllServerIP,LogContent);	
	}

	return true;
}

//查询备份玩家角色信息
bool CCG2Info::CG2_BakAccount_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"帐号<%s>查询大区<%s>备份玩家<%s>角色信息",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("魔力宝贝2",strLog);
		if(strcmp("", cg2_username))//如果玩家帐号不为空
		{
			
			operAssist.getRemoteSql("CG2_BAKACCOUNT_QUERY",operRemoteSql);//构造根据IP查询登录记录的sql语句
			if(!strcmp("",operRemoteSql))
			{
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BakAccount_Query_Resp, "数据库中找不到对应的sql语句");
				return false;
			}
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
			sprintf(operszSql,operRemoteSql,cg2_username);
			int tmpLength = 0;
			if(dbid==1)//玩家帐号在第一个数据库中
			{
				
				tmpLength =DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//构造从数据库查询得到的数据
				operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BakAccount_Query_Resp, &DBVect, tmpLength);
				
			}
			else if(dbid==2)
			{
				tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//构造从数据库查询得到的数据
				operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BakAccount_Query_Resp, &DBVect, tmpLength);
			}
			else
			{
				operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BakAccount_Query_Resp, "该帐号不存在");
			}
			
		}
		else
		{
			operAssist.getRemoteSql("CG2_BAKNICK_QUERY",operRemoteSql);//获取利用昵称查询的sql语句
			if(!strcmp("",operRemoteSql))
			{
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BakAccount_Query_Resp, "数据库中找不到对应的sql语句");
				return false;
			}
			sprintf(operszSql,operRemoteSql,"\%",cg2_nickname,"\%");//格式化sql语句，注意转义符
			DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, 0, 0);//构造从数据库查询得到的数据
			DBVect1.OperVectorMain("CG2",cg2_serverip, operszSql, 5, 0, 0);//构造从数据库查询得到的数据
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BakAccount_Query_Resp, &DBVect, &DBVect1,index,iPageSize);
			
			
		}
		
	}
	catch (...)
	{
		
	}
	return true;
}


//恢复角色操作
bool CCG2Info::CG2_Recovery_Account_Qurey()
{
	try
	{
		int tmpLength=0;
		int dbid=0;
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_Recovery_Account_Qurey");//将操作类型写入日志文件

		int iResult=0;
		//恢复角色之前踢人
		bool b_result=CG2_KickAccount_Query_NoAnswer();//踢人
		char result[40];
		if(b_result)
		{
			Sleep(30000);//延时30秒等待结果
			//完成查询
			operAssist.getRemoteSql("CG2_SearchCharNo_Query",operRemoteSql);//获取返回结果sql语句
			sprintf(operszSql,operRemoteSql,cg2_username,cg2_charno);
			dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);//查询IP
			if(dbid==1)
			{
				operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,result);
			}
			else
			{
				operAssist.CG2_Proc_Query(cg2_serverip,5,operszSql,result);
			}
			if(StrToInt(result)==0)
			{
				operAssist.getRemoteSql("CG2_isCharNo_Query",operRemoteSql);//获取返回结果sql语句
				sprintf(operszSql,operRemoteSql,cg2_username);
				operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);//查询IP
				if(dbid==1)
				{
					operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,result);
				}
				else
				{
					operAssist.CG2_Proc_Query(cg2_serverip,5,operszSql,result);
				}
				if(cg2_charno>0&&StrToInt(result)<0)
				{
					iResult=3;
				}
				else
				{
					for(int i = 1;i<=13;i++)
					{
						char sql_type[40];
						sprintf(sql_type,"CG2_IsExists_Account_Qurey%i",i);
						operAssist.getRemoteSql(sql_type,operRemoteSql);//获取返回结果sql语句
						sprintf(operszSql,operRemoteSql,cg2_username,cg2_usersn);
						operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);//查询IP
						if(dbid==1)
						{
							operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,result);
						}
						else
						{
							operAssist.CG2_Proc_Query(cg2_serverip,5,operszSql,result);
						}
						if(StrToInt(result)==0)
						{
							sprintf(sql_type,"CG2_Recovery_Account_Qurey%i",i);
							operAssist.getRemoteSql(sql_type,operRemoteSql);//获取返回结果sql语句
							sprintf(operszSql,operRemoteSql,cg2_username,cg2_usersn);
							operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);//查询IP
							if(dbid==1)
							{
								iResult = iResult+ DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//构造从数据库查询得到的数据
							}
							else
							{
								iResult = iResult+DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//构造从数据库查询得到的数据
							}
						}
					}
				}
				if(iResult==13)
				{
					sprintf(result,"账号%s恢复角色%s成功",cg2_username,cg2_usersn);
					operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Recovery_Account_Qurey_Resp, result);
					operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
					logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_Recovery_Account",cg2_serverip,result);
				}
				else
				{
					sprintf(result,"账号%s恢复角色%s失败",cg2_username,cg2_usersn);
					operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Recovery_Account_Qurey_Resp, result);
					operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
					logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_Recovery_Account",cg2_serverip,result);
				}
				
			}
			else
			{
				iResult=2;
				sprintf(result,"账号%s角色栏%i上有角色",cg2_username,cg2_charno);
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Recovery_Account_Qurey_Resp, result);
				operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
				logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_Recovery_Account",cg2_serverip,result);
			}
			


		}
		else
		{
			sprintf(result,"%s","踢人失败");
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Recovery_Account_Qurey_Resp, result);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_Recovery_Account",cg2_serverip,result);
		}
	}
	catch (...)
	{
		
	}
	return true;
}


//玩家爱情树
bool CCG2Info::CG2_LoveTree_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"帐号<%s>查询大区<%s>玩家<%s>爱情树",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("魔力宝贝2",strLog);
			
		operAssist.getRemoteSql("CG2_LOVETREE_QUERY",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_LoveTree_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		sprintf(operszSql,operRemoteSql,"%Y-%m-%d %H:%m:%s",cg2_usersn);
		int tmpLength = 0;
		if(dbid==1)//玩家帐号在第一个数据库中
		{
			tmpLength =DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//构造从数据库查询得到的数据
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//构造从数据库查询得到的数据

		}
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_LoveTree_Query_Resp, &DBVect, tmpLength);

			
		
		
	}
	catch (...)
	{
		
	}
	return true;
}



//玩家婚姻情况(并库)
bool CCG2Info::CG2_Marriage_Info_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"帐号<%s>查询大区<%s>玩家<%s>婚姻情况",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("魔力宝贝2",strLog);
		
		operAssist.getRemoteSql("CG2_MARRIAGE_INFO_QUERY",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Marriage_Info_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		sprintf(operszSql,operRemoteSql,"%Y-%m-%d %H:%m:%s",cg2_usersn);
		int tmpLength = 0;
		
		DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, 0, 0);//构造从数据库查询得到的数据
		DBVect1.OperVectorMain("CG2",cg2_serverip, operszSql, 5, 0, 0);//构造从数据库查询得到的数据
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Marriage_Info_Query_Resp, &DBVect, &DBVect1,index,iPageSize);

		
		
		
		
	}
	catch (...)
	{
		
	}
	return true;
}
//公会信息(并库)
bool CCG2Info::CG2_SearchGuild_Query()
{
	try
	{

		sprintf(strLog,"帐号<%s>查询大区<%s>公会信息",m_UserName,cg2_serverip);
		logFile.WriteText("魔力宝贝2",strLog);
		
		operAssist.getRemoteSql("CG2_SEARCHGUILD_QUERY",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_SearchGuild_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		sprintf(operszSql,operRemoteSql,"\%",cg2_guildname,"\%");//格式化sql语句，注意转义符
		int tmpLength = 0;
		
		DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, 0, 0);//构造从数据库查询得到的数据
		DBVect1.OperVectorMain("CG2",cg2_serverip, operszSql, 5, 0, 0);//构造从数据库查询得到的数据
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_SearchGuild_Query_Resp, &DBVect, &DBVect1,index,iPageSize);
			
	}
	catch (...)
	{
		
	}
	return true;


}


//公会成员信息(并库)
bool CCG2Info::CG2_SearchGuild_Detail_Query()
{
	try
	{

		sprintf(strLog,"帐号<%s>查询大区<%s>公会成员信息",m_UserName,cg2_serverip);
		logFile.WriteText("魔力宝贝2",strLog);
		
		operAssist.getRemoteSql("CG2_SEARCHGUILDDETAIL_QUERY",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_SearchGuild_Detail_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		sprintf(operszSql,operRemoteSql,cg2_guildid);//格式化sql语句，注意转义符
		int tmpLength = 0;
		logFile.WriteText("fa",operszSql);
		
		DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, 0, 0);//构造从数据库查询得到的数据
		DBVect1.OperVectorMain("CG2",cg2_serverip, operszSql, 5, 0, 0);//构造从数据库查询得到的数据
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_SearchGuild_Detail_Query_Resp, &DBVect, &DBVect1,index,iPageSize);
		
	}
	catch (...)
	{
		
	}
	return true;
	
	
}



//解散公会
bool CCG2Info::CG2_CloseGuild_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_SkillLevel_Update");//将操作类型写入日志文件
		
		int result=0;

		char UserSkillLevel[50];
		ZeroMemory(UserSkillLevel,50);
		sprintf(UserSkillLevel,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SkillLevel,0)).lpdata);//获取用户技能等级

		char SkillID[20];
		ZeroMemory(SkillID,20);
		sprintf(SkillID,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SkillID,0)).lpdata);//获取用户技能id

		char remark[256];
		ZeroMemory(remark,256);
		sprintf(remark,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Remark,0)).lpdata);///备注////update skill level

		//GetConfigInfo(ServerIP,DBUser,DBUserOper,DBPwd,DBName,DBNameSec,CharSet);//获得配置文件信息

		operAssist.getRemoteSql("CG2_SkillLevel_Update",operRemoteSql);//获取修改玩家角色职业sql语句


		//sprintf(szSql,remotesql,curTime,UserName,charNo,UserSkillLevel,remark,UserSN);//格式化sql语句
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,SkillID,UserSkillLevel,remark,cg2_usersn);//格式化sql语句

		//MakeRemoteSql((char *)&ServerIP,(char*)&DBUser,(char*)&DBPwd,(char*)&DBName, (char*)&CharSet, remoteConnectSql);
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		int dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		int tmpLength=0;
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)//如果玩家在第一个数据库中
		{
			//构造对第一个数据库进行操作的sql语句
			//	MakeRemoteSql((char *)&ServerIP,(char*)&DBUserOper,(char*)&DBPwd,(char*)&DBName, (char*)&CharSet, remoteConnectSql);
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		}
		else if(dbid==2)//如果玩家在第二个数据库中
		{
			//构造对第二个数据库进行操作的sql语句
			//MakeRemoteSql((char *)&ServerIP,(char*)&DBUserOper,(char*)&DBPwd,(char*)&DBNameSec, (char*)&CharSet, remoteConnectSql);
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,11);
		}

		if(tmpLength>0)//操作成功
		{							
			operAssist.getRemoteSql("CG2_UpdateChar_Query",operRemoteSql);//获取插入到数据库中的sql语句
			sprintf(operszSql,operRemoteSql,cg2_username,cg2_charno,cg2_usersn,remark,0,cg2_serverip,result);//格式化sql语句
			tmpLength=DBVect.OperVectorMain(logdbip,operszSql,0);
			if(tmpLength>0)//操作成功
			{
				//maple add
					Sleep(30000);

					operAssist.getRemoteSql("CG2_Return_Result",operRemoteSql);//获取返回结果sql语句
					sprintf(operszSql,operRemoteSql,curTime,cg2_username,SkillID,UserSkillLevel);//格式化sql语句，当前时间，玩家帐号，参数

					char gm_result[2];
					ZeroMemory(gm_result,2);
					operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
					if(dbid==1)//如果玩家在第一个数据库中
					{
						//构造对第一个数据库进行操作的sql语句
						operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,gm_result);
					}
					else if(dbid==2)//如果玩家在第二个数据库中
					{
						//构造对第二个数据库进行操作的sql语句
						operAssist.CG2_Proc_Query(cg2_serverip,5,operszSql,gm_result);
					}
					
							

					if(!strcmp(gm_result,"2"))
					{
						GetPrivateProfileString("CG2","Update_SkillLevel_Fail",NULL,RetMessage,30,filepath);//保存单个返回信息
						operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_CloseGuild_Query_Resp,RetMessage);//返回信息
						char LogContent[128];
						ZeroMemory(LogContent,128);
						//写入日志文件
						sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill Level %s Fail",curTime,m_UserName,cg2_username);
						logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
					}
					else
					{
						GetPrivateProfileString("CG2","Update_SkillLevel_Success",NULL,RetMessage,30,filepath);//保存单个返回信息
						operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_CloseGuild_Query_Resp,RetMessage);//返回信息
						char LogContent[128];
						ZeroMemory(LogContent,128);
						//写入日志文件
						sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill Level %s Success",curTime,m_UserName,cg2_username);
						logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
					}
				}
			}
			else
			{
				GetPrivateProfileString("CG2","Update_CharJob_Fail",NULL,RetMessage,30,filepath);//保存单个返回信息
				operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_CloseGuild_Query_Resp,RetMessage);//返回封停玩家失败的信息
				char LogContent[128];
				ZeroMemory(LogContent,128);
				//写入日志文件
				sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Job %s Fail",curTime,m_UserName,cg2_username);
				logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
			}
		}
		catch(...)
		{
				operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_CloseGuild_Query_Resp,"ERROR");//返回修改职业错误信息
				char LogContent[128];
				ZeroMemory(LogContent,128);
				//写入日志文件
				sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill %s Fail",curTime,m_UserName,cg2_username);
				logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
		}
		return true;
	
}

//修改公会名字
bool CCG2Info::CG2_RenameGuild_Query()
{
		try
		{
			logFile.WriteLog("Concerto Gate",m_UserName,"CG2_RenameGuild_Query");//将操作类型写入日志文件
		
			int result=0;

			operAssist.getRemoteSql("CG2_GUILDNAME_QUERY",operRemoteSql);//获取返回结果sql语句
			sprintf(operszSql,operRemoteSql,cg2_guildid);
			char gm_result[40];
			ZeroMemory(gm_result,40);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,gm_result);

			char guildname[256];
			ZeroMemory(guildname,256);
			sprintf(guildname,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_GuildName,0)).lpdata);///新公会名

			char pame[256];
			sprintf(pame,"%i %s",cg2_guildid,guildname);
			operAssist.getRemoteSql("CG2_RENAMEGUILD_Query",operRemoteSql);//获取修改玩家角色职业sql语句
		
	

			int tmpLength=0;
			sprintf(operszSql,operRemoteSql,curTime,"",0,7,pame,0,"","");//格式化sql语句
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
			if(tmpLength>0)//操作成功
			{							
				Sleep(30000);
				sprintf(RetMessage,"公会%s 修改成新公会名%s成功",gm_result,cg2_guildname);							
				operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_RenameGuild_Query_Resp,RetMessage);//返回信息
				char LogContent[128];
				ZeroMemory(LogContent,128);
				//写入日志文件
				sprintf(LogContent,"%s,魔力宝贝2,公会%s修改成新公会名%s成功",curTime,gm_result,cg2_guildname);
				logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_RenameGuild_Query",cg2_serverip,LogContent);
				
			}
			else
			{
				sprintf(RetMessage,"公会%s 修改成新公会名%s失败",gm_result,cg2_guildname);	
				operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_RenameGuild_Query_Resp,RetMessage);//返回信息
				char LogContent[128];
				ZeroMemory(LogContent,128);
				//写入日志文件
				sprintf(LogContent,"%s,魔力宝贝2,公会%s修改成新公会名%s失败",curTime,gm_result,cg2_guildname);
				logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_RenameGuild_Query",cg2_serverip,LogContent);


			}
		}
		catch(...)
		{
			
				operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_RenameGuild_Query_Resp,"ERROR");//返回修改职业错误信息
				char LogContent[128];
				ZeroMemory(LogContent,128);
				//写入日志文件
				sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill %s Fail",curTime,m_UserName,cg2_username);
				logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
		}
		return true;
	
}

//公会等级修改
bool CCG2Info::CG2_UpdateGuildLv_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_UpdateGuildLv_Query");//将操作类型写入日志文件
		
		int result=0;
		
		operAssist.getRemoteSql("CG2_GUILDNAME_QUERY",operRemoteSql);//获取返回结果sql语句
		sprintf(operszSql,operRemoteSql,cg2_guildid);
		char gm_result[40];
		ZeroMemory(gm_result,40);
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,gm_result);
		
		char newguildlevel[256];
		ZeroMemory(newguildlevel,256);
		sprintf(newguildlevel,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_GuildRank,0)).lpdata);///新公会名
		
		char pame[256];
		sprintf(pame,"%i %s",cg2_guildid,newguildlevel);
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);//获取修改玩家角色职业sql语句
		
		
		
		int tmpLength=0;
		sprintf(operszSql,operRemoteSql,curTime,"",0,28,pame,0,"","");//格式化sql语句
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		if(tmpLength>0)//操作成功
		{							
			Sleep(30000);
			sprintf(RetMessage,"公会%s修改成新等级%s成功",gm_result,newguildlevel);							
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UpdateGuildLv_Query_Resp,RetMessage);//返回信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//写入日志文件
			sprintf(LogContent,"%s,魔力宝贝2,公会%s修改成新公会名%s成功",curTime,gm_result,cg2_guildname);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_RenameGuild_Query",cg2_serverip,LogContent);
			
		}
		else
		{
			sprintf(RetMessage,"公会%s 修改成新等级%s失败",gm_result,newguildlevel);	
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UpdateGuildLv_Query_Resp,RetMessage);//返回信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//写入日志文件
			sprintf(LogContent,"%s,魔力宝贝2,公会%s修改成新公会名%s失败",curTime,gm_result,cg2_guildname);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_RenameGuild_Query",cg2_serverip,LogContent);
			
			
		}
	}
	catch(...)
	{
		
		operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_RenameGuild_Query_Resp,"ERROR");//返回修改职业错误信息
		char LogContent[128];
		ZeroMemory(LogContent,128);
		//写入日志文件
		sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill %s Fail",curTime,m_UserName,cg2_username);
		logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
	}
	return true;
	
}

//公会荣誉修改
bool CCG2Info::CG2_UpdateGuildHonor_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_UpdateGuildLv_Query");//将操作类型写入日志文件
		
		int result=0;
		
		operAssist.getRemoteSql("CG2_GUILDNAME_QUERY",operRemoteSql);//获取返回结果sql语句
		sprintf(operszSql,operRemoteSql,cg2_guildid);
		char gm_result[40];
		ZeroMemory(gm_result,40);
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,gm_result);
		
		char newguildlevel[256];
		ZeroMemory(newguildlevel,256);
		sprintf(newguildlevel,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_GuildRank,0)).lpdata);///新公会名
		
		char pame[256];
		sprintf(pame,"%i %s",cg2_guildid,newguildlevel);
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);//获取修改玩家角色职业sql语句
		
		
		
		int tmpLength=0;
		sprintf(operszSql,operRemoteSql,curTime,"",0,28,pame,0,"","");//格式化sql语句
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		if(tmpLength>0)//操作成功
		{							
			Sleep(30000);
			sprintf(RetMessage,"公会%s修改成新等级%s成功",gm_result,newguildlevel);							
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UpdateGuildLv_Query_Resp,RetMessage);//返回信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//写入日志文件
			sprintf(LogContent,"%s,魔力宝贝2,公会%s修改成新公会名%s成功",curTime,gm_result,cg2_guildname);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_RenameGuild_Query",cg2_serverip,LogContent);
			
		}
		else
		{
			sprintf(RetMessage,"公会%s 修改成新等级%s失败",gm_result,newguildlevel);	
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UpdateGuildLv_Query_Resp,RetMessage);//返回信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//写入日志文件
			sprintf(LogContent,"%s,魔力宝贝2,公会%s修改成新公会名%s失败",curTime,gm_result,cg2_guildname);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_RenameGuild_Query",cg2_serverip,LogContent);
			
			
		}
	}
	catch(...)
	{
		
		operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_RenameGuild_Query_Resp,"ERROR");//返回修改职业错误信息
		char LogContent[128];
		ZeroMemory(LogContent,128);
		//写入日志文件
		sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill %s Fail",curTime,m_UserName,cg2_username);
		logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
	}
	return true;
	
}


//查询背包道具信息
bool CCG2Info::CG2_PackItem_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"帐号<%s>查询大区<%s>玩家<%s>背包道具信息",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("魔力宝贝2",strLog);
		
		operAssist.getRemoteSql("CG2_PACKITEM_QUERY",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_PackItem_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		sprintf(operszSql,operRemoteSql,cg2_username,cg2_usersn);
		int tmpLength = 0;
		if(dbid==1)//玩家帐号在第一个数据库中
		{
			tmpLength =DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//构造从数据库查询得到的数据
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//构造从数据库查询得到的数据
			
		}
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_PackItem_Query_Resp, &DBVect, tmpLength);
		
		
		
		
	}
	catch (...)
	{
		
	}
	return true;
}
//删除角色道具栏道具
bool CCG2Info::CG2_BagItem_Del_Query()
{
	try
	{
		int tmpLength=0;
		int dbid=0;
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_BagItem_Del_Query");//将操作类型写入日志文件
		
		char strResult[256];
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid=operAssist.GetDBID(cg2_serverip,cg2_username);
		
		
		char pame[40];
		char itemname[40];
		
		sprintf(pame,"%s %s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SlotNum,0)).lpdata,
			(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetItemID,0)).lpdata);
		sprintf(itemname,"%s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemName,0)).lpdata);
		
		
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);	
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,27,pame,0,"",cg2_usersn);//格式化sql语句
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//构造从数据库查询得到的数据
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//构造从数据库查询得到的数据
		}
		if(tmpLength>0)
		{
			Sleep(30000);
			sprintf(strResult,"账号%s角色名%s删除角色道具栏道具:卡槽%s删除道具%s成功",cg2_username,cg2_usersn,
				(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SlotNum,0)).lpdata,
				itemname);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BagItem_Del_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_Recovery_Account",cg2_serverip,strResult);
			
			
		}
		else
		{
			sprintf(strResult,"账号%s角色名%s删除角色道具栏道具:卡槽%s删除道具%s失败",cg2_username,cg2_usersn,
				(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SlotNum,0)).lpdata,
				itemname);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BagItem_Del_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_Recovery_Account",cg2_serverip,strResult);
		}
		
	}
	catch (...)
	{
		
	}
	
	return true;

}


//角色临时背包信息
bool CCG2Info::CG2_TempItem_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"帐号<%s>查询大区<%s>玩家<%s>角色临时背包信息",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("魔力宝贝2",strLog);
		
		operAssist.getRemoteSql("CG2_TEMPITEM_QUERY",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_TempItem_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		sprintf(operszSql,operRemoteSql,cg2_usersn);
		int tmpLength = 0;
		if(dbid==1)//玩家帐号在第一个数据库中
		{
			tmpLength =DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//构造从数据库查询得到的数据
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//构造从数据库查询得到的数据
			
		}
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_TempItem_Query_Resp, &DBVect, tmpLength);
		
		
		
		
	}
	catch (...)
	{
		
	}
	return true;
}
//删除临时道具栏道具
bool CCG2Info::CG2_TmpItem_Del_Query()
{
	try
	{
		int tmpLength=0;
		int dbid=0;
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_TmpItem_Del_Query");//将操作类型写入日志文件
		
		char strResult[256];
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid=operAssist.GetDBID(cg2_serverip,cg2_username);
		
		
		char pame[40];
		char itemname[40];
		
		sprintf(pame,"%s %s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SlotNum,0)).lpdata,
			(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetItemID,0)).lpdata);
		sprintf(itemname,"%s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemName,0)).lpdata);
		
		
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);	
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,31,pame,0,"",cg2_usersn);//格式化sql语句
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//构造从数据库查询得到的数据
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//构造从数据库查询得到的数据
		}
		if(tmpLength>0)
		{
			Sleep(30000);
			sprintf(strResult,"账号%s角色名%s删除临时道具栏道具:卡槽%s删除道具%s成功",cg2_username,cg2_usersn,
				(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SlotNum,0)).lpdata,
				itemname);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_TmpItem_Del_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_Recovery_Account",cg2_serverip,strResult);
			
			
		}
		else
		{
			sprintf(strResult,"账号%s角色名%s删除临时道具栏道具:卡槽%s删除道具%s失败",cg2_username,cg2_usersn,
				(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SlotNum,0)).lpdata,
				itemname);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_TmpItem_Del_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_Recovery_Account",cg2_serverip,strResult);
		}
		
	}
	catch (...)
	{
		
	}
	
	return true;

}
//身上装备信息
bool CCG2Info::CG2_CharItem_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"帐号<%s>查询大区<%s>玩家<%s>身上装备信息",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("魔力宝贝2",strLog);
		
		operAssist.getRemoteSql("CG2_CHARITEM_QUERY",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_CharItem_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		sprintf(operszSql,operRemoteSql,cg2_usersn);
		int tmpLength = 0;
		if(dbid==1)//玩家帐号在第一个数据库中
		{
			tmpLength =DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//构造从数据库查询得到的数据
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//构造从数据库查询得到的数据
			
		}

		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_CharItem_Query_Resp, &DBVect, tmpLength);
		
		
		
		
	}
	catch (...)
	{
		
	}
	return true;
}

//删除随身装备道具
bool CCG2Info::CG2_EquipItem_Del_Query()
{
	try
	{
		int tmpLength=0;
		int dbid=0;
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_EquipItem_Del_Query");//将操作类型写入日志文件
		
		char strResult[256];
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid=operAssist.GetDBID(cg2_serverip,cg2_username);
		
		
		char pame[40];
		char itemname[40];
		
		sprintf(pame,"%s %s %s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SlotNum,0)).lpdata,
			(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemColNum,0)).lpdata,
			(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetItemID,0)).lpdata);
		sprintf(itemname,"%s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemName,0)).lpdata);
		
		
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);	
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,32,pame,0,"",cg2_usersn);//格式化sql语句
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//构造从数据库查询得到的数据
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//构造从数据库查询得到的数据
		}
		if(tmpLength>0)
		{
			Sleep(30000);
			sprintf(strResult,"账号%s角色名%s删除随身装备道具:卡槽%s物品栏%s删除道具%s成功",cg2_username,cg2_usersn,
				(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SlotNum,0)).lpdata,
				(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemColNum,0)).lpdata,
				itemname);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_EquipItem_Del_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_Recovery_Account",cg2_serverip,strResult);
			
			
		}
		else
		{
			sprintf(strResult,"账号%s角色名%s删除随身装备道具:卡槽%s物品栏%s删除道具%s失败",cg2_username,cg2_usersn,
				(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SlotNum,0)).lpdata,
				(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemColNum,0)).lpdata,
				itemname);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_EquipItem_Del_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_Recovery_Account",cg2_serverip,strResult);
		}
		
	}
	catch (...)
	{
		
	}
	
	return true;

}
//收费道具查询
bool CCG2Info::CG2_ShopItem_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"帐号<%s>查询大区<%s>玩家<%s>收费道具查询",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("魔力宝贝2",strLog);
		
		operAssist.getRemoteSql("CG2_SHOPITEM_QUERY",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_ShopItem_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		sprintf(operszSql,operRemoteSql,cg2_username);
		int tmpLength = 0;
		if(dbid==1)//玩家帐号在第一个数据库中
		{
			tmpLength =DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//构造从数据库查询得到的数据
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//构造从数据库查询得到的数据
			
		}
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_ShopItem_Query_Resp, &DBVect, tmpLength);
		
		
		
		
	}
	catch (...)
	{
		
	}
	return true;
}

//宠物基本资料
bool CCG2Info::CG2_PetInfo_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"帐号<%s>查询大区<%s>玩家<%s>宠物基本资料",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("魔力宝贝2",strLog);
		
		operAssist.getRemoteSql("CG2_PETINFO_QUERY",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_PetInfo_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		sprintf(operszSql,operRemoteSql,cg2_usersn);
		int tmpLength = 0;
		if(dbid==1)//玩家帐号在第一个数据库中
		{
			tmpLength =DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//构造从数据库查询得到的数据
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//构造从数据库查询得到的数据
			
		}

		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_PetInfo_Query_Resp, &DBVect, tmpLength);
		
		
		
		
	}
	catch (...)
	{
		
	}
	return true;
}

//宠物技能信息
bool CCG2Info::CG2_PetSkill_Query()
{
	try
	{
		int dbid=0;
		int PetID=0;
		PetID=StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetID,0)).lpdata);//宠物ID
		sprintf(strLog,"帐号<%s>查询大区<%s>玩家<%s>宠物技能信息",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("魔力宝贝2",strLog);
		
		operAssist.getRemoteSql("CG2_PETSKILL_QUERY",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_PetSkill_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		sprintf(operszSql,operRemoteSql,cg2_usersn,PetID);
		int tmpLength = 0;
		if(dbid==1)//玩家帐号在第一个数据库中
		{
			tmpLength =DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//构造从数据库查询得到的数据
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//构造从数据库查询得到的数据
			
		}
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_PetSkill_Query_Resp, &DBVect, tmpLength);
		
		
		
		
	}
	catch (...)
	{
		
	}
	return true;
}

//银行存款信息
bool CCG2Info::CG2_BankInfo_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"帐号<%s>查询大区<%s>玩家<%s>银行存款信息",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("魔力宝贝2",strLog);
		
		operAssist.getRemoteSql("CG2_BANKINFO_Query",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BankInfo_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		sprintf(operszSql,operRemoteSql,cg2_username);
		int tmpLength = 0;
		if(dbid==1)//玩家帐号在第一个数据库中
		{
			tmpLength =DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//构造从数据库查询得到的数据
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//构造从数据库查询得到的数据
			
		}
		
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BankInfo_Query_Resp, &DBVect, tmpLength);
		
		
		
		
	}
	catch (...)
	{
		
	}
	return true;
}
//设置银行金钱
bool CCG2Info::CG2_SetBankMoney_Query()
{
	try
	{
		operAssist.ReadValue("GameName",retMessage[0],256);
		operAssist.ReadValue("CG2_SetBankMoney_Query",retMessage[1],256);
		logFile.WriteLog(retMessage[0],m_UserName,retMessage[1]);//将操作类型写入日志文件
		
		int result=0;

		char BankMoeny[256];
		ZeroMemory(BankMoeny,256);
		sprintf(BankMoeny,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Money,0)).lpdata);//获得道具名称


		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);//获取添加道具sql语句
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,20,BankMoeny,0,"",cg2_usersn);//格式化sql语句
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		int dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		int tmpLength=0;
		if(dbid==1)//如果玩家在第一个数据库中
		{
			//构造对第一个数据库进行操作的sql语句
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		}
		else if(dbid==2)//如果玩家在第二个数据库中
		{
			//构造对第二个数据库进行操作的sql语句
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,11);
		}

		if(tmpLength>0)//操作成功
		{
				//maple add///重新再连接一次数据库
 				Sleep(30000);
					
				operAssist.getRemoteSql("CG2_getResult",operRemoteSql);//获取返回结果sql语句
				sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,20,BankMoeny,cg2_usersn);//格式化sql语句
				char gm_result[2];
				ZeroMemory(gm_result,2);
				operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
				operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,gm_result);
				if(!strcmp(gm_result,"2"))
				{
					GetPrivateProfileString("CG2","Add_PresentItem_Fail",NULL,RetMessage,30,filepath);//保存单个返回信息
					operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_SetBankMoney_Query_Resp,RetMessage);//返回信息
					char LogContent[128];
					ZeroMemory(LogContent,128);
					//写入日志文件
					sprintf(LogContent,"%s,In Game Concerto Gate,%s Add PresentItem %s Fail",curTime,m_UserName,cg2_username);
					logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query_Query",cg2_serverip,LogContent);
				}
				else
				{
					GetPrivateProfileString("CG2","CG2_SetBankMoney_Query_Success",NULL,RetMessage,30,filepath);//保存单个返回信息
					operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_SetBankMoney_Query_Resp,RetMessage);//返回信息
					char LogContent[128];
					ZeroMemory(LogContent,128);
					//写入日志文件
					sprintf(LogContent,"%s,In Game Concerto Gate,%s Add PresentItem %s Success",curTime,m_UserName,cg2_username);
					logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query",cg2_serverip,LogContent);
				}
		}
		else
		{
			GetPrivateProfileString("CG2","Add_PresentItem_Fail",NULL,RetMessage,30,filepath);//保存单个返回信息
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Item_Query_Resp,RetMessage);//返回信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//写入日志文件
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Add Present Item %s Fail",curTime,m_UserName,cg2_username);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query",cg2_serverip,LogContent);
		}
	}
	catch(...)
	{
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Item_Query_Resp,"ERROR");//返回修改职业错误信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//写入日志文件
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Add PresentItem %s Fail",curTime,m_UserName,cg2_serverip);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query",cg2_serverip,LogContent);
	}
	return true;
}
//银行物品信息
bool CCG2Info::CG2_BankItem_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"帐号<%s>查询大区<%s>玩家<%s>银行物品信息",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("魔力宝贝2",strLog);
		
		operAssist.getRemoteSql("CG2_BANKITEM_QUERY",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BankItem_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		sprintf(operszSql,operRemoteSql,cg2_username);
		int tmpLength = 0;
		if(dbid==1)//玩家帐号在第一个数据库中
		{
			tmpLength =DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//构造从数据库查询得到的数据
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//构造从数据库查询得到的数据
			
		}
		
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BankItem_Query_Resp, &DBVect, tmpLength);
		
		
		
		
	}
	catch (...)
	{
		
	}
	return true;
}

//删除银行道具
bool CCG2Info::CG2_BankItem_Del_Query()
{

	try
	{
		int tmpLength=0;
		int dbid=0;
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_BagItem_Del_Query");//将操作类型写入日志文件
		
		char strResult[256];
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid=operAssist.GetDBID(cg2_serverip,cg2_username);
		
		
		char pame[40];
		char itemname[40];
		
		sprintf(pame,"%s %s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SlotNum,0)).lpdata,
			(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetItemID,0)).lpdata);
		sprintf(itemname,"%s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemName,0)).lpdata);
		
		
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);	
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,33,pame,0,"",cg2_usersn);//格式化sql语句
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//构造从数据库查询得到的数据
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//构造从数据库查询得到的数据
		}
		if(tmpLength>0)
		{
			Sleep(30000);
			sprintf(strResult,"账号%s角色名%s删除银行道具:卡槽%s删除道具%s成功",cg2_username,cg2_usersn,
				(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SlotNum,0)).lpdata,
				itemname);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BankItem_Del_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_Recovery_Account",cg2_serverip,strResult);
			
			
		}
		else
		{
			sprintf(strResult,"账号%s角色名%s删除银行道具:卡槽%s删除道具%s失败",cg2_username,cg2_usersn,
				(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SlotNum,0)).lpdata,
				itemname);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BankItem_Del_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_Recovery_Account",cg2_serverip,strResult);
		}
		
	}
	catch (...)
	{
		
	}
	
	return true;

}

//银行宠物信息
bool CCG2Info::CG2_BankPet_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"帐号<%s>查询大区<%s>玩家<%s>银行宠物信息",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("魔力宝贝2",strLog);
		
		operAssist.getRemoteSql("CG2_BANKPET_QUERY",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BankPet_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		sprintf(operszSql,operRemoteSql,cg2_username);
		int tmpLength = 0;
		if(dbid==1)//玩家帐号在第一个数据库中
		{
			tmpLength =DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//构造从数据库查询得到的数据
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//构造从数据库查询得到的数据
			
		}
		
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BankPet_Query_Resp, &DBVect, tmpLength);
		
		
		
		
	}
	catch (...)
	{
		
	}
	return true;
}


//修改宠物属性
bool CCG2Info::CG2_Update_Pet_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_Update_Pet_Query");//将操作类型写入日志文件
		
		int result=0;
		char petID[256];
		ZeroMemory(petID,256);
		sprintf(petID,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetID,0)).lpdata);
		
		char petName[256];
		ZeroMemory(petName,256);
		sprintf(petName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetSrcName,0)).lpdata);


		
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);//获取修改玩家角色职业sql语句
		int tmpLength=0;
		

		char attr0[256];
		char attr1[256];
		char attr2[256];
		char attr3[256];
		char attr4[256];
		ZeroMemory(attr0,256);
		ZeroMemory(attr1,256);
		ZeroMemory(attr2,256);
		ZeroMemory(attr3,256);
		ZeroMemory(attr4,256);
		sprintf(attr0,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetPhysical,0)).lpdata);
		sprintf(attr1,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetPower,0)).lpdata);
		sprintf(attr2,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetSmart,0)).lpdata);
		sprintf(attr3,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetSpeed,0)).lpdata);
		sprintf(attr4,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetSpirit,0)).lpdata);


		char pame[256];
		sprintf(pame,"%s vtl %s str %s dex %s qui %s mnd %s",petID,attr0,attr1,attr2,attr3,attr4);

		
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,21,pame,0,"",cg2_usersn);//格式化sql语句
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		if(tmpLength>0)//操作成功
		{							
			Sleep(30000);
			sprintf(RetMessage,"用户名%s修改宠物%s新体力%s,新力量%s,新灵巧%s,新速度%s,新精神%s成功",cg2_username,petName,attr0,attr1,attr2,attr3,attr4);							
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Update_Pet_Query_Resp,RetMessage);//返回信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//写入日志文件
			sprintf(LogContent,"%s,魔力宝贝2,用户名%s修改宠物%s新体力%s,新力量%s,新灵巧%s,新速度%s,新精神%s成功",curTime,cg2_username,petName,attr0,attr1,attr2,attr3,attr4);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_AddPet_Query",cg2_serverip,LogContent);
			
		}
		else
		{
			sprintf(RetMessage,"用户名%s修改宠物%s新体力%s,新力量%s,新灵巧%s,新速度%s,新精神%s失败",cg2_username,petName,attr0,attr1,attr2,attr3,attr4);	
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Update_Pet_Query_Resp,RetMessage);//返回信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//写入日志文件
			sprintf(LogContent,"%s,魔力宝贝2,用户名%s修改宠物%s新体力%s,新力量%s,新灵巧%s,新速度%s,新精神%s失败",curTime,cg2_username,petName,attr0,attr1,attr2,attr3,attr4);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_AddPet_Query",cg2_serverip,LogContent);
			
			
		}
	}
	catch(...)
	{
		
		operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Update_Pet_Query_Resp,"ERROR");//返回修改职业错误信息
		char LogContent[128];
		ZeroMemory(LogContent,128);
		//写入日志文件
		sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill %s Fail",curTime,m_UserName,cg2_username);
		logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
	}
	return true;
}

//添加宠物
bool CCG2Info::CG2_AddPet_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_AddPet_Query");//将操作类型写入日志文件
		
		int result=0;
		char petID[256];
		ZeroMemory(petID,256);
		sprintf(petID,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetID,0)).lpdata);///新公会名

		char petName[256];
		ZeroMemory(petName,256);
		sprintf(petName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetSrcName,0)).lpdata);///新公会名

		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);//获取修改玩家角色职业sql语句
		int tmpLength=0;

		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,22,petID,0,"",cg2_usersn);//格式化sql语句
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		if(tmpLength>0)//操作成功
		{							
			Sleep(30000);
			sprintf(RetMessage,"用户名%s添加宠物%s成功",cg2_username,petName);							
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_AddPet_Query_Resp,RetMessage);//返回信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//写入日志文件
			sprintf(LogContent,"%s,魔力宝贝2,用户名%s添加宠物%s成功",curTime,cg2_username,petName);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_AddPet_Query",cg2_serverip,LogContent);
			
		}
		else
		{
			sprintf(RetMessage,"用户名%s添加宠物%s失败",cg2_username,petName);	
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_AddPet_Query_Resp,RetMessage);//返回信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//写入日志文件
			sprintf(LogContent,"%s,魔力宝贝2,用户名%s添加宠物%s失败",curTime,cg2_username,petName);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_AddPet_Query",cg2_serverip,LogContent);
			
			
		}
	}
	catch(...)
	{
		
		operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_RenameGuild_Query_Resp,"ERROR");//返回修改职业错误信息
		char LogContent[128];
		ZeroMemory(LogContent,128);
		//写入日志文件
		sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill %s Fail",curTime,m_UserName,cg2_username);
		logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
	}
	return true;
}

//修改宠物等级
bool CCG2Info::CG2_UpdatePetLevel_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_UpdatePetLevel_Query");//将操作类型写入日志文件
		int result=0;
		char petID[256];
		ZeroMemory(petID,256);
		sprintf(petID,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetID,0)).lpdata);
		
		char petName[256];
		ZeroMemory(petName,256);
		sprintf(petName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetSrcName,0)).lpdata);
		
		
		
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);//获取修改玩家角色职业sql语句
		int tmpLength=0;
		
		
		char petLevel[256];
		ZeroMemory(petLevel,256);

		sprintf(petLevel,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Level,0)).lpdata);
		
		
		char pame[256];
		sprintf(pame,"%s %s",petID,petLevel);
		
		
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,25,pame,0,"",cg2_usersn);//格式化sql语句
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		if(tmpLength>0)//操作成功
		{							
			Sleep(30000);
			sprintf(RetMessage,"用户名%s修改宠物%s新等级%s成功",cg2_username,petName,petLevel);							
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UpdatePetLevel_Query_Resp,RetMessage);//返回信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//写入日志文件
			sprintf(LogContent,"%s,魔力宝贝2,用户名%s修改宠物%s新等级%s成功",curTime,cg2_username,petName,petLevel);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_AddPet_Query",cg2_serverip,LogContent);
			
		}
		else
		{
			sprintf(RetMessage,"用户名%s修改宠物%s新等级%s失败",cg2_username,petName,petLevel);	
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UpdatePetLevel_Query_Resp,RetMessage);//返回信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//写入日志文件
			sprintf(LogContent,"%s,魔力宝贝2,用户名%s新等级%s失败",curTime,cg2_username,petName,petLevel);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_AddPet_Query",cg2_serverip,LogContent);
			
		}
	}
	catch(...)
	{
		
		operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UpdatePetLevel_Query_Resp,"ERROR");//返回修改职业错误信息
		char LogContent[128];
		ZeroMemory(LogContent,128);
		//写入日志文件
		sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill %s Fail",curTime,m_UserName,cg2_username);
		logFile.WriteDBLog(userByID,"Concerto Gate","CG2_UpdatePetLevel_Query",cg2_serverip,LogContent);
	}
	return true;
}
//添加宠物技能
bool CCG2Info::CG2_Insert_Pet_Skill_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_Insert_Pet_Skill_Query");//将操作类型写入日志文件
		int result=0;
		char petID[256];
		ZeroMemory(petID,256);
		sprintf(petID,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetID,0)).lpdata);
		
		char petName[256];
		ZeroMemory(petName,256);
		sprintf(petName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetSrcName,0)).lpdata);
		
		
		
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);//获取修改玩家角色职业sql语句
		int tmpLength=0;
		
		
		

		char petSkillName[256];
		ZeroMemory(petSkillName,256);
		sprintf(petSkillName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SkillName,0)).lpdata);

		char petSkillID[256];
		ZeroMemory(petSkillID,256);

		char procName[256];
		ZeroMemory(procName,256);
		sprintf(procName,"exec CG2_SkillID_Query '%s'",petSkillName);
		
		operAssist.CG2_Proc_Query(logdbip,4,procName,petSkillID);////根据技能名称获得技能ID

		char pame[256];
		sprintf(pame,"%s %s",petID,petSkillID);
		
		
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,15,pame,0,"",cg2_usersn);//格式化sql语句
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		if(tmpLength>0)//操作成功
		{							
			Sleep(30000);
			sprintf(RetMessage,"用户名%s角色名%s添加宠物%s新技能%s成功",cg2_username,cg2_usersn,petName,petSkillName);							
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Pet_Skill_Query_Resp,RetMessage);//返回信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//写入日志文件
			sprintf(LogContent,"%s,魔力宝贝2,用户名%s角色名%s添加宠物%s新技能%s成功",curTime,cg2_username,cg2_usersn,petName,petSkillName);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_AddPet_Query",cg2_serverip,LogContent);
			
		}
		else
		{
			sprintf(RetMessage,"用户名%s角色名%s添加宠物%s新技能%s失败",cg2_username,cg2_usersn,petName,petSkillName);	
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Pet_Skill_Query_Resp,RetMessage);//返回信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//写入日志文件
			sprintf(LogContent,"%s,魔力宝贝2,用户名%s角色名%s添加宠物%s新技能%s失败",curTime,cg2_username,cg2_usersn,petName,petSkillName);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_AddPet_Query",cg2_serverip,LogContent);
			
		}
	}
	catch(...)
	{
		
		operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Pet_Skill_Query_Resp,"ERROR");//返回修改职业错误信息
		char LogContent[128];
		ZeroMemory(LogContent,128);
		//写入日志文件
		sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill %s Fail",curTime,m_UserName,cg2_username);
		logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Pet_Skill_Query",cg2_serverip,LogContent);
	}
	return true;
}

//修改宠物技能
bool CCG2Info::CG2_Update_Pet_Skill_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_Update_Pet_Skill_Query");//将操作类型写入日志文件
		int result=0;
		char petID[256];
		ZeroMemory(petID,256);
		sprintf(petID,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetID,0)).lpdata);
		
		char petName[256];
		ZeroMemory(petName,256);
		sprintf(petName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetSrcName,0)).lpdata);
		
		
		
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);//获取修改玩家角色职业sql语句
		int tmpLength=0;
		
		char petSkillName[256];
		ZeroMemory(petSkillName,256);
		sprintf(petSkillName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SkillName,0)).lpdata);

		char petSkillLevel[256];
		ZeroMemory(petSkillLevel,256);
		sprintf(petSkillLevel,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SkillLevel,0)).lpdata);

		char petSkillID[256];
		ZeroMemory(petSkillID,256);
		
		char procName[256];
		ZeroMemory(procName,256);
		sprintf(procName,"exec CG2_SkillID_Query '%s'",petSkillName);
		
		operAssist.CG2_Proc_Query(logdbip,4,procName,petSkillID);////根据技能名称获得技能ID
		
		char pame[256];
		sprintf(pame,"%s %s %s",petID,petSkillID,petSkillLevel);
		
		
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,17,pame,0,"",cg2_usersn);//格式化sql语句
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);

		int iSkillLevel=StrToInt(petSkillLevel)+1;
		if(tmpLength>0)//操作成功
		{							
			Sleep(30000);
			sprintf(RetMessage,"用户名%s角色名%s修改宠物%s技能%s新等级%i成功",cg2_username,cg2_usersn,petName,petSkillName,iSkillLevel);							
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Update_Pet_Skill_Query_Resp,RetMessage);//返回信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//写入日志文件
			sprintf(LogContent,"%s,魔力宝贝2,用户名%s角色名%s修改宠物%s技能%s新等级%i成功",curTime,cg2_username,cg2_usersn,petName,petSkillName,iSkillLevel);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_AddPet_Query",cg2_serverip,LogContent);
			
		}
		else
		{
			sprintf(RetMessage,"用户名%s角色名%s修改宠物%s技能%s新等级%i失败",cg2_username,cg2_usersn,petName,petSkillName,iSkillLevel);	
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Update_Pet_Skill_Query_Resp,RetMessage);//返回信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//写入日志文件
			sprintf(LogContent,"%s,魔力宝贝2,用户名%s角色名%s修改宠物%s技能%s新等级%i失败",curTime,cg2_username,cg2_usersn,petName,petSkillName,iSkillLevel);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_AddPet_Query",cg2_serverip,LogContent);
			
		}
	}
	catch(...)
	{
		
		operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Update_Pet_Skill_Query_Resp,"ERROR");//返回修改职业错误信息
		char LogContent[128];
		ZeroMemory(LogContent,128);
		//写入日志文件
		sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill %s Fail",curTime,m_UserName,cg2_username);
		logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Pet_Skill_Query",cg2_serverip,LogContent);
	}
	return true;
}
//宠物图鉴信息
bool CCG2Info::CG2_PetPic_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"帐号<%s>查询大区<%s>玩家<%s>宠物图鉴信息",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("魔力宝贝2",strLog);
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		operAssist.getRemoteSql("CG2_PetPic_Query",operRemoteSql);//构造根据IP查询登录记录的sql语句
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_PetPic_Query_Resp, "数据库中找不到对应的sql语句");
			return false;
		}
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		sprintf(operszSql,operRemoteSql,cg2_usersn);
		int tmpLength =0;
		if(dbid==1)//玩家帐号在第一个数据库中
		{
			
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//构造从数据库查询得到的数据
			
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//构造从数据库查询得到的数据
		}
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_PetPic_Query_Resp, &DBVect, tmpLength);

		
		
		
	}
	catch (...)
	{
		
	}
	return true;
}
//设置/删除宠物图鉴
bool CCG2Info::CG2_PetPic_Del_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_PetPic_Del_Query");//将操作类型写入日志文件
		int result=0;
		char petID[256];
		ZeroMemory(petID,256);
		sprintf(petID,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetID,0)).lpdata);
		
		char pictype[256];
		ZeroMemory(pictype,256);
		sprintf(pictype,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_TypeID,0)).lpdata);
		
		
		
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);//获取修改玩家角色职业sql语句
		int tmpLength=0;
		
		
		
		char pame[256];
		sprintf(pame,"%s %s",petID,pictype);
		
		
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,34,pame,0,"",cg2_usersn);//格式化sql语句
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		
		if(tmpLength>0)//操作成功
		{							
			Sleep(30000);
			sprintf(RetMessage,"操作成功");							
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_PetPic_Del_Query_Resp,RetMessage);//返回信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//写入日志文件
		//	sprintf(LogContent,"%s,魔力宝贝2,用户名%s角色名%s修改宠物%s技能%s新等级%i成功",curTime,cg2_username,cg2_usersn,petName,petSkillName,iSkillLevel);
		//	logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_AddPet_Query",cg2_serverip,LogContent);
			
		}
		else
		{
		//	sprintf(RetMessage,"用户名%s角色名%s修改宠物%s技能%s新等级%i失败",cg2_username,cg2_usersn,petName,petSkillName,iSkillLevel);	
		//	operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_PetPic_Del_Query_Resp,RetMessage);//返回信息
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//写入日志文件
		//	sprintf(LogContent,"%s,魔力宝贝2,用户名%s角色名%s修改宠物%s技能%s新等级%i失败",curTime,cg2_username,cg2_usersn,petName,petSkillName,iSkillLevel);
		//	logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_AddPet_Query",cg2_serverip,LogContent);
			
		}
	}
	catch(...)
	{
		
		operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_PetPic_Del_Query_Resp,"ERROR");//返回修改职业错误信息
		char LogContent[128];
		ZeroMemory(LogContent,128);
		//写入日志文件
		sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill %s Fail",curTime,m_UserName,cg2_username);
		logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Pet_Skill_Query",cg2_serverip,LogContent);
	}
	return true;


}
//添加临时密码
bool CCG2Info::CG2_Insert_TmpPwd_Query()
{
	try
	{
		int dbid=0;
		int tmpLength=0;
		sprintf(strLog,"帐号<%s>查询大区<%s>玩家<%s>添加临时密码",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("魔力宝贝2",strLog);
		char strResult[256];
		char RelPwd[40];
		char sign[40];
		char TempPwd[40];
		ZeroMemory(strResult,256);
		ZeroMemory(RelPwd,40);
		ZeroMemory(sign,40);
		ZeroMemory(TempPwd,40);
		//查询现在密码
		sprintf(operszSql,"exec CG2_SearchTmpPWD_Staute_Query '%s','%s'",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ServerName,0)).lpdata,cg2_username);
		operAssist.CG2_Proc_Query(logdbip,4,operszSql,strResult);
		if(strcmp(strResult,""))//密码存在
		{
			sprintf(strResult,"账号%s临时密码已经设置",cg2_username);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Insert_TmpPwd_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_Recovery_Account",cg2_serverip,strResult);
		}
		else
		{
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			dbid=operAssist.GetDBID(cg2_serverip,cg2_username);
			operAssist.getRemoteSql("CG2_GetPassWord_Query",operRemoteSql);
			sprintf(operszSql,operRemoteSql,cg2_username);
			if(dbid==1)
			{
				operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,RelPwd);
			}
			else if(dbid==2)
			{
				operAssist.CG2_Proc_Query(cg2_serverip,5,operszSql,RelPwd);
			}
			
			sprintf(sign,"%s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_TempPwd,0)).lpdata);
			int newlength=0;
			MD5_EnCode(TempPwd,&newlength,sign,strlen(sign));
			
			operAssist.getRemoteSql("CG2_TmpPassWord_Query",operRemoteSql);
			sprintf(operszSql,operRemoteSql,TempPwd,cg2_username);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
			if(dbid==1)
			{
				tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//构造从数据库查询得到的数据
			}
			else if(dbid==2)
			{
				tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//构造从数据库查询得到的数据
			}
			if(tmpLength>0)
			{
				sprintf(operszSql,"exec CG2_InsertTmpPassWord_Query '%s','%s','%s','%s','%s','%s',%i",
					(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ServerName,0)).lpdata,
					cg2_username,
					cg2_usersn,
					RelPwd,
					TempPwd,
					sign,
					1);
				DBVect.OperVectorMain(logdbip, operszSql, 4);//构造从数据库查询得到的数据

				sprintf(strResult,"账号%s临时密码设置成功",cg2_username);
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Insert_TmpPwd_Query_Resp, strResult);
				operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
				logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_Recovery_Account",cg2_serverip,strResult);


			}
			else
			{
				sprintf(strResult,"账号%s临时密码设置失败",cg2_username);
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Insert_TmpPwd_Query_Resp, strResult);
				operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
				logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_Recovery_Account",cg2_serverip,strResult);
			}
		}


	}
	catch (...)
	{
	}

	return true;
}
//查看临时密码
bool CCG2Info::CG2_SearchPassWord_Query()
{
	try
	{
		sprintf(operszSql,"exec CG2_SearchPassWord_Query '%s','%s'",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ServerName,0)).lpdata,cg2_username);
		int tmpLength=DBVect.OperVectorMain(logdbip, operszSql, 4, 0, 0);//构造从数据库查询得到的数据
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_SearchPassWord_Query_Resp, &DBVect, tmpLength);

	}
	catch (...)
	{
		
	}
	return true;
}
//恢复临时密码
bool CCG2Info::CG2_Recover_TmpPwd_Query()
{
	try
	{
		int dbid=0;
		int tmpLength=0;
		sprintf(strLog,"帐号<%s>查询大区<%s>玩家<%s>恢复临时密码",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("魔力宝贝2",strLog);
		char strResult[256];
		char RelPwd[40];
		char sign[40];
		char TempPwd[40];
		ZeroMemory(strResult,40);
		ZeroMemory(RelPwd,40);
		ZeroMemory(sign,40);
		ZeroMemory(TempPwd,40);
		//查询现在密码
		sprintf(operszSql,"exec CG2_GetRelPassWord_Query '%s','%s'",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ServerName,0)).lpdata,cg2_username);
		operAssist.CG2_Proc_Query(logdbip,4,operszSql,RelPwd);
		if(!strcmp(RelPwd,""))//密码不存在
		{
			sprintf(strResult,"账号%s临时密码未设置",cg2_username);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Recover_TmpPwd_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_Recovery_Account",cg2_serverip,strResult);
		}
		else
		{
			
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			dbid=operAssist.GetDBID(cg2_serverip,cg2_username);
			operAssist.getRemoteSql("CG2_TmpPassWord_Query",operRemoteSql);
			sprintf(operszSql,operRemoteSql,RelPwd,cg2_username);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
			if(dbid==1)
			{
				tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//构造从数据库查询得到的数据
			}
			else if(dbid==2)
			{
				tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//构造从数据库查询得到的数据
			}
			if(tmpLength>0)
			{
				sprintf(operszSql,"exec CG2_UpdateTmpPassWord_Query '%s','%s'",
					(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ServerName,0)).lpdata,cg2_username);
				DBVect.OperVectorMain(logdbip, operszSql, 4);//构造从数据库查询得到的数据
				
				sprintf(strResult,"账号%s临时密码恢复成功",cg2_username);
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Recover_TmpPwd_Query_Resp, strResult);
				operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
				logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_Recovery_Account",cg2_serverip,strResult);
				
				
			}
			else
			{
				sprintf(strResult,"账号%s临时密码恢复失败",cg2_username);
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Recover_TmpPwd_Query_Resp, strResult);
				operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
				logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_Recovery_Account",cg2_serverip,strResult);
			}
		}
		
		
	}
	catch (...)
	{
	}
	return true;
}
//宠物列表信息
bool CCG2Info::CG2_PetList_Query()
{
	try
	{
		sprintf(operszSql,"exec CG2_PetList_Query");
		int tmpLength=DBVect.OperVectorMain(logdbip, operszSql, 4, 0, 0);//构造从数据库查询得到的数据
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_PetList_Query_Resp, &DBVect, tmpLength);

	}
	catch (...)
	{
		
	}
	return true;
}

//角色换位
bool CCG2Info::CG2_ChangeCharNo_Query()
{
	try
	{
		int tmpLength=0;
		int dbid=0;
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_ChangeCharNo_Query");//将操作类型写入日志文件
		
		int iResult=0;
		char strResult[256];
		//恢复角色之前踢人
		bool b_result=CG2_KickAccount_Query_NoAnswer();//踢人
		if(b_result)
		{
			Sleep(30000);//延时30秒等待结果
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			dbid=operAssist.GetDBID(cg2_serverip,cg2_username);
			operAssist.getRemoteSql("CG2_ChangeCharNo_Query",operRemoteSql);

			int cg2_newcharno=StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ChangeCharNo,0)).lpdata);
			sprintf(operszSql,operRemoteSql,cg2_username,cg2_charno,cg2_newcharno);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
			if(dbid==1)
			{
				tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//构造从数据库查询得到的数据
			}
			else if(dbid==2)
			{
				tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//构造从数据库查询得到的数据
			}
			if(tmpLength>0)
			{
							
				sprintf(strResult,"账号%s角色换位成功",cg2_username);
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_ChangeCharNo_Query_Resp, strResult);
				operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
				logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_Recovery_Account",cg2_serverip,strResult);
				
				
			}
			else
			{
				sprintf(strResult,"账号%s角色换位失败",cg2_username);
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_ChangeCharNo_Query_Resp, strResult);
				operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
				logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_Recovery_Account",cg2_serverip,strResult);
			}

			
		}
	}
	catch (...)
	{
		
	}
	
	return true;
}
//移动角色坐标
bool CCG2Info::CG2_ChangeMap_Query()
{
	try
	{
		int tmpLength=0;
		int dbid=0;
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_ChangeMap_Query");//将操作类型写入日志文件
		
		int iResult=0;
		char strResult[256];
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid=operAssist.GetDBID(cg2_serverip,cg2_username);
		
		
		char pame[40];
		char MapX[40];
		char MapY[40];
		char mapCoord[40];
		char MapID[40];
		sprintf(MapID,"%s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_MapID,0)).lpdata);
		sprintf(mapCoord,"%s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Coordinate,0)).lpdata);
		char *lpStr=strtok((char *)&mapCoord,",");//查询标志","
		sprintf(MapX,lpStr);
		lpStr=strtok(NULL,",");
		sprintf(MapY,lpStr);
		if(StrToInt(MapX)>0)
		{
		}
		else
		{
			sprintf(operszSql,"exec CG2_SearchMapID_Query '%s'",MapID);
			operAssist.CG2_Proc_Query(logdbip,4,operszSql,mapCoord);
			lpStr=strtok(mapCoord,",");//查询标志","
			sprintf(MapX,lpStr);
			lpStr=strtok(NULL,",");
			sprintf(MapY,lpStr);
		}

		sprintf(pame,"%s %s %s",MapID,MapX,MapY);

		
		
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);	
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,0,pame,0,"",cg2_usersn);//格式化sql语句
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//构造从数据库查询得到的数据
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//构造从数据库查询得到的数据
		}
		if(tmpLength>0)
		{
			Sleep(30000);
			sprintf(strResult,"账号%s移动角色坐标成功",cg2_username);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Change_Map_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_Recovery_Account",cg2_serverip,strResult);
			
				
		}
		else
		{
			sprintf(strResult,"账号%s移动角色坐标失败",cg2_username);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Change_Map_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_Recovery_Account",cg2_serverip,strResult);
		}
			
	}
	catch (...)
	{
		
	}
	
	return true;
}

//查询地图列表
bool CCG2Info::CG2_MapList_Query()
{
	try
	{
		sprintf(operszSql,"exec CG2_MapList_Query");
		int tmpLength=DBVect.OperVectorMain(logdbip, operszSql, 4, 0, 0);//构造从数据库查询得到的数据
		operAssist.ReturnData(CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_MapList_Query_Resp, DBVect.getDes(),tmpLength);
	}
	catch (...)
	{
		
	}
	return true;
}

//模糊查询地图
bool CCG2Info::CG2_SearchMap_Query()
{
	try
	{
		sprintf(operszSql,"exec CG2_SearchMap_Query '%s'",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_MapName,0)).lpdata);
		int tmpLength=DBVect.OperVectorMain(logdbip, operszSql, 4, 0, 0);//构造从数据库查询得到的数据
		operAssist.ReturnData(CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_SearchMap_Query_Resp, DBVect.getDes(),tmpLength);
	}
	catch (...)
	{
		
	}
	return true;
}

//发送问答信息
bool CCG2Info::CG2_SendAsk_Query()
{
	try
	{
		int tmpLength=0;
		int dbid=0;
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_SendAsk_Query");//将操作类型写入日志文件
		
		char strResult[256];
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid=operAssist.GetDBID(cg2_serverip,cg2_username);
		
				
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);	
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,8,"",0,"",cg2_usersn);//格式化sql语句
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//构造从数据库查询得到的数据
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//构造从数据库查询得到的数据
		}
		if(tmpLength>0)
		{
			Sleep(30000);
			sprintf(strResult,"账号%s角色名%s发送问答信息成功",cg2_username,cg2_usersn);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_SendAsk_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_Recovery_Account",cg2_serverip,strResult);
			
			
		}
		else
		{
			sprintf(strResult,"账号%s角色名%s发送问答信息失败",cg2_username,cg2_usersn);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_SendAsk_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"魔力宝贝2","CG2_Recovery_Account",cg2_serverip,strResult);
		}
		
	}
	catch (...)
	{
		
	}
	
	return true;
	
}


//商城信息查询
bool CCG2Info::CG2_UserShoppingInfo_Query()
{
	try
	{
		char begintime[256];
		char lbegintime[256];
		char endtime[256];
		char lendtime[256];
		char strsign[1024];
		char strMD5sign[1024];
		char strItemName[40];
		char procName[256];
		int iTypeID=1;
		
		sprintf(begintime,"%s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_StartTime,0)).lpdata);
		sprintf(endtime,"%s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_EndTime,0)).lpdata);
		iTypeID=StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_TypeID,0)).lpdata);
		struct tm t_BeginTime; //	tm结构体类型的变量，保存时间
		sscanf(begintime,"%d-%d-%d %d:%d:%d",&t_BeginTime.tm_year,&t_BeginTime.tm_mon,&t_BeginTime.tm_mday,&t_BeginTime.tm_hour,&t_BeginTime.tm_min,&t_BeginTime.tm_sec);//将开始时间赋值给tm结构体
		sprintf(lbegintime,"%02d%02d%02d%02d%02d%02d",t_BeginTime.tm_year,t_BeginTime.tm_mon,t_BeginTime.tm_mday,t_BeginTime.tm_hour,t_BeginTime.tm_min,t_BeginTime.tm_sec);
		sscanf(endtime,"%d-%d-%d %d:%d:%d",&t_BeginTime.tm_year,&t_BeginTime.tm_mon,&t_BeginTime.tm_mday,&t_BeginTime.tm_hour,&t_BeginTime.tm_min,&t_BeginTime.tm_sec);//将开始时间赋值给tm结构体
		sprintf(lendtime,"%02d%02d%02d%02d%02d%02d",t_BeginTime.tm_year,t_BeginTime.tm_mon,t_BeginTime.tm_mday,t_BeginTime.tm_hour,t_BeginTime.tm_min,t_BeginTime.tm_sec);
		
		sprintf(strsign,"%s%i%s%s%s%s",cg2_username,iTypeID,lbegintime,lendtime,"1","|Tbds6f.AS_getbuylog_CG2");
		
		int newlength=0;
		ZeroMemory(strMD5sign,1024);
		MD5_EnCode(strMD5sign,&newlength,strsign,strlen(strsign));
		
		char url[256];
		char postdata[2048];
		ZeroMemory(postdata,2048);

		sprintf(url,"%s","http://61.152.150.158/PayCenter/AS_getbuylog_CG2.php");//商城IP固定
		sprintf(postdata,"username=%s&utp=%i&log_start=%s&log_end=%s&datetype=1&sign=%s&encoding=UTF-8",cg2_username,iTypeID,lbegintime,lendtime,strMD5sign);
		
		vector<CGlobalStruct::DataRecord> ret_vector;
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;
		int iCow=0,iCol=0;
	
		if(operAssist.ReadXml("POST",url,postdata,&ret_vector,&iCow,&iCol))
		{
			COperVector sendopervector;
			sendopervector.InitCreate(iCow+1,iCol+3);
			
			CEnumCore::TagName m_TagName;//保存获得的TagName
			CEnumCore::TagFormat m_TagFormat;//保存获得的TagFormat
			
			CMyEvent m_Event;//获取CEvent对象
			
			//获取查询的记录集的每一项
				
			for( iter = ret_vector.begin(); iter != ret_vector.end(); iter++ )
			{
				m_DataRecord = *iter;
				if(!strcmp(m_DataRecord.recordfieldname,"PKEY"))
				{
					
				}
				else if(!strcmp(m_DataRecord.recordfieldname,"PCODE"))
				{
					sprintf(procName,"exec CG2_ItemName_query %i",StrToInt((char *)&m_DataRecord.recorddata));//初始化存储过程的语句
					operAssist.CG2_Proc_Query(logdbip,4,procName,strItemName);
					sendopervector.AddTFLV(CEnumCore::TagName::CG2_ItemName, CEnumCore::TagFormat::TLV_STRING, strlen(strItemName), (unsigned char*)&strItemName);//构造TFLV,并添加到vector中
				}
				else
				{
					m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//获取记录集数据的TagName
					m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//获取记录集数据的TagFormat
					sendopervector.AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//构造TFLV,并添加到vector中
				}
				
			}
			
			operAssist.ReturnData(m_hServerSocket,&sendopervector,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UserShoppingInfo_Query_Resp,index,iPageSize);
			sendopervector.Destroy();

		}
		else
		{
			iter = ret_vector.begin();
			m_DataRecord = *iter;
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UserShoppingInfo_Query_Resp,m_DataRecord.recorddata);
		}
	
		
		if(!ret_vector.empty())
		{
			ret_vector.clear();
		}
		

	}
	catch (...)
	{
		
	}
	return true;
}

//发送道具密码锁
bool CCG2Info::CG2_Send_ItemPassword()
{
	try
	{
		char strRecv[256];
		ZeroMemory(strRecv,256);
		sprintf(strRecv,"%s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Email,0)).lpdata);
		char strPasswd[256];
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		int dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//获取玩家帐号所处的数据库ID
		operAssist.getRemoteSql("CG2_getItemPass",operRemoteSql);
		sprintf(operszSql,operRemoteSql,cg2_username);
		if(dbid==1)
		{
			operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,strPasswd);
		}
		else if(dbid==1)
		{
			operAssist.CG2_Proc_Query(cg2_serverip,5,operszSql,strPasswd);
		}
		




		char strMessage[1024];
		ZeroMemory(strMessage,1024);
		operAssist.ReadValue("CG2_Body",strMessage,1024);
		char tmpcontext[256];
		char context[2048];
		sprintf(tmpcontext,"您的魔力宝贝II道具锁密码是%s",strPasswd);
		sprintf(context,strMessage,cg2_username,tmpcontext);
		if(operAssist.SendEmail(strRecv,context))
		{
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Send_ItemPassword_Resp,"发送成功");
		}
		else
		{
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Send_ItemPassword_Resp,"发送失败");

		}

		
	}
	catch (...)
	{
		
	}
	return true;
}

//等到Email地址
bool CCG2Info::CG2_Get_EmailAddress()
{
	try
	{

		char strsign[1024];
		char strMD5sign[1024];
		char strItemName[40];
		char procName[256];
		int iTypeID=1;
		
		
		sprintf(strsign,"%s%s",cg2_username,"QUXUEXIBA");
		
		int newlength=0;
		ZeroMemory(strMD5sign,1024);
		MD5_EnCode(strMD5sign,&newlength,strsign,strlen(strsign));
		
		char url[256];
		

		sprintf(url,"http://222.73.121.86:8080/user?req=getUserInfo&id=GMTOOLS&userid=%s&usertype=0&type=userinfo&s=%s",cg2_username,strMD5sign);//商城IP固定

		
		vector<CGlobalStruct::DataRecord> ret_vector;
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;
		int iCow=0,iCol=0;
		
		if(operAssist.ReadXmlGet("Get",url,"",&ret_vector,&iCow,&iCol))
		{
			COperVector sendopervector;
			sendopervector.InitCreate(iCow+1,iCol+3);
			
			CEnumCore::TagName m_TagName;//保存获得的TagName
			CEnumCore::TagFormat m_TagFormat;//保存获得的TagFormat
			
			CMyEvent m_Event;//获取CEvent对象
			
			//获取查询的记录集的每一项
			
			for( iter = ret_vector.begin(); iter != ret_vector.end(); iter++ )
			{
				m_DataRecord = *iter;
				if(!strcmp(m_DataRecord.recordfieldname,"PKEY"))
				{
					
				}
				else if(!strcmp(m_DataRecord.recordfieldname,"PCODE"))
				{
					sprintf(procName,"exec CG2_ItemName_query %i",StrToInt((char *)&m_DataRecord.recorddata));//初始化存储过程的语句
					operAssist.CG2_Proc_Query(logdbip,4,procName,strItemName);
					sendopervector.AddTFLV(CEnumCore::TagName::CG2_ItemName, CEnumCore::TagFormat::TLV_STRING, strlen(strItemName), (unsigned char*)&strItemName);//构造TFLV,并添加到vector中
				}
				else
				{
					m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//获取记录集数据的TagName
					m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//获取记录集数据的TagFormat
					sendopervector.AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//构造TFLV,并添加到vector中
				}
				
			}
			
			operAssist.ReturnData(m_hServerSocket,&sendopervector,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Get_EmailAddress_Resp,index,iPageSize);
			sendopervector.Destroy();
			
		}
		else
		{
			iter = ret_vector.begin();
			m_DataRecord = *iter;
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Get_EmailAddress_Resp,m_DataRecord.recorddata);
		}
		
		
		if(!ret_vector.empty())
		{
			ret_vector.clear();
		}
		
		
	}
	catch (...)
	{
		
	}
	return true;
}
////账号激活查询
bool CCG2Info::CG2_Account_Active_Query()
{

	try
	{

		char strsign[1024];
		char strMD5sign[1024];
		sprintf(strsign,"%s%s",cg2_username,"|T4pb3A.Querycg2User");
		
		int newlength=0;
		ZeroMemory(strMD5sign,1024);
		MD5_EnCode(strMD5sign,&newlength,strsign,strlen(strsign));
		
		char url[256];
		char postdata[2048];
		ZeroMemory(postdata,2048);
		
		sprintf(url,"%s","http://61.152.150.158/PayCenter/Query_cg2User.php");//商城IP固定
		sprintf(postdata,"getuser=%s&sign=%s&encoding=UTF-8",cg2_username,strMD5sign);
		
		vector<CGlobalStruct::DataRecord> ret_vector;
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;
		int iCow=0,iCol=0;
		
		if(operAssist.ReadXml("POST",url,postdata,&ret_vector,&iCow,&iCol))
		{
			COperVector sendopervector;
			sendopervector.InitCreate(iCow+1,iCol+3);
			
			CEnumCore::TagName m_TagName;//保存获得的TagName
			CEnumCore::TagFormat m_TagFormat;//保存获得的TagFormat
			
			CMyEvent m_Event;//获取CEvent对象
			
			//获取查询的记录集的每一项
			
			for( iter = ret_vector.begin(); iter != ret_vector.end(); iter++ )
			{
				m_DataRecord = *iter;
				if(!strcmp(m_DataRecord.recordfieldname,"gold"))
				{
					if(!strcmp(m_DataRecord.recorddata,""))
					{
						sprintf(m_DataRecord.recorddata,"%s","0");
					}
					m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//获取记录集数据的TagName
					m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//获取记录集数据的TagFormat
					sendopervector.AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//构造TFLV,并添加到vector中

				}
				else 
				{
					m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//获取记录集数据的TagName
					m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//获取记录集数据的TagFormat
					sendopervector.AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//构造TFLV,并添加到vector中
				}
				
			}
			
			operAssist.ReturnData(m_hServerSocket,&sendopervector,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Account_Active_Query_Resp,index,iPageSize);
			sendopervector.Destroy();
			
		}
		else
		{
			iter = ret_vector.begin();
			m_DataRecord = *iter;
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Account_Active_Query_Resp,m_DataRecord.recorddata);
		}
		
		
		if(!ret_vector.empty())
		{
			ret_vector.clear();
		}
		
		
	}
	catch (...)
	{
		
	}
	return true;
}

//通过激活码查询
bool CCG2Info::CG2_Sn_Active_Query()
{
	try
	{
		
		char strsign[1024];
		char strMD5sign[1024];
		sprintf(strsign,"%s%s",cg2_username,"|T4pb5A.Querycg2Code");
		
		int newlength=0;
		ZeroMemory(strMD5sign,1024);
		MD5_EnCode(strMD5sign,&newlength,strsign,strlen(strsign));
		
		char url[256];
		char postdata[2048];
		ZeroMemory(postdata,2048);
		
		sprintf(url,"%s","http://61.152.150.158/PayCenter/Querycg2Code.php");//商城IP固定
		sprintf(postdata,"getcode=%s&sign=%s&encoding=UTF-8",cg2_username,strMD5sign);
		
		vector<CGlobalStruct::DataRecord> ret_vector;
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;
		int iCow=0,iCol=0;
		
		if(operAssist.ReadXml("POST",url,postdata,&ret_vector,&iCow,&iCol))
		{
			COperVector sendopervector;
			sendopervector.InitCreate(iCow+1,iCol+3);
			
			CEnumCore::TagName m_TagName;//保存获得的TagName
			CEnumCore::TagFormat m_TagFormat;//保存获得的TagFormat
			
			CMyEvent m_Event;//获取CEvent对象
			
			//获取查询的记录集的每一项
			
			for( iter = ret_vector.begin(); iter != ret_vector.end(); iter++ )
			{
				m_DataRecord = *iter;
				if(!strcmp(m_DataRecord.recordfieldname,"gold"))
				{
					if(!strcmp(m_DataRecord.recorddata,""))
					{
						sprintf(m_DataRecord.recorddata,"%s","0");
					}
					m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//获取记录集数据的TagName
					m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//获取记录集数据的TagFormat
					sendopervector.AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//构造TFLV,并添加到vector中
					
				}
				else 
				{
					m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//获取记录集数据的TagName
					m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//获取记录集数据的TagFormat
					sendopervector.AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//构造TFLV,并添加到vector中
				}
				
			}
			
			operAssist.ReturnData(m_hServerSocket,&sendopervector,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Sn_Active_Query_Resp,index,iPageSize);
			sendopervector.Destroy();
			
		}
		else
		{
			iter = ret_vector.begin();
			m_DataRecord = *iter;
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Sn_Active_Query_Resp,m_DataRecord.recorddata);
		}
		
		
		if(!ret_vector.empty())
		{
			ret_vector.clear();
		}
		
		
	}
	catch (...)
	{
		
	}
	return true;
}

int CCG2Info::CG2_GMInsert_Query(char *ServerIP, char *UserName,char * UserNick,char * strEndTime,char * Reason,char * memo,int iFunc)
{

	//sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,1,strBanTime,0,BanContent,cg2_usersn);//格式化sql语句封角色
	//sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,24,strBanTime,0,BanContent,cg2_usersn);//格式化sql语句封账号
	//sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,2,"",0,UnbindContent,cg2_usersn);//格式化sql语句解角色
	//sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,24,"0",0,UnbindContent,"");//格式化sql语句解账号
	//sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,24,"",0,"",cg2_usersn);//格式化sql语句踢帐号
	//sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,1,"",0,"",cg2_usersn);//格式化sql语句踢角色
	char curTime[256];
	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);//获取系统当前时间
	sprintf(curTime,"%i-%i-%i %i:%i:%i",nowtime.wYear,nowtime.wMonth,nowtime.wDay,nowtime.wHour,nowtime.wMinute,nowtime.wSecond);//给当前时间赋值
	char usersn[40];
	char charno[40];

	int tmpLength=0;
	operAssist.initialize();
	operAssist.FindDBIP(ServerIP,ServerIP,2);
	int  dbid = operAssist.GetDBID(ServerIP,UserName);//获取玩家帐号所处的数据库ID
	if(!strcmp(memo,"1"))//账号
	{
		if(dbid==1)//如果玩家在第一个数据库中
		{
			sprintf(operszSql,"select IDKey from tbl_charBase where cdkey='%s'",UserName);
			operAssist.CG2_Proc_Query(ServerIP,3,operszSql,usersn);
			sprintf(operszSql,"select charno from tbl_charBase where cdkey='%s'",UserName);
			operAssist.CG2_Proc_Query(ServerIP,3,operszSql,charno);
			
			
		}
		else if(dbid==2)//如果玩家在第二个数据库中
		{
			sprintf(operszSql,"select IDKey from tbl_charBase where cdkey='%s' ",UserName);
			operAssist.CG2_Proc_Query(ServerIP,5,operszSql,usersn);
			sprintf(operszSql,"select charno from tbl_charBase where cdkey='%s' ",UserName);
			operAssist.CG2_Proc_Query(ServerIP,5,operszSql,charno);
		}
	}
	else
	{
		if(dbid==1)//如果玩家在第一个数据库中
		{
			sprintf(operszSql,"select IDKey from tbl_charBase where cdkey='%s' and name='%s'",UserName,UserNick);
			operAssist.CG2_Proc_Query(ServerIP,3,operszSql,usersn);
			sprintf(operszSql,"select charno from tbl_charBase where cdkey='%s' and name='%s'",UserName,UserNick);
			operAssist.CG2_Proc_Query(ServerIP,3,operszSql,charno);
			
			
		}
		else if(dbid==2)//如果玩家在第二个数据库中
		{
			sprintf(operszSql,"select IDKey from tbl_charBase where cdkey='%s' and name='%s'",UserName,UserNick);
			operAssist.CG2_Proc_Query(ServerIP,5,operszSql,usersn);
			sprintf(operszSql,"select charno from tbl_charBase where cdkey='%s' and name='%s'",UserName,UserNick);
			operAssist.CG2_Proc_Query(ServerIP,5,operszSql,charno);
		}
	}
	
	


	try
	{
		operAssist.getRemoteSql("CG2_GMInsert_Query",operRemoteSql);//构造根据IP查询登录记录的sql语句
		switch(iFunc)
		{
		case 0://踢人
			{
				if(!strcmp(memo,"1"))//账号
				{

					sprintf(operszSql,operRemoteSql,curTime,UserName,charno,24,"",0,"",usersn);
				}
				else
				{
					sprintf(operszSql,operRemoteSql,curTime,UserName,charno,1,"",0,"",usersn);//格式化sql语句踢角色
				}
			}
			break;
		case 1://封停
			{
				if(!strcmp(memo,"1"))//账号
				{
					sprintf(operszSql,operRemoteSql,curTime,UserName,charno,24,strEndTime,0,Reason,usersn);
				}
				else
				{
					sprintf(operszSql,operRemoteSql,curTime,UserName,charno,1,strEndTime,0,Reason,usersn);
				}
			}
			break;
		case 2://解封
			{
				if(!strcmp(memo,"1"))//账号
				{
					sprintf(operszSql,operRemoteSql,curTime,UserName,charno,24,"0",0,Reason,"");
				}
				else
				{
					sprintf(operszSql,operRemoteSql,curTime,UserName,charno,2,"",0,Reason,usersn);
				}
			}
			break;
		default:
			break;
		}
		

		operAssist.FindDBIP(ServerIP,ServerIP,1);
		if(dbid==1)//如果玩家在第一个数据库中
		{
			tmpLength = DBVect.OperVectorMain("CG2",ServerIP, operszSql, 9);//执行远程数据操作
			
		}
		else if(dbid==2)//如果玩家在第二个数据库中
		{
			tmpLength = DBVect.OperVectorMain("CG2",ServerIP, operszSql, 11);//执行远程数据操作
		}
		
	}
	catch(...)
	{
	}
	return tmpLength;
}
