// ForceUpdateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PPTShell.h"
#include "ForceUpdateDlg.h"


// CForceUpdateDlg dialog

IMPLEMENT_DYNAMIC(CForceUpdateDlg, CDialog)

CForceUpdateDlg::CForceUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CForceUpdateDlg::IDD, pParent)
{

}

CForceUpdateDlg::~CForceUpdateDlg()
{
}

void CForceUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CForceUpdateDlg, CDialog)
END_MESSAGE_MAP()


// CForceUpdateDlg message handlers

BOOL CForceUpdateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(_T("101����PPT"));
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_ForceUpdateDUI.Subclass(this->GetSafeHwnd());
	m_ForceUpdateDUI.SendMessage(WM_CREATE);	// load skin

	CenterWindow();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
