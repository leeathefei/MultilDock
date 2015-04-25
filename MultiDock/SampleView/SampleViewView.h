#pragma once


class CSampleViewView : public CScrollView
{
protected: 
	CSampleViewView();
	DECLARE_DYNCREATE(CSampleViewView)

public:
	CSampleViewDoc* GetDocument() const;
	CMFCListCtrl    m_ListCtrl;

protected:
	virtual void OnDraw(CDC* pDC);      
	virtual void OnInitialUpdate();    

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSampleviewerActivateVerticalPane0();
	afx_msg void OnSampleviewerActivateHorizontalPane3();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();

};

#ifndef _DEBUG  
inline CSampleViewDoc* CSampleViewView::GetDocument() const
   { return reinterpret_cast<CSampleViewDoc*>(m_pDocument); }
#endif

