#include "StdAfx.h"
#include "InteractiveToolsUI.h"
#include "Resource.h"
#include "Plugins/Icr/IcrPlayer.h"

CInteractiveToolsUI::CInteractiveToolsUI(void):m_pContainer(NULL)
{
	GetModuleFileName(NULL,m_szCurDir,MAX_PATH);
	char *pTmp		= strrchr(m_szCurDir, '\\');
	if (pTmp != NULL)
	{
		*(pTmp) = 0;
	}
	maxWidth = 373;
	maxHeight = 712;
}

CInteractiveToolsUI::~CInteractiveToolsUI(void)
{
}

LPCTSTR CInteractiveToolsUI::GetWindowClassName() const
{
	return _T("InteractiveToolsUI");
}

DuiLib::CDuiString CInteractiveToolsUI::GetSkinFile()
{
	return _T("InteractiveTools\\InteractiveTools.xml");
}

DuiLib::CDuiString CInteractiveToolsUI::GetSkinFolder()
{
	return _T("skins");
}

void CInteractiveToolsUI::InitWindow()
{
	::SetWindowLong(this->GetHWND(), GWL_EXSTYLE, WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_CONTROLPARENT);
	m_pContainer = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("container")));
}

void CInteractiveToolsUI::LoadTools()
{
	totalHeight = 0;
	totalWidth = 0;
	m_pContainer->RemoveAll();
	m_mapInteractiveTools.clear();
	//�Ŵ�
	vector<CInteractiveTool> vTools;
	CInteractiveTool toolMagnifier;
	toolMagnifier.SetName("�Ŵ�");
	string imagePath = m_szCurDir;
	imagePath+="\\Skins\\InteractiveTools\\hdgj_magnifier.png";
	toolMagnifier.SetImagePath(imagePath);
	toolMagnifier.SetType(Magnifier);
	vTools.push_back(toolMagnifier);
	m_mapInteractiveTools.insert(map<string,vector<CInteractiveTool>> :: value_type("����",vTools));
	//���ù���
	vector<CInteractiveTool> vCourseTools;
	//������
	CInteractiveTool toolRoster;
	toolRoster.SetName("������");
	imagePath = m_szCurDir;
	imagePath+="\\Skins\\InteractiveTools\\hdgj_hmc.png";
	toolRoster.SetImagePath(imagePath);
	toolRoster.SetType(Roster);
	vCourseTools.push_back(toolRoster);
	//����
	CInteractiveTool toolPushScreen;
	toolPushScreen.SetName("����");
	imagePath = m_szCurDir;
	imagePath+="\\Skins\\InteractiveTools\\hdgj_tp.png";
	toolPushScreen.SetImagePath(imagePath);
	toolPushScreen.SetType(PushScreen);
	vCourseTools.push_back(toolPushScreen);
	//������ϰ
	CInteractiveTool toolCourseTraining;
	toolCourseTraining.SetName("������ϰ");
	imagePath = m_szCurDir;
	imagePath+="\\Skins\\InteractiveTools\\hdgj_fsxt.png";
	toolCourseTraining.SetImagePath(imagePath);
	toolCourseTraining.SetType(CourseTraining);
	vCourseTools.push_back(toolCourseTraining);
	//�����ܽ�
	CInteractiveTool toolCourseSummary;
	toolCourseSummary.SetName("�����ܽ�");
	imagePath = m_szCurDir;
	imagePath+="\\Skins\\InteractiveTools\\hdgj_ktzj.png";
	toolCourseSummary.SetImagePath(imagePath);
	toolCourseSummary.SetType(CourseSummary);
	vCourseTools.push_back(toolCourseSummary);
	m_mapInteractiveTools.insert(map<string,vector<CInteractiveTool>> :: value_type("��������",vCourseTools));
	//��ʾ����
	int itemCountPerRow = 4;//ÿ�и���
	for(map<string,vector<CInteractiveTool>>::iterator iter = m_mapInteractiveTools.begin(); iter != m_mapInteractiveTools.end();iter++)
	{
		if(iter->second.size()>0)
		{
			CVerticalLayoutUI* layoutGroup = CreateCourseToolsGroup(iter->first,iter->second);
			if(layoutGroup)
			{
				m_pContainer->Add(layoutGroup);
			}
		}
	}
	if(totalHeight+38<maxHeight)
	{
		totalHeight+=38;
	}
	m_pContainer->SetFixedHeight(totalHeight);
	m_pContainer->SetFixedWidth(totalWidth);
	//
	if(totalHeight>=maxHeight)
	{
		::SetWindowPos(this->GetHWND(),NULL,-10000, -10000, maxWidth,maxHeight,SWP_NOACTIVATE|SWP_SHOWWINDOW);
	}
	else
	{
		::SetWindowPos(this->GetHWND(),NULL,-10000, -10000, totalWidth,totalHeight,SWP_NOACTIVATE|SWP_SHOWWINDOW);
	}
}

CVerticalLayoutUI* CInteractiveToolsUI::CreateCourseToolsGroup( string strGroupName,vector<CInteractiveTool>& tools )
{
	CVerticalLayoutUI* layoutGroup = NULL;
	if(tools.size()>0)
	{
		try
		{
			if( !m_builderGroup.GetMarkup()->IsValid() ) {
				layoutGroup = dynamic_cast<CVerticalLayoutUI*>(m_builderGroup.Create(_T("InteractiveTools\\InteractiveToolsGroup.xml"), (UINT)0, this, &m_PaintManager));
			}
			else {
				layoutGroup = dynamic_cast<CVerticalLayoutUI*>(m_builderGroup.Create(this, &m_PaintManager));
			}
			if(layoutGroup)
			{
				int itemCountPerRow = 4;//ÿ�и���
				int lines = tools.size() / itemCountPerRow;
				int count = tools.size() % itemCountPerRow;
				if(count>0)
				{
					lines++;
				}
				//����������ĸ߶�
				int groupHeight = 38+lines*70+lines-1;
				totalHeight += groupHeight;
				layoutGroup->SetFixedHeight(groupHeight);
				layoutGroup->SetFixedWidth(itemCountPerRow*70+itemCountPerRow-1);
				totalWidth = itemCountPerRow*70+itemCountPerRow-1;
				//������ͷ
				CLabelUI* lblGroupName = static_cast<CLabelUI*>(layoutGroup->FindSubControl(_T("lblGroupName")));
				if(lblGroupName)
				{
					lblGroupName->SetText(strGroupName.c_str());
					lblGroupName->SetFont(160100);
				}
				//�������ڹ���
				CVerticalLayoutUI* layGroupTools = static_cast<CVerticalLayoutUI*>(layoutGroup->FindSubControl(_T("containerGroupTools")));
				if(layGroupTools)
				{
					for(int i=0;i<lines;i++)//���ÿһ��tool
					{
						CHorizontalLayoutUI* lineToolsLayout = new CHorizontalLayoutUI;
						for (int j=0;j<itemCountPerRow;j++)
						{
							//���Item
							CContainerUI* item = NULL;
							if( !m_builderItem.GetMarkup()->IsValid() ) {
								item = dynamic_cast<CContainerUI*>(m_builderItem.Create(_T("InteractiveTools\\InteractiveToolsItem.xml"), (UINT)0, this, &m_PaintManager));
							}
							else {
								item = dynamic_cast<CContainerUI*>(m_builderItem.Create(this, &m_PaintManager));
							}
							if(item)
							{
								item->OnNotify+=MakeDelegate(this,&CInteractiveToolsUI::OnToolItemClick);
								if(i*itemCountPerRow+j<tools.size())//����tag������ͼƬ�ı���ʾ
								{
									CInteractiveTool* toolPtr = &(tools[i*itemCountPerRow+j]);
									item->SetTag((UINT_PTR)toolPtr);
									CControlUI* ctrGroupItemImage = static_cast<CControlUI*>(item->FindSubControl(_T("ctrItemImage")));
									if(ctrGroupItemImage)
									{
										ctrGroupItemImage->SetBkImage(tools[i*itemCountPerRow+j].GetImagePath().c_str());
									}
									CLabelUI* lblGroupItemName = static_cast<CLabelUI*>(item->FindSubControl(_T("lblItemName")));
									if(lblGroupItemName)
									{
										lblGroupItemName->SetText(tools[i*itemCountPerRow+j].GetName().c_str());
										lblGroupItemName->SetFont(120000);
										lblGroupItemName->SetTextColor(0x99FFFFFF);
									}
								}
								lineToolsLayout->Add(item);
							}
							//����������һ�У������һ���ָ�����
							if(j<itemCountPerRow-1)
							{
								CControlUI* ctrVerticalLine = new CControlUI;
								ctrVerticalLine->SetBkColor(0xFF545454);
								ctrVerticalLine->SetFixedWidth(1);
								ctrVerticalLine->SetFixedHeight(70);
								lineToolsLayout->Add(ctrVerticalLine);
							}
						}
						layGroupTools->Add(lineToolsLayout);
						//����������һ�У������һ���ָ����
						if(i<lines-1)
						{
							CControlUI* ctrHorizontalLine = new CControlUI;
							ctrHorizontalLine->SetBkColor(0xFF545454);
							ctrHorizontalLine->SetFixedHeight(1);
							layGroupTools->Add(ctrHorizontalLine);
						}
					}
				}
			}
		}
		catch (...)
		{
			layoutGroup = NULL;
		}
	}
	return layoutGroup;
}

bool CInteractiveToolsUI::OnToolItemClick( void* pNotify )
{
	TNotifyUI* msg = (TNotifyUI*)pNotify;
	if (msg&&msg->sType == _T("setfocus"))
	{
		if(msg->pSender!=NULL && msg->pSender->GetTag()!=NULL)
		{
			CInteractiveTool* tool = (CInteractiveTool*)(msg->pSender->GetTag());
			if(tool)
			{
				try
				{
					if(tool->GetType()==Magnifier)//�Ŵ�
					{
	
					}
					else if (tool->GetType()==Roster)//������
					{
						IcrPlayer::GetInstance()->IcrOnClick(ICR_BUTTON_ID_ROSTER);
					}
					else if (tool->GetType()==PushScreen)//����
					{
						IcrPlayer::GetInstance()->IcrOnClick(ICR_BUTTON_ID_PUSHSCREEN);
					}
					else if (tool->GetType()==CourseTraining)//������ϰ
					{
						IcrPlayer::GetInstance()->IcrOnClick(ICR_BUTTON_ID_EXAM);
					}
					else if (tool->GetType()==CourseSummary)//�����ܽ�
					{
						IcrPlayer::GetInstance()->IcrOnClick(ICR_BUTTON_ID_SUMMARY);
					}
				}
				catch(...)
				{

				}
			}
			this->m_pContainer->SetFocus();
			this->ShowWindow(false);
		}
	}
	return true;
}
