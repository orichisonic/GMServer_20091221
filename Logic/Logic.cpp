// Logic.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "Logic.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

// 这是导出变量的一个示例
LOGIC_API int nLogic=0;

// 这是导出函数的一个示例。
LOGIC_API int fnLogic(void)
{
	return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 Logic.h
CLogic::CLogic()
{ 
	return; 
}

CLogic::~CLogic()
{
	
}

//构造logStruct结构体
void CLogic::AddLog(SOCKET m_socket,int m_userID,char * m_UserName,char * m_realname,char * m_clientIP)
{
	for(int i=0;i<256;i++)
	{
		if(logStruct[i].userID==0)
		{
			logStruct[i].comsocket=m_socket;
			logStruct[i].userID=m_userID;
			sprintf(logStruct[i].username,m_UserName);
			sprintf(logStruct[i].realname,m_realname);
			sprintf(logStruct[i].clientIP,m_clientIP);
			break;
		}
	}
}

//根据SOCKET，删除相应的用户信息
void CLogic::DelLog(SOCKET  m_socket)
{
	for(int i=0;i<256;i++)
	{
		if(logStruct[i].comsocket==m_socket)
		{
			logStruct[i].userID=0;
			logStruct[i].comsocket=INVALID_SOCKET;
			ZeroMemory(logStruct[i].username,256);
			ZeroMemory(logStruct[i].realname,256);
			ZeroMemory(logStruct[i].clientIP,256);
			break;
		}
	}
}

//根据用户的ID获得用户名
void CLogic::FindUserName(int m_userID,char * m_UserName)
{
	for(int i=0;i<256;i++)
	{
		if(logStruct[i].userID==m_userID)
		{
			sprintf(m_UserName,logStruct[i].username);
			break;
		}
	}
}

//根据SOCKET，得到相应的用户名
void CLogic::SocketToUserName(SOCKET m_socket,char * m_UserName)
{
	for(int i=0;i<256;i++)
	{
		if(logStruct[i].comsocket==m_socket)
		{
			sprintf(m_UserName,logStruct[i].username);
			break;
		}
	}

}

//根据SOCKET，得到相应的用户真实姓名
void CLogic::SocketToRealName(SOCKET m_socket,char * m_realname)
{
	for(int i=0;i<256;i++)
	{
		if(logStruct[i].comsocket==m_socket)
		{
			sprintf(m_realname,logStruct[i].realname);
			break;
		}
	}
}

//根据SOCKET，得到相应的用户ID
int CLogic::SocketToUserID(SOCKET m_socket)
{
	int userid=0;
	for(int i=0;i<256;i++)
	{
		if(logStruct[i].comsocket==m_socket)
		{
			userid=logStruct[i].userID;
			break;
		}
	}
	return userid;
}

//返回消息 
void CLogic::ReturnMessage(SOCKET m_socket,unsigned int nid,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,char * lpMessage)
{
	try
	{
		CMSocket msocket;
		COperVector opervector;
		CSocketData socketdata;
		int alllength=0;

		msocket.SetSocket(m_socket);
		opervector.InitCreate(1,2);


		opervector.AddTFLV(CEnumCore::TagName::MESSAGE,CEnumCore::TagFormat::TLV_STRING,strlen(lpMessage),(unsigned char*)lpMessage);

		alllength=0;


		alllength=opervector.EnBody();

		socketdata.Create(alllength);
		socketdata.AddHeader(nid,m_category,mServiceKey,alllength);


		socketdata.EnSendBuf(&alllength,opervector.getDes(),alllength);

		msocket.SendBuf((char *)socketdata.GetSendBuf(),alllength);
		socketdata.Destory();
		opervector.Destroy();
	}
	catch(...)
	{
		
	}
	
}

//返回消息 
void CLogic::ReturnMessage(CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,char * lpMessage,unsigned char * m_lpRetbuf, int * m_strNum)
{
	try
	{

		COperVector opervector;
		CSocketData socketdata;
		int alllength=0;
		

		opervector.InitCreate(1,2);
		
		
		opervector.AddTFLV(CEnumCore::TagName::MESSAGE,CEnumCore::TagFormat::TLV_STRING,strlen(lpMessage),(unsigned char*)lpMessage);
		
		alllength=0;
		
		
		alllength=opervector.EnBody();
		
		socketdata.Create(alllength);
		socketdata.AddHeader(1,m_category,mServiceKey,alllength);
		
		
		socketdata.EnSendBuf(&alllength,opervector.getDes(),alllength);
		
		memcpy(m_lpRetbuf,socketdata.GetSendBuf(),alllength);
		*m_strNum=alllength;

		socketdata.Destory();
		opervector.Destroy();
	}
	catch(...)
	{
		
	}
	
}
//返回数据char *
void CLogic::ReturnData(SOCKET m_socket,unsigned int nid,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,unsigned char * lpdata,int length)
{
	try
	{
		if(length == 0)
		{
			ReturnMessage(m_socket,nid, m_category, mServiceKey, "查询数据库结果为空");
			return;
		}
		
		CMSocket msocket;
		CSocketData socketdata;
		msocket.SetSocket(m_socket);

		socketdata.Create(length);
		socketdata.AddHeader(1,m_category,mServiceKey,length);
		socketdata.EnSendBuf(&length,lpdata,length);

		msocket.SetSocket(m_socket);
		msocket.SendBuf((char *)socketdata.GetSendBuf(),length);
		socketdata.Destory();
	}
	catch(...)
	{

	}
	
}

void CLogic::ReturnData(CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,unsigned char * lpdata,int length,unsigned char * m_lpRetbuf, int * m_strNum)
{
	try
	{
		if(length == 0)
		{
			ReturnMessage(m_category, mServiceKey, "查询数据库结果为空",m_lpRetbuf,m_strNum);
			return;
		}
		
		CSocketData socketdata;

		
		socketdata.Create(length);
		socketdata.AddHeader(1,m_category,mServiceKey,length);
		socketdata.EnSendBuf(&length,lpdata,length);
		
		memcpy(m_lpRetbuf,socketdata.GetSendBuf(),length);
		*m_strNum=length;
		socketdata.Destory();
	}
	catch(...)
	{
		
	}
	
}

//返回数据COperVector
void CLogic::ReturnData(SOCKET m_socket,unsigned int nid,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,COperVector opervecotr)
{
	try
	{

		int length=0;
		length=opervecotr.EnBody();
		CMSocket msocket;
		CSocketData socketdata;
		msocket.SetSocket(m_socket);
		
		socketdata.Create(length);
		socketdata.AddHeader(1,m_category,mServiceKey,length);
		socketdata.EnSendBuf(&length,opervecotr.getDes(),length);
		
		msocket.SetSocket(m_socket);
		msocket.SendBuf((char *)socketdata.GetSendBuf(),length);
		socketdata.Destory();
		
	}
	catch(...)
	{
		
	}
	
}

//返回数据COperVector
void CLogic::ReturnData(CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,COperVector opervecotr,unsigned char * m_lpRetbuf, int * m_strNum)
{
	try
	{
		
		int length=0;
		length=opervecotr.EnBody();
		CMSocket msocket;
		CSocketData socketdata;

		
		socketdata.Create(length);
		socketdata.AddHeader(1,m_category,mServiceKey,length);
		socketdata.EnSendBuf(&length,opervecotr.getDes(),length);
		

		msocket.SendBuf((char *)socketdata.GetSendBuf(),length);
		socketdata.Destory();
		
	}
	catch(...)
	{
		
	}
	
}
//邮件IP,端口,收件人,发件人,头,主题,内容
bool CLogic::SendMail(char *lpip,int Port,char *lpRecver,char *lpSender,char *lpHeader,char *lpSubject,char *lpBody)
{
	try
	{
		CMSocket m_Socket;
		char sendbuf[2048];
		char recvbuf[256];
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		int iresult=m_Socket.ConnectURL(lpip,Port);
		m_Socket.MyReceive((unsigned char *)recvbuf);
		char local_host[80];	
		gethostname( local_host, 80 );
		
		sprintf(sendbuf,"HELO %s\r\n",local_host);
		
		int sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		sprintf(sendbuf,"MAIL From: <%s>\r\n",lpSender);//邮件来自于谁
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		sprintf(sendbuf,"RCPT TO: <%s>\r\n",lpRecver);//邮件发给谁
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		// Send the DATA command
		sprintf(sendbuf,"DATA\r\n");//数据
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		sprintf(sendbuf,"%s",lpHeader);//发送头
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		
		
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		// Send the DATA command
		sprintf(sendbuf,"%s",lpBody);//数据发送身体
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		// Send the DATA command
		sprintf(sendbuf,"\r\n.\r\n");//数据结尾
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		m_Socket.MyDestory();
	}
	catch (...)
	{
		
	}
	return true;
}
