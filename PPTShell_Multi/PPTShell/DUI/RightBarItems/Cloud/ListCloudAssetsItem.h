#pragma once
//
// ListItem.h
// 
#include "../ListCloudItem.h"

class CListCloudAssetsItem :public CListCloudItem
{
public:
	CListCloudAssetsItem();
	virtual ~CListCloudAssetsItem();

	virtual	void			DoInit();

	virtual bool			OnDownloadDecodeList( void* pObj )			;	//����json����				

	virtual bool			OnChapterChanged( void* pObj )				;	//�½��л��¼�

	virtual void			DoClick(TNotifyUI*		pNotify) ;				//������

	virtual void			GetCurCountInterface(tstring strGuid)		;	//��ȡ��ǰ�½ڵ�����

	virtual void			GetTotalCountInterface()					;	//��ȡ����
protected:

	DWORD					m_dwCurImageDownloadThreadId;
	DWORD					m_dwCurFlashDownloadThreadId;
	DWORD					m_dwCurVideoDownloadThreadId;
	DWORD					m_dwCurVolumeDownloadThreadId;

	DWORD					m_dwTotalImageDownloadThreadId;
	DWORD					m_dwTotalFlashDownloadThreadId;
	DWORD					m_dwTotalVideoDownloadThreadId;
	DWORD					m_dwTotalVolumeDownloadThreadId;

	bool					OnGetAssetsCurCount(void * pParam)				;	//��ȡ��ǰ�½ڵ������ص�
	bool					OnGetAssetsTotalCount(void * pParam)				;	//��ȡ��ǰ�½ڵ������ص�

	DWORD					m_nGetCurAssetsCount;
	DWORD					m_dwCurThreadFinishCount;								//�߳���ɼ���
	DWORD					m_dwCurRequestSuccess	;								//���󲢷������ݼ���

	DWORD					m_nGetTotalAssetsCount;
	DWORD					m_dwTotalThreadFinishCount;								//�߳���ɼ���
	DWORD					m_dwTotalRequestSuccess	;								//���󲢷������ݼ���
};