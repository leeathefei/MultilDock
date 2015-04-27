#pragma once

// CSampleViewerDoc document

class CSampleViewerDoc : public CDocument
{
	DECLARE_DYNCREATE(CSampleViewerDoc)

public:
	CSampleViewerDoc();
	virtual ~CSampleViewerDoc();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
