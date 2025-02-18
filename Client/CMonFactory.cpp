#include "pch.h"
#include "CMonFactory.h"

#include "CRigidbody.h"

#include "CMonster.h"
#include "AI.h"
#include "CIdleState.h"
#include "CTraceState.h"


CMonster* CMonFactory::CreateMonster(MON_TYPE _eType, Vec2 _vPos)
{

	CMonster* pMon = nullptr;
	switch (_eType) {
	case MON_TYPE::NORMAL:
	{
		pMon = new CMonster;
		pMon->SetPos(_vPos);

		tMonInfo info = {};
		info.m_fAtt = 1;
		info.m_fAttRange = 50.f;
		info.m_fRecogRange = 300.f;
		info.m_iHP = 100;
		info.m_fSpeed = 150.f;

		pMon->SetMonInfo(info);
		pMon->CreateRigidBody();
		pMon->GetRigidbody()->SetMass(1.f);

		AI* pAI = new AI;
		pAI->AddState(new CIdleState);
		pAI->AddState(new CTraceState);
		pAI->SetCurState(MON_STATE::IDLE);

		pMon->SetAI(pAI);
	}
		break;

	case MON_TYPE::RANGE:

		break;
	}


	assert(pMon);
	return pMon;
}
