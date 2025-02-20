#include "pch.h"
#include "CWeapon.h"
#include "CScene.h"
#include "CSceneMgr.h"

CWeapon::CWeapon()
	: m_tWeaponInfo{}
	, m_vRotation()
{
	m_pCurScene = CSceneMgr::GetInstance()->GetCurScene();
}

CWeapon::~CWeapon()
{


}


void CWeapon::update()
{
	m_pTarget = SpecTarget();
}

void CWeapon::render(Gdiplus::Graphics* _pDGraphics)
{

}

void CWeapon::ShotMissile(Vec2 _vDir)
{
}

CObject* CWeapon::SpecTarget()
{
	float proximateDistance = 999999999.f;
	CObject* pObj = nullptr;
	const vector<CObject*>& vecMonObj = m_pCurScene->GetGroupObject(GROUP_TYPE::MONSTER);

	for (const auto MonObj : vecMonObj) {
		Vec2 vPos = GetPos();
		Vec2 vMonPos = MonObj->GetPos();

		float Distance = (vPos - vMonPos).Length();

		if (Distance < proximateDistance) {
			Distance = proximateDistance;
			pObj = MonObj;
		}
	}
	return pObj;
}

