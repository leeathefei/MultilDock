#include "StdAfx.h"
#include "SampleViewManager.h"
#include "SampleViewDoc.h"
#include "SampleViewFrame.h"
#include "SampleViewView.h"
#include "SampleViewDialog.h"
#include "SampleFormViewer.h"
#include "Resource.h"

CSampleViewManager* CSampleViewManager::ms_pInstance=NULL;

CSampleViewManager* CSampleViewManager::Instance()
{
   if(!ms_pInstance)
   {
      ms_pInstance = new CSampleViewManager;
   }

   return ms_pInstance;
}

CSampleViewManager::CSampleViewManager(void)
{
   m_pModuleManager = new CModuleManager;

   m_iToolbarRefCnt = 0;
   m_pToolbar = NULL;
   for(int i=0; i<4; ++i)
   {
      m_pModuleDlg[i] = NULL;
   }
   
   m_dwAlign[0] = ALIGN_VERTICAL;
   m_dwAlign[1] = ALIGN_VERTICAL;
   m_dwAlign[2] = ALIGN_HORIZONTAL;
   m_dwAlign[3] = ALIGN_HORIZONTAL;


   m_strModuleName[0] = _T("Vertical AutoDelete Pane");
   m_strModuleName[1] = _T("Vertical Closable Pane");
   m_strModuleName[2] = _T("Horizontal AutoDelete Pane");
   m_strModuleName[3] = _T("Horizontal Closable Pane");

   m_bAutoDelete[0] = true;
   m_bAutoDelete[1] = false;
   m_bAutoDelete[2] = true;
   m_bAutoDelete[3] = false;

}

CSampleViewManager::~CSampleViewManager(void)
{
   delete m_pToolbar;
   m_pToolbar = NULL;

   delete m_pModuleManager;
}

BOOL CSampleViewManager::RegisterDocTemplate()
{
   // 切换到DLL资源
   USE_CUSTOM_RESOURCE(_T("SampleView.dll"));


   // 构造一个文档模板对象
   CCATDocTemplate* pDocTemplate = new CCATDocTemplate(IDR_SAMPLEVIEWER_TYPE,
      RUNTIME_CLASS(CSampleViewDoc),
      RUNTIME_CLASS(CSampleViewFrame),
      RUNTIME_CLASS(CSampleViewView),
      _T("SampleView.dll"), _T("SampleView"));


   // 调用基类的函数实现真正的文档模板注册
   return m_pModuleManager->RegisterDocTemplate(pDocTemplate);
}

BOOL CSampleViewManager::RegisterToolBar()
{
   // 切换到DLL资源
   USE_CUSTOM_RESOURCE(_T("SampleView.dll"));

   CMFCToolBar* pToolbar=NULL;
   if( m_pModuleManager->RegisterToolBar(IDR_SAMPLEVIEWER_TOOLBAR, pToolbar) )
   {
      m_pToolbar = pToolbar;
      return TRUE;
   }
   else
      return FALSE;

}

void CSampleViewManager::RemoveToolBar()
{
   m_pModuleManager->RemoveToolBar(m_pToolbar);
}

void CSampleViewManager::ShowToolBar(BOOL bShow)
{
   m_pModuleManager->ShowToolBar(m_pToolbar, bShow);
}

BOOL CSampleViewManager::RegisterModulePane()
{
   // 切换到DLL资源
   USE_CUSTOM_RESOURCE(_T("SampleView.dll"));


   for(int i=0; i<4; ++i)
   {
      if(m_pModuleDlg[i]==NULL)
         m_pModuleDlg[i] = new CSampleViewDialog(NULL);
      if(m_pModuleDlg[i]->GetSafeHwnd()==NULL)
         m_pModuleDlg[i]->Create(CSampleViewDialog::IDD, NULL);

      HICON hIcon = (HICON)::LoadImage(::AfxGetResourceHandle(), 
         MAKEINTRESOURCE(IDR_SAMPLEVIEWER_TYPE),
         IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);

      m_pModuleManager->RegisterModulePane(
         m_pModuleDlg[i], 
         m_strModuleName[i], 
         hIcon, 
         m_dwAlign[i], 
         true, 
         m_bAutoDelete[i]);
   }

   return TRUE;
}

BOOL CSampleViewManager::CanClose(CString& strMessage/*=CString(_T(""))*/)
{
   USE_CUSTOM_RESOURCE(_T("SampleView.dll"));


   //TODO: 添加代码替换下面一行，或者保留下面一行
   return m_pModuleManager->CanClose(strMessage);
}

void CSampleViewManager::Terminate()
{
   // 切换到DLL资源
   USE_CUSTOM_RESOURCE(_T("SampleView.dll"));
   
   //for(int i=0; i<4; ++i)
   //{
   //   m_pModuleDlg[i]->PostMessage(WM_CLOSE);
   //   m_pModuleDlg[i]->DestroyWindow();
   //   delete m_pModuleDlg[i];
   //}

   //TODO: 添加代码替换下面一行，或者保留下面一行
   m_pModuleManager->Terminate();

}

void CSampleViewManager::AddFrameWnd( CFrameWnd* pFrameWnd )
{
   m_InstanceList.AddTail(pFrameWnd);
}

void CSampleViewManager::RemoveFrameWnd( CFrameWnd* pFrameWnd )
{
   POSITION pos=NULL;
   pos = m_InstanceList.Find(pFrameWnd, pos);
   if (pos)
   {
      m_InstanceList.RemoveAt(pos);
   }

   if( m_InstanceList.IsEmpty() )
   {
      ShowToolBar(SW_HIDE);
   }
}

void CSampleViewManager::ActivatePane( CString strWindowName )
{
   m_pModuleManager->ActivatePane(strWindowName);
}

BOOL CSampleViewManager::UnregisterModulePane(LPCTSTR lpszWndName)
{
   return m_pModuleManager->UnregisterModulePane(lpszWndName);
}








//////////////////////////////////////////////////////////////////////////
// exported functions
// 
extern "C"
{
   void __declspec(dllexport) Init(WORD w)
   {
      CSampleViewManager::Instance()->RegisterDocTemplate();
      CSampleViewManager::Instance()->RegisterToolBar();
   }

   BOOL __declspec(dllexport) GetIconResourceID(UINT& nResID)
   {
      nResID = IDR_SAMPLEVIEWER_TYPE;
      return TRUE;
   }

   void __declspec(dllexport) Term(WORD w)
   {
      CSampleViewManager::Instance()->Terminate();
   }

   BOOL __declspec(dllexport) CanClose(CString& strMessage)
   {
      return TRUE;
   }

   void __declspec(dllexport) LoadModulePane(WORD w)
   {
      CSampleViewManager::Instance()->RegisterModulePane();
   }

};
