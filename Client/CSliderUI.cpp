#include "CSliderUI.h"
#include "CImage.h"
#include "CkeyMgr.h"

CSliderUI::CSliderUI()
	: CUI(false)
	, m_colorNormal(D2D1::ColorF::Gray)
	, m_colorMouseOn(D2D1::ColorF::White)
	, m_fRatio(1.f)
	, m_pSceneFunc(nullptr)
	, m_pSceneInst(nullptr)
{

}

CSliderUI::~CSliderUI()
{

}

void CSliderUI::render(HDC _dc)
{
}

void CSliderUI::render(Gdiplus::Graphics* _pDGraphics)
{
}

void CSliderUI::render(ID2D1HwndRenderTarget* _pRender)
{
	component_render(_pRender);
	render_child(_pRender);
}

void CSliderUI::MouseOn()
{
}

void CSliderUI::MouseLbtnDown()
{
}

void CSliderUI::MouseLbtnUp()
{
}

void CSliderUI::MouseLbtnClicked()
{
	//마우스 클릭 했을 때. 
	Vec2 vPos = MOUSE_POS;
}

void CSliderUI::SetSliderValue(float _fValue)
{
	m_fRatio = _fValue; 

	CImage* fillImage = (CImage*)GetImage(1);
	fillImage->SetRatio(m_fRatio);
}

void CSliderUI::SetClickedCallBack(CScene* _pScene, SCENE_MEMFUNC _pSceneFunc)
{
	m_pSceneFunc = _pSceneFunc;
	m_pSceneInst = _pScene;
}


void CSliderUI::update()
{
	if (GetImages().size() < 2) return;
	// 이 UI가 눌려있을때. 
	if (IsLbtnDown()) {
		Vec2 vPos = MOUSE_POS;

		float sliderStartX = GetPos().x - GetScale().x / 2;
		float rightX = GetScale().x;
		float relativeMouseX = vPos.x - sliderStartX;

		float sliderValue = relativeMouseX / rightX;
		if (sliderValue > 1.f) sliderValue = 1.f;
		else if (sliderValue < 0.f) sliderValue = 0.f;
		
		SetSliderValue(sliderValue);
		//wprintf(L"Slider Value: %f\n", sliderValue);

		if (m_pSceneInst && m_pSceneFunc) {//슬라이딩 될 때마다, 이게 있다면 함수 실행. 
			((*m_pSceneInst).*m_pSceneFunc)();
		}
	}
}
