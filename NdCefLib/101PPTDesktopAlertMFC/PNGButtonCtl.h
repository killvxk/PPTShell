#ifndef __PNG_BUTTON_CTL__
#define __PNG_BUTTON_CTL__

#include "afxwin.h"


class PNGButton : public CWnd
{
public:
	//enum BUTTON_STATE
	//{
	//	BUTTON_ENABLE = 0,
	//	BUTTON_HOVER = 1,
	//	BUTTON_CLICK = 2,
	//	BUTTON_DISABLE= 3 
	//};//��ť״̬

	PNGButton(void);
	DECLARE_DYNCREATE(PNGButton)   
	~PNGButton(void);
/*	virtual BOOL Create(UINT x,UINT y, CWnd* pParentWnd, UINT nID,
		UINT PngID,Gdiplus::Image* m_hoverBg, CCreateContext* pContext = NULL); */ 

	//PngID �ؼ���������ʾ��ͼƬ  PngID_hOver�����ͣʱ��ʾ��ͼƬ  PngID_hDisable�ؼ���ʹ��״̬��ʾ��ͼƬ
	//PngID_hClick�������ʱ��ʾ��ͼƬ��ѡ�����ã�Ҳ�ɲ���
	virtual BOOL Create(UINT x,UINT y, CWnd* pParentWnd, UINT nID,
		UINT PngID,UINT PngID_hOver, UINT PngID_hDisable, UINT PngID_hClick = 0, CCreateContext* pContext = NULL);  
protected:
	DECLARE_MESSAGE_MAP()  

	Gdiplus::Image* m_bg;
	Gdiplus::Image* m_hoverBg;
	Gdiplus::Image* m_clickBg;
	Gdiplus::Image* m_DisableBg;
	bool m_bIsMouseOver;
	bool m_bIsTracked;
	bool m_bIsMouseHover;
	bool m_bIsChecked;
	bool m_bIsEnable;
	UINT m_Width;
	UINT m_Height;

public:
	afx_msg void OnPaint();
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnClick(); 
	void PaintParent();
	void EnableWindow(bool enable);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
private:
	//CToolTipCtrl*	m_pToolTip;
};

#endif
