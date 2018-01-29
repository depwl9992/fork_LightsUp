// LightingView.h : interface of the CLightingView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHTINGVIEW_H__618D217B_2EE6_4083_ACFE_6C58DAB0B41C__INCLUDED_)
#define AFX_LIGHTINGVIEW_H__618D217B_2EE6_4083_ACFE_6C58DAB0B41C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SceneDlg.h"
#include "odmxusb/odmxusb.h"

class CLightingView : public CFormView
{
protected: // create from serialization only
	CLightingView();
	DECLARE_DYNCREATE(CLightingView)

public:
	//{{AFX_DATA(CLightingView)
	enum { IDD = IDD_LIGHTING_FORM };
	CButton	m_dmxFreeze;
	CButton	m_ConnectButton;
	CButton	m_PrevCue;
	CButton	m_NextCue;
	CProgressCtrl	m_FadeProgress;
	CButton	m_FadeCancel;
	CListBox	m_CueList;
	CString	m_ActiveCue;
	CString	m_PreviewCue;
	UINT	m_FadeTime;
	UINT	m_FadeTimePreview;
	CString	m_frozenCueText;
	//}}AFX_DATA

// Attributes
public:
	CLightingDoc* GetDocument();
	CSceneDlg	m_SceneDlg;
	bool		m_SceneVisible;			// Is the scene dialog currently visible
	bool		m_Updating;				// Disable slider input changes during updates
	int			m_Selected;				// Keep track of the currently selected cue

	// Fade state
	int			m_fadeTimer;			// Current fade timer value
	int			m_fadePos;				// fade position
	int			m_fadeSteps;			// fade steps
	int			m_fadeEndCue;			// cue selected at end of fade
	int			m_fadeBase[numSliders];	// fade base values
	double		m_fadeVec[numSliders];	// fade vector

	// Connection state
	bool		m_connected;			// Connected to OpenDMX device
	Open_USB_DMX m_openUsbDMX;			// Open USB DMX device object

	// Freeze state
	int			m_frozenCue;			// The cue that is active when frozen

protected:
	bool		m_HasCueClip;			// There is a cue on the clipboard
	CCue		m_CueClip;				// Cue clipboard

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLightingView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLightingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void ShowScene(int fadePos = 0, bool bSetSlider = true);

public:
	afx_msg void OnViewScene();

// Generated message map functions
protected:
	//{{AFX_MSG(CLightingView)
	afx_msg void OnChangeActiveCue();
	afx_msg void OnChangePreviewCue();
	afx_msg void OnChangeFadeTime();
	afx_msg void OnSelchangeCueList();
	afx_msg void OnCueNew();
	afx_msg void OnCueInsert();
	afx_msg void OnCueDelete();
	afx_msg void OnNextCue();
	afx_msg void OnPrevCue();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCueNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCueDel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCueInsert(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnFadeCancel();
	afx_msg void OnConnect();
	afx_msg void OnChangeFadeTimePreview();
	afx_msg void OnDestroy();
	afx_msg void OnUpdateConnect(CCmdUI* pCmdUI);
	afx_msg void OnKillfocusFadeTime();
	afx_msg void OnKillfocusFadeTimePreview();
	afx_msg void OnDmxFreeze();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void FadeCue(int start, int end);
	void JumpToCue(int cue);
};

#ifndef _DEBUG  // debug version in LightingView.cpp
inline CLightingDoc* CLightingView::GetDocument()
   { return (CLightingDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIGHTINGVIEW_H__618D217B_2EE6_4083_ACFE_6C58DAB0B41C__INCLUDED_)
