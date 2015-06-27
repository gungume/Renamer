// RenamerDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"

#include "DragList.h"
#include "./BtnClass/XPStyleButtonST.h"
#include "./BtnClass/ThemeHelperST.h"
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
	CDragList		m_listCtrl;		// Drag & Drop 가능 리스트컨트롤
	CProgressCtrl	m_prgsTime;		// 프로그램 진행률
	CString			m_strRename;	// 파일명 변경
	CString			m_strMessage;	// 각종 메시지
	
	// XP스타일 버튼
	CXPStyleButtonST	m_btnRename;
	CXPStyleButtonST	m_btnOrgName;
	CXPStyleButtonST	m_btnDataDelete;
	CXPStyleButtonST	m_btnDataReset;
	CThemeHelperST		m_ThemeHelper;


	// 멤버함수
	afx_msg void OnEnChangeEditRename();		// 변경할 파일명 형식 처리
	afx_msg void OnBnClickedButtonRename();		// 파일명 변경
	afx_msg void OnBnClickedButtonOrgName();	// 최초의 파일명으로 복구
	afx_msg void OnBnClickedButtonAllReset();	// 모든 데이터 초기화
	afx_msg void OnBnClickedButtonDelete();		// 선택된 목록 제거
	void RunRename(void);
	afx_msg void OnNMRclickListRenamer(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDelete();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};