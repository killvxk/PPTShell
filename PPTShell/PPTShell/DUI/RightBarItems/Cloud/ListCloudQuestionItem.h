#pragma once
//
// ListItem.h
// 
#include "../ListCloudItem.h"

class CListCloudQuestionItem :public CListCloudItem
{
public:
	CListCloudQuestionItem();
	virtual ~CListCloudQuestionItem();

	virtual	void			DoInit();

	virtual bool			OnDownloadDecodeList( void* pObj )			;	//����json����				

	virtual bool			OnChapterChanged( void* pObj )				;	//�½��л��¼�
protected:

};