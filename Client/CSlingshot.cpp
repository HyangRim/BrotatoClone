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

CSlingshot::CSlingshot()
	: m_fCoolTime(0.f)
{
	tWeaponInfo	tInfo = {};

	tInfo.m_sName = L"Slingshot";
	tInfo.m_eType = WEAPON_TYPE::PISTOL;
	tInfo.m_fCooldown = 1.22f;
	tInfo.m_fCritialDMG = 2.f;
	tInfo.m_fCritialAcc = 3;
	tInfo.m_fRecogRange = 300;
	tInfo.m_iPenet = 1;
	tInfo.m_iDMG = 10;

	SetInfo(tInfo);
	SetScale(Vec2(10.f, 10.f));
	m_pPlayer = (CPlayer*)CSceneMgr::GetInstance()->GetCurScene()->GetPlayer();
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

void CSlingshot::ShotMissile(Vec2 _vDir)
{
	CSlingShot_Missile* pMissile = new CSlingShot_Missile;

	pMissile->SetName(L"Missile_Player");
	pMissile->SetPos(GetPos());
	pMissile->SetScale(Vec2(5.f, 5.f));
	pMissile->SetDir(_vDir);
	pMissile->SetBounce(1);

	//무기 데미지 값 넘겨주기
	int default_damage = Getinfo().m_iDMG;
	if (random_distribution(random_generator) < Getinfo().m_fCritialAcc) {
		default_damage = static_cast<int>(static_cast<float>(default_damage) * Getinfo().m_fCritialDMG);
	}
	pMissile->SetDamage(default_damage);

	CreateObject(pMissile, GROUP_TYPE::PROJ_PLAYER);
}

