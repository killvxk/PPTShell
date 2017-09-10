#pragma once
//
// ListItem.h
// 
#include "AssetsItemNotify.h"
#include "AssetsItemHandler.h"
#include "AssetsItemComponent.h"

class CAssetsItem : public CVerticalLayoutUI,
	public IAssetsItemNotify,
	public IAssetsItemHandler,
	public CAssetsItemComponent

{
public:
	CAssetsItem();
	virtual ~CAssetsItem();


	void			SetJsonUrl(tstring strJsonUrl);					//����json��ȡ��ַ
	tstring			GetJsonUrl();

	int				GetItemType();									//��ȡitem����

	virtual void	DoClick(TEventUI* pEvent) ;						//������
	virtual void	DoRClick(TEventUI* pEvent) ;					//�Ҽ����

	virtual bool	OnControlNotify(void* pEvent);					//Notify�¼�

	virtual	void	DoInit();

	virtual	void	SetDoClickCallBack(CDelegateBase& delegate);	//���õ���ص�
	virtual	void	SetDoRClickCallBack(CDelegateBase& delegate);	//��������Ҽ�����¼�
	virtual	void	SetDoFinishCallBack(CDelegateBase& delegate);							//����������ɻص�

	virtual	void	RefreshData(bool bForce = false);

	virtual bool	OnChapterChanged( void* pObj )				;	//�½��л��¼�
	virtual bool	OnRefreshGroupExplorer( void* pObj )		;	//GroupExplorerˢ�°�ť�¼�
	virtual bool	OnEventLoginComplete( void* pObj )			;	//��¼�ɹ�

	void			SetLayout(CAssetsLayout* pLayout);
	CAssetsLayout*	GetLayout();

	void			Select(bool bSelected);
	bool			GetSelected();

	void			SetCountText( int nCount );

	virtual	CStream*GetStream();									//��ȡ������
	bool			GetNetStatus();
protected:

	void			CalcText( HDC hdc, RECT& rc, LPCTSTR lpszText, int nFontId, UINT format, UITYPE_FONT nFontType, int c = -1);

	DWORD			m_dwDownloadId;

	CStream*		m_pStream;
private:

	bool			m_bSelected;
	tstring			m_strJsonUrl;


};