#pragma once

#include "DUICommon.h"
#include "DUI/WndShadow.h"
#include "Util/Singleton.h"

#define _STR_PACK_FAIL_SAVE_KNOWN		_T("�ǳ���Ǹ����ʱ�޷�����")
#define _STR_PACK_FAIL_SAVE_UNKNOWN		_T("��ѽ�������ˣ�")
#define _STR_PACK_FAIL_UNKNOWN			_T("����ԭ��:����Ա��类�����˰���ˣ�\r\n����Ա������������ע������\r\n�ͻ��˰����ˣ�")
#define _STR_PACK_FAIL_KNOWN			_T("��ر������ļ������ԣ�")

class CPackPPTDialogUI : public WindowImplBase
{
public:
	CPackPPTDialogUI();
	~CPackPPTDialogUI();

public:
	LPCTSTR GetWindowClassName() const;	

	virtual void InitWindow();

	virtual CDuiString GetSkinFile();

	virtual CDuiString GetSkinFolder();

//	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
//	bool	OnWindowDestroy(void * Param);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void	Pack( tstring strFile );
	void	UnPack( tstring strFile );

	void	Init(CRect rect);
	static void  TimerProcComplete(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);

	tstring GetPath() {return m_strFilePath ;}
private:
//	CWndShadow					m_WndShadow;
	CRect						m_rcRect;

	CProgressUI*				m_pProgress;
	CLabelUI*					m_pLabel;

	int							m_nProgressCount;

	DWORD						m_dwTaskId;
private:
	bool OnPackProgress(void * pParam);
	bool OnPackCompleted(void * pParam);
	bool OnUnPackCompleted(void * pParam);
	bool OnSetValue(void * pParam);
	bool OnSaveToDBankCompleted(void* param);

	int	 m_nTotal;
	tstring m_strFilePath;	
	tstring m_strFailPath;	
	DWORD	m_dwRet;
};

typedef Singleton<CPackPPTDialogUI> PackPPTDialogUI;