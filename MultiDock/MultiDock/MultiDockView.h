
// MultiDockView.h : interface of the CMultiDockView class
//

#pragma once

class CMultiDockDoc;
class CMultiDockView : public CView
{
protected: // create from serialization only
	CMultiDockView();
	DECLARE_DYNCREATE(CMultiDockView)

// Attributes
public:
	CMultiDockDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMultiDockView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg int  OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

public:
	CListCtrl* m_pListCtrl;
};

#ifndef _DEBUG  // debug version in MultiDockView.cpp
inline CMultiDockDoc* CMultiDockView::GetDocument() const
   { return reinterpret_cast<CMultiDockDoc*>(m_pDocument); }
#endif

