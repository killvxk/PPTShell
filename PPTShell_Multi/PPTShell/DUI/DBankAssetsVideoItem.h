#pragma once
//
// ListItem.h
// 
#include "AssetsItem.h"

class CDBankAssetsVideoItem :public CAssetsItem
{
public:
	CDBankAssetsVideoItem();
	virtual ~CDBankAssetsVideoItem();

	virtual	void			DoInit();

	virtual bool			OnDownloadDecodeList( void* pObj )			;	//����json����				

	virtual bool			OnChapterChanged( void* pObj )				;	//�½��л��¼�

	virtual bool			OnEventLoginComplete( void* pObj )			;	//��¼�ɹ�
protected:

};