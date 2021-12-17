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

/*��ʼ������*/

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
	pt.X = (rc.left + (rc.right-rc.left)) / 2.0f;
	pt.Y = (rc.top + (rc.bottom-rc.top)) / 2.0f;
	m_keymenu01.SetCenterPoint(pt);
	m_keymenu01.SetStop(false);
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


void CGame::GameRunDraw()
{
	HDC hdc = ::GetDC(m_hWnd);//��ȡ�豸���

	CRect rc;
	GetClientRect(m_hWnd, &rc);
	CDC* dc = CClientDC::FromHandle(hdc);//�Ѿ��ת��ΪCDC����

	CDC m_dcMemory;
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(dc, rc.Width(), rc.Height()); // ��������

	m_dcMemory.CreateCompatibleDC(dc);//�����豸���
	CBitmap* pOldBitmap = m_dcMemory.SelectObject(&bmp);//���ѡ��λͼ

	Graphics gh(m_dcMemory.GetSafeHdc());//������Ķ���
	gh.Clear(Color::White);
	gh.ResetClip();
	//����
	DrawFps(gh);//����֡��
	//����ͼƬ
	{
		/*m_menu.Draw(gh);

		m_menuSelect.Draw(gh);*/

		//�Ȼ��Ƶ�ͼ�������̹��
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
		if (KEYDOWN('M'))
		{
			CBullet blt;
			if (m_player01.Fire(blt))
			{
				m_lstBullets.push_back(blt);
			}
		}

		//�ӵ�
		for (auto& blt : m_lstBullets)
		{
			blt.Move();
		}
		{
			//�Ƴ���ʱ���ӵ��������˱�׼��ĺ���
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
		AfxMessageBox(_T("���һ����ʼλ�û�ȡʧ��"));
		return false;
	}
	else
	{
		m_player01.SetCenterPoint(ptCenter);
	}
	m_lstBullets.clear();//�����ӵ�
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
		gdiplus::font font(_t("����"), 10.0);
		crect rc;
		::getclientrect(m_hwnd, &rc);
		pointf origin(static_cast<float>(rc.right - 50), static_cast<float>(rc.top + 2));
		gh.drawstring(s.getstring(), -1, &font, origin, &brush);
	}
}
