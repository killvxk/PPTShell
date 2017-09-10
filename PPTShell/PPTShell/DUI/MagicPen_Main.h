#pragma once
#include "stdafx.h"
#include "DUICommon.h"   

#define MAGICPEN_TIMER_ID 90501
#define WM_SHOWMAGICPEN                      0x0218
#define WM_HIDEMAGICPEN                      0x0219
#define WM_MOVEMAGICPEN						 0x021A
class CMagicPen_Main : public WindowImplBase
{
public:
	CMagicPen_Main(void);
	~CMagicPen_Main(void);

	UIBEGIN_MSG_MAP
	UIEND_MSG_MAP
 
enum DrawState
	{
		DRAWRECT_START,
		DRAWRECT_DRAWING,
		DRAWRECT_END,		
	};
public:
	LPCTSTR GetWindowClassName() const;	
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();

 
	virtual void InitWindow();
	//virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 

public:
	//�ṩ���ֻ�����2016.03.08	
	void	DrawRoundRect(int nState, int nX, int nY);//���ƾ��ο� nState =0,��ʼ���ƣ�nState =1���ƹ���, nState = 2���ƽ���
  
	void	ShowLocalWindows();//��ʾ
	void	CloseLocalWindows();//����
	void	SetControlMode(int nMode = 0) { m_nControlMode = nMode;}
	int		GetControlMode(){ return m_nControlMode; }
	bool	IsShowing() {return m_bIsShowing;}
	bool	IsMagnifierShowing() { return IsWindow(m_hMagicPenAltha); }	// �Ŵ���ͼƬ�����Ƿ�����ʾ
	void	TopMostView();//��ʾ
private:  
	bool IsMagicPenMode(){ return m_bMagicPenMode; }   
	bool OnCloseWndEvent(void* pObj);
	void StartDrawRoundRect(int nX, int nY);
	void EndDrawRoundRect(int nX, int nY);
	void DrawingRoundRect(int nX, int nY);
	bool m_bMagicPenMode;  
	CBitmap* m_pbmpScreen;
	CBitmap* m_pbmpCopyScreen;

	CBitmap* m_pbmpToolTip;
	CPoint m_ptStart;
	CPoint m_ptEnd;
	int m_nCx;
	int m_nCy;
	bool m_bHaveShow;//�Ѿ���ʾ�˷Ŵ� 
	HWND m_hMagicPenAltha;//͸��ͼ��
	DWORD m_nStartTimeTick;
	int m_nControlMode;		// ������Դ��0-�ֻ����ƣ�1-PC�˿���;2-��������
	bool  m_bIsShowing;
protected:  
	HWND					m_hWndParent; 
	CVerticalLayoutUI*		m_pRoundRect;
	CLabelUI*				m_pShowToolTip;
};

 
typedef Singleton<CMagicPen_Main>		MagicPen_MainUI;