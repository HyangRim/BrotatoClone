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
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"items\\global\\locked_icon.png", L"LockedIcon", false);
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"items\\global\\random_icon.png", L"RandomIcon", false);
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"items\\characters\\well_rounded\\well_rounded_icon.png", L"WellRounded", false);

}

CScene_Select_Character::~CScene_Select_Character()
{

}


void CScene_Select_Character::update()
{
	CScene::update();
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
	backGround->SetObjType(GROUP_TYPE::UI);
	backGround->AddImage(pD2DMgr->GetStoredBitmap(L"BackGround"));
	backGround->SetPos(vResolution/2.f);
	backGround->SetScale(vResolution);
	AddObject(backGround, GROUP_TYPE::DEFAULT);
	/////////////////뒷 배경 오브젝트/////////////////

	/////////////////뒤로 가기 버틈///////////////////
	CBtnUI* backBtn = new CBtnUI;
	backBtn->SetName(L"BackBtn");
	backBtn->SetObjType(GROUP_TYPE::UI);
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
	CObject* panelTextSelectCharacter = new CSpriteUI;
	panelTextSelectCharacter->SetObjType(GROUP_TYPE::UI);
	panelTextSelectCharacter->SetPos(Vec2(vResolution.x / 2.f, 50.f));
	panelTextSelectCharacter->SetScale(Vec2(300.f, 36.f));
	panelTextSelectCharacter->CreateTextUI(L"캐릭터 선택", Vec2(-150.f, -18.f), Vec2(150.f, 18.f)
		, 30, D2D1::ColorF::White, true, 2.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(panelTextSelectCharacter, GROUP_TYPE::UI);
	/////////////////캐릭터 선택 글자 UI(상단)///////////////////



	//////////////////////////캐릭터들 UI////////////////////////
	float leftMargin = 32.f;
	float LR_interval = 5.f;
	float UD_interval = 4.f;

	Vec2 startPos(56.f, 387.f);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			CBtnUI* selectCharacterUI = new CBtnUI;
			selectCharacterUI->SetObjType(GROUP_TYPE::UI);
			selectCharacterUI->SetScale(Vec2(48.f, 48.f));
			selectCharacterUI->SetName(L"TEST");
			selectCharacterUI->SetUIType(UI_TYPE::BTN);

			if (i == 0 && j == 0) selectCharacterUI->AddImage(pD2DMgr->GetStoredBitmap(L"RandomIcon"));
			else if (i == 0 && j == 1) selectCharacterUI->AddImage(pD2DMgr->GetStoredBitmap(L"WellRounded"));
			else selectCharacterUI->AddImage(pD2DMgr->GetStoredBitmap(L"LockedIcon"));

			selectCharacterUI->SetPos(startPos + Vec2((selectCharacterUI->GetScale().x + LR_interval) * j
				, (selectCharacterUI->GetScale().y + UD_interval) * i));
			selectCharacterUI->SetIsRound(true, 10.f, 10.f);
			selectCharacterUI->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(31, 31, 31));


			AddObject(selectCharacterUI, GROUP_TYPE::UI);


		}
	}
	//////////////////////////캐릭터들 UI////////////////////////
}

void CScene_Select_Character::Exit()
{

}
