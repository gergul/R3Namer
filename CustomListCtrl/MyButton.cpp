#include "StdAfx.h"
#include "MyButton.h"
#include "ListCtrlCustom.h"

IMPLEMENT_DYNAMIC(CMyButton, CButton)

CMyButton::CMyButton(void)
{
}


CMyButton::~CMyButton(void)
{
}

BEGIN_MESSAGE_MAP(CMyButton, CButton)
	//{{AFX_MSG_MAP(SDCtrlComboboxListctrl)
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CMyButton::OnLButtonUp( UINT nFlags, CPoint point )
{
	CButton::OnLButtonUp(nFlags, point);

	CString sText;
	CListCtrlCustom *pListCtrl = (CListCtrlCustom *)GetParent();
	LVHITTESTINFO ht;
	pListCtrl->CurPosHitTest(ht);
	sText.Format(_T("U clicked %d"), ht.iItem);

	AfxMessageBox(sText);
}