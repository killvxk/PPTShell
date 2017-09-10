#pragma once
#include "DUICommon.h"

class CRenameDlgUI : public WindowImplBase
{
	UIBEGIN_MSG_MAP
		EVENT_BUTTON_CLICK(_T("btnOK"),OnBtnOK);
		EVENT_BUTTON_CLICK(_T("btnCancel"),OnBtnCancel);
	UIEND_MSG_MAP
public:
	CRenameDlgUI(void);
	~CRenameDlgUI(void);
	string GetNewName();
	void SetOldName2Edit(string strOldName);
	void AdjustWindowPos();

private:
	LPCTSTR GetWindowClassName() const;	
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	void InitWindow();
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

	void OnBtnOK(TNotifyUI& msg);
	void OnBtnCancel(TNotifyUI& msg);
	void OnEditRenameTextChanged( TNotifyUI& msg );
	bool IsNameValid(const TCHAR* FileName);

	bool m_bActiveClose;//��ʶ�Ƿ����������ر�
	string m_strDefaultTipText;
	string m_strOldName;
	string m_strNewName;

	CEditUI*	m_pEditRename;
};
