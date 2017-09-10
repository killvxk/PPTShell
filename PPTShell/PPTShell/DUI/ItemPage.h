#pragma once

#include "DUICommon.h"
#include <vector>
#include "TabPage.h"

#define Search_MaxItemPage_Count 5
typedef struct _ITEM_PAGE
{
	int			nItemType;
	tstring		strPath;
	tstring		strName;
	tstring		strGroup;
	tstring		strChapter;
}ITEM_PAGE,*PITEM_PAGE;

namespace DuiLib
{
	class CItemPageUI : public CVerticalLayoutUI
	{
	public:
		CItemPageUI();
		~CItemPageUI();

		virtual LPCTSTR GetClass() const;
		virtual LPVOID	GetInterface(LPCTSTR pstrName);

		void		AddSrcData( ITEM_PAGE & itemPage);

		int			GetAllPagesCount()			{return m_nAllPagesCount;}
		
		int			GetPerPageCount()			{return m_nPerPageCount;}
		void		SetPerPageCount(int nCount)	{m_nPerPageCount = nCount;}


		void		Create(int nIndex = 0);					//����
		int			GetSelectPage()				{return m_nCurrentPageIndex;}

		void		Clear();
		CVerticalLayoutUI*	GetMainLayout()		{return m_pMainLayout;}

		void			InsertPageIndex(DWORD dwCount, int nStart, int nIndex, bool bInit);
		bool			OnBtnPageChange(void* pNotify);

		CVerticalLayoutUI*					CreateItemPage();
		CVerticalLayoutUI*					GetContentLayout();//��ȡ���ݲ���
	protected:
		int									m_nCurrentPageIndex;//��ǰҳ��
		int									m_nAllPagesCount;//��ҳ��
		int									m_nPerPageCount;//ÿҳ��ʾ����

		CVerticalLayoutUI*					m_pContentViewLayout; //���ݲ���
		CVerticalLayoutUI*					m_pContentPageLayout; //��ҳ����
		CVerticalLayoutUI*					m_pMainLayout;

		vector<ITEM_PAGE>					m_vecItems;

	public:
		CEventSource						m_OnCreateCallback;

		CDialogBuilder						m_itemPageBuilder;
	};
}
