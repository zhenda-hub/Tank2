#include "pch.h"
#include "CGameMenuPanel.h"
#include "CGame.h"

CGameMenuPanel::CGameMenuPanel()
{
	//����ͼ
	m_imgBackground = Gdiplus::Image::FromFile(_T("menu_background.png"));
	//�˵�һ���˻���ս
	m_menuItems[0].index = 0;
	for (int i = 0; i < 2; i++)
	{
		TCHAR path[MAX_PATH];
		_stprintf_s(path, _T("menu_1player_%d.png"), i);//�����ļ���
		auto imgPtr = Gdiplus::Image::FromFile(path);//����ͼƬ
		RectF rc(0, 300, static_cast<float>(imgPtr->GetWidth()),
					static_cast<float>(imgPtr->GetHeight()));
		m_menuItems[0].vImageInfoPtr.push_back(make_tuple(imgPtr, rc));//�ṹ���vector����Ӷ���

	}
	//�˵����� ˫�˴�ս
	m_menuItems[1].index = 0;
	for (int i = 0; i < 2; i++)
	{
		TCHAR path[MAX_PATH];
		_stprintf_s(path, _T("menu_2player_%d.png"), i);//�����ļ���
		auto imgPtr = Gdiplus::Image::FromFile(path);//����ͼƬ
		RectF rc(400, 300, static_cast<float>(imgPtr->GetWidth()),
			static_cast<float>(imgPtr->GetHeight()));
		m_menuItems[1].vImageInfoPtr.push_back(make_tuple(imgPtr, rc));//�ṹ���vector����Ӷ���

	}
}

CGameMenuPanel::~CGameMenuPanel()
{

}

void CGameMenuPanel::Draw(Graphics& gh) const
{
	//������ͼƬ
	gh.DrawImage(m_imgBackground, m_rect);
	//���Ӳ˵�
	for (auto menuItem : m_menuItems)
	{
		auto img = get<0>(menuItem.vImageInfoPtr[menuItem.index]);
		auto rect = get<1>(menuItem.vImageInfoPtr[menuItem.index]);
		gh.DrawImage(img, rect);
	}
}

void CGameMenuPanel::OnMouseMove(UINT nFlags, CPoint point)//��2�����������λ��
{
	PointF pt(static_cast<float>(point.x), static_cast<float>(point.y));
	//���Ӳ˵�
	for (auto &menuItem : m_menuItems)
	{
		auto img = get<0>(menuItem.vImageInfoPtr[menuItem.index]);
		auto rect = get<1>(menuItem.vImageInfoPtr[menuItem.index]);
		if (rect.Contains(pt))//�ж��Ƿ������ǰ����λ��
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
			//�˻���ս�������
			//AfxMessageBox(_T("������̧��1"));
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
			//˫�˴�ս�������
			//AfxMessageBox(_T("������̧��2"));
			m_pParent->SetStep(CGame::EGameTypeOne2OneMenu);
			return;
		}
	}
}
