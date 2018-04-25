// Logic.cpp : ���� DLL Ӧ�ó������ڵ㡣
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

// ���ǵ���������һ��ʾ��
LOGIC_API int nLogic=0;

// ���ǵ���������һ��ʾ����
LOGIC_API int fnLogic(void)
{
	return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� Logic.h
CLogic::CLogic()
{ 
	return; 
}

CLogic::~CLogic()
{
	
}

//����logStruct�ṹ��
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

//����SOCKET��ɾ����Ӧ���û���Ϣ
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

//�����û���ID����û���
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

//����SOCKET���õ���Ӧ���û���
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

//����SOCKET���õ���Ӧ���û���ʵ����
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

//����SOCKET���õ���Ӧ���û�ID
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

//������Ϣ 
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

//������Ϣ 
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
//��������char *
void CLogic::ReturnData(SOCKET m_socket,unsigned int nid,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,unsigned char * lpdata,int length)
{
	try
	{
		if(length == 0)
		{
			ReturnMessage(m_socket,nid, m_category, mServiceKey, "��ѯ���ݿ���Ϊ��");
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
			ReturnMessage(m_category, mServiceKey, "��ѯ���ݿ���Ϊ��",m_lpRetbuf,m_strNum);
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

//��������COperVector
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

//��������COperVector
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
//�ʼ�IP,�˿�,�ռ���,������,ͷ,����,����
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
		sprintf(sendbuf,"MAIL From: <%s>\r\n",lpSender);//�ʼ�������˭
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		sprintf(sendbuf,"RCPT TO: <%s>\r\n",lpRecver);//�ʼ�����˭
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		// Send the DATA command
		sprintf(sendbuf,"DATA\r\n");//����
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		sprintf(sendbuf,"%s",lpHeader);//����ͷ
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		
		
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		// Send the DATA command
		sprintf(sendbuf,"%s",lpBody);//���ݷ�������
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		// Send the DATA command
		sprintf(sendbuf,"\r\n.\r\n");//���ݽ�β
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		m_Socket.MyDestory();
	}
	catch (...)
	{
		
	}
	return true;
}
