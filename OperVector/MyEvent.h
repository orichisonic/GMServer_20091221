#ifndef MYEVENT_H_
#define MYEVENT_H_

#include "..\\SqlHelper\\SqlHelper.h"
#pragma comment(lib,"..\\SqlHelper\\debug\\SqlHelper.lib")


class __declspec(dllexport) CMyEvent
{
public:
	 CMyEvent();
	 ~CMyEvent();

	 //获取TagName
	 CEnumCore::TagName GetTagName(char* fieldname);

	 //获取TagFormat
	 CEnumCore::TagFormat GetTagFormat(char* m_TagFormat);

};

#endif