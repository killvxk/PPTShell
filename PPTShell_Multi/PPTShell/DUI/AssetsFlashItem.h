#pragma once
//
// ListItem.h
// 
#include "AssetsItem.h"

class CAssetsFlashItem :public CAssetsItem
{
public:
	CAssetsFlashItem();
	virtual ~CAssetsFlashItem();

	virtual	void			DoInit();

	virtual bool			OnDownloadDecodeList( void* pObj )			;	//����json����				

	virtual bool			OnChapterChanged( void* pObj )				;	//�½��л��¼�
protected:

};