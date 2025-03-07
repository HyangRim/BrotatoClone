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
	// ������ �ִ´�. 

	Vec2 vPlayerPos = pPlayer->GetPos();



	CMonster* pMonster = GetMonster();
	Vec2 vMonsterPos = pMonster->GetPos();

	Vec2 vDiff = vPlayerPos - vMonsterPos;
	float fLen = vDiff.Length();

	//�÷��̾ ������ �νĹ��� ������ ����. 
	if (fLen < pMonster->Getinfo().m_fRecogRange) {
		ChangeAIState(GetAI(), MON_STATE::TRACE);
	}
	// �÷��̾��� ��ġ�� �޾ƿͼ� ������ ���� �ȿ� ������
	// ���� ���� ��ȯ. 
}

void CIdleState::Enter()
{
}

void CIdleState::Exit()
{
}