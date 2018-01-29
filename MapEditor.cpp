// MapEditor.cpp : implementation file
//

#include "stdafx.h"
#include "Lighting.h"
#include "MapEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx

class CListCtrlEx : public CListCtrl
{
	// Attributes
protected:

	// Operation
public:
	CImageList* SetImageList(CImageList* pImageList, int nImageListType = TVSIL_NORMAL);
	BOOL AddColumn(
		LPCTSTR strItem,int nItem, int width = -1,int nSubItem = -1,
		int nMask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM,
		int nFmt = LVCFMT_LEFT);
	BOOL AddItem(int nItem,int nSubItem,LPCTSTR strItem,int nImageIndex = -1);
};


BOOL CListCtrlEx::AddColumn(LPCTSTR strItem,int nItem,int width,int nSubItem,int nMask,int nFmt)
{
	LV_COLUMN lvc;
	lvc.mask = nMask;
	lvc.fmt = nFmt;
	lvc.pszText = (LPTSTR) strItem;
	lvc.cx = width < 0 ? GetStringWidth(lvc.pszText) + 15 : width;
	if(nMask & LVCF_SUBITEM){
		if(nSubItem != -1)
			lvc.iSubItem = nSubItem;
		else
			lvc.iSubItem = nItem;
	}
	return InsertColumn(nItem,&lvc);
}

BOOL CListCtrlEx::AddItem(int nItem,int nSubItem,LPCTSTR strItem,int nImageIndex)
{
	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItem;
	lvItem.iSubItem = nSubItem;
	lvItem.pszText = (LPTSTR) strItem;
	if(nImageIndex != -1){
		lvItem.mask |= LVIF_IMAGE;
		lvItem.iImage = nImageIndex;
	}
	if(nSubItem == 0)
		return InsertItem(&lvItem);
	return SetItem(&lvItem);
}

/////////////////////////////////////////////////////////////////////////////
// CMapEditor dialog


CMapEditor::CMapEditor(CChannel *map, CWnd* pParent)
	: CDialog(CMapEditor::IDD, pParent), m_map(map)
{
	//{{AFX_DATA_INIT(CMapEditor)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMapEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapEditor)
	DDX_Control(pDX, IDC_MAP_ADDRESSES, m_MapAddresses);
	DDX_Control(pDX, IDC_MAP_CHANNEL, m_MapChannel);
	DDX_Control(pDX, IDC_CHANNEL_MAP, m_mapList);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		for (int i = 0; i < numSliders; ++i)
		{
			CChannel &chan = m_map[i];

			// Get Channel Name
			chan.m_Name = m_mapList.GetItemText(i, 0);

			// Get Address mapping
			CString lamps(m_mapList.GetItemText(i, 1));
			chan.m_Lamps.RemoveAll();

			int addr, percent, pos = 0;
			while (pos >= 0 && _stscanf(lamps, _T("%d%%%d"), &addr, &percent) == 2)
			{
				chan.m_Lamps.Add(Lamp(addr, percent));
				pos = lamps.Find(_T(','));
				lamps = lamps.Mid(pos + 1);
			}
		}
	}
	else {
		CListCtrlEx& list = (CListCtrlEx&) m_mapList;

		// Reset the list control
		list.DeleteAllItems();
		while (list.DeleteColumn(0));

		// Format the list control
		CRect rect;
		m_mapList.GetClientRect(&rect);

		LPCTSTR szChannel = _T("Channel");
		LPCTSTR szAddresses = _T("Addresses");

		int width = list.GetStringWidth(szChannel) + 15;
		list.AddColumn(szChannel, 0, width);
		list.AddColumn(szAddresses, 1, rect.Width() - width);

		list.SetItemCount(numSliders);

		for (int i = 0; i < numSliders; ++i) {
			CChannel &chan = m_map[i];
			list.AddItem(i, 0, chan.m_Name);
			int nAddrs = chan.m_Lamps.GetSize();
			CString lamps;
			if (nAddrs > 0)
				lamps.Format(_T("%d%%%d"), chan.m_Lamps[0].addr, chan.m_Lamps[0].percent);
			for (int a = 1; a < nAddrs; ++a) {
				CString lamp;
				lamp.Format(_T(",%d%%%d"), chan.m_Lamps[a].addr, chan.m_Lamps[a].percent);
				lamps += lamp;
			}
			list.AddItem(i, 1, lamps);
		}

		m_selItem = -1;
		m_errorItem = -1;
		m_MapChannel.LimitText(sliderLabelMax);
	}
}


BEGIN_MESSAGE_MAP(CMapEditor, CDialog)
	//{{AFX_MSG_MAP(CMapEditor)
	ON_BN_CLICKED(IDC_MAP_CLEAR_ALL, OnMapClearAll)
	ON_BN_CLICKED(IDC_MAP_IDENTITY, OnMapIdentity)
	ON_NOTIFY(NM_CLICK, IDC_CHANNEL_MAP, OnClickChannelMap)
	ON_EN_CHANGE(IDC_MAP_CHANNEL, OnChangeMapChannel)
	ON_EN_CHANGE(IDC_MAP_ADDRESSES, OnChangeMapAddresses)
	ON_EN_KILLFOCUS(IDC_MAP_ADDRESSES, OnKillfocusMapAddresses)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapEditor message handlers

void CMapEditor::OnMapClearAll() 
{
	if (MessageBox(_T("Are you sure you want to clear all mappings?"), _T("Channel Map"), MB_OKCANCEL) == IDOK)
		for (int i = 0; i < numSliders; ++i)
			m_mapList.SetItemText(i, 1, _T(""));
}

void CMapEditor::OnMapIdentity() 
{
	if (MessageBox(_T("Are you sure you want to reset all mappings?"), _T("Channel Map"), MB_OKCANCEL) == IDOK)
		for (int i = 0; i < numSliders; ++i) {
			CString lamp;
			lamp.Format(_T("%d%%100"), i + 1);
			m_mapList.SetItemText(i, 1, lamp);
		}
}

void CMapEditor::OnClickChannelMap(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMLISTVIEW lpnmlv = (LPNMLISTVIEW)pNMHDR;
	int clickedItem = lpnmlv->iItem;

	if (m_errorItem < 0)
	{
		m_selItem = clickedItem;
		if (m_selItem >= 0)
		{
			m_MapChannel.SetWindowText(m_mapList.GetItemText(m_selItem, 0));
			m_MapAddresses.SetWindowText(m_mapList.GetItemText(m_selItem, 1));
		}
		else
		{
			m_MapChannel.SetWindowText(_T(""));
			m_MapAddresses.SetWindowText(_T(""));
		}
	}
	else
	{
		// Force selection back to item flagged with an error.
		m_mapList.SetItemState(clickedItem, 0, LVIS_SELECTED);
		m_mapList.SetItemState(m_errorItem, LVIS_SELECTED, LVIS_SELECTED);
	}

	*pResult = 0;
}

void CMapEditor::OnChangeMapChannel() 
{
	if (m_selItem >= 0) {
		CString buf;
		m_MapChannel.GetWindowText(buf);
		m_mapList.SetItemText(m_selItem, 0, buf);
	}
}

void CMapEditor::OnChangeMapAddresses() 
{
	if (m_selItem >= 0) {
		CString buf;
		m_MapAddresses.GetWindowText(buf);
		m_mapList.SetItemText(m_selItem, 1, buf);
	}
}

void CMapEditor::OnKillfocusMapAddresses() 
{
	m_errorItem = -1;
	if (m_selItem >= 0)
	{
		// Validate Addresses

		CString mapping(m_mapList.GetItemText(m_selItem, 1));

		int addr, percent, pos = 0;
		while (pos >= 0 && _stscanf(mapping, _T("%d%%%d"), &addr, &percent) == 2)
		{
			if (addr < 1 || addr > dmxAddrs)
			{
				CString msg;
				msg.Format(_T("Invalid DMX Address: %d\nMust be between 1 and 512"), addr);
				MessageBox(msg, _T("Invalid DMX Address"), MB_ICONERROR);
				m_errorItem = m_selItem;
				m_MapAddresses.SetFocus();
				break;
			}
			if (percent < 0 || percent > 100)
			{
				CString msg;
				msg.Format(_T("Invalid Percentage: %d\nMust be between 0 and 100"), percent);
				MessageBox(msg, _T("Invalid Percentage"), MB_ICONERROR);
				m_errorItem = m_selItem;
				m_MapAddresses.SetFocus();
				break;
			}
			pos = mapping.Find(_T(','));
			mapping = mapping.Mid(pos + 1);
		}
	}
}

BOOL CMapEditor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_MapChannel.SetLimitText(sliderLabelMax);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
