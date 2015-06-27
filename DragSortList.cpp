// DragList.cpp : 구현 파일입니다.
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



// CDragSortList 메시지 처리기입니다.
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

	CHAR		szPath[1024];		// Drag & Drop 된 파일 경로
	UINT		uiFileNum;			// Drag & Drop 된 파일 갯수
	CString		strItem;			// 목록내 아이템명 얻어올 때
	BOOL		bExist = FALSE;		// Drag & Drop 한 파일이 기존 목록에 있는지 유무
	CFileFind	m_fileFinder;		// 파일정보 얻기
	BOOL		bWorking;			// CFileFind 클래스 사용시 리턴값 저장

	// Drag & Drop 된 파일 정보 얻기
	uiFileNum = DragQueryFile(hDropInfo, 0xffffffff, NULL, 0);
	
	// 파일 갯수 이용 진행률 초기화	
	pDlg->m_prgsTime.SetRange(0, uiFileNum);
	pDlg->m_prgsTime.SetPos(0);

	// 자료 불러오는 동안 마우스 커서 모래시계 모양으로 변경
	SetCursor(LoadCursor(NULL, IDC_WAIT));

	// 얻어온 파일 정보 리스트 컨트롤에 추가
	pDlg->m_strMessage = "파일추가 중...";
	pDlg->UpdateData(FALSE);
	UINT i = 0 ;
	for(i = 0 ; i < uiFileNum ; i++ )
	{
		// 순차적으로 파일경로 얻기
		DragQueryFile(hDropInfo, i, (LPTSTR)szPath, 1023);
		
		// 데이터 중복 판단
		LVFINDINFO info;
		info.flags = LVFI_PARTIAL|LVFI_STRING;
		info.psz = szPath;
		bExist = FALSE;
		if(m_list->FindItem(&info) > -1)
		{
			pDlg->m_strMessage.Format("목록에 \"%s\"는(은) 이미 존재합니다.", szPath);
			pDlg->UpdateData(FALSE);

			bExist = TRUE;
		}

		// 중복 안된 데이터에 대해서 리스트컨트롤에 추가
		if(bExist == FALSE)
		{
			// 폴더는 제외하고 파일 데이터만 추가
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

				// 진행률 표시				
				pDlg->m_prgsTime.SetPos(i);
			}
			else
			{
				pDlg->m_strMessage = "폴더는 목록에 추가되지 않습니다.";
				pDlg->UpdateData(FALSE);
			}
			
		}
	}
	pDlg->m_prgsTime.SetPos(i);
	
	if(m_list->GetItemCount() > 0)
	{
		// 데이터가 추가되면 버튼 활성화
		pDlg->m_btnRename.EnableWindow(TRUE);
		pDlg->m_btnOrgName.EnableWindow(TRUE);
		pDlg->m_btnDataDelete.EnableWindow(TRUE);
		pDlg->m_btnDataReset.EnableWindow(TRUE);
	}

	SetCursor(LoadCursor(NULL, IDC_ARROW));
	pDlg->m_strMessage = "파일 추가 완료...";
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 리스트뷰 칼럼 클릭할 때 데이터 정렬

	// 클릭한 칼럼의 인덱스
	int nColumn = pNMLV->iSubItem;

	// 현재 리스트 컨트롤에 있는 데이터 총 자료 개수만큼 저장
	for(int i=0; i<GetItemCount(); i++)
	{
		SetItemData(i, i);
	}

	// 정렬 방식 지정(Ascend, Descend)
	m_bAscend = !m_bAscend;

	// 정렬 관련된 구조체 변수 생성 및 데이터 초기화
	SORT_PARAMS sort_params;
	sort_params.hWnd = GetSafeHwnd();
	sort_params.nCol = nColumn;
	sort_params.bAscend = m_bAscend;

	// 정렬 함수 호출
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