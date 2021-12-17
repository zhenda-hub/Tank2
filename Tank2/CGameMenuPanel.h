#pragma once
#include <vector>
#include <tuple>
#include "GameEntry.h"

using namespace std;

class CGame;
/*********************ѡ��˵�����Ӧ**************************/
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

	void OnMouseMove(UINT nFlags, CPoint point);//�������
	void OnLButtonUp(UINT nFlags, CPoint point);//���̧��
	Gdiplus::Image* m_imgBackground;
	struct MyStruct
	{
		vector<tuple<Gdiplus::Image*, RectF>> vImageInfoPtr;//ͼƬ��ָ�룬ͼƬ�ķ�Χ
		int index;
	}m_menuItems[2];
};

