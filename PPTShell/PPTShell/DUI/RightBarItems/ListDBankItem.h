#pragma once
//
// ListItem.h
// 
#include "ListBarItem.h"
#include "ListDBankItemComponent.h"

class CListDBankItem :public CListBarItem,
				public CListDBankItemComponent
{
public:
	CListDBankItem();
	virtual ~CListDBankItem();


	virtual	void			DoInit();

	virtual bool			OnRefreshGroupExplorer( void* pObj )		;	//GroupExplorerˢ�°�ť�¼�

	virtual bool			OnEventLoginComplete( void* pObj )		= 0	;	//��¼����¼�

	virtual void			GetTotalCountInterface()					;	//��ȡ����

	virtual bool			OnChapterChanged( void* pObj )				;

	virtual bool			OnGetTotalCount(void * pParam)				;	//��ȡ�����ص�

	virtual void			DoClick(TNotifyUI*		pNotify) ;				//������
	virtual void			DoRClick(TNotifyUI*		pNotify) ;				//�Ҽ����

	void					SetTotalCountUrl(tstring strUrl);			//����������ȡ��ַ
	tstring					GetTotalCountUrl();		

	bool					Select(bool bSelect = true);

	vector<int>*			GetSupportType();
protected:
	void					CalcText( HDC hdc, RECT& rc, LPCTSTR lpszText, int nFontId, UINT format, UITYPE_FONT nFontType, int c = -1);

protected:
	DWORD					m_dwTotalCountDownId;

	tstring					m_strTotalCountUrl;
	vector<int>				m_vecSupportType;
};
