// Original author: Terry Wells
// Original project: https://sourceforge.net/projects/lightsup/
// Forked on Jan 29, 2018 by Daniel Powell

// LightingDoc.cpp : implementation of the CLightingDoc class
//

#include "stdafx.h"
#include "Lighting.h"

#include "LightingDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLightingDoc

IMPLEMENT_DYNCREATE(CLightingDoc, CDocument)

BEGIN_MESSAGE_MAP(CLightingDoc, CDocument)
	//{{AFX_MSG_MAP(CLightingDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLightingDoc construction/destruction

CLightingDoc::CLightingDoc()
{
}

CLightingDoc::~CLightingDoc()
{
}

BOOL CLightingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	ClearDoc();
	m_Cues.Add(CCue(_T("[End of Show]")));

	return TRUE;
}

void CLightingDoc::ClearDoc()
{
	int i;
	for (i = 0; i < dmxAddrs; ++i) {
		m_Curves[i].Reset();
	}
	for (i = 0; i < numSliders; ++i) {
		m_Channels[i].m_Name.Format("%d", i + 1);
		m_Channels[i].m_Lamps.RemoveAll();
		m_Channels[i].m_Lamps.Add(Lamp(i + 1,100));
	}
	m_Cues.RemoveAll();
}


/////////////////////////////////////////////////////////////////////////////
// CLightingDoc serialization

void CLightingDoc::Serialize(CArchive& ar)
{
	int i;

	if (ar.IsStoring())
	{
		ar << fileVersion;

		// Save Lamp Types
		int nTypes = m_LampTypes.GetSize();
		ar << nTypes;
		for (i = 0; i < nTypes; ++i) {
			LampType &type = m_LampTypes[i];
			ar << type.name;
			ar << type.minVal;
			ar << type.maxVal;
			ar << type.curve;
		}

		// Save Lamp Curves
		for (i = 0; i <= dmxMaxChan; ++i) {
			Curve &curve = m_Curves[i];
			ar << curve.minVal;
			ar << curve.maxVal;
			ar << curve.curve;
		}

		// Save Channel Map
		ar << slidersPerBank;
		ar << sliderBanks;

		for (i = 0; i < numSliders; i++) {
			CChannel &chan = m_Channels[i];
			ar << chan.m_Name;
			int nAddrs = chan.m_Lamps.GetSize();
			ar << nAddrs;
			for (int a = 0; a < nAddrs; ++a) {
				ar << chan.m_Lamps[a].addr;
				ar << chan.m_Lamps[a].percent;
			}
		}

		// Save Scene Looks
		int nLooks = m_Looks.GetSize();
		ar << nLooks;
		for (i = 0; i < nLooks; ++i) {
			CCue &look = m_Looks[i];
			ar << look.m_Text;
			for (int s = 0; s < numSliders; s++)
				ar << look.m_Scene[s];
		}

		// Save Cues
		int nCues = m_Cues.GetSize();
		ar << nCues;
		for (i = 0; i < nCues; i++) {
			CCue &cue = m_Cues[i];
			ar << cue.m_Text;
			ar << cue.m_FadeTime;
			for (int s = 0; s < numSliders; s++)
				ar << cue.m_Scene[s];
		}
	}
	else
	{
		int version;
		ar >> version;
		if (version != fileVersion
			&& version != 0x20040917
			&& version != 0x20040919) {
			AfxThrowArchiveException(CArchiveException::badSchema, ar.GetFile()->GetFileName());
		}

		ClearDoc();

		if (version >= 0x20040919) {
			// Load Lamp Types
			int nTypes;
			ar >> nTypes;
			for (i = 0; i < nTypes; ++i) {
				CString name;
				int minVal, maxVal;
				double curve;
				ar >> name;
				ar >> minVal;
				ar >> maxVal;
				ar >> curve;
				m_LampTypes.Add(LampType(name, minVal, maxVal, curve));
			}
		}

		// Load Lamp Curves
		for (i = 0; i <= dmxMaxChan; ++i) {
			Curve &curve = m_Curves[i];
			ar >> curve.minVal;
			ar >> curve.maxVal;
			ar >> curve.curve;
		}

		// Load Channel Map
		int cols, rows, numChannelsInFile;
		ar >> cols;
		ar >> rows;
		numChannelsInFile = cols * rows;
		for (i = 0; i < numChannelsInFile; ++i) {
			CChannel devnull;
			CChannel &chan = i < numSliders ? m_Channels[i] : devnull;
			ar >> chan.m_Name;
			int nAddrs;
			ar >> nAddrs;
			chan.m_Lamps.RemoveAll();
			for (int a = 0; a < nAddrs; ++a) {
				int addr, percent;
				ar >> addr;
				ar >> percent;
				chan.m_Lamps.Add(Lamp(addr, percent));
			}
		}
		for (; i < numSliders; ++i) {
			CChannel &chan = m_Channels[i];
			chan.m_Name.Empty();
			chan.m_Lamps.RemoveAll();
		}

		if (version >= 0x20040919) {
			// Load Scene Looks
			int nLooks;
			ar >> nLooks;
			for (i = 0; i < nLooks; ++i) {
				CCue cue;
				ar >> cue.m_Text;
				for (int s = 0; s < numChannelsInFile; ++s) {
					int devnull;
					int &val = s < numSliders ? cue.m_Scene[s] : devnull;
					ar >> val;
				}
				for (; s < numSliders; ++s)
					cue.m_Scene[s] = 0;
				m_Looks.Add(cue);
			}
		}

		// Load Cues
		int	numCues;
		ar >> numCues;
		for (i = 0; i < numCues; ++i) {
			CCue	cue;
			ar >> cue.m_Text;
			ar >> cue.m_FadeTime;
			for (int s = 0; s < numChannelsInFile; ++s) {
				int devnull;
				int &val = s < numSliders ? cue.m_Scene[s] : devnull;
				ar >> val;
			}
			for (; s < numSliders; ++s)
				cue.m_Scene[s] = 0;
			m_Cues.Add(cue);
		}

		if (version < 0x20041107) {
			// Need to add End of Show cue
			m_Cues.Add(CCue(_T("[End of Show]")));
		}
	}
	SetModifiedFlag(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CLightingDoc diagnostics

#ifdef _DEBUG
void CLightingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLightingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLightingDoc commands
