#include "pch.h"
#include "CGameMenuPanel.h"
#include "CGame.h"

CGameMenuPanel::CGameMenuPanel()
{
	//背景图
	m_imgBackground = Gdiplus::Image::FromFile(_T("menu_background.png"));
	//菜单一：人机大战
	m_menuItems[0].index = 0;
	for (int i = 0; i < 2; i++)
	{
		TCHAR path[MAX_PATH];
		_stprintf_s(path, _T("menu_1player_%d.png"), i);//处理文件名
		auto imgPtr = Gdiplus::Image::FromFile(path);//载入图片
		RectF rc(0, 300, static_cast<float>(imgPtr->GetWidth()),
					static_cast<float>(imgPtr->GetHeight()));
		m_menuItems[0].vImageInfoPtr.push_back(make_tuple(imgPtr, rc));//结构体的vector里添加东西

	}
	//菜单二： 双人大战
	m_menuItems[1].index = 0;
	for (int i = 0; i < 2; i++)
	{
		TCHAR path[MAX_PATH];
		_stprintf_s(path, _T("menu_2player_%d.png"), i);//处理文件名
		auto imgPtr = Gdiplus::Image::FromFile(path);//载入图片
		RectF rc(400, 300, static_cast<float>(imgPtr->GetWidth()),
			static_cast<float>(imgPtr->GetHeight()));
		m_menuItems[1].vImageInfoPtr.push_back(make_tuple(imgPtr, rc));//结构体的vector里添加东西

	}
}

CGameMenuPanel::~CGameMenuPanel()
{

}

void CGameMenuPanel::Draw(Graphics& gh) const
{
	//画背景图片
	gh.DrawImage(m_imgBackground, m_rect);
	//画子菜单
	for (auto menuItem : m_menuItems)
	{
		auto img = get<0>(menuItem.vImageInfoPtr[menuItem.index]);
		auto rect = get<1>(menuItem.vImageInfoPtr[menuItem.index]);
		gh.DrawImage(img, rect);
	}
}

void CGameMenuPanel::OnMouseMove(UINT nFlags, CPoint point)//第2个参数，鼠标位置
{
	PointF pt(static_cast<float>(point.x), static_cast<float>(point.y));
	//画子菜单
	for (auto &menuItem : m_menuItems)
	{
		auto img = get<0>(menuItem.vImageInfoPtr[menuItem.index]);
		auto rect = get<1>(menuItem.vImageInfoPtr[menuItem.index]);
		if (rect.Contains(pt))//判断是否包含当前鼠标的位置
		{
			menuItem.index = 1;
		} 
		else
		{
			menuItem.index = 0;
		}
	}
}
void CGameMenuPanel::OnLButtonUp(UINT nFlags, CPoint point)
{
	PointF pt(static_cast<float>(point.x), static_cast<float>(point.y));
	{
		auto& menuItem = m_menuItems[0];
		auto img = get<0>(menuItem.vImageInfoPtr[menuItem.index]);
		auto rect = get<1>(menuItem.vImageInfoPtr[menuItem.index]);
		if (rect.Contains(pt))
		{
			//人机大战，随后处理
			//AfxMessageBox(_T("鼠标左键抬起1"));
			m_pParent->SetStep(CGame::EGameTypeOne2BotMenu);
			return;
		}
	}
	{
		auto& menuItem = m_menuItems[1];
		auto img = get<0>(menuItem.vImageInfoPtr[menuItem.index]);
		auto rect = get<1>(menuItem.vImageInfoPtr[menuItem.index]);
		if (rect.Contains(pt))
		{
			//双人大战，随后处理
			//AfxMessageBox(_T("鼠标左键抬起2"));
			m_pParent->SetStep(CGame::EGameTypeOne2OneMenu);
			return;
		}
	}
}
