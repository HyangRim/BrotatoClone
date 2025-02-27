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
	
	//패널 UI는 드래그해서 이동 가능하게 해야함. 
	//누른 순간에 기억을 해서 그걸 좌표를 기억하고, 움직인 좌표를 계산하여 똑같은 거리만큼 이동시키면 되는 것. 
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
			vPos.y + vScale.y / 2.f), // 사각형의 좌표 (left, top, right, bottom)
		m_fradiusX, // X축 반지름 (radiusX)
		m_fradiusY  // Y축 반지름 (radiusY)
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
