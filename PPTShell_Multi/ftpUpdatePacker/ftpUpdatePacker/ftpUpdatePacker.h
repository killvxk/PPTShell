
// ftpUpdatePacker.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CftpUpdatePackerApp:
// �йش����ʵ�֣������ ftpUpdatePacker.cpp
//

class CftpUpdatePackerApp : public CWinApp
{
public:
	CftpUpdatePackerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CftpUpdatePackerApp theApp;