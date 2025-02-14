#include "pch.h"
#include "CMonster.h"

#include "CTimeMgr.h"
#include "CCollider.h"



CMonster::CMonster()
	: m_fSpeed(100.f)
	, m_vCenterPos(Vec2(0.f, 0.f))
	, m_fMaxMoveDistance(50.f)
	, m_iDir(1)
	, m_iHP(5)
{
	CreateCollider();
	GetCollider()->SetScale(Vec2(45.f,	45.f));
}

CMonster::~CMonster()
{

}

void CMonster::update()
{

	return;
	Vec2 vCurPos = GetPos();

	//진행 방향으로 시간당 m_fSpeed만큼 이동. 
	vCurPos.x += fDT * m_fSpeed * m_iDir;

	//배회 거리 기준점을 넘어섰는지 확인. 
	//부들부들 거릴때가 있음... 한번에 크게(105) 넘어섰는데, 
	//다음 프레임에 조금밖에(-1) 못 되돌아 갔을때. 

	float fDistance = abs(m_vCenterPos.x - vCurPos.x) - m_fMaxMoveDistance;
	if (0.f < fDistance) {
		m_iDir *= -1;
		vCurPos.x += fDT * m_fSpeed * m_iDir;
	}

	SetPos(vCurPos);
}


void CMonster::OnCollision(CCollider* _pOther)
{
}

void CMonster::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();

	
	if (pOtherObj->GetName() == L"Missile_Player") {
		m_iHP -= 1;

		if(m_iHP <= 0)
			DeleteObject(this);
	}
	
}

void CMonster::OnCollisionExit(CCollider* _pOther)
{
}

