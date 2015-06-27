#pragma once


// CDragList

class CDragList : public CListCtrl
{
	DECLARE_DYNAMIC(CDragList)

public:
	CDragList();
	virtual ~CDragList();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	int nDataCnt;
	CImageList *m_SmallImage;
	HIMAGELIST hSystemSmall;
};