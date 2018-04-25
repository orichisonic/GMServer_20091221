#include "StdAfx.h"
#include "WAInfo.h"
#include <iostream>
using namespace std;

CWAInfo::CWAInfo(void)
{
}

CWAInfo::~CWAInfo(void)
{
	//ж�������
//	m_MySocket.Uninitialize();
}

//��ʼ������
vector <CGlobalStruct::TFLV> CWAInfo::initialize(CSession * pSess,unsigned char * pbuf, int length)
{
	try
	{
//		m_MySocket.Initialize();	//��ʼ�������

		ZeroMemory(LocalIP, 30);
		logFile.ReadValue("Scheme", "DATABASE", "Address",LocalIP, 30);//��ȡ���ص����ݿ��ַ

		if(length>0)
		{
			recvVect.Create(length/8);//����ռ�
			recvVect.DeBody(pbuf,length);//�������ṹ
			srv_ClusterID=0;
			srv_Realmid=0;
		}
		pGameSess = pSess;	
		CLogic::SocketToUserName(pSess->GetSocket(), operName);//����ִ�в������û���
	}
	catch (...)
	{
		sprintf(errMessage,"��ʼ������");
		logFile.WriteErrorLog("WA",errMessage);
	}

	return recvVect.GetInnerList();
}
//����Զ�̷����
bool CWAInfo::Connect_Remote_Login(char * ServerIP, int gamedbid)
{
	bool bLoginFlag = false;	//��¼�ɹ���ʶ

	try
	{
		char path[256];
		char filepath[256];
		char szAccount[40];
		char szPassword[40];
		int iPort=0;
		GetCurrentDirectory(256,path);
		sprintf(filepath,"%s%s",path,"\\config\\MWA.ini");
		
		GetPrivateProfileString("USER","szAccount","gm01",szAccount,40,filepath);
		GetPrivateProfileString("USER","szPassword","111111",szPassword,40,filepath);

		REQ_LOGIN req_login;		//��¼����
		REP_LOGIN rep_login;		//��¼��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
// 		srv_ClusterID=0;
// 		srv_Realmid=0;
		
		char WA_GateWay[40];
		ZeroMemory(WA_GateWay,40);
		int ListenPort=0;//��ȡԶ�̶˿�

		char retMessage[255];
		memset(retMessage, 0, 255);
		
		if(!m_OperatorWA.GetServerIP("WA",ServerIP, gamedbid, WA_GateWay, &ListenPort, srv_ClusterID,srv_Realmid))
		{
			logFile.WriteLog("<��ʿ��˫>",operName,"��÷�����IPʧ��");	
		}
		else
		{
// 			if(!m_OperatorWA.GetClusterID("WA",ServerIP, gamedbid, WA_GateWay,srv_ClusterID,srv_Realmid))
// 			{
// 				//printf("%d",srv_ClusterID);
// 				//printf("%d",srv_Realmid);
// 				return false;
// 			}

			//Added by tengjie 2010-04-29
			if(srv_ClusterID == 0 && srv_Realmid == 0)
			{
				sprintf(retMessage, "��ȡ����ID���������IDʧ��");
				logFile.WriteLog("<��ʿ��˫>",operName,retMessage);
				return false;
			}
			cout<<"serverip:"<<WA_GateWay<<"\tport:"<<ListenPort<<endl;
			if(m_MySocket.MyConnect(WA_GateWay, ListenPort))
			{
				req_packetLength = sizeof(REQ_LOGIN);
				
				memset(sendbuf,0, 1024);
				memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
				memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ����24
				strcpy(req_login.loginUserName, szAccount);
				strcpy(req_login.loginPassWord, szPassword);
				req_login.srv_realm_id=srv_Realmid;
				memcpy(sendbuf+4, &req_login, req_packetLength);
				if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
				{
					logFile.WriteLog("WA",operName,"��������ʧ��");
				}
				else
				{
					memset(recvbuf, 0, 10240);
					int recvbyte = m_MySocket.MyRecv(recvbuf, 10240);	//Ĭ�ϳ�ʱʱ��20��
					if (recvbyte>0 && strcmp(recvbuf, ""))
					{
						short int packetSize = *(short int *)recvbuf;
						memcpy(&rep_login, recvbuf+4, packetSize);
						switch (rep_login.eResult)
						{
						case REP_LOGIN::E_SUCCESS:	//��¼�ɹ�
							{
//								logFile.WriteLog("<��ʿ��˫>",operName,"��½�ɹ�\n");
								bLoginFlag = true;
								return bLoginFlag;
							}
						default:					//��¼ʧ��
							sprintf(retMessage, "��½ʧ��(�û���Ϣ)[%s]", WA_GateWay);
							logFile.WriteLog("<��ʿ��˫>",operName,retMessage);
							break;
						}
					}
					else
					{
						sprintf(retMessage, "��½ʧ��(��������)[%s]", WA_GateWay);
						logFile.WriteLog("<��ʿ��˫>",operName, retMessage);
					}
				}
			}
			else
			{
				sprintf(retMessage, "��½ʧ��(���Ӳ�ѯ������)[%s]", WA_GateWay);
				logFile.WriteLog("<��ʿ��˫>", operName, retMessage);				
			}
		}

		
		delete []sendbuf;
		delete []recvbuf;

	}	
	catch (...)
	{
		logFile.WriteLog("<��ʿ��˫>",operName,"��¼δ֪����");
	}

	m_MySocket.Close();	

	return bLoginFlag;
}

//�����ѯ�û���Ϣ
vector <CGlobalStruct::TFLV> CWAInfo::WA_Net_UserInfo(char * ServerIP,char * UserName, char * Character,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	char retMessage[256];
	memset(retMessage, 0, 255);
	vector <NAME_ID_ELEM> vect_FriendInfo;
	
	try
	{

		int iPageCount=0;
		
		if(!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼������ʧ��", "��ɫ��Ϣ��ѯ");
		}
		else
		{
			if(!strcmp(UserName,""))
			{
				sprintf(retMessage, "��ɫ��Ϣ��ѯ(��ɫ����ѯ)��<����>%s, <��ɫ��>%s", ServerIP, Character);			
				logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG
				
				//�����ǳƲ�ѯ��ɫ��Ϣ
				WA_Net_UserInfoByChar(Character,&DBTFLV,1);
			}
			else
			{
				sprintf(retMessage, "��ɫ��Ϣ��ѯ(�ʺŲ�ѯ)��<����>%s, <�ʺ�>%s", ServerIP, UserName);			
				logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG
				
				REQ_CHARACTER_INFO req_character_info;
				REP_NAME_ID_LIST rep_name_id_list;
				short int req_packetLength;
				short int flag = 24;
				char *sendbuf = new char[1024];
				char * recvbuf = new char[10240];
				
				req_packetLength = sizeof(REQ_CHARACTER_INFO);				
				memset(sendbuf,0, 1024);
				memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
				memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
				req_character_info.req_type = REQ_CHARACTER_INFO::RT_CHAR_BASE_INFO;
				req_character_info.query_condition = ADMIN::QC_USERNAME;
				//����
				req_character_info.srv_realm_id=srv_Realmid;
				//��������ID
				req_character_info.srv_cluster_id=srv_ClusterID;
				memcpy(&req_character_info.user_name, UserName, strlen(UserName)+1);
				memcpy(sendbuf+4, &req_character_info, req_packetLength);
				
				if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�ʻ�������", "��ɫ��Ϣ��ѯ");
				}
				else
				{
					bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
					while (bRecvFlag)
					{
						//memset(recvbuf, 0, 20480);
						*recvbuf = NULL;
						
						// ���ý��ջ�������С
						DWORD nRecvBuf=10*1024;
						setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
						Sleep(1000);
						int recvbyte = m_MySocket.MyRecv(recvbuf,10480);
						if ( recvbyte>0 && strcmp(recvbuf, "") && vect_FriendInfo.size() == 0 )
						{
							short int packetSize = 0;	//ÿ���ṹ�����ݰ���
							int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
							int offset = 0;				//ָ��ƫ����
							
							while (nLeft>0)
							{
								packetSize = *(short int *)(recvbuf+offset);
								
								if (nLeft<packetSize+4 || packetSize <= 0)
								{
									break;
								}
								else
								{
									//ȡ�����ݰ�
									memcpy(&rep_name_id_list, recvbuf+offset+4, packetSize);
									
									//��������
									if (rep_name_id_list.allSend == 1)	//�յ����һ���ṹ��
									{
										bRecvFlag = false;
										
										if (rep_name_id_list.amount>0)
										{
											for (int i=0; i<rep_name_id_list.amount; i++)
											{
												vect_FriendInfo.push_back(rep_name_id_list.elems[i]);
											}
										}
										
										break;
									}
									else
									{
										if (rep_name_id_list.amount>0)
										{
											for (int i=0; i<rep_name_id_list.amount; i++)
											{
												vect_FriendInfo.push_back(rep_name_id_list.elems[i]);
											}
										}									
									}
									
									offset += (packetSize + 4);
									nLeft -= (packetSize + 4);
								}
							}
						}
						else
						{
							break;
							return DBTFLV;
						}
					}

					//��������
					int iPageCount = 0;
					int iFriendInfoLen = vect_FriendInfo.size();
					
					if ( iFriendInfoLen%iPageSize == 0 )
					{
						iPageCount = iFriendInfoLen/iPageSize;
					}
					else
					{
						iPageCount = iFriendInfoLen/iPageSize + 1;
					}

					vector <NAME_ID_ELEM>::iterator pCharId;
					for(pCharId = vect_FriendInfo.begin(); pCharId != vect_FriendInfo.end(); pCharId++)
					{
						WA_Net_UserInfoByChar(pCharId->char_name.charName, &DBTFLV, iPageCount);
					}

// 					for ( int i=iPageSize*(iIndex-1); i<iFriendInfoLen && i<iPageSize*iIndex; i++ )
// 					{
// 						WA_Net_UserInfoByChar(rep_name_id_list.elems[i].char_name.charName, &DBTFLV, iPageCount);
// 					}

// 					memset(recvbuf, 0, 10240);
// 					int recvbyte = m_MySocket.MyRecv(recvbuf, 10240);	//Ĭ�ϳ�ʱʱ��20��
// 					if (recvbyte>0 && strcmp(recvbuf, ""))
// 					{
// 						short int packetSize = *(short int *)recvbuf;														
// 						memcpy(&rep_name_id_list, recvbuf+4, packetSize);
// 						switch(rep_name_id_list.eResult)
// 						{
// 						case REP_NAME_ID_LIST::E_SUCCESS:
// 							{
// 								unsigned short usListLength = 0;
// 								usListLength = rep_name_id_list.amount;
// 								if ( usListLength > 0 )
// 								{
// 									if ( usListLength%iPageSize == 0 )
// 									{
// 										iPageCount = usListLength/iPageSize;
// 									}
// 									else
// 									{
// 										iPageCount = usListLength/iPageSize + 1;
// 									}
// 									
// 									for ( int i=iPageSize*(iIndex-1); i<usListLength && i<iPageSize*iIndex; i++ )
// 									{
// 										WA_Net_UserInfoByChar(rep_name_id_list.elems[i].char_name.charName, &DBTFLV, iPageCount);
// 									}
// 								}
// 
// 								break;
// 							}
// 						case REP_NAME_ID_LIST::E_FAILED_NOTEXIST:
// 							{
// 								DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�ʻ�������", "��ɫ��Ϣ��ѯ");
// 								break;
// 							}
// 						}
// 					}
// 					else
// 					{
// 						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��ɫ��ѯ��ʱ", "��ɫ��Ϣ��ѯ");
// 					}
				}

				delete []sendbuf;
				delete []recvbuf;
			}

			m_MySocket.Close();
		}
	}
	catch (...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "��ɫ��Ϣ��ѯ");
	}

	return DBTFLV;
}

//ͨ����ɫ���Ʋ�ѯ��ɫ��Ϣ
void CWAInfo::WA_Net_UserInfoByChar(char * Character,vector <CGlobalStruct::TFLV>* pDBTFLV,int iPageCount)
{
	try
	{
		CGlobalStruct::TFLV m_tflv;
		REQ_CHARACTER_INFO req_character_info;					//��ɫ��ѯ����
		REP_CHARACTER_BASE_INFO rep_character_base_info;		//��ɫ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		char strInt[30];
		
		req_packetLength = sizeof(REQ_CHARACTER_INFO);
		
		memset(sendbuf,0, 1024);
		memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
		memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
		req_character_info.req_type = REQ_CHARACTER_INFO::RT_CHAR_BASE_INFO;
		req_character_info.query_condition = ADMIN::QC_CHARACTERNAME;
		//����
		req_character_info.srv_realm_id=srv_Realmid;
		//��������ID
		req_character_info.srv_cluster_id=srv_ClusterID;
		memcpy(&req_character_info.user_name, Character, strlen(Character)+1);
		memcpy(sendbuf+4, &req_character_info, req_packetLength);		

		if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
		{
			logFile.WriteLog("WA","GM","��������ʧ��\n");	
		}
		else
		{
			memset(recvbuf, 0, 10240);
			// ���ý��ջ�������С
// 			DWORD nRecvBuf=10*1024;
// 			setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
// 			Sleep(1000);
			int recvbyte = m_MySocket.MyRecv(recvbuf, 10240);	//Ĭ�ϳ�ʱʱ��20��
			if (recvbyte>0 && strcmp(recvbuf, ""))
			{
				short int msg_type = *(short int *)(recvbuf+4);

				switch (msg_type)
				{
				case CMD_REP_CHARACTER_BASE_INFO:
					{
						short int packetSize = *(short int *)recvbuf;						

						memcpy(&rep_character_base_info, recvbuf+4, packetSize);

						// ����id
						_itoa((int)rep_character_base_info.srv_realm_id,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_RealmId;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);
						
						// ��������id
						_itoa((int)rep_character_base_info.srv_cluster_id,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_ClusterID;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);
						
						// �ʺ�
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Account;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(rep_character_base_info.user_name)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",rep_character_base_info.user_name);
						pDBTFLV->push_back(m_tflv);
						
						// ��ɫid
						_itoa(rep_character_base_info.char_id,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_NickID;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);
						
						// ��ɫ����
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_UserNick;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(rep_character_base_info.char_name)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",rep_character_base_info.char_name);
						pDBTFLV->push_back(m_tflv);

						//��ɫ��ν1
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_TitleName1;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(rep_character_base_info.char_title1)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",rep_character_base_info.char_title1);
						pDBTFLV->push_back(m_tflv);
						
						//��ɫ��ν2
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_TitleName2;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(rep_character_base_info.char_title2)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",rep_character_base_info.char_title2);
						pDBTFLV->push_back(m_tflv);

// 						//��ɫ����(ְҵ�Ա��)
// 						_itoa((int)rep_character_base_info.char_type,strInt,10);
// 						m_tflv.nIndex=pDBTFLV->size()+1;
// 						m_tflv.m_tagName=CEnumCore::TagName::WA_CharType;
// 						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
// 						m_tflv.m_tvlength = strlen(strInt)+1;
// 						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
// 						pDBTFLV->push_back(m_tflv);

						//��ɫ����(ְҵ�Ա��)
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_CharType;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						switch (rep_character_base_info.char_type)
						{
						case 1:
							m_tflv.m_tvlength = strlen("�н�ʿ")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"�н�ʿ");							
							break;
						case 2:
							m_tflv.m_tvlength = strlen("Ů��ʿ")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"Ů��ʿ");
							break;
						case 3:
							m_tflv.m_tvlength = strlen("������")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"������");
							break;
						case 4:
							m_tflv.m_tvlength = strlen("Ů����")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"Ů����");
							break;
						case 5:
							m_tflv.m_tvlength = strlen("��ս��ʦ")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"��ս��ʦ");
							break;
						case 6:
							m_tflv.m_tvlength = strlen("Ůս��ʦ")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"Ůս��ʦ");
							break;
						case 7:
							m_tflv.m_tvlength = strlen("��ħ��ʿ")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"��ħ��ʿ");
							break;
						case 8:
							m_tflv.m_tvlength = strlen("Ůħ��ʿ")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"Ůħ��ʿ");
							break;
						case 9:
							m_tflv.m_tvlength = strlen("������ʦ")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"������ʦ");
							break;
						case 10:
							m_tflv.m_tvlength = strlen("Ů����ʦ")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"Ů����ʦ");
							break;
						case 11:
							m_tflv.m_tvlength = strlen("�����ʦ")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"�����ʦ");
							break;
						case 12:
							m_tflv.m_tvlength = strlen("Ů���ʦ")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"Ů���ʦ");
							break;
						case 13:
							m_tflv.m_tvlength = strlen("�п�ʿ")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"�п�ʿ");
							break;
						case 14:
							m_tflv.m_tvlength = strlen("Ů��ʿ")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"Ů��ʿ");
							break;
						case 15:
							m_tflv.m_tvlength = strlen("�й���")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"�й���");
							break;
						case 16:
							m_tflv.m_tvlength = strlen("Ů����")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"Ů����");
							break;
						case 17:
							m_tflv.m_tvlength = strlen("��Ԫ��ʹ")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"��Ԫ��ʹ");
							break;
						case 18:
							m_tflv.m_tvlength = strlen("ŮԪ��ʹ")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"ŮԪ��ʹ");
							break;
						case 19:
							m_tflv.m_tvlength = strlen("��ʥ��ʿ")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"��ʥ��ʿ");
							break;
						case 20:
							m_tflv.m_tvlength = strlen("Ůʥ��ʿ")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"Ůʥ��ʿ");
							break;
						case 21:
							m_tflv.m_tvlength = strlen("�������")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"�������");
							break;
						case 22:
							m_tflv.m_tvlength = strlen("Ů�����")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"Ů�����");
							break;
						case 23:
							m_tflv.m_tvlength = strlen("�����")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"�����");
							break;
						case 24:
							m_tflv.m_tvlength = strlen("Ů���")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"Ů���");
							break;
						default:
							m_tflv.m_tvlength = strlen("δ֪����")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"δ֪����");
							break;
						}
						pDBTFLV->push_back(m_tflv);

						// �Ƿ�ɾ��
//						_itoa((int)rep_character_base_info.is_deleted,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_IsDelete;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						if (rep_character_base_info.is_deleted)
						{
							m_tflv.m_tvlength = strlen("��ɾ��")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"��ɾ��");
						}
						else
						{
							m_tflv.m_tvlength = strlen("δɾ��")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"δɾ��");
						}
						pDBTFLV->push_back(m_tflv);

						//�Ƿ�����
//						_itoa((int)rep_character_base_info.is_online,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_IsOnline;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						if (rep_character_base_info.is_online)
						{
							m_tflv.m_tvlength = strlen("����")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"����");
						}
						else
						{
							m_tflv.m_tvlength = strlen("����")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"����");
						}
						pDBTFLV->push_back(m_tflv);

						//�Ƿ��ͣ
//						_itoa((int)rep_character_base_info.is_closed,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_IsClosed;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						if (rep_character_base_info.is_closed)
						{
							m_tflv.m_tvlength = strlen("�ѷ�ͣ")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"�ѷ�ͣ");
						}
						else
						{
							m_tflv.m_tvlength = strlen("δ��ͣ")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"δ��ͣ");
						}
						pDBTFLV->push_back(m_tflv);

						//��ɫ�ȼ�
						_itoa((int)rep_character_base_info.char_level,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Level;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//��Ϸ������
						_ultoa((unsigned long)rep_character_base_info.money,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Money;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);
						
						//9�α�����
						_ultoa((unsigned long)rep_character_base_info.nineyou_money,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_NineyouMoney;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//������
						_ultoa((unsigned long)rep_character_base_info.reserve_money,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_ReserveMoney;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//��ǰ���ڳ�����
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_CurSceneName;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(rep_character_base_info.cur_scene_name) + 1;
						sprintf((LPSTR)&m_tflv.lpdata, "%s", rep_character_base_info.cur_scene_name);
						pDBTFLV->push_back(m_tflv);

						//��ǰ���ڳ���id
						_ultoa((unsigned long)rep_character_base_info.cur_scene_id,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_CurSceneID;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//��ǰ���ڳ�������
						_ultoa((unsigned long)rep_character_base_info.cur_scene_pos,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_CurScenePos;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//����ֵ
						_ultoa((unsigned long)rep_character_base_info.hp,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Hp;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//ħ��ֵ
						_itoa((int)rep_character_base_info.mp,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Mp;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//����ֵ
						_itoa((int)rep_character_base_info.vitality,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Vitality;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//�ƶ�ֵ
						_itoa((int)rep_character_base_info.good_evil,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_GoodEvil;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//������
						_itoa((int)rep_character_base_info.physical_attack,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_PhysicalAttack;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//�������
						_itoa((int)rep_character_base_info.physical_defense,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_PhysicalDefense;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//ħ������
						_itoa((int)rep_character_base_info.magic_attack,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_MagicAttack;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//ħ������
						_itoa((int)rep_character_base_info.magic_defense,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_MagicDefense;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//������
						sprintf(strInt, "%f", rep_character_base_info.critical);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Critical;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//����
						_itoa((int)rep_character_base_info.strength,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Strength;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//����
						_itoa((int)rep_character_base_info.dexterity,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Dexterity;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);
						
						//����
						_itoa((int)rep_character_base_info.intellect,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Intellect;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//����
						_itoa((int)rep_character_base_info.corporeity,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Corporeity;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);
						
						//Ǳ����
						_itoa((int)rep_character_base_info.potential_point,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_PotentialPoint;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);
						
						//����
						_ultoa((unsigned long)rep_character_base_info.exp,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Exp;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);
						
						//��������
						_ultoa((unsigned long)rep_character_base_info.exp_next_level,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_ExpNextLevel;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//������
						sprintf(strInt, "%f", rep_character_base_info.physical_resist);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_PhysicalResist;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);
						
						//��������
						sprintf(strInt, "%f", rep_character_base_info.magic_resist);
						m_tflv.nIndex = pDBTFLV->size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_MagicResist;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);
						
						//���濹��
						sprintf(strInt, "%f", rep_character_base_info.fire_resist);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_FireResist;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//��������
						sprintf(strInt, "%f", rep_character_base_info.cold_resist);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_ColdResist;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//���ؿ���
						sprintf(strInt, "%f", rep_character_base_info.poison_resist);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_PoisonResist;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//PageCount
						_itoa(iPageCount,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PageCount;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						break;
					}
				case CMD_REP_CHARACTER_INFO_FAIL:
					{
						short int packetSize = *(short int *)recvbuf;
						REP_CHARACTER_INFO_FAIL rep_character_info_fail;
						memcpy(&rep_character_info_fail, recvbuf+4, packetSize);
						printf("��ѯ��ɫʧ��\n");
					}
					break;
				}
				
			}
		}

		delete []sendbuf;
		delete []recvbuf;
	}
	catch (...)
	{
		sprintf(errMessage,"��ѯ��ɫ��Ϣ����");
		logFile.WriteErrorLog("WA",errMessage);
	}

}

//��������
vector <CGlobalStruct::TFLV> CWAInfo::WA_Net_KickPlayer(int userByID,char * ServerIP,char * UserName, char * Character)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{

		REQ_KICK_USER req_kick_user;		//��ɫ��ѯ����
		REP_KICK_USER rep_kick_user;		//��ɫ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];		

		req_packetLength = sizeof(REQ_KICK_USER);

		memset(sendbuf,0, 1024);
		memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
		memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�

		//��ʿgameDBID=1
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "GM����");
		}
		else
		{
			sprintf(retMessage, "GM���ˣ�<����>%s, <�ʺ�>%s, <��ɫ��>%s", ServerIP, UserName, Character);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG
			
			if(!strcmp(UserName,""))		//��ɫ����
			{
				req_kick_user.kickCondition = ADMIN::QC_CHARACTERNAME;
				memcpy(&req_kick_user.char_name, Character, strlen(Character)+1);

			}
			else							//�ʺ�����
			{
				req_kick_user.kickCondition = ADMIN::QC_USERNAME;
				memcpy(&req_kick_user.user_name, UserName, strlen(UserName)+1);
			}
			//����
			req_kick_user.srv_realm_id=srv_Realmid;
			//��������ID
			req_kick_user.srv_cluster_id=srv_ClusterID;
			memcpy(sendbuf+4, &req_kick_user, req_packetLength);

			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "GM����");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				int recvbyte = m_MySocket.MyRecv(recvbuf, 10240);	//Ĭ�ϳ�ʱʱ��20��
				if (recvbyte>0 && strcmp(recvbuf, ""))
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_kick_user, recvbuf+4, packetSize);
									
					switch (rep_kick_user.eResult)
					{
					case REP_KICK_USER::E_SUCCESS:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "���˳ɹ�", "GM����");
						break;
					case REP_KICK_USER::E_FAILED_CHAR_NOTINGAME:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��ɫ������", "GM����");
						break;
					default:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "����ʧ��", "GM����");
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "GM���˳�ʱ", "GM����");
				}
			}

			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "GM����");
	}

	m_OperatorWA.GmOperLog(userByID, ServerIP, "GM����", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//�����ͣ
vector <CGlobalStruct::TFLV> CWAInfo::WA_Net_ClosePlayer(int userByID,char * ServerIP,char * UserName,int CharacterID, char * Character, int iTime, int Reason, char *description)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{

		REQ_CLOSE_ACCOUNT req_close_account;		//��ͣ�ʺ�����
		REP_CLOSE_ACCOUNT rep_close_account;		//��ͣ�ʺŻ�Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];

		//Added by tengjie 2010-07-30
		CString New_Time = "";

		req_packetLength = sizeof(REQ_CLOSE_ACCOUNT);
		
		memset(sendbuf,0, 1024);
		memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
		memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�

		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "�ʺŷ�ͣ");
		}
		else
		{
			sprintf(retMessage, "�ʺŷ�ͣ��<����>%s, <�ʺ�>%s, <ʱ��>%d��, <ԭ��>%d, <����>%s", ServerIP, UserName, iTime, Reason, description);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			if(!strcmp(UserName,""))	//��ɫ��ͣ
			{
				req_close_account.closeCondition = ADMIN::QC_CHARACTERNAME;
				memcpy(&req_close_account.char_name, Character, strlen(Character)+1);
				
			}
			else						//�ʺŷ�ͣ
			{
				req_close_account.closeCondition = ADMIN::QC_USERNAME;
				memcpy(&req_close_account.user_name, UserName, strlen(UserName)+1);
			}

			switch (Reason)
			{
// 			case 0:	// δ֪
// 				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_UNKNOWN;
// 				break;
			case 1:	// ϵͳ��ͣ
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_SYSTEM;
				break;
			case 2: // �û�����
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_SYSTEM_TEMP;
				break;
			case 3: // �û�����
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_USERCUSTOM;
				break;
			case 4: // �û���ʱ����
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_USERCUSTOM_TEMP;
				break;
			default:// δ֪
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_SYSTEM;
				break;
			}
			//����
			req_close_account.srv_realm_id=srv_Realmid;
			//��������ID
			req_close_account.srv_cluster_id=srv_ClusterID;

			req_close_account.duration_time = iTime;

			//Modified by tengjie 2010-05-13
//			strcpy(req_close_account.description, description);		
			strcpy(req_close_account.description, "");
			memcpy(sendbuf+4, &req_close_account, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "�ʺŷ�ͣ");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				int recvbyte = m_MySocket.MyRecv(recvbuf, 10240);	//Ĭ�ϳ�ʱʱ��20��
				if (recvbyte>0 && strcmp(recvbuf, ""))
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_close_account, recvbuf+4, packetSize);
					
					switch (rep_close_account.eResult)
					{
					case REP_CLOSE_ACCOUNT::E_SUCCESS:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��ͣ�ɹ�", "�ʺŷ�ͣ");

						//Added by tengjie 2010-05-13
						WA_SqlForUserInfo(UserName,ServerIP,iTime,description);
						//Added by tengjie 2010-07-29:�ʻ����ļ�¼��־�ӿ�
						New_Time.Format("%d", iTime/60);
						UserLogInterface(ServerIP,UserName,New_Time.GetBuffer(0),38,"1");
						break;
					case REP_CLOSE_ACCOUNT::E_FAILED_ACCOUNT_NOTEXIST:	
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�ʺŲ�����", "�ʺŷ�ͣ");
						break;
					default:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��ͣʧ��", "�ʺŷ�ͣ");
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�ʺŷ�ͣ��ʱ", "�ʺŷ�ͣ");
				}
			}

			m_MySocket.Close();
		}
	
		delete []sendbuf;
		delete []recvbuf;
	}
	catch (...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "�ʺŷ�ͣ");
	}

	m_OperatorWA.GmOperLog(userByID, ServerIP, "�ʺŷ�ͣ", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//������ͣ
vector <CGlobalStruct::TFLV> CWAInfo::WA_Net_ClosePlayerGroup(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character,int iTime, int Reason, char *description)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	char retMessage[255];
	memset(retMessage, 0, 255);

	////////--����--///////////////////////////////////////////////////////////
	try
	{
		REQ_KICK_USER req_kick_user;		//��ɫ��ѯ����
		REP_KICK_USER rep_kick_user;		//��ɫ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];		
		
		req_packetLength = sizeof(REQ_KICK_USER);
		
		memset(sendbuf,0, 1024);
		memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
		memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�

		cout<<"ServerIP:"<<ServerIP<<endl;
		if (!Connect_Remote_Login(ServerIP, 1))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "GM����");
		}
		else
		{
			sprintf(retMessage, "GM���ˣ�<����>%s, <�ʺ�>%s, <��ɫ��>%s", ServerIP, UserName, Character);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG
			
			if(!strcmp(UserName,""))		//��ɫ����
			{
				req_kick_user.kickCondition = ADMIN::QC_CHARACTERNAME;
				memcpy(&req_kick_user.char_name, Character, strlen(Character)+1);
	
			}
			else							//�ʺ�����
			{
				req_kick_user.kickCondition = ADMIN::QC_USERNAME;
				memcpy(&req_kick_user.user_name, UserName, strlen(UserName)+1);
			}
			//����
			req_kick_user.srv_realm_id=srv_Realmid;
			//��������ID
			req_kick_user.srv_cluster_id=srv_ClusterID;
			memcpy(sendbuf+4, &req_kick_user, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "GM����");
				return DBTFLV;
			}
			else
			{
				memset(recvbuf, 0, 10240);
				int recvbyte = m_MySocket.MyRecv(recvbuf, 10240);	//Ĭ�ϳ�ʱʱ��20��
				if (recvbyte>0 && strcmp(recvbuf, ""))
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_kick_user, recvbuf+4, packetSize);
					
					switch (rep_kick_user.eResult)
					{
					case REP_KICK_USER::E_SUCCESS:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "���˳ɹ�", "GM����");
						break;
					case REP_KICK_USER::E_FAILED_CHAR_NOTINGAME:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��ɫ������", "GM����");
						break;
					default:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "����ʧ��", "GM����");
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "GM���˳�ʱ", "GM����");
				}
			}
			
			//m_MySocket.Close();
		}
		
		delete []sendbuf;
		sendbuf = NULL;
		delete []recvbuf;
		recvbuf = NULL;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "GM����");
		return DBTFLV;
	}	
	//if (!DBTFLV.empty())
	//	return DBTFLV;
	m_OperatorWA.GmOperLog(userByID, ServerIP, "GM����", retMessage, DBTFLV[0]);

	/////////////////////////////////////////////////////////////////////////////
		try
	{
		//Added by tengjie 2010-08-05
// 		New_Time.Format("%d", iTime/60);
// 		if(!UserLogInterface(ServerIP,UserName,New_Time.GetBuffer(0),38,"1",1).compare("RESULT_6"))
// 		{
// 			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ͣ", "�ʺŷ�ͣ");
// 			WA_SqlForUserInfo(UserName,ServerIP,iTime,description);
// 			return DBTFLV;
// 		}

		REQ_CLOSE_ACCOUNT req_close_account;		//��ͣ�ʺ�����
		REP_CLOSE_ACCOUNT rep_close_account;		//��ͣ�ʺŻ�Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];

		//Added by tengjie 2010-07-30
		CString New_Time = "";

		req_packetLength = sizeof(REQ_CLOSE_ACCOUNT);
		
		memset(sendbuf,0, 1024);
		memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
		memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
		/*
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "�ʺŷ�ͣ");
		}
		else
		{
		*/
			sprintf(retMessage, "�ʺŷ�ͣ��<����>%s, <�ʺ�>%s, <ʱ��>%d��, <ԭ��>%d, <����>%s", ServerIP, UserName, iTime, Reason, description);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			if(!strcmp(UserName,""))	//��ɫ��ͣ
			{
				req_close_account.closeCondition = ADMIN::QC_CHARACTERNAME;
				memcpy(&req_close_account.char_name, Character, strlen(Character)+1);
				
			}
			else						//�ʺŷ�ͣ
			{
				req_close_account.closeCondition = ADMIN::QC_USERNAME;
				memcpy(&req_close_account.user_name, UserName, strlen(UserName)+1);
			}

			switch (Reason)
			{
// 			case 0:	// δ֪
// 				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_UNKNOWN;
// 				break;
			case 1:	// ϵͳ��ͣ
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_SYSTEM;
				break;
			case 2: // �û�����
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_SYSTEM_TEMP;
				break;
			case 3: // �û�����
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_USERCUSTOM;
				break;
			case 4: // �û���ʱ����
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_USERCUSTOM_TEMP;
				break;
			default:// δ֪
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_SYSTEM;
				break;
			}
			//����
			req_close_account.srv_realm_id=srv_Realmid;
			//��������ID
			req_close_account.srv_cluster_id=srv_ClusterID;

			req_close_account.duration_time = iTime;

			//Modified by tengjie 2010-05-13
//			strcpy(req_close_account.description, description);		
			strcpy(req_close_account.description, "");
			memcpy(sendbuf+4, &req_close_account, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "�ʺŷ�ͣ");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				int recvbyte = m_MySocket.MyRecv(recvbuf, 10240);	//Ĭ�ϳ�ʱʱ��20��
				if (recvbyte>0 && strcmp(recvbuf, ""))
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_close_account, recvbuf+4, packetSize);
					
					switch (rep_close_account.eResult)
					{
					case REP_CLOSE_ACCOUNT::E_SUCCESS:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��ͣ�ɹ�", "�ʺŷ�ͣ");
						
						//Added by tengjie 2010-05-13
						WA_SqlForUserInfo(UserName,ServerIP,iTime,description);
						//Added by tengjie 2010-07-29:�ʻ����ļ�¼��־�ӿ�
						New_Time.Format("%d", iTime/60);
						UserLogInterface(ServerIP,UserName,New_Time.GetBuffer(0),38,"1");
						break;
					case REP_CLOSE_ACCOUNT::E_FAILED_ACCOUNT_NOTEXIST:	
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�ʺŲ�����", "�ʺŷ�ͣ");
						break;
					default:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��ͣʧ��", "�ʺŷ�ͣ");
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�ʺŷ�ͣ��ʱ", "�ʺŷ�ͣ");
				}
			}

			m_MySocket.Close();
		//}
	
		delete []sendbuf;
		sendbuf = NULL;
		delete []recvbuf;
		recvbuf = NULL;
	}
	catch (...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "�ʺŷ�ͣ");
	}

	m_OperatorWA.GmOperLog(userByID, ServerIP, "�ʺŷ�ͣ", retMessage, DBTFLV[0]);
	return DBTFLV;
}


//������
vector <CGlobalStruct::TFLV> CWAInfo::WA_Net_OpenPlayer(int userByID,char * ServerIP,char * UserName,int CharacterID,char * Character)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{
		REQ_AVAILABLE_ACCOUNT req_available_account;		//����ʺ�����
		REP_AVAILABLE_ACCOUNT rep_available_account;		//����ʺŻ�Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];

		req_packetLength = sizeof(REQ_AVAILABLE_ACCOUNT);
		
		memset(sendbuf,0, 1024);
		memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
		memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "�ʺŽ��");
		}
		else
		{
			sprintf(retMessage, "�ʺŽ�⣺<����>%s, <�ʺ�>%s", ServerIP, UserName);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			if(!strcmp(UserName,""))	//��ɫ���
			{
				req_available_account.availableCondition = ADMIN::QC_CHARACTERNAME;
				memcpy(&req_available_account.char_name, Character, strlen(Character)+1);
				
			}
			else						//�ʺŽ��
			{
				req_available_account.availableCondition = ADMIN::QC_USERNAME;
				memcpy(&req_available_account.user_name, UserName, strlen(UserName)+1);
			}
			//����
			req_available_account.srv_realm_id=srv_Realmid;
			//��������ID
			req_available_account.srv_cluster_id=srv_ClusterID;
			memcpy(sendbuf+4, &req_available_account, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "�ʺŽ��");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				int recvbyte = m_MySocket.MyRecv(recvbuf, 10240);	//Ĭ�ϳ�ʱʱ��20��
				if (recvbyte>0 && strcmp(recvbuf, ""))
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_available_account, recvbuf+4, packetSize);
					
					switch (rep_available_account.eResult)
					{
					case REP_AVAILABLE_ACCOUNT::E_SUCCESS:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "���ɹ�", "�ʺŽ��");
						WA_SqlForUserInfo(UserName,"",0,"",1);
						//Added by tengjie 2010-07-29:�ʻ����ļ�¼��־�ӿ�
						UserLogInterface(ServerIP,UserName,"",38);
						break;
					case REP_AVAILABLE_ACCOUNT::E_FAILED_ACCOUNT_NOTEXIST:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�ʺŲ�����", "�ʺŽ��");
						break;
					default:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "���ʧ��", "�ʺŽ��");
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�ʺŽ�ⳬʱ", "�ʺŽ��");
				}
			}

			m_MySocket.Close();
		}

		delete []sendbuf;
		delete []recvbuf;
	}
	catch (...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "�ʺŽ��");
	}

	m_OperatorWA.GmOperLog(userByID, ServerIP, "�ʺŽ��", retMessage, DBTFLV[0]);
	return DBTFLV;
}

// add by ltx

//������Ϣ��ѯ
vector <CGlobalStruct::TFLV> CWAInfo::WA_Net_FriendInfo(char *ServerIP, char *GameRegion, char *Account, char *Character, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <CHARACTER_FRIEND_INFO_ITEM> vect_FriendInfo;
	char strInt[16];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		

		REQ_CHARACTER_INFO req_character_info;				//������Ϣ��ѯ����
		CHARACTER_FRIEND_INFO character_friend_info;		//������Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];

		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "������Ϣ��ѯ");			
		}
		else
		{
			sprintf(retMessage, "������Ϣ��ѯ��<����>%s, <�ʺ�>%s, <��ɫ��>%s", ServerIP, Account, Character);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG
			
			req_packetLength = sizeof(REQ_CHARACTER_INFO);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			req_character_info.req_type = REQ_CHARACTER_INFO::RT_CHAR_FRIEND_INFO;
			req_character_info.query_condition = ADMIN::QC_CHARACTERNAME;
			//����
			req_character_info.srv_realm_id=srv_Realmid;
			//��������ID
			req_character_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_character_info.user_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_character_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "������Ϣ��ѯ");
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
 					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_FriendInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&character_friend_info, recvbuf+offset+4, packetSize);
								
								//��������
								if (character_friend_info.allSend == 1)	//�յ����һ���ṹ��
								{
									bRecvFlag = false;

									if (character_friend_info.amount>0)
									{
										for (int i=0; i<character_friend_info.amount; i++)
										{
											vect_FriendInfo.push_back(character_friend_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (character_friend_info.amount>0)
									{
										for (int i=0; i<character_friend_info.amount; i++)
										{
											vect_FriendInfo.push_back(character_friend_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iFriendInfoLen = vect_FriendInfo.size();

				if ( iFriendInfoLen%iPageSize == 0 )
				{
					iPageCount = iFriendInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iFriendInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iFriendInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ���ѽ�ɫID
					_itoa((int)vect_FriendInfo[i].friendIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_friendIndex;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_friendName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_FriendInfo[i].friend_name.charName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_FriendInfo[i].friend_name.charName);
					DBTFLV.push_back(m_tflv);

					// ���ѵȼ�
//  					_itoa((int)vect_FriendInfo[i].friendLevel, strInt, 10);
//  					m_tflv.nIndex = DBTFLV.size()+1;
// 					m_tflv.m_tagName = CEnumCore::TagName::WA_friendLevel;
//  					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
// 					m_tflv.m_tvlength = strlen(strInt)+1;
//  					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
//  					DBTFLV.push_back(m_tflv);

					// ���ѵĽ�ɫ����
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::WA_friendTypeId;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					switch (vect_FriendInfo[i].friendTypeId)
					{
					case 1:
						m_tflv.m_tvlength = strlen("�н�ʿ")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"�н�ʿ");							
						break;
					case 2:
						m_tflv.m_tvlength = strlen("Ů��ʿ")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"Ů��ʿ");
						break;
					case 3:
						m_tflv.m_tvlength = strlen("������")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"������");
						break;
					case 4:
						m_tflv.m_tvlength = strlen("Ů����")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"Ů����");
						break;
					case 5:
						m_tflv.m_tvlength = strlen("��ս��ʦ")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��ս��ʦ");
						break;
					case 6:
						m_tflv.m_tvlength = strlen("Ůս��ʦ")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"Ůս��ʦ");
						break;
					case 7:
						m_tflv.m_tvlength = strlen("��ħ��ʿ")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��ħ��ʿ");
						break;
					case 8:
						m_tflv.m_tvlength = strlen("Ůħ��ʿ")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"Ůħ��ʿ");
						break;
					case 9:
						m_tflv.m_tvlength = strlen("������ʦ")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"������ʦ");
						break;
					case 10:
						m_tflv.m_tvlength = strlen("Ů����ʦ")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"Ů����ʦ");
						break;
					case 11:
						m_tflv.m_tvlength = strlen("�����ʦ")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"�����ʦ");
						break;
					case 12:
						m_tflv.m_tvlength = strlen("Ů���ʦ")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"Ů���ʦ");
						break;
					case 13:
						m_tflv.m_tvlength = strlen("�п�ʿ")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"�п�ʿ");
						break;
					case 14:
						m_tflv.m_tvlength = strlen("Ů��ʿ")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"Ů��ʿ");
						break;
					case 15:
						m_tflv.m_tvlength = strlen("�й���")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"�й���");
						break;
					case 16:
						m_tflv.m_tvlength = strlen("Ů����")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"Ů����");
						break;
					case 17:
						m_tflv.m_tvlength = strlen("��Ԫ��ʹ")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��Ԫ��ʹ");
						break;
					case 18:
						m_tflv.m_tvlength = strlen("ŮԪ��ʹ")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"ŮԪ��ʹ");
						break;
					case 19:
						m_tflv.m_tvlength = strlen("��ʥ��ʿ")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��ʥ��ʿ");
						break;
					case 20:
						m_tflv.m_tvlength = strlen("Ůʥ��ʿ")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"Ůʥ��ʿ");
						break;
					case 21:
						m_tflv.m_tvlength = strlen("�������")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"�������");
						break;
					case 22:
						m_tflv.m_tvlength = strlen("Ů�����")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"Ů�����");
						break;
					case 23:
						m_tflv.m_tvlength = strlen("�����")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"�����");
						break;
					case 24:
						m_tflv.m_tvlength = strlen("Ů���")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"Ů���");
						break;
					default:
						m_tflv.m_tvlength = strlen("δ֪����")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"δ֪����");
						break;
					}
					DBTFLV.push_back(m_tflv);

					// ���ѵȼ�
					_itoa((int)vect_FriendInfo[i].friendLevel, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_friendLevel;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ���ܶ�
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::WA_friendIni;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					if (vect_FriendInfo[i].friendIni == 0)
					{
						m_tflv.m_tvlength = strlen("İ��")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"İ��");
					}
					else if (vect_FriendInfo[i].friendIni>=1 && vect_FriendInfo[i].friendIni<= 99)
					{
						m_tflv.m_tvlength = strlen("��ʶ")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��ʶ");
					}
					else if (vect_FriendInfo[i].friendIni>=100 && vect_FriendInfo[i].friendIni<= 299)
					{
						m_tflv.m_tvlength = strlen("�Ѻ�")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"�Ѻ�");
					}
					else if (vect_FriendInfo[i].friendIni>=300 && vect_FriendInfo[i].friendIni<= 599)
					{
						m_tflv.m_tvlength = strlen("��Ϥ")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��Ϥ");
					}
					else if (vect_FriendInfo[i].friendIni>=600 && vect_FriendInfo[i].friendIni<= 1499)
					{
						m_tflv.m_tvlength = strlen("����")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"����");
					}
					else if (vect_FriendInfo[i].friendIni>=1500 && vect_FriendInfo[i].friendIni<= 3499)
					{
						m_tflv.m_tvlength = strlen("֪��")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"֪��");
					}
					else if (vect_FriendInfo[i].friendIni>=3500 && vect_FriendInfo[i].friendIni<= 5000)
					{
						m_tflv.m_tvlength = strlen("����/����")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"����/����");
					}
					else if (vect_FriendInfo[i].friendIni>5000)
					{
						m_tflv.m_tvlength = strlen("��")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��");
					}
					DBTFLV.push_back(m_tflv);					


					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);										
				}

			}

			m_MySocket.Close();
		}

		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "������Ϣ��ѯ");
	}
	
	return DBTFLV;
}

//��ɫ��Ʒ��Ϣ��ѯ
vector <CGlobalStruct::TFLV> CWAInfo::WA_Net_ItemInfo(char *ServerIP, char *GameRegion, char *Account, char *Character, int itemPos, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <CHARACTER_ITEM_INFO_ITEM> vect_ItemInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		

		REQ_CHARACTER_INFO req_character_info;				//��ɫ������Ʒ��Ϣ��ѯ����
		CHARACTER_ITEM_INFO character_item_info;			//��ɫ������Ʒ��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "װ����Ϣ��ѯ");							
		}
		else
		{
			sprintf(retMessage, "��Ʒ��Ϣ��ѯ��<����>%s, <�ʺ�>%s, <��ɫ��>%s, <��Ʒλ��>%d", ServerIP, Account, Character, itemPos);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_CHARACTER_INFO);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//�ж�װ��λ��
			switch (itemPos)
			{
			case ADMIN::EG_EQUIPMENT_ITEM:	//����
				req_character_info.req_type = REQ_CHARACTER_INFO::RT_CHAR_EQUIPMENT_INFO;
				break;
			case ADMIN::EG_BAG_ITEM:		//����
				req_character_info.req_type = REQ_CHARACTER_INFO::RT_CHAR_ITEM_INFO;
				break;
			case ADMIN::EG_BANK_ITEM:		//����
				req_character_info.req_type = REQ_CHARACTER_INFO::RT_CHAR_BANK_INFO;
				break;
			}
			
			req_character_info.query_condition = ADMIN::QC_CHARACTERNAME;
			//����
			req_character_info.srv_realm_id=srv_Realmid;
			//��������ID
			req_character_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_character_info.user_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_character_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "��Ʒ��Ϣ��ѯ");					
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_ItemInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&character_item_info, recvbuf+offset+4, packetSize);
								
								//��������
								if (character_item_info.allSend == 1)
								{
									bRecvFlag = false;
									if (character_item_info.amount>0)
									{
										for (int i=0; i<character_item_info.amount; i++)
										{
											vect_ItemInfo.push_back(character_item_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (character_item_info.amount>0)
									{
										for (int i=0; i<character_item_info.amount; i++)
										{
											vect_ItemInfo.push_back(character_item_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						bRecvFlag = false;
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iItemInfoLen = vect_ItemInfo.size();

				if ( iItemInfoLen%iPageSize == 0 )
				{
					iPageCount = iItemInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iItemInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iItemInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ���
					_itoa((int)vect_ItemInfo[i].itemIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemIndex;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ����ID
					_itoa((int)vect_ItemInfo[i].itemTypeId, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ItemID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_ItemInfo[i].itemName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_ItemInfo[i].itemName);
					DBTFLV.push_back(m_tflv);

					// ΨһID
					_ui64toa(vect_ItemInfo[i].itemUniqueId, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemUniqueId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ����
					_itoa((int)vect_ItemInfo[i].itemCount, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemCount;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// �;�
					_itoa((int)vect_ItemInfo[i].itemDur, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemDur;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);										
				}
			}
			
			m_MySocket.Close();
		}

		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "��Ʒ��ѯ");
	}

	return DBTFLV;
}

//������Ϣ��ѯ
vector <CGlobalStruct::TFLV> CWAInfo::WA_Net_EmblemInfo(char *ServerIP, char *GameRegion, char *Account, char *Character, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <CHARACTER_MEDAL_INFO_ITEM> vect_medalInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		

		REQ_CHARACTER_INFO req_character_info;				//������Ϣ��ѯ����
		CHARACTER_MEDAL_INFO character_medal_info;		//������Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "������Ϣ��ѯ");			
		}
		else
		{
			sprintf(retMessage, "������Ϣ��ѯ��<����>%s, <�ʺ�>%s, <��ɫ��>%s", ServerIP, Account, Character);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_CHARACTER_INFO);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			req_character_info.req_type = REQ_CHARACTER_INFO::RT_CHAR_MEDAL_INFO;
			req_character_info.query_condition = ADMIN::QC_CHARACTERNAME;
			//����
			req_character_info.srv_realm_id=srv_Realmid;
			//��������ID
			req_character_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_character_info.user_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_character_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "������Ϣ��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_medalInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&character_medal_info, recvbuf+offset+4, packetSize);
								
								//��������
								if (character_medal_info.allSend == 1)
								{
									bRecvFlag = false;

									if (character_medal_info.amount>0)
									{
										for (int i=0; i<character_medal_info.amount; i++)
										{
											vect_medalInfo.push_back(character_medal_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (character_medal_info.amount>0)
									{
										for (int i=0; i<character_medal_info.amount; i++)
										{
											vect_medalInfo.push_back(character_medal_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						bRecvFlag = false;
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int imedalInfoLen = vect_medalInfo.size();

				if ( imedalInfoLen%iPageSize == 0 )
				{
					iPageCount = imedalInfoLen/iPageSize;
				}
				else
				{
					iPageCount = imedalInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<imedalInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ����ID
					_itoa((int)vect_medalInfo[i].medalTypeId, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_medalTypeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_medalName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_medalInfo[i].medalName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_medalInfo[i].medalName);
					DBTFLV.push_back(m_tflv);

					// �;�
					_itoa((int)vect_medalInfo[i].medalDur, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_medalDur;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ����
					sprintf(strInt, "%f", vect_medalInfo[i].medalBringUp);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_medalBringUp;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ������ų���ֵ255Ϊ�޳���
					_itoa((int)vect_medalInfo[i].medalPetIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_medalPetIndex;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// �����ڳ��ﱳ����λ�� 255Ϊװ����
					_itoa((int)vect_medalInfo[i].medalPos, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_medalPos;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);										
				}
			}
			
			m_MySocket.Close();
		}

		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "������Ϣ��ѯ");
	}

	return DBTFLV;
}

//������Ϣ��ѯ
vector <CGlobalStruct::TFLV> CWAInfo::WA_Net_PetInfo(char *ServerIP, char *GameRegion, char *Account, char *Character, int petNo, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <CHARACTER_PET_INFO_ITEM> vect_petInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		

		REQ_CHARACTER_INFO req_character_info;				//��ɫ������Ϣ��ѯ����
		CHARACTER_PET_INFO character_pet_info;				//��ɫ������Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "������Ϣ��ѯ");
		}
		else
		{
			sprintf(retMessage, "������Ϣ��ѯ��<����>%s, <�ʺ�>%s, <��ɫ��>%s, <������>%d", ServerIP, Account, Character, petNo);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_CHARACTER_INFO);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			req_character_info.req_type = REQ_CHARACTER_INFO::RT_CHAR_PET_INFO;
			req_character_info.query_condition = ADMIN::QC_CHARACTERNAME;
			//����
			req_character_info.srv_realm_id=srv_Realmid;
			//��������ID
			req_character_info.srv_cluster_id=srv_ClusterID;
			req_character_info.param1 = petNo;	//�������
			memcpy(&req_character_info.user_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_character_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "������Ϣ��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_petInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&character_pet_info, recvbuf+offset+4, packetSize);
								
								//��������
								if (character_pet_info.allSend == 1)
								{
									bRecvFlag = false;

									if (character_pet_info.amount>0)
									{
										for (int i=0; i<character_pet_info.amount; i++)
										{
											vect_petInfo.push_back(character_pet_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (character_pet_info.amount>0)
									{
										for (int i=0; i<character_pet_info.amount; i++)
										{
											vect_petInfo.push_back(character_pet_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int ipetInfoLen = vect_petInfo.size();

				if ( ipetInfoLen%iPageSize == 0 )
				{
					iPageCount = ipetInfoLen/iPageSize;
				}
				else
				{
					iPageCount = ipetInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<ipetInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ����ID
					_itoa(vect_petInfo[i].petTypeId, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_petTypeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//Added by tengjie 2010-08-20
					// ����ΨһID
					_itoa(vect_petInfo[i].pet_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_PetUniqId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_petName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_petInfo[i].petName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_petInfo[i].petName);
					DBTFLV.push_back(m_tflv);

					// �ȼ�
					_itoa(vect_petInfo[i].petLevel, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_petLevel;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// �Ը�
					_itoa(vect_petInfo[i].petNature, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_petNature;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// �ɳ���
					sprintf(strInt, "%f", vect_petInfo[i].petGrowth);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_petGrowth;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ������
					_itoa(vect_petInfo[i].healthe, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_healthe;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ������
					_itoa(vect_petInfo[i].physical_attack, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_PhysicalAttack;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//�������
					_itoa(vect_petInfo[i].physical_defense, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_PhysicalDefense;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//ħ������
					_itoa(vect_petInfo[i].magic_attack, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MagicAttack;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					//ħ������
					_itoa(vect_petInfo[i].magic_defense, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MagicDefense;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//���湥��
					_itoa(vect_petInfo[i].fire_attack, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_FireAttack;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//��������
					_itoa(vect_petInfo[i].cold_attack, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ColdAttack;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//���ع���
					_itoa(vect_petInfo[i].poison_attack, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_PoisonAttack;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//���濹��
					sprintf(strInt, "%f", vect_petInfo[i].fire_resist);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_FireResist;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//��������
					sprintf(strInt, "%f", vect_petInfo[i].cold_resist);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ColdResist;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//���ؿ���
					sprintf(strInt, "%f", vect_petInfo[i].poison_resist);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_PoisonResist;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);										
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "������Ϣ��ѯ");
	}

	return DBTFLV;
}

//��ͣ�б��ѯ
vector <CGlobalStruct::TFLV> CWAInfo::WA_Net_CloseList(char *ServerIP, char *GameRegion, char *UserName, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <NAME_ID_ELEM> vect_CloseUser;

	try
	{
		logFile.WriteLog("<��ʿ��˫>",operName,"��ͣ�б��ѯ");//��LOG
		
		int iPageCount=0;
		
		if(!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼������ʧ��", "��ͣ�б��ѯ");
		}
		else
		{
			if(strcmp(UserName,""))
			{
				//�����ʺŲ�ѯ��ϸ��ͣ��Ϣ
				WA_Net_SingleCloseInfo(UserName, &DBTFLV, 1);
			}
			else
			{
				REQ_CLOSURE_ACCOUNT_INFO req_closure_account_info;
				REP_NAME_ID_LIST rep_name_id_list;					//�ʺŷ�ͣ��ѯ����
				short int req_packetLength;
				short int flag = 24;
				char *sendbuf = new char[1024];
				char * recvbuf = new char[20480];
				
				req_packetLength = sizeof(REQ_CLOSURE_ACCOUNT_INFO);
				
				memset(sendbuf,0, 1024);
				memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
				memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�

				req_closure_account_info.reqRange = REQ_CLOSURE_ACCOUNT_INFO::RR_ALL_CLOSUREACCOUNT;
				//����
				req_closure_account_info.srv_realm_id=srv_Realmid;
				//��������ID
				req_closure_account_info.srv_cluster_id=srv_ClusterID;
				memcpy(sendbuf+4, &req_closure_account_info, req_packetLength);	
				
				if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "��ͣ�б��ѯ");
				}
				else
				{

					bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
					while (bRecvFlag)
					{
						//memset(recvbuf, 0, 20480);
						*recvbuf = NULL;
						
						// ���ý��ջ�������С
						DWORD nRecvBuf=20*1024;
						setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
						Sleep(1000);
						int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
						if ( recvbyte>0 && (strcmp(recvbuf, "") && vect_CloseUser.size() == 0) )
						{
							short int packetSize = 0;	//ÿ���ṹ�����ݰ���
							int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
							int offset = 0;				//ָ��ƫ����
							
							while (nLeft>0)
							{
								packetSize = *(short int *)(recvbuf+offset);
								
								if (nLeft<packetSize+4 || packetSize <= 0)
								{
									break;
								}
								else
								{
									//ȡ�����ݰ�
									memcpy(&rep_name_id_list, recvbuf+offset+4, packetSize);
									
									//��������
									if (rep_name_id_list.allSend == 1)	//�յ����һ���ṹ��
									{
										bRecvFlag = false;
										
										if (rep_name_id_list.amount>0)
										{
											for (int i=0; i<rep_name_id_list.amount; i++)
											{
												vect_CloseUser.push_back(rep_name_id_list.elems[i]);
											}
										}
										
										break;
									}
									else
									{
										if (rep_name_id_list.amount>0)
										{
											for (int i=0; i<rep_name_id_list.amount; i++)
											{
												vect_CloseUser.push_back(rep_name_id_list.elems[i]);
											}
										}									
									}
									
									offset += (packetSize + 4);
									nLeft -= (packetSize + 4);
								}
							}
						}
						else
						{
							break;
						}
					}					
					
					//��������
					int iPageCount = 0;
					int iCloseUserLen = vect_CloseUser.size();
					
					if ( iCloseUserLen%iPageSize == 0 )
					{
						iPageCount = iCloseUserLen/iPageSize;
					}
					else
					{
						iPageCount = iCloseUserLen/iPageSize + 1;
					}
					
					for ( int i=iPageSize*(iIndex-1); i<iCloseUserLen && i<iPageSize*iIndex; i++ )
					{
						WA_Net_SingleCloseInfo(vect_CloseUser[i].char_name.charName, &DBTFLV, iPageCount);
					}
					
				}
				delete []sendbuf;
				delete []recvbuf;
			}

			m_MySocket.Close();
		}
	}
	catch(...)
	{

	}

	return DBTFLV;
}

//�ڲ����ò�ѯ�����ʺŷ�ͣ
void CWAInfo::WA_Net_SingleCloseInfo(char *UserName, vector <CGlobalStruct::TFLV>* pDBTFLV, int PageCount)
{
	try
	{
		CGlobalStruct::TFLV m_tflv;
		REQ_CLOSURE_ACCOUNT_INFO req_closure_account_info;					//�ʺŷ�ͣ��ѯ����
		REP_CLOSURE_ACCOUNT_INFO rep_closure_account_info;					//�ʺŷ�ͣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		char strInt[30];
		char strDateTime[30];
		
		req_packetLength = sizeof(REQ_CLOSURE_ACCOUNT_INFO);
		
		memset(sendbuf,0, 1024);
		memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
		memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�

		req_closure_account_info.reqRange = REQ_CLOSURE_ACCOUNT_INFO::RR_PER_CLOSUREACCOUNT;
		//����
		req_closure_account_info.srv_realm_id=srv_Realmid;
		//��������ID
		req_closure_account_info.srv_cluster_id=srv_ClusterID;
		memcpy(&req_closure_account_info.user_name, UserName, strlen(UserName)+1);
		memcpy(sendbuf+4, &req_closure_account_info, req_packetLength);		
		
		if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
		{
			logFile.WriteLog("��ʿ��˫",operName, "�����ʺŷ�ͣ��ѯ����ʧ��");
		}
		else
		{
			memset(recvbuf, 0, 10240);
			int recvbyte = m_MySocket.MyRecv(recvbuf, 10240);	//Ĭ�ϳ�ʱʱ��20��
			if (recvbyte>0 && strcmp(recvbuf, ""))
			{
				short int packetSize = *(short int *)recvbuf;						
				
				memcpy(&rep_closure_account_info, recvbuf+4, packetSize);

				switch (rep_closure_account_info.eResult)
				{
				case REP_CLOSURE_ACCOUNT_INFO::E_SUCCESS:
					{
						// �ʺ���
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Account;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(rep_closure_account_info.user_name.userName)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",rep_closure_account_info.user_name.userName);
						pDBTFLV->push_back(m_tflv);

						// �ʺ�ID
						_itoa((int)rep_closure_account_info.accountIndex,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_AccountID;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//��ͣԭ��
//						_itoa((int)rep_closure_account_info.closure_reason,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Reason;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						switch (rep_closure_account_info.closure_reason)
						{
						case 1:
							m_tflv.m_tvlength = strlen("ϵͳ��ͣ")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"ϵͳ��ͣ");
							break;
						case 2:
							m_tflv.m_tvlength = strlen("ϵͳ��ʱ��ͣ")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"ϵͳ��ʱ��ͣ");
							break;
						case 3:
							m_tflv.m_tvlength = strlen("�û�����")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"�û�����");
							break;
						case 4:
							m_tflv.m_tvlength = strlen("�û���ʱ����")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"�û���ʱ����");
							break;
						default:
							m_tflv.m_tvlength = strlen("δ֪ԭ��")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"δ֪ԭ��");
							break;

						}
						pDBTFLV->push_back(m_tflv);

						//Added by tengjie 2010-05-13:��ͣ����
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Content;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						if (!rep_closure_account_info.is_clousure)
							WA_SqlForUserInfo(UserName,"",0,"",1);
						CString szDes = WA_SearchDescription(UserName,2);
						m_tflv.m_tvlength = szDes.GetLength();
						sprintf((LPSTR)&m_tflv.lpdata,"%s",szDes.GetBuffer(0));
						pDBTFLV->push_back(m_tflv);

						//�Ƿ��ͣ
//						_itoa((int)rep_closure_account_info.is_clousure,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_IsClosed;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						if (rep_closure_account_info.is_clousure)
						{
							m_tflv.m_tvlength = strlen("�ѷ�ͣ")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"�ѷ�ͣ");
						}
						else
						{
							m_tflv.m_tvlength = strlen("δ��ͣ")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"δ��ͣ");
						}
						pDBTFLV->push_back(m_tflv);

						//�ϴε�¼ʱ��
						sprintf(strDateTime, "%4d-%02d-%02d %02d:%02d:%02d", 
							rep_closure_account_info.last_logout_time.year,
							rep_closure_account_info.last_logout_time.month,
							rep_closure_account_info.last_logout_time.day,
							rep_closure_account_info.last_logout_time.hour,
							rep_closure_account_info.last_logout_time.minute,
							rep_closure_account_info.last_logout_time.second);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_LastLoginTime;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(strDateTime)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strDateTime);
						pDBTFLV->push_back(m_tflv);
						
						// ��ͣ����ʱ��
						sprintf(strDateTime, "%4d-%02d-%02d %02d:%02d:%02d", 
							rep_closure_account_info.enable_time.year,
							rep_closure_account_info.enable_time.month,
							rep_closure_account_info.enable_time.day,
							rep_closure_account_info.enable_time.hour,
							rep_closure_account_info.enable_time.minute,
							rep_closure_account_info.enable_time.second);
							m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_EnableTime;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(strDateTime)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strDateTime);
						pDBTFLV->push_back(m_tflv);

						//PageCount
						_itoa(PageCount,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::PageCount;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength=sizeof(int);
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						break;
					}
				default:
					logFile.WriteLog("��ʿ��˫",operName, "�����ʺŷ�ͣ��ѯ���ؽ������");
					break;
				}
			}
		}

		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		logFile.WriteLog("��ʿ��˫",operName, "�����ʺŷ�ͣ��ѯδ֪����");		
	}
}

//����ɾ��
vector <CGlobalStruct::TFLV> CWAInfo::WA_Net_DeleteItem(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character, int itemIndex, int itemPos)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{

		REQ_DELETE_ITEM req_delete_item;				//����ɾ������
		REP_DELETE_ITEM rep_delete_item;				//����ɾ����Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼������ʧ��", "����ɾ��");				
		}
		else
		{		
			switch (itemPos)
			{
			case 0:	//����
				req_delete_item.itemGroup = ADMIN::EG_EQUIPMENT_ITEM;
				sprintf(retMessage, "��ҵ���ɾ����<����>%s, <�ʺ�>%s, <��ɫ��>%s, <���߱��>%d, <����λ��>%s", ServerIP, UserName, Character, itemIndex, "����");
				break;
			case 1:	//����
				req_delete_item.itemGroup = ADMIN::EG_BAG_ITEM;
				sprintf(retMessage, "��ҵ���ɾ����<����>%s, <�ʺ�>%s, <��ɫ��>%s, <���߱��>%d, <����λ��>%s", ServerIP, UserName, Character, itemIndex, "����");
				break;
			case 2:	//����
				req_delete_item.itemGroup = ADMIN::EG_BANK_ITEM;
				sprintf(retMessage, "��ҵ���ɾ����<����>%s, <�ʺ�>%s, <��ɫ��>%s, <���߱��>%d, <����λ��>%s", ServerIP, UserName, Character, itemIndex, "����");
				break;
			}

			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_DELETE_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			req_delete_item.itemIndex = itemIndex;
			//����
			req_delete_item.srv_realm_id=srv_Realmid;
			//��������ID
			req_delete_item.srv_cluster_id=srv_ClusterID;
			memcpy(&req_delete_item.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_delete_item, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "����ɾ��");				
			}
			else
			{
				memset(recvbuf, 0, 10240);
				
				int recvbyte = m_MySocket.MyRecv(recvbuf,10240);
				if ( recvbyte>0 && strcmp(recvbuf, "") )
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_delete_item, recvbuf+4, packetSize);
					switch (rep_delete_item.eResult)
					{
					case REP_DELETE_ITEM::E_SUCCESS:	//�ɹ�
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�ɹ�", "����ɾ��");
						break;
					case REP_DELETE_ITEM::E_NOT_FOUND:	//û�иõ���
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "û�иõ���", "����ɾ��");
						break;
					default:							//ʧ��
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "ʧ��", "����ɾ��");
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "����ɾ����ʱ", "����ɾ��");
				}
			}
			
			//m_MySocket.Close();
		}

		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "����ɾ��");
	}

	m_OperatorWA.GmOperLog(userByID, ServerIP, "����ɾ��", retMessage, DBTFLV[0]);
	return DBTFLV;
}
//�޸ĵȼ�
vector <CGlobalStruct::TFLV> CWAInfo::WA_Net_ModifyLevel(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character, int level)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	
	try
	{
		
		REQ_MODIFY_CHARACTER req_modify_character;				//�޸Ľ�Ǯ����
		REP_MODIFY_CHARACTER rep_modify_character;				//�޸Ľ�Ǯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼������ʧ��", "�޸���ҵȼ�");	
		}
		else
		{
			sprintf(retMessage, "�޸���ҽ�Ǯ����<����>%s, <�ʺ�>%s, <��ɫ��>%s, <�޸ĺ���ҵȼ�>%d", ServerIP, UserName, Character, level);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG
			
			req_packetLength = sizeof(REQ_MODIFY_CHARACTER);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			//����
			req_modify_character.srv_realm_id=srv_Realmid;
			//��������ID
			req_modify_character.srv_cluster_id=srv_ClusterID;
			req_modify_character.attr = ADMIN::MCA_LEVEL;
			req_modify_character.newValue = level;
			memcpy(&req_modify_character.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_modify_character, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "�޸���ҵȼ�");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				
				int recvbyte = m_MySocket.MyRecv(recvbuf,10240);
				if ( recvbyte>0 && strcmp(recvbuf, "") )
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_modify_character, recvbuf+4, packetSize);
					switch (rep_modify_character.eResult)
					{
					case REP_DELETE_ITEM::E_SUCCESS:	//�ɹ�
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�޸���ҵȼ��ɹ�", "�޸���ҵȼ�");	
						break;
					default:							//ʧ��
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�޸���ҵȼ�ʧ��", "�޸���ҵȼ�");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�޸���ҵȼ���ʱ", "�޸���ҵȼ�");
				}
			}
			
			m_MySocket.Close();
		}

		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "�޸���ҵȼ�");		
	}

	m_OperatorWA.GmOperLog(userByID, ServerIP, "�޸���ҵȼ�", retMessage, DBTFLV[0]);
	return DBTFLV;
}
//�޸Ľ�Ǯ
vector <CGlobalStruct::TFLV> CWAInfo::WA_Net_ModifyMoney(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character, int CurMoney, int newMoney)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);

	//Added by tengjie 2010-06-23
	CGlobalStruct::TFLV stTflv;
	vector <CGlobalStruct::TFLV>::iterator pTflv = NULL;

	try
	{
		//Added by tengjie 2010-06-23
		vector <CGlobalStruct::TFLV> TempDbTlv = WA_Net_KickPlayer(userByID, ServerIP, UserName, Character);

		REQ_MODIFY_CHARACTER req_modify_character;				//�޸Ľ�Ǯ����
		REP_MODIFY_CHARACTER rep_modify_character;				//�޸Ľ�Ǯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼������ʧ��", "�޸���ҽ�Ǯ");	
		}
		else
		{
			sprintf(retMessage, "�޸���ҽ�Ǯ����<����>%s, <�ʺ�>%s, <��ɫ��>%s, <�޸�ǰ��Ǯ����>%d, <�޸ĺ��Ǯ����>%d", ServerIP, UserName, Character, CurMoney, newMoney);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG
			
			req_packetLength = sizeof(REQ_MODIFY_CHARACTER);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			//����
			req_modify_character.srv_realm_id=srv_Realmid;
			//��������ID
			req_modify_character.srv_cluster_id=srv_ClusterID;
			req_modify_character.attr = ADMIN::MCA_MONEY;
			req_modify_character.newValue = newMoney;
			memcpy(&req_modify_character.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_modify_character, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "�޸���ҽ�Ǯ");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				
				int recvbyte = m_MySocket.MyRecv(recvbuf,10240);
				if ( recvbyte>0 && strcmp(recvbuf, "") )
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_modify_character, recvbuf+4, packetSize);
					switch (rep_modify_character.eResult)
					{
					case REP_DELETE_ITEM::E_SUCCESS:	//�ɹ�
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�޸Ľ�Ǯ�ɹ�", "�޸���ҽ�Ǯ");	
						break;
					default:							//ʧ��
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�޸Ľ�Ǯʧ��", "�޸���ҽ�Ǯ");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�޸Ľ�Ǯ��ʱ", "�޸���ҽ�Ǯ");
				}
			}
			
			m_MySocket.Close();
		}
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "�޸���ҽ�Ǯ");		
	}

	m_OperatorWA.GmOperLog(userByID, ServerIP, "�޸���ҽ�Ǯ", retMessage, DBTFLV[0]);
	return DBTFLV;
}
//�������
vector <CGlobalStruct::TFLV> CWAInfo::WA_BoardTasker_Add(int userByID, char * ServerIP, char *BoardMessage, char *BeginTime, char *EndTime, int interval)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	
	try
	{
		logFile.WriteLog("<��ʿ��˫>",operName,"�������");//��LOG
		
		printf("exec WA_BOARDTASK_INSERT %i,'%s','%s','%s','%s',%i", 
			userByID,ServerIP,BoardMessage,BeginTime, EndTime, interval);
		
		int tmpLength = 0;
		tmpLength = GameDBOper("WA", LocalIP, 4, "exec WA_BOARDTASK_INSERT %i,'%s','%s','%s','%s',%i",
			userByID,ServerIP,BoardMessage,BeginTime, EndTime, interval);
		
		if(tmpLength == 0)
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��ӹ���ʧ��", "�������");
		}
		else
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��ӹ���ɹ�", "�������");
		}
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��ӹ������", "�������");
	}
	
	return DBTFLV;
}

//�����޸�
vector <CGlobalStruct::TFLV> CWAInfo::WA_BoardTasker_Update(int userByID, char * ServerIP, int Taskid, char *BoardMessage, char *BeginTime, char *EndTime, int interval)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	
	try
	{
		logFile.WriteLog("<��ʿ��˫>",operName,"�������");//��LOG
		printf("exec WA_BOARDTASK_UPDATE %i, %i, '%s','%s','%s','%s',%i", 
			userByID,Taskid, ServerIP,BoardMessage,BeginTime, EndTime, interval);	
		int tmpLength = 0;
		tmpLength = GameDBOper("XDJG", LocalIP, 4, "exec WA_BOARDTASK_UPDATE %i, %i, '%s','%s','%s','%s',%i", 
			userByID,Taskid, ServerIP,BoardMessage,BeginTime, EndTime, interval);
		
		if(tmpLength == 0)
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "���¹���ʧ��", "�������");
		}
		else
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "���¹���ɹ�", "�������");
		}
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "���¹������", "�������");
	}
	
	return DBTFLV;
}
//����ɾ��	
vector <CGlobalStruct::TFLV> CWAInfo::WA_BoardTasker_Delete(int userByID, int Taskid)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{
		logFile.WriteLog("<��ʿ��˫>",operName,"�������");//��LOG
		
		int tmpLength = 0;
		tmpLength = GameDBOper("XDJG","127.0.0.1",4,"exec WA_BOARDTASK_DELETE %i, %i", 
			userByID,Taskid);
		if(tmpLength == 0)
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "ɾ������ʧ��", "�������");
		}
		else
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "ɾ������ɹ�", "�������");
		}
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "ɾ���������", "�������");
	}
	
	return DBTFLV;
}
//ģ�����߲�ѯ
vector<CGlobalStruct::TFLV> CWAInfo::WA_ITEM_QUERY(char* gameName,char* strSql,char* Item_Type,char* Item_Name,int index,int Page)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{		
		DBTFLV=GameDBQuery("WA","127.0.0.1",4,index,Page,"exec WA_Item_Query '%s','%s'",Item_Type,Item_Name);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("WA","ģ�����߲�ѯ�ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("WA","ģ�����߲�ѯ�ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("WA","ģ�����߲�ѯ�����쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("WA",(char *)err.what());
		return DBTFLV;
	}
	return DBTFLV;
}
//�������Ͳ�ѯ
vector<CGlobalStruct::TFLV> CWAInfo::WA_ITEMType_QUERY(char* gameName,char* strSql,int index,int Page)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{
		DBTFLV=GameDBQuery("WA","127.0.0.1",4,0,0,"exec WA_ItemType_Query");
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("WA","�������Ͳ�ѯ�ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("WA","�������Ͳ�ѯ�ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("WA","�������Ͳ�ѯ�����쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("WA",(char *)err.what());
		return DBTFLV;
	}
	return DBTFLV;
}
//���͵���
// vector <CGlobalStruct::TFLV> CWAInfo::WA_SendMailItem(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character, char *itemId,
// 		char *itemCount,char *itemName, int iMailTag, char *ItemGen, char *MailSub, char *MailContent, int iMoney/*, char *ItemExpire*/)
// {
// 	vector <CGlobalStruct::TFLV> DBTFLV;
// 	vector <string> tempVecChar;
// 	vector <string>::iterator ptempVecChar;
// 	vector <int> tempItemId;
// 	vector <int>::iterator ptempItemId;
// 	vector <int> tempItemCount;
// 	vector <int>::iterator ptempItemCount;
// 	vector <int> tempItemGen;
// 	vector <int>::iterator ptempItemGen;
// 	vector <int> tempItemExpire;
// 	vector <int>::iterator ptempItemExpire;
// 
// 	char retMessage[255];
// 	memset(retMessage, 0, 255);
// 
// 	try
// 	{
// 		REQ_MAIL_ITEM req_SendGift;		//������Ʒ����
// 		REP_MAIL_ITEM rep_SendGift;		//������Ʒ��Ӧ
// 		short int req_packetLength;
// 		short int flag = 24;
// 		char *sendbuf = new char[1024];
// 		char *recvbuf = new char[10240];
// 		char *pTemp = NULL;
// 		char *pCharacter = NULL;
// 		int iCount = 0;
// 		CString strLevel = "";
// 		
// 		if (!Connect_Remote_Login(ServerIP))
// 		{
// 			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼������ʧ��", "������ҵ���");	
// 		}
// 		else
// 		{
// 			sprintf(retMessage, "�����ҵ��ߣ�<����>%s, <�ʺ�>%s, <��ɫ��>%s, <����ID><%s>, <���͵�����><%s>,<���͵�������>%s", ServerIP, UserName, ptempVecChar->c_str(),itemId,itemName,itemCount);			
// 			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG
// 
// 			pCharacter = strtok(Character, "%");
// 			while(pCharacter != NULL)
// 			{
// 				tempVecChar.push_back(pCharacter);
// 				pCharacter = strtok(NULL, "%");
// 			}
// 
// 			pTemp = strtok(itemId, ";");
// 			while(pTemp != NULL)
// 			{
// 				tempItemId.push_back(atoi(pTemp));
// 				
// 				pTemp = strtok(NULL, ";");
// 			}
// 
// 			pTemp = strtok(itemCount, ";");
// 			while(pTemp != NULL)
// 			{
// 				tempItemCount.push_back(atoi(pTemp));
// 				
// 				pTemp = strtok(NULL, ";");
// 			}
// 
// 			pTemp = strtok(ItemGen, ";");
// 			while(pTemp != NULL)
// 			{
// 				tempItemGen.push_back(atoi(pTemp));
// 				
// 				pTemp = strtok(NULL, ";");
// 			}
// 
// 			for(ptempVecChar = tempVecChar.begin(); ptempVecChar != tempVecChar.end(); ptempVecChar++)
// 			{
// 				req_packetLength = sizeof(REQ_MAIL_ITEM);
// 				
// 				memset(sendbuf,0, 1024);
// 				memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
// 				memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
// 				
// 				//Added by tengjie 2010-06-24
// 				ZeroMemory(req_SendGift.item_type_id_list, sizeof(req_SendGift.item_type_id_list));
// 				ZeroMemory(req_SendGift.item_count_list, sizeof(req_SendGift.item_count_list));
// 				ZeroMemory(req_SendGift.item_gen_list, sizeof(req_SendGift.item_gen_list));
// 	//			ZeroMemory(req_SendGift.item_expire, sizeof(req_SendGift.item_expire));
// 
// 				sprintf(retMessage, "�����ҵ��ߣ�<����>%s, <�ʺ�>%s, <��ɫ��>%s, <����ID><%s>, <���͵�����><%s>,<���͵�������>%s", ServerIP, UserName, ptempVecChar->c_str(),itemId,itemName,itemCount);			
// 				logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG
// 
// 				for(ptempItemId = tempItemId.begin();ptempItemId != tempItemId.end();ptempItemId++)
// 				{
// 					req_SendGift.item_type_id_list[iCount] = (unsigned int)ptempItemId;
// 					iCount++;
// 				}
// 
// 				iCount = 0;
// 				for(ptempItemCount = tempItemCount.begin();ptempItemCount != tempItemCount.end();ptempItemCount++)
// 				{
// 					req_SendGift.item_count_list[iCount] = (unsigned int)ptempItemCount;
// 					iCount++;
// 				}
// 
// 				iCount = 0;
// 				for(ptempItemGen = tempItemGen.begin();ptempItemGen != tempItemGen.end();ptempItemGen++)
// 				{
// 					req_SendGift.item_count_list[iCount] = (unsigned int)ptempItemCount;
// 					if(0 == *ptempItemGen)
// 						req_SendGift.item_gen_list[iCount] = REQ_MAIL_ITEM::IG_WHITE;
// 					else if(1 == *ptempItemGen)
// 						req_SendGift.item_gen_list[iCount] = REQ_MAIL_ITEM::IG_BLUE;
// 					else if(2 == *ptempItemGen)
// 						req_SendGift.item_gen_list[iCount] = REQ_MAIL_ITEM::IG_PURPLE;
// 					else if(3 == *ptempItemGen)
// 						req_SendGift.item_gen_list[iCount] = REQ_MAIL_ITEM::IG_ORANGE;
// 					else
// 						req_SendGift.item_gen_list[iCount] = REQ_MAIL_ITEM::IG_WHITE;
// 
// 					iCount++;
// 				}
// 
// 	// 			iCount = 0;
// 	// 			pTemp = strtok(ItemExpire, ";");
// 	// 			while(pTemp != NULL)
// 	// 			{
// 	// 				req_SendGift.item_expire[iCount] = (unsigned int)atoi(pTemp);
// 	// 				
// 	// 				pTemp = strtok(NULL, ";");
// 	// 				
// 	// 				iCount++;
// 	// 			}
// 
// 				//End
// 
// 	//			req_SendGift.item_type_id=itemId;
// 	//			req_SendGift.item_count  =itemCount;
// 
// 				if(iMailTag == 0)
// 				{
// 					memcpy(&req_SendGift.char_name, ptempVecChar->c_str(), ptempVecChar->length()+1);
// 					req_SendGift.receiver_type = REQ_MAIL_ITEM::RT_SINGLE_PLAYER;
// 				}
// 				else if(iMailTag == 1)
// 				{
// 					memcpy(&req_SendGift.char_name, "", strlen("")+1);
// 					req_SendGift.receiver_type = REQ_MAIL_ITEM::RT_ALL_ONLINE_PLAYER;
// 				}
// 				else
// 				{
// 					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "������ҵ���ʧ��(�������ʹ���)", "������ҵ���");
// 					return DBTFLV;
// 				}
// 				
// 				memcpy(req_SendGift.subject, MailSub, strlen(MailSub)+1);
// 				memcpy(req_SendGift.body, MailContent, strlen(MailContent)+1);
// 				req_SendGift.money = iMoney;
// 
// 				//����
// 				req_SendGift.srv_realm_id=srv_Realmid;
// 				//��������ID
// 				req_SendGift.srv_cluster_id=srv_ClusterID;
// 				memcpy(sendbuf+4, &req_SendGift, req_packetLength);
// 				if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
// 				{
// 					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "������ҵ���");
// 				}
// 				else
// 				{
// 					memset(recvbuf, 0, 10240);
// 					int recvbyte = m_MySocket.MyRecv(recvbuf,10240);
// 					if ( recvbyte>0 && strcmp(recvbuf, "") )
// 					{
// 						short int packetSize = *(short int *)recvbuf;
// 						memcpy(&rep_SendGift, recvbuf+4, packetSize);
// 						switch (rep_SendGift.eResult)
// 						{
// 						case REP_MAIL_ITEM::E_SUCCESS:	//�ɹ�
// 							DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "������ҵ��߳ɹ�", "������ҵ���");	
// 							break;
// 						case REP_MAIL_ITEM::E_TOO_MUCH:	//Ⱥ���������
// 							DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "������ҵ���Ⱥ���������", "������ҵ���");	
// 							break;
// 						default:							//ʧ��
// 							DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "������ҵ���ʧ��", "������ҵ���");	
// 							break;
// 						}
// 					}
// 					else
// 					{
// 						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "������ҵ��߳�ʱ", "������ҵ���");
// 					}
// 				}
// 
// 				m_OperatorWA.GmOperLog(userByID, ServerIP, "������ҵ���", retMessage, DBTFLV[0]);
// 			}
// 			m_MySocket.Close();
// 		}
// 		delete []sendbuf;
// 		delete []recvbuf;
// 	}
// 	catch(...)
// 	{
// 		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "������ҵ���");
// 		m_OperatorWA.GmOperLog(userByID, ServerIP, "������ҵ���", retMessage, DBTFLV[0]);
// 	}
// 	
// //	m_OperatorWA.GmOperLog(userByID, ServerIP, "������ҵ���", retMessage, DBTFLV[0]);
// 	return DBTFLV;
// }

vector <CGlobalStruct::TFLV> CWAInfo::WA_SendMailItem(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character, char *itemId,
		char *itemCount,char *itemName, int iMailTag, char *ItemGen, char *MailSub, char *MailContent, int iMoney, char *ItemExpire)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	try
	{
		REQ_MAIL_ITEM req_SendGift;		//������Ʒ����
		REP_MAIL_ITEM rep_SendGift;		//������Ʒ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		char *pTemp = NULL;
		int iCount = 0;
		CString sztemp = "";
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼������ʧ��", "������ҵ���");	
		}
		else
		{
			req_packetLength = sizeof(REQ_MAIL_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//Added by tengjie 2010-06-24
			ZeroMemory(req_SendGift.item_type_id_list, sizeof(req_SendGift.item_type_id_list));
			ZeroMemory(req_SendGift.item_count_list, sizeof(req_SendGift.item_count_list));
			ZeroMemory(req_SendGift.item_gen_list, sizeof(req_SendGift.item_gen_list));
			ZeroMemory(req_SendGift.item_expire, sizeof(req_SendGift.item_expire));

			sprintf(retMessage, "�����ҵ��ߣ�<����>%s, <�ʺ�>%s, <��ɫ��>%s, <����ID><%s>, <���͵�����><%s>,<���͵�������>%s", ServerIP, UserName, Character,itemId,itemName,itemCount);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			pTemp = strtok(itemId, ";");
			while(pTemp != NULL)
			{
				sztemp = pTemp;
				sztemp.TrimLeft();
				sztemp.TrimRight();

				req_SendGift.item_type_id_list[iCount] = (unsigned int)atoi(sztemp.GetBuffer(0));

				pTemp = strtok(NULL, ";");

				iCount++;
			}

			iCount = 0;
			pTemp = strtok(itemCount, ";");
			while(pTemp != NULL)
			{
				sztemp = pTemp;
				sztemp.TrimLeft();
				sztemp.TrimRight();

				req_SendGift.item_count_list[iCount] = (unsigned short)atoi(sztemp.GetBuffer(0));

				pTemp = strtok(NULL, ";");

				iCount++;
			}

			iCount = 0;
			pTemp = strtok(ItemGen, ";");
			while(pTemp != NULL)
			{
				sztemp = pTemp;
				sztemp.TrimLeft();
				sztemp.TrimRight();

				if(0 == atoi(sztemp.GetBuffer(0)))
					req_SendGift.item_gen_list[iCount] = REQ_MAIL_ITEM::IG_WHITE;
				else if(1 == atoi(sztemp.GetBuffer(0)))
					req_SendGift.item_gen_list[iCount] = REQ_MAIL_ITEM::IG_BLUE;
				else if(2 == atoi(sztemp.GetBuffer(0)))
					req_SendGift.item_gen_list[iCount] = REQ_MAIL_ITEM::IG_PURPLE;
				else if(3 == atoi(sztemp.GetBuffer(0)))
					req_SendGift.item_gen_list[iCount] = REQ_MAIL_ITEM::IG_ORANGE;
				else
					req_SendGift.item_gen_list[iCount] = REQ_MAIL_ITEM::IG_WHITE;
				
				pTemp = strtok(NULL, ";");
				
				iCount++;
			}

			iCount = 0;
			pTemp = strtok(ItemExpire, ";");
			while(pTemp != NULL)
			{
				req_SendGift.item_expire[iCount] = (unsigned int)(atoi(pTemp)*24*3600);
				
				pTemp = strtok(NULL, ";");
				
				iCount++;
			}

			//End

//			req_SendGift.item_type_id=itemId;
//			req_SendGift.item_count  =itemCount;

			if(iMailTag == 0)
			{
				memcpy(&req_SendGift.char_name, Character, strlen(Character)+1);
				req_SendGift.receiver_type = REQ_MAIL_ITEM::RT_SINGLE_PLAYER;
			}
			else if(iMailTag == 1)
			{
				memcpy(&req_SendGift.char_name, "", strlen("")+1);
				req_SendGift.receiver_type = REQ_MAIL_ITEM::RT_ALL_ONLINE_PLAYER;
			}
			else
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "������ҵ���ʧ��(�������ʹ���)", "������ҵ���");
				return DBTFLV;
			}
			
			memcpy(req_SendGift.subject, MailSub, strlen(MailSub)+1);
			memcpy(req_SendGift.body, MailContent, strlen(MailContent)+1);
			req_SendGift.money = iMoney;

			//����
			req_SendGift.srv_realm_id=srv_Realmid;
			//��������ID
			req_SendGift.srv_cluster_id=srv_ClusterID;
			memcpy(sendbuf+4, &req_SendGift, req_packetLength);
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "������ҵ���");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				int recvbyte = m_MySocket.MyRecv(recvbuf,10240);
				if ( recvbyte>0 && strcmp(recvbuf, "") )
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_SendGift, recvbuf+4, packetSize);
					switch (rep_SendGift.eResult)
					{
					case REP_MAIL_ITEM::E_SUCCESS:	//�ɹ�
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "������ҵ��߳ɹ�", "������ҵ���");	
						break;
					case REP_MAIL_ITEM::E_TOO_MUCH:	//Ⱥ���������
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "������ҵ���Ⱥ���������", "������ҵ���");	
						break;
					default:							//ʧ��
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "������ҵ���ʧ��", "������ҵ���");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "������ҵ��߳�ʱ", "������ҵ���");
				}
			}
			m_MySocket.Close();
		}
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "������ҵ���");		
	}
	
	m_OperatorWA.GmOperLog(userByID, ServerIP, "������ҵ���", retMessage, DBTFLV[0]);
	return DBTFLV;
}


// vector <CGlobalStruct::TFLV> CWAInfo::WA_SendMailItem(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character, int itemId,int itemCount,char *itemName)
// {
// 	vector <CGlobalStruct::TFLV> DBTFLV;
// 	char retMessage[255];
// 	memset(retMessage, 0, 255);
// 	try
// 	{
// 		REQ_MAIL_ITEM req_SendGift;		//������Ʒ����
// 		REP_MAIL_ITEM rep_SendGift;		//������Ʒ��Ӧ
// 		short int req_packetLength;
// 		short int flag = 24;
// 		char *sendbuf = new char[1024];
// 		char *recvbuf = new char[10240];
// 		
// 		if (!Connect_Remote_Login(ServerIP))
// 		{
// 			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼������ʧ��", "������ҵ���");	
// 		}
// 		else
// 		{
// 			sprintf(retMessage, "�����ҵ��ߣ�<����>%s, <�ʺ�>%s, <��ɫ��>%s, <������ҵ�����>%s,<�����������>%d", ServerIP, UserName, Character,itemName,itemId);			
// 			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG
// 			req_packetLength = sizeof(REQ_MAIL_ITEM);
// 			
// 			memset(sendbuf,0, 1024);
// 			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
// 			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
// 			memcpy(&req_SendGift.char_name, Character, strlen(Character)+1);
// 			//����
// 			req_SendGift.srv_realm_id=srv_Realmid;
// 			//��������ID
// 			req_SendGift.srv_cluster_id=srv_ClusterID;
// 			req_SendGift.item_type_id=itemId;
// 			req_SendGift.item_count  =itemCount;
// 			memcpy(sendbuf+4, &req_SendGift, req_packetLength);
// 			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
// 			{
// 				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "������ҵ���");
// 			}
// 			else
// 			{
// 				memset(recvbuf, 0, 10240);
// 				int recvbyte = m_MySocket.MyRecv(recvbuf,10240);
// 				if ( recvbyte>0 )
// 				{
// 					short int packetSize = *(short int *)recvbuf;
// 					memcpy(&rep_SendGift, recvbuf+4, packetSize);
// 					switch (rep_SendGift.eResult)
// 					{
// 					case REP_MAIL_ITEM::E_SUCCESS:	//�ɹ�
// 						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "������ҵ��߳ɹ�", "������ҵ���");	
// 						break;
// 					default:							//ʧ��
// 						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "������ҵ���ʧ��", "������ҵ���");	
// 						break;
// 					}
// 				}
// 				else
// 				{
// 					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "������ҵ��߳�ʱ", "������ҵ��߽�Ǯ");
// 				}
// 			}
// 			m_MySocket.Close();
// 		}
// 		delete []sendbuf;
// 		delete []recvbuf;
// 	}
// 	catch(...)
// 	{
// 		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "������ҵ���");		
// 	}
// 	
// 	m_OperatorWA.GmOperLog(userByID, ServerIP, "������ҵ���", retMessage, DBTFLV[0]);
// 	return DBTFLV;
// }

//����ģ����ѯ
vector<CGlobalStruct::TFLV> CWAInfo::WA_SkillList_QUERY(char* gameName,char* strSql,char* Skill_Name,int index,int Page)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{		
		DBTFLV=GameDBQuery("WA","127.0.0.1",4,index,Page,"exec WA_SkillList_Query '%s'",Skill_Name);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("WA","ģ�����ܲ�ѯ�ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("WA","ģ�����ܲ�ѯ�ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("WA","ģ�����ܲ�ѯ�����쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("WA",(char *)err.what());
		return DBTFLV;
	}
	return DBTFLV;
}
// ��ѯ��ҽ�ɫ���ϼ�����Ϣ
vector <CGlobalStruct::TFLV> CWAInfo::WA_RoleSkill_Query(char *ServerIP, char *GameRegion, char *Account,char *Character,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <CHARACTER_SKILL_INFO_ITEM> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_CHARACTER_INFO req_Log_info;				//������Ϣ��ѯ����
		REP_CHARACTER_SKILL_INFO rep_Log_info;			//������Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "���ϼ�����Ϣ��ѯ");
		}
		else
		{
			sprintf(retMessage, "���ϼ�����Ϣ��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s", ServerIP, Account, Character);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_CHARACTER_INFO);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			//����
			req_Log_info.srv_realm_id=srv_Realmid;
			//��������ID
			req_Log_info.srv_cluster_id=srv_ClusterID;
			req_Log_info.req_type = REQ_CHARACTER_INFO::RT_CHAR_SKILL_INFO;
			req_Log_info.query_condition = ADMIN::QC_CHARACTERNAME;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
				if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "������ϼ��ܲ�ѯ");
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
 					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&rep_Log_info, recvbuf+offset+4, packetSize);
								//��������
								if (rep_Log_info.allSend == 1)	//�յ����һ���ṹ��
								{
									bRecvFlag = false;

									if (rep_Log_info.amount>0)
									{
										for (int i=0; i<rep_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(rep_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (rep_Log_info.amount>0)
									{
										for (int i=0; i<rep_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(rep_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iFriendInfoLen = vect_LogInfo.size();

				if ( iFriendInfoLen%iPageSize == 0 )
				{
					iPageCount = iFriendInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iFriendInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iFriendInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ����ID
					_itoa((int)vect_LogInfo[i].skillId, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SkillID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SkillName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].skillName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].skillName);
					DBTFLV.push_back(m_tflv);

					//���ܵȼ�
					_itoa((int)vect_LogInfo[i].skillLevel, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SkillLvl;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);										
				}

			}

			m_MySocket.Close();
		}

		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "���ϼ�����Ϣ��ѯ");
	}

	return DBTFLV;
}
//�޸���Ҽ���
vector <CGlobalStruct::TFLV> CWAInfo::WA_ModfiyPlayerSkill(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character, int skillID,char *skillname,int skilllvl)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	try
	{
		
		REQ_MODIFY_CHARACTER_SKILL req_modifySkill;		//�޸ļ��ܵȼ�����
		REP_MODIFY_CHARACTER_SKILL rep_modifySkill;		//�޸ļ��ܵȼ��Ļ�Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼������ʧ��", "�޸���Ҽ��ܵȼ�");	
		}
		else
		{
			sprintf(retMessage, "�޸���Ҽ��ܣ�<����>%s, <�ʺ�>%s, <��ɫ��>%s, <�޸���Ҽ�����>%s,<�޸���Ҽ��ܵȼ�>%d", ServerIP, UserName, Character,skillname,skilllvl);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG
			
			req_packetLength = sizeof(REQ_MODIFY_CHARACTER_SKILL);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			memcpy(&req_modifySkill.char_name, Character, strlen(Character)+1);
			//����
			req_modifySkill.srv_realm_id=srv_Realmid;
			//��������ID
			req_modifySkill.srv_cluster_id=srv_ClusterID;
			req_modifySkill.skill_id=skillID;
			req_modifySkill.skill_level =skilllvl;
			memcpy(sendbuf+4, &req_modifySkill, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "�޸���Ҽ��ܵȼ�");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				int recvbyte = m_MySocket.MyRecv(recvbuf,10240);
				if ( recvbyte>0 && strcmp(recvbuf, "") )
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_modifySkill, recvbuf+4, packetSize);
					switch (rep_modifySkill.eResult)
					{
					case REP_MODIFY_CHARACTER_SKILL::E_SUCCESS:	//�ɹ�
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�޸���Ҽ��ܵȼ��ɹ�", "�޸���Ҽ���");	
						break;
					default:							//ʧ��
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�޸���Ҽ��ܵȼ�ʧ��", "�޸���Ҽ���");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�޸���Ҽ��ܵȼ���ʱ", "�޸���Ҽ��ܵȼ�");
				}
			}
			
		//	m_MySocket.Close();
		}

		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "�޸���Ҽ��ܵȼ�");		
	}

	m_OperatorWA.GmOperLog(userByID, ServerIP, "�޸���Ҽ��ܵȼ�", retMessage, DBTFLV[0]);
	m_MySocket.Close();
	return DBTFLV;
}

//�ָ���ɫ���
vector <CGlobalStruct::TFLV> CWAInfo::WA_RestoreRoleInfo(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	try
	{
		
		REQ_RECOVER_CHARACTER req_RecoverCharacter;		//�ָ���ɫ����
		REP_RECOVER_CHARACTER rep_RecoverCharacter;		//�ָ���ɫ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼������ʧ��", "�ָ���ҽ�ɫ");	
		}
		else
		{
			sprintf(retMessage, "�ָ���ң�<����>%s, <�ʺ�>%s, <��ɫ��>%s", ServerIP, UserName, Character);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG
			
			req_packetLength = sizeof(REQ_RECOVER_CHARACTER);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			memcpy(&req_RecoverCharacter.char_name, Character, strlen(Character)+1);
			//����
			req_RecoverCharacter.srv_realm_id=srv_Realmid;
			//��������ID
			req_RecoverCharacter.srv_cluster_id=srv_ClusterID;
			memcpy(sendbuf+4, &req_RecoverCharacter, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "�ָ���ҽ�ɫ");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				
				int recvbyte = m_MySocket.MyRecv(recvbuf,10240);
				if ( recvbyte>0 && strcmp(recvbuf, "") )
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_RecoverCharacter, recvbuf+4, packetSize);
					switch (rep_RecoverCharacter.eResult)
					{
					case REP_RECOVER_CHARACTER::E_SUCCESS:	//�ɹ�
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�ָ���ҽ�ɫ�ɹ�", "�ָ���ҽ�ɫ");	
						break;
					default:						//ʧ��
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�ָ���ҽ�ɫʧ��", "�ָ���ҽ�ɫ");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�ָ���ҽ�ɫ��ʱ", "�ָ���ҽ�ɫ");
				}
			}
			
			m_MySocket.Close();
		}

		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "�ָ���ҽ�ɫ");		
	}

	m_OperatorWA.GmOperLog(userByID, ServerIP, "�ָ���ҽ�ɫ", retMessage, DBTFLV[0]);
	return DBTFLV;
}
//��������ѯ
vector<CGlobalStruct::TFLV> CWAInfo::WA_UserPwd_QUERY(char* gameName,char* ServerIP,char* serverName,char* account,int index,int Page)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{
		DBTFLV=GameDBQuery("WA","127.0.0.1",4,0,0,"exec WA_UserPwd_Query %s",serverName,account);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("WA","�������Ͳ�ѯ�ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("WA","�������Ͳ�ѯ�ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("WA","�������Ͳ�ѯ�����쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("WA",(char *)err.what());
		return DBTFLV;
	}
	return DBTFLV;
}
//�޸���ҵ�����
vector <CGlobalStruct::TFLV> CWAInfo::WA_ChangeLoginPasswd(int userByID,char * ServerIP, char * UserName,int CharacterID,char * newPwd)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	try
	{
		
		REQ_MODIFY_ACCOUNT_PASSW req_ChangePwd;		//�޸������������
		REP_MODIFY_ACCOUNT_PASSW rep_ChangePwd;		//�޸���������Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		CString szTime = "";
		CString szSql = "";
		CSqlHelper m_SqlHelper;
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼������ʧ��", "�޸��������");	
		}
		else
		{
			sprintf(retMessage, "�޸���ң�<����>%s, <�ʺ�>%s, <������>%s", ServerIP, UserName, newPwd);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG
			
			req_packetLength = sizeof(REQ_MODIFY_ACCOUNT_PASSW);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			memcpy(&req_ChangePwd.user_name, UserName, strlen(UserName)+1);
			memcpy(&req_ChangePwd.new_password, newPwd, strlen(newPwd)+1);
			//����
			req_ChangePwd.srv_realm_id=srv_Realmid;
			memcpy(sendbuf+4, &req_ChangePwd, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "�޸��������");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				
				int recvbyte = m_MySocket.MyRecv(recvbuf,10240);
				if ( recvbyte>0 && strcmp(recvbuf, "") )
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_ChangePwd, recvbuf+4, packetSize);
					switch (rep_ChangePwd.eResult)
					{
					case REP_MODIFY_ACCOUNT_PASSW::E_SUCCESS:	//�ɹ�
						SYSTEMTIME nowtime;
						GetLocalTime(&nowtime);
						szTime.Format("%4d-%2d-%2d %2d:%2d:%2d",nowtime.wYear,nowtime.wMonth,nowtime.wDay,nowtime.wHour,nowtime.wMinute,nowtime.wSecond);
						szSql.Format("exec WA_SAVE_OLDPASSWORD %i, '%s', '%s', '%s', '%s', '%s', %i", 
							userByID,ServerIP,UserName,rep_ChangePwd.old_password,newPwd,szTime.GetBuffer(0),0);
						m_SqlHelper.SqlHelperMain("WA","127.0.0.1", szSql.GetBuffer(0), 4);
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�޸��������ɹ�", "�޸��������");
						break;
					default:						//ʧ��
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�޸��������ʧ��", "�޸��������");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�޸�������볬ʱ", "�޸��������");
				}
			}
			
			m_MySocket.Close();
		}

		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "�޸��������");		
	}

	m_OperatorWA.InsertLocalDB(userByID,ServerIP,ServerIP,UserName,newPwd,newPwd);
	m_OperatorWA.GmOperLog(userByID, ServerIP, "�޸��������", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//Added by tengjie 2010-06-30
//�ָ��������
vector <CGlobalStruct::TFLV> CWAInfo::WA_ResumePassw(int userByID, char *ServerIP, char *Account)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	string strData = "";
	try
	{
		
		REQ_RECOVER_PASSW req_ResumePwd;		//�ָ������������
		REP_RECOVER_PASSW rep_ResumePwd;		//�ָ���������Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼������ʧ��", "�ָ��������");	
		}
		else
		{
			sprintf(retMessage, "�ָ��������:<����>%s, <�ʺ�>%s", ServerIP, Account);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG
			
			req_packetLength = sizeof(REQ_RECOVER_PASSW);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�

			strData = WA_GetOldPassWord(ServerIP, Account);

			memcpy(&req_ResumePwd.user_name, Account, strlen(Account)+1);
			ZeroMemory(req_ResumePwd.old_password, sizeof(req_ResumePwd.old_password));
			memcpy(&req_ResumePwd.old_password, strData.c_str(), strData.length()+1);

			//����
			req_ResumePwd.srv_realm_id=srv_Realmid;
			memcpy(sendbuf+4, &req_ResumePwd, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "�ָ��������");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				
				int recvbyte = m_MySocket.MyRecv(recvbuf,10240);
				if ( recvbyte>0 && strcmp(recvbuf, "") )
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_ResumePwd, recvbuf+4, packetSize);
					switch (rep_ResumePwd.eResult)
					{
					case REP_MODIFY_ACCOUNT_PASSW::E_SUCCESS:	//�ɹ�
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�ָ��������ɹ�", "�ָ��������");
						break;
					default:						//ʧ��
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�ָ��������ʧ��", "�ָ��������");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�ָ�������볬ʱ", "�ָ��������");
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "�ָ��������");		
	}
	
	m_OperatorWA.InsertLocalDB(userByID,ServerIP,ServerIP,Account,(char*)strData.c_str(),(char*)strData.c_str());
	m_OperatorWA.GmOperLog(userByID, ServerIP, "�ָ��������", retMessage, DBTFLV[0]);
	return DBTFLV;
}


string CWAInfo::WA_GetOldPassWord(char *ServerIP, char *Account)
{
	if(Account == NULL || ServerIP == NULL)
		return "";
	
	CString szSql = "";
	string szData = "";
	char connLocalDB[20];
	CSqlHelper m_SqlHelper;
	vector<CGlobalStruct::DataRecord> datarecord;
	
	
	szSql.Format("exec WA_SAVE_OLDPASSWORD %i, '%s', '%s', '%s', '%s', '%s', %i", 
					0,ServerIP,Account,"","","",1);
	
	m_SqlHelper.SqlHelperMain("WA", &datarecord,connLocalDB,szSql.GetBuffer(0),4,0,0);
	
	if(datarecord.empty())
		return "";
	
	vector<CGlobalStruct::DataRecord>::iterator iter;
	CGlobalStruct::DataRecord  m_DataRecord;
	for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
	{
		m_DataRecord = *iter;
		if(!strcmp("OldPwd", m_DataRecord.recordfieldname))//��ȡ��Ӧ��ServerIP
		{
			szData = m_DataRecord.recorddata;//�����ѯ�õ������һ����¼
		}
	}
	
	return szData;
	
}
//End

//�޸���ҵĽ�ɫ��
vector <CGlobalStruct::TFLV> CWAInfo::WA_RoleName_Modfiy(int userByID,char * ServerIP, char * UserName,char* Character,char * newRole)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	try
	{
		
		REQ_MODIFY_CHARACTER_NAME req_ChangeRole;		//�޸���ҽ�ɫ��
		REP_MODIFY_CHARACTER_NAME rep_ChangeRole;		//�޸���ҽ�ɫ��
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼������ʧ��", "�޸���ҽ�ɫ��");	
		}
		else
		{
			sprintf(retMessage, "�޸���ң�<����>%s, <�ʺ�>%s, <ԭ��ɫ��>%s,<��ɫ��>%s", ServerIP, UserName, Character,newRole);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG
			
			req_packetLength = sizeof(REQ_MODIFY_CHARACTER_NAME);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			memcpy(&req_ChangeRole.char_name, Character, strlen(Character)+1);
			memcpy(&req_ChangeRole.new_char_name, newRole, strlen(newRole)+1);
			//����
			req_ChangeRole.srv_realm_id=srv_Realmid;
			req_ChangeRole.srv_cluster_id=srv_ClusterID;
			memcpy(sendbuf+4, &req_ChangeRole, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "�޸���ҽ�ɫ��");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				
				int recvbyte = m_MySocket.MyRecv(recvbuf,10240);
				if ( recvbyte>0 && strcmp(recvbuf, "") )
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_ChangeRole, recvbuf+4, packetSize);
					switch (rep_ChangeRole.eResult)
					{
					case REP_MODIFY_CHARACTER_NAME::E_SUCCESS:	//�ɹ�
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�޸���ҽ�ɫ���ɹ�", "�޸���ҽ�ɫ��");	
						break;
					default:						//ʧ��
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�޸���ҽ�ɫ��ʧ��", "�޸���ҽ�ɫ��");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�޸���ҽ�ɫ����ʱ", "�޸���ҽ�ɫ��");
				}
			}
			
			m_MySocket.Close();
		}

		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "�޸���ҽ�ɫ��");		
	}

	m_OperatorWA.GmOperLog(userByID, ServerIP, "�޸���ҽ�ɫ��", retMessage, DBTFLV[0]);
	return DBTFLV;
}
//=====================================================================log========================================================================================

// ��Ʒ:�������
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_MonsterDropItem(char *ServerIP, char *GameRegion, char *Account,char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_MONSTER_DROP_ITEM_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_MONSTER_DROP_ITEM req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_MONSTER_DROP_ITEM res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
// 		char sendbuf[1024];
// 		char recvbuf[10240];

		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];

		DBTFLV.clear();
		vect_LogInfo.clear();

		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "���������Ʒ��Ϣ��ѯ");
		}
		else
		{
			sprintf(retMessage, "���������Ʒ��Ϣ��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_MONSTER_DROP_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;

			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);


			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "���������Ʒ��Ϣ��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;

					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0)
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����

						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft < packetSize + 4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				printf("***************%d\n",vect_LogInfo.size());
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ����ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);					
					
					// ��Ʒ����ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemTypeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��Ʒ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// ��ƷΨһID
					_ui64toa(vect_LogInfo[i].item_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemUniqueId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ����
					_itoa((int)vect_LogInfo[i].item_count, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemCount;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex= DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBTFLV.push_back(m_tflv);
				}
			}
			
			m_MySocket.Close();
		}

		delete []sendbuf;
		delete []recvbuf;
		
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "���������Ʒ��Ϣ��ѯ");
	}
	
	return DBTFLV;	
}

//��NPC����ĵ���
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_BuyItemNPC(char *ServerIP, char *GameRegion, char *Account, char *Character,char *Begintime,char *Endtime,int nickID, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_BUY_ITEM_FROM_NPC_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_BUY_ITEM_FROM_NPC req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_BUY_ITEM_FROM_NPC res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "��NPC����ĵ�����Ϣ��ѯ");
		}
		else
		{
			sprintf(retMessage, "��NPC����ĵ�����Ϣ��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_BUY_ITEM_FROM_NPC);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "��NPC����ĵ�����Ϣ��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ���ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);					
					
					// ��Ʒ����ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemTypeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��Ʒ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// ��ƷΨһID
					_ui64toa(vect_LogInfo[i].item_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemUniqueId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ����
					_itoa((int)vect_LogInfo[i].item_count, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemCount;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ������Ϸ������
					_itoa((int)vect_LogInfo[i].cost_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex= DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBTFLV.push_back(m_tflv);
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "��NPC����ĵ�����Ϣ��ѯ");
	}
	
	return DBTFLV;	
}

//��Ʒ:����NPC
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_SellItemTONPC(char *ServerIP, char *GameRegion,char *Account, char *Character,  char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_SELL_ITEM_TO_NPC_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_SELL_ITEM_TO_NPC req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_SELL_ITEM_TO_NPC res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "����NPC��Ʒ��Ϣ��ѯ");
		}
		else
		{
			sprintf(retMessage, "����NPC��Ʒ��Ϣ��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_SELL_ITEM_TO_NPC);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);


			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "����NPC��Ʒ��Ϣ��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ����ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);					
					
					// ��Ʒ����ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemTypeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��Ʒ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// ��ƷΨһID
					_ui64toa(vect_LogInfo[i].item_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemUniqueId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ����
					_itoa((int)vect_LogInfo[i].item_count, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemCount;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ������Ϸ������
					_itoa((int)vect_LogInfo[i].getMoney, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex= DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBTFLV.push_back(m_tflv);
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "����NPC��Ʒ��Ϣ��ѯ");
	}
	
	return DBTFLV;
}

//�������ɾ������
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_DropItem(char *ServerIP, char *GameRegion,  char *Account, char *Character,char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_DROP_ITEM_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_DROP_ITEM req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_DROP_ITEM res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "�������ɾ�����߲�ѯ");
		}
		else
		{
			sprintf(retMessage, "�������ɾ�����߲�ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_DROP_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "�������ɾ�����߲�ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ɾ��ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);					
					
					// ��Ʒ����ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemTypeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��Ʒ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// ��ƷΨһID
					_ui64toa(vect_LogInfo[i].item_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemUniqueId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ����
					_itoa((int)vect_LogInfo[i].item_count, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemCount;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex= DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBTFLV.push_back(m_tflv);
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "�������ɾ�����߲�ѯ");
	}
	
	return DBTFLV;
}

// ��Ʒ:���׻��
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_GetItemTrade(char *ServerIP, char *GameRegion, char *Account, char *Character,char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_GET_ITEM_TRADE_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_GET_ITEM_TRADE req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_GET_ITEM_TRADE res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "���׻����Ʒ��ѯ");
		}
		else
		{
			sprintf(retMessage, "���׻����Ʒ��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_GET_ITEM_TRADE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "���׻����Ʒ��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;

					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ��Դ��ɫID
					_itoa((int)vect_LogInfo[i].src_char_id.charIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcCharId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ��Դ��ɫ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcCharName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].src_char_name.charName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].src_char_name.charName);
					DBTFLV.push_back(m_tflv);

					// ����ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);					

					// ��Ʒ����ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemTypeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��Ʒ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);

					// ��ƷΨһID
					_ui64toa(vect_LogInfo[i].item_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemUniqueId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ����
					_itoa((int)vect_LogInfo[i].item_count, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemCount;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex= DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBTFLV.push_back(m_tflv);
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "���׻����Ʒ��ѯ");
	}
	
	return DBTFLV;	
}

// ��Ʒ:����ʧȥ
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_LoseItemTrade(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_LOSE_ITEM_TRADE_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_LOSE_ITEM_TRADE req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_LOSE_ITEM_TRADE res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "����ʧȥ��Ʒ��ѯ");
		}
		else
		{
			sprintf(retMessage, "����ʧȥ��Ʒ��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);	
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_LOSE_ITEM_TRADE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "����ʧȥ��Ʒ��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ��ƷĿ���ɫID
					_itoa((int)vect_LogInfo[i].dst_char_id.charIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_DstCharId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��ƷĿ���ɫ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_DstCharName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].dst_char_name.charName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].dst_char_name.charName);
					DBTFLV.push_back(m_tflv);
					
					// ����ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);					
					
					// ��Ʒ����ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemTypeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��Ʒ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// ��ƷΨһID
					_ui64toa(vect_LogInfo[i].item_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemUniqueId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ����
					_itoa((int)vect_LogInfo[i].item_count, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemCount;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex= DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBTFLV.push_back(m_tflv);

				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "����ʧȥ��Ʒ��ѯ");
	}
	
	return DBTFLV;	
}

// ��Ʒ:װ���󶨼�¼
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_BindItem(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_BIND_ITEM_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_BIND_ITEM req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_BIND_ITEM res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "װ���󶨼�¼��ѯ");
		}
		else
		{
			sprintf(retMessage, "װ���󶨼�¼��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_BIND_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "װ���󶨼�¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{

					// ��ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);

					// ��Ʒ����ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemTypeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��Ʒ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// ��ƷΨһID
					_ui64toa(vect_LogInfo[i].item_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemUniqueId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					if (vect_LogInfo[i].bind_reward[0] > ADMIN::MCA_MAX)
					{
						m_tflv.nIndex = DBTFLV.size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_BindRewardValue1;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen("")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"");
						DBTFLV.push_back(m_tflv);						
					}
					else
					{
						m_tflv.nIndex = DBTFLV.size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_BindRewardValue1;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
						_itoa((int)vect_LogInfo[i].bind_reward_value[0], strInt, 10);
						m_tflv.m_tvlength = strlen(strInt)+1;
						memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
						DBTFLV.push_back(m_tflv);						
					}

					if (vect_LogInfo[i].bind_reward[1] > ADMIN::MCA_MAX)
					{
						m_tflv.nIndex = DBTFLV.size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_BindRewardValue2;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen("")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"");
						DBTFLV.push_back(m_tflv);						
					}
					else
					{
						m_tflv.nIndex = DBTFLV.size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_BindRewardValue2;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
						_itoa((int)vect_LogInfo[i].bind_reward_value[1], strInt, 10);
						m_tflv.m_tvlength = strlen(strInt)+1;
						memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
						DBTFLV.push_back(m_tflv);						
					}
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex= DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBTFLV.push_back(m_tflv);
					
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "װ���󶨼�¼��ѯ");
	}
	
	return DBTFLV;	
}

// ��Ʒ:װ����󶨼�¼
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_UnBindItem(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_UNBIND_ITEM_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_UNBIND_ITEM req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_UNBIND_ITEM res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "װ����󶨼�¼��ѯ");
		}
		else
		{
			sprintf(retMessage, "װ����󶨼�¼��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_UNBIND_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "װ����󶨼�¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}
				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ���ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// ��Ʒ����ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ItemID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��Ʒ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// ��ƷΨһID
					_ui64toa(vect_LogInfo[i].item_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemUniqueId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					if (vect_LogInfo[i].bind_reward[0] > ADMIN::MCA_MAX)
					{
						m_tflv.nIndex = DBTFLV.size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_UNBindRewardValue1;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen("")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"");
						DBTFLV.push_back(m_tflv);						
					}
					else
					{
						m_tflv.nIndex = DBTFLV.size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_UNBindRewardValue1;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
						_itoa((int)vect_LogInfo[i].bind_reward_value[0], strInt, 10);
						m_tflv.m_tvlength = strlen(strInt)+1;
						memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
						DBTFLV.push_back(m_tflv);						
					}
					
					if (vect_LogInfo[i].bind_reward[1] > ADMIN::MCA_MAX)
					{
						m_tflv.nIndex = DBTFLV.size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_UNBindRewardValue2;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen("")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"");
						DBTFLV.push_back(m_tflv);						
					}
					else
					{
						m_tflv.nIndex = DBTFLV.size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_UNBindRewardValue2;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
						_itoa((int)vect_LogInfo[i].bind_reward_value[1], strInt, 10);
						m_tflv.m_tvlength = strlen(strInt)+1;
						memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
						DBTFLV.push_back(m_tflv);						
					}

					// ���������ƷID
					_itoa((int)vect_LogInfo[i].consume_item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeItemTypeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex= DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBTFLV.push_back(m_tflv);

				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "װ����󶨼�¼��ѯ");
	}
	
	return DBTFLV;	
}

// ��Ʒ:��ɫ����̯����־��ѯ
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_BoothItem(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime,int nickID, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_BOOTH_ITEM_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_BOOTH_ITEM req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_BOOTH_ITEM res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "��ɫ��̯��־��ѯ");
		}
		else
		{
			sprintf(retMessage, "��ɫ��̯��־��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_BOOTH_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "��ɫ��̯��־��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ��������
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::WA_BoothType;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					switch (vect_LogInfo[i].booth_type)
					{
					case REP_LOG_BOOTH_ITEM_ELE::BOOTH_OPEN:
					{
						m_tflv.m_tvlength = strlen("����")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"����");
						DBTFLV.push_back(m_tflv);

						// ��Ʒ���
						m_tflv.nIndex = DBTFLV.size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_DstCharName;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen("");
						sprintf((char *)&m_tflv.lpdata, "%s", "");
						DBTFLV.push_back(m_tflv);
						// ��Ʒ����
						m_tflv.nIndex = DBTFLV.size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen("");
						sprintf((char *)&m_tflv.lpdata, "%s", "");
						DBTFLV.push_back(m_tflv);
						// ��Ʒ���߼۸�
						_itoa(0, strInt, 10);
						m_tflv.nIndex = DBTFLV.size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_ItemPrice;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
						DBTFLV.push_back(m_tflv);
						break;
					}
					case REP_LOG_BOOTH_ITEM_ELE::BOOTH_SELL:
					{
						m_tflv.m_tvlength = strlen("̯������")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"̯������");	
						DBTFLV.push_back(m_tflv);

						// ��Ʒ���
						m_tflv.nIndex = DBTFLV.size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_DstCharName;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(vect_LogInfo[i].booth_sell.dst_char_name.charName);
						sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].booth_sell.dst_char_name.charName);
						DBTFLV.push_back(m_tflv);
						// ��Ʒ����
						m_tflv.nIndex = DBTFLV.size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(vect_LogInfo[i].booth_sell.item_name);
						sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].booth_sell.item_name);
						DBTFLV.push_back(m_tflv);
						// ��Ʒ���߼۸�
						_itoa((int)vect_LogInfo[i].booth_sell.item_price, strInt, 10);
						m_tflv.nIndex = DBTFLV.size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_ItemPrice;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
						DBTFLV.push_back(m_tflv);
						break;
					}
					case REP_LOG_BOOTH_ITEM_ELE::BOOTH_BUY:
						{
							m_tflv.m_tvlength = strlen("̯�����")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"̯�����");
							DBTFLV.push_back(m_tflv);

							m_tflv.nIndex = DBTFLV.size()+1;
							m_tflv.m_tagName = CEnumCore::TagName::WA_SrcCharName;
							m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tvlength = strlen(vect_LogInfo[i].booth_buy.src_char_name.charName);
							sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].booth_buy.src_char_name.charName);
							DBTFLV.push_back(m_tflv);

							m_tflv.nIndex = DBTFLV.size()+1;
							m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
							m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
							m_tflv.m_tvlength = strlen(vect_LogInfo[i].booth_buy.item_name);
							sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].booth_buy.item_name);
							DBTFLV.push_back(m_tflv);

							_itoa((int)vect_LogInfo[i].booth_buy.item_price, strInt, 10);
							m_tflv.nIndex = DBTFLV.size()+1;
							m_tflv.m_tagName = CEnumCore::TagName::WA_ItemPrice;
							m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
							m_tflv.m_tvlength = strlen(strInt)+1;
							memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
							DBTFLV.push_back(m_tflv);
							break;
						}

					case REP_LOG_BOOTH_ITEM_ELE::BOOTH_CLOSE:
					{
						m_tflv.m_tvlength = strlen("��̯")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��̯");
						DBTFLV.push_back(m_tflv);

						// ��Ʒ���
						m_tflv.nIndex = DBTFLV.size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_DstCharName;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen("");
						sprintf((char *)&m_tflv.lpdata, "%s", "");
						DBTFLV.push_back(m_tflv);
						// ��Ʒ����
						m_tflv.nIndex = DBTFLV.size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen("");
						sprintf((char *)&m_tflv.lpdata, "%s", "");
						DBTFLV.push_back(m_tflv);
						// ��Ʒ���߼۸�
						_itoa(0, strInt, 10);
						m_tflv.nIndex = DBTFLV.size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_ItemPrice;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
						DBTFLV.push_back(m_tflv);
						break;
					}
				}
					// ����ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex= DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBTFLV.push_back(m_tflv);

					//
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "��ɫ��̯��־��ѯ");
	}
	
	return DBTFLV;	
}

// ��Ʒ:����ɾ����¼(ָʹ��ʱ�䵽��)
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_DeleteItem(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_DELETE_ITEM_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_DELETE_ITEM req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_DELETE_ITEM res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "����ɾ����¼��ѯ");
		}
		else
		{
			sprintf(retMessage, "����ɾ����¼��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_DELETE_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "����ɾ����¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ɾ��ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// ɾ����Ʒ����ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ItemID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��Ʒ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// ��ƷΨһID
					_ui64toa(vect_LogInfo[i].item_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemUniqueId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ɾ����Ʒ����ID
					_itoa((int)vect_LogInfo[i].item_time_limit, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ItemTimeLimit;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex= DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBTFLV.push_back(m_tflv);
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "����ɾ����¼��ѯ");
	}
	
	return DBTFLV;	
}

// ��Ʒ:����ʹ�ü�¼
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_UseItem(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_USE_ITEM_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_USE_ITEM req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_USE_ITEM res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "����ʹ�ü�¼��ѯ");
		}
		else
		{
			sprintf(retMessage, "����ʹ�ü�¼��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_USE_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "����ʹ�ü�¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ʹ��ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);					
					
					// ʹ����Ʒ����ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ItemID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ʹ����Ʒ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// ʹ����ƷΨһID
					_ui64toa(vect_LogInfo[i].item_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemUniqueId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��Ʒ��ʹ������
					_itoa((int)vect_LogInfo[i].item_count, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemCount;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex= DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBTFLV.push_back(m_tflv);
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "����ʹ�ü�¼��ѯ");
	}
	
	return DBTFLV;	
}

// ��Ʒ:װ��������¼
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_CraftItem(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime,int nickID, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_CRAFT_ITEM_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_CRAFT_ITEM req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_CRAFT_ITEM res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "װ��������¼��ѯ");
		}
		else
		{
			sprintf(retMessage, "װ��������¼��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);	
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_CRAFT_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "װ��������¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ����ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// װ����������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_CraftType;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch (vect_LogInfo[i].craft_type)
					{
					case REP_LOG_CRAFT_ITEM_ELE::CRAFT_BLACKSMITH:
						m_tflv.m_tvlength = strlen("����")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"����");						
						break;
					case REP_LOG_CRAFT_ITEM_ELE::CRAFT_SEWING:
						m_tflv.m_tvlength = strlen("�÷�")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"�÷�");
						break;
					case REP_LOG_CRAFT_ITEM_ELE::CRAFT_HANDCRAFT:
						m_tflv.m_tvlength = strlen("�ֹ�")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"�ֹ�");
						break;
					}
					DBTFLV.push_back(m_tflv);
					
					// ͼֽID
					_itoa((int)vect_LogInfo[i].recipe_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_RecipeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ͼֽ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_RecipeName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].recipe_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].recipe_name);
					DBTFLV.push_back(m_tflv);

					for (int j=0; j<6; j++)
					{
						switch (j)
						{
						case 0:
							{
								if(vect_LogInfo[i].material_id[j] == 0xffffFFFF)
									strcpy(strInt, "");
								else
									_itoa((int)vect_LogInfo[i].material_id[j], strInt, 10);

								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialId1;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = strlen(strInt)+1;
								memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
								DBTFLV.push_back(m_tflv);

								// ��������
								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialName1;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = strlen(vect_LogInfo[i].material_name[j]);
								sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].material_name[j]);
								DBTFLV.push_back(m_tflv);
							}
							break;
						case 1:
							{
								if(vect_LogInfo[i].material_id[j] == 0xffffFFFF)
									strcpy(strInt, "");
								else
									_itoa((int)vect_LogInfo[i].material_id[j], strInt, 10);

								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialId2;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = strlen(strInt)+1;
								memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
								DBTFLV.push_back(m_tflv);

								// ��������
								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialName2;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = strlen(vect_LogInfo[i].material_name[j]);
								sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].material_name[j]);
								DBTFLV.push_back(m_tflv);
							}
							break;
						case 2:
							{
								if(vect_LogInfo[i].material_id[j] == 0xffffFFFF)
									strcpy(strInt, "");
								else
									_itoa((int)vect_LogInfo[i].material_id[j], strInt, 10);

								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialId3;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = strlen(strInt)+1;
								memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
								DBTFLV.push_back(m_tflv);

								// ��������
								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialName3;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = strlen(vect_LogInfo[i].material_name[j]);
								sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].material_name[j]);
								DBTFLV.push_back(m_tflv);
							}
							break;
						case 3:
							{
								if(vect_LogInfo[i].material_id[j] == 0xffffFFFF)
									strcpy(strInt, "");
								else
									_itoa((int)vect_LogInfo[i].material_id[j], strInt, 10);

								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialId4;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = strlen(strInt)+1;
								memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
								DBTFLV.push_back(m_tflv);

								// ��������
								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialName4;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = strlen(vect_LogInfo[i].material_name[j]);
								sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].material_name[j]);
								DBTFLV.push_back(m_tflv);
							}
							break;
						case 4:
							{
								if(vect_LogInfo[i].material_id[j] == 0xffffFFFF)
									strcpy(strInt, "");
								else
									_itoa((int)vect_LogInfo[i].material_id[j], strInt, 10);

								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialId5;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = strlen(strInt)+1;
								memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
								DBTFLV.push_back(m_tflv);

								// ��������
								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialName5;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = strlen(vect_LogInfo[i].material_name[j]);
								sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].material_name[j]);
								DBTFLV.push_back(m_tflv);
							}
							break;
						case 5:
							{
								if(vect_LogInfo[i].material_id[j] == 0xffffFFFF)
									strcpy(strInt, "");
								else
									_itoa((int)vect_LogInfo[i].material_id[j], strInt, 10);

								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialId6;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = strlen(strInt)+1;
								memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
								DBTFLV.push_back(m_tflv);

								// ��������
								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialName6;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = strlen(vect_LogInfo[i].material_name[j]);
								sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].material_name[j]);
								DBTFLV.push_back(m_tflv);
							}
							break;
						}
					}

					//��ƷID
					_itoa((int)vect_LogInfo[i].product_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ProductId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					//��Ʒ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ProductName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].product_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].product_name);
					DBTFLV.push_back(m_tflv);
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex= DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBTFLV.push_back(m_tflv);
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "װ��������¼��ѯ");
	}
	
	return DBTFLV;	
}

// ��Ʒ:װ�����
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_PunchItem(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime,int nickID, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_PUNCH_ITEM_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_PUNCH_ITEM req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_PUNCH_ITEM res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "װ����ײ�ѯ");
		}
		else
		{
			sprintf(retMessage, "װ����ײ�ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_PUNCH_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "װ����ײ�ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ���ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// ��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeHammer;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch (vect_LogInfo[i].consume_hammer)
					{
					case REP_LOG_PUNCH_ITEM_ELE::LOW_LEVEL_HAMMER:
						m_tflv.m_tvlength = strlen("��������")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��������");						
						break;
					case REP_LOG_PUNCH_ITEM_ELE::MIDDLE_LEVEL_HAMMER:
						m_tflv.m_tvlength = strlen("�м�����")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"�м�����");
						break;
					case REP_LOG_PUNCH_ITEM_ELE::HIGH_LEVEL_HAMMER:
						m_tflv.m_tvlength = strlen("�߼�����")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"�߼�����");
						break;
					}
					DBTFLV.push_back(m_tflv);

					// ��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeStithy;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch (vect_LogInfo[i].consume_stithy)
					{
					case REP_LOG_PUNCH_ITEM_ELE::LOW_LEVEL_STITHY:
						m_tflv.m_tvlength = strlen("��������")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��������");						
						break;
					case REP_LOG_PUNCH_ITEM_ELE::MIDDLE_LEVEL_STITHY:
						m_tflv.m_tvlength = strlen("�м�����")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"�м�����");
						break;
					case REP_LOG_PUNCH_ITEM_ELE::HIGH_LEVEL_STITHY:
						m_tflv.m_tvlength = strlen("�߼�����")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"�߼�����");
						break;
					}
					DBTFLV.push_back(m_tflv);

					// ���Ľ�Ǯ
					_itoa((int)vect_LogInfo[i].consume_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ��Ʒ����ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ItemID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��Ʒ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// ��ƷΨһID
					_ui64toa(vect_LogInfo[i].item_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemUniqueId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex= DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBTFLV.push_back(m_tflv);

				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "װ����ײ�ѯ");
	}
	
	return DBTFLV;
}

// ��Ʒ:װ���Ŀ�
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_ChangeSlotItem(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime,int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_CHANGE_SLOT_ITEM_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_CHANGE_SLOT_ITEM req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_CHANGE_SLOT_ITEM res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "װ���Ŀײ�ѯ");
		}
		else
		{
			sprintf(retMessage, "װ���Ŀײ�ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_CHANGE_SLOT_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "װ���Ŀײ�ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 10240);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// �Ŀ�ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// �Ŀ�ǰ��ɫ
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_OldColor;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch (vect_LogInfo[i].old_color)
					{
					case ADMIN::GEM_COLOR_RED:
						m_tflv.m_tvlength = strlen("��")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��");						
						break;
					case ADMIN::GEM_COLOR_YELLOW:
						m_tflv.m_tvlength = strlen("��")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��");
						break;
					case ADMIN::GEM_COLOR_BLUE:
						m_tflv.m_tvlength = strlen("��")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��");
						break;
					case ADMIN::GEM_COLOR_GREEN:
						m_tflv.m_tvlength = strlen("��")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��");
						break;
					case ADMIN::GEM_COLOR_PURPLE:
						m_tflv.m_tvlength = strlen("��")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��");
						break;
					case ADMIN::GEM_COLOR_WHITE:
						m_tflv.m_tvlength = strlen("��")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��");
						break;
					}
					DBTFLV.push_back(m_tflv);

					// �Ŀ�ǰ��ɫ
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NewColor;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch (vect_LogInfo[i].new_color)
					{
					case ADMIN::GEM_COLOR_RED:
						m_tflv.m_tvlength = strlen("��")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��");						
						break;
					case ADMIN::GEM_COLOR_YELLOW:
						m_tflv.m_tvlength = strlen("��")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��");
						break;
					case ADMIN::GEM_COLOR_BLUE:
						m_tflv.m_tvlength = strlen("��")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��");
						break;
					case ADMIN::GEM_COLOR_GREEN:
						m_tflv.m_tvlength = strlen("��")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��");
						break;
					case ADMIN::GEM_COLOR_PURPLE:
						m_tflv.m_tvlength = strlen("��")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��");
						break;
					case ADMIN::GEM_COLOR_WHITE:
						m_tflv.m_tvlength = strlen("��")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��");
						break;
					}
					DBTFLV.push_back(m_tflv);
					
					// ���Ľ�Ǯ
					_itoa((int)vect_LogInfo[i].consume_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��Ʒ����ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ItemID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��Ʒ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// ��ƷΨһID
					_ui64toa(vect_LogInfo[i].item_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemUniqueId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex= DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength=sizeof(int);
					sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
					DBTFLV.push_back(m_tflv);
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "װ���Ŀײ�ѯ");
	}
	
	return DBTFLV;
}

// ��Ʒ:װ������
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_UpgradeStar(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_UPGRADE_STAR_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_UPGRADE_STAR req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_UPGRADE_STAR res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "װ�����ǲ�ѯ");
		}
		else
		{
			sprintf(retMessage, "װ�����ǲ�ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_UPGRADE_STAR);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "װ�����ǲ�ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 10240);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// �Ŀ�ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);

					// ����ǰ����
					_itoa((int)vect_LogInfo[i].old_star_count, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_OldStarCount;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ���Ǻ�����
					_itoa((int)vect_LogInfo[i].new_star_count, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NewStarCount;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ���Ľ�Ǯ
					_itoa((int)vect_LogInfo[i].consume_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��Ʒ����ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ItemID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��Ʒ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// ��ƷΨһID
					_ui64toa(vect_LogInfo[i].item_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemUniqueId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "װ�����ǲ�ѯ");
	}
	
	return DBTFLV;
}

// ��Ʒ:��ʯ
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_Gem(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_GEM_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_GEM req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_GEM res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "��ʯ��Ϣ��ѯ");
		}
		else
		{
			sprintf(retMessage, "��ʯ��Ϣ��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_GEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "��ʯ��Ϣ��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 10240);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GemOperation;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch (vect_LogInfo[i].gem_operation)
					{
					case REP_LOG_GEM_ELE::GEM_ADD:
						m_tflv.m_tvlength = strlen("��Ƕ")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��Ƕ");						
						break;
					case REP_LOG_GEM_ELE::GEM_REMOVE:
						m_tflv.m_tvlength = strlen("ȥ��")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"ȥ��");
						break;
					}
					DBTFLV.push_back(m_tflv);

					// �Ŀ�ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);

					// ��ʯ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GemType;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch (vect_LogInfo[i].gem_type)
					{
					case ADMIN::GEM_COLOR_RED:
						m_tflv.m_tvlength = strlen("��")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��");						
						break;
					case ADMIN::GEM_COLOR_YELLOW:
						m_tflv.m_tvlength = strlen("��")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��");
						break;
					case ADMIN::GEM_COLOR_BLUE:
						m_tflv.m_tvlength = strlen("��")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��");
						break;
					case ADMIN::GEM_COLOR_GREEN:
						m_tflv.m_tvlength = strlen("��")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��");
						break;
					case ADMIN::GEM_COLOR_PURPLE:
						m_tflv.m_tvlength = strlen("��")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��");
						break;
					case ADMIN::GEM_COLOR_WHITE:
						m_tflv.m_tvlength = strlen("��")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��");
						break;
					}
					DBTFLV.push_back(m_tflv);
					
					// ��ʯ�ȼ�
					_itoa((int)vect_LogInfo[i].gem_level, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GemLevel;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ���Ľ�Ǯ
					_itoa((int)vect_LogInfo[i].consume_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��Ʒ����ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ItemID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��Ʒ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// ��ƷΨһID
					_ui64toa(vect_LogInfo[i].item_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemUniqueId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "��ʯ��Ϣ��ѯ");
	}
	
	return DBTFLV;
}

// ��Ʒ:��ҩ��¼
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_Pharma(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime,int nickID, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_PHARMA_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_PHARMA req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_PHARMA res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "��ҩ��¼��ѯ");
		}
		else
		{
			sprintf(retMessage, "��ҩ��¼��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);	
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_PHARMA);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "��ҩ��¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 10240);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ����ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// ͼֽID
					_itoa((int)vect_LogInfo[i].recipe_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_RecipeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ͼֽ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_RecipeName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].recipe_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].recipe_name);
					DBTFLV.push_back(m_tflv);

					for (int j=0; j<6; j++)
					{
						switch (j)
						{
						case 0:
							{
								if(vect_LogInfo[i].material_id[j] == 0xffffFFFF)
									strcpy(strInt, "");
								else
									_itoa((int)vect_LogInfo[i].material_id[j], strInt, 10);

								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialId1;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = strlen(strInt)+1;
								memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
								DBTFLV.push_back(m_tflv);

								// ��������
								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialName1;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = strlen(vect_LogInfo[i].material_name[j]);
								sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].material_name[j]);
								DBTFLV.push_back(m_tflv);
							}
							break;
						case 1:
							{
								if(vect_LogInfo[i].material_id[j] == 0xffffFFFF)
									strcpy(strInt, "");
								else
									_itoa((int)vect_LogInfo[i].material_id[j], strInt, 10);

								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialId2;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = strlen(strInt)+1;
								memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
								DBTFLV.push_back(m_tflv);

								// ��������
								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialName2;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = strlen(vect_LogInfo[i].material_name[j]);
								sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].material_name[j]);
								DBTFLV.push_back(m_tflv);
							}
							break;
						case 2:
							{
								if(vect_LogInfo[i].material_id[j] == 0xffffFFFF)
									strcpy(strInt, "");
								else
									_itoa((int)vect_LogInfo[i].material_id[j], strInt, 10);

								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialId3;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = strlen(strInt)+1;
								memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
								DBTFLV.push_back(m_tflv);

								// ��������
								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialName3;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = strlen(vect_LogInfo[i].material_name[j]);
								sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].material_name[j]);
								DBTFLV.push_back(m_tflv);
							}
							break;
						case 3:
							{
								if(vect_LogInfo[i].material_id[j] == 0xffffFFFF)
									strcpy(strInt, "");
								else
									_itoa((int)vect_LogInfo[i].material_id[j], strInt, 10);

								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialId4;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = strlen(strInt)+1;
								memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
								DBTFLV.push_back(m_tflv);

								// ��������
								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialName4;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = strlen(vect_LogInfo[i].material_name[j]);
								sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].material_name[j]);
								DBTFLV.push_back(m_tflv);
							}
							break;
						case 4:
							{
								if(vect_LogInfo[i].material_id[j] == 0xffffFFFF)
									strcpy(strInt, "");
								else
									_itoa((int)vect_LogInfo[i].material_id[j], strInt, 10);

								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialId5;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = strlen(strInt)+1;
								memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
								DBTFLV.push_back(m_tflv);

								// ��������
								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialName5;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = strlen(vect_LogInfo[i].material_name[j]);
								sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].material_name[j]);
								DBTFLV.push_back(m_tflv);
							}
							break;
						case 5:
							{
								if(vect_LogInfo[i].material_id[j] == 0xffffFFFF)
									strcpy(strInt, "");
								else
									_itoa((int)vect_LogInfo[i].material_id[j], strInt, 10);

								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialId6;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = strlen(strInt)+1;
								memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
								DBTFLV.push_back(m_tflv);

								// ��������
								m_tflv.nIndex = DBTFLV.size()+1;
								m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialName6;
								m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
								m_tflv.m_tvlength = strlen(vect_LogInfo[i].material_name[j]);
								sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].material_name[j]);
								DBTFLV.push_back(m_tflv);
							}
							break;
						}
					}

					//��ƷID
					_itoa((int)vect_LogInfo[i].product_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ProductId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					//��Ʒ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ProductName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].product_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].product_name);
					DBTFLV.push_back(m_tflv);
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "��ҩ��¼��ѯ");
	}
	
	return DBTFLV;
}

// ��Ʒ:��ʯ�ϳ�
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_CraftGem(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_CRAFT_GEM_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_CRAFT_GEM req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_CRAFT_GEM res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "��ʯ�ϳɲ�ѯ");
		}
		else
		{
			sprintf(retMessage, "��ʯ�ϳɲ�ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);	
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_CRAFT_GEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "��ʯ�ϳɲ�ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 10240);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// �ϳ�ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);

					// ���Ľ�Ǯ
					_itoa((int)vect_LogInfo[i].consume_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// �ɹ��ʵ���
					_itoa((int)vect_LogInfo[i].prop_item_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_PropItemId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ԭ����ID
					_itoa((int)vect_LogInfo[i].material_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ԭ��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].material_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].material_name);
					DBTFLV.push_back(m_tflv);

					//��ƷID
					_itoa((int)vect_LogInfo[i].product_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ProductId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					//��Ʒ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ProductName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].product_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].product_name);
					DBTFLV.push_back(m_tflv);	
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}

			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "��ʯ�ϳɲ�ѯ");
	}
	
	return DBTFLV;
}

// ��Ʒ:���Ϻϳ�
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_CraftMatirail(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_CRAFT_MATIRAIL_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_CRAFT_MATIRAIL req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_CRAFT_MATIRAIL res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "���Ϻϳɲ�ѯ");
		}
		else
		{
			sprintf(retMessage, "���Ϻϳɲ�ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);	
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_CRAFT_MATIRAIL);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "���Ϻϳɲ�ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 10240);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// �ϳ�ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// ���Ľ�Ǯ
					_itoa((int)vect_LogInfo[i].consume_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// �ɹ��ʵ���
					_itoa((int)vect_LogInfo[i].prop_item_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_PropItemId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ԭ����ID
					_itoa((int)vect_LogInfo[i].material_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ԭ��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].material_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].material_name);
					DBTFLV.push_back(m_tflv);
					
					//��ƷID
					_itoa((int)vect_LogInfo[i].product_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ProductId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					//��Ʒ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ProductName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].product_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].product_name);
					DBTFLV.push_back(m_tflv);
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "���Ϻϳɲ�ѯ");
	}
	
	return DBTFLV;
}

// ��Ʒ:ͼֽ�ϳ�
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_CraftRecipe(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_CRAFT_RECIPE_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_CRAFT_RECIPE req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_CRAFT_RECIPE res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "ͼֽ�ϳɲ�ѯ");
		}
		else
		{
			sprintf(retMessage, "ͼֽ�ϳɲ�ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_CRAFT_RECIPE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "ͼֽ�ϳɲ�ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 10240);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// �ϳ�ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// ���Ľ�Ǯ
					_itoa((int)vect_LogInfo[i].consume_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// �ɹ��ʵ���
					_itoa((int)vect_LogInfo[i].prop_item_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_PropItemId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ԭ����ID
					_itoa((int)vect_LogInfo[i].material_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ԭ��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].material_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].material_name);
					DBTFLV.push_back(m_tflv);
					
					//��ƷID
					_itoa((int)vect_LogInfo[i].product_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ProductId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					//��Ʒ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ProductName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].product_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].product_name);
					DBTFLV.push_back(m_tflv);
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "ͼֽ�ϳɲ�ѯ");
	}
	
	return DBTFLV;
}

// �Թ����ֶһ�����
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_MazeIntegral(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_MAZE_INTEGERAL_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_MAZE_INTEGRAL req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_MAZE_INTEGRAL res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "�Թ����ֶһ�������ѯ");
		}
		else
		{
			sprintf(retMessage, "�Թ����ֶһ�������ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_MAZE_INTEGRAL);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "�Թ����ֶһ�������ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 10240);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// �һ�ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);

					// �һ�����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ChangeType;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch (vect_LogInfo[i].change_type)
					{
					case REP_LOG_MAZE_INTEGERAL_ELE::CHANGE_EXP:
						m_tflv.m_tvlength = strlen("�һ�����")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"�һ�����");						
						break;
					case REP_LOG_MAZE_INTEGERAL_ELE::CHANGE_MONEY:
						m_tflv.m_tvlength = strlen("�һ���Ǯ")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"�һ���Ǯ");
						break;
					}
					DBTFLV.push_back(m_tflv);
					
					// ���Ļ���
					_itoa((int)vect_LogInfo[i].consume_integral, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeIntegral;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// �õ��ľ������Ǯ
					_itoa((int)vect_LogInfo[i].change_point, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ChangePoint;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "�Թ����ֶһ�������ѯ");
	}
	
	return DBTFLV;
}

// ��Ϸ��:�������
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_MonsterDropMoney(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_MONSTER_DROP_MONEY_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_MONSTER_DROP_MONEY req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_MONSTER_DROP_MONEY res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "���������Ϸ�Ҳ�ѯ");
		}
		else
		{
			sprintf(retMessage, "���������Ϸ�Ҳ�ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);	
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_MONSTER_DROP_MONEY);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);
			
			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "���������Ϸ�Ҳ�ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
//					memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					//Sleep(1000);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ���ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// ��ý�Ǯ����
					_itoa((int)vect_LogInfo[i].get_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��ԴID
					_itoa((int)vect_LogInfo[i].src_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��Դ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].src_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].src_name);
					DBTFLV.push_back(m_tflv);
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "���������Ϸ�Ҳ�ѯ");
	}
	
	return DBTFLV;
}

// ��Ϸ��:������
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_GetMoneyQuest(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_GET_MONEY_QUEST_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_GET_MONEY_QUEST req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_GET_MONEY_QUEST res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "��������Ϸ�Ҳ�ѯ");
		}
		else
		{
			sprintf(retMessage, "��������Ϸ�Ҳ�ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);	
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_GET_MONEY_QUEST);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "��������Ϸ�Ҳ�ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
//					memset(recvbuf, 0, 20480);
					*recvbuf = NULL;

					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ���ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// ��ý�Ǯ����
					_itoa((int)vect_LogInfo[i].get_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��ԴID
					_itoa((int)vect_LogInfo[i].src_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��Դ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].src_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].src_name);
					DBTFLV.push_back(m_tflv);
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "��������Ϸ�Ҳ�ѯ");
	}
	
	return DBTFLV;
}

// ��Ϸ��:��װ
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_LoseMoneyRepair(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_LOSE_MONEY_REPAIR_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_LOSE_MONEY_REPAIR req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_LOSE_MONEY_REPAIR res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "��װ��Ϣ��ѯ");
		}
		else
		{
			sprintf(retMessage, "��װ��Ϣ��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);	
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_LOSE_MONEY_REPAIR);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "��װ��Ϣ��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// ���Ľ�Ǯ
					_itoa((int)vect_LogInfo[i].consume_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��Ʒ����
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ItemID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��Ʒ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "��װ��Ϣ��ѯ");
	}
	
	return DBTFLV;
}

// ��Ϸ��:���׻��
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_GetMoneyTrade(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_GET_MONEY_TRADE_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_GET_MONEY_TRADE req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_GET_MONEY_TRADE res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "���׻����Ϸ�Ҳ�ѯ");
		}
		else
		{
			sprintf(retMessage, "���׻����Ϸ�Ҳ�ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);	
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_GET_MONEY_TRADE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "���׻����Ϸ�Ҳ�ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ���ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// ��ý�Ǯ����
					_itoa((int)vect_LogInfo[i].get_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��ԴID
					_itoa((int)vect_LogInfo[i].src_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��Դ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].src_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].src_name);
					DBTFLV.push_back(m_tflv);
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "���׻����Ϸ�Ҳ�ѯ");
	}
	
	return DBTFLV;
}

// ��Ϸ��:����ʧȥ
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_LoseMoneyTrade(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime,int nickID, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_LOSE_MONEY_TRADE_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_LOSE_MONEY_TRADE req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_LOSE_MONEY_TRADE res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "����ʧȥ��Ϸ�Ҳ�ѯ");
		}
		else
		{
			sprintf(retMessage, "����ʧȥ��Ϸ�Ҳ�ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_LOSE_MONEY_TRADE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "����ʧȥ��Ϸ�Ҳ�ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ���ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// ʧȥ��Ǯ����
					_itoa((int)vect_LogInfo[i].lose_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_LoseMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��ԴID
					_itoa((int)vect_LogInfo[i].dst_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_DstId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��Դ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_DstName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].dst_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].dst_name);
					DBTFLV.push_back(m_tflv);
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "����ʧȥ��Ϸ�Ҳ�ѯ");
	}
	
	return DBTFLV;
}

// ����:��ɻ�ü�¼
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_QuestReward(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_QUEST_REWARD_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_QUEST_REWARD req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_QUEST_REWARD res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "��ɻ�ü�¼��ѯ");
		}
		else
		{
			sprintf(retMessage, "��ɻ�ü�¼��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_QUEST_REWARD);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "��ɻ�ü�¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// ����ID
					_itoa((int)vect_LogInfo[i].quest_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_QuestId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// �����Ľ�Ǯ
					_itoa((int)vect_LogInfo[i].reward_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_RewardMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_QuestName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].quest_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].quest_name);
					DBTFLV.push_back(m_tflv);

					// ��������Ʒ
					_itoa((int)vect_LogInfo[i].reward_item_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_RewardItemId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��������Ʒ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_RewardItemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].reward_item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].reward_item_name);
					DBTFLV.push_back(m_tflv);
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "��ɻ�ü�¼��ѯ");
	}
	
	return DBTFLV;
}

// ����:������Ʒɾ����¼
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_QuestGive(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime,int nickID, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_QUEST_GIVE_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_QUEST_GIVE req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_QUEST_GIVE res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "������Ʒɾ����¼��ѯ");
		}
		else
		{
			sprintf(retMessage, "������Ʒɾ����¼��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_QUEST_GIVE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "������Ʒɾ����¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// ����ID
					_itoa((int)vect_LogInfo[i].quest_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_QuestId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_QuestName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].quest_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].quest_name);
					DBTFLV.push_back(m_tflv);
					
					// ʧȥ����ƷID
					_itoa((int)vect_LogInfo[i].give_item_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GiveItemId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ʧȥ����Ʒ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GiveItemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].give_item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].give_item_name);
					DBTFLV.push_back(m_tflv);
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "������Ʒɾ����¼��ѯ");
	}
	
	return DBTFLV;
}

// �ʼ�:���ͼ�¼
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_MailSend(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_MAIL_SEND_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_MAIL_SEND req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_MAIL_SEND res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "�ʼ����ͼ�¼��ѯ");
		}
		else
		{
			sprintf(retMessage, "�ʼ����ͼ�¼��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);	
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_MAIL_SEND);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "�ʼ����ͼ�¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);

					// ΨһID
					_ui64toa(vect_LogInfo[i].mail_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MailId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ��Ǯ����
					_itoa((int)vect_LogInfo[i].mail_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MailMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// �ռ���ID
					_itoa((int)vect_LogInfo[i].dst_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_DstId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// �ռ�������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_DstName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].dst_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].dst_name);
					DBTFLV.push_back(m_tflv);

					for (int j=0; j<5; j++)
					{
						switch (j)
						{
						case 0:
							{
								if(vect_LogInfo[i].mail_item_count[j] != 0)
								{
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item1;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen(vect_LogInfo[i].mail_item[j]);
									sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].mail_item[j]);
									DBTFLV.push_back(m_tflv);

									_itoa((int)vect_LogInfo[i].mail_item_count[j], strInt, 10);
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item1Count;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
									m_tflv.m_tvlength = strlen(strInt)+1;
									memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
									DBTFLV.push_back(m_tflv);

								}
								else
								{
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item1;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen("");
									sprintf((char *)&m_tflv.lpdata, "");
									DBTFLV.push_back(m_tflv);
									
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item1Count;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen("");
									sprintf((char *)&m_tflv.lpdata, "");
									DBTFLV.push_back(m_tflv);
								}
							}
							break;
						case 1:
							{
								if(vect_LogInfo[i].mail_item_count[j] != 0)
								{
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item2;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen(vect_LogInfo[i].mail_item[j]);
									sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].mail_item[j]);
									DBTFLV.push_back(m_tflv);
									
									_itoa((int)vect_LogInfo[i].mail_item_count[j], strInt, 10);
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item2Count;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
									m_tflv.m_tvlength = strlen(strInt)+1;
									memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
									DBTFLV.push_back(m_tflv);
									
								}
								else
								{
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item2;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen("");
									sprintf((char *)&m_tflv.lpdata, "");
									DBTFLV.push_back(m_tflv);
									
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item2Count;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen("");
									sprintf((char *)&m_tflv.lpdata, "");
									DBTFLV.push_back(m_tflv);
								}								
							}
							break;
						case 2:
							{
								if(vect_LogInfo[i].mail_item_count[j] != 0)
								{
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item3;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen(vect_LogInfo[i].mail_item[j]);
									sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].mail_item[j]);
									DBTFLV.push_back(m_tflv);
									
									_itoa((int)vect_LogInfo[i].mail_item_count[j], strInt, 10);
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item3Count;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
									m_tflv.m_tvlength = strlen(strInt)+1;
									memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
									DBTFLV.push_back(m_tflv);
									
								}
								else
								{
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item3;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen("");
									sprintf((char *)&m_tflv.lpdata, "");
									DBTFLV.push_back(m_tflv);
									
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item3Count;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen("");
									sprintf((char *)&m_tflv.lpdata, "");
									DBTFLV.push_back(m_tflv);
								}								
							}
							break;
						case 3:
							{
								if(vect_LogInfo[i].mail_item_count[j] != 0)
								{
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item4;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen(vect_LogInfo[i].mail_item[j]);
									sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].mail_item[j]);
									DBTFLV.push_back(m_tflv);
									
									_itoa((int)vect_LogInfo[i].mail_item_count[j], strInt, 10);
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item4Count;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
									m_tflv.m_tvlength = strlen(strInt)+1;
									memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
									DBTFLV.push_back(m_tflv);
									
								}
								else
								{
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item4;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen("");
									sprintf((char *)&m_tflv.lpdata, "");
									DBTFLV.push_back(m_tflv);
									
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item4Count;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen("");
									sprintf((char *)&m_tflv.lpdata, "");
									DBTFLV.push_back(m_tflv);
								}								
							}
							break;
						case 4:
							{
								if(vect_LogInfo[i].mail_item_count[j] != 0)
								{
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item5;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen(vect_LogInfo[i].mail_item[j]);
									sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].mail_item[j]);
									DBTFLV.push_back(m_tflv);
									
									_itoa((int)vect_LogInfo[i].mail_item_count[j], strInt, 10);
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item5Count;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
									m_tflv.m_tvlength = strlen(strInt)+1;
									memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
									DBTFLV.push_back(m_tflv);
									
								}
								else
								{
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item5;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen("");
									sprintf((char *)&m_tflv.lpdata, "");
									DBTFLV.push_back(m_tflv);
									
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item5Count;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen("");
									sprintf((char *)&m_tflv.lpdata, "");
									DBTFLV.push_back(m_tflv);
								}								
							}
							break;
						}
					}

					// �ʼ�����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MailTitle;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].mail_title);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].mail_title);
					DBTFLV.push_back(m_tflv);

					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	

				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "�ʼ����ͼ�¼��ѯ");
	}
	
	return DBTFLV;
}

// �ʼ�:���ռ�¼
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_MailRecv(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_MAIL_RECV_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_MAIL_RECV req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_MAIL_RECV res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "�ʼ����ռ�¼��ѯ");
		}
		else
		{
			sprintf(retMessage, "�ʼ����ռ�¼��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);	
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_MAIL_RECV);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "�ʼ����ռ�¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// ΨһID
					_ui64toa(vect_LogInfo[i].mail_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MailId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��Ǯ����
					_itoa((int)vect_LogInfo[i].mail_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MailMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ������ID
					_itoa((int)vect_LogInfo[i].src_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ����������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MailorName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].src_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].src_name);
					DBTFLV.push_back(m_tflv);
					
					for (int j=0; j<5; j++)
					{
						switch (j)
						{
						case 0:
							{
								if(vect_LogInfo[i].mail_item_count[j] != 0)
								{
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item1;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen(vect_LogInfo[i].mail_item[j]);
									sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].mail_item[j]);
									DBTFLV.push_back(m_tflv);

									_itoa((int)vect_LogInfo[i].mail_item_count[j], strInt, 10);
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item1Count;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
									m_tflv.m_tvlength = strlen(strInt)+1;
									memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
									DBTFLV.push_back(m_tflv);

								}
								else
								{
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item1;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen("");
									sprintf((char *)&m_tflv.lpdata, "");
									DBTFLV.push_back(m_tflv);
									
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item1Count;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen("");
									sprintf((char *)&m_tflv.lpdata, "");
									DBTFLV.push_back(m_tflv);
								}
							}
							break;
						case 1:
							{
								if(vect_LogInfo[i].mail_item_count[j] != 0)
								{
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item2;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen(vect_LogInfo[i].mail_item[j]);
									sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].mail_item[j]);
									DBTFLV.push_back(m_tflv);
									
									_itoa((int)vect_LogInfo[i].mail_item_count[j], strInt, 10);
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item2Count;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
									m_tflv.m_tvlength = strlen(strInt)+1;
									memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
									DBTFLV.push_back(m_tflv);
									
								}
								else
								{
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item2;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen("");
									sprintf((char *)&m_tflv.lpdata, "");
									DBTFLV.push_back(m_tflv);
									
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item2Count;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen("");
									sprintf((char *)&m_tflv.lpdata, "");
									DBTFLV.push_back(m_tflv);
								}								
							}
							break;
						case 2:
							{
								if(vect_LogInfo[i].mail_item_count[j] != 0)
								{
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item3;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen(vect_LogInfo[i].mail_item[j]);
									sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].mail_item[j]);
									DBTFLV.push_back(m_tflv);
									
									_itoa((int)vect_LogInfo[i].mail_item_count[j], strInt, 10);
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item3Count;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
									m_tflv.m_tvlength = strlen(strInt)+1;
									memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
									DBTFLV.push_back(m_tflv);
									
								}
								else
								{
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item3;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen("");
									sprintf((char *)&m_tflv.lpdata, "");
									DBTFLV.push_back(m_tflv);
									
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item3Count;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen("");
									sprintf((char *)&m_tflv.lpdata, "");
									DBTFLV.push_back(m_tflv);
								}								
							}
							break;
						case 3:
							{
								if(vect_LogInfo[i].mail_item_count[j] != 0)
								{
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item4;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen(vect_LogInfo[i].mail_item[j]);
									sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].mail_item[j]);
									DBTFLV.push_back(m_tflv);
									
									_itoa((int)vect_LogInfo[i].mail_item_count[j], strInt, 10);
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item4Count;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
									m_tflv.m_tvlength = strlen(strInt)+1;
									memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
									DBTFLV.push_back(m_tflv);
									
								}
								else
								{
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item4;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen("");
									sprintf((char *)&m_tflv.lpdata, "");
									DBTFLV.push_back(m_tflv);
									
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item4Count;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen("");
									sprintf((char *)&m_tflv.lpdata, "");
									DBTFLV.push_back(m_tflv);
								}								
							}
							break;
						case 4:
							{
								if(vect_LogInfo[i].mail_item_count[j] != 0)
								{
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item5;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen(vect_LogInfo[i].mail_item[j]);
									sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].mail_item[j]);
									DBTFLV.push_back(m_tflv);
									
									// �ռ���ID
									_itoa((int)vect_LogInfo[i].mail_item_count[j], strInt, 10);
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item5Count;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
									m_tflv.m_tvlength = strlen(strInt)+1;
									memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
									DBTFLV.push_back(m_tflv);
									
								}
								else
								{
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item5;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen("");
									sprintf((char *)&m_tflv.lpdata, "");
									DBTFLV.push_back(m_tflv);
									
									m_tflv.nIndex = DBTFLV.size()+1;
									m_tflv.m_tagName = CEnumCore::TagName::WA_Item5Count;
									m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
									m_tflv.m_tvlength = strlen("");
									sprintf((char *)&m_tflv.lpdata, "");
									DBTFLV.push_back(m_tflv);
								}								
							}
							break;
						}
					}
					
					// �ʼ�����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MailTitle;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].mail_title);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].mail_title);
					DBTFLV.push_back(m_tflv);
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "�ʼ����ռ�¼��ѯ");
	}
	
	return DBTFLV;
}
// �ʼ�:ɾ����¼
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_MailDelete(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_MAIL_DELETE_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_MAIL_DELETE req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_MAIL_DELETE res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "�ʼ�ɾ����¼��ѯ");
		}
		else
		{
			sprintf(retMessage, "�ʼ�ɾ����¼��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_MAIL_DELETE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "�ʼ�ɾ����¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// ΨһID
					_ui64toa(vect_LogInfo[i].mail_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MailId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ������ID
					_itoa((int)vect_LogInfo[i].src_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ����������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].src_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].src_name);
					DBTFLV.push_back(m_tflv);
					
					// �ʼ�����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MailTitle;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].mail_title);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].mail_title);
					DBTFLV.push_back(m_tflv);

					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "�ʼ�ɾ����¼��ѯ");
	}
	
	return DBTFLV;
}

// �̳�:�����¼
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_ItemShop(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector<REP_LOG_ITEM_SHOP_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_ITEM_SHOP req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_ITEM_SHOP res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "�û��ĸ�����־��ѯ");
		}
		else
		{
			sprintf(retMessage, "�û����̳ǹ����¼��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_ITEM_SHOP);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "�û����̳ǹ����¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);

					// ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_TYPE;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					int iNum = vect_LogInfo[i].item_type_id;
					switch (ADMIN::GetStructType(vect_LogInfo[i].item_type_id))
					{
						case 1:
							m_tflv.m_tvlength = strlen("����װ��")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"����װ��");						
							break;
						case 2:
							m_tflv.m_tvlength = strlen("����װ��")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"����װ��");
							break;
						case 3:
							m_tflv.m_tvlength = strlen("����Ʒ/����")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"����Ʒ/����");
							break;
						case 4:
							m_tflv.m_tvlength = strlen("����Ʒ")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"����Ʒ");
							break;
						case 5:
							m_tflv.m_tvlength = strlen("����")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"����");
							break;
						case 6:
							m_tflv.m_tvlength = strlen("�䷽")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"�䷽");
							break;
						case 7:
							m_tflv.m_tvlength = strlen("��ʯ����Ʒ")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"��ʯ����Ʒ");
							break;
						default:
							m_tflv.m_tvlength = strlen("������")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"������");
							break;	
					}

// 					switch (vect_LogInfo[i].item_type_id)
// 					{
// 						case 1:
// 							m_tflv.m_tvlength = strlen("����װ��")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"����װ��");						
// 							break;
// 						case 2:
// 							m_tflv.m_tvlength = strlen("������Ʒ")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"������Ʒ");
// 							break;
// 						case 3:
// 							m_tflv.m_tvlength = strlen("ҩƷ��")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"ҩƷ��");
// 							break;
// 						case 4:
// 							m_tflv.m_tvlength = strlen("������Ʒ")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"������Ʒ");
// 							break;
// 						case 5:
// 							m_tflv.m_tvlength = strlen("����ʹ��������Ʒ")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"����ʹ��������Ʒ");
// 							break;
// 						case 6:
// 							m_tflv.m_tvlength = strlen("����")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"����");
// 							break;
// 						case 7:
// 							m_tflv.m_tvlength = strlen("�Թ���Ʒ")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"�Թ���Ʒ");
// 							break;
// 						case 8:
// 							m_tflv.m_tvlength = strlen("�̳���Ʒ")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"�̳���Ʒ");
// 							break;
// 						case 9:
// 							m_tflv.m_tvlength = strlen("����")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"����");
// 							break;
// 						case 10:
// 							m_tflv.m_tvlength = strlen("�����ɫͼֽ")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"�����ɫͼֽ");
// 							break;
// 						case 11:
// 							m_tflv.m_tvlength = strlen("ҩˮͼֽ")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"ҩˮͼֽ");
// 							break;
// 						case 12:
// 							m_tflv.m_tvlength = strlen("����ʯ")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"����ʯ");
// 							break;	
// 						default:
// 							m_tflv.m_tvlength = strlen("������")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"������");
// 							break;	
// 					}
					DBTFLV.push_back(m_tflv);
					

					// ����ID
					_itoa((int)vect_LogInfo[i].item_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ItemID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					//���߸���
					_itoa((int)vect_LogInfo[i].item_count, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::WA_itemCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					//����M��
					_itoa((int)vect_LogInfo[i].comsume_m_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::WA_ConsumeMoney;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					// ʣ��M��
					_itoa((int)vect_LogInfo[i].rest_m_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::WA_Money;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "�û��ĸ�����־��ѯ");
	}
	
	return DBTFLV;
	return DBTFLV;
}

// ��ѯ�û��ĸ�����־
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_Instance(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_INSTANCE_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_INSTANCE req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_INSTANCE res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "�û��ĸ�����־��ѯ");
		}
		else
		{
			sprintf(retMessage, "�û��ĸ�����־��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_INSTANCE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "�û��ĸ�����־��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);

					// ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_OpType;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch (vect_LogInfo[i].op_type)
					{
						case REP_LOG_INSTANCE_ELE::INSTANCE_ENTER:
							m_tflv.m_tvlength = strlen("���븱��")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"���븱��");						
							break;
						case REP_LOG_INSTANCE_ELE::INSTANCE_LEAVE:
							m_tflv.m_tvlength = strlen("�뿪����")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"�뿪����");
							break;
					}
					DBTFLV.push_back(m_tflv);
					

					// ����ID
					_itoa((int)vect_LogInfo[i].instance_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_InstanceId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_InstanceName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].instance_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].instance_name);
					DBTFLV.push_back(m_tflv);
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "�û��ĸ�����־��ѯ");
	}
	
	return DBTFLV;
}

// ��ҵ�½/�ǳ���־
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_LoginLogout(char *ServerIP, char *GameRegion, char *Account, char *Character, char *Begintime,char *Endtime, int nickID,int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_LOGIN_LOGOUT_ELE> vect_LogInfo;
	char strInt[30];
	char strTemInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_LOGIN_LOGOUT req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_LOGIN_LOGOUT res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "��½�ǳ���־");
		}
		else
		{
			sprintf(retMessage, "��½�ǳ���־: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_LOGIN_LOGOUT);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "��½�ǳ���־");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ʱ��ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_OpType;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch (vect_LogInfo[i].op_type)
					{
					case REP_LOG_LOGIN_LOGOUT_ELE::LOGIN_CHAR:
						m_tflv.m_tvlength = strlen("��ɫ��¼")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��ɫ��¼");						
						break;
					case REP_LOG_LOGIN_LOGOUT_ELE::LOGOUT_CHAR:
						m_tflv.m_tvlength = strlen("��ɫ�ǳ�")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��ɫ�ǳ�");
						break;
					}
					DBTFLV.push_back(m_tflv);
					
					// ��ɫID
					_itoa((int)vect_LogInfo[i].char_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��ɫ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].char_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].char_name);
					DBTFLV.push_back(m_tflv);

					// ��½IP
					_itoa((int)(vect_LogInfo[i].clientIp >> 24), strInt, 10);
					strcat(strInt,".");
					strcat(strInt,_itoa((int)((vect_LogInfo[i].clientIp >> 16) & 0xff), strTemInt, 10));
					strcat(strInt,".");
					strcat(strInt,_itoa((int)((vect_LogInfo[i].clientIp >> 8) & 0xff), strTemInt, 10));
					strcat(strInt,".");
					strcat(strInt,_itoa((int)(vect_LogInfo[i].clientIp & 0xff), strTemInt, 10));
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_LoginIP;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "��½�ǳ���־");
	}
	
	return DBTFLV;
}

// ��ҵ�½/�ǳ���־(��IP��ʽ��ѯ)
vector <CGlobalStruct::TFLV> CWAInfo::WA_Log_LoginLogoutByIP(char *ServerIP, char *Begintime,char *Endtime, char* LoginIP, int iIndex, int iPageSize)
{
 	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_LOGIN_LOGOUT_BY_IP_ELE> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_LOG_LOGIN_LOGOUT_BY_IP req_Log_info;				//��־��Ϣ��ѯ����
		REP_LOG_LOGIN_LOGOUT_BY_IP res_Log_info;				//��־��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		char *strTemp = new char[strlen(LoginIP)+1];
		ZeroMemory(strTemp, strlen(LoginIP)+1);
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "��½�ǳ���־(IP��ʽ)");
		}
		else
		{
			sprintf(retMessage, "��½�ǳ���־(IP��ʽ): <����>%s, <��ʼʱ��>%s, <����ʱ��>%s, <��½IP>%d", ServerIP, Begintime, Endtime, LoginIP);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_LOGIN_LOGOUT_BY_IP);
			
			strcpy(strTemp, LoginIP);

			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			int iTemp = 0;
			char *pTemp = strtok(LoginIP, ".");
			while(pTemp != NULL)
			{
				if(iTemp == 0)
					req_Log_info.clientIp = atoi(pTemp) << 24;
				else if(iTemp == 1)
					req_Log_info.clientIp += (atoi(pTemp) << 16);
				else if(iTemp == 2)
					req_Log_info.clientIp += (atoi(pTemp) << 8);
				else if(iTemp == 3)
					req_Log_info.clientIp += atoi(pTemp);

				pTemp = strtok(NULL, ".");
				iTemp++;
			}

			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
//			req_Log_info.clientIp = LoginIP;
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "��½�ǳ���־(IP��ʽ)");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//��������
								if (res_Log_info.all_send == 1)
								{
									bRecvFlag = false;

									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (res_Log_info.amount>0)
									{
										for (int i=0; i<res_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(res_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ʱ��ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_OpType;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch (vect_LogInfo[i].op_type)
					{
					case REP_LOG_LOGIN_LOGOUT_ELE::LOGIN_CHAR:
						m_tflv.m_tvlength = strlen("��ɫ��¼")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��ɫ��¼");						
						break;
					case REP_LOG_LOGIN_LOGOUT_ELE::LOGOUT_CHAR:
						m_tflv.m_tvlength = strlen("��ɫ�ǳ�")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"��ɫ�ǳ�");
						break;
					}
					DBTFLV.push_back(m_tflv);
					
					// ��ɫID
					_itoa((int)vect_LogInfo[i].char_id.charIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��ɫ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].char_name.charName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].char_name.charName);
					DBTFLV.push_back(m_tflv);

					// ��¼�ǳ�IP
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_LoginIP;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strTemp);
					sprintf((char *)&m_tflv.lpdata, "%s", strTemp);
					DBTFLV.push_back(m_tflv);

					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
		delete []strTemp;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "��½�ǳ���־(IP��ʽ)");
	}
	
 	return DBTFLV;
}

//Added by tengjie 2010-05-13:���ؼ�¼��ͣԭ��,������ʾ���û�
void CWAInfo::WA_SqlForUserInfo(char *UserName,char *ServerIP,int iTime,char *description,int iStatus)
{
	try{
 
		CString szTime = "";
		CString szSql = "";
		CSqlHelper m_SqlHelper;

		SYSTEMTIME timeDest;
		GetLocalTime(&timeDest);

		szTime.Format("%4d-%2d-%2d %2d:%2d:%2d",timeDest.wYear,timeDest.wMonth,timeDest.wDay,timeDest.wHour,timeDest.wMinute,timeDest.wSecond);

		szSql.Format("exec WA_CloseDescription '%s','%s','%s',%i,'%s',%i",UserName,ServerIP,szTime.GetBuffer(0),iTime,description,iStatus);

		m_SqlHelper.SqlHelperMain("127.0.0.1", szSql.GetBuffer(0), 4);
	}
	catch(...){
	
	}
}

CString CWAInfo::WA_SearchDescription(char *UserName,int iStatus)
{
	if(UserName == NULL)
		return "";

	CString szSql = "";
	CString szData = "";
	char connLocalDB[20];
	CSqlHelper m_SqlHelper;
	vector<CGlobalStruct::DataRecord> datarecord;


	szSql.Format("exec WA_CloseDescription '%s','%s','%s',%i,'%s',%i",UserName,"","",0,"",iStatus);

	m_SqlHelper.SqlHelperMain("WA", &datarecord,connLocalDB,szSql.GetBuffer(0),4,0,0);

	if(datarecord.empty())
		return "";

	vector<CGlobalStruct::DataRecord>::iterator iter;
	CGlobalStruct::DataRecord  m_DataRecord;
	int iCount = 0;
	for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
	{
		m_DataRecord = *iter;
		iCount++;
		if(!strcmp("Description", m_DataRecord.recordfieldname))//��ȡ��Ӧ��ServerIP
		{
			if(iCount == datarecord.size() - 1)
				szData.Format("%s", m_DataRecord.recorddata);//�����ѯ�õ������һ����¼
			else
				continue;
		}
	}

	return szData;

}

vector <CGlobalStruct::TFLV> CWAInfo::WA_Net_CloseSingle(char * ServerIP, char * UserName, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;

	try
	{
		if(!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼������ʧ��", "������ͣ��ѯ");
		}
		else
			WA_Net_SingleCloseInfo(UserName, &DBTFLV, 1);
	}
	catch(...)
	{

	}

	return DBTFLV;
}

// bool CWAInfo::JudgeIsExist(vector <REP_LOG_MONSTER_DROP_ITEM_ELE> &vect_LogInfo, REP_LOG_MONSTER_DROP_ITEM_ELE elem)
// {
// 	if(vect_LogInfo.empty())
// 		return false;
// 
// 	vector <REP_LOG_MONSTER_DROP_ITEM_ELE>::iterator pVecLogInfo = NULL;
// 
// 	for(pVecLogInfo = vect_LogInfo.begin(); pVecLogInfo != vect_LogInfo.end(); pVecLogInfo++)
// 	{
// 		if(elem.date_time.day		== pVecLogInfo->date_time.day		&&
// 		   elem.date_time.hour		== pVecLogInfo->date_time.hour		&&
// 		   elem.date_time.minute	== pVecLogInfo->date_time.minute	&&
// 		   elem.date_time.month		== pVecLogInfo->date_time.month		&&
// 		   elem.date_time.second	== pVecLogInfo->date_time.second	&&
// 		   elem.date_time.year		== pVecLogInfo->date_time.year		&&
// 		   elem.item_count			== pVecLogInfo->item_count			&&
// 		   !strcmp(elem.item_name, pVecLogInfo->item_name)				&&
// 		   elem.item_type_id		== pVecLogInfo->item_type_id		/*&&
// 		   elem.item_unique_id		== pVecLogInfo->item_unique_id*/)
// 			return true;
// 	}
// 
// 	return false;
// }

//Added by tengjie 2010-06-11
//��ɫ��־:��ɫ������¼
vector <CGlobalStruct::TFLV> CWAInfo::WA_Character_Upgrade_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_CHAR_LEVELUP_ELE> vect_LogInfo;
	vector <unsigned int> vect_PartID;
	vector <string> vect_PartName;
	char strInt[30];
	char *retMessage = new char[255];
	ZeroMemory(retMessage, 255);

	try
	{		
		REQ_LOG_CHAR_LEVELUP	req_PartLog_info;				//��ɫ������¼��ѯ����
		REP_LOG_CHAR_LEVELUP	rep_PartLog_info;				//��ɫ������¼��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "��ɫ������¼");
		}
		else
		{
			sprintf(retMessage, "��ɫ������¼: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_CHAR_LEVELUP);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "��ɫ������¼");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//��������
								if (rep_PartLog_info.all_send == 1)
								{
									bRecvFlag = false;

									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_PartID.push_back(rep_PartLog_info.char_id.charIndex);
											vect_PartName.push_back(rep_PartLog_info.char_name.charName);
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_PartID.push_back(rep_PartLog_info.char_id.charIndex);
											vect_PartName.push_back(rep_PartLog_info.char_name.charName);
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ��ɫID
					_itoa((int)vect_PartID[i], strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��ɫ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = vect_PartName[i].length();
					sprintf((char *)&m_tflv.lpdata, "%s", vect_PartName[i].c_str());
					DBTFLV.push_back(m_tflv);
					
					// ����ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// �ȼ�
					_itoa((int)vect_LogInfo[i].char_level, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_Level;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt);
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "��ɫ������¼");
	}

	delete []retMessage;

	return DBTFLV;
}

//��ɫ��־:��ɫ������¼��ѯ
vector <CGlobalStruct::TFLV> CWAInfo::WA_Character_Create_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_CHAR_CREATE_ELE> vect_LogInfo;
	vector <unsigned int> vect_PartID;
	vector <string> vect_PartName;
	char strInt[30];
	char *retMessage = new char[255];
	ZeroMemory(retMessage, 255);

	try
	{		
		REQ_LOG_CHAR_CREATE		req_PartLog_info;				//��ɫ������¼��ѯ����
		REP_LOG_CHAR_CREATE		rep_PartLog_info;				//��ɫ������¼��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "��ɫ������¼��ѯ");
		}
		else
		{
			sprintf(retMessage, "��ɫ������¼: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_CHAR_CREATE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "��ɫ������¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//��������
								if (rep_PartLog_info.all_send == 1)
								{
									bRecvFlag = false;

									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_PartID.push_back(rep_PartLog_info.char_id.charIndex);
											vect_PartName.push_back(rep_PartLog_info.char_name.charName);
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_PartID.push_back(rep_PartLog_info.char_id.charIndex);
											vect_PartName.push_back(rep_PartLog_info.char_name.charName);
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ��ɫID
					_itoa((int)vect_PartID[i], strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��ɫ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = vect_PartName[i].length();
					sprintf((char *)&m_tflv.lpdata, "%s", vect_PartName[i].c_str());
					DBTFLV.push_back(m_tflv);
					
					// ��ɫ����ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);

					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "��ɫ������¼��ѯ");
	}

	delete []retMessage;

	return DBTFLV;
}

//��ɫ��־:��ɫɾ����¼��ѯ
vector <CGlobalStruct::TFLV> CWAInfo::WA_Character_Delete_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_CHAR_DELETE_ELE> vect_LogInfo;
	vector <unsigned int> vect_PartID;
	vector <string> vect_PartName;
	char strInt[30];
	char *retMessage = new char[255];
	ZeroMemory(retMessage, 255);

	try
	{		
		REQ_LOG_CHAR_DELETE		req_PartLog_info;				//��ɫɾ����¼��ѯ����
		REP_LOG_CHAR_DELETE		rep_PartLog_info;				//��ɫɾ����¼��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "��ɫɾ����¼��ѯ");
		}
		else
		{
			sprintf(retMessage, "��ɫɾ����¼: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_CHAR_DELETE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "��ɫɾ����¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//��������
								if (rep_PartLog_info.all_send == 1)
								{
									bRecvFlag = false;

									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_PartID.push_back(rep_PartLog_info.char_id.charIndex);
											vect_PartName.push_back(rep_PartLog_info.char_name.charName);
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_PartID.push_back(rep_PartLog_info.char_id.charIndex);
											vect_PartName.push_back(rep_PartLog_info.char_name.charName);
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ��ɫID
					_itoa((int)vect_PartID[i], strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��ɫ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = vect_PartName[i].length();
					sprintf((char *)&m_tflv.lpdata, "%s", vect_PartName[i].c_str());
					DBTFLV.push_back(m_tflv);
					
					// ɾ��ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// ɾ��ʱ�Ľ�ɫ�ȼ�
					_itoa((int)vect_LogInfo[i].char_level, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_Level;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt);
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "��ɫɾ����¼��ѯ");
	}

	delete []retMessage;

	return DBTFLV;
}

//��ɫ��־:������
vector <CGlobalStruct::TFLV> CWAInfo::WA_Join_Guild_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_CHAR_JOIN_GUILD_ELE> vect_LogInfo;
	vector <unsigned int> vect_PartID;
	vector <string> vect_PartName;
	char strInt[30];
	char *retMessage = new char[255];
	ZeroMemory(retMessage, 255);

	try
	{		
		REQ_LOG_CHAR_JOIN_GUILD		req_PartLog_info;				//�������¼��ѯ����
		REP_LOG_CHAR_JOIN_GUILD		rep_PartLog_info;				//�������¼��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "�������¼��ѯ");
		}
		else
		{
			sprintf(retMessage, "������: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_CHAR_JOIN_GUILD);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "�������¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//��������
								if (rep_PartLog_info.all_send == 1)
								{
									bRecvFlag = false;

									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_PartID.push_back(rep_PartLog_info.char_id.charIndex);
											vect_PartName.push_back(rep_PartLog_info.char_name.charName);
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_PartID.push_back(rep_PartLog_info.char_id.charIndex);
											vect_PartName.push_back(rep_PartLog_info.char_name.charName);
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ��ɫID
					_itoa((int)vect_PartID[i], strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��ɫ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = vect_PartName[i].length() ;
					sprintf((char *)&m_tflv.lpdata, "%s", vect_PartName[i].c_str());
					DBTFLV.push_back(m_tflv);
					
					// ����ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// ��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GuildName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].guild_name.guildName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].guild_name.guildName);
					DBTFLV.push_back(m_tflv);

					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "�������¼��ѯ");
	}

	delete []retMessage;

	return DBTFLV;
}

//��ɫ��־:�뿪���
vector <CGlobalStruct::TFLV> CWAInfo::WA_Quit_Guild_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_CHAR_LEAVE_GUILD_ELE> vect_LogInfo;
	vector <unsigned int> vect_PartID;
	vector <string> vect_PartName;
	char strInt[30];
	char *retMessage = new char[255];
	ZeroMemory(retMessage, 255);

	try
	{		
		REQ_LOG_CHAR_LEAVE_GUILD		req_PartLog_info;				//�뿪����¼��ѯ����
		REP_LOG_CHAR_LEAVE_GUILD		rep_PartLog_info;				//�뿪����¼��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "�뿪����¼��ѯ");
		}
		else
		{
			sprintf(retMessage, "�뿪���: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_CHAR_LEAVE_GUILD);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "�뿪����¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//��������
								if (rep_PartLog_info.all_send == 1)
								{
									bRecvFlag = false;

									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_PartID.push_back(rep_PartLog_info.char_id.charIndex);
											vect_PartName.push_back(rep_PartLog_info.char_name.charName);
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_PartID.push_back(rep_PartLog_info.char_id.charIndex);
											vect_PartName.push_back(rep_PartLog_info.char_name.charName);
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ��ɫID
					_itoa((int)vect_PartID[i], strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��ɫ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = vect_PartName[i].length();
					sprintf((char *)&m_tflv.lpdata, "%s", vect_PartName[i].c_str());
					DBTFLV.push_back(m_tflv);
					
					// �뿪ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// ��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GuildName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].guild_name.guildName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].guild_name.guildName);
					DBTFLV.push_back(m_tflv);

					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "�뿪����¼��ѯ");
	}

	delete []retMessage;

	return DBTFLV;
}

//��ɫ��־:�������
vector <CGlobalStruct::TFLV> CWAInfo::WA_Create_Guild_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_CHAR_CREATE_GUILD_ELE> vect_LogInfo;
	vector <unsigned int> vect_PartID;
	vector <string> vect_PartName;
	char strInt[30];
	char *retMessage = new char[255];
	ZeroMemory(retMessage, 255);

	try
	{		
		REQ_LOG_CHAR_CREATE_GUILD		req_PartLog_info;				//��������¼��ѯ����
		REP_LOG_CHAR_CREATE_GUILD		rep_PartLog_info;				//��������¼��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "��������¼��ѯ");
		}
		else
		{
			sprintf(retMessage, "�������: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_CHAR_CREATE_GUILD);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "��������¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//��������
								if (rep_PartLog_info.all_send == 1)
								{
									bRecvFlag = false;

									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_PartID.push_back(rep_PartLog_info.char_id.charIndex);
											vect_PartName.push_back(rep_PartLog_info.char_name.charName);
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_PartID.push_back(rep_PartLog_info.char_id.charIndex);
											vect_PartName.push_back(rep_PartLog_info.char_name.charName);
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ��ɫID
					_itoa((int)vect_PartID[i], strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��ɫ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = vect_PartName[i].length();
					sprintf((char *)&m_tflv.lpdata, "%s", vect_PartName[i].c_str());
					DBTFLV.push_back(m_tflv);
					
					// ����ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);	
					
					//��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_OpType;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch(vect_LogInfo[i].create_type)
					{
					case 0:
						m_tflv.m_tvlength = strlen("��������") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "��������");
						break;
					case 1:
						m_tflv.m_tvlength = strlen("�����ɹ�") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "�����ɹ�");
						break;
					case 2:
						m_tflv.m_tvlength = strlen("����ʧ��") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "����ʧ��");
						break;
					default:
						break;
					}
					DBTFLV.push_back(m_tflv);

					// ����ID
					_itoa((int)vect_LogInfo[i].guild_id.guildIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GuildID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GuildName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].guild_name.guildName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].guild_name.guildName);
					DBTFLV.push_back(m_tflv);

					// ������ƷID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemTypeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ������Ʒ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);

					// ���Ľ�Ǯ
					_itoa((int)vect_LogInfo[i].cosume_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "��������¼��ѯ");
	}

	delete []retMessage;
	return DBTFLV;
}

//��ɫ��־:��ɢ���
vector <CGlobalStruct::TFLV> CWAInfo::WA_Dissolve_Guild_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_CHAR_DISMISS_GUILD_ELE> vect_LogInfo;
	vector <unsigned int> vect_PartID;
	vector <string> vect_PartName;
	char strInt[30];
	char *retMessage = new char[255];
	ZeroMemory(retMessage, 255);

	try
	{		
		REQ_LOG_CHAR_DISMISS_GUILD		req_PartLog_info;				//��ɢ����¼��ѯ����
		REP_LOG_CHAR_DISMISS_GUILD		rep_PartLog_info;				//��ɢ����¼��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "��ɢ����¼��ѯ");
		}
		else
		{
			sprintf(retMessage, "��ɢ���: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_CHAR_DISMISS_GUILD);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "��ɢ����¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//��������
								if (rep_PartLog_info.all_send == 1)
								{
									bRecvFlag = false;

									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_PartID.push_back(rep_PartLog_info.char_id.charIndex);
											vect_PartName.push_back(rep_PartLog_info.char_name.charName);
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_PartID.push_back(rep_PartLog_info.char_id.charIndex);
											vect_PartName.push_back(rep_PartLog_info.char_name.charName);
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ��ɫID
					_itoa((int)vect_PartID[i], strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��ɫ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = vect_PartName[i].length();
					sprintf((char *)&m_tflv.lpdata, "%s", vect_PartName[i].c_str());
					DBTFLV.push_back(m_tflv);
					
					// ��ɢʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// ��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GuildName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].guild_name.guildName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].guild_name.guildName);
					DBTFLV.push_back(m_tflv);

					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "��ɢ����¼��ѯ");
	}

	delete []retMessage;
	return DBTFLV;
}

//��ɫ��־:תְ��Ϣ��ѯ
vector <CGlobalStruct::TFLV> CWAInfo::WA_Transfer_Info_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_CHAR_CHANGE_OCC_ELE> vect_LogInfo;
	vector <unsigned int> vect_PartID;
	vector <string> vect_PartName;
	char strInt[30];
	char *retMessage = new char[255];
	ZeroMemory(retMessage, 255);

	try
	{		
		REQ_LOG_CHAR_CHANGE_OCC		req_PartLog_info;				//תְ��Ϣ��ѯ����
		REP_LOG_CHAR_CHANGE_OCC		rep_PartLog_info;				//תְ��Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "תְ��Ϣ��ѯ");
		}
		else
		{
			sprintf(retMessage, "תְ��Ϣ��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_CHAR_CHANGE_OCC);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "תְ��Ϣ��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//��������
								if (rep_PartLog_info.all_send == 1)
								{
									bRecvFlag = false;

									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_PartID.push_back(rep_PartLog_info.char_id.charIndex);
											vect_PartName.push_back(rep_PartLog_info.char_name.charName);
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_PartID.push_back(rep_PartLog_info.char_id.charIndex);
											vect_PartName.push_back(rep_PartLog_info.char_name.charName);
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ��ɫID
					_itoa((int)vect_PartID[i], strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��ɫ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = vect_PartName[i].length();
					sprintf((char *)&m_tflv.lpdata, "%s", vect_PartName[i].c_str());
					DBTFLV.push_back(m_tflv);
					
					// תְʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// תְǰְҵ
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_Profession;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch(vect_LogInfo[i].old_occupation)
					{
					case 1:
						m_tflv.m_tvlength = strlen("��ʿ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "��ʿ");
						break;
					case 2:
						m_tflv.m_tvlength = strlen("����") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "����");
						break;
					case 3:
						m_tflv.m_tvlength = strlen("��ʦ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "��ʦ");
						break;
					case 4:
						m_tflv.m_tvlength = strlen("ħ��ʿ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "ħ��ʿ");
						break;
					case 5:
						m_tflv.m_tvlength = strlen("����ʿ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "����ʿ");
						break;
					case 6:
						m_tflv.m_tvlength = strlen("���ʦ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "���ʦ");
						break;
					case 7:
						m_tflv.m_tvlength = strlen("��սʿ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "��սʿ");
						break;
					case 8:
						m_tflv.m_tvlength = strlen("������") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "������");
						break;
					case 9:
						m_tflv.m_tvlength = strlen("Ԫ��ʹ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "Ԫ��ʹ");
						break;
					case 10:
						m_tflv.m_tvlength = strlen("ʥսʿ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "ʥսʿ");
						break;
					case 11:
						m_tflv.m_tvlength = strlen("�����") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "�����");
						break;
					case 12:
						m_tflv.m_tvlength = strlen("���") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "���");
						break;
					case 13:
						m_tflv.m_tvlength = strlen("������ʦ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "������ʦ");
						break;
					case 14:
						m_tflv.m_tvlength = strlen("�ֱ���ʦ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "�ֱ���ʦ");
						break;
					case 15:
						m_tflv.m_tvlength = strlen("ħ����ʦ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "ħ����ʦ");
						break;
					case 16:
						m_tflv.m_tvlength = strlen("������ʦ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "������ʦ");
						break;
					default:
						m_tflv.m_tvlength = strlen("δ֪") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "δ֪");
						break;
					}
					DBTFLV.push_back(m_tflv);

					// תְ��ְҵ
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NewProfession;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch(vect_LogInfo[i].new_occupation)
					{
					case 1:
						m_tflv.m_tvlength = strlen("��ʿ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "��ʿ");
						break;
					case 2:
						m_tflv.m_tvlength = strlen("����") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "����");
						break;
					case 3:
						m_tflv.m_tvlength = strlen("��ʦ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "��ʦ");
						break;
					case 4:
						m_tflv.m_tvlength = strlen("ħ��ʿ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "ħ��ʿ");
						break;
					case 5:
						m_tflv.m_tvlength = strlen("����ʿ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "����ʿ");
						break;
					case 6:
						m_tflv.m_tvlength = strlen("���ʦ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "���ʦ");
						break;
					case 7:
						m_tflv.m_tvlength = strlen("��սʿ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "��սʿ");
						break;
					case 8:
						m_tflv.m_tvlength = strlen("������") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "������");
						break;
					case 9:
						m_tflv.m_tvlength = strlen("Ԫ��ʹ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "Ԫ��ʹ");
						break;
					case 10:
						m_tflv.m_tvlength = strlen("ʥսʿ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "ʥսʿ");
						break;
					case 11:
						m_tflv.m_tvlength = strlen("�����") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "�����");
						break;
					case 12:
						m_tflv.m_tvlength = strlen("���") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "���");
						break;
					case 13:
						m_tflv.m_tvlength = strlen("������ʦ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "������ʦ");
						break;
					case 14:
						m_tflv.m_tvlength = strlen("�ֱ���ʦ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "�ֱ���ʦ");
						break;
					case 15:
						m_tflv.m_tvlength = strlen("ħ����ʦ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "ħ����ʦ");
						break;
					case 16:
						m_tflv.m_tvlength = strlen("������ʦ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "������ʦ");
						break;
					default:
						m_tflv.m_tvlength = strlen("δ֪") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "δ֪");
						break;
					}
					DBTFLV.push_back(m_tflv);

					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "תְ��Ϣ��ѯ");
	}

	delete []retMessage;

	return DBTFLV;
}

//��ɫ��־:������־��¼
vector <CGlobalStruct::TFLV> CWAInfo::WA_Skills_Log_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_CHAR_SKILL_ELE> vect_LogInfo;
	char strInt[30];
	char *retMessage = new char[255];
	ZeroMemory(retMessage, 255);

	try
	{		
		REQ_LOG_CHAR_SKILL		req_PartLog_info;				//������־��¼����
		REP_LOG_CHAR_SKILL		rep_PartLog_info;				//������־��¼��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "������־��¼��ѯ");
		}
		else
		{
			sprintf(retMessage, "������־��¼: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_CHAR_SKILL);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "������־��¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//��������
								if (rep_PartLog_info.all_send == 1)
								{
									bRecvFlag = false;

									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ��������
// 					m_tflv.nIndex = DBTFLV.size()+1;
// 					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
// 					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
// 					switch(vect_LogInfo[i].learn_type)
// 					{
// 					case REP_LOG_CHAR_SKILL_ELE::LEARN_TYPE::LT_LEARN_NEW:
// 						m_tflv.m_tvlength = strlen("ѧϰ����") + 1;
// 						sprintf((char *)&m_tflv.lpdata, "%s", "ѧϰ����");
// 					case REP_LOG_CHAR_SKILL_ELE::LEARN_TYPE::LT_LEVELUP:
// 						m_tflv.m_tvlength = strlen("��������") + 1;
// 						sprintf((char *)&m_tflv.lpdata, "%s", "��������");
// 					}
// 					DBTFLV.push_back(m_tflv);
					
					// ����ID
					_itoa((int)vect_LogInfo[i].skill_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SkillID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SkillName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].skill_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].skill_name);
					DBTFLV.push_back(m_tflv);

					// ���ܵȼ�
					_itoa((int)vect_LogInfo[i].skill_level, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SkillLvl;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ѧϰ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_OpType;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch(vect_LogInfo[i].learn_type)
					{
					case REP_LOG_CHAR_SKILL_ELE::LEARN_TYPE::LT_LEARN_NEW:
						m_tflv.m_tvlength = strlen("ѧϰ����") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "ѧϰ����");
						break;
					case REP_LOG_CHAR_SKILL_ELE::LEARN_TYPE::LT_LEVELUP:
						m_tflv.m_tvlength = strlen("��������") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "��������");
						break;
					default:
						m_tflv.m_tvlength = strlen("δ֪") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "δ֪");
						break;
					}
					DBTFLV.push_back(m_tflv);

					// ���ľ�������
					_itoa((int)vect_LogInfo[i].cosume_exp, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_Exp;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ���Ľ�Ǯ
					_itoa((int)vect_LogInfo[i].cosume_meony, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ����ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "������־��¼��ѯ");
	}

	delete []retMessage;

	return DBTFLV;
}

//��ɫ��־:�������־��¼
vector <CGlobalStruct::TFLV> CWAInfo::WA_LifeSkills_Log_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_CHAR_LIVING_SKILL_ELE> vect_LogInfo;
	char strInt[30];
	char *retMessage = new char[255];
	ZeroMemory(retMessage, 255);

	try
	{		
		REQ_LOG_CHAR_LIVING_SKILL		req_PartLog_info;				//�������־��¼����
		REP_LOG_CHAR_LIVING_SKILL		rep_PartLog_info;				//�������־��¼��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "������־��¼��ѯ");
		}
		else
		{
			sprintf(retMessage, "�������־��¼: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_CHAR_LIVING_SKILL);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "�������־��¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//��������
								if (rep_PartLog_info.all_send == 1)
								{
									bRecvFlag = false;

									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_OpType;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch(vect_LogInfo[i].learn_type)
					{
					case 1:
						m_tflv.m_tvlength = strlen("����") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "����");
						break;
					case 2:
						m_tflv.m_tvlength = strlen("�÷�") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "�÷�");
						break;
					case 3:
						m_tflv.m_tvlength = strlen("�ֹ�") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "�ֹ�");
						break;
					case 4:
						m_tflv.m_tvlength = strlen("��ҩ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "��ҩ");
						break;
					case 5:
						m_tflv.m_tvlength = strlen("��ħ") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "��ħ");
						break;
					default:
						m_tflv.m_tvlength = strlen("δ֪") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "δ֪");
						break;
					}
					DBTFLV.push_back(m_tflv);

					// ���ܵȼ�
					_itoa((int)vect_LogInfo[i].skill_level, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SkillLvl;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ���ľ�������
					_itoa((int)vect_LogInfo[i].cosume_exp, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_Exp;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ���Ľ�Ǯ
					_itoa((int)vect_LogInfo[i].cosume_meony, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ����ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "�������־��¼��ѯ");
	}

	delete []retMessage;

	return DBTFLV;
}

//Ӷ������־:������¼
vector <CGlobalStruct::TFLV> CWAInfo::WA_CreateMercenary_Log_Query(char *ServerIP, char *GuildName, char *Begintime,char *Endtime, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_GUILD_CREATE_ELE> vect_LogInfo;
	char strInt[30];
	char *retMessage = new char[255];
	ZeroMemory(retMessage, 255);

	try
	{		
		REQ_LOG_GUILD_CREATE	req_PartLog_info;				//������¼����
		REP_LOG_GUILD_CREATE	rep_PartLog_info;			//������¼��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "Ӷ������־:������¼��ѯ");
		}
		else
		{
			sprintf(retMessage, "Ӷ������־:������¼: <����>%s, <��������>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, GuildName, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_GUILD_CREATE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_GUILDNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.guild_name, GuildName, strlen(GuildName)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "Ӷ������־:������¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//��������
								if (rep_PartLog_info.all_send == 1)
								{
									bRecvFlag = false;

									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ��ɫID
					_itoa((int)vect_LogInfo[i].char_id.charIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��ɫ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].char_name.charName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].char_name.charName);
					DBTFLV.push_back(m_tflv);
					
					//����ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);

					// ����ID
					_itoa((int)vect_LogInfo[i].guild_id.guildIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GuildID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GuildName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].guild_name.guildName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].guild_name.guildName);
					DBTFLV.push_back(m_tflv);
					
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "Ӷ������־:������¼��ѯ");
	}

	delete []retMessage;

	return DBTFLV;
}

//Ӷ������־:��ɢ��¼
vector <CGlobalStruct::TFLV> CWAInfo::WA_DissolveMercenary_Log_Query(char *ServerIP, char *GuildName, char *Begintime,char *Endtime, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_GUILD_DISMISS_ELE> vect_LogInfo;
	char strInt[30];
	char *retMessage = new char[255];
	ZeroMemory(retMessage, 255);

	try
	{		
		REQ_LOG_GUILD_DIMISS		req_PartLog_info;				//��ɢ��¼����
		REP_LOG_GUILD_DISMISS		rep_PartLog_info;				//��ɢ��¼��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "Ӷ������־:��ɢ��¼");
		}
		else
		{
			sprintf(retMessage, "Ӷ������־:��ɢ��¼: <����>%s, <��������>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, GuildName, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_GUILD_DIMISS);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_GUILDNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.guild_name, GuildName, strlen(GuildName)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "Ӷ������־:��ɢ��¼");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//��������
								if (rep_PartLog_info.all_send == 1)
								{
									bRecvFlag = false;

									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ��ɫID
					_itoa((int)vect_LogInfo[i].char_id.charIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��ɫ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].char_name.charName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].char_name.charName);
					DBTFLV.push_back(m_tflv);
					
					//��ɢʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					// ����ID
					_itoa((int)vect_LogInfo[i].guild_id.guildIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GuildID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GuildName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].guild_name.guildName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].guild_name.guildName);
					DBTFLV.push_back(m_tflv);
					
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "Ӷ������־:��ɢ��¼");
	}

	delete []retMessage;

	return DBTFLV;
}

//���������־:���ﾫ�겶׽��¼
vector <CGlobalStruct::TFLV> CWAInfo::WA_CatchPetSpirit_Log_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_PET_CATCH_ELE> vect_LogInfo;
	vector <unsigned int> vect_PartID;
	vector <string> vect_PartName;
	char strInt[30];
	char *retMessage = new char[255];
	ZeroMemory(retMessage, 255);

	try
	{		
		REQ_LOG_PET_CATCH		req_PartLog_info;				//���ﾫ�겶׽��¼����
		REP_LOG_PET_CATCH		rep_PartLog_info;				//���ﾫ�겶׽��¼��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "���ﾫ�겶׽��¼��ѯ");
		}
		else
		{
			sprintf(retMessage, "���ﾫ�겶׽��¼��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_PET_CATCH);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "���ﾫ�겶׽��¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//��������
								if (rep_PartLog_info.all_send == 1)
								{
									bRecvFlag = false;

									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_PartID.push_back(rep_PartLog_info.char_id.charIndex);
											vect_PartName.push_back(rep_PartLog_info.char_name.charName);
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_PartID.push_back(rep_PartLog_info.char_id.charIndex);
											vect_PartName.push_back(rep_PartLog_info.char_name.charName);
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ��ɫID
					_itoa((int)vect_PartID[i], strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��ɫ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = vect_PartName[i].length();
					sprintf((char *)&m_tflv.lpdata, "%s", vect_PartName[i].c_str());
					DBTFLV.push_back(m_tflv);

					//��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_petName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].pet_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].pet_name);
					DBTFLV.push_back(m_tflv);
					
					// ��׽ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "���ﾫ�겶׽��¼��ѯ");
	}

	delete []retMessage;

	return DBTFLV;
}

//���������־
//���꽻�׼�¼:���׻��
vector <CGlobalStruct::TFLV> CWAInfo::WA_GainSpirit_Log_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_PET_GET_TRADE_ELE> vect_LogInfo;
	vector <unsigned int> vect_PartID;
	vector <string> vect_PartName;
	char strInt[30];
	char *retMessage = new char[255];
	ZeroMemory(retMessage, 255);

	try
	{		
		REQ_LOG_PET_GET_TRADE		req_PartLog_info;				//���׻�ü�¼����
		REP_LOG_PET_GET_TRADE		rep_PartLog_info;				//���׻�ü�¼��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "���꽻�׼�¼:���׻�ü�¼��ѯ");
		}
		else
		{
			sprintf(retMessage, "���꽻�׼�¼:���׻�ü�¼��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_PET_GET_TRADE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "���꽻�׼�¼:���׻�ü�¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//��������
								if (rep_PartLog_info.all_send == 1)
								{
									bRecvFlag = false;

									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_PartID.push_back(rep_PartLog_info.char_id.charIndex);
											vect_PartName.push_back(rep_PartLog_info.char_name.charName);
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_PartID.push_back(rep_PartLog_info.char_id.charIndex);
											vect_PartName.push_back(rep_PartLog_info.char_name.charName);
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ��ɫID
					_itoa((int)vect_PartID[i], strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��ɫ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = vect_PartName[i].length();
					sprintf((char *)&m_tflv.lpdata, "%s", vect_PartName[i].c_str());
					DBTFLV.push_back(m_tflv);

					//�Է��ǳ�
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcCharName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].src_char_name.charName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].src_char_name.charName);
					DBTFLV.push_back(m_tflv);
					
					//�����Ʒ
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_RewardItemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);

					// ����ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "���꽻�׼�¼:���׻�ü�¼��ѯ");
	}

	delete []retMessage;

	return DBTFLV;
}

//���������־
//���꽻�׼�¼:����ʧȥ
vector <CGlobalStruct::TFLV> CWAInfo::WA_LoseSpirit_Log_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_PET_LOSE_TRADE_ELE> vect_LogInfo;
	vector <unsigned int> vect_PartID;
	vector <string> vect_PartName;
	char strInt[30];
	char *retMessage = new char[255];
	ZeroMemory(retMessage, 255);

	try
	{		
		REQ_LOG_PET_LOSE_TRADE		req_PartLog_info;				//����ʧȥ��¼����
		REP_LOG_PET_LOSE_TRADE		rep_PartLog_info;				//����ʧȥ��¼��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "���꽻�׼�¼:����ʧȥ��¼��ѯ");
		}
		else
		{
			sprintf(retMessage, "���꽻�׼�¼:����ʧȥ��¼��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_PET_LOSE_TRADE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "���꽻�׼�¼:����ʧȥ��¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//��������
								if (rep_PartLog_info.all_send == 1)
								{
									bRecvFlag = false;

									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_PartID.push_back(rep_PartLog_info.char_id.charIndex);
											vect_PartName.push_back(rep_PartLog_info.char_name.charName);
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_PartID.push_back(rep_PartLog_info.char_id.charIndex);
											vect_PartName.push_back(rep_PartLog_info.char_name.charName);
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ��ɫID
					_itoa((int)vect_PartID[i], strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��ɫ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = vect_PartName[i].length();
					sprintf((char *)&m_tflv.lpdata, "%s", vect_PartName[i].c_str());
					DBTFLV.push_back(m_tflv);

					//�Է��ǳ�
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcCharName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].dst_char_name.charName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].dst_char_name.charName);
					DBTFLV.push_back(m_tflv);
					
					//�����Ʒ
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GiveItemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);

					// ����ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "���꽻�׼�¼:����ʧȥ��¼��ѯ");
	}

	delete []retMessage;

	return DBTFLV;
}

//���������־:������ۼ�¼
vector <CGlobalStruct::TFLV> CWAInfo::WA_SellSpirit_Log_Query(char *ServerIP, char *Account, char *Character,char *Begintime,char *Endtime, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <REP_LOG_PET_BOOTH_ELE> vect_LogInfo;
	vector <unsigned int> vect_PartID;
	vector <string> vect_PartName;
	char strInt[30];
	char *retMessage = new char[255];
	ZeroMemory(retMessage, 255);

	try
	{		
		REQ_LOG_PET_BOOTH		req_PartLog_info;				//������ۼ�¼����
		REP_LOG_PET_BOOTH		rep_PartLog_info;				//������ۼ�¼��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "������ۼ�¼");
		}
		else
		{
			sprintf(retMessage, "������ۼ�¼��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s, <��ʼʱ��>%s, <����ʱ��>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_LOG_PET_BOOTH);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//��ʼʱ�䣬����ʱ��
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "������ۼ�¼��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//��������
								if (rep_PartLog_info.all_send == 1)
								{
									bRecvFlag = false;

									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_PartID.push_back(rep_PartLog_info.char_id.charIndex);
											vect_PartName.push_back(rep_PartLog_info.char_name.charName);
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_PartID.push_back(rep_PartLog_info.char_id.charIndex);
											vect_PartName.push_back(rep_PartLog_info.char_name.charName);
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ��ɫID
					_itoa((int)vect_PartID[i], strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��ɫ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = vect_PartName[i].length();
					sprintf((char *)&m_tflv.lpdata, "%s", vect_PartName[i].c_str());
					DBTFLV.push_back(m_tflv);

					//�Է��ǳ�
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcCharName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].dst_char_name.charName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].dst_char_name.charName);
					DBTFLV.push_back(m_tflv);
					
					//�����Ʒ
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GiveItemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);

					// ����ʱ��
// 					m_tflv.nIndex = DBTFLV.size()+1;
// 					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
// 					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
// 						vect_LogInfo[i].date_time.year,
// 						vect_LogInfo[i].date_time.month,
// 						vect_LogInfo[i].date_time.day,
// 						vect_LogInfo[i].date_time.hour,
// 						vect_LogInfo[i].date_time.minute,
// 						vect_LogInfo[i].date_time.second);
// 					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
// 					DBTFLV.push_back(m_tflv);
					
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "������ۼ�¼��ѯ");
	}

	delete []retMessage;

	return DBTFLV;
}

//�û���Ϣ��ѯ:���ܲ�ѯ
vector <CGlobalStruct::TFLV> CWAInfo::WA_SkillsInfo_Log_Query(char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <CHARACTER_SKILL_INFO_ITEM> vect_LogInfo;
	char strInt[30];
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{		
		REQ_CHARACTER_INFO req_Log_info;				//������Ϣ��ѯ����
		REP_CHARACTER_SKILL_INFO rep_Log_info;			//������Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "���ϼ�����Ϣ��ѯ");
		}
		else
		{
			sprintf(retMessage, "���ϼ�����Ϣ��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s", ServerIP, Account, Character);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_CHARACTER_INFO);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			//����
			req_Log_info.srv_realm_id=srv_Realmid;
			//��������ID
			req_Log_info.srv_cluster_id=srv_ClusterID;
			req_Log_info.req_type = REQ_CHARACTER_INFO::RT_CHAR_SKILL_INFO;
			req_Log_info.query_condition = ADMIN::QC_CHARACTERNAME;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
				if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "������ϼ��ܲ�ѯ");
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
 					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&rep_Log_info, recvbuf+offset+4, packetSize);
								//��������
								if (rep_Log_info.allSend == 1)	//�յ����һ���ṹ��
								{
									bRecvFlag = false;

									if (rep_Log_info.amount>0)
									{
										for (int i=0; i<rep_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(rep_Log_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (rep_Log_info.amount>0)
									{
										for (int i=0; i<rep_Log_info.amount; i++)
										{
											vect_LogInfo.push_back(rep_Log_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iFriendInfoLen = vect_LogInfo.size();

				if ( iFriendInfoLen%iPageSize == 0 )
				{
					iPageCount = iFriendInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iFriendInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iFriendInfoLen && i<iPageSize*iIndex; i++ )
				{
					// ����ID
					_itoa((int)vect_LogInfo[i].skillId, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SkillID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SkillName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].skillName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].skillName);
					DBTFLV.push_back(m_tflv);

					//���ܵȼ�
					_itoa((int)vect_LogInfo[i].skillLevel, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SkillLvl;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);										
				}

			}

			m_MySocket.Close();
		}

		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "���ϼ�����Ϣ��ѯ");
	}

	return DBTFLV;
}

//�û���Ϣ��ѯ:�����ѯ
vector <CGlobalStruct::TFLV> CWAInfo::WA_MissionsCompleted_Log_Query(char *ServerIP, char *Account, char *Character, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <CHARACTER_QUEST_INFO> vect_LogInfo;
	char strInt[30];
	char *retMessage = new char[255];
	ZeroMemory(retMessage, 255);
	
	try
	{		
		REQ_QUERY_QUEST_INFO		req_PartLog_info;				//�����ѯ����
		REP_QUERY_QUEST_INFO		rep_PartLog_info;				//�����ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "��ɫ������Ϣ��ѯ");
		}
		else
		{
			sprintf(retMessage, "��ɫ������Ϣ��ѯ: <����>%s, <�ʺ�>%s, <��ɫ��>%s", ServerIP, Account, Character);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_QUERY_QUEST_INFO);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�

			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "��ɫ������Ϣ��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//��������
//								if (rep_PartLog_info.all_send == 1)
								{
									bRecvFlag = false;

									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}

									break;
								}
//								else
								{
									if (rep_PartLog_info.amount>0)
									{
										for (int i=0; i<rep_PartLog_info.amount; i++)
										{
											vect_LogInfo.push_back(rep_PartLog_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					//�������״̬
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_OpType;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch(vect_LogInfo[i].quest_type)
					{
					case CHARACTER_QUEST_INFO::CHARACTER_QUEST_TYPE::CQT_CURRENT:
						m_tflv.m_tvlength = strlen("�ѽ�") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "�ѽ�");
						break;
					case CHARACTER_QUEST_INFO::CHARACTER_QUEST_TYPE::CQT_FINISHED:
						m_tflv.m_tvlength = strlen("�����") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "�����");
						break;
					default:
						m_tflv.m_tvlength = strlen("δ֪") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "δ֪");
						break;
					}
					DBTFLV.push_back(m_tflv);

					// ����ID
					_itoa((int)vect_LogInfo[i].quest_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_QuestId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_QuestName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].quest_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].quest_name);
					DBTFLV.push_back(m_tflv);

					// ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].date_time.year,
						vect_LogInfo[i].date_time.month,
						vect_LogInfo[i].date_time.day,
						vect_LogInfo[i].date_time.hour,
						vect_LogInfo[i].date_time.minute,
						vect_LogInfo[i].date_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "��ɫ������Ϣ��ѯ");
	}

	delete []retMessage;

	return DBTFLV;
}

//Ӷ������Ϣ��ѯ
vector <CGlobalStruct::TFLV> CWAInfo::WA_MercenaryGroup_Query(char *ServerIP, char *MercenaryGroup_Name, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <GUILD_INFO_ELE> vect_LogInfo;
	char strInt[30];
	char *retMessage = new char[255];
	ZeroMemory(retMessage, 255);
	
	try
	{		
		REQ_QUERY_GUILD_INFO		req_Guild_info;				//Ӷ������Ϣ��ѯ����
		REP_QUERY_GUILD_INFO		rep_Guild_info;				//Ӷ������Ϣ��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "Ӷ������Ϣ��ѯ");
		}
		else
		{
			sprintf(retMessage, "Ӷ������Ϣ��ѯ: <����>%s, <������>%s", ServerIP, MercenaryGroup_Name);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_QUERY_GUILD_INFO);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�

			req_Guild_info.srv_realm_id=srv_Realmid;
			req_Guild_info.srv_cluster_id=srv_ClusterID;
			if(!strcmp(MercenaryGroup_Name, ""))
				req_Guild_info.query_type = REQ_QUERY_GUILD_INFO::ALL_GUILD;
			else
				req_Guild_info.query_type = REQ_QUERY_GUILD_INFO::ONE_GUILD;
			memcpy(&req_Guild_info.guld_name, MercenaryGroup_Name, strlen(MercenaryGroup_Name)+1);
			memcpy(sendbuf+4, &req_Guild_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "Ӷ������Ϣ��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&rep_Guild_info, recvbuf+offset+4, packetSize);

								//��������
								if (rep_Guild_info.allSend == 1)
								{
									bRecvFlag = false;

									if (rep_Guild_info.amount>0)
									{
										for (int i=0; i<rep_Guild_info.amount; i++)
										{
											vect_LogInfo.push_back(rep_Guild_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (rep_Guild_info.amount>0)
									{
										for (int i=0; i<rep_Guild_info.amount; i++)
										{
											vect_LogInfo.push_back(rep_Guild_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					//�������״̬
// 					m_tflv.nIndex = DBTFLV.size()+1;
// 					m_tflv.m_tagName = CEnumCore::TagName::WA_OpType;
// 					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					switch(vect_LogInfo[i].quest_type)
// 					{
// 					case CHARACTER_QUEST_INFO::CHARACTER_QUEST_TYPE::CQT_CURRENT:
// 						m_tflv.m_tvlength = strlen("δ���") + 1;
// 						sprintf((char *)&m_tflv.lpdata, "%s", "δ���");
// 						break;
// 					case CHARACTER_QUEST_INFO::CHARACTER_QUEST_TYPE::CQT_FINISHED:
// 						m_tflv.m_tvlength = strlen("�����") + 1;
// 						sprintf((char *)&m_tflv.lpdata, "%s", "�����");
// 						break;
// 					default:
// 						m_tflv.m_tvlength = strlen("δ֪") + 1;
// 						sprintf((char *)&m_tflv.lpdata, "%s", "δ֪");
// 						break;
// 					}
// 					DBTFLV.push_back(m_tflv);

					// ����ID
					_itoa((int)vect_LogInfo[i].guild_id.guildIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GuildID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GuildName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].guild_name.guildName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].guild_name.guildName);
					DBTFLV.push_back(m_tflv);

					// ����ȼ�
					_itoa((int)vect_LogInfo[i].guild_level, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GuildLevel;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ��������
					_itoa((int)vect_LogInfo[i].guild_member_count, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GuildMemCount;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// ����ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].found_time.year,
						vect_LogInfo[i].found_time.month,
						vect_LogInfo[i].found_time.day,
						vect_LogInfo[i].found_time.hour,
						vect_LogInfo[i].found_time.minute,
						vect_LogInfo[i].found_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "Ӷ������Ϣ��ѯ");
	}

	delete []retMessage;

	return DBTFLV;
}

//Ӷ���ų�Ա�б��ѯ
vector <CGlobalStruct::TFLV> CWAInfo::WA_MemberList_Info_Query(char *ServerIP, char *MercenaryGroup_Name, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <GUILD_MEMBER_INFO> vect_LogInfo;
	vector <unsigned int> vect_PartID;
	vector <string> vect_PartName;
	char strInt[30];
	char *retMessage = new char[255];
	ZeroMemory(retMessage, 255);
	
	try
	{		
		REQ_QUERY_GUILD_MEMBER		req_GuildmemList_info;				//Ӷ���ų�Ա�б��ѯ����
		REP_QUERY_GUILD_MEMBER		rep_GuildmemList_info;				//Ӷ���ų�Ա�б��ѯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "Ӷ���ų�Ա�б��ѯ");
		}
		else
		{
			sprintf(retMessage, "Ӷ���ų�Ա�б��ѯ: <����>%s, <������>%s", ServerIP, MercenaryGroup_Name);		
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			req_packetLength = sizeof(REQ_QUERY_GUILD_MEMBER);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�

			req_GuildmemList_info.srv_realm_id=srv_Realmid;
			req_GuildmemList_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_GuildmemList_info.guld_name, MercenaryGroup_Name, strlen(MercenaryGroup_Name)+1);
			memcpy(sendbuf+4, &req_GuildmemList_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "Ӷ���ų�Ա�б��ѯ");				
			}
			else
			{
				bool bRecvFlag = true;	//�Ƿ���Ҫ�����������ݰ�
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// ���ý��ջ�������С
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//ÿ���ṹ�����ݰ���
						int nLeft = recvbyte;		//���յ��ֽ���ƫ�ƺ�ʣ�µ��ֽ���
						int offset = 0;				//ָ��ƫ����
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//ȡ�����ݰ�
								memcpy(&rep_GuildmemList_info, recvbuf+offset+4, packetSize);

								//��������
								if (rep_GuildmemList_info.allSend == 1)
								{
									bRecvFlag = false;

									if (rep_GuildmemList_info.amount>0)
									{
										for (int i=0; i<rep_GuildmemList_info.amount; i++)
										{
											vect_PartID.push_back(rep_GuildmemList_info.guild_id.guildIndex);
											vect_PartName.push_back(rep_GuildmemList_info.guild_name.guildName);
											vect_LogInfo.push_back(rep_GuildmemList_info.elems[i]);
										}
									}

									break;
								}
								else
								{
									if (rep_GuildmemList_info.amount>0)
									{
										for (int i=0; i<rep_GuildmemList_info.amount; i++)
										{
											vect_PartID.push_back(rep_GuildmemList_info.guild_id.guildIndex);
											vect_PartName.push_back(rep_GuildmemList_info.guild_name.guildName);
											vect_LogInfo.push_back(rep_GuildmemList_info.elems[i]);
										}
									}									
								}
																
								offset += (packetSize + 4);
								nLeft -= (packetSize + 4);
							}
						}
					}
					else
					{
						break;
					}
				}

				//��������
				int iPageCount = 0;
				int iLogInfoLen = vect_LogInfo.size();

				if ( iLogInfoLen%iPageSize == 0 )
				{
					iPageCount = iLogInfoLen/iPageSize;
				}
				else
				{
					iPageCount = iLogInfoLen/iPageSize + 1;
				}
				
				for ( int i=iPageSize*(iIndex-1); i<iLogInfoLen && i<iPageSize*iIndex; i++ )
				{
					//�������״̬
// 					m_tflv.nIndex = DBTFLV.size()+1;
// 					m_tflv.m_tagName = CEnumCore::TagName::WA_OpType;
// 					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					switch(vect_LogInfo[i].quest_type)
// 					{
// 					case CHARACTER_QUEST_INFO::CHARACTER_QUEST_TYPE::CQT_CURRENT:
// 						m_tflv.m_tvlength = strlen("δ���") + 1;
// 						sprintf((char *)&m_tflv.lpdata, "%s", "δ���");
// 						break;
// 					case CHARACTER_QUEST_INFO::CHARACTER_QUEST_TYPE::CQT_FINISHED:
// 						m_tflv.m_tvlength = strlen("�����") + 1;
// 						sprintf((char *)&m_tflv.lpdata, "%s", "�����");
// 						break;
// 					default:
// 						m_tflv.m_tvlength = strlen("δ֪") + 1;
// 						sprintf((char *)&m_tflv.lpdata, "%s", "δ֪");
// 						break;
// 					}
// 					DBTFLV.push_back(m_tflv);

					// ��ɫID
					_itoa((int)vect_LogInfo[i].char_id.charIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// ��ɫ����
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].char_name.charName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].char_name.charName);
					DBTFLV.push_back(m_tflv);

					// ְ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MemLevel;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch(vect_LogInfo[i].membership_level)
					{
					case ADMIN::GJ_MASTER:
						m_tflv.m_tvlength = strlen("�᳤") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "�᳤");
						break;
					case ADMIN::GJ_NORMAL_MEMBER:
						m_tflv.m_tvlength = strlen("һ���Ա") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "һ���Ա");
						break;
					default:
						m_tflv.m_tvlength = strlen("δ֪") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "δ֪");
						break;
					}
					DBTFLV.push_back(m_tflv);

					// ��������
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GuildName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(MercenaryGroup_Name);
					sprintf((char *)&m_tflv.lpdata, "%s", MercenaryGroup_Name);
					DBTFLV.push_back(m_tflv);

					// ʱ��
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetTime;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					sprintf((char *)&m_tflv.lpdata, "%04d-%02d-%02d %02d:%02d:%02d", 
						vect_LogInfo[i].join_time.year,
						vect_LogInfo[i].join_time.month,
						vect_LogInfo[i].join_time.day,
						vect_LogInfo[i].join_time.hour,
						vect_LogInfo[i].join_time.minute,
						vect_LogInfo[i].join_time.second);
					m_tflv.m_tvlength = strlen((char *)m_tflv.lpdata)+1;
					DBTFLV.push_back(m_tflv);
					
					//PageCount
					_itoa(iPageCount,strInt,10);
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::PageCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);	
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "Ӷ���ų�Ա�б��ѯ");
	}

	delete []retMessage;

	return DBTFLV;
}


//������:
//�������:��ӳ���(�ȼ�\����)
vector <CGlobalStruct::TFLV> CWAInfo::WA_PetAdded_Operater(int userByID, char * ServerIP, char* Character, int PetTypeID)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	try
	{
		
		REQ_ADD_PET req_PetAdded;		//��ӳ���
		REP_ADD_PET rep_PetAdded;		//��ӳ���
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼������ʧ��", "��ӳ���");	
		}
		else
		{
			sprintf(retMessage, "��ӳ���:<����>%s, <��ɫ��>%s,<��������ID>%d", ServerIP, Character,PetTypeID);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG
			
			req_packetLength = sizeof(REQ_ADD_PET);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			memcpy(&req_PetAdded.char_name, Character, strlen(Character)+1);
			//����
			req_PetAdded.srv_realm_id=srv_Realmid;
			req_PetAdded.srv_cluster_id=srv_ClusterID;
			req_PetAdded.pet_type_id = PetTypeID;
			memcpy(sendbuf+4, &req_PetAdded, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "��ӳ���");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				int recvbyte = m_MySocket.MyRecv(recvbuf,10240);
				if ( recvbyte>0 && strcmp(recvbuf, "") )
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_PetAdded, recvbuf+4, packetSize);
					switch (rep_PetAdded.eResult)
					{
					case REP_ADD_PET::E_SUCCESS:	//�ɹ�
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��ӳ���ɹ�", "��ӳ���");	
						break;
					default:						//ʧ��
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��ӳ���ʧ��", "��ӳ���");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��ӳ��ﳬʱ", "��ӳ���");
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "��ӳ���");		
	}
	
	m_OperatorWA.GmOperLog(userByID, ServerIP, "��ӳ���", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//�������:ɾ������(�ȼ�\����)
vector <CGlobalStruct::TFLV> CWAInfo::WA_PetDeleted_Operater(int userByID, char * ServerIP, char* Character, int PetUniqueID)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	try
	{
		
		REQ_REMOVE_PET req_PetDeleted;		//ɾ������
		REP_REMOVE_PET rep_PetDeleted;		//ɾ������
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼������ʧ��", "ɾ������");	
		}
		else
		{
			sprintf(retMessage, "ɾ������:<����>%s, <��ɫ��>%s,<����ΨһID>%d", ServerIP, Character,PetUniqueID);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG
			
			req_packetLength = sizeof(REQ_REMOVE_PET);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			memcpy(&req_PetDeleted.char_name, Character, strlen(Character)+1);
			//����
			req_PetDeleted.srv_realm_id=srv_Realmid;
			req_PetDeleted.srv_cluster_id=srv_ClusterID;
			req_PetDeleted.pet_unique_id = PetUniqueID;
			memcpy(sendbuf+4, &req_PetDeleted, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "ɾ������");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				int recvbyte = m_MySocket.MyRecv(recvbuf,10240);
				if ( recvbyte>0 && strcmp(recvbuf, "") )
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_PetDeleted, recvbuf+4, packetSize);
					switch (rep_PetDeleted.eResult)
					{
					case REP_REMOVE_PET::E_SUCCESS:	//�ɹ�
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "ɾ������ɹ�", "ɾ������");	
						break;
					default:						//ʧ��
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "ɾ������ʧ��", "ɾ������");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "ɾ�����ﳬʱ", "ɾ������");
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "ɾ������");		
	}
	
	m_OperatorWA.GmOperLog(userByID, ServerIP, "ɾ������", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//�������:ת�û᳤Ȩ��
vector <CGlobalStruct::TFLV> CWAInfo::WA_AttornPopedom_Operater(int userByID, char * ServerIP, char* GuildName, char * Character)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	try
	{
		
		REQ_GUILD_CHANGE_MASTER req_GuildChanged;		//ת�û᳤Ȩ��
		REP_GUILD_CHANGE_MASTER rep_GuildChanged;		//ת�û᳤Ȩ��
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼������ʧ��", "ת�û᳤Ȩ��");	
		}
		else
		{
			sprintf(retMessage, "ת�û᳤Ȩ��:<����>%s, <��ɫ��>%s,<��������>%s", ServerIP, Character,GuildName);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG
			
			req_packetLength = sizeof(REQ_GUILD_CHANGE_MASTER);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			memcpy(&req_GuildChanged.char_name, Character, strlen(Character)+1);
			memcpy(&req_GuildChanged.guld_name, GuildName, strlen(GuildName)+1);
			//����
			req_GuildChanged.srv_realm_id=srv_Realmid;
			req_GuildChanged.srv_cluster_id=srv_ClusterID;
			memcpy(sendbuf+4, &req_GuildChanged, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "ת�û᳤Ȩ��");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				int recvbyte = m_MySocket.MyRecv(recvbuf,10240);
				if ( recvbyte>0 && strcmp(recvbuf, "") )
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_GuildChanged, recvbuf+4, packetSize);
					switch (rep_GuildChanged.eResult)
					{
					case REP_GUILD_CHANGE_MASTER::E_SUCCESS:		//�ɹ�
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "ת�û᳤Ȩ�޳ɹ�", "ת�û᳤Ȩ��");	
						break;
					default:										//ʧ��
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "ת�û᳤Ȩ��ʧ��", "ת�û᳤Ȩ��");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "ת�û᳤Ȩ�޳�ʱ", "ת�û᳤Ȩ��");
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "ת�û᳤Ȩ��");		
	}
	
	m_OperatorWA.GmOperLog(userByID, ServerIP, "ת�û᳤Ȩ��", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//�������:�޸Ĺ�������
vector <CGlobalStruct::TFLV> CWAInfo::WA_ModifyGuildName_Operater(int userByID, char * ServerIP, char * GuildName, char* NewGuildName)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	try
	{
		
		REQ_GUILD_CHANGE_NAME req_GuildNameChanged;		//�޸Ĺ�������
		REP_GUILD_CHANGE_NAME rep_GuildNameChanged;		//�޸Ĺ�������
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼������ʧ��", "�޸Ĺ�������");	
		}
		else
		{
			sprintf(retMessage, "�޸Ĺ�������:<����>%s, <��������>%s,<�¹�������>%s", ServerIP, GuildName,NewGuildName);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG
			
			req_packetLength = sizeof(REQ_GUILD_CHANGE_NAME);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			memcpy(&req_GuildNameChanged.guld_name, GuildName, strlen(GuildName)+1);
			memcpy(&req_GuildNameChanged.new_name, NewGuildName, strlen(NewGuildName)+1);
			//����
			req_GuildNameChanged.srv_realm_id=srv_Realmid;
			req_GuildNameChanged.srv_cluster_id=srv_ClusterID;
			memcpy(sendbuf+4, &req_GuildNameChanged, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "�޸Ĺ�������");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				int recvbyte = m_MySocket.MyRecv(recvbuf,10240);
				if ( recvbyte>0 && strcmp(recvbuf, "") )
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_GuildNameChanged, recvbuf+4, packetSize);
					switch (rep_GuildNameChanged.eResult)
					{
					case REP_GUILD_CHANGE_NAME::E_SUCCESS:		//�ɹ�
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�޸Ĺ������Ƴɹ�", "�޸Ĺ�������");	
						break;
					default:									//ʧ��
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�޸Ĺ�������ʧ��", "�޸Ĺ�������");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�޸Ĺ������Ƴ�ʱ", "ת�û᳤Ȩ��");
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "�޸Ĺ�������");		
	}
	
	m_OperatorWA.GmOperLog(userByID, ServerIP, "�޸Ĺ�������", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//�������:�߳������Ա
vector <CGlobalStruct::TFLV> CWAInfo::WA_KickGuildMember_Operater(int userByID, char * ServerIP, char * Character, char* GuildName)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	try
	{
		
		REQ_GUILD_KICK_MEMBER req_KickedMember;		//�߳������Ա
		REP_GUILD_KICK_MEMBER rep_KickedMember;		//�߳������Ա
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼������ʧ��", "�߳������Ա");	
		}
		else
		{
			sprintf(retMessage, "�߳������Ա:<����>%s, <��������>%s,<Ҫ�ߵĽ�ɫ����>%s", ServerIP, GuildName,Character);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG
			
			req_packetLength = sizeof(REQ_GUILD_KICK_MEMBER);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			memcpy(&req_KickedMember.guld_name, GuildName, strlen(GuildName)+1);
			memcpy(&req_KickedMember.char_name, Character, strlen(Character)+1);
			//����
			req_KickedMember.srv_realm_id=srv_Realmid;
			req_KickedMember.srv_cluster_id=srv_ClusterID;
			memcpy(sendbuf+4, &req_KickedMember, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "�߳������Ա");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				int recvbyte = m_MySocket.MyRecv(recvbuf,10240);
				if ( recvbyte>0 && strcmp(recvbuf, "") )
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_KickedMember, recvbuf+4, packetSize);
					switch (rep_KickedMember.eResult)
					{
					case REP_GUILD_KICK_MEMBER::E_SUCCESS:		//�ɹ�
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�߳������Ա�ɹ�", "�߳������Ա");	
						break;
					default:										//ʧ��
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�߳������Աʧ��", "�߳������Ա");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "�߳������Ա��ʱ", "�߳������Ա");
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "�߳������Ա");		
	}
	
	m_OperatorWA.GmOperLog(userByID, ServerIP, "�߳������Ա", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//�������:��ɢ����
vector <CGlobalStruct::TFLV> CWAInfo::WA_DissolveGuild_Operater(int userByID, char * ServerIP, char* GuildName)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	try
	{
		
		REQ_GUILD_DISMISS req_GuildDismiss;		//��ɢ����
		REP_GUILD_DISMISS rep_GuildDismiss;		//��ɢ����
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼������ʧ��", "��ɢ����");	
		}
		else
		{
			sprintf(retMessage, "��ɢ����:<����>%s, <��������>%s", ServerIP, GuildName);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG
			
			req_packetLength = sizeof(REQ_GUILD_DISMISS);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			memcpy(&req_GuildDismiss.guld_name, GuildName, strlen(GuildName)+1);
			//����
			req_GuildDismiss.srv_realm_id=srv_Realmid;
			req_GuildDismiss.srv_cluster_id=srv_ClusterID;
			memcpy(sendbuf+4, &req_GuildDismiss, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "��ɢ����");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				int recvbyte = m_MySocket.MyRecv(recvbuf,10240);
				if ( recvbyte>0 && strcmp(recvbuf, "") )
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_GuildDismiss, recvbuf+4, packetSize);
					switch (rep_GuildDismiss.eResult)
					{
					case REP_GUILD_KICK_MEMBER::E_SUCCESS:		//�ɹ�
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��ɢ����ɹ�", "��ɢ����");	
						break;
					default:										//ʧ��
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��ɢ����ʧ��", "��ɢ����");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��ɢ���ᳬʱ", "��ɢ����");
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "��ɢ����");		
	}
	
	m_OperatorWA.GmOperLog(userByID, ServerIP, "��ɢ����", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//�����б��ѯ
vector <CGlobalStruct::TFLV> CWAInfo::WA_PET_QUERY(char* gameName,char* strSql,int index,int Page)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{
		DBTFLV=GameDBQuery("WA","127.0.0.1",4,0,0,"exec WA_PetList_Query");
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("WA","�����б��ѯ�ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("WA","�����б��ѯ�ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("WA","�����б��ѯ�����쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("WA",(char *)err.what());
		return DBTFLV;
	}
	return DBTFLV;
}

//��ҽ�ɫ���Իָ�		
vector <CGlobalStruct::TFLV> CWAInfo::WA_ResumeAttribute_Operate(int userByID, char * ServerIP, char *Account, char *Character)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	
	try
	{
		
		REQ_MODIFY_CHARACTER req_modify_character;				//�޸Ľ�Ǯ����
		REP_MODIFY_CHARACTER rep_modify_character;				//�޸Ľ�Ǯ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼������ʧ��", "�ָ���ɫ����");	
		}
		else
		{
			sprintf(retMessage, "��ɫ���Իָ�:<����>%s, <�ʺ�>%s, <��ɫ��>%s", ServerIP, Account, Character);			
			logFile.WriteLog("<��ʿOL>",operName, retMessage);//��LOG
			
			req_packetLength = sizeof(REQ_MODIFY_CHARACTER);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			//����
			req_modify_character.srv_realm_id=srv_Realmid;
			//��������ID
			req_modify_character.srv_cluster_id=srv_ClusterID;
			req_modify_character.attr = ADMIN::MCA_ONLINE_REPAIR;
//			req_modify_character.newValue = level;
			memcpy(&req_modify_character.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_modify_character, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "��ɫ���Իָ�");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				
				int recvbyte = m_MySocket.MyRecv(recvbuf,10240);
				if ( recvbyte>0 && strcmp(recvbuf, "") )
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_modify_character, recvbuf+4, packetSize);
					switch (rep_modify_character.eResult)
					{
					case REP_DELETE_ITEM::E_SUCCESS:	//�ɹ�
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��ɫ���Իָ��ɹ�", "��ɫ���Իָ�");	
						break;
					default:							//ʧ��
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��ɫ���Իָ�ʧ��", "��ɫ���Իָ�");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��ɫ���Իָ���ʱ", "��ɫ���Իָ�");
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "��ɫ���Իָ�");		
	}
	
	m_OperatorWA.GmOperLog(userByID, ServerIP, "��ɫ���Իָ�", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//Ⱥ���ʼ�(�޵���)
vector <CGlobalStruct::TFLV> CWAInfo::WA_SendMails_Operate(int userByID, char *ServerIP, char *Character, char *MailSub, char *MailContent)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	try
	{
		REQ_MAIL_ITEM req_SendGift;		//������Ʒ����
		REP_MAIL_ITEM rep_SendGift;		//������Ʒ��Ӧ
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		char *pTemp = NULL;
		int iCount = 0;
		CString sztemp = "";
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��¼������ʧ��", "Ⱥ���ʼ�(�޵���)");	
		}
		else
		{
			req_packetLength = sizeof(REQ_MAIL_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//�����ݰ�����
			memcpy(sendbuf+2, &flag, 2);			//�̶��ֶ�
			
			//Added by tengjie 2010-06-24
			ZeroMemory(req_SendGift.item_type_id_list, sizeof(req_SendGift.item_type_id_list));
			ZeroMemory(req_SendGift.item_count_list, sizeof(req_SendGift.item_count_list));
			ZeroMemory(req_SendGift.item_gen_list, sizeof(req_SendGift.item_gen_list));
			ZeroMemory(req_SendGift.item_expire, sizeof(req_SendGift.item_expire));

			sprintf(retMessage, "Ⱥ���ʼ�<����>%s, <��ɫ��>%s, <�ʼ�����><%s>,<�ʼ�����>%s", ServerIP, Character, MailSub, MailContent);			
			logFile.WriteLog("<��ʿ��˫>",operName, retMessage);//��LOG

			memcpy(&req_SendGift.char_name, Character, strlen(Character)+1);
			req_SendGift.receiver_type = REQ_MAIL_ITEM::RT_SINGLE_PLAYER;

			memcpy(req_SendGift.subject, MailSub, strlen(MailSub)+1);
			memcpy(req_SendGift.body, MailContent, strlen(MailContent)+1);
			req_SendGift.money = 0;

			//����
			req_SendGift.srv_realm_id=srv_Realmid;
			//��������ID
			req_SendGift.srv_cluster_id=srv_ClusterID;
			memcpy(sendbuf+4, &req_SendGift, req_packetLength);
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "��������ʧ��", "Ⱥ���ʼ�");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				int recvbyte = m_MySocket.MyRecv(recvbuf,10240);
				if ( recvbyte>0 && strcmp(recvbuf, "") )
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_SendGift, recvbuf+4, packetSize);
					switch (rep_SendGift.eResult)
					{
					case REP_MAIL_ITEM::E_SUCCESS:	//�ɹ�
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "Ⱥ���ʼ��ɹ�", "Ⱥ���ʼ�");	
						break;
					default:							//ʧ��
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "Ⱥ���ʼ�ʧ��", "Ⱥ���ʼ�");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "Ⱥ���ʼ���ʱ", "Ⱥ���ʼ�");
				}
			}
			m_MySocket.Close();
		}
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "δ֪����", "Ⱥ���ʼ�");		
	}
	
	m_OperatorWA.GmOperLog(userByID, ServerIP, "Ⱥ���ʼ�", retMessage, DBTFLV[0]);
	return DBTFLV;
}

// string CWAInfo::UserLogInterface(char *ServerIP,string NewUserName,string NewTime,string BanType)
// {
// 	char *_totalpath = new char[100];
// 	string _temppath  = "";
// 	CString szTemUrl = "";
// 	CInternetSession MySession;
// 	CHttpConnection* lpConnetction;
// 	CHttpFile*       lpHttpFile;
// 	INTERNET_PORT    HttpPort = 80;
// //	CString szMd5 = "";
// 	CString strSentence, strGetSentence = "";
// 	CString szSql = "";
// 	CString szData = "";
// 	CSqlHelper m_SqlHelper;
// 	vector<CGlobalStruct::DataRecord> datarecord;
// 	CMD5 md5_OOp;
// 
// 	GetCurrentDirectory(100,_totalpath);
// 	strcat(_totalpath,"\\Config\\config.ini");
// 	_temppath = _totalpath;
// 
// 	//��ȡ��ϷIP
// 	char WAServerIp[20]= "";
// 	char WAUrl[128]="";
// 	GetPrivateProfileString("server","WAServer","61.152.150.158",WAServerIp,20,_temppath.c_str());//����һ��
// 	GetPrivateProfileString("url","WAUrl","passport/games/gmtools_ban_log_w.php",WAUrl,128,_temppath.c_str());//WA���յ�ַ
// 
// 	delete []_totalpath;
// 	
// 	szSql.Format("exec WA_GetIdx '%s'",ServerIP);
// 	
// 	m_SqlHelper.SqlHelperMain("WA", &datarecord,"127.0.0.1",szSql.GetBuffer(0),4,0,0);
// 	
// 	if(datarecord.empty())
// 		return "";
// 	
// 	vector<CGlobalStruct::DataRecord>::iterator iter;
// 	CGlobalStruct::DataRecord  m_DataRecord;
// 	for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
// 	{
// 		m_DataRecord = *iter;
// 		if(!strcmp("idx", m_DataRecord.recordfieldname))//��ȡ��Ӧ��ServerIP
// 		{
// 			szData.Format("%s", m_DataRecord.recorddata);//�����ѯ�õ������һ����¼
// 			break;
// 		}
// 	}
// 
// 	szTemUrl.Format("http://%s/%s",WAServerIp,WAUrl);
// 	lpConnetction = MySession.GetHttpConnection(WAServerIp, HttpPort);
// 	
// 	lpHttpFile = lpConnetction->OpenRequest(CHttpConnection::HTTP_VERB_POST, 
// 		WAUrl,
// 		szTemUrl.GetBuffer(0),1, NULL, NULL, 
// 		INTERNET_FLAG_NO_AUTO_REDIRECT);
// 	
// 	if(NULL != lpHttpFile)
// 	{
// 		string  strHeader = "Content-Type:application/x-www-form-urlencoded";
// 		
// // 		//asciiת����unicode
// // 		int  wcsLen  =  ::MultiByteToWideChar(CP_ACP, NULL, you9Nickname.c_str(), you9Nickname.length(), NULL,  0 );
// // 		wchar_t *  wszString  =   new  wchar_t[wcsLen  +   1 ];
// // 		::MultiByteToWideChar(CP_ACP, NULL, you9Nickname.c_str(), you9Nickname.length(), wszString, wcsLen);
// // 		wszString[wcsLen]  = '\0' ;
// // 		
// // 		//unicodeת����utf8
// // 		int u8Len = ::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);
// // 		char* szU8 = new char[u8Len + 1];
// // 		::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), szU8, u8Len, NULL, NULL);
// // 		szU8[u8Len] = '\0';
// 		
// 		int length=0;
// 		
// 		string strMd5 = NewUserName;
// 		strMd5 += "|T4pb5A.gmtoolbanlog";
// 		//md5����
// // 		char *szMd5 = new char[strlen(strMd5.c_str())+1];
// // 		ZeroMemory(szMd5, strlen(strMd5.c_str())+1);
// //		MD5_EnCode(szMd5,&length,(char*)strMd5.c_str(),strMd5.length());
// 
// 		string requestString = "username=";
// 			   requestString += NewUserName;
// 			   requestString += "&gmareaid=";
// 			   requestString += szData.GetBuffer(0);
// 			   requestString += "&ban=";
// 			   requestString += BanType;
// 			   requestString += "&banhour=";
// 			   requestString += NewTime;
// 			   requestString += "&sign=";
// 			   requestString += md5_OOp.MD5Encode((char*)strMd5.c_str()).c_str();
// 		
// 		printf("the parameters of request: %s\n",requestString.c_str());
// 
// 		if(lpHttpFile->SendRequest(strHeader.c_str(),
// 			(DWORD)strHeader.length(),
// 			(void*)requestString.c_str(),
// 			(DWORD)requestString.length()))
// 			printf("send request success!\n");
// 		
// 		while(lpHttpFile->ReadString(strSentence))    // ��ȡ�ύ���ݺ�ķ��ؽ��
// 		{
// 			strGetSentence = strGetSentence + strSentence + char(13) + char(10);
// 		}
// 		
// 		printf("���ص�����:%s\n",strGetSentence);
// 		
// 	}
// 
// 	return strGetSentence;
// }
//end