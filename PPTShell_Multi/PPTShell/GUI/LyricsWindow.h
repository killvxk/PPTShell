// ========================================================
// �ļ�:LyricsWindow.h
// ����:CLyricsWindow
// ����:�����ʾ����
// ����:ʹ��GDIPlus���Ƹ��
// ========================================================
// �ӿ�˵��
// Create				������ʴ��� 
// UpdateLyrics			���¸��(����ı�,�������Ȱٷֱ�)
// Draw					�ػ���ʴ���
// SetLyricsColor		���ø����ɫ
// SetLyricsBorder		���ø�ʱ߿�
// SetHighlightColor	���ø��������ɫ
// SetHighlightBorder	���ø�����ʱ߿�
// SetLyricsShadow		���ø����Ӱ
// SetLyricsFont		���ø������
// ========================================================

#pragma once

#include <gdiplus.h>
#pragma comment(lib,"GdiPlus.lib")
using namespace Gdiplus; 

// CWordWindow
typedef struct tagWordInfo
{
	LPWSTR lpszWord;	//��ʾ����
	int	row;			//��ʾ����
	RectF dstRect;		//��ʾλ��
	int	speed;			//�ƶ��ٶ�
	BOOL DeleteMark;	//�Ƴ���־
	BOOL isFirstTime;	//�Ƿ�ʱ����һ������
} WordInfo, *PWordInfo,FAR *LPWordInfo;

enum GoWordModel
{
	LeftTurn = 0,
	RightTurn = 1
};

//��ʽ���ģʽ
enum LyricsGradientMode
{
	LyricsGradientMode_None		=0, //�޽���
	LyricsGradientMode_Two		=1, //��ɫ����
	LyricsGradientMode_Three	=2 //��ɫ����
};

// CLyricsWindow

class CLyricsWindow : public CWnd
{
	DECLARE_DYNAMIC(CLyricsWindow)

public:
	CLyricsWindow();
	virtual ~CLyricsWindow();
public:
	////�������� 
	BOOL Create(HWND hParentWnd = NULL);
public:
	//���¸��(����ı�,�������Ȱٷֱ�)
	void UpdateLyrics(LPCSTR lpszLyrics,int nHighlight);
	void UpdateLyrics(LPCWSTR lpszLyrics,int nHighlight);
	//���¸�������(�������Ȱٷֱ�)
	void UpdateLyrics(int nHighlight);
	//�ػ���ʴ���
	void Draw();
	//���ø����ɫ
	void SetLyricsColor(Gdiplus::Color TextColor1);
	void SetLyricsColor(Gdiplus::Color TextColor1,Gdiplus::Color TextColor2,LyricsGradientMode TextGradientMode);
	//���ø�ʱ߿�
	void SetLyricsBorder(Gdiplus::Color BorderColor,REAL BorderWidth = 1.0f);
	//���ø��������ɫ
	void SetHighlightColor(Gdiplus::Color TextColor1);
	void SetHighlightColor(Gdiplus::Color TextColor1,Gdiplus::Color TextColor2,LyricsGradientMode TextGradientMode);
	//���ø�����ʱ߿�
	void SetHighlightBorder(Gdiplus::Color BorderColor,REAL BorderWidth = 1.0f);
	//���ø����Ӱ
	void SetLyricsShadow(Gdiplus::Color ShadowColor,int nShadowOffset=2);
	//���ø������
	void SetLyricsFont(const WCHAR * familyName,REAL emSize,INT style=FontStyleRegular,Unit unit=UnitPixel);
protected:
	//���Ƹ��
	void DrawLyrics(Gdiplus::Graphics* pGraphics);
	//���Ƹ������
	void DrawHighlightLyrics(Gdiplus::Graphics* pGraphics,Gdiplus::GraphicsPath* pPath,RectF& dstRect);
	//�������仭ˢ
	Gdiplus::Brush* CreateGradientBrush(LyricsGradientMode TextGradientMode,Gdiplus::Color& Color1,Gdiplus::Color& Color2,RectF& dstRect);
	//ע�ᴰ����
	BOOL RegisterWndClass(LPCTSTR lpszClassName);
protected:
	DECLARE_MESSAGE_MAP()
private:
	HDC m_hCacheDC;//����DC
	int m_nWidth;
	int m_nHeight;
	LPWSTR m_lpszLyrics;//Unicode��ʽ�ĸ��
	int m_nHighlight;//������ʵİٷֱ� 0--100
	Gdiplus::Color m_TextColor1;//��ͨ�����ɫ,ARGB��ɫ
	Gdiplus::Color m_TextColor2;//��ͨ�����ɫ,ARGB��ɫ
	LyricsGradientMode m_TextGradientMode;//��ͨ��ʽ���ģʽ
	Gdiplus::Pen* m_pTextPen;//��ͨ��ʱ߿򻭱�
	Gdiplus::Color m_HighlightColor1;//���������ɫ,ARGB��ɫ
	Gdiplus::Color m_HighlightColor2;//���������ɫ,ARGB��ɫ
	LyricsGradientMode m_HighlightGradientMode;//������ʽ���ģʽ
	Gdiplus::Pen*m_pHighlightPen;//������ʱ߿򻭱�
	Gdiplus::Brush* m_pShadowBrush;//��Ӱ��ˢ,GDIPlus��ˢ 
	int m_nShadowOffset;//��Ӱƫ��
	int m_FontStyle;
	REAL m_FontSize;
	Gdiplus::FontFamily* m_pFontFamily;
	Gdiplus::StringFormat* m_pTextFormat;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	// �麯�� ���ƴ���
	Gdiplus::Font* m_pFont;//GDIPlus����

	CArray<LPWordInfo, LPWordInfo> m_WordInfoArray;	//��ŵ�Ļ���������

	void AddWord(LPWordInfo lpWordInfo);
	void AddString(LPWSTR lpWstr);	  //��ӵ�Ļ����
	void AddString(LPWSTR lpWstr, int pos, int speed);
	int GetRndNum(int nMin,int nMax); //��ȡһ����Χ�ڵ������
	void GoWords(GoWordModel goWordModel);	//���ŵ�Ļ����
	void StopWords();						//ֹͣ���ŵ�Ļ����
	void Draw2();

	GoWordModel m_goWordModel;	//��Ļ����
	int m_emSize;				//�����С
	BOOL m_StopFlag;			//ֹͣ��־
	

	CRITICAL_SECTION g_cs;


};

