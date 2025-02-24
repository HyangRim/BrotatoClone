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


//��ư���� �ؾ� �� ���� ���� �� �ٸ��� �ʳ�?
//� ��ư�� ����, �ε�, ���������... �� �ٸ�. 
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

	// vPos�� vScale�� �̹� ���� ��ǥ�� ������ ���Դϴ�.
	D2D1_RECT_F rect = D2D1::RectF(
		vPos.x - vScale.x / 2.f,
		vPos.y - vScale.y / 2.f,
		vPos.x + vScale.x / 2.f,
		vPos.y + vScale.y / 2.f
	);

	ID2D1SolidColorBrush* pBrush = nullptr;
	HRESULT hr = _pRender->CreateSolidColorBrush(
		m_bMouseOn ? D2D1::ColorF(D2D1::ColorF::White)   // ���콺 ���� ��ư�� ���ȴٸ� ���
		: D2D1::ColorF(D2D1::ColorF::Black),  // �ƴϸ� ���
		&pBrush
	);

	if (SUCCEEDED(hr))
	{
		_pRender->FillRectangle(rect, pBrush);
		pBrush->Release();
	}
}

//����, �г� UI�ȿ� �̹����� ����� �� �ִ� ��ư���� ����.
//���� Ÿ�ϵ��� �̹����� ����. 

//�� ���� �� ������ ���� ������ �̹�����. 