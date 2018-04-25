#pragma once

#include "MyEvent.h"

#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")

#include<vector>
using namespace std;

class __declspec(dllexport) COperVector
{
private:

	//数据包中的数据单元数和Query的行数
	int alltflv,allquery,colTFLV;

	//保存TFLV的vector
	vector<CGlobalStruct::TFLV> InnerList;

	//保存数据库查询数据集
//	vector<CGlobalStruct::DataRecord> m_datarecord;
	
	//发送的数据
	unsigned char *lpDes;

	//执行数据库相关操作
	CSqlHelper m_SqlHelper;

	//检查是否真正列
	bool queryflag;

public:
	
	//构造函数
	COperVector(void);

	//构造数据包的初始化函数
	COperVector(int querynum,int tflvnum);
	COperVector(vector<CGlobalStruct::TFLV> m_innerList);

	//析构函数
	~COperVector(void);



	// 返回要发送的数据
	unsigned char * getDes();

	//分配数据包中的数据单元数
	bool Create(int m_alltflv);

	//用特定的行数和列数初始化数据包
	bool InitCreate(int tflvnum,int querynum);




	// 构造要发送的数据单元
	int MakeTLV(unsigned char * lpDes,CGlobalStruct::TFLV m_tflv);

	// 添加特定数据的数据单元
	bool AddTFLV(CEnumCore::TagName tag, CEnumCore::TagFormat tagFormat, int length, unsigned char * lpdata);

	// 添加特定数据的数据单元
	bool AddTFLV(CEnumCore::TagName tag, CEnumCore::TagFormat tagFormat, size_t length, unsigned char * lpdata);

	// 添加单个数据单元
	bool AddTFLV(CGlobalStruct::TFLV m_tflv);

	// 构造数据包
	int EnBody(void);
	int EnBody(vector<CGlobalStruct::TFLV> *TFLVList, unsigned char * pDes);



	// 解析接收到的数据并封装
	CGlobalStruct::TFLV DeMakeTLV(unsigned char * lpSou,int* m_length, int index, int* m_Judge);

	// 解析接收到的数据
	BOOL DeBody(unsigned char* lpSou, int bodylength);



	// 获取所有的TFLV数目
	int getAllTFLV();

	// 获取所有的Query数目
	int getQueryNum();

	// 获取所有的列数
	int getColTFLVNum();




	// 利用数据单元的索引查询数据单元
	CGlobalStruct::TFLV getTLVBynIndex(int nIndex);

	// 利用数据单元的TagName和开始索引查询数据单元
	CGlobalStruct::TFLV getTLVByTag(CEnumCore::TagName tag,int beginIndex);

	// 利用数据单元的行、列号查询数据单元
	CGlobalStruct::TFLV getTLV(int nRow,int nCol);

	// 利用数据单元的行号和索引查询数据单元
	CGlobalStruct::TFLV getTLVByName(int nRow,CEnumCore::TagName tag);

	//设置真正列数
	bool getActiveCol(CEnumCore::TagName tag);

	//是否设置过真正列数
	bool IsActiveCol();

	//清空innerlist
	void ClearInnerList();

	//释放数据包中的资源
	bool Destroy(void);

public:

	//执行查询，构造最终要发送的数据
	int OperVectorMain(char* ip, char* sql, int flag,int nIndex,int nPageSize);

	//执行操作，返回操作结果
	int OperVectorMain(char* ip, char* sql, int flag);

	//执行查询，构造最终要发送的数据
	int OperVectorMain(char * gamename,char* ip, char* sql, int flag,int nIndex,int nPageSize);
	
	//执行操作，返回操作结果
	int OperVectorMain(char * gamename,char* ip, char* sql, int flag);
	
	//maple add
	int OperVectorMain_char(char * gamename,char* ip, char* sql, int flag,char *str);

public:

	//执行查询，返回查询结果集
//	vector<CGlobalStruct::DataRecord> * GetDBSet(char * gamename,char* ip, char* sql, int flag,int nIndex,int nPageSize);

	vector<CGlobalStruct::DataRecord> GetDBSet(char * gamename,char* ip, char* sql, int flag,int nIndex,int nPageSize);
	//处理结果集，构造TFLV数组
	vector<CGlobalStruct::TFLV> GetTFLV(vector<CGlobalStruct::DataRecord> m_datarecord);
	
	vector<CGlobalStruct::TFLV> GetInnerList();
};
