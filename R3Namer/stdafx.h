
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <afxdisp.h>        // MFC �Զ�����

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��

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
//ע�⣺���ַ���Ϊ��ʱ��Ҳ�᷵��һ�����ַ���
void StringSplitA(const std::string& s, const std::string& delim, std::vector< std::string >& ret);
void StringSplitW(const std::wstring& s, const std::wstring& delim, std::vector< std::wstring >& ret);

void ListFilesA(const char* lpPath, std::vector<std::string>& vctDir, std::vector<std::string>& vctFiles, const char* filter = "*.*", bool bSubDir = true, bool bAppendPath = true);
void ListFielsW(const wchar_t* lpPath, std::vector<std::wstring>& vctDir, std::vector<std::wstring>& vctFiles, const wchar_t* filter = L"*.*", bool bSubDir = true, bool bAppendPath = true);

//��������MoveFolder
//������lpszFromPath Դ�ļ���·�� ��lpszToPath Ŀ���ļ���·��
//���ã��ƶ�ԭ�ļ��м������ļ���ָ����·����
/////////////////////////////////////
BOOL MoveFolder(LPCSTR lpszFromPath, LPCSTR lpszToPath);

/////////////////////////////////////
//ReNameFolder
//������lpszFromPath Դ�ļ���·�� ��lpszToPath Ŀ���ļ���·��
//���ã��޸�ԭ�ļ��е����֡�
/////////////////////////////////////
BOOL ReNameFolder(LPCSTR lpszFromPath, LPCSTR lpszToPath);

/////////////////////////////////////
//������:DeleteFolder
//���������LpszPath Ҫɾ����·��ָ��
//���ã�ɾ��ָ���ļ����Լ�������ļ�
/////////////////////////////////////
BOOL DeleteFolder(LPCSTR lpszPath);

/////////////////////////////////////
//��������CopyFolder
//������lpszFromPath Դ�ļ��е�·�� �� lpszToPath Ŀ���ļ��е�·��
//���ã������ļ��м����ļ����е���������
//////////////////////////////////////
BOOL CopyFolder(LPCSTR lpszFromPath, LPCSTR lpszToPath);

bool IsSamePathA(const char* path1, const char* path2);
bool IsSamePathW(const wchar_t* path1, const wchar_t* path2);

CString GetDirFromPath(const CString& sPath);
CString GetFileNameFromPath(const CString& sPath);
CString GetFileNameWithoutExtensionFromPath(const CString& sPath);
//�����ļ�·�������չ��
CString GetFileExtensionFromPath(const CString& sPath);

duk_ret_t js_load_json(duk_context *ctx);
duk_ret_t js_alert(duk_context *ctx);

const std::vector< std::pair<std::string, COleVariant> >& GetJson(const char* path);
const std::vector< std::pair<std::string, COleVariant> >& ParseJson(const char* jsonStr);