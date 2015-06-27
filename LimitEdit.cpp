// LimitEdit.cpp : 구현 파일입니다.
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

// CLimitEdit 메시지 처리기입니다.


void CLimitEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// 파일명 부적합 문자입력 차단
	if(nChar == '\\' || nChar == '/' || nChar == ':' || nChar == '?' || nChar == '"' || nChar == '<' || nChar == '>' || nChar == '|')
	{
		return;
	}
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}