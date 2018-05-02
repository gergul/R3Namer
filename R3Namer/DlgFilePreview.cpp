// DlgFilePreview.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "R3Namer.h"
#include "DlgFilePreview.h"
#include "afxdialogex.h"


// CDlgFilePreview �Ի���

IMPLEMENT_DYNAMIC(CDlgFilePreview, CDialogEx)

CDlgFilePreview::CDlgFilePreview(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_FILE_PREVIEW, pParent)
	, m_bIsFolder(false)
{

}

CDlgFilePreview::~CDlgFilePreview()
{
}

void CDlgFilePreview::SetFile(const CString& file1, const CString& file2)
{
	m_ctrlFromFile.SetWindowText(file1);
	m_ctrlToFile.SetWindowText(file2);
}

void CDlgFilePreview::GetFile(CString& file1, CString& file2)
{
	if (m_ctrlEnable.GetCheck())
	{
		m_ctrlFromFile.GetWindowText(file1);
		m_ctrlToFile.GetWindowText(file2);
	}
}

void CDlgFilePreview::SetFolder(bool bIsFolder)
{
	m_bIsFolder = bIsFolder;
}

bool CDlgFilePreview::IsFolder()
{
	return m_bIsFolder;
}

bool CDlgFilePreview::IsEnable()
{
	return (m_ctrlEnable.GetCheck() == TRUE);
}

void CDlgFilePreview::SetEnable(bool b)
{
	m_ctrlEnable.SetCheck(b);
}

void CDlgFilePreview::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_ENABLE, m_ctrlEnable);
	DDX_Control(pDX, IDC_EDIT_FROM, m_ctrlFromFile);
	DDX_Control(pDX, IDC_EDIT_TO, m_ctrlToFile);
}


BOOL CDlgFilePreview::OnInitDialog()
{
	BOOL bRet = CDialogEx::OnInitDialog();

	m_ctrlEnable.SetCheck(TRUE);

	return bRet;
}

BEGIN_MESSAGE_MAP(CDlgFilePreview, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgFilePreview ��Ϣ�������


HBRUSH CDlgFilePreview::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	pDC->SetBkColor(RGB(255,255,255));


	return hbr;
}
