#pragma once
#include "..\Common\ModuleDefs.h"
#include "..\Common\ResourceHandle.h"
#include "..\Common\MTDocTemplate.h"

class CMTDocTemplate;
class  CModuleManager
{
public:
   CModuleManager(void);
   ~CModuleManager(void);

   // Only for modules which contain doc/view
   virtual BOOL RegisterDocTemplate(CMTDocTemplate* pDocTemplate=NULL);
   virtual BOOL RegisterToolBar(UINT nToolbarResID, CMFCToolBar* &pToolbar);
   virtual void RemoveToolBar(CMFCToolBar* pToolbar);
   virtual void ShowToolBar(CMFCToolBar* pToolbar, BOOL bShow=TRUE);


   // For modules which contain dialog pane.
   virtual BOOL RegisterModulePane(CWnd* pWnd=NULL, LPCTSTR lpszWndName=NULL, HICON hIcon=NULL, EPANE_ALIGNMENT align=ALIGN_VERTICAL, bool bAttachToPrev=true, bool bAutoDelete=true);
   virtual void ActivatePane(CString strWndName);
   virtual BOOL UnregisterModulePane(LPCTSTR lpszWndName);
   virtual void ActivateFrame(CMDIChildWnd* pFrame);

   void         ShowPane(CString strWndName);
   CWnd*        GetWndInActivePane();

   // For all modules(doc/view only, dialog pane only, doc/view plus dialog pane)
   virtual BOOL CanClose(CString& strMessage=CString(_T("")));
   virtual void Terminate();


public:
   CMFCToolBar* CreateToolbar(UINT nToolbarRes);
   BOOL  RegisterToolBar(CMFCToolBar* pToolbar);
   bool  IsDocTemplateRegistered() { return m_bDocTemplateRegistered; }
   LRESULT  SendMainWndMessage(UINT,WPARAM,LPARAM);

protected:
   bool m_bDocTemplateRegistered;
};