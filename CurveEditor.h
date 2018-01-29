#if !defined(AFX_CURVEEDITOR_H__059DD60A_BB09_461E_A808_A36221005841__INCLUDED_)
#define AFX_CURVEEDITOR_H__059DD60A_BB09_461E_A808_A36221005841__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CurveEditor.h : header file
//

#include "LightingDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCurveEditor dialog

class CCurveEditor : public CDialog
{
// Construction
public:
	CCurveEditor(CLightingDoc *pDoc, CWnd* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCurveEditor)
	enum { IDD = IDD_CURVE_EDITOR };
	CComboBox	m_TypeList;
	CSpinButtonCtrl	m_AddrSpin;
	CSliderCtrl	m_MinSlider;
	CSliderCtrl	m_MaxSlider;
	CSliderCtrl	m_CurveSlider;
	CStatic	m_Graph;
	int		m_Addr;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCurveEditor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CLightingDoc *m_pDoc;
	Curve		*m_DocCurves;

	LampTypes	m_LampTypes;
	Curve		m_Curves[dmxAddrs];

	CBitmap		m_GraphBitmap;
	CSize		m_GraphBitmapSize;
	int			m_nAddr;

	// Generated message map functions
	//{{AFX_MSG(CCurveEditor)
	afx_msg void OnChangeLampAddr();
	virtual BOOL OnInitDialog();
	afx_msg void OnLampTypeSet();
	afx_msg void OnDeltaposLampAddrSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeLampType();
	afx_msg void OnKillfocusLampType();
	afx_msg void OnLampTypeDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void		UpdateGraph();
	void		UpdateLampTypes(int sel = -1);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CURVEEDITOR_H__059DD60A_BB09_461E_A808_A36221005841__INCLUDED_)
