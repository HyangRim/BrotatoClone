#include "pch.h"
#include "CObject.h"
#include "CCollider.h"
#include "CPlayer.h"
#include "CMissile.h"
#include "CSlingshot.h"
#include "CSlingShot_Missile.h"
#include "CScene.h"
#include "CTimeMgr.h"
#include "CSceneMgr.h"
#include "Direct2DMgr.h"
#include "CSoundMgr.h"

CSlingshot::CSlingshot()
	: m_fCoolTime(0.f)
{
	tWeaponInfo	tInfo = {};

	tInfo.m_sName = L"Slingshot";
	tInfo.m_sIconImageKey = L"slingshot_icon";
	tInfo.m_eType = WEAPON_TYPE::PISTOL;
	tInfo.m_fCooldown = 1.22f;
	tInfo.m_fCritialDMG = 2.f;
	tInfo.m_fCritialAcc = 3;
	tInfo.m_fRecogRange = 300;
	tInfo.m_iPenet = 1;
	tInfo.m_iDMG = 10;

	SetInfo(tInfo);
	SetScale(Vec2(80.f, 80.f));
	m_pPlayer = (CPlayer*)CSceneMgr::GetInstance()->GetPlayer();

	CreateImage();
	AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"slingshot"));
}

CSlingshot::~CSlingshot()
{

}



void CSlingshot::update()
{
	//타겟 몬스터 선정하기.
	CWeapon::update();

	m_fCoolTime += fDT;

	//쿨타임 주기마다 AND 타겟이 있을 때 타겟을 향해 총을 쏜다. 
	if (nullptr != GetTarget() && (Getinfo().m_fCooldown < m_fCoolTime)) {
		Vec2 targetPos = GetTarget()->GetPos();
		Vec2 direction = targetPos - GetPos();

		//또한 사거리 안에 있을 때. 
		if (direction.Length() <= Getinfo().m_fRecogRange) {
			direction.Normalize();
			ShotMissile(direction);

			m_fCoolTime = 0.f;
		}
	}
}

void CSlingshot::render(Gdiplus::Graphics* _pDGraphics)
{
	Vec2 vPos = GetPos();
	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(vPos);
	Vec2 vScale = GetScale();
	Pen pen(Color(255, 255, 0, 255), 2.0f);
	_pDGraphics->DrawEllipse(&pen,
		vRenderPos.x - vScale.x / 2.f,
		vRenderPos.y - vScale.y / 2.f,
		vScale.x,
		vScale.y);
}

void CSlingshot::render(ID2D1HwndRenderTarget* _pRender)
{
	if (GetImages().size() == 0) {
		// 실제 좌표 계산
		Vec2 vPos = GetPos();
		Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(vPos);
		Vec2 vScale = GetScale();

		// Direct2D에서는 DrawEllipse 함수를 사용합니다.
		// 원의 중심은 vRenderPos, 가로/세로 반지름은 각각 vScale.x/2, vScale.y/2 로 설정합니다.
		D2D1_ELLIPSE ellipse = D2D1::Ellipse(
			D2D1::Point2F(vRenderPos.x, vRenderPos.y),
			vScale.x / 2.f,
			vScale.y / 2.f
		);

		// 노란색 브러시 생성 (RGBA: 255, 255, 0, 255)
		ID2D1SolidColorBrush* pBrush = nullptr;
		HRESULT hr = _pRender->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Yellow),
			&pBrush
		);
		if (SUCCEEDED(hr))
		{
			// 선 두께 2.0f로 타원 그리기
			_pRender->DrawEllipse(ellipse, pBrush, 2.0f);
			pBrush->Release();
		}
	}
	CWeapon::render(_pRender);
}

void CSlingshot::ShotMissile(Vec2 _vDir)
{
	CSlingShot_Missile* pMissile = new CSlingShot_Missile;

	pMissile->SetName(L"Missile_Player");
	pMissile->SetPos(GetPos());
	pMissile->SetScale(Vec2(15.f, 15.f));
	pMissile->SetDir(_vDir);
	pMissile->SetBounce(1);

	//무기 데미지 값 넘겨주기
	int default_damage = (Getinfo().m_iDMG + static_cast<int>(m_pPlayer->GetPlayerInfo().m_fRangeCoef));

	float finalDMGCoef = 1.f + m_pPlayer->GetPlayerInfo().m_fDamageCoef;

	if (nullptr != m_pPlayer->GetPlayerInfo().m_stCharacterInfo) {
		finalDMGCoef += m_pPlayer->GetPlayerInfo().m_stCharacterInfo->m_fDamageCoef;
	}

	default_damage = static_cast<int>(default_damage * finalDMGCoef);
	if (random_distribution(random_generator) < Getinfo().m_fCritialAcc) {
		default_damage = static_cast<int>(static_cast<float>(default_damage) * Getinfo().m_fCritialDMG);
		pMissile->SetCritical(true);
	}
	pMissile->SetDamage(default_damage);
	pMissile->CreateImage();
	pMissile->AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"slingshot_projectile"));
	
	CSoundMgr::GetInstance()->Play(L"SlingFire");
	CreateObject(pMissile, GROUP_TYPE::PROJ_PLAYER);
}

