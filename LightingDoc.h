// LightingDoc.h : interface of the CLightingDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHTINGDOC_H__85BFBF93_24D9_4319_83D5_CC5F747971F8__INCLUDED_)
#define AFX_LIGHTINGDOC_H__85BFBF93_24D9_4319_83D5_CC5F747971F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>

class CCue {
public:
	CCue(LPCTSTR text = _T("")) : m_Text(text), m_FadeTime(0) {
		for (int i = 0; i < numSliders; ++i) {
			m_Scene[i] = 0;
		}
	}
	CCue(const CCue &src) : m_Text(src.m_Text), m_FadeTime(src.m_FadeTime) {
		for (int i = 0; i < numSliders; ++i) {
			m_Scene[i] = src.m_Scene[i];
		}
	}
	CCue& operator=(const CCue &src) {
		m_Text = src.m_Text;
		m_FadeTime = src.m_FadeTime;
		for (int i = 0; i < numSliders; ++i) {
			m_Scene[i] = src.m_Scene[i];
		}
		return *this;
	}
	CString		m_Text;
	int			m_FadeTime;
	int			m_Scene[numSliders];
};

typedef CArray<CCue,CCue> Cues;

struct Lamp {
	Lamp() : addr(0), percent(0) {}
	Lamp(int _addr, int _percent) : addr(_addr), percent(_percent) {}
	int		addr;
	int		percent;
};

class CChannel {
public:
	CString				m_Name;
	CArray<Lamp,Lamp>	m_Lamps;
};

struct Curve {
	Curve() : minVal(0), maxVal(dmxMaxVal), curve(1.0) {}
	Curve(int _min, int _max, double _curve) : minVal(_min), maxVal(_max), curve(_curve) {}
	void Reset(void) {
		minVal = 0;
		maxVal = dmxMaxVal;
		curve = 1.0;
	}
	int Apply(int x) const {
		int slope = maxVal - minVal;
		double dx = (double)x / dmxMaxVal;
		double dy = pow(dx, 1.0 / curve);
		return (int)(dy * slope + minVal + 0.5);
	}

	int		minVal;
	int		maxVal;
	double	curve;
};

struct LampType : public Curve {
	LampType() {}
	LampType(LPCTSTR _name) : name(_name) {}
	LampType(LPCTSTR _name, int _min, int _max, double _curve) : Curve(_min, _max, _curve), name(_name) {}

	CString	name;
};

typedef CArray<LampType,LampType> LampTypes;

class CLightingDoc : public CDocument
{
protected: // create from serialization only
	CLightingDoc();
	DECLARE_DYNCREATE(CLightingDoc)

// Attributes
public:
	CChannel		m_Channels[numSliders];
	Curve			m_Curves[dmxAddrs];
	Cues			m_Cues;
	Cues			m_Looks;
	LampTypes		m_LampTypes;

// Operations
public:
	void ClearDoc(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLightingDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLightingDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLightingDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIGHTINGDOC_H__85BFBF93_24D9_4319_83D5_CC5F747971F8__INCLUDED_)
