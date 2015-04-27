// PaneDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SampleViewer.h"
#include "SampleViewerDialog.h"


// CSampleViewerDialog dialog

IMPLEMENT_DYNAMIC(CSampleViewerDialog, CDialog)

CSampleViewerDialog::CSampleViewerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSampleViewerDialog::IDD, pParent)
{
   m_bPaneClosed = false;
}

CSampleViewerDialog::~CSampleViewerDialog()
{
}

void CSampleViewerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSampleViewerDialog, CDialog)
   ON_WM_CLOSE()
   ON_WM_DESTROY()
END_MESSAGE_MAP()


// CSampleViewerDialog message handlers

void CSampleViewerDialog::OnClose()
{
   // TODO: Add your message handler code here and/or call default
   m_bPaneClosed = true;
   CDialog::OnClose();
}

void CSampleViewerDialog::OnDestroy()
{
   CDialog::OnDestroy();

   // TODO: Add your message handler code here
}

BOOL CSampleViewerDialog::DestroyWindow()
{
   // TODO: Add your specialized code here and/or call the base class

   return CDialog::DestroyWindow();
}

void CSampleViewerDialog::PostNcDestroy()
{
   // TODO: Add your specialized code here and/or call the base class

   CDialog::PostNcDestroy();
}
