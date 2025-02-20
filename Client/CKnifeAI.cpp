
#include "pch.h"
#include "CObject.h"
#include "CKnifeAI.h"

CKnifeAI::CKnifeAI()
	: m_vOriginPos()
	, m_vTargetPos()
{
}

CKnifeAI::~CKnifeAI()
{
}



void CKnifeAI::update(float _f)
{
	Vec2 curPos = lerp(m_vOriginPos, m_vTargetPos, _f);
	m_pOwner->SetPos(curPos);
}

