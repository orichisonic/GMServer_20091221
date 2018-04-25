// 下列 ifdef 块是创建使从 DLL 导出更简单的
//宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 SQLHELPER_EXPORTS
// 符号编译的。在使用此 DLL 的
//任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将 
// SQLHELPER_API 函数视为是从此 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef SQLHELPER_EXPORTS
#define SQLHELPER_API __declspec(dllexport)
#else
#define SQLHELPER_API __declspec(dllimport)
#endif
#include "database.h"
#include "DBRecord.h"
using namespace std;
#include <vector>
#include "..\\Global\\Global.h"
#pragma comment(lib,"..\\Global\\Debug\\Global.lib")

// 此类是从 SqlHelper.dll 导出的
class SQLHELPER_API CSqlHelper {
public:
	//构造函数
	CSqlHelper(void);

	//析构函数
	~CSqlHelper(void);

	//输出vector,输入ip地址,Sql语句,flag标志,返回查询类是否查到
	bool SqlHelperMain(vector<CGlobalStruct::DataRecord>* ret_vector,char * ip,char *sql,int flag,int nIndex, int nPageSize);

	//根据游戏查询类返回vecter集
	bool SqlHelperMain(char * gamename,vector<CGlobalStruct::DataRecord>* ret_vector,char * ip,char *sql,int flag,int nIndex, int nPageSize);
	
	vector<CGlobalStruct::DataRecord> SqlHelperMain(char * gamename,char * ip,char *sql,int flag,int nIndex, int nPageSize);

	//返回操作类是否成功
	bool SqlHelperMain(char * ip,char *sql,int flag);
	
	//根据游戏操作类返回成功失败
	bool SqlHelperMain(char * gamename,char * ip,char *sql,int flag);

	//maple add
	void SqlHelperMain_char(char * gamename,char * ip,char *sql,int flag,char * result);
	
	//获取列数
	int GetFieldNum();

private:
	//构造数据库连接Driver
	void MakeDataSource(char * szData,char *ip,int flag);

	//根据游戏构造数据库连接Driver
	void MakeDataSource(char * gamename,char * szData,char *ip,int flag);

	//获取SQL数据库
	void GetSQLData(char* gamename,char * szData,char * lpIP,int flag);

	//获取MySql数据库
	void GetMySqlData(char* gamename,char * szData,char * lpIP,int flag);


	//获取SQL数据库
	void GetSQLData(char * szData,char * lpIP,int flag);
	
	//获取MySql数据库
	void GetMySqlData(char * szData,char * lpIP,int flag);
	//获取SQL数据库
	void GetSQLData(char * user,char * passwd,int iSQLPort,char * szData,char * lpIP);
	//获取MySql数据库
	void GetMySqlData(char * user,char * passwd,int iMySQLPort,char * CharSet,char * szData,char * lpIP);

	// TODO: 在此添加您的方法。
	void GetDefault(char * gamename,char *user,char * passwd,char * CharSet,int* iSQLPort,int *iMySqlPort);

	//构建SQL加上限制limit
	void MakeSQL(char * lpobjSQL,char * lpResSQL,int flag,int iIndex,int iPageSize);




private:
	int fieldnum;
	//连接数据库
//	CDatabase 	database;

	//datarecord
//	CGlobalStruct::DataRecord tmpDataRecord;


};

extern SQLHELPER_API int nSqlHelper;

SQLHELPER_API int fnSqlHelper(void);
