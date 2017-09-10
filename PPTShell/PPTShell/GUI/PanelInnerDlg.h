#pragma once
#include "Resource.h"

// CPanelInnerDlg �Ի���

enum eInnerIndex
{
	eInner_Navgate,
	eInner_Group,
	eInner_Item,
};

class IInnerable{

public:
	virtual	int			GetInnerIndex()				= 0;
	virtual	int			GetInnerWidth()				= 0;
	virtual	void		SetInnerWidth(int nWidth)	= 0;
	virtual CWindowWnd* GetWindowWnd()				= 0;

};


class CPanelInnerDlg : 
	public CDialog,
	public IInnerable
{
	DECLARE_DYNAMIC(CPanelInnerDlg)

public:
	CPanelInnerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPanelInnerDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage( MSG* pMsg );

	DECLARE_MESSAGE_MAP()
	afx_msg virtual void OnSize(UINT nType, int cx, int cy);

public:
	static CDockablePane*	m_MainPanel;
	static void				AdjustMainPanel();

	

};
