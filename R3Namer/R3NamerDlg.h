
// R3NamerDlg.h : ͷ�ļ�
//

#pragma once
#include "ListCtrlCustom.h"
#include "afxwin.h"
#include "ffduktape.h"
#include <vector>
#include <string>
#include <map>
#include "afxcmn.h"

class CDlgLog;

// CR3NamerDlg �Ի���
class CR3NamerDlg : public CDialogEx
{
// ����
public:
	CR3NamerDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CR3NamerDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_R3NAMER_DIALOG };
#endif
public:
	static CStringA GetConfig(const char* ruleFile, bool& bReadFromDefault);
	static CStringA GetConfig(const char* ruleFile);
	static std::vector< std::pair<std::string, COleVariant> > ParseGetConfig(const char* ruleFile, bool& bReadFromDefault);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	bool InitRules();
	void LoadConfig(const char* ruleFileName);
	void GetRenameFiles(
		const std::vector<std::string>& vctDirs, const std::vector<std::string>& vctFiles,
		std::map<std::string, std::string>& mpResultFiles, std::map<std::string, std::string>& mpResultDirs);

	std::string GetNewName(const char* path, bool bIsFile);	

	void ShowLog(const CString& txt, bool bShow=true, bool bAppend=true);
	void ShowLog(const CStringA& txt, bool bShow = true, bool bAppend = true);
	void AppendLog(const CString& txt);
	void AppendLog(const CStringA& txt);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnRename();
	afx_msg void OnBnClickedBtnPreview();
	afx_msg void OnBnClickedBtnSelFolder();

public:
	afx_msg void OnBnClickedBtnOpenRuleFolder();
	afx_msg void OnBnClickedBtnSelectAll();
	afx_msg void OnBnClickedBtnSelectReverse();
	afx_msg void OnBnClickedBtnRefreshRule();
	afx_msg void OnClose();
	afx_msg void OnNMDblclkListPreview(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckShowLog();

private:
	CButton m_ctrlSubDir;
	CButton m_ctrlShowLog;
	CDlgLog* pLogDlg = NULL;
	CListCtrlCustom m_ctrlRules;
	CListCtrlCustom m_ctrlPreview;
	CEdit m_ctrlSrcPath;
	JS::Context* m_js;
	CProgressCtrl m_ctrlProgress;
	CStatic m_ctrlStatusText;
	CFont m_getRuleOnNetworkBtnFont;

public:
	afx_msg void OnBnClickedBtnClearConfigs();
	afx_msg void OnBnClickedBtnOpenFolder();
private:
	CButton m_ctrlGetRuleOnNetwork;
public:
	afx_msg void OnBnClickedBtnGetNetworkRule();
};
