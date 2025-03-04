
#include "pch.h"
#include "CObject.h"
#include "CKnifeAI.h"
#include "CSoundMgr.h"

CKnifeAI::CKnifeAI()
	: m_vOriginPos()
	, m_vTargetPos()
{
	int randomRc = distribution(rng) % 2;

	if (randomRc) {
		CSoundMgr::GetInstance()->Play(L"knife_harsh_1");
	}
	else {
		CSoundMgr::GetInstance()->Play(L"knife_harsh_2");
	}
}

CKnifeAI::~CKnifeAI()
{
}



void CKnifeAI::update(float _f)
{
	Vec2 curPos = lerp(m_vOriginPos, m_vTargetPos, _f);
	m_pOwner->SetPos(curPos);
}

