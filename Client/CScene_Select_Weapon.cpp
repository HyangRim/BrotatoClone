#include "pch.h"

#include "CScene_Select_Weapon.h"
#include "CScene.h"
#include "CCore.h"
#include "CSpriteUI.h"
#include "CBtnUI.h"
#include "CPanelUI.h"
#include "CWeapon.h"
#include "CPistol.h"
#include "CSlingshot.h"
#include "CTextUI.h"
#include "CKnife.h"
#include "CImage.h"
#include "CPlayer.h"

#include "ItemMgr.h"
#include "CSceneMgr.h"
#include "Direct2DMgr.h"

CScene_Select_Weapon::CScene_Select_Weapon()
{

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
	CreateBackGround(pD2DMgr, vResolution);
	/////////////////뒷 배경 오브젝트/////////////////

	/////////////////뒤로 가기 버틈///////////////////
	CreateBackBtn();
	/////////////////뒤로 가기 버틈///////////////////

	/////////////////무기선택 UI(상단)///////////////////
	CreateWeaponSelectText(vResolution);
	/////////////////무기선택 UI(상단)///////////////////

	/////////////////선택한 캐릭터//////////////////////
	CreateSelectedCharacterInfoPanel(pD2DMgr, vResolution);
	/////////////////선택한 캐릭터//////////////////////

	//////////////////////////무기들 UI////////////////////////
	CreateWeaponItemBtn();
	//////////////////////////무기들 UI////////////////////////
}

void CScene_Select_Weapon::Exit()
{
	DeleteAll();
}

void CScene_Select_Weapon::BackBtnCallBack()
{
	ItemMgr::GetInstance()->SetBasicCharacter(nullptr);
	ItemMgr::GetInstance()->m_vPassiveItems.pop_back();
	ItemMgr::GetInstance()->m_bBaseCharacterIsAdded = false;
	ChangeScene(SCENE_TYPE::SELECT_CHARACTER);
}

void CScene_Select_Weapon::CreateBackGround(Direct2DMgr* _pD2DMgr, Vec2 _vResolution)
{
	/////////////////뒷 배경 오브젝트/////////////////
	CObject* backGround = new CSpriteUI;
	backGround->SetObjType(GROUP_TYPE::DEFAULT);
	backGround->AddImage(_pD2DMgr->GetStoredBitmap(L"shop_background"));
	backGround->SetPos(_vResolution / 2.f);
	backGround->SetScale(_vResolution);
	AddObject(backGround, GROUP_TYPE::DEFAULT);
	/////////////////뒷 배경 오브젝트/////////////////
}

void CScene_Select_Weapon::CreateBackBtn()
{
	/////////////////뒤로 가기 버틈///////////////////
	CBtnUI* backBtn = new CBtnUI;
	backBtn->SetName(L"BackBtn");
	backBtn->SetObjType(GROUP_TYPE::UI);
	backBtn->SetScale(Vec2(94.f, 28.f));
	backBtn->SetPos(Vec2(71.f, 38.f));
	backBtn->SetIsRound(true, 10.f, 10.f);
	backBtn->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(28, 28, 28));
	backBtn->SetClickedCallBack(this, static_cast<SCENE_MEMFUNC>(&CScene_Select_Weapon::BackBtnCallBack));
	backBtn->CreateTextUI(L"뒤로", Vec2(-47.f, -14.f), Vec2(47.f, 14.f)
		, 12, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(backBtn, GROUP_TYPE::UI);
	/////////////////뒤로 가기 버틈///////////////////
}

void CScene_Select_Weapon::CreateWeaponSelectText(Vec2 _vResolution)
{
	/////////////////무기선택 UI(상단)///////////////////
	CObject* panelTextSelectWeapon = new CSpriteUI;
	panelTextSelectWeapon->SetObjType(GROUP_TYPE::UI);
	panelTextSelectWeapon->SetPos(Vec2(_vResolution.x / 2.f, 50.f));
	panelTextSelectWeapon->SetScale(Vec2(300.f, 36.f));
	panelTextSelectWeapon->CreateTextUI(L"무기 선택", Vec2(-150.f, -18.f), Vec2(150.f, 18.f)
		, 30, D2D1::ColorF::White, true, 2.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(panelTextSelectWeapon, GROUP_TYPE::UI);
	/////////////////무기선택 UI(상단)///////////////////
}

void CScene_Select_Weapon::CreateSelectedCharacterInfoPanel(Direct2DMgr* _pD2DMgr, Vec2 _vResolution)
{
	wchar_t buffer[20];

	/////////////////선택한 캐릭터//////////////////////
	CPanelUI* panel = new CPanelUI;
	panel->SetScale(Vec2(186.f, 250.f));
	panel->SetObjType(GROUP_TYPE::UI);
	panel->SetPos(Vec2(_vResolution.x / 2.f - 3.f - panel->GetScale().x / 2.f, 210.f));
	panel->SetName(L"Parent2");
	panel->SetRadius(10.f, 10.f);
	panel->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));

	ItemMgr* ItemMgr = ItemMgr::GetInstance();
	Item* characterInfo = ItemMgr->GetBasicCharacter();
	Vec2 vPos = Vec2(35.f, 35.f) - (panel->GetScale() / 2.f);


	CSpriteUI* characterImage = panel->AddChild<CSpriteUI>(vPos);
	wstring iconTag = ItemMgr->GetBasicCharacter()->tag + L"_icon";
	characterImage->AddImage(_pD2DMgr->GetStoredBitmap(iconTag));
	characterImage->SetObjType(GROUP_TYPE::UI);
	characterImage->SetName(L"Child2");
	characterImage->SetScale(Vec2(48.f, 48.f));
	AddObject(panel, GROUP_TYPE::UI);

	CSpriteUI* characterName = panel->AddChild<CSpriteUI>(Vec2(32.f, -(panel->GetScale().y / 2.f) + 11.f + 10.f));
	characterName->SetScale(Vec2(116.f, 20.f));
	swprintf_s(buffer, L"%s", characterInfo->m_sName.c_str());
	characterName->CreateTextUI(buffer, -(characterName->GetScale() / 2.f), characterName->GetScale() / 2.f
		, 12, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	characterName->GetTextUI()->SetHorizontal(1);

	//y축 offset설명 11->panel과의 마진 , 10->characterName 절반크기, 5->위에 객체와 간격, 10->이 객체 크기 절반
	CSpriteUI* itemType = panel->AddChild<CSpriteUI>(Vec2(32.f, -(panel->GetScale().y / 2.f) + 11.f + 10.f + 5.f + 10.f));
	itemType->SetScale(Vec2(116.f, 20.f));
	swprintf_s(buffer, L"캐릭터");
	itemType->CreateTextUI(buffer, -(itemType->GetScale() / 2.f), itemType->GetScale() / 2.f
		, 12, ColorNormalize(202, 195, 152), true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	itemType->GetTextUI()->SetHorizontal(1);

	/////////////////////////캐릭터 정보(스탯)/////////////////////////////////
	// 
	 /*
	struct {
		float   m_fDefaultSpeed;    // 기본 스피드
		int     m_iAddMaxHP;         // + 최대 체력
		float   m_fDamageCoef;      // 최종 데미지 %
		float   m_fMeleeCoef;       // 근거리 최종 데미지 %
		float   m_fRangeCoef;       // 원거리 최종 데미지 %
		float   m_fAttackSpeedCoef; // 공격 속도 %
		int     m_iCriticalAcc;     // 크리티컬 확률 %
		float   m_fSpeed;           // 속도 계수
	} PassiveStats;                 // 패시브 아이템 관련 정보 (PASSIVE 타입일 때 사용)
	*/
	//총 스탯 갯수
	int tmp = 0;
	for (int i = 0; i < 8; i++)
	{
		if (i == 0 && characterInfo->PassiveStats.m_fDefaultSpeed == 0.f) continue;
		else if (i == 1 && characterInfo->PassiveStats.m_iAddMaxHP == 0) continue;
		else if (i == 2 && characterInfo->PassiveStats.m_fDamageCoef == 0.f) continue;
		else if (i == 3 && characterInfo->PassiveStats.m_fMeleeCoef == 0.f) continue;
		else if (i == 4 && characterInfo->PassiveStats.m_fRangeCoef == 0.f) continue;
		else if (i == 5 && characterInfo->PassiveStats.m_fAttackSpeedCoef == 0.f) continue;
		else if (i == 6 && characterInfo->PassiveStats.m_iCriticalAcc == 0) continue;
		else if (i == 7 && characterInfo->PassiveStats.m_fSpeed == 0.f) continue;

		CPanelUI* statpanel = panel->AddChild<CPanelUI>(Vec2(0.f, -50.f + tmp * 21.f));
		statpanel->SetScale(Vec2(164.f, 16.f));
		statpanel->SetRadius(0.f, 0.f);
		//statpanel->SetColor(ColorNormalize(255, 255, 255), ColorNormalize(255, 255, 255));
		statpanel->SetMouseOnAlpha(0.f);
		statpanel->SetNormalAlpha(0.f);


		CSpriteUI* statNumber = statpanel->AddChild<CSpriteUI>(Vec2(-statpanel->GetScale().x / 2.f + 8.f, 0.f));
		statNumber->SetScale(Vec2(26.f, 16.f));
		if (i == 0) { swprintf_s(buffer, L"+%d", (int)characterInfo->PassiveStats.m_fDefaultSpeed); }
		else if (i == 1) { swprintf_s(buffer, L"+%d", characterInfo->PassiveStats.m_iAddMaxHP); }
		else if (i == 2) { swprintf_s(buffer, L"+%d%%", (int)characterInfo->PassiveStats.m_fDamageCoef); }
		else if (i == 3) { swprintf_s(buffer, L"+%d%%", (int)characterInfo->PassiveStats.m_fMeleeCoef); }
		else if (i == 4) { swprintf_s(buffer, L"+%d%%", (int)characterInfo->PassiveStats.m_fRangeCoef); }
		else if (i == 5) { swprintf_s(buffer, L"+%d%%", (int)characterInfo->PassiveStats.m_fAttackSpeedCoef); }
		else if (i == 6) { swprintf_s(buffer, L"+%d", characterInfo->PassiveStats.m_iCriticalAcc); }
		else if (i == 7) { swprintf_s(buffer, L"+%d", (int)characterInfo->PassiveStats.m_fSpeed); }
		statNumber->CreateTextUI(buffer, -(statNumber->GetScale() / 2.f), statNumber->GetScale() / 2.f
			, 10, D2D1::ColorF::Green, true, 1.f, D2D1::ColorF::Black
			, FONT_TYPE::KR
			, TextUIMode::TEXT
			, 0);
		statNumber->GetTextUI()->SetHorizontal(1);

		CSpriteUI* statText = statpanel->AddChild<CSpriteUI>(Vec2(0.f, 0.f));
		statText->SetScale(Vec2(120.f, 16.f));
		if (i == 0) { swprintf_s(buffer, L"기본속도"); }
		else if (i == 1) { swprintf_s(buffer, L"최대체력"); }
		else if (i == 2) { swprintf_s(buffer, L"최종데미지"); }
		else if (i == 3) { swprintf_s(buffer, L"근거리 최종데미지"); }
		else if (i == 4) { swprintf_s(buffer, L"원거리 최종데미지"); }
		else if (i == 5) { swprintf_s(buffer, L"공격속도"); }
		else if (i == 6) { swprintf_s(buffer, L"치명타 확률"); }
		else if (i == 7) { swprintf_s(buffer, L"속도계수"); }
		statText->CreateTextUI(buffer, -(statText->GetScale() / 2.f), statText->GetScale() / 2.f
			, 10, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
			, FONT_TYPE::KR
			, TextUIMode::TEXT
			, 0);
		statText->GetTextUI()->SetHorizontal(1);


		tmp++;
	}
	/////////////////////////캐릭터 정보(스탯)/////////////////////////////////
	/////////////////선택한 캐릭터//////////////////////
}

void CScene_Select_Weapon::CreateWeaponItemBtn()
{
	//////////////////////////무기들 UI////////////////////////
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
				, reinterpret_cast<DWORD_PTR>(pTag)
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
	//////////////////////////무기들 UI////////////////////////
}

void ShowWeaponInfo(DWORD_PTR lParam, DWORD_PTR wParam)
{
	//lParam -> 태그(x _icon)
	//wParam -> 버튼에서 자체적으로 on이 해제되면 panel과 image를 delete 시키기 위해 저장해둘 벡터 포인터
	printf("tesT\n");
	wstring tag = *reinterpret_cast<wstring *>(lParam);
	vector<CObject*>* tempObjects = reinterpret_cast<vector<CObject*>*>(wParam);
	Item* weaponInfo = ItemMgr::GetInstance()->GetItem(tag);
	wchar_t buffer[20];

	if (weaponInfo == nullptr || tempObjects == nullptr) return;

	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	CPanelUI* panel = new CPanelUI;
	panel->SetScale(Vec2(186.f, 250.f));
	panel->SetPos(Vec2(vResolution.x / 2.f + 3.f + panel->GetScale().x / 2.f, 210.f));
	panel->SetName(L"Parent3");
	panel->SetRadius(10.f, 10.f);
	panel->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));

	Vec2 vPos = Vec2(35.f, 35.f) - (panel->GetScale() / 2.f);
	CSpriteUI* weaponImage = panel->AddChild<CSpriteUI>(vPos);
	wstring iconTag = tag + L"_icon";
	weaponImage->AddImage(pD2DMgr->GetStoredBitmap(iconTag));
	weaponImage->SetObjType(GROUP_TYPE::UI);
	weaponImage->SetBackGround(true);
	weaponImage->SetIsRound(true, 10.f, 10.f);
	weaponImage->SetBackGroundColor(ColorNormalize(30, 30, 30), ColorNormalize(0, 0, 0)
		, ColorNormalize(30, 30, 30), ColorNormalize(0, 0, 0));
	weaponImage->SetName(L"Child3");
	weaponImage->SetScale(Vec2(48.f, 48.f));

	CSpriteUI* weaponName = panel->AddChild<CSpriteUI>(Vec2(32.f, -(panel->GetScale().y / 2.f) + 11.f + 10.f));
	weaponName->SetScale(Vec2(116.f, 20.f));
	swprintf_s(buffer, L"%s", weaponInfo->m_sName.c_str());
	weaponName->CreateTextUI(buffer, -(weaponName->GetScale() / 2.f), weaponName->GetScale() / 2.f
		, 12, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	weaponName->GetTextUI()->SetHorizontal(1);

	//y축 offset설명 11->panel과의 마진 , 10->characterName 절반크기, 5->위에 객체와 간격, 10->이 객체 크기 절반
	CSpriteUI* itemType = panel->AddChild<CSpriteUI>(Vec2(32.f, -(panel->GetScale().y / 2.f) + 11.f + 10.f + 5.f + 10.f));
	itemType->SetScale(Vec2(116.f, 20.f));
	swprintf_s(buffer, L"무기");
	itemType->CreateTextUI(buffer, -(itemType->GetScale() / 2.f), itemType->GetScale() / 2.f
		, 12, ColorNormalize(202, 195, 152), true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	itemType->GetTextUI()->SetHorizontal(1);


	/////////////////////////캐릭터 정보(스탯)/////////////////////////////////
		/*
		struct tWeaponInfo {
			wstring         m_sName;            //무기 이름
			wstring         m_sIconImageKey;    //무기 이미지 Key값.
			WEAPON_TYPE     m_eType;            //무기 타입

			int             m_iDMG;             //무기 기본 데미지
			float           m_iMeleeCoef;       //근거리 데미지 계수
			float           m_iRangeCoef;       //원거리 데미지 계수
			float           m_fCritialDMG;      //크리티컬 데미지 계수
			float           m_fCritialAcc;      //크리티컬 데미지 확률
			float           m_fCooldown;        //무기 공격 쿨다운
			float           m_fRecogRange;      //무기 적 인식범위
			int             m_iPenet;           //무기 튕김 수치.
		};*/
	int tmp = 0;
	int maxStatCount;
	if (weaponInfo->m_eItemType == ITEM_TYPE::WEAPON) maxStatCount = 8;
	else if (weaponInfo->m_eItemType == ITEM_TYPE::PASSIVE) maxStatCount = 8;
	for (int i = 0; i < maxStatCount; i++)
	{
		if (i == 0 && weaponInfo->m_tWeaponInfo.m_iDMG == 0) continue;
		else if (i == 1 && weaponInfo->m_tWeaponInfo.m_fMeleeCoef == 0.f) continue;
		else if (i == 2 && weaponInfo->m_tWeaponInfo.m_fRangeCoef == 0) continue;
		else if (i == 3 && weaponInfo->m_tWeaponInfo.m_fCritialDMG == 0.f) continue;
		else if (i == 4 && weaponInfo->m_tWeaponInfo.m_fCritialAcc == 0.f) continue;
		else if (i == 5 && weaponInfo->m_tWeaponInfo.m_fCooldown == 0.f) continue;
		else if (i == 6 && weaponInfo->m_tWeaponInfo.m_fRecogRange == 0.f) continue;
		else if (i == 7 && weaponInfo->m_tWeaponInfo.m_iPenet == 0) continue;

		CPanelUI* statpanel = panel->AddChild<CPanelUI>(Vec2(0.f, -50.f + tmp * 21.f));
		statpanel->SetScale(Vec2(154.f, 16.f));
		statpanel->SetRadius(0.f, 0.f);
		//statpanel->SetColor(ColorNormalize(255, 255, 255), ColorNormalize(255, 255, 255));
		statpanel->SetMouseOnAlpha(0.f);
		statpanel->SetNormalAlpha(0.f);

		CSpriteUI* statText = statpanel->AddChild<CSpriteUI>(Vec2(-statpanel->GetScale().x / 2.f + 50.f, 0.f));
		statText->SetScale(Vec2(100.f, 16.f));
		if (i == 0) { swprintf_s(buffer, L"데미지:"); }
		else if (i == 1) { swprintf_s(buffer, L"근거리 데미지:"); }
		else if (i == 2) { swprintf_s(buffer, L"원거리 데미지:"); }
		else if (i == 3) { swprintf_s(buffer, L"치명타 데미지:"); }
		else if (i == 4) { swprintf_s(buffer, L"치명타 확률:"); }
		else if (i == 5) { swprintf_s(buffer, L"공격속도:"); }
		else if (i == 6) { swprintf_s(buffer, L"범위:"); }
		else if (i == 7) { swprintf_s(buffer, L"튕기는 횟수:"); }
		statText->CreateTextUI(buffer, -(statText->GetScale() / 2.f), statText->GetScale() / 2.f
			, 10, ColorNormalize(202, 195, 152), true, 1.f, D2D1::ColorF::Black
			, FONT_TYPE::KR
			, TextUIMode::TEXT
			, 0);
		//statText->SetBackGround(true);
		//statText->SetBackGroundColor(ColorNormalize(255, 255, 255), ColorNormalize(255, 255, 255)
		//	, ColorNormalize(255, 255, 255), ColorNormalize(255, 255, 255));
		statText->GetTextUI()->SetHorizontal(1);
		
		CSpriteUI* statNumber = statpanel->AddChild<CSpriteUI>(Vec2(10.f, -1.f));
		statNumber->SetScale(Vec2(26.f, 16.f));
		if (i == 0)		 { swprintf_s(buffer, L"+%d", weaponInfo->m_tWeaponInfo.m_iDMG); }
		else if (i == 1) { swprintf_s(buffer, L"+%d", (int)weaponInfo->m_tWeaponInfo.m_fMeleeCoef); }
		else if (i == 2) { swprintf_s(buffer, L"+%d", (int)weaponInfo->m_tWeaponInfo.m_fRangeCoef); }
		else if (i == 3) { swprintf_s(buffer, L"+%d%%", (int)weaponInfo->m_tWeaponInfo.m_fCritialDMG); }
		else if (i == 4) { swprintf_s(buffer, L"+%d%%", (int)weaponInfo->m_tWeaponInfo.m_fCritialAcc); }
		else if (i == 5) { swprintf_s(buffer, L"+%d%%", (int)weaponInfo->m_tWeaponInfo.m_fCooldown); }
		else if (i == 6) { swprintf_s(buffer, L"+%d%%", (int)weaponInfo->m_tWeaponInfo.m_fRecogRange); }
		else if (i == 7) { swprintf_s(buffer, L"+%d", weaponInfo->m_tWeaponInfo.m_iPenet); }
		statNumber->CreateTextUI(buffer, -(statNumber->GetScale() / 2.f), statNumber->GetScale() / 2.f
			, 10, D2D1::ColorF::Green, true, 1.f, D2D1::ColorF::Black
			, FONT_TYPE::KR
			, TextUIMode::TEXT
			, 0);
		//statNumber->SetBackGround(true);
		//statNumber->SetBackGroundColor(ColorNormalize(255, 0, 0), ColorNormalize(255, 0, 0)
		//	, ColorNormalize(255, 0, 0), ColorNormalize(255, 0, 0));
		statNumber->GetTextUI()->SetHorizontal(1);
		
	
		tmp++;
	}
	/////////////////////////캐릭터 정보(스탯)/////////////////////////////////







	CSceneMgr::GetInstance()->GetCurScene()->AddObject(panel, GROUP_TYPE::UI);
	tempObjects->push_back(panel);
}

void SelectWeapon(DWORD_PTR lParam, DWORD_PTR wParam)
{
	// lParam -> 이미지 태그
	// wParam -> x
	wstring tag = *reinterpret_cast<wstring*>(lParam);

	if (tag.compare(L"random") == 0)
	{
		int random_weapon = rand() % weapon_tag_list.size();
		tag = weapon_tag_list[random_weapon];
	}

	Item* selectedItem = ItemMgr::GetInstance()->GetItem(tag);
	
	//CWeapon* selectedWeapon = new CWeapon;
	CWeapon* selectedWeapon = nullptr;
	if (tag == L"pistol") {
		selectedWeapon = new CPistol;
	}
	else if (tag == L"knife") {
		selectedWeapon = new CKnife;
	}
	else if (tag == L"slingshot") {
		selectedWeapon = new CSlingshot;
	}
	//selectedWeapon->SetInfo(selectedItem->m_tWeaponInfo);
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