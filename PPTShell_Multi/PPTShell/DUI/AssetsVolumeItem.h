#pragma once
//
// ListItem.h
// 
#include "AssetsItem.h"

class CAssetsVolumeItem :public CAssetsItem
{
public:
	CAssetsVolumeItem();
	virtual ~CAssetsVolumeItem();

	virtual	void			DoInit();

	virtual bool			OnDownloadDecodeList( void* pObj )			;	//����json����				

	virtual bool			OnChapterChanged( void* pObj )				;	//�½��л��¼�
protected:

};