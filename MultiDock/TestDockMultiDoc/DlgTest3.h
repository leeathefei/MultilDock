#pragma once
#include "afxwin.h"


// CDlgTest3 dialog

class CDlgTest3 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTest3)

public:
	CDlgTest3(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTest3();

// Dialog Data
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CEdit m_edit1;
	CEdit m_edit2;
};
