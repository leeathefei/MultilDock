// SampleViewerFrame.cpp : implementation file
//

#include "stdafx.h"
#include "SampleViewer.h"
#include "SampleViewerFrame.h"
#include "SampleViewerManager.h"
#include "SampleViewerDialog.h"

// CSampleViewerFrame

IMPLEMENT_DYNCREATE(CSampleViewerFrame, CMDIChildWndEx)

CSampleViewerFrame::CSampleViewerFrame()
{
   CSampleViewerManager::Instance()->AddFrameWnd(this);
}

CSampleViewerFrame::~CSampleViewerFrame()
{
}


BEGIN_MESSAGE_MAP(CSampleViewerFrame, CMDIChildWndEx)
   ON_WM_MDIACTIVATE()
   ON_WM_CLOSE()
   ON_WM_CREATE()
END_MESSAGE_MAP()



// CSampleViewerFrame message handlers



void CSampleViewerFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
   CSampleViewerManager* pDllManager = CSampleViewerManager::Instance();

   if(bActivate&&pActivateWnd==this)
   {
      pDllManager->ShowToolBar(TRUE);
      pDllManager->ActivatePane(_T("Vertical Pane 0"));
      pDllManager->ActivatePane(_T("Horizontal Pane 3"));
   }

   if(!bActivate&&pDeactivateWnd==this)
   {
      pDllManager->ShowToolBar(FALSE);
   }

   CMDIChildWndEx::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
}

void CSampleViewerFrame::OnClose()
{
   //Control the toolbar's refer_counter
   CSampleViewerManager::Instance()->RemoveFrameWnd(this);

   //CSampleViewerManager::Instance()->UnregisterModulePane( _T("Vertical Pane 1"));
   //CSampleViewerManager::Instance()->UnregisterModulePane(_T("Vertical Pane 0"));
   //CSampleViewerManager::Instance()->UnregisterModulePane(_T("Horizontal Pane 2"));
   //CSampleViewerManager::Instance()->UnregisterModulePane(_T("Horizontal Pane 3"));

   CMDIChildWndEx::OnClose();
}

int CSampleViewerFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
      return -1;



   return 0;
}
