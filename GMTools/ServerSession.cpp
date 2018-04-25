// ServerSession.cpp: implementation of the CServerSession class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GMTOOLS.h"
#include "ServerSession.h"
#include "ServiceManagerModule.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
UINT WorkThread(LPVOID lpParameter)
{
	try
	{
		CServerSession * pModule = (CServerSession*)lpParameter;
		SOCKET m_hSocket = pModule->GetSocket();
		Mepacket* pPacket=NULL;
//		Mepacket* pPacket= new Mepacket;
		//Mepacket pPacket;
		while(pModule!=NULL&&pModule->GetStatus())
		{
			if(!pModule->vePacket.empty())
			{
				pPacket=pModule->PupPacket();
				if(pPacket->m_iCount>0)
			//	if(pPacket.m_iCount>0)	
				{
					CSocketData socketdata;
					if(socketdata.Create(pPacket->m_iCount))
					{
						//	socketdata.Create(pPacket.m_iCount);
					if(!socketdata.DeHead(pPacket->m_pPacketBuf,pPacket->m_iCount))
				//	if(!socketdata.DeHead(pPacket.m_pPacketBuf,pPacket.m_iCount))
					{
						socketdata.Destory();
						break;
					}
					else
					{
						if(socketdata.GetMessageTagID()==CEnumCore::Message_Tag_ID::USER_ACTIVE)
						{
							//	this->m_bIsActive=TRUE;
							printf("回收:%i\n",(int)m_hSocket);
							pModule->SetStatus(1);
						}
						else
						{
							switch(socketdata.GetPacketHead().m_msg_category)
							{
								case CEnumCore::Msg_Category::COMMON://0x80
								case CEnumCore::Msg_Category::USER_ADMIN:
								case CEnumCore::Msg_Category::MODULE_ADMIN:
								case CEnumCore::Msg_Category::USER_MODULE_ADMIN://0x83
								case CEnumCore::Msg_Category::GAME_ADMIN:
								if(GM_Main!=NULL)
									GM_Main(pModule,m_hSocket,socketdata.GetPacketHead().m_msg_category,socketdata.GetMessageTagID(),socketdata.GetDataBuf()+18, (int)socketdata.GetPacketHead().bodylen);
								else
								{
									printf("没有该模块");
									pModule->Send((unsigned char *)"error",5);
								}
								break;
								case CEnumCore::Msg_Category::JW2_ADMIN:
								if(JW2_Main!=NULL)
									JW2_Main(pModule,m_hSocket,socketdata.GetPacketHead().m_msg_category,socketdata.GetMessageTagID(),socketdata.GetDataBuf()+18, (int)socketdata.GetPacketHead().bodylen);
								else
								{
									printf("没有该模块");
									pModule->Send((unsigned char *)"error",5);
								}
								break;
								case CEnumCore::Msg_Category::PAL_ADMIN:
								if(Pal_Main!=NULL)
									Pal_Main(pModule,m_hSocket,socketdata.GetPacketHead().m_msg_category,socketdata.GetMessageTagID(),socketdata.GetDataBuf()+18, (int)socketdata.GetPacketHead().bodylen);
								else
									{
										printf("没有该模块");
										pModule->Send((unsigned char *)"error",5);
									}
									break;
								case CEnumCore::Msg_Category::PALLOG_ADMIN:
									if(PalLog_Main!=NULL)
										PalLog_Main(pModule,m_hSocket,socketdata.GetPacketHead().m_msg_category,socketdata.GetMessageTagID(),socketdata.GetDataBuf()+18, (int)socketdata.GetPacketHead().bodylen);
									else
									{
										printf("没有该模块");
										pModule->Send((unsigned char *)"error",5);
									}
									break;
								case CEnumCore::Msg_Category::GT_ADMIN:
									if(GT_Main!=NULL)
										GT_Main(pModule,m_hSocket,socketdata.GetPacketHead().m_msg_category,socketdata.GetMessageTagID(),socketdata.GetDataBuf()+18, (int)socketdata.GetPacketHead().bodylen);
									else
									{
										printf("没有该模块");
										pModule->Send((unsigned char *)"error",5);
									}
									break;
								case CEnumCore::Msg_Category::GT_OP_ADMIN:
									if(GTOP_Main!=NULL)
										GTOP_Main(pModule,m_hSocket,socketdata.GetPacketHead().m_msg_category,socketdata.GetMessageTagID(),socketdata.GetDataBuf()+18, (int)socketdata.GetPacketHead().bodylen);
									else
									{
										printf("没有该模块");
										pModule->Send((unsigned char *)"error",5);
									}
									break;
								case CEnumCore::Msg_Category::CG2_ADMIN:
									if(CG2_Main!=NULL)
										CG2_Main(pModule,m_hSocket,socketdata.GetPacketHead().m_msg_category,socketdata.GetMessageTagID(),socketdata.GetDataBuf()+18, (int)socketdata.GetPacketHead().bodylen);
									else
									{
										printf("没有该模块");
										pModule->Send((unsigned char *)"error",5);
									}
									break;
								case CEnumCore::Msg_Category::SDO_ADMIN:
									if(SDO_Main!=NULL)
										SDO_Main(pModule,m_hSocket,socketdata.GetPacketHead().m_msg_category,socketdata.GetMessageTagID(),socketdata.GetDataBuf()+18, (int)socketdata.GetPacketHead().bodylen);
									else
									{
										printf("没有该模块");
										pModule->Send((unsigned char *)"error",5);
									}
									break;
								case CEnumCore::Msg_Category::WA_ADMIN:
									if(WA_Main!=NULL)
										WA_Main(pModule,m_hSocket,socketdata.GetPacketHead().m_msg_category,socketdata.GetMessageTagID(),socketdata.GetDataBuf()+18, (int)socketdata.GetPacketHead().bodylen);
									else
									{
										printf("没有该模块");
										pModule->Send((unsigned char *)"error",5);
									}
									break;
								case CEnumCore::Msg_Category::CF_ADMIN:
									if(CF_Main!=NULL)
										CF_Main(pModule,m_hSocket,socketdata.GetPacketHead().m_msg_category,socketdata.GetMessageTagID(),socketdata.GetDataBuf()+18, (int)socketdata.GetPacketHead().bodylen);
									else
									{
										printf("没有该模块");
										pModule->Send((unsigned char *)"error",5);
									}
									break;

								//add by ltx
								case CEnumCore::Msg_Category::XDJG_ADMIN:
									if(XDJG_Main!=NULL)
										XDJG_Main(pModule,m_hSocket,socketdata.GetPacketHead().m_msg_category,socketdata.GetMessageTagID(),socketdata.GetDataBuf()+18, (int)socketdata.GetPacketHead().bodylen);
									else
									{
										printf("没有该模块");
										pModule->Send((unsigned char *)"error",5);
									}
									break;
								case CEnumCore::Msg_Category::Ban_ADMIN:
									if(Ban_Main!=NULL)
										Ban_Main(pModule,m_hSocket,socketdata.GetPacketHead().m_msg_category,socketdata.GetMessageTagID(),socketdata.GetDataBuf()+18, (int)socketdata.GetPacketHead().bodylen);
									else
									{
										printf("没有该模块");
										pModule->Send((unsigned char *)"error",5);
									}
									break;
								default:
									break;
								}
							}
				
						}
						socketdata.Destory();
					}
						
				}
				if(pPacket!=NULL)
				{
					delete pPacket;
					pPacket=NULL;
				}
				
			}	
			
		/*	if(pPacket.m_pPacketBuf!=NULL)
			{
				delete[] pPacket.m_pPacketBuf;
				pPacket.m_pPacketBuf=NULL;
				pPacket.m_iCount=0;
			}*/
			Sleep(300);
		}
		if(pModule!=NULL&&!pModule->vePacket.empty())
		{
			pModule->vePacket.clear();
		}
	
	}
	catch(...)
	{

	}
/*	catch (CMemoryException* e)
	{
		
	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}*/
	return 0;
}

CServerSession::CServerSession(CServiceManagerModule * pModule)
:CSession(pModule)
{
}

CServerSession::~CServerSession()
{
	delete []m_pSendBuf;
	m_pSendBuf = NULL;
}

bool CServerSession::Create(SOCKET hSocket)
{
	bool m_result=true;
	try
	{
		m_result=CSession::Create(hSocket);
		i_status=1;
		InitWorkThread();
	}
	catch(...)
	{
	}
/*	catch (CMemoryException* e)
	{
		
	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}*/
	return m_result;
	
}
bool CServerSession::Connect(LPCTSTR strAddr, int nPort)
{
	return CSession::Connect(strAddr,nPort);
}
int CServerSession::Receive(DWORD dwFlag)
{
	try
	{
		if( NETWORK_BUF_SIZE > m_nReadCount )
		{
			memset(&m_ovRecv, 0, sizeof(OVERLAPPED2));
			m_ovRecv.dwFlag = dwFlag |PACKET_RECV ;
			
			
			if( !ReadFile((HANDLE)m_hSocket, m_msg.GetBuf(), NETWORK_BUF_SIZE,  NULL,(LPOVERLAPPED)&m_ovRecv))
			{
				int nLastError = GetLastError();
				if((nLastError != ERROR_IO_PENDING) && (nLastError != ERROR_SUCCESS))
				{
					return ERROR_RECV2;
				}
			}
			//接收数据包，插入队列
			if(m_nReadCount>0)
			{
				InsertPacket();
			}
			return ERROR_NONE;
		}
		else
		{
			printf("没有该模块");
			this->Send((unsigned char *)"error",5);
			return ERROR_RECVBUFFULL;
		}
	}
	catch(...)
	{
	}
/*	catch (CMemoryException* e)
	{
		
	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}*/
	return ERROR_RECVBUFFULL;
}
int CServerSession::Send(LPBYTE pBuf, int nBytes)
{
	try
	{
		int i=0;
		int iSend=nBytes;
		if(nBytes>0)
		{
			if(nBytes>=NETWORK_BUF_SIZE)
			{
				i=0;
				while(iSend>0)
				{
					while(i<nBytes/NETWORK_BUF_SIZE)
					{
						if(m_nBytesInSendBuf==0)
						{
							CSession::Send(pBuf+i*NETWORK_BUF_SIZE,NETWORK_BUF_SIZE);
							iSend=iSend-NETWORK_BUF_SIZE;
							i++;
						}
						else
						{
							Sleep(500);
						}
					}
					if(m_nBytesInSendBuf==0)
					{
						CSession::Send(pBuf+nBytes/NETWORK_BUF_SIZE*NETWORK_BUF_SIZE,nBytes%NETWORK_BUF_SIZE);
						iSend=iSend-NETWORK_BUF_SIZE;
					}
					else
					{
						Sleep(500);
					}
					
				}
			}
			else
			{
				CSession::Send(pBuf,nBytes);
			}
		}

	}
	catch(...)
	{
	}
/*	catch (CMemoryException* e)
	{
		
	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}*/
	return nBytes;
}
void CServerSession::CheckErrorReason(int nError)
{
	CSession::CheckErrorReason(nError);
}
void CServerSession::Close()
{
	CSession::Close();

}
int	CServerSession::ParseNetworkMessage()
{
	return -1;
}

bool CServerSession::SendActive()
{
	try
	{
		i_status=2;
		COperVector SendVectBuf;
		SendVectBuf.InitCreate(1,1);
		
		//添加TFLV到数据包中
		SendVectBuf.AddTFLV(CEnumCore::TagName::MESSAGE,CEnumCore::TagFormat::TLV_STRING,strlen("Is Acitve"),(unsigned char*)"Is Acitve");
		
		//解析数据包并获取长度
		int vectLength = SendVectBuf.EnBody();
		int sBodyLength = 0;
		CSocketData SendDataBuf;
		SendDataBuf.Create(vectLength);//构造发送数据包
		SendDataBuf.AddHeader(1, CEnumCore::Msg_Category::COMMON, CEnumCore::ServiceKey::USER_ACTIVE_RESP, vectLength);//添加数据包的头部
		SendDataBuf.EnSendBuf(&sBodyLength, SendVectBuf.getDes(), vectLength);//加密并构造发送数据包
		
		this->Send(SendDataBuf.GetSendBuf(),sBodyLength);
		SendDataBuf.Destory();
		SendVectBuf.Destroy();
		printf("发送 %i",(int)m_hSocket);
	}
	catch(...)
	{
	}
/*	catch (CMemoryException* e)
	{
		
	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}*/
	return TRUE;
}

void CServerSession::InsertPacket()
{
	//创建数据包，添加到队列中
	try
	{
		m_insertQue.Lock();
		Mepacket* pMepacket=new Mepacket();
		memcpy(pMepacket->m_pPacketBuf, m_msg.GetBuf(), m_nReadCount);
		pMepacket->m_iCount = m_nReadCount;
		vePacket.push_back(pMepacket);
	/*	Mepacket pMepacket;
		memcpy(pMepacket.m_pPacketBuf, m_msg.GetBuf(), m_nReadCount);
		pMepacket.m_iCount=m_nReadCount;
		vePacket.push_back(pMepacket);*/
		m_insertQue.Unlock();
	}
	catch(...)
	{
	}
/*	catch (CMemoryException* e)
	{
		
	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}*/


}
/*Mepacket CServerSession::PupPacket()
{
	Mepacket m_Mepacket;
	try
	{
		m_insertQue.Lock();
		m_Mepacket = vePacket.back();
		vePacket.pop_back();	
		m_insertQue.Unlock();
	}
	catch (CMemoryException* e)
	{
		
	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}
	return m_Mepacket;
}*/
Mepacket* CServerSession::PupPacket()
{
	Mepacket* m_Mepacket=NULL;
	try
	{
		m_insertQue.Lock();
		m_Mepacket = vePacket.back();
		vePacket.pop_back();	
		m_insertQue.Unlock();
	}
	catch(...)
	{
	}
/*	catch (CMemoryException* e)
	{
		
	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}*/
	return m_Mepacket;
}

bool CServerSession::InitWorkThread()
{
	//Modified by tengjie 2010-06-13:For stack overflow
	//if( AfxBeginThread(WorkThread, this) == NULL)
	if( AfxBeginThread(WorkThread, this, THREAD_PRIORITY_NORMAL, NETWORK_BUF_SIZE*100) == NULL)
		return false;

	return true;
}
