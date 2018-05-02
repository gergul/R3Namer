// DlgEditRename.cpp : 实现文件
//

#include "stdafx.h"
#include "R3Namer.h"
#include "DlgEditRename.h"
#include "afxdialogex.h"


// CDlgEditRename 对话框

IMPLEMENT_DYNAMIC(CDlgEditRename, CDialogEx)

CDlgEditRename::CDlgEditRename(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_EDIT_RENAME, pParent)
{

}

CDlgEditRename::~CDlgEditRename()
{
}

void CDlgEditRename::SetPaths(const CString& srcPath, const CString& targetPath)
{
	m_srcPath = srcPath;
	m_targetPath = targetPath;
}

CString CDlgEditRename::GetTargetPath()
{
	return m_targetPath;
}

void CDlgEditRename::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SRC_PATH, m_ctrlSrcPath);
	DDX_Control(pDX, IDC_EDIT_TARGET_DIR, m_ctrlTargetDir);
	DDX_Control(pDX, IDC_EDIT_TARGET_NAME, m_ctrlTargetName);
}


BOOL CDlgEditRename::OnInitDialog()
{
	BOOL bRet = CDialogEx::OnInitDialog();

	m_ctrlSrcPath.SetWindowText(m_srcPath);

	CString dir = GetDirFromPath(m_targetPath);
	CString fileName = GetFileNameFromPath(m_targetPath);
	m_ctrlTargetDir.SetWindowText(dir);
	m_ctrlTargetName.SetWindowText(fileName);

	return bRet;
}

BEGIN_MESSAGE_MAP(CDlgEditRename, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgEditRename::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgEditRename 消息处理程序


void CDlgEditRename::OnBnClickedOk()
{
	CString fileName;
	m_ctrlTargetName.GetWindowText(fileName);
	if (fileName.IsEmpty())
	{
		AfxMessageBox(_T("文件名不能为空！"));
		return;
	}

	m_ctrlTargetDir.GetWindowText(m_targetPath);
	m_targetPath += _T("\\") + fileName;

	CDialogEx::OnOK();
}
