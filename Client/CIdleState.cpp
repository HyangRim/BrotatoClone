#include "pch.h"
#include "CIdleState.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CPlayer.h"
#include "CMonster.h"



CIdleState::CIdleState()
	: CState(MON_STATE::IDLE)
{
}

CIdleState::~CIdleState()
{
}

void CIdleState::update()
{
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInstance()->GetPlayer();
	// 가만히 있는다. 

	Vec2 vPlayerPos = pPlayer->GetPos();



	CMonster* pMonster = GetMonster();
	Vec2 vMonsterPos = pMonster->GetPos();

	Vec2 vDiff = vPlayerPos - vMonsterPos;
	float fLen = vDiff.Length();

	//플레이어가 몬스터의 인식범위 안으로 진입. 
	if (fLen < pMonster->Getinfo().m_fRecogRange) {
		ChangeAIState(GetAI(), MON_STATE::TRACE);
	}
	// 플레이어의 위치를 받아와서 몬스터의 범위 안에 들어오면
	// 추적 상대료 전환. 
}

void CIdleState::Enter()
{
}

void CIdleState::Exit()
{
}