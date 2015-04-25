
// SampleView.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SampleView.h"

BEGIN_MESSAGE_MAP(CSampleViewApp, CWinApp)
END_MESSAGE_MAP()

CSampleViewApp theApp;

CSampleViewApp::CSampleViewApp()
{
}


BOOL CSampleViewApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};
