#include "stdafx.h"
#include ".\socketdata.h"
#include ".\Des.h"

CSocketData::CSocketData(void)
: lpDatabuf(NULL)
{

}

CSocketData::~CSocketData(void)
{
	Destory();
}

//初始化数据包
bool CSocketData::Create(int length)
{
	lptmpsendbuf=new unsigned char[(length+20)*2];//加密前
	lpsendbuf=new unsigned char[(((length+20)*2+7)/8)*8];	//加密后
	lpDatabuf=new unsigned char[length*2];
	ZeroMemory(lptmpsendbuf,(length+20)*2);
	ZeroMemory(lpsendbuf,(((length+20)*2+7)/8)*8);
	ZeroMemory(lpDatabuf,length*2);
	return true;
}



//得到当前时间作数据包头信息
bool CSocketData::GetNowTime(void)
{
	SYSTEMTIME str_systemtime;
	ZeroMemory(&str_systemtime,sizeof(str_systemtime));
	GetLocalTime(&str_systemtime);
	m_packethead.SystemData[0]=str_systemtime.wYear-1900;
	m_packethead.SystemData[1]=str_systemtime.wMonth;
	m_packethead.SystemData[2]=str_systemtime.wDay;
	m_packethead.SystemData[3]=str_systemtime.wHour;
	m_packethead.SystemData[4]=str_systemtime.wMinute;
	m_packethead.SystemData[5]=str_systemtime.wSecond;
	return true;
}

//构造数据包的头部
bool CSocketData::AddHeader(unsigned int nid,CEnumCore::Msg_Category m_msg_category,CEnumCore::ServiceKey m_servicekey,unsigned int bodyLen)
{
	m_packethead.uId=nid;
	m_packethead.m_msg_category=m_msg_category;
	m_packethead.m_servicekey=m_servicekey;
	GetNowTime();
	m_packethead.bodylen=bodyLen;
	return true;
}

//添加数据包的头部
int CSocketData::AddPacketHead(unsigned char * lpDes, CGlobalStruct::PACKETHEAD m_packethead, unsigned char* lpSou)
{
	int length=0;
	memcpy(lpDes,(unsigned int*)&m_packethead.uId,4);
	memcpy(lpDes+4,(short int*)&m_packethead.m_msg_category,2);
	memcpy(lpDes+6,(short int*)&m_packethead.m_servicekey,2);

	memcpy(lpDes+8,m_packethead.SystemData,6);

	memcpy(lpDes+14,(short int*)&m_packethead.bodylen,4);

	memcpy(lpDes+18,lpSou,m_packethead.bodylen);
	length=m_packethead.bodylen+18;
	return length;
}

//替换数据包中的0xfe、0xef和0xfd
int CSocketData::EncodeMessage(unsigned char* lpDes, int deslength,unsigned char * lpSou)
{
	unsigned char headendflag;
	headendflag=0xFE;
	unsigned char addflag;
	memcpy(lpDes,(char*)&headendflag,1);
	int nowlength=1;
	///////////////////////////////////////////////////
	for (int i = 0; i < deslength; i++)
	{
		switch (*(lpSou+i))
		{

		case 0xFE:
			addflag=0xFD;
			memcpy(lpDes+nowlength,(char*)&addflag,1);
			nowlength=nowlength+1;

			addflag=0x01;
			memcpy(lpDes+nowlength,(char*)&addflag,1);
			nowlength=nowlength+1;

			break;
		case 0xEF:
			addflag=0xFD;
			memcpy(lpDes+nowlength,(char*)&addflag,1);
			nowlength=nowlength+1;

			addflag=0xF2;
			memcpy(lpDes+nowlength,(char*)&addflag,1);
			nowlength=nowlength+1;
			break;
		case 0xFD:
			addflag=0xFD;
			memcpy(lpDes+nowlength,(char*)&addflag,1);
			nowlength=nowlength+1;

			addflag=0x00;
			memcpy(lpDes+nowlength,(char*)&addflag,1);
			nowlength=nowlength+1;
			break;
		default:
			memcpy(lpDes+nowlength,lpSou+i,1);
			nowlength=nowlength+1;
			break;
		}
	}

	/////////////////////////////////
	headendflag=0xEF;
	memcpy(lpDes+nowlength,(char*)&headendflag,1);
	///////////////////////////////////////////////////////
	nowlength=nowlength+1;
	return nowlength;
}

//构造最终发送的数据
unsigned char * CSocketData::EnSendBuf(int* alllength, unsigned char * pbodybuf,int bodylen)
{	
	int count=0;
	while(CSocketData::isfileupdate==false)
	{
        count++;
		Sleep(50);
		if(count>20*50)
		{
			CSocketData::isfileupdate=true;
			count=0;
		}
		//	printf("havevvvvvvvvv\n");
	}
	int actlength=0;

	CDES CDes;
	unsigned char *lptmpbuf=new unsigned char[bodylen+18];
	ZeroMemory(lptmpbuf,bodylen+18);

	actlength=AddPacketHead(lptmpbuf,m_packethead,pbodybuf);//加头后大小

	actlength=((actlength+7)/8)*8;

	CDes.Des_Go((char *)lptmpsendbuf,(char *)lptmpbuf,actlength,"*12*23*2",8,ENCRYPT);

	CCrc m_CRC(32, 0xA42F3D65, 0, 0xFFFFFFFF);  
	DWORD crcNum = m_CRC.Calc(lptmpsendbuf, actlength);
	memcpy(lptmpsendbuf + actlength, (DWORD*)&crcNum, 8);
	actlength = actlength + 8;

	actlength=EncodeMessage(lpsendbuf,actlength,lptmpsendbuf);
	
	*alllength=actlength;
	if(lptmpbuf!=NULL)
	{
		delete[] lptmpbuf;
		lptmpbuf=NULL;
	}

	return lpsendbuf;
}

//构造最终发送的数据
unsigned char * CSocketData::EnSendBuf(int* alllength, unsigned char * pbodybuf,int bodylen,bool isupfile)
{	
	
	int filecount=0;
	while(CSocketData::isfileupdate==false)
	{   
		filecount++;
		Sleep(50);
		if(filecount>20*90)
		{
			CSocketData::isfileupdate=true;
			filecount=0;
		}
		
		//	printf("havevvvvvvvvv\n");
	}
	int actlength=0;
	
	CDES CDes;
	unsigned char *lptmpbuf=new unsigned char[bodylen+18];
	ZeroMemory(lptmpbuf,bodylen+18);
	//Sleep(100);
	actlength=AddPacketHead(lptmpbuf,m_packethead,pbodybuf);//加头后大小
	//Sleep(100);
	
	actlength=((actlength+7)/8)*8;
	//printf("have first\n");
	CSocketData::isfileupdate=false;
	Sleep(500);
	CDes.Des_Go((char *)lptmpsendbuf,(char *)lptmpbuf,actlength,"*12*23*2",8,ENCRYPT);
	Sleep(100);
	CSocketData::isfileupdate=true;
	//printf("have end\n");
    
	CCrc m_CRC(32, 0xA42F3D65, 0, 0xFFFFFFFF);  
	DWORD crcNum = m_CRC.Calc(lptmpsendbuf, actlength);
	memcpy(lptmpsendbuf + actlength, (DWORD*)&crcNum, 8);
	actlength = actlength + 8;
	
	actlength=EncodeMessage(lpsendbuf,actlength,lptmpsendbuf,true);
	//	Sleep(100);
	*alllength=actlength;
	if(lptmpbuf!=NULL)
	{
		delete[] lptmpbuf;
		lptmpbuf=NULL;
	}
	
	return lpsendbuf;
}
//替换数据包中的0xfe、0xef和0xfd
int CSocketData::EncodeMessage(unsigned char* lpDes, int deslength,unsigned char * lpSou,bool isupfile)
{
	unsigned char headendflag;
	headendflag=0xFE;
	unsigned char addflag;
	memcpy(lpDes,(char*)&headendflag,1);
	int nowlength=1;
	///////////////////////////////////////////////////
	//printf("length%d\n",deslength);
	for (int i = 0; i < deslength; i++)
	{
		if(i%200==0)
		{
			//	 Sleep(1);
		}
		
		switch (*(lpSou+i))
		{
			
		case 0xFE:
			addflag=0xFD;
			memcpy(lpDes+nowlength,(char*)&addflag,1);
			nowlength=nowlength+1;
			
			addflag=0x01;
			memcpy(lpDes+nowlength,(char*)&addflag,1);
			nowlength=nowlength+1;
			
			break;
		case 0xEF:
			addflag=0xFD;
			memcpy(lpDes+nowlength,(char*)&addflag,1);
			nowlength=nowlength+1;
			
			addflag=0xF2;
			memcpy(lpDes+nowlength,(char*)&addflag,1);
			nowlength=nowlength+1;
			break;
		case 0xFD:
			addflag=0xFD;
			memcpy(lpDes+nowlength,(char*)&addflag,1);
			nowlength=nowlength+1;
			
			addflag=0x00;
			memcpy(lpDes+nowlength,(char*)&addflag,1);
			nowlength=nowlength+1;
			break;
		default:
			memcpy(lpDes+nowlength,lpSou+i,1);
			nowlength=nowlength+1;
			break;
		}
	}
	//printf("length%d\n",deslength);
	/////////////////////////////////
	headendflag=0xEF;
	memcpy(lpDes+nowlength,(char*)&headendflag,1);
	///////////////////////////////////////////////////////
	nowlength=nowlength+1;
	return nowlength;
}
//还原数据包中的0xfe、0xef和0xfd
int CSocketData::DecodeMessage(unsigned char* lpDes, int soulength,unsigned char * lpSou)
{
	int head = 0, tail = 0;
	int nowlength=0;
	unsigned char addflag;


	for (; head < soulength; head++)
		if (*(lpSou+head)== 0xFE)
			break;
	for (tail = head; tail < soulength; tail++)
		if (*(lpSou+tail) == 0xEF)
			break;
	if (tail >= soulength)
		return false;

	for (int i = head + 1; i < tail; i++)
	{
		if (*(lpSou+i) == 0xFD)
		{
			i=i+1;
			switch (*(lpSou+i))
			{
			case 0x01:
				addflag=0xFE;
				memcpy(lpDes+nowlength,(char*)&addflag,1);
				nowlength=nowlength+1;

				break;
			case 0x00:
				addflag=0xFD;
				memcpy(lpDes+nowlength,(char*)&addflag,1);
				nowlength=nowlength+1;
				break;
			case 0xF2:
				addflag=0xEF;
				memcpy(lpDes+nowlength,(char*)&addflag,1);
				nowlength=nowlength+1;
				break;
			default:
				return false;
			}
		}
		else
		{
			memcpy(lpDes+nowlength,lpSou+i,1);
			nowlength=nowlength+1;
		}
	}
	return nowlength;
}

//解析得到数据包的头部
int CSocketData::DePacketHead(unsigned char* lpSou)
{

	unsigned char tmpuid[4];
	unsigned char tmpm_msg_category[2];
	unsigned char tmpm_servicekey[2];
	unsigned char tmpbodylen[4];

	ZeroMemory(tmpuid,4);
	ZeroMemory(tmpm_msg_category,2);
	ZeroMemory(tmpm_servicekey,2);
	ZeroMemory(tmpbodylen,4);


	memcpy(&tmpuid,lpSou,4);
	m_packethead.uId=*(unsigned int *)&tmpuid;
	memcpy(&tmpm_msg_category,lpSou+4,2);
	m_packethead.m_msg_category=*(short int *)&tmpm_msg_category;
	memcpy(&tmpm_servicekey,lpSou+4+2,2);
	m_packethead.m_servicekey=*(short int *)&tmpm_servicekey;
	memcpy(m_packethead.SystemData,lpSou+4+2+2,6);
	memcpy(&tmpbodylen,lpSou+4+2+2+6,4);
	m_packethead.bodylen=*(int *)&tmpbodylen;

	return m_packethead.bodylen;
}

//解析得到数据包的Message_Tag_ID
bool CSocketData::GetMessageID(void)
{
	unsigned int uiID;
	unsigned int bCategory = (unsigned short int)m_packethead.m_msg_category;

	unsigned int bServicekey = (unsigned short int )m_packethead.m_servicekey;
	uiID =(unsigned int)(bCategory<<16) + (unsigned int)bServicekey;
	m_message_tag_id=uiID;

	return false;
}

//解析数据包
bool CSocketData::DeHead(unsigned char * lpbuf,int length)
{
	int count=0;
	while(CSocketData::isfileupdate==false)
	{
		count++;
		Sleep(50);
		if(count>20*90)
		{
			count=0;
			CSocketData::isfileupdate=true;
		}
	}
	CDES CDes;
	int nowlength=0;
	nowlength=DecodeMessage(lptmpsendbuf,length,lpbuf);//收到的整个长度

	if(nowlength <= 22)
	{
		return false;
	}

	CCrc m_CRC(32, 0xA42F3D65, 0, 0xFFFFFFFF);
	unsigned char *lptmpbuf = new unsigned char[(length+20)*2];//加密前
	ZeroMemory(lptmpbuf, (length+20)*2);
	memcpy(lptmpbuf, lptmpsendbuf, nowlength - 8);
	nowlength = nowlength - 8;
	DWORD m_CRCNum = m_CRC.Calc(lptmpbuf, nowlength);
	DWORD m_SRCNum = *(DWORD*)(lptmpsendbuf + nowlength);
	if(m_CRCNum != m_SRCNum)
	{
		//		return false;// cancel for testing, memory leak potentially 
		goto FUN_END;
	}

	if(!CDes.Des_Go((char *)lpDatabuf ,(char *)lptmpbuf ,nowlength ,"*12*23*2",8,DECRYPT))
	{
		//		return false;// cancel for testing, memory leak potentially 
		goto FUN_END;
	}
	DePacketHead(lpDatabuf);
	GetMessageID();
	if(lptmpbuf != NULL)
	{
		delete[] lptmpbuf;
		lptmpbuf=NULL;
	}
	return true;
FUN_END: // add for testing 
	return FALSE;
}



//获取数据包的头部
CGlobalStruct::PACKETHEAD CSocketData::GetPacketHead()
{
	return m_packethead;
}

//获取数据包的Message_Tag_ID
CEnumCore::Message_Tag_ID CSocketData::GetMessageTagID()
{
	return m_message_tag_id;
}

//获取发送的数据
unsigned char * CSocketData::GetSendBuf()
{
	return lpsendbuf;
}

//获取接收的数据
unsigned char * CSocketData::GetDataBuf()
{
	return lpDatabuf;
}

//释放数据包
bool CSocketData::Destory(void)
{
	if(lptmpsendbuf!=NULL)
	{
		delete[] lptmpsendbuf;
		lptmpsendbuf=NULL;
	}
	if(lpsendbuf!=NULL)
	{
		delete[] lpsendbuf;
		lpsendbuf=NULL;
	}
	if(lpDatabuf!=NULL)
	{
		delete[] lpDatabuf;
		lpDatabuf=NULL;
	}
	return true;
}
bool CSocketData::isfileupdate=true;
void CSocketData::setfileupdatestate(bool trueorfalse)
{
	CSocketData::isfileupdate=trueorfalse;
}
bool CSocketData::getfileupdatestate()
{
	return CSocketData::isfileupdate;
}