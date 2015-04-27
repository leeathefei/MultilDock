#include "StdAfx.h"
#include "XmlConfig.h"

#define ENCODE(x) m_bKnownLanguage? (x):CString(CStringA(UrlEncode(CStringA(CString(x)).GetBuffer(0)).c_str())).GetBuffer(0)
#define DECODE(x) m_bKnownLanguage? (x):CString(CStringA(UrlDecode(CStringA(CString(x)).GetBuffer(0)).c_str())).GetBuffer(0)

CCriticalSection CXmlConfig::m_Lock;
CXmlConfig* CXmlConfig::m_pInstance = NULL;

BOOL FindFirstFileExists(LPCTSTR lpPath, DWORD dwFilter)
{
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(lpPath, &fd);
	BOOL bFilter = (FALSE == dwFilter) ? TRUE : fd.dwFileAttributes & dwFilter;
	BOOL RetValue = ((hFind != INVALID_HANDLE_VALUE) && bFilter) ? TRUE : FALSE;
	FindClose(hFind);
	return RetValue;
}

void ReplaceAll(_tstring &base,const _tstring &needle, const _tstring &s){
	unsigned long lastpos = 0, thispos;
	while ((thispos = base.find(needle, lastpos)) != _tstring::npos){
		base.replace(thispos, needle.length(), s);
		lastpos = thispos+s.length() ;
	}
}

CXmlConfig::CXmlConfig(CString strConfigFile):m_strLanguageCode(_T("0x0409")),m_bKnownLanguage(TRUE)
{
	if(!FindFirstFileExists((LPCTSTR)strConfigFile,FALSE))
	{
		HANDLE hFile = NULL;
		hFile = CreateFile(strConfigFile, READ_CONTROL, 0, NULL,
			CREATE_ALWAYS, 0, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hFile);
		}
	}
	QueryDefaultLanguage();
	m_strConfigFile = strConfigFile;
	try
	{
		SetLanguage();	
		std::wifstream configFile(m_strConfigFile,ios_base::binary,_SH_SECURE);//以安全权限方式打开
		if(configFile)
		{
			//configFile.imbue(std::locale("CHS")); 
			file<TCHAR> fdoc(configFile);
			TCHAR* pConfigData = m_xmlDoc.allocate_string(fdoc.data());        // 分配字符串并将Config数据复制进去
			m_xmlDoc.parse<0>(pConfigData); 
			locale::global(locale("C"));//还原全局区域设定
			m_xmlRoot = m_xmlDoc.first_node();	
			if(0 == m_xmlRoot)
			{
				m_xmlDoc.append_node(m_xmlDoc.allocate_node(rapidxml::node_pi,m_xmlDoc.allocate_string(_T("xml version='1.0' encoding='utf-8'"))));
				m_xmlRoot =   m_xmlDoc.allocate_node(node_element,_T("config"),_T(""));  
				m_xmlDoc.append_node(m_xmlRoot);
			}
			configFile.close();
		}
		else
		{
			locale::global(locale("C"));//还原全局区域设定
			//LOGEX_FATAL(_T("cannot open file: ") + m_strConfigFile);
			throw runtime_error(string("cannot open file ")/* + filename*/);
		}
	}
	catch(...)
	{
		locale::global(locale("C"));//还原全局区域设定
		//LOGEX_FATAL(_T("error reading stream. "));
		throw runtime_error("error reading stream.");
	}


   locale::global(locale(""));
   //LOG_INFO(_T("ANSI CodePage: %d"), GetACP());
  // LOG_INFO(_T("SystemDefaultLangID: 0x%04x"), GetSystemDefaultLangID());
}

CXmlConfig::~CXmlConfig(void)
{
   m_xmlDoc.clear();//Free memory pool in xml_document<>
}

CXmlConfig* CXmlConfig::Instance(LPCTSTR lpszConfigFile/*=NULL*/)
{
	if( m_pInstance )
		return m_pInstance;

  CSingleLock singleLock(&m_Lock);
  singleLock.Lock();
	if( !m_pInstance ) 
	{
		CString strXmlDefault = GetAppPath();
		CString strConfigFile = lpszConfigFile==NULL? strXmlDefault+ _T("\\MultiDock.xml"):lpszConfigFile;
		try
		{
			m_pInstance = new CXmlConfig(strConfigFile);
    }
		catch (...)
		{
			//LOGEX_FATAL(_T("cannot open file: %s to read."), strConfigFile);
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	return m_pInstance;
}

void CXmlConfig::DeleteInstance()
{
   delete m_pInstance;
   m_pInstance = NULL;
}

void CXmlConfig::QueryDefaultLanguage()
{
	HKEY hLanguageKey;
	BYTE  strLanguageCode[512] = {0};
	DWORD szType = REG_SZ;//定义数据类型
	DWORD cbData = 512;//定义数据长度
	CString skeyName = _T("SYSTEM\\CurrentControlSet\\Control\\Nls\\Language");
	long errCode = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,skeyName, NULL, KEY_READ, &hLanguageKey);
	if(ERROR_SUCCESS == errCode)
	{
		errCode = RegQueryValueEx(hLanguageKey, _T("Default"),NULL,&szType,strLanguageCode,&cbData);
		if(ERROR_SUCCESS == errCode)
		{
			m_strLanguageCode = _T("0x");
			m_strLanguageCode += (TCHAR*)strLanguageCode;
		}
		//else
			//LOGEX_ERROR(_T("RegQueryValueEx fail, ErrCode: %d."),errCode);
	}
	//else
		//LOGEX_ERROR(_T("RegOpenKeyEx fail, ErrCode: %d."),errCode);
}

unsigned char ToHex(unsigned char x) 
{ 
	return  x > 9 ? x + 55 : x + 48; 
}

unsigned char FromHex(unsigned char x) 
{ 
	unsigned char y;
	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
	else if (x >= '0' && x <= '9') y = x - '0';
	else assert(0);
	return y;
}

std::string CXmlConfig::UrlEncode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (isalnum((unsigned char)str[i]) || 
			(str[i] == '-') ||
			(str[i] == '_') || 
			(str[i] == '.') || 
			(str[i] == '~'))
			strTemp += str[i];
// 		else if (str[i] == ' ')
// 			strTemp += '+';
		else
		{
			strTemp += '%';
			strTemp += ToHex((unsigned char)str[i] >> 4);
			strTemp += ToHex((unsigned char)str[i] % 16);
		}
	}
	return strTemp;
}

std::string CXmlConfig::UrlDecode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		/*if (str[i] == '+') strTemp += ' ';
		else*/ if (str[i] == '%')
		{
			assert(i + 2 < length);
			unsigned char high = FromHex((unsigned char)str[++i]);
			unsigned char low = FromHex((unsigned char)str[++i]);
			strTemp += high*16 + low;
		}
		else strTemp += str[i];
	}
	return strTemp;
}

void CXmlConfig::SetLanguage()
{
	if(_T("0x0804") == m_strLanguageCode)//中文简体
	{
		m_bKnownLanguage = TRUE;
		locale::global(locale("CHS"));
	}
	else if(_T("0x0412") == m_strLanguageCode)//韩文
	{
		m_bKnownLanguage = TRUE;
		locale::global(locale("KOR"));
	}
	else if(_T("0x0c04") == m_strLanguageCode)//中国香港
	{
		m_bKnownLanguage = TRUE;
		locale::global(locale("ZHH"));
	}
	else if(_T("0x1404") == m_strLanguageCode)//中国澳门
	{
		m_bKnownLanguage = TRUE;
		locale::global(locale("ZHM"));
	}
	else if(_T("0x1004") == m_strLanguageCode)//新加坡
	{
		m_bKnownLanguage = TRUE;
		locale::global(locale("ZHI"));
	}
	else if(_T("0x0404") == m_strLanguageCode)//中国台湾
	{
		m_bKnownLanguage = TRUE;
		locale::global(locale("CHT"));
	}
	else
	{
		m_bKnownLanguage = FALSE;
		locale::global(locale(""));//将全局区域设为操作系统默认区域
	}
}

bool CXmlConfig::FlushData()
{
   // Use Writing Lock to queue FlushData invokings in the same process,
   // to avoid open file failure caused by FlushData function being invoked 
   // in multiple threads at the same time. 
   // [HLZHU 2013-07-01]
   LockToWrite();
	try
	{
		SetLanguage();	
		std::wofstream configFile(m_strConfigFile,ios_base::out,_SH_DENYRW);//以独占读写权限方式打开 
		if(configFile)
		{
		//	configFile.imbue(std::locale("CHS")); 
			configFile << m_xmlDoc;
			locale::global(locale("C"));//还原全局区域设定
			configFile.close();
		}
		else
		{
			locale::global(locale("C"));//还原全局区域设定
			//LOGEX_FATAL(_T("cannot open file: %s to write."), m_strConfigFile);
			UnLock();
         return false;
		}
	}
	catch(...)
	{
		locale::global(locale("C"));//还原全局区域设定
		//LOGEX_FATAL(_T("write file: %s exception."), m_strConfigFile);
		UnLock();
      return false;
	}

   UnLock();
	return true;
}

bool CXmlConfig::Save()
{
	return FlushData();
}

xml_node<TCHAR>* CXmlConfig::GetRoot()
{
	ASSERT(FALSE);//不要轻易用这个接口，会跳过读写锁保护。
	return m_xmlRoot;
}

xml_document<TCHAR>* CXmlConfig::GetDocument()
{
   return &m_xmlDoc;
}

xml_node<TCHAR>* CXmlConfig::SetNode(const TCHAR* chNodePath, const TCHAR* chNodeValue)
{
	LockToWrite();
	
	TCHAR* pNodePath = m_xmlDoc.allocate_string(chNodePath);        // 分配字符串并将Path复制进去
	TCHAR* pNodeValue = m_xmlDoc.allocate_string(DECODE(chNodeValue));      // 分配字符串并将Value复制进去
	
	xml_node<TCHAR>* node = m_xmlRoot->set_node(pNodePath,pNodeValue);

	UnLock();
	return node;
}

xml_node<TCHAR>* CXmlConfig::SetAttribute(const TCHAR* attributeName,const TCHAR* attributeValue){
	return SetNode(attributeName,attributeValue);
}

xml_node<TCHAR>* CXmlConfig::FindChild(const TCHAR* path){
	_tstring allPath=path;
	ReplaceAll(allPath,_T("/"),_T("\\"));
	while(allPath.find(_T("\\\\"))!=_tstring::npos){
		ReplaceAll(allPath,_T("\\\\"),_T("\\"));
	}
	
	xml_node<TCHAR>* node = m_xmlRoot->search_node(path);

	return node;
}

bool CXmlConfig::SetAttributeInt(const TCHAR* attributeName,int attributeValue){
	TCHAR ch[50];
	_stprintf_s(ch,_T("%d"),attributeValue);
	return SetAttribute(attributeName,ch)? true:false;
}
void CXmlConfig::SetAttributeLong(const TCHAR* attributeName,long attributeValue){
	TCHAR ch[50];
	_stprintf_s(ch,_T("%d"),attributeValue);
	SetAttribute(attributeName,ch);
}
void CXmlConfig::SetAttributeDouble(const TCHAR* attributeName,double attributeValue){
	TCHAR ch[50];
	_stprintf_s(ch,_T("%f"),attributeValue);
	SetAttribute(attributeName,ch);
}
void CXmlConfig::SetAttributeBool(const TCHAR* attributeName,bool attributeValue){
	SetAttribute(attributeName,attributeValue?_T("true"):_T("false"));
}
void CXmlConfig::RemoveAttribute(const TCHAR* attributeName){

	LockToWrite();
	xml_node<TCHAR>* pXmlNode = m_xmlRoot->search_node(attributeName);
	if(pXmlNode)
		m_xmlRoot->remove_node(pXmlNode);
	UnLock();
}
int CXmlConfig::GetChildInt(const TCHAR* szChildName)
{
	int nRet = 0;

	LockToRead();
	xml_node<TCHAR>* pXmlNode = FindChild(szChildName);
	if ( pXmlNode)
	{
		nRet = _ttoi(pXmlNode->value());
	}
	UnLock();

	return nRet;
}
long CXmlConfig::GetChildLong(const TCHAR* szChildName)
{
	long lRet = 0;

	LockToRead();
	xml_node<TCHAR>* pXmlNode = FindChild(szChildName);
	if ( pXmlNode)
	{
		lRet = _ttol(pXmlNode->value());
	}
	UnLock();

	return lRet;
}
_tstring CXmlConfig::GetChildText(const TCHAR* szChildName)
{
	_tstring strRet = _T("");

	LockToRead();
	xml_node<TCHAR>* pXmlNode = FindChild(szChildName);
	if ( pXmlNode)
	{
		strRet = DECODE(pXmlNode->value());
	}
	UnLock();

	return strRet;
}
double CXmlConfig::GetChildDouble(const TCHAR* szChildName)
{
	double dRet = 0.0;

	LockToRead();
	xml_node<TCHAR>* pXmlNode = FindChild(szChildName);
	if ( pXmlNode)
	{
		dRet = _tcstod(pXmlNode->value(),NULL);
	}
	UnLock();

	return dRet;
}
bool CXmlConfig::GetChildBool(const TCHAR* szChildName)
{
	LockToRead();
	_tstring strRet = _T("");
	xml_node<TCHAR>* pXmlNode = FindChild(szChildName);
	if ( pXmlNode)
	{
		strRet = pXmlNode->value();
	}
	transform(strRet.begin(),strRet.end(),strRet.begin(),_ttolower);
	UnLock();

	if(strRet.find(_T("true"))!=_tstring::npos) return true;
	if(strRet.find(_T("false"))!=_tstring::npos) return false;
	if(strRet==_T("1")) return true;
	if(strRet==_T("0")) return false;
	return false;
}
void CXmlConfig::Remove(const TCHAR* path)
{
	RemoveNode(path);
}
void CXmlConfig::RemoveNode(const TCHAR* path){
	LockToRead();
	xml_node<TCHAR>* pXmlNode = FindChild(path);
	UnLock();
	if(pXmlNode)
	{
		LockToWrite();
		if(pXmlNode->parent())
			pXmlNode->parent()->remove_node(pXmlNode);
		UnLock();
	}

}
_tstring CXmlConfig::GetAttributeText(const TCHAR* attributeName,_tstring defValue,bool* pResult){
	bool bRet=false;
	_tstring textValue=GetAttributeValue(attributeName,defValue.c_str(),&bRet);
	if(pResult) (*pResult)=bRet;
	return bRet?textValue:defValue;
}
int CXmlConfig::GetAttributeInt(const TCHAR* attributeName,int defValue,bool* pResult){
	bool bRet=false;
	CString strDefaultValve;
	strDefaultValve.Format(_T("%d"),defValue);
	_tstring textValue=GetAttributeValue(attributeName,strDefaultValve,&bRet);
	if(pResult) (*pResult)=bRet;
	return bRet?_ttoi(textValue.c_str()):defValue;
}
long CXmlConfig::GetAttributeLong(const TCHAR* attributeName,long defValue,bool* pResult){
	bool bRet=false;
	CString strDefaultValve;
	strDefaultValve.Format(_T("%d"),defValue);
	_tstring textValue=GetAttributeValue(attributeName,strDefaultValve,&bRet);
	if(pResult) (*pResult)=bRet;
	return bRet?_ttol(textValue.c_str()):defValue;
}
double CXmlConfig::GetAttributeDouble(const TCHAR* attributeName,double defValue,bool* pResult){
	bool bRet=false;
	CString strDefaultValve;
	strDefaultValve.Format(_T("%f"),defValue);
	_tstring textValue=GetAttributeValue(attributeName,strDefaultValve,&bRet);
	if(pResult) (*pResult)=bRet;
	return bRet?_tcstod(textValue.c_str(),NULL):defValue;
}
bool CXmlConfig::GetAttributeBool(const TCHAR* attributeName,bool defValue,bool* pResult){
	bool bRet=false;
	CString strDefaultValve = defValue? _T("true"):_T("false");
	_tstring textValue=GetAttributeValue(attributeName,strDefaultValve,&bRet);
	if(pResult) (*pResult)=bRet;
	if(!bRet) return defValue;

	transform(textValue.begin(),textValue.end(),textValue.begin(),_ttolower);
	if(textValue.find(_T("true"))!=_tstring::npos) return true;
	if(textValue.find(_T("false"))!=_tstring::npos) return false;
	if(textValue==_T("1")) return true;
	if(textValue==_T("0")) return false;

	if(pResult) (*pResult)=false;
	return defValue;
}

_tstring CXmlConfig::GetAttributeValue(const TCHAR* attributeName,const TCHAR* attributeDefaultValue,bool* pResult){
	
	LockToRead();
	xml_node<TCHAR>* node = FindChild(attributeName);
	bool bNodeExists = node? true:false;
	if(pResult)
		(*pResult) = bNodeExists;
	_tstring strRetValue = bNodeExists? DECODE(node->value()):attributeDefaultValue;
	UnLock();

	if(!bNodeExists)
		SetNode(attributeName,attributeDefaultValue);

	return strRetValue;
}

UINT CXmlConfig::GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault)
{
	bool bResult = false;
	UINT uiVal = GetAttributeInt(CString(lpszSection) + _T("\\") + lpszEntry,nDefault,&bResult);
	if (bResult)
		return uiVal;
	return nDefault;
}

bool CXmlConfig::WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue)
{
	return SetAttributeInt(CString(lpszSection) + _T("\\") + lpszEntry, nValue);
}

CString CXmlConfig::GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault /*=NULL*/)
{
	bool bResult = false;
	CString strVal = GetAttributeValue(CString(lpszSection) + _T("\\") + lpszEntry,lpszDefault,&bResult).c_str();
	if (bResult)
		return strVal;

	return CString(lpszDefault);
}

bool CXmlConfig::WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue)
{
	return SetAttribute(CString(lpszSection) + _T("\\") + lpszEntry,lpszValue)? true:false;
}

CString  CXmlConfig::GetAppDBPath()
{
	if( m_sAppDBPath.IsEmpty() )
	{
		m_sAppDBPath = GetAppPath() + _T("\\DB");
	}

	return m_sAppDBPath;
}

CString  CXmlConfig::GetAppPath()
{
	CString strAppPath;
	TCHAR szTemp[1024];
	::GetModuleFileName(GetModuleHandle(_T("MultiDock.exe")), szTemp, 1024);
	strAppPath = (const TCHAR*)szTemp;
	int iIndex = strAppPath.ReverseFind('\\');
	strAppPath = strAppPath.Left(iIndex);

	return strAppPath;
}

void CXmlConfig::LockToRead()
{
	m_RWLock.WaitToRead();
}

void CXmlConfig::LockToWrite()
{
	m_RWLock.WaitToWrite();
}

void CXmlConfig::UnLock()
{
	m_RWLock.Done();
}

