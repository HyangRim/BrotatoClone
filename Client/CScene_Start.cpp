#include "pch.h"
#include "CScene.h"
#include "CScene_start.h"
#include "CObject.h"
#include "CCore.h"
#include "CTexture.h"

#include "CPlayer.h"
#include "CMonster.h"
#include "CCollisionMgr.h"
#include "CPathMgr.h"
#include "CSoundMgr.h"


#include "CWeapon.h"
#include "CPistol.h"
#include "CKnife.h"
#include "CSlingshot.h"

#include "CkeyMgr.h"
#include "CSceneMgr.h"
#include "CCamera.h"

#include "AI.h"
#include "CIdleState.h"
#include "CTraceState.h"
#include "CRigidbody.h"
#include "SelectGDI.h"
#include "CTimeMgr.h"
#include "CGround.h"
#include "CImage.h"

#include "Direct2DMgr.h"
#include "ItemMgr.h"
#include "CUIMgr.h"
#include "CPanelUI.h"
#include "CBtnUI.h"
#include "CSpriteUI.h"
#include "CSliderUI.h"
#include "CTextUI.h"
#include "CWaveMgr.h"

#include "CMobSpawner.h"
#include "CTile.h"



constexpr const wchar_t LifebarString[20] = L"Lifebar";
constexpr const wchar_t XpbarString[20] = L"Xpbar";
constexpr const wchar_t HarvestingTextString[20] = L"HarvestingText";


CScene_Start::CScene_Start()
	: m_bUseForce(false)
	, m_fForceRadius(500.f)
	, m_fCurRadius(0.f)
	, m_fForce(500.f)
	, m_fFailDuration(0.f)
	, m_bFailed(false)
{


}

CScene_Start::~CScene_Start()
{

}

void CScene_Start::update()
{

	//게임 도중에 Pause관련. 

	if (KEY_TAP(KEY::ESC)) {
		if (GetPause()) {
			//지금이 상태인데 ESC눌렀을 때. 
			//Pause -> Normal
			OffPause();
		}
		else {
			//지금이 퍼즈가 아닌데 ESC 눌렀을 때.
			//Normal -> Pause
			OnPause();
		}
	}

	if (KEY_TAP(KEY::H)) {
		CreateLevelUpShop();
	}


	if (true == m_bFailed) {
		m_fFailDuration += fDTN;
		
		float lerpAlpha = min(m_fFailDuration * 0.5f, 0.7f);
		m_pFailPanel->SetNormalAlpha(lerpAlpha);
		m_pFailPanel->SetMouseOnAlpha(lerpAlpha);
		

		if (m_fFailDuration >= 2.5f) {
			ChangeScene(SCENE_TYPE::RUN_END);
		}
	}

	UINT typeIDX = (UINT)GROUP_TYPE::UI;
	for (UINT typeIDX = 0; typeIDX < (UINT)GROUP_TYPE::END; typeIDX++) {
		const vector<CObject*>& vecObj = GetGroupObject((GROUP_TYPE)typeIDX);
		wchar_t buffer[20];
		///////////////////////////////////////////
		if (typeIDX == (UINT)GROUP_TYPE::UI)
		{
			for (size_t objIDX = 0; objIDX < vecObj.size(); objIDX++) 
			{
				////////////////////체력바 업뎃//////////////////////////////////
				if (vecObj[objIDX]->GetName().compare(LifebarString) == 0)
				{
					CPlayer* player = static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer());
					playerParameter playerInfo = player->GetPlayerInfo();

					swprintf_s(buffer, L"%d / %d", playerInfo.m_iCurHP, playerInfo.m_iMaxHP + playerInfo.m_AddMaxHP);

					vecObj[objIDX]->GetTextUI()->SetText(buffer);

					//체력바 비율변경
					CImage* image = (CImage*)vecObj[objIDX]->GetImage(1);
					image->SetRatio((float)playerInfo.m_iCurHP/ ((float)playerInfo.m_iMaxHP + (float)playerInfo.m_AddMaxHP));
				}
				///////////////////////////////////////////////////////////////////
				////////////////////경험치바 업뎃//////////////////////////////////
				if (vecObj[objIDX]->GetName().compare(XpbarString) == 0)
				{
					CPlayer* player = static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer());
					playerParameter playerInfo = player->GetPlayerInfo();

					swprintf_s(buffer, L"%d / %d", playerInfo.m_iCurEXP, playerInfo.m_iMaxEXP);

					vecObj[objIDX]->GetTextUI()->SetText(buffer);

					//경험치바 비율변경
					CImage* image = (CImage*)vecObj[objIDX]->GetImage(1);
					image->SetRatio(min((float)((float)playerInfo.m_iCurEXP / (float)playerInfo.m_iMaxEXP), 1.f));
				}
				///////////////////////////////////////////////////////////////////
				////////////////////재화갯수 업뎃//////////////////////////////////
				if (vecObj[objIDX]->GetName().compare(HarvestingTextString) == 0)
				{
					CPlayer* player = static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer());
					playerParameter playerInfo = player->GetPlayerInfo();

					swprintf_s(buffer, L"%d", playerInfo.m_iCoin);

					vecObj[objIDX]->GetTextUI()->SetText(buffer);
				}
				///////////////////////////////////////////////////////////////////
			}
		}
		///////////////////////////////////////////
		//이 부분이 물리 처리하는 부분. 
		for (size_t objIDX = 0; objIDX < vecObj.size(); objIDX++) {
			if (!vecObj[objIDX]->IsDead()) {
				if (m_bUseForce && vecObj[objIDX]->GetRigidbody()) {

					Vec2 vDiff = vecObj[objIDX]->GetPos() - m_vForcePos;
					float fLen = vDiff.Length();
					if (fLen < m_fForceRadius) {
						float fRatio = 1.f - (fLen / m_fForceRadius);
						float fForce = m_fForce * fRatio;

						vecObj[objIDX]->GetRigidbody()->AddForce(vDiff.Normalize() * fForce);
					}
				}

				vecObj[objIDX]->update();
			}
		}
	}
	
	if (KEY_TAP(KEY::ENTER)  ) {
		ChangeScene(SCENE_TYPE::SHOP);
	}

	//음향 텍스트 바꾸는 곳. 
	if (CSoundMgr::GetInstance()->IsOptionPanel()) {
		OptionPanelupdate();
	}
}

void CScene_Start::render(HDC _dc)
{
	CScene::render(_dc);

	if (!m_bUseForce) return;

	SelectGDI gdi1(_dc, BRUSH_TYPE::HOLLOW);
	SelectGDI gdi2(_dc, PEN_TYPE::GREEN);

	m_fCurRadius += m_fForceRadius * 3.f * fDT;

	if (m_fCurRadius > m_fForceRadius) {
		m_fCurRadius = 0.f;
	}

	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(m_vForcePos);
	Ellipse(_dc,
		(int)(vRenderPos.x - m_fCurRadius),
		(int)(vRenderPos.y - m_fCurRadius),
		(int)(vRenderPos.x + m_fCurRadius),
		(int)(vRenderPos.y + m_fCurRadius)
	);
}

void CScene_Start::render(Gdiplus::Graphics* _pDGraphics)
{
	CScene::render(_pDGraphics);

	if (!m_bUseForce) return;

	m_fCurRadius += m_fForceRadius * 3.f * fDT;

	if (m_fCurRadius > m_fForceRadius) {
		m_fCurRadius = 0.f;
	}
	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(m_vForcePos);

	Gdiplus::Pen pen(Gdiplus::Color(255, 0, 255, 0), 1.0f);
	_pDGraphics->DrawEllipse(&pen,
		vRenderPos.x - m_fCurRadius,
		vRenderPos.y - m_fCurRadius,
		m_fCurRadius * 2,
		m_fCurRadius * 2);
}

void CScene_Start::render(ID2D1HwndRenderTarget* _pRender)
{ 
	CScene::render(_pRender);
	if(CScene::GetPause() == true)
		RenderScrollArea(_pRender); 
	
	if (!m_bUseForce) return;

	m_fCurRadius += m_fForceRadius * 3.f * fDT;

	if (m_fCurRadius > m_fForceRadius) {
		m_fCurRadius = 0.f;
	}
	
	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(m_vForcePos);

	// vRenderPos와 m_fCurRadius는 이미 계산되어 있다고 가정합니다.
	D2D1_ELLIPSE ellipse = D2D1::Ellipse(
		D2D1::Point2F(vRenderPos.x, vRenderPos.y),
		m_fCurRadius,    // x축 반지름
		m_fCurRadius     // y축 반지름
	);

	// 녹색 브러시 생성 (ARGB: 255, 0, 255, 0 => 녹색)
	ID2D1SolidColorBrush* pBrush = nullptr;
	HRESULT hr = _pRender->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Green),
		&pBrush
	);
	if (SUCCEEDED(hr))
	{
		_pRender->DrawEllipse(ellipse, pBrush, 1.0f);
		pBrush->Release();
	}
}


void CScene_Start::Enter()
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	if (static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer()))
	{
		CPlayer* player = static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer());
		RegisterPlayer(player);
		AddObject(player, GROUP_TYPE::PLAYER);
		player->SetPos(Vec2(576.f,576.f));

		player->GetCharacterParam().m_iCurHP = player->GetCharacterParam().m_iMaxHP + player->GetCharacterParam().m_AddMaxHP;
	}


	ChangePause(false);
	
	////////////////////////////맵생성///////////////////////////////////////
	int randV = rand() % 7;
	if (randV == 0) randV++;
	wstring mapPath = L"texture\\tiles\\tiles_" + std::to_wstring(randV) + L".png";
	MakeMapTile(L"texture\\tiles\\tiles_outline.png", mapPath.c_str(), L"texture\\tiles\\map\\",50, 1);
	pD2DMgr->StoreBitmapsFromFolder(L"texture\\tiles\\map\\", L"Map");
	//MakeTile(L"Map");

	//새로운 맵 생성 알고리즘
	pD2DMgr->StoreCreateMap(CreateCompositeMapBitmap(L"Map"), L"startSceneMap");
	CObject* pMapObj = new CTile;
	//584
	pMapObj->SetPos(Vec2(569.f, 569.f));
	//1168
	pMapObj->SetScale(Vec2(1138.f, 1138.f));
	pMapObj->SetName(L"MapSprite");
	pMapObj->CreateImage();
	pMapObj->AddImage(pD2DMgr->GetStoredBitmap(L"startSceneMap"));
	AddObject(pMapObj, GROUP_TYPE::TILE);
	
	////////////////////////////////////////////////////////////////////////

	/////////////////////////////체력바,경험치바////////////////////////////
	
	//pD2DMgr->GetStoredBitmap(L"ifebarBackGround")
	CObject* lifebar = new CSpriteUI;
	lifebar->AddImage(pD2DMgr->GetStoredBitmap(L"ui_lifebar_bg"));
	lifebar->AddImage(pD2DMgr->GetStoredBitmap(L"ui_lifebar_fill"));
	lifebar->AddImage(pD2DMgr->GetStoredBitmap(L"ui_lifebar_frame"));
	lifebar->SetPos(Vec2(90.f, 25.f));
	lifebar->SetScale(Vec2(320.f, 48.f) * 0.5f);
	lifebar->CreateTextUI(L"", Vec2(-80.f, -12.f), Vec2(80.f, 12.f)
		, 12, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR, TextUIMode::TEXT, 0);
	lifebar->SetName(L"Lifebar");
	lifebar->SetObjType(GROUP_TYPE::UI);

	CObject* xpbar = new CSpriteUI;
	xpbar->AddImage(pD2DMgr->GetStoredBitmap(L"ui_xp_bg"));
	xpbar->AddImage(pD2DMgr->GetStoredBitmap(L"ui_xp_fill"));
	xpbar->AddImage(pD2DMgr->GetStoredBitmap(L"ui_lifebar_frame"));
	xpbar->SetPos(Vec2(90.f, 60.f));
	xpbar->SetScale(Vec2(320.f, 48.f) * 0.5f);
	xpbar->CreateTextUI(L"", Vec2(-80.f, -12.f), Vec2(80.f, 12.f)
		, 12, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR, TextUIMode::TEXT, 0);
	xpbar->SetName(L"Xpbar");
	xpbar->SetObjType(GROUP_TYPE::UI);

	AddObject(lifebar, GROUP_TYPE::UI);
	AddObject(xpbar, GROUP_TYPE::UI);
	////////////////////////////////////////////////////////////////////////

	////////////////////////////웨이브 글자/////////////////////////////////
	CObject* wavePanel = new CSpriteUI;
	wstring waveLevel = L"웨이브 ";
	waveLevel += std::to_wstring(CWaveMgr::GetInstance()->GetLevel());
	wavePanel->SetScale(Vec2(100.f, 100.f));
	wavePanel->SetPos(Vec2(vResolution.x / 2.f, 25.f));
	wavePanel->CreateTextUI(waveLevel, Vec2(-100.f, -100.f), Vec2(100.f, 100.f)
		, 24, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR, TextUIMode::TEXT, 0);
	wavePanel->SetName(L"WavePanel");
	wavePanel->SetObjType(GROUP_TYPE::UI);

	AddObject(wavePanel, GROUP_TYPE::UI);
	////////////////////////////////////////////////////////////////////////

	////////////////////////////웨이브 숫자/////////////////////////////////
	CObject* countDownPanel = new CSpriteUI;
	countDownPanel->SetScale(Vec2(100.f, 100.f));
	countDownPanel->SetPos(Vec2(vResolution.x / 2.f, 50.f));
	int waveLength = static_cast<int>(CWaveMgr::GetInstance()->GetWaveDuration());
	countDownPanel->CreateTextUI(L"", Vec2(-100.f, -100.f), Vec2(100.f, 100.f)
		, 24, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR, TextUIMode::COUNT_DOWN, waveLength);
	countDownPanel->SetName(L"CountDownPanel");
	countDownPanel->SetObjType(GROUP_TYPE::UI);
	AddObject(countDownPanel, GROUP_TYPE::UI);
	////////////////////////////////////////////////////////////////////////

	////////////////////////////재화 표시///////////////////////////////////
	CObject* harvestingIcon = new CSpriteUI;
	harvestingIcon->AddImage(pD2DMgr->GetStoredBitmap(L"harvesting_icon"));
	harvestingIcon->SetScale(Vec2(96.f, 96.f) * 0.5f);
	harvestingIcon->SetPos(Vec2(30.f, 100.f));

	harvestingIcon->SetName(L"HarvestingIcon");
	harvestingIcon->SetObjType(GROUP_TYPE::UI);
	AddObject(harvestingIcon, GROUP_TYPE::UI);

	CObject* harvestingText = new CSpriteUI;
	harvestingText->SetScale(Vec2(96.f, 96.f) * 0.5f);
	harvestingText->SetPos(Vec2(70.f, 100.f));
	harvestingText->CreateTextUI(L"", Vec2(-100.f, -100.f), Vec2(100.f, 100.f)
		, 24, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR, TextUIMode::TEXT, 30);
	harvestingText->SetName(L"HarvestingText");
	harvestingText->SetObjType(GROUP_TYPE::UI);
	AddObject(harvestingText, GROUP_TYPE::UI);
	////////////////////////////////////////////////////////////////////////

	////////////////////////////////외부 벽/////////////////////////////////
	CObject* leftWall = new CGround;
	leftWall->SetPos(Vec2(16.f, 576.f));
	leftWall->SetScale(Vec2(32.f, 1152.f));
	AddObject(leftWall, GROUP_TYPE::GROUND);

	CObject* rightWall = new CGround;
	rightWall->SetPos(Vec2(1152.f - 16.f, 576.f));
	rightWall->SetScale(Vec2(32.f, 1152.f));
	AddObject(rightWall, GROUP_TYPE::GROUND);

	CObject* topWall = new CGround;
	topWall->SetPos(Vec2(576.f, 16.f));
	topWall->SetScale(Vec2(1152.f - 64.f, 32.f));
	AddObject(topWall, GROUP_TYPE::GROUND);

	CObject* bottomWall = new CGround;
	bottomWall->SetPos(Vec2(576.f, 1152.f - 16.f));
	bottomWall->SetScale(Vec2(1152.f - 64.f, 32.f));
	AddObject(bottomWall, GROUP_TYPE::GROUND);
	////////////////////////////////외부 벽/////////////////////////////////
	

	CCamera::GetInstance()->SetTarget(static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer()));

	//이제 새로운 충돌이 발생할수도 있음. 
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::WEAPON, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PROJ_PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::GROUND, GROUP_TYPE::PLAYER);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::DROP_ITEM, GROUP_TYPE::PLAYER);
	
	//Camera Look 지점.
	CCamera::GetInstance()->SetLookAt(vResolution / 2.f);

	CWaveMgr::GetInstance()->WaveStart();
	m_fFailDuration = 0.f;
	m_bFailed = false;
	m_vecPauseObj.clear();
	m_vecFailObj.clear();
	CScene* tempScene = CSceneMgr::GetInstance()->GetCurScene();
	//웨이브에 들어오면 플레이어의 무기를 Scene에 추가. 
	static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->PushSceneWeapons();
	start();
}

void CScene_Start::Exit()
{
	

	//나갈때 전부 삭제해줘야함.
	if (GetPause() || m_bFailed) {
		CTimeMgr::GetInstance()->SetTimeScale(1.f);
		m_bFailed = false;
	}
	DeleteAll();

	m_pPausePanel = nullptr;
	m_pFailPanel = nullptr;
	//충돌도 전부 초기화 해주기. 
	CCollisionMgr::GetInstance()->Reset();

	//웨이브가 멈춤. 
	CWaveMgr::GetInstance()->WaveStart();

	//사운드 매니저 포인터 초기화. 
	CSoundMgr::GetInstance()->InitVolumePointer();
	m_vecPauseObj.clear();
	m_vecFailObj.clear();
	m_vecOptionObjs.clear();
	m_scrollContent.clear();
}

void CScene_Start::CreateForce()
{
	m_vForcePos = CCamera::GetInstance()->GetRealPos(MOUSE_POS);
}


void CScene_Start::CreatePause()
{
	CreateLeftBtns();
	CreateMiddleInfo();
	CreateInfoPanel();
}

void CScene_Start::OnPause()
{
	CreatePause();
	ChangePause(true);

}

void CScene_Start::OffPause()
{
	//Safe_Delete_Vec(m_vecPauseObj);

	for (auto* pauseObj : m_vecPauseObj) {
		DeleteObject(pauseObj);
	}
	CUIMgr::GetInstance()->SetFocusedUI(nullptr);
	m_vecPauseObj.clear();
	ChangePause(false);
}

void CScene_Start::CreateLeftBtns()
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	CWaveMgr* waveMgr = CWaveMgr::GetInstance();

	//////////////////뒷 판떼기///////////////////////
	m_pPausePanel = new CPanelUI;
	m_pPausePanel->SetObjType(GROUP_TYPE::IMAGE);
	m_pPausePanel->SetPos(vResolution / 2.f);
	m_pPausePanel->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));
	m_pPausePanel->SetNormalAlpha(0.7f);
	m_pPausePanel->SetMouseOnAlpha(0.7f);
	m_pPausePanel->SetScale(vResolution);
	m_vecPauseObj.push_back(m_pPausePanel);
	AddObject(m_pPausePanel, GROUP_TYPE::IMAGE);
	//////////////////뒷 판떼기///////////////////////


	////////////좌 상단 텍스트 (웨이브 %d)////////////////////
	int wave = waveMgr->GetLevel();

	CObject* panelTextRunSuccessOrFail = new CSpriteUI;
	panelTextRunSuccessOrFail->SetObjType(GROUP_TYPE::UI);
	panelTextRunSuccessOrFail->SetPos(Vec2(175.f, 170.f));
	panelTextRunSuccessOrFail->SetScale(Vec2(196.f, 26.f));

	wchar_t buffer[20];
	swprintf_s(buffer, L"웨이브 %d", wave);

	panelTextRunSuccessOrFail->CreateTextUI(buffer, -(panelTextRunSuccessOrFail->GetScale() / 2.f), (panelTextRunSuccessOrFail->GetScale() / 2.f)
		, 14, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	m_vecPauseObj.push_back(panelTextRunSuccessOrFail);
	AddObject(panelTextRunSuccessOrFail, GROUP_TYPE::UI);
	////////////상단 실패or성공 - 격돌지대 text ui////////////////////


	/////////////////재개 버튼//////////////////////
	CBtnUI* returnToGameBtn = new CBtnUI;
	returnToGameBtn->SetName(L"ReturnToGame");
	returnToGameBtn->SetObjType(GROUP_TYPE::UI);
	returnToGameBtn->SetScale(Vec2(302.f, 37.f));
	returnToGameBtn->SetIsRound(true, 10.f, 10.f);
	//returnToGameBtn->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(0, 0, 0));
	returnToGameBtn->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Start::OffPause);
	//returnToGameBtn->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::MAIN, 0);
	returnToGameBtn->CreateTextUI(L"재개", -(returnToGameBtn->GetScale() / 2.f), (returnToGameBtn->GetScale() / 2.f)
		, 20, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	returnToGameBtn->SetPos(Vec2(175.f, 206.f));
	m_vecPauseObj.push_back(returnToGameBtn);
	AddObject(returnToGameBtn, GROUP_TYPE::UI);
	/////////////////재개 버튼//////////////////////

	/////////////////재시작 버튼//////////////////////
	CBtnUI* reStartGameBtn = new CBtnUI;
	reStartGameBtn->SetName(L"ReStartGame");
	reStartGameBtn->SetObjType(GROUP_TYPE::UI);
	reStartGameBtn->SetScale(Vec2(302.f, 37.f));
	reStartGameBtn->SetIsRound(true, 10.f, 10.f);
	reStartGameBtn->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(0, 0, 0));
	reStartGameBtn->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::START, 0);
	reStartGameBtn->CreateTextUI(L"재시작", -(reStartGameBtn->GetScale() / 2.f), (reStartGameBtn->GetScale() / 2.f)
		, 20, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	reStartGameBtn->SetPos(Vec2(175.f, 256.f));
	m_vecPauseObj.push_back(reStartGameBtn);
	AddObject(reStartGameBtn, GROUP_TYPE::UI);
	/////////////////재시작 버튼//////////////////////


	/////////////////달리기 종료 버튼//////////////////////
	CBtnUI* retireGameBtn = new CBtnUI;
	retireGameBtn->SetName(L"retireGame");
	retireGameBtn->SetObjType(GROUP_TYPE::UI);
	retireGameBtn->SetScale(Vec2(302.f, 37.f));
	retireGameBtn->SetIsRound(true, 10.f, 10.f);
	retireGameBtn->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(0, 0, 0));
	retireGameBtn->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::RUN_END, 0);
	retireGameBtn->CreateTextUI(L"달리기를 종료합니다", -(retireGameBtn->GetScale() / 2.f), (retireGameBtn->GetScale() / 2.f)
		, 20, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	retireGameBtn->SetPos(Vec2(175.f, 306.f));
	m_vecPauseObj.push_back(retireGameBtn);
	AddObject(retireGameBtn, GROUP_TYPE::UI);
	/////////////////달리기 종료 버튼//////////////////////

	/////////////////옵션 버튼//////////////////////
	CBtnUI* optionBtn = new CBtnUI;
	optionBtn->SetName(L"optionBtn");
	optionBtn->SetObjType(GROUP_TYPE::UI);
	optionBtn->SetScale(Vec2(302.f, 37.f));
	optionBtn->SetIsRound(true, 10.f, 10.f);
	optionBtn->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(0, 0, 0));
	//optionBtn->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::MAIN, 0);
	//optionBtn->SetClickedCallBack(this, )
	optionBtn->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Start::CreateOptionPanel);
	optionBtn->CreateTextUI(L"옵션", -(optionBtn->GetScale() / 2.f), (optionBtn->GetScale() / 2.f)
		, 20, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	optionBtn->SetPos(Vec2(175.f, 356.f));
	m_vecPauseObj.push_back(optionBtn);
	AddObject(optionBtn, GROUP_TYPE::UI);
	/////////////////옵션 버튼//////////////////////

	/////////////////메인 메뉴로 돌아가기//////////////////////
	CBtnUI* backtoMainBtn = new CBtnUI;
	backtoMainBtn->SetName(L"optionBtn");
	backtoMainBtn->SetObjType(GROUP_TYPE::UI);
	backtoMainBtn->SetScale(Vec2(302.f, 37.f));
	backtoMainBtn->SetIsRound(true, 10.f, 10.f);
	backtoMainBtn->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(0, 0, 0));
	backtoMainBtn->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::MAIN, 0);
	backtoMainBtn->CreateTextUI(L"메인 메뉴로 돌아가기", -(backtoMainBtn->GetScale() / 2.f), (backtoMainBtn->GetScale() / 2.f)
		, 20, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	backtoMainBtn->SetPos(Vec2(175.f, 406.f));
	m_vecPauseObj.push_back(backtoMainBtn);
	AddObject(backtoMainBtn, GROUP_TYPE::UI);
	/////////////////메인 메뉴로 돌아가기//////////////////////
}

void CScene_Start::CreateMiddleInfo()
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	CWaveMgr* waveMgr = CWaveMgr::GetInstance();
	wchar_t buffer[20];

	CPanelUI* panelWeaponInfo = new CPanelUI;
	panelWeaponInfo->SetName(L"WeaponInfoPanel");
	panelWeaponInfo->SetPos(Vec2(542.f,200.f));
	panelWeaponInfo->SetScale(Vec2(376.f,96.f));
	panelWeaponInfo->SetMouseOnAlpha(0.f);
	panelWeaponInfo->SetNormalAlpha(0.f);
	m_vecPauseObj.push_back(panelWeaponInfo);
	
	////////////중앙 무기 텍스트 보여주는 곳////////////////////
	CSpriteUI* weaponCountText = panelWeaponInfo->AddChild<CSpriteUI>
		(Vec2(-panelWeaponInfo->GetScale().x/2.f + 50.f, -panelWeaponInfo->GetScale().y/2.f + 17.f));

	weaponCountText->SetName(L"weaponCountText");
	weaponCountText->SetObjType(GROUP_TYPE::UI);
	weaponCountText->SetScale(Vec2(100.f, 34.f));
	swprintf_s(buffer, L"무기(%zd/6)", static_cast<CPlayer*>((CSceneMgr::GetInstance()->GetCurScene()->GetPlayer()))->GetWeaponCount());
	weaponCountText->CreateTextUI(buffer, -(weaponCountText->GetScale() / 2.f), (weaponCountText->GetScale() / 2.f)
		, 20, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	weaponCountText->GetTextUI()->SetHorizontal(1);
	////////////중앙 무기 텍스트 보여주는 곳////////////////////

	////////////중앙 무기 출력///////////////////////////////////
	const list<CWeapon*>& vPlayerWeaponList = static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->GetPlayerWeapons();
	CPlayer* tmp = static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer());
	int curWeaponCnt = (int)((CPlayer*)CSceneMgr::GetInstance()->GetPlayer())->GetWeaponCount();

	CPanelUI* weaponImages = panelWeaponInfo->AddChild<CPanelUI>(Vec2(0.f, panelWeaponInfo->GetScale().y / 2.f - 30.f - 7.f));
	weaponImages->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));
	weaponImages->SetScale(Vec2(376.f, 60.f));
	weaponImages->SetMouseOnAlpha(0.f);
	weaponImages->SetNormalAlpha(0.f);
	
	int temp = 0;
	for (auto iter = vPlayerWeaponList.begin(); iter != vPlayerWeaponList.end(); iter++, temp++)
	{
		CWeapon* weapon = *iter;
		wstring iconTag = weapon->Getinfo().m_sIconImageKey;

		CSpriteUI* weapons = weaponImages->AddChild<CSpriteUI>(Vec2(-155.f + temp * 55.f, 0.f));
		weapons->SetName(L"Weapon");
		weapons->AddImage(pD2DMgr->GetStoredBitmap(iconTag));
		weapons->SetObjType(GROUP_TYPE::UI);
		weapons->SetScale(Vec2(50.f, 50.f));
		weapons->SetBackGround(true);
		weapons->SetBackGroundColor(ColorNormalize(30, 30, 30), ColorNormalize(0, 0, 0)
			, ColorNormalize(30, 30, 30), ColorNormalize(0, 0, 0));
		weapons->SetIsRound(true, 10.f, 10.f);
	}
	////////////중앙 무기 출력///////////////////////////////////

	CPanelUI* panelItemInfo = new CPanelUI;
	panelItemInfo->SetName(L"ItemInfoPanel");
	panelItemInfo->SetPos(Vec2(542.f,400.f));
	panelItemInfo->SetScale(Vec2(376.f,260.f));
	m_vecPauseObj.push_back(panelItemInfo);
	panelItemInfo->SetMouseOnAlpha(0.f);
	panelItemInfo->SetNormalAlpha(0.f);

	////////////중앙 아이템 텍스트 보여주는 곳////////////////////
	CSpriteUI* itemCountText = panelItemInfo->AddChild<CSpriteUI>
		(Vec2(-panelItemInfo->GetScale().x / 2.f + 60.f, -panelItemInfo->GetScale().y / 2.f + 17.f));
	itemCountText->SetName(L"itemCountText");
	itemCountText->SetObjType(GROUP_TYPE::UI);
	itemCountText->SetScale(Vec2(120.f, 34.f));
	swprintf_s(buffer, L"아이템(%zd)", ItemMgr::GetInstance()->GetPassiveItemssize());
	itemCountText->CreateTextUI(buffer, -(itemCountText->GetScale() / 2.f), (itemCountText->GetScale() / 2.f)
		, 20, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	itemCountText->GetTextUI()->SetHorizontal(1);
	////////////중앙 아이템 텍스트 보여주는 곳////////////////////

	//중앙 스크롤 영역///
	CreateScrollArea();
	//중앙 스크롤 영역///
	
	AddObject(panelWeaponInfo, GROUP_TYPE::UI);
	AddObject(panelItemInfo, GROUP_TYPE::UI);
}

void CScene_Start::CreateScrollArea()
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	// 스크롤 영역 설정
	m_scrollArea.viewRect = D2D1::RectF(354.f, 310.f, 730.f, 530.f);
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

		int xCount = i % 6;
		int yCount = i / 6;

		possessedItem->SetPos(Vec2(35.f + xCount * 55.f, 27.f + yCount * 55.f));
		possessedItem->SetScale(Vec2(50.f, 50.f));

		possessedItem->SetBackGround(true);
		possessedItem->SetBackGroundColor(ColorNormalize(30, 30, 30), ColorNormalize(0, 0, 0)
			, ColorNormalize(30, 30, 30), ColorNormalize(0, 0, 0));
		possessedItem->SetIsRound(true, 10.f, 10.f);

		m_scrollContent.push_back(possessedItem);
	}
}
void CScene_Start::UpdateScrollPosition(float deltaY)
{
	m_scrollArea.scrollPos.y += deltaY;
	m_scrollArea.scrollPos.y = max(0.f, min(m_scrollArea.scrollPos.y,
		m_scrollArea.contentRect.bottom - (m_scrollArea.viewRect.bottom - m_scrollArea.viewRect.top)));
}
void CScene_Start::RenderScrollArea(ID2D1HwndRenderTarget* _pRender)
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


void CScene_Start::CreateInfoPanel()
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	CWaveMgr* waveMgr = CWaveMgr::GetInstance();
	wchar_t buffer[20];
	//플레이어 Info 출력. 
	const playerParameter playerInfo = static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetCurScene()->GetPlayer())->GetPlayerInfo();

	//////////////////우측 능력치 보여주는 곳///////////////////////
	CPanelUI* parameterPanel = new CPanelUI;
	parameterPanel->SetName(L"parameterPanel");
	parameterPanel->SetObjType(GROUP_TYPE::IMAGE);
	parameterPanel->SetPos(Vec2(840.f, vResolution.y / 2));
	parameterPanel->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));
	parameterPanel->SetNormalAlpha(0.5f);
	parameterPanel->SetMouseOnAlpha(0.5f);
	parameterPanel->SetScale(Vec2(200.f, 300.f));
	m_vecPauseObj.push_back(parameterPanel);
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
	m_vecPauseObj.push_back(abilityText);
	AddObject(abilityText, GROUP_TYPE::UI);
	////////////우측 능력치 보여주는 곳////////////////////

	////////////현재 레벨 ////////////////////
	CObject* nowLevelIcon = new CSpriteUI;
	nowLevelIcon->SetName(L"nowLevelIcon");
	nowLevelIcon->SetObjType(GROUP_TYPE::UI);
	nowLevelIcon->AddImage(pD2DMgr->GetStoredBitmap(L"brotato_icon"));
	nowLevelIcon->SetPos(Vec2(760.f, 170.f));
	nowLevelIcon->SetScale(Vec2(15.f, 15.f));
	m_vecPauseObj.push_back(nowLevelIcon);
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
	m_vecPauseObj.push_back(nowLevelText);
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
	m_vecPauseObj.push_back(nowLevelCount);
	AddObject(nowLevelCount, GROUP_TYPE::UI);
	////////////현재 레벨 ////////////////////

	////////////최대 HP ////////////////////
	CObject* maxHPIcon = new CSpriteUI;
	maxHPIcon->SetName(L"maxHPIcon");
	maxHPIcon->SetObjType(GROUP_TYPE::UI);
	maxHPIcon->AddImage(pD2DMgr->GetStoredBitmap(L"max_hp"));
	maxHPIcon->SetPos(Vec2(760.f, 200.f));
	maxHPIcon->SetScale(Vec2(15.f, 15.f));
	m_vecPauseObj.push_back(maxHPIcon);
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
	m_vecPauseObj.push_back(maxHPText);
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
	m_vecPauseObj.push_back(maxHPCount);
	AddObject(maxHPCount, GROUP_TYPE::UI);
	////////////최대 HP ////////////////////

	////////////데미지 % ////////////////////
	CObject* FinalDMGIcon = new CSpriteUI;
	FinalDMGIcon->SetName(L"FinalDMGIcon");
	FinalDMGIcon->SetObjType(GROUP_TYPE::UI);
	FinalDMGIcon->AddImage(pD2DMgr->GetStoredBitmap(L"percent_damage"));
	FinalDMGIcon->SetPos(Vec2(760.f, 230.f));
	FinalDMGIcon->SetScale(Vec2(15.f, 15.f));
	m_vecPauseObj.push_back(FinalDMGIcon);
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
	m_vecPauseObj.push_back(FinalDMGText);
	AddObject(FinalDMGText, GROUP_TYPE::UI);

	//

	CObject* FinalDMGCount = new CSpriteUI;
	FinalDMGCount->SetName(L"FinalDMGCount");
	FinalDMGCount->SetObjType(GROUP_TYPE::UI);
	//740 + Alpha
	FinalDMGCount->SetPos(Vec2(920.f, 230.f));
	FinalDMGCount->SetScale(Vec2(35.f, 35.f));
	swprintf_s(buffer, L"%d", static_cast<int>(playerInfo.m_fDamageCoef * 100.f));


	FinalDMGCount->CreateTextUI(buffer, -(FinalDMGCount->GetScale() / 2.f), (FinalDMGCount->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	m_vecPauseObj.push_back(FinalDMGCount);
	AddObject(FinalDMGCount, GROUP_TYPE::UI);
	////////////데미지 % ////////////////////


	////////////근거리 데미지 % ////////////////////
	CObject* MeleeDMGIcon = new CSpriteUI;
	MeleeDMGIcon->SetName(L"MeleeDMGIcon");
	MeleeDMGIcon->SetObjType(GROUP_TYPE::UI);
	MeleeDMGIcon->AddImage(pD2DMgr->GetStoredBitmap(L"melee_damage"));
	MeleeDMGIcon->SetPos(Vec2(760.f, 260.f));
	MeleeDMGIcon->SetScale(Vec2(15.f, 15.f));
	m_vecPauseObj.push_back(MeleeDMGIcon);
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
	m_vecPauseObj.push_back(MeleeText);
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
	m_vecPauseObj.push_back(MeleeCount);
	AddObject(MeleeCount, GROUP_TYPE::UI);
	////////////근거리 데미지 % ////////////////////


	////////////원거리 데미지 % ////////////////////
	CObject* rangedDMGIcon = new CSpriteUI;
	rangedDMGIcon->SetName(L"rangedDMGIcon");
	rangedDMGIcon->SetObjType(GROUP_TYPE::UI);
	rangedDMGIcon->AddImage(pD2DMgr->GetStoredBitmap(L"ranged_damage"));
	rangedDMGIcon->SetPos(Vec2(760.f, 290.f));
	rangedDMGIcon->SetScale(Vec2(15.f, 15.f));
	m_vecPauseObj.push_back(rangedDMGIcon);
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
	m_vecPauseObj.push_back(rangedText);
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
	m_vecPauseObj.push_back(rangedCount);
	AddObject(rangedCount, GROUP_TYPE::UI);
	////////////원거리 데미지 % ////////////////////


	////////////공격속도  % ////////////////////
	CObject* attackSpeedIcon = new CSpriteUI;
	attackSpeedIcon->SetName(L"attackSpeedIcon");
	attackSpeedIcon->SetObjType(GROUP_TYPE::UI);
	attackSpeedIcon->AddImage(pD2DMgr->GetStoredBitmap(L"attack_speed"));
	attackSpeedIcon->SetPos(Vec2(760.f, 320.f));
	attackSpeedIcon->SetScale(Vec2(15.f, 15.f));
	m_vecPauseObj.push_back(attackSpeedIcon);
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
	m_vecPauseObj.push_back(attackSpeedText);
	AddObject(attackSpeedText, GROUP_TYPE::UI);

	//

	CObject* attackSpeedCount = new CSpriteUI;
	attackSpeedCount->SetName(L"attackSpeedCount");
	attackSpeedCount->SetObjType(GROUP_TYPE::UI);
	//740 + Alpha
	attackSpeedCount->SetPos(Vec2(920.f, 320.f));
	attackSpeedCount->SetScale(Vec2(35.f, 35.f));
	swprintf_s(buffer, L"%d", static_cast<int>(playerInfo.m_fAttackSpeedCoef * 100.f));


	attackSpeedCount->CreateTextUI(buffer, -(attackSpeedCount->GetScale() / 2.f), (attackSpeedCount->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	m_vecPauseObj.push_back(attackSpeedCount);
	AddObject(attackSpeedCount, GROUP_TYPE::UI);
	////////////공격속도 % ////////////////////


	////////////치명타율  % ////////////////////
	CObject* critChanceIcon = new CSpriteUI;
	critChanceIcon->SetName(L"critChanceIcon");
	critChanceIcon->SetObjType(GROUP_TYPE::UI);
	critChanceIcon->AddImage(pD2DMgr->GetStoredBitmap(L"crit_chance"));
	critChanceIcon->SetPos(Vec2(760.f, 350.f));
	critChanceIcon->SetScale(Vec2(15.f, 15.f));
	m_vecPauseObj.push_back(critChanceIcon);
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
	m_vecPauseObj.push_back(critChanceText);
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
	m_vecPauseObj.push_back(critChanceCount);
	AddObject(critChanceCount, GROUP_TYPE::UI);
	////////////치명타율 % ////////////////////

	////////////속도  % ////////////////////
	CObject* speedIcon = new CSpriteUI;
	speedIcon->SetName(L"speedIcon");
	speedIcon->SetObjType(GROUP_TYPE::UI);
	speedIcon->AddImage(pD2DMgr->GetStoredBitmap(L"speed"));
	speedIcon->SetPos(Vec2(760.f, 380.f));
	speedIcon->SetScale(Vec2(15.f, 15.f));
	m_vecPauseObj.push_back(speedIcon);
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
	m_vecPauseObj.push_back(speedText);
	AddObject(speedText, GROUP_TYPE::UI);

	//

	CObject* speedCount = new CSpriteUI;
	speedCount->SetName(L"speedCount");
	speedCount->SetObjType(GROUP_TYPE::UI);
	//740 + Alpha
	speedCount->SetPos(Vec2(920.f, 380.f));
	speedCount->SetScale(Vec2(35.f, 35.f));
	swprintf_s(buffer, L"%d", static_cast<int>(playerInfo.m_fSpeed * 100.f));


	speedCount->CreateTextUI(buffer, -(speedCount->GetScale() / 2.f), (speedCount->GetScale() / 2.f)
		, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	m_vecPauseObj.push_back(speedCount);
	AddObject(speedCount, GROUP_TYPE::UI);
	////////////치명타율 % ////////////////////
}

void CScene_Start::CreateLevelUpShop()
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	CWaveMgr* waveMgr = CWaveMgr::GetInstance();

	CTimeMgr::GetInstance()->SetTimeScale(0.f);


	//////////////////뒷 판떼기///////////////////////
	CPanelUI* LevelUpPanel = new CPanelUI;
	LevelUpPanel->SetObjType(GROUP_TYPE::IMAGE);
	LevelUpPanel->SetPos(vResolution / 2.f);
	LevelUpPanel->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));
	LevelUpPanel->SetNormalAlpha(0.5f);
	LevelUpPanel->SetMouseOnAlpha(0.5f);
	LevelUpPanel->SetScale(vResolution);
	AddObject(LevelUpPanel, GROUP_TYPE::IMAGE);
	//////////////////뒷 판떼기///////////////////////

	////////////레벨 업 텍스트////////////////////
	CObject* levelUpText = new CSpriteUI;
	levelUpText->SetName(L"levelUpText");
	levelUpText->SetObjType(GROUP_TYPE::UI);
	levelUpText->SetPos(Vec2(vResolution.x / 2, 150.f));
	levelUpText->SetScale(Vec2(350.f, 75.f));


	levelUpText->CreateTextUI(L"레벨 업!", -(levelUpText->GetScale() / 2.f), (levelUpText->GetScale() / 2.f)
		, 38, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(levelUpText, GROUP_TYPE::UI);
	////////////레벨 업 텍스트////////////////////

	////////////////////////가운데 아이템 패널 4개////////////////////////////////
	vector<int> upgrade_numbers;

	while (upgrade_numbers.size() < 4) {
		int randomRC = rand() % upgrade_tag_list.size();
		auto it = find(upgrade_numbers.begin(), upgrade_numbers.end(), randomRC);
		if (it == upgrade_numbers.end()) {
			upgrade_numbers.push_back(randomRC);
		}
	}

	for (int upgradeIndex = 0; upgradeIndex < 4; upgradeIndex++)
	{
		CPanelUI* panelItemUI = new CPanelUI;
		panelItemUI->SetObjType(GROUP_TYPE::IMAGE);
		panelItemUI->SetName(L"이거다");
		panelItemUI->SetScale(Vec2(176.f, 132.f));
		panelItemUI->SetPos(Vec2(100.f + upgradeIndex * (5.f + panelItemUI->GetScale().x), vResolution.y / 2));
		panelItemUI->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));


		////////////////////////Upgrade 능력치 아이콘////////////////////////////////
		wstring iconTag = upgrade_tag_list[upgrade_numbers[upgradeIndex]];
		Vec2 vPos = Vec2(35.f, 35.f) - (panelItemUI->GetScale() / 2.f);
		CSpriteUI* itemImage = panelItemUI->AddChild<CSpriteUI>(vPos);
		itemImage->SetName(L"ICON");
		itemImage->AddImage(pD2DMgr->GetStoredBitmap(iconTag));
		itemImage->SetObjType(GROUP_TYPE::UI);
		itemImage->SetName(L"Child");
		itemImage->SetScale(Vec2(48.f, 48.f));
		////////////////////////Upgrade 능력치 아이콘////////////////////////////////

		////////////Upgrade Name////////////////////
		CObject* upgradeNameText = panelItemUI->AddChild<CSpriteUI>(Vec2(20.f, -50.f));
		upgradeNameText->SetName(L"upgradeNameText");
		upgradeNameText->SetObjType(GROUP_TYPE::UI);
		upgradeNameText->SetScale(Vec2(80.f, 15.f));

		upgradeNameText->CreateTextUI(upgrade_name_list[upgrade_numbers[upgradeIndex]], -(upgradeNameText->GetScale() / 2.f), (upgradeNameText->GetScale() / 2.f)
			, 16, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
			, FONT_TYPE::KR
			, TextUIMode::TEXT
			, 0);
		upgradeNameText->GetTextUI()->SetHorizontal(1);
		////////////Upgrade Name////////////////////


		////////////Upgrade Classifi////////////////////
		CObject* upgradeClassifi = panelItemUI->AddChild<CSpriteUI>(Vec2(20.f, -25.f));
		upgradeClassifi->SetName(L"upgradeClassifi");
		upgradeClassifi->SetObjType(GROUP_TYPE::UI);
		upgradeClassifi->SetScale(Vec2(80.f, 15.f));

		upgradeClassifi->CreateTextUI(upgrade_classifi, -(upgradeClassifi->GetScale() / 2.f), (upgradeClassifi->GetScale() / 2.f)
			, 12, D2D1::ColorF::Yellow, true, 1.f, D2D1::ColorF::Black
			, FONT_TYPE::KR
			, TextUIMode::TEXT
			, 0);
		upgradeClassifi->GetTextUI()->SetHorizontal(1);
		////////////Upgrade Classifi////////////////////

		////////////Upgrade Text////////////////////
		//CObject* upgradeText = new CSpriteUI;
		CObject* upgradeText = panelItemUI->AddChild<CSpriteUI>(Vec2(15.f, 5.f));
		upgradeText->SetName(L"upgradeText");
		upgradeText->SetObjType(GROUP_TYPE::UI);
		//upgradeText->SetPos(Vec2(115.f + upgradeIndex * (5.f + panelItemUI->GetScale().x), (vResolution.y / 2) + 5.f));
		upgradeText->SetScale(Vec2(176.f, 15.f));

		upgradeText->CreateTextUI(upgrade_text_list[upgrade_numbers[upgradeIndex]], -(upgradeText->GetScale() / 2.f), (upgradeText->GetScale() / 2.f)
			, 11, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
			, FONT_TYPE::KR
			, TextUIMode::TEXT
			, 0);
		upgradeText->GetTextUI()->SetHorizontal(1);
		////////////Upgrade Name////////////////////

		AddObject(panelItemUI, GROUP_TYPE::IMAGE);

		/////////////////선택 버튼//////////////////////
		CBtnUI* selectBtn = new CBtnUI;
		selectBtn->SetName(L"selectBtn");
		selectBtn->SetObjType(GROUP_TYPE::UI);
		selectBtn->SetScale(Vec2(156.f, 30.f));
		selectBtn->SetPos(Vec2(100.f + upgradeIndex * (5.f + panelItemUI->GetScale().x), vResolution.y / 2 + 40.f));
		selectBtn->SetIsRound(true, 5.f, 5.f);
		selectBtn->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(36, 36, 36));

		//콜백 함수로 구현. 
		int callbackParam = upgrade_numbers[upgradeIndex];
		selectBtn->SetClickedCallBack([this, callbackParam]() {
			this->callPlayerUpgrade(callbackParam);
		});
		
		selectBtn->CreateTextUI(L"선택", -(selectBtn->GetScale() / 2.f), (selectBtn->GetScale() / 2.f)
			, 20, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
			, FONT_TYPE::KR
			, TextUIMode::TEXT
			, 0);


		//버튼 기능 추가.
		AddObject(selectBtn, GROUP_TYPE::UI);
		/////////////////선택 버튼//////////////////////
	}

	////////////////////////가운데 아이템 패널 4개////////////////////////////////


	CreateInfoPanel();
}


void CScene_Start::SceneFailed()
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	CWaveMgr* waveMgr = CWaveMgr::GetInstance();

	m_bFailed = true;
	CTimeMgr::GetInstance()->SetTimeScale(0.f);

	//////////////////뒷 판떼기///////////////////////
	m_pFailPanel = new CPanelUI;
	m_pFailPanel->SetObjType(GROUP_TYPE::IMAGE);
	m_pFailPanel->SetPos(vResolution / 2.f);
	m_pFailPanel->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));
	m_pFailPanel->SetNormalAlpha(0.0f);
	m_pFailPanel->SetMouseOnAlpha(0.0f);
	m_pFailPanel->SetScale(vResolution);
	m_vecFailObj.push_back(m_pFailPanel);
	AddObject(m_pFailPanel, GROUP_TYPE::IMAGE);
	//////////////////뒷 판떼기///////////////////////


	////////////중앙 달리기 패배////////////////////
	CObject* runFailed = new CSpriteUI;
	runFailed->SetName(L"runFailed");
	runFailed->SetObjType(GROUP_TYPE::UI);
	runFailed->SetPos(Vec2(vResolution.x / 2, 100.f));
	runFailed->SetScale(Vec2(350.f, 75.f));

	runFailed->CreateTextUI(L"달리기 패배", -(runFailed->GetScale() / 2.f), (runFailed->GetScale() / 2.f)
		, 56, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	m_vecFailObj.push_back(runFailed);
	AddObject(runFailed, GROUP_TYPE::UI);
	////////////중앙 달리기 패배////////////////////


}

void CScene_Start::callPlayerUpgrade(int upgradeIdx)
{
	static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->upgradeParameter(upgradeIdx);
	ChangeScene(SCENE_TYPE::SHOP);
}




void CScene_Start::CreateOptionPanel()
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	CWaveMgr* waveMgr = CWaveMgr::GetInstance();

	//////////////////뒷 판떼기///////////////////////
	CPanelUI* optionPanel = new CPanelUI;
	optionPanel->SetObjType(GROUP_TYPE::IMAGE);
	optionPanel->SetPos(vResolution / 2.f);
	optionPanel->SetScale(vResolution);
	optionPanel->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));
	optionPanel->SetNormalAlpha(0.6f);
	optionPanel->SetMouseOnAlpha(0.6f);
	m_vecOptionObjs.push_back(optionPanel);
	AddObject(optionPanel, GROUP_TYPE::IMAGE);
	//////////////////뒷 판떼기///////////////////////

	////////////음향 텍스트 보여주는 곳////////////////////
	CObject* soundText = new CSpriteUI;
	soundText->SetName(L"soundText");
	soundText->SetObjType(GROUP_TYPE::IMAGE);
	soundText->SetPos(Vec2(vResolution.x / 2, vResolution.y / 2 - 150.f));
	soundText->SetScale(Vec2(196.f, 38.f));


	soundText->CreateTextUI(L"음향", -(soundText->GetScale() / 2.f), (soundText->GetScale() / 2.f)
		, 38, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	m_vecOptionObjs.push_back(soundText);
	AddObject(soundText, GROUP_TYPE::UI);
	////////////음향 텍스트 보여주는 곳////////////////////


	////////////마스터 텍스트 보여주는 곳////////////////////
	CObject* masterSoundText = new CSpriteUI;
	masterSoundText->SetName(L"masterSoundText");
	masterSoundText->SetObjType(GROUP_TYPE::IMAGE);
	masterSoundText->SetPos(Vec2(vResolution.x / 2 - 100.f, vResolution.y / 2 - 100.f));
	masterSoundText->SetScale(Vec2(196.f, 24.f));


	masterSoundText->CreateTextUI(L"마스터", -(masterSoundText->GetScale() / 2.f), (masterSoundText->GetScale() / 2.f)
		, 24, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	masterSoundText->GetTextUI()->SetHorizontal(1);
	m_vecOptionObjs.push_back(masterSoundText);
	AddObject(masterSoundText, GROUP_TYPE::UI);
	////////////마스터 텍스트 보여주는 곳////////////////////


	////////////마스터 슬라이더////////////////////
	CSliderUI* masterSoundSlider = new CSliderUI;
	masterSoundSlider->SetName(L"masterSoundSlider");
	masterSoundSlider->AddImage(pD2DMgr->GetStoredBitmap(L"ui_lifebar_bg"));
	masterSoundSlider->AddImage(pD2DMgr->GetStoredBitmap(L"ui_soundbar_fill"));
	masterSoundSlider->AddImage(pD2DMgr->GetStoredBitmap(L"ui_lifebar_frame"));
	masterSoundSlider->SetObjType(GROUP_TYPE::UI);
	masterSoundSlider->SetPos(Vec2(vResolution.x / 2, vResolution.y / 2 - 100.f));
	masterSoundSlider->SetScale(Vec2(182.f, 33.f));
	masterSoundSlider->SetSliderValue(CSoundMgr::GetInstance()->m_fMasterRatio);
	CSoundMgr::GetInstance()->m_pMasterSoundSlider = masterSoundSlider;
	masterSoundSlider->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Start::ChangeMasterRatio);
	m_vecOptionObjs.push_back(masterSoundSlider);
	AddObject(masterSoundSlider, GROUP_TYPE::UI);
	////////////마스터 슬라이더////////////////////



	////////////마스터 Ratio 보여주는 곳////////////////////
	CObject* masterSoundRatio = new CSpriteUI;
	masterSoundRatio->SetName(L"masterSoundRatio");
	masterSoundRatio->SetObjType(GROUP_TYPE::IMAGE);
	masterSoundRatio->SetPos(Vec2(vResolution.x / 2 + 150.f, vResolution.y / 2 - 100.f));
	masterSoundRatio->SetScale(Vec2(96.f, 24.f));

	int RatioView = static_cast<int>(CSoundMgr::GetInstance()->m_fMasterRatio * 100.f);
	//static_cast<int>(masterSoundSlider->GetSliderValue() * 100.f);
	wchar_t buffer[20];
	swprintf_s(buffer, L"%d%%", RatioView);
	masterSoundRatio->CreateTextUI(buffer, -(masterSoundRatio->GetScale() / 2.f), (masterSoundRatio->GetScale() / 2.f)
		, 24, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	masterSoundRatio->GetTextUI()->SetHorizontal(2);
	CSoundMgr::GetInstance()->m_pMasterSoundRatio = masterSoundRatio;
	m_vecOptionObjs.push_back(masterSoundRatio);
	AddObject(masterSoundRatio, GROUP_TYPE::UI);
	////////////마스터 Ratio 보여주는 곳////////////////////

	////////////음악 텍스트 보여주는 곳////////////////////
	CObject* BGMSoundText = new CSpriteUI;
	BGMSoundText->SetName(L"BGMSoundText");
	BGMSoundText->SetObjType(GROUP_TYPE::IMAGE);
	BGMSoundText->SetPos(Vec2(vResolution.x / 2 - 100.f, vResolution.y / 2 - 50.f));
	BGMSoundText->SetScale(Vec2(196.f, 24.f));


	BGMSoundText->CreateTextUI(L"음악", -(BGMSoundText->GetScale() / 2.f), (BGMSoundText->GetScale() / 2.f)
		, 24, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	BGMSoundText->GetTextUI()->SetHorizontal(1);
	m_vecOptionObjs.push_back(BGMSoundText);
	AddObject(BGMSoundText, GROUP_TYPE::UI);
	////////////음악 텍스트 보여주는 곳////////////////////


	////////////음악 슬라이더////////////////////
	CSliderUI* BGMSoundSlider = new CSliderUI;
	BGMSoundSlider->SetName(L"BGMSoundSlider");
	BGMSoundSlider->AddImage(pD2DMgr->GetStoredBitmap(L"ui_lifebar_bg"));
	BGMSoundSlider->AddImage(pD2DMgr->GetStoredBitmap(L"ui_soundbar_fill"));
	BGMSoundSlider->AddImage(pD2DMgr->GetStoredBitmap(L"ui_lifebar_frame"));
	BGMSoundSlider->SetObjType(GROUP_TYPE::UI);
	BGMSoundSlider->SetPos(Vec2(vResolution.x / 2, vResolution.y / 2 - 50.f));
	BGMSoundSlider->SetScale(Vec2(182.f, 33.f));
	BGMSoundSlider->SetSliderValue(CSoundMgr::GetInstance()->m_fBGMRatio);
	CSoundMgr::GetInstance()->m_pBGMSoundSlider = BGMSoundSlider;
	BGMSoundSlider->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Start::ChangeBGMRatio);

	m_vecOptionObjs.push_back(BGMSoundSlider);
	AddObject(BGMSoundSlider, GROUP_TYPE::UI);
	////////////음악 슬라이더////////////////////

	////////////음악 Ratio 보여주는 곳////////////////////
	CObject* BGMSoundRatio = new CSpriteUI;
	BGMSoundRatio->SetName(L"BGMSoundRatio");
	BGMSoundRatio->SetObjType(GROUP_TYPE::IMAGE);
	BGMSoundRatio->SetPos(Vec2(vResolution.x / 2 + 150.f, vResolution.y / 2 - 50.f));
	BGMSoundRatio->SetScale(Vec2(96.f, 24.f));

	RatioView = static_cast<int>(CSoundMgr::GetInstance()->m_fBGMRatio * 100.f);
	swprintf_s(buffer, L"%d%%", RatioView);
	BGMSoundRatio->CreateTextUI(buffer, -(BGMSoundRatio->GetScale() / 2.f), (BGMSoundRatio->GetScale() / 2.f)
		, 24, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	BGMSoundRatio->GetTextUI()->SetHorizontal(2);
	CSoundMgr::GetInstance()->m_pBGMSoundRatio = BGMSoundRatio;
	m_vecOptionObjs.push_back(BGMSoundRatio);
	AddObject(BGMSoundRatio, GROUP_TYPE::UI);
	////////////음악 Ratio 보여주는 곳////////////////////


	////////////SFX 텍스트 보여주는 곳////////////////////
	CObject* SFXSoundText = new CSpriteUI;
	SFXSoundText->SetName(L"SFXSoundText");
	SFXSoundText->SetObjType(GROUP_TYPE::IMAGE);
	SFXSoundText->SetPos(Vec2(vResolution.x / 2 - 100.f, vResolution.y / 2));
	SFXSoundText->SetScale(Vec2(196.f, 24.f));


	SFXSoundText->CreateTextUI(L"음향", -(SFXSoundText->GetScale() / 2.f), (SFXSoundText->GetScale() / 2.f)
		, 24, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	SFXSoundText->GetTextUI()->SetHorizontal(1);
	m_vecOptionObjs.push_back(SFXSoundText);
	AddObject(SFXSoundText, GROUP_TYPE::UI);
	////////////SFX 텍스트 보여주는 곳////////////////////

	////////////SFX 슬라이더////////////////////
	CSliderUI* SFXSoundSlider = new CSliderUI;
	SFXSoundSlider->SetName(L"SFXSoundSlider");
	SFXSoundSlider->AddImage(pD2DMgr->GetStoredBitmap(L"ui_lifebar_bg"));
	SFXSoundSlider->AddImage(pD2DMgr->GetStoredBitmap(L"ui_soundbar_fill"));
	SFXSoundSlider->AddImage(pD2DMgr->GetStoredBitmap(L"ui_lifebar_frame"));
	SFXSoundSlider->SetObjType(GROUP_TYPE::UI);
	SFXSoundSlider->SetPos(Vec2(vResolution.x / 2, vResolution.y / 2));
	SFXSoundSlider->SetScale(Vec2(182.f, 33.f));
	SFXSoundSlider->SetSliderValue(CSoundMgr::GetInstance()->m_fSFXRatio);
	CSoundMgr::GetInstance()->m_pSFXSoundSlider = SFXSoundSlider;
	SFXSoundSlider->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Start::ChangeSFXRatio);
	m_vecOptionObjs.push_back(SFXSoundSlider);
	AddObject(SFXSoundSlider, GROUP_TYPE::UI);
	////////////SFX 슬라이더////////////////////


	////////////SFX Ratio 보여주는 곳////////////////////
	CObject* SFXSoundRatio = new CSpriteUI;
	SFXSoundRatio->SetName(L"SFXSoundRatio");
	SFXSoundRatio->SetObjType(GROUP_TYPE::IMAGE);
	SFXSoundRatio->SetPos(Vec2(vResolution.x / 2 + 150.f, vResolution.y / 2));
	SFXSoundRatio->SetScale(Vec2(96.f, 24.f));

	RatioView = static_cast<int>(CSoundMgr::GetInstance()->m_fSFXRatio * 100.f);
	swprintf_s(buffer, L"%d%%", RatioView);
	SFXSoundRatio->CreateTextUI(buffer, -(SFXSoundRatio->GetScale() / 2.f), (SFXSoundRatio->GetScale() / 2.f)
		, 24, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	SFXSoundRatio->GetTextUI()->SetHorizontal(2);
	CSoundMgr::GetInstance()->m_pSFXSoundRatio = SFXSoundRatio;
	m_vecOptionObjs.push_back(SFXSoundRatio);
	AddObject(SFXSoundRatio, GROUP_TYPE::UI);
	////////////SFX Ratio 보여주는 곳////////////////////

	/////////////////뒤로 버튼//////////////////////
	CBtnUI* backMain = new CBtnUI;
	backMain->SetName(L"backMain");
	backMain->SetObjType(GROUP_TYPE::UI);
	backMain->SetPos(Vec2(vResolution.x / 2, vResolution.y / 2 + 50.f));
	backMain->SetScale(Vec2(302.f, 37.f));
	backMain->SetIsRound(true, 10.f, 10.f);
	backMain->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(0, 0, 0));
	backMain->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Start::OffOptionPanel);
	//pBtnOption->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Main::CreateOptionPanel);
	backMain->CreateTextUI(L"뒤로", -(backMain->GetScale() / 2.f), (backMain->GetScale() / 2.f)
		, 20, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	m_vecOptionObjs.push_back(backMain);
	AddObject(backMain, GROUP_TYPE::UI);
	/////////////////뒤로 버튼//////////////////////
}

void CScene_Start::OffOptionPanel()
{
	for (auto* OptionObj : m_vecOptionObjs) {
		DeleteObject(OptionObj);
	}
	CUIMgr::GetInstance()->SetFocusedUI(nullptr);
	m_vecOptionObjs.clear();
	CSoundMgr::GetInstance()->InitVolumePointer();
}

void CScene_Start::OptionPanelupdate()
{
	wchar_t buffer[20];

	swprintf_s(buffer, L"%d%%", static_cast<int>(CSoundMgr::GetInstance()->m_pMasterSoundSlider->GetSliderValue() * 100.f));
	CSoundMgr::GetInstance()->m_pMasterSoundRatio->GetTextUI()->SetText(buffer);

	swprintf_s(buffer, L"%d%%", static_cast<int>(CSoundMgr::GetInstance()->m_pBGMSoundSlider->GetSliderValue() * 100.f));
	CSoundMgr::GetInstance()->m_pBGMSoundRatio->GetTextUI()->SetText(buffer);

	swprintf_s(buffer, L"%d%%", static_cast<int>(CSoundMgr::GetInstance()->m_pSFXSoundSlider->GetSliderValue() * 100.f));
	CSoundMgr::GetInstance()->m_pSFXSoundRatio->GetTextUI()->SetText(buffer);
}

void CScene_Start::ChangeMasterRatio()
{
	CSoundMgr::GetInstance()->m_fMasterRatio = CSoundMgr::GetInstance()->m_pMasterSoundSlider->GetSliderValue();
	CSoundMgr::GetInstance()->SetBGMChannelVolume(CSoundMgr::GetInstance()->m_fMasterRatio * CSoundMgr::GetInstance()->m_pBGMSoundSlider->GetSliderValue());
	CSoundMgr::GetInstance()->SetSFXChannelVolume(CSoundMgr::GetInstance()->m_fMasterRatio * CSoundMgr::GetInstance()->m_pSFXSoundSlider->GetSliderValue());
}

void CScene_Start::ChangeBGMRatio()
{
	CSoundMgr::GetInstance()->m_fBGMRatio = CSoundMgr::GetInstance()->m_pBGMSoundSlider->GetSliderValue();
	CSoundMgr::GetInstance()->SetBGMChannelVolume(CSoundMgr::GetInstance()->m_fMasterRatio * CSoundMgr::GetInstance()->m_pBGMSoundSlider->GetSliderValue());
}

void CScene_Start::ChangeSFXRatio()
{
	CSoundMgr::GetInstance()->m_fSFXRatio = CSoundMgr::GetInstance()->m_pSFXSoundSlider->GetSliderValue();
	CSoundMgr::GetInstance()->SetSFXChannelVolume(CSoundMgr::GetInstance()->m_fMasterRatio * CSoundMgr::GetInstance()->m_pSFXSoundSlider->GetSliderValue());
}