// Renamer.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ


// CRenamerApp:
// �� Ŭ������ ������ ���ؼ��� Renamer.cpp�� �����Ͻʽÿ�.
//

class CRenamerApp : public CWinApp
{
public:
	CRenamerApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CRenamerApp theApp;
