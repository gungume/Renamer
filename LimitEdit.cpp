// LimitEdit.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Renamer.h"
#include "LimitEdit.h"


// CLimitEdit

IMPLEMENT_DYNAMIC(CLimitEdit, CEdit)

CLimitEdit::CLimitEdit()
{

}

CLimitEdit::~CLimitEdit()
{
}


BEGIN_MESSAGE_MAP(CLimitEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()

// CLimitEdit �޽��� ó�����Դϴ�.


void CLimitEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// ���ϸ� ������ �����Է� ����
	if(nChar == '\\' || nChar == '/' || nChar == ':' || nChar == '?' || nChar == '"' || nChar == '<' || nChar == '>' || nChar == '|')
	{
		return;
	}
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}