// R3NamerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "R3Namer.h"
#include "R3NamerDlg.h"
#include "afxdialogex.h"
#include "DlgRuleItem.h"
#include "Convertor.h"
#include "DlgFilePreview.h"
#include "json\json.h"
#include <fstream>
#include "DlgLog.h"
#include <algorithm>
#include <xfunctional>
#include "DlgEditRename.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static CR3NamerDlg* pTHIS = NULL;

duk_ret_t js_print(duk_context *ctx)
{
	//std::string s1, s2, s3;
	//JS::Context js(ctx);
	//js >> s1 >> s2 >> s3;

	int n = GET_ARGC(ctx);  /* #argc */
	std::string msg;
	for (int i = 0; i < n; i++)
	{
		duk_size_t out = 0;
		const char *p = duk_get_lstring(ctx, i, &out);
		if (NULL == p)
			continue;
		if (msg.empty())
			msg = p;
		else
			msg += std::string(",") + p;
	}

	GL::Convert(msg, msg.c_str(), "GBK", "UTF-8");
	StringReplaceA(msg, "\r\n", "\n");
	StringReplaceA(msg, "\n", "\r\n");
	pTHIS->ShowLog((msg).c_str());

	RETURN_VOID();
}

// CR3NamerDlg 对话框

CR3NamerDlg::CR3NamerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_R3NAMER_DIALOG, pParent)
	, m_js(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CR3NamerDlg::~CR3NamerDlg()
{
	if (m_js)
		delete m_js;
}

CStringA CR3NamerDlg::GetConfig(const char* ruleFile, bool& bReadFromDefault)
{
	JS::Context js;
	INIT_JS(js);

	//先读已保存的配置
	CStringA sRuleFileName = ruleFile;
	CStringA sConfigFileName = sRuleFileName.Mid(0, sRuleFileName.GetLength() - 3);
	if (!sConfigFileName.IsEmpty())
	{
		sConfigFileName = "Config/" + sConfigFileName + ".json";

		HANDLE hFile;
		hFile = CreateFileA(sConfigFileName.GetString(),
			GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			DWORD sizeFile = GetFileSize(hFile, NULL);
			char *pText = new char[sizeFile + 1];
			pText[sizeFile] = '\0';
			ReadFile(hFile, pText, sizeFile, &sizeFile, NULL);
			CloseHandle(hFile);

			std::string sRet;
			GL::Utf82Ansi(sRet, pText);
			delete[] pText;

			bReadFromDefault = false;
			return sRet.c_str();
		}
	}

	//如果没有保存过配置则读默认配置
	CStringA sJsFilePath = "Rules/";
	sJsFilePath += ruleFile;
	try
	{
		js.evalFile(sJsFilePath.GetString());
		std::string sRes = js.eval<std::string>("JSON.stringify(RN.PARAMS[0])");//[0]当为参数列表中有且只有一个，并为当前的那个
		GL::Utf82Ansi(sRes, sRes.c_str());
		bReadFromDefault = true;
		return sRes.c_str();
	}
	catch (std::runtime_error e)
	{
		std::string errMsg = e.what();

		CStringA ruleError;
		ruleError.Format(("\"%s\"规则错误 : %s\r\n"), sRuleFileName.GetString(), errMsg.c_str());
		pTHIS->ShowLog(ruleError);
	}

	return "";
}

CStringA CR3NamerDlg::GetConfig(const char* ruleFile)
{
	bool b = false;
	return GetConfig(ruleFile, b);
}

std::vector< std::pair<std::string, COleVariant> > CR3NamerDlg::ParseGetConfig(const char* ruleFile, bool& bReadFromDefault)
{
	//先读已保存的配置
	CStringA sRuleFileName = ruleFile;
	CStringA sConfigFileName = sRuleFileName.Mid(0, sRuleFileName.GetLength() - 3);
	if (!sConfigFileName.IsEmpty())
	{
		sConfigFileName = "Config/" + sConfigFileName + ".json";

		HANDLE hFile;
		hFile = CreateFileA(sConfigFileName.GetString(),
			GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			DWORD sizeFile = GetFileSize(hFile, NULL);
			char *pText = new char[sizeFile + 1];
			pText[sizeFile] = '\0';
			ReadFile(hFile, pText, sizeFile, &sizeFile, NULL);
			CloseHandle(hFile);

			std::string sRes = pText;
			delete[] pText;

			bReadFromDefault = false;
			return ParseJson(sRes.c_str());
		}
	}

	JS::Context js;
	INIT_JS(js);

	//如果没有保存过配置则读默认配置
	CStringA sJsFilePath = "Rules/";
	sJsFilePath += ruleFile;
	try
	{
		js.evalFile(sJsFilePath.GetString());
		std::string sRes = js.eval<std::string>("JSON.stringify(RN.PARAMS[0])");//[0]当为参数列表中有且只有一个，并为当前的那个
		//GL::Utf82Ansi(sRes, sRes.c_str());
		bReadFromDefault = true;

		return ParseJson(sRes.c_str());
	}
	catch (std::runtime_error e)
	{
		std::string errMsg = e.what();

		CStringA ruleError;
		ruleError.Format(("\"%s\"规则错误 : %s\r\n"), sRuleFileName.GetString(), errMsg.c_str());
		pTHIS->ShowLog(ruleError);
	}

	return std::vector< std::pair<std::string, COleVariant> >();
}

void CR3NamerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RULE, m_ctrlRules);
	DDX_Control(pDX, IDC_EDIT_FROM, m_ctrlSrcPath);
	DDX_Control(pDX, IDC_LIST_PREVIEW, m_ctrlPreview);
	DDX_Control(pDX, IDC_PROGRESS_RENAME, m_ctrlProgress);
	DDX_Control(pDX, IDC_STATUS_TEXT, m_ctrlStatusText);
	DDX_Control(pDX, IDC_CHECK_SUB_DIR, m_ctrlSubDir);
	DDX_Control(pDX, IDC_CHECK_SHOW_LOG, m_ctrlShowLog);
	DDX_Control(pDX, IDC_BTN_GET_NETWORK_RULE, m_ctrlGetRuleOnNetwork);
}

bool CR3NamerDlg::InitRules()
{
	bool bRet = false;

	if (m_js)
		delete m_js;
	m_js = new JS::Context;
	INIT_JS(*m_js);

	//
	m_js->addCFunction("alert", js_alert);
	m_js->addCFunction("print", js_print);
	m_js->addCFunction("loadJson", js_load_json);

	//加载命名规则
	for (int i = 0; i < m_ctrlRules.GetItemCount(); ++i)
	{
		CWnd* pWin = m_ctrlRules.GetCtrl(i, 0);
		if (!pWin->IsKindOf(RUNTIME_CLASS(CDlgRuleItem)))
			continue;

		CDlgRuleItem* pDlgRule = dynamic_cast<CDlgRuleItem*>(pWin);
		if (!pDlgRule->IsEnable())
			continue;

		CString sRuleFile = pDlgRule->GetRuleFile();
		if (sRuleFile.IsEmpty())
			continue;

		std::string szRuleFile;
		GL::WideByte2Ansi(szRuleFile, sRuleFile.GetString());

		try
		{
			m_js->evalFile<>(("Rules/" + szRuleFile).c_str());
			m_js->eval("while (RN.RULES.length > RN.PARAMS.length) RN.PARAMS.push({});");
		}
		catch (std::runtime_error e)
		{
			std::string errMsg = e.what();
			std::wstring ws;
			GL::Ansi2WideByte(ws, errMsg.c_str());

			CString ruleError;
			ruleError.Format(TEXT("\"%s\"规则错误 : %s\r\n"), sRuleFile, ws.c_str());
			ShowLog(ruleError);
			return false;
		}

		LoadConfig(szRuleFile.c_str());
		//m_js->eval("alert(JSON.stringify(RN.PARAMS));");

		bRet = true;
	}

	if (!bRet)
		ShowLog(TEXT("未选择规则！\r\n"));

	return bRet;
}

void CR3NamerDlg::LoadConfig(const char* ruleFileName)
{
	CStringA sRuleFileName = ruleFileName;
	CStringA sFileName = sRuleFileName.Mid(0, sRuleFileName.GetLength() - 3);
	if (sFileName.IsEmpty())
		return;

	sFileName = "Config/" + sFileName + ".json";

	Json::Reader reader;
	std::ifstream is;
	is.open(sFileName.GetString(), std::ios::in);
	if (!is.fail())
	{
		Json::Value root;
		if (reader.parse(is, root))
		{
			try
			{
				std::string sConfigLoad = root.toStyledString();
				m_js->eval("RN.PARAMS.pop();");
				sConfigLoad = "RN.PARAMS.push(" + sConfigLoad + ");";
				GL::Ansi2Utf8(sConfigLoad, sConfigLoad.c_str());
				m_js->eval(sConfigLoad.c_str());
			}
			catch (std::runtime_error e)
			{
				std::string errMsg = ":\"";
				errMsg += sFileName.GetString();
				errMsg += "\" : ";
				errMsg += e.what();
				std::wstring ws;
				GL::Ansi2WideByte(ws, errMsg.c_str());
				ShowLog((ws + L"\r\n").c_str());
			}
		}

		is.close();
	}
}

void CR3NamerDlg::GetRenameFiles(
	const std::vector<std::string>& vctDirs, const std::vector<std::string>& vctFiles,
	std::map<std::string, std::string>& mpResultFiles, std::map<std::string, std::string>& mpResultDirs)
{
	m_ctrlStatusText.SetWindowText(TEXT("正在分析规则..."));
	m_ctrlProgress.SetPos(0);
	size_t range = vctDirs.size() + vctFiles.size();
	m_ctrlProgress.SetRange32(0, range * 2);
	m_ctrlProgress.SetStep(1);

	//要首先命名文件，再命名文件夹
	for (std::vector<std::string>::const_iterator it = vctFiles.cbegin(); it != vctFiles.cend(); ++it)
	{
		std::string sNewDir = GetNewName(it->c_str(), true);

		if (m_js->eval<bool>("RN.TERMINATED"))
			return;

		m_ctrlProgress.StepIt();

		std::wstring ws;
		GL::Utf82WideByte(ws, sNewDir.c_str());
		GL::WideByte2Ansi(sNewDir, ws.c_str());
		if (sNewDir.empty() || IsSamePathA(sNewDir.c_str(), it->c_str()))
		{
			mpResultFiles.insert(std::make_pair(*it, ""));
			continue;
		}

		mpResultFiles.insert(std::make_pair(*it, sNewDir));
		//ReNameFolder(it->c_str(), sNewDir.c_str());
	}

	for (std::vector<std::string>::const_iterator it = vctDirs.cbegin(); it != vctDirs.cend(); ++it)
	{
		std::string sNewDir = GetNewName(it->c_str(), false);

		if (m_js->eval<bool>("RN.TERMINATED"))
			return;

		m_ctrlProgress.StepIt();

		std::wstring ws;
		GL::Utf82WideByte(ws, sNewDir.c_str());
		GL::WideByte2Ansi(sNewDir, ws.c_str());
		if (sNewDir.empty() || IsSamePathA(sNewDir.c_str(), it->c_str()))
		{
			mpResultDirs.insert(std::make_pair(*it, ""));
			continue;
		}

		mpResultDirs.insert(std::make_pair(*it, sNewDir));
		//ReNameFolder(it->c_str(), sNewDir.c_str());
	}
}

std::string CR3NamerDlg::GetNewName(const char* path, bool bIsFile)
{
	std::string sPath = path;
	StringReplaceA(sPath, "\"", "\\\"");
	StringReplaceA(sPath, "'", "\\'");
	StringReplaceA(sPath, "\\", "\\\\");

	const char* jsFmt = " \
var _p='%s'; \
for(var i=0;i<RN.RULES.length;++i){ \
  var _old=_p; \
  _p=RN.RULES[i](_p,%s,RN.PARAMS[i]); \
  if(_p == '' || _old==_p){ \
    _p=''; \
    break; \
  } \
} \
_p;";

	char js[MAX_PATH * 3] = { 0 };
	sprintf(js, jsFmt, sPath.c_str(), bIsFile ? "true" : "false");

	std::wstring ws;
	GL::Ansi2WideByte(ws, js);
	std::string sJs;
	GL::WideByte2Utf8(sJs, ws.c_str());

	try
	{
		std::string sRes = m_js->eval<std::string>(sJs.c_str());
		if (CStringA(sRes.c_str()) == CStringA("undefined"))
			return "";
		return sRes;
	}
	catch (std::runtime_error e)
	{
		std::string errMsg = "File:\"";
		errMsg += sPath + "\" : ";
		errMsg += e.what();
		std::wstring ws;
		GL::Ansi2WideByte(ws, errMsg.c_str());
		ShowLog((ws + L"\r\n").c_str());
	}

	return "";
}

void CR3NamerDlg::ShowLog(const CString& txt, bool bShow/*=true*/, bool bAppend/*=true*/)
{
	if (pLogDlg == NULL)
	{
		pLogDlg = new CDlgLog(this);
		pLogDlg->Create(CDlgLog::IDD, this);
	}

	if (bShow)
	{
		pLogDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		pLogDlg->ShowWindow(SW_HIDE);
	}

	if (!txt.IsEmpty())
	{
		if (bAppend)
			pLogDlg->AppendText(txt);
		else
			pLogDlg->SetText(txt);
	}
}

void CR3NamerDlg::ShowLog(const CStringA& txt, bool bShow /*= true*/, bool bAppend /*= true*/)
{
	std::wstring ws;
	GL::Ansi2WideByte(ws, txt.GetString());
	ShowLog(ws.c_str(), bShow, bAppend);
}

void CR3NamerDlg::AppendLog(const CString& txt)
{
	ShowLog(txt, m_ctrlShowLog.GetCheck(), true);
}

void CR3NamerDlg::AppendLog(const CStringA& txt)
{
	ShowLog(txt, m_ctrlShowLog.GetCheck(), true);
}

BEGIN_MESSAGE_MAP(CR3NamerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_RENAME, &CR3NamerDlg::OnBnClickedBtnRename)
	ON_BN_CLICKED(IDC_BTN_PREVIEW, &CR3NamerDlg::OnBnClickedBtnPreview)
	ON_BN_CLICKED(IDC_BTN_SEL_FOLDER, &CR3NamerDlg::OnBnClickedBtnSelFolder)
	ON_BN_CLICKED(IDC_BTN_OPEN_RULE_FOLDER, &CR3NamerDlg::OnBnClickedBtnOpenRuleFolder)
	ON_BN_CLICKED(IDC_BTN_SELECT_ALL, &CR3NamerDlg::OnBnClickedBtnSelectAll)
	ON_BN_CLICKED(IDC_BTN_SELECT_REVERSE, &CR3NamerDlg::OnBnClickedBtnSelectReverse)
	ON_BN_CLICKED(IDC_BTN_REFRESH_RULE, &CR3NamerDlg::OnBnClickedBtnRefreshRule)
	ON_BN_CLICKED(IDC_CHECK_SHOW_LOG, &CR3NamerDlg::OnBnClickedCheckShowLog)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PREVIEW, &CR3NamerDlg::OnNMDblclkListPreview)
	ON_BN_CLICKED(IDC_BTN_CLEAR_CONFIGS, &CR3NamerDlg::OnBnClickedBtnClearConfigs)
	ON_BN_CLICKED(IDC_BTN_OPEN_FOLDER, &CR3NamerDlg::OnBnClickedBtnOpenFolder)
	ON_BN_CLICKED(IDC_BTN_GET_NETWORK_RULE, &CR3NamerDlg::OnBnClickedBtnGetNetworkRule)
END_MESSAGE_MAP()

BOOL CALLBACK OnSettingRect(CListCtrlCustom *pListCtrl, CWnd *pCtrl, CRect &rcGrid)
{
#if 0
	//使用默认的处理方式，既填满整个格子
	return FALSE;
#else
	if (pCtrl->IsKindOf(RUNTIME_CLASS(CDlgRuleItem)) || pCtrl->IsKindOf(RUNTIME_CLASS(CDlgFilePreview)))
	{
		CRect rcList;
		pListCtrl->GetWindowRect(&rcList);

		rcGrid.right = rcGrid.left + rcList.Width() - 25;//25为滚动条的宽度
	}

	return FALSE;
#endif
}


// CR3NamerDlg 消息处理程序

BOOL CR3NamerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	pTHIS = this;

	CFont * pFont = m_ctrlGetRuleOnNetwork.GetFont();
	LOGFONT LogFont;
	pFont->GetLogFont(&LogFont);
	LogFont.lfHeight = 12;//设置字体大小
	m_getRuleOnNetworkBtnFont.CreateFontIndirect(&LogFont);
	m_ctrlGetRuleOnNetwork.SetFont(&m_getRuleOnNetworkBtnFont);

	//m_ctrlSubDir.SetCheck(TRUE);

	{
		DWORD dwExStyle = LVS_EX_FULLROWSELECT;
		m_ctrlRules.SetExtendedStyle(dwExStyle);
		m_ctrlRules.RegResizeEvent(OnSettingRect);
		m_ctrlRules.SetRowHeight(25);
		m_ctrlRules.ShowWindow(SW_SHOW);
		m_ctrlRules.GetHeaderCtrl()->EnableWindow(FALSE);
		m_ctrlRules.InsertColumn(m_ctrlRules.GetHeaderCtrl()->GetItemCount(), _T("启用"), LVCFMT_LEFT, 40);
		m_ctrlRules.InsertColumn(m_ctrlRules.GetHeaderCtrl()->GetItemCount(), _T("名称"), LVCFMT_CENTER, 370);
		m_ctrlRules.InsertColumn(m_ctrlRules.GetHeaderCtrl()->GetItemCount(), _T(""), LVCFMT_CENTER, 190);
	}
	{
		DWORD dwExStyle = LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES;
		m_ctrlPreview.SetExtendedStyle(dwExStyle);
		m_ctrlPreview.SetRowHeight(11);
		m_ctrlPreview.ShowWindow(SW_SHOW);
		m_ctrlPreview.InsertColumn(m_ctrlPreview.GetHeaderCtrl()->GetItemCount(), _T("替换"), LVCFMT_LEFT, 40);
		m_ctrlPreview.InsertColumn(m_ctrlPreview.GetHeaderCtrl()->GetItemCount(), _T("源文件（夹）"), LVCFMT_LEFT, 320);
		m_ctrlPreview.InsertColumn(m_ctrlPreview.GetHeaderCtrl()->GetItemCount(), _T("目标文件（夹）"), LVCFMT_LEFT, 340);
	}

	OnBnClickedBtnRefreshRule();

	//加载软件配置
	try
	{
		JS::Context js;
		js.addCFunction("loadJson", js_load_json);
		//js.evalFile("config.js");
		js.eval("loadJson('StartConfig', 'config.json')");

		std::string sSrcPath = js.eval<std::string>("StartConfig['SrcPath'];");
		if (sSrcPath != "undefined")
		{
			std::wstring ws;
			GL::Utf82WideByte(ws, sSrcPath.c_str());
			m_ctrlSrcPath.SetWindowText(ws.c_str());
		}

		int nSubDirSel = js.eval<int>("StartConfig['SubPath'];");
		if (nSubDirSel != 0)
			m_ctrlSubDir.SetCheck(TRUE);
		else
			m_ctrlSubDir.SetCheck(FALSE);

		std::string sPaths = js.eval<std::string>("StartConfig['SelRules'];");
		if (sPaths != "undefined")
		{
			std::wstring ws;
			GL::Utf82WideByte(ws, sPaths.c_str());
			std::vector<std::wstring> strs;
			StringSplitW(ws, L"|", strs);
			std::set<std::wstring> findStrs;
			for (auto s : strs)
			{
				s.resize(wcslen(s.c_str()));
				findStrs.insert(s);
			}
			for (int i = 0; i < m_ctrlRules.GetItemCount(); ++i)
			{
				CWnd* pWin = m_ctrlRules.GetCtrl(i, 0);
				if (!pWin->IsKindOf(RUNTIME_CLASS(CDlgRuleItem)))
					continue;

				CDlgRuleItem* pDlgRule = dynamic_cast<CDlgRuleItem*>(pWin);
				std::wstring sf = pDlgRule->GetRuleFile().GetString();
				if (findStrs.find(sf) != findStrs.end())
					pDlgRule->SetEnable(true);
			}
		}		
	}
	catch (std::runtime_error ex)
	{
	}


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CR3NamerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CR3NamerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CR3NamerDlg::OnBnClickedBtnPreview()
{
	if (pLogDlg)
		pLogDlg->SetText(TEXT(""));

	m_ctrlPreview.DeleteAllItems();

	CString sSrcPath;
	m_ctrlSrcPath.GetWindowTextW(sSrcPath);
	if (sSrcPath.IsEmpty())
	{
		ShowLog(TEXT("源文件夹未设定！\r\n"));
		return;
	}

	if (!InitRules())
	{
		return;
	}

	std::string szSrcPath;
	GL::WideByte2Ansi(szSrcPath, sSrcPath.GetString());

	std::vector<std::string> vctDirs;
	std::vector<std::string> vctFiles;
	ListFilesA(szSrcPath.c_str(), vctDirs, vctFiles, "*.*", m_ctrlSubDir.GetCheck(), true);

	std::map<std::string, std::string> mpResultFiles, mpResultDirs;
	GetRenameFiles(vctDirs, vctFiles, mpResultFiles, mpResultDirs);

	m_ctrlPreview.LockWindowUpdate();

	int nFileStep = 0;
	int nFileApply = 0;
	//文件
	for (std::map<std::string, std::string>::iterator it = mpResultFiles.begin();
		it != mpResultFiles.end(); ++it)
	{
		m_ctrlProgress.StepIt();

		if (it->first.empty() || it->second.empty())
		{
			//std::string msg = "文件:\"" + it->first + "\"";
			//msg += "不符合规则。\r\n";
			//AppendLog(msg.c_str());
			++nFileStep;
			continue;
		}

		int nCurRow = m_ctrlPreview.InsertItem(m_ctrlPreview.GetItemCount(), _T(""));

		std::wstring fromfile, tofile;
		GL::Ansi2WideByte(fromfile, it->first.c_str());
		GL::Ansi2WideByte(tofile, it->second.c_str());
		m_ctrlPreview.SetItemText(nCurRow, 1, fromfile.c_str());
		m_ctrlPreview.SetItemText(nCurRow, 2, tofile.c_str());
		m_ctrlPreview.SetItemData(nCurRow, 0);
		m_ctrlPreview.SetCheck(nCurRow, TRUE);
		++nFileApply;
	}

	int nDirStep = 0;
	int nDirApply = 0;
	//文件夹
	for (std::map<std::string, std::string>::iterator it = mpResultDirs.begin();
		it != mpResultDirs.end(); ++it)
	{
		m_ctrlProgress.StepIt();

		if (it->first.empty() || it->second.empty())
		{
			//std::string msg = "文件夹:\"" + it->first + "\"";
			//msg += "不符合规则。\r\n";
			//AppendLog(msg.c_str());
			++nDirStep;
			continue;
		}

		int nCurRow = m_ctrlPreview.InsertItem(m_ctrlPreview.GetItemCount(), _T(""));

		std::wstring fromfile, tofile;
		GL::Ansi2WideByte(fromfile, it->first.c_str());
		GL::Ansi2WideByte(tofile, it->second.c_str());
		m_ctrlPreview.SetItemText(nCurRow, 1, fromfile.c_str());
		m_ctrlPreview.SetItemText(nCurRow, 2, tofile.c_str());
		m_ctrlPreview.SetItemData(nCurRow, 1);
		m_ctrlPreview.SetCheck(nCurRow, TRUE);
		++nDirApply;
	}

	m_ctrlPreview.UnlockWindowUpdate();

	TCHAR* fmt = _T("分析完成。(匹配:%d个文件 %d个文件夹) (跳过:%d个文件 %d个文件夹)");
	TCHAR msg[512];
	_stprintf(msg, fmt, nFileApply, nDirApply, nFileStep, nDirStep);
	m_ctrlStatusText.SetWindowText(msg);
}

void CR3NamerDlg::OnBnClickedBtnRename()
{
	m_ctrlStatusText.SetWindowText(TEXT("正在重命名..."));
	m_ctrlProgress.SetPos(0);
	m_ctrlProgress.SetRange32(0, m_ctrlPreview.GetItemCount());
	m_ctrlProgress.SetStep(1);

	//跟据路径由长到短进行重命名，防止先应用了父目录，在应用子目录或此目录中的文件
	typedef std::map<int, std::vector<std::pair<CString, CString>>, std::greater<int>> GREATER_SORT;
	GREATER_SORT mpSortByStrLen;

	for (int i = 0; i < m_ctrlPreview.GetItemCount(); ++i)
	{
		if (TRUE != m_ctrlPreview.GetCheck(i))
		{
			m_ctrlProgress.StepIt();
			continue;//跳过非勾选
		}

		CString fileFrom = m_ctrlPreview.GetText(i, 1);
		CString fileTo = m_ctrlPreview.GetText(i, 2);
		if (fileFrom.IsEmpty() || fileTo.IsEmpty())
			continue;
		mpSortByStrLen[fileFrom.GetLength()].push_back(std::make_pair(fileFrom, fileTo));
	}

	int nFileStep = 0;
	int nFileApply = 0;
	int nDirStep = 0;
	int nDirApply = 0;
	for (GREATER_SORT::iterator it = mpSortByStrLen.begin(); it != mpSortByStrLen.end(); ++it)
	{
		for (auto pir : it->second)
		{
			CString fileFrom = pir.first;
			CString fileTo = pir.second;
			if (fileFrom.IsEmpty() || fileTo.IsEmpty())
				continue;
			fileFrom.Replace(TEXT("/"), TEXT("\\"));
			fileFrom.Replace(TEXT("\\\\"), TEXT("\\"));
			fileTo.Replace(TEXT("/"), TEXT("\\"));
			fileTo.Replace(TEXT("\\\\"), TEXT("\\"));

			if (::PathIsDirectory(fileFrom))
			{//文件夹
				std::string sFileFrom, sFileTo;
				GL::WideByte2Ansi(sFileFrom, fileFrom.GetString());
				GL::WideByte2Ansi(sFileTo, fileTo.GetString());
				if (ReNameFolder(sFileFrom.c_str(), sFileTo.c_str()))
					++nDirApply;
				else
					++nDirStep;
			}
			else
			{
				try
				{
					CFile::Rename(fileFrom.GetString(), fileTo.GetString());
					++nFileApply;
				}
				catch (CFileException* pEx)
				{
					pEx->Delete();
					++nFileStep;
				}
			}

			m_ctrlProgress.StepIt();
		}
	}

	TCHAR* fmt = _T("重命名完成。(成功:%d个文件 %d个文件夹) (失败:%d个文件 %d个文件夹)");
	TCHAR msg[512];
	_stprintf(msg, fmt, nFileApply, nDirApply, nFileStep, nDirStep);
	m_ctrlStatusText.SetWindowText(msg);
}

CString sDefaultDir;
int CALLBACK SHBrowseForFolderCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg)
	{

	case BFFM_INITIALIZED:    //初始化消息BFFM_INITIALIZED
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)sDefaultDir.GetString());   //传递默认打开路径 break;
	case BFFM_SELCHANGED:    //选择路径变化，BFFM_SELCHANGED
	{
		TCHAR curr[MAX_PATH];
		SHGetPathFromIDList((LPCITEMIDLIST)lParam, curr);
		::SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)&curr[0]);
	}
	break;
	default:
		break;
	}
	return 0;
}

void CR3NamerDlg::OnBnClickedBtnSelFolder()
{
	TCHAR pszPath[MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = this->GetSafeHwnd();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = TEXT("请选择需要重命名的文件夹");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	m_ctrlSrcPath.GetWindowText(sDefaultDir);
	bi.lpfn = SHBrowseForFolderCallbackProc;
	bi.lParam = 0;
	bi.iImage = 0;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl == NULL)
	{
		return;
	}

	if (SHGetPathFromIDList(pidl, pszPath))
	{
		m_ctrlSrcPath.SetWindowText(pszPath);
		//AfxMessageBox(pszPath);
	}
}


void CR3NamerDlg::OnBnClickedBtnOpenRuleFolder()
{
	system("start Rules");
}


void CR3NamerDlg::OnBnClickedBtnSelectAll()
{
	bool bHasGetFirstEnable = false;
	bool bEnable = false;

	for (int i = 0; i < m_ctrlPreview.GetItemCount(); ++i)
	{
		if (!bHasGetFirstEnable)
		{
			bHasGetFirstEnable = true;
			bEnable = m_ctrlPreview.GetCheck(i);
		}
		m_ctrlPreview.SetCheck(i, !bEnable);
	}
}

void CR3NamerDlg::OnBnClickedBtnSelectReverse()
{
	for (int i = 0; i < m_ctrlPreview.GetItemCount(); ++i)
	{
		m_ctrlPreview.SetCheck(i, !m_ctrlPreview.GetCheck(i));
	}
}


void CR3NamerDlg::OnBnClickedBtnRefreshRule()
{
	m_ctrlRules.DeleteAllItems();

	std::vector<std::wstring> vctDir;
	std::vector<std::wstring> vctFiles;
	ListFielsW(L"Rules", vctDir, vctFiles, L"*.js", false, false);
	for (std::vector<std::wstring>::iterator it = vctFiles.begin(); it != vctFiles.end(); ++it)
	{
		int nCurRow = m_ctrlRules.InsertItem(m_ctrlRules.GetItemCount(), _T(""));

		CDlgRuleItem* pRuleItem = new CDlgRuleItem;
		pRuleItem->Create(CDlgRuleItem::IDD, &m_ctrlRules);
		pRuleItem->SetRuleFile(it->c_str());

		m_ctrlRules.SetItemExCtrl(nCurRow, 0, pRuleItem);
	}
}


void CR3NamerDlg::OnBnClickedCheckShowLog()
{
	ShowLog("", m_ctrlShowLog.GetCheck(), true);
	pLogDlg->jumpLast();
}


void CR3NamerDlg::OnClose()
{
	wchar_t* fmt = L"{ \n\
	\"SrcPath\":\"%s\", \n\
	\"SubPath\":%d, \n\
	\"SelRules\":\"%s\" \n\
}";

	wchar_t wJson[2048] = { 0 };
	CString sSrcPath;
	m_ctrlSrcPath.GetWindowText(sSrcPath);
	sSrcPath.Replace(_T("\\"), _T("\\\\"));

	CString sRuleFiles;
	for (int i = 0; i < m_ctrlRules.GetItemCount(); ++i)
	{
		CWnd* pWin = m_ctrlRules.GetCtrl(i, 0);
		if (!pWin->IsKindOf(RUNTIME_CLASS(CDlgRuleItem)))
			continue;

		CDlgRuleItem* pDlgRule = dynamic_cast<CDlgRuleItem*>(pWin);
		if (!pDlgRule->IsEnable())
			continue;

		sRuleFiles += pDlgRule->GetRuleFile();
		if (i != m_ctrlRules.GetItemCount() - 1)
			sRuleFiles += '|';
	}

	wsprintf(wJson, fmt, sSrcPath.GetString(), m_ctrlSubDir.GetCheck(), sRuleFiles.GetString());
	std::string json;
	GL::WideByte2Utf8(json, wJson);

	HANDLE hFile;
	hFile = CreateFileA("config.json", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD writed = 0;
		WriteFile(hFile, json.c_str(), strlen(json.c_str()), &writed, NULL);
		CloseHandle(hFile);
	}

	CDialogEx::OnClose();
}


void CR3NamerDlg::OnNMDblclkListPreview(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMItemActivate->iItem > -1 && pNMItemActivate->iSubItem > 0)
	{
		//AfxMessageBox(m_ctrlPreview.GetItemText(pNMItemActivate->iItem, pNMItemActivate->iSubItem));
		CDlgEditRename dlgER;
		CString srcPath = m_ctrlPreview.GetItemText(pNMItemActivate->iItem, 1);
		CString targetPath = m_ctrlPreview.GetItemText(pNMItemActivate->iItem, 2);
		dlgER.SetPaths(srcPath, targetPath);
		if (dlgER.DoModal() == IDOK)
		{
			m_ctrlPreview.SetItemText(pNMItemActivate->iItem, 2, dlgER.GetTargetPath());
		}
	}
	*pResult = 0;
}


void CR3NamerDlg::OnBnClickedBtnClearConfigs()
{
	std::vector<std::string> vctDir;
	std::vector<std::string> vctFiles;
	ListFilesA("Config", vctDir, vctFiles, "*.json", false, true);
	for (auto f : vctFiles)
	{
		::DeleteFileA(f.c_str());
	}
}


void CR3NamerDlg::OnBnClickedBtnOpenFolder()
{
	CString sFolder;
	m_ctrlSrcPath.GetWindowText(sFolder);
	if (sFolder.IsEmpty())
		return;
	std::string s;
	GL::WideByte2Ansi(s, sFolder.GetString());
	s = "\"" + s + "\"";
	system(("start \"\" " + s).c_str());
}


void CR3NamerDlg::OnBnClickedBtnGetNetworkRule()
{
	std::string urlCmd = "start \"\" \"http://linux0.net/?id=4\"";
	system(urlCmd.c_str());
}
