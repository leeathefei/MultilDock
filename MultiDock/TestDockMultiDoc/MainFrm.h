
#pragma once
#include "FileView.h"
#include "BasePane.h"
#include <vector>
#include <string>

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

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, 
		DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, 
		CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

public:
	virtual ~CMainFrame();
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
	CMFCRibbonApplicationButton m_MainButton;

	CFileView         m_wndFileView;
	COutlookBar       m_wndNavigationBar;
	VecBasePanes	  m_vecUserDlgs;

protected:
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
	DECLARE_MESSAGE_MAP()

	BOOL CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, int nInitialWidth);
	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
};


