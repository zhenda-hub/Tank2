#pragma once
#include <vector>
#include <tuple>
#include "GameEntry.h"

using namespace std;

class CGame;
/*********************选择菜单的响应**************************/
class CGameMenuPanel: public CGameEntryMoveable
{
public:
	CGameMenuPanel();
	virtual ~CGameMenuPanel();

	CGame* m_pParent{nullptr};
	void SetParent(CGame* g)
	{
		m_pParent = g;
	}

	virtual void Draw(Graphics& gh) const;

	Gdiplus::RectF m_rect{ 0, 0, 800, 600 };

	void OnMouseMove(UINT nFlags, CPoint point);//鼠标悬浮
	void OnLButtonUp(UINT nFlags, CPoint point);//左键抬起
	Gdiplus::Image* m_imgBackground;
	struct MyStruct
	{
		vector<tuple<Gdiplus::Image*, RectF>> vImageInfoPtr;//图片的指针，图片的范围
		int index;
	}m_menuItems[2];
};

