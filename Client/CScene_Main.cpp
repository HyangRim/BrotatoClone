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
#include "CSoundMgr.h"

#include "CObject.h"
#include "CEmptyObj.h"
#include "CGround.h"
#include "CImage.h"

CScene_Main::CScene_Main()
{
}

CScene_Main::~CScene_Main()
{
}

void CScene_Main::Enter()
{
	// Direct2DMgr 인스턴스 가져오기 (텍스쳐 관련)
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	pD2DMgr->LoadAndStoreBitmap(L"texture\\ui\\menus\\title_screen\\splash_art_bg.png", L"Title_Ground", false);
	pD2DMgr->LoadAndStoreBitmap(L"texture\\ui\\menus\\title_screen\\splash_art_brotato.png", L"Title_Brotato", false);
	pD2DMgr->LoadAndStoreBitmap(L"texture\\ui\\menus\\title_screen\\splash_art_mist_back.png", L"Title_back", false);
	pD2DMgr->LoadAndStoreBitmap(L"texture\\ui\\menus\\title_screen\\splash_art_mist_front.png", L"Title_front", false);
	pD2DMgr->LoadAndStoreBitmap(L"texture\\ui\\menus\\title_screen\\splash_art_mist_mid.png", L"Title_mid", false);
	pD2DMgr->LoadAndStoreBitmap(L"texture\\ui\\menus\\title_screen\\splash_art_post_processing.png", L"Title_lights", false);
	pD2DMgr->LoadAndStoreBitmap(L"texture\\ui\\menus\\title_screen\\ui_logo.png", L"Brotato_logo", false);


	//사운드 관련 Instance 가져오기. 
	CSoundMgr::GetInstance()->AddSound(L"MainTitle", L"sound\\music\\main_title_bgm.mp3", true, true);
	Vec2 vResolution = CCore::GetInstance()->GetResolution();



	//UI구성. 
	//Object 추가.
	//실제 생성된 객체는 플레이어, 주소를 받은 건 부모 클래스. 
	//맨 뒤 Brotato 바닥. 
	CObject* pObj = new CGround;
	pObj->SetPos(vResolution / 2.f);
	pObj->SetScale(Vec2(1035.f, 540.f));
	pObj->SetName(L"Title_Ground");
	pObj->CreateImage();
	pObj->GetImage()->SetBitmap(pD2DMgr->GetStoredBitmap(L"Title_Ground"));
	AddObject(pObj, GROUP_TYPE::DEFAULT);

	//맨 뒤, 응원단
	pObj = new CGround;
	pObj->SetPos(vResolution / 2.f);
	pObj->SetScale(Vec2(1035.f, 540.f));
	pObj->SetName(L"Title_back");
	pObj->CreateImage();
	pObj->GetImage()->SetBitmap(pD2DMgr->GetStoredBitmap(L"Title_back"));
	AddObject(pObj, GROUP_TYPE::DEFAULT);

	//중간, 응원단
	pObj = new CGround;
	pObj->SetPos(vResolution / 2.f);
	pObj->SetScale(Vec2(1035.f, 540.f));
	pObj->SetName(L"Title_mid");
	pObj->CreateImage();
	pObj->GetImage()->SetBitmap(pD2DMgr->GetStoredBitmap(L"Title_mid"));
	AddObject(pObj, GROUP_TYPE::DEFAULT);

	//맨 앞, 응원단
	pObj = new CGround;
	pObj->SetPos(vResolution / 2.f);
	pObj->SetScale(Vec2(1035.f, 540.f));
	pObj->SetName(L"Title_front");
	pObj->CreateImage();
	pObj->GetImage()->SetBitmap(pD2DMgr->GetStoredBitmap(L"Title_front"));
	AddObject(pObj, GROUP_TYPE::DEFAULT);

	//Brotato
	pObj = new CGround;
	pObj->SetPos(vResolution / 2.f);
	pObj->SetScale(Vec2(1035.f, 540.f));
	pObj->SetName(L"Title_Brotato");
	pObj->CreateImage();
	pObj->GetImage()->SetBitmap(pD2DMgr->GetStoredBitmap(L"Title_Brotato"));
	AddObject(pObj, GROUP_TYPE::DEFAULT);

	//포스트 프로세싱
	pObj = new CGround;
	pObj->SetPos(vResolution / 2.f);
	pObj->SetScale(Vec2(1035.f, 540.f));
	pObj->SetName(L"Title_lights");
	pObj->CreateImage();
	pObj->GetImage()->SetBitmap(pD2DMgr->GetStoredBitmap(L"Title_lights"));
	AddObject(pObj, GROUP_TYPE::DEFAULT);

	//브로타토 로고
	pObj = new CGround;
	pObj->SetPos(Vec2(vResolution.x / 2.f, 115.f));
	pObj->SetScale(Vec2(561.f, 165.f));
	pObj->SetName(L"Brotato_logo");
	pObj->CreateImage();
	pObj->GetImage()->SetBitmap(pD2DMgr->GetStoredBitmap(L"Brotato_logo"));
	AddObject(pObj, GROUP_TYPE::DEFAULT);


	//카메라 위치 세팅. 
	CCamera::GetInstance()->SetLookAt(vResolution / 2.f);
	
	//처음 시작했을 때, BGM틀기. 
	CSoundMgr::GetInstance()->Play(L"MainTitle", 0.2f);
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

void CScene_Main::render(ID2D1HwndRenderTarget* _pRender)
{
	CScene::render(_pRender);
}


