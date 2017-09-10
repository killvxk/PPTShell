#pragma once
#include "AssetsVideoItem.h"
#include "AssetsImageItem.h"
#include "AssetsFlashItem.h"
#include "AssetsVolumeItem.h"
#include "AssetsAllItem.h"

#include "AssetsAllLayout.h"

class CAssetsWindowUI : public CVerticalLayoutUI
{
public:
	CAssetsWindowUI();
	virtual ~CAssetsWindowUI();

public:
	void				Init();

	void				RefreshData();
// 	CAssetsVideoItem *	GetAssetsVideoItem();
// 	CAssetsImageItem *	GetAssetsImageItem();
// 	CAssetsFlashItem *	GetAssetsFlashItem();
// 	CAssetsVolumeItem *	GetAssetsVolumeItem();
protected:
	bool OnItemClick( void* pObj );
	bool OnDoFinish( void* pObj );

	virtual bool	OnChapterChanged( void* pObj )				;	//�½��л��¼�
	//virtual bool	OnRefreshGroupExplorer( void* pObj )		;	//GroupExplorerˢ�°�ť�¼�

private:
	CAssetsVideoItem	*m_pAssetsVideoItem;
	CAssetsImageItem	*m_pAssetsImageItem;
	CAssetsFlashItem	*m_pAssetsFlashItem;
	CAssetsVolumeItem	*m_pAssetsVolumeItem;
	CAssetsAllItem		*m_pAssetsAllItem;

	void*				m_pCurSelected;

	int					m_nDoFinishCount;

	CAssetsAllLayout*	m_pAssetsAllLayout;
};