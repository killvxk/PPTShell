#pragma once
//
// ListCloudItemComponent.h
// 
#include "DUI/GifAnimUI.h"

class CListCloudItemComponent 
{

protected:
	CGifAnimUI*			m_pTotalLoadGif;				//��ȡ����gif
	CGifAnimUI*			m_pCurLoadGif;					//��ȡ��ǰgif

	CLabelUI*			m_pTextCurCount;				//��ǰ�����ı�
	CLabelUI*			m_pTextSeparator;				//�ָ���
	CLabelUI*			m_pTextTotalCount;				//ȫ�������ı�

	CButtonUI*			m_pContentBtn;					//�ı���ť

	CHorizontalLayoutUI*m_pCountContainer;

	COptionUI*			m_pContainerIcon;

	DWORD				m_dwSelectedTextColor;			//ѡ���ı���ɫ
	DWORD				m_dwSelectedCountColor;			//ѡ��������ɫ
	DWORD				m_dwNormalTextColor;			//Ĭ���ı���ɫ
	DWORD				m_dwNormalCountColor;			//Ĭ��������ɫ

	int					m_nCurCount;					
	int					m_nTotalCount;

	bool				m_bCurNetLess;					//�����ж�����״̬
	bool				m_bTotalNetLess;				//�����ж�����״̬
};