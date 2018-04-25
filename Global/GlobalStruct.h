#pragma once
#include "EnumCore.h"


class __declspec(dllexport) CGlobalStruct
{
public:
	CGlobalStruct(void);
	~CGlobalStruct(void);
	struct TFLV
	{
		unsigned int nIndex;
		CEnumCore::TagName m_tagName;//2个字节
		CEnumCore::TagFormat m_tagFormat;//2个字节
		unsigned int m_tvlength;//4个字节
		unsigned char lpdata[2048];
		TFLV()
		{
			nIndex=0;
			m_tvlength=0;
			ZeroMemory(lpdata, 2048);
		}

	};
	struct PACKETHEAD
	{
		unsigned int uId;//4个字节
		CEnumCore::Msg_Category m_msg_category;//2个字节
		CEnumCore::ServiceKey m_servicekey;//2个字节
		unsigned char SystemData[6];//6个字节
		unsigned int bodylen;//4个字节
	};
	struct DataRecord
	{
		char recordfieldname[50];
		int recordfieldtype;
//		char recorddata[4000];
		char recorddata[12000];
		DataRecord()
		{
			ZeroMemory(recordfieldname,50);
			recordfieldtype=0;
			ZeroMemory(recorddata,12000);
		}
	};
};