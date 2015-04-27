#include "stdafx.h"
#include "FileHelper.h"

CString CFileHelper::GetModuleDir()
{
	TCHAR szModuleDir[1024];
	DWORD nResult = ::GetModuleFileName(NULL, szModuleDir, MAX_PATH);
	if (0 == nResult)
	{
		return _T("");
	}

	CString strModuleDir(szModuleDir);
	UINT nIndex = strModuleDir.ReverseFind('\\');
	return strModuleDir.Left(nIndex);
}