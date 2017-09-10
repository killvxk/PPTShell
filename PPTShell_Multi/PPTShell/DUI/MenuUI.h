#pragma once
#include "DUICommon.h"

class CMenuUI: public WindowImplBase
{
	UIBEGIN_MSG_MAP
		EVENT_ID_HANDLER(DUI_MSGTYPE_ITEMCLICK, _T("menu_open"), OnMenuOpenClick);
		EVENT_ID_HANDLER(DUI_MSGTYPE_ITEMCLICK, _T("menu_import"), OnMenuImportClick);
		EVENT_ID_HANDLER(DUI_MSGTYPE_ITEMCLICK, _T("menu_rename"), OnMenuRenameClick);
		EVENT_ID_HANDLER(DUI_MSGTYPE_ITEMCLICK, _T("menu_clearup"), OnMenuClearupClick);
		EVENT_ID_HANDLER(DUI_MSGTYPE_ITEMCLICK, _T("menu_upload_netdisc"), OnMenuUploadNetdiscClick);
		EVENT_ID_HANDLER(DUI_MSGTYPE_ITEMCLICK, _T("menu_download_local"), OnMenuDownloadLocalClick);
		EVENT_ID_HANDLER(DUI_MSGTYPE_ITEMCLICK, _T("menu_delete"), OnMenuDeleteClick);
		EVENT_ID_HANDLER(DUI_MSGTYPE_ITEMCLICK, _T("menu_edit_exercises"), OnMenuEditExercisesClick);
		EVENT_ID_HANDLER(DUI_MSGTYPE_ITEMCLICK, _T("menu_upload"), OnMenuUploadClick);
	UIEND_MSG_MAP

public:
	CMenuUI(void);
	~CMenuUI(void);
	void SetType(int nType);//���ò˵���Ӧ���Ǹ�ģ��
	void SetItemIndex(int nIndex);
	void SetIsCloudResource(bool isCloud);//
	void HideAllMenuItem();//�������в˵���
	void ShowMenuOpen();//��ʾ����
	void ShowMenuImport();//��ʾ������
	void ShowMenuRename();//��ʾ��������
	void ShowMenuClearup();//��ʾ�����
	void ShowMenuUploadNetDisc();//��ʾ�ϴ����ҵ�������
	void ShowMenuDownloadLocal();//��ʾ���ص�������
	void ShowMenuDelete();//��ʾɾ����
	void ShowMenuEditExercises();//��ʾ�༭ϰ����
	void ShowMenuUpload();//��ʾ�ϴ���
private:
	LPCTSTR GetWindowClassName() const;	
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual void InitWindow();
	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void OnMenuOpenClick(TNotifyUI& msg);
	void OnMenuImportClick(TNotifyUI& msg);
	void OnMenuRenameClick(TNotifyUI& msg);
	void OnMenuClearupClick(TNotifyUI& msg);
	void OnMenuUploadNetdiscClick(TNotifyUI& msg);
	void OnMenuDownloadLocalClick(TNotifyUI& msg);
	void OnMenuDeleteClick(TNotifyUI& msg);
	void OnMenuEditExercisesClick(TNotifyUI& msg);
	void OnMenuUploadClick(TNotifyUI& msg);
	void RefreshMenuItem();
	void RefreshSize();

	int m_nType;
	int m_nItemIndex;
	bool m_bIsCloud;

	CListContainerElementUI* m_pMenuOpen;
	CControlUI* m_pMenuImportSplit;
	CListContainerElementUI* m_pMenuImport;
	CControlUI* m_pMenuRenameSplit;
	CListContainerElementUI* m_pMenuRename;
	CControlUI* m_pMenuClearupSplit;
	CListContainerElementUI* m_pMenuClearup;
	CControlUI* m_pMenuUploadNetdiscSplit;
	CListContainerElementUI* m_pMenuUploadNetdisc;
	CControlUI* m_pMenuDownloadLocalSplit;
	CListContainerElementUI* m_pMenuDownloadLocal;
	CControlUI* m_pMenuDeleteSplit;
	CListContainerElementUI* m_pMenuDelete;
	CControlUI* m_pMenuEditExercisesSplit;
	CListContainerElementUI* m_pMenuEditExercises;
	CControlUI* m_pMenuUploadSplit;
	CListContainerElementUI* m_pMenuUpload;
};
