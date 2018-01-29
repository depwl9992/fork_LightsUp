// NewLampType.cpp : implementation file
//

#include "stdafx.h"
#include "Lighting.h"
#include "NewLampType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewLampType dialog


CNewLampType::CNewLampType(LampTypes &_types, CWnd* pParent)
	: CDialog(CNewLampType::IDD, pParent), m_Types(_types)
{
	//{{AFX_DATA_INIT(CNewLampType)
	//}}AFX_DATA_INIT
}


void CNewLampType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewLampType)
	DDX_Control(pDX, IDC_LAMP_TYPE_NAMES, m_NameList);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) {
		// Save selected name
		m_NameList.GetWindowText(m_Name);
	}
	else {
		// Load lamp types from curve editor
		int nTypes = m_Types.GetSize();
		for (int i = 0; i < nTypes; ++i) {
			m_NameList.AddString(m_Types[i].name);
		}
	}
}


BEGIN_MESSAGE_MAP(CNewLampType, CDialog)
	//{{AFX_MSG_MAP(CNewLampType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewLampType message handlers

BOOL CNewLampType::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_NameList.SetFocus();
	m_NameList.SetCurSel(0);

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
