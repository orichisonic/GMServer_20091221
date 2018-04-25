xin// OperPal.cpp: implementation of the COperPal class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "OperPal.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COperPal::COperPal()
{
}

COperPal::~COperPal()
{
}
//====================================================================================================
// ����
//====================================================================================================
// ���߳ɹ�-------------------------------------------------------------------------------------------
void COperPal::OnConnect(int iSockID, long lParam)
{
	printf("Connect Success\n");
}

// ����-----------------------------------------------------------------------------------------------
void COperPal::OnDisconnect(int iSockID, long lParam)
{
	printf("Disconnect from GMServer\n");
}

// ����ʧ��-------------------------------------------------------------------------------------------
void COperPal::OnConnectfailed(int iConnectID, long lParam)
{
	printf("Connect Failed\n");
}

bool COperPal::ReadValue(char * strKey,char * strValue,int strLength)
{
	return logFile.ReadValue("M_PAL","PAL",strKey,strValue,strLength);
}

void COperPal::GetLogSendNum(int* m_log,int* m_send)
{
	char path[256];
	GetCurrentDirectory(256,path);
	char filepath[256];
	sprintf(filepath,"%s%s",path,".\\config\\PalServer.ini");
	
	*m_log=GetPrivateProfileInt("REMOTE","Login_num",0,filepath);
	*m_send=GetPrivateProfileInt("REMOTE","Send_num",0,filepath);
}
void COperPal::GetUserNamePwd(char * title,char * m_UserName,char * m_pwd,int * m_port)
{
	//////////////////�������ļ�////////////////////////
	char path[256];
	GetCurrentDirectory(256,path);
	char filepath[256];
	wsprintf(filepath,"%s%s",path,"\\config\\PalServer.ini");
	
	
	GetPrivateProfileString(title,"szAccount",NULL,m_UserName,32,filepath);
	GetPrivateProfileString(title,"szPassword",NULL,m_pwd,32,filepath);
	* m_port=GetPrivateProfileInt(title,"iGMServerPort",9991,filepath);
	
}
int COperPal::FindWorldID(char * ServerName,char * ServerIP)
{
	int worldid=-1;
	try
	{
		char sql[256];
		ZeroMemory(sql,256);
		getRemoteSql("PAL","PAL_FindWorldID", m_sql,0);//��ȡ��ѯ��sql���
		sprintf(sql, m_sql, ServerIP,ServerName);

		char m_result[256];
		QuerySingleValue("PAL","127.0.0.1",4,sql,m_result);
		if(!strcmp(m_result,""))
		{
			return -1;
		}
		else
		{
			worldid=StrToInt(m_result);
		}
	}
	catch(...)
	{
		worldid=-1;
	}
	return worldid;

}
void COperPal::FindLogdbIP(char * logdbip,char * servername)
{
	try
	{
		char sql[256];
		ZeroMemory(sql,256);
		getRemoteSql("PAL","PAL_FindLogDBIP", m_sql,0);//��ȡ��ѯLogDB��sql���
		sprintf(sql, m_sql, servername);
		QuerySingleValue("PAL","127.0.0.1",4,sql,logdbip);
	}
	catch(...)
	{
		sprintf(logdbip,"");
	}
}
void COperPal::FindGameDBIP(char * gamedbip,char * servername)
{
	try
	{
		char sql[256];
		ZeroMemory(sql,256);
		getRemoteSql("PAL","PAL_FindGameDBIP", m_sql,0);//��ȡ��ѯGameDB��sql���
		sprintf(sql, m_sql, servername);
		QuerySingleValue("PAL","127.0.0.1",4,sql,gamedbip);
	}
	catch(...)
	{
		sprintf(gamedbip,"");
	}
}
void COperPal::FindAccountDBIP(char * accountdbip,char * servername)
{
	try
	{
		char sql[256];
		ZeroMemory(sql,256);
		getRemoteSql("PAL","PAL_FindAccountDBIP", m_sql,0);//��ȡ��ѯAccountDB��sql���
		sprintf(sql, m_sql, servername);
		QuerySingleValue("PAL","127.0.0.1",4,sql,accountdbip);
	}
	catch(...)
	{
		sprintf(accountdbip,"");
	}
}
bool COperPal::BuildTLV(COperVector operVect,int iIndex, int iPageSize)
{
	CGlobalStruct::TFLV m_tflv;
	if(iIndex==0&&iPageSize==0)
	{
		m_tflv.nIndex=operVect.getAllTFLV()+1;//���ݵ�Ԫ��������
		m_tflv.m_tagName=CEnumCore::TagName::PageCount;//���ݵ�Ԫ��tagName
		m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_INTEGER;//���ݵ�Ԫ��tagFormat
		m_tflv.m_tvlength=sizeof(int);//���ݵ�Ԫ���ֽ���
		sprintf((LPSTR)&m_tflv.lpdata,"%i",1);//�����ݵ�Ԫ���浽���ݰ���
		operVect.GetInnerList().push_back(m_tflv);
		COperatorGame::BuildTLV((COperVector)operVect.GetInnerList());
	}
	else
	{

		int PageCount=0;
		char strInt[10];
		int alltflv=operVect.getAllTFLV();
		int querynum=0;//����
		int colnum = (operVect.getTLVByTag((operVect.getTLVBynIndex(1)).m_tagName,1)).nIndex-1;//����
		if(colnum<0)
		{
			colnum=alltflv;
			querynum=1;
		}
		else
		{
			querynum=alltflv/colnum;
		}
		
		if(querynum%iPageSize==0)
		{
			PageCount=querynum/iPageSize;
		}
		else
		{
			PageCount=querynum/iPageSize+1;
		}


		SendVectBuf.InitCreate(iPageSize,colnum+2);//��ʼ�����͵�����
		m_tflv=operVect.getTLVBynIndex(1);
		SendVectBuf.AddTFLV(m_tflv);
		for(int i=0; i < alltflv; i++)
		{
			m_tflv= operVect.getTLVBynIndex(i+1);//��ȡÿһ��TFLV
			if(i>(iIndex-1)*iPageSize*colnum&&i<=iIndex*iPageSize*colnum)
			{

				SendVectBuf.AddTFLV(m_tflv);
				if((i+1)%colnum==0)
				{
					_itoa(PageCount,strInt,10);
					SendVectBuf.AddTFLV(CEnumCore::TagName::PageCount,CEnumCore::TagFormat::TLV_INTEGER,sizeof(int),(unsigned char *)&strInt);
				}
			}
		}
		if(querynum=1)
		{
			_itoa(PageCount,strInt,10);
			SendVectBuf.AddTFLV(CEnumCore::TagName::PageCount,CEnumCore::TagFormat::TLV_INTEGER,sizeof(int),(unsigned char *)&strInt);
		}
	}
	iEnbodyLength = SendVectBuf.EnBody();

	return true;
}
bool COperPal::BuildTLV(char * message)
{
	return COperatorGame::BuildTLV(message);
}
bool COperPal::BuildTLV(COperVector DBVect)
{
	return COperatorGame::BuildTLV(DBVect);
}
//
void COperPal::GetPoint(char * lpGMServerName,char * accountname, int *point)
{

	try
	{
		char accountdb[20];
		char szRemoteData[256];
		char szSql[2048];
		char szRemoteSql[2048];
		
		FindAccountDBIP(accountdb,lpGMServerName);
		getRemoteSql("PAL","PAL_ROLENAMETOPOINT",szRemoteSql,0);
		
		sprintf(szSql,szRemoteSql,accountname);

		char m_result[256];
		QuerySingleValue("PAL",accountdb,0,szSql,m_result);
		if(!strcmp(m_result,""))
		{
			* point=-1;
			return;
		}
		else
		{
			* point=StrToInt(m_result);
		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("PAL","��ȡԪ�����ڴ治��");
		* point=-1;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("PAL","��ȡԪ�����ļ��쳣");
		* point=-1;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("PAL","��ȡԪ���������쳣");
		* point=-1;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("PAL",(char *)err.what());
		* point=-1;
	}
	catch(...)
	{
		* point=-1;
	}
}

void COperPal::RecordData(int UserByID,CEnumCore::Message_Tag_ID m_Message_Tag_ID,char * lpServerName,COperVector * pSocketData,char * lpReason)
{
			char text[256];
			char szData[256];
			char szSql[256];
			ZeroMemory(text,256);
			ZeroMemory(szData,256);
			ZeroMemory(szSql,256);
			switch(m_Message_Tag_ID)
			{
			case CEnumCore::Message_Tag_ID::PAL_BANISHPLAYER://����
				{

					sprintf(text,"%s%s",pSocketData->getTLVByTag(CEnumCore::TagName::MESSAGE,0).lpdata,lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",
						UserByID,"PAL_BANISHPLAYER",lpServerName,text);
				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_MODIFYGUILDNAME://�޸Ĺ�������
				{
					sprintf(text,"%s,����Ա����:%s",pSocketData->getTLVByTag(CEnumCore::TagName::MESSAGE,0).lpdata,lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_MODIFYGUILDNAME",lpServerName,text);
				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_MODIFYGUILDMEMBERPL://�޸Ĺ����Աְ��
				{
					sprintf(text,"%s,����Ա����:%s",pSocketData->getTLVByTag(CEnumCore::TagName::MESSAGE,0).lpdata,lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_MODIFYGUILDMEMBERPL",lpServerName,text);
				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_MODIFYCHARACTERQUEST://�޸Ľ�ɫ����
				{
					sprintf(text,"%s%s",pSocketData->getTLVByTag(CEnumCore::TagName::MESSAGE,0).lpdata,lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_MODIFYCHARACTERQUEST",lpServerName,text);

				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_DISMISSGUILD://��ɢ���
				{
					sprintf(text,"%s,����Ա����:%s",pSocketData->getTLVByTag(CEnumCore::TagName::MESSAGE,0).lpdata,lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_DISMISSGUILD",lpServerName,text);
				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_SENDGIFTMAIL://�����ʼ�
				{
					sprintf(text,"%s%s",pSocketData->getTLVByTag(CEnumCore::TagName::MESSAGE,0).lpdata,lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_SENDGIFTMAIL",lpServerName,text);
				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_MODIFYGUILDLV://�޸İ��ȼ�
				{
					sprintf(text,"%s,����Ա����:%s",pSocketData->getTLVByTag(CEnumCore::TagName::MESSAGE,0).lpdata,lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_MODIFYGUILDLV",lpServerName,text);
				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTBANKPW://�޸Ľ�ɫ��������
				{
					sprintf(text,"%s%s",pSocketData->getTLVByTag(CEnumCore::TagName::MESSAGE,0).lpdata,lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_MODIFYACCOUNTBANKPW",lpServerName,text);
				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTBANKPW://��ԭ��������
				{
					sprintf(text,"%s,����Ա����:%s",pSocketData->getTLVByTag(CEnumCore::TagName::MESSAGE,0).lpdata,lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_RECALLACCOUNTBANKPW",lpServerName,text);
				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_MODIFYCHARACTERDROPPW://�޸�ɾ����ɫ����
				{
					sprintf(text,"%s,����Ա����:%s",pSocketData->getTLVByTag(CEnumCore::TagName::MESSAGE,0).lpdata,lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_MODIFYCHARACTERDROPPW",lpServerName,text);
				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_RECALLCHARACTERDROPPW://��ԭɾ����ɫ����
				{
					sprintf(text,"%s,����Ա����:%s",pSocketData->getTLVByTag(CEnumCore::TagName::MESSAGE,0).lpdata,lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_RECALLCHARACTERDROPPW",lpServerName,text);
				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_MODIFYCHARACTERMONEY://�޸Ľ�ɫ��Ǯ
				{
					sprintf(text,"%s,����Ա����:%s",pSocketData->getTLVByTag(CEnumCore::TagName::MESSAGE,0).lpdata,lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_MODIFYCHARACTERMONEY",lpServerName,text);
				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_DELCHARACTERITEM://ɾ����ɫ����
				{
					sprintf(text,"%s��������%s",pSocketData->getTLVByTag(CEnumCore::TagName::MESSAGE,0).lpdata,lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_DELCHARACTERITEM",lpServerName,text);
				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_MODIFYGUILDNOTICE://�޸Ĺ��ṫ��
				{
					sprintf(text,"%s,����Ա����:%s",pSocketData->getTLVByTag(CEnumCore::TagName::MESSAGE,0).lpdata,lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_MODIFYGUILDNOTICE",lpServerName,text);
				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPW://�޸��˺����� 
				{
					sprintf(text,"%s",lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_MODIFYACCOUNTPW",lpServerName,text);
				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTPW://��ԭ�˺����� 
				{
					sprintf(text,"%s",lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_RECALLACCOUNTPW",lpServerName,text);
				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_MODIFYSECONDPW://�޸Ķ������� 
				{
					sprintf(text,"%s",lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_MODIFYSECONDPW",lpServerName,text);
				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_RECALLSECONDPW://��ԭ��������
				{
					sprintf(text,"%s",lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_RECALLSECONDPW",lpServerName,text);
				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_USESAFELOCK://������ȫ��
				{
					sprintf(text,"%s",lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_USESAFELOCK",lpServerName,text);
				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_FREESAFELOCK://������ȫ��
				{
					sprintf(text,"%s",lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_FREESAFELOCK",lpServerName,text);
				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_MODIFYPUZZLEMAP://�޸�ͼ��
				{
					sprintf(text,"%s%s",pSocketData->getTLVByTag(CEnumCore::TagName::MESSAGE,0).lpdata,lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_MODIFYPUZZLEMAP",lpServerName,text);
				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_DELCHARACTERMOB://ɾ����ħ
				{
					sprintf(text,"%s%s",pSocketData->getTLVByTag(CEnumCore::TagName::MESSAGE,0).lpdata,lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_DELCHARACTERMOB",lpServerName,text);
				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_MOVECHARACTERTOSAFE://�ƶ�����ȫ��
				{
					sprintf(text,"%s%s",pSocketData->getTLVByTag(CEnumCore::TagName::MESSAGE,0).lpdata,lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_MOVECHARACTERTOSAFE",lpServerName,text);
				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_SENDCUSTOMITEMMAIL://���Ϳ�����Ʒ�ʼ�
				{
					sprintf(text,"%s%s",pSocketData->getTLVByTag(CEnumCore::TagName::MESSAGE,0).lpdata,lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_SENDCUSTOMITEMMAIL",lpServerName,text);
				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_MODIFYACCOUNTPWALL_RESP://�޸����д�������
				{
					sprintf(text,"%s", lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_MODIFYACCOUNTPWALL",lpServerName,text);
				}
				break;
			case CEnumCore::Message_Tag_ID::PAL_RECALLACCOUNTPWALL_RESP://�ָ����д�������
				{
					sprintf(text, "%s", lpReason);
					sprintf(szSql,"exec Pal_GMTOOLSLOG_INSERT %i,'%s','%s','%s'",UserByID,
						"PAL_RECALLACCOUNTPWALL",lpServerName,text);
				}
				break;
			default:
				break;

			}
			m_SqlHelper.SqlHelperMain("127.0.0.1", szSql, 4);//��ȡSqlExpress�е���Ӧsql���

}

void COperPal::GiftIDToName(char * m_name,int m_id)
{
	try
	{
		if(m_id!=-1)
		{
			char m_sql[1024];
			sprintf(m_sql,"exec Pal_GiftIdToName %i",m_id);
			QuerySingleValue("PAL","127.0.0.1",4,m_sql,m_name);
		}
		else
		{
			sprintf(m_name,"%s","��");
		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("PAL","����IDת�����ڴ治��");
		sprintf(m_name,"��");
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("PAL","����IDת�����ļ��쳣");
		sprintf(m_name,"��");
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("PAL","����IDת���������쳣");
		sprintf(m_name,"��");
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("PAL",(char *)err.what());
		sprintf(m_name,"��");
	}
	
}
//�ʺ��ҽ�ɫ��
int COperPal::AccountToRoleName(S_ROLENAME* s_rolename,char* lpGMServerName,char* accoutnname)
{
	int i=0;
	try
	{
		char logdbip[20];
		char szSql[2048];
		char szRemoteSql[2048];
		ZeroMemory(szRemoteSql,2048);
		ZeroMemory(szSql,2048);
		ZeroMemory(logdbip,20);
		FindLogdbIP(logdbip,lpGMServerName);
		getRemoteSql("PAL","PAL_ACCOUNTTOROLENAME",szRemoteSql,0);
		
		sprintf(szSql,szRemoteSql,accoutnname);

		vector<CGlobalStruct::DataRecord> datarecord;
		
		m_SqlHelper.SqlHelperMain("PAL",&datarecord,logdbip, szSql, 1, 0, 0);//��ȡSqlExpress�е���Ӧsql���

		if(datarecord.empty())//�����¼Ϊ��
		{
			return 0;
		}
		//��ȡ��ѯ�ļ�¼����ÿһ��
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("rolename", m_DataRecord.recordfieldname))//��ȡSqlExpress�ж�Ӧsql_type��sql_statement
			{
				s_rolename[i].rolenameid=i+1;
				sprintf(s_rolename[i].rolename,m_DataRecord.recorddata);
				i=i+1;
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("PAL","�ʺ��ҽ�ɫ���ڴ治��");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("PAL","�ʺ��ҽ�ɫ���ļ��쳣");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("PAL","�ʺ��ҽ�ɫ�������쳣");
		return false;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("PAL",(char *)err.what());
		return false;
	}
	return i;
}

bool COperPal::SendEmail(char * lpRecv,char *lpContent,char *lptmpcontent)
{
	try
	{
		//maple add
		char strMailServer[256];
		ZeroMemory(strMailServer,256);
		char strMailPort[256];
		ZeroMemory(strMailPort,256);
		char strPalSender[256];
		ZeroMemory(strPalSender,256);
		char strPalXMailer[256];
		ZeroMemory(strPalXMailer,256);
		//char strPalSubject[256];
		//ZeroMemory(strPalSubject,256);
		

		/*ReadValue("Mail_Server",retMessage[0],256);//Mail_Server=127.0.0.1
		ReadValue("Mail_Port",retMessage[1],256);//Mail_Port=25
		ReadValue("Pal_Sender",retMessage[2],256);//CG2_Sender=zhubin8265@hotmail.com
		ReadValue("Pal_XMailer",retMessage[3],256);//CG2_XMailer=zb
		ReadValue("Pal_Subject",retMessage[4],256);//CG2_Subject=���͵�������*/
		ReadValue("Mail_Server",strMailServer,256);
		ReadValue("Mail_Port",strMailPort,256);
		ReadValue("Pal_Sender",strPalSender,256);
		ReadValue("Pal_XMailer",strPalXMailer,256);
		//ReadValue("Pal_Subject",strPalSubject,256);

		SYSTEMTIME nowtime;
		GetLocalTime(&nowtime);//��ȡϵͳ��ǰʱ��
		char strHeader[40];
		ZeroMemory(strHeader,40);
		/*char strDate[128];
		ZeroMemory(strDate,128);
		sprintf(strDate,"%d-%d-%d %d:%d:%d",nowtime.wYear,nowtime.wMonth,nowtime.wDay,nowtime.wHour,nowtime.wMinute,nowtime.wMinute);
		sprintf(strHeader,
			"Date: %s\r\n"\
			"From: %s\r\n"\
			"To: %s\r\n"\
			"Subject: %s\r\n"\
			"X-Mailer: <%s>\r\n\r\n",
			strDate,
			strPalSender,lpRecv,//retMessage[2]
			strPalSubject,//retMessage[4]
			strPalXMailer);//���ݷ���ͷ//retMessage[3]*/
		//maple add
		sprintf(strHeader,"send password\r\n");
		///char strPalBody1[256];
		///ZeroMemory(strPalBody1,256);
		char strPalBody2[256];
		ZeroMemory(strPalBody2,256);
		char strPalBody3[256];
		ZeroMemory(strPalBody3,256);
		char strPalBody4[256];
		ZeroMemory(strPalBody4,256);
		char strPalBody5[256];
		ZeroMemory(strPalBody5,256);
		char strPalBody6[256];
		ZeroMemory(strPalBody6,256);
		char strPalBody7[256];
		ZeroMemory(strPalBody7,256);

		///ReadValue("PAL_Body1",strPalBody1,256);//retMessage[5],256);//ǰ���Ѿ���ȡ�ˣ���lpContent������
		ReadValue("PAL_Body2",strPalBody2,256);//retMessage[6],256);
		ReadValue("PAL_Body3",strPalBody3,256);//retMessage[7],256);
		ReadValue("PAL_Body4",strPalBody4,256);//retMessage[8],256);
		ReadValue("PAL_Body5",strPalBody5,256);//retMessage[9],256);
		ReadValue("PAL_Body6",strPalBody6,256);//retMessage[10],256);
		ReadValue("PAL_Body7",strPalBody7,256);//retMessage[11],256);

		//maple add
		
		//CLogic::SendMail(retMessage[0],StrToInt(retMessage[1]),lpRecv,retMessage[2],strHeader,retMessage[4],password,retMessage[6],retMessage[7],retMessage[8],retMessage[9],retMessage[10],retMessage[11],AccountOrRole);//lpContent

		//CLogic::SendMail(retMessage[0],StrToInt(retMessage[1]),lpRecv,retMessage[2],strHeader,retMessage[4],lptmpcontent,retMessage[6],retMessage[7],retMessage[8],retMessage[9],retMessage[10],retMessage[11],lpContent);//lpContent
		SendMail(strMailServer,StrToInt(strMailPort),lpRecv,strPalSender,strHeader,lptmpcontent,strPalBody2,strPalBody3,strPalBody4,strPalBody5,strPalBody6,strPalBody7,lpContent);

	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("PAL","����E-Mail�ڴ治��");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("PAL","����E-Mail�ļ��쳣");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("PAL","����E-Mail�����쳣");
		return false;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("PAL",(char *)err.what());
		return false;
	}
	return true;
}


bool COperPal::SendMail(char *lpip,int Port,char *lpRecver,char *lpSender,char *lpHeader,char *lpBody1,char *lpBody2,char *lpBody3,char *lpBody4,char *lpBody5,char *lpBody6,char *lpBody7,char *lpAccount)//,char *lpBody
{
	try
	{
		CMSocket m_Socket;
		char sendbuf[2048];//2048
		char recvbuf[512];
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,512);
		int iresult=m_Socket.ConnectURL(lpip,Port);
		m_Socket.MyReceive((unsigned char *)recvbuf);
		//printf(recvbuf);
		
		ZeroMemory(recvbuf,512);
		sprintf(sendbuf,"HELO\r\n");//maple add

		int sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		//printf(recvbuf);

		//maple add
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,512);
		sprintf(sendbuf,"EHLO\r\n");

		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		//printf(recvbuf);
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,512);
		sprintf(sendbuf,"MAIL From: <%s>\r\n",lpSender);//�ʼ�������˭

		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		//printf(recvbuf);
		
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,512);
		sprintf(sendbuf,"RCPT TO: <%s>\r\n",lpRecver);//�ʼ�����˭

		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		//printf(recvbuf);
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,512);
		// Send the DATA command
		sprintf(sendbuf,"DATA\r\n");//����
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		//printf(recvbuf);

		
		//Sleep(100);
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,512);
		sprintf(sendbuf,"Subject: %s\r\n",lpHeader);//����ͷ
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		//printf(recvbuf);

		
		/*Sleep(1000);
		ZeroMemory(sendbuf,512);
		ZeroMemory(recvbuf,256);
		// Send the DATA command
		sprintf(sendbuf,"%s\r\n",lpBody);//���ݷ�������
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		*/

		Sleep(100);
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,512);
		// Send the DATA command
		sprintf(sendbuf,"%s\r\n",lpAccount);//���ݷ�������///lpBody1
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		//printf(recvbuf);

		Sleep(100);
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,512);
		// Send the DATA command
		sprintf(sendbuf,"%s%s\r\n",lpBody1,lpBody2);//���ݷ�������lpBody2
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		//printf(recvbuf);

		Sleep(100);
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,512);
		// Send the DATA command
		sprintf(sendbuf,"%s\r\n",lpBody3);//���ݷ�������
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		//printf(recvbuf);

		Sleep(100);
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,512);
		// Send the DATA command
		sprintf(sendbuf,"%s\r\n",lpBody4);//���ݷ�������
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		//printf(recvbuf);

		Sleep(100);
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,512);
		// Send the DATA command
		sprintf(sendbuf,"%s\r\n",lpBody5);//���ݷ�������
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		//printf(recvbuf);

		Sleep(100);
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,512);
		// Send the DATA command
		sprintf(sendbuf,"%s\r\n",lpBody6);//���ݷ�������
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		//printf(recvbuf);

		Sleep(100);
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,512);
		// Send the DATA command
		sprintf(sendbuf,"%s\r\n",lpBody7);//���ݷ�������
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		//printf(recvbuf);
		

		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		// Send the DATA command
		sprintf(sendbuf,"\r\n.\r\n");//���ݽ�β
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		//printf(recvbuf);


		//maple add
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		sprintf(sendbuf,"QUIT\r\n");//�˳�
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		//printf(recvbuf);
		m_Socket.MyDestory();
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("PAL","����E-Mail�ڴ治��");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("PAL","����E-Mail�ļ��쳣");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("PAL","����E-Mail�����쳣");
		return false;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("PAL",(char *)err.what());
		return false;
	}
	return true;
}

bool COperPal::ReadXmlGet(char * strFlag,char *strUrl,char * strpostData,vector<CGlobalStruct::DataRecord> * pVect,int* iCow,int *iCol)
{
	
	bool r_flag=false;
	try
	{
		CoInitialize(NULL);
		IXMLHTTPRequestPtr xmlrequest; //�������
		xmlrequest.CreateInstance("MSXML2.XMLHTTP");
		xmlrequest->open(_bstr_t("Get"),_bstr_t(strUrl),_variant_t("false"));
		xmlrequest->send();
		
		BSTR bstrbody;
		xmlrequest->get_responseText(&bstrbody);
		
		_bstr_t bstrtbody(bstrbody);
		
		
		
		CGlobalStruct::DataRecord tmpDataRecord;
		IXMLDOMDocumentPtr   pDoc;  
		IXMLDOMNodePtr  pNode;
		
		
		
		HRESULT hr = pDoc.CreateInstance(__uuidof(DOMDocument)); 
		pDoc->loadXML(bstrtbody);
		////cout<<static_cast <char*>(pDoc->xml); 
		pNode=pDoc->selectSingleNode("you9/userinfo/email");
		
		sprintf(tmpDataRecord.recordfieldname,"%s",(LPCSTR)_bstr_t(pNode->GetnodeName()));
		tmpDataRecord.recordfieldtype=3;
		sprintf(tmpDataRecord.recorddata,"%s",(LPCSTR)_bstr_t(pNode->text));
		pVect->push_back(tmpDataRecord);
		
		pNode.Release();
		pDoc.Release(); 
		xmlrequest.Release();
		CoUninitialize();
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("PAL","��ȡXML�ڴ治��");
		return false;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("PAL","��ȡXML�ļ��쳣");
		return false;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("PAL","��ȡXML�����쳣");
		return false;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("PAL",(char *)err.what());
		return false;
	}
	return r_flag;
	
}
int COperPal::GetPuzzleMap(char * PuzzleMap)
{
	
	int iPuzzleMap=0;
	try
	{
		char m_sql[1024];
		char m_result[256];
		sprintf(m_sql,"exec Pal_GetPuzzleMap '%s'",PuzzleMap);
		QuerySingleValue("PAL","127.0.0.1",4,m_sql,m_result);
		if(!strcmp(m_result,""))
		{
			return -1;
		}
		else
		{
			iPuzzleMap=StrToInt(m_result);
		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("PAL","ͼ������תID�ڴ治��");
		return -1;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("PAL","ͼ������תID�ļ��쳣");
		return -1;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("PAL","ͼ������תID�����쳣");
		return -1;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("PAL",(char *)err.what());
		return -1;
	}
	return iPuzzleMap;
}

//maple add
int COperPal::FindAccountStatus(char *UserName)
{
	int iResult=-1;
	try
	{
		char sql[256];
		ZeroMemory(sql,256);
		getRemoteSql("PAL","PAL_FindAccountStatus", m_sql,0);//��ȡ��ѯ��sql���
		sprintf(sql, m_sql, UserName);
		
		char m_result[256];
		QuerySingleValue("PAL","127.0.0.1",4,sql,m_result);
		if(!strcmp(m_result,""))
		{
			return -1;
		}
		else
		{
			iResult=StrToInt(m_result);
		}
	}
	catch(...)
	{
		iResult=-1;
	}
	return iResult;
}