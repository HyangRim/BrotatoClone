#include "pch.h"
#include "CScene_Run_End.h"

#include "CCore.h"
#include "Direct2DMgr.h"
#include "CWaveMgr.h"

#include "CObject.h"
#include "CSpriteUI.h"
#include "CBtnUI.h"
#include "CUI.h"
#include "CPanelUI.h"
#include "CScene.h"

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
}

void CScene_Run_End::Enter()
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	CWaveMgr* waveMgr = CWaveMgr::GetInstance();

	/////////////////뒷 배경 오브젝트/////////////////
	CObject* backGround = new CSpriteUI;
	backGround->SetObjType(GROUP_TYPE::DEFAULT);
	backGround->AddImage(pD2DMgr->GetStoredBitmap(L"shop_background"));
	backGround->SetPos(vResolution / 2.f);
	backGround->SetScale(vResolution);
	AddObject(backGround, GROUP_TYPE::DEFAULT);
	/////////////////뒷 배경 오브젝트/////////////////

	//////////////////뒷 판떼기///////////////////////
	CPanelUI* backPanel = new CPanelUI;
	backPanel->SetObjType(GROUP_TYPE::DEFAULT);
	backPanel->SetPos(vResolution / 2.f);
	backPanel->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));
	backPanel->SetNormalAlpha(0.5f);
	backPanel->SetMouseOnAlpha(0.5f);
	backPanel->SetScale(Vec2(882.f, 428.f));
	AddObject(backPanel, GROUP_TYPE::DEFAULT);
	//////////////////뒷 판떼기///////////////////////
	
	/////////////////재시작 버튼//////////////////////
	//244,506
	CBtnUI* rerunBtn = new CBtnUI;
	rerunBtn->SetName(L"RerunBtn");
	rerunBtn->SetObjType(GROUP_TYPE::UI);
	rerunBtn->SetScale(Vec2(152.f, 34.f));
	rerunBtn->SetIsRound(true, 10.f, 10.f);
	rerunBtn->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(0, 0, 0));

	rerunBtn->CreateTextUI(L"재시작", -(rerunBtn->GetScale()/2.f), (rerunBtn->GetScale() / 2.f)
		, 20, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	rerunBtn->SetPos(Vec2(244.f, 506.f));
	AddObject(rerunBtn, GROUP_TYPE::UI);
	/////////////////재시작 버튼//////////////////////

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


	////////////상단 실패or성공 - 격돌지대 text ui////////////////////
	int wave = waveMgr->GetLevel();

	CObject* panelTextRunSuccessOrFail = new CSpriteUI;
	panelTextRunSuccessOrFail->SetObjType(GROUP_TYPE::UI);
	panelTextRunSuccessOrFail->SetPos(Vec2(360.f,28.f));
	panelTextRunSuccessOrFail->SetScale(Vec2(196.f, 26.f));

	wchar_t buffer[20];
	if(wave<= 20)
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

	//////////////////웨이브 정보/////////////////////////////////////
	CObject* panelTextWaveInfo = new CSpriteUI;
	panelTextWaveInfo->SetObjType(GROUP_TYPE::UI);
	panelTextWaveInfo->SetPos(Vec2(600.f, 28.f));
	panelTextWaveInfo->SetScale(Vec2(74.f, 28.f));

	swprintf_s(buffer, L"웨이브 %d", wave);
	panelTextWaveInfo->CreateTextUI(buffer, -(panelTextWaveInfo->GetScale() / 2.f), (panelTextWaveInfo->GetScale() / 2.f)
		, 18, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(panelTextWaveInfo, GROUP_TYPE::UI);
	//////////////////웨이브 정보/////////////////////////////////////
}

void CScene_Run_End::Exit()
{
	DeleteAll();
}
