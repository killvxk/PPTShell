#pragma once
//
// ListItem.h
// 
#include "../ListDBankItem.h"

class CListDBankCourseItem :public CListDBankItem
{
public:
	CListDBankCourseItem();
	virtual ~CListDBankCourseItem();

	virtual	void			DoInit();

	virtual bool			OnDownloadDecodeList( void* pObj )			;	//����json����	

	virtual bool			OnEventLoginComplete( void* pObj )		;

protected:

};