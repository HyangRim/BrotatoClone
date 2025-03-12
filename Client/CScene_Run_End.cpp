#include "pch.h"
#include "CScene_Run_End.h"

#include "CCore.h"
#include "Direct2DMgr.h"
#include "CSceneMgr.h"
#include "ItemMgr.h"
#include "CWaveMgr.h"

#include "CObject.h"
#include "CSpriteUI.h"
#include "CTextUI.h"
#include "CBtnUI.h"
#include "CUI.h"
#include "CWeapon.h"
#include "CPanelUI.h"
#include "CScene.h"
#include "CPlayer.h"

CScene_Run_End::CScene_Run_End()
{

}

CScene_Run_End::~CScene_Run_End()
{

}

void CScene_Run_End::update()
{
	CScene::update();
}


void CScene_Run_End::render(ID2D1HwndRenderTarget* _pRender)
{
	CScene::render(_pRender);
	RenderScrollArea(_pRender);
}

void CScene_Run_End::Enter()
{
	wchar_t buffer[20];
	size_t bufferSize = sizeof(buffer) / sizeof(wchar_t);
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	CWaveMgr* waveMgr = CWaveMgr::GetInstance();

	/////////////////뒷 배경 오브젝트/////////////////
	CreateBackGround(pD2DMgr, vResolution);
	/////////////////뒷 배경 오브젝트/////////////////

	//////////////////뒷 판떼기///////////////////////
	CreateMainPanel();
	//////////////////뒷 판떼기///////////////////////
	
	/////////////////재시작 버튼//////////////////////
	//244,506
	CreateReRunBtn();
	/////////////////재시작 버튼//////////////////////

	/////////////////새로달리기 버튼//////////////////
	CreateNewRunBtn();
	/////////////////새로달리기 버튼//////////////////

	/////////////////메인메뉴로 돌아가기 버튼/////////
	CreateReturnMainMenuBtn();
	/////////////////메인메뉴로 돌아가기 버튼/////////

	////////////상단 실패or성공 - 격돌지대 text ui////////////////////
	int wave = waveMgr->GetLevel();

	CreateWaveInfoTextPanel(wave);
	////////////상단 실패or성공 - 격돌지대 text ui////////////////////

	//////////////////웨이브 정보/////////////////////////////////////
	CreateWaveInfoNumberPanel(buffer, bufferSize, wave);
	//////////////////웨이브 정보/////////////////////////////////////
}

void CScene_Run_End::Exit()
{
	Safe_Delete_Vec(m_scrollContent);
	DeleteAll();
}


void CScene_Run_End::CreateMainPanel()
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	//뒷 판떼기
	CPanelUI* backPanel = new CPanelUI;
	backPanel->SetObjType(GROUP_TYPE::DEFAULT);
	backPanel->SetPos(vResolution / 2.f);
	backPanel->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));
	backPanel->SetNormalAlpha(0.5f);
	backPanel->SetMouseOnAlpha(0.5f);
	backPanel->SetScale(Vec2(882.f, 428.f));
	AddObject(backPanel, GROUP_TYPE::DEFAULT);

	//정보창
	CreateInfoPanel(backPanel);

	//무기
	CreateWeaponInfoPanel(backPanel);

	//아이템
	CreateItemInfoPanel(backPanel);
}

void CScene_Run_End::CreateInfoPanel(CPanelUI* _backPanel)
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	CWaveMgr* waveMgr = CWaveMgr::GetInstance();
	wchar_t buffer[20];
	//플레이어 Info 출력. 
	const playerParameter playerInfo = static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->GetPlayerInfo();

	//////////////////우측 능력치 보여주는 곳///////////////////////
	//CPanelUI* parameterPanel = new CPanelUI;
	CPanelUI* parameterPanel = _backPanel->AddChild<CPanelUI>(Vec2(-_backPanel->GetScale().x / 2.f + 100.f + 20.f, 0.f));
	parameterPanel->SetName(L"parameterPanel");
	parameterPanel->SetObjType(GROUP_TYPE::IMAGE);
	//parameterPanel->SetPos(_panelPos);
	parameterPanel->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));
	parameterPanel->SetScale(Vec2(200.f, 300.f));

	//AddObject(parameterPanel, GROUP_TYPE::IMAGE);
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
	CPanelUI* statLevelPanel = parameterPanel->AddChild<CPanelUI>(Vec2(0.f ,-80.f));
	statLevelPanel->SetScale(Vec2(180.f, 15.f));
	statLevelPanel->SetColor(D2D1::ColorF(0, 0, 0, 0), D2D1::ColorF(0, 0, 0, 0));
	
	//레벨 아이콘
	CSpriteUI* nowLevelIcon = statLevelPanel->AddChild<CSpriteUI>(Vec2(-statLevelPanel->GetScale().x / 2.f + 10.f , 0.f));
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

	//현재 최대hp 아이콘
	CSpriteUI* maxHPIcon = statMaxHpPanel->AddChild<CSpriteUI>(Vec2(-statMaxHpPanel->GetScale().x / 2.f + 10.f , 0.f));
	maxHPIcon->SetName(L"maxHPIcon");
	maxHPIcon->SetObjType(GROUP_TYPE::UI);
	maxHPIcon->AddImage(pD2DMgr->GetStoredBitmap(L"max_hp"));
	maxHPIcon->SetScale(Vec2(15.f, 15.f));

	//현재 최대hp text
	CSpriteUI* maxHPText = statMaxHpPanel->AddChild<CSpriteUI>(Vec2(0.f,0.f));
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
	swprintf_s(buffer, L"%d", playerInfo.m_iMaxHP);
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

	//
	CSpriteUI* FinalDMGIcon = statDamagePanel->AddChild<CSpriteUI>(Vec2(-statDamagePanel->GetScale().x / 2.f + 10.f, 0.f));
	FinalDMGIcon->SetName(L"FinalDMGIcon");
	FinalDMGIcon->SetObjType(GROUP_TYPE::UI);
	FinalDMGIcon->AddImage(pD2DMgr->GetStoredBitmap(L"percent_damage"));
	FinalDMGIcon->SetScale(Vec2(15.f, 15.f));

	// 
	CSpriteUI* FinalDMGText = statDamagePanel->AddChild<CSpriteUI>(Vec2(0.f,0.f));
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

	//C
	CSpriteUI* critChanceIcon = statCritChancePanel->AddChild<CSpriteUI>(Vec2(-statCritChancePanel->GetScale().x / 2.f + 10.f, 0.f));
	critChanceIcon->SetName(L"critChanceIcon");
	critChanceIcon->SetObjType(GROUP_TYPE::UI);
	critChanceIcon->AddImage(pD2DMgr->GetStoredBitmap(L"crit_chance"));
	critChanceIcon->SetScale(Vec2(15.f, 15.f));

	// 
	CSpriteUI* critChanceText = statCritChancePanel->AddChild<CSpriteUI>(Vec2(0.f,0.f));
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
	swprintf_s(buffer, L"%d", static_cast<int>(playerInfo.m_fSpeed));
	speedCount->CreateTextUI(buffer, -(speedCount->GetScale() / 2.f), (speedCount->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	////////////치명타율 % ////////////////////

	
}

void CScene_Run_End::CreateItemInfoPanel(CPanelUI* _backPanel)
{
	wchar_t buffer[20];

	CPanelUI* panelItemArea = _backPanel->AddChild<CPanelUI>(Vec2(-20.f, 60.f));
	panelItemArea->SetName(L"PanelWeaponArea");
	panelItemArea->SetObjType(GROUP_TYPE::IMAGE);
	panelItemArea->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));
	panelItemArea->SetScale(Vec2(376.f, 300.f));
	panelItemArea->SetMouseOnAlpha(0.f);
	panelItemArea->SetNormalAlpha(0.f);

	CSpriteUI* itemText =
		panelItemArea->AddChild<CSpriteUI>(Vec2(-panelItemArea->GetScale().x / 2.f + 30.f + 5.f
			, -panelItemArea->GetScale().y / 2.f + 17.f));
	itemText->SetScale(Vec2(60.f, 35.f));
	swprintf_s(buffer, L"아이템");
	itemText->CreateTextUI(buffer, -(itemText->GetScale() / 2.f), (itemText->GetScale() / 2.f)
		, 18, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);

	CreateScrollArea();
}

void CScene_Run_End::CreateWeaponInfoPanel(CPanelUI* _backPanel)
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	wchar_t buffer[20];

	CPanelUI* panelWeaponArea = _backPanel->AddChild<CPanelUI>(Vec2(-20.f, -150.f));
	panelWeaponArea->SetName(L"PanelWeaponArea");
	panelWeaponArea->SetObjType(GROUP_TYPE::IMAGE);
	panelWeaponArea->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));
	panelWeaponArea->SetScale(Vec2(376.f, 104.f));
	panelWeaponArea->SetMouseOnAlpha(0.f);
	panelWeaponArea->SetNormalAlpha(0.f);
	
	CSpriteUI* weaponText = 
		panelWeaponArea->AddChild<CSpriteUI>(Vec2(-panelWeaponArea->GetScale().x / 2.f + 22.f + 5.f
												, -panelWeaponArea->GetScale().y/2.f + 17.f));
	weaponText->SetScale(Vec2(45.f, 35.f));
	swprintf_s(buffer, L"무기");
	weaponText->CreateTextUI(buffer, -(weaponText->GetScale() / 2.f), (weaponText->GetScale() / 2.f)
		, 18, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);


	const list<CWeapon*>& vPlayerWeaponList = static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->GetPlayerWeapons();
	CPlayer* tmp = static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer());
	int curWeaponCnt = (int)((CPlayer*)CSceneMgr::GetInstance()->GetPlayer())->GetWeaponCount();

	CPanelUI* weaponImages = panelWeaponArea->AddChild<CPanelUI>(Vec2(0.f, 15.f));
	weaponImages->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));
	weaponImages->SetScale(Vec2(376.f, 60.f));
	weaponImages->SetMouseOnAlpha(0.f);
	weaponImages->SetNormalAlpha(0.f);

	int temp = 0;
	for (auto iter = vPlayerWeaponList.begin(); iter != vPlayerWeaponList.end(); iter++, temp++)
	{
		CWeapon* weapon = *iter;
		wstring iconTag = weapon->Getinfo().m_sIconImageKey;
		CSpriteUI* weapons = weaponImages->AddChild<CSpriteUI>(Vec2(-155.f + temp*55.f ,0.f));
		weapons->SetName(L"Weapon");
		weapons->AddImage(pD2DMgr->GetStoredBitmap(iconTag));
		weapons->SetObjType(GROUP_TYPE::UI);
		weapons->SetScale(Vec2(50.f, 50.f));
		weapons->SetBackGround(true);
		weapons->SetBackGroundColor(ColorNormalize(30, 30, 30), ColorNormalize(0, 0, 0)
			, ColorNormalize(30, 30, 30), ColorNormalize(0, 0, 0));
		weapons->SetIsRound(true, 10.f, 10.f);
	}
}

void CScene_Run_End::CreateBackGround(Direct2DMgr* _pD2DMgr, Vec2 _vResolution)
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

void CScene_Run_End::CreateReRunBtn()
{
	/////////////////재시작 버튼//////////////////////
	//244,506
	CBtnUI* rerunBtn = new CBtnUI;
	rerunBtn->SetName(L"RerunBtn");
	rerunBtn->SetObjType(GROUP_TYPE::UI);
	rerunBtn->SetScale(Vec2(152.f, 34.f));
	rerunBtn->SetIsRound(true, 10.f, 10.f);
	rerunBtn->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(0, 0, 0));
	rerunBtn->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::START, 0);
	rerunBtn->CreateTextUI(L"재시작", -(rerunBtn->GetScale() / 2.f), (rerunBtn->GetScale() / 2.f)
		, 20, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	rerunBtn->SetPos(Vec2(244.f, 506.f));
	AddObject(rerunBtn, GROUP_TYPE::UI);
	/////////////////재시작 버튼//////////////////////
}

void CScene_Run_End::CreateNewRunBtn()
{
	/////////////////새로달리기 버튼//////////////////
	CBtnUI* newRunBtn = new CBtnUI;
	newRunBtn->SetName(L"NewRunBtn");
	newRunBtn->SetObjType(GROUP_TYPE::UI);
	newRunBtn->SetScale(Vec2(150.f, 34.f));
	newRunBtn->SetIsRound(true, 10.f, 10.f);
	newRunBtn->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(0, 0, 0));
	newRunBtn->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::SELECT_CHARACTER, 0);
	newRunBtn->CreateTextUI(L"새로운 달리기", -(newRunBtn->GetScale() / 2.f), (newRunBtn->GetScale() / 2.f)
		, 20, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	newRunBtn->SetPos(Vec2(406.f, 506.f));
	AddObject(newRunBtn, GROUP_TYPE::UI);
	/////////////////새로달리기 버튼//////////////////
}

void CScene_Run_End::CreateReturnMainMenuBtn()
{
	/////////////////메인메뉴로 돌아가기 버튼/////////
	CBtnUI* returnToMainMenuBtn = new CBtnUI;
	returnToMainMenuBtn->SetName(L"ReturnToMainMenu");
	returnToMainMenuBtn->SetObjType(GROUP_TYPE::UI);
	returnToMainMenuBtn->SetScale(Vec2(302.f, 34.f));
	returnToMainMenuBtn->SetIsRound(true, 10.f, 10.f);
	returnToMainMenuBtn->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(0, 0, 0));
	returnToMainMenuBtn->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::MAIN, 0);
	returnToMainMenuBtn->CreateTextUI(L"메인 메뉴로 돌아가기", -(returnToMainMenuBtn->GetScale() / 2.f), (returnToMainMenuBtn->GetScale() / 2.f)
		, 20, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	returnToMainMenuBtn->SetPos(Vec2(640.f, 506.f));
	AddObject(returnToMainMenuBtn, GROUP_TYPE::UI);
	/////////////////메인메뉴로 돌아가기 버튼/////////
}

void CScene_Run_End::CreateWaveInfoTextPanel(int _wave)
{
	////////////상단 실패or성공 - 격돌지대 text ui////////////////////
	CObject* panelTextRunSuccessOrFail = new CSpriteUI;
	panelTextRunSuccessOrFail->SetObjType(GROUP_TYPE::UI);
	panelTextRunSuccessOrFail->SetPos(Vec2(360.f, 28.f));
	panelTextRunSuccessOrFail->SetScale(Vec2(196.f, 26.f));

	wchar_t buffer[20];
	if (_wave <= 10)
		swprintf_s(buffer, L"달리기 패배 - 격돌지대");
	else
		swprintf_s(buffer, L"달리기 성공 - 격돌지대");

	panelTextRunSuccessOrFail->CreateTextUI(buffer, -(panelTextRunSuccessOrFail->GetScale() / 2.f), (panelTextRunSuccessOrFail->GetScale() / 2.f)
		, 18, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(panelTextRunSuccessOrFail, GROUP_TYPE::UI);
	////////////상단 실패or성공 - 격돌지대 text ui////////////////////
}

void CScene_Run_End::CreateWaveInfoNumberPanel(wchar_t *buffer, size_t bufferSize, int _wave)
{
	//////////////////웨이브 정보/////////////////////////////////////
	CObject* panelTextWaveInfo = new CSpriteUI;
	panelTextWaveInfo->SetObjType(GROUP_TYPE::UI);
	panelTextWaveInfo->SetPos(Vec2(600.f, 28.f));
	panelTextWaveInfo->SetScale(Vec2(74.f, 28.f));

	swprintf_s(buffer, bufferSize, L"웨이브 %d", _wave);
	panelTextWaveInfo->CreateTextUI(buffer, -(panelTextWaveInfo->GetScale() / 2.f), (panelTextWaveInfo->GetScale() / 2.f)
		, 18, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(panelTextWaveInfo, GROUP_TYPE::UI);
	//////////////////웨이브 정보/////////////////////////////////////
}

void CScene_Run_End::CreateScrollArea()
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	// 스크롤 영역 설정
	m_scrollArea.viewRect = D2D1::RectF(272.f, 220.f, 668.f, 480.f);
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

		int xCount = i % 7;
		int yCount = i / 7;

		possessedItem->SetPos(Vec2(35.f + xCount * 54.f, 27.f + yCount * 54.f));
		possessedItem->SetScale(Vec2(50.f, 50.f));

		possessedItem->SetBackGround(true);
		possessedItem->SetBackGroundColor(ColorNormalize(30, 30, 30), ColorNormalize(0, 0, 0)
			, ColorNormalize(30, 30, 30), ColorNormalize(0, 0, 0));
		possessedItem->SetIsRound(true, 10.f, 10.f);

		m_scrollContent.push_back(possessedItem);
	}
}

void CScene_Run_End::UpdateScrollPosition(float deltaY)
{
	m_scrollArea.scrollPos.y += deltaY;
	m_scrollArea.scrollPos.y = max(0.f, min(m_scrollArea.scrollPos.y,
		m_scrollArea.contentRect.bottom - (m_scrollArea.viewRect.bottom - m_scrollArea.viewRect.top)));
}

void CScene_Run_End::RenderScrollArea(ID2D1HwndRenderTarget* _pRender)
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

