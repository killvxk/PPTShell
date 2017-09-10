#pragma once
//
// ListItem.h
// 
#include "../ListBarItem.h"

class CListBarDBankUploadItem : public CListBarItem
{
public:
	CListBarDBankUploadItem();
	~CListBarDBankUploadItem();

	virtual bool			OnChapterChanged( void* pObj )				;	//�½��л��¼�
	virtual bool			OnRefreshGroupExplorer( void* pObj )		;	//GroupExplorerˢ�°�ť�¼�
	virtual bool			OnDownloadDecodeList( void* pObj )			;	//����json����	

	virtual void			DoClick(TNotifyUI*		pNotify) ;				//������

	bool					OnBtnUpload(void * pObj);
protected:
	//override control
	virtual	void			DoInit();

private:
	CButtonUI*				m_pUploadBtn;

};