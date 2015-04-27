#pragma once
#include "afx.h"
#include "XmlConfig.h"
#include <map>
#include <vector>
#include <list>
using namespace std;



class CWorkspaceBase
{
public:
   CWorkspaceBase(){};
   virtual ~CWorkspaceBase(){};

   virtual BOOL SetWorkspace(const xml_node<TCHAR>* pWorkspaceNode) {return FALSE;}
   virtual BOOL GetWorkspace(xml_node<TCHAR>* pWorkspaceNode) {return FALSE;}
};

class CWorkspaceData
{
public:
   CString strWorkspaceName;
   xml_node<TCHAR>* pWorkspaceData;
};

class CWorkspace : public CWorkspaceBase
{
public:
   CWorkspace(void){};
   virtual ~CWorkspace(void){};
   enum GROUP_ALIGNMENT
   {
      GROUP_NO_ALIGN,
      GROUP_VERT_ALIGN,
      GROUP_HORZ_ALIGN
   };

   


   map<int, vector<CWorkspaceData>> m_mapWks;
   GROUP_ALIGNMENT           m_eGroupAlign;
};
