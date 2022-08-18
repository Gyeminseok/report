
// MFCApplication1Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
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


// CMFCApplication1Dlg 대화 상자


CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
    , Str_Input(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EFEM, EFEM);
    DDX_Control(pDX, IDC_LL, LL);
    DDX_Control(pDX, IDC_TM, TM);
    DDX_Control(pDX, IDC_PM, PM);
    DDX_Control(pDX, IDC_DUMMY, DUMMY);
    DDX_Control(pDX, IDC_LPM, LPM);
    DDX_Control(pDX, IDC_OUTPUT, Output);
    DDX_Text(pDX, IDC_INPUT, Str_Input);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_LPM, &CMFCApplication1Dlg::OnBnClickedLpm)

    ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMFCApplication1Dlg 메시지 처리기

HANDLE g_hEvent[4];

BOOL CMFCApplication1Dlg::OnInitDialog()
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
    SetIcon(m_hIcon, TRUE);         // 큰 아이콘을 설정합니다.
    SetIcon(m_hIcon, FALSE);      // 작은 아이콘을 설정합니다.

    // TODO: 여기에 추가 초기화 작업을 추가합니다.
    
    g_hEvent[0] = CreateEvent(NULL, TRUE, FALSE, NULL);
    g_hEvent[1] = CreateEvent(NULL, TRUE, FALSE, NULL);
    g_hEvent[2] = CreateEvent(NULL, TRUE, FALSE, NULL);
    g_hEvent[3] = CreateEvent(NULL, TRUE, FALSE, NULL);
    
    return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCApplication1Dlg::OnPaint()
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
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


//=============================================================//
//                      *Code 시작구간*                         //
//=============================================================//
static int cout = -1;

DWORD WINAPI EFEM_One(LPVOID p) //EFEM Pre-Process
{
    CMFCApplication1Dlg* pEFEM_PreT = (CMFCApplication1Dlg*)p;
   
    int LPM_cnt;
    int LL_cnt;
    int EFEM_cnt;

    CString LPM_str;
    CString LL_str;
    CString EFEM_str;

    for (int i = 0; i < 3; i++)
    {
        pEFEM_PreT->LPM.GetWindowText(LPM_str);
        LPM_cnt = _ttoi(LPM_str);

        pEFEM_PreT->LL.GetWindowText(LL_str);
        LL_cnt = _ttoi(LL_str);

        int count = LPM_cnt;
        for (int i = 0; i <= count; i++)
        {
            if (LL_cnt == 6) break;

            LPM_str.Format(_T("%d"), LPM_cnt);
            pEFEM_PreT->LPM.SetWindowText(LPM_str);

            LPM_str.Format(_T("%d"), i);
            pEFEM_PreT->EFEM.SetWindowText(LPM_str);

            if (LPM_cnt == 0)break;
            LPM_cnt--;

            Sleep(100);

            if (i == 2) break;
        }
        
        pEFEM_PreT->EFEM.GetWindowText(EFEM_str);
        EFEM_cnt = _ttoi(EFEM_str);

        for (int i = LL_cnt; i <= LL_cnt + 2; i++)
        {
            if (LL_cnt == 6) break;

            EFEM_str.Format(_T("%d"), EFEM_cnt);
            pEFEM_PreT->EFEM.SetWindowText(EFEM_str);

            EFEM_str.Format(_T("%d"), i);
            pEFEM_PreT->LL.SetWindowText(EFEM_str);

            if (EFEM_cnt == 0) break;
            EFEM_cnt--;
          
            Sleep(100);
        }
    }
    
    SetEvent(g_hEvent[0]);
    
    return 0;
}

DWORD WINAPI TM_Two(LPVOID p)   //TM Pre-Process
{
    CMFCApplication1Dlg* pTM_PreT = (CMFCApplication1Dlg*)p;

    WaitForSingleObject(g_hEvent[0], INFINITE);
    int TM_cnt;
    int LL_cnt;
    int PM_cnt;

    CString TM_str;
    CString LL_str;
    CString PM_str;

    for (int i = 0; i < 3; i++)
    {
        pTM_PreT->LL.GetWindowText(LL_str);
        LL_cnt = _ttoi(LL_str);

        pTM_PreT->PM.GetWindowText(PM_str);
        PM_cnt = _ttoi(PM_str);

        int count = LL_cnt;
        for (int i = 0; i <= count; i++)
        {
            if (PM_cnt == 6) break;

            LL_str.Format(_T("%d"), LL_cnt);
            pTM_PreT->LL.SetWindowText(LL_str);

            LL_str.Format(_T("%d"), i);
            pTM_PreT->TM.SetWindowText(LL_str);

            if (LL_cnt == 0) break;
            LL_cnt--;

            Sleep(100);

            if (i == 2) break;
        }

        pTM_PreT->TM.GetWindowText(TM_str);
        TM_cnt = _ttoi(TM_str);

        for (int i = PM_cnt; i <= PM_cnt + 2; i++)
        {
            if (PM_cnt == 6) break;

            TM_str.Format(_T("%d"), TM_cnt);
            pTM_PreT->TM.SetWindowText(TM_str);

            TM_str.Format(_T("%d"), i);
            pTM_PreT->PM.SetWindowText(TM_str);
            cout++;

            if (TM_cnt == 0)break;
            TM_cnt--;
            Sleep(100);
        }
    }

    Sleep(100);

    SetEvent(g_hEvent[1]);
    return 0;
}

DWORD WINAPI TM_Three(LPVOID p) //TM Post-Process
{
    CMFCApplication1Dlg* pTM_PosT = (CMFCApplication1Dlg*)p;

    WaitForSingleObject(g_hEvent[1], INFINITE);
    int TM_cnt;
    int LL_cnt;
    int PM_cnt;

    CString TM_str;
    CString LL_str;
    CString PM_str;

    for (int i = 0; i < 3; i++)
    {
        pTM_PosT->LL.GetWindowText(LL_str);
        LL_cnt = _ttoi(LL_str);

        pTM_PosT->PM.GetWindowText(PM_str);
        PM_cnt = _ttoi(PM_str);

        int count = PM_cnt;
        pTM_PosT->TM.GetWindowText(TM_str);
        TM_cnt = _ttoi(TM_str);

        for (int i = 0; i <= cout; i++)
        {

            if (LL_cnt == 6) break;

            PM_str.Format(_T("%d"), PM_cnt);
            pTM_PosT->PM.SetWindowText(PM_str);

            PM_str.Format(_T("%d"), i);
            pTM_PosT->TM.SetWindowText(PM_str);

            if (PM_cnt == 0) break;
            PM_cnt--;

            Sleep(100);

            if (i == 2) break;
        }

        pTM_PosT->TM.GetWindowText(TM_str);
        TM_cnt = _ttoi(TM_str);

        for (int i = LL_cnt; i <= LL_cnt + 2; i++)
        {
            if (LL_cnt == 6) break;

            TM_str.Format(_T("%d"), TM_cnt);
            pTM_PosT->TM.SetWindowText(TM_str);

            TM_str.Format(_T("%d"), i);
            pTM_PosT->LL.SetWindowText(TM_str);

            if (TM_cnt == 0) break;
            TM_cnt--;

            Sleep(100);
        }
    }
    SetEvent(g_hEvent[2]);
    return 0;
}

DWORD WINAPI EFEM_Four(LPVOID p)    //EFEM Pre-Process
{
    CMFCApplication1Dlg* pEFEM_PosT = (CMFCApplication1Dlg*)p;

    WaitForSingleObject(g_hEvent[2], INFINITE);
    int LL_cnt;
    int EFEM_cnt;
    int OUTPUT_cnt;

    CString LL_str;
    CString EFEM_str;
    CString OUTPUT_str;

    for (int i = 0; i < 3; i++)
    {
        pEFEM_PosT->LL.GetWindowText(LL_str);
        LL_cnt = _ttoi(LL_str);

        pEFEM_PosT->Output.GetWindowText(OUTPUT_str);
        OUTPUT_cnt = _ttoi(OUTPUT_str);

        int count = LL_cnt;
        pEFEM_PosT->EFEM.GetWindowText(EFEM_str);
        EFEM_cnt = _ttoi(EFEM_str);

        for (int i = 0; i <= cout; i++)
        {
            LL_str.Format(_T("%d"), LL_cnt);
            pEFEM_PosT->LL.SetWindowText(LL_str);

            LL_str.Format(_T("%d"), i);
            pEFEM_PosT->EFEM.SetWindowText(LL_str);

            if (LL_cnt == 0) break;
            LL_cnt--;

            Sleep(100);

            if (i == 2) break;
        }

        pEFEM_PosT->EFEM.GetWindowText(EFEM_str);
        EFEM_cnt = _ttoi(EFEM_str);

        for (int i = OUTPUT_cnt; i <= OUTPUT_cnt + 2; i++)
        {
            EFEM_str.Format(_T("%d"), EFEM_cnt);
            pEFEM_PosT->EFEM.SetWindowText(EFEM_str);

            EFEM_str.Format(_T("%d"), i);
            pEFEM_PosT->Output.SetWindowText(EFEM_str);

            if (EFEM_cnt == 0) break;
            EFEM_cnt--;
            
            Sleep(100);
        } 
    }
    SetEvent(g_hEvent[3]);
    return 0;
}

DWORD WINAPI EFEM_O(LPVOID p) 
{
    CMFCApplication1Dlg* pEFEM_PreT = (CMFCApplication1Dlg*)p;
    
    CString LPM_str;
    int LPM_cnt;

    pEFEM_PreT->LPM.GetWindowText(LPM_str);

    if (LPM_str == _T("0"))
        pEFEM_PreT->LPM.SetWindowText(_T("25")); //Foup을 놓은 상태!

    

    pEFEM_PreT->LPM.GetWindowText(LPM_str);
    LPM_cnt = _ttoi(LPM_str);

    while (LPM_cnt!=0)
    {
        pEFEM_PreT->LPM.GetWindowText(LPM_str);
        LPM_cnt = _ttoi(LPM_str);

        //if () break;
        ResetEvent(g_hEvent[0]);
        ResetEvent(g_hEvent[1]);
        ResetEvent(g_hEvent[2]);
        ResetEvent(g_hEvent[3]);

        CloseHandle(CreateThread(NULL, 0, EFEM_One, pEFEM_PreT, 0, 0));
        CloseHandle(CreateThread(NULL, 0, TM_Two, pEFEM_PreT, 0, 0));
        CloseHandle(CreateThread(NULL, 0, TM_Three, pEFEM_PreT, 0, 0));
        CloseHandle(CreateThread(NULL, 0, EFEM_Four, pEFEM_PreT, 0, 0));
        WaitForSingleObject(g_hEvent[3], INFINITE);
    }
    return 0;
}

void CMFCApplication1Dlg::OnBnClickedLpm()
{
    UpdateData(1);
    LPM.SetWindowText(Str_Input);
    CloseHandle(CreateThread(NULL, 0, EFEM_O, this, 0, 0));
    UpdateData(0);
}

void CMFCApplication1Dlg::OnClose()
{
    CloseHandle(g_hEvent[0]);
    CloseHandle(g_hEvent[1]);
    CloseHandle(g_hEvent[2]);
    CDialogEx::OnClose();
}

