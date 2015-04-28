#include "StdAfx.h"
#include "ModuleManager.h"

CModuleManager::CModuleManager(void)
{
   m_bDocTemplateRegistered = FALSE;
}

CModuleManager::~CModuleManager(void)
{
}


BOOL CModuleManager::RegisterDocTemplate(CMTDocTemplate* pDocTemplate)
{
   if( pDocTemplate==NULL  )
      return FALSE;

   if( m_bDocTemplateRegistered )
      return TRUE;

   AfxGetApp()->AddDocTemplate(pDocTemplate);

   m_bDocTemplateRegistered = true;
   return TRUE;
}

BOOL CModuleManager::RegisterToolBar(UINT nToolbarResID, CMFCToolBar* &pToolbar)
{
   pToolbar = CreateToolbar(nToolbarResID);
   return RegisterToolBar(pToolbar);
}

BOOL CModuleManager::RegisterToolBar( CMFCToolBar* pToolbar )
{
   if( pToolbar )
   {
      LRESULT lErrCode = SendMainWndMessage(WM_USER+2, (WPARAM)pToolbar, TOOLBAR_DEF::TOOLBAR_ADD);
      return lErrCode==0? TRUE:FALSE;
   }
   else
      return FALSE;
}

CMFCToolBar* CModuleManager::CreateToolbar( UINT nToolbarRes)
{
   CWnd* pMainFrame = AfxGetMainWnd();
   CMFCToolBar* pToolbar = new CMFCToolBar;
   if( !pToolbar->CreateEx(pMainFrame, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
      | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) )
   {
      DWORD dw = GetLastError();
      TRACE0("Failed to create toolbar\n");
      delete pToolbar;
      pToolbar = NULL;
      return NULL;
   }

   if( !pToolbar->LoadToolBar(nToolbarRes, 0, 0, FALSE, 0, 0, 0) )
   {
      DWORD dw = GetLastError();
      TRACE0("Failed to create toolbar\n");
      delete pToolbar;
      pToolbar = NULL;
      return NULL;
   }

   return pToolbar;
}

void CModuleManager::ShowToolBar(CMFCToolBar* pToolbar,  BOOL bShow/*=TRUE*/)
{
   TOOLBAR_DEF::CONTROL_CODE nCode = bShow? TOOLBAR_DEF::TOOLBAR_SHOW:TOOLBAR_DEF::TOOLBAR_HIDE;
   SendMainWndMessage(WM_USER+2, (LPARAM)pToolbar, (WPARAM)nCode);
}

void CModuleManager::RemoveToolBar(CMFCToolBar* pToolbar)
{
   SendMainWndMessage(WM_USER+2, (LPARAM)pToolbar, (WPARAM)TOOLBAR_DEF::TOOLBAR_REMOVE);
}

BOOL CModuleManager::RegisterModulePane( CWnd* pWnd/*=NULL*/, LPCTSTR lpszWndName/*=NULL*/, HICON hIcon/*=NULL*/, EPANE_ALIGNMENT align/*=ALIGN_VERTICAL*/, bool bAttachToPrev/*=true*/, bool bAutoDelete/*=true*/)
{
   MODULE_WINDOW_DEF moduleDef;
   moduleDef.pWnd = pWnd;
   moduleDef.strWindowName = lpszWndName;
   moduleDef.bAttachToPrev = bAttachToPrev;
   moduleDef.bAutoDelete = bAutoDelete;
   moduleDef.nEnabledAlign = align;
   moduleDef.hIcon = hIcon;
   LRESULT lErrCode = SendMainWndMessage(WM_USER+1, (WPARAM)&moduleDef, 0);
   
   return lErrCode==0? TRUE:FALSE;
}

BOOL CModuleManager::UnregisterModulePane(LPCTSTR lpszWndName)
{
   LRESULT lErrCode = SendMainWndMessage(WM_USER+4, 0, (LPARAM)lpszWndName);
   return lErrCode==0? TRUE:FALSE;
}

BOOL CModuleManager::CanClose(CString& strMessage/*=CString(_T(""))*/)
{
   return TRUE;
}

void CModuleManager::Terminate()
{
   return;
}

void CModuleManager::ActivatePane( CString strWndName )
{
   MODULE_WINDOW_DEF::CONTROL_CODE nCode = MODULE_WINDOW_DEF::PANE_ACTIVATE;
   SendMainWndMessage(WM_USER+3, (WPARAM)nCode, (LPARAM)strWndName.GetString());
}

void CModuleManager::ActivateFrame( CMDIChildWnd* pFrame )
{
   SendMainWndMessage(WM_USER+12, 0, (LPARAM)pFrame);
}

void CModuleManager::ShowPane( CString strWndName )
{
   MODULE_WINDOW_DEF::CONTROL_CODE nCode = MODULE_WINDOW_DEF::PANE_SHOW;
   SendMainWndMessage(WM_USER+3, (WPARAM)nCode, (LPARAM)strWndName.GetString());
}

LRESULT CModuleManager::SendMainWndMessage( UINT nMessage,WPARAM wParam,LPARAM lParam)
{
   CWnd* pMainFrm = AfxGetMainWnd();
   if(pMainFrm->GetSafeHwnd())
   {
      return pMainFrm->SendMessage(nMessage, wParam, lParam);
   }

   return -1;
}

CWnd* CModuleManager::GetWndInActivePane()
{
   ASSERT(IsGUIThread(FALSE));

   CWnd* pMainFrm = AfxGetMainWnd();
   if(pMainFrm->GetSafeHwnd())
   {
      return (CWnd*)pMainFrm->SendMessage(WM_USER+13, 0, 0);
   }

   return NULL;
}
