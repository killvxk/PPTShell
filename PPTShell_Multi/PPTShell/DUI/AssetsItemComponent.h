#pragma once
#include "AssetsLayout.h"

class CAssetsItemComponent
{
protected:
	int				m_nType;								//Item����		
	tstring			m_strName;								//Item����

	CGifAnimUI*		m_pCurLoadGif;							//��ȡ��ǰgif

	CLabelUI*		m_pTextLabel;
	CLabelUI*		m_pCountLabel;
	CGifAnimUI*		m_pGif;

	CAssetsLayout*	m_pLayout;								//PList

	int				m_nCurCount;					

	bool			m_bCurNetLess;							//�����ж�����״̬

};