#pragma once
//
// INotify.h
// 
#include "EventCenter/EventDefine.h"

class IAssetsItemNotify
{
public:

	virtual void	DoClick(TEventUI* pEvent) = 0;				//������
	virtual void	DoRClick(TEventUI* pEvent) = 0;				//�Ҽ����

	virtual bool	OnControlNotify(void* pEvent)	 = 0;		//Notify�¼�
	virtual bool	OnEventLoginComplete( void* pObj )	= 0;	//��¼�ɹ�

	virtual	void	SetDoClickCallBack(CDelegateBase& delegate)	= 0;	//��������������¼�
	virtual	void	SetDoRClickCallBack(CDelegateBase& delegate)	= 0;	//��������Ҽ�����¼�
	virtual	void	SetDoFinishCallBack(CDelegateBase& delegate)	= 0;	//����������ɻص�
	
protected:
	CEventSource	m_OnDoClickCallBack;
	CEventSource	m_OnDoRClickCallBack;
	CEventSource	m_OnFinishCallBack;
};

