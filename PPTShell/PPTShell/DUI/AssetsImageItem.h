#pragma once
//
// ListItem.h
// 
#include "AssetsItem.h"

class CAssetsImageItem :public CAssetsItem
{
public:
	CAssetsImageItem();
	virtual ~CAssetsImageItem();

	virtual	void			DoInit();

	virtual bool			OnDownloadDecodeList( void* pObj )			;	//����json����				

	virtual bool			OnChapterChanged( void* pObj )				;	//�½��л��¼�
protected:

};