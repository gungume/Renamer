// RenamerDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"

#include "DragSortList.h"
#include "LimitEdit.h"
#include "afxwin.h"

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
	CDragSortList	m_listCtrl;		// Drag & Drop ���� ListControl
	CLimitEdit		m_ctrlEdit;		// �Է����� EditControl
	CProgressCtrl	m_prgsTime;		// ���α׷� �����
	CString			m_strRename;	// ���ϸ� ����
	CString			m_strMessage;	// ���� �޽���
	
	// ��ư
	CButton	m_btnRename;
	CButton	m_btnOrgName;
	CButton	m_btnDataDelete;
	CButton	m_btnDataReset;
	
	// ���� �Ϸù�ȣ
	int m_nStartNumber;
	BOOL m_bStartNumber;
	

	// ����Լ�
	afx_msg void OnEnChangeEditRename();	// ������ ���ϸ� ���� ó��
	afx_msg void OnBtnRename();				// ���ϸ� ����
	afx_msg void OnBtnOrgName();			// ������ ���ϸ����� ����
	afx_msg void OnBtnAllReset();			// ��� ������ �ʱ�ȭ
	afx_msg void OnBtnDelete();				// ���õ� ��� ����
	afx_msg void OnDelete();				// ���õ� ��� ����
	afx_msg void OnBnCheckStartNumber();	// �Ϸù�ȣ ���۹�ȣ ����
	afx_msg void OnEnChangeEditStartNumber();
	afx_msg void OnNMRclickListRenamer(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void RunRename(void);					// ���ϸ� �������
};