
// gLinkBudgetDlg.h: 헤더 파일
//

#pragma once


// CgLinkBudgetDlg 대화 상자
class CgLinkBudgetDlg : public CDialogEx
{
// 생성입니다.
public:
	CgLinkBudgetDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GLINKBUDGET_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CListBox m_log;
	CString m_strFrequency;
	CString m_strH2OAtten;
	CString m_strTxOutputPower;
	CString m_strPathLoss;
	CString m_strTxAntennaGain;
	CString m_strRxAntennaGain;
	CString m_str1GbpsBwRxSensitivityLevel;
	CString m_str2GbpsBwRxSensitivityLevel;
	CString m_str3GbpsBwRxSensitivityLevel;
	CString m_str4GbpsBwRxSensitivityLevel;
	CString m_str5GbpsBwRxSensitivityLevel;
	CString m_str6GbpsBwRxSensitivityLevel;
	CString m_str7GbpsBwRxSensitivityLevel;
	CString m_str8GbpsBwRxSensitivityLevel;
	CString m_str9GbpsBwRxSensitivityLevel;
	CListCtrl m_fsplCtrl;
	CListCtrl m_rslCtrl;
	CListCtrl m_lbCtrl;
	CSliderCtrl m_controlSlider;
	CString m_strSelectedStatic;
	CString m_strSliderMin;
	CString m_strSliderMax;
	CString m_strSliderValue;

	void L(const TCHAR* str, ...);
	BOOL LoadValue(TCHAR* regTagName, CString* pTargetStr, CString fileName);
	int fillFreeSpacePathLoss();
	int fillReceiverSignalLevel(int nFSPL);
	void fillLinkBudget(int nRslv);
	void comparison(CString& m_strGbpsBwRxSensitivityLevel, int nRslv, int nItem);
	afx_msg void OnNMCustomdrawLinkBudgetList(NMHDR* pNMHDR, LRESULT* pResult);
	BOOL Load(CString fileName);
	void ShowSlider(int nCmdShow);
	afx_msg void OnNMClkReceiverSignalLevelList(NMHDR* pNMHDR, LRESULT* pResult);
	void SetControlSlider(int min, int max, CString strCurVal, int ticFreq, int lineSize, int pageSize);
	int SliderValueUpdate();
	afx_msg void OnNMClickFreeSpacePathLossList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickLinkBudgetList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	static void updateStrValue(CString& strValue, CListCtrl& listCtrl, int row, int curPos, int start, int end);
};
