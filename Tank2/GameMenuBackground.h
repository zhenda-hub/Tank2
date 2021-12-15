#pragma once
#include "GameEntry.h"
class CGame;
/********************��ʾ����ͼƬ***************************/
class CGameMenuBackground : public CGameEntryMoveable
{
public:
	CGameMenuBackground();
	virtual ~CGameMenuBackground();

	CGame* m_pParent{nullptr};//��Ķ���ָ��
	void SetParent(CGame* g)
	{
		m_pParent = g;
	}
	virtual void Draw(Graphics& gh) const; // ���Լ�

	RectF m_rect{0, 0, 800, 600};
	Image* m_imgBackground;//����ͼ
};

