#ifndef _UTIL_H_
#define _UTIL_H_

#include <winsock2.h>

#include <string>
using namespace std;

//��������
#define DOWN_TYPE_NULL		-1
#define DOWN_TYPE_HTTP		1
#define DOWN_TYPE_FTP		2

//�����������
#define TIMEOUT				10	//��ʱʱ��
#define RE_CONN_TIME		2	//��������

//����������
#define URL_LEN_MAX			2048

//�Զ�����Ϣ
#define WM_DOWNMESSAGE (WM_USER+300)

#define UPDATE_TEMP_DIR		"temp\\"

typedef struct _DownInfo
{
	BOOL		bIsDown;
	int			nPer;
	LONG		nDownSize;
	LONG		nFileSize;
	int			nID;
	
	_DownInfo() : bIsDown(FALSE), nPer(0), nDownSize(0), nFileSize(-1), nID(0) {}
}DownInfo, *PDownInfo;

//�ַ���
int FindChar_f(LPSTR t, LPSTR s);
int FindChar_b(LPSTR t, LPSTR s);
int FindChar_n(LPSTR t, LPSTR s, int n);

//����
int JudgeProtocol(LPCSTR szUrl);
int ConnWithTime(SOCKET& s_conn, const char* szServ, const int nPort);

//HTTP
int SplitHttpUrl(char* szUrl, char* szServ, int& nPort, char* szPath);
LONG HttpSize(char* szUrl);
int HttpSend(SOCKET& s_conn, const char* szServ, const char* szPath, const LONG nLow=0, const LONG nRange=0);
LONGLONG GetNetFileSize(char* szUrl, PLONG plStop);

//��ȫ
BOOL CheckApiInLineHook(PVOID ApiAddress);
BOOL RemoveHook(LPCSTR szDllPath, LPCSTR szFuncName);

//����
bool JudgeDir(char* szPath);
void print(const char *fmt, ...);

//Ŀ¼����
string GetParentPath(const string& strFileName);
bool IsDir(const string& strFileName);
bool DirectoryMake(const string& strDirectory);
bool MiscMyMoveFile(const char* pcszSrcFile,const char* pcszDstFile);
bool MiscMyDeleteFile(const char* pcszFileName);

string GetFileName(const string& strFileName);
string StrReplace(string s, string oldSub, string newSub);
// ɾ��Ŀ¼
bool DirectoryDelete(const string& strDirectory);
// ȡ�ļ���
string GetFileTitle(const string& strFileName);
bool IsFileExist(const string& strFileName);

// �ж��Ƿ���Զ����ļ�
bool FileReadAllow(const string& strFileName);
unsigned __int64 GetFileSize(const string& strFileName);

BOOL MakeDir(const char* pszDir);

bool ChkDir();
bool CheckDat(char* sDatBuff);
bool SafeMove(LPCSTR lpExistingFileName, LPCSTR lpNewFileName);
bool isFileExsit(LPTSTR strPath);
//bool DirectoryDelete(char* szDirectory);
bool CreateHashFile(char* szTempPath, vector<UPDATEDATA> &list);
bool CreateHashFile(char* szTempPath, vector<UPDATEDATA> &list, vector<UPDATEDATA> &locallist);
bool CreateHashFile(char* szTempPath, vector<UPDATEDATA> &list, vector<UPDATEDATA> &locallist, vector<string> &vecRedownload, vector<string> &vecCover);

void ANSIToUnicode(char* szBuf, WCHAR *wszBuf);
void UnicodeToANSI(WCHAR *wszBuf, char* szBuf);

DWORD  CalcFileCRC(LPCTSTR lptcsPath);
DWORD  CalcCRC(char* ptr, DWORD Size);

bool CodePageConvert(const string& Source, string& Target, int SourceCodePage, int TargetCodePage);
#endif //_UTIL_H_
