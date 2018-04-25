#include "StdAfx.h"
#include ".\sqlhelper.h"


CSqlHelper::CSqlHelper(void)
{
}

CSqlHelper::~CSqlHelper(void)
{
}
bool CSqlHelper::ConnectData(char* sql)
{
	CoInitialize(NULL);
	m_pConnection=NULL;
	m_pRecordset=NULL;
	m_pConnection.CreateInstance(__uuidof(Connection));
	try 
	{	
		//m_pConnection->Open("Provider=SQLOLEDB.1;Persist Security Info=True;User ID=GMTOOLS;Password=GMTOOLS123!@#;Data Source=192.168.0.125;Initial Catalog=mydb","","",adModeUnknown);
		m_pConnection->Open(sql,"","",adModeUnknown);
	}
	catch(_com_error e)
	{	
		return FALSE;
	}
	return TRUE;
}

bool CSqlHelper::DoSql(char * sql)
{
	_variant_t RecordsAffected;
	try		
	{	
		m_pRecordset=m_pConnection->Execute(sql,&RecordsAffected,adCmdText); 

	}	
	catch(_com_error e)
	{		
		return FALSE;
	}
	return TRUE;
}


bool CSqlHelper::CloseData()
{
	if(m_pRecordset!=NULL)
	{
		if(m_pRecordset->State) 
		{
			m_pRecordset->Close();
			m_pRecordset= NULL;
		}
	}
	if(m_pConnection->State) 
	{
		m_pConnection->Close();
		m_pConnection= NULL;
	}
	CoUninitialize();
	return FALSE;
}
