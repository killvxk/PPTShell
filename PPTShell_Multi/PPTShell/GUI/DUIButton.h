#pragma once

#include <GdiPlus.h>
using namespace Gdiplus;

enum DRAW_STATUS
{
	DS_NORMAL = 0,
	DS_HOVERING,
	DS_PRESSED,
	DS_DISABLED,

	DS_TOTAL,
};

enum BUTTON_MOUSE_STATUS
{
	BMS_HOVER = 1,
	BMS_MOUSE,
	BMS_LEAVE,
};


#define		BUTTON_MAX_WIDTH	(250)
#define		WINDOW_MIN_WIDTH	(1200)


#define		TEXT_OFFSET			(2)
//#define  BUTTON_MAX_WIDTH	(200)

class CDUIButton
{
public:
	// ��׼���캯��
	CDUIButton();

	virtual ~CDUIButton();

	//�޸İ�ť����
	void SetCaption(CString strCaption);

	//�Ƿ���
	BOOL IsRightOffset()	{return m_bRightOffset;}

	//�Ƿ���Բ���
	BOOL IsRelativeLayout()	{return m_bRelativeLayout;}

	//�õ���Բ���ƫ��
	void GetOffsetRect(LPRECT lpRect){*lpRect = *m_rcOffsetRect;}

	// ���ð�ť����λ��
	void SetRect(LPCRECT lpRect);
	// ��ȡ��ť����λ��
	void GetRect(LPRECT lpRect){*lpRect = *m_rcRect;}
	// ���ָ�����Ƿ��ڰ�ť��
	BOOL PtInButton(CPoint& point){return m_rcRect.PtInRect(point);}
	// ���ư�ť
	virtual int DrawButton( Gdiplus::Graphics&  graphics, CSize offset = CSize(0, 0));
	// ������ť
	void CreateButton(LPCTSTR lpszCaption, LPRECT lpRect, CWnd* pParent, Image* pImage, UINT nCount = 4, UINT nID = 0, BOOL bHorizontal = TRUE, BOOL bRight = FALSE, BOOL bRelativeLayout = FALSE);
	//void CreateButton(LPCTSTR lpszCaption, LPRECT lpRect, CWnd* pParent, Image* pImage, UINT nCount = 4, UINT nID = 0, BOOL bHorizontal = TRUE, BOOL bRight = FALSE, BOOL bRelativeLayout = FALSE);
	//���ð�ť״̬ͼƬ
	void  SetBKImage(Image *pImage, UINT nCount = 4, BOOL bHorizontal = TRUE, BOOL bRedraw = FALSE);
	
	//���ð�ťͼ����Ϣ
	void  SetIconInfo(Image *pIcon, UINT nCount = 1, CPoint ptPos = CPoint(5,-1), BOOL bHorizontal = TRUE, BOOL bRedraw = FALSE);
	void  SetIconInfo(Image *pIcon, UINT nCount, CSize cSize, CPoint ptPos = CPoint(5,-1), BOOL bHorizontal = TRUE, BOOL bRedraw = FALSE);
	void  SetStatusIconInfo(Image *pIcon, UINT nStatus, CSize cSize, CPoint ptPos = CPoint(5,-1));
	
	//����ͼ���Ƿ���ʾ
	void  SetShowIcon( BOOL bShow );

	//������ʾͼ����Ϣ
	void  SetTipIconInfo(Image *pIcon, UINT nCount = 1, CPoint ptPos = CPoint(5,-1), BOOL bHorizontal = TRUE, BOOL bRedraw = FALSE);
	void  SetTipIconInfo(Image *pIcon, UINT nCount, CSize cSize, CPoint ptPos = CPoint(5,-1), BOOL bHorizontal = TRUE, BOOL bRedraw = FALSE);
	void  SetStatusTipIconInfo(Image *pIcon, UINT nStatus, CSize cSize, CPoint ptPos = CPoint(5,-1));

	void  SetStatusTextInfo(UINT nStatus, DWORD dwColor);

	void  SetAutoSize(BOOL bAutoSize)	{	m_bAutoSize = bAutoSize;	};
	void  SetMaxWidth(int nWidth = BUTTON_MAX_WIDTH )	{	m_nMaxWidth = nWidth;	};

	//����Ϊ�Ź����и�
	void  SetDivison(BOOL bDivison)		{	m_bDivision = bDivison;		};
	void  SetDivsionRect(CRect rect);
	// ���ð�ť״̬
	void EnableButton(BOOL bEnable)
	{
		m_bDisabled = !bEnable;
		m_bHovering = FALSE;
		m_bPressed  = FALSE;
		m_pParent->InvalidateRect(m_rcRect);
	}

	void SetCheckBox(BOOL bCheck = TRUE)
	{
		m_bIsCheckBox = bCheck;
	}

	BOOL IsChecked()	{	return m_bCheck;	}

	// ���ð�ťѡ��״̬
	void SetCheck(BOOL bCheck)
	{
		m_bCheck = bCheck;
		m_pParent->InvalidateRect(m_rcRect);
	}
	// ��ȡ��ťId
	UINT GetControlId(){return m_nID;};
	// ������ť�¼�
	void DoCommand()
	{
		if(!m_bDisabled)
		{
			m_bCheck = !m_bCheck;
			m_pParent->InvalidateRect(m_rcRect);
			m_pParent->PostMessage(WM_COMMAND, m_nID);
		}
	}

	// ��갴�°�ť�¼�
	void LButtonDown()
	{
		if(!m_bDisabled)
		{
			m_bPressed = TRUE;
			m_pParent->InvalidateRect(m_rcRect);
		}
	};
	// �����°�ť�¼�
	void LButtonUp()
	{
		if(!m_bDisabled)
		{
			if(m_bPressed == TRUE)
			{
				m_bCheck = !m_bCheck;
				m_bPressed = FALSE;
				m_pParent->InvalidateRect(m_rcRect);
				m_pParent->PostMessage(WM_COMMAND, m_nID);
			}
		}
	}
	// �����밴ť�����¼�
	void MouseHover()
	{
		if( !m_bDisabled )
		{
			m_bHovering = TRUE;
			m_pParent->InvalidateRect(m_rcRect);
			ShowToolTip();
			m_pParent->PostMessage(WM_TITLE_MOUSE, m_nID, BMS_HOVER);
		}
	}

	// ��갴ť�����ƶ��¼�
	void MouseMouse()
	{
		if( !m_bDisabled && m_bHovering )
		{
			MoveToolTip();
			m_pParent->PostMessage(WM_TITLE_MOUSE, m_nID, BMS_MOUSE);
		}
	}

	// ����뿪��ť�����¼�
	void MouseLeave()
	{
		if( !m_bDisabled )
		{
			m_bPressed = FALSE;
			m_bHovering = FALSE;
			m_pParent->InvalidateRect(m_rcRect);
		}
		HideToolTip();
		m_pParent->PostMessage(WM_TITLE_MOUSE, m_nID, BMS_LEAVE);
	}

	// ����������ʾ����
	void SetTipText(LPCTSTR lpstrTipText)
	{
		m_strTipText = lpstrTipText;
	}

	CWnd* GetParentWnd() { return m_pParent;}

protected:
	//
	void ShowToolTip();
	void MoveToolTip();
	void HideToolTip();
	//����ɫ�汳��
	void DrawColor(Gdiplus::Graphics&  graphics, const RECT& rc, DWORD color);
	// ��ť����λ��
	CRect	m_rcRect;
	// ��ťƫ�ƾ���λ��
	CRect	m_rcOffsetRect;
	//�ĸ��Ƿ�Χ
	CRect	m_rcCornerRect;
	// ��ʶ�����밴ť����
	BOOL	m_bHovering;
	// ��ʶ�Ѿ������ť
	BOOL	m_bPressed;
	// ��ʶ��ť�Ѿ�������
	BOOL	m_bDisabled;
	// ��ʶ��ť�Ѿ���ѡ��
	BOOL	m_bCheck;
	// ��ʶ�Ƿ���CheckBox
	BOOL	m_bIsCheckBox;
	// ��ʶ��ť�Ƿ����Ҽ���ƫ��
	BOOL	m_bRightOffset;
	// ��Բ���
	BOOL	m_bRelativeLayout;

	BOOL	m_bAutoSize;

	//��ť��9����ָ�
	BOOL	m_bDivision;

	// ��ťͼƬ
	Image*	m_pImage;
	//״̬ͼƬ��ť����
	Image*	m_pDrawImage[4];
	RectF	m_rtImage[4];
	//״̬������ɫ����
	DWORD	m_dwColor[4];

	// ��ťͼƬ�а�����ͼƬ����
	UINT	m_nImageCount;

//============================================
//				Сͼ��
//============================================
//ͼ��(һ����ť���Ը�������һ��Сͼ��)
	Image*	m_pIcon;
	Image*	m_pDrawIcon[4];
	RectF	m_rtIcon[4];

	// ��ťͼƬ�а�����ͼƬ����
	UINT	m_nIconCount;
	//ͼ�����Ͻ�λ��
	CPoint	m_ptIcon;
	//ͼ��Ĵ�С
	CSize	m_csizeIcon;

	BOOL	m_bShowIcon;

//============================================
//				��ʾͼ��
//============================================
	Image*	m_pTipIcon;
	Image*	m_pDrawTipIcon[4];
	RectF	m_rtTipIcon[4];

	// ��ťͼƬ�а�������ʾͼ�����
	UINT	m_nTipIconCount;
	//ͼ�����Ͻ�λ��
	CPoint m_ptTipIcon;
	//ͼ��Ĵ�С
	CSize  m_csizeTipIcon;

	// ��ťId��
	int		m_nID;
	// ��ť������
	CWnd*	m_pParent;
	//��ť�����ı�
	CString m_strCaption;

// ��ʾ���
	
	TOOLINFO	m_sToolTip;
	HWND		m_hWndToolTip;
	CString		m_strTipText;
	POINT		m_ToolLastPos;


	//��ť���
	int		m_nMaxWidth;
	int		m_nLeftMaxLen;//2015.11.11 cws


		
	//Gdiplus::Graphics  m_graphics;
	//�ı���ɫ
	//Color m_colorNormal;
	//Color m_colorDisable;
	//Color m_colorPressed;
	//Color m_colorHovering;
};
