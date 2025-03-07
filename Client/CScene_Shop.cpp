#include "pch.h"

#include "CSpriteUI.h"
#include "CPlayer.h"
#include "CObject.h"
#include "CBtnUI.h"
#include "CImage.h"
#include "CObject.h"
#include "CPanelUI.h"
#include "CWeapon.h"
#include "CTextUI.h"

#include "Direct2DMgr.h"
#include "ItemMgr.h"
#include "CkeyMgr.h"
#include "CWaveMgr.h"
#include "CCore.h"
#include "CSceneMgr.h"

#include "CScene_Shop.h"

constexpr const wchar_t RestCoinString[20] = L"RestCoin";
constexpr const wchar_t ItemInfoPanelString[20] = L"ItemInfoPanel";
constexpr const wchar_t WeaponInfoPanelString[20] = L"WeaponInfoPanel";

CScene_Shop::CScene_Shop()
{

}

CScene_Shop::~CScene_Shop()
{

}

void CScene_Shop::update()
{
	CScene::update();

	wchar_t buffer[20];
	for (UINT typeIDX = 0; typeIDX < (UINT)GROUP_TYPE::END; typeIDX++) 
	{
		const vector<CObject*>& vecObj = GetGroupObject((GROUP_TYPE)typeIDX);
		
		///////////////////////////////////////////
		for (size_t objIDX = 0; objIDX < vecObj.size(); objIDX++) 
		{
			if (!vecObj[objIDX]->IsDead()) 
			{
				if (vecObj[objIDX]->GetName().compare(RestCoinString) == 0)
				{
					swprintf_s(buffer, L"%d", ((CPlayer*)CSceneMgr::GetInstance()->GetPlayer())->GetCharacterParam().m_iCoin);
					vecObj[objIDX]->GetTextUI()->SetText(buffer);
				}
				if (vecObj[objIDX]->GetName().compare(ItemInfoPanelString) == 0)
				{
					swprintf_s(buffer, L"아이템 %d개", (int)ItemMgr::GetInstance()->GetPassiveItemssize());
					vecObj[objIDX]->GetTextUI()->SetText(buffer);
				}
				if (vecObj[objIDX]->GetName().compare(WeaponInfoPanelString) == 0)
				{
					swprintf_s(buffer, L"무기(%d/6)", (int)((CPlayer*)CSceneMgr::GetInstance()->GetPlayer())->GetWeaponCount());
					vecObj[objIDX]->GetTextUI()->SetText(buffer);
				}

				vecObj[objIDX]->update();
			}
		}
	}
}

void CScene_Shop::render(ID2D1HwndRenderTarget* _pRender)
{
	CScene::render(_pRender);
	RenderScrollArea(_pRender);
}

void CScene_Shop::Enter()
{
	m_vItemPanels.clear();
	
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	wchar_t buffer[20];
	size_t bufferSize = sizeof(buffer) / sizeof(wchar_t);

	/////////////////뒷 배경 오브젝트/////////////////
	CreateBackGround(pD2DMgr, vResolution);
	/////////////////뒷 배경 오브젝트/////////////////

	/////////////////상점 웨이브 글자 UI(좌상단)///////////////////
	CreateShopWaveTextUI(buffer, bufferSize);
	/////////////////상점 웨이브 글자 UI(좌상단)///////////////////

	/////////////////아이템 글자 UI(우하단)///////////////////
	CreateItemTextUI(buffer, bufferSize);
	/////////////////아이템 글자 UI(우하단)///////////////////

	///////////////////////////무기 글자 UI///////////////////
	CreateWeaponTextUI(buffer, bufferSize);
	///////////////////////////무기 글자 UI///////////////////

	/////////////////초기화 버튼/////////////////
	CreateResetBtn(pD2DMgr);
	/////////////////초기화 버튼/////////////////

	///////////////////////////남은 재화 표시///////////////////
	CreateRemainingCoinText(buffer, bufferSize);
	///////////////////////////남은 재화 표시///////////////////

	///////////////////////////남은 재화 표시 이미지///////////////////
	CreateRemainingCoinImage(pD2DMgr);
	///////////////////////////남은 재화 표시 이미지///////////////////

	/////////////////다음 웨이브 버튼/////////////////
	CreateNextWaveBtn(buffer, bufferSize);
	/////////////////다음 웨이브 버튼/////////////////

	////////////////////////가운데 아이템 패널 4개////////////////////////////////
	CreateShowItemPanels(pD2DMgr, buffer, bufferSize);
	////////////////////////가운데 아이템 패널 4개////////////////////////////////

	//////보유무기//////////
	CreatePossessedWeapons(pD2DMgr);
	//////보유무기//////////

	//////스텟창///////////
	CreateInfoPanel(Vec2(852.f, 204.f));
	//////스텟창///////////

	/////보유아이템 영역/////////
	CreateScrollArea();
	/////보유아이템 영역/////////
}

void CScene_Shop::Exit()
{
	Safe_Delete_Vec(m_scrollContent);
	DeleteAll();
}

void CScene_Shop::CreateBackGround(Direct2DMgr* _pD2DMgr, Vec2 _vResolution)
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

void CScene_Shop::CreateShopWaveTextUI(wchar_t* buffer, size_t bufferSize)
{

	/////////////////상점 웨이브 글자 UI(좌상단)///////////////////
	CObject* panelTextShopAndWave = new CSpriteUI;
	panelTextShopAndWave->SetObjType(GROUP_TYPE::UI);
	panelTextShopAndWave->SetPos(Vec2(78.f, 32.f));
	panelTextShopAndWave->SetScale(Vec2(130.f, 32.f));

	swprintf_s(buffer, bufferSize, L"상점(웨이브 %d)", CWaveMgr::GetInstance()->GetLevel() - 1);
	panelTextShopAndWave->CreateTextUI(buffer, Vec2(-150.f, -18.f), Vec2(150.f, 18.f)
		, 20, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(panelTextShopAndWave, GROUP_TYPE::UI);
	/////////////////상점 웨이브 글자 UI(좌상단)///////////////////
}

void CScene_Shop::CreateItemTextUI(wchar_t* buffer, size_t bufferSize)
{
	
	/////////////////아이템 글자 UI(우하단)///////////////////
	CObject* panelTextItem = new CSpriteUI;
	panelTextItem->SetName(L"ItemInfoPanel");
	panelTextItem->SetObjType(GROUP_TYPE::UI);
	panelTextItem->SetPos(Vec2(60.f, 386.f));
	panelTextItem->SetScale(Vec2(105.f, 30.f));

	swprintf_s(buffer, bufferSize, L"아이템 %d개", (int)ItemMgr::GetInstance()->GetPassiveItemssize());
	panelTextItem->CreateTextUI(buffer, Vec2(-150.f, -18.f), Vec2(150.f, 18.f)
		, 20, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(panelTextItem, GROUP_TYPE::UI);
	/////////////////아이템 글자 UI(우하단)///////////////////
}

void CScene_Shop::CreateWeaponTextUI(wchar_t* buffer, size_t bufferSize)
{
	///////////////////////////무기 글자 UI///////////////////
	CObject* panelTextWeapon = new CSpriteUI;
	panelTextWeapon->SetName(L"WeaponInfoPanel");
	panelTextWeapon->SetObjType(GROUP_TYPE::UI);
	panelTextWeapon->SetPos(Vec2(608.f, 386.f));
	panelTextWeapon->SetScale(Vec2(90.f, 30.f));

	swprintf_s(buffer, bufferSize, L"무기(%d/6)", (int)((CPlayer*)CSceneMgr::GetInstance()->GetPlayer())->GetWeaponCount());
	panelTextWeapon->CreateTextUI(buffer, Vec2(-150.f, -18.f), Vec2(150.f, 18.f)
		, 20, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(panelTextWeapon, GROUP_TYPE::UI);
	///////////////////////////무기 글자 UI///////////////////
}

void CScene_Shop::CreateResetBtn(Direct2DMgr* _pD2DMgr)
{
	/////////////////초기화 버튼/////////////////
	CBtnUI* resetBtn = new CBtnUI;
	resetBtn->SetName(L"ResetBtn");
	resetBtn->SetObjType(GROUP_TYPE::UI);
	resetBtn->SetScale(Vec2(134.f, 42.f));
	resetBtn->SetPos(Vec2(662.f, 32.f));
	resetBtn->SetIsRound(true, 10.f, 10.f);
	resetBtn->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(0, 0, 0));
	resetBtn->SetClickedCallBack(this, static_cast<SCENE_MEMFUNC>(&CScene_Shop::ReRollItem));
	resetBtn->CreateTextUI(L"초기화 -1", -(resetBtn->GetScale() / 2.f), resetBtn->GetScale() / 2.f
		, 16, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);

	CSpriteUI* coinImage = resetBtn->AddChild<CSpriteUI>(Vec2(46.1f, 1.f));
	coinImage->AddImage(_pD2DMgr->GetStoredBitmap(L"harvesting_icon"));
	coinImage->SetObjType(GROUP_TYPE::UI);
	coinImage->SetName(L"Child");
	coinImage->SetScale(Vec2(24.f, 24.f));
	coinImage->SetPos(resetBtn->GetPos());

	AddObject(resetBtn, GROUP_TYPE::UI);
	/////////////////초기화 버튼/////////////////
}

void CScene_Shop::CreateRemainingCoinText(wchar_t* buffer, size_t bufferSize)
{
	///////////////////////////남은 재화 표시///////////////////
	CObject* panelTextCurCoin = new CSpriteUI;
	panelTextCurCoin->SetName(L"RestCoin");
	panelTextCurCoin->SetObjType(GROUP_TYPE::UI);
	panelTextCurCoin->SetPos(Vec2(386.f, 32.f));
	panelTextCurCoin->SetScale(Vec2(32.f, 22.f));
	swprintf_s(buffer, bufferSize, L"%d", ((CPlayer*)CSceneMgr::GetInstance()->GetPlayer())->GetCharacterParam().m_iCoin);
	panelTextCurCoin->CreateTextUI(buffer, -(panelTextCurCoin->GetScale() / 2.f), panelTextCurCoin->GetScale() / 2.f
		, 16, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(panelTextCurCoin, GROUP_TYPE::UI);
	///////////////////////////남은 재화 표시///////////////////
}

void CScene_Shop::CreateRemainingCoinImage(Direct2DMgr* _pD2DMgr)
{
	///////////////////////////남은 재화 표시 이미지///////////////////
	CObject* panelTextCurCoinImage = new CSpriteUI;
	panelTextCurCoinImage->SetObjType(GROUP_TYPE::UI);
	panelTextCurCoinImage->AddImage(_pD2DMgr->GetStoredBitmap(L"harvesting_icon"));
	panelTextCurCoinImage->SetPos(Vec2(356.f, 32.f));
	panelTextCurCoinImage->SetScale(Vec2(24.f, 24.f));
	AddObject(panelTextCurCoinImage, GROUP_TYPE::UI);
	///////////////////////////남은 재화 표시 이미지///////////////////
}

void CScene_Shop::CreateNextWaveBtn(wchar_t* buffer, size_t bufferSize)
{
	/////////////////다음 웨이브 버튼/////////////////
	CBtnUI* nextWaveBtn = new CBtnUI;
	nextWaveBtn->SetName(L"NextWaveBtn");
	nextWaveBtn->SetObjType(GROUP_TYPE::UI);
	nextWaveBtn->SetScale(Vec2(206.f, 34.f));
	nextWaveBtn->SetPos(Vec2(844.f, 508.f));
	nextWaveBtn->SetIsRound(true, 10.f, 10.f);
	nextWaveBtn->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(0, 0, 0));
	nextWaveBtn->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::START, 0);
	swprintf_s(buffer, bufferSize, L"이동(웨이브%d)", CWaveMgr::GetInstance()->GetLevel());
	nextWaveBtn->CreateTextUI(buffer, -(nextWaveBtn->GetScale() / 2.f), nextWaveBtn->GetScale() / 2.f
		, 16, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(nextWaveBtn, GROUP_TYPE::UI);
	/////////////////다음 웨이브 버튼/////////////////
}

void CScene_Shop::CreateShowItemPanels(Direct2DMgr* _pD2DMgr, wchar_t* buffer, size_t bufferSize)
{
	for (int i = 0; i < 4; i++)
	{
		item_numbers.push_back(rand() % item_tag_list.size());
	}

	for (int i = 0; i < 4; i++)
	{
		wstring iconTag = item_tag_list[item_numbers[i]] + L"_icon";
		Item* item = ItemMgr::GetInstance()->GetItem(item_tag_list[item_numbers[i]]);

		CPanelUI* panelItemUI = new CPanelUI;
		panelItemUI->SetName(iconTag);
		panelItemUI->SetScale(Vec2(176.f, 242.f));
		panelItemUI->SetPos(Vec2(100.f + i * (5.f + panelItemUI->GetScale().x), 204.f));
		panelItemUI->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));

		////////////////////아이템 이미지/////////////////////////////
		CSpriteUI* itemImage = panelItemUI->AddChild<CSpriteUI>(Vec2(35.f, 35.f) - (panelItemUI->GetScale() / 2.f));
		itemImage->AddImage(_pD2DMgr->GetStoredBitmap(iconTag));
		itemImage->SetObjType(GROUP_TYPE::UI);
		itemImage->SetName(L"Child");
		itemImage->SetScale(Vec2(48.f, 48.f));
		itemImage->SetBackGround(true);
		itemImage->SetBackGroundColor(ColorNormalize(68, 68, 68), ColorNormalize(68, 68, 68)
			, ColorNormalize(68, 68, 68), ColorNormalize(68, 68, 68));
		itemImage->SetIsRound(true, 10.f, 10.f);
		////////////////////아이템 이미지/////////////////////////////

		/////////////////////구매 버튼/////////////////////////////////
		CBtnUI* priceBtn = panelItemUI->AddChild<CBtnUI>(Vec2(0.f, panelItemUI->GetScale().y / 2.f - 8.f - 17.f));
		priceBtn->SetName(L"PriceBtn");
		priceBtn->SetScale(Vec2(80.f, 34.f));
		priceBtn->SetObjType(GROUP_TYPE::UI);
		priceBtn->SetColor(ColorNormalize(255, 255, 255), ColorNormalize(30, 30, 30));
		priceBtn->SetClickedCallBack(this, static_cast<SCENE_MEMFUNC2>(&CScene_Shop::PurchaseItem), (DWORD_PTR)i, (DWORD_PTR)nullptr);
		priceBtn->SetIsRound(true, 10.f, 10.f);

		//구매버튼에 이미지//
		CSpriteUI* priceBtnCoinImage = priceBtn->AddChild<CSpriteUI>(Vec2(20.f, 0.f));  // priceBtn 내에서의 상대적인 위치
		priceBtnCoinImage->SetName(L"TEST1234");
		priceBtnCoinImage->AddImage(_pD2DMgr->GetStoredBitmap(L"harvesting_icon"));
		priceBtnCoinImage->SetObjType(GROUP_TYPE::DEFAULT);
		priceBtnCoinImage->SetScale(Vec2(24.f, 24.f));
		//구매버튼에 이미지//
		// priceBtnCoinImage의 위치를 priceBtn 내에서 조정

		//구매버튼에 가격 표시//
		CSpriteUI* priceBtnShowPrice = priceBtn->AddChild<CSpriteUI>(Vec2(-7.f, 0.f));
		priceBtnShowPrice->SetName(L"PriceBtnShowPrice");
		priceBtnShowPrice->SetObjType(GROUP_TYPE::DEFAULT);
		priceBtnShowPrice->SetScale(Vec2(30.f, 30.f));
		swprintf_s(buffer, bufferSize, L"%d", ItemMgr::GetInstance()->GetItem(item_tag_list[item_numbers[i]])->m_iBasePrice);
		priceBtnShowPrice->CreateTextUI(buffer, -(priceBtnShowPrice->GetScale() / 2.f), priceBtnShowPrice->GetScale() / 2.f
			, 16, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
			, FONT_TYPE::KR
			, TextUIMode::TEXT
			, 0);
		//구매버튼에 가격 표시//

		/*
		//아이템 이름//
		CSpriteUI* panelTextItemName = panelItemUI->AddChild<CSpriteUI>(Vec2(30.f, -100.f));
		panelTextItemName->SetName(L"ShowItemNameText");
		panelTextItemName->SetObjType(GROUP_TYPE::UI);
		panelTextItemName->SetScale(Vec2(90.f, 22.f));
		swprintf_s(buffer, bufferSize, L"%s", ItemMgr::GetInstance()->GetItem(item_tag_list[item_numbers[i]])->m_sName.c_str());
		panelTextItemName->CreateTextUI(buffer, -(panelTextItemName->GetScale() / 2.f), panelTextItemName->GetScale() / 2.f
			, 12, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
			, FONT_TYPE::KR
			, TextUIMode::TEXT
			, 0);
		panelTextItemName->GetTextUI()->SetHorizontal(1);
		//아이템 이름//
		*/
		/////////////////////////캐릭터 이름//////////////////////////
		CSpriteUI* characterName = panelItemUI->AddChild<CSpriteUI>(Vec2(35.f, -(panelItemUI->GetScale().y / 2.f) + 11.f + 10.f));
		characterName->SetScale(Vec2(116.f, 20.f));
		swprintf_s(buffer, bufferSize, L"%s", item->m_sName.c_str());
		characterName->CreateTextUI(buffer, -(characterName->GetScale() / 2.f), characterName->GetScale() / 2.f
			, 12, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
			, FONT_TYPE::KR
			, TextUIMode::TEXT
			, 0);
		characterName->GetTextUI()->SetHorizontal(1);
		/////////////////////////캐릭터 이름//////////////////////////

		/////////////////////////캐릭터 타입(캐릭터or무기)//////////////////////////
		//y축 offset설명 11->panel과의 마진 , 10->characterName 절반크기, 5->위에 객체와 간격, 10->이 객체 크기 절반
		CSpriteUI* itemType = panelItemUI->AddChild<CSpriteUI>(Vec2(35.f, -(panelItemUI->GetScale().y / 2.f) + 11.f + 10.f + 5.f + 10.f));
		itemType->SetScale(Vec2(116.f, 20.f));
		swprintf_s(buffer, bufferSize, L"아이템");
		itemType->CreateTextUI(buffer, -(itemType->GetScale() / 2.f), itemType->GetScale() / 2.f
			, 12, ColorNormalize(202, 195, 152), true, 1.f, D2D1::ColorF::Black
			, FONT_TYPE::KR
			, TextUIMode::TEXT
			, 0);
		itemType->GetTextUI()->SetHorizontal(1);
		/////////////////////////캐릭터 이름//////////////////////////
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
		if (item->m_eItemType == ITEM_TYPE::WEAPON) maxStatCount = 8;
		else if (item->m_eItemType == ITEM_TYPE::PASSIVE) maxStatCount = 8;
		for (int i = 0; i < maxStatCount; i++)
		{
			if (item->m_eItemType == ITEM_TYPE::PASSIVE)
			{
				if (i == 0 && item->PassiveStats.m_fDefaultSpeed == 0.f) continue;
				else if (i == 1 && item->PassiveStats.m_iAddMaxHP == 0) continue;
				else if (i == 2 && item->PassiveStats.m_fDamageCoef == 0.f) continue;
				else if (i == 3 && item->PassiveStats.m_fMeleeCoef == 0.f) continue;
				else if (i == 4 && item->PassiveStats.m_fRangeCoef == 0.f) continue;
				else if (i == 5 && item->PassiveStats.m_fAttackSpeedCoef == 0.f) continue;
				else if (i == 6 && item->PassiveStats.m_iCriticalAcc == 0) continue;
				else if (i == 7 && item->PassiveStats.m_fSpeed == 0.f) continue;

				CPanelUI* statpanel = panelItemUI->AddChild<CPanelUI>(Vec2(0.f, -50.f + tmp * 21.f));
				statpanel->SetScale(Vec2(154.f, 16.f));
				statpanel->SetRadius(0.f, 0.f);
				//statpanel->SetColor(ColorNormalize(255, 255, 255), ColorNormalize(255, 255, 255));
				statpanel->SetMouseOnAlpha(0.f);
				statpanel->SetNormalAlpha(0.f);

				CSpriteUI* statNumber = statpanel->AddChild<CSpriteUI>(Vec2(-statpanel->GetScale().x / 2.f + 8.f, 0.f));
				statNumber->SetScale(Vec2(26.f, 16.f));
				if (i == 0) { swprintf_s(buffer, bufferSize, L"+%d", (int)item->PassiveStats.m_fDefaultSpeed); }
				else if (i == 1) { swprintf_s(buffer, bufferSize, L"+%d", item->PassiveStats.m_iAddMaxHP); }
				else if (i == 2) { swprintf_s(buffer, bufferSize, L"+%d%%", (int)item->PassiveStats.m_fDamageCoef); }
				else if (i == 3) { swprintf_s(buffer, bufferSize, L"+%d%%", (int)item->PassiveStats.m_fMeleeCoef); }
				else if (i == 4) { swprintf_s(buffer, bufferSize, L"+%d%%", (int)item->PassiveStats.m_fRangeCoef); }
				else if (i == 5) { swprintf_s(buffer, bufferSize, L"+%d%%", (int)item->PassiveStats.m_fAttackSpeedCoef); }
				else if (i == 6) { swprintf_s(buffer, bufferSize, L"+%d", item->PassiveStats.m_iCriticalAcc); }
				else if (i == 7) { swprintf_s(buffer, bufferSize, L"+%d", (int)item->PassiveStats.m_fSpeed); }
				statNumber->CreateTextUI(buffer, -(statNumber->GetScale() / 2.f), statNumber->GetScale() / 2.f
					, 10, D2D1::ColorF::Green, true, 1.f, D2D1::ColorF::Black
					, FONT_TYPE::KR
					, TextUIMode::TEXT
					, 0);
				statNumber->GetTextUI()->SetHorizontal(1);

				CSpriteUI* statText = statpanel->AddChild<CSpriteUI>(Vec2(0.f, 0.f));
				statText->SetScale(Vec2(120.f, 16.f));
				if (i == 0) { swprintf_s(buffer, bufferSize, L"기본속도"); }
				else if (i == 1) { swprintf_s(buffer, bufferSize, L"최대체력"); }
				else if (i == 2) { swprintf_s(buffer, bufferSize, L"최종데미지"); }
				else if (i == 3) { swprintf_s(buffer, bufferSize, L"근거리 최종데미지"); }
				else if (i == 4) { swprintf_s(buffer, bufferSize, L"원거리 최종데미지"); }
				else if (i == 5) { swprintf_s(buffer, bufferSize, L"공격속도"); }
				else if (i == 6) { swprintf_s(buffer, bufferSize, L"치명타 확률"); }
				else if (i == 7) { swprintf_s(buffer, bufferSize, L"속도계수"); }
				statText->CreateTextUI(buffer, -(statText->GetScale() / 2.f), statText->GetScale() / 2.f
					, 10, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
					, FONT_TYPE::KR
					, TextUIMode::TEXT
					, 0);
				statText->GetTextUI()->SetHorizontal(1);


			}
			else if (item->m_eItemType == ITEM_TYPE::WEAPON)
			{
				if		(i == 0 && item->m_tWeaponInfo.m_iDMG == 0) continue;
				else if (i == 1 && item->m_tWeaponInfo.m_fMeleeCoef == 0.f) continue;
				else if (i == 2 && item->m_tWeaponInfo.m_fRangeCoef == 0) continue;
				else if (i == 3 && item->m_tWeaponInfo.m_fCritialDMG == 0.f) continue;
				else if (i == 4 && item->m_tWeaponInfo.m_fCritialAcc == 0.f) continue;
				else if (i == 5 && item->m_tWeaponInfo.m_fCooldown == 0.f) continue;
				else if (i == 6 && item->m_tWeaponInfo.m_fRecogRange == 0.f) continue;
				else if (i == 7 && item->m_tWeaponInfo.m_iPenet == 0) continue;

				CPanelUI* statpanel = panelItemUI->AddChild<CPanelUI>(Vec2(0.f, -50.f + tmp * 21.f));
				statpanel->SetScale(Vec2(154.f, 16.f));
				statpanel->SetRadius(0.f, 0.f);
				//statpanel->SetColor(ColorNormalize(255, 255, 255), ColorNormalize(255, 255, 255));
				statpanel->SetMouseOnAlpha(0.f);
				statpanel->SetNormalAlpha(0.f);

				CSpriteUI* statNumber = statpanel->AddChild<CSpriteUI>(Vec2(-statpanel->GetScale().x / 2.f + 8.f, 0.f));
				statNumber->SetScale(Vec2(26.f, 16.f));
				if		(i == 0) { swprintf_s(buffer, bufferSize, L"+%d", item->m_tWeaponInfo.m_iDMG); }
				else if (i == 1) { swprintf_s(buffer, bufferSize, L"+%d", (int)item->m_tWeaponInfo.m_fMeleeCoef); }
				else if (i == 2) { swprintf_s(buffer, bufferSize, L"+%d", (int)item->m_tWeaponInfo.m_fRangeCoef); }
				else if (i == 3) { swprintf_s(buffer, bufferSize, L"+%d%%", (int)item->m_tWeaponInfo.m_fCritialDMG); }
				else if (i == 4) { swprintf_s(buffer, bufferSize, L"+%d%%", (int)item->m_tWeaponInfo.m_fCritialAcc); }
				else if (i == 5) { swprintf_s(buffer, bufferSize, L"+%d%%", (int)item->m_tWeaponInfo.m_fCooldown); }
				else if (i == 6) { swprintf_s(buffer, bufferSize, L"+%d%%", (int)item->m_tWeaponInfo.m_fRecogRange); }
				else if (i == 7) { swprintf_s(buffer, bufferSize, L"+%d", item->m_tWeaponInfo.m_iPenet); }
				
				statNumber->CreateTextUI(buffer, -(statNumber->GetScale() / 2.f), statNumber->GetScale() / 2.f
					, 10, D2D1::ColorF::Green, true, 1.f, D2D1::ColorF::Black
					, FONT_TYPE::KR
					, TextUIMode::TEXT
					, 0);
				statNumber->GetTextUI()->SetHorizontal(1);

				CSpriteUI* statText = statpanel->AddChild<CSpriteUI>(Vec2(0.f, 0.f));
				statText->SetScale(Vec2(120.f, 16.f));
				if		(i == 0) { swprintf_s(buffer, bufferSize, L"데미지"); }
				else if (i == 1) { swprintf_s(buffer, bufferSize, L"근거리 데미지"); }
				else if (i == 2) { swprintf_s(buffer, bufferSize, L"원거리 데미지"); }
				else if (i == 3) { swprintf_s(buffer, bufferSize, L"치명타 데미지"); }
				else if (i == 4) { swprintf_s(buffer, bufferSize, L"치명타 확률"); }
				else if (i == 5) { swprintf_s(buffer, bufferSize, L"공격속도"); }
				else if (i == 6) { swprintf_s(buffer, bufferSize, L"범위"); }
				else if (i == 7) { swprintf_s(buffer, bufferSize, L"튕기는 횟수"); }
				
				statText->CreateTextUI(buffer, -(statText->GetScale() / 2.f), statText->GetScale() / 2.f
					, 10, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
					, FONT_TYPE::KR
					, TextUIMode::TEXT
					, 0);
				statText->GetTextUI()->SetHorizontal(1);


			}
			tmp++;
		}
		/////////////////////////캐릭터 정보(스탯)/////////////////////////////////


		/////////////////////구매 버튼/////////////////////////////////

		m_vItemPanels.push_back(panelItemUI);

		AddObject(panelItemUI, GROUP_TYPE::UI);
	}
}

void CScene_Shop::CreatePossessedWeapons(Direct2DMgr* _pD2DMgr)
{
	const list<CWeapon*>& vPlayerWeaponList = static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->GetPlayerWeapons();
	int curWeaponCnt = (int)((CPlayer*)CSceneMgr::GetInstance()->GetPlayer())->GetWeaponCount();

	int temp = 0;
	for (auto iter = vPlayerWeaponList.begin(); iter != vPlayerWeaponList.end(); iter++, temp++)
	{
		CWeapon* weapon = *iter;
		wstring iconTag = weapon->Getinfo().m_sIconImageKey;

		CSpriteUI* weapons = new CSpriteUI;
		weapons->SetName(L"Weapon");
		weapons->SetPos(Vec2(600.f + (temp % 3) * 55.f, 436.f + (temp / 3) * 55.f)); // 기존 무기들 옆에 배치
		weapons->AddImage(_pD2DMgr->GetStoredBitmap(iconTag));
		weapons->SetObjType(GROUP_TYPE::UI);
		weapons->SetScale(Vec2(50.f, 50.f));
		weapons->SetBackGround(true);
		weapons->SetBackGroundColor(ColorNormalize(30, 30, 30), ColorNormalize(0, 0, 0)
			, ColorNormalize(30, 30, 30), ColorNormalize(0, 0, 0));
		weapons->SetIsRound(true, 10.f, 10.f);
		AddObject(weapons, GROUP_TYPE::UI);
	}
}

void CScene_Shop::CreateScrollArea()
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	// 스크롤 영역 설정
	m_scrollArea.viewRect = D2D1::RectF(8.f, 408.f, 508.f, 562.f);
	m_scrollArea.contentRect = D2D1::RectF(0.f, 0.f, 500.f, 800.f);
	m_scrollArea.scrollPos = D2D1::Point2F(0, 0);

	const vector<Item*>& passiveItem = ItemMgr::GetInstance()->GetPassiveItems();
	wstring iconTag;
	// 스크롤 내용 생성 (예: 여러 개의 아이템)
	for (int i = 0; i < passiveItem.size(); ++i)
	{
		CSpriteUI* possessedItem = new CSpriteUI;
		possessedItem->SetObjType(GROUP_TYPE::UI);

		iconTag = passiveItem[i]->tag + L"_icon";
		possessedItem->AddImage(pD2DMgr->GetStoredBitmap(iconTag));

		int xCount = i % 9;
		int yCount = i / 9;

		possessedItem->SetPos(Vec2(35.f + xCount * 54.f, 27.f + yCount * 54.f));
		possessedItem->SetScale(Vec2(50.f, 50.f));

		possessedItem->SetBackGround(true);
		possessedItem->SetBackGroundColor(ColorNormalize(30, 30, 30), ColorNormalize(0, 0, 0)
										, ColorNormalize(30, 30, 30), ColorNormalize(0, 0, 0));
		possessedItem->SetIsRound(true, 10.f, 10.f);
		
		m_scrollContent.push_back(possessedItem);
	}
}

void CScene_Shop::UpdateScrollPosition(float deltaY)
{
	m_scrollArea.scrollPos.y += deltaY;
	m_scrollArea.scrollPos.y = max(0.f, min(m_scrollArea.scrollPos.y,
		m_scrollArea.contentRect.bottom - (m_scrollArea.viewRect.bottom - m_scrollArea.viewRect.top)));
}

void CScene_Shop::RenderScrollArea(ID2D1HwndRenderTarget* _pRender)
{
	if (!_pRender) return;

	//외부영역은 안 그리게
	_pRender->PushAxisAlignedClip(m_scrollArea.viewRect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

	//변환 행렬
	D2D1::Matrix3x2F transform = D2D1::Matrix3x2F::Translation(
		m_scrollArea.viewRect.left - m_scrollArea.scrollPos.x,
		m_scrollArea.viewRect.top - m_scrollArea.scrollPos.y
	);
	_pRender->SetTransform(transform);

	for (auto& item : m_scrollContent)
	{
		//item->update();
		item->finalupdate();
		item->render(_pRender);
	}

	_pRender->SetTransform(D2D1::Matrix3x2F::Identity());
	_pRender->PopAxisAlignedClip();
}

void CScene_Shop::ReRollItem()
{
	if (static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->GetCharacterParam().m_iCoin < 1)
		return;

	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	wchar_t buffer[20];
	size_t bufferSize = sizeof(buffer) / sizeof(wchar_t);

	for (int i = 0; i < 4; i++)
	{
		DeleteObject(m_vItemPanels[i]);
	}
	m_vItemPanels.clear();
	item_numbers.clear();
	CreateShowItemPanels(pD2DMgr, buffer, bufferSize);

	/*
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	item_numbers.clear();
	for (int i = 0; i < 4; i++)
	{
		item_numbers.push_back(rand() % item_tag_list.size());
	}
	wstring iconTag;

	for (int i = 0; i < 4; i++)
	{
		const vector<CUI*>& childs = m_vItemPanels[i]->GetChildsUI();
		//이미지 변경
		childs[0]->DeleteImage();
		iconTag = item_tag_list[item_numbers[i]] + L"_icon";
		childs[0]->AddImage(pD2DMgr->GetStoredBitmap(iconTag));

		//가격 변경
		wchar_t buffer[20];
		swprintf_s(buffer, L"%d", ItemMgr::GetInstance()->GetItem(item_tag_list[item_numbers[i]])->m_iBasePrice);
		//childs[1]->GetTextUI()->SetText(buffer);
		childs[1]->GetChildsUI()[1]->GetTextUI()->SetText(buffer);

		//이름 변경
		swprintf_s(buffer, L"%s", ItemMgr::GetInstance()->GetItem(item_tag_list[item_numbers[i]])->m_sName.c_str());
		childs[2]->GetTextUI()->SetText(buffer);
	}
	*/
	static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->DecreaseCoin(1);
}

void CScene_Shop::PurchaseItem(DWORD_PTR lParam, DWORD_PTR wParam)
{
	int panelNumber = (int)lParam;

	const vector<CUI*>& childs = m_vItemPanels[panelNumber]->GetChildsUI();
	wstring itemTag = item_tag_list[item_numbers[panelNumber]];

	Item* selectedItem = ItemMgr::GetInstance()->GetItem(itemTag);
	
	//가진돈보다 비싸면 아무것도 안하고 return
	int itemPrice = stoi(childs[1]->GetChildsUI()[1]->GetTextUI()->GetText());
	if (itemPrice > static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->GetCharacterParam().m_iCoin)
	{
		return;
	}

	if (selectedItem->m_eItemType == ITEM_TYPE::WEAPON)
	{
		int curWeaponCnt = (int)((CPlayer*)CSceneMgr::GetInstance()->GetPlayer())->GetWeaponCount();

		if (curWeaponCnt == 6) return;

		CWeapon* selectedWeapon = new CWeapon;
		selectedWeapon->SetInfo(selectedItem->m_tWeaponInfo);
		selectedWeapon->SetPlayer();
		static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->AddWeapon(selectedWeapon);

		// 새로 구매한 무기를 화면에 추가
		Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
		wstring iconTag = selectedItem->tag + L"_icon";

		size_t xCount = curWeaponCnt % 3; // 열 번호
		size_t yCount = curWeaponCnt / 3; // 행 번호

		CSpriteUI* newWeapon = new CSpriteUI;
		newWeapon->SetName(L"Weapon");
		newWeapon->SetPos(Vec2(600.f + xCount * 55.f, 436.f + yCount * 55.f)); // 기존 무기들 옆에 배치
		newWeapon->AddImage(pD2DMgr->GetStoredBitmap(iconTag));
		newWeapon->SetObjType(GROUP_TYPE::UI);
		newWeapon->SetScale(Vec2(50.f, 50.f));
		newWeapon->SetBackGround(true);
		newWeapon->SetBackGroundColor(ColorNormalize(30, 30, 30), ColorNormalize(0, 0, 0)
									, ColorNormalize(30, 30, 30), ColorNormalize(0, 0, 0));
		newWeapon->SetIsRound(true, 10.f, 10.f);
		AddObject(newWeapon, GROUP_TYPE::UI);
	}
	else
	{
		ItemMgr::GetInstance()->AddPassive(selectedItem);

		// 스크롤 영역에 새로운 아이템 추가
		Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
		wstring iconTag = itemTag + L"_icon";

		CSpriteUI* newItem = new CSpriteUI;
		newItem->SetObjType(GROUP_TYPE::UI);
		newItem->AddImage(pD2DMgr->GetStoredBitmap(iconTag));

		CPlayer* player = static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer());
		playerParameter& tmp = player->GetCharacterParam();

		/*
		float			m_fDefaultSpeed;	// 기본 스피드.
		int				m_AddMaxHP;			// + 최대 체력
		float			m_fDamageCoef;		//최종뎀 %
		float			m_fMeleeCoef;		//근거리 최종뎀 %
		float			m_fRangeCoef;		//원거리 최종뎀 %
		float			m_fAttackSpeedCoef; // 공격속도 %
		float			m_fCriticalAcc;		// 크확 %
		float			m_fSpeed;			// 속도 계수.
		*/

		printf("플레이어스텟 전\n");
		printf("%d\n", (int)tmp.m_fDefaultSpeed);
		printf("%d\n", tmp.m_AddMaxHP);
		printf("%d\n", (int)tmp.m_fDamageCoef);
		printf("%d\n", (int)tmp.m_fMeleeCoef);
		printf("%d\n", (int)tmp.m_fRangeCoef);
		printf("%d\n", (int)tmp.m_fAttackSpeedCoef);
		printf("%d\n", tmp.m_iCriticalAcc);
		printf("%d\n", (int)tmp.m_fSpeed);

		printf("===========================\n");
		printf("아이템 스텟 \n");
		printf("%d\n", (int)selectedItem->PassiveStats.m_fDefaultSpeed);
		printf("%d\n", selectedItem->PassiveStats.m_iAddMaxHP);
		printf("%d\n", (int)selectedItem->PassiveStats.m_fDamageCoef);
		printf("%d\n", (int)selectedItem->PassiveStats.m_fMeleeCoef);
		printf("%d\n", (int)selectedItem->PassiveStats.m_fRangeCoef);
		printf("%d\n", (int)selectedItem->PassiveStats.m_fAttackSpeedCoef);
		printf("%d\n", selectedItem->PassiveStats.m_iCriticalAcc);
		printf("%d\n", (int)selectedItem->PassiveStats.m_fSpeed);

		tmp.m_fDefaultSpeed		+= selectedItem->PassiveStats.m_fDefaultSpeed;
		tmp.m_AddMaxHP			+= selectedItem->PassiveStats.m_iAddMaxHP;
		tmp.m_fDamageCoef		+= selectedItem->PassiveStats.m_fDamageCoef;
		tmp.m_fMeleeCoef		+= selectedItem->PassiveStats.m_fMeleeCoef;
		tmp.m_fRangeCoef		+= selectedItem->PassiveStats.m_fRangeCoef;
		tmp.m_fAttackSpeedCoef	+= selectedItem->PassiveStats.m_fAttackSpeedCoef;
		tmp.m_iCriticalAcc		+= selectedItem->PassiveStats.m_iCriticalAcc;
		tmp.m_fSpeed			+= selectedItem->PassiveStats.m_fSpeed;

		printf("플레이어스텟 후\n");
		printf("%d\n", (int)static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->GetCharacterParam().m_fDefaultSpeed);
		printf("%d\n", static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->GetCharacterParam().m_AddMaxHP);
		printf("%d\n", (int)static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->GetCharacterParam().m_fDamageCoef);
		printf("%d\n", (int)static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->GetCharacterParam().m_fMeleeCoef);
		printf("%d\n", (int)static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->GetCharacterParam().m_fRangeCoef);
		printf("%d\n", (int)static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->GetCharacterParam().m_fAttackSpeedCoef);
		printf("%d\n", static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->GetCharacterParam().m_iCriticalAcc);
		printf("%d\n", (int)static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->GetCharacterParam().m_fSpeed);

		// 새로운 아이템의 위치를 계산하여 설정
		size_t xCount = m_scrollContent.size() % 9; // 열 번호
		size_t yCount = m_scrollContent.size() / 9; // 행 번호
		// 새로운 아이템의 위치를 계산하여 설정
		newItem->SetPos(Vec2(xCount * 54.f + 35.f, yCount * 54.f + 27.f));
		newItem->SetScale(Vec2(50.f, 50.f));
		newItem->SetBackGround(true);
		newItem->SetBackGroundColor(ColorNormalize(30, 30, 30), ColorNormalize(0, 0, 0)
								, ColorNormalize(30, 30, 30), ColorNormalize(0, 0, 0));
		newItem->SetIsRound(true, 10.f, 10.f);

		m_scrollContent.push_back(newItem);

		// 스크롤 가능한 콘텐츠 영역 크기 업데이트
		float contentHeight = (yCount + 1) * 54.f + 27.f; // 행 개수에 따라 높이를 계산
		m_scrollArea.contentRect.bottom = max(m_scrollArea.contentRect.bottom, contentHeight);
	}

	if (m_statPanel != nullptr)
	{
		DeleteObject(m_statPanel);
		CreateInfoPanel(Vec2(852.f, 204.f));
	}



	static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->DecreaseCoin(itemPrice);
}

void CScene_Shop::CreateInfoPanel(Vec2 _panelPos)
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	CWaveMgr* waveMgr = CWaveMgr::GetInstance();
	wchar_t buffer[20];
	//플레이어 Info 출력. 
	const playerParameter playerInfo = static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->GetPlayerInfo();

	//////////////////우측 능력치 보여주는 곳///////////////////////
	CPanelUI* parameterPanel = new CPanelUI;
	parameterPanel->SetName(L"parameterPanel");
	parameterPanel->SetObjType(GROUP_TYPE::IMAGE);
	parameterPanel->SetPos(_panelPos);
	parameterPanel->SetNormalAlpha(0.5f);
	parameterPanel->SetMouseOnAlpha(0.5f);
	parameterPanel->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));
	parameterPanel->SetScale(Vec2(200.f, 300.f));

	m_statPanel = parameterPanel;
	AddObject(parameterPanel, GROUP_TYPE::IMAGE);
	//////////////////우측 능력치 보여주는 곳///////////////////////

	////////////우측 능력치 보여주는 곳////////////////////
	CSpriteUI* abilityText = parameterPanel->AddChild<CSpriteUI>(Vec2(0.f, -130.f));
	abilityText->SetName(L"abilityText");
	abilityText->SetObjType(GROUP_TYPE::UI);
	abilityText->SetScale(Vec2(196.f, 26.f));
	abilityText->CreateTextUI(L"능력치", -(abilityText->GetScale() / 2.f), (abilityText->GetScale() / 2.f)
		, 22, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	////////////우측 능력치 보여주는 곳////////////////////


	////////////현재 레벨 ////////////////////
	CPanelUI* statLevelPanel = parameterPanel->AddChild<CPanelUI>(Vec2(0.f, -80.f));
	statLevelPanel->SetScale(Vec2(180.f, 15.f));
	statLevelPanel->SetColor(D2D1::ColorF(0, 0, 0, 0), D2D1::ColorF(0, 0, 0, 0));
	statLevelPanel->SetNormalAlpha(0.f);
	statLevelPanel->SetMouseOnAlpha(0.f);

	//레벨 아이콘
	CSpriteUI* nowLevelIcon = statLevelPanel->AddChild<CSpriteUI>(Vec2(-statLevelPanel->GetScale().x / 2.f + 10.f, 0.f));
	nowLevelIcon->SetName(L"nowLevelIcon");
	nowLevelIcon->SetObjType(GROUP_TYPE::UI);
	nowLevelIcon->AddImage(pD2DMgr->GetStoredBitmap(L"brotato_icon"));
	nowLevelIcon->SetScale(Vec2(15.f, 15.f));

	//현재레벨 text
	CSpriteUI* nowLevelText = statLevelPanel->AddChild<CSpriteUI>(Vec2(0.f, 0.f));
	nowLevelText->SetName(L"nowLevelText");
	nowLevelText->SetObjType(GROUP_TYPE::UI);
	nowLevelText->SetScale(Vec2(110.f, 35.f));
	swprintf_s(buffer, L"현재 레벨");
	nowLevelText->CreateTextUI(buffer, -(nowLevelText->GetScale() / 2.f), (nowLevelText->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	nowLevelText->GetTextUI()->SetHorizontal(1);

	//현재레벨 숫자
	CSpriteUI* nowLevelCount = statLevelPanel->AddChild<CSpriteUI>(Vec2(statLevelPanel->GetScale().x / 2.f - 10.f, 0.f));
	nowLevelCount->SetName(L"nowLevelCount");
	nowLevelCount->SetObjType(GROUP_TYPE::UI);
	nowLevelCount->SetScale(Vec2(35.f, 35.f));
	swprintf_s(buffer, L"%d", playerInfo.m_iLevel);
	nowLevelCount->CreateTextUI(buffer, -(nowLevelCount->GetScale() / 2.f), (nowLevelCount->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	////////////현재 레벨 ////////////////////


	////////////최대 HP ////////////////////
	CPanelUI* statMaxHpPanel = parameterPanel->AddChild<CPanelUI>(Vec2(0.f, -55.f));
	statMaxHpPanel->SetScale(Vec2(180.f, 15.f));
	statMaxHpPanel->SetColor(D2D1::ColorF(0, 0, 0, 0), D2D1::ColorF(0, 0, 0, 0));
	statMaxHpPanel->SetNormalAlpha(0.f);
	statMaxHpPanel->SetMouseOnAlpha(0.f);

	//현재 최대hp 아이콘
	CSpriteUI* maxHPIcon = statMaxHpPanel->AddChild<CSpriteUI>(Vec2(-statMaxHpPanel->GetScale().x / 2.f + 10.f, 0.f));
	maxHPIcon->SetName(L"maxHPIcon");
	maxHPIcon->SetObjType(GROUP_TYPE::UI);
	maxHPIcon->AddImage(pD2DMgr->GetStoredBitmap(L"max_hp"));
	maxHPIcon->SetScale(Vec2(15.f, 15.f));

	//현재 최대hp text
	CSpriteUI* maxHPText = statMaxHpPanel->AddChild<CSpriteUI>(Vec2(0.f, 0.f));
	maxHPText->SetName(L"maxHPText");
	maxHPText->SetObjType(GROUP_TYPE::UI);
	maxHPText->SetScale(Vec2(110.f, 35.f));
	swprintf_s(buffer, L"최대 HP");
	maxHPText->CreateTextUI(buffer, -(maxHPText->GetScale() / 2.f), (maxHPText->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	maxHPText->GetTextUI()->SetHorizontal(1);

	//현재 최대hp 숫자
	CSpriteUI* maxHPCount = statMaxHpPanel->AddChild<CSpriteUI>(Vec2(statMaxHpPanel->GetScale().x / 2.f - 10.f, 0.f));
	maxHPCount->SetName(L"maxHPCount");
	maxHPCount->SetObjType(GROUP_TYPE::UI);
	maxHPCount->SetScale(Vec2(35.f, 35.f));
	swprintf_s(buffer, L"%d", playerInfo.m_iMaxHP + playerInfo.m_AddMaxHP);
	maxHPCount->CreateTextUI(buffer, -(maxHPCount->GetScale() / 2.f), (maxHPCount->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	////////////최대 HP ////////////////////



	////////////데미지 % ////////////////////
	CPanelUI* statDamagePanel = parameterPanel->AddChild<CPanelUI>(Vec2(0.f, -30.f));
	statDamagePanel->SetScale(Vec2(180.f, 15.f));
	statDamagePanel->SetColor(D2D1::ColorF(0, 0, 0, 0), D2D1::ColorF(0, 0, 0, 0));
	statDamagePanel->SetNormalAlpha(0.f);
	statDamagePanel->SetMouseOnAlpha(0.f);

	//
	CSpriteUI* FinalDMGIcon = statDamagePanel->AddChild<CSpriteUI>(Vec2(-statDamagePanel->GetScale().x / 2.f + 10.f, 0.f));
	FinalDMGIcon->SetName(L"FinalDMGIcon");
	FinalDMGIcon->SetObjType(GROUP_TYPE::UI);
	FinalDMGIcon->AddImage(pD2DMgr->GetStoredBitmap(L"percent_damage"));
	FinalDMGIcon->SetScale(Vec2(15.f, 15.f));

	// 
	CSpriteUI* FinalDMGText = statDamagePanel->AddChild<CSpriteUI>(Vec2(0.f, 0.f));
	FinalDMGText->SetName(L"FinalDMGText");
	FinalDMGText->SetObjType(GROUP_TYPE::UI);
	FinalDMGText->SetScale(Vec2(110.f, 35.f));
	swprintf_s(buffer, L"%% 데미지");
	FinalDMGText->CreateTextUI(buffer, -(FinalDMGText->GetScale() / 2.f), (FinalDMGText->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	FinalDMGText->GetTextUI()->SetHorizontal(1);

	//
	CSpriteUI* FinalDMGCount = statDamagePanel->AddChild<CSpriteUI>(Vec2(statDamagePanel->GetScale().x / 2.f - 10.f, 0.f));
	FinalDMGCount->SetName(L"FinalDMGCount");
	FinalDMGCount->SetObjType(GROUP_TYPE::UI);
	FinalDMGCount->SetScale(Vec2(35.f, 35.f));
	swprintf_s(buffer, L"%d", static_cast<int>(playerInfo.m_fDamageCoef));
	FinalDMGCount->CreateTextUI(buffer, -(FinalDMGCount->GetScale() / 2.f), (FinalDMGCount->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	////////////데미지 % ////////////////////


	////////////근거리 데미지 % ////////////////////
	CPanelUI* statMeleeDMGPanel = parameterPanel->AddChild<CPanelUI>(Vec2(0.f, -5.f));
	statMeleeDMGPanel->SetScale(Vec2(180.f, 15.f));
	statMeleeDMGPanel->SetColor(D2D1::ColorF(0, 0, 0, 0), D2D1::ColorF(0, 0, 0, 0));
	statMeleeDMGPanel->SetNormalAlpha(0.f);
	statMeleeDMGPanel->SetMouseOnAlpha(0.f);

	//
	CSpriteUI* MeleeDMGIcon = statMeleeDMGPanel->AddChild<CSpriteUI>(Vec2(-statMeleeDMGPanel->GetScale().x / 2.f + 10.f, 0.f));
	MeleeDMGIcon->SetName(L"MeleeDMGIcon");
	MeleeDMGIcon->SetObjType(GROUP_TYPE::UI);
	MeleeDMGIcon->AddImage(pD2DMgr->GetStoredBitmap(L"melee_damage"));
	MeleeDMGIcon->SetScale(Vec2(15.f, 15.f));

	// 
	CSpriteUI* MeleeText = statMeleeDMGPanel->AddChild<CSpriteUI>(Vec2(0.f, 0.f));
	MeleeText->SetName(L"MeleeText");
	MeleeText->SetObjType(GROUP_TYPE::UI);
	MeleeText->SetScale(Vec2(110.f, 35.f));
	swprintf_s(buffer, L"근거리 데미지");
	MeleeText->CreateTextUI(buffer, -(MeleeText->GetScale() / 2.f), (MeleeText->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	MeleeText->GetTextUI()->SetHorizontal(1);

	//
	CSpriteUI* MeleeCount = statMeleeDMGPanel->AddChild<CSpriteUI>(Vec2(statMeleeDMGPanel->GetScale().x / 2.f - 10.f, 0.f));
	MeleeCount->SetName(L"MeleeCount");
	MeleeCount->SetObjType(GROUP_TYPE::UI);
	MeleeCount->SetScale(Vec2(35.f, 35.f));
	swprintf_s(buffer, L"%d", static_cast<int>(playerInfo.m_fMeleeCoef));
	MeleeCount->CreateTextUI(buffer, -(MeleeCount->GetScale() / 2.f), (MeleeCount->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	////////////근거리 데미지 % ////////////////////

	////////////원거리 데미지 % ////////////////////
	CPanelUI* statRangedDMGPanel = parameterPanel->AddChild<CPanelUI>(Vec2(0.f, 20.f));
	statRangedDMGPanel->SetScale(Vec2(180.f, 15.f));
	statRangedDMGPanel->SetColor(D2D1::ColorF(0, 0, 0, 0), D2D1::ColorF(0, 0, 0, 0));
	statRangedDMGPanel->SetNormalAlpha(0.f);
	statRangedDMGPanel->SetMouseOnAlpha(0.f);

	//
	CSpriteUI* rangedDMGIcon = statRangedDMGPanel->AddChild<CSpriteUI>(Vec2(-statRangedDMGPanel->GetScale().x / 2.f + 10.f, 0.f));
	rangedDMGIcon->SetName(L"rangedDMGIcon");
	rangedDMGIcon->SetObjType(GROUP_TYPE::UI);
	rangedDMGIcon->AddImage(pD2DMgr->GetStoredBitmap(L"ranged_damage"));
	rangedDMGIcon->SetScale(Vec2(15.f, 15.f));

	// 
	CSpriteUI* rangedText = statRangedDMGPanel->AddChild<CSpriteUI>(Vec2(0.f, 0.f));
	rangedText->SetObjType(GROUP_TYPE::UI);
	rangedText->SetScale(Vec2(110.f, 35.f));
	swprintf_s(buffer, L"원거리 데미지");
	rangedText->CreateTextUI(buffer, -(rangedText->GetScale() / 2.f), (rangedText->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	rangedText->GetTextUI()->SetHorizontal(1);

	//
	CSpriteUI* rangedCount = statRangedDMGPanel->AddChild<CSpriteUI>(Vec2(statRangedDMGPanel->GetScale().x / 2.f - 10.f, 0.f));
	rangedCount->SetName(L"rangedCount");
	rangedCount->SetObjType(GROUP_TYPE::UI);
	rangedCount->SetScale(Vec2(35.f, 35.f));
	swprintf_s(buffer, L"%d", static_cast<int>(playerInfo.m_fRangeCoef));
	rangedCount->CreateTextUI(buffer, -(rangedCount->GetScale() / 2.f), (rangedCount->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	////////////원거리 데미지 % ////////////////////


	////////////공격속도  % ////////////////////
	CPanelUI* statAttackSpeedPanel = parameterPanel->AddChild<CPanelUI>(Vec2(0.f, 45.f));
	statAttackSpeedPanel->SetScale(Vec2(180.f, 15.f));
	statAttackSpeedPanel->SetColor(D2D1::ColorF(0, 0, 0, 0), D2D1::ColorF(0, 0, 0, 0));
	statAttackSpeedPanel->SetNormalAlpha(0.f);
	statAttackSpeedPanel->SetMouseOnAlpha(0.f);

	//
	CSpriteUI* attackSpeedIcon = statAttackSpeedPanel->AddChild<CSpriteUI>(Vec2(-statAttackSpeedPanel->GetScale().x / 2.f + 10.f, 0.f));
	attackSpeedIcon->SetName(L"attackSpeedIcon");
	attackSpeedIcon->SetObjType(GROUP_TYPE::UI);
	attackSpeedIcon->AddImage(pD2DMgr->GetStoredBitmap(L"attack_speed"));
	attackSpeedIcon->SetScale(Vec2(15.f, 15.f));

	// 
	CSpriteUI* attackSpeedText = statAttackSpeedPanel->AddChild<CSpriteUI>(Vec2(0.f, 0.f));
	attackSpeedText->SetName(L"attackSpeedText");
	attackSpeedText->SetObjType(GROUP_TYPE::UI);
	attackSpeedText->SetScale(Vec2(110.f, 35.f));
	swprintf_s(buffer, L"%% 공격 속도");
	attackSpeedText->CreateTextUI(buffer, -(attackSpeedText->GetScale() / 2.f), (attackSpeedText->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	attackSpeedText->GetTextUI()->SetHorizontal(1);

	//
	CSpriteUI* attackSpeedCount = statAttackSpeedPanel->AddChild<CSpriteUI>(Vec2(statAttackSpeedPanel->GetScale().x / 2.f - 10.f, 0.f));
	attackSpeedCount->SetName(L"attackSpeedCount");
	attackSpeedCount->SetObjType(GROUP_TYPE::UI);
	attackSpeedCount->SetScale(Vec2(35.f, 35.f));
	swprintf_s(buffer, L"%d", static_cast<int>(playerInfo.m_fAttackSpeedCoef));
	attackSpeedCount->CreateTextUI(buffer, -(attackSpeedCount->GetScale() / 2.f), (attackSpeedCount->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	////////////공격속도 % ////////////////////


	////////////치명타율  % ////////////////////
	CPanelUI* statCritChancePanel = parameterPanel->AddChild<CPanelUI>(Vec2(0.f, 70.f));
	statCritChancePanel->SetScale(Vec2(180.f, 15.f));
	statCritChancePanel->SetColor(D2D1::ColorF(0, 0, 0, 0), D2D1::ColorF(0, 0, 0, 0));
	statCritChancePanel->SetNormalAlpha(0.f);
	statCritChancePanel->SetMouseOnAlpha(0.f);

	//C
	CSpriteUI* critChanceIcon = statCritChancePanel->AddChild<CSpriteUI>(Vec2(-statCritChancePanel->GetScale().x / 2.f + 10.f, 0.f));
	critChanceIcon->SetName(L"critChanceIcon");
	critChanceIcon->SetObjType(GROUP_TYPE::UI);
	critChanceIcon->AddImage(pD2DMgr->GetStoredBitmap(L"crit_chance"));
	critChanceIcon->SetScale(Vec2(15.f, 15.f));

	// 
	CSpriteUI* critChanceText = statCritChancePanel->AddChild<CSpriteUI>(Vec2(0.f, 0.f));
	critChanceText->SetName(L"critChanceText");
	critChanceText->SetObjType(GROUP_TYPE::UI);
	critChanceText->SetScale(Vec2(110.f, 35.f));
	swprintf_s(buffer, L"%% 치명타율");
	critChanceText->CreateTextUI(buffer, -(critChanceText->GetScale() / 2.f), (critChanceText->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	critChanceText->GetTextUI()->SetHorizontal(1);

	//
	CSpriteUI* critChanceCount = statCritChancePanel->AddChild<CSpriteUI>(Vec2(statCritChancePanel->GetScale().x / 2.f - 10.f, 0.f));
	critChanceCount->SetName(L"critChanceCount");
	critChanceCount->SetObjType(GROUP_TYPE::UI);
	//740 + Alpha
	critChanceCount->SetPos(Vec2(920.f, 350.f));
	critChanceCount->SetScale(Vec2(35.f, 35.f));
	swprintf_s(buffer, L"%d", static_cast<int>(playerInfo.m_iCriticalAcc));
	critChanceCount->CreateTextUI(buffer, -(critChanceCount->GetScale() / 2.f), (critChanceCount->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	////////////치명타율 % ////////////////////


	////////////속도  % ////////////////////
	CPanelUI* statSpeedPanel = parameterPanel->AddChild<CPanelUI>(Vec2(0.f, 95.f));
	statSpeedPanel->SetScale(Vec2(180.f, 15.f));
	statSpeedPanel->SetColor(D2D1::ColorF(0, 0, 0, 0), D2D1::ColorF(0, 0, 0, 0));
	statSpeedPanel->SetNormalAlpha(0.f);
	statSpeedPanel->SetMouseOnAlpha(0.f);

	//
	CSpriteUI* speedIcon = statSpeedPanel->AddChild<CSpriteUI>(Vec2(-statSpeedPanel->GetScale().x / 2.f + 10.f, 0.f));
	speedIcon->SetName(L"speedIcon");
	speedIcon->SetObjType(GROUP_TYPE::UI);
	speedIcon->AddImage(pD2DMgr->GetStoredBitmap(L"speed"));
	speedIcon->SetScale(Vec2(15.f, 15.f));

	// 
	CSpriteUI* speedText = statSpeedPanel->AddChild<CSpriteUI>(Vec2(0.f, 0.f));
	speedText->SetName(L"speedText");
	speedText->SetObjType(GROUP_TYPE::UI);
	speedText->SetScale(Vec2(110.f, 35.f));
	swprintf_s(buffer, L"%% 속도");
	speedText->CreateTextUI(buffer, -(speedText->GetScale() / 2.f), (speedText->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	speedText->GetTextUI()->SetHorizontal(1);

	//	
	CSpriteUI* speedCount = statSpeedPanel->AddChild<CSpriteUI>(Vec2(statSpeedPanel->GetScale().x / 2.f - 10.f, 0.f));
	speedCount->SetName(L"speedCount");
	speedCount->SetObjType(GROUP_TYPE::UI);
	speedCount->SetScale(Vec2(35.f, 35.f));
	swprintf_s(buffer, L"%d", static_cast<int>(playerInfo.m_fSpeed) + (int)playerInfo.m_fDefaultSpeed);
	speedCount->CreateTextUI(buffer, -(speedCount->GetScale() / 2.f), (speedCount->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	////////////치명타율 % ////////////////////


}