#pragma once
//����ҵ�����̣� ϵͳ -> ���� -> �����߼�
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
	void SetHandle(HWND hWnd);//������ڵľ��
	bool EnterFrame(DWORD dwTime);//������Ϸ��ÿһ֡
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);

	enum EGameType
	{
		EGameTypeMenu = 0,
		EGameTypeOne2BotMenu,	//�˻���ս
		EgameTypeOne2Bot,
		EGameTypeOne2BotEnd,
		EGameTypeOne2OneMenu,	//˫�˶�ս
		EGameTypeOne2One,
		EGameTypeOne2OneEnd,
		EGameTypeCount			//=7

	};
	void SetStep(CGame::EGameType step);//���������׶Σ��������ʼ��

private:
	/*����*/
	int m_fps{0};
	EGameType m_eStep{EGameTypeMenu};
	CPlayer m_player01;
	CPlayer m_player02;
	CBot m_bot;
	std::list<CBullet> m_lstBullets;//�ӵ������б�Ƶ����ɾ
	CGameMap m_map{20, 20, 780, 580};//��ͼ��С
	CGameMenuPanel m_menuSelect;//��ʼ�˵�
	CGameMenuBackground m_menu;//����ͼ
	CGameMenuPanel2 m_menuBackup;//���ز˵�
	CKeyMenuPlayer01 m_keymenu01;//��ʾ�˵�
	CKeyMenuPlayer02 m_keymenu02;//��ʾ�˵�2

private:
	//����
	HWND m_hWnd;
	/*��ʼ������
	1. ������Ϸ����
	2. ��ʼ����ͼ������λ��
	*/
	bool GameInit();
	bool GameInitMenu();
	bool GameInitOne2BotMenu();
	bool GameInitOne2Bot();
	bool GameInitOne2BotEnd();
	bool GameInitOne2OneMenu();
	bool GameInitOne2One();
	bool GameInitOne2OneEnd();
	//�����淽������ ����ָ������飬 �������
	bool (CGame::* m_initFunc[EGameTypeCount])() = {
		&CGame::GameInitMenu,
		&CGame::GameInitOne2BotMenu,
		&CGame::GameInitOne2Bot,
		&CGame::GameInitOne2BotEnd,
		&CGame::GameInitOne2OneMenu,
		&CGame::GameInitOne2One,
		&CGame::GameInitOne2OneEnd
	};

	/*�߼�����
	1.ά���ӵ�״̬
	2.ά�������˵��Զ��ƶ�������
	3. ά�����̹��״̬
	*/
	//void GameLogic();//��Ϸ���߼�
	void GameRunLogic();
	void GameRunLogicOnMenu();
	void GameRunLogicOnOne2BotMenu();
	void GameRunLogicOnOne2Bot();
	void GameRunLogicOnOne2BotEnd();
	void GameRunLogicOnOne2OneMenu();
	void GameRunLogicOnOne2One();
	void GameRunLogicOnOne2OneEnd();
	//�����淽������ ����ָ������飬 �������
	void (CGame::* m_logicFunc[EGameTypeCount])() = {
		&CGame::GameRunLogicOnMenu,
		&CGame::GameRunLogicOnOne2BotMenu,
		&CGame::GameRunLogicOnOne2Bot,
		&CGame::GameRunLogicOnOne2BotEnd,
		&CGame::GameRunLogicOnOne2OneMenu,
		&CGame::GameRunLogicOnOne2One,
		&CGame::GameRunLogicOnOne2OneEnd
	};

	/*��ͼ����
	���������Ϸ�Ķ���
	*/
	void GameRunDraw();//��ͼ
	void GameRunDrawOnMenu(Graphics& gh);
	void GameRunDrawOnOne2BotMenu(Graphics& gh);
	void GameRunDrawOnOne2Bot(Graphics& gh);
	void GameRunDrawOnOne2BotEnd(Graphics& gh);
	void GameRunDrawOnOne2OneMenu(Graphics& gh);
	void GameRunDrawOnOne2One(Graphics& gh);
	void GameRunDrawOnOne2OneEnd(Graphics& gh);
	//�����淽������ ����ָ������飬 �������
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
	void RemoveTimeoutBullets();//�����ӵ��ĳ�ʱ
	void ProcessHitBullets();//�����ӵ�����
	void AI();
	void DrawFps(Graphics& gh);//����FPS
	


};

