#include "stdafx.h"
#include ".\OperatorGT.h"

//���캯��
COperatorGT::COperatorGT(void)
{
	ZeroMemory(connLocalDB, 20);//��ʼ��������IP
	ZeroMemory(m_sql, 256);//��ʼ�����ز�ѯ��sql���
	sprintf(connLocalDB,"127.0.0.1");

	psText = new char[2048];
	ZeroMemory( psText, 2048 );//��ʼ��char*���ͱ���
	pwText = new wchar_t[2048];
	ZeroMemory( pwText, 2048 );//��ʼ��wchar_t*���ͱ���
}

//��������
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


// ���ֽ�תΪ�ֽ�
char* COperatorGT::wcharToChar(wstring str)
{
	ZeroMemory( psText, 2048 );
	int length = str.length();
	//DWORD size = WideCharToMultiByte(CP_ACP, NULL, str.c_str(), -1, NULL, 0, NULL, NULL);    
	WideCharToMultiByte(CP_ACP, NULL, str.c_str(), -1, psText, length*2, NULL, NULL);
	//psText[length+1]=0;
	return psText;
}

// �ֽ�תΪ���ֽ�
wchar_t* COperatorGT::charToWChar(char* str)
{
	ZeroMemory( pwText, 2048 );
	DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);	
	MultiByteToWideChar(CP_ACP, 0, str, -1, pwText, dwNum);
	pwText[dwNum+1]='\0';
	return pwText;
}

//��ȡ��Ҫ���ַ���
bool COperatorGT::getSpecChar(const char* string, const char* find, int nNum, const char* findNext, int mNum, char* nChar)
{
	int nCount = 0;
	int mCount = 0;
	if(strcmp("",find))
	{
		nCount = StringFind(string, find, nNum);//��ȡ��Ӧ�ĵ�һ��ָ���ַ���λ��
	}
	if(strcmp("",findNext))
	{
		mCount = StringFind(string, findNext, mNum);//��ȡ��Ӧ�ĵڶ���ָ���ַ���λ��
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

//�����ַ������ض����ַ���һ�γ��ֵ�λ��
int COperatorGT::StringFind(const char* string,const char* find,int number)
{
	char* pos = (char *)string;
	char *p=pos;
	int count = 0;
	while (number > 0)
	{
		//������ҵ����ַ�λ�õ�ָ�룬�Ա���ʱָ����б���
		pos = strstr(p,find);
		//��λ��ָ��Ϊ0ʱ��˵��û���ҵ�����ַ�
		if (pos == 0)
			return -1;
		//��λ��ָ�����ʱָ�����˵����һ���ַ�����Ҫ�ҵ��ַ��������ʱָ��С��λ��ָ�룬����б����ַ�������������count��1
		while(p <= pos)
		{
			p++;
			count++;
		}
		//��Ҫ���ҵĴ�����һ
		number--;
	}
	return count;
}

//�����ʵ�ķ�����IP�Ͷ˿�
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
			m_SqlHelper.SqlHelperMain(gameName, &datarecord, connLocalDB, szSql, 0, 1, 1);//��ȡҪ�滻��ֵ
		}
		else
		{
			m_SqlHelper.SqlHelperMain(&datarecord, connLocalDB, szSql, 0, 1, 1);//��ȡҪ�滻��ֵ
		}		
		
		if(datarecord.empty())//�����¼Ϊ��
		{
			return false;
		}
		
		//��ȡ��ѯ�ļ�¼����ÿһ��
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("ServerIP", m_DataRecord.recordfieldname))//��ȡ��Ӧ��ServerIP
			{
				sprintf(DestServerIP, m_DataRecord.recorddata);//�����ѯ�õ��ĵ�һ����¼
			}
			else if(!strcmp("Port", m_DataRecord.recordfieldname))//��ȡ��Ӧ��Port
			{
				*DestPort = StrToInt(m_DataRecord.recorddata);//�����ѯ�õ��ĵ�һ����¼
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