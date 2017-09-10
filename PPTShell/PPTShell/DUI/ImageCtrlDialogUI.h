#pragma once
#include "DUICommon.h"
#include "Util/Singleton.h"

class CImageCtrlDialogUI : public WindowImplBase
{
private:
	CImageCtrlDialogUI(void);
	~CImageCtrlDialogUI(void);

	UIBEGIN_MSG_MAP
		EVENT_BUTTON_CLICK(_T("btnClose"),OnBtnCloseClick);
	UIEND_MSG_MAP

	void OnBtnCloseClick(TNotifyUI& msg);

public:
	void OpenDialog(LPCTSTR strImagePath);
	void CloseDialog();
	void ZoomIn(float scale);
	void ZoomOut(float scale);
	void Normal();

	bool OnImageControl(void* param);

	// ���ô����ѹرյı�־
	bool IsWindowClosed();

	DECLARE_SINGLETON_CLASS(CImageCtrlDialogUI);

protected:
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual	void InitWindow();
	virtual LRESULT OnKeyDown(UINT , WPARAM , LPARAM , BOOL& bHandled);

	// ��ȡ�����Ƿ��ѹرյı�־
	void	SetWindowClosed(bool bClosed = true);
	// ���㴰�ڴ�С�����ƶ�����
	void	ResizeWindow(int nWidth, int nHeight);
protected:
	CDuiString			m_strImagePath;
	RECT				m_rtOriginal;

	bool				m_bClosed;		// �����Ƿ��ѹر�
};

typedef Singleton<CImageCtrlDialogUI>	ImageCtrlDialogUI;