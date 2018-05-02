#pragma once
#include "afxwin.h"


// CDlgFilePreview �Ի���

class CDlgFilePreview : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFilePreview)

public:
	CDlgFilePreview(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFilePreview();

// �Ի�������
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
