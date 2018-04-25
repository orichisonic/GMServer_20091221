#include "StdAfx.h"
#include "WAInfo.h"
#include <iostream>
using namespace std;

CWAInfo::CWAInfo(void)
{
}

CWAInfo::~CWAInfo(void)
{
	//卸载网络库
//	m_MySocket.Uninitialize();
}

//初始化工作
vector <CGlobalStruct::TFLV> CWAInfo::initialize(CSession * pSess,unsigned char * pbuf, int length)
{
	try
	{
//		m_MySocket.Initialize();	//初始化网络库

		ZeroMemory(LocalIP, 30);
		logFile.ReadValue("Scheme", "DATABASE", "Address",LocalIP, 30);//获取本地的数据库地址

		if(length>0)
		{
			recvVect.Create(length/8);//分配空间
			recvVect.DeBody(pbuf,length);//解析包结构
			srv_ClusterID=0;
			srv_Realmid=0;
		}
		pGameSess = pSess;	
		CLogic::SocketToUserName(pSess->GetSocket(), operName);//保存执行操作的用户名
	}
	catch (...)
	{
		sprintf(errMessage,"初始化错误");
		logFile.WriteErrorLog("WA",errMessage);
	}

	return recvVect.GetInnerList();
}
//连接远程服务端
bool CWAInfo::Connect_Remote_Login(char * ServerIP, int gamedbid)
{
	bool bLoginFlag = false;	//登录成功标识

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

		REQ_LOGIN req_login;		//登录请求
		REP_LOGIN rep_login;		//登录回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
// 		srv_ClusterID=0;
// 		srv_Realmid=0;
		
		char WA_GateWay[40];
		ZeroMemory(WA_GateWay,40);
		int ListenPort=0;//获取远程端口

		char retMessage[255];
		memset(retMessage, 0, 255);
		
		if(!m_OperatorWA.GetServerIP("WA",ServerIP, gamedbid, WA_GateWay, &ListenPort, srv_ClusterID,srv_Realmid))
		{
			logFile.WriteLog("<勇士无双>",operName,"获得服务器IP失败");	
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
				sprintf(retMessage, "获取大区ID与服务器组ID失败");
				logFile.WriteLog("<勇士无双>",operName,retMessage);
				return false;
			}
			cout<<"serverip:"<<WA_GateWay<<"\tport:"<<ListenPort<<endl;
			if(m_MySocket.MyConnect(WA_GateWay, ListenPort))
			{
				req_packetLength = sizeof(REQ_LOGIN);
				
				memset(sendbuf,0, 1024);
				memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
				memcpy(sendbuf+2, &flag, 2);			//固定字段填充24
				strcpy(req_login.loginUserName, szAccount);
				strcpy(req_login.loginPassWord, szPassword);
				req_login.srv_realm_id=srv_Realmid;
				memcpy(sendbuf+4, &req_login, req_packetLength);
				if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
				{
					logFile.WriteLog("WA",operName,"发送请求失败");
				}
				else
				{
					memset(recvbuf, 0, 10240);
					int recvbyte = m_MySocket.MyRecv(recvbuf, 10240);	//默认超时时间20秒
					if (recvbyte>0 && strcmp(recvbuf, ""))
					{
						short int packetSize = *(short int *)recvbuf;
						memcpy(&rep_login, recvbuf+4, packetSize);
						switch (rep_login.eResult)
						{
						case REP_LOGIN::E_SUCCESS:	//登录成功
							{
//								logFile.WriteLog("<勇士无双>",operName,"登陆成功\n");
								bLoginFlag = true;
								return bLoginFlag;
							}
						default:					//登录失败
							sprintf(retMessage, "登陆失败(用户信息)[%s]", WA_GateWay);
							logFile.WriteLog("<勇士无双>",operName,retMessage);
							break;
						}
					}
					else
					{
						sprintf(retMessage, "登陆失败(接收数据)[%s]", WA_GateWay);
						logFile.WriteLog("<勇士无双>",operName, retMessage);
					}
				}
			}
			else
			{
				sprintf(retMessage, "登陆失败(连接查询服务器)[%s]", WA_GateWay);
				logFile.WriteLog("<勇士无双>", operName, retMessage);				
			}
		}

		
		delete []sendbuf;
		delete []recvbuf;

	}	
	catch (...)
	{
		logFile.WriteLog("<勇士无双>",operName,"登录未知错误");
	}

	m_MySocket.Close();	

	return bLoginFlag;
}

//网络查询用户信息
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
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录服务器失败", "角色信息查询");
		}
		else
		{
			if(!strcmp(UserName,""))
			{
				sprintf(retMessage, "角色信息查询(角色名查询)：<大区>%s, <角色名>%s", ServerIP, Character);			
				logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG
				
				//根据昵称查询角色信息
				WA_Net_UserInfoByChar(Character,&DBTFLV,1);
			}
			else
			{
				sprintf(retMessage, "角色信息查询(帐号查询)：<大区>%s, <帐号>%s", ServerIP, UserName);			
				logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG
				
				REQ_CHARACTER_INFO req_character_info;
				REP_NAME_ID_LIST rep_name_id_list;
				short int req_packetLength;
				short int flag = 24;
				char *sendbuf = new char[1024];
				char * recvbuf = new char[10240];
				
				req_packetLength = sizeof(REQ_CHARACTER_INFO);				
				memset(sendbuf,0, 1024);
				memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
				memcpy(sendbuf+2, &flag, 2);			//固定字段
				req_character_info.req_type = REQ_CHARACTER_INFO::RT_CHAR_BASE_INFO;
				req_character_info.query_condition = ADMIN::QC_USERNAME;
				//大区
				req_character_info.srv_realm_id=srv_Realmid;
				//服务器组ID
				req_character_info.srv_cluster_id=srv_ClusterID;
				memcpy(&req_character_info.user_name, UserName, strlen(UserName)+1);
				memcpy(sendbuf+4, &req_character_info, req_packetLength);
				
				if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "帐户不存在", "角色信息查询");
				}
				else
				{
					bool bRecvFlag = true;	//是否需要继续接收数据包
					while (bRecvFlag)
					{
						//memset(recvbuf, 0, 20480);
						*recvbuf = NULL;
						
						// 设置接收缓冲区大小
						DWORD nRecvBuf=10*1024;
						setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
						Sleep(1000);
						int recvbyte = m_MySocket.MyRecv(recvbuf,10480);
						if ( recvbyte>0 && strcmp(recvbuf, "") && vect_FriendInfo.size() == 0 )
						{
							short int packetSize = 0;	//每个结构体数据包长
							int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
							int offset = 0;				//指针偏移量
							
							while (nLeft>0)
							{
								packetSize = *(short int *)(recvbuf+offset);
								
								if (nLeft<packetSize+4 || packetSize <= 0)
								{
									break;
								}
								else
								{
									//取出数据包
									memcpy(&rep_name_id_list, recvbuf+offset+4, packetSize);
									
									//处理数据
									if (rep_name_id_list.allSend == 1)	//收到最后一个结构包
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

					//处理结果集
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
// 					int recvbyte = m_MySocket.MyRecv(recvbuf, 10240);	//默认超时时间20秒
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
// 								DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "帐户不存在", "角色信息查询");
// 								break;
// 							}
// 						}
// 					}
// 					else
// 					{
// 						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "角色查询超时", "角色信息查询");
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "角色信息查询");
	}

	return DBTFLV;
}

//通过角色名称查询角色信息
void CWAInfo::WA_Net_UserInfoByChar(char * Character,vector <CGlobalStruct::TFLV>* pDBTFLV,int iPageCount)
{
	try
	{
		CGlobalStruct::TFLV m_tflv;
		REQ_CHARACTER_INFO req_character_info;					//角色查询请求
		REP_CHARACTER_BASE_INFO rep_character_base_info;		//角色查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		char strInt[30];
		
		req_packetLength = sizeof(REQ_CHARACTER_INFO);
		
		memset(sendbuf,0, 1024);
		memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
		memcpy(sendbuf+2, &flag, 2);			//固定字段
		req_character_info.req_type = REQ_CHARACTER_INFO::RT_CHAR_BASE_INFO;
		req_character_info.query_condition = ADMIN::QC_CHARACTERNAME;
		//大区
		req_character_info.srv_realm_id=srv_Realmid;
		//服务器组ID
		req_character_info.srv_cluster_id=srv_ClusterID;
		memcpy(&req_character_info.user_name, Character, strlen(Character)+1);
		memcpy(sendbuf+4, &req_character_info, req_packetLength);		

		if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
		{
			logFile.WriteLog("WA","GM","发送请求失败\n");	
		}
		else
		{
			memset(recvbuf, 0, 10240);
			// 设置接收缓冲区大小
// 			DWORD nRecvBuf=10*1024;
// 			setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
// 			Sleep(1000);
			int recvbyte = m_MySocket.MyRecv(recvbuf, 10240);	//默认超时时间20秒
			if (recvbyte>0 && strcmp(recvbuf, ""))
			{
				short int msg_type = *(short int *)(recvbuf+4);

				switch (msg_type)
				{
				case CMD_REP_CHARACTER_BASE_INFO:
					{
						short int packetSize = *(short int *)recvbuf;						

						memcpy(&rep_character_base_info, recvbuf+4, packetSize);

						// 大区id
						_itoa((int)rep_character_base_info.srv_realm_id,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_RealmId;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);
						
						// 服务器组id
						_itoa((int)rep_character_base_info.srv_cluster_id,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_ClusterID;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);
						
						// 帐号
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Account;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(rep_character_base_info.user_name)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",rep_character_base_info.user_name);
						pDBTFLV->push_back(m_tflv);
						
						// 角色id
						_itoa(rep_character_base_info.char_id,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_NickID;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);
						
						// 角色名称
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_UserNick;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(rep_character_base_info.char_name)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",rep_character_base_info.char_name);
						pDBTFLV->push_back(m_tflv);

						//角色称谓1
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_TitleName1;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(rep_character_base_info.char_title1)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",rep_character_base_info.char_title1);
						pDBTFLV->push_back(m_tflv);
						
						//角色称谓2
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_TitleName2;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(rep_character_base_info.char_title2)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",rep_character_base_info.char_title2);
						pDBTFLV->push_back(m_tflv);

// 						//角色类型(职业性别等)
// 						_itoa((int)rep_character_base_info.char_type,strInt,10);
// 						m_tflv.nIndex=pDBTFLV->size()+1;
// 						m_tflv.m_tagName=CEnumCore::TagName::WA_CharType;
// 						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
// 						m_tflv.m_tvlength = strlen(strInt)+1;
// 						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
// 						pDBTFLV->push_back(m_tflv);

						//角色类型(职业性别等)
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_CharType;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						switch (rep_character_base_info.char_type)
						{
						case 1:
							m_tflv.m_tvlength = strlen("男剑士")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"男剑士");							
							break;
						case 2:
							m_tflv.m_tvlength = strlen("女剑士")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"女剑士");
							break;
						case 3:
							m_tflv.m_tvlength = strlen("男武者")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"男武者");
							break;
						case 4:
							m_tflv.m_tvlength = strlen("女武者")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"女武者");
							break;
						case 5:
							m_tflv.m_tvlength = strlen("男战法师")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"男战法师");
							break;
						case 6:
							m_tflv.m_tvlength = strlen("女战法师")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"女战法师");
							break;
						case 7:
							m_tflv.m_tvlength = strlen("男魔剑士")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"男魔剑士");
							break;
						case 8:
							m_tflv.m_tvlength = strlen("女魔剑士")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"女魔剑士");
							break;
						case 9:
							m_tflv.m_tvlength = strlen("男气功师")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"男气功师");
							break;
						case 10:
							m_tflv.m_tvlength = strlen("女气功师")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"女气功师");
							break;
						case 11:
							m_tflv.m_tvlength = strlen("男摄魂师")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"男摄魂师");
							break;
						case 12:
							m_tflv.m_tvlength = strlen("女摄魂师")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"女摄魂师");
							break;
						case 13:
							m_tflv.m_tvlength = strlen("男狂剑士")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"男狂剑士");
							break;
						case 14:
							m_tflv.m_tvlength = strlen("女狂剑士")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"女狂剑士");
							break;
						case 15:
							m_tflv.m_tvlength = strlen("男鬼武")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"男鬼武");
							break;
						case 16:
							m_tflv.m_tvlength = strlen("女鬼武")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"女鬼武");
							break;
						case 17:
							m_tflv.m_tvlength = strlen("男元素使")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"男元素使");
							break;
						case 18:
							m_tflv.m_tvlength = strlen("女元素使")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"女元素使");
							break;
						case 19:
							m_tflv.m_tvlength = strlen("男圣骑士")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"男圣骑士");
							break;
						case 20:
							m_tflv.m_tvlength = strlen("女圣骑士")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"女圣骑士");
							break;
						case 21:
							m_tflv.m_tvlength = strlen("男武道家")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"男武道家");
							break;
						case 22:
							m_tflv.m_tvlength = strlen("女武道家")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"女武道家");
							break;
						case 23:
							m_tflv.m_tvlength = strlen("男神官")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"男神官");
							break;
						case 24:
							m_tflv.m_tvlength = strlen("女神官")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"女神官");
							break;
						default:
							m_tflv.m_tvlength = strlen("未知类型")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"未知类型");
							break;
						}
						pDBTFLV->push_back(m_tflv);

						// 是否被删除
//						_itoa((int)rep_character_base_info.is_deleted,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_IsDelete;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						if (rep_character_base_info.is_deleted)
						{
							m_tflv.m_tvlength = strlen("已删除")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"已删除");
						}
						else
						{
							m_tflv.m_tvlength = strlen("未删除")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"未删除");
						}
						pDBTFLV->push_back(m_tflv);

						//是否在线
//						_itoa((int)rep_character_base_info.is_online,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_IsOnline;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						if (rep_character_base_info.is_online)
						{
							m_tflv.m_tvlength = strlen("在线")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"在线");
						}
						else
						{
							m_tflv.m_tvlength = strlen("离线")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"离线");
						}
						pDBTFLV->push_back(m_tflv);

						//是否封停
//						_itoa((int)rep_character_base_info.is_closed,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_IsClosed;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						if (rep_character_base_info.is_closed)
						{
							m_tflv.m_tvlength = strlen("已封停")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"已封停");
						}
						else
						{
							m_tflv.m_tvlength = strlen("未封停")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"未封停");
						}
						pDBTFLV->push_back(m_tflv);

						//角色等级
						_itoa((int)rep_character_base_info.char_level,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Level;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//游戏币数量
						_ultoa((unsigned long)rep_character_base_info.money,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Money;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);
						
						//9游币数量
						_ultoa((unsigned long)rep_character_base_info.nineyou_money,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_NineyouMoney;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//储备金
						_ultoa((unsigned long)rep_character_base_info.reserve_money,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_ReserveMoney;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//当前所在场景名
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_CurSceneName;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(rep_character_base_info.cur_scene_name) + 1;
						sprintf((LPSTR)&m_tflv.lpdata, "%s", rep_character_base_info.cur_scene_name);
						pDBTFLV->push_back(m_tflv);

						//当前所在场景id
						_ultoa((unsigned long)rep_character_base_info.cur_scene_id,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_CurSceneID;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//当前躲在场景坐标
						_ultoa((unsigned long)rep_character_base_info.cur_scene_pos,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_CurScenePos;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//生命值
						_ultoa((unsigned long)rep_character_base_info.hp,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Hp;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//魔法值
						_itoa((int)rep_character_base_info.mp,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Mp;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//活力值
						_itoa((int)rep_character_base_info.vitality,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Vitality;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//善恶值
						_itoa((int)rep_character_base_info.good_evil,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_GoodEvil;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//物理攻击
						_itoa((int)rep_character_base_info.physical_attack,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_PhysicalAttack;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//物理防御
						_itoa((int)rep_character_base_info.physical_defense,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_PhysicalDefense;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//魔法攻击
						_itoa((int)rep_character_base_info.magic_attack,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_MagicAttack;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//魔法防御
						_itoa((int)rep_character_base_info.magic_defense,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_MagicDefense;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//暴击率
						sprintf(strInt, "%f", rep_character_base_info.critical);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Critical;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//力量
						_itoa((int)rep_character_base_info.strength,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Strength;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//敏捷
						_itoa((int)rep_character_base_info.dexterity,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Dexterity;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);
						
						//智力
						_itoa((int)rep_character_base_info.intellect,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Intellect;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//体质
						_itoa((int)rep_character_base_info.corporeity,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Corporeity;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);
						
						//潜力点
						_itoa((int)rep_character_base_info.potential_point,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_PotentialPoint;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);
						
						//经验
						_ultoa((unsigned long)rep_character_base_info.exp,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Exp;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);
						
						//升级经验
						_ultoa((unsigned long)rep_character_base_info.exp_next_level,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_ExpNextLevel;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//物理抗性
						sprintf(strInt, "%f", rep_character_base_info.physical_resist);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_PhysicalResist;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);
						
						//法术抗性
						sprintf(strInt, "%f", rep_character_base_info.magic_resist);
						m_tflv.nIndex = pDBTFLV->size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_MagicResist;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);
						
						//火焰抗性
						sprintf(strInt, "%f", rep_character_base_info.fire_resist);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_FireResist;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//冰冻抗性
						sprintf(strInt, "%f", rep_character_base_info.cold_resist);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_ColdResist;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//毒素抗性
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
						printf("查询角色失败\n");
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
		sprintf(errMessage,"查询角色信息错误");
		logFile.WriteErrorLog("WA",errMessage);
	}

}

//网络踢人
vector <CGlobalStruct::TFLV> CWAInfo::WA_Net_KickPlayer(int userByID,char * ServerIP,char * UserName, char * Character)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{

		REQ_KICK_USER req_kick_user;		//角色查询请求
		REP_KICK_USER rep_kick_user;		//角色查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];		

		req_packetLength = sizeof(REQ_KICK_USER);

		memset(sendbuf,0, 1024);
		memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
		memcpy(sendbuf+2, &flag, 2);			//固定字段

		//勇士gameDBID=1
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "GM踢人");
		}
		else
		{
			sprintf(retMessage, "GM踢人：<大区>%s, <帐号>%s, <角色名>%s", ServerIP, UserName, Character);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG
			
			if(!strcmp(UserName,""))		//角色踢人
			{
				req_kick_user.kickCondition = ADMIN::QC_CHARACTERNAME;
				memcpy(&req_kick_user.char_name, Character, strlen(Character)+1);

			}
			else							//帐号踢人
			{
				req_kick_user.kickCondition = ADMIN::QC_USERNAME;
				memcpy(&req_kick_user.user_name, UserName, strlen(UserName)+1);
			}
			//大区
			req_kick_user.srv_realm_id=srv_Realmid;
			//服务器组ID
			req_kick_user.srv_cluster_id=srv_ClusterID;
			memcpy(sendbuf+4, &req_kick_user, req_packetLength);

			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "GM踢人");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				int recvbyte = m_MySocket.MyRecv(recvbuf, 10240);	//默认超时时间20秒
				if (recvbyte>0 && strcmp(recvbuf, ""))
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_kick_user, recvbuf+4, packetSize);
									
					switch (rep_kick_user.eResult)
					{
					case REP_KICK_USER::E_SUCCESS:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "踢人成功", "GM踢人");
						break;
					case REP_KICK_USER::E_FAILED_CHAR_NOTINGAME:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "角色不在线", "GM踢人");
						break;
					default:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "踢人失败", "GM踢人");
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "GM踢人超时", "GM踢人");
				}
			}

			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "GM踢人");
	}

	m_OperatorWA.GmOperLog(userByID, ServerIP, "GM踢人", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//网络封停
vector <CGlobalStruct::TFLV> CWAInfo::WA_Net_ClosePlayer(int userByID,char * ServerIP,char * UserName,int CharacterID, char * Character, int iTime, int Reason, char *description)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{

		REQ_CLOSE_ACCOUNT req_close_account;		//封停帐号请求
		REP_CLOSE_ACCOUNT rep_close_account;		//封停帐号回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];

		//Added by tengjie 2010-07-30
		CString New_Time = "";

		req_packetLength = sizeof(REQ_CLOSE_ACCOUNT);
		
		memset(sendbuf,0, 1024);
		memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
		memcpy(sendbuf+2, &flag, 2);			//固定字段

		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "帐号封停");
		}
		else
		{
			sprintf(retMessage, "帐号封停：<大区>%s, <帐号>%s, <时间>%d分, <原因>%d, <描述>%s", ServerIP, UserName, iTime, Reason, description);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			if(!strcmp(UserName,""))	//角色封停
			{
				req_close_account.closeCondition = ADMIN::QC_CHARACTERNAME;
				memcpy(&req_close_account.char_name, Character, strlen(Character)+1);
				
			}
			else						//帐号封停
			{
				req_close_account.closeCondition = ADMIN::QC_USERNAME;
				memcpy(&req_close_account.user_name, UserName, strlen(UserName)+1);
			}

			switch (Reason)
			{
// 			case 0:	// 未知
// 				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_UNKNOWN;
// 				break;
			case 1:	// 系统封停
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_SYSTEM;
				break;
			case 2: // 用户自助
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_SYSTEM_TEMP;
				break;
			case 3: // 用户自助
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_USERCUSTOM;
				break;
			case 4: // 用户临时自助
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_USERCUSTOM_TEMP;
				break;
			default:// 未知
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_SYSTEM;
				break;
			}
			//大区
			req_close_account.srv_realm_id=srv_Realmid;
			//服务器组ID
			req_close_account.srv_cluster_id=srv_ClusterID;

			req_close_account.duration_time = iTime;

			//Modified by tengjie 2010-05-13
//			strcpy(req_close_account.description, description);		
			strcpy(req_close_account.description, "");
			memcpy(sendbuf+4, &req_close_account, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "帐号封停");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				int recvbyte = m_MySocket.MyRecv(recvbuf, 10240);	//默认超时时间20秒
				if (recvbyte>0 && strcmp(recvbuf, ""))
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_close_account, recvbuf+4, packetSize);
					
					switch (rep_close_account.eResult)
					{
					case REP_CLOSE_ACCOUNT::E_SUCCESS:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "封停成功", "帐号封停");

						//Added by tengjie 2010-05-13
						WA_SqlForUserInfo(UserName,ServerIP,iTime,description);
						//Added by tengjie 2010-07-29:帐户中心记录日志接口
						New_Time.Format("%d", iTime/60);
						UserLogInterface(ServerIP,UserName,New_Time.GetBuffer(0),38,"1");
						break;
					case REP_CLOSE_ACCOUNT::E_FAILED_ACCOUNT_NOTEXIST:	
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "帐号不存在", "帐号封停");
						break;
					default:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "封停失败", "帐号封停");
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "帐号封停超时", "帐号封停");
				}
			}

			m_MySocket.Close();
		}
	
		delete []sendbuf;
		delete []recvbuf;
	}
	catch (...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "帐号封停");
	}

	m_OperatorWA.GmOperLog(userByID, ServerIP, "帐号封停", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//批量封停
vector <CGlobalStruct::TFLV> CWAInfo::WA_Net_ClosePlayerGroup(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character,int iTime, int Reason, char *description)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	char retMessage[255];
	memset(retMessage, 0, 255);

	////////--踢人--///////////////////////////////////////////////////////////
	try
	{
		REQ_KICK_USER req_kick_user;		//角色查询请求
		REP_KICK_USER rep_kick_user;		//角色查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];		
		
		req_packetLength = sizeof(REQ_KICK_USER);
		
		memset(sendbuf,0, 1024);
		memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
		memcpy(sendbuf+2, &flag, 2);			//固定字段

		cout<<"ServerIP:"<<ServerIP<<endl;
		if (!Connect_Remote_Login(ServerIP, 1))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "GM踢人");
		}
		else
		{
			sprintf(retMessage, "GM踢人：<大区>%s, <帐号>%s, <角色名>%s", ServerIP, UserName, Character);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG
			
			if(!strcmp(UserName,""))		//角色踢人
			{
				req_kick_user.kickCondition = ADMIN::QC_CHARACTERNAME;
				memcpy(&req_kick_user.char_name, Character, strlen(Character)+1);
	
			}
			else							//帐号踢人
			{
				req_kick_user.kickCondition = ADMIN::QC_USERNAME;
				memcpy(&req_kick_user.user_name, UserName, strlen(UserName)+1);
			}
			//大区
			req_kick_user.srv_realm_id=srv_Realmid;
			//服务器组ID
			req_kick_user.srv_cluster_id=srv_ClusterID;
			memcpy(sendbuf+4, &req_kick_user, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "GM踢人");
				return DBTFLV;
			}
			else
			{
				memset(recvbuf, 0, 10240);
				int recvbyte = m_MySocket.MyRecv(recvbuf, 10240);	//默认超时时间20秒
				if (recvbyte>0 && strcmp(recvbuf, ""))
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_kick_user, recvbuf+4, packetSize);
					
					switch (rep_kick_user.eResult)
					{
					case REP_KICK_USER::E_SUCCESS:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "踢人成功", "GM踢人");
						break;
					case REP_KICK_USER::E_FAILED_CHAR_NOTINGAME:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "角色不在线", "GM踢人");
						break;
					default:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "踢人失败", "GM踢人");
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "GM踢人超时", "GM踢人");
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "GM踢人");
		return DBTFLV;
	}	
	//if (!DBTFLV.empty())
	//	return DBTFLV;
	m_OperatorWA.GmOperLog(userByID, ServerIP, "GM踢人", retMessage, DBTFLV[0]);

	/////////////////////////////////////////////////////////////////////////////
		try
	{
		//Added by tengjie 2010-08-05
// 		New_Time.Format("%d", iTime/60);
// 		if(!UserLogInterface(ServerIP,UserName,New_Time.GetBuffer(0),38,"1",1).compare("RESULT_6"))
// 		{
// 			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "已自助封停", "帐号封停");
// 			WA_SqlForUserInfo(UserName,ServerIP,iTime,description);
// 			return DBTFLV;
// 		}

		REQ_CLOSE_ACCOUNT req_close_account;		//封停帐号请求
		REP_CLOSE_ACCOUNT rep_close_account;		//封停帐号回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];

		//Added by tengjie 2010-07-30
		CString New_Time = "";

		req_packetLength = sizeof(REQ_CLOSE_ACCOUNT);
		
		memset(sendbuf,0, 1024);
		memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
		memcpy(sendbuf+2, &flag, 2);			//固定字段
		/*
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "帐号封停");
		}
		else
		{
		*/
			sprintf(retMessage, "帐号封停：<大区>%s, <帐号>%s, <时间>%d分, <原因>%d, <描述>%s", ServerIP, UserName, iTime, Reason, description);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			if(!strcmp(UserName,""))	//角色封停
			{
				req_close_account.closeCondition = ADMIN::QC_CHARACTERNAME;
				memcpy(&req_close_account.char_name, Character, strlen(Character)+1);
				
			}
			else						//帐号封停
			{
				req_close_account.closeCondition = ADMIN::QC_USERNAME;
				memcpy(&req_close_account.user_name, UserName, strlen(UserName)+1);
			}

			switch (Reason)
			{
// 			case 0:	// 未知
// 				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_UNKNOWN;
// 				break;
			case 1:	// 系统封停
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_SYSTEM;
				break;
			case 2: // 用户自助
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_SYSTEM_TEMP;
				break;
			case 3: // 用户自助
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_USERCUSTOM;
				break;
			case 4: // 用户临时自助
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_USERCUSTOM_TEMP;
				break;
			default:// 未知
				req_close_account.eResult = REQ_CLOSE_ACCOUNT::R_SYSTEM;
				break;
			}
			//大区
			req_close_account.srv_realm_id=srv_Realmid;
			//服务器组ID
			req_close_account.srv_cluster_id=srv_ClusterID;

			req_close_account.duration_time = iTime;

			//Modified by tengjie 2010-05-13
//			strcpy(req_close_account.description, description);		
			strcpy(req_close_account.description, "");
			memcpy(sendbuf+4, &req_close_account, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "帐号封停");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				int recvbyte = m_MySocket.MyRecv(recvbuf, 10240);	//默认超时时间20秒
				if (recvbyte>0 && strcmp(recvbuf, ""))
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_close_account, recvbuf+4, packetSize);
					
					switch (rep_close_account.eResult)
					{
					case REP_CLOSE_ACCOUNT::E_SUCCESS:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "封停成功", "帐号封停");
						
						//Added by tengjie 2010-05-13
						WA_SqlForUserInfo(UserName,ServerIP,iTime,description);
						//Added by tengjie 2010-07-29:帐户中心记录日志接口
						New_Time.Format("%d", iTime/60);
						UserLogInterface(ServerIP,UserName,New_Time.GetBuffer(0),38,"1");
						break;
					case REP_CLOSE_ACCOUNT::E_FAILED_ACCOUNT_NOTEXIST:	
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "帐号不存在", "帐号封停");
						break;
					default:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "封停失败", "帐号封停");
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "帐号封停超时", "帐号封停");
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "帐号封停");
	}

	m_OperatorWA.GmOperLog(userByID, ServerIP, "帐号封停", retMessage, DBTFLV[0]);
	return DBTFLV;
}


//网络解封
vector <CGlobalStruct::TFLV> CWAInfo::WA_Net_OpenPlayer(int userByID,char * ServerIP,char * UserName,int CharacterID,char * Character)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{
		REQ_AVAILABLE_ACCOUNT req_available_account;		//解封帐号请求
		REP_AVAILABLE_ACCOUNT rep_available_account;		//解封帐号回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];

		req_packetLength = sizeof(REQ_AVAILABLE_ACCOUNT);
		
		memset(sendbuf,0, 1024);
		memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
		memcpy(sendbuf+2, &flag, 2);			//固定字段
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "帐号解封");
		}
		else
		{
			sprintf(retMessage, "帐号解封：<大区>%s, <帐号>%s", ServerIP, UserName);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			if(!strcmp(UserName,""))	//角色解封
			{
				req_available_account.availableCondition = ADMIN::QC_CHARACTERNAME;
				memcpy(&req_available_account.char_name, Character, strlen(Character)+1);
				
			}
			else						//帐号解封
			{
				req_available_account.availableCondition = ADMIN::QC_USERNAME;
				memcpy(&req_available_account.user_name, UserName, strlen(UserName)+1);
			}
			//大区
			req_available_account.srv_realm_id=srv_Realmid;
			//服务器组ID
			req_available_account.srv_cluster_id=srv_ClusterID;
			memcpy(sendbuf+4, &req_available_account, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "帐号解封");
			}
			else
			{
				memset(recvbuf, 0, 10240);
				int recvbyte = m_MySocket.MyRecv(recvbuf, 10240);	//默认超时时间20秒
				if (recvbyte>0 && strcmp(recvbuf, ""))
				{
					short int packetSize = *(short int *)recvbuf;
					memcpy(&rep_available_account, recvbuf+4, packetSize);
					
					switch (rep_available_account.eResult)
					{
					case REP_AVAILABLE_ACCOUNT::E_SUCCESS:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "解封成功", "帐号解封");
						WA_SqlForUserInfo(UserName,"",0,"",1);
						//Added by tengjie 2010-07-29:帐户中心记录日志接口
						UserLogInterface(ServerIP,UserName,"",38);
						break;
					case REP_AVAILABLE_ACCOUNT::E_FAILED_ACCOUNT_NOTEXIST:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "帐号不存在", "帐号解封");
						break;
					default:
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "解封失败", "帐号解封");
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "帐号解封超时", "帐号解封");
				}
			}

			m_MySocket.Close();
		}

		delete []sendbuf;
		delete []recvbuf;
	}
	catch (...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "帐号解封");
	}

	m_OperatorWA.GmOperLog(userByID, ServerIP, "帐号解封", retMessage, DBTFLV[0]);
	return DBTFLV;
}

// add by ltx

//好友信息查询
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

		REQ_CHARACTER_INFO req_character_info;				//好友信息查询请求
		CHARACTER_FRIEND_INFO character_friend_info;		//好友信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];

		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "好友信息查询");			
		}
		else
		{
			sprintf(retMessage, "好友信息查询：<大区>%s, <帐号>%s, <角色名>%s", ServerIP, Account, Character);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG
			
			req_packetLength = sizeof(REQ_CHARACTER_INFO);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			req_character_info.req_type = REQ_CHARACTER_INFO::RT_CHAR_FRIEND_INFO;
			req_character_info.query_condition = ADMIN::QC_CHARACTERNAME;
			//大区
			req_character_info.srv_realm_id=srv_Realmid;
			//服务器组ID
			req_character_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_character_info.user_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_character_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "好友信息查询");
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
 					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_FriendInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&character_friend_info, recvbuf+offset+4, packetSize);
								
								//处理数据
								if (character_friend_info.allSend == 1)	//收到最后一个结构包
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

				//处理结果集
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
					// 好友角色ID
					_itoa((int)vect_FriendInfo[i].friendIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_friendIndex;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 好友名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_friendName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_FriendInfo[i].friend_name.charName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_FriendInfo[i].friend_name.charName);
					DBTFLV.push_back(m_tflv);

					// 好友等级
//  					_itoa((int)vect_FriendInfo[i].friendLevel, strInt, 10);
//  					m_tflv.nIndex = DBTFLV.size()+1;
// 					m_tflv.m_tagName = CEnumCore::TagName::WA_friendLevel;
//  					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
// 					m_tflv.m_tvlength = strlen(strInt)+1;
//  					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
//  					DBTFLV.push_back(m_tflv);

					// 好友的角色类型
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::WA_friendTypeId;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					switch (vect_FriendInfo[i].friendTypeId)
					{
					case 1:
						m_tflv.m_tvlength = strlen("男剑士")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"男剑士");							
						break;
					case 2:
						m_tflv.m_tvlength = strlen("女剑士")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"女剑士");
						break;
					case 3:
						m_tflv.m_tvlength = strlen("男武者")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"男武者");
						break;
					case 4:
						m_tflv.m_tvlength = strlen("女武者")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"女武者");
						break;
					case 5:
						m_tflv.m_tvlength = strlen("男战法师")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"男战法师");
						break;
					case 6:
						m_tflv.m_tvlength = strlen("女战法师")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"女战法师");
						break;
					case 7:
						m_tflv.m_tvlength = strlen("男魔剑士")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"男魔剑士");
						break;
					case 8:
						m_tflv.m_tvlength = strlen("女魔剑士")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"女魔剑士");
						break;
					case 9:
						m_tflv.m_tvlength = strlen("男气功师")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"男气功师");
						break;
					case 10:
						m_tflv.m_tvlength = strlen("女气功师")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"女气功师");
						break;
					case 11:
						m_tflv.m_tvlength = strlen("男摄魂师")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"男摄魂师");
						break;
					case 12:
						m_tflv.m_tvlength = strlen("女摄魂师")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"女摄魂师");
						break;
					case 13:
						m_tflv.m_tvlength = strlen("男狂剑士")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"男狂剑士");
						break;
					case 14:
						m_tflv.m_tvlength = strlen("女狂剑士")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"女狂剑士");
						break;
					case 15:
						m_tflv.m_tvlength = strlen("男鬼武")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"男鬼武");
						break;
					case 16:
						m_tflv.m_tvlength = strlen("女鬼武")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"女鬼武");
						break;
					case 17:
						m_tflv.m_tvlength = strlen("男元素使")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"男元素使");
						break;
					case 18:
						m_tflv.m_tvlength = strlen("女元素使")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"女元素使");
						break;
					case 19:
						m_tflv.m_tvlength = strlen("男圣骑士")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"男圣骑士");
						break;
					case 20:
						m_tflv.m_tvlength = strlen("女圣骑士")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"女圣骑士");
						break;
					case 21:
						m_tflv.m_tvlength = strlen("男武道家")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"男武道家");
						break;
					case 22:
						m_tflv.m_tvlength = strlen("女武道家")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"女武道家");
						break;
					case 23:
						m_tflv.m_tvlength = strlen("男神官")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"男神官");
						break;
					case 24:
						m_tflv.m_tvlength = strlen("女神官")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"女神官");
						break;
					default:
						m_tflv.m_tvlength = strlen("未知类型")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"未知类型");
						break;
					}
					DBTFLV.push_back(m_tflv);

					// 好友等级
					_itoa((int)vect_FriendInfo[i].friendLevel, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_friendLevel;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 亲密度
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::WA_friendIni;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					if (vect_FriendInfo[i].friendIni == 0)
					{
						m_tflv.m_tvlength = strlen("陌生")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"陌生");
					}
					else if (vect_FriendInfo[i].friendIni>=1 && vect_FriendInfo[i].friendIni<= 99)
					{
						m_tflv.m_tvlength = strlen("相识")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"相识");
					}
					else if (vect_FriendInfo[i].friendIni>=100 && vect_FriendInfo[i].friendIni<= 299)
					{
						m_tflv.m_tvlength = strlen("友好")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"友好");
					}
					else if (vect_FriendInfo[i].friendIni>=300 && vect_FriendInfo[i].friendIni<= 599)
					{
						m_tflv.m_tvlength = strlen("熟悉")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"熟悉");
					}
					else if (vect_FriendInfo[i].friendIni>=600 && vect_FriendInfo[i].friendIni<= 1499)
					{
						m_tflv.m_tvlength = strlen("信任")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"信任");
					}
					else if (vect_FriendInfo[i].friendIni>=1500 && vect_FriendInfo[i].friendIni<= 3499)
					{
						m_tflv.m_tvlength = strlen("知己")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"知己");
					}
					else if (vect_FriendInfo[i].friendIni>=3500 && vect_FriendInfo[i].friendIni<= 5000)
					{
						m_tflv.m_tvlength = strlen("敬重/恩爱")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"敬重/恩爱");
					}
					else if (vect_FriendInfo[i].friendIni>5000)
					{
						m_tflv.m_tvlength = strlen("无")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"无");
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "好友信息查询");
	}
	
	return DBTFLV;
}

//角色物品信息查询
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

		REQ_CHARACTER_INFO req_character_info;				//角色身上物品信息查询请求
		CHARACTER_ITEM_INFO character_item_info;			//角色身上物品信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "装备信息查询");							
		}
		else
		{
			sprintf(retMessage, "物品信息查询：<大区>%s, <帐号>%s, <角色名>%s, <物品位置>%d", ServerIP, Account, Character, itemPos);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_CHARACTER_INFO);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//判断装备位置
			switch (itemPos)
			{
			case ADMIN::EG_EQUIPMENT_ITEM:	//身上
				req_character_info.req_type = REQ_CHARACTER_INFO::RT_CHAR_EQUIPMENT_INFO;
				break;
			case ADMIN::EG_BAG_ITEM:		//背包
				req_character_info.req_type = REQ_CHARACTER_INFO::RT_CHAR_ITEM_INFO;
				break;
			case ADMIN::EG_BANK_ITEM:		//银行
				req_character_info.req_type = REQ_CHARACTER_INFO::RT_CHAR_BANK_INFO;
				break;
			}
			
			req_character_info.query_condition = ADMIN::QC_CHARACTERNAME;
			//大区
			req_character_info.srv_realm_id=srv_Realmid;
			//服务器组ID
			req_character_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_character_info.user_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_character_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "物品信息查询");					
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_ItemInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&character_item_info, recvbuf+offset+4, packetSize);
								
								//处理数据
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

				//处理结果集
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
					// 序号
					_itoa((int)vect_ItemInfo[i].itemIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemIndex;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 类型ID
					_itoa((int)vect_ItemInfo[i].itemTypeId, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ItemID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_ItemInfo[i].itemName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_ItemInfo[i].itemName);
					DBTFLV.push_back(m_tflv);

					// 唯一ID
					_ui64toa(vect_ItemInfo[i].itemUniqueId, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemUniqueId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 数量
					_itoa((int)vect_ItemInfo[i].itemCount, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemCount;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 耐久
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "物品查询");
	}

	return DBTFLV;
}

//徽章信息查询
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

		REQ_CHARACTER_INFO req_character_info;				//徽章信息查询请求
		CHARACTER_MEDAL_INFO character_medal_info;		//徽章信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "徽章信息查询");			
		}
		else
		{
			sprintf(retMessage, "徽章信息查询：<大区>%s, <帐号>%s, <角色名>%s", ServerIP, Account, Character);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_CHARACTER_INFO);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			req_character_info.req_type = REQ_CHARACTER_INFO::RT_CHAR_MEDAL_INFO;
			req_character_info.query_condition = ADMIN::QC_CHARACTERNAME;
			//大区
			req_character_info.srv_realm_id=srv_Realmid;
			//服务器组ID
			req_character_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_character_info.user_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_character_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "徽章信息查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_medalInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&character_medal_info, recvbuf+offset+4, packetSize);
								
								//处理数据
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

				//处理结果集
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
					// 类型ID
					_itoa((int)vect_medalInfo[i].medalTypeId, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_medalTypeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 徽章名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_medalName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_medalInfo[i].medalName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_medalInfo[i].medalName);
					DBTFLV.push_back(m_tflv);

					// 耐久
					_itoa((int)vect_medalInfo[i].medalDur, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_medalDur;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 培差度
					sprintf(strInt, "%f", vect_medalInfo[i].medalBringUp);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_medalBringUp;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 宠物序号超所值255为无宠物
					_itoa((int)vect_medalInfo[i].medalPetIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_medalPetIndex;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 徽章在宠物背包中位置 255为装备中
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "徽章信息查询");
	}

	return DBTFLV;
}

//宠物信息查询
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

		REQ_CHARACTER_INFO req_character_info;				//角色银行信息查询请求
		CHARACTER_PET_INFO character_pet_info;				//角色银行信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "宠物信息查询");
		}
		else
		{
			sprintf(retMessage, "宠物信息查询：<大区>%s, <帐号>%s, <角色名>%s, <宠物编号>%d", ServerIP, Account, Character, petNo);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_CHARACTER_INFO);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			req_character_info.req_type = REQ_CHARACTER_INFO::RT_CHAR_PET_INFO;
			req_character_info.query_condition = ADMIN::QC_CHARACTERNAME;
			//大区
			req_character_info.srv_realm_id=srv_Realmid;
			//服务器组ID
			req_character_info.srv_cluster_id=srv_ClusterID;
			req_character_info.param1 = petNo;	//宠物序号
			memcpy(&req_character_info.user_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_character_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "宠物信息查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_petInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&character_pet_info, recvbuf+offset+4, packetSize);
								
								//处理数据
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

				//处理结果集
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
					// 类型ID
					_itoa(vect_petInfo[i].petTypeId, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_petTypeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//Added by tengjie 2010-08-20
					// 宠物唯一ID
					_itoa(vect_petInfo[i].pet_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_PetUniqId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 宠物名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_petName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_petInfo[i].petName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_petInfo[i].petName);
					DBTFLV.push_back(m_tflv);

					// 等级
					_itoa(vect_petInfo[i].petLevel, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_petLevel;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 性格
					_itoa(vect_petInfo[i].petNature, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_petNature;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 成长率
					sprintf(strInt, "%f", vect_petInfo[i].petGrowth);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_petGrowth;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 健康度
					_itoa(vect_petInfo[i].healthe, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_healthe;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 物理攻击
					_itoa(vect_petInfo[i].physical_attack, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_PhysicalAttack;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//物理防御
					_itoa(vect_petInfo[i].physical_defense, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_PhysicalDefense;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//魔法攻击
					_itoa(vect_petInfo[i].magic_attack, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MagicAttack;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					//魔法防御
					_itoa(vect_petInfo[i].magic_defense, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MagicDefense;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//火焰攻击
					_itoa(vect_petInfo[i].fire_attack, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_FireAttack;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//冰冻攻击
					_itoa(vect_petInfo[i].cold_attack, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ColdAttack;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//毒素攻击
					_itoa(vect_petInfo[i].poison_attack, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_PoisonAttack;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//火焰抗性
					sprintf(strInt, "%f", vect_petInfo[i].fire_resist);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_FireResist;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//冰冻抗性
					sprintf(strInt, "%f", vect_petInfo[i].cold_resist);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ColdResist;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//毒素抗性
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "宠物信息查询");
	}

	return DBTFLV;
}

//封停列表查询
vector <CGlobalStruct::TFLV> CWAInfo::WA_Net_CloseList(char *ServerIP, char *GameRegion, char *UserName, int iIndex, int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	vector <NAME_ID_ELEM> vect_CloseUser;

	try
	{
		logFile.WriteLog("<勇士无双>",operName,"封停列表查询");//记LOG
		
		int iPageCount=0;
		
		if(!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录服务器失败", "封停列表查询");
		}
		else
		{
			if(strcmp(UserName,""))
			{
				//根据帐号查询详细封停信息
				WA_Net_SingleCloseInfo(UserName, &DBTFLV, 1);
			}
			else
			{
				REQ_CLOSURE_ACCOUNT_INFO req_closure_account_info;
				REP_NAME_ID_LIST rep_name_id_list;					//帐号封停查询请求
				short int req_packetLength;
				short int flag = 24;
				char *sendbuf = new char[1024];
				char * recvbuf = new char[20480];
				
				req_packetLength = sizeof(REQ_CLOSURE_ACCOUNT_INFO);
				
				memset(sendbuf,0, 1024);
				memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
				memcpy(sendbuf+2, &flag, 2);			//固定字段

				req_closure_account_info.reqRange = REQ_CLOSURE_ACCOUNT_INFO::RR_ALL_CLOSUREACCOUNT;
				//大区
				req_closure_account_info.srv_realm_id=srv_Realmid;
				//服务器组ID
				req_closure_account_info.srv_cluster_id=srv_ClusterID;
				memcpy(sendbuf+4, &req_closure_account_info, req_packetLength);	
				
				if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "封停列表查询");
				}
				else
				{

					bool bRecvFlag = true;	//是否需要继续接收数据包
					while (bRecvFlag)
					{
						//memset(recvbuf, 0, 20480);
						*recvbuf = NULL;
						
						// 设置接收缓冲区大小
						DWORD nRecvBuf=20*1024;
						setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
						Sleep(1000);
						int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
						if ( recvbyte>0 && (strcmp(recvbuf, "") && vect_CloseUser.size() == 0) )
						{
							short int packetSize = 0;	//每个结构体数据包长
							int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
							int offset = 0;				//指针偏移量
							
							while (nLeft>0)
							{
								packetSize = *(short int *)(recvbuf+offset);
								
								if (nLeft<packetSize+4 || packetSize <= 0)
								{
									break;
								}
								else
								{
									//取出数据包
									memcpy(&rep_name_id_list, recvbuf+offset+4, packetSize);
									
									//处理数据
									if (rep_name_id_list.allSend == 1)	//收到最后一个结构包
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
					
					//处理结果集
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

//内部调用查询单个帐号封停
void CWAInfo::WA_Net_SingleCloseInfo(char *UserName, vector <CGlobalStruct::TFLV>* pDBTFLV, int PageCount)
{
	try
	{
		CGlobalStruct::TFLV m_tflv;
		REQ_CLOSURE_ACCOUNT_INFO req_closure_account_info;					//帐号封停查询请求
		REP_CLOSURE_ACCOUNT_INFO rep_closure_account_info;					//帐号封停查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		char strInt[30];
		char strDateTime[30];
		
		req_packetLength = sizeof(REQ_CLOSURE_ACCOUNT_INFO);
		
		memset(sendbuf,0, 1024);
		memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
		memcpy(sendbuf+2, &flag, 2);			//固定字段

		req_closure_account_info.reqRange = REQ_CLOSURE_ACCOUNT_INFO::RR_PER_CLOSUREACCOUNT;
		//大区
		req_closure_account_info.srv_realm_id=srv_Realmid;
		//服务器组ID
		req_closure_account_info.srv_cluster_id=srv_ClusterID;
		memcpy(&req_closure_account_info.user_name, UserName, strlen(UserName)+1);
		memcpy(sendbuf+4, &req_closure_account_info, req_packetLength);		
		
		if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
		{
			logFile.WriteLog("勇士无双",operName, "单个帐号封停查询请求失败");
		}
		else
		{
			memset(recvbuf, 0, 10240);
			int recvbyte = m_MySocket.MyRecv(recvbuf, 10240);	//默认超时时间20秒
			if (recvbyte>0 && strcmp(recvbuf, ""))
			{
				short int packetSize = *(short int *)recvbuf;						
				
				memcpy(&rep_closure_account_info, recvbuf+4, packetSize);

				switch (rep_closure_account_info.eResult)
				{
				case REP_CLOSURE_ACCOUNT_INFO::E_SUCCESS:
					{
						// 帐号名
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Account;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength=strlen(rep_closure_account_info.user_name.userName)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",rep_closure_account_info.user_name.userName);
						pDBTFLV->push_back(m_tflv);

						// 帐号ID
						_itoa((int)rep_closure_account_info.accountIndex,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_AccountID;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
						m_tflv.m_tvlength = strlen(strInt)+1;
						sprintf((LPSTR)&m_tflv.lpdata,"%s",strInt);
						pDBTFLV->push_back(m_tflv);

						//封停原因
//						_itoa((int)rep_closure_account_info.closure_reason,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Reason;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						switch (rep_closure_account_info.closure_reason)
						{
						case 1:
							m_tflv.m_tvlength = strlen("系统封停")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"系统封停");
							break;
						case 2:
							m_tflv.m_tvlength = strlen("系统临时封停")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"系统临时封停");
							break;
						case 3:
							m_tflv.m_tvlength = strlen("用户自助")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"用户自助");
							break;
						case 4:
							m_tflv.m_tvlength = strlen("用户临时自助")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"用户临时自助");
							break;
						default:
							m_tflv.m_tvlength = strlen("未知原因")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"未知原因");
							break;

						}
						pDBTFLV->push_back(m_tflv);

						//Added by tengjie 2010-05-13:封停描述
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_Content;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						if (!rep_closure_account_info.is_clousure)
							WA_SqlForUserInfo(UserName,"",0,"",1);
						CString szDes = WA_SearchDescription(UserName,2);
						m_tflv.m_tvlength = szDes.GetLength();
						sprintf((LPSTR)&m_tflv.lpdata,"%s",szDes.GetBuffer(0));
						pDBTFLV->push_back(m_tflv);

						//是否封停
//						_itoa((int)rep_closure_account_info.is_clousure,strInt,10);
						m_tflv.nIndex=pDBTFLV->size()+1;
						m_tflv.m_tagName=CEnumCore::TagName::WA_IsClosed;
						m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
						if (rep_closure_account_info.is_clousure)
						{
							m_tflv.m_tvlength = strlen("已封停")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"已封停");
						}
						else
						{
							m_tflv.m_tvlength = strlen("未封停")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"未封停");
						}
						pDBTFLV->push_back(m_tflv);

						//上次登录时间
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
						
						// 封停结束时间
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
					logFile.WriteLog("勇士无双",operName, "单个帐号封停查询返回结果错误");
					break;
				}
			}
		}

		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		logFile.WriteLog("勇士无双",operName, "单个帐号封停查询未知错误");		
	}
}

//道具删除
vector <CGlobalStruct::TFLV> CWAInfo::WA_Net_DeleteItem(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character, int itemIndex, int itemPos)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);

	try
	{

		REQ_DELETE_ITEM req_delete_item;				//道具删除请求
		REP_DELETE_ITEM rep_delete_item;				//道具删除回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录服务器失败", "道具删除");				
		}
		else
		{		
			switch (itemPos)
			{
			case 0:	//身上
				req_delete_item.itemGroup = ADMIN::EG_EQUIPMENT_ITEM;
				sprintf(retMessage, "玩家道具删除：<大区>%s, <帐号>%s, <角色名>%s, <道具编号>%d, <道具位置>%s", ServerIP, UserName, Character, itemIndex, "身上");
				break;
			case 1:	//背包
				req_delete_item.itemGroup = ADMIN::EG_BAG_ITEM;
				sprintf(retMessage, "玩家道具删除：<大区>%s, <帐号>%s, <角色名>%s, <道具编号>%d, <道具位置>%s", ServerIP, UserName, Character, itemIndex, "背包");
				break;
			case 2:	//银行
				req_delete_item.itemGroup = ADMIN::EG_BANK_ITEM;
				sprintf(retMessage, "玩家道具删除：<大区>%s, <帐号>%s, <角色名>%s, <道具编号>%d, <道具位置>%s", ServerIP, UserName, Character, itemIndex, "银行");
				break;
			}

			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_DELETE_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			req_delete_item.itemIndex = itemIndex;
			//大区
			req_delete_item.srv_realm_id=srv_Realmid;
			//服务器组ID
			req_delete_item.srv_cluster_id=srv_ClusterID;
			memcpy(&req_delete_item.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_delete_item, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "道具删除");				
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
					case REP_DELETE_ITEM::E_SUCCESS:	//成功
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "成功", "道具删除");
						break;
					case REP_DELETE_ITEM::E_NOT_FOUND:	//没有该道具
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "没有该道具", "道具删除");
						break;
					default:							//失败
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "失败", "道具删除");
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "道具删除超时", "道具删除");
				}
			}
			
			//m_MySocket.Close();
		}

		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "道具删除");
	}

	m_OperatorWA.GmOperLog(userByID, ServerIP, "道具删除", retMessage, DBTFLV[0]);
	return DBTFLV;
}
//修改等级
vector <CGlobalStruct::TFLV> CWAInfo::WA_Net_ModifyLevel(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character, int level)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	
	try
	{
		
		REQ_MODIFY_CHARACTER req_modify_character;				//修改金钱请求
		REP_MODIFY_CHARACTER rep_modify_character;				//修改金钱回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录服务器失败", "修改玩家等级");	
		}
		else
		{
			sprintf(retMessage, "修改玩家金钱数：<大区>%s, <帐号>%s, <角色名>%s, <修改后玩家等级>%d", ServerIP, UserName, Character, level);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG
			
			req_packetLength = sizeof(REQ_MODIFY_CHARACTER);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			//大区
			req_modify_character.srv_realm_id=srv_Realmid;
			//服务器组ID
			req_modify_character.srv_cluster_id=srv_ClusterID;
			req_modify_character.attr = ADMIN::MCA_LEVEL;
			req_modify_character.newValue = level;
			memcpy(&req_modify_character.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_modify_character, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "修改玩家等级");
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
					case REP_DELETE_ITEM::E_SUCCESS:	//成功
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "修改玩家等级成功", "修改玩家等级");	
						break;
					default:							//失败
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "修改玩家等级失败", "修改玩家等级");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "修改玩家等级超时", "修改玩家等级");
				}
			}
			
			m_MySocket.Close();
		}

		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "修改玩家等级");		
	}

	m_OperatorWA.GmOperLog(userByID, ServerIP, "修改玩家等级", retMessage, DBTFLV[0]);
	return DBTFLV;
}
//修改金钱
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

		REQ_MODIFY_CHARACTER req_modify_character;				//修改金钱请求
		REP_MODIFY_CHARACTER rep_modify_character;				//修改金钱回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录服务器失败", "修改玩家金钱");	
		}
		else
		{
			sprintf(retMessage, "修改玩家金钱数：<大区>%s, <帐号>%s, <角色名>%s, <修改前金钱数量>%d, <修改后金钱数量>%d", ServerIP, UserName, Character, CurMoney, newMoney);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG
			
			req_packetLength = sizeof(REQ_MODIFY_CHARACTER);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			//大区
			req_modify_character.srv_realm_id=srv_Realmid;
			//服务器组ID
			req_modify_character.srv_cluster_id=srv_ClusterID;
			req_modify_character.attr = ADMIN::MCA_MONEY;
			req_modify_character.newValue = newMoney;
			memcpy(&req_modify_character.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_modify_character, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "修改玩家金钱");
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
					case REP_DELETE_ITEM::E_SUCCESS:	//成功
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "修改金钱成功", "修改玩家金钱");	
						break;
					default:							//失败
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "修改金钱失败", "修改玩家金钱");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "修改金钱超时", "修改玩家金钱");
				}
			}
			
			m_MySocket.Close();
		}
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "修改玩家金钱");		
	}

	m_OperatorWA.GmOperLog(userByID, ServerIP, "修改玩家金钱", retMessage, DBTFLV[0]);
	return DBTFLV;
}
//公告添加
vector <CGlobalStruct::TFLV> CWAInfo::WA_BoardTasker_Add(int userByID, char * ServerIP, char *BoardMessage, char *BeginTime, char *EndTime, int interval)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	
	try
	{
		logFile.WriteLog("<勇士无双>",operName,"公告管理");//记LOG
		
		printf("exec WA_BOARDTASK_INSERT %i,'%s','%s','%s','%s',%i", 
			userByID,ServerIP,BoardMessage,BeginTime, EndTime, interval);
		
		int tmpLength = 0;
		tmpLength = GameDBOper("WA", LocalIP, 4, "exec WA_BOARDTASK_INSERT %i,'%s','%s','%s','%s',%i",
			userByID,ServerIP,BoardMessage,BeginTime, EndTime, interval);
		
		if(tmpLength == 0)
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "添加公告失败", "公告管理");
		}
		else
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "添加公告成功", "公告管理");
		}
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "添加公告错误", "公告管理");
	}
	
	return DBTFLV;
}

//公告修改
vector <CGlobalStruct::TFLV> CWAInfo::WA_BoardTasker_Update(int userByID, char * ServerIP, int Taskid, char *BoardMessage, char *BeginTime, char *EndTime, int interval)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	
	try
	{
		logFile.WriteLog("<勇士无双>",operName,"公告管理");//记LOG
		printf("exec WA_BOARDTASK_UPDATE %i, %i, '%s','%s','%s','%s',%i", 
			userByID,Taskid, ServerIP,BoardMessage,BeginTime, EndTime, interval);	
		int tmpLength = 0;
		tmpLength = GameDBOper("XDJG", LocalIP, 4, "exec WA_BOARDTASK_UPDATE %i, %i, '%s','%s','%s','%s',%i", 
			userByID,Taskid, ServerIP,BoardMessage,BeginTime, EndTime, interval);
		
		if(tmpLength == 0)
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "更新公告失败", "公告管理");
		}
		else
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "更新公告成功", "公告管理");
		}
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "更新公告错误", "公告管理");
	}
	
	return DBTFLV;
}
//公告删除	
vector <CGlobalStruct::TFLV> CWAInfo::WA_BoardTasker_Delete(int userByID, int Taskid)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{
		logFile.WriteLog("<勇士无双>",operName,"公告管理");//记LOG
		
		int tmpLength = 0;
		tmpLength = GameDBOper("XDJG","127.0.0.1",4,"exec WA_BOARDTASK_DELETE %i, %i", 
			userByID,Taskid);
		if(tmpLength == 0)
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "删除公告失败", "公告管理");
		}
		else
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "删除公告成功", "公告管理");
		}
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "删除公告错误", "公告管理");
	}
	
	return DBTFLV;
}
//模糊道具查询
vector<CGlobalStruct::TFLV> CWAInfo::WA_ITEM_QUERY(char* gameName,char* strSql,char* Item_Type,char* Item_Name,int index,int Page)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{		
		DBTFLV=GameDBQuery("WA","127.0.0.1",4,index,Page,"exec WA_Item_Query '%s','%s'",Item_Type,Item_Name);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("WA","模糊道具查询内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("WA","模糊道具查询文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("WA","模糊道具查询其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("WA",(char *)err.what());
		return DBTFLV;
	}
	return DBTFLV;
}
//道具类型查询
vector<CGlobalStruct::TFLV> CWAInfo::WA_ITEMType_QUERY(char* gameName,char* strSql,int index,int Page)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{
		DBTFLV=GameDBQuery("WA","127.0.0.1",4,0,0,"exec WA_ItemType_Query");
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("WA","道具类型查询内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("WA","道具类型查询文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("WA","道具类型查询其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("WA",(char *)err.what());
		return DBTFLV;
	}
	return DBTFLV;
}
//寄送道具
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
// 		REQ_MAIL_ITEM req_SendGift;		//寄送物品请求
// 		REP_MAIL_ITEM rep_SendGift;		//寄送物品回应
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
// 			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录服务器失败", "寄送玩家道具");	
// 		}
// 		else
// 		{
// 			sprintf(retMessage, "添加玩家道具：<大区>%s, <帐号>%s, <角色名>%s, <道具ID><%s>, <寄送道具名><%s>,<寄送道具数量>%s", ServerIP, UserName, ptempVecChar->c_str(),itemId,itemName,itemCount);			
// 			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG
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
// 				memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
// 				memcpy(sendbuf+2, &flag, 2);			//固定字段
// 				
// 				//Added by tengjie 2010-06-24
// 				ZeroMemory(req_SendGift.item_type_id_list, sizeof(req_SendGift.item_type_id_list));
// 				ZeroMemory(req_SendGift.item_count_list, sizeof(req_SendGift.item_count_list));
// 				ZeroMemory(req_SendGift.item_gen_list, sizeof(req_SendGift.item_gen_list));
// 	//			ZeroMemory(req_SendGift.item_expire, sizeof(req_SendGift.item_expire));
// 
// 				sprintf(retMessage, "添加玩家道具：<大区>%s, <帐号>%s, <角色名>%s, <道具ID><%s>, <寄送道具名><%s>,<寄送道具数量>%s", ServerIP, UserName, ptempVecChar->c_str(),itemId,itemName,itemCount);			
// 				logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG
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
// 					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "寄送玩家道具失败(发送类型错误)", "寄送玩家道具");
// 					return DBTFLV;
// 				}
// 				
// 				memcpy(req_SendGift.subject, MailSub, strlen(MailSub)+1);
// 				memcpy(req_SendGift.body, MailContent, strlen(MailContent)+1);
// 				req_SendGift.money = iMoney;
// 
// 				//大区
// 				req_SendGift.srv_realm_id=srv_Realmid;
// 				//服务器组ID
// 				req_SendGift.srv_cluster_id=srv_ClusterID;
// 				memcpy(sendbuf+4, &req_SendGift, req_packetLength);
// 				if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
// 				{
// 					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "寄送玩家道具");
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
// 						case REP_MAIL_ITEM::E_SUCCESS:	//成功
// 							DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "寄送玩家道具成功", "寄送玩家道具");	
// 							break;
// 						case REP_MAIL_ITEM::E_TOO_MUCH:	//群发请求过多
// 							DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "寄送玩家道具群发请求过多", "寄送玩家道具");	
// 							break;
// 						default:							//失败
// 							DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "寄送玩家道具失败", "寄送玩家道具");	
// 							break;
// 						}
// 					}
// 					else
// 					{
// 						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "寄送玩家道具超时", "寄送玩家道具");
// 					}
// 				}
// 
// 				m_OperatorWA.GmOperLog(userByID, ServerIP, "寄送玩家道具", retMessage, DBTFLV[0]);
// 			}
// 			m_MySocket.Close();
// 		}
// 		delete []sendbuf;
// 		delete []recvbuf;
// 	}
// 	catch(...)
// 	{
// 		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "寄送玩家道具");
// 		m_OperatorWA.GmOperLog(userByID, ServerIP, "寄送玩家道具", retMessage, DBTFLV[0]);
// 	}
// 	
// //	m_OperatorWA.GmOperLog(userByID, ServerIP, "寄送玩家道具", retMessage, DBTFLV[0]);
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
		REQ_MAIL_ITEM req_SendGift;		//寄送物品请求
		REP_MAIL_ITEM rep_SendGift;		//寄送物品回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		char *pTemp = NULL;
		int iCount = 0;
		CString sztemp = "";
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录服务器失败", "寄送玩家道具");	
		}
		else
		{
			req_packetLength = sizeof(REQ_MAIL_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//Added by tengjie 2010-06-24
			ZeroMemory(req_SendGift.item_type_id_list, sizeof(req_SendGift.item_type_id_list));
			ZeroMemory(req_SendGift.item_count_list, sizeof(req_SendGift.item_count_list));
			ZeroMemory(req_SendGift.item_gen_list, sizeof(req_SendGift.item_gen_list));
			ZeroMemory(req_SendGift.item_expire, sizeof(req_SendGift.item_expire));

			sprintf(retMessage, "添加玩家道具：<大区>%s, <帐号>%s, <角色名>%s, <道具ID><%s>, <寄送道具名><%s>,<寄送道具数量>%s", ServerIP, UserName, Character,itemId,itemName,itemCount);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

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
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "寄送玩家道具失败(发送类型错误)", "寄送玩家道具");
				return DBTFLV;
			}
			
			memcpy(req_SendGift.subject, MailSub, strlen(MailSub)+1);
			memcpy(req_SendGift.body, MailContent, strlen(MailContent)+1);
			req_SendGift.money = iMoney;

			//大区
			req_SendGift.srv_realm_id=srv_Realmid;
			//服务器组ID
			req_SendGift.srv_cluster_id=srv_ClusterID;
			memcpy(sendbuf+4, &req_SendGift, req_packetLength);
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "寄送玩家道具");
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
					case REP_MAIL_ITEM::E_SUCCESS:	//成功
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "寄送玩家道具成功", "寄送玩家道具");	
						break;
					case REP_MAIL_ITEM::E_TOO_MUCH:	//群发请求过多
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "寄送玩家道具群发请求过多", "寄送玩家道具");	
						break;
					default:							//失败
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "寄送玩家道具失败", "寄送玩家道具");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "寄送玩家道具超时", "寄送玩家道具");
				}
			}
			m_MySocket.Close();
		}
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "寄送玩家道具");		
	}
	
	m_OperatorWA.GmOperLog(userByID, ServerIP, "寄送玩家道具", retMessage, DBTFLV[0]);
	return DBTFLV;
}


// vector <CGlobalStruct::TFLV> CWAInfo::WA_SendMailItem(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character, int itemId,int itemCount,char *itemName)
// {
// 	vector <CGlobalStruct::TFLV> DBTFLV;
// 	char retMessage[255];
// 	memset(retMessage, 0, 255);
// 	try
// 	{
// 		REQ_MAIL_ITEM req_SendGift;		//寄送物品请求
// 		REP_MAIL_ITEM rep_SendGift;		//寄送物品回应
// 		short int req_packetLength;
// 		short int flag = 24;
// 		char *sendbuf = new char[1024];
// 		char *recvbuf = new char[10240];
// 		
// 		if (!Connect_Remote_Login(ServerIP))
// 		{
// 			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录服务器失败", "寄送玩家道具");	
// 		}
// 		else
// 		{
// 			sprintf(retMessage, "添加玩家道具：<大区>%s, <帐号>%s, <角色名>%s, <寄送玩家道具名>%s,<寄送玩家数量>%d", ServerIP, UserName, Character,itemName,itemId);			
// 			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG
// 			req_packetLength = sizeof(REQ_MAIL_ITEM);
// 			
// 			memset(sendbuf,0, 1024);
// 			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
// 			memcpy(sendbuf+2, &flag, 2);			//固定字段
// 			memcpy(&req_SendGift.char_name, Character, strlen(Character)+1);
// 			//大区
// 			req_SendGift.srv_realm_id=srv_Realmid;
// 			//服务器组ID
// 			req_SendGift.srv_cluster_id=srv_ClusterID;
// 			req_SendGift.item_type_id=itemId;
// 			req_SendGift.item_count  =itemCount;
// 			memcpy(sendbuf+4, &req_SendGift, req_packetLength);
// 			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
// 			{
// 				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "寄送玩家道具");
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
// 					case REP_MAIL_ITEM::E_SUCCESS:	//成功
// 						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "寄送玩家道具成功", "寄送玩家道具");	
// 						break;
// 					default:							//失败
// 						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "寄送玩家道具失败", "寄送玩家道具");	
// 						break;
// 					}
// 				}
// 				else
// 				{
// 					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "寄送玩家道具超时", "寄送玩家道具金钱");
// 				}
// 			}
// 			m_MySocket.Close();
// 		}
// 		delete []sendbuf;
// 		delete []recvbuf;
// 	}
// 	catch(...)
// 	{
// 		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "寄送玩家道具");		
// 	}
// 	
// 	m_OperatorWA.GmOperLog(userByID, ServerIP, "寄送玩家道具", retMessage, DBTFLV[0]);
// 	return DBTFLV;
// }

//技能模糊查询
vector<CGlobalStruct::TFLV> CWAInfo::WA_SkillList_QUERY(char* gameName,char* strSql,char* Skill_Name,int index,int Page)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{		
		DBTFLV=GameDBQuery("WA","127.0.0.1",4,index,Page,"exec WA_SkillList_Query '%s'",Skill_Name);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("WA","模糊技能查询内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("WA","模糊技能查询文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("WA","模糊技能查询其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("WA",(char *)err.what());
		return DBTFLV;
	}
	return DBTFLV;
}
// 查询玩家角色身上技能信息
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
		REQ_CHARACTER_INFO req_Log_info;				//技能信息查询请求
		REP_CHARACTER_SKILL_INFO rep_Log_info;			//技能信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "身上技能信息查询");
		}
		else
		{
			sprintf(retMessage, "身上技能信息查询: <大区>%s, <帐号>%s, <角色名>%s", ServerIP, Account, Character);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_CHARACTER_INFO);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			//大区
			req_Log_info.srv_realm_id=srv_Realmid;
			//服务器组ID
			req_Log_info.srv_cluster_id=srv_ClusterID;
			req_Log_info.req_type = REQ_CHARACTER_INFO::RT_CHAR_SKILL_INFO;
			req_Log_info.query_condition = ADMIN::QC_CHARACTERNAME;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
				if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "玩家身上技能查询");
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
 					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&rep_Log_info, recvbuf+offset+4, packetSize);
								//处理数据
								if (rep_Log_info.allSend == 1)	//收到最后一个结构包
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

				//处理结果集
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
					// 技能ID
					_itoa((int)vect_LogInfo[i].skillId, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SkillID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//技能名称
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SkillName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].skillName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].skillName);
					DBTFLV.push_back(m_tflv);

					//技能等级
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "身上技能信息查询");
	}

	return DBTFLV;
}
//修改玩家技能
vector <CGlobalStruct::TFLV> CWAInfo::WA_ModfiyPlayerSkill(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character, int skillID,char *skillname,int skilllvl)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	try
	{
		
		REQ_MODIFY_CHARACTER_SKILL req_modifySkill;		//修改技能等级请求
		REP_MODIFY_CHARACTER_SKILL rep_modifySkill;		//修改技能等级的回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录服务器失败", "修改玩家技能等级");	
		}
		else
		{
			sprintf(retMessage, "修改玩家技能：<大区>%s, <帐号>%s, <角色名>%s, <修改玩家技能名>%s,<修改玩家技能等级>%d", ServerIP, UserName, Character,skillname,skilllvl);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG
			
			req_packetLength = sizeof(REQ_MODIFY_CHARACTER_SKILL);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			memcpy(&req_modifySkill.char_name, Character, strlen(Character)+1);
			//大区
			req_modifySkill.srv_realm_id=srv_Realmid;
			//服务器组ID
			req_modifySkill.srv_cluster_id=srv_ClusterID;
			req_modifySkill.skill_id=skillID;
			req_modifySkill.skill_level =skilllvl;
			memcpy(sendbuf+4, &req_modifySkill, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "修改玩家技能等级");
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
					case REP_MODIFY_CHARACTER_SKILL::E_SUCCESS:	//成功
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "修改玩家技能等级成功", "修改玩家技能");	
						break;
					default:							//失败
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "修改玩家技能等级失败", "修改玩家技能");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "修改玩家技能等级超时", "修改玩家技能等级");
				}
			}
			
		//	m_MySocket.Close();
		}

		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "修改玩家技能等级");		
	}

	m_OperatorWA.GmOperLog(userByID, ServerIP, "修改玩家技能等级", retMessage, DBTFLV[0]);
	m_MySocket.Close();
	return DBTFLV;
}

//恢复角色结果
vector <CGlobalStruct::TFLV> CWAInfo::WA_RestoreRoleInfo(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	try
	{
		
		REQ_RECOVER_CHARACTER req_RecoverCharacter;		//恢复角色请求
		REP_RECOVER_CHARACTER rep_RecoverCharacter;		//恢复角色回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录服务器失败", "恢复玩家角色");	
		}
		else
		{
			sprintf(retMessage, "恢复玩家：<大区>%s, <帐号>%s, <角色名>%s", ServerIP, UserName, Character);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG
			
			req_packetLength = sizeof(REQ_RECOVER_CHARACTER);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			memcpy(&req_RecoverCharacter.char_name, Character, strlen(Character)+1);
			//大区
			req_RecoverCharacter.srv_realm_id=srv_Realmid;
			//服务器组ID
			req_RecoverCharacter.srv_cluster_id=srv_ClusterID;
			memcpy(sendbuf+4, &req_RecoverCharacter, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "恢复玩家角色");
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
					case REP_RECOVER_CHARACTER::E_SUCCESS:	//成功
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "恢复玩家角色成功", "恢复玩家角色");	
						break;
					default:						//失败
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "恢复玩家角色失败", "恢复玩家角色");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "恢复玩家角色超时", "恢复玩家角色");
				}
			}
			
			m_MySocket.Close();
		}

		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "恢复玩家角色");		
	}

	m_OperatorWA.GmOperLog(userByID, ServerIP, "恢复玩家角色", retMessage, DBTFLV[0]);
	return DBTFLV;
}
//玩家密码查询
vector<CGlobalStruct::TFLV> CWAInfo::WA_UserPwd_QUERY(char* gameName,char* ServerIP,char* serverName,char* account,int index,int Page)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{
		DBTFLV=GameDBQuery("WA","127.0.0.1",4,0,0,"exec WA_UserPwd_Query %s",serverName,account);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("WA","道具类型查询内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("WA","道具类型查询文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("WA","道具类型查询其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("WA",(char *)err.what());
		return DBTFLV;
	}
	return DBTFLV;
}
//修改玩家的密码
vector <CGlobalStruct::TFLV> CWAInfo::WA_ChangeLoginPasswd(int userByID,char * ServerIP, char * UserName,int CharacterID,char * newPwd)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	try
	{
		
		REQ_MODIFY_ACCOUNT_PASSW req_ChangePwd;		//修改玩家密码请求
		REP_MODIFY_ACCOUNT_PASSW rep_ChangePwd;		//修改玩家密码回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		CString szTime = "";
		CString szSql = "";
		CSqlHelper m_SqlHelper;
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录服务器失败", "修改玩家密码");	
		}
		else
		{
			sprintf(retMessage, "修改玩家：<大区>%s, <帐号>%s, <新密码>%s", ServerIP, UserName, newPwd);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG
			
			req_packetLength = sizeof(REQ_MODIFY_ACCOUNT_PASSW);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			memcpy(&req_ChangePwd.user_name, UserName, strlen(UserName)+1);
			memcpy(&req_ChangePwd.new_password, newPwd, strlen(newPwd)+1);
			//大区
			req_ChangePwd.srv_realm_id=srv_Realmid;
			memcpy(sendbuf+4, &req_ChangePwd, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "修改玩家密码");
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
					case REP_MODIFY_ACCOUNT_PASSW::E_SUCCESS:	//成功
						SYSTEMTIME nowtime;
						GetLocalTime(&nowtime);
						szTime.Format("%4d-%2d-%2d %2d:%2d:%2d",nowtime.wYear,nowtime.wMonth,nowtime.wDay,nowtime.wHour,nowtime.wMinute,nowtime.wSecond);
						szSql.Format("exec WA_SAVE_OLDPASSWORD %i, '%s', '%s', '%s', '%s', '%s', %i", 
							userByID,ServerIP,UserName,rep_ChangePwd.old_password,newPwd,szTime.GetBuffer(0),0);
						m_SqlHelper.SqlHelperMain("WA","127.0.0.1", szSql.GetBuffer(0), 4);
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "修改玩家密码成功", "修改玩家密码");
						break;
					default:						//失败
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "修改玩家密码失败", "修改玩家密码");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "修改玩家密码超时", "修改玩家密码");
				}
			}
			
			m_MySocket.Close();
		}

		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "修改玩家密码");		
	}

	m_OperatorWA.InsertLocalDB(userByID,ServerIP,ServerIP,UserName,newPwd,newPwd);
	m_OperatorWA.GmOperLog(userByID, ServerIP, "修改玩家密码", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//Added by tengjie 2010-06-30
//恢复玩家密码
vector <CGlobalStruct::TFLV> CWAInfo::WA_ResumePassw(int userByID, char *ServerIP, char *Account)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	string strData = "";
	try
	{
		
		REQ_RECOVER_PASSW req_ResumePwd;		//恢复玩家密码请求
		REP_RECOVER_PASSW rep_ResumePwd;		//恢复玩家密码回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录服务器失败", "恢复玩家密码");	
		}
		else
		{
			sprintf(retMessage, "恢复玩家密码:<大区>%s, <帐号>%s", ServerIP, Account);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG
			
			req_packetLength = sizeof(REQ_RECOVER_PASSW);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段

			strData = WA_GetOldPassWord(ServerIP, Account);

			memcpy(&req_ResumePwd.user_name, Account, strlen(Account)+1);
			ZeroMemory(req_ResumePwd.old_password, sizeof(req_ResumePwd.old_password));
			memcpy(&req_ResumePwd.old_password, strData.c_str(), strData.length()+1);

			//大区
			req_ResumePwd.srv_realm_id=srv_Realmid;
			memcpy(sendbuf+4, &req_ResumePwd, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "恢复玩家密码");
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
					case REP_MODIFY_ACCOUNT_PASSW::E_SUCCESS:	//成功
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "恢复玩家密码成功", "恢复玩家密码");
						break;
					default:						//失败
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "恢复玩家密码失败", "恢复玩家密码");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "恢复玩家密码超时", "恢复玩家密码");
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "恢复玩家密码");		
	}
	
	m_OperatorWA.InsertLocalDB(userByID,ServerIP,ServerIP,Account,(char*)strData.c_str(),(char*)strData.c_str());
	m_OperatorWA.GmOperLog(userByID, ServerIP, "恢复玩家密码", retMessage, DBTFLV[0]);
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
		if(!strcmp("OldPwd", m_DataRecord.recordfieldname))//获取相应的ServerIP
		{
			szData = m_DataRecord.recorddata;//保存查询得到的最后一条记录
		}
	}
	
	return szData;
	
}
//End

//修改玩家的角色名
vector <CGlobalStruct::TFLV> CWAInfo::WA_RoleName_Modfiy(int userByID,char * ServerIP, char * UserName,char* Character,char * newRole)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	try
	{
		
		REQ_MODIFY_CHARACTER_NAME req_ChangeRole;		//修改玩家角色名
		REP_MODIFY_CHARACTER_NAME rep_ChangeRole;		//修改玩家角色名
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录服务器失败", "修改玩家角色名");	
		}
		else
		{
			sprintf(retMessage, "修改玩家：<大区>%s, <帐号>%s, <原角色名>%s,<角色名>%s", ServerIP, UserName, Character,newRole);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG
			
			req_packetLength = sizeof(REQ_MODIFY_CHARACTER_NAME);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			memcpy(&req_ChangeRole.char_name, Character, strlen(Character)+1);
			memcpy(&req_ChangeRole.new_char_name, newRole, strlen(newRole)+1);
			//大区
			req_ChangeRole.srv_realm_id=srv_Realmid;
			req_ChangeRole.srv_cluster_id=srv_ClusterID;
			memcpy(sendbuf+4, &req_ChangeRole, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "修改玩家角色名");
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
					case REP_MODIFY_CHARACTER_NAME::E_SUCCESS:	//成功
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "修改玩家角色名成功", "修改玩家角色名");	
						break;
					default:						//失败
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "修改玩家角色名失败", "修改玩家角色名");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "修改玩家角色名超时", "修改玩家角色名");
				}
			}
			
			m_MySocket.Close();
		}

		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "修改玩家角色名");		
	}

	m_OperatorWA.GmOperLog(userByID, ServerIP, "修改玩家角色名", retMessage, DBTFLV[0]);
	return DBTFLV;
}
//=====================================================================log========================================================================================

// 物品:怪物掉落
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
		REQ_LOG_MONSTER_DROP_ITEM req_Log_info;				//日志信息查询请求
		REP_LOG_MONSTER_DROP_ITEM res_Log_info;				//日志信息查询回应
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
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "怪物掉落物品信息查询");
		}
		else
		{
			sprintf(retMessage, "怪物掉落物品信息查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_MONSTER_DROP_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;

			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);


			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "怪物掉落物品信息查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;

					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0)
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量

						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft < packetSize + 4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 交易时间
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
					
					// 物品类型ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemTypeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 物品名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// 物品唯一ID
					_ui64toa(vect_LogInfo[i].item_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemUniqueId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 数量
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "怪物掉落物品信息查询");
	}
	
	return DBTFLV;	
}

//从NPC购买的道具
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
		REQ_LOG_BUY_ITEM_FROM_NPC req_Log_info;				//日志信息查询请求
		REP_LOG_BUY_ITEM_FROM_NPC res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "从NPC购买的道具信息查询");
		}
		else
		{
			sprintf(retMessage, "从NPC购买的道具信息查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_BUY_ITEM_FROM_NPC);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "从NPC购买的道具信息查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 买进时间
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
					
					// 物品类型ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemTypeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 物品名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// 物品唯一ID
					_ui64toa(vect_LogInfo[i].item_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemUniqueId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 数量
					_itoa((int)vect_LogInfo[i].item_count, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemCount;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 消耗游戏币数量
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "从NPC购买的道具信息查询");
	}
	
	return DBTFLV;	
}

//物品:卖给NPC
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
		REQ_LOG_SELL_ITEM_TO_NPC req_Log_info;				//日志信息查询请求
		REP_LOG_SELL_ITEM_TO_NPC res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "卖给NPC物品信息查询");
		}
		else
		{
			sprintf(retMessage, "卖给NPC物品信息查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_SELL_ITEM_TO_NPC);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);


			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "卖给NPC物品信息查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 卖出时间
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
					
					// 物品类型ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemTypeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 物品名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// 物品唯一ID
					_ui64toa(vect_LogInfo[i].item_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemUniqueId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 数量
					_itoa((int)vect_LogInfo[i].item_count, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemCount;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 消耗游戏币数量
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "卖给NPC物品信息查询");
	}
	
	return DBTFLV;
}

//玩家自行删除道具
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
		REQ_LOG_DROP_ITEM req_Log_info;				//日志信息查询请求
		REP_LOG_DROP_ITEM res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "玩家自行删除道具查询");
		}
		else
		{
			sprintf(retMessage, "玩家自行删除道具查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_DROP_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "玩家自行删除道具查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 删除时间
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
					
					// 物品类型ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemTypeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 物品名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// 物品唯一ID
					_ui64toa(vect_LogInfo[i].item_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemUniqueId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 数量
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "玩家自行删除道具查询");
	}
	
	return DBTFLV;
}

// 物品:交易获得
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
		REQ_LOG_GET_ITEM_TRADE req_Log_info;				//日志信息查询请求
		REP_LOG_GET_ITEM_TRADE res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "交易获得物品查询");
		}
		else
		{
			sprintf(retMessage, "交易获得物品查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_GET_ITEM_TRADE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "交易获得物品查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;

					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 来源角色ID
					_itoa((int)vect_LogInfo[i].src_char_id.charIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcCharId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 来源角色名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcCharName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].src_char_name.charName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].src_char_name.charName);
					DBTFLV.push_back(m_tflv);

					// 交易时间
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

					// 物品类型ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemTypeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 物品名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);

					// 物品唯一ID
					_ui64toa(vect_LogInfo[i].item_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemUniqueId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 数量
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "交易获得物品查询");
	}
	
	return DBTFLV;	
}

// 物品:交易失去
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
		REQ_LOG_LOSE_ITEM_TRADE req_Log_info;				//日志信息查询请求
		REP_LOG_LOSE_ITEM_TRADE res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "交易失去物品查询");
		}
		else
		{
			sprintf(retMessage, "交易失去物品查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);	
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_LOSE_ITEM_TRADE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "交易失去物品查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 物品目标角色ID
					_itoa((int)vect_LogInfo[i].dst_char_id.charIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_DstCharId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 物品目标角色名
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_DstCharName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].dst_char_name.charName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].dst_char_name.charName);
					DBTFLV.push_back(m_tflv);
					
					// 交易时间
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
					
					// 物品类型ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemTypeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 物品名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// 物品唯一ID
					_ui64toa(vect_LogInfo[i].item_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemUniqueId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 数量
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "交易失去物品查询");
	}
	
	return DBTFLV;	
}

// 物品:装备绑定记录
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
		REQ_LOG_BIND_ITEM req_Log_info;				//日志信息查询请求
		REP_LOG_BIND_ITEM res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "装备绑定记录查询");
		}
		else
		{
			sprintf(retMessage, "装备绑定记录查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_BIND_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "装备绑定记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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

					// 绑定时间
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

					// 物品类型ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemTypeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 物品名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// 物品唯一ID
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "装备绑定记录查询");
	}
	
	return DBTFLV;	
}

// 物品:装备解绑定记录
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
		REQ_LOG_UNBIND_ITEM req_Log_info;				//日志信息查询请求
		REP_LOG_UNBIND_ITEM res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "装备解绑定记录查询");
		}
		else
		{
			sprintf(retMessage, "装备解绑定记录查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_UNBIND_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "装备解绑定记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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
				//处理结果集
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
					// 解绑定时间
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
					
					// 物品类型ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ItemID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 物品名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// 物品唯一ID
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

					// 解绑消耗物品ID
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "装备解绑定记录查询");
	}
	
	return DBTFLV;	
}

// 物品:角色“摆摊”日志查询
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
		REQ_LOG_BOOTH_ITEM req_Log_info;				//日志信息查询请求
		REP_LOG_BOOTH_ITEM res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "角色摆摊日志查询");
		}
		else
		{
			sprintf(retMessage, "角色摆摊日志查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_BOOTH_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "角色摆摊日志查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 操作类型
					m_tflv.nIndex=DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::WA_BoothType;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
					switch (vect_LogInfo[i].booth_type)
					{
					case REP_LOG_BOOTH_ITEM_ELE::BOOTH_OPEN:
					{
						m_tflv.m_tvlength = strlen("开摆")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"开摆");
						DBTFLV.push_back(m_tflv);

						// 物品玩家
						m_tflv.nIndex = DBTFLV.size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_DstCharName;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen("");
						sprintf((char *)&m_tflv.lpdata, "%s", "");
						DBTFLV.push_back(m_tflv);
						// 物品道具
						m_tflv.nIndex = DBTFLV.size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen("");
						sprintf((char *)&m_tflv.lpdata, "%s", "");
						DBTFLV.push_back(m_tflv);
						// 物品道具价格
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
						m_tflv.m_tvlength = strlen("摊主卖出")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"摊主卖出");	
						DBTFLV.push_back(m_tflv);

						// 物品玩家
						m_tflv.nIndex = DBTFLV.size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_DstCharName;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(vect_LogInfo[i].booth_sell.dst_char_name.charName);
						sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].booth_sell.dst_char_name.charName);
						DBTFLV.push_back(m_tflv);
						// 物品道具
						m_tflv.nIndex = DBTFLV.size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen(vect_LogInfo[i].booth_sell.item_name);
						sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].booth_sell.item_name);
						DBTFLV.push_back(m_tflv);
						// 物品道具价格
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
							m_tflv.m_tvlength = strlen("摊主买进")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"摊主买进");
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
						m_tflv.m_tvlength = strlen("收摊")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"收摊");
						DBTFLV.push_back(m_tflv);

						// 物品玩家
						m_tflv.nIndex = DBTFLV.size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_DstCharName;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen("");
						sprintf((char *)&m_tflv.lpdata, "%s", "");
						DBTFLV.push_back(m_tflv);
						// 物品道具
						m_tflv.nIndex = DBTFLV.size()+1;
						m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
						m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
						m_tflv.m_tvlength = strlen("");
						sprintf((char *)&m_tflv.lpdata, "%s", "");
						DBTFLV.push_back(m_tflv);
						// 物品道具价格
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
					// 操作时间
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "角色摆摊日志查询");
	}
	
	return DBTFLV;	
}

// 物品:道具删除记录(指使用时间到了)
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
		REQ_LOG_DELETE_ITEM req_Log_info;				//日志信息查询请求
		REP_LOG_DELETE_ITEM res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "道具删除记录查询");
		}
		else
		{
			sprintf(retMessage, "道具删除记录查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_DELETE_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "道具删除记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 删除时间
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
					
					// 删除物品类型ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ItemID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 物品名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// 物品唯一ID
					_ui64toa(vect_LogInfo[i].item_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemUniqueId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 删除物品类型ID
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "道具删除记录查询");
	}
	
	return DBTFLV;	
}

// 物品:道具使用记录
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
		REQ_LOG_USE_ITEM req_Log_info;				//日志信息查询请求
		REP_LOG_USE_ITEM res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "道具使用记录查询");
		}
		else
		{
			sprintf(retMessage, "道具使用记录查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_USE_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "道具使用记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 使用时间
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
					
					// 使用物品类型ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ItemID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 使用物品名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// 使用物品唯一ID
					_ui64toa(vect_LogInfo[i].item_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemUniqueId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 物品的使用数量
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "道具使用记录查询");
	}
	
	return DBTFLV;	
}

// 物品:装备制作记录
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
		REQ_LOG_CRAFT_ITEM req_Log_info;				//日志信息查询请求
		REP_LOG_CRAFT_ITEM res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "装备制作记录查询");
		}
		else
		{
			sprintf(retMessage, "装备制作记录查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);	
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_CRAFT_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "装备制作记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 制作时间
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
					
					// 装备制作类型
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_CraftType;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch (vect_LogInfo[i].craft_type)
					{
					case REP_LOG_CRAFT_ITEM_ELE::CRAFT_BLACKSMITH:
						m_tflv.m_tvlength = strlen("打造")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"打造");						
						break;
					case REP_LOG_CRAFT_ITEM_ELE::CRAFT_SEWING:
						m_tflv.m_tvlength = strlen("裁缝")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"裁缝");
						break;
					case REP_LOG_CRAFT_ITEM_ELE::CRAFT_HANDCRAFT:
						m_tflv.m_tvlength = strlen("手工")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"手工");
						break;
					}
					DBTFLV.push_back(m_tflv);
					
					// 图纸ID
					_itoa((int)vect_LogInfo[i].recipe_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_RecipeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 图纸名字
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

								// 材料名字
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

								// 材料名字
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

								// 材料名字
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

								// 材料名字
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

								// 材料名字
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

								// 材料名字
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

					//产品ID
					_itoa((int)vect_LogInfo[i].product_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ProductId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					//产品名字
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "装备制作记录查询");
	}
	
	return DBTFLV;	
}

// 物品:装备打孔
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
		REQ_LOG_PUNCH_ITEM req_Log_info;				//日志信息查询请求
		REP_LOG_PUNCH_ITEM res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "装备打孔查询");
		}
		else
		{
			sprintf(retMessage, "装备打孔查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_PUNCH_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "装备打孔查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 打孔时间
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
					
					// 消耗铁锤
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeHammer;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch (vect_LogInfo[i].consume_hammer)
					{
					case REP_LOG_PUNCH_ITEM_ELE::LOW_LEVEL_HAMMER:
						m_tflv.m_tvlength = strlen("初级铁锤")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"初级铁锤");						
						break;
					case REP_LOG_PUNCH_ITEM_ELE::MIDDLE_LEVEL_HAMMER:
						m_tflv.m_tvlength = strlen("中级铁锤")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"中级铁锤");
						break;
					case REP_LOG_PUNCH_ITEM_ELE::HIGH_LEVEL_HAMMER:
						m_tflv.m_tvlength = strlen("高级铁锤")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"高级铁锤");
						break;
					}
					DBTFLV.push_back(m_tflv);

					// 消耗铁砧
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeStithy;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch (vect_LogInfo[i].consume_stithy)
					{
					case REP_LOG_PUNCH_ITEM_ELE::LOW_LEVEL_STITHY:
						m_tflv.m_tvlength = strlen("初级铁砧")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"初级铁砧");						
						break;
					case REP_LOG_PUNCH_ITEM_ELE::MIDDLE_LEVEL_STITHY:
						m_tflv.m_tvlength = strlen("中级铁砧")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"中级铁砧");
						break;
					case REP_LOG_PUNCH_ITEM_ELE::HIGH_LEVEL_STITHY:
						m_tflv.m_tvlength = strlen("高级铁砧")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"高级铁砧");
						break;
					}
					DBTFLV.push_back(m_tflv);

					// 消耗金钱
					_itoa((int)vect_LogInfo[i].consume_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 物品类型ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ItemID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 物品名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// 物品唯一ID
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "装备打孔查询");
	}
	
	return DBTFLV;
}

// 物品:装备改孔
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
		REQ_LOG_CHANGE_SLOT_ITEM req_Log_info;				//日志信息查询请求
		REP_LOG_CHANGE_SLOT_ITEM res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "装备改孔查询");
		}
		else
		{
			sprintf(retMessage, "装备改孔查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_CHANGE_SLOT_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "装备改孔查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 10240);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 改孔时间
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
					
					// 改孔前颜色
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_OldColor;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch (vect_LogInfo[i].old_color)
					{
					case ADMIN::GEM_COLOR_RED:
						m_tflv.m_tvlength = strlen("红")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"红");						
						break;
					case ADMIN::GEM_COLOR_YELLOW:
						m_tflv.m_tvlength = strlen("黄")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"黄");
						break;
					case ADMIN::GEM_COLOR_BLUE:
						m_tflv.m_tvlength = strlen("蓝")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"蓝");
						break;
					case ADMIN::GEM_COLOR_GREEN:
						m_tflv.m_tvlength = strlen("绿")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"绿");
						break;
					case ADMIN::GEM_COLOR_PURPLE:
						m_tflv.m_tvlength = strlen("紫")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"紫");
						break;
					case ADMIN::GEM_COLOR_WHITE:
						m_tflv.m_tvlength = strlen("白")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"白");
						break;
					}
					DBTFLV.push_back(m_tflv);

					// 改孔前颜色
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NewColor;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch (vect_LogInfo[i].new_color)
					{
					case ADMIN::GEM_COLOR_RED:
						m_tflv.m_tvlength = strlen("红")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"红");						
						break;
					case ADMIN::GEM_COLOR_YELLOW:
						m_tflv.m_tvlength = strlen("黄")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"黄");
						break;
					case ADMIN::GEM_COLOR_BLUE:
						m_tflv.m_tvlength = strlen("蓝")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"蓝");
						break;
					case ADMIN::GEM_COLOR_GREEN:
						m_tflv.m_tvlength = strlen("绿")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"绿");
						break;
					case ADMIN::GEM_COLOR_PURPLE:
						m_tflv.m_tvlength = strlen("紫")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"紫");
						break;
					case ADMIN::GEM_COLOR_WHITE:
						m_tflv.m_tvlength = strlen("白")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"白");
						break;
					}
					DBTFLV.push_back(m_tflv);
					
					// 消耗金钱
					_itoa((int)vect_LogInfo[i].consume_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 物品类型ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ItemID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 物品名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// 物品唯一ID
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "装备改孔查询");
	}
	
	return DBTFLV;
}

// 物品:装备升星
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
		REQ_LOG_UPGRADE_STAR req_Log_info;				//日志信息查询请求
		REP_LOG_UPGRADE_STAR res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "装备升星查询");
		}
		else
		{
			sprintf(retMessage, "装备升星查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_UPGRADE_STAR);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "装备升星查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 10240);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 改孔时间
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

					// 升星前星数
					_itoa((int)vect_LogInfo[i].old_star_count, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_OldStarCount;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 升星后星数
					_itoa((int)vect_LogInfo[i].new_star_count, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NewStarCount;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 消耗金钱
					_itoa((int)vect_LogInfo[i].consume_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 物品类型ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ItemID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 物品名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// 物品唯一ID
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "装备升星查询");
	}
	
	return DBTFLV;
}

// 物品:宝石
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
		REQ_LOG_GEM req_Log_info;				//日志信息查询请求
		REP_LOG_GEM res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "宝石信息查询");
		}
		else
		{
			sprintf(retMessage, "宝石信息查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_GEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "宝石信息查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 10240);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 操作
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GemOperation;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch (vect_LogInfo[i].gem_operation)
					{
					case REP_LOG_GEM_ELE::GEM_ADD:
						m_tflv.m_tvlength = strlen("镶嵌")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"镶嵌");						
						break;
					case REP_LOG_GEM_ELE::GEM_REMOVE:
						m_tflv.m_tvlength = strlen("去除")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"去除");
						break;
					}
					DBTFLV.push_back(m_tflv);

					// 改孔时间
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

					// 宝石类型
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GemType;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch (vect_LogInfo[i].gem_type)
					{
					case ADMIN::GEM_COLOR_RED:
						m_tflv.m_tvlength = strlen("红")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"红");						
						break;
					case ADMIN::GEM_COLOR_YELLOW:
						m_tflv.m_tvlength = strlen("黄")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"黄");
						break;
					case ADMIN::GEM_COLOR_BLUE:
						m_tflv.m_tvlength = strlen("蓝")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"蓝");
						break;
					case ADMIN::GEM_COLOR_GREEN:
						m_tflv.m_tvlength = strlen("绿")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"绿");
						break;
					case ADMIN::GEM_COLOR_PURPLE:
						m_tflv.m_tvlength = strlen("紫")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"紫");
						break;
					case ADMIN::GEM_COLOR_WHITE:
						m_tflv.m_tvlength = strlen("白")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"白");
						break;
					}
					DBTFLV.push_back(m_tflv);
					
					// 宝石等级
					_itoa((int)vect_LogInfo[i].gem_level, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GemLevel;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 消耗金钱
					_itoa((int)vect_LogInfo[i].consume_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 物品类型ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ItemID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 物品名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					
					// 物品唯一ID
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "宝石信息查询");
	}
	
	return DBTFLV;
}

// 物品:制药记录
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
		REQ_LOG_PHARMA req_Log_info;				//日志信息查询请求
		REP_LOG_PHARMA res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "制药记录查询");
		}
		else
		{
			sprintf(retMessage, "制药记录查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);	
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_PHARMA);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "制药记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 10240);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 制作时间
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
					
					// 图纸ID
					_itoa((int)vect_LogInfo[i].recipe_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_RecipeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 图纸名字
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

								// 材料名字
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

								// 材料名字
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

								// 材料名字
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

								// 材料名字
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

								// 材料名字
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

								// 材料名字
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

					//产品ID
					_itoa((int)vect_LogInfo[i].product_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ProductId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					//产品名字
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "制药记录查询");
	}
	
	return DBTFLV;
}

// 物品:宝石合成
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
		REQ_LOG_CRAFT_GEM req_Log_info;				//日志信息查询请求
		REP_LOG_CRAFT_GEM res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "宝石合成查询");
		}
		else
		{
			sprintf(retMessage, "宝石合成查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);	
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_CRAFT_GEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "宝石合成查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 10240);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 合成时间
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

					// 消耗金钱
					_itoa((int)vect_LogInfo[i].consume_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 成功率道具
					_itoa((int)vect_LogInfo[i].prop_item_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_PropItemId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 原材料ID
					_itoa((int)vect_LogInfo[i].material_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 原材料名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].material_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].material_name);
					DBTFLV.push_back(m_tflv);

					//产品ID
					_itoa((int)vect_LogInfo[i].product_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ProductId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					//产品名字
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "宝石合成查询");
	}
	
	return DBTFLV;
}

// 物品:材料合成
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
		REQ_LOG_CRAFT_MATIRAIL req_Log_info;				//日志信息查询请求
		REP_LOG_CRAFT_MATIRAIL res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "材料合成查询");
		}
		else
		{
			sprintf(retMessage, "材料合成查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);	
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_CRAFT_MATIRAIL);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "材料合成查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 10240);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 合成时间
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
					
					// 消耗金钱
					_itoa((int)vect_LogInfo[i].consume_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 成功率道具
					_itoa((int)vect_LogInfo[i].prop_item_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_PropItemId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 原材料ID
					_itoa((int)vect_LogInfo[i].material_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 原材料名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].material_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].material_name);
					DBTFLV.push_back(m_tflv);
					
					//产品ID
					_itoa((int)vect_LogInfo[i].product_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ProductId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					//产品名字
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "材料合成查询");
	}
	
	return DBTFLV;
}

// 物品:图纸合成
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
		REQ_LOG_CRAFT_RECIPE req_Log_info;				//日志信息查询请求
		REP_LOG_CRAFT_RECIPE res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "图纸合成查询");
		}
		else
		{
			sprintf(retMessage, "图纸合成查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_CRAFT_RECIPE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "图纸合成查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 10240);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 合成时间
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
					
					// 消耗金钱
					_itoa((int)vect_LogInfo[i].consume_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 成功率道具
					_itoa((int)vect_LogInfo[i].prop_item_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_PropItemId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 原材料ID
					_itoa((int)vect_LogInfo[i].material_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 原材料名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MaterialName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].material_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].material_name);
					DBTFLV.push_back(m_tflv);
					
					//产品ID
					_itoa((int)vect_LogInfo[i].product_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ProductId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					//产品名字
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "图纸合成查询");
	}
	
	return DBTFLV;
}

// 迷宫积分兑换奖励
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
		REQ_LOG_MAZE_INTEGRAL req_Log_info;				//日志信息查询请求
		REP_LOG_MAZE_INTEGRAL res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "迷宫积分兑换奖励查询");
		}
		else
		{
			sprintf(retMessage, "迷宫积分兑换奖励查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_MAZE_INTEGRAL);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "迷宫积分兑换奖励查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 10240);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 兑换时间
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

					// 兑换类型
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ChangeType;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch (vect_LogInfo[i].change_type)
					{
					case REP_LOG_MAZE_INTEGERAL_ELE::CHANGE_EXP:
						m_tflv.m_tvlength = strlen("兑换经验")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"兑换经验");						
						break;
					case REP_LOG_MAZE_INTEGERAL_ELE::CHANGE_MONEY:
						m_tflv.m_tvlength = strlen("兑换金钱")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"兑换金钱");
						break;
					}
					DBTFLV.push_back(m_tflv);
					
					// 消耗积分
					_itoa((int)vect_LogInfo[i].consume_integral, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeIntegral;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 得到的经验或是钱
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "迷宫积分兑换奖励查询");
	}
	
	return DBTFLV;
}

// 游戏币:怪物掉落
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
		REQ_LOG_MONSTER_DROP_MONEY req_Log_info;				//日志信息查询请求
		REP_LOG_MONSTER_DROP_MONEY res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "怪物掉落游戏币查询");
		}
		else
		{
			sprintf(retMessage, "怪物掉落游戏币查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);	
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_MONSTER_DROP_MONEY);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);
			
			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "怪物掉落游戏币查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
//					memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					//Sleep(1000);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 获得时间
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
					
					// 获得金钱数量
					_itoa((int)vect_LogInfo[i].get_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 来源ID
					_itoa((int)vect_LogInfo[i].src_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 来源名
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "怪物掉落游戏币查询");
	}
	
	return DBTFLV;
}

// 游戏币:任务获得
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
		REQ_LOG_GET_MONEY_QUEST req_Log_info;				//日志信息查询请求
		REP_LOG_GET_MONEY_QUEST res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "任务获得游戏币查询");
		}
		else
		{
			sprintf(retMessage, "任务获得游戏币查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);	
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_GET_MONEY_QUEST);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "任务获得游戏币查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
//					memset(recvbuf, 0, 20480);
					*recvbuf = NULL;

					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 获得时间
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
					
					// 获得金钱数量
					_itoa((int)vect_LogInfo[i].get_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 来源ID
					_itoa((int)vect_LogInfo[i].src_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 来源名
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "任务获得游戏币查询");
	}
	
	return DBTFLV;
}

// 游戏币:修装
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
		REQ_LOG_LOSE_MONEY_REPAIR req_Log_info;				//日志信息查询请求
		REP_LOG_LOSE_MONEY_REPAIR res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "修装信息查询");
		}
		else
		{
			sprintf(retMessage, "修装信息查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);	
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_LOSE_MONEY_REPAIR);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "修装信息查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 时间
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
					
					// 消耗金钱
					_itoa((int)vect_LogInfo[i].consume_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 物品类型
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ItemID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 物品名
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "修装信息查询");
	}
	
	return DBTFLV;
}

// 游戏币:交易获得
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
		REQ_LOG_GET_MONEY_TRADE req_Log_info;				//日志信息查询请求
		REP_LOG_GET_MONEY_TRADE res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "交易获得游戏币查询");
		}
		else
		{
			sprintf(retMessage, "交易获得游戏币查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);	
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_GET_MONEY_TRADE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "交易获得游戏币查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 获得时间
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
					
					// 获得金钱数量
					_itoa((int)vect_LogInfo[i].get_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GetMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 来源ID
					_itoa((int)vect_LogInfo[i].src_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 来源名
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "交易获得游戏币查询");
	}
	
	return DBTFLV;
}

// 游戏币:交易失去
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
		REQ_LOG_LOSE_MONEY_TRADE req_Log_info;				//日志信息查询请求
		REP_LOG_LOSE_MONEY_TRADE res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "交易失去游戏币查询");
		}
		else
		{
			sprintf(retMessage, "交易失去游戏币查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_LOSE_MONEY_TRADE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "交易失去游戏币查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 获得时间
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
					
					// 失去金钱数量
					_itoa((int)vect_LogInfo[i].lose_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_LoseMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 来源ID
					_itoa((int)vect_LogInfo[i].dst_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_DstId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 来源名
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "交易失去游戏币查询");
	}
	
	return DBTFLV;
}

// 任务:完成获得记录
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
		REQ_LOG_QUEST_REWARD req_Log_info;				//日志信息查询请求
		REP_LOG_QUEST_REWARD res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "完成获得记录查询");
		}
		else
		{
			sprintf(retMessage, "完成获得记录查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_QUEST_REWARD);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "完成获得记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 时间
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
					
					// 任务ID
					_itoa((int)vect_LogInfo[i].quest_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_QuestId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 奖励的金钱
					_itoa((int)vect_LogInfo[i].reward_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_RewardMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 任务名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_QuestName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].quest_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].quest_name);
					DBTFLV.push_back(m_tflv);

					// 奖励的物品
					_itoa((int)vect_LogInfo[i].reward_item_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_RewardItemId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 奖励的物品名字
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "完成获得记录查询");
	}
	
	return DBTFLV;
}

// 任务:任务物品删除记录
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
		REQ_LOG_QUEST_GIVE req_Log_info;				//日志信息查询请求
		REP_LOG_QUEST_GIVE res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "任务物品删除记录查询");
		}
		else
		{
			sprintf(retMessage, "任务物品删除记录查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_QUEST_GIVE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "任务物品删除记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 时间
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
					
					// 任务ID
					_itoa((int)vect_LogInfo[i].quest_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_QuestId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 任务名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_QuestName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].quest_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].quest_name);
					DBTFLV.push_back(m_tflv);
					
					// 失去的物品ID
					_itoa((int)vect_LogInfo[i].give_item_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GiveItemId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 失去的物品名字
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "任务物品删除记录查询");
	}
	
	return DBTFLV;
}

// 邮件:发送记录
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
		REQ_LOG_MAIL_SEND req_Log_info;				//日志信息查询请求
		REP_LOG_MAIL_SEND res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "邮件发送记录查询");
		}
		else
		{
			sprintf(retMessage, "邮件发送记录查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);	
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_MAIL_SEND);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "邮件发送记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 时间
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

					// 唯一ID
					_ui64toa(vect_LogInfo[i].mail_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MailId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 金钱数量
					_itoa((int)vect_LogInfo[i].mail_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MailMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 收件人ID
					_itoa((int)vect_LogInfo[i].dst_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_DstId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 收件人名字
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

					// 邮件标题
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "邮件发送记录查询");
	}
	
	return DBTFLV;
}

// 邮件:接收记录
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
		REQ_LOG_MAIL_RECV req_Log_info;				//日志信息查询请求
		REP_LOG_MAIL_RECV res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "邮件接收记录查询");
		}
		else
		{
			sprintf(retMessage, "邮件接收记录查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);	
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_MAIL_RECV);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "邮件接收记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 时间
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
					
					// 唯一ID
					_ui64toa(vect_LogInfo[i].mail_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MailId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 金钱数量
					_itoa((int)vect_LogInfo[i].mail_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MailMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 发件人ID
					_itoa((int)vect_LogInfo[i].src_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 发件人名字
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
									
									// 收件人ID
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
					
					// 邮件标题
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "邮件接收记录查询");
	}
	
	return DBTFLV;
}
// 邮件:删除记录
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
		REQ_LOG_MAIL_DELETE req_Log_info;				//日志信息查询请求
		REP_LOG_MAIL_DELETE res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "邮件删除记录查询");
		}
		else
		{
			sprintf(retMessage, "邮件删除记录查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_MAIL_DELETE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "邮件删除记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 时间
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
					
					// 唯一ID
					_ui64toa(vect_LogInfo[i].mail_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MailId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 发件人ID
					_itoa((int)vect_LogInfo[i].src_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 发件人名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].src_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].src_name);
					DBTFLV.push_back(m_tflv);
					
					// 邮件标题
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "邮件删除记录查询");
	}
	
	return DBTFLV;
}

// 商城:购买记录
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
		REQ_LOG_ITEM_SHOP req_Log_info;				//日志信息查询请求
		REP_LOG_ITEM_SHOP res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "用户的副本日志查询");
		}
		else
		{
			sprintf(retMessage, "用户的商城购买记录查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_ITEM_SHOP);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "用户的商城购买记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 时间
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

					// 类型
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_TYPE;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					int iNum = vect_LogInfo[i].item_type_id;
					switch (ADMIN::GetStructType(vect_LogInfo[i].item_type_id))
					{
						case 1:
							m_tflv.m_tvlength = strlen("武器装备")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"武器装备");						
							break;
						case 2:
							m_tflv.m_tvlength = strlen("其他装备")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"其他装备");
							break;
						case 3:
							m_tflv.m_tvlength = strlen("简单物品/货物")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"简单物品/货物");
							break;
						case 4:
							m_tflv.m_tvlength = strlen("消耗品")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"消耗品");
							break;
						case 5:
							m_tflv.m_tvlength = strlen("背包")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"背包");
							break;
						case 6:
							m_tflv.m_tvlength = strlen("配方")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"配方");
							break;
						case 7:
							m_tflv.m_tvlength = strlen("宝石类物品")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"宝石类物品");
							break;
						default:
							m_tflv.m_tvlength = strlen("空类型")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"空类型");
							break;	
					}

// 					switch (vect_LogInfo[i].item_type_id)
// 					{
// 						case 1:
// 							m_tflv.m_tvlength = strlen("武器装备")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"武器装备");						
// 							break;
// 						case 2:
// 							m_tflv.m_tvlength = strlen("任务物品")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"任务物品");
// 							break;
// 						case 3:
// 							m_tflv.m_tvlength = strlen("药品类")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"药品类");
// 							break;
// 						case 4:
// 							m_tflv.m_tvlength = strlen("宠物物品")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"宠物物品");
// 							break;
// 						case 5:
// 							m_tflv.m_tvlength = strlen("不可使用任务物品")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"不可使用任务物品");
// 							break;
// 						case 6:
// 							m_tflv.m_tvlength = strlen("材料")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"材料");
// 							break;
// 						case 7:
// 							m_tflv.m_tvlength = strlen("迷宫物品")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"迷宫物品");
// 							break;
// 						case 8:
// 							m_tflv.m_tvlength = strlen("商城物品")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"商城物品");
// 							break;
// 						case 9:
// 							m_tflv.m_tvlength = strlen("背包")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"背包");
// 							break;
// 						case 10:
// 							m_tflv.m_tvlength = strlen("打造白色图纸")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"打造白色图纸");
// 							break;
// 						case 11:
// 							m_tflv.m_tvlength = strlen("药水图纸")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"药水图纸");
// 							break;
// 						case 12:
// 							m_tflv.m_tvlength = strlen("纯宝石")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"纯宝石");
// 							break;	
// 						default:
// 							m_tflv.m_tvlength = strlen("空类型")+1;
// 							sprintf((LPSTR)&m_tflv.lpdata,"空类型");
// 							break;	
// 					}
					DBTFLV.push_back(m_tflv);
					

					// 道具ID
					_itoa((int)vect_LogInfo[i].item_unique_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ItemID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 道具名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);
					//道具个数
					_itoa((int)vect_LogInfo[i].item_count, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::WA_itemCount;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					//消耗M币
					_itoa((int)vect_LogInfo[i].comsume_m_money, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName=CEnumCore::TagName::WA_ConsumeMoney;
					m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					// 剩余M币
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "用户的副本日志查询");
	}
	
	return DBTFLV;
	return DBTFLV;
}

// 查询用户的副本日志
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
		REQ_LOG_INSTANCE req_Log_info;				//日志信息查询请求
		REP_LOG_INSTANCE res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "用户的副本日志查询");
		}
		else
		{
			sprintf(retMessage, "用户的副本日志查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_INSTANCE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "用户的副本日志查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 时间
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

					// 类型
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_OpType;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch (vect_LogInfo[i].op_type)
					{
						case REP_LOG_INSTANCE_ELE::INSTANCE_ENTER:
							m_tflv.m_tvlength = strlen("进入副本")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"进入副本");						
							break;
						case REP_LOG_INSTANCE_ELE::INSTANCE_LEAVE:
							m_tflv.m_tvlength = strlen("离开副本")+1;
							sprintf((LPSTR)&m_tflv.lpdata,"离开副本");
							break;
					}
					DBTFLV.push_back(m_tflv);
					

					// 副本ID
					_itoa((int)vect_LogInfo[i].instance_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_InstanceId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 副本名字
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "用户的副本日志查询");
	}
	
	return DBTFLV;
}

// 玩家登陆/登出日志
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
		REQ_LOG_LOGIN_LOGOUT req_Log_info;				//日志信息查询请求
		REP_LOG_LOGIN_LOGOUT res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "登陆登出日志");
		}
		else
		{
			sprintf(retMessage, "登陆登出日志: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_LOGIN_LOGOUT);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_Log_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_Log_info.endTime);

			req_Log_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_Log_info.srv_realm_id=srv_Realmid;
			req_Log_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "登陆登出日志");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 时入时间
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
					
					// 类型
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_OpType;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch (vect_LogInfo[i].op_type)
					{
					case REP_LOG_LOGIN_LOGOUT_ELE::LOGIN_CHAR:
						m_tflv.m_tvlength = strlen("角色登录")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"角色登录");						
						break;
					case REP_LOG_LOGIN_LOGOUT_ELE::LOGOUT_CHAR:
						m_tflv.m_tvlength = strlen("角色登出")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"角色登出");
						break;
					}
					DBTFLV.push_back(m_tflv);
					
					// 角色ID
					_itoa((int)vect_LogInfo[i].char_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 角色名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].char_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].char_name);
					DBTFLV.push_back(m_tflv);

					// 登陆IP
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "登陆登出日志");
	}
	
	return DBTFLV;
}

// 玩家登陆/登出日志(按IP方式查询)
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
		REQ_LOG_LOGIN_LOGOUT_BY_IP req_Log_info;				//日志信息查询请求
		REP_LOG_LOGIN_LOGOUT_BY_IP res_Log_info;				//日志信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		char *strTemp = new char[strlen(LoginIP)+1];
		ZeroMemory(strTemp, strlen(LoginIP)+1);
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "登陆登出日志(IP方式)");
		}
		else
		{
			sprintf(retMessage, "登陆登出日志(IP方式): <大区>%s, <开始时间>%s, <结束时间>%s, <登陆IP>%d", ServerIP, Begintime, Endtime, LoginIP);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_LOGIN_LOGOUT_BY_IP);
			
			strcpy(strTemp, LoginIP);

			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
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
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "登陆登出日志(IP方式)");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&res_Log_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 时入时间
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
					
					// 类型
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_OpType;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch (vect_LogInfo[i].op_type)
					{
					case REP_LOG_LOGIN_LOGOUT_ELE::LOGIN_CHAR:
						m_tflv.m_tvlength = strlen("角色登录")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"角色登录");						
						break;
					case REP_LOG_LOGIN_LOGOUT_ELE::LOGOUT_CHAR:
						m_tflv.m_tvlength = strlen("角色登出")+1;
						sprintf((LPSTR)&m_tflv.lpdata,"角色登出");
						break;
					}
					DBTFLV.push_back(m_tflv);
					
					// 角色ID
					_itoa((int)vect_LogInfo[i].char_id.charIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 角色名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].char_name.charName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].char_name.charName);
					DBTFLV.push_back(m_tflv);

					// 登录登出IP
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "登陆登出日志(IP方式)");
	}
	
 	return DBTFLV;
}

//Added by tengjie 2010-05-13:本地记录封停原因,不再显示给用户
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
		if(!strcmp("Description", m_DataRecord.recordfieldname))//获取相应的ServerIP
		{
			if(iCount == datarecord.size() - 1)
				szData.Format("%s", m_DataRecord.recorddata);//保存查询得到的最后一条记录
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
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录服务器失败", "单个封停查询");
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
//角色日志:角色升级记录
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
		REQ_LOG_CHAR_LEVELUP	req_PartLog_info;				//角色升级记录查询请求
		REP_LOG_CHAR_LEVELUP	rep_PartLog_info;				//角色升级记录查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "角色升级记录");
		}
		else
		{
			sprintf(retMessage, "角色升级记录: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_CHAR_LEVELUP);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "角色升级记录");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 角色ID
					_itoa((int)vect_PartID[i], strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 角色名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = vect_PartName[i].length();
					sprintf((char *)&m_tflv.lpdata, "%s", vect_PartName[i].c_str());
					DBTFLV.push_back(m_tflv);
					
					// 升级时间
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
					
					// 等级
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "角色升级记录");
	}

	delete []retMessage;

	return DBTFLV;
}

//角色日志:角色创建记录查询
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
		REQ_LOG_CHAR_CREATE		req_PartLog_info;				//角色创建记录查询请求
		REP_LOG_CHAR_CREATE		rep_PartLog_info;				//角色创建记录查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "角色创建记录查询");
		}
		else
		{
			sprintf(retMessage, "角色创建记录: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_CHAR_CREATE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "角色创建记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 角色ID
					_itoa((int)vect_PartID[i], strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 角色名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = vect_PartName[i].length();
					sprintf((char *)&m_tflv.lpdata, "%s", vect_PartName[i].c_str());
					DBTFLV.push_back(m_tflv);
					
					// 角色创建时间
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "角色创建记录查询");
	}

	delete []retMessage;

	return DBTFLV;
}

//角色日志:角色删除记录查询
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
		REQ_LOG_CHAR_DELETE		req_PartLog_info;				//角色删除记录查询请求
		REP_LOG_CHAR_DELETE		rep_PartLog_info;				//角色删除记录查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "角色删除记录查询");
		}
		else
		{
			sprintf(retMessage, "角色删除记录: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_CHAR_DELETE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "角色删除记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 角色ID
					_itoa((int)vect_PartID[i], strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 角色名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = vect_PartName[i].length();
					sprintf((char *)&m_tflv.lpdata, "%s", vect_PartName[i].c_str());
					DBTFLV.push_back(m_tflv);
					
					// 删除时间
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
					
					// 删除时的角色等级
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "角色删除记录查询");
	}

	delete []retMessage;

	return DBTFLV;
}

//角色日志:加入帮会
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
		REQ_LOG_CHAR_JOIN_GUILD		req_PartLog_info;				//加入帮会记录查询请求
		REP_LOG_CHAR_JOIN_GUILD		rep_PartLog_info;				//加入帮会记录查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "加入帮会记录查询");
		}
		else
		{
			sprintf(retMessage, "加入帮会: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_CHAR_JOIN_GUILD);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "加入帮会记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 角色ID
					_itoa((int)vect_PartID[i], strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 角色名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = vect_PartName[i].length() ;
					sprintf((char *)&m_tflv.lpdata, "%s", vect_PartName[i].c_str());
					DBTFLV.push_back(m_tflv);
					
					// 加入时间
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
					
					// 公会名字
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "加入帮会记录查询");
	}

	delete []retMessage;

	return DBTFLV;
}

//角色日志:离开帮会
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
		REQ_LOG_CHAR_LEAVE_GUILD		req_PartLog_info;				//离开帮会记录查询请求
		REP_LOG_CHAR_LEAVE_GUILD		rep_PartLog_info;				//离开帮会记录查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "离开帮会记录查询");
		}
		else
		{
			sprintf(retMessage, "离开帮会: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_CHAR_LEAVE_GUILD);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "离开帮会记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 角色ID
					_itoa((int)vect_PartID[i], strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 角色名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = vect_PartName[i].length();
					sprintf((char *)&m_tflv.lpdata, "%s", vect_PartName[i].c_str());
					DBTFLV.push_back(m_tflv);
					
					// 离开时间
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
					
					// 公会名字
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "离开帮会记录查询");
	}

	delete []retMessage;

	return DBTFLV;
}

//角色日志:创建帮会
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
		REQ_LOG_CHAR_CREATE_GUILD		req_PartLog_info;				//创建帮会记录查询请求
		REP_LOG_CHAR_CREATE_GUILD		rep_PartLog_info;				//创建帮会记录查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "创建帮会记录查询");
		}
		else
		{
			sprintf(retMessage, "创建帮会: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_CHAR_CREATE_GUILD);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "创建帮会记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 角色ID
					_itoa((int)vect_PartID[i], strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 角色名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = vect_PartName[i].length();
					sprintf((char *)&m_tflv.lpdata, "%s", vect_PartName[i].c_str());
					DBTFLV.push_back(m_tflv);
					
					// 创建时间
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
					
					//创建类型
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_OpType;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch(vect_LogInfo[i].create_type)
					{
					case 0:
						m_tflv.m_tvlength = strlen("创建申请") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "创建申请");
						break;
					case 1:
						m_tflv.m_tvlength = strlen("创建成功") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "创建成功");
						break;
					case 2:
						m_tflv.m_tvlength = strlen("创建失败") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "创建失败");
						break;
					default:
						break;
					}
					DBTFLV.push_back(m_tflv);

					// 公会ID
					_itoa((int)vect_LogInfo[i].guild_id.guildIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GuildID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 公会名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GuildName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].guild_name.guildName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].guild_name.guildName);
					DBTFLV.push_back(m_tflv);

					// 消耗物品ID
					_itoa((int)vect_LogInfo[i].item_type_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemTypeId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 消耗物品名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_itemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);

					// 消耗金钱
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "创建帮会记录查询");
	}

	delete []retMessage;
	return DBTFLV;
}

//角色日志:解散帮会
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
		REQ_LOG_CHAR_DISMISS_GUILD		req_PartLog_info;				//解散帮会记录查询请求
		REP_LOG_CHAR_DISMISS_GUILD		rep_PartLog_info;				//解散帮会记录查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "解散帮会记录查询");
		}
		else
		{
			sprintf(retMessage, "解散帮会: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_CHAR_DISMISS_GUILD);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "解散帮会记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 角色ID
					_itoa((int)vect_PartID[i], strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 角色名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = vect_PartName[i].length();
					sprintf((char *)&m_tflv.lpdata, "%s", vect_PartName[i].c_str());
					DBTFLV.push_back(m_tflv);
					
					// 解散时间
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
					
					// 公会名字
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "解散帮会记录查询");
	}

	delete []retMessage;
	return DBTFLV;
}

//角色日志:转职信息查询
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
		REQ_LOG_CHAR_CHANGE_OCC		req_PartLog_info;				//转职信息查询请求
		REP_LOG_CHAR_CHANGE_OCC		rep_PartLog_info;				//转职信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "转职信息查询");
		}
		else
		{
			sprintf(retMessage, "转职信息查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_CHAR_CHANGE_OCC);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "转职信息查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 角色ID
					_itoa((int)vect_PartID[i], strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 角色名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = vect_PartName[i].length();
					sprintf((char *)&m_tflv.lpdata, "%s", vect_PartName[i].c_str());
					DBTFLV.push_back(m_tflv);
					
					// 转职时间
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
					
					// 转职前职业
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_Profession;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch(vect_LogInfo[i].old_occupation)
					{
					case 1:
						m_tflv.m_tvlength = strlen("剑士") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "剑士");
						break;
					case 2:
						m_tflv.m_tvlength = strlen("武者") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "武者");
						break;
					case 3:
						m_tflv.m_tvlength = strlen("法师") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "法师");
						break;
					case 4:
						m_tflv.m_tvlength = strlen("魔剑士") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "魔剑士");
						break;
					case 5:
						m_tflv.m_tvlength = strlen("念气士") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "念气士");
						break;
					case 6:
						m_tflv.m_tvlength = strlen("摄魂师") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "摄魂师");
						break;
					case 7:
						m_tflv.m_tvlength = strlen("狂战士") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "狂战士");
						break;
					case 8:
						m_tflv.m_tvlength = strlen("鬼武者") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "鬼武者");
						break;
					case 9:
						m_tflv.m_tvlength = strlen("元素使") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "元素使");
						break;
					case 10:
						m_tflv.m_tvlength = strlen("圣战士") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "圣战士");
						break;
					case 11:
						m_tflv.m_tvlength = strlen("武道家") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "武道家");
						break;
					case 12:
						m_tflv.m_tvlength = strlen("神官") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "神官");
						break;
					case 13:
						m_tflv.m_tvlength = strlen("炼金术师") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "炼金术师");
						break;
					case 14:
						m_tflv.m_tvlength = strlen("钢兵术师") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "钢兵术师");
						break;
					case 15:
						m_tflv.m_tvlength = strlen("魔炼术师") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "魔炼术师");
						break;
					case 16:
						m_tflv.m_tvlength = strlen("火器术师") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "火器术师");
						break;
					default:
						m_tflv.m_tvlength = strlen("未知") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "未知");
						break;
					}
					DBTFLV.push_back(m_tflv);

					// 转职后职业
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NewProfession;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch(vect_LogInfo[i].new_occupation)
					{
					case 1:
						m_tflv.m_tvlength = strlen("剑士") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "剑士");
						break;
					case 2:
						m_tflv.m_tvlength = strlen("武者") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "武者");
						break;
					case 3:
						m_tflv.m_tvlength = strlen("法师") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "法师");
						break;
					case 4:
						m_tflv.m_tvlength = strlen("魔剑士") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "魔剑士");
						break;
					case 5:
						m_tflv.m_tvlength = strlen("念气士") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "念气士");
						break;
					case 6:
						m_tflv.m_tvlength = strlen("摄魂师") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "摄魂师");
						break;
					case 7:
						m_tflv.m_tvlength = strlen("狂战士") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "狂战士");
						break;
					case 8:
						m_tflv.m_tvlength = strlen("鬼武者") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "鬼武者");
						break;
					case 9:
						m_tflv.m_tvlength = strlen("元素使") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "元素使");
						break;
					case 10:
						m_tflv.m_tvlength = strlen("圣战士") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "圣战士");
						break;
					case 11:
						m_tflv.m_tvlength = strlen("武道家") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "武道家");
						break;
					case 12:
						m_tflv.m_tvlength = strlen("神官") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "神官");
						break;
					case 13:
						m_tflv.m_tvlength = strlen("炼金术师") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "炼金术师");
						break;
					case 14:
						m_tflv.m_tvlength = strlen("钢兵术师") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "钢兵术师");
						break;
					case 15:
						m_tflv.m_tvlength = strlen("魔炼术师") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "魔炼术师");
						break;
					case 16:
						m_tflv.m_tvlength = strlen("火器术师") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "火器术师");
						break;
					default:
						m_tflv.m_tvlength = strlen("未知") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "未知");
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "转职信息查询");
	}

	delete []retMessage;

	return DBTFLV;
}

//角色日志:技能日志记录
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
		REQ_LOG_CHAR_SKILL		req_PartLog_info;				//技能日志记录请求
		REP_LOG_CHAR_SKILL		rep_PartLog_info;				//技能日志记录回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "技能日志记录查询");
		}
		else
		{
			sprintf(retMessage, "技能日志记录: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_CHAR_SKILL);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "技能日志记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 操作类型
// 					m_tflv.nIndex = DBTFLV.size()+1;
// 					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
// 					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
// 					switch(vect_LogInfo[i].learn_type)
// 					{
// 					case REP_LOG_CHAR_SKILL_ELE::LEARN_TYPE::LT_LEARN_NEW:
// 						m_tflv.m_tvlength = strlen("学习技能") + 1;
// 						sprintf((char *)&m_tflv.lpdata, "%s", "学习技能");
// 					case REP_LOG_CHAR_SKILL_ELE::LEARN_TYPE::LT_LEVELUP:
// 						m_tflv.m_tvlength = strlen("升级技能") + 1;
// 						sprintf((char *)&m_tflv.lpdata, "%s", "升级技能");
// 					}
// 					DBTFLV.push_back(m_tflv);
					
					// 技能ID
					_itoa((int)vect_LogInfo[i].skill_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SkillID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 技能名称
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SkillName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].skill_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].skill_name);
					DBTFLV.push_back(m_tflv);

					// 技能等级
					_itoa((int)vect_LogInfo[i].skill_level, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SkillLvl;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 学习类型
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_OpType;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch(vect_LogInfo[i].learn_type)
					{
					case REP_LOG_CHAR_SKILL_ELE::LEARN_TYPE::LT_LEARN_NEW:
						m_tflv.m_tvlength = strlen("学习技能") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "学习技能");
						break;
					case REP_LOG_CHAR_SKILL_ELE::LEARN_TYPE::LT_LEVELUP:
						m_tflv.m_tvlength = strlen("升级技能") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "升级技能");
						break;
					default:
						m_tflv.m_tvlength = strlen("未知") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "未知");
						break;
					}
					DBTFLV.push_back(m_tflv);

					// 消耗经验数量
					_itoa((int)vect_LogInfo[i].cosume_exp, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_Exp;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 消耗金钱
					_itoa((int)vect_LogInfo[i].cosume_meony, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 操作时间
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "技能日志记录查询");
	}

	delete []retMessage;

	return DBTFLV;
}

//角色日志:生活技能日志记录
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
		REQ_LOG_CHAR_LIVING_SKILL		req_PartLog_info;				//生活技能日志记录请求
		REP_LOG_CHAR_LIVING_SKILL		rep_PartLog_info;				//生活技能日志记录回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "技能日志记录查询");
		}
		else
		{
			sprintf(retMessage, "生活技能日志记录: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_CHAR_LIVING_SKILL);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "生活技能日志记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 操作类型
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_OpType;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch(vect_LogInfo[i].learn_type)
					{
					case 1:
						m_tflv.m_tvlength = strlen("打造") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "打造");
						break;
					case 2:
						m_tflv.m_tvlength = strlen("裁缝") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "裁缝");
						break;
					case 3:
						m_tflv.m_tvlength = strlen("手工") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "手工");
						break;
					case 4:
						m_tflv.m_tvlength = strlen("制药") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "制药");
						break;
					case 5:
						m_tflv.m_tvlength = strlen("附魔") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "附魔");
						break;
					default:
						m_tflv.m_tvlength = strlen("未知") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "未知");
						break;
					}
					DBTFLV.push_back(m_tflv);

					// 技能等级
					_itoa((int)vect_LogInfo[i].skill_level, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SkillLvl;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 消耗经验数量
					_itoa((int)vect_LogInfo[i].cosume_exp, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_Exp;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 消耗金钱
					_itoa((int)vect_LogInfo[i].cosume_meony, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_ConsumeMoney;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 升级时间
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "生活技能日志记录查询");
	}

	delete []retMessage;

	return DBTFLV;
}

//佣兵团日志:创建记录
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
		REQ_LOG_GUILD_CREATE	req_PartLog_info;				//创建记录请求
		REP_LOG_GUILD_CREATE	rep_PartLog_info;			//创建记录回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "佣兵团日志:创建记录查询");
		}
		else
		{
			sprintf(retMessage, "佣兵团日志:创建记录: <大区>%s, <公会名称>%s, <开始时间>%s, <结束时间>%s", ServerIP, GuildName, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_GUILD_CREATE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_GUILDNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.guild_name, GuildName, strlen(GuildName)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "佣兵团日志:创建记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 角色ID
					_itoa((int)vect_LogInfo[i].char_id.charIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 角色名称
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].char_name.charName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].char_name.charName);
					DBTFLV.push_back(m_tflv);
					
					//创建时间
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

					// 公会ID
					_itoa((int)vect_LogInfo[i].guild_id.guildIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GuildID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 公会名称
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "佣兵团日志:创建记录查询");
	}

	delete []retMessage;

	return DBTFLV;
}

//佣兵团日志:解散记录
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
		REQ_LOG_GUILD_DIMISS		req_PartLog_info;				//解散记录请求
		REP_LOG_GUILD_DISMISS		rep_PartLog_info;				//解散记录回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "佣兵团日志:解散记录");
		}
		else
		{
			sprintf(retMessage, "佣兵团日志:解散记录: <大区>%s, <公会名称>%s, <开始时间>%s, <结束时间>%s", ServerIP, GuildName, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_GUILD_DIMISS);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_GUILDNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.guild_name, GuildName, strlen(GuildName)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "佣兵团日志:解散记录");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 角色ID
					_itoa((int)vect_LogInfo[i].char_id.charIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 角色名称
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].char_name.charName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].char_name.charName);
					DBTFLV.push_back(m_tflv);
					
					//解散时间
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
					
					// 公会ID
					_itoa((int)vect_LogInfo[i].guild_id.guildIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GuildID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 公会名称
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "佣兵团日志:解散记录");
	}

	delete []retMessage;

	return DBTFLV;
}

//宠物徽章日志:宠物精魂捕捉记录
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
		REQ_LOG_PET_CATCH		req_PartLog_info;				//宠物精魂捕捉记录请求
		REP_LOG_PET_CATCH		rep_PartLog_info;				//宠物精魂捕捉记录回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "宠物精魂捕捉记录查询");
		}
		else
		{
			sprintf(retMessage, "宠物精魂捕捉记录查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_PET_CATCH);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "宠物精魂捕捉记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 角色ID
					_itoa((int)vect_PartID[i], strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 角色名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = vect_PartName[i].length();
					sprintf((char *)&m_tflv.lpdata, "%s", vect_PartName[i].c_str());
					DBTFLV.push_back(m_tflv);

					//精魂名称
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_petName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].pet_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].pet_name);
					DBTFLV.push_back(m_tflv);
					
					// 捕捉时间
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "宠物精魂捕捉记录查询");
	}

	delete []retMessage;

	return DBTFLV;
}

//宠物徽章日志
//精魂交易记录:交易获得
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
		REQ_LOG_PET_GET_TRADE		req_PartLog_info;				//交易获得记录请求
		REP_LOG_PET_GET_TRADE		rep_PartLog_info;				//交易获得记录回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "精魂交易记录:交易获得记录查询");
		}
		else
		{
			sprintf(retMessage, "精魂交易记录:交易获得记录查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_PET_GET_TRADE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "精魂交易记录:交易获得记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 角色ID
					_itoa((int)vect_PartID[i], strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 角色名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = vect_PartName[i].length();
					sprintf((char *)&m_tflv.lpdata, "%s", vect_PartName[i].c_str());
					DBTFLV.push_back(m_tflv);

					//对方昵称
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcCharName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].src_char_name.charName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].src_char_name.charName);
					DBTFLV.push_back(m_tflv);
					
					//获得物品
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_RewardItemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);

					// 交易时间
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "精魂交易记录:交易获得记录查询");
	}

	delete []retMessage;

	return DBTFLV;
}

//宠物徽章日志
//精魂交易记录:交易失去
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
		REQ_LOG_PET_LOSE_TRADE		req_PartLog_info;				//交易失去记录请求
		REP_LOG_PET_LOSE_TRADE		rep_PartLog_info;				//交易失去记录回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "精魂交易记录:交易失去记录查询");
		}
		else
		{
			sprintf(retMessage, "精魂交易记录:交易失去记录查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_PET_LOSE_TRADE);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "精魂交易记录:交易失去记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 角色ID
					_itoa((int)vect_PartID[i], strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 角色名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = vect_PartName[i].length();
					sprintf((char *)&m_tflv.lpdata, "%s", vect_PartName[i].c_str());
					DBTFLV.push_back(m_tflv);

					//对方昵称
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcCharName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].dst_char_name.charName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].dst_char_name.charName);
					DBTFLV.push_back(m_tflv);
					
					//获得物品
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GiveItemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);

					// 交易时间
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "精魂交易记录:交易失去记录查询");
	}

	delete []retMessage;

	return DBTFLV;
}

//宠物徽章日志:精魂出售记录
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
		REQ_LOG_PET_BOOTH		req_PartLog_info;				//精魂出售记录请求
		REP_LOG_PET_BOOTH		rep_PartLog_info;				//精魂出售记录回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "精魂出售记录");
		}
		else
		{
			sprintf(retMessage, "精魂出售记录查询: <大区>%s, <帐号>%s, <角色名>%s, <开始时间>%s, <结束时间>%s", ServerIP, Account, Character, Begintime, Endtime);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_LOG_PET_BOOTH);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//开始时间，结束时间
			m_OperatorWA.ParseTime(Begintime, req_PartLog_info.startTime);
			m_OperatorWA.ParseTime(Endtime, req_PartLog_info.endTime);

			req_PartLog_info.availableCondition = ADMIN::QC_CHARACTERNAME;
			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "精魂出售记录查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					// 角色ID
					_itoa((int)vect_PartID[i], strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 角色名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = vect_PartName[i].length();
					sprintf((char *)&m_tflv.lpdata, "%s", vect_PartName[i].c_str());
					DBTFLV.push_back(m_tflv);

					//对方昵称
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SrcCharName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].dst_char_name.charName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].dst_char_name.charName);
					DBTFLV.push_back(m_tflv);
					
					//获得物品
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GiveItemName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].item_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].item_name);
					DBTFLV.push_back(m_tflv);

					// 交易时间
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "精魂出售记录查询");
	}

	delete []retMessage;

	return DBTFLV;
}

//用户信息查询:技能查询
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
		REQ_CHARACTER_INFO req_Log_info;				//技能信息查询请求
		REP_CHARACTER_SKILL_INFO rep_Log_info;			//技能信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "身上技能信息查询");
		}
		else
		{
			sprintf(retMessage, "身上技能信息查询: <大区>%s, <帐号>%s, <角色名>%s", ServerIP, Account, Character);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_CHARACTER_INFO);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			//大区
			req_Log_info.srv_realm_id=srv_Realmid;
			//服务器组ID
			req_Log_info.srv_cluster_id=srv_ClusterID;
			req_Log_info.req_type = REQ_CHARACTER_INFO::RT_CHAR_SKILL_INFO;
			req_Log_info.query_condition = ADMIN::QC_CHARACTERNAME;
			memcpy(&req_Log_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_Log_info, req_packetLength);
			
				if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "玩家身上技能查询");
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
 					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&rep_Log_info, recvbuf+offset+4, packetSize);
								//处理数据
								if (rep_Log_info.allSend == 1)	//收到最后一个结构包
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

				//处理结果集
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
					// 技能ID
					_itoa((int)vect_LogInfo[i].skillId, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SkillID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					//技能名称
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_SkillName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].skillName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].skillName);
					DBTFLV.push_back(m_tflv);

					//技能等级
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "身上技能信息查询");
	}

	return DBTFLV;
}

//用户信息查询:任务查询
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
		REQ_QUERY_QUEST_INFO		req_PartLog_info;				//任务查询请求
		REP_QUERY_QUEST_INFO		rep_PartLog_info;				//任务查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "角色任务信息查询");
		}
		else
		{
			sprintf(retMessage, "角色任务信息查询: <大区>%s, <帐号>%s, <角色名>%s", ServerIP, Account, Character);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_QUERY_QUEST_INFO);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段

			req_PartLog_info.srv_realm_id=srv_Realmid;
			req_PartLog_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_PartLog_info.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_PartLog_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "角色任务信息查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&rep_PartLog_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					//任务完成状态
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_OpType;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch(vect_LogInfo[i].quest_type)
					{
					case CHARACTER_QUEST_INFO::CHARACTER_QUEST_TYPE::CQT_CURRENT:
						m_tflv.m_tvlength = strlen("已接") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "已接");
						break;
					case CHARACTER_QUEST_INFO::CHARACTER_QUEST_TYPE::CQT_FINISHED:
						m_tflv.m_tvlength = strlen("已完成") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "已完成");
						break;
					default:
						m_tflv.m_tvlength = strlen("未知") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "未知");
						break;
					}
					DBTFLV.push_back(m_tflv);

					// 任务ID
					_itoa((int)vect_LogInfo[i].quest_id, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_QuestId;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 任务名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_QuestName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].quest_name);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].quest_name);
					DBTFLV.push_back(m_tflv);

					// 时间
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "角色任务信息查询");
	}

	delete []retMessage;

	return DBTFLV;
}

//佣兵团信息查询
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
		REQ_QUERY_GUILD_INFO		req_Guild_info;				//佣兵团信息查询请求
		REP_QUERY_GUILD_INFO		rep_Guild_info;				//佣兵团信息查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "佣兵团信息查询");
		}
		else
		{
			sprintf(retMessage, "佣兵团信息查询: <大区>%s, <公会名>%s", ServerIP, MercenaryGroup_Name);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_QUERY_GUILD_INFO);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段

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
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "佣兵团信息查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&rep_Guild_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					//任务完成状态
// 					m_tflv.nIndex = DBTFLV.size()+1;
// 					m_tflv.m_tagName = CEnumCore::TagName::WA_OpType;
// 					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					switch(vect_LogInfo[i].quest_type)
// 					{
// 					case CHARACTER_QUEST_INFO::CHARACTER_QUEST_TYPE::CQT_CURRENT:
// 						m_tflv.m_tvlength = strlen("未完成") + 1;
// 						sprintf((char *)&m_tflv.lpdata, "%s", "未完成");
// 						break;
// 					case CHARACTER_QUEST_INFO::CHARACTER_QUEST_TYPE::CQT_FINISHED:
// 						m_tflv.m_tvlength = strlen("已完成") + 1;
// 						sprintf((char *)&m_tflv.lpdata, "%s", "已完成");
// 						break;
// 					default:
// 						m_tflv.m_tvlength = strlen("未知") + 1;
// 						sprintf((char *)&m_tflv.lpdata, "%s", "未知");
// 						break;
// 					}
// 					DBTFLV.push_back(m_tflv);

					// 公会ID
					_itoa((int)vect_LogInfo[i].guild_id.guildIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GuildID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 公会名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GuildName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].guild_name.guildName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].guild_name.guildName);
					DBTFLV.push_back(m_tflv);

					// 公会等级
					_itoa((int)vect_LogInfo[i].guild_level, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GuildLevel;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 公会人数
					_itoa((int)vect_LogInfo[i].guild_member_count, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GuildMemCount;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);

					// 成立时间
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "佣兵团信息查询");
	}

	delete []retMessage;

	return DBTFLV;
}

//佣兵团成员列表查询
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
		REQ_QUERY_GUILD_MEMBER		req_GuildmemList_info;				//佣兵团成员列表查询请求
		REP_QUERY_GUILD_MEMBER		rep_GuildmemList_info;				//佣兵团成员列表查询回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[20480];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录游戏服务器失败", "佣兵团成员列表查询");
		}
		else
		{
			sprintf(retMessage, "佣兵团成员列表查询: <大区>%s, <公会名>%s", ServerIP, MercenaryGroup_Name);		
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			req_packetLength = sizeof(REQ_QUERY_GUILD_MEMBER);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段

			req_GuildmemList_info.srv_realm_id=srv_Realmid;
			req_GuildmemList_info.srv_cluster_id=srv_ClusterID;
			memcpy(&req_GuildmemList_info.guld_name, MercenaryGroup_Name, strlen(MercenaryGroup_Name)+1);
			memcpy(sendbuf+4, &req_GuildmemList_info, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "佣兵团成员列表查询");				
			}
			else
			{
				bool bRecvFlag = true;	//是否需要继续接收数据包
				while (bRecvFlag)
				{
					//memset(recvbuf, 0, 20480);
					*recvbuf = NULL;
					
					// 设置接收缓冲区大小
					DWORD nRecvBuf=20*1024;
					setsockopt(m_MySocket.GetSocketHandle(),SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(DWORD));
					Sleep(1000);
					int recvbyte = m_MySocket.MyRecv(recvbuf,20480);
					if ( recvbyte>0 && strcmp(recvbuf, "") && vect_LogInfo.size() == 0 )
					{
						short int packetSize = 0;	//每个结构体数据包长
						int nLeft = recvbyte;		//接收的字节流偏移后剩下的字节数
						int offset = 0;				//指针偏移量
						
						while (nLeft>0)
						{
							packetSize = *(short int *)(recvbuf+offset);

							if (nLeft<packetSize+4 || packetSize <= 0)
							{
								break;
							}
							else
							{
								//取出数据包
								memcpy(&rep_GuildmemList_info, recvbuf+offset+4, packetSize);

								//处理数据
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

				//处理结果集
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
					//任务完成状态
// 					m_tflv.nIndex = DBTFLV.size()+1;
// 					m_tflv.m_tagName = CEnumCore::TagName::WA_OpType;
// 					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
// 					switch(vect_LogInfo[i].quest_type)
// 					{
// 					case CHARACTER_QUEST_INFO::CHARACTER_QUEST_TYPE::CQT_CURRENT:
// 						m_tflv.m_tvlength = strlen("未完成") + 1;
// 						sprintf((char *)&m_tflv.lpdata, "%s", "未完成");
// 						break;
// 					case CHARACTER_QUEST_INFO::CHARACTER_QUEST_TYPE::CQT_FINISHED:
// 						m_tflv.m_tvlength = strlen("已完成") + 1;
// 						sprintf((char *)&m_tflv.lpdata, "%s", "已完成");
// 						break;
// 					default:
// 						m_tflv.m_tvlength = strlen("未知") + 1;
// 						sprintf((char *)&m_tflv.lpdata, "%s", "未知");
// 						break;
// 					}
// 					DBTFLV.push_back(m_tflv);

					// 角色ID
					_itoa((int)vect_LogInfo[i].char_id.charIndex, strInt, 10);
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_NickID;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
					m_tflv.m_tvlength = strlen(strInt)+1;
					memcpy(m_tflv.lpdata, strInt, strlen(strInt)+1);
					DBTFLV.push_back(m_tflv);
					
					// 角色名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_UserNick;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(vect_LogInfo[i].char_name.charName);
					sprintf((char *)&m_tflv.lpdata, "%s", vect_LogInfo[i].char_name.charName);
					DBTFLV.push_back(m_tflv);

					// 职级
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_MemLevel;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					switch(vect_LogInfo[i].membership_level)
					{
					case ADMIN::GJ_MASTER:
						m_tflv.m_tvlength = strlen("会长") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "会长");
						break;
					case ADMIN::GJ_NORMAL_MEMBER:
						m_tflv.m_tvlength = strlen("一般会员") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "一般会员");
						break;
					default:
						m_tflv.m_tvlength = strlen("未知") + 1;
						sprintf((char *)&m_tflv.lpdata, "%s", "未知");
						break;
					}
					DBTFLV.push_back(m_tflv);

					// 公会名字
					m_tflv.nIndex = DBTFLV.size()+1;
					m_tflv.m_tagName = CEnumCore::TagName::WA_GuildName;
					m_tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
					m_tflv.m_tvlength = strlen(MercenaryGroup_Name);
					sprintf((char *)&m_tflv.lpdata, "%s", MercenaryGroup_Name);
					DBTFLV.push_back(m_tflv);

					// 时间
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
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "佣兵团成员列表查询");
	}

	delete []retMessage;

	return DBTFLV;
}


//操作类:
//宠物管理:添加宠物(等级\属性)
vector <CGlobalStruct::TFLV> CWAInfo::WA_PetAdded_Operater(int userByID, char * ServerIP, char* Character, int PetTypeID)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	try
	{
		
		REQ_ADD_PET req_PetAdded;		//添加宠物
		REP_ADD_PET rep_PetAdded;		//添加宠物
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录服务器失败", "添加宠物");	
		}
		else
		{
			sprintf(retMessage, "添加宠物:<大区>%s, <角色名>%s,<宠物类型ID>%d", ServerIP, Character,PetTypeID);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG
			
			req_packetLength = sizeof(REQ_ADD_PET);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			memcpy(&req_PetAdded.char_name, Character, strlen(Character)+1);
			//大区
			req_PetAdded.srv_realm_id=srv_Realmid;
			req_PetAdded.srv_cluster_id=srv_ClusterID;
			req_PetAdded.pet_type_id = PetTypeID;
			memcpy(sendbuf+4, &req_PetAdded, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "添加宠物");
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
					case REP_ADD_PET::E_SUCCESS:	//成功
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "添加宠物成功", "添加宠物");	
						break;
					default:						//失败
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "添加宠物失败", "添加宠物");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "添加宠物超时", "添加宠物");
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "添加宠物");		
	}
	
	m_OperatorWA.GmOperLog(userByID, ServerIP, "添加宠物", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//宠物管理:删除宠物(等级\属性)
vector <CGlobalStruct::TFLV> CWAInfo::WA_PetDeleted_Operater(int userByID, char * ServerIP, char* Character, int PetUniqueID)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	try
	{
		
		REQ_REMOVE_PET req_PetDeleted;		//删除宠物
		REP_REMOVE_PET rep_PetDeleted;		//删除宠物
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录服务器失败", "删除宠物");	
		}
		else
		{
			sprintf(retMessage, "删除宠物:<大区>%s, <角色名>%s,<宠物唯一ID>%d", ServerIP, Character,PetUniqueID);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG
			
			req_packetLength = sizeof(REQ_REMOVE_PET);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			memcpy(&req_PetDeleted.char_name, Character, strlen(Character)+1);
			//大区
			req_PetDeleted.srv_realm_id=srv_Realmid;
			req_PetDeleted.srv_cluster_id=srv_ClusterID;
			req_PetDeleted.pet_unique_id = PetUniqueID;
			memcpy(sendbuf+4, &req_PetDeleted, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "删除宠物");
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
					case REP_REMOVE_PET::E_SUCCESS:	//成功
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "删除宠物成功", "删除宠物");	
						break;
					default:						//失败
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "删除宠物失败", "删除宠物");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "删除宠物超时", "删除宠物");
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "删除宠物");		
	}
	
	m_OperatorWA.GmOperLog(userByID, ServerIP, "删除宠物", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//家族管理:转让会长权限
vector <CGlobalStruct::TFLV> CWAInfo::WA_AttornPopedom_Operater(int userByID, char * ServerIP, char* GuildName, char * Character)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	try
	{
		
		REQ_GUILD_CHANGE_MASTER req_GuildChanged;		//转让会长权限
		REP_GUILD_CHANGE_MASTER rep_GuildChanged;		//转让会长权限
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录服务器失败", "转让会长权限");	
		}
		else
		{
			sprintf(retMessage, "转让会长权限:<大区>%s, <角色名>%s,<公会名称>%s", ServerIP, Character,GuildName);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG
			
			req_packetLength = sizeof(REQ_GUILD_CHANGE_MASTER);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			memcpy(&req_GuildChanged.char_name, Character, strlen(Character)+1);
			memcpy(&req_GuildChanged.guld_name, GuildName, strlen(GuildName)+1);
			//大区
			req_GuildChanged.srv_realm_id=srv_Realmid;
			req_GuildChanged.srv_cluster_id=srv_ClusterID;
			memcpy(sendbuf+4, &req_GuildChanged, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "转让会长权限");
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
					case REP_GUILD_CHANGE_MASTER::E_SUCCESS:		//成功
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "转让会长权限成功", "转让会长权限");	
						break;
					default:										//失败
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "转让会长权限失败", "转让会长权限");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "转让会长权限超时", "转让会长权限");
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "转让会长权限");		
	}
	
	m_OperatorWA.GmOperLog(userByID, ServerIP, "转让会长权限", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//公会管理:修改公会名称
vector <CGlobalStruct::TFLV> CWAInfo::WA_ModifyGuildName_Operater(int userByID, char * ServerIP, char * GuildName, char* NewGuildName)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	try
	{
		
		REQ_GUILD_CHANGE_NAME req_GuildNameChanged;		//修改公会名称
		REP_GUILD_CHANGE_NAME rep_GuildNameChanged;		//修改公会名称
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录服务器失败", "修改公会名称");	
		}
		else
		{
			sprintf(retMessage, "修改公会名称:<大区>%s, <公会名称>%s,<新公会名称>%s", ServerIP, GuildName,NewGuildName);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG
			
			req_packetLength = sizeof(REQ_GUILD_CHANGE_NAME);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			memcpy(&req_GuildNameChanged.guld_name, GuildName, strlen(GuildName)+1);
			memcpy(&req_GuildNameChanged.new_name, NewGuildName, strlen(NewGuildName)+1);
			//大区
			req_GuildNameChanged.srv_realm_id=srv_Realmid;
			req_GuildNameChanged.srv_cluster_id=srv_ClusterID;
			memcpy(sendbuf+4, &req_GuildNameChanged, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "修改公会名称");
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
					case REP_GUILD_CHANGE_NAME::E_SUCCESS:		//成功
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "修改公会名称成功", "修改公会名称");	
						break;
					default:									//失败
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "修改公会名称失败", "修改公会名称");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "修改公会名称超时", "转让会长权限");
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "修改公会名称");		
	}
	
	m_OperatorWA.GmOperLog(userByID, ServerIP, "修改公会名称", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//公会管理:踢出公会成员
vector <CGlobalStruct::TFLV> CWAInfo::WA_KickGuildMember_Operater(int userByID, char * ServerIP, char * Character, char* GuildName)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	try
	{
		
		REQ_GUILD_KICK_MEMBER req_KickedMember;		//踢出公会成员
		REP_GUILD_KICK_MEMBER rep_KickedMember;		//踢出公会成员
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录服务器失败", "踢出公会成员");	
		}
		else
		{
			sprintf(retMessage, "踢出公会成员:<大区>%s, <公会名称>%s,<要踢的角色名称>%s", ServerIP, GuildName,Character);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG
			
			req_packetLength = sizeof(REQ_GUILD_KICK_MEMBER);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			memcpy(&req_KickedMember.guld_name, GuildName, strlen(GuildName)+1);
			memcpy(&req_KickedMember.char_name, Character, strlen(Character)+1);
			//大区
			req_KickedMember.srv_realm_id=srv_Realmid;
			req_KickedMember.srv_cluster_id=srv_ClusterID;
			memcpy(sendbuf+4, &req_KickedMember, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "踢出公会成员");
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
					case REP_GUILD_KICK_MEMBER::E_SUCCESS:		//成功
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "踢出公会成员成功", "踢出公会成员");	
						break;
					default:										//失败
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "踢出公会成员失败", "踢出公会成员");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "踢出公会成员超时", "踢出公会成员");
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "踢出公会成员");		
	}
	
	m_OperatorWA.GmOperLog(userByID, ServerIP, "踢出公会成员", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//公会管理:解散公会
vector <CGlobalStruct::TFLV> CWAInfo::WA_DissolveGuild_Operater(int userByID, char * ServerIP, char* GuildName)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	try
	{
		
		REQ_GUILD_DISMISS req_GuildDismiss;		//解散公会
		REP_GUILD_DISMISS rep_GuildDismiss;		//解散公会
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录服务器失败", "解散公会");	
		}
		else
		{
			sprintf(retMessage, "解散公会:<大区>%s, <公会名称>%s", ServerIP, GuildName);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG
			
			req_packetLength = sizeof(REQ_GUILD_DISMISS);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			memcpy(&req_GuildDismiss.guld_name, GuildName, strlen(GuildName)+1);
			//大区
			req_GuildDismiss.srv_realm_id=srv_Realmid;
			req_GuildDismiss.srv_cluster_id=srv_ClusterID;
			memcpy(sendbuf+4, &req_GuildDismiss, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "解散公会");
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
					case REP_GUILD_KICK_MEMBER::E_SUCCESS:		//成功
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "解散公会成功", "解散公会");	
						break;
					default:										//失败
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "解散公会失败", "解散公会");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "解散公会超时", "解散公会");
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "解散公会");		
	}
	
	m_OperatorWA.GmOperLog(userByID, ServerIP, "解散公会", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//宠物列表查询
vector <CGlobalStruct::TFLV> CWAInfo::WA_PET_QUERY(char* gameName,char* strSql,int index,int Page)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{
		DBTFLV=GameDBQuery("WA","127.0.0.1",4,0,0,"exec WA_PetList_Query");
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("WA","宠物列表查询内存不足");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("WA","宠物列表查询文件异常");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("WA","宠物列表查询其他异常");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("WA",(char *)err.what());
		return DBTFLV;
	}
	return DBTFLV;
}

//玩家角色属性恢复		
vector <CGlobalStruct::TFLV> CWAInfo::WA_ResumeAttribute_Operate(int userByID, char * ServerIP, char *Account, char *Character)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	
	try
	{
		
		REQ_MODIFY_CHARACTER req_modify_character;				//修改金钱请求
		REP_MODIFY_CHARACTER rep_modify_character;				//修改金钱回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录服务器失败", "恢复角色属性");	
		}
		else
		{
			sprintf(retMessage, "角色属性恢复:<大区>%s, <帐号>%s, <角色名>%s", ServerIP, Account, Character);			
			logFile.WriteLog("<勇士OL>",operName, retMessage);//记LOG
			
			req_packetLength = sizeof(REQ_MODIFY_CHARACTER);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			//大区
			req_modify_character.srv_realm_id=srv_Realmid;
			//服务器组ID
			req_modify_character.srv_cluster_id=srv_ClusterID;
			req_modify_character.attr = ADMIN::MCA_ONLINE_REPAIR;
//			req_modify_character.newValue = level;
			memcpy(&req_modify_character.char_name, Character, strlen(Character)+1);
			memcpy(sendbuf+4, &req_modify_character, req_packetLength);
			
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "角色属性恢复");
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
					case REP_DELETE_ITEM::E_SUCCESS:	//成功
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "角色属性恢复成功", "角色属性恢复");	
						break;
					default:							//失败
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "角色属性恢复失败", "角色属性恢复");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "角色属性恢复超时", "角色属性恢复");
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "角色属性恢复");		
	}
	
	m_OperatorWA.GmOperLog(userByID, ServerIP, "角色属性恢复", retMessage, DBTFLV[0]);
	return DBTFLV;
}

//群发邮件(无道具)
vector <CGlobalStruct::TFLV> CWAInfo::WA_SendMails_Operate(int userByID, char *ServerIP, char *Character, char *MailSub, char *MailContent)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	char retMessage[255];
	memset(retMessage, 0, 255);
	try
	{
		REQ_MAIL_ITEM req_SendGift;		//寄送物品请求
		REP_MAIL_ITEM rep_SendGift;		//寄送物品回应
		short int req_packetLength;
		short int flag = 24;
		char *sendbuf = new char[1024];
		char *recvbuf = new char[10240];
		char *pTemp = NULL;
		int iCount = 0;
		CString sztemp = "";
		
		if (!Connect_Remote_Login(ServerIP))
		{
			DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "登录服务器失败", "群发邮件(无道具)");	
		}
		else
		{
			req_packetLength = sizeof(REQ_MAIL_ITEM);
			
			memset(sendbuf,0, 1024);
			memcpy(sendbuf, &req_packetLength, 2);	//添数据包长度
			memcpy(sendbuf+2, &flag, 2);			//固定字段
			
			//Added by tengjie 2010-06-24
			ZeroMemory(req_SendGift.item_type_id_list, sizeof(req_SendGift.item_type_id_list));
			ZeroMemory(req_SendGift.item_count_list, sizeof(req_SendGift.item_count_list));
			ZeroMemory(req_SendGift.item_gen_list, sizeof(req_SendGift.item_gen_list));
			ZeroMemory(req_SendGift.item_expire, sizeof(req_SendGift.item_expire));

			sprintf(retMessage, "群发邮件<大区>%s, <角色名>%s, <邮件标题><%s>,<邮件内容>%s", ServerIP, Character, MailSub, MailContent);			
			logFile.WriteLog("<勇士无双>",operName, retMessage);//记LOG

			memcpy(&req_SendGift.char_name, Character, strlen(Character)+1);
			req_SendGift.receiver_type = REQ_MAIL_ITEM::RT_SINGLE_PLAYER;

			memcpy(req_SendGift.subject, MailSub, strlen(MailSub)+1);
			memcpy(req_SendGift.body, MailContent, strlen(MailContent)+1);
			req_SendGift.money = 0;

			//大区
			req_SendGift.srv_realm_id=srv_Realmid;
			//服务器组ID
			req_SendGift.srv_cluster_id=srv_ClusterID;
			memcpy(sendbuf+4, &req_SendGift, req_packetLength);
			if (!m_MySocket.MySend(sendbuf, req_packetLength+4))
			{
				DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "发送请求失败", "群发邮件");
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
					case REP_MAIL_ITEM::E_SUCCESS:	//成功
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "群发邮件成功", "群发邮件");	
						break;
					default:							//失败
						DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "群发邮件失败", "群发邮件");	
						break;
					}
				}
				else
				{
					DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "群发邮件超时", "群发邮件");
				}
			}
			m_MySocket.Close();
		}
		delete []sendbuf;
		delete []recvbuf;
	}
	catch(...)
	{
		DBTFLV = m_OperatorWA.ReturnOpMsg(operName, "未知错误", "群发邮件");		
	}
	
	m_OperatorWA.GmOperLog(userByID, ServerIP, "群发邮件", retMessage, DBTFLV[0]);
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
// 	//获取游戏IP
// 	char WAServerIp[20]= "";
// 	char WAUrl[128]="";
// 	GetPrivateProfileString("server","WAServer","61.152.150.158",WAServerIp,20,_temppath.c_str());//华东一区
// 	GetPrivateProfileString("url","WAUrl","passport/games/gmtools_ban_log_w.php",WAUrl,128,_temppath.c_str());//WA接收地址
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
// 		if(!strcmp("idx", m_DataRecord.recordfieldname))//获取相应的ServerIP
// 		{
// 			szData.Format("%s", m_DataRecord.recorddata);//保存查询得到的最后一条记录
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
// // 		//ascii转换成unicode
// // 		int  wcsLen  =  ::MultiByteToWideChar(CP_ACP, NULL, you9Nickname.c_str(), you9Nickname.length(), NULL,  0 );
// // 		wchar_t *  wszString  =   new  wchar_t[wcsLen  +   1 ];
// // 		::MultiByteToWideChar(CP_ACP, NULL, you9Nickname.c_str(), you9Nickname.length(), wszString, wcsLen);
// // 		wszString[wcsLen]  = '\0' ;
// // 		
// // 		//unicode转换成utf8
// // 		int u8Len = ::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);
// // 		char* szU8 = new char[u8Len + 1];
// // 		::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), szU8, u8Len, NULL, NULL);
// // 		szU8[u8Len] = '\0';
// 		
// 		int length=0;
// 		
// 		string strMd5 = NewUserName;
// 		strMd5 += "|T4pb5A.gmtoolbanlog";
// 		//md5加密
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
// 		while(lpHttpFile->ReadString(strSentence))    // 读取提交数据后的返回结果
// 		{
// 			strGetSentence = strGetSentence + strSentence + char(13) + char(10);
// 		}
// 		
// 		printf("返回的数据:%s\n",strGetSentence);
// 		
// 	}
// 
// 	return strGetSentence;
// }
//end