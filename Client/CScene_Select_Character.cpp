#include "pch.h"

#include "CCore.h"
#include "CSpriteUI.h"
#include "CCamera.h"
#include "CBtnUI.h"

#include "Direct2DMgr.h"

#include "CScene_Select_Character.h"

CScene_Select_Character::CScene_Select_Character()
{
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"texture\\ui\\menus\\shop\\shop_background.png", L"BackGround", false);

}

CScene_Select_Character::~CScene_Select_Character()
{

}


void CScene_Select_Character::update()
{

}

void CScene_Select_Character::render(ID2D1HwndRenderTarget* _pRender)
{
	CScene::render(_pRender);
}

void CScene_Select_Character::Enter()
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	/////////////////뒷 배경 오브젝트/////////////////
	CObject* backGround = new CSpriteUI;
	backGround->AddImage(pD2DMgr->GetStoredBitmap(L"BackGround"));
	backGround->SetPos(vResolution/2.f);
	backGround->SetScale(vResolution);
	AddObject(backGround, GROUP_TYPE::DEFAULT);
	/////////////////뒷 배경 오브젝트/////////////////

	/////////////////뒤로 가기 버틈///////////////////
	CBtnUI* backBtn = new CBtnUI;
	backBtn->SetName(L"BackBtn");
	backBtn->SetScale(Vec2(94.f, 28.f)); 
	backBtn->SetPos(Vec2(71.f, 38.f));
	backBtn->SetIsRound(true, 10.f, 10.f);
	backBtn->SetColor(ColorNormalize(237,237,237), ColorNormalize(28,28,28));
	backBtn->CreateTextUI(L"뒤로", Vec2(-47.f, -14.f), Vec2(47.f, 14.f)
		, 12, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(backBtn, GROUP_TYPE::UI);
	/////////////////뒤로 가기 버틈///////////////////

	/////////////////캐릭터 선택 글자 UI(상단)///////////////////

	/////////////////캐릭터 선택 글자 UI(상단)///////////////////
}

void CScene_Select_Character::Exit()
{

}
