#pragma once
#include "Resource.h"

// CSampleViewerDialog dialog

class CSampleViewerDialog : public CDialog
{
	DECLARE_DYNAMIC(CSampleViewerDialog)

public:
	CSampleViewerDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSampleViewerDialog();

// Dialog Data
	enum { IDD = IDD_PANEDIALOG };

   bool m_bPaneClosed;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnClose();
   afx_msg void OnDestroy();
   virtual BOOL DestroyWindow();
protected:
   virtual void PostNcDestroy();
};
