#include "stdafx.h"
#include ".\operatorGM.h"
#include "time.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <iostream>
#include <vector>
using namespace std;

//���캯��
COperatorGM::COperatorGM(void)
{
}

//��������
COperatorGM::~COperatorGM(void)
{
}



bool COperatorGM::getWebInfo(char *strUrl, char* param, vector<string> &result)
{
	CInternetSession mySession("WA", 0);
	CHttpConnection* lpConnetction;
	CHttpFile* myHttpFile = NULL;
	CString myData; 
	string strConnect = "";
	
	//cout<<param<<"*****************"<<endl;
	//strtok(param, ",");
	//myHttpFile = (CHttpFile *)mySession.OpenURL(strUrl, 1, INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_RELOAD, NULL, 0);
	CString ServerIp = "61.152.150.158";
	DWORD HttpPort = 80;
	lpConnetction = mySession.GetHttpConnection(ServerIp, HttpPort, NULL, NULL);
	
	myHttpFile = lpConnetction->OpenRequest(CHttpConnection::HTTP_VERB_POST, 
		"PayCenter/check_vip_rule.php",
		strUrl,1, NULL, NULL, 
		INTERNET_FLAG_NO_AUTO_REDIRECT);
	
	if (myHttpFile==NULL)
		return false;
	
	string  strHeader = "Content-Type:application/x-www-form-urlencoded";
	strConnect = "username=";
	strConnect += param;
	
	if(!myHttpFile->SendRequest(strHeader.c_str(), (DWORD)strHeader.length(), (void*)strConnect.c_str(),  strConnect.length()))
		return false;
	
	//cout<<"COperatorGM::getWebInfo()---��ʼ��������\n"<<endl;
	DWORD time = GetTickCount();
	while (myHttpFile->ReadString(myData))
	{
		result.push_back((LPCTSTR)myData);
		if(GetTickCount()-time > 5000) 
			break;
	}
	//lpConnetction->Close();

	mySession.Close();
	//cout<<"COperatorGM::getWebInfo()---�����������\n"<<endl;
	return true;
}

string COperatorGM::getXMLNodeAttribute(vector<string>& strList, string nodeName)
{
	//�Ժ������xml�ĵ���ȡ
	
	
	string ret = "RESULT_01";  
	
	typedef vector<string>::iterator Str_Iter;
	Str_Iter it;

	for (it = strList.begin(); it != strList.end(); it++)
	{
		if (strstr(it->c_str(), "RESULT_00"))
		{
			ret = "RESULT_00";
			cout<<"ret: "<<ret.c_str()<<endl;
			return ret;
		}
	}
	cout<<"ret: "<<ret.c_str()<<endl;
	return ret;
}

/*
bool COperatorGM::BuildTLV(vector <CGlobalStruct::TFLV> DBTFLV,CEnumCore::ServiceKey m_ServiceKey)
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
			CGlobalStruct::TFLV m_tflv= *iter;
			SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
			
		}
		iEnbodyLength = SendVectBuf.EnBody();
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�������ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
		return false;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�������ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
		return false;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�����������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
		return false;
	}
	return TRUE;
}

*/

vector<CGlobalStruct::TFLV> COperatorGM::ReturnOpMsg(char* operName, char* result,char *funcName)
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
		logFile.WriteLog("<��ʿ��˫>", operName, const_cast<char *>(strLog.c_str()));
	}
	catch(...)
	{
	}
	
	return DBTFLV;
}