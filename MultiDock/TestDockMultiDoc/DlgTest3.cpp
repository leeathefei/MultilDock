// DlgTest3.cpp : implementation file
//

#include "stdafx.h"
#include "MultiDock.h"
#include "DlgTest3.h"
#include "afxdialogex.h"


// CDlgTest3 dialog

IMPLEMENT_DYNAMIC(CDlgTest3, CDialogEx)

CDlgTest3::CDlgTest3(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTest3::IDD, pParent)
{

}

CDlgTest3::~CDlgTest3()
{
}

void CDlgTest3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
}

BOOL CDlgTest3::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	m_edit1.SetWindowTextW(_T("«Î ‰»Î÷µ..."));
	m_edit2.SetWindowTextW(_T("Input your value..."));

	return TRUE;
}

BEGIN_MESSAGE_MAP(CDlgTest3, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgTest3::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgTest3 message handlers


void CDlgTest3::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
}
