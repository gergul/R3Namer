#pragma once
#include "afxwin.h"


// CDlgEditRename 对话框

class CDlgEditRename : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgEditRename)

public:
	CDlgEditRename(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgEditRename();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_EDIT_RENAME };
#endif

	void SetPaths(const CString& srcPath, const CString& targetPath);
	CString GetTargetPath();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();

private:
	CString m_srcPath;
	CString m_targetPath;
	CEdit m_ctrlSrcPath;
	CEdit m_ctrlTargetDir;
	CEdit m_ctrlTargetName;
};
