#pragma once
//
// ListItem.h
// 
#include "AssetsItem.h"

class CAssetsAllItem :public CAssetsItem
{
public:
	CAssetsAllItem();
	virtual ~CAssetsAllItem();

	virtual	void			DoInit();

	virtual bool			OnDownloadDecodeList( void* pObj )			;	//����json����				

	virtual bool			OnChapterChanged( void* pObj )				;	//�½��л��¼�

	virtual	void			RefreshData();

	void					StartLoadGif();
	void					StopLoadGif();
protected:
	bool					m_bData;
};