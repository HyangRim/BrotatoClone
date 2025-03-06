#include "pch.h"

#include "CScene_Select_Weapon.h"
#include "CScene.h"
#include "CCore.h"
#include "CSpriteUI.h"
#include "CBtnUI.h"
#include "CPanelUI.h"
#include "CWeapon.h"
#include "CImage.h"
#include "CPlayer.h"

#include "ItemMgr.h"
#include "CSceneMgr.h"
#include "Direct2DMgr.h"

CScene_Select_Weapon::CScene_Select_Weapon()
{
	/////////////무조건 백그라운드, 물음표, 자물쇠는 로딩 되어있을거임 무조건무조건
	//Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"weapons\\melee\\knife\\knife_icon.png", L"WeaponKnife", false);

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
	backGround->SetObjType(GROUP_TYPE::DEFAULT);
	backGround->AddImage(pD2DMgr->GetStoredBitmap(L"shop_background"));
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

	/////////////////무기선택 UI(상단)///////////////////
	CObject* panelTextSelectWeapon = new CSpriteUI;
	panelTextSelectWeapon->SetObjType(GROUP_TYPE::UI);
	panelTextSelectWeapon->SetPos(Vec2(vResolution.x / 2.f, 50.f));
	panelTextSelectWeapon->SetScale(Vec2(300.f, 36.f));
	panelTextSelectWeapon->CreateTextUI(L"무기 선택", Vec2(-150.f, -18.f), Vec2(150.f, 18.f)
		, 30, D2D1::ColorF::White, true, 2.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(panelTextSelectWeapon, GROUP_TYPE::UI);
	/////////////////무기선택 UI(상단)///////////////////

	/////////////////선택한 캐릭터//////////////////////
	CPanelUI* panel = new CPanelUI;
	panel->SetScale(Vec2(186.f, 250.f));
	panel->SetObjType(GROUP_TYPE::UI);
	panel->SetPos(Vec2(vResolution.x / 2.f - 3.f - panel->GetScale().x / 2.f, 210.f));
	panel->SetName(L"Parent2");
	panel->SetRadius(10.f, 10.f);
	panel->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));
	
	ItemMgr* ItemMgr = ItemMgr::GetInstance();
	CObject* characterImage = new CSpriteUI;
	wstring iconTag = ItemMgr->GetBasicCharacter()->tag + L"_icon";
	characterImage->AddImage(pD2DMgr->GetStoredBitmap(iconTag));
	Vec2 vPos = Vec2(35.f, 35.f) - (panel->GetScale() / 2.f);
	characterImage->GetImage(0)->SetOffset(vPos);
	characterImage->SetObjType(GROUP_TYPE::UI);
	characterImage->SetName(L"Child2");
	characterImage->SetScale(Vec2(48.f, 48.f));
	characterImage->SetPos(panel->GetPos());

	panel->AddChild((CUI*)characterImage);
	AddObject(panel, GROUP_TYPE::UI);

	/////////////////선택한 캐릭터//////////////////////

	//////////////////////////캐릭터들 UI////////////////////////
	float leftMargin = 32.f;
	float LR_interval = 5.f;
	float UD_interval = 4.f;

	Vec2 startPos(190.f, 380.f);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			CBtnUI* selectWeaponUI = new CBtnUI;
			selectWeaponUI->SetObjType(GROUP_TYPE::UI);
			selectWeaponUI->SetScale(Vec2(48.f, 48.f));
			selectWeaponUI->SetName(L"TEST12314");
			selectWeaponUI->SetUIType(UI_TYPE::BTN);

			wstring tag;

			WeaponMapping(selectWeaponUI, i, j, tag);
			wstring* pTag = new wstring(tag);

			/*
			if (i == 0 && j == 0) selectCharacterUI->AddImage(pD2DMgr->GetStoredBitmap(L"random_icon"));
			else if (i == 0 && j == 1) selectCharacterUI->AddImage(pD2DMgr->GetStoredBitmap(L"knife_icon"));
			else selectCharacterUI->AddImage(pD2DMgr->GetStoredBitmap(L"locked_icon"));*/

			CImage* image = selectWeaponUI->GetImage(0);
			selectWeaponUI->SetOnCallBack(ShowWeaponInfo
				, reinterpret_cast<DWORD_PTR>(image)
				, reinterpret_cast<DWORD_PTR>(&selectWeaponUI->GetTrashCan()));

			if (tag.compare(L"") != 0)
			{
				selectWeaponUI->SetClickedCallBack(SelectWeapon
					, reinterpret_cast<DWORD_PTR>(pTag)
					, reinterpret_cast<DWORD_PTR>(nullptr));
			}


			selectWeaponUI->SetPos(startPos + Vec2((selectWeaponUI->GetScale().x + LR_interval) * j
				, (selectWeaponUI->GetScale().y + UD_interval) * i));
			selectWeaponUI->SetIsRound(true, 10.f, 10.f);
			selectWeaponUI->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(31, 31, 31));

			AddObject(selectWeaponUI, GROUP_TYPE::UI);
		}
	}
	//////////////////////////캐릭터들 UI////////////////////////



}

void CScene_Select_Weapon::Exit()
{
	DeleteAll();
}

void ShowWeaponInfo(DWORD_PTR lParam, DWORD_PTR wParam)
{
	//lParam -> 이미지
	//wParam -> 버튼에서 자체적으로 on이 해제되면 panel과 image를 delete 시키기 위해 저장해둘 벡터 포인터
	printf("tesT\n");
	CImage* image = reinterpret_cast<CImage*>(lParam);
	vector<CObject*>* tempObjects = reinterpret_cast<vector<CObject*>*>(wParam);

	if (image == nullptr || tempObjects == nullptr) return;

	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	CPanelUI* panel = new CPanelUI;
	panel->SetScale(Vec2(186.f, 250.f));
	panel->SetPos(Vec2(vResolution.x / 2.f + 3.f + panel->GetScale().x / 2.f, 210.f));
	panel->SetName(L"Parent3");
	panel->SetRadius(10.f, 10.f);
	panel->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));

	CObject* characterImage = new CSpriteUI;
	characterImage->AddImage(image->GetBitmap());
	Vec2 vPos = Vec2(35.f, 35.f) - (panel->GetScale() / 2.f);
	characterImage->GetImage(0)->SetOffset(vPos);
	characterImage->SetObjType(GROUP_TYPE::UI);
	characterImage->SetName(L"Child3");
	characterImage->SetScale(Vec2(48.f, 48.f));
	characterImage->SetPos(panel->GetPos());

	panel->AddChild((CUI*)characterImage);

	CSceneMgr::GetInstance()->GetCurScene()->AddObject(panel, GROUP_TYPE::UI);

	tempObjects->push_back(panel);
}

void SelectWeapon(DWORD_PTR lParam, DWORD_PTR wParam)
{
	// lParam -> 이미지 태그
	// wParam -> x
	wstring tag = *reinterpret_cast<wstring*>(lParam);

	Item* selectedItem = ItemMgr::GetInstance()->GetItem(tag);
	
	CWeapon* selectedWeapon = new CWeapon;
	selectedWeapon->SetInfo(selectedItem->m_tWeaponInfo);
	static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->AddWeapon(selectedWeapon);

	ChangeScene(SCENE_TYPE::START);
}


void CScene_Select_Weapon::WeaponMapping(CBtnUI* _btn, int i, int j, wstring& tag)
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	if (i == 0 && j == 0)
	{
		_btn->AddImage(pD2DMgr->GetStoredBitmap(L"random_icon"));
		tag = L"random";
	}
	else if (i == 0 && j == 1)
	{
		_btn->AddImage(pD2DMgr->GetStoredBitmap(L"knife_icon"));
		tag = L"knife";
	}
	else if (i == 0 && j == 2)
	{
		_btn->AddImage(pD2DMgr->GetStoredBitmap(L"pistol_icon"));
		tag = L"pistol";
	}
	else if (i == 0 && j == 3)
	{
		_btn->AddImage(pD2DMgr->GetStoredBitmap(L"slingshot_icon"));
		tag = L"slingshot";
	}
	else _btn->AddImage(pD2DMgr->GetStoredBitmap(L"locked_icon"));
}