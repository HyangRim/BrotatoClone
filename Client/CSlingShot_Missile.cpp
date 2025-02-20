#include "pch.h"
#include "CCollider.h"
#include "CScene.h"
#include "CSceneMgr.h"

#include "CSlingShot_Missile.h"



CSlingShot_Missile::CSlingShot_Missile()
	: m_iBounce(0)
{
}

CSlingShot_Missile::~CSlingShot_Missile()
{
}

void CSlingShot_Missile::update()
{
	CMissile::update();
}


void CSlingShot_Missile::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();


	if (pOtherObj->GetName() == L"Monster") {

		if (m_iBounce > 0) {
			m_iBounce -= 1;
			ReDirection(pOtherObj);
		}
		else {
			DeleteObject(this);
		}
	}
}

void CSlingShot_Missile::ReDirection(CObject* _ReachObj)
{
	const vector<CObject*>& vecMonObj = CSceneMgr::GetInstance()->GetCurScene()->GetGroupObject(GROUP_TYPE::MONSTER);
	CObject* pTarget = nullptr;
	for (const auto MonObj : vecMonObj) {
		if (MonObj->IsDead() || _ReachObj == MonObj) continue;
		pTarget = MonObj;
		break;
	}

	if (nullptr != pTarget) {
		Vec2 vDir = (pTarget->GetPos()) - GetPos();
		SetDir(vDir.Normalize());
	}
	else {
		DeleteObject(this);
	}
}
