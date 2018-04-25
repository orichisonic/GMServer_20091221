#include "stdafx.h"
#include ".\operatorcg2.h"
#include "time.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//���캯��
COperatorCG2::COperatorCG2(void)
{
///	ZeroMemory(connLocalDB, 20);//��ʼ��������IP
//	ZeroMemory(petname, 128);//��ʼ����������
//	ZeroMemory(m_sql, 256);//��ʼ�����ز�ѯ��sql���
//	sprintf(connLocalDB,"127.0.0.1");
}

//��������
COperatorCG2::~COperatorCG2(void)
{
	SendVectBuf.Destroy();
}

//��ʼ���������ݵ���
bool COperatorCG2::initialize(CSession * pSess,SOCKET s_Socket)
{
	ZeroMemory(connLocalDB, 20);//��ʼ��������IP
	char path[256];
	GetCurrentDirectory(256,path);
	char filepath[256];
	sprintf(filepath,"%s%s",path,"\\config\\scheme.ini");//���������ļ���·��
	GetPrivateProfileString("DATABASE","Address","127.0.0.1",connLocalDB,20,filepath);//��ȡ���ص����ݿ��ַ
	
	ZeroMemory(petname, 128);//��ʼ����������
	mSocket.SetSocket(s_Socket);//����SOCKET
	ZeroMemory(m_sql, 256);//��ʼ�����ز�ѯ��sql���

	pCG2Session=(CCG2Session *)pSess;
	return true;
}
bool COperatorCG2::initialize()
{
	ZeroMemory(connLocalDB, 20);//��ʼ��������IP
	char path[256];
	GetCurrentDirectory(256,path);
	char filepath[256];
	sprintf(filepath,"%s%s",path,"\\config\\scheme.ini");//���������ļ���·��
	GetPrivateProfileString("DATABASE","Address","127.0.0.1",connLocalDB,20,filepath);//��ȡ���ص����ݿ��ַ
	ZeroMemory(m_sql, 256);//��ʼ�����ز�ѯ��sql���

	return true;
}
bool COperatorCG2::ReadValue(char * strKey,char * strValue,int strLength)
{
	return logFile.ReadValue("M_CG2","CG2",strKey,strValue,strLength);
}


//���ص�����Ϣ
void COperatorCG2::ReturnMessage(unsigned int nid, CEnumCore::Msg_Category m_category, CEnumCore::ServiceKey mServiceKey, char* strMessage)
{
	try
	{
		SendVectBuf.InitCreate(1,2);
		
		//���TFLV�����ݰ���
		SendVectBuf.AddTFLV(CEnumCore::TagName::MESSAGE,CEnumCore::TagFormat::TLV_STRING,strlen(strMessage),(unsigned char*)strMessage);
		
		//�������ݰ�����ȡ����
		int vectLength = SendVectBuf.EnBody();
		int sBodyLength = 0;
		
		CSocketData SendDataBuf;
		SendDataBuf.Create(vectLength);//���췢�����ݰ�
		SendDataBuf.AddHeader(nid, m_category, mServiceKey, vectLength);//������ݰ���ͷ��
		SendDataBuf.EnSendBuf(&sBodyLength, SendVectBuf.getDes(), vectLength);//���ܲ����췢�����ݰ�
		
		pCG2Session->Send(SendDataBuf.GetSendBuf(),sBodyLength);
	
		SendDataBuf.Destory();
	}
	catch(...)
	{
	}
}
void COperatorCG2::ReturnData(CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,unsigned char * lpdata,int length)
{
	try
	{
		if(length == 0)
		{
			ReturnMessage(1,m_category, mServiceKey, "��ѯ���ݿ���Ϊ��");
			return;
		}
		
		CSocketData socketdata;
		
		
		socketdata.Create(length);
		socketdata.AddHeader(1,m_category,mServiceKey,length);
		socketdata.EnSendBuf(&length,lpdata,length);
		
		pCG2Session->Send(socketdata.GetSendBuf(),length);
		socketdata.Destory();
	}
	catch(...)
	{
		
	}
	
}
//��ȡSqlExpress�е���Ӧsql���
void COperatorCG2::getRemoteSql(char * lpFlag,char* lpSql)
{
	try
	{
		vector<CGlobalStruct::DataRecord> datarecord;
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		sprintf(m_sql,"SELECT sql_statement FROM SqlExpress WHERE (sql_type = '%s')", lpFlag);
		m_SqlHelper.SqlHelperMain("CG2",&datarecord,"127.0.0.1", m_sql, 0, 0, 0);//��ȡSqlExpress�е���Ӧsql���
		
		if(datarecord.empty())//�����¼Ϊ��
		{
			sprintf(lpSql,"");
			return;
		}
		
		//��ȡ��ѯ�ļ�¼����ÿһ��
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("sql_statement", m_DataRecord.recordfieldname))//��ȡ��Ӧ��LogDBIP
			{
				sprintf(lpSql, "%s", m_DataRecord.recorddata);
			}
		}
		if(!datarecord.empty())
		{
			datarecord.clear();
		}
		
	}
	catch(...)
	{
		sprintf(lpSql,"");
	}
}
//��������
void COperatorCG2::ReturnData(unsigned int nid,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey, COperVector* m_OpVect, int SendLength)
{
	try
	{
		
		if(SendLength == 0)
		{
			ZeroMemory(retMessage,256);
			logFile.ReadValue("M_Global","Global","DataBaseWithoutRecord",retMessage[0],256);
			ReturnMessage(nid, m_category, mServiceKey, retMessage[0]);
		}
		unsigned int uiID;
		unsigned int bCategory = (unsigned short int)m_category;
		
		unsigned int bServicekey = (unsigned short int )mServiceKey;
		uiID =(unsigned int)(bCategory<<16) + (unsigned int)bServicekey;
		CEnumCore::Message_Tag_ID m_message_tag_id=uiID;


		char strInt[10];

		char m_result[256];//����ÿһ��Ľ��
		ZeroMemory(m_result,256);//��ʼ��ÿһ��Ľ��
		char procName[256];
		SendVectBuf.InitCreate(m_OpVect->getQueryNum()+1, m_OpVect->getColTFLVNum()+5);//��ʼ�����͵�����
		for(int i=0; i < m_OpVect->getAllTFLV(); i++)
		{
			CGlobalStruct::TFLV m_tflv = m_OpVect->getTLVBynIndex(i+1);//��ȡÿһ��TFLV

			if(m_tflv.m_tagName == CEnumCore::TagName::CG2_JobID)//ְҵID
			{
				try
				{
					//��ȡ�������͵�˵��
					ZeroMemory(m_result,256);
					
					sprintf(procName,"exec CG2_JobName_query %i",StrToInt((char *)&m_tflv.lpdata));//��ʼ���洢���̵����
					CG2_Proc_Query(connLocalDB,4,procName,m_result);
					if(strlen(m_result)>0)
					{
						SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_JobID, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
					else
					{
						SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV	
					}
				}
				catch(...)
				{
				}
			}
			else if (m_tflv.m_tagName == CEnumCore::TagName::CG2_MapID)//��ͼID
			{
				try
				{
						//��ȡ�������͵�˵��
					ZeroMemory(m_result,256);
						
					sprintf(procName,"exec CG2_MapName_query %i",StrToInt((char *)&m_tflv.lpdata));//��ʼ���洢���̵����
					CG2_Proc_Query(connLocalDB,4,procName,m_result);
					if(strlen(m_result)>0)
					{
						SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_MapID, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
					else
					{
						SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
					}
				}
				catch(...)
				{
				}
			}
			else if(m_tflv.m_tagName == CEnumCore::TagName::CG2_SkillName)
			{
				try
				{
					//��ȡ�������͵�˵��
					ZeroMemory(m_result,256);
					
					sprintf(procName,"exec CG2_SkillName_query %i",StrToInt((char *)&m_tflv.lpdata));//��ʼ���洢���̵����
					CG2_Proc_Query(connLocalDB,4,procName,m_result);
					if(strlen(m_result)>0)
					{
						SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_SkillName, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
					else
					{
						SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
					}
					
				}
				catch(...)
				{
				}
			}
			else if(m_tflv.m_tagName == CEnumCore::TagName::CG2_ProHonorRank)
			{
				try
				{
					//��ȡ�������͵�˵��
					ZeroMemory(m_result,256);
					sprintf(procName,"exec CG2_FameName_query -1,%i",StrToInt((char *)&m_tflv.lpdata));//��ʼ���洢���̵����
					CG2_Proc_Query(connLocalDB,4,procName,m_result);
					if(strlen(m_result)>0)
					{
						SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_ProHonorRank, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
					else
					{
						SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
					}
				}
				catch(...)
				{
				}
			}
			else if(m_tflv.m_tagName == CEnumCore::TagName::CG2_ItemName)
			{
				try
				{
					//��ȡ�������͵�˵��
					ZeroMemory(m_result,256);
					
					sprintf(procName,"exec CG2_ItemName_query %i",StrToInt((char *)&m_tflv.lpdata));//��ʼ���洢���̵����
					CG2_Proc_Query(connLocalDB,4,procName,m_result);
					if(strlen(m_result)>0)
					{
						SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_ItemName, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
					else
					{
						SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
					}
				}
				catch(...)
				{
				}
			}
			else if((m_tflv.m_tagName == CEnumCore::TagName::CG2_PetPhysical)
				||(m_tflv.m_tagName == CEnumCore::TagName::CG2_Power)
				||(m_tflv.m_tagName == CEnumCore::TagName::CG2_Smart)
				||(m_tflv.m_tagName == CEnumCore::TagName::CG2_Speed)
				||(m_tflv.m_tagName == CEnumCore::TagName::CG2_Spirit)
				||(m_tflv.m_tagName == CEnumCore::TagName::CG2_PetAttack)
				||(m_tflv.m_tagName == CEnumCore::TagName::CG2_PetDefend)
				||(m_tflv.m_tagName == CEnumCore::TagName::CG2_PetCheesy)
				||(m_tflv.m_tagName == CEnumCore::TagName::CG2_PetMagAttack)
				||(m_tflv.m_tagName == CEnumCore::TagName::CG2_PetMagDefend)
				||(m_tflv.m_tagName == CEnumCore::TagName::CG2_PetRecover)
				||(m_tflv.m_tagName == CEnumCore::TagName::CG2_PetHit)
				||(m_tflv.m_tagName == CEnumCore::TagName::CG2_PetAvoid)
				||(m_tflv.m_tagName == CEnumCore::TagName::CG2_PetCritical)
				||(m_tflv.m_tagName == CEnumCore::TagName::CG2_PetRetort)
				||(m_tflv.m_tagName == CEnumCore::TagName::CG2_PetMaxBlood)
				||(m_tflv.m_tagName == CEnumCore::TagName::CG2_PetMaxMag))
			{
				_itoa(floor(atof((char *)&m_tflv.lpdata)),strInt,10);
				SendVectBuf.AddTFLV(m_tflv.m_tagName,CEnumCore::TagFormat::TLV_INTEGER,sizeof(int),(unsigned char *)&strInt);
			}
			else if(m_tflv.m_tagName == CEnumCore::TagName::CG2_PetFaith)
			{
				int rLoyalty = 0;
				char *lpParstr=strtok((char *)&m_tflv.lpdata,",");//��ѯ��־","
				int loyalty = atoi(lpParstr);
				lpParstr=strtok(NULL,",");
				int uLv = atoi(lpParstr);
				lpParstr=strtok(NULL,",");
				int charisma = atoi(lpParstr);
				lpParstr=strtok(NULL,",");
				int pLv = atoi(lpParstr);
				int loyaltyBase = uLv*charisma*100;
				int loyaltyPetRate = pLv * 250;
				if( loyaltyPetRate > 0 )
				{
					// ���\�����
					rLoyalty = ( loyaltyBase + ( loyaltyPetRate / 2 ) ) / loyaltyPetRate;
				}
				if( loyalty < 0 || loyalty >= 20*100 )
				{
					// �ڥåȤΤʤĤ��ȼӤ���
					rLoyalty += ( loyalty / 100 );
				}
				// �ƥ����o������󂎥����å�
				if( rLoyalty > 60 )
				{
					rLoyalty = 60;
				}
				
				// ��ͣ�
				if( rLoyalty < 0 )
				{
					rLoyalty = 0;
				}
				_itoa(rLoyalty,strInt,10);
				SendVectBuf.AddTFLV(m_tflv.m_tagName,CEnumCore::TagFormat::TLV_INTEGER,sizeof(int),(unsigned char *)&strInt);

				
				
			}
			else
			{
				try
				{
					SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
				}
				catch(...)
				{
				}
					
			}
			
		}

		int midLength = SendVectBuf.EnBody();
		int sBodyLength = 0;
		CSocketData SendDataBuf;
		SendDataBuf.Create(midLength);//��ʼ��Ҫ���͵�����
		SendDataBuf.AddHeader(nid, m_category, mServiceKey, midLength);
		SendDataBuf.EnSendBuf(&sBodyLength, (unsigned char *)SendVectBuf.getDes(), SendDataBuf.GetPacketHead().bodylen);
		pCG2Session->Send(SendDataBuf.GetSendBuf(),sBodyLength);

		SendDataBuf.Destory();
	}
	catch(...)
	{
	}
}



//�������ݣ������ݣ�
void COperatorCG2::ReturnData(unsigned int nid,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey, COperVector* m_OpVect, COperVector* m_OpVect1,int iIndex,int iPageSize)
{
	try
	{
		int alltflv=m_OpVect->getAllTFLV()+m_OpVect1->getAllTFLV();//����TFLV
		char strInt[10];
		if(alltflv==0)
		{
			ReturnMessage(nid, m_category, mServiceKey, "��ѯ���ݿ���Ϊ��");
		}
		
		
		int fieldnum=m_OpVect->getColTFLVNum();//����
		//����
		int allquery=0;
		if(alltflv%fieldnum==0)
		{
			allquery=alltflv/fieldnum;
		}
		else
		{
			allquery=alltflv/fieldnum+1;
		}
		
		//���ҳ��
		int PageNum=0;
		if(allquery%iPageSize==0)
		{
			PageNum=allquery/iPageSize;
		}
		else
		{
			PageNum=allquery/iPageSize+1;
		}
		

		
		int allreturntflv=iPageSize*fieldnum;
		int firsttflv=(iIndex-1)*iPageSize*fieldnum;

		int i=0;
		char m_result[256];//����ÿһ��Ľ��
		ZeroMemory(m_result,256);//��ʼ��ÿһ��Ľ��
		char procName[256];
		SendVectBuf.InitCreate(alltflv/fieldnum+1, fieldnum+5);//��ʼ�����͵�����
		for(i=0; i < m_OpVect->getAllTFLV(); i++)
		{
			CGlobalStruct::TFLV m_tflv = m_OpVect->getTLVBynIndex(i+1);//��ȡÿһ��TFLV
			if(firsttflv==0)
			{
					if(allreturntflv==0)
					{
						break;
					}
					else
					{
						if(m_tflv.m_tagName == CEnumCore::TagName::CG2_JobID)//ְҵID
						{
							try
							{
								//��ȡ�������͵�˵��
								ZeroMemory(m_result,256);
								
								sprintf(procName,"exec CG2_JobName_query %i",StrToInt((char *)&m_tflv.lpdata));//��ʼ���洢���̵����
								CG2_Proc_Query(connLocalDB,4,procName,m_result);
								if(strlen(m_result)>0)
								{
									SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_JobID, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
								}
								else
								{
									SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
								}
							}
							catch(...)
							{
							}
						}
						else if (m_tflv.m_tagName == CEnumCore::TagName::CG2_MapID)//��ͼID
						{
							try
							{
								//��ȡ�������͵�˵��
								ZeroMemory(m_result,256);
								
								sprintf(procName,"exec CG2_MapName_query %i",StrToInt((char *)&m_tflv.lpdata));//��ʼ���洢���̵����
								CG2_Proc_Query(connLocalDB,4,procName,m_result);
								if(strlen(m_result)>0)
								{
									SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_MapID, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
								}	
								else
								{
									SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
								}
							}
							catch(...)
							{
							}
						}
						else if(m_tflv.m_tagName == CEnumCore::TagName::CG2_SkillName)
						{
							try
							{
								//��ȡ�������͵�˵��
								ZeroMemory(m_result,256);
								
								sprintf(procName,"exec CG2_SkillName_query %i",StrToInt((char *)&m_tflv.lpdata));//��ʼ���洢���̵����
								CG2_Proc_Query(connLocalDB,4,procName,m_result);
								if(strlen(m_result)>0)
								{
									SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_SkillName, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
								}
								else
								{
									SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
								}
							}
							catch(...)
							{
							}
						}
						else if(m_tflv.m_tagName == CEnumCore::TagName::CG2_ProHonorRank)
						{
							try
							{
								//��ȡ�������͵�˵��
								ZeroMemory(m_result,256);
								
								sprintf(procName,"exec CG2_FameName_query -1,%i",StrToInt((char *)&m_tflv.lpdata));//��ʼ���洢���̵����
								CG2_Proc_Query(connLocalDB,4,procName,m_result);
								if(strlen(m_result)>0)
								{
									SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_ProHonorRank, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
								}
								else
								{
									SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
								}
							}
							catch(...)
							{
							}
						}
						else if(m_tflv.m_tagName == CEnumCore::TagName::CG2_ItemName)
						{
							try
							{
								//��ȡ�������͵�˵��
								ZeroMemory(m_result,256);
								
								sprintf(procName,"exec CG2_ItemName_query %i",StrToInt((char *)&m_tflv.lpdata));//��ʼ���洢���̵����
								CG2_Proc_Query(connLocalDB,4,procName,m_result);
								if(strlen(m_result)>0)
								{
									SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_ItemName, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
								}	
								else
								{
									SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
								}
							}
							catch(...)
							{
							}
						}
						else if(m_tflv.m_tagName == CEnumCore::TagName::PageCount)
						{
							_itoa(PageNum,strInt,10);
							SendVectBuf.AddTFLV(CEnumCore::TagName::PageCount, CEnumCore::TagFormat::TLV_INTEGER, 4, (unsigned char*)&strInt);
							allreturntflv=allreturntflv-1;
						}
						else
						{
							SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
							allreturntflv=allreturntflv-1;
						}
						
					}
		
			}
			else
			{
				firsttflv=firsttflv-1;
			}
		}
		for(i=0; i < m_OpVect1->getAllTFLV(); i++)
		{
			CGlobalStruct::TFLV m_tflv = m_OpVect1->getTLVBynIndex(i+1);//��ȡÿһ��TFLV
			if(firsttflv==0)
			{
					if(allreturntflv==0)
					{
						break;
					}
					else
					{
						if(m_tflv.m_tagName == CEnumCore::TagName::CG2_JobID)//ְҵID
						{
							try
							{
								//��ȡ�������͵�˵��
								ZeroMemory(m_result,256);
								
								sprintf(procName,"exec CG2_JobName_query %i",StrToInt((char *)&m_tflv.lpdata));//��ʼ���洢���̵����
								CG2_Proc_Query(connLocalDB,4,procName,m_result);
								if(strlen(m_result)>0)
								{
									SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_JobID, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
								}	
								else
								{
									SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
								}
							}
							catch(...)
							{
							}
						}
						else if (m_tflv.m_tagName == CEnumCore::TagName::CG2_MapID)//��ͼID
						{
							try
							{
								//��ȡ�������͵�˵��
								ZeroMemory(m_result,256);
								
								sprintf(procName,"exec CG2_MapName_query %i",StrToInt((char *)&m_tflv.lpdata));//��ʼ���洢���̵����
								CG2_Proc_Query(connLocalDB,4,procName,m_result);
								if(strlen(m_result)>0)
								{
									SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_MapID, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
								}	
								else
								{
									SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
								}
							}
							catch(...)
							{
							}
						}
						else if(m_tflv.m_tagName == CEnumCore::TagName::CG2_SkillName)
						{
							try
							{
								//��ȡ�������͵�˵��
								ZeroMemory(m_result,256);
								
								sprintf(procName,"exec CG2_SkillName_query %i",StrToInt((char *)&m_tflv.lpdata));//��ʼ���洢���̵����
								CG2_Proc_Query(connLocalDB,4,procName,m_result);
								if(strlen(m_result)>0)
								{
									SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_SkillName, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
								}
								else
								{
									SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
								}
							}
							catch(...)
							{
							}
						}
						else if(m_tflv.m_tagName == CEnumCore::TagName::CG2_ProHonorRank)
						{
							try
							{
								//��ȡ�������͵�˵��
								ZeroMemory(m_result,256);
								
								sprintf(procName,"exec CG2_FameName_query -1,%i",StrToInt((char *)&m_tflv.lpdata));//��ʼ���洢���̵����
								CG2_Proc_Query(connLocalDB,4,procName,m_result);
								if(strlen(m_result)>0)
								{
									SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_SkillName, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
								}
								else
								{
									SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
								}
							}
							catch(...)
							{
							}
						}
						else if(m_tflv.m_tagName == CEnumCore::TagName::PageCount)
						{
							_itoa(PageNum,strInt,10);
							SendVectBuf.AddTFLV(CEnumCore::TagName::PageCount, CEnumCore::TagFormat::TLV_INTEGER, 4, (unsigned char*)&strInt);
							allreturntflv=allreturntflv-1;
						}
						else
						{
							SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
							allreturntflv=allreturntflv-1;
						}
					}
				
			}
			else
			{
					firsttflv=firsttflv-1;
			}

		}
		
		int midLength = SendVectBuf.EnBody();
		int sBodyLength = 0;
		CSocketData SendDataBuf;
		SendDataBuf.Create(midLength);//��ʼ��Ҫ���͵�����
		SendDataBuf.AddHeader(nid, m_category, mServiceKey, midLength);
		SendDataBuf.EnSendBuf(&sBodyLength, (unsigned char *)SendVectBuf.getDes(), SendDataBuf.GetPacketHead().bodylen);
		pCG2Session->Send(SendDataBuf.GetSendBuf(),sBodyLength);
			
		SendDataBuf.Destory();
	}
	catch(...)
	{
	}
}
//��ȡ���ݿ�ID
int COperatorCG2::GetDBID(char * remoteip,char* username)
{
	int result = -1;
	char m_RemoteSql[1024];//����Զ�����ݿ������sql���
	char m_sql[1024];
	getRemoteSql("CG2_DB_Query",m_RemoteSql);//��ȡԶ�����ݿ������sql���
	if(!strcmp("",m_RemoteSql))
	{
		return -1;
	}
	vector<CGlobalStruct::DataRecord> datarecord;
	if(!datarecord.empty())
	{
		datarecord.clear();
	}
	sprintf(m_sql,m_RemoteSql,username);
	logFile.WriteLogFile(m_sql);
	m_SqlHelper.SqlHelperMain("CG2",&datarecord, remoteip, m_sql, 3, 1, 1);//��ȡ��Ӧ��LogDBIP

	if(datarecord.empty())//�����¼Ϊ��
	{
		return -1;
	}
	
	//��ȡ��ѯ�ļ�¼����ÿһ��
	vector<CGlobalStruct::DataRecord>::iterator iter;
	CGlobalStruct::DataRecord  m_DataRecord;	
	for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
	{
		m_DataRecord = *iter;
		if(!strcmp("dbid", m_DataRecord.recordfieldname))//��ȡ��Ӧ�ļ�¼��
		{
			result=StrToInt(m_DataRecord.recorddata);
		}
	}
	if(!datarecord.empty())
	{
		datarecord.clear();
	}
	return result;
}



//���ָ������һ���ֶ�ֵ
void COperatorCG2::CG2_Proc_Query(char * lpIP,int flag,char* procName,char* result)
{
	
	vector<CGlobalStruct::DataRecord> datarecord;
	if(!datarecord.empty())
	{
		datarecord.clear();
	}
	m_SqlHelper.SqlHelperMain("CG2",&datarecord, lpIP, procName, flag, 0, 0);//��ȡ��Ӧ��LogDBIP
	
	if(datarecord.empty())//�����¼Ϊ��
	{
		result="";
	}
	
	//��ȡ��ѯ�ļ�¼����ÿһ��
	vector<CGlobalStruct::DataRecord>::iterator iter;
	CGlobalStruct::DataRecord  m_DataRecord;	
	for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
	{
		m_DataRecord = *iter;
		sprintf(result,"%s",m_DataRecord.recorddata);
		break;
	}
	if(!datarecord.empty())
	{
		datarecord.clear();
	}
}

//ͨ��DBID��IP
void COperatorCG2::FindDBIP(char * lpIP,char * lpNewIP,int dbid)
{
	char szSql[256];
	vector<CGlobalStruct::DataRecord> datarecord;
	if(!datarecord.empty())
	{
		datarecord.clear();
	}
	sprintf(szSql,"exec CG2_FindDBIP '%s',%i",lpIP,dbid);
	m_SqlHelper.SqlHelperMain("CG2",&datarecord,connLocalDB , szSql, 4, 0, 0);//��ȡ��Ӧ��LogDBIP
	
	if(datarecord.empty())//�����¼Ϊ��
	{
		lpNewIP="";
	}
	
	//��ȡ��ѯ�ļ�¼����ÿһ��
	vector<CGlobalStruct::DataRecord>::iterator iter;
	CGlobalStruct::DataRecord  m_DataRecord;	
	for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
	{
		m_DataRecord = *iter;
		if(!strcmp("serverip", m_DataRecord.recordfieldname))//��ȡ��Ӧ�ļ�¼��
		{
			sprintf(lpNewIP,"%s",m_DataRecord.recorddata);
		}
	}
	if(!datarecord.empty())
	{
		datarecord.clear();
	}
}


//�����������ݣ���ɫ�ƺţ�
void COperatorCG2::ReturnSpecData(unsigned int nid,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey, COperVector* m_OpVect, int SendLength)
{
		try
		{
			if(SendLength == 0)
			{
				ZeroMemory(retMessage,1024);
				logFile.ReadValue("M_Global","Global","DataBaseWithoutRecord",retMessage[0],256);
				ReturnMessage(nid, m_category, mServiceKey, retMessage[0]);
			}

			char m_result[256];//����ÿһ��Ľ��
			ZeroMemory(m_result,256);//��ʼ��ÿһ��Ľ��
			SendVectBuf.InitCreate(m_OpVect->getQueryNum()+1, m_OpVect->getColTFLVNum()+5);//��ʼ�����͵�����
			for(int i=0; i < m_OpVect->getAllTFLV(); i++)
			{
				CGlobalStruct::TFLV m_tflv = m_OpVect->getTLVBynIndex(i+1);//��ȡÿһ��TFLV
			
				try
				{
					SendVectBuf.AddTFLV(m_tflv);//Ĭ��Ϊֱ�����TFLV
				}
				catch(...)
				{
				}
			
			
		}

		int midLength = SendVectBuf.EnBody();
		int sBodyLength = 0;
		CSocketData SendDataBuf;
		SendDataBuf.Create(midLength);//��ʼ��Ҫ���͵�����
		SendDataBuf.AddHeader(nid, m_category, mServiceKey, midLength);
		SendDataBuf.EnSendBuf(&sBodyLength, (unsigned char *)SendVectBuf.getDes(), SendDataBuf.GetPacketHead().bodylen);
		pCG2Session->Send(SendDataBuf.GetSendBuf(),sBodyLength);
		SendDataBuf.Destory();
	}
	catch(...)
	{
	}
}


///��������ӿ�///////////////////////////
bool COperatorCG2::ReadXml(char * strFlag,char *strUrl,char * strpostData,vector<CGlobalStruct::DataRecord> * pVect,int* iCow,int *iCol)
{
	
	bool r_flag=false;
	try
	{
		CoInitialize(NULL);
		IXMLHTTPRequestPtr xmlrequest; //�������
		xmlrequest.CreateInstance("MSXML2.XMLHTTP");
		xmlrequest->open(_bstr_t("POST"),_bstr_t(strUrl),_variant_t("false"));
		xmlrequest->setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
		xmlrequest->send(strpostData);
		
		BSTR bstrbody;
		xmlrequest->get_responseText(&bstrbody);
		
		_bstr_t bstrtbody(bstrbody);
		
		
		
		CGlobalStruct::DataRecord tmpDataRecord;
		IXMLDOMDocumentPtr   pDoc;  
		IXMLDOMNodeListPtr pNoteList,pSecNoteList;
		IXMLDOMNodePtr  pNode,pChildNote,pSecChildNote;
		
		
		
		HRESULT hr = pDoc.CreateInstance(__uuidof(DOMDocument)); 
		pDoc->loadXML(bstrtbody);
		cout<<static_cast <char*>(pDoc->xml); 
		pNode=pDoc->selectSingleNode("you9/status");
		
		
		
		if(!strcmp((LPCSTR)_bstr_t(pNode->text),"RESULT_00")||!strcmp((LPCSTR)_bstr_t(pNode->text),"RESULT_0"))
		{
			pNode=pDoc->selectSingleNode("you9");
			pNoteList=pNode->GetchildNodes();
			*iCow=pNoteList->length-1;
			for(int i=1;i<pNoteList->length;i++)
			{
				pChildNote=pNoteList->Getitem(i);
				pSecNoteList=pChildNote->GetchildNodes();
				*iCol=pSecNoteList->length;
				for(int j=0;j<pSecNoteList->length;j++)
				{
					pSecChildNote=pSecNoteList->Getitem(j);
					sprintf(tmpDataRecord.recordfieldname,"%s",(LPCSTR)_bstr_t(pSecChildNote->GetnodeName()));
					tmpDataRecord.recordfieldtype=3;
					sprintf(tmpDataRecord.recorddata,"%s",(LPCSTR)_bstr_t(pSecChildNote->text));
					pVect->push_back(tmpDataRecord);
				}
				
			}
			r_flag=true;
		}
		else 
		{
			sprintf(tmpDataRecord.recordfieldname,"%s","MESSAGE");
			tmpDataRecord.recordfieldtype=3;
			ReadValue((char *)_bstr_t(pNode->text),retMessage[0],256);
			sprintf(tmpDataRecord.recorddata,"%s",retMessage);
			pVect->push_back(tmpDataRecord);
			r_flag=false;
			
		}
		pNoteList.Release();
		pSecNoteList.Release();
		pNode.Release();
		pChildNote.Release();
		pSecChildNote.Release();
		pDoc.Release(); 
		xmlrequest.Release();
		CoUninitialize();
	}
	catch (...)
	{
		
	}
	return r_flag;

}
bool COperatorCG2::ReadXmlGet(char * strFlag,char *strUrl,char * strpostData,vector<CGlobalStruct::DataRecord> * pVect,int* iCow,int *iCol)
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
		cout<<static_cast <char*>(pDoc->xml); 
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
	catch (...)
	{
		
	}
	return r_flag;
	
}
void COperatorCG2::ReturnData(SOCKET m_socket,COperVector* m_socketdata,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey,int iIndex,int iPageSize)
{
	CLogFile logFile;
	try
	{
		
		COperVector newsendsocketdata;
		int alllength=0;
		int PageCount=0;
		char strInt[10];
		int acttflv=0;
		int actquery=0;
		int i=0;
		CSocketData SendDataBuf;
		bool addpageflag=false;
		
		
		
		if(iIndex==0&&iPageSize==0)
		{
			int midLength = m_socketdata->EnBody();
			int sBodyLength = 0;
			mSocket.SetSocket(m_socket);
			SendDataBuf.Create(midLength);//��ʼ��Ҫ���͵�����
			SendDataBuf.AddHeader(1, m_category, mServiceKey, midLength);
			SendDataBuf.EnSendBuf(&sBodyLength, (unsigned char *)(m_socketdata->getDes()), SendDataBuf.GetPacketHead().bodylen);
			pCG2Session->Send(SendDataBuf.GetSendBuf(),sBodyLength);
			SendDataBuf.Destory();
		}
		else
		{
			////////////��ȡʵ������/////////////////
			acttflv=m_socketdata->getAllTFLV();
			int querynum=0;
			int colnum=0;
			if(m_socketdata->IsActiveCol())
			{
				colnum=m_socketdata->getColTFLVNum();
			}
			else
			{
				colnum=acttflv;
			}
			querynum=m_socketdata->getAllTFLV()/colnum;
			
			
			if(querynum%iPageSize==0)
			{
				PageCount=querynum/iPageSize;
			}
			else
			{
				PageCount=querynum/iPageSize+1;
			}
			newsendsocketdata.InitCreate(iPageSize,colnum+2);
			CGlobalStruct::TFLV tmptflv;
			tmptflv = m_socketdata->getTLVBynIndex(1);//��ȡÿһ��TFLV
			newsendsocketdata.AddTFLV(tmptflv);
			for(i=0; i < m_socketdata->getAllTFLV(); i++)
			{
				
				if(i>(iIndex-1)*iPageSize*colnum&&i<=iIndex*iPageSize*colnum)
				{
					tmptflv = m_socketdata->getTLVBynIndex(i+1);//��ȡÿһ��TFLV
					newsendsocketdata.AddTFLV(tmptflv.m_tagName,tmptflv.m_tagFormat,tmptflv.m_tvlength,tmptflv.lpdata);
					if((i+1)%colnum==0)
					{
						addpageflag=true;
						_itoa(PageCount,strInt,10);
						newsendsocketdata.AddTFLV(CEnumCore::TagName::PageCount,CEnumCore::TagFormat::TLV_INTEGER,sizeof(int),(unsigned char *)&strInt);
					}
				}
			}
			if(colnum==m_socketdata->getAllTFLV()&&addpageflag!=true)
			{
				_itoa(PageCount,strInt,10);
				newsendsocketdata.AddTFLV(CEnumCore::TagName::PageCount,CEnumCore::TagFormat::TLV_INTEGER,sizeof(int),(unsigned char *)&strInt);
			}
			int midLength = newsendsocketdata.EnBody();
			int sBodyLength = 0;
			//mSocket.SetSocket(m_socket);
			SendDataBuf.Create(midLength);//��ʼ��Ҫ���͵�����
			SendDataBuf.AddHeader(1, m_category, mServiceKey, midLength);
			SendDataBuf.EnSendBuf(&sBodyLength, (unsigned char *)(newsendsocketdata.getDes()), SendDataBuf.GetPacketHead().bodylen);
			pCG2Session->Send(SendDataBuf.GetSendBuf(),sBodyLength);
			newsendsocketdata.Destroy();
			SendDataBuf.Destory();
		}
	}
	catch(...)
	{
	}
}
bool COperatorCG2::SendEmail(char * lpRecv,char *lpContent)
{
	try
	{
		
		char strHeader[256];
		char strDate[256];
		ZeroMemory(strHeader,256);
		ZeroMemory(strDate,256);
		ReadValue("Mail_Server",retMessage[0],256);//Mail_Server=127.0.0.1
		ReadValue("Mail_Port",retMessage[1],256);//Mail_Port=25
		ReadValue("CG2_Sender",retMessage[2],256);//CG2_Sender=zhubin8265@hotmail.com
		ReadValue("CG2_XMailer",retMessage[3],256);//CG2_XMailer=zb
		ReadValue("CG2_Subject",retMessage[4],256);//CG2_Subject=���͵�������
		SYSTEMTIME nowtime;
		GetLocalTime(&nowtime);//��ȡϵͳ��ǰʱ��
		sprintf(strDate,"%d-%d-%d %d:%d:%d",nowtime.wYear,nowtime.wMonth,nowtime.wDay,nowtime.wHour,nowtime.wMinute,nowtime.wMinute);
		sprintf(strHeader,
			"Date: %s\r\n"\
			"From: %s\r\n"\
			"To: %s\r\n"\
			"Subject: %s\r\n"\
			"X-Mailer: <%s>\r\n\r\n",
			strDate,
			retMessage[2],lpRecv,
			retMessage[4],
			retMessage[3]);//���ݷ���ͷ

		CLogic::SendMail(retMessage[0],StrToInt(retMessage[1]),lpRecv,retMessage[2],strHeader,retMessage[4],lpContent);
	/*	CMSocket m_Socket;
		char sendbuf[2048];
		char recvbuf[256];
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		int iresult=m_Socket.ConnectURL("127.0.0.1",25);
		m_Socket.MyReceive((unsigned char *)recvbuf);
		char local_host[80];	
		gethostname( local_host, 80 );
		
		sprintf(sendbuf,"HELO %s\r\n",local_host);
		
		int sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		sprintf(sendbuf,"MAIL From: <%s>\r\n","zhubin8265@hotmail.com");//�ʼ�������˭
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		sprintf(sendbuf,"RCPT TO: <%s>\r\n","zhubin8265@hotmail.com");//�ʼ�����˭
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		// Send the DATA command
		sprintf(sendbuf,"DATA\r\n");//����
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		char Date[256];
		ZeroMemory(Date,256);
		sprintf(Date,"%s","Wed, 11 Mar 09 15:17:14 �й���׼ʱ��");
		// Send the DATA command
		
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		
		
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		// Send the DATA command
		sprintf(sendbuf,"DATA\r\n");//���ݷ�������
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		// Send the DATA command
		sprintf(sendbuf,"\r\n.\r\n");//���ݽ�β
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		m_Socket.MyDestory();*/
	}
	catch (...)
	{
		
	}
	return true;
}