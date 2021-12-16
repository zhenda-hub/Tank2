#pragma once
//整体业务流程： 系统 -> 界面 -> 核心逻辑
#include "CGameMenuPanel.h"
#include "GameMenuBackground.h"
#include "TankEntry.h"
#include "Player.h"
#include "Bullet.h"
#include "Bot.h"

class CGame
{
	//基本属性
	HWND m_hWnd;//窗口

	void GameRunDraw();//绘图
	void GameLogic();
	//绘制FPS
	void DrawFps(Graphics& gh);
	int m_fps{0};
	//开始菜单
	CGameMenuPanel m_menuSelect;
	CGameMenuBackground m_menu;
	//玩家，电脑
	CPlayer m_player01{100, 100, _T("tank_player1.png")};
	CPlayer m_player02{200, 100, _T("tank_player2.png")};
	CBot m_bot{300, 200, _T("tank_bot.png")};
	std::list<CBullet> m_lstBullets;//保存已经发射的子弹

public:
	CGame();
	~CGame();
	void SetHandle(HWND hWnd);//输出窗口的句柄

	bool EnterFrame(DWORD dwTime);//

	void OnMouseMove(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
};

