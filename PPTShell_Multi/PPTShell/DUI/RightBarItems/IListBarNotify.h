#pragma once
//
// INotify.h
// 
#include "EventCenter/EventDefine.h"

class IListBarNotify
{
public:

	virtual void	DoClick(TNotifyUI*		pNotify) = 0;		//������
	virtual void	DoRClick(TNotifyUI*		pNotify) = 0;		//�Ҽ����
	

	virtual bool	OnControlNotify(void* pNotify)	 = 0;		//Notify�¼�

	virtual	void	SetDoClickCallBack(CDelegateBase& delegate)	= 0;	//��������������¼�
	virtual	void	SetDoRClickCallBack(CDelegateBase& delegate)	= 0;	//��������Ҽ�����¼�
protected:
	CEventSource	m_OnDoClickCallBack;
	CEventSource	m_OnDoRClickCallBack;
};

