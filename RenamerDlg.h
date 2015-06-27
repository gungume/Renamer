// RenamerDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"

#include "DragSortList.h"
#include "LimitEdit.h"
#include "afxwin.h"

// CRenamerDlg 대화 상자
class CRenamerDlg : public CDialog
{
// 생성
public:
	CRenamerDlg(CWnd* pParent = NULL);	// 표준 생성자

// 대화 상자 데이터
	enum { IDD = IDD_RENAMER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원


// 구현
protected:
	HICON m_hIcon;

	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 멤버변수
	CDragSortList	m_listCtrl;		// Drag & Drop 가능 ListControl
	CLimitEdit		m_ctrlEdit;		// 입력제한 EditControl
	CProgressCtrl	m_prgsTime;		// 프로그램 진행률
	CString			m_strRename;	// 파일명 변경
	CString			m_strMessage;	// 각종 메시지
	
	// 버튼
	CButton	m_btnRename;
	CButton	m_btnOrgName;
	CButton	m_btnDataDelete;
	CButton	m_btnDataReset;
	
	// 시작 일련번호
	int m_nStartNumber;
	BOOL m_bStartNumber;
	

	// 멤버함수
	afx_msg void OnEnChangeEditRename();	// 변경할 파일명 형식 처리
	afx_msg void OnBtnRename();				// 파일명 변경
	afx_msg void OnBtnOrgName();			// 최초의 파일명으로 복구
	afx_msg void OnBtnAllReset();			// 모든 데이터 초기화
	afx_msg void OnBtnDelete();				// 선택된 목록 제거
	afx_msg void OnDelete();				// 선택된 목록 제거
	afx_msg void OnBnCheckStartNumber();	// 일련번호 시작번호 지정
	afx_msg void OnEnChangeEditStartNumber();
	afx_msg void OnNMRclickListRenamer(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void RunRename(void);					// 파일명 변경실행
};