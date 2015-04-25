

#pragma once

class CSampleViewFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CSampleViewFrame)

public:
	CSampleViewFrame();
	virtual ~CSampleViewFrame();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};



