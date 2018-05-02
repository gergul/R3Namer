#pragma once
#include "afxwin.h"


// CDlgFilePreview 对话框

class CDlgFilePreview : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFilePreview)

public:
	CDlgFilePreview(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFilePreview();

// 对话框数据
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_FILE_PREVIEW };
//#endif

	void SetFile(const CString& file1, const CString& file2);
	void GetFile(CString& file1, CString& file2);
	void SetFolder(bool bIsFolder);
	bool IsFolder();

	bool IsEnable();
	void SetEnable(bool b);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
private:
	CButton m_ctrlEnable;
	CEdit m_ctrlFromFile;
	CEdit m_ctrlToFile;
	bool m_bIsFolder;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
