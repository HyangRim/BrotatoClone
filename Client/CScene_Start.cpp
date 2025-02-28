#include "pch.h"
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
#include "CSpriteUI.h"
#include "CTextUI.h"
#include "CWaveMgr.h"

#include "CMobSpawner.h"


CScene_Start::CScene_Start()
	: m_bUseForce(false)
	, m_fForceRadius(500.f)
	, m_fCurRadius(0.f)
	, m_fForce(500.f)
{
	/*
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"texture\\entities\\enemies\\baby_alien.png", L"NormalEnemy", false);
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"texture\\entities\\enemies\\spitter.png", L"RangeEnemy", false);
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"texture\\entities\\enemies\\entity_birth.png", L"BirthEnemy", false);
	*/
	/*
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"texture\\ui\\hud\\ui_lifebar_bg.png", L"LifebarBackGround", false);
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"texture\\ui\\hud\\ui_lifebar_fill.png", L"LifebarFill", false);

	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"texture\\ui\\hud\\ui_xp_bg.png", L"XpBackGround", false);
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"texture\\ui\\hud\\ui_xp_fill.png", L"XpFill", false);

	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"texture\\ui\\hud\\ui_lifebar_frame.png", L"LifebarFrame", false);

	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"items\\materials\\harvesting_icon.png", L"HarvestingIcon", false);
	*/
	//Drop_ITEM��
	/*
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"items\\materials\\material_0000.png", L"drop_item0", false);
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"items\\materials\\material_0001.png", L"drop_item1", false);
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"items\\materials\\material_0002.png", L"drop_item2", false);
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"items\\materials\\material_0003.png", L"drop_item3", false);
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"items\\materials\\material_0004.png", L"drop_item4", false);
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"items\\materials\\material_0005.png", L"drop_item5", false);
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"items\\materials\\material_0006.png", L"drop_item6", false);
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"items\\materials\\material_0007.png", L"drop_item7", false);
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"items\\materials\\material_0008.png", L"drop_item8", false);
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"items\\materials\\material_0009.png", L"drop_item9", false);
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"items\\materials\\material_0010.png", L"drop_item10", false);
	*/
	//���� ���� ���
	//���� ���� Instance ��������. 

	/*
	CSoundMgr::GetInstance()->AddSound(L"drop_item1", L"sound\\materials\\drop_item1.wav", false, false);
	CSoundMgr::GetInstance()->AddSound(L"drop_item2", L"sound\\materials\\drop_item2.wav", false, false);
	CSoundMgr::GetInstance()->AddSound(L"drop_item3", L"sound\\materials\\drop_item3.wav", false, false);
	CSoundMgr::GetInstance()->AddSound(L"drop_item4", L"sound\\materials\\drop_item4.wav", false, false);
	CSoundMgr::GetInstance()->AddSound(L"drop_item5", L"sound\\materials\\drop_item5.wav", false, false);
	*/
	/*
	CSoundMgr::GetInstance()->AddSound(L"step1", L"sound\\step\\Step1.mp3", false, false);
	CSoundMgr::GetInstance()->AddSound(L"step2", L"sound\\step\\Step2.mp3", false, false);
	CSoundMgr::GetInstance()->AddSound(L"step3", L"sound\\step\\Step3.mp3", false, false);
	CSoundMgr::GetInstance()->AddSound(L"step4", L"sound\\step\\Step4.mp3", false, false);
	*/
}

CScene_Start::~CScene_Start()
{

}

void CScene_Start::update()
{
	//���̺� ����. 
	if (KEY_HOLD(KEY::LBTN)) {
		m_bUseForce = true;
		CreateForce();
	}
	else {
		m_bUseForce = false;
	}

	if (KEY_TAP(KEY::C)) {
		CSoundMgr::GetInstance()->Play(L"Extend");
	}

	for (UINT typeIDX = 0; typeIDX < (UINT)GROUP_TYPE::END; typeIDX++) {
		const vector<CObject*>& vecObj = GetGroupObject((GROUP_TYPE)typeIDX);

		///////////////////////////////////////////
		if (typeIDX == (UINT)GROUP_TYPE::UI)
		{
			for (size_t objIDX = 0; objIDX < vecObj.size(); objIDX++) 
			{
				////////////////////ü�¹� ����//////////////////////////////////
				if (vecObj[objIDX]->GetName().compare(L"Lifebar") == 0)
				{
					CPlayer* player = (CPlayer*)GetPlayer();
					playerParameter playerInfo = player->GetPlayerInfo();

					wchar_t buffer[20];
					swprintf_s(buffer, L"%d / %d", playerInfo.m_iCurHP, playerInfo.m_iMaxHP);

					vecObj[objIDX]->GetTextUI()->SetText(buffer);

					//ü�¹� ��������
					CImage* image = (CImage*)vecObj[objIDX]->GetImage(1);
					image->SetRatio((float)playerInfo.m_iCurHP/ (float)playerInfo.m_iMaxHP);
				}
				///////////////////////////////////////////////////////////////////
				////////////////////����ġ�� ����//////////////////////////////////
				if (vecObj[objIDX]->GetName().compare(L"Xpbar") == 0)
				{
					CPlayer* player = (CPlayer*)GetPlayer();
					playerParameter playerInfo = player->GetPlayerInfo();

					wchar_t buffer[20];
					swprintf_s(buffer, L"%d / %d", playerInfo.m_iCurEXP, playerInfo.m_iMaxEXP);

					vecObj[objIDX]->GetTextUI()->SetText(buffer);

					//����ġ�� ��������
					CImage* image = (CImage*)vecObj[objIDX]->GetImage(1);
					image->SetRatio(min((float)((float)playerInfo.m_iCurEXP / (float)playerInfo.m_iMaxEXP), 1.f));
				}
				///////////////////////////////////////////////////////////////////
				////////////////////��ȭ���� ����//////////////////////////////////
				if (vecObj[objIDX]->GetName().compare(L"HarvestingText") == 0)
				{
					CPlayer* player = (CPlayer*)GetPlayer();
					playerParameter playerInfo = player->GetPlayerInfo();

					wchar_t buffer[20];
					swprintf_s(buffer, L"%d", playerInfo.m_iCoin);

					vecObj[objIDX]->GetTextUI()->SetText(buffer);
				}
				///////////////////////////////////////////////////////////////////
			}
		}
		///////////////////////////////////////////
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
		ChangeScene(SCENE_TYPE::TOOL);
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

	if (!m_bUseForce) return;

	m_fCurRadius += m_fForceRadius * 3.f * fDT;

	if (m_fCurRadius > m_fForceRadius) {
		m_fCurRadius = 0.f;
	}
	
	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(m_vForcePos);

	// vRenderPos�� m_fCurRadius�� �̹� ���Ǿ� �ִٰ� �����մϴ�.
	D2D1_ELLIPSE ellipse = D2D1::Ellipse(
		D2D1::Point2F(vRenderPos.x, vRenderPos.y),
		m_fCurRadius,    // x�� ������
		m_fCurRadius     // y�� ������
	);

	// ��� �귯�� ���� (ARGB: 255, 0, 255, 0 => ���)
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
	
	////////////////////////////�ʻ���///////////////////////////////////////
	int randV = rand() % 7;
	if (randV == 0) randV++;
	wstring mapPath = L"texture\\tiles\\tiles_" + std::to_wstring(randV) + L".png";
	MakeMapTile(L"texture\\tiles\\tiles_outline.png", mapPath.c_str(), L"texture\\tiles\\map\\",50, 1);
	pD2DMgr->StoreBitmapsFromFolder(L"texture\\tiles\\map\\", L"Map");
	MakeTile(L"Map");
	////////////////////////////////////////////////////////////////////////

	/////////////////////////////ü�¹�,����ġ��////////////////////////////
	
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

	////////////////////////////���̺� ����/////////////////////////////////
	CObject* wavePanel = new CSpriteUI;
	wstring waveLevel = L"���̺� ";
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

	////////////////////////////���̺� ����/////////////////////////////////
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

	////////////////////////////��ȭ ǥ��///////////////////////////////////
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

	//Object �߰�.
	//���� ������ ��ü�� �÷��̾�, �ּҸ� ���� �� �θ� Ŭ����. 
	CObject* pObj = new CPlayer;
	pObj->SetPos(Vec2(640.f, 384.f));
	pObj->SetScale(Vec2(65.f, 65.f));
	pObj->SetName(L"Player");
	AddObject(pObj, GROUP_TYPE::PLAYER);
	RegisterPlayer(pObj);
	

	CCamera::GetInstance()->SetTarget(pObj);

	CWeapon* pWeapon = nullptr;
	CMonster* pMon = nullptr;
	/*
	//���� ���� �߰�. 
	pWeapon = new CPistol;
	((CPlayer*)GetPlayer())->AddWeapon(pWeapon);

	//���� �ܰ� �߰�. 
	pWeapon = new CKnife;
	((CPlayer*)GetPlayer())->AddWeapon(pWeapon);

	//���� ���� �߰�.
	pWeapon = new CSlingshot;
	((CPlayer*)GetPlayer())->AddWeapon(pWeapon);

	pMon = CMonFactory::CreateMonster(MON_TYPE::NORMAL, vResolution / 2.f - Vec2(0.f, 300.f));
	AddObject(pMon, GROUP_TYPE::MONSTER);

	//pMon = CMonFactory::CreateMonster(MON_TYPE::NORMAL, vResolution / 2.f - Vec2(-200.f, 300.f));
	//AddObject(pMon, GROUP_TYPE::MONSTER);
	*/

	
	/*
	pMon = CMonFactory::CreateMonster(MON_TYPE::RANGE, vResolution / 2.f - Vec2(-400.f, 300.f));
	pMon->SetScale(Vec2(45.f, 45.f));
	pMon->SetWaveDuration(1.f);
	AddObject(pMon, GROUP_TYPE::MONSTER);*/

	//Enter�� ����� update������ CreateObject�� �ؾ���...
	//CreateObject(pMon, Object_type::MONSTER):
	
	
	
	//�� ��ü ��ġ
	/*
	CObject* pGround = new CGround;
	pGround->SetName(L"Ground");
	pGround->SetPos(Vec2(640.f, 584.f));
	pGround->SetScale(Vec2(200.f, 60.f));
	AddObject(pGround, GROUP_TYPE::GROUND);
	*/
	//�浹 ����. 
	//Player �׷�� Monster�׷찣�� �浹 üũ 
	//update���� ����, finalupdate���� ���� ������ �Ϸ�



	//���� �߰�
	CSoundMgr::GetInstance()->AddSound(L"extend", L"sound\\extend.wav", false, false);

	//���� ���ο� �浹�� �߻��Ҽ��� ����. 
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::WEAPON, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PROJ_PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::GROUND, GROUP_TYPE::PLAYER);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::DROP_ITEM, GROUP_TYPE::PLAYER);
	
	//Camera Look ����.
	CCamera::GetInstance()->SetLookAt(vResolution / 2.f);

	/*
	//Camera ȿ�� ����. 
	CCamera::GetInstance()->FadeOut(1.f);
	CCamera::GetInstance()->FadeIn(1.f);
	*/

	//���̺� �����ϵ��� ����.(Scene_Start�� ������ ������ �� �� �����̴ϱ�.)
	CWaveMgr::GetInstance()->WaveStart();
	start();
}

void CScene_Start::Exit()
{
	//������ ���� �����������.

	DeleteAll();
	//�浹�� ���� �ʱ�ȭ ���ֱ�. 
	CCollisionMgr::GetInstance()->Reset();

	//���̺갡 ����. 
	CWaveMgr::GetInstance()->WaveStart();
}

void CScene_Start::CreateForce()
{
	m_vForcePos = CCamera::GetInstance()->GetRealPos(MOUSE_POS);
}