#include "pch.h"
#include "CGame.h"
#include <algorithm>
#include "GameMap.h"

//ʹ��windowsAPI �� ������ ����һ����
#define KEYDOWN(vk) (GetAsyncKeyState(vk) & (0x8000))//����д�������ĵģ��Ų�1Сʱ������

CGame::CGame()
{
	//GameInitOne2Bot();
	/*���������˵���*/
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

/*******************************************��ʼ������*******************************************/

bool CGame::GameInit()
{
	srand(GetTickCount());//��ʼ�������������
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
	for (;;)//��ѭ����ֱ����ͼ��������
	{
		m_map.LoadMap();
		//���һ
		{
			m_player01 = CPlayer(0, 0, _T("tank_player1.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("player01��ʼ��λ��ʧ��"));
			}
			else
			{
				m_player01.SetCenterPoint(ptCenter);
			}
		}
		//������
		{
			m_bot = CBot(0, 0, _T("tank_bot.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("bot��ʼ��λ��ʧ��"));
			}
			else
			{
				m_bot.SetCenterPoint(ptCenter);
			}

		}
		m_lstBullets.clear();
		//�ж��Ƿ�Ϸ�,�������˺���ҵ�̹�˲���ͬһ����
		{
			int startX, startY, targetX, targetY;
			if (!m_map.FindObjPosition(m_bot, startX, startY) || 
				!m_map.FindObjPosition(m_player01, targetX, targetY))
			{
				AfxMessageBox(_T("��ȡ̹��λ�ô���"));
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

	pt.X = rc.right - m_keymenu02.GetRect().Width / 2.0f - 100;//copy�������޸�right���Ų��Сʱ
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
		//���һ
		{
			m_player01 = CPlayer(0, 0, _T("tank_player1.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("player01��ʼ��λ��ʧ��"));
			}
			else
			{
				m_player01.SetCenterPoint(ptCenter);
			}
		}
		//��Ҷ�
		{
			m_player02 = CPlayer(0, 0, _T("tank_player2.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("player02��ʼ��λ��ʧ��"));
			}
			else
			{
				m_player02.SetCenterPoint(ptCenter);
			}
		}

		
		m_lstBullets.clear();
		//�ж��Ƿ�Ϸ�,�������˺���ҵ�̹�˲���ͬһ����
		{
			int startX, startY, targetX, targetY;
			if (!m_map.FindObjPosition(m_player02, startX, startY) ||
				!m_map.FindObjPosition(m_player01, targetX, targetY))
			{
				AfxMessageBox(_T("��ȡ̹��λ�ô���"));
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

/*******************************************�߼�����*******************************************/

void CGame::GameRunLogic()
{
	(this->*m_logicFunc[m_eStep])();
}
void CGame::GameRunLogicOnMenu()
{

}
void CGame::GameRunLogicOnOne2BotMenu()
{
	if (KEYDOWN('M'))//����M,ֹͣ��ʾ��
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
	/************״̬ά��*******************/

	RemoveTimeoutBullets();//�Ƴ���Ч���ӵ�����̹��װ�ӵ�
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

	ProcessHitBullets();//�ӵ�����ά��
	AI();

	/******************��������*****************/

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
			if (m_map.IsHitTheWall(m_player01, true))//��ǽ��ײʱ�Ĵ���
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
			if (m_player01.Fire(blt))//̹�˷����ӵ�
			{
				m_lstBullets.push_back(blt);//�����ӵ��б�
			}
		}

		//if (KEYDOWN('I'))//���ƻ����ˣ� ���Է���
		//{
		//	int startX, startY, targetX, targetY;
		//	if (!m_map.FindObjPosition(m_bot, startX, startY) || 
		//		!m_map.FindObjPosition(m_player01, targetX, targetY))
		//	{
		//		return;
		//	}
		//	float fDirNext = 0;//��������һ������
		//	if (!m_map.FindNextDirection(&fDirNext, startX, startY, targetX, targetY))
		//	{
		//		return;
		//	}

		//	//��ȡbot̹�˵����ĵ�
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
	/******************״̬ά��********************/
	RemoveTimeoutBullets();
	ProcessHitBullets();
	/******************��������û��*******************/

}
void CGame::GameRunLogicOnOne2OneMenu()
{
	if (KEYDOWN('M'))//����M,ֹͣ��ʾ��
	{
		m_keymenu01.SetStop();
	}
	if (KEYDOWN('Q'))//����Q,ֹͣ��ʾ��
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

	/************״̬ά��*******************/

	RemoveTimeoutBullets();//�Ƴ���Ч���ӵ�����̹��װ�ӵ�
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

	ProcessHitBullets();//�ӵ�����ά��

	/******************��������*****************/

	{
		/******************���һ*****************/

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
			if (m_map.IsHitTheWall(m_player01, true))//��ǽ��ײʱ�Ĵ���
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
			if (m_player01.Fire(blt))//̹�˷����ӵ�
			{
				m_lstBullets.push_back(blt);//�����ӵ��б�
			}
		}

		/******************��Ҷ�*****************/
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
			if (m_map.IsHitTheWall(m_player02, true))//��ǽ��ײʱ�Ĵ���
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
			if (m_player02.Fire(blt))//̹�˷����ӵ�
			{
				m_lstBullets.push_back(blt);//�����ӵ��б�
			}
		}
		if (KEYDOWN('Z'))
		{
			if (m_map.IsCanKillTarget(m_player01, m_player02))
			{
				AfxMessageBox(_T("���Դﵽ"));
			}
		}


	}
		
}
void CGame::GameRunLogicOnOne2OneEnd()
{
	/******************״̬ά��********************/
	RemoveTimeoutBullets();
	ProcessHitBullets();
	/******************��������û��*******************/
}


/*******************************************��ͼ����*******************************************/


void CGame::GameRunDraw()
{
	HDC hdc = ::GetDC(m_hWnd);//��ȡ�豸���

	CRect rc;
	GetClientRect(m_hWnd, &rc);
	//˫�����ͼ
	CDC* dc = CClientDC::FromHandle(hdc);//�Ѿ��ת��ΪCDC����
	CBitmap bmp;
	CDC m_dcMemory;
	{
		bmp.CreateCompatibleBitmap(dc, rc.Width(), rc.Height()); // ��������
		m_dcMemory.CreateCompatibleDC(dc);//�����豸���
		CBitmap* pOldBitmap = m_dcMemory.SelectObject(&bmp);//���ѡ��λͼ
	}


	Graphics gh(m_dcMemory.GetSafeHdc());//������Ķ���
	gh.Clear(Color::White);
	gh.ResetClip();
	

	//���ݳ�������ͼƬ
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
	m_menuBackup.Draw(gh);//���ز˵���
	m_map.Draw(gh);//����ͼ
	m_bot.Draw(gh);//������
	m_player01.Draw(gh);//���1
	DrawFps(gh);//FPS
	for (auto b : m_lstBullets)
	{
		b.Draw(gh);//�ӵ�
	}
}
void CGame::GameRunDrawOnOne2BotEnd(Graphics& gh)
{
	m_menuBackup.Draw(gh);//���ز˵���
	m_map.Draw(gh);//����ͼ
	m_bot.Draw(gh);//������
	m_player01.Draw(gh);//���1
	DrawFps(gh);//FPS
	for (auto b : m_lstBullets)
	{
		b.Draw(gh);//�ӵ�
	}

	if (m_player01.IsBombEnd() || m_bot.IsBombEnd())//��Ϸ�������ز˵�
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


	m_menuBackup.Draw(gh);//���ز˵���
	m_map.Draw(gh);//����ͼ
	m_player02.Draw(gh);//���2
	m_player01.Draw(gh);//���1
	DrawFps(gh);//FPS
	for (auto b : m_lstBullets)
	{
		b.Draw(gh);//�ӵ�
	}
}
void CGame::GameRunDrawOnOne2OneEnd(Graphics& gh)
{
	m_menuBackup.Draw(gh);//���ز˵���
	m_map.Draw(gh);//����ͼ
	m_player02.Draw(gh);//���2
	m_player01.Draw(gh);//���1
	DrawFps(gh);//FPS
	for (auto b : m_lstBullets)
	{
		b.Draw(gh);//�ӵ�
	}
	if (m_player01.IsBombEnd() || m_player02.IsBombEnd())//��Ϸ�������ز˵�
	{
		m_eStep = EGameTypeMenu;
	}
}

/*******************************************��������*******************************************/

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
		Gdiplus::Font font(_T("����"), 10.0);
		CRect rc;
		::GetClientRect(m_hWnd, &rc);
		PointF origin(static_cast<float>(rc.right - 50), static_cast<float>(rc.top + 2));
		gh.DrawString(s.GetString(), -1, &font, origin, &brush);
	}
}

void CGame::RemoveTimeoutBullets()
{
	auto itRemote = std::remove_if(m_lstBullets.begin(), m_lstBullets.end(),
		[](CBullet& blt)->bool {return blt.IsTimeout();});//�Ƴ���ʱ���ӵ�
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
	static CGameTimer acTimer(-1, 150);//һ����ʱ��ʱ����
	if (acTimer.IsTimeval())
	{
		int startX, startY, targetX, targetY;
		if (!m_map.FindObjPosition(m_bot, startX, startY) ||
			!m_map.FindObjPosition(m_player01, targetX, targetY))
		{
			return;
		}

		float fDirNext = 0;
		//if (!m_map.FindNextDirection(&fDirNext, startX, startY, targetX, targetY));//��д�˷ֺţ��Ų�2Сʱ
		if (!m_map.FindNextDirection(&fDirNext, startX, startY, targetX, targetY))
		{
			return;
		}

		PointF ptTankCenter = m_bot.GetCenterPoint();//���������ĵ�
		PointF ptAreaCenter = m_map.GetElementAreaCenter(startX, startY);//��ͼ�еĿո����ĵ�
		RectF rc(ptAreaCenter.X-5, ptAreaCenter.Y-5, 10, 10);
		//û�����ĵ㣬��ǰ��
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