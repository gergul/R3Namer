
// R3Namer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CR3NamerApp: 
// �йش����ʵ�֣������ R3Namer.cpp
//

class CR3NamerApp : public CWinApp
{
public:
	CR3NamerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CR3NamerApp theApp;