#include "pch.h"

#include "CScene_Select_Weapon.h"
#include "CScene.h"
#include "CCore.h"
#include "CSpriteUI.h"
#include "CBtnUI.h"

#include "Direct2DMgr.h"

CScene_Select_Weapon::CScene_Select_Weapon()
{
	/////////////무조건 백그라운드와 물음표는 로딩 되어있을거임 무조건무조건
}

CScene_Select_Weapon::~CScene_Select_Weapon()
{

}

void CScene_Select_Weapon::update()
{
	CScene::update();
}

void CScene_Select_Weapon::render(ID2D1HwndRenderTarget* _pRender)
{
	CScene::render(_pRender);
}

void CScene_Select_Weapon::Enter()
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	/////////////////뒷 배경 오브젝트/////////////////
	CObject* backGround = new CSpriteUI;
	backGround->SetObjType(GROUP_TYPE::UI);
	backGround->AddImage(pD2DMgr->GetStoredBitmap(L"BackGround"));
	backGround->SetPos(vResolution / 2.f);
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
	backBtn->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(28, 28, 28));
	backBtn->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::SELECT_CHARACTER, 0);
	backBtn->CreateTextUI(L"뒤로", Vec2(-47.f, -14.f), Vec2(47.f, 14.f)
		, 12, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(backBtn, GROUP_TYPE::UI);
	/////////////////뒤로 가기 버틈///////////////////

	//////////////////////////캐릭터들 UI////////////////////////
	float leftMargin = 32.f;
	float LR_interval = 5.f;
	float UD_interval = 4.f;

	Vec2 startPos(190.f, 380.f);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			CBtnUI* selectCharacterUI = new CBtnUI;
			selectCharacterUI->SetObjType(GROUP_TYPE::UI);
			selectCharacterUI->SetScale(Vec2(48.f, 48.f));
			selectCharacterUI->SetName(L"TEST");
			selectCharacterUI->SetUIType(UI_TYPE::BTN);

			if (i == 0 && j == 0) selectCharacterUI->AddImage(pD2DMgr->GetStoredBitmap(L"RandomIcon"));
			else if (i == 0 && j == 1) selectCharacterUI->AddImage(pD2DMgr->GetStoredBitmap(L"WellRounded"));
			else selectCharacterUI->AddImage(pD2DMgr->GetStoredBitmap(L"LockedIcon"));

			CImage* image = selectCharacterUI->GetImage(0);
			selectCharacterUI->SetOnCallBack(ShowCharacterInfo
				, reinterpret_cast<DWORD_PTR>(image)
				, reinterpret_cast<DWORD_PTR>(&selectCharacterUI->GetTrashCan()));

			selectCharacterUI->SetPos(startPos + Vec2((selectCharacterUI->GetScale().x + LR_interval) * j
				, (selectCharacterUI->GetScale().y + UD_interval) * i));
			selectCharacterUI->SetIsRound(true, 10.f, 10.f);
			selectCharacterUI->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(31, 31, 31));


			AddObject(selectCharacterUI, GROUP_TYPE::UI);


		}
	}
	//////////////////////////캐릭터들 UI////////////////////////
}

void CScene_Select_Weapon::Exit()
{

}
