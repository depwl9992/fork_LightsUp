#if !defined(AFX_LOOKEDIT_H__9F00BD3D_CE49_4B0B_A7BE_7CA32494C8DB__INCLUDED_)
#define AFX_LOOKEDIT_H__9F00BD3D_CE49_4B0B_A7BE_7CA32494C8DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LookEdit.h : header file
//

#include "LightingDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CLookEdit dialog

class CLookEdit : public CDialog
{
// Construction
public:
	enum EditLookType {SetLook, DelLook} ;
	CLookEdit(Cues &cues, CWnd* pParent, enum EditLookType type);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CLookEdit)
	enum { IDD = IDD_LOOK_EDIT };
	CComboBox	m_List;
	CString	m_Prompt;
	//}}AFX_DATA

	CString		m_Name;
	Cues		&m_Looks;
	enum EditLookType	m_Type;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLookEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLookEdit)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOOKEDIT_H__9F00BD3D_CE49_4B0B_A7BE_7CA32494C8DB__INCLUDED_)
