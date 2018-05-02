
// stdafx.cpp : 只包括标准包含文件的源文件
// R3Namer.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#include "Convertor.h"

std::string String2JsonA(const std::string& s)
{
	std::string sb;
	for (int i = 0; i < s.length(); i++)
	{
		char c = s[i];
		switch (c)
		{
		case '\"':
			sb.append("\\\""); break;
		case '\\':
			sb.append("\\\\"); break;
		case '/':
			sb.append("\\/"); break;
		case '\b':
			sb.append("\\b"); break;
		case '\f':
			sb.append("\\f"); break;
		case '\n':
			sb.append("\\n"); break;
		case '\r':
			sb.append("\\r"); break;
		case '\t':
			sb.append("\\t"); break;
		default:
			//在ASCⅡ码中，第0～31号及第127号(共33个)是控制字符或通讯专用字符
			if ((c >= 0 && c <= 31) || c == 127)
			{

			}
			else
			{
				sb += c;
			}
			break;
		}
	}
	return sb;
}


std::wstring String2JsonW(const std::wstring& s)
{
	std::wstring sb;
	for (int i = 0; i < s.length(); i++)
	{
		wchar_t c = s[i];
		switch (c)
		{
		case '\"':
			sb.append(L"\\\""); break;
		case '\\':
			sb.append(L"\\\\"); break;
		case '/':
			sb.append(L"\\/"); break;
		case '\b':
			sb.append(L"\\b"); break;
		case '\f':
			sb.append(L"\\f"); break;
		case '\n':
			sb.append(L"\\n"); break;
		case '\r':
			sb.append(L"\\r"); break;
		case '\t':
			sb.append(L"\\t"); break;
		default:
			//在ASCⅡ码中，第0～31号及第127号(共33个)是控制字符或通讯专用字符
			if ((c >= 0 && c <= 31) || c == 127)
			{

			}
			else
			{
				sb += c;
			}
			break;
		}
	}
	return sb;
}

CStringA OleVarToJsonValueA(const COleVariant& B)
{
	CStringA A;
	switch (B.vt)
	{
	case VT_BSTR: 
	{
		A = "\"";

		CString val = V_BSTRT(&B);
		std::string s;
		GL::WideByte2Ansi(s, val.GetString());
		s = String2JsonA(s);
		A += s.c_str();

		A += "\"";
		break;//COleVariant是一个字串
	}
	case VT_INT:  A.Format("%d", V_INT(&B)); break;//是整型
	case VT_I2:   A.Format("%hd", V_I2(&B)); break;//是短整型
	case VT_I4:   A.Format("%d", V_I4(&B)); break;//是长整型
	case VT_R4:   A.Format("%e", (double)V_R4(&B)); break;//是浮点数
	case VT_R8:   A.Format("%e", V_R8(&B)); break;//是浮点数
	case VT_BOOL: A = V_BOOL(&B) ? "true" : "false"; break;//是布尔值
	}

	return A;
}
CStringW OleVarToJsonValueW(const COleVariant& B)
{
	CStringW A;
	switch (B.vt)
	{
	case VT_BSTR:
	{
		A = L"\"";

		CStringW val = V_BSTRT(&B);
		std::wstring ws = String2JsonW(val.GetString());
		A += ws.c_str();

		A += L"\"";
		break;//COleVariant是一个字串
	}
	case VT_INT:  A.Format(L"%d", V_INT(&B)); break;//是整型
	case VT_I2:   A.Format(L"%hd", V_I2(&B)); break;//是短整型
	case VT_I4:   A.Format(L"%d", V_I4(&B)); break;//是长整型
	case VT_R4:   A.Format(L"%e", (double)V_R4(&B)); break;//是浮点数
	case VT_R8:   A.Format(L"%e", V_R8(&B)); break;//是浮点数
	case VT_BOOL: A = V_BOOL(&B) ? L"true" : L"false"; break;//是布尔值
	}

	return A;
}

bool StringReplaceA(std::string& strBase, const std::string& strSrc, const std::string& strDes)
{
	bool b = false;

	std::string::size_type pos = 0;
	std::string::size_type srcLen = strSrc.size();
	std::string::size_type desLen = strDes.size();
	pos = strBase.find(strSrc, pos);
	while ((pos != std::string::npos))
	{
		strBase.replace(pos, srcLen, strDes);
		pos = strBase.find(strSrc, (pos + desLen));
		b = true;
	}

	return b;
}

//注意：当字符串为空时，也会返回一个空字符串
void StringSplitA(const std::string& s, const std::string& delim, std::vector< std::string >& ret)
{
	size_t last = 0;
	size_t index = s.find_first_of(delim, last);
	while (index != std::string::npos)
	{
		ret.push_back(s.substr(last, index - last));
		last = index + 1;
		index = s.find_first_of(delim, last);
	}
	if (index - last > 0)
	{
		ret.push_back(s.substr(last, index - last));
	}
}

void StringSplitW(const std::wstring& s, const std::wstring& delim, std::vector< std::wstring >& ret)
{
	size_t last = 0;
	size_t index = s.find_first_of(delim, last);
	while (index != std::wstring::npos)
	{
		ret.push_back(s.substr(last, index - last));
		last = index + 1;
		index = s.find_first_of(delim, last);
	}
	if (index - last > 0)
	{
		ret.push_back(s.substr(last, index - last));
	}
}

// void FindFilesInFolder()
// {
// 	std::vector<std::string> fileLists;
// 	std::tr2::sys::directory_iterator it("C:\\Users\\Administrator\\Desktop\\原来的桌面");
// 
// 	while (it != std::tr2::sys::directory_iterator())
// 	{
// 		std::string tmp = it->path().string();
// 		fileLists.push_back(tmp);
// 		++it;
// 
// 	}
// }

void ListFilesA(const char* lpPath, std::vector<std::string>& vctDir, std::vector<std::string>& vctFiles, const char* filter/* = "*.*"*/, bool bSubDir/* = true*/, bool bAppendPath/* = true*/)
{
	char szFind[MAX_PATH] = { 0 };
	StrCpyA(szFind, lpPath);
	StrCatA(szFind, "/");
	StrCatA(szFind, filter);

	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = ::FindFirstFileA(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
		return;

	char szFile[MAX_PATH] = { 0 };
	while (TRUE)
	{
		szFile[0] = '\0';
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (FindFileData.cFileName[0] != '.')
			{
				if (bAppendPath)
				{
					StrCpyA(szFile, lpPath);
					StrCatA(szFile, "\\");
					StrCatA(szFile, FindFileData.cFileName);
				}
				else
				{
					StrCpyA(szFile, FindFileData.cFileName);
				}
				vctDir.push_back(szFile);

				if (bSubDir)
				{
					ListFilesA(szFile, vctDir, vctFiles, filter, bSubDir, bAppendPath);
				}
			}
		}
		else
		{
			if (bAppendPath)
			{
				StrCpyA(szFile, lpPath);
				StrCatA(szFile, "\\");
				StrCatA(szFile, FindFileData.cFileName);
			}
			else
			{
				StrCpyA(szFile, FindFileData.cFileName);
			}
			vctFiles.push_back(szFile);
		}

		if (!FindNextFileA(hFind, &FindFileData))
			break;
	}

	FindClose(hFind);
}

void ListFielsW(const wchar_t* lpPath, std::vector<std::wstring>& vctDir, std::vector<std::wstring>& vctFiles, const wchar_t* filter/* = L"*.*"*/, bool bSubDir/* = true*/, bool bAppendPath/* = true*/)
{
	wchar_t szFind[MAX_PATH];
	StrCpyW(szFind, lpPath);
	StrCatW(szFind, L"/");
	StrCatW(szFind, filter);

	WIN32_FIND_DATAW FindFileData;
	HANDLE hFind = ::FindFirstFileW(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
		return;

	wchar_t szFile[MAX_PATH];
	while (TRUE)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (FindFileData.cFileName[0] != '.')
			{
				if (bAppendPath)
				{
					StrCpyW(szFile, lpPath);
					StrCatW(szFile, L"/");
					StrCatW(szFile, FindFileData.cFileName);
				}
				else
				{
					StrCpyW(szFile, FindFileData.cFileName);
				}
				vctDir.push_back(szFile);

				if (bSubDir)
				{
					ListFielsW(szFile, vctDir, vctFiles, filter, bSubDir, bAppendPath);
				}
			}
		}
		else
		{
			if (bAppendPath)
			{
				StrCpyW(szFile, lpPath);
				StrCatW(szFile, L"/");
				StrCatW(szFile, FindFileData.cFileName);
			}
			else
			{
				StrCpyW(szFile, FindFileData.cFileName);
			}
			vctFiles.push_back(szFile);
		}

		if (!FindNextFileW(hFind, &FindFileData))
			break;
	}

	FindClose(hFind);
}

//函数名：MoveFolder
//参数：lpszFromPath 源文件夹路径 。lpszToPath 目的文件夹路径
//作用：移动原文件夹及其中文件都指定的路径下
/////////////////////////////////////
BOOL MoveFolder(LPCSTR lpszFromPath, LPCSTR lpszToPath)
{
	int nLengthFrm = strlen(lpszFromPath);
	char *NewPathFrm = new char[nLengthFrm + 2];
	strcpy(NewPathFrm, lpszFromPath);
	NewPathFrm[nLengthFrm] = '\0';
	NewPathFrm[nLengthFrm + 1] = '\0';

	SHFILEOPSTRUCTA FileOp;
	ZeroMemory((void*)&FileOp, sizeof(SHFILEOPSTRUCTA));
	FileOp.fFlags = FOF_NOCONFIRMATION;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = NewPathFrm;
	FileOp.pTo = lpszToPath;
	FileOp.wFunc = FO_MOVE;

	return SHFileOperationA(&FileOp) == 0;
}

/////////////////////////////////////
//ReNameFolder
//参数：lpszFromPath 源文件夹路径 。lpszToPath 目的文件夹路径
//作用：修改原文件夹的名字。
/////////////////////////////////////
BOOL ReNameFolder(LPCSTR lpszFromPath, LPCSTR lpszToPath)
{
	int nLengthFrm = strlen(lpszFromPath);
	char *NewPathFrm = new char[nLengthFrm + 2];
	strcpy(NewPathFrm, lpszFromPath);
	NewPathFrm[nLengthFrm] = '\0';
	NewPathFrm[nLengthFrm + 1] = '\0';

	SHFILEOPSTRUCTA FileOp;
	ZeroMemory((void*)&FileOp, sizeof(SHFILEOPSTRUCTA));
	FileOp.fFlags = FOF_NOCONFIRMATION;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = NewPathFrm;
	FileOp.pTo = lpszToPath;
	FileOp.wFunc = FO_RENAME;

	return SHFileOperationA(&FileOp) == 0;
}

/////////////////////////////////////
//函数名:DeleteFolder
//输入参数：LpszPath 要删除的路径指针
//作用：删除指定文件夹以及里面的文件
/////////////////////////////////////
BOOL DeleteFolder(LPCSTR lpszPath)
{
	int nLength = strlen(lpszPath);
	char *NewPath = new char[nLength + 2];
	strcpy(NewPath, lpszPath);
	NewPath[nLength] = '\0';
	NewPath[nLength + 1] = '\0';

	SHFILEOPSTRUCTA FileOp;
	ZeroMemory((void*)&FileOp, sizeof(SHFILEOPSTRUCTA));
	FileOp.fFlags = FOF_NOCONFIRMATION;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = NewPath;
	FileOp.pTo = NULL;
	FileOp.wFunc = FO_DELETE;

	return SHFileOperationA(&FileOp) == 0;
}

/////////////////////////////////////
//函数名：CopyFolder
//参数：lpszFromPath 源文件夹的路径 。 lpszToPath 目的文件夹的路径
//作用：拷贝文件夹及其文件夹中的所有内容
//////////////////////////////////////
BOOL CopyFolder(LPCSTR lpszFromPath, LPCSTR lpszToPath)
{
	int nLengthFrm = strlen(lpszFromPath);
	char *NewPathFrm = new char[nLengthFrm + 2];
	strcpy(NewPathFrm, lpszFromPath);
	NewPathFrm[nLengthFrm] = '\0';
	NewPathFrm[nLengthFrm + 1] = '\0';

	SHFILEOPSTRUCTA FileOp;
	ZeroMemory((void*)&FileOp, sizeof(SHFILEOPSTRUCTA));
	FileOp.fFlags = FOF_NOCONFIRMATION;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = NewPathFrm;
	FileOp.pTo = lpszToPath;
	FileOp.wFunc = FO_COPY;

	return SHFileOperationA(&FileOp) == 0;
}

bool IsSamePathA(const char* path1, const char* path2)
{
	CStringA s1 = path1;
	CStringA s2 = path2;

	while (s1.Replace("\\", "/") != 0) {}
	while (s1.Replace("//", "/") != 0) {}
	while (s2.Replace("\\", "/") != 0) {}
	while (s2.Replace("//", "/") != 0) {}

	return (s1 == s2);
}
bool IsSamePathW(const wchar_t* path1, const wchar_t* path2)
{
	CStringW s1 = path1;
	CStringW s2 = path2;

	while (s1.Replace(L"\\", L"/") != 0) {}
	while (s1.Replace(L"//", L"/") != 0) {}
	while (s2.Replace(L"\\", L"/") != 0) {}
	while (s2.Replace(L"//", L"/") != 0) {}

	return (s1 == s2);
}

CString GetDirFromPath(const CString& sPath)
{
	CString path = sPath;
	TCHAR c = path[path.GetLength() - 1];
	if (c == '\\' || c == '/')
		path = path.Mid(0, path.GetLength() - 1);

	int split1 = path.ReverseFind('/');
	int split2 = path.ReverseFind('\\');
	int split = max(split1, split2);
	if (-1 != split)
	{
		path = path.Mid(0, split);
	}

	return path;
}

CString GetFileNameFromPath(const CString& sPath)
{
	int split1 = sPath.ReverseFind('/');
	int split2 = sPath.ReverseFind('\\');
	int split = max(split1, split2);
	if (-1 == split)
		return _T("");

	return sPath.Mid(split + 1);
}

//跟据文件路径获得文件名或文件夹名（去除扩展名）
CString GetFileNameWithoutExtensionFromPath(const CString& sPath)
{
	CString fileName = GetFileNameFromPath(sPath);

	int dot = fileName.ReverseFind('.');
	if (dot == 0)
		return _T("");
	else if (dot < 0)
		return fileName;
	else
		return fileName.Mid(0, dot);

	return _T("");
}

//跟据文件路径获得扩展名
CString GetFileExtensionFromPath(const CString& sPath)
{
	CString fileName = GetFileNameFromPath(sPath);

	int dot = fileName.ReverseFind('.');
	if (dot < 0)
		return _T("");
	else
		return fileName.Mid(dot + 1);

	return _T("");
}

duk_ret_t js_load_json(duk_context *ctx)
{
	int n = GET_ARGC(ctx);  /* #argc */
	if (n < 2)
		RETURN_VOID();

	std::string sVarName, sJsonFile;
	{
		duk_size_t out = 0;
		const char *p = duk_get_lstring(ctx, 0, &out);
		if (NULL == p)
			RETURN_VOID();
		sVarName = p;
	}
	{
		duk_size_t out = 0;
		const char *p = duk_get_lstring(ctx, 1, &out);
		if (NULL == p)
			RETURN_VOID();
		GL::Utf82Ansi(sJsonFile, p);
	}

	std::string sJs = "var " + sVarName + "=";

	HANDLE hFile;
	hFile = CreateFileA(sJsonFile.c_str(),
		GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD sizeFile = GetFileSize(hFile, NULL);
		char *pText = new char[sizeFile + 1];
		pText[sizeFile] = '\0';
		ReadFile(hFile, pText, sizeFile, &sizeFile, NULL);
		CloseHandle(hFile);

		sJs += pText;
		delete[] pText;
	}
	else
	{
		sJs += "{\"Error\":\"Can not open json file.\"}";
	}

	JS::Context(ctx).eval(sJs.c_str());

	RETURN_VOID();
}

duk_ret_t js_alert(duk_context *ctx)
{
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
	MessageBoxA(NULL, msg.c_str(), "JS", 0);

	RETURN_VOID();
}

static std::vector< std::pair<std::string, COleVariant> > g_objProps;
duk_ret_t _js_push_obj_pair(duk_context *ctx)
{
	int nArg = GET_ARGC(ctx);
	if (nArg != 2)
		RETURN_VOID();

	duk_size_t lenName = 0;
	const char* name = duk_get_lstring(ctx, 0, &lenName);
	std::string sName;
	GL::Utf82Ansi(sName, name);

	COleVariant val;
	if (duk_is_boolean(ctx, 1))
	{
		val.boolVal = duk_get_boolean(ctx, 1) == 1 ? -1 : 0;
		val.vt = VT_BOOL;
	}
	else if (duk_is_number(ctx, 1))
	{
		double dv = duk_get_number(ctx, 1);
		int iv = duk_get_int(ctx, 1);
		if (iv == dv)
		{
			val.intVal = iv;
			val.vt = VT_INT;
		}
		else
		{
			val.dblVal = dv;
			val.vt = VT_R8; 
		}
	}
	else if (duk_is_string(ctx, 1))
	{
		duk_size_t lenVal = 0;
		const char* str = duk_get_lstring(ctx, 1, &lenVal);
		std::wstring ws;
		GL::Utf82WideByte(ws, str);
		val = COleVariant(CStringW(ws.c_str()));
	}

	g_objProps.push_back(std::make_pair(sName, val));

	RETURN_VOID();
}

//读取json文件，并且对象中的属性是按顺序的
//  只能读取简单单层的json结构
const std::vector< std::pair<std::string, COleVariant> >& GetJson(const char* path)
{
	std::string sJson;

	HANDLE hFile;
	hFile = CreateFileA(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD sizeFile = GetFileSize(hFile, NULL);
		char *pText = new char[sizeFile + 1];
		pText[sizeFile] = '\0';
		ReadFile(hFile, pText, sizeFile, &sizeFile, NULL);
		CloseHandle(hFile);

		sJson += pText;
		delete[] pText;
	}
	else
	{
		sJson += "{\"Error\":\"Can not open json file.\"}";
	}
	
	return ParseJson(sJson.c_str());
}

const std::vector< std::pair<std::string, COleVariant> >& ParseJson(const char* jsonStr)
{
	JS::Context js;
	js.addCFunction("loadJson", js_load_json);
	js.addCFunction("push_obj_pair", _js_push_obj_pair);

	std::string sJs = "var jsonObj=";
	sJs += jsonStr;
	sJs += ";var names=[];for(var name in jsonObj){push_obj_pair(name,jsonObj[name]);}";

	g_objProps.clear();

	try
	{
		js.eval(sJs.c_str());
	}
	catch (std::runtime_error ex)
	{

	}

	return g_objProps;
}
