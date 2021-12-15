#pragma once
#include "GameEntry.h"
class CGame;
/********************显示背景图片***************************/
class CGameMenuBackground : public CGameEntryMoveable
{
public:
	CGameMenuBackground();
	virtual ~CGameMenuBackground();

	CGame* m_pParent{nullptr};//类的对象指针
	void SetParent(CGame* g)
	{
		m_pParent = g;
	}
	virtual void Draw(Graphics& gh) const; // 画自己

	RectF m_rect{0, 0, 800, 600};
	Image* m_imgBackground;//背景图
};

