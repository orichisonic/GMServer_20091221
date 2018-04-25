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

// 返回要发送的数据
unsigned char * COperVector::getDes()
{
	return lpDes;
}

// 分配数据包的数据单元数
bool COperVector::Create(int m_alltflv)
{
	Destroy();
	alltflv = m_alltflv;
	return false;
}

// 初始化分配数据包的行列数
bool COperVector::InitCreate(int querynum,int tflvnum)//querynum是行数,tflvnum是列数
{
	Destroy();

	allquery = querynum;//数据单元的行数
	colTFLV = tflvnum;
	alltflv = 0;//所有的数据单元数
	lpDes = new unsigned char[querynum * tflvnum * sizeof(CGlobalStruct::TFLV)];//给发送的数组分配空间
	ZeroMemory(lpDes, querynum * tflvnum * sizeof(CGlobalStruct::TFLV));//初始化发送数组

	return true;
}

// 添加数据单元
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
				m_tflv.nIndex=i*colTFLV+j+1;//数据单元的索引号
				m_tflv.m_tagName=tag;//数据单元的tagName
				m_tflv.m_tagFormat=tagFormat;//数据单元的tagFormat
				m_tflv.m_tvlength=length;//数据单元的字节数
				wsprintf((LPSTR)&m_tflv.lpdata,"%s",lpdata);//将数据单元保存到数据包中
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
// 添加数据单元
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
				m_tflv.nIndex=i*colTFLV+j+1;//数据单元的索引号
				m_tflv.m_tagName=tag;//数据单元的tagName
				m_tflv.m_tagFormat=tagFormat;//数据单元的tagFormat
				m_tflv.m_tvlength= (unsigned int)length;//数据单元的字节数
				wsprintf((LPSTR)&m_tflv.lpdata,"%s",lpdata);//将数据单元保存到数据包中
				InnerList.push_back(m_tflv);
				alltflv++;
				if(queryflag==false)
				{
					queryflag=getActiveCol(tag);//得到真正的列
				}
				return TRUE;	
			}
		}
	}
	
	return FALSE;	
}

// 添加数据单元
bool COperVector::AddTFLV(CGlobalStruct::TFLV m_tflv)
{
	
	int i=0,j=0;
	for(i=0;i<allquery;i++)
	{
		for(j=0;j<colTFLV;j++)
		{
			if(i*colTFLV+j+1 > alltflv)
			{
				m_tflv.nIndex=i*colTFLV+j+1;//数据单元的索引号
				InnerList.push_back(m_tflv);
				alltflv++;
				if(queryflag==false)
				{
					queryflag=getActiveCol(m_tflv.m_tagName);//得到真正的列
				}
				return TRUE;	
			}
		}
	}
	
	return FALSE;
}

// 构造数据单元
int COperVector::MakeTLV(unsigned char * lpDes, CGlobalStruct::TFLV m_tflv)
{
	int length=0;
	int tmpint=0;
	memcpy(lpDes,(short int  *)&m_tflv.m_tagName,sizeof(short int));
	memcpy(lpDes+sizeof(short int),(short int  *)&m_tflv.m_tagFormat,sizeof(short int));
	memcpy(lpDes+sizeof(short int)+sizeof(short int),(int *)&m_tflv.m_tvlength,sizeof(unsigned int));
	switch(m_tflv.m_tagFormat)
	{
	case CEnumCore::TagFormat::TLV_INTEGER://添加数据类型的数据单元
		{
				tmpint=StrToInt((LPCSTR)m_tflv.lpdata);
				memcpy(lpDes+sizeof(short int)+sizeof(short int)+sizeof(unsigned int),(int *)&tmpint,4);
				length=4+sizeof(short int)+sizeof(unsigned int)+sizeof(short int);
		}
		break;
	case CEnumCore::TagFormat::TLV_STRING://添加string类型的数据单元
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

// 构造数据包
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
		length=length+MakeTLV(lpDes+length,m_tflv);//数据包的字节数
	}
	return length;
}

//通过TFLV链表，构造数据包
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
		length=length+MakeTLV(pDes+length,m_tflv);//数据包的字节数
	}
	return length;	
}



// 解析接收到的数据
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

	////////////////////////////////////////一维变成二维/////////////////////////////////
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

// 解析接收到的数据并封装
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

// 获取所有的TFLV数目
int COperVector::getAllTFLV()
{
	return InnerList.size();
}

// 获取所有的Query数目
int COperVector::getQueryNum()
{
	return allquery;
}

// 获取所有的列数
int COperVector::getColTFLVNum()
{
	return colTFLV;
}

// 利用数据单元的TagName和开始索引查询数据单元
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

// 利用数据单元的索引查询数据单元
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

// 利用数据单元的行、列号查询数据单元
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

// 利用数据单元的行号和索引查询数据单元
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

// 销毁数据包
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

//执行查询，构造最终要发送的数据
int COperVector::OperVectorMain(char* ip, char* sql, int flag,int nIndex,int nPageSize)
{	
	vector<CGlobalStruct::DataRecord> datarecord;

	if(!datarecord.empty())//清空队列
	{
		datarecord.clear();
	}

	if(!m_SqlHelper.SqlHelperMain(&datarecord, ip, sql, flag,nIndex,nPageSize))//执行查询操作并得到结果
		return 0;
	
	if(!InnerList.empty())//清空列表
	{
		InnerList.clear();
	}

	InitCreate(datarecord.size()/m_SqlHelper.GetFieldNum()+1, m_SqlHelper.GetFieldNum()+5);//初始化变量，并分配空间
	
	CEnumCore::TagName m_TagName;//保存获得的TagName
	CEnumCore::TagFormat m_TagFormat;//保存获得的TagFormat
	
	CMyEvent m_Event;//获取CEvent对象
	
	//获取查询的记录集的每一项
	vector<CGlobalStruct::DataRecord>::iterator iter;
	CGlobalStruct::DataRecord  m_DataRecord;	
	for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
	{
		m_DataRecord = *iter;
		m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//获取记录集数据的TagName
		m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//获取记录集数据的TagFormat
		AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//构造TFLV,并添加到vector中
	}

	if(!datarecord.empty())//清空队列
	{
		datarecord.clear();
	}
	int length = EnBody();//解析数据包，得到要发送的数组
	if(length == 0)
	{
		return 0;
	}
	
	return length;
}
//执行查询，构造最终要发送的数据
int COperVector::OperVectorMain(char * gamename,char* ip, char* sql, int flag,int nIndex,int nPageSize)
{
	vector<CGlobalStruct::DataRecord> datarecord;

	if(!datarecord.empty())
	{
		datarecord.clear();
	}

	if(!m_SqlHelper.SqlHelperMain(gamename,&datarecord, ip, sql, flag,nIndex,nPageSize))//执行查询操作并得到结果
		return 0;
	
	if(!InnerList.empty())//清空列表
	{
		InnerList.clear();
	}
	
	InitCreate(datarecord.size()/m_SqlHelper.GetFieldNum()+1, m_SqlHelper.GetFieldNum()+5);//初始化变量，并分配空间
	
	CEnumCore::TagName m_TagName;//保存获得的TagName
	CEnumCore::TagFormat m_TagFormat;//保存获得的TagFormat
	
	CMyEvent m_Event;//获取CEvent对象
	
	//获取查询的记录集的每一项
	vector<CGlobalStruct::DataRecord>::iterator iter;
	CGlobalStruct::DataRecord  m_DataRecord;	
	for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
	{
		m_DataRecord = *iter;
		m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//获取记录集数据的TagName
		m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//获取记录集数据的TagFormat
		AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//构造TFLV,并添加到vector中
	}

	if(!datarecord.empty())//清空队列
	{
		datarecord.clear();
	}
	int length = EnBody();//解析数据包，得到要发送的数组
	if(length == 0)
	{
		return 0;
	}
	
	return length;
}
//执行操作，返回操作结果
int COperVector::OperVectorMain(char* ip, char* sql, int flag)
{
	return m_SqlHelper.SqlHelperMain(ip, sql, flag);//执行操作并返回结果
	
}
int COperVector::OperVectorMain(char * gamename,char* ip, char* sql, int flag)
{

	return m_SqlHelper.SqlHelperMain(gamename,ip, sql, flag);//执行操作并返回结果
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

//清空innerlist
void COperVector::ClearInnerList()
{
	if(!InnerList.empty())
	{
		InnerList.clear();
		alltflv=0;
	}
}
//设置真正列数
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
//是否设置过真正列数
bool COperVector::IsActiveCol()
{
	return queryflag;
}

//执行查询，返回查询结果集
vector<CGlobalStruct::DataRecord> COperVector::GetDBSet(char * gamename,char* ip, char* sql, int flag,int nIndex,int nPageSize)
{
	return 	m_SqlHelper.SqlHelperMain(gamename,ip, sql, flag,nIndex,nPageSize);	
}

//处理结果集，构造TFLV数组
vector<CGlobalStruct::TFLV> COperVector::GetTFLV(vector<CGlobalStruct::DataRecord> m_datarecord)
{
	if(!InnerList.empty())//清空列表
	{
		InnerList.clear();
	}
	if(m_datarecord.empty())
		return InnerList;

	InitCreate(m_datarecord.size()/m_SqlHelper.GetFieldNum()+1, m_SqlHelper.GetFieldNum()+5);//初始化变量，并分配空间

	CEnumCore::TagName m_TagName;//保存获得的TagName
	CEnumCore::TagFormat m_TagFormat;//保存获得的TagFormat
	
	CMyEvent m_Event;//获取CEvent对象
	
	//获取查询的记录集的每一项
	vector<CGlobalStruct::DataRecord>::iterator iter;
	CGlobalStruct::DataRecord  DataRecord;	
	for( iter = m_datarecord.begin(); iter != m_datarecord.end(); iter++ )
	{
		DataRecord = *iter;
		m_TagName = m_Event.GetTagName((char* )&DataRecord.recordfieldname);//获取记录集数据的TagName
		m_TagFormat = m_Event.GetTagFormat((char* )&DataRecord.recordfieldtype);//获取记录集数据的TagFormat
		AddTFLV(m_TagName, m_TagFormat, strlen(DataRecord.recorddata), (unsigned char*)&DataRecord.recorddata);//构造TFLV,并添加到vector中
	}
	return InnerList;
}

vector<CGlobalStruct::TFLV> COperVector::GetInnerList()
{
	return InnerList;
}