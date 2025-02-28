#include "pch.h"
#include "CMonFactory.h"

#include "CRigidbody.h"

#include "CMonster.h"
#include "CNormal_Monster.h"
#include "CRange_Monster.h"
#include "CDropItem.h"
#include "AI.h"
#include "CIdleState.h"
#include "CTraceState.h"
#include "CRange_TraceState.h"
#include "CRange_AttackState.h"

#include "Direct2DMgr.h"
#include "CImage.h"

float CMonFactory::m_fRecog = 250.f;
float CMonFactory::m_fSpeed = 250.f;

CMonster* CMonFactory::CreateMonster(MON_TYPE _eType, Vec2 _vPos)
{
	CMonster* pMon = nullptr;
	switch (_eType) {
	case MON_TYPE::NORMAL:
	{
		pMon = new CNormal_Monster;
		pMon->SetPos(_vPos);
		pMon->SetName(L"Monster");
		pMon->SetScale(Vec2(40.f, 40.f));
		pMon->SetWaveDuration(1.f);

		tMonInfo info = {};
		info.m_eMonType = _eType;
		info.m_fAtt = 1;
		info.m_fAttRange = 50.f;
		info.m_fRecogRange = 300.f;
		info.m_iHP = 100;
		info.m_fSpeed = 150.f;

		pMon->SetMonInfo(info);
		//pMon->CreateRigidBody();
		//pMon->GetRigidbody()->SetMass(1.f);

		AI* pAI = new AI;
		pAI->AddState(new CIdleState);
		pAI->AddState(new CTraceState);
		pAI->SetCurState(MON_STATE::IDLE);

		pMon->SetAI(pAI);


		pMon->CreateImage();
		pMon->AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"baby_alien"));
		//pMon->AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"NormalEnemy"));
		//pMon->GetImage()->SetBitmap(Direct2DMgr::GetInstance()->GetStoredBitmap(L"NormalEnemy"));
	}
		break;

	case MON_TYPE::RANGE:
	{
		pMon = new CRange_Monster;
		pMon->SetPos(_vPos);
		pMon->SetName(L"Monster");
		pMon->SetScale(Vec2(40.f, 40.f));
		pMon->SetWaveDuration(1.f);

		tMonInfo info = {};
		info.m_eMonType = _eType;
		info.m_fAtt = 1;
		info.m_fAttRange = 250.f;
		info.m_fRecogRange = 450.f;
		info.m_iHP = 100;
		info.m_fSpeed = 100.f;

		pMon->SetMonInfo(info);
		//pMon->CreateRigidBody();
		//pMon->GetRigidbody()->SetMass(1.f);

		AI* pAI = new AI;
		pAI->AddState(new CIdleState);
		pAI->AddState(new CRange_TraceState);
		pAI->AddState(new CRange_AttackState);
		pAI->SetCurState(MON_STATE::IDLE);

		pMon->SetAI(pAI);
		pMon->CreateImage();
		pMon->AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"spitter"));
		//pMon->AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"RangeEnemy"));
		//pMon->GetImage()->SetBitmap(Direct2DMgr::GetInstance()->GetStoredBitmap(L"RangeEnemy"));
	}
		break;

	case MON_TYPE::DROP_ITEM:
		pMon = new CDropItem;
		pMon->SetPos(_vPos);
		pMon->SetScale(Vec2(15.f, 15.f));

		tMonInfo info = {};
		info.m_eMonType = _eType;
		info.m_fAtt = 1;
		info.m_fAttRange = 1.f;
		info.m_fRecogRange = CMonFactory::m_fRecog;
		info.m_iHP = 9999999;
		info.m_fSpeed = CMonFactory::m_fSpeed;

		pMon->SetMonInfo(info);

		AI* pAI = new AI;
		pAI->AddState(new CIdleState);
		pAI->AddState(new CTraceState);
		pAI->SetCurState(MON_STATE::IDLE);

		pMon->SetAI(pAI);

		pMon->CreateImage();

		int dropItemImage = distribution(rng) % 11;
		wstring dropItemImageKey = dropItemKey + std::to_wstring(dropItemImage);
		pMon->AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(dropItemImageKey));
	}


	assert(pMon);
	return pMon;
}
