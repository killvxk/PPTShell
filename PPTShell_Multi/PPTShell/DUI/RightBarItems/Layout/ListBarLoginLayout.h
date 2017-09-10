#pragma once
//
// ListItem.h
// 
#include "../ListBarLayout.h"

class CListBarLoginLayout : public CListBarLayout
{
public:
	CListBarLoginLayout();
	~CListBarLoginLayout();

	virtual void			DoClick(TNotifyUI*		pNotify) ;				//������
	virtual void			DoRClick(TNotifyUI*		pNotify) ;				//�Ҽ����

	bool					OnBtnLogin(void* pObj);
	bool					OnBtnRegister(void* pObj);

	void					OnSetRegisterCallBack(CDelegateBase& delegate);
protected:
	//override control
	virtual	void	DoInit();

private:
	CButtonUI*			m_pNdbankLoginBtn;
	CButtonUI*			m_pNdbankRegisterBtn;

	CEventSource		m_OnDoClickRegister;
};