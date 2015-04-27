#pragma once
#include "Resource.h"

// CGraphViewerDialog dialog

class CGraphViewerDialog : public CDialog
{
	DECLARE_DYNAMIC(CGraphViewerDialog)

public:
	CGraphViewerDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGraphViewerDialog();

// Dialog Data
	enum { IDD = IDD_GRAPHVIEWER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
