// SampleViewer.h : main header file for the SampleViewer DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSampleViewerApp
// See SampleViewer.cpp for the implementation of this class
//

class CSampleViewerApp : public CWinApp
{
public:
	CSampleViewerApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
