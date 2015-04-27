// DlgPaneConfig.cpp : implementation file
//

#include "stdafx.h"
#include "MultiDock.h"
#include "DlgPaneConfig.h"
#include "afxdialogex.h"
#include "..\Common\XmlConfig.h"
#include "MainFrm.h"

// CDlgPaneConfig dialog

IMPLEMENT_DYNAMIC(CDlgPaneConfig, CDialogEx)

CDlgPaneConfig::CDlgPaneConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPaneConfig::IDD, pParent)
{

}

CDlgPaneConfig::~CDlgPaneConfig()
{
}

void CDlgPaneConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_SHOWPANE_1, m_checkBtn1);
	DDX_Control(pDX, IDC_CHECK_SHOWPANE_2, m_checkBtn2);
	DDX_Control(pDX, IDC_CHECK_SHOWPANE_3, m_checkBtn3);
	DDX_Control(pDX, IDC_CHECK_SHOWPANE_4, m_checkBtn4);
	DDX_Control(pDX, IDC_CHECK_SHOWPANE_5, m_checkBtn5);
	DDX_Control(pDX, IDC_CHECK_SHOWPANE_6, m_checkBtn6);
	DDX_Control(pDX, IDC_CHECK_SHOWPANE_7, m_checkBtn7);
	DDX_Control(pDX, IDC_CHECK_SHOWPANE_8, m_checkBtn8);
	DDX_Control(pDX, IDC_CHECK_SHOWPANE_9, m_checkBtn9);
	DDX_Control(pDX, IDC_CHECK_SHOWPANE_10, m_checkBtn10);
}


BEGIN_MESSAGE_MAP(CDlgPaneConfig, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SAVETOXML, &CDlgPaneConfig::OnBnClickedBtnSavetoxml)
END_MESSAGE_MAP()


// CDlgPaneConfig message handlers

BOOL CDlgPaneConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UINT nPaneIndex = 1;
	for (UINT id = IDC_CHECK_SHOWPANE_1; id<=IDC_CHECK_SHOWPANE_10; id++ )
	{
		CString strNode;
		strNode.Format(_T("BaseDlgNode\\bShow_%d"), nPaneIndex);

		int nCheckValue = AppXml()->GetAttributeInt(strNode, 0);
		((CButton*)GetDlgItem(id))->SetCheck(nCheckValue);

		nPaneIndex++;
	}

	UpdateData(FALSE);

	return TRUE;
}
void CDlgPaneConfig::OnBnClickedBtnSavetoxml()
{
	UpdateData(TRUE);

	//lee：拿到当前bar下面的basetabctrl。
	CMFCOutlookBarTabCtrl* pOutlookBar = NULL;
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	if (NULL != pMainFrm)
	{
		pOutlookBar = (CMFCOutlookBarTabCtrl*)(pMainFrm->m_wndNavigationBar.GetUnderlyingWindow());

		if (pOutlookBar == NULL)
		{
			ASSERT(FALSE);
			return ;
		}
	}


	BOOL bSetActive = FALSE;
	UINT nPaneIndex = 1;
	for (UINT id = IDC_CHECK_SHOWPANE_1; id<=IDC_CHECK_SHOWPANE_10; id++ )
	{
		UINT nCheckState = ((CButton*)GetDlgItem(id))->GetCheck();
		CString strNode;
		strNode.Format(_T("BaseDlgNode\\bShow_%d"), nPaneIndex);

		if (BST_UNCHECKED == nCheckState )
		{
			AppXml()->SetAttributeInt(strNode, 0);
			AppXml()->FlushData();
		}
		else if(BST_CHECKED == nCheckState )
		{
			AppXml()->SetAttributeInt(strNode, 1);
			AppXml()->FlushData();
		}

		if ((BOOL)nCheckState != pOutlookBar->IsTabVisible(nPaneIndex-1))
		{
			pOutlookBar->ShowTab(nPaneIndex-1, (BOOL)nCheckState, FALSE);
		}
		if ((BOOL)nCheckState && !bSetActive)//hide tab会使active tab index为空，导致显示的时候没有激活tab。需要设置一次激活tab。
		{
			pOutlookBar->SetActiveTab(nPaneIndex-1);
			bSetActive = TRUE;
		}

		nPaneIndex++;
	}

	pOutlookBar->RecalcLayout();
	
	CDialogEx::OnOK();
}
