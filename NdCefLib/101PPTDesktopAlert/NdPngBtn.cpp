#include "StdAfx.h"
#include <windows.h>
#include "NdPngBtn.h"
#include "Shellapi.h" 


CGdiPlusBitmapResource* CNdPngBtn::pbgImage = NULL;
bool CNdPngBtn::m_bIsMouseOver = false;

CNdPngBtn::CNdPngBtn(void)
{
}

CNdPngBtn::~CNdPngBtn(void)
{
}

ATOM CNdPngBtn::NdBtnRegisterClass(HINSTANCE hInstance){
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= NdBtnProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= _T("NDBTNCLASS");
	wcex.hIconSm		= NULL;

	return RegisterClassEx(&wcex);
}

BOOL CNdPngBtn::NdBtnInitInstance(HINSTANCE hInstance,HWND hWndParent, int nCmdShow){
	HWND hWnd;
	NdBtnRegisterClass(hInstance);
	hWnd = CreateWindow(_T("NDBTNCLASS"), L"", WS_CHILD|WS_VISIBLE,
		250, 214, 100, 35, hWndParent, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	return TRUE;
}
void  CNdPngBtn::NdDrawImage(HWND hWnd,HDC hdc)
{
	//����ͼ��
	if (pbgImage==NULL){
		pbgImage = new CGdiPlusBitmapResource;
	}
	if (pbgImage){
		if(pbgImage->Load(IDB_LOOK, L"PNG")){

			//��ͼ
			Graphics graphics(hdc);
			if (m_bIsMouseOver){
				graphics.DrawImage(*pbgImage,0,-34);
			}else{
				graphics.DrawImage(*pbgImage,0,0);
			}
			//0,0); 0 -33 -68
			FontFamily fontFamily(L"΢���ź�"); 
			Gdiplus::Font font(&fontFamily, 14, FontStyleRegular, UnitPixel);  
			RECT rcClient ;  
			GetClientRect(hWnd,&rcClient);  
		    SolidBrush brush(Color(17, 176, 182));  
			StringFormat stringformat;  
			stringformat.SetAlignment(StringAlignmentCenter);  
			stringformat.SetLineAlignment(StringAlignmentCenter);
			graphics.DrawString(_T("�����鿴"),-1, &font,  
				RectF(0, 0, rcClient.right-rcClient.left,rcClient.bottom-rcClient.top), &stringformat, &brush);
		}
	}
	//if(pbgImage) delete pbgImage;
	return;
}

bool ShellExecuteExPPT(std::wstring path)
{
	SHELLEXECUTEINFO sei = {sizeof(SHELLEXECUTEINFO)};
	sei.lpVerb = L"runas";
	sei.lpFile = path.c_str();
	sei.hwnd = NULL;
	sei.nShow = SW_NORMAL;
	if (!ShellExecuteEx(&sei))
	{
	}
	return true;
}
bool ProcessInServicePPT(){
	TCHAR szPath[MAX_PATH];
	if( !GetModuleFileName( NULL, szPath, MAX_PATH ) )
	{
		return false;
	}
	std::wstring nPath(szPath);
	nPath = nPath.substr(0,nPath.find_last_of('\\'));
	nPath+= L"\\..\\..\\101PPT.exe";
	return ShellExecuteExPPT(nPath);
}
LRESULT CALLBACK CNdPngBtn::NdBtnProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		{
		}
		break;
	case WM_MOUSEMOVE:
		if (!m_bIsMouseOver){
			TRACKMOUSEEVENT csTME;
			csTME.cbSize = sizeof (csTME);
			csTME.dwFlags = TME_LEAVE|TME_HOVER;
			csTME.hwndTrack = hWnd ;// ָ��Ҫ ׷�� �Ĵ��� 
			csTME.dwHoverTime = 10;  // ����ڰ�ť��ͣ������ 10ms ������Ϊ״̬Ϊ HOVER
			::TrackMouseEvent(&csTME); // ���� Windows �� WM_MOUSELEAVE �� WM_MOUSEHOVER �¼�֧��
			m_bIsMouseOver   =   true; 
			RECT rcClient ;  
			GetClientRect(hWnd,&rcClient);  
			InvalidateRect(hWnd,&rcClient,false);
		}
		break;
	case WM_MOUSELEAVE:
		if (m_bIsMouseOver){
			m_bIsMouseOver   =   false; 
			RECT rcClient ;  
			GetClientRect(hWnd,&rcClient); 
			InvalidateRect(hWnd,&rcClient,false);
		} 
		break;
	case WM_LBUTTONDOWN:
		{
			ProcessInServicePPT();
			HWND hPWnd = GetParent(hWnd);
			SendMessage(hPWnd,WM_DESTROY,0,0);
		}
		break;
	case WM_CLOSE:
		break;
	case WM_ERASEBKGND:
		{
			HDC hdc = (HDC)wParam;
			CNdPngBtn::NdDrawImage(hWnd,hdc);
		}
		return 0;
	case WM_PRINTCLIENT:
		{
			HDC hdc = (HDC)wParam;
			CNdPngBtn::NdDrawImage(hWnd,hdc);
		}
		return 0;
	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			CNdPngBtn::NdDrawImage(hWnd,hdc);
			EndPaint(hWnd, &ps);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}