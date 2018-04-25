#include "StdAfx.h"
#include "SDOInfo.h"

#pragma warning(disable:4129) 

CSDOInfo::CSDOInfo(void)
{
}

CSDOInfo::~CSDOInfo(void)
{
}

//��ʼ�����еı���
COperVector * CSDOInfo::initialize(CSession * pSess,CEnumCore::Message_Tag_ID MessageTagID, unsigned char * pbuf, int length)
{
	COperVector * operVector = NULL;
	try
	{
		ZeroMemory(localIP, 20);
		ZeroMemory(SDO_DBName, 64);
		ZeroMemory(Log_DBName, 64);
		ZeroMemory(Item_DBName, 64);
		logFile.ReadValue("scheme", "DATABASE","Address",localIP, 20);//��ȡ���ص����ݿ��ַ
		logFile.ReadValue("SDO", "DATABASE","DBName",SDO_DBName, 64);//��ȡ��Ϸ���ݿ�����
		logFile.ReadValue("SDO", "DATABASE","LogDBName",Log_DBName, 64);//��ȡ��־���ݿ�����
		logFile.ReadValue("SDO", "DATABASE","ItemDBName",Item_DBName, 64);//��ȡ�������ݿ�����
		
		//���û����ʼ������
		operVector = CGameInfo::initialize(pSess, MessageTagID, pbuf, length);

	}
	catch(...)
	{
	}
	return operVector;
}

//��ѯ��һ���������Ϣ
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_UserInfo(int userByID, char * ServerName, char * ServerIP,char * Account,char * NickName,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{		
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_UserInfo", "", operName, ServerName, Account);
		operSDO.WriteText("SDO",strLog);
		
		//��ȡԶ�̲�����Sql���
		if(strcmp("",Account))
		{
			if(!operSDO.getRemoteSql("SDO", "SDO_CharInfo_Account",operRemoteSql,1))
			{
				return DBTFLV;//�޷��õ���ѯ��ҽ�ɫ��Ϣ��sql���
			}
			DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql,SDO_DBName,SDO_DBName,SDO_DBName,SDO_DBName,Account);

		}
		else
		{
			if(!operSDO.getRemoteSql("SDO", "SDO_CharInfo_Nick",operRemoteSql,1))
			{
				return DBTFLV;//�޷��õ���ѯ��ҽ�ɫ��Ϣ��sql���
			}
//			printf(operRemoteSql,SDO_DBName,SDO_DBName,SDO_DBName,SDO_DBName,"\%",NickName,"\%");
			DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql,SDO_DBName,SDO_DBName,SDO_DBName,SDO_DBName,"\%",NickName,"\%");
		}
		
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//��ѯ����ʺ���Ϣ
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_AccountInfo(int userByID, char * ServerName, char * ServerIP,char * Account,char * NickName,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_AccountInfo", "", operName,ServerName,Account);
		operSDO.WriteText("SDO",strLog);
		
		//��ȡԶ�̲�����Sql���
		if(strcmp("",Account))
		{
			if(!operSDO.getRemoteSql("SDO","SDO_MemberInfo_Account",operRemoteSql, 1))
			{
				return DBTFLV;//�޷��õ���ѯ����ʺ���Ϣ��sql���
			}
//			printf(operRemoteSql,SDO_DBName,SDO_DBName,Account);
			DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql,SDO_DBName,SDO_DBName,Account);

		}
		else
		{
			if(!operSDO.getRemoteSql("SDO","SDO_MemberInfo_Nick",operRemoteSql, 1))
			{
				return DBTFLV;//�޷��õ���ѯ����ʺ���Ϣ��sql���
			}
//			sprintf(operszSql,operRemoteSql,SDO_DBName,SDO_DBName,"\%",NickName,"\%");
			DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql,SDO_DBName,SDO_DBName,"\%",NickName,"\%");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//��ѯ�����Ʒ
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_ItemInfo(int userByID, char * ServerName, char * ServerIP,char * Account, int UserID,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_ItemInfo", "", operName,ServerName,Account);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))
		{
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		if(!operSDO.getRemoteSql("SDO","SDO_ItemInfo_Query",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ���ѯ�����Ʒ��Ϣ��sql���
		}
		DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql,Item_DBName,Item_DBName,UserID);
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//��ѯ��ҳ�����Ϣ
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_PetInfo(int userByID,char * ServerName, char * ServerIP,char * Account, int UserID,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_PetInfo", "", operName,ServerName,Account);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))
		{
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_PetInfo_Query",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ���ѯ��ҳ�����Ϣ��sql���
		}
//		sprintf(operszSql,operRemoteSql,Item_DBName,Item_DBName,Item_DBName,UserID);//����������ѯ��sql���
		DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql,Item_DBName,Item_DBName,Item_DBName,UserID);
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//�鿴��ҵĳ���ɾ����Ϣ
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_PetDrop(int userByID, char * ServerName, char * ServerIP,char * Account, int UserID,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_PetDrop", "", operName,ServerName,Account);
		operSDO.WriteText("SDO",strLog);
		char tempServerIp[20];

		if(!operSDO.GetServerIP(tempServerIp,ServerIP,3))
		{
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_PetDrop_Query",operRemoteSql, 1))
		{
			return DBTFLV;//�޷��õ���ѯ��ҳ���ɾ����Ϣ��sql���
		}
//		sprintf(operszSql,operRemoteSql,Log_DBName,SDO_DBName,UserID);//����������ѯ��sql���
		DBTFLV=GameDBQuery("SDO",tempServerIp,0,iIndex,iPageSize,operRemoteSql,Log_DBName,SDO_DBName,UserID);		
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//��ѯ����˺ŵ�¼��¼
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_LoginLog(int userByID, char * ServerName, char * ServerIP,char * Account,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_LoginLog", "", operName,ServerName,Account);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_UserLogin_Query",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ���ѯ����˺ŵ�¼��¼��sql���
		}
//		sprintf(operszSql,operRemoteSql,Log_DBName,Account);//����������ѯ��sql���
		DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql,Log_DBName,Account);

	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//��ѯ�����Ʒ���ͼ�¼
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_UserTrade_Log(int userByID, char * ServerName, char * ServerIP,char * Account,char * NickName,int itype, int iIndex,int iPageSize)
{	
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_UserTrade_Log", "", operName,ServerName,NickName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,3))
		{
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		if(itype == 1)//�����Ϊ������
		{
			if(!operSDO.getRemoteSql("SDO", "SDO_TradeLog_Sender",operRemoteSql,1))
			{
				return DBTFLV;//�޷��õ���ѯ�����Ʒ���ͼ�¼��sql���
			}
//			printf(operRemoteSql, Log_DBName, Account, Log_DBName, Account);
			DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql, Log_DBName, Account, Log_DBName, Account);
		}
		else if(itype == 2)//�����Ϊ������
		{
			if(!operSDO.getRemoteSql("SDO","SDO_TradeLog_Recver",operRemoteSql,1))
			{
				return DBTFLV;//�޷��õ���ѯ�����Ʒ���ͼ�¼��sql���
			}
//			printf(operRemoteSql, Log_DBName, "\%", NickName, "\%", Log_DBName, "\%", NickName, "\%");
			DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql, Log_DBName, "\%", NickName, "\%", Log_DBName, "\%", NickName, "\%");
		}
		
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//��ѯ������Ѽ�¼
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_UserConsume(int userByID, char * ServerName, char * ServerIP,char * Account,char * NickName,int iMoneytype, char *ItemName, char *BeginTime, char *EndTime, int iIndex,int iPageSize)
{	
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_UserConsume", "", operName,ServerName,NickName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_UserConsume",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ���ѯ������Ѽ�¼��sql���
		}
		DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql,Log_DBName,Account,iMoneytype,BeginTime,EndTime,"\%",ItemName,"\%",
							Log_DBName,Account,iMoneytype,BeginTime,EndTime,"\%",ItemName,"\%");
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//�鿴��ҵ�ǰ״̬(������/����/����״̬)
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_UserStatus(int userByID, char * ServerName, char * ServerIP,char * Account,int UserID,int iIndex,int iPageSize)
{	
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_UserStatus", "", operName,ServerName,Account);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO","SDO_UserStatus",operRemoteSql, 1))
		{
			return DBTFLV;//�޷��õ��鿴��ҵ�ǰ״̬��sql���
		}
//		printf(operRemoteSql,SDO_DBName,SDO_DBName,UserID);
		DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql,SDO_DBName,SDO_DBName,UserID);
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//�鿴��һ���״̬
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_UserMarriage(int userByID, char * ServerName, char * ServerIP,char * Account,char * NickName,int UserID,int iIndex,int iPageSize)
{	
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_UserMarriage", "", operName,ServerName,NickName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO","SDO_UserMarriage",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ��鿴��һ���״̬��sql���
		}
		DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql,UserID,UserID,UserID,UserID,SDO_DBName,SDO_DBName,SDO_DBName,UserID,UserID);
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//�鿴���ҡҡ�ֻ����Ʒ
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_YYItem_QUERY(int userByID, char * ServerName, char * ServerIP,char * Account,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;	
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_YYItem_QUERY", "", operName,ServerName,Account);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,3))
		{
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_RewardItem_Query",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ��鿴���ҡҡ�ֻ����Ʒ��sql���
		}
		DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql,Log_DBName,Account);		
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//ǿ���������
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_KickPlayer_Off(int userByID, char * ServerName, char * ServerIP,char * Account, int UserID)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_KickPlayer", "", "", operName,ServerName,Account);
		
		int IsOnline = 0;
		if(!operSDO.getRemoteSql("SDO","SDO_UserOnline_Status",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ���ȡ�������״̬��sql���
		}
		operSDO.QuerySingleValue(&IsOnline, "SDO", ServerIP, 0, operRemoteSql, SDO_DBName, UserID);//��ȡ����Ƿ����ߵ�״̬
		
		if(IsOnline != 1)//��Ҳ�����
		{
			DBTFLV = operSDO.ReturnOpMsg(strLog,"SDO_UserNotOnline");//��������Ϣд����־�в����ش�����Ϣ
		}
		else
		{
			if(!operSDO.getRemoteSql("SDO", "SDO_KickPlayer_Off",operRemoteSql,1))
			{
				return DBTFLV;//�޷��õ�ǿ��������ߵ�sql���
			}
			
			int tmpLength = 0;
			tmpLength = GameDBOper("SDO",ServerIP,0,operRemoteSql,SDO_DBName,UserID);
			if(tmpLength == 0)
			{
				DBTFLV = operSDO.ReturnOpMsg(strLog,"KickPlayer_Fail");//��������Ϣд����־�в����ش�����Ϣ

			}
			else
			{
				DBTFLV = operSDO.ReturnOpMsg(strLog,"KickPlayer_Success");//��������Ϣд����־�в����ش�����Ϣ				
			}
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//��ѯ��ҷ�ͣ״̬
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_UserBanStatus(int userByID, char * ServerName, char * ServerIP,char * Account,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_UserBanStatus", "", operName,ServerName,Account);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO","SDO_Banishment_Query",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ���ѯ��ҷ�ͣ״̬��sql���
		}
//		sprintf(operszSql,operRemoteSql,SDO_DBName,Account);
		DBTFLV=GameDBQuery("SDO",ServerIP,0,iIndex,iPageSize,operRemoteSql,SDO_DBName,Account);
		if (DBTFLV.empty())
		{
			DBTFLV = operSDO.ReturnOpMsg(strLog,"ACOUNT_CLOSEED");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//��ͣ����ʺ�
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_Account_Close(int userByID, char * ServerName, char * ServerIP,char * Account,char *BanReason)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
	
		operSDO.GetLogText(strLog, "SDO_Account_Close", "", "", operName,ServerName,Account,BanReason);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO","SDO_Account_Close",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ���ͣ����ʺŵ�sql���
		}
		int tmpLength = 0;
		tmpLength = GameDBOper("SDO",ServerIP, 0,operRemoteSql,SDO_DBName,SDO_DBName,Account);
		if(tmpLength == 0)
		{
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Account_Close_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_Account_Close", "", "Success", operName,ServerName,Account,BanReason);
			operSDO.WriteDBLog(userByID,"SDO","SDO_Account_Close",ServerIP,strLog);
			
			if(!operSDO.getRemoteSql("SDO","SDO_BanAccount_Local",operRemoteSql,1))
			{
				return DBTFLV;//�޷��õ���ͣ����ʺŵ�sql���
			}
			tmpLength = 0;
//			printf(operRemoteSql, userByID, Account, BanReason, ServerIP);
			tmpLength = GameDBOper("SDO",localIP, 4, operRemoteSql, userByID, Account, BanReason, ServerIP);
			
			if(tmpLength == 0)
			{
				DBTFLV = operSDO.ReturnOpMsg(strLog,"MiddleState");
			}
			else
			{
				DBTFLV = operSDO.ReturnOpMsg(strLog,"Account_Close_Success");
			}
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//��ѯ���б���ͣ�ʺ��б�
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_QueryBan_All(int userByID, char * ServerName, char * ServerIP,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage1", "SDO_QueryBanAll", "", operName,ServerName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO","SDO_QueryBan_All",operRemoteSql,1))
		{
			return false;//�޷��õ���ѯ���б���ͣ�ʺ��б��sql���
		}
		DBTFLV=GameDBQuery("SDO",localIP,4,iIndex,iPageSize,operRemoteSql,ServerIP);
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//�������ʺ�
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_Account_Open(int userByID, char * ServerName, char * ServerIP,char * Account,char *UnbindReason)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_Account_Open", "", "", operName,ServerName,Account,UnbindReason);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO","SDO_Account_Open",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ��������ʺŵ�sql���
		}
		int tmpLength = 0;
		tmpLength = GameDBOper("SDO",ServerIP, 0,operRemoteSql,SDO_DBName,Account);
		
		if(tmpLength == 0)
		{
			operSDO.WriteDBLog(userByID,"SDO","SDO_Account_Open",ServerIP,strLog);
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Account_Open_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_Account_Open", "", "Success", operName,ServerName,Account,UnbindReason);
			operSDO.WriteDBLog(userByID,"SDO","SDO_Account_Open",ServerIP,strLog);
			
			if(!operSDO.getRemoteSql("SDO","SDO_UnbindAccount_Local",operRemoteSql,1))
			{
				return DBTFLV;//�޷��õ����ؽ������ʺŵ�sql���
			}

			tmpLength = 0;
			tmpLength = GameDBOper("SDO",localIP, 0,operRemoteSql, Account, ServerIP);
			
			if(tmpLength == 0)
			{
				DBTFLV = operSDO.ReturnOpMsg(strLog,"MiddleState");
			}
			else
			{
				DBTFLV = operSDO.ReturnOpMsg(strLog,"Account_Open_Success");
			}
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//��ѯ������Ϣ�б�
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_NoticeInfo(int userByID, char * ServerName, char * ServerIP,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage2", "SDO_NoticeInfo", "", operName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_NoticeInfo",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ���ѯ���б���ͣ�ʺ��б��sql���
		}
//		sprintf(operszSql,operRemoteSql);//����������ѯ��sql���
		DBTFLV = BroadTask_Query("SDO",operRemoteSql,iIndex,iPageSize);	
//		DBTFLV=GameDBQuery("SDO",localIP,4,iIndex,iPageSize,operRemoteSql);		
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//��ӹ�����Ϣ
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_AddNotice(int userByID, char * ServerName, char * ServerIP, char *BoardMessage, char *BeginTime, char *EndTime, int interval, int iNoticeType)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{

		int iAtonce ;
		char strNotice[10];
		ZeroMemory(strNotice, 10);
/*
		if (iNoticeType == 1)	//��ʱ����
		{
			logFile.ReadValue("SDO", "NOTICETYPE", "NOtice_inTime", strNotice, 10);
			iAtonce = -1;
		}
		else					//��ʱ����
		{
			logFile.ReadValue("SDO", "NOTICETYPE", "NOtice_Time", strNotice, 10);
			iAtonce = 0;
		}
*/
		//��ȡ������������<strNotice>�Լ����ͱ�ʶ<iAtonce>
		GetNoticeType(strNotice, iAtonce, iNoticeType);

		//д���͹������־
		operSDO.GetLogText(strLog, "SDO_AddNotice", "", "", operName, ServerName, BoardMessage,BeginTime, EndTime, interval, strNotice);
		operSDO.WriteText("SDO",strLog);
		
		sprintf(operszSql,"exec SDO_TaskList_Insert %i,'%s','%s','%s','%s',%i", 
			userByID,ServerIP,BoardMessage,BeginTime, EndTime, interval);

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO",localIP,4,"exec SDO_TaskList_Insert %i,'%s','%s','%s','%s',%i,%i", 
								userByID,ServerIP,BoardMessage,BeginTime, EndTime, interval, iAtonce);
		
		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_AddNotice", "", "Fail", operName, ServerName, BoardMessage,BeginTime, EndTime, interval, strNotice);
			operSDO.WriteDBLog(userByID,"SDO","SDO_AddNotice",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Add_Notice_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_AddNotice", "", "Success", operName, ServerName, BoardMessage,BeginTime, EndTime, interval, strNotice);
			operSDO.WriteDBLog(userByID,"SDO","SDO_AddNotice",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Add_Notice_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//��ѯ���͹���Ĵ����б���Ϣ
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_BoardCity_Query(int userByID, int taskID,int iIndex,int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{	
		operSDO.GetLogText(strLog, "SDO_BoardCity_Query", "", "", operName, taskID);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_BoardCity_Query",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ���ѯ���͹���Ĵ����б���Ϣ��sql���
		}
		DBTFLV = GameDBQuery("SDO",localIP,4, iIndex, iPageSize, operRemoteSql,taskID);
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//�޸Ĺ�����Ϣ
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_EditNotice(int userByID, char * ServerName, char * ServerIP, int taskID, char *BoardMessage, char *BeginTime, char *EndTime, int interval,int status,int iNoticeType)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	
	try
	{
		int iAtonce ;
		char strNotice[10];
		ZeroMemory(strNotice, 10);
/*
		if (iNoticeType == 1)	//��ʱ����
		{
			logFile.ReadValue("SDO", "NOTICETYPE", "NOtice_inTime", strNotice, 10);
			iAtonce = -1;
		}
		else					//��ʱ����
		{
			logFile.ReadValue("SDO", "NOTICETYPE", "NOtice_Time", strNotice, 10);
			iAtonce = 0;
		}
*/		
		//��ȡ������������<strNotice>�Լ����ͱ�ʶ<iAtonce>
		GetNoticeType(strNotice, iAtonce, iNoticeType);
		operSDO.GetLogText(strLog, "SDO_EditNotice", "", "", operName, 
			taskID,ServerName,BoardMessage,BeginTime,EndTime,interval,status, strNotice);
		operSDO.WriteText("SDO",strLog);

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", localIP, 4, "exec SDO_TaskList_Update %i,'%s',%i,'%s','%s','%s',%i,%i,%i", 
			userByID,ServerIP,taskID,BoardMessage,BeginTime, EndTime, interval, status,iAtonce);
		
		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_EditNotice", "", "Fail",  operName, taskID,ServerName,BoardMessage,BeginTime,EndTime,interval,status, strNotice);
			operSDO.WriteDBLog(userByID,"SDO","SDO_EditNotice",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Edit_Notice_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_EditNotice", "", "Success",  operName, taskID,ServerName,BoardMessage,BeginTime,EndTime,interval,status, strNotice);
			operSDO.WriteDBLog(userByID,"SDO","SDO_EditNotice",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Edit_Notice_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//��ѯ���������Ϣ
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_FamilyInfo(int userByID, char * ServerName, char * ServerIP, char *FamilyName, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "SDO_FamilyInfo", "SDO_UserInfo", "", operName, ServerName, FamilyName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO","SDO_FamilyInfo_Query",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ���ѯ���������Ϣ��sql���
		}
//		printf(operRemoteSql,SDO_DBName,SDO_DBName,SDO_DBName, "\%", FamilyName, "\%");
		DBTFLV = GameDBQuery("SDO",ServerIP,0, iIndex, iPageSize, operRemoteSql,SDO_DBName,SDO_DBName,SDO_DBName, "\%", FamilyName, "\%");		
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//��ѯ�����Ա��Ϣ
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_FamilyMember(int userByID, char * ServerName, char * ServerIP, int FamilyID, char *FamilyName, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "SDO_FamilyInfo", "SDO_MemberInfo", "", operName, ServerName, FamilyName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_FamilyMember_Query",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ���ѯ�����Ա��Ϣ��sql���
		}
//		printf(operRemoteSql,SDO_DBName,SDO_DBName,SDO_DBName,FamilyID);
		DBTFLV = GameDBQuery("SDO",ServerIP,0, iIndex, iPageSize, operRemoteSql,SDO_DBName,SDO_DBName,SDO_DBName,FamilyID);		

	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}

	return DBTFLV;
}

//��ѯ�����м����Ա��Ϣ
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_FamilyFormal(int userByID, char * ServerName, char * ServerIP, int FamilyID, char *FamilyName, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "SDO_FamilyInfo", "SDO_ApplicationMember", "", operName, ServerName, FamilyName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_Family_ApplicMember",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ���ѯ�����м����Ա��Ϣ��sql���
		}
//		printf(operRemoteSql,SDO_DBName,SDO_DBName,FamilyID);
		DBTFLV = GameDBQuery("SDO",ServerIP,0, iIndex, iPageSize, operRemoteSql,SDO_DBName,SDO_DBName,FamilyID);
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//��ѯ����ѫ����Ϣ
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_FamilyBadge(int userByID, char * ServerName, char * ServerIP, int FamilyID, char *FamilyName, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "SDO_FamilyInfo", "SDO_BadgeInfo", "", operName, ServerName, FamilyName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO","SDO_FamilyBadge_Query",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ���ѯ����ѫ����Ϣ��sql���
		}
//		printf(operRemoteSql,SDO_DBName,SDO_DBName,SDO_DBName,FamilyID);
		DBTFLV = GameDBQuery("SDO",ServerIP,0, iIndex, iPageSize, operRemoteSql,SDO_DBName,SDO_DBName,SDO_DBName,FamilyID);
	}
	catch(...)
	{
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//��ѯ�����������ϸ
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_FamilyDonate(int userByID, char * ServerName, char * ServerIP, int FamilyID, char *FamilyName, char *BeginTime, char *EndTime,int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "SDO_FamilyInfo", "SDO_DonateInfo", "", operName, ServerName, FamilyName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO","SDO_FamilyDonate_Query",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ���ѯ�����������ϸ��sql���
		}
//		printf(operRemoteSql,SDO_DBName,SDO_DBName,FamilyID,BeginTime,EndTime);
		DBTFLV = GameDBQuery("SDO",ServerIP,0, iIndex, iPageSize, operRemoteSql,SDO_DBName,SDO_DBName,FamilyID,BeginTime,EndTime);
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//��ѯ���幺���¼
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_FamilyConsume(int userByID, char * ServerName, char * ServerIP, int FamilyID, char *FamilyName, char *BeginTime, char *EndTime,int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "SDO_FamilyInfo", "SDO_FamilyConsume", "", operName, ServerName, FamilyName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO","SDO_FamilyConsume_Query",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ���ѯ�����������ϸ��sql���
		}
		DBTFLV = GameDBQuery("SDO",ServerIP,0, iIndex, iPageSize, operRemoteSql,SDO_DBName,SDO_DBName,SDO_DBName,FamilyID,BeginTime,EndTime);		

	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//�޸ļ����Աְ��
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_ModiMemDuty(int userByID, char * ServerName, char * ServerIP, char *NickName, int UserID, int FamilyID, char *FamilyName, int type)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	
	try
	{
		int type = 0;
		type = StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::SDO_Type,0)).lpdata);//��Ա��ְ������
		char MemDuty[10];
		ZeroMemory(MemDuty, 10);
		if(type == 255)
		{
			logFile.ReadValue("SDO","Code","Shaikh",MemDuty,10);//�峤
		}
		if(type == 2)
		{
			logFile.ReadValue("SDO","Code","AssistShaikh",MemDuty,10);//���峤
		}
		if(type == 0)
		{
			logFile.ReadValue("SDO","Code","CommonMem",MemDuty,10);//��ͨ��Ա
		}
		if(type == 1)
		{
			logFile.ReadValue("SDO","Code","Admin",MemDuty,10);//����Ա
		}
		
		operSDO.GetLogText(strLog, "SDO_ModiMemDuty", "", "", operName, ServerName, FamilyName, NickName, MemDuty);
		operSDO.WriteText("SDO",strLog);
		
		if(type == 255)//���Ҫ�޸����Ϊ�峤
		{
			int ShaikhNum = 0;//��ʼ�������峤������Ϣ
			if(!operSDO.getRemoteSql("SDO","SDO_FamilyShaikh_Query",operRemoteSql,1))
			{
				return DBTFLV;//�޷��õ���ѯ�����峤��Ϣ��sql���
			}
			operSDO.QuerySingleValue(&ShaikhNum, "SDO", ServerIP, 0, operRemoteSql, SDO_DBName, FamilyID);//��ȡ�����峤�ĸ���
			if(ShaikhNum != 0)//����Ѿ����ڼ����峤
			{
				DBTFLV = operSDO.ReturnOpMsg(strLog, "SDO_ShaiExist");
				return DBTFLV;
			}
		}
		
		if(!operSDO.getRemoteSql("SDO","SDO_Update_MemDuty",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ��޸����ְ����Ϣ��sql���
		}
		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,SDO_DBName,type,MemDuty,FamilyID,UserID);	
		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_ModiMemDuty", "", "Fail", operName, ServerName, FamilyName, NickName, MemDuty);
			operSDO.WriteDBLog(userByID,"SDO","SDO_ModiMemDuty",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog, "Update_Member_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_ModiMemDuty", "", "Success", operName, ServerName, FamilyName, NickName, MemDuty);
			operSDO.WriteDBLog(userByID,"SDO","SDO_ModiMemDuty",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog, "Update_Member_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog, "Error");
	}

	return DBTFLV;
}

//��ɢ����
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_DeleteFamily(int userByID, char * ServerName, char * ServerIP, int FamilyID, char *FamilyName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_FamilyDelete", "", "", operName, ServerName, FamilyName);
		operSDO.WriteText("SDO",strLog);
		
		char TableFlag[20];
		char TableName[40];
		ZeroMemory(TableFlag,20);//��ʼ����ı�ʶ��
		ZeroMemory(TableName,40);//��ʼ��������
		logFile.ReadValue("SDO","TABLE","TabelName",TableName,40);//��ȡ������µı�����
		
		if(!operSDO.getRemoteSql("SDO","SDO_FamSelDBInfo",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ���ѯ����Ļ��������Ϣ��sql���
		}
		sprintf(operszSql,operRemoteSql,SDO_DBName,SDO_DBName,FamilyID);//����������ѯ��sql���
		
		if(!operSDO.BackupDataBase("",localIP,0,TableName,"SDO",ServerIP,0,operszSql))//����ѯ�õ������ݲ��뵽T_sdo_personal_emblem������
		{
			return DBTFLV;
		}
		
		if(!operSDO.getRemoteSql("SDO","SDO_FamilyDBInfo",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ���ѯ�����������ݿ���Ϣ��sql���
		}
		char TableNum[4];
		ZeroMemory(TableNum,4);
		logFile.ReadValue("SDO","TABLE","TabelNum", TableNum, 4);//��ȡҪ���ݵı������
		int intTableNum = StrToInt(TableNum);
		int i=1;
		for(i=1; i<=intTableNum; i++)
		{
			sprintf(TableFlag,"%s%i","TabelName",i);
			logFile.ReadValue("SDO","TABLE",TableFlag,TableName,40);//��ȡ��Ӧ�ļ��������

			sprintf(operszSql,operRemoteSql,SDO_DBName,TableName,FamilyID);//����������ѯ��sql���
			if(!operSDO.BackupDataBase("",localIP,0,TableName,"SDO",ServerIP,0,operszSql))//����ѯ�õ������ݲ��뵽���ݼ��������
			{
				return DBTFLV;
			}
		}
		
		if(!operSDO.getRemoteSql("SDO","SDO_DelFamEmblem",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ�ɾ������Ļ��±���Ϣ��sql���
		}

		int tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,SDO_DBName,SDO_DBName,FamilyID);

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_FamilyDelete", "", "Fail", operName, ServerName, FamilyName);
			operSDO.WriteDBLog(userByID,"SDO","SDO_FamilyDelete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog, "Delete_Family_Fail");
			return DBTFLV;
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_DelFamTable",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ�ɾ���������Ӧ����Ϣ��sql���
		}
		for(i=1; i<=intTableNum; i++)
		{
			sprintf(TableFlag,"%s%i","TabelName",i);
			logFile.ReadValue("SDO","TABLE",TableFlag,TableName,40);//��ȡ��Ӧ�ļ��������

			tmpLength = 0;
			tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,SDO_DBName,TableName,FamilyID);
			if(tmpLength == 0)
			{
				operSDO.GetLogText(strLog, "SDO_FamilyDelete", "", "Fail", operName, ServerName, FamilyName);
				operSDO.WriteDBLog(userByID,"SDO","SDO_FamilyDelete",ServerIP,strLog);
				
				DBTFLV = operSDO.ReturnOpMsg(strLog, "Delete_Family_Fail");
				return DBTFLV;
			}
		}
		operSDO.GetLogText(strLog, "SDO_FamilyDelete", "", "Success", operName, ServerName, FamilyName);
		operSDO.WriteDBLog(userByID,"SDO","SDO_FamilyDelete",ServerIP,strLog);
		
		DBTFLV = operSDO.ReturnOpMsg(strLog, "Delete_Family_Success");
		
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog, "Error");
	}

	return DBTFLV;
}

//��Ӽ������
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_Insert_FamilyBadge(int userByID, char * ServerName, char * ServerIP, char *NickName, int UserID, int FamilyID, char *FamilyName, int EmblemType, int EmblemNum)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_InsertEmblem", "", "", operName, ServerName, FamilyName, NickName, EmblemType, EmblemNum);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_Emblem_Insert",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ���Ӽ�����µ�sql���
		}

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,SDO_DBName,UserID,EmblemType,EmblemNum); 	

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_InsertEmblem", "", "Fail",operName, ServerName, FamilyName, NickName, EmblemType, EmblemNum);
			operSDO.WriteDBLog(userByID,"SDO","SDO_InsertEmblem",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog, "Insert_Emblem_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_InsertEmblem", "", "Success",operName, ServerName, FamilyName, NickName, EmblemType, EmblemNum);
			operSDO.WriteDBLog(userByID,"SDO","SDO_InsertEmblem",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog, "Insert_Emblem_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog, "Error");
	}

	return DBTFLV;
}

//ɾ���������
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_Delete_FamilyBadge(int userByID, char * ServerName, char * ServerIP, char *NickName, int UserID, char *FamilyName, int EmblemType)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_DeleteEmblem", "", "", operName, ServerName, FamilyName, NickName, EmblemType);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO","SDO_Emblem_Delete",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ�ɾ��������µ�sql���
		}
		sprintf(operszSql,operRemoteSql,SDO_DBName,UserID,EmblemType);//����������ѯ��sql���

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,SDO_DBName,UserID,EmblemType);
				
		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_DeleteEmblem", "", "Fail",operName, ServerName, FamilyName, NickName, EmblemType);
			operSDO.WriteDBLog(userByID,"SDO","SDO_DeleteEmblem",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog, "Delete_Emblem_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_DeleteEmblem", "", "Success",operName, ServerName, FamilyName, NickName, EmblemType);
			operSDO.WriteDBLog(userByID,"SDO","SDO_DeleteEmblem",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog, "Delete_Emblem_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog, "Error");
	}

	return DBTFLV;
}

//�鿴���G��
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_UserGCash_Query(int userByID, char * ServerName, char * ServerIP, char *Account)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_UserGCash", "", operName,ServerName,Account);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO","SDO_UserGCash_Query",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ��鿴���G����Ϣ��sql���
		}
//		printf(operRemoteSql,SDO_DBName,Account);
		DBTFLV = GameDBQuery("SDO", ServerIP, 0, 0, 0, operRemoteSql,SDO_DBName,Account);
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//�������G��
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_UserGCash_Update(int userByID, char * ServerName, char * ServerIP, char *Account, int GCash)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_UpdateGCash", "", "", operName, ServerName, Account, GCash);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_AddGCash_Query",operRemoteSql, 1))
		{
			return DBTFLV;//�޷��õ���ѯ�����û�����G����Ϣ��sql���
		}
//		printf(operRemoteSql,SDO_DBName,Account);
		DBTFLV = GameDBQuery("SDO", ServerIP, 0, 0, 0, operRemoteSql,SDO_DBName,Account);

		int tmpLength = 0;
		if(DBTFLV.empty())
		{
			if(!operSDO.getRemoteSql("SDO", "SDO_AddGCash_Insert",operRemoteSql,1))
			{
				return DBTFLV;//�޷��õ���������G�ҵ�sql���
			}
//			printf(operRemoteSql,SDO_DBName,Account,GCash);
			tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,SDO_DBName,Account,GCash);
			}
		else
		{
			if(!operSDO.getRemoteSql("SDO", "SDO_AddGCash_Update",operRemoteSql, 1))
			{
				return DBTFLV;//�޷��õ�����Ҹ���G�ҵ�sql���
			}
//			printf(operRemoteSql,SDO_DBName,GCash,Account);
			tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,SDO_DBName,GCash,Account);
		}

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_UpdateGCash", "", "Fail", operName, ServerName, Account, GCash);
			operSDO.WriteDBLog(userByID,"SDO","SDO_UpdateGCash",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Update_GCash_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_UpdateGCash", "", "Success", operName, ServerName, Account, GCash);
			operSDO.WriteDBLog(userByID,"SDO","SDO_UpdateGCash",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Update_GCash_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//������ս��Ϣ��ѯ
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_CHALLENGE_QUERY(int userByID, char * ServerName, char * ServerIP, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "BaseMessage1", "SDO_ChallengeInfo", "", operName,ServerName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))
		{
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_Challenge_Query",operRemoteSql, 1))
		{
			return DBTFLV;//�޷��õ��鿴������ս��Ϣ��sql���
		}
		DBTFLV = GameDBQuery("SDO", ServerIP, 0, iIndex, iPageSize, operRemoteSql,Item_DBName);
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//���ô�����ս��Ϣ
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_CHALLENGE_INSERT(int userByID, char * ServerName, char * ServerIP, int weekDay, int matPtMin, int stPtMin, int GCash, int scence, int isBattle)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		char readTag[20];//��ȡ�����ļ��ı�ǩ��Ϣ		
		char strWeekDay[10];//���������ܼ�������
		char SceneName[64];//����������
		char BattleMode[20];//����ģʽ������
		ZeroMemory(readTag,20);//��ʼ����ȡ�����ļ��ı�ǩ��Ϣ
		ZeroMemory(strWeekDay,10);//��ʼ�����������ܼ�������
		ZeroMemory(SceneName,64);//��ʼ������������
		ZeroMemory(BattleMode,20);//��ʼ������ģʽ������
		
		sprintf(readTag, "%s%i", "WeekDay", weekDay);
		logFile.ReadValue("SDO","Code",readTag,strWeekDay,10);//��ȡ��ǰ�����ڼ�
		
		operSDO.getRemoteSql("SDO", "SDO_SceneName_Query",operRemoteSql,1);//��ȡ��ѯ�������Ƶ�sql���
		operSDO.QuerySingleValue(SceneName, "", localIP, 0, operRemoteSql, scence);//��ȡ��ǰ��������
		
		ZeroMemory(readTag,20);//��ʼ����ȡ�����ļ��ı�ǩ��Ϣ
		if (isBattle == 10)
		{
			sprintf(readTag, "%s", "BattleType1");
			logFile.ReadValue("SDO","Code",readTag,BattleMode,20);//��ȡ��ǰ�ı���ģʽ
		}
		else if (isBattle == 13)
		{
			sprintf(readTag, "%s", "BattleType2");
			logFile.ReadValue("SDO","Code",readTag,BattleMode,20);//��ȡ��ǰ�ı���ģʽ
		}
		else if (isBattle == 12)
		{
			sprintf(readTag, "%s", "BattleType3");
			logFile.ReadValue("SDO","Code",readTag,BattleMode,20);//��ȡ��ǰ�ı���ģʽ
		}
		else if (isBattle == 11)
		{
			sprintf(readTag, "%s", "BattleType4");
			logFile.ReadValue("SDO","Code",readTag,BattleMode,20);//��ȡ��ǰ�ı���ģʽ
        }
		
		operSDO.GetLogText(strLog, "SDO_Challenge_Insert", "", "", operName, ServerName, strWeekDay,stPtMin,matPtMin,GCash,SceneName,BattleMode);//����ִ�в�������־
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))
		{
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_Challenge_Insert",operRemoteSql, 1))
		{
			return DBTFLV;//�޷��õ���Ӵ�����ս��Ϣ��sql���
		}

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,Item_DBName,weekDay,matPtMin,stPtMin,GCash,scence,isBattle);
		
		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_Challenge_Insert", "", "Fail", operName, ServerName, strWeekDay,stPtMin,matPtMin,GCash,SceneName,BattleMode);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_Challenge_Insert",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Insert_Challenge_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_Challenge_Insert", "", "Success", operName, ServerName, strWeekDay,stPtMin,matPtMin,GCash,SceneName,BattleMode);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_Challenge_Insert",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Insert_Challenge_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//�޸Ĵ�����ս��Ϣ
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_CHALLENGE_UPDATE(int userByID, char * ServerName, char * ServerIP, int ChallengeID, int weekDay, int matPtMin, int stPtMin, int GCash, int scence, int isBattle)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		char readTag[20];//��ȡ�����ļ��ı�ǩ��Ϣ		
		char strWeekDay[10];//���������ܼ�������
		char SceneName[64];//����������
		char BattleMode[20];//����ģʽ������
		ZeroMemory(readTag,20);//��ʼ����ȡ�����ļ��ı�ǩ��Ϣ
		ZeroMemory(strWeekDay,10);//��ʼ�����������ܼ�������
		ZeroMemory(SceneName,64);//��ʼ������������
		ZeroMemory(BattleMode,20);//��ʼ������ģʽ������
		
		sprintf(readTag, "%s%i", "WeekDay", weekDay);
		logFile.ReadValue("SDO","Code",readTag,strWeekDay,10);//��ȡ��ǰ�����ڼ�
		
		operSDO.getRemoteSql("SDO", "SDO_SceneName_Query",operRemoteSql,1);//��ȡ��ѯ�������Ƶ�sql���
		operSDO.QuerySingleValue(SceneName,"",localIP,0,operRemoteSql,scence);//��ȡ��ǰ��������
		
		ZeroMemory(readTag,20);//��ʼ����ȡ�����ļ��ı�ǩ��Ϣ
		if (isBattle == 10)
		{
			sprintf(readTag, "%s", "BattleType1");
			logFile.ReadValue("SDO","Code",readTag,BattleMode,20);//��ȡ��ǰ�ı���ģʽ
		}
		else if (isBattle == 13)
		{
			sprintf(readTag, "%s", "BattleType2");
			logFile.ReadValue("SDO","Code",readTag,BattleMode,20);//��ȡ��ǰ�ı���ģʽ
		}
		else if (isBattle == 12)
		{
			sprintf(readTag, "%s", "BattleType3");
			logFile.ReadValue("SDO","Code",readTag,BattleMode,20);//��ȡ��ǰ�ı���ģʽ
		}
		else if (isBattle == 11)
		{
			sprintf(readTag, "%s", "BattleType4");
			logFile.ReadValue("SDO","Code",readTag,BattleMode,20);//��ȡ��ǰ�ı���ģʽ
        }
		
		operSDO.GetLogText(strLog,"SDO_Challenge_Update","","",operName,ServerName,ChallengeID,strWeekDay,stPtMin,matPtMin,GCash,SceneName,BattleMode);//����ִ�в�������־
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))
		{
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		//���汸����Ϣ
		operSDO.BakUpdateInfo(userByID,"SDO_Challenge_Update","SDO",ServerIP,0,Item_DBName, "SDO_ChallengeBak_Get", "SDO_CHALLENGE_UPDATE", ChallengeID, -1);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_Challenge_Update",operRemoteSql, 1))
		{
			return DBTFLV;//�޷��õ��޸Ĵ�����ս��Ϣ��sql���
		}

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,Item_DBName,weekDay,matPtMin,stPtMin,GCash,scence,isBattle,ChallengeID);

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog,"SDO_Challenge_Update","","Fail",operName,ServerName,ChallengeID,strWeekDay,stPtMin,matPtMin,GCash,SceneName,BattleMode);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_Challenge_Update",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Update_Challenge_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog,"SDO_Challenge_Update","","Success",operName,ServerName,ChallengeID,strWeekDay,stPtMin,matPtMin,GCash,SceneName,BattleMode);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_Challenge_Update",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Update_Challenge_Success");
		}
	}
	catch(...)
	{ 
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//ɾ��������ս��Ϣ
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_CHALLENGE_DELETE(int userByID, char * ServerName, char * ServerIP, int ChallengeID)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog,"SDO_Challenge_Delete","","",operName,ServerName,ChallengeID);//����ִ�в�������־
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP, ServerIP,2))
		{
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_Challenge_Delete",operRemoteSql, 1))
		{
			return DBTFLV;//�޷��õ�ɾ��������ս��Ϣ��sql���
		}

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,Item_DBName,ChallengeID);
		
		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog,"SDO_Challenge_Delete","","Fail",operName,ServerName,ChallengeID);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_Challenge_Delete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Delete_Challenge_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog,"SDO_Challenge_Delete","","Success",operName,ServerName,ChallengeID);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_Challenge_Delete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Delete_Challenge_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//�鿴���������б�
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_SCENE_QUERY(int userByID, char * ServerName, char * ServerIP)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage2", "SDO_Scene_Query", "", operName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_Scene_Query",operRemoteSql, 1))
		{
			return DBTFLV;//�޷��õ��鿴���������б��sql���
		}

		DBTFLV = GameDBQuery("SDO", localIP, 4, 0, 0, operRemoteSql);		
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//��Ӵ�����ս�ĳ���
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_SCENE_CREATE(int userByID, char * ServerName, char * ServerIP, char *SceneName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	
	try
	{
		operSDO.GetLogText(strLog, "SDO_Scene_Insert", "", "", operName,SceneName);//������־���
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_Scene_Insert",operRemoteSql, 1))
		{
			return DBTFLV;//�޷��õ���Ӵ�����ս������sql���
		}

		int tmpLength = 0;
//		printf(operRemoteSql,SceneName);
		tmpLength = GameDBOper("SDO", localIP, 4, operRemoteSql,SceneName);

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_Scene_Insert", "", "Fail", operName,SceneName);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_Scene_Insert",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Insert_Scene_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_Scene_Insert", "", "Success", operName,SceneName);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_Scene_Insert",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Insert_Scene_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//�޸Ĵ�����ս�ĳ���
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_SCENE_UPDATE(int userByID, char * ServerName, char * ServerIP, int SceneID, char *SceneName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		int SceneID =StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::SDO_SenceID,0)).lpdata);//����ID
		char SceneName[64];
		ZeroMemory(SceneName,64);//��ʼ����������
		sprintf(SceneName,"%s",(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::SDO_Sence,0)).lpdata);//��ȡ��������
		
		operSDO.GetLogText(strLog, "SDO_Scene_Update", "", "", operName,SceneID,SceneName);//������־���
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_Scene_Update",operRemoteSql, 1))
		{
			return DBTFLV;//�޷��õ��޸Ĵ�����ս������sql���
		}

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", localIP, 4, operRemoteSql,SceneName,SceneID);

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_Scene_Update", "", "Fail", operName,SceneID,SceneName);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_Scene_Update",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Update_Scene_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_Scene_Update", "", "Success", operName,SceneID,SceneName);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_Scene_Update",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Update_Scene_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//ɾ��������ս�ĳ���
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_SCENE_DELETE(int userByID, char * ServerName, char * ServerIP, int SceneID)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_Scene_Delete", "", "", operName,SceneID);//������־���
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_Scene_Delete",operRemoteSql, 1))
		{
			return DBTFLV;//�޷��õ�ɾ��������ս�ĳ�����sql���
		}

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", localIP, 4, operRemoteSql,SceneID);

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_Scene_Delete", "", "Fail", operName,SceneID);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_Scene_Delete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Delete_Scene_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_Scene_Delete", "", "Success", operName,SceneID);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_Scene_Delete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Delete_Scene_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//�鿴����Ϸ��ҡҡ�ֻ�ø���
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_YYHappyItem_Query(int userByID, char * ServerName, char * ServerIP, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage1", "SDO_YYHappyItem_Query", "", operName,ServerName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))//��ȡ�����ķ�����IP
		{
			return DBTFLV;
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_YYHappyItem_Query",operRemoteSql, 1))
		{
			return DBTFLV;//�޷��õ���ѯ��Ϸ��ҡҡ�ֻ�ø��ʵ�sql���
		}

		DBTFLV = GameDBQuery("SDO", ServerIP, 0, iIndex, iPageSize, operRemoteSql,Item_DBName);

	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//���ҡҡ�ֻ�ø���
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_YYHappyItem_Insert(int userByID, char * ServerName, char * ServerIP, int itemCode1, int itemCode2, int level, int levPercent, int precent, int timeslimit, int dayslimit, char *itemName1, char *itemName2)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_YYHappyItem_Insert", "", "", operName, ServerName, 
			itemCode1,itemName1,itemCode2,itemName2,level,levPercent,precent,timeslimit,dayslimit);//����ִ�в�������־
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))
		{
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		int itemCount = 0;
		if(!operSDO.getRemoteSql("SDO", "SDO_YYHappyItem_Count",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ����ҡҡ�ֻ�ø��ʵ�sql���
		}

		operSDO.QuerySingleValue(&itemCount,"SDO",ServerIP,0,operRemoteSql,Item_DBName,itemCode1);

		int tmpLength = 0;

		if(itemCount > 0)
		{
			if(!operSDO.getRemoteSql("SDO", "SDO_YYHappyItem_Create",operRemoteSql, 1))
			{
				return DBTFLV;//�޷��õ����ҡҡ�ֻ�ø��ʵ�sql���
			}
			tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,Item_DBName,level,levPercent,precent,timeslimit,dayslimit,itemCode1);
		}
		else
		{
			if(!operSDO.getRemoteSql("SDO", "SDO_YYHappyItem_Insert",operRemoteSql,1))
			{
				return DBTFLV;//�޷��õ����ҡҡ�ֻ�ø��ʵ�sql���
			}

			tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,Item_DBName,itemCode1,itemCode2,level,levPercent,precent,timeslimit,dayslimit);
		}		
		
		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_YYHappyItem_Insert", "", "Fail", operName, ServerName, 
				itemCode1,itemName1,itemCode2,itemName2,level,levPercent,precent,timeslimit,dayslimit);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_YYHappyItem_Insert",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Insert_YYHappyItem_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_YYHappyItem_Insert", "", "Success", operName, ServerName, 
				itemCode1,itemName1,itemCode2,itemName2,level,levPercent,precent,timeslimit,dayslimit);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_YYHappyItem_Insert",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Insert_YYHappyItem_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//����ҡҡ�ֻ�ø���
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_YYHappyItem_Update(int userByID, char * ServerName, char * ServerIP, int itemCode, int itemCode1, int itemCode2, int level, int levPercent, int precent, int timeslimit, int dayslimit, char *itemName1, char *itemName2)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_YYHappyItem_Update", "", "", operName, ServerName, itemCode,
			itemCode1,itemName1,itemCode2,itemName2,level,levPercent,precent,timeslimit,dayslimit);//����ִ�в�������־
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))
		{
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_YYHappyItem_Update",operRemoteSql, 1))
		{
			return DBTFLV;//�޷��õ�����ҡҡ�ֻ�ø��ʵ�sql���
		}

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,Item_DBName,itemCode1,itemCode2,level,levPercent,precent,timeslimit,dayslimit,itemCode);
		
		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_YYHappyItem_Update", "", "Fail", operName, ServerName, itemCode,
				itemCode1,itemName1,itemCode2,itemName2,level,levPercent,precent,timeslimit,dayslimit);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_YYHappyItem_Update",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Update_YYHappyItem_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_YYHappyItem_Update", "", "Success", operName, ServerName, itemCode,
				itemCode1,itemName1,itemCode2,itemName2,level,levPercent,precent,timeslimit,dayslimit);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_YYHappyItem_Update",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Update_YYHappyItem_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//ɾ��ҡҡ�ֵ��߻�ø���
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_YYHappyItem_Delete(int userByID, char * ServerName, char * ServerIP, int itemCode)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_YYHappyItem_Delete", "", "", operName,ServerName, itemCode);//������־���
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))
		{
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_YYHappyItem_Delete",operRemoteSql, 1))
		{
			return DBTFLV;//�޷��õ�ɾ��ҡҡ�ֵ��߻�ø��ʵ�sql���
		}

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,Item_DBName,itemCode);

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_YYHappyItem_Delete", "", "Fail", operName,ServerName, itemCode);//������־���
			operSDO.WriteDBLog(userByID,"SDO","SDO_Scene_Delete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Delete_YYHappyItem_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_YYHappyItem_Delete", "", "Success", operName,ServerName, itemCode);//������־���
			operSDO.WriteDBLog(userByID,"SDO","SDO_Scene_Delete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Delete_YYHappyItem_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//�鿴��Ϸ����ĵ�����Ϣ
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_Item_QueryAll(int userByID, char * ServerName, char * ServerIP, int BigType, int SmallType, int Sex, char * itemName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_ItemInfo_All", "", "", operName,ServerName,BigType,SmallType,Sex,itemName);
		operSDO.WriteText("SDO",strLog);
		
//		printf("exec SDO_ItemShop_Query_ALL '%s','%i','%i',%i,'%s'",ServerIP, BigType, SmallType, Sex, itemName);
		DBTFLV = GameDBQuery("SDO", localIP, 4, 0, 0, "exec SDO_ItemShop_Query_ALL '%s','%i','%i',%i,'%s'", 
			ServerIP, BigType, SmallType, Sex, itemName);
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//���߻�ȡ���ʲ�ѯ
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_MedalItem_Query(int userByID, char * ServerName, char * ServerIP, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage1", "SDO_MedalItem_Query", "", operName,ServerName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))//��ȡ�����ķ�����IP
		{
			return DBTFLV;
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_MedalItem_Query",operRemoteSql, 1))
		{
			return DBTFLV;//�޷��õ���ѯ���߻�ȡ���ʵ�sql���
		}

		DBTFLV = GameDBQuery("SDO", ServerIP, 0, iIndex, iPageSize, operRemoteSql,Item_DBName);

	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//�������߻�ȡ���ʲ�ѯ
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_MedalItem_Owner_Query(int userByID, char * ServerName, char * ServerIP, char *itemName, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage1", "SDO_MedalItem_Owner_Query", "", operName,ServerName);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))//��ȡ�����ķ�����IP
		{
			return DBTFLV;
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_MedalItem_Owner_Query",operRemoteSql, 1))
		{
			return DBTFLV;//�޷��õ���ѯ�������߻�ȡ���ʵ�sql���
		}
		printf(operRemoteSql, "\%", itemName, "\%");
		DBTFLV = GameDBQuery("SDO", localIP, 4, iIndex, iPageSize, operRemoteSql, "\%", itemName, "\%");
	
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//��ӵ��߻�ȡ����
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_MedalItem_Insert(int userByID, char * ServerName, char * ServerIP, int itemCode, int precent, char *itemName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog,"SDO_MedalItem_Insert","","",operName,ServerName,itemCode,itemName,precent);//����ִ�в�������־
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))
		{
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		int itemCount = 0;
		if(!operSDO.getRemoteSql("SDO", "SDO_MedalItem_Count",operRemoteSql, 1))
		{
			return DBTFLV;//�޷��õ��鿴������Ӧ���ʴ治���ڵ�sql���
		}
		operSDO.QuerySingleValue(&itemCount,"SDO",ServerIP,0,operRemoteSql,Item_DBName,itemCode);

		int tmpLength = 0;

		if(itemCount > 0)
		{
			if(!operSDO.getRemoteSql("SDO", "SDO_MedalItem_Update",operRemoteSql, 1))
			{
				return DBTFLV;//�޷��õ����µ��߻�ȡ���ʵ�sql���
			}

			tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,Item_DBName,precent,itemCode);
		}
		else
		{
			if(!operSDO.getRemoteSql("SDO", "SDO_MedalItem_Insert",operRemoteSql, 1))
			{
				return DBTFLV;//�޷��õ���ӵ��߻�ȡ���ʵ�sql��� 
			}
			tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,Item_DBName,itemCode,precent);
		}		
		
		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog,"SDO_MedalItem_Insert","","Fail",operName,ServerName,itemCode,itemName,precent);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_MedalItem_Insert",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Insert_MedalItem_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog,"SDO_MedalItem_Insert","","Success",operName,ServerName,itemCode,itemName,precent);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_MedalItem_Insert",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Insert_MedalItem_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//���µ��߻�ȡ����
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_MedalItem_Update(int userByID, char * ServerName, char * ServerIP, int itemCode, int precent, char *itemName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog,"SDO_MedalItem_Update","","",operName,ServerName,itemCode,itemName,precent);//����ִ�в�������־
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))
		{
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_MedalItem_Update",operRemoteSql, 1))
		{
			return DBTFLV;//�޷��õ����µ��߻�ȡ���ʵ�sql���
		}

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,Item_DBName,precent,itemCode);

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog,"SDO_MedalItem_Update","","Fail",operName,ServerName,itemCode,itemName,precent);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_MedalItem_Update",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Update_MedalItem_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog,"SDO_MedalItem_Update","","Success",operName,ServerName,itemCode,itemName,precent);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_MedalItem_Update",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Update_MedalItem_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//ɾ�����߻�ȡ����
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_MedalItem_Delete(int userByID, char * ServerName, char * ServerIP, int itemCode, char *itemName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		int itemCode = StrToInt((char *)&(recvVect.getTLVByTag(CEnumCore::TagName::SDO_ItemCode,0)).lpdata);//����code
		char itemName[128];
		ZeroMemory(itemName,128);
		sprintf(itemName,"%s",(char *)&(recvVect.getTLVByTag(CEnumCore::TagName::SDO_ItemName,0)).lpdata);//��ȡ��������
		
		operSDO.GetLogText(strLog,"SDO_MedalItem_Delete","","",operName,ServerName,itemCode,itemName);//����ִ�в�������־
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))
		{
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		if(!operSDO.getRemoteSql("SDO", "SDO_MedalItem_Delete",operRemoteSql, 1))
		{
			return DBTFLV;//�޷��õ�ɾ�����߻�ȡ���ʵ�sql���
		}

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,Item_DBName,itemCode);

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog,"SDO_MedalItem_Delete","","Fail",operName,ServerName,itemCode,itemName);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_MedalItem_Delete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Delete_MedalItem_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog,"SDO_MedalItem_Delete","","Success",operName,ServerName,itemCode,itemName);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_MedalItem_Delete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Delete_MedalItem_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//������ҽ�ɫ��Ϣ
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_CharacterInfo_Update(int userByID, char * ServerName, char * ServerIP, int UserID, char *Account,int level, int experience, int battle, int win, int draw, int lose, int MCash, int GCash)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_CharacterInfo_Update", "", "", operName, ServerName,Account, 
			level,experience,battle,win,draw,lose,MCash,GCash);//����ִ�в�������־
		operSDO.WriteText("SDO",strLog);
		
		//���ݸ�����Ϣ
		operSDO.BakUpdateInfo(userByID,"SDO_CharacterInfo_Update","SDO",ServerIP,0,SDO_DBName,"SDO_CharInfo_Bak","SDO_CharInfo_Update",UserID,-1);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_CharInfo_Update",operRemoteSql, 1))
		{
			return DBTFLV;//�޷��õ�������ҽ�ɫ��Ϣ��sql���
		}
	
		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,SDO_DBName,experience,win,lose,MCash,GCash,UserID);

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_CharacterInfo_Update", "", "Fail", operName, ServerName,Account, 
				level,experience,battle,win,draw,lose,MCash,GCash);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_CharacterInfo_Update",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Update_CharacterInfo_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_CharacterInfo_Update", "", "Success", operName, ServerName,Account, 
				level,experience,battle,win,draw,lose,MCash,GCash);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_CharacterInfo_Update",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Update_CharacterInfo_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//ɾ��������ϵ���
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_ItemShop_Delete(int userByID, char * ServerName, char * ServerIP, int UserID, char *Account, int itemCode, char *itemName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog,"SDO_UserEquip_Delete","","",operName,Account,itemCode,itemName);//����ִ�в�������־
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.GetServerIP(ServerIP,ServerIP,2))
		{
			return DBTFLV;//�޷���ȡ�����ķ�����IP
		}
		
		//����ɾ����Ϣ
		operSDO.BakDeleteInfo(userByID,"SDO_UserEquip_Delete","SDO",ServerIP,0,Item_DBName,
			"T_sdo_item","SDO_Remote_GetEquipInfo","SDO_Remote_SaveInfo",itemCode, UserID);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_UserEquip_Delete",operRemoteSql,1))
		{
			return DBTFLV;//�޷��õ�ɾ��������ϵ��ߵ�sql���
		}

		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,Item_DBName,itemCode,UserID);

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog,"SDO_UserEquip_Delete","","Fail",operName,ServerName,Account,itemCode,itemName);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_UserEquip_Delete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Delete_UserEquip_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog,"SDO_UserEquip_Delete","","Success",operName,ServerName,Account,itemCode,itemName);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_UserEquip_Delete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Delete_UserEquip_Success");
		}
	}
	catch (...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//�鿴�������е���
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_GiftBox_Query(int userByID, char * ServerName, char * ServerIP, int UserID, char *Account, int iIndex, int iPageSize)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;
	
	try
	{
		operSDO.GetLogText(strLog, "BaseMessage", "SDO_GiftBox", "", operName,ServerName,Account);
		operSDO.WriteText("SDO",strLog);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_GiftBox_Query",operRemoteSql, 1))
		{
			return DBTFLV;//�޷��õ��鿴�������е��ߵ�sql���
		}

		DBTFLV = GameDBQuery("SDO", ServerIP, 0, iIndex, iPageSize, operRemoteSql,SDO_DBName,UserID);		
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
	}
	return DBTFLV;
}

//ɾ���������е���
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_GiftBox_Delete(int userByID, char * ServerName, char * ServerIP, int UserID, char *Account, int itemCode, char *itemName)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog,"SDO_GiftBox_Delete","","",operName,ServerName,Account,itemCode,itemName);//����ִ�в�������־
		operSDO.WriteText("SDO",strLog);
		
		//����ɾ����Ϣ
		operSDO.BakDeleteInfo(userByID,"SDO_GiftBox_Delete","SDO",ServerIP,0,SDO_DBName,
			"T_sdo_Message","SDO_BackUp_GiftBox","SDO_Remote_SaveInfo",itemCode, UserID);
		
		if(!operSDO.getRemoteSql("SDO", "SDO_GiftBox_Delete",operRemoteSql, 1))
		{
			return DBTFLV;//�޷��õ�ɾ���������е��ߵ�sql���
		}

		int tmpLength;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operRemoteSql,SDO_DBName,itemCode,UserID);

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog,"SDO_GiftBox_Delete","","Fail",operName,ServerName,Account,itemCode,itemName);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_GiftBox_Delete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Delete_GiftBox_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog,"SDO_GiftBox_Delete","","Success",operName,ServerName,Account,itemCode,itemName);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_GiftBox_Delete",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Delete_GiftBox_Success");
		}
	}
	catch (...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//����ҷ��͵���
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_GiftBox_Insert(int userByID, char * ServerName, char * ServerIP, int UserID, char *Account, int itemCode, int timeslimit, int DateLimit, char *itemName, char *title, char *content)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		operSDO.GetLogText(strLog, "SDO_GiftBox_Insert", "", "", operName, ServerName, Account,
			itemCode,itemName,timeslimit,DateLimit,title,content);//����ִ�в�������־
		operSDO.WriteText("SDO",strLog);
		
		int bigtype = 0;
		if(!operSDO.getRemoteSql("SDO", "SDO_ItemType_Query",operRemoteSql, 1))
		{
			return DBTFLV;//�޷��õ���ѯ�������������sql���
		}
		operSDO.QuerySingleValue(&bigtype,"",localIP,0,operRemoteSql,itemCode);

		if(bigtype == 3)
		{
			if(!operSDO.getRemoteSql("SDO", "SDO_GiftBox_Insert",operRemoteSql, 1))
			{
				return DBTFLV;//�޷��õ�����ҷ��͵��ߵ�sql���
			}
			sprintf(operszSql,operRemoteSql,SDO_DBName,UserID,title,content,itemCode,DateLimit,-1,0,3);//��������������sql���		
		}
		else
		{
			char StrInt[10];
			char tmpStr[10];
			ZeroMemory(StrInt,10);//��ʼ��
			ZeroMemory(tmpStr, 10);
			_itoa(itemCode, StrInt, 10);
			int i=0;//ѭ������
			for(i=1; i<=12; i++)
			{
				sprintf(tmpStr, "0%i", i);
				if(!strcmp(tmpStr, StrInt))
				{
					if(!operSDO.getRemoteSql("SDO", "SDO_GiftMedal_Insert",operRemoteSql, 1))
					{
						return DBTFLV;//�޷��õ�����ҷ���ѫ�µ��ߵ�sql���
					}
					sprintf(operszSql,operRemoteSql,SDO_DBName,2^(i-1),2^(i-1),UserID);//��������������sql���
					break;
				}
			}
			if(i<=12)
			{
			}
			else
			{
				if(!operSDO.getRemoteSql("SDO", "SDO_GiftBox_Insert",operRemoteSql, 1))
				{
					return DBTFLV;//�޷��õ�����ҷ��͵��ߵ�sql���
				}
			    if((itemCode==100003) || (itemCode==100004) || (itemCode==100005) || (itemCode==100006) || (itemCode==100007) ||
					(itemCode==100021) || (itemCode==100022) || (itemCode==100023) || (itemCode==100024) || (itemCode==100025) || 
					(itemCode==100061) || (itemCode==100063) || (itemCode==100611) || (itemCode==100612) || (itemCode==100868) || 
					(itemCode==100776) || (itemCode==100778) || (itemCode==100780) || (itemCode==100754) || (itemCode==100756) || 
					(itemCode==100758) || (itemCode==100069) || (itemCode==100065) || (itemCode==100067) || (itemCode==100420) || 
					(itemCode==100728) || (itemCode==100922) || (itemCode==100924) || (itemCode==100926) || (itemCode==100912) || 
					(itemCode==100913) || (itemCode==100914) || (itemCode==100915) || (itemCode==100898) || (itemCode==200000) || 
					(itemCode==202999))
				{					
					sprintf(operszSql,operRemoteSql,SDO_DBName,UserID,title,content,itemCode,DateLimit,-1,0,3);//��������������sql���	
				}
				else if((itemCode==100000) || (itemCode==100002) || (itemCode==100020) || (itemCode==100030) || (itemCode==100031) || 
					(itemCode==100032) || (itemCode==100033) || (itemCode==100034) || (itemCode==100035) || (itemCode==100036) || 
					(itemCode==100037) || (itemCode==100038) || (itemCode==100039) || (itemCode==100040) || (itemCode==100041) || 
					(itemCode==100050) || (itemCode==100051) || (itemCode==100052) || (itemCode==100060) || (itemCode==100062) || 
					(itemCode==100064) || (itemCode==100066) || (itemCode==100068) || (itemCode==100070) || (itemCode==100071) || 
					(itemCode==100072) || (itemCode==100080) || (itemCode==100081) || (itemCode==100082) || (itemCode==100083) || 
					(itemCode==100084) || (itemCode==100085) || (itemCode==100086) || (itemCode==100087) || (itemCode==100088) || 
					(itemCode==100089) || (itemCode==100140) || (itemCode==100141) || (itemCode==100142) || (itemCode==100410) || 
					(itemCode==100500) || (itemCode==100501) || (itemCode==100502) || (itemCode==100601) || (itemCode==100602) || 
					(itemCode==100713) || (itemCode==100714) || (itemCode==100715) || (itemCode==100716) || (itemCode==100717) || 
					(itemCode==100718) || (itemCode==100719) || (itemCode==100720) || (itemCode==100721) || (itemCode==100722) || 
					(itemCode==100723) || (itemCode==100724) || (itemCode==100725) || (itemCode==100726) || (itemCode==100727) || 
					(itemCode==100729) || (itemCode==100730) || (itemCode==100731) || (itemCode==100732) || (itemCode==100733) || 
					(itemCode==100739) || (itemCode==100741) || (itemCode==100743) || (itemCode==100745) || (itemCode==100747) || 
					(itemCode==100749) || (itemCode==100751) || (itemCode==100753) || (itemCode==100755) || (itemCode==100757) || 
					(itemCode==100759) || (itemCode==100761) || (itemCode==100763) || (itemCode==100765) || (itemCode==100767) || 
					(itemCode==100769) || (itemCode==100771) || (itemCode==100773) || (itemCode==100775) || (itemCode==100777) || 
					(itemCode==100779) || (itemCode==100781) || (itemCode==100783) || (itemCode==100785) || (itemCode==100787) || 
					(itemCode==100789) || (itemCode==100790) || (itemCode==100792) || (itemCode==100794) || (itemCode==100796) || 
					(itemCode==100797) || (itemCode==100798) || (itemCode==100807) || (itemCode==100815) || (itemCode==100823) || 
					(itemCode==100831) || (itemCode==100839) || (itemCode==100847) || (itemCode==100855) || (itemCode==100866) || 
					(itemCode==100867) || (itemCode==100869) || (itemCode==100876) || (itemCode==100882) || (itemCode==100883) || 
					(itemCode==100884) || (itemCode==100886) || (itemCode==100887) || (itemCode==100888) || (itemCode==100889) || 
					(itemCode==100890) || (itemCode==100891) || (itemCode==100892) || (itemCode==100893) || (itemCode==100894) || 
					(itemCode==100895) || (itemCode==100896) || (itemCode==100897) || (itemCode==100903) || (itemCode==100904) || 
					(itemCode==100905) || (itemCode==100906) || (itemCode==100907) || (itemCode==100908) || (itemCode==100909) || 
					(itemCode==100910) || (itemCode==100911) || (itemCode==100916) || (itemCode==100917) || (itemCode==100918) || 
					(itemCode==100927))
				{
				    sprintf(operszSql,operRemoteSql,SDO_DBName,UserID,title,content,itemCode,-1,timeslimit,0,3);//��������������sql���
				}
				else
				{
					sprintf(operszSql,operRemoteSql,SDO_DBName,UserID,title,content,itemCode,DateLimit,timeslimit,0,3);//��������������sql���
				}
			}
		}
		int tmpLength = 0;
		tmpLength = GameDBOper("SDO", ServerIP, 0, operszSql);

		if(tmpLength == 0)
		{
			operSDO.GetLogText(strLog, "SDO_GiftBox_Insert", "", "Fail", operName, ServerName, Account,
			    itemCode,itemName,timeslimit,DateLimit,title,content);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_GiftBox_Insert",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Insert_GiftBox_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_GiftBox_Insert", "", "Success", operName, ServerName, Account,
			    itemCode,itemName,timeslimit,DateLimit,title,content);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_GiftBox_Insert",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"Insert_GiftBox_Success");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

//������ӱ�����Ϣ
vector<CGlobalStruct::TFLV> CSDOInfo::SDO_Challenge_InsertAll(int userByID, char * ServerName, char * ServerIP, int weekDay, int matPtMin, int stPtMin, int GCash, int scence, int isBattle)
{
	vector<CGlobalStruct::TFLV> DBTFLV;
	COperatorSDO operSDO;

	try
	{
		char readTag[20];//��ȡ�����ļ��ı�ǩ��Ϣ		
		char strWeekDay[10];//���������ܼ�������
		char SceneName[64];//����������
		char BattleMode[20];//����ģʽ������
		ZeroMemory(readTag,20);//��ʼ����ȡ�����ļ��ı�ǩ��Ϣ
		ZeroMemory(strWeekDay,10);//��ʼ�����������ܼ�������
		ZeroMemory(SceneName,64);//��ʼ������������
		ZeroMemory(BattleMode,20);//��ʼ������ģʽ������
		
		sprintf(readTag, "%s%i", "WeekDay", weekDay);
		logFile.ReadValue("SDO","Code",readTag,strWeekDay,10);//��ȡ��ǰ�����ڼ�
		
		operSDO.getRemoteSql("SDO", "SDO_SceneName_Query",operRemoteSql, 1);//��ȡ��ѯ�������Ƶ�sql���
		operSDO.QuerySingleValue(SceneName, "", localIP, 0, operRemoteSql, scence);//��ȡ��ǰ��������
		
		ZeroMemory(readTag,20);//��ʼ����ȡ�����ļ��ı�ǩ��Ϣ
		if (isBattle == 10)
		{
			sprintf(readTag, "%s", "BattleType1");
			logFile.ReadValue("SDO","Code",readTag,BattleMode,20);//��ȡ��ǰ�ı���ģʽ
		}
		else if (isBattle == 13)
		{
			sprintf(readTag, "%s", "BattleType2");
			logFile.ReadValue("SDO","Code",readTag,BattleMode,20);//��ȡ��ǰ�ı���ģʽ
		}
		else if (isBattle == 12)
		{
			sprintf(readTag, "%s", "BattleType3");
			logFile.ReadValue("SDO","Code",readTag,BattleMode,20);//��ȡ��ǰ�ı���ģʽ
		}
		else if (isBattle == 11)
		{
			sprintf(readTag, "%s", "BattleType4");
			logFile.ReadValue("SDO","Code",readTag,BattleMode,20);//��ȡ��ǰ�ı���ģʽ
        }
		
		operSDO.GetLogText(strLog, "SDO_Challenge_InsertAll", "", "", operName, ServerName, strWeekDay,stPtMin,matPtMin,GCash,SceneName,BattleMode);//����ִ�в�������־
		operSDO.WriteText("SDO",strLog);

		char SuccessSvr[500];
		char FailSvr[500];
		ZeroMemory(SuccessSvr, 500);//��ʼ����ӳɹ��ķ���������
		ZeroMemory(FailSvr, 500);//��ʼ�����ʧ�ܵķ���������

		char tempServerIP[64];
		ZeroMemory(tempServerIP,64);

		const char* split = ",";//��������ַ��","����
		int splitNum = 1;//����Ϊ��ȡ��һ��","��
		
		char tmpServerIP[64];
		char tmpServerName[64];
		ZeroMemory(tmpServerIP, 64);
		ZeroMemory(tmpServerName, 64);
		operSDO.strGetChar(ServerIP, split, tmpServerIP, 64, splitNum);//��ȡ��һ��������IP
		operSDO.strGetChar(ServerName, split, tmpServerName, 64, splitNum);//��ȡ��һ������������
		int tmpLength = 0;
		while(strcmp("",tmpServerIP))
		{
			if(!operSDO.GetServerIP(tempServerIP,tmpServerIP,2))
			{
				return DBTFLV;//�޷���ȡ�����ķ�����IP
			}
			
			if(!operSDO.getRemoteSql("SDO", "SDO_Challenge_Insert",operRemoteSql, 1))
			{
				return DBTFLV;//�޷��õ���Ӵ�����ս��Ϣ��sql���
			}

			tmpLength = GameDBOper("SDO", tempServerIP, 0, operRemoteSql,Item_DBName,weekDay,matPtMin,stPtMin,GCash,scence,isBattle);

			if(tmpLength == 0)
			{
				if(strcmp("",FailSvr))
				{
					sprintf(FailSvr,"%s,%s",FailSvr,tmpServerName);
				}
				else
				{
					sprintf(FailSvr,"%s",tmpServerName);
				}
			}
			else
			{
				if(strcmp("",SuccessSvr))
				{
					sprintf(SuccessSvr,"%s,%s",SuccessSvr,tmpServerName);
				}
				else
				{
					sprintf(SuccessSvr,"%s",tmpServerName);
				}
			}
			splitNum++;//ѭ������","�ĸ���
			operSDO.strGetChar(ServerIP, split, tmpServerIP, 64, splitNum);//��ȡ��splitNum��������IP
	    	operSDO.strGetChar(ServerName, split, tmpServerName, 64, splitNum);//��ȡ��splitNum������������
		}
		ZeroMemory(strLog, 2048);
		if(!strcmp("",FailSvr))
		{			
			operSDO.GetLogText(strLog, "SDO_Challenge_InsertAll", "", "Success", operName, ServerName, strWeekDay,stPtMin,matPtMin,GCash,SceneName,BattleMode);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_Challenge_InsertAll",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"InsertAll_Challenge_Success");
		}
		else if(!strcmp("",SuccessSvr))
		{
			operSDO.GetLogText(strLog, "SDO_Challenge_InsertAll", "", "Fail", operName, ServerName, strWeekDay,stPtMin,matPtMin,GCash,SceneName,BattleMode);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_Challenge_InsertAll",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"InsertAll_Challenge_Fail");
		}
		else
		{
			operSDO.GetLogText(strLog, "SDO_Challenge_InsertAll_Result", "", "", operName, SuccessSvr, FailSvr, strWeekDay,stPtMin,matPtMin,GCash,SceneName,BattleMode);//����ִ�в�������־
			operSDO.WriteDBLog(userByID,"SDO","SDO_Challenge_InsertAll",ServerIP,strLog);
			
			DBTFLV = operSDO.ReturnOpMsg(strLog,"MiddleState");
		}
	}
	catch(...)
	{
		DBTFLV = operSDO.ReturnOpMsg(strLog,"Error");
	}

	return DBTFLV;
}

/************************************************************************/
/* //�鿴������ϵ���
bool CSDOInfo::SDO_UserEquip_Query()
{
try
{
operSDO.GetLogText(strLog, "BaseMessage", "SDO_UserEquip", "", m_UserName,SDO_ServerName,SDO_Account);
operSDO.WriteText("SDO",strLog);

		if(!operSDO.GetServerIP(SDO_ServerIP,SDO_ServerIP,2))
		{
		return false;//�޷���ȡ�����ķ�����IP
		}
		
		  if(!operSDO.getRemoteSql("SDO_UserEquip_Query",operRemoteSql))
		  {
		  return false;//�޷��õ��鿴������ϵ��ߵ�sql���
		  }
		  sprintf(operszSql,operRemoteSql,Item_DBName,Item_DBName,UserID);//����������ѯ��sql���
		  
			int tmpLength = DBVect.OperVectorMain("SDO", SDO_ServerIP, operszSql, 0, index, iPageSize);//��������ݿ��ѯ�õ�������
			operSDO.ReturnData(&DBVect, tmpLength, operszSql);//��������
			}
			catch(...)
			{
			operSDO.ReturnOpMsg(strLog,"Error");//��������Ϣд����־�в����ش�����Ϣ
			}
			return false;
}                                                                     */
/************************************************************************/
//��ȡ�������� <iSouNT: 1 ��ʱ���棬0 ��ʱ����
void CSDOInfo::GetNoticeType(char *strDesNT,int &iDesNT, int iSouNT)
{
	if (iSouNT == 1)		//��ʱ����
	{
		logFile.ReadValue("SDO", "NOTICETYPE", "NOtice_inTime", strDesNT, sizeof(&strDesNT));
		iDesNT = -1;
	}
	else					//��ʱ����
	{
		logFile.ReadValue("SDO", "NOTICETYPE", "NOtice_Time", strDesNT, sizeof(&strDesNT));
		iDesNT = 0;
	}
}