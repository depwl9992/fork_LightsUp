// LookEdit.cpp : implementation file
//

#include "stdafx.h"
#include "Lighting.h"
#include "LookEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLookEdit dialog


CLookEdit::CLookEdit(Cues &looks, CWnd* pParent, enum EditLookType type)
	: CDialog(CLookEdit::IDD, pParent), m_Looks(looks), m_Type(type)
{
	//{{AFX_DATA_INIT(CLookEdit)
	m_Prompt = _T("");
	//}}AFX_DATA_INIT
}


void CLookEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLookEdit)
	DDX_Control(pDX, IDC_LOOK_LIST, m_List);
	DDX_Text(pDX, IDC_LOOK_PROMPT, m_Prompt);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) {
		// Save looks back to curve editor
		m_List.GetWindowText(m_Name);
	}
	else {
		// Load looks from document
		int nLooks = m_Looks.GetSize();
		for (int i = 0; i < nLooks; ++i) {
			m_List.AddString(m_Looks[i].m_Text);
		}
	}
}


BEGIN_MESSAGE_MAP(CLookEdit, CDialog)
	//{{AFX_MSG_MAP(CLookEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLookEdit message handlers

BOOL CLookEdit::OnInitDialog() 
{
	CString title;

	switch (m_Type) {
	case SetLook:
		title = _T("Add a new look");
		m_Prompt = _T("Save the current slider settings as a new look named:");
		break;

	case DelLook:
		title = _T("Delete a look");
		m_Prompt = _T("Remove the look named:");
		break;
	}

	CDialog::OnInitDialog();
	
	m_List.SetFocus();
	m_List.SetCurSel(-1);
	SetWindowText(title);

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
