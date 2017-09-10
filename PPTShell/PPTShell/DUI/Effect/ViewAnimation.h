#pragma once
#include "../DUICommon.h"
#include "Action.h"

/*!
    \brief    ����Ԫ�صĶ���������
*****************************************/
class  ViewAnimation : public CAction
{
public:

	ViewAnimation();

	virtual ~ViewAnimation();
protected:
private:
};


/*!
    \brief    λ�ö���
	\note     ���Control��Style  
*****************************************/
class  PosChangeAnimation : public ViewAnimation
{
public:
	void SetKeyFrameCoordinate(const CRect pStartCoordinate, const CRect pEndCoordinate);

	//�̳к���
	virtual LPCTSTR GetClass();

	void	SetAnimationName(tstring strAnimationName){m_strAnimationName = strAnimationName;}
	//{{
protected:

	virtual void startWithTarget( CControlUI* pActionTarget );

	virtual void step( IActionInterval* pActionInterval );

	virtual void onStop();

	virtual void onFinished();

	CRect m_startCoordinate;
	CRect m_endCoordinate;
	tstring m_strAnimationName;
	//}}
};

/*!
    \brief    ���ж���
	\note     ���ImageStyle  
*****************************************/
class  ImageSequenceAnimation : public ViewAnimation
{
public:
	//�̳к���
	virtual LPCTSTR GetClass();
	//{{
protected:

	virtual void startWithTarget( CControlUI* pActionTarget );

	virtual void step( IActionInterval* pActionInterval );

	virtual void onStop();

	virtual void onFinished();
private:
	//}}
};
/*!
    \brief    ������������
	\note     ��Կؼ��е�����ͼƬ��ʽ
*****************************************/
class  TransitionAnimation : public ViewAnimation
{
public:
	//{{
	TransitionAnimation();
	//}}

	/*!
	   \brief    ��
	   \param    CControlUI * pOut 
	   \param    CControlUI * pIn
	   \return   void 
	 ************************************/
	void BindObj(CControlUI* pOut, CControlUI* pIn);

	/*!
	   \brief    ��ȡ�Ƴ�
	   \return   CControlUI* 
	 ************************************/
	CControlUI* GetOut();

	/*!
	   \brief    ��ȡ����
	   \return   CControlUI* 
	 ************************************/
	CControlUI* GetIn();
	//{{
protected:

	virtual void startWithTarget( CControlUI* pActionTarget );
private:
	/// 
	CControlUI* m_pOutView;
	/// 
	CControlUI* m_pInView;
	//}}
};

/*!
    \brief    ��ת����
	\note     ���ó������ؼ�������ͼƬ��ʽ����ת������ͼƬλ���ص���
*****************************************/
class  TurnTransition : public TransitionAnimation
{
public:

	virtual LPCTSTR GetClass();
	//{{
protected:

	virtual void step( IActionInterval* pActionInterval );

	virtual void onStop();

	virtual void onFinished();
	//}}
};

/*!
    \brief    ����
	\note     ���ó������ؼ�������ͼƬ��ʽ�Ļ��뻬����λ��Ϊ8|0,0,0,0��
*****************************************/
class  SlideTransition : public TransitionAnimation
{
public:
	//{{
	SlideTransition();
	//}}
	typedef enum
	{
		SlideAnimation_Left = 0,
		SlideAnimation_Top,
		SlideAnimation_Right,
		SlideAnimation_Bottom,
		SlideAnimation_LeftTop,
		SlideAnimation_RightTop,
		SlideAnimation_LeftBottom,
		SlideAnimation_RightBottom,
	}SlideType;

	void SetSlideType(SlideType slideType);

	virtual LPCTSTR GetClass();
	//{{
protected:

	virtual void step( IActionInterval* pActionInterval );

	virtual void onStop();

	virtual void onFinished();
private:
	SlideType m_SlideType;
	//}}
};

/*!
    \brief    
*****************************************/
class  FadeTransition : public TransitionAnimation
{
public:

	virtual LPCTSTR GetClass();
	//{{
protected:

	virtual void step( IActionInterval* pActionInterval );

	virtual void onStop();

	virtual void onFinished();
private:
	//}}
};