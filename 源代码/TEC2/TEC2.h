// TEC2.h : main header file for the TEC2 application
//

#if !defined(AFX_TEC2_H__6788157C_C1E3_4F3B_BD2B_41D339D4FF64__INCLUDED_)
#define AFX_TEC2_H__6788157C_C1E3_4F3B_BD2B_41D339D4FF64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTEC2App:
// See TEC2.cpp for the implementation of this class
//

class CTEC2App : public CWinApp
{
public:
	CTEC2App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTEC2App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTEC2App)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEC2_H__6788157C_C1E3_4F3B_BD2B_41D339D4FF64__INCLUDED_)
