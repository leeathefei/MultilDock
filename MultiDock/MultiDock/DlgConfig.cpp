
#include "stdafx.h"
#include "MultiDock.h"
#include "DlgConfig.h"
#include "afxdialogex.h"
#include "MultiDockView.h"


COutlookOptionsDlg::COutlookOptionsDlg(CMFCOutlookBarTabCtrl& parentBar)
	: CDialog(COutlookOptionsDlg::IDD, &parentBar), m_parentBar(parentBar)
{
}

void COutlookOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutlookOptionsDlg)
	DDX_Control(pDX, IDC_AFXBARRES_MOVEUP, m_btnMoveUp);
	DDX_Control(pDX, IDC_AFXBARRES_MOVEDOWN, m_wndMoveDown);
	DDX_Control(pDX, IDC_AFXBARRES_LIST, m_wndList);
	DDX_Control(pDX, IDC_AFXBARRES_RESET, m_wndReset);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COutlookOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(COutlookOptionsDlg)
	ON_LBN_SELCHANGE(IDC_AFXBARRES_LIST, &COutlookOptionsDlg::OnSelchange)
	ON_LBN_DBLCLK(IDC_AFXBARRES_LIST, &COutlookOptionsDlg::OnDblclkList)
	ON_BN_CLICKED(IDC_AFXBARRES_MOVEDOWN, &COutlookOptionsDlg::OnMoveDown)
	ON_BN_CLICKED(IDC_AFXBARRES_MOVEUP, &COutlookOptionsDlg::OnMoveUp)
	ON_BN_CLICKED(IDC_AFXBARRES_RESET, &COutlookOptionsDlg::OnReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutlookOptionsDlg message handlers

void COutlookOptionsDlg::OnSelchange()
{
	m_btnMoveUp.EnableWindow(m_wndList.GetCurSel() > 0);
	m_wndMoveDown.EnableWindow(m_wndList.GetCurSel() < m_wndList.GetCount() - 1);
}

void COutlookOptionsDlg::OnDblclkList()
{
	int nSel = m_wndList.GetCurSel();
	if (nSel >= 0)
	{
		m_wndList.SetCheck(nSel, !m_wndList.GetCheck(nSel));
	}
}

void COutlookOptionsDlg::OnMoveDown()
{
	MoveItem(FALSE);
}

void COutlookOptionsDlg::OnMoveUp()
{
	MoveItem(TRUE);
}

BOOL COutlookOptionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (AfxGetMainWnd() != NULL && (AfxGetMainWnd()->GetExStyle() & WS_EX_LAYOUTRTL))
	{
		ModifyStyleEx(0, WS_EX_LAYOUTRTL);
	}

	for (int i = 0; i < m_parentBar.m_iTabsNum; i ++)
	{
		CString str;
		m_parentBar.GetTabLabel(i, str);

		int nIndex = m_wndList.AddString(str);

		m_wndList.SetItemData(nIndex, (DWORD_PTR) i);
		m_wndList.SetCheck(nIndex, m_parentBar.IsTabVisible(i));
	}

	m_wndList.SetCurSel(0);
	OnSelchange();

	CMFCOutlookBar* pOutlookBar = DYNAMIC_DOWNCAST(CMFCOutlookBar, m_parentBar.GetParent());
	if (pOutlookBar == NULL)
	{
		m_wndReset.EnableWindow(FALSE);
		m_wndReset.ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}

void COutlookOptionsDlg::OnOK()
{
	CArray<int, int> arTabsOrder;

	for (int nIndex = 0; nIndex < m_wndList.GetCount(); nIndex++)
	{
		int i = (int) m_wndList.GetItemData(nIndex);

		BOOL bVisible = m_wndList.GetCheck(nIndex);

		if (bVisible != m_parentBar.IsTabVisible(i))
		{
			m_parentBar.ShowTab(i, bVisible, FALSE);
		}

		arTabsOrder.Add(i);
	}

	m_parentBar.SetTabsOrder(arTabsOrder);

	CDialog::OnOK();
}

void COutlookOptionsDlg::OnReset()
{
	CMFCOutlookBar* pOutlookBar = DYNAMIC_DOWNCAST(CMFCOutlookBar, m_parentBar.GetParent());
	if (pOutlookBar == NULL)
	{
		ASSERT(FALSE);
		return;
	}

	CArray<int, int> arTabsOrder;
	int i = 0;

	for (i = 0; i < pOutlookBar->GetDefaultTabsOrder().GetSize(); i++)
	{
		int iTabID = pOutlookBar->GetDefaultTabsOrder() [i];
		int iTab = m_parentBar.GetTabByID(iTabID);

		if (iTab < 0)
		{
			ASSERT(FALSE);
			return;
		}

		arTabsOrder.Add(iTab);
	}

	m_wndList.ResetContent();

	for (i = 0; i < arTabsOrder.GetSize(); i ++)
	{
		int iTabNum = arTabsOrder [i];

		CString str;
		m_parentBar.GetTabLabel(iTabNum, str);

		int nIndex = m_wndList.AddString(str);

		m_wndList.SetItemData(nIndex, (DWORD_PTR) iTabNum);
		m_wndList.SetCheck(nIndex, TRUE);
	}

	m_wndList.SetCurSel(0);
	OnSelchange();
}

void COutlookOptionsDlg::MoveItem(BOOL bMoveUp)
{
	int nSel = m_wndList.GetCurSel();

	CString str;
	m_wndList.GetText(nSel, str);
	DWORD_PTR dwData = m_wndList.GetItemData(nSel);
	BOOL bCheck = m_wndList.GetCheck(nSel);

	m_wndList.DeleteString(nSel);

	int nNewIndex = bMoveUp ? nSel - 1 : nSel + 1;

	int nIndex = m_wndList.InsertString(nNewIndex, str);

	m_wndList.SetItemData(nIndex, dwData);
	m_wndList.SetCheck(nIndex, bCheck);

	m_wndList.SetCurSel(nIndex);
	OnSelchange();
}