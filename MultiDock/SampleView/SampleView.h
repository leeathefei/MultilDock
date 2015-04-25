
// SampleView.h : main header file for the SampleView application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CSampleViewApp:
// See SampleView.cpp for the implementation of this class
//

class CSampleViewApp : public CWinApp
{
public:
	CSampleViewApp();

public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CSampleViewApp theApp;
