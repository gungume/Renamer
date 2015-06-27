// DragList.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Renamer.h"
#include "DragList.h"
#include ".\draglist.h"
#include "RenamerDlg.h"

// CDragList

IMPLEMENT_DYNAMIC(CDragList, CListCtrl)
CDragList::CDragList()
: nDataCnt(0)
, m_SmallImage(NULL)
{
	m_SmallImage = new CImageList;
}

CDragList::~CDragList()
{
	if(m_SmallImage != NULL)
		delete m_SmallImage;
}


BEGIN_MESSAGE_MAP(CDragList, CListCtrl)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()



// CDragList 메시지 처리기입니다.


void CDragList::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CHAR		szPath[1024];		// Drag & Drop 된 파일 경로
	UINT		uiFileNum;			// Drag & Drop 된 파일 갯수
	CString		strItem;			// 목록내 아이템명 얻어올 때
	BOOL		bExist = FALSE;		// Drag & Drop 한 파일이 기존 목록에 있는지 유무
	CFileFind	m_fileFinder;		// 파일정보 얻기
	BOOL		bWorking;			// CFileFind 클래스 사용시 리턴값 저장
	SHFILEINFO	shFileInfo;			// 파일 정보
	CRenamerDlg* pDlg = (CRenamerDlg*)AfxGetMainWnd();	// 메인 다이얼로그 포인터 얻기

	
	// Drag & Drop 된 파일 정보 얻기
	uiFileNum = DragQueryFile(hDropInfo, 0xffffffff, NULL, 0);
	
	// 파일 갯수 이용 진행률 초기화	
	pDlg->m_prgsTime.SetRange(0, uiFileNum);
	pDlg->m_prgsTime.SetPos(0);

	// 자료 불러오는 동안 마우스 커서 모래시계 모양으로 변경
	SetCursor(LoadCursor(NULL, IDC_WAIT));

	
	// 얻어온 파일 정보 리스트 컨트롤에 추가
	for( UINT i = 0 ; i < uiFileNum ; i++ )
	{
		// 순차적으로 파일경로 얻기
		DragQueryFile(hDropInfo, i, (LPTSTR)szPath, 1023);
		
		// 데이터 중복 판단
		for(int j=0; j<GetItemCount(); j++)
		{
			bExist = FALSE;
			strItem = GetItemText(j, 0);
			if(strItem.Compare(szPath) == 0)
			{
				pDlg->m_strMessage.Format("목록에 \"%s\"는(은) 이미 존재합니다.", szPath);
				pDlg->UpdateData(FALSE);

				bExist = TRUE;
				break;
			}			
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
				// 파일의 시스템 이미지 얻기
				ZeroMemory(&shFileInfo, sizeof(SHFILEINFO));
				hSystemSmall = (HIMAGELIST)SHGetFileInfo( szPath, NULL, &shFileInfo, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_DISPLAYNAME | SHGFI_ICON);
				m_SmallImage->Attach( hSystemSmall );
				SetImageList(m_SmallImage, LVSIL_SMALL);
				// SHGetFileInfo(szPath, NULL, &shFileInfo, sizeof(SHFILEINFO), );

				// 얻어온 데이터 추가
				// 실제 출력되는 데이터는 2, 3 칼럼...
				// 0, 1 칼럼은 화면에 출력 없이 단순히 데이터로만 사용...
				InsertItem(nDataCnt, szPath);
				SetItem(nDataCnt, 1, LVIF_TEXT, m_fileFinder.GetRoot(), NULL, NULL, NULL, NULL);
				SetItem(nDataCnt, 2, LVIF_TEXT | LVIF_IMAGE, shFileInfo.szDisplayName, shFileInfo.iIcon, NULL, NULL, NULL);
				SetItem(nDataCnt, 3, LVIF_TEXT, shFileInfo.szDisplayName, shFileInfo.iIcon, NULL, NULL, NULL);
				nDataCnt++;

				// 진행률 표시				
				pDlg->m_prgsTime.SetPos(i);
				Sleep(10);

				m_SmallImage->Detach();

				pDlg->m_strMessage = "파일 추가 완료...";
				pDlg->UpdateData(FALSE);
			}
			else
			{
				pDlg->m_strMessage = "폴더는 목록에 추가되지 않습니다.";
				pDlg->UpdateData(FALSE);
			}
		}
	}
	pDlg->m_prgsTime.SetPos(i);

	if(GetItemCount() > 0)
	{
		// 데이터가 추가되면 버튼 활성화
		pDlg->m_btnRename.EnableWindow(TRUE);
		pDlg->m_btnOrgName.EnableWindow(TRUE);
		pDlg->m_btnDataDelete.EnableWindow(TRUE);
		pDlg->m_btnDataReset.EnableWindow(TRUE);
	}

	SetCursor(LoadCursor(NULL, IDC_ARROW));

	CListCtrl::OnDropFiles(hDropInfo);
}
