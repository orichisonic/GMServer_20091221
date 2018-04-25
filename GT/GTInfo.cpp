#include "StdAfx.h"
#include "GTInfo.h"

CGTInfo::CGTInfo(void)
{
}

CGTInfo::~CGTInfo(void)
{
}

//���ѽ�����־
vector <CGlobalStruct::TFLV> CGTInfo::GT_FriendTrade_Log(char * ServerName, char * ServerIP,char * UserName, int UserID,int iIndex,int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	try
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>���<%s>���ѽ�����־",ServerName,UserName);
		logFile.WriteText("GTown",strLog);
		
		//��ȡԶ�̲�����Sql���
		operGT.getRemoteSql("GT","GT_FriendTrade_Log",operRemoteSql,4);
		if(!strcmp("",operRemoteSql))
		{
			return DBTFLV;
		}
		DBTFLV=GameDBQuery("GT",ServerIP,0,iIndex,iPageSize,operRemoteSql,UserID,UserID);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"���ѽ�����־�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"���ѽ�����־�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"���ѽ�����־�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>���<%s>��ϸ������Ϣ����ϵͳ����",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);//��������Ϣд����־��
		return DBTFLV;
	}
	return DBTFLV;
}

//��ѯ������Ϣ
vector <CGlobalStruct::TFLV> CGTInfo::GT_FamilyInfo(char * ServerName, char * ServerIP,char * FamilyName,int iIndex,int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	try
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>����<%s>������Ϣ",operName,ServerName,FamilyName);
		logFile.WriteText("GTown",strLog);
		
		//��ȡԶ�̲�����Sql���
		operGT.getRemoteSql("GT","GT_FamilyInfo",operRemoteSql,4);
		if(!strcmp("",operRemoteSql))
		{
			return DBTFLV;
		}
		
		DBTFLV=GameDBQuery("GT", ServerIP,0, iIndex, iPageSize,
			operRemoteSql, "gtown_db_gamedb","gtown_db_gamedb", "\%", FamilyName, "\%");
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"���������Ϣ�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"���������Ϣ�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"���������Ϣ�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>����<%s>������Ϣ����ϵͳ����",m_UserName,ServerName,FamilyName);
		logFile.WriteText("GTown",strLog);//��������Ϣд����־��
		return DBTFLV;
	}
	return DBTFLV;
}

//��ѯ�����Ա��Ϣ
vector <CGlobalStruct::TFLV> CGTInfo::GT_MemberInfo(char * ServerName, char * ServerIP,char * FamilyName,int FamilyID,int iIndex,int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	try
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>����<%s>��ϸ��Ϣ",m_UserName,ServerName,FamilyName);
		logFile.WriteText("GTown",strLog);
		
		//��ȡԶ�̲�����Sql���
		operGT.getRemoteSql("GT","GT_Family_MemberInfo",operRemoteSql,4);
		if(!strcmp("",operRemoteSql))
		{
			return DBTFLV;
		}
		DBTFLV=GameDBQuery("GT", ServerIP,0, iIndex, iPageSize,
			operRemoteSql, "gtown_db_gamedb",FamilyID);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"������ϸ��Ϣ�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"������ϸ��Ϣ�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"������ϸ��Ϣ�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>����<%s>��ϸ��Ϣ����ϵͳ����",operName,ServerName,FamilyName);
		logFile.WriteText("GTown",strLog);//��������Ϣд����־��
		return DBTFLV;
	}
	return DBTFLV;
}


//�ʼ���Ϣ��ѯ
vector <CGlobalStruct::TFLV> CGTInfo::GT_MailInfo(char * ServerName, char * ServerIP,char * UserName, int UserID,char* szStartTime,char* szEndTime,int iIndex,int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	try
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>���<%s>�ʼ���Ϣ",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);
		
		operGT.getRemoteSql("GT","GT_MailInfo",operRemoteSql,4);//��ȡԶ�̲�����Sql���
		if(!strcmp("",operRemoteSql))
		{
			return DBTFLV;
		}
		DBTFLV=GameDBQuery("GT", ServerIP, 0, iIndex, iPageSize,operRemoteSql,
				"gtown_db_gamedb","gtown_db_gamedb",UserID,szStartTime,szEndTime);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"����ʼ���Ϣ�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"����ʼ���Ϣ�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"����ʼ���Ϣ�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>���<%s>�ʼ���Ϣ����ϵͳ����",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);//��������Ϣд����־��
		return DBTFLV;
	}
	return DBTFLV;
}

//��ѯ��ұ��俪����Ϣ
vector <CGlobalStruct::TFLV> CGTInfo::GT_TreasureBox_Open(char * ServerName, char * ServerIP,char * UserName, char* szStartTime,char* szEndTime,int iIndex,int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	try
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>���<%s>���俪����Ϣ",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);
		
		operGT.getRemoteSql("GT","GT_TreasureBox_Open",operRemoteSql,4);//��ȡԶ�̲�����Sql���
		if(!strcmp("",operRemoteSql))
		{
			return DBTFLV;
		}
		DBTFLV=GameDBQuery("GT", ServerIP, 0, iIndex, iPageSize,operRemoteSql,
			"gtown_db_gamedb","gtown_db_logdb","gtown_db_gamedb",UserName,szStartTime,szEndTime);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ұ��俪����Ϣ�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ұ��俪����Ϣ�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ұ��俪����Ϣ�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>���<%s>���俪����Ϣ����ϵͳ����",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);//��������Ϣд����־��
		return DBTFLV;
	}
	return DBTFLV;
}

//��ѯ���߹������Ѽ�¼
vector <CGlobalStruct::TFLV> CGTInfo::GT_ItemBuy(char * ServerName, char * ServerIP,char* UserName,
						 int UserID,int type,char * ItemName,char * szStartTime,char * szEndTime,int iIndex,int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	try
	{
	
		sprintf(strLog,"<%s>,��ѯ����<%s>���<%s>���߹������Ѽ�¼",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);
		if(!strcmp(ItemName,""))//û������
		{
			switch(type)
			{
			case 0:
				operGT.getRemoteSql("GT","GT_ItemBuy_0",operRemoteSql,4);
				break;
			case 1:
				operGT.getRemoteSql("GT","GT_ItemBuy_1",operRemoteSql,4);
				break;
			case 2:
				operGT.getRemoteSql("GT","GT_ItemBuy_2",operRemoteSql,4);
				break;
			case 3:
				operGT.getRemoteSql("GT","GT_ItemBuy_3",operRemoteSql,4);
				break;
			default:
				break;
			}
			if(!strcmp("",operRemoteSql))
			{
				operGT.BuildTLV("���ݿ����Ҳ�����Ӧ��sql���");
				operGT.SendBuf(pGameSess,CEnumCore::Msg_Category::GT_ADMIN,CEnumCore::ServiceKey::GT_ItemConsume_Query_Resp,operGT.GetSendBuf(),operGT.GetSendLength());
				return false;
			}
			DBTFLV=GameDBQuery("GT",ServerIP,0,iIndex,iPageSize,operRemoteSql, UserID, szStartTime, szEndTime);//����Զ�̲�����sql���
		}
		else
		{
			switch(type)
			{
			case 0:
				operGT.getRemoteSql("GT","GT_ItemBuy_4",operRemoteSql,4);
				break;
			case 1:
				operGT.getRemoteSql("GT","GT_ItemBuy_5",operRemoteSql,4);
				break;
			case 2:
				operGT.getRemoteSql("GT","GT_ItemBuy_6",operRemoteSql,4);
				break;
			case 3:
				operGT.getRemoteSql("GT","GT_ItemBuy_7",operRemoteSql,4);
				break;
			default:
				break;
			}
			if(!strcmp("",operRemoteSql))
			{
				return DBTFLV;
			}
			DBTFLV=GameDBQuery("GT",ServerIP,0,iIndex,iPageSize,operRemoteSql,ItemName, UserID, szStartTime, szEndTime);//����Զ�̲�����sql���
		}
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ҵ��߹������Ѽ�¼�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ҵ��߹������Ѽ�¼�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"��ҵ��߹������Ѽ�¼�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>���<%s>���߹������Ѽ�¼����ϵͳ����",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);//��������Ϣд����־��
		return DBTFLV;
	}
	return DBTFLV;
}



//Ѫƴս��
vector <CGlobalStruct::TFLV> CGTInfo::GT_Mcoin_Rank(char * ServerName,char * ServerIP,int iType,int iIndex,int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	try
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>Ѫƴս��",operName,ServerName);
		switch(iType)
		{
		case 0:
			operGT.getRemoteSql("GT","GT_Mcoin_Rank0",operRemoteSql,4);//��ȡԶ�̲�����Sql���
			break;
		case 1:
			operGT.getRemoteSql("GT","GT_Mcoin_Rank1",operRemoteSql,4);//��ȡԶ�̲�����Sql���
			break;
		case 2:
			operGT.getRemoteSql("GT","GT_Mcoin_Rank2",operRemoteSql,4);//��ȡԶ�̲�����Sql���
			break;
		default:
			break;
		}
		if(!strcmp("",operRemoteSql))
		{
			return DBTFLV;
		}
		
		DBTFLV=GameDBQuery("GT",ServerIP,0,iIndex,iPageSize,operRemoteSql,"gtown_db_gamedb","gtown_db_gamedb");
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�鿴����Ѫƴս���ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�鿴����Ѫƴս���ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�鿴����Ѫƴս�������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>Ѫƴս������ϵͳ����",operName,ServerName);
		logFile.WriteText("GTown",strLog);//��������Ϣд����־��
		return DBTFLV;
	}
	return DBTFLV;
}

//�ռ�����
vector <CGlobalStruct::TFLV> CGTInfo::GT_Sns_Query(char * ServerName,char * ServerIP,int iType,int iIndex,int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	try
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>�ռ�����",operName,ServerName);
		logFile.WriteText("GTown",strLog);
		switch(iType)
		{
		case 0:
			operGT.getRemoteSql("GT","GT_Sns_Query0",operRemoteSql,4);//��ȡԶ�̲�����Sql���
			break;
		case 1:
			operGT.getRemoteSql("GT","GT_Sns_Query1",operRemoteSql,4);//��ȡԶ�̲�����Sql���
			break;
		case 2:
			operGT.getRemoteSql("GT","GT_Sns_Query2",operRemoteSql,4);//��ȡԶ�̲�����Sql���
			break;
		default:
			break;
		}
		if(!strcmp("",operRemoteSql))
		{
			return DBTFLV;
		}
		DBTFLV=GameDBQuery("GT",ServerIP,0,iIndex,iPageSize,operRemoteSql);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�鿴�����ռ������ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�鿴�����ռ������ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�鿴�����ռ����������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>�ռ����з���ϵͳ����",operName,ServerName);
		logFile.WriteText("GTown",strLog);//��������Ϣд����־��
		return DBTFLV;
	}
	return DBTFLV;
}

//��������
vector <CGlobalStruct::TFLV> CGTInfo::GT_Attr_Queue(char * ServerName,char * ServerIP,int iType,int iIndex,int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	try
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>��������",operName,ServerName);
		logFile.WriteText("GTown",strLog);
		
		switch(iType)
		{
		case 0:
			operGT.getRemoteSql("GT","GT_Attr_Queue0",operRemoteSql,4);//��ȡԶ�̲�����Sql���
			break;
		case 1:
			operGT.getRemoteSql("GT","GT_Attr_Queue1",operRemoteSql,4);//��ȡԶ�̲�����Sql���
			break;
		case 2:
			operGT.getRemoteSql("GT","GT_Attr_Queue2",operRemoteSql,4);//��ȡԶ�̲�����Sql���
			break;
		case 3:
			operGT.getRemoteSql("GT","GT_Attr_Queue3",operRemoteSql,4);//��ȡԶ�̲�����Sql���
			break;
		case 4:
			operGT.getRemoteSql("GT","GT_Attr_Queue4",operRemoteSql,4);//��ȡԶ�̲�����Sql���
			break;
		case 5:
			operGT.getRemoteSql("GT","GT_Attr_Queue5",operRemoteSql,4);//��ȡԶ�̲�����Sql���
			break;
		default:
			break;
		}
		if(!strcmp("",operRemoteSql))
		{
			return DBTFLV;
		}
		DBTFLV=GameDBQuery("GT",ServerIP,0,iIndex,iPageSize,operRemoteSql,"gtown_db_gamedb","gtown_db_gamedb");
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�鿴�������������ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�鿴�������������ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�鿴�����������������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>�������з���ϵͳ����",operName,ServerName);
		logFile.WriteText("GTown",strLog);//��������Ϣд����־��
		return DBTFLV;
	}
	return DBTFLV;
}

//��Ƭ����
vector <CGlobalStruct::TFLV> CGTInfo::GT_Card_Record(char * ServerName,char * ServerIP,int iType,int iIndex,int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	try
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>��Ƭ����",operName,ServerName);
		logFile.WriteText("GTown",strLog);
		switch(iType)
		{
		case 0:
			operGT.getRemoteSql("GT","GT_Card_Record0",operRemoteSql,4);//��ȡԶ�̲�����Sql���
			if(!strcmp("",operRemoteSql))
			{
				return DBTFLV;
			}
			DBTFLV=GameDBQuery("GT",ServerIP,0,iIndex,iPageSize,operRemoteSql,"gtown_db_carddb","gtown_db_gamedb");
			break;
		case 1:
			operGT.getRemoteSql("GT","GT_Card_Record1",operRemoteSql,4);//��ȡԶ�̲�����Sql���
			if(!strcmp("",operRemoteSql))
			{
				return DBTFLV;
			}
			DBTFLV=GameDBQuery("GT",ServerIP,0,iIndex,iPageSize,operRemoteSql,"gtown_db_carddb","gtown_db_gamedb");
			break;
		case 2:
			operGT.getRemoteSql("GT","GT_Card_Record2",operRemoteSql,4);//��ȡԶ�̲�����Sql���
			if(!strcmp("",operRemoteSql))
			{
				return DBTFLV;
			}
			DBTFLV=GameDBQuery("GT",ServerIP,0,iIndex,iPageSize,operRemoteSql,"gtown_db_gamedb","gtown_db_gamedb","gtown_db_gamedb");
			break;
		case 3:
			operGT.getRemoteSql("GT","GT_Card_Record3",operRemoteSql,4);//��ȡԶ�̲�����Sql���
			if(!strcmp("",operRemoteSql))
			{
				return DBTFLV;
			}
			DBTFLV=GameDBQuery("GT",ServerIP,0,iIndex,iPageSize,operRemoteSql,"gtown_db_gamedb","gtown_db_gamedb");
			break;
		case 4:
			operGT.getRemoteSql("GT","GT_Card_Record4",operRemoteSql,4);//��ȡԶ�̲�����Sql���
			if(!strcmp("",operRemoteSql))
			{
				return DBTFLV;
			}
			DBTFLV=GameDBQuery("GT",ServerIP,0,iIndex,iPageSize,operRemoteSql,"gtown_db_logdb","gtown_db_gamedb");
			break;
		default:
			break;
		}
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�鿴������Ƭ�����ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�鿴������Ƭ�����ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�鿴������Ƭ���������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>��Ƭ���з���ϵͳ����",operName,ServerName);
		logFile.WriteText("GTown",strLog);//��������Ϣд����־��
		return DBTFLV;
	}
	return DBTFLV;
}






//����
vector <CGlobalStruct::TFLV> CGTInfo::GT_FriendTrade_Queue(char * ServerName,char * ServerIP,int iType,int iIndex,int iPageSize)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	try
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>��������",operName,ServerName);
		logFile.WriteText("GTown",strLog);
	
		switch(iType)
		{
		case 0://�������
			operGT.getRemoteSql("GT","GT_FriendTrade_Queue0",operRemoteSql,4);//��ȡԶ�̲�����Sql���
			break;
		case 1://���±���
			operGT.getRemoteSql("GT","GT_FriendTrade_Queue1",operRemoteSql,4);//��ȡԶ�̲�����Sql���
			break;
		case 2://�ʲ����
			operGT.getRemoteSql("GT","GT_FriendTrade_Queue2",operRemoteSql,4);//��ȡԶ�̲�����Sql���
			break;
		case 3://�ֽ����
			operGT.getRemoteSql("GT","GT_FriendTrade_Queue3",operRemoteSql,4);//��ȡԶ�̲�����Sql���
			break;
		case 4://�ۿ����
			operGT.getRemoteSql("GT","GT_FriendTrade_Queue4",operRemoteSql,4);//��ȡԶ�̲�����Sql���
			break;
		default:
			break;
		}
		if(!strcmp("",operRemoteSql))
		{
			return DBTFLV;
		}

		DBTFLV=GameDBQuery("GT",ServerIP,0,iIndex,iPageSize,operRemoteSql);
		
		
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�鿴�������������ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�鿴�������������ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�鿴�����������������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>��������",operName,ServerName);
		logFile.WriteText("GTown",strLog);//��������Ϣд����־��
		return DBTFLV;		
	}
	return DBTFLV;
}


//���߲�ѯ
vector <CGlobalStruct::TFLV> CGTInfo::GT_Select_Item(char * ServerName,char * ServerIP,char * ItemName)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	try
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>���߲�ѯ",operName,ServerName);
		logFile.WriteText("GTown",strLog);
		
		operGT.getRemoteSql("GT","GT_Select_Item",operRemoteSql,4);//��ȡԶ�̲�����Sql���


		if(!strcmp("",operRemoteSql))
		{
			return DBTFLV;
		}
		DBTFLV=GameDBQuery(
			"GT",ServerIP,0,0,0,operRemoteSql,"gtown_db_gamedb","gtown_db_gamedb", "\%",ItemName,"\%");//����Զ�̲�����sql���

	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�鿴���߲�ѯ�ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�鿴���߲�ѯ�ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"�鿴���߲�ѯ�����쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch(...)
	{
		sprintf(strLog,"<%s>,��ѯ����<%s>���߲�ѯ",operName,ServerName);
		logFile.WriteText("GTown",strLog);//��������Ϣд����־��
		return DBTFLV;
	}
	return DBTFLV;
}
//�ظ���������˿�
vector <CGlobalStruct::TFLV> CGTInfo::GT_Back_Money(int userByID,char * ServerName, char * ServerIP, char * UserName, int iID,int iType,int iMoney,char * strItemName)
{
	vector <CGlobalStruct::TFLV> DBTFLV;
	COperatorGT operGT;
	CGlobalStruct::TFLV m_tflv;
	m_tflv.m_tagName=CEnumCore::TagName::MESSAGE;
	m_tflv.m_tagFormat=CEnumCore::TagFormat::TLV_STRING;
	try
	{
		sprintf(strLog,"<%s>,����<%s>����ظ���������˿�",operName,ServerName,UserName);
		logFile.WriteText("GTown",strLog);
		

		switch(iType)
		{
		case 0:
		case 2:
		case 4:
		case 6:
			operGT.getRemoteSql("GT","GT_IS_BackMoney0",operRemoteSql,4);//��ȡԶ�̲�����Sql���
			break;
		default:
			operGT.getRemoteSql("GT","GT_IS_BackMoney1",operRemoteSql,4);//��ȡԶ�̲�����Sql���	
			break;
		}
	
		if(!strcmp("",operRemoteSql))
		{
			return DBTFLV;
		}
		char strResult[256];
		sprintf(operszSql,operRemoteSql,UserName,strItemName);
		operGT.QuerySingleValue("GT",ServerIP,0,operszSql,strResult);
		if(StrToInt(strResult)<=1)
		{
			sprintf((char *)&m_tflv.lpdata,"���%s,δ�ظ��������%s",UserName,strItemName);
			m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
			DBTFLV.push_back(m_tflv);
			return DBTFLV;
		}

		switch(iType)
		{
		case 0:
		case 2:
		case 4:
		case 6:
			operGT.getRemoteSql("GT","GT_Back_Money0",operRemoteSql,4);//��ȡԶ�̲�����Sql���
			break;
		default:
			operGT.getRemoteSql("GT","GT_Back_Money1",operRemoteSql,4);//��ȡԶ�̲�����Sql���	
			break;
		}
		
		if(!strcmp("",operRemoteSql))
		{
			return DBTFLV;
		}

		int tmpLength=GameDBOper("GT",ServerIP,0,operRemoteSql,iID,UserName,UserName,iMoney);
		
		if(tmpLength>0)
		{
			sprintf((char *)&m_tflv.lpdata,"���%s,�˿���%i�ɹ�",UserName,iMoney);
		}
		else
		{
			sprintf((char *)&m_tflv.lpdata,"���%s,�˿���%i�ɹ�",UserName,iMoney);
		}
		logFile.WriteDBLog(userByID,"����Online","GT_Back_Money",ServerIP,(char *)&m_tflv.lpdata);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"����˿��ڴ治��,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"����˿��ļ��쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	catch (CException* e)
	{
		e->GetErrorMessage(errMessage,256);
		sprintf(errMessage,"����˿������쳣,������Ϣ:%s",errMessage);
		logFile.WriteErrorLog("GT",errMessage);
		return DBTFLV;
	}
	m_tflv.m_tvlength=strlen((char *)&m_tflv.lpdata);
	DBTFLV.push_back(m_tflv);
	return DBTFLV;

}



