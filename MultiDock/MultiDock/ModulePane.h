#pragma once


// CModulePane

struct MODULE_WINDOW_DEF;

class CModulePane : public CDockablePane
{
	DECLARE_DYNAMIC(CModulePane)

public:
	CModulePane();
	virtual ~CModulePane();
   enum PANE_ALIGNMENT{
      ALIGN_NONE,
      ALIGN_HORIZONTAL,
      ALIGN_VERTICAL
   };
   
   CString  m_strWndName;

   void InitPane(MODULE_WINDOW_DEF* pDef);
	bool IsInitialized() { return m_bInitialized; }
   static CModulePane* GetActivePane();
   static CWnd*  GetWndInActivePane();
   static void SetActivePane(CDockablePane* pActivePane);
   
protected:
	DECLARE_MESSAGE_MAP()
   afx_msg void OnClose();

public:
   
   CWnd*    m_pWnd;
   bool     m_bInitialized;
   bool     m_bAutoDelete;
   PANE_ALIGNMENT m_Alignment;
   static  CModulePane* ms_pActivePane;
public:
   afx_msg void OnSize(UINT nType, int cx, int cy);
   afx_msg void OnDestroy();

protected:
   virtual BOOL OnShowControlBarMenu( CPoint point );
public:
   virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#define WM_AFTER_RESIZE             (WM_USER+12)
#define MIN_HORIZONTAL_BAR_WIDTH    (795)
#define MIN_HORIZONTAL_BAR_HIEGHT   (210)
#define MIN_VERTICAL_BAR_WIDTH      (226)
#define MIN_VERTICAL_BAR_HIEGHT     (540)
