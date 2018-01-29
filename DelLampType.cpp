// DelLampType.cpp : implementation file
//

#include "stdafx.h"
#include "Lighting.h"
#include "DelLampType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDelLampType dialog


CDelLampType::CDelLampType(LampTypes &types, CWnd* pParent)
	: CDialog(CDelLampType::IDD, pParent), m_Types(types)
{
	//{{AFX_DATA_INIT(CDelLampType)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDelLampType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDelLampType)
	DDX_Control(pDX, IDC_LAMP_TYPE_NAMES, m_TypeList);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) {
		// Save selected name
		m_TypeList.GetWindowText(m_Name);
	}
	else {
		// Load lamp types from curve editor
		int nTypes = m_Types.GetSize();
		for (int i = 0; i < nTypes; ++i) {
			m_TypeList.AddString(m_Types[i].name);
		}
	}
}


BEGIN_MESSAGE_MAP(CDelLampType, CDialog)
	//{{AFX_MSG_MAP(CDelLampType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDelLampType message handlers

BOOL CDelLampType::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_TypeList.SetFocus();
	m_TypeList.SetCurSel(0);

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
