#pragma once
#include "DUI/GifAnimUI.h"

class CAssetsLayoutComponent
{
protected:
	CGifAnimUI*					m_pLoadingGif;							//��ȡ��ǰgif

	CTileLayoutUI*				m_pContentLayout;			//�����б�
	CVerticalLayoutUI*			m_pLoadingLayout;			//loading����
	CVerticalLayoutUI*			m_pNetlessLayout;			//����ʧ�ܽ���
	CContainerUI*				m_pContainerLayout;

};