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
			recvVect.Create(length/8);//分配空间
			recvVect.DeBody(pbuf,length);//解析包结构
		}
		pGameSess = pSess;	
		CLogic::SocketToUserName(pSess->GetSocket(), operName);//保存执行操作的用户名
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"初始化内存不足,错误信息:%s",errMessage);
		logFile.WriteErrorLog("CF",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"初始化文件异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("CF",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"初始化其他异常,错误信息:%s",errMessage);
		logFile.WriteErrorLog("CF",errMessage);
	}
	return recvVect.GetInnerList();
}



