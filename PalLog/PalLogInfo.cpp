#include "stdafx.h"
#include "time.h"
#include "PalLogInfo.h"
//���캯��
CPalLogInfo::CPalLogInfo()
{
	ZeroMemory(strLog,2048);//��ʼ����־��Ϣ
	ZeroMemory(operRemoteSql, 2048);//��ʼ����Զ�����ݿ�ִ�в�����sql���
	ZeroMemory(logdbip, 20);//��ʼ��������IP
}

//��������
CPalLogInfo::~CPalLogInfo()
{
}

//��ѯ�̳ǹ����¼
bool CPalLogInfo::Shop_Buy_Query(char * ServerName,char * ServerIP,char * RoleName,int iMallType,
								 char * szBegintime, char * szEndtime,int iIndex, int iPageSize)
{
	try
	{
		int recordnum=0;
		int PageNum=0;
		sprintf(strLog,"<%s>,��ѯ����<%s>���<%s>�̳ǹ����¼",operName,ServerName,RoleName);
		logFile.WriteText("�ɽ�OL",strLog);
		vector <CGlobalStruct::TFLV> DBTFLV;
	    //����servername��ѯlogdbid
		operAssist.FindLogdbIP(logdbip,ServerName);

		//��ȡԶ�̲�����Sql���
		switch(iMallType)
		{
			case 0:
				{
					///�鵽��¼��
					sprintf(operszSql,"select count(*) as countnum from logdb.item_itemmall_buy where (rolename='%s' or fromname='%s') and time>='%s'  and time<='%s'",
									RoleName,RoleName,szBegintime,szEndtime);
					recordnum=operAssist.FindRecordNum(logdbip,operszSql,1);
					if(recordnum==0)
					{
						operAssist.BuildTLV("��ѯ���ݿ���Ϊ��");
						operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ITEMMAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
						return false;

					}
					if(recordnum%iPageSize==0)
					{
						PageNum=recordnum/iPageSize;
					}
					else
					{
						PageNum=recordnum/iPageSize+1;
					}
					
					operAssist.getRemoteSql("PAL","PALLOG_ITEMMALL_ALL",operRemoteSql,4);

					if(!strcmp("",operRemoteSql))
					{
						operAssist.BuildTLV("���ݿ����Ҳ�����Ӧ��sql���");
						operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ITEMMAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
						return false;
					}
					DBTFLV=GameDBQuery("PAL",logdbip,1, 0, 0,operRemoteSql,RoleName,RoleName,szBegintime,szEndtime,(iIndex-1)*iPageSize,iPageSize);
					//DBTFLV=GameDBQuery("PAL",logdbip,1, iIndex, iPageSize,operRemoteSql,RoleName,RoleName,szBegintime,szEndtime,(iIndex-1)*iPageSize,iPageSize);
				}
				break;
			case 1:
				{
				}
				break;
			case 2:
				{
				}
				break;
		}
		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV),CEnumCore::ServiceKey::PALLOG_ITEMMAIL_RESP,PageNum);
		}
		else
		{
			operAssist.BuildTLV("���ݿ�Ϊ��");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ITEMMAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�̳ǹ����¼�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ITEMMAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�̳ǹ����¼�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ITEMMAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�̳ǹ����¼�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ITEMMAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>���<%s>�̳ǹ����¼����ϵͳ����",operName,ServerName,RoleName);
		logFile.WriteText("�ɽ�OL",strLog);//��������Ϣд����־��
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ITEMMAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	return true;
}

//��ѯ������Ϣ
bool CPalLogInfo::MailBox_Info_Query(char * ServerName,char * ServerIP,char * RoleName,int RoleID, char * szBegintime, char * szEndtime,int iIndex, int iPageSize)
{
	try
	{
		int tmpLength=0;
		sprintf(strLog,"<%s>,��ѯ����<%s>���<%s>������Ϣ",operName,ServerName,RoleName);
		logFile.WriteText("�ɽ�OL",strLog);

		//����servername��ѯlogdbid
		operAssist.FindGameDBIP(logdbip,ServerName);

		char begintime[20];
		char endtime[20];
		ZeroMemory(begintime,20);//��ʼ����ʼʱ��
		ZeroMemory(endtime,20);//��ʼ������ʱ��

		struct tm t_BeginTime; //	tm�ṹ�����͵ı���������ʱ��					
		sscanf(szBegintime,"%d-%d-%d %d:%d:%d",&t_BeginTime.tm_year,&t_BeginTime.tm_mon,&t_BeginTime.tm_mday,&t_BeginTime.tm_hour,&t_BeginTime.tm_min,&t_BeginTime.tm_sec);//����ʼʱ�丳ֵ��tm�ṹ��
		t_BeginTime.tm_year = t_BeginTime.tm_year - 1900;
		t_BeginTime.tm_mon = t_BeginTime.tm_mon - 1;
		time_t BeginTime;//time_t���͵ı���������ʱ���������ʾ
		BeginTime =mktime(&t_BeginTime); //��ȡ��ͣʱ�䵽1970-1-1֮���ʱ���������ʾ
		
		struct tm t_EndTime; //	tm�ṹ�����͵ı������������ʱ��
		sscanf(szEndtime,"%d-%d-%d %d:%d:%d",&t_EndTime.tm_year,&t_EndTime.tm_mon,&t_EndTime.tm_mday,&t_EndTime.tm_hour,&t_EndTime.tm_min,&t_EndTime.tm_sec);//������ʱ�丳ֵ��tm�ṹ��
		t_EndTime.tm_year = t_EndTime.tm_year - 1900;
		t_EndTime.tm_mon = t_EndTime.tm_mon - 1;
		time_t EndTime;//time_t���͵ı���������ʱ���������ʾ
		EndTime=mktime(&t_EndTime); //��ȡ��ͣʱ�䵽1970-1-1֮���ʱ���������ʾ
		
		sprintf(begintime,"%ld",BeginTime);//��ȡ��ʼʱ��
		sprintf(endtime,"%ld",EndTime);//��ȡ����ʱ��
	
		operAssist.getRemoteSql("PAL","PALLOG_MAILBOX_INFO",operRemoteSql,4);
		if(!strcmp("",operRemoteSql))
		{
			operAssist.BuildTLV("���ݿ����Ҳ�����Ӧ��sql���");
			operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CHECKMAILBOX_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
			return false;
		}
		vector <CGlobalStruct::TFLV> DBTFLV=GameDBQuery("PAL",logdbip,1,0,0,operRemoteSql,RoleID%20,RoleID,begintime,endtime);
		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV),CEnumCore::ServiceKey::PAL_CHECKMAILBOX_QUERY_RESP);
		}
		else
		{
			operAssist.BuildTLV("���ݿ�Ϊ��");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CHECKMAILBOX_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ������Ϣ�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CHECKMAILBOX_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ������Ϣ�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CHECKMAILBOX_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ������Ϣ�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CHECKMAILBOX_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>���<%s>������Ϣ����ϵͳ����",operName,ServerName,RoleName);
		logFile.WriteText("�ɽ�OL",strLog);//��������Ϣд����־��
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CHECKMAILBOX_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	return true;
}

//��ѯ�����ʼ��ĸ���
bool CPalLogInfo::MailBox_Info_Detail(char * ServerName,char * ServerIP,char * RoleName,int MailID, char * szBegintime, char * szEndtime,int iIndex, int iPageSize)
{
	try
	{
		int tmpLength=0;
		sprintf(strLog,"<%s>,��ѯ����<%s>���<%s>������Ϣ",operName,ServerName,RoleName);
		logFile.WriteText("�ɽ�OL",strLog);
		
		//����servername��ѯlogdbid
		operAssist.FindGameDBIP(logdbip,ServerName);
		
		char begintime[20];
		char endtime[20];
		ZeroMemory(begintime,20);//��ʼ����ʼʱ��
		ZeroMemory(endtime,20);//��ʼ������ʱ��
		
		struct tm t_BeginTime; //	tm�ṹ�����͵ı���������ʱ��					
		sscanf(szBegintime,"%d-%d-%d %d:%d:%d",&t_BeginTime.tm_year,&t_BeginTime.tm_mon,&t_BeginTime.tm_mday,&t_BeginTime.tm_hour,&t_BeginTime.tm_min,&t_BeginTime.tm_sec);//����ʼʱ�丳ֵ��tm�ṹ��
		t_BeginTime.tm_year = t_BeginTime.tm_year - 1900;
		t_BeginTime.tm_mon = t_BeginTime.tm_mon - 1;
		time_t BeginTime;//time_t���͵ı���������ʱ���������ʾ
		BeginTime =mktime(&t_BeginTime); //��ȡ��ͣʱ�䵽1970-1-1֮���ʱ���������ʾ
		
		struct tm t_EndTime; //	tm�ṹ�����͵ı������������ʱ��
		sscanf(szEndtime,"%d-%d-%d %d:%d:%d",&t_EndTime.tm_year,&t_EndTime.tm_mon,&t_EndTime.tm_mday,&t_EndTime.tm_hour,&t_EndTime.tm_min,&t_EndTime.tm_sec);//������ʱ�丳ֵ��tm�ṹ��
		t_EndTime.tm_year = t_EndTime.tm_year - 1900;
		t_EndTime.tm_mon = t_EndTime.tm_mon - 1;
		time_t EndTime;//time_t���͵ı���������ʱ���������ʾ
		EndTime=mktime(&t_EndTime); //��ȡ��ͣʱ�䵽1970-1-1֮���ʱ���������ʾ
		
		sprintf(begintime,"%ld",BeginTime);//��ȡ��ʼʱ��
		sprintf(endtime,"%ld",EndTime);//��ȡ����ʱ��

		
		operAssist.getRemoteSql("PAL","PALLOG_MAILBOX_DETAIL",operRemoteSql,4);
		if(!strcmp("",operRemoteSql))
		{
			operAssist.BuildTLV("���ݿ����Ҳ�����Ӧ��sql���");
			operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CHECKMAILBOX_DETAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
			return false;
		}
		vector <CGlobalStruct::TFLV> DBTFLV=GameDBQuery("PAL",logdbip,1,0,0,operRemoteSql,MailID%20,MailID,begintime,endtime);
		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV),CEnumCore::ServiceKey::PAL_CHECKMAILBOX_DETAIL_RESP);
		}
		else
		{
			operAssist.BuildTLV("���ݿ�Ϊ��");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CHECKMAILBOX_DETAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ�����ʼ��ĸ����ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CHECKMAILBOX_DETAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ�����ʼ��ĸ����ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CHECKMAILBOX_DETAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ�����ʼ��ĸ��������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CHECKMAILBOX_DETAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>���<%s>������Ϣ����ϵͳ����",operName,ServerName,RoleName);
		logFile.WriteText("�ɽ�OL",strLog);//��������Ϣд����־��
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CHECKMAILBOX_DETAIL_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	return true;
}
//��ѯ������Ϣ
bool CPalLogInfo::Auction_Info_Query(char * ServerName,char * ServerIP,char * RoleName,int RoleID, char * szBegintime, char * szEndtime,int iIndex, int iPageSize)
{
	try
	{
		COperVector tmpOperVector;
		int tmpOperVectorlength=0;
		int tmpLength=0;
		sprintf(strLog,"<%s>,��ѯ����<%s>���<%s>������Ϣ",operName,ServerName,RoleName);
		logFile.WriteText("�ɽ�OL",strLog);
		
		//����servername��ѯlogdbid
		operAssist.FindGameDBIP(logdbip,ServerName);

		operAssist.getRemoteSql("PAL","PALLOG_AUCTION_INFO",operRemoteSql,4);//�������IP��ѯ������Ϣ��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.BuildTLV("���ݿ����Ҳ�����Ӧ��sql���");
			operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_AUCTION_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
			return false;
		}


		vector <CGlobalStruct::TFLV> DBTFLV=GameDBQuery("PAL",logdbip,1,iIndex,iPageSize,operRemoteSql,
			RoleID,RoleID,RoleID,RoleID,RoleID
			,RoleID,RoleID,RoleID,RoleID,RoleID
			,RoleID,RoleID,RoleID,RoleID,RoleID
			,RoleID,RoleID,RoleID,RoleID,RoleID
			,RoleID,RoleID,RoleID,RoleID,RoleID);
		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV),CEnumCore::ServiceKey::PAL_AUCTION_QUERY_RESP);
		}
		else
		{
			operAssist.BuildTLV("���ݿ�Ϊ��");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_AUCTION_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ������Ϣ�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_AUCTION_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ������Ϣ�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_AUCTION_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ������Ϣ�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_AUCTION_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>���<%s>������Ϣ����ϵͳ����",operName,ServerName,RoleName);
		logFile.WriteText("�ɽ�OL",strLog);//��������Ϣд����־��
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_AUCTION_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	return true;
}

//��ѯ��־��Ϣ
bool CPalLogInfo::LogInfo_Query(char * ServerName,char * ServerIP,char * RoleName,int iBigType,char * szBigType,
								int iSmallType,char * szSmallType,char * szBegintime, char * szEndtime,int iIndex, int iPageSize)
{
	try
	{
		char szTableName[100];
		char szSqlFlag[40];
		ZeroMemory(szTableName,100);
		ZeroMemory(szSqlFlag,40);
		int recordnum=0;
		int PageNum=0;
		vector <CGlobalStruct::TFLV> DBTFLV;

		sprintf(strLog,"%s, ��ѯ����<%s>���<%s>����־��Ϣ:����%s,С��%s", operName,ServerName,RoleName,szBigType,szSmallType);
		logFile.WriteText("�ɽ�OL",strLog);
		
		
		//����servername��ѯlogdbid
		operAssist.FindLogdbIP(logdbip,ServerName);
		
		operAssist.FindTableAndSql(iBigType,iSmallType,szTableName,szSqlFlag);
		//////////���ݴ�С�࣬�ҵ�table name ,sql_flag/////////////
		if(iBigType == 5)//�������Ƚ�����
		{
			sprintf(operszSql,"select count(*) as countnum from %s where eventtype=%i and rolename='%s' and time>='%s'and time<'%s' ",
				szTableName,iSmallType,RoleName,szBegintime,szEndtime);	
			recordnum=operAssist.FindRecordNum(logdbip,operszSql,1);
			if(recordnum==0)
			{
				operAssist.BuildTLV("��ѯ���ݿ���Ϊ��");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
			}
			if(recordnum%iPageSize==0)
			{
				PageNum=recordnum/iPageSize;
			}
			else
			{
				PageNum=recordnum/iPageSize+1;
			}
			operAssist.getRemoteSql("PAL",szSqlFlag,operRemoteSql,4);//��ȡԶ�̲�����Sql���
			if(!strcmp("",operRemoteSql))
			{
				operAssist.BuildTLV("���ݿ����Ҳ�����Ӧ��sql���");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
			}
			DBTFLV=GameDBQuery("PAL",logdbip,1,0,0,operRemoteSql,szTableName,
				iSmallType,RoleName,szBegintime,szEndtime,(iIndex-1)*iPageSize,iPageSize);
		}
		else if(iBigType == 4&& iSmallType!=28)//��Ǯ����
		{
			
			
			sprintf(operszSql,"select count(*) as countnum from %s where eventtype=%i and rolename='%s' and time>='%s'and time<'%s' ",
				szTableName,iSmallType,RoleName,szBegintime,szEndtime);	
			recordnum=operAssist.FindRecordNum(logdbip,operszSql,1);
			if(recordnum==0)
			{
				operAssist.BuildTLV("��ѯ���ݿ���Ϊ��");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;				
			}
			if(recordnum%iPageSize==0)
			{
				PageNum=recordnum/iPageSize;
			}
			else
			{
				PageNum=recordnum/iPageSize+1;
			}
			
			operAssist.getRemoteSql("PAL",szSqlFlag,operRemoteSql,4);//��ȡԶ�̲�����Sql���
			if(!strcmp("",operRemoteSql))
			{
				operAssist.BuildTLV("���ݿ����Ҳ�����Ӧ��sql���");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
			}
			DBTFLV=GameDBQuery("PAL",logdbip,1,0,0,operRemoteSql,szTableName,
				iSmallType,RoleName,szBegintime,szEndtime,(iIndex-1)*iPageSize,iPageSize);
		}
		else
		{
			if(iBigType==1&&iSmallType==8)
			{
				sprintf(operszSql,"select count(*) as countnum from %s where rolename='%s' and logintime>='%s' and logintime<'%s' ",
						szTableName,
						RoleName,
						szBegintime,
						szEndtime);//����Զ�̲�����sql���
			}
			else if(iBigType==1&&iSmallType==2)
			{
				
				sprintf(operszSql,"select count(*) as countnum from %s where rolename='%s' and createtime>='%s' and createtime<'%s' ",
					szTableName,
					RoleName,
					szBegintime,
					szEndtime);//����Զ�̲�����sql���

			}
			else if(iBigType==1&&iSmallType==9)
			{
				sprintf(operszSql,"select count(*) as countnum from %s where rolename='%s' and fromname='DelByTimeOut' and time>='%s' and time<'%s' ",
					szTableName,
					RoleName,
					szBegintime,
					szEndtime);//����Զ�̲�����sql���
			}
			else
			{

				sprintf(operszSql,"select count(*) as countnum from %s where rolename='%s' and time>='%s' and time<'%s' ",
							szTableName,
							RoleName,
							szBegintime,
							szEndtime);//����Զ�̲�����sql���
			}
			
			
			recordnum=operAssist.FindRecordNum(logdbip,operszSql,1);
			if(recordnum==0)
			{
				operAssist.BuildTLV("��ѯ���ݿ���Ϊ��");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
				
			}
			if(recordnum%iPageSize==0)
			{
				PageNum=recordnum/iPageSize;
			}
			else
			{
				PageNum=recordnum/iPageSize+1;
			}
			operAssist.getRemoteSql("PAL",szSqlFlag,operRemoteSql,4);//��ȡԶ�̲�����Sql���
			if(!strcmp("",operRemoteSql))
			{
				operAssist.BuildTLV("���ݿ����Ҳ�����Ӧ��sql���");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
			}
			DBTFLV=GameDBQuery("PAL",logdbip,1,0,0,operRemoteSql,szTableName,
				RoleName,szBegintime,szEndtime,(iIndex-1)*iPageSize,iPageSize);
		}
		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV),CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP,PageNum);
		}
		else
		{
			operAssist.BuildTLV("���ݿ�Ϊ��");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());

	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ��־��Ϣ�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ��־��Ϣ�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ��־��Ϣ�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>���<%s>��־��Ϣ����ϵͳ����",operName,ServerName,RoleName);
		logFile.WriteText("�ɽ�OL",strLog);//��������Ϣд����־��
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	return TRUE;
}

//��ѯ��־��ʷ��Ϣ
bool CPalLogInfo::HisLogInfo_Query(char * ServerName,char * ServerIP,char * RoleName,int iBigType,char * szBigType,
								   int iSmallType,char * szSmallType,char * szBegintime, char * szEndtime,int iIndex, int iPageSize)
{
	try
	{
		char szTableName[100];
		char szSqlFlag[40];
		ZeroMemory(szTableName,100);
		ZeroMemory(szSqlFlag,40);
		int recordnum=0;
		int PageNum=0;
		vector <CGlobalStruct::TFLV> DBTFLV;

		sprintf(strLog,"%s, ��ѯ����<%s>���<%s>����־��Ϣ:����%s,С��%s", operName,ServerName,RoleName,szBigType,szSmallType);
		logFile.WriteText("�ɽ�OL",strLog);
		
		
		//����servername��ѯlogdbid
		operAssist.FindLogdbIP(logdbip,ServerName);
		
		operAssist.FindTableAndSql1(iBigType,iSmallType,szTableName,szSqlFlag);
		//////////���ݴ�С�࣬�ҵ�table name ,sql_flag/////////////
		if(iBigType == 5)//�������Ƚ�����
		{
			sprintf(operszSql,"select count(*) as countnum from %s where eventtype=%i and rolename='%s' and time>='%s'and time<'%s' ",
				szTableName,iSmallType,RoleName,szBegintime,szEndtime);	
			recordnum=operAssist.FindRecordNum(logdbip,operszSql,1);
			if(recordnum==0)
			{
				operAssist.BuildTLV("��ѯ���ݿ���Ϊ��");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_HISTORY_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
			}
			if(recordnum%iPageSize==0)
			{
				PageNum=recordnum/iPageSize;
			}
			else
			{
				PageNum=recordnum/iPageSize+1;
			}
			operAssist.getRemoteSql("PAL",szSqlFlag,operRemoteSql,4);//��ȡԶ�̲�����Sql���
			if(!strcmp("",operRemoteSql))
			{
				operAssist.BuildTLV("���ݿ����Ҳ�����Ӧ��sql���");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_HISTORY_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
			}
			DBTFLV=GameDBQuery("PAL",logdbip,1,0,0,operRemoteSql,szTableName,
				iSmallType,RoleName,szBegintime,szEndtime,(iIndex-1)*iPageSize,iPageSize);
		}
		else if(iBigType == 4&& iSmallType!=28)//��Ǯ����
		{
			
			
			sprintf(operszSql,"select count(*) as countnum from %s where eventtype=%i and rolename='%s' and time>='%s'and time<'%s' ",
				szTableName,iSmallType,RoleName,szBegintime,szEndtime);	
			recordnum=operAssist.FindRecordNum(logdbip,operszSql,1);
			if(recordnum==0)
			{
				operAssist.BuildTLV("��ѯ���ݿ���Ϊ��");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_HISTORY_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;				
			}
			if(recordnum%iPageSize==0)
			{
				PageNum=recordnum/iPageSize;
			}
			else
			{
				PageNum=recordnum/iPageSize+1;
			}
			
			operAssist.getRemoteSql("PAL",szSqlFlag,operRemoteSql,4);//��ȡԶ�̲�����Sql���
			if(!strcmp("",operRemoteSql))
			{
				operAssist.BuildTLV("���ݿ����Ҳ�����Ӧ��sql���");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_HISTORY_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
			}
			DBTFLV=GameDBQuery("PAL",logdbip,1,0,0,operRemoteSql,szTableName,
				iSmallType,RoleName,szBegintime,szEndtime,(iIndex-1)*iPageSize,iPageSize);
		}
		else
		{
			if(iBigType==1&&iSmallType==8)
			{
				sprintf(operszSql,"select count(*) as countnum from %s where rolename='%s' and logintime>='%s' and logintime<'%s' ",
						szTableName,
						RoleName,
						szBegintime,
						szEndtime);//����Զ�̲�����sql���
			}
			else if(iBigType==1&&iSmallType==2)
			{
				
				sprintf(operszSql,"select count(*) as countnum from %s where rolename='%s' and createtime>='%s' and createtime<'%s' ",
					szTableName,
					RoleName,
					szBegintime,
					szEndtime);//����Զ�̲�����sql���

			}
			else if(iBigType==1&&iSmallType==9)
			{
				sprintf(operszSql,"select count(*) as countnum from %s where rolename='%s' and fromname='DelByTimeOut' and time>='%s' and time<'%s' ",
					szTableName,
					RoleName,
					szBegintime,
					szEndtime);//����Զ�̲�����sql���
			}
			else
			{

				sprintf(operszSql,"select count(*) as countnum from %s where rolename='%s' and time>='%s' and time<'%s' ",
							szTableName,
							RoleName,
							szBegintime,
							szEndtime);//����Զ�̲�����sql���
			}
			
			
			recordnum=operAssist.FindRecordNum(logdbip,operszSql,1);
			if(recordnum==0)
			{
				operAssist.BuildTLV("��ѯ���ݿ���Ϊ��");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_HISTORY_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
				
			}
			if(recordnum%iPageSize==0)
			{
				PageNum=recordnum/iPageSize;
			}
			else
			{
				PageNum=recordnum/iPageSize+1;
			}
			operAssist.getRemoteSql("PAL",szSqlFlag,operRemoteSql,4);//��ȡԶ�̲�����Sql���
			if(!strcmp("",operRemoteSql))
			{
				operAssist.BuildTLV("���ݿ����Ҳ�����Ӧ��sql���");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_HISTORY_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
			}
			DBTFLV=GameDBQuery("PAL",logdbip,1,0,0,operRemoteSql,szTableName,
				RoleName,szBegintime,szEndtime,(iIndex-1)*iPageSize,iPageSize);
		}
		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV),CEnumCore::ServiceKey::PAL_HISTORY_LOGINFO_QUERY_RESP,PageNum);
		}
		else
		{
			operAssist.BuildTLV("���ݿ�Ϊ��");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_HISTORY_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ��־��ʷ��Ϣ�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_HISTORY_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ��־��ʷ��Ϣ�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_HISTORY_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ��־��ʷ��Ϣ�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_HISTORY_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>���<%s>��־��Ϣ����ϵͳ����",operName,ServerName,RoleName);
		logFile.WriteText("�ɽ�OL",strLog);//��������Ϣд����־��
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_HISTORY_LOGINFO_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	return TRUE;
}

//��ѯ���CDKEY������ȡ��Ϣ
bool CPalLogInfo::CDKey_Get_Query(char * ServerName,char * ServerIP,char * szAccount,char * szBegintime, char * szEndtime,int iIndex, int iPageSize)
{
	try
	{
		sprintf(strLog,"%s, ��ѯ����<%s>���<%s>��CDKey������ȡ��¼",operName,ServerName,szAccount);
		logFile.WriteText("�ɽ�OL",strLog);
		
		
		//����servername��ѯlogdbid
		operAssist.FindAccountDBIP(logdbip,ServerName);
		
		operAssist.getRemoteSql("PAL","PAL_CDKEY_QUERY",operRemoteSql,4);//��ȡ��ѯCDKEY������ȡ��¼��Sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.BuildTLV("���ݿ����Ҳ�����Ӧ��sql���");
			operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CDKEY_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
			return false;
		}

		vector<CGlobalStruct::TFLV>	DBTFLV=GameDBQuery("PAL",logdbip,0,iIndex,iPageSize,operRemoteSql,szAccount,szBegintime,szEndtime);

		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV),CEnumCore::ServiceKey::PAL_CDKEY_QUERY_RESP);
		}
		else
		{
			operAssist.BuildTLV("���ݿ�Ϊ��");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CDKEY_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ���CDKEY������ȡ��Ϣ�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CDKEY_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ���CDKEY������ȡ��Ϣ�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CDKEY_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ���CDKEY������ȡ��Ϣ�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CDKEY_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>���<%s>��CDKey������ȡ��¼����ϵͳ����",operName,ServerName,szAccount);
		logFile.WriteText("�ɽ�OL",strLog);//��������Ϣд����־��
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_CDKEY_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	return true;
}

//��ѯ���Ԫ�����ļ�¼
bool CPalLogInfo::Money_Waste_Query(char * ServerName,char * ServerIP,char * szAccount,char * szBegintime, char * szEndtime,int iIndex, int iPageSize)
{
	try
	{
		sprintf(strLog,"%s, ��ѯ����<%s>���<%s>��Ԫ�����ļ�¼",operName,ServerName,szAccount);
		logFile.WriteText("�ɽ�OL",strLog);
		
		
		//����servername��ѯlogdbid
		operAssist.FindAccountDBIP(logdbip,ServerName);
		
		operAssist.getRemoteSql("PAL","PAL_ITEMMAIL_WASTE",operRemoteSql,4);//��ȡ��ѯ���Ԫ�����ļ�¼��Sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.BuildTLV("���ݿ����Ҳ�����Ӧ��sql���");
			operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ITEMMAIL_WASTE_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
			return false;
		}
		vector<CGlobalStruct::TFLV>	DBTFLV=GameDBQuery("PAL",logdbip,0,iIndex,iPageSize,operRemoteSql,szAccount,szBegintime,szEndtime);

		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV),CEnumCore::ServiceKey::PAL_ITEMMAIL_WASTE_RESP);
		}
		else
		{
			operAssist.BuildTLV("���ݿ�Ϊ��");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ITEMMAIL_WASTE_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ���Ԫ�����ļ�¼�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ITEMMAIL_WASTE_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ���Ԫ�����ļ�¼�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ITEMMAIL_WASTE_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ���Ԫ�����ļ�¼�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ITEMMAIL_WASTE_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>���<%s>��Ԫ�����ļ�¼����ϵͳ����",operName,ServerName,szAccount);
		logFile.WriteText("�ɽ�OL",strLog);//��������Ϣд����־��
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ITEMMAIL_WASTE_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;	
	}
	return true;
}

//��ȡ��־����
bool CPalLogInfo::GetLogBigType()
{
	try
	{
		sprintf(strLog,"%s, ��ѯ��־������Ϣ",operName);
		logFile.WriteText("�ɽ�OL",strLog);
		
		operAssist.getRemoteSql("PAL","PAL_GET_LOGBIGTYPE",operRemoteSql,4);//��ȡ��־������Ϣ��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.BuildTLV("���ݿ����Ҳ�����Ӧ��sql���");
			operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOG_BIGTYPE_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
			return false;
		}
		vector<CGlobalStruct::TFLV>	DBTFLV=GameDBQuery("PAL","127.0.0.1",4,0,0,operRemoteSql);
		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV),CEnumCore::ServiceKey::PAL_LOG_BIGTYPE_QUERY_RESP);
		}
		else
		{
			operAssist.BuildTLV("���ݿ�Ϊ��");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOG_BIGTYPE_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡ��־�����ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOG_BIGTYPE_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡ��־�����ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOG_BIGTYPE_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡ��־���������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOG_BIGTYPE_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ѯ��־������Ϣ����ϵͳ����",operName);
		logFile.WriteText("�ɽ�OL",strLog);//��������Ϣд����־��
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOG_BIGTYPE_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	return true;
}

//��ȡ��־С��
bool CPalLogInfo::GetLogSmallType(int iBigType)
{
	try
	{
		sprintf(strLog,"%s, ��ѯ��־С����Ϣ",operName);
		logFile.WriteText("�ɽ�OL",strLog);
		
		operAssist.getRemoteSql("PAL","PAL_GET_LOGSMALLTYPE",operRemoteSql,4);//��ȡ��־������Ϣ��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.BuildTLV("���ݿ����Ҳ�����Ӧ��sql���");
			operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOG_SMALLTYPE_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
			return false;
		}
		vector<CGlobalStruct::TFLV>	DBTFLV=GameDBQuery("PAL","127.0.0.1",4,0,0,operRemoteSql,iBigType);

		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV),CEnumCore::ServiceKey::PAL_LOG_SMALLTYPE_QUERY_RESP);
		}
		else
		{
			operAssist.BuildTLV("���ݿ�Ϊ��");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOG_SMALLTYPE_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡ��־С���ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOG_SMALLTYPE_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡ��־С���ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOG_SMALLTYPE_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ȡ��־С�������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOG_SMALLTYPE_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ѯ��־����<%s>����־С����Ϣ����ϵͳ����", operName,iBigType);
		logFile.WriteText("PAL",strLog);//��������Ϣд����־��
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_LOG_SMALLTYPE_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	return TRUE;
}


//��ѯ��ɫɾ����־
bool CPalLogInfo::Pal_RoleDel_Query(char * ServerName,char * ServerIP,char * szAccount,char * RoleName,int iType,char * szBegintime, char * szEndtime,int iIndex, int iPageSize)
{
	try
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>���<%s>��ɫɾ����־",operName,ServerName,szAccount);
		logFile.WriteText("�ɽ�OL",strLog);
		
		//����servername��ѯlogdbid
		
		operAssist.FindLogdbIP(logdbip,ServerName);

		vector<CGlobalStruct::TFLV>	DBTFLV;
		
		//�ж��ʺ��Ƿ�Ϊ��
		if(!strcmp("", szAccount))
		{
			operAssist.getRemoteSql("PAL","Pal_RoleDel_byRoleName",operRemoteSql,4);//�������IP��ѯ��¼��¼��sql���
			if(!strcmp("",operRemoteSql))
			{
				operAssist.BuildTLV("���ݿ����Ҳ�����Ӧ��sql���");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ROLEDEL_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
			}
			if(iType==0)
			{
				DBTFLV=GameDBQuery("PAL",logdbip,1,iIndex,iPageSize,operRemoteSql,"logdb",RoleName,szBegintime,szEndtime);
			}
			else
			{
				DBTFLV=GameDBQuery("PAL",logdbip,1,iIndex,iPageSize,operRemoteSql,"logdb1",RoleName,szBegintime,szEndtime);
			}
			
		}
		else
		{
			operAssist.getRemoteSql("PAL","Pal_RoleDel_byAccount",operRemoteSql,4);//��������ʺŲ�ѯ��¼��¼��sql���
			if(!strcmp("",operRemoteSql))
			{
				operAssist.BuildTLV("���ݿ����Ҳ�����Ӧ��sql���");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ROLEDEL_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
			}
			if(iType==0)
			{
				DBTFLV=GameDBQuery("PAL",logdbip,1,iIndex,iPageSize,operRemoteSql,"logdb",szAccount,szBegintime,szEndtime);
			}
			else
			{
				DBTFLV=GameDBQuery("PAL",logdbip,1,iIndex,iPageSize,operRemoteSql,"logdb1",szAccount,szBegintime,szEndtime);
			}
			
			
		}
		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV),CEnumCore::ServiceKey::PAL_ROLEDEL_QUERY_RESP);
		}
		else
		{
			operAssist.BuildTLV("���ݿ�Ϊ��");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ROLEDEL_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ��ɫɾ����־�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ROLEDEL_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ��ɫɾ����־�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ROLEDEL_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ѯ��ɫɾ����־�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ROLEDEL_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>���<%s>��½��¼��־����ϵͳ����",operName,ServerName,szAccount);
		logFile.WriteText("�ɽ�OL",strLog);//��������Ϣд����־��
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_ROLEDEL_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	return true;
}


//��½�ǳ���Ϣ
vector <CGlobalStruct::TFLV>  CPalLogInfo::LoginInfo(char * ServerName, char * ServerIP,char * UserName, char * UserNick,char * IP,char * begintime, char * endtime,int iIndex, int iPageSize)
{
	try
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>���<%s>��½��¼��־",operName,ServerName,UserName);
		logFile.WriteText("�ɽ�OL",strLog);
		
		//����servername��ѯlogdbid
		int iType=StrToInt(UserNick);//��UserNick����itype��ʷ���ǵ�ǰ
		operAssist.FindLogdbIP(logdbip,ServerName);

		vector <CGlobalStruct::TFLV> DBTFLV;
		
		
		//�ж��ʺ��Ƿ�Ϊ��
		if(!strcmp("", UserName))
		{
			/*operAssist.getRemoteSql("PAL","PALLOG_ACCOUNT_LOGINBYIP",operRemoteSql,4);//�������IP��ѯ��¼��¼��sql���
			if(!strcmp("",operRemoteSql))
			{
				operAssist.BuildTLV("SQLΪ��");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ACCOUNT_LOGIN_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
			}*/
			if(iType==0)
			{
				operAssist.getRemoteSql("PAL","PALLOG_ACCOUNT_LOGINBYIP",operRemoteSql,4);//�������IP��ѯ��¼��¼��sql���
				if(!strcmp("",operRemoteSql))
				{
					operAssist.BuildTLV("SQLΪ��");
					operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ACCOUNT_LOGIN_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
					return false;
				}
				DBTFLV=GameDBQuery("PAL",logdbip,1,iIndex,iPageSize,operRemoteSql,"logdb",IP,begintime,endtime);
			}
			else
			{
				//maple update
				operAssist.getRemoteSql("PAL","PALLOG_IP_LOGINHISTORY",operRemoteSql,4);//�������IP��ѯ��¼��¼��sql���
				if(!strcmp("",operRemoteSql))
				{
					operAssist.BuildTLV("SQLΪ��");
					operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ACCOUNT_LOGIN_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
					return false;
				}
				DBTFLV=GameDBQuery("PAL",logdbip,1,iIndex,iPageSize,operRemoteSql,IP,begintime,endtime,IP,begintime,endtime,IP,begintime,endtime);

			}
			
		}
		else
		{
			/*operAssist.getRemoteSql("PAL","PALLOG_ACCOUNT_LOGINBYACCOUNT",operRemoteSql,4);//��������ʺŲ�ѯ��¼��¼��sql���
			if(!strcmp("",operRemoteSql))
			{
				operAssist.BuildTLV("SQLΪ��");
				operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ACCOUNT_LOGIN_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
				return false;
			}*/
			
			if(iType==0)
			{
				operAssist.getRemoteSql("PAL","PALLOG_ACCOUNT_LOGINBYACCOUNT",operRemoteSql,4);//��������ʺŲ�ѯ��¼��¼��sql���
				if(!strcmp("",operRemoteSql))
				{
					operAssist.BuildTLV("SQLΪ��");
					operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ACCOUNT_LOGIN_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
					return false;
				}
				DBTFLV=GameDBQuery("PAL",logdbip,1,iIndex,iPageSize,operRemoteSql,"logdb",UserName,begintime,endtime);
			}
			else
			{
				//maple update
				operAssist.getRemoteSql("PAL","PALLOG_ACCOUNT_LOGINHISTORY",operRemoteSql,4);//��������ʺŲ�ѯ��¼��¼��sql���
				if(!strcmp("",operRemoteSql))
				{
					operAssist.BuildTLV("SQLΪ��");
					operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ACCOUNT_LOGIN_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
					return false;
				}
				//DBTFLV=GameDBQuery("PAL",logdbip,1,iIndex,iPageSize,operRemoteSql,"logdb1",UserName,begintime,endtime);
				DBTFLV=GameDBQuery("PAL",logdbip,1,iIndex,iPageSize,operRemoteSql,UserName,begintime,endtime,UserName,begintime,endtime,UserName,begintime,endtime);
			}
		}


		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV),CEnumCore::ServiceKey::PALLOG_ACCOUNT_LOGIN_RESP);
		}
		else
		{
			operAssist.BuildTLV("���ݿ�Ϊ��");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ACCOUNT_LOGIN_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��½�ǳ���Ϣ�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ACCOUNT_LOGIN_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��½�ǳ���Ϣ�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ACCOUNT_LOGIN_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��½�ǳ���Ϣ�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("PAL",errMessage);
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ACCOUNT_LOGIN_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>���<%s>��½��¼��־����ϵͳ����",operName,ServerName,UserName);
		logFile.WriteText("�ɽ�OL",strLog);//��������Ϣд����־��
		operAssist.BuildTLV("���ݿ�Ϊ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PALLOG_ACCOUNT_LOGIN_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
		return FALSE;
	}
	return true;
}

//add by ltx
//װ�����߲�ѯ
void CPalLogInfo::Pal_Equipment_Query(char * CustomItemName)
{
	try
	{
		char strlog[60];
		ZeroMemory(strlog,60);
		sprintf(strlog,"װ�����߲�ѯ");
		logFile.WriteText("�ɽ�OL",strlog);
		
		vector<CGlobalStruct::TFLV> DBTFLV=GameDBQuery("PAL","127.0.0.1",4,0,0,"exec Pal_Customitem_Query '%s'", CustomItemName);
		if(!DBTFLV.empty())
		{
			operAssist.BuildTLV(COperVector(DBTFLV));

#ifdef _DEBUG
			vector <CGlobalStruct::TFLV>::iterator iter;
			for(iter=DBTFLV.begin(); iter!=DBTFLV.end(); iter++)
			{
				printf("%x %x %s\n", (*iter).m_tagName, (*iter).m_tagFormat, (*iter).lpdata);
			}
#endif
		}
		else
		{
			operAssist.BuildTLV("װ�����߲�����");
		}
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_EQUIPMENT_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
	catch(...)
	{	
		operAssist.BuildTLV("װ�����߲�ѯʧ��");
		operAssist.SendBuf(pGameSess,CEnumCore::Msg_Category::PALLOG_ADMIN,CEnumCore::ServiceKey::PAL_EQUIPMENT_QUERY_RESP,operAssist.GetSendBuf(),operAssist.GetSendLength());
	}
}
