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

	/*
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(m_sConfigJson.GetString(), root))
	{
		Json::Value::Members keys = root.getMemberNames();
		for (int i=0; i<keys.size(); ++i)
		{
			std::wstring ws;
			GL::Ansi2WideByte(ws, keys[i].c_str());
			CString key = ws.c_str();

			//�����ֶ�
			CString test = key.Mid(key.GetLength() - 5, 5);
			if (test == TEXT("_DESC"))
				continue;

			//��������ʱ����
			BOOL bTemp = FALSE;
			if (key.Mid(0, 3) == TEXT("_T_"))
				bTemp = TRUE;

			const Json::Value& kv = root[keys[i]];

			//���Ի���ֶε�����
			CString desc;
			if (!root[keys[i] + "_DESC"].isNull())
			{
				std::wstring ws;
				GL::Ansi2WideByte(ws, root[keys[i] + "_DESC"].asString().c_str());
				desc = ws.c_str();
			}

			//��������ֶ�
			if (kv.isInt())
			{
				int val = kv.asInt();
				CMFCPropertyGridProperty* pKV = new CMFCPropertyGridProperty(key, (_variant_t)val, desc);
				m_ctrlRuleParams.AddProperty(pKV);
				pKV->Show(!bTemp);
			}
			else if (kv.isDouble())
			{
				float val = kv.asDouble();
				CMFCPropertyGridProperty* pKV = new CMFCPropertyGridProperty(key, (_variant_t)val, desc);
				m_ctrlRuleParams.AddProperty(pKV);
				pKV->Show(!bTemp);
			}
			else if (kv.isBool())
			{
				bool val = kv.asBool();
				CMFCPropertyGridProperty* pKV = new CMFCPropertyGridProperty(key, (_variant_t)val, desc);
				m_ctrlRuleParams.AddProperty(pKV);
				pKV->Show(!bTemp);
			}
			else
			{
				std::string val = kv.asCString();
				std::wstring ws;
				GL::Ansi2WideByte(ws, val.c_str());
				CMFCPropertyGridProperty* pKV = new CMFCPropertyGridProperty(key, ws.c_str(), desc);
				m_ctrlRuleParams.AddProperty(pKV);
				pKV->Show(!bTemp);
			}
		}
	}
	*/

	//const std::vector< std::pair<std::string, COleVariant> >& kvs = GetJson("config.js");

	//desc map
	std::map<CString, CString> mpDesc;
	for (std::vector< std::pair<std::string, COleVariant> >::const_iterator it = m_configs.cbegin(); it != m_configs.cend(); ++it)
	{
		std::wstring ws;
		GL::Ansi2WideByte(ws, it->first.c_str());
		CString key = ws.c_str();

		//�������ֶ�
		CString test = key.Mid(key.GetLength() - 5, 5);
		if (test != TEXT("_DESC"))
			continue;

		mpDesc.insert(std::make_pair(key, V_BSTRT(&(it->second))));
	}

	//create property
	for (std::vector< std::pair<std::string, COleVariant> >::const_iterator it = m_configs.cbegin(); it != m_configs.cend(); ++it)
	{
		std::wstring ws;
		GL::Ansi2WideByte(ws, it->first.c_str());
		CString key = ws.c_str();

		//�����ֶ�
		CString test = key.Mid(key.GetLength() - 5, 5);
		if (test == TEXT("_DESC"))
			continue;

		//��������ʱ����
		BOOL bTemp = FALSE;
		if (key.Mid(0, 3) == TEXT("_T_"))
			bTemp = TRUE;

		//���Ի���ֶε�����
		CString desc = mpDesc[key+TEXT("_DESC")];

		COleVariant v = it->second;
		CMFCPropertyGridProperty* pKV = new CMFCPropertyGridProperty(key, v, desc);
		m_ctrlRuleParams.AddProperty(pKV);
		pKV->Show(!bTemp);
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

	std::string sFile;
	GL::WideByte2Ansi(sFile, m_sRuleName.GetString());
	sFile = "Config/" + sFile + ".json";

	HANDLE hFile;
	hFile = CreateFileA(sFile.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD writed = 0;
		WriteFile(hFile, sJson.c_str(), sJson.size(), &writed, NULL);
		CloseHandle(hFile);
	}
	
	CDialogEx::OnOK();
}


void CDlgRuleParam::OnBnClickedBtnResetDefault()
{
	::DeleteFile(TEXT("Config/") + m_sRuleName + TEXT(".json"));
	m_bHasResetDefault = true;
	CDialogEx::OnCancel();
}
