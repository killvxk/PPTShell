#pragma once
//
// ListItem.h
// 
#include "../ListBarLayout.h"

class CListBarSelectChapterLayout : public CListBarLayout
{
public:
	CListBarSelectChapterLayout();
	~CListBarSelectChapterLayout();

	virtual void			DoClick(TNotifyUI*		pNotify) ;				//������
	virtual void			DoRClick(TNotifyUI*		pNotify) ;				//�Ҽ����

	bool					OnBtnSelectChapter(void* pObj);
protected:
	//override control
	virtual	void	DoInit();

private:
	CButtonUI*			m_pSelectChapterBtn;
};