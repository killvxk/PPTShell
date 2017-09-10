#pragma once
//
// ListItem.h
// 
#include "ListBarItem.h"
#include "ListCloudItemComponent.h"

class CListLocalItem :public CListBarItem,
	public CListCloudItemComponent
{
public:
	CListLocalItem();
	virtual ~CListLocalItem();

	virtual	void			DoInit();

	virtual bool			OnChapterChanged( void* pObj )				;	//�½��л��¼�
	virtual bool			OnRefreshGroupExplorer( void* pObj )		;	//GroupExplorerˢ�°�ť�¼�

	virtual void			DoClick(TNotifyUI*		pNotify) ;				//������
	virtual void			DoRClick(TNotifyUI*		pNotify) ;				//�Ҽ����
	virtual void			DoSelect(TNotifyUI*		pNotify) ;				//���ѡ��
				
	virtual void			SetFileCount(int nCount);

	virtual tstring			GetFolderName();
	virtual int				GetFileCount();

	bool					Select(bool bSelect = true);	
private:
	void					CalcText( HDC hdc, RECT& rc, LPCTSTR lpszText, int nFontId, UINT format, UITYPE_FONT nFontType, int c = -1);

protected:
	tstring					m_strFolderName;
	int						m_nCount;

	tstring					m_strChapterGUID;
	tstring					m_strChapter;

};