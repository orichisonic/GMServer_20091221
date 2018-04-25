#include "stdafx.h"
#include ".\operatorpal.h"
#include "time.h"

//构造函数
COperatorPal::COperatorPal(void)
{
	ZeroMemory(connLocalDB, 20);//初始化服务器IP
	ZeroMemory(petname, 128);//初始化宠物名称
	ZeroMemory(m_sql, 256);//初始化本地查询的sql语句
	sprintf(connLocalDB,"127.0.0.1");
}

//析构函数
COperatorPal::~COperatorPal(void)
{

}

//获取发送的道具名称
void COperatorPal::FindRewardName(char * m_result, char *m_resource)
{
	try
	{
		getRemoteSql("PAL","PAL_GetRewardName", m_sql,4);
		sprintf(m_sql, m_sql, m_resource);
		QuerySingleValue("PAL","127.0.0.1",4,m_sql,m_result);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取发送的道具名称内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取发送的道具名称文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取发送的道具名称其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch(...)
	{
		sprintf(m_result,"");
	}
}

//获取寄卖行物品类型
void COperatorPal::TranslateItemType(char * m_result, char *m_resource)//寄卖行物品类型
{
	try
	{
		getRemoteSql("PAL","PAL_GetItemType", m_sql,4);//获取查询GameDB的sql语句
		sprintf(m_sql, m_sql, m_resource);
		QuerySingleValue("PAL","127.0.0.1",4,m_sql,m_result);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取寄卖行物品类型内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取寄卖行物品类型文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取寄卖行物品类型其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch(...)
	{
		sprintf(m_result,"");
	}
}

//获取寄卖行道具可装备/使用职业
void COperatorPal::TranslateItemVoc(char * m_result, char *m_resource)//寄卖行道具可装备/使用职业
{
	try
	{
		getRemoteSql("PAL","PAL_GetItemVoc", m_sql,4);//获取道具对应的可装备/使用职业
		sprintf(szSql, m_sql, m_resource);
		QuerySingleValue("PAL","127.0.0.1",4,m_sql,m_result);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取寄卖行道具可装备/使用职业内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取寄卖行道具可装备/使用职业文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取寄卖行道具可装备/使用职业其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch(...)
	{
		sprintf(m_result,"");
	}
}

//根据宠物名称获取宠物技能
void COperatorPal::PetSkillToName(char * m_result, char *m_resource,char *petname)
{
	try
	{
		getRemoteSql("PAL","PAL_GetPetSkillName", m_sql,4);//根据宠物名称获取宠物技能
		sprintf(szSql, m_sql, petname, m_resource);
		vector<CGlobalStruct::DataRecord> datarecord;
		if(!datarecord.empty())
		{
			datarecord.clear();
		}	
	//	vector<CGlobalStruct::DataRecord>* datarecord = new vector<CGlobalStruct::DataRecord>;

		m_SqlHelper.SqlHelperMain("PAL",&datarecord, connLocalDB, m_sql, 4, 1, 1);//获取宠物技能

		if(datarecord.empty())//如果记录为空
		{
			sprintf(m_result,"");
			return;
		}
		
		//获取查询的记录集的每一项
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("petskill_name", m_DataRecord.recordfieldname))//获取宠物技能
			{
				sprintf(m_result, "%s", m_DataRecord.recorddata);
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
	//	delete datarecord;
	//	datarecord=NULL;
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"根据宠物名称获取宠物技能内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"根据宠物名称获取宠物技能文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"根据宠物名称获取宠物技能其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch(...)
	{
		sprintf(m_result,"");
	}
}

//将元宝消耗记录中的英文说明改为中文
void COperatorPal::TranslateSYSMEMO(char * m_result, char *m_resource)
{
	try
	{
		char BuyAccount[40];//购买者账号
		char BuyRoleName[40];//购买者角色名
		char GoodsID[40];//物品ID
		char GoodsNum[40];//物品数量
		char AcceptName[40];//接收者角色名
		char Cost[40];//花费

		ZeroMemory(BuyAccount,40);
		ZeroMemory(BuyRoleName,40);
		ZeroMemory(GoodsID,40);
		ZeroMemory(GoodsNum,40);
		ZeroMemory(AcceptName,40);
		ZeroMemory(Cost,40);

		sprintf(m_result,"%s",m_resource);
		int begin=0;//查找的字符串开始处
		int end=0;//查找的字符串结束处
		int length=0;
		int alllength=0;

		alllength=strlen(m_result);

		begin=StringFind(m_result,"<",1);//查找第一个"<"所在的位置
		end=StringFind(m_result,">",1);//查找第一个">"所在的位置
		if((begin != -1) && (end != -1))//如果二者都存在的话
		{
			memcpy((char *)&BuyAccount,(char *)&m_result[begin],end-begin-1);
			BuyAccount[end-begin-1]='\0';//得到购买者的帐号
		}
		else
		{
			sprintf(BuyAccount,"NULL");//购买者帐号为空
		}



		sprintf(m_result,"%s",(char *)&m_result[end]);//将购买者帐号去掉，得到剩下的字符串
		begin=StringFind(m_result,"<",1);//查找第二个"<"所在的位置
		end=StringFind(m_result,">",1);//查找第二个">"所在的位置
		if((begin != -1) && (end != -1))//如果二者都存在的话
		{
			memcpy((char *)&BuyRoleName,(char *)&m_result[begin],end-begin-1);
			BuyRoleName[end-begin-1]='\0';//得到购买者的角色名
		}
		else
		{
			sprintf(BuyRoleName,"NULL");//购买者角色名为空
		}


		sprintf(m_result,"%s",(char *)&m_result[end]);//将购买者角色名去掉，得到剩下的字符串
		begin=StringFind(m_result,"<",1);//查找第三个"<"所在的位置
		end=StringFind(m_result,">",1);//查找第三个">"所在的位置
		if((begin != -1) && (end != -1))//如果二者都存在的话
		{
			memcpy((char *)&AcceptName,(char *)&m_result[begin],end-begin-1);
			AcceptName[end-begin-1]='\0';//得到接收者的角色名
		}
		else
		{
			sprintf(AcceptName,"NULL");//接收者角色名为空
		}



		sprintf(m_result,"%s",(char *)&m_result[end]);//将接收者角色名去掉，得到剩下的字符串
		begin=StringFind(m_result,"<",1);//查找第三个"<"所在的位置
		end=StringFind(m_result,"x",1);//查找第一个"x"所在的位置
		if((begin != -1) && (end != -1))//如果二者都存在的话
		{
			memcpy((char *)&GoodsID,(char *)&m_result[begin],end-begin-1);
			GoodsID[end-begin-1]='\0';//得到物品ID
		}
		else
		{
			sprintf(GoodsID,"NULL");//物品ID为空
		}



		sprintf(m_result,"%s",(char *)&m_result[end-1]);//将物品ID去掉，得到剩下的字符串
		begin=StringFind(m_result,"x ",1);//查找第二个"x"所在的位置
		end=StringFind(m_result,">",1);//查找第三个">"所在的位置
		if((begin != -1) && (end != -1))//如果二者都存在的话
		{
			memcpy((char *)&GoodsNum,(char *)&m_result[begin+1],end-begin-2);
			GoodsNum[end-begin-2]='\0';//得到物品数量
		}
		else
		{
			sprintf(GoodsNum,"NULL");//物品数量为空
		}


		sprintf(m_result,"%s",(char *)&m_result[end]);//将物品数量去掉，得到剩下的字符串
		begin=StringFind(m_result,"<",1);//查找第四个"<"所在的位置
		end=StringFind(m_result,">",1);//查找第四个">"所在的位置
		if((begin != -1) && (end != -1))//如果二者都存在的话
		{
			memcpy((char *)&Cost,(char *)&m_result[begin],end-begin-1);
			Cost[end-begin-1]='\0';//得到花费的金钱
		}
		else
		{
			sprintf(Cost,"NULL");//花费的金钱数量为空
		}


		////////查找名字中文//////////
		getRemoteSql("PAL","PAL_GetGoodsName", m_sql,4);//获取查询物品名称的sql语句
		sprintf(szSql, m_sql, GoodsID);

		vector<CGlobalStruct::DataRecord> datarecord;
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
    	m_SqlHelper.SqlHelperMain("PAL",&datarecord, "127.0.0.1", szSql, 4, 1, 1);//获取对应的LogDBIP

		if(datarecord.empty())//如果记录为空
		{
		}
		else
		{		
			//获取查询的记录集的每一项
			vector<CGlobalStruct::DataRecord>::iterator iter;
			CGlobalStruct::DataRecord  m_DataRecord;	
			for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
			{
				m_DataRecord = *iter;
				if(!strcmp("goodsname", m_DataRecord.recordfieldname))//获取相应的道具名称
				{
					sprintf(GoodsID, "%s", m_DataRecord.recorddata);
				}
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		sprintf(m_result,"购买者帐号<%s>,购买者角色名<%s>,接受者角色名<%s>,物品名<%sX数量%s>,价格<%s>",BuyAccount,BuyRoleName,AcceptName,GoodsID,GoodsNum,Cost);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"将元宝消耗记录中的英文说明改为中文内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"将元宝消耗记录中的英文说明改为中文文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"将元宝消耗记录中的英文说明改为中文其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch(...)
	{
		sprintf(m_result,"");
	}
}

//查找字符串中特定的字符第一次出现的位置
int COperatorPal::StringFind(const char* string,const char* find,int number)
{
	int count = 0;
	try
	{
		char* pos = (char *)string;
		char * p=pos;
		while (number > 0)
		{
			//定义查找到的字符位置的指针，以便临时指针进行遍历
			pos = strstr(p,find);
			//当位置指针为0时，说明没有找到这个字符
			if (pos == 0)
				return -1;
			//当位置指针和临时指针相等说明下一个字符就是要找的字符，如果临时指针小于位置指针，则进行遍历字符串操作，并将count增1
			while(p <= pos)
			{
				//if(*p > 0x80 || *p < 0)
				//{
				//	p++;
				//	}
				p++;
				count++;
			}
			//对要查找的次数减一
			number--;
		}
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查找字符串中特定的字符内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		return -1;	
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查找字符串中特定的字符文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		return -1;	
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查找字符串中特定的字符其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		return -1;	
	}
	catch(...)
	{
		return -1;	
	}
	return count;
}

void COperatorPal::FindLogdbIP(char * logdbip,char * servername)
{
	try
	{
		char sql[256];
		ZeroMemory(sql,256);
		getRemoteSql("PAL","PAL_FindLogDBIP", m_sql,0);//获取查询LogDB的sql语句
		sprintf(sql, m_sql, servername);
		QuerySingleValue("PAL","127.0.0.1",4,sql,logdbip);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查找LogdbIP内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(logdbip,"");
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查找LogdbIP文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(logdbip,"");
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查找LogdbIP其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(logdbip,"");
	}
	catch(...)
	{
		sprintf(logdbip,"");
	}
}
void COperatorPal::FindGameDBIP(char * gamedbip,char * servername)
{
	try
	{
		char sql[256];
		ZeroMemory(sql,256);
		getRemoteSql("PAL","PAL_FindGameDBIP", m_sql,0);//获取查询GameDB的sql语句
		sprintf(sql, m_sql, servername);
		QuerySingleValue("PAL","127.0.0.1",4,sql,gamedbip);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查找GameDBIP内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(gamedbip,"");
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查找GameDBIP文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(gamedbip,"");
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查找GameDBIP其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(gamedbip,"");
	}
	catch(...)
	{
		sprintf(gamedbip,"");
	}
}
void COperatorPal::FindAccountDBIP(char * accountdbip,char * servername)
{
	try
	{
		char sql[256];
		ZeroMemory(sql,256);
		getRemoteSql("PAL","PAL_FindAccountDBIP", m_sql,0);//获取查询AccountDB的sql语句
		sprintf(sql, m_sql, servername);
		QuerySingleValue("PAL","127.0.0.1",4,sql,accountdbip);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查找accountdbip内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(accountdbip,"");
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查找accountdbip文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(accountdbip,"");
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查找accountdbip其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(accountdbip,"");
	}
	catch(...)
	{
		sprintf(accountdbip,"");
	}
}

//获取查询日志的数据库名称
void COperatorPal::FindTableAndSql(int bigtype,int smalltype,char * strTable,char *strsql)
{
	try
	{
		vector<CGlobalStruct::DataRecord> datarecord;
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		
		getRemoteSql("PAL","PAL_FindTableAndSql", m_sql,4);//获取查询数据库名称的sql语句
		sprintf(szSql, m_sql, bigtype, smalltype);
		
		m_SqlHelper.SqlHelperMain("PAL",&datarecord, "127.0.0.1", szSql, 4, 1, 1);//获取对应的LogDBIP
		
		if(datarecord.empty())//如果记录为空
		{
			sprintf(strTable,"");
			sprintf(strsql,"");
			return;
		}
		
		//获取查询的记录集的每一项
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("log_table", m_DataRecord.recordfieldname))//获取相应的数据库名称
			{
				sprintf(strTable, "%s", m_DataRecord.recorddata);
			}
			if(!strcmp("log_sql_flag", m_DataRecord.recordfieldname))
			{
				sprintf(strsql, "%s", m_DataRecord.recorddata);
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
		sprintf(errMessage,"获取查询日志的数据库名称内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(strTable,"");
		sprintf(strsql,"");
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取查询日志的数据库名称文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(strTable,"");
		sprintf(strsql,"");
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取查询日志的数据库名称其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(strTable,"");
		sprintf(strsql,"");
	}
	catch(...)
	{
		sprintf(strTable,"");
		sprintf(strsql,"");
	}
}
//获取查询历史日志的数据库名称
void COperatorPal::FindTableAndSql1(int bigtype,int smalltype,char * strTable,char *strsql)
{
	try
	{
		//		vector<CGlobalStruct::DataRecord>* datarecord = new vector<CGlobalStruct::DataRecord>;
		vector<CGlobalStruct::DataRecord> datarecord;
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		
		getRemoteSql("PAL","PAL_FindTableAndSql1", m_sql,4);//获取查询数据库名称的sql语句
		sprintf(szSql, m_sql, bigtype, smalltype);
		
		m_SqlHelper.SqlHelperMain("PAL",&datarecord, "127.0.0.1", szSql, 4, 1, 1);//获取对应的LogDBIP
		
		if(datarecord.empty())//如果记录为空
		{
			sprintf(strTable,"");
			sprintf(strsql,"");
			return;
		}
		
		//获取查询的记录集的每一项
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("log_table", m_DataRecord.recordfieldname))//获取相应的数据库名称
			{
				sprintf(strTable, "%s", m_DataRecord.recorddata);
			}
			if(!strcmp("log_sql_flag", m_DataRecord.recordfieldname))
			{
				sprintf(strsql, "%s", m_DataRecord.recorddata);
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
		sprintf(errMessage,"获取查询历史日志的数据库名称内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(strTable,"");
		sprintf(strsql,"");
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取查询历史日志的数据库名称文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(strTable,"");
		sprintf(strsql,"");
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"获取查询历史日志的数据库名称其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(strTable,"");
		sprintf(strsql,"");
	}
	catch(...)
	{
		sprintf(strTable,"");
		sprintf(strsql,"");
	}
}


//查询记录数
int COperatorPal::FindRecordNum(char * ip,char * sql,int flag)
{
	int countnum=0;
	try
	{
		vector<CGlobalStruct::DataRecord> datarecord;
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		m_SqlHelper.SqlHelperMain("PAL",&datarecord, ip, sql, flag, 1, 1);//获取对应的LogDBIP
		//m_SqlHelper.SqlHelperMain(&datarecord, ip, sql, flag, 1, 1);
		if(datarecord.empty())//如果记录为空
		{
			return 0;
		}
		//获取查询的记录集的每一项
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("countnum", m_DataRecord.recordfieldname))//获取相应的记录数
			{
				countnum=StrToInt(m_DataRecord.recorddata);
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
		sprintf(errMessage,"查询记录数内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		return -1;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询记录数文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		return -1;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"查询记录数其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		return -1;
	}
	catch(...)
	{
		return -1;
	}
	return countnum;
}



bool COperatorPal::BuildTLV(char * message)
{
	return COperatorGame::BuildTLV(message);
}
bool COperatorPal::BuildTLV(COperVector DBVect)
{
	return COperatorGame::BuildTLV(DBVect);
}
bool COperatorPal::BuildTLV(COperVector m_OpVect,CEnumCore::ServiceKey m_ServiceKey)
{
	try
	{
		int allTFLV = m_OpVect.getAllTFLV();//获取所有的TFLV数量
		if(allTFLV<=0)
		{
			return false;
		}
		SendVectBuf.InitCreate(allTFLV, 1);//初始化发送的数据
		char m_result[256];
		for(int i=0; i < allTFLV; i++)
		{
			CGlobalStruct::TFLV m_tflv = m_OpVect.getTLVBynIndex(i+1);//获取每一个TFLV
		    //元宝消耗记录
		    if(m_ServiceKey == CEnumCore::ServiceKey::PAL_ITEMMAIL_WASTE_RESP)
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::PAL_SYSMEMO)//元宝消耗记录的说明
				{
					//获取中文类型的说明
					ZeroMemory(m_result,256);
					TranslateSYSMEMO(m_result,(char *)&m_tflv.lpdata);
					if(strlen(m_result)>0)
					{
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_SYSMEMO, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}								
				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
				}
				
			}

			//CDKEY道具领取记录
			else if(m_ServiceKey == CEnumCore::ServiceKey::PAL_CDKEY_QUERY_RESP)
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::PAL_ISGOT)//CDKEY道具是否领取
				{
					ZeroMemory(m_result,256);
					int isgot=StrToInt((char *)&m_tflv.lpdata);
					if(isgot==1)//道具已领取
					{
						sprintf(m_result,"%s","是");
					}
					else
					{
						sprintf(m_result,"%s","否");
					}
					SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_ISGOT, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);			
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_REWARDID)//领取的道具ID
				{
					ZeroMemory(m_result,256);
					FindRewardName(m_result,(char *)&m_tflv.lpdata);//获取相应的道具名称
					if(strlen(m_result)>0)
					{
						//保存相应的道具名称
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_REWARDID, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
				}
			
			}

			//日志信息查询
			else if(m_ServiceKey == CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP)
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETNAME)//获取宠物名称
				{
					sprintf(petname,"%s",(char *)&m_tflv.lpdata);//获取宠物名称
					SendVectBuf.AddTFLV(m_tflv);
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETSKILL0)//宠物技能1
				{
					ZeroMemory(m_result,256);
					//根据宠物名称，获取相应的宠物技能
                    PetSkillToName(m_result, (char *)&m_tflv.lpdata, petname);
					if(strlen(m_result)>0)
					{
						//保存相应的宠物技能
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_PETSKILL0, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETSKILL1)//宠物技能2
				{
					ZeroMemory(m_result,256);
					//根据宠物名称，获取相应的宠物技能
                    PetSkillToName(m_result, (char *)&m_tflv.lpdata, petname);
					if(strlen(m_result)>0)
					{
							//保存相应的宠物技能
							SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_PETSKILL1, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETSKILL2)//宠物技能3
				{

					ZeroMemory(m_result,256);
					//根据宠物名称，获取相应的宠物技能
                    PetSkillToName(m_result, (char *)&m_tflv.lpdata, petname);
					if(strlen(m_result)>0)
					{
						//保存相应的宠物技能
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_PETSKILL2, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETSKILL3)//宠物技能4
				{
					ZeroMemory(m_result,256);
					//根据宠物名称，获取相应的宠物技能
                    PetSkillToName(m_result, (char *)&m_tflv.lpdata, petname);
					if(strlen(m_result)>0)
					{
						//保存相应的宠物技能
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_PETSKILL3, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETSKILL4)//宠物技能5
				{
					ZeroMemory(m_result,256);

					//根据宠物名称，获取相应的宠物技能
                    PetSkillToName(m_result, (char *)&m_tflv.lpdata, petname);
					if(strlen(m_result)>0)
					{
						//保存相应的宠物技能
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_PETSKILL4, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETSKILL5)//宠物技能6
				{
					ZeroMemory(m_result,256);
					//根据宠物名称，获取相应的宠物技能
                    PetSkillToName(m_result, (char *)&m_tflv.lpdata, petname);
					if(strlen(m_result)>0)
					{
						//保存相应的宠物技能
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_PETSKILL5, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
				}
			}

			//拍卖信息查询
			else if(m_ServiceKey == CEnumCore::ServiceKey::PAL_AUCTION_QUERY_RESP)
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::PAL_ITEMTYPE)//获取道具类型
				{
					ZeroMemory(m_result,256);
					TranslateItemType(m_result, (char *)&m_tflv.lpdata);//获取道具类型
					if(strlen(m_result)>0)
					{
						//保存相应的道具类型
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_ITEMTYPE, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}								
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_ITEMVOC)
				{
					ZeroMemory(m_result,256);
					TranslateItemVoc(m_result, (char *)&m_tflv.lpdata);//获取道具适用职能
					if(strlen(m_result)>0)
					{
						//保存相应的道具适用职能
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_ITEMVOC, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}					
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_EXPIREDDATE)//过期时间
				{
					ZeroMemory(m_result,256);
					NumTimeToFormatTime(m_result,(char *)&m_tflv.lpdata);//时间格式转换
					if(strlen(m_result)>0)
					{
						//保存相应的道具适用职能
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_EXPIREDDATE, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}	
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_LOCALTIME)//获得时间
				{
					ZeroMemory(m_result,256);
					NumTimeToFormatTime(m_result,(char *)&m_tflv.lpdata);//时间格式转换
					if(strlen(m_result)>0)
					{
						//保存相应的道具适用职能
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_LOCALTIME, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}	
				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
				}
			}
			//邮箱查询
			else if(m_ServiceKey == CEnumCore::ServiceKey::PAL_CHECKMAILBOX_QUERY_RESP)
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::PAL_TIME)//记录时间
				{
					ZeroMemory(m_result,256);
					NumTimeToFormatTime(m_result,(char *)&m_tflv.lpdata);//时间格式转换
					if(strlen(m_result)>0)
					{
						//保存相应的道具适用职能
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_TIME, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}									
				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
				}
			}
			//邮箱附件查询
			else if(m_ServiceKey == CEnumCore::ServiceKey::PAL_CHECKMAILBOX_DETAIL_RESP)
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::PAL_TIME)//记录时间
				{
					ZeroMemory(m_result,256);
					NumTimeToFormatTime(m_result,(char *)&m_tflv.lpdata);//时间格式转换
					if(strlen(m_result)>0)
					{
						//保存相应的道具适用职能
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_TIME, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}									
				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
				}
			}
			else
			{
				SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
			}
		}
		iEnbodyLength= SendVectBuf.EnBody();
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"构建数据包内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"构建数据包文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"构建数据包其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		return false;
	}
	catch(...)
	{
		return false;
	}
	return TRUE;
}


bool COperatorPal::BuildTLV(COperVector m_OpVect,CEnumCore::ServiceKey m_ServiceKey,int PageNum)
{
	try
	{
		int allTFLV = m_OpVect.getAllTFLV();//获取所有的TFLV数量
		if(allTFLV<=0)
		{
			return false;
		}
		SendVectBuf.InitCreate(allTFLV, 1);//初始化发送的数据
		char m_result[256];
		char strInt[10];
		for(int i=0; i < allTFLV; i++)
		{
			CGlobalStruct::TFLV m_tflv = m_OpVect.getTLVBynIndex(i+1);//获取每一个TFLV
				//元宝消耗记录
		    if(m_ServiceKey == CEnumCore::ServiceKey::PAL_ITEMMAIL_WASTE_RESP)
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::PAL_SYSMEMO)//元宝消耗记录的说明
				{
					//获取中文类型的说明
					TranslateSYSMEMO(m_result,(char *)&m_tflv.lpdata);
					if(strlen(m_result)>0)
					{
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_SYSMEMO, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}								
				}
				else
				{
					if(m_tflv.m_tagName == CEnumCore::TagName::PageCount)
					{
						_itoa(PageNum,strInt,10);
						SendVectBuf.AddTFLV(CEnumCore::TagName::PageCount, CEnumCore::TagFormat::TLV_INTEGER, 4, (unsigned char*)&strInt);
					}
					else
					{
						SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
					}
				}
				
			}
			//CDKEY道具领取记录
			else if(m_ServiceKey == CEnumCore::ServiceKey::PAL_CDKEY_QUERY_RESP)
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::PAL_ISGOT)//CDKEY道具是否领取
				{
					ZeroMemory(m_result,256);
					int isgot=StrToInt((char *)&m_tflv.lpdata);
					if(isgot==1)//道具已领取
					{
						sprintf(m_result,"%s","是");
					}
					else
					{
						sprintf(m_result,"%s","否");
					}
					SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_ISGOT, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);			
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_REWARDID)//领取的道具ID
				{
					ZeroMemory(m_result,256);
					FindRewardName(m_result,(char *)&m_tflv.lpdata);//获取相应的道具名称
					if(strlen(m_result)>0)
					{
						//保存相应的道具名称
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_REWARDID, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else
				{
					if(m_tflv.m_tagName == CEnumCore::TagName::PageCount)
					{
						_itoa(PageNum,strInt,10);
						SendVectBuf.AddTFLV(CEnumCore::TagName::PageCount, CEnumCore::TagFormat::TLV_INTEGER, 4, (unsigned char*)&strInt);
					}
					else
					{
						SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
					}
				}
			
			}
			//日志信息查询
			else if(m_ServiceKey == CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP)
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETNAME)//获取宠物名称
				{
					sprintf(petname,"%s",(char *)&m_tflv.lpdata);//获取宠物名称
					SendVectBuf.AddTFLV(m_tflv);
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETSKILL0)//宠物技能1
				{
					ZeroMemory(m_result,256);
					//根据宠物名称，获取相应的宠物技能
                    PetSkillToName(m_result, (char *)&m_tflv.lpdata, petname);
					if(strlen(m_result)>0)
					{
						//保存相应的宠物技能
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_PETSKILL0, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETSKILL1)//宠物技能2
				{
					ZeroMemory(m_result,256);
					//根据宠物名称，获取相应的宠物技能
                    PetSkillToName(m_result, (char *)&m_tflv.lpdata, petname);
					if(strlen(m_result)>0)
					{
						//保存相应的宠物技能
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_PETSKILL1, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETSKILL2)//宠物技能3
				{
					ZeroMemory(m_result,256);
					//根据宠物名称，获取相应的宠物技能
                    PetSkillToName(m_result, (char *)&m_tflv.lpdata, petname);
					if(strlen(m_result)>0)
					{
						//保存相应的宠物技能
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_PETSKILL2, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETSKILL3)//宠物技能4
				{
					ZeroMemory(m_result,256);
					//根据宠物名称，获取相应的宠物技能
                    PetSkillToName(m_result, (char *)&m_tflv.lpdata, petname);
					if(strlen(m_result)>0)
					{
						//保存相应的宠物技能
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_PETSKILL3, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETSKILL4)//宠物技能5
				{
					ZeroMemory(m_result,256);

					//根据宠物名称，获取相应的宠物技能
                    PetSkillToName(m_result, (char *)&m_tflv.lpdata, petname);
					if(strlen(m_result)>0)
					{
						//保存相应的宠物技能
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_PETSKILL4, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETSKILL5)//宠物技能6
				{
					ZeroMemory(m_result,256);
					//根据宠物名称，获取相应的宠物技能
                    PetSkillToName(m_result, (char *)&m_tflv.lpdata, petname);
					if(strlen(m_result)>0)
					{
						//保存相应的宠物技能
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_PETSKILL5, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else
				{
					if(m_tflv.m_tagName == CEnumCore::TagName::PageCount)
					{
						_itoa(PageNum,strInt,10);
						SendVectBuf.AddTFLV(CEnumCore::TagName::PageCount, CEnumCore::TagFormat::TLV_INTEGER, 4, (unsigned char*)&strInt);
					}
					else
					{
						SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
					}
				}
			}
			//拍卖信息查询
			else if(m_ServiceKey == CEnumCore::ServiceKey::PAL_AUCTION_QUERY_RESP)
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::PAL_ITEMTYPE)//获取道具类型
				{
					ZeroMemory(m_result,256);
		            TranslateItemType(m_result, (char *)&m_tflv.lpdata);//获取道具类型
					if(strlen(m_result)>0)
					{
						//保存相应的道具类型
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_ITEMTYPE, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}								
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_ITEMVOC)//获取道具适用职能
				{
					ZeroMemory(m_result,256);
					TranslateItemVoc(m_result, (char *)&m_tflv.lpdata);//获取道具适用职能
					if(strlen(m_result)>0)
					{
						//保存相应的道具适用职能
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_ITEMVOC, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else
				{
					if(m_tflv.m_tagName == CEnumCore::TagName::PageCount)
					{
						_itoa(PageNum,strInt,10);
						SendVectBuf.AddTFLV(CEnumCore::TagName::PageCount, CEnumCore::TagFormat::TLV_INTEGER, 4, (unsigned char*)&strInt);
					}
					else
					{
						SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
					}
				}
			}
			else 
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::PageCount)
				{
					_itoa(PageNum,strInt,10);
					SendVectBuf.AddTFLV(CEnumCore::TagName::PageCount, CEnumCore::TagFormat::TLV_INTEGER, 4, (unsigned char*)&strInt);
				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
				}
			}	
		}
		iEnbodyLength = SendVectBuf.EnBody();
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"构建数据包内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"构建数据包文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"构建数据包其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		return false;
	}
	catch(...)
	{
		return false;
	}

	return TRUE;
}


//"1231012"时间格式转换成"年-月-日 时-分-秒"
void COperatorPal::NumTimeToFormatTime(char * strTime,char * numTime)
{
	time_t t=StrToInt(numTime);
	struct tm *local = localtime(&t);
	sprintf(strTime,"%d-%d-%d %d:%d:%d",local->tm_year+1900,local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min,local->tm_sec);

}


