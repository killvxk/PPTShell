#pragma once
//
// ListItem.h
// 
#include "../ListCloudItem.h"

class CListCloudCourseItem :public CListCloudItem
{
public:
	CListCloudCourseItem();
	virtual ~CListCloudCourseItem();

	virtual	void			DoInit();

	virtual bool			OnDownloadDecodeList( void* pObj )			;	//����json����				

	virtual bool			OnChapterChanged( void* pObj )				;	//�½��л��¼�
protected:

};