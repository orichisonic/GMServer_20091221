#include "stdafx.h"
#include ".\OperatorXDJG.h"
#include <iostream>
#include "afxinet.h"
#include <string>
#include <vector>
using namespace std;

//���캯��
COperatorXDJG::COperatorXDJG(void)
{	
}

//��������
COperatorXDJG::~COperatorXDJG(void)
{

}

bool COperatorXDJG::BuildTLV(char * message)
{
	return COperatorGame::BuildTLV(message);
}
bool COperatorXDJG::BuildTLV(vector <CGlobalStruct::TFLV> DBTFLV,CEnumCore::ServiceKey m_ServiceKey,char * ServerIP)
{
	try
	{
		if(DBTFLV.empty())
		{
			return false;
		}
		SendVectBuf.InitCreate(DBTFLV.size()+1, 1);//��ʼ�����͵�����
		
		vector<CGlobalStruct::TFLV>::iterator iter;
		for( iter = DBTFLV.begin(); iter != DBTFLV.end(); iter++ )
		{
// 			CGlobalStruct::TFLV m_tflv= *iter;
// 			SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
			SendVectBuf.AddTFLV(*iter);//Ĭ��Ϊֱ�����TFLV
			
		}
		iEnbodyLength = SendVectBuf.EnBody();
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�������ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("WS",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�������ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("WS",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�����������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("WS",errMessage);
		return false;
	}
	return true;
}

bool COperatorXDJG::getWebInfo(char *strUrl, vector<string> &result)
{
	try
	{
		CInternetSession mySession("XDJG", 0);
		CHttpFile* myHttpFile = NULL;
		CString myData;
		string strInfo; 
		myHttpFile = (CHttpFile *)mySession.OpenURL(strUrl, 1, INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_RELOAD, NULL, 0);
		if (myHttpFile==NULL)
		{
			return false;
		}
		
		while (myHttpFile->ReadString(myData))
		{
			strInfo = (LPCTSTR)myData;
			result.push_back(strInfo);
		}

		mySession.Close();
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool COperatorXDJG::getWebInfo(char *strUrl, string &result)
{
	try
	{
		CInternetSession mySession("XDJG", 0);
		CHttpFile* myHttpFile = NULL;
		CString myData; 
		myHttpFile = (CHttpFile *)mySession.OpenURL(strUrl, 1, INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_RELOAD, NULL, 0);
		if (myHttpFile==NULL)
		{
			return false;
		}
		
		if (myHttpFile->ReadString(myData))
		{
			result = (LPCTSTR)myData;
		}

		mySession.Close();

	}
	catch(...)
	{
		return false;
	}

	return true;
}

//���ӷ�ҳ��������ָ����¼�ֶ�
bool COperatorXDJG::getPageVector(vector<string> &newDataVect, int &iPageCount, vector<string> DataVect, int iIndex, int iPageSize, int iCol)
{
	int iRowCount;
	string label;
	string content;
	if (iPageSize==0 && iIndex==0)
	{
		vector<string>::iterator iter_DataVect;
		for (iter_DataVect=DataVect.begin(); iter_DataVect!=DataVect.end(); iter_DataVect++)
		{
			splitString(*iter_DataVect, label, content);
			newDataVect.push_back(content);
		}
	}
	else
	{
		//�����ҳ����
		int iRecord=0;
		iRowCount = DataVect.size()/iCol;
		if (iRowCount%iPageSize == 0)
		{
			iPageCount = iRowCount/iPageSize;
		}
		else
		{
			iPageCount = iRowCount/iPageSize+1;
		}

		vector<string>::iterator iter_DataVect;
		for (iter_DataVect=DataVect.begin(); iter_DataVect!=DataVect.end(); iter_DataVect++)
		{
			iRecord++;
			if (iRecord>iPageSize*(iIndex-1)*iCol && iRecord<=iPageSize*iIndex*iCol)
			{
				splitString(*iter_DataVect, label, content);
				newDataVect.push_back(content);
				
			}
		}
	}

	return true;
}

//test
bool COperatorXDJG::getPageVector(vector<colInfo> &newDataVect, int &iPageCount, vector<string> DataVect, int iIndex, int iPageSize, int iCol)
{
	int iRowCount;
	string label;
	string content;
	if (iPageSize==0 && iIndex==0)
	{
		vector<string>::iterator iter_DataVect;
		for (iter_DataVect=DataVect.begin(); iter_DataVect!=DataVect.end(); iter_DataVect++)
		{
			splitString(*iter_DataVect, label, content);
			colInfo tempcol;
			tempcol.label = label;
			tempcol.content = content;
			newDataVect.push_back(tempcol);
		}
	}
	else
	{
		//�����ҳ����
		int iRecord=0;
		iRowCount = DataVect.size()/iCol;
		if (iRowCount%iPageSize == 0)
		{
			iPageCount = iRowCount/iPageSize;
		}
		else
		{
			iPageCount = iRowCount/iPageSize+1;
		}

		vector<string>::iterator iter_DataVect;
		for (iter_DataVect=DataVect.begin(); iter_DataVect!=DataVect.end(); iter_DataVect++)
		{
			iRecord++;
			if (iRecord>iPageSize*(iIndex-1)*iCol && iRecord<=iPageSize*iIndex*iCol)
			{
				splitString(*iter_DataVect, label, content);
				colInfo tempcol;
				tempcol.label = label;
				tempcol.content = content;
				newDataVect.push_back(tempcol);
			}
		}
	}

	return true;
}

//���سɹ���ʧ�ܻ�ϵͳ�������Ϣ
vector<CGlobalStruct::TFLV> COperatorXDJG::ReturnOpMsg(char* operName, char* result,char *funcName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV tagtflv;
	string strLog;
	try
	{
		tagtflv.m_tagName=CEnumCore::TagName::MESSAGE;
		tagtflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
		sprintf((char *)&tagtflv.lpdata, "%s", result);
		tagtflv.m_tvlength = strlen((char *)&tagtflv.lpdata);
		DBTFLV.push_back(tagtflv);
		strLog = result;
		strLog += "<";
		strLog += funcName; 
		strLog += ">";  
		logFile.WriteLog("<�������>", operName, const_cast<char *>(strLog.c_str()));
	}
	catch(...)
	{
	}
	
	return DBTFLV;
}

//�����ַ����еı�ʶ������
void COperatorXDJG::splitString(const string strSrc, string &strLabel, string &strContent)
{
	int ilength = strSrc.size();
	int pos = strSrc.find(':');
	strLabel = strSrc.substr(0, pos);
	strContent = strSrc.substr(pos+1, ilength);
	return;
}

//�����ʵ�ķ�����IP�Ͷ˿�
bool COperatorXDJG::GetServerIP(char* DestServerIP, char* SrcServerIP, int gameDBID)
{
	try
	{
		sprintf(szSql,"exec XDJG_FindRealDB '%s',%i",SrcServerIP,gameDBID);
		
		vector<CGlobalStruct::DataRecord> datarecord;
		
		m_SqlHelper.SqlHelperMain("XDJG", &datarecord, connLocalDB, szSql, 4, 1, 1);//��ȡҪ�滻��ֵ
		
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

//������־��¼
void COperatorXDJG::GmOperLog(int UserByID, char * lpServerIp, char *OperName, char * Content, CGlobalStruct::TFLV result)
{
	try
	{
		char message[2048];
		char szSql[2048];
		sprintf(message, "�������:<%s> ��ϸ��Ϣ[%s]", result.lpdata, Content);
		sprintf(szSql, "insert into GMTools_log(UserID, SP_Name, Game_Name, ServerIP, Real_Act) values(%d, '%s', '%s', '%s', '%s')",
			UserByID, OperName, "�������", lpServerIp, message);
		
		m_SqlHelper.SqlHelperMain("127.0.0.1", szSql, 4);
	}
	catch(...)
	{
		return;
	}
}

/*
bool COperatorXDJG::getXmlInfo(char *resq, char *strUrl,char * strpostData,vector<CGlobalStruct::DataRecord> * pVect,int iIndex, int iPageSize)
{
	try
	{
		CoInitialize(NULL);
		IXMLHTTPRequestPtr xmlrequest; //�������
		xmlrequest.CreateInstance("MSXML2.XMLHTTP");
		xmlrequest->open(_bstr_t("Get"),_bstr_t(strUrl),_variant_t("false"));
		xmlrequest->send();
		
		BSTR bstrbody;
		xmlrequest->get_responseText(&bstrbody);
		
		_bstr_t bstrtbody(bstrbody);
		
		CGlobalStruct::DataRecord tmpDataRecord;
		IXMLDOMDocumentPtr   pDoc;  
		IXMLDOMNodeListPtr pNoteList,pSecNoteList;
		IXMLDOMNodePtr  pNode,pChildNote,pSecChildNote;
		
		
		
		HRESULT hr = pDoc.CreateInstance(__uuidof(DOMDocument)); 
		pDoc->loadXML(bstrtbody);
		cout<<static_cast <char*>(pDoc->xml);
		
		pNode=pDoc->selectSingleNode("you9");
		pNoteList=pNode->GetchildNodes();
		
		int ibegin =0, iend=0;
		
		//�����ҳ����
		int row = pNoteList->length;
		if (row < iPageSize*iIndex)
		{
			ibegin = iPageSize * (iIndex-1);
			iend = row;
		}
		else
		{
			ibegin = iPageSize * (iIndex-1);
			iend = iPageSize*iIndex;
		}

		//����xml�и����ڵ������
		for(ibegin; ibegin<iend; ibegin++)
		{
			pChildNote = pNoteList->Getitem(ibegin);
			pSecNoteList = pChildNote->GetchildNodes();
			int col = pSecNoteList->length;
			for(int j=0; j<col; j++)
			{
				pSecChildNote=pSecNoteList->Getitem(j);
				sprintf(tmpDataRecord.recordfieldname,"%s",(LPCSTR)_bstr_t(pSecChildNote->GetnodeName()));
				tmpDataRecord.recordfieldtype=3;
				sprintf(tmpDataRecord.recorddata,"%s",(LPCSTR)_bstr_t(pSecChildNote->text));
				pVect->push_back(tmpDataRecord);
			}
		}
		
		pNode.Release();
		pDoc.Release(); 
		xmlrequest.Release();
		CoUninitialize();
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("WS","��ȡXML�ڴ治��");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("WS","��ȡXML�ļ��쳣");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("WS","��ȡXML�����쳣");
		return false;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("WS",(char *)err.what());
		return false;
	}

	return true;
}*/
