#pragma once

#include <gdiplus.h>
#include <math.h>
#include <cmath>

/*
�ɻ��ƶ���ĳ�����
���ã����̳���������涨�ӿڣ����涨���������ʲô
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
	bool m_bActive{ false };//�Ƿ���Ч
	RectF m_rect{ 0, 0, 100, 100};//ռ������, RectF ��λ����ϵ���� ̹�˹̶�100
	float m_fSpeed{10};//�ƶ��ٶ�
	float m_direction{0};//��ǰ�Ƕ�
	//float m_directionTurn{90};//��ת�Ƕ�
	float m_directionTurn{5};//��ת�Ƕ�

public:
	CGameEntryMoveable() {}
	virtual ~CGameEntryMoveable() {}
	/**********��������**********CGameEntryMoveable a(b);***********************/
	CGameEntryMoveable(const CGameEntryMoveable& obj)
	{
		if (this != &obj)//thisָ��͵�ַ�Ƚ�
		{
			this->m_bActive = obj.m_bActive;
			this->m_rect = obj.m_rect;
			this->m_fSpeed = obj.m_fSpeed;
			this->m_direction = obj.m_direction;
			this->m_directionTurn = obj.m_directionTurn;
		}
	}
	/**********���صȺ�**********CGameEntryMoveable a = b;***********************/

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
	/**************************������������Ի�ȡ��***************************/
#ifndef PI
#define PI (3.1415926f)
#endif
	virtual void SetDirectionArc(float dir)//���ýǶȣ���λ��a*PI������
	{
		m_direction = dir * 180.0f / PI;
	}
	virtual void SetDirection(float dir)//���ýǶȣ���λ"��"
	{
		m_direction = dir;
	}
	virtual float GetDirectionArc() const//��ȡ�Ƕȣ���λ��a*PI������
	{
		return PI * m_direction / 180.0f;
	}
	virtual float GetDirection() const//��ȡ�Ƕȣ���λ���ȡ�
	{
		return m_direction;
	}
	//������ת�Ƕ�
	virtual void SetDirectionTurnArc(float dir)//��λ��a*PI������
	{
		m_directionTurn = PI * dir / 180.0f;
	}
	virtual void SetDirectionTurn(float dir)//��λ���ȡ�
	{
		m_directionTurn = dir;
	}
	virtual float GetDiretionTurnArc() const//��λ��a*PI������
	{
		return PI * m_directionTurn / 180.0f;
	}
	virtual float GetDiretionTurn() const//��λ���ȡ�
	{
		return m_directionTurn;
	}
	//�Ƿ���Ч
	virtual bool IsActive() const
	{
		return m_bActive;
	}
	virtual void SetActive(bool bActive)
	{
		m_bActive = bActive;
	}
	//ռ�÷�Χ
	virtual void SetRect(const RectF rect)
	{
		m_rect = rect;
	}
	virtual RectF GetRect() const
	{
		return m_rect;
	}
	//�ƶ��ٶ�
	virtual void SetSpeed(float speed)
	{
		m_fSpeed = speed;
	}
	virtual float GetSpeed() const
	{
		return m_fSpeed;
	}


	/**************************��������Ĺ�ͬ����***************************/
	virtual void RotateRight()//��ת
	{
		m_direction += m_directionTurn;
		if (m_direction >= 360) {
			m_direction -= 360;
		}
	}
	virtual void RotateLeft()//��ת
	{
		m_direction -= m_directionTurn;
		if (m_direction < 0) {
			m_direction = static_cast<float>(360 - m_directionTurn);
		}
	}
	virtual void Forward()//ǰ��
	{
		m_rect.X += static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
		m_rect.Y -= static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
	}
	virtual void Backward()//����
	{
		m_rect.X -= static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
		m_rect.Y += static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
	}
	virtual float GetXSpeed() const//X������ٶ�
	{
		return static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
	}
	virtual float GetYSpeed() const//Y������ٶ�
	{
		return -static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
	}
	virtual RectF ForwardNextRect() const
	{
		RectF rc = m_rect;
		rc.X += static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
		rc.Y -= static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));//Yд����X��3Сʱ�Ų顣����
		return rc;
	}
	virtual RectF BackwardNextRect() const
	{
		RectF rc = m_rect;
		rc.X -= static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
		rc.Y += static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
		return rc;
	}
	virtual PointF GetHeadPos() const//��ȡͷ��λ�ã������ӵ�
	{
		PointF ptCenter = GetCenterPoint();
		PointF ptHead = ptCenter;
		float fRadius = static_cast<float>(std::sqrt(std::pow(m_rect.Width / 2, 2) + 
									std::pow(m_rect.Height / 2, 2)));//����뾶
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