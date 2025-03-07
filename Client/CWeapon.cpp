#include "pch.h"
#include "CWeapon.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CScene.h"
#include "CSceneMgr.h"

CWeapon::CWeapon()
	: m_tWeaponInfo{}
	, m_vRotation()
	, m_vWeaponOffset()
	, m_pTexture(nullptr)
	, m_pPlayer(nullptr)
	, m_pCurScene(nullptr)
	, m_pTarget(nullptr)
{
	//m_pCurScene = CSceneMgr::GetInstance()->GetCurScene();
	m_pPlayer = static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer());
}

CWeapon::~CWeapon()
{


}


void CWeapon::update()
{
	m_pTarget = SpecTarget();
	//무기의 Pos가 플레이어 옆에 있도록 하는 update
	Vec2 vPlayerPos = m_pPlayer->GetPos();
	SetPos(vPlayerPos + m_vWeaponOffset);

}

void CWeapon::render(Gdiplus::Graphics* _pDGraphics)
{

}

void CWeapon::render(ID2D1HwndRenderTarget* _pRender)
{
	component_render(_pRender);
}

void CWeapon::ShotMissile(Vec2 _vDir)
{
}

CObject* CWeapon::SpecTarget()
{
	float proximateDistance = 999999.f;
	CObject* pObj = nullptr;
	m_pTarget = nullptr;
	const vector<CObject*>& vecMonObj = m_pCurScene->GetGroupObject(GROUP_TYPE::MONSTER);
	for (const auto MonObj : vecMonObj) {
		if (MonObj->IsDead()) continue;
		//if (MonObj->GetIn)
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

