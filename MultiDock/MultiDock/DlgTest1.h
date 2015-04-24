#pragma once
#include "afxwin.h"


// CDlgTest1 dialog

class CDlgTest1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTest1)

public:
	CDlgTest1(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTest1();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CComboBox m_comboBox1;
	CComboBox m_comboBox2;
	CComboBox m_comboBox3;
};
