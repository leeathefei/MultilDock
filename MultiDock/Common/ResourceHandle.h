#pragma once
#include <yvals.h>

class CIntaffCommonResourceHandle
{
public:
   HINSTANCE   hRscInstance;
   HINSTANCE   hOldInstance;

   CIntaffCommonResourceHandle()
      : hOldInstance(0)
   {
      ASSERT( FALSE); // this method is obsolit, mmicomress is depricated
      hRscInstance = ::GetModuleHandle(_T("MMICOMRES.DLL"));
      if (hRscInstance)
      {
         hOldInstance = AfxGetResourceHandle();
         AfxSetResourceHandle(hRscInstance);
      }
      else
      {
         CString sError;
         sError.Format( _T("Failed to change resource handle\nSee system error ID: %4li at WINERROR.H"), (LONG)GetLastError());
         AfxMessageBox( (LPCTSTR)sError, MB_OK|MB_ICONSTOP );
         ASSERT(FALSE);
      }
   }
   CIntaffCommonResourceHandle( LPCTSTR pszModuleName,  LPCTSTR pszSource, int uiLine)
      : hOldInstance(0)
   {
      hRscInstance = ::GetModuleHandle( pszModuleName );
      if (hRscInstance)
      {
         hOldInstance = AfxGetResourceHandle();
         AfxSetResourceHandle(hRscInstance);
      }
      else
      {	// FORMAT_MESSAGE_FROM_SYSTEM 
         LPVOID lpMsgBuf;
         FormatMessage( 
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            GetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
            (LPTSTR) &lpMsgBuf,
            0,
            NULL 
            );
         CString sError;
         sError.Format( _T("Failed to change resource handle\n(System error: %s )\nMudule name: %s\nSource Files:%s, %i"), 
            (LPCTSTR)lpMsgBuf, pszModuleName, pszSource, uiLine);
         // Free the buffer.
         LocalFree( lpMsgBuf );
         ::MessageBox( NULL, (LPCTSTR)sError, _T("Error in - Use Custom Resource") , MB_OK|MB_ICONSTOP );
         ASSERT(FALSE);
      }
   }
   ~CIntaffCommonResourceHandle()
   {
      if (hOldInstance)
         AfxSetResourceHandle(hOldInstance);
   }
};


#define USE_CUSTOM_RESOURCE( pszModuleName ) \
   CIntaffCommonResourceHandle CommonResourceHandle( pszModuleName, __FILEW__, __LINE__);

