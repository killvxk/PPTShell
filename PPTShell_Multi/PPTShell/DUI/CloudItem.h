#pragma once
#include "DUICommon.h"
#include "Util/Stream.h"
#include "ResourceItem.h"
#include "NDCloud/NDCloudContentService.h"

class CCloudItemUI : public CResourceItemUI
{
public:
	CCloudItemUI();
	virtual ~CCloudItemUI();

	virtual void DownloadLocal();//���ص�����
	void Delete(int currentModeType);//ɾ��
	void Rename(string strNewName);//������
protected:
	void			ShowProgress(bool bVisible);
	void			SetProgress(int nPos);

public:
	virtual	void	Init();
	virtual void	DownloadResource(int nButtonIndex, int nType, int nThumbnailSize);
	virtual void	OnDownloadResourceBefore();
	virtual void	OnDownloadResourceCompleted(int nButtonIndex, LPCTSTR lptcsPath);

	bool			OnDownloadResourceProgress(void* pNotify);
	bool			OnDownloadResourceCompleted(void* pNotify);
	bool OnDeleteResourceCompelete( void* pNotify );
	bool OnRenameResourceCompelete( void* pNotify );
	virtual	void	ReadStream(CStream* pStream);

	DWORD			GetSlideId();
	DWORD			GetPlaceHolderId();
	void			SetSlideId(DWORD dwId);
	void			SetPlaceHolderId(DWORD dwId);
protected:
	CProgressUI*			m_proDownload;
	DWORD					m_dwResourceDownloadId;
	DWORD					m_dwLastOperationerId;
	CDialogBuilder			m_builder;
	DWORD					m_dwSlideId;
	DWORD					m_dwPlaceHolderId;

	tstring					m_strGuid;
	tstring					m_strJsonInfo;

	CONTENT_TYPE			m_eContentType;
private:
	string m_strNewName;//��ʱ�洢�������ַ���������ص���ֵ
};
