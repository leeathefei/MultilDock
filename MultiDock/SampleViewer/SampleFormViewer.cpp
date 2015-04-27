// SampleFormViewer.cpp : implementation file
//

#include "stdafx.h"
#include "SampleViewer.h"
#include "SampleFormViewer.h"


// CSampleFormViewer

IMPLEMENT_DYNCREATE(CSampleFormViewer, CFormView)

CSampleFormViewer::CSampleFormViewer()
	: CFormView(CSampleFormViewer::IDD)
{

}

CSampleFormViewer::~CSampleFormViewer()
{
}

void CSampleFormViewer::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSampleFormViewer, CFormView)
END_MESSAGE_MAP()


// CSampleFormViewer diagnostics

#ifdef _DEBUG
void CSampleFormViewer::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSampleFormViewer::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}


#endif
#endif //_DEBUG


// CSampleFormViewer message handlers
BOOL CSampleFormViewer::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
                               DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
                               CCreateContext* pContext)
{
   return CFormView::Create(lpszClassName, lpszWindowName, dwRequestedStyle, rect, pParentWnd, nID, pContext);
}
