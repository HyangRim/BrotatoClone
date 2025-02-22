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
	//Ÿ���� �� Player�� �i�ư���. 
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInstance()->GetCurScene()->GetPlayer();
	Vec2 vPlayerPos = pPlayer->GetPos();

	CMonster* pMonster = GetMonster();
	Vec2 vMonsterPos = pMonster->GetPos();

	Vec2 vMonDir = vPlayerPos - vMonsterPos;
	float fLen = vMonDir.Length();								//���Ͷ� �÷��̾� �Ÿ��� ��


	if (fLen < pMonster->Getinfo().m_fAttRange) {				//�����Ÿ� ������ ������ ATTACK���� ����. 
		ChangeAIState(GetAI(), MON_STATE::ATTACK);
	}
	else {
		vMonDir.Normalize();
		vMonsterPos += vMonDir * pMonster->Getinfo().m_fSpeed * fDT;//�����Ÿ� ���̸� ���� ����. 
		pMonster->SetPos(vMonsterPos);
	}
}

void CRange_TraceState::Enter()
{
}

void CRange_TraceState::Exit()
{
}


