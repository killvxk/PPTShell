#pragma once

#include "DUICommon.h"
#include <vector>
#include "Util/Stream.h"
#include "DUI/ResourceItem.h"
#include "DUI/GifAnimUI.h"
#include "DUI/CourseItem.h"
#include "DUI/MusicItem.h"
#include "DUI/PhotoItem.h"
#include "DUI/VideoItem.h"
#include "DUI/FlashItem.h"
#include "DUI/NdpCourseItem.h"

#include "DUI/LocalCourseItem.h"
#include "DUI/LocalMusicItem.h"
#include "DUI/LocalPhotoItem.h"
#include "DUI/LocalVideoItem.h"
#include "DUI/LocalFlashItem.h"
#include "DUI/LocalQuestionItem.h"

#include "NDCloud/NDCloudAPI.h"
#include "DUI/LocalItem.h"
enum
{
	TabLayout_Type_Course = 0,
	TabLayout_Type_Video,
	TabLayout_Type_Pic,
	TabLayout_Type_Flash ,
	TabLayout_Type_Volume,
	TabLayout_Type_Question,
	TabLayout_Type_All		//��ע��ALLΪ����ȫ�����������ע��
};

namespace DuiLib
{

	class CTabPageCallback : public IDialogBuilderCallback 
	{
	public:
		void			SetType(int nType) {m_nType = nType; }

	private:
		int									m_nType;

	public:

		CControlUI* CreateControl(LPCTSTR pstrClass)
		{

			if( _tcscmp(pstrClass, _T("ResourceItem")) == 0 )
			{
				switch (m_nType)
				{
				case CloudFileCourse:
					return new CCourseItemUI;
				case CloudFileVideo:
					return new CVideoItemUI;
				case CloudFileImage:
					return new CPhotoItemUI;
				case CloudFileFlash:
					return new CFlashItemUI;
				case CloudFileVolume:
					return new CMusicItemUI;
				case LocalFileImport:
				case LocalFileSearch:
					return new CLocalItemUI;
				case CloudFileNdpCourse:
					return new CNdpCourseItemUI;
				}
			}
			else if( _tcscmp(pstrClass, _T("LocalItem")) == 0 )
			{
				switch(m_nType)
				{
					case TabLayout_Type_Course:
						return new CLocalCourseItemUI;
					case TabLayout_Type_Video:
						return new CLocalVideoItemUI;
					case TabLayout_Type_Pic:
						return new CLocalPhotoItemUI;
					case TabLayout_Type_Flash:
						return new CLocalFlashItemUI;
					case TabLayout_Type_Volume:
						return new CLocalMusicItemUI;
					case TabLayout_Type_Question:
						return new CLocalQuestionItemUI;
				}
			}
			else if( _tcscmp(pstrClass, _T("GifAnim")) == 0 )
				return new CGifAnimUI;

			return NULL;
		}
	};

	class CTabLayoutPageUI : public CVerticalLayoutUI
	{
	public:
		CTabLayoutPageUI();
		~CTabLayoutPageUI();

		virtual LPCTSTR GetClass() const;
		virtual LPVOID	GetInterface(LPCTSTR pstrName);

	public:
		bool			OnOptSearch(void* pNotify);
		bool			OnBtnShowAll(void* pNotify);
		virtual bool	OnBtnPageChange(void* pNotify);
		virtual bool	OnItemSelect(void* pNotify);

		virtual bool	SetContent(int nIndex, CStream & stream, DWORD &dwRetCount, bool bInit = true, bool bDBank = false);
		virtual void	SetSimpleContent(int nIndex,CStream & stream, bool bDBank = false);

		virtual void	Init();

		void			SetKeyword(tstring &strKeyword);
	public:
		void			SelectOption(int nIndex);
		void			SelectOption(tstring strName);
		void			SelectContentLayout(int nIndex);
		void			SelectContentLayout(tstring strName);

		void			InsertPageIndex(DWORD dwCount, int nStart, int nIndex, bool bInit);

		virtual CResourceItemUI*	CreateCloudItem();

		bool			OnGetCloudRes(void * pParam);

		void			SetType(int nType);
		void			SetResType(int nResType);
		int				GetResType();

		virtual	void	SelectItem(int nIndex);
		virtual	void	HideSearchTab();
		virtual	void	ShowSearchTab();
	public:
		CTabPageCallback					m_callback;
		vector<COptionUI *>					m_vecTabLayoutOpt; //Tab�������л���ť

		vector<CVerticalLayoutUI *>			m_vecContentPageLayout; //���ݲ���

		vector<CVerticalLayoutUI *>			m_vecContentViewLayout; //���ݲ���
		vector<CVerticalLayoutUI *>			m_vecContentLayout; //���ݲ���

		vector<CVerticalLayoutUI *>			m_vecSimpleContentViewLayout; //���ݲ���
		vector<CVerticalLayoutUI *>			m_vecSimpleContentLayout; //���ݲ���

		vector<CLabelUI *>					m_vecSimpleCountTextLabel;

		CTabLayoutUI*						m_pSearchTabLayout;//TabLayout����

		CDialogBuilder						m_cloudItemBuilder;

		tstring								m_strKeyword;

		DWORD								m_dwDownloadId;

		int									m_nType;
		int									m_nResType;
		//map<int nIndex, CStream stream>		m_mapSearchSaveData;

	};

	class CTabLayoutLocalPageUI : public CTabLayoutPageUI
	{
	public:
		CTabLayoutLocalPageUI();
		~CTabLayoutLocalPageUI();

		virtual LPCTSTR GetClass() const;
		virtual LPVOID	GetInterface(LPCTSTR pstrName);

	public:
		virtual bool	OnBtnPageChange(void* pNotify);

		virtual bool	SetContent(int nIndex, CStream & stream, DWORD &dwRetCount, bool bInit = true);
		virtual void	SetSimpleContent(int nIndex,CStream & stream);

	private:

		virtual CLocalItemUI*	CreateLocalItem();

	private:
		CStream			m_streamCourse;
		CStream			m_streamVideo;
		CStream			m_streamPic;
		CStream			m_streamFlash;
		CStream			m_streamVolume;
	};
}
