#include "pch.h"
#include "CGame.h"
#include <algorithm>
#include "GameMap.h"

//使用windowsAPI 和 与运算 创建一个宏
#define KEYDOWN(vk) (GetAsyncKeyState(vk) & (0x8000))//括号写成了中文的，排查1小时。。。

CGame::CGame()
{
	//GameInitOne2Bot();
	/*设置两个菜单类*/
	m_menuSelect.m_pParent = this;
	m_menuBackup.m_pParent = this;

}

CGame::~CGame() {}

void CGame::SetHandle(HWND hWnd)
{
	m_hWnd = hWnd;
}
bool CGame::EnterFrame(DWORD dwTime)
{
	GameRunLogic();
	GameRunDraw();
	return false;//
}

void CGame::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_eStep == EGameTypeMenu)
	{
		m_menuSelect.OnMouseMove(nFlags, point);
	}
	else
	{
		m_menuBackup.OnMouseMove(nFlags, point);
	}
	
}

void CGame::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_eStep == EGameTypeMenu)
	{
		m_menuSelect.OnLButtonUp(nFlags, point);
	}
	else
	{
		m_menuBackup.OnLButtonUp(nFlags, point);
	}
	
}

void CGame::SetStep(CGame::EGameType step)
{
	m_eStep = step;
	GameInit();
}

/*初始化方法*/

bool CGame::GameInit()
{
	srand(GetTickCount());//初始化随机数生成器
	return (this->*m_initFunc[m_eStep])();
}
bool CGame::GameInitMenu()
{
	return true;
}
bool CGame::GameInitOne2BotMenu()
{
	RECT rc;
	GetWindowRect(m_hWnd, &rc);
	PointF pt;
	pt.X = (rc.left + (rc.right-rc.left)) / 2.0f;
	pt.Y = (rc.top + (rc.bottom-rc.top)) / 2.0f;
	m_keymenu01.SetCenterPoint(pt);
	m_keymenu01.SetStop(false);
}
bool CGame::GameInitOne2Bot()
{
	for (;;)//死循环，直到地图正常生成
	{
		m_map.LoadMap();
		//玩家一
		{
			m_player01 = CPlayer(0, 0, _T("tank_player1.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("player01初始化位置失败"));
			}
			else
			{
				m_player01.SetCenterPoint(ptCenter);
			}
		}
		//机器人
		{
			m_bot = CBot(0, 0, _T("tank_bot.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("bot初始化位置失败"));
			}
			else
			{
				m_bot.SetCenterPoint(ptCenter);
			}

		}
		m_lstBullets.clear();
		//判断是否合法,（机器人和玩家的坦克不在同一区域）
		{
			int startX, startY, targetX, targetY;
			if (!m_map.FindObjPosition(m_bot, startX, startY) || 
				!m_map.FindObjPosition(m_player01, targetX, targetY))
			{
				AfxMessageBox(_T("获取坦克位置错误"));
				goto __Init_End;
			}
			VPath path;
			m_map.FindPath(startX, startY, targetX, targetY, path);
			if (!path.empty())
			{
				goto __Init_End;
			}
		}
	}
__Init_End:
	return true;

}
bool CGame::GameInitOne2BotEnd()
{
	return true;
}
bool CGame::GameInitOne2OneMenu()
{
	RECT rc;
	GetWindowRect(m_hWnd, &rc);
	PointF pt;
	pt.X = (rc.left + m_keymenu01.GetRect().Width / 2.0f + 100;
	pt.Y = (rc.top + (rc.bottom - rc.top)) / 2.0f;
	m_keymenu01.SetCenterPoint(pt);
	m_keymenu01.SetStop(false);

	pt.X = (rc.left - m_keymenu02.GetRect().Width / 2.0f - 100;
	pt.Y = (rc.top + (rc.bottom - rc.top)) / 2.0f;
	m_keymenu02.SetCenterPoint(pt);
	m_keymenu02.SetStop(false);

}
bool CGame::GameInitOne2One()
{
	for (;;)
	{
		m_map.LoadMap();
		//玩家一
		{
			m_player01 = CPlayer(0, 0, _T("tank_player1.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("player01初始化位置失败"));
			}
			else
			{
				m_player01.SetCenterPoint(ptCenter);
			}
		}
		//玩家二
		{
			m_player02 = CPlayer(0, 0, _T("tank_player2.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("player02初始化位置失败"));
			}
			else
			{
				m_player02.SetCenterPoint(ptCenter);
			}
		}

		
		m_lstBullets.clear();
		//判断是否合法,（机器人和玩家的坦克不在同一区域）
		{
			int startX, startY, targetX, targetY;
			if (!m_map.FindObjPosition(m_player02, startX, startY) ||
				!m_map.FindObjPosition(m_player01, targetX, targetY))
			{
				AfxMessageBox(_T("获取坦克位置错误"));
				break;
			}
			VPath path;
			m_map.FindPath(startX, startY, targetX, targetY, path);
			if (!path.empty())
			{
				break;
			}
		}
	}
	return true;
}
bool CGame::GameInitOne2OneEnd()
{
	return true;
}


void CGame::GameRunDraw()
{
	HDC hdc = ::GetDC(m_hWnd);//获取设备句柄

	CRect rc;
	GetClientRect(m_hWnd, &rc);
	CDC* dc = CClientDC::FromHandle(hdc);//把句柄转化为CDC对象

	CDC m_dcMemory;
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(dc, rc.Width(), rc.Height()); // 创建对象

	m_dcMemory.CreateCompatibleDC(dc);//创建设备句柄
	CBitmap* pOldBitmap = m_dcMemory.SelectObject(&bmp);//句柄选择位图

	Graphics gh(m_dcMemory.GetSafeHdc());//创建类的对象
	gh.Clear(Color::White);
	gh.ResetClip();
	//绘制
	DrawFps(gh);//绘制帧数
	//绘制图片
	{
		/*m_menu.Draw(gh);

		m_menuSelect.Draw(gh);*/

		//先绘制地图，后绘制坦克
		m_map.Draw(gh);
		m_player01.Draw(gh);
		for (auto &blt : m_lstBullets)
		{
			blt.Draw(gh);
		}

	}

	::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
	
	dc->DeleteDC();
	return;

}

void CGame::GameLogic()
{
	
	{
		if (KEYDOWN(VK_LEFT))
		{
			m_player01.RotateLeft();
		}
		if (KEYDOWN(VK_RIGHT))
		{
			m_player01.RotateRight();
		}
		if (KEYDOWN(VK_UP))
		{
			if (m_map.IsHitTheWall(m_player01, true))//和墙碰撞时的处理
			{
				m_player01.ChangeDirection(true);
				//return;
			}
			else
			{
				m_player01.Forward();
			}
		}
		if (KEYDOWN(VK_DOWN))
		{
			if (m_map.IsHitTheWall(m_player01, false))
			{
				m_player01.ChangeDirection(true);
				//return;
			}
			else
			{
				m_player01.Backward();
			}
		}
		if (KEYDOWN('M'))
		{
			CBullet blt;
			if (m_player01.Fire(blt))
			{
				m_lstBullets.push_back(blt);
			}
		}

		//子弹
		for (auto& blt : m_lstBullets)
		{
			blt.Move();
		}
		{
			//移除超时的子弹，调用了标准库的函数
			auto itRemove = std::remove_if(m_lstBullets.begin(), m_lstBullets.end(),
				[](CBullet& blt)->bool {return blt.IsTimeout(); });

			for (auto it = itRemove; it != m_lstBullets.end(); ++it)
			{
				it->SetActive(false);
				it->GetOwner()->AddBullet(*it);
			}
			m_lstBullets.erase(itRemove, m_lstBullets.end());
		}
		for (auto &blt : m_lstBullets)
		{
			m_map.HitWallProcess(blt);
			blt.Move();
		}

		for (auto &blt : m_lstBullets)
		{
			if (m_player01.IsHitted(blt))
			{
				m_player01.Bomb();
			}
		}

	}
}

bool CGame::GameInitOne2Bot()
{
	m_map.LoadMap();
	m_player01 = CPlayer(0, 0, _T("tank_player1.png"));
	PointF ptCenter;
	if (!m_map.FindRandomPosition(ptCenter))
	{
		AfxMessageBox(_T("玩家一的起始位置获取失败"));
		return false;
	}
	else
	{
		m_player01.SetCenterPoint(ptCenter);
	}
	m_lstBullets.clear();//清理子弹
	return true;
}


void CGame::drawfps(graphics& gh)
{
	static int fps = 0;
	m_fps++;
	static dword dwlast = gettickcount();
	if (gettickcount() - dwlast >= 1000)
	{
		fps = m_fps;
		m_fps = 0;
		dwlast = gettickcount();

	}

	{
		cstring s;
		s.format(_t("fps:%d"), fps);
		solidbrush brush(color(0, 0, 0xff));
		gdiplus::font font(_t("宋体"), 10.0);
		crect rc;
		::getclientrect(m_hwnd, &rc);
		pointf origin(static_cast<float>(rc.right - 50), static_cast<float>(rc.top + 2));
		gh.drawstring(s.getstring(), -1, &font, origin, &brush);
	}
}
