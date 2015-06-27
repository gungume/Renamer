// RenamerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Renamer.h"
#include "RenamerDlg.h"
#include ".\renamerdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 대화 상자 데이터
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원

	// 구현
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CRenamerDlg 대화 상자



CRenamerDlg::CRenamerDlg(CWnd* pParent /*=NULL*/)
: CDialog(CRenamerDlg::IDD, pParent)
, m_strMessage(_T(""))
, m_strRename(_T(""))
, m_nStartNumber(1)
, m_bStartNumber(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRenamerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RENAMER, m_listCtrl);
	DDX_Control(pDX, IDC_PROGRESS_TIME, m_prgsTime);
	DDX_Control(pDX, IDC_BUTTON_RENAME, m_btnRename);
	DDX_Control(pDX, IDC_BUTTON_ORG_NAME, m_btnOrgName);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btnDataDelete);
	DDX_Control(pDX, IDC_BUTTON_ALL_RESET, m_btnDataReset);
	DDX_Control(pDX, IDC_EDIT_RENAME, m_ctrlEdit);

	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_strMessage);
	DDX_Text(pDX, IDC_EDIT_RENAME, m_strRename);
	DDX_Text(pDX, IDC_EDIT_START_NUMBER, m_nStartNumber);
	
	DDX_Check(pDX, IDC_CHECK_START_NUMBER, m_bStartNumber);
	DDV_MinMaxInt(pDX, m_nStartNumber, 0, 2147483640);
}

BEGIN_MESSAGE_MAP(CRenamerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDIT_RENAME, OnEnChangeEditRename)
	ON_BN_CLICKED(IDC_BUTTON_RENAME, OnBtnRename)
	ON_BN_CLICKED(IDC_BUTTON_ALL_RESET, OnBtnAllReset)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBtnDelete)
	ON_BN_CLICKED(IDC_BUTTON_ORG_NAME, OnBtnOrgName)
	ON_BN_CLICKED(IDC_CHECK_START_NUMBER, &CRenamerDlg::OnBnCheckStartNumber)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_RENAMER, OnNMRclickListRenamer)
	ON_COMMAND(ID_DELETE, OnDelete)	
	ON_EN_CHANGE(IDC_EDIT_START_NUMBER, &CRenamerDlg::OnEnChangeEditStartNumber)
END_MESSAGE_MAP()


// CRenamerDlg 메시지 처리기

BOOL CRenamerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	// 프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	// Button
	m_btnRename.EnableWindow(FALSE);
	m_btnOrgName.EnableWindow(FALSE);
	m_btnDataDelete.EnableWindow(FALSE);
	m_btnDataReset.EnableWindow(FALSE);

    // ListControl
	m_listCtrl.SetBkColor(RGB(0xe0, 0xff, 0xff));
	m_listCtrl.SetTextBkColor(RGB(0xe0, 0xff, 0xff));
	m_listCtrl.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	m_listCtrl.InsertColumn(0, "전체 경로", LVCFMT_LEFT, 0, -1);
	m_listCtrl.InsertColumn(1, "상위 경로", LVCFMT_LEFT, 0, -1);
	m_listCtrl.InsertColumn(2, "원본 파일명", LVCFMT_LEFT, 200, -1);
	m_listCtrl.InsertColumn(3, "변경 후 파일명", LVCFMT_LEFT, 200, -1);
	
	// StartNumber
	GetDlgItem(IDC_EDIT_START_NUMBER)->EnableWindow(FALSE);

	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

void CRenamerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면 
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

void CRenamerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

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
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
// 이 함수를 호출합니다. 
HCURSOR CRenamerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRenamerDlg::OnEnChangeEditRename()
{
	// 입력한 형식으로 파일명 변경...일련번호 포함
	UpdateData(TRUE);
	CString strData = m_strRename;	// 사용자 입력형식
	CString strTmp = _T("");		// 일련번호 생성에 따른 임시 변수
	CString strResult = _T("");		// 최종결과

	// 일련번호 카운터
	int nTotalCnt;
	if(m_bStartNumber == TRUE)
		nTotalCnt = m_nStartNumber;
	else if(m_bStartNumber == FALSE)
		nTotalCnt = 1;

	int nLength = 0;		// 각각의 일련번호 자릿수
	char ext[32];			// 파일확장자 저장


	// 리스트컨트롤에 있는 모든 데이터에 대해 반복...
	for(int i=0; i<m_listCtrl.GetItemCount(); i++)
	{
		// 변경할 파일명의 길이만큼 반복...
		// 기본 로직은 일반 문자열과 일련번호인 #을 구분해서 
		// strResult에 계속 저장...
		for(int j=0; j<strData.GetLength(); j++)
		{
			// 한 문자씩 불러와서 # 문자인지 여부 판단
			// # 문자인경우 연속적으로 몇개 붙어있는지 판단
			// 연속적으로 붙어있는 갯수가 곧 일련번호의 자리수
			// 예 Test ### -> Test 001
			if(strData.GetAt(j) == '#')
			{
				while(strData.GetAt(j) == '#')
				{
					nLength++;	// #문자 갯수 증가
					j++;		// 연속된 문자 판단위해 다음 문자로 이동
				}
				// while문 내에서 j값을 통해 다음 문자로 이동후에 비교를 하므로...
				// while문 탈출후 다시 위의 for문으로 돌아가면...
				// 다음, 다음 문자로 이동하므르로 문자 하나가 비교가 안되기에
				// j값 1 감소시킴
				j--;

				// 일련번호 완성
				strTmp.Format("%0*d", nLength, nTotalCnt);
				nLength=0;

				// strResult에 일련번호 데이터 추가
				strResult += strTmp;
			}
			else
			{
				// strResult에 단순 문자값 추가
				strResult += strData.GetAt(j);
			}
		}
		// 이곳까지오면 변경할 확장자가 없는 파일명 완성
		// 리스트컨트롤의 0번 칼럼은 Drag & Drop를 이용 데이터를 추가할때
		// 각 파일의 절대경로가 들어가 있음...
		// 이 정보 이용 각 파일의 확장자를 얻어올 수 있음...
		// 위에서 얻은 변경할 파일명과 원본 파일의 확장자를 합쳐서...
		// 변경할 파일명 최종 완성
		_splitpath_s(m_listCtrl.GetItemText(i, 0), 
			NULL, NULL,		NULL, NULL,		NULL, NULL,
			ext, sizeof(ext));

		strResult+=ext;

		// 변경할 파일명을 3번 칼럼에 삽입
		m_listCtrl.SetItem(i, 3, LVIF_TEXT, strResult, NULL, NULL, NULL, NULL);

		strResult.Empty();
		nTotalCnt++;
	}
}

void CRenamerDlg::OnEnChangeEditStartNumber()
{
	// 시작번호 입력시 즉각반영
	UpdateData(TRUE);
	if(!m_strRename.IsEmpty())
	{
		OnEnChangeEditRename();
	}
}

void CRenamerDlg::OnBtnRename()
{
	UpdateData(TRUE);

	if(m_strRename.IsEmpty())
		MessageBox("변경할 파일명을 입력하세요.", "오류");
	else if(m_strRename.Find("#") == -1)
		MessageBox("한개 이상의 일련번호를 입력하세요.", "오류");
	else
		RunRename();
}

void CRenamerDlg::OnBtnOrgName()
{
	if(MessageBox("최초 파일명으로 변경하시겠습니까?", "원본명으로...", MB_YESNO) == IDYES)
	{
		for(int i=0; i<m_listCtrl.GetItemCount(); i++)
		{		
			m_listCtrl.SetItem(i, 3, LVIF_TEXT, m_listCtrl.GetItemText(i, 2), NULL, NULL, NULL, NULL);
		}
		m_strRename.Empty();
		UpdateData(FALSE);

		RunRename();
	}
}

void CRenamerDlg::OnBtnDelete()
{
	POSITION pos;
	while ( pos = m_listCtrl.GetFirstSelectedItemPosition() )
	{
		int nIndex = m_listCtrl.GetNextSelectedItem(pos);
		m_listCtrl.DeleteItem(nIndex);
		m_listCtrl.nDataCnt--;

		m_strMessage = "선택한 목록 제거 완료...";
		UpdateData(FALSE);
	}

	if(m_listCtrl.nDataCnt <= 0)
	{
		OnBtnAllReset();
		m_strMessage = "목록에 자료가 없어서 모든 데이터 초기화...";
		UpdateData(FALSE);
	}
}

void CRenamerDlg::OnDelete()
{
	OnBtnDelete();
}

void CRenamerDlg::RunRename(void)
{
	CString strOldName, strNewName;

	m_prgsTime.SetRange(0, m_listCtrl.GetItemCount());
	m_prgsTime.SetPos(0);

	m_strMessage.Empty();
	UpdateData(FALSE);
	int result;
	int i=0;
	for(i=0; i<m_listCtrl.GetItemCount(); i++)
	{
		// 원본 파일명 : 0번 칼럼에 있는 데이터
		strOldName = m_listCtrl.GetItemText(i, 0);

		// - 변경될 파일명 -
		// 1번 칼럼의 데이터 : 원본 파일의 파일명을 제외한 절대경로
		// 경로와 파일을 구분짓는 "\" 문자 추가
		// 3번 칼럼의 데이터 : 변경될 파일명 추가
		strNewName = m_listCtrl.GetItemText(i, 1);
		strNewName += "\\";
		strNewName += m_listCtrl.GetItemText(i, 3);

		m_listCtrl.SetItem(i, 0, LVIF_TEXT, strNewName, NULL, NULL, NULL, NULL);
		result = rename(strOldName, strNewName);

		m_prgsTime.SetPos(i);
	}
	m_prgsTime.SetPos(i);

	m_strMessage = "파일명 변경 완료...";
	UpdateData(FALSE);
}

void CRenamerDlg::OnBtnAllReset()
{
	// 버튼 비활성화
	m_btnRename.EnableWindow(FALSE);
	m_btnOrgName.EnableWindow(FALSE);
	m_btnDataDelete.EnableWindow(FALSE);
	m_btnDataReset.EnableWindow(FALSE);

	// 일련번호 초기값
	m_bStartNumber = FALSE;
	m_nStartNumber = 1;
	GetDlgItem(IDC_EDIT_START_NUMBER)->EnableWindow(FALSE);

	m_prgsTime.SetPos(0);
	m_listCtrl.DeleteAllItems();
	m_listCtrl.nDataCnt = 0;
	m_strRename.Empty();
	m_strMessage = "모든 데이터 초기화 완료...";
	UpdateData(FALSE);
}

void CRenamerDlg::OnNMRclickListRenamer(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_listCtrl.GetItemCount() > 0)
	{
		CPoint point;
		GetCursorPos(&point);

		// Context Menu 사용 
		CMenu ConTextMenu;					// 메뉴 객체 생성
		ConTextMenu.LoadMenu(IDR_MENU);		// 메뉴 리소스 불러오기

		// 메뉴 팝업
		ConTextMenu.GetSubMenu(0)->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON,  point.x, point.y, this);
	}
	*pResult = 0;
}

void CRenamerDlg::OnBnCheckStartNumber()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if(m_bStartNumber == TRUE)
		GetDlgItem(IDC_EDIT_START_NUMBER)->EnableWindow(TRUE);
	else if(m_bStartNumber == FALSE)
		GetDlgItem(IDC_EDIT_START_NUMBER)->EnableWindow(FALSE);
	
	OnEnChangeEditRename();
}

BOOL CRenamerDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{ 
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return FALSE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}