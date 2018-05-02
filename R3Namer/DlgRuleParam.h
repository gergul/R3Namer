#pragma once
#include "afxpropertygridctrl.h"


// CDlgRuleParam 对话框

class CDlgRuleParam : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRuleParam)

public:
	CDlgRuleParam(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRuleParam();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_RULE_PARAM };
#endif

	void SetJson(const char* jsonStr);
	void SetConfigs(const std::vector< std::pair<std::string, COleVariant> >& configs);
	void SetRuleName(const CString& name);
	void SetCaption(const CString& val) { m_sCaption = val; }

	bool IsCloseForResetDefault() { return m_bHasResetDefault; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	CMFCPropertyGridCtrl m_ctrlRuleParams;
	CStringA m_sConfigJson;
	std::vector< std::pair<std::string, COleVariant> > m_configs;
	CString m_sRuleName;
	CString m_sCaption;
	bool m_bHasResetDefault;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnResetDefault();
};
