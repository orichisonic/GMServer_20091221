#include "stdafx.h"
#include ".\OperatorGT.h"

//构造函数
COperatorGT::COperatorGT(void)
{
	ZeroMemory(connLocalDB, 20);//初始化服务器IP
	ZeroMemory(m_sql, 256);//初始化本地查询的sql语句
	sprintf(connLocalDB,"127.0.0.1");

	psText = new char[2048];
	ZeroMemory( psText, 2048 );//初始化char*类型变量
	pwText = new wchar_t[2048];
	ZeroMemory( pwText, 2048 );//初始化wchar_t*类型变量
}

//析构函数
COperatorGT::~COperatorGT(void)
{
	if(psText != NULL)
	{
		delete []psText;
		psText = NULL;
	}
	if(pwText != NULL)
	{
		delete []pwText;
		pwText = NULL;
	}
}


// 宽字节转为字节
char* COperatorGT::wcharToChar(wstring str)
{
	ZeroMemory( psText, 2048 );
	int length = str.length();
	//DWORD size = WideCharToMultiByte(CP_ACP, NULL, str.c_str(), -1, NULL, 0, NULL, NULL);    
	WideCharToMultiByte(CP_ACP, NULL, str.c_str(), -1, psText, length*2, NULL, NULL);
	//psText[length+1]=0;
	return psText;
}

// 字节转为宽字节
wchar_t* COperatorGT::charToWChar(char* str)
{
	ZeroMemory( pwText, 2048 );
	DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);	
	MultiByteToWideChar(CP_ACP, 0, str, -1, pwText, dwNum);
	pwText[dwNum+1]='\0';
	return pwText;
}

//获取需要的字符串
bool COperatorGT::getSpecChar(const char* string, const char* find, int nNum, const char* findNext, int mNum, char* nChar)
{
	int nCount = 0;
	int mCount = 0;
	if(strcmp("",find))
	{
		nCount = StringFind(string, find, nNum);//获取相应的第一个指定字符的位置
	}
	if(strcmp("",findNext))
	{
		mCount = StringFind(string, findNext, mNum);//获取相应的第二个指定字符的位置
	}
	if(mCount > 0)
	{
		memcpy(nChar, (char*)&string[nCount], mCount-nCount-1);
	}
	else
	{
		sprintf(nChar, "%s", (char*)&string[nCount]);
	}
	
	return true;
}

//查找字符串中特定的字符第一次出现的位置
int COperatorGT::StringFind(const char* string,const char* find,int number)
{
	char* pos = (char *)string;
	char *p=pos;
	int count = 0;
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
			p++;
			count++;
		}
		//对要查找的次数减一
		number--;
	}
	return count;
}

//获得真实的服务器IP和端口
bool COperatorGT::GetServerIP(char* gameName, char* SrcServerIP, int gameDBID, char* DestServerIP, int* DestPort)
{
	try
	{
		getRemoteSql("GT","GetServerIP",m_sql,4);
		if(!strcmp("",m_sql))
		{
			return false;
		}
		sprintf(szSql,m_sql,SrcServerIP,gameDBID);

		vector<CGlobalStruct::DataRecord> datarecord;
		if(strcmp("",gameName))
		{
			m_SqlHelper.SqlHelperMain(gameName, &datarecord, connLocalDB, szSql, 0, 1, 1);//获取要替换的值
		}
		else
		{
			m_SqlHelper.SqlHelperMain(&datarecord, connLocalDB, szSql, 0, 1, 1);//获取要替换的值
		}		
		
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
			if(!strcmp("ServerIP", m_DataRecord.recordfieldname))//获取相应的ServerIP
			{
				sprintf(DestServerIP, m_DataRecord.recorddata);//保存查询得到的第一条记录
			}
			else if(!strcmp("Port", m_DataRecord.recordfieldname))//获取相应的Port
			{
				*DestPort = StrToInt(m_DataRecord.recorddata);//保存查询得到的第一条记录
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}


bool COperatorGT::BuildTLV(char * message)
{
	return COperatorGame::BuildTLV(message);
}
bool COperatorGT::BuildTLV(COperVector DBVect)
{
	return COperatorGame::BuildTLV(DBVect);
}