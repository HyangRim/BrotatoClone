#include "pch.h"
#include "CPanelUI.h"
#include "CkeyMgr.h"


CPanelUI::CPanelUI()
	: CUI(false)
	, m_bCanMove(true)
{

}

CPanelUI::~CPanelUI() {

}

void CPanelUI::update()
{
}

void CPanelUI::MouseOn()
{
	if (m_bCanMove && IsLbtnDown()) {
		Vec2 vDiff = MOUSE_POS - m_vDragStart;

		Vec2 vCurPos = GetPos();
		vCurPos += vDiff;
		SetPos(vCurPos);

		m_vDragStart = MOUSE_POS;
	}
}

void CPanelUI::MouseLbtnDown()
{
	//�г� UI�� �巡���ؼ� �̵� �����ϰ� �ؾ���. 
	//���� ������ ����� �ؼ� �װ� ��ǥ�� ����ϰ�, ������ ��ǥ�� ����Ͽ� �Ȱ��� �Ÿ���ŭ �̵���Ű�� �Ǵ� ��. 
	if (m_bCanMove) {
		m_vDragStart = MOUSE_POS;
	}
}

void CPanelUI::MouseLbtnUp()
{
}

void CPanelUI::render(HDC _dc)
{
}

void CPanelUI::render(Gdiplus::Graphics* _pDGraphics)
{
}

void CPanelUI::render(ID2D1HwndRenderTarget* _pRender)
{

	CUI::render(_pRender);
}
