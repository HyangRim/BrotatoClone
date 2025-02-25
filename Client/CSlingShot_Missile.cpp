#include "pch.h"
#include "CCollider.h"
#include "CScene.h"
#include "CSceneMgr.h"
#include "CDamageUI.h"

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

			//파라미터 세팅. 
			Vec2 objRenderScale = pOtherObj->GetRenderScale();
			wstring damage = std::to_wstring(GetDamage());

			//DamageUI 오브젝트 만들기. 
			CDamageUI* damageText = new CDamageUI;
			damageText->SetPos(pOtherObj->GetPos());
			damageText->SetPivotPos();
			damageText->SetDuration(1.5f);
			if (!GetCritical()) {
				damageText->CreateTextUI(damage, Vec2(-20.f, -objRenderScale.y - 10.f), Vec2(20.f, -objRenderScale.y - 5.f)
					, 16, D2D1::ColorF::White
					, true
					, 1.f, D2D1::ColorF::Black
					, FONT_TYPE::KR
					, TextUIMode::TEXT, 0);

			}
			else {
				damageText->CreateTextUI(damage, Vec2(-20.f, -objRenderScale.y - 10.f), Vec2(20.f, -objRenderScale.y - 5.f)
					, 16, D2D1::ColorF::Yellow
					, true
					, 1.f, D2D1::ColorF::Black
					, FONT_TYPE::KR
					, TextUIMode::TEXT, 0);
			}
			CreateObject(damageText, GROUP_TYPE::IMAGE);
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
