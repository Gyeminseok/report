
// MDBtoExcelDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MDBtoExcel.h"
#include "MDBtoExcelDlg.h"
#include "afxdialogex.h"

#include <afxdb.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_IDX 6

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


// CMDBtoExcelDlg 대화 상자



CMDBtoExcelDlg::CMDBtoExcelDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MDBTOEXCEL_DIALOG, pParent)
	, m_strPath(_T(""))
	, m_strPW(_T("B1594C47"))
	, m_record(&m_db)
	, m_sArr{0}
	, m_strCurrentTable(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMDBtoExcelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DB, m_ctrlListDB);
	DDX_Text(pDX, IDC_EDIT_FILE, m_strPath);
	DDX_Text(pDX, IDC_EDIT_PW, m_strPW);
	DDX_Control(pDX, IDC_COMBO_TABLE, m_ctrlComboTable);
	DDX_Control(pDX, IDC_COMBO_FIELD, m_ctrlComboField);
}

BEGIN_MESSAGE_MAP(CMDBtoExcelDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN, &CMDBtoExcelDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CMDBtoExcelDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_SELECT, &CMDBtoExcelDlg::OnBnClickedBtnSelect)
	ON_BN_CLICKED(IDC_BTN_ADD, &CMDBtoExcelDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BUTTON_SHOW, &CMDBtoExcelDlg::OnBnClickedButtonShow)
END_MESSAGE_MAP()


// CMDBtoExcelDlg 메시지 처리기

BOOL CMDBtoExcelDlg::OnInitDialog()
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
	m_ctrlListDB.ModifyStyle(LVS_TYPEMASK, LVS_REPORT | LBS_OWNERDRAWFIXED);
	m_ctrlListDB.SetExtendedStyle(m_ctrlListDB.GetExtendedStyle()
		| LVS_EX_GRIDLINES
		| LVS_EX_FULLROWSELECT);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMDBtoExcelDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMDBtoExcelDlg::OnPaint()
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
HCURSOR CMDBtoExcelDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMDBtoExcelDlg::OnBnClickedBtnOpen()
{
	CFileDialog dlgfile(1);
	if (dlgfile.DoModal() == IDOK)
	{
		m_strPath = dlgfile.GetPathName();
	}
	UpdateData(0);
}

// MDB 연결
void CMDBtoExcelDlg::OnBnClickedBtnConnect()
{

	UpdateData(1);
	CString strConnection;
	strConnection.Format(_T("Driver={Microsoft Access Driver (*.mdb, *.accdb)}; DBQ=%s;PWD=%s")
		, m_strPath, m_strPW);

	if (!(m_db.OpenEx(strConnection, 0)))
	{
		AfxMessageBox(_T("연결이 실패하였습니다."));
		return;
	}
	AfxMessageBox(_T("연결이 성공하였습니다."));

	m_record.Open(CRecordset::dynaset, _T("Select * from TableList"));

	CString strTable;
	int nIdx = 1;

	m_record.GetFieldValue(short(0), strTable);
	m_ctrlComboTable.AddString(strTable);
	m_ctrlComboTable.SetCurSel(0);
	m_record.MoveNext();

	while (!(m_record.IsEOF()))
	{
		m_record.GetFieldValue(short(0), strTable);
		m_ctrlComboTable.InsertString(nIdx, strTable);

		m_record.MoveNext();

		nIdx++;
	}
	
	m_record.Close();
}

// Table 선택
void CMDBtoExcelDlg::OnBnClickedBtnSelect()
{

	CString sqlCommand; // sql 명령
	
	if (m_ctrlComboField.GetCount() != 0)
	{
		m_ctrlComboField.ResetContent();
	}

	m_ctrlComboTable.GetLBText(m_ctrlComboTable.GetCurSel(), m_strCurrentTable);
	sqlCommand.Format(_T("Select * From %s"), m_strCurrentTable);

	m_record.Open(CRecordset::dynaset, sqlCommand);

	short nCount = m_record.GetODBCFieldCount();
	CODBCFieldInfo fieldInfo = { 0 };

	m_record.GetODBCFieldInfo(short(0), fieldInfo);
	m_ctrlComboField.AddString(fieldInfo.m_strName);
	m_ctrlComboField.SetCurSel(0);

	for (short i = 1; i < nCount; i++)
	{
		m_record.GetODBCFieldInfo(i, fieldInfo);
		m_ctrlComboField.InsertString(i, fieldInfo.m_strName);
	}

	m_record.Close();
}

// 필드 선택
void CMDBtoExcelDlg::OnBnClickedBtnAdd()
{
	static int nArrIdx = 0;
	CString strField;
	
	m_ctrlComboField.GetLBText(m_ctrlComboField.GetCurSel(), strField);
	m_sArr[nArrIdx].fieldEdit = (CEdit*)GetDlgItem(IDC_EDIT_FIELD1 + nArrIdx);
	m_sArr[nArrIdx].fieldEdit->SetWindowText(strField);

	m_sArr[nArrIdx].bState = TRUE;

	nArrIdx++;
	
}

void CMDBtoExcelDlg::OnBnClickedButtonShow()
{
#pragma region MyCode

	CString strSqlCommand; // sql 명령어
	CString strFieldGroup; // DB Column 집합
	CString strField;	   // Column 임시 저장

	if (m_sArr[0].bState == TRUE)
	{
		m_sArr[0].fieldEdit->GetWindowText(strField);
		strFieldGroup += strField;
		m_ctrlListDB.InsertColumn(0, strField, 0, 100);
	}

	for (int i = 1; i < MAX_IDX; i++)
	{
		if (m_sArr[i].bState == FALSE)
		{
			break;
		}

		m_sArr[i].fieldEdit->GetWindowText(strField);
		strFieldGroup += _T(",");
		strFieldGroup += strField;

		m_ctrlListDB.InsertColumn(i, strField, 0, 100);
	}

	strSqlCommand.Format(_T("Select %s from %s"), strFieldGroup, m_strCurrentTable);

	m_record.Open(CRecordset::dynaset, strSqlCommand, CRecordset::userAllocMultiRowBuffers);
	m_record.SetRowsetSize(100);

#pragma endregion

#pragma region ODBC_EX
	//
	//CFileDialog dlgfile(1);
	//if (dlgfile.DoModal() == IDOK)
	//{
	//	m_strPath = dlgfile.GetPathName();
	//}

	//CString strConnection;
	//strConnection.Format(_T("Driver={Microsoft Access Driver (*.mdb, *.accdb)}; DBQ=%s;PWD=B1594C47")
	//	, m_strPath);

	//if (!(m_db.OpenEx(strConnection, 0)))
	//{
	//	AfxMessageBox(_T("연결이 실패하였습니다."));
	//	return;
	//}
	//AfxMessageBox(_T("연결이 성공하였습니다."));

	//m_ctrlListDB.InsertColumn(0, _T("KeyIndex"), 0, 100);
	//m_ctrlListDB.InsertColumn(1, _T("Name"), 0, 100);
	//m_ctrlListDB.InsertColumn(2, _T("ValueFormat"), 0, 100);
	//m_ctrlListDB.InsertColumn(3, _T("VID"), 0, 100);

	//m_record.Open(CRecordset::dynaset, _T("select KeyIndex,NAME,ValueFormat,VID from SV_DVVAL"));

#pragma endregion

	short nFieldCount = m_record.GetODBCFieldCount();
	long nListIdx = 0;
	CString strValue;

	AfxMessageBox(m_record.GetSQL());

	while (!(m_record.IsEOF()))
	{
		
		m_record.GetFieldValue(short(0), strValue);
		m_ctrlListDB.InsertItem(nListIdx, strValue);

		for (short i = 1; i < nFieldCount; i++)
		{
			m_record.GetFieldValue(short(i), strValue);
			m_ctrlListDB.SetItemText(nListIdx, i, strValue);
		}

		nListIdx++;
		m_record.Move(1);
		//m_record.MoveNext();

		
	}

	m_record.Close();

}