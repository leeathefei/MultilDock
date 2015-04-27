
#pragma once
#include "FileView.h"
#include "BaseDlg.h"
#include <vector>
#include <string>
#include "CommDefine.h"
#include "Workspace.h"


using namespace std;

struct stBasePane
{
	BOOL    bShow;
	CString strPaneTitle;
	EDLGTYPE eType;
	CBaseDlg* pBaseDlg;

	stBasePane()
	{
		bShow = TRUE;
		pBaseDlg = NULL;
	}
};
typedef vector<stBasePane> VecBasePanes;

class COutlookBar : public CMFCOutlookBar
{
	virtual BOOL AllowShowOnPaneMenu() const { return TRUE; }
	virtual void GetPaneName(CString& strName) const 
	{ 
		BOOL bNameValid = strName.LoadString(IDS_OUTLOOKBAR); ASSERT(bNameValid); 
		if (!bNameValid) strName.Empty(); 
	}
};

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();


public:
	void LoadModuleMenuItems();
	void OpenStartupModules();
	void OpenLastUsedModules();
	BOOL StartupAsWorkspace();
	void RecalcLayoutEx();
	
protected:
	void CacheMenus();
	void AddModuleMenuItem(const CString &strModuleName, int nID, BOOL bIsView /*=FALSE*/, BOOL bIsUtility /*=FALSE*/);
	BOOL OpenModuleByName(CString strModuleName, bool onStartup = true);
	int  GetMDITabCtrls(vector<CMFCTabCtrl*>& vecTabCtrls);
	BOOL OpenModule(UINT nID, bool onStartup = true);
	void EnumTabbedView();
	const CMenuCommand* GetMenuCommand(LPCTSTR lpszModuleName);

public:
	virtual ~CMainFrame();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, 
		DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, 
		CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	

	class CMyMDIClientAreaWnd : public CMDIClientAreaWnd
	{
	public:
		BOOL IsAlign() const {return m_groupAlignment==0? FALSE:TRUE;};
		BOOL IsVertAlign() const {return m_groupAlignment==1? TRUE:FALSE; }
		BOOL IsHorzAlign() const {return m_groupAlignment==2? TRUE:FALSE; }
		int  GetAlignment() const {return m_groupAlignment;}
	};

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public: 
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CMFCRibbonBar     m_wndRibbonBar;
	CFileView         m_wndFileView;
	COutlookBar       m_wndNavigationBar;
	VecBasePanes	  m_vecUserDlgs; 

	CMenuCommandList  m_AllCommands;
	CView*            m_pViewLastCreated;
	map<CView*, CString> m_mapViewRegister;
	CWorkspace        m_workSpace;

protected:
	afx_msg LRESULT OnInitModulePanes(WPARAM W, LPARAM L);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRibbonPane1Cmd1();
	afx_msg void OnRibbonPane1Cmd2();
	afx_msg void OnRibbonPane1Cmd3();
	afx_msg void OnRibbonPane1Cmd4();
	afx_msg void OnRibbonPane2Cmd1();
	afx_msg void OnRibbonPane2Cmd2();
	afx_msg void OnRibbonPane2Cmd3();
	afx_msg void OnRibbonPane2Cmd4();

	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	afx_msg void OnClose();
	
	
	

	BOOL CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, int nInitialWidth);
	BOOL CreateFileView();
	void SetDockingWindowIcons(BOOL bHiColorIcons);

	DECLARE_MESSAGE_MAP()
};


