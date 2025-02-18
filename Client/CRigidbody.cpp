#include "pch.h"
#include "CRigidbody.h"
#include "CTimeMgr.h"
#include "CObject.h"




CRigidbody::CRigidbody()
	: m_pOwner(nullptr)
	, m_fMass(1.f)
	, m_fFricCoeff(150.f)
	, m_vMaxVelocity(Vec2(200.f, 200.f))
{
}

CRigidbody::~CRigidbody()
{
}


void CRigidbody::finalupdate()
{
	
	float fForce = m_vForce.Length();		//힘의 크기

	if (0.f != fForce) {
		m_vForce.Normalize();

		float m_fAccel = fForce / m_fMass;		//최종적인 가속도. 
		m_vAccel = m_vForce * m_fAccel;
	}

	m_vAccel += m_vAccelA;						//추가 가속도
	m_vVelocity += m_vAccel * fDT;				//속도. 

	//마찰력에 의한 반대방향으로의 가속도 적용.

	if (!m_vVelocity.IsZero()) {
		Vec2 vFircDir = -m_vVelocity;
		vFircDir.Normalize();
		Vec2 vFriction = vFircDir * m_fFricCoeff * fDT;

		if (m_vVelocity.Length() <= vFriction.Length()) {
			//마찰 가속도가 본래 속도보다 더 큰 경우.
			m_vVelocity = Vec2(0.f, 0.f);
		}
		else {
			m_vVelocity += vFriction;
		}
	}


	//속도 제한 검사. 
	//축 별로 따로 검사해야함. 

	//X축 속도 제한 검사. 
	if (m_vMaxVelocity.x < abs(m_vVelocity.x)) {
		m_vVelocity.x /= abs(m_vVelocity.x);
		m_vVelocity.x *= m_vMaxVelocity.x;
	}

	//Y축 속도 제한 검사. 
	if (m_vMaxVelocity.y < abs(m_vVelocity.y)) {
		m_vVelocity.y /= abs(m_vVelocity.y);
		m_vVelocity.y *= m_vMaxVelocity.y;
	}
	//속도에 따른 이동. 
	Move();

	//힘 초기화. 
	m_vForce = Vec2(0.f, 0.f);

	//가속도 초기화
	m_vAccel = Vec2(0.f, 0.f);
	
	//추가 가속도 초기화
	m_vAccelA = Vec2(0.f, 0.f);
}

void CRigidbody::update_gravity()
{
	AddForce(Vec2(0.f, 500.f));
}

void CRigidbody::Move()
{
	float fSpeed = m_vVelocity.Length();

	if (0.f != fSpeed) {
		Vec2 vPos = m_pOwner->GetPos();
		vPos += m_vVelocity * fDT;

		m_pOwner->SetPos(vPos);
	}
}