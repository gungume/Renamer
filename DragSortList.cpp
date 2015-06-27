// DragList.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Renamer.h"
#include "DragSortList.h"
#include ".\dragsortlist.h"
#include "RenamerDlg.h"

// CDragSortList

IMPLEMENT_DYNAMIC(CDragSortList, CListCtrl)
CDragSortList::CDragSortList()
: nDataCnt(0)
, m_bAscend(FALSE)
{
}

CDragSortList::~CDragSortList()
{
}


BEGIN_MESSAGE_MAP(CDragSortList, CListCtrl)
	ON_WM_DROPFILES()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CDragSortList::OnLvnColumnclick)
END_MESSAGE_MAP()



// CDragSortList �޽��� ó�����Դϴ�.
struct ThreadStruct
{
	HDROP hDropInfo;
	CDragSortList * m_list;
	CRenamerDlg* pDlg;
};

UINT AddDragData(LPVOID pParam)
{
	ThreadStruct* param = (ThreadStruct *)pParam;

	HDROP hDropInfo = (HDROP)param->hDropInfo;
	CDragSortList* m_list = param->m_list;
	CRenamerDlg* pDlg = param->pDlg;

	CHAR		szPath[1024];		// Drag & Drop �� ���� ���
	UINT		uiFileNum;			// Drag & Drop �� ���� ����
	CString		strItem;			// ��ϳ� �����۸� ���� ��
	BOOL		bExist = FALSE;		// Drag & Drop �� ������ ���� ��Ͽ� �ִ��� ����
	CFileFind	m_fileFinder;		// �������� ���
	BOOL		bWorking;			// CFileFind Ŭ���� ���� ���ϰ� ����

	// Drag & Drop �� ���� ���� ���
	uiFileNum = DragQueryFile(hDropInfo, 0xffffffff, NULL, 0);
	
	// ���� ���� �̿� ����� �ʱ�ȭ	
	pDlg->m_prgsTime.SetRange(0, uiFileNum);
	pDlg->m_prgsTime.SetPos(0);

	// �ڷ� �ҷ����� ���� ���콺 Ŀ�� �𷡽ð� ������� ����
	SetCursor(LoadCursor(NULL, IDC_WAIT));

	// ���� ���� ���� ����Ʈ ��Ʈ�ѿ� �߰�
	pDlg->m_strMessage = "�����߰� ��...";
	pDlg->UpdateData(FALSE);
	UINT i = 0 ;
	for(i = 0 ; i < uiFileNum ; i++ )
	{
		// ���������� ���ϰ�� ���
		DragQueryFile(hDropInfo, i, (LPTSTR)szPath, 1023);
		
		// ������ �ߺ� �Ǵ�
		LVFINDINFO info;
		info.flags = LVFI_PARTIAL|LVFI_STRING;
		info.psz = szPath;
		bExist = FALSE;
		if(m_list->FindItem(&info) > -1)
		{
			pDlg->m_strMessage.Format("��Ͽ� \"%s\"��(��) �̹� �����մϴ�.", szPath);
			pDlg->UpdateData(FALSE);

			bExist = TRUE;
		}

		// �ߺ� �ȵ� �����Ϳ� ���ؼ� ����Ʈ��Ʈ�ѿ� �߰�
		if(bExist == FALSE)
		{
			// ������ �����ϰ� ���� �����͸� �߰�
			bWorking = m_fileFinder.FindFile(szPath);
			m_fileFinder.FindNextFile();
			if(!m_fileFinder.IsDirectory())
			// if(GetFileAttributes(szPath) != FILE_ATTRIBUTE_DIRECTORY)
			{
				m_list->InsertItem(m_list->nDataCnt, szPath);
				m_list->SetItem(m_list->nDataCnt, 1, LVIF_TEXT, m_fileFinder.GetRoot(), NULL, NULL, NULL, NULL);
				m_list->SetItem(m_list->nDataCnt, 2, LVIF_TEXT, m_fileFinder.GetFileName(), NULL, NULL, NULL, NULL);
				m_list->SetItem(m_list->nDataCnt, 3, LVIF_TEXT, m_fileFinder.GetFileName(), NULL, NULL, NULL, NULL);
				m_list->nDataCnt++;

				// ����� ǥ��				
				pDlg->m_prgsTime.SetPos(i);
			}
			else
			{
				pDlg->m_strMessage = "������ ��Ͽ� �߰����� �ʽ��ϴ�.";
				pDlg->UpdateData(FALSE);
			}
			
		}
	}
	pDlg->m_prgsTime.SetPos(i);
	
	if(m_list->GetItemCount() > 0)
	{
		// �����Ͱ� �߰��Ǹ� ��ư Ȱ��ȭ
		pDlg->m_btnRename.EnableWindow(TRUE);
		pDlg->m_btnOrgName.EnableWindow(TRUE);
		pDlg->m_btnDataDelete.EnableWindow(TRUE);
		pDlg->m_btnDataReset.EnableWindow(TRUE);
	}

	SetCursor(LoadCursor(NULL, IDC_ARROW));
	pDlg->m_strMessage = "���� �߰� �Ϸ�...";
	pDlg->UpdateData(FALSE);

	delete param;
	
	return 0;
}
void CDragSortList::OnDropFiles(HDROP hDropInfo)
{

	ThreadStruct * pThreadParam = new ThreadStruct;
	pThreadParam->hDropInfo = hDropInfo;
	pThreadParam->m_list = this;
	pThreadParam->pDlg = (CRenamerDlg*)AfxGetMainWnd();

	AfxBeginThread(AddDragData, pThreadParam, THREAD_PRIORITY_HIGHEST);

	CListCtrl::OnDropFiles(hDropInfo);
}

void CDragSortList::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// ����Ʈ�� Į�� Ŭ���� �� ������ ����

	// Ŭ���� Į���� �ε���
	int nColumn = pNMLV->iSubItem;

	// ���� ����Ʈ ��Ʈ�ѿ� �ִ� ������ �� �ڷ� ������ŭ ����
	for(int i=0; i<GetItemCount(); i++)
	{
		SetItemData(i, i);
	}

	// ���� ��� ����(Ascend, Descend)
	m_bAscend = !m_bAscend;

	// ���� ���õ� ����ü ���� ���� �� ������ �ʱ�ȭ
	SORT_PARAMS sort_params;
	sort_params.hWnd = GetSafeHwnd();
	sort_params.nCol = nColumn;
	sort_params.bAscend = m_bAscend;

	// ���� �Լ� ȣ��
	SortItems(&SortFunc, (LPARAM)&sort_params );

	*pResult = 0;
}

int CALLBACK CDragSortList::SortFunc(LPARAM lParam1 , LPARAM lParam2 , LPARAM lParamSort)
{
	SORT_PARAMS *pSortParams = (SORT_PARAMS *)lParamSort;

	CListCtrl *pListCtrl = (CListCtrl *)CWnd::FromHandle(pSortParams->hWnd);
	BOOL bAscend = pSortParams->bAscend;
	int nCol = pSortParams->nCol;

	CString strItem1 = pListCtrl->GetItemText((int)lParam1, nCol);
	CString	strItem2 = pListCtrl->GetItemText((int)lParam2, nCol);

	strItem1.MakeLower();
	strItem2.MakeLower();

	if(bAscend)
		return strItem1.Compare(strItem2);
	else
		return strItem2.Compare(strItem1);
}