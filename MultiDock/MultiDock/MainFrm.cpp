
#include "stdafx.h"
#include "MultiDock.h"
#include "MainFrm.h"
#include "..\Common\XmlConfig.h"
#include "DlgPaneConfig.h"
#include "DlgConfig.h"

#include "..\Common\ModuleDefs.h"
#include "..\Common\CSVFileReader.h"
#include "..\Common\FileHelper.h"
#include "..\Common\ResourceHandle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



#define  MAX_NUM_MODULES		   (300)
#define  MAX_NUM_UTILITES		   (300)
#define  MAX_NUM_VIEW_TYPES		(300)

#define  BASE_MODULES_MENU_ID	   (34000)
#define  BASE_UTILITIES_MENU_ID	(34000 + (MAX_NUM_MODULES*2))
#define  BASE_VIEWS_MENU_ID		(BASE_MODULES_MENU_ID + MAX_NUM_MODULES)


#define WM_INIT_MODULES				WM_USER+400


IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;
CString CMainFrame::m_strModuleMenuItems[2] = {_T("&Modules"), _T("&Utilities")};


BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_WM_CLOSE()
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

	ON_MESSAGE(WM_INIT_MODULES, OnInitModulePanes)
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
	m_pViewLastCreated = NULL;
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::OnClose()
{
	//m_exitEvent.SetEvent();

	//ResetWorkspaceNode();
	EnumTabbedView();
	//EnumDockablePane();

	CMDIFrameWndEx::OnClose();
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
	m_wndMenuBar.SetRecentlyUsedMenus(FALSE);
	m_wndMenuBar.SetShowAllCommands(TRUE);

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

	PostMessage(WM_INIT_MODULES);//OnInitModulePanes

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
//////////////////////////////////////////////////////////////////////////

void CMainFrame::CacheMenus()
{
	CCSVFile modules;
	if( !modules.Load() )
	{
		AfxMessageBox(_T("Load ...\\Config\\InternalModules.csv file failed!"));
		return;
	}


	int nModuleMenuId  = 0;
	int nUtilityMenuId = 0;
	for(int i=0; i<modules.GetCount(); ++i)
	{
		try
		{
			CMenuCommand mc;
			mc.m_strDll = modules.ReadString(i, _T("DLLName"));
			mc.m_strTitle =  modules.ReadString(i, _T("Title"));
			mc.m_nMenuID = i;
			mc.m_bAddToMenu = modules.ReadBool(i,_T("IsAddToMenu"));
			mc.m_bUtility = modules.ReadBool(i,_T("IsUtility"));
			mc.m_bHasView = modules.ReadBool(i,_T("HasView"));
			mc.m_bSingleView = modules.ReadBool(i,_T("IsSingleView"));
			mc.m_bLoadAtStartup = modules.ReadBool(i,_T("IsLoadAtStartup"));
			if(mc.m_bUtility)
				mc.m_nMenuID = nUtilityMenuId++;
			else
				mc.m_nMenuID = nModuleMenuId++;

			m_AllCommands.AddTail(mc);
		}
		catch(...)
		{
			continue;
		}
	}

	if(m_AllCommands.GetCount()==0)
	{
		AfxMessageBox(_T("Exec\\DB\\InternalModules.csv may be empty or corrupted!"));
	}

}

void CMainFrame::LoadModuleMenuItems()
{
	CacheMenus();

	POSITION	 pos;
	for (pos  = m_AllCommands.GetHeadPosition(); pos !=NULL; )
	{
		CMenuCommand &mc = m_AllCommands.GetNext(pos);
		if ( mc.m_bAddToMenu )
			AddModuleMenuItem(mc.m_strTitle, mc.m_nMenuID, FALSE, mc.m_bUtility);

		if(!mc.hLib)
		{
			mc.hLib = LoadLibrary(mc.m_strDll);
		}

		if(mc.hLib && !mc.m_bUtility )
		{
			typedef BOOL (*LPFN_GETICONRESOUCEID)(UINT&);
			LPFN_GETICONRESOUCEID pFunc = (LPFN_GETICONRESOUCEID)GetProcAddress(mc.hLib, "GetIconResourceID");
			if(pFunc)
			{
				UINT nResID;
				if(pFunc(nResID))
				{
					CIntaffCommonResourceHandle resHandler(mc.m_strDll, __FILEW__, __LINE__);
					HICON hIcon = (HICON) LoadImage( AfxGetResourceHandle(), MAKEINTRESOURCE( nResID ), IMAGE_ICON, 16, 16, 0 );
					
					mc.m_hIcon = hIcon;
				}
			}
		}

	}

	m_wndMenuBar.RecalcLayout();
}

void CMainFrame::AddModuleMenuItem(const CString &strModuleName, int nID, BOOL bIsView /*=FALSE*/, BOOL bIsUtility /*=FALSE*/)
{
	CMenu menu;
	menu.Attach(m_wndMenuBar.GetHMenu());

	BOOL bAppend=FALSE;
	int nIndex = FindMenuItem(&menu, m_strModuleMenuItems[bIsUtility?1:0]);
	CMenu *pSubMenu = menu.GetSubMenu(nIndex);
	if(bIsView)
		bAppend = pSubMenu->AppendMenu(MF_ENABLED|MF_STRING, BASE_VIEWS_MENU_ID+nID, strModuleName);
	else if(bIsUtility)
		bAppend = pSubMenu->AppendMenu(MF_ENABLED|MF_STRING, BASE_UTILITIES_MENU_ID+nID, strModuleName);
	else
		bAppend = pSubMenu->AppendMenu(MF_ENABLED|MF_STRING, BASE_MODULES_MENU_ID+nID, strModuleName);


	// Remove dummy
	if( pSubMenu->GetMenuItemCount() )
	{
		CString str;
		pSubMenu->GetMenuString(0, str, MF_BYPOSITION);
		if( str.CompareNoCase(_T("Dummy"))==0 )
		{
			pSubMenu->RemoveMenu(0, MF_BYPOSITION);
		}
	}

	m_wndMenuBar.CreateFromMenu(menu.Detach(), TRUE, TRUE);
}

int CMainFrame::FindMenuItem(CMenu *pMenu, const CString &str)
{
	int count = pMenu->GetMenuItemCount();
	CString temp;
	for (int i=0; i<count; i++)
	{
		if( pMenu->GetMenuString(i, temp, MF_BYPOSITION) && str == temp)
			return i;
	}

	return -1;
}

LRESULT CMainFrame::OnInitModulePanes(WPARAM W, LPARAM L)
{
	LockWindowUpdate();

	// Open Startup modules
	OpenStartupModules();

	// Open Viewers
	OpenLastUsedModules();

	// Open DevComm Module
	//OpenAlwaysShowPane();

	// Activate Pane
	//ActiveOnePaneByName(_T("Module1"), MODULE_WINDOW_DEF::PANE_ACTIVATE);

	UnlockWindowUpdate();

	return 0;

}

void CMainFrame::OpenStartupModules()
{
	
}

void CMainFrame::OpenLastUsedModules()
{
	LockWindowUpdate();

	StartupAsWorkspace();

	UnlockWindowUpdate();
}
BOOL CMainFrame::StartupAsWorkspace()
{
	CXmlConfig* pXml = CXmlConfig::Instance();
	if(pXml==NULL)
	{
		return FALSE;
	}


	pXml->LockToRead();
	xml_node<TCHAR>* pNodeTabbedView = pXml->FindChild(_T("Workspace\\TabbedView"));
	if(pNodeTabbedView)
	{
		//ParseNode<Alignment>
		xml_node<TCHAR>* pNodeAlignment = pNodeTabbedView->first_node(_T("Alignment"));
		int iGroupAlign = 0;
		BOOL bVert = TRUE;
		BOOL bNewMDITabbedGroup=FALSE;
		if(pNodeAlignment)
		{
			iGroupAlign = _ttoi((LPCTSTR)pNodeAlignment->value());
			bVert = (iGroupAlign==1);
		}


		//ParseNode<Group>
		xml_node<TCHAR>* pGroupNode = pNodeTabbedView->first_node();
		while(pGroupNode)
		{
			CString strName = pGroupNode->name();
			if(strName.Find(_T("Group"))>=0)
			{
				xml_node<TCHAR>* pTabNode = pGroupNode->first_node();
				while(pTabNode)
				{
					CString strModule;
					xml_attribute<TCHAR>* pAttr = pTabNode->first_attribute(_T("Name"));
					if(pAttr) 
					{
						strModule = (LPCTSTR)pAttr->value();
						pXml->UnLock();
						OpenModuleByName(strModule);

						if( bNewMDITabbedGroup && iGroupAlign>0 )
						{
							MDITabNewGroup(bVert);
							bNewMDITabbedGroup = FALSE;
						}
						pXml->LockToRead();
						if(m_pViewLastCreated)
						{
							m_pViewLastCreated->SendMessage(WM_SET_WKSDATA, eTabbedView, (LPARAM)pTabNode);
						}
					}

					pTabNode = pTabNode->next_sibling();
				}

				bNewMDITabbedGroup = TRUE;
			}

			pGroupNode = pGroupNode->next_sibling();
		}
	}



	//DockablePane
	typedef xml_node<TCHAR> NodeT;
	typedef NodeT* NodePtrT;
	NodeT*  pNodePane = pXml->FindChild(_T("Workspace\\DockablePane"));
	if(pNodePane)
	{
		NodeT* pNodeModule = pNodePane->first_node();
		while(pNodeModule)
		{
			//Open Module first
			CString strModule = (LPCTSTR)pNodeModule->name();
			BOOL bRet = TRUE;
			if(theApp.GetNumOfView(strModule)==0)
			{
				pXml->UnLock();
				bRet = OpenModuleByName(strModule);
				pXml->LockToRead();
			}

			if(!bRet)
			{
				CString strError;
				strError.Format(_T("Failed to Open module %s"), strModule);
				AfxMessageBox(strError);
				break;
			}


			// Get InitPaneByWorkspace function from dll
			LPFN_DLLINITPANEBYWORKSPACE pFunc = NULL;
			for (POSITION pos  = m_AllCommands.GetHeadPosition(); pos !=NULL; )
			{
				CMenuCommand &mc = m_AllCommands.GetNext(pos);
				if(!mc.m_bUtility && mc.m_strTitle.CompareNoCase(strModule)==0)
				{
					pFunc = (LPFN_DLLINITPANEBYWORKSPACE)GetProcAddress(mc.hLib, "InitPaneByWorkspace");
					break;
				}
			}
			if(!pFunc)
			{
				CString strError;
				strError.Format(_T("Failed to find InitPaneByWorkspace function in %s.dll"), strModule);
				break;
			}


			// call InitPaneByWorkspace function ......
			NodeT* pNodeDashboards = pNodeModule->first_node();
			if(pNodeDashboards)
			{
				CString strEr;
				strEr.Format(_T("Calling %s InitPaneByWorkspace function..."), strModule);
				bRet = FALSE;

				try
				{
					pXml->UnLock();
					bRet = pFunc(pNodeDashboards);
					pXml->LockToRead();
				}
				catch (CException* e)
				{
					CString strError;
					e->GetErrorMessage(strError.GetBuffer(256), 256);
					strError.ReleaseBuffer();
					strEr.Format(_T("Exception from InitPaneByWorkspace, error: %s"), strError.GetString());
					AfxMessageBox(strEr);
					break;
				}
				catch(...)
				{
					strEr.Format(_T("Unknown exception from InitPaneByWorkspace"));
					AfxMessageBox(strEr);
					break;
				}
			}
			pNodeModule = pNodeModule->next_sibling();
		}

	}


	CXmlConfig::Instance()->UnLock();


	RecalcLayoutEx();

	return TRUE;
}

void CMainFrame::RecalcLayoutEx()
{
	vector<CMFCTabCtrl*> vecTabCtrls;
	const int nGroupNum = GetMDITabCtrls(vecTabCtrls);
	if( nGroupNum > 1 )
	{
		const CMyMDIClientAreaWnd* pClientArea = (const CMyMDIClientAreaWnd*) &m_wndClientArea;    

		CRect rc;
		pClientArea->GetClientRect(&rc);
		for(int i=0; i<nGroupNum; ++i)
		{
			CMFCTabCtrl* pTabCtrl = vecTabCtrls.at(i);
			CRect rect;
			pTabCtrl->GetClientRect(&rect);
			if(pClientArea->IsVertAlign())
			{
				rect.right = rc.right/nGroupNum;
			}
			else
			{
				rect.bottom = rc.bottom/nGroupNum;
			}

			pTabCtrl->MoveWindow(&rect);
		}
	}

	__super::RecalcLayout();
}

BOOL CMainFrame::OpenModuleByName(CString strModuleName, bool onStartup /*=true*/)
{
	for (POSITION pos  = m_AllCommands.GetHeadPosition(); pos !=NULL; )
	{
		CMenuCommand &mc = m_AllCommands.GetNext(pos);
		if(mc.m_strTitle.CompareNoCase(strModuleName)==0)
		{
			UINT nID = mc.m_nMenuID + BASE_MODULES_MENU_ID;
			return OpenModule(nID, onStartup);
		}
	}

	return FALSE;
}

BOOL CMainFrame::OpenModule( UINT nID, bool onStartup /*=true*/ )
{
	for (POSITION pos  = m_AllCommands.GetHeadPosition(); pos !=NULL; )
	{
		CMenuCommand &mc = m_AllCommands.GetNext(pos);
		if (!mc.m_bUtility && mc.m_nMenuID == nID-BASE_MODULES_MENU_ID )
		{
			if( !mc.hLib )
			{
				CString strPath = CFileHelper::GetModuleDir();
				mc.hLib = LoadLibrary(strPath+_T("\\")+mc.m_strDll);
			}

			if( !mc.hLib )
			{
				CString strError;

				strError.Format(_T("Load %s failed. ErrorCode:%u"), mc.m_strDll, GetLastError());
				return FALSE;
			}

			if( !mc.m_bInitialized)
			{
				LPDLLFUNC *pInit = (LPDLLFUNC*)GetProcAddress(mc.hLib, "Init");
				if(pInit)
					pInit(0);
				mc.m_bInitialized = true;
			}

			if( mc.m_bHasView && !mc.m_strTitle.IsEmpty())
			{
				BeginWaitCursor();
				CIntaffCommonResourceHandle resHandle(mc.m_strDll, __FILEW__, __LINE__);
				m_pViewLastCreated = theApp.OpenView(mc.m_strTitle, mc.m_bSingleView);
				EndWaitCursor();
				m_mapViewRegister[m_pViewLastCreated] = mc.m_strTitle;
			}


			LPDLLFUNC* pFunc = (LPDLLFUNC*)GetProcAddress(mc.hLib, "LoadModulePane");
			if(pFunc)
				pFunc(0);

			if (!onStartup)	this->EnumTabbedView();

			return TRUE;
		}

	}

	return FALSE;
}

void CMainFrame::EnumTabbedView()
{
	CString strTemp;
	CXmlConfig* pXml = CXmlConfig::Instance();
	if(pXml==NULL)
		return;

	xml_document<TCHAR>* pDoc = pXml->GetDocument();

	//MakeNode <Workspace\\TabbedView>
	pXml->RemoveNode(_T("Workspace\\TabbedView"));
	pXml->SetAttributeBool(_T("Workspace\\TabbedView"), false);
	xml_node<TCHAR>* pWksNode = pXml->FindChild(_T("Workspace\\TabbedView"));


	//MakeNode<Alignment>
	const CMyMDIClientAreaWnd* pClientArea = (const CMyMDIClientAreaWnd*) &m_wndClientArea;
	int iAlign = pClientArea->IsAlign()? (pClientArea->IsVertAlign()?1:2):0;
	strTemp.Format(_T("%d"), iAlign);
	TCHAR* pchName = pDoc->allocate_string(_T("Alignment"));
	TCHAR* pchValue= pDoc->allocate_string(strTemp.GetString());
	xml_node<TCHAR>* pNodeAlign = pWksNode->set_node(pchName, pchValue);


	//MakeNode<TabbedGroup>
	vector<CMFCTabCtrl*> vecTabCtrls;
	const int nGroupNum = GetMDITabCtrls(vecTabCtrls);
	for(int i=0; i<nGroupNum; ++i)
	{
		vector<CWorkspaceData>& vecWksData = m_workSpace.m_mapWks[i];
		CMFCTabCtrl* pTabCtrl = vecTabCtrls[i];
		const int nTabsNum = pTabCtrl->GetTabsNum();
		for(int n=0; n<nTabsNum; ++n)
		{
			CWnd* pWnd = pTabCtrl->GetTabWnd(n);
			if(!pWnd->GetSafeHwnd())
				continue;

			CFrameWnd* pFrame = dynamic_cast<CFrameWnd*>(pWnd);
			if(!pFrame||!(pFrame->GetActiveView()))
				continue;

			CView* pView = pFrame->GetActiveView();
			map<CView*, CString>::const_iterator it = m_mapViewRegister.find(pView);
			if(it!=m_mapViewRegister.end())
			{
				CString strViewName = it->second;
				const CMenuCommand* pMenuCmd = GetMenuCommand(strViewName);
				if(pMenuCmd && !pMenuCmd->m_bLoadAtStartup)
				{
					continue;
				}

				CString strTab;
				strTab.Format(_T("Workspace\\TabbedView\\Group%d\\Tab%d"), i, n);
				pXml->SetAttributeBool(strTab.GetString(), false);

				pXml->LockToWrite();
				xml_node<TCHAR>* pTabNode = pXml->FindChild(strTab);
				xml_attribute<TCHAR>* pAttr = pTabNode->document()->allocate_attribute(_T("Name"),strViewName.GetString());
				pTabNode->append_attribute(pAttr);

				try
				{
					pView->SendMessage(WM_GET_WKSDATA, 0, (LPARAM)pTabNode);
				}
				catch (...)
				{
					pXml->RemoveNode(strTab.GetString());
				}

				pXml->UnLock();
			}
		}
	}

	pXml->FlushData();
}


int CMainFrame::GetMDITabCtrls(vector<CMFCTabCtrl*>& vecTabCtrls)
{
	const CObList& obList = GetMDITabGroups();
	POSITION pos = obList.GetHeadPosition();
	while(pos)
	{
		const CObject* pOb = obList.GetNext(pos);
		CMFCTabCtrl* pTab = DYNAMIC_DOWNCAST(CMFCTabCtrl, pOb);
		if(pTab)
		{
			vecTabCtrls.push_back(pTab);
		}
	}

	return vecTabCtrls.size();
}

const CMenuCommand* CMainFrame::GetMenuCommand( LPCTSTR lpszModuleName )
{
	for (POSITION pos  = m_AllCommands.GetHeadPosition(); pos !=NULL; )
	{
		const CMenuCommand &mc = m_AllCommands.GetNext(pos);
		if(mc.m_strTitle.CompareNoCase(lpszModuleName)==0)
			return &mc;
	}

	return NULL;
}