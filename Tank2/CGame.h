#pragma once
//����ҵ�����̣� ϵͳ -> ���� -> �����߼�
#include "CGameMenuPanel.h"
#include "GameMenuBackground.h"
#include "TankEntry.h"
#include "Player.h"
#include "Bullet.h"
#include "Bot.h"

class CGame
{
	//��������
	HWND m_hWnd;//����

	void GameRunDraw();//��ͼ
	void GameLogic();
	//����FPS
	void DrawFps(Graphics& gh);
	int m_fps{0};
	//��ʼ�˵�
	CGameMenuPanel m_menuSelect;
	CGameMenuBackground m_menu;
	//��ң�����
	CPlayer m_player01{100, 100, _T("tank_player1.png")};
	CPlayer m_player02{200, 100, _T("tank_player2.png")};
	CBot m_bot{300, 200, _T("tank_bot.png")};
	std::list<CBullet> m_lstBullets;//�����Ѿ�������ӵ�

public:
	CGame();
	~CGame();
	void SetHandle(HWND hWnd);//������ڵľ��

	bool EnterFrame(DWORD dwTime);//

	void OnMouseMove(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
};

