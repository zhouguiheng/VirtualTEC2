// TEC2Dlg.h : header file
//

#if !defined(AFX_TEC2DLG_H__14A96538_310D_434B_A284_88B4B966BD33__INCLUDED_)
#define AFX_TEC2DLG_H__14A96538_310D_434B_A284_88B4B966BD33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BtnST.h"
#include "ctec2.h"
#include "shadebuttonst.h"

/////////////////////////////////////////////////////////////////////////////
// CTEC2Dlg dialog

class CTEC2Dlg : public CDialog
{
// Construction
public:
	CTEC2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTEC2Dlg)
	enum { IDD = IDD_TEC2_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTEC2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	CButtonST m_btnD[16], m_btnLED[16], m_btnFS[4], m_btnS[3];
	CButtonST m_btnSW1[12], m_btnSW2[12], m_btnWay;
	CShadeButtonST m_btnStep, m_btnReset, m_btnLDMC, m_btnMonitor, m_btnExit;
	BOOL m_D[16], m_FS[4], m_S[3], m_Way, m_SW1[12], m_SW2[12];
	int iFS, iD, iSW2, iSW1, iS, iStyle;

	CTEC2 MyTEC2;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTEC2Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStep();
	afx_msg void OnS0();
	afx_msg void OnS1();
	afx_msg void OnS2();
	afx_msg void OnFS1();
	afx_msg void OnFS2();
	afx_msg void OnFS3();
	afx_msg void OnFS4();
	afx_msg void OnD1();
	afx_msg void OnD2();
	afx_msg void OnD3();
	afx_msg void OnD4();
	afx_msg void OnD5();
	afx_msg void OnD6();
	afx_msg void OnD7();
	afx_msg void OnD8();
	afx_msg void OnD9();
	afx_msg void OnD10();
	afx_msg void OnD11();
	afx_msg void OnD12();
	afx_msg void OnD13();
	afx_msg void OnD14();
	afx_msg void OnD15();
	afx_msg void OnD16();
	afx_msg void OnSW1_0();
	afx_msg void OnSW1_1();
	afx_msg void OnSW1_2();
	afx_msg void OnSW1_3();
	afx_msg void OnSW1_4();
	afx_msg void OnSW1_5();
	afx_msg void OnSW1_6();
	afx_msg void OnSW1_7();
	afx_msg void OnSW1_8();
	afx_msg void OnSW1_9();
	afx_msg void OnSW1_10();
	afx_msg void OnSW1_11();
	afx_msg void OnSW2_0();
	afx_msg void OnSW2_1();
	afx_msg void OnSW2_2();
	afx_msg void OnSW2_3();
	afx_msg void OnSW2_4();
	afx_msg void OnSW2_5();
	afx_msg void OnSW2_6();
	afx_msg void OnSW2_7();
	afx_msg void OnSW2_8();
	afx_msg void OnSW2_9();
	afx_msg void OnSW2_10();
	afx_msg void OnSW2_11();
	afx_msg void OnMonitor();
	afx_msg void OnRunStep();
	afx_msg void OnRunCont();
	afx_msg void OnWay();
	afx_msg void OnLed1();
	afx_msg void OnLed2();
	afx_msg void OnLed3();
	afx_msg void OnLed4();
	afx_msg void OnLed5();
	afx_msg void OnLed6();
	afx_msg void OnLed7();
	afx_msg void OnLed8();
	afx_msg void OnF1();
	afx_msg void OnF2();
	afx_msg void OnF3();
	afx_msg void OnF4();
	afx_msg void OnF5();
	afx_msg void OnF6();
	afx_msg void OnF7();
	afx_msg void OnF8();
	afx_msg void OnF9();
	afx_msg void OnExit();
	afx_msg void OnReset();
	afx_msg void OnAbout();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnHelp();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnStyle1();
	afx_msg void OnStyle2();
	afx_msg void OnStyle3();
	afx_msg void OnStyle4();
	afx_msg void OnStyle5();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CBitmap bmpBackground;
	CFont MyFont;

	void Update();
	void SetSwitch(int value, BOOL arr[], int n);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEC2DLG_H__14A96538_310D_434B_A284_88B4B966BD33__INCLUDED_)
