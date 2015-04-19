
// MultiDockView.cpp : implementation of the CMultiDockView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MultiDock.h"
#endif

#include "MultiDockDoc.h"
#include "MultiDockView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMultiDockView

IMPLEMENT_DYNCREATE(CMultiDockView, CView)

BEGIN_MESSAGE_MAP(CMultiDockView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMultiDockView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CMultiDockView construction/destruction

CMultiDockView::CMultiDockView()
{
	// TODO: add construction code here
	m_pListCtrl = NULL;
}

CMultiDockView::~CMultiDockView()
{
	if (NULL != m_pListCtrl)
	{
		m_pListCtrl->DestroyWindow();
		delete m_pListCtrl;
		m_pListCtrl = NULL;
	}
}

BOOL CMultiDockView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMultiDockView drawing

void CMultiDockView::OnDraw(CDC* /*pDC*/)
{
	CMultiDockDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CMultiDockView printing


void CMultiDockView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMultiDockView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMultiDockView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMultiDockView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMultiDockView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMultiDockView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMultiDockView diagnostics

#ifdef _DEBUG
void CMultiDockView::AssertValid() const
{
	CView::AssertValid();
}

void CMultiDockView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMultiDockDoc* CMultiDockView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMultiDockDoc)));
	return (CMultiDockDoc*)m_pDocument;
}
#endif //_DEBUG


// CMultiDockView message handlers
int CMultiDockView::OnCreate(LPCREATESTRUCT lpcs)
{
	if (CView::OnCreate(lpcs) == -1)
	{
		return -1;
	}


	m_pListCtrl = new CListCtrl();
	m_pListCtrl->Create(WS_CHILD|WS_VISIBLE	|WS_BORDER|LVS_SINGLESEL|LVS_SHOWSELALWAYS|LVS_NOSORTHEADER|LVS_REPORT, 
		CRect(0,0,0,0), this, 2015);
	m_pListCtrl->SetExtendedStyle(LVS_EX_HEADERDRAGDROP	|LVS_EX_INFOTIP	|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	//m_pListCtrl->EnableMarkSortedColumn(FALSE);
	m_pListCtrl->EnableToolTips();

	m_pListCtrl->InsertColumn(0, _T("Column1"), LVCFMT_LEFT, 120);
	m_pListCtrl->InsertColumn(1, _T("Column2"), LVCFMT_LEFT, 120);
	m_pListCtrl->InsertColumn(2, _T("Column3"), LVCFMT_LEFT, 120);
	m_pListCtrl->InsertColumn(3, _T("Column4"), LVCFMT_LEFT, 120);
	m_pListCtrl->InsertColumn(4, _T("Column5"), LVCFMT_LEFT, 120);
	m_pListCtrl->InsertColumn(5, _T("Column6"), LVCFMT_LEFT, 120);
}

void CMultiDockView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if ( !m_pListCtrl->GetSafeHwnd())
		return;

	m_pListCtrl->SetWindowPos(NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

	CRect rect;
	GetClientRect(&rect);
	m_pListCtrl->MoveWindow(&rect);

	m_pListCtrl->GetClientRect(&rect);
	int nWidth = rect.Width();

}