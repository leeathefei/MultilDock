#pragma once



// CSampleFormViewer form view

class CSampleFormViewer : public CFormView
{
	DECLARE_DYNCREATE(CSampleFormViewer)

public:
	CSampleFormViewer();           // protected constructor used by dynamic creation
	virtual ~CSampleFormViewer();

public:
	enum { IDD = IDD_FORMVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

   virtual BOOL Create(LPCTSTR, LPCTSTR, DWORD, const RECT&, CWnd*, UINT, CCreateContext*);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   
	DECLARE_MESSAGE_MAP()
};


