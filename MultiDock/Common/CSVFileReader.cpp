
#include "stdafx.h"
#include "CSVFileReader.h"
#include "FileHelper.h"

CCSVFile::CCSVFile()
{

}

CCSVFile::~CCSVFile()
{

}

BOOL CCSVFile::Load()
{

	if( !Open(CFileHelper::GetModuleDir()+_T("\\Config\\InternalModules.csv"), CFile::modeRead) )
	{
		return FALSE;
	}

	CString rString;
	CString resToken;
	int curPos = 0;

	//Read header
	if(!CStdioFile::ReadString(rString))
	{
		return FALSE;
	}
	rString.TrimLeft();
	rString.TrimRight();

	int iColumn=0;
	curPos=0;
	resToken= rString.Tokenize(_T(","),curPos);
	while (resToken != _T(""))
	{
		resToken.TrimLeft();
		resToken.TrimRight();
		resToken.MakeLower();
		m_mapLookupIndex.insert(pair<CString,int>(resToken,iColumn));
		resToken = rString.Tokenize(_T(","), curPos);
		iColumn++;
	};


	//Read data
	int iRow=0;
	while(CStdioFile::ReadString(rString))
	{
		rString.TrimLeft();
		rString.TrimRight();
		if(rString.IsEmpty())
			continue;

		vector<CString> vecLine;
		curPos = 0;
		resToken= rString.Tokenize(_T(","),curPos);
		while (resToken != _T(""))
		{
			resToken.TrimLeft();
			resToken.TrimRight();
			vecLine.push_back(resToken);
			resToken = rString.Tokenize(_T(","), curPos);
		};

		if(vecLine.empty())
			continue;

		m_data.push_back(vecLine);
	}


	Close();

	return TRUE;
}

int CCSVFile::GetCount()
{
	return (int)m_data.size();
}

CString CCSVFile::ReadString( int i, LPCTSTR szkey, bool* pRet/*=NULL*/ )
{
	if(i>=(int)m_data.size())
	{
		throw _T("Invalid line");
	}

	CString strKey = szkey;
	strKey.MakeLower();
	map<CString, int>::iterator it = m_mapLookupIndex.find(strKey);
	if(it==m_mapLookupIndex.end())
	{
		throw _T("Invalid line");
	}

	int j = it->second;
	CString strText = m_data.at(i).at(j);

	return strText;
}

UINT CCSVFile::ReadUInt( int i, LPCTSTR skey, bool* pRet/*=NULL*/ )
{
	CString strText = ReadString(i, skey, pRet);
	TCHAR* endptr=NULL;
	UINT uiVal = _tcstoul(strText.GetBuffer(0), &endptr, 10);

	return uiVal;
}

bool CCSVFile::ReadBool( int i, LPCTSTR skey, bool* pRet/*=NULL*/ )
{
	CString strText = ReadString(i, skey, pRet);
	if(strText.CompareNoCase(_T("true"))==0)
		return true;
	else
		return false;
}
