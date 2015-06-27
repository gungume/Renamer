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
, m_SmallImage(NULL)
, m_bAscend(FALSE)
{
	m_SmallImage = new CImageList;
}

CDragSortList::~CDragSortList()
{
	if(m_SmallImage != NULL)
		delete m_SmallImage;
}


BEGIN_MESSAGE_MAP(CDragSortList, CListCtrl)
	ON_WM_DROPFILES()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CDragSortList::OnLvnColumnclick)
END_MESSAGE_MAP()



// CDragSortList �޽��� ó�����Դϴ�.


void CDragSortList::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CHAR		szPath[1024];		// Drag & Drop �� ���� ���
	UINT		uiFileNum;			// Drag & Drop �� ���� ����
	CString		strItem;			// ��ϳ� �����۸� ���� ��
	BOOL		bExist = FALSE;		// Drag & Drop �� ������ ���� ��Ͽ� �ִ��� ����
	CFileFind	m_fileFinder;		// �������� ���
	BOOL		bWorking;			// CFileFind Ŭ���� ���� ���ϰ� ����
	SHFILEINFO	shFileInfo;			// ���� ����
	CRenamerDlg* pDlg = (CRenamerDlg*)AfxGetMainWnd();	// ���� ���̾�α� ������ ���

	// Drag & Drop �� ���� ���� ���
	uiFileNum = DragQueryFile(hDropInfo, 0xffffffff, NULL, 0);
	
	// ���� ���� �̿� ����� �ʱ�ȭ	
	pDlg->m_prgsTime.SetRange(0, uiFileNum);
	pDlg->m_prgsTime.SetPos(0);

	// �ڷ� �ҷ����� ���� ���콺 Ŀ�� �𷡽ð� ������� ����
	SetCursor(LoadCursor(NULL, IDC_WAIT));

	
	// ���� ���� ���� ����Ʈ ��Ʈ�ѿ� �߰�
	UINT i = 0 ;
	for(i = 0 ; i < uiFileNum ; i++ )
	{
		// ���������� ���ϰ�� ���
		DragQueryFile(hDropInfo, i, (LPTSTR)szPath, 1023);
		
		// ������ �ߺ� �Ǵ�
		for(int j=0; j<GetItemCount(); j++)
		{
			bExist = FALSE;
			strItem = GetItemText(j, 0);
			if(strItem.Compare(szPath) == 0)
			{
				pDlg->m_strMessage.Format("��Ͽ� \"%s\"��(��) �̹� �����մϴ�.", szPath);
				pDlg->UpdateData(FALSE);

				bExist = TRUE;
				break;
			}			
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
				// ������ �ý��� �̹��� ���
				ZeroMemory(&shFileInfo, sizeof(SHFILEINFO));
				hSystemSmall = (HIMAGELIST)SHGetFileInfo( szPath, NULL, &shFileInfo, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_DISPLAYNAME | SHGFI_ICON);
				m_SmallImage->Attach( hSystemSmall );
				SetImageList(m_SmallImage, LVSIL_SMALL);
				// SHGetFileInfo(szPath, NULL, &shFileInfo, sizeof(SHFILEINFO), );

				// ���� ������ �߰�
				// ���� ��µǴ� �����ʹ� 2, 3 Į��...
				// 0, 1 Į���� ȭ�鿡 ��� ���� �ܼ��� �����ͷθ� ���...
				InsertItem(nDataCnt, szPath);
				SetItem(nDataCnt, 1, LVIF_TEXT, m_fileFinder.GetRoot(), NULL, NULL, NULL, NULL);
				SetItem(nDataCnt, 2, LVIF_TEXT | LVIF_IMAGE, shFileInfo.szDisplayName, shFileInfo.iIcon, NULL, NULL, NULL);
				SetItem(nDataCnt, 3, LVIF_TEXT, shFileInfo.szDisplayName, shFileInfo.iIcon, NULL, NULL, NULL);
				// SetItemData(nDataCnt, nDataCnt);
				nDataCnt++;

				// ����� ǥ��				
				pDlg->m_prgsTime.SetPos(i);

				m_SmallImage->Detach();

				pDlg->m_strMessage = "���� �߰� �Ϸ�...";
				pDlg->UpdateData(FALSE);
			}
			else
			{
				pDlg->m_strMessage = "������ ��Ͽ� �߰����� �ʽ��ϴ�.";
				pDlg->UpdateData(FALSE);
			}
		}
	}
	pDlg->m_prgsTime.SetPos(i);

	if(GetItemCount() > 0)
	{
		// �����Ͱ� �߰��Ǹ� ��ư Ȱ��ȭ
		pDlg->m_btnRename.EnableWindow(TRUE);
		pDlg->m_btnOrgName.EnableWindow(TRUE);
		pDlg->m_btnDataDelete.EnableWindow(TRUE);
		pDlg->m_btnDataReset.EnableWindow(TRUE);
	}

	SetCursor(LoadCursor(NULL, IDC_ARROW));

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