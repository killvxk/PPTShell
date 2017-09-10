#pragma once
//
// ListItem.h
// 
#include "ListBarItem.h"
#include "ListCloudItemComponent.h"

class CListCloudItem :public CListBarItem,
					public CListCloudItemComponent
{
public:
	CListCloudItem();
	virtual ~CListCloudItem();

	virtual	void			DoInit();

	virtual bool			OnChapterChanged( void* pObj )				;	//�½��л��¼�
	virtual bool			OnRefreshGroupExplorer( void* pObj )		;	//GroupExplorerˢ�°�ť�¼�

	virtual void			GetTotalCountInterface()					;	//��ȡ����
	virtual void			GetCurCountInterface(tstring strGuid)		;	//��ȡ��ǰ�½ڵ�����

	virtual bool			OnGetTotalCount(void * pParam)				;	//��ȡ�����ص�
	virtual bool			OnGetCurCount(void * pParam)				;	//��ȡ��ǰ�½ڵ������ص�

	virtual void			DoClick(TNotifyUI*		pNotify) ;				//������
	virtual void			DoRClick(TNotifyUI*		pNotify) ;				//�Ҽ����


	void					SetCurCountUrl(tstring strUrl);				//���õ�ǰ�½�������ȡ��ַ
	tstring					GetCurCountUrl();	

	void					SetTotalCountUrl(tstring strUrl);			//����������ȡ��ַ
	tstring					GetTotalCountUrl();							

	bool					Select(bool bSelect = true);			

protected:
	void					CalcText( HDC hdc, RECT& rc, LPCTSTR lpszText, int nFontId, UINT format, UITYPE_FONT nFontType, int c = -1);

protected:
	DWORD					m_dwCurCountDownId;
	DWORD					m_dwTotalCountDownId;

	tstring					m_strCurCountUrl;
	tstring					m_strTotalCountUrl;

	
};