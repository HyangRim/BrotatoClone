#pragma once
#include "CObject.h"


struct tMonInfo {
    MON_TYPE    m_eMonType;
    float       m_fSpeed;           //속도. 
    int         m_iHP;              //체력
    float       m_fRecogRange;      //인지 범위
    float       m_fAttRange;        //공격 범위
    int         m_fAtt;             //공격력
};

class AI;
class CMonster :
    public CObject
{
private:
    tMonInfo    m_tInfo;
    AI*         m_pAI;

public:
    float GetSpeed() { return m_tInfo.m_fSpeed; }
    void SetSpeed(float _f) { m_tInfo.m_fSpeed = _f; }
    void SetAI(AI* _AI);
    const tMonInfo& Getinfo() { return m_tInfo; }

    void TakeDamaged(int _iDamage);


private:
    void KnifeDamageUI();


private:
    void SetMonInfo(const tMonInfo& _info) {
        m_tInfo = _info;
    }

public:
    virtual void OnCollisionEnter(CCollider* _pOther);


protected:
    virtual void update();
    CLONE(CMonster)


public:
    CMonster();
    virtual ~CMonster();

    friend class CMonFactory;
};