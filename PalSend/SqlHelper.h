#pragma once
#import "c:\program files\common files\system\ado\msado15.dll"\
	no_namespace \
	rename ("EOF", "adoEOF")

class CSqlHelper
{
public:
	CSqlHelper(void);
	~CSqlHelper(void);
	bool ConnectData(char*);
	bool DoSql(char *);
	bool CloseData();
public:
	_ConnectionPtr m_pConnection;
	_RecordsetPtr	m_pRecordset;
};
