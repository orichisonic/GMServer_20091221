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
//连接远程服务端
bool CGTOper::CONNECT_SERVERMANAGER_LOGIN(char * ServerIP,char * retMessage)
{
	try
	{
		char GT_GateWay[40];
		ZeroMemory(GT_GateWay,40);
		int ListenPort=0;//获取远程端口

		if(!operGT.GetServerIP("",ServerIP,2,GT_GateWay,&ListenPort))
		{
			sprintf(retMessage,"%s", "数据库中没有相关数据");
			return false;
		}

	
		int ID=0;
		for(ID=1010;ID<1020;ID++)
		{
			if(msocket.ConnectURL(GT_GateWay, ListenPort) == 0)
			{
				sprintf(retMessage,"%s", "无法连接远程接口");
				msocket.closeSocket();
				return false;
			}

			CPacket req(ServiceKey::GMProtocol::CONNECT_GATEWAY_LOGIN);
			req<<(WORD)12<<(int)ID<<(WORD)9100;
			int count=0;
			if(!msocket.SendNewData(req.GetPack(),req.GetPackLen()))
			{
				sprintf(retMessage,"%s", "无法连接远程接口!");	
				msocket.closeSocket();
				return false;
			}
			CPacket packet;
			int recvLen =-1;
			recvLen	= msocket.recvData();
			if(recvLen<=0)
			{
				sprintf(retMessage,"%s", "登陆失败,请重试!");	
				printf("登陆失败ID:%i,请重试!\n",ID);
				msocket.closeSocket();
			}
			else
			{
				memcpy(packet.GetPack(), msocket.recvBuffer(), recvLen);
				printf("登陆成功ID:%i!\n",ID);
				return TRUE;
			}
			

		}
		
	
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("GT","连接远程接口内存不足");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("GT","连接远程接口文件异常");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("GT","连接远程接口其他异常");
		return false;
	}
	return true;
}

//查询公告
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
					msocket.SendNewData(req.GetPack(),req.GetPackLen());//发送查询公告信息的请求
					
					CPacket packet(ServiceKey::GMProtocol::GM_BROADCAST_SELECT_ACK);
					recvLen = msocket.recvData();//获取查询公告信息的回应
					memcpy(packet.GetPack(),msocket.recvBuffer(),recvLen);
					
					
					
					int count = 0;//公告数量	
					int ID = 0;//公告ID
					wstring Content;//公告内容
					time_t tmpSTime = 0;//开始时间
					time_t tmpETime = 0;//结束时间
					DWORD interval = 0;//公告间隔
					int pageCount = 0;//公告页数
					char strInt[10];//保存int类型的变量
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
							if((i >= (iIndex-1)*iPageSize) && (i < iIndex*iPageSize))//分页查询
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
						m_tflv.m_tvlength=strlen("没有公告");
						sprintf((LPSTR)&m_tflv.lpdata,"%s","没有公告");
						DBVect.push_back(m_tflv);
					}
				}
				
			}
			else
			{
				m_tflv.nIndex=DBVect.size()+1;
				m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
				m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
				m_tflv.m_tvlength=strlen("远程服务端连接失败");
				sprintf((LPSTR)&m_tflv.lpdata,"%s","远程服务端连接失败");
				DBVect.push_back(m_tflv);
			}
			
		}
		else
		{
			
			m_tflv.nIndex=DBVect.size()+1;
			m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
			m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
			m_tflv.m_tvlength=strlen("查找SqL错误");
			sprintf((LPSTR)&m_tflv.lpdata,"%s","查找SqL错误");
			DBVect.push_back(m_tflv);

		}
		msocket.closeSocket();
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("GT","查询公告内存不足");
		return DBVect;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("GT","查询公告文件异常");
		return DBVect;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("GT","查询公告其他异常");
		return DBVect;
	}
	return DBVect;

}

//添加公告
BOOL CGTOper::GT_BROADCAST_ADD(int IsImmediate,wstring context,DWORD interval,size_t StartTime,size_t EndTime)
{
	bool result = true;
	try
	{
		CPacket req(ServiceKey::GMProtocol::GM_BROADCAST_ADD);
		if(IsImmediate == 0)//不是即时发送
		{
			req<<false<<context<<StartTime<<EndTime<<interval;
			msocket.SendNewData(req.GetPack(),req.GetPackLen());//发送添加公告的请求

			CPacket packet;
			int recvLen = msocket.recvData();//获取添加公告的回应
			memcpy(packet.GetPack(), msocket.recvBuffer(), recvLen);
			//packet>>result;//得到添加公告的结果
		}
		else
		{
			req<<true<<context;
			msocket.SendNewData(req.GetPack(),req.GetPackLen());//发送添加公告的请求

			CPacket packet;
			int recvLen = msocket.recvData();//获取添加公告的回应
			memcpy(packet.GetPack(), msocket.recvBuffer(), recvLen);			
	    //	packet>>result;//得到添加公告的结果
		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("GT","添加公告内存不足");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("GT","添加公告文件异常");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("GT","添加公告其他异常");
		return false;
	}
	return result;
}


//删除公告
BOOL CGTOper::GT_BROADCAST_REMOVE(int NoticeID)
{
	bool result = true;
	try
	{
		CPacket req(ServiceKey::GMProtocol::GM_BROADCAST_REMOVE);
		req<<NoticeID;		
		msocket.SendNewData(req.GetPack(),req.GetPackLen());//发送要删除公告的请求
		
	/*	CPacket packet(ServiceKey::GMProtocol::GM_BROADCAST_REMOVE_ACK);
		recvLen = msocket.recvData();//获取删除公告的回应
		memcpy(packet.GetPack(), msocket.recvBuffer(), recvLen);
		packet>>result;//得到删除公告的结果*/
		msocket.closeSocket();
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("GT","删除公告内存不足");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("GT","删除公告文件异常");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("GT","删除公告其他异常");
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
		
		GetPrivateProfileString("SERVER","Address","127.0.0.1",ServerIP,20,filepath);//获取远程接口地址
		int ListenPort=GetPrivateProfileInt("SERVER","Port",0,filepath);//获取远程端口
		
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
		logFile.WriteErrorLog("GT","强制玩家下线内存不足");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("GT","强制玩家下线文件异常");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("GT","强制玩家下线其他异常");
		return false;
	}
	
	return 1;
}
//强制玩家下线（查询通过数据表，这里只用账号踢人）
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
			if(!msocket.SendNewData(req.GetPack(),req.GetPackLen()))//发送强制玩家下线的请求
			{
				sprintf(retMessage,"%s","玩家不在线");
				return FALSE;
			}
			CPacket packet;
			int recvLen = msocket.recvData();//得到强制玩家下线的回应
			memcpy(packet.GetPack(), msocket.recvBuffer(), recvLen);
			packet>>b_result;
			msocket.closeSocket();
		}		
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("GT","强制玩家下线内存不足");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("GT","强制玩家下线文件异常");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("GT","强制玩家下线其他异常");
		return false;
	}
	return b_result;


}

//封停和解封
BOOL CGTOper::GT_BanUser(int userByID, char * ServerName, char * ServerIP,char * UserName,int UserID,char* userState,char * Reason)
{
	try
	{
		int curState = 0;//保存帐号当前有没有被封停
		char retMessage[256];
		char operRemoteSql[256];
		operGT.getRemoteSql("GT","GT_GetUserState_Local",operRemoteSql,4);//获取远程操作的Sql语句
		if(!strcmp("",operRemoteSql))
		{
			operGT.BuildTLV("数据库中找不到对应的sql语句");
			operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
			return false;
		}

		vector <CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GT","127.0.0.1",4,0,0,operRemoteSql, UserID, ServerName);
		if(!DBTFLV.empty())//查询操作没有结果
		{
			curState = StrToInt((char*)((COperVector(DBTFLV)).getTLVByTag(CEnumCore::TagName::GT_UserState, 0)).lpdata);//curState = 1,帐号已被封停
		}
		


		BYTE uState = 0;
		if(!strcmp("1",userState))//要封停玩家帐号
		{
			uState = 1;
			if(curState == 1)//玩家账号已被封停
			{
				operGT.BuildTLV("帐号已被封停，不能再次封停");
				operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
				return false;
			}

			GT_KICK_USER(ServerIP,UserName,retMessage);
		}
		else
		{
			uState = 0;
			if(curState == 0)//玩家账号未被封停
			{
				operGT.BuildTLV("帐号未被封停，无法解封");
				operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
				return false;
			}
		}
		wstring Account=operGT.charToWChar(UserName);
		if(CONNECT_SERVERMANAGER_LOGIN(ServerIP,retMessage))
		{
			CPacket req(ServiceKey::GMProtocol::GM_USER_STOP);
			req<<Account<<uState;
			msocket.SendNewData(req.GetPack(),req.GetPackLen());//发送封停或解封玩家帐号的请求
			
			CPacket packet;
			int recvLen = msocket.recvData();//获取封停或解封玩家帐号的请求
			memcpy(packet.GetPack(), msocket.recvBuffer(), recvLen);
			bool result = true;
			packet>>result;//获取失败或成功的信息
			
			if(!result)
			{
				if(uState == 1)
				{
					sprintf(strLog,"<%s>,封停服务器<%s> 玩家<%s>失败",operName,ServerName,UserName);
					logFile.WriteText("GTown",strLog);//将失败信息写入日志中
					logFile.WriteDBLog(userByID,"吉堂Online","GT_BanPlayer",ServerIP,strLog);
					operGT.BuildTLV("封停帐号失败");
					operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
					
					return false;
				}
				else
				{
					sprintf(strLog,"<%s>,解封服务器<%s> 玩家<%s>失败",operName,ServerName,UserName);
					logFile.WriteText("GTown",strLog);//将失败信息写入日志中
					logFile.WriteDBLog(userByID,"吉堂Online","GT_UnbindPlayer",ServerIP,strLog);
					operGT.BuildTLV("解封帐号失败");
					operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
					return false;
				}
			}
			else
			{
				if(uState == 1)
				{
					sprintf(strLog,"<%s>,封停服务器<%s> 玩家<%s>成功",operName,ServerName,UserName);
					logFile.WriteText("GTown",strLog);//将成功信息写入日志中
					logFile.WriteDBLog(userByID,"吉堂Online","GT_BanPlayer",ServerIP,strLog);


					//获取远程操作的Sql语句
					operGT.getRemoteSql("GT","GT_BanPlayer",operRemoteSql,4);
					if(!strcmp("",operRemoteSql))
					{
						operGT.BuildTLV("数据库中找不到对应的sql语句");
						operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
						return false;
					}

					int tmpLength =GameDBOper("GT","127.0.0.1",4,operRemoteSql, UserID, UserName, 1, Reason, ServerName); //保存封停帐号信息
					if(tmpLength == 0)
					{
						operGT.BuildTLV("封停账号插入本地数据库数据库失败");
						operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
						return false;
					}
					else
					{
						operGT.BuildTLV("封停账号成功");
						operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
						return false;
					}
			}
			else
			{
				sprintf(strLog,"<%s>,解封服务器<%s> 玩家<%s>成功",operName,ServerName,UserName);
				logFile.WriteText("GTown",strLog);//将成功信息写入日志中
				logFile.WriteDBLog(userByID,"吉堂Online","GT_UnbindPlayer",ServerIP,strLog);

				//获取远程操作的Sql语句
				operGT.getRemoteSql("GT","GT_UnbindPlayer",operRemoteSql,4);
				if(!strcmp("",operRemoteSql))
				{
					operGT.BuildTLV("数据库中找不到对应的sql语句");
					operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
					return false;
				}

				
				int tmpLength =GameDBOper("GT","127.0.0.1",4,operRemoteSql,UserID); //保存解封帐号信息
				if(tmpLength == 0)
				{
					operGT.BuildTLV("解封账号修改本地数据库数据库失败");
					operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
					return false;
				}
				else
				{
					operGT.BuildTLV("解封账号成功");
					operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
					return false;
				}
			}
			operGT.BuildTLV("成功");
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
		logFile.WriteErrorLog("GT","封停账号内存不足");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("GT","封停账号文件异常");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("GT","封停账号其他异常");
		return false;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,封停服务器<%s> 玩家<%s>,发生系统错误",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);
		logFile.WriteDBLog(userByID,"吉堂Online","GT_BanPlayer",ServerIP,strLog);
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_STOP_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
		return false;
	}
	return true;
}

bool CGTOper::GT_BanUser(char * ServerIP,char * strAccount,int status)//1.封停,0.解封
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
			msocket.SendNewData(req.GetPack(),req.GetPackLen());//发送封停或解封玩家帐号的请求
			
			CPacket packet;
			recvLen = msocket.recvData();//获取封停或解封玩家帐号的请求
			memcpy(packet.GetPack(), msocket.recvBuffer(), recvLen);
			packet>>result;//获取失败或成功的信息
			return TRUE;

		}

		
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("GT","远程解封账号内存不足");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("GT","远程解封账号文件异常");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("GT","远程解封账号其他异常");
		return false;
	}
	return true;

}

//角色移至黑屋
void CGTOper::GT_MOVE_USER(int userByID, char * ServerName, char * ServerIP,char * UserName)
{
	try
	{
		wstring Account =operGT.charToWChar(UserName);
		if(CONNECT_SERVERMANAGER_LOGIN(ServerIP,strLog))
		{
			CPacket req(ServiceKey::GMProtocol::GM_MOVE_USER);
			req<<Account;
			msocket.SendNewData(req.GetPack(),req.GetPackLen());//发送将角色移到黑屋的请求
			bool result = true;
			if(!result)
			{
				sprintf(strLog,"<%s>,将服务器<%s>中的玩家<%s>移到黑屋失败",operName,ServerIP,UserName);
				logFile.WriteText("GTown",strLog);//将失败信息写入日志中
				logFile.WriteDBLog(userByID,"吉堂Online","GT_MOVE_USER",ServerIP,strLog);
			}
			else
			{
				sprintf(strLog,"<%s>,将服务器<%s>中的玩家<%s>移到黑屋成功",operName,ServerIP,UserName);
				logFile.WriteText("GTown",strLog);//将成功信息写入日志中
				logFile.WriteDBLog(userByID,"吉堂Online","GT_MOVE_USER",ServerIP,strLog);
			}
		}
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_MOVE_USER_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
		
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,将服务器<%s>中的玩家<%s>移到黑屋发生系统错误",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);
		logFile.WriteDBLog(userByID,"吉堂Online","GT_MOVE_USER",ServerIP,strLog);
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_MOVE_USER_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
	}
}
//重设玩家到出生状态
void CGTOper::GT_USER_RESET(int userByID, char * ServerName, char * ServerIP,char * UserName)
{
	try
	{
		wstring Account =operGT.charToWChar(UserName);
		if(CONNECT_SERVERMANAGER_LOGIN(ServerIP,strLog))
		{
			CPacket req(ServiceKey::GMProtocol::GM_USER_RESET);
			req<<Account;
			msocket.SendNewData(req.GetPack(),req.GetPackLen());//发送重设玩家到出生状态的请求
			
			CPacket packet;
			int recvLen = msocket.recvData();//获取重设玩家到出生状态的回应
			memcpy(packet.GetPack(),msocket.recvBuffer(),recvLen);
			bool result = true;
			packet>>Account;
			packet>>result;
			if(!result)
			{
				sprintf(strLog,"<%s>,重设服务器<%s>中的玩家<%s>到出生状态失败",operName,ServerName,UserName);
				logFile.WriteText("GTown",strLog);//将失败信息写入日志中
				logFile.WriteDBLog(userByID,"吉堂Online","GT_USER_RESET",ServerIP,strLog);
			}
			else
			{
				sprintf(strLog,"<%s>,重设服务器<%s>中的玩家<%s>到出生状态成功",operName,ServerName,UserName);
				logFile.WriteText("GTown",strLog);//将成功信息写入日志中
				logFile.WriteDBLog(userByID,"吉堂Online","GT_USER_RESET",ServerIP,strLog);
			}
		}
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_RESET_ACK,operGT.GetSendBuf(),operGT.GetSendLength());


		
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,重设服务器<%s>中的玩家<%s>到出生状态发生系统错误",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);
		logFile.WriteDBLog(userByID,"吉堂Online","GT_USER_RESET",ServerIP,strLog);
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_USER_RESET_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
	}
}


//瞬间移动
void CGTOper::GT_Transmit_Query(int userByID, char * ServerName, char * ServerIP,char * UserName,int x,int y,int z)
{
	try
	{
		wstring Account =operGT.charToWChar(UserName);
		if(CONNECT_SERVERMANAGER_LOGIN(ServerIP,strLog))
		{
			CPacket req(ServiceKey::GMProtocol::GM_TRANSMIT_REQ);
			req<<Account<<(float)x<<(float)y<<(float)z;
			msocket.SendNewData(req.GetPack(),req.GetPackLen());//发送瞬间移动的请求

			bool result = true;
			if(!result)
			{
				sprintf(strLog,"<%s>,移动服务器<%s>中的玩家<%s>到x:%i,y:%i,z:%i失败",operName,ServerName,UserName,x,y,z);
				logFile.WriteText("GTown",strLog);//将失败信息写入日志中
				logFile.WriteDBLog(userByID,"吉堂Online","GT_USER_RESET",ServerIP,strLog);
			}
			else
			{
				sprintf(strLog,"<%s>,移动服务器<%s>中的玩家<%s>到x:%i,y:%i,z:%i成功",operName,ServerName,UserName,x,y,z);
				logFile.WriteText("GTown",strLog);//将成功信息写入日志中
				logFile.WriteDBLog(userByID,"吉堂Online","GT_USER_RESET",ServerIP,strLog);
			}
		}
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Transmit_Query_Resp,operGT.GetSendBuf(),operGT.GetSendLength());

		
		

		
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,移动服务器<%s>中的玩家<%s>发生系统错误",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);
		logFile.WriteDBLog(userByID,"吉堂Online","GT_USER_RESET",ServerIP,strLog);
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Transmit_Query_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
	}
}
//禁止、解禁玩家上传图片
bool CGTOper::GT_Picture_Stop(int userByID, char * ServerName, char * ServerIP,char * UserName,int UserID,char* userState,char * Reason)
{
	try
	{
		int curState=-1;
		vector <CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GT","127.0.0.1",4,0,0,"exec GT_GetStopState %i,'%s'", UserID, ServerName);
		if(!DBTFLV.empty())//查询操作没有结果
		{
			curState = StrToInt((char*)((COperVector(DBTFLV)).getTLVByTag(CEnumCore::TagName::GT_UserState, 0)).lpdata);//curState = 1,帐号已被封停
		}
		
		BYTE uState = 0;
		if(!strcmp("0",userState))//要禁止图片帐号
		{
			uState = 0;
			if(curState == 0)//玩家账号已被禁止图片
			{
				operGT.BuildTLV("帐号已被禁止图片，不能再次禁止图片");
				operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Picture_Stop_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
				return false;
			}
		}
		else
		{
			uState = 1;
			if(curState == 1)//玩家账号未被禁止
			{
				operGT.BuildTLV("帐号未被禁止，无法解禁");
				operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Picture_Stop_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
				return false;
			}
		}
		wstring Account =operGT.charToWChar(UserName);
		if(CONNECT_SERVERMANAGER_LOGIN(ServerIP,strLog))
		{
			CPacket req(ServiceKey::GMProtocol::GM_PICTURE_STOP);
			req<<Account<<uState;
			msocket.SendNewData(req.GetPack(),req.GetPackLen());//发送封停或解封玩家帐号的请求
			
			CPacket packet;
			int recvLen = msocket.recvData();//获取封停或解封玩家帐号的请求
			memcpy(packet.GetPack(), msocket.recvBuffer(), recvLen);
			bool result = true;
			packet>>result;//获取失败或成功的信息
			if(!result)
			{
				if(uState == 0)
				{
					sprintf(strLog,"<%s>,封停服务器<%s> 玩家<%s>失败",operName,ServerName,UserName);
					logFile.WriteText("GTown",strLog);//将失败信息写入日志中
					logFile.WriteDBLog(userByID,"吉堂Online","GT_BanPlayer",ServerIP,strLog);
					operGT.BuildTLV(strLog);
					operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Picture_Stop_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
					return false;
				}
				else
				{
					sprintf(strLog,"<%s>,解封服务器<%s> 玩家<%s>失败",operName,ServerName,UserName);
					logFile.WriteText("GTown",strLog);//将失败信息写入日志中
					logFile.WriteDBLog(userByID,"吉堂Online","GT_UnbindPlayer",ServerIP,strLog);
					operGT.BuildTLV(strLog);
					operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Picture_Stop_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
					return false;
				}	
			}
			
			else
			{
				if(uState == 0)
				{
					sprintf(strLog,"<%s>,禁止服务器<%s> 玩家<%s>图片成功",operName,ServerName,UserName);
					logFile.WriteText("GTown",strLog);//将成功信息写入日志中
					
					int tmpLength=GameDBOper("GT","127.0.0.1",4,"exec GT_StopUser %i,%i,'%s','%s','%s'",userByID,UserID,UserName,ServerName,Reason);
					if(tmpLength == 0)
					{
						operGT.BuildTLV("禁止图片插入本地数据库数据库失败");
						operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Picture_Stop_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
						return false;
					}
					else
					{
						operGT.BuildTLV("禁止图片成功");
						operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Picture_Stop_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
						return false;
					}
				}
				else
				{
					sprintf(strLog,"<%s>,解禁服务器<%s> 玩家<%s>图片成功",operName,ServerName,UserName);
					logFile.WriteText("GTown",strLog);//将成功信息写入日志中
					int tmpLength=GameDBOper("GT","127.0.0.1",4,"exec GT_UnStopUser %i,%i,'%s','%s','%s'",userByID,UserID,UserName,ServerName,Reason);
					if(tmpLength == 0)
					{
						operGT.BuildTLV("解禁账号修改本地数据库数据库失败");
						operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Picture_Stop_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
						return false;
					}
					else
					{
						operGT.BuildTLV("解禁账号成功");
						operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Picture_Stop_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
						return false;
					}
				}
				operGT.BuildTLV("成功");
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
		sprintf(strLog,"<%s>,封停服务器<%s> 玩家<%s>,发生系统错误",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);
		logFile.WriteDBLog(userByID,"吉堂Online","GT_BanPlayer",ServerIP,strLog);
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Picture_Stop_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
	}
	return false;
}

//清理头像图片
void CGTOper::GT_Picture_Clear(int userByID, char * ServerName, char * ServerIP,char * UserName)
{
	try
	{
		wstring Account =operGT.charToWChar(UserName);
		if(CONNECT_SERVERMANAGER_LOGIN(ServerIP,strLog))
		{
			CPacket req(ServiceKey::GMProtocol::GM_CONTENT_CLEAR);
			req<<Account<<(BYTE)1;
			msocket.SendNewData(req.GetPack(),req.GetPackLen());//发送清理违禁图片的请求
			
			CPacket packet;
			int recvLen = msocket.recvData();//获取清理违禁图片的回应
			memcpy(packet.GetPack(),msocket.recvBuffer(),recvLen);
			bool result = true;
			BYTE B_Result=0;
			packet>>Account;
			packet>>B_Result;
			packet>>result;
			
			
			if(!result)
			{
				sprintf(strLog,"<%s>,清理服务器<%s>中的玩家<%s>违禁图片失败",operName,ServerName,UserName);
				logFile.WriteText("GTown",strLog);//将失败信息写入日志中
				logFile.WriteDBLog(userByID,"吉堂Online","GT_USER_RESET",ServerIP,strLog);
			}
			else
			{
				sprintf(strLog,"<%s>,清理服务器<%s>中的玩家<%s>违禁图片成功",operName,ServerName,UserName);
				logFile.WriteText("GTown",strLog);//将成功信息写入日志中
				logFile.WriteDBLog(userByID,"吉堂Online","GT_USER_RESET",ServerIP,strLog);
			}
		}
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Picture_Clear_Resp,operGT.GetSendBuf(),operGT.GetSendLength());

		
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,清理服务器<%s>中的玩家<%s>违禁图片发生系统错误",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);
		logFile.WriteDBLog(userByID,"吉堂Online","GT_Picture_Clear",ServerName,strLog);
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_Picture_Clear_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
	}
}

//清空个性签名
void CGTOper::GT_ClearName_Query(int userByID, char * ServerName, char * ServerIP,char * UserName)
{
	try
	{

		wstring Account =operGT.charToWChar(UserName);
		if(CONNECT_SERVERMANAGER_LOGIN(ServerIP,strLog))
		{
			CPacket req(ServiceKey::GMProtocol::GM_CONTENT_CLEAR);
			req<<Account<<(BYTE)2;
			msocket.SendNewData(req.GetPack(),req.GetPackLen());//发送清理违禁图片的请求
			
			CPacket packet;
			int recvLen = msocket.recvData();//获取清理违禁图片的回应
			memcpy(packet.GetPack(),msocket.recvBuffer(),recvLen);
			bool result = true;
			BYTE B_Result=0;
			packet>>Account;
			packet>>B_Result;
			packet>>result;
			
			if(!result)
			{
				sprintf(strLog,"<%s>,清理服务器<%s>中的玩家<%s>违禁图片失败",operName,ServerName,UserName);
				logFile.WriteText("GTown",strLog);//将失败信息写入日志中
				logFile.WriteDBLog(userByID,"吉堂Online","GT_ClearName_Query",ServerIP,strLog);
				operGT.BuildTLV(strLog);
				operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_ClearName_Query_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
			}
			else
			{
				sprintf(strLog,"<%s>,清理服务器<%s>中的玩家<%s>违禁图片成功",operName,ServerName,UserName);
				logFile.WriteText("GTown",strLog);//将成功信息写入日志中
				logFile.WriteDBLog(userByID,"吉堂Online","GT_ClearName_Query",ServerIP,strLog);
				operGT.BuildTLV(strLog);
				operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_ClearName_Query_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
			}
		}
		
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,清理服务器<%s>中的玩家<%s>违禁图片发生系统错误",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);
		logFile.WriteDBLog(userByID,"吉堂Online","GT_ClearName_Query",ServerIP,strLog);
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_ClearName_Query_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
	}
}

//查询道具、宠物
void CGTOper::GM_ITEM_Check()
{
/*	try
	{
		//获取模糊查询的道具名称
		wstring tmpName = operGT.charToWChar((char*)(recvVect.getTLVByTag(CEnumCore::TagName::GT_ItemName,0)).lpdata);

		CPacket req(ServiceKey::GMProtocol::GM_ITEM_SELECT);
		req<<tmpName;//构造查询道具信息的数据包
		msocket.SendNewData(req.GetPack(),req.GetPackLen());//发送查询道具信息的请求

		CPacket packet(ServiceKey::GMProtocol::GM_ITEM_SELECT_ACK);
		int recvLen = msocket.recvData();//获取查询道具信息的回应
		memcpy(packet.GetPack(),msocket.recvBuffer(),recvLen);
		int pageCount=0;//保存查询得到的数据页数
		int mID=0;//查询得到的商品ID
		wstring itemName;//查询得到的商品名称
		int timesLimit=0;//商品使用的次数限制
		int daysLimit=0;//商品使用的日期限制
		char StrInt[10];//保存int类型的变量
		ZeroMemory(StrInt, 10);
		int count = 0;
 		packet>>count;//获取查询到的道具数量		
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
					
				_itoa(mID, StrInt, 10);//商品ID
				DBVect.AddTFLV(CEnumCore::TagName::GT_MID, CEnumCore::TagFormat::TLV_INTEGER, sizeof(int), (unsigned char*)&StrInt);
				DBVect.AddTFLV(CEnumCore::TagName::GT_ItemName, CEnumCore::TagFormat::TLV_STRING, itemName.length()*2, (unsigned char*)operGT.wcharToChar(itemName));
				
				_itoa(timesLimit, StrInt, 10);//次数限制
				DBVect.AddTFLV(CEnumCore::TagName::GT_TimesLimit, CEnumCore::TagFormat::TLV_INTEGER, sizeof(int), (unsigned char*)&StrInt);
				_itoa(daysLimit, StrInt, 10);//日期限制
				DBVect.AddTFLV(CEnumCore::TagName::GT_DaysLimit, CEnumCore::TagFormat::TLV_INTEGER, sizeof(int), (unsigned char*)&StrInt);
				_itoa(pageCount, StrInt, 10);//页数
				DBVect.AddTFLV(CEnumCore::TagName::PageCount, CEnumCore::TagFormat::TLV_INTEGER, sizeof(int), (unsigned char*)&StrInt);
			}
		}
		else
		{
			operGT.ReturnMessage(1, CEnumCore::Msg_Category::GT_OP_ADMIN, m_ServiceKey, "没有名称相近的道具信息");
			return;
		}
		operGT.ReturnData(1, CEnumCore::Msg_Category::GT_OP_ADMIN, m_ServiceKey, &DBVect, 1);//发送数据
	}
	catch (...)
	{
	}*/
}

//添加道具、宠物
//type – 0:一般物品;1:avatar;2:狗;3:马)\t服务器id
void CGTOper::GT_ITEM_ADD(int userByID,char * ServerName, char * ServerIP,char * UserName,char * szMailTitle, char * szMailContent,int count,char * itemName)
{	
	wstring title = operGT.charToWChar(szMailTitle);//添加道具的标题	
	wstring content = operGT.charToWChar(szMailContent);//添加道具的内容
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
			int mID = 0;//商品ID
			int itemNum = 0;//道具数量
			char strMID[20];
			char strItemNum[20];
			ZeroMemory(strMID, 20);//初始化临时保存商品ID的变量		
			ZeroMemory(strItemNum, 20);//初始化临时保存道具数量的变量
			
			int nNum = 1;//第一个字符的次数
			int mNum = 1;//第二个字符的次数
			
			CPacket req(ServiceKey::GMProtocol::GM_ITEM_ADD);
			req<<title<<content<<count;//添加了邮件标题、邮件内容和循环数量
			for(int i=0;i<count;i++)
			{
				ZeroMemory(strMID, 20);//初始化临时保存商品ID的变量		
				ZeroMemory(strItemNum, 20);//初始化临时保存道具数量的变量
				if(i == 0)//第1对（ItemID*ItemNum）
				{
					operGT.getSpecChar(itemName, "", mNum, "*", nNum, strMID);
					operGT.getSpecChar(itemName, "*", nNum, "|", mNum, strItemNum);
					nNum++;
				}
				else if(i == count-1)//最后1对(ItemID*ItemNum)
				{
					operGT.getSpecChar(itemName, "|", mNum, "*", nNum, strMID);
					operGT.getSpecChar(itemName, "*", nNum, "", mNum, strItemNum);
				}
				else//中间对(ItemID*ItemNum)
				{
					operGT.getSpecChar(itemName, "|", mNum, "*", nNum, strMID);
					operGT.getSpecChar(itemName, "*", nNum, "|", mNum+1, strItemNum);
					nNum++;
					mNum++;
				}
				mID=atoi(strMID);
				itemNum = atoi(strItemNum);
				/////////////记录log///////////////////
				sprintf(tmpMessage,"exec GT_ItemCodeToName %i",mID);
				operGT.QuerySingleValue("GT","127.0.0.1",4,tmpMessage,strItemName);
				sprintf(tmpMessage,"【道具ID】%i【道具名】%s:【道具数量】%i",mID,strItemName,itemNum);
				sprintf(retMessage,"%s%s",retMessage,tmpMessage);

				req<<Account<<mID<<itemNum;

			}
			msocket.SendNewData(req.GetPack(),req.GetPackLen());//发送添加道具的请求
			
			CPacket packet;
			int recvLen = msocket.recvData();//获取添加道具的回应
			memcpy(packet.GetPack(),msocket.recvBuffer(),recvLen);
			bool result = true;
			packet>>result;
			if(result)
			{
				sprintf(strLog,"【用户】%s,【服务器】%s,【给玩家】%s添加道具%s成功", operName, ServerName, UserName,retMessage);
			}
			else
			{
				sprintf(strLog,"【用户】%s,【服务器】%s,【给玩家】%s添加道具%s失败", operName, ServerName, UserName,retMessage);
			}
			logFile.WriteText("GTown",strLog);//将成功信息写入日志中
			logFile.WriteDBLog(userByID,"吉堂Online","GT_ITEM_ADD",ServerIP,strLog);
		}
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_ADD_ITEM_ACK,operGT.GetSendBuf(),operGT.GetSendLength());

	}
	catch(...)
	{
		sprintf(strLog,"<%s>,在服务器<%s>,给玩家<%s>添加道具发生系统错误", operName, ServerName, UserName);
		logFile.WriteText("GTown",strLog);
		logFile.WriteDBLog(userByID,"吉堂Online","GT_ITEM_ADD",ServerIP,strLog);
		operGT.BuildTLV(strLog);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_ADD_ITEM_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
	}
}
//批量添加道具
bool CGTOper::GT_Import_Item(int userByID,char * ServerName, char * ServerIP, char * MailTitle,
							 char * MailContent, char * ListUserName, char * ListItemID, char * ListItemNum)
{	
	vector <ITEMPACKET> v_item;
	char * pUserName= strtok(ListUserName,"|");
	ITEMPACKET itemPacket;
	while(pUserName!=NULL)///账号
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
		(*iter).mID=StrToInt(ItemID);//道具ID
		pItemID= strtok(NULL,"|");
		if(pItemID==NULL)
			break;
	}
	char ItemNum[10];
	char* pItemNum=strtok(ListItemNum,"|");
	for( iter = v_item.begin(); iter != v_item.end(); iter++ )
	{
		sprintf(ItemNum,"%s",pItemNum);
		(*iter).iNum=StrToInt(ItemNum);//道具数量
		pItemNum= strtok(NULL,"|");
		if(pItemNum==NULL)
			break;
	}
	wstring title = operGT.charToWChar(MailTitle);//添加道具的标题	
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
		for( iter = v_item.begin(); iter != v_item.end(); iter++ )//根据迭代器循环添加
		{
			itemPacket = *iter;
			CPacket req(ServiceKey::GMProtocol::GM_ITEM_ADD);
			int count=1;
			req<<title<<content<<count;//添加了邮件标题、邮件内容和循环数量
			Account=operGT.charToWChar(itemPacket.szAccount);
			req<<Account<<itemPacket.mID<<itemPacket.iNum;
			msocket.SendNewData(req.GetPack(),req.GetPackLen());//发送添加道具的请求
			
			CPacket packet;
			int recvLen = msocket.recvData();//获取添加道具的回应
			memcpy(packet.GetPack(),msocket.recvBuffer(),recvLen);
			packet>>result;
			sprintf(tmpMessage,"exec GT_ItemCodeToName %i",itemPacket.mID);//根据道具ID获得道具名
			operGT.QuerySingleValue("GT","127.0.0.1",4,tmpMessage,strItemName);

			if(result)
			{
				sprintf(tmpMessage,"道具:ID%i道具名%s:道具数量%i成功",itemPacket.mID,strItemName,itemPacket.iNum);
				sprintf(retMessage,"%s%s",retMessage,tmpMessage);
			}
			else
			{
				sprintf(tmpMessage,"道具ID%i道具数量%i失败",itemPacket.mID,itemPacket.iNum);
				sprintf(retMessage,"%s%s",retMessage,tmpMessage);
			}
			
		}
	/*	if(result)
		{
			sprintf(retMessage,"%s","批量添加成功");
		}
		else
		{
			sprintf(retMessage,"%s","批量添加失败");
		}*/
	}
	operGT.BuildTLV(retMessage);
	operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,
			CEnumCore::ServiceKey::GT_Import_Item_Resp,operGT.GetSendBuf(),operGT.GetSendLength());

	logFile.WriteDBLog(userByID,"吉堂Online","GT_Import_Item",ServerIP,retMessage);

/*	try
	{
		wstring title = operGT.charToWChar((char*)(recvVect.getTLVByTag(CEnumCore::TagName::GT_MailTitle,0)).lpdata);//添加道具的标题	
		wstring content = operGT.charToWChar((char*)(recvVect.getTLVByTag(CEnumCore::TagName::GT_MailContent,0)).lpdata);//添加道具的内容
		char itemcount[2048];
		ZeroMemory(itemcount,2048);//添加道具的详细信息
		sprintf(itemcount,"%s",(char*)(recvVect.getTLVByTag(CEnumCore::TagName::PageCount, 0)).lpdata);//获取添加道具的详细信息
		
		char itemName[2048];
		ZeroMemory(itemName,2048);//添加道具的详细信息
		sprintf(itemName,"%s",(char*)(recvVect.getTLVByTag(CEnumCore::TagName::GT_ItemName, 0)).lpdata);//获取添加道具的详细信息

		int mID = 0;//商品ID
		int itemNum = 0;//道具数量
		char strMID[20];
		char strItemNum[20];
		ZeroMemory(strMID, 20);//初始化临时保存商品ID的变量		
		ZeroMemory(strItemNum, 20);//初始化临时保存道具数量的变量
		
		int nNum = 1;//第一个字符的次数
		int mNum = 1;//第二个字符的次数
		
		CPacket req(ServiceKey::GMProtocol::GM_ITEM_ADD);
		req<<title<<content<<count;//添加了邮件标题、邮件内容和循环数量
		for(int i=0;i<count;i++)
		{
			ZeroMemory(strMID, 20);//初始化临时保存商品ID的变量		
			ZeroMemory(strItemNum, 20);//初始化临时保存道具数量的变量
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
		msocket.SendNewData(req.GetPack(),req.GetPackLen());//发送添加道具的请求
		
		CPacket packet;
		int recvLen = msocket.recvData();//获取添加道具的回应
		memcpy(packet.GetPack(),msocket.recvBuffer(),recvLen);
		bool result = true;
		packet>>result;
		
		else
		{
			sprintf(strLog,"<%s>,在服务器<%s>,给玩家<%s>添加道具成功", m_UserName, GT_ServerName, GT_Account);
			logFile.WriteText("GTown",strLog);//将成功信息写入日志中
			logFile.WriteDBLog(UserByID,"吉堂Online","GT_ITEM_ADD",GT_ServerIP,strLog);
			operGT.ReturnMessage(1, CEnumCore::Msg_Category::GT_OP_ADMIN, m_ServiceKey, "添加成功");
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
//根据帐号踢人
vector <CGlobalStruct::TFLV> CGTOper::KickPlayer(int userByID,char * ServerName, char * ServerIP,char * UserName)
{
	try
	{
		char retMessage[256];
		if(GT_KICK_USER(ServerIP,UserName,retMessage))
		{
			sprintf(retMessage,"%s","踢人成功");
			sprintf(strLog,"<%s>,强制服务器<%s> 玩家<%s>下线成功",operName,ServerIP,UserName);
			logFile.WriteText("GTown",strLog);
			logFile.WriteDBLog(userByID,"吉堂Online","GT_KICK_USER",ServerIP,strLog);
		}
		else
		{
			sprintf(strLog,"<%s>,强制服务器<%s> 玩家<%s>下线失败",operName,ServerIP,UserName);
			logFile.WriteText("GTown",strLog);//将失败信息写入日志中
			logFile.WriteDBLog(userByID,"吉堂Online","GT_KICK_USER",ServerIP,strLog);
		}
		operGT.BuildTLV(retMessage);
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_KICK_USER_ACK,operGT.GetSendBuf(),operGT.GetSendLength());
	}
	catch(...)
	{
	}
	return true;
}

//查询公告(特殊重载)
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

//更新公告(只有删除用boardMessage代替IP)
bool CGTOper::BroadTask_Update(int userByID,int taskid,char * boardMessage,char * begintime,char * endtime,int interval,int status)
{
	char retMessage[256];
	if(CONNECT_SERVERMANAGER_LOGIN(boardMessage,retMessage))
	{
		GT_BROADCAST_REMOVE(taskid);
		logFile.WriteDBLog(userByID,"吉堂Online","GT_BROADCAST_REMOVE",boardMessage,"删除公告成功");
		operGT.BuildTLV("删除公告成功");
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_BROADCAST_REMOVE_ACK,operGT.GetSendBuf(),operGT.GetSendLength());

	}
	
	
	return true;
}

//插入公告ServerIP代替ServerIPList,GSServerIP是否立即发送
bool CGTOper::BroadTask_Insert(int userByID,char * ServerIP,char * GSServerIP,char * boardMessage,char * begintime,char * endtime,int interval)
{
	int iResult=0;
	char retMessage[256];
	int IsImmediate=StrToInt(GSServerIP);//是否立即发送
	wstring context = operGT.charToWChar(boardMessage);//获取公告内容
	struct tm m_StartTime;///转换开始时间类型
	sscanf(begintime, "%d-%d-%d %d:%d:%d", &m_StartTime.tm_year,  &m_StartTime.tm_mon, &m_StartTime.tm_mday, &m_StartTime.tm_hour, &m_StartTime.tm_min, &m_StartTime.tm_sec);
	m_StartTime.tm_year = m_StartTime.tm_year - 1900;
	m_StartTime.tm_mon = m_StartTime.tm_mon - 1;
	size_t StartTime = mktime(&m_StartTime);///根据开始时间转换日期值
	struct tm m_EndTime;///转换结束时间类型
	sscanf(endtime, "%d-%d-%d %d:%d:%d", &m_EndTime.tm_year,  &m_EndTime.tm_mon, &m_EndTime.tm_mday, &m_EndTime.tm_hour, &m_EndTime.tm_min, &m_EndTime.tm_sec);
	m_EndTime.tm_year = m_EndTime.tm_year - 1900;
	m_EndTime.tm_mon = m_EndTime.tm_mon - 1;
	size_t	EndTime = mktime(&m_EndTime);
	char *lpServerIP=strtok(ServerIP,"|");//查询标志";"
	while(lpServerIP!=NULL)
	{
		
		if(CONNECT_SERVERMANAGER_LOGIN(ServerIP,retMessage))
		{
			if(GT_BROADCAST_ADD(IsImmediate,context,(DWORD)interval,StartTime,EndTime))
			{
				sprintf(strLog,"<%s>,在服务器<%s>添加公告,内容:<%s>,成功",operName,lpServerIP,boardMessage);
				logFile.WriteText("吉堂Online",strLog);//将成功信息写入日志中
				logFile.WriteDBLog(userByID,"吉堂Online","GT_BROADCAST_ADD",lpServerIP,strLog);
				iResult++;
			}
			else
			{
				sprintf(strLog,"<%s>,在服务器<%s>添加公告,内容:<%s>,失败",operName,lpServerIP,boardMessage);
				logFile.WriteText("GTown",strLog);//将失败信息写入日志中
			}
		}
		lpServerIP=strtok(NULL,"|");//查询标志";"

	}
	if(iResult==0)
	{
		operGT.BuildTLV("添加公告失败");
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_BROADCAST_ADD_ACK,operGT.GetSendBuf(),operGT.GetSendLength());

	}
	else
	{
		operGT.BuildTLV("添加公告成功");
		operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_OP_ADMIN,CEnumCore::ServiceKey::GT_BROADCAST_ADD_ACK,operGT.GetSendBuf(),operGT.GetSendLength());

	}	
	msocket.closeSocket();
	return true;
}

