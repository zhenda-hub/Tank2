#pragma once

#include <gdiplus.h>
#include <math.h>
#include <cmath>

/*
可绘制对象的抽象类
作用：给继承它的子类规定接口，即规定子类必须做什么
*/
class CGameEntry
{
public:
	CGameEntry() {}
	virtual ~CGameEntry() {}
public:
	virtual void Draw(Gdiplus::Graphics& gh) const = 0;

};

class CGameEntryMoveable : CGameEntry
{
	bool m_bActive{ false };//是否有效
	RectF m_rect{ 0, 0, 100, 100};//占用区域, RectF 二位坐标系的类 坦克固定100
	float m_fSpeed{10};//移动速度
	float m_direction{0};//当前角度
	//float m_directionTurn{90};//旋转角度
	float m_directionTurn{5};//旋转角度

public:
	CGameEntryMoveable() {}
	virtual ~CGameEntryMoveable() {}
	/**********拷贝构造**********CGameEntryMoveable a(b);***********************/
	CGameEntryMoveable(const CGameEntryMoveable& obj)
	{
		if (this != &obj)//this指针和地址比较
		{
			this->m_bActive = obj.m_bActive;
			this->m_rect = obj.m_rect;
			this->m_fSpeed = obj.m_fSpeed;
			this->m_direction = obj.m_direction;
			this->m_directionTurn = obj.m_directionTurn;
		}
	}
	/**********重载等号**********CGameEntryMoveable a = b;***********************/

	CGameEntryMoveable &operator=(const CGameEntryMoveable& obj)
	{
		if (this != &obj)
		{
			this->m_bActive = obj.m_bActive;
			this->m_rect = obj.m_rect;
			this->m_fSpeed = obj.m_fSpeed;
			this->m_direction = obj.m_direction;
			this->m_directionTurn = obj.m_directionTurn;
		}
		return *this;
	}

public:
	virtual void Draw(Gdiplus::Graphics& gh) const = 0;
	/**************************可移物体的属性获取器***************************/
#ifndef PI
#define PI (3.1415926f)
#endif
	virtual void SetDirectionArc(float dir)//设置角度，单位“a*PI”弧度
	{
		m_direction = dir * 180.0f / PI;
	}
	virtual void SetDirection(float dir)//设置角度，单位"度"
	{
		m_direction = dir;
	}
	virtual float GetDirectionArc() const//获取角度，单位“a*PI”弧度
	{
		return PI * m_direction / 180.0f;
	}
	virtual float GetDirection() const//获取角度，单位“度”
	{
		return m_direction;
	}
	//设置旋转角度
	virtual void SetDirectionTurnArc(float dir)//单位“a*PI”弧度
	{
		m_directionTurn = PI * dir / 180.0f;
	}
	virtual void SetDirectionTurn(float dir)//单位“度”
	{
		m_directionTurn = dir;
	}
	virtual float GetDiretionTurnArc() const//单位“a*PI”弧度
	{
		return PI * m_directionTurn / 180.0f;
	}
	virtual float GetDiretionTurn() const//单位“度”
	{
		return m_directionTurn;
	}
	//是否有效
	virtual bool IsActive() const
	{
		return m_bActive;
	}
	virtual void SetActive(bool bActive)
	{
		m_bActive = bActive;
	}
	//占用范围
	virtual void SetRect(const RectF rect)
	{
		m_rect = rect;
	}
	virtual RectF GetRect() const
	{
		return m_rect;
	}
	//移动速度
	virtual void SetSpeed(float speed)
	{
		m_fSpeed = speed;
	}
	virtual float GetSpeed() const
	{
		return m_fSpeed;
	}


	/**************************可移物体的共同方法***************************/
	virtual void RotateRight()//右转
	{
		m_direction += m_directionTurn;
		if (m_direction >= 360) {
			m_direction -= 360;
		}
	}
	virtual void RotateLeft()//左转
	{
		m_direction -= m_directionTurn;
		if (m_direction < 0) {
			m_direction = static_cast<float>(360 - m_directionTurn);
		}
	}
	virtual void Forward()//前进
	{
		m_rect.X += static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
		m_rect.Y -= static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
	}
	virtual void Backward()//后退
	{
		m_rect.X -= static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
		m_rect.Y += static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
	}
	virtual float GetXSpeed() const//X方向的速度
	{
		return static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
	}
	virtual float GetYSpeed() const//Y方向的速度
	{
		return -static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
	}
	virtual RectF ForwardNextRect() const
	{
		RectF rc = m_rect;
		rc.X += static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
		rc.Y -= static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));//Y写成了X，3小时排查。。。
		return rc;
	}
	virtual RectF BackwardNextRect() const
	{
		RectF rc = m_rect;
		rc.X -= static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
		rc.Y += static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
		return rc;
	}
	virtual PointF GetHeadPos() const//获取头部位置，发射子弹
	{
		PointF ptCenter = GetCenterPoint();
		PointF ptHead = ptCenter;
		float fRadius = static_cast<float>(std::sqrt(std::pow(m_rect.Width / 2, 2) + 
									std::pow(m_rect.Height / 2, 2)));//计算半径
		ptHead.X += fRadius * sin(GetDirectionArc());
		ptHead.Y -= fRadius * cos(GetDirectionArc());
		return ptHead;
	}
	virtual PointF GetCenterPoint()const
	{
		PointF center = PointF(m_rect.X+m_rect.Width/2, m_rect.Y+m_rect.Height/2);
		return center;
	}
	virtual void SetCenterPoint(const PointF& ptCenter)
	{
		PointF cen = GetCenterPoint();
		cen.X = ptCenter.X - cen.X;
		cen.Y = ptCenter.Y - cen.Y;
		m_rect.Offset(cen);
	}

};