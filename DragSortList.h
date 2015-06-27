#pragma once

typedef struct SORT_PARAMS
{
	HWND	hWnd;					//리스트컨트롤 핸들
	int		nCol;					//정렬기준인 컬럼
	BOOL	bAscend;				//오름차순인가 내림차순인가...
}SORT_PARAMS;

// CDragSortList
class CDragSortList : public CListCtrl
{
	DECLARE_DYNAMIC(CDragSortList)

public:
	CDragSortList();
	virtual ~CDragSortList();

protected:
	DECLARE_MESSAGE_MAP()
public:
	int nDataCnt;
	CImageList *m_SmallImage;
	HIMAGELIST hSystemSmall;
	BOOL m_bAscend;

	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);
	static int CALLBACK SortFunc(LPARAM lParam1 , LPARAM lParam2 , LPARAM lParamSort);
};