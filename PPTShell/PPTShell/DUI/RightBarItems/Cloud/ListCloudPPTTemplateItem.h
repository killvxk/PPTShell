#pragma once
//
// ListItem.h
// 
#include "../ListCloudItem.h"

class CListCloudPPTTemplateItem :public CListCloudItem
{
public:
	CListCloudPPTTemplateItem();
	virtual ~CListCloudPPTTemplateItem();

	virtual	void			DoInit();

	virtual bool			OnDownloadDecodeList( void* pObj )			;	//����json����				

	virtual bool			OnChapterChanged( void* pObj )				;	//�½��л��¼�
protected:
	map<tstring,CStream *>		m_mapPPTTemplateStream;
};