
// SampleViewView.cpp : implementation of the CSampleViewView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SampleView.h"
#endif

#include "SampleViewDoc.h"
#include "SampleViewView.h"
#include "SampleViewManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSampleViewView

IMPLEMENT_DYNCREATE(CSampleViewView, CScrollView)

BEGIN_MESSAGE_MAP(CSampleViewView, CScrollView)
   ON_COMMAND(ID_SAMPLEVIEWER_ACTIVATEVERTICALPANE0, &CSampleViewView::OnSampleviewerActivateVerticalPane0)
   ON_COMMAND(ID_SAMPLEVIEWER_ACTIVATEHORIZONTALPANE3, &CSampleViewView::OnSampleviewerActivateHorizontalPane3)
   ON_WM_RBUTTONUP()
   ON_WM_SIZE()
   ON_WM_TIMER()
   ON_WM_DESTROY()
END_MESSAGE_MAP()

// CSampleViewView construction/destruction

CSampleViewView::CSampleViewView()
{
	// TODO: add construction code here

}

CSampleViewView::~CSampleViewView()
{
}
void CSampleViewView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	CRect rcClient;
	GetClientRect(&rcClient);
	m_ListCtrl.Create(LVS_REPORT, rcClient, this, 99);
	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_ListCtrl.ShowWindow(SW_SHOW);
	m_ListCtrl.InsertColumn(0, _T("Index"));
	m_ListCtrl.InsertColumn(1, _T("InstanceID"));
	m_ListCtrl.InsertColumn(2, _T("Protocol"));

	SetTimer(1000, 50, NULL);
}


// CSampleViewView drawing

void CSampleViewView::OnDraw(CDC* /*pDC*/)
{
	CSampleViewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}


// CSampleViewView printing

void CSampleViewView::OnSampleviewerActivateVerticalPane0()
{
	CSampleViewManager* pManager = CSampleViewManager::Instance();
	pManager->ActivatePane(_T("Vertical Pane 0"));
}

void CSampleViewView::OnSampleviewerActivateHorizontalPane3()
{
	CSampleViewManager* pManager = CSampleViewManager::Instance();
	pManager->ActivatePane(_T("Horizontal Pane 3"));
}

void CSampleViewView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	AfxMessageBox(_T("CSampleViewView"));
	CScrollView::OnRButtonUp(nFlags, point);
}

void CSampleViewView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	if(m_ListCtrl.GetSafeHwnd())
	{
		CRect rc;
		GetClientRect(&rc);
		m_ListCtrl.MoveWindow(&rc, TRUE);
		m_ListCtrl.SetColumnWidth(0, 100);
		m_ListCtrl.SetColumnWidth(1, 100);
		m_ListCtrl.SetColumnWidth(2, 100);
	}
}



// CSampleViewView diagnostics

#ifdef _DEBUG
void CSampleViewView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CSampleViewView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CSampleViewDoc* CSampleViewView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSampleViewDoc)));
	return (CSampleViewDoc*)m_pDocument;
}
#endif //_DEBUG
