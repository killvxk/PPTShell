#pragma once
//
// ListItem.h
// 
#include "AssetsItem.h"

class CDBankAssetsImageItem :public CAssetsItem
{
public:
	CDBankAssetsImageItem();
	virtual ~CDBankAssetsImageItem();

	virtual	void			DoInit();

	virtual bool			OnDownloadDecodeList( void* pObj )			;	//����json����				

	virtual bool			OnChapterChanged( void* pObj )				;	//�½��л��¼�

	virtual bool			OnEventLoginComplete( void* pObj )			;	//��¼�ɹ�
protected:

};