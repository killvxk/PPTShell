#pragma once
//
// ListItem.h
// 
#include "../ListBarItem.h"

class CListBarCloudSearchItem : public CListBarItem
{
public:
	CListBarCloudSearchItem();
	~CListBarCloudSearchItem();

	virtual bool			OnChapterChanged( void* pObj )				;	//�½��л��¼�
	virtual bool			OnRefreshGroupExplorer( void* pObj )		;	//GroupExplorerˢ�°�ť�¼�
	virtual bool			OnDownloadDecodeList( void* pObj )			;	//����json����	

	virtual void			DoClick(TNotifyUI*		pNotify) ;				//������
	virtual void			DoRClick(TNotifyUI*		pNotify) ;				//�Ҽ����

	bool					Select(bool bSelect = true);	
protected:
	//override control
	virtual	void			DoInit();

	COptionUI*				m_pContainerIcon;
	CButtonUI*				m_pContentBtn;					//�ı���ť
};