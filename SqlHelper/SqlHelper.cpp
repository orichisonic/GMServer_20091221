// SqlHelper.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "SqlHelper.h"

#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

// 这是导出变量的一个示例
SQLHELPER_API int nSqlHelper=0;

// 这是导出函数的一个示例。
SQLHELPER_API int fnSqlHelper(void)
{
	return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 SqlHelper.h
CSqlHelper::CSqlHelper()
{ 
}
CSqlHelper::~CSqlHelper()
{ 
}
//操作类返回成功失败
bool CSqlHelper::SqlHelperMain(char * ip,char *sql,int flag)
{
	bool b_result=false;
	try
	{
		CDatabase 	database;
		char szData[256];
		ZeroMemory(szData,256);

		MakeDataSource(szData,ip,flag);//构建Driver

		if(database.Open(szData))//打开数据库配置环境
		{
			CDBRecord   dbrecord(&database);
			if(dbrecord.Open())//打开表结构
			{
				
				b_result=dbrecord.Query(sql);
				Sleep(1000);
			}
		}
	}
	catch(...)
	{
		return false;
	}
	return b_result;
}
//根据游戏操作类返回成功失败
bool CSqlHelper::SqlHelperMain(char * gamename,char * ip,char *sql,int flag)
{
	bool b_result=false;
	try
	{
		CDatabase 	database;
		char szData[256];
		ZeroMemory(szData,256);
		MakeDataSource(gamename,szData,ip,flag);//构建Driver
		if(database.Open(szData))
		{
		//	printf("cc10");
			CDBRecord   dbrecord(&database);
			if(dbrecord.Open())
			{
				
				b_result=dbrecord.Query(sql);
				Sleep(1000);
			}
		}
		else
		{
		//	printf("dd");
		}
	}
	catch(...)
	{
		return false;
	}
	return b_result;
}

//maple add
void CSqlHelper::SqlHelperMain_char(char * gamename,char * ip,char *sql,int flag,char * result)
{
	try
	{
		CDatabase 	database;
		char szData[256];
		ZeroMemory(szData,256);
		MakeDataSource(gamename,szData,ip,flag);//构建Driver
		
		if(database.Open(szData))
		{
			CDBRecord   dbrecord(&database);
			if(dbrecord.Open())
			{
				
				//b_result=dbrecord.Query(sql);
				result=(char *)dbrecord.ExecuteSql(sql);
				Sleep(1000);
			}
		}
	}
	catch(...)
	{
		
	}
}

//查询类返回vecter集
bool CSqlHelper::SqlHelperMain(vector<CGlobalStruct::DataRecord>*  ret_vector,char * ip,char *sql,int flag,int nIndex, int nPageSize)
{
	bool b_result=false;
	try
	{
		CDatabase 	database;
		CGlobalStruct::DataRecord tmpDataRecord;
		char szData[256];
		char strInt[10];
		ZeroMemory(szData,256);
		ZeroMemory(strInt,10);
		
		MakeDataSource(szData,ip,flag);//构建Driver
		int i=0;
		int allrecordnum=0;
		int PageNum=0;
		int iRecord=0;

	
		if(!ret_vector->empty())
		{
			ret_vector->clear();
		}

		if(nIndex==0&&nPageSize==0)
		{
			if(database.Open(szData))
			{
			//	printf("OK");
				CDBRecord   dbrecord(&database);
				if(dbrecord.Open())
				{
					if((b_result=dbrecord.Query(sql)))
					{
						fieldnum=dbrecord.GetFieldNum();
						while(dbrecord.Fetch())
						{
							for(int i=0;i<fieldnum;i++)
							{
								dbrecord>>&tmpDataRecord;
								ret_vector->push_back(tmpDataRecord);
							}
							sprintf(tmpDataRecord.recordfieldname,"%s","PageCount");
							tmpDataRecord.recordfieldtype=4;
							_itoa(1,strInt,10);
							sprintf(tmpDataRecord.recorddata,"%s",strInt);
							ret_vector->push_back(tmpDataRecord);
							

						}

					}
				}
			}
		}
		else
		{
			if(database.Open(szData))
			{
			//	printf("OK");
				CDBRecord   dbrecord(&database);
				if(dbrecord.Open())
				{
					if((b_result=dbrecord.Query(sql)))
					{
						fieldnum=dbrecord.GetFieldNum();
						while(dbrecord.Fetch())
						{
							allrecordnum++;
						}
						if(allrecordnum%nPageSize==0)
						{
							PageNum=allrecordnum/nPageSize;
						}
						else
						{
							PageNum=allrecordnum/nPageSize+1;
						}
					}
					if((b_result=dbrecord.Query(sql)))
					{
						while(dbrecord.Fetch())
						{
							iRecord++;
							if(iRecord>nPageSize*(nIndex-1)&&iRecord<=nPageSize*nIndex)
							{
								for(int i=0;i<fieldnum;i++)
								{
									dbrecord>>&tmpDataRecord;
									ret_vector->push_back(tmpDataRecord);
								}
								sprintf(tmpDataRecord.recordfieldname,"%s","PageCount");
								tmpDataRecord.recordfieldtype=4;
								_itoa(PageNum,strInt,10);
								sprintf(tmpDataRecord.recorddata,"%s",strInt);
								ret_vector->push_back(tmpDataRecord);
							}
							

						}

					}
				}
			}
		}
	}
	catch(...)
	{

	}

	return b_result;
}
vector<CGlobalStruct::DataRecord> CSqlHelper::SqlHelperMain(char * gamename,char * ip,char *sql,int flag,int nIndex, int nPageSize)
{
	vector<CGlobalStruct::DataRecord>  ret_vector;
	try
	{
		CDatabase 	database;
		CGlobalStruct::DataRecord tmpDataRecord;
		char szData[256];
		char strInt[10];
		ZeroMemory(szData,256);
		ZeroMemory(strInt,10);
		MakeDataSource(gamename,szData,ip,flag);//构建Driver
			
		int i=0;
		int allrecordnum=0;
		int PageNum=0;
		int iRecord=0;
	
		if(!ret_vector.empty())
		{
			ret_vector.clear();
		}

		if(nIndex==0&&nPageSize==0)
		{
			if(database.Open(szData))
			{
				CDBRecord   dbrecord(&database);
				if(dbrecord.Open())
				{
					if(dbrecord.Query(sql))
					{
						fieldnum=dbrecord.GetFieldNum();
						while(dbrecord.Fetch())
						{
							for(int i=0;i<fieldnum;i++)
							{
								dbrecord>>&tmpDataRecord;
								ret_vector.push_back(tmpDataRecord);
							}
							sprintf(tmpDataRecord.recordfieldname,"%s","PageCount");
							tmpDataRecord.recordfieldtype=4;
							_itoa(1,strInt,10);
							sprintf(tmpDataRecord.recorddata,"%s",strInt);
							ret_vector.push_back(tmpDataRecord);
							

						}

					}
				}
			}
		}
		else
		{
			if(database.Open(szData))
			{
			//	printf("OK1");
				CDBRecord   dbrecord(&database);
				if(dbrecord.Open())
				{
					if(dbrecord.Query(sql))
					{
						fieldnum=dbrecord.GetFieldNum();
						while(dbrecord.Fetch())
						{
							allrecordnum++;
						}
						if(allrecordnum%nPageSize==0)
						{
							PageNum=allrecordnum/nPageSize;
						}
						else
						{
							PageNum=allrecordnum/nPageSize+1;
						}
					}
					if(dbrecord.Query(sql))
					{
						while(dbrecord.Fetch())
						{
							iRecord++;
							if(iRecord>nPageSize*(nIndex-1)&&iRecord<=nPageSize*nIndex)
							{
								for(int i=0;i<fieldnum;i++)
								{
									dbrecord>>&tmpDataRecord;
									ret_vector.push_back(tmpDataRecord);
								}
								sprintf(tmpDataRecord.recordfieldname,"%s","PageCount");
								tmpDataRecord.recordfieldtype=4;
								_itoa(PageNum,strInt,10);
								sprintf(tmpDataRecord.recorddata,"%s",strInt);
								ret_vector.push_back(tmpDataRecord);
							}
							

						}

					}
				}
			}
		}
	}
	catch (...)
	{
		
	}
	return ret_vector;
}
//根据游戏查询类返回vecter集
bool CSqlHelper::SqlHelperMain(char * gamename,vector<CGlobalStruct::DataRecord>*  ret_vector,char * ip,char *sql,int flag,int nIndex, int nPageSize)
{
	bool b_result=false;
	try
	{
		CDatabase 	database;
		CGlobalStruct::DataRecord tmpDataRecord;
		char szData[256];
		char strInt[10];
		ZeroMemory(szData,256);
		ZeroMemory(strInt,10);
		
		MakeDataSource(gamename,szData,ip,flag);//构建Driver
			

		int i=0;
		int allrecordnum=0;
		int PageNum=0;
		int iRecord=0;
	
		if(!ret_vector->empty())
		{
			ret_vector->clear();
		}

		if(nIndex==0&&nPageSize==0)
		{
			if(database.Open(szData))
			{
			//	printf("OK1");
				CDBRecord   dbrecord(&database);
				if(dbrecord.Open())
				{
					if((b_result=dbrecord.Query(sql)))
					{
						fieldnum=dbrecord.GetFieldNum();
						while(dbrecord.Fetch())
						{
							for(int i=0;i<fieldnum;i++)
							{
								dbrecord>>&tmpDataRecord;
								ret_vector->push_back(tmpDataRecord);
							}
							sprintf(tmpDataRecord.recordfieldname,"%s","PageCount");
							tmpDataRecord.recordfieldtype=4;
							_itoa(1,strInt,10);
							sprintf(tmpDataRecord.recorddata,"%s",strInt);
							ret_vector->push_back(tmpDataRecord);
							

						}

					}
				}
			}
		}
		else
		{
			if(database.Open(szData))
			{
			//	printf("OK1");
				CDBRecord   dbrecord(&database);
				if(dbrecord.Open())
				{
					if((b_result=dbrecord.Query(sql)))
					{
						fieldnum=dbrecord.GetFieldNum();
						while(dbrecord.Fetch())
						{
							allrecordnum++;
						}
						if(allrecordnum%nPageSize==0)
						{
							PageNum=allrecordnum/nPageSize;
						}
						else
						{
							PageNum=allrecordnum/nPageSize+1;
						}
					}
					if((b_result=dbrecord.Query(sql)))
					{
						while(dbrecord.Fetch())
						{
							iRecord++;
							if(iRecord>nPageSize*(nIndex-1)&&iRecord<=nPageSize*nIndex)
							{
								for(int i=0;i<fieldnum;i++)
								{
									dbrecord>>&tmpDataRecord;
									ret_vector->push_back(tmpDataRecord);
								}
								sprintf(tmpDataRecord.recordfieldname,"%s","PageCount");
								tmpDataRecord.recordfieldtype=4;
								_itoa(PageNum,strInt,10);
								sprintf(tmpDataRecord.recorddata,"%s",strInt);
								ret_vector->push_back(tmpDataRecord);
							}
							

						}

					}
				}
			}
		}
	}
	catch(...)
	{

	}
	return b_result;
}
void CSqlHelper::MakeDataSource(char * szData,char *ip,int flag)
{
	try
	{
		switch(flag)
		{
		case 0://SQL
		case 2:
		case 4:
			{
				GetSQLData(szData,ip,flag);

			}
			break;
		case 1://MySql
		case 3:
		case 5:
		case 7:
		case 9:
		case 11:
			{
				GetMySqlData(szData,ip,flag);
			}
			break;
		default:
			break;
		}
	}
	catch(...)
	{

	}
	

}
void CSqlHelper::GetSQLData(char * szData,char * lpIP,int flag)//获取SQL数据库
{
	try
	{
		char path[1024];
		char filepath[1024];
		
		char databaseUser[20];	//本地数据库用户名
		char databasepwd[20];	//本地数据库密码
		char database[20];
		char IP[40];
		int iPort=0;
		GetCurrentDirectory(1024,path);
		sprintf(filepath,"%s%s",path,"\\Config\\Database.ini");
		
			
		switch(flag)
		{
		case 0:
			{
				GetPrivateProfileString(lpIP,"DBUser","gmtools",databaseUser,20,filepath);
				GetPrivateProfileString(lpIP,"DBPwd","FqJ23ddxCDky36vS",databasepwd,20,filepath);
				GetPrivateProfileString(lpIP,"Data",NULL,database,20,filepath);//数据库名
				iPort=GetPrivateProfileInt(lpIP,"DBPort",1433,filepath);
				
				if(!strcmp(lpIP,"127.0.0.1"))
				{
					sprintf(szData,"DRIVER={SQL Server};Server=%s,%i;UID=%s;PWD=%s;database=%s",lpIP,iPort,databaseUser,databasepwd,database);
				}
				else
				{
					sprintf(szData,"DRIVER={SQL Server};Server=%s,%i;UID=%s;PWD=%s",lpIP,iPort,databaseUser,databasepwd);
				}
			}
			break;
		case 2:
			{
				sprintf(filepath,"%s%s",path,"\\Config\\Scheme.INI");
				GetPrivateProfileString("DATABASE","Address","127.0.0.1",lpIP,40,filepath);
				GetPrivateProfileString("DATABASE","User","gmtools",databaseUser,20,filepath);
				GetPrivateProfileString("DATABASE","Pwd","FqJ23ddxCDky36vS",databasepwd,20,filepath);
				GetPrivateProfileString("DATABASE","Data",NULL,database,20,filepath);//数据库名
				iPort=GetPrivateProfileInt("DATABASE","Port",1433,filepath);
				sprintf(szData,"DRIVER={SQL Server};Server=%s,%i;UID=%s;PWD=%s;database=%s",lpIP,iPort,databaseUser,databasepwd,database);
				
			}
			break;
		case 4:
			{
				sprintf(filepath,"%s%s",path,"\\Config\\Scheme.INI");
				GetPrivateProfileString("DATABASE","Address","127.0.0.1",IP,40,filepath);
				GetPrivateProfileString("DATABASE","User","gmtools",databaseUser,20,filepath);
				GetPrivateProfileString("DATABASE","Pwd","FqJ23ddxCDky36vS",databasepwd,20,filepath);
				GetPrivateProfileString("DATABASE","Data",NULL,database,20,filepath);//数据库名
				iPort=GetPrivateProfileInt("DATABASE","Port",1433,filepath);

			//	sprintf(szData,"server=%s;user id=%s;password=%s;database=%s;pooling=false",lpIP,databaseUser,databasepwd,database);
				sprintf(szData,"DRIVER={SQL Server};Server=%s;UID=%s;PWD=%s;database=%s",IP,databaseUser,databasepwd,database);
				
			}
			break;
		default:
			break;
		}
		
		
		
		
	}
	catch(...)
	{
		
	}
	
}
void  CSqlHelper::GetMySqlData(char * szData,char * lpIP,int flag)//获取MySql数据库
{
	try
	{
		char path[1024];
		char filepath[1024];
		
		
		char databaseUser[20];	//本地数据库用户名
		char databasepwd[20];	//本地数据库密码
		char database[20];//数据库
		
		
		int iPort=0;
		char charset[20];//字符集设置
		
		GetCurrentDirectory(1024,path);
		sprintf(filepath,"%s%s",path,"\\Config\\Database.ini");
		

		switch(flag)
		{
			case 1:
				{
					GetPrivateProfileString(lpIP,"DBUser","gm_tools",databaseUser,20,filepath);
					GetPrivateProfileString(lpIP,"DBPwd","Qa9afto25aZ4npBb@",databasepwd,20,filepath);
					GetPrivateProfileString(lpIP,"CharSet","latin1",charset,20,filepath);
					iPort=GetPrivateProfileInt(lpIP,"DBPort",3306,filepath);
					
					if(!strcmp(lpIP,"60.206.15.93"))
					{
						sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=3307;USER=%s;PASSWORD=%s;SERVER=%s;charset=%s;OPTION=3;",databaseUser,databasepwd,"60.206.15.90",charset);
						
					}
					else if(!strcmp(lpIP,"60.206.15.92"))	
					{
						sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=3308;USER=%s;PASSWORD=%s;SERVER=%s;charset=%s;OPTION=3;",databaseUser,databasepwd,"60.206.15.90",charset);
						
					}
					else
					{
						sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=%i;USER=%s;PASSWORD=%s;SERVER=%s;charset=%s;OPTION=3;",iPort,databaseUser,databasepwd,lpIP,charset);
					}
				}
				break;
			case 3://默认数据库
				{
					GetPrivateProfileString(lpIP,"DBUser","gmtools",databaseUser,20,filepath);
					GetPrivateProfileString(lpIP,"DBPwd","FqJ23ddxCDky36vS",databasepwd,20,filepath);
					GetPrivateProfileString(lpIP,"Data",NULL,database,20,filepath);
					GetPrivateProfileString(lpIP,"CharSet","latin1",charset,20,filepath);
					iPort=GetPrivateProfileInt(lpIP,"DBPort",3306,filepath);
					sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=%i;USER=%s;PASSWORD=%s;SERVER=%s;database=%s;charset=%s;OPTION=3;",iPort,databaseUser,databasepwd,lpIP,database,charset);
				}
				break;
			case 5://默认数据库1
				{
					GetPrivateProfileString(lpIP,"DBUser","gmtools",databaseUser,20,filepath);
					GetPrivateProfileString(lpIP,"DBPwd","FqJ23ddxCDky36vS",databasepwd,20,filepath);
					GetPrivateProfileString(lpIP,"Data1",NULL,database,20,filepath);
					GetPrivateProfileString(lpIP,"CharSet","latin1",charset,20,filepath);
					iPort=GetPrivateProfileInt(lpIP,"DBPort",3306,filepath);
					sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=%i;USER=%s;PASSWORD=%s;SERVER=%s;database=%s;charset=%s;OPTION=3;",iPort,databaseUser,databasepwd,lpIP,database,charset);
				}
				break;
			case 7://默认数据库2
				{
					GetPrivateProfileString(lpIP,"DBUser","gmtools",databaseUser,20,filepath);
					GetPrivateProfileString(lpIP,"DBPwd","FqJ23ddxCDky36vS",databasepwd,20,filepath);
					GetPrivateProfileString(lpIP,"Data2",NULL,database,20,filepath);
					GetPrivateProfileString(lpIP,"CharSet","latin1",charset,20,filepath);
					iPort=GetPrivateProfileInt(lpIP,"DBPort",3306,filepath);
					//log内侧3306其他3307
					sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=%i;USER=%s;PASSWORD=%s;SERVER=%s;database=%s;charset=%s;OPTION=3;",iPort,databaseUser,databasepwd,lpIP,database,charset);
				}
				break;
			case 9://user2+数据库1
				{
					GetPrivateProfileString(lpIP,"DBUser1","gmtools",databaseUser,20,filepath);
					GetPrivateProfileString(lpIP,"DBPwd","FqJ23ddxCDky36vS",databasepwd,20,filepath);
					GetPrivateProfileString(lpIP,"Data",NULL,database,20,filepath);
					GetPrivateProfileString(lpIP,"CharSet","latin1",charset,20,filepath);
					iPort=GetPrivateProfileInt(lpIP,"DBPort",3306,filepath);
					//log内侧3306其他3307
					sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=%i;USER=%s;PASSWORD=%s;SERVER=%s;database=%s;charset=%s;OPTION=3;",iPort,databaseUser,databasepwd,lpIP,database,charset);
				}
				break;
			case 11://user2+数据库2
				{
					GetPrivateProfileString(lpIP,"DBUser1","gmtools",databaseUser,20,filepath);
					GetPrivateProfileString(lpIP,"DBPwd","FqJ23ddxCDky36vS",databasepwd,20,filepath);
					GetPrivateProfileString(lpIP,"Data1",NULL,database,20,filepath);
					GetPrivateProfileString(lpIP,"CharSet","latin1",charset,20,filepath);
					iPort=GetPrivateProfileInt(lpIP,"DBPort",3306,filepath);
					//log内侧3306其他3307
					sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=%i;USER=%s;PASSWORD=%s;SERVER=%s;database=%s;charset=%s;OPTION=3;",iPort,databaseUser,databasepwd,lpIP,database,charset);
				}
				break;
			default:
				break;
		}
		
		
		
	}
	catch(...)
	{
		
	}
	
	
}







void CSqlHelper::MakeDataSource(char * gamename,char * szData,char *ip,int flag)
{
/*	try
	{
		char defaultuser[40];
		char defaultpasswd[40];
		char defaultCharSet[40];
		int  defaultSQLPort;
		int  defaultMySqlPort;

		GetDefault(gamename,defaultuser,defaultpasswd,defaultCharSet,&defaultSQLPort,&defaultMySqlPort);
		switch(flag)
		{
		case 0://SQL(取默认)
			{
				GetSQLData(defaultuser,defaultpasswd,defaultSQLPort,szData,ip);
			}
			break;
		case 1://MySql(取默认)
			{
				GetMySqlData(defaultuser,defaultpasswd,defaultMySqlPort,defaultCharSet,szData,ip);
			}
			break;
		default:
			break;
	
		}
	}
	catch(...)
	{
		
	}
	*/

	try
	{
		switch(flag)
		{
		case 0://SQL
		case 2:
		case 4:
			{
				GetSQLData(gamename,szData,ip,flag);
				
			}
			break;
		case 1://MySql
		case 3:
		case 5:
		case 7:
		case 9:
		case 11:
		case 12:
		case 13:
			{
				GetMySqlData(gamename,szData,ip,flag);
			}
			break;
		default:
			break;
		}
	}
	catch(...)
	{
		
	}
	
}
void CSqlHelper::GetSQLData(char* gamename,char * szData,char * lpIP,int flag)//获取SQL数据库
{
	try
	{
		char path[1024];
		char filepath[1024];
		char IP[40];
		
		char databaseUser[20];	//本地数据库用户名
		char databasepwd[20];	//本地数据库密码
		char database[20];
		int iPort=0;
		GetCurrentDirectory(1024,path);
		sprintf(filepath,"%s%s",path,"\\Config\\Database.ini");



		char defaultuser[40];
		char defaultpasswd[40];
		char defaultCharSet[40];
		int  defaultSQLPort=1433;
		int  defaultMySqlPort=3306;
		
	//	GetDefault(gamename,defaultuser,defaultpasswd,defaultCharSet,&defaultSQLPort,&defaultMySqlPort);
		
		switch(flag)
		{
		case 0:
			{
				////////////获取默认一下///////////////////////
				GetPrivateProfileString(gamename,"DBUser","gmtools",defaultuser,20,filepath);
				GetPrivateProfileString(gamename,"DBPwd","",defaultpasswd,20,filepath);
				GetPrivateProfileString(gamename,"CharSet","",defaultCharSet,20,filepath);
				defaultSQLPort=GetPrivateProfileInt(gamename,"DBSQLPort",1433,filepath);



				GetPrivateProfileString(lpIP,"DBUser",defaultuser,databaseUser,20,filepath);
				GetPrivateProfileString(lpIP,"DBPwd",defaultpasswd,databasepwd,20,filepath);
				GetPrivateProfileString(lpIP,"Data",NULL,database,20,filepath);//数据库名
				iPort=GetPrivateProfileInt(lpIP,"DBPort",defaultSQLPort,filepath);
				
				if(!strcmp(lpIP,"127.0.0.1"))
				{
					sprintf(szData,"DRIVER={SQL Server};Server=%s,%i;UID=%s;PWD=%s;database=%s",lpIP,iPort,databaseUser,databasepwd,database);
					
				}
				else
				{
					sprintf(szData,"DRIVER={SQL Server};Server=%s,%i;UID=%s;PWD=%s",lpIP,iPort,databaseUser,databasepwd);
					
				}
			}
			break;
		case 2:
			{
				GetPrivateProfileString(gamename,"DBUser","gmtools",defaultuser,20,filepath);
				GetPrivateProfileString(gamename,"DBPwd","",defaultpasswd,20,filepath);
				GetPrivateProfileString(gamename,"CharSet","",defaultCharSet,20,filepath);
				defaultSQLPort=GetPrivateProfileInt(gamename,"DBSQLPort",1433,filepath);



				GetPrivateProfileString(lpIP,"DBUser",defaultuser,databaseUser,20,filepath);
				GetPrivateProfileString(lpIP,"DBPwd",defaultpasswd,databasepwd,20,filepath);
				GetPrivateProfileString(lpIP,"Data",NULL,database,20,filepath);//数据库名
				iPort=GetPrivateProfileInt(lpIP,"DBPort",defaultSQLPort,filepath);

				sprintf(szData,"DRIVER={SQL Server};Server=%s,%i;UID=%s;PWD=%s;database=%s",lpIP,iPort,databaseUser,databasepwd,database);
							
			}
			break;
		case 4:
			{

				sprintf(filepath,"%s%s",path,"\\Config\\Scheme.INI");
				GetPrivateProfileString("DATABASE","Address","127.0.0.1",IP,40,filepath);
				GetPrivateProfileString("DATABASE","User","gmtools",databaseUser,20,filepath);
				GetPrivateProfileString("DATABASE","Pwd","FqJ23ddxCDky36vS",databasepwd,20,filepath);
				GetPrivateProfileString("DATABASE","Data",NULL,database,20,filepath);//数据库名
				iPort=GetPrivateProfileInt("DATABASE","Port",1433,filepath);
				
				//	sprintf(szData,"server=%s;user id=%s;password=%s;database=%s;pooling=false",lpIP,databaseUser,databasepwd,database);
				sprintf(szData,"DRIVER={SQL Server};Server=%s;UID=%s;PWD=%s;database=%s",IP,databaseUser,databasepwd,database);	
			}
			break;
		default:
			break;
		}
		
		
		
		
	}
	catch(...)
	{
		
	}
}
void  CSqlHelper::GetMySqlData(char * gamename,char * szData,char * lpIP,int flag)//获取MySql数据库
{

	char path[1024];
	char filepath[1024];
	
	
	char databaseUser[40];	//本地数据库用户名
	char databasepwd[40];	//本地数据库密码
	char database[40];//数据库
	ZeroMemory(databaseUser,40);
	ZeroMemory(databasepwd,40);
	ZeroMemory(database,40);
	
	int iPort=0;
	char charset[20];//字符集设置
	
	GetCurrentDirectory(1024,path);
	sprintf(filepath,"%s%s",path,"\\Config\\Database.ini");
	
	
	char defaultuser[40];
	char defaultpasswd[40];
	char defaultCharSet[40];
	char defaultData[40];
	ZeroMemory(defaultuser,40);
	ZeroMemory(defaultpasswd,40);
	ZeroMemory(defaultCharSet,40);
	ZeroMemory(defaultData,40);
	int  defaultSQLPort=0;
	int  defaultMySqlPort=0;

	if(!strcmp(gamename,"JW2"))
	{
		char ip[40];
		char port[40];
		for(int i=0;i<strlen(lpIP);i++)
		{
			if(lpIP[i]==',')//查询标志","
			{
				memcpy(ip,lpIP,i);//得到文件名
				ip[i]='\0';
				break;
			}
		}
		int aa=strlen(ip);//得到文件名长度
		memcpy(port,&lpIP[aa+1],strlen(lpIP)-aa);
		int tmpPort=StrToInt(port);
		switch(flag)
		{
			case 1:
				{
					GetPrivateProfileString(gamename,"DBUser","gmtools",defaultuser,40,filepath);
					GetPrivateProfileString(gamename,"DBPwd","FqJ23ddxCDky36vS",defaultpasswd,40,filepath);
					GetPrivateProfileString(gamename,"CharSet","latin1",defaultCharSet,40,filepath);
					defaultMySqlPort=GetPrivateProfileInt(gamename,"DBPort",3306,filepath);

					GetPrivateProfileString(lpIP,"DBUser",defaultuser,databaseUser,40,filepath);
					GetPrivateProfileString(lpIP,"DBPwd",defaultpasswd,databasepwd,40,filepath);
					GetPrivateProfileString(lpIP,"CharSet",defaultCharSet,charset,40,filepath);
					iPort=GetPrivateProfileInt(lpIP,"DBPort",defaultMySqlPort,filepath);
					

					sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=%i;USER=%s;PASSWORD=%s;SERVER=%s;charset=%s;OPTION=3;",iPort,databaseUser,databasepwd,ip,charset);

				}
				break;
			case 3://默认数据库
				{
					GetPrivateProfileString(gamename,"DBUser","gmtools",defaultuser,40,filepath);
					GetPrivateProfileString(gamename,"DBPwd","FqJ23ddxCDky36vS",defaultpasswd,40,filepath);
					GetPrivateProfileString(gamename,"Data",NULL,defaultData,40,filepath);
					GetPrivateProfileString(gamename,"CharSet","latin1",defaultCharSet,40,filepath);
					defaultMySqlPort=GetPrivateProfileInt(gamename,"DBPort",3306,filepath);


					GetPrivateProfileString(lpIP,"DBUser",defaultuser,databaseUser,40,filepath);
					GetPrivateProfileString(lpIP,"DBPwd",defaultpasswd,databasepwd,40,filepath);
					GetPrivateProfileString(lpIP,"Data",defaultData,database,40,filepath);
					GetPrivateProfileString(lpIP,"CharSet",defaultCharSet,charset,40,filepath);
					iPort=GetPrivateProfileInt(lpIP,"DBPort",defaultMySqlPort,filepath);
					sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=%i;USER=%s;PASSWORD=%s;SERVER=%s;database=%s;charset=%s;OPTION=3;",port,databaseUser,databasepwd,lpIP,database,charset);
				}
				break;
			case 5://默认数据库1
				{
					GetPrivateProfileString(gamename,"DBUser","gmtools",defaultuser,40,filepath);
					GetPrivateProfileString(gamename,"DBPwd","FqJ23ddxCDky36vS",defaultpasswd,40,filepath);
					GetPrivateProfileString(gamename,"Data1",NULL,defaultData,40,filepath);
					GetPrivateProfileString(gamename,"CharSet","latin1",defaultCharSet,40,filepath);
					defaultMySqlPort=GetPrivateProfileInt(gamename,"DBPort",3306,filepath);

					GetPrivateProfileString(lpIP,"DBUser",defaultuser,databaseUser,40,filepath);
					GetPrivateProfileString(lpIP,"DBPwd",defaultpasswd,databasepwd,40,filepath);
					GetPrivateProfileString(lpIP,"Data1",defaultData,database,40,filepath);
					GetPrivateProfileString(lpIP,"CharSet",defaultCharSet,charset,40,filepath);
					iPort=GetPrivateProfileInt(lpIP,"DBPort",defaultMySqlPort,filepath);
					sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=%i;USER=%s;PASSWORD=%s;SERVER=%s;database=%s;charset=%s;OPTION=3;",iPort,databaseUser,databasepwd,lpIP,database,charset);
				}
				break;
			case 7://默认数据库2
				{
					GetPrivateProfileString(gamename,"DBUser","gmtools",defaultuser,40,filepath);
					GetPrivateProfileString(gamename,"DBPwd","FqJ23ddxCDky36vS",defaultpasswd,40,filepath);
					GetPrivateProfileString(gamename,"Data2",NULL,defaultData,40,filepath);
					GetPrivateProfileString(gamename,"CharSet","latin1",defaultCharSet,40,filepath);
					defaultMySqlPort=GetPrivateProfileInt(gamename,"DBPort1",3306,filepath);

					GetPrivateProfileString(lpIP,"DBUser",defaultuser,databaseUser,40,filepath);
					GetPrivateProfileString(lpIP,"DBPwd",defaultpasswd,databasepwd,40,filepath);
					GetPrivateProfileString(lpIP,"Data2",defaultData,database,40,filepath);
					GetPrivateProfileString(lpIP,"CharSet",defaultCharSet,charset,40,filepath);
					iPort=GetPrivateProfileInt(lpIP,"DBPort1",defaultMySqlPort,filepath);
					//log内侧3306其他3307
					sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=%i;USER=%s;PASSWORD=%s;SERVER=%s;database=%s;charset=%s;OPTION=3;",port,databaseUser,databasepwd,lpIP,database,charset);
				}
				break;
			case 9://user2+数据库1
				{
					GetPrivateProfileString(gamename,"DBUser1","gmtools",defaultuser,40,filepath);
					GetPrivateProfileString(gamename,"DBPwd","FqJ23ddxCDky36vS",defaultpasswd,40,filepath);
					GetPrivateProfileString(gamename,"Data",NULL,defaultData,40,filepath);
					GetPrivateProfileString(gamename,"CharSet","latin1",defaultCharSet,40,filepath);
					defaultMySqlPort=GetPrivateProfileInt(gamename,"DBPort",3306,filepath);

					GetPrivateProfileString(lpIP,"DBUser1",defaultuser,databaseUser,40,filepath);
					GetPrivateProfileString(lpIP,"DBPwd",defaultpasswd,databasepwd,40,filepath);
					GetPrivateProfileString(lpIP,"Data",defaultData,database,40,filepath);
					GetPrivateProfileString(lpIP,"CharSet",defaultCharSet,charset,40,filepath);
					iPort=GetPrivateProfileInt(lpIP,"DBPort",defaultMySqlPort,filepath);
					//log内侧3306其他3307
					sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=%i;USER=%s;PASSWORD=%s;SERVER=%s;database=%s;charset=%s;OPTION=3;",port,databaseUser,databasepwd,lpIP,database,charset);
				}
				break;
			case 11://user2+数据库2
				{
					GetPrivateProfileString(gamename,"DBUser1","gmtools",defaultuser,40,filepath);
					GetPrivateProfileString(gamename,"DBPwd","FqJ23ddxCDky36vS",defaultpasswd,40,filepath);
					GetPrivateProfileString(gamename,"Data1",NULL,defaultData,40,filepath);
					GetPrivateProfileString(gamename,"CharSet","latin1",defaultCharSet,40,filepath);
					defaultMySqlPort=GetPrivateProfileInt(gamename,"DBPort",3306,filepath);

					GetPrivateProfileString(lpIP,"DBUser1",defaultuser,databaseUser,40,filepath);
					GetPrivateProfileString(lpIP,"DBPwd",defaultpasswd,databasepwd,40,filepath);
					GetPrivateProfileString(lpIP,"Data1",defaultData,database,40,filepath);
					GetPrivateProfileString(lpIP,"CharSet",defaultCharSet,charset,40,filepath);
					iPort=GetPrivateProfileInt(lpIP,"DBPort",defaultMySqlPort,filepath);
					//log内侧3306其他3307
					sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=%i;USER=%s;PASSWORD=%s;SERVER=%s;database=%s;charset=%s;OPTION=3;",port,databaseUser,databasepwd,lpIP,database,charset);
				}
			case 12://XDJG-GameDb/userDb
				{
					GetPrivateProfileString(gamename,"DBUser1","gmtools_xd",defaultuser,40,filepath);
					GetPrivateProfileString(gamename,"DBPwd","#Di(37%i3>@d9",defaultpasswd,40,filepath);
					GetPrivateProfileString(gamename,"Data1",NULL,defaultData,40,filepath);
					GetPrivateProfileString(gamename,"CharSet","latin1",defaultCharSet,40,filepath);
					defaultMySqlPort=GetPrivateProfileInt(gamename,"DBPort",3306,filepath);
					
					GetPrivateProfileString(lpIP,"DBUser1",defaultuser,databaseUser,40,filepath);
					GetPrivateProfileString(lpIP,"DBPwd",defaultpasswd,databasepwd,40,filepath);
					GetPrivateProfileString(lpIP,"Data1",defaultData,database,40,filepath);
					GetPrivateProfileString(lpIP,"CharSet",defaultCharSet,charset,40,filepath);
					iPort=GetPrivateProfileInt(lpIP,"DBPort",defaultMySqlPort,filepath);

					//GameDb/userDb
					sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=%i;USER=%s;PASSWORD=%s;SERVER=%s;database=%s;charset=%s;OPTION=3;",iPort,databaseUser,databasepwd,lpIP,database,charset);
				}
				break;
			case 13://logDb
				{
					GetPrivateProfileString(gamename,"DBUser1","gmtools_xd",defaultuser,40,filepath);
					GetPrivateProfileString(gamename,"DBPwd","#Di(37%i3>@d9",defaultpasswd,40,filepath);
					GetPrivateProfileString(gamename,"Data1",NULL,defaultData,40,filepath);
					GetPrivateProfileString(gamename,"CharSet","latin1",defaultCharSet,40,filepath);
					defaultMySqlPort=GetPrivateProfileInt(gamename,"DBPort",3307,filepath);
					
					GetPrivateProfileString(lpIP,"DBUser1",defaultuser,databaseUser,40,filepath);
					GetPrivateProfileString(lpIP,"DBPwd",defaultpasswd,databasepwd,40,filepath);
					GetPrivateProfileString(lpIP,"Data1",defaultData,database,40,filepath);
					GetPrivateProfileString(lpIP,"CharSet",defaultCharSet,charset,40,filepath);
					iPort=GetPrivateProfileInt(lpIP,"DBPort",defaultMySqlPort,filepath);
					
					//userDb
					sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=%i;USER=%s;PASSWORD=%s;SERVER=%s;database=%s;charset=%s;OPTION=3;",iPort,databaseUser,databasepwd,lpIP,database,charset);
				}
				break;
			default:
				break;
		}


	}
	else
	{

		try
		{
		
		

		switch(flag)
		{
			case 1:
				{
					GetPrivateProfileString(gamename,"DBUser","gmtools",defaultuser,40,filepath);
					GetPrivateProfileString(gamename,"DBPwd","FqJ23ddxCDky36vS",defaultpasswd,40,filepath);
					GetPrivateProfileString(gamename,"CharSet","latin1",defaultCharSet,40,filepath);
					defaultMySqlPort=GetPrivateProfileInt(gamename,"DBPort",3306,filepath);

					GetPrivateProfileString(lpIP,"DBUser",defaultuser,databaseUser,40,filepath);
					GetPrivateProfileString(lpIP,"DBPwd",defaultpasswd,databasepwd,40,filepath);
					GetPrivateProfileString(lpIP,"CharSet",defaultCharSet,charset,40,filepath);
					iPort=GetPrivateProfileInt(lpIP,"DBPort",defaultMySqlPort,filepath);
					
					if(!strcmp(lpIP,"60.206.15.93"))
					{
						sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=3307;USER=%s;PASSWORD=%s;SERVER=%s;charset=%s;OPTION=3;",databaseUser,databasepwd,"60.206.15.90",charset);
						
					}
					else if(!strcmp(lpIP,"60.206.15.92"))	
					{
						sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=3308;USER=%s;PASSWORD=%s;SERVER=%s;charset=%s;OPTION=3;",databaseUser,databasepwd,"60.206.15.90",charset);
						
					}
					else
					{
						sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=%i;USER=%s;PASSWORD=%s;SERVER=%s;charset=%s;OPTION=3;",iPort,databaseUser,databasepwd,lpIP,charset);
					}
				}
				break;
			case 3://默认数据库
				{
					GetPrivateProfileString(gamename,"DBUser","gmtools",defaultuser,40,filepath);
					GetPrivateProfileString(gamename,"DBPwd","FqJ23ddxCDky36vS",defaultpasswd,40,filepath);
					GetPrivateProfileString(gamename,"Data",NULL,defaultData,40,filepath);
					GetPrivateProfileString(gamename,"CharSet","latin1",defaultCharSet,40,filepath);
					defaultMySqlPort=GetPrivateProfileInt(gamename,"DBPort",3306,filepath);


					GetPrivateProfileString(lpIP,"DBUser",defaultuser,databaseUser,40,filepath);
					GetPrivateProfileString(lpIP,"DBPwd",defaultpasswd,databasepwd,40,filepath);
					GetPrivateProfileString(lpIP,"Data",defaultData,database,40,filepath);
					GetPrivateProfileString(lpIP,"CharSet",defaultCharSet,charset,40,filepath);
					iPort=GetPrivateProfileInt(lpIP,"DBPort",defaultMySqlPort,filepath);
					sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=%i;USER=%s;PASSWORD=%s;SERVER=%s;database=%s;charset=%s;OPTION=3;",iPort,databaseUser,databasepwd,lpIP,database,charset);
				}
				break;
			case 5://默认数据库1
				{
					GetPrivateProfileString(gamename,"DBUser","gmtools",defaultuser,40,filepath);
					GetPrivateProfileString(gamename,"DBPwd","FqJ23ddxCDky36vS",defaultpasswd,40,filepath);
					GetPrivateProfileString(gamename,"Data1",NULL,defaultData,40,filepath);
					GetPrivateProfileString(gamename,"CharSet","latin1",defaultCharSet,40,filepath);
					defaultMySqlPort=GetPrivateProfileInt(gamename,"DBPort",3306,filepath);

					GetPrivateProfileString(lpIP,"DBUser",defaultuser,databaseUser,40,filepath);
					GetPrivateProfileString(lpIP,"DBPwd",defaultpasswd,databasepwd,40,filepath);
					GetPrivateProfileString(lpIP,"Data1",defaultData,database,40,filepath);
					GetPrivateProfileString(lpIP,"CharSet",defaultCharSet,charset,40,filepath);
					iPort=GetPrivateProfileInt(lpIP,"DBPort",defaultMySqlPort,filepath);
					sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=%i;USER=%s;PASSWORD=%s;SERVER=%s;database=%s;charset=%s;OPTION=3;",iPort,databaseUser,databasepwd,lpIP,database,charset);
				}
				break;
			case 7://默认数据库2
				{
					GetPrivateProfileString(gamename,"DBUser","gmtools",defaultuser,40,filepath);
					GetPrivateProfileString(gamename,"DBPwd","FqJ23ddxCDky36vS",defaultpasswd,40,filepath);
					GetPrivateProfileString(gamename,"Data2",NULL,defaultData,40,filepath);
					GetPrivateProfileString(gamename,"CharSet","latin1",defaultCharSet,40,filepath);
					defaultMySqlPort=GetPrivateProfileInt(gamename,"DBPort1",3306,filepath);

					GetPrivateProfileString(lpIP,"DBUser",defaultuser,databaseUser,40,filepath);
					GetPrivateProfileString(lpIP,"DBPwd",defaultpasswd,databasepwd,40,filepath);
					GetPrivateProfileString(lpIP,"Data2",defaultData,database,40,filepath);
					GetPrivateProfileString(lpIP,"CharSet",defaultCharSet,charset,40,filepath);
					iPort=GetPrivateProfileInt(lpIP,"DBPort1",defaultMySqlPort,filepath);
					//log内侧3306其他3307
					sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=%i;USER=%s;PASSWORD=%s;SERVER=%s;database=%s;charset=%s;OPTION=3;",iPort,databaseUser,databasepwd,lpIP,database,charset);
				}
				break;
			case 9://user2+数据库1
				{
					GetPrivateProfileString(gamename,"DBUser1","gmtools",defaultuser,40,filepath);
					GetPrivateProfileString(gamename,"DBPwd","FqJ23ddxCDky36vS",defaultpasswd,40,filepath);
					GetPrivateProfileString(gamename,"Data",NULL,defaultData,40,filepath);
					GetPrivateProfileString(gamename,"CharSet","latin1",defaultCharSet,40,filepath);
					defaultMySqlPort=GetPrivateProfileInt(gamename,"DBPort",3306,filepath);

					GetPrivateProfileString(lpIP,"DBUser1",defaultuser,databaseUser,40,filepath);
					GetPrivateProfileString(lpIP,"DBPwd",defaultpasswd,databasepwd,40,filepath);
					GetPrivateProfileString(lpIP,"Data",defaultData,database,40,filepath);
					GetPrivateProfileString(lpIP,"CharSet",defaultCharSet,charset,40,filepath);
					iPort=GetPrivateProfileInt(lpIP,"DBPort",defaultMySqlPort,filepath);
					//log内侧3306其他3307
					sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=%i;USER=%s;PASSWORD=%s;SERVER=%s;database=%s;charset=%s;OPTION=3;",iPort,databaseUser,databasepwd,lpIP,database,charset);
				}
				break;
			case 11://user2+数据库2
				{
					GetPrivateProfileString(gamename,"DBUser1","gmtools",defaultuser,40,filepath);
					GetPrivateProfileString(gamename,"DBPwd","FqJ23ddxCDky36vS",defaultpasswd,40,filepath);
					GetPrivateProfileString(gamename,"Data1",NULL,defaultData,40,filepath);
					GetPrivateProfileString(gamename,"CharSet","latin1",defaultCharSet,40,filepath);
					defaultMySqlPort=GetPrivateProfileInt(gamename,"DBPort",3306,filepath);

					GetPrivateProfileString(lpIP,"DBUser1",defaultuser,databaseUser,40,filepath);
					GetPrivateProfileString(lpIP,"DBPwd",defaultpasswd,databasepwd,40,filepath);
					GetPrivateProfileString(lpIP,"Data1",defaultData,database,40,filepath);
					GetPrivateProfileString(lpIP,"CharSet",defaultCharSet,charset,40,filepath);
					iPort=GetPrivateProfileInt(lpIP,"DBPort",defaultMySqlPort,filepath);
					//log内侧3306其他3307
					sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=%i;USER=%s;PASSWORD=%s;SERVER=%s;database=%s;charset=%s;OPTION=3;",iPort,databaseUser,databasepwd,lpIP,database,charset);
				}
				break;
			case 12://XDJG-GameDb/userDb
				{
					GetPrivateProfileString(gamename,"DBUser1","gmtools_xd",defaultuser,40,filepath);
					GetPrivateProfileString(gamename,"DBPwd","#Di(37%i3>@d9",defaultpasswd,40,filepath);
					GetPrivateProfileString(gamename,"Data1",NULL,defaultData,40,filepath);
					GetPrivateProfileString(gamename,"CharSet","latin1",defaultCharSet,40,filepath);
					defaultMySqlPort=GetPrivateProfileInt(gamename,"DBPort",3306,filepath);
					
					GetPrivateProfileString(lpIP,"DBUser1",defaultuser,databaseUser,40,filepath);
					GetPrivateProfileString(lpIP,"DBPwd",defaultpasswd,databasepwd,40,filepath);
					GetPrivateProfileString(lpIP,"Data1",defaultData,database,40,filepath);
					GetPrivateProfileString(lpIP,"CharSet",defaultCharSet,charset,40,filepath);
					iPort=GetPrivateProfileInt(lpIP,"DBPort",defaultMySqlPort,filepath);
					
					//GameDb/userDb
					sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=%i;USER=%s;PASSWORD=%s;SERVER=%s;database=%s;charset=%s;OPTION=3;",iPort,databaseUser,databasepwd,lpIP,database,charset);
				}
				break;
			case 13://logDb
				{
					GetPrivateProfileString(gamename,"DBUser1","gmtools_xd",defaultuser,40,filepath);
					GetPrivateProfileString(gamename,"DBPwd","#Di(37%i3>@d9",defaultpasswd,40,filepath);
					GetPrivateProfileString(gamename,"Data1",NULL,defaultData,40,filepath);
					GetPrivateProfileString(gamename,"CharSet","latin1",defaultCharSet,40,filepath);
					defaultMySqlPort=GetPrivateProfileInt(gamename,"DBPort",3307,filepath);
					
					GetPrivateProfileString(lpIP,"DBUser1",defaultuser,databaseUser,40,filepath);
					GetPrivateProfileString(lpIP,"DBPwd",defaultpasswd,databasepwd,40,filepath);
					GetPrivateProfileString(lpIP,"Data1",defaultData,database,40,filepath);
					GetPrivateProfileString(lpIP,"CharSet",defaultCharSet,charset,40,filepath);
					iPort=GetPrivateProfileInt(lpIP,"DBPort",defaultMySqlPort,filepath);
					
					//userDb
					sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=%i;USER=%s;PASSWORD=%s;SERVER=%s;database=%s;charset=%s;OPTION=3;",iPort,databaseUser,databasepwd,lpIP,database,charset);
				}
				break;
			default:
				break;
		}
		
		
		
	}
	catch(...)
	{
		
	}

	}
	
/*	try
	{
		char path[1024];
		char filepath[1024];


		char databaseUser[20];	//本地数据库用户名
		char databasepwd[20];	//本地数据库密码


		int iPort=0;
		char charset[20];//字符集设置

		GetCurrentDirectory(1024,path);
		sprintf(filepath,"%s%s",path,"\\Config\\Database.ini");

		GetPrivateProfileString(lpIP,"DBUser","gmtools",databaseUser,20,filepath);
		GetPrivateProfileString(lpIP,"DBPwd","FqJ23ddxCDky36vS",databasepwd,20,filepath);
		GetPrivateProfileString(lpIP,"CharSet","latin1",charset,20,filepath);
		iPort=GetPrivateProfileInt(lpIP,"DBPort",3306,filepath);

		if(!strcmp(lpIP,"60.206.15.93"))
		{
			sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=3307;USER=%s;PASSWORD=%s;SERVER=%s;charset=%s;OPTION=3;",databaseUser,databasepwd,"60.206.15.91",charset);

		}
		else if(!strcmp(lpIP,"60.206.15.92"))	
		{
			sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=3308;USER=%s;PASSWORD=%s;SERVER=%s;charset=%s;OPTION=3;",databaseUser,databasepwd,"60.206.15.91",charset);

		}
		else
		{
			sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=%i;USER=%s;PASSWORD=%s;SERVER=%s;charset=%s;OPTION=3;",iPort,databaseUser,databasepwd,lpIP,charset);
		}


	}
	catch(...)
	{

	}*/
	

}
void CSqlHelper::GetDefault(char * gamename,char *user,char * passwd,char * CharSet,int* iSQLPort,int *iMySqlPort)
{
	try
	{
		char path[1024];
		char filepath[1024];
		

		GetCurrentDirectory(1024,path);
		sprintf(filepath,"%s%s",path,"\\Config\\Database.ini");
		
		GetPrivateProfileString(gamename,"DBUser","gmtools",user,40,filepath);
		GetPrivateProfileString(gamename,"DBPwd","",passwd,40,filepath);
		GetPrivateProfileString(gamename,"CharSet","",CharSet,40,filepath);
		* iSQLPort=GetPrivateProfileInt(gamename,"DBSQLPort",1433,filepath);
		* iMySqlPort=GetPrivateProfileInt(gamename,"DBMySQLPort",3306,filepath);
	
		
	}
	catch(...)
	{
		
	}

}

void CSqlHelper::GetSQLData(char * user,char * passwd,int iSQLPort,char * szData,char * lpIP)//获取SQL数据库
{
	try
	{
		char path[1024];
		char filepath[1024];
		
		char databaseUser[20];	//本地数据库用户名
		char databasepwd[20];	//本地数据库密码
		char database[20];
		
		
		
		//	char charset[20];//字符集设置
		int iPort=0;
		GetCurrentDirectory(1024,path);
		sprintf(filepath,"%s%s",path,"\\Config\\Database.ini");
		
		GetPrivateProfileString(lpIP,"DBUser",user,databaseUser,20,filepath);
		GetPrivateProfileString(lpIP,"DBPwd",passwd,databasepwd,20,filepath);
		GetPrivateProfileString(lpIP,"Data",NULL,database,20,filepath);//数据库名
		iPort=GetPrivateProfileInt(lpIP,"DBPort",iSQLPort,filepath);
		
		if(!strcmp(lpIP,"127.0.0.1"))
		{
			sprintf(szData,"DRIVER={SQL Server};PORT=%i;UID=%s;PWD=%s;Server=%s;database=%s",iPort,databaseUser,databasepwd,lpIP,database);
		}
		else
		{
			sprintf(szData,"DRIVER={SQL Server};PORT=%i;UID=%s;PWD=%s;Server=%s",iPort,databaseUser,databasepwd,lpIP);
			
		}
		
	}
	catch(...)
	{
		
	}
	
}
void  CSqlHelper::GetMySqlData(char * user,char * passwd,int iMySqlPort,char * CharSet,char * szData,char * lpIP)//获取MySql数据库
{
	try
	{
		char path[1024];
		char filepath[1024];
		
		
		char databaseUser[20];	//本地数据库用户名
		char databasepwd[20];	//本地数据库密码
		
		
		int iPort=0;
		char charset[20];//字符集设置
		
		GetCurrentDirectory(1024,path);
		sprintf(filepath,"%s%s",path,"\\Config\\Database.ini");
		
		GetPrivateProfileString(lpIP,"DBUser",user,databaseUser,20,filepath);
		GetPrivateProfileString(lpIP,"DBPwd",passwd,databasepwd,20,filepath);
		GetPrivateProfileString(lpIP,"CharSet",CharSet,charset,20,filepath);
		iPort=GetPrivateProfileInt(lpIP,"DBPort",3306,filepath);
		
		if(!strcmp(lpIP,"60.206.15.93"))
		{
			sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=3307;USER=%s;PASSWORD=%s;SERVER=%s;charset=%s;OPTION=3;",databaseUser,databasepwd,"60.206.15.91",charset);
			
		}
		else if(!strcmp(lpIP,"60.206.15.92"))	
		{
			sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=3308;USER=%s;PASSWORD=%s;SERVER=%s;charset=%s;OPTION=3;",databaseUser,databasepwd,"60.206.15.91",charset);
			
		}
		else
		{
			sprintf(szData,"DRIVER={MySQL ODBC 3.51 Driver};Persist Security Info=True;PORT=%i;USER=%s;PASSWORD=%s;SERVER=%s;charset=%s;OPTION=3;",iPort,databaseUser,databasepwd,lpIP,charset);
		}
		
		
	}
	catch(...)
	{
		
	}
	
	
}
//构建SQL加上限制limit
void CSqlHelper::MakeSQL(char * lpobjSQL,char * lpResSQL,int flag,int iIndex,int iPageSize)
{
	switch(flag)
	{
	case 0://SQL
		{
			sprintf(lpobjSQL,lpResSQL);
		}
		break;
	case 1:
		{
			sprintf(lpobjSQL,"%s limit %i,%i",lpResSQL,(iIndex-1)*iPageSize,iPageSize);
		}
		break;
	}
}
//获取列数
int CSqlHelper::GetFieldNum()
{
	return fieldnum;
}



