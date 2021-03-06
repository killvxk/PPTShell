#include "stdafx.h"
#include "ListCloud3DResourceItem.h"
#include "NDCloud/NDCloudFile.h"
#include "NDCloud/NDCloudAPI.h"
#include "DUI/GroupExplorer.h"

CListCloud3DResourceItem::CListCloud3DResourceItem()
{

}

CListCloud3DResourceItem::~CListCloud3DResourceItem()
{

}

void CListCloud3DResourceItem::DoInit()
{
	__super::DoInit();

	m_nType = CloudFile3DResource;
	SetTotalCountUrl(NDCloudComposeUrl3DResourceCount());
	GetTotalCountInterface();
}

bool CListCloud3DResourceItem::OnChapterChanged( void* pObj )
{
	TEventNotify* pEventNotify = (TEventNotify*)pObj;
	CStream* pStream = (CStream*)pEventNotify->wParam;
	pStream->ResetCursor();
	tstring strGuid = pStream->ReadString();

	SetCurCountUrl(NDCloudComposeUrl3DResourceInfo(_T("") ,_T(""), 0, 1));
	SetJsonUrl(NDCloudComposeUrl3DResourceInfo(_T(""), _T(""), 0, 500));

	if(m_pStream)
	{
		delete m_pStream;
		m_pStream = NULL; 
	}

	if(IsSelected())
	{
		if(m_dwDownloadId != -1)
		{
			NDCloudDownloadCancel(m_dwDownloadId);
		}

		CGroupExplorerUI * pGroupExplorer = CGroupExplorerUI::GetInstance();
		pGroupExplorer->ShowWindow(true);

		pGroupExplorer->StartMask();
		m_dwDownloadId = NDCloudDownload(GetJsonUrl(), MakeHttpDelegate(this, &CListCloudItem::OnDownloadDecodeList));
	}
// 	else
// 	{
// 		if(m_pStream == NULL)
// 			m_dwDownloadId = NDCloudDownload(GetJsonUrl(), MakeHttpDelegate(this, &ClistCloudItem::OnDownloadDecodeList));
// 	}

	if(m_dwCurCountDownId != -1)
	{
		NDCloudDownloadCancel(m_dwCurCountDownId);
	}

	__super::OnChapterChanged(pObj);

	return true;
}

bool CListCloud3DResourceItem::OnDownloadDecodeList( void* pObj )
{
	CGroupExplorerUI * pGroupExplorer = CGroupExplorerUI::GetInstance();
	THttpNotify* pHttpNotify = (THttpNotify*)pObj;

	if (pHttpNotify->dwErrorCode > 0)
	{
		pGroupExplorer->StopMask();
		pGroupExplorer->ShowNetlessUI(true);
		return true;
	}
	else
	{
		pGroupExplorer->ShowNetlessUI(false);
	}

	if(m_pStream)
		delete m_pStream;
	m_pStream = new CStream(1024);

	if (!NDCloudDecode3DResourceList(pHttpNotify->pData, pHttpNotify->nDataSize, m_pStream))
	{
		pGroupExplorer->ShowNetlessUI(true);
		pGroupExplorer->StopMask();
		return false;
	}

	if(IsSelected())
	{
		pGroupExplorer->ShowResource(m_nType, m_pStream);
		pGroupExplorer->StopMask();
	}
	

	return true;
}
