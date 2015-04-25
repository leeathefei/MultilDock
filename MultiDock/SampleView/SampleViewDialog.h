#pragma once
#include "Resource.h"

// CSampleViewDialog dialog

class CSampleViewDialog : public CDialog
{
	DECLARE_DYNAMIC(CSampleViewDialog)

public:
	CSampleViewDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSampleViewDialog();

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
