#pragma once
//
// ListItem.h
// 
#include "../ListDBankItem.h"

class CListDBankCoursewareObjectsItem :public CListDBankItem
{
public:
	CListDBankCoursewareObjectsItem();
	virtual ~CListDBankCoursewareObjectsItem();

	virtual	void			DoInit();

	virtual bool			OnDownloadDecodeList( void* pObj )			;	//����json����				

	virtual bool			OnEventLoginComplete( void* pObj )		;

	virtual bool			OnGetTotalCount(void * pParam)				;	//��ȡ�����ص�
protected:

};