#include "stdafx.h"
#include ".\operatorcg2.h"
#include "time.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//构造函数
COperatorCG2::COperatorCG2(void)
{
///	ZeroMemory(connLocalDB, 20);//初始化服务器IP
//	ZeroMemory(petname, 128);//初始化宠物名称
//	ZeroMemory(m_sql, 256);//初始化本地查询的sql语句
//	sprintf(connLocalDB,"127.0.0.1");
}

//析构函数
COperatorCG2::~COperatorCG2(void)
{
	SendVectBuf.Destroy();
}

//初始化发送数据的类
bool COperatorCG2::initialize(CSession * pSess,SOCKET s_Socket)
{
	ZeroMemory(connLocalDB, 20);//初始化服务器IP
	char path[256];
	GetCurrentDirectory(256,path);
	char filepath[256];
	sprintf(filepath,"%s%s",path,"\\config\\scheme.ini");//构造配置文件的路径
	GetPrivateProfileString("DATABASE","Address","127.0.0.1",connLocalDB,20,filepath);//获取本地的数据库地址
	
	ZeroMemory(petname, 128);//初始化宠物名称
	mSocket.SetSocket(s_Socket);//设置SOCKET
	ZeroMemory(m_sql, 256);//初始化本地查询的sql语句

	pCG2Session=(CCG2Session *)pSess;
	return true;
}
bool COperatorCG2::initialize()
{
	ZeroMemory(connLocalDB, 20);//初始化服务器IP
	char path[256];
	GetCurrentDirectory(256,path);
	char filepath[256];
	sprintf(filepath,"%s%s",path,"\\config\\scheme.ini");//构造配置文件的路径
	GetPrivateProfileString("DATABASE","Address","127.0.0.1",connLocalDB,20,filepath);//获取本地的数据库地址
	ZeroMemory(m_sql, 256);//初始化本地查询的sql语句

	return true;
}
bool COperatorCG2::ReadValue(char * strKey,char * strValue,int strLength)
{
	return logFile.ReadValue("M_CG2","CG2",strKey,strValue,strLength);
}


//返回单个信息
void COperatorCG2::ReturnMessage(unsigned int nid, CEnumCore::Msg_Category m_category, CEnumCore::ServiceKey mServiceKey, char* strMessage)
{
	try
	{
		SendVectBuf.InitCreate(1,2);
		
		//添加TFLV到数据包中
		SendVectBuf.AddTFLV(CEnumCore::TagName::MESSAGE,CEnumCore::TagFormat::TLV_STRING,strlen(strMessage),(unsigned char*)strMessage);
		
		//解析数据包并获取长度
		int vectLength = SendVectBuf.EnBody();
		int sBodyLength = 0;
		
		CSocketData SendDataBuf;
		SendDataBuf.Create(vectLength);//构造发送数据包
		SendDataBuf.AddHeader(nid, m_category, mServiceKey, vectLength);//添加数据包的头部
		SendDataBuf.EnSendBuf(&sBodyLength, SendVectBuf.getDes(), vectLength);//加密并构造发送数据包
		
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
			ReturnMessage(1,m_category, mServiceKey, "查询数据库结果为空");
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
//获取SqlExpress中的相应sql语句
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
		m_SqlHelper.SqlHelperMain("CG2",&datarecord,"127.0.0.1", m_sql, 0, 0, 0);//获取SqlExpress中的相应sql语句
		
		if(datarecord.empty())//如果记录为空
		{
			sprintf(lpSql,"");
			return;
		}
		
		//获取查询的记录集的每一项
		vector<CGlobalStruct::DataRecord>::iterator iter;
		CGlobalStruct::DataRecord  m_DataRecord;	
		for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
		{
			m_DataRecord = *iter;
			if(!strcmp("sql_statement", m_DataRecord.recordfieldname))//获取相应的LogDBIP
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
//返回数据
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

		char m_result[256];//保存每一项的结果
		ZeroMemory(m_result,256);//初始化每一项的结果
		char procName[256];
		SendVectBuf.InitCreate(m_OpVect->getQueryNum()+1, m_OpVect->getColTFLVNum()+5);//初始化发送的数据
		for(int i=0; i < m_OpVect->getAllTFLV(); i++)
		{
			CGlobalStruct::TFLV m_tflv = m_OpVect->getTLVBynIndex(i+1);//获取每一个TFLV

			if(m_tflv.m_tagName == CEnumCore::TagName::CG2_JobID)//职业ID
			{
				try
				{
					//获取中文类型的说明
					ZeroMemory(m_result,256);
					
					sprintf(procName,"exec CG2_JobName_query %i",StrToInt((char *)&m_tflv.lpdata));//初始化存储过程的语句
					CG2_Proc_Query(connLocalDB,4,procName,m_result);
					if(strlen(m_result)>0)
					{
						SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_JobID, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
					else
					{
						SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV	
					}
				}
				catch(...)
				{
				}
			}
			else if (m_tflv.m_tagName == CEnumCore::TagName::CG2_MapID)//地图ID
			{
				try
				{
						//获取中文类型的说明
					ZeroMemory(m_result,256);
						
					sprintf(procName,"exec CG2_MapName_query %i",StrToInt((char *)&m_tflv.lpdata));//初始化存储过程的语句
					CG2_Proc_Query(connLocalDB,4,procName,m_result);
					if(strlen(m_result)>0)
					{
						SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_MapID, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
					else
					{
						SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
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
					//获取中文类型的说明
					ZeroMemory(m_result,256);
					
					sprintf(procName,"exec CG2_SkillName_query %i",StrToInt((char *)&m_tflv.lpdata));//初始化存储过程的语句
					CG2_Proc_Query(connLocalDB,4,procName,m_result);
					if(strlen(m_result)>0)
					{
						SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_SkillName, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
					else
					{
						SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
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
					//获取中文类型的说明
					ZeroMemory(m_result,256);
					sprintf(procName,"exec CG2_FameName_query -1,%i",StrToInt((char *)&m_tflv.lpdata));//初始化存储过程的语句
					CG2_Proc_Query(connLocalDB,4,procName,m_result);
					if(strlen(m_result)>0)
					{
						SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_ProHonorRank, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
					else
					{
						SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
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
					//获取中文类型的说明
					ZeroMemory(m_result,256);
					
					sprintf(procName,"exec CG2_ItemName_query %i",StrToInt((char *)&m_tflv.lpdata));//初始化存储过程的语句
					CG2_Proc_Query(connLocalDB,4,procName,m_result);
					if(strlen(m_result)>0)
					{
						SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_ItemName, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
					}
					else
					{
						SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
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
				char *lpParstr=strtok((char *)&m_tflv.lpdata,",");//查询标志","
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
					// 忠誠度算出
					rLoyalty = ( loyaltyBase + ( loyaltyPetRate / 2 ) ) / loyaltyPetRate;
				}
				if( loyalty < 0 || loyalty >= 20*100 )
				{
					// ペットのなつき度加える
					rLoyalty += ( loyalty / 100 );
				}
				// テイム無しの最大値チェック
				if( rLoyalty > 60 )
				{
					rLoyalty = 60;
				}
				
				// 最低０
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
					SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
				}
				catch(...)
				{
				}
					
			}
			
		}

		int midLength = SendVectBuf.EnBody();
		int sBodyLength = 0;
		CSocketData SendDataBuf;
		SendDataBuf.Create(midLength);//初始化要发送的数据
		SendDataBuf.AddHeader(nid, m_category, mServiceKey, midLength);
		SendDataBuf.EnSendBuf(&sBodyLength, (unsigned char *)SendVectBuf.getDes(), SendDataBuf.GetPacketHead().bodylen);
		pCG2Session->Send(SendDataBuf.GetSendBuf(),sBodyLength);

		SendDataBuf.Destory();
	}
	catch(...)
	{
	}
}



//返回数据（并数据）
void COperatorCG2::ReturnData(unsigned int nid,CEnumCore::Msg_Category m_category,CEnumCore::ServiceKey mServiceKey, COperVector* m_OpVect, COperVector* m_OpVect1,int iIndex,int iPageSize)
{
	try
	{
		int alltflv=m_OpVect->getAllTFLV()+m_OpVect1->getAllTFLV();//所有TFLV
		char strInt[10];
		if(alltflv==0)
		{
			ReturnMessage(nid, m_category, mServiceKey, "查询数据库结果为空");
		}
		
		
		int fieldnum=m_OpVect->getColTFLVNum();//列数
		//条数
		int allquery=0;
		if(alltflv%fieldnum==0)
		{
			allquery=alltflv/fieldnum;
		}
		else
		{
			allquery=alltflv/fieldnum+1;
		}
		
		//求出页数
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
		char m_result[256];//保存每一项的结果
		ZeroMemory(m_result,256);//初始化每一项的结果
		char procName[256];
		SendVectBuf.InitCreate(alltflv/fieldnum+1, fieldnum+5);//初始化发送的数据
		for(i=0; i < m_OpVect->getAllTFLV(); i++)
		{
			CGlobalStruct::TFLV m_tflv = m_OpVect->getTLVBynIndex(i+1);//获取每一个TFLV
			if(firsttflv==0)
			{
					if(allreturntflv==0)
					{
						break;
					}
					else
					{
						if(m_tflv.m_tagName == CEnumCore::TagName::CG2_JobID)//职业ID
						{
							try
							{
								//获取中文类型的说明
								ZeroMemory(m_result,256);
								
								sprintf(procName,"exec CG2_JobName_query %i",StrToInt((char *)&m_tflv.lpdata));//初始化存储过程的语句
								CG2_Proc_Query(connLocalDB,4,procName,m_result);
								if(strlen(m_result)>0)
								{
									SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_JobID, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
								}
								else
								{
									SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
								}
							}
							catch(...)
							{
							}
						}
						else if (m_tflv.m_tagName == CEnumCore::TagName::CG2_MapID)//地图ID
						{
							try
							{
								//获取中文类型的说明
								ZeroMemory(m_result,256);
								
								sprintf(procName,"exec CG2_MapName_query %i",StrToInt((char *)&m_tflv.lpdata));//初始化存储过程的语句
								CG2_Proc_Query(connLocalDB,4,procName,m_result);
								if(strlen(m_result)>0)
								{
									SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_MapID, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
								}	
								else
								{
									SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
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
								//获取中文类型的说明
								ZeroMemory(m_result,256);
								
								sprintf(procName,"exec CG2_SkillName_query %i",StrToInt((char *)&m_tflv.lpdata));//初始化存储过程的语句
								CG2_Proc_Query(connLocalDB,4,procName,m_result);
								if(strlen(m_result)>0)
								{
									SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_SkillName, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
								}
								else
								{
									SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
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
								//获取中文类型的说明
								ZeroMemory(m_result,256);
								
								sprintf(procName,"exec CG2_FameName_query -1,%i",StrToInt((char *)&m_tflv.lpdata));//初始化存储过程的语句
								CG2_Proc_Query(connLocalDB,4,procName,m_result);
								if(strlen(m_result)>0)
								{
									SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_ProHonorRank, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
								}
								else
								{
									SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
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
								//获取中文类型的说明
								ZeroMemory(m_result,256);
								
								sprintf(procName,"exec CG2_ItemName_query %i",StrToInt((char *)&m_tflv.lpdata));//初始化存储过程的语句
								CG2_Proc_Query(connLocalDB,4,procName,m_result);
								if(strlen(m_result)>0)
								{
									SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_ItemName, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
								}	
								else
								{
									SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
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
							SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
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
			CGlobalStruct::TFLV m_tflv = m_OpVect1->getTLVBynIndex(i+1);//获取每一个TFLV
			if(firsttflv==0)
			{
					if(allreturntflv==0)
					{
						break;
					}
					else
					{
						if(m_tflv.m_tagName == CEnumCore::TagName::CG2_JobID)//职业ID
						{
							try
							{
								//获取中文类型的说明
								ZeroMemory(m_result,256);
								
								sprintf(procName,"exec CG2_JobName_query %i",StrToInt((char *)&m_tflv.lpdata));//初始化存储过程的语句
								CG2_Proc_Query(connLocalDB,4,procName,m_result);
								if(strlen(m_result)>0)
								{
									SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_JobID, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
								}	
								else
								{
									SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
								}
							}
							catch(...)
							{
							}
						}
						else if (m_tflv.m_tagName == CEnumCore::TagName::CG2_MapID)//地图ID
						{
							try
							{
								//获取中文类型的说明
								ZeroMemory(m_result,256);
								
								sprintf(procName,"exec CG2_MapName_query %i",StrToInt((char *)&m_tflv.lpdata));//初始化存储过程的语句
								CG2_Proc_Query(connLocalDB,4,procName,m_result);
								if(strlen(m_result)>0)
								{
									SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_MapID, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
								}	
								else
								{
									SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
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
								//获取中文类型的说明
								ZeroMemory(m_result,256);
								
								sprintf(procName,"exec CG2_SkillName_query %i",StrToInt((char *)&m_tflv.lpdata));//初始化存储过程的语句
								CG2_Proc_Query(connLocalDB,4,procName,m_result);
								if(strlen(m_result)>0)
								{
									SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_SkillName, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
								}
								else
								{
									SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
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
								//获取中文类型的说明
								ZeroMemory(m_result,256);
								
								sprintf(procName,"exec CG2_FameName_query -1,%i",StrToInt((char *)&m_tflv.lpdata));//初始化存储过程的语句
								CG2_Proc_Query(connLocalDB,4,procName,m_result);
								if(strlen(m_result)>0)
								{
									SendVectBuf.AddTFLV(CEnumCore::TagName::CG2_SkillName, CEnumCore::TagFormat::TLV_STRING, strlen(m_result), (unsigned char*)&m_result);
								}
								else
								{
									SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
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
							SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
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
		SendDataBuf.Create(midLength);//初始化要发送的数据
		SendDataBuf.AddHeader(nid, m_category, mServiceKey, midLength);
		SendDataBuf.EnSendBuf(&sBodyLength, (unsigned char *)SendVectBuf.getDes(), SendDataBuf.GetPacketHead().bodylen);
		pCG2Session->Send(SendDataBuf.GetSendBuf(),sBodyLength);
			
		SendDataBuf.Destory();
	}
	catch(...)
	{
	}
}
//获取数据库ID
int COperatorCG2::GetDBID(char * remoteip,char* username)
{
	int result = -1;
	char m_RemoteSql[1024];//保存远程数据库操作的sql语句
	char m_sql[1024];
	getRemoteSql("CG2_DB_Query",m_RemoteSql);//获取远程数据库操作的sql语句
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
	m_SqlHelper.SqlHelperMain("CG2",&datarecord, remoteip, m_sql, 3, 1, 1);//获取对应的LogDBIP

	if(datarecord.empty())//如果记录为空
	{
		return -1;
	}
	
	//获取查询的记录集的每一项
	vector<CGlobalStruct::DataRecord>::iterator iter;
	CGlobalStruct::DataRecord  m_DataRecord;	
	for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
	{
		m_DataRecord = *iter;
		if(!strcmp("dbid", m_DataRecord.recordfieldname))//获取相应的记录数
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



//获得指定语句第一个字段值
void COperatorCG2::CG2_Proc_Query(char * lpIP,int flag,char* procName,char* result)
{
	
	vector<CGlobalStruct::DataRecord> datarecord;
	if(!datarecord.empty())
	{
		datarecord.clear();
	}
	m_SqlHelper.SqlHelperMain("CG2",&datarecord, lpIP, procName, flag, 0, 0);//获取对应的LogDBIP
	
	if(datarecord.empty())//如果记录为空
	{
		result="";
	}
	
	//获取查询的记录集的每一项
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

//通过DBID查IP
void COperatorCG2::FindDBIP(char * lpIP,char * lpNewIP,int dbid)
{
	char szSql[256];
	vector<CGlobalStruct::DataRecord> datarecord;
	if(!datarecord.empty())
	{
		datarecord.clear();
	}
	sprintf(szSql,"exec CG2_FindDBIP '%s',%i",lpIP,dbid);
	m_SqlHelper.SqlHelperMain("CG2",&datarecord,connLocalDB , szSql, 4, 0, 0);//获取对应的LogDBIP
	
	if(datarecord.empty())//如果记录为空
	{
		lpNewIP="";
	}
	
	//获取查询的记录集的每一项
	vector<CGlobalStruct::DataRecord>::iterator iter;
	CGlobalStruct::DataRecord  m_DataRecord;	
	for( iter = datarecord.begin(); iter != datarecord.end(); iter++ )
	{
		m_DataRecord = *iter;
		if(!strcmp("serverip", m_DataRecord.recordfieldname))//获取相应的记录数
		{
			sprintf(lpNewIP,"%s",m_DataRecord.recorddata);
		}
	}
	if(!datarecord.empty())
	{
		datarecord.clear();
	}
}


//返回特殊数据（角色称号）
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

			char m_result[256];//保存每一项的结果
			ZeroMemory(m_result,256);//初始化每一项的结果
			SendVectBuf.InitCreate(m_OpVect->getQueryNum()+1, m_OpVect->getColTFLVNum()+5);//初始化发送的数据
			for(int i=0; i < m_OpVect->getAllTFLV(); i++)
			{
				CGlobalStruct::TFLV m_tflv = m_OpVect->getTLVBynIndex(i+1);//获取每一个TFLV
			
				try
				{
					SendVectBuf.AddTFLV(m_tflv);//默认为直接添加TFLV
				}
				catch(...)
				{
				}
			
			
		}

		int midLength = SendVectBuf.EnBody();
		int sBodyLength = 0;
		CSocketData SendDataBuf;
		SendDataBuf.Create(midLength);//初始化要发送的数据
		SendDataBuf.AddHeader(nid, m_category, mServiceKey, midLength);
		SendDataBuf.EnSendBuf(&sBodyLength, (unsigned char *)SendVectBuf.getDes(), SendDataBuf.GetPacketHead().bodylen);
		pCG2Session->Send(SendDataBuf.GetSendBuf(),sBodyLength);
		SendDataBuf.Destory();
	}
	catch(...)
	{
	}
}


///访问网络接口///////////////////////////
bool COperatorCG2::ReadXml(char * strFlag,char *strUrl,char * strpostData,vector<CGlobalStruct::DataRecord> * pVect,int* iCow,int *iCol)
{
	
	bool r_flag=false;
	try
	{
		CoInitialize(NULL);
		IXMLHTTPRequestPtr xmlrequest; //定义变量
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
		IXMLHTTPRequestPtr xmlrequest; //定义变量
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
			SendDataBuf.Create(midLength);//初始化要发送的数据
			SendDataBuf.AddHeader(1, m_category, mServiceKey, midLength);
			SendDataBuf.EnSendBuf(&sBodyLength, (unsigned char *)(m_socketdata->getDes()), SendDataBuf.GetPacketHead().bodylen);
			pCG2Session->Send(SendDataBuf.GetSendBuf(),sBodyLength);
			SendDataBuf.Destory();
		}
		else
		{
			////////////获取实际行列/////////////////
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
			tmptflv = m_socketdata->getTLVBynIndex(1);//获取每一个TFLV
			newsendsocketdata.AddTFLV(tmptflv);
			for(i=0; i < m_socketdata->getAllTFLV(); i++)
			{
				
				if(i>(iIndex-1)*iPageSize*colnum&&i<=iIndex*iPageSize*colnum)
				{
					tmptflv = m_socketdata->getTLVBynIndex(i+1);//获取每一个TFLV
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
			SendDataBuf.Create(midLength);//初始化要发送的数据
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
		ReadValue("CG2_Subject",retMessage[4],256);//CG2_Subject=发送道具密码
		SYSTEMTIME nowtime;
		GetLocalTime(&nowtime);//获取系统当前时间
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
			retMessage[3]);//数据发送头

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
		sprintf(sendbuf,"MAIL From: <%s>\r\n","zhubin8265@hotmail.com");//邮件来自于谁
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		sprintf(sendbuf,"RCPT TO: <%s>\r\n","zhubin8265@hotmail.com");//邮件发给谁
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		// Send the DATA command
		sprintf(sendbuf,"DATA\r\n");//数据
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		char Date[256];
		ZeroMemory(Date,256);
		sprintf(Date,"%s","Wed, 11 Mar 09 15:17:14 中国标准时间");
		// Send the DATA command
		
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		
		
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		// Send the DATA command
		sprintf(sendbuf,"DATA\r\n");//数据发送身体
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		
		
		ZeroMemory(sendbuf,2048);
		ZeroMemory(recvbuf,256);
		// Send the DATA command
		sprintf(sendbuf,"\r\n.\r\n");//数据结尾
		sendbytes=m_Socket.SendData(sendbuf,strlen(sendbuf));
		m_Socket.MyReceive((unsigned char *)recvbuf);
		m_Socket.MyDestory();*/
	}
	catch (...)
	{
		
	}
	return true;
}