
#include "stdafx.h"
#include "MultiDock.h"
#include "MainFrm.h"
#include "XmlConfig.h"
#include "DlgPaneConfig.h"
#include "DlgConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_SET_PANE1_CMD1, &CMainFrame::OnRibbonPane1Cmd1)
	ON_COMMAND(ID_SET_PANE1_CMD2, &CMainFrame::OnRibbonPane1Cmd2)
	ON_COMMAND(ID_SET_PANE1_CMD3, &CMainFrame::OnRibbonPane1Cmd3)
	ON_COMMAND(ID_SET_PANE1_CMD4, &CMainFrame::OnRibbonPane1Cmd4)

	ON_COMMAND(ID_SET_PANE2_CMD1, &CMainFrame::OnRibbonPane2Cmd1)
	ON_COMMAND(ID_SET_PANE2_CMD2, &CMainFrame::OnRibbonPane2Cmd2)
	ON_COMMAND(ID_SET_PANE2_CMD3, &CMainFrame::OnRibbonPane2Cmd3)
	ON_COMMAND(ID_SET_PANE2_CMD4, &CMainFrame::OnRibbonPane2Cmd4)
	
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_OFF_2007_BLUE, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_OFF_2007_BLUE, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	//ON_COMMAND(ID_TOOLS_OPTIONS, &CMainFrame::OnOptions)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,         
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


CMainFrame::CMainFrame()
{
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // other styles available...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // set to FALSE to place close button at right of tab area
	mdiTabParams.m_bTabIcons = FALSE;    // set to TRUE to enable document icons on MDI taba
	mdiTabParams.m_bAutoColor = TRUE;    // set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = TRUE; // enable the document menu at the right edge of the tab area
	EnableMDITabbedGroups(TRUE, mdiTabParams);


	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, 
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Allow user-defined toolbars operations:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);

	//create rabin，这行代码必须在在Menu创建完成才可以。
	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Load menu item image (not placed on any standard toolbars):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// create docking windows
	//if (!CreateFileView())
	m_vecUserDlgs.clear();
	int nBasePaneCount = AppXml()->GetAttributeInt(_T("BaseDlgNodeCount"), 0);
	if(nBasePaneCount > 0)
	{
		for(int index=1; index<=nBasePaneCount; index++)
		{
			stBasePane oneItem;
			oneItem.pBaseDlg = new CBaseDlg;
			
			CString strNode;
			strNode.Format(_T("BaseDlgNode\\Index_%d"), index);
			oneItem.eType = (EDLGTYPE)(AppXml()->GetAttributeInt(strNode, 0));

			strNode.Empty();
			strNode.Format(_T("BaseDlgNode\\Title_%d"), index);
			std::wstring strTitle= AppXml()->GetAttributeText(strNode, _T(""));
			oneItem.strPaneTitle = strTitle.c_str();

			strNode.Empty();
			strNode.Format(_T("BaseDlgNode\\bShow_%d"), index);
			oneItem.bShow = AppXml()->GetAttributeInt(strNode) == 0 ? FALSE : TRUE;
			
			m_vecUserDlgs.push_back(oneItem);
		}
	}
	else if(nBasePaneCount == 0)//没配置文件，默认创建一个。
	{
		stBasePane oneItem;
		oneItem.pBaseDlg = new CBaseDlg;
		oneItem.strPaneTitle = _T("测试面板一");
		oneItem.eType = enmDlgType_Test1;
		m_vecUserDlgs.push_back(oneItem);

		stBasePane oneItem2;
		oneItem2.pBaseDlg = new CBaseDlg;
		oneItem2.strPaneTitle = _T("测试面板二");
		oneItem2.eType = enmDlgType_Test2;
		m_vecUserDlgs.push_back(oneItem2);
	
		stBasePane oneItem3;
		oneItem3.pBaseDlg = new CBaseDlg;
		oneItem3.strPaneTitle = _T("测试面板三");
		oneItem3.eType = enmDlgType_Test3;
		m_vecUserDlgs.push_back(oneItem3);

		stBasePane oneItem4;
		oneItem4.pBaseDlg = new CBaseDlg;
		oneItem4.strPaneTitle = _T("测试面板四");
		oneItem4.eType = enmDlgType_Test4;
		m_vecUserDlgs.push_back(oneItem4);
	}
	

	if (!CreateOutlookBar(m_wndNavigationBar, ID_VIEW_NAVIGATION,250))
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	CreateFileView();


	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);

	m_wndNavigationBar.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndNavigationBar);

	//EnableDocking(CBRS_ALIGN_LEFT);
	//EnableAutoHidePanes(CBRS_ALIGN_RIGHT);

	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// enable menu personalization (most-recently used commands)
	// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	// Switch the order of document name and application name on the window title bar. This
	// improves the usability of the taskbar because the document name is visible with the thumbnail.
	ModifyStyle(0, FWS_PREFIXTITLE);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}
BOOL CMainFrame::CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, int nInitialWidth)
{
	bar.SetMode2003();

	BOOL bNameValid;
	CString strTemp("Short Cut");
	
	if (!bar.Create(strTemp, this, CRect(0, 0, nInitialWidth, 32000), uiID,
		WS_CHILD 
		| WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI|CBRS_FLOATING))
	{
		return FALSE;
	}

	//lee：拿到当前bar下面的basetabctrl。
	CMFCOutlookBarTabCtrl* pOutlookBar = (CMFCOutlookBarTabCtrl*)bar.GetUnderlyingWindow();

	if (pOutlookBar == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pOutlookBar->EnableInPlaceEdit(TRUE);

	static UINT uiPageID = 1;
	static UINT uDlgID = 1200;

	// can float, can autohide, can resize, CAN NOT CLOSE
	DWORD dwStyle = AFX_CBRS_FLOAT | AFX_CBRS_AUTOHIDE | AFX_CBRS_RESIZE;

	CRect rectDummy(0, 0, 0, 0);
	int nDlgIndex = 1;
	BOOL bSetActive = FALSE;
	for(VecBasePanes::iterator it = m_vecUserDlgs.begin();
		it != m_vecUserDlgs.end(); ++it)
	{
		CBaseDlg* pBaseDlg = it->pBaseDlg;
		if(NULL != pBaseDlg)
		{
			CString strNode;
			strNode.Format(_T("BaseDlgNode\\Index_%d"), nDlgIndex);
			AppXml()->SetAttributeInt(strNode, (UINT)it->eType);
			AppXml()->FlushData();

			strNode.Empty();
			strNode.Format(_T("BaseDlgNode\\Title_%d"), nDlgIndex);
			AppXml()->SetAttribute(strNode, it->strPaneTitle);
			AppXml()->FlushData();

			strNode.Empty();
			strNode.Format(_T("BaseDlgNode\\bShow_%d"), nDlgIndex);
			AppXml()->SetAttributeInt(strNode, (UINT)it->bShow);
			AppXml()->FlushData();

			
			pBaseDlg->Create(rectDummy, &bar,  it->eType, uDlgID++);
			pOutlookBar->AddControl(pBaseDlg, it->strPaneTitle, ++uiPageID, TRUE, dwStyle);
			pOutlookBar->ShowTab(nDlgIndex-1, it->bShow,TRUE);//!:tab index start with 0;
			if (it->bShow && !bSetActive)//hide tab会使active tab index为空，导致显示的时候没有激活tab。需要设置一次激活tab。
			{
				pOutlookBar->SetActiveTab(nDlgIndex-1);
				bSetActive = TRUE;
			}
		}
		nDlgIndex++;
	}
	AppXml()->SetAttributeInt(_T("BaseDlgNodeCount"), m_vecUserDlgs.size());
	AppXml()->FlushData();


	bNameValid = strTemp.LoadString(IDS_CALENDAR);
	ASSERT(bNameValid);
	
	bar.SetPaneStyle(bar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	pOutlookBar->SetImageList(theApp.m_bHiColorIcons ? IDB_PAGES_HC : IDB_PAGES, 24);
	pOutlookBar->SetToolbarImageList(theApp.m_bHiColorIcons ? IDB_PAGES_SMALL_HC : IDB_PAGES_SMALL, 16);
	pOutlookBar->RecalcLayout();

	BOOL bAnimation = theApp.GetInt(_T("OutlookAnimation"), TRUE);
	CMFCOutlookBarTabCtrl::EnableAnimation(bAnimation);

	bar.SetButtonsFont(&afxGlobalData.fontBold);

	return TRUE;
}
BOOL CMainFrame::CreateFileView()
{
	BOOL bNameValid;

	// Create file view
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, 
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT| CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create File View window\n");
		return FALSE; 
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), 
		MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON),
		::GetSystemMetrics(SM_CYSMICON), 0);

	m_wndFileView.SetIcon(hFileViewIcon, FALSE);
}

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif


void CMainFrame::OnRibbonPane1Cmd1()
{
	//lee：拿到当前bar下面的basetabctrl。
	CMFCOutlookBarTabCtrl* pOutlookBar = (CMFCOutlookBarTabCtrl*)m_wndNavigationBar.GetUnderlyingWindow();

	if (pOutlookBar == NULL)
	{
		ASSERT(FALSE);
		return /*FALSE*/;
	}
	
	//pOutlookBar->OnShowOptions();

	COutlookOptionsDlg dlg(*pOutlookBar);
	if (dlg.DoModal() == IDOK)
	{
		pOutlookBar->RecalcLayout();
	}

}
void CMainFrame::OnRibbonPane1Cmd2()
{
	CDlgPaneConfig dlg;
	dlg.DoModal();

	//AfxMessageBox(_T("调出你自己的用户界面1！"));
}
void CMainFrame::OnRibbonPane1Cmd3()
{
	AfxMessageBox(_T("调出你自己的用户界面2！"));
}
void CMainFrame::OnRibbonPane1Cmd4()
{
	AfxMessageBox(_T("调出你自己的用户界面3！"));
}
void CMainFrame::OnRibbonPane2Cmd1()
{
	AfxMessageBox(_T("调出你自己的用户界面4！"));
}
void CMainFrame::OnRibbonPane2Cmd2()
{
	AfxMessageBox(_T("调出你自己的用户界面5！"));
}
void CMainFrame::OnRibbonPane2Cmd3()
{
	AfxMessageBox(_T("调出你自己的用户界面6！"));
}
void CMainFrame::OnRibbonPane2Cmd4()
{
	AfxMessageBox(_T("调出你自己的用户界面7！"));
}

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
}

