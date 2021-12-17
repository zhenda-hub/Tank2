#pragma once
//整体业务流程： 系统 -> 界面 -> 核心逻辑
#include "CGameMenuPanel.h"
#include "GameMenuPanel2.h"
#include "GameMenuBackground.h"
#include "TankEntry.h"
#include "Player.h"
#include "Bullet.h"
#include "Bot.h"
#include "GameMap.h"
#include "Wall.h"
#include "KeyMenuPlayer01.h"
#include "KeyMenuPlayer02.h"


class CGame
{

public:
	CGame();
	~CGame();
	void SetHandle(HWND hWnd);//输出窗口的句柄
	bool EnterFrame(DWORD dwTime);//处理游戏的每一帧
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);

	enum EGameType
	{
		EGameTypeMenu = 0,
		EGameTypeOne2BotMenu,	//人机对战
		EgameTypeOne2Bot,
		EGameTypeOne2BotEnd,
		EGameTypeOne2OneMenu,	//双人对战
		EGameTypeOne2One,
		EGameTypeOne2OneEnd,
		EGameTypeCount			//=7

	};
	void SetStep(CGame::EGameType step);//设置所处阶段，按步骤初始化

private:
	/*属性*/
	int m_fps{0};
	EGameType m_eStep{EGameTypeMenu};
	CPlayer m_player01;
	CPlayer m_player02;
	CBot m_bot;
	std::list<CBullet> m_lstBullets;//子弹对象列表，频繁增删
	CGameMap m_map{20, 20, 780, 580};//地图大小
	CGameMenuPanel m_menuSelect;//开始菜单
	CGameMenuBackground m_menu;//背景图
	CGameMenuPanel2 m_menuBackup;//返回菜单
	CKeyMenuPlayer01 m_keymenu01;//提示菜单
	CKeyMenuPlayer02 m_keymenu02;//提示菜单2

private:
	//窗口
	HWND m_hWnd;
	/*初始化方法
	1. 生成游戏对象
	2. 初始化地图，对象位置
	*/
	bool GameInit();
	bool GameInitMenu();
	bool GameInitOne2BotMenu();
	bool GameInitOne2Bot();
	bool GameInitOne2BotEnd();
	bool GameInitOne2OneMenu();
	bool GameInitOne2One();
	bool GameInitOne2OneEnd();
	//把上面方法加入 函数指针的数组， 方便调用
	bool (CGame::* m_initFunc[EGameTypeCount])() = {
		&CGame::GameInitMenu,
		&CGame::GameInitOne2BotMenu,
		&CGame::GameInitOne2Bot,
		&CGame::GameInitOne2BotEnd,
		&CGame::GameInitOne2OneMenu,
		&CGame::GameInitOne2One,
		&CGame::GameInitOne2OneEnd
	};

	/*逻辑方法
	1.维护子弹状态
	2.维护机器人的自动移动，攻击
	3. 维护玩家坦克状态
	*/
	//void GameLogic();//游戏主逻辑
	void GameRunLogic();
	void GameRunLogicOnMenu();
	void GameRunLogicOnOne2BotMenu();
	void GameRunLogicOnOne2Bot();
	void GameRunLogicOnOne2BotEnd();
	void GameRunLogicOnOne2OneMenu();
	void GameRunLogicOnOne2One();
	void GameRunLogicOnOne2OneEnd();
	//把上面方法加入 函数指针的数组， 方便调用
	void (CGame::* m_logicFunc[EGameTypeCount])() = {
		&CGame::GameRunLogicOnMenu,
		&CGame::GameRunLogicOnOne2BotMenu,
		&CGame::GameRunLogicOnOne2Bot,
		&CGame::GameRunLogicOnOne2BotEnd,
		&CGame::GameRunLogicOnOne2OneMenu,
		&CGame::GameRunLogicOnOne2One,
		&CGame::GameRunLogicOnOne2OneEnd
	};

	/*绘图方法
	负责绘制游戏的对象
	*/
	void GameRunDraw();//绘图
	void GameRunDrawOnMenu(Graphics& gh);
	void GameRunDrawOnOne2BotMenu(Graphics& gh);
	void GameRunDrawOnOne2Bot(Graphics& gh);
	void GameRunDrawOnOne2BotEnd(Graphics& gh);
	void GameRunDrawOnOne2OneMenu(Graphics& gh);
	void GameRunDrawOnOne2One(Graphics& gh);
	void GameRunDrawOnOne2OneEnd(Graphics& gh);
	//把上面方法加入 函数指针的数组， 方便调用
	void (CGame::* m_drawFunc[EGameTypeCount])(Graphics&) = {
		&CGame::GameRunDrawOnMenu,
		&CGame::GameRunDrawOnOne2BotMenu,
		&CGame::GameRunDrawOnOne2Bot,
		&CGame::GameRunDrawOnOne2BotEnd,
		&CGame::GameRunDrawOnOne2OneMenu,
		&CGame::GameRunDrawOnOne2One,
		&CGame::GameRunDrawOnOne2OneEnd
	};

private:
	void RemoveTimeoutBullets();//处理子弹的超时
	void ProcessHitBullets();//处理子弹运行
	void AI();
	void DrawFps(Graphics& gh);//绘制FPS
	


};

