// SceneDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lighting.h"
#include "SceneDlg.h"
#include "LightingDoc.h"
#include "LightingView.h"
#include "MapEditor.h"
#include "CurveEditor.h"
#include "LookEdit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSceneDlg dialog


CSceneDlg::CSceneDlg()
	: CDialog(CSceneDlg::IDD, NULL), m_Doc(NULL), m_View(NULL), m_SliderRect(0,0,0,0)
{
	//{{AFX_DATA_INIT(CSceneDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSceneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSceneDlg)
	DDX_Control(pDX, IDC_LOOK_LIST, m_List);
	//}}AFX_DATA_MAP

	if (m_Doc) {
		m_View->m_Updating = true;
		for (int i = 0; i < numSliders; ++i) {
			DDX_Text(pDX, IDC_EDIT0 + i, m_Doc->m_Channels[i].m_Name);
		}
		m_View->m_Updating = false;

		UpdateLooks();
	}
}


BEGIN_MESSAGE_MAP(CSceneDlg, CDialog)
	//{{AFX_MSG_MAP(CSceneDlg)
	ON_BN_CLICKED(IDC_EDIT_CHANNEL_MAP, OnEditChannelMap)
	ON_BN_CLICKED(IDC_EDIT_CURVES, OnEditCurves)
	ON_CBN_SELCHANGE(IDC_LOOK_LIST, OnSelchangeLookList)
	ON_BN_CLICKED(IDC_LOOK_SET, OnLookSet)
	ON_BN_CLICKED(IDC_LOOK_DEL, OnLookDel)
	ON_CBN_KILLFOCUS(IDC_LOOK_LIST, OnKillfocusLookList)
	ON_WM_KILLFOCUS()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSceneDlg message handlers

BOOL CSceneDlg::Create(CLightingView *pView)
{
	m_View = pView;
	return CDialog::Create(IDD, pView);
}

BOOL CSceneDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	//TRACE("CSceneDlg::Command:%d\n", wParam);
	if (wParam == IDCANCEL) {
		m_View->OnViewScene();
		return TRUE;
	}
	if (wParam == IDOK)
		// Absorb Return key
		return TRUE;

	if (HIWORD(wParam) == EN_CHANGE) {
		int editId = LOWORD(wParam) - IDC_EDIT0;
		if (editId >= 0 && editId < numSliders && !m_View->m_Updating) {
			char	text[80] = "";
			int len = m_SliderLabel[editId].GetLine(0, text, 80);
			text[len] = '\0'; // must add NULL termination.
			TRACE("Edit %d:%d:'%s'\n", editId, len, text);
			CLightingDoc	*pDoc = m_View->GetDocument();
			pDoc->m_Channels[editId].m_Name = text;
			pDoc->SetModifiedFlag();
			return TRUE;
		}
	}

	return CDialog::OnCommand(wParam, lParam);
}

BOOL CSceneDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	//TRACE("CSceneDlg::OnNotify:%d\n", wParam);
	int		sliderNum = wParam - IDC_SLIDER0;
	if (sliderNum >= 0 && sliderNum < numSliders && !m_View->m_Updating) {
		NMHDR *pHdr = (NMHDR *)lParam;
		//TRACE("Slider %d:%d:%d\n", sliderNum, m_Slider[sliderNum].GetPos(), pHdr->code);
		CLightingDoc *pDoc = m_View->GetDocument();
		int sel = m_View->m_Selected;
		if (sel < pDoc->m_Cues.GetSize()) {
			int	&pos = pDoc->m_Cues[sel].m_Scene[sliderNum];
			int newPos = GetSliderPos(sliderNum);
			if (pos != newPos) {
				m_View->m_fadeBase[sliderNum] = pos = newPos;
				pDoc->SetModifiedFlag();
				TRACE("Modified cue %d, slider[%d] = %d\n", sel, sliderNum, pos);

				m_View->ShowScene(0, false);
			}
		}
		return TRUE;
	}

	return CDialog::OnNotify(wParam, lParam, pResult);
}

BOOL CSceneDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CWnd *space = GetDlgItem(IDC_SLIDER_SPACE);
	space->GetClientRect(&m_SliderRect);
	space->MapWindowPoints(this, &m_SliderRect);

	m_SliderWidth = 40;
	m_SliderPitch = m_SliderWidth + sliderHorizSpacing;
	m_SliderHeight = 70;
	m_LabelHeight = 16;
	m_BankPitch = m_SliderHeight + m_LabelHeight + sliderVertSpacing;
	m_Font.CreateFont(
		m_LabelHeight,
		0,
		0,
		0,
		FW_NORMAL,
		0,
		0,
		0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		"Microsoft Sans Serif");

	for (int i = 0; i < numSliders; ++i) {
		int left = i % slidersPerBank * m_SliderPitch + m_SliderRect.left;
		int top = i / slidersPerBank * m_BankPitch + m_SliderRect.top;
		int right = left + m_SliderWidth;
		int bottom = top + m_SliderHeight;
		TRACE("Creating slider %d at: %d, %d, %d, %d\n", i, left, top, right, bottom);

		m_Slider[i].Create(WS_VISIBLE | TBS_VERT | TBS_BOTH | WS_TABSTOP,
			CRect(left, top, right, bottom),
			this,
			IDC_SLIDER0 + i);
		m_Slider[i].SetRange(0, dmxMaxVal);
		m_SliderLabel[i].Create(WS_VISIBLE | ES_AUTOHSCROLL, CRect(0,0,m_SliderWidth,m_LabelHeight), this, IDC_EDIT0 + i);
		m_SliderLabel[i].LimitText(sliderLabelMax);
		m_SliderLabel[i].SetFont(&m_Font);
		m_Slider[i].SetBuddy(&m_SliderLabel[i], FALSE);

		// NOTES
		// - If the slider control is made very wide and doesn't have the TBS_BOTH property,
		//   then it will be placed off center in its frame (e.g. left-justified).
		// - The Buddy is centered on the slider, not on the slider's frame.
	}

	CRect	dlgRect;
	GetClientRect(&dlgRect);

	m_SliderEdgePad.x = dlgRect.right - m_SliderRect.right;
	m_SliderEdgePad.y = dlgRect.bottom - m_SliderRect.bottom;

	int	diff = m_SliderPitch * slidersPerBank - m_SliderRect.Width();
	if (diff > 0) {
		dlgRect.right += diff;
	}
	diff = m_BankPitch * sliderBanks - m_SliderRect.Height();
	if (diff > 0) {
		dlgRect.bottom += diff;
	}
	CalcWindowRect(&dlgRect);
	SetWindowPos(NULL, 0, 0, dlgRect.Width(), dlgRect.Height(), SWP_NOZORDER | SWP_NOMOVE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSceneDlg::OnEditChannelMap() 
{
	CMapEditor mapEditor(m_View->GetDocument()->m_Channels, this);

	if (mapEditor.DoModal() == IDOK)
	{
		m_View->GetDocument()->SetModifiedFlag();
		UpdateData(FALSE);
		m_View->ShowScene();
	}
}

void CSceneDlg::OnEditCurves() 
{
	CCurveEditor curveEditor(m_View->GetDocument(), this);

	if (curveEditor.DoModal() == IDOK)
	{
		m_View->GetDocument()->SetModifiedFlag();
		m_View->ShowScene();
	}
}

void CSceneDlg::OnSelchangeLookList() 
{
	int sel = m_List.GetCurSel();
	if (sel >= 0)
	{
		CLightingDoc *pDoc = m_View->GetDocument();
		int *cueScene = pDoc->m_Cues[m_View->m_Selected].m_Scene;
		int *lookScene = pDoc->m_Looks[sel].m_Scene;
		for (int i = 0; i < numSliders; ++i)
			m_View->m_fadeBase[i] = cueScene[i] = lookScene[i];

		m_View->ShowScene();
		pDoc->SetModifiedFlag();
	}
}

void CSceneDlg::OnLookSet() 
{
	Cues &looks = m_View->GetDocument()->m_Looks;
	CCue &current = m_View->GetDocument()->m_Cues[m_View->m_Selected];
	
	CLookEdit edit(looks, this, CLookEdit::SetLook);
	
	bool done = false;
	while (! done)
	{
		if (edit.DoModal() != IDOK || edit.m_Name.GetLength() == 0)
			break; // Get out entirely
		
		// Is this replacing an existing name?
		int nLooks = looks.GetSize();
		int i;
		for (i = 0; i < nLooks; ++i) {
			if (edit.m_Name == looks[i].m_Text) {
				// Found a matching name
				break;
			}
		}
		
		if (i == nLooks) {
			// New name
			looks.Add(current);
		}
		else
		{
			// Found a matching name
			CString msg;
			msg.Format(_T("The Look named \"%s\" already exists.\nDo you want to replace it?"), edit.m_Name);
			if (MessageBox(msg, _T("Duplicate Name"), MB_ICONQUESTION | MB_YESNO) != IDYES)
				continue; // Go back and try again

			// Replace existing look with this one
			looks[i] = current;
		}
		
		// Replace Cue name with Look name
		looks[i].m_Text = edit.m_Name;
		
		UpdateLooks(i);
		m_View->GetDocument()->SetModifiedFlag();
		m_List.SetFocus();

		done = true;
	}
}
	
void CSceneDlg::OnLookDel() 
{
	Cues &looks = m_View->GetDocument()->m_Looks;

	CLookEdit edit(looks, this, CLookEdit::DelLook);

	if (edit.DoModal() == IDOK && edit.m_Name.GetLength() > 0) {
		int nLooks = looks.GetSize();
		for (int i = 0; i < nLooks; ++i) {
			if (edit.m_Name == looks[i].m_Text) {
				// Found the matching name, delete it
				looks.RemoveAt(i);
				UpdateLooks();
				m_View->GetDocument()->SetModifiedFlag();
				break;
			}
		}
	}
}

void CSceneDlg::OnKillfocusLookList() 
{
	m_List.SetCurSel(-1);
}

void CSceneDlg::UpdateLooks(int sel)
{
	Cues &looks = m_View->GetDocument()->m_Looks;

	m_List.ResetContent();
	int nLooks = looks.GetSize();
	for (int i = 0; i < nLooks; ++i)
		m_List.AddString(looks[i].m_Text);
	m_List.SetCurSel(sel);
}

void CSceneDlg::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	
	OnKillfocusLookList();
}

void CSceneDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (m_SliderRect.left > 0)
	{
		m_SliderRect.right = cx - m_SliderEdgePad.x;
		m_SliderRect.bottom = cy - m_SliderEdgePad.y;
		m_SliderPitch = m_SliderRect.Width() / slidersPerBank;
		m_SliderWidth = m_SliderPitch - sliderHorizSpacing;
		m_BankPitch = m_SliderRect.Height() / sliderBanks;
		m_SliderHeight = m_BankPitch - (m_LabelHeight + sliderVertSpacing);
		
		for (int i = 0; i < numSliders; ++i) {
			int left = i % slidersPerBank * m_SliderPitch + m_SliderRect.left;
			int top = i / slidersPerBank * m_BankPitch + m_SliderRect.top;
//			TRACE("Resizing slider %d at: %d, %d, %d, %d\n", i, left, top, m_SliderWidth, m_SliderHeight);
			
			m_SliderLabel[i].MoveWindow(0, 0, m_SliderWidth, m_LabelHeight, FALSE);
			m_Slider[i].MoveWindow(left, top, m_SliderWidth, m_SliderHeight, FALSE);
			m_Slider[i].SetBuddy(&m_SliderLabel[i], FALSE);
		}
	}
	Invalidate();
}
