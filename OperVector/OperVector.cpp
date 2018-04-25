#include "stdafx.h"
#include ".\opervector.h"
#include "Windows.h"

COperVector::COperVector(void)
: lpDes(NULL)
{
	queryflag=false;
}

COperVector::COperVector(int querynum,int tflvnum)
: lpDes(NULL)
{
	allquery = querynum;
	colTFLV = tflvnum;
	alltflv = querynum*tflvnum;
}
COperVector::COperVector(vector<CGlobalStruct::TFLV> m_innerList)
: lpDes(NULL)
{
	InnerList=m_innerList;
}
COperVector::~COperVector(void)
{
	Destroy();
}

// ����Ҫ���͵�����
unsigned char * COperVector::getDes()
{
	return lpDes;
}

// �������ݰ������ݵ�Ԫ��
bool COperVector::Create(int m_alltflv)
{
	Destroy();
	alltflv = m_alltflv;
	return false;
}

// ��ʼ���������ݰ���������
bool COperVector::InitCreate(int querynum,int tflvnum)//querynum������,tflvnum������
{
	Destroy();

	allquery = querynum;//���ݵ�Ԫ������
	colTFLV = tflvnum;
	alltflv = 0;//���е����ݵ�Ԫ��
	lpDes = new unsigned char[querynum * tflvnum * sizeof(CGlobalStruct::TFLV)];//�����͵��������ռ�
	ZeroMemory(lpDes, querynum * tflvnum * sizeof(CGlobalStruct::TFLV));//��ʼ����������

	return true;
}

// ������ݵ�Ԫ
bool COperVector::AddTFLV(CEnumCore::TagName tag, CEnumCore::TagFormat tagFormat, int length, unsigned char * lpdata)
{	
	
	CGlobalStruct::TFLV m_tflv;
	int i=0,j=0;
	for(i=0;i<allquery;i++)
	{
		for(j=0;j<colTFLV;j++)
		{
			if(i*colTFLV+j+1 > alltflv)
			{
				m_tflv.nIndex=i*colTFLV+j+1;//���ݵ�Ԫ��������
				m_tflv.m_tagName=tag;//���ݵ�Ԫ��tagName
				m_tflv.m_tagFormat=tagFormat;//���ݵ�Ԫ��tagFormat
				m_tflv.m_tvlength=length;//���ݵ�Ԫ���ֽ���
				wsprintf((LPSTR)&m_tflv.lpdata,"%s",lpdata);//�����ݵ�Ԫ���浽���ݰ���
				InnerList.push_back(m_tflv);
				alltflv++;
				if(queryflag==false)
				{
					queryflag=getActiveCol(tag);
				}
				return TRUE;	
			}
		}
	}
	
	return FALSE;	
}
// ������ݵ�Ԫ
bool COperVector::AddTFLV(CEnumCore::TagName tag, CEnumCore::TagFormat tagFormat, size_t length, unsigned char * lpdata)
{
	
	CGlobalStruct::TFLV m_tflv;
	int i=0,j=0;
	for(i=0;i<allquery;i++)
	{
		for(j=0;j<colTFLV;j++)
		{
			if(i*colTFLV+j+1 > alltflv)
			{
				m_tflv.nIndex=i*colTFLV+j+1;//���ݵ�Ԫ��������
				m_tflv.m_tagName=tag;//���ݵ�Ԫ��tagName
				m_tflv.m_tagFormat=tagFormat;//���ݵ�Ԫ��tagFormat
				m_tflv.m_tvlength= (unsigned int)length;//���ݵ�Ԫ���ֽ���
				wsprintf((LPSTR)&m_tflv.lpdata,"%s",lpdata);//�����ݵ�Ԫ���浽���ݰ���
				InnerList.push_back(m_tflv);
				alltflv++;
				if(queryflag==false)
				{
					queryflag=getActiveCol(tag);//�õ���������
				}
				return TRUE;	
			}
		}
	}
	
	return FALSE;	
}

// ������ݵ�Ԫ
bool COperVector::AddTFLV(CGlobalStruct::TFLV m_tflv)
{
	
	int i=0,j=0;
	for(i=0;i<allquery;i++)
	{
		for(j=0;j<colTFLV;j++)
		{
			if(i*colTFLV+j+1 > alltflv)
			{
				m_tflv.nIndex=i*colTFLV+j+1;//���ݵ�Ԫ��������
				InnerList.push_back(m_tflv);
				alltflv++;
				if(queryflag==false)
				{
					queryflag=getActiveCol(m_tflv.m_tagName);//�õ���������
				}
				return TRUE;	
			}
		}
	}
	
	return FALSE;
}

// �������ݵ�Ԫ
int COperVector::MakeTLV(unsigned char * lpDes, CGlobalStruct::TFLV m_tflv)
{
	int length=0;
	int tmpint=0;
	memcpy(lpDes,(short int  *)&m_tflv.m_tagName,sizeof(short int));
	memcpy(lpDes+sizeof(short int),(short int  *)&m_tflv.m_tagFormat,sizeof(short int));
	memcpy(lpDes+sizeof(short int)+sizeof(short int),(int *)&m_tflv.m_tvlength,sizeof(unsigned int));
	switch(m_tflv.m_tagFormat)
	{
	case CEnumCore::TagFormat::TLV_INTEGER://����������͵����ݵ�Ԫ
		{
				tmpint=StrToInt((LPCSTR)m_tflv.lpdata);
				memcpy(lpDes+sizeof(short int)+sizeof(short int)+sizeof(unsigned int),(int *)&tmpint,4);
				length=4+sizeof(short int)+sizeof(unsigned int)+sizeof(short int);
		}
		break;
	case CEnumCore::TagFormat::TLV_STRING://���string���͵����ݵ�Ԫ
		{
			memcpy(lpDes+sizeof(short int)+sizeof(short int)+sizeof(unsigned int),m_tflv.lpdata,m_tflv.m_tvlength);
			length=m_tflv.m_tvlength+sizeof(short int)+sizeof(unsigned int)+sizeof(short int);
		}
		break;
	default:
		{
			memcpy(lpDes+sizeof(short int)+sizeof(short int)+sizeof(unsigned int),m_tflv.lpdata,m_tflv.m_tvlength);
			length=m_tflv.m_tvlength+sizeof(short int)+sizeof(unsigned int)+sizeof(short int);
		}
		break;
	}
	return length;
}

// �������ݰ�
int COperVector::EnBody(void)
{
	int length=0;
	if(InnerList.empty())
	{
		return 0;
	}
	vector<CGlobalStruct::TFLV>::iterator iter;
	CGlobalStruct::TFLV m_tflv;
	for( iter = InnerList.begin(); iter != InnerList.end(); iter++ )
	{
		m_tflv = *iter;
		length=length+MakeTLV(lpDes+length,m_tflv);//���ݰ����ֽ���
	}
	return length;
}

//ͨ��TFLV�����������ݰ�
int COperVector::EnBody(vector<CGlobalStruct::TFLV> *TFLVList, unsigned char * pDes)
{
	int length=0;
	if(TFLVList->empty())
	{
		return 0;
	}
	vector<CGlobalStruct::TFLV>::iterator iter;
	CGlobalStruct::TFLV m_tflv;
	for( iter = TFLVList->begin(); iter != TFLVList->end(); iter++ )
	{
		m_tflv = *iter;
		length=length+MakeTLV(pDes+length,m_tflv);//���ݰ����ֽ���
	}
	return length;	
}



// �������յ�������
BOOL COperVector::DeBody(unsigned char* lpSou, int bodylength)
{
	int lengthdata=0;
	int i=0,j=0;
	int mJudge = 0;

	int actlength=0;
	while(lengthdata<bodylength)
	{
		i++;
		InnerList.push_back(DeMakeTLV(lpSou+lengthdata, &actlength, i, &mJudge));
		if(mJudge == 2)
		{
			return FALSE;
		}
		else if(mJudge == 1)
		{
		    lengthdata=lengthdata+actlength;
		}
	}
	alltflv=i;

	////////////////////////////////////////һά��ɶ�ά/////////////////////////////////
	CGlobalStruct::TFLV m_basetflv;
	CGlobalStruct::TFLV m_Nexttflv;

	if(InnerList.empty())
	{
		return FALSE;
	}
	vector<CGlobalStruct::TFLV>::iterator iter;
	iter = InnerList.begin();
	m_basetflv = *iter;
	m_Nexttflv = getTLVByTag(m_basetflv.m_tagName,1);

	if(m_Nexttflv.nIndex!=0)
	{
		colTFLV = m_Nexttflv.nIndex-1;
	}
	else
	{
		colTFLV = alltflv;		
	}
	allquery = alltflv/colTFLV;

	return TRUE;
}

// �������յ������ݲ���װ
CGlobalStruct::TFLV COperVector::DeMakeTLV(unsigned char * lpSou,int* m_length, int index, int* m_Judge)
{
	CGlobalStruct::TFLV	m_tflv;
	try
	{		
		unsigned char tmptag[2];
		memcpy(&tmptag,lpSou,2);
		m_tflv.m_tagName=*(short int *)&tmptag;
		unsigned char tmpformat[2];
		memcpy(&tmpformat,lpSou+2,2);
		m_tflv.m_tagFormat=*(short int *)&tmpformat;

		unsigned char tmplength[4];
		memcpy(&tmplength,lpSou+2+2,4);
		m_tflv.m_tvlength=*(int *)&tmplength;

		if(m_tflv.m_tvlength > 2048)
		{
			*m_Judge = 2;
		    CGlobalStruct::TFLV	tmp_tflv;
		    return tmp_tflv;
		}

		unsigned char tmpint[4];
		int actlength=0;
		switch(m_tflv.m_tagFormat)
		{
		case CEnumCore::TagFormat::TLV_INTEGER:
			{
				memcpy(&tmpint,lpSou+2+2+4,4);
				actlength=wsprintf((LPSTR)m_tflv.lpdata,"%i",*(int *)&tmpint);
				m_tflv.lpdata[actlength]='\0';
				*m_length=8+4;
			}
			break;
		case CEnumCore::TagFormat::TLV_STRING:
			{
				memcpy(m_tflv.lpdata,lpSou+2+2+4,m_tflv.m_tvlength);
				m_tflv.lpdata[m_tflv.m_tvlength]='\0';
				*m_length=8+m_tflv.m_tvlength;
			}
			break;
		default:
			{
				memcpy(m_tflv.lpdata,lpSou+2+2+4,m_tflv.m_tvlength);
				m_tflv.lpdata[m_tflv.m_tvlength]='\0';
				*m_length=8+m_tflv.m_tvlength;
			}
			break;
		}
		m_tflv.nIndex = index;
	}
	catch(...)
	{
		*m_Judge = 2;
		CGlobalStruct::TFLV	tmp_tflv;
		return tmp_tflv;
	}
	*m_Judge = 1;
	return m_tflv;
}

// ��ȡ���е�TFLV��Ŀ
int COperVector::getAllTFLV()
{
	return InnerList.size();
}

// ��ȡ���е�Query��Ŀ
int COperVector::getQueryNum()
{
	return allquery;
}

// ��ȡ���е�����
int COperVector::getColTFLVNum()
{
	return colTFLV;
}

// �������ݵ�Ԫ��TagName�Ϳ�ʼ������ѯ���ݵ�Ԫ
CGlobalStruct::TFLV COperVector::getTLVByTag(CEnumCore::TagName tag,int beginIndex)
{
	CGlobalStruct::TFLV m_basetflv;
	CGlobalStruct::TFLV m_tflv;
	m_tflv.nIndex=0;
	ZeroMemory(m_tflv.lpdata,2048);

	if(InnerList.empty())
	{
		return m_tflv;
	}
	vector<CGlobalStruct::TFLV>::iterator iter;
	iter = InnerList.begin();
	for(int i=0;i<beginIndex;i++)
	{
		iter++;
	}
	for( ; iter != InnerList.end(); iter++ )
	{
		m_basetflv = *iter;
		unsigned short tmpTagName = m_basetflv.m_tagName;
		if(tmpTagName == tag)
		{
			m_tflv = m_basetflv;
			break;
		}
	}
	return m_tflv;
}

// �������ݵ�Ԫ��������ѯ���ݵ�Ԫ
CGlobalStruct::TFLV COperVector::getTLVBynIndex(int nIndex)
{
	CGlobalStruct::TFLV m_basetflv;
	CGlobalStruct::TFLV m_tflv;
	m_tflv.nIndex=0;

	if(InnerList.empty())
	{
		return m_tflv;
	}
	vector<CGlobalStruct::TFLV>::iterator iter;
	for( iter = InnerList.begin(); iter != InnerList.end(); iter++ )
	{
		m_basetflv = *iter;
		if(m_basetflv.nIndex == nIndex)
		{
			m_tflv = m_basetflv;
			break;
		}
	}
	return m_tflv;
}

// �������ݵ�Ԫ���С��кŲ�ѯ���ݵ�Ԫ
CGlobalStruct::TFLV COperVector::getTLV(int nRow,int nCol)
{
	CGlobalStruct::TFLV m_basetflv;
	CGlobalStruct::TFLV m_tflv;
	m_tflv.nIndex=0;
	int index = (nRow-1)*colTFLV + nCol;

	if(InnerList.empty())
	{
		return m_tflv;
	}
	vector<CGlobalStruct::TFLV>::iterator iter;
	for( iter = InnerList.begin(); iter != InnerList.end(); iter++ )
	{
		m_basetflv = *iter;
		if(m_basetflv.nIndex==(UINT)index)
		{
			m_tflv = m_basetflv;
			break;
		}
	}
	return m_tflv;
}

// �������ݵ�Ԫ���кź�������ѯ���ݵ�Ԫ
CGlobalStruct::TFLV COperVector::getTLVByName(int nRow,CEnumCore::TagName tag)
{
	CGlobalStruct::TFLV m_basetflv;
	CGlobalStruct::TFLV m_tflv;
	m_tflv.nIndex=0;

	if(InnerList.empty())
	{
		return m_tflv;
	}
	vector<CGlobalStruct::TFLV>::iterator iter;
	iter = InnerList.begin();
	for(int i=0;i<(nRow-1)*colTFLV;i++)
	{
		iter++;
	}
	for( ; iter != InnerList.end(); iter++ )
	{
		m_basetflv = *iter;
		unsigned short tmpTagName = m_basetflv.m_tagName;
		if(tmpTagName == tag)
		{
			m_tflv = m_basetflv;
			break;
		}
	}
	return m_tflv;
}

// �������ݰ�
bool COperVector::Destroy(void)
{
	if(!InnerList.empty())
	{
		InnerList.clear();
	}
	if(lpDes!=NULL)
	{
		delete[] lpDes;
		lpDes=NULL;
	}
	return true;
}

//ִ�в�ѯ����������Ҫ���͵�����
int COperVector::OperVectorMain(char* ip, char* sql, int flag,int nIndex,int nPageSize)
{	
	vector<CGlobalStruct::DataRecord> datarecord;

	if(!datarecord.empty())//��ն���
	{
		datarecord.clear();
	}

	if(!m_SqlHelper.SqlHelperMain(&datarecord, ip, sql, flag,nIndex,nPageSize))//ִ�в�ѯ�������õ����
		return 0;
	
	if(!InnerList.empty())//����б�
	{
		InnerList.clear();
	}

	InitCreate(datarecord.size()/m_SqlHelper.GetFieldNum()+1, m_SqlHelper.GetFieldNum()+5);//��ʼ��������������ռ�
	
	CEnumCore::TagName m_TagName;//�����õ�TagName
	CEnumCore::TagFormat m_TagFormat;//�����õ�TagFormat
	
	CMyEvent m_Event;//��ȡCEvent����
	
	//��ȡ��ѯ�ļ�¼����ÿһ��
	vector<CGlobalStruct::DataRecord>::iterator iter;
	CGlobalStruct::DataRecord  m_DataRecord;	
	for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
	{
		m_DataRecord = *iter;
		m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//��ȡ��¼�����ݵ�TagName
		m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//��ȡ��¼�����ݵ�TagFormat
		AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//����TFLV,����ӵ�vector��
	}

	if(!datarecord.empty())//��ն���
	{
		datarecord.clear();
	}
	int length = EnBody();//�������ݰ����õ�Ҫ���͵�����
	if(length == 0)
	{
		return 0;
	}
	
	return length;
}
//ִ�в�ѯ����������Ҫ���͵�����
int COperVector::OperVectorMain(char * gamename,char* ip, char* sql, int flag,int nIndex,int nPageSize)
{
	vector<CGlobalStruct::DataRecord> datarecord;

	if(!datarecord.empty())
	{
		datarecord.clear();
	}

	if(!m_SqlHelper.SqlHelperMain(gamename,&datarecord, ip, sql, flag,nIndex,nPageSize))//ִ�в�ѯ�������õ����
		return 0;
	
	if(!InnerList.empty())//����б�
	{
		InnerList.clear();
	}
	
	InitCreate(datarecord.size()/m_SqlHelper.GetFieldNum()+1, m_SqlHelper.GetFieldNum()+5);//��ʼ��������������ռ�
	
	CEnumCore::TagName m_TagName;//�����õ�TagName
	CEnumCore::TagFormat m_TagFormat;//�����õ�TagFormat
	
	CMyEvent m_Event;//��ȡCEvent����
	
	//��ȡ��ѯ�ļ�¼����ÿһ��
	vector<CGlobalStruct::DataRecord>::iterator iter;
	CGlobalStruct::DataRecord  m_DataRecord;	
	for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
	{
		m_DataRecord = *iter;
		m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//��ȡ��¼�����ݵ�TagName
		m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//��ȡ��¼�����ݵ�TagFormat
		AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//����TFLV,����ӵ�vector��
	}

	if(!datarecord.empty())//��ն���
	{
		datarecord.clear();
	}
	int length = EnBody();//�������ݰ����õ�Ҫ���͵�����
	if(length == 0)
	{
		return 0;
	}
	
	return length;
}
//ִ�в��������ز������
int COperVector::OperVectorMain(char* ip, char* sql, int flag)
{
	return m_SqlHelper.SqlHelperMain(ip, sql, flag);//ִ�в��������ؽ��
	
}
int COperVector::OperVectorMain(char * gamename,char* ip, char* sql, int flag)
{

	return m_SqlHelper.SqlHelperMain(gamename,ip, sql, flag);//ִ�в��������ؽ��
 //   char result[255]; 
//	m_SqlHelper.SqlHelperMain_char(gamename,ip, sql, flag,result);
//	return 1;
	
}

//maple add
int COperVector::OperVectorMain_char(char * gamename,char* ip, char* sql, int flag,char *str)
{
////	char result[255]; 
	m_SqlHelper.SqlHelperMain_char(gamename,ip, sql, flag,str);
	return 1;
}

//���innerlist
void COperVector::ClearInnerList()
{
	if(!InnerList.empty())
	{
		InnerList.clear();
		alltflv=0;
	}
}
//������������
bool COperVector::getActiveCol(CEnumCore::TagName tag)
{
	CGlobalStruct::TFLV m_basetflv;

	bool firstflag=false;
	int firstindex=0;
	if(InnerList.empty())
	{
		return false;
	}
	vector<CGlobalStruct::TFLV>::iterator iter;
	iter = InnerList.begin();
	for( ; iter != InnerList.end(); iter++ )
	{
		m_basetflv = *iter;
		if(m_basetflv.m_tagName == tag)
		{
			if(firstflag==false)
			{
				firstindex=m_basetflv.nIndex;
				firstflag=true;
			}
			else
			{
				colTFLV=m_basetflv.nIndex-firstindex;
			//	allquery=allquery/colTFLV;
				return true;
			}
			
		}
	}


	return false;
}
//�Ƿ����ù���������
bool COperVector::IsActiveCol()
{
	return queryflag;
}

//ִ�в�ѯ�����ز�ѯ�����
vector<CGlobalStruct::DataRecord> COperVector::GetDBSet(char * gamename,char* ip, char* sql, int flag,int nIndex,int nPageSize)
{
	return 	m_SqlHelper.SqlHelperMain(gamename,ip, sql, flag,nIndex,nPageSize);	
}

//��������������TFLV����
vector<CGlobalStruct::TFLV> COperVector::GetTFLV(vector<CGlobalStruct::DataRecord> m_datarecord)
{
	if(!InnerList.empty())//����б�
	{
		InnerList.clear();
	}
	if(m_datarecord.empty())
		return InnerList;

	InitCreate(m_datarecord.size()/m_SqlHelper.GetFieldNum()+1, m_SqlHelper.GetFieldNum()+5);//��ʼ��������������ռ�

	CEnumCore::TagName m_TagName;//�����õ�TagName
	CEnumCore::TagFormat m_TagFormat;//�����õ�TagFormat
	
	CMyEvent m_Event;//��ȡCEvent����
	
	//��ȡ��ѯ�ļ�¼����ÿһ��
	vector<CGlobalStruct::DataRecord>::iterator iter;
	CGlobalStruct::DataRecord  DataRecord;	
	for( iter = m_datarecord.begin(); iter != m_datarecord.end(); iter++ )
	{
		DataRecord = *iter;
		m_TagName = m_Event.GetTagName((char* )&DataRecord.recordfieldname);//��ȡ��¼�����ݵ�TagName
		m_TagFormat = m_Event.GetTagFormat((char* )&DataRecord.recordfieldtype);//��ȡ��¼�����ݵ�TagFormat
		AddTFLV(m_TagName, m_TagFormat, strlen(DataRecord.recorddata), (unsigned char*)&DataRecord.recorddata);//����TFLV,����ӵ�vector��
	}
	return InnerList;
}

vector<CGlobalStruct::TFLV> COperVector::GetInnerList()
{
	return InnerList;
}