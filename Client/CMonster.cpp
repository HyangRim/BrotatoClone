#include "pch.h"
#include "CMonster.h"
#include "CWeapon.h"
#include "CMissile.h"
#include "CKnife.h"

#include "AI.h"
#include "CTimeMgr.h"
#include "CCollider.h"
#include "CMonFactory.h"
#include "CScene.h"
#include "CSceneMgr.h"




CMonster::CMonster()
	: m_tInfo{}
	, m_pAI(nullptr)
{
	CreateCollider();
	GetCollider()->SetScale(Vec2(45.f,	45.f));
}

CMonster::~CMonster()
{
	if (nullptr != m_pAI)
		delete m_pAI;

	if (m_tInfo.m_eMonType != MON_TYPE::DROP_ITEM) {
		//DropItem 만들기. 
		Vec2 vPos = GetPos();
		CMonster* pMon = CMonFactory::CreateMonster(MON_TYPE::DROP_ITEM, vPos);
		CreateObject(pMon, GROUP_TYPE::DROP_ITEM);
	}
}

void CMonster::update()
{
	CObject::update();
	if(nullptr != m_pAI) m_pAI->update();

	//Flip체크. 
	
	Vec2 playerPos = CSceneMgr::GetInstance()->GetCurScene()->GetPlayer()->GetPos();
	Vec2 monPos = GetPos();
	Vec2 Dir = playerPos - monPos;

	//이미지 Flip 체크. 여기다가 이걸 넣는게 맞나?
	if (Dir.x > 0) {//플레이어가 몬스터보다 오른쪽에 있을 때. 
		SetFlipX(false);
	}
	else {//플레이어가 몬스터보다 왼쪽에 있을 때. 
		SetFlipX(true);
	}
}


void CMonster::SetAI(AI* _AI)
{
	m_pAI = _AI;
	m_pAI->m_pOwner = this;
}

void CMonster::TakeDamaged(int _iDamage)
{
}


void CMonster::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();

	
	if (pOtherObj->GetName() == L"Missile_Player") {

		m_tInfo.m_iHP -= ((CMissile*)pOtherObj)->GetDamage();

		if (m_tInfo.m_iHP <= 0) {

			DeleteObject(this);
		}
	}
	if (pOtherObj->GetName() == L"Knife") {
		m_tInfo.m_iHP -= ((CKnife*)pOtherObj)->GetDamage();

		if (m_tInfo.m_iHP <= 0) {
			DeleteObject(this);
		}
	}
	
}


