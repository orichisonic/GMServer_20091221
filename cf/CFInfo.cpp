#include "StdAfx.h"
#include "CFInfo.h"


CCFInfo::CCFInfo(void)
{
}

CCFInfo::~CCFInfo(void)
{
}
vector <CGlobalStruct::TFLV> CCFInfo::initialize(CSession * pSess,unsigned char * pbuf, int length)
{
	try
	{
		if(length>0)
		{
			recvVect.Create(length/8);//����ռ�
			recvVect.DeBody(pbuf,length);//�������ṹ
		}
		pGameSess = pSess;	
		CLogic::SocketToUserName(pSess->GetSocket(), operName);//����ִ�в������û���
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ʼ���ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("CF",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ʼ���ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("CF",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ʼ�������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("CF",errMessage);
	}
	return recvVect.GetInnerList();
}



