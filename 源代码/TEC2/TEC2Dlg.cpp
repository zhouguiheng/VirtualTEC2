// TEC2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "TEC2.h"
#include "TEC2Dlg.h"
#include "cbit.h"
#include "btnst.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	CButtonST email;
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	email.SubclassDlgItem(IDC_EMAIL, this);
	email.SetURL("mailto:modengxian21@163.com");
	email.SetBtnCursor(IDC_LINK);
	email.DrawTransparent(true);
	email.DrawBorder(false);
	email.SetTooltipText("欢迎来信");
	email.SetColor(1, RGB(255,0,0));
	email.SetColor(3, RGB(0,0,255));
	email.SetColor(5, RGB(0,0,255));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTEC2Dlg dialog

CTEC2Dlg::CTEC2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTEC2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTEC2Dlg)
	//}}AFX_DATA_INIT
	for (int i=0; i<16; i++) m_D[i] = FALSE;
	for (i=0; i<4; i++) m_FS[i] = FALSE;
	for (i=0; i<3; i++) m_S[i] = FALSE;
	for (i=0; i<12; i++) m_SW1[i] = m_SW2[i] = FALSE;
	m_Way = FALSE;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTEC2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTEC2Dlg)
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_D1, m_D[0]);
	DDX_Check(pDX, IDC_D2, m_D[1]);
	DDX_Check(pDX, IDC_D3, m_D[2]);
	DDX_Check(pDX, IDC_D4, m_D[3]);
	DDX_Check(pDX, IDC_D5, m_D[4]);
	DDX_Check(pDX, IDC_D6, m_D[5]);
	DDX_Check(pDX, IDC_D7, m_D[6]);
	DDX_Check(pDX, IDC_D8, m_D[7]);
	DDX_Check(pDX, IDC_D9, m_D[8]);
	DDX_Check(pDX, IDC_D10, m_D[9]);
	DDX_Check(pDX, IDC_D11, m_D[10]);
	DDX_Check(pDX, IDC_D12, m_D[11]);
	DDX_Check(pDX, IDC_D13, m_D[12]);
	DDX_Check(pDX, IDC_D14, m_D[13]);
	DDX_Check(pDX, IDC_D15, m_D[14]);
	DDX_Check(pDX, IDC_D16, m_D[15]);
	
	DDX_Check(pDX, IDC_FS1, m_FS[3]);
	DDX_Check(pDX, IDC_FS2, m_FS[2]);
	DDX_Check(pDX, IDC_FS3, m_FS[1]);
	DDX_Check(pDX, IDC_FS4, m_FS[0]);

	DDX_Check(pDX, IDC_S0, m_S[0]);
	DDX_Check(pDX, IDC_S1, m_S[1]);
	DDX_Check(pDX, IDC_S2, m_S[2]);

	DDX_Check(pDX, IDC_SW1_0, m_SW1[0]);
	DDX_Check(pDX, IDC_SW1_1, m_SW1[1]);
	DDX_Check(pDX, IDC_SW1_2, m_SW1[2]);
	DDX_Check(pDX, IDC_SW1_3, m_SW1[3]);
	DDX_Check(pDX, IDC_SW1_4, m_SW1[4]);
	DDX_Check(pDX, IDC_SW1_5, m_SW1[5]);
	DDX_Check(pDX, IDC_SW1_6, m_SW1[6]);
	DDX_Check(pDX, IDC_SW1_7, m_SW1[7]);
	DDX_Check(pDX, IDC_SW1_8, m_SW1[8]);
	DDX_Check(pDX, IDC_SW1_9, m_SW1[9]);
	DDX_Check(pDX, IDC_SW1_10, m_SW1[10]);
	DDX_Check(pDX, IDC_SW1_11, m_SW1[11]);

	DDX_Check(pDX, IDC_SW2_0, m_SW2[0]);
	DDX_Check(pDX, IDC_SW2_1, m_SW2[1]);
	DDX_Check(pDX, IDC_SW2_2, m_SW2[2]);
	DDX_Check(pDX, IDC_SW2_3, m_SW2[3]);
	DDX_Check(pDX, IDC_SW2_4, m_SW2[4]);
	DDX_Check(pDX, IDC_SW2_5, m_SW2[5]);
	DDX_Check(pDX, IDC_SW2_6, m_SW2[6]);
	DDX_Check(pDX, IDC_SW2_7, m_SW2[7]);
	DDX_Check(pDX, IDC_SW2_8, m_SW2[8]);
	DDX_Check(pDX, IDC_SW2_9, m_SW2[9]);
	DDX_Check(pDX, IDC_SW2_10, m_SW2[10]);
	DDX_Check(pDX, IDC_SW2_11, m_SW2[11]);

	DDX_Check(pDX, IDC_WAY, m_Way);

}

BEGIN_MESSAGE_MAP(CTEC2Dlg, CDialog)
	//{{AFX_MSG_MAP(CTEC2Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_STEP, OnStep)
	ON_BN_CLICKED(IDC_S0, OnS0)
	ON_BN_CLICKED(IDC_S1, OnS1)
	ON_BN_CLICKED(IDC_S2, OnS2)
	ON_BN_CLICKED(IDC_FS1, OnFS1)
	ON_BN_CLICKED(IDC_FS2, OnFS2)
	ON_BN_CLICKED(IDC_FS3, OnFS3)
	ON_BN_CLICKED(IDC_FS4, OnFS4)
	ON_BN_CLICKED(IDC_D1, OnD1)
	ON_BN_CLICKED(IDC_D2, OnD2)
	ON_BN_CLICKED(IDC_D3, OnD3)
	ON_BN_CLICKED(IDC_D4, OnD4)
	ON_BN_CLICKED(IDC_D5, OnD5)
	ON_BN_CLICKED(IDC_D6, OnD6)
	ON_BN_CLICKED(IDC_D7, OnD7)
	ON_BN_CLICKED(IDC_D8, OnD8)
	ON_BN_CLICKED(IDC_D9, OnD9)
	ON_BN_CLICKED(IDC_D10, OnD10)
	ON_BN_CLICKED(IDC_D11, OnD11)
	ON_BN_CLICKED(IDC_D12, OnD12)
	ON_BN_CLICKED(IDC_D13, OnD13)
	ON_BN_CLICKED(IDC_D14, OnD14)
	ON_BN_CLICKED(IDC_D15, OnD15)
	ON_BN_CLICKED(IDC_D16, OnD16)
	ON_BN_CLICKED(IDC_SW1_0, OnSW1_0)
	ON_BN_CLICKED(IDC_SW1_1, OnSW1_1)
	ON_BN_CLICKED(IDC_SW1_2, OnSW1_2)
	ON_BN_CLICKED(IDC_SW1_3, OnSW1_3)
	ON_BN_CLICKED(IDC_SW1_4, OnSW1_4)
	ON_BN_CLICKED(IDC_SW1_5, OnSW1_5)
	ON_BN_CLICKED(IDC_SW1_6, OnSW1_6)
	ON_BN_CLICKED(IDC_SW1_7, OnSW1_7)
	ON_BN_CLICKED(IDC_SW1_8, OnSW1_8)
	ON_BN_CLICKED(IDC_SW1_9, OnSW1_9)
	ON_BN_CLICKED(IDC_SW1_10, OnSW1_10)
	ON_BN_CLICKED(IDC_SW1_11, OnSW1_11)
	ON_BN_CLICKED(IDC_SW2_0, OnSW2_0)
	ON_BN_CLICKED(IDC_SW2_1, OnSW2_1)
	ON_BN_CLICKED(IDC_SW2_2, OnSW2_2)
	ON_BN_CLICKED(IDC_SW2_3, OnSW2_3)
	ON_BN_CLICKED(IDC_SW2_4, OnSW2_4)
	ON_BN_CLICKED(IDC_SW2_5, OnSW2_5)
	ON_BN_CLICKED(IDC_SW2_6, OnSW2_6)
	ON_BN_CLICKED(IDC_SW2_7, OnSW2_7)
	ON_BN_CLICKED(IDC_SW2_8, OnSW2_8)
	ON_BN_CLICKED(IDC_SW2_9, OnSW2_9)
	ON_BN_CLICKED(IDC_SW2_10, OnSW2_10)
	ON_BN_CLICKED(IDC_SW2_11, OnSW2_11)
	ON_BN_CLICKED(IDC_MONITOR, OnMonitor)
	ON_COMMAND(ID_RUN_STEP, OnRunStep)
	ON_COMMAND(ID_RUN_CONT, OnRunCont)
	ON_BN_CLICKED(IDC_WAY, OnWay)
	ON_COMMAND(ID_LED_1, OnLed1)
	ON_COMMAND(ID_LED_2, OnLed2)
	ON_COMMAND(ID_LED_3, OnLed3)
	ON_COMMAND(ID_LED_4, OnLed4)
	ON_COMMAND(ID_LED_5, OnLed5)
	ON_COMMAND(ID_LED_6, OnLed6)
	ON_COMMAND(ID_LED_7, OnLed7)
	ON_COMMAND(ID_LED_8, OnLed8)
	ON_COMMAND(ID_F_1, OnF1)
	ON_COMMAND(ID_F_2, OnF2)
	ON_COMMAND(ID_F_3, OnF3)
	ON_COMMAND(ID_F_4, OnF4)
	ON_COMMAND(ID_F_5, OnF5)
	ON_COMMAND(ID_F_6, OnF6)
	ON_COMMAND(ID_F_7, OnF7)
	ON_COMMAND(ID_F_8, OnF8)
	ON_COMMAND(ID_F_9, OnF9)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_BN_CLICKED(IDC_RESET, OnReset)
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_WM_TIMER()
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_STYLE_1, OnStyle1)
	ON_COMMAND(ID_STYLE_2, OnStyle2)
	ON_COMMAND(ID_STYLE_3, OnStyle3)
	ON_COMMAND(ID_STYLE_4, OnStyle4)
	ON_COMMAND(ID_STYLE_5, OnStyle5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTEC2Dlg message handlers

BOOL CTEC2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	if (!MyTEC2.LDMC_RESET())
	{
		MessageBox("初始化错误! 请确认所在目录下有文件 MCM.ROM 和 MAPROM.ROM");
		CDialog::OnCancel();
	}
	m_btnWay.SubclassDlgItem(IDC_WAY, this);
	m_btnWay.SetIcon(IDI_SWITCHON, IDI_SWITCHOFF);
	m_btnWay.DrawBorder(FALSE);
	m_btnWay.SetBtnCursor(IDC_HAND);
	m_btnWay.DrawTransparent(true);

	int ID[16] = {IDC_D1, IDC_D2, IDC_D3, IDC_D4,
		IDC_D5, IDC_D6, IDC_D7, IDC_D8, IDC_D9, IDC_D10,
		IDC_D11, IDC_D12, IDC_D13, IDC_D14, IDC_D15, IDC_D16};
	for (int i=0; i<16; i++)
	{
		m_btnD[i].SubclassDlgItem(ID[i], this);
		m_btnD[i].SetIcon(IDI_SWITCHON, IDI_SWITCHOFF);
		m_btnD[i].DrawBorder(FALSE);
		m_btnD[i].SetBtnCursor(IDC_HAND);
		m_btnD[i].DrawTransparent(true);
	}

	ID[0] = IDC_LED1; ID[1] = IDC_LED2;
	ID[2] = IDC_LED3; ID[3] = IDC_LED4;
	ID[4] = IDC_LED5; ID[5] = IDC_LED6;
	ID[6] = IDC_LED7; ID[7] = IDC_LED8;
	ID[8] = IDC_LED9; ID[9] = IDC_LED10;
	ID[10] = IDC_LED11; ID[11] = IDC_LED12;
	ID[12] = IDC_LED13; ID[13] = IDC_LED14;
	ID[14] = IDC_LED15; ID[15] = IDC_LED16;
	for (i=0; i<16; i++)
	{
		m_btnLED[i].SubclassDlgItem(ID[i], this);
		m_btnLED[i].SetIcon(getbit(i, 2) ? IDI_LEDOFF_RED: IDI_LEDOFF_GREEN);
		m_btnLED[i].DrawBorder(FALSE);
		m_btnLED[i].DrawTransparent(true);
	}

	ID[0] = IDC_FS1; ID[1] = IDC_FS2;
	ID[2] = IDC_FS3; ID[3] = IDC_FS4;
	for (i=0; i<4; i++)
	{
		m_btnFS[i].SubclassDlgItem(ID[i], this);
		m_btnFS[i].SetIcon(IDI_SWITCHON, IDI_SWITCHOFF);
		m_btnFS[i].DrawBorder(FALSE);
		m_btnFS[i].SetBtnCursor(IDC_HAND);
		m_btnFS[i].DrawTransparent(true);
	}

	ID[0] = IDC_S0; ID[1] = IDC_S1; ID[2] = IDC_S2;
	for (i=0; i<3; i++)
	{
		m_btnS[i].SubclassDlgItem(ID[i], this);
		m_btnS[i].SetIcon(IDI_SWITCHON, IDI_SWITCHOFF);
		m_btnS[i].DrawBorder(FALSE);
		m_btnS[i].SetBtnCursor(IDC_HAND);
		m_btnS[i].DrawTransparent(true);
	}

	ID[0] = IDC_SW1_0; ID[1] = IDC_SW1_1;
	ID[2] = IDC_SW1_2; ID[3] = IDC_SW1_3;
	ID[4] = IDC_SW1_4; ID[5] = IDC_SW1_5;
	ID[6] = IDC_SW1_6; ID[7] = IDC_SW1_7;
	ID[8] = IDC_SW1_8; ID[9] = IDC_SW1_9;
	ID[10] = IDC_SW1_10; ID[11] = IDC_SW1_11;
	for (i=0; i<12; i++)
	{
		m_btnSW1[i].SubclassDlgItem(ID[i], this);
		m_btnSW1[i].SetIcon(IDI_SWON, IDI_SWOFF);
		m_btnSW1[i].DrawBorder(FALSE);
		m_btnSW1[i].SetBtnCursor(IDC_HAND);
		m_btnSW1[i].DrawTransparent(true);
	}

	ID[0] = IDC_SW2_0; ID[1] = IDC_SW2_1;
	ID[2] = IDC_SW2_2; ID[3] = IDC_SW2_3;
	ID[4] = IDC_SW2_4; ID[5] = IDC_SW2_5;
	ID[6] = IDC_SW2_6; ID[7] = IDC_SW2_7;
	ID[8] = IDC_SW2_8; ID[9] = IDC_SW2_9;
	ID[10] = IDC_SW2_10; ID[11] = IDC_SW2_11;
	for (i=0; i<12; i++)
	{
		m_btnSW2[i].SubclassDlgItem(ID[i], this);
		m_btnSW2[i].SetIcon(IDI_SWON, IDI_SWOFF);
		m_btnSW2[i].DrawBorder(FALSE);
		m_btnSW2[i].SetBtnCursor(IDC_HAND);
		m_btnSW2[i].DrawTransparent(true);
	}

	m_btnStep.SubclassDlgItem(IDC_STEP, this);
	m_btnReset.SubclassDlgItem(IDC_RESET, this);
	m_btnLDMC.SubclassDlgItem(IDC_LDMC, this);
	m_btnMonitor.SubclassDlgItem(IDC_MONITOR, this);
	m_btnExit.SubclassDlgItem(IDC_EXIT, this);

	m_btnStep.SetShade(CShadeButtonST::SHS_HARDBUMP,8,50,20,RGB(180,180,255));
	m_btnReset.SetShade(CShadeButtonST::SHS_HARDBUMP,8,50,30,RGB(180,180,255));
	m_btnLDMC.SetShade(CShadeButtonST::SHS_HARDBUMP,8,50,30,RGB(180,180,255));
	m_btnMonitor.SetShade(CShadeButtonST::SHS_HARDBUMP,8,50,45,RGB(180,180,255));
	m_btnExit.SetShade(CShadeButtonST::SHS_HARDBUMP,8,50,50,RGB(180,180,255));

	m_btnStep.SetTooltipText(_T("单步运行"));
	m_btnReset.SetTooltipText(_T("复位"));
	m_btnLDMC.SetTooltipText(_T("加载微码"));
	m_btnMonitor.SetTooltipText(_T("启动通讯程序PCEC"));
	m_btnExit.SetTooltipText(_T("退出本实验系统"));

	/*MyFont.CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
		DEFAULT_PITCH | FF_SWISS, _T("宋体"));
	
	m_btnStep.SetFont(&MyFont);
	m_btnReset.SetFont(&MyFont);
	m_btnLDMC.SetFont(&MyFont);
	m_btnMonitor.SetFont(&MyFont);
	m_btnExit.SetFont(&MyFont);*/

	m_btnStep.SetColor(1, RGB(100,100,255));
	m_btnReset.SetColor(1, RGB(100,100,255));
	m_btnLDMC.SetColor(1, RGB(100,100,255));
	m_btnMonitor.SetColor(1, RGB(100,100,255));
	m_btnExit.SetColor(1, RGB(100,100,255));

	m_btnStep.SetColor(3, RGB(0,0,255));
	m_btnReset.SetColor(3, RGB(0,0,255));
	m_btnLDMC.SetColor(3, RGB(0,0,255));
	m_btnMonitor.SetColor(3, RGB(0,0,255));
	m_btnExit.SetColor(3, RGB(0,0,255));

	m_btnStep.SetColor(5, RGB(0,0,255));
	m_btnReset.SetColor(5, RGB(0,0,255));
	m_btnLDMC.SetColor(5, RGB(0,0,255));
	m_btnMonitor.SetColor(5, RGB(0,0,255));
	m_btnExit.SetColor(5, RGB(0,0,255));

	bmpBackground.LoadBitmap(IDB_BACKGROUND);//加载背景图片

	OnRunStep();
	OnLed1();
	OnF1();
	OnStyle4();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTEC2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CTEC2Dlg::OnDestroy()
{
	WinHelp(0L, HELP_QUIT);
	CDialog::OnDestroy();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTEC2Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialog::OnPaint();
		CPaintDC dc(this);
		CRect rect;
		GetClientRect(&rect);//得到窗体的大小
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);
		BITMAP bitMap;
		bmpBackground.GetBitmap(&bitMap);
		CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground);
		//dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,bitMap.bmWidth,bitMap.bmHeight,SRCCOPY);//画窗体
		dc.BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);//画窗体
	}
}

HCURSOR CTEC2Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTEC2Dlg::Update()
{
	int i, temp;
	UpdateData();

	iFS = 0;
	for (i=3; i>=0; i--) iFS <<= 1, iFS |= m_FS[i];
	iD = 0;
	for (i=15; i>=0; i--) iD <<= 1, iD |= m_D[i];
	iSW2 = 0;
	for (i=11; i>=0; i--) iSW2 <<= 1, iSW2 |= m_SW2[i];
	iSW1 = 0;
	for (i=11; i>=0; i--) iSW1 <<= 1, iSW1 |= m_SW1[i];
	iS = 0;
	for (i=2; i>=0; i--) iS <<= 1, iS |= m_S[i];

	GetMenu()->CheckMenuRadioItem(ID_LED_1, ID_LED_8, ID_LED_1+iS, MF_CHECKED);
	if (iFS % 2 == 1) temp = ID_F_1;
	else temp = ID_F_2 + iFS / 2;
	GetMenu()->CheckMenuRadioItem(ID_F_1, ID_F_9, temp, MF_CHECKED);

	temp = MyTEC2.LED;
	if (m_FS[0]) MyTEC2.Offline(iD, iSW2, iSW1, 0);
	else if (iFS == 4 || iFS == 6)
	{
		MyTEC2.SetStatus(iFS, iD, 0, 0x0300, 0x9000, (iFS==4 ? 0:0X50));
		MyTEC2.Run(0);
	}
	MyTEC2.SetLED(m_S[2]<<2 | m_S[1]<<1 | m_S[0]);
	if (temp != MyTEC2.LED)
	{
		temp = MyTEC2.LED;
		for (i=0; i<16; i++)
		{
			if (getbit(temp, i))
			{
				m_btnLED[i].SetIcon(getbit(i, 2) ? IDI_LEDON_RED : IDI_LEDON_GREEN);
			}
			else
			{
				m_btnLED[i].SetIcon(getbit(i, 2) ? IDI_LEDOFF_RED : IDI_LEDOFF_GREEN);
			}
		}
	}
	if (iFS == 10 && m_Way) m_btnMonitor.EnableWindow(true);
	else m_btnMonitor.EnableWindow(false);
}

void CTEC2Dlg::OnStep() 
{
	// TODO: Add your control notification handler code here
	if (m_FS[0] && m_Way==0) MyTEC2.Offline(iD, iSW2, iSW1, 1);
	else if (m_Way)
	{
		switch (iFS)
		{
		case 4:		//存储器写
			MyTEC2.SetStatus(iFS, iD, 0x29, 0x0300, 0x1000, 0x0000);	//写
			MyTEC2.Run(1);
			MyTEC2.SetStatus(iFS, iD, 0x29, 0x0300, 0xB0B0, 0x5402);	//+1
			MyTEC2.Run(1);
			break;
		case 6:		//存储器读(单步)
			MyTEC2.SetStatus(iFS, iD, 0x29, 0x0300, 0x1080, 0x0000);	//读
			MyTEC2.Run(1);
			MyTEC2.SetStatus(iFS, iD, 0x29, 0x0300, 0xB0B0, 0x5452);	//+1
			MyTEC2.Run(1);
			break;
		}
	}
	Update();
}

void CTEC2Dlg::OnS0() { Update(); }
void CTEC2Dlg::OnS1() { Update(); }
void CTEC2Dlg::OnS2() { Update(); }

void CTEC2Dlg::OnFS1() { Update(); }
void CTEC2Dlg::OnFS2() { Update(); }
void CTEC2Dlg::OnFS3() { Update(); }
void CTEC2Dlg::OnFS4() { Update(); }

void CTEC2Dlg::OnD1() { Update(); }
void CTEC2Dlg::OnD2() { Update(); }
void CTEC2Dlg::OnD3() { Update(); }
void CTEC2Dlg::OnD4() { Update(); }
void CTEC2Dlg::OnD5() { Update(); }
void CTEC2Dlg::OnD6() { Update(); }
void CTEC2Dlg::OnD7() { Update(); }
void CTEC2Dlg::OnD8() { Update(); }
void CTEC2Dlg::OnD9() { Update(); }
void CTEC2Dlg::OnD10() { Update(); }
void CTEC2Dlg::OnD11() { Update(); }
void CTEC2Dlg::OnD12() { Update(); }
void CTEC2Dlg::OnD13() { Update(); }
void CTEC2Dlg::OnD14() { Update(); }
void CTEC2Dlg::OnD15() { Update(); }
void CTEC2Dlg::OnD16() { Update(); }


void CTEC2Dlg::OnSW1_0() { Update(); }
void CTEC2Dlg::OnSW1_1() { Update(); }
void CTEC2Dlg::OnSW1_2() { Update(); }
void CTEC2Dlg::OnSW1_3() { Update(); }
void CTEC2Dlg::OnSW1_4() { Update(); }
void CTEC2Dlg::OnSW1_5() { Update(); }
void CTEC2Dlg::OnSW1_6() { Update(); }
void CTEC2Dlg::OnSW1_7() { Update(); }
void CTEC2Dlg::OnSW1_8() { Update(); }
void CTEC2Dlg::OnSW1_9() { Update(); }
void CTEC2Dlg::OnSW1_10() { Update(); }
void CTEC2Dlg::OnSW1_11() { Update(); }


void CTEC2Dlg::OnSW2_0() { Update(); }
void CTEC2Dlg::OnSW2_1() { Update(); }
void CTEC2Dlg::OnSW2_2() { Update(); }
void CTEC2Dlg::OnSW2_3() { Update(); }
void CTEC2Dlg::OnSW2_4() { Update(); }
void CTEC2Dlg::OnSW2_5() { Update(); }
void CTEC2Dlg::OnSW2_6() { Update(); }
void CTEC2Dlg::OnSW2_7() { Update(); }
void CTEC2Dlg::OnSW2_8() { Update(); }
void CTEC2Dlg::OnSW2_9() { Update(); }
void CTEC2Dlg::OnSW2_10() { Update(); }
void CTEC2Dlg::OnSW2_11() { Update(); }

void CTEC2Dlg::OnMonitor() 
{
	// TODO: Add your control notification handler code here
	FILE *f = fopen("Monitor.exe", "rb");
	if (f == NULL)
	{
		MessageBox("文件丢失！请确保软件完整！");
		return;
	}
	fclose(f);
	//system("Monitor.exe");
	CString strPara;
	strPara.Format("%d", iStyle);
	ShellExecute(NULL,"open","monitor.exe",strPara.GetBuffer(2),NULL,SW_SHOWNORMAL);
}

void CTEC2Dlg::SetSwitch(int value, BOOL arr[], int n)
{
	int i;
	for (i=0; i<n; i++) arr[i] = value & 1, value >>= 1;
	UpdateData(false);
}

void CTEC2Dlg::OnRunStep()
{
	// TODO: Add your command handler code here
	GetMenu()->CheckMenuRadioItem(ID_RUN_STEP, ID_RUN_CONT, ID_RUN_STEP, MF_CHECKED);
	m_Way = 0;
	UpdateData(false);
	Update();
}

void CTEC2Dlg::OnRunCont() 
{
	// TODO: Add your command handler code here
	GetMenu()->CheckMenuRadioItem(ID_RUN_STEP, ID_RUN_CONT, ID_RUN_CONT, MF_CHECKED);
	m_Way = 1;
	UpdateData(false);
	Update();
}

void CTEC2Dlg::OnWay()
{
	// TODO: Add your control notification handler code here
	Update();
	if (m_Way) OnRunCont(); else OnRunStep();
}

void CTEC2Dlg::OnLed1()
{
	iS = 0;
	SetSwitch(iS, m_S, 3);
	Update();
}

void CTEC2Dlg::OnLed2()
{
	iS = 1;
	SetSwitch(iS, m_S, 3);
	Update();
}

void CTEC2Dlg::OnLed3()
{
	iS = 2;
	SetSwitch(iS, m_S, 3);
	Update();
}

void CTEC2Dlg::OnLed4()
{
	iS = 3;
	SetSwitch(iS, m_S, 3);
	Update();
}

void CTEC2Dlg::OnLed5()
{
	iS = 4;
	SetSwitch(iS, m_S, 3);
	Update();
}

void CTEC2Dlg::OnLed6()
{
	iS = 5;
	SetSwitch(iS, m_S, 3);
	Update();
}

void CTEC2Dlg::OnLed7()
{
	iS = 6;
	SetSwitch(iS, m_S, 3);
	Update();
}

void CTEC2Dlg::OnLed8()
{
	iS = 7;
	SetSwitch(iS, m_S, 3);
	Update();
}

void CTEC2Dlg::OnF1()
{
	iFS = 1;
	SetSwitch(iFS, m_FS, 4);
	Update();
}

void CTEC2Dlg::OnF2()
{
	iFS = 0;
	SetSwitch(iFS, m_FS, 4);
	Update();
}

void CTEC2Dlg::OnF3()
{
	iFS = 2;
	SetSwitch(iFS, m_FS, 4);
	Update();
}

void CTEC2Dlg::OnF4()
{
	iFS = 4;
	SetSwitch(iFS, m_FS, 4);
	Update();
}

void CTEC2Dlg::OnF5()
{
	iFS = 6;
	SetSwitch(iFS, m_FS, 4);
	Update();
}

void CTEC2Dlg::OnF6()
{
	iFS = 8;
	SetSwitch(iFS, m_FS, 4);
	Update();
}

void CTEC2Dlg::OnF7()
{
	iFS = 10;
	SetSwitch(iFS, m_FS, 4);
	Update();
}

void CTEC2Dlg::OnF8()
{
	iFS = 12;
	SetSwitch(iFS, m_FS, 4);
	Update();
}

void CTEC2Dlg::OnF9()
{
	iFS = 14;
	SetSwitch(iFS, m_FS, 4);
	Update();
}

void CTEC2Dlg::OnExit() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}

void CTEC2Dlg::OnReset()
{
	// TODO: Add your control notification handler code here
	MyTEC2.SetStatus(iFS, iD, 0x29, 0x0300, 0xb0f0, 0x5002);		//iD -> PC, iD -> AB
	MyTEC2.Run(1);
}

void CTEC2Dlg::OnAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlg;
	dlg.DoModal();
}

void CTEC2Dlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);
}


void CTEC2Dlg::OnHelp() 
{
	// TODO: Add your command handler code here
	//HtmlHelp(NULL, (LPCSTR)"TEC2.chm", HH_DISPLAY_TOPIC, 0);
	ShellExecute(NULL,"open","tec2.chm",NULL,NULL,SW_SHOWNORMAL);
}

void CTEC2Dlg::OnStyle1()
{
	iStyle = 0;
	GetMenu()->CheckMenuRadioItem(ID_STYLE_1, ID_STYLE_5, ID_STYLE_1+iStyle, MF_CHECKED);
}

void CTEC2Dlg::OnStyle2()
{
	iStyle = 1;
	GetMenu()->CheckMenuRadioItem(ID_STYLE_1, ID_STYLE_5, ID_STYLE_1+iStyle, MF_CHECKED);
}

void CTEC2Dlg::OnStyle3()
{
	iStyle = 2;
	GetMenu()->CheckMenuRadioItem(ID_STYLE_1, ID_STYLE_5, ID_STYLE_1+iStyle, MF_CHECKED);
}

void CTEC2Dlg::OnStyle4()
{
	iStyle = 3;
	GetMenu()->CheckMenuRadioItem(ID_STYLE_1, ID_STYLE_5, ID_STYLE_1+iStyle, MF_CHECKED);
}

void CTEC2Dlg::OnStyle5()
{
	iStyle = 4;
	GetMenu()->CheckMenuRadioItem(ID_STYLE_1, ID_STYLE_5, ID_STYLE_1+iStyle, MF_CHECKED);
}
