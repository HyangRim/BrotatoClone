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


CScene_Start::CScene_Start()
	: m_bUseForce(false)
	, m_fForceRadius(500.f)
	, m_fCurRadius(0.f)
	, m_fForce(500.f)
{
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"texture\\entities\\enemies\\baby_alien.png", L"NormalEnemy", false);
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"texture\\entities\\enemies\\spitter.png", L"RangeEnemy", false);
}

CScene_Start::~CScene_Start()
{

}

void CScene_Start::update()
{
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


	////////////////////////////맵생성///////////////////////////////////////
	/*
	int randV = rand() % 7;
	if (randV == 0) randV++;
	//printf("randV : %d\n", randV);
	wstring mapPath = L"texture\\tiles\\tiles_" + std::to_wstring(randV) + L".png";
	MakeMapTile(L"texture\\tiles\\tiles_outline.png", mapPath.c_str(), L"texture\\tiles\\map\\",10, 1);
	pD2DMgr->StoreBitmapsFromFolder(L"texture\\tiles\\map\\", L"Map");
	MakeTile(L"Map");*/
	////////////////////////////////////////////////////////////////////////

	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	//Object 추가.
	//실제 생성된 객체는 플레이어, 주소를 받은 건 부모 클래스. 
	CObject* pObj = new CPlayer;
	pObj->SetPos(Vec2(640.f, 384.f));
	pObj->SetScale(Vec2(100.f, 100.f));
	pObj->SetName(L"Player");
	AddObject(pObj, GROUP_TYPE::PLAYER);
	RegisterPlayer(pObj);


	CCamera::GetInstance()->SetTarget(pObj);

	CWeapon* pWeapon = nullptr;
	CMonster* pMon = nullptr;
	/*
	//무기 권총 추가. 
	pWeapon = new CPistol;
	((CPlayer*)GetPlayer())->AddWeapon(pWeapon);

	//무기 단검 추가. 
	pWeapon = new CKnife;
	((CPlayer*)GetPlayer())->AddWeapon(pWeapon);

	//무기 새총 추가.
	pWeapon = new CSlingshot;
	((CPlayer*)GetPlayer())->AddWeapon(pWeapon);

	pMon = CMonFactory::CreateMonster(MON_TYPE::NORMAL, vResolution / 2.f - Vec2(0.f, 300.f));
	AddObject(pMon, GROUP_TYPE::MONSTER);

	//pMon = CMonFactory::CreateMonster(MON_TYPE::NORMAL, vResolution / 2.f - Vec2(-200.f, 300.f));
	//AddObject(pMon, GROUP_TYPE::MONSTER);
	*/

	
	pMon = CMonFactory::CreateMonster(MON_TYPE::RANGE, vResolution / 2.f - Vec2(-400.f, 300.f));
	pMon->SetScale(Vec2(45.f, 45.f));
	pMon->SetWaveDuration(1.f);
	AddObject(pMon, GROUP_TYPE::MONSTER);

	//Enter은 몰라고 update에서는 CreateObject로 해야함...
	//CreateObject(pMon, Object_type::MONSTER):
	
	//땅 물체 배치
	CObject* pGround = new CGround;
	pGround->SetName(L"Ground");
	pGround->SetPos(Vec2(640.f, 584.f));
	pGround->SetScale(Vec2(200.f, 60.f));
	AddObject(pGround, GROUP_TYPE::GROUND);

	//충돌 지점. 
	//Player 그룹과 Monster그룹간의 충돌 체크 
	//update에서 로직, finalupdate에서 최종 움직임 완료



	//사운드 추가
	CSoundMgr::GetInstance()->AddSound(L"Extend", L"sound\\extend.wav", false, false);

	//이제 새로운 충돌이 발생할수도 있음. 
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::WEAPON, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PROJ_PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::GROUND, GROUP_TYPE::PLAYER);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::DROP_ITEM, GROUP_TYPE::PLAYER);
	
	//Camera Look 지점.
	CCamera::GetInstance()->SetLookAt(vResolution / 2.f);

	//Camera 효과 지점. 
	CCamera::GetInstance()->FadeOut(1.f);
	CCamera::GetInstance()->FadeIn(1.f);
	
	start();
}

void CScene_Start::Exit()
{
	//나갈때 전부 삭제해줘야함.

	DeleteAll();
	//충돌도 전부 초기화 해주기. 
	CCollisionMgr::GetInstance()->Reset();
}

void CScene_Start::CreateForce()
{
	m_vForcePos = CCamera::GetInstance()->GetRealPos(MOUSE_POS);
}