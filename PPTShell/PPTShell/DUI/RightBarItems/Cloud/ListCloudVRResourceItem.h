#pragma once
//
// ListItem.h
// 
#include "../ListCloudItem.h"

class CListCloudVRResourceItem :public CListCloudItem
{
public:
	CListCloudVRResourceItem();
	virtual ~CListCloudVRResourceItem();

	virtual	void			DoInit();

	virtual bool			OnDownloadDecodeList( void* pObj )			;	//����json����				

	virtual bool			OnChapterChanged( void* pObj )				;	//�½��л��¼�
protected:

};