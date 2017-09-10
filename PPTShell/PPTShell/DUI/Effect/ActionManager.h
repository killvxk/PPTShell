#pragma once

#include "../DUICommon.h"
#include "Action.h"
#include "ViewAnimation.h"
#include "Utility.h"
#include <math.h>
#include <set>

/*!
    \brief    ����������
*****************************************/
class  ActionManager
{
public:
	//{{
	ActionManager();
	virtual ~ActionManager();
	//}}
	static ActionManager* GetInstance();
	static void ReleaseInstance();

	//////////////////////////////////////////////////////////////////////////
	// ����Ԫ�صĶ���
	/*!
	   \brief    �Ƴ����ж���
	   \return   void 
	 ************************************/
	void removeAllActions();

	/*!
	   \brief    �ж�ָ���Ƿ��Ƴ�
	   \param    CAction * pAction 
	   \return   bool 
	 ************************************/
	bool isAction(CAction* pAction);

	/*!
	   \brief    ��Ӳ���ʼִ�ж���
	   \note     
	   \param    ViewAnimation * pAction 
	   \param    CControlUI * pView 
	   \param    bool bStopOther �Ƿ�ֹͣTarget������������������ֹͣ����
	   \return   void 
	 ************************************/
	void addViewAnimation(ViewAnimation* pAction, CControlUI* pView, bool bStopOther = true);

	/*!
	   \brief    
	   \note     
	   \param    ViewAnimation * pAction 
	   \param    CControlUI * pTarget 
	   \return   void 
	 ************************************/
	void addViewAnimationPair(ViewAnimation* pAction, CControlUI* pTarget);

	/*!
	   \brief    �Ƴ�����
	   \note     ��������ʱĬ�ϵ���
	   \param    ViewAnimation * pAction 
	   \return   void 
	 ************************************/
	void removeViewAnimation(ViewAnimation* pAction);

	/*!
	   \brief    ��������
	   \note     Target����ʱĬ��ִ��
	   \param    CControlUI * pTarget 
	   \param    bool bStop �Ƿ���ö���ֹͣ����
	   \return   void 
	 ************************************/
	void removeViewAnimationByTarget(CControlUI* pTarget, bool bStop = false);

	bool isViewAcioning(CControlUI* pTarget);
	
protected:
private:
};

