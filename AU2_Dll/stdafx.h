// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>
#include "LogFile.h"

#include <string>
using namespace std;

#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif

#include "DSNShellInc/DSNShell.h"
// TODO: �ڴ˴����ó���Ҫ��ĸ���ͷ�ļ�
