
// PPTViewer.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "PPTViewer.h"
#include "PPTViewerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPPTViewerApp

BEGIN_MESSAGE_MAP(CPPTViewerApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPPTViewerApp ����

CPPTViewerApp::CPPTViewerApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CPPTViewerApp ����

CPPTViewerApp theApp;


// CPPTViewerApp ��ʼ��

BOOL CPPTViewerApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));


	//
	char szDestPath[MAX_PATH];
	char szTempPath[MAX_PATH];

	GetTempPath(MAX_PATH, szTempPath);
	sprintf(szDestPath, "%s\\ppt", szTempPath);

	CreateDirectory(szDestPath, NULL);

	ExtractDlls(szDestPath);
	ExtractMainfests(szDestPath);
	ExtractPptFile(szDestPath);
	WritePlayListFile(szDestPath);
	RunPPTViewer(szDestPath);

	ExitProcess(0);

	CPPTViewerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

BOOL CPPTViewerApp::ExtractResource(char* szPath, int nResourceID, char* szResourceType)
{
	HRSRC hRsrc = FindResourceA(NULL, MAKEINTRESOURCE(nResourceID), szResourceType);
	if( hRsrc == NULL )
		return FALSE;

	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
	if( hGlobal == NULL )
		return FALSE;

	DWORD dwSize = SizeofResource(NULL, hRsrc);
	if( dwSize == 0 )
		return FALSE;

	char* pBuffer = (char*)LockResource(hGlobal);
	if( pBuffer == NULL )
		return FALSE;

	FILE* fp = fopen(szPath, "wb");
	if( fp == NULL )
		return FALSE;

	fwrite(pBuffer, dwSize, 1, fp);
	fclose(fp);

	UnlockResource(hGlobal);
	return TRUE;
}


void CPPTViewerApp::ExtractDlls(char* szDestDir)
{
	char* szNames[] = 
	{
		"PPTVIEW.EXE",
		"INTLDATE.DLL",
		"msvcm80.dll",
		"msvcp80.dll",
		"MSVCR80.dll",
		"OGL.DLL",
		"PPVWINTL.DLL",
		"SAEXT.DLL",
	};

	for(int i = IDR_DLL_PPTVIEW; i <= IDR_DLL_SAEXT; i++)
	{
		char szPath[MAX_PATH];
		sprintf(szPath, "%s\\%s", szDestDir, szNames[i-IDR_DLL_PPTVIEW]);

		 ExtractResource(szPath, i, "DLL");
	}

}

void CPPTViewerApp::ExtractMainfests(char* szDestDir)
{
	char* szNames[] = 
	{
		"microsoft.vc80.crt.manifest",
		"pptview.exe.manifest",
	};

	for(int i = IDR_TXT_MICRO; i <= IDR_TXT_PPTVIEW; i++)
	{
		char szPath[MAX_PATH];
		sprintf(szPath, "%s\\%s", szDestDir, szNames[i-IDR_TXT_MICRO]);

		ExtractResource(szPath, i, "TXT");
	}

}

void CPPTViewerApp::ExtractPptFile(char* szDestDir)
{
	// extract ppt file
	char szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	
	FILE* fp = fopen(szPath, "rb");
	if( fp == NULL )
		return;

	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char* pBuffer = new char[size];
	if( pBuffer == NULL )
		return;

	fread(pBuffer, size, 1, fp);
	fclose(fp);

	DWORD dwPptSize = *(DWORD*)(pBuffer+size-4);
	DWORD dwOffset = size - dwPptSize - 4;

	char szPptFilePath[MAX_PATH];
	sprintf(szPptFilePath, "%s\\test.ppt", szDestDir);

	fp = fopen(szPptFilePath, "wb");
	if( fp == NULL )
		return;

	fwrite(pBuffer+dwOffset, dwPptSize, 1, fp);
	fclose(fp);

}

void CPPTViewerApp::WritePlayListFile(char* szDestDir)
{	
	char szPath[MAX_PATH];
	sprintf(szPath, "%s\\playlist.txt", szDestDir);
	
	FILE* fp = fopen(szPath, "wb");
	if( fp == NULL )
		return;

	char* name = "test.ppt";

	fwrite(name, strlen(name), 1, fp);
	fclose(fp);


}

void CPPTViewerApp::RunPPTViewer(char* szDestDir)
{
	char szPath[MAX_PATH];
	sprintf(szPath, "%s\\PPTVIEW.EXE", szDestDir);

	ShellExecute(NULL, "open", szPath, "/L \"playlist.txt\"", szDestDir, SW_SHOW);
}