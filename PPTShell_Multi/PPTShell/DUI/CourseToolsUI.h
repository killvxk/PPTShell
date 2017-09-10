#pragma once
#include "DUICommon.h"
#include "..\..\DuiLib\Layout\UIVerticalLayout.h"
#include "CourseTool.h"

class CCourseToolsUI: public WindowImplBase
{
	/*UIBEGIN_MSG_MAP
		EVENT_BUTTON_CLICK(_T("layoutGroupItem"),OnToolItemClick);
	UIEND_MSG_MAP*/
public:
	CCourseToolsUI(void);
	~CCourseToolsUI(void);

	LPCTSTR GetWindowClassName() const;	
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();

	void LoadTools();
	void KillOsrWindows();//�ر�ѧ�ƹ���exe
	void SetLeftOrRight(int nLeftOrRight);
private:
	CDialogBuilder m_builderGroup;
	CDialogBuilder m_builderGroupItem;
	CVerticalLayoutUI* m_pContainer;
	char m_szCurDir[MAX_PATH];//����Ŀ¼
	string m_strToolsName;//ѧ�ƹ��߼��ع���
	map<string,vector<CCourseTool>> m_mapCourseTools;
	int totalHeight;
	int maxHeight;
	int m_nLeftOrRight;

	void InitWindow();
	BOOL GetToolsList(LPCTSTR lpszPath, vector<string>& arrFileNames, int pSearchType = 0);//pSearchType = 0��ѯĿ¼��pSearchType = 1��ѯ�ļ�
	void AddCourseToolsByFolderPath(string folderPath,vector<CCourseTool>& vTools);
	CVerticalLayoutUI* CreateCourseToolsGroup(string strGroupName,vector<CCourseTool>& tools);//����ѧ�ƹ�����
	bool OnToolItemClick(void* pNotify);
};
