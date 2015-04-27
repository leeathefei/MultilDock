#pragma once
#include "..\common\ModuleManager.h"

class CSampleViewerDialog;
class CSampleViewerManager
{
public:
   CSampleViewerManager(void);
   ~CSampleViewerManager(void);
   
   static CSampleViewerManager* Instance();
   static CSampleViewerManager* ms_pInstance;


public:
   BOOL RegisterDocTemplate();
   BOOL RegisterToolBar();
   void RemoveToolBar();
   void ShowToolBar(BOOL bShow=TRUE);
   void ActivatePane(CString strWindowName);
   BOOL RegisterModulePane();
   BOOL UnregisterModulePane(LPCTSTR);
   BOOL CanClose(CString& strMessage);
   void Terminate();

public:

   void AddFrameWnd(CFrameWnd* pFrmaeWnd);
   void RemoveFrameWnd(CFrameWnd* pFrmeWnd);

private:
   CModuleManager* m_pModuleManager;

  
   CSampleViewerDialog* m_pModuleDlg[4];
   CString   m_strModuleName[4];
   bool      m_bAutoDelete[4];
   EPANE_ALIGNMENT   m_dwAlign[4];


   //各个子框架使用的是同一个工具条，所以需要根据子框架的个数维护一个
   //工具条的引用计数，从而保证在正确的时候，显示隐藏和释放工具条资源
   //如果dll不需要工具条就不需要此成员和相应的工具条操作的函数。
   CList<CFrameWnd*, CFrameWnd*> m_InstanceList;
   CMFCToolBar*    m_pToolbar;
   int m_iToolbarRefCnt;

};
