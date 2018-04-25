// GMAPI.cpp: implementation of the CGMInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GMInfo.h"

//#include "ASTA.h"
//#include "ATAS.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGMInfo::CGMInfo()
{

}

CGMInfo::~CGMInfo()
{
}
//�ж��Ƿ��½�ɹ�
void CGMInfo::IsLogin()
{
	try
	{
		SOCKADDR_IN remoteaddr;//IP��ַ
		int iAddrSize=0;//IP��ַ�ĳ���
		int length=0;
		//���szMD5password
		ZeroMemory(szMD5Password,256);
		//md5����
		MD5_Password(szMD5Password,&length,(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::PassWord,0)).lpdata,
			strlen((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::PassWord,0)).lpdata));

		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GM","127.0.0.1",4,0,0,"exec IsLogin '%s','%s','%s'",
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::UserName,0)).lpdata,
			szMD5Password,
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::MAC,0)).lpdata);
		if(!DBTFLV.empty())
		{
			ZeroMemory(&remoteaddr,sizeof(SOCKADDR_IN));
			getpeername(pGameSess->GetSocket(),(SOCKADDR   *)&remoteaddr,&iAddrSize); 

			/*CLogic::AddLog(pGameSess->GetSocket(),
			StrToInt((char *)&(GetDBVect()->getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
			(char *)&(GetDBVect()->getTLVByTag(CEnumCore::TagName::UserName,0)).lpdata,
			(char *)&(GetDBVect()->getTLVByTag(CEnumCore::TagName::RealName,0)).lpdata,
			(char *)inet_ntoa(remoteaddr.sin_addr)); */
			CLogic::AddLog(pGameSess->GetSocket(),
				StrToInt((char *)&(COperVector(DBTFLV)).getTLVByTag(CEnumCore::TagName::UserByID,0).lpdata),
				(char *)&(COperVector(DBTFLV)).getTLVByTag(CEnumCore::TagName::UserName,0).lpdata,
				(char *)&(COperVector(DBTFLV)).getTLVByTag(CEnumCore::TagName::RealName,0).lpdata,
				(char *)inet_ntoa(remoteaddr.sin_addr));
			
			GameDBOper("GM","127.0.0.1",4,"exec UpdateMac %i,'%s'",
				StrToInt((char *)&((COperVector(DBTFLV)).getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
				(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::MAC,0)).lpdata);
			logFile.WriteLog("GM",(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::UserName,0)).lpdata,"��½�ɹ�");
			operGM.BuildTLV(COperVector(DBTFLV));
			
		}
		else
		{
			char passwd[40];
			char mac[40];
			int status=0;
			logFile.WriteLog("GM",(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::UserName,0)).lpdata,"��½ʧ��");//��LOG
			if(GetUserInfo((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::UserName,0)).lpdata,passwd,mac,&status))//�õ��û���Ϣ
			{
				if(strcmp(passwd,szMD5Password))//�Ƚ�����
				{
					sprintf(retMessage,"%s","�������\n");
				}
				else if(strcmp(mac,(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::MAC,0)).lpdata))//�Ƚ�MAC��
				{
					sprintf(retMessage,"%s","MAC�����\n");
				}
				else if(status==0)//�Ƚ�״̬
				{
					sprintf(retMessage,"%s","״̬����\n");
				}
				else//�������
				{
					sprintf(retMessage,"%s","��Ч�ڴ���\n");
				}

			}	
			else
			{
				sprintf(retMessage,"%s","�û�������\n");
			}
			operGM.BuildTLV(retMessage);
		}
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::CONNECT_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�û���¼�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�û���¼�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�û���¼�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch(...)
	{
		//��ӡ��SQL���	
		logFile.WriteErrorLog("GM","catch error");
		//������Ϣ
		operGM.BuildTLV("���Ӵ���");
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::CONNECT_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
	}

}
//��ѯģ����û���ϵ
void CGMInfo::User_Module_Query()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);
		logFile.WriteLog("GM",m_username,"��ѯģ����û���ϵ");


		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GM","127.0.0.1",4,0,0,"exec GMTOOLS_MODULE_QUERY %i",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::User_ID,0)).lpdata));
		if(!DBTFLV.empty())
		{
			operGM.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operGM.BuildTLV("û��ģ��");
		}
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::USER_MODULE_ADMIN,CEnumCore::ServiceKey::USER_MODULE_QUERY_RESP,operGM.GetSendBuf(),operGM.GetSendLength());

	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯģ����û���ϵ�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯģ����û���ϵ�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯģ����û���ϵ�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch(...)
	{
		operGM.BuildTLV("û��ģ��");
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::USER_MODULE_ADMIN,CEnumCore::ServiceKey::USER_MODULE_QUERY_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
	}

}
//����ģ��
void CGMInfo::User_Module_Update()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//ͨ��socket��username
		logFile.WriteLog("GM����",m_username,"����ģ��");//��LOG

		int	length=GameDBOper("GM","127.0.0.1",4,"exec  Gmtool_GmUserModule_Admin %i,'%s'",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::User_ID,0)).lpdata),
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::ModuleList,0)).lpdata);
		
		if(length==0)
		{
			//������Ϣ
			sprintf(retMessage,"%s","����ģ��ʧ��\n");
		}
		else
		{
			sprintf(retMessage,"%s","����ģ��ɹ�\n");
		}
		operGM.BuildTLV(retMessage);
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::USER_MODULE_ADMIN,CEnumCore::ServiceKey::USER_MODULE_UPDATE_RESP,operGM.GetSendBuf(),operGM.GetSendLength());

	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"����ģ���ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"����ģ���ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"����ģ�������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch(...)
	{
		//������Ϣ
		operGM.BuildTLV("����ģ��ʧ��");
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::USER_MODULE_ADMIN,CEnumCore::ServiceKey::USER_MODULE_UPDATE_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
	}

}
//��ѯ�û��б�
void CGMInfo::User_Query_All()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//ͨ��socket��username
		logFile.WriteLog("GM����",m_username,"��ѯ�û��б�");//��LOG


		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GM","127.0.0.1",4,
			StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
			StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata),
			"exec Gmtool_UserInfo_Query %i",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata));
		if(!DBTFLV.empty())
		{
			operGM.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operGM.BuildTLV("û���û�");
		}
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::USER_ADMIN,CEnumCore::ServiceKey::USER_QUERY_ALL_RESP,operGM.GetSendBuf(),operGM.GetSendLength());

		
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ�û��б��ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ�û��б��ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ�û��б������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch(...)
	{
		operGM.BuildTLV("û���û�");
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::USER_ADMIN,CEnumCore::ServiceKey::USER_QUERY_ALL_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
	}
}

//�鿴�ض���Ϸ������
void  CGMInfo::Server_IP_Query()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//ͨ��socket��username
		logFile.WriteLog("GM����",m_username,"�鿴������Ϸ������");//��LOG
		//����SQL���

		//////IDC///////////////////////////////
		/*	sprintf(sql,"exec ServerInfo_Query %i,%i,%i",
						StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
						StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::ServerInfo_GameID,0)).lpdata),
						StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::ServerInfo_GameDBID,0)).lpdata));
		
		sprintf(sql,"exec ServerInfo_Query %i,%i",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::ServerInfo_GameID,0)).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::ServerInfo_GameDBID,0)).lpdata));*/
	
		
		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GM","127.0.0.1",4,
			0,0,
			"exec ServerInfo_Query_New %i,%i",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::ServerInfo_GameID,0)).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::ServerInfo_GameDBID,0)).lpdata));
		if(!DBTFLV.empty())
		{
			operGM.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operGM.BuildTLV("û����Ϸ������");
		}
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::SERVERINFO_IP_QUERY_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�鿴�ض���Ϸ�������ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�鿴�ض���Ϸ�������ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�鿴�ض���Ϸ�����������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
}

//�鿴������Ϸ������
void CGMInfo::ServerInfo_IP_All_Query()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//ͨ��socket��username
		logFile.WriteLog("GM����",m_username,"�鿴������Ϸ������");//��LOG
		vector<CGlobalStruct::TFLV> DBTFLV;
		//����SQL���
		if(StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::GameID,0)).lpdata)==0)
		{
			DBTFLV=GameDBQuery("GM","127.0.0.1",4,0,0,"exec ServerInfo_Query_NewALL '1'");
		}
		else
		{
			DBTFLV=GameDBQuery("GM","127.0.0.1",4,0,0,"exec ServerInfo_Query_NewALL '%s'",
				(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::GameID,0)).lpdata);
		}

		if(!DBTFLV.empty())
		{
			operGM.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operGM.BuildTLV("û����Ϸ������");
		}
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::SERVERINFO_IP_ALL_QUERY_RESP,operGM.GetSendBuf(),operGM.GetSendLength());

	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ��Ϸ�������ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ��Ϸ�������ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ��Ϸ�����������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
}
//����GM�ʺŵĹ�������
void CGMInfo::Gmtools_UserMac_Update()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//ͨ��socket��username
		logFile.WriteLog("GM����",m_username,"����GM�ʺŵĹ�������");//��LOG

		int	length=GameDBOper("GM","127.0.0.1",4,"exec GMTOOLS_UpdateMAC %i,%i,'%s'",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::User_ID,0)).lpdata),
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::Limit,0)).lpdata);
		
		if(length==0)
		{
			//������Ϣ
			sprintf(retMessage,"%s","����GM�ʺŵĹ�������ʧ��");
		}
		else
		{
			sprintf(retMessage,"%s","����GM�ʺŵĹ������ڳɹ�");
		}
		operGM.BuildTLV(retMessage);
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::GMTOOLS_USERMAC_UPDATE_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
		
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"����GM�ʺŵĹ��������ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"����GM�ʺŵĹ��������ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"����GM�ʺŵĹ������������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
}

//�鿴���߲�����¼
void CGMInfo::Gmtools_OperateLog_Query()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//ͨ��socket��username
		logFile.WriteLog("GM����",m_username,"�鿴���߲�����¼");//��LOG

		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GM","127.0.0.1",4,
			StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
			StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata),
			"exec GmTools_OperatorLog_NewQuery %i,%i,'%s','%s','%s'",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::User_ID,0)).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::GameID,0)).lpdata),
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::ServerInfo_IP,0)).lpdata,
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::BeginTime,0)).lpdata,
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::EndTime,0)).lpdata);
		if(!DBTFLV.empty())
		{
			operGM.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operGM.BuildTLV("û�м�¼");
		}
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::GMTOOLS_OperateLog_Query_RESP,operGM.GetSendBuf(),operGM.GetSendLength());

		
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�鿴���߲�����¼�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�鿴���߲�����¼�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�鿴���߲�����¼�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
}

//��ѯ�����б�
void CGMInfo::Depart_Query()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//ͨ��socket��username
		logFile.WriteLog("GM����",m_username,"��ѯ�����б�");//��LOG

		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GM","127.0.0.1",4,
			0,0,
			"exec Depart_Query");
		if(!DBTFLV.empty())
		{
			operGM.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operGM.BuildTLV("û�в���");
		}
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::USER_ADMIN,CEnumCore::ServiceKey::DEPART_QUERY_RESP,operGM.GetSendBuf(),operGM.GetSendLength());

	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ�����б��ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ�����б��ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ�����б������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
}

//�����û�
void CGMInfo::User_Create()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//ͨ��socket��username
		logFile.WriteLog("GM����",m_username,"�����û�");//��LOG
		ZeroMemory(szMD5Password,256);
		int length=0;
		MD5_Password(szMD5Password,&length,(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::PassWord,0)).lpdata,
								strlen((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::PassWord,0)).lpdata));//��ȡMD5���ܺ�����


		length=GameDBOper("GM","127.0.0.1",4,
			"exec Gmtool_Gminfo_Add_New %i,'%s','%s',%i,'%s','%s',%i,%i",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::UserName,0)).lpdata,
			szMD5Password,
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::DepartID,0)).lpdata),
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::RealName,0)).lpdata,
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::Limit,0)).lpdata,
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::User_Status,0)).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::SysAdmin,0)).lpdata));
		
		if(length==0)
		{
			//������Ϣ
			sprintf(retMessage,"%s","�����û�ʧ��");
		}
		else
		{
			sprintf(retMessage,"%s","�����û��ɹ�");
		}
		operGM.BuildTLV(retMessage);
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::USER_ADMIN,CEnumCore::ServiceKey::USER_CREATE_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
		
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�����û��ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�����û��ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�����û������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	////////IDC/////////////////////////
	/*	sprintf(sql,"exec Gmtool_Gminfo_Add %i,'%s','%s',%i,'%s','%s',%i,%i,'%s'",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::UserName,0)).lpdata,
			szMD5Password,
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::DepartID,0)).lpdata),
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::RealName,0)).lpdata,
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::Limit,0)).lpdata,
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::User_Status,0)).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::SysAdmin,0)).lpdata),
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::ServerInfo_IP,0)).lpdata);//����SQL���*/

}


//�����û�
void CGMInfo::User_Update()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//ͨ��socket��username
		logFile.WriteLog("GM����",m_username,"�����û�");//��LOG

		int length=GameDBOper("GM","127.0.0.1",4,
			"exec Gmtool_Gminfo_Edit %i,%i,'%s',%i,'%s',%i,%i,%i",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::User_ID,0)).lpdata),
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::RealName,0)).lpdata,
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::DepartID,0)).lpdata),
			(char*)&(recvVect.getTLVByTag(CEnumCore::TagName::Limit,0).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::User_Status,0)).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::OnlineActive,0)).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::SysAdmin,0)).lpdata));
		
		if(length==0)
		{
			//������Ϣ
			sprintf(retMessage,"%s","�����û�ʧ��");
		}
		else
		{
			sprintf(retMessage,"%s","�����û��ɹ�");
		}
		operGM.BuildTLV(retMessage);
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::USER_ADMIN,CEnumCore::ServiceKey::USER_UPDATE_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�����û��ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�����û��ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�����û������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
}

//ɾ���û�
void CGMInfo::User_Delete()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//ͨ��socket��username
		logFile.WriteLog("GM����",m_username,"ɾ���û�");//��LOG

		int	length=GameDBOper("GM","127.0.0.1",4,
			"exec Gmtool_Gminfo_Del %i,%i",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::User_ID,0)).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata));
		
		if(length==0)
		{
			//������Ϣ
			sprintf(retMessage,"%s","ɾ���û�ʧ��");
		}
		else
		{
			sprintf(retMessage,"%s","ɾ���û��ɹ�");
		}
		operGM.BuildTLV(retMessage);
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::USER_ADMIN,CEnumCore::ServiceKey::USER_DELETE_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"ɾ���û��ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"ɾ���û��ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"ɾ���û������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
}

//�޸�����
void CGMInfo::User_Passwd_Modif()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//ͨ��socket��username
		logFile.WriteLog("GM����",m_username,"�޸�����");//��LOG
		ZeroMemory(szMD5Password,256);
		int length=0;
		MD5_Password(szMD5Password,&length,(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::PassWord,0)).lpdata,
									strlen((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::PassWord,0)).lpdata));
	
		
		
		
		length=GameDBOper("GM","127.0.0.1",4,
			"exec Gmtool_PASSWD_Edit %i,%i,'%s'",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::UserByID,0)).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::User_ID,0)).lpdata),
			szMD5Password);
		
		if(length==0)
		{
			//������Ϣ
			sprintf(retMessage,"%s","�޸�����ʧ��");
		}
		else
		{
			sprintf(retMessage,"%s","�޸�����ɹ�");
		}
		operGM.BuildTLV(retMessage);
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::USER_ADMIN,CEnumCore::ServiceKey::USER_PASSWD_MODIF_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
		
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�޸������ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�޸������ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�޸����������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
}

//��Ϸ��ѯ
void CGMInfo::Game_Query()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//ͨ��socket��username
		logFile.WriteLog("GM����",m_username,"��Ϸ��ѯ");//��LOG



		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GM","127.0.0.1",4,
			0,0,
			"exec  PAL_GAME_QUERY");
		if(!DBTFLV.empty())
		{
			operGM.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operGM.BuildTLV("��Ϸ��ѯʧ��");
		}
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::GAME_ADMIN,CEnumCore::ServiceKey::GAME_QUERY_RESP,operGM.GetSendBuf(),operGM.GetSendLength());

	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��Ϸ��ѯ�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��Ϸ��ѯ�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��Ϸ��ѯ�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
}


//��ѯ��Ϸ��ģ���б�
void CGMInfo::Game_Module_Query()
{

	try
	{
		CLogic::SocketToUserName(pGameSess->GetSocket(),m_username);//ͨ��socket��username
		logFile.WriteLog("GM����",m_username,"��ѯ��Ϸ��ģ���б�");//��LOG
		
		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GM","127.0.0.1",4,
			0,0,
			"exec PAL_GAME_MODULE_QUERY");
		if(!DBTFLV.empty())
		{
			operGM.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operGM.BuildTLV("û����Ϸģ��");
		}
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::GAME_ADMIN,CEnumCore::ServiceKey::GAME_MODULE_QUERY_RESP,operGM.GetSendBuf(),operGM.GetSendLength());

	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ��Ϸ��ģ���б��ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ��Ϸ��ģ���б��ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ��Ϸ��ģ���б������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}

}
//�ļ��Ƚ�
void CGMInfo::GM_FileCompare()
{
	try
	{
		//�Ƚ��ļ�
		FUpdate.CompareFile((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::UpdateFileName,0)).lpdata,
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::UpdateFilePath,0)).lpdata,pGameSess);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�ļ��Ƚ��ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);		
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�ļ��Ƚ��ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�ļ��Ƚ������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
}
//�ļ�����
void CGMInfo::GM_FileUpdate()
{
	try
	{
		//�����ļ�
		FUpdate.UpdateFile((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::UpdateFileName,0)).lpdata,
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::UpdateFilePath,0)).lpdata,
	    StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::UpdateFileSize,0)).lpdata),pGameSess);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�ļ������ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�ļ������ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�ļ����������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	

}

//��ȡUserName,PASSWORD,User_State,MAC,
bool CGMInfo::GetUserInfo(char * username,char * passwd,char * mac,int *user_state)
{
	try
	{

		
		char sql[256];
		ZeroMemory(sql,256);
		sprintf(sql, "exec GetUser '%s'",username);//����SQL
		
		CSqlHelper m_SqlHelper;
		
		vector<CGlobalStruct::DataRecord> datarecord;//�������ݶ���
		
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		m_SqlHelper.SqlHelperMain("GM",&datarecord,"127.0.0.1",sql,4,0,0);
		
		if(datarecord.empty())//�����¼Ϊ��
		{
			return false;
		}
		
		//��ȡ��ѯ�ļ�¼����ÿһ��
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("user_pwd", m_DataRecord.recordfieldname))//��ȡ��Ӧ��LogDBIP
			{
				sprintf(passwd,"%s",m_DataRecord.recorddata);
			}
			else if(!strcmp("user_mac", m_DataRecord.recordfieldname))
			{
				sprintf(mac,"%s",m_DataRecord.recorddata);
			}
			else if(!strcmp("user_status", m_DataRecord.recordfieldname))
			{
				*user_state=StrToInt(m_DataRecord.recorddata);
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡ��¼�û���Ϣ�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡ��¼�û���Ϣ�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡ��¼�û���Ϣ�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
		return FALSE;
	}
	return true;
}

//Added by tengjie 2010-05-05:��ѯ��ҽ�����Ϣ
void CGMInfo::Gm_HarmFulProc_Query()
{
	try
	{
		CString path = "";
		CString filepath = "";
		CString lpIP = "";
		CSqlHelper m_SqlHelper;
		char sql[256];
		ZeroMemory(sql,256);
		GetCurrentDirectory(256,path.GetBuffer(0));//��ȡ��ǰĿ¼
		sprintf(filepath.GetBuffer(0),"%s%s",path.GetBuffer(0),"\\config\\scheme.ini");//��ȡscheme.ini�ļ�·��
		GetPrivateProfileString("SERVER","OutAddress","127.0.0.1",lpIP.GetBuffer(0),40,filepath.GetBuffer(0));

		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GM","127.0.0.1",4,
			StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::Index,0)).lpdata),
			StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata),
			"exec Game_ToolsAnalysis %i,%i,'%s','%s','%s'",
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::GameID,0)).lpdata),
			StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::ToolProcTypeID,0)).lpdata),
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::ServerInfo_IP,0)).lpdata,
			(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::ServerInfo_City,0)).lpdata,
			lpIP.GetBuffer(0));

		if(!DBTFLV.empty())
		{
			operGM.BuildTLV(COperVector(DBTFLV));
		}
		else
		{
			operGM.BuildTLV("����ҽ���");
		}

		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::HARMFUL_PROC_QUERY_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ��ҽ�����Ϣ�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ��ҽ�����Ϣ�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ��ҽ�����Ϣ�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GM",errMessage);
	}
	catch(...)
	{
		operGM.BuildTLV("����ҽ���");
		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::HARMFUL_PROC_QUERY_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
	}
}


// void CGMInfo::Gm_HarmFulProc_Query()
// {
// 	try
// 	{
// 		CString path = "";
// 		CString filepath = "";
// 		CString lpIP = "";
// 		CString strVarValue ="";
// 		CString strTempValue ="";
// 		CSqlHelper m_SqlHelper;
// 		vector<string> strVect;
// 		char sql[256];
// 		string strtext = "";
// 		ZeroMemory(sql,256);
// 		GetCurrentDirectory(256,path.GetBuffer(0));//��ȡ��ǰĿ¼
// 		sprintf(filepath.GetBuffer(0),"%s%s",path.GetBuffer(0),"\\config\\scheme.ini");//��ȡscheme.ini�ļ�·��
// 		GetPrivateProfileString("SERVER","OutAddress","127.0.0.1",lpIP.GetBuffer(0),40,filepath.GetBuffer(0));
// 
// 		CLogic::SocketToClientIP(pGameSess->GetSocket(),m_ClientIP);//ͨ��socket��clientIP
// 		logFile.WriteLog("GM����",m_ClientIP,"��ѯ��ҽ�����Ϣ");//��LOG
// 
// 		//sql.Format("%s","exec Game_GetVarValue '%s'",lpIP.GetBuffer(0));//����SQL	
// 		sprintf(sql,"exec Game_GetVarValue '%s'",lpIP.GetBuffer(0));//����SQL	
// 		vector<CGlobalStruct::DataRecord> datarecord;//�������ݶ���
// 		
// 		if(!datarecord.empty())
// 		{
// 			datarecord.clear();
// 		}
// 		m_SqlHelper.SqlHelperMain("GM",&datarecord,"127.0.0.1",sql,4,0,0);
// 		
// 		if(datarecord.empty())//�����¼Ϊ��
// 		{
// 			logFile.WriteErrorLog("GM����","�����Ϣ��¼Ϊ��");//��LOG
// 			return;
// 		}
// 
// 		if(!strVect.empty())
// 			strVect.clear();
// 		
// 		//��ȡ�ؼ��ּ�¼��
// 		vector<CGlobalStruct::DataRecord>::iterator iter;
// 		vector<CGlobalStruct::TFLV> TempDBTFLV;
// 		CGlobalStruct::DataRecord  m_DataRecord;
// 		vector<string>::iterator pStrData = NULL;
// 		int iNum = 0,iDataSize,iMod,iLeave = 0;
// 
// 		if(!TempDBTFLV.empty())
// 			TempDBTFLV.clear();
// 
// 		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
// 		{
// 			m_DataRecord = *iter;
// 
// 			if(!strcmp("VAR", m_DataRecord.recordfieldname))
// 			{
// 				strVect.push_back(m_DataRecord.recorddata);
// 			}
// 		}
// 
// 		iDataSize = strVect.size()/10;
// 		iMod	  = strVect.size()%10;
// 
// 
// 		for( pStrData = strVect.begin(); pStrData != strVect.end(); pStrData++ )
// 		{
// 			strtext = *pStrData;
// 
// 			iNum++;
// 
// 			if(iNum == 1)
// 				strVarValue.Format("%s","[");
// 
// 			if(iNum == iDataSize && iMod != strVect.size() - iLeave*iDataSize)
// 			{
// 				iLeave++;
// 				strTempValue.Format("%s]",strtext.c_str());
// 				strcat(strVarValue.GetBuffer(0),strTempValue.GetBuffer(0));
// 				iNum = 0;
// 				vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GM","127.0.0.1",4,
// 					StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::Index + iLeave - 1,0)).lpdata),
// 					StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata),
// 					"exec Game_ToolsAnalysis %i,%i,'%s','%s','%s'",
// 					StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::GameID,0)).lpdata),
// 					StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::ToolProcTypeID,0)).lpdata),
// 					(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::ServerInfo_IP,0)).lpdata,
// 					lpIP.GetBuffer(0),strVarValue.GetBuffer(0));
// 
// 				strVarValue = "";
// 
// 				if(!DBTFLV.empty())
// 					CreateNewDBTFLV(TempDBTFLV,DBTFLV);
// 			}
// 			else if(iNum == strVect.size() - iLeave*iDataSize)
// 			{
// 				strTempValue.Format("%s]",strtext.c_str());
// 				strcat(strVarValue.GetBuffer(0),strTempValue.GetBuffer(0));
// 				vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("GM","127.0.0.1",4,
// 					StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::Index + iLeave - 1,0)).lpdata),
// 					StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata),
// 					"exec Game_ToolsAnalysis %i,%i,'%s','%s','%s'",
// 					StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::GameID,0)).lpdata),
// 					StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::ToolProcTypeID,0)).lpdata),
// 					(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::ServerInfo_IP,0)).lpdata,
// 					lpIP.GetBuffer(0), strVarValue.GetBuffer(0));
// 				
// 				strVarValue = "";
// 
// 				if(!DBTFLV.empty())
// 					CreateNewDBTFLV(TempDBTFLV,DBTFLV);
// 			}
// 			else
// 			{
// 				strTempValue.Format("%s,",strtext.c_str());
// 				strcat(strVarValue.GetBuffer(0),strTempValue.GetBuffer(0));
// 			}
// 		}
// 		
// 		if(!TempDBTFLV.empty())
// 		{
// 			operGM.BuildTLV(COperVector(TempDBTFLV));
// 		}
// 		else
// 		{
// 			operGM.BuildTLV("����ҽ���");
// 		}
// 
// 		if(!datarecord.empty())
// 		{
// 			datarecord.clear();
// 		}
// 
// 		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::HARMFUL_PROC_QUERY_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
// 	}
// 	catch (CMemoryException* e)
// 	{
// 		e->GetErrorMessage(errMessage,256);
// 		sprintf(errMessage,"��ѯ��ҽ�����Ϣ�ڴ治��,������Ϣ:%s",errMessage);
// 		logFile.WriteErrorLog("GM",errMessage);
// 	}
// 	catch (CFileException* e)
// 	{
// 		e->GetErrorMessage(errMessage,256);
// 		sprintf(errMessage,"��ѯ��ҽ�����Ϣ�ļ��쳣,������Ϣ:%s",errMessage);
// 		logFile.WriteErrorLog("GM",errMessage);
// 	}
// 	catch (CException* e)
// 	{
// 		e->GetErrorMessage(errMessage,256);
// 		sprintf(errMessage,"��ѯ��ҽ�����Ϣ�����쳣,������Ϣ:%s",errMessage);
// 		logFile.WriteErrorLog("GM",errMessage);
// 	}
// 	catch(...)
// 	{
// 		operGM.BuildTLV("����ҽ���");
// 		operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::USER_ADMIN,CEnumCore::ServiceKey::USER_QUERY_ALL_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
// 	}
// }
// 
// void CGMInfo::CreateNewDBTFLV(vector<CGlobalStruct::TFLV> &NewDBTFLV, vector<CGlobalStruct::TFLV> DBTFLV)
// {
// 	if(DBTFLV.empty())
// 		return;
// 
// 	vector<CGlobalStruct::TFLV>::iterator pDbTflv = NULL;
// 	CGlobalStruct::TFLV szTflv;
// 
// 	for(pDbTflv = DBTFLV.begin(); pDbTflv != DBTFLV.end(); pDbTflv++)
// 	{
// 		szTflv = *pDbTflv;
// 
// 		NewDBTFLV.push_back(szTflv);
// 	}
// 
// }

//��ѯ�ʺ��Ƿ�Ϊvip�û�
void CGMInfo::GM_Net_vipAccountInfo(char * account)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV tflv;
	vector<string> strResult;
	char strConnect[512];
	ZeroMemory(strConnect, 512);
	
	try
	{
		printf("account:%s\n",account);
		sprintf(strConnect, "http://61.152.150.158/PayCenter/check_vip_rule.php");
		if(!operGM.getWebInfo(strConnect, account, strResult))
		{
			operGM.BuildTLV("CGMInfo::GM_Net_vipAccountInfo----���Ӵ���");
			return;
		}
		else
		{
			int i;
			int isVip = FALSE;
			//char patOK[20] = "RESULT_00";
			string patOK = "RESULT_00";

			string tmp = operGM.getXMLNodeAttribute(strResult,"status");
			if(patOK == tmp)
				isVip = TRUE;
			printf("%s is vip account? %d\n", account, isVip);
			CGlobalStruct::TFLV tflv;
			
			tflv.nIndex = DBTFLV.size() + 1;
			tflv.m_tagName = CEnumCore::TagName::PlayerAccount;
			tflv.m_tagFormat = CEnumCore::TagFormat::TLV_STRING;
			tflv.m_tvlength = strlen(account);
			sprintf((char *)&tflv.lpdata,"%s", account);
			DBTFLV.push_back(tflv);
			
			tflv.nIndex = DBTFLV.size() + 1;
			tflv.m_tagName = CEnumCore::TagName::GM_AccountInfo_Vip;
			tflv.m_tagFormat = CEnumCore::TagFormat::TLV_INTEGER;
			tflv.m_tvlength = sizeof(int);
			sprintf((char *)&tflv.lpdata, "%d", isVip);
			DBTFLV.push_back(tflv);
			
			if(!DBTFLV.empty())
				operGM.BuildTLV(DBTFLV);

			operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::GM_AccountInfo_Vip_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
	
			//DBTFLV = operGM.ReturnOpMsg(operName, "��ѯ�ɹ�", "vip�û���ѯ");
			return;
		}
		
	}
	catch (...)
	{
		//return operGM.ReturnOpMsg(operName, "�쳣", "vip�û���ѯ");
		return;
	}
	
	//return DBTFLV = operGM.ReturnOpMsg(operName, "��ѯʧ��", "vip�û���ѯ");
	return;
}

//����
/*
BOOL CGMInfo::GM_Net_KickPlayer(int userByID,char * ServerIP, char * UserName, char * Character)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	char retMessage[255];
	memset(retMessage, 0, 255);
	BOOL ret;

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
		
		
		if (!Connect_Remote_Login(ServerIP))
		{
			//DBTFLV = operGM.BuildTLV("����ҽ���");;
			logFile.WriteLog("GM",userByID, "[GM����][��¼������ʧ��]");
			ret = FALSE;
			//return;
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
				//DBTFLV = operGM.BuildTLV("[GM����]--[��������ʧ��]");
				//return DBTFLV;
				logFile.WriteLog("GM",userByID, "[GM����][��������ʧ��]");
				ret = FALSE;
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
						//DBTFLV = operGM.BuildTLV("[GM����][���˳ɹ�]");
						logFile.WriteLog("GM",userByID, "[GM����][���˳ɹ�]");
						ret = TRUE;
						break;
					case REP_KICK_USER::E_FAILED_CHAR_NOTINGAME:
						//DBTFLV = operGM.BuildTLV("[GM����][��ɫ������]");
						logFile.WriteLog("GM",userByID, "[GM����][��ɫ������]");
						ret = TRUE;
						break;
					default:
						//DBTFLV = operGM.BuildTLV("[GM����]--[����ʧ��]");
						logFile.WriteLog("GM", userByID, "[GM����][����ʧ��]");
						ret = FALSE;
						break;
					}
				}
				else
				{
					//DBTFLV = operGM.BuildTLV("[GM����]--[���˳�ʱ]");
					logFile.WriteLog("GM", userByID, "[GM����][���˳�ʱ]");
				}
			}
			
			m_MySocket.Close();
		}
		
		delete []sendbuf;
		sendbuf = NULL;
		delete []recvbuf;
		recvbuf = NULL;
	}
	catch(...)
	{
		//DBTFLV = operGM.BuildTLV("[GM����]--[δ֪����]");
		logFile.WriteLog("GM", userByID, "[GM����][δ֪�쳣]");
		ret = FALSE;
		return ret;
	}	
	//m_OperatorWA.GmOperLog(userByID, ServerIP, "GM����", retMessage, DBTFLV[0]);
	return ret;
}


//������ͣ(��ʱֻ֧����ʿ��˫)
void CGMInfo::GM_Net_ClosePlayerGroup(int userByID,char * ServerIP, char * UserName,int CharacterID,char * Character,int iTime, int Reason, char *description)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	char retMessage[255];
	memset(retMessage, 0, 255);

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

		
		if (!GM_Net_KickPlayer(userByID, ServerIP, UserName, Character))
		{
			DBTFLV = operGM.ReturnOpMsg(operName, "�ý�ɫ�����߻����쳣", "�ʺŷ�ͣ");
		}


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
			DBTFLV = operGM.ReturnOpMsg(operName, "��¼��Ϸ������ʧ��", "�ʺŷ�ͣ");
		}
		else
		{
			sprintf(retMessage, "�ʺŷ�ͣ��<����>%s, <�ʺ�>%s, <ʱ��>%d��, <ԭ��>%d, <����>%s", ServerIP, UserName, iTime, Reason, description);			
			logFile.WriteLog("GM",operName, retMessage);//��LOG

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
						GM_SqlForCloseUserInfo(UserName,ServerIP,iTime,description);
						//Added by tengjie 2010-07-29:�ʻ����ļ�¼��־�ӿ�
						New_Time.Format("%d", iTime/60);
						UserLogInterface(ServerIP,UserName,New_Time.GetBuffer(0),38,"1");
						break;
					case REP_CLOSE_ACCOUNT::E_FAILED_ACCOUNT_NOTEXIST:	
						DBTFLV = operGM.ReturnOpMsg(operName, "�ʺŲ�����", "�ʺŷ�ͣ");
						break;
					default:
						DBTFLV = operGM.ReturnOpMsg(operName, "��ͣʧ��", "�ʺŷ�ͣ");
						break;
					}
				}
				else
				{
					DBTFLV = operGM.ReturnOpMsg(operName, "�ʺŷ�ͣ��ʱ", "�ʺŷ�ͣ");
				}
			}

			m_MySocket.Close();
		}
	
		delete []sendbuf;
		sendbuf = NULL;
		delete []recvbuf;
		recvbuf = NULL;
	}
	catch (...)
	{
		DBTFLV = operGM.ReturnOpMsg(operName, "δ֪����", "�ʺŷ�ͣ");
	}

	operGM.GmOperLog(userByID, ServerIP, "�ʺŷ�ͣ", retMessage, DBTFLV[0]);

	if(!DBTFLV.empty())
		operGM.BuildTLV(DBTFLV);
	
	operGM.SendBuf(pGameSess,CEnumCore::Msg_Category::COMMON,CEnumCore::ServiceKey::GM_ClosePlayerGroup_RESP,operGM.GetSendBuf(),operGM.GetSendLength());
	
}
*/

//���ؼ�¼��ͣԭ��,������ʾ���û�(��ʱֻ֧����ʿ��˫)
void CGMInfo::GM_SqlForCloseUserInfo(char *UserName,char *ServerIP,int iTime,char *description,int iStatus)
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