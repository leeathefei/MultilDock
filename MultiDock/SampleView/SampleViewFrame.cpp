
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SampleView.h"
#include "SampleViewFrame.h"
#include "SampleViewManager.h"
#include "SampleViewDialog.h"

// CSampleViewFrame

IMPLEMENT_DYNCREATE(CSampleViewFrame, CMDIChildWndEx)

	CSampleViewFrame::CSampleViewFrame()
{
	CSampleViewManager::Instance()->AddFrameWnd(this);
}

CSampleViewFrame::~CSampleViewFrame()
{
}


BEGIN_MESSAGE_MAP(CSampleViewFrame, CMDIChildWndEx)
	ON_WM_MDIACTIVATE()
	ON_WM_CLOSE()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CSampleViewFrame message handlers



void CSampleViewFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CSampleViewManager* pDllManager = CSampleViewManager::Instance();

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

void CSampleViewFrame::OnClose()
{
	//Control the toolbar's refer_counter
	CSampleViewManager::Instance()->RemoveFrameWnd(this);

	//CSampleViewManager::Instance()->UnregisterModulePane( _T("Vertical Pane 1"));
	//CSampleViewManager::Instance()->UnregisterModulePane(_T("Vertical Pane 0"));
	//CSampleViewManager::Instance()->UnregisterModulePane(_T("Horizontal Pane 2"));
	//CSampleViewManager::Instance()->UnregisterModulePane(_T("Horizontal Pane 3"));

	CMDIChildWndEx::OnClose();
}

int CSampleViewFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;



	return 0;
}
