#pragma once
//
// ListItem.h
// 
#include "../ListDBankItem.h"

class CListDBankQuestionItem :public CListDBankItem
{
public:
	CListDBankQuestionItem();
	virtual ~CListDBankQuestionItem();

	virtual	void			DoInit();

	virtual bool			OnDownloadDecodeList( void* pObj )			;	//����json����				

	virtual bool			OnEventLoginComplete( void* pObj )		;

	virtual bool			OnGetTotalCount(void * pParam)				;	//��ȡ�����ص�
protected:

};