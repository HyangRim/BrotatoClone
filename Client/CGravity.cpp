
#include "pch.h"
#include "CGravity.h"
#include "CObject.h"
#include "CRigidbody.h"



CGravity::CGravity()
	: m_pOwner(nullptr)
	, m_bGround(false)
{
}

CGravity::~CGravity()
{
}


void CGravity::SetGround(bool _b)
{
	m_bGround = _b;
	if (m_bGround) {
		Vec2 vV = m_pOwner->GetRigidbody()->GetVelocity();
		m_pOwner->GetRigidbody()->SetVelocity(Vec2(vV.x, 0.f));
	}
}

void CGravity::finalupdate()
{
	m_pOwner->GetRigidbody()->SetAccelAlpha(Vec2(0.f, 98.f));
}