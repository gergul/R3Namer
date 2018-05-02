#pragma once
#include "afxwin.h"


// CDlgLog �Ի���

class CDlgLog : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLog)

public:
	CDlgLog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLog();

// �Ի�������
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CString m_sLogText;
	CEdit m_ctrlLogText;
public:
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnClose();
};
