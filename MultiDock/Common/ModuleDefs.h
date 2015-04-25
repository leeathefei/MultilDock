#pragma once



enum EPANE_ALIGNMENT
{
   ALIGN_NON         = CBRS_ALIGN_ANY,
   ALIGN_VERTICAL    = CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT,
   ALIGN_HORIZONTAL  = CBRS_ALIGN_TOP|CBRS_ALIGN_BOTTOM,
};

struct MODULE_WINDOW_DEF {
   CWnd*    pWnd;
   HMENU    hMenu;
   HICON    hIcon;
   LPCTSTR  strWindowName;
   bool     bAutoDelete;
   bool     bAttachToPrev;
   EPANE_ALIGNMENT    nEnabledAlign;


   MODULE_WINDOW_DEF() {
      pWnd = NULL;
      hMenu = NULL;
      hIcon = NULL;
      bAutoDelete = true;
      bAttachToPrev = true;
      nEnabledAlign = ALIGN_VERTICAL;
   }

   enum CONTROL_CODE
   {
      PANE_ADD,
      PANE_REMOVE,
      PANE_SHOW,
      PANE_HIDE,
      PANE_ACTIVATE,
   };
};


struct TOOLBAR_DEF {
   CString strModuleName;
   CString strName;
   UINT uiResID;
   UINT uiColdResID;
   UINT uiDisabledResID;
   UINT uiHotResID;

   TOOLBAR_DEF() 
   {
      uiResID = uiColdResID =  uiDisabledResID = uiHotResID = 0;
   }

   enum CONTROL_CODE
   {
      TOOLBAR_ADD,
      TOOLBAR_REMOVE,
      TOOLBAR_SHOW,
      TOOLBAR_HIDE,
   };

};

typedef	 void(LPDLLFUNC)(WORD);


typedef   void(*PFN_DLLINIT)(WORD);
typedef   PFN_DLLINIT  LPFN_DLLINIT; 

typedef   void(*PFN_DLLTERM)(WORD);
typedef   PFN_DLLTERM  LPFN_DLLTERM; 

typedef   void(*PFN_DLLLOADMODULEPANE)(WORD);
typedef   PFN_DLLLOADMODULEPANE  LPFN_DLLLOADMODULEPANE; 

typedef   BOOL(*PFN_DLLGETICONRESOUCEID)(UINT& nResID);
typedef   PFN_DLLGETICONRESOUCEID  LPFN_DLLGETICONRESOUCEID; 

typedef   BOOL(*PFN_DLLINITPANEBYWORKSPACE)(LPVOID);
typedef   PFN_DLLINITPANEBYWORKSPACE LPFN_DLLINITPANEBYWORKSPACE;

typedef   BOOL(*PFN_DLLGETPANESWORKSPACE)(LPVOID);
typedef   PFN_DLLGETPANESWORKSPACE LPFN_DLLGETPANESWORKSPACE;



//Command ID Range Assigned for all modules 
//Module                                From     To
//LogViewer                          35000    35799
//MSCViewer                        35800    35999
//GraphViewer                     36000    36199
//ASN1Viewer                      36200    36399
//MiniViewer                        36600    36799
//StructureViewer              37000    37199
//TestStudio                          37200    37399
//UEConsole                          37400    37499
//ExtViewer                           37500    38499 
//MultiComm                        38500    38599
//FlashExplorer                     38600    38799
//NvmEditor                          38800    38999
//CommandConsole           39000    39199
//CATAudio                 39200    39699
//ChipLab                  39700    39900
//MapViewer                39900    40400
//RFCalTool                40401    40900
//Netmonitor               40901    41100

#define     WM_SET_WKSDATA       WM_USER+500
#define     WM_GET_WKSDATA       WM_USER+501

enum WKSDATA_CODE
{
   eTabbedView,
   eDashboard,
};



#if defined(_DEBUG) && defined(USE_VLD)
#include <vld.h>
#endif
