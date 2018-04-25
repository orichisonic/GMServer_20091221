#pragma once
#include "OperInfo.h"

//发送公告信息
extern "C"  bool __declspec(dllexport) BulletinTool(char *IP, int port, char *content);

//踢人
extern "C"  bool __declspec(dllexport) KickTool(char *IP, int port, char *userId);
