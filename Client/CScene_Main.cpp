#include "pch.h"
#include "CScene_Main.h"
#include "CTimeMgr.h"
#include "CPanelUI.h"
#include "CCore.h"
#include "CScene.h"
#include "CCamera.h"
#include "CFontMgr.h"
#include "CTextUI.h"
#include "CSpriteUI.h"
#include "Direct2DMgr.h"

CScene_Main::CScene_Main()
{
}

CScene_Main::~CScene_Main()
{
}

void CScene_Main::Enter()
{
	// Direct2DMgr 인스턴스 가져오기
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	pD2DMgr->LoadAndStoreBitmap(L"texture\\ui\\menus\\splash_art_bg.png", L"Title_Ground");
	pD2DMgr->LoadAndStoreBitmap(L"texture\\ui\\menus\\splash_art_brotato.png", L"Title_Brotato");
	pD2DMgr->LoadAndStoreBitmap(L"texture\\ui\\menus\\splash_art_mist_back.png", L"Title_back");
	pD2DMgr->LoadAndStoreBitmap(L"texture\\ui\\menus\\splash_art_mist_front.png", L"Title_front");
	pD2DMgr->LoadAndStoreBitmap(L"texture\\ui\\menus\\splash_art_mist_mid.png", L"Title_mid");
	pD2DMgr->LoadAndStoreBitmap(L"texture\\ui\\menus\\splash_art_post_processing.png", L"Title_lights");
	pD2DMgr->LoadAndStoreBitmap(L"texture\\ui\\menus\\ui_logo.png", L"Brotato_logo");

	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	CCamera::GetInstance()->SetLookAt(vResolution / 2.f);
}

void CScene_Main::Exit()
{
}

void CScene_Main::update()
{
	CScene::update();
}

void CScene_Main::finalupdate()
{
	CScene::finalupdate();
}

void CScene_Main::render(HDC _dc)
{
	CScene::render(_dc);
}

void CScene_Main::render(Gdiplus::Graphics* _pDGraphics)
{
	// 부모 클래스의 render 호출
	CScene::render(_pDGraphics);
}


