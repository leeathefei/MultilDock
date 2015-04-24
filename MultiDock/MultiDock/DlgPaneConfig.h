#pragma once
#include "afxwin.h"


// CDlgPaneConfig dialog

class CDlgPaneConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPaneConfig)

public:
	CDlgPaneConfig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPaneConfig();

// Dialog Data
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSavetoxml();
	CButton m_checkBtn1;
	CButton m_checkBtn2;
	CButton m_checkBtn3;
	CButton m_checkBtn4;
	CButton m_checkBtn5;
	CButton m_checkBtn6;
	CButton m_checkBtn7;
	CButton m_checkBtn8;
	CButton m_checkBtn9;
	CButton m_checkBtn10;
};
