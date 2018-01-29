// Lighting.h : main header file for the LIGHTING application
//

#if !defined(AFX_LIGHTING_H__B9D77C1B_247F_4DCC_AE7B_7F6BCD292A14__INCLUDED_)
#define AFX_LIGHTING_H__B9D77C1B_247F_4DCC_AE7B_7F6BCD292A14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

const int	fileVersion = 0x20041107;
const int	slidersPerBank = 12;
const int	sliderBanks = 4;
const int	numSliders = slidersPerBank * sliderBanks;
const int	sliderHorizSpacing = 4;
const int	sliderVertSpacing = 2;
const int	sliderLabelMax = 20;// max chars per channel label
const int	fadeFactor = 50;	// ms per step
const int	dmxMaxVal = 255;	// 0-based max channel value
const int	dmxAddrs = 512;		// total number of DMX addresses
const int	dmxMaxChan = dmxAddrs - 1;// 0-based max channel address
const double minCurve = 1.0;	// min lamp curve
const double maxCurve = 3.0;	// max lamp curve

/////////////////////////////////////////////////////////////////////////////
// CLightingApp:
// See Lighting.cpp for the implementation of this class
//

class CLightingApp : public CWinApp
{
public:
	CLightingApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLightingApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CLightingApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIGHTING_H__B9D77C1B_247F_4DCC_AE7B_7F6BCD292A14__INCLUDED_)
