#include "StdAfx.h"
#include "Workspace.h"
#include "XmlConfig.h"

CWorkspace::CWorkspace(void)
{
}

CWorkspace::~CWorkspace(void)
{
}

BOOL CWorkspace::Read()
{
   CXmlConfig::Instance()->LockToRead();
   CXmlConfig* pXml = CXmlConfig::Instance();

   xml_node<TCHAR>* pNode = pXml->FindChild(_T("Workspace\\StartupModules"));
   xml_node<TCHAR>* pModuleNode = pNode? pNode->first_node():NULL;
   while( pModuleNode )
   {
      CString strModuleName = pModuleNode->name();
      CModuleContext moduleContext;
      if( moduleContext.Read(pModuleNode) )
      {
         mapModuleContext[strModuleName] = moduleContext;
      }

      pModuleNode = pModuleNode->next_sibling();
   }
   CXmlConfig::Instance()->UnLock();
}

BOOL CWorkspace::Write()
{
   CXmlConfig::Instance()->LockToWrite();
   CXmlConfig* pXml = CXmlConfig::Instance();

   CString strNodePath = _T("Workspace\\StartupModules");
   CString strModuleName;
   CModuleContext oModuleContext;
   POSITION pos = mapModuleContext.GetStartPosition();
   while(pos)
   {
      mapModuleContext.GetNextAssoc(pos, strModuleName, oModuleContext);
      oModuleContext.Write(pXml);
   }

   CXmlConfig::Instance()->UnLock();
}

BOOL CViewContext::Read( LPVOID )
{

}

BOOL CViewContext::Write( LPVOID )
{

}

BOOL CPaneContext::Read( LPVOID )
{

}

BOOL CPaneContext::Write( LPVOID )
{

}

BOOL CModuleContext::Read( LPVOID lpszNodePath)
{
   CXmlConfig* pXml = CXmlConfig::Instance();

   CString strViewsNode = lpszNodePath + CString(_T("\\View"));
   xml_node<TCHAR>* pNode = pXml->FindChild(strViewsNode);
   xml_node<TCHAR>* pViewNode = pNode? pNode->first_node():NULL;
   while( pViewNode )
   {
      CViewContext oViewContext;
      oViewContext.strViewName = pViewNode->name();

      oViewContext.Read(strViewsNode)

      CModuleContext moduleContext;
      if( moduleContext.Read(pModuleNode) )
      {
         mapModuleContext[strModuleName] = moduleContext;
      }

      pModuleNode = pModuleNode->next_sibling();
   }
   
}

BOOL CModuleContext::Write( LPVOID )
{

}
