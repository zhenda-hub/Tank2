#pragma once
//整体业务流程： 系统 -> 界面 -> 核心逻辑
#include "CGameMenuPanel.h"
#include "GameMenuBackground.h"
class CGame
{
	HWND m_hWnd;//窗口
	void GameRunDraw();
	void DrawFps(Graphics& gh);

	int m_fps{0};
	CGameMenuPanel m_menuSelect;//开始菜单
	CGameMenuBackground m_menu;
public:
	CGame();
	~CGame();
	void SetHandle(HWND hWnd);//输出窗口的句柄

	bool EnterFrame(DWORD dwTime);

	void OnMouseMove(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
};

