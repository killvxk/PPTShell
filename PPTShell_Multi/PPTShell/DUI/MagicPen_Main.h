#pragma once
#include "stdafx.h"
#include "DUICommon.h"   

#define MAGICPEN_TIMER_ID 90501
class CMagicPen_Main : public WindowImplBase
{
public:
	CMagicPen_Main(void);
	~CMagicPen_Main(void);

	UIBEGIN_MSG_MAP
	UIEND_MSG_MAP
 

public:
	LPCTSTR GetWindowClassName() const;	
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();

 
	virtual void InitWindow();
	//virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
  
	void	ShowLocalWindows();//��ʾ
private:  
	bool IsMagicPenMode(){ return m_bMagicPenMode; }   
	bool OnCloseWndEvent(void* pObj);
	bool m_bMagicPenMode;  
	CBitmap* m_pbmpScreen;
	CBitmap* m_pbmpCopyScreen;

	CBitmap* m_pbmpToolTip;
	CPoint m_ptStart;
	CPoint m_ptEnd;
	int m_nCx;
	int m_nCy;
	BOOL m_bHaveShow;//�Ѿ���ʾ�˷Ŵ� 
	HWND m_hMagicPenAltha;//͸��ͼ��
	DWORD m_nStartTimeTick;
protected:  
	HWND					m_hWndParent; 
	CVerticalLayoutUI*		m_pRoundRect;
	CLabelUI*				m_pShowToolTip;
};

 
