#pragma once
#include <map>
#include <vector>

class CMyRadioButton
	: public CButton
{
	DECLARE_DYNAMIC(CMyRadioButton)

public:
	CMyRadioButton(void);
	~CMyRadioButton(void);

	virtual BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID);

private:
	static std::map<CWnd *, std::vector<UINT> > ms_arrGroupButton;

protected:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};

