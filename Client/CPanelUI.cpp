#include "pch.h"
#include "CPanelUI.h"
#include "CkeyMgr.h"
#include "CUI.h"


CPanelUI::CPanelUI()
	: CUI(false)
	, m_bCanMove(true)
	, m_fradiusX(10.f)
	, m_fradiusY(10.f)
	, m_colorMouseOn(D2D1::ColorF::White)
	, m_colorNormal(D2D1::ColorF::White)
{

}

CPanelUI::~CPanelUI() 
{

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
	Vec2 vPos = GetFinalPos();
	//vPos = CCamera::GetInstance()->GetRenderPos(vPos);
	Vec2 vScale = GetScale();

	D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
		D2D1::RectF(
			vPos.x - vScale.x / 2.f,
			vPos.y - vScale.y / 2.f,
			vPos.x + vScale.x / 2.f,
			vPos.y + vScale.y / 2.f), // �簢���� ��ǥ (left, top, right, bottom)
		m_fradiusX, // X�� ������ (radiusX)
		m_fradiusY  // Y�� ������ (radiusY)
	);

	ID2D1SolidColorBrush* pBrush = nullptr;
	HRESULT hr = _pRender->CreateSolidColorBrush(
		m_bMouseOn ? m_colorMouseOn : m_colorNormal,
		&pBrush
	);
	if (SUCCEEDED(hr))
	{
		_pRender->FillRoundedRectangle(roundedRect, pBrush);
		pBrush->Release();
	}
	component_render(_pRender);
	render_child(_pRender);
	//CUI::render(_pRender);
}
