#pragma once
//
// ListItem.h
// 
#include "../ListBarItem.h"

class CListBarLocalSearchItem : public CListBarItem
{
public:
	CListBarLocalSearchItem();
	~CListBarLocalSearchItem();

	virtual bool			OnChapterChanged( void* pObj )				;	//�½��л��¼�
	virtual bool			OnRefreshGroupExplorer( void* pObj )		;	//GroupExplorerˢ�°�ť�¼�
	virtual bool			OnDownloadDecodeList( void* pObj )			;	//����json����	

	virtual void			DoClick(TNotifyUI*		pNotify) ;				//������
	virtual void			DoRClick(TNotifyUI*		pNotify) ;				//�Ҽ����

protected:
	//override control
	virtual	void			DoInit();

};