#ifndef MYEVENT_H_
#define MYEVENT_H_

#include "..\\SqlHelper\\SqlHelper.h"
#pragma comment(lib,"..\\SqlHelper\\debug\\SqlHelper.lib")


class __declspec(dllexport) CMyEvent
{
public:
	 CMyEvent();
	 ~CMyEvent();

	 //��ȡTagName
	 CEnumCore::TagName GetTagName(char* fieldname);

	 //��ȡTagFormat
	 CEnumCore::TagFormat GetTagFormat(char* m_TagFormat);

};

#endif