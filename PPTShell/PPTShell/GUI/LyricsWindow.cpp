// LyricsWindow.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LyricsWindow.h"

#define DANMUKU_WND_NAME _T("��Ļ����")
#define	DANMUKU_WND_CLASS _T("Danmuku_Wnd_Class")
#define MAX_WORD_COUNT 20
// CLyricsWindow

IMPLEMENT_DYNAMIC(CLyricsWindow, CWnd)

CLyricsWindow::CLyricsWindow()
{
	HDC hDC=::GetDC(NULL);
	m_hCacheDC=::CreateCompatibleDC(hDC);
	::ReleaseDC(NULL,hDC);
	//---------------------------------
	m_lpszLyrics=NULL ; //Unicode��ʽ�ĸ��
	m_nHighlight=NULL ; //������ʵİٷֱ� 0--100
	m_TextGradientMode=LyricsGradientMode_Two ; //��ͨ��ʽ���ģʽ
	m_pTextPen=NULL ; //��ͨ��ʱ߿򻭱�
	m_HighlightGradientMode=LyricsGradientMode_Two ; //������ʽ���ģʽ
	m_pHighlightPen=NULL ; //������ʱ߿򻭱�
	m_pShadowBrush=NULL ; //��Ӱ��ˢ,GDIPlus��ˢ 
	m_nShadowOffset=LyricsGradientMode_Two ; //��Ӱƫ��
	m_pFont=NULL ; //GDIPlus����
	m_FontStyle=NULL ; 
	m_FontSize=NULL ; 
	m_pTextFormat=NULL;
	//---------------------------------
	m_pFontFamily=new Gdiplus::FontFamily();
	m_pTextFormat=new Gdiplus::StringFormat();
	m_pTextFormat->SetFormatFlags(StringFormatFlagsNoWrap);//������
	m_pTextFormat->SetAlignment(StringAlignmentCenter); //��ˮƽ���뷽ʽ
	m_pTextFormat->SetLineAlignment(StringAlignmentNear); //�ô�ֱ���뷽ʽ
	//---------------------------------
	//SetLyricsFont(L"����",50,FontStyleBold);
	SetLyricsColor(Gdiplus::Color::Red,Gdiplus::Color(255,172,0),LyricsGradientMode_Three);
	SetLyricsBorder(Gdiplus::Color::Black,1);
	SetLyricsShadow(Gdiplus::Color(150,0,0,0),2);
	SetHighlightColor(Gdiplus::Color(255,100,26),Gdiplus::Color(255,255,0),LyricsGradientMode_Three);
	SetHighlightBorder(Gdiplus::Color::Black,1);

}

CLyricsWindow::~CLyricsWindow()
{
	if(m_lpszLyrics){
		delete  m_lpszLyrics;
		m_lpszLyrics=NULL;
	}
	if(m_pTextPen){
		delete m_pTextPen;
		m_pTextPen=NULL;
	}
	if(m_pHighlightPen){
		delete m_pHighlightPen;
		m_pHighlightPen=NULL;
	}
	if(m_pShadowBrush){
		delete m_pShadowBrush;
		m_pShadowBrush=NULL;
	}
	if(m_pFontFamily){
		delete m_pFontFamily;
		m_pFontFamily=NULL;
	}	
	if(m_pTextFormat){
		delete m_pTextFormat;
		m_pTextFormat=NULL;
	}	
	if(m_pFont){
		delete m_pFont;
		m_pFont=NULL;
	}
}


BEGIN_MESSAGE_MAP(CLyricsWindow, CWnd)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


BOOL CLyricsWindow::Create(HWND hParentWnd)
{

	if(!RegisterWndClass(DANMUKU_WND_CLASS))
	{
		TRACE("Class��Registration��Failedn");
	}

	//m_nWidth = GetSystemMetrics (SM_CXSCREEN);
	//m_nHeight =  GetSystemMetrics (SM_CYSCREEN);
	RECT rt;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0) ;   // ��ù�������С
	m_nWidth = rt.right - rt.left;
	m_nHeight = rt.bottom - rt.top;

	int x = 0;
	int y = 0;
	DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT;
	//DWORD dwStyle = WS_POPUP | WS_VISIBLE | WS_SYSMENU ;
	DWORD dwStyle = WS_POPUP;

	if(!CWnd::CreateEx(dwExStyle,DANMUKU_WND_CLASS,DANMUKU_WND_NAME,dwStyle,x,y,m_nWidth,m_nHeight,hParentWnd,NULL))
		return FALSE;

	//���������С ��Ļ��������25��
	m_emSize = m_nHeight/25;
	//m_emSize = (m_emSize > 50)?50:m_emSize;
	SetLyricsFont(L"����",m_emSize,FontStyleBold);

	m_goWordModel = RightTurn;

	InitializeCriticalSection(&g_cs);//��ʼ���ٽ��� 

	return TRUE;

}

BOOL CLyricsWindow::RegisterWndClass(LPCTSTR lpszClassName)
{
	HINSTANCE hInstance=AfxGetInstanceHandle();
	WNDCLASSEX wndcls;
	memset(&wndcls,0,sizeof(WNDCLASSEX));
	wndcls.cbSize=sizeof(WNDCLASSEX);
	if(GetClassInfoEx(hInstance,lpszClassName,&wndcls))
	{
		return TRUE;
	}
	if(GetClassInfoEx(NULL,lpszClassName,&wndcls))
	{
		return TRUE;
	}

	wndcls.style=CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
	wndcls.lpfnWndProc=::DefWindowProc;
	wndcls.hInstance=hInstance;
	wndcls.hIcon=NULL;
	wndcls.hCursor=::LoadCursor(NULL,IDC_ARROW);
	wndcls.hbrBackground=(HBRUSH)(COLOR_BTNFACE+1);
	wndcls.lpszMenuName=NULL;
	wndcls.lpszClassName=lpszClassName;
	if(!RegisterClassEx(&wndcls))
	{
		return FALSE;
	}
	return TRUE;
}


//���¸��(����ı�,�������Ȱٷֱ�)
void CLyricsWindow::UpdateLyrics(LPCSTR lpszLyrics,int nHighlight)
{
	if(m_lpszLyrics){
		delete  m_lpszLyrics;
		m_lpszLyrics=NULL;
	}
	if(lpszLyrics){
		//���ݽ�����ANSI�汾���ַ���,��Ҫת����Unicode
		int nLen=MultiByteToWideChar(CP_ACP,0,lpszLyrics,-1, NULL,NULL);
		if(nLen){
			m_lpszLyrics = new WCHAR[nLen+1];
			ZeroMemory(m_lpszLyrics,sizeof(WCHAR)*(nLen+1));
			MultiByteToWideChar(CP_ACP,0,lpszLyrics,-1,m_lpszLyrics,nLen); 
		}		
	}
	UpdateLyrics(nHighlight);
}
void CLyricsWindow::UpdateLyrics(LPCWSTR lpszLyrics,int nHighlight)
{
	if(m_lpszLyrics){
		delete  m_lpszLyrics;
		 m_lpszLyrics=NULL;
	}
	int nLen=0;
	if(lpszLyrics){
		nLen=lstrlenW(lpszLyrics);
	}
	if(nLen>0){
		m_lpszLyrics=new WCHAR[nLen+1];
		ZeroMemory(m_lpszLyrics,sizeof(WCHAR)*(nLen+1));
		CopyMemory(m_lpszLyrics,lpszLyrics,sizeof(WCHAR)*(nLen));
	}
	UpdateLyrics(nHighlight);
}
//���¸�������(�������Ȱٷֱ�)
void CLyricsWindow::UpdateLyrics(int nHighlight)
{
	m_nHighlight=nHighlight;
	if(m_nHighlight<0)
		m_nHighlight=0;
	if(m_nHighlight>100)
		m_nHighlight=100;
	Draw();
}
//�ػ���ʴ���
void CLyricsWindow::Draw()
{
	CRect rcClient;
	GetClientRect(rcClient);
	m_nWidth=rcClient.Width();
	m_nHeight=rcClient.Height();
	//----------------------------------
	BITMAPINFO bitmapinfo;
	bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapinfo.bmiHeader.biBitCount = 32;
	bitmapinfo.bmiHeader.biHeight = m_nHeight;
	bitmapinfo.bmiHeader.biWidth = m_nWidth;
	bitmapinfo.bmiHeader.biPlanes = 1;
	bitmapinfo.bmiHeader.biCompression=BI_RGB;
	bitmapinfo.bmiHeader.biXPelsPerMeter=0;
	bitmapinfo.bmiHeader.biYPelsPerMeter=0;
	bitmapinfo.bmiHeader.biClrUsed=0;
	bitmapinfo.bmiHeader.biClrImportant=0;
	bitmapinfo.bmiHeader.biSizeImage = bitmapinfo.bmiHeader.biWidth * bitmapinfo.bmiHeader.biHeight * bitmapinfo.bmiHeader.biBitCount / 8;
	HBITMAP hBitmap=CreateDIBSection (m_hCacheDC,&bitmapinfo, 0,NULL, 0, 0);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject (m_hCacheDC,hBitmap);
	//----------------------------------
	Gdiplus::Graphics* pGraphics=new Gdiplus::Graphics(m_hCacheDC);
	pGraphics->SetSmoothingMode (SmoothingModeAntiAlias);
	pGraphics->SetTextRenderingHint (TextRenderingHintAntiAlias);
	DrawLyrics(pGraphics);
	delete pGraphics;
	//----------------------------------
	//����͸������
	CPoint DestPt(0,0);
	CSize psize(m_nWidth,m_nHeight);
	BLENDFUNCTION blendFunc32bpp;
	blendFunc32bpp.AlphaFormat = AC_SRC_ALPHA;
	blendFunc32bpp.BlendFlags = 0;
	blendFunc32bpp.BlendOp = AC_SRC_OVER;
	blendFunc32bpp.SourceConstantAlpha = 255;
	HDC hDC=::GetDC(m_hWnd);
	::UpdateLayeredWindow(m_hWnd,hDC,NULL,&psize,m_hCacheDC,&DestPt,0,&blendFunc32bpp,ULW_ALPHA);
	//----------------------------------
	//�ͷ���Դ
	::SelectObject (m_hCacheDC,hOldBitmap);
	::DeleteObject(hBitmap);
	::ReleaseDC(m_hWnd,hDC);
}
//���Ƹ��
void CLyricsWindow::DrawLyrics(Gdiplus::Graphics* pGraphics)
{
	//EnterCriticalSection(&g_cs);

	//��ȡ�����ֿ�Ⱥ͸߶�
	RectF layoutRect(0,0,0,0);
	RectF boundingBox;

	if (m_goWordModel == LeftTurn)
	{
		int size = m_WordInfoArray.GetCount();
		for (int i = 0; i < size; i++)
		{		

			if (m_WordInfoArray.GetAt(i)->dstRect.X > m_nWidth)
			{
				m_WordInfoArray.GetAt(i)->DeleteMark = TRUE;
			}
			else
			{
				if (m_WordInfoArray.GetAt(i)->isFirstTime)
				{
					LPWSTR lpszWord = m_WordInfoArray.GetAt(i)->lpszWord;
					pGraphics->MeasureString (lpszWord, -1, m_pFont,layoutRect, m_pTextFormat,&boundingBox, 0, 0);
					m_WordInfoArray.GetAt(i)->dstRect.X -= boundingBox.Width;
					m_WordInfoArray.GetAt(i)->isFirstTime = FALSE;
				}
				else
				{
					m_WordInfoArray.GetAt(i)->dstRect.X += m_WordInfoArray.GetAt(i)->speed;
				}

			}

		}

		for (int j = size - 1; j >= 0; j--)
		{
			if (m_WordInfoArray.GetAt(j)->DeleteMark)
			{
				LPWordInfo lpWordInfo = m_WordInfoArray.GetAt(j);
				m_WordInfoArray.RemoveAt(j);
				delete lpWordInfo;
				continue;
			}

		}
	}
	else if (m_goWordModel == RightTurn)
	{
		int size = m_WordInfoArray.GetCount();
		if (size == 0)
		{
			ShowWindow(SW_HIDE);
			return;
		}
		for (int i = 0; i < min(size, MAX_WORD_COUNT); i++)
		{		
			LPWSTR lpszWord = m_WordInfoArray.GetAt(i)->lpszWord;
			pGraphics->MeasureString (lpszWord, -1, m_pFont,layoutRect, m_pTextFormat,&boundingBox, 0, 0);

			if (m_WordInfoArray.GetAt(i)->dstRect.X < (0-boundingBox.Width))
			{
				m_WordInfoArray.GetAt(i)->DeleteMark = TRUE;
			}
			else
			{
				if (size == 1)  //�����Ļ�Ĵ���ֻ��һ�䣬�Ͳ���Ҫ����������Ƚ�λ�ӣ���ֹ�ص�
				{
					m_WordInfoArray.GetAt(i)->dstRect.X -= m_WordInfoArray.GetAt(i)->speed;
				}
				else
				{
					BOOL stopFlag = FALSE;
					for (int j = 0; j < min(size, MAX_WORD_COUNT); j++)
					{
						if (i != j)
						{
							//if ((m_WordInfoArray.GetAt(j)->dstRect.Y + 50 > m_WordInfoArray.GetAt(i)->dstRect.Y) 
							//	&& (m_WordInfoArray.GetAt(j)->dstRect.Y < m_WordInfoArray.GetAt(i)->dstRect.Y + 50))
							if(m_WordInfoArray.GetAt(i)->dstRect.Y == m_WordInfoArray.GetAt(j)->dstRect.Y)
							{
								lpszWord = m_WordInfoArray.GetAt(j)->lpszWord;
								pGraphics->MeasureString (lpszWord, -1, m_pFont,layoutRect, m_pTextFormat,&boundingBox, 0, 0);
								
								if (m_WordInfoArray.GetAt(i)->dstRect.X > m_WordInfoArray.GetAt(j)->dstRect.X)
								{
									//int ssss = ((m_WordInfoArray.GetAt(i)->dstRect.X) - (m_WordInfoArray.GetAt(i)->speed));
									//int hhh = (m_WordInfoArray.GetAt(j)->dstRect.X + boundingBox.Width);
									if(((m_WordInfoArray.GetAt(i)->dstRect.X) - (m_WordInfoArray.GetAt(i)->speed)) < (m_WordInfoArray.GetAt(j)->dstRect.X + boundingBox.Width))
									{
										stopFlag = TRUE;
										m_WordInfoArray.GetAt(i)->speed = m_WordInfoArray.GetAt(j)->speed;
									}
								}

							}
						}
					}

					if (!stopFlag)
					{
						m_WordInfoArray.GetAt(i)->dstRect.X -= m_WordInfoArray.GetAt(i)->speed;
						stopFlag = FALSE;
					}
				}

				//m_WordInfoArray.GetAt(i)->dstRect.X -= m_WordInfoArray.GetAt(i)->speed;

			}

		}

		for (int j = min(size, MAX_WORD_COUNT) - 1; j >= 0; j--)
		{
			if (m_WordInfoArray.GetAt(j)->DeleteMark)
			{
				LPWordInfo lpWordInfo = m_WordInfoArray.GetAt(j);
				m_WordInfoArray.RemoveAt(j);
				delete lpWordInfo->lpszWord;
				delete lpWordInfo;
				continue;
			}
		}
	}


	int size = m_WordInfoArray.GetCount();
	for (int i = 0; i < min(size, MAX_WORD_COUNT); i++)
	{
		LPWSTR lpszWord = m_WordInfoArray.GetAt(i)->lpszWord;
		RectF dstRect = m_WordInfoArray.GetAt(i)->dstRect;

		//��ȡ�����ֿ�Ⱥ͸߶�
		RectF layoutRect(0,0,0,0);
		RectF boundingBox;
		pGraphics->MeasureString (lpszWord, -1, m_pFont,layoutRect, m_pTextFormat,&boundingBox, 0, 0);
		//�����ʻ�����λ��
		dstRect.Width = boundingBox.Width;
		dstRect.Height = boundingBox.Height;

		if(dstRect.Width>m_nWidth)dstRect.Width=m_nWidth;

		////-----------------------------------------------------------
		////������Ӱ
		//if(m_pShadowBrush){
		//	layoutRect=dstRect;
		//	layoutRect.X=layoutRect.X + m_nShadowOffset;
		//	layoutRect.Y=layoutRect.Y + m_nShadowOffset;
		//	Gdiplus::GraphicsPath* pShadowPath=new Gdiplus::GraphicsPath(FillModeAlternate);//����·��
		//	pShadowPath->AddString (lpszWord, -1, m_pFontFamily, m_FontStyle, m_FontSize, layoutRect, m_pTextFormat); //�����ּ���·��
		//	pGraphics->FillPath (m_pShadowBrush,pShadowPath);//���·��
		//	delete pShadowPath; //����·��
		//}

		//-----------------------------------------------------------
		//�������
		Gdiplus::GraphicsPath* pStringPath=new Gdiplus::GraphicsPath(FillModeAlternate);//����·��
		pStringPath->AddString (lpszWord, -1, m_pFontFamily, m_FontStyle, m_FontSize,dstRect, m_pTextFormat); //�����ּ���·��
		if(m_pTextPen){
			pGraphics->DrawPath (m_pTextPen,pStringPath);//��·��,���ֱ߿�
		}
		Gdiplus::Brush* pBrush = CreateGradientBrush(m_TextGradientMode, m_TextColor1,m_TextColor2,dstRect);
		pGraphics->FillPath (pBrush,pStringPath);//���·��
		delete pBrush;//���ٻ�ˢ
		DrawHighlightLyrics(pGraphics,pStringPath,dstRect);
		delete pStringPath; //����·��
	}

	//LeaveCriticalSection(&g_cs);

}

void CLyricsWindow::Draw2()
{
	CRect rcClient;
	GetClientRect(rcClient);
	m_nWidth=rcClient.Width();
	m_nHeight=rcClient.Height();
	//----------------------------------
	BITMAPINFO bitmapinfo;
	bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapinfo.bmiHeader.biBitCount = 32;
	bitmapinfo.bmiHeader.biHeight = m_nHeight;
	bitmapinfo.bmiHeader.biWidth = m_nWidth;
	bitmapinfo.bmiHeader.biPlanes = 1;
	bitmapinfo.bmiHeader.biCompression=BI_RGB;
	bitmapinfo.bmiHeader.biXPelsPerMeter=0;
	bitmapinfo.bmiHeader.biYPelsPerMeter=0;
	bitmapinfo.bmiHeader.biClrUsed=0;
	bitmapinfo.bmiHeader.biClrImportant=0;
	bitmapinfo.bmiHeader.biSizeImage = bitmapinfo.bmiHeader.biWidth * bitmapinfo.bmiHeader.biHeight * bitmapinfo.bmiHeader.biBitCount / 8;
	HBITMAP hBitmap=CreateDIBSection (m_hCacheDC,&bitmapinfo, 0,NULL, 0, 0);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject (m_hCacheDC,hBitmap);
	//����͸������
	CPoint DestPt(0,0);
	CSize psize(m_nWidth,m_nHeight);
	BLENDFUNCTION blendFunc32bpp;
	blendFunc32bpp.AlphaFormat = AC_SRC_ALPHA;
	blendFunc32bpp.BlendFlags = 0;
	blendFunc32bpp.BlendOp = AC_SRC_OVER;
	blendFunc32bpp.SourceConstantAlpha = 255;
	HDC hDC=::GetDC(m_hWnd);
	::UpdateLayeredWindow(m_hWnd,hDC,NULL,&psize,m_hCacheDC,&DestPt,0,&blendFunc32bpp,ULW_ALPHA);
	//----------------------------------
	//�ͷ���Դ
	::SelectObject (m_hCacheDC,hOldBitmap);
	::DeleteObject(hBitmap);
	::ReleaseDC(m_hWnd,hDC);
}

//���Ƹ������
void CLyricsWindow::DrawHighlightLyrics(Gdiplus::Graphics* pGraphics,Gdiplus::GraphicsPath* pPath,RectF& dstRect)
{
	if(m_nHighlight<=0)return;
	Gdiplus::Region* pRegion=NULL;
	if(m_nHighlight<100){
		RectF CliptRect(dstRect);
		CliptRect.Width=CliptRect.Width * (REAL)m_nHighlight / (REAL)100.0f;
		pRegion=new Gdiplus::Region(CliptRect);
		pGraphics->SetClip(pRegion,CombineModeReplace);
	}
	//--------------------------------------------
	if(m_pHighlightPen){
		pGraphics->DrawPath (m_pHighlightPen,pPath);//��·��,���ֱ߿�
	}
	Gdiplus::Brush* pBrush = CreateGradientBrush(m_HighlightGradientMode, m_HighlightColor1,m_HighlightColor2,dstRect);
	pGraphics->FillPath (pBrush,pPath);//���·��
	delete pBrush;//���ٻ�ˢ
	//--------------------------------------------
	if(pRegion){
		pGraphics->ResetClip();
		delete pRegion;
	}
}
//�������仭ˢ
Gdiplus::Brush* CLyricsWindow::CreateGradientBrush(LyricsGradientMode TextGradientMode,Gdiplus::Color& Color1,Gdiplus::Color& Color2,RectF& dstRect)
{
	PointF pt1;
	PointF pt2;
	Gdiplus::Brush* pBrush=NULL;
	switch (TextGradientMode)
	{
	case LyricsGradientMode_Two://��ɫ����
		{
			PointF point1(dstRect.X,dstRect.Y);
			PointF point2(dstRect.X,dstRect.Y+dstRect.Height);
			pBrush=new Gdiplus::LinearGradientBrush(point1,point2,Color1,Color2);
			((LinearGradientBrush*)pBrush)->SetWrapMode(WrapModeTileFlipXY);
			break;
		}

	case LyricsGradientMode_Three://��ɫ����
		{
			PointF point1(dstRect.X,dstRect.Y);
			PointF point2(dstRect.X,dstRect.Y+dstRect.Height/2);
			pBrush=new Gdiplus::LinearGradientBrush(point1,point2,Color1,Color2);
			((LinearGradientBrush*)pBrush)->SetWrapMode(WrapModeTileFlipXY);
			break;
		}

	default://�޽���
		{
			pBrush=new Gdiplus::SolidBrush(Color1);
			break;
		}
	}
	return pBrush;
}

//���ø����ɫ
void CLyricsWindow::SetLyricsColor(Gdiplus::Color TextColor1)
{
	CLyricsWindow::SetLyricsColor(TextColor1,Gdiplus::Color::Black,LyricsGradientMode_None);
}
void CLyricsWindow::SetLyricsColor(Gdiplus::Color TextColor1,Gdiplus::Color TextColor2,LyricsGradientMode TextGradientMode)
{
	m_TextColor1=TextColor1;
	m_TextColor2=TextColor2;
	m_TextGradientMode=TextGradientMode;

}
//���ø�ʱ߿�
void CLyricsWindow::SetLyricsBorder(Gdiplus::Color BorderColor,REAL BorderWidth)
{
	if(m_pTextPen){
		delete m_pTextPen;
		m_pTextPen=NULL;
	}
	if(BorderColor.GetA()>0 && BorderWidth>0)
		m_pTextPen=new Gdiplus::Pen(BorderColor,BorderWidth);
}
//���ø��������ɫ
void CLyricsWindow::SetHighlightColor(Gdiplus::Color TextColor1)
{
	CLyricsWindow::SetHighlightColor(TextColor1,Gdiplus::Color::Black,LyricsGradientMode_None);
}
void CLyricsWindow::SetHighlightColor(Gdiplus::Color TextColor1,Gdiplus::Color TextColor2,LyricsGradientMode TextGradientMode)
{
	m_HighlightColor1=TextColor1;
	m_HighlightColor2=TextColor2;
	m_HighlightGradientMode=TextGradientMode;

}
//���ø�����ʱ߿�
void CLyricsWindow::SetHighlightBorder(Gdiplus::Color BorderColor,REAL BorderWidth)
{
	if(m_pHighlightPen){
		delete m_pHighlightPen;
		m_pHighlightPen=NULL;
	}
	if(BorderColor.GetA()>0 && BorderWidth>0)
		m_pHighlightPen=new Gdiplus::Pen(BorderColor,BorderWidth);
}
//���ø����Ӱ
void CLyricsWindow::SetLyricsShadow(Gdiplus::Color ShadowColor,int nShadowOffset)
{
	if(m_pShadowBrush){
		delete m_pShadowBrush;
		m_pShadowBrush=NULL;
	}
	if(ShadowColor.GetA()>0 && nShadowOffset>0){
		m_nShadowOffset=nShadowOffset;
		m_pShadowBrush=new Gdiplus::SolidBrush(ShadowColor);
	}else{
		m_nShadowOffset=0;
	}
}
//���ø������
void CLyricsWindow::SetLyricsFont(const WCHAR * familyName,REAL emSize,INT style,Unit unit)
{
	if(m_pFont){
		delete m_pFont;
		m_pFont=NULL;
	}
	FontFamily family(familyName,NULL);
	Status lastResult = family.GetLastStatus();
	if (lastResult != Ok)
	{
		HFONT hFont=(HFONT)GetStockObject(DEFAULT_GUI_FONT);
		LOGFONTW lf;
		ZeroMemory(&lf,sizeof(LOGFONTW));
		GetObjectW(hFont,sizeof(LOGFONTW),&lf);
		FontFamily family2(lf.lfFaceName,NULL);
		m_pFont=new Gdiplus::Font(&family2,emSize,style,unit);
	}else{
		m_pFont=new Gdiplus::Font(&family,emSize,style,unit);
	}
	 //----------------
	//����һЩ��������,����·��ʱҪ�õ�
	m_pFont->GetFamily (m_pFontFamily);
	m_FontSize=m_pFont->GetSize ();
	m_FontStyle=m_pFont->GetStyle ();

}


void CLyricsWindow::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnTimer(nIDEvent);


	if (!m_StopFlag)
	{
		//AddString(L"����");
		//AttachThreadInput(GetWindowThreadProcessId(::GetForegroundWindow(),NULL), GetCurrentThreadId(),TRUE); 
		//HWND hForegroundWnd = ::GetForegroundWindow();
		//if (hForegroundWnd != this->m_hWnd)
		//{
		//	SetForegroundWindow(); 
		//	SetFocus(); 
		//}
		//AttachThreadInput(GetWindowThreadProcessId(::GetForegroundWindow(),NULL), GetCurrentThreadId(),FALSE);
		Draw();
	}
	else
	{
		Draw2();
		KillTimer(1);
	}

}


int CLyricsWindow::GetRndNum(int nMin,int nMax)
{
	int r=nMin + rand() % (nMax-nMin+1);
	return r;
}

void CLyricsWindow::AddWord(LPWordInfo lpWordInfo)
{
	m_WordInfoArray.Add(lpWordInfo);
}

void CLyricsWindow::AddString(LPWSTR lpWstrWord)
{
	if (m_StopFlag)
	{
		//delete lpWstr;
		return;
	}

	int wstrLenth = wcslen(lpWstrWord);
	LPWSTR lpWstr = new WCHAR[wstrLenth+1];
	wcscpy(lpWstr, lpWstrWord);


	LPWordInfo lpWordInfo = new WordInfo; 
	lpWordInfo->lpszWord = lpWstr;

	if (m_goWordModel == LeftTurn)
	{
		lpWordInfo->dstRect.X = 0.0f;
	}
	else if (m_goWordModel == RightTurn)
	{
		lpWordInfo->dstRect.X = m_nWidth;
	}
	//int fontHeight = m_pFont->GetHeight(0.0);
	//lpWordInfo->dstRect.Y = GetRndNum(0, m_nHeight - fontHeight);
	lpWordInfo->row = GetRndNum(0, 24);
	lpWordInfo->dstRect.Y = lpWordInfo->row * m_emSize;
	int wordLenth = wcslen(lpWstr);
	//lpWordInfo->speed = min(max(10 + wordLenth, 10), 30);
	lpWordInfo->speed = max((30 - wordLenth), 10);
	lpWordInfo->DeleteMark = FALSE;

	AddWord(lpWordInfo);

}


void CLyricsWindow::AddString(LPWSTR lpWstr, int pos, int speed)
{
	LPWordInfo lpWordInfo = new WordInfo; 
	lpWordInfo->lpszWord = lpWstr;

	if (m_goWordModel == LeftTurn)
	{
		lpWordInfo->dstRect.X = 0.0f;
	}
	else if (m_goWordModel == RightTurn)
	{
		lpWordInfo->dstRect.X = m_nWidth;
	}
	if (pos > 24 || pos < 0)
	{
		lpWordInfo->row = 10;
	}
	else
	{
		lpWordInfo->row = pos;
	}
	lpWordInfo->dstRect.Y = lpWordInfo->row * m_emSize;
	lpWordInfo->speed = speed;
	lpWordInfo->DeleteMark = FALSE;

	AddWord(lpWordInfo);
}


void CLyricsWindow::GoWords(GoWordModel goModel)
{
	m_goWordModel = goModel;
	m_StopFlag = FALSE;
	m_WordInfoArray.RemoveAll();
	
	SetTimer(1, 100, NULL);
}

void CLyricsWindow::StopWords()
{
	m_StopFlag = TRUE;
}

void CLyricsWindow::OnDestroy()
{
	CWnd::OnDestroy();

	DeleteCriticalSection(&g_cs);//ɾ���ٽ���
}
