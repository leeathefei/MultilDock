
// Module1View.cpp : implementation of the CModule1View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Module1.h"
#endif

#include "Module1Doc.h"
#include "Module1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CModule1View

IMPLEMENT_DYNCREATE(CModule1View, CView)

BEGIN_MESSAGE_MAP(CModule1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CModule1View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CModule1View construction/destruction

CModule1View::CModule1View()
{
	// TODO: add construction code here

}

CModule1View::~CModule1View()
{
}

BOOL CModule1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CModule1View drawing

void CModule1View::OnDraw(CDC* /*pDC*/)
{
	CModule1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CModule1View printing


void CModule1View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CModule1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CModule1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CModule1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CModule1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CModule1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CModule1View diagnostics

#ifdef _DEBUG
void CModule1View::AssertValid() const
{
	CView::AssertValid();
}

void CModule1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CModule1Doc* CModule1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CModule1Doc)));
	return (CModule1Doc*)m_pDocument;
}
#endif //_DEBUG


// CModule1View message handlers
