// DlgLog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "R3Namer.h"
#include "DlgLog.h"
#include "afxdialogex.h"
#include "Convertor.h"


// CDlgLog �Ի���

IMPLEMENT_DYNAMIC(CDlgLog, CDialogEx)

CDlgLog::CDlgLog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_LOG, pParent)
	, m_sLogText(_T(""))
{

}

CDlgLog::~CDlgLog()
{
}

void CDlgLog::SetText(const CStringA& txt)
{
	std::wstring ws;
	GL::Ansi2WideByte(ws, txt.GetString());
	m_sLogText = ws.c_str();

	m_ctrlLogText.SetWindowText(m_sLogText);
	jumpLast();
}

void CDlgLog::SetText(const CString& txt)
{
	m_sLogText = txt; 

	m_ctrlLogText.SetWindowText(m_sLogText);
	jumpLast();
}

void CDlgLog::AppendText(const CStringA& txt)
{
	std::wstring ws;
	GL::Ansi2WideByte(ws, txt.GetString());
	m_sLogText += ws.c_str();

	m_ctrlLogText.SetWindowText(m_sLogText);
	jumpLast();
}

void CDlgLog::AppendText(const CString& txt)
{
	m_sLogText += txt; 

	m_ctrlLogText.SetWindowText(m_sLogText);
	jumpLast();
}



void CDlgLog::jumpLast()
{
	int nLength = m_sLogText.GetLength();
	m_ctrlLogText.SetSel(nLength, nLength, FALSE);
	m_ctrlLogText.SetFocus();
}

void CDlgLog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG, m_ctrlLogText);
}


BEGIN_MESSAGE_MAP(CDlgLog, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CDlgLog::OnBnClickedBtnClear)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CDlgLog::OnBnClickedBtnClose)
END_MESSAGE_MAP()


// CDlgLog ��Ϣ�������


void CDlgLog::OnBnClickedBtnClear()
{
	SetText(_T(""));
}


void CDlgLog::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}


void CDlgLog::OnBnClickedBtnClose()
{
	this->OnOK();
}
