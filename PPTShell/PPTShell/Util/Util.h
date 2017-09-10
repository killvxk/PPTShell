//===========================================================================
// FileName:				Util.h
// 
// Desc:				
//============================================================================
#ifndef _UTIL_H_
#define _UTIL_H_


#include "Http/HttpDelegate.h"
#include "Update/UpdateOperation.h"
#include "NDCloud/ChapterTree.h"
#include "ThirdParty/Tinyxml/tinyxml.h"
#include "ThirdParty/zip/ZipWrapper.h"

//
// WM_DROPFILE FIXED
//
BOOL ChangeWindowMessageFilterForWin8();


//�ж�nodejs�Ƿ��ѹ�������
bool isNodeJSRunning();
void setNodeJsRunning();

//
// string conversion
//
string		Utf8ToAnsi(string utf);
wstring		Utf8ToUnicode(string utf);
string		AnsiToUtf8(string ansi);
string		UnicodeToUtf8(wstring unicode);
wstring		AnsiToUnicode(string ansi);
string		UnicodeToAnsi(wstring unicode);


tstring		Ansi2Str(const string& str);
tstring		Un2Str(const wstring& str);
tstring		Utf82Str(const string& str);
wstring		Str2Unicode(const tstring& str);
string		Str2Ansi(const tstring& str);
string		Str2Utf8(const tstring& str);

vector<tstring> SplitString( tstring strData, size_t nSize, TCHAR chSpe, bool bPassNull );

tstring		LTrim(tstring &str);
tstring		RTrim(tstring &str);
tstring		Trim(tstring &str);
INT64		GetNanoTime();
INT64		GetMilliTime();
string		NewGuid(bool bCross = true, bool bLowerCase = true);

//
// device information
//
tstring		GetPCName();
string		GetMac();
DWORD		GetPCID();
void		GetIPs(vector<DWORD>& vecIPs);
void		GetScreenResolution(int& nScreenWidth, int& nScreenHeight);

//
// others
//
DWORD		CalcCRC(char* ptr, DWORD Size);
DWORD		CalcFileCRC(LPCTSTR lptcsPath);
tstring     GetToolsPath();
tstring     GetQuestionJsonPath();
tstring		GetHtmlPlayerPath();
tstring     GetUnzipExePath();
tstring     GetCoursePlayerExePath();
tstring     GetCoursePlayerPath();
tstring     GetAndCefX64Path();
tstring		GetModulePath();
tstring		GetLocalPath();
tstring		GetSkinsDir();
tstring		UrlEncode(string str, bool bFullUrl = true);
tstring		UrlEncodeEx(string str);
tstring&	trim(tstring &s);
unsigned __int64 GetCycleCount();

void AdjustPrivilege();
BOOL FindProcess(tstring strExeName);
int KillExeCheckParentPid(tstring strExeName, BOOL bCloseAll = FALSE) ; //bCloseAll�Ƿ�ر����н���
int KillExeCheckParentPidNotPlayer(tstring strExeName, BOOL bCloseAll );

tstring CreateFlashThumbFile(tstring strFlashFile);
bool GetLocalIpMacs(vector<pair<string, string>>& vecIpMacs);	// IP <--> MAC

//chapterTree
ChapterNode* TraversalChapterTree(ChapterNode* pNode, BOOL bDirect);
ChapterNode* TraversalParentChapterTree(ChapterNode* pNode, BOOL bDirect);
ChapterNode* TraversalChildChapterTree(ChapterNode* pNode, BOOL bDirect);
BOOL isEndChapterTreeNode(ChapterNode* pNode, BOOL bDirect);

// pic 

bool GetEncoderClsid( const WCHAR* format, CLSID* pClsid );
tstring CreateAdaptiveScreenPicture(tstring strPicPath);


bool CompressImagePixel(   
						const WCHAR* pszOriFilePath,   
						const WCHAR* pszDestFilePah,   
						UINT ulMaxHeigth,   
						UINT ulMaxWidth, 
						WCHAR* format = L"image/jpeg"); 

bool CompressImageQuality(   
						const WCHAR* pszOriFilePath,   
						const WCHAR* pszDestFilePah,  
						ULONG quality, 
						WCHAR* format = L"image/jpeg");  

bool GetImageSize(tstring strPath, UINT& nWidth, UINT& nHeight);

// create question thumbnail
tstring GenerateQuestionThumbnail(tstring strPath);


//
//xml
//
TiXmlElement* GetElementsByTagName(TiXmlElement* pElement, string strTagName);
bool createDirWithFullPath(TCHAR* strDirName);
bool isICRPlayer();//��ʱ��ͨ���жϱ����Ƿ��л������ó������жϻ���


//SavePassword
typedef struct _PASSWORD_UNIT
{
	char szRandKeyStart[MAX_PATH];
	char szUserName[MAX_PATH * 2 + 1];
	char szPassword[MAX_PATH * 2 + 1] ;
	char szRandKeyEnd[MAX_PATH];
	DWORD dwCRC;
}PASSWORD_UNIT, *PPASSWORD_UNIT;

void	EncryptA(const char * pInput, int nInPutLen, char * pOutput, const char * szKey);
void	DecryptA(const char * pInput,int nInPutLen ,char * pOutput, const char * szKey);
bool	SavePassword(tstring strUserName, tstring strPassword);
bool	GetPassword(LPTSTR szUserName, LPTSTR szPassword);
bool	RemoveSaveAccount();
//

// Zip
bool ZipPackDir(string strSrcDicPath);
bool UnZipPackDir(string strSrcFile, string strDestPath);
bool CopyDir(CString strSrcPath, CString strDstPath, BOOL bFailIfExists = FALSE);
bool CopyDir(CString strSrcPath, CString strDstPath, LPTSTR szFailPath);
bool DeleteDir(CString strPath);
//

//�ַ���
int FindChar_f(LPSTR t, LPSTR s);
int FindChar_b(LPSTR t, LPSTR s);
int FindChar_n(LPSTR t, LPSTR s, int n);
int ConnWithTime(SOCKET& s_conn, int nTimeOut, int nReConnTime, const char* szServ, const int nPort, BOOL bStop);
//

BOOL EnableDebugPriv();
BOOL SetPowerPointDPIAware(HWND hwnd);
void _SetProcessDPIAware();
//


//calc md5
tstring CalcFileMD5(tstring strFilePath);
tstring CalcMD5(tstring str);
//
typedef struct LOCAL_RES_INFO
{
	tstring		strChapterGuid;
	tstring		strChapter;
	tstring		strResPath;
	tstring		strTitle;
	int			nType;

	friend bool operator == (const LOCAL_RES_INFO &resA, const LOCAL_RES_INFO &resB)  
	{  
		return (_tcsicmp(resA.strResPath.c_str(), resB.strResPath.c_str()) == 0);  
	}  

	friend bool operator < (const LOCAL_RES_INFO &resA, const LOCAL_RES_INFO &resB)  
	{  
		return (_tcsicmp(resA.strResPath.c_str(), resB.strResPath.c_str()) < 0);  
	}  

} _sLOCAL_RES_INFO, *spLOCAL_RES_INFO;

#define FILTER_PICTURE _T("ͼƬ (*.bmp;*.jpg;*.jpeg;*.png;*.gif)\0*.bmp;*.jpg;*.jpeg;*.png;*gif\0\0")

#define FILTER_PPT	_T("PowerPoint ��ʾ�ĸ�(*.pptx)\0*.pptx\0PowerPoint 97 - 2003 ��ʾ�ĸ�(*.ppt)\0*.ppt\0\0")	//\0PPT����ļ�(*.ndpx)\0*.ndpx

#define FILTER_PPT_AND_NDPX	_T("��ʾ�ĸ� (*.pptx;*.ppt;*.ndpx)\0*.pptx;*.ppt;*.ndpx\0PowerPoint ��ʾ�ĸ�(*.pptx)\0*.pptx\0PowerPoint 97 - 2003 ��ʾ�ĸ�(*.ppt)\0*.ppt\0PPT����ļ�(*.ndpx)\0*.ndpx\0\0")

#define FILTER_VIDEO _T("ӰƬ�ļ� (*.asf;*.avi;*.mov;*.mp4;*.3gp;*.mpeg;*.mpg;*.wmv)\0*.asf;*.avi;*.mov;*.mp4;*.3gp;*.mpeg;*.mpg;*.wmv\0\
								Windows Media file (*.asf;*.wmv)\0*.asf;*.wmv\0\
								Windows video file (*.avi)\0*.avi\0\
								QuickTime Movie file (*.mov)\0*.mov\0\
								MP4 Video (*.mp4;*.3gp)\0*.mp4;*.3gp\0\
								Movie file (*.mpeg;*.mpg)\0*.mpeg;*.mpg\0\
								Windows Media Video file (*.wmv;)\0*.wmv\0\0")

#define	FILTER_PPT_ALL	_T("PowerPoint ��ʾ�ĸ�(*.pptx;*.ppt)\0*.pptx;*.ppt\0\0")

#define FILTER_VIDEO_ALL _T("��Ƶ (*.asf;*.avi;*.mov;*.mp4;*.3gp;*.mpeg;*.mpg;*.wmv)\0*.asf;*.avi;*.mov;*.mp4;*.3gp;*.mpeg;*.mpg;*.wmv\0\0")

#define FILTER_FALSH	_T("���� (*.swf)\0*.swf\0\0")

#define FILTER_VOLMUE	_T("��Ƶ (*.wav;*.mp3;*.wma;*.mid)\0*.wav;*.mp3;*.wma;*.mid\0\0")

#define FILTER_ALL _T("֧���ļ�\0*.ppt;*.pptx;*.ndpx;*.bmp;*.jpg;*.jpeg;*.png;*gif;*.mov;*.wmv;*.asf;*.mpg;*.mpeg;*.mp4;*.3gp;*.avi;*.wav;*.mp3;*.wma;*.mid;*.swf\0\
								PowerPoint ��ʾ�ĸ�(*.ppt;*.pptx;*.ndpx)\0*.ppt;*.pptx;*.ndpx\0\
								ͼƬ (*.bmp;*.jpg;*.jpeg;*.png;*.gif)\0*.bmp;*.jpg;*.jpeg;*.png;*.gif\0\
								��Ƶ (*.asf;*.avi;*.mov;*.mp4;*.3gp;*.mpeg;*.mpg;*.wmv)\0*.asf;*.avi;*.mov;*.mp4;*.3gp;*.mpeg;*.mpg;*.wmv\0\
								���� (*.swf)\0*.swf\0\
								��Ƶ (*.wav;*.mp3;*.wma;*.mid)\0*.wav;*.mp3;*.wma;*.mid\0\0")

#define FILTER_IMPORT_ALL _T("֧���ļ�\0*.ppt;*.pptx;*.bmp;*.jpg;*.jpeg;*.png;*gif;*.mov;*.wmv;*.asf;*.mpg;*.mpeg;*.mp4;*.3gp;*.avi;*.wav;*.mp3;*.wma;*.mid;*.swf\0\
PowerPoint ��ʾ�ĸ�(*.ppt;*.pptx;)\0*.ppt;*.pptx;\0\
ͼƬ (*.bmp;*.jpg;*.jpeg;*.png;*.gif)\0*.bmp;*.jpg;*.jpeg;*.png;*.gif\0\
��Ƶ (*.asf;*.avi;*.mov;*.mp4;*.3gp;*.mpeg;*.mpg;*.wmv)\0*.asf;*.avi;*.mov;*.mp4;*.3gp;*.mpeg;*.mpg;*.wmv\0\
���� (*.swf)\0*.swf\0\
��Ƶ (*.wav;*.mp3;*.wma;*.mid)\0*.wav;*.mp3;*.wma;*.mid\0\0")



//file dialog

UINT_PTR
CALLBACK
OFNHookProc(
			HWND	hdlg,
			UINT	uiMsg,
			WPARAM	wParam,
			LPARAM	lParam
			);


BOOL	FileOpenDialog(LPCTSTR lptcsTitile, LPCTSTR lptcsFilter, DWORD Flags, tstring& vctrFiles, BOOL& bReadOnly, HWND hWndParent = NULL);
BOOL	FileSelectDialog(LPCTSTR lptcsTitile, LPCTSTR lptcsFilter, DWORD Flags, vector<tstring>& vctrFiles, HWND hWndParent = NULL);
BOOL	FileSaveDialog(LPCTSTR lptcsTitile, LPCTSTR lptcsDefalutName, LPCTSTR lptcsFilter, DWORD Flags, tstring& strFile, HWND hWndParent = NULL);
BOOL	IsFileDialogShow();
//
// openssl
//
int base64_encode(char *str,int str_len,char *encode,int encode_len);
int base64_decode(char *str,int str_len,char *decode,int decode_buffer_len);



tstring&  replace_all_distinct(tstring& str, const tstring& old_value, const tstring& new_value) ;

//������ܼ�����
void SleepRunNull(int iMilliSeconds);



template<class Type>
std::string toString(const Type &t)
{
	std::string  strTemp;
	std::ostringstream temp;
	temp << t;
	strTemp = temp.str();
	return  strTemp;
}

enum OS_BIT_TYPE
{
	OS_BIT_TYPE_32 = 0,
	OS_BIT_TYPE_64,
};
OS_BIT_TYPE	GetOsVersion();		// 32λ��64λ����ϵͳ�ж�

bool CheckBinIs64(string strBinFile);	// �ж�exe dll�Ƿ���64λ

#endif