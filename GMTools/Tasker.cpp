#include "StdAfx.h"
#include "GMTools.h"
#include ".\tasker.h"
DWORD CTasker::run()
{
	this->Process();//�����߳�
	return 0;
}
void CTasker::Process()
{
	WaitForSingleObject(m_eventThreadStop,INFINITE);//�ȴ�״̬�����˳��߳�
}
void CTasker::Receive()
{
	try
	{
		int recvlength=0;//���յ����ݵĳ���
		CLogFile logFile;
		CMSocket msocket;//���ڽ�������
		msocket.SetSocket(this->m_socket);//���ý������ݵ�SOCKET
		recvlength=msocket.MyReceive((unsigned char *)&recv);//���յ�����
	
		if(recvlength>0)
		{
			Dll_Main m_dll_main;
			vector<Dll_Main>::iterator iter;
			for( iter = dll_vector.begin(); iter != dll_vector.end(); iter++ )//��Ѱ��ں���
			{	

				m_dll_main = *iter;
				if(m_dll_main((SOCKET)this->m_socket,(unsigned char *)&recv,recvlength))//������ں������ɹ�true,ʧ��false)
					break;

			}
		}
		
		msocket.MyDestory();//���ս��յ��ڴ�
	}
	catch (...)
	{
	//	CLogFile file;
	//	file.WriteText("EROOR","����");//д������Ϣ
	}

}


