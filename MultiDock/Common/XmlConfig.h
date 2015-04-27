#pragma once
#include <string>
#include "RWLock.h"
#include <algorithm> 
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

using namespace std;
using namespace rapidxml;

#ifdef UNICODE
#define _tstring	wstring
#define _ttoupper	towupper
#define _ttolower	tolower
#else
#define _tstring	string
#define _ttoupper	toupper
#define _ttolower	towlower
#endif

class CXmlConfig
{
public:
	CXmlConfig(CString strConfigFile);
	~CXmlConfig(void);
	static CXmlConfig* Instance(LPCTSTR lpszConfigFile=NULL);
  static void DeleteInstance();
   static CXmlConfig* m_pInstance;
   void SetLanguage();
   void QueryDefaultLanguage();
private:
	CRWLock	m_RWLock;
	BOOL m_bKnownLanguage;
	CString m_strLanguageCode;
	xml_node<TCHAR>* m_xmlRoot;
	xml_document<TCHAR> m_xmlDoc;  
	CString m_sAppPath;
	CString m_sAppDBPath;
	CString m_sAppIniPath;
	CString m_strConfigFile;
	
	static CCriticalSection m_Lock;
	std::string UrlEncode(const std::string& str);
	std::string UrlDecode(const std::string& str);
	xml_node<TCHAR>* SetNode(const TCHAR* chNodePath, const TCHAR* chNodeValue);
public:
	//CXmlConfig* AddChild(_tstring id,_tstring name,_tstring value=_T(""));
	void LockToRead();
	void LockToWrite();
	void UnLock();
	xml_node<TCHAR>* SetAttribute(const TCHAR* attributeName,const TCHAR* attributeValue);
	bool SetAttributeInt(const TCHAR* attributeName,int attributeValue);
	void SetAttributeLong(const TCHAR* attributeName,long attributeValue);
	void SetAttributeDouble(const TCHAR* attributeName,double attributeValue);
	void SetAttributeBool(const TCHAR* attributeName,bool attributeValue);
	bool GetChildBool(const TCHAR* szChildName);
	double GetChildDouble(const TCHAR* szChildName);
	_tstring GetChildText(const TCHAR* szChildName);
	long GetChildLong(const TCHAR* szChildName);
	int GetChildInt(const TCHAR* szChildName);
	void RemoveAttribute(const TCHAR* attributeName);
	void Remove(const TCHAR* path);
	void CXmlConfig::RemoveNode(const TCHAR* path);
	xml_node<TCHAR>* FindChild(const TCHAR* path);
	//CXmlConfig* FindChildById(const TCHAR* path);
	_tstring GetAttributeValue(const TCHAR* attributeName,const TCHAR* attributeDefaultValue = NULL,bool* pResult=NULL);
	_tstring GetAttributeText(const TCHAR* attributeName,_tstring defValue=_T(""),bool* pResult=NULL);
	int GetAttributeInt(const TCHAR* attributeName,int defValue=0,bool* pResult=NULL);
	long GetAttributeLong(const TCHAR* attributeName,long defValue=0,bool* pResult=NULL);
	double GetAttributeDouble(const TCHAR* attributeName,double defValue=0,bool* pResult=NULL);
	bool GetAttributeBool(const TCHAR* attributeName,bool defValue=false,bool* pResult=NULL);
	bool Save();
	//bool SaveTo(const TCHAR* filePath);
	//bool GetTextData(vector<BYTE>* pData);
	//_tstring GetText();
	xml_node<TCHAR>* GetRoot();
   xml_document<TCHAR>* GetDocument();
	//_tstring GetPath();
	//_tstring GetSrc();
	bool      FlushData();
	static CString   GetAppPath();
	CString   GetAppDBPath();
	UINT      GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault);
	bool      WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue);
	CString   GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault = NULL);
	bool      WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue);	
};
#define AppXml			CXmlConfig::Instance
#define AppIni			CXmlConfig::Instance
#define AppPath			CXmlConfig::Instance()->GetAppPath
#define AppDBPath	    CXmlConfig::Instance()->GetAppDBPath