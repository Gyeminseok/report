
// MDBtoExcelDlg.h: 헤더 파일
//

#pragma once
#include <afxdb.h>

typedef struct FIELD
{
	CEdit* fieldEdit;
	BOOL bState;
}FIELD;

// CMDBtoExcelDlg 대화 상자
class CMDBtoExcelDlg : public CDialogEx
{
// 생성입니다.
public:
	CMDBtoExcelDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MDBTOEXCEL_DIALOG };
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
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedBtnSelect();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedButtonShow();

	CListCtrl m_ctrlListDB;
	CString m_strPath;
	CString m_strPW;
	CString m_strCurrentTable;

	CDatabase m_db;
	CRecordset m_record;

	CComboBox m_ctrlComboTable;
	CComboBox m_ctrlComboField;

	FIELD m_sArr[6];
};
