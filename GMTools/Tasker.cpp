#include "StdAfx.h"
#include "GMTools.h"
#include ".\tasker.h"
DWORD CTasker::run()
{
	this->Process();//启动线程
	return 0;
}
void CTasker::Process()
{
	WaitForSingleObject(m_eventThreadStop,INFINITE);//等待状态设置退出线程
}
void CTasker::Receive()
{
	try
	{
		int recvlength=0;//接收到数据的长度
		CLogFile logFile;
		CMSocket msocket;//用于接收数据
		msocket.SetSocket(this->m_socket);//设置接收数据的SOCKET
		recvlength=msocket.MyReceive((unsigned char *)&recv);//接收到数据
	
		if(recvlength>0)
		{
			Dll_Main m_dll_main;
			vector<Dll_Main>::iterator iter;
			for( iter = dll_vector.begin(); iter != dll_vector.end(); iter++ )//遍寻入口函数
			{	

				m_dll_main = *iter;
				if(m_dll_main((SOCKET)this->m_socket,(unsigned char *)&recv,recvlength))//启动入口函数（成功true,失败false)
					break;

			}
		}
		
		msocket.MyDestory();//回收接收的内存
	}
	catch (...)
	{
	//	CLogFile file;
	//	file.WriteText("EROOR","错误");//写错误信息
	}

}


