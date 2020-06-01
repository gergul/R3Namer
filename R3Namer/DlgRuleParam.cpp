string
// DlgRuleParam.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "R3Namer.h"
#include "DlgRuleParam.h"
#include "afxdialogex.h"
#include "json\reader.h"
#include "Convertor.h"
#include "json\writer.h"
#include "ffduktape.h"
#include <direct.h>

// CDlgRuleParam �Ի���

IMPLEMENT_DYNAMIC(CDlgRuleParam, CDialogEx)

CDlgRuleParam::CDlgRuleParam(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_RULE_PARAM, pParent)
	, m_bHasResetDefault(false)
	, m_sCaption("�����б�")
{

}

CDlgRuleParam::~CDlgRuleParam()
{
}

void CDlgRuleParam::SetJson(const char* jsonStr)
{
	m_sConfigJson = jsonStr;
}

void CDlgRuleParam::SetConfigs(const std::vector< std::pair<std::string, COleVariant> >& configs)
{
	m_configs = configs;
}

void CDlgRuleParam::SetRuleName(const CString& name)
{
	m_sRuleName = name;
}

void CDlgRuleParam::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RULE_PARAMS, m_ctrlRuleParams);
}


BOOL CDlgRuleParam::OnInitDialog()
{
	BOOL bRet = CDialogEx::OnInitDialog();

	this->SetWindowText(m_sCaption);

	HDITEM keyItem;
	keyItem.cxy = 200;
	keyItem.pszText = TEXT("����");
	keyItem.mask = HDI_WIDTH | HDI_TEXT;
	m_ctrlRuleParams.GetHeaderCtrl().SetItem(0, new HDITEM(keyItem));
	HDITEM valItem;
	valItem.pszText = TEXT("ֵ");
	valItem.mask = HDI_TEXT;
	m_ctrlRuleParams.GetHeaderCtrl().SetItem(1, new HDITEM(valItem));
		
	//��������ֶ�
	std::map<CString, CString> mpDesc;
	for (std::vector< std::pair<std::string, COleVariant> >::const_iterator it = m_configs.cbegin(); it != m_configs.cend(); ++it)
	{
		std::wstring ws;
		GL::Ansi2WideByte(ws, it->first.c_str());
		CString key = ws.c_str();

		//�������ֶ�
		if (key.GetLength() <= 5 || key.Mid(key.GetLength() - 5, 5) != TEXT("_DESC"))
			continue;

		mpDesc.insert(std::make_pair(key, V_BSTRT(&(it->second))));
	}

	//create property
	for (std::vector< std::pair<std::string, COleVariant> >::const_iterator it = m_configs.cbegin(); it != m_configs.cend(); ++it)
	{
		std::wstring ws;
		GL::Ansi2WideByte(ws, it->first.c_str());
		CString key = ws.c_str();
		key.Trim();

		//�����ֶ�
		if (key.GetLength() > 5 && key.Mid(key.GetLength() - 5, 5) == TEXT("_DESC"))
			continue;

		//�ڲ���������ʾ�����ô�����
		BOOL bInner = FALSE;
		if (key.GetLength() >= 2 && key.Mid(0, 2) == TEXT("__"))
			bInner = TRUE;

		//���Ի���ֶε�����
		CString desc = mpDesc[key+TEXT("_DESC")];

		COleVariant v = it->second;
		CMFCPropertyGridProperty* pKV = new CMFCPropertyGridProperty(key, v, desc);
		m_ctrlRuleParams.AddProperty(pKV);
		pKV->Show(!bInner);
	}

	return bRet;
}

BEGIN_MESSAGE_MAP(CDlgRuleParam, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgRuleParam::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_RESET_DEFAULT, &CDlgRuleParam::OnBnClickedBtnResetDefault)
END_MESSAGE_MAP()


// CDlgRuleParam ��Ϣ�������

void CDlgRuleParam::OnBnClickedOk()
{
	std::string sJson = "{";

	int nCount = m_ctrlRuleParams.GetPropertyCount();
	for (int i=0; i<nCount; ++i)
	{
		CMFCPropertyGridProperty* pProperty = m_ctrlRuleParams.GetProperty(i);
		std::string key;
		GL::WideByte2Ansi(key, pProperty->GetName());
		sJson += "\n\t\"" + key + "\" : ";

		CStringA sValue = OleVarToJsonValueA(pProperty->GetValue());
		sJson += sValue.GetString();
		sJson += ",";

		CString sDesc = pProperty->GetDescription();
		std::string desc;
		GL::WideByte2Ansi(desc, sDesc.GetString());
		desc = String2JsonA(desc);
		sJson += "\n\t\"" + key + "_DESC\" : ";
		sJson += "\"" + desc + "\"";

		if (i != nCount - 1)
			sJson += ",";
	}
	sJson += "\n}";
	
	GL::Ansi2Utf8(sJson, sJson.c_str());

	mkdir("Config");
	std::string sFile;
	GL::WideByte2Ansi(sFile, m_sRuleName.GetString());
	sFile = "Config/" + sFile + ".json";

	HANDLE hFile;
	hFile = CreateFileA(sFile.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("����ʧ�ܡ�"));
		return;
	}

	DWORD writed = 0;
	WriteFile(hFile, sJson.c_str(), sJson.size(), &writed, NULL);
	CloseHandle(hFile);
	
	CDialogEx::OnOK();
}


void CDlgRuleParam::OnBnClickedBtnResetDefault()
{
	::DeleteFile(TEXT("Config/") + m_sRuleName + TEXT(".json"));
	m_bHasResetDefault = true;
	CDialogEx::OnCancel();
}
