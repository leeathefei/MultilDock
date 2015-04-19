// DlgTest4.cpp : implementation file
//

#include "stdafx.h"
#include "MultiDock.h"
#include "DlgTest4.h"
#include "afxdialogex.h"


// CDlgTest4 dialog

IMPLEMENT_DYNAMIC(CDlgTest4, CDialogEx)

CDlgTest4::CDlgTest4(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTest4::IDD, pParent)
	, m_nRadioValue(0)
	, m_nRadio2Value(0)
{

}

CDlgTest4::~CDlgTest4()
{
}

void CDlgTest4::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_radioBtn);
	DDX_Radio(pDX, IDC_RADIO1, m_nRadioValue);
	DDX_Control(pDX, IDC_BUTTON1, m_radio2Btn);
}

BOOL CDlgTest4::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}

BEGIN_MESSAGE_MAP(CDlgTest4, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgTest4::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgTest4 message handlers


void CDlgTest4::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
}
