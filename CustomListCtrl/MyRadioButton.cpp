#include "StdAfx.h"
#include "MyRadioButton.h"

IMPLEMENT_DYNAMIC(CMyRadioButton, CButton)

CMyRadioButton::CMyRadioButton(void)
{
}


CMyRadioButton::~CMyRadioButton(void)
{
}

BEGIN_MESSAGE_MAP(CMyRadioButton, CButton)
	//{{AFX_MSG_MAP(SDCtrlComboboxListctrl)
	ON_WM_LBUTTONUP()	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CMyRadioButton::OnLButtonUp( UINT nFlags, CPoint point )
{
	CButton::OnLButtonUp(nFlags, point);//必需调用，否则会假死

	SetCheck(TRUE);//this button always checked

	CWnd *pParent = GetParent();
	if (NULL == pParent)
		return ;

	std::map<CWnd *, std::vector<UINT> >::iterator itFinder = ms_arrGroupButton.find(pParent);
	if (itFinder != ms_arrGroupButton.end())
	{
		std::vector<UINT> &ids = itFinder->second;
		UINT uThisId = GetDlgCtrlID();
		CWnd *pCtrl = NULL;
		int nSizeIds = (int)ids.size();
		for (int i=0; i<nSizeIds; ++i)
		{
			UINT uId = ids[i];
			if (uId == uThisId)
				continue;

			pCtrl = pParent->GetDlgItem(uId);
			if (NULL == pCtrl || !pCtrl->IsKindOf(RUNTIME_CLASS(CMyRadioButton)))
				continue;
			((CButton *)pCtrl)->SetCheck(FALSE);//other buttons always unchecked
		}
	}
}

BOOL CMyRadioButton::Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	if (NULL == pParentWnd)
		return FALSE;

	BOOL bRet = CButton::Create(lpszCaption, dwStyle, rect, pParentWnd, nID);

	ms_arrGroupButton[pParentWnd].push_back(nID);

	return bRet;
}

std::map<CWnd *, std::vector<UINT> > CMyRadioButton::ms_arrGroupButton;

