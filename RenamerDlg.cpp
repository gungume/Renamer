// RenamerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "Renamer.h"
#include "RenamerDlg.h"
#include ".\renamerdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// ��ȭ ���� ������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ����

	// ����
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


// CRenamerDlg ��ȭ ����



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


// CRenamerDlg �޽��� ó����

BOOL CRenamerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	// �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// Button
	m_btnRename.EnableWindow(FALSE);
	m_btnOrgName.EnableWindow(FALSE);
	m_btnDataDelete.EnableWindow(FALSE);
	m_btnDataReset.EnableWindow(FALSE);

    // ListControl
	m_listCtrl.SetBkColor(RGB(0xe0, 0xff, 0xff));
	m_listCtrl.SetTextBkColor(RGB(0xe0, 0xff, 0xff));
	m_listCtrl.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	m_listCtrl.InsertColumn(0, "��ü ���", LVCFMT_LEFT, 0, -1);
	m_listCtrl.InsertColumn(1, "���� ���", LVCFMT_LEFT, 0, -1);
	m_listCtrl.InsertColumn(2, "���� ���ϸ�", LVCFMT_LEFT, 200, -1);
	m_listCtrl.InsertColumn(3, "���� �� ���ϸ�", LVCFMT_LEFT, 200, -1);
	
	// StartNumber
	GetDlgItem(IDC_EDIT_START_NUMBER)->EnableWindow(FALSE);

	return TRUE;  // ��Ʈ�ѿ� ���� ��Ŀ���� �������� ���� ��� TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸����� 
// �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
// �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CRenamerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
// �� �Լ��� ȣ���մϴ�. 
HCURSOR CRenamerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRenamerDlg::OnEnChangeEditRename()
{
	// �Է��� �������� ���ϸ� ����...�Ϸù�ȣ ����
	UpdateData(TRUE);
	CString strData = m_strRename;	// ����� �Է�����
	CString strTmp = _T("");		// �Ϸù�ȣ ������ ���� �ӽ� ����
	CString strResult = _T("");		// �������

	// �Ϸù�ȣ ī����
	int nTotalCnt;
	if(m_bStartNumber == TRUE)
		nTotalCnt = m_nStartNumber;
	else if(m_bStartNumber == FALSE)
		nTotalCnt = 1;

	int nLength = 0;		// ������ �Ϸù�ȣ �ڸ���
	char ext[32];			// ����Ȯ���� ����


	// ����Ʈ��Ʈ�ѿ� �ִ� ��� �����Ϳ� ���� �ݺ�...
	for(int i=0; i<m_listCtrl.GetItemCount(); i++)
	{
		// ������ ���ϸ��� ���̸�ŭ �ݺ�...
		// �⺻ ������ �Ϲ� ���ڿ��� �Ϸù�ȣ�� #�� �����ؼ� 
		// strResult�� ��� ����...
		for(int j=0; j<strData.GetLength(); j++)
		{
			// �� ���ھ� �ҷ��ͼ� # �������� ���� �Ǵ�
			// # �����ΰ�� ���������� � �پ��ִ��� �Ǵ�
			// ���������� �پ��ִ� ������ �� �Ϸù�ȣ�� �ڸ���
			// �� Test ### -> Test 001
			if(strData.GetAt(j) == '#')
			{
				while(strData.GetAt(j) == '#')
				{
					nLength++;	// #���� ���� ����
					j++;		// ���ӵ� ���� �Ǵ����� ���� ���ڷ� �̵�
				}
				// while�� ������ j���� ���� ���� ���ڷ� �̵��Ŀ� �񱳸� �ϹǷ�...
				// while�� Ż���� �ٽ� ���� for������ ���ư���...
				// ����, ���� ���ڷ� �̵��ϹǸ��� ���� �ϳ��� �񱳰� �ȵǱ⿡
				// j�� 1 ���ҽ�Ŵ
				j--;

				// �Ϸù�ȣ �ϼ�
				strTmp.Format("%0*d", nLength, nTotalCnt);
				nLength=0;

				// strResult�� �Ϸù�ȣ ������ �߰�
				strResult += strTmp;
			}
			else
			{
				// strResult�� �ܼ� ���ڰ� �߰�
				strResult += strData.GetAt(j);
			}
		}
		// �̰��������� ������ Ȯ���ڰ� ���� ���ϸ� �ϼ�
		// ����Ʈ��Ʈ���� 0�� Į���� Drag & Drop�� �̿� �����͸� �߰��Ҷ�
		// �� ������ �����ΰ� �� ����...
		// �� ���� �̿� �� ������ Ȯ���ڸ� ���� �� ����...
		// ������ ���� ������ ���ϸ�� ���� ������ Ȯ���ڸ� ���ļ�...
		// ������ ���ϸ� ���� �ϼ�
		_splitpath_s(m_listCtrl.GetItemText(i, 0), 
			NULL, NULL,		NULL, NULL,		NULL, NULL,
			ext, sizeof(ext));

		strResult+=ext;

		// ������ ���ϸ��� 3�� Į���� ����
		m_listCtrl.SetItem(i, 3, LVIF_TEXT, strResult, NULL, NULL, NULL, NULL);

		strResult.Empty();
		nTotalCnt++;
	}
}

void CRenamerDlg::OnEnChangeEditStartNumber()
{
	// ���۹�ȣ �Է½� �ﰢ�ݿ�
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
		MessageBox("������ ���ϸ��� �Է��ϼ���.", "����");
	else if(m_strRename.Find("#") == -1)
		MessageBox("�Ѱ� �̻��� �Ϸù�ȣ�� �Է��ϼ���.", "����");
	else
		RunRename();
}

void CRenamerDlg::OnBtnOrgName()
{
	if(MessageBox("���� ���ϸ����� �����Ͻðڽ��ϱ�?", "����������...", MB_YESNO) == IDYES)
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

		m_strMessage = "������ ��� ���� �Ϸ�...";
		UpdateData(FALSE);
	}

	if(m_listCtrl.nDataCnt <= 0)
	{
		OnBtnAllReset();
		m_strMessage = "��Ͽ� �ڷᰡ ��� ��� ������ �ʱ�ȭ...";
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
		// ���� ���ϸ� : 0�� Į���� �ִ� ������
		strOldName = m_listCtrl.GetItemText(i, 0);

		// - ����� ���ϸ� -
		// 1�� Į���� ������ : ���� ������ ���ϸ��� ������ ������
		// ��ο� ������ �������� "\" ���� �߰�
		// 3�� Į���� ������ : ����� ���ϸ� �߰�
		strNewName = m_listCtrl.GetItemText(i, 1);
		strNewName += "\\";
		strNewName += m_listCtrl.GetItemText(i, 3);

		m_listCtrl.SetItem(i, 0, LVIF_TEXT, strNewName, NULL, NULL, NULL, NULL);
		result = rename(strOldName, strNewName);

		m_prgsTime.SetPos(i);
	}
	m_prgsTime.SetPos(i);

	m_strMessage = "���ϸ� ���� �Ϸ�...";
	UpdateData(FALSE);
}

void CRenamerDlg::OnBtnAllReset()
{
	// ��ư ��Ȱ��ȭ
	m_btnRename.EnableWindow(FALSE);
	m_btnOrgName.EnableWindow(FALSE);
	m_btnDataDelete.EnableWindow(FALSE);
	m_btnDataReset.EnableWindow(FALSE);

	// �Ϸù�ȣ �ʱⰪ
	m_bStartNumber = FALSE;
	m_nStartNumber = 1;
	GetDlgItem(IDC_EDIT_START_NUMBER)->EnableWindow(FALSE);

	m_prgsTime.SetPos(0);
	m_listCtrl.DeleteAllItems();
	m_listCtrl.nDataCnt = 0;
	m_strRename.Empty();
	m_strMessage = "��� ������ �ʱ�ȭ �Ϸ�...";
	UpdateData(FALSE);
}

void CRenamerDlg::OnNMRclickListRenamer(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(m_listCtrl.GetItemCount() > 0)
	{
		CPoint point;
		GetCursorPos(&point);

		// Context Menu ��� 
		CMenu ConTextMenu;					// �޴� ��ü ����
		ConTextMenu.LoadMenu(IDR_MENU);		// �޴� ���ҽ� �ҷ�����

		// �޴� �˾�
		ConTextMenu.GetSubMenu(0)->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON,  point.x, point.y, this);
	}
	*pResult = 0;
}

void CRenamerDlg::OnBnCheckStartNumber()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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