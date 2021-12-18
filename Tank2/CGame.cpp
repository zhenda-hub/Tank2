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

/*******************************************初始化方法*******************************************/

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
	pt.X = rc.left + (rc.right-rc.left) / 2.0f;
	pt.Y = rc.top + (rc.bottom-rc.top) / 2.0f;
	m_keymenu01.SetCenterPoint(pt);
	m_keymenu01.SetStop(false);
	return true;
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
	pt.X = rc.left + m_keymenu01.GetRect().Width / 2.0f + 100;
	pt.Y = (rc.top + (rc.bottom - rc.top)) / 2.0f;
	m_keymenu01.SetCenterPoint(pt);
	m_keymenu01.SetStop(false);

	pt.X = rc.right - m_keymenu02.GetRect().Width / 2.0f - 100;//copy后，忘记修改right，排查半小时
	//pt.X = rc.left - m_keymenu02.GetRect().Width / 2.0f - 100;
	pt.Y = (rc.top + (rc.bottom - rc.top)) / 2.0f;
	m_keymenu02.SetCenterPoint(pt);
	m_keymenu02.SetStop(false);
	return true;
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

/*******************************************逻辑方法*******************************************/

void CGame::GameRunLogic()
{
	(this->*m_logicFunc[m_eStep])();
}
void CGame::GameRunLogicOnMenu()
{

}
void CGame::GameRunLogicOnOne2BotMenu()
{
	if (KEYDOWN('M'))//按下M,停止提示框
	{
		m_keymenu01.SetStop();
	}
	if (m_keymenu01.GetStop())
	{
		SetStep(EGameTypeOne2Bot);
	}
}
void CGame::GameRunLogicOnOne2Bot()
{
	/************状态维护*******************/

	RemoveTimeoutBullets();//移除无效的子弹，给坦克装子弹
	for (auto &blt : m_lstBullets)
	{
		if (m_bot.IsHitted(blt))
		{
			m_bot.Bomb();
			m_eStep = EGameTypeOne2BotEnd;
			blt.SetActive(false);
		}
		if (m_player01.IsHitted(blt))
		{
			m_player01.Bomb();
			m_eStep = EGameTypeOne2BotEnd;
			blt.SetActive(false);
		}

	}

	ProcessHitBullets();//子弹运行维护
	AI();

	/******************按键处理*****************/

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
		if (KEYDOWN(VK_NUMPAD0))
		{
			CBullet blt;
			if (m_player01.Fire(blt))//坦克发射子弹
			{
				m_lstBullets.push_back(blt);//加入子弹列表
			}
		}

		//if (KEYDOWN('I'))//控制机器人， 测试方法
		//{
		//	int startX, startY, targetX, targetY;
		//	if (!m_map.FindObjPosition(m_bot, startX, startY) || 
		//		!m_map.FindObjPosition(m_player01, targetX, targetY))
		//	{
		//		return;
		//	}
		//	float fDirNext = 0;//机器人下一步方向
		//	if (!m_map.FindNextDirection(&fDirNext, startX, startY, targetX, targetY))
		//	{
		//		return;
		//	}

		//	//获取bot坦克的中心点
		//	PointF ptTankCenter = m_bot.GetCenterPoint();
		//	PointF ptAreaCenter = m_map.GetElementAreaCenter(startX, startY);
		//	RectF rc(ptAreaCenter.X-5, ptAreaCenter.Y-5, 10, 10);
		//	if (!rc.Contains(ptTankCenter))
		//	{
		//		m_bot.Forward();
		//		return;
		//	}
		//	else
		//	{
		//		m_bot.SetDirection(fDirNext);
		//		m_bot.Forward();
		//	}

		

	}
}
void CGame::GameRunLogicOnOne2BotEnd()
{
	/******************状态维护********************/
	RemoveTimeoutBullets();
	ProcessHitBullets();
	/******************按键处理：没有*******************/

}
void CGame::GameRunLogicOnOne2OneMenu()
{
	if (KEYDOWN('M'))//按下M,停止提示框
	{
		m_keymenu01.SetStop();
	}
	if (KEYDOWN('Q'))//按下Q,停止提示框
	{
		m_keymenu02.SetStop();
	}

	if (m_keymenu01.GetStop() && m_keymenu02.GetStop())
	{
		SetStep(EGameTypeOne2One);
	}
}
void CGame::GameRunLogicOnOne2One()
{

	/************状态维护*******************/

	RemoveTimeoutBullets();//移除无效的子弹，给坦克装子弹
	for (auto& blt : m_lstBullets)
	{
		if (!blt.IsActive())
		{
			continue;
		}

		if (m_player01.IsHitted(blt))
		{
			m_player01.Bomb();
			m_eStep = EGameTypeOne2OneEnd;
			blt.SetActive(false);
		}
		if (m_player02.IsHitted(blt))
		{
			m_player02.Bomb();
			m_eStep = EGameTypeOne2OneEnd;
			blt.SetActive(false);
		}
	}

	ProcessHitBullets();//子弹运行维护

	/******************按键处理*****************/

	{
		/******************玩家一*****************/

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
				//m_player01.ChangeDirection(true);
				return;
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
				//m_player01.ChangeDirection(true);
				return;
			}
			else
			{
				m_player01.Backward();
			}
		}
		if (KEYDOWN(VK_NUMPAD0))
		{
			CBullet blt;
			if (m_player01.Fire(blt))//坦克发射子弹
			{
				m_lstBullets.push_back(blt);//加入子弹列表
			}
		}

		/******************玩家二*****************/
		if (KEYDOWN('A'))
		{
			m_player02.RotateLeft();
		}
		if (KEYDOWN('D'))
		{
			m_player02.RotateRight();
		}
		if (KEYDOWN('W'))
		{
			if (m_map.IsHitTheWall(m_player02, true))//和墙碰撞时的处理
			{
				m_player02.ChangeDirection(true);
				//return;
			}
			else
			{
				m_player02.Forward();
			}
		}
		if (KEYDOWN('S'))
		{
			if (m_map.IsHitTheWall(m_player02, false))
			{
				m_player02.ChangeDirection(true);
				//return;
			}
			else
			{
				m_player02.Backward();
			}
		}
		if (KEYDOWN(VK_SPACE))
		{
			CBullet blt;
			if (m_player02.Fire(blt))//坦克发射子弹
			{
				m_lstBullets.push_back(blt);//加入子弹列表
			}
		}
		if (KEYDOWN('Z'))
		{
			if (m_map.IsCanKillTarget(m_player01, m_player02))
			{
				AfxMessageBox(_T("可以达到"));
			}
		}


	}
		
}
void CGame::GameRunLogicOnOne2OneEnd()
{
	/******************状态维护********************/
	RemoveTimeoutBullets();
	ProcessHitBullets();
	/******************按键处理：没有*******************/
}


/*******************************************绘图方法*******************************************/


void CGame::GameRunDraw()
{
	HDC hdc = ::GetDC(m_hWnd);//获取设备句柄

	CRect rc;
	GetClientRect(m_hWnd, &rc);
	//双缓冲绘图
	CDC* dc = CClientDC::FromHandle(hdc);//把句柄转化为CDC对象
	CBitmap bmp;
	CDC m_dcMemory;
	{
		bmp.CreateCompatibleBitmap(dc, rc.Width(), rc.Height()); // 创建对象
		m_dcMemory.CreateCompatibleDC(dc);//创建设备句柄
		CBitmap* pOldBitmap = m_dcMemory.SelectObject(&bmp);//句柄选择位图
	}


	Graphics gh(m_dcMemory.GetSafeHdc());//创建类的对象
	gh.Clear(Color::White);
	gh.ResetClip();
	

	//根据场景绘制图片
	{
		(this->*m_drawFunc[m_eStep])(gh);

	}

	::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
	
	//dc->DeleteDC();
	::ReleaseDC(m_hWnd, hdc);
	return;

}

void CGame::GameRunDrawOnMenu(Graphics& gh)
{
	m_menuSelect.Draw(gh);
}
void CGame::GameRunDrawOnOne2BotMenu(Graphics& gh)
{
	m_menu.Draw(gh);
	m_keymenu01.Draw(gh);
}
void CGame::GameRunDrawOnOne2Bot(Graphics& gh)
{
	m_menuBackup.Draw(gh);//返回菜单？
	m_map.Draw(gh);//画地图
	m_bot.Draw(gh);//机器人
	m_player01.Draw(gh);//玩家1
	DrawFps(gh);//FPS
	for (auto b : m_lstBullets)
	{
		b.Draw(gh);//子弹
	}
}
void CGame::GameRunDrawOnOne2BotEnd(Graphics& gh)
{
	m_menuBackup.Draw(gh);//返回菜单？
	m_map.Draw(gh);//画地图
	m_bot.Draw(gh);//机器人
	m_player01.Draw(gh);//玩家1
	DrawFps(gh);//FPS
	for (auto b : m_lstBullets)
	{
		b.Draw(gh);//子弹
	}

	if (m_player01.IsBombEnd() || m_bot.IsBombEnd())//游戏结束返回菜单
	{
		m_eStep = EGameTypeMenu;
	}
}
void CGame::GameRunDrawOnOne2OneMenu(Graphics& gh)
{
	m_menu.Draw(gh);
	m_keymenu01.Draw(gh);
	m_keymenu02.Draw(gh);

}
void CGame::GameRunDrawOnOne2One(Graphics& gh)
{


	m_menuBackup.Draw(gh);//返回菜单？
	m_map.Draw(gh);//画地图
	m_player02.Draw(gh);//玩家2
	m_player01.Draw(gh);//玩家1
	DrawFps(gh);//FPS
	for (auto b : m_lstBullets)
	{
		b.Draw(gh);//子弹
	}
}
void CGame::GameRunDrawOnOne2OneEnd(Graphics& gh)
{
	m_menuBackup.Draw(gh);//返回菜单？
	m_map.Draw(gh);//画地图
	m_player02.Draw(gh);//玩家2
	m_player01.Draw(gh);//玩家1
	DrawFps(gh);//FPS
	for (auto b : m_lstBullets)
	{
		b.Draw(gh);//子弹
	}
	if (m_player01.IsBombEnd() || m_player02.IsBombEnd())//游戏结束返回菜单
	{
		m_eStep = EGameTypeMenu;
	}
}

/*******************************************其他方法*******************************************/

void CGame::DrawFps(Graphics& gh)
{
	static int fps = 0;
	m_fps++;
	static DWORD dwlast = GetTickCount();
	if (GetTickCount() - dwlast >= 1000)
	{
		fps = m_fps;
		m_fps = 0;
		dwlast = GetTickCount();

	}

	{
		CString s;
		s.Format(_T("fps:%d"), fps);
		SolidBrush brush(Color(0, 0, 0xff));
		Gdiplus::Font font(_T("宋体"), 10.0);
		CRect rc;
		::GetClientRect(m_hWnd, &rc);
		PointF origin(static_cast<float>(rc.right - 50), static_cast<float>(rc.top + 2));
		gh.DrawString(s.GetString(), -1, &font, origin, &brush);
	}
}

void CGame::RemoveTimeoutBullets()
{
	auto itRemote = std::remove_if(m_lstBullets.begin(), m_lstBullets.end(),
		[](CBullet& blt)->bool {return blt.IsTimeout();});//移除超时的子弹
	for (auto it = itRemote; it != m_lstBullets.end(); ++it)
	{
		it->SetActive(false);
		it->GetOwner()->AddBullet(*it);
	}
	m_lstBullets.erase(itRemote, m_lstBullets.end());
}
void CGame::ProcessHitBullets()
{
	for (auto &blt : m_lstBullets)
	{
		m_map.HitWallProcess(blt);
		blt.Move();
	}
}
void CGame::AI()
{
	static CGameTimer acTimer(-1, 150);//一个定时器时间间隔
	if (acTimer.IsTimeval())
	{
		int startX, startY, targetX, targetY;
		if (!m_map.FindObjPosition(m_bot, startX, startY) ||
			!m_map.FindObjPosition(m_player01, targetX, targetY))
		{
			return;
		}

		float fDirNext = 0;
		//if (!m_map.FindNextDirection(&fDirNext, startX, startY, targetX, targetY));//多写了分号，排查2小时
		if (!m_map.FindNextDirection(&fDirNext, startX, startY, targetX, targetY))
		{
			return;
		}

		PointF ptTankCenter = m_bot.GetCenterPoint();//机器人中心点
		PointF ptAreaCenter = m_map.GetElementAreaCenter(startX, startY);//地图中的空格中心点
		RectF rc(ptAreaCenter.X-5, ptAreaCenter.Y-5, 10, 10);
		//没到中心点，就前进
		if (!rc.Contains(ptTankCenter))
		{
			m_bot.Forward();
			return;
		}
		else
		{
			m_bot.SetDirection(fDirNext);
			float dir;
			if (m_map.IsCanKillTarget(m_bot, m_player01, &dir))
			{
				CBullet blt;
				if (m_bot.Fire(blt))
				{
					m_lstBullets.push_back(blt);
				}

				return;
			}
			m_bot.Forward();
		}

	}
}