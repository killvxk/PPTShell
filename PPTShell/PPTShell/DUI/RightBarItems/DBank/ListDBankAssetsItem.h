#pragma once
//
// ListItem.h
// 
#include "../ListDBankItem.h"

class CListDBankAssetsItem :public CListDBankItem
{
public:
	CListDBankAssetsItem();
	virtual ~CListDBankAssetsItem();

	virtual	void			DoInit();		

	virtual bool			OnEventLoginComplete( void* pObj )		;

	virtual void			DoClick(TNotifyUI*		pNotify) ;				//������

	virtual bool			OnDownloadDecodeList( void* pObj )			;	//����json����	

	virtual void			GetTotalCountInterface()					;	//��ȡ����
protected:
	DWORD					m_dwImageDownloadThreadId;
	DWORD					m_dwFlashDownloadThreadId;
	DWORD					m_dwVideoDownloadThreadId;
	DWORD					m_dwVolumeDownloadThreadId;

	bool					OnGetAssetsCurCount(void * pParam)				;	//��ȡ��ǰ�½ڵ������ص�

	DWORD					m_nGetCurAssetsCount;
	DWORD					m_dwThreadFinishCount;								//�߳���ɼ���
	DWORD					m_dwRequestSuccess	;								//���󲢷������ݼ���
};