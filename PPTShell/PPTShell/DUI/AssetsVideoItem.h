#pragma once
//
// ListItem.h
// 
#include "AssetsItem.h"

class CAssetsVideoItem :public CAssetsItem
{
public:
	CAssetsVideoItem();
	virtual ~CAssetsVideoItem();

	virtual	void			DoInit();

	virtual bool			OnDownloadDecodeList( void* pObj )			;	//����json����				

	virtual bool			OnChapterChanged( void* pObj )				;	//�½��л��¼�
protected:

};