#pragma once
#include "CObject.h"

class CMonster :
    public CObject
{
private:
    float       m_fSpeed;
    Vec2        m_vCenterPos;
    float       m_fMaxMoveDistance;
    int         m_iDir; // 1, -1 (1 : Right, -1 : Left);

    int         m_iHP;

public:
    virtual void update();
    float GetSpeed() { return m_fSpeed; }
    void SetSpeed(float _f) { m_fSpeed = _f; }

    void SetCenterPos(Vec2 _vPos) { m_vCenterPos = _vPos; }
    void SetMaxMoveDistance(float _f) { m_fMaxMoveDistance = _f; }


public:
    virtual void OnCollision(CCollider* _pOther);
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);


private:
    CLONE(CMonster)


public:
    CMonster();
    ~CMonster();
};