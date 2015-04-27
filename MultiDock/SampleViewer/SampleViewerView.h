#pragma once

// CSampleViewerView view

class CSampleViewerView : public CScrollView
{
	DECLARE_DYNCREATE(CSampleViewerView)

protected:
	CSampleViewerView();           // protected constructor used by dynamic creation
	virtual ~CSampleViewerView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif


   CMFCListCtrl    m_ListCtrl;

protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnSampleviewerActivateVerticalPane0();
   afx_msg void OnSampleviewerActivateHorizontalPane3();
   afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
   afx_msg void OnSize(UINT nType, int cx, int cy);
   afx_msg void OnTimer(UINT_PTR nIDEvent);
   afx_msg void OnDestroy();
};


