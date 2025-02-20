#include "pch.h"
#include "CScene_Test.h"
#include "CTimeMgr.h"
#include "CPanelUI.h"
#include "CCore.h"
#include "CScene.h"
#include "CCamera.h"
#include "CFontMgr.h"
#include "CTextUI.h"

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
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	CTextUI* textUI = new CTextUI();
	textUI->SetPos(Vec2(0.f, 0.f));			  // ��ġ ����(�»��)
	textUI->SetScale(Vec2(300.0f, 100.0f));   // ũ�� ���� (�ؽ�Ʈ ����)
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
	textUI2->SetScale(Vec2(300.0f, 100.0f));   // ũ�� ���� (�ؽ�Ʈ ����)
	textUI2->SetMode(TextUIMode::TEXT);
	textUI2->SetText(L"Hello World!");         // ����� �ؽ�Ʈ ����
	textUI2->SetFontSize(24);                  // ��Ʈ ũ�� ����
	textUI2->SetGdiPlusTextColor(Gdiplus::Color(255, 255, 0, 255)); // GDI+�� ������ ����
	textUI2->SetDrawOutline(true);             // �ܰ��� Ȱ��ȭ
	textUI2->SetOutlineThickness(4.0f);        // �ܰ��� �β� ����
	textUI2->SetGdiPlusOutlineColor(Gdiplus::Color(0, 0, 0)); // �ܰ��� ����(GDI+)
	AddObject(textUI2, GROUP_TYPE::UI);

	CCamera::GetInstance()->SetLookAt(vResolution / 2.f);
}

void CScene_Test::Exit()
{

}