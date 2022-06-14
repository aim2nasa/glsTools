
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

const int MinDistance = 0;
const int MaxDistance = 6;
const int MinTxOutputPower = 0;
const int MaxTxOutputPower = 50;
const int MinPathLoss = 0;
const int MaxPathLoss = 10;
const int MinTxAntennaGain = 0;
const int MaxTxAntennaGain = 50;
const int MinRxAntennaGain = 0;
const int MaxRxAntennaGain = 50;


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
	DDX_Control(pDX, IDC_CONTROL_SLIDER, m_controlSlider);
	DDX_Text(pDX, IDC_SELECTED_STATIC, m_strSelectedStatic);
	DDX_Text(pDX, IDC_SLIDER_MIN_STATIC, m_strSliderMin);
	DDX_Text(pDX, IDC_SLIDER_MAX_STATIC, m_strSliderMax);
	DDX_Text(pDX, IDC_SLIDER_VALUE_STATIC, m_strSliderValue);
	DDX_Check(pDX, IDC_GRADIENT_CHECK, m_bGradient);
	DDX_Text(pDX, IDC_FREQUENCY_VALUE_STATIC, m_strDistFreq);
	DDX_Text(pDX, IDC_H2OATTEN_VALUE_STATIC, m_strDistH2OAtten);
	DDX_Text(pDX, IDC_FSPL_VALUE_STATIC, m_strDistFSPL);

	DDX_Text(pDX, IDC_TXOP_VALUE_STATIC, m_strDistTxOutPower);
	DDX_Text(pDX, IDC_PATHLOSS_VALUE_STATIC, m_strDistPathLoss);
	DDX_Text(pDX, IDC_TX_ANT_GAIN_VALUE_STATIC, m_strDistTxAntGain);
	DDX_Text(pDX, IDC_RX_ANT_GAIN_VALUE_STATIC, m_strDistRxAntGain);
	DDX_Text(pDX, IDC_RECEIVER_SIGNAL_LEVEL_VALUE_STATIC, m_strDistRecvSigLev);
}

BEGIN_MESSAGE_MAP(CgLinkBudgetDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LINK_BUDGET_LIST, OnNMCustomdrawLinkBudgetList)
	ON_NOTIFY(NM_CLICK, IDC_RECEIVER_SIGNAL_LEVEL_LIST, &CgLinkBudgetDlg::OnNMClkReceiverSignalLevelList)
	ON_NOTIFY(NM_CLICK, IDC_FREE_SPACE_PATH_LOSS_LIST, &CgLinkBudgetDlg::OnNMClickFreeSpacePathLossList)
	ON_NOTIFY(NM_CLICK, IDC_LINK_BUDGET_LIST, &CgLinkBudgetDlg::OnNMClickLinkBudgetList)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_GRADIENT_CHECK, &CgLinkBudgetDlg::OnBnClickedGradientCheck)
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
	CString fileName = _T("LinkBudget.ini");
	Load(fileName) ? L(fileName + _T(" loaded")) : L(fileName + _T(" load failed"));

	fillLinkBudget(fillReceiverSignalLevel(fillFreeSpacePathLoss()));

	m_strDistance.Format(_T("%.1f"), 0.);
	ShowSlider(SW_HIDE);
	ShowParams(SW_HIDE);
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

int CgLinkBudgetDlg::fillFreeSpacePathLoss()
{
	CString str;
	m_fsplCtrl.InsertColumn(0, _T(""), LVCFMT_LEFT, 260);
	for (int i = 1; i <= 15; i++) {
		str.Format(_T("%d"), i);
		m_fsplCtrl.InsertColumn(i, str, LVCFMT_LEFT, 65);
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

	int nH2oA = m_fsplCtrl.InsertItem(2, _T("H2O attenuation"));
	for (int i = 1; i <= 15; i++) {
		double val = _ttof(m_fsplCtrl.GetItemText(nDist, i).GetBuffer()) * _ttof(m_strH2OAtten.GetBuffer());
		str.Format(_T("%.4f"), val);
		m_fsplCtrl.SetItemText(nH2oA, i, str);
	}

	int nFSPL = m_fsplCtrl.InsertItem(3, _T("FSPL"));
	for (int i = 1; i <= 15; i++) {
		double freq = _ttof(m_fsplCtrl.GetItemText(nItem, i).GetBuffer());
		double dist = _ttof(m_fsplCtrl.GetItemText(nDist, i).GetBuffer());
		double h20a = _ttof(m_fsplCtrl.GetItemText(nH2oA, i).GetBuffer());
		double fspl = 20. * log10(freq) + 20. * log10(dist / 1000.) + 92.4 + h20a;
		str.Format(_T("%.4f"), fspl);
		m_fsplCtrl.SetItemText(nFSPL, i, str);
	}
	return nFSPL;
}

int CgLinkBudgetDlg::fillReceiverSignalLevel(int nFSPL)
{
	CString str;
	m_rslCtrl.InsertColumn(0, _T(""), LVCFMT_LEFT, 260);
	for (int i = 1; i <= 15; i++) {
		str.Format(_T("%d"), i);
		m_rslCtrl.InsertColumn(i, str, LVCFMT_LEFT, 65);
	}

	int nTxop = m_rslCtrl.InsertItem(0, _T("Tx Output Power(dBm)"));
	for (int i = 1; i <= 15; i++) m_rslCtrl.SetItemText(nTxop, i, m_strTxOutputPower);

	int nPtls = m_rslCtrl.InsertItem(1, _T("Path Loss(dBm)"));
	for (int i = 1; i <= 15; i++) m_rslCtrl.SetItemText(nPtls, i, m_strPathLoss);

	int nTxag = m_rslCtrl.InsertItem(2, _T("Tx Antenna Gain(dBi)"));
	for (int i = 1; i <= 15; i++) m_rslCtrl.SetItemText(nTxag, i, m_strTxAntennaGain);

	int nRxag = m_rslCtrl.InsertItem(3, _T("Rx Antenna Gain(dBi)"));
	for (int i = 1; i <= 15; i++) m_rslCtrl.SetItemText(nRxag, i, m_strRxAntennaGain);

	int nRslv = m_rslCtrl.InsertItem(4, _T("Receiver Signal Level(dBm)"));
	for (int i = 1; i <= 15; i++) {
		double txop = _ttof(m_rslCtrl.GetItemText(nTxop,i).GetBuffer());
		double ptls = _ttof(m_rslCtrl.GetItemText(nPtls, i).GetBuffer());
		double txag = _ttof(m_rslCtrl.GetItemText(nTxag,i).GetBuffer());
		double rxag = _ttof(m_rslCtrl.GetItemText(nRxag,i).GetBuffer());
		double fspl = _ttof(m_fsplCtrl.GetItemText(nFSPL,i).GetBuffer());
		double rslv = txop + txag + rxag - fspl - ptls;

		str.Format(_T("%.4f"), rslv);
		m_rslCtrl.SetItemText(nRslv, i, str);
	}
	return nRslv;
}

void CgLinkBudgetDlg::fillLinkBudget(int nRslv)
{
	CString str;
	m_lbCtrl.InsertColumn(0, _T(""), LVCFMT_LEFT, 210);
	m_lbCtrl.InsertColumn(1, _T(""), LVCFMT_LEFT, 50);
	for (int i = 2; i <= 16; i++) {
		str.Format(_T("%d"), i-1);
		m_lbCtrl.InsertColumn(i, str, LVCFMT_LEFT, 65);
	}

	int nItem = m_lbCtrl.InsertItem(0, _T("1Gbps BW RX Sensitivity Level(dBm)"));
	m_lbCtrl.SetItemText(nItem, 1, m_str1GbpsBwRxSensitivityLevel);
	comparison(m_str1GbpsBwRxSensitivityLevel, nRslv, nItem);

	nItem = m_lbCtrl.InsertItem(1, _T("2Gbps BW RX Sensitivity Level(dBm)"));
	m_lbCtrl.SetItemText(nItem, 1, m_str2GbpsBwRxSensitivityLevel);
	comparison(m_str2GbpsBwRxSensitivityLevel, nRslv, nItem);

	nItem = m_lbCtrl.InsertItem(2, _T("3Gbps BW RX Sensitivity Level(dBm)"));
	m_lbCtrl.SetItemText(nItem, 1, m_str3GbpsBwRxSensitivityLevel);
	comparison(m_str3GbpsBwRxSensitivityLevel, nRslv, nItem);

	nItem = m_lbCtrl.InsertItem(3, _T("4Gbps BW RX Sensitivity Level(dBm)"));
	m_lbCtrl.SetItemText(nItem, 1, m_str4GbpsBwRxSensitivityLevel);
	comparison(m_str4GbpsBwRxSensitivityLevel, nRslv, nItem);

	nItem = m_lbCtrl.InsertItem(4, _T("5Gbps BW RX Sensitivity Level(dBm)"));
	m_lbCtrl.SetItemText(nItem, 1, m_str5GbpsBwRxSensitivityLevel);
	comparison(m_str5GbpsBwRxSensitivityLevel, nRslv, nItem);

	nItem = m_lbCtrl.InsertItem(5, _T("6Gbps BW RX Sensitivity Level(dBm)"));
	m_lbCtrl.SetItemText(nItem, 1, m_str6GbpsBwRxSensitivityLevel);
	comparison(m_str6GbpsBwRxSensitivityLevel, nRslv, nItem);

	nItem = m_lbCtrl.InsertItem(6, _T("7Gbps BW RX Sensitivity Level(dBm)"));
	m_lbCtrl.SetItemText(nItem, 1, m_str7GbpsBwRxSensitivityLevel);
	comparison(m_str7GbpsBwRxSensitivityLevel, nRslv, nItem);

	nItem = m_lbCtrl.InsertItem(7, _T("8Gbps BW RX Sensitivity Level(dBm)"));
	m_lbCtrl.SetItemText(nItem, 1, m_str8GbpsBwRxSensitivityLevel);
	comparison(m_str8GbpsBwRxSensitivityLevel, nRslv, nItem);

	nItem = m_lbCtrl.InsertItem(8, _T("9Gbps BW RX Sensitivity Level(dBm)"));
	m_lbCtrl.SetItemText(nItem, 1, m_str9GbpsBwRxSensitivityLevel);
	comparison(m_str9GbpsBwRxSensitivityLevel, nRslv, nItem);
}

void CgLinkBudgetDlg::comparison(CString& m_strGbpsBwRxSensitivityLevel,int nRslv,int nItem)
{
	for (int i = 2; i <= 16; i++) {
		double rslv = _ttof(m_rslCtrl.GetItemText(nRslv, i - 1).GetBuffer());
		double sens = _ttof(m_strGbpsBwRxSensitivityLevel.GetBuffer());
		double diff = rslv - sens;

		CString str;
		str.Format(_T("%.4f"), diff);
		m_lbCtrl.SetItemText(nItem, i, str);
	}
}

void CgLinkBudgetDlg::OnNMCustomdrawLinkBudgetList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;

	int nRow, nSub;
	switch (lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;				// 아이템외에 일반적으로 처리하는 부분
		lplvcd->clrTextBk = RGB(0, 0, 255);
		break;

	case CDDS_ITEMPREPAINT:                         // 행 아이템에 대한 처리를 할 경우
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:			// 행과 열 아이템에 대한 처리를 할 경우
			nRow = (int)lplvcd->nmcd.dwItemSpec;	// 행 인덱스를 가져옴
			nSub = (int)lplvcd->iSubItem;           // 열 인덱스를 가져옴

			if ((nRow >= 0 && nRow <= 8) && (nSub >= 2 && nSub <= 16)) {
				double diff = _ttof(m_lbCtrl.GetItemText(nRow, nSub));

				if (m_bGradient) {
					if (diff > 0.) {
						int nBlue = (int)(255. * (diff / _ttof(m_lbCtrl.GetItemText(0, 2))));
						lplvcd->clrTextBk = RGB(255 - nBlue, 255 - nBlue, 255);
						lplvcd->clrText = RGB(0, 0, 0);
					}
					else {
						int nRed = (int)(255. * (diff / _ttof(m_lbCtrl.GetItemText(8, 16))));
						lplvcd->clrTextBk = RGB(255, 255 - nRed, 255 - nRed);
						lplvcd->clrText = RGB(0, 0, 0);
					}
				}
				else {
					if (diff > 0.) {
						lplvcd->clrTextBk = RGB(0, 0, 255);
						lplvcd->clrText = RGB(255, 255, 255);
					}
					else {
						lplvcd->clrTextBk = RGB(255, 0, 0);
						lplvcd->clrText = RGB(0, 0, 0);
					}
				}
			}
		break;
	default:
		*pResult = CDRF_DODEFAULT;
		break;
	}
}

BOOL CgLinkBudgetDlg::Load(CString fileName)
{
	TCHAR path[256] = { 0, };
	GetModuleFileName(NULL, path, 256);
	CString strFolderPath(path);
	strFolderPath = strFolderPath.Left(strFolderPath.ReverseFind(_T('\\')));
	CString strInifile = strFolderPath + _T("\\") + fileName;

	if (!LoadValue(_T("Frequency"), &m_strFrequency, strInifile)) return FALSE;
	if (!LoadValue(_T("H2OAtten"), &m_strH2OAtten, strInifile)) return FALSE;
	if (!LoadValue(_T("TxOutputPower"), &m_strTxOutputPower, strInifile)) return FALSE;
	if (!LoadValue(_T("PathLoss"), &m_strPathLoss, strInifile)) return FALSE;
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

	return TRUE;
}

void CgLinkBudgetDlg::ShowSlider(int nCmdShow)
{
	GetDlgItem(IDC_CONTROL_SLIDER)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_SLIDER_MAX_STATIC)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_SLIDER_MIN_STATIC)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_SELECTED_STATIC)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_SLIDER_VALUE_STATIC)->ShowWindow(nCmdShow);
}

void CgLinkBudgetDlg::OnNMClkReceiverSignalLevelList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int idx = pNMListView->iItem;

	if (idx >= 0 && idx <= 3) {
		switch (idx) {
		case 0:	//Tx Output Power
			SetControlSlider(10 * MinTxOutputPower, 10 * MaxTxOutputPower, m_strTxOutputPower, 10, 1, 10);
			break;
		case 1:	//Path Loss
			SetControlSlider(10 * MinPathLoss, 10 * MaxPathLoss, m_strPathLoss, 10, 1, 10);
			break;
		case 2:	//Tx Antenna Gain
			SetControlSlider(10 * MinTxAntennaGain, 10 * MaxTxAntennaGain, m_strTxAntennaGain, 10, 1, 10);
			break;
		case 3:	//Rx Antenna Gain
			SetControlSlider(10 * MinRxAntennaGain, 10 * MaxRxAntennaGain, m_strRxAntennaGain, 10, 1, 10);
			break;
		default:
			break;
		}
		m_strSelectedStatic = m_rslCtrl.GetItemText(idx, 0);
		ShowSlider(SW_SHOW);
		ShowParams(SW_HIDE);
		UpdateData(FALSE);
	}
	else {
		ShowSlider(SW_HIDE);
		ShowParams(SW_HIDE);
	}

	*pResult = 0;
}

void CgLinkBudgetDlg::SetControlSlider(int min, int max, CString strCurVal, int ticFreq, int lineSize, int pageSize)
{
	int curVal = _tcstol(strCurVal.GetBuffer(), NULL, 10);

	m_strSliderMin.Format(_T("Min:%d"), min/10);
	m_strSliderMax.Format(_T("Max:%d"), max/10);

	m_controlSlider.SetRange(min, max);
	m_controlSlider.SetRangeMin(min);
	m_controlSlider.SetRangeMax(max);
	m_controlSlider.SetPos(curVal*10);
	m_controlSlider.SetTicFreq(ticFreq);
	m_controlSlider.SetLineSize(lineSize);
	m_controlSlider.SetPageSize(pageSize);

	SliderValueUpdate();
}

int CgLinkBudgetDlg::SliderValueUpdate()
{
	int curPos = m_controlSlider.GetPos();
	m_strSliderValue.Format(_T("%.1f"), float(curPos)/10.);
	UpdateData(FALSE);
	return curPos;
}

void CgLinkBudgetDlg::OnNMClickFreeSpacePathLossList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int idx = pNMListView->iItem;

	if (idx==1) {	//Distance(m)
		SetControlSlider(10 * MinDistance, 10 * MaxDistance, m_strDistance, 10, 1, 10);
		m_strSelectedStatic = m_fsplCtrl.GetItemText(idx, 0);
		ShowSlider(SW_SHOW);
		ShowParams(SW_SHOW);
		calcLinkBudget(SliderValueUpdate());
		UpdateData(FALSE);
	}
	else {
		ShowSlider(SW_HIDE);
		ShowParams(SW_HIDE);
	}

	*pResult = 0;
}


void CgLinkBudgetDlg::OnNMClickLinkBudgetList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ShowSlider(SW_HIDE);
	ShowParams(SW_HIDE);

	*pResult = 0;
}


void CgLinkBudgetDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (IDC_CONTROL_SLIDER == pScrollBar->GetDlgCtrlID()) {
		int curPos = SliderValueUpdate();

		UpdateData(TRUE);
		if (m_strSelectedStatic == _T("Distance(m)")) {
			calcLinkBudget((double)curPos/10.);
		}
		else {
			for (int i = 0; i < m_rslCtrl.GetItemCount(); i++) {
				if (m_strSelectedStatic == m_rslCtrl.GetItemText(i, 0)) {
					switch (i) {
					case 0:
						updateStrValue(m_strTxOutputPower, m_rslCtrl, i, curPos, 1, 15);
						clearList();
						fillLinkBudget(fillReceiverSignalLevel(fillFreeSpacePathLoss()));
						break;
					case 1:
						updateStrValue(m_strPathLoss, m_rslCtrl, i, curPos, 1, 15);
						clearList();
						fillLinkBudget(fillReceiverSignalLevel(fillFreeSpacePathLoss()));
						break;
					case 2:
						updateStrValue(m_strTxAntennaGain, m_rslCtrl, i, curPos, 1, 15);
						clearList();
						fillLinkBudget(fillReceiverSignalLevel(fillFreeSpacePathLoss()));
						break;
					case 3:
						updateStrValue(m_strRxAntennaGain, m_rslCtrl, i, curPos, 1, 15);
						clearList();
						fillLinkBudget(fillReceiverSignalLevel(fillFreeSpacePathLoss()));
						break;
					}
				}
			}
		}
		UpdateData(FALSE);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CgLinkBudgetDlg::updateStrValue(CString& strValue, CListCtrl& listCtrl, int row, int curPos, int start, int end)
{
	strValue.Format(_T("%.1f"), float(curPos) / 10.);
	for (int j = start; j <= end; j++) listCtrl.SetItemText(row, j, strValue);
}

void CgLinkBudgetDlg::clearFreeSpacePathLoss()
{
	m_fsplCtrl.DeleteAllItems();
	while(m_fsplCtrl.DeleteColumn(0));
}

void CgLinkBudgetDlg::clearReceiverSignalLevel()
{
	m_rslCtrl.DeleteAllItems();
	while (m_rslCtrl.DeleteColumn(0));
}

void CgLinkBudgetDlg::clearLinkBudget()
{
	m_lbCtrl.DeleteAllItems();
	while (m_lbCtrl.DeleteColumn(0));
}

void CgLinkBudgetDlg::clearList()
{
	clearLinkBudget();
	clearReceiverSignalLevel();
	clearFreeSpacePathLoss();
}

void CgLinkBudgetDlg::OnBnClickedGradientCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bGradient = !m_bGradient;
	UpdateData(FALSE);

	clearList();
	fillLinkBudget(fillReceiverSignalLevel(fillFreeSpacePathLoss()));
}

void CgLinkBudgetDlg::ShowParams(int nCmdShow)
{
	GetDlgItem(IDC_FSPL_GROUP_STATIC)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_DISTANCE_FREQUENCY_STATIC)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_DISTANCE_H2OATTEN_STATIC)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_DISTANCE_FSPL_STATIC)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_FREQUENCY_VALUE_STATIC)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_H2OATTEN_VALUE_STATIC)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_FSPL_VALUE_STATIC)->ShowWindow(nCmdShow);

	GetDlgItem(IDC_RSL_GROUP_STATIC)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_DISTANCE_TXOP_STATIC)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_DISTANCE_PATHLOSS_STATIC)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_DISTANCE_TX_ANT_GAIN_STATIC)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_DISTANCE_RX_ANT_GAIN_STATIC)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_DISTANCE_RECEIVER_SIGNAL_LEVEL_STATIC)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_TXOP_VALUE_STATIC)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_PATHLOSS_VALUE_STATIC)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_TX_ANT_GAIN_VALUE_STATIC)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_RX_ANT_GAIN_VALUE_STATIC)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_RECEIVER_SIGNAL_LEVEL_VALUE_STATIC)->ShowWindow(nCmdShow);
}

void CgLinkBudgetDlg::calcLinkBudget(double dist)
{
	UpdateData(TRUE);

	double freq = _ttof(m_strFrequency.GetBuffer());
	double h20a = dist * _ttof(m_strH2OAtten.GetBuffer());
	double fspl = 20. * log10(freq) + 20. * log10(dist / 1000.) + 92.4 + h20a;

	m_strDistFreq.Format(_T("%.4f"), freq);
	m_strDistH2OAtten.Format(_T("%.4f"), h20a);
	m_strDistFSPL.Format(_T("%.4f"), fspl);

	double txop = _ttof(m_strTxOutputPower.GetBuffer());
	double txag = _ttof(m_strTxAntennaGain.GetBuffer());
	double rxag = _ttof(m_strRxAntennaGain.GetBuffer());
	double ptls = _ttof(m_strPathLoss.GetBuffer());
	double rslv = txop + txag + rxag - fspl - ptls;

	m_strDistTxOutPower.Format(_T("%.4f"), txop);
	m_strDistTxAntGain.Format(_T("%.4f"), txag);
	m_strDistRxAntGain.Format(_T("%.4f"), rxag);
	m_strDistPathLoss.Format(_T("%.4f"), ptls);
	m_strDistRecvSigLev.Format(_T("%.4f"), rslv);

	UpdateData(FALSE);
}