// DlgRuleItem.cpp : 实现文件
//

#include "stdafx.h"
#include "R3Namer.h"
#include "DlgRuleItem.h"
#include "afxdialogex.h"
#include <string>
#include "R3namerDlg.h"
#include "Convertor.h"
#include "DlgRuleParam.h"


// CDlgRuleItem 对话框

IMPLEMENT_DYNAMIC(CDlgRuleItem, CDialogEx)

CDlgRuleItem::CDlgRuleItem(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_RULE_ITEM, pParent)
{

}

CDlgRuleItem::~CDlgRuleItem()
{
}

CString CDlgRuleItem::GetRuleFile()
{
	CString sRuleFile;
	m_ctrlRuleFile.GetWindowText(sRuleFile);
	return sRuleFile;
}

void CDlgRuleItem::SetRuleFile(const CString& ruleFile)
{
	m_ctrlRuleFile.SetWindowText(ruleFile);
}

void CDlgRuleItem::SetEnable(bool b)
{
	m_ctrlEnableRule.SetCheck(b);
}

bool CDlgRuleItem::IsEnable()
{
	return m_ctrlEnableRule.GetCheck();
}

void CDlgRuleItem::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_ENABLE, m_ctrlEnableRule);
	DDX_Control(pDX, IDC_EDIT1, m_ctrlRuleFile);
}


BOOL CDlgRuleItem::OnInitDialog()
{
	BOOL bRet = CDialogEx::OnInitDialog();

	m_brushBK.CreateSolidBrush(RGB(255, 255, 255));

	return bRet;
}

BEGIN_MESSAGE_MAP(CDlgRuleItem, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_EDIT, &CDlgRuleItem::OnBnClickedBtnEdit)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_RULE_PARAM, &CDlgRuleItem::OnBnClickedBtnRuleParam)
	ON_BN_CLICKED(IDC_CHECK_ENABLE, &CDlgRuleItem::OnBnClickedCheckEnable)
END_MESSAGE_MAP()


// CDlgRuleItem 消息处理程序

CString ReturnPath()
{
	CString sPath;
	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	sPath.ReleaseBuffer();
	int nPos;
	nPos = sPath.ReverseFind('\\');
	sPath = sPath.Left(nPos);
	return sPath;
}

void CDlgRuleItem::OnBnClickedBtnEdit()
{
	CString sRuleFile;
	m_ctrlRuleFile.GetWindowText(sRuleFile);
	TCHAR curPath[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, curPath);
	CString sCurPath = curPath;
	sRuleFile = TEXT("start \"\" \"") + sCurPath + TEXT("\\Rules\\") + sRuleFile + TEXT("\"");

	std::string s;
	GL::WideByte2Ansi(s, sRuleFile.GetString());
	system(s.c_str());
}


HBRUSH CDlgRuleItem::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	if (pWnd == this)
	{
		return m_brushBK;
	}

	return hbr;
}


void CDlgRuleItem::OnBnClickedBtnRuleParam()
{
	CString sRuleFile;
	m_ctrlRuleFile.GetWindowText(sRuleFile);
	std::string sJsFile;
	GL::WideByte2Ansi(sJsFile, sRuleFile.GetString());
	bool bReadFromDefault = false;
	//CStringA sConfigJson = CR3NamerDlg::GetConfig(sJsFile.c_str(), bReadFromDefault);
	//if (sConfigJson.IsEmpty())
	//	return;
	std::vector< std::pair<std::string, COleVariant> > configs = CR3NamerDlg::ParseGetConfig(sJsFile.c_str(), bReadFromDefault);

	CDlgRuleParam rp;
	//rp.SetJson(sConfigJson.GetString());
	rp.SetConfigs(configs);
	CString sRuleName = sRuleFile.Mid(0, sRuleFile.GetLength() - 3);
	rp.SetRuleName(sRuleName);
	rp.SetCaption(bReadFromDefault ? sRuleName + TEXT(" - (默认)") : sRuleName + TEXT(" - (自定义)"));
	rp.DoModal();
	if (rp.IsCloseForResetDefault())
		OnBnClickedBtnRuleParam();
}



void CDlgRuleItem::OnBnClickedCheckEnable()
{
	// TODO: 在此添加控件通知处理程序代码

}
