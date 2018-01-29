#if !defined(AFX_SCENEDLG_H__E6818B87_44C3_4DF3_BE94_499274292816__INCLUDED_)
#define AFX_SCENEDLG_H__E6818B87_44C3_4DF3_BE94_499274292816__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SceneDlg.h : header file
//

class CLightingDoc;
class CLightingView;

/////////////////////////////////////////////////////////////////////////////
// CSceneDlg dialog

class CSceneDlg : public CDialog
{
// Construction
public:
	CSceneDlg();   // standard constructor
	BOOL Create(CLightingView *pView);

	CLightingDoc	*m_Doc;

// Dialog Data
	//{{AFX_DATA(CSceneDlg)
	enum { IDD = IDD_SCENE_DLG };
	CComboBox	m_List;
	//}}AFX_DATA

	void SetSliderLabel(int channel, CString &str) {
		m_SliderLabel[channel].SetWindowText(str);
	}
	LPCTSTR GetSliderLabel(int channel) {
		static char	text[sliderLabelMax + 1];
		m_SliderLabel[channel].GetLine(0, text, sliderLabelMax);
		return text;
	}
	void SetSliderPos(int channel, int pos) {
		pos = min(max(pos, 0), dmxMaxVal);
		m_Slider[channel].SetPos(dmxMaxVal - pos);
	}
	int GetSliderPos(int channel) {
		return dmxMaxVal - m_Slider[channel].GetPos();
	}

protected:
	int		m_SliderWidth;
	int		m_SliderHeight;
	int		m_SliderPitch;
	int		m_LabelHeight;
	int		m_BankPitch;
	CRect	m_SliderRect;
	CPoint	m_SliderEdgePad;
	CFont	m_Font;

	CSliderCtrl m_Slider[numSliders];
	CEdit	m_SliderLabel[numSliders];

	int		m_ChannelVal[numSliders];
	CString	m_ChannelName[numSliders];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSceneDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CLightingView	*m_View;

	void UpdateLooks(int sel = -1);

	// Generated message map functions
	//{{AFX_MSG(CSceneDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnEditChannelMap();
	afx_msg void OnEditCurves();
	afx_msg void OnSelchangeLookList();
	afx_msg void OnLookSet();
	afx_msg void OnLookDel();
	afx_msg void OnKillfocusLookList();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCENEDLG_H__E6818B87_44C3_4DF3_BE94_499274292816__INCLUDED_)
