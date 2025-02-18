#include "pch.h"
#include "CObject.h"

#include "CCollider.h"
#include "CAnimator.h"
#include "CResMgr.h"
#include "CTexture.h"
#include "CRigidbody.h"
#include "CGravity.h"


CObject::CObject()
	: m_vPos{}
	, m_vScale{} 
	, m_pCollider(nullptr)
	, m_pAnimator(nullptr)
	, m_pRigidBody(nullptr)
	, m_pGravity(nullptr)
	, m_bAlive(true)
	, m_bEnable(true)
{

}

CObject::CObject(const CObject& _origin)
	: m_ObjName(_origin.m_ObjName)
	, m_vPos(_origin.m_vPos)
	, m_vScale(_origin.m_vScale)
	, m_pCollider(nullptr)
	, m_bAlive(true)
	, m_bEnable(true)
	, m_pAnimator(nullptr)
	, m_pRigidBody(nullptr)
	, m_pGravity(nullptr)
{
	if (_origin.m_pCollider != nullptr) {
		m_pCollider = new CCollider(*_origin.m_pCollider);
		m_pCollider->m_pOwner = this;
	}

	if (_origin.m_pAnimator != nullptr) {
		m_pAnimator = new CAnimator(*_origin.m_pAnimator);
		m_pAnimator->m_pOwner = this;
	}

	if (_origin.m_pGravity != nullptr) {
		m_pGravity = new CGravity(*_origin.m_pGravity);
		m_pGravity->m_pOwner = this;
	}

	if (_origin.m_pRigidBody != nullptr) {
		m_pRigidBody = new CRigidbody(*_origin.m_pRigidBody);
		m_pRigidBody->m_pOwner = this;
	}

}

CObject::~CObject() {
	if (m_pCollider != nullptr) delete m_pCollider;
	if (m_pAnimator != nullptr) delete m_pAnimator;
	if (m_pGravity != nullptr)  delete m_pGravity;
	if (m_pRigidBody != nullptr)delete m_pRigidBody;
}


void CObject::finalupdate()
{
	if (m_pAnimator) m_pAnimator->finalupdate();
	if (m_pGravity)  m_pGravity->finalupdate();
	if (m_pRigidBody)m_pRigidBody->finalupdate();
	if (m_pCollider) m_pCollider->finalupdate();
}

void CObject::render(HDC _dc)
{
	//ÁøÂ¥ ÁÂÇ¥.
	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(m_vPos);


	
	Rectangle(_dc, (int)(vRenderPos.x - m_vScale.x / 2.f), (int)(vRenderPos.y - m_vScale.y / 2.f),
		(int)(vRenderPos.x + m_vScale.x / 2.f), (int)(vRenderPos.y + m_vScale.y / 2.f));
		
	component_render(_dc);

}

void CObject::component_render(HDC _dc)
{
	if (m_pAnimator != nullptr) m_pAnimator->render(_dc);

	if (m_pCollider != nullptr)	m_pCollider->render(_dc);
}

void CObject::CreateCollider()
{
	m_pCollider = new CCollider;

	m_pCollider->m_pOwner = this;
}

void CObject::CreateAnimator()
{
	m_pAnimator = new CAnimator;
	m_pAnimator->m_pOwner = this;
}

void CObject::CreateRigidBody()
{
	m_pRigidBody = new CRigidbody;
	m_pRigidBody->m_pOwner = this;
}

void CObject::CreateGravity()
{
	m_pGravity = new CGravity;
	m_pGravity->m_pOwner = this;
}
