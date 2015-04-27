//////////////////////////////////////////////////////////////////////////
// MTDocTemplate.cpp: implementation of the CMTDocTemplate class.

#include "stdafx.h"
#include "MTDocTemplate.h"
#include "ResourceHandle.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CMTDocTemplate, CMultiDocTemplate)

CMTDocTemplate::CMTDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass, const CString &strDllName, const CString &strTemplateName) :
CMultiDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
{
   m_nIDResource = nIDResource;
   m_ModuleName = strDllName;
   m_TemplateName = strTemplateName;
}

CMTDocTemplate::~CMTDocTemplate()
{

}


void CMTDocTemplate::LoadTemplate()
{
   CMultiDocTemplate::LoadTemplate();

   TCHAR tcName[300];
   GetModuleFileName(NULL, tcName, 300);
   CString strExeName(tcName);
   int nIdx = strExeName.ReverseFind('\\');
   strExeName = strExeName.Mid(nIdx+1);

   USE_CUSTOM_RESOURCE(strExeName);

   CFrameWnd *pMainFrame = (CFrameWnd*)AfxGetMainWnd();
   CMenu* pMainMenu = (CMenu*)pMainFrame->SendMessage(WM_USER+100, 0, 0);
   
   CMenu NewMenu;
   NewMenu.CreatePopupMenu();
   DuplicateMenu(pMainMenu, &NewMenu);


   CMenu* pViewMenu = CMenu::FromHandle(this->m_hMenuShared);
   if ( pViewMenu )
   {
      AppendMenu(&NewMenu, pViewMenu, TRUE);
      pViewMenu->DestroyMenu();
   }
   
   this->m_hMenuShared = NewMenu.Detach();
   pMainMenu->Detach();
}

//////////////////////////////////////////////////////////////////////////
// nID   
//    -1       subMenu (e.g. Modules)
//    0        seperator in submenu
//    others   menuItem in submenu  (e.g. Viewer)
//    
// pSource 
//    Main frame menu
//    
// pTarget
//    View menu, from DLL
//    
// pSource duplicate to pTarget
//    采用迭代的方式复制（InsertMenu函数）
//    从pSource 的第一个子菜单至最后一个子菜单
//    从子菜单的第一个MenuItem 至子菜单的最后一个MenuItem
//    
// InsertMenu函数
//    InsertMenu(nPosition, nFlags, nNewMenuItemID, strNewMenuItemName)
//    nFlags
//       MF_BYPOSITION, MF_BYCOMMAND
//       MF_STRING, MF_BITMAP, MF_OWNERDRAW
//       MF_SEPERATOR
//       MF_INSERT, MF_CHANGE, MF_APPEND, MF_DELETE, MF_REMOVE
//       MF_ENABLE, MF_GRAYED, MF_DISABLE
//       MF_UNCHECKED, MF_CHECKED, MF_USECHECKBITMAP
//  

#define MAINMENU_ITEMNUM_LEFT   4
#define MAINMENU_ITEMNUM_RIGHT  4

void CMTDocTemplate::DuplicateMenu(const CMenu *pSource, CMenu *pTarget)
{
   int count = pSource->GetMenuItemCount();
   CString strMenuItem;
   for (int i=0; i<count; i++)
   {
      pSource->GetMenuString(i, strMenuItem, MF_BYPOSITION);
      UINT nID = pSource->GetMenuItemID(i);
      if (nID > 0 && nID != (UINT)(-1))
         pTarget->InsertMenu(i, MF_BYPOSITION | MF_STRING, nID, strMenuItem);
      else if (nID == 0) // seperator
         pTarget->InsertMenu(i, MF_BYPOSITION | MF_SEPARATOR , 0, _T(""));
      else if (nID == (UINT)(-1))
      {
         if( i<MAINMENU_ITEMNUM_LEFT || i>=count-MAINMENU_ITEMNUM_RIGHT )
         {
            CMenu subMenu;
            subMenu.CreatePopupMenu();
            DuplicateMenu(pSource->GetSubMenu(i), &subMenu);
            pTarget->InsertMenu(i, MF_BYPOSITION | MF_POPUP, (UINT)subMenu.Detach(), strMenuItem);
         }
     }
   }
}


void CMTDocTemplate::AppendMenu(CMenu *pFirst, CMenu *pSecond, BOOL bMain /*=FALSE*/)
{
   CString strMenuItem;
   for (UINT i=0; i<pSecond->GetMenuItemCount(); i++)
   {
      pSecond->GetMenuString(i, strMenuItem, MF_BYPOSITION);
      UINT nID = pSecond->GetMenuItemID(i);
      if (nID > 0 && nID != (UINT)(-1))
      {
         if ( bMain )
            pFirst->InsertMenu(pFirst->GetMenuItemCount()-MAINMENU_ITEMNUM_RIGHT, MF_BYPOSITION | MF_STRING, nID, strMenuItem);
         else
            pFirst->AppendMenu(MF_BYPOSITION | MF_STRING, nID, strMenuItem);
      }
      else if (nID == 0) // seperator
      {
         if ( bMain )
            pFirst->InsertMenu(pFirst->GetMenuItemCount()-MAINMENU_ITEMNUM_RIGHT, MF_BYPOSITION | MF_SEPARATOR , 0, _T(""));
         else
            pFirst->AppendMenu(MF_BYPOSITION | MF_SEPARATOR , 0, _T(""));
      }
      else if (nID == (UINT)(-1))
      {
         CMenu subMenu;
         subMenu.CreatePopupMenu();
         AppendMenu(&subMenu, pSecond->GetSubMenu(i));
         if ( bMain )
            pFirst->InsertMenu(pFirst->GetMenuItemCount()-MAINMENU_ITEMNUM_RIGHT, MF_BYPOSITION | MF_POPUP, (UINT)subMenu.Detach(), strMenuItem);
         else
            pFirst->AppendMenu(MF_BYPOSITION | MF_POPUP, (UINT)subMenu.Detach(), strMenuItem);
      }

   }
}


CDocument* CMTDocTemplate::OpenDocumentFile(LPCTSTR lpszPathName, LPCTSTR lpszDocTitle, BOOL bMakeVisible)
{
   CIntaffCommonResourceHandle resouceHandler(m_ModuleName, __FILEW__, __LINE__);
   CDocument* pDocument = CreateNewDocument();
   if (pDocument == NULL)
   {
      TRACE0("CDocTemplate::CreateNewDocument returned NULL.\n");
      AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
      return NULL;
   }
   ASSERT_VALID(pDocument);

   BOOL bAutoDelete = pDocument->m_bAutoDelete;
   pDocument->m_bAutoDelete = FALSE;   // don't destroy if something goes wrong
   CFrameWnd* pFrame = CreateNewFrame(pDocument, NULL);
   pDocument->m_bAutoDelete = bAutoDelete;
   if (pFrame == NULL)
   {
      AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
      delete pDocument;       // explicit delete on error
      pDocument = NULL;
      return NULL;
   }
   ASSERT_VALID(pFrame);

   if (lpszPathName == NULL)
   {
      // create a new document - with default document name
      if ( lpszDocTitle && lpszDocTitle[0] != 0 )
         pDocument->SetTitle(lpszDocTitle);

      // avoid creating temporary compound file when starting up invisible
      if (!bMakeVisible)
         pDocument->m_bEmbedded = TRUE;

      if (!pDocument->OnNewDocument())
      {
         // user has be alerted to what failed in OnNewDocument
         TRACE0("CDocument::OnNewDocument returned FALSE.\n");
         pFrame->DestroyWindow();
         return NULL;
      }

      // it worked, now bump untitled count
      m_nUntitledCount++;
   }
   else
   {
      // open an existing document
      CWaitCursor wait;
      if (!pDocument->OnOpenDocument(lpszPathName))
      {
         // user has be alerted to what failed in OnOpenDocument
         TRACE0("CDocument::OnOpenDocument returned FALSE.\n");
         pFrame->DestroyWindow();
         return NULL;
      }
      pDocument->SetPathName(lpszPathName);
   }

   InitialUpdateFrame(pFrame, pDocument, bMakeVisible);
   return pDocument;
}
