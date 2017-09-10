#pragma once
//
// ListItem.h
// 
#include "AssetsItem.h"

class CDBankAssetsAllItem :public CAssetsItem
{
public:
	CDBankAssetsAllItem();
	virtual ~CDBankAssetsAllItem();

	virtual	void			DoInit();

	virtual bool			OnDownloadDecodeList( void* pObj )			;	//����json����				

	virtual bool			OnChapterChanged( void* pObj )				;	//�½��л��¼�

	virtual	void			RefreshData();

	void					StartLoadGif();
	void					StopLoadGif();
protected:
	
};