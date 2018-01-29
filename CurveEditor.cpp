// CurveEditor.cpp : implementation file
//

#include "stdafx.h"
#include "Lighting.h"
#include "CurveEditor.h"
#include "NewLampType.h"
#include "DelLampType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCurveEditor dialog


CCurveEditor::CCurveEditor(CLightingDoc *pDoc, CWnd* pParent)
	: CDialog(CCurveEditor::IDD, pParent), m_pDoc(pDoc), m_DocCurves(pDoc->m_Curves)
{
	//{{AFX_DATA_INIT(CCurveEditor)
	m_Addr = 1;
	//}}AFX_DATA_INIT

	m_nAddr = 0;
}


void CCurveEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCurveEditor)
	DDX_Control(pDX, IDC_LAMP_TYPE, m_TypeList);
	DDX_Control(pDX, IDC_LAMP_ADDR_SPIN, m_AddrSpin);
	DDX_Control(pDX, IDC_MIN_SLIDER, m_MinSlider);
	DDX_Control(pDX, IDC_MAX_SLIDER, m_MaxSlider);
	DDX_Control(pDX, IDC_CURVE_SLIDER, m_CurveSlider);
	DDX_Control(pDX, IDC_CURVE_GRAPH, m_Graph);
	DDX_Text(pDX, IDC_LAMP_ADDR, m_Addr);
	DDV_MinMaxInt(pDX, m_Addr, 1, 512);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) {
		// Save dialog curves back to document
		for (int i = 0; i < dmxAddrs; ++i)
			m_DocCurves[i] = m_Curves[i];

		// Save lamp types back to document
		m_pDoc->m_LampTypes.Copy(m_LampTypes);
	}
	else {
		// Load document curves into dialog
		for (int i = 0; i < dmxAddrs; ++i)
			m_Curves[i] = m_DocCurves[i];

		// Load lamp types from document
		m_LampTypes.Copy(m_pDoc->m_LampTypes);
	}
}


BEGIN_MESSAGE_MAP(CCurveEditor, CDialog)
	//{{AFX_MSG_MAP(CCurveEditor)
	ON_EN_CHANGE(IDC_LAMP_ADDR, OnChangeLampAddr)
	ON_BN_CLICKED(IDC_LAMP_TYPE_SET, OnLampTypeSet)
	ON_NOTIFY(UDN_DELTAPOS, IDC_LAMP_ADDR_SPIN, OnDeltaposLampAddrSpin)
	ON_CBN_SELCHANGE(IDC_LAMP_TYPE, OnSelchangeLampType)
	ON_CBN_KILLFOCUS(IDC_LAMP_TYPE, OnKillfocusLampType)
	ON_BN_CLICKED(IDC_LAMP_TYPE_DEL, OnLampTypeDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCurveEditor message handlers

void CCurveEditor::OnChangeLampAddr() 
{
	CDataExchange DX(this, TRUE);
	DDX_Text(&DX, IDC_LAMP_ADDR, m_Addr);
	DDV_MinMaxInt(&DX, m_Addr, 1, 512);

	m_nAddr = m_Addr - 1;		

	Curve &cur = m_Curves[m_nAddr];
	// Translate 0-255 -> 255-0
	m_MinSlider.SetPos(dmxMaxVal - cur.minVal);
	m_MaxSlider.SetPos(dmxMaxVal - cur.maxVal);
	// Translate 1.0-3.0 -> 100-0
	m_CurveSlider.SetPos(100 - (int)((cur.curve - 1.0) / 2.0 * 100.0));

	UpdateGraph();
}

void CCurveEditor::OnDeltaposLampAddrSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	if ((pNMUpDown->iDelta > 0 && m_Addr < dmxAddrs)
		|| (pNMUpDown->iDelta < 0 && m_Addr > 1)) {
		m_Addr += pNMUpDown->iDelta;
		CDataExchange DX(this, FALSE);
		DDX_Text(&DX, IDC_LAMP_ADDR, m_Addr);
	}

	*pResult = 0;
}

BOOL CCurveEditor::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	switch (wParam) {
	case IDC_MIN_SLIDER:
		// Translate 255-0 -> 0-255
		m_Curves[m_nAddr].minVal = dmxMaxVal - m_MinSlider.GetPos();
		UpdateGraph();
		break;

	case IDC_MAX_SLIDER:
		// Translate 255-0 -> 0-255
		m_Curves[m_nAddr].maxVal = dmxMaxVal - m_MaxSlider.GetPos();
		UpdateGraph();
		break;

	case IDC_CURVE_SLIDER:
		// Translate 100-0 -> 1.0-3.0
		m_Curves[m_nAddr].curve = (100 - m_CurveSlider.GetPos()) / 100.0 * 2.0 + 1.0;
		UpdateGraph();
		break;
	}

	return CDialog::OnNotify(wParam, lParam, pResult);
}

BOOL CCurveEditor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect	rect;
	m_Graph.GetClientRect(&rect);
	m_GraphBitmapSize.cx = rect.Width();
	m_GraphBitmapSize.cy = rect.Height();
	m_GraphBitmap.CreateCompatibleBitmap(GetDC(), m_GraphBitmapSize.cx, m_GraphBitmapSize.cy);
	m_Graph.SetBitmap((HBITMAP)m_GraphBitmap);

	m_MinSlider.SetRange(0, dmxMaxVal);
	m_MaxSlider.SetRange(0, dmxMaxVal);
	m_CurveSlider.SetRange(0, 100);
	m_AddrSpin.SetRange(1, 512);
	m_AddrSpin.SetPos(1);

	OnChangeLampAddr();
	UpdateLampTypes();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

const COLORREF	WHITE = RGB(255,255,255);
const COLORREF	BLACK = RGB(0,0,0);

void CCurveEditor::UpdateGraph()
{
	CDC		dc;
	dc.CreateCompatibleDC(GetDC());
	dc.SelectObject(&m_GraphBitmap);

	CPen pen(PS_SOLID, 3, BLACK);
	CPen *oldPen = dc.SelectObject(&pen);

	CSize sz(m_GraphBitmapSize);
	dc.FillSolidRect(0, 0, sz.cx, sz.cy, WHITE);

	double xScale = (double)sz.cx / dmxMaxVal;
	double yScale = (double)sz.cy / dmxMaxVal;

	Curve &cur = m_Curves[m_nAddr];
	dc.MoveTo(0, sz.cy - (int)(cur.Apply(0) * yScale));
	for (int i = 0; i <= dmxMaxVal; i += 8) {
		dc.LineTo((int)(i * xScale) , sz.cy - (int)(cur.Apply(i) * yScale));
	}

	dc.SelectObject(oldPen);

	m_Graph.Invalidate(FALSE);
}

void CCurveEditor::OnSelchangeLampType() 
{
	int sel = m_TypeList.GetCurSel();
	if (sel >= 0) {
		m_Curves[m_nAddr] = (Curve)m_LampTypes[sel];
		OnChangeLampAddr();
	}
}

void CCurveEditor::OnKillfocusLampType() 
{
	m_TypeList.SetCurSel(-1);
}

void CCurveEditor::OnLampTypeSet() 
{
	CNewLampType nlt(m_LampTypes, this);

	if (nlt.DoModal() == IDOK && nlt.m_Name.GetLength() > 0) {
		// Is this replacing an existing name?
		int nTypes = m_LampTypes.GetSize();
		for (int i = 0; i < nTypes; ++i) {
			if (nlt.m_Name == m_LampTypes[i].name) {
				// Found the matching name, use it
				break;
			}
		}
		if (i == nTypes) {
			// New name
			m_LampTypes.Add(LampType(nlt.m_Name));
		}

		LampType &type = m_LampTypes[i];
		Curve &cur = m_Curves[m_nAddr];
		type.minVal = cur.minVal;
		type.maxVal = cur.maxVal;
		type.curve = cur.curve;

		UpdateLampTypes(i);
	}
}

void CCurveEditor::UpdateLampTypes(int sel)
{
	m_TypeList.ResetContent();
	int nTypes = m_LampTypes.GetSize();
	for (int i = 0; i < nTypes; ++i)
		m_TypeList.AddString(m_LampTypes[i].name);
	m_TypeList.SetCurSel(sel);
}

void CCurveEditor::OnLampTypeDel() 
{
	CDelLampType dlt(m_LampTypes, this);

	if (dlt.DoModal() == IDOK && dlt.m_Name.GetLength() > 0) {
		int nTypes = m_LampTypes.GetSize();
		for (int i = 0; i < nTypes; ++i) {
			if (dlt.m_Name == m_LampTypes[i].name) {
				// Found the matching name, delete it
				m_LampTypes.RemoveAt(i);
				UpdateLampTypes();
				break;
			}
		}
	}
}
