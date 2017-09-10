#pragma once
#include "DUICommon.h"

class CSliderTabLayoutUI : public CTabLayoutUI
{
	public:
	CSliderTabLayoutUI();
 
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	void DoEvent(TEventUI& event);
	void OnTimer(int nTimerID );

	bool SelectItem( int iIndex , bool bAnim = true , bool bHorizontalAnim = true);

	void OnSliderStep();

	void SetAttribute(LPCTSTR pstrName,LPCTSTR pstrValue);

protected:
	int         m_iCurFrame;
	bool        m_bIsVertical;
	int         m_nPositiveDirection;

	RECT        m_rcCurPos;
	RECT        m_rcNextPos;

	CControlUI* m_pCurPage;            // ���浱ǰ��ʾ��ҳ��
	CControlUI* m_pNextPage;        // ������һҳ��
	RECT		m_rc; 

	enum
	{
		TIMER_ANIMATION_ID = 1,
		ANIMATION_ELLAPSE = 2,
		ANIMATION_FRAME_COUNT = 30
	};
};
// 
// class UILIB_API CSliderTabLayoutUI2 : public CTabLayoutUI
// {
// 	public:
// 	CSliderTabLayoutUI2();
// 	
// 	LPCTSTR GetClass() const;
// 	LPVOID GetInterface(LPCTSTR pstrName);
// 	void DoEvent(TEventUI& event);
// 	void OnTimer(int nTimerID );
// 
// 	bool SelectItem(int iIndex );
// 
// 	void OnSliderStep();
// 
// 	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
//  
// protected:
// 	int         m_iCurFrame;
// 	bool        m_bIsVertical;
// 	int         m_nPositiveDirection;
// 
// 	RECT        m_rcCurPos;
// 	RECT        m_rcNextPos;
// 	RECT        m_SldItem;
// 	
// 	CControlUI* m_pCurPage;            // ���浱ǰ��ʾ��ҳ��
// 	CControlUI* m_pNextPage;            // ������һҳ��
// 	CHorizontalLayoutUI* m_pTabSlider;
// 	CVerticalLayoutUI* m_pVTabSlider;
// 
// 	enum
// 	{
// 		TIMER_ANIMATION_ID = 1,
// 		ANIMATION_ELLAPSE = 10,
// 		ANIMATION_FRAME_COUNT = 30
// 	};
// };