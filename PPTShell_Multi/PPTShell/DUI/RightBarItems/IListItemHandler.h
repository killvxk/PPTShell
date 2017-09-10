#pragma once
//
// IHandler.h
// 

class IListItemHandler
{
public:
	virtual bool			OnChapterChanged( void* pObj )				= 0;	//�½��л��¼�
	virtual bool			OnRefreshGroupExplorer( void* pObj )		= 0;	//GroupExplorerˢ�°�ť�¼�

	virtual bool			OnDownloadDecodeList( void* pObj )			= 0;	//����json����

	

private:
	
};