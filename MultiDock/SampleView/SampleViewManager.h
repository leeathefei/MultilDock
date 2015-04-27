#pragma once
#include "..\Common\ModuleManager.h"

class CSampleViewDialog;
class CSampleViewManager
{
public:
   CSampleViewManager(void);
   ~CSampleViewManager(void);
   
   static CSampleViewManager* Instance();
   static CSampleViewManager* ms_pInstance;


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

  
   CSampleViewDialog* m_pModuleDlg[4];
   CString   m_strModuleName[4];
   bool      m_bAutoDelete[4];
   EPANE_ALIGNMENT   m_dwAlign[4];


   //�����ӿ��ʹ�õ���ͬһ����������������Ҫ�����ӿ�ܵĸ���ά��һ��
   //�����������ü������Ӷ���֤����ȷ��ʱ����ʾ���غ��ͷŹ�������Դ
   //���dll����Ҫ�������Ͳ���Ҫ�˳�Ա����Ӧ�Ĺ����������ĺ�����
   CList<CFrameWnd*, CFrameWnd*> m_InstanceList;
   CMFCToolBar*    m_pToolbar;
   int m_iToolbarRefCnt;

};