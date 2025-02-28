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

	/////////////////�� ��� ������Ʈ/////////////////
	CObject* backGround = new CSpriteUI;
	backGround->SetObjType(GROUP_TYPE::DEFAULT);
	backGround->AddImage(pD2DMgr->GetStoredBitmap(L"shop_background"));
	backGround->SetPos(vResolution / 2.f);
	backGround->SetScale(vResolution);
	AddObject(backGround, GROUP_TYPE::DEFAULT);
	/////////////////�� ��� ������Ʈ/////////////////

	//////////////////�� �Ƕ���///////////////////////
	CPanelUI* backPanel = new CPanelUI;
	backPanel->SetObjType(GROUP_TYPE::DEFAULT);
	backPanel->SetPos(vResolution / 2.f);
	backPanel->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));
	backPanel->SetNormalAlpha(0.5f);
	backPanel->SetMouseOnAlpha(0.5f);
	backPanel->SetScale(Vec2(882.f, 428.f));
	AddObject(backPanel, GROUP_TYPE::DEFAULT);
	//////////////////�� �Ƕ���///////////////////////
	
	/////////////////����� ��ư//////////////////////
	//244,506
	CBtnUI* rerunBtn = new CBtnUI;
	rerunBtn->SetName(L"RerunBtn");
	rerunBtn->SetObjType(GROUP_TYPE::UI);
	rerunBtn->SetScale(Vec2(152.f, 34.f));
	rerunBtn->SetIsRound(true, 10.f, 10.f);
	rerunBtn->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(0, 0, 0));

	rerunBtn->CreateTextUI(L"�����", -(rerunBtn->GetScale()/2.f), (rerunBtn->GetScale() / 2.f)
		, 20, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	rerunBtn->SetPos(Vec2(244.f, 506.f));
	AddObject(rerunBtn, GROUP_TYPE::UI);
	/////////////////����� ��ư//////////////////////

	/////////////////���δ޸��� ��ư//////////////////
	CBtnUI* newRunBtn = new CBtnUI;
	newRunBtn->SetName(L"NewRunBtn");
	newRunBtn->SetObjType(GROUP_TYPE::UI);
	newRunBtn->SetScale(Vec2(150.f, 34.f));
	newRunBtn->SetIsRound(true, 10.f, 10.f);
	newRunBtn->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(0, 0, 0));
	newRunBtn->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::SELECT_CHARACTER, 0);
	newRunBtn->CreateTextUI(L"���ο� �޸���", -(newRunBtn->GetScale() / 2.f), (newRunBtn->GetScale() / 2.f)
		, 20, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	newRunBtn->SetPos(Vec2(406.f, 506.f));
	AddObject(newRunBtn, GROUP_TYPE::UI);
	/////////////////���δ޸��� ��ư//////////////////

	/////////////////���θ޴��� ���ư��� ��ư/////////
	CBtnUI* returnToMainMenuBtn = new CBtnUI;
	returnToMainMenuBtn->SetName(L"ReturnToMainMenu");
	returnToMainMenuBtn->SetObjType(GROUP_TYPE::UI);
	returnToMainMenuBtn->SetScale(Vec2(302.f, 34.f));
	returnToMainMenuBtn->SetIsRound(true, 10.f, 10.f);
	returnToMainMenuBtn->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(0, 0, 0));
	returnToMainMenuBtn->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::MAIN, 0);
	returnToMainMenuBtn->CreateTextUI(L"���� �޴��� ���ư���", -(returnToMainMenuBtn->GetScale() / 2.f), (returnToMainMenuBtn->GetScale() / 2.f)
		, 20, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	returnToMainMenuBtn->SetPos(Vec2(640.f, 506.f));
	AddObject(returnToMainMenuBtn, GROUP_TYPE::UI);
	/////////////////���θ޴��� ���ư��� ��ư/////////


	////////////��� ����or���� - �ݵ����� text ui////////////////////
	int wave = waveMgr->GetLevel();

	CObject* panelTextRunSuccessOrFail = new CSpriteUI;
	panelTextRunSuccessOrFail->SetObjType(GROUP_TYPE::UI);
	panelTextRunSuccessOrFail->SetPos(Vec2(360.f,28.f));
	panelTextRunSuccessOrFail->SetScale(Vec2(196.f, 26.f));

	wchar_t buffer[20];
	if(wave<= 20)
		swprintf_s(buffer, L"�޸��� �й� - �ݵ�����");
	else
		swprintf_s(buffer, L"�޸��� ���� - �ݵ�����");

	panelTextRunSuccessOrFail->CreateTextUI(buffer, -(panelTextRunSuccessOrFail->GetScale() / 2.f), (panelTextRunSuccessOrFail->GetScale() / 2.f)
		, 18, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(panelTextRunSuccessOrFail, GROUP_TYPE::UI);
	////////////��� ����or���� - �ݵ����� text ui////////////////////

	//////////////////���̺� ����/////////////////////////////////////
	CObject* panelTextWaveInfo = new CSpriteUI;
	panelTextWaveInfo->SetObjType(GROUP_TYPE::UI);
	panelTextWaveInfo->SetPos(Vec2(600.f, 28.f));
	panelTextWaveInfo->SetScale(Vec2(74.f, 28.f));

	swprintf_s(buffer, L"���̺� %d", wave);
	panelTextWaveInfo->CreateTextUI(buffer, -(panelTextWaveInfo->GetScale() / 2.f), (panelTextWaveInfo->GetScale() / 2.f)
		, 18, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(panelTextWaveInfo, GROUP_TYPE::UI);
	//////////////////���̺� ����/////////////////////////////////////
}

void CScene_Run_End::Exit()
{
	DeleteAll();
}
