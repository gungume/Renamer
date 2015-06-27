// RenamerDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"

#include "DragList.h"
#include "./BtnClass/XPStyleButtonST.h"
#include "./BtnClass/ThemeHelperST.h"
// CRenamerDlg ��ȭ ����
class CRenamerDlg : public CDialog
{
// ����
public:
	CRenamerDlg(CWnd* pParent = NULL);	// ǥ�� ������

// ��ȭ ���� ������
	enum { IDD = IDD_RENAMER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ����


// ����
protected:
	HICON m_hIcon;

	// �޽��� �� �Լ��� �����߽��ϴ�.
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// �������
	CDragList		m_listCtrl;		// Drag & Drop ���� ����Ʈ��Ʈ��
	CProgressCtrl	m_prgsTime;		// ���α׷� �����
	CString			m_strRename;	// ���ϸ� ����
	CString			m_strMessage;	// ���� �޽���
	
	// XP��Ÿ�� ��ư
	CXPStyleButtonST	m_btnRename;
	CXPStyleButtonST	m_btnOrgName;
	CXPStyleButtonST	m_btnDataDelete;
	CXPStyleButtonST	m_btnDataReset;
	CThemeHelperST		m_ThemeHelper;


	// ����Լ�
	afx_msg void OnEnChangeEditRename();		// ������ ���ϸ� ���� ó��
	afx_msg void OnBnClickedButtonRename();		// ���ϸ� ����
	afx_msg void OnBnClickedButtonOrgName();	// ������ ���ϸ����� ����
	afx_msg void OnBnClickedButtonAllReset();	// ��� ������ �ʱ�ȭ
	afx_msg void OnBnClickedButtonDelete();		// ���õ� ��� ����
	void RunRename(void);
	afx_msg void OnNMRclickListRenamer(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDelete();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};