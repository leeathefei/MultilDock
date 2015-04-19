// DlgTest2.cpp : implementation file
//

#include "stdafx.h"
#include "MultiDock.h"
#include "DlgTest2.h"
#include "afxdialogex.h"


// CDlgTest2 dialog
const int nBorderSize = 10;


IMPLEMENT_DYNAMIC(CDlgTest2, CDialogEx)

CDlgTest2::CDlgTest2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTest2::IDD, pParent)
{
}

CDlgTest2::~CDlgTest2()
{
}

void CDlgTest2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboBox1);
	DDX_Control(pDX, IDC_COMBO2, m_comboBox2);
}


BEGIN_MESSAGE_MAP(CDlgTest2, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON1, &CDlgTest2::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL CDlgTest2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_comboBox1.InsertString(0, _T("这是测试值1"));
	m_comboBox2.InsertString(0, _T("这是测试值2"));

	m_comboBox1.SetCurSel(0);
	m_comboBox2.SetCurSel(0);

	return TRUE;
}

void CDlgTest2::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
}
