#include "pch.h"
#include "CBtnUI.h"
#include "CScene.h"
#include "CScene_Tool.h"
#include "CObject.h"


CBtnUI::CBtnUI()
	: CUI(false)
	, m_pFunc(nullptr)
	, m_param1(0)
	, m_param2(0)
	, m_pSceneInst(nullptr)
	, m_pSceneFunc(nullptr)
	, m_colorNormal(D2D1::ColorF::Black)
	, m_colorMouseOn(D2D1::ColorF::White)
{
}

CBtnUI::~CBtnUI()
{
}


void CBtnUI::MouseOn()
{
}

void CBtnUI::MouseLbtnDown()
{
}

void CBtnUI::MouseLbtnUp()
{
}


//버튼마다 해야 할 일이 전부 다 다르지 않나?
//어떤 버튼은 저장, 로드, 레벨업등등... 다 다름. 
void CBtnUI::MouseLbtnClicked()
{
	if (nullptr != m_pFunc) {
		m_pFunc(m_param1, m_param2);
	}

	if (m_pSceneInst && m_pSceneFunc) {
		((*m_pSceneInst).*m_pSceneFunc)();
	}
}

void CBtnUI::SetClickedCallBack(CScene* _pScene, SCENE_MEMFUNC _pSceneFunc) 
{
	m_pSceneFunc = _pSceneFunc;
	m_pSceneInst = _pScene;
}

void CBtnUI::render(HDC _dc)
{
}

void CBtnUI::render(Gdiplus::Graphics* _pDGraphics)
{
}

void CBtnUI::render(ID2D1HwndRenderTarget* _pRender)
{
	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();

	if (m_bCamAffected) {
		vPos = CCamera::GetInstance()->GetRenderPos(vPos);
	}

	// vPos와 vScale은 이미 계산된 좌표와 스케일 값입니다.
	D2D1_RECT_F rect = D2D1::RectF(
		vPos.x - vScale.x / 2.f,
		vPos.y - vScale.y / 2.f,
		vPos.x + vScale.x / 2.f,
		vPos.y + vScale.y / 2.f
	);
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
		if (m_bIsRoundedRect)
		{
			_pRender->FillRoundedRectangle(roundedRect, pBrush);
		}
		else _pRender->FillRectangle(rect, pBrush);

		// 둥근 사각형 그리기
		//_pRender->DrawRoundedRectangle(roundedRect, pBrush, 2.0f); // 윤곽선 그리기
		//_pRender->FillRoundedRectangle(roundedRect, pBrush);

		pBrush->Release();
	}

	component_render(_pRender);
}

//과제, 패널 UI안에 이미지를 출력할 수 있는 버튼들을 넣음.
//각각 타일들의 이미지를 넣음. 

//다 넣을 수 없으면 다음 페이지 이미지로. 