#ifndef CFInfo_H_
#define CFInfo_H_

#include "OperatorCF.h"

#include "..\\GameIOCP\\GameInfo.h"


class  CCFInfo:public CGameInfo
{
public:
	CCFInfo(void);
	~CCFInfo(void);
public:
	//连接远程服务端
	vector <CGlobalStruct::TFLV> initialize(CSession * pSess,unsigned char * pbuf, int length);

};
#endif




















