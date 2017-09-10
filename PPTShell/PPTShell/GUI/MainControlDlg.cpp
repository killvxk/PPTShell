// MainControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PPTShell.h"
#include "MainControlDlg.h"
#include "DUI\GuideDialog.h"
#include "DUI\UpdateDialog.h"
#include "DUI/ShadeWindow.h"
#include "GUI/MainFrm.h"
#include "Config.h"
// CMainControlDlg dialog

IMPLEMENT_DYNAMIC(CMainControlDlg, CPanelInnerDlg)

CMainControlDlg::CMainControlDlg(CWnd* pParent /*=NULL*/)
	: CPanelInnerDlg(pParent)
{
	m_pListBarDlg = NULL;
}

CMainControlDlg::~CMainControlDlg()
{
}

BEGIN_MESSAGE_MAP(CMainControlDlg, CPanelInnerDlg)
END_MESSAGE_MAP()
 
// CMainControlDlg message handlers
BOOL CMainControlDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//--------------------------------------------------
	m_pListBarDlg = CListBarDlg::GetInstance(this->GetParent()->GetParent()->GetSafeHwnd());
	m_pListBarDlg->Subclass(this->GetSafeHwnd());
	m_pListBarDlg->SendMessage(WM_CREATE);	// load skin
	//--------------------------------------------------

	g_Config::GetInstance()->SetStartAppCount();
	g_Config::GetInstance()->SetCheckUpdateCount();
	SetTimer(5522, 1000 * 60 * 10, NULL);
//	SetTimer(5522, 1000 * 60 * 2, NULL);
/*	CGuideDialogUI::GetInstance()->Create(GetSafeHwnd(), _T("GuideWindow"), WS_OVERLAPPED, 0, 0, 0, 0, 0);*/
	//������ʱ��ͬ���������½�������ݻ�ȡ���� 2016.03.04 cws
	//if(g_Config::GetInstance()->GetModuleVisible(MODULE_AUTOSYNCTIME))//2016.03.07 Ĭ�ϲ��Զ�������ʱ����
	//{	}
	//SetTimer(5523, 1000, NULL);//ͬ������ʱ�䣬ͬ����ϣ��رն�ʱ�� 


	return TRUE;
}


LRESULT CMainControlDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch ( message )
	{	
	case WM_CREATE_GUIDE:
		{
			CRect rect;
			HWND hwnd = AfxGetApp()->m_pMainWnd->GetSafeHwnd();
			::GetWindowRect(hwnd, &rect);
			CGuideDialogUI *pGuideDialog = CGuideDialogUI::GetInstance();

			if(pGuideDialog->GetHWND() == NULL)
			{
				pGuideDialog->Create(hwnd, _T("GuideWindow"), WS_POPUP, 0, 0, 0, 0, 0);//�����ö�2016.03.09cws 

				pGuideDialog->Init(rect);
				pGuideDialog->ShowWindow(true);
				pGuideDialog->LoadTree((bool)wParam);
			}
		}
		break;
	case WM_EXIT_PROCESS:
		{
			CMainFrame* pMainFrame = (CMainFrame* )AfxGetApp()->m_pMainWnd;

			if ( pMainFrame != NULL )
				pMainFrame->CloseRelateApp();
		}
		break;
	case WM_TIMER:
		{
			if ( wParam == 5522 )
			{
				DWORD dwCheckUpdateCount = g_Config::GetInstance()->GetCheckUpdateCount();
				DWORD dwStartAppCount = g_Config::GetInstance()->GetStartAppCount();
				if(g_Config::GetInstance()->GetCheckUpdateStatus() != eUpdateType_Has_Update && GetTickCount() - dwCheckUpdateCount > /*1000 * 60 * 1*/1000 * 60 * 30)//0.5ʱ
				{
					g_Config::GetInstance()->SetCheckUpdateCount();
					//������
					CMainFrame* pMainFrame = (CMainFrame* )AfxGetApp()->m_pMainWnd;

					if ( pMainFrame != NULL )
						pMainFrame->m_update.CheckUpdate();
					//
				}
				else if(g_Config::GetInstance()->GetCheckUpdateStatus() == eUpdateType_Has_Update && GetTickCount() - dwStartAppCount > /*1000 * 60 * 1*/1000 * 60 * 60 * 24)
				{
					//������ʾ
					::PostMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(), WM_NOTIFY_UPDATE, NOTIFY_FORM_TIMER, 0);
					//
				}
			}
			/*if ( wParam == 5523 )
			{
				DWORD nStart = GetTickCount();
				CSyncInternetTime* pSyncTime= new CSyncInternetTime();
				pSyncTime->SyncSystime();
				delete pSyncTime;
				WRITE_LOG_LOCAL( _T("use synctime :%d ms"), GetTickCount() - nStart); 
				KillTimer(5523);
			}*/
			
		}
		break;
	//case WM_SHOW_SHADE:
	//	{
	//		TCHAR * szBuff = (TCHAR *)wParam;
	//		CShadeWindow *pShadeWindow = CShadeWindow::GetInstance();
	//		
	//		if(pShadeWindow && pShadeWindow->GetHWND() == NULL)
	//		{
	//			pShadeWindow->Create(GetSafeHwnd(), _T("GuideWindow"), WS_POPUP, 0, 0, 0, 0, 0);
	//		}

	//		CRect rect;
	//		::GetWindowRect(AfxGetMainWnd()->GetSafeHwnd(), &rect);
	//		
	//		pShadeWindow->Init(rect);
	//		pShadeWindow->Show(szBuff, true);
	//	}
	//	break;
	//case WM_HIDE_SHADE:
	//	{
	//		CShadeWindow *pShadeWindow = CShadeWindow::GetInstance();
	//		
	//		if(pShadeWindow && pShadeWindow->GetHWND() != NULL)
	//		{
	//			pShadeWindow->Hide();
	//		}
	//	}
	//	break;
	case WM_SHOW_UCLOGIN:
		{
			CItemExplorerUI::GetInstance()->ShowWindow(false);
			CGroupExplorerUI::GetInstance()->ShowWindow(true);
			CGroupExplorerUI::GetInstance()->ShowLoginUI();
		}
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

int CMainControlDlg::GetInnerIndex()
{
	return eInner_Navgate;
}

int CMainControlDlg::GetInnerWidth()
{
	return 100;
}

void CMainControlDlg::SetInnerWidth( int nWidth )
{
	
}

CWindowWnd* CMainControlDlg::GetWindowWnd()
{
	return dynamic_cast<CWindowWnd*>(m_pListBarDlg);
}
