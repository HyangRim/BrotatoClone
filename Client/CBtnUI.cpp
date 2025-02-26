#include "pch.h"
#include "CBtnUI.h"
#include "CScene.h"
#include "CScene_Tool.h"
#include "CObject.h"
#include "CCore.h"
#include "CPanelUI.h"
#include "CSceneMgr.h"
#include "Direct2DMgr.h"
#include "CSpriteUI.h"
#include "CImage.h"


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
	for (size_t i = 0; i < m_vTempObjects.size(); ++i)
		DeleteObject(m_vTempObjects[i]);
}


void CBtnUI::MouseOn()
{
	// MouseOn �ݹ� �Լ��� �̹� ȣ��Ǿ����� �ƹ� �۾��� ���� ����
	if (m_bMouseOnCalled) {
		return;
	}
	if (nullptr != m_pFuncMouseOn) {
		m_pFuncMouseOn(m_param1MouseOn, m_param2MouseOn);
		// MouseOn �ݹ� �Լ��� �̹� ȣ��Ǿ����� �ƹ� �۾��� ���� ����
		m_bMouseOnCalled = true; // ȣ�� ���·� ����
		return;
	}
	/*
	if (m_bPanelCreated) {
		return;
	}
	//186, 250
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	/////////////////
	CImage* image = GetImage(0);
	if (image == nullptr) return;
	/////////////////

	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	CPanelUI* panel = new CPanelUI;
	panel->SetPos(Vec2(vResolution.x / 2.f, 210.f));
	panel->SetScale(Vec2(186.f, 250.f));
	panel->SetRadius(10.f, 10.f);
	panel->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));

	CObject* characterImage = new CSpriteUI;
	characterImage->AddImage(image->GetBitmap());
	characterImage->SetScale(Vec2(48.f, 48.f));

	Vec2 vPos = panel->GetPos() - (panel->GetScale() / 2.f) + Vec2(35.f,35.f);
	characterImage->SetPos(vPos);

	CSceneMgr::GetInstance()->GetCurScene()->AddObject(panel, GROUP_TYPE::UI);
	CSceneMgr::GetInstance()->GetCurScene()->AddObject(characterImage, GROUP_TYPE::UI);
	
	m_vTempObjects.push_back(characterImage);
	m_vTempObjects.push_back(panel);

	m_bPanelCreated = true;*/
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
		if (m_bIsRoundedRect)
		{
			_pRender->FillRoundedRectangle(roundedRect, pBrush);
			//_pRender->DrawRoundedRectangle(roundedRect, pBrush, 2.0f);
			//_pRender->FillRoundedRectangle(roundedRect, pBrush2);
		}
		else _pRender->FillRectangle(rect, pBrush);

		// �ձ� �簢�� �׸���
		//_pRender->DrawRoundedRectangle(roundedRect, pBrush, 2.0f); // ������ �׸���
		//_pRender->FillRoundedRectangle(roundedRect, pBrush);

		pBrush->Release();
	}

	component_render(_pRender);
}

//����, �г� UI�ȿ� �̹����� ����� �� �ִ� ��ư���� ����.
//���� Ÿ�ϵ��� �̹����� ����. 

//�� ���� �� ������ ���� ������ �̹�����. 

void CBtnUI::update()
{
	if (IsMouseOn() == true)
		MouseOn();
	else {
		
		if (m_vTempObjects.size() != 0)
		{
			printf("OUT\n");
			for (size_t i = 0; i < m_vTempObjects.size(); ++i)
				DeleteObject(m_vTempObjects[i]);
			m_vTempObjects.clear();
			m_bMouseOnCalled = false;
		}
	}
}