
// gLinkBudgetDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "gLinkBudget.h"
#include "gLinkBudgetDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CgLinkBudgetDlg 대화 상자



CgLinkBudgetDlg::CgLinkBudgetDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GLINKBUDGET_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CgLinkBudgetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOG_LIST, m_log);
	DDX_Control(pDX, IDC_FREE_SPACE_PATH_LOSS_LIST, m_fsplCtrl);
	DDX_Control(pDX, IDC_RECEIVER_SIGNAL_LEVEL_LIST, m_rslCtrl);
	DDX_Control(pDX, IDC_LINK_BUDGET_LIST, m_lbCtrl);
}

BEGIN_MESSAGE_MAP(CgLinkBudgetDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CgLinkBudgetDlg 메시지 처리기

BOOL CgLinkBudgetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	TCHAR path[256] = { 0, };
	GetModuleFileName(NULL, path, 256);
	CString strFolderPath(path);
	strFolderPath = strFolderPath.Left(strFolderPath.ReverseFind(_T('\\')));
	CString strInifile = strFolderPath + _T("\\LinkBudget.ini");

	if (!LoadValue(_T("Frequency"), &m_strFrequency, strInifile)) return FALSE;
	if (!LoadValue(_T("H2OAtten"), &m_strH2OAtten, strInifile)) return FALSE;
	if (!LoadValue(_T("TxOutputPower"), &m_strTxOutputPower, strInifile)) return FALSE;
	if (!LoadValue(_T("TxAntennaGain"), &m_strTxAntennaGain, strInifile)) return FALSE;
	if (!LoadValue(_T("RxAntennaGain"), &m_strRxAntennaGain, strInifile)) return FALSE;
	if (!LoadValue(_T("1GbpsBwRxSensitivityLevel"), &m_str1GbpsBwRxSensitivityLevel, strInifile)) return FALSE;
	if (!LoadValue(_T("2GbpsBwRxSensitivityLevel"), &m_str2GbpsBwRxSensitivityLevel, strInifile)) return FALSE;
	if (!LoadValue(_T("3GbpsBwRxSensitivityLevel"), &m_str3GbpsBwRxSensitivityLevel, strInifile)) return FALSE;
	if (!LoadValue(_T("4GbpsBwRxSensitivityLevel"), &m_str4GbpsBwRxSensitivityLevel, strInifile)) return FALSE;
	if (!LoadValue(_T("5GbpsBwRxSensitivityLevel"), &m_str5GbpsBwRxSensitivityLevel, strInifile)) return FALSE;
	if (!LoadValue(_T("6GbpsBwRxSensitivityLevel"), &m_str6GbpsBwRxSensitivityLevel, strInifile)) return FALSE;
	if (!LoadValue(_T("7GbpsBwRxSensitivityLevel"), &m_str7GbpsBwRxSensitivityLevel, strInifile)) return FALSE;
	if (!LoadValue(_T("8GbpsBwRxSensitivityLevel"), &m_str8GbpsBwRxSensitivityLevel, strInifile)) return FALSE;
	if (!LoadValue(_T("9GbpsBwRxSensitivityLevel"), &m_str9GbpsBwRxSensitivityLevel, strInifile)) return FALSE;

	fillFreeSpacePathLoss();
	fillReceiverSignalLevel();
	fillLinkBudget();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CgLinkBudgetDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CgLinkBudgetDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CgLinkBudgetDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CgLinkBudgetDlg::L(const TCHAR* str, ...)
{
	va_list args;
	va_start(args, str);

	int len = _vsctprintf(str, args) + 1;	//_vscprintf doesn't count terminating '\0'
	TCHAR* buffer = new TCHAR[len];
	_vstprintf(buffer, len, str, args);
	va_end(args);

	m_log.AddString(buffer);
	delete[](buffer);

	m_log.SetTopIndex(m_log.GetCount() - 1);
}

BOOL CgLinkBudgetDlg::LoadValue(TCHAR* regTagName, CString* pTargetStr, CString fileName)
{
	TCHAR strTmp[256];
	GetPrivateProfileString(_T("reference"), regTagName, _T(""), strTmp, sizeof(strTmp) / sizeof(TCHAR), fileName);
	if (CString(strTmp).IsEmpty()) return FALSE;
	*pTargetStr = CString(strTmp);
	return TRUE;
}

void CgLinkBudgetDlg::fillFreeSpacePathLoss()
{
	CString str;
	m_fsplCtrl.InsertColumn(0, _T(""), LVCFMT_LEFT, 260);
	for (int i = 1; i <= 15; i++) {
		str.Format(_T("%d"), i);
		m_fsplCtrl.InsertColumn(i, str, LVCFMT_LEFT, 60);
	}

	int nItem = m_fsplCtrl.InsertItem(0, _T("Frequency(GHz)"));
	for (int i = 1; i <= 15; i++) m_fsplCtrl.SetItemText(nItem, i, m_strFrequency);

	int nDist = m_fsplCtrl.InsertItem(1, _T("Distance(m)"));
	m_fsplCtrl.SetItemText(nDist, 1, _T("0.1"));
	m_fsplCtrl.SetItemText(nDist, 2, _T("0.2"));
	m_fsplCtrl.SetItemText(nDist, 3, _T("0.3"));
	m_fsplCtrl.SetItemText(nDist, 4, _T("0.4"));
	m_fsplCtrl.SetItemText(nDist, 5, _T("0.5"));
	m_fsplCtrl.SetItemText(nDist, 6, _T("0.6"));
	m_fsplCtrl.SetItemText(nDist, 7, _T("0.7"));
	m_fsplCtrl.SetItemText(nDist, 8, _T("0.8"));
	m_fsplCtrl.SetItemText(nDist, 9, _T("0.9"));
	m_fsplCtrl.SetItemText(nDist,10, _T("1.0"));
	m_fsplCtrl.SetItemText(nDist,11, _T("1.5"));
	m_fsplCtrl.SetItemText(nDist,12, _T("2.0"));
	m_fsplCtrl.SetItemText(nDist,13, _T("2.5"));
	m_fsplCtrl.SetItemText(nDist,14, _T("3.0"));
	m_fsplCtrl.SetItemText(nDist,15, _T("6.0"));

	nItem = m_fsplCtrl.InsertItem(2, _T("H2O attenuation"));
	for (int i = 1; i <= 15; i++) {
		double val = _ttof(m_fsplCtrl.GetItemText(nDist, i).GetBuffer()) * _ttof(m_strH2OAtten.GetBuffer());
		str.Format(_T("%.4f"), val);
		m_fsplCtrl.SetItemText(nItem, i, str);
	}

	nItem = m_fsplCtrl.InsertItem(3, _T("FSPL"));
	for (int i = 1; i <= 15; i++) {
		str.Format(_T("%d"), i);
		m_fsplCtrl.SetItemText(nItem, i, str);
	}
}

void CgLinkBudgetDlg::fillReceiverSignalLevel()
{
	CString str;
	m_rslCtrl.InsertColumn(0, _T(""), LVCFMT_LEFT, 260);
	for (int i = 1; i <= 15; i++) {
		str.Format(_T("%d"), i);
		m_rslCtrl.InsertColumn(i, str, LVCFMT_LEFT, 50);
	}

	int nItem = m_rslCtrl.InsertItem(0, _T("Tx Output Power(dBm)"));
	for (int i = 1; i <= 15; i++) m_rslCtrl.SetItemText(nItem, i, m_strTxOutputPower);

	nItem = m_rslCtrl.InsertItem(1, _T("Tx Antenna Gain(dBi)"));
	for (int i = 1; i <= 15; i++) m_rslCtrl.SetItemText(nItem, i, m_strTxAntennaGain);

	nItem = m_rslCtrl.InsertItem(2, _T("Rx Antenna Gain(dBi)"));
	for (int i = 1; i <= 15; i++) m_rslCtrl.SetItemText(nItem, i, m_strRxAntennaGain);

	nItem = m_rslCtrl.InsertItem(3, _T("Receiver Signal Level(dBm)"));
	for (int i = 1; i <= 15; i++) {
		str.Format(_T("%d"), i);
		m_rslCtrl.SetItemText(nItem, i, str);
	}
}

void CgLinkBudgetDlg::fillLinkBudget()
{
	CString str;
	m_lbCtrl.InsertColumn(0, _T(""), LVCFMT_LEFT, 210);
	m_lbCtrl.InsertColumn(1, _T(""), LVCFMT_LEFT, 50);
	for (int i = 2; i <= 16; i++) {
		str.Format(_T("%d"), i-1);
		m_lbCtrl.InsertColumn(i, str, LVCFMT_LEFT, 50);
	}

	int nItem = m_lbCtrl.InsertItem(0, _T("1Gbps BW RX Sensitivity Level(dBm)"));
	m_lbCtrl.SetItemText(nItem, 1, m_str1GbpsBwRxSensitivityLevel);
	for (int i = 2; i <= 16; i++) m_lbCtrl.SetItemText(nItem, i, _T("-"));

	nItem = m_lbCtrl.InsertItem(1, _T("2Gbps BW RX Sensitivity Level(dBm)"));
	m_lbCtrl.SetItemText(nItem, 1, m_str2GbpsBwRxSensitivityLevel);
	for (int i = 2; i <= 16; i++) m_lbCtrl.SetItemText(nItem, i, _T("-"));

	nItem = m_lbCtrl.InsertItem(2, _T("3Gbps BW RX Sensitivity Level(dBm)"));
	m_lbCtrl.SetItemText(nItem, 1, m_str3GbpsBwRxSensitivityLevel);
	for (int i = 2; i <= 16; i++) m_lbCtrl.SetItemText(nItem, i, _T("-"));

	nItem = m_lbCtrl.InsertItem(3, _T("4Gbps BW RX Sensitivity Level(dBm)"));
	m_lbCtrl.SetItemText(nItem, 1, m_str4GbpsBwRxSensitivityLevel);
	for (int i = 2; i <= 16; i++) m_lbCtrl.SetItemText(nItem, i, _T("-"));

	nItem = m_lbCtrl.InsertItem(4, _T("5Gbps BW RX Sensitivity Level(dBm)"));
	m_lbCtrl.SetItemText(nItem, 1, m_str5GbpsBwRxSensitivityLevel);
	for (int i = 2; i <= 16; i++) m_lbCtrl.SetItemText(nItem, i, _T("-"));

	nItem = m_lbCtrl.InsertItem(5, _T("6Gbps BW RX Sensitivity Level(dBm)"));
	m_lbCtrl.SetItemText(nItem, 1, m_str6GbpsBwRxSensitivityLevel);
	for (int i = 2; i <= 16; i++) m_lbCtrl.SetItemText(nItem, i, _T("-"));

	nItem = m_lbCtrl.InsertItem(6, _T("7Gbps BW RX Sensitivity Level(dBm)"));
	m_lbCtrl.SetItemText(nItem, 1, m_str7GbpsBwRxSensitivityLevel);
	for (int i = 2; i <= 16; i++) m_lbCtrl.SetItemText(nItem, i, _T("-"));

	nItem = m_lbCtrl.InsertItem(7, _T("8Gbps BW RX Sensitivity Level(dBm)"));
	m_lbCtrl.SetItemText(nItem, 1, m_str8GbpsBwRxSensitivityLevel);
	for (int i = 2; i <= 16; i++) m_lbCtrl.SetItemText(nItem, i, _T("-"));

	nItem = m_lbCtrl.InsertItem(8, _T("9Gbps BW RX Sensitivity Level(dBm)"));
	m_lbCtrl.SetItemText(nItem, 1, m_str9GbpsBwRxSensitivityLevel);
	for (int i = 2; i <= 16; i++) m_lbCtrl.SetItemText(nItem, i, _T("-"));
}