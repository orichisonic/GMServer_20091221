#include "stdafx.h"
#include "time.h"
#include "JW2Info.h"
#include "AU2_Dll.h"
#include "md5.h"
#pragma comment(lib,"AU2_Dll.lib")
#pragma comment(lib,"md5.lib")

#include "OperTool.h"
#pragma comment(lib,"OperTool.lib")


//���캯��
CJW2Info::CJW2Info()
{
}

//��������
CJW2Info::~CJW2Info()
{
}

//�鿴�������7��10��
vector<CGlobalStruct::TFLV> CJW2Info::JW2_SMALL_PRESENT_QUERY(char* ServerName,char * ServerIP,char * UserName,int JW2_UserSN,int iType,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_SMALL_PRESENT_QUERY","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
		
		operJW2.FindDBIP(ServerIP,ServerIP,4);
		//��ȡԶ�̲�����Sql���
		if(!operJW2.getRemoteSql("JW2","JW2_SMALL_PRESENT_QUERY",operRemoteSql,4))
		{
			return DBTFLV;
		}
		switch(iType)
		{
		case 1:
			DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,JW2_UserSN,"M","G");
			break;
		case 2:
			DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,JW2_UserSN,"M","M");
			break;
		case 3:
			DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,JW2_UserSN,"G","G");
			break;
		default:
			break;
		}

	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","�鿴����/�����ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","�鿴����/�����ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","�鿴����/���������쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_SMALL_PRESENT_QUERY Error");
		return DBTFLV;
	}
	return DBTFLV;
}
//���߲�ѯ
vector<CGlobalStruct::TFLV>  CJW2Info::JW2_ItemInfo_Query(char* ServerName,char * ServerIP,char * UserName,int JW2_UserSN,int iItemPos,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_ItemInfo_Query","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
		
		switch(iItemPos)
		{
		case 0://����
			{
				if(!operJW2.getRemoteSql("JW2","JW2_ItemInfo_Query0",operRemoteSql,4))
				{
					return false;
				}
				operJW2.FindDBIP(ServerIP,ServerIP,ItemDB);
			}
			break;
		case 1://��Ʒ��
			{
				if(!operJW2.getRemoteSql("JW2","JW2_ItemInfo_Query1",operRemoteSql,4))
				{
					return false;
				}
				operJW2.FindDBIP(ServerIP,ServerIP,ItemDB);
			}
			break;
		case 2://������
			{
				if(!operJW2.getRemoteSql("JW2","JW2_ItemInfo_Query2",operRemoteSql,4))
				{
					return false;
				}
				operJW2.FindDBIP(ServerIP,ServerIP,Messenger);
			}
			break;
		default:
			break;
		}
		

		DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,JW2_UserSN);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","���߲�ѯ�ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","���߲�ѯ�ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","���߲�ѯ�����쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		return DBTFLV;
	}
	return DBTFLV;
}
//T�˹���GateWay
vector<CGlobalStruct::TFLV>  CJW2Info::KickPlayer(int userByID,char * ServerName,char * ServerIP,char * UserName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	COperatorJW2 operJW2;
	try
	{
		char logServerIP[40];
		sprintf(logServerIP,"%s",ServerIP);
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_BANISHPLAYER","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
		operJW2.FindDBIP(ServerIP,ServerIP,5);

		int res=0;
		res=BanishPlayer(ServerIP,UserName,operName);
		
		m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
		m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
		if(res==0)
		{
			
			sprintf((char *)&m_tflv.lpdata,"��%s�ɹ�",UserName);
			
		}
		else
		{
			sprintf((char *)&m_tflv.lpdata,"��%sʧ��",UserName);
		
		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","���˲����ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","���˲����ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","���˲��������쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		sprintf((char *)&m_tflv.lpdata,"��%sʧ��",UserName);
		return DBTFLV;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;
}

//����ʺ�logindb
vector<CGlobalStruct::TFLV>  CJW2Info::MakeOpen(int userByID,char * ServerName, char * ServerIP ,char * UserName,char * UserNick,
												int JW2_UserSN,char* Reason)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{
		COperatorJW2 operJW2;
		char sql1[256];
		char sql2[256];
		char sql3[256];
		char logServerIP[40];
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_ACCOUNT_OPEN","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
		sprintf(logServerIP,"%s",ServerIP);
		sprintf(sql1,"exec JW2_BanUserID '%s'",UserName);
		if(!operJW2.getRemoteSql("JW2","JW2_ACCOUNT_OPEN",operRemoteSql,4))
		{
			return DBTFLV;
		}
		sprintf(sql2,operRemoteSql,JW2_UserSN);
		sprintf(sql3,"exec JW2_UnBanUser %i, %i,'%s','%s','%s'",
			userByID,
			JW2_UserSN,
			UserName,
			logServerIP,
			Reason);
		operJW2.FindDBIP(ServerIP,ServerIP,8);
		DBTFLV=OpenPlayer("JW2",sql1,ServerIP,1,sql2,sql3);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","����ʺ��ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","����ʺ��ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","����ʺ������쳣");
		return DBTFLV;
	}
	return DBTFLV;
}


//�����ʺŷ�ͣlogindb
vector<CGlobalStruct::TFLV>  CJW2Info::MakeClose(int userByID,char * ServerName, char * ServerIP ,char * UserName,char * UserNick,
												 int JW2_UserSN, char* Reason)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	try
	{
		COperatorJW2 operJW2;
		char sql1[256];
		char sql2[256];
		char sql3[256];
		char logServerIP[40];
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_ACCOUNT_CLOSE","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
		sprintf(logServerIP,"%s",ServerIP);
		operJW2.FindDBIP(ServerIP,ServerIP,5);
		BanishPlayer(ServerIP,UserName,operName);
		sprintf(sql1,"exec JW2_BanUserID %s",UserName);///��ѯ�������ݿ���˺��Ƿ񱻷�ͣ
		if(!operJW2.getRemoteSql("JW2","JW2_ACCOUNT_CLOSE",operRemoteSql,4))
		{
			return DBTFLV;
		}
		sprintf(sql2,operRemoteSql,JW2_UserSN);///����Զ�����ݿ�
		sprintf(sql3,"exec JW2_BanUser %i, %i,'%s','%s','%s','%s'",
			userByID,
			JW2_UserSN,
			UserName,
			UserNick,
			logServerIP,
			Reason);///�������ݿ��¼log��־
		operJW2.FindDBIP(ServerIP,ServerIP,8);
		DBTFLV=ClosePlayer("JW2",sql1,ServerIP,1,sql2,sql3);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","��ͣ�ʺ��ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","��ͣ�ʺ��ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","��ͣ�ʺ������쳣");
		return DBTFLV;
	}
	return DBTFLV;
}
//����������/�ǳ���Ϣ
vector<CGlobalStruct::TFLV>  CJW2Info::LoginInfo(char * ServerName, char * ServerIP,char * UserName, char * UserNick,char * IP,char * begintime, char * endtime,int iIndex, int iPageSize)
{

	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_LOGINOUT_QUERY","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);

		operJW2.FindDBIP(ServerIP,ServerIP,LoginDB);
		
		if(strcmp("",UserName))//�ʺŲ�Ϊ��
		{
			if(!operJW2.getRemoteSql("JW2","JW2_LOGINOUT_QUERYBYACCOUNT",operRemoteSql,4))
			{
				return DBTFLV;
			}
			DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,UserName,begintime,endtime);
		}
		else if(strcmp("",UserNick))
		{
			if(!operJW2.getRemoteSql("JW2","JW2_LOGINOUT_QUERYBYNICKNAME",operRemoteSql,4))
			{
				return false;
			}
			DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,UserNick,begintime,endtime);
		}
		else
		{
			if(!operJW2.getRemoteSql("JW2","JW2_LOGINOUT_QUERYBYIP",operRemoteSql,4))
			{
				return false;
			}
			DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,IP,begintime,endtime);

		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","����������/�ǳ���Ϣ�ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","����������/�ǳ���Ϣ�ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","����������/�ǳ���Ϣ�����쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_LOGINOUT_QUERY Error");
		return DBTFLV;
	}
	return DBTFLV;
}



//��ѯ������Ϣ
vector<CGlobalStruct::TFLV>  CJW2Info::JW2_FAMILYINFO_QUERY(char * ServerName, char * ServerIP,char * FamilyName, int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_FAMILYINFO_QUERY","", operName,ServerName,FamilyName);
		operJW2.WriteText(strLog);
		
		operJW2.FindDBIP(ServerIP,ServerIP,Messenger);
		//��ȡԶ�̲�����Sql���
		if(!operJW2.getRemoteSql("JW2","JW2_FAMILYINFO_QUERY",operRemoteSql,4))
		{
			return false;
		}

		DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,"%",FamilyName,"%");
		
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","��ѯ������Ϣ�ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","��ѯ������Ϣ�ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","��ѯ������Ϣ�����쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_COUPLELOG_QUERY Error");
		return DBTFLV;
	}
	return DBTFLV;
}
//��ѯ�������;
vector<CGlobalStruct::TFLV>  CJW2Info::JW2_Family_Detail(CEnumCore::Message_Tag_ID m_tagID,char * ServerName, char * ServerIP,char * FamilyName, int FamilyID,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		char sql_type[256];
		operJW2.TagIDToSqlType(m_tagID,sql_type);
		operJW2.GetLogText(strLog, "BaseMessage", sql_type,"", operName,ServerName,FamilyName);
		operJW2.WriteText(strLog);
		
		operJW2.FindDBIP(ServerIP,ServerIP,operJW2.TagIDToDBID(m_tagID));
		//��ȡԶ�̲�����Sql���
		if(!operJW2.getRemoteSql("JW2",sql_type,operRemoteSql,4))
		{
			return false;
		}

		DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,FamilyID);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","��ѯ���������Ϣ�ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","��ѯ���������Ϣ�ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","��ѯ���������Ϣ�����쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_COUPLELOG_QUERY Error");
		return DBTFLV;
	}
	return DBTFLV;	
}

//�޸�����
vector<CGlobalStruct::TFLV>  CJW2Info::JW2_MODIFY_PWD(int userByID, char * ServerName,char * ServerIP,char * UserName,int JW2_UserSN,char * szNewPw)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	COperatorJW2 operJW2;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		char logServerIP[40];
		sprintf(logServerIP,"%s",ServerIP);
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_MODIFY_PWD","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
	
		char m_Result[256];
		ZeroMemory(m_Result,256);
		///��ѯ����////////////
		sprintf(operszSql,"exec JW2_FindAccountPw '%s'",UserName);
		operJW2.QuerySingleValue("JW2",logdbip,0,operszSql,m_Result);

		if(strcmp(m_Result,""))//û��PASSWPRD
		{
			sprintf((char *)&m_tflv.lpdata,"%s","�޸�����ɹ�");
			m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
			DBTFLV.push_back(m_tflv);
			return DBTFLV;
		}
		else
		{
		
			if(!operJW2.getRemoteSql("JW2","JW2_Get_AccountPw",operRemoteSql,4))
			{
				return DBTFLV;
			}
			sprintf(operszSql,operRemoteSql,ServerIP);//����������ѯ��sql���
			operJW2.FindDBIP(ServerIP,ServerIP,3);
			operJW2.QuerySingleValue("JW2",ServerIP,1,operszSql,m_Result);

			//��ȡԶ�̲�����Sql���
			
			if(!operJW2.getRemoteSql("JW2","JW2_MODIFY_PWD",operRemoteSql,4))
			{
				return DBTFLV;
			}
			char JW2_NewMd5[256];
			int	 iJW2_NewMd5=0;
			char JW2_NewPasswd[256];
			ZeroMemory(JW2_NewMd5,256);
			ZeroMemory(JW2_NewPasswd,256);
			sprintf(JW2_NewPasswd,"%s",szNewPw);
			MD5_EnCode(JW2_NewMd5,&iJW2_NewMd5,JW2_NewPasswd,strlen(JW2_NewPasswd));
			operJW2.FindDBIP(ServerIP,ServerIP,8);
			sprintf(operszSql,operRemoteSql,JW2_NewMd5,JW2_UserSN);//����������ѯ��sql���

			int tmpLength =GameDBOper("JW2",ServerIP,1,operRemoteSql,JW2_NewMd5,JW2_UserSN);
			if(tmpLength>0)
			{
				sprintf((char *)&m_tflv.lpdata,"%s","�޸�����ɹ�");
				GameDBOper("JW2","127.0.0.1",4,"exec JW2_InsertAccountPw %i,'%s','%s','%s','%s'",
						userByID,logServerIP,UserName,m_Result,JW2_NewPasswd);
			}
			else
			{
				sprintf((char *)&m_tflv.lpdata,"%s","�޸�����ʧ��");
			}
		}

	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","�޸������ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","�޸������ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","�޸����������쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		sprintf((char *)&m_tflv.lpdata,"%s","������Ϸ������ʧ��");	
		return DBTFLV;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;
}
//�ָ�����
vector<CGlobalStruct::TFLV>  CJW2Info::JW2_RECALL_PWD(int userByID, char * ServerName,char * ServerIP,char * UserName,int JW2_UserSN)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	COperatorJW2 operJW2;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		char logServerIP[40];
		sprintf(logServerIP,"%s",ServerIP);
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_RECALL_PWD","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
		
		char m_Result[256];
		ZeroMemory(m_Result,256);
		///��ѯ����////////////
		sprintf(operszSql,"exec JW2_FindAccountPw '%s'",UserName);
		operJW2.QuerySingleValue("JW2",logdbip,0,operszSql,m_Result);
		if(strcmp(m_Result,""))//û��PASSWPRD
		{
				
			//��ȡԶ�̲�����Sql���!
			
			if(!operJW2.getRemoteSql("JW2","JW2_MODIFY_PWD",operRemoteSql,4))
			{
				return DBTFLV;
			}
			operJW2.FindDBIP(ServerIP,ServerIP,8);
			int tmpLength=GameDBOper("JW2",ServerIP,1,operRemoteSql,m_Result,JW2_UserSN);
			if(tmpLength>0)
			{
				sprintf((char *)&m_tflv.lpdata,"%s","��ԭ����ɹ�");

				GameDBOper("JW2","127.0.0.1",4,"exec JW2_RecallAccountPw %i,'%s','%s'",
						userByID,logServerIP,UserName);
				
			}
			else
			{
				sprintf((char *)&m_tflv.lpdata,"%s","��ԭ����ʧ��");
			}


		}
		else
		{
			sprintf((char *)&m_tflv.lpdata,"%s","����û���޸�");
		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","��ԭ�����ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","��ԭ�����ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","��ԭ���������쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		sprintf((char *)&m_tflv.lpdata,"%s","������Ϸ������ʧ��");
		return DBTFLV;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;
}



//�޸ĵȼ�
vector<CGlobalStruct::TFLV> CJW2Info::JW2_MODIFYLEVEL_QUERY(int userByID, char * ServerName,char * ServerIP,char * UserName,int JW2_UserSN,int iLevel)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	COperatorJW2 operJW2;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		char logServerIP[40];
		sprintf(logServerIP,"%s",ServerIP);
		int tmpLength=0;
		char m_Result[256];
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_MODIFYLEVEL_QUERY","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
		
		//��ȡԶ�̲�����Sql���
		if(!operJW2.getRemoteSql("JW2","JW2_MODIFYLEVEL_QUERY1",operRemoteSql,4))
		{
			return DBTFLV;
		}
		operJW2.FindDBIP(ServerIP,ServerIP,7);

		tmpLength=GameDBOper("JW2",ServerIP,1,operRemoteSql,iLevel,JW2_UserSN);

		if(!operJW2.getRemoteSql("JW2","JW2_MODIFYLEVEL_QUERY2",operRemoteSql,4))
		{
			return DBTFLV;
		}
		operJW2.FindDBIP(ServerIP,ServerIP,9);
		tmpLength=GameDBOper("JW2",ServerIP,1,operRemoteSql,iLevel,JW2_UserSN);

		sprintf(operszSql,"exec JW2_LevelToExp %i",iLevel);
		operJW2.QuerySingleValue("JW2",logdbip,0,operszSql,m_Result);
		long iExp=StrToLong(m_Result);

		if(!operJW2.getRemoteSql("JW2","JW2_MODIFYEXP_QUERY",operRemoteSql,4))
		{
			return DBTFLV;
		}
		operJW2.FindDBIP(ServerIP,ServerIP,7);
		tmpLength=GameDBOper("JW2",ServerIP,1,operRemoteSql,iExp,JW2_UserSN);

		if(tmpLength>0)
		{
			sprintf((char *)&m_tflv.lpdata,"�޸ĵȼ�,�µȼ�%i�ɹ�",iLevel);
			logFile.WriteDBLog(userByID,"������2","JW2_MODIFYLEVEL_QUERY",logServerIP,(char *)&m_tflv.lpdata);
		}
		else
		{
			sprintf((char *)&m_tflv.lpdata,"�޸ĵȼ�,�µȼ�%iʧ��",iLevel);
		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","�޸ĵȼ��ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","�޸ĵȼ��ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","�޸ĵȼ������쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		sprintf((char *)&m_tflv.lpdata,"%s","������Ϸ������ʧ��");
		return DBTFLV;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;
}


//�޸ľ���
vector<CGlobalStruct::TFLV> CJW2Info::JW2_MODIFYEXP_QUERY(int userByID, char * ServerName,char * ServerIP,char * UserName,int JW2_UserSN,int iExp)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	COperatorJW2 operJW2;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		char logServerIP[40];
		sprintf(logServerIP,"%s",ServerIP);
		int tmpLength=0;
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_MODIFYEXP_QUERY","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
		
		char m_Result[256];
		//��ȡԶ�̲�����Sql���
		if(!operJW2.getRemoteSql("JW2","JW2_MODIFYEXP_QUERY",operRemoteSql,4))
		{
			return DBTFLV;
		}
		operJW2.FindDBIP(ServerIP,ServerIP,7);
		tmpLength=GameDBOper("JW2",ServerIP,1,operRemoteSql,iExp,JW2_UserSN);

		sprintf(operszSql,"exec JW2_ExpToLevel %d",iExp);
		operJW2.QuerySingleValue("JW2",logdbip,0,operszSql,m_Result);
		int iLevel=StrToInt(m_Result);
		
		if(!operJW2.getRemoteSql("JW2","JW2_MODIFYLEVEL_QUERY1",operRemoteSql,4))
		{
			return DBTFLV;
		}
		operJW2.FindDBIP(ServerIP,ServerIP,7);
		tmpLength=GameDBOper("JW2",ServerIP,1,operRemoteSql,iLevel,JW2_UserSN);

		
		if(!operJW2.getRemoteSql("JW2","JW2_MODIFYLEVEL_QUERY2",operRemoteSql,4))
		{
			return DBTFLV;
		}
		operJW2.FindDBIP(ServerIP,ServerIP,9);
		tmpLength=GameDBOper("JW2",ServerIP,1,operRemoteSql,iLevel,JW2_UserSN);

		if(tmpLength>0)
		{
			sprintf((char *)&m_tflv.lpdata,"�޸ľ���,�¾���%i�ɹ�",iExp);
			logFile.WriteDBLog(userByID,"������2","JW2_MODIFYLEVEL_QUERY",logServerIP,(char *)&m_tflv.lpdata);
			
		}
		else
		{
			sprintf((char *)&m_tflv.lpdata,"�޸ľ���,�¾���%iʧ��",iExp);
		}
			
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","�޸ľ����ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","�޸ľ����ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","�޸ľ��������쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
	}
	catch (...)
	{
		sprintf((char *)&m_tflv.lpdata,"%s","������Ϸ������ʧ��");
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;
}

//ɾ������
vector<CGlobalStruct::TFLV> CJW2Info::JW2_ITEM_DEL(int userByID,char * ServerName,char * ServerIP,
							char * UserName, int JW2_UserSN,char * avatarItem,char * avatarItemName,
							int iItemPos, int iItemNo)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	COperatorJW2 operJW2;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		char logServerIP[40];
		sprintf(logServerIP,"%s",ServerIP);
		int tmpLength=0;
		char retMessage[256];
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_ITEM_DEL","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);

		switch(iItemPos)
		{
		case 0://����
			{
				//��ȡԶ�̲�����Sql���
				if(!operJW2.getRemoteSql("JW2","JW2_ITEM_DEL",operRemoteSql,4))
				{
					return false;
				}
				operJW2.FindDBIP(ServerIP,ServerIP,10);
				tmpLength=GameDBOper("JW2",ServerIP,1,operRemoteSql,avatarItem,JW2_UserSN);
			}
			break;
		case 1://��Ʒ��
			{
				//��ȡԶ�̲�����Sql���
				if(!operJW2.getRemoteSql("JW2","JW2_ITEM_DEL",operRemoteSql,4))
				{
					return false;
				}
				operJW2.FindDBIP(ServerIP,ServerIP,10);
				tmpLength=GameDBOper("JW2",ServerIP,1,operRemoteSql,avatarItem,JW2_UserSN);
			}
			break;
		case 2://������
			{
				if(!operJW2.getRemoteSql("JW2","JW2_ITEM_DEL1",operRemoteSql,4))
				{
					return false;
				}
				operJW2.FindDBIP(ServerIP,ServerIP,9);
				tmpLength=GameDBOper("JW2",ServerIP,1,operRemoteSql,avatarItem,JW2_UserSN,iItemNo);
			}
			break;
		default:
			break;
		}
	
		if(tmpLength>0)
		{
			sprintf(retMessage,"ɾ�����߳ɹ�,��������%s",avatarItemName);
			logFile.WriteDBLog(userByID,"������2","JW2_ITEM_DEL",logServerIP,retMessage);
			
		}
		else
		{
			sprintf(retMessage,"ɾ������ʧ��,��������%s",avatarItemName);
		}
		
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","ɾ�������ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","ɾ�������ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","ɾ�����������쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		return DBTFLV;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;
}

//��ӵ���
vector<CGlobalStruct::TFLV> CJW2Info::JW2_ADD_ITEM(int userByID,char * ServerName, char * ServerIP ,char* UserName,int JW2_UserSN,
							char * strMailTitle,char * strMailContent,char* JW2_AvatarItem,int count)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	COperatorJW2 operJW2;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		char logServerIP[40];
		sprintf(logServerIP,"%s",ServerIP);
		char retMessage[256];
		int tmpLength=0;
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_ADD_ITEM","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);

		//��ȡԶ�̲�����Sql���
		if(!operJW2.getRemoteSql("JW2","JW2_ADD_ITEM",operRemoteSql,4))
		{
			return false;
		}
		operJW2.FindDBIP(ServerIP,ServerIP,9);//Item_db���ݿ�

		int mID = 0;//��ƷID
		int itemNum = 0;//��������
		char strMID[20];
		char strItemNum[20];
		ZeroMemory(strMID, 20);//��ʼ����ʱ������ƷID�ı���		
		ZeroMemory(strItemNum, 20);//��ʼ����ʱ������������ı���

		char strAddItemName[2048];//�ܵ�������
		char strtmpItemName[256];
		char strMIDName[256];//������
		ZeroMemory(strAddItemName,2048);
		ZeroMemory(strtmpItemName,256);
		ZeroMemory(strMIDName,256);
			
		int nNum = 1;//��һ���ַ��Ĵ���
		int mNum = 1;//�ڶ����ַ��Ĵ���
		
		for(int i=0;i<count;i++)
		{
			ZeroMemory(strMID, 20);//��ʼ����ʱ������ƷID�ı���		
			ZeroMemory(strItemNum, 20);//��ʼ����ʱ������������ı���
			if(i == 0)
			{
				operJW2.getSpecChar(JW2_AvatarItem, "", mNum, "*", nNum, strMID);
				operJW2.getSpecChar(JW2_AvatarItem, "*", nNum, "|", mNum, strItemNum);
				nNum++;
			}
			else if(i == count-1)
			{
				operJW2.getSpecChar(JW2_AvatarItem, "|", mNum, "*", nNum, strMID);
				operJW2.getSpecChar(JW2_AvatarItem, "*", nNum, "", mNum, strItemNum);
			}
			else
			{
				operJW2.getSpecChar(JW2_AvatarItem, "|", mNum, "*", nNum, strMID);
				operJW2.getSpecChar(JW2_AvatarItem, "*", nNum, "|", mNum+1, strItemNum);
				nNum++;
				mNum++;
			}
			mID=atoi(strMID);
			///ͨ��ID��������
			sprintf(operszSql,"select b.itemname from jw2_item a,jw2_product b where a.itemcode=b.itemcode and a.productid=%i",mID);
			operJW2.QuerySingleValue("JW2","127.0.0.1",4,operszSql,strMIDName);
			itemNum = atoi(strItemNum);
			sprintf(strtmpItemName,"������%s(ID��%i)*����:%i|",strMIDName,mID,itemNum);
			sprintf(strAddItemName,"%s%s",strAddItemName,strtmpItemName);

			for(int j=0;j<itemNum;j++)
			{
				tmpLength= GameDBOper("JW2",ServerIP,1,operRemoteSql,JW2_UserSN,strMailTitle,strMailContent,strMID);
			}




		}
	
		if(tmpLength>0)
		{
			sprintf((char *)&m_tflv.lpdata,"%s","Add_Item_Success");
			sprintf(retMessage,"����˺�%s���߳ɹ�,%s",UserName,JW2_AvatarItem);
			logFile.WriteDBLog(userByID,"������2","JW2_ADD_ITEM",logServerIP,retMessage);
			
		}
		else
		{
			sprintf((char *)&m_tflv.lpdata,"%s","Add_Item_Failed");
		}

		
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","��ӵ����ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","��ӵ����ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","��ӵ��������쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		return DBTFLV;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;
}


//�޸Ľ�Ǯ
vector<CGlobalStruct::TFLV> CJW2Info::JW2_MODIFY_MONEY(int userByID, char * ServerName,char * ServerIP,char * UserName,int JW2_UserSN,int iMoney)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	CGlobalStruct::TFLV m_tflv;
	COperatorJW2 operJW2;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		char logServerIP[40];
		sprintf(logServerIP,"%s",ServerIP);
		int tmpLength=0;
		char retMessage[256];
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_MODIFY_MONEY","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
		

		//��ȡԶ�̲�����Sql���
		if(!operJW2.getRemoteSql("JW2","JW2_MODIFY_MONEY",operRemoteSql,4))
		{
			return false;
		}
		operJW2.FindDBIP(ServerIP,ServerIP,7);
		tmpLength=GameDBOper("JW2",ServerIP,1,operRemoteSql,iMoney,JW2_UserSN);
		
		if(tmpLength>0)
		{
			sprintf(retMessage,"�޸�G�ҳɹ�,��G��%i",iMoney);
		}
		else
		{
			sprintf(retMessage,"�޸�G��ʧ��,��G��%i",iMoney);
		}
		logFile.WriteDBLog(userByID,"������2","JW2_MODIFY_MONEY",logServerIP,retMessage);
		sprintf((char *)&m_tflv.lpdata,"%s",retMessage);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","�޸�G���ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","�޸�G���ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","�޸�G�������쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{

		return TRUE;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;
}


//ģ�����߲�ѯ
vector<CGlobalStruct::TFLV> CJW2Info::JW2_ITEM_SELECT(int userByID,char * JW2_AvatarItemName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		int tmpLength=0;
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_ITEM_SELECT","", operName);
		operJW2.WriteText(strLog);
		
		DBTFLV=GameDBQuery("JW2","127.0.0.1",4,0,0,"exec JW2_ITEM_SELECT %i,'%s'",userByID,JW2_AvatarItemName);
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","ģ�����߲�ѯ�ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","ģ�����߲�ѯ�ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","ģ�����߲�ѯ�����쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	return DBTFLV;
}



//������Ϣ��ѯ
vector<CGlobalStruct::TFLV> CJW2Info::JW2_BasicInfo_Query(char * ServerName, char * ServerIP,char * FamilyName, int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_BasicInfo_Query","", operName,ServerName,FamilyName);
		operJW2.WriteText(strLog);
		
		operJW2.FindDBIP(ServerIP,ServerIP,Messenger);
		//��ȡԶ�̲�����Sql���
		if(!operJW2.getRemoteSql("JW2","JW2_BasicInfo_Query",operRemoteSql,4))
		{
			return false;
		}
		
		DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,"%",FamilyName,"%");
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","������Ϣ��ѯ�ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","������Ϣ��ѯ�ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","������Ϣ��ѯ�����쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_COUPLELOG_QUERY Error");
	}
	return DBTFLV;
}


//��Ǯ��־��ѯ
vector<CGlobalStruct::TFLV> CJW2Info::JW2_MoneyLog_Query(char * ServerName, char * ServerIP,char* UserName,
						 int JW2_UserSN,int iType,char * szStartTime,char * szEndTime,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_MoneyLog_Query","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
		
		operJW2.FindDBIP(ServerIP,ServerIP,LogDB);

		switch (iType)
		{
		case 2:
			if(!operJW2.getRemoteSql("JW2","JW2_MMoney_Log",operRemoteSql,4))
			{
				return DBTFLV;
			}
			break;
		case 3:
			if(!operJW2.getRemoteSql("JW2","JW2_GMoney_Log",operRemoteSql,4))
			{
				return DBTFLV;
			}
			break;
		default:
			break;
		}
		//��ȡԶ�̲�����Sql���
		DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,JW2_UserSN,szStartTime,szEndTime);

	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_MoneyLog_Query Error");
	}
	return DBTFLV;
}

//������־(0,����,1,����)
vector<CGlobalStruct::TFLV> CJW2Info::JW2_FamilyFund_Log(char * ServerName, char * ServerIP,char* FamilyName,
						 int FamilyID,int iStatus,char * szStartTime,char * szEndTime,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_FamilyFund_Log","", operName,ServerName,FamilyName);
		operJW2.WriteText(strLog);
		
		operJW2.FindDBIP(ServerIP,ServerIP,Messenger);

		if(iStatus==0)
		{
			if(!operJW2.getRemoteSql("JW2","JW2_FamilyFund_Log0",operRemoteSql,4))
			{
				return DBTFLV;
			}
	
		}
		else
		{
			if(!operJW2.getRemoteSql("JW2","JW2_FamilyFund_Log1",operRemoteSql,4))
			{
				return DBTFLV;
			}
		}
		//��ȡԶ�̲�����Sql���
		DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,FamilyID,szStartTime,szEndTime);
		
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","��ѯ������־�ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","��ѯ������־�ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","��ѯ������־�����쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_FamilyFund_Log Error");
		return DBTFLV;
	}
	return DBTFLV;
}

//������߹�����־
vector<CGlobalStruct::TFLV> CJW2Info::JW2_FamilyBuyLog_Query(char * ServerName, char * ServerIP,char* FamilyName,
						 int FamilyID,char * szStartTime,char * szEndTime,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_FamilyBuyLog_Query","", operName,ServerName,FamilyName);
		operJW2.WriteText(strLog);
		
		operJW2.FindDBIP(ServerIP,ServerIP,Messenger);
		//��ȡԶ�̲�����Sql���
		if(!operJW2.getRemoteSql("JW2","JW2_FamilyBuyLog_Query",operRemoteSql,4))
		{
			return false;
		}
		DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,FamilyID,szStartTime,szEndTime);
		
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","������߹�����־�ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","������߹�����־�ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","������߹�����־�����쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_FamilyBuyLog_Query Error");
		return DBTFLV;
	}
	return DBTFLV;
}

//������־
vector<CGlobalStruct::TFLV> CJW2Info::JW2_CashMoney_Log(char * ServerName, char * ServerIP,char* UserName,
						 int JW2_UserSN,int iType,char * szStartTime,char * szEndTime,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_CashMoney_Log","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
		
		operJW2.FindDBIP(ServerIP,ServerIP,LogDB);
		//��ȡԶ�̲�����Sql���
		if(!operJW2.getRemoteSql("JW2","JW2_CashMoney_Log",operRemoteSql,4))
		{
			return DBTFLV;
		}
		DBTFLV=GameDBQuery("JW2",ServerIP,1,iIndex,iPageSize,operRemoteSql,JW2_UserSN,iType,szStartTime,szEndTime);
	
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","������־�ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","������־�ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","������־�����쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_Task_Log Error");
		return DBTFLV;
	}
	return DBTFLV;
}

//��������
vector<CGlobalStruct::TFLV> CJW2Info::JW2_Import_KickPlayer()
{
/*	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_Import_KickPlayer","", m_UserName,JW2_ServerName,JW2_UserName);
		operJW2.WriteText(strLog);
		
		operJW2.FindDBIP(JW2_ServerIP,JW2_ServerIP,5);
		char m_result[256];
		CLogic::SocketToRealName(m_hServerSocket,m_result);

		int res=0;
		char allUserName[2048];
		char retMessage[2048];
		char tmpMessage[256];
		ZeroMemory(retMessage,2048);
		sprintf(allUserName,"%s",(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::JW2_BoardMessage,0)).lpdata);

		
		char *username=strtok(allUserName,"|");//��ѯ��־"|"
		while(username!=NULL)
		{
			res=BanishPlayer(JW2_ServerIP,username,m_result);
			
			if(res==0)
			{
				logFile.WriteDBLog(userByID,"������2","JW2_BANISHPLAYER",JW2_ServerIP,"���˳ɹ�");
			}
			else
			{
				sprintf(tmpMessage,"��%sʧ��\n",username);
				sprintf(retMessage,"%s%s",retMessage,tmpMessage);
				logFile.WriteDBLog(userByID,"������2","JW2_BANISHPLAYER",JW2_ServerIP,"����ʧ��");
			}		
			username=strtok(NULL,"|");//��ѯ��־";"
		}
		if(strlen(retMessage)>0)
		{
			operJW2.ReturnMessage(retMessage);
		}
		else
		{
			operJW2.ReturnMessage("�������˳ɹ�");
		}
		
		
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return false;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_Task_Log Error");
		operJW2.ReturnMessage("������Ϸ������ʧ��");
		return TRUE;
	}*/
	return true;
}

//��ͣ�ʺ�
vector<CGlobalStruct::TFLV> CJW2Info::JW2_Import_AccountClose()
{
	/*try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_Task_Log","", m_UserName,JW2_ServerName,JW2_UserName);
		operJW2.WriteText(strLog);
		
		char m_result[256];

		char allUserName[2048];
		char retMessage[2048];
		char tmpMessage[256];
		ZeroMemory(retMessage,2048);
		sprintf(allUserName,"%s",(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::JW2_BoardMessage,0)).lpdata);


		char *username=strtok(allUserName,"|");//��ѯ��־"|"
		while(username!=NULL)
		{
			operJW2.FindDBIP(JW2_ServerIP,JW2_ServerIP,GameDB);
			if(!operJW2.getRemoteSql("JW2_ACCOUNTTOSN",operRemoteSql,1))
			{
				return false;
			}
			sprintf(operszSql,operRemoteSql,JW2_UserName);
			operJW2.QuerySingleValue(JW2_ServerIP,1,operszSql,m_result);
			JW2_UserSN=StrToInt(m_result);
			if(JW2_UserSN==0)
			{
				sprintf(tmpMessage,"��ͣ%sʧ��\n",username);
				sprintf(retMessage,"%s%s",retMessage,tmpMessage);
				
			}
			else
			{
				CLogic::SocketToRealName(m_hServerSocket,m_result);
				operJW2.FindDBIP(JW2_ServerIP,JW2_ServerIP,5);
				int res=BanishPlayer(JW2_ServerIP,JW2_UserName,m_result);
				
				operJW2.FindDBIP(JW2_ServerIP,JW2_ServerIP,8);
				if(!operJW2.getRemoteSql("JW2_ACCOUNT_CLOSE",operRemoteSql,1))
				{
					return false;
				}
				sprintf(operszSql,operRemoteSql,JW2_UserSN);//����������ѯ��sql���
				int tmpLength = DBVect.OperVectorMain("JW2", JW2_ServerIP, operszSql, 1);//��������ݿ��ѯ�õ�������
				if(tmpLength>0)
				{
					sprintf(operszSql,"exec JW2_UnBanUser %i,%i,'%s','%s','%s'",
						userByID,JW2_UserSN,JW2_UserName,JW2_ServerIP,(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::JW2_Reason,0)).lpdata);
					DBVect.OperVectorMain("JW2",logdbip,operszSql,4);
				}
				else
				{
					sprintf(tmpMessage,"��ͣ%sʧ��\n",username);
					sprintf(retMessage,"%s%s",retMessage,tmpMessage);
				}
			}
			username=strtok(NULL,"|");//��ѯ��־";"
		}
		if(strlen(retMessage)>0)
		{
			operJW2.ReturnMessage(retMessage);
		}
		else
		{
			operJW2.ReturnMessage("������ͣ�ɹ�");
		}

		
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return false;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_Task_Log Error");
		operJW2.ReturnMessage("������Ϸ������ʧ��");
		return TRUE;
	}*/
	return true;
}

//�������
vector<CGlobalStruct::TFLV> CJW2Info::JW2_Import_AccountOpen()
{
/*	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_Task_Log","", m_UserName,JW2_ServerName,JW2_UserName);
		operJW2.WriteText(strLog);
		
		char m_result[256];
		
		char allUserName[2048];
		char retMessage[2048];
		char tmpMessage[256];
		ZeroMemory(retMessage,2048);
		sprintf(allUserName,"%s",(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::JW2_BoardMessage,0)).lpdata);
		
		
		char *username=strtok(allUserName,"|");//��ѯ��־"|"
		while(username!=NULL)
		{
			operJW2.FindDBIP(JW2_ServerIP,JW2_ServerIP,GameDB);
			if(!operJW2.getRemoteSql("JW2_ACCOUNTTOSN",operRemoteSql,1))
			{
				return false;
			}
			sprintf(operszSql,operRemoteSql,JW2_UserName);
			operJW2.QuerySingleValue(JW2_ServerIP,1,operszSql,m_result);
			JW2_UserSN=StrToInt(m_result);
			if(JW2_UserSN==0)
			{
				sprintf(tmpMessage,"���%sʧ��\n",username);
				sprintf(retMessage,"%s%s",retMessage,tmpMessage);
				
			}
			else
			{
			
				operJW2.FindDBIP(JW2_ServerIP,JW2_ServerIP,8);
				if(!operJW2.getRemoteSql("JW2_ACCOUNT_OPEN",operRemoteSql,1))
				{
					return false;
				}
				sprintf(operszSql,operRemoteSql,JW2_UserSN);//����������ѯ��sql���
				int tmpLength = DBVect.OperVectorMain("JW2", JW2_ServerIP, operszSql, 1);//��������ݿ��ѯ�õ�������
				if(tmpLength>0)
				{
					sprintf(operszSql,"exec JW2_BanUser %i, %i,'%s','%s','%s','%s'",
						userByID,JW2_UserSN,JW2_UserName,JW2_NickName,JW2_ServerIP,	(LPCSTR)&(recvVect.getTLVByTag(CEnumCore::TagName::JW2_Reason,0)).lpdata);
					DBVect.OperVectorMain("JW2",logdbip,operszSql,4);
				}
				else
				{
					sprintf(tmpMessage,"���%sʧ��\n",username);
					sprintf(retMessage,"%s%s",retMessage,tmpMessage);
				}
			}
			username=strtok(NULL,"|");//��ѯ��־";"
		}
		if(strlen(retMessage)>0)
		{
			operJW2.ReturnMessage(retMessage);
		}
		else
		{
			operJW2.ReturnMessage("�������ɹ�");
		}
		
		
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return false;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_Task_Log Error");
		operJW2.ReturnMessage("������Ϸ������ʧ��");
		return TRUE;
	}*/
	return true;
}



//����GM״̬
vector<CGlobalStruct::TFLV> CJW2Info::JW2_GM_Update(int userByID, char * ServerName,char * ServerIP,char * UserName,int JW2_UserSN,int iStatus)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	CGlobalStruct::TFLV m_tflv;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		char logServerIP[40];
		sprintf(logServerIP,"%s",ServerIP);
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_GM_Update","", operName,ServerName,UserName);
		operJW2.WriteText(strLog);
		///ͨ���ʺŲ�ѯUserSN
		
		operJW2.FindDBIP(ServerIP,ServerIP,7);
		
		//��ȡԶ�̲�����Sql���
		if(!operJW2.getRemoteSql("JW2","JW2_GM_Update",operRemoteSql,4))
		{

			return false;
		}
		
		int tmpLength = GameDBOper("JW2", ServerIP, 1,operRemoteSql,iStatus,JW2_UserSN);//��������ݿ��ѯ�õ�������
		if(tmpLength>0)
		{
			sprintf((char *)&m_tflv.lpdata,"%s","����GM״̬�ɹ�");
			logFile.WriteDBLog(userByID,"������2","JW2_GM_Update",logServerIP,"����GM״̬�ɹ�");
		}
		else
		{
			sprintf((char *)&m_tflv.lpdata,"%s","����GM״̬ʧ��");
			logFile.WriteDBLog(userByID,"������2","JW2_GM_Update",logServerIP,"����GM״̬ʧ��");
		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","����GM״̬�ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","����GM״̬�ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","����GM״̬�����쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_Task_Log Error");
		return DBTFLV;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;
}

//ɾ��Ȩ��
vector<CGlobalStruct::TFLV> CJW2Info::JW2_ProductManage_Del(char * ServerName, char * ServerIP,int userByID,int iTaskid)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	CGlobalStruct::TFLV m_tflv;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_PRODUCTMANAGE_DEL","", operName);
		operJW2.WriteText(strLog);

		int tmpLength=GameDBOper("JW2","127.0.0.1",4,"exec JW2_PRODUCTMANAGE_DEL %i",iTaskid);

		if(tmpLength>0)
		{
			sprintf((char *)&m_tflv.lpdata,"%s","ɾ��Ȩ�޳ɹ���");
			logFile.WriteDBLog(userByID,"������2","JW2_PRODUCTMANAGE_DEL",ServerIP,"ɾ��Ȩ�޳ɹ���");
		}
		else
		{
			logFile.WriteDBLog(userByID,"������2","JW2_PRODUCTMANAGE_DEL",ServerIP,"ɾ��Ȩ��ʧ�ܣ�");
			sprintf((char *)&m_tflv.lpdata,"%s","ɾ��Ȩ��ʧ�ܣ�");
		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","ɾ��Ȩ���ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","ɾ��Ȩ���ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","ɾ��Ȩ�������쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_Task_Log Error");
		return DBTFLV;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;
}

//�鿴Ȩ��
vector<CGlobalStruct::TFLV> CJW2Info::JW2_ProductManage_View(int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_ProductManage_View","", operName);
		operJW2.WriteText(strLog);
		
		
		DBTFLV=GameDBQuery("JW2","127.0.0.1",4,iIndex,iPageSize,"exec JW2_ProductManage_View");
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","�鿴Ȩ���ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","�鿴Ȩ���ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","�鿴Ȩ�������쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_Task_Log Error");
		return DBTFLV;
	}
	return DBTFLV;
}
////maple add//////////��֤Ȩ��
vector<CGlobalStruct::TFLV> CJW2Info::JW2_ProductManage_Validate(char * ServerName,char * ServerIP, int GMID,char * JW2_AvatarItem)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_ProductManage_Validate","", operName,ServerName);
		operJW2.WriteText(strLog);

		char result[128]=_T("F");
		
		int tmpLength=GameDBOper("JW2",ServerIP,4,"exec JW2_PRODUCTPOWER_Valid '%s',%i,'%s','%s'",
							ServerIP,GMID,JW2_AvatarItem,result);
		if(tmpLength>0)
		{
	
			int tmp=GameDBOper("JW2",ServerIP,4,"exec JW2_VaildResult '%s'",JW2_AvatarItem);

			if(tmp>0)
			{
				//��ȡԶ�̲�����Sql���
				if(!operJW2.getRemoteSql("JW2","JW2_GetValidateResult",operRemoteSql,4))
				{
					return false;
				}
				DBTFLV=GameDBQuery("JW2",logdbip,4,1,1,operRemoteSql);
			}
		}
		
		
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","��֤Ȩ���ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","��֤Ȩ���ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","��֤Ȩ�������쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_Task_Log Error");
		return DBTFLV;
	}
	return DBTFLV;
}
//��֤ͨ����Ȩ�ޱ��м�ȥ
vector<CGlobalStruct::TFLV> CJW2Info::JW2_ProductManage_Deduct(int userByID,int GMID,char * ServerIP, char * JW2_AvatarItem)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	CGlobalStruct::TFLV m_tflv;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_ProductManage_Deduct","", operName);
		operJW2.WriteText(strLog);
		
		int tmpLength =GameDBOper("JW2","127.0.0.1",4,"exec JW2_PRODUCT_Deduct %i,'%s','%s'",
			GMID,ServerIP,JW2_AvatarItem);

		if(tmpLength>0)
		{
			
			logFile.WriteDBLog(userByID,"������2","JW2_ProductManage_Deduct",ServerIP,"�ɹ���Ȩ�ޱ��м�ȥ������ӵ��߳ɹ�!��");
			sprintf((char *)&m_tflv.lpdata,"%s","��ӵ��߳ɹ�!");
		}
		else
		{
			logFile.WriteDBLog(userByID,"������2","JW2_ProductManage_Deduct",ServerIP,"��Ȩ�ޱ��м�ȥʧ�ܵ�����ӵ���ʧ��!��");
			sprintf((char *)&m_tflv.lpdata,"%s","��ӵ���ʧ��!");
		}
	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","��Ȩ�ޱ��м�ȥ�������ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","��Ȩ�ޱ��м�ȥ�������ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","��Ȩ�ޱ��м�ȥ�����������쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_Task_Log Error");
		return DBTFLV;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;


}

//����Ȩ��
vector<CGlobalStruct::TFLV> CJW2Info::JW2_ProductManage_Insert(int userByID,char * ServerName,char * ServerIP,char * JW2_AvatarItem,
										char * GMAccount,char * begintime,char * endtime,int totalprice)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorJW2 operJW2;
	CGlobalStruct::TFLV m_tflv;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		operJW2.GetLogText(strLog, "BaseMessage", "JW2_ProductManage_Insert","", operName,ServerName);
		operJW2.WriteText(strLog);

		int tmpLength=GameDBOper("JW2",logdbip,4,"exec JW2_ITEMPOWNER_ADMIN '%s','%s','%s','%s','%s',%i",
								ServerIP,
								JW2_AvatarItem,
								GMAccount,
								begintime,
								endtime,
								totalprice);
		if(tmpLength>0)
		{
			sprintf((char *)&m_tflv.lpdata,"%s","����Ȩ�޳ɹ�!");
			GameDBOper("JW2","127.0.0.1",4,"exec JW2_ITEMPOWNER_LOG %i,'������2','JW2_ProductManage_Insert','%s','����Ȩ�޳ɹ�!'",
				userByID,ServerIP);
		}
		else
		{
			sprintf((char *)&m_tflv.lpdata,"%s","����Ȩ��ʧ��!");
			GameDBOper("JW2","127.0.0.1",4,"exec JW2_ITEMPOWNER_LOG %i,'������2','JW2_ProductManage_Insert','%s','����Ȩ��ʧ��!'",
				userByID,ServerIP);
		}

	}
	catch (CMemoryException* e)
	{
		logFile.WriteErrorLog("JW2","����Ȩ���ڴ治��");
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		logFile.WriteErrorLog("JW2","����Ȩ���ļ��쳣");
		return DBTFLV;
	}
	catch (CException* e)
	{
		logFile.WriteErrorLog("JW2","����Ȩ�������쳣");
		return DBTFLV;
	}
	catch (runtime_error err)
	{
		logFile.WriteErrorLog("JW2",(char *)err.what());
		return DBTFLV;
	}
	catch (...)
	{
		logFile.WriteLogFile("JW2_Task_Log Error");
		return DBTFLV;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;
}

