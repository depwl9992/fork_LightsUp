#if !defined(AFX_DELLAMPTYPE_H__6E950819_EBEA_40DB_91C0_CA5D564A9624__INCLUDED_)
#define AFX_DELLAMPTYPE_H__6E950819_EBEA_40DB_91C0_CA5D564A9624__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DelLampType.h : header file
//

#include "LightingDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CDelLampType dialog

class CDelLampType : public CDialog
{
// Construction
public:
	CDelLampType(LampTypes &types, CWnd* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDelLampType)
	enum { IDD = IDD_LAMP_TYPE_DEL };
	CComboBox	m_TypeList;
	//}}AFX_DATA

	LampTypes	&m_Types;
	CString		m_Name;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDelLampType)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDelLampType)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELLAMPTYPE_H__6E950819_EBEA_40DB_91C0_CA5D564A9624__INCLUDED_)
