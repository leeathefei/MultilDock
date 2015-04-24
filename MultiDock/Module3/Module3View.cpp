
// Module3View.cpp : implementation of the CModule3View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Module3.h"
#endif

#include "Module3Doc.h"
#include "Module3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CModule3View

IMPLEMENT_DYNCREATE(CModule3View, CView)

BEGIN_MESSAGE_MAP(CModule3View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CModule3View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CModule3View construction/destruction

CModule3View::CModule3View()
{
	// TODO: add construction code here

}

CModule3View::~CModule3View()
{
}

BOOL CModule3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CModule3View drawing

void CModule3View::OnDraw(CDC* /*pDC*/)
{
	CModule3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CModule3View printing


void CModule3View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CModule3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CModule3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CModule3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CModule3View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CModule3View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CModule3View diagnostics

#ifdef _DEBUG
void CModule3View::AssertValid() const
{
	CView::AssertValid();
}

void CModule3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CModule3Doc* CModule3View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CModule3Doc)));
	return (CModule3Doc*)m_pDocument;
}
#endif //_DEBUG


// CModule3View message handlers
