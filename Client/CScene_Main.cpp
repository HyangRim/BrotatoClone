#include "pch.h"
#include "CScene_Main.h"
#include "CTimeMgr.h"
#include "CUI.h"
#include "CPanelUI.h"
#include "CBtnUI.h"
#include "CCore.h"
#include "CScene.h"
#include "CCamera.h"
#include "CFontMgr.h"
#include "CTextUI.h"
#include "CSpriteUI.h"
#include "Direct2DMgr.h"
#include "CSoundMgr.h"

#include "CObject.h"
#include "CDamageUI.h"
#include "CSpriteUI.h"
#include "CGround.h"
#include "CImage.h"


CScene_Main::CScene_Main()
	: m_tMainPanel{}
	, m_fPanelMoveDuration(6.f)
	, m_fPanelMoveElapsed(0.f)
	, m_fPanelMoveWave(37.f)
	, m_fBrotatoDuration(1.5f)
	, m_fBrotatoElased(0.f)
	, m_fBrotatoWave(0.15f)
{
}

CScene_Main::~CScene_Main()
{
}

void CScene_Main::Enter()
{
	// Direct2DMgr 인스턴스 가져오기 (텍스쳐 관련)
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	//pD2DMgr->LoadAndStoreBitmap(L"texture\\ui\\menus\\title_screen\\splash_art_bg.png", L"Title_Ground", false);
	//pD2DMgr->LoadAndStoreBitmap(L"texture\\ui\\menus\\title_screen\\splash_art_brotato.png", L"Title_Brotato", false);
	//pD2DMgr->LoadAndStoreBitmap(L"texture\\ui\\menus\\title_screen\\splash_art_mist_back.png", L"Title_back", false);
	//pD2DMgr->LoadAndStoreBitmap(L"texture\\ui\\menus\\title_screen\\splash_art_mist_front.png", L"Title_front", false);
	//pD2DMgr->LoadAndStoreBitmap(L"texture\\ui\\menus\\title_screen\\splash_art_mist_mid.png", L"Title_mid", false);
	//pD2DMgr->LoadAndStoreBitmap(L"texture\\ui\\menus\\title_screen\\splash_art_post_processing.png", L"Title_lights", false);
	//pD2DMgr->LoadAndStoreBitmap(L"texture\\ui\\menus\\title_screen\\ui_logo.png", L"Brotato_logo", false);


	//사운드 관련 Instance 가져오기. 
	//CSoundMgr::GetInstance()->AddSound(L"MainTitle", L"sound\\music\\main_title_bgm.mp3", true, true);
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	//UI구성. 
	//Object 추가.
	//실제 생성된 객체는 플레이어, 주소를 받은 건 부모 클래스. 
	//맨 뒤 Brotato 바닥. 
	m_tMainPanel.pTitleGround = new CGround;
	m_tMainPanel.pTitleGround->SetPos(vResolution / 2.f);
	m_tMainPanel.pTitleGround->SetObjType(GROUP_TYPE::DEFAULT);
	m_tMainPanel.pTitleGround->SetScale(Vec2(1035.f, 540.f));
	m_tMainPanel.pTitleGround->SetName(L"Title_Ground");
	m_tMainPanel.pTitleGround->CreateImage();
	m_tMainPanel.pTitleGround->AddImage(pD2DMgr->GetStoredBitmap(L"splash_art_bg"));
	m_tMainPanel.pTitleGround->SetObjType(GROUP_TYPE::DEFAULT);
	AddObject(m_tMainPanel.pTitleGround, GROUP_TYPE::DEFAULT);

	
	//맨 뒤, 응원단
	m_tMainPanel.pTitleBack = new CGround;
	//m_tMainPanel.pTitleBack = new CSpriteUI;

	m_tMainPanel.pTitleBack->SetPos(vResolution / 2.f);
	m_tMainPanel.pTitleBack->SetScale(Vec2(1035.f, 540.f));
	m_tMainPanel.pTitleBack->SetName(L"Title_back");
	m_tMainPanel.pTitleBack->CreateImage();
	m_tMainPanel.pTitleBack->AddImage(pD2DMgr->GetStoredBitmap(L"splash_art_mist_back"));
	//m_tMainPanel.pTitleBack->GetImage()->SetBitmap(pD2DMgr->GetStoredBitmap(L"Title_back"));
	m_tMainPanel.pTitleBack->SetObjType(GROUP_TYPE::DEFAULT);
	AddObject(m_tMainPanel.pTitleBack, GROUP_TYPE::DEFAULT);

	//중간, 응원단
	m_tMainPanel.pTitleMid = new CGround;
	m_tMainPanel.pTitleMid->SetPos(vResolution / 2.f);
	m_tMainPanel.pTitleMid->SetScale(Vec2(1035.f, 540.f));
	m_tMainPanel.pTitleMid->SetName(L"Title_mid");
	m_tMainPanel.pTitleMid->CreateImage();
	m_tMainPanel.pTitleMid->AddImage(pD2DMgr->GetStoredBitmap(L"splash_art_mist_mid"));
	//m_tMainPanel.pTitleMid->GetImage()->SetBitmap(pD2DMgr->GetStoredBitmap(L"Title_mid"));
	m_tMainPanel.pTitleMid->SetObjType(GROUP_TYPE::DEFAULT);
	AddObject(m_tMainPanel.pTitleMid, GROUP_TYPE::DEFAULT);

	//맨 앞, 응원단
	m_tMainPanel.pTitlefront = new CGround;
	m_tMainPanel.pTitlefront->SetPos(vResolution / 2.f);
	m_tMainPanel.pTitlefront->SetScale(Vec2(1035.f, 540.f));
	m_tMainPanel.pTitlefront->SetName(L"Title_front");
	m_tMainPanel.pTitlefront->CreateImage();
	m_tMainPanel.pTitlefront->AddImage(pD2DMgr->GetStoredBitmap(L"splash_art_mist_front"));
	//m_tMainPanel.pTitlefront->GetImage()->SetBitmap(pD2DMgr->GetStoredBitmap(L"Title_front"));
	m_tMainPanel.pTitlefront->SetObjType(GROUP_TYPE::DEFAULT);
	AddObject(m_tMainPanel.pTitlefront, GROUP_TYPE::DEFAULT);
	
	//Brotato
	m_tMainPanel.pTitleBrotato = new CGround;
	m_tMainPanel.pTitleBrotato->SetPos(vResolution / 2.f);
	m_tMainPanel.pTitleBrotato->SetScale(Vec2(931.5f, 486.f));
	m_tMainPanel.pTitleBrotato->SetName(L"Title_Brotato");
	m_tMainPanel.pTitleBrotato->CreateImage();
	m_tMainPanel.pTitleBrotato->AddImage(pD2DMgr->GetStoredBitmap(L"splash_art_brotato"));
	//m_tMainPanel.pTitleBrotato->GetImage()->SetBitmap(pD2DMgr->GetStoredBitmap(L"Title_Brotato"));
	m_tMainPanel.pTitleBrotato->SetObjType(GROUP_TYPE::DEFAULT);
	AddObject(m_tMainPanel.pTitleBrotato, GROUP_TYPE::DEFAULT);

	//포스트 프로세싱
	m_tMainPanel.pTitlelights = new CGround;
	m_tMainPanel.pTitlelights->SetPos(vResolution / 2.f);
	m_tMainPanel.pTitlelights->SetScale(Vec2(1035.f, 540.f));
	m_tMainPanel.pTitlelights->SetName(L"Title_lights");
	m_tMainPanel.pTitlelights->CreateImage();
	m_tMainPanel.pTitlelights->AddImage(pD2DMgr->GetStoredBitmap(L"splash_art_post_processing"));
	//m_tMainPanel.pTitlelights->GetImage()->SetBitmap(pD2DMgr->GetStoredBitmap(L"Title_lights"));
	m_tMainPanel.pTitlelights->SetObjType(GROUP_TYPE::DEFAULT);
	AddObject(m_tMainPanel.pTitlelights, GROUP_TYPE::DEFAULT);

	//브로타토 로고
	m_tMainPanel.pTitlelogo = new CGround;
	m_tMainPanel.pTitlelogo->SetPos(Vec2(vResolution.x / 2.f, 115.f));
	m_tMainPanel.pTitlelogo->SetScale(Vec2(561.f, 165.f));
	m_tMainPanel.pTitlelogo->SetName(L"Brotato_logo");
	m_tMainPanel.pTitlelogo->CreateImage();
	m_tMainPanel.pTitlelogo->AddImage(pD2DMgr->GetStoredBitmap(L"ui_logo"));
	//m_tMainPanel.pTitlelogo->GetImage()->SetBitmap(pD2DMgr->GetStoredBitmap(L"Brotato_logo"));
	m_tMainPanel.pTitlelogo->SetObjType(GROUP_TYPE::DEFAULT);
	AddObject(m_tMainPanel.pTitlelogo, GROUP_TYPE::DEFAULT);


	/*
	//버튼 패널 추가.
	CUI* pPanelUI = new CPanelUI;
	pPanelUI->SetName(L"ParentUI");
	pPanelUI->SetScale(Vec2(100.f, 200.f));
	pPanelUI->SetPos(Vec2(80.f, 400.f));
	((CPanelUI*)pPanelUI)->SetCanMove(false);
	*/

	/*
	CBtnUI* backBtn = new CBtnUI;
	backBtn->SetName(L"BackBtn");
	backBtn->SetObjType(GROUP_TYPE::UI);
	backBtn->SetScale(Vec2(94.f, 28.f));
	backBtn->SetPos(Vec2(71.f, 38.f));
	backBtn->SetIsRound(true, 10.f, 10.f);
	backBtn->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(28, 28, 28));
	backBtn->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::MAIN, 0);
	backBtn->CreateTextUI(L"뒤로", Vec2(-47.f, -14.f), Vec2(47.f, 14.f)
		, 12, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(backBtn, GROUP_TYPE::UI);
	*/

	//버튼 추가. 

	//////////////////////////시작 버튼///////////////////////////
	CBtnUI* pStartBtn = new CBtnUI;
	pStartBtn->SetName(L"StartBtn");
	pStartBtn->SetObjType(GROUP_TYPE::UI);
	pStartBtn->SetScale(Vec2(52.f, 34.f));
	pStartBtn->SetPos(Vec2(51.f,348.f));
	pStartBtn->SetIsRound(true, 10.f, 10.f);
	pStartBtn->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(0, 0, 0));
	pStartBtn->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::START, 0);
	pStartBtn->CreateTextUI(L"시작", Vec2(-26.f, -17.f), Vec2(26.f, 17.f)
		, 20, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(pStartBtn, GROUP_TYPE::UI);
	//////////////////////////시작 버튼///////////////////////////


	//////////////////////////임시 버튼///////////////////////////
	CBtnUI* pBtnTemp = new CBtnUI;
	pBtnTemp->SetName(L"Temp");
	pBtnTemp->SetObjType(GROUP_TYPE::UI);
	pBtnTemp->SetScale(Vec2(52.f, 34.f));
	pBtnTemp->SetPos(Vec2(51.f, 348.f + 12.f + 34.f));
	pBtnTemp->SetIsRound(true, 10.f, 10.f);
	pBtnTemp->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(0, 0, 0));
	pBtnTemp->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::SELECT_CHARACTER, 0);
	pBtnTemp->CreateTextUI(L"캐선", Vec2(-26.f, -17.f), Vec2(26.f, 17.f)
		, 20, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(pBtnTemp, GROUP_TYPE::UI);
	//////////////////////////임시 버튼///////////////////////////

	//////////////////////////임시 버튼///////////////////////////
	CBtnUI* pBtnTemp2 = new CBtnUI;
	pBtnTemp2->SetName(L"Temp2");
	pBtnTemp2->SetObjType(GROUP_TYPE::UI);
	pBtnTemp2->SetScale(Vec2(52.f, 34.f));
	pBtnTemp2->SetPos(Vec2(51.f, 348.f + (12.f + 34.f) * 2.f));
	pBtnTemp2->SetIsRound(true, 10.f, 10.f);
	pBtnTemp2->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(0, 0, 0));
	pBtnTemp2->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::RUN_END, 0);
	pBtnTemp2->CreateTextUI(L"실패화면", Vec2(-26.f, -17.f), Vec2(26.f, 17.f)
		, 20, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(pBtnTemp2, GROUP_TYPE::UI);
	//////////////////////////임시 버튼///////////////////////////
	
	//버튼 이벤트 등록.

	//pBtnUI->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Main::StartBtn);
	//버튼을 패널에 자식으로 등록. 
	//pPanelUI->AddChild(pBtnUI);

	//AddObject(pBtnUI, GROUP_TYPE::UI);

	//카메라 위치 세팅. 
	CCamera::GetInstance()->SetLookAt(vResolution / 2.f);
	wstring mainTitleBGMKey = L"main_title_bgm";
	//처음 시작했을 때, BGM틀기. 
	if (!CSoundMgr::GetInstance()->IsPlaySound(mainTitleBGMKey)) {
		CSoundMgr::GetInstance()->Play(mainTitleBGMKey, 0.4f);
	}
}

void CScene_Main::Exit()
{
	DeleteAll();
}

void CScene_Main::update()
{
	CScene::update();

	

	m_fPanelMoveElapsed += fDT;
	m_fBrotatoElased += fDT;

	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	Vec2 oriPos = vResolution / 2.f;


	float angle = 2 * PI * m_fPanelMoveElapsed / m_fPanelMoveDuration;
	float delta = m_fPanelMoveWave * sin(angle);


	Vec2 backImagePos = m_tMainPanel.pTitleBack->GetPos();
	Vec2 frontImagePos = m_tMainPanel.pTitlefront->GetPos();
	backImagePos.x = oriPos.x + delta;
	frontImagePos.x = oriPos.x - delta;
	//backImagePos.x += 10.f * fDT;

	m_tMainPanel.pTitleBack->SetPos(backImagePos);
	m_tMainPanel.pTitlefront->SetPos(frontImagePos);

	if (m_fPanelMoveElapsed > m_fPanelMoveDuration) {
		m_fPanelMoveElapsed = 0.f;
	}

	//여기까진 메인 화면 군중들 왔다리 갔다리 하는 것. 
	////////////////////////////////////////////////

	Vec2 OriginalBrotatoScale = m_tMainPanel.pTitleBrotato->GetScale();
	Vec2 BrotatoScaleWave = OriginalBrotatoScale * m_fBrotatoWave;

	angle = 2 * PI * m_fBrotatoElased / m_fBrotatoDuration;
	delta = m_fBrotatoWave * sin(angle);

	Vec2 BrotatoRenderScale;
	BrotatoRenderScale.x = OriginalBrotatoScale.x + ((1.f + delta) * BrotatoScaleWave.x);
	BrotatoRenderScale.y = OriginalBrotatoScale.y + ((1.f - delta) * BrotatoScaleWave.y);

	m_tMainPanel.pTitleBrotato->SetRenderScale(BrotatoRenderScale);

	if (m_fBrotatoElased > m_fBrotatoDuration) {
		m_fBrotatoElased = 0.f;
	}
}

void CScene_Main::finalupdate()
{
	CScene::finalupdate();
}

void CScene_Main::render(HDC _dc)
{
	CScene::render(_dc);
}

void CScene_Main::render(Gdiplus::Graphics* _pDGraphics)
{
	// 부모 클래스의 render 호출
	CScene::render(_pDGraphics);
}

void CScene_Main::render(ID2D1HwndRenderTarget* _pRender)
{
	CScene::render(_pRender);
}

void CScene_Main::StartBtn()
{
	ChangeScene(SCENE_TYPE::START);
}

