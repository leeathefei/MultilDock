// PaneDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SampleView.h"
#include "SampleViewDialog.h"


// CSampleViewDialog dialog

IMPLEMENT_DYNAMIC(CSampleViewDialog, CDialog)

CSampleViewDialog::CSampleViewDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSampleViewDialog::IDD, pParent)
{
   m_bPaneClosed = false;
}

CSampleViewDialog::~CSampleViewDialog()
{
}

void CSampleViewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSampleViewDialog, CDialog)
   ON_WM_CLOSE()
   ON_WM_DESTROY()
END_MESSAGE_MAP()


// CSampleViewDialog message handlers

void CSampleViewDialog::OnClose()
{
   // TODO: Add your message handler code here and/or call default
   m_bPaneClosed = true;
   CDialog::OnClose();
}

void CSampleViewDialog::OnDestroy()
{
   CDialog::OnDestroy();

   // TODO: Add your message handler code here
}

BOOL CSampleViewDialog::DestroyWindow()
{
   // TODO: Add your specialized code here and/or call the base class

   return CDialog::DestroyWindow();
}

void CSampleViewDialog::PostNcDestroy()
{
   // TODO: Add your specialized code here and/or call the base class

   CDialog::PostNcDestroy();
}
