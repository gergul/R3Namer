
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展
#include <afxdisp.h>        // MFC 自动化类

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#include <string>
#include <vector>
#include "ffduktape.h"

#define AFX_DESIGN_TIME

#define INIT_JS(context) \
	(context).eval("var RN=[]; RN.RULES=[]; RN.PARAMS=[]; RN.TERMINATED=false;"); \
	(context).evalFile("init.js")

std::string String2JsonA(const std::string& s);
std::wstring String2JsonW(const std::wstring& s);
CStringA OleVarToJsonValueA(const COleVariant& B);
CStringW OleVarToJsonValueW(const COleVariant& B);
bool StringReplaceA(std::string& strBase, const std::string& strSrc, const std::string& strDes);
//注意：当字符串为空时，也会返回一个空字符串
void StringSplitA(const std::string& s, const std::string& delim, std::vector< std::string >& ret);
void StringSplitW(const std::wstring& s, const std::wstring& delim, std::vector< std::wstring >& ret);

void ListFilesA(const char* lpPath, std::vector<std::string>& vctDir, std::vector<std::string>& vctFiles, const char* filter = "*.*", bool bSubDir = true, bool bAppendPath = true);
void ListFielsW(const wchar_t* lpPath, std::vector<std::wstring>& vctDir, std::vector<std::wstring>& vctFiles, const wchar_t* filter = L"*.*", bool bSubDir = true, bool bAppendPath = true);

//函数名：MoveFolder
//参数：lpszFromPath 源文件夹路径 。lpszToPath 目的文件夹路径
//作用：移动原文件夹及其中文件都指定的路径下
/////////////////////////////////////
BOOL MoveFolder(LPCSTR lpszFromPath, LPCSTR lpszToPath);

/////////////////////////////////////
//ReNameFolder
//参数：lpszFromPath 源文件夹路径 。lpszToPath 目的文件夹路径
//作用：修改原文件夹的名字。
/////////////////////////////////////
BOOL ReNameFolder(LPCSTR lpszFromPath, LPCSTR lpszToPath);

/////////////////////////////////////
//函数名:DeleteFolder
//输入参数：LpszPath 要删除的路径指针
//作用：删除指定文件夹以及里面的文件
/////////////////////////////////////
BOOL DeleteFolder(LPCSTR lpszPath);

/////////////////////////////////////
//函数名：CopyFolder
//参数：lpszFromPath 源文件夹的路径 。 lpszToPath 目的文件夹的路径
//作用：拷贝文件夹及其文件夹中的所有内容
//////////////////////////////////////
BOOL CopyFolder(LPCSTR lpszFromPath, LPCSTR lpszToPath);

bool IsSamePathA(const char* path1, const char* path2);
bool IsSamePathW(const wchar_t* path1, const wchar_t* path2);

CString GetDirFromPath(const CString& sPath);
CString GetFileNameFromPath(const CString& sPath);
CString GetFileNameWithoutExtensionFromPath(const CString& sPath);
//跟据文件路径获得扩展名
CString GetFileExtensionFromPath(const CString& sPath);

duk_ret_t js_load_json(duk_context *ctx);
duk_ret_t js_alert(duk_context *ctx);

const std::vector< std::pair<std::string, COleVariant> >& GetJson(const char* path);
const std::vector< std::pair<std::string, COleVariant> >& ParseJson(const char* jsonStr);