#pragma once
//
// ListItem.h
// 
#include "../ListCloudItem.h"

class CListCloudCoursewareObjectsItem :public CListCloudItem
{
public:
	CListCloudCoursewareObjectsItem();
	virtual ~CListCloudCoursewareObjectsItem();

	virtual	void			DoInit();

	virtual bool			OnDownloadDecodeList( void* pObj )			;	//����json����				

	virtual bool			OnChapterChanged( void* pObj )				;	//�½��л��¼�
protected:

};