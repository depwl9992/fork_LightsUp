// Original author: Terry Wells
// Original project: https://sourceforge.net/projects/lightsup/
// Forked on Jan 29, 2018 by Daniel Powell

// LightingView.cpp : implementation of the CLightingView class
//

#include "stdafx.h"
#include "Lighting.h"

#include "LightingDoc.h"
#include "LightingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLightingView

IMPLEMENT_DYNCREATE(CLightingView, CFormView)

BEGIN_MESSAGE_MAP(CLightingView, CFormView)
	//{{AFX_MSG_MAP(CLightingView)
	ON_EN_CHANGE(IDC_ACTIVE_CUE, OnChangeActiveCue)
	ON_EN_CHANGE(IDC_PREVIEW_CUE, OnChangePreviewCue)
	ON_EN_CHANGE(IDC_FADE_TIME, OnChangeFadeTime)
	ON_LBN_SELCHANGE(IDC_CUE_LIST, OnSelchangeCueList)
	ON_COMMAND(ID_CUE_NEW, OnCueNew)
	ON_COMMAND(ID_CUE_INSERT, OnCueInsert)
	ON_COMMAND(ID_CUE_DEL, OnCueDelete)
	ON_BN_CLICKED(IDC_NEXT_CUE, OnNextCue)
	ON_BN_CLICKED(IDC_PREV_CUE, OnPrevCue)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_CUE_NEW, OnUpdateCueNew)
	ON_UPDATE_COMMAND_UI(ID_CUE_DEL, OnUpdateCueDel)
	ON_UPDATE_COMMAND_UI(ID_CUE_INSERT, OnUpdateCueInsert)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_FADE_CANCEL, OnFadeCancel)
	ON_COMMAND(ID_CONNECT, OnConnect)
	ON_EN_CHANGE(IDC_FADE_TIME_PREVIEW, OnChangeFadeTimePreview)
	ON_WM_DESTROY()
	ON_EN_KILLFOCUS(IDC_FADE_TIME, OnKillfocusFadeTime)
	ON_EN_KILLFOCUS(IDC_FADE_TIME_PREVIEW, OnKillfocusFadeTimePreview)
	ON_COMMAND(ID_VIEW_SCENE, OnViewScene)
	ON_BN_CLICKED(IDC_CONNECT, OnConnect)
	ON_BN_CLICKED(IDC_DMX_FREEZE, OnDmxFreeze)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLightingView construction/destruction

CLightingView::CLightingView()
	: CFormView(CLightingView::IDD)
{
	//{{AFX_DATA_INIT(CLightingView)
	m_ActiveCue = _T("");
	m_PreviewCue = _T("");
	m_FadeTime = 0;
	m_FadeTimePreview = 0;
	m_frozenCueText = _T("");
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	m_SceneVisible = false;
	m_Updating = false;
	m_HasCueClip = false;
	m_fadeTimer = 0;
	m_fadePos = 0;
	m_fadeEndCue = -1;
	m_connected = false;
	m_frozenCue = -1;
}

CLightingView::~CLightingView()
{
}

void CLightingView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLightingView)
	DDX_Control(pDX, IDC_DMX_FREEZE, m_dmxFreeze);
	DDX_Control(pDX, IDC_CONNECT, m_ConnectButton);
	DDX_Control(pDX, IDC_PREV_CUE, m_PrevCue);
	DDX_Control(pDX, IDC_NEXT_CUE, m_NextCue);
	DDX_Control(pDX, IDC_FADE_PROGRESS, m_FadeProgress);
	DDX_Control(pDX, IDC_FADE_CANCEL, m_FadeCancel);
	DDX_Control(pDX, IDC_CUE_LIST, m_CueList);
	DDX_Text(pDX, IDC_ACTIVE_CUE, m_ActiveCue);
	DDX_Text(pDX, IDC_PREVIEW_CUE, m_PreviewCue);
	DDX_Text(pDX, IDC_FADE_TIME, m_FadeTime);
	DDV_MinMaxUInt(pDX, m_FadeTime, 0, 300);
	DDX_Text(pDX, IDC_FADE_TIME_PREVIEW, m_FadeTimePreview);
	DDV_MinMaxUInt(pDX, m_FadeTimePreview, 0, 300);
	DDX_Text(pDX, IDC_FROZEN_CUE_TEXT, m_frozenCueText);
	//}}AFX_DATA_MAP
}

BOOL CLightingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	m_SceneDlg.Create(this);

	return CFormView::PreCreateWindow(cs);
}

void CLightingView::OnDestroy() 
{
	CFormView::OnDestroy();
	
	m_SceneDlg.DestroyWindow();	
}

void CLightingView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	m_Updating = false;
}

void CLightingView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	TRACE("OnUpdate()\n");

	CLightingDoc	*pDoc = GetDocument();
	m_SceneDlg.m_Doc = pDoc;
	int i;

	m_SceneDlg.UpdateData(FALSE);

	m_CueList.ResetContent();
	for (i = 0; i < pDoc->m_Cues.GetSize(); i++) {
		m_CueList.AddString(pDoc->m_Cues[i].m_Text);
	}
//	m_CueList.AddString("[End of Show]");

	m_CueList.SetCurSel(0);
	OnSelchangeCueList();

	TRACE("OnUpdate: Done\n");
}

/////////////////////////////////////////////////////////////////////////////
// CLightingView printing

BOOL CLightingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLightingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLightingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLightingView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CLightingView diagnostics

#ifdef _DEBUG
void CLightingView::AssertValid() const
{
	CFormView::AssertValid();
}

void CLightingView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CLightingDoc* CLightingView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLightingDoc)));
	return (CLightingDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLightingView message handlers

void CLightingView::OnViewScene() 
{
	m_SceneVisible = !m_SceneVisible;
	m_SceneDlg.ShowWindow(m_SceneVisible ? SW_SHOW : SW_HIDE);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_VIEW_SCENE, m_SceneVisible ? MF_CHECKED : MF_UNCHECKED);
}

void CLightingView::OnChangeActiveCue() 
{
	UpdateData();

	GetDocument()->m_Cues[m_Selected].m_Text = m_ActiveCue;
	GetDocument()->SetModifiedFlag();
	m_CueList.DeleteString(m_Selected);
	m_CueList.InsertString(m_Selected, m_ActiveCue);
	m_CueList.SetCurSel(m_Selected);
}

void CLightingView::OnChangePreviewCue() 
{
	UpdateData();

	GetDocument()->m_Cues[m_Selected].m_Text = m_PreviewCue;
	GetDocument()->SetModifiedFlag();
	m_CueList.DeleteString(m_Selected + 1);
	m_CueList.InsertString(m_Selected + 1, m_PreviewCue);
}

void CLightingView::OnChangeFadeTime() 
{
	GetDocument()->SetModifiedFlag();
}

void CLightingView::OnKillfocusFadeTime() 
{
	UpdateData();
	GetDocument()->m_Cues[m_Selected].m_FadeTime = m_FadeTime;
}

void CLightingView::OnChangeFadeTimePreview() 
{
	GetDocument()->SetModifiedFlag();
}

void CLightingView::OnKillfocusFadeTimePreview() 
{
	UpdateData();
	GetDocument()->m_Cues[m_Selected + 1].m_FadeTime = m_FadeTimePreview;
}

void CLightingView::OnSelchangeCueList() 
{
	TRACE("CLightingView::OnSelchangeCueList: Selected: %d\n", m_CueList.GetCurSel());

	CLightingDoc *pDoc = GetDocument();
	int	pos = m_CueList.GetCurSel();
	if (pos == LB_ERR || pos > pDoc->m_Cues.GetUpperBound())
		pos = 0;

	if (pos == pDoc->m_Cues.GetUpperBound()) {
		// Active cue is the End of Show cue
		GetDlgItem(IDC_ACTIVE_CUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CUE_LIST)->SetFocus();
	}
	else {
		GetDlgItem(IDC_ACTIVE_CUE)->EnableWindow();
	}
	GetDlgItem(IDC_FADE_TIME)->EnableWindow(pos > 0);

	m_ActiveCue = pDoc->m_Cues[pos].m_Text;
	m_FadeTime = pDoc->m_Cues[pos].m_FadeTime;
	int	*pPos = pDoc->m_Cues[pos].m_Scene;

	// Load preview name edit field
	int preview = pos + 1;
	if (preview < pDoc->m_Cues.GetSize()) {
		// There is a valid cue in the preview position
		if (preview == pDoc->m_Cues.GetUpperBound()) {
			// Preview cue is the End of Show cue
			GetDlgItem(IDC_PREVIEW_CUE)->EnableWindow(FALSE);
		}
		else {
			// Preview cue is a normal cue
			GetDlgItem(IDC_PREVIEW_CUE)->EnableWindow();
		}
		GetDlgItem(IDC_FADE_TIME_PREVIEW)->EnableWindow();
		m_PreviewCue = pDoc->m_Cues[preview].m_Text;
		m_FadeTimePreview = pDoc->m_Cues[preview].m_FadeTime;
	} else {
		// Off the end of the script
		GetDlgItem(IDC_PREVIEW_CUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_FADE_TIME_PREVIEW)->EnableWindow(FALSE);
		m_PreviewCue.Empty();
		m_FadeTimePreview = 0;
	}

	// State of Next and Prev Cue buttons depends on displayed cue, not selected.
	if (m_frozenCue >= 0)
	{
		m_NextCue.EnableWindow(m_frozenCue + 1 < pDoc->m_Cues.GetSize());
		m_PrevCue.EnableWindow(m_frozenCue > 0);
	}
	else
	{
		m_NextCue.EnableWindow(preview < pDoc->m_Cues.GetSize());
		m_PrevCue.EnableWindow(pos > 0);
	}

	// Load cue settings into sliders
	for (int i = 0; i < numSliders; i++) {
		m_fadeBase[i] = pPos[i];
	}
	m_fadePos = 0;
	m_fadeEndCue = -1;
	ShowScene();
	CString title;
	title.Format(_T("Scene Control - %s"), m_ActiveCue);
	m_SceneDlg.SetWindowText(title);
	m_SceneDlg.EnableWindow();

	UpdateData(FALSE);
	m_Selected = pos;
}

void CLightingView::ShowScene(int fadePos, bool bSetSlider)
{
	int	channelMix[dmxAddrs] = {0};	// Default all channels to zero
	CChannel *pChan = GetDocument()->m_Channels;
	Curve *pCurves = GetDocument()->m_Curves;
	
	m_Updating = true;

	//
	// Mix slider values into channels
	//
	for (int i = 0; i < numSliders; i++) {
		int sVal = (int)(m_fadeBase[i] + m_fadeVec[i] * fadePos); // Faded slider value
		if (bSetSlider)
			m_SceneDlg.SetSliderPos(i, (int)(sVal + 0.5));
		
		// Apply slider values to each attached lamp
		// Collect the mix as intermediate values.
		int nAddrs = pChan[i].m_Lamps.GetSize();
		for (int a = 0; a < nAddrs; ++a) {
			Lamp &lamp = pChan[i].m_Lamps[a];
			int addr = max(0, min(lamp.addr - 1, dmxMaxChan)); // Convert address to 0 based and clamp
			channelMix[addr] += sVal * lamp.percent / 100;
		}
	}

	if (! m_dmxFreeze.GetCheck())
	{
		//
		// Build DMX values by applying lamp curves
		//
		unsigned char dmxPacket[dmxAddrs];
		for (int a = 0; a < dmxAddrs; ++a)
			dmxPacket[a] = pCurves[a].Apply(max(0, min(dmxMaxVal, channelMix[a])));
		m_openUsbDMX.send_dmx_packet(dmxPacket);
//		TRACE("Set new DMX packet starting with: %d\n", (int)dmxPacket[0]);
	}

	m_Updating = false;
}

void CLightingView::OnCueNew() 
{
	CLightingDoc *pDoc = GetDocument();
	int pos = m_Selected;

	if (pos < pDoc->m_Cues.GetUpperBound()) {
		pDoc->m_Cues.InsertAt(pos + 1, pDoc->m_Cues[pos]);
		++pos;
		m_CueList.InsertString(pos, pDoc->m_Cues[pos].m_Text);
		m_CueList.SetCurSel(pos);
		OnSelchangeCueList();
		CEdit *active = (CEdit *)GetDlgItem(IDC_ACTIVE_CUE);
		active->SetFocus();
		active->SetSel(0, -1);

		pDoc->SetModifiedFlag();
	}
	else
		OnCueInsert();
}

void CLightingView::OnCueInsert() 
{
	CLightingDoc *pDoc = GetDocument();
	int pos = m_Selected;

	CCue &cue = pos < pDoc->m_Cues.GetUpperBound() ? pDoc->m_Cues[pos] : CCue();
	pDoc->m_Cues.InsertAt(pos, cue);
	m_CueList.InsertString(m_Selected, pDoc->m_Cues[pos].m_Text);
	m_CueList.SetCurSel(pos);
	OnSelchangeCueList();
	CEdit *active = (CEdit *)GetDlgItem(IDC_ACTIVE_CUE);
	active->SetFocus();
	active->SetSel(0, -1);

	pDoc->SetModifiedFlag();
}

void CLightingView::OnCueDelete() 
{
	CLightingDoc *pDoc = GetDocument();

	// Do not remove 'End of Show'
	if (m_Selected == pDoc->m_Cues.GetUpperBound())
		return;

	pDoc->m_Cues.RemoveAt(m_Selected);
	m_CueList.DeleteString(m_Selected);
	m_CueList.SetCurSel(m_Selected);
	OnSelchangeCueList();

	pDoc->SetModifiedFlag();
}

void CLightingView::OnUpdateCueNew(CCmdUI* pCmdUI) 
{
	CWnd *focus = GetFocus();
	if (focus == GetDlgItem(IDC_CUE_LIST)) {
		pCmdUI->Enable();
		return;
	}

	CEdit	*activeCue = (CEdit *)GetDlgItem(IDC_ACTIVE_CUE);
	if (focus == activeCue) {
		pCmdUI->Enable();
		return;
	}

	pCmdUI->Enable(FALSE);
}

void CLightingView::OnUpdateCueInsert(CCmdUI* pCmdUI) 
{
	CWnd *focus = GetFocus();
	if (focus == GetDlgItem(IDC_CUE_LIST)) {
		pCmdUI->Enable();
		return;
	}

	CEdit	*activeCue = (CEdit *)GetDlgItem(IDC_ACTIVE_CUE);
	if (focus == activeCue) {
		pCmdUI->Enable();
		return;
	}

	pCmdUI->Enable(FALSE);
}

void CLightingView::OnUpdateCueDel(CCmdUI* pCmdUI) 
{
	CWnd *focus = GetFocus();
	if (focus == GetDlgItem(IDC_CUE_LIST)) {
		pCmdUI->Enable(GetDocument()->m_Cues.GetSize() > 0);
		return;
	}

	pCmdUI->Enable(FALSE);
}


void CLightingView::OnPrevCue() 
{
	TRACE("PrevCue!\n");

	if (m_fadeTimer > 0)
		return;

	// Check for and kill freeze
	if (m_dmxFreeze.GetCheck())
	{
		m_dmxFreeze.SetCheck(FALSE);
		OnDmxFreeze();
	}

	if (m_fadeEndCue == m_Selected + 1)
	{
		// Reverse a previous request to forward fade
		FadeCue(m_Selected + 1, m_Selected);
	}
	else if (m_Selected > 0)
	{
		// Normal reverse fade
		FadeCue(m_Selected, m_Selected - 1);
	}
}

void CLightingView::OnNextCue() 
{
	TRACE("NextCue!\n");

	if (m_fadeTimer > 0)
		return;

	// Check for and kill freeze
	if (m_dmxFreeze.GetCheck())
	{
		m_dmxFreeze.SetCheck(FALSE);
		OnDmxFreeze();
	}

	if (m_fadePos > 0 && m_fadeEndCue == m_Selected - 1)
	{
		// Reverse a previous request to back fade
		FadeCue(m_Selected - 1, m_Selected);
	}
	else if (m_Selected < GetDocument()->m_Cues.GetUpperBound())
	{
		// Normal forward fade
		FadeCue(m_Selected, m_Selected + 1);
	}
}

void CLightingView::FadeCue(int start, int end)
{
	CLightingDoc *pDoc = GetDocument();

	if (m_fadeTimer > 0)
	{
		KillTimer(m_fadeTimer);
		m_fadeTimer = 0;
	}

 	if (m_fadePos == 0 || start == m_fadeEndCue)
	{
		// start new fade, or reverse direction of paused fade

		CCue	&fromCue = pDoc->m_Cues[start];
		CCue	&toCue = pDoc->m_Cues[end];
		int		fadeTime = end > start ? toCue.m_FadeTime : fromCue.m_FadeTime;

		m_fadeEndCue = end;
		m_fadeSteps = fadeTime * 1000 / fadeFactor + 1;
		if (m_fadePos > 0)
			m_fadePos = m_fadeSteps - m_fadePos; // reverse direction
		for (int i = 0; i < numSliders; i++) {
			m_fadeBase[i] = fromCue.m_Scene[i];
			m_fadeVec[i] = (double)(toCue.m_Scene[i] - m_fadeBase[i]) / m_fadeSteps;
		}
	}
	else if (end == m_fadeEndCue)
	{
		// Continue in the same direction as before
	}

	m_NextCue.EnableWindow();
	m_PrevCue.EnableWindow();
	m_FadeCancel.EnableWindow();
	m_FadeProgress.SetRange(0, m_fadeSteps - 1);
	m_FadeProgress.SetPos(m_fadePos);
	m_fadeTimer = SetTimer(1, fadeFactor, NULL);
}

void CLightingView::OnFadeCancel() 
{
	if (m_fadeTimer > 0)
	{
		KillTimer(m_fadeTimer);
		m_fadeTimer = 0;
	}
	m_FadeCancel.EnableWindow(FALSE);
}

void CLightingView::JumpToCue(int cue)
{
	OnFadeCancel();
	m_FadeProgress.SetPos(0);
	m_Selected = cue;
	m_CueList.SetCurSel(m_Selected);
	OnSelchangeCueList();
}

void CLightingView::OnTimer(UINT nIDEvent) 
{
	if (m_fadePos < m_fadeSteps)
	{
		ShowScene(m_fadePos);

		m_FadeProgress.SetPos(++m_fadePos);
		if (m_fadePos == m_fadeSteps)
		{
			JumpToCue(m_fadeEndCue);
		}
	}
	
	CFormView::OnTimer(nIDEvent);
}

BOOL CLightingView::PreTranslateMessage(MSG* pMsg) 
{
	// Handle keyboard accelerators
	if (pMsg->message == WM_KEYDOWN) {

		CLightingDoc *pDoc = GetDocument();
		int	pos = m_Selected;

		CWnd *focus = GetFocus();

		switch (pMsg->wParam)
		{
		case VK_DELETE:
			if (focus == GetDlgItem(IDC_CUE_LIST)) {
				// Delete current
				OnCueDelete();
				return TRUE;
			}
			break;

		case VK_DOWN:
			if (pos < pDoc->m_Cues.GetUpperBound()) {
				// Skip to next cue
				JumpToCue(++pos);
			}
			return TRUE;

		case VK_UP:
			if (pos > 0) {
				// Skip to prev cue
				JumpToCue(--pos);
			}
			return TRUE;

		case VK_SPACE:
			if (focus != GetDlgItem(IDC_ACTIVE_CUE)
				&& focus != GetDlgItem(IDC_PREVIEW_CUE)
				&& focus != GetDlgItem(IDC_FADE_TIME)
				&& focus != GetDlgItem(IDC_FADE_TIME_PREVIEW)) {
				// Fade to next cue
				OnNextCue();
				return TRUE;
			}
			break;

		case VK_BACK:
			if (focus != GetDlgItem(IDC_ACTIVE_CUE)
				&& focus != GetDlgItem(IDC_PREVIEW_CUE)
				&& focus != GetDlgItem(IDC_FADE_TIME)
				&& focus != GetDlgItem(IDC_FADE_TIME_PREVIEW)) {
				// Fade to prev cue
				OnPrevCue();
				return TRUE;
			}
			break;

		case VK_ESCAPE:
			// Cancel fade in progress
			OnFadeCancel();
			return TRUE;

		default:
			break;
		}
	}
	return CFormView::PreTranslateMessage(pMsg);
}

void CLightingView::OnEditCopy() 
{
	CWnd *focus = GetFocus();
	if (focus == GetDlgItem(IDC_CUE_LIST)) {
		m_CueClip = GetDocument()->m_Cues[m_Selected];
		m_HasCueClip = true;
		return;
	}

	CEdit	*activeCue = (CEdit *)GetDlgItem(IDC_ACTIVE_CUE);
	if (focus == activeCue) {
		activeCue->Copy();
	}
}

void CLightingView::OnEditCut() 
{
	CWnd *focus = GetFocus();
	if (focus == GetDlgItem(IDC_CUE_LIST)) {
		OnEditCopy();
		OnCueDelete();
		return;
	}

	CEdit	*activeCue = (CEdit *)GetDlgItem(IDC_ACTIVE_CUE);
	if (focus == activeCue) {
		activeCue->Cut();
	}
}

void CLightingView::OnEditPaste() 
{
	CWnd *focus = GetFocus();
	if (focus == GetDlgItem(IDC_CUE_LIST)) {
		CLightingDoc *pDoc = GetDocument();
		pDoc->m_Cues.InsertAt(m_Selected, m_CueClip);
		m_CueList.InsertString(m_Selected, m_CueClip.m_Text);
		m_CueList.SetCurSel(m_Selected);
		OnSelchangeCueList();

		pDoc->SetModifiedFlag();
		return;
	}

	CEdit	*activeCue = (CEdit *)GetDlgItem(IDC_ACTIVE_CUE);
	if (focus == activeCue) {
		activeCue->Paste();
	}
}

void CLightingView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	CWnd *focus = GetFocus();
	if (focus == GetDlgItem(IDC_CUE_LIST)) {
		pCmdUI->Enable(GetDocument()->m_Cues.GetSize() > 0);
		return;
	}

	CEdit	*activeCue = (CEdit *)GetDlgItem(IDC_ACTIVE_CUE);
	if (focus == activeCue) {
		int	s, e;
		activeCue->GetSel(s, e);
		pCmdUI->Enable(e > s);
		return;
	}

	pCmdUI->Enable(FALSE);
}

void CLightingView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	CWnd *focus = GetFocus();
	if (focus == GetDlgItem(IDC_CUE_LIST)) {
		pCmdUI->Enable(GetDocument()->m_Cues.GetSize() > 0);
		return;
	}

	CEdit	*activeCue = (CEdit *)GetDlgItem(IDC_ACTIVE_CUE);
	if (focus == activeCue) {
		int	s, e;
		activeCue->GetSel(s, e);
		pCmdUI->Enable(e > s);
		return;
	}

	pCmdUI->Enable(FALSE);
}

void CLightingView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	CWnd *focus = GetFocus();
	if (focus == GetDlgItem(IDC_CUE_LIST)) {
		pCmdUI->Enable(m_HasCueClip);
		return;
	}

	CEdit	*activeCue = (CEdit *)GetDlgItem(IDC_ACTIVE_CUE);
	if (focus == activeCue) {
		//pCmdUI->Enable();
		return;
	}

	pCmdUI->Enable(FALSE);
}

void CLightingView::OnConnect() 
{
	CWaitCursor();

	if (m_connected) {
		m_openUsbDMX.close_dmx_devices();
		m_connected = false;
	}
	else {
		int devCode = m_openUsbDMX.open_dmx_devices();
		if (devCode > 0)
			m_connected = true;
		else if (devCode == 0) {
			CString msg;
			msg.Format(_T("There are no USB DMX devices available."));
			MessageBox(msg, _T("LightsUp!"));
		}
		else {
			CString msg;
			msg.Format(_T("Could not open USB DMX device.\nError: %d"), devCode);
			MessageBox(msg, _T("LightsUp!"));
		}
	}
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_CONNECT, m_connected ? MF_CHECKED : MF_UNCHECKED);
	m_ConnectButton.SetCheck(m_connected);
}

void CLightingView::OnDmxFreeze() 
{
	if (m_dmxFreeze.GetCheck())
	{
		m_frozenCue = m_Selected;
		m_frozenCueText = GetDocument()->m_Cues[m_Selected].m_Text;
		UpdateData(FALSE);
	}
	else
	{
		m_Selected = m_frozenCue;
		m_frozenCue = -1;
		m_frozenCueText = _T("");
		m_CueList.SetCurSel(m_Selected);
		OnSelchangeCueList();
	}
}
