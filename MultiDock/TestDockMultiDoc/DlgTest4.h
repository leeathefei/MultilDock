#pragma once
#include "afxwin.h"


// CDlgTest4 dialog

class CDlgTest4 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTest4)

public:
	CDlgTest4(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTest4();

// Dialog Data
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CButton m_radioBtn;
	int m_nRadioValue;
	int m_nRadio2Value;
	CButton m_radio2Btn;
};
