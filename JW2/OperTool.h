#pragma once
#include "OperInfo.h"

//���͹�����Ϣ
extern "C"  bool __declspec(dllexport) BulletinTool(char *IP, int port, char *content);

//����
extern "C"  bool __declspec(dllexport) KickTool(char *IP, int port, char *userId);
