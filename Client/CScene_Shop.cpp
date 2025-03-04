#include "pch.h"

#include "CSpriteUI.h"
#include "CPlayer.h"
#include "CObject.h"
#include "CBtnUI.h"
#include "CImage.h"
#include "CObject.h"
#include "CPanelUI.h"
#include "CTextUI.h"

#include "Direct2DMgr.h"
#include "CWaveMgr.h"
#include "CCore.h"
#include "CSceneMgr.h"

#include "CScene_Shop.h"

CScene_Shop::CScene_Shop()
{

}

CScene_Shop::~CScene_Shop()
{

}


void CScene_Shop::update()
{
	//CScene::update();

	wchar_t buffer[20];
	for (UINT typeIDX = 0; typeIDX < (UINT)GROUP_TYPE::END; typeIDX++) 
	{
		const vector<CObject*>& vecObj = GetGroupObject((GROUP_TYPE)typeIDX);
		
		///////////////////////////////////////////
		for (size_t objIDX = 0; objIDX < vecObj.size(); objIDX++) 
		{
			if (!vecObj[objIDX]->IsDead()) 
			{
				if (vecObj[objIDX]->GetName().compare(L"RestCoin") == 0)
				{
					swprintf_s(buffer, L"%d", ((CPlayer*)CSceneMgr::GetInstance()->GetPlayer())->GetCharacterParam().m_iCoin);
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

	/////////////////뒷 배경 오브젝트/////////////////
	CObject* backGround = new CSpriteUI;
	backGround->SetObjType(GROUP_TYPE::DEFAULT);
	backGround->AddImage(pD2DMgr->GetStoredBitmap(L"shop_background"));
	backGround->SetPos(vResolution / 2.f);
	backGround->SetScale(vResolution);
	AddObject(backGround, GROUP_TYPE::DEFAULT);
	/////////////////뒷 배경 오브젝트/////////////////

	/////////////////상점 웨이브 글자 UI(좌상단)///////////////////
	CObject* panelTextShopAndWave = new CSpriteUI;
	panelTextShopAndWave->SetObjType(GROUP_TYPE::UI);
	panelTextShopAndWave->SetPos(Vec2(78.f, 32.f));
	panelTextShopAndWave->SetScale(Vec2(130.f, 32.f));

	swprintf_s(buffer, L"상점(웨이브 %d)", CWaveMgr::GetInstance()->GetLevel() - 1);
	panelTextShopAndWave->CreateTextUI(buffer, Vec2(-150.f, -18.f), Vec2(150.f, 18.f)
		, 20, D2D1::ColorF::White, true, 2.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(panelTextShopAndWave, GROUP_TYPE::UI);
	/////////////////상점 웨이브 글자 UI(좌상단)///////////////////

	/////////////////아이템 글자 UI(우하단)///////////////////
	CObject* panelTextItem= new CSpriteUI;
	panelTextItem->SetObjType(GROUP_TYPE::UI);
	panelTextItem->SetPos(Vec2(40.f, 386.f));
	panelTextItem->SetScale(Vec2(64.f, 3.f));

	panelTextItem->CreateTextUI(L"아이템", Vec2(-150.f, -18.f), Vec2(150.f, 18.f)
		, 20, D2D1::ColorF::White, true, 2.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(panelTextItem, GROUP_TYPE::UI);
	/////////////////아이템 글자 UI(우하단)///////////////////

	///////////////////////////무기 글자 UI///////////////////
	CObject* panelTextWeapon = new CSpriteUI;
	panelTextWeapon->SetObjType(GROUP_TYPE::UI);
	panelTextWeapon->SetPos(Vec2(608.f, 386.f));
	panelTextWeapon->SetScale(Vec2(64.f, 3.f));

	swprintf_s(buffer, L"무기(%d/6)", (int)((CPlayer*)CSceneMgr::GetInstance()->GetPlayer())->GetWeaponCount());
	panelTextWeapon->CreateTextUI(buffer, Vec2(-150.f, -18.f), Vec2(150.f, 18.f)
		, 20, D2D1::ColorF::White, true, 2.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(panelTextWeapon, GROUP_TYPE::UI);
	///////////////////////////무기 글자 UI///////////////////

	/////////////////초기화 버튼/////////////////
	CBtnUI* resetBtn = new CBtnUI;
	resetBtn->SetName(L"ResetBtn");
	resetBtn->SetObjType(GROUP_TYPE::UI);
	resetBtn->SetScale(Vec2(134.f, 42.f));
	resetBtn->SetPos(Vec2(662.f, 32.f));
	resetBtn->SetIsRound(true, 10.f, 10.f);
	resetBtn->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(0, 0, 0));

	//returnToGameBtn->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Start::OffPause);
	resetBtn->SetClickedCallBack(this, static_cast<SCENE_MEMFUNC>(&CScene_Shop::ReRollItem));
	resetBtn->CreateTextUI(L"초기화 -1", -(resetBtn->GetScale() / 2.f), resetBtn->GetScale() / 2.f
		, 16, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);

	CObject* coinImage = new CSpriteUI;
	coinImage->AddImage(pD2DMgr->GetStoredBitmap(L"harvesting_icon"));
	coinImage->GetImage(0)->SetOffset(Vec2(46.f,1.f));
	coinImage->SetObjType(GROUP_TYPE::UI);
	coinImage->SetName(L"Child");
	coinImage->SetScale(Vec2(24.f, 24.f));
	coinImage->SetPos(resetBtn->GetPos());

	resetBtn->AddChild((CUI*)coinImage);

	AddObject(resetBtn, GROUP_TYPE::UI);
	/////////////////초기화 버튼/////////////////

	///////////////////////////남은 재화 표시///////////////////
	CObject* panelTextCurCoin = new CSpriteUI;
	panelTextCurCoin->SetName(L"RestCoin");
	panelTextCurCoin->SetObjType(GROUP_TYPE::UI);
	panelTextCurCoin->SetPos(Vec2(386.f, 32.f));
	panelTextCurCoin->SetScale(Vec2(32.f, 22.f));
	swprintf_s(buffer, L"%d", ((CPlayer*)CSceneMgr::GetInstance()->GetPlayer())->GetCharacterParam().m_iCoin);
	panelTextCurCoin->CreateTextUI(buffer, -(panelTextCurCoin->GetScale() / 2.f), panelTextCurCoin->GetScale() / 2.f
		, 16, D2D1::ColorF::White, true, 2.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(panelTextCurCoin, GROUP_TYPE::UI);
	///////////////////////////남은 재화 표시///////////////////

	///////////////////////////남은 재화 표시 이미지///////////////////
	CObject* panelTextCurCoinImage = new CSpriteUI;
	panelTextCurCoinImage->SetObjType(GROUP_TYPE::UI);
	panelTextCurCoinImage->AddImage(pD2DMgr->GetStoredBitmap(L"harvesting_icon"));
	panelTextCurCoinImage->SetPos(Vec2(356.f, 32.f));
	panelTextCurCoinImage->SetScale(Vec2(24.f, 24.f));
	AddObject(panelTextCurCoinImage, GROUP_TYPE::UI);
	///////////////////////////남은 재화 표시 이미지///////////////////


	/////////////////다음 웨이브 버튼/////////////////
	CBtnUI* nextWaveBtn = new CBtnUI;
	nextWaveBtn->SetName(L"NextWaveBtn");
	nextWaveBtn->SetObjType(GROUP_TYPE::UI);
	nextWaveBtn->SetScale(Vec2(206.f, 34.f));
	nextWaveBtn->SetPos(Vec2(844.f, 508.f));
	nextWaveBtn->SetIsRound(true, 10.f, 10.f);
	nextWaveBtn->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(0, 0, 0));
	nextWaveBtn->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::START, 0);
	swprintf_s(buffer, L"이동(웨이브%d)", CWaveMgr::GetInstance()->GetLevel());
	nextWaveBtn->CreateTextUI(buffer, -(nextWaveBtn->GetScale() / 2.f), nextWaveBtn->GetScale() / 2.f
		, 16, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(nextWaveBtn, GROUP_TYPE::UI);
	/////////////////다음 웨이브 버튼/////////////////

	////////////////////////가운데 아이템 패널 4개////////////////////////////////
	vector<int> item_numbers;
	for (int i = 0; i < 4; i++)
	{
		item_numbers.push_back(rand() % item_tag_list.size());
	}

	for (int i = 0; i < 4; i++)
	{
		wstring iconTag = item_tag_list[item_numbers[i]] + L"_icon";

		CPanelUI* panelItemUI = new CPanelUI;
		panelItemUI->SetName(iconTag);
		panelItemUI->SetScale(Vec2(176.f, 242.f));
		panelItemUI->SetPos(Vec2(100.f + i * (5.f + panelItemUI->GetScale().x), 204.f));
		panelItemUI->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));

		////////////////////아이템 이미지/////////////////////////////
		CObject* itemImage = new CSpriteUI;
		itemImage->AddImage(pD2DMgr->GetStoredBitmap(iconTag));
		Vec2 vPos = Vec2(35.f, 35.f) - (panelItemUI->GetScale() / 2.f);
		itemImage->GetImage(0)->SetOffset(vPos);
		itemImage->SetObjType(GROUP_TYPE::UI);
		itemImage->SetName(L"Child");
		itemImage->SetScale(Vec2(48.f, 48.f));
		itemImage->SetPos(panelItemUI->GetPos());
		////////////////////아이템 이미지/////////////////////////////

		/////////////////////구매 버튼/////////////////////////////////
		CBtnUI* priceBtn = new CBtnUI;
		priceBtn->SetName(L"PriceBtn");
		priceBtn->SetPos(Vec2(0.f, panelItemUI->GetScale().y / 2.f - 8.f - 17.f));
		priceBtn->SetScale(Vec2(80.f, 34.f));
		priceBtn->SetObjType(GROUP_TYPE::UI);
		priceBtn->SetColor(ColorNormalize(255, 255, 255), ColorNormalize(30, 30, 30));
		priceBtn->SetIsRound(true, 10.f, 10.f);

		//구매버튼에 이미지//
		CObject* priceBtnCoinImage = new CSpriteUI;
		priceBtnCoinImage->SetName(L"TEST1234");
		priceBtnCoinImage->SetPos(panelItemUI->GetPos());
		priceBtnCoinImage->AddImage(pD2DMgr->GetStoredBitmap(L"harvesting_icon"));
		priceBtnCoinImage->GetImage(0)->SetOffset(Vec2(20.f, panelItemUI->GetScale().y / 2.f - 8.f - 17.f));
		priceBtnCoinImage->SetObjType(GROUP_TYPE::UI);
		priceBtnCoinImage->SetScale(Vec2(24.f, 24.f));
		//구매버튼에 이미지//
	
		/////////////////////구매 버튼/////////////////////////////////
		panelItemUI->AddChild((CUI*)priceBtn);
		panelItemUI->AddChild((CUI*)priceBtnCoinImage);
		panelItemUI->AddChild((CUI*)itemImage);
	

		m_vItemPanels.push_back(panelItemUI);
		AddObject(panelItemUI, GROUP_TYPE::UI);
	}

	////////////////////////가운데 아이템 패널 4개////////////////////////////////


	CreateInfoPanel();

	CreateScrollArea();
}

void CScene_Shop::Exit()
{
	//Safe_Delete_Vec(m_scrollContent);
	//Safe_Delete_Vec(m_vItemPanels);
	DeleteAll();
}

void CScene_Shop::CreateScrollArea()
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	// 스크롤 영역 설정
	m_scrollArea.viewRect = D2D1::RectF(8.f, 408.f, 508.f, 562.f);
	m_scrollArea.contentRect = D2D1::RectF(0.f, 0.f, 500.f, 800.f);
	m_scrollArea.scrollPos = D2D1::Point2F(0, 0);

	// 스크롤 내용 생성 (예: 여러 개의 아이템)
	for (int i = 0; i < 3; ++i)
	{
		CObject* item = new CSpriteUI;
		item->SetObjType(GROUP_TYPE::UI);
		item->AddImage(pD2DMgr->GetStoredBitmap(L"well_rounded_icon")); // 적절한 아이템 아이콘으로 변경
		item->SetPos(Vec2(24.f + i * (50.f), 27.f)); //contentRect기준 offset개념으로 접근
		item->SetScale(Vec2(60.f, 60.f));

		m_scrollContent.push_back(item);
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
	vector<int> item_numbers;
	for (int i = 0; i < 4; i++)
	{
		item_numbers.push_back(rand() % item_tag_list.size());
	}
	wstring iconTag;

	for (int i = 0; i < 4; i++)
	{
		Vec2 vPos = Vec2(35.f, 35.f) - (m_vItemPanels[i]->GetScale() / 2.f);
		const vector<CUI*>& childs = m_vItemPanels[i]->GetChildsUI();
		childs[0]->DeleteImage();
		iconTag = item_tag_list[item_numbers[i]] + L"_icon";
		childs[0]->AddImage(pD2DMgr->GetStoredBitmap(iconTag));
		childs[0]->GetImage(0)->SetOffset(vPos);
	}

	static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->DecreaseCoin(1);
	int a = static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->GetCharacterParam().m_iCoin;
	int b = 0;
}



void CScene_Shop::CreateInfoPanel()
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
	parameterPanel->SetPos(Vec2(840.f, vResolution.y / 2));
	parameterPanel->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));
	parameterPanel->SetNormalAlpha(0.5f);
	parameterPanel->SetMouseOnAlpha(0.5f);
	parameterPanel->SetScale(Vec2(200.f, 300.f));
	
	AddObject(parameterPanel, GROUP_TYPE::IMAGE);
	//////////////////우측 능력치 보여주는 곳///////////////////////


	////////////우측 능력치 보여주는 곳////////////////////
	CObject* abilityText = new CSpriteUI;
	abilityText->SetName(L"abilityText");
	abilityText->SetObjType(GROUP_TYPE::UI);
	abilityText->SetPos(Vec2(840.f, 140.f));
	abilityText->SetScale(Vec2(196.f, 26.f));


	abilityText->CreateTextUI(L"능력치", -(abilityText->GetScale() / 2.f), (abilityText->GetScale() / 2.f)
		, 22, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	
	AddObject(abilityText, GROUP_TYPE::UI);
	////////////우측 능력치 보여주는 곳////////////////////


	
	////////////현재 레벨 ////////////////////
	CObject* nowLevelIcon = new CSpriteUI;
	nowLevelIcon->SetName(L"nowLevelIcon");
	nowLevelIcon->SetObjType(GROUP_TYPE::UI);
	nowLevelIcon->AddImage(pD2DMgr->GetStoredBitmap(L"brotato_icon"));
	nowLevelIcon->SetPos(Vec2(760.f, 170.f));
	nowLevelIcon->SetScale(Vec2(15.f, 15.f));
	
	AddObject(nowLevelIcon, GROUP_TYPE::UI);

	// 
	CObject* nowLevelText = new CSpriteUI;
	nowLevelText->SetName(L"nowLevelText");
	nowLevelText->SetObjType(GROUP_TYPE::UI);
	//740 + Alpha
	nowLevelText->SetPos(Vec2(800.f, 170.f));
	nowLevelText->SetScale(Vec2(196.f, 35.f));
	swprintf_s(buffer, L"현재 레벨");


	nowLevelText->CreateTextUI(buffer, -(nowLevelText->GetScale() / 2.f), (nowLevelText->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	
	AddObject(nowLevelText, GROUP_TYPE::UI);

	//

	CObject* nowLevelCount = new CSpriteUI;
	nowLevelCount->SetName(L"nowLevelCount");
	nowLevelCount->SetObjType(GROUP_TYPE::UI);
	//740 + Alpha
	nowLevelCount->SetPos(Vec2(920.f, 170.f));
	nowLevelCount->SetScale(Vec2(35.f, 35.f));
	swprintf_s(buffer, L"%d", playerInfo.m_iLevel);


	nowLevelCount->CreateTextUI(buffer, -(nowLevelCount->GetScale() / 2.f), (nowLevelCount->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	
	AddObject(nowLevelCount, GROUP_TYPE::UI);
	////////////현재 레벨 ////////////////////

	////////////최대 HP ////////////////////
	CObject* maxHPIcon = new CSpriteUI;
	maxHPIcon->SetName(L"maxHPIcon");
	maxHPIcon->SetObjType(GROUP_TYPE::UI);
	maxHPIcon->AddImage(pD2DMgr->GetStoredBitmap(L"max_hp"));
	maxHPIcon->SetPos(Vec2(760.f, 200.f));
	maxHPIcon->SetScale(Vec2(15.f, 15.f));
	
	AddObject(maxHPIcon, GROUP_TYPE::UI);

	// 
	CObject* maxHPText = new CSpriteUI;
	maxHPText->SetName(L"maxHPText");
	maxHPText->SetObjType(GROUP_TYPE::UI);
	//740 + Alpha
	maxHPText->SetPos(Vec2(796.f, 200.f));
	maxHPText->SetScale(Vec2(196.f, 35.f));
	swprintf_s(buffer, L"최대 HP");


	maxHPText->CreateTextUI(buffer, -(maxHPText->GetScale() / 2.f), (maxHPText->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	
	AddObject(maxHPText, GROUP_TYPE::UI);

	//

	CObject* maxHPCount = new CSpriteUI;
	maxHPCount->SetName(L"maxHPCount");
	maxHPCount->SetObjType(GROUP_TYPE::UI);
	//740 + Alpha
	maxHPCount->SetPos(Vec2(920.f, 200.f));
	maxHPCount->SetScale(Vec2(35.f, 35.f));
	swprintf_s(buffer, L"%d", playerInfo.m_iMaxHP);


	maxHPCount->CreateTextUI(buffer, -(maxHPCount->GetScale() / 2.f), (maxHPCount->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	
	AddObject(maxHPCount, GROUP_TYPE::UI);
	////////////최대 HP ////////////////////

	////////////데미지 % ////////////////////
	CObject* FinalDMGIcon = new CSpriteUI;
	FinalDMGIcon->SetName(L"FinalDMGIcon");
	FinalDMGIcon->SetObjType(GROUP_TYPE::UI);
	FinalDMGIcon->AddImage(pD2DMgr->GetStoredBitmap(L"percent_damage"));
	FinalDMGIcon->SetPos(Vec2(760.f, 230.f));
	FinalDMGIcon->SetScale(Vec2(15.f, 15.f));
	
	AddObject(FinalDMGIcon, GROUP_TYPE::UI);

	// 
	CObject* FinalDMGText = new CSpriteUI;
	FinalDMGText->SetName(L"FinalDMGText");
	FinalDMGText->SetObjType(GROUP_TYPE::UI);
	//740 + Alpha
	FinalDMGText->SetPos(Vec2(800.f, 230.f));
	FinalDMGText->SetScale(Vec2(196.f, 35.f));
	swprintf_s(buffer, L"%% 데미지");


	FinalDMGText->CreateTextUI(buffer, -(FinalDMGText->GetScale() / 2.f), (FinalDMGText->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	
	AddObject(FinalDMGText, GROUP_TYPE::UI);

	//

	CObject* FinalDMGCount = new CSpriteUI;
	FinalDMGCount->SetName(L"FinalDMGCount");
	FinalDMGCount->SetObjType(GROUP_TYPE::UI);
	//740 + Alpha
	FinalDMGCount->SetPos(Vec2(920.f, 230.f));
	FinalDMGCount->SetScale(Vec2(35.f, 35.f));
	swprintf_s(buffer, L"%d", static_cast<int>(playerInfo.m_fDamageCoef));


	FinalDMGCount->CreateTextUI(buffer, -(FinalDMGCount->GetScale() / 2.f), (FinalDMGCount->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	
	AddObject(FinalDMGCount, GROUP_TYPE::UI);
	////////////데미지 % ////////////////////


	////////////근거리 데미지 % ////////////////////
	CObject* MeleeDMGIcon = new CSpriteUI;
	MeleeDMGIcon->SetName(L"MeleeDMGIcon");
	MeleeDMGIcon->SetObjType(GROUP_TYPE::UI);
	MeleeDMGIcon->AddImage(pD2DMgr->GetStoredBitmap(L"melee_damage"));
	MeleeDMGIcon->SetPos(Vec2(760.f, 260.f));
	MeleeDMGIcon->SetScale(Vec2(15.f, 15.f));

	AddObject(MeleeDMGIcon, GROUP_TYPE::UI);

	// 
	CObject* MeleeText = new CSpriteUI;
	MeleeText->SetName(L"MeleeText");
	MeleeText->SetObjType(GROUP_TYPE::UI);
	//740 + Alpha
	MeleeText->SetPos(Vec2(810.f, 260.f));
	MeleeText->SetScale(Vec2(196.f, 35.f));
	swprintf_s(buffer, L"근거리 데미지");


	MeleeText->CreateTextUI(buffer, -(MeleeText->GetScale() / 2.f), (MeleeText->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	
	AddObject(MeleeText, GROUP_TYPE::UI);

	//

	CObject* MeleeCount = new CSpriteUI;
	MeleeCount->SetName(L"MeleeCount");
	MeleeCount->SetObjType(GROUP_TYPE::UI);
	//740 + Alpha
	MeleeCount->SetPos(Vec2(920.f, 260.f));
	MeleeCount->SetScale(Vec2(35.f, 35.f));
	swprintf_s(buffer, L"%d", static_cast<int>(playerInfo.m_fMeleeCoef));


	MeleeCount->CreateTextUI(buffer, -(MeleeCount->GetScale() / 2.f), (MeleeCount->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	
	AddObject(MeleeCount, GROUP_TYPE::UI);
	////////////근거리 데미지 % ////////////////////


	////////////원거리 데미지 % ////////////////////
	CObject* rangedDMGIcon = new CSpriteUI;
	rangedDMGIcon->SetName(L"rangedDMGIcon");
	rangedDMGIcon->SetObjType(GROUP_TYPE::UI);
	rangedDMGIcon->AddImage(pD2DMgr->GetStoredBitmap(L"ranged_damage"));
	rangedDMGIcon->SetPos(Vec2(760.f, 290.f));
	rangedDMGIcon->SetScale(Vec2(15.f, 15.f));
	
	AddObject(rangedDMGIcon, GROUP_TYPE::UI);

	// 
	CObject* rangedText = new CSpriteUI;
	rangedText->SetObjType(GROUP_TYPE::UI);
	//740 + Alpha
	rangedText->SetPos(Vec2(810.f, 290.f));
	rangedText->SetScale(Vec2(196.f, 35.f));
	swprintf_s(buffer, L"원거리 데미지");


	rangedText->CreateTextUI(buffer, -(rangedText->GetScale() / 2.f), (rangedText->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	
	AddObject(rangedText, GROUP_TYPE::UI);

	//

	CObject* rangedCount = new CSpriteUI;
	rangedCount->SetName(L"rangedCount");
	rangedCount->SetObjType(GROUP_TYPE::UI);
	//740 + Alpha
	rangedCount->SetPos(Vec2(920.f, 290.f));
	rangedCount->SetScale(Vec2(35.f, 35.f));
	swprintf_s(buffer, L"%d", static_cast<int>(playerInfo.m_fRangeCoef));


	rangedCount->CreateTextUI(buffer, -(rangedCount->GetScale() / 2.f), (rangedCount->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	
	AddObject(rangedCount, GROUP_TYPE::UI);
	////////////원거리 데미지 % ////////////////////


	////////////공격속도  % ////////////////////
	CObject* attackSpeedIcon = new CSpriteUI;
	attackSpeedIcon->SetName(L"attackSpeedIcon");
	attackSpeedIcon->SetObjType(GROUP_TYPE::UI);
	attackSpeedIcon->AddImage(pD2DMgr->GetStoredBitmap(L"attack_speed"));
	attackSpeedIcon->SetPos(Vec2(760.f, 320.f));
	attackSpeedIcon->SetScale(Vec2(15.f, 15.f));
	
	AddObject(attackSpeedIcon, GROUP_TYPE::UI);

	// 
	CObject* attackSpeedText = new CSpriteUI;
	attackSpeedText->SetName(L"attackSpeedText");
	attackSpeedText->SetObjType(GROUP_TYPE::UI);
	//740 + Alpha
	attackSpeedText->SetPos(Vec2(805.f, 320.f));
	attackSpeedText->SetScale(Vec2(196.f, 35.f));
	swprintf_s(buffer, L"%% 공격 속도");


	attackSpeedText->CreateTextUI(buffer, -(attackSpeedText->GetScale() / 2.f), (attackSpeedText->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	
	AddObject(attackSpeedText, GROUP_TYPE::UI);

	//

	CObject* attackSpeedCount = new CSpriteUI;
	attackSpeedCount->SetName(L"attackSpeedCount");
	attackSpeedCount->SetObjType(GROUP_TYPE::UI);
	//740 + Alpha
	attackSpeedCount->SetPos(Vec2(920.f, 320.f));
	attackSpeedCount->SetScale(Vec2(35.f, 35.f));
	swprintf_s(buffer, L"%d", static_cast<int>(playerInfo.m_fAttackSpeedCoef));


	attackSpeedCount->CreateTextUI(buffer, -(attackSpeedCount->GetScale() / 2.f), (attackSpeedCount->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	
	AddObject(attackSpeedCount, GROUP_TYPE::UI);
	////////////공격속도 % ////////////////////


	////////////치명타율  % ////////////////////
	CObject* critChanceIcon = new CSpriteUI;
	critChanceIcon->SetName(L"critChanceIcon");
	critChanceIcon->SetObjType(GROUP_TYPE::UI);
	critChanceIcon->AddImage(pD2DMgr->GetStoredBitmap(L"crit_chance"));
	critChanceIcon->SetPos(Vec2(760.f, 350.f));
	critChanceIcon->SetScale(Vec2(15.f, 15.f));
	
	AddObject(critChanceIcon, GROUP_TYPE::UI);

	// 
	CObject* critChanceText = new CSpriteUI;
	critChanceText->SetName(L"critChanceText");
	critChanceText->SetObjType(GROUP_TYPE::UI);
	//740 + Alpha
	critChanceText->SetPos(Vec2(803.f, 350.f));
	critChanceText->SetScale(Vec2(196.f, 35.f));
	swprintf_s(buffer, L"%% 치명타율");


	critChanceText->CreateTextUI(buffer, -(critChanceText->GetScale() / 2.f), (critChanceText->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	
	AddObject(critChanceText, GROUP_TYPE::UI);

	//

	CObject* critChanceCount = new CSpriteUI;
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

	AddObject(critChanceCount, GROUP_TYPE::UI);
	////////////치명타율 % ////////////////////

	////////////속도  % ////////////////////
	CObject* speedIcon = new CSpriteUI;
	speedIcon->SetName(L"speedIcon");
	speedIcon->SetObjType(GROUP_TYPE::UI);
	speedIcon->AddImage(pD2DMgr->GetStoredBitmap(L"speed"));
	speedIcon->SetPos(Vec2(760.f, 380.f));
	speedIcon->SetScale(Vec2(15.f, 15.f));
	
	AddObject(speedIcon, GROUP_TYPE::UI);

	// 
	CObject* speedText = new CSpriteUI;
	speedText->SetName(L"speedText");
	speedText->SetObjType(GROUP_TYPE::UI);
	//740 + Alpha
	speedText->SetPos(Vec2(792.f, 380.f));
	speedText->SetScale(Vec2(196.f, 35.f));

	swprintf_s(buffer, L"%% 속도");


	speedText->CreateTextUI(buffer, -(speedText->GetScale() / 2.f), (speedText->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	
	AddObject(speedText, GROUP_TYPE::UI);

	//

	CObject* speedCount = new CSpriteUI;
	speedCount->SetName(L"speedCount");
	speedCount->SetObjType(GROUP_TYPE::UI);
	//740 + Alpha
	speedCount->SetPos(Vec2(920.f, 380.f));
	speedCount->SetScale(Vec2(35.f, 35.f));
	swprintf_s(buffer, L"%d", static_cast<int>(playerInfo.m_fSpeed));


	speedCount->CreateTextUI(buffer, -(speedCount->GetScale() / 2.f), (speedCount->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	
	AddObject(speedCount, GROUP_TYPE::UI);
	////////////치명타율 % ////////////////////
}