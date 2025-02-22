#include "pch.h"
#include "CRange_TraceState.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CTimeMgr.h"

CRange_TraceState::CRange_TraceState()
	: CState(MON_STATE::TRACE)
{
}

CRange_TraceState::~CRange_TraceState()
{
}

void CRange_TraceState::update()
{
	//타겟팅 된 Player를 쫒아간다. 
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInstance()->GetCurScene()->GetPlayer();
	Vec2 vPlayerPos = pPlayer->GetPos();

	CMonster* pMonster = GetMonster();
	Vec2 vMonsterPos = pMonster->GetPos();

	Vec2 vMonDir = vPlayerPos - vMonsterPos;
	float fLen = vMonDir.Length();								//몬스터랑 플레이어 거리를 재어서


	if (fLen < pMonster->Getinfo().m_fAttRange) {				//사정거리 안으로 들어오면 ATTACK으로 변경. 
		ChangeAIState(GetAI(), MON_STATE::ATTACK);
	}
	else {
		vMonDir.Normalize();
		vMonsterPos += vMonDir * pMonster->Getinfo().m_fSpeed * fDT;//사정거리 밖이면 추적 상태. 
		pMonster->SetPos(vMonsterPos);
	}
}

void CRange_TraceState::Enter()
{
}

void CRange_TraceState::Exit()
{
}


