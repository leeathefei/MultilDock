// ModulePane.cpp : implementation file
//

#include "stdafx.h"
#include "ModulePane.h"
#include "..\Common\ModuleDefs.h"

// CModulePane
CModulePane* CModulePane::ms_pActivePane = NULL;

IMPLEMENT_DYNAMIC(CModulePane, CDockablePane)

CModulePane::CModulePane()
{
   m_bInitialized =false;
   m_pWnd = NULL;
}

CModulePane::~CModulePane()
{
   TRACE1("The Pane:%08x is destroyed\n", this);
}


BEGIN_MESSAGE_MAP(CModulePane, CDockablePane)
   ON_WM_SIZE()
   ON_WM_CLOSE()
   ON_WM_DESTROY()
END_MESSAGE_MAP()

void CModulePane::InitPane( MODULE_WINDOW_DEF* pDef )
{
   m_strWndName = pDef->strWindowName;
   m_pWnd = pDef->pWnd;
   m_pWnd->ModifyStyle((DWORD)0, WS_CHILD);
   m_pWnd->SetParent(this);
   m_pWnd->SetOwner(this);
   m_pWnd->ShowWindow(SW_SHOW);
   m_bAutoDelete = pDef->bAutoDelete;

   m_bInitialized = true;
   
   AdjustLayout();
}


// CModulePane message handlers
void CModulePane::OnSize(UINT nType, int cx, int cy)
{
   CDockablePane::OnSize(nType, cx, cy);

   if( m_pWnd && m_pWnd->GetSafeHwnd() )
   {
      m_pWnd->MoveWindow(0, 0, cx, cy);
   }
}

 void CModulePane::OnClose()
 {
    if( m_pWnd && m_pWnd->GetSafeHwnd() && ::IsWindow(m_pWnd->m_hWnd) )
    {
       m_pWnd->SetParent(NULL);
       m_pWnd->SetOwner(NULL);
       m_pWnd->SendMessage(WM_CLOSE);
    }

    CDockablePane::OnClose();
 }

 void CModulePane::OnDestroy()
 {
    if( GetSafeHwnd() )
      CDockablePane::OnDestroy();
 }

 BOOL CModulePane::OnShowControlBarMenu( CPoint point )
 {
	return TRUE;
 }

 BOOL CModulePane::PreTranslateMessage(MSG* pMsg)
 {
    // TODO: Add your specialized code here and/or call the base class
    switch (pMsg->message)
    {
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
    case WM_NCLBUTTONDOWN:
    case WM_NCRBUTTONDOWN:
    case WM_NCMBUTTONDOWN:
    case WM_NCLBUTTONUP:
    case WM_NCRBUTTONUP:
    case WM_NCMBUTTONUP:
       if(pMsg->hwnd==GetSafeHwnd())
       {
          SetActivePane(this);  
       }
    }

    return CDockablePane::PreTranslateMessage(pMsg);
 }

 CModulePane* CModulePane::GetActivePane()
 {
    if(dynamic_cast<CModulePane*>(ms_pActivePane)==NULL)
       return NULL;

    try
    {
        if(ms_pActivePane->GetSafeHwnd()==NULL) 
           return NULL;
        return ms_pActivePane;
    }
    catch (...)
    {
       
    }

    return NULL;
 }

 void CModulePane::SetActivePane( CDockablePane* pActivePane )
 {
    if(dynamic_cast<CModulePane*>(pActivePane))
    {
       ms_pActivePane = static_cast<CModulePane*>(pActivePane);
    }
    else
    {
       //ASSERT(FALSE);
    }
 }

 CWnd* CModulePane::GetWndInActivePane()
 {
     CModulePane* pModulePane = GetActivePane();
     if(pModulePane)
        return pModulePane->m_pWnd;
     return NULL;
 }

