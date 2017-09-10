#pragma once
class  Interpolator
{
public:
	typedef enum{
		TWEEN_Linear = 0,
		TWEEN_Quadratic,
		TWEEN_Cubic,
		TWEEN_Quartic,
		TWEEN_Quintic,
		TWEEN_Sinusoidal,
		TWEEN_Exponential,
		TWEEN_Circular,
		TWEEN_Back,
		TWEEN_Bounce,
		TWEEN_Elastic,
	}TWEENTYPE;

	typedef enum{ 
		EASEIN = 0, 
		EASEOUT, 
		EASEINOUT 
	} EaseType;

public:
	Interpolator(void);
	~Interpolator(void);

	//////////////////////////////////////////////////////////////////////////
	// TweenFunc
	// @twType			��������
	// @t						��ǰ֡
	// @b					��ʼλ��
	// @c					�ƶ����ܾ���
	// @d					��֡��
	// @iEaseType		��������( easeIn=0,easeOut=1,easeInOut=2)
	// @s					iEaseType = TWEEN_Backʱ�Ĳ�����Ĭ��ֵΪ1.70158
	// @a					iEaseType = TWEEN_Elasticʱ�Ĳ�����Ĭ��ֵ����c
	// @p					iEaseType = TWEEN_Elasticʱ�Ĳ�����Ĭ��ֵ����0.3*d
	// return					��ǰλ��
	//////////////////////////////////////////////////////////////////////////
	static float getInterpolator(int twType, float t, float b, float c, float d, int iEaseType = 0, float s = 1.70158);
};
