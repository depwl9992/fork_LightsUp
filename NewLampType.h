#if !defined(AFX_NEWLAMPTYPE_H__7DC110EF_17E5_4A17_88D5_6976A52C1D01__INCLUDED_)
#define AFX_NEWLAMPTYPE_H__7DC110EF_17E5_4A17_88D5_6976A52C1D01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewLampType.h : header file
//

#include "LightingDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CNewLampType dialog

class CNewLampType : public CDialog
{
// Construction
public:
	CNewLampType(LampTypes &types, CWnd* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewLampType)
	enum { IDD = IDD_NEW_LAMP_TYPE };
	CComboBox	m_NameList;
	//}}AFX_DATA

	LampTypes	&m_Types;
	CString		m_Name;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewLampType)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewLampType)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWLAMPTYPE_H__7DC110EF_17E5_4A17_88D5_6976A52C1D01__INCLUDED_)
