#pragma once
#include "afxwin.h"


// CDlgLog 对话框

class CDlgLog : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLog)

public:
	CDlgLog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_LOG };
#endif
	void SetText(const CString& txt);
	void SetText(const CStringA& txt);

	void AppendText(const CString& txt);
	void AppendText(const CStringA& txt);

	void clear() { m_sLogText.Empty(); }

	void jumpLast();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CString m_sLogText;
	CEdit m_ctrlLogText;
public:
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnClose();
};
