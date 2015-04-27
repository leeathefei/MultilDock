/********************************************************************
	Created:	2015/04/27   11:15
	Filename:	CSVFileReader.h
	Author:		Chris Lee
	Function:	
*********************************************************************/

#pragma once
#include <map>
#include <vector>

using namespace std;

class CCSVFile : public CStdioFile
{
public:
	CCSVFile();
	virtual ~CCSVFile();

	BOOL Load();
	int  GetCount();

	CString  ReadString(int i, LPCTSTR skey, bool* pRet=NULL);
	UINT  ReadUInt(int i, LPCTSTR skey, bool* pRet=NULL);
	bool  ReadBool(int i, LPCTSTR skey, bool* pRet=NULL);


private:
	map<CString,int>  m_mapLookupIndex;
	vector<vector<CString>> m_data;
};

