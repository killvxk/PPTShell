
// PPTShell.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "PPTShell.h"
#include "GUI/MainFrm.h"
#include "GUI/PPTShellDoc.h"
#include "GUI/PPTShellView.h"
#include "GUI/SkinManager.h"
#include "DUI/DUICommon.h"
#include "GUI/ForceUpdateDlg.h"
#include "DUI/WndShadow.h"
#include "Statistics/Statistics.h"
#include "BlueTooth/BlueTooth.h"
#include "NDCloud/NDCloudAPI.h"
#include "EventCenter/EventDefine.h"
#include "EventCenter/EventCenter.h"
#include "PPTControl/PPTControllerManager.h"
#include "Http/HttpUploadManager.h"
#include "Util/HWID.h"
#include "DUI/LaserPointerDlg.h"
#include "Plugins/Icr/IcrPlayer.h"
#include "NDCloud/NDCloudLocalQuestion.h"
#include "DUI/ThirdPartyLogin.h"
#include "DUI/CoursePlayUI.h"
#include "DUI/ProgressLoading.h"
#include "AutoBuildStr.h"

#include <dbghelp.h> 
#pragma comment(lib, "dbghelp.lib")

#include "Config.h"
#include "DUI/QuestionPreviewDialogUI.h"
#include "DUI/QuestionPreviewFullScreenUI.h"
#include "NDCloud/NDCloudUser.h"


LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS *lpExceptionInfo);

#define  PPT_REGISTER_KEY_COUNT		3

CLogManager*		g_pLogMgr		   = NULL;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPPTShellApp
void OnProcessPacket(SOCKET sock, int type, char* packet);
void OnSocketClose(SOCKET sock);

BEGIN_MESSAGE_MAP(CPPTShellApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CPPTShellApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CPPTShellApp construction

CPPTShellApp::CPPTShellApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bFixedIME		= FALSE;
	m_bDBClickOpen	= FALSE;
	m_bFromNet		= FALSE;
}

// The one and only CPPTShellApp object

CPPTShellApp theApp;

LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS *lpExceptionInfo)
{
	LONG ret = EXCEPTION_EXECUTE_HANDLER;

	char szFileName[MAX_PATH]; 

	SYSTEMTIME st;
	GetLocalTime(&st); 

	tstring strPath = GetLocalPath();
	strPath += _T("\\Dump\\");
	CreateDirectory(strPath.c_str(), NULL);

#ifdef DEVELOP_VERSION
	tstring strBuild=_STR_BUILD_TIME;  
#else
	tstring strBuild=_STR_FINAL_TIME;  
#endif

	strBuild=replace_all_distinct(strBuild, _T("Build."), _T("b"));
	strBuild=replace_all_distinct(strBuild, _T("."), _T(""));
	wsprintf(szFileName, _T("%s%d-%d%d-%d%d-%d-%s.dmp"), strPath.c_str(), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,strBuild.c_str());

	HANDLE hFile = CreateFile(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if (hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION ExInfo;

		ExInfo.ThreadId				= GetCurrentThreadId();
		ExInfo.ExceptionPointers	= lpExceptionInfo;
		ExInfo.ClientPointers		= false;

		// write the dump
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL ); 
		CloseHandle(hFile);


		tstring strUpdatePath = GetLocalPath();
		strUpdatePath += _T("\\CrashCollector.exe");

		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		si.wShowWindow = SW_SHOW;

		char szParamFile[MAX_PATH];
		wsprintf(szParamFile, _T("%d-%d%d-%d%d-%d-%s.dmp"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,strBuild.c_str());
		tstring strCmd = szParamFile;
		strCmd += _T(" ");
		strCmd += NDCloudUser::GetInstance()->GetUserName().c_str();

		if (CreateProcess(strUpdatePath.c_str(),(LPTSTR)strCmd.c_str(),NULL,NULL,FALSE,0,NULL,GetLocalPath().c_str(),&si,&pi))
		{
			CloseHandle(pi.hThread);
		}

	}

	return EXCEPTION_EXECUTE_HANDLER;
}

// CPPTShellApp initialization



//////////////////////////////////////////////////////////////////////////
HWND ReadMainFrameHwnd()
{
	tstring strPath = GetLocalPath();
	strPath += _T("\\Setting\\Config.ini");
	TCHAR szHwnd[12]={0};
	GetPrivateProfileString(_T("config"),  _T("MainHwnd"), _T("0"),  szHwnd, sizeof(szHwnd),  strPath.c_str());
	return HWND(_ttoi(szHwnd));
}

BOOL isFixedLink()
{
	tstring strPath =  NDCloudFileManager::GetInstance()->GetNDCloudDirectory();
	strPath += _T("\\Config.ini");	
	int iGet = GetPrivateProfileInt(_T("config"),  _T("fixedlink"), 0,  strPath.c_str());
	if(iGet==2)
		return TRUE;
	else
		return FALSE;
}

void WriteFixedLink()
{
	tstring strPath =  NDCloudFileManager::GetInstance()->GetNDCloudDirectory();
	strPath += _T("\\Config.ini");
	TCHAR szConfig[]=_T("config");
	TCHAR szKey[]=_T("fixedlink");
	WritePrivateProfileString(szConfig,  szKey,  "2", strPath.c_str());	
}

void WriteRunPcReport()
{
	tstring strPath = GetLocalPath();
	strPath += _T("\\Setting\\Config.ini");
	TCHAR szConfig[]=_T("config");
	TCHAR szKey[]=_T("runpc");

	int iDay=GetPrivateProfileInt(szConfig, szKey, 0, strPath.c_str());
	CTime theTime = CTime::GetCurrentTime();
	if(iDay!=theTime.GetDay())
	{
		Statistics::GetInstance()->Report(STAT_RUNPC);
		TCHAR szTime[12]={0};
		wsprintf(szTime, "%d", theTime.GetDay());
		WritePrivateProfileString(szConfig,  szKey,  szTime, strPath.c_str());
	}
}



//�������ļ�
void SendMsgToMainProcess(tstring strFilePath)
{
	HWND hwMain=ReadMainFrameHwnd();
	if(IsWindow(hwMain))
	{
		DWORD dwRet = GetFileAttributes(strFilePath.c_str());
		if (dwRet == INVALID_FILE_ATTRIBUTES)
		{
			return;
		}
		COPYDATASTRUCT ps;
		ps.dwData=MSG_OPEN_FILE;
		ps.cbData=strFilePath.length()+sizeof(TCHAR);
		ps.lpData=(PVOID)strFilePath.c_str();
		//������Ϣ
		::SendMessage(hwMain,WM_COPYDATA, 0,(LPARAM)&ps);
	}
}

void SendMsgToMainProcess(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hwMain=ReadMainFrameHwnd();
	if(IsWindow(hwMain))
		::SendMessage(hwMain,uMsg, wParam, lParam);
}

BOOL RunExec(const TCHAR *cmd, const TCHAR *para, const TCHAR* dir, DWORD dwMilliseconds)
{
	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = _T("open");
	ShExecInfo.lpFile = cmd;
	ShExecInfo.lpParameters = para;
	ShExecInfo.lpDirectory = dir;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	BOOL suc = ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	return suc;
}

//test code ��ʱ���� �� �Է������°汾��ɾ��
//char szToolDir[MAX_PATH]; 
//sprintf(szToolDir, "%s\\Package\\tools", NDCloudFileManager::GetInstance()->GetNDCloudDirectory().c_str());
//char szImageFile[MAX_PATH]; 
//sprintf(szImageFile, "%s\\Package\\tools\\ƽ�漸��\\�ȱ�������.html", NDCloudFileManager::GetInstance()->GetNDCloudDirectory().c_str());
//if (GetFileAttributes(szImageFile) != INVALID_FILE_ATTRIBUTES)
//{
//	DeleteDir(szToolDir);
//}

BOOL UnzipCoursePlayer()
{

	KillExeCheckParentPid("node.exe");

	// js player path
	tstring strLocalPath = GetLocalPath();

	tstring strExePath=GetUnzipExePath();

	tstring strWorkDirectory = GetCoursePlayerPath();

	//tstring strZipPath= strWorkDirectory + _T("\\player.zip");
	//tstring strlocales= strWorkDirectory + _T("\\locales.zip");
	//tstring strtools= strWorkDirectory + _T("\\tools.zip");
	tstring strNodeJs = strWorkDirectory + _T("\\nodejs.zip");

	PROCESS_INFORMATION pi;
	STARTUPINFO si;    
	ZeroMemory( &pi, sizeof(pi) );
	ZeroMemory( &si, sizeof(si) );

	si.cb = sizeof(si);

	//unzip d:\player.zip d:\player
	TCHAR szParam[MAX_PATH*2]={0};
	tstring strSave=GetLocalPath();
	_stprintf_s(szParam, _T("unzip \"%s\" \"%s\\Package\""), strNodeJs.c_str(),  strSave.c_str());
	RunExec(strExePath.c_str(), szParam, strWorkDirectory.c_str(), INFINITE);
	
	// execute nodeJS server
	NDCloudLocalQuestionManager::GetInstance()->Initialize();
	setNodeJsRunning();


	return TRUE;
}


//�õ�������������·��

BOOL GetIEQuickLaunchPath(char *pszIEQueickLaunchPath)
{     
	LPITEMIDLIST  ppidl;                    
	if (SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &ppidl) == S_OK)
	{
		BOOL flag =   SHGetPathFromIDList(ppidl, pszIEQueickLaunchPath);
		strcat(pszIEQueickLaunchPath, "\\Microsoft\\Internet Explorer\\Quick Launch");
		CoTaskMemFree(ppidl);
		return flag;
	}
	return FALSE;
}

//�õ� ��ʼ->������ ��·��

BOOL GetProgramsPath(char *pszProgramsPath)
{
	LPITEMIDLIST  ppidl; 
	if (SHGetSpecialFolderLocation(NULL, CSIDL_COMMON_PROGRAMS, &ppidl) == S_OK)
	{
		BOOL flag = SHGetPathFromIDList(ppidl, pszProgramsPath);
		CoTaskMemFree(ppidl);
		return flag;
	}
	return FALSE;
}


//�õ���ǰ����·��

BOOL GetDesktopPathOLD(char *pszDesktopPath)
{
	LPITEMIDLIST  ppidl = NULL; 
	if (SHGetSpecialFolderLocation(NULL,  CSIDL_DESKTOPDIRECTORY, &ppidl) == S_OK)
	{
		BOOL flag =   SHGetPathFromIDList(ppidl, pszDesktopPath);
		CoTaskMemFree(ppidl);
		return flag;
	}
	return FALSE;
} 

//�õ���ǰ����·��

BOOL GetDesktopPath(char *pszDesktopPath)
{
	LPITEMIDLIST  ppidl = NULL; 
	if (SHGetSpecialFolderLocation(NULL,  CSIDL_COMMON_DESKTOPDIRECTORY, &ppidl) == S_OK)
	{
		BOOL flag =   SHGetPathFromIDList(ppidl, pszDesktopPath);
		CoTaskMemFree(ppidl);
		return flag;
	}
	return FALSE;
}     
/*

�������ܣ���ָ���ļ���ָ����Ŀ¼�´������ݷ�ʽ

����������

lpszFileName    ָ���ļ���ΪNULL��ʾ��ǰ���̵�EXE�ļ���

lpszLnkFileDir  ָ��Ŀ¼������ΪNULL��

lpszLnkFileName ��ݷ�ʽ���ƣ�ΪNULL��ʾEXE�ļ�����

wHotkey         Ϊ0��ʾ�����ÿ�ݼ�

pszDescription  ��ע

iShowCmd        ���з�ʽ��Ĭ��Ϊ���洰��

*/

BOOL CreateFileShortcut(LPCSTR lpszFileName, LPCSTR lpszLnkFileDir, LPCSTR lpszLnkFileName,       LPCSTR lpszWorkDir, WORD wHotkey, LPCTSTR lpszDescription, int iShowCmd = SW_SHOWNORMAL)

{
	CoInitialize(NULL);
	if (lpszLnkFileDir == NULL)

		return FALSE;

	HRESULT hr;
	IShellLink     *pLink;  //IShellLink����ָ��
	IPersistFile   *ppf; //IPersisFil����ָ��

	//����IShellLink����

	hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pLink);

	if (FAILED(hr))

		return FALSE;

	//��IShellLink�����л�ȡIPersistFile�ӿ�

	hr = pLink->QueryInterface(IID_IPersistFile, (void**)&ppf);

	if (FAILED(hr))
	{

		pLink->Release();
		return FALSE;
	}

	//Ŀ��

	if (lpszFileName == NULL)
		pLink->SetPath(_pgmptr);
	else
		pLink->SetPath(lpszFileName);



	//����Ŀ¼

	if (lpszWorkDir != NULL)
		pLink->SetPath(lpszWorkDir);
	//��ݼ�

	if (wHotkey != 0)
		pLink->SetHotkey(wHotkey);
	//��ע
	if (lpszDescription != NULL)
		pLink->SetDescription(lpszDescription);

	//��ʾ��ʽ
	pLink->SetShowCmd(iShowCmd);

	//��ݷ�ʽ��·�� + ����
	char szBuffer[MAX_PATH];
	if (lpszLnkFileName != NULL) //ָ���˿�ݷ�ʽ������
		sprintf(szBuffer, "%s\\%s", lpszLnkFileDir, lpszLnkFileName);
	else   
	{
		//û��ָ�����ƣ��ʹ�ȡָ���ļ����ļ�����Ϊ��ݷ�ʽ���ơ�

		CONST TCHAR *pstr;

		if (lpszFileName != NULL)

			pstr = _tcsrchr(lpszFileName, _T('\\'));

		else

			pstr = _tcsrchr(_pgmptr, _T('\\'));
		if (pstr == NULL)
		{     
			ppf->Release();
			pLink->Release();
			return FALSE;
		}
		//ע���׺��Ҫ��.exe��Ϊ.lnk
		sprintf(szBuffer, "%s\\%s", lpszLnkFileDir, pstr);
		int nLen = strlen(szBuffer);
		szBuffer[nLen - 3] = 'l';
		szBuffer[nLen - 2] = 'n';
		szBuffer[nLen - 1] = 'k';
	}

	//�����ݷ�ʽ��ָ��Ŀ¼��

	WCHAR  wsz[MAX_PATH];  //����Unicode�ַ���

	MultiByteToWideChar(CP_ACP, 0, szBuffer, -1, wsz, MAX_PATH);

	hr = ppf->Save(wsz, TRUE);


	ppf->Release();

	pLink->Release();

	CoUninitialize();
	return SUCCEEDED(hr);

}



BOOL RegOcx(tstring strOcx)
{
	tstring strFile="regsvr32.exe ";
	strFile = strFile + "\"" +strOcx + "\" /s";
	WinExec(strFile.c_str() ,SW_HIDE);
	return TRUE;
}

//�޸����룬ֻ���м����汾
//���������ݷ�ʽ
//try
//{
//
//	TCHAR  szPath[MAX_PATH];
//	GetDesktopPathOLD(szPath);
//	tstring strLnkFile=MAIN_APP_NAME;
//	strLnkFile="\\"+strLnkFile+_T(".lnk");;
//	strLnkFile=szPath+strLnkFile;
//	DeleteFile(strLnkFile.c_str());
//	if(isFixedLink()==FALSE)//һ������ֻ�޸�һ��
//	{
//		TCHAR  szPath[MAX_PATH];
//		GetDesktopPath(szPath);
//		tstring strLnkFile=MAIN_APP_NAME;
//		strLnkFile=strLnkFile+_T(".lnk");
//		if(CreateFileShortcut(NULL, szPath, strLnkFile.c_str(), NULL, MAKEWORD(VK_F1, HOTKEYF_CONTROL), MAIN_APP_NAME))
//		{
//			WriteFixedLink();
//		}
//		//���¿�ʼ�˵�
//		GetProgramsPath(szPath);
//		tstring strProgra=szPath;
//		strProgra=strProgra+"\\"+MAIN_APP_NAME;
//		CreateFileShortcut(NULL, strProgra.c_str(), strLnkFile.c_str(), NULL, MAKEWORD(VK_F1, HOTKEYF_CONTROL), MAIN_APP_NAME);
//
//		//���¿�������
//		GetIEQuickLaunchPath(szPath);
//		CreateFileShortcut(NULL, szPath, strLnkFile.c_str(), NULL, MAKEWORD(VK_F1, HOTKEYF_CONTROL), MAIN_APP_NAME);
//	}
//}
//catch (...)
//{
//}

//////////////////////////////////////////////////////////////////////////

BOOL CPPTShellApp::InitInstance()
{	

	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);

	//_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	g_pLogMgr = new CLogManager();
	g_pLogMgr->init();

	//�򿪲���
	// Parse command line for standard shell commands, DDE, file open

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	cmdInfo.m_nShellCommand = cmdInfo.FileNew;  //����ϵͳ�����̴��ĵ�
	if(GetFileAttributes(cmdInfo.m_strFileName.GetString()) != INVALID_FILE_ATTRIBUTES)
	{
		m_bDBClickOpen	= TRUE;
		m_strLoadFile	= cmdInfo.m_strFileName;
	}

	LPCTSTR lpcsCommandLine = GetCommandLine();
	if( _tcsstr(lpcsCommandLine, _T("pptshell:")) != NULL )
		m_bFromNet = TRUE;


	TCHAR szMutex[] = {"_Mutex_PPTShell_New"};
	HANDLE hMutex = CreateMutex(NULL, TRUE, szMutex);     
	DWORD dwRet = GetLastError();    
	if (hMutex)     
	{        
		if (ERROR_ALREADY_EXISTS == dwRet)         
		{
			//���ͼ�����Ϣ
			HWND hwLast=ReadMainFrameHwnd();
			if ( IsIconic(hwLast) )
				ShowWindow(hwLast, SW_SHOWNORMAL|SW_RESTORE);
			SetForegroundWindow(hwLast);

			// loading from website
			LPCTSTR lpcsCommandLine = GetCommandLine();
			if( strstr(lpcsCommandLine, _T("pptshell:")) != NULL )
			{
				// send message to main process
				COPYDATASTRUCT ps;
				ps.dwData	= MSG_OPEN_FILE_WEBSITE;
				ps.cbData	= _tcslen(lpcsCommandLine);
				ps.lpData	= (PVOID)lpcsCommandLine;

				SendMsgToMainProcess(WM_COPYDATA, 0, (LPARAM)&ps);
			}
			//���ʹ���Ϣ
			else if(m_strLoadFile!="")
			{
				SendMsgToMainProcess(m_strLoadFile);
			}

			//���̽�����
			CloseHandle(hMutex);             
			return 0;         
		}    
	}    
	else    
	{ 
		CloseHandle(hMutex);
		return 0;
	}

	try
	{

		// event center
		EventCenter::GetInstance()->Initialize();

		// http download mananger
		HttpDownloadManager::GetInstance()->Initialize(); 
		HttpUploadManager::GetInstance()->Initialize(); 
		// ndcloud
		CHttpDownloadManager* pHttpManager = HttpDownloadManager::GetInstance();
		NDCloudInitialize(pHttpManager);


		// icrplayer
		IcrPlayer::GetInstance()->Initialize();

		WriteRunPcReport();

		CWinApp::InitInstance();

		if (!AfxSocketInit())
		{
			return FALSE;
		}

	if ( !m_bFixedIME )
	{
		AfxOleInit();
		CoInitializeEx(NULL,COINIT_MULTITHREADED|COINIT_SPEED_OVER_MEMORY);
	}
	
	//m_strLoadFile = _T("");
	//LPCWSTR  lpCommand;
	//lpCommand = GetCommandLineW();
	//int iCommandNum = 0;
	//LPWSTR *CommandArray = CommandLineToArgvW(lpCommand, &iCommandNum);
		//// ��ʼ�� OLE �� ,�޸����뷨BUG
		try{
			OSVERSIONINFOEX os;
			::ZeroMemory(&os,sizeof(os));
			os.dwOSVersionInfoSize = sizeof(os);
			::GetVersionEx(reinterpret_cast<LPOSVERSIONINFO>(&os));	
			if ((os.dwMajorVersion > 6) || (os.dwMajorVersion == 6 && os.dwMinorVersion>=2))
			{
				if ( AfxOleInit() )
				{
					m_bFixedIME = TRUE;
					WRITE_LOG_LOCAL("CPPTShellApp::InitInstance(), m_bFixedIME is true!");
				}			
			}
		}catch(...)
		{

		}

		if ( !m_bFixedIME )
		{
			CoInitializeEx(NULL,COINIT_MULTITHREADED|COINIT_SPEED_OVER_MEMORY);
		}

		//m_strLoadFile = _T("");
		//LPCWSTR  lpCommand;
		//lpCommand = GetCommandLineW();
		//int iCommandNum = 0;
		//LPWSTR *CommandArray = CommandLineToArgvW(lpCommand, &iCommandNum);
		ThirdPartyLoginUI::GetInstance()->Initialize(m_hInstance);
		CoursePlayUI::GetInstance()->Initialize(m_hInstance);
		CQuestionPreviewDialogUI::Initialize( m_hInstance );
		CQuestionPreviewFullScreenUI::Initialize( m_hInstance );
		//if ( iCommandNum >= 2)
		//{
		//	m_strLoadFile = CommandArray[1];
		//}

		// duilib 
		CPaintManagerUI::SetInstance(m_hInstance);
		CWndShadow::Initialize(m_hInstance);


		AfxEnableControlContainer();

		_SetProcessDPIAware();  // ֧�ָ�DPI

		//CoInitializeEx(NULL,COINIT_MULTITHREADED|COINIT_SPEED_OVER_MEMORY);

		//KillExeCheckParentPid(PPT_EXE_NAME);

		//m_pUpdate		= new CUpdate;

		//m_pUpdate->m_pMainDlg = this->GetMainWnd();
		//if(m_pUpdate->CheckUpdate())
		//{
		//	m_pUpdate->startUpdate();
		//}

		// Standard initialization
		// If you are not using these features and wish to reduce the size
		// of your final executable, you should remove from the following
		// the specific initialization routines you do not need
		// Change the registry key under which our settings are stored
		// TODO: You should modify this string to be something appropriate
		// such as the name of your company or organization
		SetRegistryKey(_T("Local AppWizard-Generated Applications"));
		LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

		tstring strPath = GetLocalPath();
		strPath += _T("\\Skins\\Title\\");
		CSkinManager::GetInstance()->SetSkinPath(strPath.c_str());
		strPath = GetLocalPath();
		strPath += _T("\\Setting\\");
		CreateDirectory(strPath.c_str(),NULL);
		strPath =GetLocalPath() + _T("\\Package");
		CreateDirectory(strPath.c_str(),NULL);
		strPath =GetLocalPath() + _T("\\temp");
		CreateDirectory(strPath.c_str(),NULL);

	}
	catch (...)
	{
	}

	//������
	LPCWSTR  lpCommand;
	lpCommand = GetCommandLineW();
	int iCommandNum = 0;
	LPWSTR *CommandArray = CommandLineToArgvW(lpCommand, &iCommandNum);

	wstring strCommand = L"";
	if ( iCommandNum == 3)
	{
		strCommand = CommandArray[2];
	}

	//���update.exe
	tstring strUpdatePath;
	tstring strTempUpdatePath;
	strUpdatePath = GetLocalPath();
	strTempUpdatePath = strUpdatePath;
	strUpdatePath += _T("\\update.exe");
	strTempUpdatePath += _T("\\Temp\\update.exe");
	if(GetFileAttributes(strTempUpdatePath.c_str()) != INVALID_FILE_ATTRIBUTES)
	{
		MoveFileEx(strTempUpdatePath.c_str(), strUpdatePath.c_str(), MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING);
	}

	
	if(strCommand != L"restart" && strCommand != L"success")
	{
		tstring strHashListPath;
		tstring strUpdatePath;
		strHashListPath = GetLocalPath();
		strUpdatePath = strHashListPath;
		strHashListPath += _T("\\Temp\\UpdateHashList.dat");
		strUpdatePath += _T("\\update.exe");

		//�������м��ѹ�����Ƿ��ѹ
		tstring strNDCloudDir = NDCloudFileManager::GetInstance()->GetNDCloudDirectory();
		tstring strSrcPackagePath ; 
		tstring strDestPackageDir ; 
		tstring strPackagePath = GetLocalPath();
		strPackagePath += "\\skins\\config\\Package.xml";

		TiXmlDocument	docPackage;

		HANDLE hHashListFile = INVALID_HANDLE_VALUE;
		if(docPackage.LoadFile(strPackagePath.c_str()))
		{
			TiXmlElement* pRootElement = docPackage.FirstChildElement();
			if( pRootElement)
			{
				TiXmlElement* pListsElement = pRootElement->FirstChildElement();

				TiXmlElement* pChildElement;  
				if( pListsElement )
				{
					for ( pChildElement = pListsElement->FirstChildElement(); pChildElement != 0; pChildElement = pChildElement->NextSiblingElement())   
					{  
						TiXmlElement * pSrcPathElement = pChildElement->FirstChildElement();
						TiXmlElement * pDestPathElement = pSrcPathElement->NextSiblingElement();
						const char *szSrcPath = pSrcPathElement->GetText();
						const char *szDestDir = pDestPathElement->GetText();

						strSrcPackagePath = GetLocalPath();
						strSrcPackagePath += _T("\\");
						strDestPackageDir = strSrcPackagePath;
						strSrcPackagePath += szSrcPath;
						
						if(szDestDir[0] == '\\')//��ǰĿ¼
						{
							strDestPackageDir += szDestDir;
						}
						else//ndCloudĿ¼
						{
							strDestPackageDir = strNDCloudDir;
							strDestPackageDir += _T("\\");
							strDestPackageDir += szDestDir;
						}
						//
						
						CString cstrSrcPackagePath = strSrcPackagePath.c_str();
						cstrSrcPackagePath.Replace(_T("\\\\"), _T("\\"));
						if(GetFileAttributes(cstrSrcPackagePath.GetString()) != INVALID_FILE_ATTRIBUTES)
						{
							DWORD dwNewCrc = CalcFileCRC(cstrSrcPackagePath.GetString());
							// compare crc
							char szName[MAX_PATH] = {0};
							const char * p = strrchr(szSrcPath, '\\');
							if(p)
							{
								strcpy(szName, p+1);
							}
							char szNewCrc[MAX_PATH] = {0};
							sprintf(szNewCrc, "%08lX", dwNewCrc);


							string strConfigFilePath = GetLocalPath();
							strConfigFilePath += "\\Package\\config.ini";

							// compare crc
							char szCrc[MAX_PATH] = {0};
							GetPrivateProfileStringA("crc", szName, "", szCrc, MAX_PATH, strConfigFilePath.c_str());

							if( _tcsicmp(szCrc, szNewCrc) != 0)
							{
								
								if (INVALID_HANDLE_VALUE == hHashListFile)
								{
									hHashListFile = CreateFile(strHashListPath.c_str(), GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS , FILE_ATTRIBUTE_NORMAL, NULL);
								}

								if (INVALID_HANDLE_VALUE != hHashListFile)
								{
									char szBuff[MAX_PATH * 2] = {0};
									sprintf(szBuff,"%s|%s\r\n",szSrcPath,szNewCrc);

									DWORD dwSize = 0;
									WriteFile(hHashListFile, szBuff, strlen(szBuff), &dwSize, NULL);
								}
								
							}
						}	


					}  
				}
			}
		}

		if(INVALID_HANDLE_VALUE != hHashListFile)
		{
			CloseHandle(hHashListFile);
		}

		if(GetFileAttributes(strHashListPath.c_str()) != INVALID_FILE_ATTRIBUTES)
		{
			tstring strCmd = _T(" cover");
			if(!m_strLoadFile.empty() && GetFileAttributes(m_strLoadFile.c_str()) != INVALID_FILE_ATTRIBUTES)
			{
				strCmd += _T(" \"");
				strCmd += m_strLoadFile;
				strCmd += _T("\"");
			}
			else
			{
				strCmd += _T(" \"\"");
			}

			strCmd += _T(" \"");
			strCmd += NDCloudFileManager::GetInstance()->GetNDCloudDirectory();
			strCmd += _T("\"");

			STARTUPINFO si;
			PROCESS_INFORMATION pi;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			ZeroMemory(&pi, sizeof(pi));
			si.wShowWindow = SW_SHOW;
			si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

			if(GetFileAttributes(strUpdatePath.c_str()) != INVALID_FILE_ATTRIBUTES)
			{
				if (CreateProcess(strUpdatePath.c_str(),(LPSTR)strCmd.c_str(),NULL,NULL,TRUE,0,NULL,GetLocalPath().c_str(),&si,&pi))
				{
					CloseHandle(pi.hThread);
					return TRUE;
				}
			}
		}
		
		//
	}
	else
	{
		if(strCommand == L"success")
		{
			g_Config::GetInstance()->SetUpdatedStatus(eUpdateType_Update_Success);
			ModifyUninstallVersion();
		}

	}

	//ע��򿪷�ʽ
	RegisterOpenMode();

	//falsh������
	CLSID clsid;
	if ( !SUCCEEDED(CLSIDFromProgID(L"ShockwaveFlash.ShockwaveFlash", &clsid)) )
	{
		int nRet = UIMessageBox( NULL, _T("����δ��װflash���, ���ܵ�����Ƶ�޷����ţ�"), _T("101����PPT"), _T("����flash,����"), CMessageBoxUI::enMessageBoxTypeWarn, IDCANCEL, NULL, 340, 150, true, NULL );
		if ( nRet == ID_MSGBOX_BTN )
		{
			ShellExecute( NULL, _T("open"), _T("http://p.101.com/down/install_flash_player_20_active_x.exe"), NULL, NULL, SW_SHOW );
		}
	}

	//ǿɱ��ʾ
	if( FindProcess(PPT_EXE_NAME)==TRUE)
	{
		int nRet = UIMessageBox(NULL, _T("101����PPT��Ҫ�ر�PowerPoint��ʾ�ĸ壬�����ȱ����ĸ岢�˳���"), _T("101����PPT"), _T("��������,�˳�"), CMessageBoxUI::enMessageBoxTypeWarn, IDCANCEL);
		if (nRet != ID_MSGBOX_BTN)
		{
			return IDYES;
		}
	}

	KillExeCheckParentPid(PPT_EXE_NAME);
	

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CPPTShellDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CPPTShellView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

/*	CPPTController::CheckPPTRunTime();*/

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
#ifdef _DEBUG
	m_pMainWnd->ShowWindow(SW_SHOW);
#else
	m_pMainWnd->ShowWindow(SW_MAXIMIZE);
#endif

	::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)UnzipCoursePlayer, this, 0, NULL);

	m_pMainWnd->UpdateWindow();
	::SetWindowText(m_pMainWnd->GetSafeHwnd(), _T("101����PPT"));
	//m_pMainWnd->ModifyStyle(m_pMainWnd->GetSafeHwnd(), WS_CAPTION, 0, 0);  //�޸���ʽΪû�б���
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand

	// Generate PPTShell GUID
	GeneratePPTID();

	PacketProcesser::GetInstance()->Initialize();
	FileTransferManager::GetInstance()->Initialize();

	BlueToothServer::GetInstance()->Initialize(OnProcessPacket, OnSocketClose);
	BlueToothServer::GetInstance()->StartWork();

	LANServer::GetInstance()->Initialize(OnProcessPacket, OnSocketClose);
	LANServer::GetInstance()->StartWork(17777);

	WANClientManager::GetInstance()->Initialize("61.160.40.166", 18888, m_dwPPTID);
	WANClientManager::GetInstance()->SetRecvCloseNotifyFunc(OnProcessPacket, OnSocketClose);

	PPTInfoSender::GetInstance()->Initialize();
	Statistics::GetInstance()->Initialize();
	BlueToothMonitor::GetInstance()->Initialize();


	//��ʾ������
	HWND hWnd = ::FindWindow("Shell_TrayWnd",NULL); 
	::ShowWindow(hWnd, SW_SHOWNORMAL);
	//��ʾ��ʼ��ť2015.11.11 cws
	HWND hStar = ::FindWindow(_T("Button"),NULL);  
	::ShowWindow(hStar, SW_SHOWNORMAL);

	// loading from website
	lpcsCommandLine = GetCommandLine();
	if( strstr(lpcsCommandLine, _T("pptshell:")) != NULL )
	{
		CProgressLoadingUI* pLoadingDialog = new CProgressLoadingUI();
		pLoadingDialog->ShowWindow();
		pLoadingDialog->Start(lpcsCommandLine, MakeHttpDelegate(NULL), MakeHttpDelegate(NULL));
	}
	

	// Report statistics
	Statistics::GetInstance()->Report(STAT_RUN);

	return TRUE;
}


int CPPTShellApp::ExitInstance()
{
	g_pLogMgr->destroy();
	delete g_pLogMgr;
	CSkinManager::UnLoadSkin();
	return CWinApp::ExitInstance();
}


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CPPTShellApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CPPTShellApp::LoadCustomState()
{
}

void CPPTShellApp::SaveCustomState()
{
}

// CPPTShellApp ��Ϣ�������
BOOL CPPTShellApp::SendPacket(SOCKET sock, char *pData, int nDataSize, int nConnectType /*= 0*/)
{
	if( nConnectType == CONNECT_TYPE_LAN )
		LANServer::GetInstance()->SendPacket(sock, pData, nDataSize);

	else if( nConnectType == CONNECT_TYPE_WAN )
		WANClientManager::GetInstance()->SendPacket(sock, pData, nDataSize);

	else if( nConnectType == CONNECT_TYPE_BLUETOOTH )
		BlueToothServer::GetInstance()->SendPacket(sock, pData, nDataSize);


	return TRUE;
}

BOOL CPPTShellApp::SendStreamPacket(SOCKET sock, CStream *pStream, int nConnectType/* = 0*/)
{
	char* pData = pStream->GetBuffer();
	int nDataSize = pStream->GetDataSize();

	return SendPacket(sock, pData, nDataSize, nConnectType);
}

BOOL CPPTShellApp::SetPacketSentNotifyFunc(SOCKET sock ,int nConnectType, PVOID pPacketSentNotifyFunc)
{
	return TRUE;
}

BOOL CPPTShellApp::GeneratePPTID()
{
	cHWID hwid;

	char szHardwareInfo[1024] = {0};
	hwid.getHardwareIDString(szHardwareInfo);

	char szSystemInfo[1024] = {0};
	hwid.getSystemIDString(szSystemInfo);

	string str = szHardwareInfo;
	str += szSystemInfo;

	m_dwPPTID = CalcCRC((char*)str.c_str(), str.length());
	return TRUE;
}

DWORD CPPTShellApp::GetPPTID()
{
	return m_dwPPTID;
}

void OnProcessPacket(SOCKET sock, int type, char* packet)
{
	CPacketProcessor* pProcessor = PacketProcesser::GetInstance();
	if( pProcessor != NULL )
		pProcessor->OnProcessPacket(sock, type, packet);
}


void OnSocketClose(SOCKET sock)
{
	CPacketProcessor* pProcessor = PacketProcesser::GetInstance();
	if( pProcessor != NULL )
		pProcessor->OnSocketClosed(sock);
}


//
//void CPPTShellApp::SetWindowClass()
//{
//	WNDCLASS wc = {0};
//	::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wc);
//	wc.lpszClassName= _T("PPTShell");
//	AfxRegisterClass(&wc);
//}

BOOL CPPTShellApp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if((pMsg->message == WM_KEYUP  ||  pMsg->message == WM_KEYDOWN) &&  pMsg->wParam == VK_F5)
	{	
		//m_pptController.Play();
		if( GetAsyncKeyState(VK_LBUTTON) == 0 )
			((CMainFrame*)m_pMainWnd)->OnPptPlay();

		return FALSE;
	}		

	return CWinApp::PreTranslateMessage(pMsg);
}

//---------------------------------------------------------------------------

// ע���ļ�����

// strExe: Ҫ������չ��(����: ".txt")

// strAppName: Ҫ������Ӧ�ó�����(����: "C:\MyApp\MyApp.exe")

// strAppKey: ExeName��չ����ע����еļ�ֵ(����: "txtfile")

// strDefaultIcon: ��չ��ΪstrAppName��ͼ���ļ�(����: "C:\MyApp\MyApp.exe,0")

// strDescribe: �ļ���������

void CPPTShellApp::RegisterOpenMode()
{
	//ʹ���������������й�����ʾ������

	//BOOL bUsed = GetPrivateProfileInt("App", "Reg", 0, m_strAppPath+INI_NAME);

	//if ( bUsed )
	//	return;

	try
	{
		OSVERSIONINFOEX osvi;  
		BOOL bOsVersionInfoEx;  

		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));  
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);  
		bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*) &osvi);  

		// win7��ϵͳ�汾ΪNT6.1   win8��Ҫ���� ����ʽ����Ҫ�򿪼��
		//if ( VER_PLATFORM_WIN32_NT == osvi.dwPlatformId &&    
		//	osvi.dwMajorVersion >= 6 &&   
		//	osvi.dwMinorVersion >= 2 )  
		//{  

			CString strExt[PPT_REGISTER_KEY_COUNT];
			strExt[0] = _T(".ppt");
			strExt[1] = _T(".pptx");
			strExt[2] = _T(".ndpx");

			char strAppKey[20] = "PPTShell"; 


			// ����2���޸�ppt�����Ļᵼ����ppt�޷�����

			for (int i = 0; i < PPT_REGISTER_KEY_COUNT; i++ )
			{
				BOOL relationExists = CheckFileRelation(strExt[i].GetString(), strAppKey);
				if(!relationExists)
				{
					TCHAR szExePath[MAX_PATH] = {0};
					GetModuleFileName(NULL, szExePath, MAX_PATH);
					TCHAR strDefaultIcon[MAX_PATH + 1] = {0};       
					sprintf(strDefaultIcon, "%s,0", szExePath);
					TCHAR strDescribe[100] = "101����PPT�μ�";
					RegisterFileRelation((TCHAR*)strExt[i].GetString(), szExePath, strAppKey, strDefaultIcon, strDescribe);
				}
			}

			//�޸�win8 ���� //write root �򿪷�ʽ
			char strShellAppKey[] = "PPTShell\\Shell\\Open\\Command";        
			bool bWrite = CheckSelfRelation(strShellAppKey);
			if(!bWrite)
			{
				TCHAR szExePath[MAX_PATH] = {0};
				GetModuleFileName(NULL, szExePath, MAX_PATH);
				TCHAR strDefaultIcon[MAX_PATH + 1] = {0};       
				sprintf(strDefaultIcon, "%s,0", szExePath);
				TCHAR strDescribe[100] = "101����PPT�μ�";
				RegisterFileRelation(strAppKey, szExePath, strAppKey, strDefaultIcon, strDescribe);
			}
		//}

		//д��URL Protocol
		tstring strWrite=_T("URL Protocol");
		HKEY hKeyTemp = NULL;
		//д��root
		if ( ERROR_SUCCESS == RegOpenKeyEx(HKEY_CLASSES_ROOT, strAppKey, 0, KEY_READ|KEY_SET_VALUE, &hKeyTemp))
		{
			DWORD dwReserved = 0;
			RegSetValueEx(hKeyTemp, strWrite.c_str(), dwReserved, REG_SZ, NULL, 0);
			RegCloseKey(hKeyTemp);
		}	

		//д���ֿ�����ʾѡ�� outlookfeeds
		char strUrlAppKey[] = _T("outlookfeeds\\Shell\\Open\\Command");        
		if(!CheckSelfRelation(strUrlAppKey))
		{	
			TCHAR szExePath[MAX_PATH] = {0};
			GetModuleFileName(NULL, szExePath, MAX_PATH);
			RegisterURLProtocol(_T("outlookfeeds"), szExePath);
		}




	}
	catch (...)
	{
	}

	CString strKey[PPT_REGISTER_KEY_COUNT];

	CString strOpenWithList[PPT_REGISTER_KEY_COUNT];
	CString strOpenWithProgids[PPT_REGISTER_KEY_COUNT];

	CString strUserChoice[PPT_REGISTER_KEY_COUNT];
	CString strUserChoiceKey[PPT_REGISTER_KEY_COUNT];

	strKey[0] = (_T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.pptx\\"));
	strKey[1] = (_T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.ppt\\"));
	strKey[2] = (_T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.ndpx\\"));

	for ( int i = 0; i < PPT_REGISTER_KEY_COUNT ; i++)
	{
		strOpenWithList[i]		= _T("OpenWithList");
		strOpenWithProgids[i]	= _T("OpenWithProgids");

		strUserChoice[i]		= _T("UserChoice");
		strUserChoiceKey[i]		= _T("Progid");
	}


	CString strPPTShell = _T("Software\\Classes\\Applications\\PPTShell\\shell\\open\\command");

	TCHAR szExePath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szExePath, MAX_PATH);

	CString strUserChoiceValue = _T("Applications\\PPTShell");

	CString strCommand = _T("\"");
	strCommand += szExePath;
	strCommand += _T("\" \"%1\"");

	HKEY hKey = NULL;
	TCHAR szValue[MAX_PATH] = {0};
	DWORD dwSize = MAX_PATH;
	DWORD dwType = REG_SZ;
	//ע��PPTSHELL·��
	DWORD dwReserved = 0;
	//ע��DLL
	if ( ERROR_SUCCESS != RegOpenKeyEx(HKEY_CURRENT_USER, strPPTShell, 0, KEY_READ|KEY_SET_VALUE, &hKey))
	{
		RegCreateKey(HKEY_CURRENT_USER, strPPTShell, &hKey);
		RegSetValueEx(hKey, NULL, dwReserved, REG_SZ, (BYTE*)strCommand.GetString(), _tcslen(strCommand.GetString()));
	}
	else
	{
		RegQueryValueEx(hKey, NULL, NULL, &dwType, (BYTE*)szValue, &dwSize);
		if ( (strCommand.CompareNoCase(szValue) != 0) || (_tcslen(szValue) == 0) )
		{
			RegSetValueEx(hKey, NULL, dwReserved, REG_SZ, (BYTE*)strCommand.GetString(), _tcslen(strCommand.GetString()));
		}
	}

	//ע��PPT�򿪷�ʽ
	for(int i = 0; i < 2; i++)
	{
		DWORD dwReserved = 0;
		CString strDelKey =	strKey[i] + strOpenWithList[i];
		RegDeleteKey(HKEY_CURRENT_USER, strDelKey.GetString()); //ֱ��ɾ������д�롣 ʹ��ϵͳѡ��򿪷�ʽ�󣬲��ܸ�ֵ��ֻ��ɾ����д
		strDelKey =	strKey[i] + strOpenWithProgids[i];
		RegDeleteKey(HKEY_CURRENT_USER, strDelKey.GetString()); 
		strDelKey =	strKey[i] + strUserChoice[i];	//����ֻ��Ҫд�����ֵ�Ϳ�����
		RegDeleteKey(HKEY_CURRENT_USER, strDelKey.GetString()); 
		//ע��DLL
		if ( ERROR_SUCCESS != RegOpenKeyEx(HKEY_CURRENT_USER, strDelKey.GetString(), 0, KEY_READ|KEY_SET_VALUE, &hKey))  
		{
			RegCreateKey(HKEY_CURRENT_USER, strDelKey, &hKey);
			RegSetValueEx(hKey, _T("Progid"), dwReserved, REG_SZ, (BYTE*)strUserChoiceValue.GetString(), _tcslen(strUserChoiceValue.GetString()));
		}
		else
		{
			RegQueryValueEx(hKey, strUserChoiceKey[i], NULL, &dwType, (BYTE*)szValue, &dwSize);
			if ( (strUserChoiceValue.Compare(szValue) != 0) || (_tcslen(szValue) == 0) )
			{
				RegSetValueEx(hKey, _T("Progid"), dwReserved, REG_SZ, (BYTE*)strUserChoiceValue.GetString(), _tcslen(strUserChoiceValue.GetString()));
			}
		}

		RegCloseKey(hKey);
	}

	//WritePrivateProfileString("APP", "Reg", "1", m_strAppPath+INI_NAME);
}


BOOL CPPTShellApp::CheckSelfRelation(const char *strAppKey)
{
	int nRet = FALSE;
	HKEY hExtKey;
	char szPath[_MAX_PATH] = {0};
	char szSelfPath[_MAX_PATH] = {0};
	GetModuleFileName(NULL, szSelfPath, MAX_PATH);
	DWORD dwSize = sizeof(szPath);
	if(RegOpenKey(HKEY_CLASSES_ROOT, strAppKey, &hExtKey) == ERROR_SUCCESS)
	{

		RegQueryValueEx(hExtKey, NULL, NULL, NULL, (LPBYTE)szPath, &dwSize);


		if(_tcsstr(szPath , szSelfPath) != NULL)
		{
			nRet=TRUE;
		}

		RegCloseKey(hExtKey);
		return nRet;
	}
	return nRet;
}

BOOL CPPTShellApp::CheckFileRelation(const char *strExt, const char *strAppKey)
{
	int nRet = FALSE;
	HKEY hExtKey;
	char szPath[_MAX_PATH] = {0};
	DWORD dwSize = sizeof(szPath);

	if(RegOpenKey(HKEY_CLASSES_ROOT, strExt, &hExtKey) == ERROR_SUCCESS)
	{

		RegQueryValueEx(hExtKey, NULL, NULL, NULL, (LPBYTE)szPath, &dwSize);

		if(_stricmp(szPath, strAppKey) == 0)
		{
			nRet=TRUE;
		}

		RegCloseKey(hExtKey);
		return nRet;
	}
	return nRet;
}

void CPPTShellApp::RegisterURLProtocol(TCHAR *pszProtocal, TCHAR *pszExePath)
{
	char strTemp[_MAX_PATH]={0};
	HKEY hKey;
	RegCreateKey(HKEY_CLASSES_ROOT,pszProtocal,&hKey);
	RegSetValueEx(hKey,_T("URL Protocol"), 0, REG_SZ, NULL, 0);
	RegCloseKey(hKey);   
	wsprintf(strTemp, _T("%s\\Shell"), pszProtocal);
	RegCreateKey(HKEY_CLASSES_ROOT,strTemp,&hKey);
	RegSetValue(hKey,_T(""),REG_SZ, _T("Open"),_tcslen(_T("Open"))+1);
	RegCloseKey(hKey);
	wsprintf(strTemp,_T("%s\\Shell\\Open\\Command"), pszProtocal);

	RegCreateKey(HKEY_CLASSES_ROOT,strTemp,&hKey);
	wsprintf(strTemp,_T("\"%s\" \"%%1\""),pszExePath);
	RegSetValue(hKey,_T(""),REG_SZ,strTemp,strlen(strTemp)+1);

	RegCloseKey(hKey);
}


void CPPTShellApp::RegisterFileRelation(char *strExt, char *strAppName, char *strAppKey, char *strDefaultIcon, char *strDescribe)
{
	char strTemp[_MAX_PATH];

	HKEY hKey;

	RegCreateKey(HKEY_CLASSES_ROOT,strExt,&hKey);

	RegSetValue(hKey,"",REG_SZ,strAppKey,strlen(strAppKey)+1);

	RegCloseKey(hKey);   

	RegCreateKey(HKEY_CLASSES_ROOT,strAppKey,&hKey);

	RegSetValue(hKey,"",REG_SZ,strDescribe,strlen(strDescribe)+1);

	RegCloseKey(hKey);

	sprintf(strTemp,"%s\\DefaultIcon",strAppKey);

	RegCreateKey(HKEY_CLASSES_ROOT,strTemp,&hKey);

	RegSetValue(hKey,"",REG_SZ,strDefaultIcon,strlen(strDefaultIcon)+1);

	RegCloseKey(hKey);

	sprintf(strTemp,"%s\\Shell",strAppKey);

	RegCreateKey(HKEY_CLASSES_ROOT,strTemp,&hKey);

	RegSetValue(hKey,"",REG_SZ,"Open",strlen("Open")+1);

	RegCloseKey(hKey);

	sprintf(strTemp,"%s\\Shell\\Open\\Command",strAppKey);

	RegCreateKey(HKEY_CLASSES_ROOT,strTemp,&hKey);

	sprintf(strTemp,"\"%s\" \"%%1\"",strAppName);

	RegSetValue(hKey,"",REG_SZ,strTemp,strlen(strTemp)+1);

	RegCloseKey(hKey);

}

void CPPTShellApp::ModifyUninstallVersion()
{
	tstring strRegVersion = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\101����PPT");
	tstring strCurVersion = g_Config::GetInstance()->GetVersion();
	//��
	HKEY hKey;
	if ( ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, strRegVersion.c_str(), 0, KEY_READ|KEY_SET_VALUE, &hKey))
	{
		DWORD dwReserved = 0;
		RegSetValueEx(hKey, _T("DisplayVersion"), dwReserved, REG_SZ, (BYTE*)strCurVersion.c_str(), _tcslen(strCurVersion.c_str()));
	}
}

void logMsg(char* filename, int logtype, int logfolder, char *format, ...)
{
	va_list va;
	va_start(va, format);

	if( filename == NULL )
		g_pLogMgr->logMsg(logtype, logfolder, format, va);

	else
		g_pLogMgr->logMsg(filename, format, va);

	va_end(va);

}

void logToOutput(char *format,...)
{
	va_list va;
	va_start(va, format);
	static char sContent[LOG_MAX_BUFFER_LENGTH];
	memset(sContent, 0x0, sizeof(sContent));
	vsnprintf(sContent, LOG_MAX_BUFFER_LENGTH-1,format,va);
	strcat(sContent, "\n");
	OutputDebugString(sContent);
	va_end(va);
}