// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

#pragma once
#include"resource.h"
#include"DlgTest1.h"
#include"DlgTest2.h"
#include"DlgTest3.h"
#include"DlgTest4.h"

/////////////////////////////////////////////////////////////////////////////
// CBaseDlg window

enum EDLGTYPE
{	
	enmDlgType_Test1 = 1,
	enmDlgType_Test2 = 2,
	enmDlgType_Test3 = 3,
	enmDlgType_Test4 = 4,
};

class CBaseDlg : public CWnd
{
public:
	CBaseDlg();

protected:
	CDialogEx* m_pBaseDlg;
	int m_nMyCalendarsY;
	CImageList m_Images;
	EDLGTYPE m_eDlgType;

// Overrides
public:
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, EDLGTYPE eType, UINT nID = (UINT)-1);

// Implementation
public:
	virtual ~CBaseDlg();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};
