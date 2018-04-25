#include "stdafx.h"
#include ".\operatorpal.h"
#include "time.h"

//���캯��
COperatorPal::COperatorPal(void)
{
	ZeroMemory(connLocalDB, 20);//��ʼ��������IP
	ZeroMemory(petname, 128);//��ʼ����������
	ZeroMemory(m_sql, 256);//��ʼ�����ز�ѯ��sql���
	sprintf(connLocalDB,"127.0.0.1");
}

//��������
COperatorPal::~COperatorPal(void)
{

}

//��ȡ���͵ĵ�������
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
		sprintf(errMessage,"��ȡ���͵ĵ��������ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡ���͵ĵ��������ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡ���͵ĵ������������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch(...)
	{
		sprintf(m_result,"");
	}
}

//��ȡ��������Ʒ����
void COperatorPal::TranslateItemType(char * m_result, char *m_resource)//��������Ʒ����
{
	try
	{
		getRemoteSql("PAL","PAL_GetItemType", m_sql,4);//��ȡ��ѯGameDB��sql���
		sprintf(m_sql, m_sql, m_resource);
		QuerySingleValue("PAL","127.0.0.1",4,m_sql,m_result);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡ��������Ʒ�����ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡ��������Ʒ�����ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡ��������Ʒ���������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch(...)
	{
		sprintf(m_result,"");
	}
}

//��ȡ�����е��߿�װ��/ʹ��ְҵ
void COperatorPal::TranslateItemVoc(char * m_result, char *m_resource)//�����е��߿�װ��/ʹ��ְҵ
{
	try
	{
		getRemoteSql("PAL","PAL_GetItemVoc", m_sql,4);//��ȡ���߶�Ӧ�Ŀ�װ��/ʹ��ְҵ
		sprintf(szSql, m_sql, m_resource);
		QuerySingleValue("PAL","127.0.0.1",4,m_sql,m_result);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡ�����е��߿�װ��/ʹ��ְҵ�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡ�����е��߿�װ��/ʹ��ְҵ�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡ�����е��߿�װ��/ʹ��ְҵ�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch(...)
	{
		sprintf(m_result,"");
	}
}

//���ݳ������ƻ�ȡ���＼��
void COperatorPal::PetSkillToName(char * m_result, char *m_resource,char *petname)
{
	try
	{
		getRemoteSql("PAL","PAL_GetPetSkillName", m_sql,4);//���ݳ������ƻ�ȡ���＼��
		sprintf(szSql, m_sql, petname, m_resource);
		vector<CGlobalStruct::DataRecord> datarecord;
		if(!datarecord.empty())
		{
			datarecord.clear();
		}	
	//	vector<CGlobalStruct::DataRecord>* datarecord = new vector<CGlobalStruct::DataRecord>;

		m_SqlHelper.SqlHelperMain("PAL",&datarecord, connLocalDB, m_sql, 4, 1, 1);//��ȡ���＼��

		if(datarecord.empty())//�����¼Ϊ��
		{
			sprintf(m_result,"");
			return;
		}
		
		//��ȡ��ѯ�ļ�¼����ÿһ��
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("petskill_name", m_DataRecord.recordfieldname))//��ȡ���＼��
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
		sprintf(errMessage,"���ݳ������ƻ�ȡ���＼���ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"���ݳ������ƻ�ȡ���＼���ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"���ݳ������ƻ�ȡ���＼�������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch(...)
	{
		sprintf(m_result,"");
	}
}

//��Ԫ�����ļ�¼�е�Ӣ��˵����Ϊ����
void COperatorPal::TranslateSYSMEMO(char * m_result, char *m_resource)
{
	try
	{
		char BuyAccount[40];//�������˺�
		char BuyRoleName[40];//�����߽�ɫ��
		char GoodsID[40];//��ƷID
		char GoodsNum[40];//��Ʒ����
		char AcceptName[40];//�����߽�ɫ��
		char Cost[40];//����

		ZeroMemory(BuyAccount,40);
		ZeroMemory(BuyRoleName,40);
		ZeroMemory(GoodsID,40);
		ZeroMemory(GoodsNum,40);
		ZeroMemory(AcceptName,40);
		ZeroMemory(Cost,40);

		sprintf(m_result,"%s",m_resource);
		int begin=0;//���ҵ��ַ�����ʼ��
		int end=0;//���ҵ��ַ���������
		int length=0;
		int alllength=0;

		alllength=strlen(m_result);

		begin=StringFind(m_result,"<",1);//���ҵ�һ��"<"���ڵ�λ��
		end=StringFind(m_result,">",1);//���ҵ�һ��">"���ڵ�λ��
		if((begin != -1) && (end != -1))//������߶����ڵĻ�
		{
			memcpy((char *)&BuyAccount,(char *)&m_result[begin],end-begin-1);
			BuyAccount[end-begin-1]='\0';//�õ������ߵ��ʺ�
		}
		else
		{
			sprintf(BuyAccount,"NULL");//�������ʺ�Ϊ��
		}



		sprintf(m_result,"%s",(char *)&m_result[end]);//���������ʺ�ȥ�����õ�ʣ�µ��ַ���
		begin=StringFind(m_result,"<",1);//���ҵڶ���"<"���ڵ�λ��
		end=StringFind(m_result,">",1);//���ҵڶ���">"���ڵ�λ��
		if((begin != -1) && (end != -1))//������߶����ڵĻ�
		{
			memcpy((char *)&BuyRoleName,(char *)&m_result[begin],end-begin-1);
			BuyRoleName[end-begin-1]='\0';//�õ������ߵĽ�ɫ��
		}
		else
		{
			sprintf(BuyRoleName,"NULL");//�����߽�ɫ��Ϊ��
		}


		sprintf(m_result,"%s",(char *)&m_result[end]);//�������߽�ɫ��ȥ�����õ�ʣ�µ��ַ���
		begin=StringFind(m_result,"<",1);//���ҵ�����"<"���ڵ�λ��
		end=StringFind(m_result,">",1);//���ҵ�����">"���ڵ�λ��
		if((begin != -1) && (end != -1))//������߶����ڵĻ�
		{
			memcpy((char *)&AcceptName,(char *)&m_result[begin],end-begin-1);
			AcceptName[end-begin-1]='\0';//�õ������ߵĽ�ɫ��
		}
		else
		{
			sprintf(AcceptName,"NULL");//�����߽�ɫ��Ϊ��
		}



		sprintf(m_result,"%s",(char *)&m_result[end]);//�������߽�ɫ��ȥ�����õ�ʣ�µ��ַ���
		begin=StringFind(m_result,"<",1);//���ҵ�����"<"���ڵ�λ��
		end=StringFind(m_result,"x",1);//���ҵ�һ��"x"���ڵ�λ��
		if((begin != -1) && (end != -1))//������߶����ڵĻ�
		{
			memcpy((char *)&GoodsID,(char *)&m_result[begin],end-begin-1);
			GoodsID[end-begin-1]='\0';//�õ���ƷID
		}
		else
		{
			sprintf(GoodsID,"NULL");//��ƷIDΪ��
		}



		sprintf(m_result,"%s",(char *)&m_result[end-1]);//����ƷIDȥ�����õ�ʣ�µ��ַ���
		begin=StringFind(m_result,"x ",1);//���ҵڶ���"x"���ڵ�λ��
		end=StringFind(m_result,">",1);//���ҵ�����">"���ڵ�λ��
		if((begin != -1) && (end != -1))//������߶����ڵĻ�
		{
			memcpy((char *)&GoodsNum,(char *)&m_result[begin+1],end-begin-2);
			GoodsNum[end-begin-2]='\0';//�õ���Ʒ����
		}
		else
		{
			sprintf(GoodsNum,"NULL");//��Ʒ����Ϊ��
		}


		sprintf(m_result,"%s",(char *)&m_result[end]);//����Ʒ����ȥ�����õ�ʣ�µ��ַ���
		begin=StringFind(m_result,"<",1);//���ҵ��ĸ�"<"���ڵ�λ��
		end=StringFind(m_result,">",1);//���ҵ��ĸ�">"���ڵ�λ��
		if((begin != -1) && (end != -1))//������߶����ڵĻ�
		{
			memcpy((char *)&Cost,(char *)&m_result[begin],end-begin-1);
			Cost[end-begin-1]='\0';//�õ����ѵĽ�Ǯ
		}
		else
		{
			sprintf(Cost,"NULL");//���ѵĽ�Ǯ����Ϊ��
		}


		////////������������//////////
		getRemoteSql("PAL","PAL_GetGoodsName", m_sql,4);//��ȡ��ѯ��Ʒ���Ƶ�sql���
		sprintf(szSql, m_sql, GoodsID);

		vector<CGlobalStruct::DataRecord> datarecord;
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
    	m_SqlHelper.SqlHelperMain("PAL",&datarecord, "127.0.0.1", szSql, 4, 1, 1);//��ȡ��Ӧ��LogDBIP

		if(datarecord.empty())//�����¼Ϊ��
		{
		}
		else
		{		
			//��ȡ��ѯ�ļ�¼����ÿһ��
			vector<CGlobalStruct::DataRecord>::iterator iter;
			CGlobalStruct::DataRecord  m_DataRecord;	
			for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
			{
				m_DataRecord = *iter;
				if(!strcmp("goodsname", m_DataRecord.recordfieldname))//��ȡ��Ӧ�ĵ�������
				{
					sprintf(GoodsID, "%s", m_DataRecord.recorddata);
				}
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		sprintf(m_result,"�������ʺ�<%s>,�����߽�ɫ��<%s>,�����߽�ɫ��<%s>,��Ʒ��<%sX����%s>,�۸�<%s>",BuyAccount,BuyRoleName,AcceptName,GoodsID,GoodsNum,Cost);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��Ԫ�����ļ�¼�е�Ӣ��˵����Ϊ�����ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��Ԫ�����ļ�¼�е�Ӣ��˵����Ϊ�����ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��Ԫ�����ļ�¼�е�Ӣ��˵����Ϊ���������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(m_result,"");
	}
	catch(...)
	{
		sprintf(m_result,"");
	}
}

//�����ַ������ض����ַ���һ�γ��ֵ�λ��
int COperatorPal::StringFind(const char* string,const char* find,int number)
{
	int count = 0;
	try
	{
		char* pos = (char *)string;
		char * p=pos;
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
				//if(*p > 0x80 || *p < 0)
				//{
				//	p++;
				//	}
				p++;
				count++;
			}
			//��Ҫ���ҵĴ�����һ
			number--;
		}
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�����ַ������ض����ַ��ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		return -1;	
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�����ַ������ض����ַ��ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		return -1;	
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�����ַ������ض����ַ������쳣,������Ϣ:%s",errMessage);
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
		getRemoteSql("PAL","PAL_FindLogDBIP", m_sql,0);//��ȡ��ѯLogDB��sql���
		sprintf(sql, m_sql, servername);
		QuerySingleValue("PAL","127.0.0.1",4,sql,logdbip);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"����LogdbIP�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(logdbip,"");
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"����LogdbIP�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(logdbip,"");
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"����LogdbIP�����쳣,������Ϣ:%s",errMessage);
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
		getRemoteSql("PAL","PAL_FindGameDBIP", m_sql,0);//��ȡ��ѯGameDB��sql���
		sprintf(sql, m_sql, servername);
		QuerySingleValue("PAL","127.0.0.1",4,sql,gamedbip);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"����GameDBIP�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(gamedbip,"");
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"����GameDBIP�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(gamedbip,"");
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"����GameDBIP�����쳣,������Ϣ:%s",errMessage);
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
		getRemoteSql("PAL","PAL_FindAccountDBIP", m_sql,0);//��ȡ��ѯAccountDB��sql���
		sprintf(sql, m_sql, servername);
		QuerySingleValue("PAL","127.0.0.1",4,sql,accountdbip);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"����accountdbip�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(accountdbip,"");
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"����accountdbip�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(accountdbip,"");
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"����accountdbip�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(accountdbip,"");
	}
	catch(...)
	{
		sprintf(accountdbip,"");
	}
}

//��ȡ��ѯ��־�����ݿ�����
void COperatorPal::FindTableAndSql(int bigtype,int smalltype,char * strTable,char *strsql)
{
	try
	{
		vector<CGlobalStruct::DataRecord> datarecord;
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		
		getRemoteSql("PAL","PAL_FindTableAndSql", m_sql,4);//��ȡ��ѯ���ݿ����Ƶ�sql���
		sprintf(szSql, m_sql, bigtype, smalltype);
		
		m_SqlHelper.SqlHelperMain("PAL",&datarecord, "127.0.0.1", szSql, 4, 1, 1);//��ȡ��Ӧ��LogDBIP
		
		if(datarecord.empty())//�����¼Ϊ��
		{
			sprintf(strTable,"");
			sprintf(strsql,"");
			return;
		}
		
		//��ȡ��ѯ�ļ�¼����ÿһ��
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("log_table", m_DataRecord.recordfieldname))//��ȡ��Ӧ�����ݿ�����
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
		sprintf(errMessage,"��ȡ��ѯ��־�����ݿ������ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(strTable,"");
		sprintf(strsql,"");
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡ��ѯ��־�����ݿ������ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(strTable,"");
		sprintf(strsql,"");
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡ��ѯ��־�����ݿ����������쳣,������Ϣ:%s",errMessage);
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
//��ȡ��ѯ��ʷ��־�����ݿ�����
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
		
		getRemoteSql("PAL","PAL_FindTableAndSql1", m_sql,4);//��ȡ��ѯ���ݿ����Ƶ�sql���
		sprintf(szSql, m_sql, bigtype, smalltype);
		
		m_SqlHelper.SqlHelperMain("PAL",&datarecord, "127.0.0.1", szSql, 4, 1, 1);//��ȡ��Ӧ��LogDBIP
		
		if(datarecord.empty())//�����¼Ϊ��
		{
			sprintf(strTable,"");
			sprintf(strsql,"");
			return;
		}
		
		//��ȡ��ѯ�ļ�¼����ÿһ��
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("log_table", m_DataRecord.recordfieldname))//��ȡ��Ӧ�����ݿ�����
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
		sprintf(errMessage,"��ȡ��ѯ��ʷ��־�����ݿ������ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(strTable,"");
		sprintf(strsql,"");
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡ��ѯ��ʷ��־�����ݿ������ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		sprintf(strTable,"");
		sprintf(strsql,"");
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡ��ѯ��ʷ��־�����ݿ����������쳣,������Ϣ:%s",errMessage);
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


//��ѯ��¼��
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
		m_SqlHelper.SqlHelperMain("PAL",&datarecord, ip, sql, flag, 1, 1);//��ȡ��Ӧ��LogDBIP
		//m_SqlHelper.SqlHelperMain(&datarecord, ip, sql, flag, 1, 1);
		if(datarecord.empty())//�����¼Ϊ��
		{
			return 0;
		}
		//��ȡ��ѯ�ļ�¼����ÿһ��
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("countnum", m_DataRecord.recordfieldname))//��ȡ��Ӧ�ļ�¼��
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
		sprintf(errMessage,"��ѯ��¼���ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		return -1;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ��¼���ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		return -1;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ��¼�������쳣,������Ϣ:%s",errMessage);
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
		int allTFLV = m_OpVect.getAllTFLV();//��ȡ���е�TFLV����
		if(allTFLV<=0)
		{
			return false;
		}
		SendVectBuf.InitCreate(allTFLV, 1);//��ʼ�����͵�����
		char m_result[256];
		for(int i=0; i < allTFLV; i++)
		{
			CGlobalStruct::TFLV m_tflv = m_OpVect.getTLVBynIndex(i+1);//��ȡÿһ��TFLV
		    //Ԫ�����ļ�¼
		    if(m_ServiceKey == CEnumCore::ServiceKey::PAL_ITEMMAIL_WASTE_RESP)
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::PAL_SYSMEMO)//Ԫ�����ļ�¼��˵��
				{
					//��ȡ�������͵�˵��
					ZeroMemory(m_result,256);
					TranslateSYSMEMO(m_result,(char *)&m_tflv.lpdata);
					if(strlen(m_result)>0)
					{
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_SYSMEMO, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}								
				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
				}
				
			}

			//CDKEY������ȡ��¼
			else if(m_ServiceKey == CEnumCore::ServiceKey::PAL_CDKEY_QUERY_RESP)
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::PAL_ISGOT)//CDKEY�����Ƿ���ȡ
				{
					ZeroMemory(m_result,256);
					int isgot=StrToInt((char *)&m_tflv.lpdata);
					if(isgot==1)//��������ȡ
					{
						sprintf(m_result,"%s","��");
					}
					else
					{
						sprintf(m_result,"%s","��");
					}
					SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_ISGOT, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);			
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_REWARDID)//��ȡ�ĵ���ID
				{
					ZeroMemory(m_result,256);
					FindRewardName(m_result,(char *)&m_tflv.lpdata);//��ȡ��Ӧ�ĵ�������
					if(strlen(m_result)>0)
					{
						//������Ӧ�ĵ�������
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_REWARDID, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
				}
			
			}

			//��־��Ϣ��ѯ
			else if(m_ServiceKey == CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP)
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETNAME)//��ȡ��������
				{
					sprintf(petname,"%s",(char *)&m_tflv.lpdata);//��ȡ��������
					SendVectBuf.AddTFLV(m_tflv);
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETSKILL0)//���＼��1
				{
					ZeroMemory(m_result,256);
					//���ݳ������ƣ���ȡ��Ӧ�ĳ��＼��
                    PetSkillToName(m_result, (char *)&m_tflv.lpdata, petname);
					if(strlen(m_result)>0)
					{
						//������Ӧ�ĳ��＼��
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_PETSKILL0, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETSKILL1)//���＼��2
				{
					ZeroMemory(m_result,256);
					//���ݳ������ƣ���ȡ��Ӧ�ĳ��＼��
                    PetSkillToName(m_result, (char *)&m_tflv.lpdata, petname);
					if(strlen(m_result)>0)
					{
							//������Ӧ�ĳ��＼��
							SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_PETSKILL1, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETSKILL2)//���＼��3
				{

					ZeroMemory(m_result,256);
					//���ݳ������ƣ���ȡ��Ӧ�ĳ��＼��
                    PetSkillToName(m_result, (char *)&m_tflv.lpdata, petname);
					if(strlen(m_result)>0)
					{
						//������Ӧ�ĳ��＼��
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_PETSKILL2, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETSKILL3)//���＼��4
				{
					ZeroMemory(m_result,256);
					//���ݳ������ƣ���ȡ��Ӧ�ĳ��＼��
                    PetSkillToName(m_result, (char *)&m_tflv.lpdata, petname);
					if(strlen(m_result)>0)
					{
						//������Ӧ�ĳ��＼��
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_PETSKILL3, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETSKILL4)//���＼��5
				{
					ZeroMemory(m_result,256);

					//���ݳ������ƣ���ȡ��Ӧ�ĳ��＼��
                    PetSkillToName(m_result, (char *)&m_tflv.lpdata, petname);
					if(strlen(m_result)>0)
					{
						//������Ӧ�ĳ��＼��
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_PETSKILL4, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETSKILL5)//���＼��6
				{
					ZeroMemory(m_result,256);
					//���ݳ������ƣ���ȡ��Ӧ�ĳ��＼��
                    PetSkillToName(m_result, (char *)&m_tflv.lpdata, petname);
					if(strlen(m_result)>0)
					{
						//������Ӧ�ĳ��＼��
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_PETSKILL5, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
				}
			}

			//������Ϣ��ѯ
			else if(m_ServiceKey == CEnumCore::ServiceKey::PAL_AUCTION_QUERY_RESP)
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::PAL_ITEMTYPE)//��ȡ��������
				{
					ZeroMemory(m_result,256);
					TranslateItemType(m_result, (char *)&m_tflv.lpdata);//��ȡ��������
					if(strlen(m_result)>0)
					{
						//������Ӧ�ĵ�������
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_ITEMTYPE, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}								
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_ITEMVOC)
				{
					ZeroMemory(m_result,256);
					TranslateItemVoc(m_result, (char *)&m_tflv.lpdata);//��ȡ��������ְ��
					if(strlen(m_result)>0)
					{
						//������Ӧ�ĵ�������ְ��
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_ITEMVOC, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}					
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_EXPIREDDATE)//����ʱ��
				{
					ZeroMemory(m_result,256);
					NumTimeToFormatTime(m_result,(char *)&m_tflv.lpdata);//ʱ���ʽת��
					if(strlen(m_result)>0)
					{
						//������Ӧ�ĵ�������ְ��
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_EXPIREDDATE, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}	
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_LOCALTIME)//���ʱ��
				{
					ZeroMemory(m_result,256);
					NumTimeToFormatTime(m_result,(char *)&m_tflv.lpdata);//ʱ���ʽת��
					if(strlen(m_result)>0)
					{
						//������Ӧ�ĵ�������ְ��
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_LOCALTIME, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}	
				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
				}
			}
			//�����ѯ
			else if(m_ServiceKey == CEnumCore::ServiceKey::PAL_CHECKMAILBOX_QUERY_RESP)
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::PAL_TIME)//��¼ʱ��
				{
					ZeroMemory(m_result,256);
					NumTimeToFormatTime(m_result,(char *)&m_tflv.lpdata);//ʱ���ʽת��
					if(strlen(m_result)>0)
					{
						//������Ӧ�ĵ�������ְ��
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_TIME, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}									
				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
				}
			}
			//���丽����ѯ
			else if(m_ServiceKey == CEnumCore::ServiceKey::PAL_CHECKMAILBOX_DETAIL_RESP)
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::PAL_TIME)//��¼ʱ��
				{
					ZeroMemory(m_result,256);
					NumTimeToFormatTime(m_result,(char *)&m_tflv.lpdata);//ʱ���ʽת��
					if(strlen(m_result)>0)
					{
						//������Ӧ�ĵ�������ְ��
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_TIME, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}									
				}
				else
				{
					SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
				}
			}
			else
			{
				SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
			}
		}
		iEnbodyLength= SendVectBuf.EnBody();
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�������ݰ��ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�������ݰ��ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�������ݰ������쳣,������Ϣ:%s",errMessage);
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
		int allTFLV = m_OpVect.getAllTFLV();//��ȡ���е�TFLV����
		if(allTFLV<=0)
		{
			return false;
		}
		SendVectBuf.InitCreate(allTFLV, 1);//��ʼ�����͵�����
		char m_result[256];
		char strInt[10];
		for(int i=0; i < allTFLV; i++)
		{
			CGlobalStruct::TFLV m_tflv = m_OpVect.getTLVBynIndex(i+1);//��ȡÿһ��TFLV
				//Ԫ�����ļ�¼
		    if(m_ServiceKey == CEnumCore::ServiceKey::PAL_ITEMMAIL_WASTE_RESP)
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::PAL_SYSMEMO)//Ԫ�����ļ�¼��˵��
				{
					//��ȡ�������͵�˵��
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
						SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
					}
				}
				
			}
			//CDKEY������ȡ��¼
			else if(m_ServiceKey == CEnumCore::ServiceKey::PAL_CDKEY_QUERY_RESP)
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::PAL_ISGOT)//CDKEY�����Ƿ���ȡ
				{
					ZeroMemory(m_result,256);
					int isgot=StrToInt((char *)&m_tflv.lpdata);
					if(isgot==1)//��������ȡ
					{
						sprintf(m_result,"%s","��");
					}
					else
					{
						sprintf(m_result,"%s","��");
					}
					SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_ISGOT, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);			
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_REWARDID)//��ȡ�ĵ���ID
				{
					ZeroMemory(m_result,256);
					FindRewardName(m_result,(char *)&m_tflv.lpdata);//��ȡ��Ӧ�ĵ�������
					if(strlen(m_result)>0)
					{
						//������Ӧ�ĵ�������
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
						SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
					}
				}
			
			}
			//��־��Ϣ��ѯ
			else if(m_ServiceKey == CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP)
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETNAME)//��ȡ��������
				{
					sprintf(petname,"%s",(char *)&m_tflv.lpdata);//��ȡ��������
					SendVectBuf.AddTFLV(m_tflv);
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETSKILL0)//���＼��1
				{
					ZeroMemory(m_result,256);
					//���ݳ������ƣ���ȡ��Ӧ�ĳ��＼��
                    PetSkillToName(m_result, (char *)&m_tflv.lpdata, petname);
					if(strlen(m_result)>0)
					{
						//������Ӧ�ĳ��＼��
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_PETSKILL0, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETSKILL1)//���＼��2
				{
					ZeroMemory(m_result,256);
					//���ݳ������ƣ���ȡ��Ӧ�ĳ��＼��
                    PetSkillToName(m_result, (char *)&m_tflv.lpdata, petname);
					if(strlen(m_result)>0)
					{
						//������Ӧ�ĳ��＼��
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_PETSKILL1, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETSKILL2)//���＼��3
				{
					ZeroMemory(m_result,256);
					//���ݳ������ƣ���ȡ��Ӧ�ĳ��＼��
                    PetSkillToName(m_result, (char *)&m_tflv.lpdata, petname);
					if(strlen(m_result)>0)
					{
						//������Ӧ�ĳ��＼��
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_PETSKILL2, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETSKILL3)//���＼��4
				{
					ZeroMemory(m_result,256);
					//���ݳ������ƣ���ȡ��Ӧ�ĳ��＼��
                    PetSkillToName(m_result, (char *)&m_tflv.lpdata, petname);
					if(strlen(m_result)>0)
					{
						//������Ӧ�ĳ��＼��
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_PETSKILL3, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETSKILL4)//���＼��5
				{
					ZeroMemory(m_result,256);

					//���ݳ������ƣ���ȡ��Ӧ�ĳ��＼��
                    PetSkillToName(m_result, (char *)&m_tflv.lpdata, petname);
					if(strlen(m_result)>0)
					{
						//������Ӧ�ĳ��＼��
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_PETSKILL4, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_PETSKILL5)//���＼��6
				{
					ZeroMemory(m_result,256);
					//���ݳ������ƣ���ȡ��Ӧ�ĳ��＼��
                    PetSkillToName(m_result, (char *)&m_tflv.lpdata, petname);
					if(strlen(m_result)>0)
					{
						//������Ӧ�ĳ��＼��
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
						SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
					}
				}
			}
			//������Ϣ��ѯ
			else if(m_ServiceKey == CEnumCore::ServiceKey::PAL_AUCTION_QUERY_RESP)
			{
				if(m_tflv.m_tagName == CEnumCore::TagName::PAL_ITEMTYPE)//��ȡ��������
				{
					ZeroMemory(m_result,256);
		            TranslateItemType(m_result, (char *)&m_tflv.lpdata);//��ȡ��������
					if(strlen(m_result)>0)
					{
						//������Ӧ�ĵ�������
						SendVectBuf.AddTFLV(CEnumCore::TagName::PAL_ITEMTYPE, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}								
				}
				else if(m_tflv.m_tagName == CEnumCore::TagName::PAL_ITEMVOC)//��ȡ��������ְ��
				{
					ZeroMemory(m_result,256);
					TranslateItemVoc(m_result, (char *)&m_tflv.lpdata);//��ȡ��������ְ��
					if(strlen(m_result)>0)
					{
						//������Ӧ�ĵ�������ְ��
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
						SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
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
					SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
				}
			}	
		}
		iEnbodyLength = SendVectBuf.EnBody();
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�������ݰ��ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�������ݰ��ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�������ݰ������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		return false;
	}
	catch(...)
	{
		return false;
	}

	return TRUE;
}


//"1231012"ʱ���ʽת����"��-��-�� ʱ-��-��"
void COperatorPal::NumTimeToFormatTime(char * strTime,char * numTime)
{
	time_t t=StrToInt(numTime);
	struct tm *local = localtime(&t);
	sprintf(strTime,"%d-%d-%d %d:%d:%d",local->tm_year+1900,local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min,local->tm_sec);

}


