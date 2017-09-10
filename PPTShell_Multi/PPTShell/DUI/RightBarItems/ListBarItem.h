#pragma once
//
// ListItem.h
// 
#include "IListBarNotify.h"
#include "IListItemHandler.h"
#include "ListItemComponent.h"
#include "NDCloud/NDCloudAPI.h"

class CListBarItem : public CListContainerElementUI,
				public IListBarNotify,
				public IListItemHandler,
				public CListItemComponent

{
public:
	CListBarItem();
	virtual ~CListBarItem();


	void			SetJsonUrl(tstring strJsonUrl);					//����json��ȡ��ַ
	tstring			GetJsonUrl();

	int				GetItemType();									//��ȡitem����

	virtual void	DoClick(TNotifyUI*		pNotify) ;				//������
	virtual void	DoRClick(TNotifyUI*		pNotify) ;				//�Ҽ����

	virtual bool	OnControlNotify(void* pNotify);					//Notify�¼�
	
	virtual	void	DoInit();

	virtual	void	SetDoClickCallBack(CDelegateBase& delegate);	//���õ���ص�
	virtual	void	SetDoRClickCallBack(CDelegateBase& delegate);	//��������Ҽ�����¼�

	void			SetParentListPtr(void * pControl);
	void*			GetParentListPtr();
protected:
	DWORD			m_dwDownloadId;
	CStream*		m_pStream;

private:
	
					
	tstring			m_strJsonUrl;
	
	void*			m_pParentListPtr;		//ListUI
};