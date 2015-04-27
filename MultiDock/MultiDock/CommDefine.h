/***********************************************************************
	Created:		2015/4/27	11:49
	FileName:		CommDefine.h
	Author:			Chris Lee
	Function:		
************************************************************************/

#pragma once

struct CMenuCommand
{
public:
	UINT	   m_nMenuID;
	CString	m_strTitle;
	CString	m_strDll;
	bool	   m_bAddToMenu;
	bool     m_bUtility;
	bool	   m_bHasView;
	bool	   m_bSingleView;
	bool     m_bLoadAtStartup;
	bool     m_bInitialized;

	HINSTANCE	hLib;
	HICON    m_hIcon;

	CMenuCommand() { 
		hLib = 0; 
		m_hIcon=NULL; 
		m_bAddToMenu = true; 
		m_bUtility = false; 
		m_bHasView = true; 
		m_bSingleView = false; 
		m_bLoadAtStartup = true; 
		m_bInitialized=false;
	}
};


typedef CList<CMenuCommand, CMenuCommand&>   CMenuCommandList;