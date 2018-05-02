#pragma once
#include "afxwin.h"


// CDlgRuleItem 对话框

class CDlgRuleItem : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRuleItem)

public:
	CDlgRuleItem(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRuleItem();

// 对话框数据
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_RULE_ITEM };
//#endif

public:
	CString GetRuleFile();
	void SetRuleFile(const CString& ruleFile);
	void SetEnable(bool b);
	bool IsEnable();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnEdit();
private:
	CButton m_ctrlEnableRule;
	CEdit m_ctrlRuleFile;
	CBrush m_brushBK;

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnRuleParam();
	afx_msg void OnBnClickedCheckEnable();
};
