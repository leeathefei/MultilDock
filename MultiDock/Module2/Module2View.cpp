
// Module2View.cpp : implementation of the CModule2View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Module2.h"
#endif

#include "Module2Doc.h"
#include "Module2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CModule2View

IMPLEMENT_DYNCREATE(CModule2View, CView)

BEGIN_MESSAGE_MAP(CModule2View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CModule2View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CModule2View construction/destruction

CModule2View::CModule2View()
{
	// TODO: add construction code here

}

CModule2View::~CModule2View()
{
}

BOOL CModule2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CModule2View drawing

void CModule2View::OnDraw(CDC* /*pDC*/)
{
	CModule2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CModule2View printing


void CModule2View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CModule2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CModule2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CModule2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CModule2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CModule2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CModule2View diagnostics

#ifdef _DEBUG
void CModule2View::AssertValid() const
{
	CView::AssertValid();
}

void CModule2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CModule2Doc* CModule2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CModule2Doc)));
	return (CModule2Doc*)m_pDocument;
}
#endif //_DEBUG


// CModule2View message handlers
