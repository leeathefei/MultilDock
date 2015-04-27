// SampleViewerDoc.cpp : implementation file
//

#include "stdafx.h"
#include "SampleViewer.h"
#include "SampleViewerDoc.h"


// CSampleViewerDoc

IMPLEMENT_DYNCREATE(CSampleViewerDoc, CDocument)

CSampleViewerDoc::CSampleViewerDoc()
{
}

BOOL CSampleViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

   m_strTitle = _T("Sample Viewer");

	return TRUE;
}

CSampleViewerDoc::~CSampleViewerDoc()
{
}


BEGIN_MESSAGE_MAP(CSampleViewerDoc, CDocument)
END_MESSAGE_MAP()


// CSampleViewerDoc diagnostics

#ifdef _DEBUG
void CSampleViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CSampleViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CSampleViewerDoc serialization

void CSampleViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
#endif


// CSampleViewerDoc commands
