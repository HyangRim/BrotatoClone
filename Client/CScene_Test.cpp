#include "pch.h"
#include "CScene_Test.h"
#include "CTimeMgr.h"
#include "CPanelUI.h"
#include "CCore.h"
#include "CScene.h"
#include "CCamera.h"
#include "CFontMgr.h"
#include "CTextUI.h"
#include "CSpriteUI.h"

CScene_Test::CScene_Test()
	: m_fAcc(0.f)
	, m_iTest(0)
{

}

CScene_Test::~CScene_Test()
{

}

void CScene_Test::update()
{
	CScene::update();
}

void CScene_Test::finalupdate()
{
	CScene::finalupdate();
}

void CScene_Test::render(HDC _dc)
{
	//CScene::render(_dc);
}

void CScene_Test::render(Gdiplus::Graphics* _pDGraphics)
{
	CScene::render(_pDGraphics);
}

void CScene_Test::Enter()
{
	/*
	CSpriteUI* pSpriteUI = new CSpriteUI();
	pSpriteUI->SetTexture(L"MySprite", L"texture\\ui_lifebar_bg.png");
	pSpriteUI->SetPos(Vec2(300.f, 100.f));
	pSpriteUI->SetScale(Vec2(320.f, 48.f) * 0.5f);
	AddObject(pSpriteUI, GROUP_TYPE::UI);
	
	CSpriteUI* pSpriteUI2 = new CSpriteUI();
	pSpriteUI2->SetTexture(L"MySprite2", L"texture\\ui_lifebar_fill.png");
	pSpriteUI2->SetPos(Vec2(0, 0));
	pSpriteUI2->SetScale(Vec2(320.f, 48.f) * 0.5f);
	AddObject(pSpriteUI2, GROUP_TYPE::UI);

	CSpriteUI* pSpriteUI3 = new CSpriteUI();
	pSpriteUI3->SetTexture(L"MySprite3", L"texture\\ui_lifebar_frame.png");
	pSpriteUI3->SetPos(Vec2(0, 0));
	pSpriteUI3->SetScale(Vec2(320.f, 48.f) * 0.5f);
	AddObject(pSpriteUI3, GROUP_TYPE::UI);*/

	CSpriteUI* hpBar = new CSpriteUI();
	
	hpBar->SetTexture(
		L"LifeBarBg", L"texture\\ui_lifebar_bg.png",       // ��� �̹���
		L"LifeBarFill", L"texture\\ui_lifebar_fill.png",   // ä��texture\\�� �̹���
		L"LifeBarFrame", L"texture\\ui_lifebar_frame.png"  // �׵θ� �̹���
	);
	// 3. ��ġ�� ũ�� ����
	hpBar->SetPos(Vec2(400.f, 300.f));     // ȭ�� �߾ӿ� ��ġ
	hpBar->SetScale(Vec2(160.f, 24.f));   // ũ�� ����

	// 4. HP ���� ���� (50%�� ����)
	hpBar->SetHpRatio(0.9f);
	AddObject(hpBar, GROUP_TYPE::UI);
	
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
/*
	CTextUI* textUI = new CTextUI();
	textUI->SetPos(Vec2(0.f, 0.f));			  // ��ġ ����(�»��)
	//textUI->SetScale(Vec2(300.0f, 100.0f)); // ũ�� ���� (�ؽ�Ʈ ����) �ǹ�x ũ��� FontSize��
	textUI->SetMode(TextUIMode::NUMBER);
	textUI->SetText(L"");					  // ����� �ؽ�Ʈ ����
	textUI->SetFontSize(24);                  // ��Ʈ ũ�� ����
	textUI->SetGdiPlusTextColor(Gdiplus::Color(255, 255, 0, 255)); // GDI+�� ������ ����
	textUI->SetDrawOutline(true);             // �ܰ��� Ȱ��ȭ
	textUI->SetOutlineThickness(4.0f);        // �ܰ��� �β� ����
	textUI->SetGdiPlusOutlineColor(Gdiplus::Color(0, 0, 0)); // �ܰ��� ����(GDI+)
	AddObject(textUI, GROUP_TYPE::UI);

	CTextUI* textUI2 = new CTextUI();
	textUI2->SetPos(Vec2(100.f, 200.0f));      // ��ġ ����(�»��)
	//textUI2->SetScale(Vec2(0, 0));		   // ũ�� ���� (�ؽ�Ʈ ����) �ǹ�x ũ��� FontSize��
	textUI2->SetMode(TextUIMode::TEXT);
	textUI2->SetText(L"Hello World!");         // ����� �ؽ�Ʈ ����
	textUI2->SetFontSize(24);                  // ��Ʈ ũ�� ����
	textUI2->SetGdiPlusTextColor(Gdiplus::Color(255, 255, 0, 255)); // GDI+�� ������ ����
	textUI2->SetDrawOutline(true);             // �ܰ��� Ȱ��ȭ
	textUI2->SetOutlineThickness(4.0f);        // �ܰ��� �β� ����
	textUI2->SetGdiPlusOutlineColor(Gdiplus::Color(0, 0, 0)); // �ܰ��� ����(GDI+)
	AddObject(textUI2, GROUP_TYPE::UI);
	*/
	CCamera::GetInstance()->SetLookAt(vResolution / 2.f);
}

void CScene_Test::Exit()
{

}