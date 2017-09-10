#pragma once
//
// ListItem.h
// 
#include "../ListBarItem.h"

class CListBarLocalImportItem : public CListBarItem
{
public:
	CListBarLocalImportItem();
	~CListBarLocalImportItem();

	virtual bool			OnChapterChanged( void* pObj )				;	//�½��л��¼�
	virtual bool			OnRefreshGroupExplorer( void* pObj )		;	//GroupExplorerˢ�°�ť�¼�
	virtual bool			OnDownloadDecodeList( void* pObj )			;	//����json����	

	virtual void			DoClick(TNotifyUI*		pNotify) ;				//������

protected:
	//override control
	virtual	void			DoInit();

	bool					OnBtnImport( void* pObj);

private:
	CButtonUI*				m_pImportBtn;

};