#pragma once
#include "stdafx.h"
#include "DUICommon.h"
#include "Util\Singleton.h"

#define DESKTOP_TIMER_ID 90001

class CUserReLogin : public WindowImplBase/*, public IListCallbackUI*/
{
public:
	CUserReLogin(void);
	~CUserReLogin(void);

	UIBEGIN_MSG_MAP 
		EVENT_ITEM_CLICK(_T("LogOutBtn"),	OnLogOutBtn);
		EVENT_ITEM_CLICK(_T("ChangeUserBtn"),OnChangeUserBtn);
		EVENT_ITEM_CLICK(_T("ModifyPwdBtn"),OnModifyPwdBtn);

		EVENT_ID_HANDLER(DUI_MSGTYPE_ITEMSELECT,	_T("UserDept"),		OnSelectEvent);
	UIEND_MSG_MAP
 

public:
	LPCTSTR GetWindowClassName() const;	
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();

	void	InitWindows(HWND hWndParent, POINT ptPos ,tstring pUserPhoto,  tstring pNewUserName, int nLoginType = 0);   
	void	ShowLocalWindows( );
	void	OnLogOutBtn(TNotifyUI& msg);
	void	OnChangeUserBtn(TNotifyUI& msg);
	void	OnModifyPwdBtn(TNotifyUI& msg);
	void	SetUserName(tstring pNewUserName);
	void	SetUserDept();
	bool	OnCloseWndEvent(void* pObj);
	void	OnSelectEvent(TNotifyUI& msg);
	//virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
protected:  
	HWND			m_hWndParent;  
	POINT			m_ptPos;
	CLabelUI*		m_pUserName;//��ʦ����
	CComboUI*		m_pUserDept;//�����꼶
	CButtonUI*		m_pLogOutBtn;//�˳�
	CButtonUI*		m_pChangeUserBtn;//�л���½
	CButtonUI*		m_pModifyPwdBtn;//�޸�����
	CControlUI*		m_pUserImage;//�û�����ͼ
	CContainerUI*	m_pFreeTeachCtn;//�����ڿ�ѡ��״̬����2016.02.14

	CListContainerElementUI*	m_pUCLoginElement;
private:
	tstring			m_sTeacherGradeName;//��ʦ���ѡ��İ༶
	tstring			m_sTeacherName;//��ʦ����
	bool			m_bFreeTeach;//�����ڿ�ģʽ
	bool			m_bInit;
};


typedef Singleton<CUserReLogin>		UserReLoginUI;
