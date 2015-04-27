// GraphViewerDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SampleViewer.h"
#include "GraphViewerDialog.h"


// CGraphViewerDialog dialog

IMPLEMENT_DYNAMIC(CGraphViewerDialog, CDialog)

CGraphViewerDialog::CGraphViewerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CGraphViewerDialog::IDD, pParent)
{

}

CGraphViewerDialog::~CGraphViewerDialog()
{
}

void CGraphViewerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGraphViewerDialog, CDialog)
END_MESSAGE_MAP()


// CGraphViewerDialog message handlers
