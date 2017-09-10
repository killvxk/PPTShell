
// PPTViewer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPPTViewerApp:
// �йش����ʵ�֣������ PPTViewer.cpp
//

class CPPTViewerApp : public CWinAppEx
{
public:
	CPPTViewerApp();

// ��д
	public:
	virtual BOOL InitInstance();

	BOOL ExtractResource(char* szPath, int nResourceID, char* szResourceType);
	void ExtractDlls(char* szDestDir);
	void ExtractMainfests(char* szDestDir);
	void ExtractPptFile(char* szDestDir);
	void WritePlayListFile(char* szDestDir);
	void RunPPTViewer(char* szDestDir);

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPPTViewerApp theApp;