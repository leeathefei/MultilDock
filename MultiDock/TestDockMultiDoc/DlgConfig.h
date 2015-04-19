#pragma once
#include "afxwin.h"




/////////////////////////////////////////////////////////////////////////////
// COutlookOptionsDlg dialog

class COutlookOptionsDlg : public CDialog
{
	// Construction
public:
	COutlookOptionsDlg(CMFCOutlookBarTabCtrl& parentBar);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(COutlookOptionsDlg)
	enum { IDD = IDD_AFXBARRES_OUTLOOKBAR_OPTIONS };
	CButton m_btnMoveUp;
	CButton m_wndMoveDown;
	CButton m_wndReset;
	CMFCToolBarsListCheckBox m_wndList;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutlookOptionsDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COutlookOptionsDlg)
	afx_msg void OnSelchange();
	afx_msg void OnDblclkList();
	afx_msg void OnMoveDown();
	afx_msg void OnMoveUp();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CMFCOutlookBarTabCtrl& m_parentBar;

	void MoveItem(BOOL bMoveUp);
};
