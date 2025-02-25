#include "pch.h"
#include "CRange_AttackState.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CMissile.h"
#include "CTimeMgr.h"



CRange_AttackState::CRange_AttackState()
	: CState(MON_STATE::ATTACK)
	, m_fShotDuration(1.2f)
	, m_fShotCoolTime(0)
{
}

CRange_AttackState::~CRange_AttackState()
{
}

void CRange_AttackState::Enter()
{
	m_fShotCoolTime = 0.f;
	//���� �� �� ���, �ٷ� IDLE�� ��ȯ ����. 
	m_vPlayerPos = ((CPlayer*)CSceneMgr::GetInstance()->GetCurScene()->GetPlayer())->GetPos();
	m_vMonsterPos = GetMonster()->GetPos();
	m_vMissileDir = (m_vPlayerPos - m_vMonsterPos).Normalize();
}

void CRange_AttackState::Exit()
{
	m_fShotCoolTime = 0.f;
}

void CRange_AttackState::update()
{
	m_fShotCoolTime += fDT;
	if (m_fShotCoolTime < m_fShotDuration) {
		//���� �Ӱ� ��ȭ ���Ѿ���. 

		return;
	}
	//���� �ð� ������ �̻��� �߻�. 
	CreateMissile();

	ChangeAIState(GetAI(), MON_STATE::IDLE);
}

void CRange_AttackState::CreateMissile()
{
	CMissile* pMissile = new CMissile;
	pMissile->SetName(L"Missile_Monster");
	pMissile->SetPos(m_vMonsterPos);
	pMissile->SetScale(Vec2(20.f, 20.f));
	pMissile->SetDir(m_vMissileDir);

	CreateObject(pMissile, GROUP_TYPE::PROJ_MONSTER);
}
