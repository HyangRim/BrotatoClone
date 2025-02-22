#pragma once
#include "CState.h"
class CRange_AttackState :
    public CState
{
private:
    float m_fShotDuration;
    float m_fShotCoolTime;

    Vec2 m_vPlayerPos;
    Vec2 m_vMonsterPos;
    Vec2 m_vMissileDir;

public:
    virtual void update();
    virtual void Enter();
    virtual void Exit();

private:
    void CreateMissile();

public:
    CRange_AttackState();
    ~CRange_AttackState();
};

