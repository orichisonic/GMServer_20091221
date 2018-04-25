#include "stdafx.h"
#include "time.h"
#include "CG2Info.h"
#include "md5.h"
#pragma comment(lib,"md5.lib")


//���캯��
CCG2Info::CCG2Info()
{
	//ZeroMemory(m_UserName,128);//��ʼ��ִ�в������û���
	//ZeroMemory(strLog,2048);//��ʼ����־��Ϣ
	//ZeroMemory(operRemoteSql, 2048);//��ʼ����Զ�����ݿ�ִ�в�����sql���
	//index = 0;//��ʼ��ҳ����Ϊ0
	//iPageSize = 0;//��ʼ��ҳ���СΪ0
	//ZeroMemory(logdbip, 20);//��ʼ��������IP
}

//��������
CCG2Info::~CCG2Info()
{
	Destroy();
}

//��ʼ�����еı���
bool CCG2Info::initialize(CSession * pSess,SOCKET m_Socket, unsigned char * pbuf, int length,CEnumCore::Message_Tag_ID m_message_tag_id)
{
	try
	{
		if(length>0)
		{
			recvVect.Create(length/8);
			if(recvVect.DeBody((unsigned char *)pbuf,length) == 0)//�����ͻ��˷��͹���������
			{
				return false;
			}
		}
		index = 0;//��ʼ��ҳ����Ϊ0
		iPageSize = 0;//��ʼ��ҳ���СΪ0
		if(recvVect.getTLVByTag(CEnumCore::TagName::Index,0).nIndex != 0)
		{
			index=StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::Index,0)).lpdata);//��ȡҳ����
		}
		if(recvVect.getTLVByTag(CEnumCore::TagName::PageSize,0).nIndex != 0)
		{
			iPageSize=StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::PageSize,0)).lpdata);//��ȡҳ���С
		}
	
		ZeroMemory(cg2_serverip,40);
		ZeroMemory(cg2_username,40);
		ZeroMemory(cg2_nickname,40);

		if(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Serverip,0).nIndex != 0)
		{
			sprintf(cg2_serverip,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Serverip,0)).lpdata);//��ȡ������IP
		}
		if(m_message_tag_id!=CEnumCore::Message_Tag_ID::CG2_InsertBankItemALL_Query)
		{
			if(recvVect.getTLVByTag(CEnumCore::TagName::CG2_UserName,0).nIndex != 0)
			{
				sprintf(cg2_username,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_UserName,0)).lpdata);//��ȡ�û���
			}
			if(recvVect.getTLVByTag(CEnumCore::TagName::CG2_nickname,0).nIndex != 0)
			{
				sprintf(cg2_nickname,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_nickname,0)).lpdata);//��ȡ�ǳ�
			}
		}

		
		if(recvVect.getTLVByTag(CEnumCore::TagName::CG2_UserID,0).nIndex != 0)
		{
			sprintf(cg2_usersn,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_UserID,0)).lpdata);//��ȡ���ID
		}
		if(recvVect.getTLVByTag(CEnumCore::TagName::CG2_CharNo,0).nIndex != 0)
		{
			cg2_charno=StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_CharNo,0)).lpdata);//��ȡ�û���ɫ��
		}
		if(recvVect.getTLVByTag(CEnumCore::TagName::CG2_GuildName,0).nIndex != 0)
		{
			sprintf(cg2_guildname,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_GuildName,0)).lpdata);//��������
		}
		if(recvVect.getTLVByTag(CEnumCore::TagName::CG2_GuildID,0).nIndex != 0)
		{
			cg2_guildid=StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_GuildID,0)).lpdata);//����ID
		}
		ZeroMemory(strLog,2048);//��ʼ����־��Ϣ		
		ZeroMemory(operRemoteSql, 2048);//��ʼ����Զ�����ݿ�ִ�в�����sql���
		
		ZeroMemory(m_UserName,128);//��ʼ��ִ�в������û���
		CLogic::SocketToUserName(m_Socket, m_UserName);//����ִ�в������û���
		
		ZeroMemory(logdbip, 20);//��ʼ��������IP
		char path[256];
		GetCurrentDirectory(256,path);
		char schemefilepath[256];
		sprintf(schemefilepath,"%s%s",path,"\\config\\scheme.ini");//���������ļ���·��
		GetPrivateProfileString("DATABASE","Address","127.0.0.1",logdbip,20,schemefilepath);//��ȡ���ص����ݿ��ַ
		sprintf(filepath,"%s%s",path,"\\config\\M_CG2.INI");//�����ļ���·��
		
		operAssist.initialize(pSess,m_Socket);
		m_hServerSocket=m_Socket;
		userByID = CLogic::SocketToUserID(m_Socket);//��ȡ����ԱID

		SYSTEMTIME nowtime;
		GetLocalTime(&nowtime);//��ȡϵͳ��ǰʱ��
		sprintf(curTime,"%i-%i-%i %i:%i:%i",nowtime.wYear,nowtime.wMonth,nowtime.wDay,nowtime.wHour,nowtime.wMinute,nowtime.wSecond);//����ǰʱ�丳ֵ

	}
	catch(...)
	{
	}
	return true;
}
//ɾ�����еı���
bool CCG2Info::Destroy()
{
	try
	{
		recvVect.Destroy();
		DBVect.Destroy();
		DBVect1.Destroy();
		
	}
	catch (...)
	{
		
	}
	return true;
}

//��ɫ�������ϣ���ѯ�ࣩ
bool CCG2Info::CG2_Account_Query()
{
	try
	{
	
		operAssist.ReadValue("GameName",retMessage[0],256);
		operAssist.ReadValue("CG2_Account_Query",retMessage[1],256);
		logFile.WriteLog(retMessage[0],m_UserName,retMessage[1]);//����������д����־�ļ�


		int dbid=0;
		if(strcmp("", cg2_username))//�������ʺŲ�Ϊ��
		{

			operAssist.getRemoteSql("CG2_ACCOUNT_QUERYBYACCOUNT",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
			if(!strcmp("",operRemoteSql))
			{
				logFile.ReadValue("M_Global","Global","DatabaseWithoutSql",retMessage[0],256);
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Query_Resp, retMessage[0]);
				return false;
			}
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteLogFile(cg2_serverip);
			dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
			sprintf(operszSql,operRemoteSql,cg2_username);
			logFile.WriteLogFile(operszSql);
			int tmpLength = 0;
			if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
			{
			
				tmpLength =DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//��������ݿ��ѯ�õ�������
				sprintf(operszSql,"����%i",tmpLength);
				logFile.WriteLogFile(operszSql);
				operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Query_Resp, &DBVect, tmpLength);

			}
			else if(dbid==2)
			{
				tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//��������ݿ��ѯ�õ�������
				sprintf(operszSql,"����%i",tmpLength);
				logFile.WriteLogFile(operszSql);
				operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Query_Resp, &DBVect, tmpLength);
			}
			else
			{
				operAssist.ReadValue("No_Account_Exist",retMessage[0],256);
				operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Query_Resp, retMessage[0]);
			}

		}
		else
		{
			operAssist.getRemoteSql("CG2_ACCOUNT_QUERYBYUSERNICK",operRemoteSql);//��ȡ�����ǳƲ�ѯ��sql���
			if(!strcmp("",operRemoteSql))
			{
				logFile.ReadValue("M_Global","Global","DatabaseWithoutSql",retMessage[0],256);
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Query_Resp, retMessage[0]);
				return false;
			}
			sprintf(operszSql,operRemoteSql,"\%",cg2_nickname,"\%");//��ʽ��sql��䣬ע��ת���
			logFile.WriteLogFile(operszSql);
			DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, 0, 0);//��������ݿ��ѯ�õ�������
			DBVect1.OperVectorMain("CG2",cg2_serverip, operszSql, 5, 0, 0);//��������ݿ��ѯ�õ�������
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Query_Resp, &DBVect, &DBVect1,index,iPageSize);


		}

	}
	catch (...)
	{
		logFile.WriteLogFile("CG2_Account_Query Error");
	}
	return true;
}
//��ɫ��ϸ����
bool CCG2Info::CG2_Account_Detail_Query()
{
	try
	{
		
		operAssist.ReadValue("GameName",retMessage[0],256);
		operAssist.ReadValue("CG2_Account_Detail_Query",retMessage[1],256);
		logFile.WriteLog(retMessage[0],m_UserName,retMessage[1]);//����������д����־�ļ�
		
		int dbid=0;
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		operAssist.getRemoteSql("CG2_Account_Detail_Query",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			logFile.ReadValue("M_Global","Global","DatabaseWithoutSql",retMessage[0],256);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Detail_Query_Resp,retMessage[0]);
			return false;
		}
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		sprintf(operszSql,operRemoteSql,cg2_username,cg2_usersn);
		if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
		{
		
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//��������ݿ��ѯ�õ�������
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Detail_Query_Resp, &DBVect, tmpLength);
			
		}
		else if(dbid==2)
		{
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//��������ݿ��ѯ�õ�������
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Detail_Query_Resp, &DBVect, tmpLength);
		}


		
	}
	catch (...)
	{
		logFile.WriteLogFile("CG2_Account_Detail_Query Error");
	}
	return true;
}

//��ɫ��������
bool CCG2Info::CG2_Account_Attribute_Query()
{
	try
	{
		operAssist.ReadValue("GameName",retMessage[0],256);
		operAssist.ReadValue("CG2_Account_Attribute_Query",retMessage[1],256);
		logFile.WriteLog(retMessage[0],m_UserName,retMessage[1]);//����������д����־�ļ�

		int dbid=0;
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		operAssist.getRemoteSql("CG2_Account_Attribute_Query",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{

			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Attribute_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		sprintf(operszSql,operRemoteSql,cg2_username,cg2_usersn);
		if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
		{
		
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//��������ݿ��ѯ�õ�������
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Attribute_Query_Resp, &DBVect, tmpLength);
			
		}
		else if(dbid==2)
		{
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//��������ݿ��ѯ�õ�������
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Attribute_Query_Resp, &DBVect, tmpLength);
		}
	}
	catch (...)
	{
		logFile.WriteLogFile("CG2_Account_Attribute_Query Error");
	}
	return true;
}
//��ɫ��������
bool CCG2Info::CG2_Account_Guild_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"�ʺ�<%s>��ѯ����<%s>���<%s>��ɫ��������",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("ħ������2",strLog);
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		operAssist.getRemoteSql("CG2_Account_Guild_Query",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Guild_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		sprintf(operszSql,operRemoteSql,cg2_username,cg2_usersn);
		if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
		{

			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//��������ݿ��ѯ�õ�������
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Guild_Query_Resp, &DBVect, tmpLength);
			
		}
		else if(dbid==2)
		{
			
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//��������ݿ��ѯ�õ�������
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Guild_Query_Resp, &DBVect, tmpLength);
		}
	}
	catch (...)
	{
		
	}
	return true;
}
//��ɫ�ƺ�����(�Ƚ�����ֱ�Ӳ����ݿ�)
bool CCG2Info::CG2_Account_Title_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"�ʺ�<%s>��ѯ����<%s>���<%s>��ɫ�ƺ�����",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("ħ������2",strLog);
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		operAssist.getRemoteSql("CG2_Account_Title_Query",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Title_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		sprintf(operszSql,operRemoteSql,cg2_username,cg2_usersn);
		
			
			CSqlHelper m_SqlHelper;
			vector<CGlobalStruct::DataRecord> datarecord;
			
			if(!datarecord.empty())//��ն���
			{
				datarecord.clear();
			}
			if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
			{
				m_SqlHelper.SqlHelperMain("CG2",&datarecord, cg2_serverip, operszSql, 3,index,iPageSize);//ִ�в�ѯ�������õ����
			}
			else if(dbid==2)
			{
				m_SqlHelper.SqlHelperMain("CG2",&datarecord, cg2_serverip, operszSql, 5,index,iPageSize);//ִ�в�ѯ�������õ����
			}
			DBVect.ClearInnerList();
					
			DBVect.InitCreate(datarecord.size()/m_SqlHelper.GetFieldNum()+1,m_SqlHelper.GetFieldNum()+5);//��ʼ��������������ռ�
			
			CEnumCore::TagName m_TagName;//�����õ�TagName
			CEnumCore::TagFormat m_TagFormat;//�����õ�TagFormat
			
			CMyEvent m_Event;//��ȡCEvent����
			
			//��ȡ��ѯ�ļ�¼����ÿһ��
			vector<CGlobalStruct::DataRecord>::iterator iter;
			CGlobalStruct::DataRecord  m_DataRecord;

			int i=0;
			char strTitle[15];//����ƺŵ�����
			int titleIndex = 0;//���治Ϊ�յĳƺ����

			for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
			{
				m_DataRecord = *iter;
				sprintf(strTitle,"titleFlags%03d",i);
				if(!strcmp(m_DataRecord.recordfieldname,strTitle))
				{
					i++;
					unsigned int title=StrToInt((char *)&m_DataRecord.recorddata);//��ȡ���ݿ��¼�еĳƺ�ID

					if(title!=0)
					{
						titleIndex++;
						sprintf(m_DataRecord.recordfieldname,"titleFlags%03d",titleIndex);
						int cnt = 0;
						while(true)
						{
							if(title ==1)
							{
								break;
							}
							title = title >> 1;
							cnt++;
						}
						int titleID = i*32+cnt;//��ȡ�����ĳƺ�ID
						char procName[256];
						sprintf(procName,"exec CG2_titleName_query %i",titleID);//��ʼ���洢���̵����
						
						char title_Name[256];
						ZeroMemory(title_Name,256);
						operAssist.CG2_Proc_Query(logdbip,4,procName,title_Name);//��ȡ��ҵĳƺ���Ϣ
						
						if(!strcmp(title_Name,""))
						{
							//�������ݿ��еõ���ֵ���浽CSocketData������
							sprintf(m_DataRecord.recorddata,"NULL");
						}
						else
						{
							//�������ݿ��еõ���ֵ���浽CSocketData������
							sprintf(m_DataRecord.recorddata,title_Name);
						}
						m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//��ȡ��¼�����ݵ�TagName
						m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//��ȡ��¼�����ݵ�TagFormat
						DBVect.AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//����TFLV,����ӵ�vector��

					}
					
						
				
					
				}
				else if(!strcmp(m_DataRecord.recordfieldname,"fameTitle"))
				{
					int title=StrToInt((char *)&m_DataRecord.recorddata);
					
					char procName[256];
					sprintf(procName,"exec CG2_FameName_query %i,%i",-1,title);//��ʼ���洢���̵����
					
					char title_Name[256];
					ZeroMemory(title_Name,256);
					operAssist.CG2_Proc_Query(logdbip,4,procName,title_Name);//��ȡ��ҵ���ʱ�ƺ�
					
					if(!strcmp(title_Name,""))
					{
						//�������ݿ��еõ���ֵ���浽CSocketData������
						sprintf(m_DataRecord.recorddata,"NULL");
					}
					else
					{
						//�������ݿ��еõ���ֵ���浽CSocketData������
						sprintf(m_DataRecord.recorddata,title_Name);
					}
					m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//��ȡ��¼�����ݵ�TagName
					m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//��ȡ��¼�����ݵ�TagFormat
					DBVect.AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//����TFLV,����ӵ�vector��
					
			
				}
				else if(!strcmp(m_DataRecord.recordfieldname,"tmpTitle"))
				{
					int title=StrToInt((char *)&m_DataRecord.recorddata);
					
					char procName[256];
					sprintf(procName,"exec CG2_TmptitleName_query %i",title);//��ʼ���洢���̵����
					
					char title_Name[256];
					ZeroMemory(title_Name,256);
					operAssist.CG2_Proc_Query(logdbip,4,procName,title_Name);//��ȡ��ҵ���ʱ�ƺ�
					
					if(!strcmp(title_Name,""))
					{
						//�������ݿ��еõ���ֵ���浽CSocketData������
						sprintf(m_DataRecord.recorddata,"NULL");
					}
					else
					{
						//�������ݿ��еõ���ֵ���浽CSocketData������
						sprintf(m_DataRecord.recorddata,title_Name);
					}
					m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//��ȡ��¼�����ݵ�TagName
					m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//��ȡ��¼�����ݵ�TagFormat
					DBVect.AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//����TFLV,����ӵ�vector��

				}
				else if(!strcmp(m_DataRecord.recordfieldname,"counter00")||
						!strcmp(m_DataRecord.recordfieldname,"reserve00")||
						!strcmp(m_DataRecord.recordfieldname,"reserve01")||
						!strcmp(m_DataRecord.recordfieldname,"reserve02")||
						!strcmp(m_DataRecord.recordfieldname,"reserve03")||
						!strcmp(m_DataRecord.recordfieldname,"reserve04")||
						!strcmp(m_DataRecord.recordfieldname,"reserve05")||
						!strcmp(m_DataRecord.recordfieldname,"reserve06")||
						!strcmp(m_DataRecord.recordfieldname,"reserve07")||
						!strcmp(m_DataRecord.recordfieldname,"charNo")||
						!strcmp(m_DataRecord.recordfieldname,"signalMessage"))
				{

				}
				else
				{
					m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//��ȡ��¼�����ݵ�TagName
					m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//��ȡ��¼�����ݵ�TagFormat

					DBVect.AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//����TFLV,����ӵ�vector��

				}


			}
			
			if(!datarecord.empty())//��ն���
			{
				datarecord.clear();
			}
			int tmpLength = DBVect.EnBody();//�������ݰ����õ�Ҫ���͵�����
			operAssist.ReturnSpecData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Title_Query_Resp, &DBVect, tmpLength);

	}
	catch (...)
	{
		
	}
	return true;
}
//��ɫ����ϵ
bool CCG2Info::CG2_Account_Address_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"�ʺ�<%s>��ѯ����<%s>���<%s>��ɫ����ϵ",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("ħ������2",strLog);
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		operAssist.getRemoteSql("CG2_Account_Address_Query",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Address_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		sprintf(operszSql,operRemoteSql,cg2_username,cg2_usersn);
		if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
		{
			
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//��������ݿ��ѯ�õ�������
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Address_Query_Resp, &DBVect, tmpLength);
			
		}
		else if(dbid==2)
		{
			
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//��������ݿ��ѯ�õ�������
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Address_Query_Resp, &DBVect, tmpLength);
		}
		
	}
	catch (...)
	{
		
	}
	return true;
}
//��ɫ�����YӍ
bool CCG2Info::CG2_Account_Skill_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"�ʺ�<%s>��ѯ����<%s>���<%s>��ɫ�����YӍ",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("ħ������2",strLog);
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		operAssist.getRemoteSql("CG2_Account_Skill_Query",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Skill_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		sprintf(operszSql,operRemoteSql,cg2_username,cg2_usersn);
		if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
		{
			
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//��������ݿ��ѯ�õ�������
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Skill_Query_Resp, &DBVect, tmpLength);
			
		}
		else if(dbid==2)
		{
			
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//��������ݿ��ѯ�õ�������
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Skill_Query_Resp, &DBVect, tmpLength);
		}
	}
	catch (...)
	{
		
	}
	return true;
}
//ʦ����ϵ��ѯ
bool CCG2Info::CG2_Account_Teach_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"�ʺ�<%s>��ѯ����<%s>���<%s>ʦ����ϵ��ѯ",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("ħ������2",strLog);
		
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);


		int type = StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_TypeID,0)).lpdata);//��ȡҪ��ѯ������
		if(type == 0)//����Ϊ0������ѧ������ѯ
		{
			operAssist.getRemoteSql("CG2_Account_Teach_Query",operRemoteSql);//��ȡ��ѯ��ɫʦ����ϵ��sql���
		}
		else//����Ϊ1��������ʦ����ѯ
		{
			operAssist.getRemoteSql("CG2_Account_Student_Query",operRemoteSql);//��ȡ��ѯ��ɫʦ����ϵ��sql���
		}
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Teach_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		sprintf(operszSql,operRemoteSql,cg2_username,cg2_usersn);
		if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
		{
			
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//��������ݿ��ѯ�õ�������
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Teach_Query_Resp, &DBVect, tmpLength);
			
		}
		else if(dbid==2)
		{
			
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//��������ݿ��ѯ�õ�������
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Teach_Query_Resp, &DBVect, tmpLength);
		}
	}
	catch (...)
	{
		
	}
	return true;
}

//��־��ѯ
bool CCG2Info::CG2_Log_Query()
{
		
	try
	{
		logFile.ReadValue("M_CG2","CG2","GameName",retMessage[0],256);
		logFile.ReadValue("M_CG2","CG2","CG2_Log_Query",retMessage[1],256);
		logFile.WriteLog(retMessage[0],m_UserName,retMessage[1]);//����������д����־�ļ�

		char Category[20];//��־����
		char Action[20];//��־С��
		char StartTime[25],EndTime[25];//��־��ѯ�Ŀ�ʼʱ��ͽ���ʱ��
		ZeroMemory(Category,20);
		ZeroMemory(Action,20);
		ZeroMemory(StartTime,25);
		ZeroMemory(EndTime,25);
		sprintf(Category,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_category,0)).lpdata);//��־����
		sprintf(Action,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_action,0)).lpdata);//��־С��
		sprintf(StartTime,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Log_StartTime,0)).lpdata);//��ȡ��־��ѯ�Ŀ�ʼʱ��
		sprintf(EndTime,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Log_EndTime,0)).lpdata);//��ȡ��־��ѯ�Ľ���ʱ��

		operAssist.getRemoteSql("CG2_LOG_QUERY",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Log_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,3);
		sprintf(operszSql,operRemoteSql,Category,Category,Action,cg2_username,cg2_usersn,StartTime,EndTime);//��ʽ��sql���
		int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 7, index, iPageSize);//��������ݿ��ѯ�õ�������
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Log_Query_Resp, &DBVect, tmpLength);
	}
	catch (...)
	{
		
	}
	return true;
}
//��ѯְҵ����
bool CCG2Info::CG2_Job_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_Job_Query");//����������д����־�ļ�
	
		operAssist.getRemoteSql("CG2_Job_Query",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Job_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		sprintf(operszSql,operRemoteSql);
		int tmpLength = DBVect.OperVectorMain(logdbip, operszSql, 2, 0, 0);//��������ݿ��ѯ�õ�������
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Job_Query_Resp, &DBVect, tmpLength);
	}
	catch (...)
	{
		
	}
	return true;
}
//��ѯ�������
bool CCG2Info::CG2_GetSkill_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_GetSkill_Query");//����������д����־�ļ�
		
		sprintf(operszSql,"%s","exec CG2_GetSkill_Query");
		int tmpLength = DBVect.OperVectorMain(logdbip, operszSql, 4, 0, 0);//��������ݿ��ѯ�õ�������
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_GetSkill_Query_Resp, &DBVect, tmpLength);
	}
	catch (...)
	{
		
	}
	return true;
}
//��ѯ��������
bool CCG2Info::CG2_SkillList_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_SkillList_Query");//����������д����־�ļ�
		
		char SkillType[256];
		ZeroMemory(SkillType,256);
		sprintf(SkillType,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SkillType,0)).lpdata);//�������

		sprintf(operszSql,"exec CG2_SkillList_Query '%s'",SkillType);
		int tmpLength = DBVect.OperVectorMain(logdbip, operszSql, 4, 0, 0);//��������ݿ��ѯ�õ�������
		operAssist.ReturnData(CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_SkillList_Query_Resp, DBVect.getDes(),tmpLength);
	}
	catch (...)
	{
		
	}
	return true;

}
//ģ����ѯ����
bool CCG2Info::CG2_SearchItem_Query()
{
	try
	{
		operAssist.ReadValue("GameName",retMessage[0],256);
		operAssist.ReadValue("CG2_SearchItem_Query",retMessage[1],256);
		logFile.WriteLog(retMessage[0],m_UserName,retMessage[1]);//����������д����־�ļ�
		int iType=0;
		int tmpLength=0;
		char itemname[40];
		iType=StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_TypeID,0).lpdata));
		sprintf(itemname,"%s",(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_TypeName,0).lpdata));
		tmpLength=sprintf(operszSql,"exec CG2_SearchItem_Query %i,%s",iType,itemname);//0:������1://����
		DBVect.OperVectorMain(logdbip,operszSql,4,0,0);
		operAssist.ReturnData(CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_SearchItem_Query_Resp,DBVect.getDes(),tmpLength);
	}
	catch (...)
	{
		
	}
	return true;

}
//��ѯ�������
bool CCG2Info::CG2_Get_Item_Query()
{
	try
	{
		operAssist.ReadValue("GameName",retMessage[0],256);
		operAssist.ReadValue("CG2_Get_Item_Query",retMessage[1],256);
		logFile.WriteLog(retMessage[0],m_UserName,retMessage[1]);//����������д����־�ļ�
		int iType=0;
		int tmpLength=0;
		iType=StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_TypeID,0).lpdata));
		tmpLength=sprintf(operszSql,"exec CG2_Get_Item_Query %i",iType);//0:������1://����
		tmpLength = DBVect.OperVectorMain(logdbip, operszSql, 4, 0, 0);//��������ݿ��ѯ�õ�������
		operAssist.ReturnData(CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Get_Item_Query_Resp,DBVect.getDes(),tmpLength);
	}
	catch (...)
	{
		
	}
	return true;
}
//�������Ʋ�ѯ
bool CCG2Info::CG2_Get_Item_Detail_Query()
{
	try
	{
		operAssist.ReadValue("GameName",retMessage[0],256);
		operAssist.ReadValue("CG2_Get_Item_Detail_Query",retMessage[1],256);
		logFile.WriteLog(retMessage[0],m_UserName,retMessage[1]);//����������д����־�ļ�
		
		char ItemType[40];
		ZeroMemory(ItemType,40);
		sprintf(ItemType,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemType,0)).lpdata);//�������

		////���д洢���̻��item ID
		char procName[256];
		ZeroMemory(procName,256);
		sprintf(procName,"exec CG2_ItemID_Query '%s'",ItemType);
		
		
		char strID[10];
		ZeroMemory(strID,10);
		operAssist.CG2_Proc_Query(logdbip,4,procName,strID);////���ݵ�������õ���ID

		operAssist.getRemoteSql("CG2_ItemName_Query",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Get_Item_Detail_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		sprintf(operszSql,operRemoteSql,strID);
		int tmpLength = DBVect.OperVectorMain(logdbip, operszSql, 4, 0, 0);//��������ݿ��ѯ�õ�������
		operAssist.ReturnData(CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Get_Item_Detail_Query_Resp,DBVect.getDes(),tmpLength);
	}
	catch (...)
	{
		
	}
	return true;
}
//��ӵ���
bool CCG2Info::CG2_Insert_Item_Query()
{
	try
	{
		operAssist.ReadValue("GameName",retMessage[0],256);
		operAssist.ReadValue("CG2_Insert_Item_Query",retMessage[1],256);
		logFile.WriteLog(retMessage[0],m_UserName,retMessage[1]);//����������д����־�ļ�
		
		int result=0;

		char ItemName[256];
		ZeroMemory(ItemName,256);
		char ItemNumber[256];
		ZeroMemory(ItemNumber,256);
		sprintf(ItemName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemName,0)).lpdata);//��õ�������
		sprintf(ItemNumber,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemNum,0)).lpdata);//��õ�������
		////���д洢���̻��item ID
		char procName[256];
		ZeroMemory(procName,256);
		sprintf(procName,"exec CG2_ItemID_Query_itemname '%s'",ItemName);

		char strID[10];
		ZeroMemory(strID,10);
		operAssist.CG2_Proc_Query(logdbip,4,procName,strID);////���ݵ������ƻ�õ���ID

		char remark[256];
		ZeroMemory(remark,256);
		sprintf(remark,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Remark,0)).lpdata);//��ע///present item

		operAssist.getRemoteSql("CG2_AddItem_Query",operRemoteSql);//��ȡ��ӵ���sql���
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,strID,ItemNumber,remark,cg2_usersn);//��ʽ��sql���

		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		int dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		int tmpLength=0;
		if(dbid==1)//�������ڵ�һ�����ݿ���
		{
			//����Ե�һ�����ݿ���в�����sql���
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		}
		else if(dbid==2)//�������ڵڶ������ݿ���
		{
			//����Եڶ������ݿ���в�����sql���
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,11);
		}

		if(tmpLength>0)//�����ɹ�
		{
			operAssist.getRemoteSql("CG2_UpdateChar_Query",operRemoteSql);//��ȡ���뵽���ݿ��е�sql���
			sprintf(operszSql,operRemoteSql,cg2_username,cg2_charno,cg2_usersn,remark,0,cg2_serverip,result);//��ʽ��sql���
			tmpLength=DBVect.OperVectorMain(logdbip,operszSql,0);
							
			if(tmpLength>0)
			{
					//maple add///����������һ�����ݿ�
					Sleep(30000);
					
					operAssist.getRemoteSql("CG2_Return_Result",operRemoteSql);//��ȡ���ؽ��sql���
					sprintf(operszSql,operRemoteSql,curTime,cg2_username,strID,ItemNumber);//��ʽ��sql��䣬��ǰʱ�䣬����ʺţ�����
					char gm_result[2];
					ZeroMemory(gm_result,2);
					operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
					operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,gm_result);
					if(!strcmp(gm_result,"2"))
					{
						GetPrivateProfileString("CG2","Add_PresentItem_Fail",NULL,RetMessage,30,filepath);//���浥��������Ϣ
						operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Item_Query_Resp,RetMessage);//������Ϣ
						char LogContent[128];
						ZeroMemory(LogContent,128);
						//д����־�ļ�
						sprintf(LogContent,"%s,In Game Concerto Gate,%s Add PresentItem %s Fail",curTime,m_UserName,cg2_username);
						logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query_Query",cg2_serverip,LogContent);
					}
					else
					{
						GetPrivateProfileString("CG2","Add_PresentItem_Success",NULL,RetMessage,30,filepath);//���浥��������Ϣ
						operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Item_Query_Resp,RetMessage);//������Ϣ
						char LogContent[128];
						ZeroMemory(LogContent,128);
						//д����־�ļ�
						sprintf(LogContent,"%s,In Game Concerto Gate,%s Add PresentItem %s Success",curTime,m_UserName,cg2_username);
						logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query",cg2_serverip,LogContent);
					}
				}

			}
			else
			{
				GetPrivateProfileString("CG2","Add_PresentItem_Fail",NULL,RetMessage,30,filepath);//���浥��������Ϣ
				operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Item_Query_Resp,RetMessage);//������Ϣ
				char LogContent[128];
				ZeroMemory(LogContent,128);
				//д����־�ļ�
				sprintf(LogContent,"%s,In Game Concerto Gate,%s Add Present Item %s Fail",curTime,m_UserName,cg2_username);
				logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query",cg2_serverip,LogContent);
			}
		}
		catch(...)
		{
				operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Item_Query_Resp,"ERROR");//�����޸�ְҵ������Ϣ
				char LogContent[128];
				ZeroMemory(LogContent,128);
				//д����־�ļ�
				sprintf(LogContent,"%s,In Game Concerto Gate,%s Add PresentItem %s Fail",curTime,m_UserName,cg2_serverip);
				logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query",cg2_serverip,LogContent);
		}
		return true;
}
//������е���
bool CCG2Info::CG2_InsertBankItem_Query()
{

	try
	{
		operAssist.ReadValue("GameName",retMessage[0],256);
		operAssist.ReadValue("CG2_InsertBankItem_Query",retMessage[1],256);
		logFile.WriteLog(retMessage[0],m_UserName,retMessage[1]);//����������д����־�ļ�
		
		int result=0;

		char ItemName[256];
		char ItemNumber[256];
		char pame[256];
		ZeroMemory(pame,256);
		ZeroMemory(ItemName,256);
		ZeroMemory(ItemNumber,256);
		sprintf(ItemName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemName,0)).lpdata);//��õ�������
		sprintf(ItemNumber,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemNum,0)).lpdata);//��õ�������
		char procName[256];
		ZeroMemory(procName,256);
		sprintf(procName,"exec CG2_ItemID_Query_itemname '%s'",ItemName);
		char strID[10];
		ZeroMemory(strID,10);
		operAssist.CG2_Proc_Query(logdbip,4,procName,strID);////���ݵ������ƻ�õ���ID
	
		sprintf(pame,"%s %s",strID,ItemNumber);


	/*	char *lpItemName=strtok((char*)&ItemName,"|");
		char lpItemNum[10];
		while(lpItemName!=NULL)
		{
			//////////////////////////////////////////////////////////////////////////
			char procName[256];
			ZeroMemory(procName,256);
			sprintf(procName,"exec CG2_ItemID_Query_itemname '%s'",lpItemName);
			char strID[10];
			ZeroMemory(strID,10);
			operAssist.CG2_Proc_Query(logdbip,0,procName,strID);////���ݵ������ƻ�õ���ID
			for(int i=0;i<strlen(ItemNumber);i++)
			{
				if(ItemNumber[i]=='|')//��ѯ��־","
				{
					memcpy(lpItemNum,ItemNumber,i);//�õ��ļ���
					lpItemNum[i]='\0';
					memcpy(ItemNumber,ItemNumber+i+1,strlen(ItemNumber)-i-1);
					break;
				}
			}
			sprintf(pame,"%s%s %s ",pame,strID,lpItemNum);
			lpItemName=strtok(NULL,"|");//��ѯ��־"|"
		}*/

		
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);//��ȡ��ӵ���sql���
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,19,pame,0,"",cg2_usersn);//��ʽ��sql���
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		int dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		int tmpLength=0;
		if(dbid==1)//�������ڵ�һ�����ݿ���
		{
			//����Ե�һ�����ݿ���в�����sql���
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		}
		else if(dbid==2)//�������ڵڶ������ݿ���
		{
			//����Եڶ������ݿ���в�����sql���
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,11);
		}

		if(tmpLength>0)//�����ɹ�
		{
				//maple add///����������һ�����ݿ�
 				Sleep(30000);
					
				operAssist.getRemoteSql("CG2_getResult",operRemoteSql);//��ȡ���ؽ��sql���
				sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,19,pame,cg2_usersn);//��ʽ��sql���
				char gm_result[2];
				ZeroMemory(gm_result,2);
				operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
				operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,gm_result);
				if(!strcmp(gm_result,"2"))
				{
					GetPrivateProfileString("CG2","Add_PresentItem_Fail",NULL,RetMessage,30,filepath);//���浥��������Ϣ
					operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_InsertBankItem_Query_Resp,RetMessage);//������Ϣ
					char LogContent[128];
					ZeroMemory(LogContent,128);
					//д����־�ļ�
					sprintf(LogContent,"%s,In Game Concerto Gate,%s Add PresentItem %s Fail",curTime,m_UserName,cg2_username);
					logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query_Query",cg2_serverip,LogContent);
				}
				else
				{
					GetPrivateProfileString("CG2","Add_PresentItem_Success",NULL,RetMessage,30,filepath);//���浥��������Ϣ
					operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_InsertBankItem_Query_Resp,RetMessage);//������Ϣ
					char LogContent[128];
					ZeroMemory(LogContent,128);
					//д����־�ļ�
					sprintf(LogContent,"%s,In Game Concerto Gate,%s Add PresentItem %s Success",curTime,m_UserName,cg2_username);
					logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query",cg2_serverip,LogContent);
				}
		}
		else
		{
			GetPrivateProfileString("CG2","Add_PresentItem_Fail",NULL,RetMessage,30,filepath);//���浥��������Ϣ
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Item_Query_Resp,RetMessage);//������Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//д����־�ļ�
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Add Present Item %s Fail",curTime,m_UserName,cg2_username);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query",cg2_serverip,LogContent);
		}
	}
	catch(...)
	{
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Item_Query_Resp,"ERROR");//�����޸�ְҵ������Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//д����־�ļ�
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Add PresentItem %s Fail",curTime,m_UserName,cg2_serverip);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query",cg2_serverip,LogContent);
	}
	return true;
}
//�������е�����ӣ����룩
bool CCG2Info::CG2_InsertBankItemALL_Query()
{

	try
	{
		operAssist.ReadValue("GameName",retMessage[0],256);
		operAssist.ReadValue("CG2_InsertBankItem_Query",retMessage[1],256);
		logFile.WriteLog(retMessage[0],m_UserName,retMessage[1]);//����������д����־�ļ�
		
		int result=0;

		char ItemName[256];
		char ItemNumber[256];
		char pame[256];
		ZeroMemory(pame,256);
		ZeroMemory(ItemName,256);
		ZeroMemory(ItemNumber,256);
		sprintf(ItemName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemName,0)).lpdata);//��õ�������
		sprintf(ItemNumber,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemNum,0)).lpdata);//��õ�������
		char procName[256];
		ZeroMemory(procName,256);
		sprintf(procName,"exec CG2_ItemID_Query_itemname '%s'",ItemName);
		char strID[10];
		ZeroMemory(strID,10);
		operAssist.CG2_Proc_Query(logdbip,4,procName,strID);////���ݵ������ƻ�õ���ID
	
		sprintf(pame,"%s %s",strID,ItemNumber);


	/*	char *lpItemName=strtok((char*)&ItemName,"|");
		char lpItemNum[10];
		while(lpItemName!=NULL)
		{
			//////////////////////////////////////////////////////////////////////////
			char procName[256];
			ZeroMemory(procName,256);
			sprintf(procName,"exec CG2_ItemID_Query_itemname '%s'",lpItemName);
			char strID[10];
			ZeroMemory(strID,10);
			operAssist.CG2_Proc_Query(logdbip,0,procName,strID);////���ݵ������ƻ�õ���ID
			for(int i=0;i<strlen(ItemNumber);i++)
			{
				if(ItemNumber[i]=='|')//��ѯ��־","
				{
					memcpy(lpItemNum,ItemNumber,i);//�õ��ļ���
					lpItemNum[i]='\0';
					memcpy(ItemNumber,ItemNumber+i+1,strlen(ItemNumber)-i-1);
					break;
				}
			}
			sprintf(pame,"%s%s %s ",pame,strID,lpItemNum);
			lpItemName=strtok(NULL,"|");//��ѯ��־"|"
		}*/

		
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);//��ȡ��ӵ���sql���
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,19,pame,0,"",cg2_usersn);//��ʽ��sql���
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		int dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		int tmpLength=0;
		if(dbid==1)//�������ڵ�һ�����ݿ���
		{
			//����Ե�һ�����ݿ���в�����sql���
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		}
		else if(dbid==2)//�������ڵڶ������ݿ���
		{
			//����Եڶ������ݿ���в�����sql���
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,11);
		}

		if(tmpLength>0)//�����ɹ�
		{
				//maple add///����������һ�����ݿ�
 				Sleep(30000);
					
				operAssist.getRemoteSql("CG2_getResult",operRemoteSql);//��ȡ���ؽ��sql���
				sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,19,pame,cg2_usersn);//��ʽ��sql���
				char gm_result[2];
				ZeroMemory(gm_result,2);
				operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
				operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,gm_result);
				if(!strcmp(gm_result,"2"))
				{
					GetPrivateProfileString("CG2","Add_PresentItem_Fail",NULL,RetMessage,30,filepath);//���浥��������Ϣ
					operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_InsertBankItemALL_Query_Resp,RetMessage);//������Ϣ
					char LogContent[128];
					ZeroMemory(LogContent,128);
					//д����־�ļ�
					sprintf(LogContent,"%s,In Game Concerto Gate,%s Add PresentItem %s Fail",curTime,m_UserName,cg2_username);
					logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query_Query",cg2_serverip,LogContent);
				}
				else
				{
					GetPrivateProfileString("CG2","Add_PresentItem_Success",NULL,RetMessage,30,filepath);//���浥��������Ϣ
					operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_InsertBankItemALL_Query_Resp,RetMessage);//������Ϣ
					char LogContent[128];
					ZeroMemory(LogContent,128);
					//д����־�ļ�
					sprintf(LogContent,"%s,In Game Concerto Gate,%s Add PresentItem %s Success",curTime,m_UserName,cg2_username);
					logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query",cg2_serverip,LogContent);
				}
		}
		else
		{
			GetPrivateProfileString("CG2","Add_PresentItem_Fail",NULL,RetMessage,30,filepath);//���浥��������Ϣ
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_InsertBankItemALL_Query_Resp,RetMessage);//������Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//д����־�ļ�
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Add Present Item %s Fail",curTime,m_UserName,cg2_username);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query",cg2_serverip,LogContent);
		}
	}
	catch(...)
	{
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_InsertBankItemALL_Query_Resp,"ERROR");//�����޸�ְҵ������Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//д����־�ļ�
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Add PresentItem %s Fail",curTime,m_UserName,cg2_serverip);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query",cg2_serverip,LogContent);
	}
	return true;
}

//�޸���ҽ�ɫְҵ
bool CCG2Info::CG2_UpdateJob_Query()
{
	try
	{
			logFile.WriteLog("Concerto Gate",m_UserName,"CG2_UpdateJob_Query");//����������д����־�ļ�
			int result=0;

			char JobName[50];
			ZeroMemory(JobName,50);
			sprintf(JobName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_JobID,0)).lpdata);//��ȡ�û�job ID

			////���д洢���̻��JobID
			char procName[256];
			ZeroMemory(procName,256);
			sprintf(procName,"exec CG2_JobID_Query '%s'",JobName);

			char JobID[10];
			ZeroMemory(JobID,10);
			operAssist.CG2_Proc_Query(logdbip,4,procName,JobID);////����ְҵ���ƻ��ְҵID


			char remark[256];
			ZeroMemory(remark,256);
			sprintf(remark,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Remark,0)).lpdata);//��ע///update job


			operAssist.getRemoteSql("CG2_UpdateJob_Query",operRemoteSql);//��ȡ�޸���ҽ�ɫְҵsql���
			sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,JobID,remark,cg2_usersn);//��ʽ��sql���
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			int dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
			int tmpLength=0;
			if(dbid==1)//�������ڵ�һ�����ݿ���
			{
				//����Ե�һ�����ݿ���в�����sql���
				tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
			}
			else if(dbid==2)//�������ڵڶ������ݿ���
			{
				//����Եڶ������ݿ���в�����sql���
				tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,11);
			}

			if(tmpLength>0)//�����ɹ�
			{							
				operAssist.getRemoteSql("CG2_UpdateChar_Query",operRemoteSql);//��ȡ���뵽���ݿ��е�sql���
				sprintf(operszSql,operRemoteSql,cg2_username,cg2_charno,cg2_usersn,remark,0,cg2_serverip,result);//��ʽ��sql���

				tmpLength=DBVect.OperVectorMain(logdbip,operszSql,0);
				if(tmpLength>0)//�����ɹ�
				{
					//maple add
					Sleep(30000);
					operAssist.getRemoteSql("CG2_Return_Result_1",operRemoteSql);//��ȡ���뵽���ݿ��е�sql���
					sprintf(operszSql,operRemoteSql,curTime,cg2_username,JobID);//��ʽ��sql��䣬��ǰʱ�䣬����ʺţ�����
					char gm_result[2];
					ZeroMemory(gm_result,2);
					operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
					if(dbid==1)//�������ڵ�һ�����ݿ���
					{
						//����Ե�һ�����ݿ���в�����sql���
						operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,gm_result);
					}
					else if(dbid==2)//�������ڵڶ������ݿ���
					{
						//����Եڶ������ݿ���в�����sql���
						operAssist.CG2_Proc_Query(cg2_serverip,5,operszSql,gm_result);
					}

				

					if(!strcmp(gm_result,"2"))
					{
						GetPrivateProfileString("CG2","Update_CharJob_Fail",NULL,RetMessage,30,filepath);//���浥��������Ϣ
						operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UpdateJob_Query_Resp,RetMessage);//������Ϣ
						char LogContent[128];
						ZeroMemory(LogContent,128);
						//д����־�ļ�
						sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Job %s Fail",curTime,m_UserName,cg2_username);
						logFile.WriteDBLog(userByID,"Concerto Gate","CG2_UpdateJob_Query",cg2_serverip,LogContent);
					}
					else
					{
						GetPrivateProfileString("CG2","Update_CharJob_Success",NULL,RetMessage,30,filepath);//���浥��������Ϣ
						operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UpdateJob_Query_Resp,RetMessage);//������Ϣ
						char LogContent[128];
						ZeroMemory(LogContent,128);
						//д����־�ļ�
						sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Job %s Success",curTime,m_UserName,cg2_username);
						logFile.WriteDBLog(userByID,"Concerto Gate","CG2_UpdateJob_Query",cg2_serverip,LogContent);
					}
				}
			}
			else
			{
				GetPrivateProfileString("CG2","Update_CharJob_Fail",NULL,RetMessage,30,filepath);//���浥��������Ϣ
				operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UpdateJob_Query_Resp,RetMessage);//���ط�ͣ���ʧ�ܵ���Ϣ
				char LogContent[128];
				ZeroMemory(LogContent,128);
				//д����־�ļ�
				sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Job %s Fail",curTime,m_UserName,cg2_username);
				logFile.WriteDBLog(userByID,"Concerto Gate","CG2_UpdateJob_Query",cg2_serverip,LogContent);
			}

		}
		catch(...)
		{
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UpdateJob_Query_Resp,"ERROR");//�����޸�ְҵ������Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//д����־�ļ�
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Job %s Fail",curTime,m_UserName,cg2_username);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_UpdateJob_Query",cg2_serverip,LogContent);
		}
		return true;
}
//�޸ļ��ܵȼ�
bool CCG2Info::CG2_Update_Account_Skill_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_Update_Account_Skill_Query");//����������д����־�ļ�
		
		int result=0;

		char UserSkillLevel[50];
		ZeroMemory(UserSkillLevel,50);
		sprintf(UserSkillLevel,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SkillLevel,0)).lpdata);//��ȡ�û����ܵȼ�

		char SkillID[20];
		ZeroMemory(SkillID,20);
		sprintf(SkillID,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SkillID,0)).lpdata);//��ȡ�û�����id

		char remark[256];
		ZeroMemory(remark,256);
		sprintf(remark,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Remark,0)).lpdata);///��ע////update skill level

		//GetConfigInfo(ServerIP,DBUser,DBUserOper,DBPwd,DBName,DBNameSec,CharSet);//��������ļ���Ϣ

		operAssist.getRemoteSql("CG2_SkillLevel_Update",operRemoteSql);//��ȡ�޸���ҽ�ɫְҵsql���


		//sprintf(szSql,remotesql,curTime,UserName,charNo,UserSkillLevel,remark,UserSN);//��ʽ��sql���
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,SkillID,UserSkillLevel,remark,cg2_usersn);//��ʽ��sql���

		//MakeRemoteSql((char *)&ServerIP,(char*)&DBUser,(char*)&DBPwd,(char*)&DBName, (char*)&CharSet, remoteConnectSql);
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		int dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		int tmpLength=0;
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)//�������ڵ�һ�����ݿ���
		{
			//����Ե�һ�����ݿ���в�����sql���
			//	MakeRemoteSql((char *)&ServerIP,(char*)&DBUserOper,(char*)&DBPwd,(char*)&DBName, (char*)&CharSet, remoteConnectSql);
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		}
		else if(dbid==2)//�������ڵڶ������ݿ���
		{
			//����Եڶ������ݿ���в�����sql���
			//MakeRemoteSql((char *)&ServerIP,(char*)&DBUserOper,(char*)&DBPwd,(char*)&DBNameSec, (char*)&CharSet, remoteConnectSql);
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,11);
		}

		if(tmpLength>0)//�����ɹ�
		{							
			operAssist.getRemoteSql("CG2_UpdateChar_Query",operRemoteSql);//��ȡ���뵽���ݿ��е�sql���
			sprintf(operszSql,operRemoteSql,cg2_username,cg2_charno,cg2_usersn,remark,0,cg2_serverip,result);//��ʽ��sql���
			tmpLength=DBVect.OperVectorMain(logdbip,operszSql,0);
			if(tmpLength>0)//�����ɹ�
			{
				//maple add
					Sleep(30000);

					operAssist.getRemoteSql("CG2_Return_Result",operRemoteSql);//��ȡ���ؽ��sql���
					sprintf(operszSql,operRemoteSql,curTime,cg2_username,SkillID,UserSkillLevel);//��ʽ��sql��䣬��ǰʱ�䣬����ʺţ�����

					char gm_result[2];
					ZeroMemory(gm_result,2);
					operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
					if(dbid==1)//�������ڵ�һ�����ݿ���
					{
						//����Ե�һ�����ݿ���в�����sql���
						operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,gm_result);
					}
					else if(dbid==2)//�������ڵڶ������ݿ���
					{
						//����Եڶ������ݿ���в�����sql���
						operAssist.CG2_Proc_Query(cg2_serverip,5,operszSql,gm_result);
					}
					
							

					if(!strcmp(gm_result,"2"))
					{
						GetPrivateProfileString("CG2","Update_SkillLevel_Fail",NULL,RetMessage,30,filepath);//���浥��������Ϣ
						operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Update_Account_Skill_Query_Resp,RetMessage);//������Ϣ
						char LogContent[128];
						ZeroMemory(LogContent,128);
						//д����־�ļ�
						sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill Level %s Fail",curTime,m_UserName,cg2_username);
						logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
					}
					else
					{
						GetPrivateProfileString("CG2","Update_SkillLevel_Success",NULL,RetMessage,30,filepath);//���浥��������Ϣ
						operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Update_Account_Skill_Query_Resp,RetMessage);//������Ϣ
						char LogContent[128];
						ZeroMemory(LogContent,128);
						//д����־�ļ�
						sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill Level %s Success",curTime,m_UserName,cg2_username);
						logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
					}
				}
			}
			else
			{
				GetPrivateProfileString("CG2","Update_CharJob_Fail",NULL,RetMessage,30,filepath);//���浥��������Ϣ
				operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Update_Account_Skill_Query_Resp,RetMessage);//���ط�ͣ���ʧ�ܵ���Ϣ
				char LogContent[128];
				ZeroMemory(LogContent,128);
				//д����־�ļ�
				sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Job %s Fail",curTime,m_UserName,cg2_username);
				logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
			}
		}
		catch(...)
		{
				operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Update_Account_Skill_Query_Resp,"ERROR");//�����޸�ְҵ������Ϣ
				char LogContent[128];
				ZeroMemory(LogContent,128);
				//д����־�ļ�
				sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill %s Fail",curTime,m_UserName,cg2_username);
				logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
		}
		return true;
}
//��ӽ�ɫ����
bool CCG2Info::CG2_Insert_Account_Skill_Query()
{
	try
	{
		int tmpLength=0;
		int dbid=0;
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_Insert_Account_Skill_Query");//����������д����־�ļ�

		int result=0;
		
		char SkillName[50];
		ZeroMemory(SkillName,50);
		sprintf(SkillName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SkillName,0)).lpdata);//��ȡ�û�Skill ID
		
		
		////���д洢���̻��SkillID
		char procName[256];
		ZeroMemory(procName,256);
		sprintf(procName,"exec CG2_SkillID_Query '%s'",SkillName);
		
		char SkillID[10];
		ZeroMemory(SkillID,10);
		operAssist.CG2_Proc_Query(logdbip,4,procName,SkillID);////���ݼ������ƻ�ü���ID
		
		char remark[256];
		ZeroMemory(remark,256);
		sprintf(remark,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Remark,0)).lpdata);///��ע////update skill level

		
		operAssist.getRemoteSql("CG2_AddSkill_Query",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Insert_Account_Skill_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,SkillID,remark,cg2_usersn);//��ʽ��sql���
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//��������ݿ��ѯ�õ�������
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//��������ݿ��ѯ�õ�������
		}
		if(tmpLength>0)//Զ�̲����ɹ�
		{
			operAssist.getRemoteSql("CG2_UpdateChar_Query",operRemoteSql);//��ȡ���뵽���ݿ��е�sql���
			sprintf(operszSql,operRemoteSql,cg2_username,cg2_charno,cg2_usersn,remark,0,cg2_serverip,result);//��ʽ��sql���
			tmpLength=DBVect.OperVectorMain(logdbip, operszSql, 0);//�������ݿ����
			if(tmpLength>0)
			{
				Sleep(30000);
				operAssist.getRemoteSql("CG2_Return_Result_1",operRemoteSql);//��ȡ���ؽ��sql���
				sprintf(operszSql,operRemoteSql,curTime,cg2_username,SkillID);//��ʽ��sql��䣬��ǰʱ�䣬����ʺţ�����
				char gm_result[2];
				ZeroMemory(gm_result,2);
				operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
				if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
				{
					operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,gm_result);
				}
				else if(dbid==2)
				{
					operAssist.CG2_Proc_Query(cg2_serverip,5,operszSql,gm_result);
				}
			
				if(!strcmp(gm_result,"2"))
				{
					GetPrivateProfileString("CG2","AddSkill_Fail",NULL,RetMessage,30,filepath);//���浥��������Ϣ
					operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Account_Skill_Query_Resp,RetMessage);//������Ϣ
					char LogContent[128];
					ZeroMemory(LogContent,128);
					//д����־�ļ�
					sprintf(LogContent,"%s,In Game Concerto Gate,%s Add Skill %s Fail",curTime,m_UserName,cg2_username);
					logFile.WriteDBLog(userByID,"Concerto Gate","CG2_AddSkill_Query",cg2_serverip,LogContent);
				}
				else
				{
					GetPrivateProfileString("CG2","AddSkill_Success",NULL,RetMessage,30,filepath);//���浥��������Ϣ
					operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Account_Skill_Query_Resp,RetMessage);//������Ϣ
					char LogContent[128];
					ZeroMemory(LogContent,128);
					//д����־�ļ�
					sprintf(LogContent,"%s,In Game Concerto Gate,%s Add Skill %s Success",curTime,m_UserName,cg2_username);
					logFile.WriteDBLog(userByID,"Concerto Gate","CG2_AddSkill_Query",cg2_serverip,LogContent);
				}


			}
		}
		else
		{

		}
	}
	catch (...)
	{
		
	}
	return true;
}
//�ʺŵ�½�ǳ���¼
bool CCG2Info::CG2_Use_Query()
{
	try
	{
		int dbid=0;
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_Use_Query");//����������д����־�ļ�
		char LoginTime[25],LogoutTime[25];
		ZeroMemory(LoginTime,25);
		ZeroMemory(LogoutTime,25);
		sprintf(LoginTime,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_LatestLoginTime,0)).lpdata);//��ȡ��¼ʱ��
		sprintf(LogoutTime,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_LatestLogoutTime,0)).lpdata);//��ȡ�ǳ�ʱ��
	
		operAssist.getRemoteSql("CG2_UserLogin_Query",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Use_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		sprintf(operszSql,operRemoteSql,cg2_username,LoginTime,LogoutTime);//��ʽ��sql���

		if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
		{
			
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//��������ݿ��ѯ�õ�������
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Use_Query_Resp, &DBVect, tmpLength);
			
		}
		else if(dbid==2)
		{
			
			int tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//��������ݿ��ѯ�õ�������
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Use_Query_Resp, &DBVect, tmpLength);
		}
	}
	catch (...)
	{
		
	}
	return true;
}

//ǿ���������
bool CCG2Info::CG2_KickAccount_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"�ʺ�<%s>����<%s>ǿ�����<%s>����",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("ħ������2",strLog);
		
		operAssist.getRemoteSql("CG2_KickPlayer_Query",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_KickAccount_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
	
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,24,"",0,"",cg2_usersn);//��ʽ��sql���
		int tmpLength =0;
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
		{
			
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//��������ݿ��ѯ�õ�������
			
		}
		else if(dbid==2)
		{
			
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//��������ݿ��ѯ�õ�������
		}
		if(tmpLength>0)
		{
			GetPrivateProfileString("CG2","Kick_Player_Success",NULL,RetMessage,30,filepath);//���浥��������Ϣ
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_KickAccount_Query_Resp, RetMessage);//����ǿ��������߳ɹ�����Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Kick Player %s Success",curTime,m_UserName,cg2_username);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_KickPlayer_Query",cg2_serverip,LogContent);
		}
		else
		{
			GetPrivateProfileString("CG2","Kick_Player_Fail",NULL,RetMessage,30,filepath);//���浥��������Ϣ
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_KickAccount_Query_Resp, RetMessage);//����ǿ���������ʧ�ܵ���Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Kick Player %s Fail",curTime,m_UserName,cg2_username);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_KickPlayer_Query",cg2_serverip,LogContent);
		}
	}	
	catch (...)
	{
		
	}
	return true;
}

//ǿ���������(û�лش�)
bool CCG2Info::CG2_KickAccount_Query_NoAnswer()
{
	try
	{
		int dbid=0;
		
		operAssist.getRemoteSql("CG2_KickPlayer_Query",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,24,"",0,"",cg2_usersn);//��ʽ��sql���
		int tmpLength =0;
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
		{
			
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//��������ݿ��ѯ�õ�������
			
		}
		else if(dbid==2)
		{
			
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//��������ݿ��ѯ�õ�������
		}
		if(tmpLength>0)
		{
			char LogContent[128];
			ZeroMemory(LogContent,128);
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Kick Player %s Success",curTime,m_UserName,cg2_username);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_KickPlayer_Query",cg2_serverip,LogContent);
		}
		else
		{
			char LogContent[128];
			ZeroMemory(LogContent,128);
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Kick Player %s Fail",curTime,m_UserName,cg2_username);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_KickPlayer_Query",cg2_serverip,LogContent);
		}
	}	
	catch (...)
	{
		
	}
	return true;
}
//bool CCG2Info::CG2_Insert_Gmtools
//��ѯ���з�ͣ�ʺ���Ϣ
bool CCG2Info::CG2_BanAccount_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_BanAccount_Query");//����������д����־�ļ�
		operAssist.getRemoteSql("CG2_SQLGetAllBanAccount_Qurey",operRemoteSql);//��ȡ��Զ�����ݿ��в�ѯ�����ʺŷ�ͣ��Ϣ��sql���
		sprintf(operszSql,operRemoteSql,cg2_serverip);//��ʽ��sql���
		int tmpLength = DBVect.OperVectorMain(logdbip, operszSql, 2, index, iPageSize);//��������ݿ��ѯ�õ�������
		if(tmpLength>0)
		{
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BanAccount_Query_Resp, &DBVect, tmpLength);
		}
		else
		{
			GetPrivateProfileString("CG2","No_BanAccount_Exist",NULL,RetMessage,30,filepath);//���浥��������Ϣ
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BanAccount_Query_Resp,RetMessage);
		}
		
		
		
	}
	catch (...)
	{
		operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BanAccount_Query_Resp,"ERROR");
	}
	return true;
}
//��ѯ�ض��ʺ���û�б���ͣ
bool CCG2Info::CG2_Account_BANISHMENT_QUERY()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_Account_BANISHMENT_QUERY");//����������д����־�ļ�
		operAssist.getRemoteSql("CG2_SQLGetBanAccount_Qurey",operRemoteSql);//��ȡ��Զ�����ݿ��в�ѯ�����ʺŷ�ͣ��Ϣ��sql���
		sprintf(operszSql,operRemoteSql,cg2_username,cg2_serverip);//��ʽ��sql���
		int tmpLength = DBVect.OperVectorMain(logdbip, operszSql, 2, index, iPageSize);//��������ݿ��ѯ�õ�������
		if(tmpLength>0)
		{
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_BANISHMENT_QUERY_RESP, &DBVect, tmpLength);
		}
		else
		{
			GetPrivateProfileString("CG2","No_Select_BanAccount_Exist",NULL,RetMessage,30,filepath);//���浥��������Ϣ
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_BANISHMENT_QUERY_RESP,RetMessage);
		}
		
		
		
	}
	catch (...)
	{
		operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_BANISHMENT_QUERY_RESP,"ERROR");
	}
	return true;
}


//��ͣ����ʺ�
bool CCG2Info::CG2_Account_Close()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_Account_Close");//����������д����־�ļ�						
		
		int type = 0;
		type = StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_TypeID,0)).lpdata);//��ȡ���ʺŻ��ǽ�ɫ��ͣ

		char BanContent[512];
		ZeroMemory(BanContent,512);
		sprintf(BanContent,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Content,0)).lpdata);//��ȡ��ͣԭ��

		time_t curTimeSec;
		time( &curTimeSec );//��ȡ��ǰʱ�䵽1970-1-1֮���ʱ���������ʾ

		char EndTime[25];
		ZeroMemory(EndTime,25);
		sprintf(EndTime,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_BanTime,0)).lpdata);//��ȡ��ͣʱ��	
		
		struct tm t_BanTime; //	tm�ṹ�����͵ı���������ʱ��					
		sscanf(EndTime,"%d-%d-%d %d:%d:%d",&t_BanTime.tm_year,&t_BanTime.tm_mon,&t_BanTime.tm_mday,&t_BanTime.tm_hour,&t_BanTime.tm_min,&t_BanTime.tm_sec);//����ͣʱ�丳ֵ��tm�ṹ��
		t_BanTime.tm_year = t_BanTime.tm_year - 1900;
		t_BanTime.tm_mon = t_BanTime.tm_mon - 1;
		time_t BanTime;//time_t���͵ı���������ʱ���������ʾ
		BanTime=mktime(&t_BanTime); //��ȡ��ͣʱ�䵽1970-1-1֮���ʱ���������ʾ
		char strBanTime[20];
		ZeroMemory(strBanTime,20);
		sprintf(strBanTime,"%i",BanTime-curTimeSec);//��ȡ��ͣʱ��			

		operAssist.getRemoteSql("CG2_GMInsert_Query",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		int tmpLength=0;
		if(type == 0)
		{
			sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,1,strBanTime,0,BanContent,cg2_usersn);//��ʽ��sql���
		}
		else
		{
			sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,24,strBanTime,0,BanContent,cg2_usersn);//��ʽ��sql���
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		int  dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)//�������ڵ�һ�����ݿ���
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//ִ��Զ�����ݲ���
		
		}
		else if(dbid==2)//�������ڵڶ������ݿ���
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//ִ��Զ�����ݲ���
		}
		if(tmpLength>0)//Զ�̲����ɹ�
		{
			operAssist.getRemoteSql("CG2_Account_Close",operRemoteSql);//��ȡ���뵽�������ݿ��е�sql���
			sprintf(operszSql,operRemoteSql,cg2_username,cg2_charno,cg2_usersn,EndTime,BanContent,cg2_serverip,type);//��ʽ��sql���
			tmpLength = DBVect.OperVectorMain(logdbip, operszSql, 0);//ִ�б������ݲ���
			if(tmpLength>0)
			{
				GetPrivateProfileString("CG2","Ban_Player_Success",NULL,RetMessage,30,filepath);//���浥��������Ϣ
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Close_RESP, RetMessage);//���ط�ͣ���ʧ�ܵ���Ϣ
				char LogContent[128];
				ZeroMemory(LogContent,128);
				sprintf(LogContent,"%s,In Game Concerto Gate,%s Ban Player %s Success",curTime,m_UserName,cg2_username);
				logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Account_Close",cg2_serverip,LogContent);		
			}
			else
			{
				GetPrivateProfileString("CG2","Ban_Player_Fail",NULL,RetMessage,30,filepath);//���浥��������Ϣ
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Close_RESP, RetMessage);//���ط�ͣ���ʧ�ܵ���Ϣ
				char LogContent[128];
				ZeroMemory(LogContent,128);
				sprintf(LogContent,"%s,In Game Concerto Gate,%s Ban Player %s Fail",curTime,m_UserName,cg2_username);
				logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Account_Close",cg2_serverip,LogContent);		
			}

		}
		else
		{
			GetPrivateProfileString("CG2","Ban_Player_Fail",NULL,RetMessage,30,filepath);//���浥��������Ϣ
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Close_RESP, RetMessage);//���ط�ͣ���ʧ�ܵ���Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Ban Player %s Fail",curTime,m_UserName,cg2_username);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Account_Close",cg2_serverip,LogContent);			
		}
	}
	catch (...)
	{
		
	}
	return true;
}

//�������ʺ�
bool CCG2Info::CG2_Account_Open()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_Account_Open");//����������д����־�ļ�				
		
		int type = 0;
		type = StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_TypeID,0)).lpdata);//��ȡ���ʺŻ��ǽ�ɫ��ͣ
		
		char UnbindContent[512];
		sprintf(UnbindContent,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Content,0)).lpdata);//��ȡ��ͣԭ��
	
		
		operAssist.getRemoteSql("CG2_GMInsert_Query",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		int tmpLength=0;
		if(type == 0)
		{
			sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,2,"",0,UnbindContent,cg2_usersn);//��ʽ��sql���
		}
		else
		{
			sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,24,"0",0,UnbindContent,"");//��ʽ��sql���
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		int  dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)//�������ڵ�һ�����ݿ���
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//ִ��Զ�����ݲ���
			
		}
		else if(dbid==2)//�������ڵڶ������ݿ���
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//ִ��Զ�����ݲ���
		}
		if(tmpLength>0)//Զ�̲����ɹ�
		{
			if(type == 1)
			{
				operAssist.getRemoteSql("CG2_Account_Open_ByAccount",operRemoteSql);//��ȡͨ���ʺŽ��н����뵽�������ݿ��е�sql���
				sprintf(operszSql,operRemoteSql,cg2_username,type);//��ʽ��sql���
			}
			else
			{
				operAssist.getRemoteSql("CG2_Account_Open_ByChar",operRemoteSql);//��ȡͨ����ɫ���ƽ��н����뵽�������ݿ��е�sql���
				sprintf(operszSql,operRemoteSql,cg2_username,cg2_usersn,type);//��ʽ��sql���
			}	
			tmpLength = DBVect.OperVectorMain(logdbip, operszSql, 0);//ִ�б������ݲ���
			if(tmpLength>0)
			{
				GetPrivateProfileString("CG2","Unbind_Player_Sucess",NULL,RetMessage,30,filepath);//���浥��������Ϣ
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Open_RESP, RetMessage);//���ط�ͣ���ʧ�ܵ���Ϣ
				char LogContent[128];
				ZeroMemory(LogContent,128);
				sprintf(LogContent,"%s,In Game Concerto Gate,%s Unbind Player %s Success",curTime,m_UserName,cg2_username);
				logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Account_Open",cg2_serverip,LogContent);		
			}
			else
			{
				GetPrivateProfileString("CG2","Unbind_Player_Fail",NULL,RetMessage,30,filepath);//���浥��������Ϣ
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Open_RESP, RetMessage);//���ط�ͣ���ʧ�ܵ���Ϣ
				char LogContent[128];
				ZeroMemory(LogContent,128);
				sprintf(LogContent,"%s,In Game Concerto Gate,%s Unbind Player %s Fail",curTime,m_UserName,cg2_username);
				logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Account_Open",cg2_serverip,LogContent);		
			}
			
		}
		else
		{
			GetPrivateProfileString("CG2","Unbind_Player_Fail",NULL,RetMessage,30,filepath);//���浥��������Ϣ
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Open_RESP, RetMessage);//���ط�ͣ���ʧ�ܵ���Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Unbind Player %s Fail",curTime,m_UserName,cg2_username);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Account_Open",cg2_serverip,LogContent);		
		}	
	}
	catch (...)
	{
		operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Account_Open_RESP, "ERROR");//���ؽ������ʺŴ�����Ϣ
		char LogContent[128];
		ZeroMemory(LogContent,128);
		sprintf(LogContent,"%s,In Game Concerto Gate,%s Unbind Player %s Error",curTime,m_UserName,cg2_username);
		logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Account_Open",cg2_serverip,LogContent);	
	}
	return true;
}

//�鿴����
bool CCG2Info::CG2_SearchNote_Query()
{
	
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_SearchNote_Query");//����������д����־�ļ�
		
		operAssist.getRemoteSql("CG2_SearchNote_Query",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_SearchNote_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		sprintf(operszSql,operRemoteSql);
		int tmpLength = DBVect.OperVectorMain(logdbip, operszSql, 2, index, iPageSize);//��������ݿ��ѯ�õ�������
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_SearchNote_Query_Resp, &DBVect, tmpLength);
		
	}
	catch (...)
	{
		
	}
	return true;
}

//���͹���
bool CCG2Info::CG2_SendNote_Query()
{		
	try
	{
		char CurServerIP[256];//������IP
		ZeroMemory(CurServerIP,256);
		char AllServerIP[256];//������IP
		ZeroMemory(AllServerIP,256);
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_SendNote_Query");//����������д����־�ļ�	
		
		sprintf(CurServerIP,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Serverip,0)).lpdata);//��ȡ���еķ�����IP
		sprintf(AllServerIP,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Serverip,0)).lpdata);//��ȡ���еķ�����IP
		
		char CurServerName[1024];//����������
		sprintf(CurServerName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ServerName,0)).lpdata);//��ȡ���еķ���������
		
		char noteContent[512];//��������
		sprintf(noteContent,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_NoteContent,0)).lpdata);//��ȡ��������
		
		int interval = StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Interval,0)).lpdata);//��ȡ����ʱ����
		int type = StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_TypeID,0)).lpdata);//��ȡ���ͣ��Ƿ�ʱ���͹���
		
		char EndTime[20];//���淢�͹����ֹʱ��	
		ZeroMemory(EndTime,20);
		time_t LimitTime;//time_t���͵ı���������ʱ���������ʾ
			
		
		
		if(type != 0)//������Ǽ�ʱ���͹���
		{
			sprintf(EndTime,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_EndTime,0)).lpdata);//��ȡ���͹����ֹʱ��					
			struct tm t_EndTime; //	tm�ṹ�����͵ı���������ʱ��					
			sscanf(EndTime,"%d-%d-%d %d:%d:%d",&t_EndTime.tm_year,&t_EndTime.tm_mon,&t_EndTime.tm_mday,&t_EndTime.tm_hour,&t_EndTime.tm_min,&t_EndTime.tm_sec);//�����͹����ֹʱ�丳ֵ��tm�ṹ��							
			t_EndTime.tm_year = t_EndTime.tm_year - 1900;
			t_EndTime.tm_mon = t_EndTime.tm_mon - 1;
			LimitTime=mktime(&t_EndTime); 
		}
		else
		{
			sprintf(EndTime,curTime);//��ʱ���͹���	
			LimitTime = 0; 
		}
		
		char result[1024];//����������
		ZeroMemory(result,1024);
		char tmpResult[256];//������ʱ�������
		ZeroMemory(tmpResult,256);//��ʼ����ʱ�������
		
		int noticeID = 0;//Ҫ��ȡ�Ĺ���ID
		char sNoticeID[128];
		ZeroMemory(sNoticeID,128);//��ʼ�����еĹ���ID
		char tmpNoticeID[8];
		ZeroMemory(tmpNoticeID,8);//��ʼ����ʱ����ID
		
		char tmpInfo[1024];//������ʱ���
		ZeroMemory(tmpInfo,1024);//��ʼ����ʱ���
		
		const char* split = "|";//��������ַ��"|"����
		char* tmpServerIP = strtok(CurServerIP,split);//��ȡ��һ��������IP
		int tmpLength=0;
		while(tmpServerIP != NULL)
		{
			operAssist.FindDBIP(tmpServerIP,tmpServerIP,2);
			operAssist.getRemoteSql("CG2_GetNoticeCount_Query",operRemoteSql);//��ȡҪ��ѯ���ݿ��е�ǰ���ڷ��͹���������sql���
			int iCount=0;
			char strCount[10];
			operAssist.CG2_Proc_Query(tmpServerIP,3,operRemoteSql,strCount);
			iCount=StrToInt(strCount);
			if(iCount<10)
			{
				char pame[520];
				ZeroMemory(pame,520);
				sprintf(pame,"%i %s",interval,noteContent);
				operAssist.getRemoteSql("CG2_GMInsert_Query",operRemoteSql);//��ȡ���뵽���ݿ��е�sql���
				sprintf(operszSql,operRemoteSql,curTime,"",0,4,pame,LimitTime,"","");//��ʽ��sql���
				operAssist.FindDBIP(tmpServerIP,tmpServerIP,1);
				tmpLength = DBVect.OperVectorMain(tmpServerIP, operszSql, 9);
				if(tmpLength>0)
				{
					operAssist.getRemoteSql("CG2_NoticeResult_Query",operRemoteSql);//��ȡ��ѯ����ID��sql���
					sprintf(operszSql,operRemoteSql,curTime,4,LimitTime);//��ʽ��sql���
					int inoticeID=0;
					char strnoticeID[10];
					operAssist.FindDBIP(tmpServerIP,tmpServerIP,2);
					operAssist.CG2_Proc_Query(tmpServerIP,3,operRemoteSql,strnoticeID);
					inoticeID=StrToInt(strnoticeID);
					ZeroMemory(tmpNoticeID,8);//��ʼ����ʱ����ID									
					sprintf(tmpNoticeID,"%i|",noticeID);//��ȡ��ʱ����ID
					
					ZeroMemory(tmpInfo,1024);//��ʼ����ʱ���
					sprintf(tmpInfo,"%s",sNoticeID);
					sprintf(sNoticeID,"%s%s",tmpInfo,tmpNoticeID);//��ȡ����ID
					
					ZeroMemory(tmpResult,256);//��ʼ����ʱ�������
					GetPrivateProfileString("CG2","Server",NULL,RetMessage,30,filepath);//������
					sprintf(tmpResult,"%s%s,",RetMessage,tmpServerIP);
					GetPrivateProfileString("CG2","Send_BoardTask_Success",NULL,RetMessage,30,filepath);//���淢�ͳɹ�!
					sprintf(tmpResult,"%s%s,",tmpResult,RetMessage);//����ÿһ�β����Ĳ������
					
					ZeroMemory(tmpInfo,1024);//��ʼ����ʱ���
					sprintf(tmpInfo,"%s",result);
					sprintf(result,"%s\n%s",tmpInfo,tmpResult);//�������ղ����Ĳ������
				}
				else
				{
					ZeroMemory(tmpResult,256);//��ʼ����ʱ�������
					GetPrivateProfileString("CG2","Server",NULL,RetMessage,30,filepath);//������
					sprintf(tmpResult,"%s%s,",RetMessage,tmpServerIP);
					GetPrivateProfileString("CG2","TooMany_BoardTask",NULL,RetMessage,30,filepath);//�������泬��10�������Ժ��ٷ�!
					sprintf(tmpResult,"%s%s,",tmpResult,RetMessage);//����ÿһ�β����Ĳ������
					
					ZeroMemory(tmpInfo,1024);//��ʼ����ʱ���
					sprintf(tmpInfo,"%s",result);
					sprintf(result,"%s\n%s",tmpInfo,tmpResult);//�������ղ����Ĳ������
				}
				


			}
			else
			{
				ZeroMemory(tmpResult,256);//��ʼ����ʱ�������
				GetPrivateProfileString("CG2","Server",NULL,RetMessage,30,filepath);//������
				sprintf(tmpResult,"%s%s,",RetMessage,tmpServerIP);
				GetPrivateProfileString("CG2","TooMany_BoardTask",NULL,RetMessage,30,filepath);//�������泬��10�������Ժ��ٷ�!
				sprintf(tmpResult,"%s%s,",tmpResult,RetMessage);//����ÿһ�β����Ĳ������
				
				ZeroMemory(tmpInfo,1024);//��ʼ����ʱ���
				sprintf(tmpInfo,"%s",result);
				sprintf(result,"%s\n%s",tmpInfo,tmpResult);//�������ղ����Ĳ������
			}

			tmpServerIP = strtok(NULL,split);//˳��õ�������IP
		}
		if(tmpServerIP != NULL)
		{
			delete []tmpServerIP;
			tmpServerIP=NULL;
		}


		operAssist.getRemoteSql("CG2_SendNote_Query",operRemoteSql);//��ȡ���һ��sql���
		sprintf(operszSql,operRemoteSql,AllServerIP,CurServerName,noteContent,"0",curTime,sNoticeID,EndTime);//��ʽ��sql���
		
		tmpLength=DBVect.OperVectorMain(logdbip, operszSql, 0);
		if(tmpLength>0)
		{
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_SendNote_Query_Resp,result);//�����������õ��Ľ��
			char LogContent[256];
			ZeroMemory(LogContent,256);
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Send Notice %s %s",curTime,m_UserName,sNoticeID,result);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SendNote_Query",AllServerIP,LogContent);
		}
		else
		{
			GetPrivateProfileString("CG2","Send_BoardTask_Fail",NULL,RetMessage,30,filepath);//���浥��������Ϣ
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_SendNote_Query_Resp,RetMessage);//�����������õ��Ľ��
			
			char LogContent[256];
			ZeroMemory(LogContent,256);
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Send Notice sNoticeID Fail",curTime,m_UserName,sNoticeID);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SendNote_Query",AllServerIP,LogContent);
		}		
		
		
		
	}
	catch (...)
	{
		
	}
	return true;
}

//ֹͣ���͹���
bool CCG2Info::CG2_DeleteNote_Query()
{
	char CurServerIP[256];//������IP
	ZeroMemory(CurServerIP,256);
	char AllServerIP[256];//������IP
	ZeroMemory(AllServerIP,256);
	try
	{
		
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_DeleteNote_Query");//����������д����־�ļ�						
		
		sprintf(CurServerIP,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Serverip,0)).lpdata);//��ȡ���еķ�����IP
		sprintf(AllServerIP,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Serverip,0)).lpdata);//��ȡ���еķ�����IP
		
		char noticeID[256];//����ID
		char AllNoticeID[256];//����ID^
		sprintf(noticeID,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_NoticeID,0)).lpdata);//��ȡ����ID
		sprintf(AllNoticeID,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_NoticeID,0)).lpdata);//��ȡ���еķ�����IP
		
		const char* split = "|";//��������ַ��"|"����
		char* tmpServerIP = strtok(CurServerIP,split);//��ȡ��һ��������IP
		char* tmpNoticeID = strtok(noticeID,split);//��ȡ��һ������ID
		int tmpLength=0;
		while(tmpServerIP != NULL)
		{
		//	GetConfigInfo(tmpServerIP,DBUser,DBUserOper,DBPwd,DBName,DBNameSec,CharSet);
			
			operAssist.getRemoteSql("CG2_GMInsert_Query",operRemoteSql);//��ȡ���뵽���ݿ��е�sql���
			sprintf(operszSql,operRemoteSql,curTime,"",0,5,tmpNoticeID,0,"","");//��ʽ��sql���
			operAssist.FindDBIP(tmpServerIP,tmpServerIP,1);
			tmpLength=DBVect.OperVectorMain(tmpServerIP, operszSql, 9);

			
			tmpServerIP = strtok(NULL,split);//˳��õ�������IP
			tmpNoticeID = strtok(NULL,split);//˳��õ�����ID
		}						
		
		if(tmpServerIP != NULL)
		{
			delete []tmpServerIP;
			tmpServerIP = NULL;
		}
		if(tmpNoticeID != NULL)
		{
			delete []tmpNoticeID;
			tmpNoticeID = NULL;
		}
		
		if(tmpLength>0)//�����ɹ�
		{
			sprintf(operszSql,"update CG2_SendNote set result=1 where gm_id='%s'",AllNoticeID);//��ʽ��sql���
			DBVect.OperVectorMain(logdbip, operszSql, 0);
			
			GetPrivateProfileString("CG2","Stop_BoardTask_Success",NULL,RetMessage,30,filepath);//���浥��������Ϣ
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_DeleteNote_Query_Resp,RetMessage);//����ֹͣ���͹���ɹ�����Ϣ
			
			char LogContent[256];
			ZeroMemory(LogContent,256);
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Stop Notice %s Success",curTime,m_UserName,AllNoticeID);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_DeleteNote_Query",AllServerIP,LogContent);
		}
		else
		{
			GetPrivateProfileString("CG2","Stop_BoardTask_Fail",NULL,RetMessage,30,filepath);//���浥��������Ϣ
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_DeleteNote_Query_Resp,RetMessage);//����ֹͣ���͹���ɹ�����Ϣ
			
			char LogContent[256];
			ZeroMemory(LogContent,256);
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Stop Notice %s Fail",curTime,m_UserName,AllNoticeID);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_DeleteNote_Query",AllServerIP,LogContent);
		}
	}
	catch(...)
	{
		char ErrorMsg[400];
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER| //�Զ�������Ϣ������
			FORMAT_MESSAGE_FROM_SYSTEM, //��ϵͳ��ȡ��Ϣ
			NULL,GetLastError(), //��ȡ������Ϣ��ʶ
			MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),//ʹ��ϵͳȱʡ����
			(LPTSTR)&lpMsgBuf, //��Ϣ������
			0,
			NULL);
		sprintf(ErrorMsg, "%s",lpMsgBuf);
		LocalFree(lpMsgBuf);
		
		operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_DeleteNote_Query_Resp,"Error");//���ش�����Ϣ

		
		char LogContent[256];
		ZeroMemory(LogContent,256);
		sprintf(LogContent,"%s,In Game Concerto Gate,%s Stop Notice Error",curTime,m_UserName);
		logFile.WriteDBLog(userByID,"Concerto Gate","CG2_DeleteNote_Query",AllServerIP,LogContent);	
	}

	return true;
}

//��ѯ������ҽ�ɫ��Ϣ
bool CCG2Info::CG2_BakAccount_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"�ʺ�<%s>��ѯ����<%s>�������<%s>��ɫ��Ϣ",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("ħ������2",strLog);
		if(strcmp("", cg2_username))//�������ʺŲ�Ϊ��
		{
			
			operAssist.getRemoteSql("CG2_BAKACCOUNT_QUERY",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
			if(!strcmp("",operRemoteSql))
			{
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BakAccount_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
				return false;
			}
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
			sprintf(operszSql,operRemoteSql,cg2_username);
			int tmpLength = 0;
			if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
			{
				
				tmpLength =DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//��������ݿ��ѯ�õ�������
				operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BakAccount_Query_Resp, &DBVect, tmpLength);
				
			}
			else if(dbid==2)
			{
				tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//��������ݿ��ѯ�õ�������
				operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BakAccount_Query_Resp, &DBVect, tmpLength);
			}
			else
			{
				operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BakAccount_Query_Resp, "���ʺŲ�����");
			}
			
		}
		else
		{
			operAssist.getRemoteSql("CG2_BAKNICK_QUERY",operRemoteSql);//��ȡ�����ǳƲ�ѯ��sql���
			if(!strcmp("",operRemoteSql))
			{
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BakAccount_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
				return false;
			}
			sprintf(operszSql,operRemoteSql,"\%",cg2_nickname,"\%");//��ʽ��sql��䣬ע��ת���
			DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, 0, 0);//��������ݿ��ѯ�õ�������
			DBVect1.OperVectorMain("CG2",cg2_serverip, operszSql, 5, 0, 0);//��������ݿ��ѯ�õ�������
			operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BakAccount_Query_Resp, &DBVect, &DBVect1,index,iPageSize);
			
			
		}
		
	}
	catch (...)
	{
		
	}
	return true;
}


//�ָ���ɫ����
bool CCG2Info::CG2_Recovery_Account_Qurey()
{
	try
	{
		int tmpLength=0;
		int dbid=0;
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_Recovery_Account_Qurey");//����������д����־�ļ�

		int iResult=0;
		//�ָ���ɫ֮ǰ����
		bool b_result=CG2_KickAccount_Query_NoAnswer();//����
		char result[40];
		if(b_result)
		{
			Sleep(30000);//��ʱ30��ȴ����
			//��ɲ�ѯ
			operAssist.getRemoteSql("CG2_SearchCharNo_Query",operRemoteSql);//��ȡ���ؽ��sql���
			sprintf(operszSql,operRemoteSql,cg2_username,cg2_charno);
			dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);//��ѯIP
			if(dbid==1)
			{
				operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,result);
			}
			else
			{
				operAssist.CG2_Proc_Query(cg2_serverip,5,operszSql,result);
			}
			if(StrToInt(result)==0)
			{
				operAssist.getRemoteSql("CG2_isCharNo_Query",operRemoteSql);//��ȡ���ؽ��sql���
				sprintf(operszSql,operRemoteSql,cg2_username);
				operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);//��ѯIP
				if(dbid==1)
				{
					operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,result);
				}
				else
				{
					operAssist.CG2_Proc_Query(cg2_serverip,5,operszSql,result);
				}
				if(cg2_charno>0&&StrToInt(result)<0)
				{
					iResult=3;
				}
				else
				{
					for(int i = 1;i<=13;i++)
					{
						char sql_type[40];
						sprintf(sql_type,"CG2_IsExists_Account_Qurey%i",i);
						operAssist.getRemoteSql(sql_type,operRemoteSql);//��ȡ���ؽ��sql���
						sprintf(operszSql,operRemoteSql,cg2_username,cg2_usersn);
						operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);//��ѯIP
						if(dbid==1)
						{
							operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,result);
						}
						else
						{
							operAssist.CG2_Proc_Query(cg2_serverip,5,operszSql,result);
						}
						if(StrToInt(result)==0)
						{
							sprintf(sql_type,"CG2_Recovery_Account_Qurey%i",i);
							operAssist.getRemoteSql(sql_type,operRemoteSql);//��ȡ���ؽ��sql���
							sprintf(operszSql,operRemoteSql,cg2_username,cg2_usersn);
							operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);//��ѯIP
							if(dbid==1)
							{
								iResult = iResult+ DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//��������ݿ��ѯ�õ�������
							}
							else
							{
								iResult = iResult+DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//��������ݿ��ѯ�õ�������
							}
						}
					}
				}
				if(iResult==13)
				{
					sprintf(result,"�˺�%s�ָ���ɫ%s�ɹ�",cg2_username,cg2_usersn);
					operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Recovery_Account_Qurey_Resp, result);
					operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
					logFile.WriteDBLog(userByID,"ħ������2","CG2_Recovery_Account",cg2_serverip,result);
				}
				else
				{
					sprintf(result,"�˺�%s�ָ���ɫ%sʧ��",cg2_username,cg2_usersn);
					operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Recovery_Account_Qurey_Resp, result);
					operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
					logFile.WriteDBLog(userByID,"ħ������2","CG2_Recovery_Account",cg2_serverip,result);
				}
				
			}
			else
			{
				iResult=2;
				sprintf(result,"�˺�%s��ɫ��%i���н�ɫ",cg2_username,cg2_charno);
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Recovery_Account_Qurey_Resp, result);
				operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
				logFile.WriteDBLog(userByID,"ħ������2","CG2_Recovery_Account",cg2_serverip,result);
			}
			


		}
		else
		{
			sprintf(result,"%s","����ʧ��");
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Recovery_Account_Qurey_Resp, result);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_Recovery_Account",cg2_serverip,result);
		}
	}
	catch (...)
	{
		
	}
	return true;
}


//��Ұ�����
bool CCG2Info::CG2_LoveTree_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"�ʺ�<%s>��ѯ����<%s>���<%s>������",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("ħ������2",strLog);
			
		operAssist.getRemoteSql("CG2_LOVETREE_QUERY",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_LoveTree_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		sprintf(operszSql,operRemoteSql,"%Y-%m-%d %H:%m:%s",cg2_usersn);
		int tmpLength = 0;
		if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
		{
			tmpLength =DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//��������ݿ��ѯ�õ�������
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//��������ݿ��ѯ�õ�������

		}
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_LoveTree_Query_Resp, &DBVect, tmpLength);

			
		
		
	}
	catch (...)
	{
		
	}
	return true;
}



//��һ������(����)
bool CCG2Info::CG2_Marriage_Info_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"�ʺ�<%s>��ѯ����<%s>���<%s>�������",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("ħ������2",strLog);
		
		operAssist.getRemoteSql("CG2_MARRIAGE_INFO_QUERY",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Marriage_Info_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		sprintf(operszSql,operRemoteSql,"%Y-%m-%d %H:%m:%s",cg2_usersn);
		int tmpLength = 0;
		
		DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, 0, 0);//��������ݿ��ѯ�õ�������
		DBVect1.OperVectorMain("CG2",cg2_serverip, operszSql, 5, 0, 0);//��������ݿ��ѯ�õ�������
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Marriage_Info_Query_Resp, &DBVect, &DBVect1,index,iPageSize);

		
		
		
		
	}
	catch (...)
	{
		
	}
	return true;
}
//������Ϣ(����)
bool CCG2Info::CG2_SearchGuild_Query()
{
	try
	{

		sprintf(strLog,"�ʺ�<%s>��ѯ����<%s>������Ϣ",m_UserName,cg2_serverip);
		logFile.WriteText("ħ������2",strLog);
		
		operAssist.getRemoteSql("CG2_SEARCHGUILD_QUERY",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_SearchGuild_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		sprintf(operszSql,operRemoteSql,"\%",cg2_guildname,"\%");//��ʽ��sql��䣬ע��ת���
		int tmpLength = 0;
		
		DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, 0, 0);//��������ݿ��ѯ�õ�������
		DBVect1.OperVectorMain("CG2",cg2_serverip, operszSql, 5, 0, 0);//��������ݿ��ѯ�õ�������
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_SearchGuild_Query_Resp, &DBVect, &DBVect1,index,iPageSize);
			
	}
	catch (...)
	{
		
	}
	return true;


}


//�����Ա��Ϣ(����)
bool CCG2Info::CG2_SearchGuild_Detail_Query()
{
	try
	{

		sprintf(strLog,"�ʺ�<%s>��ѯ����<%s>�����Ա��Ϣ",m_UserName,cg2_serverip);
		logFile.WriteText("ħ������2",strLog);
		
		operAssist.getRemoteSql("CG2_SEARCHGUILDDETAIL_QUERY",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_SearchGuild_Detail_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		sprintf(operszSql,operRemoteSql,cg2_guildid);//��ʽ��sql��䣬ע��ת���
		int tmpLength = 0;
		logFile.WriteText("fa",operszSql);
		
		DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, 0, 0);//��������ݿ��ѯ�õ�������
		DBVect1.OperVectorMain("CG2",cg2_serverip, operszSql, 5, 0, 0);//��������ݿ��ѯ�õ�������
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_SearchGuild_Detail_Query_Resp, &DBVect, &DBVect1,index,iPageSize);
		
	}
	catch (...)
	{
		
	}
	return true;
	
	
}



//��ɢ����
bool CCG2Info::CG2_CloseGuild_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_SkillLevel_Update");//����������д����־�ļ�
		
		int result=0;

		char UserSkillLevel[50];
		ZeroMemory(UserSkillLevel,50);
		sprintf(UserSkillLevel,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SkillLevel,0)).lpdata);//��ȡ�û����ܵȼ�

		char SkillID[20];
		ZeroMemory(SkillID,20);
		sprintf(SkillID,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SkillID,0)).lpdata);//��ȡ�û�����id

		char remark[256];
		ZeroMemory(remark,256);
		sprintf(remark,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Remark,0)).lpdata);///��ע////update skill level

		//GetConfigInfo(ServerIP,DBUser,DBUserOper,DBPwd,DBName,DBNameSec,CharSet);//��������ļ���Ϣ

		operAssist.getRemoteSql("CG2_SkillLevel_Update",operRemoteSql);//��ȡ�޸���ҽ�ɫְҵsql���


		//sprintf(szSql,remotesql,curTime,UserName,charNo,UserSkillLevel,remark,UserSN);//��ʽ��sql���
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,SkillID,UserSkillLevel,remark,cg2_usersn);//��ʽ��sql���

		//MakeRemoteSql((char *)&ServerIP,(char*)&DBUser,(char*)&DBPwd,(char*)&DBName, (char*)&CharSet, remoteConnectSql);
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		int dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		int tmpLength=0;
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)//�������ڵ�һ�����ݿ���
		{
			//����Ե�һ�����ݿ���в�����sql���
			//	MakeRemoteSql((char *)&ServerIP,(char*)&DBUserOper,(char*)&DBPwd,(char*)&DBName, (char*)&CharSet, remoteConnectSql);
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		}
		else if(dbid==2)//�������ڵڶ������ݿ���
		{
			//����Եڶ������ݿ���в�����sql���
			//MakeRemoteSql((char *)&ServerIP,(char*)&DBUserOper,(char*)&DBPwd,(char*)&DBNameSec, (char*)&CharSet, remoteConnectSql);
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,11);
		}

		if(tmpLength>0)//�����ɹ�
		{							
			operAssist.getRemoteSql("CG2_UpdateChar_Query",operRemoteSql);//��ȡ���뵽���ݿ��е�sql���
			sprintf(operszSql,operRemoteSql,cg2_username,cg2_charno,cg2_usersn,remark,0,cg2_serverip,result);//��ʽ��sql���
			tmpLength=DBVect.OperVectorMain(logdbip,operszSql,0);
			if(tmpLength>0)//�����ɹ�
			{
				//maple add
					Sleep(30000);

					operAssist.getRemoteSql("CG2_Return_Result",operRemoteSql);//��ȡ���ؽ��sql���
					sprintf(operszSql,operRemoteSql,curTime,cg2_username,SkillID,UserSkillLevel);//��ʽ��sql��䣬��ǰʱ�䣬����ʺţ�����

					char gm_result[2];
					ZeroMemory(gm_result,2);
					operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
					if(dbid==1)//�������ڵ�һ�����ݿ���
					{
						//����Ե�һ�����ݿ���в�����sql���
						operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,gm_result);
					}
					else if(dbid==2)//�������ڵڶ������ݿ���
					{
						//����Եڶ������ݿ���в�����sql���
						operAssist.CG2_Proc_Query(cg2_serverip,5,operszSql,gm_result);
					}
					
							

					if(!strcmp(gm_result,"2"))
					{
						GetPrivateProfileString("CG2","Update_SkillLevel_Fail",NULL,RetMessage,30,filepath);//���浥��������Ϣ
						operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_CloseGuild_Query_Resp,RetMessage);//������Ϣ
						char LogContent[128];
						ZeroMemory(LogContent,128);
						//д����־�ļ�
						sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill Level %s Fail",curTime,m_UserName,cg2_username);
						logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
					}
					else
					{
						GetPrivateProfileString("CG2","Update_SkillLevel_Success",NULL,RetMessage,30,filepath);//���浥��������Ϣ
						operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_CloseGuild_Query_Resp,RetMessage);//������Ϣ
						char LogContent[128];
						ZeroMemory(LogContent,128);
						//д����־�ļ�
						sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill Level %s Success",curTime,m_UserName,cg2_username);
						logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
					}
				}
			}
			else
			{
				GetPrivateProfileString("CG2","Update_CharJob_Fail",NULL,RetMessage,30,filepath);//���浥��������Ϣ
				operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_CloseGuild_Query_Resp,RetMessage);//���ط�ͣ���ʧ�ܵ���Ϣ
				char LogContent[128];
				ZeroMemory(LogContent,128);
				//д����־�ļ�
				sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Job %s Fail",curTime,m_UserName,cg2_username);
				logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
			}
		}
		catch(...)
		{
				operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_CloseGuild_Query_Resp,"ERROR");//�����޸�ְҵ������Ϣ
				char LogContent[128];
				ZeroMemory(LogContent,128);
				//д����־�ļ�
				sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill %s Fail",curTime,m_UserName,cg2_username);
				logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
		}
		return true;
	
}

//�޸Ĺ�������
bool CCG2Info::CG2_RenameGuild_Query()
{
		try
		{
			logFile.WriteLog("Concerto Gate",m_UserName,"CG2_RenameGuild_Query");//����������д����־�ļ�
		
			int result=0;

			operAssist.getRemoteSql("CG2_GUILDNAME_QUERY",operRemoteSql);//��ȡ���ؽ��sql���
			sprintf(operszSql,operRemoteSql,cg2_guildid);
			char gm_result[40];
			ZeroMemory(gm_result,40);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,gm_result);

			char guildname[256];
			ZeroMemory(guildname,256);
			sprintf(guildname,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_GuildName,0)).lpdata);///�¹�����

			char pame[256];
			sprintf(pame,"%i %s",cg2_guildid,guildname);
			operAssist.getRemoteSql("CG2_RENAMEGUILD_Query",operRemoteSql);//��ȡ�޸���ҽ�ɫְҵsql���
		
	

			int tmpLength=0;
			sprintf(operszSql,operRemoteSql,curTime,"",0,7,pame,0,"","");//��ʽ��sql���
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
			if(tmpLength>0)//�����ɹ�
			{							
				Sleep(30000);
				sprintf(RetMessage,"����%s �޸ĳ��¹�����%s�ɹ�",gm_result,cg2_guildname);							
				operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_RenameGuild_Query_Resp,RetMessage);//������Ϣ
				char LogContent[128];
				ZeroMemory(LogContent,128);
				//д����־�ļ�
				sprintf(LogContent,"%s,ħ������2,����%s�޸ĳ��¹�����%s�ɹ�",curTime,gm_result,cg2_guildname);
				logFile.WriteDBLog(userByID,"ħ������2","CG2_RenameGuild_Query",cg2_serverip,LogContent);
				
			}
			else
			{
				sprintf(RetMessage,"����%s �޸ĳ��¹�����%sʧ��",gm_result,cg2_guildname);	
				operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_RenameGuild_Query_Resp,RetMessage);//������Ϣ
				char LogContent[128];
				ZeroMemory(LogContent,128);
				//д����־�ļ�
				sprintf(LogContent,"%s,ħ������2,����%s�޸ĳ��¹�����%sʧ��",curTime,gm_result,cg2_guildname);
				logFile.WriteDBLog(userByID,"ħ������2","CG2_RenameGuild_Query",cg2_serverip,LogContent);


			}
		}
		catch(...)
		{
			
				operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_RenameGuild_Query_Resp,"ERROR");//�����޸�ְҵ������Ϣ
				char LogContent[128];
				ZeroMemory(LogContent,128);
				//д����־�ļ�
				sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill %s Fail",curTime,m_UserName,cg2_username);
				logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
		}
		return true;
	
}

//����ȼ��޸�
bool CCG2Info::CG2_UpdateGuildLv_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_UpdateGuildLv_Query");//����������д����־�ļ�
		
		int result=0;
		
		operAssist.getRemoteSql("CG2_GUILDNAME_QUERY",operRemoteSql);//��ȡ���ؽ��sql���
		sprintf(operszSql,operRemoteSql,cg2_guildid);
		char gm_result[40];
		ZeroMemory(gm_result,40);
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,gm_result);
		
		char newguildlevel[256];
		ZeroMemory(newguildlevel,256);
		sprintf(newguildlevel,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_GuildRank,0)).lpdata);///�¹�����
		
		char pame[256];
		sprintf(pame,"%i %s",cg2_guildid,newguildlevel);
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);//��ȡ�޸���ҽ�ɫְҵsql���
		
		
		
		int tmpLength=0;
		sprintf(operszSql,operRemoteSql,curTime,"",0,28,pame,0,"","");//��ʽ��sql���
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		if(tmpLength>0)//�����ɹ�
		{							
			Sleep(30000);
			sprintf(RetMessage,"����%s�޸ĳ��µȼ�%s�ɹ�",gm_result,newguildlevel);							
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UpdateGuildLv_Query_Resp,RetMessage);//������Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//д����־�ļ�
			sprintf(LogContent,"%s,ħ������2,����%s�޸ĳ��¹�����%s�ɹ�",curTime,gm_result,cg2_guildname);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_RenameGuild_Query",cg2_serverip,LogContent);
			
		}
		else
		{
			sprintf(RetMessage,"����%s �޸ĳ��µȼ�%sʧ��",gm_result,newguildlevel);	
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UpdateGuildLv_Query_Resp,RetMessage);//������Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//д����־�ļ�
			sprintf(LogContent,"%s,ħ������2,����%s�޸ĳ��¹�����%sʧ��",curTime,gm_result,cg2_guildname);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_RenameGuild_Query",cg2_serverip,LogContent);
			
			
		}
	}
	catch(...)
	{
		
		operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_RenameGuild_Query_Resp,"ERROR");//�����޸�ְҵ������Ϣ
		char LogContent[128];
		ZeroMemory(LogContent,128);
		//д����־�ļ�
		sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill %s Fail",curTime,m_UserName,cg2_username);
		logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
	}
	return true;
	
}

//���������޸�
bool CCG2Info::CG2_UpdateGuildHonor_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_UpdateGuildLv_Query");//����������д����־�ļ�
		
		int result=0;
		
		operAssist.getRemoteSql("CG2_GUILDNAME_QUERY",operRemoteSql);//��ȡ���ؽ��sql���
		sprintf(operszSql,operRemoteSql,cg2_guildid);
		char gm_result[40];
		ZeroMemory(gm_result,40);
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,gm_result);
		
		char newguildlevel[256];
		ZeroMemory(newguildlevel,256);
		sprintf(newguildlevel,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_GuildRank,0)).lpdata);///�¹�����
		
		char pame[256];
		sprintf(pame,"%i %s",cg2_guildid,newguildlevel);
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);//��ȡ�޸���ҽ�ɫְҵsql���
		
		
		
		int tmpLength=0;
		sprintf(operszSql,operRemoteSql,curTime,"",0,28,pame,0,"","");//��ʽ��sql���
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		if(tmpLength>0)//�����ɹ�
		{							
			Sleep(30000);
			sprintf(RetMessage,"����%s�޸ĳ��µȼ�%s�ɹ�",gm_result,newguildlevel);							
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UpdateGuildLv_Query_Resp,RetMessage);//������Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//д����־�ļ�
			sprintf(LogContent,"%s,ħ������2,����%s�޸ĳ��¹�����%s�ɹ�",curTime,gm_result,cg2_guildname);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_RenameGuild_Query",cg2_serverip,LogContent);
			
		}
		else
		{
			sprintf(RetMessage,"����%s �޸ĳ��µȼ�%sʧ��",gm_result,newguildlevel);	
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UpdateGuildLv_Query_Resp,RetMessage);//������Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//д����־�ļ�
			sprintf(LogContent,"%s,ħ������2,����%s�޸ĳ��¹�����%sʧ��",curTime,gm_result,cg2_guildname);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_RenameGuild_Query",cg2_serverip,LogContent);
			
			
		}
	}
	catch(...)
	{
		
		operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_RenameGuild_Query_Resp,"ERROR");//�����޸�ְҵ������Ϣ
		char LogContent[128];
		ZeroMemory(LogContent,128);
		//д����־�ļ�
		sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill %s Fail",curTime,m_UserName,cg2_username);
		logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
	}
	return true;
	
}


//��ѯ����������Ϣ
bool CCG2Info::CG2_PackItem_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"�ʺ�<%s>��ѯ����<%s>���<%s>����������Ϣ",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("ħ������2",strLog);
		
		operAssist.getRemoteSql("CG2_PACKITEM_QUERY",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_PackItem_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		sprintf(operszSql,operRemoteSql,cg2_username,cg2_usersn);
		int tmpLength = 0;
		if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
		{
			tmpLength =DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//��������ݿ��ѯ�õ�������
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//��������ݿ��ѯ�õ�������
			
		}
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_PackItem_Query_Resp, &DBVect, tmpLength);
		
		
		
		
	}
	catch (...)
	{
		
	}
	return true;
}
//ɾ����ɫ����������
bool CCG2Info::CG2_BagItem_Del_Query()
{
	try
	{
		int tmpLength=0;
		int dbid=0;
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_BagItem_Del_Query");//����������д����־�ļ�
		
		char strResult[256];
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid=operAssist.GetDBID(cg2_serverip,cg2_username);
		
		
		char pame[40];
		char itemname[40];
		
		sprintf(pame,"%s %s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SlotNum,0)).lpdata,
			(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetItemID,0)).lpdata);
		sprintf(itemname,"%s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemName,0)).lpdata);
		
		
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);	
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,27,pame,0,"",cg2_usersn);//��ʽ��sql���
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//��������ݿ��ѯ�õ�������
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//��������ݿ��ѯ�õ�������
		}
		if(tmpLength>0)
		{
			Sleep(30000);
			sprintf(strResult,"�˺�%s��ɫ��%sɾ����ɫ����������:����%sɾ������%s�ɹ�",cg2_username,cg2_usersn,
				(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SlotNum,0)).lpdata,
				itemname);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BagItem_Del_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_Recovery_Account",cg2_serverip,strResult);
			
			
		}
		else
		{
			sprintf(strResult,"�˺�%s��ɫ��%sɾ����ɫ����������:����%sɾ������%sʧ��",cg2_username,cg2_usersn,
				(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SlotNum,0)).lpdata,
				itemname);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BagItem_Del_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_Recovery_Account",cg2_serverip,strResult);
		}
		
	}
	catch (...)
	{
		
	}
	
	return true;

}


//��ɫ��ʱ������Ϣ
bool CCG2Info::CG2_TempItem_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"�ʺ�<%s>��ѯ����<%s>���<%s>��ɫ��ʱ������Ϣ",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("ħ������2",strLog);
		
		operAssist.getRemoteSql("CG2_TEMPITEM_QUERY",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_TempItem_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		sprintf(operszSql,operRemoteSql,cg2_usersn);
		int tmpLength = 0;
		if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
		{
			tmpLength =DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//��������ݿ��ѯ�õ�������
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//��������ݿ��ѯ�õ�������
			
		}
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_TempItem_Query_Resp, &DBVect, tmpLength);
		
		
		
		
	}
	catch (...)
	{
		
	}
	return true;
}
//ɾ����ʱ����������
bool CCG2Info::CG2_TmpItem_Del_Query()
{
	try
	{
		int tmpLength=0;
		int dbid=0;
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_TmpItem_Del_Query");//����������д����־�ļ�
		
		char strResult[256];
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid=operAssist.GetDBID(cg2_serverip,cg2_username);
		
		
		char pame[40];
		char itemname[40];
		
		sprintf(pame,"%s %s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SlotNum,0)).lpdata,
			(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetItemID,0)).lpdata);
		sprintf(itemname,"%s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemName,0)).lpdata);
		
		
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);	
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,31,pame,0,"",cg2_usersn);//��ʽ��sql���
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//��������ݿ��ѯ�õ�������
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//��������ݿ��ѯ�õ�������
		}
		if(tmpLength>0)
		{
			Sleep(30000);
			sprintf(strResult,"�˺�%s��ɫ��%sɾ����ʱ����������:����%sɾ������%s�ɹ�",cg2_username,cg2_usersn,
				(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SlotNum,0)).lpdata,
				itemname);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_TmpItem_Del_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_Recovery_Account",cg2_serverip,strResult);
			
			
		}
		else
		{
			sprintf(strResult,"�˺�%s��ɫ��%sɾ����ʱ����������:����%sɾ������%sʧ��",cg2_username,cg2_usersn,
				(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SlotNum,0)).lpdata,
				itemname);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_TmpItem_Del_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_Recovery_Account",cg2_serverip,strResult);
		}
		
	}
	catch (...)
	{
		
	}
	
	return true;

}
//����װ����Ϣ
bool CCG2Info::CG2_CharItem_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"�ʺ�<%s>��ѯ����<%s>���<%s>����װ����Ϣ",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("ħ������2",strLog);
		
		operAssist.getRemoteSql("CG2_CHARITEM_QUERY",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_CharItem_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		sprintf(operszSql,operRemoteSql,cg2_usersn);
		int tmpLength = 0;
		if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
		{
			tmpLength =DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//��������ݿ��ѯ�õ�������
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//��������ݿ��ѯ�õ�������
			
		}

		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_CharItem_Query_Resp, &DBVect, tmpLength);
		
		
		
		
	}
	catch (...)
	{
		
	}
	return true;
}

//ɾ������װ������
bool CCG2Info::CG2_EquipItem_Del_Query()
{
	try
	{
		int tmpLength=0;
		int dbid=0;
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_EquipItem_Del_Query");//����������д����־�ļ�
		
		char strResult[256];
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid=operAssist.GetDBID(cg2_serverip,cg2_username);
		
		
		char pame[40];
		char itemname[40];
		
		sprintf(pame,"%s %s %s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SlotNum,0)).lpdata,
			(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemColNum,0)).lpdata,
			(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetItemID,0)).lpdata);
		sprintf(itemname,"%s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemName,0)).lpdata);
		
		
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);	
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,32,pame,0,"",cg2_usersn);//��ʽ��sql���
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//��������ݿ��ѯ�õ�������
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//��������ݿ��ѯ�õ�������
		}
		if(tmpLength>0)
		{
			Sleep(30000);
			sprintf(strResult,"�˺�%s��ɫ��%sɾ������װ������:����%s��Ʒ��%sɾ������%s�ɹ�",cg2_username,cg2_usersn,
				(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SlotNum,0)).lpdata,
				(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemColNum,0)).lpdata,
				itemname);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_EquipItem_Del_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_Recovery_Account",cg2_serverip,strResult);
			
			
		}
		else
		{
			sprintf(strResult,"�˺�%s��ɫ��%sɾ������װ������:����%s��Ʒ��%sɾ������%sʧ��",cg2_username,cg2_usersn,
				(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SlotNum,0)).lpdata,
				(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemColNum,0)).lpdata,
				itemname);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_EquipItem_Del_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_Recovery_Account",cg2_serverip,strResult);
		}
		
	}
	catch (...)
	{
		
	}
	
	return true;

}
//�շѵ��߲�ѯ
bool CCG2Info::CG2_ShopItem_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"�ʺ�<%s>��ѯ����<%s>���<%s>�շѵ��߲�ѯ",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("ħ������2",strLog);
		
		operAssist.getRemoteSql("CG2_SHOPITEM_QUERY",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_ShopItem_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		sprintf(operszSql,operRemoteSql,cg2_username);
		int tmpLength = 0;
		if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
		{
			tmpLength =DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//��������ݿ��ѯ�õ�������
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//��������ݿ��ѯ�õ�������
			
		}
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_ShopItem_Query_Resp, &DBVect, tmpLength);
		
		
		
		
	}
	catch (...)
	{
		
	}
	return true;
}

//�����������
bool CCG2Info::CG2_PetInfo_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"�ʺ�<%s>��ѯ����<%s>���<%s>�����������",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("ħ������2",strLog);
		
		operAssist.getRemoteSql("CG2_PETINFO_QUERY",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_PetInfo_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		sprintf(operszSql,operRemoteSql,cg2_usersn);
		int tmpLength = 0;
		if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
		{
			tmpLength =DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//��������ݿ��ѯ�õ�������
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//��������ݿ��ѯ�õ�������
			
		}

		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_PetInfo_Query_Resp, &DBVect, tmpLength);
		
		
		
		
	}
	catch (...)
	{
		
	}
	return true;
}

//���＼����Ϣ
bool CCG2Info::CG2_PetSkill_Query()
{
	try
	{
		int dbid=0;
		int PetID=0;
		PetID=StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetID,0)).lpdata);//����ID
		sprintf(strLog,"�ʺ�<%s>��ѯ����<%s>���<%s>���＼����Ϣ",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("ħ������2",strLog);
		
		operAssist.getRemoteSql("CG2_PETSKILL_QUERY",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_PetSkill_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		sprintf(operszSql,operRemoteSql,cg2_usersn,PetID);
		int tmpLength = 0;
		if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
		{
			tmpLength =DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//��������ݿ��ѯ�õ�������
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//��������ݿ��ѯ�õ�������
			
		}
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_PetSkill_Query_Resp, &DBVect, tmpLength);
		
		
		
		
	}
	catch (...)
	{
		
	}
	return true;
}

//���д����Ϣ
bool CCG2Info::CG2_BankInfo_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"�ʺ�<%s>��ѯ����<%s>���<%s>���д����Ϣ",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("ħ������2",strLog);
		
		operAssist.getRemoteSql("CG2_BANKINFO_Query",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BankInfo_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		sprintf(operszSql,operRemoteSql,cg2_username);
		int tmpLength = 0;
		if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
		{
			tmpLength =DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//��������ݿ��ѯ�õ�������
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//��������ݿ��ѯ�õ�������
			
		}
		
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BankInfo_Query_Resp, &DBVect, tmpLength);
		
		
		
		
	}
	catch (...)
	{
		
	}
	return true;
}
//�������н�Ǯ
bool CCG2Info::CG2_SetBankMoney_Query()
{
	try
	{
		operAssist.ReadValue("GameName",retMessage[0],256);
		operAssist.ReadValue("CG2_SetBankMoney_Query",retMessage[1],256);
		logFile.WriteLog(retMessage[0],m_UserName,retMessage[1]);//����������д����־�ļ�
		
		int result=0;

		char BankMoeny[256];
		ZeroMemory(BankMoeny,256);
		sprintf(BankMoeny,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Money,0)).lpdata);//��õ�������


		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);//��ȡ��ӵ���sql���
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,20,BankMoeny,0,"",cg2_usersn);//��ʽ��sql���
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		int dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		int tmpLength=0;
		if(dbid==1)//�������ڵ�һ�����ݿ���
		{
			//����Ե�һ�����ݿ���в�����sql���
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		}
		else if(dbid==2)//�������ڵڶ������ݿ���
		{
			//����Եڶ������ݿ���в�����sql���
			tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,11);
		}

		if(tmpLength>0)//�����ɹ�
		{
				//maple add///����������һ�����ݿ�
 				Sleep(30000);
					
				operAssist.getRemoteSql("CG2_getResult",operRemoteSql);//��ȡ���ؽ��sql���
				sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,20,BankMoeny,cg2_usersn);//��ʽ��sql���
				char gm_result[2];
				ZeroMemory(gm_result,2);
				operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
				operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,gm_result);
				if(!strcmp(gm_result,"2"))
				{
					GetPrivateProfileString("CG2","Add_PresentItem_Fail",NULL,RetMessage,30,filepath);//���浥��������Ϣ
					operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_SetBankMoney_Query_Resp,RetMessage);//������Ϣ
					char LogContent[128];
					ZeroMemory(LogContent,128);
					//д����־�ļ�
					sprintf(LogContent,"%s,In Game Concerto Gate,%s Add PresentItem %s Fail",curTime,m_UserName,cg2_username);
					logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query_Query",cg2_serverip,LogContent);
				}
				else
				{
					GetPrivateProfileString("CG2","CG2_SetBankMoney_Query_Success",NULL,RetMessage,30,filepath);//���浥��������Ϣ
					operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_SetBankMoney_Query_Resp,RetMessage);//������Ϣ
					char LogContent[128];
					ZeroMemory(LogContent,128);
					//д����־�ļ�
					sprintf(LogContent,"%s,In Game Concerto Gate,%s Add PresentItem %s Success",curTime,m_UserName,cg2_username);
					logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query",cg2_serverip,LogContent);
				}
		}
		else
		{
			GetPrivateProfileString("CG2","Add_PresentItem_Fail",NULL,RetMessage,30,filepath);//���浥��������Ϣ
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Item_Query_Resp,RetMessage);//������Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//д����־�ļ�
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Add Present Item %s Fail",curTime,m_UserName,cg2_username);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query",cg2_serverip,LogContent);
		}
	}
	catch(...)
	{
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Item_Query_Resp,"ERROR");//�����޸�ְҵ������Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//д����־�ļ�
			sprintf(LogContent,"%s,In Game Concerto Gate,%s Add PresentItem %s Fail",curTime,m_UserName,cg2_serverip);
			logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Item_Query",cg2_serverip,LogContent);
	}
	return true;
}
//������Ʒ��Ϣ
bool CCG2Info::CG2_BankItem_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"�ʺ�<%s>��ѯ����<%s>���<%s>������Ʒ��Ϣ",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("ħ������2",strLog);
		
		operAssist.getRemoteSql("CG2_BANKITEM_QUERY",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BankItem_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		sprintf(operszSql,operRemoteSql,cg2_username);
		int tmpLength = 0;
		if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
		{
			tmpLength =DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//��������ݿ��ѯ�õ�������
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//��������ݿ��ѯ�õ�������
			
		}
		
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BankItem_Query_Resp, &DBVect, tmpLength);
		
		
		
		
	}
	catch (...)
	{
		
	}
	return true;
}

//ɾ�����е���
bool CCG2Info::CG2_BankItem_Del_Query()
{

	try
	{
		int tmpLength=0;
		int dbid=0;
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_BagItem_Del_Query");//����������д����־�ļ�
		
		char strResult[256];
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid=operAssist.GetDBID(cg2_serverip,cg2_username);
		
		
		char pame[40];
		char itemname[40];
		
		sprintf(pame,"%s %s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SlotNum,0)).lpdata,
			(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetItemID,0)).lpdata);
		sprintf(itemname,"%s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ItemName,0)).lpdata);
		
		
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);	
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,33,pame,0,"",cg2_usersn);//��ʽ��sql���
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//��������ݿ��ѯ�õ�������
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//��������ݿ��ѯ�õ�������
		}
		if(tmpLength>0)
		{
			Sleep(30000);
			sprintf(strResult,"�˺�%s��ɫ��%sɾ�����е���:����%sɾ������%s�ɹ�",cg2_username,cg2_usersn,
				(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SlotNum,0)).lpdata,
				itemname);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BankItem_Del_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_Recovery_Account",cg2_serverip,strResult);
			
			
		}
		else
		{
			sprintf(strResult,"�˺�%s��ɫ��%sɾ�����е���:����%sɾ������%sʧ��",cg2_username,cg2_usersn,
				(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SlotNum,0)).lpdata,
				itemname);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BankItem_Del_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_Recovery_Account",cg2_serverip,strResult);
		}
		
	}
	catch (...)
	{
		
	}
	
	return true;

}

//���г�����Ϣ
bool CCG2Info::CG2_BankPet_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"�ʺ�<%s>��ѯ����<%s>���<%s>���г�����Ϣ",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("ħ������2",strLog);
		
		operAssist.getRemoteSql("CG2_BANKPET_QUERY",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BankPet_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		sprintf(operszSql,operRemoteSql,cg2_username);
		int tmpLength = 0;
		if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
		{
			tmpLength =DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//��������ݿ��ѯ�õ�������
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//��������ݿ��ѯ�õ�������
			
		}
		
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_BankPet_Query_Resp, &DBVect, tmpLength);
		
		
		
		
	}
	catch (...)
	{
		
	}
	return true;
}


//�޸ĳ�������
bool CCG2Info::CG2_Update_Pet_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_Update_Pet_Query");//����������д����־�ļ�
		
		int result=0;
		char petID[256];
		ZeroMemory(petID,256);
		sprintf(petID,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetID,0)).lpdata);
		
		char petName[256];
		ZeroMemory(petName,256);
		sprintf(petName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetSrcName,0)).lpdata);


		
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);//��ȡ�޸���ҽ�ɫְҵsql���
		int tmpLength=0;
		

		char attr0[256];
		char attr1[256];
		char attr2[256];
		char attr3[256];
		char attr4[256];
		ZeroMemory(attr0,256);
		ZeroMemory(attr1,256);
		ZeroMemory(attr2,256);
		ZeroMemory(attr3,256);
		ZeroMemory(attr4,256);
		sprintf(attr0,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetPhysical,0)).lpdata);
		sprintf(attr1,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetPower,0)).lpdata);
		sprintf(attr2,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetSmart,0)).lpdata);
		sprintf(attr3,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetSpeed,0)).lpdata);
		sprintf(attr4,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetSpirit,0)).lpdata);


		char pame[256];
		sprintf(pame,"%s vtl %s str %s dex %s qui %s mnd %s",petID,attr0,attr1,attr2,attr3,attr4);

		
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,21,pame,0,"",cg2_usersn);//��ʽ��sql���
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		if(tmpLength>0)//�����ɹ�
		{							
			Sleep(30000);
			sprintf(RetMessage,"�û���%s�޸ĳ���%s������%s,������%s,������%s,���ٶ�%s,�¾���%s�ɹ�",cg2_username,petName,attr0,attr1,attr2,attr3,attr4);							
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Update_Pet_Query_Resp,RetMessage);//������Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//д����־�ļ�
			sprintf(LogContent,"%s,ħ������2,�û���%s�޸ĳ���%s������%s,������%s,������%s,���ٶ�%s,�¾���%s�ɹ�",curTime,cg2_username,petName,attr0,attr1,attr2,attr3,attr4);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_AddPet_Query",cg2_serverip,LogContent);
			
		}
		else
		{
			sprintf(RetMessage,"�û���%s�޸ĳ���%s������%s,������%s,������%s,���ٶ�%s,�¾���%sʧ��",cg2_username,petName,attr0,attr1,attr2,attr3,attr4);	
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Update_Pet_Query_Resp,RetMessage);//������Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//д����־�ļ�
			sprintf(LogContent,"%s,ħ������2,�û���%s�޸ĳ���%s������%s,������%s,������%s,���ٶ�%s,�¾���%sʧ��",curTime,cg2_username,petName,attr0,attr1,attr2,attr3,attr4);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_AddPet_Query",cg2_serverip,LogContent);
			
			
		}
	}
	catch(...)
	{
		
		operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Update_Pet_Query_Resp,"ERROR");//�����޸�ְҵ������Ϣ
		char LogContent[128];
		ZeroMemory(LogContent,128);
		//д����־�ļ�
		sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill %s Fail",curTime,m_UserName,cg2_username);
		logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
	}
	return true;
}

//��ӳ���
bool CCG2Info::CG2_AddPet_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_AddPet_Query");//����������д����־�ļ�
		
		int result=0;
		char petID[256];
		ZeroMemory(petID,256);
		sprintf(petID,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetID,0)).lpdata);///�¹�����

		char petName[256];
		ZeroMemory(petName,256);
		sprintf(petName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetSrcName,0)).lpdata);///�¹�����

		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);//��ȡ�޸���ҽ�ɫְҵsql���
		int tmpLength=0;

		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,22,petID,0,"",cg2_usersn);//��ʽ��sql���
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		if(tmpLength>0)//�����ɹ�
		{							
			Sleep(30000);
			sprintf(RetMessage,"�û���%s��ӳ���%s�ɹ�",cg2_username,petName);							
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_AddPet_Query_Resp,RetMessage);//������Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//д����־�ļ�
			sprintf(LogContent,"%s,ħ������2,�û���%s��ӳ���%s�ɹ�",curTime,cg2_username,petName);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_AddPet_Query",cg2_serverip,LogContent);
			
		}
		else
		{
			sprintf(RetMessage,"�û���%s��ӳ���%sʧ��",cg2_username,petName);	
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_AddPet_Query_Resp,RetMessage);//������Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//д����־�ļ�
			sprintf(LogContent,"%s,ħ������2,�û���%s��ӳ���%sʧ��",curTime,cg2_username,petName);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_AddPet_Query",cg2_serverip,LogContent);
			
			
		}
	}
	catch(...)
	{
		
		operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_RenameGuild_Query_Resp,"ERROR");//�����޸�ְҵ������Ϣ
		char LogContent[128];
		ZeroMemory(LogContent,128);
		//д����־�ļ�
		sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill %s Fail",curTime,m_UserName,cg2_username);
		logFile.WriteDBLog(userByID,"Concerto Gate","CG2_SkillLevel_Update",cg2_serverip,LogContent);
	}
	return true;
}

//�޸ĳ���ȼ�
bool CCG2Info::CG2_UpdatePetLevel_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_UpdatePetLevel_Query");//����������д����־�ļ�
		int result=0;
		char petID[256];
		ZeroMemory(petID,256);
		sprintf(petID,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetID,0)).lpdata);
		
		char petName[256];
		ZeroMemory(petName,256);
		sprintf(petName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetSrcName,0)).lpdata);
		
		
		
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);//��ȡ�޸���ҽ�ɫְҵsql���
		int tmpLength=0;
		
		
		char petLevel[256];
		ZeroMemory(petLevel,256);

		sprintf(petLevel,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Level,0)).lpdata);
		
		
		char pame[256];
		sprintf(pame,"%s %s",petID,petLevel);
		
		
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,25,pame,0,"",cg2_usersn);//��ʽ��sql���
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		if(tmpLength>0)//�����ɹ�
		{							
			Sleep(30000);
			sprintf(RetMessage,"�û���%s�޸ĳ���%s�µȼ�%s�ɹ�",cg2_username,petName,petLevel);							
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UpdatePetLevel_Query_Resp,RetMessage);//������Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//д����־�ļ�
			sprintf(LogContent,"%s,ħ������2,�û���%s�޸ĳ���%s�µȼ�%s�ɹ�",curTime,cg2_username,petName,petLevel);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_AddPet_Query",cg2_serverip,LogContent);
			
		}
		else
		{
			sprintf(RetMessage,"�û���%s�޸ĳ���%s�µȼ�%sʧ��",cg2_username,petName,petLevel);	
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UpdatePetLevel_Query_Resp,RetMessage);//������Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//д����־�ļ�
			sprintf(LogContent,"%s,ħ������2,�û���%s�µȼ�%sʧ��",curTime,cg2_username,petName,petLevel);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_AddPet_Query",cg2_serverip,LogContent);
			
		}
	}
	catch(...)
	{
		
		operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UpdatePetLevel_Query_Resp,"ERROR");//�����޸�ְҵ������Ϣ
		char LogContent[128];
		ZeroMemory(LogContent,128);
		//д����־�ļ�
		sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill %s Fail",curTime,m_UserName,cg2_username);
		logFile.WriteDBLog(userByID,"Concerto Gate","CG2_UpdatePetLevel_Query",cg2_serverip,LogContent);
	}
	return true;
}
//��ӳ��＼��
bool CCG2Info::CG2_Insert_Pet_Skill_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_Insert_Pet_Skill_Query");//����������д����־�ļ�
		int result=0;
		char petID[256];
		ZeroMemory(petID,256);
		sprintf(petID,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetID,0)).lpdata);
		
		char petName[256];
		ZeroMemory(petName,256);
		sprintf(petName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetSrcName,0)).lpdata);
		
		
		
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);//��ȡ�޸���ҽ�ɫְҵsql���
		int tmpLength=0;
		
		
		

		char petSkillName[256];
		ZeroMemory(petSkillName,256);
		sprintf(petSkillName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SkillName,0)).lpdata);

		char petSkillID[256];
		ZeroMemory(petSkillID,256);

		char procName[256];
		ZeroMemory(procName,256);
		sprintf(procName,"exec CG2_SkillID_Query '%s'",petSkillName);
		
		operAssist.CG2_Proc_Query(logdbip,4,procName,petSkillID);////���ݼ������ƻ�ü���ID

		char pame[256];
		sprintf(pame,"%s %s",petID,petSkillID);
		
		
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,15,pame,0,"",cg2_usersn);//��ʽ��sql���
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		if(tmpLength>0)//�����ɹ�
		{							
			Sleep(30000);
			sprintf(RetMessage,"�û���%s��ɫ��%s��ӳ���%s�¼���%s�ɹ�",cg2_username,cg2_usersn,petName,petSkillName);							
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Pet_Skill_Query_Resp,RetMessage);//������Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//д����־�ļ�
			sprintf(LogContent,"%s,ħ������2,�û���%s��ɫ��%s��ӳ���%s�¼���%s�ɹ�",curTime,cg2_username,cg2_usersn,petName,petSkillName);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_AddPet_Query",cg2_serverip,LogContent);
			
		}
		else
		{
			sprintf(RetMessage,"�û���%s��ɫ��%s��ӳ���%s�¼���%sʧ��",cg2_username,cg2_usersn,petName,petSkillName);	
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Pet_Skill_Query_Resp,RetMessage);//������Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//д����־�ļ�
			sprintf(LogContent,"%s,ħ������2,�û���%s��ɫ��%s��ӳ���%s�¼���%sʧ��",curTime,cg2_username,cg2_usersn,petName,petSkillName);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_AddPet_Query",cg2_serverip,LogContent);
			
		}
	}
	catch(...)
	{
		
		operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Insert_Pet_Skill_Query_Resp,"ERROR");//�����޸�ְҵ������Ϣ
		char LogContent[128];
		ZeroMemory(LogContent,128);
		//д����־�ļ�
		sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill %s Fail",curTime,m_UserName,cg2_username);
		logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Pet_Skill_Query",cg2_serverip,LogContent);
	}
	return true;
}

//�޸ĳ��＼��
bool CCG2Info::CG2_Update_Pet_Skill_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_Update_Pet_Skill_Query");//����������д����־�ļ�
		int result=0;
		char petID[256];
		ZeroMemory(petID,256);
		sprintf(petID,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetID,0)).lpdata);
		
		char petName[256];
		ZeroMemory(petName,256);
		sprintf(petName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetSrcName,0)).lpdata);
		
		
		
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);//��ȡ�޸���ҽ�ɫְҵsql���
		int tmpLength=0;
		
		char petSkillName[256];
		ZeroMemory(petSkillName,256);
		sprintf(petSkillName,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SkillName,0)).lpdata);

		char petSkillLevel[256];
		ZeroMemory(petSkillLevel,256);
		sprintf(petSkillLevel,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_SkillLevel,0)).lpdata);

		char petSkillID[256];
		ZeroMemory(petSkillID,256);
		
		char procName[256];
		ZeroMemory(procName,256);
		sprintf(procName,"exec CG2_SkillID_Query '%s'",petSkillName);
		
		operAssist.CG2_Proc_Query(logdbip,4,procName,petSkillID);////���ݼ������ƻ�ü���ID
		
		char pame[256];
		sprintf(pame,"%s %s %s",petID,petSkillID,petSkillLevel);
		
		
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,17,pame,0,"",cg2_usersn);//��ʽ��sql���
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);

		int iSkillLevel=StrToInt(petSkillLevel)+1;
		if(tmpLength>0)//�����ɹ�
		{							
			Sleep(30000);
			sprintf(RetMessage,"�û���%s��ɫ��%s�޸ĳ���%s����%s�µȼ�%i�ɹ�",cg2_username,cg2_usersn,petName,petSkillName,iSkillLevel);							
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Update_Pet_Skill_Query_Resp,RetMessage);//������Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//д����־�ļ�
			sprintf(LogContent,"%s,ħ������2,�û���%s��ɫ��%s�޸ĳ���%s����%s�µȼ�%i�ɹ�",curTime,cg2_username,cg2_usersn,petName,petSkillName,iSkillLevel);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_AddPet_Query",cg2_serverip,LogContent);
			
		}
		else
		{
			sprintf(RetMessage,"�û���%s��ɫ��%s�޸ĳ���%s����%s�µȼ�%iʧ��",cg2_username,cg2_usersn,petName,petSkillName,iSkillLevel);	
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Update_Pet_Skill_Query_Resp,RetMessage);//������Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//д����־�ļ�
			sprintf(LogContent,"%s,ħ������2,�û���%s��ɫ��%s�޸ĳ���%s����%s�µȼ�%iʧ��",curTime,cg2_username,cg2_usersn,petName,petSkillName,iSkillLevel);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_AddPet_Query",cg2_serverip,LogContent);
			
		}
	}
	catch(...)
	{
		
		operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Update_Pet_Skill_Query_Resp,"ERROR");//�����޸�ְҵ������Ϣ
		char LogContent[128];
		ZeroMemory(LogContent,128);
		//д����־�ļ�
		sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill %s Fail",curTime,m_UserName,cg2_username);
		logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Pet_Skill_Query",cg2_serverip,LogContent);
	}
	return true;
}
//����ͼ����Ϣ
bool CCG2Info::CG2_PetPic_Query()
{
	try
	{
		int dbid=0;
		sprintf(strLog,"�ʺ�<%s>��ѯ����<%s>���<%s>����ͼ����Ϣ",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("ħ������2",strLog);
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		operAssist.getRemoteSql("CG2_PetPic_Query",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		if(!strcmp("",operRemoteSql))
		{
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_PetPic_Query_Resp, "���ݿ����Ҳ�����Ӧ��sql���");
			return false;
		}
		dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		sprintf(operszSql,operRemoteSql,cg2_usersn);
		int tmpLength =0;
		if(dbid==1)//����ʺ��ڵ�һ�����ݿ���
		{
			
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 3, index, iPageSize);//��������ݿ��ѯ�õ�������
			
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 5, index, iPageSize);//��������ݿ��ѯ�õ�������
		}
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_PetPic_Query_Resp, &DBVect, tmpLength);

		
		
		
	}
	catch (...)
	{
		
	}
	return true;
}
//����/ɾ������ͼ��
bool CCG2Info::CG2_PetPic_Del_Query()
{
	try
	{
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_PetPic_Del_Query");//����������д����־�ļ�
		int result=0;
		char petID[256];
		ZeroMemory(petID,256);
		sprintf(petID,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_PetID,0)).lpdata);
		
		char pictype[256];
		ZeroMemory(pictype,256);
		sprintf(pictype,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_TypeID,0)).lpdata);
		
		
		
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);//��ȡ�޸���ҽ�ɫְҵsql���
		int tmpLength=0;
		
		
		
		char pame[256];
		sprintf(pame,"%s %s",petID,pictype);
		
		
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,34,pame,0,"",cg2_usersn);//��ʽ��sql���
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		tmpLength=DBVect.OperVectorMain("CG2",cg2_serverip,operszSql,9);
		
		if(tmpLength>0)//�����ɹ�
		{							
			Sleep(30000);
			sprintf(RetMessage,"�����ɹ�");							
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_PetPic_Del_Query_Resp,RetMessage);//������Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//д����־�ļ�
		//	sprintf(LogContent,"%s,ħ������2,�û���%s��ɫ��%s�޸ĳ���%s����%s�µȼ�%i�ɹ�",curTime,cg2_username,cg2_usersn,petName,petSkillName,iSkillLevel);
		//	logFile.WriteDBLog(userByID,"ħ������2","CG2_AddPet_Query",cg2_serverip,LogContent);
			
		}
		else
		{
		//	sprintf(RetMessage,"�û���%s��ɫ��%s�޸ĳ���%s����%s�µȼ�%iʧ��",cg2_username,cg2_usersn,petName,petSkillName,iSkillLevel);	
		//	operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_PetPic_Del_Query_Resp,RetMessage);//������Ϣ
			char LogContent[128];
			ZeroMemory(LogContent,128);
			//д����־�ļ�
		//	sprintf(LogContent,"%s,ħ������2,�û���%s��ɫ��%s�޸ĳ���%s����%s�µȼ�%iʧ��",curTime,cg2_username,cg2_usersn,petName,petSkillName,iSkillLevel);
		//	logFile.WriteDBLog(userByID,"ħ������2","CG2_AddPet_Query",cg2_serverip,LogContent);
			
		}
	}
	catch(...)
	{
		
		operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_PetPic_Del_Query_Resp,"ERROR");//�����޸�ְҵ������Ϣ
		char LogContent[128];
		ZeroMemory(LogContent,128);
		//д����־�ļ�
		sprintf(LogContent,"%s,In Game Concerto Gate,%s Update Skill %s Fail",curTime,m_UserName,cg2_username);
		logFile.WriteDBLog(userByID,"Concerto Gate","CG2_Insert_Pet_Skill_Query",cg2_serverip,LogContent);
	}
	return true;


}
//�����ʱ����
bool CCG2Info::CG2_Insert_TmpPwd_Query()
{
	try
	{
		int dbid=0;
		int tmpLength=0;
		sprintf(strLog,"�ʺ�<%s>��ѯ����<%s>���<%s>�����ʱ����",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("ħ������2",strLog);
		char strResult[256];
		char RelPwd[40];
		char sign[40];
		char TempPwd[40];
		ZeroMemory(strResult,256);
		ZeroMemory(RelPwd,40);
		ZeroMemory(sign,40);
		ZeroMemory(TempPwd,40);
		//��ѯ��������
		sprintf(operszSql,"exec CG2_SearchTmpPWD_Staute_Query '%s','%s'",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ServerName,0)).lpdata,cg2_username);
		operAssist.CG2_Proc_Query(logdbip,4,operszSql,strResult);
		if(strcmp(strResult,""))//�������
		{
			sprintf(strResult,"�˺�%s��ʱ�����Ѿ�����",cg2_username);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Insert_TmpPwd_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_Recovery_Account",cg2_serverip,strResult);
		}
		else
		{
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			dbid=operAssist.GetDBID(cg2_serverip,cg2_username);
			operAssist.getRemoteSql("CG2_GetPassWord_Query",operRemoteSql);
			sprintf(operszSql,operRemoteSql,cg2_username);
			if(dbid==1)
			{
				operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,RelPwd);
			}
			else if(dbid==2)
			{
				operAssist.CG2_Proc_Query(cg2_serverip,5,operszSql,RelPwd);
			}
			
			sprintf(sign,"%s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_TempPwd,0)).lpdata);
			int newlength=0;
			MD5_EnCode(TempPwd,&newlength,sign,strlen(sign));
			
			operAssist.getRemoteSql("CG2_TmpPassWord_Query",operRemoteSql);
			sprintf(operszSql,operRemoteSql,TempPwd,cg2_username);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
			if(dbid==1)
			{
				tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//��������ݿ��ѯ�õ�������
			}
			else if(dbid==2)
			{
				tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//��������ݿ��ѯ�õ�������
			}
			if(tmpLength>0)
			{
				sprintf(operszSql,"exec CG2_InsertTmpPassWord_Query '%s','%s','%s','%s','%s','%s',%i",
					(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ServerName,0)).lpdata,
					cg2_username,
					cg2_usersn,
					RelPwd,
					TempPwd,
					sign,
					1);
				DBVect.OperVectorMain(logdbip, operszSql, 4);//��������ݿ��ѯ�õ�������

				sprintf(strResult,"�˺�%s��ʱ�������óɹ�",cg2_username);
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Insert_TmpPwd_Query_Resp, strResult);
				operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
				logFile.WriteDBLog(userByID,"ħ������2","CG2_Recovery_Account",cg2_serverip,strResult);


			}
			else
			{
				sprintf(strResult,"�˺�%s��ʱ��������ʧ��",cg2_username);
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Insert_TmpPwd_Query_Resp, strResult);
				operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
				logFile.WriteDBLog(userByID,"ħ������2","CG2_Recovery_Account",cg2_serverip,strResult);
			}
		}


	}
	catch (...)
	{
	}

	return true;
}
//�鿴��ʱ����
bool CCG2Info::CG2_SearchPassWord_Query()
{
	try
	{
		sprintf(operszSql,"exec CG2_SearchPassWord_Query '%s','%s'",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ServerName,0)).lpdata,cg2_username);
		int tmpLength=DBVect.OperVectorMain(logdbip, operszSql, 4, 0, 0);//��������ݿ��ѯ�õ�������
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_SearchPassWord_Query_Resp, &DBVect, tmpLength);

	}
	catch (...)
	{
		
	}
	return true;
}
//�ָ���ʱ����
bool CCG2Info::CG2_Recover_TmpPwd_Query()
{
	try
	{
		int dbid=0;
		int tmpLength=0;
		sprintf(strLog,"�ʺ�<%s>��ѯ����<%s>���<%s>�ָ���ʱ����",m_UserName,cg2_serverip,cg2_username);
		logFile.WriteText("ħ������2",strLog);
		char strResult[256];
		char RelPwd[40];
		char sign[40];
		char TempPwd[40];
		ZeroMemory(strResult,40);
		ZeroMemory(RelPwd,40);
		ZeroMemory(sign,40);
		ZeroMemory(TempPwd,40);
		//��ѯ��������
		sprintf(operszSql,"exec CG2_GetRelPassWord_Query '%s','%s'",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ServerName,0)).lpdata,cg2_username);
		operAssist.CG2_Proc_Query(logdbip,4,operszSql,RelPwd);
		if(!strcmp(RelPwd,""))//���벻����
		{
			sprintf(strResult,"�˺�%s��ʱ����δ����",cg2_username);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Recover_TmpPwd_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_Recovery_Account",cg2_serverip,strResult);
		}
		else
		{
			
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			dbid=operAssist.GetDBID(cg2_serverip,cg2_username);
			operAssist.getRemoteSql("CG2_TmpPassWord_Query",operRemoteSql);
			sprintf(operszSql,operRemoteSql,RelPwd,cg2_username);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
			if(dbid==1)
			{
				tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//��������ݿ��ѯ�õ�������
			}
			else if(dbid==2)
			{
				tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//��������ݿ��ѯ�õ�������
			}
			if(tmpLength>0)
			{
				sprintf(operszSql,"exec CG2_UpdateTmpPassWord_Query '%s','%s'",
					(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ServerName,0)).lpdata,cg2_username);
				DBVect.OperVectorMain(logdbip, operszSql, 4);//��������ݿ��ѯ�õ�������
				
				sprintf(strResult,"�˺�%s��ʱ����ָ��ɹ�",cg2_username);
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Recover_TmpPwd_Query_Resp, strResult);
				operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
				logFile.WriteDBLog(userByID,"ħ������2","CG2_Recovery_Account",cg2_serverip,strResult);
				
				
			}
			else
			{
				sprintf(strResult,"�˺�%s��ʱ����ָ�ʧ��",cg2_username);
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Recover_TmpPwd_Query_Resp, strResult);
				operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
				logFile.WriteDBLog(userByID,"ħ������2","CG2_Recovery_Account",cg2_serverip,strResult);
			}
		}
		
		
	}
	catch (...)
	{
	}
	return true;
}
//�����б���Ϣ
bool CCG2Info::CG2_PetList_Query()
{
	try
	{
		sprintf(operszSql,"exec CG2_PetList_Query");
		int tmpLength=DBVect.OperVectorMain(logdbip, operszSql, 4, 0, 0);//��������ݿ��ѯ�õ�������
		operAssist.ReturnData(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_PetList_Query_Resp, &DBVect, tmpLength);

	}
	catch (...)
	{
		
	}
	return true;
}

//��ɫ��λ
bool CCG2Info::CG2_ChangeCharNo_Query()
{
	try
	{
		int tmpLength=0;
		int dbid=0;
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_ChangeCharNo_Query");//����������д����־�ļ�
		
		int iResult=0;
		char strResult[256];
		//�ָ���ɫ֮ǰ����
		bool b_result=CG2_KickAccount_Query_NoAnswer();//����
		if(b_result)
		{
			Sleep(30000);//��ʱ30��ȴ����
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			dbid=operAssist.GetDBID(cg2_serverip,cg2_username);
			operAssist.getRemoteSql("CG2_ChangeCharNo_Query",operRemoteSql);

			int cg2_newcharno=StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_ChangeCharNo,0)).lpdata);
			sprintf(operszSql,operRemoteSql,cg2_username,cg2_charno,cg2_newcharno);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
			if(dbid==1)
			{
				tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//��������ݿ��ѯ�õ�������
			}
			else if(dbid==2)
			{
				tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//��������ݿ��ѯ�õ�������
			}
			if(tmpLength>0)
			{
							
				sprintf(strResult,"�˺�%s��ɫ��λ�ɹ�",cg2_username);
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_ChangeCharNo_Query_Resp, strResult);
				operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
				logFile.WriteDBLog(userByID,"ħ������2","CG2_Recovery_Account",cg2_serverip,strResult);
				
				
			}
			else
			{
				sprintf(strResult,"�˺�%s��ɫ��λʧ��",cg2_username);
				operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_ChangeCharNo_Query_Resp, strResult);
				operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
				logFile.WriteDBLog(userByID,"ħ������2","CG2_Recovery_Account",cg2_serverip,strResult);
			}

			
		}
	}
	catch (...)
	{
		
	}
	
	return true;
}
//�ƶ���ɫ����
bool CCG2Info::CG2_ChangeMap_Query()
{
	try
	{
		int tmpLength=0;
		int dbid=0;
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_ChangeMap_Query");//����������д����־�ļ�
		
		int iResult=0;
		char strResult[256];
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid=operAssist.GetDBID(cg2_serverip,cg2_username);
		
		
		char pame[40];
		char MapX[40];
		char MapY[40];
		char mapCoord[40];
		char MapID[40];
		sprintf(MapID,"%s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_MapID,0)).lpdata);
		sprintf(mapCoord,"%s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Coordinate,0)).lpdata);
		char *lpStr=strtok((char *)&mapCoord,",");//��ѯ��־","
		sprintf(MapX,lpStr);
		lpStr=strtok(NULL,",");
		sprintf(MapY,lpStr);
		if(StrToInt(MapX)>0)
		{
		}
		else
		{
			sprintf(operszSql,"exec CG2_SearchMapID_Query '%s'",MapID);
			operAssist.CG2_Proc_Query(logdbip,4,operszSql,mapCoord);
			lpStr=strtok(mapCoord,",");//��ѯ��־","
			sprintf(MapX,lpStr);
			lpStr=strtok(NULL,",");
			sprintf(MapY,lpStr);
		}

		sprintf(pame,"%s %s %s",MapID,MapX,MapY);

		
		
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);	
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,0,pame,0,"",cg2_usersn);//��ʽ��sql���
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//��������ݿ��ѯ�õ�������
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//��������ݿ��ѯ�õ�������
		}
		if(tmpLength>0)
		{
			Sleep(30000);
			sprintf(strResult,"�˺�%s�ƶ���ɫ����ɹ�",cg2_username);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Change_Map_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_Recovery_Account",cg2_serverip,strResult);
			
				
		}
		else
		{
			sprintf(strResult,"�˺�%s�ƶ���ɫ����ʧ��",cg2_username);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_Change_Map_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_Recovery_Account",cg2_serverip,strResult);
		}
			
	}
	catch (...)
	{
		
	}
	
	return true;
}

//��ѯ��ͼ�б�
bool CCG2Info::CG2_MapList_Query()
{
	try
	{
		sprintf(operszSql,"exec CG2_MapList_Query");
		int tmpLength=DBVect.OperVectorMain(logdbip, operszSql, 4, 0, 0);//��������ݿ��ѯ�õ�������
		operAssist.ReturnData(CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_MapList_Query_Resp, DBVect.getDes(),tmpLength);
	}
	catch (...)
	{
		
	}
	return true;
}

//ģ����ѯ��ͼ
bool CCG2Info::CG2_SearchMap_Query()
{
	try
	{
		sprintf(operszSql,"exec CG2_SearchMap_Query '%s'",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_MapName,0)).lpdata);
		int tmpLength=DBVect.OperVectorMain(logdbip, operszSql, 4, 0, 0);//��������ݿ��ѯ�õ�������
		operAssist.ReturnData(CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_SearchMap_Query_Resp, DBVect.getDes(),tmpLength);
	}
	catch (...)
	{
		
	}
	return true;
}

//�����ʴ���Ϣ
bool CCG2Info::CG2_SendAsk_Query()
{
	try
	{
		int tmpLength=0;
		int dbid=0;
		logFile.WriteLog("Concerto Gate",m_UserName,"CG2_SendAsk_Query");//����������д����־�ļ�
		
		char strResult[256];
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		dbid=operAssist.GetDBID(cg2_serverip,cg2_username);
		
				
		operAssist.getRemoteSql("CG2_Insert_Query",operRemoteSql);	
		sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,8,"",0,"",cg2_usersn);//��ʽ��sql���
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,1);
		if(dbid==1)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 9);//��������ݿ��ѯ�õ�������
		}
		else if(dbid==2)
		{
			tmpLength = DBVect.OperVectorMain("CG2",cg2_serverip, operszSql, 11);//��������ݿ��ѯ�õ�������
		}
		if(tmpLength>0)
		{
			Sleep(30000);
			sprintf(strResult,"�˺�%s��ɫ��%s�����ʴ���Ϣ�ɹ�",cg2_username,cg2_usersn);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_SendAsk_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_Recovery_Account",cg2_serverip,strResult);
			
			
		}
		else
		{
			sprintf(strResult,"�˺�%s��ɫ��%s�����ʴ���Ϣʧ��",cg2_username,cg2_usersn);
			operAssist.ReturnMessage(1, CEnumCore::Msg_Category::CG2_ADMIN, CEnumCore::ServiceKey::CG2_SendAsk_Query_Resp, strResult);
			operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
			logFile.WriteDBLog(userByID,"ħ������2","CG2_Recovery_Account",cg2_serverip,strResult);
		}
		
	}
	catch (...)
	{
		
	}
	
	return true;
	
}


//�̳���Ϣ��ѯ
bool CCG2Info::CG2_UserShoppingInfo_Query()
{
	try
	{
		char begintime[256];
		char lbegintime[256];
		char endtime[256];
		char lendtime[256];
		char strsign[1024];
		char strMD5sign[1024];
		char strItemName[40];
		char procName[256];
		int iTypeID=1;
		
		sprintf(begintime,"%s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_StartTime,0)).lpdata);
		sprintf(endtime,"%s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_EndTime,0)).lpdata);
		iTypeID=StrToInt((LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_TypeID,0)).lpdata);
		struct tm t_BeginTime; //	tm�ṹ�����͵ı���������ʱ��
		sscanf(begintime,"%d-%d-%d %d:%d:%d",&t_BeginTime.tm_year,&t_BeginTime.tm_mon,&t_BeginTime.tm_mday,&t_BeginTime.tm_hour,&t_BeginTime.tm_min,&t_BeginTime.tm_sec);//����ʼʱ�丳ֵ��tm�ṹ��
		sprintf(lbegintime,"%02d%02d%02d%02d%02d%02d",t_BeginTime.tm_year,t_BeginTime.tm_mon,t_BeginTime.tm_mday,t_BeginTime.tm_hour,t_BeginTime.tm_min,t_BeginTime.tm_sec);
		sscanf(endtime,"%d-%d-%d %d:%d:%d",&t_BeginTime.tm_year,&t_BeginTime.tm_mon,&t_BeginTime.tm_mday,&t_BeginTime.tm_hour,&t_BeginTime.tm_min,&t_BeginTime.tm_sec);//����ʼʱ�丳ֵ��tm�ṹ��
		sprintf(lendtime,"%02d%02d%02d%02d%02d%02d",t_BeginTime.tm_year,t_BeginTime.tm_mon,t_BeginTime.tm_mday,t_BeginTime.tm_hour,t_BeginTime.tm_min,t_BeginTime.tm_sec);
		
		sprintf(strsign,"%s%i%s%s%s%s",cg2_username,iTypeID,lbegintime,lendtime,"1","|Tbds6f.AS_getbuylog_CG2");
		
		int newlength=0;
		ZeroMemory(strMD5sign,1024);
		MD5_EnCode(strMD5sign,&newlength,strsign,strlen(strsign));
		
		char url[256];
		char postdata[2048];
		ZeroMemory(postdata,2048);

		sprintf(url,"%s","http://61.152.150.158/PayCenter/AS_getbuylog_CG2.php");//�̳�IP�̶�
		sprintf(postdata,"username=%s&utp=%i&log_start=%s&log_end=%s&datetype=1&sign=%s&encoding=UTF-8",cg2_username,iTypeID,lbegintime,lendtime,strMD5sign);
		
		vector<CGlobalStruct::DataRecord> ret_vector;
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;
		int iCow=0,iCol=0;
	
		if(operAssist.ReadXml("POST",url,postdata,&ret_vector,&iCow,&iCol))
		{
			COperVector sendopervector;
			sendopervector.InitCreate(iCow+1,iCol+3);
			
			CEnumCore::TagName m_TagName;//�����õ�TagName
			CEnumCore::TagFormat m_TagFormat;//�����õ�TagFormat
			
			CMyEvent m_Event;//��ȡCEvent����
			
			//��ȡ��ѯ�ļ�¼����ÿһ��
				
			for( iter = ret_vector.begin(); iter != ret_vector.end(); iter++ )
			{
				m_DataRecord = *iter;
				if(!strcmp(m_DataRecord.recordfieldname,"PKEY"))
				{
					
				}
				else if(!strcmp(m_DataRecord.recordfieldname,"PCODE"))
				{
					sprintf(procName,"exec CG2_ItemName_query %i",StrToInt((char *)&m_DataRecord.recorddata));//��ʼ���洢���̵����
					operAssist.CG2_Proc_Query(logdbip,4,procName,strItemName);
					sendopervector.AddTFLV(CEnumCore::TagName::CG2_ItemName, CEnumCore::TagFormat::TLV_STRING, strlen(strItemName), (unsigned char*)&strItemName);//����TFLV,����ӵ�vector��
				}
				else
				{
					m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//��ȡ��¼�����ݵ�TagName
					m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//��ȡ��¼�����ݵ�TagFormat
					sendopervector.AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//����TFLV,����ӵ�vector��
				}
				
			}
			
			operAssist.ReturnData(m_hServerSocket,&sendopervector,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UserShoppingInfo_Query_Resp,index,iPageSize);
			sendopervector.Destroy();

		}
		else
		{
			iter = ret_vector.begin();
			m_DataRecord = *iter;
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_UserShoppingInfo_Query_Resp,m_DataRecord.recorddata);
		}
	
		
		if(!ret_vector.empty())
		{
			ret_vector.clear();
		}
		

	}
	catch (...)
	{
		
	}
	return true;
}

//���͵���������
bool CCG2Info::CG2_Send_ItemPassword()
{
	try
	{
		char strRecv[256];
		ZeroMemory(strRecv,256);
		sprintf(strRecv,"%s",(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::CG2_Email,0)).lpdata);
		char strPasswd[256];
		operAssist.FindDBIP(cg2_serverip,cg2_serverip,2);
		int dbid = operAssist.GetDBID(cg2_serverip,cg2_username);//��ȡ����ʺ����������ݿ�ID
		operAssist.getRemoteSql("CG2_getItemPass",operRemoteSql);
		sprintf(operszSql,operRemoteSql,cg2_username);
		if(dbid==1)
		{
			operAssist.CG2_Proc_Query(cg2_serverip,3,operszSql,strPasswd);
		}
		else if(dbid==1)
		{
			operAssist.CG2_Proc_Query(cg2_serverip,5,operszSql,strPasswd);
		}
		




		char strMessage[1024];
		ZeroMemory(strMessage,1024);
		operAssist.ReadValue("CG2_Body",strMessage,1024);
		char tmpcontext[256];
		char context[2048];
		sprintf(tmpcontext,"����ħ������II������������%s",strPasswd);
		sprintf(context,strMessage,cg2_username,tmpcontext);
		if(operAssist.SendEmail(strRecv,context))
		{
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Send_ItemPassword_Resp,"���ͳɹ�");
		}
		else
		{
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Send_ItemPassword_Resp,"����ʧ��");

		}

		
	}
	catch (...)
	{
		
	}
	return true;
}

//�ȵ�Email��ַ
bool CCG2Info::CG2_Get_EmailAddress()
{
	try
	{

		char strsign[1024];
		char strMD5sign[1024];
		char strItemName[40];
		char procName[256];
		int iTypeID=1;
		
		
		sprintf(strsign,"%s%s",cg2_username,"QUXUEXIBA");
		
		int newlength=0;
		ZeroMemory(strMD5sign,1024);
		MD5_EnCode(strMD5sign,&newlength,strsign,strlen(strsign));
		
		char url[256];
		

		sprintf(url,"http://222.73.121.86:8080/user?req=getUserInfo&id=GMTOOLS&userid=%s&usertype=0&type=userinfo&s=%s",cg2_username,strMD5sign);//�̳�IP�̶�

		
		vector<CGlobalStruct::DataRecord> ret_vector;
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;
		int iCow=0,iCol=0;
		
		if(operAssist.ReadXmlGet("Get",url,"",&ret_vector,&iCow,&iCol))
		{
			COperVector sendopervector;
			sendopervector.InitCreate(iCow+1,iCol+3);
			
			CEnumCore::TagName m_TagName;//�����õ�TagName
			CEnumCore::TagFormat m_TagFormat;//�����õ�TagFormat
			
			CMyEvent m_Event;//��ȡCEvent����
			
			//��ȡ��ѯ�ļ�¼����ÿһ��
			
			for( iter = ret_vector.begin(); iter != ret_vector.end(); iter++ )
			{
				m_DataRecord = *iter;
				if(!strcmp(m_DataRecord.recordfieldname,"PKEY"))
				{
					
				}
				else if(!strcmp(m_DataRecord.recordfieldname,"PCODE"))
				{
					sprintf(procName,"exec CG2_ItemName_query %i",StrToInt((char *)&m_DataRecord.recorddata));//��ʼ���洢���̵����
					operAssist.CG2_Proc_Query(logdbip,4,procName,strItemName);
					sendopervector.AddTFLV(CEnumCore::TagName::CG2_ItemName, CEnumCore::TagFormat::TLV_STRING, strlen(strItemName), (unsigned char*)&strItemName);//����TFLV,����ӵ�vector��
				}
				else
				{
					m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//��ȡ��¼�����ݵ�TagName
					m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//��ȡ��¼�����ݵ�TagFormat
					sendopervector.AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//����TFLV,����ӵ�vector��
				}
				
			}
			
			operAssist.ReturnData(m_hServerSocket,&sendopervector,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Get_EmailAddress_Resp,index,iPageSize);
			sendopervector.Destroy();
			
		}
		else
		{
			iter = ret_vector.begin();
			m_DataRecord = *iter;
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Get_EmailAddress_Resp,m_DataRecord.recorddata);
		}
		
		
		if(!ret_vector.empty())
		{
			ret_vector.clear();
		}
		
		
	}
	catch (...)
	{
		
	}
	return true;
}
////�˺ż����ѯ
bool CCG2Info::CG2_Account_Active_Query()
{

	try
	{

		char strsign[1024];
		char strMD5sign[1024];
		sprintf(strsign,"%s%s",cg2_username,"|T4pb3A.Querycg2User");
		
		int newlength=0;
		ZeroMemory(strMD5sign,1024);
		MD5_EnCode(strMD5sign,&newlength,strsign,strlen(strsign));
		
		char url[256];
		char postdata[2048];
		ZeroMemory(postdata,2048);
		
		sprintf(url,"%s","http://61.152.150.158/PayCenter/Query_cg2User.php");//�̳�IP�̶�
		sprintf(postdata,"getuser=%s&sign=%s&encoding=UTF-8",cg2_username,strMD5sign);
		
		vector<CGlobalStruct::DataRecord> ret_vector;
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;
		int iCow=0,iCol=0;
		
		if(operAssist.ReadXml("POST",url,postdata,&ret_vector,&iCow,&iCol))
		{
			COperVector sendopervector;
			sendopervector.InitCreate(iCow+1,iCol+3);
			
			CEnumCore::TagName m_TagName;//�����õ�TagName
			CEnumCore::TagFormat m_TagFormat;//�����õ�TagFormat
			
			CMyEvent m_Event;//��ȡCEvent����
			
			//��ȡ��ѯ�ļ�¼����ÿһ��
			
			for( iter = ret_vector.begin(); iter != ret_vector.end(); iter++ )
			{
				m_DataRecord = *iter;
				if(!strcmp(m_DataRecord.recordfieldname,"gold"))
				{
					if(!strcmp(m_DataRecord.recorddata,""))
					{
						sprintf(m_DataRecord.recorddata,"%s","0");
					}
					m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//��ȡ��¼�����ݵ�TagName
					m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//��ȡ��¼�����ݵ�TagFormat
					sendopervector.AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//����TFLV,����ӵ�vector��

				}
				else 
				{
					m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//��ȡ��¼�����ݵ�TagName
					m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//��ȡ��¼�����ݵ�TagFormat
					sendopervector.AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//����TFLV,����ӵ�vector��
				}
				
			}
			
			operAssist.ReturnData(m_hServerSocket,&sendopervector,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Account_Active_Query_Resp,index,iPageSize);
			sendopervector.Destroy();
			
		}
		else
		{
			iter = ret_vector.begin();
			m_DataRecord = *iter;
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Account_Active_Query_Resp,m_DataRecord.recorddata);
		}
		
		
		if(!ret_vector.empty())
		{
			ret_vector.clear();
		}
		
		
	}
	catch (...)
	{
		
	}
	return true;
}

//ͨ���������ѯ
bool CCG2Info::CG2_Sn_Active_Query()
{
	try
	{
		
		char strsign[1024];
		char strMD5sign[1024];
		sprintf(strsign,"%s%s",cg2_username,"|T4pb5A.Querycg2Code");
		
		int newlength=0;
		ZeroMemory(strMD5sign,1024);
		MD5_EnCode(strMD5sign,&newlength,strsign,strlen(strsign));
		
		char url[256];
		char postdata[2048];
		ZeroMemory(postdata,2048);
		
		sprintf(url,"%s","http://61.152.150.158/PayCenter/Querycg2Code.php");//�̳�IP�̶�
		sprintf(postdata,"getcode=%s&sign=%s&encoding=UTF-8",cg2_username,strMD5sign);
		
		vector<CGlobalStruct::DataRecord> ret_vector;
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;
		int iCow=0,iCol=0;
		
		if(operAssist.ReadXml("POST",url,postdata,&ret_vector,&iCow,&iCol))
		{
			COperVector sendopervector;
			sendopervector.InitCreate(iCow+1,iCol+3);
			
			CEnumCore::TagName m_TagName;//�����õ�TagName
			CEnumCore::TagFormat m_TagFormat;//�����õ�TagFormat
			
			CMyEvent m_Event;//��ȡCEvent����
			
			//��ȡ��ѯ�ļ�¼����ÿһ��
			
			for( iter = ret_vector.begin(); iter != ret_vector.end(); iter++ )
			{
				m_DataRecord = *iter;
				if(!strcmp(m_DataRecord.recordfieldname,"gold"))
				{
					if(!strcmp(m_DataRecord.recorddata,""))
					{
						sprintf(m_DataRecord.recorddata,"%s","0");
					}
					m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//��ȡ��¼�����ݵ�TagName
					m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//��ȡ��¼�����ݵ�TagFormat
					sendopervector.AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//����TFLV,����ӵ�vector��
					
				}
				else 
				{
					m_TagName = m_Event.GetTagName((char* )&m_DataRecord.recordfieldname);//��ȡ��¼�����ݵ�TagName
					m_TagFormat = m_Event.GetTagFormat((char* )&m_DataRecord.recordfieldtype);//��ȡ��¼�����ݵ�TagFormat
					sendopervector.AddTFLV(m_TagName, m_TagFormat, strlen(m_DataRecord.recorddata), (unsigned char*)&m_DataRecord.recorddata);//����TFLV,����ӵ�vector��
				}
				
			}
			
			operAssist.ReturnData(m_hServerSocket,&sendopervector,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Sn_Active_Query_Resp,index,iPageSize);
			sendopervector.Destroy();
			
		}
		else
		{
			iter = ret_vector.begin();
			m_DataRecord = *iter;
			operAssist.ReturnMessage(1,CEnumCore::Msg_Category::CG2_ADMIN,CEnumCore::ServiceKey::CG2_Sn_Active_Query_Resp,m_DataRecord.recorddata);
		}
		
		
		if(!ret_vector.empty())
		{
			ret_vector.clear();
		}
		
		
	}
	catch (...)
	{
		
	}
	return true;
}

int CCG2Info::CG2_GMInsert_Query(char *ServerIP, char *UserName,char * UserNick,char * strEndTime,char * Reason,char * memo,int iFunc)
{

	//sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,1,strBanTime,0,BanContent,cg2_usersn);//��ʽ��sql�����ɫ
	//sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,24,strBanTime,0,BanContent,cg2_usersn);//��ʽ��sql�����˺�
	//sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,2,"",0,UnbindContent,cg2_usersn);//��ʽ��sql�����ɫ
	//sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,24,"0",0,UnbindContent,"");//��ʽ��sql�����˺�
	//sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,24,"",0,"",cg2_usersn);//��ʽ��sql������ʺ�
	//sprintf(operszSql,operRemoteSql,curTime,cg2_username,cg2_charno,1,"",0,"",cg2_usersn);//��ʽ��sql����߽�ɫ
	char curTime[256];
	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);//��ȡϵͳ��ǰʱ��
	sprintf(curTime,"%i-%i-%i %i:%i:%i",nowtime.wYear,nowtime.wMonth,nowtime.wDay,nowtime.wHour,nowtime.wMinute,nowtime.wSecond);//����ǰʱ�丳ֵ
	char usersn[40];
	char charno[40];

	int tmpLength=0;
	operAssist.initialize();
	operAssist.FindDBIP(ServerIP,ServerIP,2);
	int  dbid = operAssist.GetDBID(ServerIP,UserName);//��ȡ����ʺ����������ݿ�ID
	if(!strcmp(memo,"1"))//�˺�
	{
		if(dbid==1)//�������ڵ�һ�����ݿ���
		{
			sprintf(operszSql,"select IDKey from tbl_charBase where cdkey='%s'",UserName);
			operAssist.CG2_Proc_Query(ServerIP,3,operszSql,usersn);
			sprintf(operszSql,"select charno from tbl_charBase where cdkey='%s'",UserName);
			operAssist.CG2_Proc_Query(ServerIP,3,operszSql,charno);
			
			
		}
		else if(dbid==2)//�������ڵڶ������ݿ���
		{
			sprintf(operszSql,"select IDKey from tbl_charBase where cdkey='%s' ",UserName);
			operAssist.CG2_Proc_Query(ServerIP,5,operszSql,usersn);
			sprintf(operszSql,"select charno from tbl_charBase where cdkey='%s' ",UserName);
			operAssist.CG2_Proc_Query(ServerIP,5,operszSql,charno);
		}
	}
	else
	{
		if(dbid==1)//�������ڵ�һ�����ݿ���
		{
			sprintf(operszSql,"select IDKey from tbl_charBase where cdkey='%s' and name='%s'",UserName,UserNick);
			operAssist.CG2_Proc_Query(ServerIP,3,operszSql,usersn);
			sprintf(operszSql,"select charno from tbl_charBase where cdkey='%s' and name='%s'",UserName,UserNick);
			operAssist.CG2_Proc_Query(ServerIP,3,operszSql,charno);
			
			
		}
		else if(dbid==2)//�������ڵڶ������ݿ���
		{
			sprintf(operszSql,"select IDKey from tbl_charBase where cdkey='%s' and name='%s'",UserName,UserNick);
			operAssist.CG2_Proc_Query(ServerIP,5,operszSql,usersn);
			sprintf(operszSql,"select charno from tbl_charBase where cdkey='%s' and name='%s'",UserName,UserNick);
			operAssist.CG2_Proc_Query(ServerIP,5,operszSql,charno);
		}
	}
	
	


	try
	{
		operAssist.getRemoteSql("CG2_GMInsert_Query",operRemoteSql);//�������IP��ѯ��¼��¼��sql���
		switch(iFunc)
		{
		case 0://����
			{
				if(!strcmp(memo,"1"))//�˺�
				{

					sprintf(operszSql,operRemoteSql,curTime,UserName,charno,24,"",0,"",usersn);
				}
				else
				{
					sprintf(operszSql,operRemoteSql,curTime,UserName,charno,1,"",0,"",usersn);//��ʽ��sql����߽�ɫ
				}
			}
			break;
		case 1://��ͣ
			{
				if(!strcmp(memo,"1"))//�˺�
				{
					sprintf(operszSql,operRemoteSql,curTime,UserName,charno,24,strEndTime,0,Reason,usersn);
				}
				else
				{
					sprintf(operszSql,operRemoteSql,curTime,UserName,charno,1,strEndTime,0,Reason,usersn);
				}
			}
			break;
		case 2://���
			{
				if(!strcmp(memo,"1"))//�˺�
				{
					sprintf(operszSql,operRemoteSql,curTime,UserName,charno,24,"0",0,Reason,"");
				}
				else
				{
					sprintf(operszSql,operRemoteSql,curTime,UserName,charno,2,"",0,Reason,usersn);
				}
			}
			break;
		default:
			break;
		}
		

		operAssist.FindDBIP(ServerIP,ServerIP,1);
		if(dbid==1)//�������ڵ�һ�����ݿ���
		{
			tmpLength = DBVect.OperVectorMain("CG2",ServerIP, operszSql, 9);//ִ��Զ�����ݲ���
			
		}
		else if(dbid==2)//�������ڵڶ������ݿ���
		{
			tmpLength = DBVect.OperVectorMain("CG2",ServerIP, operszSql, 11);//ִ��Զ�����ݲ���
		}
		
	}
	catch(...)
	{
	}
	return tmpLength;
}
