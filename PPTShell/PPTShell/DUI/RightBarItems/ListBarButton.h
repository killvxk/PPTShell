#pragma once
//
// ListItem.h
// 
#include "IListBarNotify.h"
#include "ListItemComponent.h"
#include "NDCloud/NDCloudAPI.h"

class CListBarButton : public CButtonUI,
	public IListBarNotify,
	public CListItemComponent

{
public:
	CListBarButton();
	virtual ~CListBarButton();

	virtual void	DoClick(TNotifyUI*		pNotify) ;		//������
	virtual void	DoRClick(TNotifyUI*		pNotify) ;		//�Ҽ����
	

	virtual bool	OnControlNotify(void* pNotify);			//Notify�¼�

	virtual	void	SetDoClickCallBack(CDelegateBase& delegate);	//���õ���ص�
	virtual	void	SetDoRClickCallBack(CDelegateBase& delegate);	//��������Ҽ�����¼�

	int				GetItemType();									//��ȡitem����

protected:
	//override control
	virtual	void	DoInit();
};