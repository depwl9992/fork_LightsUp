#if !defined(AFX_MAPEDITOR_H__5EC75197_68B2_4231_8457_3021237CBE07__INCLUDED_)
#define AFX_MAPEDITOR_H__5EC75197_68B2_4231_8457_3021237CBE07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapEditor.h : header file
//

#include "LightingDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CMapEditor dialog

class CMapEditor : public CDialog
{
// Construction
public:
	CMapEditor(CChannel *map, CWnd* pParent);

// Dialog Data
	//{{AFX_DATA(CMapEditor)
	enum { IDD = IDD_MAP_EDITOR };
	CEdit	m_MapAddresses;
	CEdit	m_MapChannel;
	CListCtrl	m_mapList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapEditor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CChannel *m_map;
	int	m_selItem;
	int m_errorItem;

	// Generated message map functions
	//{{AFX_MSG(CMapEditor)
	afx_msg void OnMapClearAll();
	afx_msg void OnMapIdentity();
	afx_msg void OnClickChannelMap(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeMapChannel();
	afx_msg void OnChangeMapAddresses();
	afx_msg void OnKillfocusMapAddresses();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPEDITOR_H__5EC75197_68B2_4231_8457_3021237CBE07__INCLUDED_)
