// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// ("Fluent UI")����ʾ������Ϊ�ο������ṩ�� 
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ���� (C) Microsoft Corporation
// ��������Ȩ����

// CntrItem.h : CPPTShellCntrItem ��Ľӿ�
//

#pragma once

//class CPPTShellDoc;
//class CPPTShellView;

class CPPTShellCntrItem : public COleDocObjectItem
{
	DECLARE_SERIAL(CPPTShellCntrItem)

// ���캯��
public:
	CPPTShellCntrItem(COleDocument* pContainer = NULL);
		// ע��: ���� pContainer Ϊ NULL ������ IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE Ҫ������д���
		//  �����Ĺ��캯����OLE ��ͨ������
		//  �� NULL �ĵ�ָ�빹���

// ����
public:
	CDocument* GetDocument()
		{ return reinterpret_cast<CDocument*>(COleDocObjectItem::GetDocument()); }
	//CPPTShellView* GetActiveView()
	//	{ return reinterpret_cast<CPPTShellView*>(COleDocObjectItem::GetActiveView()); }

public:
	virtual void OnChange(OLE_NOTIFICATION wNotification, DWORD dwParam);
	virtual void OnActivate();

protected:
	virtual void OnDeactivateUI(BOOL bUndoable);
	virtual BOOL OnChangeItemPosition(const CRect& rectPos);
//	virtual BOOL OnShowControlBars(CFrameWnd* pFrameWnd, BOOL bShow);

// ʵ��
public:
	~CPPTShellCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);
};

