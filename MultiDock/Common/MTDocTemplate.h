#pragma once
#include "GeneralMacroDefine.h"

class  CMTDocTemplate : public CMultiDocTemplate  
{
   DECLARE_DYNAMIC(CMTDocTemplate)

public:
   CMTDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass, const CString &strDllName, const CString &strTemplateName);
   virtual ~CMTDocTemplate();

public:
   void LoadTemplate();
   CDocument* OpenDocumentFile(LPCTSTR lpszPathName, LPCTSTR lpszDocTitle = NULL, BOOL bMakeVisible = TRUE);

   CString m_ModuleName;
   CString m_TemplateName;
   UINT m_nIDResource;

private:
   void AppendMenu(CMenu *pFirst, CMenu* pSecond, BOOL bMain = FALSE);
   void DuplicateMenu(const CMenu *pSource, CMenu *pTarget);
};