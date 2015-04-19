#pragma once
#include "afxwin.h"


// CDlgTest2 dialog

class CDlgTest2 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTest2)

public:
	CDlgTest2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTest2();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CComboBox m_comboBox1;
	CComboBox m_comboBox2;
};
