#pragma once
//
// ListItem.h
// 
#include "../ListCloudItem.h"

class CListCloud3DResourceItem :public CListCloudItem
{
public:
	CListCloud3DResourceItem();
	virtual ~CListCloud3DResourceItem();

	virtual	void			DoInit();

	virtual bool			OnDownloadDecodeList( void* pObj )			;	//����json����				

	virtual bool			OnChapterChanged( void* pObj )				;	//�½��л��¼�
protected:

};