#include "StdAfx.h"
#include "GTOper.h"
#include "AUDes.h"
#include "time.h"

CGTOper::CGTOper(void)
{
}

CGTOper::~CGTOper(void)
{
	if(msocket.m_socket!=INVALID_SOCKET)
	{
		msocket.closeSocket();
	}
}
//����Զ�̷����
bool CGTOper::CONNECT_SERVERMANAGER_LOGIN(char * ServerIP,char * retMessage)
{
	try
	{
		char GT_GateWay[40];
		ZeroMemory(GT_GateWay,40);
		int ListenPort=0;//��ȡԶ�̶˿�

		if(!operGT.GetServerIP("",ServerIP,2,GT_GateWay,&ListenPort))
		{
			sprintf(retMessage,"%s", "���ݿ���û���������");
			return false;
		}

	
		int ID=0;
		for(ID=1010;ID<1020;ID++)
		{
			if(msocket.ConnectURL(GT_GateWay, ListenPort) == 0)
			{
				sprintf(retMessage,"%s", "�޷�����Զ�̽ӿ�");
				msocket.closeSocket();
				return false;
			}

			CPacket req(ServiceKey::GMProtocol::CONNECT_GATEWAY_LOGIN);
			req<<(WORD)12<<(int)ID<<(WORD)9100;
			int count=0;
			if(!msocket.SendNewData(req.GetPack(),req.GetPackLen()))
			{
				sprintf(retMessage,"%s", "�޷�����Զ�̽ӿ�!");	
				msocket.closeSocket();
				return false;
			}
			CPacket packet;
			int recvLen =-1;
			recvLen	= msocket.recvData();
			if(recvLen<=0)
			{
				sprintf(retMessage,"%s", "��½ʧ��,������!");	
				printf("��½ʧ��ID:%i,������!\n",ID);
				msocket.closeSocket();
			}
			else
			{
				memcpy(packet.GetPack(), msocket.recvBuffer(), recvLen);
				printf("��½�ɹ�ID:%i!\n",ID);
				return TRUE;
			}
			

		}
		
	
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("GT","����Զ�̽ӿ��ڴ治��");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("GT","����Զ�̽ӿ��ļ��쳣");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("GT","����Զ�̽ӿ������쳣");
		return false;
	}
	return true;
}

//��ѯ����
vector<CGlobalStruct::TFLV> CGTOper::GT_BROADCAST_SELECT(char * ServerIP,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBVect;
	try
	{
		char GT_StartTime[256];
		char GT_EndTime[256];
		CGlobalStruct::TFLV m_tflv;
		int ListenPort;

		if(operGT.GetServerIP("",ServerIP,2,ServerIP,&ListenPort))
		{
			if(msocket.ConnectURL(ServerIP, ListenPort) != 0)
			{
				CPacket req(ServiceKey::GMProtocol::CONNECT_GATEWAY_LOGIN);
				req<<(WORD)12<<(int)1010<<(WORD)9100;
				if(msocket.SendNewData(req.GetPack(),req.GetPackLen()))
				{
					CPacket packet1;
					int recvLen = msocket.recvData();
					memcpy(packet1.GetPack(), msocket.recvBuffer(), recvLen);
					
					
					CPacket req(ServiceKey::GMProtocol::GM_BROADCAST_SELECT);
					msocket.SendNewData(req.GetPack(),req.GetPackLen());//���Ͳ�ѯ������Ϣ������
					
					CPacket packet(ServiceKey::GMProtocol::GM_BROADCAST_SELECT_ACK);
					recvLen = msocket.recvData();//��ȡ��ѯ������Ϣ�Ļ�Ӧ
					memcpy(packet.GetPack(),msocket.recvBuffer(),recvLen);
					
					
					
					int count = 0;//��������	
					int ID = 0;//����ID
					wstring Content;//��������
					time_t tmpSTime = 0;//��ʼʱ��
					time_t tmpETime = 0;//����ʱ��
					DWORD interval = 0;//������
					int pageCount = 0;//����ҳ��
					char strInt[10];//����int���͵ı���
					ZeroMemory(strInt, 10);
					packet>>count;
					if(count > 0)
					{
						pageCount = (count - 1)/iPageSize + 1;
						for(int i=0; i<count ; i++)
						{
							packet>>ID;
							packet>>Content;
							packet>>tmpSTime;
							packet>>tmpETime;
							packet>>interval;
							if((i >= (iIndex-1)*iPageSize) && (i < iIndex*iPageSize))//��ҳ��ѯ
							{					
								_itoa(ID, strInt, 10);
								m_tflv.nIndex=DBVect.size()+1;
								m_tflv.m_tagName=CEnumCore::TagName::GT_NoticeID;
								m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
								m_tflv.m_tvlength=sizeof(int);
								sprintf((LPSTR)&m_tflv.lpdata,"%s",(unsigned char*)&strInt);
								DBVect.push_back(m_tflv);
								
								m_tflv.nIndex=DBVect.size()+1;
								m_tflv.m_tagName=CEnumCore::TagName::GT_BoardMessage;
								m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength=Content.length()*2;
								sprintf((LPSTR)&m_tflv.lpdata,"%s",operGT.wcharToChar(Content));
								DBVect.push_back(m_tflv);
								
								struct tm* m_StartTime;
								m_StartTime = gmtime(&tmpSTime);
								sprintf(GT_StartTime,"%i-%i-%i %i:%i:%i", m_StartTime->tm_year+1900, m_StartTime->tm_mon+1, m_StartTime->tm_mday,m_StartTime->tm_hour,m_StartTime->tm_min,m_StartTime->tm_sec);
								m_tflv.nIndex=DBVect.size()+1;
								m_tflv.m_tagName=CEnumCore::TagName::GT_StartTime;
								m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength=strlen(GT_StartTime);
								sprintf((LPSTR)&m_tflv.lpdata,"%s",GT_StartTime);
								DBVect.push_back(m_tflv);
								
								struct tm* m_EndTime;
								m_EndTime = gmtime(&tmpETime);
								sprintf(GT_EndTime,"%i-%i-%i %i:%i:%i", m_EndTime->tm_year+1900, m_EndTime->tm_mon+1, m_EndTime->tm_mday,m_EndTime->tm_hour,m_EndTime->tm_min,m_EndTime->tm_sec);
								m_tflv.nIndex=DBVect.size()+1;
								m_tflv.m_tagName=CEnumCore::TagName::GT_EndTime;
								m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength=strlen(GT_EndTime);
								sprintf((LPSTR)&m_tflv.lpdata,"%s",GT_EndTime);
								DBVect.push_back(m_tflv);
								
								_itoa(interval, strInt, 10);
								m_tflv.nIndex=DBVect.size()+1;
								m_tflv.m_tagName=CEnumCore::TagName::GT_Interval;
								m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
								m_tflv.m_tvlength=sizeof(int);
								sprintf((LPSTR)&m_tflv.lpdata,"%s",(unsigned char*)&strInt);
								DBVect.push_back(m_tflv);
								
								_itoa(pageCount, strInt, 10);
								m_tflv.nIndex=DBVect.size()+1;
								m_tflv.m_tagName=CEnumCore::TagName::PageCount;
								m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
								m_tflv.m_tvlength=sizeof(int);
								sprintf((LPSTR)&m_tflv.lpdata,"%s",(unsigned char*)&strInt);
								DBVect.push_back(m_tflv);
							}
						}
					}
					else
					{
						m_tflv.nIndex=DBVect.size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen("û�й���");
						sprintf((LPSTR)&m_tflv.lpdata,"%s","û�й���");
						DBVect.push_back(m_tflv);
					}
				}
				
			}
			else
			{
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen("Զ�̷��������ʧ��");
				sprintf((LPSTR)&m_tflv.lpdata,"%s","Զ�̷��������ʧ��");
				DBVect.push_back(m_tflv);
			}
			
		}
		else
		{
			
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen("����SqL����");
			sprintf((LPSTR)&m_tflv.lpdata,"%s","����SqL����");
			DBVect.push_back(m_tflv);

		}
		msocket.closeSocket();
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("GT","��ѯ�����ڴ治��");
		return DBVect;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("GT","��ѯ�����ļ��쳣");
		return DBVect;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("GT","��ѯ���������쳣");
		return DBVect;
	}
	return DBVect;

}

//��ӹ���
BOOL CGTOper::GT_BROADCAST_ADD(int IsImmediate,wstring context,DWORD interval,size_t StartTime,size_t EndTime)
{
	bool result = true;
	try
	{
		CPacket req(ServiceKey::GMProtocol::GM_BROADCAST_ADD);
		if(IsImmediate == 0)//���Ǽ�ʱ����
		{
			req<<false<<context<<StartTime<<EndTime<<interval;
			msocket.SendNewData(req.GetPack(),req.GetPackLen());//������ӹ��������

			CPacket packet;
			int recvLen = msocket.recvData();//��ȡ��ӹ���Ļ�Ӧ
			memcpy(packet.GetPack(), msocket.recvBuffer(), recvLen);
			//packet>>result;//�õ���ӹ���Ľ��
		}
		else
		{
			req<<true<<context;
			msocket.SendNewData(req.GetPack(),req.GetPackLen());//������ӹ��������

			CPacket packet;
			int recvLen = msocket.recvData();//��ȡ��ӹ���Ļ�Ӧ
			memcpy(packet.GetPack(), msocket.recvBuffer(), recvLen);			
	    //	packet>>result;//�õ���ӹ���Ľ��
		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("GT","��ӹ����ڴ治��");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("GT","��ӹ����ļ��쳣");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("GT","��ӹ��������쳣");
		return false;
	}
	return result;
}


//ɾ������
BOOL CGTOper::GT_BROADCAST_REMOVE(int NoticeID)
{
	bool result = true;
	try
	{
		CPacket req(ServiceKey::GMProtocol::GM_BROADCAST_REMOVE);
		req<<NoticeID;		
		msocket.SendNewData(req.GetPack(),req.GetPackLen());//����Ҫɾ�����������
		
	/*	CPacket packet(ServiceKey::GMProtocol::GM_BROADCAST_REMOVE_ACK);
		recvLen = msocket.recvData();//��ȡɾ������Ļ�Ӧ
		memcpy(packet.GetPack(), msocket.recvBuffer(), recvLen);
		packet>>result;//�õ�ɾ������Ľ��*/
		msocket.closeSocket();
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("GT","ɾ�������ڴ治��");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("GT","ɾ�������ļ��쳣");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("GT","ɾ�����������쳣");
		return false;
	}
	return result;
}

int CGTOper::GT_KICK_USER(char * ServerIP,char * strAccount)
{
	CLogFile logFile;
	try
	{
		CMSocket msocket;
		
		char path[256];
		GetCurrentDirectory(256,path);
		char filepath[256];
		sprintf(filepath,"%s%s",path,"\\config\\GTOper.ini");
		
		GetPrivateProfileString("SERVER","Address","127.0.0.1",ServerIP,20,filepath);//��ȡԶ�̽ӿڵ�ַ
		int ListenPort=GetPrivateProfileInt("SERVER","Port",0,filepath);//��ȡԶ�̶˿�
		
		if(msocket.ConnectURL(ServerIP, ListenPort) == 0)
		{
			return 0;
		}
		
		CPacket req(ServiceKey::GMProtocol::CONNECT_GATEWAY_LOGIN);
		req<<(WORD)12<<(int)1010<<(WORD)9100;
		int count=0;
		if(!msocket.SendNewData(req.GetPack(),req.GetPackLen()))
		{
			return false;
		}
		
		CPacket packet;
		int recvLen = msocket.recvData();
		memcpy(packet.GetPack(), msocket.recvBuffer(), recvLen);
		
		CPacket req1(ServiceKey::GMProtocol::GM_KICK_USER);
		req1<<strAccount;
		
		
		msocket.SendNewData(req1.GetPack(),req1.GetPackLen());
		msocket.closeSocket();
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("GT","ǿ����������ڴ治��");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("GT","ǿ����������ļ��쳣");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("GT","ǿ��������������쳣");
		return false;
	}
	
	return 1;
}
//ǿ��������ߣ���ѯͨ�����ݱ�����ֻ���˺����ˣ�
BOOL CGTOper::GT_KICK_USER(char * ServerIP,char * szAccount,char * retMessage)
{
	BOOL b_result=FALSE;
	try
	{
		wstring Account=operGT.charToWChar(szAccount);
		if(CONNECT_SERVERMANAGER_LOGIN(ServerIP,retMessage))
		{
			CPacket req(ServiceKey::GMProtocol::GM_KICK_USER);
			req<<Account;
			if(!msocket.SendNewData(req.GetPack(),req.GetPackLen()))//����ǿ��������ߵ�����
			{
				sprintf(retMessage,"%s","��Ҳ�����");
				return FALSE;
			}
			CPacket packet;
			int recvLen = msocket.recvData();//�õ�ǿ��������ߵĻ�Ӧ
			memcpy(packet.GetPack(), msocket.recvBuffer(), recvLen);
			packet>>b_result;
			msocket.closeSocket();
		}		
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("GT","ǿ����������ڴ治��");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("GT","ǿ����������ļ��쳣");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("GT","ǿ��������������쳣");
		return false;
	}
	return b_result;


}

//��ͣ�ͽ��
BOOL CGTOper::GT_BanUser(int userByID, char * ServerName, char * ServerIP,char * UserName,int UserID,char* userState,char * Reason)
{
	try
	{
		int curState = 0;//�����ʺŵ�ǰ��û�б���ͣ
		char retMessage[256];
		char operRemoteSql[256];
		operGT.getRemoteSql("GT","GT_GetUserState_Local",operRemoteSql,4);//��ȡԶ�̲�����Sql���
		if(!strcmp("",operRemoteSql))
		{
			operGT.BuildTLV("���ݿ����Ҳ�����Ӧ��sql���");
			operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
			return false;
		}

		vector <CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GT","127.0.0.1",4,0,0,operRemoteSql, UserID, ServerName);
		if(!DBTFLV.empty())//��ѯ����û�н��
		{
			curState = StrToInt((char*)((COperVector(DBTFLV)).getTLVByTag(CEnumCore::TagName::GT_UserState, 0)).lpdata);//curState = 1,�ʺ��ѱ���ͣ
		}
		


		BYTE uState = 0;
		if(!strcmp("1",userState))//Ҫ��ͣ����ʺ�
		{
			uState = 1;
			if(curState == 1)//����˺��ѱ���ͣ
			{
				operGT.BuildTLV("�ʺ��ѱ���ͣ�������ٴη�ͣ");
				operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
				return false;
			}

			GT_KICK_USER(ServerIP,UserName,retMessage);
		}
		else
		{
			uState = 0;
			if(curState == 0)//����˺�δ����ͣ
			{
				operGT.BuildTLV("�ʺ�δ����ͣ���޷����");
				operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
				return false;
			}
		}
		wstring Account=operGT.charToWChar(UserName);
		if(CONNECT_SERVERMANAGER_LOGIN(ServerIP,retMessage))
		{
			CPacket req(ServiceKey::GMProtocol::GM_USER_STOP);
			req<<Account<<uState;
			msocket.SendNewData(req.GetPack(),req.GetPackLen());//���ͷ�ͣ��������ʺŵ�����
			
			CPacket packet;
			int recvLen = msocket.recvData();//��ȡ��ͣ��������ʺŵ�����
			memcpy(packet.GetPack(), msocket.recvBuffer(), recvLen);
			bool result = true;
			packet>>result;//��ȡʧ�ܻ�ɹ�����Ϣ
			
			if(!result)
			{
				if(uState == 1)
				{
					sprintf(strLog,"<%s>,��ͣ������<%s> ���<%s>ʧ��",operName,ServerName,UserName);
					logFile.WriteText("GTown",strLog);//��ʧ����Ϣд����־��
					logFile.WriteDBLog(userByID,"����Online","GT_BanPlayer",ServerIP,strLog);
					operGT.BuildTLV("��ͣ�ʺ�ʧ��");
					operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
					
					return false;
				}
				else
				{
					sprintf(strLog,"<%s>,��������<%s> ���<%s>ʧ��",operName,ServerName,UserName);
					logFile.WriteText("GTown",strLog);//��ʧ����Ϣд����־��
					logFile.WriteDBLog(userByID,"����Online","GT_UnbindPlayer",ServerIP,strLog);
					operGT.BuildTLV("����ʺ�ʧ��");
					operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
					return false;
				}
			}
			else
			{
				if(uState == 1)
				{
					sprintf(strLog,"<%s>,��ͣ������<%s> ���<%s>�ɹ�",operName,ServerName,UserName);
					logFile.WriteText("GTown",strLog);//���ɹ���Ϣд����־��
					logFile.WriteDBLog(userByID,"����Online","GT_BanPlayer",ServerIP,strLog);


					//��ȡԶ�̲�����Sql���
					operGT.getRemoteSql("GT","GT_BanPlayer",operRemoteSql,4);
					if(!strcmp("",operRemoteSql))
					{
						operGT.BuildTLV("���ݿ����Ҳ�����Ӧ��sql���");
						operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
						return false;
					}

					int tmpLength =GameDBOper("GT","127.0.0.1",4,operRemoteSql, UserID, UserName, 1, Reason, ServerName); //�����ͣ�ʺ���Ϣ
					if(tmpLength == 0)
					{
						operGT.BuildTLV("��ͣ�˺Ų��뱾�����ݿ����ݿ�ʧ��");
						operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
						return false;
					}
					else
					{
						operGT.BuildTLV("��ͣ�˺ųɹ�");
						operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
						return false;
					}
			}
			else
			{
				sprintf(strLog,"<%s>,��������<%s> ���<%s>�ɹ�",operName,ServerName,UserName);
				logFile.WriteText("GTown",strLog);//���ɹ���Ϣд����־��
				logFile.WriteDBLog(userByID,"����Online","GT_UnbindPlayer",ServerIP,strLog);

				//��ȡԶ�̲�����Sql���
				operGT.getRemoteSql("GT","GT_UnbindPlayer",operRemoteSql,4);
				if(!strcmp("",operRemoteSql))
				{
					operGT.BuildTLV("���ݿ����Ҳ�����Ӧ��sql���");
					operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
					return false;
				}

				
				int tmpLength =GameDBOper("GT","127.0.0.1",4,operRemoteSql,UserID); //�������ʺ���Ϣ
				if(tmpLength == 0)
				{
					operGT.BuildTLV("����˺��޸ı������ݿ����ݿ�ʧ��");
					operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
					return false;
				}
				else
				{
					operGT.BuildTLV("����˺ųɹ�");
					operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
					return false;
				}
			}
			operGT.BuildTLV("�ɹ�");
			operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
			}
		}
		else
		{
			operGT.BuildTLV(strLog);
			operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("GT","��ͣ�˺��ڴ治��");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("GT","��ͣ�˺��ļ��쳣");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("GT","��ͣ�˺������쳣");
		return false;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ͣ������<%s> ���<%s>,����ϵͳ����",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);
		logFile.WriteDBLog(userByID,"����Online","GT_BanPlayer",ServerIP,strLog);
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
		return false;
	}
	return true;
}

bool CGTOper::GT_BanUser(char * ServerIP,char * strAccount,int status)//1.��ͣ,0.���
{
	bool result = false;
	try
	{
		char retMessage[256];
		wstring Account=operGT.charToWChar(strAccount);
		if(CONNECT_SERVERMANAGER_LOGIN(ServerIP,retMessage))
		{
			CPacket req(ServiceKey::GMProtocol::GM_USER_STOP);
			req<<Account<<status;
			msocket.SendNewData(req.GetPack(),req.GetPackLen());//���ͷ�ͣ��������ʺŵ�����
			
			CPacket packet;
			recvLen = msocket.recvData();//��ȡ��ͣ��������ʺŵ�����
			memcpy(packet.GetPack(), msocket.recvBuffer(), recvLen);
			packet>>result;//��ȡʧ�ܻ�ɹ�����Ϣ
			return TRUE;

		}

		
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("GT","Զ�̽���˺��ڴ治��");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("GT","Զ�̽���˺��ļ��쳣");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("GT","Զ�̽���˺������쳣");
		return false;
	}
	return true;

}

//��ɫ��������
void CGTOper::GT_MOVE_USER(int userByID, char * ServerName, char * ServerIP,char * UserName)
{
	try
	{
		wstring Account =operGT.charToWChar(UserName);
		if(CONNECT_SERVERMANAGER_LOGIN(ServerIP,strLog))
		{
			CPacket req(ServiceKey::GMProtocol::GM_MOVE_USER);
			req<<Account;
			msocket.SendNewData(req.GetPack(),req.GetPackLen());//���ͽ���ɫ�Ƶ����ݵ�����
			bool result = true;
			if(!result)
			{
				sprintf(strLog,"<%s>,��������<%s>�е����<%s>�Ƶ�����ʧ��",operName,ServerIP,UserName);
				logFile.WriteText("GTown",strLog);//��ʧ����Ϣд����־��
				logFile.WriteDBLog(userByID,"����Online","GT_MOVE_USER",ServerIP,strLog);
			}
			else
			{
				sprintf(strLog,"<%s>,��������<%s>�е����<%s>�Ƶ����ݳɹ�",operName,ServerIP,UserName);
				logFile.WriteText("GTown",strLog);//���ɹ���Ϣд����־��
				logFile.WriteDBLog(userByID,"����Online","GT_MOVE_USER",ServerIP,strLog);
			}
		}
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_MOVE_USER_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
		
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��������<%s>�е����<%s>�Ƶ����ݷ���ϵͳ����",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);
		logFile.WriteDBLog(userByID,"����Online","GT_MOVE_USER",ServerIP,strLog);
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_MOVE_USER_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
	}
}
//������ҵ�����״̬
void CGTOper::GT_USER_RESET(int userByID, char * ServerName, char * ServerIP,char * UserName)
{
	try
	{
		wstring Account =operGT.charToWChar(UserName);
		if(CONNECT_SERVERMANAGER_LOGIN(ServerIP,strLog))
		{
			CPacket req(ServiceKey::GMProtocol::GM_USER_RESET);
			req<<Account;
			msocket.SendNewData(req.GetPack(),req.GetPackLen());//����������ҵ�����״̬������
			
			CPacket packet;
			int recvLen = msocket.recvData();//��ȡ������ҵ�����״̬�Ļ�Ӧ
			memcpy(packet.GetPack(),msocket.recvBuffer(),recvLen);
			bool result = true;
			packet>>Account;
			packet>>result;
			if(!result)
			{
				sprintf(strLog,"<%s>,���������<%s>�е����<%s>������״̬ʧ��",operName,ServerName,UserName);
				logFile.WriteText("GTown",strLog);//��ʧ����Ϣд����־��
				logFile.WriteDBLog(userByID,"����Online","GT_USER_RESET",ServerIP,strLog);
			}
			else
			{
				sprintf(strLog,"<%s>,���������<%s>�е����<%s>������״̬�ɹ�",operName,ServerName,UserName);
				logFile.WriteText("GTown",strLog);//���ɹ���Ϣд����־��
				logFile.WriteDBLog(userByID,"����Online","GT_USER_RESET",ServerIP,strLog);
			}
		}
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_RESET_ACK,operGT.GetSendBuf(),operGT.GetSendLength());


		
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,���������<%s>�е����<%s>������״̬����ϵͳ����",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);
		logFile.WriteDBLog(userByID,"����Online","GT_USER_RESET",ServerIP,strLog);
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_RESET_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
	}
}


//˲���ƶ�
void CGTOper::GT_Transmit_Query(int userByID, char * ServerName, char * ServerIP,char * UserName,int x,int y,int z)
{
	try
	{
		wstring Account =operGT.charToWChar(UserName);
		if(CONNECT_SERVERMANAGER_LOGIN(ServerIP,strLog))
		{
			CPacket req(ServiceKey::GMProtocol::GM_TRANSMIT_REQ);
			req<<Account<<(float)x<<(float)y<<(float)z;
			msocket.SendNewData(req.GetPack(),req.GetPackLen());//����˲���ƶ�������

			bool result = true;
			if(!result)
			{
				sprintf(strLog,"<%s>,�ƶ�������<%s>�е����<%s>��x:%i,y:%i,z:%iʧ��",operName,ServerName,UserName,x,y,z);
				logFile.WriteText("GTown",strLog);//��ʧ����Ϣд����־��
				logFile.WriteDBLog(userByID,"����Online","GT_USER_RESET",ServerIP,strLog);
			}
			else
			{
				sprintf(strLog,"<%s>,�ƶ�������<%s>�е����<%s>��x:%i,y:%i,z:%i�ɹ�",operName,ServerName,UserName,x,y,z);
				logFile.WriteText("GTown",strLog);//���ɹ���Ϣд����־��
				logFile.WriteDBLog(userByID,"����Online","GT_USER_RESET",ServerIP,strLog);
			}
		}
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Transmit_Query_Resp,operGT.GetSendBuf(),operGT.GetSendLength());

		
		

		
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,�ƶ�������<%s>�е����<%s>����ϵͳ����",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);
		logFile.WriteDBLog(userByID,"����Online","GT_USER_RESET",ServerIP,strLog);
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Transmit_Query_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
	}
}
//��ֹ���������ϴ�ͼƬ
bool CGTOper::GT_Picture_Stop(int userByID, char * ServerName, char * ServerIP,char * UserName,int UserID,char* userState,char * Reason)
{
	try
	{
		int curState=-1;
		vector <CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GT","127.0.0.1",4,0,0,"exec GT_GetStopState %i,'%s'", UserID, ServerName);
		if(!DBTFLV.empty())//��ѯ����û�н��
		{
			curState = StrToInt((char*)((COperVector(DBTFLV)).getTLVByTag(CEnumCore::TagName::GT_UserState, 0)).lpdata);//curState = 1,�ʺ��ѱ���ͣ
		}
		
		BYTE uState = 0;
		if(!strcmp("0",userState))//Ҫ��ֹͼƬ�ʺ�
		{
			uState = 0;
			if(curState == 0)//����˺��ѱ���ֹͼƬ
			{
				operGT.BuildTLV("�ʺ��ѱ���ֹͼƬ�������ٴν�ֹͼƬ");
				operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Picture_Stop_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
				return false;
			}
		}
		else
		{
			uState = 1;
			if(curState == 1)//����˺�δ����ֹ
			{
				operGT.BuildTLV("�ʺ�δ����ֹ���޷����");
				operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Picture_Stop_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
				return false;
			}
		}
		wstring Account =operGT.charToWChar(UserName);
		if(CONNECT_SERVERMANAGER_LOGIN(ServerIP,strLog))
		{
			CPacket req(ServiceKey::GMProtocol::GM_PICTURE_STOP);
			req<<Account<<uState;
			msocket.SendNewData(req.GetPack(),req.GetPackLen());//���ͷ�ͣ��������ʺŵ�����
			
			CPacket packet;
			int recvLen = msocket.recvData();//��ȡ��ͣ��������ʺŵ�����
			memcpy(packet.GetPack(), msocket.recvBuffer(), recvLen);
			bool result = true;
			packet>>result;//��ȡʧ�ܻ�ɹ�����Ϣ
			if(!result)
			{
				if(uState == 0)
				{
					sprintf(strLog,"<%s>,��ͣ������<%s> ���<%s>ʧ��",operName,ServerName,UserName);
					logFile.WriteText("GTown",strLog);//��ʧ����Ϣд����־��
					logFile.WriteDBLog(userByID,"����Online","GT_BanPlayer",ServerIP,strLog);
					operGT.BuildTLV(strLog);
					operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Picture_Stop_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
					return false;
				}
				else
				{
					sprintf(strLog,"<%s>,��������<%s> ���<%s>ʧ��",operName,ServerName,UserName);
					logFile.WriteText("GTown",strLog);//��ʧ����Ϣд����־��
					logFile.WriteDBLog(userByID,"����Online","GT_UnbindPlayer",ServerIP,strLog);
					operGT.BuildTLV(strLog);
					operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Picture_Stop_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
					return false;
				}	
			}
			
			else
			{
				if(uState == 0)
				{
					sprintf(strLog,"<%s>,��ֹ������<%s> ���<%s>ͼƬ�ɹ�",operName,ServerName,UserName);
					logFile.WriteText("GTown",strLog);//���ɹ���Ϣд����־��
					
					int tmpLength=GameDBOper("GT","127.0.0.1",4,"exec GT_StopUser %i,%i,'%s','%s','%s'",userByID,UserID,UserName,ServerName,Reason);
					if(tmpLength == 0)
					{
						operGT.BuildTLV("��ֹͼƬ���뱾�����ݿ����ݿ�ʧ��");
						operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Picture_Stop_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
						return false;
					}
					else
					{
						operGT.BuildTLV("��ֹͼƬ�ɹ�");
						operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Picture_Stop_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
						return false;
					}
				}
				else
				{
					sprintf(strLog,"<%s>,���������<%s> ���<%s>ͼƬ�ɹ�",operName,ServerName,UserName);
					logFile.WriteText("GTown",strLog);//���ɹ���Ϣд����־��
					int tmpLength=GameDBOper("GT","127.0.0.1",4,"exec GT_UnStopUser %i,%i,'%s','%s','%s'",userByID,UserID,UserName,ServerName,Reason);
					if(tmpLength == 0)
					{
						operGT.BuildTLV("����˺��޸ı������ݿ����ݿ�ʧ��");
						operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Picture_Stop_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
						return false;
					}
					else
					{
						operGT.BuildTLV("����˺ųɹ�");
						operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Picture_Stop_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
						return false;
					}
				}
				operGT.BuildTLV("�ɹ�");
				operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Picture_Stop_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
			}
		}
		else
		{
			operGT.BuildTLV(strLog);
			operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Picture_Stop_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
		}
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ͣ������<%s> ���<%s>,����ϵͳ����",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);
		logFile.WriteDBLog(userByID,"����Online","GT_BanPlayer",ServerIP,strLog);
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Picture_Stop_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
	}
	return false;
}

//����ͷ��ͼƬ
void CGTOper::GT_Picture_Clear(int userByID, char * ServerName, char * ServerIP,char * UserName)
{
	try
	{
		wstring Account =operGT.charToWChar(UserName);
		if(CONNECT_SERVERMANAGER_LOGIN(ServerIP,strLog))
		{
			CPacket req(ServiceKey::GMProtocol::GM_CONTENT_CLEAR);
			req<<Account<<(BYTE)1;
			msocket.SendNewData(req.GetPack(),req.GetPackLen());//��������Υ��ͼƬ������
			
			CPacket packet;
			int recvLen = msocket.recvData();//��ȡ����Υ��ͼƬ�Ļ�Ӧ
			memcpy(packet.GetPack(),msocket.recvBuffer(),recvLen);
			bool result = true;
			BYTE B_Result=0;
			packet>>Account;
			packet>>B_Result;
			packet>>result;
			
			
			if(!result)
			{
				sprintf(strLog,"<%s>,���������<%s>�е����<%s>Υ��ͼƬʧ��",operName,ServerName,UserName);
				logFile.WriteText("GTown",strLog);//��ʧ����Ϣд����־��
				logFile.WriteDBLog(userByID,"����Online","GT_USER_RESET",ServerIP,strLog);
			}
			else
			{
				sprintf(strLog,"<%s>,���������<%s>�е����<%s>Υ��ͼƬ�ɹ�",operName,ServerName,UserName);
				logFile.WriteText("GTown",strLog);//���ɹ���Ϣд����־��
				logFile.WriteDBLog(userByID,"����Online","GT_USER_RESET",ServerIP,strLog);
			}
		}
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Picture_Clear_Resp,operGT.GetSendBuf(),operGT.GetSendLength());

		
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,���������<%s>�е����<%s>Υ��ͼƬ����ϵͳ����",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);
		logFile.WriteDBLog(userByID,"����Online","GT_Picture_Clear",ServerName,strLog);
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Picture_Clear_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
	}
}

//��ո���ǩ��
void CGTOper::GT_ClearName_Query(int userByID, char * ServerName, char * ServerIP,char * UserName)
{
	try
	{

		wstring Account =operGT.charToWChar(UserName);
		if(CONNECT_SERVERMANAGER_LOGIN(ServerIP,strLog))
		{
			CPacket req(ServiceKey::GMProtocol::GM_CONTENT_CLEAR);
			req<<Account<<(BYTE)2;
			msocket.SendNewData(req.GetPack(),req.GetPackLen());//��������Υ��ͼƬ������
			
			CPacket packet;
			int recvLen = msocket.recvData();//��ȡ����Υ��ͼƬ�Ļ�Ӧ
			memcpy(packet.GetPack(),msocket.recvBuffer(),recvLen);
			bool result = true;
			BYTE B_Result=0;
			packet>>Account;
			packet>>B_Result;
			packet>>result;
			
			if(!result)
			{
				sprintf(strLog,"<%s>,���������<%s>�е����<%s>Υ��ͼƬʧ��",operName,ServerName,UserName);
				logFile.WriteText("GTown",strLog);//��ʧ����Ϣд����־��
				logFile.WriteDBLog(userByID,"����Online","GT_ClearName_Query",ServerIP,strLog);
				operGT.BuildTLV(strLog);
				operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_ClearName_Query_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
			}
			else
			{
				sprintf(strLog,"<%s>,���������<%s>�е����<%s>Υ��ͼƬ�ɹ�",operName,ServerName,UserName);
				logFile.WriteText("GTown",strLog);//���ɹ���Ϣд����־��
				logFile.WriteDBLog(userByID,"����Online","GT_ClearName_Query",ServerIP,strLog);
				operGT.BuildTLV(strLog);
				operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_ClearName_Query_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
			}
		}
		
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,���������<%s>�е����<%s>Υ��ͼƬ����ϵͳ����",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);
		logFile.WriteDBLog(userByID,"����Online","GT_ClearName_Query",ServerIP,strLog);
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_ClearName_Query_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
	}
}

//��ѯ���ߡ�����
void CGTOper::GM_ITEM_Check()
{
/*	try
	{
		//��ȡģ����ѯ�ĵ�������
		wstring tmpName = operGT.charToWChar((char*)(recvVect.getTLVByTag(CEnumCore::TagName::GT_ItemName,0)).lpdata);

		CPacket req(ServiceKey::GMProtocol::GM_ITEM_SELECT);
		req<<tmpName;//�����ѯ������Ϣ�����ݰ�
		msocket.SendNewData(req.GetPack(),req.GetPackLen());//���Ͳ�ѯ������Ϣ������

		CPacket packet(ServiceKey::GMProtocol::GM_ITEM_SELECT_ACK);
		int recvLen = msocket.recvData();//��ȡ��ѯ������Ϣ�Ļ�Ӧ
		memcpy(packet.GetPack(),msocket.recvBuffer(),recvLen);
		int pageCount=0;//�����ѯ�õ�������ҳ��
		int mID=0;//��ѯ�õ�����ƷID
		wstring itemName;//��ѯ�õ�����Ʒ����
		int timesLimit=0;//��Ʒʹ�õĴ�������
		int daysLimit=0;//��Ʒʹ�õ���������
		char StrInt[10];//����int���͵ı���
		ZeroMemory(StrInt, 10);
		int count = 0;
 		packet>>count;//��ȡ��ѯ���ĵ�������		
		if(count > 0)
		{
			pageCount = (count - 1)/iPageSize + 1;
			DBVect.InitCreate(count, 5);
			for(int i=0; i<count ; i++)
			{
				packet>>mID;
				packet>>itemName;
				packet>>timesLimit;
				packet>>daysLimit;
					
				_itoa(mID, StrInt, 10);//��ƷID
				DBVect.AddTFLV(CEnumCore::TagName::GT_MID, CEnumCore::TagFormat::TLV_INTEGER, sizeof(int), (unsigned char*)&StrInt);
				DBVect.AddTFLV(CEnumCore::TagName::GT_ItemName, CEnumCore::TagFormat::TLV_STRING, itemName.length()*2, (unsigned char*)operGT.wcharToChar(itemName));
				
				_itoa(timesLimit, StrInt, 10);//��������
				DBVect.AddTFLV(CEnumCore::TagName::GT_TimesLimit, CEnumCore::TagFormat::TLV_INTEGER, sizeof(int), (unsigned char*)&StrInt);
				_itoa(daysLimit, StrInt, 10);//��������
				DBVect.AddTFLV(CEnumCore::TagName::GT_DaysLimit, CEnumCore::TagFormat::TLV_INTEGER, sizeof(int), (unsigned char*)&StrInt);
				_itoa(pageCount, StrInt, 10);//ҳ��
				DBVect.AddTFLV(CEnumCore::TagName::PageCount, CEnumCore::TagFormat::TLV_INTEGER, sizeof(int), (unsigned char*)&StrInt);
			}
		}
		else
		{
			operGT.ReturnMessage(1, CEnumCore::Msg_Category::GT_OP_ADMIN, m_ServiceKey, "û����������ĵ�����Ϣ");
			return;
		}
		operGT.ReturnData(1, CEnumCore::Msg_Category::GT_OP_ADMIN, m_ServiceKey, &DBVect, 1);//��������
	}
	catch (...)
	{
	}*/
}

//��ӵ��ߡ�����
//type �C 0:һ����Ʒ;1:avatar;2:��;3:��)\t������id
void CGTOper::GT_ITEM_ADD(int userByID,char * ServerName, char * ServerIP,char * UserName,char * szMailTitle, char * szMailContent,int count,char * itemName)
{	
	wstring title = operGT.charToWChar(szMailTitle);//��ӵ��ߵı���	
	wstring content = operGT.charToWChar(szMailContent);//��ӵ��ߵ�����
	wstring Account = operGT.charToWChar(UserName);
	char retMessage[2048];
	char strItemName[40];
	char tmpMessage[256];
	ZeroMemory(retMessage,2048);
	ZeroMemory(tmpMessage,256);
	try
	{
		if(CONNECT_SERVERMANAGER_LOGIN(ServerIP,strLog))
		{
			int mID = 0;//��ƷID
			int itemNum = 0;//��������
			char strMID[20];
			char strItemNum[20];
			ZeroMemory(strMID, 20);//��ʼ����ʱ������ƷID�ı���		
			ZeroMemory(strItemNum, 20);//��ʼ����ʱ������������ı���
			
			int nNum = 1;//��һ���ַ��Ĵ���
			int mNum = 1;//�ڶ����ַ��Ĵ���
			
			CPacket req(ServiceKey::GMProtocol::GM_ITEM_ADD);
			req<<title<<content<<count;//������ʼ����⡢�ʼ����ݺ�ѭ������
			for(int i=0;i<count;i++)
			{
				ZeroMemory(strMID, 20);//��ʼ����ʱ������ƷID�ı���		
				ZeroMemory(strItemNum, 20);//��ʼ����ʱ������������ı���
				if(i == 0)//��1�ԣ�ItemID*ItemNum��
				{
					operGT.getSpecChar(itemName, "", mNum, "*", nNum, strMID);
					operGT.getSpecChar(itemName, "*", nNum, "|", mNum, strItemNum);
					nNum++;
				}
				else if(i == count-1)//���1��(ItemID*ItemNum)
				{
					operGT.getSpecChar(itemName, "|", mNum, "*", nNum, strMID);
					operGT.getSpecChar(itemName, "*", nNum, "", mNum, strItemNum);
				}
				else//�м��(ItemID*ItemNum)
				{
					operGT.getSpecChar(itemName, "|", mNum, "*", nNum, strMID);
					operGT.getSpecChar(itemName, "*", nNum, "|", mNum+1, strItemNum);
					nNum++;
					mNum++;
				}
				mID=atoi(strMID);
				itemNum = atoi(strItemNum);
				/////////////��¼log///////////////////
				sprintf(tmpMessage,"exec GT_ItemCodeToName %i",mID);
				operGT.QuerySingleValue("GT","127.0.0.1",4,tmpMessage,strItemName);
				sprintf(tmpMessage,"������ID��%i����������%s:������������%i",mID,strItemName,itemNum);
				sprintf(retMessage,"%s%s",retMessage,tmpMessage);

				req<<Account<<mID<<itemNum;

			}
			msocket.SendNewData(req.GetPack(),req.GetPackLen());//������ӵ��ߵ�����
			
			CPacket packet;
			int recvLen = msocket.recvData();//��ȡ��ӵ��ߵĻ�Ӧ
			memcpy(packet.GetPack(),msocket.recvBuffer(),recvLen);
			bool result = true;
			packet>>result;
			if(result)
			{
				sprintf(strLog,"���û���%s,����������%s,������ҡ�%s��ӵ���%s�ɹ�", operName, ServerName, UserName,retMessage);
			}
			else
			{
				sprintf(strLog,"���û���%s,����������%s,������ҡ�%s��ӵ���%sʧ��", operName, ServerName, UserName,retMessage);
			}
			logFile.WriteText("GTown",strLog);//���ɹ���Ϣд����־��
			logFile.WriteDBLog(userByID,"����Online","GT_ITEM_ADD",ServerIP,strLog);
		}
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_ADD_ITEM_ACK,operGT.GetSendBuf(),operGT.GetSendLength());

	}
	catch(...)
	{
		sprintf(strLog,"<%s>,�ڷ�����<%s>,�����<%s>��ӵ��߷���ϵͳ����", operName, ServerName, UserName);
		logFile.WriteText("GTown",strLog);
		logFile.WriteDBLog(userByID,"����Online","GT_ITEM_ADD",ServerIP,strLog);
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_ADD_ITEM_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
	}
}
//������ӵ���
bool CGTOper::GT_Import_Item(int userByID,char * ServerName, char * ServerIP, char * MailTitle,
							 char * MailContent, char * ListUserName, char * ListItemID, char * ListItemNum)
{	
	vector <ITEMPACKET> v_item;
	char * pUserName= strtok(ListUserName,"|");
	ITEMPACKET itemPacket;
	while(pUserName!=NULL)///�˺�
	{
		sprintf(itemPacket.szAccount,"%s",pUserName);
		v_item.push_back(itemPacket);
		pUserName= strtok(NULL,"|");
	}
	vector<ITEMPACKET>::iterator iter;
	char ItemID[10];
	char* pItemID=strtok(ListItemID,"|");
	for( iter = v_item.begin(); iter != v_item.end(); iter++ )
	{
		sprintf(ItemID,"%s",pItemID);
		(*iter).mID=StrToInt(ItemID);//����ID
		pItemID= strtok(NULL,"|");
		if(pItemID==NULL)
			break;
	}
	char ItemNum[10];
	char* pItemNum=strtok(ListItemNum,"|");
	for( iter = v_item.begin(); iter != v_item.end(); iter++ )
	{
		sprintf(ItemNum,"%s",pItemNum);
		(*iter).iNum=StrToInt(ItemNum);//��������
		pItemNum= strtok(NULL,"|");
		if(pItemNum==NULL)
			break;
	}
	wstring title = operGT.charToWChar(MailTitle);//��ӵ��ߵı���	
	wstring content = operGT.charToWChar(MailContent);
	wstring Account;
	char retMessage[2048];
	char strItemName[40];
	char tmpMessage[256];
	ZeroMemory(retMessage,2048);
	ZeroMemory(tmpMessage,256);
	bool result = true;
	if(CONNECT_SERVERMANAGER_LOGIN(ServerIP,retMessage))
	{
		for( iter = v_item.begin(); iter != v_item.end(); iter++ )//���ݵ�����ѭ�����
		{
			itemPacket = *iter;
			CPacket req(ServiceKey::GMProtocol::GM_ITEM_ADD);
			int count=1;
			req<<title<<content<<count;//������ʼ����⡢�ʼ����ݺ�ѭ������
			Account=operGT.charToWChar(itemPacket.szAccount);
			req<<Account<<itemPacket.mID<<itemPacket.iNum;
			msocket.SendNewData(req.GetPack(),req.GetPackLen());//������ӵ��ߵ�����
			
			CPacket packet;
			int recvLen = msocket.recvData();//��ȡ��ӵ��ߵĻ�Ӧ
			memcpy(packet.GetPack(),msocket.recvBuffer(),recvLen);
			packet>>result;
			sprintf(tmpMessage,"exec GT_ItemCodeToName %i",itemPacket.mID);//���ݵ���ID��õ�����
			operGT.QuerySingleValue("GT","127.0.0.1",4,tmpMessage,strItemName);

			if(result)
			{
				sprintf(tmpMessage,"����:ID%i������%s:��������%i�ɹ�",itemPacket.mID,strItemName,itemPacket.iNum);
				sprintf(retMessage,"%s%s",retMessage,tmpMessage);
			}
			else
			{
				sprintf(tmpMessage,"����ID%i��������%iʧ��",itemPacket.mID,itemPacket.iNum);
				sprintf(retMessage,"%s%s",retMessage,tmpMessage);
			}
			
		}
	/*	if(result)
		{
			sprintf(retMessage,"%s","������ӳɹ�");
		}
		else
		{
			sprintf(retMessage,"%s","�������ʧ��");
		}*/
	}
	operGT.BuildTLV(retMessage);
	operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,
			CEnumCore::ServiceKey::GT_Import_Item_Resp,operGT.GetSendBuf(),operGT.GetSendLength());

	logFile.WriteDBLog(userByID,"����Online","GT_Import_Item",ServerIP,retMessage);

/*	try
	{
		wstring title = operGT.charToWChar((char*)(recvVect.getTLVByTag(CEnumCore::TagName::GT_MailTitle,0)).lpdata);//��ӵ��ߵı���	
		wstring content = operGT.charToWChar((char*)(recvVect.getTLVByTag(CEnumCore::TagName::GT_MailContent,0)).lpdata);//��ӵ��ߵ�����
		char itemcount[2048];
		ZeroMemory(itemcount,2048);//��ӵ��ߵ���ϸ��Ϣ
		sprintf(itemcount,"%s",(char*)(recvVect.getTLVByTag(CEnumCore::TagName::PageCount, 0)).lpdata);//��ȡ��ӵ��ߵ���ϸ��Ϣ
		
		char itemName[2048];
		ZeroMemory(itemName,2048);//��ӵ��ߵ���ϸ��Ϣ
		sprintf(itemName,"%s",(char*)(recvVect.getTLVByTag(CEnumCore::TagName::GT_ItemName, 0)).lpdata);//��ȡ��ӵ��ߵ���ϸ��Ϣ

		int mID = 0;//��ƷID
		int itemNum = 0;//��������
		char strMID[20];
		char strItemNum[20];
		ZeroMemory(strMID, 20);//��ʼ����ʱ������ƷID�ı���		
		ZeroMemory(strItemNum, 20);//��ʼ����ʱ������������ı���
		
		int nNum = 1;//��һ���ַ��Ĵ���
		int mNum = 1;//�ڶ����ַ��Ĵ���
		
		CPacket req(ServiceKey::GMProtocol::GM_ITEM_ADD);
		req<<title<<content<<count;//������ʼ����⡢�ʼ����ݺ�ѭ������
		for(int i=0;i<count;i++)
		{
			ZeroMemory(strMID, 20);//��ʼ����ʱ������ƷID�ı���		
			ZeroMemory(strItemNum, 20);//��ʼ����ʱ������������ı���
			if(i == 0)
			{
				operGT.getSpecChar(itemName, "", mNum, "*", nNum, strMID);
				operGT.getSpecChar(itemName, "*", nNum, "|", mNum, strItemNum);
				nNum++;
			}
			else if(i == count-1)
			{
				operGT.getSpecChar(itemName, "|", mNum, "*", nNum, strMID);
				operGT.getSpecChar(itemName, "*", nNum, "", mNum, strItemNum);
			}
			else
			{
				operGT.getSpecChar(itemName, "|", mNum, "*", nNum, strMID);
				operGT.getSpecChar(itemName, "*", nNum, "|", mNum+1, strItemNum);
				nNum++;
				mNum++;
			}
			mID=atoi(strMID);
			itemNum = atoi(strItemNum);
			req<<Account<<mID<<itemNum;
		}
		msocket.SendNewData(req.GetPack(),req.GetPackLen());//������ӵ��ߵ�����
		
		CPacket packet;
		int recvLen = msocket.recvData();//��ȡ��ӵ��ߵĻ�Ӧ
		memcpy(packet.GetPack(),msocket.recvBuffer(),recvLen);
		bool result = true;
		packet>>result;
		
		else
		{
			sprintf(strLog,"<%s>,�ڷ�����<%s>,�����<%s>��ӵ��߳ɹ�", m_UserName, GT_ServerName, GT_Account);
			logFile.WriteText("GTown",strLog);//���ɹ���Ϣд����־��
			logFile.WriteDBLog(UserByID,"����Online","GT_ITEM_ADD",GT_ServerIP,strLog);
			operGT.ReturnMessage(1, CEnumCore::Msg_Category::GT_OP_ADMIN, m_ServiceKey, "��ӳɹ�");
		}
	}
	catch (CMemoryException* e)
	{
		
	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}*/
	return true;
}




bool CGTOper::UserInfo(char * ServerName, char * ServerIP,char * UserName, char * UserNick,int iIndex,int iPageSize)
{
	return true;
}
//�����ʺ�����
vector <CGlobalStruct::TFLV> CGTOper::KickPlayer(int userByID,char * ServerName, char * ServerIP,char * UserName)
{
	try
	{
		char retMessage[256];
		if(GT_KICK_USER(ServerIP,UserName,retMessage))
		{
			sprintf(retMessage,"%s","���˳ɹ�");
			sprintf(strLog,"<%s>,ǿ�Ʒ�����<%s> ���<%s>���߳ɹ�",operName,ServerIP,UserName);
			logFile.WriteText("GTown",strLog);
			logFile.WriteDBLog(userByID,"����Online","GT_KICK_USER",ServerIP,strLog);
		}
		else
		{
			sprintf(strLog,"<%s>,ǿ�Ʒ�����<%s> ���<%s>����ʧ��",operName,ServerIP,UserName);
			logFile.WriteText("GTown",strLog);//��ʧ����Ϣд����־��
			logFile.WriteDBLog(userByID,"����Online","GT_KICK_USER",ServerIP,strLog);
		}
		operGT.BuildTLV(retMessage);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_KICK_USER_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
	}
	catch(...)
	{
	}
	return true;
}

//��ѯ����(��������)
bool CGTOper::BroadTask_Query(char * ServerIP,int iIndex,int iPageSize)
{
	try
	{
		vector <CGlobalStruct::TFLV> DBVect=GT_BROADCAST_SELECT(ServerIP,iIndex,iPageSize);
		operGT.BuildTLV(DBVect);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_BROADCAST_SELECT_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
	}
	catch(...)
	{
	}
	return false;
}

//���¹���(ֻ��ɾ����boardMessage����IP)
bool CGTOper::BroadTask_Update(int userByID,int taskid,char * boardMessage,char * begintime,char * endtime,int interval,int status)
{
	char retMessage[256];
	if(CONNECT_SERVERMANAGER_LOGIN(boardMessage,retMessage))
	{
		GT_BROADCAST_REMOVE(taskid);
		logFile.WriteDBLog(userByID,"����Online","GT_BROADCAST_REMOVE",boardMessage,"ɾ������ɹ�");
		operGT.BuildTLV("ɾ������ɹ�");
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_BROADCAST_REMOVE_ACK,operGT.GetSendBuf(),operGT.GetSendLength());

	}
	
	
	return true;
}

//���빫��ServerIP����ServerIPList,GSServerIP�Ƿ���������
bool CGTOper::BroadTask_Insert(int userByID,char * ServerIP,char * GSServerIP,char * boardMessage,char * begintime,char * endtime,int interval)
{
	int iResult=0;
	char retMessage[256];
	int IsImmediate=StrToInt(GSServerIP);//�Ƿ���������
	wstring context = operGT.charToWChar(boardMessage);//��ȡ��������
	struct tm m_StartTime;///ת����ʼʱ������
	sscanf(begintime, "%d-%d-%d %d:%d:%d", &m_StartTime.tm_year,  &m_StartTime.tm_mon, &m_StartTime.tm_mday, &m_StartTime.tm_hour, &m_StartTime.tm_min, &m_StartTime.tm_sec);
	m_StartTime.tm_year = m_StartTime.tm_year - 1900;
	m_StartTime.tm_mon = m_StartTime.tm_mon - 1;
	size_t StartTime = mktime(&m_StartTime);///���ݿ�ʼʱ��ת������ֵ
	struct tm m_EndTime;///ת������ʱ������
	sscanf(endtime, "%d-%d-%d %d:%d:%d", &m_EndTime.tm_year,  &m_EndTime.tm_mon, &m_EndTime.tm_mday, &m_EndTime.tm_hour, &m_EndTime.tm_min, &m_EndTime.tm_sec);
	m_EndTime.tm_year = m_EndTime.tm_year - 1900;
	m_EndTime.tm_mon = m_EndTime.tm_mon - 1;
	size_t	EndTime = mktime(&m_EndTime);
	char *lpServerIP=strtok(ServerIP,"|");//��ѯ��־";"
	while(lpServerIP!=NULL)
	{
		
		if(CONNECT_SERVERMANAGER_LOGIN(ServerIP,retMessage))
		{
			if(GT_BROADCAST_ADD(IsImmediate,context,(DWORD)interval,StartTime,EndTime))
			{
				sprintf(strLog,"<%s>,�ڷ�����<%s>��ӹ���,����:<%s>,�ɹ�",operName,lpServerIP,boardMessage);
				logFile.WriteText("����Online",strLog);//���ɹ���Ϣд����־��
				logFile.WriteDBLog(userByID,"����Online","GT_BROADCAST_ADD",lpServerIP,strLog);
				iResult++;
			}
			else
			{
				sprintf(strLog,"<%s>,�ڷ�����<%s>��ӹ���,����:<%s>,ʧ��",operName,lpServerIP,boardMessage);
				logFile.WriteText("GTown",strLog);//��ʧ����Ϣд����־��
			}
		}
		lpServerIP=strtok(NULL,"|");//��ѯ��־";"

	}
	if(iResult==0)
	{
		operGT.BuildTLV("��ӹ���ʧ��");
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_BROADCAST_ADD_ACK,operGT.GetSendBuf(),operGT.GetSendLength());

	}
	else
	{
		operGT.BuildTLV("��ӹ���ɹ�");
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_BROADCAST_ADD_ACK,operGT.GetSendBuf(),operGT.GetSendLength());

	}	
	msocket.closeSocket();
	return true;
}

