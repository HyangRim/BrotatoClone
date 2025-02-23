#include "pch.h"
#include "CObject.h"

#include "CTimeMgr.h"

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
	, m_fWaveDuration(0.f)
	, m_fWaveElapsed(0.f)
{
	m_vRenderScale = m_vScale;
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
	, m_fWaveDuration(_origin.m_fWaveDuration)
	, m_fWaveElapsed(0.f)
{
	m_vRenderScale = m_vScale;
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


void CObject::update()
{
	ShakeScale();

	m_vPrevPos = m_vPos;
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


	
	Rectangle(_dc, (int)(vRenderPos.x - m_vRenderScale.x / 2.f), (int)(vRenderPos.y - m_vRenderScale.y / 2.f),
		(int)(vRenderPos.x + m_vRenderScale.x / 2.f), (int)(vRenderPos.y + m_vRenderScale.y / 2.f));
		
	component_render(_dc);

}

void CObject::render(Gdiplus::Graphics* _pDGraphics)
{
	//ÁøÂ¥ ÁÂÇ¥.
	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(m_vPos);
	Gdiplus::Pen pen(Gdiplus::Color(255, 0, 0, 0), 1.0f);
	Gdiplus::Rect rect(
		(int)(vRenderPos.x - m_vRenderScale.x / 2.f),
		(int)(vRenderPos.y - m_vRenderScale.y / 2.f),
		(int)m_vRenderScale.x,
		(int)m_vRenderScale.y
	);

	_pDGraphics->DrawRectangle(&pen, rect);
	component_render(_pDGraphics);
}

void CObject::component_render(HDC _dc)
{
	if (m_pAnimator != nullptr) m_pAnimator->render(_dc);

	if (m_pCollider != nullptr)	m_pCollider->render(_dc);
}

void CObject::component_render(Gdiplus::Graphics* _pDGraphics)
{
	if (m_pAnimator != nullptr) m_pAnimator->render(_pDGraphics);

	//if (m_pCollider != nullptr)	m_pCollider->render(_pDGraphics);
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


void CObject::ShakeScale()
{
	

	Vec2 OriginalScale = GetScale();
	Vec2 ScaleWave = OriginalScale * 0.3f;
	float waveScale = 1.f;
	if (IsMove()) {
		waveScale = 1.5f;
	}
	m_fWaveElapsed += (fDT * waveScale);

	float angle = 2 * PI * m_fWaveElapsed / m_fWaveDuration;
	float delta = 0.2f * sin(angle);

	m_vRenderScale.x = m_vScale.x + ((1.f + delta) * ScaleWave.x);
	m_vRenderScale.y = m_vScale.y + ((1.f - delta) * ScaleWave.y);

	if (m_fWaveElapsed > m_fWaveDuration) {
		m_fWaveElapsed = 0.f;
	}
}