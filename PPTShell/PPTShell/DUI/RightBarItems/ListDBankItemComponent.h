#pragma once
//
// ListDBankItemComponent.h
// 
#include "DUI/GifAnimUI.h"

class CListDBankItemComponent 
{

protected:
	CGifAnimUI*			m_pTotalLoadGif;				//��ȡ����gif

	CLabelUI*			m_pTextTotalCount;				//��ǰ�ı�

	CButtonUI*			m_pContentBtn;					//�ı���ť
			
	CHorizontalLayoutUI*m_pCountContainer;
	COptionUI*			m_pContainerIcon;

	int					m_nTotalCount;

	DWORD				m_dwSelectedTextColor;			//ѡ���ı���ɫ
	DWORD				m_dwSelectedCountColor;			//ѡ��������ɫ
	DWORD				m_dwNormalTextColor;			//Ĭ���ı���ɫ
	DWORD				m_dwNormalCountColor;			//Ĭ��������ɫ

	bool				m_bTotalNetLess;				//�����ж�����״̬
};