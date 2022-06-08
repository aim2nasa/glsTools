
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

	void L(const TCHAR* str, ...);
	BOOL LoadValue(TCHAR* regTagName, CString* pTargetStr, CString fileName);
};
