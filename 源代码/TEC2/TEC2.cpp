// TEC2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TEC2.h"
#include "TEC2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTEC2App

BEGIN_MESSAGE_MAP(CTEC2App, CWinApp)
	//{{AFX_MSG_MAP(CTEC2App)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTEC2App construction

CTEC2App::CTEC2App()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTEC2App object

CTEC2App theApp;

/////////////////////////////////////////////////////////////////////////////
// CTEC2App initialization

BOOL CTEC2App::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CTEC2Dlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
